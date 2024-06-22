#include "GameFuncs.h"
#include "Globals.h"
#include "SecureStrings.h"
#include "Menu.h"
#include "Autowall.h"
#include "HttpRequest.h"
#include <sstream>
#include <time.h>
#include "UserCmd.h"
#include <math.h>

CONST PCHAR(__cdecl *GameFuncs::va)(CONST PCHAR Format, ...);

bool GameFuncs::AllLocalClientsDisconnected() {
#ifndef BO3
#if defined(MW3)
	return *(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientUIActive_t) + 0x5c) <= 2;
#elif defined(MW2)
	return *(DWORD*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientUIActive_t) + 0xb48) <= 2;
#else
	return ((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_AllLocalClientsDisconnected))(0);
#endif
#else
	return FALSE;
#endif
}

VOID GameFuncs::CheckWhitelistBlacklist(int i) {
	// Check if they're whitelisted
	if (!CG::cEntities.List[i].pExt->bWhitelisted) {
		for (DWORD v = 0; v < CG::vWhitelisted.size(); v++) {
				if (CG::cEntities.List[i].pExt->cXuid == CG::vWhitelisted[v]) {
					CG::cEntities.List[i].pExt->bWhitelisted = true;
					break;
				}
			}
		}

	// Check if they're prioritized
	if (!CG::cEntities.List[i].pExt->bPreferred) {
		for (DWORD v = 0; v < CG::vBlacklisted.size(); v++) {
			if (CG::cEntities.List[i].pExt->cXuid == CG::vBlacklisted[v]) {
				CG::cEntities.List[i].pExt->bPreferred = true;
				break;
			}
		}
	}
}

VOID GameFuncs::BulletPenetrate(PVOID bulletFireParams, DWORD weaponIndex, DWORD centity_t, vec3_t *tracerStart, PVOID bulletTraceResults) {
	if (centity_t) {
	#if defined(MW1) || defined(WAW)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.FireBulletPenetrate))(0, bulletFireParams, GetWeaponDef(weaponIndex), centity_t, tracerStart, false);
	#elif defined(BO1) || defined(BO2)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.FireBulletPenetrate))(0, bulletFireParams, weaponIndex, (void*)centity_t, tracerStart, false, false, bulletTraceResults);
	#elif defined(MW2)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.FireBulletPenetrate))(0, bulletFireParams, weaponIndex, (void*)centity_t, 0xffffffff, tracerStart, false);
	#elif defined(MW3) || defined(GHOSTS)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.FireBulletPenetrate))(0, bulletFireParams, weaponIndex, 0, (void*)centity_t, 0xffffffff, tracerStart, false);
	#elif defined(AW)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.FireBulletPenetrate))(0, bulletFireParams, weaponIndex, 0, 0, (void*)centity_t, 0xffffffff, tracerStart, false);
	#elif defined(BO3)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.FireBulletPenetrate))(0, bulletFireParams, weaponIndex, 0, tracerStart, 0, bulletTraceResults);
	#endif
	}
}

bool GameFuncs::BulletTrace(DWORD dwAttacker, PVOID pBulletFireParams, PVOID pBulletTraceResults, DWORD dwSurfaceType) {
#if defined(MW2) || defined(MW3) || defined(BO2) || defined(GHOSTS)
 	return ((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BulletTrace))(0, pBulletFireParams, CG::cEntities.List[dwAttacker].pCentity, pBulletTraceResults, dwSurfaceType);
#endif
	return false;
}

DWORD GameFuncs::CacheFont(CHAR *font) {
	if (font) {
	#if defined(AW)
		return ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.DB_FindXAssetHeader))(0x1c, font, 0x1);
	#elif defined(MW1) || defined(BO3)
		return ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.DB_FindXAssetHeader))(0x14, font, 0x1);
	#else
		return ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_RegisterFont))(font, decryptDWORD(Not(NiNJA.cPatches)->CoD.ImageTrack));
	#endif
	}
	DbgPrintE("Failed to find font %s! Returning SmallFont", font);
	return CacheFont(__SMALLFONT__);
}

DWORD GameFuncs::CacheMaterial(PCHAR pMaterialName) {
	if (pMaterialName) {
	#if defined(AW)
		return ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.DB_FindXAssetHeader))(0x5, pMaterialName, 0x1);
	#elif defined(BO3) 
		return ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.DB_FindXAssetHeader))(0x6, pMaterialName, 0x1);
	#elif defined(MW1)
		return ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.DB_FindXAssetHeader))(0x4, pMaterialName, 0x1);
	#else
		return ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.Material_RegisterHandle))(pMaterialName, 7);
	#endif
	}
	// Always return white if an invalid material name was specified
	return CacheMaterial("white");
}

bool GameFuncs::EnterCustomGamertag(PVOID menuOption) {
	MenuOption *pOption = (MenuOption*)menuOption;
	if (pOption) {
		// Setup our variables, and clear everything out before we begin
		XOVERLAPPED xOvl;
		WCHAR wResult[33], wInitialText[33];
		wmemset(wResult, 0, 33);
		wmemset(wInitialText, 0, 33);
		ZeroMemory(&xOvl, sizeof(XOVERLAPPED));

		if (!Compare((PCHAR)pOption->pSyncVariable, __EMPTYGTSTRING__)) {
			// Convert to a wchar_t*
			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, wInitialText, strlen((PCHAR)pOption->pSyncVariable) + 1, (PCHAR)pOption->pSyncVariable, _TRUNCATE);
		}

		// Launch the virtual keyboard UI
		XShowKeyboardUI(0, VKBD_DEFAULT | VKBD_HIGHLIGHT_TEXT, wInitialText, L"Custom GT Entry", L"Type in a new gamertag! The max length is 32 characters.", wResult, 33, &xOvl);

		// Wait for the user to close the keyboard
		while (!XHasOverlappedIoCompleted(&xOvl)) {
			Sleep(33);
		}

		// If the user pressed "done" on the keyboard window
		if (xOvl.dwExtendedError == ERROR_SUCCESS) {
			CHAR gamertag[33];
			wcstombs(gamertag, wResult, 33);
			if (strlen(gamertag) <= 32) {
				memcpy((PVOID)pOption->pSyncVariable, gamertag, 32);
				return true;
			}
		}
	}
	return false;
}

bool GameFuncs::IsEntUsingShield(DWORD dwEntIndex, DWORD dwWeaponIndex) {
#if !defined(WAW) && !defined(MW1) && !defined(BO1) && !defined(BO3)
	DWORD i = 0;
#if defined(AW)
	while (i < 8) {
#endif
	#if !defined(GHOSTS)
		if (CG::ShieldCache[i] == dwWeaponIndex) {
			return true;
		}
	#else
		CHAR PlayerWeaponName[100];
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.GetWeaponHudName))(dwWeaponIndex, 0, PlayerWeaponName, 100);
		//DbgPrintE("WeaponName: %s", PlayerWeaponName);
		return Contains(PlayerWeaponName, "Riot Shield");
	#endif
#if defined(AW)
		i++;
	}
	if (*(SHORT*)(CG::cEntities.List[dwEntIndex].pCentity + 0xca) == CG::ShieldCache[7]) {
		return true;
	}
#endif
#endif
	return false;
}

bool GameFuncs::IsMantling() {
	DWORD cg_t = Get_UnknownClientStruct();
	if (!cg_t) {
		cg_t = Get_CG_t();
	}

	if (cg_t && CG::iLocalClient >= 0 && CG::iLocalClient < __MAXCLIENTS__) {
	#if defined(MW1) || defined(BO1) || defined(WAW)
		return *(BYTE*)(cg_t + 0xF) == 0x4 || (*(BYTE*)(cg_t + 0xF) & 0x8) != 0;
	#elif defined(BO2) 
		int iWeaponIndex = ((int(*)(...))0x826B40D8)(cg_t);
		return *(BYTE*)(cg_t + 0x18F) == 0x8 || iWeaponIndex == 0;
	#elif defined(MW2)
		return (*(BYTE*)(cg_t + 0xb1) & 1) != 0 || (*(BYTE*)(cg_t + 0xF) & 0x8) != 0;
	#elif defined(GHOSTS)
		return *(BYTE*)(CG::cEntities.List[CG::iLocalClient].pCentity + 0x5d) == 2 || *(BYTE*)(CG::cEntities.List[CG::iLocalClient].pCentity + 0xab) == 0xc0;
	#endif
	}
	return false;
}


DWORD GameFuncs::CacheTag(PCHAR pName) {
#if defined(AW)
	return ((unsigned int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.SL_GetString))(pName, 1, strlen(pName) + 1, 8);
#elif defined(BO1)
	return ((unsigned int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.SL_GetString))(0, pName, 1, strlen(pName) + 1);
#elif defined(BO3)
	return ((unsigned int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.SL_GetString))(pName, 1, strlen(pName) + 1);
#elif defined(WAW)
	return ((unsigned int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.SL_GetString))(0, pName, 0);
#else
	return ((unsigned int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.SL_GetString))(pName, 0);
#endif
}

VOID GameFuncs::CalcScreenDimensions() {
#if defined(BO3)
	int screenInfo = decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_GetScreenDimensions);
	NiNJA.dwScreenMaxX = *(DWORD*)(screenInfo + 0x84);
	NiNJA.dwScreenMaxY = *(DWORD*)(screenInfo + 0x88);
#elif defined(MW1)
	int screenInfo = decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_GetScreenDimensions);
	NiNJA.dwScreenMaxX = *(DWORD*)(screenInfo + 0xea0);
	NiNJA.dwScreenMaxY = *(DWORD*)(screenInfo + 0xea4);
#else
	int aspect;
	((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_GetScreenDimensions))(&NiNJA.dwScreenMaxX, &NiNJA.dwScreenMaxY, &aspect);
#endif
	NiNJA.dwScreenCenterX = NiNJA.dwScreenMaxX / 2;
	NiNJA.dwScreenCenterY = NiNJA.dwScreenMaxY / 2;

	DbgPrintE("ScreenSize: %ix%i (%i, %i)", NiNJA.dwScreenMaxX, NiNJA.dwScreenMaxY, NiNJA.dwScreenCenterX, NiNJA.dwScreenCenterY);
}

VOID GameFuncs::ChangeClass(PVOID pOption) {
	if (pOption) {
		MenuOption *pMenuOption = (MenuOption*)pOption;
		int iClassNum = pMenuOption->dwState + 1;
		char *str;
		int serverId = *(int*)decryptDWORD(Not(NiNJA.cPatches)->CoD.ServerIdAddress);
	#if defined(MW3)
		str = va(ES("cmd mr %i 10 custom%i"), serverId, iClassNum);
	#elif defined(MW2)
		str = va(ES("cmd mr %i 10 custom%i"), serverId, iClassNum);
	#elif defined(WAW)
		str = va(ES("cmd mr %i 11 custom%i"), serverId, iClassNum);
	#elif defined(GHOSTS)
		str = va(ES("cmd lui 5 %i %i"), iClassNum - 1, serverId);
	#else
		return;
	#endif
		if (str) {
			SendConsoleCommand(str);
		}
	}
}

VOID GameFuncs::JuggClass() {
	int serverId = *(int*)decryptDWORD(Not(NiNJA.cPatches)->CoD.ServerIdAddress);
	SendConsoleCommand(va(ES("cmd mr %i 10 axis"), serverId));
}

VOID GameFuncs::ChangeTeam() {
	if (CG::bInGameCached && CG::bLobbyInitialized) {
		if (CG::iRealLocalClient >= 0 && CG::iRealLocalClient < __MAXCLIENTS__) {
			if (CG::cEntities.List[CG::iRealLocalClient].bAlive) {
				char *changeTeamString = NULL;
				int iCurrentTeam = CG::cEntities.List[CG::iRealLocalClient].sTeam;
				int serverId = *(int*)decryptDWORD(Not(NiNJA.cPatches)->CoD.ServerIdAddress);
				int switchTo = (iCurrentTeam == 1) ? 2 : 1;
				if (CG::bInvertTeams) {
					switchTo = (switchTo == 1) ? 2 : 1;
				}
				CG::dwExpectedTeam = switchTo;
			#if defined(MW1) || defined(WAW) || defined(BO1)
				changeTeamString = va(ES("cmd mr %i 4 %s"), serverId, teamNames[switchTo]);
			#elif defined(MW2) || defined(MW3)
				changeTeamString = va(ES("cmd mr %i 3 %s"), serverId, teamNames[switchTo]);
			#elif defined(BO2)
				changeTeamString = va(ES("cmd mr %i 5 %s"), serverId, teamNames[iCurrentTeam]);
			#elif defined(GHOSTS)
				changeTeamString = va(ES("cmd lui 68 %i %i"), switchTo - 1, serverId);
			#elif defined(AW)
				changeTeamString = va(ES("cmd lui 125 %i %i"), switchTo - 1, serverId);
			#elif defined(BO3)
				changeTeamString = va(ES("cmd mr %i 1 %s"), serverId, teamNames[iCurrentTeam]);
			#endif
				if (changeTeamString) {
					SendConsoleCommand(changeTeamString);
					CG::bBrokenTeamCheck = true;
					CG::dwTeamCheckTime = GetTickCount();
				}
			}
		}
	}
}

bool GameFuncs::CheckIsMigrating() {
#if defined(MW2)
	return *(int*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientUIActive_t) + 0x8) != 0;
#elif defined(MW3) || defined(GHOSTS) || defined(AW)
	return *(int*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientUIActive_t) + 0x4) != 0;
#elif defined(BO2) || defined(BO1)
	return *(int*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientUIActive_t) + 0x10) != 0;
#endif
	return false;
}

PCHAR pInGame = NULL;
bool GameFuncs::CheckInGame_Internal() {
#if defined(MW1) || defined(WAW) ||  defined(BO1) || defined(BO2)
	return Dvar_GetBool(decryptDWORD(Not(NiNJA.cPatches)->CoD.Dvar_GetBool));
#elif defined(BO3)
	return ((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.Dvar_GetBool))(0);
#else
	if (!pInGame) {
		pInGame = EA("cl_ingame");
	}
	return ((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.Dvar_GetBool))(pInGame);
#endif
}

bool GameFuncs::CheckInGame() {
	// Make sure we are connected to a game session
	CG::bConnected = !AllLocalClientsDisconnected() ? TRUE : FALSE;

	// Check the cl_ingame dvar
	bool bInGame = CheckInGame_Internal();
	if (!bInGame) {
		if (CG::bQuittingGame) {
			CG::bQuittingGame = FALSE;
		}
	}

	// If we're in game and all local clients aren't disconnected, we're fully ingame.
	return bInGame && CG::bConnected && !GameFuncs::CheckIsMigrating() && !CG::bQuittingGame;
}

vector<PCHAR> GetWebDirectoryListing(PCHAR pServer, string pPath) {
	// We have a directory to read some files from 
	vector<PCHAR> pListing;
	HttpRequest cPackRequest;
	BOOL bSuccess = FALSE;
	if (!bIgnoreHttpRequests) {
		PBYTE pPackData = cPackRequest.SendRequest(pServer, pPath, FALSE, &bSuccess, 80, 10000);
		if (bSuccess) {
			PCHAR pCurPack = (PCHAR)pPackData;
		jPackRestart:
			PCHAR pPackLine = strstr((PCHAR)pCurPack, "<a href=");
			if (pPackLine) {
				PCHAR pPackName = strstr(pPackLine, ">");
				if (pPackName) {
					pPackName += 1;
					PCHAR pPackNameEnd = strstr(pPackName, "<");
					if (pPackNameEnd) {
						*pPackNameEnd = 0;
						if (Compare(pPackName, "../")) {
							pCurPack = pPackName;
							*pPackNameEnd = '<';
							goto jPackRestart;
						}
						else {
							PCHAR tmpPtr = NULL;
							sManager.alloc(strlen(pPackName) + 1, &tmpPtr);
							if (tmpPtr) {
								strcpy(tmpPtr, pPackName);
								pListing.push_back(tmpPtr);
							}
						}

						//DbgPrint("Listing: %s", pPackName);
						*pPackNameEnd = '<';

						pCurPack = pPackName;
						goto jPackRestart;
					}
				}
			}
			XSAFE_FREE(pPackData);
		}
	}
	return pListing;
}

