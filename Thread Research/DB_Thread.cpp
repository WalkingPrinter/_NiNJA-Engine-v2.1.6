#include "stdafx.h"
#include "DB_Thread.h"
#include "Ragebot.h"
#include "Game.h"
#include <setjmp.h>

/*
	Method 1 for threading:
	What this method does is it takes the thread 'DB_Thread'
	and it replaces the TLS values with the main thread's TLS values
*/

bool initialize = false;

detour<bool> *DB_Thread = nullptr;

bool DB_ThreadHook(void) {
	if (!initialize) {
		// Set the database thread's TLS values
		((void(*)(int))0x8223C9A0)(0);
		initialize = true;
	}

	if (isInGame() && ragebot.state != AIMBOT_DISABLE) {
	//	updateStructs();
	//	ragebot.closestClient = ragebot.getClosestClient();
	}

	if (*(int*)0x82D1B7FC == 0)
		return false;

	else if (*(int*)0x82D1B7F0 == 0)
		return true;

	*(int*)0x82D1B7FC = 0;
	return false;
}