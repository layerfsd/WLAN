/*
 * Bluetooth filter DLL implementation
 * 
 * Copyright (c) 2007 Atheros Communications Inc.
 * All rights reserved.
 *
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

#include "a_types.h"
#include "a_osapi.h"
#include "btfilter_wince.h"
#include <msgqueue.h>
#include <ndis.h>
#include <nuiouser.h>
#include "bt_api.h"
#include "drvmsgqlogging.h"

typedef enum _BTACTION_QUEUE_PROC {
    BTACTION_QUEUE_NORMAL       = 0,   /* normal processing of the action queue */
    BTACTION_QUEUE_FLUSH_ALL    = 1,   /* flush all actions for shutdown */ 
    BTACTION_QUEUE_FLUSH_STATE,        /* flush all actions associated with a specific state */     
    BTACTION_QUEUE_SYNC_STATE          /* issue queued actions when we are syncing radio state on adapter available */         
} BTACTION_QUEUE_PROC;

#define ACTION_WAIT_TIMEOUT   100

static void SyncBTState(ATHBT_FILTER_INFO *pInfo);

BOOL    g_LogToMessageQueue = FALSE;
BOOL    g_LogSetupComplete = FALSE;
HANDLE  g_hLogMsgQueue = NULL;

#ifdef ATHFILT_STATIC_LIB

/* as a static library, we have to use our internal zone logging */
DWORD g_AthDebugMask = ATH_DEF_DEBUGMASK;

#else

/* as a DLL we can use the standard debug logging mechanisms */
DBGPARAM dpCurSettings = {
    TEXT("athbtfilt"), {
    TEXT("Error"),TEXT("Init"),TEXT("HCI CMD"),TEXT("HCI EVT"),
    TEXT("Adapter"),TEXT(""),TEXT(""),TEXT(""),
    TEXT(""),TEXT(""),TEXT(""),TEXT(""),
    TEXT(""),TEXT(""),TEXT(""),TEXT("") },
    ATH_DEF_DEBUGMASK
};


/* DLL entry point if we are a stand-alone DLL */
BOOL WINAPI DllEntry( HANDLE hInstDll, DWORD fdwReason, LPVOID lpvReserved)
{
    BOOL  fRet      = TRUE;

    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
            DEBUGREGISTER((HINSTANCE)hInstDll);
            DisableThreadLibraryCalls((HMODULE)hInstDll);
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return fRet;
}

#endif // ATHFILT_STATIC_LIB


static DWORD FilterThread(LPVOID pContext);
static void AthFilterCmdEventsCallback(void *pContext, ATHBT_HCI_CTRL_TYPE Type, unsigned char *pBuffer, int Length);
static void AthFilterIndicateStateCallback(void *pContext, ATHBT_STATE_INDICATION Indication, ATHBT_STATE State, unsigned char LMPVersion);
static void ProcessBTActionMessages(ATHBT_FILTER_INFO      *pInfo, 
                                    BTACTION_QUEUE_PROC    Process,
                                    ATHBT_STATE_INDICATION StateToFlush);
static void GetActionStringOverrides(ATHBT_FILTER_INFO *pInfo);
static void AthFilterAclDataOutCallback(void *pContext, unsigned char *pBuffer, int Length);
static void AthFilterAclDataInCallback(void *pContext, unsigned char *pBuffer, int Length);
static void CleanupLogging();
static void GetFilterRegistrySettings(ATHBT_FILTER_INFO *pInfo);    
                             
/* exported filter destroy function */
void AthBtFilter_Destroy(ATH_BT_FILTER_INSTANCE *pInstance)
{
    ATHBT_FILTER_INFO  *pInfo = (ATHBT_FILTER_INFO  *)pInstance->pContext;
    BT_ACTION_MSG      *pBTActionMsg;
    int                i;
    
        /* make sure filter is disabled */
    pInstance->FilterEnabled = FALSE;
    
    if (NULL == pInfo) {
        return;    
    }
        
    pInfo->Shutdown = TRUE;
        
    if (pInfo->hWakeEvent != NULL) {
        SetEvent(pInfo->hWakeEvent);
    }
    
    if (pInfo->hFilterThread != NULL) {
            /* wait for thread to exit */
        WaitForSingleObject(pInfo->hFilterThread, INFINITE);    
        CloseHandle(pInfo->hFilterThread);            
        pInfo->hFilterThread = NULL;
    }

        /* cleanup any stack notification resources */
    AthBtStackNotificationsShutdown(&pInfo->BtStackNotifyInfo);

    DisconnectNdisUIO(pInfo);
    
    if (pInfo->hMsgQueue != NULL) {
        CloseMsgQueue(pInfo->hMsgQueue);
        pInfo->hMsgQueue = NULL;
    }
    
    if (pInfo->hWakeEvent) {
        CloseHandle(pInfo->hWakeEvent);
        pInfo->hWakeEvent = NULL;    
    }
    
    pBTActionMsg = (BT_ACTION_MSG *)((PUCHAR)pInfo + sizeof(ATHBT_FILTER_INFO));  
    
        /* close all action message wait objects */
    for (i = 0; i < pInfo->MaxBtActionMessages; i++, pBTActionMsg++) {
        if (pBTActionMsg->hWaitEvent != NULL) {
            CloseHandle(pBTActionMsg->hWaitEvent);
            pBTActionMsg->hWaitEvent = NULL;
        }       
    }        
        
    pInstance->pContext = NULL;
    DeleteCriticalSection(&pInfo->CritSection);
    FCore_Cleanup(&pInfo->FilterCore);
    LocalFree(pInfo);
    CleanupLogging();
}