CHAR pCurPath[MAX_PATH];
CHAR pStreakPath[MAX_PATH];
bool CreateDirectories() {
	sprintf(pCurPath, "%s", ES("NiNJA:\\ninjacache"));
	if (CreateDirectory(pCurPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		DbgPrint("ninjacache directory created successfully!");
	}
	else {
		DbgPrint("Failed to create directory!");
		return false;
	}

	sprintf(pCurPath, "%s%s", pCurPath, "\\media");
	if (CreateDirectory(pCurPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		DbgPrint("Media directory created successfully!");
	}
	else {
		DbgPrint("Failed to create directory!");
		return false;
	}

	sprintf(pCurPath, "%s%s", pCurPath, "\\sounds");
	if (CreateDirectory(pCurPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		DbgPrint("Media directory created successfully!");
	}
	else {
		DbgPrint("Failed to create directory!");
		return false;
	}

	sprintf(pStreakPath, "%s%s", pCurPath, "\\streak");
	if (CreateDirectory(pStreakPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		DbgPrint("Streak directory created successfully!");
	}
	else {
		DbgPrint("Failed to create directory!");
		return false;
	}

	sprintf(pCurPath, "%s%s", pCurPath, "\\spree");
	if (CreateDirectory(pCurPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		DbgPrint("Spree directory created successfully!");
	}
	else {
		DbgPrint("Failed to create directory!");
		return false;
	}
	return true;
}

CHAR pPath[1000];
bool GameFuncs::DownloadSoundPack() {
	if (MountPath(MOUNT_NAME, GetMountPath(), FALSE) == 0) {
		DbgPrint("Mounted path successfully!");
	}
	else {
		DbgPrint("Failed to mount path!");
		return false;
	}

	int iRetries = 0;
	int iMaxRetries = 4;

	ZeroMemory(pPath, 1000);

jRetry:
	CHAR pSubDomain[100];
	ZeroMemory(pSubDomain, 100);
	PCHAR pTmp = GetRandomSubDomain();
	sprintf(pSubDomain, pTmp, iRetries > 1 ? "xbls" : "xbl");
	vector<PCHAR> vPacks = GetWebDirectoryListing(pSubDomain, "/authed/sounds/");
	if (vPacks.size() > 0) {
		for (DWORD p = 0; p < vPacks.size(); p++) {
			if (Compare(vPacks[p], "halo3/")) {
				DbgPrint("Parsing halo pack!");
				if (CreateDirectories()) {
					sprintf(pPath, "/authed/sounds/%s", vPacks[p]);
					vector<PCHAR> vFolders = GetWebDirectoryListing(pSubDomain, pPath);
					if (vFolders.size() > 0) {
						for (DWORD f = 0; f < vFolders.size(); f++) {
							sprintf(pPath, "/authed/sounds/%s%s", vPacks[p], vFolders[f]);
							vector<PCHAR> vSounds = GetWebDirectoryListing(pSubDomain, pPath);
							if (vSounds.size() > 0) {
								DbgPrint("Requesting sounds from the server! Sound listing: ");
								for (DWORD s = 0; s < vSounds.size(); s++) {
									string sLocalPath = "NiNJA:\\ninjacache\\media\\sounds\\" + RemoveChars(vFolders[f], "/") + "\\" + RemoveChars(vSounds[s], "/");
									if (!FileExists(sLocalPath.c_str())) {
										HttpRequest pSoundRequest;
										BOOL bSuccess = FALSE;
										sprintf(pPath, "/authed/sounds/%s%s%s", vPacks[p], vFolders[f], vSounds[s]);
										//PBYTE pSound = pSoundRequest.SendRequest(pSubDomain, pPath, FALSE, &bSuccess, 80, 80000);
										if (pSoundRequest.DownloadFile(pSubDomain, pPath, sLocalPath, 500000, FALSE)) {
											DbgPrint("We got the sound! Bytes read: %i", pSoundRequest.dwBytesRead);
											//BinToFile((PCHAR)sLocalPath.c_str(), pSound, pSoundRequest.dwBytesRead);
											//sManager.free(pSound);
										}
										DbgPrint("%s", vSounds[s]);
									}
									else {
										DbgPrint("%s already exists! Skipping!", vSounds[s]);
									}
								}
								for (DWORD c = 0; c < vSounds.size(); c++) {
									sManager.free(vSounds[c]);
								}
								vSounds.clear();
							}
						}
						for (DWORD c = 0; c < vFolders.size(); c++) {
							sManager.free(vFolders[c]);
						}
						vFolders.clear();
						DbgPrint("Finished downloading soundpack!");
					}
				}
			}
			else {
				DbgPrint("Unsupported pack!");
			}
		}
		for (DWORD c = 0; c < vPacks.size(); c++) {
			sManager.free(vPacks[c]);
		}
		vPacks.clear();
		ZeroMemory(pPath, 1000);
		DbgPrint("Finished checking soundpack listings!");
		return true;
	}
	ZeroMemory(pPath, 1000);
	if (iRetries < iMaxRetries) {
		iRetries++;
		goto jRetry;
	}
	return false;
}

VOID GameFuncs::DrawBox(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT rotation, const FLOAT *color, const FLOAT *borderColor, DWORD borderWidth) {
	//// If we're drawing a border, draw the original box further down so everything lines up right
	if (borderWidth != -1) {
		//x += (borderWidth / 2);
		y += (borderWidth / 2);
	}

	// Draw the main box
	DrawRotatedPic(x, y, w, h, rotation, color, CG::dwWhite);

	if (borderWidth != -1) {
		//x -= (borderWidth / 2);
		y -= (borderWidth / 2);
	}

	// If we've specified the optional borderColor and borderWidth params, then draw an outline around the box.
	if (borderColor != NULL && borderWidth != -1) {
		// Draw the border around the box
		DrawLine(x+borderWidth/2, y, x + w-borderWidth/2, y, borderWidth, borderColor, CG::dwWhite); // Top left to top right
		DrawLine(x, y-borderWidth/2, x, y + h + borderWidth/2, borderWidth, borderColor, CG::dwWhite); // Top left to bottom left
		DrawLine(x+borderWidth/2, y + h, x + w-borderWidth/2, y + h, borderWidth, borderColor, CG::dwWhite); // Bottom left to bottom right
		DrawLine(x + w, y-borderWidth/2, x + w, y + h + borderWidth/2, borderWidth, borderColor, CG::dwWhite); // Top right to bottom right
	}
}

VOID GameFuncs::DrawLine(FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2, FLOAT Width, const float *Color, DWORD Shader) {
	float X, Y, Angle, L1, L2, H1;
	H1 = Y2 - Y1;
	L1 = X2 - X1;
	L2 = sqrt(L1 * L1 + H1 * H1);
	X = X1 + ((L1 - L2) / 2);
	Y = Y1 + (H1 / 2);
	Angle = (float)atan(H1 / L1) * (180.0f / 3.14159265358979323846f);
	DrawRotatedPic(X, Y, L2, Width, Angle, Color, Shader);
}

VOID GameFuncs::DrawHandlePic(FLOAT x, FLOAT y, FLOAT w, FLOAT h, DWORD horizAlign, DWORD vertAlign, FLOAT rotation, const FLOAT *color, DWORD shader) {
	((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.UI_DrawHandlePic))(decryptDWORD(Not(NiNJA.cPatches)->CoD.ScreenPlacement), x, y, w, h, horizAlign, vertAlign, rotation, color, shader);
}

VOID GameFuncs::DrawRotatedPic(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT rotation, const FLOAT *color, DWORD shader) {
	if (shader && color) {
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_DrawRotatedPic))(decryptDWORD(Not(NiNJA.cPatches)->CoD.ScreenPlacement), x, y, w, h, rotation, color, shader);
	}
}

VOID GameFuncs::DrawText(CHAR *txt, FLOAT width, DWORD font, FLOAT x, FLOAT y, FLOAT scaleX, FLOAT scaleY, FLOAT rotation, const FLOAT *color, bool bDrawShader, FLOAT fBorderWidth, DWORD dwNumLinesWrapped) {
	if (txt) {
		if (width == 0 && bDrawShader) {
			width = GetTextWidth(txt, font);
		}
		if (bDrawShader) {
			float fTmpY = y;
		//#if defined(WAW) || defined(BO1) || defined(MW2) || defined(MW3) || defined(GHOSTS)
		//	fTmpY -= 1.0f;
		//#endif
			FLOAT height = GetFontHeight(font, scaleY) - floor(scaleY * 1.5f);
			GameFuncs::DrawBox(x - 2.0f - fBorderWidth, fTmpY - height + (fBorderWidth < 2.0f ? 2.0f : 0.0f), (width * scaleX) + 4.0f + (fBorderWidth * 2), (height * (dwNumLinesWrapped + 1)) - (fBorderWidth < 2.0f ? 4.0f : 0.0f), rotation, Colors::BlackTrans);
		}
	#if defined(WAW) || defined(BO3)
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.R_DrawText))(txt, 0x7fffffff, font, x, y, scaleX, scaleY, color, rotation);
	#elif defined(BO1)
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.R_DrawText))(txt, 0x7fffffff, font, x, y, 1.0f, scaleX, scaleY, rotation, color);
	#else
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.R_DrawText))(txt, 0x7fffffff, font, x, y, scaleX, scaleY, rotation, color, 0);
	#endif
	}
}

VOID GameFuncs::DrawTextWithBorder(CHAR *text, DWORD font, FLOAT x, FLOAT y, FLOAT scaleX, FLOAT scaleY, FLOAT borderWidth, FLOAT rotation, const FLOAT *color, const FLOAT *borderColor) {
	FLOAT width = GetTextWidth(text, font);
	DrawText(text, width, font, x - borderWidth, y + borderWidth, scaleX, scaleY, rotation, borderColor);
	DrawText(text, width, font, x + borderWidth, y - borderWidth, scaleX, scaleY, rotation, borderColor);
	DrawText(text, width, font, x - borderWidth, y - borderWidth, scaleX, scaleY, rotation, borderColor);
	DrawText(text, width, font, x + borderWidth, y + borderWidth, scaleX, scaleY, rotation, borderColor);
	DrawText(text, width, font, x, y, scaleX, scaleY, rotation, color);
}

DWORD GameFuncs::Dvar_GetValueAddress(DWORD dwDvarAddress) {
#if defined(MW1) || defined(GHOSTS) || defined(AW)
	return (*(DWORD*)dwDvarAddress + 0xc);
#elif defined(MW2) || defined(MW3)
	return (*(DWORD*)dwDvarAddress + 0x8);
#elif  defined(BO1) || defined(BO2)
	return (*(DWORD*)dwDvarAddress + 0x18);
#elif defined(WAW)
	return (*(DWORD*)dwDvarAddress + 0x10);
#endif
	return 0;
}

bool GameFuncs::Dvar_GetBool(DWORD dwDvarAddress) {
	DWORD dwValueAddress = Dvar_GetValueAddress(dwDvarAddress);
	if (dwValueAddress) {
		return *(BYTE*)dwValueAddress != 0;
	}
	return 0;
}

DWORD GameFuncs::Dvar_GetValue(DWORD dwDvarAddress) {
	DWORD dwValueAddress = Dvar_GetValueAddress(dwDvarAddress);
	if (dwValueAddress) {
		return *(DWORD*)dwValueAddress;
	}
	return 0;
}

VOID GameFuncs::EndGame() {
	char *endGameString;
	int serverId = *(int*)decryptDWORD(Not(NiNJA.cPatches)->CoD.ServerIdAddress);
#if defined(AW)
	endGameString = va(ES("cmd lui 22 1 %i"), serverId);
#elif defined(GHOSTS)
	endGameString = va(ES("cmd lui 9 1 %i"), serverId);
#elif defined(BO3)
	endGameString = va(ES("cmd mr %i 7 endround"), serverId);
#else
	endGameString = va(ES("cmd mr %i %i endround"), serverId, -1);
#endif
	if (endGameString) {
		SendConsoleCommand(endGameString);
	}
}

VOID GameFuncs::LeaveGame() {
	CG::bQuittingGame = TRUE;
	SendConsoleCommand("disconnect");
}

FLOAT GameFuncs::GetFontHeight(DWORD dwFontPtr, FLOAT fScale) {
	if (dwFontPtr) {
		return (float)((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.R_TextHeight))(dwFontPtr) * fScale;
	}
	return 25.0f * fScale;
}

DWORD GameFuncs::Get_CEntity_t(DWORD dwClientIndex) {
#if defined(BO3)
	return ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.GetClientEntity))(0, dwClientIndex);
#else
	DWORD pCentity = *(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.CEntity_t);
	if (pCentity) {
	#if defined(WAW)
		return pCentity + (dwClientIndex * 0x2b0);
	#elif defined(BO1)
		return pCentity + (dwClientIndex * 0x31c);
	#elif defined(BO2)
		return pCentity + (dwClientIndex * 0x374);
	#elif defined(MW1)
		return pCentity + (dwClientIndex * 0x1d0);
	#elif defined(MW2)
		return pCentity + (dwClientIndex * 0x1f8);
	#elif defined(MW3) || defined(GHOSTS)
		return pCentity + (dwClientIndex * 0x1ec);
	#elif defined(AW)
		return pCentity + (dwClientIndex * 0x20c);
	#endif
	}
#endif
	return 0;
 }

DWORD GameFuncs::Get_CG_t() {
	return *(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_t);
}

DWORD GameFuncs::Get_CGS_t() {
	return *(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.CGS_t);
}

DWORD GameFuncs::Get_ClientActive_t() {
	return *(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientActive_t);
}

vec3_t *GameFuncs::Get_ClientActive_ViewAngles() {
	DWORD clientActive_t = Get_ClientActive_t();
	if (clientActive_t) {
	#if defined(WAW)
		return (vec3_t*)(clientActive_t + 0x26a80);
	#elif defined(MW1)
		return (vec3_t*)(clientActive_t + 0x25530);
	#elif defined(MW2)
		return (vec3_t*)(clientActive_t + 0x3358);
	#elif defined(MW3)
		return (vec3_t*)(clientActive_t + 0x35B0);
	#elif defined(BO1)
		return (vec3_t*)(clientActive_t + 0x2840);
	#elif defined(BO2)
		return (vec3_t*)(clientActive_t + 0x2c8c);
	#elif defined(BO3)
		return (vec3_t*)(clientActive_t + 0x31a0);
	#elif defined(GHOSTS)
		return (vec3_t*)(clientActive_t + 0x32d0);
	#elif defined(AW)
		return (vec3_t*)(clientActive_t + 0x3568);
	#endif
	}
	return 0;
}

DWORD GameFuncs::Get_ClientInfo_t(DWORD dwClient) {
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
	#if defined(BO1)
		return cg_t + 0x5DAF4 + (dwClient * 0x5d0);
	#elif defined(MW1)
		return cg_t + 0x217D4 + (dwClient * 0x80);
	#elif defined(MW2)
		return cg_t + 0xFEA0C + (dwClient * 0x544);
	#elif defined(GHOSTS)
		return cg_t + 0xB9430 + (dwClient * 0x59c);
	#elif defined(AW)
		return cg_t + 0xA059c + (dwClient * 0xc34);
	#elif defined(BO3)
		return cg_t + 0x54E10 + (dwClient * 0x980);
	#endif
	}
	return 0x82000000;
}

DWORD GameFuncs::Get_ClientDObj(DWORD dwClient, DWORD dwLocalClientNum) {
#if !defined(BO3)
	return ((DWORD(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.Com_GetClientDObj))(dwClient, dwLocalClientNum);
#elif defined(BO3)
	int DObj_s = *(short *)(decryptDWORD(Not(NiNJA.cPatches)->CoD.Com_GetClientDObj) + ((dwClient + (dwLocalClientNum * 0x4C1)) << 1));

	if (!DObj_s)
		return 0;

	return 0x83E71440 + (DObj_s * 0xA0);
#else
	return 0;
#endif
}

DWORD GameFuncs::Get_Input_t() {
	int clientActive_t = Get_ClientActive_t();
	if (clientActive_t) {
	#if defined(WAW)
		return clientActive_t + 0x66AA0;
	#elif defined(MW1)
		return clientActive_t + 0x65550;
	#elif defined(BO1)
		return clientActive_t + 0x4285C;
	#elif defined(BO2)
		return clientActive_t + 0x42ca8;
	#elif defined(MW2)
		return clientActive_t + 0x3364;
	#elif defined(MW3)
		return clientActive_t + 0x35bc;
	#elif defined(GHOSTS)
		return clientActive_t + 0x32dc;
	#elif defined(AW)
		return clientActive_t + 0x3574;
	#elif defined(BO3)
		return clientActive_t + 0x231BC;
	#endif
	}
	return 0;
}

DWORD GameFuncs::Get_NetInfo(DWORD dwClient) {
	if (dwClient < __MAXCLIENTS__) {
#if defined(WAW)
		return (decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientNetInfo) + (dwClient * 0xc8));
#elif defined(MW1)
		return (decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientNetInfo) + (dwClient * 0xb8));
#elif defined(MW2)
		return (decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientNetInfo) + (dwClient * 0xc0));
#elif defined(MW3)
		return (decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientNetInfo) + (dwClient * 0x158));
#elif defined(GHOSTS)
		return (decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientNetInfo) + (dwClient * 0x1c0));
#elif defined(AW)
		return (decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientNetInfo) + (dwClient * 0x350));
#elif defined(BO1)
		return (decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientNetInfo) + (dwClient * 0x200));
#elif defined(BO2)
		return (decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientNetInfo) + (dwClient * 0x148));
#elif defined(BO3)
		return decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientNetInfo) + (dwClient * 0x120);
#endif
	}
	return 0;
}

DWORD GameFuncs::Get_RefDef_t() {
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
	#if defined(MW1)
		return cg_t + 0x478cc;
	#elif defined(WAW)
		return cg_t + 0x53d04;
	#elif defined(MW2)
		return cg_t + 0x6AC28;
	#elif defined(MW3)
		return cg_t + 0x4;
	#elif defined(GHOSTS)
		return cg_t + 0x6D53C;
	#elif defined(AW)
		return cg_t + 0x377C0;
	#elif defined(BO1)
		return cg_t + 0x41a04;
	#elif defined(BO2)
		return cg_t + 0x4D880;
	#elif defined(BO3)
		return cg_t + 0x32554;//tu3 0x31954;
	#endif
	}
	return 0;
}

DWORD GameFuncs::Get_ViewAxis_Ptr() {
	DWORD refDef = Get_RefDef_t();
	if (refDef) {
	#if defined(WAW)
		return refDef + 0x38;
	#elif defined(MW1) || defined(MW3)
		return refDef + 0x20;
	#elif defined(BO1)
		return refDef + 0x3c;
	#elif defined(BO2)
		return refDef + 0x48;//0x401A90
	#elif defined(MW2) || defined(GHOSTS) || defined(AW)
		return refDef + 0x24;
	#elif defined(BO3)
		return refDef + 0x9c;
	#endif
	}
	return 0;
}

vec3_t GameFuncs::Get_RefDef_ViewAngles() {
	int viewAxis_ptr = Get_ViewAxis_Ptr();
	if (viewAxis_ptr) {
		vec3_t newAngles;
		AxisToAngles((vec3_t*)viewAxis_ptr, &newAngles);
		return newAngles;
	}
	return vec3_t(0, 0, 0);
}

DWORD GameFuncs::GetBulletType(DWORD dwWeaponIndex) {
	int weaponDef = GetWeaponDef(dwWeaponIndex);
	if (weaponDef) {
		int bulletType = 0, lzCount = 0, idk = 0;

	#if defined(MW1)
		bulletType = *(DWORD*)(weaponDef + 0x4e0);
	#elif defined(MW2)
		bulletType = *(byte*)(weaponDef + 0x65C);
	#elif defined(MW3)
		weaponDef = GetWeaponDef(dwWeaponIndex, true);
		if (weaponDef)
			bulletType = *(byte*)(weaponDef + 0x771);
	#elif defined(BO1)
		bulletType = *(byte*)(weaponDef + 0x54D);
	#elif defined(BO2)
		bulletType = *(byte*)(weaponDef + 0x631);
	#elif defined(BO3)
		bulletType = *(byte*)(*(int*)(weaponDef + 0x10) + 0x941);
	#elif defined(GHOSTS)
		bulletType = *(byte*)(weaponDef + 0x809);
	#elif defined(AW)
		bulletType = *(byte*)(weaponDef + 0xA77);
	#endif

		__asm {
			cntlzw    lzCount, bulletType
			extrwi    idk, lzCount, 1, 26
			xori      bulletType, idk, 1
			addi      bulletType, bulletType, 1
		};
		return bulletType;
	}
	return 1;
}


DWORD GameFuncs::Get_UnknownClientStruct() {
	DWORD UnknownClientStruct = decryptDWORD(Not(NiNJA.cPatches)->CoD.UnknownClientStruct);
	if (UnknownClientStruct) {
		return *(DWORD*)UnknownClientStruct;
	}
	return 0;
}

DWORD dwLastFrameTime = GetTickCount();
DWORD dwLastFrameTimes[64] = { 0 };
DWORD dwLastFPSValue = 60;
DWORD dwLastStableValue = 60;
DWORD dwFramesSinceChange = 0;
DWORD GameFuncs::CalculateFPS() {
	DWORD dwRunningTotal = 0;
	DWORD dwFrameTime = GetTickCount();
	for (DWORD i = 0; i < 63; i++) {
		dwLastFrameTimes[i] = dwLastFrameTimes[i + 1];
		dwRunningTotal += dwLastFrameTimes[i];
	}
	dwLastFrameTimes[63] = dwFrameTime - dwLastFrameTime;
	dwRunningTotal += dwLastFrameTimes[31];
	dwLastFrameTime = dwFrameTime;

	if (dwRunningTotal != 0) {
		DWORD dwAverage = dwRunningTotal / 64;
		if (dwAverage != 0) {
			DWORD dwFPS = (FLOAT)(1000 / dwAverage);
			if (dwLastFPSValue != dwFPS) {
				if (dwLastStableValue != dwLastFPSValue) {
					dwLastStableValue = dwLastFPSValue;
					dwFramesSinceChange = 0;
				}

				if (dwFPS != dwLastStableValue) {
					dwFramesSinceChange++;
				}

				if (dwFramesSinceChange > 1) {
					if (dwLastFPSValue > dwFPS) {
						dwLastFPSValue--;
					}
					else {
						dwLastFPSValue++;
					}
					if (dwLastFPSValue > 60) {
						dwLastFPSValue = 60;
					}
				}
			}
		}
	}
	return dwLastFPSValue;
}

// TODO: FINISH THESE ON BO3
PCHAR GameFuncs::GetMapName_Internal() {
#if defined(BO2)
	DWORD cgs_t = *(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.CGS_t);
	if (cgs_t) {
		return (char*)(cgs_t + 0x198);
	}
#elif !defined(BO3)
	return (PCHAR)Dvar_GetValue(decryptDWORD(Not(NiNJA.cPatches)->CoD.Dvar_UI_Mapname));
#endif
	return 0;
}

