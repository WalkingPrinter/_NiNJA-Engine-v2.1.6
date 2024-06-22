#pragma once
#include "Globals.h"
#include "GameFuncs.h"

namespace UserCmd {
	static DWORD GetUserCmdSize()  {
	#if defined(BO1)
		return 0x34;
	#elif defined(BO2) || defined(GHOSTS)
		return 0x3c;
	#elif defined(AW) || defined(BO3)
		return 0x40;
	#elif defined(MW1)
		return 0x20;
	#elif defined(MW2)
		return 0x28;
	#elif defined(WAW) || defined(MW3)
		return 0x2c;
	#endif
		return 0;
	}

	static DWORD GetCurrentUserCmdNum_addr() {
		DWORD input_t = GameFuncs::Get_Input_t();
		if (input_t) {
			return input_t + GetUserCmdSize() * 128;
		}
		return 0;
	}

	static DWORD GetCurrentUserCmdNum() {
		return *(int*)(GetCurrentUserCmdNum_addr());
	}

	static DWORD GetUserCmd(int cmdNum) {
		DWORD input_t = GameFuncs::Get_Input_t();
		if (input_t) {
			int size = GetUserCmdSize();
			cmdNum = cmdNum & 0x7f;
			return input_t + (size * cmdNum);
		}
		return 0;
	}

	static DWORD GetOldUserCmd() {
	#if defined(AW)
		return GetUserCmd(GetCurrentUserCmdNum() - 1);
	#else
		if (CG::bIsMultiplayer) {
			return GetUserCmd(GetCurrentUserCmdNum() - 3);
		}
		else {
			return GetUserCmd(GetCurrentUserCmdNum() - 1);
		}
	#endif
	}

	static DWORD GetCurrentUserCmd() {
		return GetUserCmd(GetCurrentUserCmdNum());
	}

	static DWORD GetNextUserCmd() {
	#if defined(AW)
		return GetUserCmd(GetCurrentUserCmdNum() + 1);
	#else
		if (CG::bIsMultiplayer) {
			return GetUserCmd(GetCurrentUserCmdNum() + 3);
		}
		else {
			return GetUserCmd(GetCurrentUserCmdNum() + 1);
		}
	#endif
	}

	static DWORD GetStance() {
		DWORD dwButtons = *(DWORD*)(GetUserCmd(GetCurrentUserCmdNum()) + 0x4);
		//DbgPrintE("Buttons: %X", dwButtons);
		if (dwButtons & STANCE_PRONE) {
			return STANCE_PRONE;
		}
		else if (dwButtons & STANCE_CROUCH) {
			return STANCE_CROUCH;
		}
		return STANCE_STAND;
	}

	static DWORD GetNospreadSeed(int cmdNum) {
	#if defined(MW2) || defined(MW3) || defined(GHOSTS) || defined(AW)
		return *(DWORD*)GetOldUserCmd();
	#else
		return *(int*)GetCurrentUserCmd();
	#endif
	}

	static DWORD GetUserCmdViewAngles(int cmd) {
	#if defined(BO1) || defined(BO2)
		return cmd + 0xc;
	#elif defined(BO3)
		return cmd + 0x10;
	#else
		return cmd + 0x8;
	#endif
	}

	static DWORD GetUserCmdPlayerMove(int cmd) {
	#if defined(BO1)
		return cmd + 0x1e;
	#elif defined(BO2)
		return cmd + 0x24;
	#elif defined(MW1) || defined(WAW)
		return cmd + 0x16;
	#elif defined(MW2)
		return cmd + 0x1a;
	#elif defined(MW3) || defined(GHOSTS) || defined(AW)
		return cmd + 0x1c;
	#elif defined(BO3)
		return cmd + 0x28;
	#endif
	}

	static VOID SetIsAiming(int cmd, bool isAiming) {
		int buttons = cmd + 0x4, mask;
		int value = *(DWORD*)(buttons);

	#if defined(WAW)
		mask = 0x00500800;
	#elif defined(BO1) || defined(BO2) || defined(BO3)
		mask = 0x00100280;
	#else
		mask = 0x00080800;
	#endif

		if (isAiming) {
			ADDFLAG(*(DWORD*)(buttons), mask);
		}
		else {
			DELFLAG(*(DWORD*)(buttons), mask);
		}
	}
	static VOID SetInAttack(int cmd, bool inAttack) {
		int buttons = cmd + 0x4, mask;
		int value = *(DWORD*)(buttons);

	#if defined(BO1) || defined(BO2) || defined(BO3)
		mask = 0x80000000;
	#else
		mask = 0x00000001;
	#endif

		if (inAttack) {
			ADDFLAG(*(DWORD*)(buttons), mask);
		}
		else {
			DELFLAG(*(DWORD*)(buttons), mask);
		}
	}

	static bool IsAttacking(int cmd) {
		int buttons = cmd + 0x4;
	#if defined(BO1) || defined(BO2) || defined(BO3)
		return (*(DWORD*)(buttons) & 0x80000000) != 0;
	#else
		return (*(DWORD*)(buttons) & 0x00000001) != 0;
	#endif
	}

	static bool IsAiming(int cmd) {
		int buttons = cmd + 0x4;
	#if defined(WAW)
		return (*(DWORD*)(buttons) & 0x00500800) != 0;
	#elif defined(BO1) || defined(BO2) || defined(BO3)
		return (*(DWORD*)(buttons) & 0x00100280) != 0;
	#else
		return (*(DWORD*)(buttons) & 0x00080800) != 0;
	#endif
	}

	static VOID AddUserCommand() {
		int currentUserCmd = GetCurrentUserCmd();
		int nextUserCmd = GetNextUserCmd();

		// Copy our currentCmd into our new cmd
		memcpy((void*)nextUserCmd, (void*)currentUserCmd, GetUserCmdSize());

		// Increment our current cmd number, which basically inserts our command instead of replacing one
	#if defined(AW)
		*(DWORD*)(GetCurrentUserCmdNum_addr()) += 1;
	#else
		if (CG::bIsMultiplayer) {
			*(DWORD*)(GetCurrentUserCmdNum_addr()) += 3;
		}
		else {
			*(DWORD*)(GetCurrentUserCmdNum_addr()) += 1;
		}
	#endif

		// Mod our usercmd time
		*(DWORD*)nextUserCmd += 1;
	}
};

