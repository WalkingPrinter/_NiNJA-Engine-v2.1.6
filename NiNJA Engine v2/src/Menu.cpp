#include "Menu.h"
#include "SecureStrings.h"

HudElem NiNJAElem;
MenuManager *pThis = NULL;
DWORD dwMenuOpenButton = (BUTTON_LT | DPAD_LEFT);
char EngineAscii[] = { 0x3B, 0x20, 0x20, 0x20, 0x20, 0x20, 0x29, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x29, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x3B, 0x20, 0x20, 0x28, 0x20, 0x2F, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20, 0x2F, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x29, 0x20, 0x20, 0x0D, 0x0A, 0x3B, 0x20, 0x20, 0x29, 0x5C, 0x28, 0x29, 0x29, 0x20, 0x28, 0x20, 0x20, 0x20, 0x29, 0x5C, 0x28, 0x29, 0x29, 0x20, 0x20, 0x20, 0x28, 0x20, 0x20, 0x20, 0x20, 0x29, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20, 0x20, 0x28, 0x20, 0x20, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20, 0x29, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20, 0x2F, 0x28, 0x20, 0x20, 0x0D, 0x0A, 0x3B, 0x20, 0x28, 0x28, 0x5F, 0x29, 0x5C, 0x20, 0x20, 0x29, 0x5C, 0x20, 0x28, 0x28, 0x5F, 0x29, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x29, 0x5C, 0x28, 0x28, 0x28, 0x28, 0x5F, 0x29, 0x28, 0x20, 0x20, 0x20, 0x20, 0x29, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20, 0x29, 0x5C, 0x29, 0x29, 0x28, 0x20, 0x29, 0x5C, 0x20, 0x20, 0x20, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x29, 0x29, 0x5C, 0x20, 0x20, 0x20, 0x2F, 0x28, 0x28, 0x20, 0x20, 0x20, 0x29, 0x28, 0x5F, 0x29, 0x29, 0x20, 0x0D, 0x0A, 0x3B, 0x20, 0x20, 0x5F, 0x28, 0x28, 0x5F, 0x29, 0x28, 0x28, 0x5F, 0x29, 0x20, 0x5F, 0x28, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x28, 0x28, 0x5F, 0x29, 0x29, 0x5C, 0x20, 0x5F, 0x20, 0x29, 0x5C, 0x20, 0x20, 0x28, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x20, 0x29, 0x5C, 0x20, 0x29, 0x20, 0x28, 0x28, 0x5F, 0x29, 0x29, 0x5C, 0x28, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x29, 0x5C, 0x20, 0x29, 0x20, 0x20, 0x2F, 0x28, 0x28, 0x5F, 0x29, 0x20, 0x28, 0x5F, 0x29, 0x29, 0x5C, 0x20, 0x28, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x3B, 0x20, 0x7C, 0x20, 0x5C, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x29, 0x7C, 0x20, 0x5C, 0x7C, 0x20, 0x7C, 0x20, 0x5F, 0x20, 0x7C, 0x20, 0x7C, 0x28, 0x5F, 0x29, 0x5F, 0x5C, 0x28, 0x5F, 0x29, 0x20, 0x7C, 0x20, 0x5F, 0x5F, 0x7C, 0x20, 0x5F, 0x28, 0x5F, 0x2F, 0x28, 0x20, 0x20, 0x28, 0x28, 0x29, 0x28, 0x5F, 0x29, 0x28, 0x5F, 0x29, 0x20, 0x5F, 0x28, 0x5F, 0x2F, 0x28, 0x20, 0x28, 0x5F, 0x29, 0x29, 0x20, 0x20, 0x20, 0x5F, 0x29, 0x28, 0x28, 0x5F, 0x29, 0x7C, 0x5F, 0x20, 0x20, 0x29, 0x20, 0x20, 0x0D, 0x0A, 0x3B, 0x20, 0x7C, 0x20, 0x2E, 0x60, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x7C, 0x20, 0x2E, 0x60, 0x20, 0x7C, 0x7C, 0x20, 0x7C, 0x7C, 0x20, 0x7C, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x20, 0x5C, 0x29, 0x29, 0x2F, 0x20, 0x5F, 0x60, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x7C, 0x20, 0x27, 0x20, 0x5C, 0x29, 0x29, 0x2F, 0x20, 0x2D, 0x5F, 0x29, 0x20, 0x20, 0x5C, 0x20, 0x56, 0x20, 0x2F, 0x20, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x3B, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x7C, 0x20, 0x5C, 0x5F, 0x5F, 0x2F, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x5C, 0x5F, 0x5C, 0x20, 0x20, 0x7C, 0x5F, 0x5F, 0x5F, 0x7C, 0x7C, 0x5F, 0x7C, 0x7C, 0x5F, 0x7C, 0x20, 0x5C, 0x5F, 0x5F, 0x2C, 0x20, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x7C, 0x5F, 0x7C, 0x7C, 0x5F, 0x7C, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x7C, 0x20, 0x20, 0x20, 0x5C, 0x5F, 0x2F, 0x20, 0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x7C, 0x20, 0x20, 0x0D, 0x0A, 0x3B, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0 };

Menu *MenuManager::GetMenu(eMenuLevels menuLevel) {
	return &Menus[menuLevel];
}
Menu *MenuManager::GetMenu(DWORD menuLevel) {
	return GetMenu((eMenuLevels)menuLevel);
}
MenuOptions *MenuManager::GetMenuOptions(eMenuLevels menuLevel) {
	return &Menus[menuLevel].Options;
}
MenuOptions *MenuManager::GetMenuOptions(DWORD menuLevel) {
	return GetMenuOptions((eMenuLevels)menuLevel);
}

/* Retrieves a menu object by name */
Menu *MenuManager::FindMenu(PCHAR pMenuName) {
	for (DWORD m = 0; m < MENU_SIZE; m++) {
		if (Menus[m].DisplayName && pMenuName) {
			if (Compare((PCHAR)RemoveChars(string(Menus[m].DisplayName), " ").c_str(), pMenuName, FALSE)) {
				return &Menus[m];
			}
		}
	}
	return 0;
}

/* All the functions associated with building, displaying, and updating the menus user interface */
#pragma region Menu UI functions

/* Retrieves the location of the menu corresponding to eMenuPos */
VOID MenuManager::GetMenuPos(eMenuPos pos, FLOAT *x, FLOAT *y) {
	switch (pos) {
	case TOPLEFT:
		*x = CG::dwMenuPos[0] - (CG::fMenuWidth / 2);
		*y = CG::dwMenuPos[1] - (CG::fMenuHeight / 2);
		break;
	case TOPCENTER:
		*x = CG::dwMenuPos[0];
		*y = CG::dwMenuPos[1] - (CG::fMenuHeight / 2);
		break;
	case TOPRIGHT:
		*x = CG::dwMenuPos[0] + (CG::fMenuWidth / 2);
		*y = CG::dwMenuPos[1] - (CG::fMenuHeight / 2);
		break;
	case BOTTOMLEFT:
		*x = CG::dwMenuPos[0] - (CG::fMenuWidth / 2);
		*y = CG::dwMenuPos[1] + (CG::fMenuHeight / 2);
		break;
	case BOTTOMMIDDLE:
		*x = CG::dwMenuPos[0];
		*y = CG::dwMenuPos[1] + (CG::fMenuHeight / 2);
		break;
	case BOTTOMRIGHT:
		*x = CG::dwMenuPos[0] + (CG::fMenuWidth / 2);
		*y = CG::dwMenuPos[1] + (CG::fMenuHeight / 2);
		break;
	}
}

/* Gets the height of the menu depending on the number of options */
FLOAT MenuManager::GetHeight(DWORD numMenuOptions) {
	// Only use a max of __MAXMENUOPTIONS__ for our menu height, however allow less if we are displaying less than the max
	numMenuOptions = (numMenuOptions == -1 ? __MAXMENUOPTIONS__ : (numMenuOptions < __MAXMENUOPTIONS__ ? numMenuOptions : __MAXMENUOPTIONS__));

	//DbgPrint("OptionHeight: %f, OptionSpacing: %f, TopBottomHeight: %f, NumMenuOptions: %i", CG::fMenuOptionHeight, CG::fOptionSpacing, CG::fTopBottomBoxHeight, numMenuOptions);

	return (numMenuOptions * CG::fMenuOptionHeight) + (numMenuOptions * CG::fOptionSpacing) + (CG::fTopBottomBoxHeight * 2) + (CG::fOptionSpacing * 2) + (CG::fMenuTextPadding * 2) + (numMenuOptions + 1);// +(((DWORD)Description.bNumLinesWrapped > 0) ? (FLOAT)((Description.bNumLinesWrapped - 1) * CG::fMenuFontHeight) : 0.0f);
}

VOID MenuManager::SetControlPosition(FLOAT x, FLOAT y, BOOL bMoveOverTime, DWORD dwMoveDuration, DWORD dwMoveStartTime, BOOL bTextMoveOverTime) {
	// Grab the coords of a few different positions where we want to draw stuff on the menu
	FLOAT topLeftX, topLeftY;
	GetMenuPos(TOPLEFT, &topLeftX, &topLeftY);
	FLOAT topCenterX, topCenterY;
	GetMenuPos(TOPCENTER, &topCenterX, &topCenterY);
	FLOAT bottomLeftX, bottomLeftY;
	GetMenuPos(BOTTOMLEFT, &bottomLeftX, &bottomLeftY);
	FLOAT topRightX, topRightY;
	GetMenuPos(TOPRIGHT, &topRightX, &topRightY);

	for (DWORD i = 0; i < ARRAY_SIZE(DynamicMenuElems); i++) {
		// No moveovertime on controls
		if (DynamicMenuElems[i].Control) {
			//if (DynamicMenuElems[i].Control->dwOptionType == OPTION_LIST) {
			//	DynamicMenuElems[i].Control->SetPosition(topRightX - CG::fOptionIndentAmount - (CG::fMenuTextPadding * 1.5f) - (GameFuncs::GetTextWidth(DynamicMenuElems[i].Control->Elems[0]->pText, CG::dwSmallFont) * CG::GetFontScale() ), DynamicMenuElems[i].Text.fY, topRightX, topRightY);// , bMoveOverTime, dwMoveDuration, dwMoveStartTime);
			//	//DynamicMenuElems[i].Control->Elems[0]->secureValue->Clear();
			//}
			//else {
				DynamicMenuElems[i].Control->SetPosition(topRightX - CG::fOptionIndentAmount - (CG::fMenuTextPadding * 1.5f), DynamicMenuElems[i].Material.fY + (CG::fMenuFontHeight / 2), topRightX, topRightY, DynamicMenuElems[i].Text.fY, bTextMoveOverTime, dwMoveDuration, dwMoveStartTime);
			//}
		}
	}
}

/* Sets the position of a menu control, depending on its type */
VOID MenuControl::SetPosition(FLOAT x, FLOAT y, FLOAT topRightX, FLOAT topRightY, FLOAT textY, BOOL bMoveOverTime, DWORD dwMoveDuration, DWORD dwMoveStartTime) {
	this->topRightX = topRightX;
	this->topRightY = topRightY;
	switch (dwOptionType) {
		// Checkboxes have 2 elems, the background box and the checked icon
		case OPTION_CHECKBOX:
			Elems[0]->SetPos(x - Elems[0]->fWidth, y - (Elems[0]->fHeight / 2), bMoveOverTime, dwMoveDuration, dwMoveStartTime);
			Elems[1]->SetPos(Elems[0]->fX + (Elems[0]->fWidth - Elems[1]->fWidth)/2, Elems[0]->fY + (Elems[0]->fHeight - Elems[1]->fHeight)/ 2, bMoveOverTime, dwMoveDuration, dwMoveStartTime);
			break;
		// Sliders have 2 elems, the slider and the bar it slides on
		case OPTION_SLIDER:
			Elems[0]->SetPos(x - (fWidth * fScale), y - (Elems[0]->fHeight / 2), bMoveOverTime, dwMoveDuration, dwMoveStartTime);
			Elems[1]->SetPos(CG::bMovingMenu ? Elems[0]->fX : Elems[1]->fX, Elems[0]->fY - (fHeight*fScale) + (Elems[0]->fHeight / 2), bMoveOverTime, dwMoveDuration, dwMoveStartTime);
			Elems[2]->SetPos(x - (GameFuncs::GetTextWidth(Elems[2]->pText, Elems[2]->dwAsset) * fScale) - (fSliderWidth * fScale) - (7.0f * fScale), textY, bMoveOverTime, dwMoveDuration, dwMoveStartTime);
			break;
		// Lists have 1 elem, which is just the list items display name
		case OPTION_LIST:
		case OPTION_INFORMATION:
			Elems[0]->SetPos(x - (GameFuncs::GetTextWidth(Elems[0]->pText, Elems[0]->dwAsset) * fScale), textY, bMoveOverTime, dwMoveDuration, dwMoveStartTime);
			break;
		case OPTION_COLOR_PREVIEW:
		case OPTION_COLOR:
			Elems[0]->SetPos(x - Elems[0]->fWidth, y - (Elems[0]->fHeight / 2), bMoveOverTime, dwMoveDuration, dwMoveStartTime);
			break;
	}
}

FLOAT MenuManager::GetScrollbarOptionHeight() {
	DWORD dwNumOptions = GetMenu(dwCurrentMenu)->Options.GetSize();
	if (dwNumOptions > 1) {
		return (DynamicMenuElems[1].Material.fY - DynamicMenuElems[0].Material.fY); //((CG::fMenuHeight - (CG::fTopBottomBoxHeight * 2) - (CG::fMenuTextPadding * 4)) / (dwNumOptions));
	}
	return 0;
}

FLOAT MenuManager::GetScrollbarYPos(DWORD dwDisplayIndex) {
	FLOAT topRightX, topRightY;
	GetMenuPos(TOPRIGHT, &topRightX, &topRightY);
	return DynamicMenuElems[0].Material.fY + (GetScrollbarOptionHeight() * DynamicMenuElems[0].dwIndex); //topRightY + CG::fTopBottomBoxHeight + CG::fMenuTextPadding  + (GetScrollbarOptionHeight() * DynamicMenuElems[0].dwIndex);
}

FLOAT MenuManager::GetScrollbarHeight() {
	DWORD dwNumOptions = GetMenu(dwCurrentMenu)->Options.GetSize();
	FLOAT dwOptionHeight = GetScrollbarOptionHeight();// *(CG::GetFontScale() - (((CG::GetFontScale() - 1.0f)) * (CG::GetFontScale(1.199f)));
	if (dwNumOptions >= __MAXMENUOPTIONS__) {
		DWORD dwDifference = dwNumOptions - __MAXMENUOPTIONS__;
		return dwOptionHeight * (__MAXMENUOPTIONS__ - dwDifference);
	}
	return dwOptionHeight * dwNumOptions;
}

/* Sets the menus position (x, y is the CENTER of the menu) */
VOID MenuManager::SetMenuPosition(FLOAT x, FLOAT y, BOOL bMoveOverTime, DWORD dwMoveDuration, DWORD dwMoveStartTime, BOOL bTextMoveOverTime) {
	CG::dwMenuPos[0] = (DWORD)x;
	CG::dwMenuPos[1] = (DWORD)y;

	// Grab the coords of a few different positions where we want to draw stuff on the menu
	FLOAT topLeftX, topLeftY;
	GetMenuPos(TOPLEFT, &topLeftX, &topLeftY);
	FLOAT topCenterX, topCenterY;
	GetMenuPos(TOPCENTER, &topCenterX, &topCenterY);
	FLOAT bottomLeftX, bottomLeftY;
	GetMenuPos(BOTTOMLEFT, &bottomLeftX, &bottomLeftY);
	FLOAT topRightX, topRightY;
	GetMenuPos(TOPRIGHT, &topRightX, &topRightY);

	// Move our elems
	TitleText.SetPos(topCenterX - ((GameFuncs::GetTextWidth(TitleText.pText, CG::dwSmallFont) * CG::GetFontScale() ) / 2), topCenterY + CG::fMenuFontHeight + CG::fMenuTextPadding, bMoveOverTime, dwMoveDuration, dwMoveStartTime);
	TopSeparator.SetPos(topLeftX, topLeftY, bMoveOverTime, dwMoveDuration, dwMoveStartTime);
	BottomSeparator.SetPos(bottomLeftX, bottomLeftY - CG::fTopBottomBoxHeight /*- (((DWORD)Description.bNumLinesWrapped > 0) ? (FLOAT)((Description.bNumLinesWrapped - 1) * CG::fMenuFontHeight) : 0.0f)*/, bMoveOverTime, dwMoveDuration, dwMoveStartTime);
	Description.SetPos(bottomLeftX + (CG::fMenuTextPadding * 1.5f), bottomLeftY - CG::fMenuTextPadding /*- (((DWORD)Description.bNumLinesWrapped > 0) ? (FLOAT)((Description.bNumLinesWrapped - 1) * CG::fMenuFontHeight) : 0.0f)*/, bMoveOverTime, dwMoveDuration, dwMoveStartTime);
	Background.SetPos(topLeftX + CG::fOptionIndentAmount, topLeftY + CG::fTopBottomBoxHeight + CG::fOptionPadding, bMoveOverTime, dwMoveDuration, dwMoveStartTime);

	HudElem *pSelectedMaterial = NULL;
	DWORD dwSelection = GetMenu(dwCurrentMenu)->dwSelection;
	for (DWORD i = 0; i < ARRAY_SIZE(DynamicMenuElems); i++) {
		DynamicMenuElems[i].Material.SetPos(topLeftX + CG::fOptionIndentAmount, topLeftY + CG::fTopBottomBoxHeight + CG::fOptionSpacing + (i * CG::fMenuOptionHeight) + (i * CG::fOptionSpacing) + CG::fMenuTextPadding + (i + 1) , bMoveOverTime, dwMoveDuration, dwMoveStartTime);
		DynamicMenuElems[i].Text.SetPos(DynamicMenuElems[i].Material.fX + (CG::fMenuTextPadding * 1.5f), DynamicMenuElems[i].Material.fY + CG::fMenuFontHeight + (CG::fMenuOptionHeight / 2) - (CG::fMenuFontHeight / 2) , bTextMoveOverTime, dwMoveDuration, dwMoveStartTime);
		if (DynamicMenuElems[i].dwIndex == dwSelection) {
			pSelectedMaterial = &DynamicMenuElems[i].Material;
		}
	}

	// If we found our selection, set our slider and scrollbar positions based off its position
	if (pSelectedMaterial) {
		Slider.SetPos(pSelectedMaterial->fX, pSelectedMaterial->fY + 1, bMoveOverTime, dwMoveDuration, dwMoveStartTime);
		Scrollbar.SetPos(topRightX - 2.0f, GetScrollbarYPos(dwSelection), bMoveOverTime, dwMoveDuration, dwMoveStartTime);
	}

	// Set the position of our menu controls
	SetControlPosition(x, y , bMoveOverTime, dwMoveDuration, dwMoveStartTime, bTextMoveOverTime);
}

