//------------------------------------------------------------------------------
// <copyright file="ar6000_cf.c" company="Atheros">
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
// AR6000 HIF CF source
//
// Author(s): ="Atheros"
//==============================================================================

#include <windows.h>
#include <nkintr.h>
#include "giisr.h"
#include "ar6000_cf.h"
#include "ar6000_cf_internal.h"
#include "bus_driver.h"
#include "osapi.h"

CF_DEVICE   *cfDevice = NULL;
CF_FUNCTION  cfFunction;
#ifdef CEPC
HANDLE       isrHandler;
GIISR_INFO   Info;
PVOID	     giisrMappedAddress;
#endif // CEPC

extern void InterruptDone(DWORD);
extern boolean printMask;

NDIS_STATUS
busDriverInit (NDIS_HANDLE miniportHandle, 
               NDIS_HANDLE wrapperConfigurationContext, 
               A_UINT32   sysIntr,
               BUS_DRIVER_HANDLE *busDriverHandle)
{
    PNDIS_RESOURCE_LIST pNdisResourceList;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR pResourceDesc;
    NDIS_PHYSICAL_ADDRESS           physicalAddress;
    A_UINT32						bufferSize;
    A_UINT32						dwResourceCount;
    NDIS_STATUS                     status=NDIS_STATUS_FAILURE;
    A_UINT32						addressSpace;

    *busDriverHandle = NULL;
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC,
                        "ar6000CFBusInit: Enter \n");

    cfDevice = A_MALLOC(sizeof(CF_DEVICE));
    if (cfDevice == NULL) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR,
            "ar6000CFBusInit: Allocate Memory for CF_DEVICE failed\n");
        return status; 
	}
    A_MEMZERO(cfDevice,sizeof(CF_DEVICE));

    cfDevice->miniportHandle = miniportHandle;
    cfDevice->wrapperConfigurationContext=wrapperConfigurationContext;

	pNdisResourceList = NULL;
	bufferSize = 0;

	//	First call is just to determine buffer size
    NdisMQueryAdapterResources(
            &status,
            wrapperConfigurationContext,
            pNdisResourceList,
            &bufferSize);

    pNdisResourceList = A_MALLOC(bufferSize);

    if (pNdisResourceList == NULL) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, 
            "ar6000CFBusInit: AllocateMemory for NDIS_RESOURCE_LIST failed\n");
        A_FREE(cfDevice);
        return NDIS_STATUS_FAILURE;
    }

    NdisMQueryAdapterResources(
            &status,
            wrapperConfigurationContext,
            pNdisResourceList,
            &bufferSize);

    if (status != NDIS_STATUS_SUCCESS) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, 
            "ar6000CFBusInit: NdisMQueryAdapterResources failed \n");
        A_FREE(pNdisResourceList);
        A_FREE(cfDevice);
        return status; 
    }

    /* Search for I/O address and IRQ in assigned resources. */
    
    dwResourceCount = pNdisResourceList->Count;
    pResourceDesc = &(pNdisResourceList->PartialDescriptors[0]);

    while (dwResourceCount--) {
        switch(pResourceDesc->Type) {
            case CmResourceTypeInterrupt:
                cfDevice->interruptNumber = 
                    pResourceDesc->u.Interrupt.Vector;
                HIF_DEBUG_PRINTF(ATH_LOG_INF,
                    "ar6000CFBusInit: Interrupt Number = %u\n", cfDevice->interruptNumber);
                break;

            case CmResourceTypeMemory:
                cfDevice->memLen = pResourceDesc->u.Memory.Length;
                cfDevice->deviceMemBase = pResourceDesc->u.Memory.Start.LowPart;
                HIF_DEBUG_PRINTF(ATH_LOG_INF,
				    "ar6000CFBusInit: Physical Address = %x Length = %x\n", 
                        cfDevice->deviceMemBase,cfDevice->memLen);
                break;
        
            default:
                HIF_DEBUG_PRINTF(ATH_LOG_ERR,
                    "ar6000CFBusInit: Unexpected assigned resource type %u\n", 
                    pResourceDesc->Type);
                break;
        }
        pResourceDesc++;
    }

    A_FREE(pNdisResourceList);

    cfDevice->sysIntr =  sysIntr;
    HIF_DEBUG_PRINTF(ATH_LOG_INF,
        "ar6000CFBusInit: sysIntr = %u\n", cfDevice->sysIntr);
    
    NdisSetPhysicalAddressLow(physicalAddress, cfDevice->deviceMemBase);
    NdisSetPhysicalAddressHigh(physicalAddress, 0);

    cfDevice->ndisMapped = 1;
    status = NdisMMapIoSpace((PVOID *)&cfDevice->mappedMemBase,
                            cfDevice->miniportHandle,
                            physicalAddress,
                            cfDevice->memLen);
    if (status != NDIS_STATUS_SUCCESS) {
		HIF_DEBUG_PRINTF(ATH_LOG_ERR,
				"ar6000CFBusInit: NdisMmapIoSpace failed.. Trying MmMapIoSpace \n");
        /* NdisMmapIoSpace fails sometimes in WINCE, type MmMapIoSpace */
	    cfDevice->mappedMemBase = (A_UINT32) MmMapIoSpace(physicalAddress,
                                                          cfDevice->memLen,
                                                          FALSE);
        if (!cfDevice->mappedMemBase) {
            HIF_DEBUG_PRINTF(ATH_LOG_ERR,
                "ar6000CFBusInit: MmMapIoSpace failed \n");
            A_FREE(cfDevice);
            return NDIS_STATUS_FAILURE;
        } 
        cfDevice->ndisMapped = 0;
    } 

	 addressSpace = 0;
