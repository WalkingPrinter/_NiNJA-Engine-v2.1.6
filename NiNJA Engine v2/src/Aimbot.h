#pragma once
#include <xtl.h>
#include "Debug.h"
#include "Globals.h"
#include "GameFuncs.h"
#include "Buttons.h"
#include "UserCmd.h"

namespace Aimbot
{
	static int iCurrentTarget = -1;
	static bool bTargetUsingShield = false;
	static vec3_t realAimAngle;
	static vec2_t vScreenCenter(0, 0);
	static vec3_t vOldLocalAngles, vLocalAngles, vAimbotAngles, vTargetPosition;
	static bool bIsTargeting = false, bIsDelaying = false;
	static DWORD dwDelayStartTime = 0, dwDelayTime = 0;
	static int iLastTarget = -1;
	
	static bool ValidTarget(DWORD dwEntNum) {
		// Skip invalid entities
		if (!CG::cEntities.List[dwEntNum].bValid) {
			return false;
		}

		// Skip any entities that aren't player or AI
		if (!(CG::cEntities.List[dwEntNum].bPlayer || CG::cEntities.List[dwEntNum].bAI)) {
			return false;
		}

		// Skip friendlies
		if (!CG::cEntities.List[dwEntNum].bEnemy) {
			return false;
		}

		// Skip our own entity
		if (dwEntNum == CG::iLocalClient) {
			return false;
		}

		// Skip dead clients
		if (!CG::cEntities.List[dwEntNum].bAlive) {
			return false;
		}

		// Skip clients who are offscreen if the option is disabled in the menu
		if (!CG::bOffscreenTargeting && !CG::cEntities.List[dwEntNum].bOnScreen) {
			return false;
		}

		// Skip clients who don't have a valid entityextention
		if (!CG::cEntities.List[dwEntNum].pExt) {
			return false;
		}

		// Skip unkillable entities
		if (!CG::cEntities.List[dwEntNum].bKillable) {
			return false;
		}
		
		// Skip invisible entities, if we're not in multiplayer
		if (CG::cEntities.List[dwEntNum].bInvisible && !CG::cEntities.List[dwEntNum].bPlayer && !CG::bIsMultiplayer) {
			return false;
		}

		// Skip players, if we aren't in multiplayer. Theres no point in trying to aimbot players if we can't harm them
		if (CG::cEntities.List[dwEntNum].bPlayer && !CG::bIsMultiplayer) {
			return false;
		}

		if (CG::cEntities.List[dwEntNum].pExt->bWhitelisted) {
			return false;
		}

		// Completely ignore them if BOTH their weapons are shields
		if (CG::cEntities.List[dwEntNum].pExt->bPrimaryShield && CG::cEntities.List[dwEntNum].pExt->bSecondaryShield) {
			return false;
		}
		return true;
	}


	// NOTE: EACH NEW PRIORITY NUMBER MUST BE AT LEAST 2x AS MUCH AS THE PREVIOUS ONE!
	static int CalcTargetPriority(DWORD dwTarget) {
		int iPriority = 0;

		// Visibility increases their priority by 32000
		if (CG::cEntities.List[dwTarget].pExt->bVisible) {
			iPriority += 32000;
		}
		// Being wallbangable increases their priority by 16000
		else if (CG::cEntities.List[dwTarget].pExt->bWallbangable) {
			iPriority += 16000;
		}

		if (CG::dwAimbotType != AIM_TYPE_HUMANIZED) {
			// Being prioritized increases their priority by 8000
			if (CG::cEntities.List[dwTarget].pExt->bPreferred) {
				iPriority += 8000;
			}

			// Add in their "threat assessment", an extra 2000 if they are in the top 2
			iPriority += CG::cEntities.List[dwTarget].pExt->bTopPlayer ? 4000 : 0;
		}

		// If they are looking at us, their priority jumps up by the priority of the bone they are looking at (2000 for head, neck, helmet, 1000 for mainroot, hips, 500 for others). This priority setup 
		// ensures that if a non-top-2 person is looking at our head, it will target them over a top 2 person who isnt looking at our head (of equal visibility)
		if (CG::cEntities.List[dwTarget].pExt->sLookingAtBoneIndex != -1) {
			// Grab the priority from the bone they're looking at, and add it to our total
			iPriority += CG::cTags.List[CG::cEntities.List[dwTarget].pExt->sLookingAtBoneIndex].sPriority;
		}

		// Finally, calculate their disance into this value, so that if two players would have otherwise had the same priority rating, this should bump it one way or the other. 
		// This also makes it so that if no one met any of our prioritization criteria, the aimbot still targets the closest person :)
		int iDist = (int)floor(CG::cEntities.List[dwTarget].fDistance / 100.0f);
		iPriority += (iDist >= 500 ? 0 : 500 - iDist);
		return iPriority;
	}

