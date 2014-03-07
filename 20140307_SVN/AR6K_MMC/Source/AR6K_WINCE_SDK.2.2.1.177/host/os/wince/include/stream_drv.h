//------------------------------------------------------------------------------
// <copyright file="stream_drv.h" company="Atheros">
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef _STREAM_DRV_H_
#define _STREAM_DRV_H_

#ifdef __cplusplus
extern "C" {
#endif

void DRG_Exit();

extern BOOL bIsNdisRegistered;

/* the following are defined in the HIF layer which form the streams driver interface. */
extern TCHAR g_AR6KDevicePowerName[];
extern TCHAR g_AR6KAdapterInstance[];
extern TCHAR g_AR6KMiniportName[];
extern TCHAR g_AR6KDeviceInitEventName[];
#define AR6K_DEVICE_POWER_NAME g_AR6KDevicePowerName
#define AR6K_ADAPTER_INSTANCE  g_AR6KAdapterInstance
#define AR6K_ADAPTER_MINIPORT  g_AR6KMiniportName
#define AR6K_DEVICE_INIT_EVENT_NAME g_AR6KDeviceInitEventName



#ifdef __cplusplus
}
#endif

#endif
