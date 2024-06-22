#pragma once
#include <algorithm>
#include <cctype>
#include <memory>

#include "SecureStrings.h"
#include "Colors.h"


enum HudElemLayers {
	LAYER_ESP_FRIENDLY,
	LAYER_ESP_ENEMY,
	LAYER_RADAR,
	LAYER_KILLFEED_ESP,
	LAYER_MENU_BORDER,
	LAYER_MENU_BACKGROUND,
	LAYER_MENU_SLIDER,
	LAYER_MENU_CONTROL_BACKGROUND,
	LAYER_MENU_FOREGROUND,
	LAYER_TOPMOST
};

enum eHudFlags {
	ELEM_TYPE_TEXT = 1,
	ELEM_TYPE_MATERIAL = 2,
	ELEM_FLAG_ENCRYPTED = 4,
	ELEM_FLAG_BORDER = 8,
	ELEM_FLAG_WRAP = 16,
	ELEM_FLAG_DESTROY = 32
};

enum eElemEventTypes {
	ELEM_EVENT_MOVE,
	ELEM_EVENT_FADE,
	ELEM_EVENT_SCALE
};

class HudElemEventOut {
public:
	vec2_t vToPosition;
	vec2_t vToScale;
	FLOAT fToScale;
	FLOAT pToColor[4];
};

class HudElemEvent {
public:
	DWORD dwType;
	DWORD dwStartTime;
	DWORD dwDuration;

	virtual bool Process(PVOID pParent, HudElemEventOut *pOutInfo) { DbgPrint("This is shit");  return false; }; // Not implemented for the base class, must be defined in any classes which inherit this one
};

class HudElem {
private:
	vector<HudElemEvent*> vEventQueue; // A queue of events associated with this hudelem

public:
	bool bInitialized; // bInitialized; // Whether or not he hudElem has been initialized and is being actively displayed
	bool bVisible; // bVisible; // Whether or not the hudElem should be dispalyed
	SHORT sSort; // sSort; // The layer the hudElem should be dispalyed on

	DWORD dwFlags; // eHudFlags pertaining to this elem
	DWORD dwAsset; // The asset associated with this elem (font or material)
	PCHAR pText; // pThe text associated with this elem
	SecureString *pSecureValue; // For encrypted text elems

	FLOAT pColor[4]; // The color of the hudelem
	const FLOAT *pBorderColor; // The color of the border, for hudElems that have one
	FLOAT pColorBuff[4]; // A buffer to hold the current color while its transitioning between colors
	FLOAT fAlpha; // The alpha which will override whatever alpha is contained in the final color calculation

	FLOAT fFontScale; // The fontscale of the hudElem
	FLOAT fWidth; // The width of the hudElem
	FLOAT fHeight; // The height of the hudElem
	FLOAT fBorderWidth; // The width of the border, for hudElems that have one
	FLOAT fRotation; // The angle to draw the elem at 

	FLOAT fX; // The x position of the hudElem
	FLOAT fY; // The y position of the hudElem

	FLOAT fWrapWidth; // If the elem has the wrap text flag, the width at which to wrap it
	BYTE bNumLinesWrapped; // The number of lines that were wrapped by the last Wrap() function call

	VOID EraseEvent(DWORD dwEventIndex) {
		vEventQueue.erase(vEventQueue.begin() + dwEventIndex);
	}

	VOID QueueEvent(HudElemEvent *pEvent) {
		vEventQueue.push_back(pEvent);
	}

	DWORD GetEventQueueSize() {
		return vEventQueue.size();
	}

	HudElemEvent *GetEvent(DWORD dwIndex) {
		return vEventQueue[dwIndex];
	}

	VOID ClearEvents() {
		vEventQueue.clear();
	}

	VOID SetPos(FLOAT setX, FLOAT setY, BOOL bMoveOverTime = FALSE, DWORD dwDuration = 0, DWORD dwStartTime = 0);
	VOID SetScale(FLOAT fWidth, FLOAT fHeight, BOOL bScaleOverTime = FALSE, DWORD dwDuration = 0, DWORD dwStartTime = 0);

