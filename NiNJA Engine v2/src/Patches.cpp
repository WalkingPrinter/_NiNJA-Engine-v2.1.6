#include "Patches.h"
#include "Utilities.h"

BYTE bXorKey[0x10];

#ifdef DUMP_BINS
BYTE dumpBuf[sizeof(CheatPatches_s)];
#define dumpCoDPatch(a) ((DWORD)(docryptoBypass(a, (QWORD)((((DWORD*)&a - (DWORD*)&CoD))))))
#define DumpName "NiNJA:\\ninjacache\\" __GAMENAME__ ".bin" 
VOID Cheats_s::DumpPatchBinary(char* file) {
	if (!bHasPatches)
		return;

	DbgPrint("Dumping!");

	memset(dumpBuf, 0, sizeof(CheatPatches_s));

	DWORD *fileBuff = (DWORD*)&dumpBuf[0];
	for (int i = 0; i * 4 < sizeof(CheatPatches_s); i += 1) {
		fileBuff[i] = (DWORD)dumpCoDPatch(*(DWORD*)((DWORD)&CoD + ((DWORD)i * 4)));
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

#if !defined(DEBUG)
#undef __GAMENAME__
#endif