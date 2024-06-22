#include "MemoryManager.h"
#include "xkelib.h"
#include "Debug.h"
#include "Settings.h"


MemoryManager::MemoryManager() {
}

MemoryManager::MemoryManager(DWORD Buffer, DWORD Size) {
	SetupManager(Buffer, Size);
}

MemoryManager::~MemoryManager() {
	if (allocated.size() > 0) {
		allocated.clear();
	}
}

VOID MemoryManager::SetupManager(DWORD Buffer, DWORD Size) {
	memory = Buffer;
	memorySize = Size;
}

BOOL MemoryManager::alloc(DWORD Size, DWORD *Out, BOOL zeroMemory) {
	for (DWORD i = 0; i < memorySize - Size; i++) {
		BOOL validMemoryRegion = TRUE;
		for (DWORD a = 0; a < allocated.size(); a++) {
			for (DWORD s = 0; s < Size; s++) {
				if (memory + i + s >= allocated[a].address && memory + i + s < allocated[a].address + allocated[a].size) {
					//the memory we're trying to allocate falls within the memory region of allocated[a]
					validMemoryRegion = FALSE;
					i += (allocated[a].size + s - 1);
					break;
				}
			}
		}
		if (validMemoryRegion) {
			DWORD dwAddress = memory + i;
			if (MmIsAddressValid((PVOID)dwAddress)) {
				if (zeroMemory) {
					RtlSecureZeroMemory((PVOID)dwAddress, Size);
				}
				allocated.push_back(MemoryRegion(memory + i, Size));
				*Out = memory + i;
				return TRUE;
			}
			else {
				DbgPrint("GOD DAMNIT BOBBY!");
				break;
			}
		}
	}
	return FALSE;
}

BOOL MemoryManager::alloc(DWORD Size, PVOID Out, BOOL zeroMemory) {
	return alloc(Size, (DWORD*)Out);
}

BOOL MemoryManager::free(DWORD Address, BOOL clearMemory) {
	for (DWORD i = 0; i < allocated.size(); i++) {
		if (allocated[i].address == Address) {
			if (clearMemory) {
				ZeroMemory((PVOID)allocated[i].address, allocated[i].size); //clear our the buffer
			}
			std::vector<MemoryRegion>::iterator it = allocated.begin();
			std::advance(it, i);
			allocated.erase(it); //erase it from the vector of allocated memory
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MemoryManager::free(PVOID Address, BOOL clearMemory) {
	return free((DWORD)Address);
}