	static bool FindBestTarget() {
		int iTargetPriority = -1;
		int iTmpTarget = -1;
		float fClosestDistance = FLT_MAX;
		vec2_t vClosestPos(999999, 999999);
		bool bPreferredCheck = true, bVisibleCheck = false, bWallbangableCheck = false, bFinalCheck = false;

		// If we're using the humanized aimbot, keep our same target until they aren't visibile or living anymore, only in multiplayer, though :)
		if (CG::dwAimbotType == AIM_TYPE_HUMANIZED && CG::bIsMultiplayer) {
			// Check if we even have a target
			if (iCurrentTarget == -1) {
				goto jStart;
			}

			// If we do, make sure they're still valid
			if (!ValidTarget(iCurrentTarget)) {
				goto jStart;
			}

			// If we're looking at a different entity, interrupt the legit aimbot and aim at the new best target
			if (CG::iLookingAtEntity != -1) {
				if (CG::iLookingAtEntity >= 0 && CG::iLookingAtEntity < __MAXENTITIES__) {
					if (CG::cEntities.List[CG::iLookingAtEntity].bPlayer || CG::cEntities.List[CG::iLookingAtEntity].bAI) {
						if (CG::cEntities.List[CG::iLookingAtEntity].pExt) {
							if (CG::cEntities.List[CG::iLookingAtEntity].pExt->bVisible) {
								if (CG::iLookingAtEntity != iCurrentTarget) {
									goto jStart;
								}
							}
						}
					}
				}
			}

			// If they are, continue to target them as long as they are visible/wallbangable and on screen
			if (CG::cEntities.List[iCurrentTarget].pExt) {
				if (CG::cEntities.List[iCurrentTarget].pExt->bVisible && CG::cEntities.List[iCurrentTarget].bOnScreen) {
					goto jEnd;
				}
			}
		}
	
	jStart:
		for (DWORD i = 0; i < __MAXENTITIES__; i++) {
			if (!ValidTarget(i)) {
				continue;
			}
			
			if (vScreenCenter.IsZero()) {
				vScreenCenter.Set((FLOAT)NiNJA.dwScreenCenterX, (FLOAT)NiNJA.dwScreenCenterY);
			}

			CG::cEntities.List[i].pExt->iPriority = CalcTargetPriority(i);
			if (CG::dwAimbotType != AIM_TYPE_HUMANIZED && /*CG::cEntities.List[i].fDistance >= fClosestDistance*/ CG::cEntities.List[i].pExt->iPriority <= iTargetPriority ||
				CG::dwAimbotType == AIM_TYPE_HUMANIZED && (!CG::cEntities.List[i].vScreen.IsWithin(vClosestPos, vScreenCenter) || CG::cEntities.List[i].pExt->iPriority  < 4000)) { //TODO: Potentially change this code to use the lowest angle delta between our current viewangles and the angles needed to aimbot the client
				continue;
			}
			else {
				iTargetPriority = CG::cEntities.List[i].pExt->iPriority;
				vClosestPos = CG::cEntities.List[i].vScreen;
				fClosestDistance = CG::cEntities.List[i].fDistance;
				iTmpTarget = i;
			}
		}
		// Set our target player
		iCurrentTarget = iTmpTarget;

	jEnd:

		// Make sure our target was valid
		bool bValid = iCurrentTarget >= 0;
		if (bValid) {
			bTargetUsingShield =  CG::cEntities.List[iCurrentTarget].pExt->bPrimaryShield;
		}
		return bValid;
	}

