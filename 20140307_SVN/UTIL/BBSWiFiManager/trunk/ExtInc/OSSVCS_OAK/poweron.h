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

Copyright (c) Microsoft Corporation. All rights reserved. 

Module Name:

    poweron.h

Abstract:

    Definition of IOCTL that is used for determining why the device powered on

Revision History:

--*/

#pragma once


#include <winioctl.h>


//
// Flags for reasons we turned on
//

#ifndef POWERON_REMINDER
#define POWERON_REMINDER        0x0001
#endif
#ifndef POWERON_CPM_REBOOT
#define POWERON_CPM_REBOOT      0x0002
#endif
#ifndef POWERON_SCHEDULEDTIME
#define POWERON_SCHEDULEDTIME   0x0003
#endif

//
// Further flags for PowerOn
//

#ifndef PO_SYSINTR
#define PO_SYSINTR              0x0001
#endif
#ifndef PO_REASON
#define PO_REASON               0x0002
#endif

//
// Structure that returns a reason we powered on.  Can be either due to a system interrupt,
// or due to one of the reasons listed above.  dwFlags is one of the PO_ constants above
// and determines which field to use
//

#ifndef POWERONREASON
typedef struct _POWERONREASON {
    DWORD dwFlags;
    union {
        DWORD dwSysIntr;
        DWORD dwReason;
    }; 
} POWERONREASON;
#endif

/*++

	IOCTL_HAL_GET_POWERON

	Tells us how the device was powered on
	
	Input buffer is unused.

	Output buffer is a POWERONREASON structure

--*/


#ifndef IOCTL_HAL_GET_POWERONREASON
#define IOCTL_HAL_GET_POWERONREASON \
        CTL_CODE(FILE_DEVICE_HAL, 50, METHOD_BUFFERED, FILE_ANY_ACCESS)
#endif