	VOID Initialize() {
		bInitialized = false;
		bVisible = false;
		sSort = 0;
		dwFlags = 0;
		dwAsset = 0;
		pText = 0;
		pSecureValue = 0;
		memcpy(pColor, Colors::White, 0x10);
		pBorderColor = Colors::BlackTrans;
		fAlpha = -1.0f;
		fFontScale = 0;
		fWidth = 0;
		fHeight = 0;
		fBorderWidth = 0;
		fRotation = 0;
		fX = 0;
		fY = 0;
		fWrapWidth = 0;
		bNumLinesWrapped = 0;
		memcpy(pColorBuff, Colors::White, 0x10);
	}

	VOID SetTextAndCountLines(PCHAR text) {
		this->pText = text;
		Wrap();
	}

	string Wrap();

	HudElem() {
		Initialize();
	}
};

class MoveOverTimeEvent : public HudElemEvent {
public:
	vec2_t vToPosition;
	vec2_t vFromPosition;
	virtual bool Process(PVOID pParent, HudElemEventOut *pOutInfo) override;

	MoveOverTimeEvent(vec2_t vToPosition, DWORD dwDuration, DWORD dwStartTime) {
		//DbgPrint("Created move event!");
		dwType = ELEM_EVENT_MOVE;
		this->dwDuration = dwDuration;
		this->dwStartTime = dwStartTime;
		this->vToPosition = vToPosition;
	}
};

class ScaleOverTimeEvent : public HudElemEvent {
public:
	vec2_t vToScale;
	vec2_t vFromScale;
	FLOAT fToScale;
	FLOAT fFromScale;
	virtual bool Process(PVOID pParent, HudElemEventOut *pOutInfo) override;

	ScaleOverTimeEvent(vec2_t vToScale, DWORD dwDuration, DWORD dwStartTime) {
		dwType = ELEM_EVENT_SCALE;
		this->dwDuration = dwDuration;
		this->dwStartTime = dwStartTime;
		this->vToScale = vToScale;
	}

	ScaleOverTimeEvent(FLOAT fToScale, DWORD dwDuration, DWORD dwStartTime) {
		dwType = ELEM_EVENT_SCALE;
		this->dwDuration = dwDuration;
		this->dwStartTime = dwStartTime;
		this->fToScale = fToScale;
	}
};


class FadeOverTimeEvent : public HudElemEvent {
public:
	FLOAT pToColor[4];
	FLOAT pFromColor[4];
	virtual bool Process(PVOID pParent, HudElemEventOut *pOutInfo) override;

	FadeOverTimeEvent(FLOAT *pToColor, DWORD dwDuration, DWORD dwStartTime) {
		dwType = ELEM_EVENT_FADE;
		this->dwDuration = dwDuration;
		this->dwStartTime = dwStartTime;
		memcpy(this->pToColor, pToColor, 0x10);
	}
};

namespace HudElems {
	 extern vector<HudElem*> hudElems;

	//VOID resetHudElemsMenu();
	VOID DestroyHudElem(HudElem *elem);
	VOID InitHudElem(HudElem *elem);
	BOOL IsHudElemInitialized(HudElem *elem);
	VOID MoveOverTime(HudElem * pElem, FLOAT fToX, FLOAT fToY, DWORD dwDuration, DWORD dwStartTime);
	VOID ScaleOverTime(HudElem * pElem, FLOAT fToWidth, FLOAT fToHeight, DWORD dwDuration, DWORD dwStartTime);
	VOID FadeOverTime(HudElem * pElem, FLOAT * pToColor, DWORD dwDuration, DWORD dwStartTime);
	VOID ProcessHudElems();
	VOID SortHudElems();
	VOID TransitionFloatArray(const FLOAT *from, const FLOAT *to, int arraySize, DWORD startTime, int transLen, FLOAT *out);
	string WrapText(PCHAR pText, FLOAT fMaxWidth, FLOAT fScale, DWORD dwAsset, BYTE * dwNumLinesWrapped);
};