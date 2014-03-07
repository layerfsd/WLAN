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
#include <ndis.h>
#include "ar6000_sdio.h"
#include "hif_internal.h"


#define _P	L"\r[HIF] "L

enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0,
}HifDebug;

#if 0
struct hif_device {
    SD_DEVICE_HANDLE *handle;
	void     *claimedContext;
	HTC_CALLBACKS htcCallbacks;
};
#endif

#define USE_IRQ_THREAD		0


#if USE_IRQ_THREAD
NDIS_EVENT	 hifIRQEvent;
#endif

CRITICAL_SECTION gCriticalSection ;

//EnterCriticalSection(&gCriticalSection);
//LeaveCriticalSection(&gCriticalSection);
//DeleteCriticalSection(&gCriticalSection);



static BOOL
hifDeviceInserted(SD_DEVICE_HANDLE *device);

static void
hifDeviceRemoved(SD_DEVICE_HANDLE *device);
#if 0
void 
hifRWCompletionHandler(SD_DEVICE_HANDLE  hDevice,
					   PSD_BUS_REQUEST   pRequest,
					   PVOID             notUsed,
					   DWORD             dwParam);
#endif

static SD_API_STATUS 
hifIRQHandler(SD_DEVICE_HANDLE hDevice, PVOID notUsed);

static HIF_DEVICE *
addHifDevice(SD_DEVICE_HANDLE *handle);

static HIF_DEVICE *
getHifDevice(SD_DEVICE_HANDLE *handle);

static void
delHifDevice(SD_DEVICE_HANDLE *handle);


static BUS_REQUEST *hifAllocateBusRequest(HIF_DEVICE *device);
static void hifFreeBusRequest(HIF_DEVICE *device, BUS_REQUEST *busrequest);

static A_STATUS PowerChangeNotify(HIF_DEVICE *device, HIF_DEVICE_POWER_CHANGE_TYPE PowerChange);
static A_STATUS ReinitSDIO(HIF_DEVICE *device);


#define SLOTPOWER_DEV_NAME_REG_VAL_KEY  TEXT("SlotPowerDevName")
#define SLOTPOWER_OFF_IOCTL_REG_VAL_KEY TEXT("WifiModuleOffIoctl")
#define SLOTPOWER_ON_IOCTL_REG_VAL_KEY  TEXT("WifiModuleOnIoctl")
static void SetupSlotPowerControl(HIF_DEVICE *device);
static void CleanupSlotPowerControl(HIF_DEVICE *device);
static BOOL PowerUpDownSlot(HIF_DEVICE *device, A_BOOL PowerUp);


//static void CleanupSlotPowerControl(HIF_DEVICE *device);



/* ------ Static Variables ------ */

static SD_FUNCTION   sdFunction;
static OSDRV_CALLBACKS osdrvCallbacks;

HIF_DEVICE    hifDevice[HIF_MAX_DEVICES];
HTC_CALLBACKS htcCallbacks;
A_UINT16      maxBlocks;
A_UINT16      maxBlockSize;
A_UINT8       funcNo = 0;

// bluebird start
/* make sure we only unregister when registered. */
static int registered = 0;

static     A_MUTEX_T                       hif_lock;

#define LOCK_HIF()     A_MUTEX_LOCK(&hif_lock)
#define UNLOCK_HIF()   A_MUTEX_UNLOCK(&hif_lock)

static NDIS_SPIN_LOCK						sLock;

#define Hif_Lock() 		NdisAcquireSpinLock(&sLock)
#define Hif_Unlock() 	NdisReleaseSpinLock(&sLock)


// bluebird end.

/* ------ Functions ------ */
#ifdef ATHR_CE_LEGACY
A_STATUS HIFInit(OSDRV_CALLBACKS *callbacks)
{
	SD_API_STATUS sdStatus;
	
    AR_DEBUG_ASSERT(callbacks != NULL);

    /* store the callback handlers */
    osdrvCallbacks = *callbacks;

    /* Register with bus driver core */
    NDIS_DEBUG_PRINTF(DBG_LEVEL_HIF, "AR6000: HIFInit registering \r\n");
    registered = 1;
#if defined(CONFIG_PM)
	// TODO about power management.
#endif /* CONFIG_PM */

    /* Register with bus driver core */
    sdFunction.pName      = "sdio_wlan";
    sdFunction.pProbe     = hifDeviceInserted;
    sdFunction.pRemove    = hifDeviceRemoved;

	
	A_MUTEX_INIT(&hif_lock);
	NdisAllocateSpinLock(&sLock);
	
    sdStatus = SDIORegisterFunction(&sdFunction);
    AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));	
    
    return A_OK;
}
#else
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
#endif