/* exported filter creation function */
int AthBtFilter_Create(ATH_BT_FILTER_INSTANCE *pInstance)
{
    int                             retVal = -1;
    ATHBT_FILTER_INFO               *pInfo = NULL;
    MSGQUEUEOPTIONS                 msgOptions;
    DWORD                           threadId;
    int                             i;
    BT_ACTION_MSG                   *pBTActionMsg;
    DWORD                           maxBTActionMsgs = MAX_BT_ACTION_MESSAGES;
    A_STATUS                        status;
    
    do {
        
        pInfo = (ATHBT_FILTER_INFO *)LocalAlloc(LPTR, sizeof(ATHBT_FILTER_INFO) +
                                                      maxBTActionMsgs * (sizeof(BT_ACTION_MSG)));
        
        if (NULL == pInfo) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to allocate BT filter info \r\n")));
            break;    
        }       
        
        pInstance->pContext = pInfo;
        pInfo->hNdisUIO = INVALID_HANDLE_VALUE;
        pInfo->pInstance = pInstance;
        pInfo->MaxBtActionMessages = (int)maxBTActionMsgs;
        
        if (pInstance->pWlanAdapterName != NULL) {
                /* use caller provided name */
            wcsncpy(pInfo->AdapterName, pInstance->pWlanAdapterName, MAX_WLAN_ADAPTER_NAME);
        } else {
                /* use default instance */
            wcsncpy(pInfo->AdapterName,  TEXT("AR6K_SD1"), MAX_WLAN_ADAPTER_NAME);    
        }
        
        _snwprintf(pInfo->ChannelInfoRegPath, MAX_WLAN_REG_PATH, TEXT("Comm\\%s"), pInfo->AdapterName);
        
        InitializeCriticalSection(&pInfo->CritSection);   
        pInfo->MaxNdisNotificationMessages = MAX_NDIS_NOTIFICATION_MESSAGES;
        DL_LIST_INIT(&pInfo->BTActionMsgList);
        DL_LIST_INIT(&pInfo->FreeActionMsgList);
        
        if (!FCore_Init(&pInfo->FilterCore)) {
            break;    
        }
        
        //DebugBreak();
        
        GetFilterRegistrySettings(pInfo);
                
        GetActionStringOverrides(pInfo);
        
        status = FCore_RefreshActionList(&pInfo->FilterCore);
        
        if (A_FAILED(status)) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed refresh action list (status:%d) \r\n"), status));
            break;    
        }
        
            /* message buffers are at the end of our context blob */
        pBTActionMsg = (BT_ACTION_MSG *)((PUCHAR)pInfo + sizeof(ATHBT_FILTER_INFO));  
         
        for (i = 0; i < pInfo->MaxBtActionMessages; i++, pBTActionMsg++) {
                /* create the event for blocking requests */
            pBTActionMsg->hWaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
            
            if (NULL == pBTActionMsg->hWaitEvent) {
                ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to allocate BT action event wait object \r\n")));
                /* if we are running out of memory we'll fail farther down */
                break;    
            }
                /* free to list */
            FREE_BT_ACTION_MSG(pInfo,pBTActionMsg);        
        }        
        
            /* create the wake event for our dispatcher thread */        
        pInfo->hWakeEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        
        if (NULL == pInfo->hWakeEvent) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to allocate wakeup event \r\n")));
            break;    
        }
        
        memset(&msgOptions, 0, sizeof(msgOptions));
        msgOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
        msgOptions.dwMaxMessages = pInfo->MaxNdisNotificationMessages;
        msgOptions.cbMaxMessage = sizeof(NDISUIO_DEVICE_NOTIFICATION);
        msgOptions.bReadAccess = TRUE;
        
            /* create message queue for NDIS notification messages */
        pInfo->hMsgQueue = CreateMsgQueue(NULL, &msgOptions);
        
        if (NULL == pInfo->hMsgQueue) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to create message NDIS notification msg queue \r\n"))); 
            break;    
        }
        
            /* the BT stack notification uses the same "precise" indication API through the filter */
        pInfo->BtStackNotifyInfo.pStateIndicationFn = AthFilterIndicateStateCallback;
        pInfo->BtStackNotifyInfo.pStateIndicationContext = pInfo;
        
        pInfo->BtStackNotifyInfo.hBtNotify = NULL;
        
            /* enable BT stack notifications if supported */
        if (!AthBtStackNotificationsInit(&pInfo->BtStackNotifyInfo)) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to intialize BT stack notification layer \r\n"))); 
            break;               
        }
        
            /* get the event types that the filter core can ignore. The BT notification side can
             * handle them */
        pInfo->FilterCore.StateFilterIgnore = 
                        AthBtStackGetSupportedIndicationsMask(&pInfo->BtStackNotifyInfo);
                
        pInfo->hFilterThread = CreateThread(NULL,
                                            0,
                                            FilterThread,
                                            pInfo,
                                            0,
                                            &threadId);
        
        if (NULL == pInfo->hFilterThread) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to create filter thread (%d) \r\n"), GetLastError())); 
            break;   
        }
        
        pInstance->pFilterCmdEvents = AthFilterCmdEventsCallback;
        pInstance->pIndicateState = AthFilterIndicateStateCallback;    
        pInstance->pFilterAclDataOut = AthFilterAclDataOutCallback;
        pInstance->pFilterAclDataIn = AthFilterAclDataInCallback;
                      
            /* We are fully initialized and ready to filter.
             * The filter core needs to stay in sync with the BT radio state until the
             * WLAN driver comes up, when the WLAN driver comes on-line the filter will issue operating
             * parameters for the current BT radio state (see HandleAdapterEvent) */
        pInstance->FilterEnabled = TRUE;
                
        retVal = 0;
        
    } while (FALSE);
    
    if (retVal < 0) {
        AthBtFilter_Destroy(pInstance);    
    }
    
    return retVal;
}

#define BT_ENABLE_SCO_OPT    (BT_SCO_ALLOW_CLOSE_RANGE_OPT | BT_SCO_FORCE_AWAKE_OPT)

