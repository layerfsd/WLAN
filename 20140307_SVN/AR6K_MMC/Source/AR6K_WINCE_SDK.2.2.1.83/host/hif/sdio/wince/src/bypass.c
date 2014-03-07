//------------------------------------------------------------------------------
// <copyright file="bypass.c" company="Atheros">
//    Copyright (c) 2008 Atheros Corporation.  All rights reserved.
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
// This file contains the routines handling the SDIO stack bypass mode on WinCE
//
// Author(s): ="Atheros"
//==============================================================================
#include "athdefs.h"
#include "a_types.h"
#include "a_osapi.h"
#include "a_debug.h"
#include <sdcardddk.h>
#include <sdhcd.h>
#include "ar6000_sdio.h"
#include "ar6k_sdio_bypass.h"

#define MAX_BUS_REQUESTS      64
#define MAX_BUS_WAIT_OBJECTS  8

typedef struct _BUS_REQ_WAIT_OBJECT {
    struct _BUS_REQ_WAIT_OBJECT *pNext;
    HANDLE                      hWaitObject;
    SD_API_STATUS               BusCompletionStatus;
} BUS_REQ_WAIT_OBJECT;

typedef struct _SDIO_BYPASS_INFO {
    A_BOOL              InitCalled;
    TCHAR               HCDDeviceName[16];
    HANDLE              hHCDStreamDev;          /* stream interface handle to HCD */
    SD_DEVICE_HANDLE    hSDDevice;
    CRITICAL_SECTION    Lock;
    A_BOOL              BypassEnabled;
    A_BOOL              ShutDown;
    PSDCARD_HC_CONTEXT  pHcd;                   /* host controller struct */
    DWORD               SlotNumber;             /* slot number that bypass is active on */
    LIST_ENTRY          FreeBusRequestList;     /* free list */
    LIST_ENTRY          HcdBusRequestList;      /* active request queue */
    LIST_ENTRY          CompletedBusRequestList; /* completed request queue (for in-order completion) */
    BUS_REQ_WAIT_OBJECT *pWaitObjectList;       /* wait object fee list */
    HANDLE              hSDIOIRQThread;         /* Synchronous IRQ processing thread */
    HANDLE              hWakeSDIOIRQThread;
    SD_BUS_REQUEST      *pCurrentRequest;       /* current request hcd is working on */
    PSD_INTERRUPT_CALLBACK pIRQFunc;
    AR6K_SDIO_BYPASS_INFO_INPUT   BypassSettingIn;
    AR6K_SDIO_BYPASS_INFO_OUTPUT  BypassSettingOut;
        /* requests and wait objects */
    SD_BUS_REQUEST      _busRequests[MAX_BUS_REQUESTS];
    BUS_REQ_WAIT_OBJECT _busWaitObjects[MAX_BUS_WAIT_OBJECTS];
    DWORD               BypassEnableIoctl;
    DWORD               BypassDisableIoctl;
}SDIO_BYPASS_INFO;


#ifdef HIF_SDIO_BYPASS

/* this code runs LEAN AND MEAN, use a global info structure */
SDIO_BYPASS_INFO g_BypassInfo;

#define LOCK()   EnterCriticalSection(&g_BypassInfo.Lock);
#define UNLOCK() LeaveCriticalSection(&g_BypassInfo.Lock);

static void CompleteAndStartNextRequest(SD_BUS_REQUEST *pRequestToComplete, A_BOOL Recursive);
static DWORD IRQProcThreadFunc(LPVOID pContext);

static _inline SD_BUS_REQUEST *DequeueBusRequest(LIST_ENTRY *pHead) {
    LIST_ENTRY *pItem;

    if (IsListEmpty(pHead)) {
        return NULL;
    }
    pItem = RemoveHeadList(pHead);
    return CONTAINING_RECORD(pItem, SD_BUS_REQUEST, ListEntry);
}

#define EnqueueBusRequest(pHead, pRequest)  \
       InsertTailList(pHead, &(pRequest)->ListEntry)


#define AllocateBusRequest()      DequeueBusRequest(&g_BypassInfo.FreeBusRequestList)
#define FreeBusRequest(pRequest)  EnqueueBusRequest(&g_BypassInfo.FreeBusRequestList, pRequest)

