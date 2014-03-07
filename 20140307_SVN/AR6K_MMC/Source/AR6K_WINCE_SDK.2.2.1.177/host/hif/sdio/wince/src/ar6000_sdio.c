//------------------------------------------------------------------------------
// <copyright file="ar6000_sdio.c" company="Atheros">
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
// Driver entry points for Atheros SDIO based Wifi Card driver
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include <ceddk.h>
#include <ndis.h>
#include <types.h>
#include "SDCard.h"
#include "Sdcardddk.h"
#include "ar6000_sdio.h"
#include "bus_driver.h"
#include "wince_common_drv.h"
#include "athdrv_wince.h"
#include "common_drv.h"


static      SD_FUNCTION         sdFunction;
static      SD_DEVICE_HANDLE    hClientHandle = NULL;
static      HANDLE              wlanHandle    = NULL;

TCHAR g_AR6KDevicePowerName[]   = _T("{98C5250D-C29A-4985-AE5F-AFE5367E5006}\\AR6K_SD1");
TCHAR g_AR6KAdapterInstance[]   = _T("AR6K_SD1");
TCHAR g_AR6KMiniportName[]      = _T("AR6K_SD");
TCHAR g_AR6KDeviceInitEventName[] =_T("SYSTEM\\netui-AR6K_SD1");

SD_API_STATUS
SDIORegisterFunction (SD_FUNCTION *pFunction)
{
    sdFunction = *pFunction;
    if (hClientHandle == NULL) {
        return SD_API_STATUS_UNSUCCESSFUL;
    }

    if (sdFunction.pProbe(hClientHandle) == FALSE) {
        return SD_API_STATUS_UNSUCCESSFUL;
    }

    return SD_API_STATUS_SUCCESS;
}

SD_API_STATUS
SDIOUnregisterFunction(SD_FUNCTION *pFunction)
{
    sdFunction.pRemove(hClientHandle);
    memset(&sdFunction, 0, sizeof(sdFunction));
    return SD_API_STATUS_SUCCESS;
}

SD_DEVICE_HANDLE SDIOGetHandle ()
{
    return (hClientHandle != NULL)?hClientHandle:NULL;
}

