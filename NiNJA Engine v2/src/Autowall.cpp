#include "Autowall.h"
#include "GameFuncs.h"

namespace Autowall {
	BulletFireParams GetBFP(vec3_t *start, vec3_t *end) {
		BulletFireParams BFP;
		vec3_t dir, angles;
		dir.Set(end->x - start->x, end->y - start->y, end->z - start->z);
		angles = vectoangles(dir.x, dir.y, dir.z);

		ZeroMemory(&BFP, sizeof(BulletFireParams));

#if defined(MW2)
		BFP.worldEntNum = __WORLDENTNUM__;
		BFP.skipNum = CG::iLocalClient;
		BFP.power = 1.0f;
		BFP.bulletType = GameFuncs::GetBulletType(CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon);
		BFP.vieworg = *start;
		BFP.start = *start;
		BFP.end = *end;
		AngleVectors(&angles, &BFP.viewDirection, NULL, NULL);
#elif defined(AW)
		BFP.worldEntNum = __WORLDENTNUM__;
		BFP.skipNum = CG::iLocalClient;
		BFP.power = 1.0f;
		BFP.bulletType = GameFuncs::GetBulletType(CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon);
		BFP.vieworg = *start;
		BFP.start = *start;
		BFP.end = *end;
		AngleVectors(&angles, &BFP.viewDirection, NULL, NULL);
#elif defined(BO3)
		BFP.weaponEntIndex = __WORLDENTNUM__;
		BFP.ignoreEntIndex = CG::iLocalClient;
		BFP.damageMultiplier = 1.0f;
		BFP.methodOfDeath = GameFuncs::GetBulletType(CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon);
		BFP.origStart = *start;
		BFP.start = *start;
		BFP.end = *end;
		AngleVectors(&angles, &BFP.dir, NULL, NULL);
#elif defined(MW3) || defined(GHOSTS)
		BFP.weaponEntIndex = __WORLDENTNUM__;
		BFP.ignoreEntIndex = CG::iLocalClient;
		BFP.damageMultiplier = 1.0f;
		BFP.methodOfDeath = GameFuncs::GetBulletType(CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon);
		BFP.origStart = *start;
		BFP.start = *start;
		BFP.end = *end;
		AngleVectors(&angles, &BFP.dir, NULL, NULL);
#elif defined(BO2)
		BFP.weaponEntIndex = __WORLDENTNUM__;
		BFP.ignoreEntIndex = CG::iLocalClient;
		BFP.damageMultiplier = 1.0f;
		BFP.methodOfDeath = GameFuncs::GetBulletType(CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon);//1;
		BFP.origStart = *(vec3*)start;
		BFP.start = *(vec3*)start;
		BFP.end = *(vec3*)end;
		AngleVectors(&angles, (vec3_t*)&BFP.dir, NULL, NULL);
#else
		BFP.weaponEntIndex = __WORLDENTNUM__;
		BFP.ignoreEntIndex = CG::iLocalClient;
		BFP.damageMultiplier = 1.0f;
		BFP.bulletType = GameFuncs::GetBulletType(CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon);//1;
		BFP.origStart = *start;
		BFP.start = *start;
		BFP.end = *end;
		AngleVectors(&angles, &BFP.dir, NULL, NULL);
#endif
		return BFP;
	}

#if defined(BO2)
	namespace definitions {
		namespace BG_ {
			bool(*BG_WeaponHasPerk)(int weaponId, int perkId);
			bool(*BG_AdvanceTrace)(BulletFireParams* bp, BulletTraceResults* br, float dist);
			float(*BG_GetSurfacePenetrationDepth)(int type, int surfaceType);
		}

		namespace misc {
			bool(*BulletTrace)(int localClientNum, BulletFireParams* bp, centity_s* attacker, BulletTraceResults* br, int lastSurfaceType, bool simulated);

			//short(*Trace_GetEntityHitId)(trace_t* trace) = (short(*)(trace_t*))0x823F7160;
		}

