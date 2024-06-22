#include "TitleHooks.h"
#include "Sounds.h"
#include "ServerComm.h"
#include "HttpRequest.h"

Detour<VOID> R_EndFrame_Detour;
Detour<VOID> CL_CreateNewCommands_Detour;
Detour<DWORD> XamInputGetState_Detour;
Detour<VOID> BG_UpdatePlayerDObj_Detour;
Detour<VOID> CG_CompassDrawPlayerMap_Detour, CG_CompassDrawPlayerMapLocationSelector_Detour;
Detour<VOID> CG_EntityEvent_Detour;
Detour<VOID> CL_SendCommand_Detour;

BOOL bFirstRun = TRUE;
bool bAutoShootFlip = false;
DWORD dwScoreEndTime = 0;
sScoreboardInfo cOldScore;
BOOL TitleHooks::bInitialized = FALSE;
Sounds pSounds;
bool bWasReloading = false, bYPressed = false, bWasInKillstreak = false, bWasFiring = false;
int iYCount = 0;
DWORD dwTimer = 0;
DWORD dwPrestigeDiscoTime = 0;
bool bSpoofIP = false;
bool bThreadSuspended = false;
DWORD dwOrigIP = 0;
bool bPartyStarted = false;
DWORD dwLastCheckInTime = 0;
bool bNewLobby = false;
bool bSwitchFlip = false;
DWORD dwLeftDelay = 0, dwRightDelay = 0;
bool bJustFired = false, bJustAimed = false, bRightSwitch = false, bLeftSwitch = false;
BYTE bLastRTVal = 0, bLastLTVal = 0;
bool bPressingRT = false, bPressingLT = false;
bool bApplyJitter = true;
DWORD dwLastWeapSwitchTime = 0;
bool bJustSpammedInfo = false;
bool bThreadRelaunching = false, bWasMigrating = false;
DWORD dwLastMigrationTime = NULL;