VOID MenuManager::SetMenuScale(BOOL bMoveOverTime, DWORD dwMoveDuration, DWORD dwStartTime) {
	// Update any elem properties that have the possibility of changing
	Description.fWrapWidth = CG::fMenuWidth - ((CG::fMenuTextPadding * 2.0f) * 1.5f);
	Description.fFontScale = CG::GetFontScale() ;
	TitleText.fFontScale = CG::GetFontScale() ;
	Scrollbar.SetScale(2.0f, GetScrollbarHeight(), bMoveOverTime, dwMoveDuration, dwStartTime);
	Background.SetScale(CG::fMenuWidth - (CG::fOptionIndentAmount * 2), CG::fMenuHeight - (CG::fTopBottomBoxHeight * 2), bMoveOverTime, dwMoveDuration, dwStartTime);
	Slider.SetScale(CG::fMenuWidth - (CG::fOptionIndentAmount * 2), CG::fMenuOptionHeight, bMoveOverTime, dwMoveDuration, dwStartTime);

	// We aren't going to ever scale this one over time, it looks weird with the changing  text sizes
	TopSeparator.SetScale(CG::fMenuWidth, CG::fTopBottomBoxHeight);// , bMoveOverTime, dwMoveDuration, dwStartTime);
	BottomSeparator.SetScale(CG::fMenuWidth, CG::fTopBottomBoxHeight + (((DWORD)Description.bNumLinesWrapped > 0) ? (FLOAT)((Description.bNumLinesWrapped - 1) * CG::fMenuFontHeight) : 0.0f));// , bMoveOverTime, dwMoveDuration, dwStartTime);
	
	// These arent scaled over time, and will likely never change.
	Slider.fBorderWidth = CG::fBorderWidth;
	Scrollbar.fBorderWidth = CG::fBorderWidth;
	Background.fBorderWidth = CG::fBorderWidth;
	TopSeparator.fBorderWidth = CG::fBorderWidth;
	BottomSeparator.fBorderWidth = CG::fBorderWidth;

	for (DWORD i = 0; i < ARRAY_SIZE(DynamicMenuElems); i++) {
		DynamicMenuElems[i].Material.SetPos(CG::fMenuWidth - (CG::fOptionIndentAmount * 2), CG::fMenuOptionHeight, bMoveOverTime, dwMoveDuration, dwStartTime);

		DynamicMenuElems[i].Material.fBorderWidth = CG::fBorderWidth;
		DynamicMenuElems[i].Text.fFontScale = CG::GetFontScale() ;
		if (DynamicMenuElems[i].Control) {
			DynamicMenuElems[i].Control->SetScale(CG::GetFontScale());// , bMoveOverTime, dwMoveDuration, dwStartTime);
		}
	}
}

VOID MenuManager::SetMenuColors(BOOL bEffects, DWORD dwTransitionLength, DWORD dwFadeStartTime) {
	memcpy(Background.pColor, CG::cMenuBackgroundColor, 0x10);
	memcpy(TopSeparator.pColor, CG::cTopBottomColor, 0x10);
	memcpy(BottomSeparator.pColor, CG::cTopBottomColor, 0x10);
	memcpy(Slider.pColor, CG::cMenuSliderColor, 0x10);
	memcpy(Scrollbar.pColor, CG::cScrollbarColor, 0x10);
	memcpy(TitleText.pColor, CG::cTitleTextColor, 0x10);
	memcpy(Description.pColor, CG::cDescriptionTextColor, 0x10);

	TopSeparator.pBorderColor = CG::cMenuBorderColor;
	BottomSeparator.pBorderColor = CG::cMenuBorderColor;

	for (DWORD i = 0; i < ARRAY_SIZE(DynamicMenuElems); i++) {
		
		if (DynamicMenuElems[i].dwIndex == Menus[dwCurrentMenu].dwSelection) {
			memcpy(DynamicMenuElems[i].Text.pColor, CG::cSelectedTextColor, 0x10);
		}
		else {
			memcpy(DynamicMenuElems[i].Text.pColor, CG::cMenuTextColor, 0x10);
		}
	}
}

/* Updates the visual appearance and location of all menu elems */
VOID MenuManager::UpdateMenuElems(DWORD dwNumOptions, BOOL bEffects) {
	// Copy over the new number of menu options to dwNumOptions, then grab the new menu width/height
	this->dwNumOptions = dwNumOptions;

	CG::fMenuFontHeight = GameFuncs::GetFontHeight(CG::dwSmallFont, CG::GetFontScale());
	CG::fOptionPadding = (CG::fMenuTextPadding / 8);
	CG::fMenuOptionHeight = (CG::fMenuFontHeight*0.75f) + (CG::fMenuTextPadding / 4);
	CG::fTopBottomBoxHeight = CG::fMenuFontHeight + (CG::fMenuTextPadding * 2);

	// We need to get all the other variables populated before our menu height, as they're all used in the calculation
	CG::fMenuHeight = GetHeight(dwNumOptions);
	CG::fMenuWidth = CG::fMenuWidth;

	//DbgPrint("MenuHeight: %f", CG::fMenuHeight);

	DWORD dwMoveStartTime = NiNJA.dwTickCount + 10;
	DWORD dwTransitionLength = 50;// 75;

	// Set the menus position based off the size
	SetMenuPosition((FLOAT)CG::dwMenuPos[0], (FLOAT)CG::dwMenuPos[1], bEffects, dwTransitionLength, dwMoveStartTime);

	// Set the menu scale
	SetMenuScale(bEffects, dwTransitionLength, dwMoveStartTime);

	// Set the menu colors
	SetMenuColors(bEffects, dwTransitionLength, dwMoveStartTime);
}

/* Sets the menus visibility, bFadeOverTime and dwFadeTime are unimplemented! */
VOID MenuManager::SetVisibility(bool vis, BOOL bFadeOverTime, DWORD dwFadeTime, DWORD dwStartTime) {
	// Set our menu visible boolean
	bMenuVisible = vis;

	// Setup all the elems visibility
	Border.bVisible = vis;
	Slider.bVisible = vis;
	Scrollbar.bVisible = vis;
	TitleText.bVisible = vis;
	Background.bVisible = vis;
	Description.bVisible = vis;
	TopSeparator.bVisible = vis;
	BottomSeparator.bVisible = vis;
	for (DWORD i = 0; i < __MAXMENUOPTIONS__; i++) {
		if (i < this->dwNumOptions) {
			//DynamicMenuElems[i].Material.visible = vis;
			DynamicMenuElems[i].Text.bVisible = vis;
			if (DynamicMenuElems[i].Control) {
				DynamicMenuElems[i].Control->SetVisible(vis);
				// If we're toggling the menu to visible, set the visual state of the items in our current menu
				if (vis) {
					GetMenu(dwCurrentMenu)->Options.Get(DynamicMenuElems[i].dwIndex)->SetVisualState();
				}
			}
		}
	}
}

VOID MenuManager::RefreshMenuText(int dwMenu, int dwNumOptions) {
	DWORD dwBackupMenu = dwCurrentMenu;
jRestart:
	for (DWORD i = 0; i < __MAXMENUOPTIONS__; i++) {
		if (dwBackupMenu != dwCurrentMenu) {
			goto jRestart;
		}
		if (i < (dwNumOptions == -1 ? this->dwNumOptions : dwNumOptions)) {
			Menu *pMenu = GetMenu(dwMenu == -1 ? this->dwCurrentMenu : dwMenu);
			if (pMenu) {
				MenuOption *pOption = pMenu->Options.Get(DynamicMenuElems[i].dwIndex);
				if (pOption) {
					DynamicMenuElems[i].Text.pText = pOption->GetDisplayName();
				}
			}
		}
	}
}
#pragma endregion

/* All functions associated with menu options */
#pragma region Menu Option Functions
enum eClientOptions {
	CLIENT_OPTION_PREFER,
	CLIENT_OPTION_WHITELIST,
#if defined(MW1) || defined(MW2) || defined(MW3)
	CLIENT_OPTION_INVERTTEAM,
#endif
	CLIENT_OPTION_STEALIP,
	CLIENT_OPTION_STEALGT,
	//CLIENT_OPTION_TELEPORT,
	CLIENT_OPTION_IP
};

DWORD dwNextUpdateTime = 0;
VOID MenuManager::UpdateClientMenu() {
	if (dwNextUpdateTime < GetTickCount()) {
		Menu *pClientMenu = &pThis->Menus[MENU_CLIENTS];
		CG::cLocationInfo = CG::cEntities.List[pClientMenu->dwSelection].pExt->cGeoIPInfo;
		pThis->Menus[MENU_SELECTED_CLIENT].DisplayName = CG::cEntities.List[pClientMenu->dwSelection].pExt->pRealName ? CG::cEntities.List[pClientMenu->dwSelection].pExt->pRealName : CG::cEntities.List[pClientMenu->dwSelection].pExt->GetName();
		pThis->Menus[MENU_SELECTED_CLIENT].Options.Get(CLIENT_OPTION_PREFER)->dwState = CG::cEntities.List[pClientMenu->dwSelection].pExt->bPreferred ? TRUE : FALSE;
		pThis->Menus[MENU_SELECTED_CLIENT].Options.Get(CLIENT_OPTION_WHITELIST)->dwState = CG::cEntities.List[pClientMenu->dwSelection].pExt->bWhitelisted ? TRUE : FALSE;
	#if defined(MW1) || defined(MW2) || defined(MW3)
		pThis->Menus[MENU_SELECTED_CLIENT].Options.Get(CLIENT_OPTION_INVERTTEAM)->dwState = CG::cEntities.List[pClientMenu->dwSelection].pExt->bInvertTeam ? TRUE : FALSE;
	#endif
		sprintf((PCHAR)pThis->Menus[MENU_SELECTED_CLIENT].Options.Get(CLIENT_OPTION_IP)->pSyncVariable, "%i.%i.%i.%i", CG::cEntities.List[pClientMenu->dwSelection].pExt->cIP.S_un.S_un_b.s_b1, CG::cEntities.List[pClientMenu->dwSelection].pExt->cIP.S_un.S_un_b.s_b2, CG::cEntities.List[pClientMenu->dwSelection].pExt->cIP.S_un.S_un_b.s_b3, CG::cEntities.List[pClientMenu->dwSelection].pExt->cIP.S_un.S_un_b.s_b4);
		UpdateMenuUI(NULL);
		dwNextUpdateTime = GetTickCount() + 2000;
	}
}

/* Sets up our selected client menu for the currently selected client, then opens it */
VOID MenuManager::EnterClientMenu(MenuOption *pOption) {
	Menu *pParent = (Menu*)pOption->pParent;

	if (CG::cEntities.List[pParent->dwSelection].pExt) {
		PCHAR pName = CG::cEntities.List[pParent->dwSelection].pExt->GetName();
		if (pName && pName[0] != 0) {
			DbgPrint("Entering client menu for client %i", pParent->dwSelection);

			// Force our ui thread to update the client menu settings next frame
			dwNextUpdateTime = GetTickCount();

			// Enter our client menu
			EnterMenu(MENU_SELECTED_CLIENT);
		}
	}
}

VOID MenuOption::EnterColorMenu(MenuOption *pOption) {
	// Setup the color menu for the option we just clicked to enter the color menu
	for (DWORD i = 0; i < 4; i++) {
		pThis->Menus[MENU_COLOR_PICKER].Options.Get(i)->pSyncVariable = (PDWORD)&pOption->GetPSyncVariable()[i];
	}
	// Setup the preview
	pThis->Menus[MENU_COLOR_PICKER].Options.Get(4)->pSyncVariable = pOption->GetPSyncVariable();
	pThis->EnterMenu(MENU_COLOR_PICKER);
}

VOID Whitelist(DWORD dwClient, bool bState) {
	if (!bState) {
		for (DWORD i = 0; i < CG::vWhitelisted.size(); i++) {
			if (CG::cEntities.List[dwClient].pExt->cXuid == CG::vWhitelisted[i]) {
				CG::vWhitelisted.erase(CG::vWhitelisted.begin() + i);
				return;
			}
		}
	}
	else {
		CG::vWhitelisted.push_back(CG::cEntities.List[dwClient].pExt->cXuid);
	}
}

VOID Prefer(DWORD dwClient, bool bState) {
	if (!bState) {
		for (DWORD i = 0; i < CG::vBlacklisted.size(); i++) {
			if (CG::cEntities.List[dwClient].pExt->cXuid == CG::vBlacklisted[i]) {
				CG::vBlacklisted.erase(CG::vBlacklisted.begin() + i);
				return;
			}
		}
	}
	else {
		CG::vBlacklisted.push_back(CG::cEntities.List[dwClient].pExt->cXuid);
	}
}

/* Handles any option that is toggled from the selected client submenu */
VOID MenuManager::ClientMenuHandler(MenuOption *pOption) {
	Menu *pParent = (Menu*)pOption->pParent;
	DWORD selectedClient = pThis->Menus[MENU_CLIENTS].dwSelection;
	CHAR pGtBuffer[50];

	switch (pParent->dwSelection) {
		case CLIENT_OPTION_PREFER:
			Prefer(selectedClient, pOption->dwState == TRUE);
			CG::cEntities.List[selectedClient].pExt->bPreferred = pOption->dwState == TRUE;
			DbgPrint("Client %i is%s preferred!", selectedClient, pOption->dwState ? "" : " not");
			break;
		case CLIENT_OPTION_WHITELIST:
			Whitelist(selectedClient, pOption->dwState == TRUE);
			CG::cEntities.List[selectedClient].pExt->bWhitelisted = pOption->dwState == TRUE;
			DbgPrint("Client %i is%s whitelisted!", selectedClient, pOption->dwState ? "" : " not");
			break;
	#if defined(MW1) || defined(MW2) || defined(MW3)
		case CLIENT_OPTION_INVERTTEAM:
			CG::cEntities.List[selectedClient].pExt->bInvertTeam = pOption->dwState == TRUE;
			break;
	#endif
		case CLIENT_OPTION_STEALIP:
			ZeroMemory(pGtBuffer, 50);
			sprintf(pGtBuffer, "%i.%i.%i.%i=%s", CG::cEntities.List[selectedClient].pExt->cIP.S_un.S_un_b.s_b1, CG::cEntities.List[selectedClient].pExt->cIP.S_un.S_un_b.s_b2, CG::cEntities.List[selectedClient].pExt->cIP.S_un.S_un_b.s_b3, CG::cEntities.List[selectedClient].pExt->cIP.S_un.S_un_b.s_b4, Contains(CG::cEntities.List[selectedClient].pExt->pName, "=") ? CG::cEntities.List[selectedClient].pExt->pRealName : CG::cEntities.List[selectedClient].pExt->pName);
			GameFuncs::SetGamertag(pGtBuffer);
			break;
		case CLIENT_OPTION_STEALGT:
			GameFuncs::SetGamertag(CG::cEntities.List[selectedClient].pExt->pName);
			break;
		/*case CLIENT_OPTION_TELEPORT:
			GameFuncs::TeleportTo(selectedClient);
			DbgPrint("Teleported to client %i!", selectedClient);
			break;*/
	}
}

