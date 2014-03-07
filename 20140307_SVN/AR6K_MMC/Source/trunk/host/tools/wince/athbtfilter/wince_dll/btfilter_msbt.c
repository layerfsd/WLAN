/*
 * Bluetooth filter , Microsoft BT stack support
 * 
 * This file handles Windows CE BT stack notifications
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

#include "a_types.h"
#include "a_osapi.h"
#include "btfilter_wince.h"
#include "btfilter_msbt.h"

#ifndef BTE_CLASS_AVDTP    
static DWORD  A2DPIndicationThread(DWORD *pContext);
#endif


void AthBtStackNotificationsShutdown(ATH_BT_STACK_NOTIFY_INFO  *pNotifyInfo)
{
    MS_BT_STACK_NOTIFY_INFO *pInfo = pNotifyInfo->pContext;
    
    if (NULL == pInfo) {
        return;   
    }
    
    pInfo->ShutDown = TRUE;
    pNotifyInfo->pContext = NULL;
    
#ifndef BTE_CLASS_AVDTP

        /* clean up A2DP helper */        
    if (pInfo->hBtA2DPProcThread != NULL) {
       SetEvent(pInfo->hBtA2DPEvent);
       WaitForSingleObject(pInfo->hBtA2DPProcThread,INFINITE);
       CloseHandle(pInfo->hBtA2DPProcThread);
       pInfo->hBtA2DPProcThread = NULL;
    }
    
    if (pInfo->hBtA2DPEvent != NULL) {
        CloseHandle(pInfo->hBtA2DPEvent);
        pInfo->hBtA2DPEvent = NULL;    
    }
        
#endif

#ifdef REGISTER_MS_BTSTACK_NOTIFICATIONS     

    if (pInfo->hBtNotification != NULL) {
        StopBluetoothNotifications(pInfo->hBtNotification);
        pInfo->hBtNotification = NULL;
    }
    
    if (pInfo->hBtNotifyMsgQueue != NULL) {
        CloseMsgQueue(pInfo->hBtNotifyMsgQueue);
        pInfo->hBtNotifyMsgQueue = NULL;
    }

#endif
    
    AthBtFilterDisconnectHCI(pInfo);
    DeleteCriticalSection(&pInfo->Lock);
    LocalFree(pInfo);
}

BOOL AthBtStackNotificationsInit(ATH_BT_STACK_NOTIFY_INFO  *pNotifyInfo)
{
    MS_BT_STACK_NOTIFY_INFO *pInfo = NULL;
    BOOL                    retVal = FALSE;
    MSGQUEUEOPTIONS         msgOptions;   
        
    do {
        
        pInfo = (MS_BT_STACK_NOTIFY_INFO *)LocalAlloc(LPTR, sizeof(MS_BT_STACK_NOTIFY_INFO));
        
        if (NULL == pInfo) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to allocate BT stack notify info \r\n")));
            break;    
        }       
        
        pInfo->pNotifyInfo = pNotifyInfo;
        pNotifyInfo->pContext = pInfo;
        InitializeCriticalSection(&pInfo->Lock);
        
        memset(&msgOptions, 0, sizeof(msgOptions));
        
#ifdef REGISTER_MS_BTSTACK_NOTIFICATIONS 
                      
#if (BT_EVENT_CLASSES_TO_REGISTER > 0)

        memset(&msgOptions, 0, sizeof(msgOptions));
        msgOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
        msgOptions.dwMaxMessages = MAX_BT_NOTIFY_MSGS;
        msgOptions.cbMaxMessage =  sizeof(BTEVENT);
        msgOptions.bReadAccess = TRUE;
        
        pInfo->hBtNotifyMsgQueue = CreateMsgQueue(NULL, &msgOptions);

        if (NULL == pInfo->hBtNotifyMsgQueue) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to create message BT notification msg queue \r\n"))); 
            break;
        }
        
        pInfo->hBtNotification = RequestBluetoothNotifications(BT_EVENT_CLASSES_TO_REGISTER, pInfo->hBtNotifyMsgQueue);
        
        if (NULL == pInfo->hBtNotification) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to register for notifications (%d) \r\n"), GetLastError())); 
            break;   
            
        }   
                
            /* set callback info */
        pNotifyInfo->hBtNotify = pInfo->hBtNotifyMsgQueue; /* event queue will get signalled */
        pNotifyInfo->Callback = ProcessMSBTStackNotification;
           