static void AdjustBtControlAction(ATHBT_FILTER_INFO      *pInfo, 
                                  BT_ACTION_MSG          *pActionMsg)
{
    WMI_SET_BT_PARAMS_CMD   *pParamsCmd;
    
    do {    
           /* adjust control action for BT_PARAMS_A2DP control action */
        if (pActionMsg->IndicationForControlAction == ATH_BT_A2DP) {
            
            A_UINT32 a2dpOptFlags = 0;
            
            if (pActionMsg->StateForControlAction != STATE_ON) {
                /* nothing to adjust in OFF case */
                break;
            }

            if (pActionMsg->ControlAction.Type != BT_CONTROL_ACTION_PARAMS) {
                /* only modify action that issues a PARAMS control action */
                break;
            }

            pParamsCmd = (WMI_SET_BT_PARAMS_CMD *)(pActionMsg->ControlAction.Buffer);

            if (pParamsCmd->paramType != BT_PARAM_A2DP) {
                /* only modify A2DP params */
                break;
            }

                /*Role =0 is Master, Role =1, is slave */
            if (pInfo->BtStackNotifyInfo.A2DPConnection_Role == 0) {
                pParamsCmd->info.a2dpParams.isCoLocatedBtRoleMaster = 1;
            }else {
                pParamsCmd->info.a2dpParams.isCoLocatedBtRoleMaster = 0;
                    /* check slave-role WAR */
                if (pInfo->BtCoexFlags & BTCOEX_FLAGS_NO_EDR_SLAVE_ROLE) {
                        /* workaround for local BT radio that disables EDR
                         * rates when operating as a slave. We downgrade
                         * the remote lmp version to protect A2DP as if the radio was 1.2 */        
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: No EDR for SLAVE Role WAR..\r\n")));               
                    pInfo->BtStackNotifyInfo.A2DPConnection_LMPVersion = 2;    
                } 
            }
                       
            switch (pInfo->BtStackNotifyInfo.A2DPConnection_LMPVersion) {
                case 0: // 1.0
                case 1: // 1.1
                case 2: // 1.2
                    pParamsCmd->info.a2dpParams.a2dpWlanUsageLimit = 30;
                    pParamsCmd->info.a2dpParams.a2dpBurstCntMin = 3;
                    pParamsCmd->info.a2dpParams.a2dpDataRespTimeout =10;
                    break;
                case 3: // 2.0
                case 4: // 2.1
                default:
                    if( pParamsCmd->info.a2dpParams.isCoLocatedBtRoleMaster){
                        /* allow close range optimization for newer BT radios */
                        a2dpOptFlags |= BT_A2DP_ALLOW_CLOSE_RANGE_OPT;
                        a2dpOptFlags |= BT_A2DP_FORCE_AWAKE_OPT;
                    }
                    pParamsCmd->info.a2dpParams.a2dpWlanUsageLimit = 100;
                    pParamsCmd->info.a2dpParams.a2dpBurstCntMin = 1;
                    pParamsCmd->info.a2dpParams.a2dpDataRespTimeout =10;
/*                  pParamsCmd->info.a2dpParams.p2lrpOptModeBound=7;
                    pParamsCmd->info.a2dpParams.p2lrpNonOptModeBound=9;
                    BT_A2DP_SET_DEFAULT_OVERRIDE(a2dpOptFlags);*/
                    /*uncomment the above if need to modify the 
                    Atheros recommended default values. It is expected
                    that if you are modifying this, you know what you
                    know the details*/
                    break;
            }
            pParamsCmd->info.a2dpParams.a2dpOptFlags = a2dpOptFlags;
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: BT PARAMS A2DP Adjustments :\r\n")));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    a2dpWlanUsageLimit  : %d\r\n"),
                    pParamsCmd->info.a2dpParams.a2dpWlanUsageLimit));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    a2dpBurstCntMin     : %d\r\n"),
                    pParamsCmd->info.a2dpParams.a2dpBurstCntMin));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    a2dpDataRespTimeout : %d\r\n"),
                    pParamsCmd->info.a2dpParams.a2dpDataRespTimeout));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    Co-located role     : %s\r\n"),
                    pParamsCmd->info.a2dpParams.isCoLocatedBtRoleMaster ? 
                        TEXT("MASTER") : TEXT("SLAVE")));        
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    a2dpOptFlags        : 0x%X\r\n"),
                    pParamsCmd->info.a2dpParams.a2dpOptFlags));
                                
        }

            /* adjust control action for BT_PARAMS_SCO control action  */
        if ((pActionMsg->IndicationForControlAction == ATH_BT_SCO) || 
            (pActionMsg->IndicationForControlAction == ATH_BT_ESCO)) {
                
            A_UINT32 scoOptFlags = 0;
            
            if (pActionMsg->StateForControlAction != STATE_ON) {
                    /* nothing to adjust in OFF case */
                break;     
            }           
            
            if (pActionMsg->ControlAction.Type != BT_CONTROL_ACTION_PARAMS) {
                    /* only modify action that issues a PARAMS control action */
                break;                
            }        
            
            pParamsCmd = (WMI_SET_BT_PARAMS_CMD *)(pActionMsg->ControlAction.Buffer);
            
            if (pParamsCmd->paramType != BT_PARAM_SCO) {
                    /* only modify SCO params */
                break;    
            }  

            if ((pInfo->SCOConnectInfo.LinkType == BT_LINK_TYPE_ESCO) && 
                (pInfo->SCOConnectInfo.Valid)) {
                    
                UCHAR scoSlots;
            
                /* decode packet length to get packet type */
                
                ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: BT PARAMS eSCO detected, txpkt length :%d \r\n"), 
                      pInfo->SCOConnectInfo.TxPacketLength));
                
                /* Note: We allow close range optimizations for
                 * packet types that are EDR data rates see BT_SCO_ALLOW_CLOSE_RANGE_OPT */
                       
                if (pInfo->SCOConnectInfo.TxPacketLength <= 30) {
                    /* EV3 */
                    scoSlots = 1;
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("     eSCO - EV3\r\n")));
                } else if (pInfo->SCOConnectInfo.TxPacketLength <= 60) {
                    /* 2-EV3 */
                    scoSlots = 1;
                    scoOptFlags |= BT_ENABLE_SCO_OPT;
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("     eSCO - 2-EV3\r\n")));
                } else if (pInfo->SCOConnectInfo.TxPacketLength <= 90) {
                    /*3-EV3 */
                    scoSlots = 1; 
                    scoOptFlags |= BT_ENABLE_SCO_OPT ;
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("     eSCO - 3-EV3\r\n")));           
                } else if (pInfo->SCOConnectInfo.TxPacketLength <= 120) {
                    /* EV4: */  
                    scoSlots = 3;   
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("     eSCO - EV4\r\n")));            
                } else if (pInfo->SCOConnectInfo.TxPacketLength <= 180) {
                    /* EV5: */  
                    scoSlots = 3;   
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("     eSCO - EV5\r\n")));
                } else if (pInfo->SCOConnectInfo.TxPacketLength <= 360) {
                    /* 2-EV5 */
                    scoSlots = 3;
                    scoOptFlags |= BT_ENABLE_SCO_OPT;
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("     eSCO - 2-EV5\r\n")));
                } else {
                    /* 3-EV5 */
                    scoSlots = 3;
                    scoOptFlags |= BT_ENABLE_SCO_OPT;
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("     eSCO - 3-EV5\r\n")));
                }                                     
                    /* account for RX/TX */
                scoSlots *= 2;
 /*             pParamsCmd->info.scoParams.p2lrpOptModeBound=7;
                pParamsCmd->info.scoParams.p2lrpNonOptModeBound=9;
                BT_SCO_SET_DEFAULT_OVERRIDE(scoOptFlags);*/
                /*uncomment the above if need to modify the 
                  Atheros recommended default values. It is expected
                  that if you are modifying this, you know what you
                  know the details*/             
                pParamsCmd->info.scoParams.noSCOSlots =  scoSlots;
                
                if (pInfo->SCOConnectInfo.TransmissionInterval >= scoSlots) {
                    pParamsCmd->info.scoParams.noIdleSlots = pInfo->SCOConnectInfo.TransmissionInterval - 
                                                             scoSlots;
                } else {
                    ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Invalid scoSlot,  got:%d, transInt: %d \r\n"), 
                       scoSlots,
                       pInfo->SCOConnectInfo.TransmissionInterval));
                }
                                                         
            } else {
                    /* legacy SCO */
                pParamsCmd->info.scoParams.noSCOSlots = 2;
                pParamsCmd->info.scoParams.noIdleSlots = 4;
            }
            
                /* provide SCO packet type hint */             
            pParamsCmd->info.scoParams.scoOptFlags = scoOptFlags;
                                        
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: BT PARAMS SCO adjustment (%s) \r\n"), 
                    pInfo->SCOConnectInfo.LinkType == BT_LINK_TYPE_ESCO ? _T("eSCO"):_T("SCO")));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    numScoCyclesForceTrigger : %d\r\n"),
                    pParamsCmd->info.scoParams.numScoCyclesForceTrigger));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    dataResponseTimeout      : %d\r\n"),
                    pParamsCmd->info.scoParams.dataResponseTimeout));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    stompScoRules            : %d\r\n"),
                    pParamsCmd->info.scoParams.stompScoRules));                
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    stompDutyCyleVal         : %d\r\n"),
                    pParamsCmd->info.scoParams.stompDutyCyleVal));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    stompDutyCyleMaxVal      : %d\r\n"),
                    pParamsCmd->info.scoParams.stompDutyCyleMaxVal));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    psPollLatencyFraction    : %d\r\n"),
                    pParamsCmd->info.scoParams.psPollLatencyFraction));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    noSCOSlots               : %d\r\n"),
                    pParamsCmd->info.scoParams.noSCOSlots));
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    noIdleSlots              : %d\r\n"),
                    pParamsCmd->info.scoParams.noIdleSlots));                           
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("    scoOptFlags              : 0x%X \r\n"),
                    pParamsCmd->info.scoParams.scoOptFlags));                    
        }  
        
    } while (FALSE); 
    
}

