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
    uhcdddsi.h
    
Abstract:  
    This file provides the definitions for the USB UHCD MDD->PDD 
    interface.
Notes: 

    @doc DRIVERS           
    
    @topic   Creating an UHCI driver for Windows CE    |
    
             The Windows CE Universal Host Controller Driver (UHCD) module consists of
             two portions - a platform independant portion (MDD) which is provided with
             the OS in public\\common\\oak\\drivers\\usb\\hcd\\uhcd, and a platform 
             dependant portion (PDD), which must be written by the OEM.  
             
             The bulk of the processing occurs in the MDD, with the PDD providing 
             the necessary glue to interface the hardware to a particular platform.  
             Since the UHCI spec is very detailed as to how the hardware should behave, 
             there is very little that needs to be done in the PDD, primarily locating 
             the card at init, and providing the MDD with pointers to the hardware and 
             shared memory area.
             
             To configure the UHCI driver to be loaded by device.exe during system
             startup, the standard driver registry settings should be added to the
             platform.reg file, for example
             

             ; USB - UHCI driver <nl>
             [HKEY_LOCAL_MACHINE\\Drivers\\BuiltIn\\UHCI] <nl>
                "Prefix"="HCD"   <nl>
                "Dll"="uhci.dll" <nl>
                "Index"=dword:1  <nl>
                "Order"=dword:1  <nl>
             
             This will cause device.exe to load uhci.dll, and call the initialization
             routine.
--*/
#ifndef   __UHCDDDSI_H__
#define   __UHCDDDSI_H__
#include  <hcdddsi.h>
#define ZONE_UHCD    ZONE_HCD
#endif
