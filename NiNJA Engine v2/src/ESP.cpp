#include "ESP.h"
#include "GameFuncs.h"
#include "Aimbot.h"
#include "UI.h"

CRectDef ESP::lastCompass;
DWORD ESP::lastCompassType = 0;
DWORD ESP::drawCompassTimer = 0;
bool ESP::bAnyEnemyVisible = false;
bool ESP::bAnyEnemyWallbangable = false;

FLOAT *ESP::GetESPColor(DWORD dwClient) {
	if (CG::cEntities.List[dwClient].bEnemy) {
		if (CG::cEntities.List[dwClient].pExt) {
			if (CG::cEntities.List[dwClient].pExt->bWhitelisted) {
				return CG::cWhitelistedEspColor;
			}
			else if (CG::cEntities.List[dwClient].pExt->bVisible) {
				return CG::cVisibleEspColor;
			}
			else if (CG::cEntities.List[dwClient].pExt->bWallbangable) {
				return CG::cWallbangableEspColor;
			}
			else if (CG::cEntities.List[dwClient].pExt->bPreferred) {
				return CG::cPreferredEspColor;
			}
		}
		return CG::cEnemyEspColor;
	}
	else {
		return CG::cFriendlyEspColor;
	}
}

FLOAT ESP::fTopLeftOffsetX = 35.0f, ESP::fTopLeftOffsetY = 25.0f;
VOID ESP::DrawDebugMessage(int iIndex, PCHAR pMsg) {
	FLOAT fTextWidth = GameFuncs::GetTextWidth(pMsg, CG::dwSmallFont);
	//GameFuncs::DrawBox((FLOAT)NiNJA.dwScreenMaxX - fTopLeftOffsetX - fTextWidth - 4.0f, fTopLeftOffsetY - (CG::fMenuFontHeight * 0.7) - 4.0f + (CG::fMenuFontHeight * iIndex), fTextWidth + 8.0f + 1.0f, (CG::fMenuFontHeight * 0.7f) + 4.0f, 0.0f, CG::cTopRightBackgroundColor);// , Colors::WhiteTrans, 2.0f);
	FLOAT fScale = CG::GetFontScale() - 0.2f < 0.7f ? 0.7f : CG::GetFontScale() - 0.2f, fHeight = GameFuncs::GetFontHeight(CG::dwSmallFont, fScale + 0.1f);

	if (NiNJA.dwScreenMaxY < 720) {
		fScale -= 0.1f;
	}

	GameFuncs::DrawText(pMsg, fTextWidth, CG::dwSmallFont, (FLOAT)NiNJA.dwScreenMaxX - fTextWidth * fScale - fTopLeftOffsetX, fTopLeftOffsetY + fHeight + (fHeight * iIndex), fScale, fScale, 0.0f, CG::cTopRightTextColor, true, 2.0f);
}

CHAR pInstructionText[200] = { 0 };
CHAR pNavigationText[200] = { 0 };
VOID ESP::DrawInformationOverlay() {
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

	vector<string> vDebugMessages;

	int iIndex = 0;

	if (NiNJA.bHasReadConfig) {
		if (CG::bShowEngineLogo) {
			string sEngineLogo = string(ES("NiNJA Engine v2.1.6"));
			// Draw our engine logo
			vDebugMessages.push_back(sEngineLogo);
			iIndex++;
		}

		if (CG::bShowHost) {
			PCHAR pHostName = GameFuncs::GetHostName();
			if (pHostName && pHostName[0] != 0) {
				string sHost = "Host: " + string(pHostName);
				vDebugMessages.push_back(sHost);
				iIndex++;
			}
		}

		if (CG::bInGameCached && CG::bLobbyInitialized) {
			if (CG::bShowMapInfo) {
				PCHAR pMapName = GameFuncs::GetMapName();
				PCHAR pGametype = CG::pGameTypeDisplayName;
				if ((pMapName && pMapName[0] != 0) && (pGametype && pGametype[0] != 0)) {
					CHAR pNewBuff[0x100];
					sprintf(pNewBuff, "Playing %s on %s", pGametype, string(pMapName).c_str());
					vDebugMessages.push_back(string(pNewBuff));
					iIndex++;
				}
			}
		}

		if (CG::bDrawFPS) {
			CHAR pFPSBuff[10];
			// Draw our FPS text
			string sFPS = "FPS: " + string(itoa(GameFuncs::CalculateFPS(), pFPSBuff, 10));
			vDebugMessages.push_back(sFPS);
			iIndex++;
		}

		if (CG::bShowPing) {
			DWORD dwPing = GameFuncs::GetPing();
			if (dwPing != 0) {
				CHAR pPingBuff[10];
				string sPing = "Ping: " + string(itoa(dwPing, pPingBuff, 10));
				vDebugMessages.push_back(sPing);
				iIndex++;
			}
		}

		for (DWORD i = 0; i < vDebugMessages.size(); i++) {
			DrawDebugMessage(i, (PCHAR)vDebugMessages[i].c_str());
		}

		if (CG::bShowMenuInstructions) {
			if (pInstructionText[0] == 0) {
				string sText = ReplaceButtonsInString(string("Press [[DPAD_LEFT]] & [[BUTTON_LT]] To %s ^7NiNJA Engine"));
				strcpy(pInstructionText, sText.c_str());
			}

			CHAR pMsg[200];
			sprintf(pMsg, pInstructionText, CG::bMenuOpen ? "Close" : "Open");
			FLOAT fTextWidth = GameFuncs::GetTextWidth(pMsg, CG::dwSmallFont);
			FLOAT fScale = CG::GetFontScale() + 0.3f;
			if (NiNJA.dwScreenMaxY < 720) {
				fScale -= 0.2f;
			}
			//GameFuncs::DrawBox((FLOAT)NiNJA.dwScreenMaxX - (fTopLeftOffsetX * 2) - fTextWidth - 4.0f, NiNJA.dwScreenMaxY - fTopLeftOffsetY*1.1f - (CG::fMenuFontHeight *  1.1f), fTextWidth + 8.0f + 2.0f, (CG::fMenuFontHeight * 1.1f) + 4.0f, 0.0f, CG::cTopRightBackgroundColor);// , Colors::WhiteTrans, 2.0f);
			GameFuncs::DrawText(pMsg, fTextWidth, CG::dwSmallFont, (FLOAT)NiNJA.dwScreenMaxX - fTextWidth * fScale - fTopLeftOffsetX, NiNJA.dwScreenMaxY - fTopLeftOffsetY, fScale, fScale, 0.0f, CG::cTopRightTextColor, true, 2.0f);

			if (UI::mManager.bMenuVisible) {
				if (pNavigationText[0] == 0) {
					string sText = ReplaceButtonsInString(string("[[BUTTON_LB]]/[[BUTTON_RB]] Submenus  [[BUTTON_A]] Select  [[BUTTON_B]] Back"));
					strcpy(pNavigationText, sText.c_str());
				}

				fTextWidth = GameFuncs::GetTextWidth(pNavigationText, CG::dwSmallFont);
				//GameFuncs::DrawBox((fTopLeftOffsetX * 2) - 2.0f, NiNJA.dwScreenMaxY - fTopLeftOffsetY*1.1f -(CG::fMenuFontHeight *  1.1f), fTextWidth + 8.0f + 2.0, (CG::fMenuFontHeight * 1.1f) + 4.0f, 0.0f, CG::cTopRightBackgroundColor);// , Colors::WhiteTrans, 2.0f);
				GameFuncs::DrawText(pNavigationText, fTextWidth, CG::dwSmallFont, fTopLeftOffsetX, NiNJA.dwScreenMaxY - fTopLeftOffsetY, fScale, fScale, 0.0f, CG::cTopRightTextColor, true, 4.0f);
			}
		}
	}

	if (vDebugMessages.size() > 0) {
		vDebugMessages.clear();
	}
}

