#include "Sounds.h"
#include "Globals.h"
#include "SecureStrings.h"

#define STREAMING_BUFFER_SIZE 65536
#define MAX_BUFFER_COUNT 3

struct StreamingVoiceContext : public IXAudio2VoiceCallback
{
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {}
	void OnVoiceProcessingPassEnd() {}
	void OnStreamEnd() {}
	void OnBufferStart(void*) {}
	void OnBufferEnd(void*) { SetEvent(hBufferEndEvent); }
	void OnLoopEnd(void*) {}
	void OnVoiceError(void*, HRESULT) {}

	HANDLE hBufferEndEvent;

	StreamingVoiceContext() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	~StreamingVoiceContext() { CloseHandle(hBufferEndEvent); }
};

bool Sounds::Initialize(BOOL bFullInitialize) {
	if (bFullInitialize) {
		// Try to mount the NiNJA path
		if (MountPath(MOUNT_NAME, GetMountPath(), FALSE) == 0) {
			DbgPrint("Mounted path successfully!");
		}
		else {
			DbgPrint("Failed to mount path!");
			return false;
		}
	}

	HRESULT hr;
	// Try to initialize our audio
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
		DbgPrint("Failed to init xaudio2 object!");
		return false;
	}


	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice, 6,
		48000, 0, 0, NULL))) {
		DbgPrint("Failed to init mastering voice object!");
		return false;
	}

	// Make sure no sounds remain in our queue next time we reinit sounds
	vSoundQueue.clear();

	// If we made it this far, we've initialized our audio thread successfully
	bInitialized = true;
	return true;
}

HRESULT GetBlockBoundaries(DWORD nBlockCount, DWORD *pSeekTable, DWORD startSample, DWORD length, DWORD *pnStartingBlock, DWORD *pnStartingBlockPlayBegin, DWORD *pnEndingBlock, DWORD *pnEndingBlockPlayLength)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = GetXmaBlockContainingSample(nBlockCount, pSeekTable, startSample, pnStartingBlock, pnStartingBlockPlayBegin)))
	{
		return hr;
	}

	DWORD endSample = startSample + length - 1;
	DWORD endSampleOffset;

	if (FAILED(hr = GetXmaBlockContainingSample(nBlockCount, pSeekTable, endSample, pnEndingBlock, &endSampleOffset)))
	{
		return hr;
	}

	*pnEndingBlockPlayLength = endSampleOffset + 1;

	if (*pnStartingBlock == *pnEndingBlock)
	{
		*pnEndingBlockPlayLength = (endSampleOffset - *pnStartingBlockPlayBegin) + 1;
	}

	return hr;
}

