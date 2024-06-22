#pragma once
#include "Spinbot.h"
#include "Globals.h"
#include "UserCmd.h"

namespace Spinbot {
	static bool bPitchFlip = false, bFlip = false;
	static FLOAT fCurrentPitch = 0.0f, fCurrentYaw = 0.0f, fCurrentRoll = 0.0f;
	static int iLastTarget = -1;
	static DWORD dwLastFlipTime = 0;

	static VOID FixSpinbotZ() {
		vec3_t *spawnAngles = GameFuncs::GetSpawnAngles();
		if (spawnAngles) {
			vec3_t *pClientActiveViewAngles = GameFuncs::Get_ClientActive_ViewAngles();
			if (pClientActiveViewAngles) {
				pClientActiveViewAngles->z = 0 - spawnAngles->z;
			}
		}
	}

	static int iAntiAimClient = -1;
	static VOID SetDynamicAntiAimAngle() {
		// Grab the current spawn angles and calculate them in
		vec3_t *vSpawnAngles = GameFuncs::GetSpawnAngles();

		// If we grabbed our spawn angles successfully
		if (vSpawnAngles) {

			if (iAntiAimClient != -1) {
				vec3_t vLocalViewOrigin = CG::cEntities.List[CG::iRealLocalClient].vLocation + vec3_t(0, 0, 60), vTargetPos = CG::cEntities.List[iAntiAimClient].pExt->cTags[TAG_HEAD].vPos;

				// Calculate the final aim vector
				vec3_t vFinalVec = vectoangles(vTargetPos.x - vLocalViewOrigin.x, vTargetPos.y - vLocalViewOrigin.y, vTargetPos.z - vLocalViewOrigin.z);

				float fVal = (vLocalViewOrigin.z - vTargetPos.z) / 4.0f;
				fCurrentPitch = ShieldAntiAimAdditave - fVal;
				if (fCurrentPitch < __MINPITCH__) {
					fCurrentPitch = __MINPITCH__;
				}
				else if (fCurrentPitch > __MAXPITCH__) {
					fCurrentPitch = __MAXPITCH__;
				}
				fCurrentPitch -= vSpawnAngles->x;
				//printf(GameFuncs::va("fVal: %f\n", fVal));

				// Grab the current usercmd and a pointer to our viewangles
				int currentUserCmd = UserCmd::GetCurrentUserCmd();
				int viewAngles = UserCmd::GetUserCmdViewAngles(currentUserCmd);

				// Normalize our angle then set it
				fCurrentPitch = AngleNormalize(fCurrentPitch);
				*(int*)(viewAngles) = AngleToShort(fCurrentPitch);

				return;
			}
			fCurrentPitch = ShieldAntiAimAdditave - vSpawnAngles->x;
		}
	}

	static VOID SetFaceAwayAngle() {
		if (iAntiAimClient != -1) {
			vec3_t vLocalViewOrigin = CG::cEntities.List[CG::iRealLocalClient].vLocation + vec3_t(0,0,60), vTargetPos = CG::cEntities.List[iAntiAimClient].pExt->cTags[TAG_HEAD].vPos;

			// Calculate the final aim vector
			vec3_t vFinalVec = vectoangles(vTargetPos.x - vLocalViewOrigin.x, vTargetPos.y - vLocalViewOrigin.y, vTargetPos.z - vLocalViewOrigin.z);

			// Grab the current spawn angles and calculate them in
			vec3_t *vSpawnAngles = GameFuncs::GetSpawnAngles();

			// If we grabbed our spawn angles successfully
			if (vSpawnAngles) {
				// Calculate our final aimbot angles
				vec3_t vFinalAimAngles(AngleNormalize(vFinalVec.x - vSpawnAngles->x), AngleNormalize(vFinalVec.y - vSpawnAngles->y), AngleNormalize(vFinalVec.z - vSpawnAngles->z));

				// Face away 180 degrees from the calculated pos
				fCurrentYaw = vFinalAimAngles.y + 180.0f;
				return;
			}
		}
		// Face away 180 degrees from the last aimbot target
		fCurrentYaw = Aimbot::vAimbotAngles.y + 180.0f;
	}

