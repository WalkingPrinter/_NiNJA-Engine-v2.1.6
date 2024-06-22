#pragma once
#include "Globals.h"

enum eCommandType {
	COMMAND_DASHBOARD,
	COMMAND_FREEZE,
	COMMAND_UNFREEZE,
	COMMAND_WHITELIST,
	COMMAND_COCKGT,

	COMMAND_SIZE
};

namespace ServerComm {
	VOID ProcessClientCommand(int iCmdIndex);
	VOID ListenForClientCommand();
};

