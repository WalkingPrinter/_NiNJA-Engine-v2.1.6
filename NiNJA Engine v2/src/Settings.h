#pragma once
// Uncomment to enable anti-hack features
#define ANTIHACK

// Uncomment to enable encrypted strings
#define CRYPTED_STRINGS

// Uncomment to dump patch binaries for the current game
//#define DUMP_BINS

// Uncomment to enable serverside patches
#define PATCH_SVR

// Uncomment to enable debug messages
//#define DEBUG

// Uncomment to enable the admin menu
//#define ADMIN

// Prevent accidents :)
#if defined(PATCH_SVR)
#undef DEBUG
#undef DUMP_BINS
#undef DBG_PRINT
#undef ADMIN
#define ANTIHACK
#endif

// Global settings
#define __MAXENTITIES__ 1024
#define __MAXFULLENTS__ 64

// Used for disabling menu options :)
enum eTitleFlags {
	TITLE_MW1 = 1,
	TITLE_MW2 = 2,
	TITLE_MW3 = 4,
	TITLE_BO1 = 8,
	TITLE_BO2 = 16,
	TITLE_BO3 = 32,
	TITLE_AW = 64,
	TITLE_GHOSTS = 128,
	TITLE_WAW = 256
};

// The default setting for all menu options, to run on all titles
#define FLAG_ALL_COD (TITLE_MW1|TITLE_MW2|TITLE_MW3|TITLE_BO1|TITLE_BO2|TITLE_BO3|TITLE_AW|TITLE_GHOSTS|TITLE_WAW)

// The game to build for, Games: MW1, MW2, MW3, BO1, BO2, BO3, AW, GHOSTS, WAW
// NOTE: This isn't used except for if we're debugging! The game name is defined in the c++ preprocesspor definitions section of the project properties
//#define BO2

#if defined(MW1)
	#define __GAMENAME__ "MW1"
	#define __MAXCLIENTS__ 18
	#define __TITLEFLAG__ 1
	#define __TITLEID__ 0x412700F6 //0x415607E6
	#define __MSGRIDENT__ 0x83721490
	#define __TIMEDATESTAMP__ 0x4A78A577
	#define __SMALLFONT__ "fonts/smallfont"
	#define __ARROWICON__  "compassping_player"
	#define __FFANAME__ "dm"
	#define __SCORECMD__ "score"
	#define __SCAVENGERWEAPNAME__ ""
	#define __SCAVENGERHUDNAME__ ""
	#define __LEFTGUNADDITAVE__ 10
	#define __WORLDENTNUM__ 1022
	#define __TRACEMASK__ 0x2806831
	#define __INGAMESWITCHDELAY__ 1500
	#define __THREADINDEX__ 2
	#define __MAXPRESTIGE__ 10
	#define __THREADSTACKSIZE__ 0
	#define __TRACECORE__ 4
	#define __THREADCORE__ 3
	#define ShieldAntiAimAdditave -60.0f
	#define __MINPITCH__ -60.0f
	#define __MAXPITCH__ 60.0f
	enum eStance {
		STANCE_STAND = 0x0,
		STANCE_CROUCH = 0x200,
		STANCE_PRONE = 0x100
	};
