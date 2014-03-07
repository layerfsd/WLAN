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

Copyright (c) 1999 - 2000 Microsoft Corporation

Module Name:

    deviceid.h

Abstract:

    Definitions for the unique identifier returned by
    IOCTL_HAL_GET_DEVICEID

Revision History:

--*/

#pragma once

#include <windows.h>
#include <pkfuncs.h>

// **************************************************************************
//
// IOCTL_HAL_GET_DEVICEID
//
// Purpose: obtains the 128 bit immutable identifier that is unique to
//          the device
//
// Arguments:
//    IN    lpInBuf              DWORD *
//                               can be used to enforce strict uniqueness
//                               if NULL or 0, the strict uniqueness is not
//                               required.
//    OUT   lpOutBuf             PUNIQUE_DEVICEID
//                               16 byte buffer containing the ID
//
// Return Values:
//  ERROR_SUCCESS
//  ERROR_INVALID_PARAMETER     lpOutBuf is NULL
//  ERROR_BUFFER_OVERFLOW       caller didn't supply enough room
//  ERROR_NOT_SUPPORTED         caller requested strict uniqueness
//                              and it is not supported
//                              OR the device doesn't have a unique ID
//

//
// flags for requesting the ID
// The only use for these now is if a caller
// will only accept the IETF standard UUID generation,
// meaning they would reject any other way of generating the ID
//

#define DEVICEID_STRICT     0x00000001UL
#define DEVICEID_SECURE     0x00000002UL


typedef struct tagUniqueDeviceId {

    BYTE    data[16];

} UNIQUE_DEVICEID, *PUNIQUE_DEVICEID;


////////////////////////////////////////////////////////////////////////////
//
// below are definitions used in the generation of the ID, requesting apps
// only need the definitions above
//

//
// values for the version and variant used
// in method 1 generation
//

#define DEVICEID_METHOD1_VERSION    0x1
#define DEVICEID_METHOD1_VARIANT    0x8

#define IOCTL_HAL_GET_HWENTROPY   CTL_CODE(FILE_DEVICE_HAL, 62, METHOD_BUFFERED, FILE_ANY_ACCESS)

