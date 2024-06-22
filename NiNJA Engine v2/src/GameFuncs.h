#pragma once
#include <xtl.h>
#include "Utilities.h"


struct XBoneInfo {
	vec3_t bounds[2];
	vec3_t offset;
	float radiusSquared;
};

static PCHAR teamNames[] = {
	"spectator",
	"axis",
	"allies"
};

static vec3_t vOffsets[] = {
	vec3_t(0,0,0),
	vec3_t(0,0,0.5),
	vec3_t(0,0,-0.5)
};

enum eSetStance {
	SET_STANCE_STAND,
	SET_STANCE_CROUCH,
	SET_STANCE_PRONE
};


namespace GameFuncs {
	//public:
	extern CHAR pGamertagBuffer[33];

	bool AllLocalClientsDisconnected(); // Checks whether or not all local clients have disconnected from the server
	VOID CheckWhitelistBlacklist(int i);
	VOID BulletPenetrate(PVOID bulletFireParams, DWORD weaponIndex, DWORD centity_t, vec3_t * tracerStart, PVOID bulletTraceResults);
	bool BulletTrace(DWORD dwAttacker, PVOID pBulletFireParams, PVOID pBulletTraceResults, DWORD dwSurfaceType);
	DWORD CacheFont(CHAR *font); // Grabs a pointer to a font given a font name
	DWORD CacheMaterial(PCHAR pMaterialName); // Grabs the pointer to a material given a material name
	VOID CacheShield();
	bool EnterCustomGamertag(PVOID menuOption);
	bool IsEntUsingShield(DWORD dwEntIndex, DWORD dwWeaponIndex);
	bool IsMantling();
	DWORD CacheTag(PCHAR pName);
	VOID CalcScreenDimensions(); // Grabs the size in pixels of the users screen
	VOID ChangeClass(PVOID pOption);
	VOID JuggClass();
	VOID ChangeTeam();
	bool CheckIsMigrating();
	bool CheckInGame(); // Checks whether or not we're in game, taking multiple things into consideration
	bool DownloadSoundPack();
	VOID DrawBox(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT rotation, const FLOAT *color, const FLOAT *borderColor = NULL, DWORD borderWidth = -1);
	VOID DrawLine(FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2, FLOAT Width, const float *Color, DWORD Shader);
	VOID DrawHandlePic(FLOAT x, FLOAT y, FLOAT w, FLOAT h, DWORD horizAlign, DWORD vertAlign, FLOAT rotation, const FLOAT * color, DWORD shader);
	VOID DrawRotatedPic(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT rotation, const FLOAT *color, DWORD shader); // Draws a rotated material on the screen at the specified location
	VOID DrawText(CHAR *txt, FLOAT width, DWORD font, FLOAT x, FLOAT y, FLOAT scaleX, FLOAT scaleY, FLOAT rotation, const FLOAT *color, bool bDrawShader = false, FLOAT fBorderWidth = 0.0f, DWORD dwNumLinesWrapped = 0); // Draws text on the screen at the specified location
	VOID DrawTextWithBorder(CHAR *text, DWORD font, FLOAT x, FLOAT y, FLOAT scaleX, FLOAT scaleY, FLOAT borderWidth, FLOAT rotation, const FLOAT *color, const FLOAT *borderColor); // Draws text with a border at the specified location
	DWORD Dvar_GetValueAddress(DWORD dwDvarAddress);
	bool Dvar_GetBool(DWORD dwDvarAddress);
	DWORD Get_CEntity_t(DWORD dwClientIndex);
	vec3_t GetEntityLocation(DWORD dwEntIndex);
	DWORD GetEntityStance(DWORD dwEntIndex);
	vec3_t GetEntityViewAngles(DWORD dwEntIndex);
	DWORD GetDropWeaponIndex(DWORD dwEntIndex);
	VOID RunVisibilityChecks(DWORD dwClient, DWORD dwTag, int iTagOffset);
	vec3_t GetTagOffset(DWORD eTagIndex, DWORD dwClient, DWORD eTagOffset);
	vec3_t *GetTagPosition(DWORD eTagIndex, DWORD dwEntIndex, vec3_t *pOutLocation, BOOL bCheckVis = FALSE);
	FLOAT GetFontHeight(DWORD dwFontPtr, FLOAT fScale); // Grabs the height of a font
	DWORD Get_CG_t();
	DWORD Get_CGS_t();
	DWORD Get_ClientActive_t();
	vec3_t * Get_ClientActive_ViewAngles();
	DWORD Get_ClientInfo_t(DWORD dwClient);
	DWORD Get_ClientDObj(DWORD dwClient, DWORD dwLocalClientNum = 0);
	DWORD Get_Input_t();
	DWORD Get_NetInfo(DWORD dwClient);
	IN_ADDR GetClientIP(DWORD dwClient);
	QWORD GetClientXUID(DWORD dwClient);
	PCHAR GetEntityName(DWORD dwEntIndex);
	DWORD Get_RefDef_t();
	DWORD Get_ViewAxis_Ptr();
	vec3_t Get_RefDef_ViewAngles();
	DWORD GetBulletType(DWORD dwWeaponIndex);
	DWORD Get_UnknownClientStruct();
	DWORD CalculateFPS();
	DWORD Dvar_GetValue(DWORD dwDvarAddress);
	VOID EndGame();
	VOID LeaveGame();
	PCHAR GetMapName_Internal();
	PCHAR GetHostName();
	DWORD GetWeaponClipSize(DWORD dwClient);
	DWORD GetWeaponAmmo(DWORD dwClient, bool bLeftHand);
	PCHAR GetGamertagFromXUID(DWORD dwClient, QWORD userXuid);
	PCHAR GetClientGamertag(DWORD dwClient);
	PCHAR GetClientGamertagFromNetInfo(DWORD dwClient);
	SHORT GetEntityTeam(DWORD dwEntIndex);
	VOID SetEntityTeam(DWORD dwEntIndex);
	DWORD GetEntityWeapon(DWORD dwClient, BOOL bPrimary);
	SHORT GetEntityType(DWORD dwClient);
	BOOL GetLocalClientIndex();
	int GetLocalClientHealth();
	int GetStaticLocalClientIndex();
	vec3_t * GetRefDefViewOrigin();
	vec3_t GetPlayerViewOrigin(DWORD cg_t);
	vec3_t CL_GetPredictedOriginForServerTime(int iTime);
	vec3_t GetLocalViewOrigAtTime(int iTime);
	vec3_t GetLocalViewOrigin(BOOL bCached = FALSE);
	int GetObituaryAttacker(DWORD entityState);
	int GetObituaryVictim(DWORD entityState);
	int GetReloadClientIndex(DWORD entityState);
	DWORD GetPing();
	DWORD GetServerTime();
	bool IsPointInBox(vec3_t vPoint, vec3_t vMins, vec3_t vMaxes);
	bool TraceHitShield(DWORD dwAttacker, DWORD dwTarget, DWORD dwTagIndex, int iTagOffset, bool bSecondRun = false);
	ShieldInfo GetShieldInfo(DWORD dwClient);
	XBoneInfo * XModelGetBoneInfo(DWORD pXModel, SHORT sModelIndex);
	DWORD GetXModelForDObj(DWORD pDObj);
	DWORD XModelGetNumBones(DWORD pXModel);
	USHORT XModelGetBone(DWORD pXModel, SHORT sModelIndex);
	PCHAR XModelGetBoneName(DWORD pXModel, SHORT sModelIndex);
	PCHAR SL_ConvertToString(USHORT sIndex);
	DWORD XModelGetPartClassification(DWORD pXModel, SHORT sModelIndex);
	DWORD GetBoneIndex(DWORD dwClient, USHORT uTag);
	DWORD GetBoneIndex(DWORD dwClient, PCHAR pTag);
	//DWORD GetTagHitLocationForDObj(DWORD eTagIndex, DWORD pDObj);
	orientation_t GetBoneOrientation(DWORD dwClient, SHORT sModelIndex);
	int GetWeaponSwitchClientIndex(DWORD entityState);
	vec3_t * GetSpawnAngles();
	VOID GetSpreadForWeapon(DWORD dwWeaponIndex, FLOAT *fMinSpread, FLOAT *fMaxSpread);
	FLOAT GetTextWidth(PCHAR pTxt, DWORD font); // Grabs the width of a piece of text given its font
	VOID AddReliableCommand(PCHAR pCmd);
	sScoreboardInfo GetScoreboardInfo(DWORD dwClient);
	DWORD GetWeaponCooldownTime(BOOL bLeftGun = FALSE);
	DWORD GetWeaponDef(DWORD dwWeaponIndex, BOOL bVariant = FALSE);
	FLOAT GetWeaponHitLocationMultiplier(SHORT sHitLoc, DWORD dwWeapon);
	DWORD GetWeaponIndexForName(PCHAR pName);
	FLOAT GetZoomState();
	VOID GiveAmmo(PVOID option);
	VOID GiveDefaultWeapon(PVOID option);
	VOID GiveKillstreak(PVOID option);
	bool IsClientTopPlayer(DWORD dwClient);
	bool IsBoneVisible(DWORD eTagIndex, DWORD dwClient, int iTagOffset);
	bool IsAntiAimBone(DWORD dwClient, DWORD dwTag);
	bool IsEntVisible(DWORD dwClient);
	bool IsLocalClientUsingVehicle();
	bool IsWeaponFullAuto(DWORD dwWeaponIndex);
	SHORT Trace_GetEntityHitId(PVOID pTrace);
	int IsLookingAtEntity();
	bool IsUsingDualWieldWeapon(DWORD dwClient);
	bool IsEntEnemy(DWORD dwClient);
	bool IsEntInvisible(DWORD dwClient);
	bool IsEntKillable(DWORD dwClient);
	bool IsEntLiving(DWORD dwClient);
	SHORT IsEntLookingAtMe(DWORD dwLookingAtIndex);
	BOOL IsEntPlayer(DWORD dwClient);
	BOOL IsEntAI(DWORD dwClient);
	bool IsEntValid(DWORD dwClient);
	FLOAT LocationalTrace(DWORD dwTargetClient, vec3_t *vFrom, vec3_t *vTo, BYTE *bResult, int optionalSeed = -420);
	VOID ToggleAntiDerank(BOOL bState);
	VOID PreventDerank();
	VOID SendConsoleCommand(PCHAR pCmd);
	PCHAR GetGamertagOffset();
	string BuildUserInfoString(string name, string clantag, string xuid, bool eliteTitle, int titleBg, string titleText);
	void SetIngameInfo(string clantag, string gamertag);
	VOID SetStance(eSetStance stance);
	VOID SetGamertag(PCHAR pNewGamertag);
	PCHAR GetModdedGamertag();
	VOID ModGamertag();
	VOID ToggleClassItems(BOOL bState);
	VOID ToggleGodmode(PVOID opt);
	VOID TeleportTo(int iClient);
	VOID ToggleNoclip(PVOID opt);
	VOID ToggleNeverHost(BOOL bState);
	VOID ToggleThirdPerson(BOOL bState);
	VOID SpoofIPAddress(DWORD ip);
	VOID SetFOV(FLOAT fFov);
	VOID SetPrestige(SHORT iPrestige);
	VOID ToggleNoSway(BOOL bState);
	VOID ToggleNoRecoil(BOOL bState);
	VOID ToggleShowPing(BOOL bState);
	PCHAR GetMapName();
	PCHAR GetGametypeName_Internal();
	PCHAR GetGametypeName();
	vec2_t GetRealFovDist(vec3_t vFromPosition, vec3_t vFromAngles, vec3_t vToPosition);
	bool IsMultiplayer();
	FLOAT LocationalTrace_GetFraction(PBYTE pResult);
	BOOL WorldToScreen(vec3_t *vWorld, FLOAT *screenX, FLOAT *screenY);
	VOID UI_DrawText(PCHAR pText, FLOAT fX, FLOAT fY, FLOAT fScale, DWORD horizAlign, DWORD vertAlign, FLOAT rotation, const FLOAT *color, DWORD font);
	VOID LiveStorage_SetStat(int localclientnum, int statindex, int value);
	VOID LiveStoreage_SetStat(int localclientnum, int statindex, int value);
	VOID UnlockAll();
	VOID WriteIPLogEntry(int iClient);
	extern CONST PCHAR(__cdecl *va)(CONST PCHAR Format, ...); // Used to concat strings in hooks

//private:
	bool CheckInGame_Internal(); // Internal function used by CheckInGame, checks whether or not we're ingame via various methods depending on the title
	int GetLocalClientIndex_Internal(); // Internal function used by GetLocalClientIndex
};

int GetObituaryVictim(DWORD entityState);