VOID MenuManager::UpdateMenuUI(MenuOption *pOption) {
	// Update our menu elems
	pThis->EnterMenu(pThis->dwCurrentMenu, FALSE, FALSE);
}

VOID SetVariableColor(FLOAT fValue, FLOAT *fOut) {
	FLOAT newVec[4];
	newVec[3] = fOut[3];

	unsigned int color = rgb(fValue / 500.0f);
	PBYTE bColor = (PBYTE)&color;

	newVec[0] = (FLOAT)bColor[1] / 255.0f;
	newVec[1] = (FLOAT)bColor[2] / 255.0f;
	newVec[2] = (FLOAT)bColor[3] / 255.0f;

	memcpy(fOut, newVec, 0x10);
}

VOID SetSelectedTextColor(MenuOption *pOption) {
	SetVariableColor(*(FLOAT*)pOption->GetPSyncVariable(), CG::cSelectedTextColor);
}

VOID MenuManager::UpdateMenuStates() {
	for (DWORD m = 0; m < MENU_SIZE; m++) {
		Menu *menu = &Menus[m];
		if (menu && menu->bSave && menu->bDisplay) {
			DWORD dwSize = GetMenuOptions(m)->GetSize();
			for (DWORD i = 0; i < dwSize; i++) {
				MenuOption *option = GetMenuOptions(m)->Get(i);
				if (option) {
					option->SetState();
					if ((option->dwSaveFlags & FLAG_SINGLE) && (option->dwSaveFlags & FLAG_TYPE_DWORD) && (option->dwFlags & FLAG_CALLONTOGGLE)) {
						option->Call();
					}
					option->SetVisualState();
				}
			}
		}
	}
}

VOID MenuManager::SetAimbotConfig(MenuOption *pOption) {
	Menu *pAimbotMenu = (Menu*)pOption->pParent;
	switch (pOption->dwState) {
		case 0: // Rage
			CG::dwAimbotType = AIM_TYPE_SILENT;
			CG::dwAimBone = TAG_HEAD;
			CG::bTargetBestBone = TRUE;
			CG::bAimbotWhenFiring = TRUE;
			CG::bOffscreenTargeting = TRUE;
			CG::bAutowall = TRUE;
			CG::bNoSpread = TRUE;
			CG::bAutoShoot = TRUE;
			CG::bTriggerbot = FALSE;
			CG::bNoRecoil = TRUE;
			CG::bNoSway = TRUE;
			CG::bRapidFire = TRUE;
			CG::bFastReload = TRUE;
			CG::bFakeLag = FALSE;
			CG::bAntiAim = TRUE;
			CG::dwYawSpinSpeed = 100;

			DbgPrint("Config has been set to RAGE!");
			break;
		case 1: // HvH
			CG::dwAimbotType = AIM_TYPE_SILENT;
			CG::dwAimBone = TAG_NECK;
			CG::bTargetBestBone = TRUE;
			CG::bAimbotWhenFiring = TRUE;
			CG::bOffscreenTargeting = TRUE;
			CG::bAutowall = TRUE;
			CG::bNoSpread = TRUE;
			CG::bAutoShoot = TRUE;
			CG::bTriggerbot = FALSE;
			CG::bNoRecoil = TRUE;
			CG::bNoSway = TRUE;
			CG::bRapidFire = TRUE;
			CG::bFastReload = TRUE;
			CG::bFakeLag = TRUE;
			CG::dwPacketSkipAmount = 8;
			CG::bAntiAim = TRUE;
			CG::dwYawSpinSpeed = 100;
			CG::dwAutoCrouch = AUTO_CROUCH_OFF;
			DbgPrint("Config has been set to Hack vs Hack!");
			break;
		case 2: // Legit
			CG::dwAimbotType = AIM_TYPE_HUMANIZED;
			CG::dwAimBone = TAG_NECK;
			CG::bTargetBestBone = TRUE;
			CG::bAimbotWhenFiring = FALSE;
			CG::bOffscreenTargeting = FALSE;
			CG::bAutowall = FALSE;
			CG::bNoSpread = FALSE;
			CG::bAutoShoot = FALSE;
			CG::bTriggerbot = TRUE;
			CG::bNoRecoil = FALSE;
			CG::bNoSway = FALSE;
			CG::bRapidFire = FALSE;
			CG::bFastReload = FALSE;
			CG::bFakeLag = FALSE;
			CG::dwPitchSpinMode = PITCH_OFF;
			CG::dwYawSpinMode = YAW_OFF;
			CG::dwRollSpinMode = ROLL_OFF;
			CG::bAntiAim = FALSE;
			DbgPrint("Config has been set to LEGIT!");
			break;
		case 3: // Rage Sniper
			CG::dwAimbotType = AIM_TYPE_SILENT;
			CG::dwAimBone = TAG_HEAD;
			CG::bTargetBestBone = FALSE;
			CG::bAimbotWhenFiring = TRUE;
			CG::bOffscreenTargeting = TRUE;
			CG::bAutowall = TRUE;
			CG::bNoSpread = TRUE;
			CG::bAutoShoot = TRUE;
			CG::bTriggerbot = FALSE;
			CG::bNoRecoil = TRUE;
			CG::bNoSway = TRUE;
			CG::bRapidFire = TRUE;
			CG::bFastReload = TRUE;
			CG::bFakeLag = FALSE;
			CG::bAntiAim = TRUE;
			CG::dwYawSpinSpeed = 100;
			DbgPrint("Config has been set to RAGE SNIPER!");
			break;
		case 4: // Legit Quickscoper
			CG::dwAimbotType = AIM_TYPE_HUMANIZED;
			CG::dwAimBone = TAG_NECK;
			CG::bTargetBestBone = TRUE;
			CG::bAimbotWhenFiring = FALSE;
			CG::bOffscreenTargeting = TRUE;
			CG::bAutowall = FALSE;
			CG::bNoSpread = FALSE;
			CG::bAutoShoot = FALSE;
			CG::bTriggerbot = TRUE;
			CG::bNoRecoil = FALSE;
			CG::bNoSway = FALSE;
			CG::bRapidFire = FALSE;
			CG::bFastReload = FALSE;
			CG::bFakeLag = FALSE;
			CG::dwPitchSpinMode = PITCH_OFF;
			CG::dwYawSpinMode = YAW_OFF;
			CG::dwRollSpinMode = ROLL_OFF;
			CG::bAntiAim = FALSE;
			DbgPrint("Config has been set to LEGIT SNIPER!");
			break;
		default:
			return;
	}
	// Update our menu ui based on our new settings
	pThis->UpdateMenuStates();
}

VOID MenuOption::EnterMenu_Internal(MenuOption *pOption) {
	// Enter the menu 
	pThis->EnterMenu(pOption->dwState);
}

DWORD MenuOption::Toggle(BOOL up) {
	int selection = (int)dwState;
	int numVariations = 0;
	BOOL bSlider = FALSE;
	switch (dwOptionType) {
	case OPTION_LIST:
		numVariations = (int)Variations.size();
		break;
	case OPTION_CHECKBOX:
		numVariations = 2;
		break;
	case OPTION_SLIDER:
		bSlider = TRUE;
		break;
	default:
		DbgPrint("Tried to toggle an option that cannot be toggled!");
		return 0;
	}

	if (!bSlider) {
		if (numVariations > 0) {
			if (up) {
				selection++;
				if (selection >= numVariations) {
					selection = 0;
				}
			}
			else
			{
				selection--;
				if (selection < 0) {
					selection = numVariations - 1;
				}
			}
		}
		else {
			selection = 0;
		}
	}
	else {
		FLOAT fValue = NULL;
		if (dwSaveFlags & FLAG_TYPE_FLOAT) {
			fValue = *(FLOAT*)GetPSyncVariable();
		}
		else if (dwSaveFlags & FLAG_TYPE_DWORD) {
			fValue = (FLOAT)*(int*)GetPSyncVariable();
		}

		fValue = (up ? fValue + fIncrement : fValue - fIncrement);

		if (fValue > fRangeMax) {
			fValue = fRangeMax;
		}
		else if (fValue < fRangeMin) {
			fValue = fRangeMin;
		}

		if (dwSaveFlags & FLAG_TYPE_FLOAT) {
			*(FLOAT*)GetPSyncVariable() = fValue;
			DbgPrint("Toggled %s to %f", pDisplayName, fValue);
		}
		else if (dwSaveFlags & FLAG_TYPE_DWORD) {
			*(int*)GetPSyncVariable() = (int)fValue;
			DbgPrint("Toggled %s to %i", pDisplayName, (int)fValue);
		}
	}

	// Set our final state
	dwState = (DWORD)selection;

	if (!bSlider) {
		DbgPrint("Toggled %s to %i", pDisplayName, dwState);

		if ((dwFlags & FLAG_SYNC) && (dwFlags & FLAG_SINGLE)) {
			*(DWORD*)GetPSyncVariable() = dwState;
		}
	}

	if (bSlider) {
		// Make sure our UI is properly updated
		pThis->UpdateMenuUI(this);
	}
	else {
		SetVisualState();
	}

	// Call the function associated with this option if this function is set to be called on toggle
	if ((dwFlags & FLAG_CALLFUNC) && (dwFlags & FLAG_CALLONTOGGLE)) {
		Call();
	}

	return dwState;
}

VOID MenuManager::LaunchCustomGTEntry(MenuOption *pOption) {
	CG::pCustomGTMenuOption = pOption;
	CG::bLaunchCustomGTEntry = true;
}

VOID MenuManager::ClearGTEntry(MenuOption *pOption) {
	memcpy((PVOID)pOption->GetPSyncVariable(), __EMPTYGTSTRING__, 9);
	pOption->SetState();
	pThis->EnterMenu(pThis->Menus[pThis->dwCurrentMenu].mLastMenu, TRUE, FALSE);
}

VOID MenuManager::GamertagMenuHandler(MenuOption *pOption) {
	if (Compare((PCHAR)pOption->GetPSyncVariable(), __EMPTYGTSTRING__)) {
		if (pOption->dwLastButtonPressed == BUTTON_A) {
			LaunchCustomGTEntry(pOption);
		}
	}
	else {
		if (pOption->dwLastButtonPressed == BUTTON_A) {
			DWORD dwNumOptions = pThis->Menus[MENU_GAMERTAG_EDITOR].Options.GetSize();
			for (DWORD i = 0; i < dwNumOptions; i++) {
				MenuOption *option = pThis->Menus[MENU_GAMERTAG_EDITOR].Options.Get(i);
				if (option) {
					option->pSyncVariable = pOption->GetPSyncVariable();
				}
			}
			EnterMenu(MENU_GAMERTAG_EDITOR);
		}
		else if (pOption->dwLastButtonPressed == BUTTON_X) {
			GameFuncs::SetGamertag((PCHAR)pOption->GetPSyncVariable());
		}
	}
}

VOID MenuManager::ResetGT(MenuOption *pOption) {
	CG::bResetGamertag = true;
}

VOID MenuManager::SetFOVValue(MenuOption *pOption) {
	GameFuncs::SetFOV(CG::fFovValue);
}

VOID MenuManager::SetPrestigeValue(MenuOption *pOption) {
	GameFuncs::SetPrestige(CG::dwPrestigeValue);
}

VOID MenuManager::CenterMenu(MenuOption *pOption) {
	// Set our initial menu position, incase it isn't initialized by the config file
	CG::dwMenuPos[0] = NiNJA.dwScreenCenterX;
	CG::dwMenuPos[1] = NiNJA.dwScreenCenterY - 20.0f;

	UpdateMenuUI(pOption);
}

VOID MenuManager::TestRenderFlags(MenuOption *pOption) {
	CG::iTestVal++;
	DbgPrintE("iRenderFlagTest: %X", CG::iTestVal);
}

VOID MenuManager::UnloadXex(MenuOption *pOption) {
	NiNJA.bTerminateProcess = TRUE;
}

VOID MenuManager::SendTestCommand(MenuOption *pOption) {
	CG::bSendCustomCommand = TRUE;
}

VOID MenuManager::DumpXModel(MenuOption *pOption) {
#if defined(ADMIN)
	if (CG::bInGameCached) {
		int iXModel = GameFuncs::GetXModelForDObj(GameFuncs::Get_ClientDObj(CG::iRealLocalClient));
		if (iXModel) {
			int iNumBones = GameFuncs::XModelGetNumBones(iXModel);
			printf("Bones:\n");
			for (int i = 0; i < iNumBones; i++) {
				printf(GameFuncs::va("CG::cTags.List.push_back(CachedTag(EA(\"%s\"), TRUE, TRUE, 500));\n", GameFuncs::XModelGetBoneName(iXModel, i)));
			}
		}
	}
#endif
}
#pragma endregion

/* The menu initialization and cleanup functions */
#pragma region Menu Initialization & Cleanup