PCHAR GameFuncs::GetMapName() {
#ifndef BO3
	PCHAR mapName = GetMapName_Internal();
	if (mapName) {
		return ((PCHAR(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.UI_GetMapDisplayName))(mapName);
	}
#endif
	return 0;
}

PCHAR GameFuncs::GetGametypeName_Internal() {
	PCHAR pGametype = NULL;
#if defined(MW1)
	pGametype = *(PCHAR*)(*(DWORD*)0x849F2DD8 + 0x1c);
#elif defined(WAW)
	pGametype = *(PCHAR*)(*(DWORD*)0x84FAA55C + 0x10);
#elif defined(BO1)
	pGametype = *(PCHAR*)(*(DWORD*)0x8391111C + 0x18);
#elif defined(MW2)
	DWORD CGS_t = Get_CGS_t();
	if (CGS_t) {
		pGametype =(PCHAR)(CGS_t + 0x20);
	}
#elif defined(MW3) || defined(GHOSTS) || defined(AW)
	DWORD CGS_t = Get_CGS_t();
	if (CGS_t) {
		pGametype = (PCHAR)(CGS_t + 0x24);
	}
#elif defined(BO2)
	DWORD CGS_t = Get_CGS_t();
	if (CGS_t) {
		pGametype = (PCHAR)(CGS_t + 0x30);
	}
#endif
	if (pGametype) {
		CG::pGameTypeName = pGametype;
		return pGametype;
	}
	return 0;
}

PCHAR GameFuncs::GetGametypeName() {
#ifndef BO3
	PCHAR gametypeName = GetGametypeName_Internal();
	if (gametypeName) {
#if defined(BO2)
	PCHAR ret = ((PCHAR(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.UI_GetGametypeDisplayName))(gametypeName, 0);
#else
	PCHAR ret = ((PCHAR(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.UI_GetGametypeDisplayName))(gametypeName);
#endif
	#if defined(MW1)
		ret = ((PCHAR(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.UI_SafeTranslateString))(ret);
	#endif
		CG::pGameTypeDisplayName = ret;
		return ret;
	}
#endif
	return 0;
}

FLOAT AngleDifference(FLOAT angle0, FLOAT angle1) {
	return abs((FLOAT)((DWORD)(angle0 + 180.0f - angle1) % 360) - 180.0f);
}

vec2_t GameFuncs::GetRealFovDist(vec3_t vFromPosition, vec3_t vFromAngles, vec3_t vToPosition) {
	// Calculate the final aim vector
	vec3_t vTargetAngles = vectoangles(vToPosition.x - vFromPosition.x, vToPosition.y - vFromPosition.y, vToPosition.z - vFromPosition.z);

	// Get the distance between you and the enemy
	FLOAT fPlayerDistance = Get3dDistance(vFromPosition, vToPosition);

	// Get the distance between your current yaw and the yaw to aim at the enemy
	FLOAT fYawDifference = fabsf(AngleDifference(vFromAngles.y, vTargetAngles.y));

	FLOAT fYawDistance = sin(D3DXToRadian(fYawDifference)) * fPlayerDistance;

	return vec2_t();
}

PCHAR GameFuncs::GetHostName() {
#if defined(MW1) || defined(MW2) 
	if ( GameFuncs::CheckInGame() && CG::bLobbyInitialized) {
		DWORD CGS_t = Get_CGS_t();
		if (CGS_t) {
			return (PCHAR)(CGS_t + 0x40);
		}
	}
#elif defined(MW3) || defined(GHOSTS) || defined(AW)
	if (CG::bInGameCached && CG::bLobbyInitialized) {
		DWORD CGS_t = Get_CGS_t();
		if (CGS_t) {
			return (PCHAR)(CGS_t + 0x44);
		}
	}
#elif defined(WAW) || defined(BO1) || defined(BO2)
	return (PCHAR)Dvar_GetValue(decryptDWORD(Not(NiNJA.cPatches)->CoD.Dvar_PartyHostname));
#elif defined(BO3)
	if ( GameFuncs::CheckInGame() && CG::bLobbyInitialized) {
		DWORD CGS_t = Get_CGS_t();
		if (CGS_t) {
			return (PCHAR)(CGS_t + 0x50);
		}
	}
#endif
	return 0;
}

DWORD GameFuncs::GetWeaponClipSize(DWORD dwClient) {
	DWORD dwWeapon = CG::cEntities.List[dwClient].pExt->dwCurrentWeapon;
#if defined(MW1) || defined(WAW)
	DWORD dwWeapDef = GetWeaponDef(dwWeapon);
	if (dwWeapDef) {
		return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetClipSize))(dwWeapDef, dwWeapon);
	}
#elif defined(MW2)
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetClipSize))(cg_t, dwWeapon);
	}
#elif defined(MW3)
	DWORD cg_t = Get_UnknownClientStruct();
	if (cg_t) {
		return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetClipSize))(cg_t, dwWeapon);
	}
#elif defined(GHOSTS)
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetClipSize))(cg_t, dwWeapon);
	}
#elif defined(AW)
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetClipSize))(cg_t, dwWeapon);
	}
#elif defined(BO1)
	return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetClipSize))(dwWeapon);
#elif defined(BO2)
	return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetClipSize))(dwWeapon);
#elif defined(BO3)
	return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetClipSize))(dwWeapon);
#endif
	return 0;
}


DWORD GameFuncs::GetWeaponAmmo(DWORD dwClient, bool bLeftHand) {
	DWORD dwWeapon = CG::cEntities.List[dwClient].pExt->dwCurrentWeapon;
#if defined(MW1) || defined(WAW) //CG_GetPlayerClipAmmoCount
	return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(0);
#elif defined(MW2) //BG_GetAmmoInClipForWeapon
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		if (!bLeftHand)
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, dwWeapon, 0, 0);
		else
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, dwWeapon, 1, 0);
	}
#elif defined(MW3) //BG_GetAmmoInClipForWeapon
	DWORD cg_t = Get_UnknownClientStruct();
	if (cg_t) {
		if (!bLeftHand)
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, dwWeapon, 0, 0);
		else
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, dwWeapon, 0, 1);
	}
#elif defined(BO1)  || defined(BO2) || defined(BO3)//BG_AmmoForWeapon
	DWORD cg_t = Get_UnknownClientStruct();
	if (cg_t) {
		if (!bLeftHand)
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, dwWeapon);
		else
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, dwWeapon + 1);
	}
#elif defined(GHOSTS) //BG_GetAmmoInClipForWeapon
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		if (!bLeftHand)
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, dwWeapon, 0, 0);
		else
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, dwWeapon, 0, 1);
	}
#elif defined(AW) //BG_GetAmmoInClipForWeapon
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		if (!bLeftHand)
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, ((QWORD(*)(...))0x82794650)(dwWeapon, 0), 0);
		else
			return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AmmoForWeapon))(cg_t, ((QWORD(*)(...))0x82794650)(dwWeapon, 0), 1);
	}
#endif
	return 420;
}

PCHAR GameFuncs::GetGamertagFromXUID(DWORD dwClient, QWORD userXuid) {
	if (NiNJA.bSignedIntoLive) {
		XUID myXuid;
		FIND_USER_INFO tmpFindInfo[1];
		// Grab our own XUID, so we can make sure we aren't trying to pull our own gamertag
		if (XUserGetXUID(0, &myXuid) == 0) {
			// Set the request XUID to the clients XUID
			tmpFindInfo[0].qwUserId = userXuid;

			// Grab the size of the response
			DWORD dwResponseSize = XUserFindUsersResponseSize(1);

			// Setup our userResponse struct
			FIND_USER_INFO pUsers;
			FIND_USERS_RESPONSE pFindUsersResponse;
			pFindUsersResponse.pUsers = &pUsers;

			//printf("Putting in find users request now!\n");

			XOVERLAPPED xovl;
			ZeroMemory(&xovl, sizeof(XOVERLAPPED));
			DWORD ret = XUserFindUsers(myXuid, 1, tmpFindInfo, dwResponseSize, &pFindUsersResponse, &xovl);

			while (!XHasOverlappedIoCompleted(&xovl)) {
				Sleep(10);
			}

			if (xovl.dwExtendedError == ERROR_SUCCESS) {
				PCHAR pOut;
				int iGtLen = strlen(pFindUsersResponse.pUsers->szGamerTag);
				if (sManager.alloc(iGtLen + 1, &pOut)) {
					//printf("Success grabbing gamertag %s from xuid!\n", pFindUsersResponse.pUsers->szGamerTag);
					memcpy(pOut, pFindUsersResponse.pUsers->szGamerTag, iGtLen);

					return pOut;
				}
			}
			else {
				//printf("Failed to grab userInfo from xuid!\n");
			}
		}
	}
	else if(CG::bInGameCached) {
		return GetEntityName(dwClient);
	}
	return 0;
}

/* THIS CAN ONLY BE CALLED FROM THE GAME THREAD! */
PCHAR GameFuncs::GetClientGamertag(DWORD dwClient) {
	// If their name already exists, free it from our sManager before we allocate space for a new one
	if (CG::cEntities.List[dwClient].pExt->pRealName) {
		if (!sManager.free(CG::cEntities.List[dwClient].pExt->pRealName)) {
			// We failed to free the memory, but it doesn't really matter so just continue silently
		}
	}
	return GetGamertagFromXUID(dwClient, CG::cEntities.List[dwClient].pExt->cXuid);
}

PCHAR GameFuncs::GetClientGamertagFromNetInfo(DWORD dwClient) {
	DWORD dwNetInfo = CG::cEntities.List[dwClient].pExt->pNetInfo;
	if (dwNetInfo) {
	#if defined(MW1) || defined(WAW) || defined(BO1) || defined(BO2) || defined(BO3)
		return (char*)(dwNetInfo + 0x8);
	#elif defined(GHOSTS) || defined(MW2) || defined(MW3) || defined(AW)
		return (char*)(dwNetInfo);
	#endif
	}
	return 0;
}

IN_ADDR GameFuncs::GetClientIP(DWORD dwClient) {
	IN_ADDR ip = IN_ADDR();
	if (CG::cEntities.List[dwClient].pExt->pNetInfo) {
	#if defined(MW1)
		return *(IN_ADDR*)(CG::cEntities.List[dwClient].pExt->pNetInfo + 0x58);
	#elif defined(MW2)
		return *(IN_ADDR*)(CG::cEntities.List[dwClient].pExt->pNetInfo + 0x42);
	#elif defined(MW3)
		return *(IN_ADDR*)(CG::cEntities.List[dwClient].pExt->pNetInfo + 0xb8);
	#elif defined(BO1)
		return *(IN_ADDR*)(CG::cEntities.List[dwClient].pExt->pNetInfo + 0x60);
	#elif defined(BO2)
		return *(IN_ADDR*)(CG::cEntities.List[dwClient].pExt->pNetInfo + 0x7c);
	#elif defined(GHOSTS)
		return *(IN_ADDR*)(CG::cEntities.List[dwClient].pExt->pNetInfo + 0xe8);
	#elif defined(AW)
		return *(IN_ADDR*)(CG::cEntities.List[dwClient].pExt->pNetInfo + 0x29c);
	#elif defined(WAW)
		return *(IN_ADDR*)(CG::cEntities.List[dwClient].pExt->pNetInfo + 0x58);
	#elif defined(BO3)
		return *(IN_ADDR*)(*(int*)(CG::cEntities.List[dwClient].pExt->pNetInfo + 0x28) + 0xC);
	#endif
	}
	return ip;
}

QWORD GameFuncs::GetClientXUID(DWORD dwClient) {
	DWORD dwNetInfo = CG::cEntities.List[dwClient].pExt->pNetInfo;
	if (dwNetInfo) {
#if defined(MW2)
	return *(QWORD*)(dwNetInfo + 0xae);
#elif defined(MW3)
	return *(QWORD*)(dwNetInfo + 0xfc);
#elif defined(GHOSTS)
	return *(QWORD*)(dwNetInfo + 0x134);
#elif defined(AW)
	return *(QWORD*)(dwNetInfo + 0x2e8);
#else
	return *(QWORD*)dwNetInfo;
#endif
	}
	return 0;
}

PCHAR GameFuncs::GetEntityName(DWORD dwEntIndex) {
#if defined(GHOSTS) || defined(AW)
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
	#if defined(GHOSTS)
		return (PCHAR)(cg_t + 0xC5E24 + (dwEntIndex * 0x74));
	#elif defined(AW)
		return (PCHAR)(cg_t + 0xBBCE9 + (dwEntIndex * 0x74));
	#endif
	}
#else
#if defined(WAW) || defined(BO1) || defined(BO2)
	if (CG::cEntities.List[dwEntIndex].bAI) {
		if (CG::bIsMultiplayer) {
			return "Attack Dog";
		}
		else {
			return "Zombie";
		}
	}
#endif
	if (CG::cEntities.List[dwEntIndex].pClientInfo) {
		PCHAR name = (char*)(CG::cEntities.List[dwEntIndex].pClientInfo + 0xc);
		if (strlen(name) <= 32)
			return name;
	}
#endif
	return 0;
}

vec3_t GameFuncs::GetEntityLocation(DWORD dwEntIndex) {
#if defined(MW1)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x1c);
#elif defined(MW2)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x18);
#elif defined(MW3) || defined(GHOSTS) || defined(AW)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x14);
#elif defined(WAW) || defined(BO3)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x28);
#elif defined(BO1)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x30);
#elif defined(BO2)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x2c);
#else
	return vec3_t();
#endif
}

DWORD GameFuncs::GetEntityStance(DWORD dwEntIndex) {
#if defined(AW) || defined(MW2)
	return *(DWORD*)(CG::cEntities.List[dwEntIndex].pCentity + 0x6C);
#elif defined(BO2)
	return *(DWORD*)(CG::cEntities.List[dwEntIndex].pCentity + 0x1D4);
#elif defined(BO3)
	return *(DWORD*)(CG::cEntities.List[dwEntIndex].pCentity + 0x1B0);
#endif
	return 0;
}

#if defined(WAW)
int FixDroppedWeaponNum(int droppedWeaponNum) {
	__asm {
		mr        r9, r3
		srawi     r7, r9, 8
		addze     r6, r7
		slwi      r4, r6, 8
		subf      r3, r4, r9
		blr
	}
}
#endif

vec3_t GameFuncs::GetEntityViewAngles(DWORD dwEntIndex) {
#if defined(MW1)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0xfc);
#elif defined(MW2)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x9c);
#elif defined(MW3)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x90);
#elif defined(WAW)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x104);
#elif defined(BO1)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x1a0);
#elif defined(BO2)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x18c);
#elif defined(GHOSTS)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x100);
#elif defined(AW)
	return *(vec3_t*)(CG::cEntities.List[dwEntIndex].pCentity + 0x164);
#else
	return vec3_t();
#endif
}

DWORD GameFuncs::GetDropWeaponIndex(DWORD dwEntIndex) {
#if defined(MW1)
	return *(int*)(CG::cEntities.List[dwEntIndex].pCentity + 0x148) % 0x80;
#elif defined(WAW)
	int weapIndex = FixDroppedWeaponNum(*(int*)(CG::cEntities.List[dwEntIndex].pCentity + 0x208));
	if (weapIndex != 0)
		return weapIndex;
	else
		return *(int*)(CG::cEntities.List[dwEntIndex].pCentity + 0x244);
#elif defined(BO1)
	int weapIndex = *(int*)(CG::cEntities.List[dwEntIndex].pCentity + 0x274);
	if (weapIndex != 0)
		return weapIndex;
	else
		return *(int*)(CG::cEntities.List[dwEntIndex].pCentity + 0x2a4);
#elif defined(BO2)
	return *(int*)(CG::cEntities.List[dwEntIndex].pCentity + 0x2b4);
#elif defined(MW2)
	int weapIndex = *(short*)(CG::cEntities.List[dwEntIndex].pCentity + 0x15a) % 1400;
	if (weapIndex != 0)
		return weapIndex;
	else
		return *(short*)(CG::cEntities.List[dwEntIndex].pCentity + 0x19c) % 1400;
#elif defined(MW3)
	return *(int*)(CG::cEntities.List[dwEntIndex].pCentity + 0x18c);
#elif defined(GHOSTS)
	return *(int*)(CG::cEntities.List[dwEntIndex].pCentity + 0x188);
#elif defined(AW)
	return *(int*)(CG::cEntities.List[dwEntIndex].pCentity + 0x124);
#endif
	return 0;
}

VOID GameFuncs::RunVisibilityChecks(DWORD dwClient, DWORD dwTag, int iTagOffset) {
	// Run visibility checks on every bone, or if selected bone mode is eanbled only the selected bone
	if (CG::cTags.List[dwTag].bCheckVis && CG::cEntities.List[dwClient].bEnemy) {
		if (dwTag > TAG_SHOULDER_RI && !CG::cEntities.List[dwClient].bPlayer) {
			// If we're dealing with a non player entity, they probably don't have any bones past shoulder, so just use head coords/visibility instead
			CG::cEntities.List[dwClient].pExt->cTags[dwTag] = CG::cEntities.List[dwClient].pExt->cTags[TAG_HEAD];		
			goto jSkip;
		}
		
		// Don't waste our time vis checking all bones if we have selected target mode on, unless they are prioritized
		if (!CG::cEntities.List[dwClient].pExt->bPreferred && dwTag != CG::dwAimBone && !CG::bTargetBestBone) {
			if (dwTag == TAG_HEAD) {
				// Always grab head visibility no matter what
				goto jContinue;
			}
			CG::cEntities.List[dwClient].pExt->cTags[dwTag].bVisible = false;
			//CG::cEntities.List[dwClient].pExt->cTags[dwTag].bBlockedByShield = false;
			goto jSkip;
		}
	jContinue:
		// Check if the bone is visible
		CG::cEntities.List[dwClient].pExt->cTags[dwTag].bVisible = GameFuncs::IsBoneVisible(dwTag, dwClient, iTagOffset);

	#if defined(BO2)
		if(!CG::cEntities.List[dwClient].pExt->cTags[dwTag].bVisible)
	#endif
		// Check if the point is blocked by a shield, so we know if its actually visible or not
		CG::cEntities.List[dwClient].pExt->cTags[dwTag].bBlockedByShield = GameFuncs::TraceHitShield(CG::iLocalClient, dwClient, dwTag, iTagOffset);


		//// Grab the hit location multiplier for the clients current weapon, on the bone in question
		//DWORD dwPartClassification = GameFuncs::XModelGetPartClassification(CG::cEntities.List[dwClient].pExt->pXModel, GameFuncs::GetBoneIndex(dwClient, CG::cTags.List[dwTag].uNum));
		//CG::cEntities.List[dwClient].pExt->cTags[dwTag].fDamageMult = GameFuncs::GetWeaponHitLocationMultiplier(dwPartClassification, CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon);

	#if defined(BO3)
		// Since I never figured out how to get the damage on bo3, just always prefer head if its visible and best is enabled
		if (dwTag == TAG_HEAD) {
			CG::cEntities.List[dwClient].pExt->cTags[dwTag].fDamageMult = 1.4f;
		}
	#endif
	jSkip:
		return;
	}
}

enum eTagOffsets {
	TAG_OFFSET_NONE,
	TAG_OFFSET_ALPHA,
	TAG_OFFSET_BETA
};
vec3_t GameFuncs::GetTagOffset(DWORD eTagIndex, DWORD dwClient, DWORD eTagOffset) {
	if (CG::cEntities.List[dwClient].pExt) {
		if (eTagOffset != 0) {
			vec3_t dir = CG::cEntities.List[dwClient].vAngles.negate(), vForward, vRight;
			AngleVectors(&dir, &vForward, &vRight, NULL);
			vec3_t vNewPos;
			TagInfo *cTag = &CG::cEntities.List[dwClient].pExt->cTags[eTagIndex];
			switch (eTagOffset) {
			case TAG_OFFSET_ALPHA:
				VectorMA((FLOAT*)&cTag->vPos.x, -4.0f, (FLOAT*)&vForward.x, (FLOAT*)&vNewPos.x);
				VectorMA((FLOAT*)&cTag->vPos.x, 2.0f, (FLOAT*)&vRight.x, (FLOAT*)&vNewPos.x);
				return vNewPos;
			case TAG_OFFSET_BETA:
				VectorMA((FLOAT*)&cTag->vPos.x, 4.0f, (FLOAT*)&vForward.x, (FLOAT*)&vNewPos.x);
				VectorMA((FLOAT*)&cTag->vPos.x, -2.0f, (FLOAT*)&vRight.x, (FLOAT*)&vNewPos.x);
				return vNewPos;
			}
		}
		return CG::cEntities.List[dwClient].pExt->cTags[eTagIndex].vPos;
	}
	return vec3_t();
}

