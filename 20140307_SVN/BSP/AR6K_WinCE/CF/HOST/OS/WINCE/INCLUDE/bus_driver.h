//------------------------------------------------------------------------------
// <copyright file="bus_driver.h" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    The software is licensed “as-is.” 
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// NDIS bus driver definitions
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef __BUS_DRIVER_H__
#define __BUS_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef PVOID BUS_DRIVER_HANDLE;

NDIS_STATUS
busDriverInit(NDIS_HANDLE MiniportHdl, NDIS_HANDLE WrapperConfiguarationContext, A_UINT32 sysIntr, BUS_DRIVER_HANDLE *busDriverHandle);

void
busDriverIsr(BUS_DRIVER_HANDLE busDriverHandle,A_BOOL *callDsr);

void
busDriverDsr(BUS_DRIVER_HANDLE busDriverHandle);

void
busDriverShutdown (BUS_DRIVER_HANDLE busDriverHandle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __BUS_DRIVER_H__
    
