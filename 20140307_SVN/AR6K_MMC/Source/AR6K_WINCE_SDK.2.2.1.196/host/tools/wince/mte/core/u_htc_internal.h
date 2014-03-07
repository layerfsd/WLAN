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

#ifndef U_HTC_INTERNAL_H_
#define U_HTC_INTERNAL_H_

#include "athstartpack.h"
typedef PREPACK struct _AR6K_IRQ_PROC_REGISTERS {
    A_UINT8                      host_int_status;
    A_UINT8                      cpu_int_status;
    A_UINT8                      error_int_status;
    A_UINT8                      counter_int_status;
    A_UINT8                      mbox_frame;
    A_UINT8                      rx_lookahead_valid;
    A_UINT8                      hole[2];
    A_UINT32                     rx_lookahead[2];
} POSTPACK AR6K_IRQ_PROC_REGISTERS;

#define AR6K_IRQ_PROC_REGS_SIZE sizeof(AR6K_IRQ_PROC_REGISTERS)

#include "athendpack.h"

#define ATH_DEBUG_RECV DBG_ZONE_HTC_TRC
#define ATH_DEBUG_SEND DBG_ZONE_HTC_TRC
#define ATH_DEBUG_TRC  DBG_ZONE_HTC_TRC

typedef struct _HTC_TARGET {
    A_UINT32                HifBlockMask;
    A_UINT32                HifBlockSize;
    A_UINT32                HifMailboxAddress;
    HIF_DEVICE              *HifDevice;
    AR6K_IRQ_PROC_REGISTERS IrqProcRegisters;
    int                     TotalCredits;
    int                     CreditsAvailable;
    int                     TargetCreditSize;
} HTC_TARGET;

A_STATUS HTCIssueMsg(HTC_HANDLE      HTCHandle, 
                     HTC_ENDPOINT_ID Endpoint, 
                     A_UINT8         *pBuffer, 
                     int             Length);
                     
#define HTC_MAILBOX 0

extern HTC_TARGET  g_HTCTarget;

#endif /*U_HTC_INTERNAL_H_*/