#ifdef CEPC
#if (UNDER_CE==600) 
	giisrMappedAddress = (PVOID)cfDevice->mappedMemBase;
#else
    if (!TransBusAddrToStatic(PCIBus,
						 0,
						 physicalAddress,
						 cfDevice->memLen,
						 &addressSpace,
						 &giisrMappedAddress)) 
    {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR,
                "ar6000CFBusInit : TransBusAddrToStatic failed \n");
        A_FREE(cfDevice);
        return NDIS_STATUS_FAILURE;
    }
#endif //UNDER_CE
#endif //CEPC

    *busDriverHandle = (BUS_DRIVER_HANDLE)cfDevice;
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC,
                        "ar6000CFBusInit: Exit \n");
    return NDIS_STATUS_SUCCESS;
}


void
busDriverIsr (BUS_DRIVER_HANDLE busDriverHandle,A_BOOL *callDsr)
{
    CF_DEVICE *cfDevice;

    cfDevice = (CF_DEVICE *)busDriverHandle;
#ifdef DEBUG
    printMask = FALSE;
#endif //DEBUG
    if (cfDevice->interruptRegistered == FALSE) {
        InterruptDone(cfDevice->sysIntr);
        *callDsr = FALSE;
    } else {
	    cfFunction.pIsr((CF_DEVICE_HANDLE)cfDevice,callDsr);
    }
#ifdef DEBUG
    printMask = TRUE;
#endif //DEBUG
    return;
}

void
busDriverDsr (BUS_DRIVER_HANDLE busDriverHandle)
{
    CF_DEVICE *cfDevice;

    cfDevice = (CF_DEVICE *)busDriverHandle;
    cfFunction.pDsr((CF_DEVICE_HANDLE)cfDevice);
    return;
}

void
busDriverShutdown (BUS_DRIVER_HANDLE busDriverHandle)
{
    CF_DEVICE *cfDevice;

    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "ar6000CFShutdown: Enter \n");
    
    cfDevice = (CF_DEVICE *)busDriverHandle;
    
    // Remove the mapped space
    if (cfDevice->ndisMapped) {
        NdisMUnmapIoSpace(cfDevice->miniportHandle,
                          (PVOID)cfDevice->mappedMemBase,
                          cfDevice->memLen);
    } else {
        MmUnmapIoSpace((PVOID)cfDevice->mappedMemBase,
                       cfDevice->memLen);
    }
    A_FREE(cfDevice);
    cfDevice = NULL;
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "ar6000CFShutdown: Exit \n");
	return;
}

