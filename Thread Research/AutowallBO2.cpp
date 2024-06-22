#include "stdafx.h"
#include "Autowall.h"
#include "Game.h"

// METHOD OF DEATH IS USELESS LISTEN TO ME. IT'S ONLY USED FOR DEATH ANIMATIONS
// Also, worldEntIndex isn't needed either.

/*
bool rageBot::clientWallbangable(int clientNum, short boneId) {
	if (!toggle.autoWall)
		return false;

	BulletFireParams bp;
	BulletTraceResults br;

	weaponDef_s* def = BG_GetWeaponDef(cg->ps.weaponId);

	vec3 end = vec3();

	if (!getTagPos(&centity[clientNum], boneId, end))
		return false;

	bp.ignoreEntIndex = cg->ps.clientNum;
	bp.damageMultiplier = 1.0f;
	bp.origStart = weaponFireEyeOrigin;
	bp.start = bp.origStart;
	bp.end = end;
	bp.dir = anglesToForward((end - bp.start).convertToAngles());

	float multiplier = Bullet_FirePenetrate(&bp, &br, def, &centity[cg->ps.clientNum]);
	return bp.ignoreEntIndex == clientNum;
}
*/

float Bullet_FirePenetrate(BulletFireParams* bp, BulletTraceResults* br, weaponDef_s* def, centity_s* attacker) {
	bool hasFMJ = BG_WeaponHasPerk(cg->ps.weaponId, 6);

	int penetrationType = def->penetrateType;

	float bulletMultiplier = perk_bulletPenetrationMultiplier->current.value;

	vec3 savedHitPos = vec3();

	BulletFireParams backWallBp;
	BulletTraceResults backWallBr;

	if (!BulletTrace(0, bp, attacker, br, 0, false))
		return bp->damageMultiplier;

	if (penetrationType <= 0)
		return bp->damageMultiplier;

	if ((Trace_GetEntityHitId(&br->trace) & 0xFFFF) == bp->ignoreEntIndex)
		return 0.0f;

	for (int i = 0; i < 5; i++) {
		float frontDepth = BG_GetSurfacePenetrationDepth(penetrationType, br->surfaceType);

		if (br->trace.surfaceType & 4)
			frontDepth = 100.0f;

		if (hasFMJ)
			frontDepth *= bulletMultiplier;

		if (frontDepth <= 0.0f)
			break;

		savedHitPos = br->hitPos;

		if (!BG_AdvanceTrace(bp, br, 0.13500001f))
			break;

		bool frontTrace = BulletTrace(0, bp, attacker, br, br->surfaceType, false);

		backWallBp = *bp;
		backWallBr = *br;
		backWallBp.dir = bp->dir.negate();
		backWallBp.start = bp->end;
		backWallBp.end = (backWallBp.dir * 0.0099999998f) + savedHitPos;
		backWallBr.trace.normal = backWallBr.trace.normal.negate();

		if (frontTrace)
			BG_AdvanceTrace(&backWallBp, &backWallBr, 0.0099999998f);

		bool backTrace = BulletTrace(0, &backWallBp, attacker, &backWallBr, backWallBr.surfaceType, false);
		bool solidWall = (backTrace) && backWallBr.trace.allsolid || br->trace.startsolid && backWallBr.trace.startsolid;

		if (backTrace || solidWall) {
			float totalDepth = 0.0f;

			if (solidWall)
				totalDepth = (backWallBp.end - backWallBp.start).getLength();
			else
				totalDepth = (backWallBr.hitPos - savedHitPos).getLength();

			if (totalDepth < 1.0f)
				totalDepth = 1.0f;

			if (backTrace) {
				float backDepth = BG_GetSurfacePenetrationDepth(penetrationType, backWallBr.surfaceType);

				if (hasFMJ)
					backDepth *= bulletMultiplier;

				frontDepth = min(frontDepth, backDepth);

				if (backWallBr.trace.surfaceType & 4)
					frontDepth = 100.0f;

				else if (frontDepth <= 0.0f)
					break;
			}

			bp->damageMultiplier -= (totalDepth / frontDepth);

			if (bp->damageMultiplier <= 0.0f)
				break;

			if (solidWall)
				continue;
		}

		else if (!frontTrace)
			break;
	}

	return 0.0f;
}
