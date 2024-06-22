#include "Utilities.h"
#include "Globals.h"
#include "xkelib.h"
#include "Settings.h"
#include "SecureStrings.h"
#include "GameFuncs.h"
#include <setjmp.h>
#include <ctime>

//--------------------------------------------------Vector 2--------------------------------------------------//

vec2::vec2() {
	x = y = 0;
}

vec2::vec2(float X, float Y) {
	x = X;
	y = Y;
}

bool vec2::operator==(const vec2 vec) {
	return (x == vec.x && y == vec.y);
}

bool vec2::operator!=(const vec2 vec) {
	return !(x == vec.x && y == vec.y);
}

float vec2::distance(const vec2 vec) {
	float lenX = abs(vec.x - x);
	float lenY = abs(vec.y - y);

	return lenX + lenY;
}

//--------------------------------------------------Vector 3--------------------------------------------------//
vec3::vec3() {
	x = y = z = 0;
}

vec3::vec3(float X, float Y, float Z) {
	x = X;
	y = Y;
	z = Z;
}

vec3 vec3::cross(const vec3 vec) {
	return vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}

vec3 vec3::scale(float scale) {
	return (*this * scale);
}

vec3 vec3::lerp(vec3 end, float fraction) {
	return (((end - *this) * fraction) + *this);
}

vec3 vec3::negate() {
	return *this * -1.0f;
}

vec3 vec3::convertToAngles() {
	float yaw = 0.0f;
	float pitch = 0.0f;

	if (x == 0.0f && y == 0.0f)
		pitch = z > 0.0f ? 90.0f : 270.0f;

	else {
		if (x != 0.0f)
			yaw = atan2f(y, x) * 57.2957795131f;
		else if (y > 0.0f)
			yaw = 90.0f;
		else
			yaw = 270.0f;

		if (yaw < 0.0f)
			yaw += 360.0f;

		pitch = atan2f(z, sqrt((x * x) + (y * y))) * 57.2957795f;

		if (pitch < 0)
			pitch += 360;
	}

	return vec3(-pitch, yaw, 0.0f);
}

vec3 vec3::operator()(float x, float y, float z) {
	return vec3(x + x, y + y, z + z);
}

bool vec3::operator==(const vec3 vec) {
	return (x == vec.x && y == vec.y && z == vec.z);
}

bool vec3::operator!=(const vec3 vec) {
	return !(x == vec.x && y == vec.y && z == vec.z);
}

bool vec3::copy(vec3 dataToCopy) {
	memcpy(&this[0], &dataToCopy, 12);
	return memcmp(&this[0], &dataToCopy, sizeof vec3) == 0;
}

vec3 vec3::operator+(const vec3 vec) {
	return vec3(x + vec.x, y + vec.y, z + vec.z);
}

vec3 vec3::operator+(const float value) {
	return vec3(x + value, y + value, z + value);
}

vec3 vec3::operator-(const vec3 vec) {
	return vec3(x - vec.x, y - vec.y, z - vec.z);
}

vec3 vec3::operator-(const float value) {
	return vec3(x - value, y - value, z - value);
}

vec3 vec3::operator*(const vec3 vec) {
	return vec3(x * vec.x, y * vec.y, z * vec.z);
}

vec3 vec3::operator*(const float value) {
	return vec3(x * value, y * value, z * value);
}

vec3 vec3::operator/(const float value) {
	return vec3(x / value, y / value, z / value);
}

vec3 vec3::operator=(const float* value) {
	return *(vec3*)&value[0];
}

vec3 vec3::operator*=(const vec3 vec) {
	return vec3(x *= vec.x, y *= vec.y, z *= vec.z);
}

vec3 vec3::operator*=(const float value) {
	return vec3(x *= value, y *= value, z *= value);
}

vec3 vec3::operator+=(const vec3 vec) {
	return vec3(x += vec.x, y += vec.y, z += vec.z);
}

vec3 vec3::operator-=(const vec3 vec) {
	return vec3(x -= vec.x, y -= vec.y, z -= vec.z);
}

vec3 vec3::operator-=(const float value) {
	return vec3(x -= value, y -= value, z -= value);
}

float vec3::length() const {
	return sqrtf((x * x) + (y * y) + (z * z));
}

float vec3::magnitude() const {
	return sqrtf((x * x) + (y * y) + (z * z));
}

