#pragma once
#include <fstream>
#include <iomanip>

#include "Globals.h"
#include "GameFuncs.h"
#include "Debug.h"
#include "HudElemProcessing.h"


#define __MAXMENUOPTIONS__ 14
#define fSliderWidth 70.0f

//const BYTE FLAG_SAVE = 0x1; // Save to config file
//const BYTE FLAG_CALLFUNC = 0x2; // Call function on use
//const BYTE FLAG_CALLONTOGGLE = 0x4; // Call function on toggle, REQUIRES FLAG_CALLFUNCTION
//const BYTE FLAG_SYNC = 0x8; // Sync value with specified variable

extern char EngineAscii[];

enum eMenuOptionFlags {
	FLAG_SAVE = 1,
	FLAG_CALLFUNC = 2,
	FLAG_CALLONTOGGLE = 4,
	FLAG_SYNC = 8,
	FLAG_VALIDATE = 16,
	FLAG_DVAR = 32,
	FLAG_APPLYONENTERBODY = 64
};

// Save flags consist of a type, and an array size. You must choose DWORD, FLOAT, or STRING as the type, then optionally 
// you can append the VEC2, VEC3, or VEC4 flag as well in order to store the information in array form :)
enum eSaveFlags {
	// Type flag
	FLAG_TYPE_DWORD = 1, 
	FLAG_TYPE_FLOAT = 2,
	FLAG_TYPE_STRING = 4,
	FLAG_SINGLE = 8,
	FLAG_VEC2 = 16,
	FLAG_VEC3 = 32,
	FLAG_VEC4 = 64
};

enum eMenuPos {
	TOPLEFT,
	TOPCENTER,
	TOPRIGHT,
	BOTTOMLEFT,
	BOTTOMMIDDLE,
	BOTTOMRIGHT
};

enum eMenuLevels {
	MENU_MAIN,
	MENU_GAMERTAG_MANAGER,
	MENU_GAMERTAGS,
	MENU_GAMERTAG_EDITOR,
	MENU_AIMBOT,
	MENU_HVH,
	MENU_CURRENTGAME,
	MENU_ESP,
	MENU_ESP_CUSTOMIZATION,
	MENU_ESP_COLORS,
	MENU_CLIENTS,
	MENU_SELECTED_CLIENT,
	MENU_SETTINGS,
	MENU_UI_SETTINGS,
	MENU_POSITION,
	MENU_UI_COLORS,
	MENU_COLOR_PICKER,
	MENU_DEBUG,
	SAVE_OTHER,
	SAVE_WHITELISTED_XUIDS,
	SAVE_PREFERRED_XUIDS,

	// Always last, size of menu levels enum
	MENU_SIZE
};

enum eOptionTypes {
	OPTION_DEFAULT,
	OPTION_CHECKBOX,
	OPTION_SLIDER,
	OPTION_LIST,
	OPTION_SUBMENU,
	OPTION_COLOR,
	OPTION_COLOR_PREVIEW,
	OPTION_XUIDLIST,
	OPTION_CUSTOMSTRING,
	OPTION_INFORMATION,

	// Always last, size of option types enum
	OPTION_SIZE
};

//struct OptionVariation {
//	SecureString *sString;
//	OptionVariation(SecureString *sString) {
//		this->sString = sString;
//	}
//};

struct MenuControl {
	PVOID pParent;
	DWORD dwOptionType;
	vector<HudElem*> Elems;
	FLOAT fWidth, fHeight;
	FLOAT fScale;
	FLOAT topRightX, topRightY;

	/* Sets the position of a menu control, depending on its type. bMoveOverTime and dwMoveTime not implemented! */
	VOID SetPosition(FLOAT x, FLOAT y, FLOAT topRightX, FLOAT topRightY, FLOAT textY, BOOL bMoveOverTime = FALSE, DWORD dwDuration = 0, DWORD dwMoveStartTime = 0);

