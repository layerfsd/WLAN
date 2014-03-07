/*
 * Bluetooth filter , Microsoft BT stack hci extension implementation
 * 
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
 
#include "a_types.h"
#include "a_osapi.h"
#include "btfilter_wince.h"
#include "btfilter_msbt.h"
#include <svsutil.hxx>
#include <bt_buffer.h>
#include <bt_ddi.h>

#define HCI_EVENT_CAPTURE_BUFFER_SIZE  64  /* to keep memory usage low, we keep the capture buffer
                                              small for the kinds of events we are interested in */
#define HCI_EVENT_CAPTURE_MAX          4
#define HCI_EVENT_CAPTURE_TIMEOUT      5000

typedef struct _HCI_EVENT_CAPTURE_INFO {
    DL_LIST     ListEntry;          
    HANDLE      hCaptureCompleteEvent;                     /* signal to trigger on capture complete */
    UCHAR       EventStatus;                               /* capture status */ 
    UCHAR       EventData[HCI_EVENT_CAPTURE_BUFFER_SIZE];  /* event-specific buffer data */    
} HCI_EVENT_CAPTURE_INFO;

typedef struct _HCI_EXTENSION_INFO {
    HCI_INTERFACE           HCI_Interface; 
    HCI_EVENT_INDICATION    HCI_EventInd;
    HCI_CALLBACKS           HCI_CallBacks;
    int                     HCI_HeaderBytes;
    int                     HCI_TrailerBytes;
    MS_BT_STACK_NOTIFY_INFO *pNotifyInfo;  
    HANDLE                  hHCI;
    DL_LIST                 FreeEventCaptureInfoList;       /* free list of event capture structs */
    HCI_EVENT_CAPTURE_INFO  HCIEventCaptureInfo[HCI_EVENT_CAPTURE_MAX]; /* event capture allocations */         
} HCI_EXTENSION_INFO;

    /* callback from BT stack */
static int AthBtFilter_HCI_RoleDiscovery_Out(void           *pCallContext, 
                                             unsigned char   status, 
                                             unsigned short  connection_handle, 
                                             unsigned char   current_role)
{
    HCI_EVENT_CAPTURE_INFO *pEventCapture = (HCI_EVENT_CAPTURE_INFO *)pCallContext;
 
    ATHBT_ASSERT(pEventCapture != NULL);
    
    ATH_LOG_MSG(ZONE_BT_NOTIFY,
                (TEXT("ATHBT: HCI role discovery complete for handle: 0x%X (%d,%d) \r\n"),
                    connection_handle, status, current_role));    
                    
    pEventCapture->EventStatus = status;
    pEventCapture->EventData[0] = current_role;
    SetEvent(pEventCapture->hCaptureCompleteEvent);
    
    return ERROR_SUCCESS;
}

static void EnqueueEventCapture(DL_LIST *pList, HCI_EVENT_CAPTURE_INFO *pEventCapture)
{
    DL_ListInsertTail(pList,&pEventCapture->ListEntry);
}

static HCI_EVENT_CAPTURE_INFO *DequeueEventCapture(DL_LIST *pList)
{
    HCI_EVENT_CAPTURE_INFO   *pEventCapture = NULL;
    DL_LIST                  *pListEntry;
    
    pListEntry = DL_ListRemoveItemFromHead(pList);
    
    if (pListEntry != NULL) { 
        pEventCapture = A_CONTAINING_STRUCT(pListEntry, HCI_EVENT_CAPTURE_INFO, ListEntry);
    }
    
    return pEventCapture;
}

extern "C" void AthBtFilterDisconnectHCI(MS_BT_STACK_NOTIFY_INFO *pInfo)
{
    HCI_EXTENSION_INFO  *pHCIExt = (HCI_EXTENSION_INFO  *)pInfo->pHCIExtension;
    int                 i;
    
    LOCK_INST(pInfo);
    
    if (pHCIExt == NULL) {
        UNLOCK_INST(pInfo);
        return;    
    }
    
    pInfo->pHCIExtension = NULL;
    
    UNLOCK_INST(pInfo);
    
    if (pHCIExt->hHCI != NULL) {
        HCI_CloseDeviceContext(pHCIExt->hHCI);
        pHCIExt->hHCI = NULL;    
    }
    
    for (i = 0; i < HCI_EVENT_CAPTURE_MAX; i++) {
        if (pHCIExt->HCIEventCaptureInfo[i].hCaptureCompleteEvent != NULL) {
            CloseHandle(pHCIExt->HCIEventCaptureInfo[i].hCaptureCompleteEvent);
            pHCIExt->HCIEventCaptureInfo[i].hCaptureCompleteEvent = NULL;   
        }
    }
        
    LocalFree(pHCIExt);
}

