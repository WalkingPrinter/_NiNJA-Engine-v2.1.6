#pragma once
#include "Globals.h"
#include "GameFuncs.h"
#include "UserCmd.h"

namespace NoSpread {
#if defined(WAW) || defined(BO1)
	static VOID RandomBulletDirOld(int *seed, FLOAT *fX, FLOAT *fY) {
		FLOAT f1 = ((FLOAT(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_GoodRandomFloat))(seed) * 360.0f;
		FLOAT f2 = ((FLOAT(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_GoodRandomFloat))(seed);
		FLOAT f28 = (FLOAT)(f1 * 0.017453292);
		*fX = cos(f28) * f2;
		*fY = sin(f28) * f2;
	}
#elif defined(BO2)
	static VOID RandomBulletDirBo2(int *seed, FLOAT *x, FLOAT *y) {
		FLOAT f1 = Radians(((FLOAT(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_GoodRandomFloat))(seed) * 360.0f);
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.AdjustSeed))(seed);
		FLOAT f2 = ((FLOAT(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_GoodRandomFloat))(seed);
		*x = cos(f1) * f2;
		*y = sin(f1) * f2;
	}
#elif defined(AW) || defined(GHOSTS)
	static VOID RandomBulletDirNew(int *seed, FLOAT *fX, FLOAT *fY) {
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.RandomBulletDir))(seed, 0.0f, 360.0f, fX, fY);
		FLOAT HAX = ((FLOAT(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_GoodRandomFloat))(seed);
		*fX *= HAX;
		*fY *= HAX;
	}
#elif defined(BO3)
	static DOUBLE BG_random_BO3(int *seed) {
		int ye = 0x343FD * *seed + 0x269EC3;
		*seed = ye;
		return (DOUBLE)(ye >> 17) * 0.000030517578;
	}
	static VOID RandomBulletDir_BO3(int *randSeed, FLOAT *fX, FLOAT *fY) {
		FLOAT theta = (BG_random_BO3(randSeed) * 360.0f) * 0.01745329251;
		FLOAT radius = BG_random_BO3(randSeed);
		*fX = radius * cos(theta);
		*fY = radius * sin(theta);
	}
#endif

	static VOID RandomBulletDir(int *seed, FLOAT *spreadX, FLOAT *spreadY) {
	#if defined(WAW) || defined(BO1)
		RandomBulletDirOld(seed, spreadX, spreadY);
	#elif defined(BO2)
		RandomBulletDirBo2(seed, spreadX, spreadY); //bo2
	#elif defined(MW1)
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.RandomBulletDir))(*seed, spreadX, spreadY);
	#elif defined(MW2) || defined(MW3)
		((VOID(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.RandomBulletDir))(seed, spreadX, spreadY);
	#elif defined(GHOSTS) || defined(AW)
		RandomBulletDirNew(seed, spreadX, spreadY);
	#elif defined(BO3)
		RandomBulletDir_BO3(seed, spreadX, spreadY);
	#endif
	}

	//struct SpreadData {
	//	int iTime;
	//	float fValue;
	//	SpreadData(int time, float value) {
	//		iTime = time;
	//		fValue = value;
	//	}
	//};