static A_STATUS 
__HIFReadWrite(HIF_DEVICE		*device, 
             A_UINT32		address, 
             A_UCHAR		*buffer, 
             A_UINT32		length, 
             A_UINT32		request, 
             void			*context) 
{
    A_UINT8             rw;
    A_UINT8             mode;
    A_UINT8             opcode;
    A_UINT32            blockLen, blockCount, count;
    PSD_BUS_REQUEST     busRequest=NULL;
    A_STATUS            status = A_OK;
	SD_TRANSFER_CLASS   transferClass;
	DWORD               dwArg;
	A_UCHAR             command;
	SD_API_STATUS       sdStatus;
	SD_COMMAND_RESPONSE response;
 
    NDIS_DEBUG_PRINTF(0, "%s() :  Enter \r\n",__FUNCTION__);

	if(context != NULL)
		NDIS_DEBUG_PRINTF(0, "%s() : context = 0x%08x \r\n", __FUNCTION__,context);
	
	if ((request & HIF_BLOCK_BASIS) && (!(request & HIF_EXTENDED_IO))) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "Block mode not allowed for this type of command\r\n");
		return A_ERROR;
	}

    if (request & HIF_BLOCK_BASIS) 
	{
		mode = SD_IO_BLOCK_MODE;
		blockLen = HIF_MBOX_BLOCK_SIZE;
		blockCount = length / HIF_MBOX_BLOCK_SIZE;
		count = blockCount; 
	} 
	else if (request & HIF_BYTE_BASIS) 
	{
		mode = SD_IO_BYTE_MODE;
		blockLen = length;
		blockCount = 1;
		count = blockLen; 
    } 
	else 
	{
		NDIS_DEBUG_PRINTF(DBG_ERR, "Invalid data mode: %08x\r\n", request);
        return A_ERROR;
	}

    if (request & HIF_FIXED_ADDRESS) 
	{
		opcode = SD_IO_FIXED_ADDRESS;
		NDIS_DEBUG_PRINTF(DBG_TRACE, "Fixed       ");
    } 
	else if (request & HIF_INCREMENTAL_ADDRESS) 
	{
		opcode = SD_IO_INCREMENT_ADDRESS;
		NDIS_DEBUG_PRINTF(0, "Incremental ");
    } 
	else 
	{
        NDIS_DEBUG_PRINTF(DBG_ERR, "Invalid address mode: %08x\r\n", request);
        return A_ERROR;
    }

    if (request & HIF_WRITE) {
        transferClass = SD_WRITE;
		rw = SD_IO_OP_WRITE;
		if ((address >= HIF_MBOX_START_ADDR(0)) && 
            (address <= HIF_MBOX_END_ADDR(3))) {
            /* Mailbox write. Adjust the address so that the last byte 
               falls on the EOM address */
            address = address + HIF_MBOX_WIDTH - length;
        }
        NDIS_DEBUG_PRINTF(0, "[Write]");
	} else {
		transferClass = SD_READ;
		rw = SD_IO_OP_READ;
		NDIS_DEBUG_PRINTF(0, "[Read ] \r\n");
	}
        

	if (request & HIF_EXTENDED_IO) {
		dwArg = BUILD_IO_RW_EXTENDED_ARG(rw, mode, funcNo, 
			address, opcode, count);
		command = SD_IO_RW_EXTENDED;

    } 
	else if (request & HIF_BASIC_IO) 
	{
		dwArg = BUILD_IO_RW_DIRECT_ARG(rw, SD_IO_RW_NORMAL, funcNo, address, 0);
		command = SD_IO_RW_NORMAL;

	} 
	else
	{
        NDIS_DEBUG_PRINTF(DBG_ERR, "Invalid command type: %08x\r\n", request);
        return A_ERROR;
	}

	if((request & HIF_READ) && (dwArg > 0x14000000)) 
	{
        NDIS_DEBUG_PRINTF(DBG_TRACE, "Synchronous, command = %d, dwArg = 0x%08x, funcNo=%d \r\n", command, dwArg,funcNo);
		NDIS_DEBUG_PRINTF(0, "blockCount=%d,blockLen=%d \r\n", blockCount, blockLen);
	}

	
		
	sdStatus = SDSynchronousBusRequest(device->handle, command, dwArg,
				transferClass, ResponseR5, &response, blockCount, 
				blockLen, buffer, 0);
		
	if (!SD_API_SUCCESS(sdStatus)) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "SDBusRequest failed 0x%x, device->handle = 0x%x, buffer = 0x%x\r\n", sdStatus, device->handle, buffer);
		NDIS_DEBUG_PRINTF(DBG_ERR, "1[%d]2[%d]3[%d]4[%d]5[%d]6[%d] \r\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
		status = A_ERROR;
	}

	if(status != A_OK)
	    NDIS_DEBUG_PRINTF(DBG_ERR, "%s() : Status = %d - Exit \r\n",__FUNCTION__, status);
	
    return status;
}


#define HIF_SYNCHRONOUS_WAIT_TIME 30000


A_STATUS 
HIFReadWrite(HIF_DEVICE		*device, 
             A_UINT32		address, 
             A_UCHAR		*buffer, 
             A_UINT32		length, 
             A_UINT32		request, 
             void			*context) 
{

    A_STATUS            status = A_OK;
	//unsigned long 		flags;
	BUS_REQUEST 		*busrequest;
	BUS_REQUEST 		*async;
	BUS_REQUEST 		*active;


	AR_DEBUG_ASSERT(device != NULL);
	
	
	
	do 
	{
		
		
		if ((request & HIF_ASYNCHRONOUS) || (request & HIF_SYNCHRONOUS))
		{
		
			/* serialize all requests through the async thread */
			NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6000: Execution mode: %s\n", (request & HIF_ASYNCHRONOUS) ?"Async":"Synch");

			busrequest = hifAllocateBusRequest(device);
			if (busrequest == NULL) {
				NDIS_DEBUG_PRINTF(DBG_ERR, "AR6000: no async bus requests available\n");
				return A_ERROR;
			}

			Hif_Lock();
			
			busrequest->address = address;
			busrequest->buffer = buffer;
			busrequest->length = length;
			busrequest->request = request;
			busrequest->context = context;

			//NDIS_DEBUG_PRINTF(1, " add = %x, length = %x, request = %d \r\n", address, length, request);
					
			/* add to async list */
			active = device->asyncreq;
			if (active == NULL) 
			{
				device->asyncreq = busrequest;
				device->asyncreq->inusenext = NULL;
			} 
			else 	
			{
				for (async = device->asyncreq; async != NULL; async = async->inusenext) {
					 active =  async;
				}
				active->inusenext = busrequest;
				busrequest->inusenext = NULL;
			}
			
			Hif_Unlock();
			
		    if (request & HIF_SYNCHRONOUS) 
			{

				NdisSetEvent(&device->sem_async);

				/* Wait Read/Write Complete from the async_task */
				if( ! NdisWaitEvent(&busrequest->sem_req, HIF_SYNCHRONOUS_WAIT_TIME)) {
					NDIS_DEBUG_PRINTF(DBG_ERR, " HIF Synchronous Read/Write Time Out !! \r\n");
					NdisResetEvent(&busrequest->sem_req);
					
					return A_ERROR;
				} 

				NdisResetEvent(&busrequest->sem_req);
				hifFreeBusRequest(device, busrequest);	
				
				if(busrequest->status != A_OK)
					NDIS_DEBUG_PRINTF(DBG_ERR, "%s() Read/Write Err \r\n", __FUNCTION__);
				
				
				return busrequest->status;
			} 
			else 
			{
				NdisSetEvent(&device->sem_async);	
				
				return  A_PENDING;
		  	}
			
		}
		else
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, "AR6000: Invalid execution mode: 0x%08x\n", (unsigned int)request);
            status = A_EINVAL;
            break;			
		}
		
	}while(0);
    NDIS_DEBUG_PRINTF(0, "%s() :  - Exit \r\n",__FUNCTION__);
    return status;
}


