#pragma once
#include <xtl.h>

#if !defined(PATCH_BUILDER)
	#include "xkelib.h"
	#include "Settings.h"
	#include "Debug.h"
#endif

#pragma warning (disable: 4244)
#pragma warning (disable: 4826)

#define docryptoBypass(a,b)  ((QWORD)((QWORD)( ((QWORD)0x610e7a17d2b42090) * ((QWORD)(b)) ) ^ (~(QWORD)(a))))

#if defined(PATCH_SVR)
#define decryptDWORD(a) ((DWORD)(docryptoBypass(a, (QWORD)((((DWORD*)&a - (DWORD*)&Not(NiNJA.cPatches)->CoD))))))
#else
#define	decryptDWORD(a) (a)
#endif

extern BYTE bXorKey[0x10];

struct CheatPatches_s {
	// Structs
	DWORD CEntity_t;
	DWORD CG_t;
	DWORD CGS_t;
	DWORD ClientActive_t;
	DWORD ClientUIActive_t;
	DWORD ClientInfo_t;
	DWORD ClientNetInfo;
	DWORD ScreenPlacement;
	DWORD SystemIPAddress;
	DWORD UploadSpeedAddress;
	DWORD UnknownClientStruct;

	// Functions
#if defined(BO2)
	DWORD AdjustSeed;
#endif
#if defined(AW) || defined(BO3)
	DWORD AimAssist_GetTagPos;
#else
	DWORD BG_UpdatePlayerDObj;
#endif
#if defined(MW2) || defined(MW3) || defined(GHOSTS) || defined(AW)
	DWORD BG_WeaponIsDualWield;
#endif

	DWORD CG_DObjGetWorldTagPos;
	DWORD BG_AdvanceTrace;
	DWORD BG_AmmoForWeapon;
	DWORD BG_GetClipSize;
	DWORD BG_GetSpreadForWeapon;
	DWORD BG_GetSurfacePenetrationDepth;
	DWORD BG_GetWeaponDef;
	DWORD BG_WeaponHasPerk;
#if defined(MW3)
	DWORD BG_GetWeaponVariantDef;
#endif
	DWORD BulletTrace;
	DWORD CBuf_AddText;
	DWORD CG_BulletHitEvent;
	DWORD CG_CompassCalcDimensions;
	DWORD CG_CalcNamePosition;
	DWORD CG_CompassDrawPlayerMap;
	DWORD CG_CompassDrawPlayerMapLocationSelector;
	DWORD CG_CompassUpYawVector;
	DWORD CG_DrawGrenadePointer;
	DWORD CG_DrawRotatedPic;
	DWORD CG_EntityEvent;
	DWORD CG_GetPlayerViewOrigin;
	DWORD CG_GetWeaponIndexForName;
	DWORD CG_GoodRandomFloat;
	DWORD CG_IsPlayerDead;
	DWORD CG_LocationalTrace;
	DWORD CG_Respawn;
	DWORD CG_VehLocalClientUsingVehicle;
	DWORD CG_WorldPosToCompass;
	DWORD CL_AddReliableCommand;
	DWORD CL_AllLocalClientsDisconnected;
	DWORD CL_CreateNewCommands;
	DWORD CL_GetPredictedOriginForServerTime;
	DWORD CL_GetScreenDimensions;
	DWORD CL_RegisterFont;
	DWORD CL_SendCommand;
	DWORD CL_SetStance;
	DWORD CycleWeapPrimary;
	DWORD Com_GetClientDObj;
	DWORD Com_InitThreadData;
	DWORD G_ThreadValues;
	DWORD DB_FindXAssetHeader;
	DWORD Dvar_GetBool;
	DWORD DynEntCl_DynEntImpactEvent;
	DWORD FireBulletPenetrate;
	DWORD FX_GetBoneOrientation;
	DWORD G_GetWeaponHitLocationMultiplier;
#if defined(BO3)
	DWORD GetClientEntity;
#endif
#if defined(GHOSTS)
	DWORD GetWeaponHudName;
#endif
#if defined(MW2) || defined(AW) || defined(GHOSTS)
	DWORD GetWeaponInfo;
#endif
#if defined(MW1) || defined(WAW) || defined(MW2) || defined(MW3) || defined(GHOSTS) || defined(AW)
	DWORD LiveStorage_SetStat;
#endif
	DWORD Material_RegisterHandle;
	DWORD RandomBulletDir;
	DWORD R_AddDObjToScene;
	DWORD R_DrawIndexedPrimitive;
	DWORD R_DrawText;
	DWORD R_EndFrame;
	DWORD R_TextHeight;
	DWORD R_TextWidth;
	DWORD SCR_UpdateFrame;
	DWORD Sys_CreateThread;
	DWORD SL_ConvertToString;
	DWORD SL_GetString;
	DWORD Trace_GetEntityHitId;
#if defined(BO2) || defined(MW2) || defined(MW3)
	DWORD TransformSeed;
#endif
	DWORD UI_DrawHandlePic;
	DWORD UI_DrawText;
	DWORD UI_GetGametypeDisplayName;
	DWORD UI_GetMapDisplayName;
	DWORD UI_SafeTranslateString;
	DWORD va;
	DWORD XSessionCreate;
	DWORD XSessionDelete;

	//Other
#if defined(MW2) || defined(MW3) || defined(GHOSTS) || defined(BO2)
	DWORD Autowall360Patch;
#endif
#if defined(WAW) || defined(BO1) || defined(BO2)
	DWORD EntityTeamAddress;
#endif
	DWORD ET_ITEM;
	DWORD ET_ACTOR;
	DWORD ET_MISSILE;
	DWORD ET_PLAYER;
	DWORD ET_HELICOPTER;
	DWORD EV_AMMO_PICKUP;
	DWORD EV_NOAMMO;
	DWORD EV_RELOAD_ADDAMMO;
	DWORD EV_RELOAD_START;
	DWORD EV_RELOAD_END;
	DWORD EV_PUTAWAY_WEAPON;
	DWORD EV_OBITUARY;
	DWORD ImageTrack;
	DWORD NoRecoil, NoRecoil2;
	DWORD NoSway, NoSway2;
	DWORD Radar, Radar2, Radar3;
	DWORD ServerIdAddress;
#if defined(MW1) || defined(WAW) || defined(MW2)
	DWORD Dvar_ActiveAction;
#endif
	DWORD Dvar_CG_FOV;
	DWORD Dvar_CG_ThirdPerson;
	DWORD Dvar_PartyHostname;
	DWORD Dvar_UI_Mapname;
#if defined(BO1) || defined(BO2)
	DWORD AutowallFix1, AutowallFix2;
#endif
	DWORD XModelOffset, XModelBoneOffset, XModelPartClassification;
	DWORD ClassItemsAddress;
	DWORD ClientIndexOffset, ScoreOffset, KillsOffset, DeathsOffset, AssistsOffset, PingOffset, RankOffset, TeamOffset, ScoresOffset, ScoresSize;
#if defined(MW2)
	DWORD DisableStatWipe;
#endif
	DWORD PregameGTOffset;
#if defined(AW)
	DWORD ArrowHeightOffset, ArrowWidthOffset;
	DWORD ArrowMaterialOffset;
	DWORD ArrowScaleOffset;
	DWORD ArrowRadiusOffset;
#endif
	DWORD ThreadIndexArray;

#if defined(BO2)
	DWORD PreGameAntiFreezeDetour;
	DWORD GameMessage1;
	DWORD GameMessage2;
#endif

	DWORD PrestigeOffset;

	DWORD NumPatches,
		SizeOfPatches;
	BYTE Patches[0x500];

	BOOL AddPatch(DWORD Address, BYTE *Data, DWORD Size) {
		// Make sure the patch is 4 byte aligned
		if ((Size & 0x3) != 0) {
			DbgPrint("Patch size must be 4 byte aligned.");
			return false;
		}
		if (SizeOfPatches + Size + 0x8 < 0x500) {
			*(DWORD*)&Patches[SizeOfPatches] = Size;
			*(DWORD*)&Patches[SizeOfPatches + 0x4] = Address;
			for (unsigned int i = 0; i<Size; i++) {
				Patches[SizeOfPatches + i + 0x8] = Data[i];
			}
			SizeOfPatches += Size + 0x8;
			NumPatches++;
			DbgPrint("Success initializing memory patch of length %i at address 0x%X", Size, Address);
			return true;
		}
		DbgPrint("Attempted to initialize memory patches exceeding 0x500 bytes in size. Increase the buffer size or remove some patches.");
		return false;
	}
};

// Structs for cheats/security offsets
class Cheats_s {
public:
	BOOL bHasPatches; // Have our patches been populated?
	CheatPatches_s CoD; // The encrypted patchset for the current game

