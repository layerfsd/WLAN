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
// ExecuteDialString in CPROG
#ifndef __EXECDIAL_H__
#define __EXECDIAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CPM_DIALSTRING      (WM_USER + 126)

typedef struct tagDialStringInfo
{
    DWORD dwFlags;
    DWORD dwDestAddressOffset;
    DWORD dwAppNameOffset;
    DWORD dwCalledPartyOffset;
    DWORD dwCommentOffset;
} DIALSTRINGINFO;

LONG ExecuteDialString(DWORD dwFlags, PCWSTR pszDestAddress, PCWSTR pszAppName, 
                       PCWSTR pszCalledParty, PCWSTR pszComment);

// ExecuteDialString flags
#define EDSF_SECURE               0x00000001
#define EDSF_PROMPTBEFORECALLING    0x00000002
#define EDSF_CONFIRMDIALREQUEST    0x00000004
#define EDSF_DIALONLINE1    0x00000008
#define EDSF_DIALONLINE2    0x00000010


#ifdef __cplusplus
}
#endif

#endif // __EXECDIAL_H_