	/* Sets the scale of a menu control, based off the font scale so everything is consistent */
	VOID SetScale(FLOAT fScale, BOOL bMoveOverTime = FALSE, DWORD dwMoveDuration = 0, DWORD dwStartTime = 0) {
		this->fScale = fScale;
		switch (dwOptionType) {
			// Checkboxes have 2 elems, the background box and the checked icon
			case OPTION_CHECKBOX:
				// Setup the checkbox background
				Elems[0]->SetScale(fScale >= 1.1 ? fWidth : fWidth - 2.0f, fScale >= 1.1f ? fHeight : fHeight - 2.0f, bMoveOverTime, dwMoveDuration, dwStartTime);

				// Setup the "checked" icon
				Elems[1]->SetScale(fScale >= 1.1f ? fWidth - 2.0f : fWidth - 4.0f, fScale >= 1.1f ? fHeight - 2.0f : fHeight - 4.0f, bMoveOverTime, dwMoveDuration, dwStartTime);
				break;
			// Sliders have 2 elems, the slider and the bar it slides on
			case OPTION_SLIDER:
				// Setup the slider background bar
				Elems[0]->SetScale(fWidth * fScale, fScale < 1.1f ? 2.0f : 3.0f, bMoveOverTime, dwMoveDuration, dwStartTime);

				// Setup the actual slider
				Elems[1]->SetScale(fScale < 1.1f ? 1.0f : 2.0f, (fHeight * 2) * fScale, bMoveOverTime, dwMoveDuration, dwStartTime);

				// Setup the value scale
				Elems[2]->fFontScale = fScale;
				break;
			// Lists have 1 elem, which is just the list items display name
			case OPTION_LIST:
			case OPTION_INFORMATION:
				Elems[0]->fFontScale = fScale;
				break;
			case OPTION_COLOR_PREVIEW:
			case OPTION_COLOR:
				Elems[0]->SetScale(fScale >= 1.1f ? fWidth : fWidth - 2.0f, fScale >= 1.1f ? fHeight : fHeight - 2.0f, bMoveOverTime, dwMoveDuration, dwStartTime);
				break;
		}
	}

	VOID SetTextColor(PFLOAT pColor) {
		switch (dwOptionType) {
			case OPTION_LIST:
			case OPTION_INFORMATION:
				memcpy(Elems[0]->pColor, pColor, 0x10);
				break;
			case OPTION_SLIDER:
				memcpy(Elems[2]->pColor, pColor, 0x10);
				break;

		}
	}

	VOID SetControlColor(PFLOAT pColor) {
		switch (dwOptionType) {
			case OPTION_CHECKBOX:
			case OPTION_SLIDER:
				memcpy(Elems[1]->pColor, pColor, 0x10);
				break;
		}
	}

	VOID SetVisible(BOOL bVisible) {
		for (DWORD i = 0; i < Elems.size(); i++) {
			Elems[i]->bVisible = bVisible == TRUE;
		}
	}

	/* Initializes a menu control, given a type and width/height */
	BOOL Initialize(DWORD dwOptionType) {
		this->dwOptionType = dwOptionType;
		this->fScale = CG::GetFontScale() ;

		HudElem *pTmpElem = NULL;
		switch (dwOptionType) {
			case OPTION_CHECKBOX:
				// Setup our width and height for checkboxes
				this->fWidth = 10.0f;
				this->fHeight = 10.0f;
			case OPTION_SLIDER:
				// Setup our width and height for sliders
				if (dwOptionType != OPTION_CHECKBOX) {
					this->fWidth = fSliderWidth;
					this->fHeight = 6.0f;
				}
				// Setup our checkbox background, then initialize the hudelem
				pTmpElem = new HudElem();
				if (pTmpElem) {
					pTmpElem->dwFlags = ELEM_TYPE_MATERIAL;
					pTmpElem->dwAsset = CG::dwWhite;
					memcpy(pTmpElem->pColor, Colors::LightGrey, 0x10);
					//pTmpElem->fAlpha = 0.7f;
					pTmpElem->sSort = LAYER_MENU_CONTROL_BACKGROUND;
					Elems.push_back(pTmpElem);
					HudElems::InitHudElem(Elems.back());

					pTmpElem = new HudElem();
					if (pTmpElem) {
						// Setup the hudelem that will represent the "check", then initialize it
						pTmpElem->dwFlags = ELEM_TYPE_MATERIAL;
						pTmpElem->dwAsset = CG::dwWhite;
						memcpy(pTmpElem->pColor, Colors::Cyan, 0x10);
						//pTmpElem->fAlpha = 0.8f;
						pTmpElem->sSort = LAYER_MENU_FOREGROUND;
						Elems.push_back(pTmpElem);
						HudElems::InitHudElem(Elems.back());

						if (dwOptionType == OPTION_SLIDER) {
							pTmpElem = new HudElem();
							if (pTmpElem) {
								Elems.push_back(pTmpElem);
								// Setup the hudelem that will represent the numeric value of the slider
								if (sManager.alloc(10, &pTmpElem->pText)) {
									memcpy(pTmpElem->pText, ES("NULL"), 5);
									pTmpElem->dwFlags = ELEM_TYPE_TEXT;
									pTmpElem->dwAsset = CG::dwSmallFont;
									memcpy(pTmpElem->pColor, Colors::White, 0x10);
									pTmpElem->sSort = LAYER_MENU_FOREGROUND;

									HudElems::InitHudElem(Elems.back());
								}
								else {
									SAFE_FREE(Elems[0]);
									SAFE_FREE(Elems[1]);
									SAFE_FREE(Elems[3]);
									DbgPrint("Failed to alloc memory for control text elem!");
								}
							}
							else {
								SAFE_FREE(Elems[0]);
								SAFE_FREE(Elems[1]);
								DbgPrint("Failed to alloc memory for elem 3!");
							}
						}
					}
					else {
						SAFE_FREE(Elems[0]);
						DbgPrint("Failed to alloc memory for elem 2!");
					}
				}
				else {
					DbgPrint("Failed to alloc memory for elem 1!");
					return FALSE;
				}
				break;
			case OPTION_LIST:
			case OPTION_INFORMATION:
				// Setup our checkbox background, then initialize the hudelem
				pTmpElem = new HudElem();
				if (pTmpElem) {
					pTmpElem->dwFlags = ELEM_TYPE_TEXT;
					pTmpElem->dwAsset = CG::dwSmallFont;
					memcpy(pTmpElem->pColor, Colors::White, 0x10);
					pTmpElem->sSort = LAYER_MENU_FOREGROUND;
					Elems.push_back(pTmpElem);
					HudElems::InitHudElem(Elems.back());
				}
				else {
					DbgPrint("Failed to alloc memory for list elem!");
					return FALSE;
				}
				break;
			case OPTION_COLOR_PREVIEW:
			case OPTION_COLOR:
				this->fWidth = 10.0f;
				this->fHeight = 10.0f;
				pTmpElem = new HudElem();
				if (pTmpElem) {
					pTmpElem->dwFlags = ELEM_TYPE_MATERIAL;
					pTmpElem->dwAsset = CG::dwWhite;
					memcpy(pTmpElem->pColor, Colors::White, 0x10);
					pTmpElem->sSort = LAYER_MENU_FOREGROUND;
					Elems.push_back(pTmpElem);
					HudElems::InitHudElem(Elems.back());
				}
				else {
					DbgPrint("Failed to alloc memory for list elem!");
					return FALSE;
				}
				break;
			default:
				DbgPrintE("Menu option type %i doesn't have any control associated with it! Skipping!", dwOptionType);
				return FALSE;
		}
		return TRUE;
	}

