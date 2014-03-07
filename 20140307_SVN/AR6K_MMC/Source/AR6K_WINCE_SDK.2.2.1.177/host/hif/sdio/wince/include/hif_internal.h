//------------------------------------------------------------------------------
// <copyright file="hif_internal.h" company="Atheros">
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

#ifndef HIF_INTERNAL_H
#define HIF_INTERNAL_H

#include <ndis.h>

#ifndef AR_DEBUG_PRINTF
#define AR_DEBUG_PRINTF
#endif

#ifndef ATH_DEBUG_TRACE
#define ATH_DEBUG_TRACE
#endif

struct hif_device {
    SD_DEVICE_HANDLE *handle;
#ifdef CEPC_ELLEN
    NDIS_EVENT   hifIRQEvent;
#endif
    BOOL         Shutdown;
#ifdef HIF_SINGLE_BLOCK_WAR
    CRITICAL_SECTION  WriteLock;
    CRITICAL_SECTION  ReadLock;
#endif
    HANDLE            hSlotPowerDevice;
    DWORD             SlotPowerOnIoctl;
    DWORD             SlotPowerOffIoctl;
    LONG              PowerStateOff;
    BOOL              SlotPowerRemoved;
    SD_CARD_INTERFACE CardInterface;
    HTC_CALLBACKS     htcCallbacks;
    void              *claimedContext;
};

static BOOL
hifDeviceInserted(SD_DEVICE_HANDLE *device);

static void
hifDeviceRemoved(SD_DEVICE_HANDLE *device);


static void
hifRWCompletionHandler(SD_DEVICE_HANDLE  hDevice,
					   PSD_BUS_REQUEST   pRequest,
					   PVOID             notUsed,
					   DWORD             dwParam);

static SD_API_STATUS
hifIRQHandler(SD_DEVICE_HANDLE hDevice, PVOID notUsed);

static HIF_DEVICE *
addHifDevice(SD_DEVICE_HANDLE *handle);

HIF_DEVICE *
getHifDevice(SD_DEVICE_HANDLE *handle);

static void
delHifDevice(SD_DEVICE_HANDLE *handle);



/* ------ Static Variables ------ */

static SD_FUNCTION   sdFunction;

HIF_DEVICE    hifDevice[HIF_MAX_DEVICES];
OSDRV_CALLBACKS osdrvCallbacks;

A_UINT16      maxBlocks;
A_UINT16      maxBlockSize;
A_UINT8       funcNo = 0;

#ifdef HIF_SDIO_BYPASS

void            AckInterruptBypass();
SD_API_STATUS   InterruptEnableDisableBypass(A_BOOL Enable, PSD_INTERRUPT_CALLBACK pIrqFunc);
A_BOOL          ActivateSDIOStackBypassMode();
A_BOOL          DeactivateSDIOStackBypassMode();

SD_API_STATUS SynchRequestBypass(UCHAR                Command,
                                 DWORD                Argument,
                                 SD_TRANSFER_CLASS    TransferType,
                                 SD_RESPONSE_TYPE     ResponseType,
                                 PSD_COMMAND_RESPONSE pReponseBuffer,
                                 ULONG                BlockCount,
                                 ULONG                BlockLength,
                                 PUCHAR               pBuffer);

SD_API_STATUS ASyncRequestBypass(UCHAR                Command,
                                 DWORD                Argument,
                                 SD_TRANSFER_CLASS    TransferType,
                                 SD_RESPONSE_TYPE     ResponseType,
                                 ULONG                BlockCount,
                                 ULONG                BlockLength,
                                 PUCHAR               pBuffer,
                                 PSD_BUS_REQUEST_CALLBACK pCallBack,
                                 DWORD                Context);

#endif

#endif

