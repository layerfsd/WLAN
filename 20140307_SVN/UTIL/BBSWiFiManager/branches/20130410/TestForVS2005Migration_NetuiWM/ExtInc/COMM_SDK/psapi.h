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


Module Name: psapi.h

Purpose: Declaration of Process Status Helper APIs.

--*/

#ifndef _PSAPI_H_
#define _PSAPI_H_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MODULEINFO {
    LPVOID lpBaseOfDll;
    DWORD SizeOfImage;
    LPVOID EntryPoint;
} MODULEINFO, *LPMODULEINFO;


#if !defined (IN_KERNEL) && !defined (COREDLL)

BOOL
WINAPI
GetModuleInformation(
    HANDLE hProcess,
    HMODULE hModule,
    __out_bcount(cb) LPMODULEINFO lpmodinfo,
    __in DWORD cb
    );

BOOL 
WINAPI 
CeSetProcessVersion (
    __in HANDLE hProcess,
    __in DWORD dwVersion
    );

#endif

#ifdef __cplusplus
}
#endif

#endif