	/* Cleans up any memory we allocated for our menu control */
	VOID Shutdown() {
		if (Elems.size() > 0) {
			for (DWORD i = 0; i < Elems.size(); i++) {
				SAFE_DELETE(Elems[i]);
			}
			Elems.clear();
		}
	}
};

class MenuOption {
public:
	PVOID pParent;
	DWORD dwFlags; // The bitflags associated with the menu option (eMenuOptionFlags)
	DWORD dwTitleFlags; // The bitflags describing what titles this option is supported on
	DWORD dwSaveFlags; // The bitflags describing how we want to save this menu option
	DWORD dwState; // The current state of the menu option
	DWORD dwLastButtonPressed; // The last button pressed while this option was selected
	PCHAR pSaveName; // The name to use when saving the option to our config file
	PCHAR pDisplayName; // The name to show in the menu
	PCHAR pDescriptionText; // The description of the menu option
	FLOAT fRangeMin; // The minimum allowed value for slider types
	FLOAT fRangeMax; // The maxiumum allowed value for slider types
	FLOAT fIncrement; // The amount to increment by when moving the slider
	MenuControl *pControl; // The menu control associated with this option
	eOptionTypes dwOptionType; // The type of menu option
	vector<PCHAR> Variations; // A vector containing all the possible variations
	PDWORD pSyncVariable; // Variable to synchronize dwState with if FLAG_SYNC is present in dwFlags
	VOID(*pFunctionCall) (MenuOption*); // Function to call if FLAG_CALLFUNC is present in dwFlags
	BOOL(*pValidationCall) (PVOID); // Function to call if FLAG_VALIDATE is present in dwFlags
	VOID(*pValidationFailed) (PVOID); // Function to call if FLAG_VALIDATE is present in dwFlags and the validation function fails


	static VOID EnterMenu_Internal(MenuOption *pOption);
	static VOID EnterColorMenu(MenuOption *pOption);

	PDWORD GetPSyncVariable() {
		return dwFlags & FLAG_DVAR ? (PDWORD)GameFuncs::Dvar_GetValueAddress((DWORD)pSyncVariable) : pSyncVariable;
	}

