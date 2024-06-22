#include "Game.h"
#include "Globals.h"
#include "GeoLocationParser.h"
#include "HudElemProcessing.h"
#include "UI.h"
#include "Debug.h"
#include "Strings.h"


Game::Game() {
	bInitialized = FALSE;
	//tHooks = new TitleHooks();
}

Game::~Game() {
	//SAFE_DELETE(tHooks);
}

/* Setup all our hooks and structs which will be used for everything on the game (drawing, cheats, etc) */
BOOL Game::Initialize() {
	// Make sure we're on the right title update
	PLDR_DATA_TABLE_ENTRY Title = *XexExecutableModuleHandle;
	if (Title->TimeDateStamp != __TIMEDATESTAMP__) {
		DbgPrint("Invalid game version (version %X)", Title->TimeDateStamp);
		return FALSE;
	}

	// Initialize va function
	GameFuncs::va = (CONST PCHAR(__cdecl *)(CONST PCHAR, ...))decryptDWORD(Not(NiNJA.cPatches)->CoD.va);

	//Initialize static memory patches
	DWORD numPatches = 0, seek = 0;
	DbgPrint("Attempting to apply %i memory patch(es)", decryptDWORD(Not(NiNJA.cPatches)->CoD.NumPatches));
	for (unsigned int i = 0; i < decryptDWORD(Not(NiNJA.cPatches)->CoD.NumPatches); i++) {
		DWORD Size = decryptDWORD(*(DWORD*)(Not(NiNJA.cPatches)->CoD.Patches + seek));
		DWORD Address = decryptDWORD(*(DWORD*)(Not(NiNJA.cPatches)->CoD.Patches + seek + 0x4));
		if (Address == 0x11e23e7c) {
			Address = (DWORD)&bXorKey;
		}
		DWORD *P;
		if (sManager.alloc(Size, &P)) {
			for (unsigned int v = 0; v < Size / 4; v++)
				P[v] = decryptDWORD(*(DWORD*)(Not(NiNJA.cPatches)->CoD.Patches + seek + 0x8 + (v * 4)));
			memcpy((PVOID)Address, (PVOID)P, Size);
			seek += Size + 0x8;
			numPatches++;
			sManager.free(P);
		}
		else {
			DbgPrint("Failed to apply memory patches! Couldn't allocate memory!");
			return FALSE;
		}

	}
	DbgPrint("Successfully applied %i memory patch%s", numPatches, numPatches > 1 ? "es." : ".");

#if defined(BO2)
	// Initialize some autowall functions
	Autowall::Initialize();
#endif

	// Try to apply our title hooks
	if (!TitleHooks::Apply()) {
		DbgPrint("Failed to apply hooks! Removing any that were successful!");
		TitleHooks::Remove(TRUE);
		return FALSE;
	}

	DbgPrint("Game initialized!");
	NiNJA.bGameInitialized = TRUE;
	bInitialized = TRUE;
	return TRUE;
}

DWORD dwTestNum = 2;
DWORD dwLastSoundTick = 0;
bool bDownloadPack = true, bCheckAPI = true;
/* Any operation that needs to run from a system thread goes here */
VOID Game::Run() {
	// After our UI has been initialized, read/save our menu options if the flag to do so has been set
	if (UI::bInitialized) {
		if (CG::bLaunchCustomGTEntry) {
			if (GameFuncs::EnterCustomGamertag(CG::pCustomGTMenuOption)) {
				if (UI::mManager.dwCurrentMenu == MENU_GAMERTAG_EDITOR) {
					UI::mManager.bUpOneLevel = true;
				}
			}
			CG::bLaunchCustomGTEntry = false;
		}

		for (DWORD i = 0; i < __MAXCLIENTS__; i++) {
			if (CG::cEntities.List[i].pExt) {
				if (CG::cEntities.List[i].pExt->bGrabGeoIPInfo) {
					// Only try to put in a geo ip request if their ip is populated
					if (CG::cEntities.List[i].pExt->cIP.S_un.S_addr != 0) {
						int iFailedIPLookups = 0, iMaxAttempts = 3;
					jRetry:
						if (iFailedIPLookups < iMaxAttempts) {
							//printf("Attempting to lookup IP %i.%i.%i.%i (UsingAltApi=%s)\n", CG::cEntities.List[i].pExt->cIP.S_un.S_un_b.s_b1, CG::cEntities.List[i].pExt->cIP.S_un.S_un_b.s_b2, CG::cEntities.List[i].pExt->cIP.S_un.S_un_b.s_b3, CG::cEntities.List[i].pExt->cIP.S_un.S_un_b.s_b4, CG::bUseAltGeoIPApi ? "TRUE" : "FALSE");

							GeoIpInfo outInfo;
							GeoIpParser::GeoIpParser cParser(CG::cEntities.List[i].pExt->cIP, &outInfo);
							if (cParser.success) {
								CG::cEntities.List[i].pExt->cGeoIPInfo = outInfo;

								if (CG::bLogIPs) {
									GameFuncs::WriteIPLogEntry(i);
								}
								//printf("%s lives in %s, %s", CG::cEntities.List[i].pExt->GetName(), outInfo.city, outInfo.stateFull);
								CG::cEntities.List[i].pExt->bGrabGeoIPInfo = false;
								break;
							}
							else {
								iFailedIPLookups++;
								DbgPrint("HTTP request failed! Trying again in a few seconds...");
								Sleep(2000);
								goto jRetry;
							}
						}
					}
					CG::cEntities.List[i].pExt->bGrabGeoIPInfo = false;
				}
			}
		}

		// Try to read our config
		if (UI::mManager.bShouldRead) {
			if (UI::mManager.Read()) {
				NiNJA.bHasReadConfig = TRUE;
			}
		}
		// If we weren't trying to read it, write it
		else if (UI::mManager.bShouldSave) {
			if (!UI::mManager.Save()) {
				ShowMessageBox(L"Failed to save Engine.ini!", L"An error occured when trying to save Engine.ini! Free up some space on the storage device where you load NiNJA.xex from, then try again!", XMB_ERRORICON);
			}
		}
		// Try to download our custom sound pack
		else if (bDownloadPack) {
			DbgPrint("Attempting to download our custom soundpack!");
			if (GameFuncs::DownloadSoundPack()) {
				DbgPrint("Success downloading soundpack!");
			}
			bDownloadPack = false;
		}
	}
}

/* Uninitialize all structs and hooks associated with the current game, and cleanup memory */
VOID Game::Shutdown() {
	// Wait for the UI to shutdown before we remove our hooks.
	DWORD dwWaitStart = GetTickCount();
	while (UI::bInitialized) {
		if (dwWaitStart + 1000 < GetTickCount()) {
			UI::Shutdown();
		}
		Sleep(33);
	}

	// Take down any detours we're initialized
	TitleHooks::Remove();

	NiNJA.bShutdownCheats = FALSE;
	DbgPrint("Game uninitialized!");
}