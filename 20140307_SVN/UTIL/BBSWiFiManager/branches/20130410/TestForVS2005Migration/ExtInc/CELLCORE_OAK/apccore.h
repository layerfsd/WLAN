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
//+----------------------------------------------------------------------------
// 
// File:        apccore.h
//
// Copyright:    (c) 1997-1999 Microsoft and/or its suppliers. All rights reserved.
//              All Rights Reserved
//              Infomation Contained Herein Is Proprietary and Confidential
//
// Contents:    Public prototypes and definitions for APC core interfaces.
//
//-----------------------------------------------------------------------------

#ifndef _APCCORE_H_
#define _APCCORE_H_

// Reboot the system if process is not responding
#define CPM_RECOVER_REBOOT                  0xFE 

#define IOCTL_HAL_OAK_START                 5000

// Determine whether or not we are in the debugger (useful for CPM)
#define IOCTL_HAL_GET_INDEBUGGER                IOCTL_HAL_OAK_START + 14
#define IOCTL_HAL_WATCHDOG_ENABLE				IOCTL_HAL_OAK_START + 15
#define IOCTL_HAL_WATCHDOG_SET					IOCTL_HAL_OAK_START + 16

#endif //_APCCORE_H_
