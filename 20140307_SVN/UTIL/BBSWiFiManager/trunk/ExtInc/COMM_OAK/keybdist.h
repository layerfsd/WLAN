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

#ifndef _KEYBDIST_H_
#define _KEYBDIST_H_

typedef UINT (*PFN_KEYBD_PDD_GET_KEYBD_EVENT)(UINT uiPddId, 
    UINT32 rguiScanCode[16], BOOL rgfKeyUp[16]);

typedef struct tagKEYBD_IST {
    HANDLE hevInterrupt;
    DWORD dwSysIntr_Keybd;
    UINT uiPddId;
    PFN_KEYBD_PDD_GET_KEYBD_EVENT pfnGetKeybdEvent;
    PFN_KEYBD_EVENT pfnKeybdEvent;
} KEYBD_IST, *PKEYBD_IST;

BOOL
KeybdIstLoop(
	PKEYBD_IST pKeybdIst
	);

#endif