float vec3::distanceTo(vec3 pos) {
	float dx = x - pos.x;
	float dy = y - pos.y;
	float dz = z - pos.z;
	return sqrtf((dx * dx) + (dy * dy) + (dz * dz));
}

float vec3::dot(vec3& vec) {
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

void vec3::normalize() {
	float len = length();

	if (len == 0.0f) {
		*this = vec3();
		return;
	}

	*this *= (len /= 1.0f);
}

void vec3::normalizeAngles() {
	if (x > 360.0f)
		x -= 360.0f;

	if (x < 0.0f)
		x += 360.0f;

	if (y > 360.0f)
		y -= 360.0f;

	if (y < 0.0f)
		y += 360.0f;


	if (z > 360.0f)
		z -= 360.0f;

	if (z < 0.0f)
		z += 360.0f;
}

void vec3::zero() {
	memset(&this[0], 0, sizeof vec3);
}

//--------------------------------------------------Vector 4--------------------------------------------------//
vec4::vec4() {
	x = y = z = w = 0;
}

vec4::vec4(float X, float Y, float Z, float W) {
	x = X;
	y = Y;
	z = Z;
	w = W;
}

vec4 vec4::lerp(vec4 start, vec4 endpos, float time) {
	return start + ((endpos - start) * time);
}

bool vec4::operator==(const vec4 vec) {
	return (x == vec.x && y == vec.y && z == vec.z && w == vec.w);
}

bool vec4::operator!=(const vec4 vec) {
	return !(x == vec.x && y == vec.y && z == vec.z  && w == vec.w);
}

vec4 vec4::operator+(const vec4 vec) {
	return vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
}

vec4 vec4::operator-(const vec4 vec) {
	return vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
}

vec4 vec4::operator*(const vec4 vec) {
	return vec4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
}

vec4 vec4::operator*(const float value) {
	return vec4(x * value, y * value, z * value, w * value);
}

void vec4::zero() {
	memset(&this[0], 0, sizeof vec4);
}

// --------------------------------------------------Color-------------------------------------------------- //

colors::colors() {
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

colors::colors(float red, float green, float blue, float alpha) {
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

void colors::colorShift(float speed) {
	if (clock() - colorShiftTimer > 1) {
		if (r > 0.0f && b <= 0.0f) {
			r -= speed;
			g += speed;
		}

		if (g > 0.0f && r <= 0.0f) {
			g -= speed;
			b += speed;
		}

		if (b > 0.0f && g <= 0.0f) {
			r += speed;
			b -= speed;
		}

		colorShiftTimer = clock();
	}
}

VOID ShowMessageBox(LPCWSTR title, LPCWSTR msg, DWORD dwFlags)
{
	LPCWSTR* text;
	text = new LPCWSTR[1];
	text[0] = L"OK";
	PXOVERLAPPED over = new XOVERLAPPED();
	memset(over, 0, sizeof(XOVERLAPPED));
	PMESSAGEBOX_RESULT result = new MESSAGEBOX_RESULT();
	memset(result, 0, sizeof(MESSAGEBOX_RESULT));

	WCHAR retSeq[] = L"\\r\\n";
	int len = lstrlenW(msg);
	WCHAR *str = new WCHAR[len + 1];
	lstrcpyW(str, msg);
	for (int i = 0; i <= len + 4; i += 1) {
		if ((*(BYTE*)&str[i]) + 1 == 0)
			break;

		if (memcmp(&str[i], retSeq, 8) == 0) {
			str[i] = '\r';
			str[i + 1] = '\n';
			memcpy((void*)(&str[i + 1] + 1), &str[i + 3] + 1, ((len * 2) - (i * 2)) + 2);
		}
	}

	while (XShowMessageBoxUI(0, title, str, 1, text, 0, dwFlags, result, over) == ERROR_ACCESS_DENIED)
		free(str);
	XINPUT_STATE mars;
	XInputGetState(0, &mars);

	while (!XHasOverlappedIoCompleted(over) && (!(result->dwButtonPressed & XINPUT_GAMEPAD_B) & !(mars.Gamepad.wButtons & XINPUT_GAMEPAD_START))) {
		XInputGetState(0, &mars);
		Sleep(500);
	}
	Sleep(5000);
}

char DEVICE_DYNAMIC[MAX_PATH];
PCHAR GetMountPath() {
	//printf("Grabbing dynamic mount path!\n");
	std::wstring ws;
	PLDR_DATA_TABLE_ENTRY TableEntry;
	XexPcToFileHeader((PVOID)0x90e00000, &TableEntry);

	ws = TableEntry->FullDllName.Buffer;
	std::string FullDllName(ws.begin(), ws.end());

	ws = TableEntry->BaseDllName.Buffer;
	std::string BaseDllName(ws.begin(), ws.end());

	std::string::size_type i = FullDllName.find(BaseDllName);

	if (i != std::string::npos)
		FullDllName.erase(i, BaseDllName.length());

	memset(DEVICE_DYNAMIC, 0x0, MAX_PATH);
	strcpy(DEVICE_DYNAMIC, FullDllName.c_str());

	return DEVICE_DYNAMIC;
}

IMAGE_SECTION_HEADER* FindNtSection(IMAGE_SECTION_HEADER* Sections, WORD SectionCount, CHAR* SectionName)
{
	// Go through and search for our section
	for (WORD x = 0; x < SectionCount; x++)
	{
		if (strcmp((CHAR*)Sections[x].Name, SectionName) == 0)
			return &Sections[x];
	}

	return NULL;
}

DWORD ReverseDWORD(DWORD in)
{
	DWORD ni = 0;

	DWORD size = sizeof(DWORD);

	DWORD c = 0;
	for (; c < (size * 8); ++c)
	{
		int olp = ((size - (c / 8) - 1) * 8) + c % 8;
		if ((1 << olp) & in)
		{
			ni |= 1 << c;
		}
	}
	return ni;
}

WORD ReverseWORD(WORD in)
{
	WORD ni = 0;

	DWORD size = sizeof(WORD);

	DWORD c = 0;
	for (; c < (size * 8); ++c)
	{
		int olp = ((size - (c / 8) - 1) * 8) + c % 8;
		if ((1 << olp) & in)
		{
			ni |= 1 << c;
		}
	}
	return ni;
}

HRESULT doMountPath(const char* szDrive, const char* szDevice, const char* sysStr) {
	STRING DeviceName, LinkName;
	CHAR szDestinationDrive[MAX_PATH];
	sprintf_s(szDestinationDrive, MAX_PATH, sysStr, szDrive);
	RtlInitAnsiString(&DeviceName, szDevice);
	RtlInitAnsiString(&LinkName, szDestinationDrive);
	ObDeleteSymbolicLink(&LinkName);
	return (HRESULT)ObCreateSymbolicLink(&LinkName, &DeviceName);
}

HRESULT MountPath(const char* szDrive, const char* szDevice, BOOL both) {
	HRESULT res;
	if (both)
	{
		res = doMountPath(szDrive, szDevice, SYS_STRING);
		res = doMountPath(szDrive, szDevice, USR_STRING);
	}
	else
	{
		if (KeGetCurrentProcessType() == 2) //SYSTEM_PROC
			res = doMountPath(szDrive, szDevice, SYS_STRING);
		else
			res = doMountPath(szDrive, szDevice, USR_STRING);
	}
	return res;
}


FLOAT RandomFloat(FLOAT fMin, FLOAT fMax) {
	return fMin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (fMax - fMin)));
}

FLOAT GetAngleDelta(FLOAT firstAngle, FLOAT secondAngle) {
	FLOAT difference = secondAngle - firstAngle;
	while (difference < -180) difference += 360;
	while (difference > 180) difference -= 360;
	return difference;
}

int inline GetIntersection(float fDst1, float fDst2, vec3_t P1, vec3_t P2, vec3_t &Hit) {
	if ((fDst1 * fDst2) >= 0.0f) return 0;
	if (fDst1 == fDst2) return 0;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return 1;
}

int inline InBox(vec3_t Hit, vec3_t B1, vec3_t B2, const int Axis) {
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return 1;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return 1;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return 1;
	return 0;
}

// returns true if line (L1, L2) intersects with the box (B1, B2)
// returns intersection point in Hit
bool CheckLineBox(vec3_t B1, vec3_t B2, vec3_t L1, vec3_t L2, vec3_t &Hit)
{
	if (L2.x < B1.x && L1.x < B1.x) return false;
	if (L2.x > B2.x && L1.x > B2.x) return false;
	if (L2.y < B1.y && L1.y < B1.y) return false;
	if (L2.y > B2.y && L1.y > B2.y) return false;
	if (L2.z < B1.z && L1.z < B1.z) return false;
	if (L2.z > B2.z && L1.z > B2.z) return false;
	if (L1.x > B1.x && L1.x < B2.x &&
		L1.y > B1.y && L1.y < B2.y &&
		L1.z > B1.z && L1.z < B2.z)
	{
		Hit = L1;
		return true;
	}
	if ((GetIntersection(L1.x - B1.x, L2.x - B1.x, L1, L2, Hit) && InBox(Hit, B1, B2, 1))
		|| (GetIntersection(L1.y - B1.y, L2.y - B1.y, L1, L2, Hit) && InBox(Hit, B1, B2, 2))
		|| (GetIntersection(L1.z - B1.z, L2.z - B1.z, L1, L2, Hit) && InBox(Hit, B1, B2, 3))
		|| (GetIntersection(L1.x - B2.x, L2.x - B2.x, L1, L2, Hit) && InBox(Hit, B1, B2, 1))
		|| (GetIntersection(L1.y - B2.y, L2.y - B2.y, L1, L2, Hit) && InBox(Hit, B1, B2, 2))
		|| (GetIntersection(L1.z - B2.z, L2.z - B2.z, L1, L2, Hit) && InBox(Hit, B1, B2, 3)))
		return true;

	return false;
}

void RotatePoint(vec3_t *point, /*const*/ vec3_t matrix[3]) { // FIXME 
	vec3_t tvec = *point;
	point->x = DotProduct((FLOAT*)&matrix[0], (FLOAT*)&tvec);
	point->y = DotProduct((FLOAT*)&matrix[1], (FLOAT*)&tvec);
	point->z = DotProduct((FLOAT*)&matrix[2], (FLOAT*)&tvec);
}

void AngleVectors(vec3_t *angles, vec3_t *forward, vec3_t *right, vec3_t *up) {
	float            angle;
	static float    sr, sp, sy, cr, cp, cy;

	angle = (float)Radians(angles->x);
	sp = sin(angle);
	cp = cos(angle);

	angle = (float)Radians(angles->y);
	sy = sin(angle);
	cy = cos(angle);

	angle = (float)Radians(angles->z);
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}
	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}
	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

