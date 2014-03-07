//------------------------------------------------------------------------------
// <copyright file="ar6000_sdio.h" company="Atheros">
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
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// SDIO HIF definitions
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef _AR6000_SDIO_H_
#define _AR6000_SDIO_H_

#include "athdefs.h"
#include "athtypes.h"
#include "osapi.h"
#include <sdcardddk.h>

#define MANUFACTURER_ID                    0x109
#define FUNCTION_CLASS                     0x0
#define MANUFACTURER_CODE                  0x271

#define BUS_REQUEST_MAX_NUM                32

#define SDIO_CLOCK_FREQUENCY_DEFAULT       24000000

#define SDIO_CLOCK_FREQUENCY_REDUCED       12000000

#define DEFAULT_SDIO_FUNCTION_RETRIES       5
#define DEFAULT_SDIO_FUNCTION_RETRY_TIMEOUT 1000

#define SDWLAN_ENABLE_DISABLE_TIMEOUT      2
#define FLAGS_CARD_ENAB                    0x02
#define FLAGS_CARD_IRQ_UNMSK               0x04

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

#endif // __AR6000_SDIO_H__
