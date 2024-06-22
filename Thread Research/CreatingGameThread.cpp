#include "stdafx.h"
#include <setjmp.h>
#include "Rendering.h"
#include "Threads.h"
#include "Game.h"
#include "Ragebot.h"
#include "SCR_DrawScreenField.h"

void (*Sys_CreateThread)(void (*func)(int), int threadContext)
	= (void(*)(void (*)(int), int))0x8223CAB0;

void (*Sys_SetValue)(int threadContext, void* data) = (void(*)(int, void*))0x8223D258;
void* (*Sys_GetValue)(int threadContext) = (void*(*)(int))0x8223D270;

HANDLE* threadId = (HANDLE*)0x83372254;

/*
	// Indexes for Sys_GetValue
	index 1 = va buffer
	index 3 = TraceThreadInfo
*/

/*
	Method 2 for threading:
	What this method does is it creates a title thread just like the game does.
	I was unable to get it to be stable but it works. My theory is if you can
	sync it with the render thread perfectly, it will be stable. What could be
	causing unstability is that you're calling too many trace functions at once.
	While you can do this fine in CL_SendCmd, which is called in the render thread.
	In our created thread, it is calling them all at once, but may be getting called
	after the current frame has ended.

	> Set event in a render hook
	> WaitForSingleObject in thread
	> Profit? Theoryzz nibba
*/

void testThread(int threadContext) {
	printf("[%s] thread created\n", __FUNCTION__);

	// Set the database thread's TLS values
	((void(*)(int))0x8223C9A0)(1); // INDEX 1 = Render Thread context id

	while (_setjmp((SETJMP_VECTOR4*)Sys_GetValue(2)) != 0);

	while (true) {
		if (isInGame() && ragebot.state != AIMBOT_DISABLE) {
			updateStructs();
			ragebot.closestClient = ragebot.getClosestClient();
		}

		Sleep(45);
	}
}

bool spawnRenderThread(void (*func)(int)) {
	Sys_CreateThread(func, 9);

	HANDLE hRenderThread = threadId[9];

	if (hRenderThread != 0)
		ResumeThread(hRenderThread);

	return hRenderThread != 0;
}

void initRenderThread() {
	spawnRenderThread(testThread);
}