VOID RemoveTrailingZerosFromFloatingString(PCHAR pTxt) {
	if (pTxt) {
		int iEnd = strlen(pTxt);
		if (iEnd > 2) {
			for (; iEnd > 2; iEnd--) {
				if (pTxt[iEnd] == '0' && pTxt[iEnd-1] != '.') {
					pTxt[iEnd] = 0;
				}
			}
		}
	}
}

float AngleNormalize(float angle)
{
	while (angle < -180) angle += 360;
	while (angle > 180) angle -= 360;
	return angle;
}

bool IsXUIDValid(QWORD xuid) {
	PBYTE bXuid = (PBYTE)&xuid;
	return (bXuid[0] == 0x0 && bXuid[1] == 0x09);
}

double Radians(float Degrees)
{
	return  0.01745329238474369 * Degrees;
}

VOID BinToFile(const char* fname, PVOID dPtr, ULONG len) {
	if (MmIsAddressValid(dPtr)) {
		FILE* fp;
		if (fopen_s(&fp, fname, "wb") == 0) {
			DbgPrint("Writing file \"%s\"", fname);
			fwrite(dPtr, len, 1, fp);
			fflush(fp);
			fclose(fp);
		}
	}
}

bool Contains(char *str1, char *str2) {
	if (str1 != NULL && str2 != NULL)
		if (strstr(str1, str2) != NULL)
			return true;
	return false;
}

