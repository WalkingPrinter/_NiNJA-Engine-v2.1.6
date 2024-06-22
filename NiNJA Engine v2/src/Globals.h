#pragma once
#include <xtl.h>
#include <setjmp.h>
#include "Utilities.h"
#include "Patches.h"
#include "Detour.h"
#include "Colors.h"
#include "Buttons.h"
#include "Entities.h"

using namespace std;

#pragma warning(disable: 4530)

enum eEspStates {
	ESP_OFF,
	ESP_FRIENDLY,
	ESP_ENEMY,
	ESP_BOTH
};

enum eAimTypes {
	AIM_TYPE_NORMAL,
	AIM_TYPE_SILENT,
	AIM_TYPE_HUMANIZED,
	AIM_TYPE_OFF
};

enum eBoxTypes {
	BOX_TYPE_CORNERS,
	BOX_TYPE_2D,
	BOX_TYPE_2D_FILLED,
	BOX_TYPE_3D,
//	BOX_TYPE_PYRAMID
};

enum eSnaplines {
	SNAPLINES_BOTTOM,
	SNAPLINES_CENTER,
	SNAPLINES_TOP
};

enum eCrosshairTypes {
	CROSSHAIR_OFF,
	CROSSHAIR_PLUS,
	CROSSHAIR_X,
	CROSSHAIR_CIRCLE
};

enum ePitchType {
	PITCH_OFF,
	PITCH_LOOK_DOWN,
	PITCH_UP_AND_DOWN,
	PITCH_LOOK_UP,
	PITCH_RANDOM,
	PITCH_DYNAMIC
};

enum eYawType {
	YAW_OFF,
	YAW_LEFT,
	YAW_RIGHT,
	YAW_FLIP,
	YAW_FACE_AWAY,
	YAW_RANDOM
};

enum eRollType {
	ROLL_OFF,
	ROLL_LEFT,
	ROLL_RIGHT,
	ROLL_FLIP,
	ROLL_RANDOM
};

enum eAutoCrouchMode {
	AUTO_CROUCH_OFF,
	AUTO_CROUCH_FIRING,
	AUTO_CROUCH_KILLABLE,
	AUTO_CROUCH_JITTER,
	AUTO_CROUCH_JITTERKILLABLE,
	AUTO_CROUCH_ON_DAMAGE,
	AUTO_CROUCH_PRIORITIZEDJITTER
};

static PCHAR pServerNames[] = {
	"arnold.%s.ninja",
	"bernard.%s.ninja"
};

namespace CG {
	// Main shit
	extern DWORD dwVersion;
	extern Buttons cButtons; // Button monitoring
	extern bool bInGameCached; // TRUE if we're in a game
	extern BOOL bLobbyInitialized; // TRUE if the lobby is initialized
	extern bool bIsMultiplayer; // TRUE if we're in multiplayer
	extern int iLocalClient; // Holds the ingame client index of localClient 0
	extern int iRealLocalClient; // Holds the STATIC client index of localClient 0 (even during killcams, spectating, etc)
	extern DWORD dwLastCmdTick; // Updated at the beginning of CL_CreateNewCommands hook, used for caching purposes
	extern Entities cEntities; // A class containing a vector of entities currently in the game
	extern CachedTags cTags; // A class containing an array of tags which we'll use to draw bone esp, and for the aimbot
	extern FLOAT fSoundVolume;
	extern bool bShouldAutoShoot; // Whether or not we should fire automatically
	extern bool bInKillstreak; // Whether or not we're currently using a killstreak
	extern int ShieldCache[8];
	extern int iLookingAtEntity;
	extern vector<int> cTopPlayers; // The clientIndexes of the top 3 players in the game, in order
	extern vector<QWORD> vWhitelisted;
	extern vector<QWORD> vBlacklisted;
	extern bool bLaunchCustomGTEntry;
	extern PVOID pCustomGTMenuOption;
	extern bool bResetGamertag;
	extern DWORD dwRapidFireDelay;
	extern PCHAR pGameTypeName;
	extern PCHAR pGameTypeDisplayName;
	extern DWORD dwCurrentStance;
	extern int iLocalHealth;
	extern bool bBrokenTeamCheck;
	extern DWORD dwExpectedTeam;
	extern bool bInvertTeams;
	extern DWORD dwTeamCheckTime;
	extern int iTestVal;
	extern BOOL bUseAltGeoIPApi;
	extern BOOL bUseXamGT;
	extern BOOL bInFrame;
	extern BOOL bLaunchExtraThread;
	extern BOOL bDisableSoundThread;
	extern BOOL bExtraThreadRunning;
	extern BOOL bTitleThreadRunning;
	extern BOOL bSwitchingTitles;
	extern BOOL bConnected; // TRUE if we are connected to a game server
	extern DWORD dwNumLivingEnemies;
	//extern jmp_buf jAutowallJump;
	//extern jmp_buf jVisibleJump;
	//extern jmp_buf jOrientationJump;
	extern BOOL bSpoofIP;
	extern BOOL bSendCustomCommand;
	extern DWORD dwSendCommandClientIndex;
	extern DWORD dwSendCommandType;
	extern DWORD dwExtraThreadId;
	extern BOOL bQuittingGame;
	extern BOOL bIsMapSelector;
	extern BOOL bJitterEnabled;