VOID ESP::DrawBoneESP(DWORD dwClient) {
	FLOAT *pColor = GetESPColor(dwClient);

#if defined(GHOSTS)
	if (CG::cEntities.List[dwClient].bAI) {
		return;
	}
#endif

	for (DWORD i = 1; i <= TAG_SHOULDER_RI; i++) {
		TagInfo *cTag = &CG::cEntities.List[dwClient].pExt->cTags[i];
		if (!GameFuncs::WorldToScreen(&cTag->vPos, &cTag->vScreenPos.x, &cTag->vScreenPos.y)) {
			if (i <= TAG_SHOULDER_RI && i != TAG_HELMET) {
				//DbgPrint("Failed to run WorldToScreen on tag %s", CG::cTags.List[i].pName);
				return;
			}
		}
		/*vec3_t vNewPos = GameFuncs::GetTagOffset(i, dwClient, 1), vNewPos2 = GameFuncs::GetTagOffset(i, dwClient, 2);
		FLOAT fX, fY, fX2, fY2;
		if (GameFuncs::WorldToScreen(&vNewPos, &fX, &fY) && GameFuncs::WorldToScreen(&vNewPos2, &fX2, &fY2)) {
			if (i <= TAG_SHOULDER_RI || CG::cEntities.List[dwClient].pExt->bPreferred) {
				GameFuncs::DrawRotatedPic(cTag->vScreenPos.x - 1.0f, cTag->vScreenPos.y - 1.0f, 2.0f, 2.0f, 0.0f, Colors::Green, CG::dwWhite);
				GameFuncs::DrawRotatedPic(fX - 1.0f, fY - 1.0f, 2.0f, 2.0f, 0.0f, Colors::Red, CG::dwWhite);
				GameFuncs::DrawRotatedPic(fX2 - 1.0f, fY2 - 1.0f, 2.0f, 2.0f, 0.0f, Colors::Blue, CG::dwWhite);
				if (CG::cButtons.dwFlags & BUTTON_A) {
					PCHAR txt = GameFuncs::va("%i", i);
					GameFuncs::DrawText(txt, GameFuncs::GetTextWidth(txt, CG::dwSmallFont), CG::dwSmallFont, cTag->vScreenPos.x, cTag->vScreenPos.y, 0.5f, 0.5f, 0.0f, Colors::White, true);
				}
			}
		}*/
	}

	// If we got this far, draw the esp!
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBones == ESP_ENEMY || CG::dwESPBones == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBones == ESP_FRIENDLY || CG::dwESPBones == ESP_BOTH)) {

		ExtendedEntity *pExt = CG::cEntities.List[dwClient].pExt;
		if (pExt) {
			FLOAT xSpineCustom = (pExt->cTags[TAG_MAINROOT].vScreenPos.x * 8 + pExt->cTags[TAG_NECK].vScreenPos.x) / 9; //point between j_mainroot and j_neck
			FLOAT ySpineCustom = (pExt->cTags[TAG_MAINROOT].vScreenPos.y * 8 + pExt->cTags[TAG_NECK].vScreenPos.y) / 9;

			FLOAT width = 2.0f;
			GameFuncs::DrawLine(pExt->cTags[TAG_NECK].vScreenPos.x, pExt->cTags[TAG_NECK].vScreenPos.y, //From j_neck
				pExt->cTags[TAG_SHOULDER_LE].vScreenPos.x, pExt->cTags[TAG_SHOULDER_LE].vScreenPos.y, width, pColor, CG::dwWhite); //To j_shoulder_le
			GameFuncs::DrawLine(pExt->cTags[TAG_NECK].vScreenPos.x, pExt->cTags[TAG_NECK].vScreenPos.y, //From j_neck
				pExt->cTags[TAG_SHOULDER_RI].vScreenPos.x, pExt->cTags[TAG_SHOULDER_RI].vScreenPos.y, width, pColor, CG::dwWhite); //To j_shoulder_ri
			GameFuncs::DrawLine(pExt->cTags[TAG_SHOULDER_LE].vScreenPos.x, pExt->cTags[TAG_SHOULDER_LE].vScreenPos.y, //From j_shoulder_le
				pExt->cTags[TAG_ELBOW_LE].vScreenPos.x, pExt->cTags[TAG_ELBOW_LE].vScreenPos.y, width, pColor, CG::dwWhite); //To j_elbow_le
			GameFuncs::DrawLine(pExt->cTags[TAG_SHOULDER_RI].vScreenPos.x, pExt->cTags[TAG_SHOULDER_RI].vScreenPos.y, //From j_shoulder_ri
				pExt->cTags[TAG_ELBOW_RI].vScreenPos.x, pExt->cTags[TAG_ELBOW_RI].vScreenPos.y, width, pColor, CG::dwWhite); //To j_elbow_ri
			GameFuncs::DrawLine(pExt->cTags[TAG_WRIST_LE].vScreenPos.x, pExt->cTags[TAG_WRIST_LE].vScreenPos.y, //From j_wrist_le
				pExt->cTags[TAG_ELBOW_LE].vScreenPos.x, pExt->cTags[TAG_ELBOW_LE].vScreenPos.y, width, pColor, CG::dwWhite); //To j_elbow_le
			GameFuncs::DrawLine(pExt->cTags[TAG_WRIST_RI].vScreenPos.x, pExt->cTags[TAG_WRIST_RI].vScreenPos.y, //From j_wrist_ri
				pExt->cTags[TAG_ELBOW_RI].vScreenPos.x, pExt->cTags[TAG_ELBOW_RI].vScreenPos.y, width, pColor, CG::dwWhite); //To j_elbow_ri
			GameFuncs::DrawLine(xSpineCustom, ySpineCustom, //From SpineCustom
				pExt->cTags[TAG_NECK].vScreenPos.x, pExt->cTags[TAG_NECK].vScreenPos.y, width, pColor, CG::dwWhite); //To j_neck
			GameFuncs::DrawLine(xSpineCustom, ySpineCustom, //From SpineCustom
				pExt->cTags[TAG_KNEE_LE].vScreenPos.x, pExt->cTags[TAG_KNEE_LE].vScreenPos.y, width, pColor, CG::dwWhite); //To j_knee_le
			GameFuncs::DrawLine(xSpineCustom, ySpineCustom, //From SpineCustom
				pExt->cTags[TAG_KNEE_RI].vScreenPos.x, pExt->cTags[TAG_KNEE_RI].vScreenPos.y, width, pColor, CG::dwWhite); //To j_knee_ri
			GameFuncs::DrawLine(pExt->cTags[TAG_ANKLE_LE].vScreenPos.x, pExt->cTags[TAG_ANKLE_LE].vScreenPos.y, //From j_ankle_le
				pExt->cTags[TAG_KNEE_LE].vScreenPos.x, pExt->cTags[TAG_KNEE_LE].vScreenPos.y, width, pColor, CG::dwWhite); //To j_knee_le
			GameFuncs::DrawLine(pExt->cTags[TAG_ANKLE_RI].vScreenPos.x, pExt->cTags[TAG_ANKLE_RI].vScreenPos.y, //From j_ankle_ri
				pExt->cTags[TAG_KNEE_RI].vScreenPos.x, pExt->cTags[TAG_KNEE_RI].vScreenPos.y, width, pColor, CG::dwWhite); //To j_knee_ri

		//#ifdef DEBUG
			//// Draw a box around the shield, do we even want this? 
			//if (CG::cEntities.List[dwClient].pExt->bPrimaryShield || CG::cEntities.List[dwClient].pExt->bSecondaryShield) {
			//	ShieldInfo tInfo = CG::cEntities.List[dwClient].pExt->oShieldInfo;
			//	vec2_t vTopRight, vTopLeft, vBottomRight, vBottomLeft;
			//	if (GameFuncs::WorldToScreen(&tInfo.pBounds[0], &vTopRight.x, &vTopRight.y) && GameFuncs::WorldToScreen(&tInfo.pBounds[1], &vTopLeft.x, &vTopLeft.y) && GameFuncs::WorldToScreen(&tInfo.pBounds[3], &vBottomRight.x, &vBottomRight.y) && GameFuncs::WorldToScreen(&tInfo.pBounds[2], &vBottomLeft.x, &vBottomLeft.y)) {
			//		GameFuncs::DrawLine(vTopRight.x, vTopRight.y, vTopLeft.x, vTopLeft.y, width, Colors::Red, CG::dwWhite); // top right to top left
			//		GameFuncs::DrawLine(vTopRight.x, vTopRight.y, vBottomRight.x, vBottomRight.y, width, Colors::Green, CG::dwWhite); // top right to bottom right
			//		GameFuncs::DrawLine(vBottomLeft.x, vBottomLeft.y, vTopLeft.x, vTopLeft.y, width, Colors::Blue, CG::dwWhite); // bottom left to top left
			//		GameFuncs::DrawLine(vBottomLeft.x, vBottomLeft.y, vBottomRight.x, vBottomRight.y, width, Colors::White, CG::dwWhite); // bottom left to bottom right
			//	}
			//}
		//#endif
		}
	}
}