	static VOID CorrectMovement(int userCmd) {
		int playerMove = UserCmd::GetUserCmdPlayerMove(userCmd);
		if (playerMove) {
			vec3_t refDefAngles = GameFuncs::Get_RefDef_ViewAngles();

			float differenceY = realAimAngle.y - refDefAngles.y;

			int forward = cos(DEG2RAD(differenceY)) * *(signed char*)(playerMove)+cos(DEG2RAD(differenceY + 90.0f)) * *(signed char*)(playerMove + 0x1);
			int right = sin(DEG2RAD(differenceY)) * *(signed char*)(playerMove)+sin(DEG2RAD(differenceY + 90.0f)) * *(signed char*)(playerMove + 0x1);

			*(signed char*)(playerMove) = ClampChar(forward);
			*(signed char*)(playerMove + 0x1) = ClampChar(right);
		}
	}

	static int FindBestAimBone() {
		if (CG::cEntities.List[iCurrentTarget].pExt->bVisible) {
			return CG::cEntities.List[iCurrentTarget].pExt->sVisibleBone;
		}
		else if (CG::cEntities.List[iCurrentTarget].pExt->bWallbangable) {
			return CG::cEntities.List[iCurrentTarget].pExt->sWallbangableBone;
		}
		return CG::dwAimBone;
	}
	
	static vec3_t SmoothAimbotAngles(vec3_t *pClientActiveViewAngles, FLOAT fSmoothValue, vec3_t &pAimbotAngles) {
		vec3_t FinishedAngles; float smoothdiff[3];
		// Subtract our actual angles from our new angles, to get the delta
		pAimbotAngles.x -= pClientActiveViewAngles->x;
		pAimbotAngles.y -= pClientActiveViewAngles->y;

		// Normalize the delta
		pAimbotAngles.x = AngleNormalize(pAimbotAngles.x);
		pAimbotAngles.y = AngleNormalize(pAimbotAngles.y);

		fSmoothValue = abs(fSmoothValue);

		FLOAT fSmoothAmount = CG::fSmoothAmount;
		// Calculate a nice smooth value based off the difference between the two sets of angles
		smoothdiff[0] = pAimbotAngles.x / fSmoothAmount;
		smoothdiff[1] = pAimbotAngles.y / fSmoothAmount;

		// If our smooth value is over the max speed threshold (this means we were spinning faster than fMaxSpeedThreshold), multiply the y smoothdiff by just less than half of our speed
		if (fSmoothValue > CG::fMaxSpeedThreshold) {
			//smoothdiff[0] *= fSmoothValue;
			smoothdiff[1] *= (fSmoothValue * 0.2f);
		}
		
		// Then set it to the finished angles variable
		FinishedAngles.x = pClientActiveViewAngles->x + smoothdiff[0];
		FinishedAngles.y = pClientActiveViewAngles->y + smoothdiff[1];

		// Normalize it one final time, then return
		FinishedAngles.x = AngleNormalize(FinishedAngles.x);
		FinishedAngles.y = AngleNormalize(FinishedAngles.y);
		return FinishedAngles;
	}

