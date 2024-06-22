#include "HudElemProcessing.h"
#include "Globals.h"
#include "GameFuncs.h"

vector<HudElem*> HudElems::hudElems;

BOOL HudElems::IsHudElemInitialized(HudElem *elem) {
	for (DWORD i = 0; i<hudElems.size(); i++)
		if (hudElems[i] == elem)
			return TRUE;
	return FALSE;
}

bool hudElemSort_Internal(const HudElem *c1, const HudElem *c2) {
	return c1->sSort < c2->sSort;
}

VOID HudElems::SortHudElems() {
	sort(hudElems.begin(), hudElems.end(), hudElemSort_Internal);
}

BOOL isSortPending = FALSE;
VOID HudElems::InitHudElem(HudElem *elem) {
	// Make sure the hudelem isnt already initialized before we continue, to ensure we dont process it more than once
	if (IsHudElemInitialized(elem)) {
		return;
	}
	hudElems.push_back(elem);
	elem->bInitialized = true;
	//elem->initTime = GetTickCount();
	isSortPending = TRUE;
}

VOID HudElems::DestroyHudElem(HudElem *elem) {
	for(DWORD i=0; i<hudElems.size(); i++) {
		if(hudElems[i] == elem) {
			hudElems[i]->dwFlags |= ELEM_FLAG_DESTROY;
			return;
		}
	}
	DbgPrint("Couldn't find specified hudElem.");
}

VOID HudElems::TransitionFloatArray(const FLOAT *from, const FLOAT *to, int arraySize, DWORD startTime, int transLen, FLOAT *out) {
	FLOAT percent = (FLOAT)(transLen - ((startTime + transLen) - NiNJA.dwTickCount)) / transLen * 100.0f;
	for (int i = 0; i < arraySize; i++) {
		out[i] = to[i];
		FLOAT difference = abs(from[i] - to[i]);
		FLOAT distance = difference / 100.0f;
		if (to[i] > from[i])
			out[i] = from[i] + distance * percent;
		else if (to[i] < from[i])
			out[i] = from[i] - distance * percent;
		else
			out[i] = to[i];
	}
}

VOID HudElem::SetPos(FLOAT setX, FLOAT setY, BOOL bMoveOverTime, DWORD dwDuration, DWORD dwStartTime) {
	if (!bMoveOverTime) {
		fX = setX;
		fY = setY;
	}
	else {
		HudElems::MoveOverTime(this, setX, setY, dwDuration, dwStartTime);
	}
}

VOID HudElem::SetScale(FLOAT setWidth, FLOAT setHeight, BOOL bScaleOverTime, DWORD dwDuration, DWORD dwStartTime) {
	if (!bScaleOverTime) {
		fWidth = setWidth;
		fHeight = setHeight;
	}
	else {
		HudElems::ScaleOverTime(this, setWidth, setHeight, dwDuration, dwStartTime);
	}
}

