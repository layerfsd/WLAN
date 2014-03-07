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
// SDIO HIF source
//
// Author(s): ="Atheros"
//==============================================================================

#include "ar6000_sdio.h"
#include "hif.h"
#include <ndis.h>

struct hif_device {
    SD_DEVICE_HANDLE *handle;
};

#ifdef CEPC
	NDIS_EVENT   hifIRQEvent;
#endif

static BOOL
hifDeviceInserted(SD_DEVICE_HANDLE *device);

static void
hifDeviceRemoved(SD_DEVICE_HANDLE *device);

static void 
hifRWCompletionHandler(SD_DEVICE_HANDLE  hDevice,
					   PSD_BUS_REQUEST   pRequest,
					   PVOID             notUsed,
					   DWORD             dwParam);

static SD_API_STATUS 
hifIRQHandler(SD_DEVICE_HANDLE hDevice, PVOID notUsed);

static HIF_DEVICE *
addHifDevice(SD_DEVICE_HANDLE *handle);

static HIF_DEVICE *
getHifDevice(SD_DEVICE_HANDLE *handle);

static void
delHifDevice(SD_DEVICE_HANDLE *handle);

/* ------ Static Variables ------ */

static SD_FUNCTION   sdFunction;
HIF_DEVICE    hifDevice[HIF_MAX_DEVICES];
HTC_CALLBACKS htcCallbacks;
A_UINT16      maxBlocks;
A_UINT16      maxBlockSize;
A_UINT8       funcNo = 0;

/* ------ Functions ------ */
void
HIFRegisterCallbacks(HTC_CALLBACKS *callbacks)
{
    SD_API_STATUS sdStatus;

    /* Store the callback and event handlers */
    htcCallbacks.deviceInsertedHandler = callbacks->deviceInsertedHandler;
    htcCallbacks.deviceRemovedHandler = callbacks->deviceRemovedHandler;
    htcCallbacks.deviceSuspendHandler = callbacks->deviceSuspendHandler;
    htcCallbacks.deviceResumeHandler = callbacks->deviceResumeHandler;
    htcCallbacks.deviceWakeupHandler = callbacks->deviceWakeupHandler;
    htcCallbacks.rwCompletionHandler = callbacks->rwCompletionHandler;
	htcCallbacks.deviceInterruptDisabler = callbacks->deviceInterruptDisabler;
    htcCallbacks.deviceInterruptEnabler = callbacks->deviceInterruptEnabler;
	htcCallbacks.dsrHandler = callbacks->dsrHandler;

    /* Register with bus driver core */
    sdFunction.pName      = "sdio_wlan";
    sdFunction.pProbe     = hifDeviceInserted;
    sdFunction.pRemove    = hifDeviceRemoved;

    sdStatus = SDIORegisterFunction(&sdFunction);
    AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));
}