static int async_task(void *param)
{
	HIF_DEVICE      *device;
	BUS_REQUEST 	*request;
	A_STATUS 		status;

	static int		rw_cnt = 0;
	
	device = (HIF_DEVICE *)param;

	
	while(!device->async_shutdown)
	{

		/* wait for work */
		NdisWaitEvent(&device->sem_async, 0);
		NdisResetEvent(&device->sem_async);

		if (device->async_shutdown) {
			NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6000: async task stopping\n");
			break;
		}		
		
		/* pull the request to work on */
		while (device->asyncreq != NULL) 
		{
			request = device->asyncreq;
	
			if (request->inusenext != NULL) 
				device->asyncreq = request->inusenext;
			else 
				device->asyncreq = NULL;
			
			/* call HIFReadWrite in sync mode to do the work */
			NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6000: async_task processing req: 0x%X \r\n", (unsigned int)request);

			rw_cnt++;

			if(rw_cnt > 1)
				NDIS_DEBUG_PRINTF(DBG_ERR, "%s() : two time call !!!!! \r\n", __FUNCTION__);
			
			status = __HIFReadWrite(device, request->address, request->buffer,
			                      request->length, request->request & ~HIF_SYNCHRONOUS, NULL);
			rw_cnt --;
			if (request->request & HIF_ASYNCHRONOUS) 
			{
				NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6000: async_task completion routine req: 0x%X\n", (unsigned int)request);
				device->htcCallbacks.rwCompletionHandler(request->context, status);
				NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6000: async_task freeing req: 0x%X\n", (unsigned int)request);
				hifFreeBusRequest(device, request);
			} 
			else 
			{
			
				NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6000: async_task upping req: 0x%X\n", (unsigned int)request);
				request->status = status;
				NdisSetEvent(&request->sem_req);			
			}


        }

	}

	return 0;
}


void
HIFShutDownDevice(HIF_DEVICE *device)
{
    SD_API_STATUS sdStatus;
    //SDCONFIG_BUS_MODE_DATA busSettings;
	UCHAR         buffer;

	NDIS_DEBUG_PRINTF(DBG_LEVEL_HIF, "%s() : + Enter \r\n", __FUNCTION__);

	if (device == NULL) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "Invalid Handle passed\r\n");
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

#if 0
void hifRWCompletionHandler(SD_DEVICE_HANDLE  hDevice,
					   PSD_BUS_REQUEST   pRequest,
					   PVOID             notUsed,
					   DWORD             dwParam) 
{

    PVOID    htcContext = (void *)dwParam;
	A_STATUS status = A_OK;

	if(SD_API_SUCCESS(pRequest->Status)) {
		status = A_OK;
	} else {
		status = A_ERROR;
	}

	if(htcContext != NULL) 
	{
		htcCallbacks.rwCompletionHandler(htcContext, A_OK);
	}


    SDFreeBusRequest(pRequest);
	
	NDIS_DEBUG_PRINTF(DBG_TRACE, "%s() : - Exit \r\n", __FUNCTION__);

}
#endif


#if USE_IRQ_THREAD
DWORD
hifIRQThread(void *Context)
{

	A_STATUS         status = A_OK;
	HIF_DEVICE      *device;

	device = (HIF_DEVICE *)Context;

	NDIS_DEBUG_PRINTF(DBG_LEVEL_HIF, " %s() is Running... \r\n",__FUNCTION__);


	while (TRUE) 
	{
		NdisWaitEvent(&hifIRQEvent, 0);
		NdisResetEvent(&hifIRQEvent);
		
		NDIS_DEBUG_PRINTF(DBG_TRACE, "%s() - Running  & ",__FUNCTION__);
		status = device->htcCallbacks.dsrHandler(device->htcCallbacks.context);
		AR_DEBUG_ASSERT(status == A_OK);
		NDIS_DEBUG_PRINTF(DBG_TRACE, " Exit \r\n");
		
		SDIOConnectInterrupt(device->handle,hifIRQHandler);
				
	} // while
	return SD_API_STATUS_SUCCESS;
	
}
#endif