string HudElems::WrapText(PCHAR pText, FLOAT fMaxWidth, FLOAT fScale, DWORD dwAsset, BYTE *dwNumLinesWrapped) {
	DWORD bNumLinesWrapped = 0;
	// Allocate a bit of extra space so we have some room to move our array back if we need to insert a new character
	string sRetValue(pText, strlen(pText) + 5);
	PCHAR pSeek = (PCHAR)sRetValue.data(), pLastPos = NULL, pLastSeek = NULL;
	DWORD dwAdditave = 0;
	CHAR cReplace = 0, cLastReplace = 0;
	while (true) {
		// Find our first breakable character in the string
		PCHAR pReplaceChars = " /", pPos = (PCHAR)0x7fffffff;
		for (DWORD i = 0; i < 2; i++) {
			PCHAR pTmp = strchr(pSeek + dwAdditave, pReplaceChars[i]);
			if (pTmp) {
				if (pTmp < pPos) {
					pPos = pTmp;
					cReplace = pReplaceChars[i];
				}
			}
		}

		// If we found a valid position
		if ((DWORD)pPos != 0x7fffffff) {
			// Swap out the character we found with a null terminator
			*pPos = '\0';

			// Grab the width of the current block of text
			FLOAT fWidth = GameFuncs::GetTextWidth(pSeek, dwAsset) * fScale;

			// Put the replace char back into the string
			*pPos = cReplace;

			// If our string is still shorter than the max, put the character back in and continue on until its too long
			if (fWidth < fMaxWidth) {
				// Increment our additave based off the difference between the position we found the space and the starting position, so we know how far to seek on the next iteration
				dwAdditave = (pPos + 1) - pSeek;

				// Store our last position and replace character for the next loop, if we need to insert a newline next iteration
				cLastReplace = cReplace;
				pLastPos = pPos;
			}
			// Otherwise, insert a newline character at the previous position (before we went past the max size)
			else {
				bNumLinesWrapped++;
				// If its a space, we can just replace it with a newline
				if (cLastReplace == ' ') {
					// Replace the space with a newline
					*pLastPos = '\n';

					// Then store the old seek, and increment it for the next iteration
					pLastSeek = pSeek;
					pSeek = pLastPos + 1;
				}
				// If its not a space we cant just replace it... so add in a new character
				else {
					// Grab the string length, and store a backup of it
					size_t sLen = strlen(pLastPos);
					string pBkup(pLastPos);

					// Copy the entire contents of the string back in memory 1 byte, to account for our newly inserted character
					memcpy(pLastPos + 1, pBkup.c_str(), sLen);

					// Terminate the newly created string, as we can't be sure the last few characters we allocated were null alrady
					*(CHAR*)(pLastPos + 1 + sLen) = 0;

					// Insert a newline after the character
					*(CHAR*)(pLastPos + 1) = '\n';

					// Then store the old seek, and increment it for the next iteration
					pLastSeek = pSeek;
					pSeek = pLastPos + 2;
				}
				// Reset our additave
				dwAdditave = 0;
			}
		}
		else {
			// One final check to make sure we don't have a final line to break

			// Grab the width of the last block of text we newlined, to see if its still longer than the max length
			FLOAT fWidth = GameFuncs::GetTextWidth(pLastSeek ? pLastSeek : pSeek, dwAsset) * fScale;

			// If it is, insert one last newline
			if (fWidth >= fMaxWidth) {
				bNumLinesWrapped++;
				*pLastPos = '\n';
			}

			//DbgPrintE("Final: %s", sRetValue.c_str());
			bNumLinesWrapped++;
			break;
		}
	}
	*dwNumLinesWrapped = bNumLinesWrapped;
	return sRetValue;
}

string HudElem::Wrap() {
	PCHAR pText = this->pText;
	FLOAT fMaxWidth = this->fWrapWidth, fScale = this->fFontScale;
	BYTE bNumLinesWrapped = 0;
	string ret = HudElems::WrapText(pText, fMaxWidth, fScale, this->dwAsset, &bNumLinesWrapped);
	this->bNumLinesWrapped = bNumLinesWrapped;
	return ret;
}


const FLOAT RainbowFade[6][4] =
{
	{ 1.0f, 0.0f, 0.0f, 0.8f }, //r
	{ 1.0, 1.0, 0.0, 0.8f }, //y
	{ 0.0, 1.0, 0.0, 0.8f }, //g
	{ 0.0, 1.0, 1.0, 0.8f }, //c
	{ 0.0, 0.0, 1.0, 0.8f }, //b
	{ 1.0, 0.0, 1.0, 0.8f } //m
};