/* Initializes all menu settings and sets up all our menu options dynamically, then
reads our config file, and finally prepares the UI objects associated with the menu */
FLOAT fTmpVal;
VOID MenuManager::Initialize() {
	//// Init our NiNJA Engine logo
	//NiNJAElem.dwFlags = ELEM_TYPE_TEXT | ELEM_FLAG_ENCRYPTED; // Our text is now encrypted in memory all the time. it's only decrypted for a split second when its being drawn :)
	//NiNJAElem.pSecureValue = EROT13("NiNJA Engine v2.0.1");
	//NiNJAElem.dwAsset = CG::dwSmallFont;
	//memcpy(NiNJAElem.pColor, Colors::White, 0x10);
	////NiNJAElem.rainbowFade = true;
	//NiNJAElem.fFontScale = 0.7f;
	//NiNJAElem.fX = NiNJA.dwScreenMaxX - 160;
	//NiNJAElem.fY = 35;
	//NiNJAElem.bVisible = TRUE;
	//HudElems::InitHudElem(&NiNJAElem);

	// Cache our fonts and materials
	CG::dwWhite = GameFuncs::CacheMaterial(ES("white"));
	CG::dwArrowIcon = GameFuncs::CacheMaterial(__ARROWICON__);
	CG::dwSmallFont = GameFuncs::CacheFont(__SMALLFONT__);
	if (__SCAVENGERHUDNAME__ != "") {
		CG::dwScavPackMaterial = GameFuncs::CacheMaterial(__SCAVENGERHUDNAME__);
	}

	pThis = this;
	bUpOneLevel = false;
	bShouldRead = FALSE;
	bShouldSave = FALSE;
	dwNumOptions = 0;
	dwCurrentMenu = MENU_MAIN;

	// Initialize some static menu strings
	PCHAR sDefault = EA("Default");
	PCHAR sOff = EA("Off");
	PCHAR sBoth = EA("Both");
	PCHAR sNormal = EA("Normal");
	PCHAR sNone = EA("None");
	PCHAR sSelected = EA("Selected");
	PCHAR sBest = EA("Best");
	PCHAR sRage = EA("Rage");
	PCHAR sLegit = EA("Legit");
	PCHAR sFriendly = EA("Friendly");
	PCHAR sEnemy = EA("Enemy");
	PCHAR sLeft = EA("Spin Left");
	PCHAR sRight = EA("Spin Right");
	PCHAR sFlip = EA("Flip Flop");
	PCHAR pRandom = EA("Random");

	// Cache some tags
	CG::cTags.List.push_back(CachedTag(EA("j_head_end"), TRUE, TRUE, 2000));
	CG::cTags.List.push_back(CachedTag(EA("j_head"), TRUE, TRUE, 2000));
	CG::cTags.List.push_back(CachedTag(EA("j_helmet"), TRUE, TRUE, 2000));
	CG::cTags.List.push_back(CachedTag(EA("j_neck"), TRUE, TRUE, 2000));
	CG::cTags.List.push_back(CachedTag(EA("j_ankle_le"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_ankle_ri"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_wrist_le"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_wrist_ri"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_hip_le"), TRUE, TRUE, 1000));
	CG::cTags.List.push_back(CachedTag(EA("j_hip_ri"), TRUE, TRUE, 1000));
	CG::cTags.List.push_back(CachedTag(EA("j_knee_le"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_knee_ri"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_mainroot"), TRUE, TRUE, 1000));
	CG::cTags.List.push_back(CachedTag(EA("j_elbow_le"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_elbow_ri"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_shoulder_le"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_shoulder_ri"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_spinelower"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_spineupper"), TRUE, TRUE, 1000));
	//CG::cTags.List.push_back(CachedTag(EA("j_jaw"), TRUE, TRUE, 2000)); 


	// More fucking bones, only for prioritized people
	//CG::cTags.List.push_back(CachedTag(EA("j_hiptwist_le"), TRUE, TRUE, 500)); 
	//CG::cTags.List.push_back(CachedTag(EA("j_hiptwist_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_spine4"), TRUE, TRUE, 500)); 
	CG::cTags.List.push_back(CachedTag(EA("j_ball_le"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_ball_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_clavicle_le"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_clavicle_ri"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_shoulderraise_le"), TRUE, TRUE, 1000));
	CG::cTags.List.push_back(CachedTag(EA("j_shoulderraise_ri"), TRUE, TRUE, 1000));
	CG::cTags.List.push_back(CachedTag(EA("j_brow_le"), TRUE, TRUE, 2000));
	CG::cTags.List.push_back(CachedTag(EA("j_brow_ri"), TRUE, TRUE, 2000));
	//CG::cTags.List.push_back(CachedTag(EA("j_elbow_bulge_le"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_elbow_bulge_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("tag_origin"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("pelvis"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("tag_sync"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("torso_stabilizer"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("back_low"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("back_mid"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_knee_bulge_le"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_knee_bulge_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("tag_inhand"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("tag_stowed_back"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("tag_weapon_chest"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_cheek_le"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_cheek_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_eyeball_le"), TRUE, TRUE, 2000));
	//CG::cTags.List.push_back(CachedTag(EA("j_eyeball_ri"), TRUE, TRUE, 2000));
	//CG::cTags.List.push_back(CachedTag(EA("j_eye_lid_bot_le"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_eye_lid_bot_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_eye_lid_top_le"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_eye_lid_top_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_levator_le"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_levator_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_lip_bot_le"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_lip_bot_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_lip_top_le"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_lip_top_ri"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_mouth_le"), TRUE, TRUE, 2000));
	//CG::cTags.List.push_back(CachedTag(EA("j_mouth_ri"), TRUE, TRUE, 2000));
	//CG::cTags.List.push_back(CachedTag(EA("j_shouldertwist_le"), TRUE, TRUE, 1000));
	//CG::cTags.List.push_back(CachedTag(EA("j_shouldertwist_ri"), TRUE, TRUE, 1000));
	//CG::cTags.List.push_back(CachedTag(EA("tag_eye"), TRUE, TRUE, 2000));
	//CG::cTags.List.push_back(CachedTag(EA("j_wristtwist_le"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_wristtwist_ri"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_index_le_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_index_ri_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_mid_le_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_mid_ri_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_pinky_le_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_pinky_ri_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_ring_le_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_ring_ri_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_thumb_le_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("j_thumb_ri_1"), TRUE, TRUE, 500));


	// Custom tags, grabbed by index
	CG::cTags.List.push_back(CachedTag(EA("n_1"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_2"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_3"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_4"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_5"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_6"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_7"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_8"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_9"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_10"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_11"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_12"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_13"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_14"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_15"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_16"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_17"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_18"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_19"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_20"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_21"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_22"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_23"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_24"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_25"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_26"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_27"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_28"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_29"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_30"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_31"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_32"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_33"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_34"), TRUE, TRUE, 500));
	CG::cTags.List.push_back(CachedTag(EA("n_35"), TRUE, TRUE, 500));

	//CG::cTags.List.push_back(CachedTag(EA("j_index_le_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_index_ri_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_mid_le_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_mid_ri_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_pinky_le_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_pinky_ri_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_ring_le_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_ring_ri_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_thumb_le_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_thumb_ri_2"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_index_le_3"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_index_ri_3"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_mid_le_3"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_mid_ri_3"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_pinky_le_3"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_pinky_ri_3"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_ring_le_3"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_ring_ri_3"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_thumb_le_3"), TRUE, TRUE, 500));
	//CG::cTags.List.push_back(CachedTag(EA("j_thumb_ri_3"), TRUE, TRUE, 500));


	//CG::cTags.List.push_back(CachedTag(pTagShield, FALSE));

	DbgPrint("Tags initialized!");

	// Settings
	Menus[MENU_SETTINGS] = Menu(EA("Settings"));
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Gamertag Manager"), OPTION_SUBMENU))->SetDescription(EA("Choose the color of your gamertag, or just change it to whatever you want!"))->EnterMenu(MENU_GAMERTAG_MANAGER);
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Menu Settings"), OPTION_SUBMENU))->SetDescription(EA("Customize the look and feel of NiNJA Engine."))->EnterMenu(MENU_UI_SETTINGS);
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Infinite Class Items"), OPTION_CHECKBOX))->SaveAs(EA("InfiniteClassItems"))->SetDescription(EA("Use as many perks, attachments, and equipment as you want when creating a class."))->EnabledOn(TITLE_BO2)->CallFunc(&GameFuncs::ToggleClassItems, TRUE)->SyncWith(&CG::bUnlimitedClassItems);
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Log IPs"), OPTION_CHECKBOX))->SaveAs(EA("LogIPs"))->SetDescription(EA("Automatically logs the IP addresses of any player you join a lobby with. Logs will be outputted to the IP-Logs folder."))->SyncWith(&CG::bLogIPs);
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Show Real GTs"), OPTION_CHECKBOX))->SaveAs(EA("ShowRealGTs"))->SetDescription(EA("Show clients real gamertags even if they have spoofed them to something else."))->SyncWith(&CG::bShowRealNames);
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Never Host"), OPTION_CHECKBOX))->SaveAs(EA("NeverHost"))->SetDescription(EA("You will NEVER receive host from host migrations, or searching for a match. Disable before forcing host!"))->CallFunc(&GameFuncs::ToggleNeverHost, TRUE)->SyncWith(&CG::bNeverHost);
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Spoof IP"), OPTION_CHECKBOX))->SaveAs(EA("SpoofIP"))->SetDescription(EA("Hides your real IP address from IP grabber tools."))->EnabledOn(TITLE_BO2)->CallFunc(&GameFuncs::ToggleNeverHost, TRUE)->SyncWith(&CG::bSpoofIP);
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Custom Sound FX"), OPTION_CHECKBOX))->SaveAs(EA("CustomSoundFX"))->SetDescription(EA("Play custom killstreak sounds from Halo as you get killing streaks/sprees."))->SyncWith(&CG::bCustomSoundFX);
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Sound Volume"), OPTION_SLIDER))->SaveAs(EA("SoundVolume"), FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("Choose the volume for all custom sound FX played ingame."))->SetRange(0.0f, 7.0f, 0.5f)->ValidateFloat()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::fSoundVolume);
	Menus[MENU_SETTINGS].AddOption(MenuOption(EA("Exit NiNJA Engine")))->SetDescription(EA("Unloads NiNJA Engine until the next time you relaunch the game."))->CallFunc(&UnloadXex);

	// UI Settings
	Menus[MENU_UI_SETTINGS] = Menu(EA("Menu Settings"), TRUE, TRUE);
	Menus[MENU_UI_SETTINGS].AddOption(MenuOption(EA("Menu Colors"), OPTION_SUBMENU))->SetDescription(EA("Choose the colors of the menu, and all of its components!"))->EnterMenu(MENU_UI_COLORS);
	Menus[MENU_UI_SETTINGS].AddOption(MenuOption(EA("Analog Scrolling"), OPTION_CHECKBOX))->SaveAs(EA("AnalogScrolling"))->SetDescription(EA("Allows you to additionally use the left analog stick to scroll through menu options; always enabled pre game."))->SyncWith(&CG::bAnalogControl);
	Menus[MENU_UI_SETTINGS].AddOption(MenuOption(EA("Font Scale"), OPTION_SLIDER))->SaveAs(EA("FontScale"), FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("Adjust the font size of menu text and components."))->SetRange(0.7f, 1.2f, 0.1f)->ValidateFloat()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::fFontScale);
	Menus[MENU_UI_SETTINGS].AddOption(MenuOption(EA("Menu Padding"), OPTION_SLIDER))->SaveAs(NULL, FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("Adjust the amount of padding used for menu text, controls, and any other element of the menu user interface."))->SetRange(2.0f, 12.0f, 2.0f)->ValidateFloat()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::fMenuTextPadding);
	Menus[MENU_UI_SETTINGS].AddOption(MenuOption(EA("Menu Position"), OPTION_SUBMENU))->SetDescription(EA("Choose where the menu is displayed on your screen."))->EnterMenu(MENU_POSITION);
	Menus[MENU_UI_SETTINGS].AddOption(MenuOption(EA("Menu Width"), OPTION_SLIDER))->SaveAs(EA("Width"), FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("Choose the width of the menu."))->SetRange(250.0f, 600.0f, 10.0f)->ValidateFloat()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::fMenuWidth);

	Menus[MENU_POSITION] = Menu(EA("Menu Position"), TRUE, TRUE);
	Menus[MENU_POSITION].AddOption(MenuOption(EA("Custom Position"), OPTION_CHECKBOX))->SetDescription(EA("Press [[BUTTON_A]] to start moving the menu, and to save its position."))->SyncWith(&CG::bMovingMenu);
	Menus[MENU_POSITION].AddOption(MenuOption(EA("Coordinates")))->SaveAs(NULL, FLAG_TYPE_DWORD | FLAG_VEC2)->DisabledOn(FLAG_ALL_COD)->SyncWith(&CG::dwMenuPos);
	Menus[MENU_POSITION].AddOption(MenuOption(EA("Center")))->SetDescription(EA("Resets the menu position to the center of the screen."))->CallFunc(&CenterMenu);

	// Menu Colors
	Menus[MENU_UI_COLORS] = Menu(EA("Menu Colors"), TRUE, TRUE);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Background"), OPTION_COLOR))->SaveAs(EA("Background"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the background color of the menu."))->ChooseColor(CG::cMenuBackgroundColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Border Color"), OPTION_COLOR))->SaveAs(EA("BorderColor"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the border color of anything that has a border in the menu."))->ChooseColor(CG::cMenuBorderColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Control Color"), OPTION_COLOR))->SaveAs(EA("ControlColor"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the color of menu controls."))->ChooseColor(CG::cControlColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Description Text"), OPTION_COLOR))->SaveAs(EA("DescriptionText"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the color of the menu option description text."))->ChooseColor(CG::cDescriptionTextColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Information Text"), OPTION_COLOR))->SaveAs(EA("TopRightText"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Set the color of the NiNJA engine logo and other info in the top right corner."))->ChooseColor(CG::cTopRightTextColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Information Background"), OPTION_COLOR))->SaveAs(EA("TopRightBackground"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Set the background color of the NiNJA engine logo and other info in the top right corner."))->ChooseColor(CG::cTopRightBackgroundColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Scrollbar"), OPTION_COLOR))->SaveAs(EA("Scrollbar"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the color of the menus scrollbar."))->ChooseColor(CG::cScrollbarColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Selected Text"), OPTION_COLOR))->SaveAs(EA("SelectedText"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the color of the menu text when an option is selected."))->ChooseColor(CG::cSelectedTextColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Slider"), OPTION_COLOR))->SaveAs(EA("Slider"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the color of the menu slider."))->ChooseColor(CG::cMenuSliderColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Text"), OPTION_COLOR))->SaveAs(EA("Text"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the color of the menu text when nothing is selected."))->ChooseColor(CG::cMenuTextColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Title Text"), OPTION_COLOR))->SaveAs(EA("TitleText"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the color of the menus title text."))->ChooseColor(CG::cTitleTextColor);
	Menus[MENU_UI_COLORS].AddOption(MenuOption(EA("Top/Bottom"), OPTION_COLOR))->SaveAs(EA("TopBottom"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the background color of the title text, and description text."))->ChooseColor(CG::cTopBottomColor);

	// ESP Customization
	Menus[MENU_ESP_CUSTOMIZATION] = Menu(EA("ESP Customization"), TRUE, TRUE);
	Menus[MENU_ESP_CUSTOMIZATION].AddOption(MenuOption(EA("Colors"), OPTION_SUBMENU))->SetDescription(EA("Don't like the ESP colors? Customize them to look however you want!"))->EnterMenu(MENU_ESP_COLORS);
	Menus[MENU_ESP_CUSTOMIZATION].AddOption(MenuOption(EA("Name Scale"), OPTION_SLIDER))->SaveAs(EA("NameScale"), FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("Choose the size of overhead names drawn over players & entities."))->SetRange(0.5f, 1.2f, 0.1f)->ValidateFloat()->SyncWith(&CG::fESPNameScale);

	// ESP Colors
	Menus[MENU_ESP_COLORS] = Menu(EA("ESP Colors"), TRUE, TRUE);
	Menus[MENU_ESP_COLORS].AddOption(MenuOption(EA("Enemy"), OPTION_COLOR))->SaveAs(EA("Enemy"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the enemy ESP color when not visible, wallbangable, preferred, or whitelisted."))->ChooseColor(CG::cEnemyEspColor);
	Menus[MENU_ESP_COLORS].AddOption(MenuOption(EA("Friendly"), OPTION_COLOR))->SaveAs(EA("Friendly"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the ESP color for friendlies."))->ChooseColor(CG::cFriendlyEspColor);
	Menus[MENU_ESP_COLORS].AddOption(MenuOption(EA("Preferred"), OPTION_COLOR))->SaveAs(EA("Preferred"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the ESP color for clients you have set as preferred targets."))->ChooseColor(CG::cPreferredEspColor);
	Menus[MENU_ESP_COLORS].AddOption(MenuOption(EA("Wallbangable"), OPTION_COLOR))->SaveAs(EA("Wallbangable"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the ESP color when a target is wallbangable."))->ChooseColor(CG::cWallbangableEspColor);
	Menus[MENU_ESP_COLORS].AddOption(MenuOption(EA("Whitelisted"), OPTION_COLOR))->SaveAs(EA("Whitelisted"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the ESP color for clients you have whitelisted from being targeted."))->ChooseColor(CG::cWhitelistedEspColor);
	Menus[MENU_ESP_COLORS].AddOption(MenuOption(EA("Visible"), OPTION_COLOR))->SaveAs(EA("Visible"), FLAG_TYPE_FLOAT | FLAG_VEC4)->SetDescription(EA("Choose the ESP color when a target is visible."))->ChooseColor(CG::cVisibleEspColor);

	// Color picker
	Menus[MENU_COLOR_PICKER] = Menu(EA("Choose a Color"), FALSE, TRUE);
	Menus[MENU_COLOR_PICKER].AddOption(MenuOption(EA("Red"), OPTION_SLIDER))->SetDescription(EA("Choose the amount of red in the final color."))->SetRange(0.0f, 1.0f, 0.01f)->SyncWith(&fTmpVal)->SetupDynamicFloatingSlider()->CallFunc(&UpdateMenuUI, TRUE);
	Menus[MENU_COLOR_PICKER].AddOption(MenuOption(EA("Green"), OPTION_SLIDER))->SetDescription(EA("Choose the amount of green in the final color."))->SetRange(0.0f, 1.0f, 0.01f)->SyncWith(&fTmpVal)->SetupDynamicFloatingSlider()->CallFunc(&UpdateMenuUI, TRUE);
	Menus[MENU_COLOR_PICKER].AddOption(MenuOption(EA("Blue"), OPTION_SLIDER))->SetDescription(EA("Choose the amount of blue in the final color."))->SetRange(0.0f, 1.0f, 0.01f)->SyncWith(&fTmpVal)->SetupDynamicFloatingSlider()->CallFunc(&UpdateMenuUI, TRUE);
	Menus[MENU_COLOR_PICKER].AddOption(MenuOption(EA("Alpha"), OPTION_SLIDER))->SetDescription(EA("Choose the transparency of the final color."))->SetRange(0.0f, 1.0f, 0.01f)->SyncWith(&fTmpVal)->SetupDynamicFloatingSlider()->CallFunc(&UpdateMenuUI, TRUE);
	Menus[MENU_COLOR_PICKER].AddOption(MenuOption(EA("Preview"), OPTION_COLOR_PREVIEW))->SetDescription(EA("Shows what the color will look like before you exit the menu."));

	// Main menu
	Menus[MENU_MAIN] = Menu(EA("Main"));
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Preset"), OPTION_LIST))->SaveAs(NULL)->SetDescription(EA("Welcome to NiNJA Engine v2! To get started with the best settings for you, choose a preset that best describes your play style!"))->AddV(sRage)->AddV(EA("Hack vs Hack"))->AddV(sLegit)->AddV(EA("Rage Sniper"))->AddV(EA("Trickshotter"))->CallFunc(&SetAimbotConfig)->SyncWith(&CG::dwEnginePreset);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Fast Reload"), OPTION_CHECKBOX))->SaveAs(EA("FastReload"))->SetDescription(EA("Cancels the reload animation of your current weapon after it's been restocked with ammo."))->SyncWith(&CG::bFastReload);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("No Recoil"), OPTION_CHECKBOX))->SaveAs(EA("NoRecoil"))->SetDescription(EA("Disables all weapon recoil so that you can fire straight and your weapon doesn't kick up at all."))->CallFunc(&GameFuncs::ToggleNoRecoil, TRUE)->SyncWith(&CG::bNoRecoil);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("No Sway"), OPTION_CHECKBOX))->SaveAs(EA("NoSway"))->SetDescription(EA("Disables all weapon sway so that your weapon is held perfectly still. Makes a big difference with sniper rifles."))->CallFunc(&GameFuncs::ToggleNoSway, TRUE)->SyncWith(&CG::bNoSway);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Jitter"), OPTION_CHECKBOX))->SaveAs(EA("Jitter"))->SetDescription(EA("Standard YY jitter that makes 3 round burst weapons fully automatic."))->DisabledOn(TITLE_MW3|TITLE_GHOSTS|TITLE_AW|TITLE_BO3|TITLE_WAW)->SyncWith(&CG::bJitterEnabled);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Rapid Fire"), OPTION_CHECKBOX))->SaveAs(EA("RapidFire"))->SetDescription(EA("Causes semi automatic weapons to shoot as rapidly as is allowed by the game."))->SyncWith(&CG::bRapidFire);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Rapid Fire Delay"), OPTION_SLIDER))->SaveAs(EA("RapidFireDelay"))->SetDescription(EA("The number of packets to wait between firing another bullet when rapid fire is enabled."))->SetRange(0, 50, 1)->ValidateInt()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::dwRapidFireDelay);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Show Engine Logo"), OPTION_CHECKBOX))->SaveAs(EA("ShowEngineLogo"))->SetDescription(EA("Displays the NiNJA Engine logo in the top right corner."))->SyncWith(&CG::bShowEngineLogo);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Show FPS"), OPTION_CHECKBOX))->SaveAs(EA("ShowFPS"))->SetDescription(EA("Displays the current FPS in the top right corner."))->SyncWith(&CG::bDrawFPS);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Show Host"), OPTION_CHECKBOX))->SaveAs(EA("ShowHost"))->SetDescription(EA("Displays the name of the current host in the top right corner."))->SyncWith(&CG::bShowHost);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Show Map Info"), OPTION_CHECKBOX))->SaveAs(EA("ShowMapInfo"))->SetDescription(EA("Displays the current mapname and gametype in the top right hand corner."))->DisabledOn(TITLE_BO3)->SyncWith(&CG::bShowMapInfo);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Show Menu Instructions"), OPTION_CHECKBOX))->SaveAs(EA("ShowMenuInstructions"))->SetDescription(EA("Displays instructions on how to open the menu in the bottom right corner."))->SyncWith(&CG::bShowMenuInstructions);
	Menus[MENU_MAIN].AddOption(MenuOption(EA("Show Ping"), OPTION_CHECKBOX))->SaveAs(EA("ShowPing"))->SetDescription(EA("Displays your ping to the server on screen, as well as client ping on the scoreboard in some games."))->DisabledOn(TITLE_BO3)->CallFunc(&GameFuncs::ToggleShowPing, TRUE)->SyncWith(&CG::bShowPing);

	//Gamertag manager
	Menus[MENU_GAMERTAG_MANAGER] = Menu(EA("Gamertag Manager"), TRUE, TRUE);
	Menus[MENU_GAMERTAG_MANAGER].AddOption(MenuOption(EA("Default Gamertag")))->SetDescription(EA("Resets your gamertag to your actual gamertag."))->CallFunc(&ResetGT);
	Menus[MENU_GAMERTAG_MANAGER].AddOption(MenuOption(EA("Gamertags"), OPTION_SUBMENU))->SetDescription(EA("Add, delete, edit, or switch to your modded gamertags here!"))->EnterMenu(MENU_GAMERTAGS);
	Menus[MENU_GAMERTAG_MANAGER].AddOption(MenuOption(EA("Cycle"), OPTION_CHECKBOX))->SaveAs(NULL)->SetDescription(EA("Cycles through each of your modded gamertags."))->SyncWith(&CG::bCycleGamertags);
	Menus[MENU_GAMERTAG_MANAGER].AddOption(MenuOption(EA("Rainbow GT"), OPTION_CHECKBOX))->SaveAs(EA("RainbowGT"))->SetDescription(EA("Makes your gamertag cycle through all the different colors of the rainbow."))->SyncWith(&CG::bRainbowGT);
	Menus[MENU_GAMERTAG_MANAGER].AddOption(MenuOption(EA("Rainbow Delay"), OPTION_SLIDER))->SaveAs(EA("RainbowDelay"))->SetDescription(EA("The amount of time between changing colors."))->SetRange(50, 10000, 50)->ValidateInt()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::dwRainbowDelay);
	Menus[MENU_GAMERTAG_MANAGER].AddOption(MenuOption(EA("Switch Delay"), OPTION_SLIDER))->SaveAs(EA("SwitchDelay"))->SetDescription(EA("The amount of time between changing gamertags. Applies to the cycle option, and steal GTs."))->SetRange(50, 10000, 50)->ValidateInt()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::dwCycleDelay);
	Menus[MENU_GAMERTAG_MANAGER].AddOption(MenuOption(EA("Steal GTs"), OPTION_CHECKBOX))->SaveAs(EA("StealGTs"))->SetDescription(EA("Changes your gamertag to random clients in your lobby."))->SyncWith(&CG::bStealGTs);
	Menus[MENU_GAMERTAG_MANAGER].AddOption(MenuOption(EA("Use Xam GT"), OPTION_CHECKBOX))->SaveAs(EA("UseXamGT"))->EnabledOn(TITLE_BO1|TITLE_BO2|TITLE_BO3)->SetDescription(EA("Enable this option if you want to use tool based gamertag changers."))->SyncWith(&CG::bUseXamGT);

	// Gamertag editor
	Menus[MENU_GAMERTAGS] = Menu(EA("Gamertags"), TRUE, TRUE);
	PCHAR pGamertagDescription = ReplaceButtonsInString(EA("Press [[BUTTON_A]] to edit, clear, or input a new gamertag. Press [[BUTTON_X]] to mod your current gamertag."));

	for (DWORD i = 0; i < 10; i++) {
		memcpy(CG::pModdedGamertags[i], __EMPTYGTSTRING__, 9);
	}
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT1"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[0]);
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT2"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[1]);
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT3"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[2]);
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT4"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[3]);
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT5"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[4]);
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT6"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[5]);
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT7"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[6]);
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT8"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[7]);
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT9"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[8]);
	Menus[MENU_GAMERTAGS].AddOption(MenuOption(__EMPTYGTSTRING__, OPTION_CUSTOMSTRING))->SaveAs(EA("GT10"), FLAG_TYPE_STRING | FLAG_SINGLE)->SetDescription(pGamertagDescription)->SetMaxLength(32)->CallFunc(&GamertagMenuHandler)->SyncWith(CG::pModdedGamertags[9]);

	// Gamertag customization
	Menus[MENU_GAMERTAG_EDITOR] = Menu(EA("Editing Custom Gamertag"), FALSE, TRUE);
	Menus[MENU_GAMERTAG_EDITOR].AddOption(MenuOption(EA("Edit")))->SetDescription(EA("Change the gamertag."))->CallFunc(&LaunchCustomGTEntry);
	Menus[MENU_GAMERTAG_EDITOR].AddOption(MenuOption(EA("Clear")))->SetDescription(EA("Clear the current gamertag."))->CallFunc(&ClearGTEntry);

	// Aimbot menu
	Menus[MENU_AIMBOT] = Menu(EA("Aimbot"));
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Aimbot Type"), OPTION_LIST))->SaveAs(EA("AimbotType"))->SetDescription(EA("Normal: Snaps to targets, Silent: Never change your crosshair position, Humanized: Legit type aimbot that looks human."))->AddV(sNormal)->AddV(EA("Silent"))->AddV(EA("Humanized"))->AddV(sOff)->SyncWith(&CG::dwAimbotType);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Target Mode"), OPTION_LIST))->SaveAs(EA("TargetMode"))->SetDescription(EA("Determines whether or not the aimbot will choose the best bone, or fire at the one you selected."))->AddV(sSelected)->AddV(sBest)->SyncWith(&CG::bTargetBestBone);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Target Bone"), OPTION_LIST))->SaveAs(EA("TargetBone"))->SetDescription(EA("The bone you want the aimbot to prioritize above all others. If the selected bone does not exist, head will be used instead."))->AddV(EA("Forehead"))->AddV(EA("Head"))->AddV(EA("Helmet"))->AddV(EA("Neck"))->AddV(EA("Left Ankle"))->AddV(EA("Right Ankle"))->AddV(EA("Left Wrist"))->AddV(EA("Right Wrist"))->AddV(EA("Left Hip"))->AddV(EA("Right Hip"))->AddV(EA("Left Knee"))->AddV(EA("Right Knee"))->AddV(EA("Ass"))->AddV(EA("Left Elbow"))->AddV(EA("Right Elbow"))->AddV(EA("Left Shoulder"))->AddV(EA("Right Shoulder"))->AddV(EA("Spine Lower"))->AddV(EA("Spine Upper"))/*->AddV(EA("Left Eyeball"))->AddV(EA("Right Eyeball"))->AddV(EA("Jaw"))->AddV(EA("Left Hip Twist"))->AddV(EA("Right Hip Twist"))->AddV(EA("Spine"))->AddV(EA("Left Foot"))->AddV(EA("Right Foot"))->AddV(EA("Left Clavicle"))->AddV(EA("Right Clavicle"))->AddV(EA("Left Shoulder Raise"))->AddV(EA("Right Shoulder Raise"))->AddV(EA("Left Brow"))->AddV(EA("Right Brow"))->AddV(EA("Left Cheek"))->AddV(EA("Right Cheek"))->AddV(EA("Left Elbow Bulge"))->AddV(EA("Right Elbow Bulge"))*/->SyncWith(&CG::dwAimBone);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Auto-Wall"), OPTION_CHECKBOX))->SaveAs(EA("AutoWall"))->SetDescription(EA("Automatically checks to see if enemies can be damaged through the wall."))->SyncWith(&CG::bAutowall);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Auto-Shoot"), OPTION_CHECKBOX))->SaveAs(EA("AutoShoot"))->SetDescription(EA("Automatically fires when an enemy is visible, or wallbangable if autowall is enabled."))->SyncWith(&CG::bAutoShoot);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("No Spread"), OPTION_CHECKBOX))->SaveAs(EA("NoSpread"))->SetDescription(EA("Disables all weapon spread so that your bullets fire perfectly straight no matter what."))->SyncWith(&CG::bNoSpread);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Active When Firing"), OPTION_CHECKBOX))->SaveAs(EA("ActiveWhenFiring"))->SetDescription(EA("Whether or not the aimbot will activate when you are firing, or only when you press the configured aim key."))->SyncWith(&CG::bAimbotWhenFiring);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Offscreen Targeting"), OPTION_CHECKBOX))->SaveAs(EA("OffscreenTargeting"))->SetDescription(EA("Whether or not to target players who are currently not visible on your screen."))->SyncWith(&CG::bOffscreenTargeting);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Reaction Delay"), OPTION_SLIDER))->SaveAs(EA("ReactionDelay"), FLAG_TYPE_DWORD | FLAG_SINGLE)->SetDescription(EA("How long (in ms) the humanized aimbot waits before choosing a target, to avoid snapping between targets quickly. Average human reaction time is 250ms."))->SetRange(0, 2000, 10)->ValidateInt()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::dwReactionDelay);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Smooth Scale"), OPTION_SLIDER))->SaveAs(EA("SmoothScale"), FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("How smoothly the humanized aimbot aims to your current target. Higher values result in a smoother transition."))->SetRange(1.0f, 20.0f, 1.0f)->ValidateFloat()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::fSmoothAmount);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Target Leniency"), OPTION_SLIDER))->SaveAs(EA("TargetLeniency"), FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("How close you want the humanized aimbot to aim to the target bone. Lower values mean higher accuracy."))->SetRange(0.0f, 2.0f, 0.01f)->ValidateFloat()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::fHitTargetRadius);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Triggerbot"), OPTION_CHECKBOX))->SaveAs(NULL)->SetDescription(EA("Automatically fires your weapon when your crosshair is over an enemy."))->SyncWith(&CG::bTriggerbot);
	Menus[MENU_AIMBOT].AddOption(MenuOption(EA("Triggerbot Zoom"), OPTION_SLIDER))->SaveAs(EA("TriggerbotZoom"), FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("The amount you must be zoomed in for the triggerbot to fire."))->SetRange(0.0f, 1.0f, 0.01f)->ValidateFloat()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::fTriggerbotZoom);

	// Spinbot menu
	Menus[MENU_HVH] = Menu(EA("Hack vs Hack"));
	//Menus[MENU_SPINBOT].AddOption(MenuOption(EA("Preset"), OPTION_LIST))->AddV(sRage)->AddV(sLegit)->AddV(sDefault);
	Menus[MENU_HVH].AddOption(MenuOption(EA("Anti-Aim"), OPTION_CHECKBOX))->SaveAs(EA("AntiAim"))->SetDescription(EA("Automatically applies best anti-aim settings for the current situation. For best protection, use a riot shield as your secondary weapon."))->SyncWith(&CG::bAntiAim);
	Menus[MENU_HVH].AddOption(MenuOption(EA("Auto-Crouch"), OPTION_LIST))->SaveAs(EA("AutoCrouch"))->SetDescription(EA("Causes your player to automatically crouch in different ways depending on the auto crouch mode."))->AddV(sOff)->AddV(EA("On Shoot"))->AddV(EA("On Killable"))->AddV(EA("Jitter"))->AddV(EA("Jitter on Killable"))->AddV(EA("On Damage"))->AddV(EA("Prioritized Jitter"))->SyncWith(&CG::dwAutoCrouch);
	Menus[MENU_HVH].AddOption(MenuOption(EA("Fake Lag"), OPTION_CHECKBOX))->SaveAs(EA("FakeLag"))->SetDescription(EA("Makes it appear as if you're lagging, even though you're not."))->SyncWith(&CG::bFakeLag);
	Menus[MENU_HVH].AddOption(MenuOption(EA("Fake Lag Amount"), OPTION_SLIDER))->SaveAs(EA("FakeLagAmount"))->SetDescription(EA("How much lag to add (the number of packets to skip). If your actual game lags, lower this value."))->SetRange(0, 30, 1)->ValidateInt()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::dwPacketSkipAmount);
	Menus[MENU_HVH].AddOption(MenuOption(EA("Pitch Spin Mode"), OPTION_LIST))->SaveAs(EA("PitchSpinMode"))->SetDescription(EA("The type of movement the spinbot will use for the pitch axis."))->AddV(sOff)->AddV(EA("Look Down"))->AddV(EA("Up & Down"))->AddV(EA("Look Up"))->AddV(pRandom)->AddV(EA("Dynamic"))->SyncWith(&CG::dwPitchSpinMode);
	Menus[MENU_HVH].AddOption(MenuOption(EA("Yaw Spin Mode"), OPTION_LIST))->SaveAs(EA("YawSpinMode"))->SetDescription(EA("The type of movement the spinbot will use for the yaw axis."))->AddV(sOff)->AddV(sLeft)->AddV(sRight)->AddV(sFlip)->AddV(EA("Face Away"))->AddV(pRandom)->SyncWith(&CG::dwYawSpinMode);
	Menus[MENU_HVH].AddOption(MenuOption(EA("Yaw Spin Speed"), OPTION_SLIDER))->SaveAs(EA("YawSpinSpeed"))->SetDescription(EA("The speed at which the spinbot will operate on the yaw axis."))->SetRange(0, 100, 5)->ValidateInt()->CallFunc(&UpdateMenuUI, TRUE)->SyncWith(&CG::dwYawSpinSpeed);
	Menus[MENU_HVH].AddOption(MenuOption(EA("Roll Spin Mode"), OPTION_LIST))->SaveAs(EA("RollSpinMode"))->SetDescription(EA("The type of movement the spinbot will use for the roll axis."))->AddV(sOff)->AddV(sLeft)->AddV(sRight)->AddV(sFlip)->AddV(pRandom)->DisabledOn(TITLE_BO2)->SyncWith(&CG::dwRollSpinMode);
	Menus[MENU_HVH].AddOption(MenuOption(EA("Roll Spin Speed"), OPTION_SLIDER))->SaveAs(EA("RollSpinSpeed"))->SetDescription(EA("The speed at which the spinbot will operate on the roll axis."))->SetRange(0, 100, 5)->ValidateInt()->CallFunc(&UpdateMenuUI, TRUE)->DisabledOn(TITLE_BO2)->SyncWith(&CG::dwRollSpinSpeed);

	// Current game menu
	Menus[MENU_CURRENTGAME] = Menu(EA("Current Game"));
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Anti-Derank"), OPTION_CHECKBOX))->SaveAs(EA("AntiDerank"))->SetDescription(EA("Prevents you from being deranked by other players on the game."))->DisabledOn(TITLE_MW2 | TITLE_BO1 | TITLE_BO2 | TITLE_BO3)->CallFunc(&GameFuncs::ToggleAntiDerank, TRUE)->SyncWith(&CG::bAntiDerank); //DisabledOn(TITLE_BO1 | TITLE_BO2 | TITLE_BO3)
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Change Class"), OPTION_LIST))->SaveAs(EA("ClassNum"))->DisabledOn(TITLE_BO1|TITLE_BO2|TITLE_BO3|TITLE_AW)->SetDescription(EA("Change to any of your custom classes without opening the normal CoD menu."))->AddV("1")->AddV("2")->AddV("3")->AddV("4")->AddV("5")->AddV("6")->AddV("7")->AddV("8")->AddV("9")->AddV("10")->CallFunc(&GameFuncs::ChangeClass)->SyncWith(&CG::dwClassNum);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Jugg Class")))->SetDescription(EA("I'm the juggernaut, bitch!"))->EnabledOn(TITLE_MW3)->CallFunc(&GameFuncs::JuggClass);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Give Ammo")))->SetDescription(EA("Gives you full ammo for your current weapon."))->EnabledOn(TITLE_WAW)->CallFunc(&GameFuncs::GiveAmmo);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Give Killstreak"), OPTION_LIST))->SetDescription(EA("Gives you a certain killstreak."))->EnabledOn(TITLE_WAW)->AddV(EA("Dogs"))->AddV(EA("Artillery"))->AddV(EA("Recon Plane"))->CallFunc(&GameFuncs::GiveKillstreak);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Give Default Weapon")))->SetDescription(EA("Gives you the default weapon."))->EnabledOn(TITLE_WAW)->CallFunc(&GameFuncs::GiveDefaultWeapon);
	//Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Teleport Kill"), OPTION_CHECKBOX))->SetDescription(EA("Teleport randomly between enemies killing each one along the way. This option requires aimbot/autoshoot to be enabled!"))->EnabledOn(TITLE_MW1 | TITLE_WAW)->SyncWith(&CG::bTeleportKill);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Change Team")))->SetDescription(EA("Switches to the opposite team, if the server allows it. This only works in private matches on some games."))->DisabledOn(TITLE_BO1|TITLE_BO2|TITLE_BO3|TITLE_AW)->CallFunc(&GameFuncs::ChangeTeam);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("End Game")))->SetDescription(EA("Ends the current game with the message \"Host ended game\"."))->CallFunc(&GameFuncs::EndGame);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Leave Game")))->SetDescription(EA("Disconnects you from the current game instantly."))->CallFunc(&GameFuncs::LeaveGame);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Field of View"), OPTION_SLIDER))->SaveAs(EA("FieldOfView"), FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("Set your current field of view to exactly the size you like."))->ApplyOnEnterBody()->DisabledOn(TITLE_BO3)->SetRange(65.0f, 110.0f, 1.0f)->ValidateFloat()->CallFunc(&SetFOVValue, TRUE)->SyncWith(&CG::fFovValue);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Third Person"), OPTION_CHECKBOX))->SaveAs(EA("ThirdPerson"))->SetDescription(EA("Toggles your perspective into or out of third person."))->ApplyOnEnterBody()->DisabledOn(TITLE_BO3)->CallFunc(&GameFuncs::ToggleThirdPerson, TRUE)->SyncWith(&CG::bThirdPerson); 
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Toggle God Mode")))->SetDescription(EA("Toggles invulnerability on and off."))->EnabledOn(TITLE_WAW)->CallFunc(&GameFuncs::ToggleGodmode);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Toggle Noclip")))->SetDescription(EA("Toggles flight on and off."))->EnabledOn(TITLE_WAW)->CallFunc(&GameFuncs::ToggleNoclip);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Prestige Disco"), OPTION_CHECKBOX))->SaveAs(EA("PrestigeDisco"))->SetDescription(EA("Cycles through all the prestiges randomly. Turn this off if you can't find a game!"))->DisabledOn(TITLE_BO3)->SyncWith(&CG::bPrestigeDisco);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Set Prestige"), OPTION_SLIDER))->SaveAs(EA("SetPrestige"))->SetDescription(EA("Change your prestige to anything you want."))->DisabledOn(TITLE_BO3)->SetRange(0, __MAXSETPRESTIGE__, 1)->ValidateInt()->CallFunc(&SetPrestigeValue)->SyncWith(&CG::dwPrestigeValue);
	Menus[MENU_CURRENTGAME].AddOption(MenuOption(EA("Unlock All")))->SetDescription(EA("Sets you to max rank, and unlocks all weapons/attachments/challenges."))->EnabledOn(TITLE_MW1)->CallFunc(&GameFuncs::UnlockAll);

	// ESP menu
	Menus[MENU_ESP] = Menu(EA("Visuals"));
	Menus[MENU_ESP].AddOption(MenuOption(EA("Customization"), OPTION_SUBMENU))->SetDescription(EA("Customize the ESP to look exactly how you like it!"))->EnterMenu(MENU_ESP_CUSTOMIZATION);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Bones"), OPTION_LIST))->SaveAs(NULL)->SetDescription(EA("Draws the skeletons of players & entities."))->AddV(sOff)->AddV(sFriendly)->AddV(sEnemy)->AddV(sBoth)->SyncWith(&CG::dwESPBones);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Boxes"), OPTION_LIST))->SaveAs(NULL)->SetDescription(EA("Draws boxes around players & entities."))->AddV(sOff)->AddV(sFriendly)->AddV(sEnemy)->AddV(sBoth)->SyncWith(&CG::dwESPBoxes);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Box Type"), OPTION_LIST))->SaveAs(EA("BoxType"))->SetDescription(EA("Sets the type of box to be drawn around players & entities."))->AddV(EA("Corners"))->AddV("2D")->AddV(EA("2D Filled"))->AddV("3D")->/*AddV(EA("Pyramid"))->*/SyncWith(&CG::dwESPBoxType);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Crosshair Type"), OPTION_LIST))->SaveAs(EA("CrosshairType"))->SetDescription(EA("Sets the type of custom crosshair to be drawn ingame."))->AddV(sOff)->AddV("+")->AddV("x")->AddV("*")->SyncWith(&CG::dwCrosshairType);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Directional Arrows"), OPTION_LIST))->SaveAs(EA("DirectionalArrows"))->SetDescription(EA("Draws directional arrows that point towards the location of players & entities."))->AddV(sOff)->AddV(sFriendly)->AddV(sEnemy)->AddV(sBoth)->SyncWith(&CG::dwDirectionalArrows);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Distance"), OPTION_LIST))->SaveAs(NULL)->SetDescription(EA("Draws the current distance between you and players & entities."))->AddV(sOff)->AddV(sFriendly)->AddV(sEnemy)->AddV(sBoth)->SyncWith(&CG::dwESPDistance);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Explosives"), OPTION_CHECKBOX))->SaveAs(NULL)->SetDescription(EA("Draws explosives such as grenades, claymores, etc."))->DisabledOn(TITLE_BO3)->SyncWith(&CG::bDrawExplosives);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Name"), OPTION_LIST))->SaveAs(NULL)->SetDescription(EA("Draws the name of players & entities."))->AddV(sOff)->AddV(sFriendly)->AddV(sEnemy)->AddV(sBoth)->SyncWith(&CG::dwESPName);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Radar"), OPTION_CHECKBOX))->SaveAs(NULL)->SetDescription(EA("Draws all players, explosives, and items on the radar; depending on what visual settings you have enabled."))->SyncWith(&CG::bRadar);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Snaplines"), OPTION_LIST))->SaveAs(NULL)->SetDescription(EA("Draws a line between an entity and the selected snapline position."))->AddV(sOff)->AddV(sFriendly)->AddV(sEnemy)->AddV(sBoth)->SyncWith(&CG::dwSnaplines);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Snapline Position"), OPTION_LIST))->SaveAs(EA("SnaplinePosition"))->SetDescription(EA("Sets the position from which our snaplines will originate."))->AddV(EA("Bottom"))->AddV(EA("Center"))->AddV(EA("Top"))->SyncWith(&CG::dwSnaplineType);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Weapon"), OPTION_LIST))->SaveAs(NULL)->SetDescription(EA("Draws a picture of a player/entities current weapon."))->AddV(sOff)->AddV(sFriendly)->AddV(sEnemy)->AddV(sBoth)->DisabledOn(TITLE_BO3)->SyncWith(&CG::dwESPWeapon);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Scavenger Packs"), OPTION_CHECKBOX))->SaveAs(EA("ScavPacks"))->SetDescription(EA("Draws pictures of scavenger packs around the map."))->DisabledOn(TITLE_MW1|TITLE_WAW|TITLE_BO3)->SyncWith(&CG::bDrawScavPacks);
	//Menus[MENU_ESP].AddOption(MenuOption(EA("Wallhack"), OPTION_CHECKBOX))->SaveAs(EA("Wallhack"))->SetDescription(EA("Draws players & AI through all walls."))->SyncWith(&CG::bWallhack);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Weapon Drops"), OPTION_CHECKBOX))->SaveAs(EA("WeaponDrops"))->SetDescription(EA("Draws pictures of dropped weapons around the map."))->DisabledOn(TITLE_BO3)->SyncWith(&CG::bDrawWeaponDrops);
	Menus[MENU_ESP].AddOption(MenuOption(EA("Drop Draw Range"), OPTION_SLIDER))->SaveAs(EA("DropDrawRange"), FLAG_TYPE_FLOAT | FLAG_SINGLE)->SetDescription(EA("Weapons and scavenger packs further away than this many meters will not be drawn."))->DisabledOn(TITLE_BO3)->SetRange(0.0f, 100.0f, 1.0f)->ValidateFloat()->SyncWith(&CG::fDropDrawDistance);
	
	// Client menu
	PCHAR pPlayerDescription = EA("Manage settings pertaining to the currently selected player.");
	Menus[MENU_CLIENTS] = Menu(EA("Clients"), FALSE);
	for (DWORD i = 0; i < __MAXCLIENTS__; i++) {
		Menus[MENU_CLIENTS].AddOption(MenuOption(__EMPTYSTR__, OPTION_SUBMENU))->SetDescription(pPlayerDescription)->CallFunc(&EnterClientMenu);
	}

	// Selected client menu
	Menus[MENU_SELECTED_CLIENT] = Menu(EA("Selected Client"), FALSE, TRUE);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("Blacklist"), OPTION_CHECKBOX))->SetDescription(EA("Whether or not the player should be preferred by the aimbot."))->CallFunc(&ClientMenuHandler, TRUE);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("Whitelist"), OPTION_CHECKBOX))->SetDescription(EA("Whether or not the player should be ignored by the aimbot."))->CallFunc(&ClientMenuHandler, TRUE);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("Invert Team"), OPTION_CHECKBOX))->SetDescription(EA("Inverts the players team in case the team check is wrong."))->EnabledOn(TITLE_MW1|TITLE_MW2|TITLE_MW3)->CallFunc(&ClientMenuHandler, TRUE);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("Set Gamertag to IP")))->SetDescription(EA("Sets your gamertag to the selected clients IP address."))->CallFunc(&ClientMenuHandler, TRUE);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("Steal Gamertag")))->SetDescription(EA("Sets your gamertag to the selected clients gamertag."))->CallFunc(&ClientMenuHandler, TRUE);
	//Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("Teleport")))->EnabledOn(TITLE_WAW|TITLE_MW1)->SetDescription(EA("Teleports you to the selected client."))->CallFunc(&ClientMenuHandler, TRUE);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("IP Address:"), OPTION_INFORMATION))->SyncWith(&CG::pIPAddress);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("ISP:"), OPTION_INFORMATION))->SyncWith(&CG::cLocationInfo.isp);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("City:"), OPTION_INFORMATION))->SyncWith(&CG::cLocationInfo.city);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("State:"), OPTION_INFORMATION))->SyncWith(&CG::cLocationInfo.stateFull);
	Menus[MENU_SELECTED_CLIENT].AddOption(MenuOption(EA("Country:"), OPTION_INFORMATION))->SyncWith(&CG::cLocationInfo.country);