BOOL Compare(PCHAR str1, PCHAR str2, BOOL caseSensitive ) {
	if (str1 && str2) {
		if (strlen(str1) == strlen(str2)) {
			if (caseSensitive) {
				return strcmp(str1, str2) == 0;
			}
			else {
				return stricmp(str1, str2) == 0;
			}
		}
	}
	return FALSE;
}

DOUBLE Get3dDistance(vec3_t vPos1, vec3_t vPos2)
{
	return sqrt(
		pow((DOUBLE)(vPos2.x - vPos1.x), 2.0) +
		pow((DOUBLE)(vPos2.y - vPos1.y), 2.0) +
		pow((DOUBLE)(vPos2.z - vPos1.z), 2.0)
	);
}

signed char ClampChar(int i)
{
	if (i < -128) {
		return -128;
	}
	if (i > 127) {
		return 127;
	}
	return i;
}

void AxisToAngles(const vec3_t axis[3], vec3_t *angles)
{
	float	length1;
	float	yaw, pitch, roll = 0.0f;

	if (axis[0].y == 0 && axis[0].x == 0.0f)
	{
		yaw = 0.0f;
		if (axis[0].z > 0.0f)
			pitch = 90.0f;
		else
			pitch = 270.0f;
	}
	else
	{
		if (axis[0].x)
			yaw = (float)(atan2(axis[0].y, axis[0].x) * 180 / M_PI);
		else if (axis[0].y > 0.0f)
			yaw = 90.0f;
		else
			yaw = 270.0f;
		if (yaw < 0.0f)
			yaw += 360.0f;

		length1 = sqrt(axis[0].x * axis[0].x + axis[0].y * axis[0].y);
		pitch = (float)(atan2(axis[0].z, length1) * 180 / M_PI);
		if (pitch < 0.0f)
			pitch += 360.0f;

		roll = (float)(atan2(axis[1].z, axis[2].z) * 180 / M_PI);
		if (roll < 0.0f)
			roll += 360.0f;
	}
	angles->x = -pitch;
	angles->y = yaw;
	angles->z = roll;

}