static void FreeWaitObject(BUS_REQ_WAIT_OBJECT *pWaitObj)
{
    if (g_BypassInfo.pWaitObjectList == NULL) {
        pWaitObj->pNext = NULL;
        g_BypassInfo.pWaitObjectList = pWaitObj;
    } else {
        pWaitObj->pNext = g_BypassInfo.pWaitObjectList;
        g_BypassInfo.pWaitObjectList = pWaitObj;
    }
}

static BUS_REQ_WAIT_OBJECT *AllocateBusWaitObject()
{
    if (g_BypassInfo.pWaitObjectList != NULL) {
        BUS_REQ_WAIT_OBJECT *pWaitObj;
        pWaitObj = g_BypassInfo.pWaitObjectList;
        g_BypassInfo.pWaitObjectList = pWaitObj->pNext;
        pWaitObj->pNext = NULL;
        return pWaitObj;
    }
    return NULL;
}

static void FlushQueue(LIST_ENTRY *pHead)
{
    SD_BUS_REQUEST  *pBusRequest;

    while (1) {
        pBusRequest = DequeueBusRequest(pHead);

        if (pBusRequest == NULL) {
            break;
        }

        pBusRequest->pCallback(g_BypassInfo.hSDDevice,
                               (SD_BUS_REQUEST *)SD_API_STATUS_CANCELED,
                               NULL,
                               pBusRequest->RequestParam);

        FreeBusRequest(pBusRequest);
    }
}

    /* handle slot event notifications */
static VOID IndicateSlotChange(PSDCARD_HC_CONTEXT pHcd,
                               DWORD              SlotNumber,
                               SD_SLOT_EVENT      SlotEvent)
{

    if (SlotEvent == BusRequestComplete) {
        /* this event was deprecated, no host controller should be using it */
        A_ASSERT(FALSE);
    } else if (SlotEvent == DeviceInterrupting) {
            /* wake up IRQ processing thread */
        SetEvent(g_BypassInfo.hWakeSDIOIRQThread);
    }  else {
        if (SlotEvent == DeviceEjected) {
            g_BypassInfo.BypassEnabled = FALSE;
        }
            /* pass it up to the real bus driver */
        g_BypassInfo.BypassSettingOut.SDBusApis.IndicateSlotStateChange(pHcd, SlotNumber, SlotEvent);

    }

}

    /* handle SDIO bus request complete indication from HCD when an
     * asynchronous request is completed */
static VOID  IndicateBusRequestComplete(PSDCARD_HC_CONTEXT pHcd,
                                        PSD_BUS_REQUEST    pRequest,
                                        SD_API_STATUS      Status)
{
    A_ASSERT(pRequest == g_BypassInfo.pCurrentRequest);
    pRequest->Status = Status;
        /* hcd is not longer busy */
    g_BypassInfo.pCurrentRequest = NULL;
        /* complete and start next */
    CompleteAndStartNextRequest(pRequest, FALSE);
}

static VOID UnlockRequest(PSDCARD_HC_CONTEXT  pHcd,
                          PSD_BUS_REQUEST     pRequest)
{
   /* nothing to do here, since none of the requests are cancelable */
}

static PSD_BUS_REQUEST GetAndLockCurrentRequest(PSDCARD_HC_CONTEXT pHcd,
                                                DWORD              SlotIndex)
{
    return g_BypassInfo.pCurrentRequest;
}

A_BOOL g_InitCalled = FALSE;

