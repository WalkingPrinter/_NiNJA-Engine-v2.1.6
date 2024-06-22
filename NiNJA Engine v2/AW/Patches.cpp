#include "Patches.h"
#include "Utilities.h"

#ifdef DUMP_BINS
BYTE dumpBuf[sizeof(CheatPatches_s)];
#define dumpCoDPatch(a, b) ((DWORD)(docryptoBypass(a, (QWORD)((((DWORD*)&a - b ))))))
#define DumpName "NiNJA:\\" __GAMENAME__ ".bin" 
VOID Globals::DumpPatchBinary(char* file) {
	if (!Not(NiNJA.cPatches)->bHasPatches)
		return;

	DbgPrint("Dumping!");

	memset(dumpBuf, 0, sizeof(CheatPatches_s));

	DWORD *fileBuff = (DWORD*)&dumpBuf[0];
	for (BYTE i = 0; i * 4 < sizeof(CheatPatches_s); i += 1) {
		fileBuff[i] = (DWORD)dumpCoDPatch(*(DWORD*)((DWORD)&Not(cPatches)->CoD + ((DWORD)i * 4)), (DWORD*)&Not(cPatches)->CoD);
	}

	DbgPrint("Writing!");

	if (MountPath(MOUNT_NAME, GetMountPath(), FALSE) == 0) {
		DbgPrint("Mounted path successfully!");
	}
	else {
		DbgPrint("Failed to mount path!");
		return;
	}

	BinToFile(file != NULL ? file : DumpName, dumpBuf, sizeof(CheatPatches_s));
}
#endif