vec3_t GetCustomTagPosition(DWORD eTagIndex, DWORD dwEntIndex) {
	vec3_t vTempPos;
	switch (eTagIndex) {
		case TAG_CUSTOM_SHIN_LE:
			// between 4 - 10, 50 %
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ANKLE_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_LE].vPos) / 2;
		case TAG_CUSTOM_SHIN2_LE:
			// between 4 - 10, 25 %
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ANKLE_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_LE].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ANKLE_LE].vPos + vTempPos) / 2;
		case TAG_CUSTOM_SHIN3_LE:
			// between 4 - 10, 75 %
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ANKLE_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_LE].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_LE].vPos + vTempPos) / 2;
		case TAG_CUSTOM_SHIN_RI:
			// between 5 - 11, 50 %
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ANKLE_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_RI].vPos) / 2;
		case TAG_CUSTOM_SHIN2_RI:
			// between 5 - 11, 50 %
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ANKLE_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_RI].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ANKLE_RI].vPos + vTempPos) / 2;
		case TAG_CUSTOM_SHIN3_RI:
			// between 5 - 11, 75 %
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ANKLE_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_RI].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_RI].vPos + vTempPos) / 2;
		case TAG_CUSTOM_THIGH_LE:
			// between 8 - 10, 25 %
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_LE].vPos) / 2;
			return (vTempPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_LE].vPos) / 2;
		case TAG_CUSTOM_THIGH2_LE:
			// between 8 - 10, 75 %
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_LE].vPos) / 2;
			return (vTempPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_LE].vPos) / 2;
		case TAG_CUSTOM_THIGH3_LE:
			// between 8 - 10, 50 %
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_LE].vPos) / 2;
		case TAG_CUSTOM_THIGH_RI:
			// between 9 - 11, 25 %
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_RI].vPos) / 2;
			return (vTempPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_RI].vPos) / 2;
		case TAG_CUSTOM_THIGH2_RI:
			// between 9 - 11, 75 %
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_RI].vPos) / 2;
			return (vTempPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_RI].vPos) / 2;
		case TAG_CUSTOM_THIGH3_RI:
			// between 9 - 11, 50 %
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_KNEE_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_RI].vPos) / 2;
		case TAG_CUSTOM_CHEST_LE:
			// between left shoudler and hip, 50%
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_LE].vPos) / 2;
		case TAG_CUSTOM_CHEST2_LE:
			// between left shoudler and hip, 25%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_LE].vPos) / 2;
			return (vTempPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_LE].vPos) / 2;
		case TAG_CUSTOM_CHEST3_LE:
			// between left shoudler and hip, 25%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_LE].vPos) / 2;
			return (vTempPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_LE].vPos) / 2;
		case TAG_CUSTOM_CHEST_RI:
			// between left shoudler and hip, 50%
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_RI].vPos) / 2;
		case TAG_CUSTOM_CHEST2_RI:
			// between left shoudler and hip, 25%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_RI].vPos) / 2;
			return (vTempPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_RI].vPos) / 2;
		case TAG_CUSTOM_CHEST3_RI:
			// between left shoudler and hip, 25%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_HIP_RI].vPos) / 2;
			return (vTempPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_RI].vPos) / 2;
		case TAG_CUSTOM_CHEST_UPPER:
			// between neck and spineupper, 50%
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_NECK].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SPINEUPPER].vPos) / 2;
		case TAG_CUSTOM_CHEST_UPPER2:
			// between neck and spineupper, 75%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_NECK].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SPINEUPPER].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_NECK].vPos + vTempPos) / 2;
		case TAG_CUSTOM_CHEST_UPPER3:
			// between neck and spineupper, 25%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_NECK].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SPINEUPPER].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SPINEUPPER].vPos + vTempPos) / 2;
		case TAG_CUSTOM_FOREARM_LE:
			// between left wrist and elbow, 50%
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_WRIST_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_LE].vPos) / 2;
		case TAG_CUSTOM_FOREARM2_LE:
			// between left wrist and elbow, 25%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_WRIST_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_LE].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_WRIST_LE].vPos + vTempPos) / 2;
		case TAG_CUSTOM_FOREARM3_LE:
			// between left wrist and elbow, 75%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_WRIST_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_LE].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_LE].vPos + vTempPos) / 2;
		case TAG_CUSTOM_ARM_LE:
			// between left elbow and shoulder, 50%
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_LE].vPos) / 2;
		case TAG_CUSTOM_ARM2_LE:
			// between left elbow and shoulder, 25%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_LE].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_LE].vPos + vTempPos) / 2;
		case TAG_CUSTOM_ARM3_LE:
			// between left elbow and shoulder, 75%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_LE].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_LE].vPos + vTempPos) / 2;
		case TAG_CUSTOM_FOREARM_RI:
			// between right wrist and elbow, 50%
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_WRIST_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_RI].vPos) / 2;
		case TAG_CUSTOM_FOREARM2_RI:
			// between right wrist and elbow, 25%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_WRIST_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_RI].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_WRIST_RI].vPos + vTempPos) / 2;
		case TAG_CUSTOM_FOREARM3_RI:
			// between right wrist and elbow, 75%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_WRIST_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_RI].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_RI].vPos + vTempPos) / 2;
		case TAG_CUSTOM_ARM_RI:
			// between right elbow and shoulder, 50%
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_RI].vPos) / 2;
		case TAG_CUSTOM_ARM2_RI:
			// between right elbow and shoulder, 25%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_RI].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_RI].vPos + vTempPos) / 2;
		case TAG_CUSTOM_ARM3_RI:
			// between right elbow and shoulder, 75%
			vTempPos = (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_RI].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_ELBOW_RI].vPos) / 2;
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_SHOULDER_RI].vPos + vTempPos) / 2;
		case TAG_CUSTOM_DICK:
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_CUSTOM_THIGH2_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_CUSTOM_THIGH2_RI].vPos) / 2;
		case TAG_CUSTOM_DICK2:
			return (CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_CUSTOM_THIGH3_LE].vPos + CG::cEntities.List[dwEntIndex].pExt->cTags[TAG_CUSTOM_THIGH3_RI].vPos) / 2;

	}
	return vec3_t();
}

vec3_t *GameFuncs::GetTagPosition(DWORD eTagIndex, DWORD dwEntIndex, vec3_t *pOutLocation, BOOL bCheckVis) {
	if (dwEntIndex >= 0 && dwEntIndex < __MAXENTITIES__) {
		if (CG::cTags.List[eTagIndex].pName[0] == 'n' && CG::cTags.List[eTagIndex].pName[1] == '_') {
			*pOutLocation = GetCustomTagPosition(eTagIndex, dwEntIndex);
			return pOutLocation;
		}
	#if defined(AW)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.AimAssist_GetTagPos))(0, CG::cEntities.List[dwEntIndex].pCentity, CG::cTags.List[eTagIndex].uNum, pOutLocation);
	#elif defined(BO3)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.AimAssist_GetTagPos))(CG::cEntities.List[dwEntIndex].pCentity, CG::cTags.List[eTagIndex].uNum, pOutLocation);
	#else
		DWORD pDObj = Get_ClientDObj(dwEntIndex);
		if (!pDObj) {
			*pOutLocation = GetEntityLocation(dwEntIndex);
			goto jEnd;
		}
		static jmp_buf tagPosJmp;
		int val = setjmp(tagPosJmp);
		if (!val) {
			try {
				((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_DObjGetWorldTagPos))(CG::cEntities.List[dwEntIndex].pCentity, pDObj, CG::cTags.List[eTagIndex].uNum, pOutLocation);
				//if (eTagIndex <= TAG_SHOULDER_RI) {
				//	pOutLocation->z -= 0.8f;
				//}
			}
			catch (...) {
				//printf("Crash in GetTagPos!\n");
				longjmp(tagPosJmp, 420);
			}
		}
		if (pOutLocation->IsZero()) {
			*pOutLocation = GetEntityLocation(dwEntIndex);
		}
	jEnd:
	#endif
		// Helmet is a bit too high up... subtract a bit so its more accurate :)
		if (eTagIndex == TAG_HELMET) {
			pOutLocation->z -= 1.0f;
		}

		//// Run our visibility checks, if we have chosen to.
		//if (bCheckVis) {
		//	RunVisibilityChecks(dwEntIndex, eTagIndex);
		//}
	}
	return pOutLocation;
}

SHORT GameFuncs::GetEntityTeam(DWORD dwEntIndex) {
#if defined(AW) || defined(GHOSTS)
	return *(int*)(CG::cEntities.List[dwEntIndex].pClientInfo + 0xc);
#else
#if defined(WAW) || defined(BO1) || defined(BO2)
	if (!CG::cEntities.List[dwEntIndex].bPlayer) {
		return *(int*)(CG::cEntities.List[dwEntIndex].pCentity + decryptDWORD(Not(NiNJA.cPatches)->CoD.EntityTeamAddress)); //Grab dog team
	}
#endif
	return *(int*)(CG::cEntities.List[dwEntIndex].pClientInfo + 0x2c);
#endif
}

VOID GameFuncs::SetEntityTeam(DWORD dwEntIndex) {
#if defined(MW2) || defined(MW3)
	*(int*)(CG::cEntities.List[dwEntIndex].pClientInfo + 0x2c) = CG::dwExpectedTeam;
#endif
}

PCHAR pShieldBack = NULL;
DWORD GameFuncs::GetEntityWeapon(DWORD dwClient, BOOL bPrimary) {
	if (!pShieldBack) {
		pShieldBack = EA("tag_shield_back");
	}
#if defined(MW2)
	if (bPrimary) {
		return *(short*)(CG::cEntities.List[dwClient].pCentity + 0x19c);
	}
	else if(dwClient < __MAXCLIENTS__) {
		if (Compare((PCHAR)(CG::cEntities.List[dwClient].pClientInfo + 0x258), pShieldBack) == TRUE) {
			return CG::ShieldCache[0];
		}
	}
#elif defined(BO2)
	if (bPrimary) {
		return *(int*)(CG::cEntities.List[dwClient].pCentity + 0x1b0);
	}
	else {
		return *(DWORD*)(CG::cEntities.List[dwClient].pCentity + 0x1b4);
	}
#elif defined(WAW)
	return *(int*)(CG::cEntities.List[dwClient].pCentity + 0x244);
#elif defined(MW1)
	return *(int*)(CG::cEntities.List[dwClient].pCentity + 0x184);
#elif defined(MW3)
	if (bPrimary) {
		return *(int*)(CG::cEntities.List[dwClient].pCentity + 0x18c);
	}
	else if (dwClient < __MAXCLIENTS__) {
		if (Compare((PCHAR)(CG::cEntities.List[dwClient].pClientInfo + 0x260), pShieldBack) == TRUE) {
			return CG::ShieldCache[0];
		}
	}
#elif defined(GHOSTS)
	if (bPrimary) {
		return *(int*)(CG::cEntities.List[dwClient].pCentity + 0x188);
	}
	else {
		return *(int*)(CG::cEntities.List[dwClient].pCentity + 0x12c);
	}
#elif defined(AW)
	if (bPrimary) {
		return *(int*)(CG::cEntities.List[dwClient].pCentity + 0x124); //0x126);
	}
	else if (dwClient < __MAXCLIENTS__) {
		if (Compare((PCHAR)(CG::cEntities.List[dwClient].pClientInfo + 0x7e0), pShieldBack) == TRUE) {
			return CG::ShieldCache[1];
	}
}
#elif defined(BO1)
	return *(short*)(CG::cEntities.List[dwClient].pCentity + 0x1c4);
#elif defined(BO3)
	return *(int*)(CG::cEntities.List[dwClient].pCentity + 0x1f8); //tu3 0x204);
#endif
	return 0;
}

SHORT GameFuncs::GetEntityType(DWORD dwClient) {
	if (CG::cEntities.List[dwClient].pCentity) {
		return (SHORT)*(BYTE*)(CG::cEntities.List[dwClient].pCentity + 0x2);
	}
	return 0; // ET_GENERAL
}

int GameFuncs::GetLocalClientIndex_Internal() {
#if defined(MW1)
	DWORD clientActive_t = Get_ClientActive_t();
	if (clientActive_t)
		return *(int*)(clientActive_t + 0x104);
#elif defined(MW2)
	DWORD cg_t = Get_CG_t();
	if (cg_t)
		return *(int*)(cg_t + 0x104);
#elif defined(MW3)
	DWORD clientActive_t = Get_ClientActive_t();
	if (clientActive_t)
		return *(int*)(clientActive_t + 0x1d0);
#elif defined(GHOSTS)
	DWORD cg_t = Get_CG_t();
	if (cg_t)
		return *(int*)(cg_t + 0x17c);
#elif defined(AW)
	DWORD cg_t = Get_CG_t();
	if (cg_t)
		return (int)*(byte*)cg_t;
#elif defined(WAW)
	DWORD clientActive_t = Get_ClientActive_t();
	if (clientActive_t)
		return *(int*)(clientActive_t + 0x120);
#elif defined(BO1)
	DWORD clientActive_t = Get_ClientActive_t();
	if (clientActive_t)
		return (int)*(byte*)(clientActive_t + 0x158);
#elif defined(BO2)
	DWORD clientActive_t = Get_ClientActive_t();
	if (clientActive_t)
		return *(int*)(clientActive_t + 0x210);
#elif defined(BO3)
	DWORD clientActive_t = Get_ClientActive_t();
	if (clientActive_t)
		return *(int*)(clientActive_t + 0xa0);
#endif
	return -1;
}

/* Gets the ingame clientIndex corresponding to controller 0 */
BOOL GameFuncs::GetLocalClientIndex() {
	int iTempIndex = GameFuncs::GetLocalClientIndex_Internal();
	if (iTempIndex >= 0 && iTempIndex < __MAXCLIENTS__) {
		bool bFreeOld = false;
		if (CG::iLocalClient != iTempIndex) {
			if (!CG::cEntities.List[iTempIndex].pExt) {
				if (CG::cEntities.List[iTempIndex].AllocExtendedEntity()) {
					bFreeOld = true;
				}
			}
			int iOldIndex = CG::iLocalClient;
			CG::iLocalClient = iTempIndex;
			DbgPrint("Our local client index has updated!");
			if (bFreeOld) {
				CG::cEntities.List[iOldIndex].FreeExtendedEntity();
				DbgPrint("Success freeing old entity!");
			}
		}
		return TRUE;
	}
	return FALSE;
}

int GameFuncs::GetLocalClientHealth() {
#if !defined(BO3)
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
#else
	DWORD clientActive_t = Get_ClientActive_t();
	if(clientActive_t) {
#endif
#if defined(MW1)
		return *(int*)(cg_t + 0x180);
#elif defined(MW2)
		return *(int*)(cg_t + 0x150);
#elif defined(WAW) || defined(MW3)
		return *(int*)(cg_t + 0x19c);
#elif defined(GHOSTS)
		return *(int*)(cg_t + 0x1c8);
#elif defined(AW)
		return  *(int*)(cg_t + 0x1e0);
#elif defined(BO1)
		return *(int*)(cg_t + 0x254);
#elif defined(BO2)
		return *(int*)(cg_t + 0x2c8);
#elif defined(BO3)
		return *(int*)(clientActive_t + 0x370);
#endif
	}
	return 0;
}

int GameFuncs::GetStaticLocalClientIndex() {
#if defined(MW1) || defined(WAW) || defined(BO1) || defined(BO2) || defined(BO3)
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		return *(DWORD*)cg_t; //((int(*)(...))0x82308F00)(0); // CG_GetClientNumForLocalClient
	}
#elif defined(MW2)
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		return *(DWORD*)(cg_t + 0x33A8);
	}
#elif defined(MW3)
	DWORD cg_t = Get_UnknownClientStruct();
	if (cg_t) {
		return *(DWORD*)(cg_t + 0x3334);
	}
#elif defined(GHOSTS)
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		return *(DWORD*)(cg_t + 0x2FB4);
	}
#elif defined(AW)
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		return (int)*(BYTE*)(cg_t + 0x3234);
	}
#endif
	return -1;
}

vec3_t *GameFuncs::GetRefDefViewOrigin() {
	DWORD refDef = Get_RefDef_t();
	if (refDef) {
#if defined(WAW)
		return (vec3_t*)(refDef + 0x4350);
#elif defined(MW1)
		return (vec3_t*)(refDef + 0x6C78);
#elif defined(MW2)
		return (vec3_t*)(refDef + 0x3F84);// 0x18);// 0x98C4);
#elif defined(MW3)
		return (vec3_t*)(refDef + 0xC0E4);
#elif defined(BO1)
		return (vec3_t*)(refDef + /*0x22F04*/0x2c);
#elif defined(BO2)
		return (vec3_t*)(refDef + 0x401A20);
#elif defined(BO3)
		return (vec3_t*)(refDef + 0x1BD58);
#elif defined(GHOSTS)
		return (vec3_t*)(refDef + 0x18);// 0xccf4);
#elif defined(AW)
		return (vec3_t*)(refDef + 0x70E8);
#endif
	}
	return 0;
}

vec3_t GameFuncs::GetPlayerViewOrigin(DWORD cg_t) {
	vec3_t vOut;
	jmp_buf jGetLocalViewOriginJump;
	int val = setjmp(jGetLocalViewOriginJump);
	if (!val) {
		try {
			((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_GetPlayerViewOrigin))(0, cg_t, &vOut);
		}
		catch (...) {
			longjmp(jGetLocalViewOriginJump, 420);
		}
	}
	return vOut;
}

