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
    iplcommon.h

Abstract:
    IPL common header file. This file contains all the type and
    function definitions used by boot loader.

Functions:


Notes:

--*/
#ifndef _IPLCOMMON_H_
#define _IPLCOMMON_H_

// Message code enumeration.
//
typedef enum
{
    IPL_ERROR_BAD_IPLTOC,
    IPL_ERROR_BPINIT_FAILED,
    IPL_ERROR_OPENPARTITION_FAILED,
    IPL_ERROR_GETSTORAGETYPE_FAILED,
    IPL_ERROR_GETIMAGEINFO_FAILED,
    IPL_ERROR_TRANSLATEADDR_FAILED,
    IPL_ERROR_READDATA_FAILED,
    IPL_ERROR_JUMP_FAILED,
    IPL_INFO_LOADING_IMAGE,
    IPL_INFO_JUMPING_IMAGE
} IPL_MESSAGE_CODE;


// Messaging handler callback pointer.
//
typedef void (*PFN_MessageHandler) (IPL_MESSAGE_CODE, LPWSTR);
extern PFN_MessageHandler g_pfnMessageHandler;

// IPLcommon Function prototypes.
//
void IPLmain(void);
BOOLEAN OEMGetUpdateMode(void);
BOOLEAN OEMTranslateBaseAddress(UINT32 ulPartType, UINT32 ulAddr, UINT32 *pulTransAddr);
BOOLEAN OEMIPLInit(void);
void OEMLaunchImage(UINT32 ulLaunchAddr);

#endif  // _IPLCOMMON_H_.
