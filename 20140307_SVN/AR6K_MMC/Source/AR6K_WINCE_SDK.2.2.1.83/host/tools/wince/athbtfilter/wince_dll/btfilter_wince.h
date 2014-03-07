/*
 * Bluetooth filter DLL internal definitions
 * 
 * Copyright (c) 2007 Atheros Communications Inc.
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
 

#ifndef ATHBTFILTER_OS_H_
#define ATHBTFILTER_OS_H_

#include "dl_list.h"
#include "btdefs.h"
#include "athbtfilter.h"
#include "btfilter_core.h"

#ifdef ATHFILT_STATIC_LIB
/* as a static lib we use internal logging */
#ifdef DEBUG

#ifdef __cplusplus
extern "C" {
#endif
void AthBtFilterLogMessage(LPCWSTR pFormatString,...);
#ifdef __cplusplus
}
#endif
   
#define ATH_LOG_MSG(test,args) if (test) AthBtFilterLogMessage args

#else
#define ATH_LOG_MSG
//#define ATH_LOG_MSG RETAILMSG
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern DWORD g_AthDebugMask;
#ifdef __cplusplus
}
#endif

#define LOG_ZONE_TEST(bit) (g_AthDebugMask & (1 << (bit)))

#else

/* as a DLL we can use CE's standard debug logging */

#ifdef DEBUG

#ifdef __cplusplus
extern "C" {
#endif
void AthBtFilterLogMessage(LPCWSTR pFormatString,...);
#ifdef __cplusplus
}
#endif

#define ATH_LOG_MSG(test,args) if (test) AthBtFilterLogMessage args
#else
#define ATH_LOG_MSG DEBUGMSG
//#define ATH_LOG_MSG RETAILMSG
extern DBGPARAM dpCurSettings;
#endif

#define LOG_ZONE_TEST(bit) DEBUGZONE((bit))

#endif // ATHFILT_STATIC_LIB


#define ZONE_ERROR       LOG_ZONE_TEST(0)
#define ZONE_INIT        LOG_ZONE_TEST(1)
#define ZONE_HCI_CMD     LOG_ZONE_TEST(2)
#define ZONE_HCI_EVT     LOG_ZONE_TEST(3)
#define ZONE_ADAPTER     LOG_ZONE_TEST(4)
#define ZONE_ACTION      LOG_ZONE_TEST(5)
#define ZONE_ACTION_DUMP LOG_ZONE_TEST(6)
#define ZONE_BT_NOTIFY   LOG_ZONE_TEST(7)
#define ZONE_BT_NOTIFY_DUMP   LOG_ZONE_TEST(8)

#define LOG_VERBOSITY  ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))
//#define LOG_VERBOSITY  ((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8))

#define ATH_DEF_DEBUGMASK (0x003 | LOG_VERBOSITY)

#define ATHBT_ASSERT(test) \
    if (!(test)) { ATH_LOG_MSG(ZONE_ERROR, (TEXT("ATHBT ASSERT! file:%s  line:%d  \r\n"), TEXT(__FILE__), __LINE__));}

#define MAX_WLAN_ADAPTER_NAME           31
#define MAX_WLAN_REG_PATH               63
#define MAX_NDIS_NOTIFICATION_MESSAGES  10
#define MAX_BT_ACTION_MESSAGES          16

    /* BT stack notification callback */
typedef void   (*ATHBT_STACK_EVENT_FN)(struct _ATH_BT_STACK_NOTIFY_INFO *pNotifyInfo);

typedef struct _ATH_BT_STACK_NOTIFY_INFO {
        /* these structures are set by the BT-stack adaptation layer */
    HANDLE                  hBtNotify;                /* object to wait on */
    ATHBT_STACK_EVENT_FN    Callback;                 /* callback to invoke when object is signalled */
    void                    *pContext;                /* context to pass callback */
    
        /* these structures are set by the filter dispatcher */
    ATHBT_INDICATE_STATE_FN pStateIndicationFn;       /* state indication function */
    void                    *pStateIndicationContext; /* context to use to make BT state indications */
    
        /* the following misc information is set by the BT-stack adaptation prior to making
         * an indication */ 
    UCHAR                   SCOConnection_LMPVersion;   /* lmp version of remote SCO device */
    UCHAR                   A2DPConnection_LMPVersion;  /* lmp version of remote A2DP device */ 
    UCHAR                   SCOConnection_Role;         /* role of remote SCO device */
    UCHAR                   A2DPConnection_Role;        /* role of remote A2DP device */
} ATH_BT_STACK_NOTIFY_INFO; 

