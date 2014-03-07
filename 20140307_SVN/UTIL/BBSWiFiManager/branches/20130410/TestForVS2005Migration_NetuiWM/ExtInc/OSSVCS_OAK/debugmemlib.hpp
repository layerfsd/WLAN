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
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyright (c) Microsoft Corporation

Module Name:

    DebugMemLib.hpp

Abstract:

    Header file for the Memory Tracking library

-------------------------------------------------------------------*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// ++++ Defines +++++++++++++++++++++++++++++++++++++++++++++++

#ifndef SHIP_BUILD
#ifndef RC_INVOKED

BOOL WINAPI OSSVCSLMEMAddTrackedItem(HANDLE hHeap, LPVOID pAllocMem, DWORD dwBytes, DWORD dwLineNum, LPCSTR szFilename);
VOID WINAPI OSSVCSLMEMRemoveTrackedItem(HANDLE hHeap, LPVOID pAllocMem);

#endif //RC_INVOKED

#else //SHIP BUILD

//Define these as nothing in SHIP_BUILD (aka WINCESHIP=1)
#define OSSVCSLMEMAddTrackedItem(h, p, b, l, f)
#define OSSVCSLMEMRemoveTrackedItem(h, p)

#endif //SHIP_BUILD

#ifdef __cplusplus
};
#endif // __cplusplus