#endif       
           
#endif

#ifndef BTE_CLASS_AVDTP
        /* set up A2DP helper */
        
        pInfo->hBtA2DPEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

        if (NULL == pInfo->hBtA2DPEvent) {
            break;    
        }
        
        {
            DWORD threadId;
            
            pInfo->hBtA2DPProcThread = CreateThread(NULL,
                                                    0,
                                                    A2DPIndicationThread,
                                                    pInfo,
                                                    0,
                                                    &threadId);
        }
                                                
        if (NULL == pInfo->hBtA2DPProcThread) {
            break;    
        }

#endif
   
   
        if (!AthBtFilterConnectHCI(pInfo)) {
            break;    
        }
        
        retVal = TRUE;
        
    } while (FALSE);
    
    if (!retVal) {
        AthBtStackNotificationsShutdown(pNotifyInfo);    
    }
    
    return retVal;
}

DWORD AthBtStackGetSupportedIndicationsMask(ATH_BT_STACK_NOTIFY_INFO  *pCallbackInfo)
{
    DWORD mask = 0;
    
#ifdef REGISTER_MS_BTSTACK_NOTIFICATIONS
    
#ifdef BTE_CLASS_AVDTP
    mask |= MAKE_BTSTATE_MASK(ATH_BT_A2DP);
#endif
   
#endif 
        /* the notification APIs support these state detection mechanisms, the HCI packet filter
         * can ignore these states */
    return mask;
}

#define INVALID_BTCONN_HANDLE 0xFFFF

USHORT FindACLConnectionHandle(BT_ADDR   *BTAddress)
{
    BASEBAND_CONNECTION connections[8];
    int                 numConnections, i;
    int                 status;
    
    status = BthGetBasebandConnections(8,connections,&numConnections);
  
    if (status != ERROR_SUCCESS) {
        ATH_LOG_MSG(ZONE_ERROR,
                    (TEXT("ATHBT: BthGetBasebandConnections failed : status =%d \r\n"), 
                    status));     
        return INVALID_BTCONN_HANDLE;    
    }
    
    for (i = 0; i < numConnections; i++) {
        if (connections[i].fLinkType) {
                /* only filter connections that are ACL */
            if(memcmp(&connections[i].baAddress,BTAddress,sizeof(BT_ADDR)) == 0) {
                return connections[i].hConnection;    
            }
        }    
    }
    
    return INVALID_BTCONN_HANDLE;
}

#define LMP_FEATURE_ACL_EDR_2MBPS_BYTE_INDEX  3
#define LMP_FEATURE_ACL_EDR_2MBPS_BIT_MASK    0x2
#define LMP_FEATURE_ACL_EDR_3MBPS_BYTE_INDEX  3
#define LMP_FEATURE_ACL_EDR_3MBPS_BIT_MASK    0x4

