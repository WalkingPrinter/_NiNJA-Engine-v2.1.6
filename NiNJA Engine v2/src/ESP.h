#pragma once
#include "Globals.h"

struct CRectDef {
	float x; //0x0
	float y; //0x4
	float w; //0x8
	float h; //0xC
	DWORD horizAlign; //0x10
	DWORD vertAlign;
	CRectDef() {
		x = 0.0f;
		y = 0.0f;
		w = 0.0f;
		h = 0.0f;
		horizAlign = 0;
		vertAlign = 0;
	}
	CRectDef(float xIn, float yIn, float wIn, float hIn, DWORD horizAlignIn, DWORD vertAlignIn = 0) {
		x = xIn;
		y = yIn;
		w = wIn;
		h = hIn;
		horizAlign = horizAlignIn;
		vertAlign = vertAlignIn;
	}
};

namespace ESP {
	extern CRectDef lastCompass;
	extern DWORD lastCompassType;
	extern DWORD drawCompassTimer;
	extern bool bAnyEnemyVisible;
	extern bool bAnyEnemyWallbangable;
	extern FLOAT fTopLeftOffsetX, fTopLeftOffsetY;

	VOID Draw();
	VOID DrawBoneESP(DWORD dwClient);
	VOID DrawNameESP(DWORD dwClient, vec2_t vTopPos, vec2_t vCenterPos, vec2_t vBottomPos);
	VOID DrawDistanceESP(DWORD dwClient, vec2_t vTopPos, vec2_t vBottomPos);
	DWORD GetKillIconType(DWORD dwWeaponIndex);
	DWORD GetWeaponKillIcon(DWORD dwWeaponIndex, FLOAT * fAspectW, FLOAT * fAspectH);
	VOID MakeLines(vec3_t Origin, FLOAT X1, FLOAT Y1, FLOAT Z1, FLOAT X2, FLOAT Y2, FLOAT Z2, FLOAT * Color, int Size);
	VOID DrawCornerBox(DWORD dwClient, vec2_t vBottomPos, vec2_t vCenterPos, vec2_t vTopPos);
	VOID Draw2DBox(DWORD dwClient, vec2_t vBottomPos, vec2_t vCenterPos, vec2_t vTopPos, BOOL bFill);
	VOID Draw3DBox(DWORD dwClient, BOOL bOverride = FALSE, vec3_t vMins = vec3_t(), vec3_t vMaxes = vec3_t());
	VOID DrawPyramidESP(DWORD dwClient);
	VOID DrawWeaponESP(DWORD dwClient, vec2_t vTopPos, vec2_t vBottomPos);
	VOID DrawDropESP(DWORD dwClient);
#if defined(AW)
	VOID SetupPointersAW(BOOL bSetup);
#endif
	VOID DrawDirectionalPointer(DWORD dwClient);
	VOID DrawSnapline(DWORD dwClient, vec2_t vBottomPos, vec2_t vTopPos);
	VOID DrawCustomCrosshair();
	FLOAT ScaleRadarX(FLOAT X, FLOAT ScreenWidth);
	FLOAT ScaleRadarY(FLOAT Y, FLOAT ScreenHeight);
	VOID GetRadarXY(FLOAT * x, FLOAT * y);
	VOID GetRectDef(FLOAT radarX, FLOAT radarY, CRectDef * rectDef);
	DWORD GetCGForCompass();
	VOID CompassUpYawVector(vec2_t & Yaw);
	VOID CalcCompassDimensions(DWORD CompassType, vec2_t & size, DWORD rectDef, vec4_t & radarSize);
	BOOL WorldPosToCompass(DWORD CompassType, vec4_t & RadarSize, vec2_t & Yaw, vec3_t * refOrigin, vec3_t * worldPos, vec2_t & outClipped);
	VOID WorldToCompass(vec3_t * World, vec2_t * Compass);
	VOID DrawRadarPic(DWORD dwCompassType, FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT rotation, const FLOAT * color, DWORD shader);
	VOID DrawRadarText(DWORD dwRadarType, PCHAR pText, FLOAT x, FLOAT y, FLOAT fScale, FLOAT rotation, const FLOAT * color, DWORD font);
	VOID DrawCustomRadar();
	VOID DrawPing();
	bool IsEntValid(DWORD dwEnt);
	VOID DrawClientESP(int iClient);
	FLOAT *GetESPColor(DWORD dwClient);
	VOID DrawDebugMessage(int iIndex, PCHAR pMsg);
	VOID DrawInformationOverlay();
};

