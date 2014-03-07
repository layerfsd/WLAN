/*
 * Bluetooth filter , MS Bt Stack definitions
 * 
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
 */

#ifndef BTFILTER_MSBT_H_
#define BTFILTER_MSBT_H_

#include <msgqueue.h>
#include <bt_api.h>

#ifdef BTE_CLASS_DEVICE
    /* this version of the BT stack supports notifications (WM5 and greater) */
#define REGISTER_MS_BTSTACK_NOTIFICATIONS

#endif


#ifdef REGISTER_MS_BTSTACK_NOTIFICATIONS

static void ProcessMSBTStackNotification(void *pContext);

#define MAX_BT_NOTIFY_MSGS              8

#ifdef BTE_CLASS_AVDTP
/* if the system we are building for supports notifications for AVDTP (aka A2DP)then we
 * register for these event notifications */ 

#define BT_EVENT_CLASSES_TO_REGISTER    (BTE_CLASS_AVDTP | BTE_CLASS_CONNECTIONS)

#else

    /* just connection events so we can get role info */
#define BT_EVENT_CLASSES_TO_REGISTER    BTE_CLASS_CONNECTIONS

#endif  // BTE_CLASS_AVDTP

#endif // REGISTER_MS_BTSTACK_NOTIFICATIONS

    /* local BT Stack info */
typedef struct _MS_BT_STACK_NOTIFY_INFO {
    ATH_BT_STACK_NOTIFY_INFO    *pNotifyInfo;
    BOOL                         ShutDown; 
    
#ifdef REGISTER_MS_BTSTACK_NOTIFICATIONS    
    HANDLE  hBtNotifyMsgQueue;      /* messsage queue to receive notifications */
    HANDLE  hBtNotification;        /* bt notification handle */
#endif

#ifndef BTE_CLASS_AVDTP
        /* this version of the stack doesn't support AVDTP indications so we
         * do things differently */
         
    HANDLE      hBtA2DPEvent;           /* event to wake A2DP processing thread */
    HANDLE      hBtA2DPProcThread;      /* thread that handles final A2DP processing */ 
    A_UINT16    A2DPAclConnHandle;
#endif
    CRITICAL_SECTION  Lock;                           /* synchronization lock */   
    VOID              *pHCIExtension;  
} MS_BT_STACK_NOTIFY_INFO;

#define LOCK_INST(p)   EnterCriticalSection(&(p)->Lock)
#define UNLOCK_INST(p) LeaveCriticalSection(&(p)->Lock)

#ifdef __cplusplus
extern "C" {
#endif 
void AthBtFilterDisconnectHCI(MS_BT_STACK_NOTIFY_INFO *pInfo);
BOOL AthBtFilterConnectHCI(MS_BT_STACK_NOTIFY_INFO *pInfo);
#ifdef __cplusplus
}
#endif
 
#ifndef BTE_CLASS_AVDTP
    /* this version of the stack doesn't support BthGetRole ! */
#define BTH_GET_ROLE_USE_EXT
#ifdef __cplusplus
extern "C" {
#endif 
LONG GetHCIRole(MS_BT_STACK_NOTIFY_INFO *pInfo, USHORT ConnectionHandle, USHORT *pRole);
#ifdef __cplusplus
}
#endif
#endif

#endif /*BTFILTER_MSBT_H_*/
