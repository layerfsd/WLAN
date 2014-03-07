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
//////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999-2000 Microsoft Corporation.  All rights reserved. //
//////////////////////////////////////////////////////////////////////////


#pragma once

#ifndef _ExTAPIPRIV_H_
#define _ExTAPIPRIV_H_


#include <windows.h>


#ifdef __cplusplus
extern "C" {
#endif


// Line barring modes
#define LINECALLCMD_RELEASEHELD                0x0000001
#define LINECALLCMD_RELEASEACTIVE_ACCEPTHELD   0x0000002
#define LINECALLCMD_RELEASECALL                0x0000003
#define LINECALLCMD_HOLDACTIVE_ACCEPTHELD      0x0000004
#define LINECALLCMD_HOLDALLBUTONE              0x0000005
#define LINECALLCMD_ADDHELDTOCONF              0x0000006
#define LINECALLCMD_ADDHELDTOCONF_DISCONNECT   0x0000007
#define LINECALLCMD_INVOKECCBS                 0x0000008
#define LINECALLCMD_SETUPCALL_ACCEPT           0x0000009


LONG WINAPI lineManageCalls(
    HLINE hLine,
    DWORD dwCommand,
    DWORD dwID
);


#ifdef __cplusplus
}
#endif


#endif _ExTAPIPRIV_H_
