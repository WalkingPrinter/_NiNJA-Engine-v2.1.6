#pragma once
#include "Globals.h"
#include "Debug.h"

enum XBLS_CMDS : DWORD {
	AUTH = 0x1,
	PATCHES = 0x2,
	PING = 0x4
};

typedef struct _Msg {
	XBLS_CMDS CMD;
	DWORD Identifier;
	DWORD* Data;
	DWORD Size;
	_Msg* nextMsg;
} XBLSMsg, *pXBLSMsg;

typedef struct _MsgBox {
	DWORD Identifier;
	pXBLSMsg Inbox;
	pXBLSMsg Outbox;
} XBLSMsgBox, *pXBLSMsgBox;

enum XBLSMSGR_IDENTS : DWORD {
	NINJA_IDENT = 'NINJ'
};

#define MAX_MSG 255

class XBLSMsgr
{
public:
	XBLSMsgBox* MsgBox;
	BOOL isRunning;

	DWORD myIdent;

	NTSTATUS CheckInbox();

	BOOL isReady() {
		return (MsgBox != NULL && isRunning);
	}

	pXBLSMsg* GetLastOutboxMsg() {
		if(!isReady())
			return NULL;
		pXBLSMsg* tmp = &MsgBox->Outbox;
		for (int i = 0; i < MAX_MSG && *tmp != NULL; i++) {
			tmp = &(*tmp)->nextMsg;
		}
		return tmp;
	}

	BOOL SendMsg(pXBLSMsg Msg);

	VOID PopInbox() {
		if (MsgBox->Inbox == NULL)
			return;
		pXBLSMsg tmp = MsgBox->Inbox->nextMsg;
		//free(MsgBox->Inbox);
		if (tmp != NULL)
			MsgBox->Inbox = tmp;
		else
			MsgBox->Inbox = NULL;
	}

	BOOL SendRecvMsg(XBLS_CMDS cmd);
	BOOL InitPatches();

	BOOL WaitForResponse() {
		for (int i = 0; (!isReady() || MsgBox->Inbox == NULL) && i < 20000; i += 1000) {
			Sleep(1000);
		}
		return MsgBox->Inbox != NULL;
	}

	VOID SetMsgBox(pXBLSMsgBox mBox) {
		if (mBox == NULL)
			return;
		MsgBox = mBox;
		isRunning = TRUE;
	}

	XBLSMsgr() {
		isRunning = FALSE;
		MsgBox = NULL;
		myIdent = __MSGRIDENT__;
	}
	~XBLSMsgr() {
		//isRunning = FALSE;
		//if (MsgBox == NULL)
		//	return;
		//for (int i = 0; i < MAX_MSG; i++) {
		//	if (MsgBox->Inbox != NULL) {
		//		pXBLSMsg tmp = MsgBox->Inbox->nextMsg;
		//		free(MsgBox->Inbox);
		//		if (tmp != NULL)
		//			MsgBox->Inbox = tmp;
		//		else
		//			break;
		//	}
		//}
		//for (int i = 0; i < MAX_MSG; i++) {
		//	if (MsgBox->Outbox != NULL) {
		//		pXBLSMsg tmp = MsgBox->Outbox->nextMsg;
		//		free(MsgBox->Outbox);
		//		if (tmp != NULL)
		//			MsgBox->Outbox = tmp;
		//		else
		//			break;
		//	}
		//}
	}
};