#pragma once
#include <xtl.h>
#include "string"
#include <stdio.h>
#include "Patches.h"

using namespace std;

#define MOUNT_NAME "NINJA:"
#define SYS_STRING "\\System??\\%s"
#define USR_STRING "\\??\\%s"
#define DEVICE_HDD "\\Device\\Harddisk0\\Partition1\\"
#define DEVICE_USB "\\Device\\Mass0"

float AngleNormalize(float angle);
FLOAT RandomFloat(FLOAT fMin, FLOAT fMax);

#ifndef __VECTOR_H__
#define __VECTOR_H__

class vec2 {
public:
	vec2();
	vec2(float x, float y);

	bool operator==(const vec2 vec);
	bool operator!=(const vec2 vec);
	bool empty() {
		return x == 0.0f && y == 0.0f;
	}

	float distance(const vec2 vec);

	float x, y;
};

class vec3 {
public:
	vec3();
	vec3(float X, float Y, float Z);

	vec3 cross(const vec3 vec);
	vec3 scale(float scale);
	vec3 lerp(vec3 end, float time);
	vec3 negate();
	vec3 convertToAngles();

	bool operator!=(const vec3 vec);
	bool operator==(const vec3 vec);
	bool copy(vec3 dataToCopy);

	vec3 operator-(const vec3 vec);
	vec3 operator-(const float value);
	vec3 operator+(const vec3 vec);
	vec3 operator+(const float value);
	vec3 operator*(const vec3 vec);
	vec3 operator*(const float value);
	vec3 operator/(const float value);
	vec3 operator=(const float* value);
	vec3 operator+=(const vec3 vec);
	vec3 operator()(float x, float y, float z);
	vec3 operator-=(const vec3 vec);
	vec3 operator-=(const float value);
	vec3 operator*=(const vec3 vec);
	vec3 operator*=(const float value);

	float length() const;
	float magnitude() const;
	float distanceTo(vec3 pos);
	float dot(vec3& vec);

	void normalize();
	void normalizeAngles();
	void zero();

	float x, y, z;
};

class vec4 {
public:
	vec4();
	vec4(float X, float Y, float Z, float W);

	vec4 lerp(vec4 vec, vec4 endpos, float time);

	bool operator!=(const vec4 vec);
	bool operator==(const vec4 vec);

	vec4 operator-(const vec4 vec);
	vec4 operator+(const vec4 vec);
	vec4 operator*(const vec4 vec);
	vec4 operator*(const float value);

	void zero();

	float x, y, z, w;
};

class colors {
public:
	void colorShift(float speed);

	colors();
	colors(float red, float green, float blue, float alpha);

	float r, g, b, a;
	int colorShiftTimer;
};

#endif //__VECTOR_H__

FLOAT GetAngleDelta(FLOAT firstAngle, FLOAT secondAngle);
HRESULT doMountPath(const char* szDrive, const char* szDevice, const char* sysStr);
struct vec2_t {
	FLOAT x;
	FLOAT y;


	VOID Erase() {
		this->x = 0;
		this->y = 0;
	}

	VOID Set(FLOAT x, FLOAT y) {
		this->x = x;
		this->y = y;
	}

	VOID Set(vec2_t &vIn) {
		this->x = vIn.x;
		this->y = vIn.y;
	}

