//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#ifndef _QUICKCONFIGUI_H_
#define _QUICKCONFIGUI_H__

#ifndef MAX_SSID_LEN
#define MAX_SSID_LEN 32
#endif // MAX_SSID_LEN

#define MAX_WZC_STATUS					(35+MAX_SSID_LEN)


/* ZeroConfig Quick Config Message Queue for logging */

#define LOG_STATUS_SIZE			(MAX_WZC_STATUS + 60)
#define WZCLOG_ENTRIES			15
#define WZCLOG_UNINITIALIZED	0x0
#define WZCLOG_INACTIVE			0x1
#define WZCLOG_ACTIVE			0x2

typedef struct _WZCLogMsgQueue
{
	TCHAR				pszList[WZCLOG_ENTRIES][LOG_STATUS_SIZE];
	DWORD				dwOldMsgIdx;
	DWORD				dwNewMsgIdx;
	DWORD				dwState;
	CRITICAL_SECTION	Lock;
}
WZCLogMsgQueue;


// ZeroConfig Logging dialog functions
BOOL CALLBACK WzcLoggingDialogProc(HWND hwndDlg, UINT 	uMsg, WPARAM	wParam, LPARAM	lParam);
HRESULT AddLoggingMsg(TCHAR* ptcDeviceName, UCHAR* pucMacAddr, FILETIME ft, TCHAR* ptcMessage);
DWORD WINAPI WzcLoggingThread(LPVOID pv);
HRESULT	InitializeLoggingQueue();
void UnloadLoggingQueue();
void UpdateWZCLogView();
DWORD GetWZCLogState();
void SaveStateToRegistry();
void LoadStateFromRegistry();




#endif
