#include "XBLSMsgr.h"

BOOL XBLSMsgr::InitPatches() {
	//DbgPrint("InitPatches: Function Start");

	if (!isReady()) {
		//DbgPrint("InitPatches: ERR - [MSGR] not ready");
		return FALSE;
	}

	if (Not(NiNJA.cPatches)->bHasPatches) {
		//DbgPrint("InitPatches: ERR - Already init");
		return FALSE;
	}

	//Request patches to be filled from server
	if (!SendRecvMsg(PATCHES)) {
		//DbgPrint("InitPatches: ERR - Message exchange failed");
		return FALSE;
	}

	if (!Not(NiNJA.cPatches)->bHasPatches) {
		//DbgPrint("InitPatches: ERR - MSG Exchange Success but patches not loaded");
		return FALSE;
	}

	//DbgPrint("InitPatches: Success!");
	return TRUE;
}

BOOL XBLSMsgr::SendRecvMsg(XBLS_CMDS cmd) {
	if (!isReady()) {
		return FALSE;
	}
	pXBLSMsg nMsg = new XBLSMsg();
	nMsg->nextMsg = NULL;
	nMsg->CMD = cmd;
	nMsg->Identifier = myIdent;
	nMsg->Data = NULL;
	//Send msg and wait for response
	if (!SendMsg(nMsg) || !WaitForResponse()) {
		//DbgPrint("[MSGR]: SendRecvMsg Send ERR");
		return FALSE;
	}

	//Receive Data
	if (FAILED(CheckInbox())) {
		//DbgPrint("[MSGR]: SendRecvMsg Recv ERR");
		return FALSE;
	}

	//Success!
	return TRUE;
}

BOOL XBLSMsgr::SendMsg(pXBLSMsg Msg) {
	if (!isReady()) {
		return FALSE;
	}
	if (Msg == NULL)
		return FALSE;

	Msg->nextMsg = MsgBox->Outbox;
	MsgBox->Outbox = Msg;

	return TRUE;
}

NTSTATUS XBLSMsgr::CheckInbox() {

	if (!isReady())
		return E_FAIL;

	if (MsgBox->Inbox == NULL)
		return ERROR_SUCCESS;

	pXBLSMsg nMsg = NULL;
	DWORD size = 0;
	PBYTE pbPatches = NULL;

	if (MsgBox->Inbox->Identifier == __MSGRIDENT__) {
		//DbgPrint("[MSGR]: Received message!");
	}
	else {
		//DbgPrint("[MSGR]: ERR: CheckInbox, wrong ident");
		PopInbox();
		return E_FAIL;
	}

	switch (MsgBox->Inbox->CMD) {
	case PATCHES:
		//DbgPrint("[MSGR]: Parsing patch data message");

		if (MsgBox->Inbox->Data == NULL || MsgBox->Inbox->Size == 0) {
			//DbgPrint("[MSGR]: ERR: Message sanity check fail");
			return E_FAIL;
		}
		//DbgPrint("Copying data to patches");

		//Install patch data
		memcpy((PVOID)&Not(NiNJA.cPatches)->CoD, (PBYTE)MsgBox->Inbox->Data, sizeof(CheatPatches_s));

		Not(NiNJA.cPatches)->bHasPatches = true;

		//DbgPrint("Installed patch data successfully");

		//Clean Up
		//XPhysicalFree(MsgBox->Inbox->Data);
		PopInbox();
		break;

	default:
		return E_FAIL;
		break;
	}

	return ERROR_SUCCESS;
}