FLOAT fDistScale = 0.5f;
VOID ESP::DrawNameESP(DWORD dwClient, vec2_t vTopPos, vec2_t vCenterPos, vec2_t vBottomPos) {
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwESPName == ESP_ENEMY || CG::dwESPName == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwESPName == ESP_FRIENDLY || CG::dwESPName == ESP_BOTH)) {

		// Draw nametag a bit lower for dogs
		if (CG::bIsMultiplayer && !CG::cEntities.List[dwClient].bPlayer) {
			vTopPos = vCenterPos;
		}

		// Grab the esp color
		FLOAT *pColor = GetESPColor(dwClient);

		// Add in our distance indicator to the nametag
		char *nameTag = GameFuncs::va("%s", CG::cEntities.List[dwClient].pExt->GetName());

		if (nameTag && strlen(nameTag) > 0) {
			// Grab the width of the new nametag, including the distance indicator
			FLOAT width = GameFuncs::GetTextWidth(nameTag, CG::dwSmallFont);
			GameFuncs::DrawText(nameTag, width, CG::dwSmallFont, vTopPos.x - CG::fESPNameScale *width / 2, vTopPos.y - 2.0f, CG::fESPNameScale, CG::fESPNameScale, 0, Colors::White, true);
		}
	}
}

VOID ESP::DrawDistanceESP(DWORD dwClient, vec2_t vTopPos, vec2_t vBottomPos) {
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwESPDistance == ESP_ENEMY || CG::dwESPDistance == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwESPDistance == ESP_FRIENDLY || CG::dwESPDistance == ESP_BOTH)) {
		// Grab the esp color
		FLOAT *pColor = GetESPColor(dwClient);

		// Draw our distance ESP
		char *distance = GameFuncs::va("%im", (int)floor(CG::cEntities.List[dwClient].fDistance / 100.0f));// , dwClient, CG::cEntities.List[dwClient].fDistance / 100.0f);

		FLOAT width = GameFuncs::GetTextWidth(distance, CG::dwSmallFont);
		//GameFuncs::DrawTextWithBorder(distance, CG::dwSmallFont, vBottomPos.x - fDistScale *width / 2, vBottomPos.y + (CG::fMenuFontHeight * fDistScale) + 2.0f, fDistScale, fDistScale, 1.0f, 0, pColor, Colors::BlackTrans);
		GameFuncs::DrawText(distance, width, CG::dwSmallFont, vBottomPos.x - fDistScale *width / 2, vBottomPos.y + (CG::fMenuFontHeight * fDistScale) + 2.0f, fDistScale, fDistScale,  0, Colors::White, true);
	}
}

int GetBoxHeight(DWORD dwStance) {
	int BoxHeight = 55;
	if (dwStance & 0x8)
		BoxHeight = 17;
	else if (dwStance & 0x4)
		BoxHeight = 35;
	return BoxHeight;
}

int GetBoxWidth(DWORD dwStance) {
	if (dwStance & 0x08)
		return 35;
	else 
		return 17;
}

VOID GetIconAspectRatio(DWORD killIconType, FLOAT *aspectWidth, FLOAT *aspectHeight) {
	FLOAT width = 1.4f, height = 1.4f;
	if (killIconType == 0) { //1:1
		width = 1.4f;
		height = 1.4f;
	}
	else if (killIconType == 1) { //2:1
		width = 2.8f;
		height = 1.4f;

	}
	else if (killIconType == 2) { //3:1
		width = 2.8f;
		height = 0.7f;
	}
	width *= 30.0f;
	height *= 30.0f;
	*aspectWidth = width;
	*aspectHeight = height;
}

DWORD ESP::GetKillIconType(DWORD dwWeaponIndex) {
#if defined(WAW)
	DWORD weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x614);
	}
#elif defined(MW1)
	DWORD weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x51C);
	}
#elif defined(BO1)
	DWORD weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x57c);
	}
#elif defined(BO2)
	DWORD weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x664);
	}
#elif defined(MW2)
	DWORD weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x3D0);
	}
#elif defined(MW3)
	DWORD weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x4C8);
	}
#elif defined(GHOSTS)
	DWORD weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex, true);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x528);
	}
#elif defined(AW)
	DWORD weaponDef = ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.GetWeaponInfo))(dwWeaponIndex, 0);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x784);
	}
#endif
	return 0;
}


DWORD GetWeaponKillIcon_Internal(DWORD dwWeaponIndex) {
#if defined(WAW)
	int weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x610);
	}
#elif defined(MW1)
	int weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x518);
	}
#elif defined(BO1)
	int weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x578);
	}
#elif defined(BO2)
	int weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x660);
	}
#elif defined(MW2)
	int weaponDef = ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.GetWeaponInfo))(dwWeaponIndex); //bg_getweaponvariantdef
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x48);
	}
#elif defined(MW3)
	int weaponDef = GameFuncs::GetWeaponDef(dwWeaponIndex, true);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x84);
	}
#elif defined(GHOSTS)
	int weaponDef = ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.GetWeaponInfo))(dwWeaponIndex, 0);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x84);
	}
#elif defined(AW)
	int weaponDef = ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.GetWeaponInfo))(dwWeaponIndex, 0);
	if (weaponDef) {
		return *(DWORD*)(weaponDef + 0x778);
	}
#endif
	return 0;
}

DWORD ESP::GetWeaponKillIcon(DWORD dwWeaponIndex, FLOAT *fAspectW, FLOAT *fAspectH) {
	DWORD dwWeaponKillIcon = GetWeaponKillIcon_Internal(dwWeaponIndex);
	if (dwWeaponKillIcon) {
		DWORD dwKillIconType = GetKillIconType(dwWeaponIndex);
		GetIconAspectRatio(dwKillIconType, fAspectW, fAspectH);
		return dwWeaponKillIcon;
	}
	return 0;
}

VOID ESP::MakeLines(vec3_t Origin, FLOAT X1, FLOAT Y1, FLOAT Z1, FLOAT X2, FLOAT Y2, FLOAT Z2, FLOAT *Color, int Size = 1) {
	vec2_t Screen1, Screen2;
	vec3_t Origin1(Origin.x + X1, Origin.y + Y1, (Origin.z + Z1) - 4);
	vec3_t Origin2(Origin.x + X2, Origin.y + Y2, (Origin.z + Z2) - 4);
	if (GameFuncs::WorldToScreen(&Origin1, &Screen1.x, &Screen1.y) && GameFuncs::WorldToScreen(&Origin2, &Screen2.x, &Screen2.y)) {
		GameFuncs::DrawLine(Screen1.x, Screen1.y, Screen2.x, Screen2.y, Size, Color, CG::dwWhite);
	}
}