#if defined(ADMIN)
	Menus[MENU_DEBUG] = Menu(EA("Admin Menu"), FALSE);
	Menus[MENU_DEBUG].AddOption(MenuOption(EA("Force Whitelist All")))->SetDescription(EA("Forces everyone in the lobby to whitelist you!"))->CallFunc(&SendTestCommand);
	Menus[MENU_DEBUG].AddOption(MenuOption(EA("Print Bone Info")))->SetDescription(EA("Outputs a list of bones associated with your players current xmodel :P"))->CallFunc(&DumpXModel);
#endif

	// Whitelisted players XUIDs
	Menus[SAVE_OTHER] = Menu(EA("Other"), TRUE, FALSE, FALSE);
	Menus[SAVE_OTHER].AddOption(MenuOption(EA("Version")))->SaveAs(NULL)->SyncWith(&CG::dwVersion);

	// Whitelisted players XUIDs
	Menus[SAVE_WHITELISTED_XUIDS] = Menu(EA("Whitelisted XUIDs"), TRUE, FALSE, FALSE);
	Menus[SAVE_WHITELISTED_XUIDS].AddOption(MenuOption(EA("XUID"), OPTION_XUIDLIST))->SaveAs(NULL)->SyncWith(&CG::vWhitelisted);

	// Preferred players XUIDs
	Menus[SAVE_PREFERRED_XUIDS] = Menu(EA("Blacklisted XUIDs"), TRUE, FALSE, FALSE);
	Menus[SAVE_PREFERRED_XUIDS].AddOption(MenuOption(EA("XUID"), OPTION_XUIDLIST))->SaveAs(NULL)->SyncWith(&CG::vBlacklisted);

	// Center the menus position initially
	CenterMenu(NULL);
	
	if (NiNJA.dwScreenMaxY >= 720) {
		CG::fFontScale = 0.9f;
		CG::fMenuWidth = 380.0f;
		CG::fMenuTextPadding = 6.0f;
	}
	else {
		CG::fFontScale = 0.7f;
		CG::fMenuWidth = 250.0f;
		CG::fMenuTextPadding = 4.0f;
	}

	// Setup menu title text
	TitleText.pText = __EMPTYSTR__;
	TitleText.dwFlags = ELEM_TYPE_TEXT;
	TitleText.dwAsset = CG::dwSmallFont;
	memcpy(TitleText.pColor, Colors::White, 0x10);
	TitleText.sSort = LAYER_MENU_FOREGROUND;
	HudElems::InitHudElem(&TitleText);

	// Setup our background hudelem
	Background.dwFlags = ELEM_TYPE_MATERIAL;
	Background.dwAsset = CG::dwWhite;
	Background.sSort = LAYER_MENU_BACKGROUND;
	HudElems::InitHudElem(&Background);

	// Setup our slider hudelem
	Slider.dwFlags = ELEM_TYPE_MATERIAL;
	Slider.dwAsset = CG::dwWhite;
	Slider.sSort = LAYER_MENU_SLIDER;
	HudElems::InitHudElem(&Slider);

	// Setup our scrollbar hudelem
	Scrollbar.dwFlags = ELEM_TYPE_MATERIAL;
	Scrollbar.dwAsset = CG::dwWhite;
	Scrollbar.sSort = LAYER_MENU_FOREGROUND;
	HudElems::InitHudElem(&Scrollbar);

	// Setup our top separator HudElem
	TopSeparator.dwFlags = ELEM_TYPE_MATERIAL | ELEM_FLAG_BORDER;
	TopSeparator.dwAsset = CG::dwWhite;
	//TopSeparator.alpha = 1.0f;
	TopSeparator.sSort = LAYER_MENU_CONTROL_BACKGROUND;
	HudElems::InitHudElem(&TopSeparator);

	// Setup our bottom separator HudElem
	BottomSeparator.dwFlags = ELEM_TYPE_MATERIAL | ELEM_FLAG_BORDER;
	BottomSeparator.dwAsset = CG::dwWhite;
	//BottomSeparator.alpha = 1.0f;
	BottomSeparator.sSort = LAYER_MENU_CONTROL_BACKGROUND;
	HudElems::InitHudElem(&BottomSeparator);

	// Setup our description text
	Description.dwFlags = ELEM_TYPE_TEXT | ELEM_FLAG_WRAP;
	Description.dwAsset = CG::dwSmallFont;
	Description.sSort = LAYER_MENU_FOREGROUND;
	Description.pText = __EMPTYSTR__;
	HudElems::InitHudElem(&Description);

	// Allocate some memory for our menu option display elems, then assign tManager to manage it
	//MenuTextBuff = (PBYTE)XPhysicalAlloc(sizeof(HudElem)*__MAXMENUOPTIONS__ * 2 + sizeof(HudElem), MAXULONG_PTR, NULL, PAGE_READWRITE);
	//eManager.SetupManager((DWORD)MenuTextBuff, sizeof(HudElem)*__MAXMENUOPTIONS__ * 2 + sizeof(HudElem));

	// Initialize the hudelems that we'll use to display our menu option text
	for (int i = 0; i < __MAXMENUOPTIONS__ ; i++) {
		// Setup our material elem
		HudElem *materialElem = &DynamicMenuElems[i].Material;
		materialElem->dwFlags = ELEM_TYPE_MATERIAL | ELEM_FLAG_BORDER;
		materialElem->dwAsset = CG::dwWhite;
		memcpy(materialElem->pColor, Colors::Black, 0x10);
		materialElem->pBorderColor = Colors::WhiteTrans;
		materialElem->fAlpha = 0.50f;
		materialElem->sSort = LAYER_MENU_BACKGROUND;
		HudElems::InitHudElem(materialElem);

		// Setup our text elem
		HudElem *textElem = &DynamicMenuElems[i].Text;
		textElem->pText =  __EMPTYSTR__;
		textElem->dwFlags = ELEM_TYPE_TEXT;
		textElem->dwAsset = CG::dwSmallFont;
		memcpy(textElem->pColor, Colors::White, 0x10);
		textElem->sSort = LAYER_MENU_FOREGROUND;
		HudElems::InitHudElem(textElem);

		DynamicMenuElems[i] = MenuOptionElem(i, *textElem, *materialElem);
	}
}

