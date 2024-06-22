#include "Globals.h"
#include "SecureStrings.h"

// GameSettings class setup
Buttons CG::cButtons = Buttons();

//jmp_buf CG::jAutowallJump;
//jmp_buf CG::jVisibleJump;
//jmp_buf CG::jOrientationJump;

DWORD CG::dwVersion = 0;
bool  CG::bInGameCached = false;
BOOL CG::bLobbyInitialized = FALSE;
bool CG::bIsMultiplayer = FALSE;
int CG::iLocalClient = -1;
int CG::iRealLocalClient = -1;
DWORD CG::dwLastCmdTick = 0;
Entities CG::cEntities = Entities();
CachedTags CG::cTags = CachedTags();
FLOAT CG::fSoundVolume = 4.0f;
bool CG::bShouldAutoShoot = false;
bool CG::bInKillstreak = false;
int CG::ShieldCache[8] = { 0 };
int CG::iLookingAtEntity = -1;
vector<int> CG::cTopPlayers;
vector<QWORD> CG::vWhitelisted;
vector<QWORD> CG::vBlacklisted;
bool CG::bLaunchCustomGTEntry = false;
PVOID CG::pCustomGTMenuOption = NULL;
bool CG::bResetGamertag = false;
PCHAR CG::pGameTypeName = "NULL";
PCHAR CG::pGameTypeDisplayName;
DWORD CG::dwCurrentStance;
int CG::iLocalHealth = 100;
bool CG::bBrokenTeamCheck = false;
DWORD CG::dwExpectedTeam = 0;
bool CG::bInvertTeams = false;
DWORD CG::dwTeamCheckTime = 0;
int CG::iTestVal = 1;
BOOL CG::bUseAltGeoIPApi = FALSE;
BOOL CG::bUseXamGT = FALSE;
BOOL CG::bInFrame = FALSE;
BOOL CG::bLaunchExtraThread = FALSE;
BOOL CG::bDisableSoundThread = FALSE;
BOOL CG::bExtraThreadRunning = FALSE;
BOOL CG::bTitleThreadRunning = FALSE;
BOOL CG::bSwitchingTitles = FALSE;
BOOL CG::bConnected = FALSE;
DWORD CG::dwNumLivingEnemies = 0;
BOOL CG::bSpoofIP = FALSE;
BOOL CG::bSendCustomCommand = FALSE;
DWORD CG::dwSendCommandClientIndex = 0;
DWORD CG::dwSendCommandType = 0;
DWORD CG::dwExtraThreadId = 0;
BOOL CG::bQuittingGame = FALSE;
BOOL CG::bIsMapSelector = FALSE;
BOOL CG::bJitterEnabled = FALSE;
BOOL CG::bTeleportKill = FALSE;

// Cached resources
DWORD CG::dwWhite = NULL;
DWORD CG::dwSmallFont = NULL;
DWORD CG::dwArrowIcon = NULL;
DWORD CG::dwScavPackWeaponID = 0;
DWORD CG::dwScavPackMaterial = NULL;

