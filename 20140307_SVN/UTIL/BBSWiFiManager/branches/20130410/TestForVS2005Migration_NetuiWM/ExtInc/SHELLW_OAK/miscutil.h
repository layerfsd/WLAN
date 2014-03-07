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
#ifndef _MISCUTIL_H_
#define _MISCUTIL_H_
// Copyright (c) 1994-1999, Microsoft Corp. All rights reserved.
//***   miscutil.h --
//

#ifdef __cplusplus
extern "C" {
#endif

#if 0 // todo: maybe?
void ArgInit(void *pvArg, int cbSize);
#endif

// for GetAppHWND below
#define awiDialer       0
#define awiStart        1
#define awiHome         2
#define awiCallProgress 3
#define awiSettings     4
#define awiLast         5 // always last item

extern HWND GetAppHWND(int id, BOOL fSleepRetry);
extern BOOL ClearAppHWND(int id, HWND hwndApp);
extern HRESULT SetForegroundApp(int id);
extern BOOL HomeHideOwnedWindows(BOOL fShow);
    
#ifdef DEBUG
TCHAR *Dbg_MaskToMneStr(UINT uMask, TCHAR *szMnemonics);
#else
/* it's an error to reference a Dbg_xxx routine (so no extern)! */
#endif

typedef BOOL (*PFN_SUBCLASS_CALLBACK)(UINT uMsg, WPARAM wParam, LPARAM lParam, DWORD dwUserData, LRESULT* plRes);
HRESULT SHSubclassWindow(HWND hwnd, DWORD dwUserData, PFN_SUBCLASS_CALLBACK pfnCallback);
HRESULT SHSubclassRemove(__in HWND hwnd);

BOOL SHDialogAutoClose(HWND hwndDialog, WORD wID);

// Return's Shell window or NULL on failure.
HWND SHGetShellWindow();

// Send Shell window a message. Returns the return value of SendMesage/PostMessage
// or 0 on failure.
int SHSendShellMessage(
    UINT uMsg,      // Message
    WPARAM wp,      // WPARAM
    LPARAM lp,      // LPARAM
    BOOL fSend      // TRUE to Send. FALSE to Post
);

#ifdef DEBUG
// not exported from aygshell.dll
void GetNextSpanTest();
#endif

// Alignment options for GetNextSpan
typedef enum GNS_Alignment
    {
    GNS_LEFT,
    GNS_CENTER,
    GNS_RIGHT,
    GNS_TILE,
    } GNS_Alignment;
// See function implementation for documentation
HRESULT GetNextSpan(GNS_Alignment iAlignment, int iBoundsStart, int iBoundsEnd,
    int iEnumStart, int iEnumEnd, int dSpan, int* piCurrentSpanStart, int* piCurrentSpanEnd);

#ifdef __cplusplus
}
#endif

#endif // _MISCUTIL_H_
