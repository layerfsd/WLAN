//------------------------------------------------------------------------------
// <copyright file="htc_raw_drv.c" company="Atheros">
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
// Author(s): ="Atheros"
//==============================================================================
#ifdef HTC_RAW_INTERFACE
#include "a_config.h"
#include "athdefs.h"
#include "a_types.h"
#include "a_debug.h"
#include "htc_api.h"
#include "htc_raw_drv.h"
#include "bmi.h"
#include "wince_common_drv.h"
#include "common_drv.h"

#define BUFFER_SIZE    1664
#define RAW_HTC_READ_BUFFERS_NUM                     4
#define RAW_HTC_WRITE_BUFFERS_NUM                    4

typedef struct {
    A_INT32 currPtr;
    A_INT32 length;
    A_UCHAR data[BUFFER_SIZE];
    HTC_PACKET    HTCPacket;
} raw_htc_buffer;

static HANDLE                   raw_htc_read_sem[HTC_RAW_STREAM_NUM_MAX];
static HANDLE                   raw_htc_write_sem[HTC_RAW_STREAM_NUM_MAX];
static HANDLE                   raw_htc_read_queue[HTC_RAW_STREAM_NUM_MAX];
static HANDLE                   raw_htc_write_queue[HTC_RAW_STREAM_NUM_MAX];
static HTC_ENDPOINT_ID          arRaw2EpMapping[HTC_RAW_STREAM_NUM_MAX];
static HTC_RAW_STREAM_ID        arEp2RawMapping[ENDPOINT_MAX];

static raw_htc_buffer raw_htc_read_buffer[HTC_RAW_STREAM_NUM_MAX][RAW_HTC_READ_BUFFERS_NUM];
static raw_htc_buffer raw_htc_write_buffer[HTC_RAW_STREAM_NUM_MAX][RAW_HTC_WRITE_BUFFERS_NUM];
static A_BOOL write_buffer_available[HTC_RAW_STREAM_NUM_MAX];
static A_BOOL read_buffer_available[HTC_RAW_STREAM_NUM_MAX];
static A_BOOL htcStarted  = FALSE;

static void
ar6000_htc_raw_read_cb(void *Context, HTC_PACKET *pPacket)
{
    raw_htc_buffer    *busy;
    HTC_RAW_STREAM_ID streamID;

    busy = (raw_htc_buffer *)pPacket->pPktContext;

    AR_DEBUG_ASSERT(busy != NULL);

    if (pPacket->Status == A_ECANCELED) {
        /*
         * HTC provides A_ECANCELED status when it doesn't want to be refilled
         * (probably due to a shutdown)
         */
        return;
}

    streamID = arEp2RawMapping[pPacket->Endpoint];

    if ( WAIT_FAILED == WaitForSingleObject(raw_htc_read_sem[streamID], INFINITE) )
    {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Unable to wait on the Semaphore\n"));
    }

    busy->length = pPacket->ActualLength + HTC_HEADER_LEN;
    busy->currPtr = HTC_HEADER_LEN;
    read_buffer_available[streamID] = TRUE;

    ReleaseMutex(raw_htc_read_sem[streamID]);

    /* Signal the waiting process */
    AR_DEBUG_PRINTF(ATH_LOG_INF, ("Waking up the endpoint(%d) read process\n", streamID));

    SetEvent(&raw_htc_read_queue[streamID]);
}

static void
ar6000_htc_raw_write_cb(void *Context, HTC_PACKET *pPacket)
{
    raw_htc_buffer *free;
    HTC_RAW_STREAM_ID   streamID;

    free = (raw_htc_buffer *)pPacket->pPktContext;
    AR_DEBUG_ASSERT(free != NULL);

    if (pPacket->Status == A_ECANCELED) {
        /*
         * HTC provides A_ECANCELED status when it doesn't want to be refilled
         * (probably due to a shutdown)
         */
        return;
    }

    streamID = arEp2RawMapping[pPacket->Endpoint];

    if ( WAIT_FAILED == WaitForSingleObject(raw_htc_write_sem[streamID], INFINITE) ) {

        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Unable to wait on the Semaphore\n"));
    }

    AR_DEBUG_ASSERT(pPacket->pBuffer == (free->data + HTC_HEADER_LEN));

    free->length = 0;
    write_buffer_available[streamID] = TRUE;
    ReleaseMutex(raw_htc_write_sem[streamID]);

    /* Signal the waiting process */
    AR_DEBUG_PRINTF(ATH_LOG_INF, ("Waking up the endpoint(%d) write process\n", streamID));

    SetEvent(&raw_htc_write_queue[streamID]);
}