	VOID SetPSyncVariable(int iValue) {
		PDWORD address = dwFlags & FLAG_DVAR ? (PDWORD)GameFuncs::Dvar_GetValueAddress((DWORD)pSyncVariable) : pSyncVariable;
		if (address) {
			*(int*)address = iValue;
		}
	}

	VOID SetPSyncVariable(FLOAT fValue) {
		PDWORD address = dwFlags & FLAG_DVAR ? (PDWORD)GameFuncs::Dvar_GetValueAddress((DWORD)pSyncVariable) : pSyncVariable;
		if (address) {
			*(FLOAT*)address = fValue;
		}
	}

	/* Adds a new variation to this option */
	MenuOption *AddV(PCHAR pNewVariation) {
		Variations.push_back(pNewVariation);
		return this;
	}

	MenuOption *ApplyOnEnterBody() {
		// Add FLAG_APPLYONENTERBODY if it doesn't already exist
		ADDFLAG(dwFlags, FLAG_APPLYONENTERBODY);
		return this;
	}

	/* Sets the function to call when the menu option is used */
	MenuOption *CallFunc(PVOID pFunctionCall, BOOL bCallOnToggle = FALSE) {
		// Add FLAG_SYNC if it doesnt already exist
		ADDFLAG(dwFlags, FLAG_CALLFUNC);
		if (bCallOnToggle) {
			ADDFLAG(dwFlags, FLAG_CALLONTOGGLE);
		}

		this->pFunctionCall = (VOID(*)(MenuOption*))pFunctionCall;
		return this;
	}

	MenuOption *ChooseColor(FLOAT *pColor) {
		// Add FLAG_SYNC if it doesnt already exist
		ADDFLAG(dwFlags, FLAG_CALLFUNC);
		this->pFunctionCall = (VOID(*)(MenuOption*))&EnterColorMenu;
		this->pSyncVariable = (PDWORD)pColor;
		return this;
	}

	MenuOption *EnterMenu(DWORD dwMenu) {
		// Add FLAG_SYNC if it doesnt already exist
		ADDFLAG(dwFlags, FLAG_CALLFUNC);

		this->dwState = dwMenu;
		this->pFunctionCall = (VOID(*)(MenuOption*))&EnterMenu_Internal;
		return this;
	}
	
	MenuOption *EnabledOn(DWORD dwFlagsToSet) {
		// Set our title flags to the ones specified
		dwTitleFlags = dwFlagsToSet;

		return this;
	}

	MenuOption *DisabledOn(DWORD dwFlagsToRemove) {
		// Remove the flags from dwTitleFlags
		DELFLAG(dwTitleFlags, dwFlagsToRemove);

		return this;
	}

	/* Adds a save name to the menu option */
	MenuOption *SaveAs(PCHAR pSaveName = NULL, DWORD dwSaveFlags = FLAG_TYPE_DWORD|FLAG_SINGLE) {
		// Add FLAG_SAVE if it doesnt already exist
		ADDFLAG(dwFlags, FLAG_SAVE);

		if (pSaveName == NULL)
			pSaveName = pDisplayName;
		this->pSaveName = pSaveName;
		this->dwSaveFlags = dwSaveFlags;
		return this;
	}

	/* Sets the description text of the current menu option */
	MenuOption *SetDescription(PCHAR pDescriptionText) {
		// Make sure all buttons have been replaced, incase we didn't preprocess the description using this function before hand
		pDescriptionText = ReplaceButtonsInString(pDescriptionText);

		this->pDescriptionText = pDescriptionText;
		return this;
	}

	/* Sets the max length of a custom string type option */
	MenuOption *SetMaxLength(FLOAT fMaxLength) {
		this->fRangeMax = fMaxLength;
		return this;
	}

	MenuOption *SetupDynamicFloatingSlider() {
		// Setup our save flags as a type of single float, so our toggle function works
		this->dwSaveFlags = (FLAG_TYPE_FLOAT | FLAG_SINGLE);
		return this;
	}

	/* Sets the floating point range of a slider type menu option 
	NOTE: This REQUIRES a pSyncVariable! FLOAT and int types supported! */
	MenuOption *SetRange(FLOAT fRangeMin, FLOAT fRangeMax, FLOAT fIncrement) {
		this->fRangeMin = fRangeMin;
		this->fRangeMax = fRangeMax;
		this->fIncrement = fIncrement;
		return this;
	}