SD_API_STATUS 
hifIRQHandler(SD_DEVICE_HANDLE hDevice, PVOID notUsed)
{
#ifndef CEPC
	A_STATUS         status = A_OK;
    HIF_DEVICE      *device = NULL;
//	A_BOOL			 callDSR;
#endif

	NDIS_DEBUG_PRINTF(DBG_TRACE, "hifIRQHandler : Enter\r\n");

	device = getHifDevice(hDevice);
#if USE_IRQ_THREAD
	

	SDIODisconnectInterrupt(device->handle);

	NdisSetEvent(&hifIRQEvent);
	
#else
	/* Disable device interrupts */
	SDIODisconnectInterrupt(device->handle);
	
#if 0	
	/* Call the DSR Handler if it is not a Spurious Interrupt */
	if (callDSR) 
#endif		
	{
		status = device->htcCallbacks.dsrHandler(device->htcCallbacks.context);	
		AR_DEBUG_ASSERT(status == A_OK);
	}
	
	SDIOConnectInterrupt(device->handle,hifIRQHandler);
		
#endif

	NDIS_DEBUG_PRINTF(0, "hifIRQHandler : Exit\r\n");	
	return SD_API_STATUS_SUCCESS;
}



static BOOL SetupSDIOInterface(HIF_DEVICE *device)
{
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
    BOOL                        doInterfaceChange = FALSE;
    SD_DEVICE_HANDLE            handle;
	int 						count;

    handle = device->handle;

	NDIS_DEBUG_PRINTF(1, "%s() : Enter + \r\n",__FUNCTION__);	

    /* Enable SDIO [dragon] function */
    fData.Interval = DEFAULT_SDIO_FUNCTION_RETRY_TIMEOUT;
    fData.ReadyRetryCount = DEFAULT_SDIO_FUNCTION_RETRIES;

    sdStatus = SDSetCardFeature (handle, SD_IO_FUNCTION_ENABLE,
                                  &fData, sizeof(fData));
    if (!SD_API_SUCCESS(sdStatus)) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "SDSetCardFeature (0x%x)\n", sdStatus);
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
		NDIS_DEBUG_PRINTF(DBG_ERR, "SDCardInfoQuery (0x%x) \r\n", sdStatus);
        return FALSE;
    }
    funcNo = sdioInfo.FunctionNumber;

    sdStatus = SDCardInfoQuery(handle, SD_INFO_REGISTER_RCA,
                                &cardRCA, sizeof(cardRCA));
    NDIS_DEBUG_PRINTF(1, "Card RCA  is 0x%x \r\n", cardRCA);

    /* Configure the SDIO Bus Width */
    memset(&ci, 0, sizeof(ci));

        /* get current interface settings */
    sdStatus = SDCardInfoQuery(handle, SD_INFO_CARD_INTERFACE,
                                &ci, sizeof(ci));

    if (!SD_API_SUCCESS(sdStatus)) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "%s() : ERR, SDCardInfoQuery Fail !!\r\n",__FUNCTION__);	
        return FALSE;
    }

    //DebugBreak();

#ifdef HIF_SDIO_1BIT
        /* force to 1 bit mode */
    sdio1bitmode = 1;
#endif

    if (sdio1bitmode && (ci.InterfaceMode != SD_INTERFACE_SD_MMC_1BIT)) {
            /* force to 1 bit mode */
        ci.InterfaceMode = SD_INTERFACE_SD_MMC_1BIT;
        doInterfaceChange = TRUE;
    }

        /* check for forced low speed operation */
    if (sdiobusspeedlow) {
            /* only set the lower of our current rate and our desired reduced rate,
             * otherwise we run at a clock rate that the bus driver setup for us */
        ci.ClockRate = min(ci.ClockRate, SDIO_CLOCK_FREQUENCY_REDUCED);
        doInterfaceChange = TRUE;
    }

    if (doInterfaceChange) {
        sdStatus = SDSetCardFeature(handle, SD_SET_CARD_INTERFACE,
                                    &ci, sizeof(ci));

        if (!SD_API_SUCCESS(sdStatus)) {
			NDIS_DEBUG_PRINTF(DBG_ERR, "%s() : SDSetCardFeature Fail !!\r\n",__FUNCTION__);	
            return FALSE;
        }
    }

    NDIS_DEBUG_PRINTF(DBG_TRACE, ("** SDIO Interface : %s : %d (%s)\r\n",
        doInterfaceChange ? "Overridden to" : "Set by busdriver",
        ci.ClockRate,
        (ci.InterfaceMode == SD_INTERFACE_SD_MMC_1BIT) ?
        "1-bit" : "4-bit"));

        /* save card interface mode to restore later */
    A_MEMCPY(&device->CardInterface, &ci, sizeof(ci));


    /* Check if the target supports block mode */
    sdStatus = SDReadWriteRegistersDirect(handle, SD_IO_READ,
                    0, 0x08, FALSE, &ucRegVal, 1);
	
    if (!SD_API_SUCCESS(sdStatus)) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "%s() : SDReadWriteRegistersDirect Fail !!\r\n",__FUNCTION__);	
        return FALSE;
    }
    blockMode = (ucRegVal & 0x2) >> 1; // SMB is bit 1
    if (!blockMode) {
        NDIS_DEBUG_PRINTF(DBG_ERR, ("Function does not support block mode\n"));
        return FALSE;
    } else {
        NDIS_DEBUG_PRINTF(DBG_TRACE, ("Function supports block mode \r\n"));

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

    NDIS_DEBUG_PRINTF(DBG_TRACE, "Bytes Per Block: %d bytes, Block Count:%d \r\n", maxBlockSize, maxBlocks);

    /* Initialize the bus requests to be used later */
    A_MEMZERO(device->busRequest, sizeof(device->busRequest));
    for (count = 0; count < BUS_REQUEST_MAX_NUM; count ++) {
		NdisInitializeEvent(&device->busRequest[count].sem_req);
		hifFreeBusRequest(device, &device->busRequest[count]);
    }	


    return TRUE;
}



