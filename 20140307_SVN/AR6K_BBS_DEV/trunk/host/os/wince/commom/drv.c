//------------------------------------------------------------------------------
// <copyright file="drv.c" company="Atheros">
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
// DRG inf functions
//
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include "osapi_wince.h"
#include "athtypes_wince.h"

#include "hif.h"
#include "htc_api.h"
//#include "stream_drv.h"
#include "common_drv.h"
#include "bmi.h"
#include "wince_common_drv.h"
#include "wmi.h"
#include "ar6xapi_wince.h"
#include "dbglog_api.h"
//#include "debug_wince.h"

enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0
}DrvDebug;


static HTC_HANDLE   gHTCHandle;
static A_UINT32     targetId;
static A_UINT32     targetType;
static HIF_DEVICE  *pHIFDevice;
static A_UINT8      logBuffer[DBGLOG_HOST_LOG_BUFFER_SIZE];
static A_UINT32     logCnt;
static A_UINT32     logBufferSize;
static HANDLE       hDbgHostEvent;

// Global debug flags
#ifdef DEBUG
A_INT32 debughtc = 0x3;  /* turn errors and warnings */
#endif



extern char data_patch_hw[1336];
extern char athwlan2_firmware[98324];


#define MAX_BUF (8*1024)
#define A_ROUND_UP(x, y)  ((((x) + ((y) - 1)) / (y)) * (y))


extern void eeprom_ar6000_transfer(HIF_DEVICE *device, char *fake_file, char *p_mac, int regcode, char *c_mac);


static void firmware_transfer(HIF_DEVICE *device, int file_id, A_UINT32 address, A_BOOL isCompressed)
{
	int ret;
	char * fw_buf = NULL, *src;
//	char tp_buf[1024 * 100];
		
	size_t length, length1, remaining, bufsize, bmisize;

	NDIS_DEBUG_PRINTF(DBG_TRACE," firmware_transfer(file_id = %d, address = 0x%08x, isCompressed = %d ) \r\n", file_id, address, isCompressed);

	//ar6000_readwrite_file(NULL, tp_buf, NULL, 98304);
		
	if(file_id == 1)		// ar6000 firmware 
		length = 94567;
	else if(file_id == 2)	//ar6000 patch file
		length = 1340;

	bufsize = (length + PAGE_SIZE) & (~(PAGE_SIZE-1));

	NDIS_DEBUG_PRINTF(DBG_TRACE, " firmware_transfer(): length = %d, bufsize = %d \r\n", length, bufsize);

	bmisize = A_ROUND_UP(length, 4);
	bufsize = max(bmisize, bufsize);
	fw_buf = (char*)malloc(bufsize);

	if(fw_buf == NULL)
	{
		NDIS_DEBUG_PRINTF(1," can't get fw_buff memory alloc !!! \r\n");
		return ;
	}

	if(file_id == 1)
		memcpy(fw_buf, athwlan2_firmware, 98324);
	else if(file_id == 2)
		memcpy(fw_buf, data_patch_hw, 1336);

	
	if ( isCompressed) 
	{
		ret = BMILZStreamStart(device, address);
		
		if (ret != A_OK) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, "firmware_transfer(): BMILZStreamStart failed, ret=%d \r\n", ret);
			goto Transfer_DONE;
		}
	}	

	remaining = bufsize;
	src = fw_buf;
	while (remaining>0) 
	{
		length = (remaining > MAX_BUF)? MAX_BUF : remaining;
		length1 = A_ROUND_UP(length, 4);
		
		if (isCompressed) 
		{
			NDIS_DEBUG_PRINTF(DBG_TRACE, "firmware_transfer(): BMILZData: len=%d, remaining=%d \r\n",length1, remaining-length);
			ret = BMILZData(device, (A_UCHAR *)src, length1);
			
			if (ret != A_OK) 
			{
				NDIS_DEBUG_PRINTF(DBG_ERR,"firmware_transfer(): BMILZData failed, ret=%d \r\n", ret);
				goto Transfer_DONE;
			}
		} 
		else 
		{
		
			ret = BMIWriteMemory(device, address, (A_UCHAR*)src, length1);
			
			if (ret != A_OK) 
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "firmware_transfer(): BMIWriteMemory failed, ret=%d \r\n",ret);
				goto Transfer_DONE;
			}
		}

		remaining -= length;
		address += length;
		src += length;
	}
