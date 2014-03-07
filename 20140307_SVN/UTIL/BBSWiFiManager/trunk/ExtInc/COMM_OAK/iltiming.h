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
//------------------------------------------------------------------------------
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//  
//------------------------------------------------------------------------------
//
//  Module Name:  
//  
//      iltiming.h
//  
//  Abstract:  
//
//      Provides interface for calling the KernelIoControl() to perform
//      interrupt latency analysis.
//      
//------------------------------------------------------------------------------
#ifndef _ILTIMING_H
#define _ILTIMING_H


#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
//
//  Application may call KernelIoControl(IOCTL_HAL_ILTIMING) with the following
//  structure passed in. The OAL will fill in the structure depending on the
//  value of wMsg. 
//
//  For ILTIMING_MSG_ENABLE and _DISABLE, no fields are updated, but the OAL
//  will hook or unhook the system tick from causing a SYSINTR_TIMING.
//
//  For ILTIMING_MSG_GET_TIMES, the OAL will fill in all the structure fields
//  with the exception of pfnPerfCountSinceTick.
//
//  For ILTIMING_MSG_GET_PFN, the OAL returns (in the PFN field) a pointer
//  to the PerfCountSinceTick function that will be called (in KMode) directly
//  from the application to minimize overhead.
//
//------------------------------------------------------------------------------
typedef struct tILTIMING_MESSAGE {
    WORD    wMsg;                       // Command to process
    WORD    wNumInterrupts;             // Number of interrupts since last call
    DWORD   dwIsrTime1;                 // Start of ISR
    DWORD   dwIsrTime2;                 // End of ISR
    DWORD   dwSPC;                      // Saved Program Counter at time of INT.
    DWORD   dwFrequency;                // PerfCount frequency
    PVOID   pfnPerfCountSinceTick;      // Pointer to OAL function
} ILTIMING_MESSAGE, *PILTIMING_MESSAGE;



//------------------------------------------
// Defines for the wMsg field.
//
#define ILTIMING_MSG_ENABLE     1
#define ILTIMING_MSG_DISABLE    2
#define ILTIMING_MSG_GET_TIMES  3
#define ILTIMING_MSG_GET_PFN    4


#ifdef __cplusplus
}
#endif

#endif