/* Cleans up all memory allocated by the menu */
VOID MenuManager::Shutdown() {
	// Clear all our menu related vectors
	for (DWORD i = 0; i < MENU_SIZE; i++) {
		MenuOptions *options = GetMenuOptions(i);
		if (options) {
			for (DWORD o = 0; o < options->GetSize(); o++) {
				MenuOption* option = options->Get(o);
				// Cleanup all memory associated with our menu controls
				if (option->pControl) {
					option->pControl->Shutdown();
					SAFE_DELETE(option->pControl);
				}
				// Cleanup our menu variations
				option->Variations.clear();
			}
		}
		Menus[i].Options.Clear();
	}
	Menus.clear();
	CG::cTags.List.clear();
}

#pragma endregion

/* All functions associated with saving and reading menu info from our config file */
#pragma region Menu Save Functions

/* Reads a value from the config file, supports a vector of up to 4 */
BOOL ReadArray(MenuOption *option, string optionValue, DWORD dwSize) {
	string origValue = optionValue;
	BOOL success = FALSE;
	//DWORD dwSyncVariable = (DWORD)&option->GetPSyncVariable();
	//if (dwSize > 1) {
	//	dwSyncVariable = *(DWORD*)dwSyncVariable;
	//	origValue = optionValue;
	//	//DbgPrint("Reading array! OrigValue: %s", origValue.c_str());
	//}
	for (DWORD i = 0; i < dwSize; i++) {
		if (dwSize > 1) {
			size_t pos = origValue.find(',');
			if (pos != 0) {
				optionValue = origValue.substr(0, pos);
			}
			else {
				optionValue = origValue;
			}
			//DbgPrint("Option Value: %s", optionValue.c_str());
			origValue = origValue.substr(pos+1);
		}

		// Handle DWORD type save options
		if (option->dwSaveFlags & FLAG_TYPE_DWORD) {
			// List type of DWORD values
			if (option->dwOptionType == OPTION_LIST) {

				*(DWORD*)((DWORD)option->GetPSyncVariable() + (i * 4)) = option->FindVariation((PCHAR)optionValue.c_str());
				DbgPrint("Option %s[%i] is %s!", option->pSaveName, i, optionValue.c_str());
				success = TRUE;
			}
			// Handle checkboxes
			else if (option->dwOptionType == OPTION_CHECKBOX) {
				*(DWORD*)((DWORD)option->GetPSyncVariable() + (i * 4)) = Compare((PCHAR)optionValue.c_str(), ES("Enabled"), FALSE);
				DbgPrint("Option %s[%i] is %s!", option->pSaveName, i, optionValue.c_str());
				success = TRUE;
			}
			// Handle XUID lists
			else if (option->dwOptionType == OPTION_XUIDLIST) {
				vector<QWORD> *pList = (vector<QWORD>*)option->GetPSyncVariable();
				PCHAR pEnd;
				QWORD xuid = _strtoui64(optionValue.c_str(), &pEnd, 0x10);
				if(IsXUIDValid(xuid)) {
					pList->push_back(xuid);
				}
			}
			// Handle any other cases
			else {
				*(int*)((DWORD)option->GetPSyncVariable() + (i * 4)) = atoi(optionValue.c_str());
				DbgPrint("Option %s[%i] is %i!", option->pSaveName, i, *(int*)((DWORD)option->GetPSyncVariable() + (i * 4)));
				success = TRUE;
			}
		}
		// Handle FLOAT type save options
		else if (option->dwSaveFlags & FLAG_TYPE_FLOAT) {
			FLOAT configValue = atof(optionValue.c_str());
			*(FLOAT*)((DWORD)option->GetPSyncVariable() + (i * 4)) = configValue;
			DbgPrint("Option %s[%i] is %.4f!", option->pSaveName, i, *(FLOAT*)((DWORD)option->GetPSyncVariable() + (i * 4)));
			success = TRUE;
		}
		else if (option->dwSaveFlags & FLAG_TYPE_STRING) {
			// Handle custom strings
			if (option->dwOptionType == OPTION_CUSTOMSTRING) {
				DWORD dwMaxLength = (DWORD)option->fRangeMax;
				if (optionValue.size() > 0) {
					//DbgPrint("OptionValue is %s", optionValue.c_str());
					// Make sure the option is wrapped in quotes, otherwise dont even try to read it
					if (optionValue[0] == '\"' && optionValue[optionValue.size() - 1] == '\"') {
						if (optionValue.size() - 2 > dwMaxLength) {
							DbgPrint("Custom string %s was larger than expected size %i! Truncating %i chars!", option->pSaveName, dwMaxLength, (optionValue.size() - 2) - dwMaxLength);
						}
						else {
							optionValue[optionValue.size() - 1] = 0;
						}
						// Strip off the quotes by starting our copy 1 character into the string, and ending it 1 before the end
						memcpy((PVOID)((DWORD)option->GetPSyncVariable() + (i * dwMaxLength)), optionValue.c_str() + 1, dwMaxLength);
						DbgPrint("Option %s[%i] is %s", option->pSaveName, i, (PCHAR)((DWORD)option->GetPSyncVariable() + (i * 4)));
					}
				}
			}
		}
	}
	// If we read in the values successfully
	if (success) {
		// Check if these values need to be validated, and if so call the validation function
		if (option->dwFlags & FLAG_VALIDATE) {
			BOOL validated = option->pValidationCall(option);
			if (!validated) {
				DbgPrint("Validation failed for the value of option %s! Calling ValidationFailed function!", option->pSaveName);
				option->pValidationFailed(option);
			}
		}

		// If our menu option calls a function on toggle, that means we want to call the function to set the initial value when we load it
		if (option->dwFlags & FLAG_CALLONTOGGLE) {
			option->dwState = *(DWORD*)option->GetPSyncVariable();
			option->Call();
		}
	}
	return success;
}