/* connect to a service */
static A_STATUS ar6000_connect_raw_service(HTC_HANDLE htcTarget, HTC_RAW_STREAM_ID StreamID)
{
    A_STATUS                 status;
    HTC_SERVICE_CONNECT_RESP response;
    A_UINT8                  streamNo;
    HTC_SERVICE_CONNECT_REQ  connect;

    do {

        A_MEMZERO (&connect,sizeof(connect));
        /* pass the stream ID as meta data to the RAW streams service */
        streamNo = (A_UINT8)StreamID;
        connect.pMetaData = &streamNo;
        connect.MetaDataLength = sizeof(A_UINT8);

        /* these fields are the same for all endpoints */
        connect.EpCallbacks.pContext         = htcTarget;
        connect.EpCallbacks.EpTxComplete     = ar6000_htc_raw_write_cb;
        connect.EpCallbacks.EpRecv         = ar6000_htc_raw_read_cb;

        /* simple interface, we don't need these optional callbacks */
        connect.EpCallbacks.EpRecvRefill     = NULL;
        connect.EpCallbacks.EpSendFull         = NULL;
        connect.MaxSendQueueDepth             = RAW_HTC_WRITE_BUFFERS_NUM;

        /* connect to the raw streams service, we may be able to get 1 or more
         * connections, depending on WHAT is running on the target */
        connect.ServiceID = HTC_RAW_STREAMS_SVC;

        A_MEMZERO(&response,sizeof(response));

        /* try to connect to the raw stream, it is okay if this fails with
         * status HTC_SERVICE_NO_MORE_EP */
        status = HTCConnectService(htcTarget,
                                   &connect,
                                   &response);

        if (A_FAILED(status))
        {
            if (response.ConnectRespCode == HTC_SERVICE_NO_MORE_EP)
            {
                AR_DEBUG_PRINTF (ATH_LOG_ERR, ("AR6K: HTC RAW , No more streams allowed\n"));
                status = A_OK;
            }
            break;
        }

        arRaw2EpMapping[StreamID] = response.Endpoint;
        arEp2RawMapping[response.Endpoint] = StreamID;

        AR_DEBUG_PRINTF (ATH_LOG_INF, ("AR6K: HTC RAW : stream ID: %d, endpoint: %d\n", StreamID, arRaw2EpMapping [StreamID]));

    } while (FALSE);

    return status;
}

