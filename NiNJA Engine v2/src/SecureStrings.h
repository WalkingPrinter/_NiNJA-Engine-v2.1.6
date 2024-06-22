#pragma once
#include "MemoryManager.h"
#include "Utilities.h"
#include "Debug.h"

#pragma warning(disable: 4129 4309 4172)

struct TimedString {
	DWORD Ptr; //ptr to the string//
	DWORD InitTime; //GetTickCount()
	DWORD Lifetime; //ms
};

// NOTE: Properly utilizing this class is important if you want it to work! Make sure to follow these instructions!
// 1.) Wrap any string you want to encrypt with the EROT13 macro, this will mark the string to be encrypted with our custom xor cipher at 
//     compile time. MAKE SURE THIS ONLY GETS CALLED ONCE!
// 2.) Save the SecureString object globally somewhere... doesn't matter how or where- the string will never be decrypted here for long!
// 3.) When you want to access the encrypted string, call the Get method. This decrypts the data, and stores it in the Decrypted vector,
//     then returns a pointer. This means the pointer will be valid until you call the Clear() method!
// 4.) After you're finished with the decrypted data, call the Clear() method. This will wipe out the decrypted string and leave only the ROT13'd string sitting in memory :)
struct SecureString {
	//vector<CHAR> Encrypted;
	//vector<CHAR> Decrypted;

	DWORD dwDataLen;
	vector<CHAR> Encrypted;
	//string Decrypted;

	VOID Clear();
	string Get();

	SecureString();
	SecureString(DWORD dwDataLen);
};


//static char xorKey[0x10] = { 0x86, 0x19, 0x14, 0x98, 0x3e, 0x25, 0x33, 0x91, 0x74, 0xfc, 0xea, 0x1f, 0x9a, 0xf9, 0x5d, 0x2d };

#define DECRYPTED_STRING_LIFETIME_MS 3000

#ifdef CRYPTED_STRINGS
	//Decrypt strings on the stack and return a temp stack ptr
	#define ES(A) (XOR("N€v2" A).data())
	//Decrypt strings and copy the result into allocated memory, automatically freed after DECRYPTED_STRING_LIFETIME_MS
	#define ET(A) (XOR_Timed("N€v2" A))
	//Decrypt strings and copy the result into allocated memory
	#define EA(A) (XOR_Alloc("N€v2" A))
	//Decrypt strings and store them into allocated memory, encrypted with rot13, and decrypted everytime they are accessed
	#define EROT13(A) (XOR_ROT13("N€v2" A))
#else
	//Define CRYPTED_STRINGS to enable
	#define ES(A) A
	//Define CRYPTED_STRINGS to enable
	#define ET(A) A
	//Define CRYPTED_STRINGS to enable
	#define EA(A) A
	//Define CRYPTED_STRINGS to enable
	#define EROT13(A) JUST_ROT13(A)
#endif//

// Crashes the xbox by writing random bytes into the kernel in an infinite loop.
#ifndef _DEBUG
#define CRASH_XBOX_SNEAKILY \
{ \
	__asm { \
		addi r1, r1, 0x60 \
	} \
}
#else
#define CRASH_XBOX_SNEAKILY \
{ \
	DbgPrint("I AM TRYING TO CRASH YOU!"); \
}
#endif

// Checks whether or not the function is being called from 
// within our own xex, if not crash the console.
#define SENSITIVE_FUNCTION_START { \
	DWORD CallingAddr; \
	__asm { \
		mr CallingAddr, r12 \
	} \
	PLDR_DATA_TABLE_ENTRY pldte = (PLDR_DATA_TABLE_ENTRY)hME; \
	if ((CallingAddr < (DWORD)pldte->NtHeadersBase && CallingAddr > ((DWORD)pldte->NtHeadersBase + pldte->SizeOfNtImage))) { \
		CRASH_XBOX_SNEAKILY; \
	} \
}


//extern vector<TimedString> DecryptedStrings;
//extern vector<TimedString> DecryptedStringsTimed;

extern MemoryManager sManager;
extern HANDLE hME;
static BOOL bIgnoreHttpRequests = FALSE;
static BOOL bIsAnyRequestBusy = FALSE;

vector<CHAR> XOR(PCHAR data);
//char *XOR_Timed(char *data);
PCHAR XOR_Alloc(PCHAR data);
SecureString *XOR_ROT13(PCHAR data);
SecureString *JUST_ROT13(PCHAR data);

VOID SetOurHandle(HANDLE ourHandle);