void UpdateBtAudioConnectionProperties(MS_BT_STACK_NOTIFY_INFO  *pInfo,
                                       BT_ADDR                  *BTAddress,
                                       USHORT                   hConnection,
                                       ATHBT_STATE_INDICATION   Indication)
{
    USHORT           role = 0;
    LONG             winError;
    A_UINT8          lmp_version = 0;
    A_UINT16         lmp_subversion = 0;
    A_UINT16         manufacturer = 0;
    A_UINT8          lmp_feature[8];
    TCHAR            *pDescr = NULL;
   
    do {
                
        winError = BthReadRemoteVersion(BTAddress,
                                        &lmp_version,
                                        &lmp_subversion,
                                        &manufacturer,
                                        lmp_feature);  
                                        
        if (winError != ERROR_SUCCESS) {
            ATH_LOG_MSG(ZONE_ERROR,
                    (TEXT("ATHBT: Failed to get remote version from btAddress : status =%d \r\n"), 
                    winError));     
            break;     
        }

        if (ZONE_BT_NOTIFY) {
            AthFilterDumpBuffer(lmp_feature, 
                                sizeof(lmp_feature), 
                                TEXT("Remote Device LMP Features"));    
        }
        
        if (lmp_version == 3) {
            /* some 2.0 BT devices are non-EDR, check the lmp_features to 
             * make the correct assessment */    
            if ((lmp_feature[LMP_FEATURE_ACL_EDR_2MBPS_BYTE_INDEX] & LMP_FEATURE_ACL_EDR_2MBPS_BIT_MASK)  ||    
                (lmp_feature[LMP_FEATURE_ACL_EDR_3MBPS_BYTE_INDEX] & LMP_FEATURE_ACL_EDR_3MBPS_BIT_MASK)) {
                ATH_LOG_MSG(ZONE_BT_NOTIFY,
                    (TEXT("ATHBT: Remote device is 2.0 + EDR \r\n")));             
            } else {
                if (Indication == ATH_BT_A2DP) {
                    ATH_LOG_MSG(ZONE_BT_NOTIFY,
                        (TEXT("ATHBT: Remote device is 2.0 non-EDR, forcing 1.2 coex protection \r\n")));             
                    /* for the purposes of A2DP BT-coexistence, non-EDR for ACL connections
                     * is treated like a 1.2 radio */
                    lmp_version = 2;
                }      
            }
        }        
            
#ifdef BTH_GET_ROLE_USE_EXT

        if (INVALID_BTCONN_HANDLE == hConnection) {
                /* we need to look up the connection handle */
            hConnection = FindACLConnectionHandle(BTAddress);
            if (hConnection == INVALID_BTCONN_HANDLE) {
                winError = ERROR_GEN_FAILURE;
                break;    
            }     
        }
        
            /* need to use HCI extension to get the role */
        winError = GetHCIRole(pInfo, hConnection,&role);
#else              
            /* use built-in API */      
        winError = BthGetRole(BTAddress,&role);
#endif    
        if (winError != ERROR_SUCCESS) {
            ATH_LOG_MSG(ZONE_ERROR,
                    (TEXT("ATHBT: Failed to get role from btAddress : status =%d \r\n"), 
                    winError)); 
            break;         
        }
        

        if (Indication == ATH_BT_A2DP) {
            pDescr = TEXT("A2DP");  
            pInfo->pNotifyInfo->A2DPConnection_LMPVersion = lmp_version;
            pInfo->pNotifyInfo->A2DPConnection_Role = (UCHAR)role;            
        } else if ((Indication == ATH_BT_SCO) || (Indication == ATH_BT_ESCO)) {
            if (Indication == ATH_BT_SCO) {
                pDescr = TEXT("SCO");  
            } else {
                pDescr = TEXT("eSCO");
            }
            pInfo->pNotifyInfo->SCOConnection_LMPVersion = lmp_version;
            pInfo->pNotifyInfo->SCOConnection_Role = (UCHAR)role;
        } else {
            pDescr = TEXT("UNKNOWN!!");    
        }
                
        ATH_LOG_MSG(ZONE_BT_NOTIFY,
            (TEXT("ATHBT: BT Audio (%s) running (role: %s, lmp version: %d) \r\n"), 
               pDescr, role ? TEXT("SLAVE") : TEXT("MASTER"), lmp_version));
                
    } while (FALSE);
                                     
}


#ifdef REGISTER_MS_BTSTACK_NOTIFICATIONS