#elif defined(MW2)
	#define __GAMENAME__ "MW2"
	#define __MAXCLIENTS__ 18
	#define __TITLEFLAG__ 2
	#define __TITLEID__ 0x41270F07 //0x41560817
	#define __MSGRIDENT__ 'MW2_'
	#define __TIMEDATESTAMP__ 0x5B11C269 // TU8 0x50749AC3
	#define __SMALLFONT__ "fonts/smallfont"
	#define __ARROWICON__  "compassping_player"
	#define __FFANAME__ "dm"
	#define __SCORECMD__ "s"
	#define __SCAVENGERWEAPNAME__ "scavenger_bag_mp"
	#define __SCAVENGERHUDNAME__ "scavenger_pickup"
	#define __LEFTGUNADDITAVE__ 10
	#define __WORLDENTNUM__ 1022
	#define __TRACEMASK__ 0x2806831
	#define __INGAMESWITCHDELAY__ 1500
	#define __THREADINDEX__ 2
	#define __MAXPRESTIGE__ 10
	#define __THREADSTACKSIZE__ 0
	#define __TRACECORE__ 4
	#define __THREADCORE__ 3
	#define ShieldFrontUp 24.0f
	#define ShieldFrontDown 29.0f
	#define ShieldFrontRight 16.0f
	#define ShieldFrontLeft 16.0f
	#define ShieldFrontDepthTopLeft -1.0f
	#define ShieldFrontDepthTopRight -1.0f
	#define ShieldFrontDepthBottomLeft -1.0f
	#define ShieldFrontDepthBottomRight -1.0f
	#define ShieldFrontXTweak 0.0f
	#define ShieldFrontYTweak 0.0f
	#define ShieldRearLeft 23.0f
	#define ShieldRearRight 29.0f
	#define ShieldRearUp 19.0f
	#define ShieldRearDown 15.0f
	#define ShieldRearDepthTopLeft 1.5f
	#define ShieldRearDepthTopRight -12.0f
	#define ShieldRearDepthBottomLeft 2.0
	#define ShieldRearDepthBottomRight -10.0f
	#define ShieldRearXTweak 0.0f
	#define ShieldRearYTweak 0.0f
	#define ShieldAntiAimAdditave -30.0f
	#define __MINPITCH__ -45.0f
	#define __MAXPITCH__ 60.0f
	enum eStance {
		STANCE_STAND = 0x0,
		STANCE_CROUCH = 0x200,
		STANCE_PRONE = 0x100
	};
#elif defined(MW3)
	#define __GAMENAME__ "MW3"
	#define __MAXCLIENTS__ 18
	#define __TITLEFLAG__ 4
	#define __TITLEID__ 0x41270FDB //0x415608CB
	#define __MSGRIDENT__ 0x828547C0
	#define __TIMEDATESTAMP__ 0x5B10A113 //tu23 0x506A5202
	#define __SMALLFONT__ "fonts/smallfont"
	#define __ARROWICON__  "compassping_player"
	#define __FFANAME__ "dm"
	#define __SCORECMD__ "s"
	#define __SCAVENGERWEAPNAME__ "scavenger_bag_mp"
	#define __SCAVENGERHUDNAME__ "scavenger_pickup"
	#define __LEFTGUNADDITAVE__ 10
	#define __WORLDENTNUM__ 1022
	#define __TRACEMASK__ 0x2806831
	#define __INGAMESWITCHDELAY__ 1500
	#define __THREADINDEX__ 2
	#define __MAXPRESTIGE__ 20
	#define __THREADSTACKSIZE__ 0
	#define __TRACECORE__ 4
	#define __THREADCORE__ 3
	#define ShieldFrontUp 24;
	#define ShieldFrontDown 29;
	#define ShieldFrontRight 15;
	#define ShieldFrontLeft 15;
	#define ShieldFrontDepthTopLeft -2.0f
	#define ShieldFrontDepthTopRight -2.0f
	#define ShieldFrontDepthBottomLeft -2.0f
	#define ShieldFrontDepthBottomRight -2.0f
	#define ShieldFrontXTweak 0.0f
	#define ShieldFrontYTweak 0.0f

	#define ShieldRearLeft 25.0f
	#define ShieldRearRight 27.0f
	#define ShieldRearUp 21.5f
	#define ShieldRearDown 9.0f
	#define ShieldRearDepthTopLeft 2.5f
	#define ShieldRearDepthTopRight 0.5f
	#define ShieldRearDepthBottomLeft -4.5f
	#define ShieldRearDepthBottomRight -5.5f
	#define ShieldRearXTweak 0.0f
	#define ShieldRearYTweak 0.0f
	#define ShieldAntiAimAdditave -30.0f
	#define __MINPITCH__ -60.0f
	#define __MAXPITCH__ 60.0f
	enum eStance {
		STANCE_STAND = 0x0,
		STANCE_CROUCH = 0x200,
		STANCE_PRONE = 0x100
	};
