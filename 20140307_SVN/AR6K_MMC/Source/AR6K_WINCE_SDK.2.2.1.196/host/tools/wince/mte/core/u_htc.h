/*
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 * 
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
 *
 * simplified HTC declarations and prototypes
 */

#ifndef _U_HTC_H_
#define _U_HTC_H_

#include <htc.h>
#include "u_hif.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ------ Endpoint IDS ------ */
typedef enum
{
    ENDPOINT_UNUSED = -1,
    ENDPOINT_0 = 0,
    ENDPOINT_1 = 1,
    ENDPOINT_2 = 2,
    ENDPOINT_3,
    ENDPOINT_4,
    ENDPOINT_5,
    ENDPOINT_6,
    ENDPOINT_7,
    ENDPOINT_8,
    ENDPOINT_MAX,
} HTC_ENDPOINT_ID;

/* this is the amount of header room required by users of HTC */
#define HTC_HEADER_LEN         HTC_HDR_LENGTH

typedef void *HTC_HANDLE;


/* ------ Function Prototypes ------ */
HTC_HANDLE  HTCConnect(HIF_DEVICE *hifDevice);

A_STATUS    HTCSendMsg(HTC_HANDLE      HTCHandle,
                       HTC_ENDPOINT_ID Endpoint,
                       A_UINT8         *pBuffer,
                       int             Length);

void        HTCShutDown(void);

A_STATUS    HTCGetMessage(HTC_HANDLE      HTCHandle,
                          HTC_ENDPOINT_ID *pEndpoint,
                          A_UINT8         *pBuffer,  /* caller must provide HTC head room */
                          int             MaxLength,
                          int             *pActualLength,
                          int             TimeoutMS);

A_STATUS    HTCConnectService(HTC_HANDLE               HTCHandle,
                              A_UINT16                 ServiceID,
                              HTC_ENDPOINT_ID          *pEndpoint);

A_STATUS    HTCStart(HTC_HANDLE HTCHandle);

#ifdef __cplusplus
}
#endif

#endif

