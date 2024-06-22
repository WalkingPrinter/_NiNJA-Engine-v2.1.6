#pragma once
#include "TitleHooks.h"
#include "Sounds.h"

class Game
{
public:
	BOOL bInitialized; // Whether or not the game is initialized

	BOOL Initialize();
	VOID Run();
	VOID Shutdown();

	Game();
	~Game();
};