static void BtStateActionProper(ATHBT_FILTER_INFO *pInfo, ATHBT_STATE_INDICATION Indication, ATHBT_STATE State)
{
    HANDLE                  hWait = NULL;
    BT_ACTION_MSG           *pActionMsg;
    DL_LIST                 *pListEntry;
    BT_CONTROL_ACTION_ITEM  *pBtControlAction;
    int                     queued = 0;

    LOCK_FILTER(pInfo);
    
    pBtControlAction = FCore_GetControlAction(&pInfo->FilterCore, 
                                              Indication,
                                              State,
                                              NULL);
    
    while (pBtControlAction != NULL) {                                   
                                        
            /* allocate an action message */
        pListEntry = DL_ListRemoveItemFromHead(&pInfo->FreeActionMsgList);
    
        if (NULL == pListEntry) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: action messages exhausted \r\n"))); 
            break;
        }
                         
        pActionMsg = A_CONTAINING_STRUCT(pListEntry, BT_ACTION_MSG, ListEntry);
            /* save state for later flushing */
        pActionMsg->StateForControlAction = State;
        pActionMsg->IndicationForControlAction = Indication;
            /* we need to buffer the control actions */       
        memcpy(&pActionMsg->ControlAction, 
               &pBtControlAction->ControlAction, 
               sizeof(pActionMsg->ControlAction));
 
        if (pActionMsg->Blocking) {
                /* this is the action to wait on */
            hWait = pActionMsg->hWaitEvent;
            ResetEvent(hWait);    
        }
        
            /* allow for adjustments to the control action beyond the defaults */
        AdjustBtControlAction(pInfo, pActionMsg);
                
            /* queue action */
        QUEUE_BT_ACTION_MSG(pInfo,pActionMsg);
        queued++;
        
            /* get the next action using the current one as a starter */
        pBtControlAction = FCore_GetControlAction(&pInfo->FilterCore, 
                                                  Indication,
                                                  State,
                                                  pBtControlAction);
    } 
    
    UNLOCK_FILTER(pInfo);

    if (queued > 0 ) {
            /* wake thread to process all the queued up actions */
        SetEvent(pInfo->hWakeEvent);
    }
        
        /* check if we need to block until the dispatch thread issued the last action
         * if the adapter becomes unavailable we cannot block the thread (queue will stall), 
         * so only block if the adapter is available and use a reasonable timeout */
    if (hWait) {
        WaitForSingleObject(hWait, ACTION_WAIT_TIMEOUT);
    }    
    
    
}

void
btfilter_DebugInit()
{
    FILE        *fStream;
    fStream = fopen("\\btfilter_scan.txt", "w");
    if (fStream)
    {
        fclose(fStream);
    }
}

void
btfilter_Printf(A_INT32 mask, A_CHAR * format, ...)
{
    va_list     argList;    /* holds the arguement list passed in */
    A_CHAR      buffer[1024];

    FILE        *fStream;

        /* get the argument list */
        va_start(argList, format);

        /*
         * using vprintf to perform the printing it is the same is printf, only
         * it takes a va_list or arguements
         */
    if (S_OK != StringCchVPrintfA (buffer, sizeof (buffer), format, argList))
    {
        return;
    }
    fStream = fopen("\\btfilter_scan.txt", "a+");
    fprintf(fStream,"%i: %s\n",GetTickCount(),buffer);
    if (fStream)
    {
        fclose(fStream);
    }
    return;
}


static void DoBtStateAction(ATHBT_FILTER_INFO *pInfo, ATHBT_STATE_INDICATION Indication, ATHBT_STATE State)
{
    A_UINT32 bitmap = FCore_GetCurrentBTStateBitMap(&pInfo->FilterCore);

    if ((Indication == ATH_BT_INQUIRY) && (State == STATE_ON)) {
        int i;
        for (i=0; i<ATH_BT_MAX_STATE_INDICATION; ++i) {
            if ((i!=Indication) && (bitmap & (1<<i))) {
                BtStateActionProper(pInfo,i,STATE_OFF);
            }
        }
    }

    if ((Indication == ATH_BT_A2DP) && ((bitmap & (1<<ATH_BT_SCO)) || (bitmap & (1<<ATH_BT_ESCO)))) {
        /* SCO protection is running, don't take any actions */
        return;
    }
    
    if (((Indication == ATH_BT_SCO) || (Indication == ATH_BT_ESCO)) && (State == STATE_ON)) {
        if (bitmap & (1<<ATH_BT_A2DP)) {
            BtStateActionProper(pInfo,ATH_BT_A2DP,STATE_OFF);
        }
    }
    
    BtStateActionProper(pInfo, Indication, State);
    
    if ((Indication == ATH_BT_INQUIRY) && (State == STATE_OFF)) {
        SyncBTState(pInfo);
    }

    if (((Indication == ATH_BT_SCO) || (Indication == ATH_BT_ESCO)) && (State == STATE_OFF)) {
        if (bitmap & (1<<ATH_BT_A2DP)) {
            BtStateActionProper(pInfo,ATH_BT_A2DP,STATE_ON);
        }
    }
}


#define HCI_GET_OP_CODE(p)          (((A_UINT16)((p)[1])) << 8) | ((A_UINT16)((p)[0]))

