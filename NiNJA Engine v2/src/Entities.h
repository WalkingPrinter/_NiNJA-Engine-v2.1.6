#pragma once
#include <xtl.h>
#include <map>
#include <vector>
#include "Utilities.h"
#include "Debug.h"

using namespace std;

enum ePlayerTags {
	TAG_FOREHEAD,
	TAG_HEAD,
	TAG_HELMET,
	TAG_NECK,
	TAG_ANKLE_LE,
	TAG_ANKLE_RI,
	TAG_WRIST_LE,
	TAG_WRIST_RI,
	TAG_HIP_LE,
	TAG_HIP_RI,
	TAG_KNEE_LE,
	TAG_KNEE_RI,
	TAG_MAINROOT,
	TAG_ELBOW_LE,
	TAG_ELBOW_RI,
	TAG_SHOULDER_LE,
	TAG_SHOULDER_RI,
	TAG_SPINELOWER,
	TAG_SPINEUPPER,
	//TAG_JAW,

	// Prioritized clients only
	//TAG_HIPTWIST_LE,
	//TAG_HIPTWIST_RI,
	//TAG_SPINE,
	TAG_FOOT_LE,
	TAG_FOOT_RI,
	//TAG_CLAVICLE_LE,
	//TAG_CLAVICLE_RI,
	TAG_SHOULDERRAISE_LE,
	TAG_SHOULDERRAISE_RI,
	TAG_BROW_LE,
	TAG_BROW_RI,
	//TAG_ELBOWBULGE_LE,
	//TAG_ELBOWBULGE_RI,
	//TAG_ORIGIN,
	//TAG_PELVIS,
	//TAG_SYNC,
	//TAG_TORSO_STABILIZER,
	TAG_BACK_LOW,
	TAG_BACK_MID,
	//TAG_KNEE_BULGE_LE,
	//TAG_KNEE_BULGE_RI,
	//TAG_INHAND,
	//TAG_STOWED_BACK,
	//TAG_WEAPON_CHEST,
	TAG_CHEEK_LE,
	TAG_CHEEK_RI,
	//TAG_EYEBALL_LE,
	//TAG_EYEBALL_RI,
	//TAG_EYELID_BOT_LE,
	//TAG_EYELID_BOT_RI,
	//TAG_EYELID_TOP_LE,
	//TAG_EYELID_TOP_RI,
	//TAG_LEVATOR_LE,
	//TAG_LEVATOR_RI,
	//TAG_LIP_BOT_LE,
	//TAG_LIP_BOT_RI,
	//TAG_LIP_TOP_LE,
	//TAG_LIP_TOP_RI,
	//TAG_MOUTH_LE,
	//TAG_MOUTH_RI,
	//TAG_SHOULDERTWIST_LE,
	//TAG_SHOULDERTWIST_RI,
	//TAG_EYE,
	//TAG_WRISTTWIST_LE,
	//TAG_WRISTTWIST_RI,
	TAG_INDEX_LE1,
	TAG_INDEX_RI1,
	TAG_MID_LE1,
	TAG_MID_RI1,
	TAG_PINKY_LE1,
	TAG_PINKY_RI1,
	TAG_RING_LE1,
	TAG_RING_RI1,
	TAG_THUMB_LE1,
	TAG_THUMB_RI1,
	//TAG_INDEX_LE2,
	//TAG_INDEX_RI2,
	//TAG_MID_LE2,
	//TAG_MID_RI2,
	//TAG_PINKY_LE2,
	//TAG_PINKY_RI2,
	//TAG_RING_LE2,
	//TAG_RING_RI2,
	//TAG_THUMB_LE2,
	//TAG_THUMB_RI2,
	//TAG_INDEX_LE3,
	//TAG_INDEX_RI3,
	//TAG_MID_LE3,
	//TAG_MID_RI3,
	//TAG_PINKY_LE3,
	//TAG_PINKY_RI3,
	//TAG_RING_LE3,
	//TAG_RING_RI3,
	//TAG_THUMB_LE3,
	//TAG_THUMB_RI3,
	//TAG_SHIELD,

	// Custom tags
	TAG_CUSTOM_SHIN_LE,
	TAG_CUSTOM_SHIN2_LE,
	TAG_CUSTOM_SHIN3_LE,
	TAG_CUSTOM_SHIN_RI,
	TAG_CUSTOM_SHIN2_RI,
	TAG_CUSTOM_SHIN3_RI,
	TAG_CUSTOM_THIGH_LE,
	TAG_CUSTOM_THIGH2_LE,
	TAG_CUSTOM_THIGH3_LE,
	TAG_CUSTOM_THIGH_RI,
	TAG_CUSTOM_THIGH2_RI,
	TAG_CUSTOM_THIGH3_RI,
	TAG_CUSTOM_CHEST_LE,
	TAG_CUSTOM_CHEST2_LE,
	TAG_CUSTOM_CHEST3_LE,
	TAG_CUSTOM_CHEST_RI,
	TAG_CUSTOM_CHEST2_RI,
	TAG_CUSTOM_CHEST3_RI,
	TAG_CUSTOM_CHEST_UPPER,
	TAG_CUSTOM_CHEST_UPPER2,
	TAG_CUSTOM_CHEST_UPPER3,
	TAG_CUSTOM_FOREARM_LE,
	TAG_CUSTOM_FOREARM2_LE,
	TAG_CUSTOM_FOREARM3_LE,
	TAG_CUSTOM_ARM_LE,
	TAG_CUSTOM_ARM2_LE,
	TAG_CUSTOM_ARM3_LE,
	TAG_CUSTOM_FOREARM_RI,
	TAG_CUSTOM_FOREARM2_RI,
	TAG_CUSTOM_FOREARM3_RI,
	TAG_CUSTOM_ARM_RI,
	TAG_CUSTOM_ARM2_RI,
	TAG_CUSTOM_ARM3_RI,
	TAG_CUSTOM_DICK,
	TAG_CUSTOM_DICK2,