static A_BOOL EnableDisableBypassMode(SDIO_BYPASS_INFO *pInfo, A_BOOL Enable)
{
    A_BOOL success = FALSE;
    DWORD  returnBytes;

    do {

        if (pInfo->hHCDStreamDev == INVALID_HANDLE_VALUE) {
            break;
        }

        if (Enable) {

            A_MEMZERO(&pInfo->BypassSettingIn, sizeof(pInfo->BypassSettingIn));
            A_MEMZERO(&pInfo->BypassSettingOut, sizeof(pInfo->BypassSettingOut));

            pInfo->BypassSettingIn.SlotNumber = pInfo->SlotNumber;
            pInfo->BypassSettingIn.ByPassFuncs.IndicateSlotStateChange = IndicateSlotChange;
            pInfo->BypassSettingIn.ByPassFuncs.IndicateBusRequestComplete = IndicateBusRequestComplete;
            pInfo->BypassSettingIn.ByPassFuncs.UnlockRequest = UnlockRequest;
            pInfo->BypassSettingIn.ByPassFuncs.GetAndLockCurrentRequest = GetAndLockCurrentRequest;

            if (!DeviceIoControl(pInfo->hHCDStreamDev,
                                 pInfo->BypassEnableIoctl,
                                 &pInfo->BypassSettingIn,
                                 sizeof(pInfo->BypassSettingIn),
                                 &pInfo->BypassSettingOut,
                                 sizeof(pInfo->BypassSettingOut),
                                 &returnBytes,
                                 NULL)) {
                AR_DEBUG_PRINTF(ATH_LOG_ERR, ("  Failed to enable HCD bypass mode \n"));
                break;
            }

            A_ASSERT(pInfo->BypassSettingOut.pHcd != NULL);
            A_ASSERT(pInfo->BypassSettingOut.SDBusApis.IndicateSlotStateChange != NULL);
            pInfo->pHcd = pInfo->BypassSettingOut.pHcd;
            pInfo->BypassEnabled = TRUE;

        } else {

            pInfo->BypassEnabled = FALSE;
            if (!DeviceIoControl(pInfo->hHCDStreamDev,
                                 pInfo->BypassDisableIoctl,
                                 &pInfo->SlotNumber,
                                 sizeof(pInfo->SlotNumber),
                                 NULL,
                                 0,
                                 &returnBytes,
                                 NULL)) {
                AR_DEBUG_PRINTF(ATH_LOG_ERR, ("  Failed to disable HCD bypass mode \n"));
                break;
            }

        }

        success = TRUE;

    } while (FALSE);

    return success;
}

void CleanupSDIOStackBypassMode()
{
    SDIO_BYPASS_INFO *pInfo = &g_BypassInfo;
    int     i;

    if (!g_InitCalled) {
        return;
    }

    pInfo->BypassEnabled = FALSE;
    pInfo->ShutDown = TRUE;

    if (pInfo->hWakeSDIOIRQThread != NULL) {
        SetEvent(pInfo->hWakeSDIOIRQThread);
    }

    if (pInfo->hSDIOIRQThread != NULL) {
        WaitForSingleObject(pInfo->hSDIOIRQThread, INFINITE);
        CloseHandle(pInfo->hSDIOIRQThread);
        pInfo->hSDIOIRQThread = NULL;
    }

    if (pInfo->hWakeSDIOIRQThread != NULL) {
        CloseHandle(pInfo->hWakeSDIOIRQThread);
        pInfo->hWakeSDIOIRQThread = NULL;
    }

        /* wait for any active requests */
    while (1) {
        Sleep(100);
        if (pInfo->pCurrentRequest != NULL) {
            continue;
        }
        break;
    }

    EnableDisableBypassMode(pInfo, FALSE);

        /* flush out any requests stuck in queues */
    FlushQueue(&pInfo->CompletedBusRequestList);
    FlushQueue(&pInfo->HcdBusRequestList);

    if (pInfo->hHCDStreamDev != INVALID_HANDLE_VALUE) {
        CloseHandle(pInfo->hHCDStreamDev);
        pInfo->hHCDStreamDev = INVALID_HANDLE_VALUE;
    }

    for (i = 0; i < MAX_BUS_WAIT_OBJECTS; i++) {
        if (pInfo->_busWaitObjects[i].hWaitObject != NULL) {
            CloseHandle(pInfo->_busWaitObjects[i].hWaitObject);
            pInfo->_busWaitObjects[i].hWaitObject = NULL;
            break;
        }
    }

    DeleteCriticalSection(&pInfo->Lock);
    g_InitCalled = FALSE;
}

