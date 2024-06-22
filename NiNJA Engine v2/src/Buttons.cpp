#include "Buttons.h"
#include "Settings.h"
#include "Debug.h"
#include "Globals.h"
//#include "Menu.h"

DWORD BUTTONS_ALL = (BUTTON_LT | BUTTON_RT | BUTTON_A | BUTTON_B | BUTTON_X | BUTTON_Y | BUTTON_LB | BUTTON_RB | BUTTON_LTS | BUTTON_RTS | DPAD_LEFT | DPAD_RIGHT | DPAD_UP | DPAD_DOWN);

VOID Buttons::Initialize() {
	dwPacketNum = 0;
	bValid = FALSE;
	dwFlags = 0;
	dwOldFlags = 1;
	bButtonJustPressed = FALSE;
}

/* Function to be called after our open button is read from the config file, to make sure its a valid button state */
BOOL Buttons::CheckOpenButton(PVOID pMenuOption) {
	/*MenuOption *pOption = (MenuOption*)pMenuOption;
	DWORD dwOpenButton = *(DWORD*)pOption->pSyncVariable;
	if((dwOpenButton & BUTTONS_ALL) == dwOpenButton) {*/
		return TRUE;
	//}
	//DbgPrint("Invalid open button specified! Using default!");
	//return FALSE;
}

/* Function to be called after our validation function is called, if the validation of the button fails */
VOID Buttons::ValidationFailed(PVOID pMenuOption) {
	//MenuOption *pOption = (MenuOption*)pMenuOption;
	//*(DWORD*)pOption->pSyncVariable = (BUTTON_LT | DPAD_LEFT);
}