static void AthFilterCmdEventsCallback(void *pContext, ATHBT_HCI_CTRL_TYPE Type, unsigned char *pBuffer, int Length)
{
    ATHBT_STATE_INDICATION  indication;
    ATHBT_FILTER_INFO       *pInfo = (ATHBT_FILTER_INFO *)pContext;
    ATHBT_STATE             state;
    
    if (Type == ATHBT_HCI_EVENT) {
        if (HCI_GET_EVENT_CODE(pBuffer) == HCI_EVT_NUM_COMPLETED_PKTS) {
                /* don't delays these packets, we don't act on them anyways */
            return;    
        }            
    }
    
    if (pInfo->Shutdown) {
        return;    
    }
    
        /* the filter state machine needs to be protected in case the HCI layer
         * can process commands and events in an unserialize manner */
    LOCK_FILTER(pInfo);
    
    if (Type == ATHBT_HCI_COMMAND) {
        if (ZONE_HCI_CMD) {
            UINT16 opcode = HCI_GET_OP_CODE(pBuffer);
            ATH_LOG_MSG(ZONE_HCI_CMD,(TEXT("ATHBT: HCI Command: 0x%4.4X OGF:0x%X OCF:0x%X  \r\n"), 
                opcode, opcode >> 10, opcode & 0x3FF));
            AthFilterDumpBuffer(pBuffer, Length, TEXT("BT HCI Command"));
        }
        indication = FCore_FilterBTCommand(&pInfo->FilterCore, pBuffer, Length, &state);
    } else {
        if (ZONE_HCI_EVT) {
            AthFilterDumpBuffer(pBuffer, Length, TEXT("BT HCI Event"));
        }
        indication = FCore_FilterBTEvent(&pInfo->FilterCore, pBuffer, Length, &state);
            /* check SCO and ESCO connection events */
        if ((indication == ATH_BT_SCO) || (indication == ATH_BT_ESCO)) {

	 RETAILMSG(1, (L" ATHBT: SCO or ESCO CONNECT COMPLETE- AthFilterCmdEventsCallback\n"));
			/* do {
					A_UINT32 ret_status;

					btfilter_DebugInit();
					ret_status = BthWriteScanEnableMask(0x0);
				        RETAILMSG(TRUE, (L"**** WRITESCAN ENABLE MASK STATUS *****: ret status = %d\n", ret_status));
					btfilter_Printf(1, "**** WRITESCAN ENABLE MASK STATUS *****: ret status = %d\n", ret_status);
				}while(FALSE);*/

            if (HCI_GET_EVENT_CODE(pBuffer) == HCI_EVT_SCO_CONNECT_COMPLETE) {    
                ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: SCO_CONNECT_COMPLETE (%s) \r\n"), 
                    (state == STATE_ON) ? _T("ON") : _T("OFF"))); 
                if (state == STATE_ON) {
                        /* save these off for the BT Action adjustment */
                    pInfo->SCOConnectInfo.LinkType = GET_BT_CONN_LINK_TYPE(pBuffer);
                    pInfo->SCOConnectInfo.TransmissionInterval = GET_TRANS_INTERVAL(pBuffer);
                    pInfo->SCOConnectInfo.RetransmissionInterval = GET_RETRANS_INTERVAL(pBuffer);
                    pInfo->SCOConnectInfo.RxPacketLength = GET_RX_PKT_LEN(pBuffer);
                    pInfo->SCOConnectInfo.TxPacketLength = GET_TX_PKT_LEN(pBuffer);
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: SCO conn info (%d, %d, %d, %d, %d)) \r\n"), 
                       pInfo->SCOConnectInfo.LinkType,
                       pInfo->SCOConnectInfo.TransmissionInterval,
                       pInfo->SCOConnectInfo.RetransmissionInterval,
                       pInfo->SCOConnectInfo.RxPacketLength,
                       pInfo->SCOConnectInfo.TxPacketLength));
                        /* now valid */
                    pInfo->SCOConnectInfo.Valid = TRUE;
                } else {
                        /* disconnected, invalidate */
                    pInfo->SCOConnectInfo.Valid = FALSE; 
                }             
            }
        }
    }
    
    UNLOCK_FILTER(pInfo);
    
    if (indication == ATH_BT_NOOP) {
        return;    
    }

    ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: New Indication :%d State:%s (map:0x%X) \r\n"), 
        indication, (state == STATE_ON) ? _T("ON") : _T("OFF"), FCore_GetCurrentBTStateBitMap(&pInfo->FilterCore))); 


    if (pInfo->AdapterAvailable) {
        DoBtStateAction(pInfo, indication, state);
    }
}

static ATHBT_STATE_INDICATION IndicateA2DP(ATHBT_FILTER_INFO        *pInfo , 
                                           ATHBT_STATE_INDICATION   Indication,
                                           ATHBT_STATE              State,
                                           unsigned char            *pACLBuffer)
{
    A_UINT16        btConnHandle;
         
        /* connection handle is the first 12 bits */
    btConnHandle = (((A_UINT16)((pACLBuffer)[0]) | (((A_UINT16)((pACLBuffer)[1])) << 8))) & 0xFFF;            

    ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: A2DP Indication State:%s (conn handle: 0x%04X) \r\n"), 
        (State == STATE_ON) ? _T("ON") : _T("OFF"), btConnHandle)); 

        /* pass the indication change to the stack adaptation layer to see if it wants to 
         * do any extra handling, it could override the indication */
    return AthBtStackNotifyA2DPIndicationFromFilterCore(&pInfo->BtStackNotifyInfo,
                                                        State,
                                                        btConnHandle);
}

                                                              
static void AthFilterAclDataOutCallback(void *pContext, unsigned char *pBuffer, int Length)
{
    ATHBT_STATE_INDICATION  indication;
    ATHBT_FILTER_INFO       *pInfo = (ATHBT_FILTER_INFO *)pContext;
    ATHBT_STATE             state;
    
    if (pInfo->Shutdown) {
        return;    
    }
    
    LOCK_FILTER(pInfo);
    
    indication = FCore_FilterACLDataOut(&pInfo->FilterCore,
                                        pBuffer, 
                                        Length, 
                                        &state);
                                        
    if (indication == ATH_BT_A2DP) {
        indication = IndicateA2DP(pInfo, 
                                  ATH_BT_A2DP,
                                  state,
                                  pBuffer);   
    }
    
    if (indication != ATH_BT_NOOP) {
        indication = FCore_FilterIndicatePreciseState(&pInfo->FilterCore, indication, state, &state);            
    }        
    
    UNLOCK_FILTER(pInfo);
    
    if (indication == ATH_BT_NOOP) {
        return;    
    }

    ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: New Indication :%d State:%s (map:0x%X) \r\n"), 
       indication, (state == STATE_ON) ? _T("ON") : _T("OFF"), FCore_GetCurrentBTStateBitMap(&pInfo->FilterCore))); 

    if (pInfo->AdapterAvailable) {
        DoBtStateAction(pInfo, indication, state);
    }
    
}

static void AthFilterAclDataInCallback(void *pContext, unsigned char *pBuffer, int Length)
{   
    ATHBT_STATE_INDICATION  indication;
    ATHBT_FILTER_INFO       *pInfo = (ATHBT_FILTER_INFO *)pContext;
    ATHBT_STATE             state;
    
    if (pInfo->Shutdown) {
        return;    
    }
    
    LOCK_FILTER(pInfo);
    
    indication = FCore_FilterACLDataIn(&pInfo->FilterCore,
                                        pBuffer, 
                                        Length, 
                                        &state);
    
    if (indication == ATH_BT_A2DP) {
        indication = IndicateA2DP(pInfo, 
                                  ATH_BT_A2DP,
                                  state,
                                  pBuffer);   
    }
                                                                 
    if (indication != ATH_BT_NOOP) {
        indication = FCore_FilterIndicatePreciseState(&pInfo->FilterCore, indication, state, &state);            
    }          
                                                                 
    UNLOCK_FILTER(pInfo);
    
    if (indication == ATH_BT_NOOP) {
        return;    
    }

    ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: New Indication :%d State:%s (map:0x%X) \r\n"), 
          indication, (state == STATE_ON) ? _T("ON") : _T("OFF"), FCore_GetCurrentBTStateBitMap(&pInfo->FilterCore))); 

    if (pInfo->AdapterAvailable) {
        DoBtStateAction(pInfo, indication, state);
    }


}