//--------------------------------------------------------------------------------------
// Name: PlayXMA2Streamed
// Desc: Plays an .XMA2 file by streaming one XMA block at a time.
//--------------------------------------------------------------------------------------
VOID Sounds::PlayXMA2Streamed(IXAudio2* pXaudio2, const char* szFilename, DWORD dwPlayBeginOverride) {
	HRESULT hr = S_OK, hrWavOpen = S_OK;
	AtgCleaned::WaveFile WaveFile;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XMA2WAVEFORMATEX xma2 = { 0 };
	DWORD dwSeekTableSize = 0;
	IXAudio2SourceVoice* pSourceVoice;
	StreamingVoiceContext voiceContext;

	// Read the wave file
	if (FAILED(hrWavOpen = WaveFile.Open(szFilename)))
		DbgPrint("Error %#X opening WAV file", hr);

	// Read the format header
	if (FAILED(hr = WaveFile.GetFormat(&wfx, &xma2)))
		DbgPrint("Error %#X reading WAV format", hr);

	if (wfx.Format.wFormatTag != WAVE_FORMAT_XMA2)
		DbgPrint("Error - Expected an XMA2 XAudio2 compatible file");

	// Read the seek structure.
	if (FAILED(hr = WaveFile.GetSeekTableSize(&dwSeekTableSize)))
		DbgPrint("Error %#X reading seek table size", hr);

	DWORD* pSeekTable = (DWORD*)new BYTE[dwSeekTableSize];
	if (FAILED(hr = WaveFile.GetSeekTable(pSeekTable)))
		DbgPrint("Error %#X reading seek table", hr);

	// Create an XAudio2 voice to stream the XMA file
	if (FAILED(hr = pXaudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&xma2, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &voiceContext))) {
		DbgPrint("Error %#X creating source voice", hr);
		WaveFile.Close();
		goto jExit;
	}

	// Set the volume of our audio every iteration
	pSourceVoice->SetVolume(CG::fSoundVolume);

	// Start the voice
	pSourceVoice->Start(0, 0);

	// I/O buffers
	BYTE *buffers[MAX_BUFFER_COUNT];
	DWORD dwSize = xma2.BytesPerBlock;
	// XMA packets must be 2KB aligned
	//for (int i = 0; i < MAX_BUFFER_COUNT; i++) {
	//	buffers[i] = (BYTE*)(0xc3000000 + (i * dwSize));
	//}
	for (DWORD i = 0; i < MAX_BUFFER_COUNT; i++) {
		DWORD dwSize = xma2.BytesPerBlock;
		// XMA packets must be 2KB aligned
		buffers[i] = (BYTE*)XPhysicalAlloc(dwSize, MAXULONG_PTR, 2048, PAGE_READWRITE);
		if (!buffers[i]) {
			DbgPrint("Failed to allocate memory for sound buffer! Aborting!");

			for (DWORD b = 0; b < MAX_BUFFER_COUNT; b++) {
				if (buffers[b]) {
					XSAFE_FREE(buffers[b]);
				}
			}
			goto jExit;
		}
	}
	// Account for special case: when looping the whole file the play region is the actual loop region.
	// The encoder sets a different 'PlayLength' value, smaller than the loop region. There are
	// 3 subframes of ramp up data that are not taken into account by the encoder when it sets the 'PlayLength' value.
	// Fix the 'PlayBegin' and 'PlayLength' values here to avoid special cases in the streaming logic below.
	const DWORD dwRampUpSamples = XMA_SAMPLES_PER_SUBFRAME * 3;
	if (xma2.LoopBegin == dwRampUpSamples && (xma2.PlayLength + dwRampUpSamples) > xma2.LoopLength)
	{
		xma2.PlayBegin = xma2.LoopBegin;
		xma2.PlayLength = xma2.LoopLength;
	}

	// Play region boundaries
	DWORD playRegionStartingBlock;
	DWORD playRegionStartingBlockPlayBegin;
	DWORD playRegionEndingBlock;
	DWORD playRegionEndingBlockPlayLength;

	if (FAILED(hr = GetBlockBoundaries(xma2.BlockCount, pSeekTable, xma2.PlayBegin, xma2.PlayLength, &playRegionStartingBlock, &playRegionStartingBlockPlayBegin, &playRegionEndingBlock, &playRegionEndingBlockPlayLength))) {
		DbgPrint("Could not find block boundaries for play region\n");
	}

	// Loop region counters
	DWORD loopRegionStartingBlock = 0;
	DWORD loopRegionStartingBlockPlayBegin = 0;
	DWORD loopRegionEndingBlock = xma2.BlockCount - 1;
	DWORD loopRegionEndingBlockPlayLength = 0;

	// The encoder sets the values of LoopBegin and LoopLength in the header according to the authored content.
	// If we were submitting one big buffer to XAudio2, we would just copy all the values and send them to XAudio2 as is,
	// and XAudio2 would loop the content for us automatically.
	// Since we'll be reading one chunk at a time (in a streaming fashion), we'll submit multiple buffers (one per block read)
	// and we can't directly use the Loop* fields of the buffer, because we can't guarantee that the loop region is contained
	// within the block that we just read - it might cross blocks.
	// We need to do the math ourselves and change the Play* fields of the block we're currently submitting according to the
	// loop region originally defined in the header.
	if (xma2.LoopBegin > 0 || xma2.LoopCount > 0)
	{
		if (FAILED(hr = GetBlockBoundaries(xma2.BlockCount, pSeekTable, xma2.LoopBegin, xma2.LoopLength, &loopRegionStartingBlock, &loopRegionStartingBlockPlayBegin, &loopRegionEndingBlock, &loopRegionEndingBlockPlayLength)))
		{
			DbgPrint("Could not find block boundaries for play region\n");
		}
	}

	//
	// Now we manually replicate XAudio2's behavior had we sent the whole buffer in one shot.
	// XAudio2 plays the buffer from the beginning of the play region (PlayBegin), and when it reaches the end of the loop region,
	// it goes back and plays from the start of the loop region, until the loop end again, "LoopCount" times.
	// After playing the loop region "LoopCount" times, XAudio2 continues playing until the end of the play region (PlayEnd).
	// Like so:
	//
	//                                           ||<<<<< "LoopCount>0" <<<<||
	//  -------------                      -------------              -----------                    -----------
	//  | PlayBegin | -------------------- | LoopBegin | ------------ | LoopEnd | ------------------ | PlayEnd |
	//  -------------                      -------------              -----------                    -----------
	//                                           ||>>>>> "LoopCount--" >>>>||>>>>>>> "LoopCount==0" >>>>>>||
	//  >>>>>>>>>>>>>>>>>> PLAYBACK >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>||

	DWORD currentDiskReadBuffer = 0;
	DWORD nextBlock = 0;
	DWORD currentPosition = playRegionStartingBlock * xma2.BytesPerBlock;
	DWORD endPosition = min(WaveFile.Duration(), (playRegionEndingBlock + 1) * xma2.BytesPerBlock);
	DWORD currentBlock = playRegionStartingBlock;
	DWORD loopsRemaining = xma2.LoopCount;

	if (dwPlayBeginOverride > 0)
	{
		// We will start playback from another sample other than the one set by the 'PlayBegin' value in the header.
		// We will start playback from this specific sample, and if we find a loop end block, we will come back
		// and start playback from the loop start block, and go from there as usual. The only difference
		// is that we're starting playback from a specific place, and this place can actually be inside
		// our loop region. This type of playback can be useful, for example, to give the idea of
		// continuous playback without the player being present in the environment. Everytime the player walks in,
		// playback starts from a random location in the content, but the loop is honored as usual in case
		// the player remains in the environment long enough.
		DWORD overridePlayBeginBlock;
		DWORD overridePlayBeginOffset;
		if (FAILED(hr = GetXmaBlockContainingSample(xma2.BlockCount, pSeekTable, dwPlayBeginOverride, &overridePlayBeginBlock, &overridePlayBeginOffset)))
		{
			DbgPrint("Error %#X processing value for 'PlayBegin' override\n", hr);
		}
		currentPosition = overridePlayBeginBlock * xma2.BytesPerBlock;
		dwPlayBeginOverride = overridePlayBeginOffset;
	}

	while (currentPosition < endPosition) {

		if (!CG::bInGameCached || bShutdown || NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
			goto jExit;
		}

		// Issue a read request. This could be an asynch request, but for simplicity we will keep it synchronous.
		DWORD cbValid = min(xma2.BytesPerBlock, endPosition - currentPosition);
		DWORD cbBytesRead;

		if (FAILED(hr = WaveFile.ReadSample(currentPosition, buffers[currentDiskReadBuffer], cbValid, &cbBytesRead))) {
			DbgPrint("Error %#X reading XMA data\n", hr);
		}
		else if (cbBytesRead != cbValid) {
			DbgPrint("Unexpected number of bytes read (%d). Expected: %d.\n", cbBytesRead, cbValid);
		}

		currentPosition += cbValid;

		// We now have audio available. The next question is whether our XAudio2 source voice
		// has played enough data for us to give it another buffer full of audio.
		// We'd like to keep no more than MAX_BUFFER_COUNT - 1 buffers on the queue,
		// so that one buffer is always free for disk I/O.
		XAUDIO2_VOICE_STATE state;
		for (;;) {
			if (!CG::bInGameCached || bShutdown || NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
				goto jExit;
			}

			// 'GetState' now has a flag to avoid query of 'SamplesPlayed' and make the call cheaper.
			pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			if (state.BuffersQueued < MAX_BUFFER_COUNT - 1)
				break;

			WaitForSingleObject(voiceContext.hBufferEndEvent, INFINITE);
		}


		// At this point we have a buffer full of audio and enough room to submit it, so
		// let's submit it and get another read request going.

		// The initialization below to zero is important: we are explicitly setting both 'PlayBegin' and 'PlayLength'
		// fields of the structure to zero - meaning that, initially, we assume that we will be sending the current block
		// in its entirety. We have a couple of special cases, and we change these values if we happen to hit one of those,
		// but if we hit none of them, these fields will be left untouched and with a value of zero, causing the
		// buffer to be submitted in its entirety at the end of the iteration.
		XAUDIO2_BUFFER buf = { 0 };
		buf.AudioBytes = cbValid;
		buf.pAudioData = buffers[currentDiskReadBuffer];
		if (currentPosition >= endPosition && loopsRemaining == 0)
			buf.Flags = XAUDIO2_END_OF_STREAM;

		// Before we submit the buffer, adjust the "PlayBegin" and "PlayLength" fields accordingly.

		// Initially assume we'll submit the next block - this value will change if we happen to be at the last block of our loop region
		// and we need to loop.
		nextBlock = currentBlock + 1;

		if (xma2.LoopCount > 0 && currentBlock == loopRegionStartingBlock && loopsRemaining != xma2.LoopCount) {
			// We are currently at the first block of our loop region, and since loopsRemaining != xma2.LoopCount, it means
			// that we're coming back from the last block of our loop region. So we need to set the "PlayBegin" field
			// according to the loop region values.
			// If loopsRemaining == xma2.LoopCount, it means that this is the first time that we're visiting this block, and as
			// such we need to play it in its entirety according to the play region boundaries - the code below shouldn't be executed.
			buf.PlayBegin = loopRegionStartingBlockPlayBegin;

			if (xma2.LoopCount == XMA_INFINITE_LOOP) {
				// Infinite loop. Reset the value for loopsRemaining. Once we hit the last block of the loop region
				// loopsRemaining will become XMA_INFINITE_LOOP-1 and we will set it again to XMA_INFINITE_LOOP once we're back here,
				// causing us to loop forever.
				loopsRemaining = XMA_INFINITE_LOOP;
			}
		}

		if (xma2.LoopCount > 0 && currentBlock == loopRegionEndingBlock && loopsRemaining > 0) {
			// We are currently at the final block of our loop region, and we have to start playback from
			// the beginning of the loop region after this buffer is sent.
			// We adjust the "PlayLength" field accordingly to this final block and
			// change "currentPosition" and "nextBlock" to the starting block of the loop region
			// so that the next iteration submits the starting block of the loop.
			// If we are on the last block of the loop region but loopsRemaining == 0, it means that
			// we just played the last loop of the sound, and we're supposed to continue towards the end
			// of the play region, so the code below shouldn't be executed - the block should be played in its entirety according
			// to the play region boundaries (handled further below).

			if (currentBlock == loopRegionStartingBlock && loopsRemaining == xma2.LoopCount)
			{
				// Another special case: the loop also starts on this block, and this is the first time
				// we're playing this block. We can't just set the 'PlayLength' of this buffer to the length
				// of the loop region, since the length of the loop region is an offset from the 'LoopBegin' value.
				// Since this is the first time we're playing this block, the length value should cover the start of the
				// block plus the loop region, until the loop end.
				buf.PlayLength = (loopRegionStartingBlockPlayBegin + loopRegionEndingBlockPlayLength) - buf.PlayBegin;
			}
			else
			{
				buf.PlayLength = loopRegionEndingBlockPlayLength;
			}

			currentPosition = loopRegionStartingBlock * xma2.BytesPerBlock;
			nextBlock = loopRegionStartingBlock;
			loopsRemaining--;
		}

		// The following cases check to see if this block is part of the play region boundary, and if so, the
		// 'PlayBegin' and 'PlayLength' fields are updated accordingly, unless they have been touched already by the
		// loop logic above.
		if (currentBlock == playRegionStartingBlock && buf.PlayBegin == 0) {
			buf.PlayBegin = playRegionStartingBlockPlayBegin;
		}
		// The last term of the if clause covers the special case where we have one block which happens to be, at the same time, the
		// first and last block of both play and loop regions (imagine for example a file with only one 64K block with a loop region contained within it).
		// If we are looping for the last time, the loop logic set the 'PlayBegin' field but left the 'PlayLength' untouched so that we keep playing
		// the buffer past the end of the loop region. We don't want to set the 'PlayLength' of this block now to the 'PlayLength'
		// of the header since it will fall beyond the block - the blocks' 'PlayBegin' field was changed by the loop logic. Let's just leave it at zero
		// in this case and let XAudio2 play the remainder of the block.
		if (currentBlock == playRegionEndingBlock && buf.PlayLength == 0 && (buf.PlayBegin + playRegionEndingBlockPlayLength < pSeekTable[playRegionEndingBlock])) {
			// The encoder might have set a 'PlayLength' value that is not divisible by 128. The decoder will refuse
			// to play such a buffer. This is likely an encoder bug, so if that's the case we just use zero, which
			// will cause XAudio2 to play the whole block. Most files have their play region set to the whole file anyway.
			buf.PlayLength = (playRegionEndingBlockPlayLength % 128) ? 0 : playRegionEndingBlockPlayLength;
		}

		// A 'PlayBegin' override supercedes everything. This happens only once (note it gets reset to zero).
		if (dwPlayBeginOverride > 0) {
			if (dwPlayBeginOverride > buf.PlayBegin + buf.PlayLength)
			{
				// We have a loop and we adjusted our values accordingly, but now we realized that we have an override
				// that's past the loop region. We won't be able to honor the loop - the override supercedes it.
				buf.PlayLength = 0;
				nextBlock = currentBlock + 1;
			}
			else if (buf.PlayLength > 0)
			{
				// Adjust 'PlayLength' if set.
				buf.PlayLength -= (dwPlayBeginOverride - buf.PlayBegin);
			}
			buf.PlayBegin = dwPlayBeginOverride;
			dwPlayBeginOverride = 0;
		}

		// At this point we're ready to submit the buffer to XAudio2.
		// It's important to note that if none of the special cases above were hit, we have a buffer to submit with
		// 'PlayBegin' and 'PlayLength' set to zero. If that's the case, it means that this block is not on the
		// play region boundary neither on the loop region boundary (if we are looping - it could be that we're simply
		// dealing with a buffer that does not have a loop region at all).

		// Submit the buffer to XAudio2.
		pSourceVoice->SubmitSourceBuffer(&buf);

		currentDiskReadBuffer++;
		currentDiskReadBuffer %= MAX_BUFFER_COUNT;

		currentBlock = nextBlock;
	}

	XAUDIO2_VOICE_STATE state;
	for (;;) {
		pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
		if (!state.BuffersQueued)
			break;

		WaitForSingleObject(voiceContext.hBufferEndEvent, INFINITE);
	}

	// Clean up
	if (pSourceVoice) {
		pSourceVoice->Stop(0);
		pSourceVoice->DestroyVoice();
	}