static void ProcessMSBTStackNotification(ATH_BT_STACK_NOTIFY_INFO  *pNotifyInfo)
{
    MS_BT_STACK_NOTIFY_INFO     *pInfo = (MS_BT_STACK_NOTIFY_INFO *)pNotifyInfo->pContext;
    BTEVENT                     event;
    DWORD                       bytesRead;
    DWORD                       flags;
    ATHBT_STATE_INDICATION      stateIndication = ATH_BT_NOOP;
    ATHBT_STATE                 state = STATE_OFF;
    
    while (1) {
        
        bytesRead = 0;
        
        if (!ReadMsgQueue(pInfo->hBtNotifyMsgQueue,
                          &event,
                          sizeof(event),
                          &bytesRead,
                          0,
                          &flags)) {
            LONG lastError = GetLastError();
            if (lastError != ERROR_TIMEOUT) {                  
                ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: ReadMsgQueue (BT) failed (%d) \r\n"),lastError));
            }
            break;                    
        }  
        
        if (0 == bytesRead) {
            break;    
        }        
        
        if (bytesRead < sizeof(event.dwEventId)) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: bytes from BT notification message is too small :%d \r\n"), bytesRead));    
            continue;
        }
        
        ATH_LOG_MSG(ZONE_BT_NOTIFY,(TEXT("ATHBT: BT Event ID: %d\r\n"), event.dwEventId));
        
        stateIndication = ATH_BT_NOOP;
        
        switch (event.dwEventId) {
#ifdef BTE_CLASS_AVDTP                 
            case BTE_AVDTP_STATE:  
                {
                    BT_AVDTP_STATE_CHANGE *pStateChange = (BT_AVDTP_STATE_CHANGE *)event.baEventData;
                                     
                    if (ZONE_BT_NOTIFY_DUMP) {
                        AthFilterDumpBuffer((A_UCHAR *)pStateChange, 
                                            sizeof(BT_AVDTP_STATE_CHANGE), 
                                            TEXT("BT A2VDTP Notification"));    
                    }
                    
                    if (pStateChange->dwState == BT_AVDTP_STATE_STREAMING) {
                        stateIndication = ATH_BT_A2DP;
                        state = STATE_ON;                         
                        UpdateBtAudioConnectionProperties(pInfo,
                                                          &pStateChange->bta,
                                                          0xFFFF, /* let API find connection handle */
                                                          ATH_BT_A2DP);
                        ATH_LOG_MSG(ZONE_BT_NOTIFY,(TEXT("ATHBT: A2DP stream start \r\n")));                                          
                    } else if (pStateChange->dwState == BT_AVDTP_STATE_SUSPENDED) {
                        stateIndication = ATH_BT_A2DP;    
                        state = STATE_OFF;                    
                        ATH_LOG_MSG(ZONE_BT_NOTIFY,(TEXT("ATHBT: A2DP stream suspend \r\n")));
                    } else if (pStateChange->dwState == BT_AVDTP_STATE_DISCONNECTED) {
                        stateIndication = ATH_BT_A2DP;   
                        state = STATE_OFF; 
                        ATH_LOG_MSG(ZONE_BT_NOTIFY,(TEXT("ATHBT: A2DP stream disconnected \r\n")));
                    }
                                        
                }
                break; 
#endif           

            case BTE_CONNECTION:
                {
                    BT_CONNECT_EVENT *pConnectEvt = (BT_CONNECT_EVENT *)event.baEventData;
                  
                    
                    if (ZONE_BT_NOTIFY_DUMP) {
                        AthFilterDumpBuffer((A_UCHAR *)pConnectEvt, 
                                            sizeof(BT_CONNECT_EVENT), 
                                            TEXT("BT Connect Notification"));    
                    }

                    ATH_LOG_MSG(ZONE_BT_NOTIFY,(TEXT("ATHBT: Connection Handle:0x%4.4X LinkType: %d\r\n"), 
                        pConnectEvt->hConnection, pConnectEvt->ucLinkType));
                    
                    if ((pConnectEvt->ucLinkType == 0x00) || (pConnectEvt->ucLinkType == 0x02)) {                        
                        UpdateBtAudioConnectionProperties(pInfo,
                                                          &pConnectEvt->bta,
                                                          0xFFFF, /* let API find connection handle */
                                                          (pConnectEvt->ucLinkType == 0x00) ? 
                                                           ATH_BT_SCO : ATH_BT_ESCO);                                                      

                    }
                             
                }
                break;
                
            default:
                break;          
        }
        
        if (stateIndication != ATH_BT_NOOP) {
                /* indicate the state directly using the set callback */
            pNotifyInfo->pStateIndicationFn(pNotifyInfo->pStateIndicationContext,
                                            stateIndication,
                                            state,
                                            0);  /* pass 0 for lmp_version, we pass this via the notify struct */   
        }
        
    }
        
}
#endif