bool FileExists(const CHAR * FileName)
{
	bool success = true;
	HANDLE hFile;
	hFile = CreateFileA(FileName,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//printf("[CFileExists] CreateFile failed with error code %i.\n", GetLastError());
		return false;
	}
	else
	{
		CloseHandle(hFile);
		return success;
	}
	CloseHandle(hFile);
	return false;
}

PCHAR GetRandomSubDomain() {
	int iValue = rand() % 2;
	if (iValue > 1) {
		iValue = 0;
	}
	return pServerNames[iValue];
}

BOOL LaunchSysThread(LPTHREAD_START_ROUTINE start, PVOID param, PHANDLE hThread, PDWORD dwThreadId, DWORD dwThreadPriority, BOOL bSpecial, DWORD dwStackSize) {
	DWORD flags = EX_CREATE_FLAG_SUSPENDED | EX_CREATE_FLAG_SYSTEM;
	if (bSpecial == 2) {
		flags = EX_CREATE_FLAG_SUSPENDED | EX_CREATE_FLAG_CORE2 | EX_CREATE_FLAG_PRIORITY1 | 0x18000424;
	}
	else if (bSpecial) {
#ifdef ANTIHACK
		flags = EX_CREATE_FLAG_SUSPENDED | EX_CREATE_FLAG_SYSTEM | EX_CREATE_FLAG_CORE2 | EX_CREATE_FLAG_PRIORITY1 | 0x18000424;
#endif
	}
	HANDLE lThread;
	DWORD lThreadId;

	if (ExCreateThread(&lThread, dwStackSize, &lThreadId, (VOID*)XapiThreadStartup, start, param, flags) != ERROR_SUCCESS)
		return FALSE;
	if (!lThread)
		return FALSE;

	if (hThread)
		*hThread = lThread;
	if (dwThreadId)
		*dwThreadId = lThreadId;

	XSetThreadProcessor(lThread, __THREADCORE__);
	SetThreadPriority(lThread, dwThreadPriority);
	ResumeThread(lThread);
	return TRUE;
}

VOID PatchInJump(DWORD* Address, PVOID Dest, BOOL Linked) {

	DWORD Bytes[4];
	DWORD Destination = (DWORD)Dest;

	Bytes[0] = 0x3C000000 + ((Destination >> 16) & 0xFFFF);// lis 	%r0, dest>>16
	Bytes[1] = 0x60000000 + (Destination & 0xFFFF); // ori	%r0, %r0, dest&0xFFFF
	Bytes[2] = 0x7C0903A6; // mtctr	%r0
	Bytes[3] = 0x4E800420; // bctr

	if (Linked)
		Bytes[3] += 1; // bctrl

	 //memcpy(Address, Bytes, 0x10);
	*(__int64 *)((DWORD)Address + 0) = *(__int64 *)&Bytes[0];
	*(__int64 *)((DWORD)Address + 8) = *(__int64 *)&Bytes[2];
	__dcbst(0, Address);
	__sync();
	__isync();
}

