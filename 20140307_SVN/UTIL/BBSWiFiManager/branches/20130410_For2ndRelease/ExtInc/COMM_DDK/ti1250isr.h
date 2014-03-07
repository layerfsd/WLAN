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
// File: TI1250ISR.h
//
// Purpose: TI1250 Installable Interrupt Service Routine definitions.
//
//

#ifndef _TI1250ISR_H_
#define _TI1250ISR_H_

#include <pkfuncs.h>

#define USER_IOCTL(X) (IOCTL_KLIB_USER + (X))

#define IOCTL_TI1250ISR_INFO USER_IOCTL(0)

#define MAX_TI1250ISR_INSTANCES 1  // A maximum of 1 TI1250 ISR handlers per dll are allowed
#define MAX_SOCKETS 4

typedef struct _TI1250ISR_INFO {
    DWORD SysIntr;              // SYSINTR for ISR handler to return (if associated device is asserting IRQ)
    BOOL bIntr;                 // If true, this is a functional interrupt
    DWORD Sockets;              // Number of sockets
    DWORD PortAddr[MAX_SOCKETS];// Array of port Addresses
} TI1250ISR_INFO, *PTI1250ISR_INFO;
    
#endif // _TI1250ISR_H_