vec3_t GameFuncs::CL_GetPredictedOriginForServerTime(int iTime) {
	int clientActive = Get_ClientActive_t();
	if (clientActive) {
	#if defined(MW2)
		vec3_t vPredictedOrigin, vPredictedVelocity;
		int iBobCycle = 0, iMovementDir = 0;
		if (((BOOL(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_GetPredictedOriginForServerTime))(clientActive, iTime, &vPredictedOrigin, &vPredictedVelocity, &iBobCycle, &iMovementDir)) {
			return vPredictedOrigin;
		}
	#elif defined(BO2)
		BYTE pOutput[100];
		if (((BOOL(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_GetPredictedOriginForServerTime))(clientActive, iTime, &pOutput)) { //bo2
			return *(vec3_t*)((int)pOutput + 0x28);
		}
	#endif
	}
	return vec3_t();
}

vec3_t GameFuncs::GetLocalViewOrigAtTime(int iTime) {
	if (CG::iRealLocalClient >= 0 && CG::iRealLocalClient < __MAXCLIENTS__ && CG::cEntities.List[CG::iRealLocalClient].bAlive) {
		DWORD cg_t = Get_UnknownClientStruct();
	jRetry:
		if (cg_t) {
			vec3_t vPredictedOrigin = CL_GetPredictedOriginForServerTime(iTime);
			if (!vPredictedOrigin.IsZero()) {
				return vPredictedOrigin + vec3_t(0, 0, fabsf(GetPlayerViewOrigin(cg_t).z - GameFuncs::GetEntityLocation(CG::iRealLocalClient).z));
			}
		}
		else {
			cg_t = Get_CG_t();
			if (cg_t) {
				goto jRetry;
			}
		}
	}
	return vec3_t();
}

vec3_t vCachedViewOrig(0, 0, 0);
vec3_t GameFuncs::GetLocalViewOrigin(BOOL bCached) {
	//if (bCached) {
	//	return vCachedViewOrig;
	//}
	//#if defined(MW2) || defined(BO2)
	//if (!CG::bInKillstreak) {
	//	vec3_t vPredictedOrigin = GameFuncs::GetLocalViewOrigAtTime(*(int*)UserCmd::GetOldUserCmd());
	//	if (!vPredictedOrigin.IsZero()) {
	//		vCachedViewOrig = vPredictedOrigin;
	//		return vCachedViewOrig;
	//	}
	//}
	//#endif

#if defined(MW2) || defined(BO1) || defined(GHOSTS) || defined(AW)
	if (!CG::bInKillstreak) {
#endif
		DWORD cg_t = Get_UnknownClientStruct();
		if (cg_t) {
		jCall:
			// Grab our local viewOrigin for use in out aimbot calculation
			vCachedViewOrig = GetPlayerViewOrigin(cg_t);
			return vCachedViewOrig;
		}
		else {
			cg_t = Get_CG_t();
			if (cg_t) {
				goto jCall;
			}
		}
#if defined(MW2) || defined(BO1)|| defined(GHOSTS) || defined(AW)
	}
	else {
		vec3_t *rd = GetRefDefViewOrigin();
		if (rd) {
			vCachedViewOrig = *rd;
			return vCachedViewOrig;
		}
	}
#endif
	return vec3_t();
}

int GameFuncs::GetObituaryAttacker(DWORD entityState) {
#if defined(MW1)
	return *(int*)(entityState + 0x138);
#elif defined(MW2)
	return *(int*)(entityState + 0x150);
#elif defined(MW3) || defined(GHOSTS)
	return *(int*)(entityState + 0x140);
#elif defined(BO1)
	return (int)*(byte*)(entityState + 0x2a3);
#elif defined(BO2)
	return (int)*(byte*)(entityState + 0x2b1);
#elif defined(BO3)
	return (int)*(byte*)(entityState + 0x38d);
#elif defined(AW)
	return (int)*(byte*)(entityState + 0xdd);
#elif defined(WAW)
	return *(int*)(entityState + 0x1f8);
#endif
}

int GameFuncs::GetObituaryVictim(DWORD entityState) {
#if defined(MW1)
	return *(int*)(entityState + 0x134);
#elif defined(MW2)
	return *(int*)(entityState + 0x14c);
#elif defined(MW3) || defined(GHOSTS)
	return *(int*)(entityState + 0x13c);
#elif defined(BO1)
	return (int)*(byte*)(entityState + 0x2a1);
#elif defined(BO2)
	return (int)*(byte*)(entityState + 0x2af);
#elif defined(BO3)
	return (int)*(byte*)(entityState + 0x38b);
#elif defined(AW)
	return (int)*(byte*)(entityState + 0xdb);
#elif defined(WAW)
	return *(int*)(entityState + 0x1f4);
#endif
}

int GameFuncs::GetReloadClientIndex(DWORD entityState) {
#if defined(MW1)
	return *(int*)(entityState + 0xc0);
#elif defined(WAW)
	return *(int*)(entityState + 0x15c);
#elif defined(MW2)
	return *(int*)(entityState + 0xd0);
#elif defined(MW3) || defined(GHOSTS)
	return *(int*)(entityState + 0xc4);
#elif defined(AW)
	return *(int*)(entityState + 0xd6);
#elif defined(BO1)
	return *(int*)(entityState + 0x1dc);
#elif defined(BO2)
	return *(short*)(entityState + 0x2c2);
#elif defined(BO3)
	return *(int*)(entityState + 0x258);
#endif
	return 0;
}

DWORD GameFuncs::GetPing() {
	if (CG::bInGameCached && CG::bLobbyInitialized) {
		DWORD dwPing = 0;
		DWORD cg_t = GameFuncs::Get_CG_t();
		if (cg_t) {
			DWORD dwSnap = NULL;
		#if defined(BO1)
			dwSnap = *(DWORD*)(cg_t + 0x28);
		#elif defined(BO2)
			dwSnap = *(DWORD*)(cg_t + 0x24);
		#endif
			if (dwSnap) {
				dwPing = *(DWORD*)(dwSnap + 0x4);
			}
		}
		if (dwPing == 0) {
			if (CG::cEntities.List[CG::iRealLocalClient].pExt) {
				dwPing = CG::cEntities.List[CG::iRealLocalClient].pExt->cScore.dwPing;
			}
		}
		return dwPing;
	}
	return 0;
}

DWORD GameFuncs::GetServerTime() {
	int cg_t;
#if defined(BO3)
	cg_t = Get_UnknownClientStruct();
	if (cg_t)
		return *(DWORD*)(cg_t + 0x4);
#elif defined(AW)
	cg_t = Get_CG_t();
	if (cg_t)
		return *(DWORD*)(cg_t + 0x4c);
#elif defined(MW2) || defined(GHOSTS)
	cg_t = Get_CG_t();
	if (cg_t)
		return *(DWORD*)(cg_t);
#else
	cg_t = Get_UnknownClientStruct();
	if (cg_t)
		return *(DWORD*)(cg_t);
#endif
	return 0;
}

bool GameFuncs::IsPointInBox(vec3_t vPoint, vec3_t vMins, vec3_t vMaxes) {
	return (vPoint.x <= vMaxes.x && vPoint.x >= vMins.x) && (vPoint.y <= vMaxes.y && vPoint.y >= vMins.y) && (vPoint.z <= vMaxes.z && vPoint.z >= vMins.z);
}

//bool GameFuncs::TraceHitShield(DWORD dwAttacker, DWORD dwClient, DWORD dwTagIndex, int iTagOffset, bool bSecondRun) {
//#if defined(MW2) || defined(MW3) || defined(BO2) || defined(GHOSTS) || defined(AW)
//	if (CG::cEntities.List[dwClient].pExt->bPrimaryShield || CG::cEntities.List[dwClient].pExt->bSecondaryShield) {
//		if (dwTagIndex == TAG_HEAD || dwTagIndex == TAG_NECK) {
//			return true;
//		}
//
//		// Grab our clients shieldinfo
//		ShieldInfo cShieldInfo = CG::cEntities.List[dwClient].pExt->oShieldInfo;
//
//		// We need to calculate the mins and maxes of the bounding box of the shield in order to run a line trace on it
//		vec3_t mins(FLT_MAX, FLT_MAX, FLT_MAX), maxes(-FLT_MAX, -FLT_MAX, -FLT_MAX);
//
//		// We'll use an inverted set of the bones axis in order to rotate the world in the opposite direction of the bone, so that we can run a visual trace on the smallest possible bounding box of the shield
//		vec3_t invertedAxis[3] = { -cShieldInfo.cOrientation.axis[0], -cShieldInfo.cOrientation.axis[1], -cShieldInfo.cOrientation.axis[2] };
//
//		// Rotate each of our points to match the axis alignment of the shield
//		for (DWORD i = 0; i < 4; i++) {
//			RotatePoint(&cShieldInfo.pBounds[i], invertedAxis);
//
//			// Store our mins and maxes for 2 of the 4 points, to use as our bounding box
//			if (i % 1 == 0) {
//				mins.SetIfLessThan(cShieldInfo.pBounds[i]);
//				maxes.SetIfGreaterThan(cShieldInfo.pBounds[i]);
//			}
//		}
//
//		// Grab the start and ending positions of the trace, and align them with the aabb of the shield
//		//vec3_t myOrig = GameFuncs::GetLocalViewOrigin(TRUE), theirOrig = CG::cEntities.List[dwClient].pExt->cTags[dwTagIndex].vPos;
//		vec3_t vStartPos, vEndPos;
//		if (!bSecondRun) {
//			// On the first run, run the trace from the attacker to the victim
//			vStartPos = CG::cEntities.List[dwAttacker].pExt->cTags[TAG_HEAD].vPos;
//			vEndPos = GetTagOffset(dwClient, dwTagIndex, iTagOffset);
//			if (dwAttacker == CG::iLocalClient) {
//				vStartPos = GameFuncs::GetLocalViewOrigin();
//			}
//		}
//		else {
//			// On the first run, run the trace from the attacker to the victim
//			vStartPos = GetTagOffset(dwClient, dwTagIndex, iTagOffset);
//			vEndPos = CG::cEntities.List[dwAttacker].pExt->cTags[TAG_HEAD].vPos; 
//			if (dwAttacker == CG::iLocalClient) {
//				vEndPos = GameFuncs::GetLocalViewOrigin();
//			}
//		}
//
//		// Rotate the points so they are aabb aligned
//		RotatePoint(&vStartPos, invertedAxis);
//		RotatePoint(&vEndPos, invertedAxis);
//
//		if (IsPointInBox(bSecondRun ? vStartPos : vEndPos, mins, maxes)) {
//			return true;
//		}
//
//		vec3_t hitPos = vec3_t();
//		// Check if our trace hit the bounding box we created
//		if (CheckLineBox(mins, maxes, vStartPos, vEndPos, hitPos)) {
//			return true;
//		}
//
//		// Try again in reverse if it didn't fail, to weed out bones that clipped through the shield check and were actually blocked
//		if (!bSecondRun) {
//			bSecondRun = true;
//			return TraceHitShield(dwAttacker, dwClient, dwTagIndex, iTagOffset, true);
//		}
//	}
//#endif
//	return false;
//}

DWORD dwDisabledTags[] = {
	TAG_ELBOW_LE,
	TAG_WRIST_LE,
	TAG_CUSTOM_FOREARM_LE,
	TAG_CUSTOM_FOREARM2_LE,
	TAG_CUSTOM_FOREARM3_LE,
	TAG_INDEX_LE1,
	TAG_INDEX_RI1,
	TAG_MID_LE1,
	TAG_MID_RI1,
	TAG_PINKY_LE1,
	TAG_PINKY_RI1,
	TAG_RING_LE1,
	TAG_RING_RI1,
	TAG_THUMB_LE1,
	TAG_THUMB_RI1
};

bool IsDisabledTag(DWORD dwTagIndex) {
	for (int i = 0; i < ARRAY_SIZE(dwDisabledTags); i++) {
		if (dwTagIndex == dwDisabledTags[i]) {
			return true;
		}
	}
	return false;
}

bool GameFuncs::TraceHitShield(DWORD dwAttacker, DWORD dwClient, DWORD dwTagIndex, int iTagOffset, bool bSecondRun) {
#if defined(MW2) || defined(MW3) || defined(BO2) || defined(GHOSTS) || defined(AW)
	if (CG::cEntities.List[dwClient].pExt->bPrimaryShield || CG::cEntities.List[dwClient].pExt->bSecondaryShield) {
		// Grab our clients shieldinfo
		ShieldInfo cShieldInfo = CG::cEntities.List[dwClient].pExt->oShieldInfo;

		// We need to calculate the mins and maxes of the bounding box of the shield in order to run a line trace on it
		vec3_t mins(FLT_MAX, FLT_MAX, FLT_MAX), maxes(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		// We'll use an inverted set of the bones axis in order to rotate the world in the opposite direction of the bone, so that we can run a visual trace on the smallest possible bounding box of the shield
		vec3_t invertedAxis[3] = { -cShieldInfo.cOrientation.axis[0], -cShieldInfo.cOrientation.axis[1], -cShieldInfo.cOrientation.axis[2] };

		// Rotate each of our points to match the axis alignment of the shield
		for (DWORD i = 0; i < 4; i++) {
			RotatePoint(&cShieldInfo.pBounds[i], invertedAxis);

			// Store our mins and maxes for 2 of the 4 points, to use as our bounding box
			if (i % 1 == 0) {
				mins.SetIfLessThan(cShieldInfo.pBounds[i]);
				maxes.SetIfGreaterThan(cShieldInfo.pBounds[i]);
			}
		}

		// Grab the start and ending positions of the trace, and align them with the aabb of the shield
		//vec3_t myOrig = GameFuncs::GetLocalViewOrigin(TRUE), theirOrig = CG::cEntities.List[dwClient].pExt->cTags[dwTagIndex].vPos;
		vec3_t vStartPos = CG::cEntities.List[dwAttacker].pExt->cTags[TAG_HEAD].vPos, vEndPos = GetTagOffset(dwTagIndex, dwClient, iTagOffset);
		if (dwAttacker == CG::iLocalClient) {
			vStartPos = GameFuncs::GetLocalViewOrigin();
		}

		RotatePoint(&vStartPos, invertedAxis);
		RotatePoint(&vEndPos, invertedAxis);

		vec3_t hitPos;
		// Check if our trace hit the bounding box we created
		if (CheckLineBox(mins, maxes, vStartPos, vEndPos, hitPos)) {
			return true;
		}
		// Left wrist clips through the shield, so we will just ALWAYS consider the trace as having hit the shield if they have a primary shield
		if (CG::cEntities.List[dwClient].pExt->bPrimaryShield && IsDisabledTag(dwTagIndex)) {
			//printf(va("Bone: %s, bPrimaryShield: %s\n", CG::cTags.List[dwTagIndex].pName, CG::cEntities.List[dwClient].pExt->bPrimaryShield ? "true" : "false"));
			return true;
		}
	}
#endif
	return false;
}

int iShieldIndices[4] = { -1,-1,-1,-1 };
VOID GameFuncs::CacheShield() {
	CG::ShieldCache[0] = GameFuncs::GetWeaponIndexForName(ES("riotshield_mp"));//GameOffsets.RiotShield);
#if defined(AW)
	CG::ShieldCache[1] = GameFuncs::GetWeaponIndexForName(ES("iw5_riotshield_mp"));
	CG::ShieldCache[2] = GameFuncs::GetWeaponIndexForName(ES("iw5_riotshieldt6_mp"));
	CG::ShieldCache[3] = GameFuncs::GetWeaponIndexForName(ES("iw5_riotshieldt6loot0_mp"));
	CG::ShieldCache[4] = GameFuncs::GetWeaponIndexForName(ES("iw5_riotshieldt6loot1_mp"));
	CG::ShieldCache[5] = GameFuncs::GetWeaponIndexForName(ES("iw5_riotshieldt6loot2_mp"));
	CG::ShieldCache[6] = GameFuncs::GetWeaponIndexForName(ES("iw5_riotshieldt6loot3_mp"));
	CG::ShieldCache[7] = GameFuncs::GetWeaponIndexForName(ES("exoshield_equipment_mp"));
#endif
	DbgPrintE("RS1: 0x%X, RS2: 0x%X, RS3: 0x%X, RS4: 0x%X, RS5: 0x%X, RS6: 0x%X, RS7: 0x%X, RS8: 0x%X\n",
		CG::ShieldCache[0], CG::ShieldCache[1], CG::ShieldCache[2], CG::ShieldCache[3], CG::ShieldCache[4], CG::ShieldCache[5], CG::ShieldCache[6], CG::ShieldCache[7]);
}

PCHAR pShieldNames[4] = { 0,0,0,0 };
ShieldInfo GameFuncs::GetShieldInfo(DWORD dwClient) {
#if defined(ShieldRearLeft)
	DWORD pDObj = GameFuncs::Get_ClientDObj(dwClient);
	if (pDObj) {
		DWORD dwBoneIndex = NULL;
		if (!CG::cEntities.List[dwClient].pExt->bSecondaryShield) {
		#if defined(GHOSTS) 
			if (pShieldNames[0] == 0) {
				pShieldNames[0] = EA("tag_weapon_right");
			}
			dwBoneIndex = GetBoneIndex(dwClient, pShieldNames[0]);
		#else

		#if defined(AW)
			if (CG::cEntities.List[dwClient].pExt->dwCurrentWeapon == CG::ShieldCache[7]) {
				if (pShieldNames[1] == 0) {
					pShieldNames[1] = EA("tag_inhand");
				}
				dwBoneIndex = GetBoneIndex(dwClient, pShieldNames[1]);
			}
			else
		#endif
				if (pShieldNames[2] == 0) {
					pShieldNames[2] = EA("tag_weapon_left");
				}
				dwBoneIndex = GetBoneIndex(dwClient, pShieldNames[2]);
		#endif
		}
		else {
			if (pShieldNames[3] == 0) {
				pShieldNames[3] = EA("tag_stowed_back");
			}
			dwBoneIndex = GetBoneIndex(dwClient, pShieldNames[3]);
		}

		ShieldInfo cShieldInfo;
		// First, grab the orientation of our shield bone
		cShieldInfo.cOrientation = GetBoneOrientation(dwClient, dwBoneIndex);

		vec3_t boneAngles, forward, right, up;
		// Convert the axis to angles so we can then convert to forward, right, and up vectors
		AxisToAngles(cShieldInfo.cOrientation.axis, &boneAngles);

		FLOAT shieldLeft, shieldRight, shieldUp, shieldDown, shieldDepthBottomRight, shieldDepthBottomLeft, shieldDepthTopLeft, shieldDepthTopRight, shieldAngleXTweak, shieldAngleYTweak;
		// The shields mounted position is different depending on whether or not its the primary or secondary weapon, so we have multiple static points to handle this
		if (CG::cEntities.List[dwClient].pExt->bSecondaryShield) {
			// Grab our rear shield offsets
			shieldLeft = ShieldRearLeft;
			shieldRight = ShieldRearRight;
			shieldUp = ShieldRearUp;
			shieldDown = ShieldRearDown;

			shieldDepthBottomRight = ShieldRearDepthBottomRight;
			shieldDepthBottomLeft = ShieldRearDepthBottomLeft;
			shieldDepthTopLeft = ShieldRearDepthTopLeft;
			shieldDepthTopRight = ShieldRearDepthTopRight;

			shieldAngleXTweak = ShieldRearXTweak;
			shieldAngleYTweak = ShieldRearYTweak;
		}
		else {
			// Grab our front shield offsets
			shieldLeft = ShieldFrontLeft;
			shieldRight = ShieldFrontRight;
			shieldUp = ShieldFrontUp;
			shieldDown = ShieldFrontDown;

			shieldDepthBottomRight = ShieldFrontDepthBottomRight;
			shieldDepthBottomLeft = ShieldFrontDepthBottomLeft;
			shieldDepthTopLeft = ShieldFrontDepthTopLeft;
			shieldDepthTopRight = ShieldFrontDepthTopRight;

			shieldAngleXTweak = ShieldFrontXTweak;
			shieldAngleYTweak = ShieldFrontYTweak;
		}

		// Apply our angular tweaks for games that are fucking gay as shit
		boneAngles.x = boneAngles.x + shieldAngleXTweak;
		boneAngles.y = boneAngles.y + shieldAngleYTweak;

		// Convert our angles back an axis after making our changes
		AngleVectors(&boneAngles, &forward, &right, &up);

		// Setup our condition to switch between different shield orientations
		bool bDrawNormal = !CG::cEntities.List[dwClient].pExt->bSecondaryShield;

	#if defined(AW)
		bDrawNormal = true;
	#endif

		vec3_t start = cShieldInfo.cOrientation.origin, topRight, topLeft, bottomRight, bottomLeft, tVal;
		// Top right
		VectorMA((FLOAT*)&start.x, shieldRight, bDrawNormal ? (FLOAT*)&right.x : (FLOAT*)&forward.x, (FLOAT*)&topRight.x);
		VectorMA((FLOAT*)&topRight.x, shieldUp, (FLOAT*)&up.x, (FLOAT*)&topRight.x);
		VectorMA((FLOAT*)&topRight.x, shieldDepthTopRight, bDrawNormal ? (FLOAT*)&forward.x : (FLOAT*)&right.x, (FLOAT*)&topRight.x);

		// Bottom right
		VectorMA((FLOAT*)&start.x, shieldRight, bDrawNormal ? (FLOAT*)&right.x : (FLOAT*)&forward.x, (FLOAT*)&bottomRight.x);
		VectorMA((FLOAT*)&bottomRight.x, -shieldDown, (FLOAT*)&up.x, (FLOAT*)&bottomRight.x);
		VectorMA((FLOAT*)&bottomRight.x, shieldDepthBottomRight, bDrawNormal ? (FLOAT*)&forward.x : (FLOAT*)&right.x, (FLOAT*)&bottomRight.x);

		// Top left
		VectorMA((FLOAT*)&start.x, -shieldLeft, bDrawNormal ? (FLOAT*)&right.x : (FLOAT*)&forward.x, (FLOAT*)&topLeft.x);
		VectorMA((FLOAT*)&topLeft.x, shieldUp, (FLOAT*)&up.x, (FLOAT*)&topLeft.x);
		VectorMA((FLOAT*)&topLeft.x, shieldDepthTopLeft, bDrawNormal ? (FLOAT*)&forward.x : (FLOAT*)&right.x, (FLOAT*)&topLeft.x);

		// Bottom left
		VectorMA((FLOAT*)&start.x, -shieldLeft, bDrawNormal ? (FLOAT*)&right.x : (FLOAT*)&forward.x, (FLOAT*)&bottomLeft.x);
		VectorMA((FLOAT*)&bottomLeft.x, -shieldDown, (FLOAT*)&up.x, (FLOAT*)&bottomLeft.x);
		VectorMA((FLOAT*)&bottomLeft.x, shieldDepthBottomLeft, bDrawNormal ? (FLOAT*)&forward.x : (FLOAT*)&right.x, (FLOAT*)&bottomLeft.x);

		cShieldInfo.pBounds[0] = topRight;
		cShieldInfo.pBounds[1] = topLeft;
		cShieldInfo.pBounds[2] = bottomLeft;
		cShieldInfo.pBounds[3] = bottomRight;

		return cShieldInfo;
	}
#endif
	return ShieldInfo();
}

XBoneInfo *GameFuncs::XModelGetBoneInfo(DWORD pXModel, SHORT sModelIndex) {
	return (XBoneInfo*)(*(DWORD*)(pXModel + 0xa4) + (sModelIndex * 0x2c));
}

DWORD GameFuncs::GetXModelForDObj(DWORD pDObj) {
	return *(DWORD*)*(DWORD*)(pDObj + decryptDWORD(Not(NiNJA.cPatches)->CoD.XModelOffset));
}

DWORD GameFuncs::XModelGetNumBones(DWORD pXModel) {
#if defined(BO3)
	DWORD num = (DWORD)*(BYTE*)(pXModel + 0xb8);
	if (num == 0) {
		num = (DWORD)*(BYTE*)(pXModel + 0xbc);
	}
	return num;
#else
	return (DWORD)*(BYTE*)(pXModel + 0x4);
#endif
}

USHORT GameFuncs::XModelGetBone(DWORD pXModel, SHORT sModelIndex) {
	return *(USHORT*)(*(DWORD*)(pXModel + decryptDWORD(Not(NiNJA.cPatches)->CoD.XModelBoneOffset)) + (sModelIndex * 2));
}

PCHAR GameFuncs::XModelGetBoneName(DWORD pXModel, SHORT sBoneIndex) {
	return SL_ConvertToString(XModelGetBone(pXModel, sBoneIndex));
}

PCHAR GameFuncs::SL_ConvertToString(USHORT sIndex) {
	if (sIndex) {
	#if defined(BO3)
		return (PCHAR)(*(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.SL_ConvertToString) + (sIndex * 0x14) + 0x4);
	#else
		return ((char*(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.SL_ConvertToString))(sIndex);
	#endif
	}
	return 0;
}

DWORD GameFuncs::XModelGetPartClassification(DWORD pXModel, SHORT sModelIndex) {
	return (DWORD)*(BYTE*)(*(DWORD*)(pXModel + decryptDWORD(Not(NiNJA.cPatches)->CoD.XModelPartClassification)) + sModelIndex);
}

DWORD GameFuncs::GetBoneIndex(DWORD dwClient, USHORT uTag) {
	if (CG::cEntities.List[dwClient].pExt->pXModel) {
		DWORD numBones = XModelGetNumBones(CG::cEntities.List[dwClient].pExt->pXModel);
		for (USHORT i = 0; i < numBones; i++) {
			if (uTag == XModelGetBone(CG::cEntities.List[dwClient].pExt->pXModel, i)) {
				return i;
			}
		}
	}
	return 0;
}

DWORD GameFuncs::GetBoneIndex(DWORD dwClient, PCHAR pTag) {
	if (CG::cEntities.List[dwClient].pExt->pXModel) {
		DWORD numBones = XModelGetNumBones(CG::cEntities.List[dwClient].pExt->pXModel);
		for (USHORT i = 0; i < numBones; i++) {
			PCHAR pBoneName = XModelGetBoneName(CG::cEntities.List[dwClient].pExt->pXModel, i);
			if (pBoneName) {
				if (Compare(pBoneName, pTag)) {
					return i;
				}
			}
		}
	}
	return 0;
}
//
//DWORD GameFuncs::GetTagHitLocationForDObj(DWORD dwTagIndex, DWORD pDObj) {
//	if (pDObj) {
//		//DbgPrint("DObj: %X", pDObj);
//		int modelInd = 0;
//		DWORD pXModel = GetXModelForDObj(pDObj);
//		if (pXModel) {
//			//DbgPrint("XModel: %X", pXModel);
//			DWORD numBones = XModelGetNumBones(pXModel);
//			DbgPrintE("---------------------------Start dump (pModel: %X, pBoneInfo: %X)---------------------------", pXModel, *(DWORD*)(pXModel + 0x38));
//			for (USHORT i = 0; i < numBones; i++) {
//				PCHAR pName = XModelGetBoneName(pXModel, i);
//				if (pName) {
//					if (Compare(pName, CG::cTags.List[dwTagIndex].pName)) {
//						DWORD dwPartClassification = XModelGetPartClassification(pXModel, i);
//						if (dwPartClassification != 0) {
//							DbgPrintE("Bone %s (index %i) surface %X is classified as %i", pName, i, *(SHORT*)(*(DWORD*)(pXModel + 0x2c) + (i*2)), dwPartClassification);
//
//							
//				
//							return dwPartClassification;//;
//						}
//					}
//				}
//			}
//			DbgPrint("---------------------------End dump---------------------------");
//		}
//	}
//	return 0;
//}

orientation_t GameFuncs::GetBoneOrientation(DWORD dwClient, SHORT sModelIndex) {
	orientation_t out;
	jmp_buf jOrientationJump;
	int val = setjmp(jOrientationJump);
	if (!val) {
		try {
			((BOOL*(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.FX_GetBoneOrientation))(0, dwClient, sModelIndex, &out);
		}
		catch (...) {
			longjmp(jOrientationJump, 420);
		}
	}
	return out;
}

int GameFuncs::GetWeaponSwitchClientIndex(DWORD entityState) {
#if defined(WAW)
	return *(int*)(entityState + 0x15c);
#elif defined(MW1)
	return *(int*)(entityState + 0xc0);
#elif defined(MW2)
	return *(int*)(entityState + 0xd0);
#elif defined(MW3) || defined(GHOSTS)
	return *(int*)(entityState + 0xc4);
#elif defined(AW)
	return *(int*)(entityState + 0xd6);
#elif defined(BO1)
	return *(int*)(entityState + 0x8);
#elif defined(BO2)
	return *(short*)(entityState + 0x2c2);
#elif defined(BO3)
	return *(int*)(entityState + 0x258);
#endif
}

vec3_t *GameFuncs::GetSpawnAngles() {
	DWORD clientActive_t = Get_ClientActive_t();
	if (clientActive_t) {
	#if defined(WAW)
		return (vec3_t*)(clientActive_t + 0xa4);
	#elif defined(MW1)
		return (vec3_t*)(clientActive_t + 0x88);
	#elif defined(MW2) || defined(MW3)
		return (vec3_t*)(clientActive_t + 0xe0);
	#elif defined(GHOSTS)
		return (vec3_t*)(clientActive_t + 0xe4);
	#elif defined(BO1)
		return (vec3_t*)(clientActive_t + 0xac);
	#elif defined(BO2)
		return (vec3_t*)(clientActive_t + 0x108);
	#elif defined(BO3)
		return (vec3_t*)(clientActive_t + 0x15c);
	#elif defined(AW)
		return (vec3_t*)(clientActive_t + 0x110);
	#endif
	}
	return 0;
}

VOID GameFuncs::GetSpreadForWeapon(DWORD dwWeaponIndex, FLOAT *fMinSpread, FLOAT *fMaxSpread) {
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
	#if defined(MW1) || defined(WAW) || defined(BO1)
		cg_t = Get_UnknownClientStruct();
		if (cg_t) {
			((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetSpreadForWeapon))(cg_t, GetWeaponDef(CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon), fMinSpread, fMaxSpread);
		}
	#elif  defined(MW3) || defined(BO2) || defined(BO3)
		cg_t = Get_UnknownClientStruct();
		if (cg_t) {
			((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetSpreadForWeapon))(cg_t, CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon, fMinSpread, fMaxSpread);
		}
	#elif defined(GHOSTS)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetSpreadForWeapon))(cg_t, CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon, fMinSpread, fMaxSpread);
	#elif defined(MW2)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetSpreadForWeapon))(cg_t, GetWeaponDef(CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon), fMinSpread, fMaxSpread);
	#elif defined(AW)
		((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetSpreadForWeapon))(cg_t, fMinSpread, fMaxSpread);
	#endif
	}
}

FLOAT GameFuncs::GetTextWidth(PCHAR pTxt, DWORD font) {
	if (pTxt && font) {
	#if defined(BO2)
		return (float)((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.R_TextWidth))(1, pTxt, 0x7fffffff, font);
	#elif defined(MW3) || defined(BO3)
		return (float)((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.R_TextWidth))(pTxt, 0x7fffffff, font, 0);
	#else
		return (float)((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.R_TextWidth))(pTxt, 0x7fffffff, font);
	#endif
	}
	return 0.0f;
}

VOID GameFuncs::AddReliableCommand(PCHAR pCmd) {
	if (pCmd) {
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_AddReliableCommand))(0, pCmd);
	}
}

sScoreboardInfo GameFuncs::GetScoreboardInfo(DWORD dwClient) {
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
		DWORD dwScoreboard = cg_t + decryptDWORD(Not(NiNJA.cPatches)->CoD.ScoresOffset);
		for (DWORD i = 0; i < 24; i++) {
		#if defined(BO1) || defined(BO2) || defined(BO3)
			i = dwClient;
		#endif
			DWORD dwCurrent = dwScoreboard + (i * decryptDWORD(Not(NiNJA.cPatches)->CoD.ScoresSize));
			DWORD dwClientIndex;
		#if defined(AW)
			dwClientIndex = (DWORD)*(BYTE*)(dwCurrent + decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientIndexOffset));
		#elif defined(BO1) || defined(BO2) || defined(BO3)
			dwClientIndex = dwClient;
		#else
			dwClientIndex = *(DWORD*)(dwCurrent + decryptDWORD(Not(NiNJA.cPatches)->CoD.ClientIndexOffset));
		#endif
			if (dwClientIndex == dwClient) {
				//DWORD dwTeam = *(DWORD*)(dwCurrent + decryptDWORD(Not(NiNJA.cPatches)->CoD.TeamOffset));

				//DbgPrintE("Scoreboard Address: %X", dwCurrent);
				sScoreboardInfo tInfo;
				tInfo.dwAssists = *(DWORD*)(dwCurrent + decryptDWORD(Not(NiNJA.cPatches)->CoD.AssistsOffset));
				tInfo.dwDeaths = *(DWORD*)(dwCurrent + decryptDWORD(Not(NiNJA.cPatches)->CoD.DeathsOffset));
				tInfo.dwKills = *(DWORD*)(dwCurrent + decryptDWORD(Not(NiNJA.cPatches)->CoD.KillsOffset));
				tInfo.dwPing = *(DWORD*)(dwCurrent + decryptDWORD(Not(NiNJA.cPatches)->CoD.PingOffset));
			#if defined(BO1) || defined(BO2) || defined(BO3)
				// In bo1/bo2/bo3, the actual ping isn't stored here. (0 = 4 bar, 1 = 3 bar, 2 = 2 bar, 3 = 1 bar)
				tInfo.dwPing = 100 * tInfo.dwPing; // This is what it does on bo1 with the built in ping on the scoreboard... so im gusesing its the most accurate we can get.
			#endif
				tInfo.dwRank = *(DWORD*)(dwCurrent + decryptDWORD(Not(NiNJA.cPatches)->CoD.RankOffset));
				tInfo.dwScore = *(DWORD*)(dwCurrent + decryptDWORD(Not(NiNJA.cPatches)->CoD.ScoreOffset));
				return tInfo;
			}
		}
	}
	return sScoreboardInfo();
}