bool MoveOverTimeEvent::Process(PVOID parent, HudElemEventOut *pOutInfo) {
	HudElem *pParent = (HudElem*)parent;
	if (dwStartTime < NiNJA.dwTickCount && NiNJA.dwTickCount < dwStartTime + dwDuration) {
		//DbgPrint("Move event in progress! (%f, %f to %f, %f)", pParent->fX, pParent->fY, vToPosition.x, vToPosition.y);
		//DbgPrintE("Processing MoveOverTime! StartTime: %X, TickCount: %X", dwStartTime, NiNJA.dwTickCount);
		FLOAT current[2], to[2], final[2];
		current[0] = vFromPosition.x;
		current[1] = vFromPosition.y;
		to[0] = vToPosition.x;
		to[1] = vToPosition.y;
		HudElems::TransitionFloatArray(current, to, 2, dwStartTime, dwDuration, final);
		pOutInfo->vToPosition.x = final[0];
		pOutInfo->vToPosition.y = final[1];
		return true;
	}
	else {
		pOutInfo->vToPosition = vFromPosition;
	}
	return false;
}

bool ScaleOverTimeEvent::Process(PVOID parent, HudElemEventOut *pOutInfo) {
	HudElem *pParent = (HudElem*)parent;
	if (dwStartTime < NiNJA.dwTickCount && NiNJA.dwTickCount < dwStartTime + dwDuration) {
		if(pParent->dwFlags & ELEM_TYPE_MATERIAL) {
			// Shader scaling
			FLOAT current[2], to[2], final[2];
			current[0] = vFromScale.x;
			current[1] = vFromScale.y;
			to[0] = vToScale.x;
			to[1] = vToScale.y;
			HudElems::TransitionFloatArray(current, to, 2, dwStartTime, dwDuration, final);
			pOutInfo->vToScale.x = final[0];
			pOutInfo->vToScale.y = final[1];
		}
		else {
			// Font scaling
			FLOAT finalScale;
			HudElems::TransitionFloatArray(&pParent->fFontScale, &fToScale, 1, dwStartTime, dwDuration, &finalScale);
			pOutInfo->fToScale = finalScale;
		}
		return true;
	}
	else {
		pOutInfo->vToScale = vFromScale;
		pOutInfo->fToScale = fFromScale;

	}
	return false;
}

bool FadeOverTimeEvent::Process(PVOID parent, HudElemEventOut *pOutInfo) {
	HudElem *pParent = (HudElem*)parent;
	if (dwStartTime < NiNJA.dwTickCount && NiNJA.dwTickCount < dwStartTime + dwDuration) {
		FLOAT finalColor[4];
		HudElems::TransitionFloatArray(pFromColor, pToColor, 4, dwStartTime, dwDuration, finalColor);
		memcpy(pOutInfo->pToColor, finalColor, 0x10);
		return true;
	}
	else {
		memcpy(pOutInfo->pToColor, pFromColor, 0x10);
	}
	return false;
}

VOID HudElems::MoveOverTime(HudElem *pElem, FLOAT fToX, FLOAT fToY, DWORD dwDuration, DWORD dwStartTime) {
	if (pElem->bVisible) {
		MoveOverTimeEvent *pNewEvent = new MoveOverTimeEvent(vec2_t(fToX, fToY), dwDuration, dwStartTime);
		pNewEvent->vFromPosition.Set(pElem->fX, pElem->fY);
		pElem->QueueEvent(pNewEvent);
	}
	// During an event, the new menu value is set before being completely applied, for positioning reasons
	pElem->fX = fToX;
	pElem->fY = fToY;
}

VOID HudElems::ScaleOverTime(HudElem *pElem, FLOAT fToWidth, FLOAT fToHeight, DWORD dwDuration, DWORD dwStartTime) {
	if (pElem->bVisible) {
		ScaleOverTimeEvent *pNewEvent = new ScaleOverTimeEvent(vec2_t(fToWidth, fToHeight), dwDuration, dwStartTime);
		pNewEvent->vFromScale.Set(pElem->fWidth, pElem->fHeight);
		pElem->QueueEvent(pNewEvent);
	}
	// During an event, the new menu value is set before being completely applied, for positioning reasons
	pElem->fWidth = fToWidth;
	pElem->fHeight = fToHeight;
}