VOID ESP::DrawCornerBox(DWORD dwClient, vec2_t vBottomPos, vec2_t vCenterPos, vec2_t vTopPos) {
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBoxes == ESP_ENEMY || CG::dwESPBoxes == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBoxes == ESP_FRIENDLY || CG::dwESPBoxes == ESP_BOTH)) {
		
		// Grab the esp color
		FLOAT *pColor = GetESPColor(dwClient);

		FLOAT fHeight, fWidth;
		if (CG::cEntities.List[dwClient].bPlayer || (!CG::bIsMultiplayer && CG::cEntities.List[dwClient].bAI)) {
			fHeight = abs(vBottomPos.y - vTopPos.y);
			fWidth = fHeight / 2.0f;
		}
		else {
			fHeight = abs(vBottomPos.y - vCenterPos.y);
			fWidth = fHeight;
			vTopPos = vCenterPos;
		}

		GameFuncs::DrawRotatedPic((vBottomPos.x - (fWidth / 2)), (vBottomPos.y - fHeight), (fWidth / 4), 1, 0, pColor, CG::dwWhite); // top left line
		GameFuncs::DrawRotatedPic((vBottomPos.x - (fWidth / 2)), vBottomPos.y, (fWidth / 4), 1, 0, pColor, CG::dwWhite); // bottom left line
		GameFuncs::DrawRotatedPic((vBottomPos.x - (fWidth / 2)), (vBottomPos.y - fHeight), 1, (fHeight / 4), 0, pColor, CG::dwWhite); // left top line
		GameFuncs::DrawRotatedPic((vBottomPos.x + (fWidth / 2)), (vBottomPos.y - fHeight), 1, (fHeight / 4), 0, pColor, CG::dwWhite); // right top line

		GameFuncs::DrawRotatedPic((vBottomPos.x + ((fWidth / 2) - (fWidth / 4))), (vBottomPos.y - fHeight), (fWidth / 4), 1, 0, pColor, CG::dwWhite); // top right line
		GameFuncs::DrawRotatedPic((vBottomPos.x + ((fWidth / 2) - (fWidth / 4))), vBottomPos.y, (fWidth / 4), 1, 0, pColor, CG::dwWhite); // bottom right line
		GameFuncs::DrawRotatedPic((vBottomPos.x - (fWidth / 2)), (vBottomPos.y - (fHeight / 4)), 1, (fHeight / 4), 0, pColor, CG::dwWhite); // left bottom line
		GameFuncs::DrawRotatedPic((vBottomPos.x + (fWidth / 2)), (vBottomPos.y - (fHeight / 4)), 1, (fHeight / 4), 0, pColor, CG::dwWhite); // right bottom line
	}
}

VOID ESP::Draw2DBox(DWORD dwClient, vec2_t vBottomPos, vec2_t vCenterPos, vec2_t vTopPos, BOOL bFill) {
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBoxes == ESP_ENEMY || CG::dwESPBoxes == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBoxes == ESP_FRIENDLY || CG::dwESPBoxes == ESP_BOTH)) {
		// Grab the esp color
		FLOAT *pColor = GetESPColor(dwClient);

		FLOAT fBoxHeight, fBoxWidth;
		if (CG::cEntities.List[dwClient].bPlayer || (!CG::bIsMultiplayer && CG::cEntities.List[dwClient].bAI)) {
			fBoxHeight = abs(vBottomPos.y - vTopPos.y);
			fBoxWidth = fBoxHeight / 2.0f;
		}
		else {
			fBoxHeight = abs(vBottomPos.y - vCenterPos.y);
			fBoxWidth = fBoxHeight;
			vTopPos = vCenterPos;
		}

		GameFuncs::DrawBox(vBottomPos.x - fBoxWidth / 2, vTopPos.y, fBoxWidth, fBoxHeight, 0.0f, bFill ? Colors::WhiteTrans : Colors::Transparent, pColor, 1.0f);
	}
}

VOID ESP::Draw3DBox(DWORD dwClient, BOOL bOverride, vec3_t vMins, vec3_t vMaxes) {
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBoxes == ESP_ENEMY || CG::dwESPBoxes == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBoxes == ESP_FRIENDLY || CG::dwESPBoxes == ESP_BOTH) ||
		bOverride) {
		// Grab the esp color
		FLOAT *pColor = GetESPColor(dwClient);

		vec3_t mins = CG::cEntities.List[dwClient].pExt->vMins, maxes = CG::cEntities.List[dwClient].pExt->vMaxes;
		if (bOverride) {
			mins = vMins;
			maxes = vMaxes;
		}

		// Setup our bounding box in its entirety
		vec3_t corner2(maxes.x, mins.y, mins.z), corner3(maxes.x, mins.y, maxes.z), corner4(mins.x, mins.y, maxes.z), corner5(mins.x, maxes.y, maxes.z), corner6(mins.x, maxes.y, mins.z), corner7(maxes.x, maxes.y, mins.z);
		FLOAT corner2X, corner2Y, corner3X, corner3Y, corner4X, corner4Y, corner5X, corner5Y, corner6X, corner6Y, corner7X, corner7Y, minsX, minsY, maxesX, maxesY;

		int checksPassed = 0;
		checksPassed += !GameFuncs::WorldToScreen(&corner2, &corner2X, &corner2Y);
		checksPassed += !GameFuncs::WorldToScreen(&corner3, &corner3X, &corner3Y);
		checksPassed += !GameFuncs::WorldToScreen(&corner4, &corner4X, &corner4Y);
		checksPassed += !GameFuncs::WorldToScreen(&corner5, &corner5X, &corner5Y);
		checksPassed += !GameFuncs::WorldToScreen(&corner6, &corner6X, &corner6Y);
		checksPassed += !GameFuncs::WorldToScreen(&corner7, &corner7X, &corner7Y);
		checksPassed += !GameFuncs::WorldToScreen(&mins, &minsX, &minsY);
		checksPassed += !GameFuncs::WorldToScreen(&maxes, &maxesX, &maxesY);

		if (checksPassed == 0) { // && !clients[client].isEntDog)
			//mins to 2, 4, and 6
			GameFuncs::DrawLine(minsX, minsY, corner2X, corner2Y, 1.0f, pColor, CG::dwWhite);
			GameFuncs::DrawLine(minsX, minsY, corner4X, corner4Y, 1.0f, pColor, CG::dwWhite);
			GameFuncs::DrawLine(minsX, minsY, corner6X, corner6Y, 1.0f, pColor, CG::dwWhite);
			//maxes to 3, 5, and 7
			GameFuncs::DrawLine(maxesX, maxesY, corner3X, corner3Y, 1.0f, pColor, CG::dwWhite);
			GameFuncs::DrawLine(maxesX, maxesY, corner5X, corner5Y, 1.0f, pColor, CG::dwWhite);
			GameFuncs::DrawLine(maxesX, maxesY, corner7X, corner7Y, 1.0f, pColor, CG::dwWhite);
			//from 2 to 3 and 7
			GameFuncs::DrawLine(corner2X, corner2Y, corner3X, corner3Y, 1.0f, pColor, CG::dwWhite);
			GameFuncs::DrawLine(corner2X, corner2Y, corner7X, corner7Y, 1.0f, pColor, CG::dwWhite);
			//from 4 to 3 and 5
			GameFuncs::DrawLine(corner4X, corner4Y, corner3X, corner3Y, 1.0f, pColor, CG::dwWhite);
			GameFuncs::DrawLine(corner4X, corner4Y, corner5X, corner5Y, 1.0f, pColor, CG::dwWhite);
			//from 6 to 5 and 7
			GameFuncs::DrawLine(corner6X, corner6Y, corner5X, corner5Y, 1.0f, pColor, CG::dwWhite);
			GameFuncs::DrawLine(corner6X, corner6Y, corner7X, corner7Y, 1.0f, pColor, CG::dwWhite);
		}
	}
}

