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
// File: NE2000ISR.H
//
// Purpose: Header file shared between ne2000.dll and ne2000isr.dll
//
//

#ifndef _NE2000ISR_H_
#define _NE2000ISR_H_

#include <pkfuncs.h>

#define USER_IOCTL(X)           (IOCTL_KLIB_USER + (X))

#define IOCTL_NE2000ISR_INFO    USER_IOCTL(0)

typedef struct _NE2000_ISR_INFO
{
    DWORD   SysIntr;            //  SYSINTR for ISR handler to return (if associated device is asserting IRQ)
    DWORD   PortAddr;           //  Port Address
    //
    // Switching to page 2 to read the interrupt mask register will abort any in-progress "DMA" activity
    // and can adversely impact other IST processing. Instead, ne2000.dll will write the current interrupt
    // mask register state here.
    //
    LPDWORD pIntMaskVirt;  // Virtual address containing the current interrupt mask register state
    LPDWORD pIntMaskPhys;  // Physical address containing the current interrupt mask register state
    
} NE2000_ISR_INFO, *PNE2000_ISR_INFO;
    
#endif // _NE2000ISR_H_