#elif defined(BO1)
	#define __GAMENAME__ "BO1"
	#define __MAXCLIENTS__ 18
	#define __TITLEFLAG__ 8
	#define __TITLEID__ 0x41270F45 //0x41560855
	#define __MSGRIDENT__ 0x83573590
	#define __TIMEDATESTAMP__ 0x4E542876
	#define __SMALLFONT__ "fonts/smallfont"
	#define __ARROWICON__  "compassping_player"
	#define __FFANAME__ "ffa"
	#define __SCORECMD__ "score"
	#define __SCAVENGERWEAPNAME__ "scavenger_item_mp"
	#define __SCAVENGERHUDNAME__ "hud_scavenger_pickup"
	#define __LEFTGUNADDITAVE__ 0
	#define __WORLDENTNUM__ 1022
	#define __TRACEMASK__ 0x280E833
	#define __INGAMESWITCHDELAY__ 1500
	#define __THREADINDEX__ 2
	#define __MAXPRESTIGE__ 15
	#define __THREADSTACKSIZE__ 0
	#define __TRACECORE__ 4
	#define __THREADCORE__ 5
	#define ShieldAntiAimAdditave -60.0f
	#define __MINPITCH__ -60.0f
	#define __MAXPITCH__ 60.0f
	enum eStance {
		STANCE_STAND = 0,
		STANCE_CROUCH = 0x400000,
		STANCE_PRONE = 0x800000
	};
#elif defined(BO2)
	#define __GAMENAME__ "BO2"
	#define __MAXCLIENTS__ 18
	#define __TITLEFLAG__ 16
	#define __TITLEID__ 0x41270FD3 //0x415608C3
	#define __MSGRIDENT__ 0x83434530
	#define __TIMEDATESTAMP__ 0x53643D71
	#define __SMALLFONT__ "fonts/720/smallFont"
	#define __ARROWICON__  "compassping_player"
	#define __FFANAME__ "dm"
	#define __SCORECMD__ "score"
	#define __SCAVENGERWEAPNAME__ "scavenger_item_mp"
	#define __SCAVENGERHUDNAME__ "hud_scavenger_pickup"
	#define __LEFTGUNADDITAVE__ 0
	#define __WORLDENTNUM__ 1022
	#define __TRACEMASK__ 0x280E833
	#define __INGAMESWITCHDELAY__ 1500
	#define __THREADINDEX__ 2
	#define __MAXPRESTIGE__ 15
	#define __THREADSTACKSIZE__ 0
	#define __TRACECORE__ 4
	#define __THREADCORE__ 3
	#define ShieldFrontUp 27.0f
	#define ShieldFrontDown 27.0f
	#define ShieldFrontRight 16.0f
	#define ShieldFrontLeft 16.0f
	#define ShieldFrontDepthTopLeft -2.0f
	#define ShieldFrontDepthTopRight -2.0f
	#define ShieldFrontDepthBottomLeft -2.0f
	#define ShieldFrontDepthBottomRight -2.0f
	#define ShieldFrontXTweak 0.0f
	#define ShieldFrontYTweak 0.0f

	#define ShieldRearLeft 26.0f
	#define ShieldRearRight 22.0f
	#define ShieldRearUp 14.0f
	#define ShieldRearDown 16.0f
	#define ShieldRearDepthTopLeft -2.0f
	#define ShieldRearDepthTopRight -2.0f
	#define ShieldRearDepthBottomLeft -2.0f
	#define ShieldRearDepthBottomRight -2.0f
	#define ShieldRearXTweak 0.0f
	#define ShieldRearYTweak -0.0f
	#define ShieldAntiAimAdditave -30.0f
	#define __MINPITCH__ -45.0f
	#define __MAXPITCH__ 60.0f
	enum eStance {
		STANCE_STAND = 0,
		STANCE_CROUCH = 0x400000,
		STANCE_PRONE = 0x800000
	};