	HRESULT SetAddresses(PVOID data) {
#ifndef PATCH_SVR
		CoD.SystemIPAddress = 0xC13180E0; // 17511 0xC13380E0;
		CoD.AddPatch(0x11e23e7c, (PBYTE)"\x86\x19\x14\x98\x3e\x25\x33\x91\x74\xfc\xea\x1f\x9a\xf9\x5d\x2d", 0x10);
	#if defined(MW1)
		// Structs
		CoD.CEntity_t = 0x823F5054;
		CoD.CG_t = 0x823F28A0;
		CoD.CGS_t = 0x823F2890;
		CoD.ClientActive_t = 0x82435AB8;
		CoD.ClientInfo_t = 0x0;
		CoD.ClientNetInfo = 0x8243D700;
		CoD.UploadSpeedAddress = 0x84C252FC;
		CoD.ScreenPlacement = 0x8246f308;
		CoD.UnknownClientStruct = 0x823F6040;
		// Functions
		CoD.BG_AmmoForWeapon = 0x823022C0;
		CoD.BG_GetClipSize = 0x820A0198;
		CoD.BG_GetSpreadForWeapon = 0x8232C478;
		CoD.BG_GetWeaponDef = 0x8232B5D8;
		CoD.BG_UpdatePlayerDObj = 0x82343030;
		CoD.CG_BulletHitEvent = 0x822EC2F8;
		CoD.CBuf_AddText = 0x82239FD0;
		CoD.CG_CalcNamePosition = 0x8231B4B0;
		CoD.CG_CompassCalcDimensions = 0x82323E28;
		CoD.CG_CompassDrawPlayerMap = 0x823250B8;
		CoD.CG_CompassDrawPlayerMapLocationSelector = 0x82324820;
		CoD.CG_CompassUpYawVector = 0x82323B78;
		CoD.CG_GetWeaponIndexForName = 0x82332FA0;
		CoD.CG_DObjGetWorldTagPos = 0x82316088;
		CoD.CG_DrawGrenadePointer = 0x8231EC68;
		CoD.CG_DrawRotatedPic = 0x82318A88;
		CoD.CG_EntityEvent = 0x82312C08;
		CoD.CG_IsPlayerDead = 0x82301E10;
		CoD.CG_LocationalTrace = 0x8235DAA0;
		CoD.CG_GetPlayerViewOrigin = 0x822EB5E8;
		CoD.CG_WorldPosToCompass = 0x82323740;
		CoD.CL_AddReliableCommand = 0x822CF268;
		CoD.CL_AllLocalClientsDisconnected = 0x822CF5C0;
		CoD.CL_CreateNewCommands = 0x822DCAE8;
		CoD.CL_GetScreenDimensions = 0x824303A0;
		CoD.CL_RegisterFont = 0x0;
		CoD.CL_SendCommand = 0x822D86A0;
		CoD.CL_SetStance = 0x822D92A0;
		CoD.Com_GetClientDObj = 0x82308E20;
		CoD.DB_FindXAssetHeader = 0x822A0298;
		CoD.Dvar_GetBool = 0x82435A08; // 0x821D15D8;
		CoD.DynEntCl_DynEntImpactEvent = 0x8229C6B0;
		CoD.FireBulletPenetrate = 0x822EC4D0;
		CoD.FX_GetBoneOrientation = 0x8228C4C0;
		CoD.G_GetWeaponHitLocationMultiplier = 0x82279DA8;
		CoD.LiveStorage_SetStat = 0x821A29D8;
		CoD.Material_RegisterHandle = 0x0;
		CoD.RandomBulletDir = 0x82288E10;
		CoD.R_AddDObjToScene = 0x82168BC8;
		CoD.R_DrawIndexedPrimitive = 0x82142F80;
		CoD.R_DrawText = 0x8216C0B8;
		CoD.R_EndFrame = 0x8216CAA0;
		CoD.R_TextHeight = 0x820BD0A0;
		CoD.R_TextWidth = 0x8216EB00;
		CoD.SL_ConvertToString = 0x8221EF30;
		CoD.SL_GetString = 0x8221F798;
		CoD.Trace_GetEntityHitId = 0x8223D5D8;
		CoD.UI_DrawHandlePic = 0x82318C10;
		CoD.UI_DrawText = 0x821EB858;
		CoD.UI_GetMapDisplayName = 0x821ED5A8;
		CoD.UI_GetGametypeDisplayName = 0x821EAA08;
		CoD.UI_SafeTranslateString = 0x821ECA78;
		CoD.va = 0x821CD858;
		CoD.XSessionCreate = 0x82109E68;
		CoD.XSessionDelete = 0x8210A0C0;

		CoD.AddPatch(0x822EB6A0, (PBYTE)"\x60\x00\x00\x00", 4); // Disable CG_GetPlayerViewOrigin error
		CoD.AddPatch(0x822CF5DC, (PBYTE)"\x60\x00\x00\x00", 4); // Allow CL_AllLocalClientsDisconnected call from any thread
		CoD.AddPatch(0x822CF5F0, (PBYTE)"\x60\x00\x00\x00", 4); // Same as above
		

		// Other
		CoD.Dvar_ActiveAction = 0x8243038C;
		CoD.Dvar_CG_FOV = 0x823F5784;
		CoD.Dvar_CG_ThirdPerson = 0x823F27A0;
		CoD.Dvar_PartyHostname = 0x8243C248;
		CoD.Dvar_UI_Mapname = 0x849F2D6C;
		CoD.ET_ITEM = 3;
		CoD.ET_ACTOR = 420;
		CoD.ET_MISSILE = 4;
		CoD.ET_PLAYER = 1;
		CoD.EV_AMMO_PICKUP = 10;
		CoD.EV_NOAMMO = 11;
		CoD.EV_RELOAD_ADDAMMO = 20;
		CoD.EV_RELOAD_START = 17;
		CoD.EV_RELOAD_END = 18;
		CoD.EV_PUTAWAY_WEAPON = 23;
		CoD.EV_OBITUARY = 72;
		CoD.ImageTrack = 1;
		CoD.NoRecoil = 0x822EDAA0;
		CoD.NoSway = 0x8233169C; // Not aiming
		CoD.NoSway2 = 0x8233048C; // Aiming
		CoD.Radar = 0x0; // g_compassshowenemies
		CoD.XModelOffset = 0x60;
		CoD.XModelBoneOffset = 0x8;
		CoD.XModelPartClassification = 0x18;
		CoD.ScoresOffset = 0x4dedc;
		CoD.ScoresSize = 0x28;
		CoD.ClientIndexOffset = 0x0;
		CoD.ScoreOffset = 0x4;
		CoD.PingOffset = 0x8;
		CoD.DeathsOffset = 0xc;
		CoD.TeamOffset = 0x10;
		CoD.KillsOffset = 0x14;
		CoD.RankOffset = 0x18;
		CoD.AssistsOffset = 0x1c;
		CoD.ServerIdAddress = 0x84B3DDD4;
		CoD.PregameGTOffset = 0x84C24BBC;
		CoD.PrestigeOffset = 0x84C5EF26;
		CoD.G_ThreadValues = 0x82B85408;
		CoD.Sys_CreateThread = 0x8222E778;
		CoD.ThreadIndexArray = 0x82B854F4;
		CoD.Com_InitThreadData = 0x8222E540; //0x821CD808;
		CoD.CycleWeapPrimary = 0x822EA2A0;
		CoD.CG_Respawn = 0x8235DE98;
	#elif defined(MW2)
		// Structs
		CoD.CEntity_t = 0x825BE150;
		CoD.CG_t = 0x825B8638;
		CoD.CGS_t = 0x825B861C;
		CoD.ClientActive_t = 0x82682350; //tu8 0x82682354;
		CoD.ClientUIActive_t = 0x8267F100;
		CoD.ClientInfo_t = 0x0;
		CoD.ClientNetInfo = 0x8268EEBA; //tu8 0x8268EEC2;
		CoD.UploadSpeedAddress = 0x838BBB10;
		CoD.ScreenPlacement = 0x82690F40; //tu8 0x82690f48;
		// Functions
		CoD.BG_AmmoForWeapon = 0x820DB108;
		CoD.BG_GetClipSize = 0x820E3908;
		CoD.BG_GetSpreadForWeapon = 0x820E33F0;
		CoD.BG_GetWeaponDef = 0x820E22C0;
		CoD.BG_UpdatePlayerDObj = 0x820CA358;
		CoD.BG_WeaponIsDualWield = 0x820DAF58;
		CoD.BulletTrace = 0x8212E760;
		CoD.CBuf_AddText = 0x82224990; //tu8 0x82224A28;
		CoD.CG_BulletHitEvent = 0x82134510;
		CoD.CG_CalcNamePosition = 0x820F2348;
		CoD.CG_CompassCalcDimensions = 0x820EB278;
		CoD.CG_CompassDrawPlayerMap = 0x820EC0C8;
		CoD.CG_CompassDrawPlayerMapLocationSelector = 0x820EC468;
		CoD.CG_CompassUpYawVector = 0x820ECD00;
		CoD.CG_GetPlayerViewOrigin = 0x8212E670;
		CoD.CG_GetWeaponIndexForName = 0x820E2308;
		CoD.CG_DObjGetWorldTagPos = 0x820FB9E8;
		CoD.CG_DrawGrenadePointer = 0x820F17F0;
		CoD.CG_DrawRotatedPic = 0x820F7900;
		CoD.CG_EntityEvent = 0x820FD778;
		CoD.CG_IsPlayerDead = 0x82111B48;
		CoD.CG_LocationalTrace = 0x82136E88;
		CoD.CG_WorldPosToCompass = 0x820EB0A0;
		CoD.CL_AddReliableCommand = 0x82142BB8; //tu8 0x82142BC0;
		CoD.CL_AllLocalClientsDisconnected = 0x82165CF0; //tu8 0x82165D00;
		CoD.CL_CreateNewCommands = 0x821400E8; //tu8 0x82140128;
		CoD.CL_GetScreenDimensions = 0x82137C50;
		CoD.CL_RegisterFont = 0x82146458; //tu8 0x82146488;
		CoD.CL_SendCommand = 0x821406E8; //tu8 0x82140728;
		CoD.CL_SetStance = 0x8213E600; //tu8 0x8213E640;
		CoD.Com_GetClientDObj = 0x8222FCC0; //tu8 0x8222FD58;
		CoD.DB_FindXAssetHeader = 0x8219BBC8; //tu8 0x8219BBD8;
		CoD.Dvar_GetBool = 0x8229EEE8; //tu8 0x8229EF58;
		CoD.DynEntCl_DynEntImpactEvent = 0x821A4A50; //tu8 0x821A4A58;
		CoD.FireBulletPenetrate = 0x82134648;
		CoD.FX_GetBoneOrientation = 0x821CA8A8; //tu8 0x821CA940;
		CoD.G_GetWeaponHitLocationMultiplier = 0x821DD068; //tu8 0x821DD100;
		CoD.GetWeaponInfo = 0x820E22D8;
		CoD.LiveStorage_SetStat = 0x8211D2A8;
		CoD.Material_RegisterHandle = 0x8234E510; //tu8 0x8234E4F0;
		CoD.RandomBulletDir = 0x821CD5F8; //tu8 0x821CD690;
		CoD.R_AddDObjToScene = 0x82352A98; //tu8 0x82352A78;
		CoD.R_DrawIndexedPrimitive = 0x82388048; //tu8 0x82388028;
		CoD.R_DrawText = 0x82350278; //tu8 0x82350258;
		CoD.R_EndFrame = 0x82351748; //tu8 0x82351728;
		CoD.R_TextHeight = 0x8234DE10; //tu8 0x8234DDF0;
		CoD.R_TextWidth = 0x8234DD20; //tu8 0x8234DD00;
		CoD.SL_ConvertToString = 0x82241898; //tu8 0x82241920;
		CoD.SL_GetString = 0x82242250; //tu8 0x822422D8;
		CoD.Trace_GetEntityHitId = 0x8221DAD0; //tu8 0x8221DB68;
		CoD.TransformSeed = 0x820D94B8;
		CoD.UI_DrawHandlePic = 0x820F7AB0;
		CoD.UI_DrawText = 0x8226D420; //tu8 0x8226D490;
		CoD.UI_GetMapDisplayName = 0x822729A0; //tu8 0x82272A10;
		CoD.UI_GetGametypeDisplayName = 0x82272BD0; //tu8 0x82272C40;
		CoD.va = 0x822A7028; //tu8 0x822A7008;
		// Other
		// HitLocation multipliers that don't get populated until we *host* a game... fuck that, we just copy it there on our own :D
		CoD.AddPatch(0x82ED64D0, (PBYTE)"\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 80);
		CoD.AddPatch(0x822CCDBC /* tu8- 0x822CCD9C */, (PBYTE)"\x60\x00\x00\x00", 0x4); // Code error: free called

		CoD.Autowall360Patch = 0x8213492C;
		CoD.DisableStatWipe = 0x8222ADF4; //tu8 0x8222AE8C;
		CoD.Dvar_ActiveAction = 0x825E21AC;
		CoD.Dvar_CG_FOV = 0x825BE8C8;
		CoD.Dvar_CG_ThirdPerson = 0x825B435C;
		CoD.Dvar_PartyHostname = 0x8268DBC8; //tu8 0x8268DBD0;
		CoD.Dvar_UI_Mapname = 0x836477B8;
		CoD.ET_ITEM = 3;
		CoD.ET_ACTOR = 420;
		CoD.ET_MISSILE = 4;
		CoD.ET_PLAYER = 1;
		CoD.EV_AMMO_PICKUP = 11;
		CoD.EV_NOAMMO = 12;
		CoD.EV_RELOAD_ADDAMMO = 23;
		CoD.EV_RELOAD_START = 20;
		CoD.EV_RELOAD_END = 21;
		CoD.EV_OBITUARY = 104;
		CoD.EV_PUTAWAY_WEAPON = 28;
		CoD.ImageTrack = 1;
		CoD.NoRecoil = 0x82135BFC;
		CoD.NoSway = 0x820E5B38; // Not aiming
		CoD.NoSway2 = 0x820E657C; // Aiming
		CoD.Radar = 0x820EF0C4;
		
		CoD.XModelOffset = 0x98;
		CoD.XModelBoneOffset = 0x24;
		CoD.XModelPartClassification = 0x34;
		CoD.ScoresOffset = 0x73170;
		CoD.ScoresSize = 0x28;
		CoD.ClientIndexOffset = 0x0;
		CoD.ScoreOffset = 0x4;
		CoD.PingOffset = 0x8;
		CoD.DeathsOffset = 0xc;
		CoD.TeamOffset = 0x10;
		CoD.KillsOffset = 0x14;
		CoD.RankOffset = 0x18;
		CoD.AssistsOffset = 0x1c;
		CoD.ServerIdAddress = 0x826237E0;
		CoD.PregameGTOffset = 0x838BA824;
		CoD.PrestigeOffset = 0x831A0DD3;
		CoD.SCR_UpdateFrame = 0x8214C248; //tu8 0x8214C258;
		CoD.G_ThreadValues = 0x83372148;
		CoD.Sys_CreateThread = 0x8223CA28; //tu8 0x8223CAB0;
		CoD.ThreadIndexArray = 0x83372254;
		CoD.Com_InitThreadData = 0x8223C918; //tu8 0x8223C9A0;
		CoD.CycleWeapPrimary = 0x82133710;
		CoD.CL_GetPredictedOriginForServerTime = 0x8214A0A0; //tu8 0x8214A0D0;
		CoD.CG_Respawn = 0x82112D58;
	#elif defined(MW3)
		// Structs
		CoD.CEntity_t = 0x826436B8;
		CoD.CG_t = 0x82643E5C;
		CoD.CGS_t = 0x8263EE44;
		CoD.ClientActive_t = 0x82713DC4;
		CoD.ClientUIActive_t = 0x82713610;
		CoD.ClientInfo_t = 0xBB6A8508;
		CoD.ClientNetInfo = 0x82720EEC;
		CoD.UploadSpeedAddress = 0x839696E4;
		CoD.ScreenPlacement = 0x82732100;
		CoD.UnknownClientStruct = 0x8263EE60;
		// Functions
		CoD.BG_AmmoForWeapon = 0x820E1FA0;
		CoD.BG_GetClipSize = 0x82102040;
		CoD.BG_GetSpreadForWeapon = 0x82101B08;
		CoD.BG_GetWeaponDef = 0x825E06E0;
		CoD.BG_GetWeaponVariantDef = 0x825E0318;
		CoD.BG_UpdatePlayerDObj = 0x820D9D10;
		CoD.BG_WeaponIsDualWield = 0x82101550;
		CoD.BulletTrace = 0x82159330;
		CoD.CBuf_AddText = 0x82287F68; //tu23 0x82287EE0;
		CoD.CG_BulletHitEvent = 0x8215F9C0;
		CoD.CG_CalcNamePosition = 0x82112908;
		CoD.CG_CompassCalcDimensions = 0x82109E28;
		CoD.CG_CompassDrawPlayerMap = 0x8210AC98;
		CoD.CG_CompassDrawPlayerMapLocationSelector = 0x8210B1B8;
		CoD.CG_CompassUpYawVector = 0x8210BB08;
		CoD.CG_GetPlayerViewOrigin = 0x82159240;
		CoD.CG_GetWeaponIndexForName = 0x820F7EB0;
		CoD.CG_DObjGetWorldTagPos = 0x8211BCB8;
		CoD.CG_DrawGrenadePointer = 0x82111B88;
		CoD.CG_DrawRotatedPic = 0x82117668;
		CoD.CG_EntityEvent = 0x8211DCD0;
		CoD.CG_IsPlayerDead = 0x821348A8;
		CoD.CG_LocationalTrace = 0x821631B8;
		CoD.CG_WorldPosToCompass = 0x82109C50;
		CoD.CL_AddReliableCommand = 0x821708B0;
		CoD.CL_AllLocalClientsDisconnected = 0x821A7A60; //tu23 0x821A79E0;
		CoD.CL_CreateNewCommands = 0x8216D8C8;
		CoD.CL_GetScreenDimensions = 0x82164088;
		CoD.CL_RegisterFont = 0x82174F88;
		CoD.CL_SendCommand = 0x8216DF18;
		CoD.CL_SetStance = 0x8216B928;
		CoD.Com_GetClientDObj = 0x822A28D0; //tu23 0x822A2848;
		CoD.DB_FindXAssetHeader = 0x821EEBF0; //tu23 0x821EEB70;
		CoD.Dvar_GetBool = 0x8232E2C8; //tu23 0x8232E200;
		CoD.DynEntCl_DynEntImpactEvent = 0x821FDF18; //tu23 0x821FDE98;
		CoD.FireBulletPenetrate = 0x8215FB08;
		CoD.G_GetWeaponHitLocationMultiplier = 0x8223A7F0; //tu23 0x8223A770;
		CoD.FX_GetBoneOrientation = 0x82222370; //tu23 0x822222F0;
		CoD.LiveStorage_SetStat = 0x82141628;
		CoD.Material_RegisterHandle = 0x82413F48; //tu23 0x82413E80;
		CoD.RandomBulletDir = 0x82229B50; //tu23 0x82229AD0;
		CoD.R_DrawText = 0x8241F9E0; //tu23 0x8241F918;
		CoD.R_EndFrame = 0x82421048; //tu23 0x82420F80;
		CoD.R_TextHeight = 0x82407C80; //tu23 0x82407BB8;
		CoD.R_TextWidth = 0x82407B00; //tu23 0x82407A38;
		CoD.SL_ConvertToString = 0x822B5120; //tu23 0x822B5080;
		CoD.SL_GetString = 0x822B5CC8; //tu23 0x822B5C28;
		CoD.Trace_GetEntityHitId = 0x82280720; //tu23 0x82280698;
		CoD.TransformSeed = 0x820E9C28;
		CoD.UI_DrawHandlePic = 0x82117810;
		CoD.UI_DrawText = 0x822EC490; //tu23 0x822EC3C8;
		CoD.UI_GetMapDisplayName = 0x822F2580; //tu23 0x822F24B8;
		CoD.UI_GetGametypeDisplayName = 0x822F27A8; //tu23 0x822F26E0;
		CoD.va = 0x823365F0; //tu23 0x82336528;
		// Other
		// HitLocation multipliers that don't get populated until we *host* a game... fuck that, we just copy it there on our own :D
		CoD.AddPatch(0x82D9FB40, (PBYTE)"\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x00\x00\x00\x00\x3F\x80\x00\x00", 80);
		CoD.AddPatch(0x8232A3C0, (PBYTE)"\x38\x60\x00\x01\x7C\x6A\x59\xAE\x4B\xFF\xFF\xD0", 12); // Enable DLC maps even if they aren't present, tu23 0x8232A2F8
		CoD.AddPatch(0x820D38D0, (PBYTE)"\x60\x00\x00\x00", 0x4); // Cannot find tag %s on entity
		CoD.AddPatch(0x823605DC, (PBYTE)"\x60\x00\x00\x00", 0x4); // Code error: free called, tu23 0x82360514
		
		CoD.Autowall360Patch = 0x8215FDEC;
		CoD.Dvar_CG_FOV = 0x82643EC4;
		CoD.Dvar_CG_ThirdPerson = 0x8263ED14;
		CoD.Dvar_UI_Mapname = 0x836E5A18;
		CoD.ET_ITEM = 3;
		CoD.ET_ACTOR = 420;
		CoD.ET_MISSILE = 4;
		CoD.ET_PLAYER = 1;
		CoD.EV_AMMO_PICKUP = 11;
		CoD.EV_NOAMMO = 12;
		CoD.EV_RELOAD_ADDAMMO = 23;
		CoD.EV_RELOAD_START = 20;
		CoD.EV_RELOAD_END = 21;
		CoD.EV_OBITUARY = 108;
		CoD.EV_PUTAWAY_WEAPON = 28;
		CoD.ImageTrack = 0;
		CoD.NoRecoil = 0x821614D4;
		CoD.NoSway = 0x82103CD8; // Not aiming
		CoD.NoSway2 = 0x8210468C; // Aiming
		CoD.Radar = 0x8210E58C;
		
		CoD.XModelOffset = 0x98;
		CoD.XModelBoneOffset = 0x24;
		CoD.XModelPartClassification = 0x34;
		CoD.ScoresOffset = 0xb138;
		CoD.ScoresSize = 0x2c;
		CoD.ClientIndexOffset = 0x0;
		CoD.ScoreOffset = 0x4;
		CoD.PingOffset = 0x8;
		CoD.DeathsOffset = 0xc;
		CoD.TeamOffset = 0x10;
		CoD.KillsOffset = 0x14;
		CoD.RankOffset = 0x18;
		CoD.AssistsOffset = 0x1c;
		CoD.ServerIdAddress = 0x826B81E8;
		CoD.PregameGTOffset = 0x839691AC;
		CoD.PrestigeOffset = 0x830A6D6B;
		CoD.G_ThreadValues = 0x83101FF0;
		CoD.Sys_CreateThread = 0x822AE7B0; //tu23 0x822AE710;
		CoD.ThreadIndexArray = 0x83102118;
		CoD.Com_InitThreadData = 0x822AE6A0; //tu23 0x822AE600;
		CoD.CycleWeapPrimary = 0x8215EBC8;
		CoD.CG_Respawn = 0x82135BC0;
	#elif defined(GHOSTS)
		// Structs
		CoD.CEntity_t = 0x82AD11AC;
		CoD.CG_t = 0x82AD56FC;
		CoD.CGS_t = 0x82AD56F8;
		CoD.ClientActive_t = 0x82BAC404;
		CoD.ClientUIActive_t = 0x82BAC408;
		CoD.ClientInfo_t = 0xBB450BBC;
		CoD.ClientNetInfo = 0x82BBD634;
		CoD.UploadSpeedAddress = 0x83F0A254;
		CoD.ScreenPlacement = 0x82bcdd10;
		// Functions
		CoD.BG_AmmoForWeapon = 0x8222C618;
		CoD.BG_GetClipSize = 0x82255110;
		CoD.BG_GetSpreadForWeapon = 0x82254700;
		CoD.BG_GetWeaponDef = 0x8213EDE0;
		CoD.BG_UpdatePlayerDObj = 0x82221F70;
		CoD.BG_WeaponIsDualWield = 0x82253798;
		CoD.BulletTrace = 0x822BC168;
		CoD.CBuf_AddText = 0x8244C738;
		CoD.CG_BulletHitEvent = 0x822C73C0;
		CoD.CG_CalcNamePosition = 0x8226A098;
		CoD.CG_CompassCalcDimensions = 0x8225F3D8;
		CoD.CG_CompassDrawPlayerMap = 0x82260B50;
		CoD.CG_CompassUpYawVector = 0x82261428;
		CoD.CG_GetPlayerViewOrigin = 0x822BC078;
		CoD.CG_GetWeaponIndexForName = 0x8224A260; // BG_FindWeaponIndexForName
		CoD.CG_DObjGetWorldTagPos = 0x82273BC8;
		CoD.CG_DrawGrenadePointer = 0x822679A8;
		CoD.CG_DrawRotatedPic = 0x82270138;
		CoD.CG_EntityEvent = 0x822785C0;
		CoD.CG_GoodRandomFloat = 0x8222EC70;
		CoD.CG_IsPlayerDead = 0x8228DF70;
		CoD.CG_LocationalTrace = 0x822CB3B0;
		CoD.CG_WorldPosToCompass = 0x8225FD18;
		CoD.CL_AddReliableCommand = 0x822D8F88;
		CoD.CL_AllLocalClientsDisconnected = 0x82316CA0;
		CoD.CL_CreateNewCommands = 0x822D67E8;
		CoD.CL_GetScreenDimensions = 0x822CC3C0;
		CoD.CL_RegisterFont = 0x822DCC20;
		CoD.CL_SendCommand = 0x822D68C0;
		CoD.CL_SetStance = 0x822D3A38;
		CoD.Com_GetClientDObj = 0x824723D8;
		CoD.DB_FindXAssetHeader = 0x0;
		CoD.Dvar_GetBool = 0x825639B0;
		CoD.DynEntCl_DynEntImpactEvent = 0x82385B38;
		CoD.FireBulletPenetrate = 0x822C7518;
		CoD.FX_GetBoneOrientation = 0x823ABC40;
		CoD.G_GetWeaponHitLocationMultiplier = 0x823E7FB0;
		CoD.GetWeaponInfo = 0x82143408;
		CoD.GetWeaponHudName = 0x822BE118;
		CoD.LiveStorage_SetStat = 0x8229CBD8;
		CoD.Material_RegisterHandle = 0x8265A470;
		CoD.RandomBulletDir = 0x823C9120;
		CoD.R_DrawText = 0x8266CEB8;
		CoD.R_EndFrame = 0x822E7298;
		CoD.R_TextHeight = 0x8264DD70;
		CoD.R_TextWidth = 0x8264DC78;
		CoD.SL_ConvertToString = 0x82492318;
		CoD.SL_GetString = 0x82493478;
		CoD.Trace_GetEntityHitId = 0x82443F98;
		CoD.UI_DrawHandlePic = 0x822702F0;
		CoD.UI_DrawText = 0x825228F0;
		CoD.UI_GetMapDisplayName = 0x82527AD0;
		CoD.UI_GetGametypeDisplayName = 0x82527CF8;
		CoD.va = 0x8256E970;
		// Other
		// HitLocation multipliers that don't get populated until we *host* a game... fuck that, we just copy it there on our own :D
		CoD.AddPatch(0x83281F98, (PBYTE)"\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x3F\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 80);
		CoD.AddPatch(0x82316CB8, (PBYTE)"\x60\x00\x00\x00", 0x4); // Allow calls to CL_AllLocalClientsDisconnected from any thread
		CoD.AddPatch(0x82316CC8, (PBYTE)"\x60\x00\x00\x00", 0x4); // Same as above
		CoD.Autowall360Patch = 0x822C784C;
		CoD.Dvar_CG_FOV = 0x82ACCADC;
		CoD.Dvar_CG_ThirdPerson = 0x82A90960;
		CoD.Dvar_UI_Mapname = 0x83CAC2C0;
		CoD.ET_ITEM = 3;
		CoD.ET_ACTOR = 18; //ET_AGENT
		CoD.ET_MISSILE = 4;
		CoD.ET_PLAYER = 1;
		CoD.EV_AMMO_PICKUP = 11;
		CoD.EV_NOAMMO = 12;
		CoD.EV_RELOAD_ADDAMMO = 22;
		CoD.EV_RELOAD_START = 19;
		CoD.EV_RELOAD_END = 20;
		CoD.EV_OBITUARY = 114;
		CoD.EV_PUTAWAY_WEAPON = 25;
		CoD.ImageTrack = 0;
		CoD.NoRecoil = 0x822C9344;
		CoD.NoSway = 0x822583D0; // Not aiming
		CoD.NoSway2 = 0x82258BEC; // Aiming
		CoD.Radar = 0x822653A0;
		CoD.Radar2 = 0x82265794;
		CoD.Radar3 = 0x82265800;

		CoD.XModelOffset = 0x98;
		CoD.XModelBoneOffset = 0x24;
		CoD.XModelPartClassification = 0x34;
		CoD.ScoresOffset = 0x786A0;
		CoD.ScoresSize = 0x44;
		CoD.ClientIndexOffset = 0x0;
		CoD.ScoreOffset = 0x8;
		CoD.PingOffset = 0xc;
		CoD.DeathsOffset = 0x10;
		CoD.TeamOffset = 0x14;
		CoD.KillsOffset = 0x18;
		CoD.RankOffset = 0x1c;
		CoD.AssistsOffset = 0x28;
		CoD.ServerIdAddress = 0x82B55908;
		CoD.PregameGTOffset = 0x83F0A35C;
		CoD.PrestigeOffset = 0x835887F3;
		CoD.G_ThreadValues = 0x83655AD8;
		CoD.Sys_CreateThread = 0x824818B0;
		CoD.ThreadIndexArray = 0x83655AA8;
		CoD.Com_InitThreadData = 0x82481820; // 0x8256EA30;
		CoD.CG_Respawn = 0x82292740;
	#elif defined(AW)
		// Structs
		CoD.CEntity_t = 0x82ACFE30;
		CoD.CG_t = 0x82AD0298;
		CoD.CGS_t = 0x82ACFE34;
		CoD.ClientActive_t = 0x82B0D308;
		CoD.ClientUIActive_t = 0x82B0D310;
		CoD.ClientInfo_t = 0x0;
		CoD.ClientNetInfo = 0x82B7E640;
		CoD.UploadSpeedAddress = 0x843DEBAC;
		CoD.ScreenPlacement = 0x82B94570;
		// Functions
		CoD.BG_AmmoForWeapon = 0x820F0168;
		CoD.BG_GetClipSize = 0x826472B8;
		CoD.BG_GetSpreadForWeapon = 0x82647658;
		CoD.BG_GetWeaponDef = 0x820F01D0;
		CoD.BG_WeaponIsDualWield = 0x82641898;
		CoD.CBuf_AddText = 0x82472B80;
		CoD.CG_BulletHitEvent = 0x825D22F8;
		CoD.AimAssist_GetTagPos = 0x82774628;
		CoD.CG_CalcNamePosition = 0x8262C140;
		CoD.CG_CompassCalcDimensions = 0x82637CE0;
		CoD.CG_CompassDrawPlayerMap = 0x82639840;
		CoD.CG_CompassUpYawVector = 0x82636A30;
		CoD.CG_DrawGrenadePointer = 0x826300B0;
		CoD.CG_GetWeaponIndexForName = 0x8248CF08;
		CoD.CG_DrawRotatedPic = 0x826289C0;
		CoD.CG_EntityEvent = 0x8261E5C8;
		CoD.CG_GetPlayerViewOrigin = 0x825D39C0;
		CoD.CG_GoodRandomFloat = 0x82663EF0;
		CoD.CG_IsPlayerDead = 0x82601F28;
		CoD.CG_LocationalTrace = 0x825C2238;
		CoD.CG_WorldPosToCompass = 0x82636A78;
		CoD.CL_AddReliableCommand = 0x825AB430;
		CoD.CL_AllLocalClientsDisconnected = 0x82570AE8;
		CoD.CL_CreateNewCommands = 0x825B79D8;
		CoD.CL_GetScreenDimensions = 0x825BC228;
		CoD.CL_RegisterFont = 0x0;
		CoD.CL_SendCommand = 0x825B7358;
		CoD.CL_SetStance = 0x825B59E8;
		CoD.Com_GetClientDObj = 0x8260DAB8;
		CoD.DB_FindXAssetHeader = 0x82544098;
		CoD.Dvar_GetBool = 0x82364D40;
		CoD.DynEntCl_DynEntImpactEvent = 0x82535D78;
		CoD.FireBulletPenetrate = 0x825D2420;
		CoD.FX_GetBoneOrientation = 0x82515AC8;
		CoD.G_GetWeaponHitLocationMultiplier = 0x8248B7A0;
		CoD.GetWeaponInfo = 0x820F01D0; // I guess this is bg_getweapondef
		CoD.LiveStorage_SetStat = 0x825FA478;
		CoD.Material_RegisterHandle = 0x0;
		CoD.RandomBulletDir = 0x82500B90;
		CoD.R_DrawText = 0x82295F60;
		CoD.R_EndFrame = 0x825A5900;
		CoD.R_TextHeight = 0x822AC010;
		CoD.R_TextWidth = 0x822AC770;
		CoD.SL_ConvertToString = 0x8242F6F0;
		CoD.SL_GetString = 0x8242FD70;
		CoD.Trace_GetEntityHitId = 0x82476F00;
		CoD.UI_DrawHandlePic = 0x82628BD0;
		CoD.UI_DrawText = 0x823934A8;
		CoD.UI_GetMapDisplayName = 0x8246F5C0;
		CoD.UI_GetGametypeDisplayName = 0x8246FA48;
		CoD.va = 0x8235DC18;

		CoD.AddPatch(0x827746D0, (PBYTE)"\x60\x00\x00\x00", 4); // AimAssist_GetTagPos error
		CoD.AddPatch(0x82570B04, (PBYTE)"\x60\x00\x00\x00", 0x4); // Allow calls to CL_AllLocalClientsDisconnected from any thread
		CoD.AddPatch(0x82570B18, (PBYTE)"\x60\x00\x00\x00", 0x4); // Same as above

		// Other
		CoD.Dvar_CG_FOV = 0x829AAEB4;
		CoD.Dvar_CG_ThirdPerson = 0x82A773A8;
		CoD.Dvar_UI_Mapname = 0x83A172D4;
		CoD.ET_ITEM = 3;
		CoD.ET_ACTOR = 420;
		CoD.ET_MISSILE = 4;
		CoD.ET_PLAYER = 1;
		CoD.EV_AMMO_PICKUP = 16;
		CoD.EV_NOAMMO = 17;
		CoD.EV_RELOAD_ADDAMMO = 30;
		CoD.EV_RELOAD_START = 27;
		CoD.EV_RELOAD_END = 28;
		CoD.EV_OBITUARY = 146;
		CoD.EV_PUTAWAY_WEAPON = 33;
		CoD.ImageTrack = 0;
		CoD.NoRecoil = 0x82648CEC;
		CoD.NoSway = 0x826404D0; // Not aiming
		CoD.NoSway2 = 0x82640418; // Aiming
		CoD.Radar = 0x826352A8;
		CoD.ArrowHeightOffset = 0x829AAEA0;
		CoD.ArrowWidthOffset = 0x829AAE9C;
		CoD.ArrowMaterialOffset = 0x826302D6;
		CoD.ArrowScaleOffset = 0x829AAE98;
		CoD.ArrowRadiusOffset = 0x829AAEAC;
		
		CoD.XModelOffset = 0x14C;
		CoD.XModelBoneOffset = 0x2c;
		CoD.XModelPartClassification = 0x3c;
		CoD.ScoresOffset = 0x4366c;
		CoD.ScoresSize = 0x4c;
		CoD.ClientIndexOffset = 0x8;
		CoD.ScoreOffset = 0x10;
		CoD.PingOffset = 0x14;
		CoD.DeathsOffset = 0x18;
		CoD.TeamOffset = 0x1c;
		CoD.KillsOffset = 0x20;
		CoD.RankOffset = 0x24;
		CoD.AssistsOffset = 0x30;
		CoD.ServerIdAddress = 0x82B4F1D4;
		CoD.PregameGTOffset = 0x843DECB4;
		CoD.PrestigeOffset = 0x83A6C150;
		CoD.Sys_CreateThread = 0x82440A70;
		CoD.G_ThreadValues = 0x83B2E7A8;
		CoD.ThreadIndexArray = 0x83B2E6F0;
		CoD.Com_InitThreadData = 0x82440568; // 0x8235CCE0;
		CoD.CG_Respawn = 0x826012E8;
	#elif defined(WAW)
		// Structs
		CoD.CEntity_t = 0x8268FC30;
		CoD.CG_t = 0x8268CD98;
		CoD.CGS_t = 0x0;
		CoD.ClientActive_t = 0x826E8BD8;
		CoD.ClientInfo_t = 0xB0C1D5CC;
		CoD.ClientNetInfo = 0x82736D68;
		CoD.UploadSpeedAddress = 0x852CC444;
		CoD.ScreenPlacement = 0x8274F580;
		CoD.UnknownClientStruct = 0x826A3F50;
		// Functions
		CoD.BG_AmmoForWeapon = 0x8217A230;
		CoD.BG_GetClipSize = 0x82140BA0;
		CoD.BG_GetSpreadForWeapon = 0x82141650;
		CoD.BG_GetWeaponDef = 0x824C6CF8;
		CoD.BG_UpdatePlayerDObj = 0x8212B748;
		CoD.CBuf_AddText = 0x8226FF08;
		CoD.CG_BulletHitEvent = 0x8219F640;
		CoD.CG_CalcNamePosition = 0x821598D8;
		CoD.CG_CompassCalcDimensions = 0x8214FCF8;
		CoD.CG_CompassDrawPlayerMap = 0x82150B48;
		CoD.CG_CompassDrawPlayerMapLocationSelector = 0x82150DF8;
		CoD.CG_CompassUpYawVector = 0x82151600;
		CoD.CG_GetWeaponIndexForName = 0x82144FF8;
		CoD.CG_DObjGetWorldTagPos = 0x82161EA8;
		CoD.CG_DrawGrenadePointer = 0x82156838;
		CoD.CG_DrawRotatedPic = 0x8215CEC0;
		CoD.CG_EntityEvent = 0x82165518;
		CoD.CG_GetPlayerViewOrigin = 0x8219BD50;
		CoD.CG_GoodRandomFloat = 0x822046D0;
		CoD.CG_LocationalTrace = 0x821A2C58;
		CoD.CG_WorldPosToCompass = 0x8214FB20;
		CoD.CL_AddReliableCommand = 0x821B4E88;
		CoD.CL_AllLocalClientsDisconnected = 0x821B4BF0;
		CoD.CL_CreateNewCommands = 0x821B0D40;
		CoD.CL_GetScreenDimensions = 0x821A3EE8;
		CoD.CL_RegisterFont = 0x821B7E40;
		CoD.CL_SendCommand = 0x821B0DF8;
		CoD.CL_SetStance = 0x821AD7D0;
		CoD.Com_GetClientDObj = 0x822758E8;
		CoD.DB_FindXAssetHeader = 0x821EA5D8;
		CoD.Dvar_GetBool = 0x826E8AFC;
		CoD.DynEntCl_DynEntImpactEvent = 0x821EDEF0;
		CoD.FireBulletPenetrate = 0x8219F6D0;
		CoD.FX_GetBoneOrientation = 0x822015D8;
		CoD.G_GetWeaponHitLocationMultiplier = 0x82214428;
		CoD.LiveStorage_SetStat = 0x822EBC50;
		CoD.Material_RegisterHandle = 0x824006F0;
		CoD.R_AddDObjToScene = 0x823F8360;
		CoD.R_DrawIndexedPrimitive = 0x82425030;
		CoD.R_DrawText = 0x821B7B18;
		CoD.R_EndFrame = 0x82402A00;
		CoD.R_TextHeight = 0x82400018;
		CoD.R_TextWidth = 0x823FFF28;
		CoD.SL_ConvertToString = 0x8233AD40;
		CoD.SL_GetString = 0x8233B770;
		CoD.Trace_GetEntityHitId = 0x82267928;
		CoD.UI_DrawHandlePic = 0x8215D048;
		CoD.UI_DrawText = 0x8229B7B0;
		CoD.UI_GetMapDisplayName = 0x8229F7A0;
		CoD.UI_GetGametypeDisplayName = 0x8229FA18;
		CoD.va = 0x822C38D8;
		// Other
		CoD.AddPatch(0x821B4C08, (PBYTE)"\x60\x00\x00\x00", 0x4); // Allow calls to CL_AllLocalClientsDisconnected from any thread
		CoD.AddPatch(0x821B4C18, (PBYTE)"\x60\x00\x00\x00", 0x4); // Same as above

		CoD.Dvar_ActiveAction = 0x826E345C;
		CoD.Dvar_CG_FOV = 0x82691678;
		CoD.Dvar_CG_ThirdPerson = 0x82510454;
		CoD.Dvar_UI_Mapname = 0x84FAA4D8;
		CoD.Dvar_PartyHostname = 0x827157FC;
		CoD.EntityTeamAddress = 0x124;
		CoD.ET_ITEM = 3;
		CoD.ET_ACTOR = 17;
		CoD.ET_MISSILE = 4;
		CoD.ET_PLAYER = 1;
		CoD.EV_AMMO_PICKUP = 12;
		CoD.EV_NOAMMO = 13;
		CoD.EV_RELOAD_ADDAMMO = 22;
		CoD.EV_RELOAD_START = 19;
		CoD.EV_RELOAD_END = 20;
		CoD.EV_OBITUARY = 92;
		CoD.EV_PUTAWAY_WEAPON = 25;
		CoD.ImageTrack = 0;
		CoD.NoRecoil = 0x821A0BFC;
		CoD.NoSway = 0x8214789C; 
		CoD.Radar = 0x0; // g_compassshowenemies
		CoD.XModelOffset = 0x64;
		CoD.XModelBoneOffset = 0x8;
		CoD.XModelPartClassification = 0x18;
		CoD.ScoresOffset = 0x5b668;
		CoD.ScoresSize = 0x28;
		CoD.ClientIndexOffset = 0x0;
		CoD.ScoreOffset = 0x4;
		CoD.PingOffset = 0x8;
		CoD.DeathsOffset = 0xc;
		CoD.TeamOffset = 0x10;
		CoD.KillsOffset = 0x14;
		CoD.RankOffset = 0x18;
		CoD.AssistsOffset = 0x1c;
		CoD.ServerIdAddress = 0x850D7728;
		CoD.PregameGTOffset = 0x852336B5;
		CoD.PrestigeOffset = 0x85349906;
		CoD.G_ThreadValues = 0x82F5C428;
		CoD.Sys_CreateThread = 0x8227FF50;
		CoD.ThreadIndexArray = 0x82F5C51C;
		CoD.Com_InitThreadData = 0x822C3998;
		CoD.CycleWeapPrimary = 0x8219E670;
		CoD.CG_Respawn = 0x8217BAA8;
	#elif defined(BO1)
		// Structs
		CoD.CEntity_t = 0x8296808C;
		CoD.CG_t = 0x82964EA0;
		CoD.CGS_t = 0x0;
		CoD.ClientActive_t = 0x829D9728;
		CoD.ClientUIActive_t = 0x829D5580;
		CoD.ClientInfo_t = 0xB4B98CF8;
		CoD.ClientNetInfo = 0x829F8450;
		CoD.UploadSpeedAddress = 0x84199200;
		CoD.ScreenPlacement = 0x82A06718;
		CoD.UnknownClientStruct = 0x82969190;
		// Functions
		CoD.BG_AmmoForWeapon = 0x82167B08;
		CoD.BG_GetClipSize = 0x82186198;
		CoD.BG_GetSpreadForWeapon = 0x82186940;
		CoD.BG_GetWeaponDef = 0x821919D8;
		CoD.BG_UpdatePlayerDObj = 0x8215E030;
		CoD.CBuf_AddText = 0x8233E8D8;
		CoD.CG_BulletHitEvent = 0x82223DA8;
		CoD.CG_CalcNamePosition = 0x821B38D8;
		CoD.CG_CompassCalcDimensions = 0x821A3610;
		CoD.CG_CompassDrawPlayerMap = 0x821A68C0;
		CoD.CG_CompassDrawPlayerMapLocationSelector = 0x821A75D0;
		CoD.CG_CompassUpYawVector = 0x821A54B0;
		CoD.CG_GetPlayerViewOrigin = 0x82223E48;
		CoD.CG_GetWeaponIndexForName = 0x82191FE0;
		CoD.CG_DObjGetWorldTagPos = 0x821BDB10;
		CoD.CG_DrawGrenadePointer = 0x821AFD58;
		CoD.CG_DrawRotatedPic = 0x821B86A8;
		CoD.CG_EntityEvent = 0x821C0818;
		CoD.CG_GoodRandomFloat = 0x822AC660;
		CoD.CG_LocationalTrace = 0x822296E0;
		CoD.CG_VehLocalClientUsingVehicle = 0x821A07C8;
		CoD.CG_WorldPosToCompass = 0x821A32C8;
		CoD.CL_AddReliableCommand = 0x822408E8;
		CoD.CL_AllLocalClientsDisconnected = 0x8255E2D0;
		CoD.CL_CreateNewCommands = 0x8223BED8;
		CoD.CL_GetScreenDimensions = 0x8222ABD8;
		CoD.CL_RegisterFont = 0x82243FA0;
		CoD.CL_SendCommand = 0x8223B8C8;
		CoD.CL_SetStance = 0x822362F0;
		CoD.Com_GetClientDObj = 0x82346710;
		CoD.DB_FindXAssetHeader = 0x82286370;
		CoD.Dvar_GetBool = 0x829D557C;
		CoD.DynEntCl_DynEntImpactEvent = 0x82292230;
		CoD.FireBulletPenetrate = 0x822252F8;
		CoD.FX_GetBoneOrientation = 0x822A8FC0;
		CoD.G_GetWeaponHitLocationMultiplier = 0x822C11D8;
		CoD.Material_RegisterHandle = 0x82624B78;
		CoD.R_AddDObjToScene = 0x8262CDE0;
		CoD.R_DrawIndexedPrimitive = 0x8267B830;
		CoD.R_DrawText = 0x8263F018;
		CoD.R_EndFrame = 0x82234B00;
		CoD.R_TextHeight = 0x823F6998;
		CoD.R_TextWidth = 0x8263D378;
		CoD.SL_ConvertToString = 0x82460778;
		CoD.SL_GetString = 0x82460E58;
		CoD.Trace_GetEntityHitId = 0x82334E58;
		CoD.UI_DrawHandlePic = 0x821B8780;
		CoD.UI_DrawText = 0x8238DEE8;
		CoD.UI_GetMapDisplayName = 0x82392230;
		CoD.UI_GetGametypeDisplayName = 0x82392508;
		CoD.va = 0x823EECA0;
		// Other
		CoD.AddPatch(0x824FA18C, (PBYTE)"\x60\x00\x00\x00", 4); // Disable EXE_INVALID_GAMERTAG callback
		CoD.AddPatch(0x823E24E8, (PBYTE)"\x60\x00\x00\x00", 4); // Disable cheat & write protection
		CoD.AddPatch(0x82223EE4, (PBYTE)"\x60\x00\x00\x00", 4); // CG_GetPlayerViewOrigin: Unable to get DObj for turret entity %i
	
		CoD.Dvar_CG_FOV = 0x82968374;
		CoD.Dvar_CG_ThirdPerson = 0x828D7118;
		CoD.Dvar_PartyHostname = 0x829F6B4C;
		CoD.Dvar_UI_Mapname = 0x83911050;
		CoD.AutowallFix1 = 0x82225C88;
		CoD.AutowallFix2 = 0x82225570;
		CoD.EntityTeamAddress = 0x1c0;
		CoD.ET_ITEM = 3;
		CoD.ET_ACTOR = 17;
		CoD.ET_HELICOPTER = 0xc;
		CoD.ET_MISSILE = 4;
		CoD.ET_PLAYER = 1;
		CoD.EV_AMMO_PICKUP = 12;
		CoD.EV_NOAMMO = 13;
		CoD.EV_RELOAD_ADDAMMO = 23;
		CoD.EV_RELOAD_START = 20;
		CoD.EV_RELOAD_END = 21;
		CoD.EV_PUTAWAY_WEAPON = 26;
		CoD.EV_OBITUARY = 100;
		CoD.ImageTrack = 0;
		CoD.NoRecoil = 0x82227624;
		CoD.NoSway = 0x82195200; // Not aiming
		CoD.NoSway2 = 0x82195D1C; // Aiming
		CoD.Radar = 0x821A81A0;
		CoD.XModelOffset = 0x78;
		CoD.XModelBoneOffset = 0x8;
		CoD.XModelPartClassification = 0x18;
		CoD.ScoresOffset = 0x1e7c0;
		CoD.ScoresSize = 0xd8;
		CoD.ClientIndexOffset = 0x0;
		CoD.ScoreOffset = 0x70;
		CoD.PingOffset = 0x64;
		CoD.DeathsOffset = 0x7c;
		CoD.TeamOffset = 0x0;
		CoD.KillsOffset = 0x74;
		CoD.RankOffset = 0x24;
		CoD.AssistsOffset = 0x78;
		CoD.ServerIdAddress = 0x829BE624;
		CoD.PregameGTOffset = 0;
		CoD.PrestigeOffset = 0x8408E7FC;
		CoD.G_ThreadValues = 0x834C0310;
		CoD.Sys_CreateThread = 0x82358B10;
		CoD.ThreadIndexArray = 0x834C040C;
		CoD.Com_InitThreadData = 0x823EED68;
		CoD.CycleWeapPrimary = 0x82226830;
		CoD.CG_Respawn = 0x821E0360;
	#elif defined(BO2)
		// Structs
		CoD.CEntity_t = 0x82BBC554;
		CoD.CG_t = 0x82BBAE68;
		CoD.CGS_t = 0x82BBAE44;
		CoD.ClientActive_t = 0x82C70F4C;
		CoD.ClientUIActive_t = 0x82C6FDC8;
		CoD.ClientInfo_t = 0xBA396710;
		CoD.ClientNetInfo = 0x82CAC3A0;
		CoD.UploadSpeedAddress = 0x84449014;
		CoD.ScreenPlacement = 0x82CBC168;
		CoD.UnknownClientStruct = 0x82BC3A20;
		// Functions
		CoD.AdjustSeed = 0x82697FC0;
		CoD.BG_AmmoForWeapon = 0x826BDD68;
		CoD.BG_GetClipSize = 0x826BDC70;
		CoD.BG_GetSpreadForWeapon = 0x826BB4E0;
		CoD.BG_GetWeaponDef = 0x826BF970;
		CoD.BG_UpdatePlayerDObj = 0x8267CC78;
		CoD.BulletTrace = 0x8224CDA8;
		CoD.CBuf_AddText = 0x824015E0;
		CoD.CG_BulletHitEvent = 0x82258520;
		CoD.CG_CalcNamePosition = 0x821C2678;
		CoD.CG_CompassCalcDimensions = 0x821B64B0;
		CoD.CG_CompassDrawPlayerMap = 0x823DAA88;
		CoD.CG_CompassUpYawVector = 0x821B7D10;
		CoD.CG_GetWeaponIndexForName = 0x822535D8;
		CoD.CG_DObjGetWorldTagPos = 0x821D03F0;
		CoD.CG_DrawGrenadePointer = 0x821BE100;
		CoD.CG_DrawRotatedPic = 0x821C7F58;
		CoD.CG_EntityEvent = 0x821D3D78;
		CoD.CG_GetPlayerViewOrigin = 0x822544B0;
		CoD.CG_GoodRandomFloat = 0x82696250;
		CoD.CG_LocationalTrace = 0x8225C568;
		CoD.CG_WorldPosToCompass = 0x821B6278;
		CoD.CL_AddReliableCommand = 0x822786E0;
		CoD.CL_AllLocalClientsDisconnected = 0x827504D0;
		CoD.CL_CreateNewCommands = 0x82261A40;
		CoD.CL_GetScreenDimensions = 0x82261D30;
		CoD.CL_RegisterFont = 0x82275F78;
		CoD.CL_SendCommand = 0x8225EAA8;
		CoD.CL_SetStance = 0x8225DDA0;
		CoD.Com_GetClientDObj = 0x82414578;
		CoD.DB_FindXAssetHeader = 0x822CAE50;
		CoD.FX_GetBoneOrientation = 0x822F9AB8;
		CoD.Dvar_GetBool = 0x82C6FDC4;
		CoD.DynEntCl_DynEntImpactEvent = 0x822DFB90;
		CoD.FireBulletPenetrate = 0x82258840;
		CoD.G_GetWeaponHitLocationMultiplier = 0x82325490;
		CoD.Material_RegisterHandle = 0x828B78F0;
		CoD.R_AddDObjToScene = 0x828A5AA0;
		CoD.R_DrawIndexedPrimitive = 0x82903D28;
		CoD.R_DrawText = 0x828B8BA0;
		CoD.R_EndFrame = 0x828B9F58;
		CoD.R_TextHeight = 0x82490390;
		CoD.R_TextWidth = 0x828B6FD8;
		CoD.SL_ConvertToString = 0x82532A18;
		CoD.SL_GetString = 0x82533528;
		CoD.Trace_GetEntityHitId = 0x823F7160;
		CoD.TransformSeed = 0x826961B8;
		CoD.UI_DrawHandlePic = 0x821C8030;
		CoD.UI_DrawText = 0x824510F8;
		CoD.UI_GetMapDisplayName = 0x82454BB0;
		CoD.UI_GetGametypeDisplayName = 0x82454ED8;
		CoD.va = 0x8249E440;
		CoD.XSessionCreate = 0x828464C8;
		CoD.XSessionDelete = 0x82846738;
		// Other
		CoD.AddPatch(0x825DE240, (PBYTE)"\x60\x00\x00\x00", 4); // Disable EXE_INVALID_GAMERTAG callback
		CoD.AddPatch(0x82498084, (PBYTE)"\x60\x00\x00\x00", 4); // Disable cheat & write protection
		CoD.Dvar_CG_FOV = 0x82BC2774;
		CoD.Dvar_CG_ThirdPerson = 0;
		CoD.Dvar_PartyHostname = 0x82CABC24;
		CoD.Dvar_UI_Mapname = 0;
		CoD.AutowallFix1 = 0x8225900C;
		CoD.AutowallFix2 = 0x82258A58;
		CoD.Autowall360Patch = 0x82258D60;
		CoD.EntityTeamAddress = 0x1ac;
		CoD.ET_ITEM = 3;
		CoD.ET_ACTOR = 16;
		CoD.ET_HELICOPTER = 0xc;
		CoD.ET_MISSILE = 4;
		CoD.ET_PLAYER = 1;
		CoD.EV_AMMO_PICKUP = 12;
		CoD.EV_NOAMMO = 13;
		CoD.EV_RELOAD_ADDAMMO = 23;
		CoD.EV_RELOAD_START = 22;
		CoD.EV_RELOAD_END = 21;
		CoD.EV_OBITUARY = 118;
		CoD.EV_PUTAWAY_WEAPON = 29;
		CoD.ImageTrack = 3;
		CoD.NoRecoil = 0x82259BC8;
		CoD.NoSway = 0x826C6E6C; // Not aiming
		CoD.NoSway2 = 0x826C7A7C; // Aiming
		CoD.Radar = 0x821B8FD4;
		CoD.ClassItemsAddress = 0x826A5FBC;
		
		CoD.XModelOffset = 0x78;
		CoD.XModelBoneOffset = 0x8;
		CoD.XModelPartClassification = 0x18;
		CoD.ScoresOffset = 0x21aec;
		CoD.ScoresSize = 0x120;
		CoD.ClientIndexOffset = 0x0;
		CoD.ScoreOffset = 0x94;
		CoD.PingOffset = 0x88;
		CoD.DeathsOffset = 0xa0;
		CoD.TeamOffset = 0x0;
		CoD.KillsOffset = 0x98;
		CoD.RankOffset = 0x24;
		CoD.AssistsOffset = 0x9c;
		CoD.ServerIdAddress = 0x82C15758;
		CoD.PregameGTOffset = 0;

		CoD.PreGameAntiFreezeDetour = 0x82717AE0;
		CoD.GameMessage1 = 0x821E6518;
		CoD.GameMessage2 = 0x821E6530;
		CoD.PrestigeOffset = 0x843491A4;
		CoD.SCR_UpdateFrame = 0x82282720;
		CoD.G_ThreadValues = 0x83AF71E8;
		CoD.Sys_CreateThread = 0x82428580;
		CoD.ThreadIndexArray = 0x83AF7330;
		CoD.Com_InitThreadData = 0x824284E0; //0x8249E508;
		CoD.CycleWeapPrimary = 0x82256AC0;
		CoD.CL_GetPredictedOriginForServerTime = 0x8227E210;
		CoD.CG_Respawn = 0x822ED980;
		CoD.BulletTrace = 0x8224CDA8;
		CoD.BG_GetSurfacePenetrationDepth = 0x826B0278;
		CoD.BG_AdvanceTrace = 0x826B0148;
		CoD.BG_WeaponHasPerk = 0x82691798;
	#elif defined(BO3)
		// Structs
		CoD.CEntity_t = 0x0;
		CoD.CG_t = 0x82E819D8;
		CoD.CGS_t = 0x82E819C0;
		CoD.ClientActive_t = 0x84475F74;
		CoD.ClientInfo_t = 0x0;
		CoD.ClientNetInfo = 0x8489E540; // Some ghetto ass shit 0x82E81FA0;
		CoD.UploadSpeedAddress = 0x844761A4;
		CoD.ScreenPlacement = 0x83088EC0;
		CoD.UnknownClientStruct = 0x844C1BC0;
		// Functions
		CoD.BG_AmmoForWeapon = 0x821F5358;
		CoD.BG_GetClipSize = 0x821F5260;
		CoD.BG_GetSpreadForWeapon = 0x821DC870;
		CoD.BG_GetWeaponDef = 0x82DE5E60;
		CoD.CBuf_AddText = 0x8263A6A0;
		CoD.CG_BulletHitEvent = 0x82271620;
		CoD.AimAssist_GetTagPos = 0x822A17E8;
		CoD.CG_CalcNamePosition = 0x82212AA0;
		CoD.CG_CompassCalcDimensions = 0;
		CoD.CG_CompassDrawPlayerMap = 0;
		CoD.CG_CompassUpYawVector = 0;
		CoD.CG_GetWeaponIndexForName = 0x0;
		CoD.CG_DrawGrenadePointer = 0x82211FD0;
		CoD.CG_DrawRotatedPic = 0x82210468;
		CoD.CG_EntityEvent = 0x822201C0;
		CoD.CG_GetPlayerViewOrigin = 0x822730E0;
		CoD.CG_LocationalTrace = 0x82285838;
		CoD.CG_WorldPosToCompass = 0;
		CoD.CL_AddReliableCommand = 0x822F59A8;
		CoD.CL_AllLocalClientsDisconnected = 0x82648970;
		CoD.CL_CreateNewCommands = 0x822E5800;
		CoD.CL_GetScreenDimensions = 0x83067810;
		CoD.CL_RegisterFont = 0x0;
		CoD.CL_SendCommand = 0x822F53C0;
		CoD.CL_SetStance = 0x822E2A88;
		CoD.Com_GetClientDObj = 0x83EC1440;
		CoD.DB_FindXAssetHeader = 0x82328110;
		CoD.GetClientEntity = 0x8215F168;
		CoD.Dvar_GetBool = 0x8253F6C0;
		CoD.DynEntCl_DynEntImpactEvent = 0x82345300;
		CoD.FireBulletPenetrate = 0x82272708;
		CoD.FX_GetBoneOrientation = 0x8235E0A8;
		CoD.Material_RegisterHandle = 0x0;
		CoD.R_DrawText = 0x824AE7C0;
		CoD.R_EndFrame = 0x822DF250;
		CoD.R_TextHeight = 0x82929000;
		CoD.R_TextWidth = 0x824A1000;
		CoD.SL_ConvertToString = 0x82E837AC;
		CoD.SL_GetString = 0x822D2248;
		CoD.UI_DrawHandlePic = 0;
		CoD.UI_GetMapDisplayName = 0x82650450;
		CoD.UI_GetGametypeDisplayName = 0x826B2030;
		CoD.va = 0x826BECE0;
		// Other
		CoD.AddPatch(0x8269F350, (PBYTE)"\x38\x60\x00\x00\x4E\x80\x00\x20", 8); // Bullet tracer fx
		CoD.Dvar_CG_FOV = 0;
		CoD.Dvar_CG_ThirdPerson = 0;
		CoD.Dvar_UI_Mapname = 0;
		CoD.ET_ITEM = 3;
		CoD.ET_ACTOR = 420;
		CoD.ET_MISSILE = 4;
		CoD.ET_PLAYER = 1;
		CoD.EV_OBITUARY = 132;
		CoD.EV_AMMO_PICKUP = 9;
		CoD.EV_NOAMMO = 10;
		CoD.EV_RELOAD_ADDAMMO = 19;
		CoD.EV_RELOAD_START = 16;
		CoD.EV_RELOAD_END = 17;
		CoD.EV_PUTAWAY_WEAPON = 23;
		CoD.ImageTrack = 0;
		CoD.NoRecoil = 0x82279CB8;
		CoD.NoSway = 0x82201008; 
		CoD.Radar = 0x8228BC78;
		CoD.XModelOffset = 0x8;
		CoD.XModelBoneOffset = 0x4;
		CoD.XModelPartClassification = 0x14;
		CoD.ScoresOffset = 0x1492c;
		CoD.ScoresSize = 0x148;
		CoD.ClientIndexOffset = 0x0;
		CoD.ScoreOffset = 0x90;
		CoD.PingOffset = 0x84;
		CoD.DeathsOffset = 0x9c;
		CoD.TeamOffset = 0x0;
		CoD.KillsOffset = 0x94;
		CoD.RankOffset = 0x24;
		CoD.AssistsOffset = 0x98;
		CoD.ServerIdAddress = 0x83067AE0;
		CoD.PregameGTOffset = 0;
		CoD.PrestigeOffset = 0x83A6C150;
		CoD.Sys_CreateThread = 0x826661C8;
		CoD.G_ThreadValues = 0x84041CB0; //not updated yet
		CoD.Com_InitThreadData = 0x826BD6D8; 
		CoD.CG_Respawn = 0x82235CC0;
	#endif

		bHasPatches = TRUE;
#else
		if (data) {
			memcpy(&CoD, data, sizeof(CheatPatches_s));
			bHasPatches = TRUE;
			return ERROR_SUCCESS;
		}
		else {
			bHasPatches = FALSE;
			return E_FAIL;
		}
#endif
		return ERROR_SUCCESS;
	}

	VOID DissolveAddresses() {
		if (bHasPatches) {
			XeCryptRandom((PBYTE)&CoD, sizeof(CheatPatches_s));
			bHasPatches = FALSE;
		}
	}

	VOID Initialize() {
		bHasPatches = FALSE;
#ifndef PATCH_SVR
		ZeroMemory(&CoD, sizeof(CheatPatches_s));
		CoD.NumPatches = 0;
		CoD.SizeOfPatches = 0;
		SetAddresses(NULL);
		bHasPatches = TRUE;
#endif
	}

	Cheats_s() {
		Initialize();
	}
	~Cheats_s() {
	}

#ifdef DUMP_BINS
	VOID DumpPatchBinary(char* file = NULL);
#endif

};