	/* Adds a sync variable to the menu option. THIS SHOULD BE THE LAST FUNCTION CALLED WHEN SETTING UP A NEW MENU OPTION! */
	MenuOption *SyncWith(PVOID pSyncVariable) {
		// Add FLAG_SYNC if it doesnt already exist
		ADDFLAG(dwFlags, FLAG_SYNC);

		this->pSyncVariable = (PDWORD)pSyncVariable;

		// Set the initial state of our menu option, based on the pSyncVariable
		if (((dwSaveFlags & FLAG_TYPE_STRING) || (dwSaveFlags & FLAG_TYPE_DWORD)) && (dwSaveFlags & FLAG_SINGLE)) {
			this->SetState();

			if (this->dwFlags & FLAG_CALLONTOGGLE) {
				this->dwState = *(DWORD*)this->GetPSyncVariable();
				this->Call();
			}
		}

		return this;
	}

	MenuOption *SyncWithDvar(DWORD dwDvarOffset) {
		// Add FLAG_DVAR if it doesnt already exist
		ADDFLAG(dwFlags, FLAG_DVAR);

		return SyncWith((PVOID)dwDvarOffset);
	}

	/* Adds a function to be called upon reading a menu option from file, to validate the values which were read. Provide a ptr to a BOOL function that takes a ptr to the current menu option! 
	Example Validation Function: BOOL ValidationFunction(PVOID pMenuOption) { Validation code } 
	Example Validation Failure Function: VOID ValidationFailed(PVOID pSyncVar) { Code to trigger on failure } */
	MenuOption *ValidateCustom(PVOID pValidationCall, PVOID pValidationFailed) {
		// Add FLAG_VALIDATE if it doesnt already exist
		ADDFLAG(dwFlags, FLAG_VALIDATE);

		this->pValidationCall = (BOOL(*)(PVOID))pValidationCall;
		this->pValidationFailed = (VOID(*)(PVOID))pValidationFailed;
		return this;
	}

	/* Validates a floating point value, for use with sliders */
	MenuOption *ValidateFloat() {
		return ValidateCustom(&ValidateFloat_Internal, &ValidateFloat_Failed);
	}

	/* Validates an integral value, for use with sliders */
	MenuOption *ValidateInt() {
		return ValidateCustom(&ValidateInt_Internal, &ValidateInt_Failed);
	}

	/* Calls the function associated with this option */
	VOID Call(DWORD dwButtonPressed = NULL) {
		if (dwButtonPressed != NULL) {
			this->dwLastButtonPressed = dwButtonPressed;
			DbgPrint("Just pressed %i", dwButtonPressed);
		}
		DbgPrint("Calling menu function!");
		if (MmIsAddressValid((PVOID)pFunctionCall)) {
			pFunctionCall(this);
		}
	}

	/* Attempts to find the mapped integral value of a menu option variation given a string of text */
	DWORD FindVariation(PCHAR pVariationName) {
		DWORD ret = 0;
		for (DWORD i = 0; i < Variations.size(); i++) {
			//SecureString *v = Variations[i];
			if (Compare((PCHAR)RemoveChars(string(Variations[i]), " ").c_str(), pVariationName, FALSE)) {
				ret = i;
			}
			//v->Clear();
		}
		return ret;
	}

	PCHAR GetDisplayName() {
		switch (dwOptionType) {
			case OPTION_CUSTOMSTRING:
				if (Compare(pDisplayName, __EMPTYGTSTRING__)) {
					return "[Empty]";
				}
				break;
		}
		return pDisplayName;
	}

	static FLOAT GetSliderAdditave(MenuOption *pThis) {
		if (pThis->pControl) {
			DWORD pSyncVariable = (DWORD)pThis->GetPSyncVariable();
			if (MmIsAddressValid((PVOID)pSyncVariable)) {
				FLOAT tValue = 0.0f, tMax = 0.0f;
				if (pThis->dwSaveFlags & FLAG_TYPE_FLOAT) {
					tValue = *(FLOAT*)pSyncVariable;
				}
				else if (pThis->dwSaveFlags & FLAG_TYPE_DWORD) {
					tValue = (FLOAT)*(int*)pSyncVariable;
				}

				float fDifference = pThis->fRangeMax - pThis->fRangeMin;
				if (fDifference != pThis->fRangeMax) {
					tValue += (fDifference - pThis->fRangeMax);
					tMax = fDifference;
				}
				else {
					tMax = pThis->fRangeMax;
				}

				return (FLOAT)((((FLOAT)fSliderWidth / tMax) * tValue) * pThis->pControl->fScale) - (pThis->pControl->Elems[1]->fWidth / 2.0f);
			}
		}
		return 0.0f;
	}