static void AthFilterIndicateStateCallback(void *pContext, ATHBT_STATE_INDICATION Indication, ATHBT_STATE State, unsigned char LMPVersion)
{
    ATHBT_FILTER_INFO *pInfo = (ATHBT_FILTER_INFO *)pContext;
    ATHBT_STATE             newState;
    
    if (pInfo->Shutdown) {
        return;    
    }
    
    LOCK_FILTER(pInfo);
    Indication = FCore_FilterIndicatePreciseState(&pInfo->FilterCore, Indication, State, &newState);
    UNLOCK_FILTER(pInfo);
    
    if (Indication == ATH_BT_NOOP) {
        return;    
    }
    
    ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: New Indication :%d State:%s (map:0x%X) \r\n"), 
       Indication, (newState == STATE_ON) ? _T("ON") : _T("OFF"), FCore_GetCurrentBTStateBitMap(&pInfo->FilterCore))); 

    if (pInfo->AdapterAvailable) {
        DoBtStateAction(pInfo, Indication, newState);
    }
}

/* main thread that dispatches NDISUIO notifications and BT Action messages */
static DWORD FilterThread(LPVOID pContext)
{
    HANDLE              eventArray[3];
    ATHBT_FILTER_INFO   *pInfo = (ATHBT_FILTER_INFO *)pContext;
    DWORD               waitStatus;
    LONG                index;
    int                 eventObjectCount;
        
    ATH_LOG_MSG(ZONE_INIT,(TEXT("ATHBT: Filter thread starting up (hThread:0x%X , Id:0x%X)\r\n"), 
      pInfo->hFilterThread, GetCurrentThreadId())); 

    if (!SetUpNdisIF(pInfo)) {
        return FALSE;    
    }
    
        /* setup events to wait on */    
    eventArray[0] = pInfo->hWakeEvent;
    eventArray[1] = pInfo->hMsgQueue;
    eventObjectCount = 2;

    if (pInfo->BtStackNotifyInfo.hBtNotify != NULL) {
            /* BT stack notification was enabled, wait on the additional event */
        eventArray[2] = pInfo->BtStackNotifyInfo.hBtNotify;
        eventObjectCount++; 
    }

    while (1) {
    
        waitStatus = WaitForMultipleObjects(eventObjectCount, eventArray, FALSE, INFINITE);
        
        if (WAIT_FAILED == waitStatus) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Wait failed (%d) \r\n"), GetLastError())); 
            break;    
        }
    
        index = (LONG)waitStatus - (LONG)WAIT_OBJECT_0;
        
        if ((index < 0) || (index >= eventObjectCount)) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Invalid Wait index (%d) \r\n"), index)); 
            break;            
        }
        
        if (1 == index) {
            ProcessNdisNotifications(pInfo);
        }
        
        if (0 == index) {
            
            if (pInfo->AdapterAvailable) { 
                ProcessBTActionMessages(pInfo, BTACTION_QUEUE_NORMAL, ATH_BT_NOOP);
            }
            
            if (pInfo->Shutdown) {
                
                    /* before we exit we need to counter-act the coexistence settings.
                     * Currently we just indicate that each state is now OFF (if they are ON)
                     * This state synchronization is typically required on HOT-removable BT adapters
                     * or where the low level adapter can be surprise removed before the BT stack can
                     * clean up HCI connections and states */
                     
                if (pInfo->AdapterAvailable) {
                    int         indication, newIndication;
                    ATHBT_STATE newState;
                    
                        /* the BT adapter is going away, indicate that all indications are now in the
                         * OFF state, this may queue up control action messages, which is okay */
                    for (indication = 0; indication < ATH_BT_MAX_STATE_INDICATION; indication++) {
                        LOCK_FILTER(pInfo);
                        newIndication = FCore_FilterIndicatePreciseState(&pInfo->FilterCore, 
                                                                         indication, 
                                                                         STATE_OFF, 
                                                                         &newState);
                        UNLOCK_FILTER(pInfo);
                            
                        if (newIndication == ATH_BT_NOOP) {
                            continue;
                        }
                        
                        DoBtStateAction(pInfo, indication, newState);
                    }
                    
                        /* issue control actions */
                    ProcessBTActionMessages(pInfo, BTACTION_QUEUE_SYNC_STATE, ATH_BT_NOOP);
                    
                }
    
    
                ATH_LOG_MSG(ZONE_INIT,(TEXT("ATHBT: Filter thread shutting down \r\n")));
                break;    
            } 
            
        }

        if (2 == index) {
                /* BT Stack notification event */
            if (pInfo->BtStackNotifyInfo.Callback != NULL) {
                pInfo->BtStackNotifyInfo.Callback(&pInfo->BtStackNotifyInfo);
            }            
        }
    
    }
    
    return 0;
}

void ProcessBTActionMessages(ATHBT_FILTER_INFO      *pInfo, 
                             BTACTION_QUEUE_PROC    Process,
                             ATHBT_STATE_INDICATION StateToFlush)
{
    BT_ACTION_MSG *pActionMsg = NULL;
    DL_LIST       *pListEntry = NULL;
    
    LOCK_FILTER(pInfo);
    
    while (1) {
        
        /* determine how we want to pull the message out */
        
        if (BTACTION_QUEUE_FLUSH_STATE == Process) {
            if (NULL == pListEntry) {
                    /* first time through */
                if (!DL_LIST_IS_EMPTY(&pInfo->BTActionMsgList)) {
                        /* get the entry at the head of the list , don't remove */     
                    pListEntry = DL_LIST_GET_ITEM_AT_HEAD(&pInfo->BTActionMsgList);     
                }
            } else {
                    /* move onto the next one */
                pListEntry = pListEntry->pNext;
                if (pListEntry == &pInfo->BTActionMsgList) {
                        /* reached the end */
                    pListEntry = NULL;    
                }    
            } 
        } else {
            /* for all others we are removing items from the head of the queue */
            pListEntry = DL_ListRemoveItemFromHead(&pInfo->BTActionMsgList);            
        }
        
        if (NULL == pListEntry) {
                /* queue was empty */
            break;
        }
        
        pActionMsg = A_CONTAINING_STRUCT(pListEntry, BT_ACTION_MSG, ListEntry);
     
        /* now determine what to do with the message that was found */
        
        switch (Process) {
            
            case BTACTION_QUEUE_FLUSH_STATE:
                
                /* caller wants to just flush action messages matching a state */
                
                if (pActionMsg->StateForControlAction == StateToFlush) {
                    
                    ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: Removed action for state=%d from queue \r\n"),
                        StateToFlush));
                        /* remove from list, it will get freed below */
                    DL_ListRemove(&pActionMsg->ListEntry);
                        /* this would re-start the scan to the head of the list each time
                         * we found one.  This type of flush doesn't happen very often so restarting
                         * from the head of the list and rescanning isn't time consuming */
                    pListEntry = NULL;
                } else {
                        /* not the one we are interested in */
                    pActionMsg = NULL;
                }
                break;
                
            case BTACTION_QUEUE_NORMAL:
            case BTACTION_QUEUE_SYNC_STATE:
            
                /* issue/execute actions */
                
                UNLOCK_FILTER(pInfo);
                
                ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: Processing action for indication=%d (%s) (%s) \r\n"),
                        pActionMsg->IndicationForControlAction,
                        (pActionMsg->StateForControlAction == STATE_ON) ? TEXT("ON") : TEXT("OFF"),
                        (BTACTION_QUEUE_SYNC_STATE == Process) ? TEXT("Sync State") : TEXT("Normal"))); 
                        
                if (BTACTION_QUEUE_SYNC_STATE == Process) {
                        /* let's not issue these too fast ... */
                    Sleep(10);    
                }
                
                ExecuteBtAction(pInfo, pActionMsg);      
                
                if (pActionMsg->Blocking) {
                    pActionMsg->Blocking = FALSE;
                        /* set the event to unblock the caller */
                    SetEvent(pActionMsg->hWaitEvent);
                }
                    
                LOCK_FILTER(pInfo);      
                 
                break;
                
            case BTACTION_QUEUE_FLUSH_ALL:
                ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: Flushed action for state=%d from queue \r\n"),
                        pActionMsg->StateForControlAction)); 
                /* nothing to do here, the action message will get recycled below */
                break;
                
            default:            
                break;
        }
        
        if (pActionMsg) {
                /* recycle message */        
            FREE_BT_ACTION_MSG(pInfo,pActionMsg);
        }
        
    }
    
    UNLOCK_FILTER(pInfo);
    
}

