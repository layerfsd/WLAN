//------------------------------------------------------------------------------
// <copyright file="ar6000_sdio.c" company="Atheros">
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
// Driver entry points for SDIO
//
// Author(s): ="Atheros"
//==============================================================================

#include <windows.h>
#include <types.h>

#include "SDCard.h"
#include "Sdcardddk.h"
#include <ceddk.h>
#include <ndis.h>

#include "athtypes.h"
#include "ar6000_sdio.h"

static SD_FUNCTION  sdFunction;
static SD_DEVICE_HANDLE  hClientHandle = NULL;
A_UINT32 sdio1bitmode = 0;
A_UINT32 sdiobusspeedlow = 0;

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
    BOOL fRet = TRUE;
    
    if ( Reason == DLL_PROCESS_ATTACH ) {
		DEBUGREGISTER(hInstance);
        if (!SDInitializeCardLib()) {
            fRet = FALSE;
        }
    }
    else if ( Reason == DLL_PROCESS_DETACH ) {
        SDDeinitializeCardLib();
    }

    return fRet;
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
	NDIS_STATUS   ndisStatus;

    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "DRG_Deinit : Enter\n");
	NdisDeregisterAdapter(&ndisStatus, L"AR6K_SD1");
	if (ndisStatus != NDIS_STATUS_SUCCESS) {
		HIF_DEBUG_PRINTF(ATH_LOG_ERR, 
			"NdisDeRegisterAdapter Failed : 0x%x\n", ndisStatus);
	}
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "DRG_DeInit : Exit\n");
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
        HIF_DEBUG_PRINTF(ATH_LOG_TRC, "SlotEventCallBack : Enter\n");
        HIF_DEBUG_PRINTF(ATH_LOG_TRC, "SlotEventCallBack : Exit\n");
	return;
}

BOOL
createRegKeyValues()
{
	DWORD	Status;
	HKEY	hKey;    
	DWORD	dwDisp;
	DWORD	dwVal;
	
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "CreateRegKeyValues: Enter \n");

	Status = RegCreateKeyEx(
	             HKEY_LOCAL_MACHINE,
	             TEXT("Comm\\AR6K_SD1\\Parms"),
	             0,
	             NULL,
	             REG_OPTION_NON_VOLATILE,
	             0,
	             NULL,
	             &hKey,
	             &dwDisp);
	
	if (Status != ERROR_SUCCESS) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, "CreateRegKeyValues: Error in creating reg key Parms \n");
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
	
	if (Status != ERROR_SUCCESS) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, "CreateRegKeyValues: Error in creating reg key BusNumber \n");
	    return FALSE;
	}
	
	Status = RegSetValueEx(
                    hKey,
	                TEXT("BusType"),
	                0,
	                REG_DWORD, 
	                (PBYTE)&dwVal,
	                sizeof(DWORD));
	
	if (Status != ERROR_SUCCESS) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, "CreateRegKeyValues: Error in creating reg key BusType \n");
	    return FALSE;
	}
	
	RegCloseKey(hKey);
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "CreateRegKeyValues: Exit \n");
	
	return TRUE;
}   // AddKeyValues


DWORD
NdisRegisterAdapterThread(PVOID Context)
{
	NDIS_STATUS   ndisStatus;

	HIF_DEBUG_PRINTF(ATH_LOG_TRC, "NdisRegisterAdapterThread : Enter\n");
	NdisRegisterAdapter(&ndisStatus, L"AR6K_SD", L"AR6K_SD1");
	if (ndisStatus != NDIS_STATUS_SUCCESS) {
		HIF_DEBUG_PRINTF(ATH_LOG_ERR, "NdisRegisterAdapter Failed : 0x%x\n", ndisStatus);
	}
	return ndisStatus;
}

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
	PWCHAR*                         pRegPath = NULL;
    SDCARD_CLIENT_REGISTRATION_INFO ClientInfo;     // client into
    SD_API_STATUS                   Status;         // intermediate status
	HANDLE                          hThread;

    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K_SDIO: +DRG_Init\n");

    // get the device handle from the bus driver
    hClientHandle = SDGetDeviceHandle(dwContext, pRegPath);
    if (NULL == hClientHandle) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, "-DRG_Init: Failed to get client handle \n");
        return 0;
    }

    memset(&ClientInfo, 0, sizeof(ClientInfo));

    // set client options and register as a client device
    _tcscpy(ClientInfo.ClientName, TEXT("Atheros AR6K SDIO Wifi Card"));

    // set the callback
    ClientInfo.pSlotEventCallBack = SlotEventCallBack;

    Status = SDRegisterClient(hClientHandle, 
                              NULL, 
                              &ClientInfo);

    if (!SD_API_SUCCESS(Status)) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, "-DRG_Init: Failed to register client : 0x%08X \n",
			            Status);
        return 0;
    }

    if (!createRegKeyValues()) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, "-DRG_Init: Failed to create ndis registryentries \n");
        return 0;
    }

	// Perform NDIS register adapter on a separate thread to avoid
	// blocking the SDIO bus driver
	hThread = CreateThread(NULL, 0, 
		NdisRegisterAdapterThread, NULL, 0, NULL);
	CeSetThreadPriority(hThread, 200);
	CloseHandle(hThread);
	
	HIF_DEBUG_PRINTF(ATH_LOG_TRC, "DRG_Init : Exit\n");
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//  
//  Notes:  PLEASE DO NOT REMOVE THE FOLLOWING FUNCTIONS. THOUGH THEY WILL NEVER
//          BE USED, STREAMS BASED DRIVER MUST EXPORT THESE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
extern 
BOOL WINAPI DRG_IOControl(
    DWORD   Handle,
    DWORD   IoctlCode,
    PBYTE   pInBuf,
    DWORD   InBufSize,
    PBYTE   pOutBuf,
    DWORD   OutBufSize,
    PDWORD  pBytesReturned
)
{
	return FALSE;
}

extern 
DWORD WINAPI DRG_Open(
    DWORD hDeviceContext,
    DWORD AccessCode,
    DWORD ShareMode)
{
    DEBUGMSG(SDCARD_ZONE_FUNC, (TEXT("AR6K_SDIO: +-DRG_Open\n")));
    return hDeviceContext;
}

extern 
BOOL WINAPI DRG_Close(DWORD hOpenContext)
{
    DEBUGMSG(SDCARD_ZONE_FUNC, (TEXT("AR6K SDIO: +-DRG_Close\n")));
    return TRUE;
}