A_STATUS 
HIFReadWrite(HIF_DEVICE  *device, 
             A_UINT32     address, 
             A_UCHAR     *buffer, 
             A_UINT32     length, 
             HIF_REQUEST *request, 
             void        *context) 
{
    A_UINT8             rw;
    A_UINT8             mode;
    A_UINT8             opcode;
    A_UINT32            blockLen, blockCount, count;
    PSD_BUS_REQUEST     busRequest;
    A_STATUS            status = A_OK;
	SD_TRANSFER_CLASS   transferClass;
	DWORD               dwArg;
	A_UCHAR             command;
	SD_API_STATUS       sdStatus;
	SD_COMMAND_RESPONSE response;
 
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "HIFReadWrite:Enter\n");
	HIF_DEBUG_PRINTF(ATH_LOG_TRC, "Address 0x%x\n", address);
    
	if (request->dmode == HIF_BLOCK_BASIS && request->type != HIF_EXTENDED_IO) {
		HIF_DEBUG_PRINTF(ATH_LOG_ERR, "Block mode not allowed for this type of command\n");
		return A_ERROR;
	}

    if (request->dmode == HIF_BLOCK_BASIS) {
        mode = SD_IO_BLOCK_MODE;
        blockLen = HIF_MBOX_BLOCK_SIZE;
        blockCount = length / HIF_MBOX_BLOCK_SIZE;
		count = blockCount;
        HIF_DEBUG_PRINTF(ATH_LOG_TRC, 
                        "Block mode (BlockLen: %d, BlockCount: %d)\n",
                        blockLen, blockCount);
    } else if (request->dmode == HIF_BYTE_BASIS) {
        mode = SD_IO_BYTE_MODE;
        blockLen = length;
        blockCount = 1;
		count = blockLen;
        HIF_DEBUG_PRINTF(ATH_LOG_TRC, 
                        "Byte mode (BlockLen: %d, BlockCount: %d)\n",
                        blockLen, blockCount);
    } else {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, 
                        "Invalid data mode: %d\n", request->dmode);
        return A_ERROR;
	}

    if (request->amode == HIF_FIXED_ADDRESS) {
        opcode = SD_IO_FIXED_ADDRESS;
        HIF_DEBUG_PRINTF(ATH_LOG_TRC, "Fixed       ");
    } else if (request->amode == HIF_INCREMENTAL_ADDRESS) {
        opcode = SD_IO_INCREMENT_ADDRESS;
        HIF_DEBUG_PRINTF(ATH_LOG_TRC, "Incremental ");
    } else {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, 
                        "Invalid address mode: %d\n", request->amode);
        return A_ERROR;
    }

    if (request->direction == HIF_WRITE) {
        transferClass = SD_WRITE;
		rw = SD_IO_OP_WRITE;
		if ((address >= HIF_MBOX_START_ADDR(0)) && 
            (address <= HIF_MBOX_END_ADDR(3))) {
            /* Mailbox write. Adjust the address so that the last byte 
               falls on the EOM address */
            address = address + HIF_MBOX_WIDTH - length;
        }
        HIF_DEBUG_PRINTF(ATH_LOG_TRC, "[Write]");
	} else {
		transferClass = SD_READ;
		rw = SD_IO_OP_READ;
		HIF_DEBUG_PRINTF(ATH_LOG_TRC, "[Read ]");
	}
        

	if (request->type == HIF_EXTENDED_IO) {
		dwArg = BUILD_IO_RW_EXTENDED_ARG(rw, mode, funcNo, 
			address, opcode, count);
		command = SD_IO_RW_EXTENDED;

    } else if (request->type == HIF_BASIC_IO) {
		dwArg = BUILD_IO_RW_DIRECT_ARG(rw, SD_IO_RW_NORMAL, 
			funcNo, address, 0);
		command = SD_IO_RW_NORMAL;

	} else {
        HIF_DEBUG_PRINTF(ATH_LOG_ERR, 
                        "Invalid command type: %d\n", request->type);
        return A_ERROR;
	}

    if (request->emode == HIF_SYNCHRONOUS) {
        HIF_DEBUG_PRINTF(ATH_LOG_TRC, "Synchronous\n");

		sdStatus = SDSynchronousBusRequest(device->handle, command, dwArg,
					transferClass, ResponseR5, &response, blockCount, 
					blockLen, buffer, 0);
		if (!SD_API_SUCCESS(sdStatus)) {
			HIF_DEBUG_PRINTF(ATH_LOG_ERR, "SDBusRequest failed 0x%x\n", sdStatus);
			status = A_ERROR;
		}
  
	} else {
       	HIF_DEBUG_PRINTF(ATH_LOG_TRC, "Asynchronous\n");
		sdStatus = SDBusRequest(device->handle, command, dwArg, transferClass,
					ResponseR5, blockCount, blockLen, buffer,
					hifRWCompletionHandler, (DWORD) context, &busRequest, 0);
		
        if (!SD_API_SUCCESS(sdStatus)) {
            status = A_ERROR;
        }
  	}

    return status;
}

