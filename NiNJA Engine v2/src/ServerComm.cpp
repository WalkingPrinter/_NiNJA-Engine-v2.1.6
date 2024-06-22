#include "ServerComm.h"
#include "GameFuncs.h"
#include "UserCmd.h"
#include "Utilities.h"
#include "SecureStrings.h"
#include "UI.h"

namespace ServerComm {

	float fCustomAngles[][2] = {
		{ 999.0f, 999.0f },
		{ 13.0f, 13.0f },
		{ 71.0f, 37.0f },
		{ 42.0, 42.0f },
		{ 13.0f, 0.0f },
		{ 999.0f, 999.0f }
	};

	DWORD dwSequenceState[__MAXCLIENTS__];
	bool bSendCommandActive = false;
	DWORD dwCmdNum = 0;
	DWORD dwCmdTimer = 0;
	VOID ProcessClientCommand(int iCmdIndex) {
		SENSITIVE_FUNCTION_START;
		if (!bSendCommandActive) {
			for (int i = 0; i < __MAXCLIENTS__; i++) {
				dwSequenceState[i] = 0;
			}
			dwCmdTimer = 0;
			dwCmdNum = 0;
			bSendCommandActive = true;
		}

		if (GetTickCount() > dwCmdTimer) {
			dwCmdTimer = GetTickCount() + GameFuncs::GetPing() + 150;
			dwCmdNum++;
		}

		int iViewAngles = UserCmd::GetUserCmdViewAngles(UserCmd::GetCurrentUserCmd());
		vec3_t *spawnAngles = GameFuncs::GetSpawnAngles();

		if (spawnAngles) {
			switch (dwCmdNum) {
				case 1:
				case 2:
				case 3:
				case 4:
					//*(int*)iViewAngles = AngleToShort(fCustomAngles[dwCmdNum][0]);
					*(int*)(iViewAngles + 0x4) = AngleToShort(fCustomAngles[dwCmdNum][1] - spawnAngles->y);
					//printf(GameFuncs::va("Setting %f\n", ShortToAngle(*(int*)(iViewAngles + 0x4))));
					break;
				case 5:
					//*(int*)iViewAngles = AngleToShort((float)CG::dwSendCommandClientIndex);
					*(int*)(iViewAngles + 0x4) = AngleToShort((float)iCmdIndex - spawnAngles->y/*(float)CG::dwSendCommandType*/);
					//printf(GameFuncs::va("Finishing %f\n", ShortToAngle(*(int*)(iViewAngles + 0x4))));
					break;
				case 6:
					CG::bSendCustomCommand = FALSE;
					bSendCommandActive = false;
					break;

			}
		}
	}

	VOID ListenForClientCommand() {
		for (int i = 0; i < __MAXCLIENTS__; i++) {
			if (CG::cEntities.List[i].bValid && CG::cEntities.List[i].pExt && i != CG::iLocalClient) {
				// User moved on to the next sequence state, increment sequence index
				if (ceil(CG::cEntities.List[i].vAngles.y) == fCustomAngles[dwSequenceState[i]+1][1]) {
					dwSequenceState[i]++;
				}
				// User is still on the old seequence, keep waiting
				else if (ceil(CG::cEntities.List[i].vAngles.y) == fCustomAngles[dwSequenceState[i]][1]) {
					continue;
				}
				else {
					if (dwSequenceState[i] >= ARRAY_SIZE(fCustomAngles)-2) {
						int iCmd = (int)ceil(CG::cEntities.List[i].vAngles.y);
					#if !defined(ADMIN)
						if(iCmd == 69) {
							bool bFound = false;
							for (DWORD w = 0; w < CG::vWhitelisted.size(); w++) {
								if (CG::vWhitelisted[w] == CG::cEntities.List[i].pExt->cXuid) {
									bFound = true;
								}
							}
							if (!bFound) {
								CG::vWhitelisted.push_back(CG::cEntities.List[i].pExt->cXuid);
								UI::mManager.bShouldSave = TRUE;
							}
						}
						//printf(GameFuncs::va("Command: %i", (int)ceil(CG::cEntities.List[i].vAngles.y)));
					#endif
					}
					dwSequenceState[i] = 0;
				}
			}
		}
	}
}