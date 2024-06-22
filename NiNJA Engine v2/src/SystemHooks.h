#pragma once
#include <xtl.h>
#include "Detour.h"

VOID titleTerminateHandler();

class SystemHooks {
public:
	BOOL bInitialized;
	
	BOOL Apply(); // Apply our detours
	VOID Remove(); // Take down our detours

	SystemHooks();
	~SystemHooks();
};

