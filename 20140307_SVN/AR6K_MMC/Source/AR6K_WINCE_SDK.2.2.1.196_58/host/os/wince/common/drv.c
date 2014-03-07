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
#include "stream_drv.h"
#include "common_drv.h"
#include "bmi.h"
#include "wince_common_drv.h"
#include "wmi.h"
#include "ar6xapi_wince.h"
#include "dbglog_api.h"
#include "debug_wince.h"

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
ULONG	debugBssInfo = 0;	//buildercheck
ULONG	debugFileLog = 0;	//buildercheck
ULONG	debugErrorLog = 0;	//buildercheck
HANDLE	targetfailEvent = NULL;

static void AR6KTargetFailureEventHandler (void *Instance,
                                           A_STATUS Status)
{
    RETAIL_DEBUG_PRINTF(1,  ( TEXT( "builder:ar6k2:AR6KTargetFailureEventHandler() start\r\n" )) );
logPrintf( (debugFileLog && debugErrorLog), "builder:ar6k2:AR6KTargetFailureEventHandler() start\r\n" );
    ATHR_DISPLAY_MSG (L"********Target ASSERTION***************\n");
    //
    // get the dbg logs from the target before shutdown
    //
    ar6000_dump_target_assert_info (pHIFDevice, targetType);

    //
    // In order to collect the dbglog while assertion, driver needs to call Deregister NDIS
    //
    DRG_Exit ();

	if( targetfailEvent )
	    SetEvent(targetfailEvent);
}

//
//  This function processes an device insertion event indication
//  from the HIF layer.
//
static A_STATUS
AR6KTargetAvailableEventHandler(void *context, void *hifDevice)
{
    struct bmi_target_info targInfo;
    HTC_INIT_INFO htcInitInfo;

    pHIFDevice = hifDevice;

    RETAIL_DEBUG_PRINTF(1,  ( TEXT( "builder:ar6k2:AR6KTargetAvailableEventHandler() start\r\n" )) );

    BMIInit();

    if (BMIGetTargetInfo(pHIFDevice, &targInfo) != A_OK)
    {
       return A_ERROR;
    }
    targetId = targInfo.target_ver;
    targetType = targInfo.target_type;

    early_init_ar6000(pHIFDevice, targetType, targetId);

    //
    // init dbg variables
    //
    logCnt        = 0;
    logBufferSize = 0;

    hDbgHostEvent = CreateEvent (NULL, FALSE, FALSE, TEXT("AR6KTGT_DBGLOG"));

    memset(&htcInitInfo,0,sizeof(htcInitInfo));
    htcInitInfo.TargetFailure  = AR6KTargetFailureEventHandler;
    gHTCHandle = HTCCreate(hifDevice, &htcInitInfo);

    if (gHTCHandle == NULL) {
        return A_ERROR;
    }
        /* need to claim the device with a non-NULL context */
    HIFClaimDevice(hifDevice, (void *)1);

    return A_OK;
}

//
//  This function processes a device removal event indication
//  from the HIF layer.
//
A_STATUS
AR6KTargetUnavailableEventHandler(void *removalContext, void *hifDevice )
{
    RETAIL_DEBUG_PRINTF(1,  ( TEXT( "builder:ar6k2:AR6KTargetUnavailableEventHandler() start\r\n" )) );
	logPrintf( (debugFileLog && debugErrorLog), "builder:ar6k2:AR6KTargetUnavailableEventHandler() start\r\n" );

    config_exit();

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