Transfer_DONE:
	if (fw_buf) 
	{
		free(fw_buf);
	}

	NDIS_DEBUG_PRINTF(DBG_TRACE, "Firmware transfer complete !! \r\n");

	return;
	
}


A_STATUS DownloadImage(void)
{

    A_STATUS ret = A_OK;


#if 1 //bluebired defined(ANDROID_ENV)
    //if (tgt_fw != NULL) 
	{
		A_UINT32 value, old_options, old_sleep;
		A_UCHAR	mac_addr[6];
		/* temporarily disable system sleep */
		value = 0;
		BMIReadSOCRegister(pHIFDevice, 0x180c0, &value);
		old_options = value;
		value |= 0x08;
		BMIWriteSOCRegister(pHIFDevice, 0x180c0, value);
		value = 0;
		BMIReadSOCRegister(pHIFDevice, 0x40c4, &value);
		old_sleep = value;
		value |= 0x01;
		BMIWriteSOCRegister(pHIFDevice, 0x40c4, value);
		NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "old options [%d] old sleep [%d]\n", old_options, old_sleep);

		/* run at 40/44MHz by default */
		value = 0;
		BMIWriteSOCRegister(pHIFDevice, 0x4020, value);

		/* Set hi_refclk_hz */
//		NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "Set hi_refclk_hz : Ref Clock=%d\n", m_RefClock);
		BMIWriteSOCRegister(pHIFDevice, 0x500478, 26000000);

		/* use internal clock? */
		BMIReadSOCRegister(pHIFDevice, 0x50047c, &value);
		if (value == 0) 
		{
			if(1) 
				NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "use internal clock\n");
		   
			value = 0x100000;
			BMIWriteSOCRegister(pHIFDevice, 0x40e0, value);
		}


		/* eeprom */
		/*
		* Change to use the mechanism as Olca 2.1 : eeprom -> Host -> FW
		* With this mechanism, though the data needs to move to the host side first.
		* But we can change the eeprom data at the driver side
		*/
        
		/* The way Olca 2.1 used */
		AR_DEBUG_PRINTF("AR6000: eeprom transfer by HOST\n");

		eeprom_ar6000_transfer(pHIFDevice, NULL, NULL, 0, (char *)mac_addr);

		NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "MAC_ADDRESS --> %02x:%02x:%02x:%02x:%02x:%02x \r\n", 
							mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);

		NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "AR6000: BMISetAppStart\n");
		BMISetAppStart(pHIFDevice, 0x9140f0);

		/* enable HI_OPTION_TIMER_WAR */
		NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "AR6000: enable HI_OPTION_TIMER_WAR\n");
        value = 0;
		BMIReadSOCRegister(pHIFDevice, 0x500410, &value);
		value |= 0x01;
		BMIWriteSOCRegister(pHIFDevice, 0x500410, value);

		/* fw */
		NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "AR6000: firmware_transfer\n");
		{
			firmware_transfer(pHIFDevice, 1, 0x502070, TRUE);
		}  

		/* WLAN patch DataSets */
		firmware_transfer(pHIFDevice, 2, 0x52d6c4, FALSE);
		BMIWriteSOCRegister(pHIFDevice, 0x500418, 0x52d6c4);


		/* restore system sleep */
		BMIWriteSOCRegister(pHIFDevice, 0x40c4, old_sleep);
		ret = BMIWriteSOCRegister(pHIFDevice, 0x180c0, old_options);
		
		if (ret!=A_OK) 
		{
		    /* check if the card has any problem before we call ar6000_init */
			NDIS_DEBUG_PRINTF(DBG_ERR, " the card has any problem before we call ar6000_init \r\n");
		    goto err_exit;
		}
