//------------------------------------------------------------------------------
// <copyright file="stream_drv.c" company="Atheros">
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
// Driver entry points for Atheros SDIO based Wifi Card driver
//
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include <winioctl.h>
#include <types.h>
#include <ndis.h>

#include "hif.h"
#include "htc_api.h"
#include "bmi.h"
#include "athdrv_wince.h"
#include "host_version.h"
//#include "htc_raw_drv.h"
#include "common_drv.h"
#include "wince_common_drv.h"
//#include "stream_drv.h"



struct {
    HTC_HANDLE      htcTarget;
    HIF_DEVICE      *hifDevice;
    A_UINT32        targetType;
    A_UINT32        targetId;
} stream_drv_t;
static BOOL     bHTCRawOpened       = FALSE;
BOOL     bIsNdisRegistered   = FALSE;
#ifdef WINCE_ART
static BOOL     bIsARTStarted       = FALSE;
#endif



DWORD
NdisRegisterAdapterThread(PVOID Context)
{
    NDIS_STATUS   ndisStatus = NDIS_STATUS_SUCCESS;

    NdisRegisterAdapter(&ndisStatus, L"AR6K_SD", L"AR6K_SD1");
    if (NDIS_STATUS_SUCCESS == ndisStatus)
    {
        bIsNdisRegistered = TRUE;
    }

    return ndisStatus;
}



void DRG_Exit()
{
    NDIS_STATUS   ndisStatus;

    if (TRUE == bIsNdisRegistered)
    {
        NdisDeregisterAdapter (&ndisStatus, L"AR6K_SD1");
        bIsNdisRegistered = FALSE;
    }

#ifdef WINCE_ART
    if (TRUE == bIsARTStarted)
    {
        ART_STOP();
    }
#endif

    if (TRUE == bHTCRawOpened)
    {
        //ar6000_htc_raw_close(stream_drv_t.htcTarget);
        bHTCRawOpened = FALSE;
    }

#ifdef HTC_EPPING_TEST
    CleanupEpTest();
#endif

    return;
}