void InitSDIOStackBypassMode(PTCHAR pRegPath, SD_DEVICE_HANDLE hSDDevice)
{
    SDIO_BYPASS_INFO    *pInfo = &g_BypassInfo;
    DWORD               threadId;
    int                 i;

    if (g_InitCalled) {
        return;
    }

    g_InitCalled = TRUE;

    A_MEMZERO(pInfo,sizeof(*pInfo));

    pInfo->hHCDStreamDev = INVALID_HANDLE_VALUE;
    InitializeListHead(&pInfo->FreeBusRequestList);
    InitializeListHead(&pInfo->HcdBusRequestList);
    InitializeListHead(&pInfo->CompletedBusRequestList);
    InitializeCriticalSection(&pInfo->Lock);
    pInfo->SlotNumber = 0xFFFFFFFF;
    _tcscpy(pInfo->HCDDeviceName, TEXT("INVALID"));
    
    pInfo->BypassEnableIoctl =  IOCTL_AR6K_SDIO_BYPASS_ENABLE;
    pInfo->BypassDisableIoctl = IOCTL_AR6K_SDIO_BYPASS_DISABLE;
    
    GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                        HIF_SDIO_PARAMS_REG_PATH,
                        HIF_BYPASS_HCD_NAME_KEY_VAL,
                        (PUCHAR)pInfo->HCDDeviceName,
                        sizeof(pInfo->HCDDeviceName));

    GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                        HIF_SDIO_PARAMS_REG_PATH,
                        HIF_BYPASS_HCD_SLOT_KEY_VAL,
                        (PUCHAR)&pInfo->SlotNumber,
                        sizeof(pInfo->SlotNumber));
    
    GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                        HIF_SDIO_PARAMS_REG_PATH,
                        HIF_BYPASS_HCD_ENABLE_IOCTL_KEY_VAL,
                        (PUCHAR)&pInfo->BypassEnableIoctl,
                        sizeof(pInfo->BypassEnableIoctl));
    
    GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                        HIF_SDIO_PARAMS_REG_PATH,
                        HIF_BYPASS_HCD_DISABLE_IOCTL_KEY_VAL,
                        (PUCHAR)&pInfo->BypassDisableIoctl,
                        sizeof(pInfo->BypassDisableIoctl));                   
                        
    do {

        if (_tcscmp(pInfo->HCDDeviceName, TEXT("INVALID")) == 0) {
                /* no valid device name */
            break;
        }

        if (pInfo->SlotNumber == 0xFFFFFFFF) {
                /* no valid slot number */
            break;
        }

            /* open handle to host controller device */
        pInfo->hHCDStreamDev = CreateFile(pInfo->HCDDeviceName,
                                          GENERIC_READ | GENERIC_WRITE,
                                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                                          NULL,
                                          OPEN_EXISTING,
                                          FILE_ATTRIBUTE_NORMAL,
                                          0);

        if (pInfo->hHCDStreamDev == INVALID_HANDLE_VALUE) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR, ("  Failed to open HCD bypass device  \n"));
            break;
        }


        /* build bus request list */
        for (i = 0; i < MAX_BUS_REQUESTS; i++) {
            FreeBusRequest(&pInfo->_busRequests[i]);
        }


        /* build wait object list */
        for (i = 0; i < MAX_BUS_WAIT_OBJECTS; i++) {
            pInfo->_busWaitObjects[i].hWaitObject = CreateEvent(NULL,FALSE,FALSE,NULL);
            if (NULL == pInfo->_busWaitObjects[i].hWaitObject) {
                A_ASSERT(FALSE);
                break;
            }
            FreeWaitObject(&pInfo->_busWaitObjects[i]);
        }

        pInfo->hSDDevice = hSDDevice;

        pInfo->hWakeSDIOIRQThread = CreateEvent(NULL, FALSE, FALSE, NULL);

        if (NULL == pInfo->hWakeSDIOIRQThread) {
            break;
        }

        pInfo->hSDIOIRQThread = CreateThread(NULL,
                                             0,
                                             IRQProcThreadFunc,
                                             NULL,
                                             0,
                                             &threadId);

        if (NULL == pInfo->hSDIOIRQThread) {
            break;
        }

    } while (FALSE);
}

A_BOOL ActivateSDIOStackBypassMode()
{
    return EnableDisableBypassMode(&g_BypassInfo, TRUE);
}

A_BOOL DeactivateSDIOStackBypassMode()
{
    return EnableDisableBypassMode(&g_BypassInfo, FALSE);
}


    /* complete and start next request */
