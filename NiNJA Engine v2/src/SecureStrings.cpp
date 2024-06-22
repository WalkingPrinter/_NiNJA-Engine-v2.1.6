#include "SecureStrings.h"

HANDLE hME = NULL;
VOID SetOurHandle(HANDLE ourHandle) {
	hME = ourHandle;
}

MemoryManager sManager;
PCHAR XOR_Alloc(PCHAR data) {
	SENSITIVE_FUNCTION_START;
	vector<CHAR> decString = XOR(data);
	int data_len = strlen(&decString[0]);

	PCHAR newMem;
	if (sManager.alloc(data_len + 1, &newMem)) {
		memcpy(newMem, &decString[0], data_len);
		return newMem;
	}
	return "";
}

string SecureString::Get() {
	SENSITIVE_FUNCTION_START;

	// Check if we forgot to close our old handle before we called Get() again... and throw a warning
	//if (Decrypted) {
	//	DbgPrint("Decrypted already exists! You got a memory leak nig!");
	//	SAFE_DELETE(Decrypted);
	//	Decrypted = NULL;
	// }
	
	// Alloc some memory
	//Decrypted = new CHAR[dwDataLen + 1];
	//if (Decrypted) {
		// Copy over our encrypted data into a newly allocated buffer
		string Decrypted = string(&Encrypted[0], dwDataLen + 1);

		// Decrypt it
		rot13((PCHAR)Decrypted.c_str(), (PCHAR)Decrypted.c_str() + dwDataLen + 1);

		// Return a pointer to the decrypted data... remember to call Clear() as quickly as possible!
		return Decrypted;
	//}
	//DbgPrint("Decryption failed- failed to allocate buffer space!");
	//return "";
}

// Deprecated
//VOID SecureString::Clear() {
//	// Delete our decrypted
//	memset(Decrypted, 0, dwDataLen);
//	SAFE_DELETE(Decrypted);
//	Decrypted = NULL;
//}

SecureString::SecureString() {
	this->dwDataLen = 0;
}

SecureString::SecureString(DWORD dwDataLen) {
	this->dwDataLen = dwDataLen;
}

SecureString *XOR_ROT13(PCHAR data) {
	SENSITIVE_FUNCTION_START;

	int data_len = strlen(data);
	//PCHAR newMem;
	SecureString *ret = NULL;
	vector<CHAR> crypted;
	if (sManager.alloc(sizeof(SecureString), &ret)) {
		for (int i = 0; i < data_len; i++) {
			crypted.push_back(data[i] ^ bXorKey[i % 0x10]);
		}
		crypted.push_back(0);

		ret->dwDataLen = data_len;
		ret->Encrypted = crypted;
		//DbgPrintE("Decrypted: %s", &newString.Encrypted[0]);

		// ROT13 the string in memory :)
		rot13(&ret->Encrypted[0], &ret->Encrypted[0] + data_len);

		//DbgPrintE("Encrypted: %s, Decrypted: %s", ret->Encrypted, ret->Get());

		crypted.clear();
	}
	return ret;
}

SecureString *JUST_ROT13(PCHAR data) {
	SENSITIVE_FUNCTION_START;

	int data_len = strlen(data);
	//PCHAR newMem;
	SecureString *ret = NULL;
	vector<CHAR> crypted;
	if (sManager.alloc(sizeof(SecureString), &ret)) {
		for (int i = 0; i < data_len; i++) {
			crypted.push_back(data[i]);
		}
		crypted.push_back(0);

		ret->dwDataLen = data_len;
		ret->Encrypted = crypted;
		//DbgPrintE("Decrypted: %s", &newString.Encrypted[0]);

		// ROT13 the string in memory :)
		rot13(&ret->Encrypted[0], &ret->Encrypted[0] + data_len);

		//DbgPrintE("Encrypted: %s, Decrypted: %s", ret->Encrypted, ret->Get());

		crypted.clear();
	}
	return ret;
}

//vector<TimedString> DecryptedStringsTimed;
//char *XOR_Timed(char *data) {
//	SENSITIVE_FUNCTION_START;
//	int data_len = strlen(data);
//	PCHAR newMem;
//	if (sManager.alloc(data_len + 1, &newMem))
//	{
//		memcpy(newMem, data, data_len);
//		for (int i = 0; i < data_len; i++)
//			newMem[i] ^= xorKey[i % 0x10];
//		newMem[data_len] = '\0';
//		TimedString str;
//		str.InitTime = GetTickCount();
//		str.Lifetime = DECRYPTED_STRING_LIFETIME_MS;
//		str.Ptr = (DWORD)newMem;
//		DecryptedStringsTimed.push_back(str);
//		return newMem;
//	}
//	return "";
//}

vector<CHAR> XOR(PCHAR data) {
	SENSITIVE_FUNCTION_START;
	vector<CHAR> decString;
	int i = 0;
	while(true) {
		CHAR decByte = data[i] ^ bXorKey[i % 0x10];
		decString.push_back(decByte);
		if (decByte == 0) {
			break;
		}
		i++;
	}
	return decString;
}