void
HIFShutDownDevice(HIF_DEVICE *device)
{
    SD_API_STATUS sdStatus;
    //SDCONFIG_BUS_MODE_DATA busSettings;
	UCHAR         buffer;

	if (device == NULL) {
		HIF_DEBUG_PRINTF(ATH_LOG_ERR, "Invalid Handle passed\n");
		return;
	}
    /* Remove the allocated current if any */
	/*
	 * There is no equivalent for this one in WINCE
    status = SDLIB_IssueConfig(device->handle, SDCONFIG_FUNC_FREE_SLOT_CURRENT, 
                               NULL, 0);
    DBG_ASSERT(SDIO_SUCCESS(status));
	*/

    /* Disable the card */
	SDIODisconnectInterrupt(device->handle);
    sdStatus = SDSetCardFeature(device->handle, SD_IO_FUNCTION_DISABLE,
			                      NULL, 0);
    AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));

    /* Perform a soft I/O reset */
	sdStatus = SDReadWriteRegistersDirect(device->handle, SD_IO_WRITE, 0, 
					SD_IO_REG_IO_ABORT, 1, &buffer, 0);
    AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));

    /* 
     * WAR - Codetelligence driver does not seem to shutdown correctly in 1
     * bit mode. By default it configures the HC in the 4 bit. Its later in
     * our driver that we switch to 1 bit mode. If we try to shutdown, the
     * driver hangs so we revert to 4 bit mode, to be transparent to the 
     * underlying bus driver.
     */
	/*
	 * Not sure whether this is required for WINCE hence commenting
	 */
	/*
    if (sdio1bitmode) {
        ZERO_OBJECT(busSettings);
        busSettings.BusModeFlags = device->handle->pHcd->CardProperties.BusMode;
        SDCONFIG_SET_BUS_WIDTH(busSettings.BusModeFlags, 
                               SDCONFIG_BUS_WIDTH_4_BIT);

        // Issue config request to change the bus width to 4 bit
        status = SDLIB_IssueConfig(device->handle, SDCONFIG_BUS_MODE_CTRL,
                                   &busSettings, 
                                   sizeof(SDCONFIG_BUS_MODE_DATA));
        DBG_ASSERT(SDIO_SUCCESS(status));
    }
	*/

    /* Unregister with bus driver core */
    sdStatus = SDIOUnregisterFunction(&sdFunction);
    AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));
	return;
}

void 
hifRWCompletionHandler(SD_DEVICE_HANDLE  hDevice,
					   PSD_BUS_REQUEST   pRequest,
					   PVOID             notUsed,
					   DWORD             dwParam) 
{
    A_STATUS status;
    PVOID    htcContext;

    if (SD_API_SUCCESS(pRequest->Status)) {
        status = A_OK;
    } else {
        status = A_ERROR;
    }
    htcContext = (void *)dwParam;

    htcCallbacks.rwCompletionHandler(htcContext, status);
    AR_DEBUG_ASSERT(status == A_OK);

    SDFreeBusRequest(pRequest);
	return;
}

#ifdef CEPC

DWORD
hifIRQThread(LPVOID Context)
{
	SD_DEVICE_HANDLE hDevice = Context;
	A_STATUS         status;
    HIF_DEVICE      *device;

	if (hDevice == NULL) {
		return SD_API_STATUS_UNSUCCESSFUL;
	}
	while (1) {
		NdisWaitEvent(&hifIRQEvent, 0);
		NdisResetEvent(&hifIRQEvent);
		device = getHifDevice(hDevice);
		status = htcCallbacks.dsrHandler(device);
		AR_DEBUG_ASSERT(status == A_OK);
	} // while
    return SD_API_STATUS_SUCCESS;
}

#endif

SD_API_STATUS 
hifIRQHandler(SD_DEVICE_HANDLE hDevice, PVOID notUsed)
{
#ifndef CEPC
	A_STATUS         status;
    HIF_DEVICE      *device;
	A_BOOL			 callDSR;
#endif

    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifIRQHandler : Enter\n");

#ifndef CEPC
	/* Disable device interrupts */
	device = getHifDevice(hDevice);

	status = htcCallbacks.deviceInterruptDisabler(device, &callDSR);
	AR_DEBUG_ASSERT(status == A_OK);

	/* Call the DSR Handler if it is not a Spurious Interrupt */
	if (callDSR) {
		status = htcCallbacks.dsrHandler(device);
		AR_DEBUG_ASSERT(status == A_OK);
	}
#else
	NdisSetEvent(&hifIRQEvent);
#endif
	HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifIRQHandler : Exit\n");
	return SD_API_STATUS_SUCCESS;
}