		//void(*AngleVectors2)(vec3& angles, vec3& forward, vec3& right, vec3& up) = (void(*)(vec3&, vec3&, vec3&, vec3&))0x8248E408;
	}

	void Initialize() {
		definitions::misc::BulletTrace = (bool(*)(int, BulletFireParams*, centity_s*, BulletTraceResults*, int, bool))decryptDWORD(Not(NiNJA.cPatches)->CoD.BulletTrace);
		definitions::BG_::BG_GetSurfacePenetrationDepth = (float(*)(int, int))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_GetSurfacePenetrationDepth);
		definitions::BG_::BG_AdvanceTrace = (bool(*)(BulletFireParams*, BulletTraceResults*, float))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_AdvanceTrace);
		definitions::BG_::BG_WeaponHasPerk = (bool(*)(int, int))decryptDWORD(Not(NiNJA.cPatches)->CoD.BG_WeaponHasPerk);
	}

	float Bullet_FirePenetrate(DWORD dwClient, DWORD dwTag, BulletFireParams* bp, BulletTraceResults* br, weaponDef_s* def, centity_s* attacker) {
		int cg_t = GameFuncs::Get_CG_t();
		if (!cg_t) {
			return 0.0f;
		}
		cg_s *cg = (cg_s*)cg_t;
		bool hasFMJ = definitions::BG_::BG_WeaponHasPerk(cg->ps.weaponId, 6);

		int penetrationType = def->penetrateType;

		float bulletMultiplier = *(FLOAT*)GameFuncs::Dvar_GetValueAddress(0x845BC7D4);//2.0f;// perk_bulletPenetrationMultiplier->current.value;

		if (bulletMultiplier > 2.0f) {
			bulletMultiplier = 2.0f;
		}

		vec3 savedHitPos = vec3();

		BulletFireParams backWallBp;
		BulletTraceResults backWallBr;

		if (!definitions::misc::BulletTrace(0, bp, attacker, br, 0, false)) {
			return bp->damageMultiplier;
		}

		if (penetrationType <= 0) {
			return bp->damageMultiplier;
		}

		if ((GameFuncs::Trace_GetEntityHitId(&br->trace) & 0xFFFF) == bp->ignoreEntIndex) {
			return 0.0f;
		}

		for (int i = 0; i < 5; i++) {
			float frontDepth = definitions::BG_::BG_GetSurfacePenetrationDepth(penetrationType, br->surfaceType);

			if (br->trace.surfaceType & 4)
				frontDepth = 100.0f;

			if (hasFMJ)
				frontDepth *= bulletMultiplier;

			if (frontDepth <= 0.0f) {
				break;
			}

			savedHitPos = br->hitPos;

			if (!definitions::BG_::BG_AdvanceTrace(bp, br, 0.13500001f))
				break;

			bool frontTrace = definitions::misc::BulletTrace(0, bp, attacker, br, br->surfaceType, false);

			backWallBp = *bp;
			backWallBr = *br;
			backWallBp.dir = bp->dir.negate();
			backWallBp.start = bp->end;
			backWallBp.end = (backWallBp.dir * 0.0099999998f) + savedHitPos;
			backWallBr.trace.normal = backWallBr.trace.normal.negate();

			if (frontTrace)
				definitions::BG_::BG_AdvanceTrace(&backWallBp, &backWallBr, 0.0099999998f);

			bool backTrace = definitions::misc::BulletTrace(0, &backWallBp, attacker, &backWallBr, backWallBr.surfaceType, false);
			bool solidWall = (backTrace) && backWallBr.trace.allsolid || br->trace.startsolid && backWallBr.trace.startsolid;

			if (backTrace || solidWall) {
				float totalDepth = 0.0f;

				if (solidWall)
					totalDepth = (backWallBp.end - backWallBp.start).length();
				else
					totalDepth = (backWallBr.hitPos - savedHitPos).length();

				if (totalDepth < 1.0f)
					totalDepth = 1.0f;

				if (backTrace) {
					float backDepth = definitions::BG_::BG_GetSurfacePenetrationDepth(penetrationType, backWallBr.surfaceType);

					if (hasFMJ)
						backDepth *= bulletMultiplier;

					frontDepth = min(frontDepth, backDepth);

					if (backWallBr.trace.surfaceType & 4)
						frontDepth = 100.0f;
					else if (frontDepth <= 0.0f)
						break;
				}

				bp->damageMultiplier -= (totalDepth / frontDepth);

				if (bp->damageMultiplier <= 0.0f) {
					break;
				}

				if (solidWall)
					continue;
			}
			else if (!frontTrace) {
				if (i == 0) {
					if (CG::cEntities.List[dwClient].pExt->cTags[dwTag].bBlockedByShield) {
						bp->ignoreEntIndex = -1;
					}
				}
				//else {
				//	//if (i == 0) {
				//	BYTE bResult[0x100];
				//	trace_t *pResult = (trace_t*)bResult;

				//	GameFuncs::LocationalTrace(__WORLDENTNUM__, (vec3_t*)&bp->start, (vec3_t*)&bp->end, (PBYTE)&bResult);
				//	if (GameFuncs::Trace_GetEntityHitId(bResult) == dwClient) {
				//		if (pResult->partGroup == 0x14) {
				//			//CG::cEntities.List[dwClient].pExt->cTags[dwTag].bBlockedByShield = true;
				//			bp->ignoreEntIndex = -1;
				//		}
				//	}
				//}
				//}
				break;
			}
		}
		return 0.0f;
	}

	vec3 anglesToForward(vec3 angle) {
		vec3 forward = vec3();
		AngleVectors((vec3_t*)&angle, (vec3_t*)&forward, NULL, NULL);
		return forward;
	}

	bool clientWallbangable(int clientNum, short boneId, vec3_t *vPos) {

		BulletFireParams bp;
		BulletTraceResults br;

		int cg_t = GameFuncs::Get_CG_t();
		if (!cg_t) {
			return 0.0f;
		}
		cg_s *cg = (cg_s*)cg_t;

		weaponDef_s* def = (weaponDef_s*)GameFuncs::GetWeaponDef(cg->ps.weaponId);

		vec3 end = *(vec3*)(vPos);

		bp.ignoreEntIndex = cg->ps.clientNum;
		bp.damageMultiplier = 1.0f;
		bp.origStart = *(vec3*)&GameFuncs::GetLocalViewOrigin();//weaponFireEyeOrigin;
		bp.start = bp.origStart;
		bp.end = end;
		bp.dir = anglesToForward((end - bp.start).convertToAngles());

		jmp_buf jAutowallJump;
		int ret = setjmp(jAutowallJump);
		if (!ret) {
			try {
				// Call FireBulletPenetrate
				float multiplier = Bullet_FirePenetrate(clientNum, boneId, &bp, &br, def, (centity_s*)CG::cEntities.List[clientNum].pCentity);
			}
			catch (...) {
				//printf("Crash in autowall, returning!\n");
				longjmp(jAutowallJump, 420);
			}
		}
		else {
			return false;
		}

		// Check if the trace hit a shield
		if (br.trace.partGroup == 0x14) {
			return false;
		}

		return (bp.ignoreEntIndex == clientNum);
	}