	// Size of ePlayerTags enum... always last
	TAG_SIZE
};

#define __MAXTARGETBONES__ TAG_SIZE

struct CachedTag {
	PCHAR pName;
	USHORT uNum; // The cached index of the tag, cached each game
	BOOL bCheckVis;
	BOOL bCheckWalls;
	SHORT sPriority;

	CachedTag(PCHAR pName, BOOL bCheckVis, BOOL bCheckWalls, SHORT sPriority) {
		this->pName = pName;
		this->bCheckVis = bCheckVis;
		this->bCheckWalls = bCheckWalls;
		this->sPriority = sPriority;
		this->uNum = 0;
	}
};

struct CachedTags {
	vector<CachedTag> List;
};

enum eEntityType {
	ETYPE_ENTITY,
	ETYPE_ACTOR,
	ETYPE_PLAYER,

	// Size of the ETYPE enum, always last
	ETYPE_SIZE
};

class TagInfo {
public:
	vec3_t vPos;
	vec2_t vScreenPos;
	FLOAT fDamageMult;
	bool bVisible;
	bool bWallbangable;
	bool bBlockedByShield;
	BYTE bTagOffset;

	TagInfo() {
		vPos.Set(0, 0, 0);
		bVisible = false;
		bWallbangable = false;
		bBlockedByShield = false;
		fDamageMult = 0.0f;
		bTagOffset = 0;
	}
};

class ExtendedEntity {
public:
	DWORD pXModel;
	DWORD pDObj;
	DWORD pNetInfo;
	TagInfo cTags[TAG_SIZE];
	ShieldInfo oShieldInfo;
	GeoIpInfo cGeoIPInfo;

	bool bGrabGeoIPInfo;
	bool bAiming;
	bool bFiring;
	bool bPreferred;
	bool bPrimaryShield;
	bool bSecondaryShield;
	bool bVisible;
	bool bWallbangable;
	bool bWeaponDualWield;
	bool bWhitelisted;
	bool bTopPlayer; // True if the client is in the top 3
	bool bHasKilledMe;
	bool bEvaluatingHacker;
	bool bUsingSpinbot;
	bool bInvertTeam;
	bool bThreadRunning;
	bool bSkipNextFrame;

	PCHAR pName;
	PCHAR pRealName;
	QWORD cXuid;
	IN_ADDR cIP;

	DWORD dwCurrentWeapon;
	DWORD dwSecondaryWeapon;
	DWORD dwStance;
	DWORD dwLastAbnormalAngle;
	DWORD dwHackerCheckStartTime;

	FLOAT fDelta, fOldDelta;

	SHORT sCurrentStreak; // The amount of kills the player has gotten since the last death
	SHORT sCurrentSpree; // The amount of kills the player has gotten within 5 seconds of the last, back to back
	SHORT sVisibleBone;
	SHORT sWallbangableBone;
	SHORT sLookingAtBoneIndex;

	SHORT sWeaponClipSize;
	SHORT sAmmoRightHand, sAmmoLeftHand;

	DWORD dwLastKillTime, dwSpreeEndTime;
	sScoreboardInfo cScore;
	int iThreatAssessment;
	int iPriority;

	vec3_t vMins;
	vec3_t vMaxes;

	PCHAR GetName();

	ExtendedEntity() {
		bInvertTeam = false;
		iPriority = 0;
	}
};

class Entity {
public:
	SHORT sIndex;

	DWORD pCentity;
	DWORD pClientInfo;
	//DWORD pDObj;
	SHORT sEntType;
	vec3_t vLocation;
	vec2_t vScreen;
	vec3_t vAngles;
	vec3_t vOldAngles;
	
	SHORT sTeam;

	bool bValid;
	bool bAlive;
	bool bPlayer;
	bool bAI;
	bool bEnemy;
	bool bKillable;
	bool bInvisible;
	bool bOnScreen;

	FLOAT fDistance;

	ExtendedEntity *pExt;

	VOID ResetExtention(BOOL bFull);

	VOID Reset(BOOL bFull = FALSE, SHORT sIndex = 0);
	bool AllocExtendedEntity();

	VOID FreeExtendedEntity();

	Entity(SHORT sIndex) {
		Reset(TRUE, sIndex);
	}

	Entity() {
		Reset(TRUE);
		pExt = NULL;
	}
};

class Entities {
public:
	Entity List[__MAXENTITIES__];
	VOID Reset() {
		for (DWORD i = 0; i < __MAXENTITIES__; i++) {
			List[i].Reset(TRUE, i);
		}
		DbgPrint("Entity reset success!");
	}
	Entities() {
		Reset();
	}
};