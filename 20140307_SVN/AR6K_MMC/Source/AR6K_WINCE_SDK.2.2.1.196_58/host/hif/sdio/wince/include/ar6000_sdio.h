//------------------------------------------------------------------------------
// <copyright file="ar6000_sdio.h" company="Atheros">
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
#ifndef _AR6000_SDIO_H_
#define _AR6000_SDIO_H_

#include "athdefs.h"
#include "a_types.h"
#include "a_osapi.h"
#include <sdcardddk.h>

#define MANUFACTURER_ID                    0x109
#define FUNCTION_CLASS                     0x0
#define MANUFACTURER_CODE                  0x271

#define BUS_REQUEST_MAX_NUM                32

#define SDIO_CLOCK_FREQUENCY_DEFAULT       25000000

#define SDIO_CLOCK_FREQUENCY_REDUCED       12000000

#define DEFAULT_SDIO_FUNCTION_RETRIES       50
#define DEFAULT_SDIO_FUNCTION_RETRY_TIMEOUT 100

#define SDWLAN_ENABLE_DISABLE_TIMEOUT      2
#define FLAGS_CARD_ENAB                    0x02
#define FLAGS_CARD_IRQ_UNMSK               0x04

#if (defined(HIF_SINGLE_BLOCK_WAR) && defined(HIF_SDIO_LARGE_BLOCK_MODE))

    /* reduce block size to minimize padding */
#define HIF_MBOX_BLOCK_SIZE                 64
#define WLAN_MAX_SEND_FRAME                 1536
#define WLAN_MAX_RECV_FRAME                 (1536 + HIF_MBOX_BLOCK_SIZE) /* rx + lookaheads and credit rpts */

    /* the block size for large block mode is the set to the largest frame size */
#define HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE  WLAN_MAX_RECV_FRAME

#else

#define HIF_MBOX_BLOCK_SIZE                128

#endif

#define HIF_MBOX_BASE_ADDR                 0x800
#define HIF_MBOX_WIDTH                     0x800
#define HIF_MBOX0_BLOCK_SIZE               1 //builder:mbox0 block size
#define HIF_MBOX1_BLOCK_SIZE               HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX2_BLOCK_SIZE               HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX3_BLOCK_SIZE               HIF_MBOX_BLOCK_SIZE

#define HIF_MBOX_START_ADDR(mbox)                        \
    HIF_MBOX_BASE_ADDR + mbox * HIF_MBOX_WIDTH

#define HIF_MBOX_END_ADDR(mbox)                          \
    HIF_MBOX_START_ADDR(mbox) + HIF_MBOX_WIDTH - 1


#define HIF_SDIO_PARAMS_REG_PATH TEXT("\\Comm\\AR6K_SD1\\Parms\\HIF\\SDIO")

/*
 * Structure used by the upper layer to register itself with the
 * device driver.
 */
typedef struct _SD_FUNCTION_ {
    /* name of registering driver */
	A_UCHAR       *pName;

	/* callback functions provided by the Upper layer module to the driver.  */
	A_BOOL (*pProbe)(SD_DEVICE_HANDLE handle);
	VOID (*pRemove)(SD_DEVICE_HANDLE handle);
	VOID (*pIsr)(SD_DEVICE_HANDLE handle,A_BOOL *callDsr);
	VOID (*pDsr)(SD_DEVICE_HANDLE handle);
} SD_FUNCTION;

extern A_UINT32 sdio1bitmode;
extern A_UINT32 sdiobusspeedlow;

/*
 * API function prototypes
 */
extern SD_API_STATUS
SDIORegisterFunction(SD_FUNCTION *pFunction);
extern SD_API_STATUS
SDIOUnregisterFunction(SD_FUNCTION *pFunction);
extern SD_DEVICE_HANDLE SDIOGetHandle ();

void SetFinalBusMode(SD_DEVICE_HANDLE hDevice);

BOOL GetRegistryKeyValue(HKEY   hKey,
                         WCHAR  *pKeyPath,
                         WCHAR  *pValueName,
                         PUCHAR pValue,
                         ULONG  BufferSize);
                         
#endif // __AR6000_SDIO_H__
