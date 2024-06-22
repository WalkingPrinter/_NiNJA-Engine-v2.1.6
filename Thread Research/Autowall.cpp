#include "stdafx.h"
#include "Autowall.h"
#include "Game.h"

float Bullet_FirePenetrate(BulletFireParams* bp, BulletTraceResults* br, weaponDef_s* def, centity_s* attacker) {
	bool hasBulletPenetration = (cg->ps.perks[0] & 32) != 0;

	float totalDepth = 0.0f;

	vec3 savedHitPos = vec3();

	BulletFireParams backWallBp;
	BulletTraceResults backWallBr;

	float bulletMultiplier = perk_bulletPenetrationMultiplier->current.value;
	float weaponMultiplier = BG_GetWeaponVarientDef(attacker->weaponId)->penetrationMultiplier;

	if (!BulletTrace(0, bp, def, br, 0))
		return bp->damageMultiplier;

	for (int i = 0; i < 5; i++) {
		float frontWallDepth = BG_GetSurfacePenetrationDepth(def, br->surfaceType) * weaponMultiplier;

		if (hasBulletPenetration)
			frontWallDepth *= bulletMultiplier;

		if (frontWallDepth <= 0.0f)
			break;

		savedHitPos = br->hitPos;

		if (!BG_AdvanceTrace(bp, br, 0.13500001f))
			break;

		bool frontWallTrace = BulletTrace(0, bp, def, br, br->surfaceType);

		backWallBp = *bp;
		backWallBr = *br;
		backWallBp.dir = bp->dir.negate();
		backWallBp.start = bp->end;
		backWallBp.end = (backWallBp.dir * 0.0099999998f) + savedHitPos;
		backWallBr.trace.normal = backWallBr.trace.normal.negate();

		if (frontWallTrace)
			BG_AdvanceTrace(&backWallBp, &backWallBr, 0.0099999998f);

		bool backTrace = BulletTrace(0, &backWallBp, def, &backWallBr, backWallBr.surfaceType);
		bool solidWall = (backTrace) && backWallBr.trace.allsolid || br->trace.startsolid && backWallBr.trace.startsolid;

		if (solidWall || backTrace) {
			float totalDepth = 0.0f;

			if (solidWall)
				totalDepth = (backWallBp.end - backWallBp.start).getLength();
			else
				totalDepth = (backWallBr.hitPos - savedHitPos).getLength();

			float backWallDepth = BG_GetSurfacePenetrationDepth(def, backWallBr.surfaceType);

			if (solidWall) {
				if (hasBulletPenetration)
					backWallDepth *= bulletMultiplier;
	
				frontWallDepth = min(frontWallDepth, backWallDepth) * weaponMultiplier;

				if (frontWallDepth <= 0.0f)
					break;
			}

			bp->damageMultiplier -= totalDepth / frontWallDepth;

			if (bp->damageMultiplier <= 0.0f)
				break;
		}

		else if (!frontWallTrace)
			break;

		if (solidWall)
			continue;
	}

	/*for (int i = 0; i < 5; i++) {
		float frontDepth = BG_GetSurfacePenetrationDepth(def, br->surfaceType) * weaponMultiplier;

		if (hasStoppingPower)
			frontDepth *= bulletMultiplier;

		if (frontDepth <= 0.0f)
			break;

		savedHitPos = br->hitPos;

		if (!BG_AdvanceTrace(bp, br, 0.13500001f))
			break;

		bool frontTrace = BulletTrace(0, bp, def, br, br->surfaceType);

		backWallBp = *bp;
		backWallBr = *br;
		backWallBp.dir = bp->dir.negate();
		backWallBp.start = bp->end;
		backWallBp.end = (backWallBp.dir * 0.0099999998f) + savedHitPos;
		backWallBr.trace.normal = backWallBr.trace.normal.negate();

		if (frontTrace)
			BG_AdvanceTrace(&backWallBp, &backWallBr, 0.0099999998f);

		bool backTrace = BulletTrace(0, &backWallBp, def, &backWallBr, backWallBr.surfaceType);
		bool solidWall = (backTrace) && backWallBr.trace.allsolid || br->trace.startsolid && backWallBr.trace.startsolid;

		if (solidWall || backTrace) {
			float totalDepth = 0.0f;

			if (solidWall)
				totalDepth = (backWallBp.end - backWallBp.start).getLength();
			else
				totalDepth = (backWallBr.hitPos - savedHitPos).getLength();

			if (totalDepth < 1.0f)
				totalDepth = 1.0f;

			float backDepth = BG_GetSurfacePenetrationDepth(def, backWallBr.surfaceType);

			if (backTrace) {
				if (hasStoppingPower)
					backDepth *= weaponMultiplier;

				frontDepth = min(frontDepth, backDepth);

				if (frontDepth <= 0.0f)
					break;
			}

			bp->damageMultiplier -= totalDepth / frontDepth;

			if (bp->damageMultiplier <= 0.0f)
				break;

			if (solidWall || frontTrace)
				continue;
		}

		if (!backTrace)
			return bp->damageMultiplier;
	}*/

	return 0.0f;
}
