#include "UI.h"
#include "GameFuncs.h"
#include "HudElemProcessing.h"
#include "SecureStrings.h"
#include "Strings.h"

BOOL UI::bInitialized = FALSE;
BOOL UI::bMessageBoxOpen = FALSE;

// A vector of all our submenus
MenuManager UI::mManager;

PCHAR pMessageBoxTitle = NULL, pMessageBoxMessage = NULL;
BOOL UI::DisplayMessageBox(PCHAR pTitle, PCHAR pMessage) {
	if (!bMessageBoxOpen) {
		pMessageBoxTitle = pTitle;
		pMessageBoxMessage = pMessage;
		bMessageBoxOpen = true;
	}
}

VOID UI::Initialize() {
	// Startup XOnline
	XOnlineStartup();

	// Calculate the screen mins/maxes and center point
	GameFuncs::CalcScreenDimensions();

	// Init our MenuManager class
	mManager.Initialize();

	// Read our menu options
	mManager.bShouldRead = TRUE;

	DbgPrint("UI initialized!");

	bInitialized = TRUE;
}

VOID UI::Shutdown() {
	// Loop through the hudelems and free any leftover refs
	for (DWORD i = 0; i < HudElems::hudElems.size(); i++) {
		DWORD dwEventQueueSize = HudElems::hudElems[i]->GetEventQueueSize();
		if (dwEventQueueSize > 0) {
			for (DWORD e = 0; e < dwEventQueueSize; e++) {
				SAFE_DELETE(HudElems::hudElems[i]->GetEvent(e));
				DbgPrint("Freed!");
			}
			HudElems::hudElems[i]->ClearEvents();
		}
	}

	// Free memory from our hudElem struct
	HudElems::hudElems.clear();

	// Shutdown our menu
	mManager.Shutdown();

	// Cleanup XOnline
	XOnlineCleanup();

	DbgPrint("Shutdown!");

	bInitialized = FALSE;
}