static void SyncBTState(ATHBT_FILTER_INFO *pInfo)
{
    int      stateIndication;
    A_UINT32 stateBitMap;
    
    LOCK_FILTER(pInfo);
    stateBitMap = FCore_GetCurrentBTStateBitMap(&pInfo->FilterCore);
    UNLOCK_FILTER(pInfo);
    
    /* the state bit map is a simple STATE ON/OFF bit map, if we detect that one of the states is ON
     * we process the BT action to synchronize the WLAN side with the BT radio state */
        
    for (stateIndication = 0; stateIndication < ATH_BT_MAX_STATE_INDICATION; stateIndication++) {
        if (stateBitMap & (1 << stateIndication)) {
                /* this state is ON */
            DoBtStateAction(pInfo, stateIndication, STATE_ON);
        }
    }   
    
}


/* called by the NDIS IF side to indicate that the WLAN adapter is available/unavailable 
 * this callback is called in the context of our single dispatch thread */
void HandleAdapterEvent(ATHBT_FILTER_INFO *pInfo, ATH_ADAPTER_EVENT Event)
{
        
    switch (Event) {
        
        case ATH_ADAPTER_ARRIVED:
            pInfo->AdapterAvailable = TRUE;  
                /* sync BT state */
            SyncBTState(pInfo);
                /* the call to sync BT state may queue a bunch of actions to the action queue, we
                 * want to issues these differently */
            ProcessBTActionMessages(pInfo, BTACTION_QUEUE_SYNC_STATE, ATH_BT_NOOP);
            break; 
        
        case ATH_ADAPTER_REMOVED:
            pInfo->AdapterAvailable = FALSE;  
                /* flush messages */
            ProcessBTActionMessages(pInfo, BTACTION_QUEUE_FLUSH_ALL, ATH_BT_NOOP);  
            break;
            
        default:
            break;
    }
    
    
}

void AthFilterDumpBuffer(A_UCHAR *buffer, int length, TCHAR *pDescription)
{
    TCHAR    stream[60];
    int      i;
    int      offset, count;

    ATH_LOG_MSG(ZONE_INIT,(TEXT("<---------Dumping %d Bytes : %s ------>\r\n"), length, pDescription));

    count = 0;
    offset = 0;
    for(i = 0; i < length; i++) {
        _stprintf(stream + offset, TEXT("%2.2X "), buffer[i]);
        count ++;
        offset += 3;

        if (count == 16) {
            count = 0;
            offset = 0;
            ATH_LOG_MSG(ZONE_INIT,(TEXT("[H]: %s\r\n"), stream));
            A_MEMZERO(stream, sizeof(stream));
        }
    }

    if(offset != 0) {
        ATH_LOG_MSG(ZONE_INIT,(TEXT("[H]: %s\r\n"), stream));
    }
    
    ATH_LOG_MSG(ZONE_INIT,(TEXT("<------------------------------------------------->\r\n")));
}

#define BTFILTER_BASE_PATH     TEXT("\\Services\\ATHSRVC\\BTFILTER")
#define ACTION_OVERRIDES_PATH  (BTFILTER_BASE_PATH TEXT("\\ACTION_OVERRIDES"))

#define MAX_VAL_NAME            32
#define MAX_VAL_DATA_LENGTH     128

const WCHAR *g_IndicationStrings[ATH_BT_MAX_STATE_INDICATION] = 
{
   TEXT("NOP"),   
   TEXT("INQUIRY"),
   TEXT("CONNECT"),
   TEXT("SCO"),
   TEXT("ACL"),
   TEXT("A2DP"),
   TEXT("ESCO"),
};

static void ProcessActionOverride(ATHBT_FILTER_INFO *pInfo, 
                                  WCHAR             *pIndicationStr, 
                                  WCHAR             *pModifyAction, 
                                  WCHAR             *pAction)
{
    int                          i;
    ATHBT_STATE_INDICATION       indication;
    ATHBT_STATE                  state = STATE_MAX;
    ATHBT_MODIFY_CONTROL_ACTION  modifyAction = ATHBT_MODIFY_CONTROL_ACTION_NOOP;
    char                         charBuffer[MAX_VAL_DATA_LENGTH];
    
        /* parse the indication string to figure which indication and state to change
         * i.e.  <indication>-ON or <indication>-OFF */   
    for (indication = 0; indication < ATH_BT_MAX_STATE_INDICATION; indication++) {
        if (wcsstr(pIndicationStr, g_IndicationStrings[indication]) != NULL) {
                /* found one */
            if (wcsstr(pIndicationStr, TEXT("-ON")) != NULL) {
                state = STATE_ON;    
            }    
                    
            if (wcsstr(pIndicationStr, TEXT("-OFF")) != NULL) {
                state = STATE_OFF;
            }  
            
            if (wcsstr(pModifyAction, TEXT("REPLACE")) != NULL) {
                modifyAction = ATHBT_MODIFY_CONTROL_ACTION_REPLACE;
            }     
            
            if (wcsstr(pModifyAction, TEXT("APPEND")) != NULL) {
                modifyAction = ATHBT_MODIFY_CONTROL_ACTION_APPEND;
            } 
            
            break;    
        }
    }    
    
    if ((indication == ATH_BT_MAX_STATE_INDICATION) || 
        (state == STATE_MAX)               || 
        (modifyAction == ATHBT_MODIFY_CONTROL_ACTION_NOOP)) {
        return;    
    }
    
    ATH_LOG_MSG(ZONE_INIT,(TEXT("ATHBT : Found Action override : %s (%s) (%s) \r\n"), 
                                pIndicationStr, pModifyAction, pAction));
   
    A_MEMZERO(charBuffer, sizeof(charBuffer));
    
    for (i = 0; (i < (int)wcslen(pAction)) && (i < (MAX_VAL_DATA_LENGTH - 1)); i++) {
        charBuffer[i] = (char)pAction[i];    
    }
    
    FCore_ModifyControlActionString(&pInfo->FilterCore, 
                                    indication,
                                    state,
                                    charBuffer,
                                    i,
                                    modifyAction);
                                    
}