#elif defined(BO3)
	#define __GAMENAME__ "BO3"
	#define __MAXCLIENTS__ 12
	#define __TITLEFLAG__ 32
	#define __TITLEID__ 0x41270E0D //0x4156091D
	#define __MSGRIDENT__ 0x94234240
	#define __TIMEDATESTAMP__ 0x5717EF77
	#define __SMALLFONT__ "fonts/smallfont"
	#define __ARROWICON__  "compassping_player"
	#define __FFANAME__ "ffa"
	#define __SCORECMD__ "score"
	#define __SCAVENGERWEAPNAME__ ""
	#define __SCAVENGERHUDNAME__ ""
	#define __LEFTGUNADDITAVE__ 0
	#define __WORLDENTNUM__ 702
	#define __TRACEMASK__ 0x280B001
	#define __INGAMESWITCHDELAY__ 1500
	#define __THREADINDEX__ 2
	#define __MAXPRESTIGE__ 11
	#define __THREADSTACKSIZE__ 0
	#define __TRACECORE__ 4
	#define __THREADCORE__ 3
	#define ShieldAntiAimAdditave -60.0f
	#define __MINPITCH__ -60.0f
	#define __MAXPITCH__ 60.0f
	enum eStance {
		STANCE_STAND = 0,
		STANCE_CROUCH = 0x400000,
		STANCE_PRONE = 0x800000
	};
#elif defined(AW)
	#define __GAMENAME__ "AW"
	#define __MAXCLIENTS__ 12
	#define __TITLEFLAG__ 64
	#define __TITLEID__ 0x41270E04 //0x41560914
	#define __MSGRIDENT__ 0x80122410
	#define __TIMEDATESTAMP__ 0x55C2C3C7
	#define __SMALLFONT__ "fonts/bodyFont"
	#define __ARROWICON__  "cb_compassping_arrow"
	#define __FFANAME__ "dm"
	#define __SCORECMD__ "s"
	#define __SCAVENGERWEAPNAME__ "scavenger_bag_mp"
	#define __SCAVENGERHUDNAME__ "scavenger_pickup"
	#define __LEFTGUNADDITAVE__ 10
	#define __WORLDENTNUM__ 768
	#define __TRACEMASK__ 0x280E831
	#define __INGAMESWITCHDELAY__ 1500
	#define __THREADINDEX__ 2
	#define __MAXPRESTIGE__ 15
	#define __THREADSTACKSIZE__ 0
	#define __TRACECORE__ 3
	#define __THREADCORE__ 4
	#define ShieldFrontUp 27.0f
	#define ShieldFrontDown 33.0f
	#define ShieldFrontRight 16.0f
	#define ShieldFrontLeft 16.0f
	#define ShieldFrontDepthTopLeft 1.0f
	#define ShieldFrontDepthTopRight 1.0f
	#define ShieldFrontDepthBottomLeft 1.0f
	#define ShieldFrontDepthBottomRight 1.0f
	#define ShieldFrontXTweak 0.0f
	#define ShieldFrontYTweak 0.0f

	#define ShieldRearLeft 11.0f
	#define ShieldRearRight 20.0f
	#define ShieldRearUp 28.0f
	#define ShieldRearDown 28.0f
	#define ShieldRearDepthTopLeft 0.5f
	#define ShieldRearDepthTopRight 0.5f
	#define ShieldRearDepthBottomLeft 0.5f
	#define ShieldRearDepthBottomRight 0.5f
	#define ShieldRearXTweak 0.0f
	#define ShieldRearYTweak -0.0f
	#define ShieldAntiAimAdditave -60.0f
	#define __MINPITCH__ -60.0f
	#define __MAXPITCH__ 60.0f
	enum eStance {
		STANCE_STAND = 0x0,
		STANCE_CROUCH = 0x200,
		STANCE_PRONE = 0x100
	};