#ifdef POLL

A_UINT32 poll;

DWORD cfPollThread(LPVOID Context)
{
    CF_DEVICE    *cfDevice;
    A_BOOL       callDsr;

    cfDevice = (CF_DEVICE *)Context;

	while (poll) {
        busDriverIsr((BUS_DRIVER_HANDLE)cfDevice, &callDsr);
        if (callDsr) {
            busDriverDsr((BUS_DRIVER_HANDLE)cfDevice);
        }
        Sleep(1);
	} 

    return 0;
}

void startPollThread(CF_DEVICE *cfDevice)
{
    HANDLE hThread;

    poll = 1;
    hThread = CreateThread(NULL, 0, 
		        cfPollThread, (LPVOID)cfDevice, 0, NULL);
    CeSetThreadPriority(hThread, 200);
    CloseHandle(hThread);
    
    return;
}

void stopPollThread(CF_DEVICE *cfDevice)
{
    poll = 0;
}
#endif

A_STATUS
CF_MaskInterrupt(CF_DEVICE_HANDLE cfHandle) {
    CF_DEVICE    *cfDevice;

    HIF_DEBUG_PRINTF(ATH_LOG_TRC,
                        "CF_MaskInterrupt \n");

    cfDevice = (CF_DEVICE *)cfHandle;
#ifdef POLL
    stopPollThread(cfDevice);
#else
    NdisMDeregisterInterrupt(&cfDevice->interruptObject);

#ifdef CEPC
    if (isrHandler) {
        FreeIntChainHandler(isrHandler);
    }
#endif // CEPC
#endif
    
    return A_OK;
}

A_STATUS
CF_UnMaskInterrupt(CF_DEVICE_HANDLE cfHandle) {
    CF_DEVICE    *cfDevice;
#ifndef POLL
    NDIS_STATUS   ndisStatus;
#endif
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC,
				"CF_UnMaskInterrupt \n");
	cfDevice = (CF_DEVICE *)cfHandle;
	
#ifdef POLL
    startPollThread(cfDevice);
#else
#ifdef CEPC
    isrHandler = LoadIntChainHandler(TEXT("giisr.dll"),TEXT("ISRHandler"),(BYTE)cfDevice->interruptNumber);
    if (!isrHandler) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR,"CF_UnMaskInterrupt: Load int chain handler failed \n");
        return A_ERROR;
	}
	memset (&Info,0,sizeof(Info));
    Info.SysIntr = cfDevice->sysIntr;
    Info.CheckPort = TRUE;
    Info.PortIsIO = FALSE;
    Info.UseMaskReg = TRUE;
    Info.PortSize = sizeof(BYTE);

	Info.PortAddr = (DWORD)giisrMappedAddress + 0x00000400;
	Info.MaskAddr = (DWORD)giisrMappedAddress + 0x00000418;

	if (!KernelLibIoControl(isrHandler,IOCTL_GIISR_INFO,&Info,sizeof(Info),NULL,0,NULL))  {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR,"CF_UnMaskInterrupt: KernelLibIoControl failed  \n");
		return A_ERROR;
    }
	
#endif // CEPC
    ndisStatus = NdisMRegisterInterrupt(&cfDevice->interruptObject,
                                    cfDevice->miniportHandle,
                                    cfDevice->interruptNumber,
                                    0, TRUE, TRUE, 0);

    if (ndisStatus != NDIS_STATUS_SUCCESS) { 
        return A_ERROR;
    }
    
#endif
    cfDevice->interruptRegistered= TRUE;

    return A_OK;
}

A_STATUS
CF_RegisterFunction (CF_FUNCTION *function)
{
    cfFunction.pName = function->pName;
    cfFunction.pProbe = function->pProbe;
    cfFunction.pRemove = function->pRemove;
    cfFunction.pIsr = function->pIsr;
    cfFunction.pDsr = function->pDsr;
    cfFunction.pInterruptPending = function->pInterruptPending;
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC,
            "CF_RegisterFunction \n");

    if (cfDevice == NULL) return A_ERROR;

    cfFunction.pProbe((CF_DEVICE_HANDLE)cfDevice);

    return A_OK;
}