typedef struct _ATHBT_SCO_CONNECTION_INFO {
    BOOL        Valid;
    UCHAR       LinkType;
    UCHAR       TransmissionInterval;
    UCHAR       RetransmissionInterval;
    USHORT      RxPacketLength;
    USHORT      TxPacketLength;
} ATHBT_SCO_CONNECTION_INFO;
                                                                     
typedef struct _ATHBT_FILTER_INFO {
    HANDLE                  hFilterThread;
    HANDLE                  hWakeEvent;
    HANDLE                  hMsgQueue;
    HANDLE                  hNdisUIO;
    BOOL                    MsgQueueRegistered;
    BOOL                    Shutdown;
    WCHAR                   AdapterName[MAX_WLAN_ADAPTER_NAME + 1];
    WCHAR                   ChannelInfoRegPath[MAX_WLAN_REG_PATH + 1];
    ATH_BT_FILTER_INSTANCE  *pInstance;
    LONG                    AdapterAvailCount;
    DL_LIST                 BTActionMsgList;
    DL_LIST                 FreeActionMsgList;
    CRITICAL_SECTION        CritSection;
    int                     MaxNdisNotificationMessages;
    int                     MaxBtActionMessages;
    BT_FILTER_CORE_INFO     FilterCore;
    BOOL                    AdapterAvailable;
    ATH_BT_STACK_NOTIFY_INFO  BtStackNotifyInfo;
    ATHBT_SCO_CONNECTION_INFO SCOConnectInfo;
    DWORD                     BtCoexFlags;
} ATHBT_FILTER_INFO;

#define BTCOEX_FLAGS_NO_EDR_SLAVE_ROLE  (1 << 0)

#define LOCK_FILTER(p)   EnterCriticalSection(&(p)->CritSection);
#define UNLOCK_FILTER(p) LeaveCriticalSection(&(p)->CritSection);


    
/* ndis action message :
 * NDIS actions to set BT parameters are performed in a separate thread through an action queue,
 * this avoids excessive thread stack depth when an NDIS OID must be dispatched */
typedef struct _BT_ACTION_MSG {
    DL_LIST                     ListEntry;
    HANDLE                      hWaitEvent;             /* wait object for blocking requests */
    BOOL                        Blocking;               /* this action requires the calling thread to block
                                                           until the dispatcher submits the command */
    ATHBT_STATE_INDICATION      IndicationForControlAction;  /* indication associated with the control action */
    ATHBT_STATE                 StateForControlAction;
    BT_CONTROL_ACTION           ControlAction;
} BT_ACTION_MSG; 

#define QUEUE_BT_ACTION_MSG(p,a) DL_ListInsertTail(&(p)->BTActionMsgList,&(a)->ListEntry)
#define FREE_BT_ACTION_MSG(p,a)  DL_ListInsertTail(&(p)->FreeActionMsgList,&(a)->ListEntry)

/* internal APIs */
void ProcessNdisNotifications(ATHBT_FILTER_INFO *pInfo);
BOOL SetUpNdisIF(ATHBT_FILTER_INFO *pInfo);
void DisconnectNdisUIO(ATHBT_FILTER_INFO *pInfo);
void AthFilterDumpBuffer(A_UCHAR *buffer, int length, TCHAR *pDescription);                             
void ExecuteBtAction(ATHBT_FILTER_INFO *pInfo, BT_ACTION_MSG *pBtActionMsg);

typedef enum _ATH_ADAPTER_EVENT {
   ATH_ADAPTER_ARRIVED = 0,
   ATH_ADAPTER_REMOVED = 1,
} ATH_ADAPTER_EVENT;

void HandleAdapterEvent(ATHBT_FILTER_INFO *pInfo, ATH_ADAPTER_EVENT Event);
   
/* BT Stack integration APIs: */
void  AthBtStackNotificationsShutdown(ATH_BT_STACK_NOTIFY_INFO  *pCallBackInfo);
BOOL  AthBtStackNotificationsInit(ATH_BT_STACK_NOTIFY_INFO  *pCallbackInfo);   

#define MAKE_BTSTATE_MASK(state) (1 << (state))
DWORD AthBtStackGetSupportedIndicationsMask(ATH_BT_STACK_NOTIFY_INFO  *pCallbackInfo);

    /* notify stack layer that the filter core made an A2DP state indication, the stack layer can
     * handle or override the indication */
ATHBT_STATE_INDICATION  AthBtStackNotifyA2DPIndicationFromFilterCore(ATH_BT_STACK_NOTIFY_INFO  *pCallbackInfo,
                                                                     ATHBT_STATE               State,                                                                    
                                                                     A_UINT16                  BtConnHandle);
                                                                  
                                                                  

#endif /*ATHBTFILTER_OS_H_*/