	// Cached resources
	extern DWORD dwWhite; // A pointer to the material "white"
	extern DWORD dwSmallFont; // A pointer to the font "SmallFont"
	extern DWORD dwArrowIcon; // A pointer to the arrow material
	extern DWORD dwScavPackWeaponID; // The current scavenger pack weapon index
	extern DWORD dwScavPackMaterial; // The current scavenger pack material


	// Main menu settings (self explanatory)
	extern BOOL bAutowall;
	extern BOOL bAutoShoot; 
	extern BOOL bFastReload;
	extern BOOL bRadar;
	extern BOOL bNoRecoil;
	extern BOOL bNoSway;
	extern BOOL bNoSpread;
	extern BOOL bRapidFire;
	extern BOOL bNeverHost;
	extern BOOL bShowPing;
	extern BOOL bUnlimitedClassItems;
	extern BOOL bTriggerbot;
	extern FLOAT fTriggerbotZoom;
	extern BOOL bAntiDerank;
	extern BOOL bShowRealNames;
	extern BOOL bFakeLag;
	extern FLOAT fFovValue;
	extern DWORD dwPacketSkipAmount;
	extern BOOL bThirdPerson;
	extern DWORD dwClassNum;
	extern DWORD dwAutoCrouch;
	extern BOOL bCustomSoundFX;
	extern BOOL bShowMenuInstructions;
	extern BOOL bShowMapInfo;
	extern BOOL bShowHost;
	extern BOOL bWallhack;
	extern DWORD dwPrestigeValue;
	extern BOOL bLogIPs;
	extern BOOL bPrestigeDisco;
	// Modded gamertag settings
	extern BOOL bRainbowGT;
	extern BOOL bSpoofGT;
	extern CHAR pModdedGamertags[10][33];
	extern BOOL bCycleGamertags;
	extern DWORD dwCycleDelay;
	extern DWORD dwRainbowDelay;
	extern BOOL bStealGTs;
	extern BOOL bTeleportKill;
	// Aimbot menu settings
	extern DWORD dwEnginePreset;
	extern DWORD dwAimbotType;
	extern BOOL bTargetBestBone;
	extern DWORD dwAimBone;
	extern DWORD dwAimButton;
	extern BOOL bAimbotWhenFiring;
	extern BOOL bOffscreenTargeting;
	extern FLOAT fHitTargetRadius; // How close to get our viewangles to the target before we call it good enough, for humanized aimbot mode. Smaller = more accurate
	extern FLOAT fMaxSpeedThreshold; // The spin speed at which we should snap our aimbot much faster to account for random spinning and things of that nature, when its less noticable beacuse of speed. Higher = more smoothing
	extern FLOAT fSmoothAmount; // The amount to smooth the legit aimbot by
	extern DWORD dwReactionDelay;
	// Spinbot settings
	extern BOOL bAntiAim;
	extern DWORD dwPitchSpinMode;
	extern DWORD dwYawSpinSpeed;
	extern DWORD dwYawSpinMode;
	extern DWORD dwRollSpinSpeed;
	extern DWORD dwRollSpinMode;
	// ESP menu settings
	extern DWORD dwESPBones;
	extern DWORD dwESPName;
	extern DWORD dwESPBoxes;
	extern DWORD dwESPBoxType;
	extern DWORD dwESPWeapon;
	extern BOOL bDrawExplosives;
	extern BOOL bDrawWeaponDrops;
	extern BOOL bDrawScavPacks;
	extern DWORD dwSnaplineType;
	extern DWORD dwSnaplines;
	extern DWORD dwDirectionalArrows;
	extern DWORD dwCrosshairType;
	extern DWORD dwESPDistance;
	extern FLOAT fESPNameScale;
	extern BOOL bDrawFPS;
	extern BOOL bShowEngineLogo;
	extern FLOAT fDropDrawDistance;
	// Colors
	extern FLOAT cFriendlyEspColor[4];
	extern FLOAT cEnemyEspColor[4];
	extern FLOAT cVisibleEspColor[4];
	extern FLOAT cWallbangableEspColor[4];
	extern FLOAT cSelectedTextColor[4];
	extern FLOAT cPreferredEspColor[4];
	extern FLOAT cWhitelistedEspColor[4];
	extern FLOAT cTopRightTextColor[4];
	extern FLOAT cMenuBackgroundColor[4];
	extern FLOAT cMenuSliderColor[4];
	extern FLOAT cMenuTextColor[4];
	extern FLOAT cTopBottomColor[4];
	extern FLOAT cTitleTextColor[4];
	extern FLOAT cDescriptionTextColor[4];
	extern FLOAT cTopRightBackgroundColor[4];
	extern FLOAT cScrollbarColor[4];
	extern FLOAT cMenuBorderColor[4];
	extern FLOAT cControlColor[4];