	vec2_t& operator-=(const vec2_t& other) {
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	vec2_t& operator+=(const vec2_t& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	bool IsWithin(vec2_t& fAmount, vec2_t& other) {
		return (abs(this->x - other.x) <= fAmount.x) && (abs(this->y - other.y) <= fAmount.y);
	}

	bool IsZero() {
		return this->x == 0 && this->y == 0;
	}

	vec2_t(FLOAT x, FLOAT y) {
		Set(x, y);
	}

	vec2_t() {
		Set(0, 0);
	}
};

struct vec3_t  {
	FLOAT x;
	FLOAT y;
	FLOAT z;

	VOID AddRandom(FLOAT fMin, FLOAT fMax) {
		x += RandomFloat(fMin, fMax);
		y += RandomFloat(fMin, fMax);
		z += RandomFloat(fMin, fMax);
	}

	VOID Erase() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	VOID Set(FLOAT x, FLOAT y, FLOAT z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	VOID Set(vec3_t &vIn) {
		this->x = vIn.x;
		this->y = vIn.y;
		this->z = vIn.z;
	}

	vec3_t& operator-=(const vec3_t& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}

	vec3_t& operator+=(const vec3_t& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	vec3_t& operator*=(const vec3_t& other) {
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		return *this;
	}

	vec3_t& operator/=(const vec3_t& other) {
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;
		return *this;
	}


	vec3_t operator()(float x, float y, float z) {
		return vec3_t(x + x, y + y, z + z);
	}

	bool operator==(const vec3_t vec) {
		return (x == vec.x && y == vec.y && z == vec.z);
	}

	bool operator!=(const vec3_t vec) {
		return !(x == vec.x && y == vec.y && z == vec.z);
	}

	vec3_t operator+(const vec3_t vec) {
		return vec3_t(x + vec.x, y + vec.y, z + vec.z);
	}

	vec3_t operator+(const float value) {
		return vec3_t(x + value, y + value, z + value);
	}

	vec3_t operator-(const vec3_t vec) {
		return vec3_t(x - vec.x, y - vec.y, z - vec.z);
	}

	vec3_t operator-(const float value) {
		return vec3_t(x - value, y - value, z - value);
	}

	vec3_t operator*(const vec3_t vec) {
		return vec3_t(x * vec.x, y * vec.y, z * vec.z);
	}

	vec3_t operator*(const float value) {
		return vec3_t(x * value, y * value, z * value);
	}

	vec3_t operator/(const float value) {
		return vec3_t(x / value, y / value, z / value);
	}

	vec3_t operator=(const float* value) {
		return *(vec3_t*)&value[0];
	}

	vec3_t operator*=(const vec3_t vec) {
		return vec3_t(x *= vec.x, y *= vec.y, z *= vec.z);
	}

	vec3_t operator*=(const float value) {
		return vec3_t(x *= value, y *= value, z *= value);
	}

	vec3_t operator+=(const vec3_t vec) {
		return vec3_t(x += vec.x, y += vec.y, z += vec.z);
	}

	vec3_t operator-=(const vec3_t vec) {
		return vec3_t(x -= vec.x, y -= vec.y, z -= vec.z);
	}

	vec3_t operator-=(const float value) {
		return vec3_t(x -= value, y -= value, z -= value);
	}

	vec3_t operator-() const {
		return vec3_t(-x, -y, -z);
	}

	vec3_t negate() {
		return *this * -1.0f;
	}

	float length() const {
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	bool IsWithin(FLOAT fAmount, vec3_t& other) {
		return (abs(this->x - other.x) <= fAmount) && (abs(this->y - other.y) <= fAmount) && (abs(this->z - other.z) <= fAmount);
	}

	bool IsWithin(vec3_t& fAmount, vec3_t& other) {
		return (abs(this->x - other.x) <= fAmount.x) && (abs(this->y - other.y) <= fAmount.y) && (abs(this->z - other.z) <= fAmount.z);
	}

	bool IsWithinAngle(vec3_t& fAmount, vec3_t& other) {
		return (abs(x - (int)other.x % 360) <= fAmount.x || (360 - abs(x - (int)other.x % 360)) <= fAmount.x) && (abs(y - (int)other.y % 360) <= fAmount.y || (360 - abs(y - (int)other.y % 360)) <= fAmount.y) && (abs(z - (int)other.z % 360) <= fAmount.z || (360 - abs(z - (int)other.z % 360)) <= fAmount.z);
	}

	bool IsZero() {
		return this->x == 0 && this->y == 0 && this->z == 0;
	}

	void Normalize() {
		x = AngleNormalize(x);
		y = AngleNormalize(y);
		z = AngleNormalize(z);
	}

	void SetIfLessThan(vec3_t val) {
		if (val.x < x) {
			x = val.x;
		}
		if (val.y < y) {
			y = val.y;
		}
		if (val.z < z) {
			z = val.z;
		}
	}

	void SetIfGreaterThan(vec3_t val) {
		if (val.x > x) {
			x = val.x;
		}
		if (val.y > y) {
			y = val.y;
		}
		if (val.z > z) {
			z = val.z;
		}
	}

	vec3_t(FLOAT x, FLOAT y, FLOAT z) {
		Set(x, y, z);
	}

	vec3_t() {
		Set(0, 0, 0);
	}
};

struct vec4_t {
	FLOAT x;
	FLOAT y;
	FLOAT z;
	FLOAT a;

	VOID Erase() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->a = 0;
	}

	VOID Set(FLOAT x, FLOAT y, FLOAT z, FLOAT a) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->a = a;
	}

	VOID Set(vec4_t &vIn) {
		this->x = vIn.x;
		this->y = vIn.y;
		this->z = vIn.z;
		this->a = vIn.a;
	}

	vec4_t& operator-=(const vec4_t& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->a -= other.a;
		return *this;
	}

	vec4_t& operator+=(const vec4_t& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->a += other.a;
		return *this;
	}

	vec4_t& operator-(const vec4_t& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->a -= other.a;
		return *this;
	}

	vec4_t& operator+(const vec4_t& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->a += other.a;
		return *this;
	}

	bool IsZero() {
		return this->x == 0 && this->y == 0 && this->z == 0 && this->a == 0;
	}

	void SetIfLessThan(vec4_t val) {
		if (val.x < x) {
			x = val.x;
		}
		if (val.y < y) {
			y = val.y;
		}
		if (val.z < z) {
			z = val.z;
		}
		if (val.a < a) {
			a = val.a;
		}
	}

	void SetIfGreaterThan(vec4_t val) {
		if (val.x > x) {
			x = val.x;
		}
		if (val.y > y) {
			y = val.y;
		}
		if (val.z > z) {
			z = val.z;
		}
		if (val.a > a) {
			a = val.a;
		}
	}

	vec4_t(FLOAT x, FLOAT y, FLOAT z, FLOAT a) {
		Set(x, y, z, a);
	}

	vec4_t() {
		Set(0, 0, 0, 0);
	}
};

typedef struct _XEX_IMPORT_TABLE_2 {
	DWORD                 TableSize;
	HV_IMAGE_IMPORT_TABLE ImportTable;
} XEX_IMPORT_TABLE_2, *PXEX_IMPORT_TABLE_2;

typedef struct _LDR_DATA_TABLE_ENTRY_2 {
	LIST_ENTRY     InLoadOrderLinks;
	LIST_ENTRY     InClosureOrderLinks;
	LIST_ENTRY     InInitializationOrderLinks;
	VOID*          NtHeadersBase;
	VOID*          ImageBase;
	DWORD          SizeOfNtImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	DWORD          Flags;
	DWORD          SizeOfFullImage;
	VOID*          EntryPoint;
	WORD           LoadCount;
	WORD           ModuleIndex;
	VOID*          DllBaseOriginal;
	DWORD          CheckSum;
	DWORD          ModuleLoadFlags;
	DWORD          TimeDateStamp;
	VOID*          LoadedImports;
	VOID*          XexHeaderBase;
	union {
		ANSI_STRING               LoadFileName;
		struct {
			_LDR_DATA_TABLE_ENTRY* ClosureRoot;
			_LDR_DATA_TABLE_ENTRY* TraversalParent;
		} asEntry;
	};
} LDR_DATA_TABLE_ENTRY_2, *PLDR_DATA_TABLE_ENTRY_2;

struct GeoIpInfo {
	char city[40];
	char stateFull[40];
	char country[40];
	char isp[40];
	GeoIpInfo() {
		ZeroMemory(city, 40);
		ZeroMemory(stateFull, 40);
		ZeroMemory(country, 40);
		ZeroMemory(isp, 40);
	}
	VOID Reset() {
		sprintf(city, "Loading...");
		sprintf(stateFull, "Loading...");
		sprintf(country, "Loading...");
		sprintf(isp, "Loading...");
	}
};

struct sScoreboardInfo {
	DWORD dwScore;
	DWORD dwKills;
	DWORD dwDeaths;
	DWORD dwAssists;
	DWORD dwPing;
	DWORD dwRank;
	FLOAT fKillDeathRatio;
	sScoreboardInfo() {
		dwScore = 0;
		dwKills = 0;
		dwDeaths = 0;
		dwAssists = 0;
		dwPing = 0;
		dwRank = 0;
		fKillDeathRatio = 0.0f;
	}
};

struct orientation_t {
	vec3_t origin;
	vec3_t axis[3];
};

class ShieldInfo {
public:
	orientation_t cOrientation;
	vec3_t pBounds[4]; // TopRight, TopLeft, BottomLeft, BottomRight
};

//typedef vec_t vec2_t[2];
//typedef vec_t vec3_t[3];
//typedef vec_t vec4_t[4];
//typedef vec_t vec5_t[5];

template <typename Iter>
VOID rot13(Iter begin, const Iter& end) {

	while (begin != end) {

		//Doesn't need to be here, but I'm lazy and don't like
		//typing *begin over and over again.
		char& c = *begin;

		if (c >= 'a' && c <= 'm') {
			c += 13;
		}
		else if (c >= 'n' && c <= 'z') {
			c -= 13;
		}
		else if (c >= 'A' && c <= 'M') {
			c += 13;
		}
		else if (c >= 'N' && c <= 'Z') {
			c -= 13;
		}

		++begin;
	}
}

// Some utility defines/macros
#define AngleToShort(X) ((DWORD)((X) * 65536 / 360) & 65535)
#define	ShortToAngle(X) ((X) * (360.0f / 65536))
#define M_PI 3.14159265358979323846
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define RAD2DEG( a ) ( ( (a) * 180.0f ) / M_PI )
#define DEG2RAD( a ) ( ( (a) * M_PI ) / 180.0F )
#define DotProduct(x,y)     ((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define PITCH 0
#define YAW 1
#define ROLL 2
#define __NOP__ 0x60000000

#define __EMPTYGTSTRING__ "&$#%@$%@\0"
#define __EMPTYSTR__ ""
#define GetTitleId() (XamGetCurrentTitleId() ^ 0x710710)

#define ARRAY_SIZE(p) sizeof(p) / sizeof(p[0])

#define SETCOLOR(c) { c[0], c[1], c[2], c[3] }

#define ADDFLAG(v, f)  { \
	if((v & f) == 0) { \
		v |= f; \
	} \
} \

#define DELFLAG(v, f) { \
	if((v & f) == f) { \
		v &= ~f; \
	} \
} \

#define SAFE_FREE(p) { \
	if(p) { \
		free(p); \
	} \
} \

