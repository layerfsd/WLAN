//------------------------------------------------------------------------------
// <copyright file="hif.c" company="Atheros">
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
// HIF CF source
//
// Author(s): ="Atheros"
//==============================================================================

#include "hif_internal.h"
#include "athtypes.h"

/* ------ Forward declarations ------ */

static BOOL
hifDeviceInserted(CF_DEVICE_HANDLE cfHandle);

static VOID
hifDeviceRemoved(CF_DEVICE_HANDLE cfHandle);

static VOID
hifISRHandler(CF_DEVICE_HANDLE cfHandle,A_BOOL *callDsr);

static void
hifDSRHandler(CF_DEVICE_HANDLE cfHandle);

static HIF_DEVICE *
addHifDevice(CF_DEVICE_HANDLE cfHandle);

static HIF_DEVICE *
getHifDevice(CF_DEVICE_HANDLE cfHandle);

static VOID
delHifDevice(CF_DEVICE_HANDLE cfHandle);

/* ------ Global Variables ------ */

HIF_DEVICE hifDevice[HIF_MAX_DEVICES];

HTC_CALLBACKS htcCallbacks;

CF_FUNCTION  cfFunction;


void
HIFRegisterCallbacks(HTC_CALLBACKS *callbacks)
{
    A_STATUS    status;
    
    /* Store the callback and event handlers */
    htcCallbacks.deviceInsertedHandler = callbacks->deviceInsertedHandler;
    htcCallbacks.deviceRemovedHandler  = callbacks->deviceRemovedHandler;
    htcCallbacks.deviceSuspendHandler  = callbacks->deviceSuspendHandler;
    htcCallbacks.deviceResumeHandler   = callbacks->deviceResumeHandler;
    htcCallbacks.deviceWakeupHandler   = callbacks->deviceWakeupHandler;
    htcCallbacks.rwCompletionHandler   = callbacks->rwCompletionHandler;
    htcCallbacks.deviceInterruptDisabler = callbacks->deviceInterruptDisabler;
    htcCallbacks.deviceInterruptEnabler = callbacks->deviceInterruptEnabler;
    htcCallbacks.dsrHandler            = callbacks->dsrHandler;

    /* Register the callback handlers with the lower driver */
    cfFunction.pName      = "cf_wlan";
    cfFunction.pProbe     = hifDeviceInserted;
    cfFunction.pRemove    = hifDeviceRemoved;
    cfFunction.pIsr       = hifISRHandler;
    cfFunction.pDsr       = hifDSRHandler;

    status = CF_RegisterFunction(&cfFunction);

    AR_DEBUG_ASSERT(status == A_OK);
}

A_STATUS 
HIFReadWrite(HIF_DEVICE *device, 
             A_UINT32 address, 
             A_UCHAR *buffer, 
             A_UINT32 length, 
             HIF_REQUEST *request, 
             void *context) 
{
    CF_REQUEST cfRequest;
	A_STATUS   status;
    A_UINT32   remainingLen;
    A_UINT32   curPos;
 
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "HIFReadWrite: Enter\n", device);
    
	A_MEMZERO(&cfRequest, sizeof(CF_REQUEST));

	if (request->emode == HIF_SYNCHRONOUS) {
		HIF_DEBUG_PRINTF(ATH_LOG_INF, "HIFReadWrite : Execution mode: Synchronous\n");
	} else if (request->emode == HIF_ASYNCHRONOUS) {
		HIF_DEBUG_PRINTF(ATH_LOG_INF, ("HIFReadWrite: Execution mode:Asynchronous\n"));
	} else {
		HIF_DEBUG_PRINTF(ATH_LOG_ERR, 
					"Invalid execution mode: %d\n", request->emode);
		return A_ERROR;
	}

    if (request->direction == HIF_WRITE) {
        cfRequest.flags |= CFREQ_FLAGS_DATA_WRITE;
        HIF_DEBUG_PRINTF(ATH_LOG_INF, ("HifReadWrite: Direction: Write\n"));
    } else if (request->direction == HIF_READ) {
        HIF_DEBUG_PRINTF(ATH_LOG_INF, ("HifReadWrite: Direction: Read\n"));
    } else {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, 
                        "Invalid direction: %d\n", request->direction);
        return A_ERROR;
    }

    if (request->amode == HIF_FIXED_ADDRESS) {
		cfRequest.flags |= CFREQ_FLAGS_FIXED_ADDRESS;
        HIF_DEBUG_PRINTF(ATH_LOG_INF, ("Address mode: Fixed\n"));
    } else if (request->amode == HIF_INCREMENTAL_ADDRESS) {
        HIF_DEBUG_PRINTF(ATH_LOG_INF, ("Address mode: Incremental\n"));
    } else {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, 
                        "Invalid address mode: %d\n", request->amode);
        return A_ERROR;
    }

	if (HIF_IS_MBOX_ADDR(address) ) {
        address = address & ~(HIF_MBOX_WIDTH-1);

#define BLK_SZ  (HIF_MBOX_WIDTH - 2)
        remainingLen = length;
        curPos = 0;

        while (remainingLen > BLK_SZ) {
		    cfRequest.address = address;
		    cfRequest.length = BLK_SZ;
			cfRequest.pDataBuffer = &buffer[curPos];

            if (request->dmode == HIF_BYTE_BASIS) {
    		    status = CF_BusRequest(device->cfHandle, &cfRequest, 8);
            } else {
    		    status = CF_BusRequest(device->cfHandle, &cfRequest, 16);
            }

    		if (status != A_OK) {
        		HIF_DEBUG_PRINTF(ATH_LOG_ERR, ("HIF Write failed\n"));
        		return A_ERROR;
    		}

            curPos += BLK_SZ;
            remainingLen -= BLK_SZ;
		}
		cfRequest.pDataBuffer = &buffer[curPos];
        cfRequest.address = address + HIF_MBOX_WIDTH - remainingLen;
		cfRequest.length = remainingLen;

#ifdef ONLY_16BIT
    	status = CF_BusRequest(device->cfHandle, &cfRequest, 16);
#else
        if (request->dmode == HIF_BYTE_BASIS) {
    		status = CF_BusRequest(device->cfHandle, &cfRequest, 8);
        } else {
    		status = CF_BusRequest(device->cfHandle, &cfRequest, 16);
        }
#endif

   		if (status != A_OK) {
        	HIF_DEBUG_PRINTF(ATH_LOG_ERR, ("HIF Write failed\n"));
        	return A_ERROR;
   		}

	} else {
		cfRequest.address = address;
        cfRequest.pDataBuffer = buffer;
	    cfRequest.length = length;
#ifdef ONLY_16BIT
    	status = CF_BusRequest(device->cfHandle, &cfRequest, 16);
#else
    	status = CF_BusRequest(device->cfHandle, &cfRequest, 8);
#endif
	}

    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "HIFReadWrite: Exit\n", device);

    return status;
}