// Main menu settings
BOOL CG::bAutowall = TRUE;
BOOL CG::bAutoShoot = FALSE;
BOOL CG::bFastReload = TRUE;
BOOL CG::bRadar = TRUE;
BOOL CG::bNoRecoil = TRUE;
BOOL CG::bNoSway = TRUE;
BOOL CG::bNoSpread = TRUE;
BOOL CG::bRapidFire = TRUE;
BOOL CG::bNeverHost = FALSE;
BOOL CG::bShowPing = TRUE;
BOOL CG::bUnlimitedClassItems = TRUE;
BOOL CG::bTriggerbot = FALSE;
#if defined(MW2)
BOOL CG::bAntiDerank = FALSE;
#else
BOOL CG::bAntiDerank = TRUE;
#endif
BOOL CG::bShowRealNames = TRUE;
FLOAT CG::fTriggerbotZoom = 0.6f;
BOOL CG::bFakeLag = FALSE;
FLOAT CG::fFovValue = 65.0f;
DWORD CG::dwPacketSkipAmount = 8;
BOOL CG::bThirdPerson = FALSE;
DWORD CG::dwClassNum = 0;
DWORD CG::dwRapidFireDelay = 0;
BOOL CG::bShowMenuInstructions = TRUE;
BOOL CG::bShowMapInfo = TRUE;
BOOL CG::bShowHost = TRUE;
BOOL CG::bWallhack = TRUE;
DWORD CG::dwPrestigeValue = 0;
BOOL CG::bLogIPs = FALSE;
BOOL CG::bPrestigeDisco = FALSE;
// Modded gamertag settings
BOOL CG::bRainbowGT = FALSE;
BOOL CG::bSpoofGT = FALSE;
CHAR CG::pModdedGamertags[10][33] = { 0 };
BOOL CG::bCycleGamertags = FALSE;
DWORD CG::dwRainbowDelay = 100;
DWORD CG::dwCycleDelay = 1000;
BOOL CG::bStealGTs = FALSE;
DWORD CG::dwAutoCrouch = 0;
BOOL CG::bCustomSoundFX = TRUE;
// Aimbot menu settings
DWORD CG::dwEnginePreset = 0;
DWORD CG::dwAimbotType = AIM_TYPE_SILENT; 
BOOL CG::bTargetBestBone = TRUE; 
DWORD CG::dwAimBone = TAG_HEAD;
DWORD CG::dwAimButton = BUTTON_LT;
BOOL CG::bAimbotWhenFiring = TRUE;
FLOAT CG::fHitTargetRadius = 0.65f;
FLOAT CG::fMaxSpeedThreshold = 9.0f;
FLOAT CG::fSmoothAmount = 10.0f;
BOOL CG::bOffscreenTargeting = TRUE;
DWORD CG::dwReactionDelay = 300.0f;
// Spinbot settings
BOOL CG::bAntiAim = FALSE;
DWORD CG::dwPitchSpinMode = 0;
DWORD CG::dwYawSpinSpeed = 50;
DWORD CG::dwYawSpinMode = 0;
DWORD CG::dwRollSpinSpeed = 50;
DWORD CG::dwRollSpinMode = 0;
// ESP menu settings
DWORD CG::dwESPBones = ESP_BOTH;
DWORD CG::dwESPName = ESP_BOTH;
DWORD CG::dwESPBoxes = ESP_ENEMY;
DWORD CG::dwESPBoxType = BOX_TYPE_2D_FILLED;
DWORD CG::dwESPWeapon = ESP_ENEMY;
BOOL CG::bDrawExplosives = FALSE;
BOOL CG::bDrawWeaponDrops = FALSE;
BOOL CG::bDrawScavPacks = TRUE;
DWORD CG::dwSnaplineType = SNAPLINES_BOTTOM;
DWORD CG::dwSnaplines = ESP_ENEMY;
DWORD CG::dwDirectionalArrows = ESP_ENEMY;
DWORD CG::dwCrosshairType = CROSSHAIR_PLUS;
DWORD CG::dwESPDistance = ESP_BOTH;
FLOAT CG::fESPNameScale = 0.7f;
BOOL CG::bDrawFPS = TRUE;
BOOL CG::bShowEngineLogo = TRUE;
FLOAT CG::fDropDrawDistance = 100.0f;
// Menu settings
DWORD CG::dwMenuPos[2] = { 4200, 7100 };
BOOL CG::bMenuOpen = FALSE;
BOOL CG::bMovingMenu = false;
BOOL CG::bAnalogControl = FALSE;


// Colors
FLOAT CG::cFriendlyEspColor[4] = SETCOLOR(Colors::Green);
FLOAT CG::cEnemyEspColor[4] = SETCOLOR(Colors::Yellow);
FLOAT CG::cVisibleEspColor[4] = SETCOLOR(Colors::Red);
FLOAT CG::cWallbangableEspColor[4] = SETCOLOR(Colors::Purple);
FLOAT CG::cPreferredEspColor[4] = SETCOLOR(Colors::White);
FLOAT CG::cWhitelistedEspColor[4] = SETCOLOR(Colors::Cyan);
FLOAT CG::cTopRightTextColor[4] = SETCOLOR(Colors::White);
FLOAT CG::cTopRightBackgroundColor[4] = SETCOLOR(Colors::BlackTrans);
FLOAT CG::cSelectedTextColor[4] = SETCOLOR(Colors::Cyan);
FLOAT CG::cMenuBackgroundColor[4] = SETCOLOR(Colors::BlackTrans);
FLOAT CG::cMenuSliderColor[4] = SETCOLOR(Colors::PurpleTrans);
FLOAT CG::cMenuTextColor[4] = SETCOLOR(Colors::White);
FLOAT CG::cTopBottomColor[4] = SETCOLOR(Colors::PurpleTrans);
FLOAT CG::cTitleTextColor[4] = SETCOLOR(Colors::White);
FLOAT CG::cDescriptionTextColor[4] = SETCOLOR(Colors::White);
FLOAT CG::cScrollbarColor[4] = SETCOLOR(Colors::WhiteTrans);
FLOAT CG::cMenuBorderColor[4] = SETCOLOR(Colors::WhiteTrans);
FLOAT CG::cControlColor[4] = SETCOLOR(Colors::Cyan);

