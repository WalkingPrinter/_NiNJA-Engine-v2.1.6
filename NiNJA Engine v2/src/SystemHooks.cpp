#include "SystemHooks.h"
#include "Globals.h"
#include "SecureStrings.h"

//EX_TITLE_TERMINATE_REGISTRATION terminateRegistration = { titleTerminateHandler, 0x7C800000 };
Detour<VOID> ExTerminateTitleProcess_Detour;
DWORD dwExtraThreadWait = 0;

SystemHooks::SystemHooks() {
	bInitialized = FALSE;
}

SystemHooks::~SystemHooks() {
}

VOID titleTerminateHandler() {
	
}

VOID ExTerminateTitleProcess_Hook() {
	DbgPrint("Hit!");
	// Run our terminate function
	CG::Terminate();

	// After the cheats have signaled that they have successfully shutdown, unload the title
	ExTerminateTitleProcess_Detour.CallOriginal(0);
}


Detour<VOID> XamLoaderLaunchTitle_Detour;
VOID XamLoaderLaunchTitle_Hook(PCHAR r3, int r4) {
	DbgPrint("Hit!");
	// Run our terminate function
	CG::Terminate();

	XamLoaderLaunchTitle(r3, r4);
}

BOOL SystemHooks::Apply() {
	if (!bInitialized) {
		//Setup Title Termination Notify Callback
		//ExRegisterTitleTerminateNotification(&terminateRegistration, TRUE);

		// Setup our detours
		if (!ExTerminateTitleProcess_Detour.SetupDetour("xboxkrnl.exe", 0x1A, &ExTerminateTitleProcess_Hook)) {
			DbgPrint("Failed to hook ExTerminateTitleProcess! Cannot continue!");
			return FALSE;
		}


		if (!XamLoaderLaunchTitle_Detour.SetupDetour(0x82000000, ES("xam.xex"), 0x01A4, &XamLoaderLaunchTitle_Hook)) {
		//	printf("FUCK\n");
		}

		DbgPrint("Applied detours!");
		bInitialized = TRUE;
		return TRUE;
	}
	else {
		DbgPrint("TitleHooks already initialized!");
	}
	return FALSE;
}

VOID SystemHooks::Remove() {
	if (bInitialized) {
		// Take down our detours
		ExTerminateTitleProcess_Detour.TakeDownDetour();

		// We don't need to unhook this one, actually if we do it causes crashes
		//XamLoaderLaunchTitle_Detour.TakeDownDetour();

		Sleep(1000);

		// Delete our detours
		//SAFE_DELETE(ExTerminateTitleProcess_Detour);

		DbgPrint("Removed detours!");
		bInitialized = FALSE;
	}
	else {
		DbgPrint("TitleHooks have not been initialized!");
	}
}