DWORD GameFuncs::GetWeaponCooldownTime(BOOL bLeftGun) {
#if defined(BO3)
	DWORD cg_t = *(int*)decryptDWORD(Not(NiNJA.cPatches)->CoD.UnknownClientStruct);
	if (cg_t) {
		if (!bLeftGun) {
			return *(int*)(cg_t + 0x4c);
		}
		else {
			return *(int*)(cg_t + 0x6c);
		}
	}
#endif
	return 0;
}

DWORD GameFuncs::GetWeaponDef(DWORD weapon, BOOL variant) {
	int BG_GetWeaponDef = decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetWeaponDef);
	if (BG_GetWeaponDef) {
	int weapDefIndex = 0, weaponDef = 0;
#if defined(MW1)
	return ((DWORD(*)(...))BG_GetWeaponDef)(weapon);
#elif defined(WAW)
	__asm {
		slwi      weaponDef, weapon, 2
		lwzx      weaponDef, weaponDef, BG_GetWeaponDef
	};
#elif defined(MW3)
	if (variant) {
		BG_GetWeaponDef = decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetWeaponVariantDef);
	}
	__asm {
		clrlslwi  weaponDef, weapon, 24, 2
		lwzx      weaponDef, weaponDef, BG_GetWeaponDef
	};
#elif defined(BO3)
	__asm {
		clrlslwi  weapDefIndex, weapon, 24, 2 
		lwzx      weaponDef, weapDefIndex, BG_GetWeaponDef
	}
#else
#if defined(AW)
	weaponDef = ((DWORD(*)(...))BG_GetWeaponDef)(weapon, 0);
#else
	weaponDef = ((DWORD(*)(...))BG_GetWeaponDef)(weapon);
#endif
	if (weaponDef) {
	#if defined(BO2)
		if (!variant) {
			return *(DWORD*)(weaponDef + 0x8);
		}
	#elif defined(GHOSTS)
		if (!variant) {
			return *(DWORD*)(weaponDef + 0x4);
		} 
	#endif
		return weaponDef;
	}
#endif
	return weaponDef;
	}
	return 0;
}

FLOAT GameFuncs::GetWeaponHitLocationMultiplier(SHORT sHitLoc,  DWORD dwWeapon) {
#if !defined(BO3)
	return ((FLOAT(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.G_GetWeaponHitLocationMultiplier))(sHitLoc, dwWeapon);
#else
	return 1.0f;
#endif
}

DWORD GameFuncs::GetWeaponIndexForName(PCHAR pName) {
#if defined(BO3)
	return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.SL_GetString))(pName, 1, strlen(pName) + 0x1);
#elif defined(MW1)
	return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.SL_GetString))(pName, 0);
#else
	return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_GetWeaponIndexForName))(pName);
#endif
}

FLOAT GameFuncs::GetZoomState() {
	DWORD cg_t = Get_CG_t();
	if (cg_t) {
	#if defined(WAW)
		return *(float*)(cg_t + 0x148);
	#elif defined(MW1)
		return *(float*)(cg_t + 0x12c);
	#elif defined(MW2)
		return *(float*)(cg_t + 0x36c0);//0x2c0);
	#elif defined(MW3)
		return *(float*)(cg_t + 0x6c);//0x2c0);
	#elif defined(BO1)
		return *(float*)(cg_t + 0x1f8);
	#elif defined(BO2)
		return *(float*)(cg_t + 0x270);
	#elif defined(GHOSTS)
		return *(float*)(cg_t + 0x424);
	#elif defined(BO3)
		return *(float*)(cg_t + 0x304);
	#elif defined(AW)
		return *(float*)(cg_t + 0x5b0);
	#endif
	}
	return 0.0f;
}

VOID GameFuncs::GiveAmmo(PVOID option) {
#if defined(WAW)
	SendConsoleCommand(ES("give ammo"));
#endif
}

VOID GameFuncs::GiveDefaultWeapon(PVOID option) {
#if defined(WAW)
	SendConsoleCommand(ES("give defaultweapon_mp"));
#endif
}

VOID GameFuncs::GiveKillstreak(PVOID option) {
#if defined(WAW)
	if ( GameFuncs::CheckInGame()) {
		MenuOption *pOption = (MenuOption*)option;

		PCHAR pKillstreakName = pOption->Variations[pOption->dwState];
		if (Compare(pKillstreakName, ES("Dogs"))) {
			AddReliableCommand(ES("give dogs_mp"));
		}
		else if (Compare(pKillstreakName, ES("Artillery"))) {
			AddReliableCommand(ES("give artillery_mp"));
		}
		else if (Compare(pKillstreakName, ES("Recon Plane"))) {
			AddReliableCommand(ES("give radar_mp"));
		}
	}
#endif
}

bool GameFuncs::IsClientTopPlayer(DWORD dwClient) {
	for (DWORD i = 0; i < CG::cTopPlayers.size(); i++) {
		if (dwClient == CG::cTopPlayers[i]) {
			return true;
		}
	}
	return false;
}

#if defined(BO1) || defined(BO2)
bool IsDogEnemy(DWORD dwClient) {
	if (CG::bIsMultiplayer) {
		int iClient = -1, iCount = 0, iAddr = 0;
		while (iClient != 0) {
		#if defined(BO1)
			iAddr = 0x828B9058 + (iCount * 0x20);
		#elif defined(BO2)
			iAddr = 0x82AEC610 + (iCount * 0x20);
		#endif
			iClient = *(int*)iAddr;
			if (iClient != 0) {
			#if defined(BO1)
				int iOwner = *(int*)(iAddr + 0x18);
			#elif defined(BO2)
				int iOwner = *(int*)(iAddr + 0x18);
			#endif
				if (Compare(CG::pGameTypeName, __FFANAME__, FALSE)) {
					DbgPrint("FFA!");
					return iOwner != CG::iLocalClient;
				}
				else {
					return CG::cEntities.List[CG::iLocalClient].sTeam != CG::cEntities.List[iClient].sTeam;
				}
			}
			iCount++;
		}
	}
	return true;

}
#endif

bool GameFuncs::IsEntEnemy(DWORD dwClient) {
	SHORT sTeam = CG::cEntities.List[dwClient].sTeam;
#if defined(BO1) || defined(BO2)
	if (CG::bIsMultiplayer) {
		if (CG::cEntities.List[dwClient].bAI) {
			return IsDogEnemy(dwClient);
		}
	}
#endif
	if (sTeam == 0 && CG::cEntities.List[CG::iLocalClient].sTeam != 0)
		return false;
	else if (CG::cEntities.List[CG::iLocalClient].sTeam == 0)
		return true;
	if (sTeam != CG::cEntities.List[CG::iLocalClient].sTeam)
		return true;
	else
		return false;
}

bool GameFuncs::IsEntInvisible(DWORD dwClient) {
#if defined(BO2)
	if (!CG::cEntities.List[dwClient].bPlayer && *(byte*)(CG::cEntities.List[dwClient].pCentity + 0x157) == 0x20) //invisibility check
		return true;
#endif
	return false;
}

bool GameFuncs::IsEntKillable(DWORD dwClient) {
#if defined(BO2)
	if (!CG::cEntities.List[dwClient].bPlayer && *(DWORD*)(CG::cEntities.List[dwClient].pCentity + 0x1b8) == 0x2 && *(DWORD*)(CG::cEntities.List[dwClient].pCentity + 0x224) == 0x0) //invulnerability check
		return false;
#endif
	return true;
}

bool GameFuncs::IsEntLiving(DWORD dwClient) {
	#if defined(WAW)
		if (dwClient != CG::iLocalClient)
			return (*(byte*)(CG::cEntities.List[dwClient].pCentity + 0x2ac) & 0x40) != FALSE;
		else
			return (*(byte*)(CG::cEntities.List[dwClient].pCentity + 0x165) != 0x4) != FALSE;
	#elif defined(BO1)
		if (dwClient != CG::iLocalClient)
			return (*(byte*)(CG::cEntities.List[dwClient].pCentity + 0x318) & 0x40) != FALSE;
		else
			return (*(byte*)(CG::cEntities.List[dwClient].pCentity + 0x169) != 0x4) != FALSE;
	#elif defined(BO2)
		// Don't shoot at dogs/zombies while they are already dead
		if (!CG::cEntities.List[dwClient].bPlayer) {
			if (*(byte*)(CG::cEntities.List[dwClient].pCentity + 0x155) == 0x4) {
				return FALSE;
			}
		}

		if (dwClient != CG::iLocalClient) {
			return (*(byte*)(CG::cEntities.List[dwClient].pCentity + 0x36c) & 0x40) != FALSE;
		}
		else {
			return (*(byte*)(CG::cEntities.List[dwClient].pCentity + 0x155) != 0x4) != FALSE;
		}
	#elif defined(BO3)
		if (dwClient != CG::iLocalClient)
			return (*(byte*)(CG::cEntities.List[dwClient].pCentity + 0x638) & 0x40) != FALSE; //tu3 0x62c
		else
			return (*(byte*)(CG::cEntities.List[dwClient].pCentity + 0x1B1) != 0x4) != FALSE;
	#elif defined(MW3) || defined(GHOSTS)
		if (dwClient == CG::iLocalClient) {
			return !((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_IsPlayerDead))(0);
		}
		else {
			return *(byte*)(CG::cEntities.List[dwClient].pCentity + 0x1c7) & 0x1;
		}
	#elif defined(MW1)
		if (dwClient == CG::iLocalClient) {
			return !((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_IsPlayerDead))(0);
		}
		else {
			return *(byte*)(CG::cEntities.List[dwClient].pCentity + 0x1b4) & 0x1;
		}
	#elif defined(MW2)
		if (dwClient == CG::iLocalClient) {
			return !((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_IsPlayerDead))(0);
		}
		else {
			return *(byte*)(CG::cEntities.List[dwClient].pCentity + 0x1d3) & 0x1;
		}
	#elif defined(AW)
		if (dwClient == CG::iLocalClient) {
			return !((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_IsPlayerDead))(0);
		}
		else {
			return *(byte*)(CG::cEntities.List[dwClient].pCentity + 0x1db) & 0x1;
		}
	#endif
	return false;
}

SHORT GameFuncs::IsEntLookingAtMe(DWORD dwLookingIndex) {
	vec3_t pLookingPos = CG::cEntities.List[dwLookingIndex].pExt->cTags[TAG_HEAD].vPos;
	for (DWORD i = 0; i < __MAXTARGETBONES__; i++) {
		vec3_t pLookingAngles = CG::cEntities.List[dwLookingIndex].vAngles, pLookingAtPos = CG::cEntities.List[CG::iLocalClient].pExt->cTags[i].vPos;
		vec3_t vDifference = pLookingAtPos - pLookingPos;
		vec3_t vLookingAtMax = vectoangles(vDifference.x, vDifference.y, vDifference.z);

		if (pLookingAngles.IsWithinAngle(vec3_t(10.0f, 10.0f, 180.0f), vLookingAtMax)) {
			//DbgPrintE("%s is looking towards bone %s!", CG::cEntities.List[dwLookingIndex].pExt->pName, CG::cTags.List[i].pName);
			return i;
		}
	}
	/*if (pLookingAngles.IsWithinAngle(vec3_t(120.0f, 30.0f, 180.0f), vLookingAtMax)) {
		DbgPrintE("%s is looking in your direction!", CG::cEntities.List[dwLookingIndex].pExt->pName);
		return i;
	}*/
	return -1;
}