string GetButtonCode(string sIn) {
	CHAR pSwap[100];
	ZeroMemory(pSwap, 100);

	if (sIn == "BUTTON_A") {
#if !defined(BO2) && !defined(BO3)
		sprintf(pSwap, "%c", 0x1);
#else
		sprintf(pSwap, ES("^BBUTTON_LUI_PRIMARY^"));
#endif
	}
	else if (sIn == "BUTTON_X") {
#if !defined(BO2) && !defined(BO3)
		sprintf(pSwap, "%c", 0x3);
#else
		sprintf(pSwap, ES("^BBUTTON_LUI_ALT1^"));
#endif
	}
	else if (sIn == "BUTTON_RB") {
#if !defined(BO2) && !defined(BO3)
		sprintf(pSwap, "%c", 0x6);
#else
		sprintf(pSwap, ES("^BXENONButtonshoulderR^"));
#endif
	}
	else if (sIn == "BUTTON_LB") {
#if !defined(BO2) && !defined(BO3)
		sprintf(pSwap, "%c", 0x5);
#else
		sprintf(pSwap, ES("^BXENONButtonshoulderL^"));
#endif
	}
	else if (sIn == "DPAD_LEFT") {
#if !defined(BO2) && !defined(BO3)
		sprintf(pSwap, "%c", 0x16);
#else
		sprintf(pSwap, ES("^BXENONButtondpadL^"));
#endif
	}
	else if (sIn == "BUTTON_LT") {
#if !defined(BO2) && !defined(BO3)
		sprintf(pSwap, "%c", 0x12);
#else
		sprintf(pSwap, ES("^BXENONButtontrigL^"));
#endif
	}
	else if (sIn == "BUTTON_B") {
#if !defined(BO2) && !defined(BO3)
		sprintf(pSwap, "%c", 0x2);
#else
		sprintf(pSwap, ES("^BXENONButtonB^"));
#endif
	}
	return string(pSwap);
}

DWORD GetModuleImportAddr(HANDLE HModule, HANDLE HImportedModule, DWORD Ordinal) {

	PLDR_DATA_TABLE_ENTRY_2 Module = (PLDR_DATA_TABLE_ENTRY_2)HModule;

	DWORD address = (DWORD)GetProcAddress((HMODULE)HImportedModule, (LPCSTR)Ordinal);

	if (address == NULL || HModule == NULL)
		return 0;

	// Get our header field from this module
	VOID* headerBase = Module->XexHeaderBase;
	PXEX_IMPORT_DESCRIPTOR importDesc = (PXEX_IMPORT_DESCRIPTOR)
		RtlImageXexHeaderField(headerBase, 0x000103FF);
	if (importDesc == NULL)
		return 0;

	// Get our string table position
	CHAR* stringTable = (CHAR*)(importDesc + 1);

	// Get our first entry
	PXEX_IMPORT_TABLE_2 importTable = (PXEX_IMPORT_TABLE_2)
		(stringTable + importDesc->NameTableSize);

	// Loop through our table
	for (DWORD x = 0; x < importDesc->ModuleCount; x++) {

		// Go through and search all addresses for something that links
		DWORD* importAdd = (DWORD*)(importTable + 1);
		for (DWORD y = 0; y < importTable->ImportTable.ImportCount; y++) {

			// Check the address of this import
			DWORD value = *((DWORD*)importAdd[y]);
			if (value == address) {
				return (DWORD)(&importAdd[y + 1]);
				//return importAdd[y + 1];
			}
		}

		// Goto the next table
		importTable = (PXEX_IMPORT_TABLE_2)(((BYTE*)importTable) +
			importTable->TableSize);
	}

	return 0;
}

