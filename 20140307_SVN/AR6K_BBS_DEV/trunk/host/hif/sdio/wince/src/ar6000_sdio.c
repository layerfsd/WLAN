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

#include "a_types.h"
#include "dbglog_api.h"
#include "ar6000_sdio.h"
#include "wince_common_drv.h"

static SD_FUNCTION  sdFunction;
static SD_DEVICE_HANDLE  hClientHandle = NULL;
A_UINT32 sdio1bitmode = 0;
A_UINT32 sdiobusspeedlow = 0;

#define _P	L"\r[AR6K_SDIO] "L

#define IOCTL_CAR6K_GET_FIRMWARE_VERSION    CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF1,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_GET_HOST_VERSION        CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF2,  METHOD_NEITHER, FILE_ANY_ACCESS)


enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 1,
}Ar6kSdioDebug;


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
	NDIS_STATUS   ndisStatus = A_OK;

	DRG_Exit();

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
	NDIS_DEBUG_PRINTF(DBG_TRACE, "SlotEventCallBack : Enter\r\n");
	return;
}

BOOL
createRegKeyValues()
{
	DWORD	Status;
	HKEY	hKey;    
	DWORD	dwDisp;
	DWORD	dwVal;
	
    NDIS_DEBUG_PRINTF(DBG_TRACE, "CreateRegKeyValues: Enter \r\n");

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
        NDIS_DEBUG_PRINTF(DBG_ERR, "CreateRegKeyValues: Error in creating reg key Parms \r\n");
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
        NDIS_DEBUG_PRINTF(DBG_ERR, "CreateRegKeyValues: Error in creating reg key BusNumber \r\n");
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
        NDIS_DEBUG_PRINTF(DBG_ERR, "CreateRegKeyValues: Error in creating reg key BusType \r\n");
	    return FALSE;
	}
	
	RegCloseKey(hKey);
    
    NDIS_DEBUG_PRINTF(DBG_TRACE, "CreateRegKeyValues: Exit \r\n");
	
	return TRUE;
}   // AddKeyValues


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




///////////////////////////////////////////////////////////////////////////////
//  DRG_Init - the init entry point for the memory driver
//  Input:  dwContext - the context for this init
//  Output: 
//  Return: non-zero context
//  Notes: 
///////////////////////////////////////////////////////////////////////////////
//extern void DbgPrintf(A_INT32 flags, A_CHAR *format, ...);


extern 
DWORD WINAPI DRG_Init(DWORD dwContext)
{
	PWCHAR*                         pRegPath = NULL;
    SDCARD_CLIENT_REGISTRATION_INFO ClientInfo;     // client into
    SD_API_STATUS                   Status;         // intermediate status
	HANDLE                          hThread;

    NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K_SDIO: +DRG_Init by DiskImage !!! \r\n");

    // get the device handle from the bus driver
    hClientHandle = SDGetDeviceHandle(dwContext, pRegPath);
    if (NULL == hClientHandle) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "-DRG_Init: Failed to get client handle \r\n");
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
        NDIS_DEBUG_PRINTF(DBG_ERR, "-DRG_Init: Failed to register client : 0x%08X \r\n", Status);
        return 0;
    }

	drvInit();

    if (!createRegKeyValues()) {
        NDIS_DEBUG_PRINTF(DBG_ERR, "-DRG_Init: Failed to create ndis registryentries \r\n");
        return 0;
    }

	// Perform NDIS register adapter on a separate thread to avoid
	// blocking the SDIO bus driver
	hThread = CreateThread(NULL, 0, 
		NdisRegisterAdapterThread, NULL, 0, NULL);
	
	CeSetThreadPriority(hThread, 200);
	CloseHandle(hThread);
	
	NDIS_DEBUG_PRINTF(DBG_TRACE, "DRG_Init : Exit\r\n");
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
	BOOL ret = FALSE;
	DWORD *ver;

	switch(IoctlCode)
	{
		case IOCTL_CAR6K_GET_FIRMWARE_VERSION:
			ver = (DWORD *)pOutBuf;
			*ver =  (FIRMWARE_MAJOR_VER << 28) | (FIRMWARE_MINOR_VER<<24) | (FIRMWARE_PATCH_VER<<16) | FIRMWARE_BUILD_VER;

			OutBufSize = 4;

			ret = TRUE;
			break;
		case IOCTL_CAR6K_GET_HOST_VERSION:
			ver = (DWORD *)pOutBuf;
			*ver =  (DRIVER_MAJOR_VER << 28) | (DRIVER_MINOR_VER<<24) | (DRIVER_PATCH_VER<<16) | (DRIVER_BUILD_VER<<8) | DRIVER_BUILD_BBS_VER;
		
			OutBufSize = 4;

			ret = TRUE;
			break;
	}
	
	return ret;
}


extern 
DWORD WINAPI DRG_Open(
    DWORD hDeviceContext,
    DWORD AccessCode,
    DWORD ShareMode)
{
    NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K_SDIO: +-DRG_Open\r\n");
    return hDeviceContext;
}

extern 
BOOL WINAPI DRG_Close(DWORD hOpenContext)
{
    NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K SDIO: +-DRG_Close\r\n");
    return TRUE;
}