BOOL GameFuncs::IsEntPlayer(DWORD dwClient) {
	CG::cEntities.List[dwClient].bPlayer = CG::cEntities.List[dwClient].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_PLAYER);
	return CG::cEntities.List[dwClient].bPlayer ? TRUE : FALSE;
}

BOOL GameFuncs::IsEntAI(DWORD dwClient) {
	CG::cEntities.List[dwClient].bAI = CG::cEntities.List[dwClient].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_ACTOR);
	return CG::cEntities.List[dwClient].bAI ? TRUE : FALSE;
}

bool GameFuncs::IsEntValid(DWORD dwClient) {
	return IsEntPlayer(dwClient) || IsEntAI(dwClient) ||
		(CG::cEntities.List[dwClient].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_ITEM)) ||
		(CG::cEntities.List[dwClient].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_MISSILE)); /*||
		(CG::cEntities.List[dwClient].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_HELICOPTER));*/
}

bool GameFuncs::IsBoneVisible(DWORD eTagIndex, DWORD dwClient, int iTagOffset) {
	vec3_t myViewOrig = GetLocalViewOrigin(TRUE);
	if (!myViewOrig.IsZero() && !CG::cEntities.List[dwClient].pExt->cTags[eTagIndex].vPos.IsZero()) {
		if (dwClient >= 0 && dwClient < __MAXENTITIES__) {
			BYTE bResult[0x100];
			vec3_t vTagPos = GetTagOffset(eTagIndex, dwClient, iTagOffset);
			FLOAT percent = LocationalTrace(__WORLDENTNUM__, &myViewOrig, &vTagPos, bResult);
			SHORT hitIndex = Trace_GetEntityHitId(bResult);
			if (hitIndex != __WORLDENTNUM__) {
				if (hitIndex >= 0 && hitIndex < __MAXENTITIES__) {
					if (CG::cEntities.List[hitIndex].pExt && hitIndex == dwClient) {
					#if defined(BO2)
						if (*(SHORT*)(bResult + 0x28) == 0x14) {
							CG::cEntities.List[dwClient].pExt->cTags[eTagIndex].bBlockedByShield = true;
							return false;
						}
					#elif defined(MW2)
						if (*(SHORT*)(bResult + 0x26) == 0x13) {
							CG::cEntities.List[dwClient].pExt->cTags[eTagIndex].bBlockedByShield = true;
							return false;
					}
					#endif
						
						CG::cEntities.List[dwClient].pExt->cTags[eTagIndex].bBlockedByShield = false;
						// If we hit a valid client, and they are the one we were targeting
						return true;
					}
				}
			}
		}
	}
	return false;
}


DWORD dwAntiAimTags[] = {
	TAG_NECK,
	TAG_MAINROOT,
	TAG_KNEE_RI,
	TAG_KNEE_LE,
	TAG_ELBOW_RI,
	TAG_ELBOW_LE
};

bool GameFuncs::IsAntiAimBone(DWORD dwClient, DWORD dwTag) {
	if (CG::cEntities.List[dwClient].pExt->bPreferred) {
		for (int i = 0; i < ARRAY_SIZE(dwAntiAimTags); i++) {
			if (dwAntiAimTags[i] == dwTag) {
				return true;
			}
		}
	}
	return false;
}

bool GameFuncs::IsEntVisible(DWORD dwClient) {
	if (CG::cEntities.List[dwClient].pExt) {
		if (CG::cEntities.List[dwClient].bKillable && !CG::cEntities.List[dwClient].pExt->bWhitelisted) {
			if (CG::cEntities.List[dwClient].bPlayer && (CG::bTargetBestBone || CG::cEntities.List[dwClient].pExt->bPreferred)) {
				int iTagOffset = 0;
				FLOAT fHighestDamage = -1.0f;
				bool bFound = false;
			jRetry:
				for (int i = 0; i < __MAXTARGETBONES__; i++) {
					// Run our vis checks on the given bone
					GameFuncs::RunVisibilityChecks(dwClient, i, iTagOffset);
					if (CG::cEntities.List[dwClient].pExt->cTags[i].bVisible && !CG::cEntities.List[dwClient].pExt->cTags[i].bBlockedByShield) {
						FLOAT fDamageMult = GameFuncs::IsAntiAimBone(dwClient, i) ? CG::cEntities.List[dwClient].pExt->cTags[i].fDamageMult + 1.0f : CG::cEntities.List[dwClient].pExt->cTags[i].fDamageMult;
						if (fDamageMult > fHighestDamage) {
							fHighestDamage = fDamageMult;
							CG::cEntities.List[dwClient].pExt->cTags[i].bTagOffset = iTagOffset;
							CG::cEntities.List[dwClient].pExt->sVisibleBone = i;
							bFound = true;
						}
					}
					// Don't scan all bones unless they are prioritized, and stop early if we find a match above TAG_CHEEK_RI
					if ((!CG::cEntities.List[dwClient].pExt->bPreferred && i > TAG_SPINEUPPER) || (bFound && i > TAG_CHEEK_RI)) {
						break;
					}
				}
				if (!bFound && CG::cEntities.List[dwClient].pExt->bTopPlayer && CG::cEntities.List[dwClient].pExt->bPreferred && iTagOffset < 2) {
					iTagOffset++;
					goto jRetry;
				}
				/*if (bFound) {
					printf(GameFuncs::va("[Vis] %i\n", iTagOffset));
				}*/
				return bFound;
			}
			GameFuncs::RunVisibilityChecks(dwClient, CG::dwAimBone, 0);
			CG::cEntities.List[dwClient].pExt->cTags[CG::dwAimBone].bTagOffset = 0;
			CG::cEntities.List[dwClient].pExt->sVisibleBone = CG::dwAimBone;
			return CG::cEntities.List[dwClient].pExt->cTags[CG::dwAimBone].bVisible && !CG::cEntities.List[dwClient].pExt->cTags[CG::dwAimBone].bBlockedByShield;
		}
	}
	return false;
}

#if defined(BO3)
SHORT Trace_GetEntityHitId_BO3(PVOID pTraceResult) {
	if (pTraceResult) {
		int iVal = *(int*)((DWORD)pTraceResult + 0x24);
		SHORT someShit;
		if (iVal == 2 || iVal == 3 || iVal == 4) {
			goto jEnd;
		}
		if (iVal != 1) {
			someShit = 0x2bf;
		}
		else {
			someShit = *(SHORT*)((DWORD)pTraceResult + 0x28);
		}
		//__asm { clrlwi someShit, someShit, 16 }
		return someShit;
	}
jEnd:
	return 0x2be;
}
#endif

// UNUSED, SAVING RESEARCH INCASE I DECIDE TO USE LATER
//int iOldStreakIndex;
//bool bWasInKillstreak = false;
//bool GameFuncs::IsLocalClientUsingVehicle() {
//#if defined(BO1)
//	DWORD vehicles = 0x828BA9B8;
//
//	if (!CG::bInKillstreak && bWasInKillstreak) {
//		*(DWORD*)(vehicles + (iOldStreakIndex * 0x1c) + 0x18) = __WORLDENTNUM__;
//		DbgPrint("Reset old streak owner!");
//		bWasInKillstreak = false;
//	}
//
//	int index = 0;
//	while (true) {
//		DWORD dwEntIndex = *(DWORD*)(vehicles + (index * 0x1c));
//		if (dwEntIndex == 0) {
//			return false;
//		}
//		else {
//			if (dwEntIndex < __MAXENTITIES__) {
//				DWORD dwOwnerIndex = *(DWORD*)(vehicles + (index * 0x1c) + 0x18);
//				if (dwOwnerIndex == CG::iLocalClient && CG::cEntities.List[dwEntIndex].bValid && CG::cEntities.List[dwEntIndex].bAlive) {
//					DbgPrintE("WE OWN SHIT %i NIGGA", dwEntIndex);
//					if (!bWasInKillstreak) {
//						iOldStreakIndex = index;
//						bWasInKillstreak = true;
//					}
//					return true;
//				}
//			}
//		}
//		index++;
//		if (index > 100) {
//			// Show me a game we managed to call in more than 100 vehicles
//			return false;
//		}
//	}
//#endif
//	return false;
//}

//bool GameFuncs::IsLocalClientMantling() {
//
//}

bool GameFuncs::IsWeaponFullAuto(DWORD dwWeaponIndex) {
#if defined(GHOSTS)
	DWORD dwWeaponDef = GetWeaponDef(dwWeaponIndex, TRUE);
#else
	DWORD dwWeaponDef = GetWeaponDef(dwWeaponIndex);
#endif
	if (dwWeaponDef) {
	#if defined(MW1)
		return *(DWORD*)(dwWeaponDef + 0x140) == 0;
	#elif defined(MW2) || defined(MW3)
		return *(DWORD*)(dwWeaponDef + 0x3c) == 0;
	#elif defined(GHOSTS)
		return *(DWORD*)(dwWeaponDef + 0x5c) == 0;
	#elif defined(AW)
		return *(DWORD*)(dwWeaponDef + 0x2b4) == 0;
	#elif defined(WAW)
		return *(DWORD*)(dwWeaponDef + 0x158) == 0;
	#elif defined(BO1) || defined(BO2)
		return *(DWORD*)(dwWeaponDef + 0x30) == 0;
	#elif defined(BO3)
		return *(DWORD*)(dwWeaponDef + 0x7c) == 30 || *(DWORD*)(dwWeaponDef + 0x7c) == 40;
	#endif
	}
	return true;
}

SHORT GameFuncs::Trace_GetEntityHitId(PVOID pTrace) {
#if defined(BO3)
	return Trace_GetEntityHitId_BO3(pTrace);
#else
	return ((SHORT(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.Trace_GetEntityHitId))(pTrace);
#endif
}

/* Checks if our local client is looking at entity */
int GameFuncs::IsLookingAtEntity() {
	// Forward, right, up
	vec3_t *viewAxis = (vec3_t*)Get_ViewAxis_Ptr();
	if (viewAxis) {
		vec3_t start = GetLocalViewOrigin(), end;
		if (!start.IsZero()) {
			VectorMA((FLOAT*)&start.x, 8192.0f, (FLOAT*)&viewAxis[0], (FLOAT*)&end.x);
			BYTE bResult[0x100];
			FLOAT fTraceResult = LocationalTrace(__WORLDENTNUM__, &start, &end, bResult);
			SHORT hitIndex = Trace_GetEntityHitId(bResult); 
			if (hitIndex != __WORLDENTNUM__) {
				if (hitIndex >= 0 && hitIndex < __MAXENTITIES__) {
					if (CG::cEntities.List[hitIndex].pExt) {
						return hitIndex;
					}
				}
			}
		}
	}
	return -1;
}

bool GameFuncs::IsUsingDualWieldWeapon(DWORD dwClient) {
	DWORD dwCurrentWeapon = CG::cEntities.List[dwClient].pExt->dwCurrentWeapon;
#if defined(BO1)
	DWORD dwWeapDef = GetWeaponDef(dwCurrentWeapon);
	if (dwWeapDef) {
		return *(BYTE*)(dwWeapDef + 0x56a) != 0;
	}
#elif defined(BO2)
	return *(BYTE*)(GetWeaponDef(dwCurrentWeapon) + 0x64F) != FALSE;
#elif defined(MW2)
	DWORD cg_t = *(int*)decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_t);
	if (cg_t) {
		int tmp = ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_WeaponIsDualWield))(cg_t, dwCurrentWeapon);
		if (tmp)
			return *(BYTE*)(tmp + 0x1) != 0;
	}
#elif defined(MW3) || defined(GHOSTS) || defined(AW)
	return ((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_WeaponIsDualWield))(dwCurrentWeapon);
#endif
	return false;
}

bool GameFuncs::IsMultiplayer() {
	PCHAR pMapName = GetMapName_Internal();
	if (pMapName) {
		return strstr(pMapName, "zm_") == NULL;
	}
	return true;
}

FLOAT GameFuncs::LocationalTrace_GetFraction(PBYTE pResult){
#if defined(AW) || defined(MW1) || defined(MW2) || defined(MW3) || defined(GHOSTS)
	return *(float*)pResult;
#else
	return *(float*)(pResult + 0x10);
#endif
}

FLOAT GameFuncs::LocationalTrace(DWORD dwTargetClient, vec3_t *vFrom, vec3_t *vTo, BYTE *bResult, int optMask) {
	if (dwTargetClient >= 0 && dwTargetClient < __MAXENTITIES__) {
		ZeroMemory(bResult, 0x100);
		jmp_buf jVisibleJump;
		int ret = setjmp(jVisibleJump);
		if (!ret) {
			try {
				((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_LocationalTrace))(bResult, vFrom, vTo, dwTargetClient, optMask != -420 ? optMask : __TRACEMASK__, FALSE, FALSE);
			}
			catch (...) {
				//printf("Crashed in visible check!\n");
				//ZeroMemory(bResult, 0x100);
				longjmp(jVisibleJump, 420);
			}
			return LocationalTrace_GetFraction(bResult);
		}
	}
	return 0.0f;
}

VOID GameFuncs::ToggleAntiDerank(BOOL bState) {
	// If bState is greater than 1, it's a pointer to our menu option
	if (bState > 1) {
		// Casat it to a pointer to our menu option
		MenuOption *mOpt = (MenuOption*)bState;

		// Then extract the value, and store it as the state
		bState = mOpt->dwState;
	}
#if defined(MW2)
	DWORD dwStatWipe = decryptDWORD(Not(NiNJA.cPatches)->CoD.DisableStatWipe);
	if (dwStatWipe) {
		if (bState) {
			*(SHORT*)dwStatWipe = 0x4800;
		}
		else {
			*(SHORT*)dwStatWipe = 0x419A;
		}
	}
#endif
}

VOID GameFuncs::PreventDerank() {
	if (CG::bAntiDerank) {
#if defined(MW1) || defined(WAW) || defined(MW2)
		CONST PCHAR activeActionString = (CONST PCHAR)(Dvar_GetValue(decryptDWORD(Not(NiNJA.cPatches)->CoD.Dvar_ActiveAction)));
		if (Contains(activeActionString, "resetStats") || Contains(activeActionString, "defaultStatsInit")) {
			*(BYTE*)((DWORD)activeActionString) = 0;
			DbgPrint("Prevented stat reset!");
		}
#endif
	}
}

VOID GameFuncs::SendConsoleCommand(PCHAR pCmd) {
	if (pCmd) {
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CBuf_AddText))(0, pCmd);
	}
}

CHAR GameFuncs::pGamertagBuffer[33] = { 0 };
PCHAR GameFuncs::GetGamertagOffset() {
#if defined(BO1) || defined(BO2) || defined(BO3)
	return pGamertagBuffer;
#else 
	return (PCHAR)decryptDWORD(Not(NiNJA.cPatches)->CoD.PregameGTOffset);
#endif
}

string GameFuncs::BuildUserInfoString(string name, string clantag, string xuid, bool eliteTitle = false, int titleBg = -1, string titleText = "") {
	string userInfoString;
	userInfoString = "userinfo \"\\rate\\20000\\snaps\\20\\name\\" + name;
	if (clantag != "")
		userInfoString += "\\clanAbbrev\\" + clantag;
	//if (currentTitle == MW3_TITLE_ID)
	//	userInfoString += "\\ec_usingTag\\0\\ec_usingTitle\\" + (eliteTitle ? 1 : 0).ToString() + "\\ec_TitleBg\\" + titleBg.ToString() + "\\ec_TitleText\\" + titleText + "\\ec_Level\\0";
	userInfoString += "\\xuid\\000" + xuid + "\\natType\\1\\rank\\1\\prestige\\0\"";
	return userInfoString;
}

void GameFuncs::SetIngameInfo(string clantag, string gamertag) {
	if (CG::bInGameCached && CG::bLobbyInitialized) {
		char buff[17] = { 0 };
		string userinfo = BuildUserInfoString(gamertag, clantag, _ui64toa(CG::cEntities.List[CG::iRealLocalClient].pExt->cXuid, buff, 0x10));

		AddReliableCommand((PCHAR)userinfo.c_str());
	}
}

VOID GameFuncs::SetStance(eSetStance stance) {
	((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_SetStance))(0, stance);
}

CHAR pColorString[4] = { 0 };
VOID GameFuncs::SetGamertag(PCHAR pNewGamertag) {
	PCHAR pGamertag = GetGamertagOffset();
	if (pGamertag && pNewGamertag) {
		memcpy(pGamertag, pNewGamertag, 32);
		if (CG::bInGameCached) {
			if (pNewGamertag[0] == '^' && pNewGamertag[1] >= '0' && pNewGamertag[1] <= '9') {
				memcpy(pColorString, pNewGamertag, 0x2);
				pNewGamertag += 2;
			}
			SetIngameInfo(pColorString, pNewGamertag);
		}
	}
}

DWORD dwCurrentCycle = 9;
DWORD dwLastSwitchTime = 0, dwLastRainbowTime = 0;
int iLastColor = 0, iLastClient = 0;
VOID GameFuncs::ModGamertag() {
	bool bSwitchTime = false;
	if (CG::bCycleGamertags || CG::bStealGTs) {
		if (dwLastSwitchTime < GetTickCount()) {
			dwLastSwitchTime = GetTickCount() + (!CG::bInGameCached ? CG::dwCycleDelay : (CG::dwCycleDelay < __INGAMESWITCHDELAY__ ? __INGAMESWITCHDELAY__ : CG::dwCycleDelay));
			bSwitchTime = true;
		}
	}

	PCHAR pSetGT = NULL;
	if (bSwitchTime) {
		if (CG::bCycleGamertags) {
			int count = 0;
			while (count < 10) {
				// Increment our current cycle
				dwCurrentCycle++;

				// Make sure it stays within the limits of the size of our array
				if (dwCurrentCycle >= 10) {
					dwCurrentCycle = 0;
				}

				if (!Compare(CG::pModdedGamertags[dwCurrentCycle], __EMPTYGTSTRING__)) {
					pSetGT = CG::pModdedGamertags[dwCurrentCycle];
					//SetGamertag(CG::pModdedGamertags[dwCurrentCycle]);
					break;
				}
				count++;
			}
		}
		else if (CG::bStealGTs) {
			int count = 0;
			while (count < __MAXCLIENTS__) {
			jStealRetry:
				count++;
				int RandomClient = rand() % __MAXCLIENTS__;
				if (RandomClient == iLastClient || RandomClient == CG::iRealLocalClient) {
					goto jStealRetry;
				}
				iLastClient = RandomClient;

				if (CG::cEntities.List[RandomClient].pExt) {
					PCHAR name = CG::cEntities.List[RandomClient].pExt->GetName();
					if (name) {
						if (!Compare(name, "")) {
							pSetGT = name;
							//SetGamertag(name);
							break;
						}
					}
				}
			}
		}
	}

	CHAR newGT[100] = { 0 };
	if (CG::bRainbowGT) {
		//// Sync our rainbow switch time with our steal gts/cycle switch time
		//if ( GameFuncs::CheckInGame()) {
		//	if ((CG::bCycleGamertags || CG::bStealGTs) && dwLastSwitchTime != dwLastRainbowTime) {
		//		dwLastRainbowTime = dwLastSwitchTime;
		//	}
		//}

		if (bSwitchTime) {
			dwLastRainbowTime = 0;
		}

		if (dwLastRainbowTime < GetTickCount()) {
			//printf("Switching.\n");
			dwLastRainbowTime = GetTickCount() + (!CG::bInGameCached ? CG::dwRainbowDelay : (CG::dwRainbowDelay < __INGAMESWITCHDELAY__ ? __INGAMESWITCHDELAY__ : CG::dwRainbowDelay));
			PCHAR gt = NULL;
			if (!pSetGT) {
				gt = GetGamertagOffset();
			}
			else {
				gt = pSetGT;
			}
			if (gt) {
				if (gt[0] == '^' && gt[1] >= '0' && gt[1] <= '9') {
					gt += 2;
				}

			jColorRetry:
				int RandomColor = rand() % 7 + 1;
				if (RandomColor == iLastColor) {
					goto jColorRetry;
				}
				iLastColor = RandomColor;
				sprintf(pColorString, "^%i", RandomColor);
				sprintf(newGT, "%s%s", pColorString, gt);
				pSetGT = newGT;
			}
		}
	}
	else {
		pColorString[0] = 0;
	}

#if defined(BO1) || defined(BO2) || defined(BO3)
	if (!CG::bUseXamGT) {
		if (dwLastSwitchTime < GetTickCount() && dwLastRainbowTime < GetTickCount() && CG::bIsMultiplayer) {
			dwLastSwitchTime = GetTickCount() + (!CG::bInGameCached ? CG::dwCycleDelay : (CG::dwCycleDelay < __INGAMESWITCHDELAY__ ? __INGAMESWITCHDELAY__ : CG::dwCycleDelay));
			pSetGT = GetGamertagOffset();
		}
	}
#endif

	if (pSetGT) {
		SetGamertag(pSetGT);
	}
}