A_INT32
ar6000_htc_raw_open(HTC_HANDLE htcTarget)
{
    A_STATUS status;
    A_INT32 streamID, endPt, count2;
    raw_htc_buffer *buffer;
    HTC_SERVICE_ID servicepriority;
    
    status = HTCWaitTarget(htcTarget);

    AR_DEBUG_PRINTF (ATH_LOG_INF, ("AR6K: HTCWaitTarget Status %x\n", status));

    for (endPt = 0; endPt < ENDPOINT_MAX; endPt++) {
        arEp2RawMapping[endPt] = HTC_RAW_STREAM_NOT_MAPPED;
    }

    for (streamID = HTC_RAW_STREAM_0; streamID < HTC_RAW_STREAM_NUM_MAX; streamID++) {
        /* Initialize the data structures */
        if (!(raw_htc_read_sem[streamID] = CreateMutex(NULL,FALSE,NULL)) ) {
            return -1;
        }
        if (!(raw_htc_write_sem[streamID] = CreateMutex(NULL, FALSE, NULL)) ) {
            return -1;
        }

        raw_htc_read_queue[streamID] = CreateEvent (NULL, FALSE, FALSE, NULL);
        raw_htc_write_queue[streamID] = CreateEvent (NULL, FALSE, FALSE, NULL);

        /* try to connect to the raw service */
        status = ar6000_connect_raw_service (htcTarget, (HTC_RAW_STREAM_ID)streamID);

        AR_DEBUG_PRINTF (ATH_LOG_INF, ("ar6000_connect_raw_service returned status %x StreamID %d\n", status, streamID));


        if (A_FAILED(status)) {
            AR_DEBUG_PRINTF (ATH_LOG_ERR, ("ar6000_connect_raw_service returned failure status %x\n", status));
            break;
        }

        if (arRaw2EpMapping[streamID] == 0)
        {
            AR_DEBUG_PRINTF (ATH_LOG_ERR, ("arRaw2EpMapping[streamID] should not be zero\n"));
            break;
        }

        for (count2 = 0; count2 < RAW_HTC_READ_BUFFERS_NUM; count2 ++) {
            /* Initialize the receive buffers */
            buffer = &raw_htc_write_buffer[streamID][count2];
            memset(buffer, 0, sizeof(raw_htc_buffer));
            buffer = &raw_htc_read_buffer[streamID][count2];
            memset(buffer, 0, sizeof(raw_htc_buffer));

            SET_HTC_PACKET_INFO_RX_REFILL(&buffer->HTCPacket,
                                          buffer,
                                          buffer->data,
                                          BUFFER_SIZE,
                                          arRaw2EpMapping[streamID]);

            /* Queue buffers to HTC for receive */
            if ((status = HTCAddReceivePkt(htcTarget, &buffer->HTCPacket)) != A_OK)
            {
                AR_DEBUG_PRINTF (ATH_LOG_ERR, ("HTCAddReceivePkt call failed\n"));
                BMIInit();
                return -1;
            }
        }

        for (count2 = 0; count2 < RAW_HTC_WRITE_BUFFERS_NUM; count2 ++) {
            /* Initialize the receive buffers */
            buffer = &raw_htc_write_buffer[streamID][count2];
            memset(buffer, 0, sizeof(raw_htc_buffer));
        }

        read_buffer_available[streamID]  = FALSE;
        write_buffer_available[streamID] = TRUE;
    }

    if (A_FAILED(status)) {
        AR_DEBUG_PRINTF (ATH_LOG_ERR, ("raw open returned failed status %x\n", status));
        return -1;
    }

    AR_DEBUG_PRINTF (ATH_LOG_INF, ("HTC RAW, number of streams the target supports: %d \n", streamID));

    servicepriority = HTC_RAW_STREAMS_SVC;  /* only 1 */

    /* set callbacks and priority list */
    HTCSetCreditDistribution(htcTarget,
                             NULL,
                             NULL,  /* use default */
                             NULL,  /* use default */
                             &servicepriority,
                             1);

    /* Start the HTC component */
    if ((status = HTCStart(htcTarget)) != A_OK) {
        BMIInit();
        AR_DEBUG_PRINTF (ATH_LOG_ERR, ("HtcStart returned failed status %x \n", status));
        return -1;
    }

    htcStarted = TRUE;

    return 0;
}

A_INT32
ar6000_htc_raw_close(HTC_HANDLE htcTarget)
{
    A_INT32 streamID;
    A_UINT32 targetId,targetType;
    HIF_DEVICE *hifDevice;

    cmnGetTargetInfo(&targetType, &targetId); 
    hifDevice = (HIF_DEVICE *)HTCGetHifDevice(htcTarget);
    
    if (htcStarted)
    {
        HTCStop (htcTarget);
        htcStarted = FALSE;
    }
    
    ar6000_reset_device (hifDevice, targetType, TRUE);

    /* Initialize the BMI component */
    BMIInit();

    for (streamID = HTC_RAW_STREAM_0; streamID < HTC_RAW_STREAM_NUM_MAX; streamID++) {

        if (raw_htc_read_sem[streamID])
        {
            CloseHandle (raw_htc_read_sem[streamID]);
            raw_htc_read_sem[streamID] = NULL;
        }
        if (raw_htc_write_sem[streamID])
        {
            CloseHandle (raw_htc_write_sem[streamID]);
            raw_htc_write_sem[streamID] = NULL;
        }

        /* free the raw event structures */
        CloseHandle(raw_htc_read_queue[streamID]);
        CloseHandle(raw_htc_write_queue[streamID]);
    }

    return 0;
}

raw_htc_buffer *
get_filled_buffer(HTC_RAW_STREAM_ID StreamID)
{
    A_INT32 count;
    raw_htc_buffer *busy;

    /* Check for data */
    for (count = 0; count < RAW_HTC_READ_BUFFERS_NUM; count ++) {
        busy = &raw_htc_read_buffer[StreamID][count];
        if (busy->length) {
            break;
        }
    }
    if (busy->length) {
        read_buffer_available[StreamID] = TRUE;
    } else {
        read_buffer_available[StreamID] = FALSE;
    }

    return busy;
}