ATHBT_STATE_INDICATION  AthBtStackNotifyA2DPIndicationFromFilterCore(ATH_BT_STACK_NOTIFY_INFO  *pNotifyInfo,
                                                                     ATHBT_STATE               State,
                                                                     A_UINT16                  BtConnHandle)
{
    ATHBT_STATE_INDICATION  indication = ATH_BT_A2DP;
    MS_BT_STACK_NOTIFY_INFO *pInfo = (MS_BT_STACK_NOTIFY_INFO *)pNotifyInfo->pContext;
                    
#ifndef BTE_CLASS_AVDTP

    /* this version of the MS stack does not support A2DP state detection, so we let the filter
     * core detect this for us. This code receives the indication and if the state is ON, we signal 
     * our remote version request thread to process */

    if (State == STATE_ON) {
        /* A2DP was detected by the filter core routines, return a NOOP because the thread
         * that gets the remote version information will make the indication for us */
        pInfo->A2DPAclConnHandle = BtConnHandle;  
        SetEvent(pInfo->hBtA2DPEvent);
        indication = ATH_BT_NOOP;
    }
      
#endif
    
    return indication;
}


#ifndef BTE_CLASS_AVDTP

    /* helper thread to make A2DP indications */
    
static DWORD  A2DPIndicationThread(DWORD *pContext)
{
    MS_BT_STACK_NOTIFY_INFO  *pInfo = (MS_BT_STACK_NOTIFY_INFO *)pContext;
    BT_ADDR                  btAddress;
    LONG                     winError;
    
    while (1) {
        
        WaitForSingleObject(pInfo->hBtA2DPEvent, INFINITE);
        
        if (pInfo->ShutDown) {
            break;    
        }
       
    
            /* look up the address based on the connection handle */
        winError = BthGetAddress(pInfo->A2DPAclConnHandle,
                                 &btAddress);
    
        if (winError == ERROR_SUCCESS) {
            UpdateBtAudioConnectionProperties(pInfo,
                                              &btAddress,
                                              pInfo->A2DPAclConnHandle,
                                              ATH_BT_A2DP);  
        } else {
            
            ATH_LOG_MSG(ZONE_ERROR,
                (TEXT("ATHBT: Failed to look up BT address from handle : (0x%04X) : status =%d \r\n"), 
                pInfo->A2DPAclConnHandle, winError));    
           
        }
        
        ATH_LOG_MSG(ZONE_BT_NOTIFY,
                (TEXT("ATHBT: helper thread : A2DP stream ON \r\n")));
        

        pInfo->pNotifyInfo->pStateIndicationFn(pInfo->pNotifyInfo->pStateIndicationContext,
                                               ATH_BT_A2DP,
                                               STATE_ON,
                                               0); /* pass 0 for lmp_version, we pass this via the notify struct */ 
                                                                                                               
    }   
    
    return 0;
}

#endif