void
HIFShutDownDevice(HIF_DEVICE *device)
{
    A_STATUS status;
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifShutDownDevice \n");
    /* Unregister with bus driver core */
    status = CF_UnregisterFunction(&cfFunction);
}

static void
hifISRHandler(CF_DEVICE_HANDLE cfHandle,A_BOOL *callDsr)
{
    A_STATUS    status;
    HIF_DEVICE *device;

    device = getHifDevice(cfHandle);
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifIsrHandler: Enter\n");

	status = htcCallbacks.deviceInterruptDisabler(device, callDsr);
	AR_DEBUG_ASSERT(status == A_OK);
        
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifIsrHandler: Exit\n");

    return;
}

static void
hifDSRHandler(CF_DEVICE_HANDLE cfHandle)
{
    A_STATUS    status;
    HIF_DEVICE *device;

	device = getHifDevice(cfHandle);
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifDsrHandler: Enter\n");

    status = htcCallbacks.dsrHandler(device);
    
    AR_DEBUG_ASSERT(status == A_OK);
    
	htcCallbacks.deviceInterruptEnabler(device);

    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifDsrHandler: Exit\n");
}


static BOOL
hifDeviceInserted(CF_DEVICE_HANDLE cfHandle)
{
    HIF_DEVICE *device;

    device = addHifDevice(cfHandle);
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifDeviceInserted: Enter\n");

    HIF_DEBUG_PRINTF(ATH_LOG_INF, "hifDeviceInsetred: Device = %p\n",device);

    /* Inform HTC */
    if ((htcCallbacks.deviceInsertedHandler(device)) != A_OK) {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, "hifDeviceInserted: HTC failed device inserted\n");
        return FALSE;
    }
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifDeviceInserted: Exit\n");

    return TRUE;
}

void
HIFAckInterrupt(HIF_DEVICE *device)
{
	return;
}

void
HIFUnMaskInterrupt(HIF_DEVICE *device)
{
    CF_UnMaskInterrupt(device->cfHandle);
    return;
}

void 
HIFMaskInterrupt(HIF_DEVICE *device)
{
    CF_MaskInterrupt(device->cfHandle);
    return;
}

static void
hifDeviceRemoved(CF_DEVICE_HANDLE cfHandle)
{
    A_STATUS status;
    HIF_DEVICE *device;

    device = getHifDevice(cfHandle);
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifDeviceRemoved: Enter\n");
    HIF_DEBUG_PRINTF(ATH_LOG_INF, "hifDeviceRemoved: Device = %p\n",device);

    /* Inform HTC */
    status = htcCallbacks.deviceRemovedHandler(device);
    
    delHifDevice(cfHandle);
    
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifDeviceRemoved: Exit\n");

    AR_DEBUG_ASSERT(status == A_OK);
}

static HIF_DEVICE *
addHifDevice(CF_DEVICE_HANDLE cfHandle)
{
    hifDevice[0].cfHandle = cfHandle;
    return &hifDevice[0];
}

static HIF_DEVICE *
getHifDevice(CF_DEVICE_HANDLE cfHandle)
{
    return &hifDevice[0];
}

static void
delHifDevice(CF_DEVICE_HANDLE cfHandle)
{
    hifDevice[0].cfHandle = NULL;
}