VOID HudElems::FadeOverTime(HudElem *pElem, FLOAT *pToColor, DWORD dwDuration, DWORD dwStartTime) {
	if (pElem->bVisible) {
		FadeOverTimeEvent *pNewEvent = new FadeOverTimeEvent(pToColor, dwDuration, dwStartTime);
		memcpy(pNewEvent->pFromColor, pElem->pColor, 0x10);
		pElem->QueueEvent(pNewEvent);
	}
	// During an event, the new menu value is set before being completely applied, for positioning reasons
	memcpy(pElem->pColor, pToColor, 0x10);
}

vector<DWORD> pRemovals;
VOID HudElems::ProcessHudElems() {
	// If we have a sort pending, do it before we loop through all our elems
	if (isSortPending) {
		SortHudElems();
		isSortPending = FALSE;
	}

	for(DWORD i=0; i<hudElems.size(); i++) {
		if (NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
			break;
		}
		if (hudElems[i] != NULL) {
			//// If we're dealing with a destructible elem, check if it's passed its lifetime and if it has, destroy it
			//if (hudElems[i]->lifetime != 0 && (int)NiNJA.dwTickCount > hudElems[i]->initTime + hudElems[i]->lifetime) {
			//	hudElems[i]->destroyElem = TRUE;
			//}

			// If the hudelem has been marked for destruction
			if (hudElems[i]->dwFlags & ELEM_FLAG_DESTROY) {
				// Set its initialization state to false so it doesn't get processed anymore
				hudElems[i]->bInitialized = false;

				// Mark our hudElem for safe removal from the list after all hudElems have been drawn
				pRemovals.push_back(i);
				continue;
			}
			if (hudElems[i]->bInitialized) {
				if (hudElems[i]->bVisible) {
					// Setup our base hudelem settings, which will be modified by our event queue if required
					FLOAT *setColor = hudElems[i]->pColorBuff;
					memcpy(setColor, hudElems[i]->pColor, 0x10);
					FLOAT setX = hudElems[i]->fX;
					FLOAT setY = hudElems[i]->fY;
					FLOAT setWidth = hudElems[i]->fWidth;
					FLOAT setHeight = hudElems[i]->fHeight;
					FLOAT setFontscale = hudElems[i]->fFontScale;

					// Process each of our events we have queued up
					DWORD dwEventQueueSize = hudElems[i]->GetEventQueueSize();
					if (dwEventQueueSize > 0) {
						for (int e = dwEventQueueSize - 1; e >= 0; e--) {
							// Process our hudelem event
							HudElemEventOut pEventOut;
							HudElemEvent *pElemEvent = hudElems[i]->GetEvent(e);
							if (pElemEvent) {
								pElemEvent->Process(hudElems[i], &pEventOut);

								// If the elem is older than its lifetime, finalize the event and clean it up
								if (pElemEvent->dwStartTime + pElemEvent->dwDuration <= NiNJA.dwTickCount) {
									switch (pElemEvent->dwType) {
										case ELEM_EVENT_MOVE:
											setX = hudElems[i]->fX;
											setY = hudElems[i]->fY;
											break;
										case ELEM_EVENT_FADE:
											memcpy(setColor, hudElems[i]->pColor, 0x10);
											break;
										case ELEM_EVENT_SCALE:
											setWidth = hudElems[i]->fWidth;
											setHeight = hudElems[i]->fHeight;
											setFontscale = hudElems[i]->fFontScale;
											break;
									}

									//DbgPrintE("Event complete! Cleaning up %i!", e);

									// Erase the elem from the queue
									hudElems[i]->EraseEvent(e);

									// Delete the event to free up memory
									SAFE_DELETE(pElemEvent);

									//DbgPrintE("SizeOf vEventQueue for elem %i is %i", i, hudElems[i]->vEventQueue.size());
								}
								// Otherwise, set the position the event returned
								else {
									switch (pElemEvent->dwType) {
									case ELEM_EVENT_MOVE:
										setX = pEventOut.vToPosition.x;
										setY = pEventOut.vToPosition.y;
										break;
									case ELEM_EVENT_FADE:
										memcpy(setColor, pEventOut.pToColor, 0x10);
										break;
									case ELEM_EVENT_SCALE:
										setWidth = pEventOut.vToScale.x;
										setHeight = pEventOut.vToScale.y;
										setFontscale = pEventOut.fToScale;
										break;
									}
								}
							}
						}
					}

					// Set our static alpha to the setcolor if relevant
					if (hudElems[i]->fAlpha != -1.0f)
						setColor[3] = hudElems[i]->fAlpha;

					if (hudElems[i]->dwFlags & ELEM_TYPE_TEXT) {
						string sDecrypted, sWrapped;
						// If the current elem has the encrypted flag, decrypt the data before we display it
						if (hudElems[i]->dwFlags & ELEM_FLAG_ENCRYPTED) {
							if (hudElems[i]->pSecureValue) {
								sDecrypted = hudElems[i]->pSecureValue->Get();
								hudElems[i]->pText = (PCHAR)sDecrypted.c_str();
							}
						}

						// Wrap our text if need be
						if (hudElems[i]->dwFlags & ELEM_FLAG_WRAP) {
							sWrapped = hudElems[i]->Wrap();
						}

						// Draw our text elem
						if ((hudElems[i]->dwFlags & ELEM_FLAG_BORDER) == 0) {
							GameFuncs::DrawText(hudElems[i]->dwFlags & ELEM_FLAG_WRAP ? (PCHAR)sWrapped.c_str() : hudElems[i]->pText, NULL, hudElems[i]->dwAsset, setX, setY, hudElems[i]->fFontScale, hudElems[i]->fFontScale, hudElems[i]->fRotation, setColor);
						}
						else {
							GameFuncs::DrawTextWithBorder(hudElems[i]->dwFlags & ELEM_FLAG_WRAP ? (PCHAR)sWrapped.c_str() : hudElems[i]->pText, hudElems[i]->dwAsset, setX, setY, hudElems[i]->fFontScale, hudElems[i]->fFontScale, hudElems[i]->fBorderWidth, hudElems[i]->fRotation, setColor, hudElems[i]->pBorderColor);
						}
						// If we were drawing encrypted text, clear the decrypted version that's still in memory
						if (hudElems[i]->dwFlags & ELEM_FLAG_ENCRYPTED) {
							if (hudElems[i]->pSecureValue) {
								sDecrypted.clear();
								hudElems[i]->pText = NULL;
							}
						}

						// Cleanup our wrapped text
						if (hudElems[i]->dwFlags & ELEM_FLAG_WRAP) {
							sWrapped.clear();
						}
					}
					else if (hudElems[i]->dwFlags & ELEM_TYPE_MATERIAL) {
						// Draw our material elem
						if ((hudElems[i]->dwFlags & ELEM_FLAG_BORDER) == 0) {
							GameFuncs::DrawRotatedPic(setX, setY, setWidth, setHeight, hudElems[i]->fRotation, setColor, hudElems[i]->dwAsset);
						}
						else {
							GameFuncs::DrawBox(setX, setY, setWidth, setHeight, hudElems[i]->fRotation, setColor, hudElems[i]->pBorderColor, hudElems[i]->fBorderWidth);
						}
					}
				}
			}
		}
	}
	// Safely remove any hudElems that were destroyed during this loop
	if (pRemovals.size() > 0) {
		for (DWORD i = 0; i < pRemovals.size(); i++) {
			std::vector<HudElem*>::iterator it = hudElems.begin();
			advance(it, pRemovals[i]);
			hudElems.erase(it);
		}
		pRemovals.clear();
	}
}