size_t
ar6000_htc_raw_read(HTC_HANDLE htcTarget, HTC_RAW_STREAM_ID StreamID, char *buffer, A_INT32 length)
{
    A_INT32 readPtr;
    raw_htc_buffer *busy;

    if (arRaw2EpMapping[StreamID] == 0)
    {
        AR_DEBUG_PRINTF (ATH_LOG_ERR, ("StreamID(%d) not connected! \n", StreamID));
        return -1;
    }

    if ( WAIT_FAILED == WaitForSingleObject(raw_htc_read_sem[StreamID], INFINITE) ) {
        return -1;
    }

    busy = get_filled_buffer(StreamID);
    while (!read_buffer_available[StreamID]) {

        ReleaseMutex(raw_htc_read_sem[StreamID]);

        /* Wait for the data */
        AR_DEBUG_PRINTF(ATH_LOG_INF, ("Sleeping endpoint(%d) read process\n", StreamID));

        WaitForSingleObject (raw_htc_read_queue[StreamID], 0);

        ResetEvent (raw_htc_read_queue[StreamID]);

        if ( WAIT_FAILED == WaitForSingleObject(raw_htc_read_sem[StreamID], INFINITE) ) {
            return -1;
        }

        busy = get_filled_buffer(StreamID);
    }

    /* Read the data */
    readPtr = busy->currPtr;
    if (length > (A_INT32)(busy->length - HTC_HEADER_LEN)) {
        length = busy->length - HTC_HEADER_LEN;
    }

    memcpy(buffer, &busy->data[readPtr], length);

    busy->currPtr += length;

    ReleaseMutex(raw_htc_read_sem[StreamID]);

    if (busy->currPtr == busy->length)
    {
        busy->currPtr = 0;
        busy->length = 0;
        HTC_PACKET_RESET_RX(&busy->HTCPacket);
        HTCAddReceivePkt(htcTarget, &busy->HTCPacket);
    }

    read_buffer_available[StreamID] = FALSE;

    return length;
}

raw_htc_buffer *
get_free_buffer(HTC_RAW_STREAM_ID StreamID)
{
    A_INT32 count;
    raw_htc_buffer *free;

    free = NULL;
    for (count = 0; count < RAW_HTC_WRITE_BUFFERS_NUM; count ++) {
        free = &raw_htc_write_buffer[StreamID][count];
        if (free->length == 0) {
            break;
        }
    }
    if (!free->length) {
        write_buffer_available[StreamID] = TRUE;
    } else {
        write_buffer_available[StreamID] = FALSE;
    }

    return free;
}

size_t
ar6000_htc_raw_write(HTC_HANDLE htcTarget, HTC_RAW_STREAM_ID StreamID, char *buffer, A_INT32 length)
{
    A_INT32 writePtr;
    raw_htc_buffer *free;

    if (arRaw2EpMapping[StreamID] == 0)
    {
        AR_DEBUG_PRINTF (ATH_LOG_ERR, ("StreamID(%d) not connected! \n", StreamID));
        return -1;
    }

       if ( WAIT_FAILED == WaitForSingleObject(raw_htc_write_sem[StreamID], INFINITE) ) {
        return -1;
    }

    /* Search for a free buffer */
    free = get_free_buffer(StreamID);

    /* Check if there is space to write else wait */
    while (!write_buffer_available[StreamID]) {

        ReleaseMutex(raw_htc_write_sem[StreamID]);

        /* Wait for buffer to become free */
        AR_DEBUG_PRINTF(ATH_LOG_INF, ("Sleeping endpoint(%d) write process\n", StreamID));

        WaitForSingleObject(raw_htc_write_queue[StreamID], 0);

        ResetEvent(raw_htc_write_queue[StreamID]);
        if ( WAIT_FAILED == WaitForSingleObject(raw_htc_write_sem[StreamID], INFINITE) ) {
            return -1;
        }

        free = get_free_buffer(StreamID);
    }

    /* Send the data */
    writePtr = HTC_HEADER_LEN;
    if (length > (BUFFER_SIZE - HTC_HEADER_LEN)) {
        length = BUFFER_SIZE - HTC_HEADER_LEN;
    }

    memcpy(&free->data[writePtr], buffer, length);

    free->length = length;
    ReleaseMutex(raw_htc_write_sem[StreamID]);
    SET_HTC_PACKET_INFO_TX(&free->HTCPacket,
                        free,
                        &free->data[writePtr],
                        length,
                        arRaw2EpMapping[StreamID],
                        AR6K_DATA_PKT_TAG);

    HTCSendPkt(htcTarget, &free->HTCPacket);

    return length;
}

#endif //HTC_RAW_INTERFACE