#if 0
		if (0) 
		{
			/* art mode */
			BMIWriteSOCRegister(pHIFDevice, 0x500478, 26000000);
			BMIWriteSOCRegister(pHIFDevice, 0x500458, 0x1);
			Sleep(1000);
		} 
		else 
		{
			/* normal WIFI or TCMD mode, done */
		
		}
#endif
	}
#endif /* ANDROID_ENV */
/* ATHENV */


err_exit:

    return ret;

}



static void AR6KTargetFailureEventHandler (void *Instance,
                                           A_STATUS Status)
{
 //   ATHR_DISPLAY_MSG (L"********Target ASSERTION***************\n");
    //
    // get the dbg logs from the target before shutdown
    //
    ar6000_dump_target_assert_info (pHIFDevice, targetType);

    //
    // In order to collect the dbglog while assertion, driver needs to call Deregister NDIS
    //
    DRG_Exit ();
}

//
//  This function processes an device insertion event indication
//  from the HIF layer.
//
static A_STATUS
AR6KTargetAvailableEventHandler(void *context, void *hifDevice)
{
//		A_STATUS		status;
	
	
		A_UINT32		value = 0;
		HTC_INIT_INFO  htcInfo;


		printf(" ######### %s ########### \r\n", __FUNCTION__);
		
		pHIFDevice = hifDevice;
		
	//	NdisMSleep(1000000);
	
		BMIInit();
	
		{
			struct bmi_target_info targ_info;
			
			if (BMIGetTargetInfo(pHIFDevice, &targ_info) != A_OK) 
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "BMIGetTargetInfo Fail !!\n");
				return A_ERROR;
			}

			targetId = targ_info.target_ver;
			targetType = targ_info.target_type;
	
			//m_TargetType = targ_info.target_type;
	
			/* do any target-specific preparation that can be done through BMI */
			if (ar6000_prepare_target(pHIFDevice, targ_info.target_type, targ_info.target_ver) != A_OK) 
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "ar6000_prepare_target Fail !!\n");
				return A_ERROR;
			}		
	
		}	
#if 0	
		if (AR6KConfigureTarget(pHIFDevice) != A_OK) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, "ar6000_configure_target Fail!!\n");
			return;
		}
#endif	
		A_MEMZERO(&htcInfo,sizeof(htcInfo));
//		htcInfo.pContext = this;
		htcInfo.TargetFailure = AR6KTargetFailureEventHandler;
	
		gHTCHandle = HTCCreate(pHIFDevice,&htcInfo);
	
		if (gHTCHandle == NULL) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, "m_HtcTarget fail !! \r\n");
			return A_ERROR;
		}
	
		HIFClaimDevice(hifDevice, (void *)1);
		
		// the following code will display the version of firmware and driver
		// also it checks whether version mismatch between drier and firmware
		// incase of version mismatch, this will false the flag and return status failure
		// notification to NDIS
		{
			BYTE  byFirmwareMajor = 0;
			BYTE  byFirmwareMinor = 0;
	
			WORD wFirmware = targetId >> 24;
			byFirmwareMajor = wFirmware >> 4;
			byFirmwareMinor = wFirmware & 0x0F;
	
			NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K,"[AR6K] FIRMWARE MAJOR VERSION ==> %x sizeof(size_t) = %d, PAGE_SIZE = %d\r\n", byFirmwareMajor,sizeof(size_t), PAGE_SIZE);
			NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K,"[AR6K] FIRMWARE MINOR VERSION ==> %x\r\n", byFirmwareMinor);
	
		}
	
		if (DownloadImage()!=A_OK) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, "ar6000_download_image fail & Skip ar6000_destroy !!! \r\n");
			return A_ERROR;
		}
	
		NDIS_DEBUG_PRINTF(DBG_TRACE, "ar6000_avail_ev() Exit - \r\n");
	