VOID ESP::DrawPyramidESP(DWORD dwClient) {
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBoxes == ESP_ENEMY || CG::dwESPBoxes == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwESPBoxes == ESP_FRIENDLY || CG::dwESPBoxes == ESP_BOTH)) {
		// Grab the esp color
		FLOAT *pColor = GetESPColor(dwClient);

		vec3_t Center = CG::cEntities.List[dwClient].vLocation;

		FLOAT W = GetBoxWidth(CG::cEntities.List[dwClient].pExt->dwStance), H = GetBoxHeight(CG::cEntities.List[dwClient].pExt->dwStance);

		W *= 1.2;
		H *= 1.0;

		//bottom
		MakeLines(Center, -W, -W, 0, W, -W, 0, pColor, 1);
		MakeLines(Center, -W, -W, 0, -W, W, 0, pColor, 1);
		MakeLines(Center, W, W, 0, W, -W, 0, pColor, 1);
		MakeLines(Center, W, W, 0, -W, W, 0, pColor, 1);
		//middle
		MakeLines(Center, -W, -W, 0, 0, 0, H, pColor, 1);
		MakeLines(Center, -W, W, 0, 0, 0, H, pColor, 1);
		MakeLines(Center, W, -W, 0, 0, 0, H, pColor, 1);
		MakeLines(Center, W, W, 0, 0, 0, H, pColor, 1);
	}
}

VOID ESP::DrawWeaponESP(DWORD dwClient, vec2_t vTopPos, vec2_t vBottomPos) {
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwESPWeapon == ESP_ENEMY || CG::dwESPWeapon == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwESPWeapon == ESP_FRIENDLY || CG::dwESPWeapon == ESP_BOTH)) {
		// Don't draw weapon esp for non-player entities
		if (!CG::cEntities.List[dwClient].bPlayer || CG::cEntities.List[dwClient].bAI) {
			return;
		}

		// Grab the esp color
		FLOAT *pColor = GetESPColor(dwClient);
		FLOAT sWidth = 40.0f, sHeight = 40.0f;
		DWORD weaponShader = GetWeaponKillIcon(CG::cEntities.List[dwClient].pExt->dwCurrentWeapon, &sWidth, &sHeight);
		if (weaponShader) {
			FLOAT opaque[4] = { pColor[0], pColor[1], pColor[2], 1.0f };
			sWidth *= 0.5f;
			sHeight *= 0.5f;

			// If we're not drawing distance esp, subtract the size of the distance text to make our esp look nicer
			if (!(CG::dwESPDistance == ESP_BOTH || ((CG::dwESPDistance == ESP_FRIENDLY && !CG::cEntities.List[dwClient].bEnemy) || (CG::dwESPDistance == ESP_ENEMY && CG::cEntities.List[dwClient].bEnemy)))) {
				vBottomPos.y -= CG::fMenuFontHeight * fDistScale;
			}
			// Draw the weapon material
			GameFuncs::DrawRotatedPic(vBottomPos.x - sWidth / 2.0f, vBottomPos.y + (fDistScale * CG::fMenuFontHeight), sWidth, sHeight, 0.0f, Colors::White, weaponShader);
		}
	}
}

VOID ESP::DrawDropESP(DWORD dwClient) {
	DWORD dwWeaponIndex = GameFuncs:: GetDropWeaponIndex(dwClient);
	if (dwWeaponIndex) {
		// Draw our explosive ESP
		if (CG::bDrawExplosives && CG::cEntities.List[dwClient].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_MISSILE) || ((CG::bDrawWeaponDrops || CG::bDrawScavPacks) && CG::cEntities.List[dwClient].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_ITEM))) {
			bool bIsScavPack = dwWeaponIndex == CG::dwScavPackWeaponID;
			// Draw our drop icon
			FLOAT fWidth = 80, fHeight = 40;
			DWORD dwKillIcon = bIsScavPack ? CG::dwScavPackMaterial : GetWeaponKillIcon(dwWeaponIndex, &fWidth, &fHeight);
			if (dwKillIcon) {
				if ((CG::cEntities.List[dwClient].sEntType != decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_MISSILE) && (bIsScavPack && CG::bDrawScavPacks) || (!bIsScavPack && CG::bDrawWeaponDrops)) || CG::cEntities.List[dwClient].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_MISSILE)) {
					// Only draw this ESP if the drop is close enough, so we don't spam their screen with drop ESP
					if (CG::cEntities.List[dwClient].fDistance / 100.0f < CG::fDropDrawDistance) {
						fWidth *= 0.7f;
						fHeight *= 0.7f;
						GameFuncs::DrawRotatedPic(CG::cEntities.List[dwClient].vScreen.x - fWidth / 2, CG::cEntities.List[dwClient].vScreen.y - fHeight / 2, fWidth, fHeight, 0.0f, Colors::White, dwKillIcon);
					}
				}
			}
		}
	}
}

#if defined(AW)
/* The grenade pointer function draws retarded looking arrows in AW.. so lets fix this */
VOID ESP::SetupPointersAW(BOOL bSetup) {
	if (bSetup) {
		*(SHORT*)decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowMaterialOffset) = 0x0988;

		// Change the size of the icon
		(*(FLOAT*)(*(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowHeightOffset)) + 0x0C)) = 18.0; // Height
		(*(FLOAT*)(*(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowWidthOffset)) + 0x0C)) = 32.5f; // Width
		(*(FLOAT*)(*(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowScaleOffset)) + 0x0C)) = 18.0f; // Scale
		(*(FLOAT*)(*(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowRadiusOffset)) + 0x0C)) = 20.0f; // Radius
	}
	else {
		// Change the material to the original
		*(SHORT*)decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowMaterialOffset) = 0xD010;

		// Reset icon size to default
		(*(FLOAT*)(*(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowHeightOffset)) + 0x0C)) = 36.0f; // Height
		(*(FLOAT*)(*(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowWidthOffset)) + 0x0C)) = 75.0f; // Width
		(*(FLOAT*)(*(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowScaleOffset)) + 0x0C)) = 36.0f; // Scale
		(*(FLOAT*)(*(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ArrowRadiusOffset)) + 0x0C)) = 50.0f; // Radius
	}
}
#endif

VOID ESP::DrawDirectionalPointer(DWORD dwClient) {
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwDirectionalArrows == ESP_ENEMY || CG::dwDirectionalArrows == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwDirectionalArrows == ESP_FRIENDLY || CG::dwDirectionalArrows == ESP_BOTH)) {
		// Grab the esp color
		FLOAT *pColor = GetESPColor(dwClient);

#if defined(AW)
		SetupPointersAW(TRUE);
#endif

		vec3_t *pLocalOrigin = GameFuncs::GetRefDefViewOrigin();
		if (pLocalOrigin) {
			vec3_t vPos(CG::cEntities.List[dwClient].vLocation - *pLocalOrigin);
		#if defined(BO3)
			((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_DrawGrenadePointer))(0, (FLOAT)NiNJA.dwScreenCenterX, (FLOAT)NiNJA.dwScreenCenterY, &vPos, pColor, 1.0f);
		#else
			((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_DrawGrenadePointer))(0, (FLOAT)NiNJA.dwScreenCenterX, (FLOAT)NiNJA.dwScreenCenterY, &vPos, pColor, 0.0f, 1.0f);
		#endif

		#if defined(AW)
			SetupPointersAW(FALSE);
		#endif
		}
	}
}