BOOL
hifDeviceInserted(SD_DEVICE_HANDLE *handle)
{
    HIF_DEVICE                 *device;
    SD_API_STATUS               sdStatus;
	SDIO_CARD_INFO              sdioInfo;
	SD_HOST_BLOCK_CAPABILITY    blockCap;
	SD_CARD_RCA                 cardRCA;
	A_UCHAR                     rgucTuple[SD_CISTPLE_MAX_BODY_SIZE];
    PSD_CISTPL_FUNCE_FUNCTION   pFunce = (PSD_CISTPL_FUNCE_FUNCTION) rgucTuple;
    A_UINT32                    ulLength = 0;
	A_UCHAR                     ucRegVal;
	A_BOOL                      blockMode;
    SD_CARD_INTERFACE           ci;
    SD_IO_FUNCTION_ENABLE_INFO  fData;
	DWORD                       bData;
    //SDCONFIG_FUNC_SLOT_CURRENT_DATA slotCurrent;
#ifdef CEPC
	HANDLE                      hThread;
#endif //CEPC


    device = addHifDevice(handle);
    HIF_DEBUG_PRINTF(ATH_LOG_TRC, "hifDeviceInserted: Enter\n");

	/* Enable SDIO [dragon] function */
    fData.Interval = DEFAULT_SDIO_FUNCTION_RETRY_TIMEOUT;
    fData.ReadyRetryCount = DEFAULT_SDIO_FUNCTION_RETRIES;

    sdStatus = SDSetCardFeature (handle, SD_IO_FUNCTION_ENABLE,
                                  &fData, sizeof(fData));
    if (!SD_API_SUCCESS(sdStatus)) {
        return FALSE;
    }

    /* 
	 * Issue commands to get the manufacturer ID and stuff and compare it 
	 * against the rev Id derived from the ID registered during the 
	 * initialization process. Report the device only in the case there 
	 * is a match.
	 */

	sdStatus = SDCardInfoQuery(handle, SD_INFO_SDIO, 
							&sdioInfo, sizeof(sdioInfo));
    if (!SD_API_SUCCESS(sdStatus)) {
        return FALSE;
    }
    funcNo = sdioInfo.FunctionNumber;

	sdStatus = SDCardInfoQuery(handle, SD_INFO_REGISTER_RCA, 
		                        &cardRCA, sizeof(cardRCA));
    HIF_DEBUG_PRINTF(ATH_LOG_INF, "Card RCA  is 0x%x\n", cardRCA);

    /* Configure the SDIO Bus Width */
	memset(&ci, 0, sizeof(ci));
    if (sdio1bitmode) {
		ci.InterfaceMode = SD_INTERFACE_SD_MMC_1BIT;
	} else {
		ci.InterfaceMode = SD_INTERFACE_SD_4BIT;
	}
	if (sdiobusspeedlow) {
		ci.ClockRate = SDIO_CLOCK_FREQUENCY_REDUCED;
	} else {
		ci.ClockRate = SDIO_CLOCK_FREQUENCY_DEFAULT;
	}
	sdStatus = SDSetCardFeature(handle, SD_SET_CARD_INTERFACE,
		                        &ci, sizeof(ci));

    if (!SD_API_SUCCESS(sdStatus)) {
		return FALSE;
	}
        
	/* Check if the target supports block mode */
	sdStatus = SDReadWriteRegistersDirect(handle, SD_IO_READ, 
					0, 0x08, FALSE, &ucRegVal, 1);
    if (!SD_API_SUCCESS(sdStatus)) {
        return FALSE;
    }
    blockMode = (ucRegVal & 0x2) >> 1; // SMB is bit 1
	if (!blockMode) {
		HIF_DEBUG_PRINTF(ATH_LOG_ERR, "Function does not support block mode\n");
		return FALSE;
	} else {
		HIF_DEBUG_PRINTF(ATH_LOG_TRC, "Function supports block mode\n");

		blockCap.ReadBlocks = blockCap.WriteBlocks = 8;
		blockCap.ReadBlockSize = blockCap.WriteBlockSize = HIF_MBOX_BLOCK_SIZE;

		sdStatus = SDCardInfoQuery(handle, SD_INFO_HOST_BLOCK_CAPABILITY, 
						&blockCap, sizeof(blockCap));

		if (blockCap.ReadBlockSize < blockCap.WriteBlockSize) {
			maxBlockSize = blockCap.ReadBlockSize;
		} else {
			maxBlockSize = blockCap.WriteBlockSize;
		}
		if (blockCap.ReadBlocks < blockCap.WriteBlocks) {
			maxBlocks = blockCap.ReadBlocks;
		} else {
			maxBlocks = blockCap.WriteBlocks;
		}

		sdStatus = SDGetTuple(handle, SD_CISTPL_FUNCE, NULL, &ulLength, FALSE);
		if ((!SD_API_SUCCESS(sdStatus)) || (ulLength > sizeof(rgucTuple)) ) {
			return FALSE;
		}
		sdStatus = SDGetTuple(handle, SD_CISTPL_FUNCE, rgucTuple, &ulLength, FALSE);
		if ((!SD_API_SUCCESS(sdStatus)) || 
			(pFunce->bType != SD_CISTPL_FUNCE_FUNCTION_TYPE) ) {
			return FALSE;
		}

		if (maxBlockSize > pFunce->wMaxBlkSize) {
			maxBlockSize = pFunce->wMaxBlkSize;
		}

		bData = (DWORD)maxBlockSize;
		sdStatus = SDSetCardFeature(handle, 
			SD_IO_FUNCTION_SET_BLOCK_SIZE, 
			&bData, sizeof(bData));

	}
    HIF_DEBUG_PRINTF(ATH_LOG_TRC,
		           "Bytes Per Block: %d bytes, Block Count:%d \n", 
                   maxBlockSize, maxBlocks);
    /* Allocate the slot current */
	/* Kowsalya : commenting as there is no equivalent for this in WINCE */
	/*
    status = SDLIB_GetDefaultOpCurrent(handle, &slotCurrent.SlotCurrent);
    if (SDIO_SUCCESS(status)) {
        HIF_DEBUG_PRINTF(ATH_DEBUG_TRACE, ("Allocating Slot current: %d mA\n", 
                                slotCurrent.SlotCurrent));         
        status = SDLIB_IssueConfig(handle, SDCONFIG_FUNC_ALLOC_SLOT_CURRENT,
                                   &slotCurrent, sizeof(slotCurrent));
        if (!SDIO_SUCCESS(status)) {
            HIF_DEBUG_PRINTF(ATH_DEBUG_ERROR, 
                            ("Failed to allocate slot current %d\n", status));
            return FALSE;
        }
    }
	*/

    /* Inform HTC */
    if ((htcCallbacks.deviceInsertedHandler(device)) != A_OK) {
        HIF_DEBUG_PRINTF(ATH_LOG_TRC, "Device rejected\n");
        return FALSE;
    }

#ifdef CEPC
	NdisInitializeEvent(&hifIRQEvent);
	hThread = CreateThread(NULL, 0, 
		hifIRQThread, (LPVOID)handle, 0, NULL);
	CeSetThreadPriority(hThread, 200);
	CloseHandle(hThread);
#endif //CEPC
	return TRUE;
}

