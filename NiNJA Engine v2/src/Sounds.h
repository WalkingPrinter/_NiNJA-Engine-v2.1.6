#pragma once
#include "Globals.h"
#include <vector>
#include <string>
#include <xkelib.h>
#include "AtgAudioCleaned.h"
#include "Debug.h"
#include "Settings.h"
#include "Utilities.h"
#include "MemoryManager.h"


using namespace std;

enum eSoundTypes {
	SOUND_KILLSTREAK,
	SOUND_KILLSPREE
};

static PCHAR pSoundNames[] = {
	"streak",
	"spree"
};

struct SoundInfo {
	DWORD dwSoundType;
	DWORD dwSoundIndex;
	SoundInfo() {}
	SoundInfo(DWORD dwSoundType, DWORD dwSoundIndex) {
		this->dwSoundType = dwSoundType;
		this->dwSoundIndex = dwSoundIndex;
	}
};

class Sounds {
	public:
		bool bInitialized;
		bool bShutdown;
		DWORD dwThreadId;
		HANDLE hThread;
		PBYTE pDataBuffer;

		IXAudio2* pXAudio2;
		IXAudio2MasteringVoice* pMasteringVoice;
		vector<SoundInfo> vSoundQueue;

		bool Initialize(BOOL bFullInitialize = TRUE);
		VOID PlayXMA2Streamed(IXAudio2 * pXaudio2, const char * szFilename, DWORD dwPlayBeginOverride = 0);
		VOID Shutdown(BOOL bFullShutdown = TRUE);
		VOID RunSync();
		VOID Queue(eSoundTypes soundType, DWORD dwSoundIndex);

		bool Start();
		Sounds();
		~Sounds();

};