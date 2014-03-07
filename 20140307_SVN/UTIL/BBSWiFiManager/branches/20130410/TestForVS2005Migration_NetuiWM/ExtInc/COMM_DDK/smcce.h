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


Module Name:

    smcce.h

Abstract:

    This files inlcudes the Windows CE specific data structure
    for the smart card library

Environment:

    Kernel mode only.

Notes:

Revision History:

    - Created Oct 1998  

--*/
#include <ntstatus.h>
#include <windows.h>
#include <types.h>
#include <tchar.h>
#include <ceddk.h>


#ifndef RtlMoveMemory
#define RtlMoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define RtlFillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

typedef struct _OS_DEP_DATA {

	// Pointer to the device object (Must be set by driver)
	struct _SMARTCARD_EXTENSION *pSmartCardExtension;
	CRITICAL_SECTION CritSect;  // used for serializing entry into driver
	HANDLE hChangeEvent;        // signaled on card-insertion event
	HANDLE hCancelEvent;        // signaled on CANCEL_BLOCKING ioctls

} OS_DEP_DATA, *POS_DEP_DATA;