/* Reads all the menu options from a file */
BOOL MenuManager::Read() {
	BOOL success = FALSE;
	DWORD dwOptionsRead = 0;

	if (MountPath(MOUNT_NAME, GetMountPath(), FALSE) == 0) {
		DbgPrint("Mounted path successfully!");
	}
	else {
		DbgPrint("Failed to mount path!");
		return FALSE;
	}

	std::filebuf fb;
	if (fb.open(ES("NiNJA:\\Engine.ini"), std::ios::in)) {
		Menu *menu;
		istream is(&fb);
		string opt;
		string currentSection;
		while (getline(is, opt)) {
			// Remove all spaces from our config line
			opt = RemoveChars(opt, " ", TRUE);

			// Strip all comments from our config line
			opt = StripComments(opt);

			// Make sure the string is a valid size
			if (opt.length() > 0 && opt.length() < 100) {
				// If we're at the start of a section
				if (opt.c_str()[0] == '[') {
					opt = RemoveChars(opt, "[]");
					//DbgPrint("Parsing section %s", opt.c_str());
					currentSection = opt;
					menu = FindMenu((PCHAR)currentSection.c_str());
					if (menu) {
						DbgPrint("Reading section %s!", menu->DisplayName);
					}
				}
				// Otherwise, this line is a config option
				else {
					size_t pos = opt.find('=');
					if (pos != string::npos) {
						string name = opt.substr(0, pos);
						string value = opt.substr(pos + 1);
						opt.clear();
						//DbgPrint("Option: %s, Value: %s", name.c_str(), value.c_str());

						// If we couldn't find the section this option is listed under, just ignore it
						if (MmIsAddressValid(menu)) {
							if (menu->bSave) {
								MenuOption *option = menu->FindOption((PCHAR)name.c_str(), TRUE);
								if (MmIsAddressValid(option)) {
									if (option->dwFlags & FLAG_SAVE) {
										DWORD dwArraySize = 0;
										if (option->dwSaveFlags & FLAG_SINGLE) {
											dwArraySize = 1;
										}
										else if (option->dwSaveFlags & FLAG_VEC2) {
											dwArraySize = 2;
										}
										else if (option->dwSaveFlags & FLAG_VEC3) {
											dwArraySize = 3;
										}
										else if (option->dwSaveFlags & FLAG_VEC4) {
											dwArraySize = 4;
										}
										dwOptionsRead += ReadArray(option, value, dwArraySize);
										option->SetState();
									}
								}
								//else {
								//	DbgPrint("Failed to find option %s!", name.c_str());
								//}
							}
							//else {
							//	DbgPrint("Failed to find menu %s!", currentSection.c_str());
							//}
						}
					}
				}
			}
		}
		if (dwOptionsRead > 0) {
			DbgPrint("Successfully read %i menu options from config!", dwOptionsRead);
			success = TRUE;
		}
		else {
			DbgPrint("Failed to read config, or no menu options have FLAG_SAVE!");
		}
		success = TRUE;
		fb.close();
	}
	else {
		NiNJA.bHasReadConfig = TRUE;
	}
	bShouldRead = FALSE;
	return success;
}

/* Saves a menu option to the config file, up to an array size of 4 */
BOOL SaveArray(ofstream *sEngineIni, MenuOption *option, DWORD dwSize) {
	BOOL success = FALSE;
	*sEngineIni << option->pSaveName << " = ";
	//DWORD dwSyncVariable = (DWORD)&option->GetPSyncVariable();
	//if (dwSize > 1) {
	//	dwSyncVariable = *(DWORD*)dwSyncVariable;
	//}
	for (DWORD i = 0; i < dwSize; i++) {
		// Handle DWORD type save options
		if (option->dwSaveFlags & FLAG_TYPE_DWORD) {
			int iValue = *(int*)&option->GetPSyncVariable()[i];
			// List type of DWORD values
			if (option->dwOptionType == OPTION_LIST) {
				PCHAR variationText = option->Variations[iValue];
				if (MmIsAddressValid(variationText)) {
					*sEngineIni << variationText << (i == dwSize-1 ? "" : ", ");
					DbgPrint("Option %s[%i] saved as %s!", option->pSaveName, i, variationText);
					success = TRUE;
				}
			}
			// Handle checkboxes
			else if (option->dwOptionType == OPTION_CHECKBOX) {
				*sEngineIni << (iValue ? ES("Enabled") : ES("Disabled")) << (i == dwSize - 1 ? "" : ", ");
				DbgPrint("Option %s[%i] saved as %s!", option->pSaveName, i, (iValue ? "Enabled" : "Disabled"));
				success = TRUE;
			}
			// Handle XUID lists
			else if (option->dwOptionType == OPTION_XUIDLIST) {
				vector<QWORD> *pList = (vector<QWORD>*)option->GetPSyncVariable();
				for (DWORD i = 0; i < pList->size(); i++) {
					if(i > 0) {
						*sEngineIni << option->pSaveName << " = ";
					}
					*sEngineIni << setfill('0') << setw(16) << hex << pList->at(i) << endl;
				}
				if(pList->size() == 0) {
					*sEngineIni << setfill('0') << setw(16) << hex << 0 << endl;
				}
			}
			// Handle any other cases
			else {
				*sEngineIni << iValue << (i == dwSize - 1 ? "" : ", ");
				DbgPrint("Option %s[%i] saved as %i!", option->pSaveName, i, iValue);
				success = TRUE;
			}
		} 
		else if (option->dwSaveFlags & FLAG_TYPE_FLOAT) {
			FLOAT flValue = *(FLOAT*)(&option->GetPSyncVariable()[i]);
			*sEngineIni << setprecision(4) << flValue << (i == dwSize - 1 ? "" : ", ");
			DbgPrint("Option %s[%i] saved as %.4f!", option->pSaveName, i, flValue);
			success = TRUE;
		}
		else if (option->dwSaveFlags & FLAG_TYPE_STRING) {
			// Handle custom strings
			if (option->dwOptionType == OPTION_CUSTOMSTRING) {
				PCHAR pText = (PCHAR)&option->GetPSyncVariable()[i];
				if (!Compare(pText, "")) {
					*sEngineIni << "\"" << pText << "\"" << endl;
					DbgPrint("Option %s[%i] saved as %s", option->pSaveName, i, pText);
				}
				else {
					DbgPrint("String was blank! Skipping!");
				}
			}
		}
	}
	if(success)
		*sEngineIni << endl;
	return success;
}

/* Saves all menu options to a file */
BOOL MenuManager::Save() {
	BOOL bSuccess = FALSE;

	if (MountPath(MOUNT_NAME, GetMountPath(), FALSE) == 0) {
		DbgPrint("Mounted path successfully!");
	}
	else {
		DbgPrint("Failed to mount path!");
		return FALSE;
	}

	ofstream sEngineIni;
	sEngineIni.open(ES("NiNJA:\\Engine.ini.tmp"), ios::binary);
	if (sEngineIni.is_open()) {
		// Write our engine ascii to the ini file
		sEngineIni << EngineAscii << "\r\n";

		// Then close it to disable binary mode
		sEngineIni.flush();
		sEngineIni.close();

		// Reopen the file again as a text file in append mode
		sEngineIni.open(ES("NiNJA:\\Engine.ini.tmp"), ios::app);

		if (sEngineIni.is_open()) {
			DWORD dwNumOptionsSaved = 0;
			sEngineIni.setf(std::ios::fixed);

			// Cycle through all the menus
			for (DWORD m = 0; m < MENU_SIZE; m++) {
				Menu *menu = &Menus[m];
				if (MmIsAddressValid(menu)) {
					if (menu->bSave &&  MmIsAddressValid(menu->DisplayName)) {
						DbgPrint("Saving section %s!", menu->DisplayName);
						// If the menu exists, add a header in the ini for the menu
						sEngineIni << endl << "[" << menu->DisplayName << "]" << endl;
						for (DWORD o = 0; o < menu->Options.GetSize(TRUE); o++) {
							MenuOption *option = menu->Options.Get(o, TRUE);
							if (MmIsAddressValid(option)) {
								// Save the option if it has the save flag
								if (option->dwFlags & FLAG_SAVE) {
									if (option->dwOptionType == OPTION_CUSTOMSTRING) {
										if (Compare((PCHAR)option->GetPSyncVariable(), __EMPTYGTSTRING__)) {
											// We don't need to save empty gamertag options to the config file, as it just takes up space for no reason 
											continue;
										}
									}
									DWORD dwArraySize = 0;
									if (option->dwSaveFlags & FLAG_SINGLE) {
										dwArraySize = 1;
									}
									else if (option->dwSaveFlags & FLAG_VEC2) {
										dwArraySize = 2;
									}
									else if (option->dwSaveFlags & FLAG_VEC3) {
										dwArraySize = 3;
									}
									else if (option->dwSaveFlags & FLAG_VEC4) {
										dwArraySize = 4;
									}
									dwNumOptionsSaved += SaveArray(&sEngineIni, option, dwArraySize);
								}
							}
						}
					}
				}
			}
			sEngineIni.close();
			if (dwNumOptionsSaved > 0) {
				DbgPrint("Successfully saved %i menu options to config!", dwNumOptionsSaved);
				bSuccess = TRUE;

				DeleteFile(ES("NiNJA:\\Engine.ini"));
				MoveFile(ES("NiNJA:\\Engine.ini.tmp"), ES("NiNJA:\\Engine.ini"));
			}
			else {
				DbgPrint("Failed to save menu options, or no menu options have FLAG_SAVE!");
			}
		}
	}
	// We tried to save, even if we failed mark it as saved.
	bShouldSave = FALSE;
	return bSuccess;
}

#pragma endregion