#if 0
	
		if (m_Config.defaultApp) 
		{
			status = StartEndpoints();
			if (A_OK != status) 
			{
				goto done;
			}
		}
	
		m_tgtReady = true;

	done:
 	
		NDIS_DEBUG_PRINTF(DBG_TRACE," %s () \r\n",__FUNCTION__);
		NdisSetEvent(&m_tgtReadyEvent);
		NDIS_DEBUG_PRINTF(DBG_TRACE," Exit TargetAvailableEventHandler !! \r\n");
		return;
#else
		return A_OK;
#endif		

}

//
//  This function processes a device removal event indication
//  from the HIF layer.
//
A_STATUS
AR6KTargetUnavailableEventHandler(void *removalContext, void *hifDevice )
{
 //   config_exit();

    if (gHTCHandle != NULL) {
        HTCDestroy(gHTCHandle);
        gHTCHandle = NULL;
    }

    if (pHIFDevice != NULL) {
        HIFShutDownDevice(pHIFDevice);
        HIFReleaseDevice(pHIFDevice);
    }

    pHIFDevice = NULL;
    return A_OK;
}


A_STATUS
drvInit()
{
    OSDRV_CALLBACKS osdrvCallbacks;

	printf(" ######## %s ######## \r\n", __FUNCTION__);

    A_MEMZERO(&osdrvCallbacks,sizeof(osdrvCallbacks));

    osdrvCallbacks.deviceInsertedHandler   = AR6KTargetAvailableEventHandler;
    osdrvCallbacks.deviceRemovedHandler = AR6KTargetUnavailableEventHandler;

    // Register HIF
    return HIFInit(&osdrvCallbacks);
}

void
drvDeInit()
{
    //
    // cleanup dbglog fields
    //
    if (hDbgHostEvent)
    {
        SetEvent (hDbgHostEvent);
        CloseHandle (hDbgHostEvent);
        hDbgHostEvent = NULL;
    }

    //
    // deregister Ndis
    //
    DRG_Exit();

}

HTC_HANDLE
cmnGetHTCHandle()
{
    return gHTCHandle;
}

void
cmnGetTargetInfo(A_UINT32 *pTargetType, A_UINT32 *pTargetId)
{
    *pTargetType = targetType;
    *pTargetId = targetId;
    return;
}

#if 0
A_STATUS
GetDbgLogs()
{
    A_STATUS Status = A_OK;
    struct dbglog_hdr_s debug_hdr;
    struct dbglog_buf_s debug_buf;
    A_UINT32 address;
    A_UINT32 length;
    A_UINT32 dropped;
    A_UINT32 firstbuf;
    A_UINT32     dbgLogHdrPtr;

     /* Get the starting address of the dbglog_hdr */
    dbgLogHdrPtr = dbglog_get_debug_hdr_ptr (targetType, pHIFDevice);

    /* Get the contents of the ring buffer */
    if (dbgLogHdrPtr)
    {
        address = dbgLogHdrPtr;
        length = sizeof(struct dbglog_hdr_s);
        ar6000_ReadDataDiag(pHIFDevice, address,
                            (A_UCHAR *)&debug_hdr, length);

        address = (A_UINT32)debug_hdr.dbuf;
        firstbuf= address;
        dropped = debug_hdr.dropped;
        length  = sizeof(struct dbglog_buf_s);
        ar6000_ReadDataDiag(pHIFDevice, address,
                            (A_UCHAR *)&debug_buf, length);

        do
        {
            address = (A_UINT32)debug_buf.buffer;
            length = debug_buf.length;

            if ((address) && (length) && (debug_buf.length <= debug_buf.bufsize))
            {
                /* Rewind the index if it is about to overrun the buffer */
                if (logCnt > (DBGLOG_HOST_LOG_BUFFER_SIZE - length))
                {
                    logCnt = 0;
                    Status = A_ERROR;
                    break;
                }

                if (length > DBGLOG_HOST_LOG_BUFFER_SIZE)
                {
                    //
                    // May be length value is corrupted, come out from the loop
                    //
                    Status = A_ERROR;
                    logCnt = 0;
                    break;
                }

                if(A_OK != ar6000_ReadDataDiag(pHIFDevice, address,
                                    (A_UCHAR *)&logBuffer[logCnt], length))
                {

                    Status = A_ERROR;
                    break;
                }

                ar6000_dbglog_event (NULL, dropped, &logBuffer[logCnt], length);

                logCnt += length;
            }
            else
            {
                AR_DEBUG_PRINTF(ATH_LOG_INF, ("Address:  %dLength: %d (Total size: %d)\n",
                                address, debug_buf.length, debug_buf.bufsize));
            }

            address = (A_UINT32)debug_buf.next;
            length = sizeof(struct dbglog_buf_s);
            if(A_OK != ar6000_ReadDataDiag(pHIFDevice, address,
                                (A_UCHAR *)&debug_buf, length))
            {
                Status = A_ERROR;
                break;
            }

        } while (address != firstbuf);
    }

    //
    // assuming, no rec event is running on host, then the evnt is NULL, this
    // case we have to check whether the event handle is NULL
    //
    if (NULL != hDbgHostEvent && A_ERROR == Status)
    {
        //
        // Make a success call to complete recevent tool to proceed
        //
        Status = A_OK;
        SetEvent (hDbgHostEvent);
    }

    return Status;
}