	/* Sets the hudelem associated with the current menu option to match the state of the menu option */
	VOID SetVisualState() {
		PCHAR tTxt = NULL;

		if (pControl) {
			// Make sure the scale of our control is set correctly
			pControl->SetScale(CG::GetFontScale() );

			switch (dwOptionType) {
				case OPTION_LIST:
					tTxt = Variations[dwState];

					pControl->fHeight = GameFuncs::GetFontHeight(CG::dwSmallFont, 1.0f);
					pControl->fWidth = GameFuncs::GetTextWidth(tTxt, CG::dwSmallFont);

					//pControl->Elems[0]->x = pControl->topRightX + (CG::fMenuTextPadding * 1.5f) - (GameFuncs::GetTextWidth(tTxt, CG::dwSmallFont) * pControl->fScale);
					pControl->Elems[0]->fX = pControl->topRightX - CG::fOptionIndentAmount - (CG::fMenuTextPadding * 1.5f) - (GameFuncs::GetTextWidth(tTxt, CG::dwSmallFont) * pControl->fScale);

					//Variations[dwState]->Clear();
					pControl->Elems[0]->pText = tTxt;
					break;				
				case OPTION_INFORMATION:
					tTxt = (PCHAR)GetPSyncVariable();
					if (tTxt) {
						pControl->fHeight = GameFuncs::GetFontHeight(CG::dwSmallFont, 1.0f);
						pControl->fWidth = GameFuncs::GetTextWidth(tTxt, CG::dwSmallFont);

						//pControl->Elems[0]->x = pControl->topRightX + (CG::fMenuTextPadding * 1.5f) - (GameFuncs::GetTextWidth(tTxt, CG::dwSmallFont) * pControl->fScale);
						pControl->Elems[0]->fX = pControl->topRightX - CG::fOptionIndentAmount - (CG::fMenuTextPadding * 1.5f) - (GameFuncs::GetTextWidth(tTxt, CG::dwSmallFont) * pControl->fScale);

						//Variations[dwState]->Clear();
						pControl->Elems[0]->pText = tTxt;
					}
					break;
				case OPTION_CHECKBOX:
					memcpy(pControl->Elems[1]->pColor, (dwState > 0 ? CG::cControlColor : Colors::Black), 0x10);
					break;
				case OPTION_SLIDER:
					memcpy(pControl->Elems[1]->pColor, CG::cControlColor, 0x10);

					pControl->Elems[1]->fX = pControl->Elems[0]->fX + GetSliderAdditave(this);

					if (dwSaveFlags & FLAG_TYPE_FLOAT) {
						sprintf(pControl->Elems[2]->pText, "%.4f", *(FLOAT*)GetPSyncVariable());
						RemoveTrailingZerosFromFloatingString(pControl->Elems[2]->pText);
					}
					else if (dwSaveFlags & FLAG_TYPE_DWORD) {
						sprintf(pControl->Elems[2]->pText, "%i", *(int*)GetPSyncVariable());
					}

					//DbgPrint("Pos: %f, Scale: %f", pControl->Elems[1]->x, pControl->fScale);

					break;
				case OPTION_COLOR_PREVIEW:
				case OPTION_COLOR:
					if (pSyncVariable) {
						memcpy(pControl->Elems[0]->pColor, GetPSyncVariable(), 0x10);
					}
					break;
			}
		}
	}

	/* Sets dwState to the initial value of pSyncVariable (ONLY FOR DWORD SINGLE TYPES) */
	VOID SetState() {
		if (dwFlags & FLAG_SINGLE) {
			if (pSyncVariable) {
				if (dwSaveFlags & FLAG_TYPE_DWORD) {
					dwState = *(DWORD*)GetPSyncVariable();
				}
				else if (dwSaveFlags & FLAG_TYPE_STRING) {
					pDisplayName = (PCHAR)GetPSyncVariable();
					if(Compare(pDisplayName, __EMPTYGTSTRING__)) {
						dwState = FALSE;
					}
					else {
						dwState = TRUE;
					}
				}
			}
		}
	}

	/* Toggles dwState up or down depending on which button we pressed */
	DWORD Toggle(BOOL up);

	MenuOption(PCHAR pDisplayName, eOptionTypes eOptionType = OPTION_DEFAULT, DWORD dwFlags = NULL) {
		this->pDisplayName = pDisplayName;
		this->dwOptionType = eOptionType;
		this->dwFlags = dwFlags;
		this->pSaveName = NULL;
		this->pSyncVariable = NULL;
		this->pFunctionCall = NULL;
		this->pValidationCall = NULL;
		this->dwState = NULL;
		this->pDescriptionText = NULL;
		this->dwTitleFlags = FLAG_ALL_COD;

		MenuControl *pNewControl = new MenuControl();
		if (pNewControl) {
			pNewControl->pParent = this;
			if (pNewControl->Initialize(eOptionType)) {
				DbgPrint("Successfully initialized control of type %i", eOptionType);
				pControl = pNewControl;
				return;
			}
			else {
				// The control was an invalid type, and failed to initialize, so cleanup the memory associated with it
				SAFE_DELETE(pNewControl);
			}
		}
		pControl = NULL;
	}
private:
	static BOOL ValidateInt_Internal(MenuOption *pOption) {
		int val = *(int*)pOption->GetPSyncVariable();
		if (val > (int)pOption->fRangeMax || val < (int)pOption->fRangeMin) {
			DbgPrint("Value %i out of range for option %s!", val, pOption->pDisplayName);
			return FALSE;
		}
		return TRUE;
	}

