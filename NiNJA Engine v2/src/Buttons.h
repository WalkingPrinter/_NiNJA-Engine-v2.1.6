#pragma once
#include <xtl.h>
#include <stdio.h>


enum eButtonState {
	BUTTON_A = 1,
	BUTTON_B = 2,
	BUTTON_X = 4,
	BUTTON_Y = 8,
	BUTTON_LT = 16,
	BUTTON_RT = 32,
	BUTTON_RB = 64,
	BUTTON_LB = 128,
	BUTTON_RTS = 256,
	BUTTON_LTS = 512,
	DPAD_LEFT = 1024,
	DPAD_RIGHT = 2048,
	DPAD_UP = 4096,
	DPAD_DOWN = 8192,
	APAD_LTHUMB_UP = 16384,
	APAD_LTHUMB_DOWN = 32768,
	APAD_LTHUMB_LEFT = 65536,
	APAD_LTHUMB_RIGHT = 131072
};

class Buttons
{
public:
	//BOOL A;
	//BOOL X;
	//BOOL B;
	//BOOL Y;
	//BOOL LT;
	//BOOL RT;
	//BOOL RB;
	//BOOL LB;
	//BOOL RTS;
	//BOOL LTS;
	//BOOL DPAD_LEFT, DPAD_RIGHT, DPAD_UP, DPAD_DOWN;
	DWORD dwPacketNum, dwOldPacketNum;
	BOOL bValid;
	BOOL bButtonJustPressed, bThumbstickJustMoved;
	bool bFiring;
	bool bFiringSecondary;
	DWORD dwFlags, dwOldFlags;
	DWORD dwThumbFlags, dwOldThumbFlags;
	_XINPUT_STATE xi;

	static BOOL CheckOpenButton(PVOID pMenuOption);
	VOID Initialize();
	BOOL Poll();
	static VOID ValidationFailed(PVOID pMenuOption);

	Buttons() {
		Initialize();
	}
};