#endif

	int GetEntityHitIndex(BulletFireParams *BFP) {
		int entityHitIndex = -1;
#if defined(MW2)
		entityHitIndex = BFP->skipNum;
#elif defined(AW)
		entityHitIndex = BFP->skipNum;
#elif defined(BO3)
		entityHitIndex = BFP->ignoreEntIndex;
#elif defined(MW3) || defined(GHOSTS)
		entityHitIndex = BFP->ignoreEntIndex;
#else
		entityHitIndex = BFP->ignoreEntIndex;
#endif
		return entityHitIndex;
	}

	FLOAT GetBFPPower(BulletFireParams *BFP) {
#if defined(MW2)
		return BFP->power;
#elif defined(AW)
		return BFP->power;
#elif defined(BO3)
		return BFP->damageMultiplier;
#elif defined(MW3) || defined(GHOSTS)
		return BFP->damageMultiplier;
#else
		return BFP->damageMultiplier;
#endif
	}

	DWORD Autowall360Fix(bool apply, DWORD dwPatch) {
#if defined(MW2) || defined(MW3) || defined(GHOSTS) || defined(BO2)
		DWORD dwAutowallPatch = decryptDWORD(Not(NiNJA.cPatches)->CoD.Autowall360Patch);
		if (apply) {
			DWORD patchBkup = *(DWORD*)dwAutowallPatch;
			*(DWORD*)dwAutowallPatch = 0x38600001;
			return patchBkup;
		}
		else {
			*(DWORD*)dwAutowallPatch = dwPatch;
		}
#endif
		return 0;
	}

	int IsPositionWallbangable(DWORD dwClient, vec3_t *start, BulletFireParams *BFP) {
		if (!start || !CG::cEntities.List[dwClient].pExt || !CG::cEntities.List[CG::iLocalClient].bAlive) {
			return -1;
		}

		DWORD dwHitEvent = decryptDWORD(Not(NiNJA.cPatches)->CoD.CG_BulletHitEvent), dwImpactEvent = decryptDWORD(Not(NiNJA.cPatches)->CoD.DynEntCl_DynEntImpactEvent);

		// Disable visual and audible bullet fire fx
		QWORD qwHitEventBkup = *(QWORD*)dwHitEvent;
		QWORD qwImpactEventBackup = *(QWORD*)dwImpactEvent;
		*(QWORD*)dwHitEvent = 0x386000004E800020;
		*(QWORD*)dwImpactEvent = 0x386000004E800020;


	#if defined(BO1) || defined(BO2)
		DWORD dwAutowallFix = decryptDWORD(Not(NiNJA.cPatches)->CoD.AutowallFix1), dwAutowallFix2 = decryptDWORD(Not(NiNJA.cPatches)->CoD.AutowallFix2);

		// Appply a fix that would otherwise cause us to shoot through unbangable walls	
		DWORD dwAutowallFixBkup = *(DWORD*)dwAutowallFix;
		DWORD dwAutowallFixBkup2 = *(DWORD*)dwAutowallFix2;

		// Different patch depending on the game
		DWORD dwPatch1 = 0x60000000;
	#if defined(BO2)
		DWORD tmpBkup = *(DWORD*)0x82259b40;
		dwPatch1 = 0x38600000;
	#endif

		// Apply our autowall fix patch
		*(DWORD*)dwAutowallFix = dwPatch1;
		*(DWORD*)dwAutowallFix2 = 0x60000000;
	#if defined(BO2)
		*(DWORD*)0x82259b40 = 0x60000000;
	#endif

	#endif

		// Apply a patch to fix autowall for people behind you
		DWORD dw360PatchBackup = Autowall360Fix(true);

		jmp_buf jAutowallJump;
		int ret = setjmp(jAutowallJump);
		if (!ret) {
			try {
				// Call FireBulletPenetrate
				byte bulletTraceResults[100];
				GameFuncs::BulletPenetrate(BFP, CG::cEntities.List[CG::iLocalClient].pExt->dwCurrentWeapon, CG::cEntities.List[CG::iLocalClient].pCentity, start, bulletTraceResults);
			}
			catch (...) {
				//printf("Crash in autowall, returning!\n");
				longjmp(jAutowallJump, 420);
			}
		}
		else {
			return -1;
		}

		// Remove 360 autowall patch
		Autowall360Fix(false, dw360PatchBackup);

	#if defined(BO1) || defined(BO2)
		// Remove our autowall fix patch
		*(DWORD*)dwAutowallFix = dwAutowallFixBkup;
		*(DWORD*)dwAutowallFix2 = dwAutowallFixBkup2;
	#if defined(BO2)
		*(DWORD*)0x82259b40 = tmpBkup;
	#endif
	#endif

		// Re-enable visual and audible bullet fire fx again
		*(QWORD*)dwHitEvent = qwHitEventBkup;
		*(QWORD*)dwImpactEvent = qwImpactEventBackup;

		//float power = GetBFPPower(bulletFireParams);
		//DbgPrint("Power: %f", power);

		return GetEntityHitIndex(BFP);
	}

	bool IsBoneWallbangable(DWORD dwClient, DWORD dwBoneIndex, int iTagOffset) {
		if (CG::cEntities.List[dwClient].pExt->cTags[dwBoneIndex].vPos.IsZero()) {
			return false;
		}
		vec3_t vLocalViewOrig = GameFuncs::GetLocalViewOrigin(TRUE);
		vec3_t vPos = GameFuncs::GetTagOffset(dwBoneIndex, dwClient, iTagOffset);
		if (!vLocalViewOrig.IsZero()) {
		#if !defined(BO2)
			BulletFireParams BFP = GetBFP(&vLocalViewOrig, &vPos);
			int hitIndex = IsPositionWallbangable(dwClient, &vLocalViewOrig, &BFP);
			//if (hitIndex == dwClient) {
			//	DbgPrintE("Power: %f", GetBFPPower(&BFP));
			//}
			return (hitIndex == dwClient);
		#else
			return clientWallbangable(dwClient, dwBoneIndex, &vPos);
		#endif
		}
		return false;
	}

	bool IsEntWallbangable(DWORD dwClient) {
		if (CG::cEntities.List[dwClient].bKillable && !CG::cEntities.List[dwClient].pExt->bWhitelisted) {
			if (CG::cEntities.List[dwClient].bPlayer && ((CG::bTargetBestBone && CG::cEntities.List[dwClient].pExt->bTopPlayer) || CG::cEntities.List[dwClient].pExt->bPreferred)) {
				int iTagOffset = 0;
				FLOAT fHighestDamage = -1.0f;
				bool bFound = false;
			jRetry:
				for (int i = 0; i < __MAXTARGETBONES__; i++) {
					if (!GameFuncs::CheckInGame() || NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
						return false;
					}

					if (CG::cTags.List[i].bCheckWalls) {// || (CG::cEntities.List[dwClient].pExt->bHasKilledMe && CG::cEntities.List[dwClient].pExt->bTopPlayer)) {
						CG::cEntities.List[dwClient].pExt->cTags[i].bWallbangable = IsBoneWallbangable(dwClient, i, iTagOffset);
						if (CG::cEntities.List[dwClient].pExt->cTags[i].bWallbangable && !CG::cEntities.List[dwClient].pExt->cTags[i].bBlockedByShield) {
							FLOAT fDamageMult = GameFuncs::IsAntiAimBone(dwClient, i) ? CG::cEntities.List[dwClient].pExt->cTags[i].fDamageMult + 1.0f : CG::cEntities.List[dwClient].pExt->cTags[i].fDamageMult;
							if (fDamageMult > fHighestDamage) {
								fHighestDamage = fDamageMult;
								CG::cEntities.List[dwClient].pExt->cTags[i].bTagOffset = iTagOffset;
								CG::cEntities.List[dwClient].pExt->sWallbangableBone = i;
								//printf(GameFuncs::va("tag %i offset %i is wallbangable!\n", i, iTagOffset));
								bFound = true;
							}
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
					printf(GameFuncs::va("[Autowall] %i\n", iTagOffset));
				}*/
				return bFound;
			}
			else {
				CG::cEntities.List[dwClient].pExt->cTags[CG::cEntities.List[dwClient].pExt->sVisibleBone].bTagOffset = 0;
				CG::cEntities.List[dwClient].pExt->sWallbangableBone = CG::dwAimBone;
				return IsBoneWallbangable(dwClient, CG::dwAimBone, 0) && !CG::cEntities.List[dwClient].pExt->cTags[CG::dwAimBone].bBlockedByShield;
			}
		}
		return false;
	}
}