	static VOID ValidateInt_Failed(MenuOption *pOption) {
		*(int*)pOption->GetPSyncVariable() = (int)pOption->fRangeMin;
	}

	static BOOL ValidateFloat_Internal(MenuOption *pOption) {
		FLOAT val = *(FLOAT*)pOption->GetPSyncVariable();
		if (val > pOption->fRangeMax || val < pOption->fRangeMin) {
			DbgPrint("Value %f out of range for option %s!", val, pOption->pDisplayName);
			return FALSE;
		}
		return TRUE;
	}

	static VOID ValidateFloat_Failed(MenuOption *pOption) {
		*(FLOAT*)pOption->GetPSyncVariable() = pOption->fRangeMin;
	}
};

struct MenuOptionElem {
	DWORD dwIndex;
	HudElem Text;
	HudElem Material;
	MenuOption *pParent;
	MenuControl *Control;

	MenuOptionElem(DWORD dwIndex,  HudElem &Text, HudElem &Material) {
		this->dwIndex = dwIndex;
		this->Text = Text;
		this->Material = Material;
		this->Control = NULL;
		this->pParent = NULL;
	}
	MenuOptionElem() {
	}
};

class MenuOptions {

	vector<MenuOption> mOpts; // A vector containing all the menu options
public:
	MenuOption *Add(MenuOption& newOption) {
		mOpts.push_back(newOption);
		return &mOpts.back();
	}
	VOID Clear() {
		if (mOpts.size() > 0) {
			mOpts.clear();
		}
	}

	BOOL IsOptionDisabled(DWORD dwOptionIndex) {
		if ((mOpts[dwOptionIndex].dwTitleFlags & __TITLEFLAG__) == 0) {
			return TRUE;
		}
		return FALSE;
	}

	DWORD GetNumDisabledOptions() {
		DWORD dwNumDisabled = 0;
		for (DWORD i = 0; i < mOpts.size(); i++) {
			dwNumDisabled += IsOptionDisabled(i);
		}
		return dwNumDisabled;
	}
	DWORD GetSize(BOOL bRealSize = FALSE) {
		if (!bRealSize) {
			return mOpts.size() - GetNumDisabledOptions();
		}
		return mOpts.size();
	}
	MenuOption* Get(DWORD dwIndex, BOOL bRealSize = FALSE) {
		if (!bRealSize) {
			if (dwIndex < GetSize()) {
				DWORD dwEnabledIndex = 0;
				for (DWORD i = 0; i < mOpts.size(); i++) {
					if (!IsOptionDisabled(i)) {
						if (dwEnabledIndex == dwIndex) {
							return &mOpts[i];
						}
						dwEnabledIndex++;
					}
				}
			}
		}
		else {
			return &mOpts[dwIndex];
		}
		DbgPrint("An invalid menu option index was specified! Fix your shit!");
		return 0;
	}
};

class Menu {
public:
	BOOL bDisplay; // Whether or not the menu should be displayed
	BOOL bSave; // Whether or not to even try to save this menu to file
	BOOL bSubmenu; // Whether or not this is a main, or submenu
	PCHAR DisplayName; // The display name of the menu
	eMenuLevels mLastMenu; // Parent menu level
	MenuOptions Options; // A handler for all the menu options
	DWORD dwSelection;

	MenuOption *AddOption(MenuOption newOption) {
		newOption.pParent = this;
		return Options.Add(newOption);
	}

	MenuOption *FindOption(PCHAR pOptionName, BOOL bRealSize = FALSE) {
		for (DWORD i = 0; i < Options.GetSize(bRealSize); i++) {
			MenuOption *pOption = Options.Get(i, bRealSize);
			if (Compare(pOption->pSaveName, pOptionName)) {
				return pOption;
			}
		}
		return 0;
	}