	static VOID Rape() {
		vOldLocalAngles = vLocalAngles;
		vLocalAngles = GameFuncs::Get_RefDef_ViewAngles();

		// If we find a valid target
		if (FindBestTarget()) {
			// Only apply reaction delay to our humanized aimbot
			if (CG::dwAimbotType == AIM_TYPE_HUMANIZED) {
				// Check for our reaction delay, and apply it if its greater than 0
				if (iLastTarget != iCurrentTarget) {
					if (CG::dwReactionDelay > 0) {
						dwDelayTime = CG::dwReactionDelay;
						dwDelayStartTime = GetTickCount();
						bIsDelaying = true;
					}
					// Save our last target
					iLastTarget = iCurrentTarget;
				}

				// Exit the function if we're delaying
				if (bIsDelaying && dwDelayStartTime + dwDelayTime < GetTickCount()) {
					// The delay is done, lets continue now
					bIsDelaying = false;
				}
				else if (bIsDelaying) {
					// We're still delaying, don't aimbot yet
					return;
				}
			}

			vec3_t vTargetPos;
			// If they're using a shield, just shoot at their ankle
			//if (bTargetUsingShield) {
			//	GameFuncs::GetTagPosition(TAG_ANKLE_LE, iCurrentTarget, &vTargetPos);
			//}
			// Use our FindBestAimBone function to choose which bone we aim at... if none are visible or wallbangable it will return the selected aimbone
			//else {
				DWORD dwBone = FindBestAimBone();
				GameFuncs::GetTagPosition(dwBone, iCurrentTarget, &CG::cEntities.List[iCurrentTarget].pExt->cTags[dwBone].vPos);
				vTargetPos = GameFuncs::GetTagOffset(dwBone, iCurrentTarget, CG::cEntities.List[iCurrentTarget].pExt->cTags[dwBone].bTagOffset);
			//}
			
			// If we got our viewOrigin and we got their position
			if (!vTargetPos.IsZero() ) {

				// Save this for us in other functions
				vTargetPosition = CG::cEntities.List[iCurrentTarget].vLocation;

				// Grab our local viewOrigin for use in out aimbot calculation
				vec3_t vLocalViewOrigin = GameFuncs::GetLocalViewOrigin();


				// Calculate the difference between our current and old viewangles to find out how fast we're spinning
				FLOAT fLocalDelta = GetAngleDelta(vLocalAngles.y, vOldLocalAngles.y);
				//DbgPrintE("fLocalDelta: %f", fLocalDelta);

				// Calculate the final aim vector
				vec3_t vFinalVec = vectoangles(vTargetPos.x - vLocalViewOrigin.x, vTargetPos.y - vLocalViewOrigin.y, vTargetPos.z - vLocalViewOrigin.z);

				// Store our real aim angle for later, when we correct our movement
				realAimAngle.Set(vFinalVec);

				// Grab the current spawn angles and calculate them in
				vec3_t *vSpawnAngles = GameFuncs::GetSpawnAngles();

				// If we grabbed our spawn angles successfully
				if (vSpawnAngles) {
					// Calculate our final aimbot angles
					vec3_t vFinalAimAngles(AngleNormalize(vFinalVec.x - vSpawnAngles->x), AngleNormalize(vFinalVec.y - vSpawnAngles->y), AngleNormalize(vFinalVec.z - vSpawnAngles->z));

					// Store this for use other uses like anti aim
					vAimbotAngles = vFinalAimAngles;

					// Rape them
					bool bAimKeyHeld = (CG::cButtons.dwFlags & CG::dwAimButton) == CG::dwAimButton;
					if (bAimKeyHeld || (CG::bAimbotWhenFiring && (CG::cButtons.bFiring || CG::cButtons.bFiringSecondary))) {
						// Apply our aim angles to our clientactive viewangles if normal aimbbot is enabled
						if (CG::dwAimbotType == AIM_TYPE_NORMAL || (CG::dwAimbotType == AIM_TYPE_SILENT && CG::bInKillstreak)) {
							vec3_t *vClientActiveViewAngles = GameFuncs::Get_ClientActive_ViewAngles();
							if (vClientActiveViewAngles) {
								vClientActiveViewAngles->x = vFinalAimAngles.x;
								vClientActiveViewAngles->y = vFinalAimAngles.y;
							}
						}
						// Apply our humanized aim angles if the humanized aimbot is enabled, or if we're in a killstreak
						else if (CG::dwAimbotType == AIM_TYPE_HUMANIZED) {
							vec3_t *vClientActiveViewAngles = GameFuncs::Get_ClientActive_ViewAngles();
							if (vClientActiveViewAngles) {
								vec3_t vOut = vFinalAimAngles;
								if (!vClientActiveViewAngles->IsWithin(CG::fHitTargetRadius, vFinalAimAngles)) {
									vOut = SmoothAimbotAngles(vClientActiveViewAngles, fLocalDelta, vFinalAimAngles);
									*(FLOAT*)&vClientActiveViewAngles->x = vOut.x;
									*(FLOAT*)&vClientActiveViewAngles->y = vOut.y;
								}
							}
						}
						// Apply our silent aim angles to  both our normal and silent aimbot... this seems to make our normal aimbot more accurate :)
						else if (CG::dwAimbotType == AIM_TYPE_SILENT) {
							int oldUserCmd = UserCmd::GetOldUserCmd();
							int viewAngles = UserCmd::GetUserCmdViewAngles(oldUserCmd);
							*(int*)(viewAngles) = AngleToShort(vFinalAimAngles.x);
							*(int*)(viewAngles + 0x4) = AngleToShort(vFinalAimAngles.y);
						#if !defined(BO3)
							CorrectMovement(oldUserCmd);
						#endif
						}
					}
				}
			}
		}
		else {
			if (bIsTargeting) {
				bIsDelaying = false;
				bIsTargeting = false;
				dwDelayTime = 0;
				dwDelayStartTime = 0;
				DbgPrint("No longer targeting any entities.");
			}
		}
	}
};