/* A thread that can call game functions used to offload our trace functions to increase FPS */
#pragma region Extra Thread
__forceinline void InitThreadData() {
	int threadInfo = 0;
	__asm lwz threadInfo, 0(r13)
#if defined(MW1) || defined(MW2)
	*(int*)(threadInfo + 0x14) = decryptDWORD(Not(NiNJA.cPatches)->CoD.G_ThreadValues) + (__THREADINDEX__ << 4);
#elif defined(BO1) 
	*(int*)(threadInfo + 0x10) = decryptDWORD(Not(NiNJA.cPatches)->CoD.G_ThreadValues) + (((__THREADINDEX__ << 2) + __THREADINDEX__) << 2);
#elif defined(MW3)
	*(int*)(threadInfo + 0x10) = decryptDWORD(Not(NiNJA.cPatches)->CoD.G_ThreadValues) + (__THREADINDEX__ << 4);
#elif defined(GHOSTS)
	*(int*)(threadInfo + 0x70) = decryptDWORD(Not(NiNJA.cPatches)->CoD.G_ThreadValues) + (__THREADINDEX__ << 4);
#elif defined(AW)
	*(int*)(threadInfo + 0x1c) = decryptDWORD(Not(NiNJA.cPatches)->CoD.G_ThreadValues) + (__THREADINDEX__ * 0x14);
#elif defined(WAW)
	*(int*)(threadInfo + 0xc) = decryptDWORD(Not(NiNJA.cPatches)->CoD.G_ThreadValues) + (__THREADINDEX__ << 4);
#elif defined(BO3)
	*(int*)(threadInfo + 0x38) = decryptDWORD(Not(NiNJA.cPatches)->CoD.G_ThreadValues) + (__THREADINDEX__ * 0x14);
#else
	((void(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.Com_InitThreadData))(0);
#endif
}

void ExtraThread(int threadContext) {
	CG::bExtraThreadRunning = TRUE;
	
	// Setup our thread to run game funtions
	InitThreadData();

	bThreadRelaunching = false;

	// Don't exit the thread for at least a second
	dwLastMigrationTime = GetTickCount() + 1000;

	while (!(NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload)) {
	jRestart:
		//DWORD dwLoopStartTime = GetTickCount();

		// Exit our thread if we're not ingame and the host hasn't been migrating for at least a second
		if (GameFuncs::CheckIsMigrating()) {
			dwLastMigrationTime = GetTickCount() + 5000;
			bWasMigrating = true;
		}
		else if (bWasMigrating) {
			if (CG::iRealLocalClient >= 0 && CG::iRealLocalClient < __MAXCLIENTS__ && CG::cEntities.List[CG::iRealLocalClient].bAlive) {
				// After a host migration, run our OnPlayerSpawned function so we get our FOV/third person reset
				UI::mManager.OnPlayerSpawned();
				bWasMigrating = false;
			}
		}
		if (!GameFuncs::CheckInGame() && dwLastMigrationTime < GetTickCount()) {
			// Since we're exiting the game, we need to make sure the game settings like team inversion get reset next game
			bNewLobby = true;
			bWasMigrating = false;
			goto jExit;
		}
		if (bThreadRelaunching) {
			//printf("Thread relaunching!\n");
			// We're relaunching this thread, this condition should never get hit because if we're relaunching the thread then this thread must be dead.
			return;
		}

		// Restart our thread if we don't check in at least once every 5 seconds.
		dwLastCheckInTime = GetTickCount();

		if (CG::bLobbyInitialized) {
			//bAnyVisible = false;
			//bAnyWallbangable = false;
			if (CG::iLocalClient >= 0 && CG::iLocalClient < __MAXCLIENTS__) {
				//if (CG::dwNumLivingEnemies > 0) {
					for (int i = 0; i < __MAXENTITIES__; i++) {
						if (!GameFuncs::CheckInGame()) {
							goto jRestart;
						}
						if (NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
							CG::bExtraThreadRunning = FALSE;
							return;
						}

						if (CG::cEntities.List[i].bValid) {
							if (CG::cEntities.List[i].pExt && (CG::cEntities.List[i].bPlayer || CG::cEntities.List[i].bAI) && !CG::cEntities.List[i].pExt->bThreadRunning) {
								if (CG::cEntities.List[i].pExt->bSkipNextFrame) {
									if (!CG::cEntities.List[i].pExt->bPreferred) {
										CG::cEntities.List[i].pExt->bSkipNextFrame = false;
										continue;
									}
								}
								else if(!CG::cEntities.List[i].pExt->bPreferred || !CG::cEntities.List[i].pExt->bTopPlayer) {
									CG::cEntities.List[i].pExt->bSkipNextFrame = true;
								}
								if (i < __MAXCLIENTS__) {
									// Check if the client is whitelisted/blacklisted
									GameFuncs::CheckWhitelistBlacklist(i);
								}
								if (CG::cEntities.List[i].pExt->pDObj) {
									if (CG::cEntities.List[i].bEnemy) {
										// Only grab shield info and check if they are looking at us in multiplayer
										if (CG::bIsMultiplayer) {
											// Check if they're looking at us
											CG::cEntities.List[i].pExt->sLookingAtBoneIndex = GameFuncs::IsEntLookingAtMe(i);
										}
									}

									if (i != CG::iLocalClient) {
										// Check if the entity is invisible
										CG::cEntities.List[i].bInvisible = GameFuncs::IsEntInvisible(i);

										// Only run visibility and wallbang checks on enemies.
										if (CG::cEntities.List[i].bEnemy) {
											// Check if the enemy is killable
											CG::cEntities.List[i].bKillable = GameFuncs::IsEntKillable(i);

											// Check if any of their bones were visible
											CG::cEntities.List[i].pExt->bVisible = GameFuncs::IsEntVisible(i);

											if (!CG::cEntities.List[i].pExt->bVisible) {
												if (CG::bAutowall) {
													CG::cEntities.List[i].pExt->bWallbangable = Autowall::IsEntWallbangable(i);
												}
												else if (CG::cEntities.List[i].pExt->bWallbangable) {
													CG::cEntities.List[i].pExt->bWallbangable = false;
												}
											}
										}
									}
								}
							}
						}
					}
				//}
				// Check if we're looking at any entities
				CG::iLookingAtEntity = GameFuncs::IsLookingAtEntity();
			}
		}
		/*if (CG::cButtons.dwFlags & BUTTON_A) {
			if (!bJustSpammedInfo) {
				printf(GameFuncs::va("Thread execution took %ims\n", GetTickCount() - dwLoopStartTime));
				bJustSpammedInfo = true;
			}
		}
		else {
			bJustSpammedInfo = false;
		}*/
		Sleep(30);
	}
jExit:
	//printf("Extra thread exited!\n");
	CG::bExtraThreadRunning = FALSE;
}

bool TitleHooks::spawnExtraThread(PVOID pFunction, int iStackSize, int iArg) {
	// Initialize some locals
	HANDLE* threadId = NULL;
	int iMaxWorkers = 3;
#if defined(BO1)
	iMaxWorkers = 2;
#endif
	int iExtraAdditave = iMaxWorkers;

//jRetry:
	// Launch our thread in a suspended state
#if defined(WAW) || defined(BO2) || defined(MW1) || defined(MW2) || defined(MW3) || defined(GHOSTS) || defined(AW) || defined(BO1) || defined(BO3)
	HANDLE hExtraThread = CreateThread(0, iStackSize, (LPTHREAD_START_ROUTINE)pFunction, (PVOID)iArg, CREATE_SUSPENDED | EX_CREATE_FLAG_CORE2 | 0x18000424, 0);
//#elif defined(BO3)
//	HANDLE hExtraThread = ((HANDLE(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.Sys_CreateThread))(ExtraThread, __THREADINDEX__);
#else
	((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.Sys_CreateThread))(ExtraThread, __THREADINDEX__ + (iExtraAdditave-1));
	threadId = (HANDLE*)(decryptDWORD(Not(NiNJA.cPatches)->CoD.ThreadIndexArray));
	HANDLE hExtraThread = threadId[__THREADINDEX__ + (iExtraAdditave-1)];
#endif
	if (hExtraThread) {
		//if (pFunction == ExtraThread) {
		XSetThreadProcessor(hExtraThread, __TRACECORE__ );
		SetThreadPriority(hExtraThread, THREAD_PRIORITY_NORMAL);
		ResumeThread(hExtraThread);
		CloseHandle(hExtraThread);
			
		//printf(GameFuncs::va("Success starting our extra thread!\n"));
	}
	//else if(iExtraAdditave-1 > 0) {
	//	printf(GameFuncs::va("Failed to startup extra thread on worker %i, retrying!\n", iExtraAdditave));
	////	iExtraAdditave--;
	//	goto jRetry;
	//}
	return hExtraThread != 0;
}
#pragma endregion

/* CL_CreateNewCommands hook and all associated functions */
#pragma region CL_CreateNewCommands hook
bool  bWasKillable = false;
bool bAnyVisible = false, bAnyWallbangable = false;
VOID CL_CreateNewCommands_Pre() {
	// If our process is shutting down, skip the detour entirely
	if (NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
		return;
	}

	DWORD dwTmpLivingEnemies = 0;

	// Grab the current tick count, we'll use this so we can cache data for clients and only update it if our server time has progressed
	CG::dwLastCmdTick = GetTickCount();
	if (GameFuncs::CheckInGame() && CG::bLobbyInitialized) {
		// Loop through all our entities and cache info required for all our mods
		for (DWORD i = 0; i < __MAXENTITIES__; i++) {
			if (!GameFuncs::CheckInGame() || NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
				return;
			}

			// Just grab entity every frame.. this is required in BO3 and it doesn't hurt performance to do it on any of the other ones
			CG::cEntities.List[i].pCentity = GameFuncs::Get_CEntity_t(i);

			// Clientinfo, on the other hand, is populated by a hook on most games. So only assign to it if it's NULL
			if (CG::cEntities.List[i].pClientInfo == NULL) {
				CG::cEntities.List[i].pClientInfo = GameFuncs::Get_ClientInfo_t(i);
			}

		jRecheck:
			// Check if the entity is of a valid entityType
			CG::cEntities.List[i].sEntType = GameFuncs::GetEntityType(i);
			if (!GameFuncs::IsEntValid(i)) {
				if (CG::cEntities.List[i].bValid) {
					//DbgPrint("Client %i is no longer of valid entityType!", i);
					CG::cEntities.List[i].Reset(FALSE, i);
				}
				continue;
			}

			// Check if the entity is living
			CG::cEntities.List[i].bAlive = GameFuncs::IsEntLiving(i);
			if (!CG::cEntities.List[i].bAlive) {
				if (CG::cEntities.List[i].bValid) {
					//DbgPrintE("Client %i is no longer alive!", i);
					CG::cEntities.List[i].bValid = false;
				}
				continue;
			}

			bool bJustSpawned = false;
			// If we got this far and we aren't already valid, the entity just spawned in or just joined the game
			if (!CG::cEntities.List[i].bValid) {
				bool bPlayer = CG::cEntities.List[i].bPlayer, bAI = CG::cEntities.List[i].bAI;
				SHORT sEntType = CG::cEntities.List[i].sEntType;

				// Reset the entity to get rid of any old info
				CG::cEntities.List[i].Reset(FALSE);

				// Restore our entity info so we can alloc an extendedEntity if needed
				CG::cEntities.List[i].bPlayer = bPlayer;
				CG::cEntities.List[i].bAI = bAI;
				CG::cEntities.List[i].sEntType = sEntType;

				// If they're a player or AI, alloc an extendedEntity if it doesn't already exist
				if (CG::cEntities.List[i].bPlayer || CG::cEntities.List[i].bAI) {
					//printf(GameFuncs::va("%s %i just spawned in! (CEntity: %X, ClientInfo %X)\n", CG::cEntities.List[i].bEnemy ? "Enemy" : "Teammate", i, CG::cEntities.List[i].pCentity, CG::cEntities.List[i].pClientInfo));
					if (!CG::cEntities.List[i].pExt) {
						if (!CG::cEntities.List[i].AllocExtendedEntity()) {
							DbgPrintE("Failed to alloc extendedEntity for client %i!", i);
						}
					}
					if (CG::cEntities.List[i].pExt) {
						CG::cEntities.List[i].bValid = true;
					}
				}
				else {
					//if (CG::cEntities.List[i].sEntType == decryptDWORD(Not(NiNJA.cPatches)->CoD.ET_HELICOPTER)) {
					//	DbgPrintE("Heli %i spawned! cEntity: %X", i, CG::cEntities.List[i].pCentity);
					//}
					CG::cEntities.List[i].bValid = true;
				}

				// Finally, make sure they're a valid entity one last time before grabbing their info. This only happens once per spawn, per player.
				if (CG::cEntities.List[i].bValid) {
					bJustSpawned = true;
					goto jRecheck;
				}
			}

			if (CG::cEntities.List[i].bValid) {
				// Get the entities base location (from cEntity)
				CG::cEntities.List[i].vLocation = GameFuncs::GetEntityLocation(i);

				// Grab the entities current viewangles, after backing up their old viewangles
				CG::cEntities.List[i].vOldAngles = CG::cEntities.List[i].vAngles;
				CG::cEntities.List[i].vAngles = GameFuncs::GetEntityViewAngles(i);

				if (bJustSpawned) {
					CG::cEntities.List[i].vOldAngles = CG::cEntities.List[i].vAngles;
				}

				if (i != CG::iLocalClient) {
					// Get the distance between the client and us
					CG::cEntities.List[i].fDistance = Get3dDistance(CG::cEntities.List[CG::iLocalClient].vLocation, CG::cEntities.List[i].vLocation);
				}

				// Player and AI entities, these entities have an EntityExtention 
				if (CG::cEntities.List[i].pExt) {
					// Grab their team, and check if they're friendly or enemy
					CG::cEntities.List[i].sTeam = GameFuncs::GetEntityTeam(i);
					bool bEnemy = GameFuncs::IsEntEnemy(i);
					bEnemy = CG::cEntities.List[i].pExt->bInvertTeam ? !bEnemy : bEnemy;
					CG::cEntities.List[i].bEnemy = CG::bInvertTeams ? !bEnemy : bEnemy;

					// Grab a handle to their current dobj so we can use it to grab their xmodel
					CG::cEntities.List[i].pExt->pDObj = GameFuncs::Get_ClientDObj(i);

					if (CG::cEntities.List[i].pExt->pDObj) {
						/*vec2_t fFovDist = GameFuncs::GetRealFovDist(GameFuncs::GetLocalViewOrigin(), GameFuncs::Get_RefDef_ViewAngles(), CG::cEntities.List[i].pExt->cTags[TAG_HEAD].vPos);
						if (i != CG::iRealLocalClient) {
							DbgPrintE("FOV Dist is %f, %f", fFovDist.x, fFovDist.y);
						}*/

						// Grab the entities xmodel
						if (CG::cEntities.List[i].pExt->pXModel == NULL) {
							CG::cEntities.List[i].pExt->pXModel = GameFuncs::GetXModelForDObj(CG::cEntities.List[i].pExt->pDObj);
							//printf(GameFuncs::va("XModel for %s: %X\n", CG::cEntities.List[i].pExt->pRealName, CG::cEntities.List[i].pExt->pXModel));
							//printf(GameFuncs::va("Bone Info for tag j_head: %X\n", GameFuncs::XModelGetBoneInfo(CG::cEntities.List[i].pExt->pXModel, GameFuncs::GetBoneIndex(i, CG::cTags.List[TAG_HEAD].uNum))));
						}

						// Grab the entities weapon
						CG::cEntities.List[i].pExt->dwCurrentWeapon = GameFuncs::GetEntityWeapon(i, TRUE);
						CG::cEntities.List[i].pExt->dwSecondaryWeapon = GameFuncs::GetEntityWeapon(i, FALSE);

						// Check if we're using a shield
						if (i == CG::iLocalClient) {
							CG::cEntities.List[i].pExt->bPrimaryShield = GameFuncs::IsEntUsingShield(i, CG::cEntities.List[i].pExt->dwCurrentWeapon);

							// Grab our own scoreboard info
							CG::cEntities.List[i].pExt->cScore = GameFuncs::GetScoreboardInfo(i);
						}

						if (CG::bIsMultiplayer && (i == CG::iLocalClient || CG::cEntities.List[i].bEnemy)) {
							// Check if the enemy is using a shield
							CG::cEntities.List[i].pExt->bPrimaryShield = GameFuncs::IsEntUsingShield(i, CG::cEntities.List[i].pExt->dwCurrentWeapon);
							CG::cEntities.List[i].pExt->bSecondaryShield = GameFuncs::IsEntUsingShield(i, CG::cEntities.List[i].pExt->dwSecondaryWeapon);

							// If they have a shield, read info about it for our shield check and esp
							if (CG::cEntities.List[i].pExt->bPrimaryShield || CG::cEntities.List[i].pExt->bSecondaryShield) {
								CG::cEntities.List[i].pExt->oShieldInfo = GameFuncs::GetShieldInfo(i);
							}
						}

						if (CG::cEntities.List[i].bEnemy) {
							//if (CG::cEntities.List[i].pExt->bPreferred && CG::cEntities.List[i].pExt->bTopPlayer && !CG::cEntities.List[i].pExt->bThreadRunning) {
							//	if (TitleHooks::spawnExtraThread(ClientThread, 0, i)) {
							//		//printf(GameFuncs::va("Success spawning thread for client %i\n", i));
							//		CG::cEntities.List[i].pExt->bThreadRunning = true;
							//	}
							//}

							// Count how many enemies are alive each frame
							dwTmpLivingEnemies++;

							// If they're a normal player
							if (i < __MAXCLIENTS__) {
								CG::cEntities.List[i].pExt->bTopPlayer = GameFuncs::IsClientTopPlayer(i);

								// Grab info about their kills and death so we can calculate who the most deadly players are
								sScoreboardInfo cScore = GameFuncs::GetScoreboardInfo(i);

								// Save the score info for use in other places
								CG::cEntities.List[i].pExt->cScore = cScore;

								// For every 2 assists, add a kill to the players score
								cScore.dwKills += cScore.dwAssists > 1 ? (cScore.dwAssists / 2) : 0;

								// Calculate their kd ratio to use as a multiplier in our threat calculation
								cScore.fKillDeathRatio = cScore.dwDeaths > 0 ? cScore.dwKills / cScore.dwDeaths : cScore.dwKills;

								// Multiply our threat multiplier by the total number of kills they have, to get our final threat assessment
								CG::cEntities.List[i].pExt->iThreatAssessment = cScore.fKillDeathRatio * cScore.dwKills;
							}

							if (i != CG::iLocalClient && CG::iLocalClient == CG::iRealLocalClient) {
								CG::cEntities.List[i].pExt->fOldDelta = CG::cEntities.List[i].pExt->fDelta;
								CG::cEntities.List[i].pExt->fDelta = GetAngleDelta(CG::cEntities.List[i].vAngles.y, CG::cEntities.List[i].vOldAngles.y);

								// If they just spawned in, set their old delta to the current delta so we don't start to consider 
								// them a hacker because of the difference in their spawn angles and old angles before dying
								if (bJustSpawned) {
									CG::cEntities.List[i].pExt->fOldDelta = CG::cEntities.List[i].pExt->fDelta;
								}

								// Only check for hackers if our real client is alive
								if (CG::cEntities.List[CG::iRealLocalClient].bAlive) {
									if (abs(CG::cEntities.List[i].pExt->fOldDelta - CG::cEntities.List[i].pExt->fDelta) > 30) {
										//DbgPrint("%s is changing their angles rapidly.", CG::cEntities.List[i].pExt->GetName());
										if (!CG::cEntities.List[i].pExt->bEvaluatingHacker) {
											CG::cEntities.List[i].pExt->dwHackerCheckStartTime = GetTickCount();
											CG::cEntities.List[i].pExt->bEvaluatingHacker = true;
										}
										CG::cEntities.List[i].pExt->dwLastAbnormalAngle = GetTickCount();
									}

									//if (!CG::cEntities.List[i].pExt->bPreferred) {
									if (CG::cEntities.List[i].pExt->bEvaluatingHacker && CG::cEntities.List[i].pExt->dwHackerCheckStartTime + 7000 < GetTickCount()) {
										if (IsXUIDValid(CG::cEntities.List[i].pExt->cXuid)) {
											bool bBlacklisted = false;
											for (DWORD b = 0; b < CG::vBlacklisted.size(); b++) {
												if (CG::vBlacklisted[b] == CG::cEntities.List[i].pExt->cXuid) {
													// Already blacklisted, skip him
													bBlacklisted = true;
												}
											}

											if (!bBlacklisted) {
												if (CG::cEntities.List[i].pExt->cScore.fKillDeathRatio >= 1.0f) {
													CG::vBlacklisted.push_back(CG::cEntities.List[i].pExt->cXuid);
													CG::cEntities.List[i].pExt->bPreferred = true;
													DbgPrintE("Blacklisted %s as they were consistently changing their viewangles rapidly.", CG::cEntities.List[i].pExt->GetName());
												}
											}
											CG::cEntities.List[i].pExt->bUsingSpinbot = true;
										}
										CG::cEntities.List[i].pExt->bEvaluatingHacker = false;
									}
									else if (CG::cEntities.List[i].pExt->dwLastAbnormalAngle + 1000 < GetTickCount() && CG::cEntities.List[i].pExt->bEvaluatingHacker) {
										CG::cEntities.List[i].pExt->bEvaluatingHacker = false;
										CG::cEntities.List[i].pExt->bUsingSpinbot = false;
									}
								}
								else {
									CG::cEntities.List[i].pExt->bEvaluatingHacker = false;
									CG::cEntities.List[i].pExt->bUsingSpinbot = false;
								}
							}
						}

						// Read the position of all their tags for bone esp and aimbot
						vec3_t mins(FLT_MAX, FLT_MAX, FLT_MAX), maxes(-FLT_MAX, -FLT_MAX, -FLT_MAX);
						for (DWORD t = 0; t < TAG_SIZE; t++) {
							// Exit our loop as quickly as possible if we leave the game or are shutting down our cheats
							if (!GameFuncs::CheckInGame() || NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
								return;
							}
							// No need to grab all these bone positions for non prioritized clients
							if (!CG::cEntities.List[i].pExt->bPreferred && t > TAG_SPINEUPPER) {
								break;
							}

							// Grab the position of the tag
							GameFuncs::GetTagPosition(t, i, &CG::cEntities.List[i].pExt->cTags[t].vPos);

							if (CG::cEntities.List[i].bEnemy && CG::cEntities.List[i].pExt->cTags[t].fDamageMult == 0.0f) {
								// Grab the hit location multiplier for the clients current weapon, on the bone in question
								DWORD dwPartClassification = GameFuncs::XModelGetPartClassification(CG::cEntities.List[i].pExt->pXModel, GameFuncs::GetBoneIndex(i, CG::cTags.List[t].uNum));
								CG::cEntities.List[i].pExt->cTags[t].fDamageMult = GameFuncs::GetWeaponHitLocationMultiplier(dwPartClassification, CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon);
							}

							if (t > TAG_FOREHEAD && t <= TAG_SHOULDER_RI) {
								// Set our mins and maxes for our bounding box later
								mins.SetIfLessThan(CG::cEntities.List[i].pExt->cTags[t].vPos);
								maxes.SetIfGreaterThan(CG::cEntities.List[i].pExt->cTags[t].vPos);
							}
						}
						// Store the mins and maxes for our bounding box calculation in the ESP class
						CG::cEntities.List[i].pExt->vMins = mins;
						CG::cEntities.List[i].pExt->vMaxes = maxes;

						if (CG::cEntities.List[i].bPlayer) {
							// Grab the entities stance
							CG::cEntities.List[i].pExt->dwStance = GameFuncs::GetEntityStance(i);
						}
					}
				}
				// Non-player entities
				else {

				}
			}
		}

		// Store the number of clients currently alive
		CG::dwNumLivingEnemies = dwTmpLivingEnemies;

		vector<int> cTopPlayers;
		// Cycle through all clients 3 times, to determine the top 3 clients to prioritize over all others
		for (DWORD c = 0; c < 2; c++) {
			int iCurrentClient = -1;
			int iCurrentTop = -1;
			// Loop through all clients
			for (DWORD i = 0; i < __MAXCLIENTS__; i++) {
				bool bContinue = false;
				// Skip players who have already been marked as a higher rank
				for (DWORD t = 0; t < cTopPlayers.size(); t++) {
					if (cTopPlayers[t] == i) {
						bContinue = true;
						continue;
					}
				}
				if (bContinue) {
					continue;
				}
				// Make sure they have a valid pExt
				if (CG::cEntities.List[i].bValid && CG::cEntities.List[i].bAlive && CG::cEntities.List[i].pExt && CG::cEntities.List[i].bEnemy) {
					// Only allow them to be considered a "top player" if they have a positive kd, if they are preferred in the menu, or if we haven't yet selected the top player.
					if (CG::cEntities.List[i].pExt->cScore.fKillDeathRatio >= 1.0 || CG::cEntities.List[i].pExt->bPreferred || CG::cEntities.List[i].pExt->bHasKilledMe || c == 0) {
						// Check if their threat assessment is higher than the current top
						if (CG::cEntities.List[i].pExt->iThreatAssessment > iCurrentTop) {
							// If it is, store it 
							iCurrentTop = CG::cEntities.List[i].pExt->iThreatAssessment;
							iCurrentClient = i;
						}
					}
				}
			}
			// Store this client at the end of the list of top players
			cTopPlayers.push_back(iCurrentClient);
		}
		// Finally, save it to our global top players vector so we can use it from everywhere :)
		CG::cTopPlayers = cTopPlayers;

		// Check if we should autoshoot
		bool bUsingDualWield = CG::cEntities.List[CG::iLocalClient].pExt->bWeaponDualWield;
		if (bUsingDualWield) {
			if (CG::cEntities.List[CG::iLocalClient].pExt->sAmmoRightHand == 0) {
				bAutoShootFlip = true;
			}
			else if (CG::cEntities.List[CG::iLocalClient].pExt->sAmmoLeftHand == 0) {
				bAutoShootFlip = false;
			}
		}
		else {
			bAutoShootFlip = false;
		}

		bool bShouldTriggerBot = CG::bTriggerbot && CG::iLookingAtEntity >= 0 && CG::iLookingAtEntity < __MAXENTITIES__;
		if (bShouldTriggerBot) {
			bShouldTriggerBot = CG::cEntities.List[CG::iLookingAtEntity].pExt != 0 && CG::cEntities.List[CG::iLookingAtEntity].bEnemy &&  CG::cEntities.List[CG::iLookingAtEntity].pExt->bVisible;
		}

		bAnyVisible = false;
		bAnyWallbangable = false;
		for (int i = 0; i < __MAXENTITIES__; i++) {
			if (i != CG::iLocalClient && CG::cEntities.List[i].bValid && CG::cEntities.List[i].pExt && CG::cEntities.List[i].bEnemy && !CG::cEntities.List[i].pExt->bWhitelisted) {
				if (CG::cEntities.List[i].pExt->bVisible) {
					bAnyVisible = true;
				}
				else if (CG::cEntities.List[i].pExt->bWallbangable) {
					bAnyWallbangable = true;
				}
			}
		}
		ESP::bAnyEnemyVisible = bAnyVisible;
		ESP::bAnyEnemyWallbangable = bAnyWallbangable;

		//Check if we're dual wielding based off our current weapon
		CG::cEntities.List[CG::iRealLocalClient].pExt->bWeaponDualWield = GameFuncs::IsUsingDualWieldWeapon(CG::iRealLocalClient);

		// Grab our weapons clip size, and ammo count
		CG::cEntities.List[CG::iRealLocalClient].pExt->sWeaponClipSize = GameFuncs::GetWeaponClipSize(CG::iRealLocalClient);
		CG::cEntities.List[CG::iRealLocalClient].pExt->sAmmoRightHand = GameFuncs::GetWeaponAmmo(CG::iRealLocalClient, false);
		// Only grab ammo for our left gun if we're dual wielding
		if (CG::cEntities.List[CG::iRealLocalClient].pExt->bWeaponDualWield) {
			CG::cEntities.List[CG::iRealLocalClient].pExt->sAmmoLeftHand = GameFuncs::GetWeaponAmmo(CG::iRealLocalClient, true);
		}

		// Check whether or not we should autoshoot
		CG::bShouldAutoShoot = CG::dwAimbotType != AIM_TYPE_OFF && !CG::cEntities.List[CG::iLocalClient].pExt->bPrimaryShield && ((bShouldTriggerBot && GameFuncs::GetZoomState() >= CG::fTriggerbotZoom) || ((CG::bAutoShoot && (bAnyVisible || bAnyWallbangable))));

		// Finally, set our final flags we'll use to decide if we should fire with the left/right weapon
		CG::cButtons.bFiring = CG::cButtons.dwFlags & BUTTON_RT || bLastRTVal > 0 || (CG::cEntities.List[CG::iLocalClient].pExt->sAmmoRightHand > 0 && GameFuncs::GetWeaponCooldownTime() <= 0 && !bAutoShootFlip && CG::bShouldAutoShoot);
		CG::cButtons.bFiringSecondary = ((bLastLTVal > 0 || CG::cButtons.dwFlags & BUTTON_LT) && bUsingDualWield) != 0 || (CG::cEntities.List[CG::iLocalClient].pExt->sAmmoLeftHand > 0 && GameFuncs::GetWeaponCooldownTime(true) <= 0 && bAutoShootFlip && CG::bShouldAutoShoot);

		// Run our aimbot logic, if our aimbot isnt disabled and we're not using silent aim
		if (CG::dwAimbotType != AIM_TYPE_OFF && (CG::dwAimbotType != AIM_TYPE_SILENT || CG::bInKillstreak)) {
			Aimbot::Rape();
		}

		if (CG::bIsMultiplayer) {
			// Update our scores from the server every 2.5 seconds... this only happens if the game isn't requesting the scores its self.
			if (dwScoreEndTime < GetTickCount()) {
				// Update our scores from the server
				GameFuncs::AddReliableCommand(__SCORECMD__);
			}
		}
	}
}

VOID CL_CreateNewCommands_Post() {
	// If our process is shutting down, skip the detour entirely
	if (NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
		return;
	}

	// If we're ingame and the lobby is initialized
	if (GameFuncs::CheckInGame() && CG::bLobbyInitialized) {
		// Store our current stance
		CG::dwCurrentStance = UserCmd::GetStance();

		// Add a new usercmd for our hax :D
		UserCmd::AddUserCommand();

		if (CG::cEntities.List[CG::iRealLocalClient].pExt) {
			bool bAutomaticWeapon = GameFuncs::IsWeaponFullAuto(CG::cEntities.List[CG::iRealLocalClient].pExt->dwCurrentWeapon) || CG::bInKillstreak;
			DWORD dwFiringCmd = NULL, dwOtherCmd = NULL;
		#if defined(MW2) || defined(MW3) || defined(GHOSTS) || defined(AW)
			dwOtherCmd = UserCmd::GetCurrentUserCmd();
			dwFiringCmd = UserCmd::GetOldUserCmd();
		#else
			dwFiringCmd = UserCmd::GetCurrentUserCmd();
			dwOtherCmd = UserCmd::GetOldUserCmd();
		#endif

			if (CG::cButtons.bFiring) {
				bool bIsFiring = UserCmd::IsAttacking(dwFiringCmd);
				if (bIsFiring) {
					// If we're already firing, wipe out our current and old cmd of firing flags and set them manually.
					//DbgPrint("We're holding the trigger!");
					UserCmd::SetInAttack(dwOtherCmd, false);
					UserCmd::SetInAttack(dwFiringCmd, false);
				}

				if (bLastRTVal > 0 && !bPressingRT) {
					bJustFired = false;
					dwRightDelay = 0;
					bPressingRT = true;
					bLastRTVal = 0;
				}
				else if (bPressingRT && bLastRTVal == 0) {
					bPressingRT = false;
				}

				if (!bAutomaticWeapon && !CG::bInKillstreak) {
				#if !defined(MW3) && !defined(GHOSTS) && !defined(AW) && !defined(BO3) && !defined(WAW)
					if (((CG::bRapidFire || CG::bJitterEnabled) && dwRightDelay == 0) || ((!CG::bRapidFire && !CG::bJitterEnabled) && !bJustFired)) {
				#else
					if ((CG::bRapidFire && dwRightDelay == 0) || (!CG::bRapidFire && !bJustFired)) {
				#endif
						UserCmd::SetInAttack(dwFiringCmd, true);
						bJustFired = true;
					}
				}
				else {
					UserCmd::SetInAttack(dwFiringCmd, true);
					if (CG::dwAimbotType == AIM_TYPE_HUMANIZED || CG::dwAimbotType == AIM_TYPE_OFF) {
						UserCmd::SetInAttack(dwOtherCmd, true);
					}
					bJustFired = true;
				}

				dwRightDelay++;
			#if !defined(MW3) && !defined(GHOSTS) && !defined(AW) && !defined(BO3) && !defined(WAW)
				if (dwRightDelay > CG::dwRapidFireDelay || CG::bJitterEnabled) {
			#else
				if (dwRightDelay > CG::dwRapidFireDelay) {
			#endif
					dwRightDelay = 0;
				}

			#if !defined(MW3) && !defined(GHOSTS) && !defined(AW) && !defined(BO3) && !defined(WAW)
				if (!bAutomaticWeapon && ((!bApplyJitter && !bSwitchFlip) || dwLastWeapSwitchTime + 1000 < GetTickCount()) && CG::bJitterEnabled) {
					//printf("jat\n");
					dwLastWeapSwitchTime = GetTickCount();
					bApplyJitter = true;
					bSwitchFlip = true;
					//GameFuncs::SendConsoleCommand("weapnext");
					//((VOID(*)(...))0x821408C8)(0, 0xb, 0x3); //mw2
					//((VOID(*)(...))0x821408C8)(0, 0xc, 0x3); //mw2
					((BOOL(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CycleWeapPrimary))(0, 1, 0, 0);
				}
			#endif
			}
			else {
				bJustFired = false;
				bApplyJitter = false;
			}


			if (CG::cButtons.bFiringSecondary) {
				bool bIsFiringSecondary = UserCmd::IsAiming(dwFiringCmd);
				if (bIsFiringSecondary) {
					// If we're already firing, wipe out our current and old cmd of firing flags and set them manually.
					//DbgPrint("We're holding the trigger!");
					UserCmd::SetIsAiming(dwOtherCmd, false);
					UserCmd::SetIsAiming(dwFiringCmd, false);
				}


				if (bLastLTVal > 0 && !bPressingLT) {
					bJustAimed = false;
					dwLeftDelay = 0;
					bPressingLT = true;
					bLastLTVal = 0;
				}
				else if (bPressingLT && bLastLTVal == 0) {
					bPressingLT = false;
				}

				if (!bAutomaticWeapon) {
					if ((CG::bRapidFire && dwLeftDelay == 0) || (!CG::bRapidFire && !bJustAimed)) {
						UserCmd::SetIsAiming(dwFiringCmd, true);
						bJustAimed = true;
					}
				}
				else {
					UserCmd::SetIsAiming(dwFiringCmd, true);
					if (CG::dwAimbotType == AIM_TYPE_HUMANIZED || CG::dwAimbotType == AIM_TYPE_OFF) {
						UserCmd::SetIsAiming(dwOtherCmd, true);
					}
					bJustAimed = true;
				}

				dwLeftDelay++;
				if (dwLeftDelay > CG::dwRapidFireDelay) {
					dwLeftDelay = 0;
				}
			}
			else {
				bJustAimed = false;
			}

		#if defined(MW1) || defined(GHOSTS) || defined(AW) || defined(BO3)
			// Run our aimbot logic, if our aimbot isnt disabled and we're using silent aim
			if (CG::dwAimbotType != AIM_TYPE_OFF && CG::dwAimbotType == AIM_TYPE_SILENT && !CG::bInKillstreak) {
				Aimbot::Rape();
			}

			// Apply our nospread angles, if we're firing
			if (CG::bNoSpread) {
				NoSpread::Apply();
			}
	#endif
		}
	}
}

VOID CL_CreateNewCommands_Hook(DWORD dwLocalClient) {
	// Call our pre-hook function
	CL_CreateNewCommands_Pre();

	// Call our original function
	CL_CreateNewCommands_Detour.CallOriginal(dwLocalClient);

	// Call our post-hook function
	CL_CreateNewCommands_Post();
}
#pragma endregion

/* R_EndFrame hook and all assoiciated functions */
#pragma region R_EndFrame hook
DWORD dwIpSpoofTimer = 0;
CHAR pOldMapName[50];
DWORD WINAPI TitleHooks::TitleThread(void *playInfo) {
	//// Delayed startup on this thread, to avoid crashing
	//Sleep(4000);

	DbgPrint("TitleThread has started!");

	CG::bTitleThreadRunning = TRUE;
	while (!(NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload || bIgnoreHttpRequests)) {
		if (UI::bInitialized) {
			if (CG::bResetGamertag) {
				XUID myXuid;
				if (XUserGetXUID(0, &myXuid) == 0) {
					if (CG::bInGameCached) {
						if (CG::cEntities.List[CG::iRealLocalClient].pExt) {
							CG::cEntities.List[CG::iRealLocalClient].pExt->cXuid = myXuid;
						}
						else {
							goto jOut;
						}
					}
					PCHAR myGT = GameFuncs::GetGamertagFromXUID(CG::iRealLocalClient, myXuid);
					if (myGT) {
						GameFuncs::SetGamertag(myGT);
					}
					CG::bResetGamertag = false;
				}
			}

		jOut:
			// Cycle through each client, and if their XUID changed regrab their user info
			for (DWORD i = 0; i < __MAXENTITIES__; i++) {
				// If we aren't ingame, allocate extended entities for all clients who don't already have one
				if (!CG::bInGameCached) {
					if (i < __MAXCLIENTS__ || CG::cEntities.List[i].bPlayer || CG::cEntities.List[i].bAI) {
						if (!CG::cEntities.List[i].pExt) {
							if (!CG::cEntities.List[i].AllocExtendedEntity()) {
								//DbgPrint("Failed to alloc extendedEntity for client %i!", i);
							}
						}
					}
				}
				if (CG::cEntities.List[i].pExt) {
					// Grab their netinfo address
					CG::cEntities.List[i].pExt->pNetInfo = GameFuncs::Get_NetInfo(i);

					// Grab their gamertag & IP from our netinfo struct, so we can see if they have spoofed their gt
					CG::cEntities.List[i].pExt->pName = GameFuncs::GetClientGamertagFromNetInfo(i);

					// Try to read their xuid
					QWORD tmpXuid = i < __MAXCLIENTS__ ? GameFuncs::GetClientXUID(i) : 420;
					// Check if their xuid changed
					if (tmpXuid != 420 && CG::cEntities.List[i].pExt->cXuid != tmpXuid) {
						// If the new XUID is valid
						if (IsXUIDValid(tmpXuid)) {
							// Set the XUID in the entity struct to the new one, so we dont spam GetClientGamertag
							CG::cEntities.List[i].pExt->cXuid = tmpXuid;

							// Mark them as not whitelisted and not preferred as they are a new client
							CG::cEntities.List[i].pExt->bWhitelisted = false;
							CG::cEntities.List[i].pExt->bPreferred = false;
							CG::cEntities.List[i].pExt->bInvertTeam = false;
							CG::cEntities.List[i].pExt->bHasKilledMe = false;

							// Reset their old location info, since its a new player
							CG::cEntities.List[i].pExt->cGeoIPInfo.Reset();

							// Read their gamertag from their XUID 
							CG::cEntities.List[i].pExt->pRealName = GameFuncs::GetClientGamertag(i);

							//DbgPrintE("Client %i gt was %s", i, CG::cEntities.List[i].pExt->pName);
							UI::mManager.Menus[MENU_CLIENTS].Options.Get(i)->pDisplayName = CG::cEntities.List[i].pExt->GetName();

							// Grab their current IP since they have a valid xuid
							CG::cEntities.List[i].pExt->cIP = GameFuncs::GetClientIP(i);

							// Grab their geo ip info
							CG::cEntities.List[i].pExt->bGrabGeoIPInfo = true;
						}
						// Otherwise, set their name as NULL. If its some other ent or AI code will grab their name ingame.
						else {
							bool bGuest = false;
							int bMainIndex = -1;
							PCHAR pClientName = CG::cEntities.List[i].pExt->pName;
							if (pClientName && strlen(pClientName) >= 3) {
								if (pClientName[strlen(pClientName) - 3] == '(' && pClientName[strlen(pClientName) - 1] == ')') {
									for (int p = 0; p < __MAXCLIENTS__; p++) {
										if (CG::cEntities.List[p].pExt) {
											PCHAR pTestName = CG::cEntities.List[p].pExt->pRealName;
											// If they have a real gamertag
											if (pTestName) {
												// If both clients have valid names, lets begin to compare them.
												if (strlen(pClientName) - 3 <= strlen(pTestName)) {
													bGuest = true;
													for (unsigned int c = 0; c < strlen(pClientName) - 3; c++) {
														if (pClientName[c] != pTestName[c]) {
															bGuest = false;
															break;
														}
													}
												}
											}
										}
										if (bGuest) {
											bMainIndex = p;
											break;
										}
									}
								}
							}
							// If they're a guest, mark them as such. Otherwise, wipe out their old display name
							if (bGuest) {
								UI::mManager.Menus[MENU_CLIENTS].Options.Get(i)->pDisplayName = CG::cEntities.List[i].pExt->pName;
								if (CG::cEntities.List[i].pExt->cIP.S_un.S_addr != CG::cEntities.List[bMainIndex].pExt->cIP.S_un.S_addr) {
									CG::cEntities.List[i].pExt->cXuid = 0;

									// Grab our guest ip from the main acct
									CG::cEntities.List[i].pExt->cIP = CG::cEntities.List[bMainIndex].pExt->cIP;
									CG::cEntities.List[i].pExt->bInvertTeam = false;
									CG::cEntities.List[i].pExt->bHasKilledMe = false;

									// Free our real name if it already exists
									if (CG::cEntities.List[i].pExt->pRealName) {
										if (sManager.free(CG::cEntities.List[i].pExt->pRealName)) {
											CG::cEntities.List[i].pExt->pRealName = NULL;
										}
									}

									// Reset their old location info, since its a new player
									CG::cEntities.List[i].pExt->cGeoIPInfo.Reset();

									// Grab their geo ip info from our geo ip apis
									CG::cEntities.List[i].pExt->bGrabGeoIPInfo = true;
								}
							}
							else {
								//if (!CG::bInGameCached && CG::cEntities.List[i].pExt->pName) {
								//	// Reset their name if we're not in game and they don't have a valid xuid, and they aren't a guest.
								//	sManager.free(CG::cEntities.List[i].pExt->pRealName);
								//	CG::cEntities.List[i].pExt->pRealName = NULL;
								//	CG::cEntities.List[i].pExt->pName = NULL;
								//}
								UI::mManager.Menus[MENU_CLIENTS].Options.Get(i)->pDisplayName = "Searching...";
							}
						}
					}
					else {
						// Update their geo IP info if any of it is missing, and bGrabGeoIP is set to false
						if (!CG::cEntities.List[i].pExt->bGrabGeoIPInfo && Contains(CG::cEntities.List[i].pExt->cGeoIPInfo.city, "Loading...")) {
							CG::cEntities.List[i].pExt->bGrabGeoIPInfo = true;
						}

						// They were a bot or some other AI, so they don't have a real name, only a fake name.
						if (CG::bInGameCached && CG::cEntities.List[i].bValid) {
						//#if !defined(BO3)
							// If they don't have a real name
							if (CG::cEntities.List[i].pExt->pRealName == NULL) {
						//#endif
								// Read it from clientInfo
								CG::cEntities.List[i].pExt->pName = GameFuncs::GetEntityName(i);
						//#if !defined(BO3)
							}
						//#endif
						}
						// Constantly update our menu display names
						if (i < __MAXCLIENTS__) {
							if (!CG::bInGameCached) {
								if (!IsXUIDValid(CG::cEntities.List[i].pExt->cXuid)) {
									CG::cEntities.List[i].Reset();
								}
							}
							PCHAR pName = CG::cEntities.List[i].pExt->GetName();
							if (pName) {
								MenuOption *option = UI::mManager.Menus[MENU_CLIENTS].Options.Get(i);
								if (option) {
									option->pDisplayName = pName;
								}
							}
						}
					}
				}
				if (i < __MAXCLIENTS__) {
					// Make sure no blank options exist in the client menu
					if (Compare(UI::mManager.Menus[MENU_CLIENTS].Options.Get(i)->pDisplayName, __EMPTYSTR__)) {
						UI::mManager.Menus[MENU_CLIENTS].Options.Get(i)->pDisplayName = "Searching...";
					}
				}
			}
		}
		Sleep(1000);
	}
	DbgPrint("TitleThread has exited!");
	CG::bTitleThreadRunning = FALSE;
	return 0;
}

HANDLE hThread = NULL;
VOID InitialSetup() {
	// Create a thread to check for xuid/gamertag changes - PUT ANY POTENTIALLY BLOCKING OPERATIONS THAT MUST BE CALLED FROM A TITLE THREAD HERE!
	LaunchSysThread((LPTHREAD_START_ROUTINE)TitleHooks::TitleThread, 0, &hThread, 0, THREAD_PRIORITY_BELOW_NORMAL, FALSE, 16000);

	// Initialize our menu HudElems
	UI::Initialize();

	bFirstRun = FALSE;
}

int iLastPrestige = 0;
PCHAR bChangelogBuffer = NULL, pChangelogVersion, pChangelogMessage, pChangelogTitle;
PCHAR pChangeLogData[3] = { NULL,NULL,NULL };
CHAR pContinueText[50] = { 0 };
bool bPressingA = false;
int iChangelogVersion = 0;
int iNumberOfLines = 0;
bool bTryGrabChangelog = true;
bool MessageBoxOpen() {
	if (NiNJA.bHasReadConfig && bTryGrabChangelog) {
	#if defined(BO1) || defined(MW2) || defined(MW3) || defined(WAW) || defined(BO3)
		FLOAT fScale = 0.7f;
	#else
		FLOAT fScale = 0.8f;
	#endif
		FLOAT fBoxWidth = 440.0f;
		if (!pChangelogVersion || !pChangelogTitle || !pChangelogMessage) {
			int iRetries = 0;
			int iMaxRetries = 2;
		jRetry:
			HttpRequest req;
			BOOL bSuccess = FALSE;
			CHAR pSubDomain[100];
			PCHAR pTmp = GetRandomSubDomain();
			ZeroMemory(pSubDomain, 100);
			sprintf(pSubDomain, pTmp, iRetries < 1 ? "xbl" : "xbls");
			bChangelogBuffer = (PCHAR)req.SendRequest(pSubDomain, "/authed/changelog", FALSE, &bSuccess);
			if (bSuccess) {
				int iNewlinesFound = 0;
				PCHAR pLastStart = bChangelogBuffer;
				BYTE dwNumLinesWrapped = 0;
				for (DWORD i = 0; i < req.dwBytesRead; i++) {
					if (bChangelogBuffer[i] == '\n') {
						if (iNewlinesFound <= 2) {
							if (iNewlinesFound < 2) {
								bChangelogBuffer[i] = '\0';
							}
							PCHAR pNewString = NULL;
							int iStringSize = &bChangelogBuffer[i] - pLastStart;

							string sWrapped;
							if (iNewlinesFound == 2) {
								sWrapped = HudElems::WrapText(pLastStart, fBoxWidth, fScale, CG::dwSmallFont, &dwNumLinesWrapped);
								iStringSize = sWrapped.size();
							}
							if (sManager.alloc(iStringSize + 1, &pNewString)) {
								if (iNewlinesFound == 2) {
									strcpy(pNewString, sWrapped.c_str());
								}
								else {
									strcpy(pNewString, pLastStart);
								}
								pChangeLogData[iNewlinesFound] = pNewString;
							}
							pLastStart = &bChangelogBuffer[i] + 1;
							bChangelogBuffer[i] = '\n';
						}
						iNewlinesFound++;
					}
				}
				iNumberOfLines = (iNewlinesFound - 2) + dwNumLinesWrapped;
				XSAFE_FREE(bChangelogBuffer);
				bChangelogBuffer = NULL;
			}
			else if (iRetries < iMaxRetries) {
				iRetries++;
				goto jRetry;
			}
			if (pChangeLogData[0] && pChangeLogData[1] && pChangeLogData[2]) {
				pChangelogVersion = pChangeLogData[0];
				pChangelogTitle = pChangeLogData[1];
				pChangelogMessage = pChangeLogData[2];

				iChangelogVersion = atoi(pChangelogVersion);
				if ((DWORD)iChangelogVersion > CG::dwVersion) {
					UI::bMessageBoxOpen = true;
				}
			}
			else {
				// Failed to grab changelog from server, skip it!
				UI::bMessageBoxOpen = FALSE;
				return false;
			}
			//else {
			//	XSAFE_FREE(bChangelogBuffer);
			//	bChangelogBuffer = NULL;
			//}
		}


		if (pChangelogVersion && pChangelogTitle && pChangelogMessage && UI::bMessageBoxOpen) {
			FLOAT fFontHeight = GameFuncs::GetFontHeight(CG::dwSmallFont, fScale);
			FLOAT fTitlePadding = 30.0f;
			FLOAT fTopHeight = GameFuncs::GetFontHeight(CG::dwSmallFont, 1.0f) + fTitlePadding; //40.0f * 2;
			FLOAT fWidth = GameFuncs::GetTextWidth(pChangelogMessage, CG::dwSmallFont);
			FLOAT fMessageHeight = ((iNumberOfLines + 1) * fFontHeight);
			FLOAT x = NiNJA.dwScreenCenterX - ((fWidth*fScale) / 2), y = NiNJA.dwScreenCenterY - (fMessageHeight / 2) - (fTopHeight / 2);

			GameFuncs::DrawBox(x - 10.0f, y, fBoxWidth + 20.0f, y + fMessageHeight, 0.0f, CG::cMenuBackgroundColor, CG::cMenuBorderColor, 2.0f);
			//GameFuncs::DrawBox(x - 10.0f, y, fBoxWidth + 20.0f, fTopHeight, 0.0f, CG::cTopBottomColor, CG::cMenuBorderColor, 2.0f);

			FLOAT fChangelogWidth = GameFuncs::GetTextWidth(pChangelogTitle, CG::dwSmallFont);
			GameFuncs::DrawText(pChangelogTitle, fChangelogWidth, CG::dwSmallFont, NiNJA.dwScreenCenterX - fChangelogWidth / 2, y + fFontHeight + (fTitlePadding / 2), 1.0f, 1.0f, 0.0f, CG::cTitleTextColor);
			GameFuncs::DrawText(pChangelogMessage, fWidth, CG::dwSmallFont, x, y + fTopHeight + fFontHeight, fScale, fScale, 0.0f, CG::cMenuTextColor);

			if (pContinueText[0] == NULL) {
				string sText = ReplaceButtonsInString(string("Press [[BUTTON_A]] to Continue"));
				strcpy(pContinueText, sText.c_str());
			}

			FLOAT fTextWidth = GameFuncs::GetTextWidth(pContinueText, CG::dwSmallFont);
			FLOAT fScale = 1.0f;
			if (NiNJA.dwScreenMaxY < 720) {
				fScale -= 0.2f;
			}
			//GameFuncs::DrawBox((FLOAT)NiNJA.dwScreenMaxX - (fTopLeftOffsetX * 2) - fTextWidth - 4.0f, NiNJA.dwScreenMaxY - fTopLeftOffsetY*1.1f - (CG::fMenuFontHeight *  1.1f), fTextWidth + 8.0f + 2.0f, (CG::fMenuFontHeight * 1.1f) + 4.0f, 0.0f, CG::cTopRightBackgroundColor);// , Colors::WhiteTrans, 2.0f);
			GameFuncs::DrawText(pContinueText, fTextWidth, CG::dwSmallFont, (FLOAT)NiNJA.dwScreenMaxX - fTextWidth * fScale - ESP::fTopLeftOffsetX, NiNJA.dwScreenMaxY - ESP::fTopLeftOffsetY, fScale, fScale, 0.0f, CG::cTopRightTextColor, true, 4.0f);

			if (bPressingA) {
				CG::dwVersion = iChangelogVersion;
				UI::mManager.bShouldSave = TRUE;
				UI::bMessageBoxOpen = false;
			}
			return true;
		}
		bTryGrabChangelog = false;
	}
	return false;
}

VOID R_EndFrame_Ext() {
	// If our process is shutting down, skip the detour entirely
	if (NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
		//if (hThread) {
		//	if (XCloseHandle(hThread)) {
		//		DbgPrint("Name thread handle closed!");
		//	}
		//}
		if (UI::bInitialized) {
			UI::Shutdown();
		}
		return;
	}

	//// Run our frame in extraThread
	//bRunFrame = true;

	// Run our initial setup
	if (bFirstRun) {
		InitialSetup();
	}

	// Keep grabbing our tick count, for ui purposes
	NiNJA.dwTickCount = GetTickCount();

	if (MessageBoxOpen()) {
		return;
	}

	#if !defined(BO2)
		CG::bInGameCached = GameFuncs::CheckInGame();
	#else 
		bool bInGame = GameFuncs::CheckInGame();
		if (bInGame) {
			DWORD cgs_t = *(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.CGS_t);
			if (cgs_t) {
				 CG::bInGameCached = bInGame && *(DWORD*)(cgs_t + 0x1660) != 0;
			}
			else {
				CG::bInGameCached = false;
			}
		}
		else {
			CG::bInGameCached = bInGame;
		}
	#endif

	if (CG::bInGameCached && !CG::bLobbyInitialized) {
		// Make sure our localClient index has been set
		if (!GameFuncs::GetLocalClientIndex()) {
			DbgPrint("Failed to get our localClientIndex!");
			return;
		}

		// Grab our real localClient index
		CG::iRealLocalClient = GameFuncs::GetStaticLocalClientIndex();

		DbgPrintE("We are client %i!", CG::iRealLocalClient);

		if (CG::bCycleGamertags) {
			// Grab our name and set our initial gamertag
			PCHAR myGT = GameFuncs::GetClientGamertag(CG::iRealLocalClient);
			if (!myGT) {
				DbgPrint("Failed to get our real gamertag!");
				return;
			}
			GameFuncs::SetGamertag(myGT);
		}

		// Cache our tags
		for (DWORD i = 0; i < CG::cTags.List.size(); i++) {
			CG::cTags.List[i].uNum = GameFuncs::CacheTag(CG::cTags.List[i].pName);
		}
		DbgPrint("Success caching tags!");

		if (__SCAVENGERWEAPNAME__ != "") {
			CG::dwScavPackWeaponID = GameFuncs::GetWeaponIndexForName(__SCAVENGERWEAPNAME__);
		}

		// Check if we're in multiplayer or not
		CG::bIsMultiplayer = GameFuncs::IsMultiplayer();

		// Cache our riot shield weapon indicies 
		GameFuncs::CacheShield();

	#if defined(AW)
		// The hud moves on aw, and it makes our custom radar look like shit... disable this.
		GameFuncs::SendConsoleCommand(ES("lui_hud_motion_enabled 0"));
	#endif

		// Grab our gametype name, so its cached for our team checks on dogs and other ai in ffa
		GameFuncs::GetGametypeName();
		DbgPrintE("Gametype is %s (%s)", CG::pGameTypeDisplayName, CG::pGameTypeName);

		// Reset our local health
		CG::iLocalHealth = GameFuncs::GetLocalClientHealth();

		if (CG::bIsMultiplayer) {
			// Request our current score from the server
			GameFuncs::AddReliableCommand(__SCORECMD__);
		}

		sScoreboardInfo cScore = GameFuncs::GetScoreboardInfo(CG::iRealLocalClient);
	#if defined(MW1) || defined(MW2) || defined(MW3)
		// If we changed maps or our score changed... we started a new game
		if (bNewLobby) {
			// Turn off team inversion if we had it enabled
			CG::bInvertTeams = false;
			bNewLobby = false;
		}
		else {
			DbgPrint("The host just migrated!");
		}

		// Grab the last host name so we can check if the host has migrated
		strcpy(pOldMapName, GameFuncs::GetMapName());
	#else
		CG::bInvertTeams = false;
	#endif

		// Reset our count of living clients, incase it was somehow already set
		CG::dwNumLivingEnemies = 0;

		if (!CG::bExtraThreadRunning) {
			// Launch our extra thread
			CG::bLaunchExtraThread = TRUE;
		}

		// Disable the sound thread on aw, cuz it doesn't have enough memory free 
	#if defined(AW)
		CG::bDisableSoundThread = TRUE;
	#endif

		DbgPrint("We have entered the game!");
		CG::bLobbyInitialized = TRUE;
	}
	else if (!CG::bInGameCached && CG::bLobbyInitialized) {
		// Store our old local client index so we can compare the score and shit after the host migrates and check if it matches ours
		cOldScore = CG::cEntities.List[CG::iRealLocalClient].pExt->cScore;

		// Reset our entity struct
		CG::cEntities.Reset();

		DbgPrint("We have left the game!");
		CG::bLobbyInitialized = FALSE;
	}
	else if (CG::bInGameCached && CG::bLobbyInitialized) {
		// Constantly grab our localClientIndex, if it fails mark us as "not ingame" anymore
		if (!GameFuncs::GetLocalClientIndex()) {
			// We failed to get a valid localClientIndex
			CG::iLocalClient = CG::iRealLocalClient;
		}

		// Start up our sounds thread
		if (!CG::bDisableSoundThread) {
			if (!pSounds.bInitialized) {
				if (pSounds.Start()) {
					pSounds.bInitialized = true;
				}
				else {
					pSounds.bInitialized = false;
				}
			}
		}

		// Spawn in our extra thread for our traces
		if (CG::bLaunchExtraThread || (CG::bExtraThreadRunning && dwLastCheckInTime + 5000 < GetTickCount())) {
			if (CG::bExtraThreadRunning) {
				// If our other thread is still responsive, make sure it exits gracefully
				bThreadRelaunching = true;
			}
			if (CG::iLocalClient == CG::iRealLocalClient) {
				if (TitleHooks::spawnExtraThread(ExtraThread)) {
					//printf("Success spawning extra thread!\n");
					CG::bLaunchExtraThread = FALSE;
				}
			}
		}

	#if defined(BO2)
		// If our "partyStarted" flag has been set, shut it off once we're in game so that when we leave a game our ip gets spoofed again
		if (CG::bSpoofIP && (*(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.SystemIPAddress) == dwOrigIP || !bPartyStarted)) {
			GameFuncs::SpoofIPAddress(0x01030307);
			bPartyStarted = true;
		}
	#endif

		// Draw our esp if we're ingame and lobby is initialized
		if (UI::bInitialized) {
			ESP::Draw();
		}
	}
	// Prevent from being deranked via the standard stat reset
	GameFuncs::PreventDerank();

	if (UI::bInitialized) {
		if (!CG::bConnected) {
			if (CG::bPrestigeDisco) {
				if (dwPrestigeDiscoTime + 1600 < NiNJA.dwTickCount) {
				jRetry:
					int iPrestige = rand() % (__MAXPRESTIGE__ + 1);
					if (iPrestige == iLastPrestige) {
						goto jRetry;
					}
					iLastPrestige = iPrestige;
					GameFuncs::SetPrestige(iPrestige);
					dwPrestigeDiscoTime = NiNJA.dwTickCount;
				}
			}
		}

		// Check if we have requested for the menu to move up a level from a system thread
		if (UI::mManager.bUpOneLevel) {
			UI::mManager.EnterMenu(UI::mManager.Menus[UI::mManager.dwCurrentMenu].mLastMenu, TRUE, FALSE);
			UI::mManager.bUpOneLevel = false;
		}

		// Make sure our client menu is always up to date with the most recent info
		if (UI::mManager.dwCurrentMenu == MENU_SELECTED_CLIENT) {
			UI::mManager.UpdateClientMenu();
		}

		// Mod our gamertag
		GameFuncs::ModGamertag();
		
		// Monitor our menu button presses
		UI::mManager.MonitorButtons();

		// Draw all our HudElems
		HudElems::ProcessHudElems();

		// Draw our debug overlay in the top right hand corner
		ESP::DrawInformationOverlay();

		// Refresh our menu ui to reflect any changes we made this frame
		UI::mManager.RefreshMenuText();
	}
}

/* Function that gets called when R_EndFrame is called */
VOID R_EndFrame_Hook() {
	// Call our R_EndFrame extention function
	R_EndFrame_Ext();

	R_EndFrame_Detour.CallOriginal(0);
}
#pragma endregion

/* XamInputGetState hook and all associated functions*/
#pragma region XamInputGetState hook
/* Function that gets called when XamInputGetState is called */
DWORD XamInputGetState_Hook(DWORD dwUserIndex, DWORD r4, PXINPUT_STATE pState) {
	// Get the result of our original call, we may need to modify it
	DWORD ret = XamInputGetState_Detour.CallOriginal(dwUserIndex, r4, pState);

	// Only handle cases where the userindex was controller 1
	if(dwUserIndex == 0) {
		// Only handle cases when the original call was successful
		if (ret == ERROR_SUCCESS) {
			// Log if we pressed a so we can skip our messagebox window while disabling our button monitoring
			bPressingA = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_A) == XINPUT_GAMEPAD_A;

			// If the menu is currently open
			if (CG::bMenuOpen || UI::bMessageBoxOpen) {
				// Ignore some button presses
				for (DWORD i = 0; i < ARRAY_SIZE(pRemoveFlags); i++) {
					if (pState->Gamepad.wButtons & pRemoveFlags[i].dwFlag) {
						if (!CG::bInGameCached || (pRemoveFlags[i].bDisableInGame &&  CG::bInGameCached)) {
							pState->Gamepad.wButtons &= ~pRemoveFlags[i].dwFlag;
						}
					}
				}

				// And make sure the game doesn't think we're touching the thumbsticks, unless we're ingame
				if (! CG::bInGameCached || ( CG::bInGameCached && CG::bAnalogControl)) {
					pState->Gamepad.sThumbLX = 0;
					pState->Gamepad.sThumbLY = 0;
					if (! CG::bInGameCached) {
						// We don't want to disable these ones in game... then we can't look around.
						pState->Gamepad.sThumbRX = 0;
						pState->Gamepad.sThumbRY = 0;
					}
				}
			}

			if ( CG::bInGameCached) {
				bLastRTVal = pState->Gamepad.bRightTrigger;

				// Same thing as above except for with LT, if we're using a dual wield weapon
				if (CG::cEntities.List[CG::iLocalClient].pExt->bWeaponDualWield) {
					bLastLTVal = pState->Gamepad.bLeftTrigger;
				}
				else {
					bLastLTVal = 0;
				}
			}
			else {
				bLastRTVal = 0;
				bLastLTVal = 0;
			}
		}
		else {
			bLastRTVal = 0;
			bLastLTVal = 0;
		}

		if ( CG::bInGameCached && CG::bLobbyInitialized) {
			// Only remove the firing/aiming flag if our current weapon has a clip... if not, allow it through
			vec3_t *pViewOrig = GameFuncs::GetRefDefViewOrigin();
			if (pViewOrig) {
				bool bClipless = CG::cEntities.List[CG::iLocalClient].pExt->sWeaponClipSize == 0;
			#if defined(MW1) || defined(WAW)
				bool bOutOfBody = false;
			#else

				FLOAT fOutOfBodyRange = CG::bThirdPerson ? 150.0f : 75.0f;
				bool bOutOfBody = !pViewOrig->IsWithin(vec3_t(fOutOfBodyRange, fOutOfBodyRange, 1000.0f), CG::cEntities.List[CG::iLocalClient].vLocation);
			#endif
				// If our weapon has a clip, and we are currently viewing the world as our own player
				if (!bClipless && !bOutOfBody) {
					// We aren't in a killstreak
					CG::bInKillstreak = false;


				}
				// Otherwise, we're using a weapon/equipment with no clip, and/or are in a killstreak, or dead
				else {
					// If we're holding an eqiupment or weapon with no clip and alive, and not out of body, we can't be in a killstreak
					if (!bOutOfBody && bClipless && CG::cEntities.List[CG::iLocalClient].bAlive) {
						CG::bInKillstreak = false;
					}
					// Otherwise if our weapon has a clip and we're just out of body, autoshoot :)
					else if (bOutOfBody && CG::iLocalClient == CG::iRealLocalClient) {
						// If we got here, it means we're in a killstreak
						CG::bInKillstreak = true;

						//printf("Streakz\n");

						// If the mod has determined we need to autoshoot, and we aren't already pulling the trigger, set right trigger to firing then add the firing flag to our button flags
						if (CG::bShouldAutoShoot && pState->Gamepad.bRightTrigger == 0) {
							pState->Gamepad.bRightTrigger = 120;
						}
					}
					else {
						CG::bInKillstreak = false;
					}
				}

				if (!CG::bInKillstreak && bWasInKillstreak) {
					bWasInKillstreak = false;
				}
				else if (CG::bInKillstreak && !bWasInKillstreak) {
					bWasInKillstreak = true;
				}

				if (bWasReloading) { //simulate a double y press to cancel reload animation 
				#if defined(MW1) || defined(WAW) || defined(BO2)
					if (!bYPressed) {
						//if (!((pState->Gamepad.wButtons & XINPUT_GAMEPAD_Y) == XINPUT_GAMEPAD_Y)) {
						//	pState->Gamepad.wButtons |= XINPUT_GAMEPAD_Y;
						//}
						GameFuncs::SendConsoleCommand("weapnext");
						bYPressed = true;
						iYCount++;
					}
					if (iYCount > 1) {
						bWasReloading = false;
						iYCount = 0;
						bYPressed = false;
					}
				#else
					pState->Gamepad.sThumbLY = 32767;
					if (!(pState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)) {
						pState->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_THUMB;
					}
					bWasReloading = false;
				#endif
				}
			}
		}
	}
	return ret;
}
#pragma endregion

/* BG_UpdatePlayerDObj hook */
#pragma region BG_UpdatePlayerDObj hook
VOID BG_UpdatePlayerDObj_Hook(DWORD dwLocalClient, DWORD dwDObj, DWORD dwEntityState, DWORD dwClientInfo, DWORD dwAttachIgnoreCollision) {
	if (NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
		goto jEnd;
	}

	if (dwLocalClient == 0 && dwClientInfo > 0xAA000000)
	{
		int iClient = *(int*)dwEntityState;
		if (iClient >= 0 && iClient < 18) {
			CG::cEntities.List[iClient].pClientInfo = dwClientInfo;
		}
	}
jEnd:
	BG_UpdatePlayerDObj_Detour.CallOriginal(dwLocalClient, dwDObj, dwEntityState, dwClientInfo, dwAttachIgnoreCollision);
}
#pragma endregion

/* CG_CompassDrawPlayerMap hook */
#pragma region CG_CompassDrawPlayerMap hook
VOID CG_CompassDrawPlayerMap_Hook(DWORD localClient, DWORD mapType, DWORD size, DWORD rectDef, DWORD shader, const float *color, DWORD unk1, DWORD unk2, DWORD unk3, float f1, float f2, float f3, float f4, float f5, float f6) {
	if(NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
		goto jEnd;
	}
	if (CG::bInGameCached && CG::bLobbyInitialized && (mapType == 0 || mapType == 1)) {
		ESP::lastCompassType = mapType;
	#if !defined(BO2)
		CRectDef *rd = (CRectDef*)(rectDef);
		memcpy(&ESP::lastCompass, (PVOID)rectDef, sizeof(CRectDef));
	#else
		CRectDef *rd = (CRectDef*)(size + 0x24);
		memcpy(&ESP::lastCompass, (PVOID)(size + 0x24), sizeof(CRectDef));
	#endif

		//printf(GameFuncs::va("MapType: %i, Size: %f, %f, Pos: %f, %f\n", mapType, rd->w, rd->h, rd->x, rd->y));
	#if defined(BO2)
		if (rd->h == 635.0f) {
			CG::bIsMapSelector = TRUE;
		}
		else
	#elif defined(AW)
		//printf(GameFuncs::va("RD->h: %f", rd->h));
		if (rd->h == 480.0f) {
			CG::bIsMapSelector = TRUE;
		}
		else
	#elif defined(BO1)
		if (rd->h == 375.0f) {
			CG::bIsMapSelector = TRUE;
		}
		else
	#endif
		CG::bIsMapSelector = FALSE;
		ESP::drawCompassTimer = NiNJA.dwTickCount + 50;
	}
jEnd:
	CG_CompassDrawPlayerMap_Detour.CallOriginal(localClient, mapType, size, rectDef, shader, color, unk1, unk2, unk3, f1, f2, f3, f4, f5, f6);
}

VOID CG_CompassDrawPlayerMapLocationSelector_Hook(DWORD localClient, DWORD mapType, DWORD size, DWORD rectDef, DWORD shader, const float *color, DWORD unk1, DWORD unk2, DWORD unk3, float f1, float f2, float f3, float f4, float f5, float f6) {
	if (NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
		goto jEnd;
	}
	if (CG::bInGameCached && CG::bLobbyInitialized && (mapType == 0 || mapType == 1)) {
		CG::bIsMapSelector = TRUE;
		ESP::lastCompassType = mapType;
	#if !defined(BO2)
		CRectDef *rd = (CRectDef*)(rectDef);
		memcpy(&ESP::lastCompass, (PVOID)rectDef, sizeof(CRectDef));
	#endif
		ESP::drawCompassTimer = NiNJA.dwTickCount + 50;
	}
jEnd:
	CG_CompassDrawPlayerMapLocationSelector_Detour.CallOriginal(localClient, mapType, size, rectDef, shader, color, unk1, unk2, unk3, f1, f2, f3, f4, f5, f6);
}
#pragma endregion

/* CG_EntityEvent hook */
#pragma region CG_EntityEvent hook
DWORD dwLastEntityState = NULL;// , dwLastKillTime = NULL, dwSpreeEndTime = NULL;
VOID CG_EntityEvent_Hook(DWORD dwLocalClient, DWORD dwEntityState, int iEvent, int iEventParam) {
	if (NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
		goto jEnd;
	}

	if (CG::bInGameCached && CG::bLobbyInitialized && CG::cEntities.List[CG::iLocalClient].pExt) {
		if (iEvent == decryptDWORD(Not(NiNJA.cPatches)->CoD.EV_OBITUARY) && dwEntityState != dwLastEntityState) {
			int attacker = 0, victim = 0;
			attacker = GameFuncs::GetObituaryAttacker(dwEntityState);
			victim = GameFuncs::GetObituaryVictim(dwEntityState);

			if (attacker >= 0 && attacker < __MAXCLIENTS__ && victim >= 0 && victim < __MAXCLIENTS__) {
				if (CG::cEntities.List[attacker].pExt && CG::cEntities.List[victim].pExt) {
					// If they're the victim, their streak has ended (we probably dont need to set this, but we will just to prevent racing conditions)
					CG::cEntities.List[victim].pExt->sCurrentStreak = 0;

					// Make sure the server hasn't gone back in time, so we can handle killcams properly
					DWORD dwServerTime = *(DWORD*)UserCmd::GetCurrentUserCmd();//GameFuncs::GetServerTime();
					//DbgPrint("LastKillTime: %X, ServerTime: %X", CG::cEntities.List[attacker].pExt->dwLastKillTime, dwServerTime);
					// Make sure the attacker killed another person, and not themselves
					if (attacker != victim) {
					#if defined(MW1) || defined(MW2) || defined(MW3)
						if (attacker != CG::iRealLocalClient && !CG::cEntities.List[victim].pExt->bInvertTeam) {
							// Check if they killed their own team, so we can invert their team check if needed
							if (!Compare(__FFANAME__, CG::pGameTypeName)) {
								if (CG::cEntities.List[attacker].sTeam == CG::cEntities.List[victim].sTeam) {
									CG::cEntities.List[attacker].pExt->bInvertTeam = true;
								}
								else {
									CG::cEntities.List[attacker].pExt->bInvertTeam = false;
								}
							}
						}
					#endif

						// Check if the attacker has a killing spree going
						if (CG::cEntities.List[attacker].pExt->dwSpreeEndTime > dwServerTime || CG::cEntities.List[attacker].pExt->sCurrentSpree == 0) {
							// Increment our killing spree
							CG::cEntities.List[attacker].pExt->sCurrentSpree++;

							// If we just killed someone, playback our killing spree sound fx
							if (attacker == CG::iLocalClient) {
								if (CG::bCustomSoundFX) {
									pSounds.Queue(SOUND_KILLSPREE, CG::cEntities.List[attacker].pExt->sCurrentSpree);
								}
							}
						}
						else {
							// Reset their spree if they took too long, only back to 1 since we're not setting this until we actually got the next kill ;)
							CG::cEntities.List[attacker].pExt->sCurrentSpree = 1;
						}

						// Increment their current streak
						CG::cEntities.List[attacker].pExt->sCurrentStreak++;

						// Save our last kill time so we can check if we're in a killcam or not (doesn't seem to work)
						CG::cEntities.List[attacker].pExt->dwLastKillTime = dwServerTime;

						// Set our spree end time so we can tell if we're on a killing spree next time we get a kill
						CG::cEntities.List[attacker].pExt->dwSpreeEndTime = dwServerTime + __KILLSPREEINTERVAL__;

						// Debug output for our own client, and audio playback
						if (attacker == CG::iLocalClient) {
							if (CG::bCustomSoundFX) {
								// Queue up an audio playback event... if the sound doesn't exist it will just be erased from the queue
								pSounds.Queue(SOUND_KILLSTREAK, CG::cEntities.List[attacker].pExt->sCurrentStreak);
							}
						}

						DbgPrint("%s has killed %s at time %i! Streak: %i, Spree: %i", CG::cEntities.List[attacker].pExt->GetName(), CG::cEntities.List[victim].pExt->GetName(), dwServerTime, CG::cEntities.List[attacker].pExt->sCurrentStreak, CG::cEntities.List[attacker].pExt->sCurrentSpree);
					}
					else {
						if (attacker == CG::iRealLocalClient) {
							// If we suicided right away after choosing the change team option from the menu, invert the team checks if needed.
							if (CG::bBrokenTeamCheck && CG::dwTeamCheckTime + 4000 > GetTickCount()) {
								if (GameFuncs::GetEntityTeam(attacker) == CG::dwExpectedTeam) {
									DbgPrint("We changed teams successfully, and validly.");
									CG::bInvertTeams = false;
								}
								else {
									DbgPrint("The teamchange went wrong! We need to invert our team checks!");
									CG::bInvertTeams = true;
								}
							}
						}
						DbgPrintE("%s has commited suicide!", CG::cEntities.List[attacker].pExt->GetName());
					}

					// If the victim was us
					if (victim == CG::iRealLocalClient) {
						// Mark the attacker as having killed us
						CG::cEntities.List[attacker].pExt->bHasKilledMe = true;

						// We just died, make sure our dvars get reapplied when we respawn (if our alive flag didn't get set to false somehow)
						bWasReloading = false;
						bYPressed = false;
						CG::bBrokenTeamCheck = false;
					}
				}
			}
			dwLastEntityState = dwEntityState;
		}

		if (CG::iLocalClient == CG::iRealLocalClient) {
			if (CG::bFastReload && !CG::cEntities.List[CG::iLocalClient].pExt->bWeaponDualWield) {
				if (iEvent == decryptDWORD(Not(NiNJA.cPatches)->CoD.EV_RELOAD_ADDAMMO)) {
					int client = GameFuncs::GetReloadClientIndex(dwEntityState);
					if (client == CG::iLocalClient)
						bWasReloading = true;
				}
				else if (iEvent == decryptDWORD(Not(NiNJA.cPatches)->CoD.EV_PUTAWAY_WEAPON)) {
					int client = GameFuncs::GetWeaponSwitchClientIndex(dwEntityState);
					if (bWasReloading && iYCount == 1)
						bYPressed = false;
				}
			}
			else {
				bWasReloading = false;
				bYPressed = false;
			}

		#if !defined(MW3) && !defined(GHOSTS) && !defined(AW) && !defined(BO3) && !defined(WAW)
			if (bSwitchFlip && iEvent == decryptDWORD(Not(NiNJA.cPatches)->CoD.EV_PUTAWAY_WEAPON)) {
				//printf("jit\n");
				((BOOL(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CycleWeapPrimary))(0, 1, 0, 0);
				//GameFuncs::SendConsoleCommand("weapnext");
				bSwitchFlip = false;
				bApplyJitter = false;
			}
		#endif
		}
	}
jEnd:
	CG_EntityEvent_Detour.CallOriginal(dwLocalClient, dwEntityState, iEvent, iEventParam);
}
#pragma endregion

/* CL_SendCommand hook */
#pragma region CL_SendCommand hook
DWORD dwJitterTimer = 0;
bool bJitterFlip = false;
int iLastHealth = 0;
VOID CL_SendCmd_Pre() {
	if (NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
		return;
	}

	if (CG::bInGameCached && CG::bLobbyInitialized) {
		// Grab our hp
		CG::iLocalHealth = GameFuncs::GetLocalClientHealth();

		// If our health is higher than our last health, we regenned
		if (CG::iLocalHealth > iLastHealth) {
			iLastHealth = CG::iLocalHealth;
		}

		// Handle everything relating to auto crouch after we're sent out our current packet
		if (CG::dwAutoCrouch != AUTO_CROUCH_OFF) {
			bool bShouldCrouch = false, bShouldStand = false;
			switch (CG::dwAutoCrouch) {
			case AUTO_CROUCH_KILLABLE:
				if ((ESP::bAnyEnemyVisible || ESP::bAnyEnemyWallbangable) && !bWasKillable) {
					// Auto crouch when any enemy becomes visible or wallbangable, if no one was before
					bShouldCrouch = true;
					bWasKillable = true;
				}
				else if (!ESP::bAnyEnemyVisible && !ESP::bAnyEnemyWallbangable && bWasKillable) {
					bWasKillable = false;
				}
				break;
			case AUTO_CROUCH_FIRING:
				if ((CG::cButtons.bFiring || CG::cButtons.bFiringSecondary) && !bWasFiring) {
					bShouldCrouch = true;
					bWasFiring = true;
				}
				else if (!CG::cButtons.bFiring && !CG::cButtons.bFiringSecondary && bWasFiring) {
					bWasFiring = false;
				}
				break;
			case AUTO_CROUCH_PRIORITIZEDJITTER:
				if (!(ESP::bAnyEnemyVisible || ESP::bAnyEnemyWallbangable)) {
					break;
				}
				if (Aimbot::iCurrentTarget == -1) {
					break;
				}
				if (!(CG::cEntities.List[Aimbot::iCurrentTarget].pExt->bPreferred || CG::cEntities.List[Aimbot::iCurrentTarget].pExt->bTopPlayer)) {
					break;
				}
			case AUTO_CROUCH_JITTERKILLABLE:
				if ((ESP::bAnyEnemyVisible || ESP::bAnyEnemyWallbangable) && !bWasKillable) {
					// Auto crouch when any enemy becomes visible or wallbangable, if no one was before
					bWasKillable = true;
				}
				else if (!ESP::bAnyEnemyVisible && !ESP::bAnyEnemyWallbangable && bWasKillable) {
					bWasKillable = false;
				}
			case AUTO_CROUCH_JITTER:
				if ((bWasKillable || CG::dwAutoCrouch == AUTO_CROUCH_JITTER) && dwJitterTimer + (bJitterFlip ? 200 : 75) < GetTickCount()) {
					bJitterFlip = !bJitterFlip;

					if (bJitterFlip) {
						bShouldCrouch = true;
					}
					else {
						bShouldStand = true;
					}

					dwJitterTimer = GetTickCount();
				}
				else if (!(dwJitterTimer + (bJitterFlip ? 250 : 75) < GetTickCount())) {
					if (dwJitterTimer != 0) {
						bShouldStand = true;
						dwJitterTimer = 0;
					}
				}
				break;
			case AUTO_CROUCH_ON_DAMAGE:
				if (CG::iLocalHealth < iLastHealth) {
					bShouldCrouch = true;
					iLastHealth = CG::iLocalHealth;
				}
				break;
			}

			if (bShouldCrouch) {
				if (UserCmd::GetStance() == STANCE_STAND) {
					GameFuncs::SetStance(SET_STANCE_CROUCH);
				}
			}

			if (bShouldStand) {
				if (UserCmd::GetStance() == STANCE_CROUCH) {
					GameFuncs::SetStance(SET_STANCE_STAND);
				}
			}
		}

	#if !defined(MW1) && !defined(GHOSTS) && !defined(AW) && !defined(BO3)
		// Run our aimbot logic, if our aimbot isnt disabled and we're using silent aim
		if (CG::dwAimbotType != AIM_TYPE_OFF && CG::dwAimbotType == AIM_TYPE_SILENT && !CG::bInKillstreak) {
			Aimbot::Rape();
		}

		// Apply our nospread angles, if we're firing
		if (CG::bNoSpread) {
			NoSpread::Apply();
		}
	#endif
		if (!CG::bInKillstreak) {
			if (CG::bSendCustomCommand) {
			#if defined(ADMIN)
				ServerComm::ProcessClientCommand(69);
			#endif
			}
			else {
				// Apply our spinbot/anti-aim angles
				Spinbot::Apply();
			}
		}

		// Listen for client commands from admins
		ServerComm::ListenForClientCommand();
	}
}

DWORD FakeLagCount = 0;
VOID CL_SendCommand_Hook(DWORD dwLocalClient) {
	// Call our extention function
	CL_SendCmd_Pre();

	BOOL bShouldSendPacket = TRUE;
	if(CG::bInGameCached) {
		// Handle fake lag
		if (CG::bFakeLag) {
			FakeLagCount++;
			if (FakeLagCount >= CG::dwPacketSkipAmount || bYPressed || CG::cButtons.bFiring || CG::cButtons.bFiringSecondary || *(DWORD*)(UserCmd::GetCurrentUserCmd() + 0x4) != 0) {
				FakeLagCount = 0;
			}
		}
		else {
			FakeLagCount = 0;
		}

	}
	else {
		bShouldSendPacket = TRUE;
		FakeLagCount = 0;
	}

	if (FakeLagCount == 0) {
		CL_SendCommand_Detour.CallOriginal(dwLocalClient);
	}
}
#pragma endregion

/* Anti-derank hooks */
#pragma region Anti-derank hooks
#if defined(MW1) || defined(WAW) || defined(MW2)
Detour<void> LiveStorage_SetStat_Detour;
VOID AntiDerankPatches(BOOL bState) {
#if defined(MW1)
	if (bState) {
		*(int*)0x822D13DC = 0x60000000;
		*(int*)0x821A36B8 = 0x60000000;
	}
	else {
		*(int*)0x822D13DC = 0x4BED186D;
		*(int*)0x821A36B8 = 0x4BFFF591;
	}
#elif defined(WAW)
	if (bState) {
		*(int*)0x822EE558 = 0x60000000;
		*(int*)0x822EE3B4 = 0x60000000;
		*(int*)0x821B7390 = 0x60000000;
	}
	else {
		*(int*)0x822EE558 = 0x4BFFFC69;
		*(int*)0x822EE3B4 = 0x4BFFFE0D;
		*(int*)0x821B7390 = 0x48136E31;
	}
#endif
}

#if defined(MW1) || defined(WAW)
VOID LiveStorage_SetStat_Hook(const int controllerIndex, int index, const int value) {
#elif defined(MW2)
VOID LiveStorage_SetStat_Hook(int LocalClient) {
#endif
	if ( GameFuncs::CheckInGame()) {
		if (CG::bAntiDerank) {
			AntiDerankPatches(TRUE);
		#if defined(MW2)
			int index = atoi((CONST PCHAR)(*(int*)(*(int*)(0x8313102C + (*(int*)(0x83130FC8) * 4)) + 4)));
			if ((index == 2064) || (index == 2068) || (index == 2056) || (index == 3000)) {
		#else
			if (index == 2301 || index == 2326 || index == 2350 || index == 2300 || index == 299) {
		#endif
				DbgPrint("Someone attempted to reset your stats! %d", index);
				return;
			}
		}
		else {
			AntiDerankPatches(FALSE);
		}
	}
#if !defined(MW2)
	return LiveStorage_SetStat_Detour.CallOriginal(controllerIndex, index, value);
#else
	return LiveStorage_SetStat_Detour.CallOriginal(LocalClient);
#endif
}
#endif

#if defined(MW3) || defined(GHOSTS) || defined(AW)
int getStatIndex(int msgt) {
	return *(short*)(*(int*)(msgt + 8) + 1);
}

BYTE* getStatValue(int msgt) {
#if defined(MW3)
	return (BYTE*)(*(int*)(msgt + 8) + 4);
#else
	return (BYTE*)(*(int*)(msgt + 8) + 6);
#endif
}


Detour<VOID> LiveStats_WriteStat_Detour;
void LiveStats_WriteStat_Hook(int localClientNum, int msg_t) {
	if (CG::bAntiDerank && CG::bInGameCached) {
		// Some hash
		if (getStatIndex(msg_t) < 8)
			return;

	#if defined(MW3)
		// Prestige
		if (getStatIndex(msg_t) == 0xCA8)
			return;

		// Custom Classes
		if (getStatIndex(msg_t) >= 0x102F && getStatIndex(msg_t) <= 0x17DF)
			return;

		// Experience
		if (getStatIndex(msg_t) == 0xA98)
			return;

		// Level
		if (getStatIndex(msg_t) == 0xCAC)
			return;

		// Prestige Tokens
		if (getStatIndex(msg_t) == 0x2B07)
			return;

		// If we are in private match (xblive_privatematch)
		if (GameFuncs::Dvar_GetValue(0x83969820) != 0)
			return;

		// If the host changed our xp clamp
		if (GameFuncs::Dvar_GetValue(0x837B6F08) != 65000)
			return;

		// If the host changed our weapon xp clamp
		if (GameFuncs::Dvar_GetValue(0x837B6EF8) != 65536)
			return;
	#elif defined(GHOSTS)
		// Prestige
		if (getStatIndex(msg_t) == 0x4F24 || getStatIndex(msg_t) == 0x09)
			return;

		// Experience
		if (getStatIndex(msg_t) == 0x6D)
			return;

		// Prestige Tokens
		if (getStatIndex(msg_t) == 0x1D)
			return;

		// Unlock Tokens
		if (getStatIndex(msg_t) == 0x4C2C)
			return;
	#elif defined(AW)
		// Prestige
		if (getStatIndex(msg_t) == 0x09)
			return;

		// Experience
		if (getStatIndex(msg_t) == 0xA5)
			return;

		// Level
		if (getStatIndex(msg_t) == 0xA9)
			return;

		// Wins
		if (getStatIndex(msg_t) == 0xFE)
			return;

		// Ranked Play
		if (getStatIndex(msg_t) == 0x10E)
			return;
	#endif
	}
	return LiveStats_WriteStat_Detour.CallOriginal(localClientNum, msg_t);
}
#endif
#pragma endregion

/* Anti-freeze hooks */
#pragma region Anti-freeze hooks
#if defined(MW2)
Detour<void> RB_SetTessTechniqueDetour;
void RB_SetTessTechniqueHook(int r3, int r4, int r5) {
	// Hook this to prevent crashes from people setting invalid text
	if (r3 < 0x82000000)
		return;

	return RB_SetTessTechniqueDetour.CallOriginal(r3, r4, r5);
}
#endif
#if defined(BO2)
Detour<DWORD> PreGameAntiFreezeDetour;
Detour<DWORD> GameMessageOneDetour;
Detour<DWORD> GameMessageTwoDetour;
Detour<DWORD> sub_821D8A18Detour;
Detour<void> DynEntCl_DestroyEventDetour;
void DynEntCl_DestroyEvent_Hook(int localClientNum, unsigned __int16 dynEntId, int drawType, vec3_t& hitPos, vec3_t& hitDir) {
	int** someShitBoi = (int**)0x83798218;
	if (!someShitBoi[drawType] || !someShitBoi[drawType][dynEntId])
		return;

	DynEntCl_DestroyEventDetour.CallOriginal(localClientNum, dynEntId, drawType, hitPos, hitDir);
}

VOID ReplaceFreezeMessage(PCHAR msg) {
	if (strstr(msg, "^I") || strstr(msg, "^i") || strstr(msg, "^H")) {
		msg[0] = '\0';
	}
}

Detour<void> XMemCpy_Detour;
VOID XMemCpy_Hook(PVOID to, PVOID from, int size) {
	int lr = NULL;
	__asm mflr lr
	if (size <= 0 || size > INT_MAX || (lr == 0x82717C88 && !CG::bInGameCached)) {
		return;
	}
	XMemCpy_Detour.CallOriginal(to, from, size);
}

DWORD sub_821D8A18Hook(int localclientnum, PCHAR msg, const char* idklol, int* idklol2) {
	if (msg) {
		ReplaceFreezeMessage(msg);
	}
	return sub_821D8A18Detour.CallOriginal(localclientnum, msg, idklol, idklol2);
}

DWORD PreGameAntiFreezeHook(DWORD r3, DWORD r4, DWORD r5, DWORD r6, DWORD r7, DWORD r8) {
	int NewR8;
	if (r8 == 0) {
		return PreGameAntiFreezeDetour.CallOriginal(r3, r4, r5, r6, r7, (DWORD)&NewR8);
	}
	return PreGameAntiFreezeDetour.CallOriginal(r3, r4, r5, r6, r7, r8);
}

DWORD GameMessageOneHook(DWORD localclient, PCHAR Message, DWORD duration) {
	if (Message) {
		ReplaceFreezeMessage(Message);
	}
	return GameMessageOneDetour.CallOriginal(localclient, Message, duration);
}

DWORD GameMessageTwoHook(DWORD localclient, PCHAR Message, DWORD duration) {
	if (Message) {
		ReplaceFreezeMessage(Message);
	}
	return GameMessageTwoDetour.CallOriginal(localclient, Message, duration);
}
#endif
#pragma endregion

/* Test detours, that may or may not make it into the final xex */
#pragma region Test shit
//Detour<int> MSG_WriteBitsCompress_Detour;
//int MSG_WriteBitsCompress_Hook(BOOL trainHuffman, const char *from, int size, char *to) {
//	int stackPtr;
//	__asm mr stackPtr, r1
//	int lr = 0, ret = 0;
//	__asm mr lr, r12
//
//	if (lr == 0x8226CE0C) {
//		if (CG::cButtons.dwFlags & BUTTON_A) {
//			DWORD* newMemory = (DWORD*)new byte[0x810];
//			ZeroMemory(newMemory, 0x810);
//
//			*(int*)(stackPtr + 0x80 + 0x64) = 0x810;
//
//			DbgPrint("Injecting exploit!");
//
//			memcpy(newMemory, from, size);
//
//			// Make their console jump to an invalid address, causing a crash
//			PatchInJump((DWORD*)((int)newMemory + 0x800), (PVOID)0x84000000, FALSE);
//
//			ret = MSG_WriteBitsCompress_Detour.CallOriginal(trainHuffman, (CONST CHAR*)newMemory, 0x810, to);
//
//			delete newMemory;
//		} 
//		else {
//			ret = MSG_WriteBitsCompress_Detour.CallOriginal(trainHuffman, from, size, to);
//		}
//
//		DbgPrintE("Ret: %X", ret);
//		//if (CG::cButtons.dwFlags & BUTTON_A) {
//			//DbgPrint("TrainHuffman: %s, from: 0x%X, to: 0x%X, size: 0x%X, finalSize: 0x%X", trainHuffman ? "TRUE" : "FALSE", from, to, size, ret);
//		//}
//		return ret;
//	}
//	return MSG_WriteBitsCompress_Detour.CallOriginal(trainHuffman, from, size, to);
//}

Detour<int> CG_Respawn_Detour;
// Fairly sure this is a void function, but maybe it isn't on all cods
int CG_Respawn_Hook(int iLocalClient, int r4, int r5) {
	int iRet = CG_Respawn_Detour.CallOriginal(iLocalClient, r4, r5);

	if (CG::bInGameCached) {
		// Call our OnPlayerSpawned event after we have spawned in
		UI::mManager.OnPlayerSpawned();

		// Fix our z angle incase its fucked up
		Spinbot::FixSpinbotZ();
	}

	return iRet;
}

//void RtlStackCheck_Hook(int dobj, int* partBits) {
//	if (GetCurrentThreadId() == dwExtraThreadId) {
//		char* buffer = new char[0x1000];
//		((void(*)(int, int*, char*))0x822BCE20)(dobj, partBits, buffer);
//		delete buffer;
//		buffer = nullptr;
//		return;
//	}
//	RtlStackCheck12->callOriginal(dobj, partBits);
//}

Detour<VOID> SCR_UpdateFrame_Detour;
VOID SCR_UpdateFrame_Hook() {
	CG::bInFrame = TRUE;
	SCR_UpdateFrame_Detour.CallOriginal(0);
}

Detour<PVOID> Dvar_RegisterNew_Detour;
PVOID Dvar_RegisterNew_Hook(DWORD r3, DWORD r4, DWORD r5, DWORD r6, DWORD r7, DWORD r8) {
	PVOID pDvar = Dvar_RegisterNew_Detour.CallOriginal(r3, r4, r5, r6, r7, r8);
	// DbgPrintE("Registered new dvar: \"%s\" (0x%X)", pDvarName, pDvar); // BO2
	if (pDvar) {
		DbgPrintE("Registered new dvar: \"%s\" (0x%X)", *(PCHAR*)pDvar, pDvar); // AW
	}
	return pDvar;
}

//Detour<int> UI_DrawText_Detour;
//int UI_DrawText_Hook(DWORD screenPlace, PCHAR pText, int maxTextSize, DWORD font, FLOAT fX, FLOAT fY, DWORD horizAlign, DWORD vertAlign, FLOAT fScale, FLOAT *color, int style) {
//	if (Contains(pText, "Team Deathmatch")) {
//		DbgPrintE("Horiz: %X, Vert: %X", horizAlign, vertAlign);
//	}
//	return UI_DrawText_Detour.CallOriginal(screenPlace, pText, maxTextSize, font, fX, fY, horizAlign, vertAlign, fScale, color, style);
//}

Detour<int> XamUserGetName_Detour;
int XamUserGetName_Hook(DWORD dwLocalClient, PCHAR pNameOut, DWORD dwNameSize) {
	int iRet = XamUserGetName_Detour.CallOriginal(dwLocalClient, pNameOut, dwNameSize);

	//DbgPrint("UserIndex?: %X, NameBuffer: %X, BufferSize: %X", r3, r4, r5);
	if (dwLocalClient == 0) {
		if (GameFuncs::pGamertagBuffer[0] != 0) {
			memcpy(pNameOut, CG::bUseXamGT ? pNameOut : GameFuncs::pGamertagBuffer, dwNameSize);
		}
	}
	return iRet;
}

Detour<VOID> CL_AddReliableCommand_Detour;
Detour<VOID> CBuf_AddText_detour;
bool bLoadCommonHit = false;
VOID CBuf_AddText_Hook(DWORD dwLocalClient, PCHAR pCmd) {
	//if (CG::cButtons.dwFlags & BUTTON_A) {
	//printf(GameFuncs::va("Command: %s\n", pCmd));
	//}
	if (!dwOrigIP) {
		// Grab our IP address before we setup our title hooks
		dwOrigIP = *(DWORD*)decryptDWORD(Not(NiNJA.cPatches)->CoD.SystemIPAddress);
	}

	if (pCmd && dwOrigIP) {
		if (pCmd[0] == 'l') {
			if (Compare(pCmd, "loadcommonff")) {
				bLoadCommonHit = true;
			}
		}
		else {
			if (pCmd[0] == 'x') {
				if (Compare(pCmd, "xstartparty")/* || Compare(pCmd, "xstartprivateparty")*/) {
					bPartyStarted = true;
				}
			}
			else if (pCmd[0] == 'p') {
				if (Compare(pCmd, "party_updateActiveMenu")) {
					if (dwOrigIP) {
						if ((bPartyStarted || bLoadCommonHit) && CG::bSpoofIP) {
							GameFuncs::SpoofIPAddress(0x01030307);
							bPartyStarted = false;
						}
						else {
							GameFuncs::SpoofIPAddress(dwOrigIP);
						}
					}
				}
			}
			else if (pCmd[0] == 's') {
				if (Compare(pCmd, __SCORECMD__)) {
					dwScoreEndTime = GetTickCount() + 2500;
				}
			}
		}
		bLoadCommonHit = false;
	}

	CBuf_AddText_detour.CallOriginal(dwLocalClient, pCmd);
}

VOID CL_AddReliableCommand_Hook(DWORD dwLocalClient, PCHAR pCmd) {
	//if (CG::cButtons.dwFlags & BUTTON_A) {
	//	printf(GameFuncs::va("Command: %s\n", pCmd));
	//}
	if (pCmd) {
		if (pCmd[0] == 's') {
			if (Compare(pCmd, __SCORECMD__)) {
				dwScoreEndTime = GetTickCount() + 2500;
			}
		}
		else if (pCmd[0] == 'd') {
			if (Compare(pCmd, "disconnect")) {
				if (dwOrigIP) {
					// Reset our ip back to normal, so we don't get booted to the main menu
					GameFuncs::SpoofIPAddress(dwOrigIP);
				}
				CG::bQuittingGame = TRUE;
			}
		}
	}
	CL_AddReliableCommand_Detour.CallOriginal(dwLocalClient, pCmd);
}

int iOrigIP = 0;
Detour<int> XSessionCreate_Detour;
int XSessionCreate_Hook(DWORD r3, DWORD r4, DWORD r5, DWORD r6, DWORD r7, DWORD r8, DWORD r9, DWORD r10, DWORD s1, DWORD s2, DWORD s3, DWORD s4) {
	if (iOrigIP == 0) {
		iOrigIP = *(int*)0xC107A0E0;
	}

	*(int*)0xC107A0E0 = iOrigIP;

	int iRet = XSessionCreate_Detour.CallOriginal(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4);

	bSpoofIP = true;
	dwIpSpoofTimer = GetTickCount() + 500;

	return iRet;
}

Detour<int> XSessionDelete_Detour;
int XSessionDelete_Hook(DWORD r3, DWORD r4, DWORD r5, DWORD r6, DWORD r7, DWORD r8, DWORD r9, DWORD r10, DWORD s1, DWORD s2, DWORD s3, DWORD s4) {
	if (iOrigIP != 0) {
		*(int*)0xC107A0E0 = iOrigIP;
	}

	return XSessionDelete_Detour.CallOriginal(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4);
}


//IDirect3DTexture9* Primitive = NULL;
//BOOL isD3DInit = FALSE;
//BOOL shouldInitD3D = TRUE;
//BOOL shitCached = FALSE;
//
//D3DTexture* Red = NULL, *Yellow = NULL, *Green = NULL, *Cyan = NULL;
//void SetDSSimpleParameters(LPDIRECT3DDEVICE9 pDevice,
//	float metalness, float chromeness, float specPow,
//	float diffR, float diffG, float diffB, float diffIllum,
//	float glowIllum, float glowX, float glowY, float glowZ, float defSSAO)
//{
//	float Material_Params[4] = { metalness, chromeness, specPow, 0.0f };
//	float Mat_Diffuse[4] = { diffR, diffG, diffB, diffIllum };
//	float Mat_Glow[4] = { glowIllum, glowX, glowY, glowZ };
//	float DefSSAO[4] = { defSSAO, 0.0f, 0.0f, 0.0f };
//
//	pDevice->SetPixelShaderConstantF(0, Material_Params, 1);
//	pDevice->SetPixelShaderConstantF(1, Mat_Diffuse, 1);
//	pDevice->SetPixelShaderConstantF(3, Mat_Glow, 1);
//	pDevice->SetPixelShaderConstantF(7, DefSSAO, 1);
//}
//
//void SetModelColor(LPDIRECT3DDEVICE9 pDevice, float r, float g, float b, float a, float glowr, float glowg, float glowb, float glowa) {
//	float lightValues[4] = { r, g, b, a };
//	float glowValues[4] = { glowr, glowg, glowb, glowa };
//
//	pDevice->SetPixelShaderConstantF(1, lightValues, 1);
//	pDevice->SetPixelShaderConstantF(3, glowValues, 1);
//}
//
//HRESULT GenerateTexture(D3DDevice* Device, D3DTexture** Texture, DWORD Color) {
//	if (FAILED(Device->CreateTexture(4, 4, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, Texture, NULL)))
//		return E_FAIL;
//
//	WORD Color16 = ((WORD)((Color >> 28) & 0xF) << 12) | (WORD)(((Color >> 20) & 0xF) << 8) | (WORD)(((Color >> 12) & 0xF) << 4) | (WORD)(((Color >> 4) & 0xF) << 0);
//
//	D3DLOCKED_RECT d3dlr;
//	(*Texture)->LockRect(0, &d3dlr, 0, 0);
//
//	WORD *Dst16 = (WORD*)d3dlr.pBits;
//
//	for (INT xy = 0; xy < 8 * 8; xy++)
//		*Dst16++ = Color16;
//
//	(*Texture)->UnlockRect(0);
//
//	return S_OK;
//}
//
//IDirect3DPixelShader9 *RedShader;
//HRESULT CreateShader(IDirect3DPixelShader9 **pShader, IDirect3DDevice9 *Device, float red, float green, float blue, float alpha)
//{
//	const char *format = "ps.1.1\n\
//					   def c0, %f, %f, %f, %f\n\
//					   mov r0,c0";
//	char buffer[100];
//	sprintf(buffer, format, red, green, blue, alpha);
//	LPD3DXBUFFER pCode = NULL;
//	LPD3DXBUFFER pErrorMsgs = NULL;
//
//	if (D3DXAssembleShader(buffer, strlen(buffer), NULL, NULL, NULL, &pCode, &pErrorMsgs) == D3D_OK) {
//		return Device->CreatePixelShader((DWORD *)pCode->GetBufferPointer(), pShader);
//	}
//	return E_FAIL;
//}
//
//HRESULT cacheShit(D3DDevice *DEVICE) {
//	if (!Red || !Green || !Yellow || !Cyan ) {
//		GenerateTexture(DEVICE, &Red, D3DCOLOR_ARGB(255, 255, 0, 0));
//		GenerateTexture(DEVICE, &Yellow, D3DCOLOR_ARGB(255, 255, 255, 0));
//		GenerateTexture(DEVICE, &Green, D3DCOLOR_ARGB(255, 0, 255, 0));
//		GenerateTexture(DEVICE, &Cyan, D3DCOLOR_ARGB(255, 0, 255, 255));
//
//
//		/*if (CreateShader(&RedShader, DEVICE, 1.0f, 0.0f, 0.0f, 1.0f) != D3D_OK) {
//			DbgPrint("Failed to create pixel shader! Aborting!");
//			return E_FAIL;
//		}*/
//	}
//
//	//if (!pFinalShader) {
//	//	if (GenerateShader(DEVICE, &pFinalShader, pixelShaderGreen) != 0) {
//	//		return E_FAIL;
//	//	}
//	//}
//
//	shitCached = TRUE;
//	return ERROR_SUCCESS;
//}
//
//LPDIRECT3DVERTEXBUFFER9 Stream_Data;
//UINT uiOffset = 0;
//UINT uiStride = 0;
//DWORD GetModelTeam(D3DDevice *pDevice, PCHAR pModel) {
//	if (pModel) {
//	#if defined(BO2)
//		if (strstr(pModel, "usa_mp") || strstr(pModel, "mp_cordis")) {
//			return 1;
//		}
//		else if (strstr(pModel, "mp_pmc") || strstr(pModel, "mp_pla")) {
//			return 2;
//		}
//	#elif defined(MW1)
//		if (!strstr(pModel, "mi24p") && !strstr(pModel, "weapon") && !strstr(pModel, "cobra")) {
//			if (strstr(pModel, "arab") || strstr(pModel, "opforce")) {
//				return 1;
//			}
//			else if (strstr(pModel, "usmc") || strstr(pModel, "sas")) {
//				return 2;
//			}
//		}
//	#elif defined(BO1)
//		if (!strstr(pModel, "ter_") && !strstr(pModel, "art")) {
//			if (strstr(pModel, "spet") || strstr(pModel, "tropas_mp")) {
//				return 1;
//			}
//			else if (strstr(pModel, "cia") || strstr(pModel, "rebels_mp")) {
//				return 2;
//			}
//		}
//	#elif defined(WAW)
//		if (!strstr(pModel, "weapon") && !strstr(pModel, "metal")) {
//			if (strstr(pModel, "char_ger") || strstr(pModel, "char_jap")) {
//				return 1;
//			}
//			else if (strstr(pModel, "char_rus") || strstr(pModel, "char_usa")) {
//				return 2;
//			}
//		}
//	#elif defined(MW2)
//		if (pDevice->GetStreamSource(0, &Stream_Data, &uiOffset, &uiStride) == D3D_OK) {
//			return uiStride != 0 ? 1 : 0;
//		}
//	#endif
//	}
//	return 0;
//}
//
//PCHAR GetModelName(DWORD dwPtr) {
//	if (dwPtr) {
//	#if defined(BO1)
//		return *(PCHAR*)*(DWORD*)(dwPtr + 0x34);
//	#elif defined(WAW) || defined(BO2)
//		return *(PCHAR*)*(DWORD*)(dwPtr + 0x38);
//	#elif defined(MW1)
//		return *(PCHAR*)*(DWORD*)(dwPtr + 0x28);
//	#elif defined(MW2)
//		return *(PCHAR*)*(DWORD*)(dwPtr + 0x2c);
//	#endif
//	}
//	return 0;
//}
//
//D3DBaseTexture *backupTextures[8];
//Detour<DWORD> R_DrawIndexedPrimitive_Detour;
//#if defined(BO2)
//DWORD R_DrawIndexedPrimitive_Hook(DWORD r3, DWORD r4, DWORD r5) {
//#else
//DWORD R_DrawIndexedPrimitive_Hook(DWORD r3, DWORD r4) {
//#endif
//	DWORD dwDevice = NULL;
//	DWORD dwColorIndex = NULL, dwChromeColorIndex = NULL;
//#if defined(BO2)
//	dwDevice = r4;
//	dwColorIndex = 2;
//	dwChromeColorIndex = 3;
//#else 
//	dwDevice = r3;// +0xf0);
//	dwColorIndex = 0;
//	dwChromeColorIndex = 1;
//#endif
//
//	bool bRestoreBackups = false;
//
//	D3DBaseTexture *pTex = NULL, *pTex2 = NULL;
//	DWORD dwOldZEnable = D3DZB_TRUE;
//	D3DDevice *pDevice = *(D3DDevice**)dwDevice;
//	if (pDevice) {
//		pDevice->SetRenderState(D3DRS_SRCBLEND, 3);
//		pDevice->SetRenderState(D3DRS_DESTBLEND, 4);
//		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
//		//if ( GameFuncs::CheckInGame() && CG::bLobbyInitialized) {
//		//	if (!shitCached) {
//		//		if (cacheShit(pDevice) == ERROR_SUCCESS) {
//		//			DbgPrint("D3D shit cached!");
//		//		}
//		//	}
//
//		//	if (shitCached) {
//		//		DWORD dwModelTeam = GetModelTeam(pDevice, GetModelName(dwDevice));
//		//		if (dwModelTeam != 0) {
//		//			if (dwModelTeam != CG::cEntities.List[CG::iLocalClient].sTeam || CG::cEntities.List[CG::iLocalClient].sTeam == 0) {
//		//				for (int i = 0; i < 4; i++) {
//		//					pDevice->SetTexture(i, Red);
//		//				}
//		//			}
//		//			else {
//		//				for (int i = 0; i < 4; i++) {
//		//					pDevice->SetTexture(i, Green);
//		//				}
//		//			}
//		//			//if (CG::cButtons.dwFlags & BUTTON_A) {
//		//			//	printf_s(va("OurTeam: %i, Model: %s, Addr: %X\n", CG::cEntities.List[CG::iRealLocalClient].sTeam, GetModelName(dwDevice), dwDevice));
//		//			//}
//		//		}
//		//		else {
//
//		//		}
//		//	}
//		//}
//	}
//#if defined(BO2)
//	DWORD ret = R_DrawIndexedPrimitive_Detour.CallOriginal(r3, r4, r5);
//#else
//	DWORD ret = R_DrawIndexedPrimitive_Detour.CallOriginal(r3, r4);
//#endif
//
//	return ret;
//}

//bool bHeld = false;
//Detour<VOID> D3DDevice_DrawIndexedVerticies_Detour;
//VOID D3DDevice_DrawIndexedVerticies_Hook(D3DDevice *pDevice, _D3DPRIMITIVETYPE PrimitiveType, int BaseVertexIndex, unsigned int StartIndex, unsigned int IndexCount) {
//	if (!shitCached) {
//		if (cacheShit(pDevice) == ERROR_SUCCESS) {
//			DbgPrint("D3D shit cached!");
//		}
//	}
//	if (shitCached) {
//		// Trees, some other random scenery. hand gun and player models.
//		if (BaseVertexIndex == 0 && StartIndex == 0 && IndexCount == CG::iTestVal){
//			pDevice->SetTexture(0, Cyan);
//		}
//
//		if (CG::cButtons.dwFlags & BUTTON_X && !bHeld) {
//			printf("Type: %i, BaseIndex: %X, StartIndex: %X, IndexCount: %X\n", PrimitiveType, BaseVertexIndex, StartIndex, IndexCount);
//			bHeld = true;
//		}
//		else {
//			bHeld = false;
//		}
//	}
//	D3DDevice_DrawIndexedVerticies_Detour.CallOriginal(pDevice, PrimitiveType, BaseVertexIndex, StartIndex, IndexCount);
//}
//
//int iRenderFlags = 0;
//Detour<VOID> R_AddDObjToScene_Detour;
//#if defined(BO1) || defined(BO2) || defined(MW2)
//VOID R_AddDObjToScene_Hook(PVOID obj, PVOID pose, int entnum, int renderFxFlags, float *lightingOrigin, DWORD r8, DWORD r9, DWORD r10, DWORD s1, DWORD s2, DWORD s3, DWORD s4, DWORD s5, DWORD s6, float materialTime, FLOAT f2, FLOAT f3, FLOAT f4, FLOAT f5, FLOAT f6) {
//#else
//VOID R_AddDObjToScene_Hook(PVOID obj, PVOID pose, unsigned int entnum, int renderFxFlags, vec3_t *lightingOrigin, float materialTime) {
//#endif
//	if (CG::bWallhack) {
//		if (entnum >= 0 && entnum < __MAXENTITIES__) {
//			if (CG::cEntities.List[entnum].bValid && CG::cEntities.List[entnum].bAlive && (CG::cEntities.List[entnum].bPlayer || CG::cEntities.List[entnum].bAI)) {
//				if (entnum != CG::iLocalClient) {
//				#if defined(MW2)
//					iRenderFlags |= 2;
//				#elif !defined(BO1)
//					renderFxFlags = -1;
//				#endif
//				#if defined(MW1) || defined(WAW) || defined(MW2)
//					entnum = __WORLDENTNUM__ - entnum;
//				#endif
//				}
//			}
//		}
//	}
//#if defined(BO1) || defined(BO2) || defined(MW2)
//	R_AddDObjToScene_Detour.CallOriginal(obj, pose, entnum, renderFxFlags, lightingOrigin, r8, r9, r10, s1, s2, s3, s4, s5, s6, materialTime, f2, f3, f4, f5, f6);
//#else
//	R_AddDObjToScene_Detour.CallOriginal(obj, pose, entnum, renderFxFlags, lightingOrigin, materialTime);
//#endif
//}
#pragma endregion

/* Apply our title-specific detours */
BOOL TitleHooks::Apply() {
	if (!bInitialized) {
		// Test detours
		//// MW2
		//Dvar_RegisterNew_Detour.SetupDetour(0x822A0038, Dvar_RegisterNew_Hook); // BO2 0x82497720, AW 0x823669C0, BO1 0x823E3DE0
		//UI_DrawText_Detour.SetupDetour(decryptPatch(Not(NiNJA.cPatches)->CoD.UI_DrawText), &UI_DrawText_Hook); 

		// Setup our important detours first, and return if we fail to hook any of them
		if (!R_EndFrame_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.R_EndFrame), &R_EndFrame_Hook)) {
			DbgPrint("Failed to hook R_EndFrame! Cannot continue!");
			return FALSE;
		}
		else if (!CL_AddReliableCommand_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_AddReliableCommand), &CL_AddReliableCommand_Hook)) {
			DbgPrint("Failed to hook CL_AddReliableCommand! Cannot continue!");
			return FALSE;
		}
		else if (!XamInputGetState_Detour.SetupDetour(0x82000000, ES("xam.xex"), 0x191, &XamInputGetState_Hook)) {
			DbgPrint("Failed to hook XamInputGetState! Cannot continue!");
			return FALSE;
		}
		else if (!CL_CreateNewCommands_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_CreateNewCommands), &CL_CreateNewCommands_Hook)) {
			DbgPrint("Failed to hook CL_CreateNewCommands! Cannot continue!");
			return FALSE;
		}
		else if (!CG_EntityEvent_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_EntityEvent), &CG_EntityEvent_Hook)) {
			DbgPrint("Failed to hook CG_EntityEvent! Cannot continue!");
			return FALSE;
		}
		else if (!CL_SendCommand_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.CL_SendCommand), &CL_SendCommand_Hook)) {
			DbgPrint("Failed to hook CL_SendCommand! Cannot continue!");
			return FALSE;
		}
		else if (!CG_Respawn_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_Respawn), CG_Respawn_Hook)) {
			DbgPrint("Failed to hook CG_Respawn! Cannot continue!");
			return FALSE;
		}
		//else if (!R_AddDObjToScene_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.R_AddDObjToScene), &R_AddDObjToScene_Hook)) {
		//	DbgPrint("Failed to hook RAddDObjToScene! Cannot continue!");
		//	return FALSE;
		//}
