#include "Globals.h"
#include "Debug.h"
#include "MailMan.h"
#include "Game.h"
#include "SystemHooks.h"
#include "SecureStrings.h"
#include "Utilities.h"
#include <xhttp.h>
#include "HttpRequest.h"
#include <iostream>

SystemHooks pSystemHooks;

bool bWasSignedIn = false;
VOID ConstantLoop() {
	// Initialize our globals
	NiNJA.Initialize();

	#if defined(DUMP_BINS) && !defined(PATCH_SVR)
		Not(NiNJA.cPatches)->DumpPatchBinary();
	#endif

	if (!XHttpStartup(0, NULL)) {
		DbgPrint("Failed to init xhttp!");
	}

	if (GetTitleId() != __TITLEID__) {
		DbgPrint("Unsupported title!");
		NiNJA.bTerminateProcess = TRUE;
		goto jUnload;
	}

	if (!pSystemHooks.Apply()) {
		DbgPrint("Failed to apply system hooks!");
		NiNJA.bTerminateProcess = TRUE;
		goto jUnload;
	}

	if (NiNJA.bIsDevkit) {
		// Disable stupid assertions on devkit
		*(int*)0x8177c120 = 0x60000000;
		*(int*)0x8177cbf0 = 0x60000000;
	}

	if (MountPath(MOUNT_NAME, GetMountPath(), FALSE) == 0) {
		DbgPrint("Mounted path successfully!");
	}

	DbgPrint("Started!");

	//setup a memory manager to store web requests and decrypted strings which need to remain decrypted in memory
	PBYTE tmpBuff = (PBYTE)XPhysicalAlloc(0x6000, MAXULONG_PTR, 0, PAGE_READWRITE);
	sManager.SetupManager((DWORD)tmpBuff, 0x6000);

	DbgPrint("AddressOf sManager: 0x%X", tmpBuff);

#if defined(PATCH_SVR)
	XBLSMsgr *Messenger = new XBLSMsgr();
	DbgPrint("Initializing [MSGR] SubSystem");

	// Find ptr to mailbox
	Messenger->SetMsgBox(MailManGetBox(Messenger->myIdent));

	// Try to init our patches
	DbgPrint("Patch initialization %s!", Messenger->InitPatches() ? "success" : "failure");

	// Clean up our messenger object
	SAFE_DELETE(Messenger);
#endif

	// Make sure we got our patches successfully... if not, unload.
	if (Not(NiNJA.cPatches)->bHasPatches) {
		// Initialize a Game object
		Game gCurrentGame;

		printf("[WRN] If you have Xbox Watson connected, disconnect it now. Ignoring this warning will just lead to shit breaking. You have been warned!\n");

		while (!NiNJA.bTerminateProcess) {
			// Perform any non-blocking operations here
			NiNJA.dwCurrentTitle = GetTitleId();
			NiNJA.dwTickCount = GetTickCount();
			NiNJA.bSignedIntoLive = XUserGetSigninState(0) == eXUserSigninState_SignedInToLive;
			if (NiNJA.bSignedIntoLive && !bWasSignedIn) {
				bWasSignedIn = true;
				CG::bResetGamertag = true;
			}
			else if (!NiNJA.bSignedIntoLive) {
				bWasSignedIn = false;
				CG::bResetGamertag = true;
			}
			if (NiNJA.dwCurrentTitle == __TITLEID__ && !NiNJA.bShutdownCheats) {
				if (!gCurrentGame.bInitialized) {
					if (!gCurrentGame.Initialize()) {
						// We failed to initialize the game for some reason... wait a second before we try again.
						Sleep(1000);
					}
				}
				else {
					gCurrentGame.Run();
				}
			}
			else {
				DbgPrint("Changing title! Shutdown initiated!");
				// Signal that our process is being terminated
				break;
			}
			Sleep(33);
		}

		// Shutdown our game modifications before we unload our xex
		if (gCurrentGame.bInitialized) {
			gCurrentGame.Shutdown();
		}
	}

	// Wait for our extra threads to complete
	while (NiNJA.bSoundLoopRunning || CG::bTitleThreadRunning) {
		Sleep(33);
	}

	// Give our extra thread 1 second to shut down before we just move on anyway
	DWORD dwExtraThreadWait = GetTickCount() + 1000;
	while (CG::bExtraThreadRunning && GetTickCount() < dwExtraThreadWait) {
		Sleep(33);
	}

	// Free all strings we decrypted and stored in memory
	XSAFE_FREE(tmpBuff);

jUnload:
	bIgnoreHttpRequests = TRUE;

	// Take down our system hooks
	if (pSystemHooks.bInitialized)
		pSystemHooks.Remove();

	// Memory cleanup
	NiNJA.Shutdown();

	DbgPrint("Exiting!");

	// Mark our constant loop thread as having exited before we leave the thread
	NiNJA.bConstantLoopRunning = FALSE;

	// If we havent hit a DLL_PROCESS_DETACH event in DllMain before getting to this point, we need to explicitly unload our module.
	if (!NiNJA.bExternUnload) {
		*(WORD*)((DWORD)NiNJA.hME + 64) = 1;
		DWORD exitCode;
		GetExitCodeThread(GetCurrentThread(), &exitCode);
		XexUnloadImageAndExitThread(NiNJA.hME, (HANDLE)exitCode);
	}
}

VOID HiddenThreadStartup() {
	NiNJA.bIsDevkit = *(BOOL*)0x8E038610 & 0x8000 ? FALSE : TRUE;
	DbgPrint("Running on %s kernel!", NiNJA.bIsDevkit ? "dev" : "retail");

	NiNJA.pDataTableEntry = (PLDR_DATA_TABLE_ENTRY)NiNJA.hME;

	NiNJA.bConstantLoopRunning = TRUE;
	if (!LaunchSysThread((LPTHREAD_START_ROUTINE)ConstantLoop, NULL, NULL, NULL, THREAD_PRIORITY_BELOW_NORMAL)) {
		NiNJA.bConstantLoopRunning = FALSE;
		DbgPrint("Failed to load main thread!");
		return;
	}
	DbgPrint("Success!");
}



HRESULT Initialize() {
	DWORD startFunc = GetModuleImportAddr(NiNJA.hME, GetModuleHandle("xboxkrnl.exe"), 25);
	//DbgPrint("StartFunc: %08X  %08X", startFunc, *(DWORD*)startFunc);
	if (!startFunc) {
		return E_FAIL;
	}

#ifdef PATCH_SVR
	*(DWORD*)startFunc = (DWORD)HiddenThreadStartup;
#else
	HiddenThreadStartup();
#endif
	return ERROR_SUCCESS;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	// Check how we want to enter
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			DbgPrint("Loaded!");

			// Store our module handle for later use
			NiNJA.hME = hModule;
			SetOurHandle(hModule);

			// Try to initialize NiNJA engine
			return Initialize() == ERROR_SUCCESS;
		}
		case DLL_PROCESS_DETACH: {
			NiNJA.bExternUnload = TRUE;
			NiNJA.bTerminateProcess = TRUE;

			while (NiNJA.bConstantLoopRunning) {
				Sleep(33);
			}
			Sleep(500);

			DbgPrint("Terminated!");
			return TRUE;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}