static void CompleteAndStartNextRequest(SD_BUS_REQUEST *pRequestToComplete, A_BOOL Recursive)
{
    SD_BUS_REQUEST      *pBusRequest = NULL;
    SD_API_STATUS       sdStatus;

    LOCK()

    if (pRequestToComplete != NULL) {
            /* queue the completed request because this function is recursive and
             * can start the next request (and complete it).  The completion queue guarantees in-order
             * I/O completion */
        EnqueueBusRequest(&g_BypassInfo.CompletedBusRequestList,pRequestToComplete);
    }

    if (g_BypassInfo.pCurrentRequest == NULL) {
        /* HCD is not busy working on a request, see if there is anything
         * in the queue */
        pBusRequest = DequeueBusRequest(&g_BypassInfo.HcdBusRequestList);
        if (pBusRequest != NULL) {
                /* mark queue busy */
            g_BypassInfo.pCurrentRequest = pBusRequest;
        }
    }

    UNLOCK();

    if (pBusRequest != NULL) {
            /* there was a new request to start */
        sdStatus = g_BypassInfo.pHcd->pBusRequestHandler(g_BypassInfo.pHcd,
                                                         g_BypassInfo.SlotNumber,
                                                         pBusRequest);

        if (sdStatus != SD_API_STATUS_PENDING) {
            pBusRequest->Status = sdStatus;
            g_BypassInfo.pCurrentRequest = NULL;
                /* recursive call */
            CompleteAndStartNextRequest(pBusRequest, TRUE);
        }
    }

    if (pRequestToComplete != NULL) {
        DWORD                    requestParam;
        PSD_BUS_REQUEST_CALLBACK pCallBack;

        /* if this call was the top most call (this function is recursive)
         * then we can now complete any requests that were queued
         * to the completion queue */

        LOCK();

        while ((pBusRequest = DequeueBusRequest(&g_BypassInfo.CompletedBusRequestList)) != NULL) {

            sdStatus = pBusRequest->Status;
            requestParam = pBusRequest->RequestParam;
            pCallBack = pBusRequest->pCallback;

            A_ASSERT(pCallBack != NULL);

                /* return this bus request to the list while we have the lock */
            FreeBusRequest(pBusRequest);

            UNLOCK();

                /* call completion callback */
            pCallBack(g_BypassInfo.hSDDevice,
                      (SD_BUS_REQUEST*)sdStatus, /* the request is actually the status */
                      NULL,
                      requestParam);
            LOCK();
        }

        UNLOCK();
    }

}


    /* this is the sync I/O completion handler that is used when the caller
     * wants the bus request to have synchronous completion  */
static void SyncIOCompletionHandler(SD_DEVICE_HANDLE  hDevice,
                                    PSD_BUS_REQUEST   pRequest,
                                    PVOID             notUsed,
                                    DWORD             dwParam)
{
    BUS_REQ_WAIT_OBJECT *pWaitObj = (BUS_REQ_WAIT_OBJECT *)dwParam;
    A_ASSERT(pWaitObj != NULL);
    A_ASSERT(pWaitObj->hWaitObject != NULL);

    pWaitObj->BusCompletionStatus = (SD_API_STATUS)pRequest;
        /* wake blocked thread */
    SetEvent(pWaitObj->hWaitObject);
}

