#pragma once
#include "Globals.h"
#include "Menu.h"


namespace UI {
	extern BOOL bInitialized;
	extern BOOL bMessageBoxOpen;
	extern MenuManager mManager;
	
	BOOL DisplayMessageBox(PCHAR pTitle, PCHAR pMessage);

	VOID Initialize();
	VOID Shutdown();
};

