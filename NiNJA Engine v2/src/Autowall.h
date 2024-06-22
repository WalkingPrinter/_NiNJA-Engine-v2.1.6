#pragma once
#include "Globals.h"

namespace Autowall {
#if defined(AW)
	class BulletFireParams {
	public:
		short worldEntNum; //0x0000
		short skipNum; //0x0002
		float power; //0x0004
		int bulletType; //0x0008
		int unknown; //0x000c
		vec3_t vieworg; //0x0010
		vec3_t start; //0x001C
		vec3_t end; //0x0028
		vec3_t viewDirection; //0x0034
	};//Size=0x0040
#elif defined(MW2)
	class trace_t {
	public:
		float fraction; //0x0
		float normal[3]; //0x4
		int surfaceFlags; //0x10
		int contents; //0x14
		const char *material; //0x18
		int hitType; //0x1c
		unsigned __int16 hitId; //0x20
		unsigned __int16 modelIndex; //0x22
		unsigned __int16 partName; //0x24
		unsigned __int16 partGroup; //0x26
		bool allsolid; //0x28
		bool startsolid; //0x29
		bool walkable; //0x30
	};
	class TraceExtents {
	public:
		vec3_t midPoint;
		DWORD midPointPad;
		vec3_t halfDelta;
		DWORD halfDeltaPad;
		vec3_t halfDeltaAbs;
		DWORD halfDeltaAbsPad;
		vec3_t invDeltaAbs;
		DWORD invDeltaAbsPad;
		vec3_t start;
		DWORD startPad;
		vec3_t end;
		DWORD endPad;
	};
	class pointtrace_t {
	public:
		TraceExtents extents;
		DWORD ignoreEntParams;
		DWORD contentmask;
		DWORD bLocational;
		DWORD bCalcDObjPose;
		QWORD priorityMap;
	};
	class BulletFireParams {
	public:
		__int32 worldEntNum; //0x0000
		__int32 skipNum; //0x0004
		float power; //0x0008
		__int32 bulletType; //0x000C
		vec3_t vieworg; //0x0010
		vec3_t start; //0x001C
		vec3_t end; //0x0028
		vec3_t viewDirection; //0x0034
	};//Size=0x0040=
#elif defined(MW3) || defined(GHOSTS)
	struct BulletFireParams {
		int weaponEntIndex;
		int ignoreEntIndex;
		float damageMultiplier;
		int methodOfDeath;
		int noRicochet;
		vec3_t origStart;
		vec3_t start;
		vec3_t end;
		vec3_t dir;
	};
#elif defined(BO3)
	struct BulletFireParams {
		int weaponEntIndex;
		int ignoreEntIndex;
		float damageMultiplier;
		int methodOfDeath;
		vec3_t origStart;
		vec3_t start;
		vec3_t end;
		vec3_t dir;
	};
#elif defined(BO1) || defined(BO2) || defined(MW1) || defined(WAW)

#if !defined(BO2)
#pragma pack(push, 1)
	struct trace_t {
		vec3_t normal;//0x0
		char unkInt[0x04];//0xc
		float Fraction;//0x10
		char unk2[0x08];//0x14
		int hitType;//0x1c
		short hitId;//0x20
		short modelIndex;//0x22
		short partName;//0x24
		short partGroup;//0x26
		char unk4[0x10];//0x28
	}; // 0x38

	struct BulletTraceResults {
		trace_t trace;//0x0
		byte unk[8];//0x34
		int hitEnt; //0x38
		float hitPos[3];//0x3c
		int ignoreHitEnt;//0x48
		int surfaceType;//0x4c
	}; // 0x50

	struct BulletFireParams {
		int weaponEntIndex;//0x0
		int ignoreEntIndex;//0x4
		float damageMultiplier;//0x8
		int bulletType;//0xc
		vec3_t origStart;//0x10
		vec3_t start;//0x1c
		vec3_t end;//0x28
		vec3_t dir;//0x34
	}; // 0x40
#endif
#endif

#if defined(BO2)
	struct trace_t {
		vec3 normal; // 0x0
		char padding[4]; // 0xC
		float fraction; // 0x10
		int surfaceType; // 0x14
		int surfaceFlags; // 0x18
		int hitType; // 0x1C
		short hitId; // 0x20
		short modelIndex; // 0x22
		short boneIndex; // 0x24
		short partName; // 0x26
		short partGroup; // 0x28
		bool allsolid; // 0x2A
		bool startsolid; // 0x2B
		bool walkable; // 0x2C
		int hitPartition; // 0x30
	};

	struct BulletTraceResults {
		trace_t trace; // 0x0
		char padding[0xC]; // 0x30
		int hitEnt; // 0x40
		vec3 hitPos; // 0x44
		bool ignoreHitEnt; // 0x50
		int surfaceType; // 0x54
		char padding2[8]; // 0x58
	};

	struct BulletFireParams {
		int weaponEntIndex; // 0x0
		int ignoreEntIndex; // 0x4
		float damageMultiplier; // 0x8
		DWORD methodOfDeath; // 0xC
		vec3 origStart; // 0x10
		vec3 start; // 0x1C
		vec3 end; // 0x28
		vec3 dir; // 0x34
	};