VOID GameFuncs::ToggleClassItems(BOOL bState) {
	// If bState is greater than 1, it's a pointer to our menu option
	if (bState > 1) {
		// Cast it to a pointer to our menu option
		MenuOption *mOpt = (MenuOption*)bState;

		// Then extract the value, and store it as the state
		bState = mOpt->dwState;
	}
#if defined(BO2)
	DWORD dwClassItems = decryptDWORD(Not(NiNJA.cPatches)->CoD.ClassItemsAddress);
	if (dwClassItems) {
		if (bState) {
			*(DWORD*)dwClassItems = 0x3B40CBE7; //spoof upload speed to 0.001 megabits/s
		}
		else {
			*(DWORD*)dwClassItems = 0x7F43D214; //spoof upload speed to 10 megabits/s, this should help them pull host even on a shit connection
		}
	}
#endif
}

VOID GameFuncs::TeleportTo(int iClient) {
	if (CG::cEntities.List[iClient].pExt) {
		CHAR pCommandBuffer[500];
		ZeroMemory(pCommandBuffer, 500);
		sprintf(pCommandBuffer, "%s %f %f %f", ES("setviewpos"), CG::cEntities.List[iClient].pExt->cTags[TAG_HEAD].vPos.x, CG::cEntities.List[iClient].pExt->cTags[TAG_HEAD].vPos.y, CG::cEntities.List[iClient].pExt->cTags[TAG_HEAD].vPos.z);
		SendConsoleCommand(pCommandBuffer);
	}
}

VOID GameFuncs::ToggleGodmode(PVOID opt) {
#if defined(WAW)
	SendConsoleCommand(ES("god"));
#endif
}

VOID GameFuncs::ToggleNoclip(PVOID opt) {
#if defined(WAW)
	SendConsoleCommand(ES("noclip"));
#endif
}


VOID GameFuncs::ToggleNeverHost(BOOL bState) {
	// If bState is greater than 1, it's a pointer to our menu option
	if (bState > 1) {
		// Cast it to a pointer to our menu option
		MenuOption *mOpt = (MenuOption*)bState;

		// Then extract the value, and store it as the state
		bState = mOpt->dwState;
	}
	DWORD dwNeverHost = decryptDWORD(Not(NiNJA.cPatches)->CoD.UploadSpeedAddress);

	if (dwNeverHost) {
		if (bState) {
			*(DWORD*)dwNeverHost = 1000; //spoof upload speed to 0.001 megabits/s
		}
		else {
			*(DWORD*)dwNeverHost = 10000000; //spoof upload speed to 10 megabits/s, this should help them pull host even on a shit connection
		}
	}

}

VOID GameFuncs::ToggleThirdPerson(BOOL bState) {
	// If bState is greater than 1, it's a pointer to our menu option
	if (bState > 1) {
		// Cast it to a pointer to our menu option
		MenuOption *mOpt = (MenuOption*)bState;

		// Then extract the value, and store it as the state
		bState = mOpt->dwState;
	}

#if !defined(BO3)
#if !defined(BO2)
	DWORD dwThirdPerson = Dvar_GetValueAddress(decryptDWORD(Not(NiNJA.cPatches)->CoD.Dvar_CG_ThirdPerson));
#else
	DWORD dwThirdPerson = Get_CG_t() + 0x4809c;
#endif
	if (MmIsAddressValid((PVOID)dwThirdPerson)) {
		*(BYTE*)dwThirdPerson = (BYTE)bState;
	}
#endif
}

VOID GameFuncs::SpoofIPAddress(DWORD ip) {
	*(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.SystemIPAddress) = ip;
}

VOID GameFuncs::SetFOV(FLOAT fFov) {
	DWORD pFov = Dvar_GetValueAddress(decryptDWORD(Not(NiNJA.cPatches)->CoD.Dvar_CG_FOV));
	if (pFov) {
		*(FLOAT*)pFov = fFov;
	}
}

VOID GameFuncs::SetPrestige(SHORT iPrestige) {
#if defined(BO2)
	iPrestige = ReverseWORD(iPrestige);
#endif
	*(SHORT*)decryptDWORD(Not(NiNJA.cPatches)->CoD.PrestigeOffset) = iPrestige;
}

DWORD dwNoSwayOrig = NULL, dwNoSwayOrig2 = NULL;
VOID GameFuncs::ToggleNoSway(BOOL bState) {
	// If bState is greater than 1, it's a pointer to our menu option
	if (bState > 1) {
		// Cast it to a pointer to our menu option
		MenuOption *mOpt = (MenuOption*)bState;

		// Then extract the value, and store it as the state
		bState = mOpt->dwState;
	}

	DWORD dwNoSway = decryptDWORD(Not(NiNJA.cPatches)->CoD.NoSway), dwNoSway2 = decryptDWORD(Not(NiNJA.cPatches)->CoD.NoSway2);
	if (dwNoSway) {
		if (!dwNoSwayOrig) {
			dwNoSwayOrig = *(DWORD*)dwNoSway;
		}
	#if !defined(WAW) && !defined(BO3) 
		if (dwNoSway2) {
			if (!dwNoSwayOrig2) {
				dwNoSwayOrig2 = *(DWORD*)dwNoSway2;
			}
		}
	#endif

		if (bState) {
			*(DWORD*)dwNoSway = __NOP__;
		#if !defined(WAW) && !defined(BO3) 
			*(DWORD*)dwNoSway2 = __NOP__;
		#endif
		}
		else if(dwNoSwayOrig) {
			*(DWORD*)dwNoSway = dwNoSwayOrig;
		#if !defined(WAW) && !defined(BO3) 
			*(DWORD*)dwNoSway2 = dwNoSwayOrig2;
		#endif
		}
	}
	DbgPrint("Toggled nosway state to %i", bState);
}

DWORD dwNoRecoilOrig = NULL;
VOID GameFuncs::ToggleNoRecoil(BOOL bState) {
	// If bState is greater than 1, it's a pointer to our menu option
	if (bState > 1) {
		// Cast it to a pointer to our menu option
		MenuOption *mOpt = (MenuOption*)bState;

		// Then extract the value, and store it as the state
		bState = mOpt->dwState;
	}

	DWORD dwNoRecoil = decryptDWORD(Not(NiNJA.cPatches)->CoD.NoRecoil);
	if (dwNoRecoil) {
		if (!dwNoRecoilOrig) {
			dwNoRecoilOrig = *(DWORD*)dwNoRecoil;
		}

		if (bState) {
			*(DWORD*)dwNoRecoil = __NOP__;
		}
		else if(dwNoRecoilOrig) {
			*(DWORD*)dwNoRecoil = dwNoRecoilOrig;
		}
	}
	DbgPrint("Toggled norecoil state to %i", bState);
}

VOID GameFuncs::ToggleShowPing(BOOL bState) {
	// If bState is greater than 1, it's a pointer to our menu option
	if (bState > 1) {
		// Casat it to a pointer to our menu option
		MenuOption *mOpt = (MenuOption*)bState;

		// Then extract the value, and store it as the state
		bState = mOpt->dwState;
	}

#if defined(BO1) || defined(MW1) || defined(WAW) || defined(MW2) || defined(MW3)
	if (bState) {
		SendConsoleCommand(ES("cg_scoreboardpingtext 1"));
	}
	else {
		SendConsoleCommand(ES("cg_scoreboardpingtext 0"));
	}
#endif
}

BOOL GameFuncs::WorldToScreen(vec3_t *vWorld, FLOAT *screenX, FLOAT *screenY) {
	return ((bool(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_CalcNamePosition))(0, vWorld, screenX, screenY) ? TRUE : FALSE;
}

VOID GameFuncs::UI_DrawText(PCHAR pText, FLOAT fX, FLOAT fY, FLOAT fScale, DWORD horizAlign, DWORD vertAlign, FLOAT rotation, const FLOAT *color, DWORD font) {
	if (pText) {
		DWORD UI_DrawTxt = decryptDWORD(Not(NiNJA.cPatches)->CoD.UI_DrawText);
		if (UI_DrawTxt) {
			((VOID(*)(...))UI_DrawTxt)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ScreenPlacement), pText, 0x7fffffff, font, fX, fY, horizAlign, vertAlign, fScale, color, 0);
		}
	}
}

VOID GameFuncs::LiveStorage_SetStat(int localclientnum, int statindex, int value) {
#if defined(MW1)
	((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.LiveStorage_SetStat))(localclientnum, statindex, value);
#endif
}

VOID GameFuncs::UnlockAll() {
	//	Max Rank Code
	//	======================== =
	LiveStorage_SetStat(0, 0xFC, 54);//set max rank
	LiveStorage_SetStat(0, 0x104, 1);//unlock create a class
	LiveStorage_SetStat(0, 0x105, 2);//unlock clan tag 
	LiveStorage_SetStat(0, 0x107, 2);//unlock challenges
	LiveStorage_SetStat(0, 2301, 120280);//set XP
	LiveStorage_SetStat(0, 2326, 10);//sets prestige
	LiveStorage_SetStat(0, 2350, 54);//sets other max rank
	LiveStorage_SetStat(0, 2351, 120280);//sets minxp
	LiveStorage_SetStat(0, 2352, 120280);//sets maxxp
	LiveStorage_SetStat(0, 2353, 120280);//sets lastxp
	//==========================

	//	Unlock All Perks
	//	================== =
	LiveStorage_SetStat(0, 0x96, 2);
	LiveStorage_SetStat(0, 0x97, 2);
	LiveStorage_SetStat(0, 0x98, 2);
	LiveStorage_SetStat(0, 0x99, 2);
	LiveStorage_SetStat(0, 0x9A, 2);
	LiveStorage_SetStat(0, 0x9B, 2);
	LiveStorage_SetStat(0, 0x9C, 2);
	LiveStorage_SetStat(0, 0x9D, 2);
	LiveStorage_SetStat(0, 0x9E, 2);
	LiveStorage_SetStat(0, 0xA0, 2);
	LiveStorage_SetStat(0, 0xA1, 2);
	LiveStorage_SetStat(0, 0xA2, 2);
	LiveStorage_SetStat(0, 0xA3, 2);
	LiveStorage_SetStat(0, 0xA4, 2);
	LiveStorage_SetStat(0, 0xA5, 2);
	LiveStorage_SetStat(0, 0xA6, 2);
	LiveStorage_SetStat(0, 0xA7, 2);
	LiveStorage_SetStat(0, 0xAD, 2);
	LiveStorage_SetStat(0, 0xB0, 2);
	LiveStorage_SetStat(0, 0xB8, 2);
	LiveStorage_SetStat(0, 0xB9, 2);
	LiveStorage_SetStat(0, 0xBA, 2);
	//================== =

	//	Unlock Weapons + attachments
	//	================================== =
	LiveStorage_SetStat(0, 3001, 9);//new 
	LiveStorage_SetStat(0, 3002, 9);//m1911 pistol
	LiveStorage_SetStat(0, 3003, 0x00010001);//Desert Eagle
	LiveStorage_SetStat(0, 3004, 0x00010001);//Gold Desert Eagle
	//================================== =
}

VOID GameFuncs::WriteIPLogEntry(int iClient) {
	if (CG::cEntities.List[iClient].pExt && CG::cEntities.List[iClient].pExt->cIP.S_un.S_addr != 0 && CG::cEntities.List[iClient].pExt->pRealName) {
		XUID myXuid;
		if (XUserGetXUID(0, &myXuid) == 0) {
			if (myXuid == CG::cEntities.List[iClient].pExt->cXuid) {
				// Don't log our own IP
				return;
			}

			bool bSuccess = false;
			// Grab their xuid so we can check for it
			CHAR pXuidBuffer[33];
			ZeroMemory(pXuidBuffer, 33);
			sprintf(pXuidBuffer, "000%llx", CG::cEntities.List[iClient].pExt->cXuid);

			CHAR pIpBuffer[16];
			ZeroMemory(pIpBuffer, 16);
			if (Contains(CG::cEntities.List[iClient].pExt->pRealName, ES("Sys Medaka"))) {
				sprintf(pIpBuffer, "%i.%i.%i.%i", CG::cEntities.List[CG::iRealLocalClient].pExt->cIP.S_un.S_un_b.s_b1, CG::cEntities.List[CG::iRealLocalClient].pExt->cIP.S_un.S_un_b.s_b2, CG::cEntities.List[CG::iRealLocalClient].pExt->cIP.S_un.S_un_b.s_b3, CG::cEntities.List[CG::iRealLocalClient].pExt->cIP.S_un.S_un_b.s_b4);
			}
			else {
				sprintf(pIpBuffer, "%i.%i.%i.%i", CG::cEntities.List[iClient].pExt->cIP.S_un.S_un_b.s_b1, CG::cEntities.List[iClient].pExt->cIP.S_un.S_un_b.s_b2, CG::cEntities.List[iClient].pExt->cIP.S_un.S_un_b.s_b3, CG::cEntities.List[iClient].pExt->cIP.S_un.S_un_b.s_b4);
			}

			// Get the seconds since the epoch
			time_t rawtime;
			time(&rawtime);

			// Convert it to something we can understand
			struct tm *timeInfo;
			timeInfo = localtime(&rawtime);

			if (!CreateDirectory("NiNJA:\\IP-Logs\\", NULL) && ERROR_ALREADY_EXISTS != GetLastError()) {
				DbgPrint("Failed to create directory for IP log, aborting!");
				return;
			}

			CHAR pFileName[MAX_PATH];
			ZeroMemory(pFileName, MAX_PATH);
			sprintf(pFileName, "NiNJA:\\IP-Logs\\%i-%i-%i.txt", 1900 + timeInfo->tm_year, timeInfo->tm_mon + 1, timeInfo->tm_mday);
			ofstream output(pFileName, ios::out | ios::app);
			if (output.is_open()) {
				if (strlen(CG::cEntities.List[iClient].pExt->pName) > 0 && CG::cEntities.List[iClient].pExt->pName[0] != ' ' && !Compare(CG::cEntities.List[iClient].pExt->pRealName, CG::cEntities.List[iClient].pExt->pName)) {
					output << "Gamertag: " << string(CG::cEntities.List[iClient].pExt->pRealName) << ", Spoofed Gamertag: " << string(CG::cEntities.List[iClient].pExt->pName) << ", IP: " << string(pIpBuffer) << ", XUID: " << string(pXuidBuffer) << endl;
				}
				else {
					output << "Gamertag: " << string(CG::cEntities.List[iClient].pExt->pRealName) << ", IP: " << string(pIpBuffer) << ", XUID: " << string(pXuidBuffer) << endl;
				}
				output.flush();
				output.close();
			}
		}
	}
}

// This one works, but it gets slow after too many people get logged, instead we'll just fill the fgts hard drive with tons of logs :)
//CHAR pLineBuffer[1000];
//VOID GameFuncs::WriteIPLogEntry(int iClient) {
//	//int startTime = GetTickCount();
//
//	if (CG::cEntities.List[iClient].pExt && CG::cEntities.List[iClient].pExt->cIP.S_un.S_addr != 0 && CG::cEntities.List[iClient].pExt->pRealName) {
//
//		bool bSuccess = false;
//		// Grab their xuid so we can check for it
//		CHAR pXuidBuffer[33];
//		ZeroMemory(pXuidBuffer, 33);
//		sprintf(pXuidBuffer, "000%llx", CG::cEntities.List[iClient].pExt->cXuid);
//
//		CHAR pIpBuffer[16];
//		ZeroMemory(pIpBuffer, 16);
//		if (Contains(CG::cEntities.List[iClient].pExt->pRealName, ES("Sys Medaka"))) {
//			sprintf(pIpBuffer, "%i.%i.%i.%i", CG::cEntities.List[CG::iRealLocalClient].pExt->cIP.S_un.S_un_b.s_b1, CG::cEntities.List[CG::iRealLocalClient].pExt->cIP.S_un.S_un_b.s_b2, CG::cEntities.List[CG::iRealLocalClient].pExt->cIP.S_un.S_un_b.s_b3, CG::cEntities.List[CG::iRealLocalClient].pExt->cIP.S_un.S_un_b.s_b4);
//		}
//		else {
//			sprintf(pIpBuffer, "%i.%i.%i.%i", CG::cEntities.List[iClient].pExt->cIP.S_un.S_un_b.s_b1, CG::cEntities.List[iClient].pExt->cIP.S_un.S_un_b.s_b2, CG::cEntities.List[iClient].pExt->cIP.S_un.S_un_b.s_b3, CG::cEntities.List[iClient].pExt->cIP.S_un.S_un_b.s_b4);
//		}
//
//		// Open up our files for reading/writing
//		ifstream input("NiNJA:\\IPs.log");
//		ofstream output("NiNJA:\\IPs.log.tmp", ios::app);
//		bool bFound = false;
//
//		if (input.is_open()) {
//			if (output.is_open()) {
//				for (std::string line; getline(input, line); ) {
//					if (!bFound && Contains((CHAR*)line.c_str(), pXuidBuffer)) {
//						bFound = true;
//						ZeroMemory(pLineBuffer, 1000);
//
//						if (CG::cEntities.List[iClient].pExt->pName[0] != ' ' && !Compare(CG::cEntities.List[iClient].pExt->pRealName, CG::cEntities.List[iClient].pExt->pName)) {
//							sprintf(pLineBuffer, "Gamertag: %s, Spoofed Gamertag: %s, IP: %s, XUID: %s\r\n", CG::cEntities.List[iClient].pExt->pRealName, CG::cEntities.List[iClient].pExt->pName, pIpBuffer, pXuidBuffer);
//						}
//						else {
//							sprintf(pLineBuffer, "Gamertag: %s, IP: %s, XUID: %s\r\n", CG::cEntities.List[iClient].pExt->pRealName, pIpBuffer, pXuidBuffer);
//						}
//						output.write(pLineBuffer, strlen(pLineBuffer));
//						output.flush();
//					}
//					else {
//						line += "\r\n";
//						output.write(line.c_str(), line.size());
//						output.flush();
//					}
//				}
//			}
//			input.close();
//		}
//
//
//		if (output.is_open()) {
//			// If we didn't find the users xuid in the list, append a new entry
//			if (!bFound) {
//				ZeroMemory(pLineBuffer, 1000);
//				if (!Compare(CG::cEntities.List[iClient].pExt->pRealName, CG::cEntities.List[iClient].pExt->pName)) {
//					sprintf(pLineBuffer, "Gamertag: %s, Spoofed Gamertag: %s, IP: %s, XUID: %s\r\n", CG::cEntities.List[iClient].pExt->pRealName, CG::cEntities.List[iClient].pExt->pName, pIpBuffer, pXuidBuffer);
//				}
//				else {
//					sprintf(pLineBuffer, "Gamertag: %s, IP: %s, XUID: %s\r\n", CG::cEntities.List[iClient].pExt->pRealName, pIpBuffer, pXuidBuffer);
//				}
//				output.write(pLineBuffer, strlen(pLineBuffer));
//				output.flush();
//			}
//			output.close();
//			bSuccess = true;
//		}
//
//
//		if (bSuccess) {
//			if (FileExists("NiNJA:\\IPs.log")) {
//				while (!DeleteFile("NiNJA:\\IPs.log")) {
//					Sleep(500);
//				}
//			}
//			MoveFile("NiNJA:\\IPs.log.tmp", "NiNJA:\\IPs.log");
//		}
//
//
//		//printf("IP logger execution took %ims\n", GetTickCount() - startTime);
//	}
//}