DWORD dwLastHoldingButtonTime = 0, dwHoldingButtonStartTime = 0, dwLastButtonHeld = 0, dwButtonDelay = 1000;
DWORD dwLastHoldingAPADTime = 0, dwHoldingAPADStartTime = 0, dwLastAPADHeld = 0, dwAPADDelay = 1000;
BOOL Buttons::Poll() {
	DWORD ret = XInputGetState(0, &xi);
	if (ret == ERROR_SUCCESS) {
		DWORD dwNewFlags = 0, dwNewThumbFlags = 0;

		// Save our last packet num
		dwOldPacketNum = dwPacketNum;

		// Store this so we don't repeat button presess
		dwPacketNum = xi.dwPacketNumber;

		// Backup our old flags
		dwOldFlags = dwFlags;
		dwOldThumbFlags = dwThumbFlags;

		// Grab our current buttons
		dwNewFlags |= (xi.Gamepad.bLeftTrigger > 0) ? BUTTON_LT : 0;
		dwNewFlags |= (xi.Gamepad.bRightTrigger > 0) ? BUTTON_RT : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? BUTTON_A : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? BUTTON_B : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? BUTTON_X : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? BUTTON_Y : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? BUTTON_LB : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? BUTTON_RB : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? BUTTON_LTS : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? BUTTON_RTS : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? DPAD_UP : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? DPAD_DOWN : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? DPAD_LEFT : 0;
		dwNewFlags |= (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? DPAD_RIGHT : 0;

		// Grab our thumbstick flags
		dwNewThumbFlags |= (xi.Gamepad.sThumbLY < (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE - (CG::bMovingMenu ? 2000 : 5000)) && abs(xi.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) ? APAD_LTHUMB_DOWN : 0;
		dwNewThumbFlags |= (xi.Gamepad.sThumbLY > (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE + (CG::bMovingMenu ? 2000 : 5000)) && abs(xi.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) ? APAD_LTHUMB_UP : 0;
		// Only consider apad right and left presses if we aren't pressing up or down.
		if (!((dwNewThumbFlags & APAD_LTHUMB_UP) && (dwNewThumbFlags & APAD_LTHUMB_DOWN)) || CG::bMovingMenu) {
			dwNewThumbFlags |= (xi.Gamepad.sThumbLX < (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE - (CG::bMovingMenu ? 2000 : 15000))  && abs(xi.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? APAD_LTHUMB_LEFT : 0;
			dwNewThumbFlags |= (xi.Gamepad.sThumbLX > (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE + (CG::bMovingMenu ? 2000 : 15000)) && abs(xi.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? APAD_LTHUMB_RIGHT : 0;
		}

		// Make our menu act similar to the normal cod menu... after holding the apad for a certain period of time it starts going faster and faster, thats how our menu will act also
		DWORD dwHeldButton = (dwNewFlags & DPAD_UP) ? DPAD_UP : (dwNewFlags & DPAD_DOWN) ? DPAD_DOWN : (dwNewFlags & DPAD_LEFT) ? DPAD_LEFT : (dwNewFlags & DPAD_RIGHT) ? DPAD_RIGHT : 0;
		// Check if we switched which apad direction we're holding, if we did restart this check completely
		if (dwHeldButton != dwLastButtonHeld) {
			dwHoldingButtonStartTime = 0;
			dwLastHoldingButtonTime = 0;
			dwLastButtonHeld = dwHeldButton;
			dwButtonDelay = 1000;
		}

		// If our held DPAD isn't 0, increment our timer
		if (dwHeldButton != 0) {
			if (dwHoldingButtonStartTime == 0) {
				dwHoldingButtonStartTime = GetTickCount();
				dwLastButtonHeld = dwHeldButton;
			}
			else {
				dwLastHoldingButtonTime = GetTickCount();
			}
		}

		// If holding down the button just overrode our "bButtonJustPressed" variable because they held it for long enough, cut the delay time in half then set a new start time
		bool bOverrideButtons = (dwLastHoldingButtonTime - dwHoldingButtonStartTime > dwButtonDelay);
		if (bOverrideButtons) {
			dwHoldingButtonStartTime = GetTickCount();
			if (dwButtonDelay > 200) {
				dwButtonDelay *= 0.6f;
			}
			else {
				switch (dwHeldButton) {
					case DPAD_LEFT:
					case DPAD_RIGHT:
						dwButtonDelay = 10;
						break;
					default:
						if (CG::bMovingMenu) {
							dwAPADDelay = 10;
						}
						else {
							dwAPADDelay = 80;
						}
						break;
				}
			}
		}

		if ((dwNewFlags != dwOldFlags) || bOverrideButtons) {
			dwFlags = dwNewFlags;
			// If our flags changed, we are no longer holding the same button combo, so that means we are free to start monitoring again
			if (bButtonJustPressed) {
				bButtonJustPressed = FALSE;
			}
		}
		else {
			bButtonJustPressed = TRUE;
		}


		// Make our menu act similar to the normal cod menu... after holding the apad for a certain period of time it starts going faster and faster, thats how our menu will act also
		DWORD dwHeldAPAD = (dwNewThumbFlags & APAD_LTHUMB_UP) ? APAD_LTHUMB_UP : (dwNewThumbFlags & APAD_LTHUMB_DOWN) ? APAD_LTHUMB_DOWN : (dwNewThumbFlags & APAD_LTHUMB_RIGHT) ? APAD_LTHUMB_RIGHT : (dwNewThumbFlags & APAD_LTHUMB_LEFT) ? APAD_LTHUMB_LEFT : 0;
		// Check if we switched which apad direction we're holding, if we did restart this check completely
		if (dwHeldAPAD != dwLastAPADHeld) {
			dwHoldingAPADStartTime = 0;
			dwLastHoldingAPADTime = 0;
			dwLastAPADHeld = dwHeldAPAD;
			dwAPADDelay = 1000;
		}

		// If our held DPAD isn't 0, increment our timer
		if (dwHeldAPAD != 0) {
			if (dwHoldingAPADStartTime == 0) {
				dwHoldingAPADStartTime = GetTickCount();
				dwLastAPADHeld = dwHeldAPAD;
			}
			else {
				dwLastHoldingAPADTime = GetTickCount();
			}
		}

		// If holding down the button just overrode our "bButtonJustPressed" variable because they held it for long enough, cut the delay time in half then set a new start time
		bool bOverrideAPAD = (dwLastHoldingAPADTime - dwHoldingAPADStartTime > dwAPADDelay);
		if (bOverrideAPAD) {
			dwHoldingAPADStartTime = GetTickCount();
			if (dwAPADDelay > 200) {
				dwAPADDelay *= 0.6f;
			}
			else {
				switch (dwHeldAPAD) {
				case APAD_LTHUMB_LEFT:
				case APAD_LTHUMB_RIGHT:
					dwAPADDelay = 10;
					break;
				default:
					if (CG::bMovingMenu) {
						dwAPADDelay = 10;
					}
					else {
						dwAPADDelay = 80;
					}
					break;
				}
			}
		}


		if ((dwNewThumbFlags != dwOldThumbFlags) || bOverrideAPAD) {
			// Save our thumbstick flags
			dwThumbFlags = dwNewThumbFlags;

			if (bThumbstickJustMoved) {
				bThumbstickJustMoved = FALSE;
			}
		}
		else {
			bThumbstickJustMoved = TRUE;
		}


		bValid = TRUE;
		return TRUE;
	}
	bValid = FALSE;
	return FALSE;
}