void
HIFAckInterrupt(HIF_DEVICE *device)
{
#ifndef CEPC
	htcCallbacks.deviceInterruptEnabler(device);
#endif
    return;
}

void
HIFUnMaskInterrupt(HIF_DEVICE *device)
{
    SD_API_STATUS sdStatus;

    /* Register the IRQ Handler */
	sdStatus = SDIOConnectInterrupt(device->handle, hifIRQHandler);
	AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));
	return;
}

void HIFMaskInterrupt(HIF_DEVICE *device)
{
    SDIODisconnectInterrupt(device->handle);
	return;
}

void
hifDeviceRemoved(SD_DEVICE_HANDLE *handle)
{
    A_STATUS    status;
    HIF_DEVICE *device;

    device = getHifDevice(handle);
    status = htcCallbacks.deviceRemovedHandler(device);
    delHifDevice(handle);
    AR_DEBUG_ASSERT(status == A_OK);
	return;
}

HIF_DEVICE *
addHifDevice(SD_DEVICE_HANDLE *handle)
{
    hifDevice[0].handle = handle;
    return &hifDevice[0];
}

HIF_DEVICE *
getHifDevice(SD_DEVICE_HANDLE *handle)
{
    return &hifDevice[0];
}

void
delHifDevice(SD_DEVICE_HANDLE *handle)
{
    hifDevice[0].handle = NULL;
}