	extern GeoIpInfo cLocationInfo;
	extern CHAR pIPAddress[20];
	
	
	extern FLOAT fSelectedTextColorIndex;
	// Menu settings
	extern DWORD dwMenuPos[2];
	extern BOOL bMenuOpen;
	extern BOOL bAnalogControl;
	extern BOOL bMovingMenu;


	extern FLOAT fBorderWidth;
	extern FLOAT fTopBottomBoxHeight;
	extern FLOAT fFontScale;
	extern FLOAT fMenuFontHeight;
	extern FLOAT fMenuOptionHeight;
	extern FLOAT fMenuTextPadding;
	extern FLOAT fMenuWidth, fMenuHeight;
	extern FLOAT fOptionIndentAmount;
	extern FLOAT fOptionPadding;
	extern FLOAT fOptionSpacing;

	extern MemoryManager mExtendedEntityManager;

	FLOAT GetFontScale();

	VOID Terminate();

	VOID Initialize();
	VOID Shutdown();
};

// This should make things a bit more confusing in ida :)
class vGlobals {
	virtual VOID Initialize() {
	}
	virtual VOID Shutdown() {
	}
};

// Global variables which need to be accessible from multiple code files
class Globals : public vGlobals
{
public:
	DWORD dwCurrentTitle; // Cached copy of the current title ID
	DWORD dwTickCount; // Cached copy of the current tick count
	DWORD dwScreenMaxX, dwScreenMaxY; // Max screen size
	DWORD dwScreenCenterX, dwScreenCenterY; // Screen center position

	HANDLE hME; // Handle of our process

	PLDR_DATA_TABLE_ENTRY pDataTableEntry; // A pointer to a PLDR_DATA_TABLE_ENTRY with info pertaining to the current title

	BOOL bSoundLoopRunning; // TRUE if the blocking loop is active
	BOOL bConstantLoopRunning; // TRUE if the constant loop is active
	BOOL bExternUnload; // TRUE if unloaded from a tool, FALSE if unloaded from a title change
	BOOL bGameInitialized; // TRUE if the game has been fully initialized
	BOOL bIsDevkit; // TRUE if we're on a dev kernel
	BOOL bShutdownCheats; // TRUE if we should immediately shutdown our title hooks
	BOOL bTerminateProcess; // TRUE if our process should halt
	BOOL bSignedIntoLive; // TRUE if we're signed into a live enabled profile
	BOOL bHasReadConfig; // TRUE if we have read our config from file at least once

	Cheats_s *cPatches; // A pointer to our encrypted patchset used for our cheats

	//MemoryManager mAsyncManager;

	VOID Initialize(); // Initialize our global variables
	VOID Shutdown(); // Uninitialize our global variables
};

extern Globals NiNJA;