#elif defined(GHOSTS)
	#define __GAMENAME__ "GHOSTS"
	#define __MAXCLIENTS__ 12
	#define __TITLEFLAG__ 128
	#define __TITLEID__ 0x41270FEC //0x415608FC
	#define __MSGRIDENT__ 0x84342380
	#define __TIMEDATESTAMP__ 0x544F01BE
	#define __SMALLFONT__ "fonts/hudsmallfont"
	#define __ARROWICON__  "compassping_player"
	#define __FFANAME__ "dm"
	#define __SCORECMD__ "s"
	#define __SCAVENGERWEAPNAME__ "scavenger_bag_mp"
	#define __SCAVENGERHUDNAME__ "scavenger_pickup"
	#define __LEFTGUNADDITAVE__ 10
	#define __WORLDENTNUM__ 1022
	#define __TRACEMASK__  0x280E831
	#define __INGAMESWITCHDELAY__ 1500
	#define __THREADINDEX__ 2
	#define __MAXPRESTIGE__ 10
	#define __THREADSTACKSIZE__ 0
	#define __TRACECORE__ 4
	#define __THREADCORE__ 3
	// Shield shit
	#define ShieldFrontUp 22.0f
	#define ShieldFrontDown 30.0f
	#define ShieldFrontRight 14.0f
	#define ShieldFrontLeft 14.0f
	#define ShieldFrontDepthTopLeft -1.0f
	#define ShieldFrontDepthTopRight -1.0f
	#define ShieldFrontDepthBottomLeft -1.0f
	#define ShieldFrontDepthBottomRight -1.0f
	#define ShieldFrontXTweak 0.0f
	#define ShieldFrontYTweak 0.0f

	#define ShieldRearLeft 20.0f
	#define ShieldRearRight 26.0f
	#define ShieldRearUp 21.0f
	#define ShieldRearDown 8.0f
	#define ShieldRearDepthTopLeft 5.0f
	#define ShieldRearDepthTopRight 1.0f
	#define ShieldRearDepthBottomLeft 2.0f
	#define ShieldRearDepthBottomRight -3.0f
	#define ShieldRearXTweak 0.0f
	#define ShieldRearYTweak -0.0f
	#define ShieldAntiAimAdditave -60.0f
	#define __MINPITCH__ -60.0f
	#define __MAXPITCH__ 60.0f
	enum eStance {
		STANCE_STAND = 0x0,
		STANCE_CROUCH = 0x200,
		STANCE_PRONE = 0x100
	};
#elif defined(WAW)
	#define __GAMENAME__ "WAW"
	#define __MAXCLIENTS__ 18
	#define __TITLEFLAG__ 256
	#define __TITLEID__ 0x41270F0C //0x4156081C
	#define __MSGRIDENT__ 0x92345320
	#define __TIMEDATESTAMP__ 0x4AD7C0EE
	#define __SMALLFONT__ "fonts/smallfont"
	#define __ARROWICON__  "compassping_player"
	#define __FFANAME__ "dm"
	#define __SCORECMD__ "score"
	#define __SCAVENGERWEAPNAME__ ""
	#define __SCAVENGERHUDNAME__ "scavenger_pickup"
	#define __LEFTGUNADDITAVE__ 0
	#define __WORLDENTNUM__ 1022
	#define __TRACEMASK__ 0x2838013
	#define __INGAMESWITCHDELAY__ 1500
	#define __THREADINDEX__ 2
	#define __MAXPRESTIGE__ 10
	#define __THREADSTACKSIZE__ 0
	#define __TRACECORE__ 5
	#define __THREADCORE__ 4
	#define ShieldAntiAimAdditave -60.0f
	#define __MINPITCH__ -60.0f
	#define __MAXPITCH__ 60.0f
	enum eStance {
		STANCE_STAND = 0x0,
		STANCE_CROUCH = 0x200,
		STANCE_PRONE = 0x100
	};
#endif

#if defined(BO2)
	#define __MAXSETPRESTIGE__ 1337
#else
	#define __MAXSETPRESTIGE__ __MAXPRESTIGE__ + 1
#endif