PCHAR ReplaceButtonsInString(PCHAR pString) {
jRestart:
	if (pString) {
		PCHAR pStart = strstr(pString, "[[");
		if (pStart) {
			pStart += 2;
			PCHAR pEnd = strstr(pStart, "]]");
			if (pEnd) {
				string sNew(pString);
				string sSub(pStart, pEnd - pStart);

				replace(sNew, "[[" + sSub + "]]", GetButtonCode(sSub));

				// If we successfully freed the old description string, allocate a new one
				if (sManager.free(pString)) {
					PCHAR pOut = NULL;
					sManager.alloc(sNew.size() + 1, &pOut);

					if (pOut) {
						memcpy(pOut, sNew.c_str(), sNew.size());
						pString = pOut;

						goto jRestart;
					}
				}
				// If we failed to free the current description text, we forgot to add the EA macro to the current description
				else {
					DbgPrintE("Description \"%s\" is missing EA() wrapper! Fix your shit!", pString);
				}
			}
		}
	}
	return pString;
}

string ReplaceButtonsInString(string pString) {
jRestart:
	PCHAR pStart = strstr((PCHAR)pString.c_str(), "[[");
	if (pStart) {
		pStart += 2;
		PCHAR pEnd = strstr(pStart, "]]");
		if (pEnd) {
			string sNew(pString);
			string sSub(pStart, pEnd - pStart);

			replace(sNew, "[[" + sSub + "]]", GetButtonCode(sSub));

			pString = sNew;
			goto jRestart;
		}
	}
	return pString;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

string RemoveChars(const string& source, const string& chars, BOOL ignoreBetweenQuotes) {
	string result = "";
	BOOL betweenQuotes = FALSE;
	for (unsigned int i = 0; i<source.length(); i++) {

		if (ignoreBetweenQuotes) {
			if (source[i] == '"' && !betweenQuotes) {
				betweenQuotes = TRUE;
			}
			else if (source[i] == '"' && betweenQuotes) {
				if (i > 0 && source[i - 1] == '\\') {
					// Skip this quote... it's been escaped so it must be within the string we're checking, not the end of it.
				}
				else {
					betweenQuotes = FALSE;
				}
			}
		}

		BOOL foundany = FALSE;
		for (unsigned int j = 0; j<chars.length() && !foundany; j++) {
			foundany = (source[i] == chars[j]) && !betweenQuotes;
		}
		if (!foundany) {
			result += source[i];
		}
	}
	return result;
}

unsigned int rgb(double hue) {
	int h = int(hue * 256 * 6);
	int x = h % 0x100;

	int r = 0, g = 0, b = 0;
	switch (h / 256) {
	case 0: r = 255; g = x;       break;
	case 1: g = 255; r = 255 - x; break;
	case 2: g = 255; b = x;       break;
	case 3: b = 255; g = 255 - x; break;
	case 4: b = 255; r = x;       break;
	case 5: r = 255; b = 255 - x; break;
	}

	return r + (g << 8) + (b << 16);
}

string StripComments(string& source) {
	string result = source;
	size_t pos = 0;
	PCHAR pCharSet = "/;#";
	for (DWORD i = 0; i < 3; i++) {
		pos = source.find(pCharSet[i]);
		if (pos != string::npos) {
			DWORD dwQuotesBefore = 0, dwQuotesAfter = 0;
			for (DWORD c = 0; c < source.size(); c++) {
				if (source[c] == '\"') {
					if (c > 0 && source[c - 1] != '\\') {
						if (c < pos) {
							dwQuotesBefore++;
						}
						else if (c > pos) {
							dwQuotesAfter++;
						}
					}
				}
			}
			if (dwQuotesBefore > 0 && dwQuotesAfter > 0) {
				// We found our replacement character between quotes, so leave it be
				continue;
			}
			result = result.substr(0, pos);
		}
	}
	return result;
}

vec3_t vectoangles(float value1, float value2, float value3)
{
	float forward;
	float yaw, pitch;
	vec3_t angles;
	if (value1 == 0 && value2 == 0)
	{
		yaw = 0;
		if (value3 > 0) pitch = 90.00;
		else pitch = 270.00;
	}
	else
	{
		if (value1 != -1) yaw = (float)(atan2((double)value2, (double)value1) * 180.00 / M_PI);
		else if (value2 > 0) yaw = 90.00;
		else yaw = 270;
		if (yaw < 0) yaw += 360.00;

		forward = (float)sqrt((double)(value1 * value1 + value2 * value2));
		pitch = (float)(atan2((double)value3, (double)forward) * 180.00 / M_PI);
		if (pitch < 0) pitch += 360.00;
	}
	angles.x = -pitch;
	angles.y = yaw;
	angles.z = 0;
	return angles;
}