//#if defined(MW2)
//		else if (!D3DDevice_DrawIndexedVerticies_Detour.SetupDetour(0x820AF1D0, &D3DDevice_DrawIndexedVerticies_Hook)) { // R_DrawIndexedPrimitive, real addr: 0x8290E1A8
//			DbgPrint("Failed to hook D3DDevice_DrawIndexedVerticies! Cannot continue!");
//			return FALSE;
//		}
//#endif
	/*#if defined(BO2) || defined(MW2)
		else if (!SCR_UpdateFrame_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.SCR_UpdateFrame), &SCR_UpdateFrame_Hook)) {
			DbgPrint("Failed to hook SCR_UpdateFrame! Cannot continue!");
			return FALSE;
		}
	#endif*/
		
		//else if (!XSessionCreate_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.XSessionCreate), &XSessionCreate_Hook)) {
		//	DbgPrint("Failed to hook XSessionCreate! Cannot continue!");
		//	return FALSE;
		//}
		//else if (!XSessionDelete_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.XSessionDelete), &XSessionDelete_Hook)) {
		//	DbgPrint("Failed to hook XSessionDelete! Cannot continue!");
		//	return FALSE;
		//}
	#if defined(BO1) || defined(BO2) || defined(BO3)
		else if (!XamUserGetName_Detour.SetupDetour(0x82000000, ES("xam.xex"), 0x20E, &XamUserGetName_Hook)) {
			DbgPrint("Failed to hook XamUserGetName! Cannot continue!");
			return FALSE;
		}
	#endif
	#if !defined(BO3)
		else if (!CG_CompassDrawPlayerMap_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_CompassDrawPlayerMap), CG_CompassDrawPlayerMap_Hook)) {
			DbgPrint("Failed to hook CG_CompassDrawPlayerMap! Cannot continue!");
			return FALSE;
		}
		#if !defined(GHOSTS) && !defined(AW) && !defined(BO1) && !defined(BO2)
		else if (!CG_CompassDrawPlayerMapLocationSelector_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_CompassDrawPlayerMapLocationSelector), CG_CompassDrawPlayerMapLocationSelector_Hook)) {
			DbgPrint("Failed to hook CG_CompassDrawPlayerMapLocationSelector! Cannot continue!");
			return FALSE;
		}
		#endif
	#endif
	#if !defined(AW) && !defined(BO3)
		else if(!BG_UpdatePlayerDObj_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_UpdatePlayerDObj), &BG_UpdatePlayerDObj_Hook)) {
			DbgPrint("Failed to hook BG_UpdatePlayerDObj! Cannot continue!");
			return FALSE;
		}
	#endif
	#if defined(MW1) || defined(WAW) || defined(MW2)
		else if (!LiveStorage_SetStat_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.LiveStorage_SetStat), &LiveStorage_SetStat_Hook)) {
			DbgPrint("Failed to hook LiveStorage_SetStat! Cannot continue!");
			return FALSE;
		}
	#endif
	#if defined(MW3) || defined(GHOSTS) || defined(AW)
		else if (!LiveStats_WriteStat_Detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.LiveStorage_SetStat), &LiveStats_WriteStat_Hook)) {
			DbgPrint("Failed to hook LiveStorage_SetStat! Cannot continue!");
			return FALSE;
		}
	#endif
		
	#if defined(BO2)
		//NetDll_XNetGetTitleXnAddr_Detour.SetupDetour(0x82000000, ES("xam.xex"), 0x49, NetDll_XNetGetTitleXnAddr_Hook);
		CBuf_AddText_detour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.CBuf_AddText), CBuf_AddText_Hook);
		PreGameAntiFreezeDetour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.PreGameAntiFreezeDetour), (PVOID)PreGameAntiFreezeHook);
		GameMessageOneDetour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.GameMessage1), (PVOID)GameMessageOneHook);
		GameMessageTwoDetour.SetupDetour(decryptDWORD(Not(NiNJA.cPatches)->CoD.GameMessage2), (PVOID)GameMessageTwoHook);
		sub_821D8A18Detour.SetupDetour(0x821D8A18, (PVOID)sub_821D8A18Hook);
		XMemCpy_Detour.SetupDetour(0x827D33F0, XMemCpy_Hook);
		DynEntCl_DestroyEventDetour.SetupDetour(0x822DDE20, DynEntCl_DestroyEvent_Hook);
		//MSG_WriteBitsCompress_Detour.SetupDetour(0x8241A2D8, MSG_WriteBitsCompress_Hook);
	#endif
	#if defined(MW2)
		RB_SetTessTechniqueDetour.SetupDetour(0x82373AF8, RB_SetTessTechniqueHook);
	#endif

		DbgPrint("Applied detours!");
		bInitialized = TRUE;
		return TRUE;
	}
	else {
		DbgPrint("TitleHooks already initialized!");
	}
	return FALSE;
}