A_STATUS
CF_UnregisterFunction (CF_FUNCTION *function)
{
    HIF_DEBUG_PRINTF(ATH_LOG_TRC,
            "CF_UnregisterFunction \n");

    AR_DEBUG_ASSERT(cfDevice != NULL);
    cfFunction.pRemove((CF_DEVICE_HANDLE)cfDevice);

    return A_OK;
}

static A_STATUS 
CF_BusRequest_8(CF_DEVICE_HANDLE cfHandle, CF_REQUEST *pReq)
{
    volatile A_UCHAR     *data, *base;
    A_UINT32     len, i;
    CF_DEVICE    *device;

    device = (CF_DEVICE *)cfHandle;

    if (device == NULL || pReq == NULL) {
        return A_ERROR;
    }

    data = (A_UCHAR *)pReq->pDataBuffer;
    base = (A_UCHAR *)(device->mappedMemBase + pReq->address);
    len = pReq->length;

    HIF_DEBUG_PRINTF(ATH_LOG_TRC,"CF_BusRequest_8 \n");

    if (pReq->flags & CFREQ_FLAGS_DATA_WRITE) {
        if (pReq->flags & CFREQ_FLAGS_FIXED_ADDRESS) {
            for (i = 0; i < len; i++) {
                *base = *data;
                data++;
            }
        }  else {
            for (i = 0; i < len; i++) {
                *base = *data;
                base++;
                data++;
            }
        }
    } else {
        if (pReq->flags & CFREQ_FLAGS_FIXED_ADDRESS) {
            for (i = 0; i < len; i++) {
                *data = *base;
                data++;
            }
        } else {
            for (i = 0; i < len; i++) {
                *data = *base;
                base++;
                data++;
            }
        }
    }
    
    return A_OK;
}

static A_STATUS 
CF_BusRequest_16(CF_DEVICE_HANDLE cfHandle, CF_REQUEST *pReq)
{
    volatile A_UINT16     *data, *base;
    A_UINT32     len, i;
    CF_DEVICE    *device;

    device = (CF_DEVICE *)cfHandle;

    if (device == NULL || pReq == NULL) {
        return A_ERROR;
    }

    HIF_DEBUG_PRINTF(ATH_LOG_TRC,"CF_BusRequest_16 \n");

    data = (A_UINT16 *)pReq->pDataBuffer;
    base = (A_UINT16 *)(device->mappedMemBase + pReq->address);
    len = pReq->length;

    /*
     * Assert on odd address and odd length 
     */
    
    AR_DEBUG_ASSERT(!(((A_UINT32)data)%2));
    AR_DEBUG_ASSERT(!(((A_UINT32)base)%2));
    AR_DEBUG_ASSERT(!(len%2));
    
    len = len/2;

    if (pReq->flags & CFREQ_FLAGS_DATA_WRITE) {
        if (pReq->flags & CFREQ_FLAGS_FIXED_ADDRESS) {
            for (i = 0; i < len; i++) {
                *base = *data;
                data++;
            }
        }  else {
            for (i = 0; i < len; i++) {
                *base = *data;
                base++;
                data++;
            }
        }

    } else {
        if (pReq->flags & CFREQ_FLAGS_FIXED_ADDRESS) {
            for (i = 0; i < len; i++) {
                *data = *base;
                data++;
            }
        } else {
            for (i = 0; i < len; i++) {
                *data = *base;
                base++;
                data++;
            }
        }
    }
    return A_OK;
}

A_STATUS
CF_BusRequest(CF_DEVICE_HANDLE cfHandle, CF_REQUEST *pReq,A_UINT32 bits)
{
    A_STATUS status = A_ERROR;
    
    switch (bits) {
    case 8:
        status = CF_BusRequest_8(cfHandle, pReq);
        break;
    case 16:
        status = CF_BusRequest_16(cfHandle, pReq);
        break;
    default:
        AR_DEBUG_ASSERT(0);
        break;
    }
    return status;
}