	struct weaponDef_s {
		byte padding[0x1C]; // 0x0
		int weapType; // 0x1C
		byte padding2[4]; // 0x20
		int penetrateType; // 0x24
		byte padding3[0x585]; // 0x20
		float fHipSpreadStandMin; // 0x5B0
		float fHipSpreadDuckedMin; // 0x5B4
		float fHipSpreadProneMin; // 0x5B8
		float hipSpreadStandMax; // 0x5BC
		float hipSpreadDuckedMax; // 0x5C0
		float hipSpreadProneMax; // 0x5C4
		byte padding4[0x69];
		bool rifleBullet; // 0x631
		bool armorPiercing; // 0x632
		byte padding5[0x28]; // 0x633
		DWORD fireTypeIcon; // 0x65C
		DWORD hudIcon; // 0x660
		int killIconRatio; // 0x664
		byte padding6[0xC7]; // 0x668
		bool bBulletImpactExplode; // 0x72F
	};

	struct centity_s {
		char padding[3]; // 0x0
		char type; //0x3
		char padding2[40]; //0x4
		vec3 origin; // 0x2C
		vec3 angles; // 0x38
		char padding3[268]; // 0x38
		int isAlive2; // 0x150
		int flags; // 0x154
		char padding4[84]; // 0x158
		int teamDog; // 0x1AC
		int weaponId; // 0x1B0
		int secondaryWeaponId; // 0x1B4
		char padding5[0x18]; // 0x1B4
		int handle; // 0x1D0
		char padding6[0x74]; // 0x1D4
		short teamRCXD_Dog_Heli; // 0x248 team for RCXD and Dog
		short teamTurret; // 0x24A Team for Turret and Heli
		char padding7[92]; // 0x24C
		short eType; // 0x2A8
		char padding8[13]; // 0x2AA
		char weaponNum; // 0x2B7
		char padding9[0xB]; // 0x2B8
		char clientNum; // 0x2C3
		char padding10[168]; // 0x2B8
		char alive; // 0x36C
		char padding11[4]; // 0x36D
	};

	struct playerState_s {
		int commandTime; // 0x0
		int pm_type; // 0x4
		int bobCycle; // 0x8
		int pm_flags; // 0xC
		long long weapFlags; // 0x10
		int otherFlags; // 0x18
		int pm_time; // 0x1C
		unsigned int loopSoundId; // 0x20
		int loopSoundFade; // 0x24
		vec3 origin; // 0x28
		vec3 velocity; // 0x34
		int remoteEyesEnt; // 0x40
		int remoteEyesTagname; // 0x44
		int remoteControlEnt; // 0x48
		int weaponTime; // 0x4C
		int weaponDelay; // 0x50
		int weaponTimeLeft; // 0x54
		int weaponDelayLeft; // 0x58
		int weaponIdleTime; // 0x5C
		int grenadeTimeLeft; // 0x60
		int throwBackGrenadeOwner; // 0x64
		int throwBackGrenadeTimeLeft; // 0x68
		int weaponRestrictKickTime; // 0x6C
		char padding[0x1C]; // 0x80
		int gravity; // 0x8C
		float leanf; // 0x90
		int speed; // 0x94
		vec3 deltaAngles; // 0x98
		int groundEntityNum; // 0xA4
		int moverEntityNum; // 0xA8
		int moverTimestamp; // 0xAC
		int groundType; // 0xB0
		vec3 vLadderVec; // 0xB4
		int jumpTime; // 0xC0
		float jumpOriginZ; // 0xC4
		byte padding2[0x34]; // 0xC8
		int eFlags; // 0xFC
		byte padding3[0xA0]; // 0xFF
		int clientNum; // 0x1A0
		int offHandWeaponIndex; // 0x1A4
		byte padding4[0x10]; // 0x1A8
		int weaponId; // 0x1B8
		byte padding5[0x24]; // 0x1BC
		float weaponPosFrac; // 0x1E0
		int adsDelayTime; // 0x1E4
		int spreadOverride; // 0x1E8
		int spreadOverrideState; // 0x1EC
		float weaponSpinLerp; // 0x1F0
		int viewmodelIndex; // 0x1F4
		vec3 viewAngles; // 0x1F8
		int viewHeightTarget; // 0x204
		float viewHeightCurrent; // 0x208
		int viewHeightLerpTime; // 0x20C
		int viewHeightLerpTarget; // 0x210
		int viewHeightLerpDown; // 0x214
		float viewAngleClampBase[2]; // 0x218
		float viewAngleClampRange[2]; // 0x220
		int damageEvent; // 0x228
		int damageYaw; // 0x22C
		int damagePitch; // 0x230
		int damageCount; // 0x234
		int health; // 0x238
		byte padding6[0x4]; // 0x23C
		int maxHealth; // 0x240
		byte padding7[0x1F4]; // 0x244
		int secondaryAmmoCount; // 0x438
		int primaryAmmoCount; // 0x43C
		byte padding8[0x24]; // 0x440
		float proneDirection; // 0x464
		byte padding9[0x84]; // 0x440
		int chargeShotLevel; // 0x4EC
		byte padding10[0x98]; // 0x4F0
		float aimSpreadScale; // 0x588
	};

	struct cg_s {
		int clientNum; // 0x0
		int localClientNum; // 0x4
		byte padding6[0x48084]; // 0x8
		int time; // 0x4808C
		byte padding[0x14];
		playerState_s ps; // 0x480A8
		byte padding7[0x5248]; // 0x48638
		//refdef_s refdef; // 0x4D880
		//byte padding8[0x1C1A4]; // 0x4D8EC
		//clientInfo_s clientInfo[18]; // 0x69A90
		//char padding9[0xE060]; // 0x72B20
		//float aimSpreadScale; //0x80B80
	};


#endif

	DWORD Autowall360Fix(bool apply, DWORD dwPatch = NULL);
	bool IsEntWallbangable(DWORD dwClient);
	VOID Initialize();
};