jExit:

	WaveFile.Close();

	SAFE_DELETE(pSeekTable);

	for (int i = 0; i < MAX_BUFFER_COUNT; i++) {
		XSAFE_FREE(buffers[i]);
	}
	//DbgPrint("Exited");
}

VOID Sounds::Shutdown(BOOL bFullShutdown) {
	// Free all resources we allocated for our audio playback
	if (pMasteringVoice) {
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = NULL;
	}

	SAFE_RELEASE(pXAudio2);
	if (bFullShutdown) {
		bInitialized = false;
	}
}

static DWORD WINAPI Run(void *pParent) {
	Sounds *pSounds = (Sounds*)pParent;
	NiNJA.bSoundLoopRunning = TRUE;
	// Initialize our sounds
	if (pSounds->Initialize()) {
		DbgPrint("Sounds initialized!");
		// Run our loop
		while (!(NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload)) {
			// If we enter the game, and our audio has been marked as "shutdown" (this means we partially uninitialized before)
			if (CG::bInGameCached && pSounds->bShutdown) {
				if (pSounds->Initialize(FALSE)) {
					DbgPrint("Sounds reinitialized!");
					pSounds->bShutdown = false;
				}
				else {
					DbgPrint("Failed to reinitialize sounds! Aborting sound thread!");
					pSounds->bInitialized = false;
					break;
				}
			}
			// Otherwise, if we aren't in the game and our sounds have been initialized, and the shutdown flag hasnt been set
			else if (!CG::bInGameCached && !pSounds->bShutdown) {
				pSounds->Shutdown(FALSE);
				DbgPrint("Sounds partially shutdown!");
				pSounds->bShutdown = true;
			}
			else if(CG::bInGameCached && !pSounds->bShutdown) {
				// If our queue isn't empty, begin to process it
				if (pSounds->vSoundQueue.size() > 0) {
					// Setup our sound path
					CHAR pSoundPath[MAX_PATH];
					sprintf(pSoundPath, "NiNJA:\\ninjacache\\media\\sounds\\%s\\%i.xma", pSoundNames[pSounds->vSoundQueue[0].dwSoundType], pSounds->vSoundQueue[0].dwSoundIndex);

					// If the specified sound exists
					if (FileExists(pSoundPath)) {
						// Play it back
						pSounds->PlayXMA2Streamed(pSounds->pXAudio2, pSoundPath);
					}

					// Erase the first element, that we just played back
					pSounds->vSoundQueue.erase(pSounds->vSoundQueue.begin());
				}
				//pSounds->Run_Internal();
			}
			Sleep(100);
		}

		// Shutdown our sounds
		pSounds->Shutdown(TRUE);
	}

	DbgPrint("Audio thread has exited!");
	NiNJA.bSoundLoopRunning = FALSE;
	return 0;
}

VOID Sounds::RunSync() {
	if (!bInitialized) {
		Initialize();
	}

	//PlayXMA2Streamed(pXAudio2, "NiNJA:\\ninjacache\\startup.xma2");
	//Run_Internal();
}

VOID Sounds::Queue(eSoundTypes soundType, DWORD dwSoundIndex) {
	vSoundQueue.push_back(SoundInfo(soundType, dwSoundIndex));
}

bool Sounds::Start() {
	if (!bInitialized) {
		hThread = CreateThread(0, 16000, Run, this, CREATE_SUSPENDED, &dwThreadId);

		if (hThread) {
			XSetThreadProcessor(hThread, __THREADCORE__);
			SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}

		// Try to launch a thread to handle our audio thread
		return hThread > 0 ? true : false;
	}
	else {
		return dwThreadId != 0;
	}
}

Sounds::Sounds() {
	pXAudio2 = NULL;
	hThread = NULL;
	bInitialized = false;
	bShutdown = false;
	dwThreadId = 0;
}

Sounds::~Sounds() {

}