BOOL
hifDeviceInserted(SD_DEVICE_HANDLE *handle)
{
	HIF_DEVICE                 *device;
#if 0
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
	//HANDLE                      hIrqThread;


#endif

	HANDLE                      hThread;

	device = addHifDevice(handle);
	NDIS_DEBUG_PRINTF(1, "hifDeviceInserted: Enter\r\n");

#if 0

	/* Enable SDIO [dragon] function */
	fData.Interval = DEFAULT_SDIO_FUNCTION_RETRY_TIMEOUT;
	fData.ReadyRetryCount = DEFAULT_SDIO_FUNCTION_RETRIES;

	sdStatus = SDSetCardFeature (handle, SD_IO_FUNCTION_ENABLE, &fData, sizeof(fData));
	if (!SD_API_SUCCESS(sdStatus)) 
	{
		return FALSE;
	}


	/* 
	* Issue commands to get the manufacturer ID and stuff and compare it 
	* against the rev Id derived from the ID registered during the 
	* initialization process. Report the device only in the case there 
	* is a match.
	*/

	sdStatus = SDCardInfoQuery(handle, SD_INFO_SDIO, &sdioInfo, sizeof(sdioInfo));
	if (!SD_API_SUCCESS(sdStatus)) 
	{
		return FALSE;
	}
	
	funcNo = sdioInfo.FunctionNumber;

	sdStatus = SDCardInfoQuery(handle, SD_INFO_REGISTER_RCA, &cardRCA, sizeof(cardRCA));
	NDIS_DEBUG_PRINTF(1, " Card RCA  is 0x%x \r\n", cardRCA);

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
	
	sdStatus = SDSetCardFeature(handle, SD_SET_CARD_INTERFACE, &ci, sizeof(ci));

	if (!SD_API_SUCCESS(sdStatus)) {
		return FALSE;
	}
        
	/* Check if the target supports block mode */
	sdStatus = SDReadWriteRegistersDirect(handle, SD_IO_READ, 0, 0x08, FALSE, &ucRegVal, 1);
    if (!SD_API_SUCCESS(sdStatus)) {
        return FALSE;
    }
	
    blockMode = (ucRegVal & 0x2) >> 1; // SMB is bit 1
    
	if (!blockMode) 
	{
		NDIS_DEBUG_PRINTF(DBG_ERR, "[HIF] Function does not support block mode \r\n");
		return FALSE;
	} 
	else 
	{
		NDIS_DEBUG_PRINTF(DBG_LEVEL_HIF, "[HIF] Function supports block mode \r\n");

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
		if ((!SD_API_SUCCESS(sdStatus)) || (pFunce->bType != SD_CISTPL_FUNCE_FUNCTION_TYPE) ) {
			return FALSE;
		}

		if (maxBlockSize > pFunce->wMaxBlkSize) {
			maxBlockSize = pFunce->wMaxBlkSize;
		}

		bData = (DWORD)maxBlockSize;
		sdStatus = SDSetCardFeature(handle,	SD_IO_FUNCTION_SET_BLOCK_SIZE, &bData, sizeof(bData));

	}
	
    NDIS_DEBUG_PRINTF(DBG_LEVEL_HIF,
						"Bytes Per Block: %d bytes, Block Count:%d \r\n", 
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

    /* Initialize the bus requests to be used later */
    A_MEMZERO(device->busRequest, sizeof(device->busRequest));
    for (count = 0; count < BUS_REQUEST_MAX_NUM; count ++) {
		NdisInitializeEvent(&device->busRequest[count].sem_req);
		hifFreeBusRequest(device, &device->busRequest[count]);
    }	

#else

	SetupSlotPowerControl(device);

	if (!SetupSDIOInterface(device)) {
		return FALSE;
	}

#endif


		
	InitializeCriticalSection(&gCriticalSection); //ÃÊ±âÈ­
	
	device->async_shutdown	= 0;
		
	hThread = CreateThread(NULL, 0, async_task, (void *)device, 0, NULL);
	CeSetThreadPriority(hThread, 200);
	CloseHandle(hThread);
		
	NdisInitializeEvent(&device->sem_async);
	
#if USE_IRQ_THREAD
	hThread = CreateThread(NULL, 0, hifIRQThread, (void *)device, 0, NULL);
	CeSetThreadPriority(hThread, 200);
	CloseHandle(hThread);	
	
	NdisInitializeEvent(&hifIRQEvent);
#endif



	/* start  up inform DRV layer */
	if ((osdrvCallbacks.deviceInsertedHandler(osdrvCallbacks.context,device)) != A_OK) 	{
		NDIS_DEBUG_PRINTF(DBG_ERR, "[HIF] AR6000: Device rejected \r\n");
	}
	
	NDIS_DEBUG_PRINTF(DBG_LEVEL_HIF, " %s() -Exit \r\n",__FUNCTION__);
	return TRUE;
}

void
HIFAckInterrupt(HIF_DEVICE *device)
{

#if 0 //bluebird
	htcCallbacks.deviceInterruptEnabler(device);
#endif

    AR_DEBUG_ASSERT(device != NULL);
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



static BUS_REQUEST *hifAllocateBusRequest(HIF_DEVICE *device)
{
    BUS_REQUEST *busrequest;
//    unsigned long flag;

    /* Acquire lock */
	//spin_lock_irqsave(&device->lock, flag);

    /* Remove first in list */
    if((busrequest = device->s_busRequestFreeQueue) != NULL)
    {
        device->s_busRequestFreeQueue = busrequest->next;
    }

    /* Release lock */
    //spin_unlock_irqrestore(&device->lock, flag);
	
    NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6000: hifAllocateBusRequest: 0x%p\n", busrequest);
    return busrequest;
}

static void
hifFreeBusRequest(HIF_DEVICE *device, BUS_REQUEST *busrequest)
{
 //   unsigned long flag;

    AR_DEBUG_ASSERT(busrequest != NULL);
    NDIS_DEBUG_PRINTF(0, "AR6000: hifFreeBusRequest: 0x%p\n", busrequest);
    /* Acquire lock */
	//spin_lock_irqsave(&device->lock, flag);
 
    /* Insert first in list */
    busrequest->next = device->s_busRequestFreeQueue;
    busrequest->inusenext = NULL;
    device->s_busRequestFreeQueue = busrequest;

    /* Release lock */
	//spin_unlock_irqrestore(&device->lock, flag);
}




void
hifDeviceRemoved(SD_DEVICE_HANDLE *handle)
{
	A_STATUS    status;
	HIF_DEVICE *device;

	NDIS_DEBUG_PRINTF(1, "%s() : + Enter \r\n", __FUNCTION__);
	
	device = getHifDevice(handle);

	if (device->claimedContext != NULL) {
		status = osdrvCallbacks.deviceRemovedHandler(device->claimedContext, device);
	}

	delHifDevice(handle);
	AR_DEBUG_ASSERT(status == A_OK);
	return;
}

HIF_DEVICE *
addHifDevice(SD_DEVICE_HANDLE *handle)
{
	NDIS_DEBUG_PRINTF(DBG_LEVEL_HIF, " %s() : handle = 0x%0x \r\n", __FUNCTION__, handle);
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


void HIFReleaseDevice(HIF_DEVICE  *device)
{
    device->claimedContext = NULL;    
}


void HIFClaimDevice(HIF_DEVICE  *device, void *context) {
    device->claimedContext = context;
}



A_STATUS HIFAttachHTC(HIF_DEVICE *device, HTC_CALLBACKS *callbacks) {

	if (device->htcCallbacks.context != NULL) {
		/* already in use! */
		return A_ERROR;
	}
	
	device->htcCallbacks = *callbacks;

	//htcCallbacks.rwCompletionHandler = callbacks->rwCompletionHandler;
	return A_OK;
}



void HIFDetachHTC(HIF_DEVICE *device) {
    A_MEMZERO(&device->htcCallbacks,sizeof(device->htcCallbacks));
}



A_STATUS
HIFConfigureDevice(HIF_DEVICE *device, HIF_DEVICE_CONFIG_OPCODE opcode,
                   void *config, A_UINT32 configLen)
{

	A_UINT32 count;
	A_STATUS status = A_OK;

	switch(opcode) 
	{
		case HIF_DEVICE_GET_MBOX_BLOCK_SIZE:
			((A_UINT32 *)config)[0] = HIF_MBOX0_BLOCK_SIZE;
			((A_UINT32 *)config)[1] = HIF_MBOX1_BLOCK_SIZE;
			((A_UINT32 *)config)[2] = HIF_MBOX2_BLOCK_SIZE;
			((A_UINT32 *)config)[3] = HIF_MBOX3_BLOCK_SIZE;
			break;

		case HIF_DEVICE_GET_MBOX_ADDR:
			for (count = 0; count < 4; count ++) {
				((A_UINT32 *)config)[count] = HIF_MBOX_START_ADDR(count);
			}
			break;
			
		case HIF_DEVICE_GET_IRQ_PROC_MODE:
			*((HIF_DEVICE_IRQ_PROCESSING_MODE *)config) = HIF_DEVICE_IRQ_SYNC_ONLY;
			break;
			
		case HIF_DEVICE_GET_OS_DEVICE:
	#if 0	//bluebird		
			/* pass back a pointer to the SDIO function's "dev" struct */
			((HIF_DEVICE_OS_DEVICE_INFO *)config)->pOSDevice = &device->func->dev;
	#else
			NDIS_DEBUG_PRINTF(DBG_ERR, "HIF_DEVICE_GET_OS_DEVICE isn't ready ~ must have to do something \r\n");
			return A_ERROR;
	#endif
	
			break; 

        case HIF_DEVICE_POWER_STATE_CHANGE:
            status = PowerChangeNotify(device, *(HIF_DEVICE_POWER_CHANGE_TYPE *)config);
            break;

	
		default:
			NDIS_DEBUG_PRINTF(DBG_ERR, "AR6000: Unsupported configuration opcode: %d \r\n", opcode);
			return A_ERROR;
			
	}

    return A_OK;
}



#define SDIO_ENABLE_REG         0x02
#define SDIO_READY_REG          0x03
#define SDIO_BUS_IF_REG         0x07
#define CARD_DETECT_DISABLE     0x80
#define SD_SDIO_R4_GET_OCR(pR) ((A_UINT32)((pR)[1])        |  \
                          (((A_UINT32)(pR)[2]) << 8)  |  \
                          (((A_UINT32)(pR)[3]) << 16))

#define SD_SDIO_R4_IS_CARD_READY(pR)       (((pR)[4] & 0x80) == 0x80)
#define SD_SDIO_R6_GET_RCA(pR)             ((A_UINT16)((pR)[3]) | ((A_UINT16)((pR)[4]) << 8))

static A_BOOL IssueSDCommand(SD_DEVICE_HANDLE     hDevice,
                             A_UINT8              Cmd,
                             A_UINT32             Argument,
                             SD_RESPONSE_TYPE     ResponseType,
                             SD_COMMAND_RESPONSE  *pResponse)
{
    SD_API_STATUS sdStatus;

    sdStatus = SDSynchronousBusRequest(hDevice,
                                       Cmd,
                                       Argument,
                                       SD_COMMAND,
                                       ResponseType,
                                       pResponse,
                                       0,
                                       0,
                                       NULL,
                                       0);

    return SD_API_SUCCESS(sdStatus) ? TRUE : FALSE;
}

static A_BOOL IssueCMD52(SD_DEVICE_HANDLE hDevice,
                         A_UINT32         Address,
                         A_UCHAR          Func,
                         A_BOOL           Write,
                         A_UINT8          *pData)
{
   SD_API_STATUS sdStatus;

   sdStatus = SDReadWriteRegistersDirect(hDevice,
                                         Write ? SD_IO_WRITE : SD_IO_READ,
                                         Func,
                                         Address,
                                         0,
                                         pData,
                                         1);

    return TRUE;
}



static A_STATUS ReinitSDIO(HIF_DEVICE *device)
{
    A_STATUS            status = A_OK;
    SD_COMMAND_RESPONSE response;
    A_UINT32            ocr;
    int                 cardReadyRetry = 10;
    A_UINT16            rca;
    A_UINT8             cmd52Data;

    do {

        if (!IssueSDCommand(device->handle,0,0,NoResponse,NULL)) {
            break;
        }

        AR_DEBUG_PRINTF(DBG_TRACE,("Issuing first CMD5 ... \n"));

            /* issue CMD5, ARG=0 */
        if (!IssueSDCommand(device->handle,5,0,ResponseR4,&response)) {
            AR_DEBUG_PRINTF(DBG_TRACE,("CMD5 FAILED, NO card detected \n"));
            break;
        }

        ocr = SD_SDIO_R4_GET_OCR(response.ResponseBuffer);

        AR_DEBUG_PRINTF(DBG_TRACE,("CMD5 success, OCR = 0x%X \n",ocr));

            /* issue CMD5, ARG=0x00100000 repeatedly until card is ready */
        while (cardReadyRetry) {
            if (!IssueSDCommand(device->handle,
                                5,
                                0x00100000, /* use 3.0 volts, although this really is a don't care */
                                ResponseR4,
                                &response)) {
                AR_DEBUG_PRINTF(DBG_ERR,("CMD5 Failed \n"));
                cardReadyRetry = 0;
                break;
            }

            if (SD_SDIO_R4_IS_CARD_READY(response.ResponseBuffer)) {
                AR_DEBUG_PRINTF(DBG_TRACE,("SDIO Card is Ready! \n"));
                break;
            }
            cardReadyRetry--;
            Sleep(10);
        }

        if (cardReadyRetry == 0) {
            AR_DEBUG_PRINTF(DBG_ERR,("CMD5 Ready polling Expired \r\n"));
            break;
        }

        AR_DEBUG_PRINTF(DBG_TRACE,("Issuing CMD3 ... \n"));

             /* issue CMD3 */
        if (!IssueSDCommand(device->handle,3,0,ResponseR6,&response)) {
            AR_DEBUG_PRINTF(DBG_ERR,("CMD3 FAILED \n"));
            break;
        }

        rca = SD_SDIO_R6_GET_RCA(response.ResponseBuffer);

        AR_DEBUG_PRINTF(DBG_TRACE,("Card RCA : 0x%X \n",rca));

        AR_DEBUG_PRINTF(DBG_TRACE,("Issuing CMD7 ... \n"));

              /* issue CMD7 to select card */
        if (!IssueSDCommand(device->handle, 7,(((A_UINT32)rca) << 16),ResponseR1b,&response)) {
            AR_DEBUG_PRINTF(DBG_ERR,("CMD7 FAILED \n"));
            break;
        }

        AR_DEBUG_PRINTF(DBG_TRACE,("Card is now selected! \n"));

            /* disable card detect resistor and restore operational bus width */
        cmd52Data = CARD_DETECT_DISABLE;
        if (device->CardInterface.InterfaceMode == SD_INTERFACE_SD_4BIT) {
                /* enable 4-bit mode */
            cmd52Data |= 0x2;
        }

        if (!IssueCMD52(device->handle,
                        SDIO_BUS_IF_REG,
                        0,
                        TRUE,
                        &cmd52Data)) {
            AR_DEBUG_PRINTF(DBG_ERR,("Failed to set bus mode register \r\n"));
            break;
        }

            /* complete interface setup */
        if (!SetupSDIOInterface(device)) {
            status = A_ERROR;
        }

    } while (FALSE);


    return status;
}



#define POWER_OFF 1
#define POWER_ON  0


static A_STATUS PowerChangeNotify(HIF_DEVICE *device, HIF_DEVICE_POWER_CHANGE_TYPE PowerChange)
{
    A_STATUS      status = A_OK;
    SD_API_STATUS sdStatus = SD_API_STATUS_SUCCESS;

	NDIS_DEBUG_PRINTF(1, "%s() : Enter (%d) + \r\n",__FUNCTION__,PowerChange);

    switch (PowerChange) {

        case HIF_DEVICE_POWER_UP:

            if (InterlockedExchange(&device->PowerStateOff, POWER_ON) != POWER_OFF) {
                    /* shouldn't be calling this if we are not powered off */
                A_ASSERT(FALSE);
                break;
            }

            if (device->SlotPowerRemoved) {
                    /* power was removed from a previous power down attempt */
                device->SlotPowerRemoved = FALSE;
                    /* try powering up */
                PowerUpDownSlot(device, TRUE);
            }

                /* Reinit SDIO since HIF_DEVICE_POWER_DOWN or HIF_DEVICE_POWER_OFF
                 * would have reset the SDIO interface */

            status = ReinitSDIO(device);

#ifdef HIF_SDIO_BYPASS
            if (A_SUCCESS(status)) {
                    /* re-activate */
                g_BypassModeActive = ActivateSDIOStackBypassMode();
            }
#endif
            break;

        case HIF_DEVICE_POWER_DOWN:
        case HIF_DEVICE_POWER_CUT:

                /* set powered off flag, but check previous value */
            if (InterlockedExchange(&device->PowerStateOff, POWER_OFF) != POWER_ON) {
                    /* shouldn't be calling this if we are powered off already  */
                A_ASSERT(FALSE);
                break;
            }

#ifdef HIF_SDIO_BYPASS
            g_BypassModeActive = FALSE;
                /* de-activeate bypass mode  */
            DeactivateSDIOStackBypassMode();
#endif

                /* disable I/O function and reset SDIO controller to minimize power */
                sdStatus = SDSetCardFeature(device->handle,
                                            SD_IO_FUNCTION_DISABLE,
                                            NULL,
                                            0);

                A_ASSERT(SD_API_SUCCESS(sdStatus));

                {
                    A_UINT8  buffer;
                        /* Perform a I/O reset, this causes the SDIO interface
                         * to be in the un-enumerated state */
                    buffer = 1 << 3;
                    sdStatus = SDReadWriteRegistersDirect(device->handle,
                                                          SD_IO_WRITE,
                                                          0,
                                                          SD_IO_REG_IO_ABORT,
                                                          0,
                                                          &buffer,
                                                          1);

                    A_ASSERT(SD_API_SUCCESS(sdStatus));
                }

            if (PowerChange == HIF_DEVICE_POWER_CUT) {
                    /* caller wants the device to be completely off */
                device->SlotPowerRemoved = PowerUpDownSlot (device, FALSE);
            }

            break;

        default:
            A_ASSERT(FALSE);
            return A_ERROR;
    }


    return status;
}


#define MAX_POWER_CONTROL_DEV_NAME 16

    /* get optional slot power control information from the registry */
static void SetupSlotPowerControl(HIF_DEVICE *device)
{
    TCHAR   deviceName[MAX_POWER_CONTROL_DEV_NAME + 1];

    if (NULL == device->hSlotPowerDevice) {
            /* first time through */
        device->hSlotPowerDevice = INVALID_HANDLE_VALUE;
    }

    if (device->hSlotPowerDevice != INVALID_HANDLE_VALUE) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "%s()-> INVALID_HANDLER !!!! \r\n",__FUNCTION__);	
        return;
    }

	NDIS_DEBUG_PRINTF(1, "%s() : Enter\r\n",__FUNCTION__);	

    A_MEMZERO(deviceName,sizeof(deviceName));

    do {

            /* get the device name */
        if (!GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                                HIF_SDIO_PARAMS_REG_PATH,
                                SLOTPOWER_DEV_NAME_REG_VAL_KEY,
                                (PUCHAR)deviceName,
                                MAX_POWER_CONTROL_DEV_NAME * (sizeof(TCHAR)))) {
        	NDIS_DEBUG_PRINTF(DBG_ERR," Slot power control device name : %s \r\n",deviceName);                        
            break;
        }


        device->hSlotPowerDevice = CreateFile(deviceName,
                                              GENERIC_READ | GENERIC_WRITE,
                                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                                              NULL,
                                              OPEN_EXISTING,
                                              FILE_ATTRIBUTE_NORMAL,
                                              0);

        if (INVALID_HANDLE_VALUE == device->hSlotPowerDevice) {
            NDIS_DEBUG_PRINTF(DBG_ERR,"Failed to open slot power control device (%d) \n",
                GetLastError());
            break;
        }

            /* get IOCTL codes for ON/OFF */
        GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                            HIF_SDIO_PARAMS_REG_PATH,
                            SLOTPOWER_OFF_IOCTL_REG_VAL_KEY,
                            (PUCHAR)&device->SlotPowerOffIoctl,
                            sizeof(device->SlotPowerOffIoctl));

        GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                            HIF_SDIO_PARAMS_REG_PATH,
                            SLOTPOWER_ON_IOCTL_REG_VAL_KEY,
                            (PUCHAR)&device->SlotPowerOnIoctl,
                            sizeof(device->SlotPowerOnIoctl));

        NDIS_DEBUG_PRINTF(DBG_ERR," Slot power IOCTL codes on:%d off:%d \r\n", device->SlotPowerOnIoctl,
                device->SlotPowerOffIoctl);

    } while (FALSE);

}



static void CleanupSlotPowerControl(HIF_DEVICE *device)
{
    if (INVALID_HANDLE_VALUE == device->hSlotPowerDevice) {
        return;
    }

    CloseHandle(device->hSlotPowerDevice);
    device->hSlotPowerDevice = INVALID_HANDLE_VALUE;
}


static BOOL PowerUpDownSlot(HIF_DEVICE *device, A_BOOL PowerUp)
{
    DWORD returnBytes;
    BOOL  retVal = FALSE;

	NDIS_DEBUG_PRINTF(1, "%s() : Enter (%d) + \r\n",__FUNCTION__,PowerUp);
	
    if (device->hSlotPowerDevice != INVALID_HANDLE_VALUE) {
        if (!DeviceIoControl(device->hSlotPowerDevice,
                             PowerUp ? device->SlotPowerOnIoctl : device->SlotPowerOffIoctl,
                             NULL,
                             0,
                             NULL,
                             0,
                             &returnBytes,
                             NULL)) {
            AR_DEBUG_PRINTF(DBG_ERR,("Failed to power %s slot \n", PowerUp ? "UP" : "DOWN"));
        } else {
            retVal = TRUE;
        }
    }

    return retVal;
}