	// Store thread multipliers for the last 10 frames, so we can get our spread multiplier for a given server time :)
	//static vector<SpreadData> vSpreadMultipliers;
	static FLOAT GetSpreadMultiplier(int iTime) {
		DWORD cg_t = GameFuncs::Get_CG_t();
		float ret = 0.0f;
		if (cg_t) {
		#if defined(WAW)
			ret = *(float*)(cg_t + 0xECF5C);//0xfd4);
		#elif defined(MW1)
			ret = *(float*)(cg_t + 0xF09DC);
		#elif defined(MW2)
			ret = *(float*)(cg_t + 0xFD898);// 0x36c4);//0x2c4);
		#elif defined(MW3)
			ret = *(float*)(cg_t + 0x999EC);
		#elif defined(BO1)
			ret = *(float*)(cg_t + 0x69264);
		#elif defined(BO2)
			ret = *(float*)(cg_t + 0x80B80);//0x618);
		#elif defined(GHOSTS)
			ret = *(float*)(cg_t + 0xC77E0);//0x428);
		#elif defined(AW)
			ret = *(float*)(cg_t + 0xBDB80);//0x5b8);
		#elif defined(BO3)
			ret = *(float*)(cg_t + 0x675A0); //tu3 0x669A0);//0x998);
		#endif
		}
		/*if (ret != 0) {
			vSpreadMultipliers.push_back(SpreadData(*(int*)UserCmd::GetCurrentUserCmd(), ret));
			if (vSpreadMultipliers.size() > 10) {
				vSpreadMultipliers.erase(vSpreadMultipliers.begin());
			}
		}
		if (vSpreadMultipliers.size() > 0 && iTime) {
			for (int i = 0; i < vSpreadMultipliers.size(); i++) {
				if (vSpreadMultipliers[i].iTime == iTime) {
					return vSpreadMultipliers[i].fValue;
				}
			}
		}*/
		return ret;
	}

#if defined(GHOSTS) || defined(AW)
	static int iTransformSeed(int * seed) {
		*seed = 0x343FD * (0x343FD * (0x343FD * (0x343FD * *seed + 0x269EC3) + 0x269EC3) + 0x269EC3) + 0x269EC3;
		return (int)seed;
	}
#elif defined(BO3)
	static int calcSeedBo3(int *time) {
		int iSeed = *time, start;
		start = 0;
		__asm {
			clrlwi    start, iSeed, 24
		}
		for (int i = 0; i<3; i++) {
			__asm {
				slwi      start, start, 8
				xor iSeed, start, iSeed
			}
		}
		*time = iSeed;
		return (int)time;
	}
#endif 
	static int TransformSeed(int *seed) {
	#if defined(BO2) || defined(MW2) || defined(MW3)
		return ((int(*)(...))decryptDWORD(Not(NiNJA.cPatches)->CoD.TransformSeed))(seed);
	#elif defined(GHOSTS) || defined(AW)
		return iTransformSeed(seed);
	#elif defined(BO3)
		return calcSeedBo3(seed);
	#endif
		return (int)seed;
	}

	static VOID Apply() {
		int cmdNum = UserCmd::GetCurrentUserCmdNum();
		int cmd = UserCmd::GetCurrentUserCmd();
		int oldCmd = UserCmd::GetOldUserCmd();
		int cmdToMod = oldCmd;
		if (CG::bNoSpread && (CG::cButtons.bFiring || CG::cButtons.bFiringSecondary))  {
			FLOAT zoomState = GameFuncs::GetZoomState();
			if (zoomState < 0.95f) {
				FLOAT minSpread, maxSpread, flSpreadX, flSpreadY;
				GameFuncs::GetSpreadForWeapon(CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon, &minSpread, &maxSpread);

				// Grab the time to be used as the seed, and modify it if required for our left gun
				int fSeed = UserCmd::GetNospreadSeed(cmdNum);
				if (CG::cButtons.bFiringSecondary) {
					fSeed += __LEFTGUNADDITAVE__;
				}
				// Transform the seed if required, so we can use it to predict the correct bullet direction
				TransformSeed(&fSeed);

				// Get the direction of the spread
				RandomBulletDir(&fSeed, &flSpreadX, &flSpreadY);

				// Then use it to determine the final spread value to apply to our pitch and yaw angles
				FLOAT flSpread = (maxSpread - minSpread) * (GetSpreadMultiplier(0) * 0.00392156862) + minSpread;
				int viewAngles = UserCmd::GetUserCmdViewAngles(oldCmd);
				*(int*)(viewAngles) -= AngleToShort(-(flSpreadY * flSpread));
				*(int*)(viewAngles + 0x4) -= AngleToShort(-(flSpreadX * flSpread));
			}
		}
	}
};