extern "C" BOOL AthBtFilterConnectHCI(MS_BT_STACK_NOTIFY_INFO *pInfo)
{
    int                 status = ERROR_GEN_FAILURE;
    HCI_EXTENSION_INFO  *pHCIExt = NULL;
    int                 i;
    
    do {
        
        pHCIExt = (HCI_EXTENSION_INFO *)LocalAlloc(LPTR, sizeof(HCI_EXTENSION_INFO));

        if (NULL == pHCIExt) {
            status = ERROR_NOT_ENOUGH_MEMORY;
            break;    
        }       
        
        DL_LIST_INIT(&pHCIExt->FreeEventCaptureInfoList); 
        pHCIExt->pNotifyInfo = pInfo;
        pInfo->pHCIExtension = pHCIExt;
        
             /* create free event capture information list */       
        for (i = 0; i < HCI_EVENT_CAPTURE_MAX; i++) {
            pHCIExt->HCIEventCaptureInfo[i].hCaptureCompleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
            if (NULL == pHCIExt->HCIEventCaptureInfo[i].hCaptureCompleteEvent) {
                break;    
            }
            EnqueueEventCapture(&pHCIExt->FreeEventCaptureInfoList,
                                &pHCIExt->HCIEventCaptureInfo[i]);       
        }
        
        if (i != HCI_EVENT_CAPTURE_MAX) {
            status = ERROR_NOT_ENOUGH_MEMORY;
            break;    
        }
        
            /* provide callback for Role discovery commands */
        pHCIExt->HCI_CallBacks.hci_RoleDiscovery_Out = AthBtFilter_HCI_RoleDiscovery_Out;
        
        status = HCI_EstablishDeviceContext(pHCIExt,
                                            0,         /* we don't want events...yet */
                                            NULL,      /* BDADDR */
                                            0,         /* COD */
                                            0,         /* linkType */
                                            &pHCIExt->HCI_EventInd,              
                                            &pHCIExt->HCI_CallBacks,             
                                            &pHCIExt->HCI_Interface,
                                            &pHCIExt->HCI_HeaderBytes,     
                                            &pHCIExt->HCI_TrailerBytes,         
                                            &pHCIExt->hHCI);
        
        if (status != ERROR_SUCCESS) {
            ATH_LOG_MSG(ZONE_ERROR,
                (TEXT("ATHBT: Failed to establish HCI Extension (%d) \r\n"), status));    
        }

    } while (FALSE);
    
    if (status != ERROR_SUCCESS) {
        AthBtFilterDisconnectHCI(pInfo);        
    } else {
        ATH_LOG_MSG(ZONE_INIT,
                (TEXT("ATHBT: HCI Extension established hHCI : 0x%X \r\n"), pHCIExt->hHCI));
    }
    
    return (status == ERROR_SUCCESS) ? TRUE : FALSE;
}


extern "C" LONG GetHCIRole(MS_BT_STACK_NOTIFY_INFO  *pInfo, USHORT ConnectionHandle, USHORT *pRole)
{    
    LONG                    status = ERROR_GEN_FAILURE;
    HCI_EXTENSION_INFO      *pHCIExt = (HCI_EXTENSION_INFO  *)pInfo->pHCIExtension;
    HCI_EVENT_CAPTURE_INFO  *pEventCapture = NULL;
    
    LOCK_INST(pInfo);
    
    do {
        
        if (pHCIExt == NULL) {
            break;    
        }
        
        ATHBT_ASSERT(pHCIExt->hHCI != NULL);
        
        if (pHCIExt->HCI_Interface.hci_RoleDiscovery_In == NULL) {
            ATHBT_ASSERT(FALSE);
            break;    
        }
        
        pEventCapture = DequeueEventCapture(&pHCIExt->FreeEventCaptureInfoList);
    
        if (NULL == pEventCapture) {  
                /* empty */
            status = ERROR_NOT_ENOUGH_MEMORY;
            break;    
        }
        
        ResetEvent(pEventCapture->hCaptureCompleteEvent);
               
    } while (FALSE);
    
    UNLOCK_INST(pInfo);   
    
    
    do {
        
        if (pEventCapture == NULL) {
            break;
        }
            /* issue role discovery command through the extension interface */
        status = pHCIExt->HCI_Interface.hci_RoleDiscovery_In(pHCIExt->hHCI, 
                                                             pEventCapture, 
                                                             ConnectionHandle);
        
        if (status != ERROR_SUCCESS) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to issue HCI_RoleDiscovery_In (%d) \r\n"),
                        status));    
            break;
        }
        
        ATH_LOG_MSG(ZONE_BT_NOTIFY,
                (TEXT("ATHBT: HCI role discovery issued for handle: 0x%X, waiting on event... \r\n"),
                    ConnectionHandle));    
               
        LONG waitStatus;
        
        waitStatus = WaitForSingleObject(pEventCapture->hCaptureCompleteEvent, 
                                         HCI_EVENT_CAPTURE_TIMEOUT);
        
        if (waitStatus != WAIT_OBJECT_0) {
                /* timeout or failure */
            status = ERROR_TIMEOUT;
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT:Wait for HCI_RoleDiscovery_Out failed (%d) \r\n"),
                        waitStatus));    
            break;
        }
        
        if (pEventCapture->EventStatus == 0) {
            *pRole = pEventCapture->EventData[0];
        } else {
            status = ERROR_GEN_FAILURE;
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Role Discovery failed at HCI (evtstatus: %d) \r\n"),
                        pEventCapture->EventStatus));    
        }
                
            
    } while (FALSE);
    
     if (pEventCapture != NULL) {
        LOCK_INST(pInfo);
            /* free item */
        EnqueueEventCapture(&pHCIExt->FreeEventCaptureInfoList, pEventCapture);  
        UNLOCK_INST(pInfo);  
     } 
        
    return status;
}
 


