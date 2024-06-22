#include "Entities.h"
#include "Globals.h"
#include "GameFuncs.h"
#include "SecureStrings.h"

PCHAR ExtendedEntity::GetName() {
	return CG::bShowRealNames ? (pRealName ? ((!Compare(pRealName, "") && pRealName[0] != 0) ? pRealName : pName) : pName) : pName;
}

VOID Entity::ResetExtention(BOOL bFull) {
	if (MmIsAddressValid(pExt)) {
		pExt->bThreadRunning = false;
		pExt->pXModel = NULL;
		pExt->pDObj = NULL;
		pExt->bFiring = false;
		pExt->bAiming = false;
		pExt->bWhitelisted = false;
		pExt->bPreferred = false;
		pExt->bPrimaryShield = false;
		pExt->bSecondaryShield = false;
		pExt->bVisible = false;
		pExt->bWallbangable = false;
		pExt->bGrabGeoIPInfo = false;
		pExt->bWeaponDualWield = false;
		pExt->bTopPlayer = false;
		pExt->dwCurrentWeapon = 0;
		pExt->dwSecondaryWeapon = 0;
		pExt->sCurrentStreak = 0;
		pExt->sCurrentSpree = 0;
		pExt->sAmmoRightHand = 0;
		pExt->sAmmoLeftHand = 0;
		pExt->dwStance = 0;
		pExt->sWeaponClipSize = 0;
		pExt->dwLastKillTime = 0;
		pExt->dwSpreeEndTime = 0;
		pExt->iThreatAssessment = 0;
		pExt->sLookingAtBoneIndex = 0;
		pExt->bEvaluatingHacker = false;
		pExt->dwHackerCheckStartTime = 0;
		pExt->fDelta = 0.0f;
		pExt->fOldDelta = 0.0f;
		pExt->bSkipNextFrame = false;
		for (int i = 0; i < TAG_SIZE; i++) {
			pExt->cTags[i].bWallbangable = false;
			pExt->cTags[i].bVisible = false;
			pExt->cTags[i].bBlockedByShield = false;
			pExt->cTags[i].bTagOffset = 0;
		}
		if (bFull) {
			pExt->pNetInfo = NULL;
			pExt->dwLastAbnormalAngle = 0;
		}
	}
}

VOID Entity::Reset(BOOL bFull, SHORT sIndex) {
	if (bFull) {
		this->sIndex = sIndex;
		pClientInfo = NULL;
		//pDObj = NULL;
		pCentity = NULL;

		if (sIndex >= __MAXCLIENTS__) {
			// Free our extended entity for non-player entities, if it exists
			FreeExtendedEntity();
			pExt = NULL;
		}
	}
	bOnScreen = false;
	bPlayer = false;
	bAI = false;
	bValid = false;
	bAlive = false;
	bEnemy = false;
	bInvisible = false;
	bKillable = true;
	sEntType = 0;
	sTeam = 0;
	fDistance = 0.0f;
	vLocation.Erase();
	vScreen.Erase();

	// Reset our entityextention if it exists
	ResetExtention(bFull);
}

bool Entity::AllocExtendedEntity() {
	if (!pExt) {
		if (CG::mExtendedEntityManager.alloc(sizeof(ExtendedEntity), &pExt)) {
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}

VOID Entity::FreeExtendedEntity() {
	if (pExt) {
		CG::mExtendedEntityManager.free((DWORD)pExt);
	}
}