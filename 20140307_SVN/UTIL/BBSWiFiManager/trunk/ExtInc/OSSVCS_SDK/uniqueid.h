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

Copyright (c) Microsoft Corporation. All Rights Reserved.

Module Name:

    uniqueid.h

Abstract:

    Definitions for the unique identifier returned by
    IOCTL_HAL_GET_DEVICEID

Revision History:

--*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <winioctl.h>

typedef struct _DEVICE_ID {
	DWORD	dwSize;
	DWORD	dwPresetIDOffset;
	DWORD	dwPresetIDBytes;
	DWORD	dwPlatformIDOffset;
	DWORD	dwPlatformIDBytes;
} DEVICE_ID, *PDEVICE_ID;


#ifndef IOCTL_HAL_GET_DEVICEID
#define IOCTL_HAL_GET_DEVICEID CTL_CODE(FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);
#endif

#ifdef __cplusplus
}
#endif