VOID ESP::DrawSnapline(DWORD dwClient, vec2_t vBottomPos, vec2_t vTopPos) {
	if (CG::cEntities.List[dwClient].bEnemy && (CG::dwSnaplines == ESP_ENEMY || CG::dwSnaplines == ESP_BOTH) ||
		!CG::cEntities.List[dwClient].bEnemy && (CG::dwSnaplines == ESP_FRIENDLY || CG::dwSnaplines == ESP_BOTH)) {
		FLOAT *pColor = GetESPColor(dwClient);
		switch (CG::dwSnaplineType) {
			case SNAPLINES_TOP:
				GameFuncs::DrawLine(NiNJA.dwScreenCenterX, 0.0f, vTopPos.x, vTopPos.y, 1.0f, pColor, CG::dwWhite);
				break;
			case SNAPLINES_CENTER:
				GameFuncs::DrawLine(NiNJA.dwScreenCenterX, NiNJA.dwScreenCenterY, vBottomPos.x, vBottomPos.y + (vTopPos.y - vBottomPos.y)/2, 1.0f, pColor, CG::dwWhite);
				break;
			case SNAPLINES_BOTTOM:
				GameFuncs::DrawLine(NiNJA.dwScreenCenterX, NiNJA.dwScreenMaxY, vBottomPos.x, vBottomPos.y, 1.0f, pColor, CG::dwWhite);
				break;
		}
	}
}

VOID ESP::DrawCustomCrosshair() {
	if (CG::dwCrosshairType != CROSSHAIR_OFF) {
		float x = (float)NiNJA.dwScreenCenterX, y = (float)NiNJA.dwScreenCenterY;
	/*	switch (CG::dwCrosshairType) {
			case CROSSHAIR_CIRCLE:
			case CROSSHAIR_PLUS:
				GameFuncs::DrawLine(NiNJA.dwScreenCenterX - 10, NiNJA.dwScreenCenterY - 1, NiNJA.dwScreenCenterX + 10, NiNJA.dwScreenCenterY - 1, 2, Colors::White, CG::dwWhite);
				GameFuncs::DrawLine(NiNJA.dwScreenCenterX, NiNJA.dwScreenCenterY - 1 - 10, NiNJA.dwScreenCenterX, NiNJA.dwScreenCenterY - 1 + 10, 2, Colors::White, CG::dwWhite);
				if (CG::dwCrosshairType != CROSSHAIR_CIRCLE) {
					break;
				}
			case CROSSHAIR_X:
				GameFuncs::DrawLine(NiNJA.dwScreenCenterX - 8, NiNJA.dwScreenCenterY - 1 - 8, NiNJA.dwScreenCenterX + 9, NiNJA.dwScreenCenterY - 1 + 8, 2, Colors::White, CG::dwWhite);
				GameFuncs::DrawLine(NiNJA.dwScreenCenterX - 8, NiNJA.dwScreenCenterY - 1 + 8, NiNJA.dwScreenCenterX + 9, NiNJA.dwScreenCenterY - 1 - 8, 2, Colors::White, CG::dwWhite);
				break;
		}*/
		FLOAT fLength = 24.0;
	#if defined(MW1) || defined(MW2) || defined(MW3)
		y -= 2.0f;
	#elif defined(AW)|| defined(GHOSTS) || defined(WAW)
		x += 1.0f;
		y -= 1.0f;
	#endif
		switch (CG::dwCrosshairType) {
			case CROSSHAIR_CIRCLE:
			case CROSSHAIR_PLUS:
				GameFuncs::DrawLine(x - fLength/2 - 1.0f, y, x + fLength/2, y, 2.0f, Colors::White, CG::dwWhite); //left to right
				GameFuncs::DrawLine(x - 1.0f, y - fLength / 2 - 1.0f, x - 1.0f, y + fLength / 2, 2.0f, Colors::White, CG::dwWhite); //top to bottom
				if (CG::dwCrosshairType != CROSSHAIR_CIRCLE) {
					break;
				}
			case CROSSHAIR_X:
			#if defined(AW) || defined(GHOSTS) || defined(WAW) || defined(MW1) || defined(MW2) || defined(MW3)
				x -= 1.0f;
			#endif
				GameFuncs::DrawLine(x, y, x + 9.0f, y + 9.0f, 2.0f, Colors::White, CG::dwWhite); //center to bottom right
				GameFuncs::DrawLine(x, y, x - 8.0f, y + 8.0f, 2.0f, Colors::White, CG::dwWhite); //center to bottom left
				GameFuncs::DrawLine(x, y, x + 9.0f, y - 9.0f, 2.0f, Colors::White, CG::dwWhite); //center to top right
				GameFuncs::DrawLine(x, y, x - 8.0f, y - 8.0f, 2.0f, Colors::White, CG::dwWhite); //center to top left
				break;
		}
	}
}

FLOAT ESP::ScaleRadarX(FLOAT X, FLOAT ScreenWidth) {
	return (X * (640.0f / ScreenWidth));
}

FLOAT ESP::ScaleRadarY(FLOAT Y, FLOAT ScreenHeight) {
	return (Y * (480.0f / ScreenHeight));
}

VOID ESP::GetRadarXY(FLOAT *fX, FLOAT *fY) {
#if defined(BO2)
	*fX = ScaleRadarX(12, NiNJA.dwScreenMaxX);
	*fY = ScaleRadarY(12, NiNJA.dwScreenMaxY);
#else
	*fX = lastCompass.x;
	*fY = lastCompass.y;
#endif
}

VOID ESP::GetRectDef(FLOAT radarX, FLOAT radarY, CRectDef *rectDef) {
	if (lastCompassType == 0) {
		rectDef->x = radarX;
		rectDef->y = radarY;
		rectDef->w = 102;
		rectDef->h = 102;
		rectDef->horizAlign = 0x08080000;
	#if defined(GHOSTS) || defined(AW)
		rectDef->horizAlign = 0x0c0dea70;
	#elif defined(MW1) || defined(WAW)
		rectDef->horizAlign = 0x1;
	#endif
	}
	else if(!CG::bIsMapSelector) {
		rectDef->x = radarX;
		rectDef->y = radarY;
		rectDef->w = 200;
		rectDef->h = 300;
		rectDef->horizAlign = 0x03010000;
	#if defined(MW1) || defined(WAW) || defined(BO1)
		rectDef->horizAlign = 0x3;
		rectDef->vertAlign = 0x1;
	#elif defined(GHOSTS)|| defined(AW)
		rectDef->horizAlign = 0x0C0DEF20;
	#endif
	}

#if !defined(BO2)
	rectDef->x = lastCompass.x;
	rectDef->y = lastCompass.y;
	rectDef->w = lastCompass.w;
	rectDef->h = lastCompass.h;
#endif
}

DWORD ESP::GetCGForCompass() {
#if defined(MW3)
	return *(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.UnknownClientStruct);
#else
	return *(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_t);
#endif
}

VOID ESP::CompassUpYawVector(vec2_t& Yaw) {
	DWORD cg_t = GetCGForCompass();
	if (cg_t) {
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_CompassUpYawVector))(cg_t, &Yaw);
	}
}