/* Take down our title-specific detours */
VOID TitleHooks::Remove(BOOL bOverride) {
	if (bInitialized || bOverride) {
		// Take down our detours
		BG_UpdatePlayerDObj_Detour.TakeDownDetour();
		CL_CreateNewCommands_Detour.TakeDownDetour();
		R_EndFrame_Detour.TakeDownDetour();
		XamInputGetState_Detour.TakeDownDetour();
		CG_CompassDrawPlayerMap_Detour.TakeDownDetour();
		CG_CompassDrawPlayerMapLocationSelector_Detour.TakeDownDetour();
		CG_EntityEvent_Detour.TakeDownDetour();
		CL_SendCommand_Detour.TakeDownDetour();
		XamUserGetName_Detour.TakeDownDetour();
		CL_AddReliableCommand_Detour.TakeDownDetour();
		CG_Respawn_Detour.TakeDownDetour();
		//D3DDevice_DrawIndexedVerticies_Detour.TakeDownDetour();
		//R_AddDObjToScene_Detour.TakeDownDetour();
		//R_DrawIndexedPrimitive_Detour.TakeDownDetour();
		//SCR_UpdateFrame_Detour.TakeDownDetour();

	#if defined(MW1) || defined(WAW) || defined(MW2)
		LiveStorage_SetStat_Detour.TakeDownDetour();
	#elif defined(MW3) || defined(GHOSTS) || defined(AW)
		LiveStats_WriteStat_Detour.TakeDownDetour();
	#endif

	#if defined(BO2)
		XMemCpy_Detour.TakeDownDetour();
		PreGameAntiFreezeDetour.TakeDownDetour();
		GameMessageOneDetour.TakeDownDetour();
		GameMessageTwoDetour.TakeDownDetour();
		sub_821D8A18Detour.TakeDownDetour();
		DynEntCl_DestroyEventDetour.TakeDownDetour();
		CBuf_AddText_detour.TakeDownDetour();
		//MSG_WriteBitsCompress_Detour.TakeDownDetour();
		//NetDll_XNetGetTitleXnAddr_Detour.TakeDownDetour();
	#endif
	#if defined(MW2)
		RB_SetTessTechniqueDetour.TakeDownDetour();
	#endif

		//XSessionCreate_Detour.TakeDownDetour();
		//XSessionDelete_Detour.TakeDownDetour();

		// Test detours
		Dvar_RegisterNew_Detour.TakeDownDetour();
		//UI_DrawText_Detour.TakeDownDetour();

		if (dwOrigIP) {
			// Reset our ip back to normal
			GameFuncs::SpoofIPAddress(dwOrigIP);
		}

		Sleep(1000);

		DbgPrint("Removed detours!");
		bInitialized = FALSE;
	}
	else {
		DbgPrint("TitleHooks have not been initialized!");
	}
}