static void GetActionStringOverrides(ATHBT_FILTER_INFO *pInfo)
{
    LONG  status;    
    HKEY  hOpenKey;
    WCHAR valueName[MAX_VAL_NAME];
    DWORD valueNameLength;
    WCHAR multiString[MAX_VAL_DATA_LENGTH];
    DWORD multiStringLength;
    DWORD type;
    DWORD index = 0;
    
    status = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                          ACTION_OVERRIDES_PATH,
                          0,
                          0,
                          &hOpenKey);

    if (status != ERROR_SUCCESS) {
        return;
    }
    
    while (1) {
        
        valueNameLength = MAX_VAL_NAME;    
        multiStringLength = sizeof(multiString);
        
        status = RegEnumValue(hOpenKey,
                              index, 
                              valueName,
                              &valueNameLength,
                              NULL,
                              &type,
                              (LPBYTE)multiString,
                              &multiStringLength);
                                 
        if (status == ERROR_NO_MORE_ITEMS) {
                /* all done */
            break;    
        }  
        
        if ((status == ERROR_SUCCESS) && (type == REG_MULTI_SZ)) {
            int chars;
                        
                /* convert to total characters (including NULLs and double NULLs) */
            multiStringLength = multiStringLength/sizeof(WCHAR);
                /* get the number of characters for the first NULL terminated string */
            chars = wcslen(multiString);   
                /* make sure second string has at least 1 character taking into account a single NULL
                 * and a double NULL at the end */           
            if (((int)multiStringLength - chars - 3) > 0 ) {
                ProcessActionOverride(pInfo, 
                                      valueName, 
                                      multiString, 
                                      &multiString[chars + 1]);  
            }  
        }
        
        index++;
        
    }
}

static LONG GetDWORDSetting(HKEY   hKey, 
                            WCHAR  *pKeyPath,
                            WCHAR  *pValueName, 
                            DWORD  *pValue)
{
    LONG  status = ERROR_SUCCESS;      
    HKEY  hOpenKey;     
    DWORD type;
    DWORD value;
    ULONG bufferSize;
    
    do {
        
        status = RegOpenKeyEx(hKey,
                              pKeyPath,
                              0,
                              0,
                              &hOpenKey);
    
        if (status != ERROR_SUCCESS) {
            break;
        }
        
        bufferSize = sizeof(DWORD);
        
        status = RegQueryValueEx(hOpenKey,
                                 pValueName,
                                 NULL,
                                 &type,
                                 (PUCHAR)&value,
                                 &bufferSize);
    
        RegCloseKey(hOpenKey); 
        
        if (status != ERROR_SUCCESS) {
            break;
        }
        
        if (type != REG_DWORD) {
            status = ERROR_INVALID_PARAMETER;
            break;    
        }
              
        *pValue = value;   
            
    } while (FALSE);
        
    return status; 
}


static void GetFilterRegistrySettings(ATHBT_FILTER_INFO *pInfo)
{
    DWORD  value;
    
    value = 0;   
    
    GetDWORDSetting(HKEY_LOCAL_MACHINE, 
                    BTFILTER_BASE_PATH,
                    TEXT("LogToViewer"), 
                    &value);
                    
    if (value) {
        g_LogToMessageQueue = TRUE;
    }
        
    GetDWORDSetting(HKEY_LOCAL_MACHINE, 
                    BTFILTER_BASE_PATH,
                    TEXT("BtCoexFlags"), 
                    &pInfo->BtCoexFlags);
   
    ATH_LOG_MSG(ZONE_INIT,(TEXT("ATHBT: BtCoexFlags : 0x%X \r\n"), pInfo->BtCoexFlags));
}

static void InitializeLogging()
{
    MSGQUEUEOPTIONS    msgOptions;   
     
    do {
        
        ATHMSGQ_INIT_QUEUE_OPTIONS(msgOptions,FALSE);
              
        g_hLogMsgQueue = CreateMsgQueue(ATHBTFILTER_LOG_MSG_QUEUE_NAME, &msgOptions);

        if (NULL == g_hLogMsgQueue) {
            RETAILMSG(1,(TEXT("ATHBT: Failed to create logging message queue (%d). \r\n"),
                GetLastError())); 
                /* force off logging to the message queue */    
            g_LogToMessageQueue = FALSE;
            break;
        }
               
        g_LogSetupComplete = TRUE;
        RETAILMSG(1,(TEXT("ATHBT:  Logging to message queue enabled! \r\n")));
        
    } while (FALSE);
   
}

static void LogToMessageQueue(TCHAR *pStringBuff, ULONG Characters)
{
    WriteMsgQueue(g_hLogMsgQueue,
                  pStringBuff,
                  Characters *(sizeof(TCHAR)),
                  0,
                  0);
}

void AthBtFilterLogMessage(LPCWSTR pFormatString,...)
{
    HRESULT hr;
    va_list argList;
    WCHAR   strBuf[ATHMSGQ_LOG_MAX_CHARS_PER_MSG + 1];
    ULONG   remaining;
    ULONG   maxChars;

    maxChars = ATHMSGQ_LOG_MAX_CHARS_PER_MSG + 1;
     
    va_start(argList, pFormatString);

    hr = StringCchVPrintfExW(strBuf, 
                             maxChars, 
                             NULL, 
                             &remaining, 
                             STRSAFE_IGNORE_NULLS, 
                             pFormatString, 
                             argList);
    
    va_end(argList);

    if (g_LogToMessageQueue && !g_LogSetupComplete) {
            /* try to setup logging */
        InitializeLogging();     
    }
            
    if (SUCCEEDED(hr)) {
        if (g_LogToMessageQueue) {
            LogToMessageQueue(strBuf, (maxChars - remaining + 1));    
        } else {
            OutputDebugStringW(strBuf);
        }
    } else {
        RETAILMSG(1,(TEXT(" Format String : (%s) failed to convert to output!!!! \r\n"),pFormatString));    
    }
    
} 

static void CleanupLogging()
{
    if (g_hLogMsgQueue != NULL) {
        CloseMsgQueue(g_hLogMsgQueue);
        g_hLogMsgQueue = NULL;    
    }
    g_LogToMessageQueue = FALSE;
    g_LogSetupComplete = FALSE;
}