VOID ESP::CalcCompassDimensions(DWORD CompassType, vec2_t& size, DWORD rectDef, vec4_t& radarSize) {
	DWORD cg_t = GetCGForCompass();
	if (cg_t) {
		((BOOL(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_CompassCalcDimensions))(CompassType, cg_t, &size, rectDef, &radarSize.x, &radarSize.y, &radarSize.z, &radarSize.a);
	}
}

BOOL ESP::WorldPosToCompass(DWORD CompassType, vec4_t& RadarSize, vec2_t& Yaw, vec3_t *refOrigin, vec3_t *worldPos, vec2_t& outClipped) {
	DWORD cg_t = GetCGForCompass();
	if (cg_t) {
		vec3_t vOut;
	#if defined(AW)
		return ((BOOL(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_WorldPosToCompass))(0, CompassType, cg_t, &RadarSize, &Yaw, refOrigin, worldPos, &vOut, &outClipped);
	#else
		return ((BOOL(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_WorldPosToCompass))(CompassType, cg_t, &RadarSize, &Yaw, refOrigin, worldPos, &vOut, &outClipped);
	#endif
	}
	return FALSE;
}

VOID ESP::WorldToCompass(vec3_t *World, vec2_t *Compass) {
	vec4_t RadarSize;
	FLOAT radarX = 0, radarY = 0;
	GetRadarXY(&radarX, &radarY);

	CRectDef rectDef;
	GetRectDef(radarX, radarY, &rectDef);

	vec2_t size(radarX, radarY);
	CalcCompassDimensions(lastCompassType, size, (DWORD)&rectDef, RadarSize);

	FLOAT x2 = (RadarSize.z * 0.5) + RadarSize.x;
	FLOAT y2 = (RadarSize.a * 0.5) + RadarSize.y;

	vec2_t Yaw(0, 0);
	CompassUpYawVector(Yaw);

	vec2_t outClipped(0, 0);
	vec3_t myOrigin = GameFuncs::GetLocalViewOrigin();
	WorldPosToCompass(lastCompassType, RadarSize, Yaw, &myOrigin, World, outClipped);

	vec2_t widthHeight(16, 16);
	Compass->x = -((widthHeight.x * 0.5) - x2) + outClipped.x;
	Compass->y = -((widthHeight.y * 0.5) - y2) + outClipped.y;

	if (lastCompassType == 0 && !CG::bIsMapSelector) {
	#if defined(MW1) || defined(WAW)
		Compass->x -= 2.0f;
		Compass->y -= 2.0f;
	#elif defined(AW)
		//DWORD cg_t = GameFuncs::Get_CG_t();
		Compass->x += 5.0f;// +cg_t ? *(FLOAT*)(cg_t + 0x84) / 4 : 0.0f;
		Compass->y += 2.0f;// +cg_t ? -*(FLOAT*)(cg_t + 0x88) / 4 : 0.0f;
	#elif defined(MW2) || defined(MW3) || defined(GHOSTS)
		Compass->x -= 2.0f;
		Compass->y -= 3.0f;
	#elif defined(BO1)
		Compass->x -= 3.0f;
		Compass->y -= 3.0f;
	#elif defined(BO2)
		Compass->y -= 5.0f;
	#endif
	}
	else if (!CG::bIsMapSelector) {
	#if defined(BO1)
		Compass->x += 251.0f;
		Compass->y -= 100.0f;
	#elif defined(MW1) ||defined(MW2) || defined(WAW) 
		Compass->x += 258.0f;
		Compass->y -= 138.0f;
	#elif defined(MW3)
		Compass->x += 270.0f;
		Compass->y -= 100.0f;
	#elif defined(AW)
		Compass->x -= 386.0f;
		Compass->y -= 49.0f;
	#elif defined(GHOSTS)
		Compass->x -= 462.0f;
		Compass->y -= 176.0f;
	#elif defined(BO2)
		Compass->x += 70.0f;
		Compass->y -= 217.0f;
	#endif
		//printf(GameFuncs::va("Compass x, y: %f, %f\n", Compass->x, Compass->y));
	}
	else {
	#if defined(MW1) || defined(WAW) || defined(BO1)
		Compass->x -= 2.0f;
		Compass->y -= 2.0f;
	#elif defined(MW2)
		Compass->x += 202.0f;
		Compass->y += 202.0f;
	#elif defined(MW3)
		Compass->x += 172.0f;
		Compass->y += 202.0f;
	#elif defined(AW)
		Compass->x -= 2.0f;
		Compass->y -= 2.0f;
	#endif
		//printf(GameFuncs::va("Compass x, y: %f, %f\n", Compass->x, Compass->y));
	}

}

VOID ESP::DrawRadarPic(DWORD dwRadarType, FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT rotation, const FLOAT *color, DWORD shader) {
	DWORD horizAlign = 0, vertAlign = 0;
	if (dwRadarType == 0 && !CG::bIsMapSelector) {
	#if defined(WAW) || defined(MW1)
		horizAlign = 0x1;
		vertAlign = 0x1;
	#elif defined(GHOSTS) || defined(AW)
		horizAlign = 0xc;
		vertAlign = 0xd;
	#else
		horizAlign = 0x8;
		vertAlign = 0x8;
	#endif
	}
	else if(!CG::bIsMapSelector) {
	#if defined(AW) || defined(GHOSTS)
		horizAlign = 0xc;
		vertAlign = 0xd;
	#elif defined(BO2)
		horizAlign = 0x2;
		vertAlign = 0x2;
	#else
		horizAlign = 0x3;
		vertAlign = 0x1;
	#endif
	}
	else {
	#if defined(BO1) || defined(MW2) || defined(MW3)
		horizAlign = 0x9;
		vertAlign = 0x9;
	#elif defined(WAW) || defined(MW1)
		horizAlign = 0x2;
		vertAlign = 0x2;
	#elif defined(AW)
		horizAlign = 0xc;
		vertAlign = 0xd;
	#elif defined(BO2)
		horizAlign = 0x2;
		vertAlign = 0x2;
#else
	#endif
	}
//#if defined(BO2)
//	if (dwRadarType == 1) {
//		GameFuncs::DrawRotatedPic(x, y, w, h, rotation, color, shader);
//		return;
//	}
//#endif
	GameFuncs::DrawHandlePic(x, y, w, h, horizAlign, vertAlign, rotation, color, shader);
}

VOID ESP::DrawRadarText(DWORD dwRadarType, PCHAR pText, FLOAT x, FLOAT y, FLOAT fScale, FLOAT rotation, const FLOAT *color, DWORD font) {
	DWORD horizAlign = 0, vertAlign = 0;
	if (!CG::bIsMapSelector && dwRadarType == 0) {
	#if defined(WAW) || defined(MW1)
		horizAlign = 0x1;
		vertAlign = 0x1;
	#elif defined(GHOSTS) || defined(AW)
		horizAlign = 0xc;
		vertAlign = 0xd;
	#else
		horizAlign = 0x8;
		vertAlign = 0x8;
	#endif
	}
	else {
	#if defined(AW) || defined(GHOSTS)
		horizAlign = 0xc;
		vertAlign = 0xd;
	#else
		horizAlign = 0x3;
		vertAlign = 0x1;
	#endif
	}
	GameFuncs::UI_DrawText(pText, x, y, fScale, horizAlign, vertAlign, rotation, color, font);
}

VOID ESP::DrawCustomRadar() {
	if (drawCompassTimer > NiNJA.dwTickCount) {
		if (CG::bRadar) {
		#if defined(BO2)
			if (lastCompassType == 1 && CG::bIsMapSelector) {
				goto jSkip;
			}
		#endif
			for (DWORD i = 0; i < __MAXENTITIES__; i++) {
				// Skip invalid entities
				if (!CG::cEntities.List[i].bValid) {
					continue;
				}

				// Skip dead entities
				if (!CG::cEntities.List[i].bAlive) {
					continue;
				}

				if (CG::cEntities.List[i].bPlayer || CG::cEntities.List[i].bAI) {
					continue;
				}

				if (CG::cEntities.List[i].sEntType != decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_ITEM) && CG::cEntities.List[i].sEntType != decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_MISSILE)) {
					continue;
				}

				DWORD dwWeaponIndex = GameFuncs::GetDropWeaponIndex(i);
				if (dwWeaponIndex != 0) {
					bool bIsScavPack = dwWeaponIndex == CG::dwScavPackWeaponID;
					// Draw our drop icon
					FLOAT fWidth = 80, fHeight = 40;
					DWORD dwKillIcon = bIsScavPack ? CG::dwScavPackMaterial : GetWeaponKillIcon(dwWeaponIndex, &fWidth, &fHeight);
					if((CG::cEntities.List[i].sEntType != decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_MISSILE) && ((bIsScavPack && CG::bDrawScavPacks) || (!bIsScavPack && CG::bDrawWeaponDrops))) || (CG::bDrawExplosives && CG::cEntities.List[i].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_MISSILE))) {
						vec2_t compass;
						WorldToCompass(&CG::cEntities.List[i].vLocation, &compass);
						DrawRadarPic(lastCompassType, compass.x, compass.y, fWidth/3.0f, fHeight/3.0f, 0.0f, Colors::White, dwKillIcon);
					}
				}
			}

			for (DWORD i = 0; i < __MAXCLIENTS__; i++) {
				// Skip invalid entities
				if (!CG::cEntities.List[i].bValid) {
					continue;
				}

				// Skip dead entities
				if (!CG::cEntities.List[i].bAlive) {
					continue;
				}

				// Skip ourselves
				if (CG::iLocalClient == i && lastCompassType == 0) {
					continue;
				}

				// If they have a valid location
				if (!CG::cEntities.List[i].vLocation.IsZero()) {
					FLOAT *pColor = GetESPColor(i);

					vec3_t viewAngles = GameFuncs::Get_RefDef_ViewAngles();
					vec3_t entityView = (i != CG::iLocalClient) ? CG::cEntities.List[i].vAngles : viewAngles;
					FLOAT yaw = entityView.y;

					FLOAT rotation = (360.0f - yaw);
					if (lastCompassType == 0 && !CG::bIsMapSelector) {
						rotation += viewAngles.y;
					}

					FLOAT w = 20.0, h = 20.0;
					vec2_t Compass;
					vec3_t loc = GameFuncs::GetEntityLocation(i);
					WorldToCompass(&loc, &Compass);
					DrawRadarPic(lastCompassType, Compass.x, Compass.y, w, h, rotation, pColor, CG::dwArrowIcon);
				}
			}
		#if defined(BO2)
			jSkip:
		#endif

			// Draw our own pointer at the end of this loop
			if (lastCompassType == 0) {
				vec2_t Compass;
				WorldToCompass(&CG::cEntities.List[CG::iLocalClient].vLocation, &Compass);
				FLOAT w = 20.0f, h = 20.0f;
				DrawRadarPic(lastCompassType, Compass.x, Compass.y, w, h, 0.0f, Colors::White, CG::dwArrowIcon);
			}
		}
	}
}