FLOAT CG::fSelectedTextColorIndex = 75.0f;

FLOAT CG::fBorderWidth = 2.0f;
FLOAT CG::fTopBottomBoxHeight;
FLOAT CG::fFontScale = 0.9f;
FLOAT CG::fMenuFontHeight;
FLOAT CG::fMenuOptionHeight;
FLOAT CG::fMenuTextPadding = 6.0f;
FLOAT CG::fMenuHeight, CG::fMenuWidth = 380.0f;
FLOAT CG::fOptionIndentAmount = 4.0f;
FLOAT CG::fOptionPadding;
FLOAT CG::fOptionSpacing = 2.0f;


GeoIpInfo CG::cLocationInfo;
CHAR CG::pIPAddress[20] = { 0 };

MemoryManager CG::mExtendedEntityManager;
BYTE eManagerBytes[sizeof(ExtendedEntity) * (__MAXFULLENTS__ + 1)];
VOID CG::Initialize() {
	// Setup a buffer for our ExtendedEntity members
	DWORD eManager = (DWORD)&eManagerBytes;// = (DWORD)XPhysicalAlloc(, MAXULONG_PTR, NULL, PAGE_READWRITE);
	if (eManager) {
		mExtendedEntityManager.SetupManager(eManager, sizeof(ExtendedEntity) * (__MAXFULLENTS__ + 1));
		DbgPrint("Success setting up extended entity manager!");
	}
	else {
		DbgPrint("Failed to initialize eManager!");
	}
}

VOID CG::Shutdown() {
	// Free the memory we allocated for our EntendedEntity objects
	//XSAFE_FREE((PVOID)mExtendedEntityManager.memory);

	// Call the destructor
	mExtendedEntityManager.~MemoryManager();
}

bool bTerminating = false;
VOID CG::Terminate() {
	// Sleep until our cheats successfully shutdown until we allow the title to exit
	if (NiNJA.bGameInitialized && !bTerminating) {
		bTerminating = true;
		DbgPrint("Title is terminating!");

		// Mark us as switching titles
		CG::bSwitchingTitles = TRUE;

		// Wait for any http requests to finish up
		bIgnoreHttpRequests = TRUE;
		while (bIsAnyRequestBusy) {
			Sleep(33);
		}

		DbgPrint("No more web requests are busy!");

		// Then signal the cheats to shutdown
		NiNJA.bShutdownCheats = TRUE;
		while (NiNJA.bShutdownCheats) {
			Sleep(33);
		}

		DbgPrint("Cheats are fully shutdown!");

		// Wait for our extra threads to complete
		while (NiNJA.bSoundLoopRunning || CG::bTitleThreadRunning) {
			Sleep(33);
		}

		// Give our extra thread 1 second to shut down before we just move on anyway
		DWORD dwExtraThreadWait = GetTickCount() + 1000;
		while (CG::bExtraThreadRunning && GetTickCount() < dwExtraThreadWait) {
			Sleep(33);
		}

		DbgPrint("Our extra thread has exited!");
	}
}

FLOAT CG::GetFontScale() {
#if defined(BO3) || defined(WAW)
	return CG::fFontScale - 0.1;
#else
	return CG::fFontScale;
#endif
}


VOID Globals::Initialize() {
	// Globals class setup
	dwCurrentTitle = 0;
	dwTickCount = 0;
	dwScreenMaxX = 0;
	dwScreenMaxY = 0;
	dwScreenCenterX = 0;
	dwScreenCenterY = 0;
	bSoundLoopRunning = FALSE;
	bExternUnload = FALSE;
	bGameInitialized = FALSE;
	bShutdownCheats = FALSE;
	bTerminateProcess = FALSE;
	bHasReadConfig = FALSE;

	// Initialize the current game
	CG::Initialize();

	// Initialize our cheat patches
	cPatches = Not((Cheats_s*)XPhysicalAlloc(sizeof(Cheats_s), MAXULONG_PTR, NULL, PAGE_READWRITE)); //Not(new Cheats_s());
	Not(cPatches)->Initialize();

	//// Allocate some memory for our async manager
	//mAsyncManager.SetupManager((DWORD)XPhysicalAlloc(0x31000, MAXULONG_PTR, 2048, PAGE_READWRITE), 0x31000);
	
	DbgPrint("Initialized!");
}

VOID Globals::Shutdown() {
	// Shutdown the current game
	CG::Shutdown();

	//XSAFE_FREE((PVOID)mAsyncManager.memory);

	// Delete our cPatches object to free up memory
	XSAFE_FREE(Not(cPatches));
}

Globals NiNJA;