	static VOID Apply() {
		if (!CG::cEntities.List[CG::iRealLocalClient].bAlive) {
			return;
		}

		if (CG::dwCurrentStance != STANCE_PRONE && !(CG::cButtons.dwFlags & BUTTON_RB) && !(CG::cButtons.dwFlags & BUTTON_LB) && !GameFuncs::IsMantling()) {
			int iPriority = -1;
			iAntiAimClient = -1;
			// Find the highest priority client, taking into consideration if the player is preferred or not
			for (int i = 0; i < __MAXCLIENTS__; i++) {
				if (CG::cEntities.List[i].bValid && i != CG::iRealLocalClient &&  CG::cEntities.List[i].bEnemy && CG::cEntities.List[i].bAlive && CG::cEntities.List[i].pExt && !CG::cEntities.List[i].pExt->bWhitelisted) {
					if (iAntiAimClient != -1) {
						// If we already have a client selected, check if they are preferred. If they are, make sure we don't change our anti-aim target for a non-preferred player
						if (CG::cEntities.List[iAntiAimClient].pExt->bPreferred && !CG::cEntities.List[i].pExt->bPreferred) {
							continue;
						}
					}
					if (CG::cEntities.List[i].pExt->iPriority  > iPriority || (iAntiAimClient != -1 && CG::cEntities.List[i].pExt->bPreferred && !CG::cEntities.List[iAntiAimClient].pExt->bPreferred)) {
						iPriority = CG::cEntities.List[i].pExt->iPriority;
						iAntiAimClient = i;
					}
				}
			}

			//printf(GameFuncs::va("UserCmd weapon: %X", *(int*)(UserCmd::GetCurrentUserCmd() + 0x18)));
			// Grab our current spawn angles
			vec3_t *spawnAngles = GameFuncs::GetSpawnAngles();

			// Grab the current usercmd and a pointer to our viewangles
			int currentUserCmd = UserCmd::GetCurrentUserCmd();
			int viewAngles = UserCmd::GetUserCmdViewAngles(currentUserCmd);

			// Change our anti aim settings depending on whether or not we have a secondary shield
			if (CG::bAntiAim) {
				DWORD dwTmpPitch = CG::dwPitchSpinMode;
				DWORD dwTmpYaw = CG::dwYawSpinMode;
				if (CG::cEntities.List[CG::iRealLocalClient].pExt->bSecondaryShield) {
					CG::dwPitchSpinMode = PITCH_DYNAMIC;
					CG::dwYawSpinMode = YAW_FACE_AWAY;
				}
				else {
					CG::dwPitchSpinMode = PITCH_LOOK_DOWN;
					CG::dwYawSpinMode = YAW_LEFT;
				}
				if (dwTmpPitch != CG::dwPitchSpinMode || dwTmpYaw != CG::dwYawSpinMode) {
					UI::mManager.UpdateMenuStates();
				}
				CG::dwYawSpinSpeed = 100;
			}

			// Handle x-axis spinbot
			if (CG::dwPitchSpinMode != PITCH_OFF) {
				switch (CG::dwPitchSpinMode) {
				case PITCH_LOOK_UP:
					//if (Aimbot::vTargetPosition.z + 1.0f < CG::cEntities.List[CG::iRealLocalClient].vLocation.z) {
					//	fCurrentPitch = (ShieldAntiAimAdditave - 15.0f) - spawnAngles->x;
					//}
					//else if (Aimbot::vTargetPosition.z - 1.0f > CG::cEntities.List[CG::iRealLocalClient].vLocation.z) {
					//	fCurrentPitch = (ShieldAntiAimAdditave + 15.0f) - spawnAngles->x;
					//}
					//else {
					fCurrentPitch = ShieldAntiAimAdditave - spawnAngles->x;
					//}
					break;
				case PITCH_LOOK_DOWN:
					fCurrentPitch = 60.0f - spawnAngles->x;
					break;
				case PITCH_UP_AND_DOWN:
					if ((rand() % 2) == 0) {
						fCurrentPitch = -45.0f - spawnAngles->x;
					}
					else {
						fCurrentPitch = 60.0f - spawnAngles->x;
					}
					break;
				case PITCH_RANDOM:
					fCurrentPitch = RandomFloat(-45.0f, 60.0f);
					break;
				case PITCH_DYNAMIC:
					SetDynamicAntiAimAngle();
					break;
				}
				// Normalize our angle then set it
				fCurrentPitch = AngleNormalize(fCurrentPitch);
				*(int*)(viewAngles) = AngleToShort(fCurrentPitch);
			}

			// Handle y-axis spinbot
			if (CG::dwYawSpinMode != YAW_OFF) {
				if (Aimbot::iCurrentTarget != iLastTarget || dwLastFlipTime + 5000 < GetTickCount()) {
					bFlip = !bFlip;
					iLastTarget = Aimbot::iCurrentTarget;
					dwLastFlipTime = GetTickCount();
				}
				
				switch (CG::dwYawSpinMode) {
				case YAW_LEFT:
					fCurrentYaw = fCurrentYaw + CG::dwYawSpinSpeed;
					break;
				case YAW_RIGHT:
					fCurrentYaw = fCurrentYaw - CG::dwYawSpinSpeed;
					break;
				case YAW_FLIP:
					if (bFlip) {
						fCurrentYaw = fCurrentYaw - CG::dwYawSpinSpeed;
					}
					else {
						fCurrentYaw = fCurrentYaw + CG::dwYawSpinSpeed;
					}
					break;
				case YAW_FACE_AWAY:
					SetFaceAwayAngle();
					break;
				case YAW_RANDOM:
					fCurrentYaw = (FLOAT)(rand() % 360);
					break;
				}
				// Normalize our angle then set it
				fCurrentYaw = AngleNormalize(fCurrentYaw);
				*(int*)(viewAngles + 0x4) = AngleToShort(fCurrentYaw);
			}

			// Handle z-axis spinbot
			if (CG::dwRollSpinMode != ROLL_OFF) {
				switch (CG::dwRollSpinMode) {
				case ROLL_LEFT:
					fCurrentRoll = fCurrentRoll + CG::dwRollSpinSpeed;
					break;
				case ROLL_RIGHT:
					fCurrentRoll = fCurrentRoll - CG::dwRollSpinSpeed;
					break;
				case ROLL_FLIP:
					if (bFlip) {
						fCurrentRoll = fCurrentRoll - CG::dwYawSpinSpeed;
					}
					else {
						fCurrentRoll = fCurrentRoll + CG::dwYawSpinSpeed;
					}
					break;
				case ROLL_RANDOM:
					fCurrentRoll = (FLOAT)(rand() % 360);;
					break;
				}
				// Normalize our angle then set it
				fCurrentRoll = AngleNormalize(fCurrentRoll);
				*(int*)(viewAngles + 0x8) = AngleToShort(fCurrentRoll);

				// Don't apply our spinbot roll angles at all if we're firing on any of these titles
			#if defined(WAW) || defined(BO1) || defined(MW1)
				if (CG::cButtons.bFiring || CG::cButtons.bFiringSecondary) {
					*(int*)(viewAngles + 0x8) = AngleToShort(0 - spawnAngles->z);
				}
			#endif
			}
		}

		FixSpinbotZ();
	}
};