SD_API_STATUS SynchRequestBypass(UCHAR                Command,
                                 DWORD                Argument,
                                 SD_TRANSFER_CLASS    TransferType,
                                 SD_RESPONSE_TYPE     ResponseType,
                                 PSD_COMMAND_RESPONSE pReponseBuffer,
                                 ULONG                BlockCount,
                                 ULONG                BlockLength,
                                 PUCHAR               pBuffer)
{
    SD_API_STATUS       sdStatus = SD_API_STATUS_PENDING;
    SD_BUS_REQUEST      *pBusRequest = NULL;
    BUS_REQ_WAIT_OBJECT *pWaitObj = NULL;
    A_BOOL              startRequest = FALSE;

    if (!g_BypassInfo.BypassEnabled) {
        return SD_API_STATUS_UNSUCCESSFUL;
    }

    LOCK();

    do {

        pBusRequest = AllocateBusRequest();

        if (NULL == pBusRequest) {
            sdStatus = SD_API_STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        pWaitObj = AllocateBusWaitObject();

        if (NULL == pWaitObj) {
            sdStatus = SD_API_STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        pBusRequest->CommandCode = Command;
        pBusRequest->CommandArgument = Argument;
        pBusRequest->NumBlocks = BlockCount;
        pBusRequest->BlockSize = BlockLength;
        pBusRequest->TransferClass = TransferType;
        pBusRequest->CommandResponse.ResponseType = ResponseType;
        pBusRequest->pBlockBuffer = pBuffer;
        pBusRequest->pCallback = SyncIOCompletionHandler;
        pBusRequest->RequestParam = (DWORD)pWaitObj;
        pBusRequest->Flags = SD_REQUEST_FLAGS_BYPASS;
        pBusRequest->HCParam = 0;
        pBusRequest->CurrentPermissions = (DWORD)-1;

        /* short data transfer optimization */
        if ((TransferType == SD_READ) && (BlockCount == 1) && (BlockLength <= 32)) {
            pBusRequest->Flags |= SD_REQUEST_FLAGS_SHORT_DATA_TRANSFER;
        }

        if (g_BypassInfo.pCurrentRequest != NULL) {
                /* queue is busy, queue it up */
            EnqueueBusRequest(&g_BypassInfo.HcdBusRequestList,pBusRequest);
            break;
        }

            /* queue was not busy, now mark it busy */
        g_BypassInfo.pCurrentRequest = pBusRequest;
            /* start the request as soon as we drop the lock */
        startRequest = TRUE;

    } while (FALSE);

    UNLOCK();

    if (startRequest) {
            /* we can submit this request right away */
        sdStatus = g_BypassInfo.pHcd->pBusRequestHandler(g_BypassInfo.pHcd,
                                                         g_BypassInfo.SlotNumber,
                                                         pBusRequest);
        if (sdStatus != SD_API_STATUS_PENDING) {
            /* bus request failed or completed successfully without a context switch */
                /* hcd is no longer busy */
            g_BypassInfo.pCurrentRequest = NULL;

            if (SD_API_SUCCESS(sdStatus)) {
                /* TODO... copy response? not really needed  */
            }
        }
    }

    if (sdStatus == SD_API_STATUS_PENDING) {
            /* in an async completion, the bus request object is automatically freed, we
             * can get the completion status through the allocated wait object */
        pBusRequest = NULL;
            /* request is pending, either queued or being worked on by the HCD */
        WaitForSingleObject(pWaitObj->hWaitObject, INFINITE);
        sdStatus = pWaitObj->BusCompletionStatus;
        if (SD_API_SUCCESS(sdStatus)) {
            /* TODO... copy response? not really needed  */
        }
    }

    LOCK();

    if (pBusRequest != NULL) {
        FreeBusRequest(pBusRequest);
    }

    if (pWaitObj != NULL) {
        FreeWaitObject(pWaitObj);
    }

    UNLOCK();

    if (startRequest) {
            /* we started the request in this context so we need to check the queues again */
        CompleteAndStartNextRequest(NULL, FALSE);
    }

    return sdStatus;
}

SD_API_STATUS ASyncRequestBypass(UCHAR                Command,
                                 DWORD                Argument,
                                 SD_TRANSFER_CLASS    TransferType,
                                 SD_RESPONSE_TYPE     ResponseType,
                                 ULONG                BlockCount,
                                 ULONG                BlockLength,
                                 PUCHAR               pBuffer,
                                 PSD_BUS_REQUEST_CALLBACK pCallBack,
                                 DWORD                Context)
{
    SD_API_STATUS   sdStatus = SD_API_STATUS_PENDING;
    SD_BUS_REQUEST  *pBusRequest = NULL;
    A_BOOL          startRequest = FALSE;

    LOCK();

    do {

        if (!g_BypassInfo.BypassEnabled) {
            sdStatus = SD_API_STATUS_UNSUCCESSFUL;
            break;
        }

        pBusRequest = AllocateBusRequest();

        if (NULL == pBusRequest) {
            sdStatus = SD_API_STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        pBusRequest->CommandCode = Command;
        pBusRequest->CommandArgument = Argument;
        pBusRequest->NumBlocks = BlockCount;
        pBusRequest->BlockSize = BlockLength;
        pBusRequest->TransferClass = TransferType;
        pBusRequest->CommandResponse.ResponseType = ResponseType;
        pBusRequest->pBlockBuffer = pBuffer;
        pBusRequest->pCallback = pCallBack;
        pBusRequest->RequestParam = Context;
        pBusRequest->Flags = SD_REQUEST_FLAGS_BYPASS;

        if (g_BypassInfo.pCurrentRequest != NULL) {
                /* queue is busy, queue it up */
            EnqueueBusRequest(&g_BypassInfo.HcdBusRequestList,pBusRequest);
            break;
        }

            /* queue was not busy, now mark it busy */
        g_BypassInfo.pCurrentRequest = pBusRequest;
            /* start the request as soon as we drop the lock */
        startRequest = TRUE;

    } while (FALSE);

    UNLOCK();

    if (startRequest) {
            /* we can submit this request right away */
        sdStatus = g_BypassInfo.pHcd->pBusRequestHandler(g_BypassInfo.pHcd,
                                                         g_BypassInfo.SlotNumber,
                                                         pBusRequest);

    }

    if (sdStatus != SD_API_STATUS_PENDING) {

        if (pBusRequest != NULL) {
            pBusRequest->Status = sdStatus;
                /* bus request failed or completed successfully without a context switch */
                /* we started the request in this context so we need to clear the current request */
            g_BypassInfo.pCurrentRequest = NULL;
                /* complete the current request */
            CompleteAndStartNextRequest(pBusRequest, FALSE);
        } else {
                /* call completion callback */
            pCallBack(g_BypassInfo.hSDDevice,
                      (SD_BUS_REQUEST*)sdStatus, /* the request is actually the status */
                      NULL,
                      Context);

        }
    }

        /* we always return pending */
    return SD_API_STATUS_PENDING;
}

void AckInterruptBypass()
{
        /* ack the interrupt at the HCD */
    g_BypassInfo.pHcd->pSlotOptionHandler(g_BypassInfo.pHcd,
                                          g_BypassInfo.SlotNumber,
                                          SDHCDAckSDIOInterrupt,
                                          NULL,
                                          0);
}


SD_API_STATUS  InterruptEnableDisableBypass(A_BOOL Enable, PSD_INTERRUPT_CALLBACK pIrqFunc)
{
    SD_API_STATUS       sdStatus = SD_API_STATUS_SUCCESS;
    DWORD               argument;
    UCHAR               value;
    SD_COMMAND_RESPONSE resp;
    SD_SLOT_OPTION_CODE slotOption;

    do {

        if (Enable) {
            g_BypassInfo.pIRQFunc = pIrqFunc;
            value = 0x3; /* turn on master interrupt enable and func 1 interrupt */
            slotOption = SDHCDEnableSDIOInterrupts;
        } else {
            g_BypassInfo.pIRQFunc = NULL;
            value = 0;
            slotOption = SDHCDDisableSDIOInterrupts;
        }

        sdStatus =  g_BypassInfo.pHcd->pSlotOptionHandler(g_BypassInfo.pHcd,
                                                          g_BypassInfo.SlotNumber,
                                                          slotOption,
                                                          NULL,
                                                          0);

        if (!SD_API_SUCCESS(sdStatus)) {
            break;
        }

        argument = BUILD_IO_RW_DIRECT_ARG(1,0,0,SD_IO_REG_INT_ENABLE,value);

            /* issue command */
        sdStatus = SynchRequestBypass(52, argument, SD_COMMAND, ResponseR5, &resp, 0, 0, NULL);

    } while (FALSE);

    return sdStatus;
}


static DWORD IRQProcThreadFunc(LPVOID pContext)
{

    CeSetThreadPriority(GetCurrentThread(),100);

    while (!g_BypassInfo.ShutDown) {

        WaitForSingleObject(g_BypassInfo.hWakeSDIOIRQThread, INFINITE);

        if (g_BypassInfo.ShutDown) {
            break;
        }

        if (g_BypassInfo.pIRQFunc != NULL) {
                /* call IRQ handler callback */
            g_BypassInfo.pIRQFunc(g_BypassInfo.hSDDevice, NULL);
        }

    }

    return 0;
}

#endif

