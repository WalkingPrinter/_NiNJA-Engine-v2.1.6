#pragma once
#include "Globals.h"
#include "GameFuncs.h"
#include "ESP.h"
#include "UI.h"
#include "Aimbot.h"
#include "Nospread.h"
#include "Autowall.h"
#include "Spinbot.h"
#include <setjmp.h>

#define __KILLSPREEINTERVAL__ 4000

struct sRemovalFlag {
	DWORD dwFlag;
	BOOL bDisableInGame;
};

static sRemovalFlag pRemoveFlags[] = {
	{ XINPUT_GAMEPAD_START, TRUE },
	{ XINPUT_GAMEPAD_BACK, FALSE },
	{ XINPUT_GAMEPAD_DPAD_LEFT, TRUE },
	{ XINPUT_GAMEPAD_DPAD_RIGHT, TRUE },
	{ XINPUT_GAMEPAD_DPAD_UP,  TRUE },
	{ XINPUT_GAMEPAD_DPAD_DOWN, TRUE },
	{ XINPUT_GAMEPAD_A, TRUE },
	{ XINPUT_GAMEPAD_B, TRUE },
	{ XINPUT_GAMEPAD_X, FALSE },
	{ XINPUT_GAMEPAD_Y, FALSE },
	{ XINPUT_GAMEPAD_RIGHT_SHOULDER, TRUE },
	{ XINPUT_GAMEPAD_LEFT_SHOULDER, TRUE }
};

void ExtraThread(int threadContext);

namespace TitleHooks {
//public:
	extern BOOL bInitialized;

	//static VOID CL_CreateNewCommands_Hook(DWORD dwLocalClient);

	//static VOID R_EndFrame_Hook();

	//static DWORD XamInputGetState_Hook(DWORD dwUserIndex, DWORD r4, PXINPUT_STATE pState);
	bool spawnExtraThread(PVOID pFunction, int iStackSize = __THREADSTACKSIZE__, int iArg = 0);
	DWORD TitleThread(void * playInfo);
	BOOL Apply();
	VOID Remove(BOOL bOverride = FALSE);
};