/* Any functions that change the state of the menu go here, such as switching menus, option state, etc */
#pragma region Menu Navigation
CHAR pCurrentMenuTitle[0x100];
VOID MenuManager::EnterMenu(DWORD dwMenu, BOOL bEffects, BOOL bSaveLastMenu) {
	Menu *pNewMenu = pThis->GetMenu(dwMenu);
	if (pNewMenu) {
		if (bSaveLastMenu) {
			pNewMenu->mLastMenu = pThis->dwCurrentMenu;
		}
		pThis->dwCurrentMenu = (eMenuLevels)dwMenu;

		for (DWORD i = 0; i < __MAXMENUOPTIONS__; i++) {
			pThis->DynamicMenuElems[i].dwIndex = i;
			if (pThis->DynamicMenuElems[i].Control) {
				pThis->DynamicMenuElems[i].Control->SetVisible(FALSE);
			}
		}

		// Get the number of options in the new menu
		DWORD dwNumOptions = pNewMenu->Options.GetSize();

		// Reflect any changes we made to our menu option displayName (mainly for client menu)
		pThis->RefreshMenuText(dwMenu, dwNumOptions);

		if (pNewMenu->DisplayName) {
			//string sTitleText = ReplaceButtonsInString("[[BUTTON_LB]]  " + string(pNewMenu->DisplayName) + "  [[BUTTON_RB]]");
			//strcpy(pCurrentMenuTitle, sTitleText.c_str());
			//sTitleText.clear();

			strcpy(pCurrentMenuTitle, pNewMenu->DisplayName);
		}
		else {
			ZeroMemory(pCurrentMenuTitle, 0x100);
		}
		pThis->TitleText.pText = pCurrentMenuTitle;
		for (DWORD i = 0; i < __MAXMENUOPTIONS__; i++) {
			if (i < dwNumOptions) {
				pThis->DynamicMenuElems[i].pParent = pNewMenu->Options.Get(pThis->DynamicMenuElems[i].dwIndex);
				MenuOption *option = pNewMenu->Options.Get(pThis->DynamicMenuElems[i].dwIndex);
				if (option) {
					pThis->DynamicMenuElems[i].Control = option->pControl;
					if (CG::bMenuOpen) {
						//pThis->DynamicMenuElems[i].Material.visible = TRUE;
						pThis->DynamicMenuElems[i].Text.bVisible = TRUE;
						if (pThis->DynamicMenuElems[i].Control) {
							// If the control elem exists, set it as visible
							pThis->DynamicMenuElems[i].Control->SetVisible(TRUE);
						}
					}
				}
				if (pThis->DynamicMenuElems[i].Control) {
					// Set the scale of the control, even if its not visible
					pThis->DynamicMenuElems[i].Control->SetScale(CG::GetFontScale() );
				}
			}
			else {
				if (CG::bMenuOpen) {
					//pThis->DynamicMenuElems[i].Material.visible = FALSE;
					pThis->DynamicMenuElems[i].Text.bVisible = FALSE;
					if (pThis->DynamicMenuElems[i].Control) {
						pThis->DynamicMenuElems[i].Control->SetVisible(FALSE);
					}
				}
			}
		}
		// Update our menu display for how many options we actually have
		pThis->UpdateMenuElems(dwNumOptions, bEffects);

		// Finally, loop through the elems one last time and set the visual state of all the controls
		for (DWORD i = 0; i < __MAXMENUOPTIONS__; i++) {
			if (i < dwNumOptions) {
				if (pThis->DynamicMenuElems[i].Control) {
					// Set the visual state of it so it's set to the correct starting value
					pNewMenu->Options.Get(pThis->DynamicMenuElems[i].dwIndex)->SetVisualState();
				}
			}
		}
		// The position of our controls
		pThis->SetControlPosition(CG::dwMenuPos[0], CG::dwMenuPos[1]);

		// Reset our selection to whatever it used to be the last time we were in this menu
		pThis->SetSelection(pNewMenu->dwSelection);

		DbgPrint("Switching to menu %i [%s]", dwMenu, pThis->GetMenu(dwMenu)->DisplayName);
	}
}

VOID MenuManager::OnPlayerSpawned() {
	DbgPrintE("OnPlayerSpawned");
	for (DWORD i = 0; i < MENU_SIZE; i++) {
		MenuOptions *options = GetMenuOptions(i);
		if (options ) {
			for (DWORD o = 0; o < options->GetSize(); o++) {
				MenuOption* option = options->Get(o);
				if (option) {
					if (option->dwFlags & FLAG_APPLYONENTERBODY) {
						// If our menu option calls a function on toggle, that means we want to call the function to set the initial value when we load it
						DbgPrintE("Setting value of %s!", option->pDisplayName);
						option->Call();
					}
				}
			}
		}
	}
}

/* Switches menu to the next, or previous menu in the list. This is not for submenus */
VOID MenuManager::ToggleMenu(BOOL bForward) {
	int iLevel = (DWORD)dwCurrentMenu;

jRetry:
	iLevel += (bForward ? 1 : -1);

	if (bForward) {
		if (iLevel > MENU_SIZE-1) {
			iLevel = 0;
		}
	}
	else {
		if (iLevel < 0) {
			iLevel = MENU_SIZE - 1;
		}
	}

	Menu *pTmpMenu = GetMenu((DWORD)iLevel);
	if (!pTmpMenu->bDisplay || pTmpMenu->bSubmenu) {
		goto jRetry;
	}

	dwCurrentMenu = (eMenuLevels)iLevel;
	Menu *pNewMenu = GetMenu(dwCurrentMenu);
	if (pNewMenu) {
		EnterMenu(dwCurrentMenu);
	}
}

/* Scrolls up or down in the currently selected menu */
VOID MenuManager::Scroll(BOOL bUp) {
	Menu *pMenu = GetMenu(dwCurrentMenu);
	if (pMenu) {
		int iNumOptions = pMenu->Options.GetSize();
		if (iNumOptions > 1) {
			int iCurrentValue = pMenu->dwSelection;

		jRestart:
			iCurrentValue = iCurrentValue + (bUp ? -1 : 1);

			// Return if we have come all the way back around to the value we started at, to avoid an infinite loop in menus that contain more than one option, but only 1 of them is a selectable option.
			if (iCurrentValue == pMenu->dwSelection) {
				return;
			}

			if (iCurrentValue > iNumOptions - 1) {
				iCurrentValue = 0;
			}
			else if (iCurrentValue < 0) {
				iCurrentValue = iNumOptions - 1;
			}
			// There are some option types we don't want to ever select in the menu, that are there for informational purposes only. Skip these options.
			switch(Menus[dwCurrentMenu].Options.Get(iCurrentValue)->dwOptionType) {
				case OPTION_COLOR_PREVIEW:
				case OPTION_INFORMATION:
					goto jRestart;
			}
			SetSelection(iCurrentValue);
		}
	}
}

/* Sets the current menu selection, bEffects is unimplemented */
VOID MenuManager::SetSelection(DWORD dwSelection, BOOL bEffects) {
	Menu *pMenu = GetMenu(dwCurrentMenu);
	if (pMenu) {
		pMenu->dwSelection = dwSelection;

	jRestart:


		DWORD dwNumOptions = pMenu->Options.GetSize();
		BOOL bFoundSelection = FALSE;
		for (DWORD i = 0; i < __MAXMENUOPTIONS__; i++) {
			// No need to set the colors/text of options that aren't being shown
			if (i >= dwNumOptions) {
				break;
			}

			// Set up the text for the current elem
			DynamicMenuElems[i].Text.pText = pMenu->Options.Get(DynamicMenuElems[i].dwIndex)->GetDisplayName();

			// Check if our selection is the current elem
			if (pMenu->dwSelection == DynamicMenuElems[i].dwIndex) {
				memcpy(DynamicMenuElems[i].Text.pColor, CG::cSelectedTextColor, 0x10); // Colors::Cyan;
				if (DynamicMenuElems[i].Control) {
					DynamicMenuElems[i].Control->SetTextColor(CG::cSelectedTextColor);
				}
				PCHAR pDescriptionText = Menus[dwCurrentMenu].Options.Get(DynamicMenuElems[i].dwIndex)->pDescriptionText;
				// Set our description text, required for our menu height to be correct
				if (pDescriptionText != NULL) {
					Description.SetTextAndCountLines(pDescriptionText);
					UpdateMenuElems(dwNumOptions, TRUE);
				}
				bFoundSelection = TRUE;
			}
			else {
				memcpy(DynamicMenuElems[i].Text.pColor, CG::cMenuTextColor, 0x10);
				if (DynamicMenuElems[i].Control) {
					DynamicMenuElems[i].Control->SetTextColor(CG::cMenuTextColor);
				}
			}
		}

		if (!bFoundSelection) {
			int iMod = 0;

			if (DynamicMenuElems[__MAXMENUOPTIONS__ - 1].dwIndex < dwSelection) {
				iMod = dwSelection - DynamicMenuElems[__MAXMENUOPTIONS__ - 1].dwIndex;
			}
			else if (DynamicMenuElems[0].dwIndex > dwSelection) {
				iMod = dwSelection - DynamicMenuElems[0].dwIndex;
			}

			// Set the current elems to invisible
			for (DWORD i = 0; i < __MAXMENUOPTIONS__; i++) {
				if (DynamicMenuElems[i].Control) {
					DynamicMenuElems[i].Control->SetVisible(FALSE);
				}
			}

			// Set the elem indicies based on our current scroll value... this will give us a "scrollbar" effect
			for (DWORD i = 0; i < __MAXMENUOPTIONS__; i++) {
				DynamicMenuElems[i].dwIndex += iMod;
				MenuOption *option = pMenu->Options.Get(pThis->DynamicMenuElems[i].dwIndex);
				if (option) {
					DynamicMenuElems[i].Control = option->pControl;
					if (DynamicMenuElems[i].Control && bMenuVisible) {
						DynamicMenuElems[i].Control->SetVisible(TRUE);
						pMenu->Options.Get(DynamicMenuElems[i].dwIndex)->SetVisualState();
					}
				}
			}
			goto jRestart;
		}
	}
}
#pragma endregion

/* Any functions associated with button presses in the menu */
#pragma region Menu Button Monitoring
/* Monitors button presses for menu events */
DWORD dwLastPacketNum = 0;
bool bFirstMenuRun = true;
VOID MenuManager::MonitorButtons() {
	// Poll our controller, and if successful apply actions based on what we pressed
	if (CG::cButtons.Poll()) {
		if (!CG::cButtons.bButtonJustPressed) {
			// Save our last packet num so we don't repeat button presses
			dwLastPacketNum = CG::cButtons.dwPacketNum;

			// If the user is opening or closing the menu
			if ((CG::cButtons.dwFlags & dwMenuOpenButton) == dwMenuOpenButton) {
				// Swap our menu state
				CG::bMenuOpen = !CG::bMenuOpen;

				if (bFirstMenuRun) {
					// Initialize all our menus by entering each one
					for (DWORD i = 0; i < Menus.size(); i++) {
						Menu* pMenu = &Menus[i];
						if (pMenu && pMenu->bDisplay) {
							this->EnterMenu(i);
							if (pMenu->Options.GetSize() > __MAXMENUOPTIONS__) {
								this->SetSelection(pMenu->Options.GetSize() - 1);
								this->EnterMenu(i);
								this->SetSelection(0);
							}
						}
					}
					// Finally go back to the main menu once all the others have been initialized
					this->EnterMenu(MENU_MAIN);

					bFirstMenuRun = false;
				}

				//DWORD menuX = CG::dwMenuPos[0], menuY = CG::dwMenuPos[1];
				//if (CG::bMenuOpen) {
				//	SetMenuPosition(NiNJA.dwScreenMaxX + CG::fMenuWidth, menuY);
				//	SetVisibility(CG::bMenuOpen == TRUE);
				//	SetMenuPosition(menuX, menuY, TRUE, 400, GetTickCount() + 10, TRUE);
				//}
				//else {
				//	SetMenuPosition(-CG::fMenuWidth, menuY, TRUE, 400, GetTickCount(), TRUE);
				//	//SetVisibility(CG::bMenuOpen == TRUE);
				//	//SetMenuPosition(menuX, menuY);
				//}

				SetVisibility(CG::bMenuOpen == TRUE);

				if (CG::bMenuOpen) {
					// Enter our current menu
					EnterMenu(dwCurrentMenu, TRUE, FALSE);
				}
				else {
					// Save the menu when we close it 
					bShouldSave = TRUE;
				}
				return;
			}

			if (!CG::bMovingMenu) {
				// If we just switched menus, don't monitor any other button presses this frame
				if (CG::cButtons.dwFlags & BUTTON_RB) {
					ToggleMenu(TRUE);
					return;
				}
				else if (CG::cButtons.dwFlags & BUTTON_LB) {
					ToggleMenu(FALSE);
					return;
				}
			}
		}


		// Handle button monitoring for menu options
		if (CG::bMenuOpen) {
			Menu *pCurrentMenu = GetMenu(dwCurrentMenu);
			if (pCurrentMenu) {
				MenuOption *pCurrentOption = pCurrentMenu->Options.Get(pCurrentMenu->dwSelection);
				if (!CG::bMovingMenu) {
					if (!CG::cButtons.bThumbstickJustMoved) {
						if ((CG::cButtons.dwThumbFlags & APAD_LTHUMB_UP) && (!CG::bInGameCached || CG::bAnalogControl)) {
							Scroll(TRUE);
						}
						else if ((CG::cButtons.dwThumbFlags & APAD_LTHUMB_DOWN) && (!CG::bInGameCached || CG::bAnalogControl)) {
							Scroll(FALSE);
						}
					}

					// Scroll up or down based on what button they press
					if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & DPAD_UP) {
						Scroll(TRUE);
					}
					else if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & DPAD_DOWN) {
						Scroll(FALSE);
					}
					// If we aren't switching menus or scrolling up and down, check if we're toggling any menu options
					else {
						if (!CG::cButtons.bButtonJustPressed && pCurrentMenu->bSubmenu && (CG::cButtons.dwFlags & BUTTON_B)) {
							EnterMenu(pCurrentMenu->mLastMenu, TRUE, FALSE);
						}
						else if (pCurrentOption) {
							switch (pCurrentOption->dwOptionType) {
							case OPTION_CUSTOMSTRING:
								if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & BUTTON_A) {
									pCurrentOption->Call(BUTTON_A);
								}
								else if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & BUTTON_X) {
									pCurrentOption->Call(BUTTON_X);
								}
								break;
							case OPTION_DEFAULT:
								// If the list has a function call associated with it, call it when we press A
								if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & BUTTON_A) {
									pCurrentOption->Call(BUTTON_A);
								}
								break;
							case OPTION_LIST:
							case OPTION_SLIDER:
								// For sliders or list, simply toggle the list or slider up or down depending on if we press left or right on the dpad
								if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & BUTTON_A) {
									pCurrentOption->Call(BUTTON_A);
								}
								else if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & DPAD_LEFT) {
									pCurrentOption->Toggle(FALSE);
								}
								else if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & DPAD_RIGHT) {
									pCurrentOption->Toggle(TRUE);
								}
								else if (!CG::cButtons.bThumbstickJustMoved && (CG::cButtons.dwThumbFlags & APAD_LTHUMB_LEFT) && (!CG::bInGameCached || CG::bAnalogControl)) {
									pCurrentOption->Toggle(FALSE);
								}
								else if (!CG::cButtons.bThumbstickJustMoved && (CG::cButtons.dwThumbFlags & APAD_LTHUMB_RIGHT) && (!CG::bInGameCached || CG::bAnalogControl)) {
									pCurrentOption->Toggle(TRUE);
								}
								break;
							case OPTION_CHECKBOX:
								// If the option type is checkbox, and we press A, toggle the checkbox state
								if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & BUTTON_A) {
									pCurrentOption->Toggle(TRUE);
								}
								break;
							case OPTION_SUBMENU:
							case OPTION_COLOR:
								// If the option type is submenu, and we press A, call the associated function to open the submenu
								if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & BUTTON_A) {
									pCurrentOption->Call(BUTTON_A);
								}
								break;
							}
						}
					}
				}
				// If we're moving the menu
				else {
					bool bMenuMoved = false;
					DWORD dwXDiff = 0;
					if (CG::cButtons.dwThumbFlags & APAD_LTHUMB_UP) {
						CG::dwMenuPos[1] -= 2;
						bMenuMoved = true;
					}
					if (CG::cButtons.dwThumbFlags & APAD_LTHUMB_DOWN) {
						CG::dwMenuPos[1] += 2;
						bMenuMoved = true;
					}
					if (CG::cButtons.dwThumbFlags & APAD_LTHUMB_LEFT) {
						CG::dwMenuPos[0] -= 2;
						dwXDiff = -2;
						bMenuMoved = true;
					}
					if (CG::cButtons.dwThumbFlags & APAD_LTHUMB_RIGHT) {
						CG::dwMenuPos[0] += 2;
						dwXDiff = 2;
						bMenuMoved = true;
					}

					if (bMenuMoved) {
						SetMenuPosition(CG::dwMenuPos[0], CG::dwMenuPos[1], TRUE, 1, GetTickCount(), TRUE);
						for (DWORD i = 0; i < ARRAY_SIZE(DynamicMenuElems); i++) {
							if (DynamicMenuElems[i].Control) {
								MenuOption *pParent = (MenuOption*)DynamicMenuElems[i].Control->pParent;
								if (pParent) {
									if (pParent->dwOptionType == OPTION_SLIDER) {
										DynamicMenuElems[i].Control->Elems[1]->fX += dwXDiff;
									}
								}
							}
						}
					}
					if (!CG::cButtons.bButtonJustPressed) {
						if (!CG::cButtons.bButtonJustPressed && CG::cButtons.dwFlags & BUTTON_A) {
							pCurrentOption->Toggle(TRUE);
						}
					}
				}
			}
		}
	}
}
#pragma endregion