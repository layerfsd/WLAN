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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

prapi.h

Abstract:

Contains private portion of the Remote API

Notes: 


--*/

#ifndef PRAPI_H
#define PRAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNDER_CE
#define GetDccManWindow         RAPI_EXP_10
#define CeEventHasOccurred     RAPI_EXP_11
#define CeSyncTimeToPc         RAPI_EXP_12
#define CeStartReplication     RAPI_EXP_13
#define CeRegCopyFile          RAPI_EXP_14
#define CeRegRestoreFile       RAPI_EXP_15
#define CeSetPasswordActive    RAPI_EXP_16
#define CeSetPassword          RAPI_EXP_17
#endif // UNDER_CE

STDAPI          GetDccManWindow(HWND *phwnd);

STDAPI_(BOOL  ) CeEventHasOccurred  (LONG, LPWSTR);
STDAPI_(BOOL  ) CeSyncTimeToPc      (DWORD);
STDAPI          CeStartReplication  (void);
STDAPI_(BOOL  ) CeRegCopyFile       (LPCWSTR);
STDAPI_(BOOL  ) CeRegRestoreFile    (LPCWSTR);
STDAPI_(BOOL  ) CeSetPasswordActive (BOOL, LPWSTR);
STDAPI_(BOOL  ) CeSetPassword       (LPWSTR, LPWSTR);

#ifdef __cplusplus
}
#endif

#endif // #ifndef PRAPI_H