	Menu(PCHAR displayName, BOOL bSave = TRUE, BOOL bSubmenu = FALSE, BOOL bDisplay = TRUE) {
		this->bDisplay = bDisplay;
		this->bSubmenu = bSubmenu;
		this->bSave = bSave;
		DisplayName = displayName;
		dwSelection = 0;
	}
	Menu() {
		bDisplay = FALSE;
		bSubmenu = FALSE;
		DisplayName = NULL;
		dwSelection = 0;
	}
};

class MenuManager {
public:
	BOOL bInitialized;
	BOOL bShouldSave;
	BOOL bShouldRead;
	bool bMenuVisible;
	bool bUpOneLevel;

	map<DWORD, Menu> Menus;
	eMenuLevels dwCurrentMenu;
	DWORD dwNumOptions;

	//MemoryManager eManager; // Manages memory for dynamic HudElems such as menu text elems
	//BYTE MenuTextBuff[sizeof(HudElem)*__MAXMENUOPTIONS__ * 2 + sizeof(HudElem)];
	MenuOptionElem DynamicMenuElems[__MAXMENUOPTIONS__];
	//vector<MenuOptionElem> DynamicMenuElems; // A vector of the elems being managed by eManager

	HudElem Background, Border, Description, TopSeparator, BottomSeparator, TitleText, Slider, Scrollbar;

	VOID ToggleMenu(BOOL bForward);
	Menu *FindMenu(PCHAR pMenuName); 
	FLOAT GetHeight(DWORD numMenuOptions = -1);
	VOID SetControlPosition(FLOAT x, FLOAT y, BOOL bMoveOverTime = FALSE, DWORD dwMoveTime = 0, DWORD dwMoveStartTime = 0, BOOL bTextMoveOverTime = FALSE);
	VOID GetMenuPos(eMenuPos offset, FLOAT *x, FLOAT *y);
	BOOL Save();
	BOOL Read();
	VOID MonitorButtons();
	VOID Scroll(BOOL bForward);
	VOID UpdateMenuElems(DWORD dwNumOptions, BOOL bEffects = FALSE);
	VOID SetMenuPosition(FLOAT x, FLOAT y, BOOL bMoveOverTime = FALSE, DWORD dwMoveDuration = 0, DWORD dwMoveStartTime = 0, BOOL bTextMoveOverTime = FALSE);
	FLOAT GetScrollbarOptionHeight();
	FLOAT GetScrollbarYPos(DWORD dwDisplayIndex);
	FLOAT GetScrollbarHeight();
	VOID SetMenuScale(BOOL bScaleOverTime = FALSE, DWORD dwMoveDuration = 0, DWORD dwMoveStartTime = 0);
	VOID SetMenuColors(BOOL bEffects, DWORD dwTransitionLength, DWORD dwFadeStartTime);
	VOID SetSelection(DWORD dwSelection, BOOL bEffects = FALSE);
	VOID SetVisibility(bool vis, BOOL bFadeOverTime = FALSE, DWORD dwFadeTime = 0, DWORD dwMoveStartTime = 0);
	static VOID UpdateClientMenu();
	static VOID EnterClientMenu(MenuOption * pOption);
	static VOID ClientMenuHandler(MenuOption * pOption);
	static VOID UpdateMenuUI(MenuOption * pOption);
	Menu *GetMenu(eMenuLevels menuLevel);
	Menu *GetMenu(DWORD menuLevel);
	MenuOptions *GetMenuOptions(eMenuLevels menuLevel);
	MenuOptions *GetMenuOptions(DWORD menuLevel);
	VOID UpdateMenuStates();
	static VOID SetAimbotConfig(MenuOption * pOption);
	static VOID LaunchCustomGTEntry(MenuOption * pOption);
	static VOID ClearGTEntry(MenuOption * pOption);
	static VOID GamertagMenuHandler(MenuOption * pOption);
	static VOID ResetGT(MenuOption * pOption);
	static VOID SetFOVValue(MenuOption * pOption);
	static VOID SetPrestigeValue(MenuOption * pOption);
	static VOID CenterMenu(MenuOption * pOption);
	static VOID TestRenderFlags(MenuOption * pOption);
	static VOID UnloadXex(MenuOption * pOption);
	static VOID SendTestCommand(MenuOption * pOption);
	static VOID DumpXModel(MenuOption * pOption);
	VOID SwitchToMenu(MenuOption * pOption);
	VOID Initialize();
	VOID Shutdown();
	VOID RefreshMenuText(int dwMenu = -1, int dwNumOptions = -1);
	static VOID EnterMenu(DWORD dwMenu, BOOL bEffects = TRUE, BOOL bSaveLastMenu = TRUE);
	VOID OnPlayerSpawned();

	MenuManager() {
	}
};