#define XSAFE_FREE(p) { \
	if(p) { \
		XPhysicalFree(p); \
	} \
} \

#define SAFE_DELETE(p) { \
	if(p) { \
		delete p; \
	} \
} \

#define SAFE_DELETE_ARRAY(p) { \
	if(p) { \
		delete[] p; \
	} \
} \

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if ( (x) != NULL && (x)->Release() == 0 ) { (x) = NULL; } }
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


// Inline functions
__forceinline Cheats_s *Not(Cheats_s *in) {
	//SENSITIVE_FUNCTION_START;
	return (Cheats_s*)~(DWORD)in;
}

VOID ShowMessageBox(LPCWSTR title, LPCWSTR msg, DWORD dwFlags = XMB_NOICON);

	PCHAR GetMountPath();

IMAGE_SECTION_HEADER * FindNtSection(IMAGE_SECTION_HEADER * Sections, WORD SectionCount, CHAR * SectionName);

DWORD ReverseDWORD(DWORD in);

WORD ReverseWORD(WORD in);

HRESULT MountPath(const char * szDrive, const char * szDevice, BOOL both);

bool CheckLineBox(vec3_t B1, vec3_t B2, vec3_t L1, vec3_t L2, vec3_t & Hit);

void AngleVectors(vec3_t * angles, vec3_t * forward, vec3_t * right, vec3_t * up);
VOID RemoveTrailingZerosFromFloatingString(PCHAR pTxt);
bool IsXUIDValid(QWORD xuid);
double Radians(float Degrees);
VOID BinToFile(const char* fname, PVOID dPtr, ULONG len); // Saves a blob of data to a file
bool Contains(char * str1, char * str2);
BOOL Compare(PCHAR str1, PCHAR str2, BOOL caseSensitive = TRUE); // Checks if 2 strings are equal
DOUBLE Get3dDistance(vec3_t vPos1, vec3_t vPos2);
signed char ClampChar(int i);
void AxisToAngles(const vec3_t axis[3], vec3_t *angles);
bool FileExists(const CHAR * FileName);
bool SpoofFileTime(const CHAR * FileName);
NTSTATUS writeFileToFlash(BYTE * buffer, char * fileName, DWORD len);
PCHAR GetRandomSubDomain();
BOOL LaunchSysThread(LPTHREAD_START_ROUTINE start, PVOID param = NULL, PHANDLE hThread = NULL, PDWORD dwThreadId = NULL, DWORD dwThreadPriority = THREAD_PRIORITY_NORMAL, BOOL bSpecial = TRUE, DWORD dwStackSize = 32000); // Launches a system thread with the specified characteristics
VOID PatchInJump(DWORD* Address, PVOID Destination, BOOL Linked); // Main function for hooking :)
string GetButtonCode(string sIn);
DWORD GetModuleImportAddr(HANDLE HModule, HANDLE HImportedModule, DWORD Ordinal);
PCHAR ReplaceButtonsInString(PCHAR pString);
string ReplaceButtonsInString(string pString);
bool replace(std::string & str, const std::string & from, const std::string & to);
string RemoveChars(const string& source, const string& chars, BOOL ignoreBetweenQuotes = FALSE);
unsigned int rgb(double hue);
string StripComments(string& source);
vec3_t vectoangles(float value1, float value2, float value3);
void RotatePoint(vec3_t *point, /*const*/ vec3_t matrix[3]);