extern CRITICAL_SECTION g_DebugPrintLock;
///////////////////////////////////////////////////////////////////////////////
//  DllEntry - the main dll entry point
//  Input:  hInstance - the instance that is attaching
//          Reason - the reason for attaching
//          pReserved - not much
//  Output:
//  Return: always returns TRUE
//  Notes:  this is only used to initialize the zones
///////////////////////////////////////////////////////////////////////////////
extern
BOOL WINAPI DllEntry(HINSTANCE hInstance, ULONG Reason, LPVOID pReserved)
{
    if ( Reason == DLL_PROCESS_ATTACH )
    {
        DEBUGREGISTER(hInstance);
        if (!SDInitializeCardLib())
        {
            return FALSE;
        }
        InitializeCriticalSection(&g_DebugPrintLock);

    }
    else if ( Reason == DLL_PROCESS_DETACH )
    {
        SDDeinitializeCardLib ();
        DeleteCriticalSection(&g_DebugPrintLock);
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  DRG_Deinit - the deinit entry point for the memory driver
//  Input:  hDeviceContext - the context returned from SMC_Init
//  Output:
//  Return: always returns TRUE
//  Notes:
///////////////////////////////////////////////////////////////////////////////
extern
BOOL WINAPI DRG_Deinit(DWORD hDeviceContext)
{

#ifdef HIF_SDIO_BYPASS
    {
        extern void CleanupSDIOStackBypassMode();
        extern A_BOOL g_BypassModeActive;
        g_BypassModeActive = FALSE;
        CleanupSDIOStackBypassMode();
    }
#endif

    CloseHandle(wlanHandle);
    drvDeInit();
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  SlotEventCallBack - slot event callback for fast-path events
//  Input:  hDevice - device handle
//          pContext - device specific context that was registered
//          SlotEventType - slot event type
//          pData - Slot event data (can be NULL)
//          DataLength - length of slot event data (can be 0)
//  Output:
//  Return:
//  Notes:
//
//      If this callback is registered the client driver can be notified of
//      slot events (such as device removal) using a fast path mechanism.  This
//      is useful if a driver must be notified of device removal
//      before its XXX_Deinit is called.
//
//      This callback can be called at a high thread priority and should only
//      set flags or set events.  This callback must not perform any
//      bus requests or call any apis that can perform bus requests.
///////////////////////////////////////////////////////////////////////////////
VOID SlotEventCallBack(SD_DEVICE_HANDLE    hDevice,
                       PVOID               pContext,
                       SD_SLOT_EVENT_TYPE  SlotEventType,
                       PVOID               pData,
                       DWORD               DataLength)
{
    return;
}

BOOL
createRegKeyValues()
{
    DWORD   Status;
    HKEY    hKey;
    DWORD   dwDisp;
    DWORD   dwVal;

    Status = RegCreateKeyEx(
                 HKEY_LOCAL_MACHINE,
                 TEXT("Comm\\AR6K_SD1\\Parms"),
                 0,
                 NULL,
                 REG_OPTION_VOLATILE,
                 0,
                 NULL,
                 &hKey,
                 &dwDisp);

    if (Status != ERROR_SUCCESS)
    {
        return FALSE;
    }

    dwVal = 0;

    Status = RegSetValueEx(
                    hKey,
                    TEXT("BusNumber"),
                    0,
                    REG_DWORD,
                    (PBYTE)&dwVal,
                    sizeof(DWORD));

    if (Status != ERROR_SUCCESS)
    {
        return FALSE;
    }

    Status = RegSetValueEx(
                    hKey,
                    TEXT("BusType"),
                    0,
                    REG_DWORD,
                    (PBYTE)&dwVal,
                    sizeof(DWORD));


    if (Status != ERROR_SUCCESS)
    {
        return FALSE;
    }

    RegCloseKey(hKey);

    return TRUE;
}   // AddKeyValues


///////////////////////////////////////////////////////////////////////////////
//  DRG_Init - the init entry point for the memory driver
//  Input:  dwContext - the context for this init
//  Output:
//  Return: non-zero context
//  Notes:
///////////////////////////////////////////////////////////////////////////////
extern
DWORD WINAPI DRG_Init(DWORD dwContext)
{
    PWCHAR                          pRegPath = NULL;
    SDCARD_CLIENT_REGISTRATION_INFO ClientInfo;   // client into
    SD_API_STATUS                   Status;       // intermediate status
    DWORD                           dwRetCode = 0;

    // get the device handle from the bus driver
    hClientHandle = SDGetDeviceHandle(dwContext, &pRegPath);
    if (NULL == hClientHandle)
    {
        return 0;
    }

    memset(&ClientInfo, 0, sizeof(ClientInfo));

    // set client options and register as a client device
    _tcscpy(ClientInfo.ClientName,TEXT("Atheros AR6K SDIO Wifi Card"));
    // set the callback
    ClientInfo.pSlotEventCallBack = SlotEventCallBack;

#ifdef HIF_FAST_SDIO_ASYNC
        // set the magic fast ASYNC flag when used with the modified SDIO bus driver with
        // the fast ASYNC enhancement.
    ClientInfo.ClientFlags = 0x00000004;
#endif

    ClientInfo.ClientFlags |= SD_CLIENT_HANDLES_RETRY;

    Status = SDRegisterClient(hClientHandle,
                              NULL,
                              &ClientInfo);

    if (!SD_API_SUCCESS(Status))
    {
        return 0;
    }

#ifdef HIF_SDIO_BYPASS
    {
        extern void InitSDIOStackBypassMode(PTCHAR pRegPath, SD_DEVICE_HANDLE hSDDevice);

        InitSDIOStackBypassMode(pRegPath,hClientHandle);
    }
#endif

    if (drvInit() != A_OK)
    {
        return 0;
    }

    if (!createRegKeyValues())
    {
        return 0;
    }

    //
    // Create Named Event to notify AR6K Monitor Service program
    //
    wlanHandle = CreateEvent (NULL, FALSE, FALSE, L"ATHRWLAN6KEVENT");
    dwRetCode  = GetLastError ();
    if (NULL == wlanHandle)
    {
        return 0;
    }
    //
    // Set the event to trigger the service
    //
    SetEvent (wlanHandle);


    return 1;
}

/////////////////////////////////////////////////////////////////////
// Dummy Bus Interface wrappers

NDIS_STATUS
busDriverInit (NDIS_HANDLE miniportHandle,
               NDIS_HANDLE wrapperConfigurationContext,
               A_UINT32   sysIntr,
               BUS_DRIVER_HANDLE *busDriverHandle)
{
    *busDriverHandle = NULL;
    return NDIS_STATUS_SUCCESS;
}


void
busDriverIsr (BUS_DRIVER_HANDLE busDriverHandle,A_BOOL *callDsr)
{
    *callDsr = FALSE;
    return;
}

void
busDriverDsr (BUS_DRIVER_HANDLE busDriverHandle)
{
   return;
}

void
busDriverShutdown (BUS_DRIVER_HANDLE busDriverHandle)
{
    return;
}

BOOL GetRegistryKeyValue(HKEY   hKey,
                         WCHAR  *pKeyPath,
                         WCHAR  *pValueName,
                         PUCHAR pValue,
                         ULONG  BufferSize)
{
    LONG  status;       /* reg api status */
    HKEY  hOpenKey;     /* opened key handle */

    status = RegOpenKeyEx(hKey,
                          pKeyPath,
                          0,
                          0,
                          &hOpenKey);

    if (status != ERROR_SUCCESS) {
        return FALSE;
    }

    status = RegQueryValueEx(hOpenKey,
                             pValueName,
                             NULL,
                             NULL,
                             pValue,
                             &BufferSize);

    RegCloseKey(hOpenKey);

    if (ERROR_SUCCESS == status) {
        return TRUE;
    }

    return FALSE;
}

#ifdef AR6K_MTE_DRV_TEST

LONG g_CmdCount = 0;
static BOOL Ar6kResetForMTE();
static UCHAR g_DataBuffer[2048];

static SD_API_STATUS DoBusRequest(AR6K_SDIO_BUS_REQUEST *pBusRequest)
{
    SD_TRANSFER_CLASS   transferClass;
    SD_API_STATUS       sdStatus;
    SD_RESPONSE_TYPE    responseType;
    SD_COMMAND_RESPONSE response;

    if (pBusRequest->DataTransfer) {

        if (pBusRequest->DataWrite) {
            transferClass = SD_WRITE;
                /* copy to the common buffer so we don't have to deal with cross-process
                 * pointer mapping */
            memcpy(g_DataBuffer,
                   &pBusRequest->Buffer[0],
                   pBusRequest->DataBlocks * pBusRequest->DataBlockLength);

        } else {
            transferClass = SD_READ;
        }

    } else {
            /* just a command */
        transferClass = SD_COMMAND;
    }

        /* translate the response for the SDIO stack */
    switch (pBusRequest->ResponseType) {

        case SDIO_CMD_RESPONSE_NONE:
            responseType = NoResponse;
            break;
        case SDIO_CMD_RESPONSE_R1:
            responseType = ResponseR1;
            break;
        case SDIO_CMD_RESPONSE_R1b:
            responseType = ResponseR1b;
            break;
        case SDIO_CMD_RESPONSE_R4:
            responseType = ResponseR4;
            break;
        case SDIO_CMD_RESPONSE_R5:
            responseType = ResponseR5;
            break;
        case SDIO_CMD_RESPONSE_R6:
            responseType = ResponseR6;
            break;
        default:
            return SD_API_STATUS_UNSUCCESSFUL;
    }

    sdStatus = SDSynchronousBusRequest(hClientHandle,
                                       (UCHAR)pBusRequest->Command,
                                       pBusRequest->Argument,
                                       transferClass,
                                       responseType,
                                       &response,
                                       (transferClass == SD_COMMAND) ? 0 : pBusRequest->DataBlocks,
                                       (transferClass == SD_COMMAND) ? 0 : pBusRequest->DataBlockLength,
                                       (transferClass == SD_COMMAND) ? NULL : g_DataBuffer,
                                       0);

    if (SD_API_SUCCESS(sdStatus)) {
            /* copy response back to caller's buffer */
        memcpy(pBusRequest->ResponseBuffer,response.ResponseBuffer, 16);

        if (transferClass == SD_READ) {
                /* copy data back to caller's buffer */
            memcpy(&pBusRequest->Buffer[0],
                   g_DataBuffer,
                   pBusRequest->DataBlocks * pBusRequest->DataBlockLength);
        }
    } else {
        RETAILMSG(1, (TEXT("SD bus failed cmd:%d, status:0x%X \r\n"),pBusRequest->Command, sdStatus));
    }

    return sdStatus;
}

static SD_API_STATUS ChangeBusWidth(UCHAR BusWidth)
{
    SD_API_STATUS       sdStatus;
    SD_CARD_INTERFACE   ci;

    memset(&ci, 0, sizeof(ci));

        /* get current interface settings */
    sdStatus = SDCardInfoQuery(hClientHandle, SD_INFO_CARD_INTERFACE,
                                &ci, sizeof(ci));

    if (!SD_API_SUCCESS(sdStatus)) {
        return sdStatus;
    }

    if (BusWidth == 1) {
        ci.InterfaceMode = SD_INTERFACE_SD_MMC_1BIT;
    } else {
        ci.InterfaceMode = SD_INTERFACE_SD_4BIT;
    }

    return SDSetCardFeature(hClientHandle, SD_SET_CARD_INTERFACE,
                                &ci, sizeof(ci));

}

A_BOOL ar6k_bus_cmd(void *pInput, int Length)
{
    AR6K_SDIO_CMD *pSDIOCmd = (AR6K_SDIO_CMD *)pInput;
    SD_API_STATUS  sdStatus = SD_API_STATUS_SUCCESS;

    if (Length < sizeof(AR6K_SDIO_CMD)) {
        return FALSE;
    }

        /* only single threaded access allowed */
    if (InterlockedIncrement(&g_CmdCount) > 1) {
        InterlockedDecrement(&g_CmdCount);
        return FALSE;
    }

    if (pSDIOCmd->CommandType == AR6K_CMD_TYPE_BUS) {

        sdStatus = DoBusRequest(&pSDIOCmd->Request.AsBusRequest);

    } else if (pSDIOCmd->CommandType == AR6K_CMD_TYPE_CLK_CONFIG) {

        /* currently we just leave the clock alone */

    } else if (pSDIOCmd->CommandType == AR6K_CMD_TYPE_BUS_WIDTH_CONFIG) {

        if (pSDIOCmd->Request.AsBusWidthRequest.BusWidth == 1) {
            sdStatus = ChangeBusWidth(1);
        } else {
            sdStatus = ChangeBusWidth(4);
        }

    }else if (pSDIOCmd->CommandType == AR6K_CMD_TYPE_FULL_RESET) {

        Ar6kResetForMTE();

    } else {

        sdStatus = SD_API_STATUS_UNSUCCESSFUL;

    }

    InterlockedDecrement(&g_CmdCount);

    if (!SD_API_SUCCESS(sdStatus)) {
        return FALSE;
    }

    return TRUE;
}

static BOOL g_ResetSuccess = FALSE;

extern HIF_DEVICE *getHifDevice(SD_DEVICE_HANDLE *handle);

static DWORD Ar6kResetThreadFunc(LPVOID pContext)
{
    UCHAR         buffer = 0x1;
    SD_API_STATUS sdStatus;
    BOOL          success = FALSE;
    DWORD         targetType,targetId;

    cmnGetTargetInfo(&targetType,&targetId);

    do {

        if (A_FAILED(ar6000_reset_device(getHifDevice(hClientHandle), targetType, TRUE))) {
            break;
        }

            /* switch host controller back to 1 bit mode */
        sdStatus = ChangeBusWidth(1);

        if (!SD_API_SUCCESS(sdStatus)) {
            break;
        }

        sdStatus = SDSetCardFeature (hClientHandle,
                                     SD_IO_FUNCTION_DISABLE,
                                     NULL,
                                     0);

        if (!SD_API_SUCCESS(sdStatus)) {
            break;
        }

        buffer = (1 << 3);

            /* reset the SDIO controller to the un-enumerated state */
        sdStatus = SDReadWriteRegistersDirect(hClientHandle,
                                              SD_IO_WRITE,
                                              0,
                                              SD_IO_REG_IO_ABORT,
                                              0,
                                              &buffer,
                                              1);

        if (!SD_API_SUCCESS(sdStatus)) {
            break;
        }

        success = TRUE;

   } while (FALSE);

   g_ResetSuccess = success;

   return 0;
}

static BOOL Ar6kResetForMTE()
{
    HANDLE hThread;
    DWORD  threadId;

    /* create a thread, because the thread's stack variables will have the right
     * process permissions  */

    hThread = CreateThread(NULL,
                           0,
                           Ar6kResetThreadFunc,
                           NULL,
                           0,
                           &threadId);

    if (hThread == NULL) {
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return g_ResetSuccess;
}

#endif