void
drvParseDebugLogs(A_INT8 *datap,
                  A_UINT32 len)
{
    A_UINT32 *buffer    = NULL;
    A_UINT32 count      = 0;
    A_UINT32 timestamp  = 0;
    A_UINT32 debugid    = 0;
    A_UINT32 moduleid   = 0;
    A_UINT32 numargs    = 0;
    A_UINT32 length     = 0;

    if (DBGLOG_HOST_LOG_BUFFER_SIZE >= len && len != 0)
    {
        A_MEMCPY (logBuffer, datap, len);
        logBufferSize = len;

#ifdef DBG
        buffer = (A_UINT32 *)logBuffer;

        length = (len >> 2);
        while (count < length) {
            debugid = DBGLOG_GET_DBGID(buffer[count]);
            moduleid = DBGLOG_GET_MODULEID(buffer[count]);
            numargs = DBGLOG_GET_NUMARGS(buffer[count]);
            timestamp = DBGLOG_GET_TIMESTAMP(buffer[count]);
            switch (numargs) {
            case 0:
                AR_DEBUG_PRINTF(ATH_LOG_INF, ("%d %d (%d)\n", moduleid, debugid, timestamp));
                break;

            case 1:
                AR_DEBUG_PRINTF(ATH_LOG_INF, ("%d %d (%d): 0x%x\n", moduleid, debugid, timestamp, buffer[count+1]));
                break;

            case 2:
                AR_DEBUG_PRINTF(ATH_LOG_INF, ("%d %d (%d): 0x%x, 0x%x\n", moduleid, debugid,
                            timestamp, buffer[count+1], buffer[count+2]));

                break;

            default:
                AR_DEBUG_PRINTF(ATH_LOG_INF, ("Invalid args: %d\n", numargs));

            }
            count += numargs + 1;
        }
#endif
    }


    //
    // assuming, no rec event is running on host, then the evnt is NULL, this
    // case we have to check whether the event handle is NULL
    //
    if (NULL != hDbgHostEvent)
    {
        SetEvent (hDbgHostEvent);
    }
}

A_STATUS
drvDbgLogbuffer (DWORD dwBufferSize,
                 PBYTE   pOutBuf,
                 PDWORD  pBytesReturned)
{
    A_STATUS Status = A_ERROR;

    do
    {
        if (NULL == pOutBuf)
        {
            break;
        }

        if ( dwBufferSize < logBufferSize )
        {
            break;
        }

        if (logBufferSize) {
            memcpy (pOutBuf, logBuffer, logBufferSize);
        }

        *pBytesReturned = logBufferSize;

        Status = A_OK;

    } while (FALSE);

    return Status;
}

void
ar6000_dbglog_event(void    *context,
                    A_UINT32 dropped,
                    A_UINT8 *datap,
                    A_UINT32 length)
{

    drvParseDebugLogs (datap, length);
    return;
}

void
ar6000_dbglog_init_done (void *ar)
{
}

#endif