bool ESP::IsEntValid(DWORD dwEnt) {
	// Skip invalid entities
	if (!CG::cEntities.List[dwEnt].bValid) {
		return false;
	}

	// Skip dead entities
	if (!CG::cEntities.List[dwEnt].bAlive) {
		return false;
	}

	// Skip ourselves
	if (CG::iLocalClient == dwEnt) {
		return false;
	}

	// Skip invisible entities if we aren't in multiplayer
	if (CG::cEntities.List[dwEnt].bInvisible && !CG::cEntities.List[dwEnt].bPlayer && !CG::bIsMultiplayer) {
		return false;
	}
	return true;
}

VOID ESP::DrawClientESP(int i) {
	if (!IsEntValid(i)) {
		return;
	}


	// Check if the entity is onscreen
	if (!CG::cEntities.List[i].vLocation.IsZero()) {
		bool bOnScreen = GameFuncs::WorldToScreen(&CG::cEntities.List[i].vLocation, &CG::cEntities.List[i].vScreen.x, &CG::cEntities.List[i].vScreen.y) == TRUE;
		if (bOnScreen) {
			CG::cEntities.List[i].bOnScreen = CG::cEntities.List[i].vScreen.x >= 0 && CG::cEntities.List[i].vScreen.x < NiNJA.dwScreenMaxX;
			// Draw ESP for ExtendedEntity attributes
			if (CG::cEntities.List[i].pExt) {
				// Grab our top and bottom origin so we can keep our esp positions consistent
				vec3_t vBottom = CG::cEntities.List[i].vLocation, vTop = CG::cEntities.List[i].pExt->cTags[TAG_MAINROOT].vPos + vec3_t(0, 0, 30.0f);
				vec3_t vCenter = vec3_t((vTop.x + vBottom.x) / 2.0f, (vTop.y + vBottom.y) / 2.0f, (vTop.z + vBottom.z) / 2.0f);

				vec2_t vTopPos, vBottomPos, vCenterPos;
				if (GameFuncs::WorldToScreen(&vBottom, &vBottomPos.x, &vBottomPos.y) && GameFuncs::WorldToScreen(&vTop, &vTopPos.x, &vTopPos.y) && GameFuncs::WorldToScreen(&vCenter, &vCenterPos.x, &vCenterPos.y)) {
					// We want these to be the same so everything lines up nicely :)
					vTopPos.x = vBottomPos.x;
					vCenterPos.x = vBottomPos.x;

					// Draw bone ESP and snaplines
					DrawSnapline(i, vBottomPos, vTopPos);
					DrawBoneESP(i);

					//// Test line from the bottom of our player to the top
					//GameFuncs::DrawLine(vTopPos.x, vTopPos.y, vCenterPos.x, vCenterPos.y, 1.0f, Colors::Cyan, CG::dwWhite);
					//GameFuncs::DrawLine(vBottomPos.x, vBottomPos.y, vCenterPos.x, vCenterPos.y, 1.0f, Colors::Green, CG::dwWhite);

					// Draw our weapon ESP
					DrawWeaponESP(i, vTopPos, vBottomPos);

					// Handle our different ESP box types
					switch (CG::dwESPBoxType) {
					case BOX_TYPE_CORNERS:
						DrawCornerBox(i, vBottomPos, vCenterPos, vTopPos);
						break;
					case BOX_TYPE_2D:
					case BOX_TYPE_2D_FILLED:
						Draw2DBox(i, vBottomPos, vCenterPos, vTopPos, CG::dwESPBoxType == BOX_TYPE_2D_FILLED);
						break;
					case BOX_TYPE_3D:
						Draw3DBox(i);
						break;
						//case BOX_TYPE_PYRAMID:
						//	DrawPyramidESP(i);
						//	break;
					}

					// If the entity has a name, display it
					if (CG::cEntities.List[i].pExt->GetName()) {
						DrawNameESP(i, vTopPos, vCenterPos, vBottomPos);
					}

					// Draw our distance ESP
					DrawDistanceESP(i, vTopPos, vBottomPos);
				}
			}
			else {
				// Draw ESP for dropped weapons, scav packs, etc
				DrawDropESP(i);
			}
		}
		else {
			CG::cEntities.List[i].bOnScreen = false;
		}
		if (CG::cEntities.List[i].bPlayer || CG::cEntities.List[i].bAI) {
			// Draw our directional pointers for players and ai, we don't care if they're onscreen for this one
			DrawDirectionalPointer(i);
		}
	}
}

VOID ESP::Draw() {
	// Draw any entity who isn't a player first, so that actual players wind up being drawn on top of entities like dogs, zombies, etc
	for (DWORD i = __MAXCLIENTS__; i < __MAXENTITIES__; i++) {
		DrawClientESP(i);
	}

	// Next, draw our friendlies
	for (DWORD i = 0; i < __MAXCLIENTS__; i++) {
		if (!CG::cEntities.List[i].bEnemy) {
			DrawClientESP(i);
		}
	}

	// Finally, draw our enemies on top of everything else
	for (DWORD i = 0; i < __MAXCLIENTS__; i++) {
		if (CG::cEntities.List[i].bEnemy) {
			DrawClientESP(i);
		}
	}

	// Draw our custom crosshair
	DrawCustomCrosshair();

	// Draw our custom radar on top of everything else
	DrawCustomRadar();
}