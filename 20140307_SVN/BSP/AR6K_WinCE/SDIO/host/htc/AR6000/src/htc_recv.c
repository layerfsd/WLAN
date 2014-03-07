//------------------------------------------------------------------------------
// <copyright file="htc_recv.c" company="Atheros">
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
// This file contains the routines handling the receive path.
//
// Author(s): ="Atheros"
//==============================================================================


#include "htc_internal.h"

/* ------ Global Variable Declarations ------- */
#ifdef DEBUG
extern A_UINT32 debughtc;
#endif

/* ------ Static Variables ------ */


/* ------ Functions ------ */
/* Makes a buffer available to the HTC module */
A_STATUS 
HTCBufferReceive(HTC_TARGET *target, 
                 HTC_ENDPOINT_ID endPointId,
                 A_UCHAR *buffer, 
                 A_UINT32 length,
                 void *cookie)
{
    A_STATUS status;
    HTC_ENDPOINT *endPoint;
    HTC_DATA_REQUEST_QUEUE *recvQueue;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, 
                    "HTCBufferReceive: Enter (endPointId: %d, buffer: 0x%p, length: %d, cookie: 0x%p)\n", endPointId, buffer, length, cookie);

    AR_DEBUG_ASSERT((endPointId >= ENDPOINT1) && (endPointId <= ENDPOINT4));

    /* Extract the end point instance */
    endPoint = &target->endPoint[endPointId];
    AR_DEBUG_ASSERT(endPoint != NULL);

    recvQueue = &endPoint->recvQueue;
    HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_RECV, "recvQueue: %p\n", 
                    recvQueue);

    /* Add this posted buffer to the pending receive queue */
    status = addToMboxQueue(recvQueue, buffer, length, 0, cookie);
    if (status != A_OK) {
        HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_RECV,
                        "Mailbox (%d) Send queue full. Unable to add buffer\n", 
                        GET_ENDPOINT_ID(endPoint));
        return A_ERROR;
    }

    /* 
     * If this API was called as a result of a HTC_DATA_AVAILABLE event to
     * the upper layer, indicating that HTC is out of buffers, then we should
     * receive the frame in the buffer supplied otherwise we simply add the 
     * buffer to the Pending Receive Queue 
     */
    if (endPoint->rxLengthPending) {
        htcReceiveFrame(endPoint);
    }

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, 
                    "HTCBufferReceive: Exit\n");
    return A_OK;
}

void
htcReceiveFrame(HTC_ENDPOINT *endPoint)
{
    A_STATUS status;
    A_UINT32 address;
    A_UINT32 paddedLength;
    A_UINT32 frameLength;
    HIF_REQUEST request;
    HTC_ENDPOINT_ID endPointId;
    HTC_QUEUE_ELEMENT *element;
    HTC_MBOX_BUFFER *mboxBuffer;
    HTC_DATA_REQUEST_QUEUE *recvQueue;
    HTC_TARGET *target;
    HTC_EVENT_INFO eventInfo;
    HIF_DATAMODE dmode;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, 
                    "htcReceiveFrame - Enter\n");

    /* Get the context */
    AR_DEBUG_ASSERT(endPoint != NULL);
    endPointId = GET_ENDPOINT_ID(endPoint);
    target = endPoint->target;
    AR_DEBUG_ASSERT(target != NULL);
    recvQueue = &endPoint->recvQueue;
    AR_DEBUG_ASSERT(recvQueue != NULL);

    /* 
     * Receive the frame if we have any pending frames and a buffer to
     * receive it into.
     */
     if (IS_DATA_QUEUE_EMPTY(recvQueue)) {
         HTC_DEBUG_PRINTF(ATH_LOG_WARN | ATH_LOG_RECV,
                         "Mailbox (%d) recv queue empty. Unable to remove buffer\n", endPointId);

         /* 
          * Communicate this situation to the host via the HTC_DATA_AVAILABLE
          * event to request some buffers in the queue.
          */
         endPoint->rxLengthPending = htcGetFrameLength(endPoint);
         AR_DEBUG_ASSERT(endPoint->rxLengthPending);
         FRAME_EVENT(eventInfo, NULL, endPoint->rxLengthPending, 
                     0, A_OK, NULL);
         dispatchEvent(target, endPointId, HTC_DATA_AVAILABLE, &eventInfo);
         return;
     }

     /* 
      * Get the length from the lookahead register if there is nothing 
      * pending.
      */
     if (endPoint->rxLengthPending) {
         frameLength = endPoint->rxLengthPending;
         endPoint->rxLengthPending = 0;
     } else {
         frameLength = htcGetFrameLength(endPoint);
     }
     
     if (frameLength > HTC_MESSAGE_SIZE_MAX) {
	     return;
     }
     
     HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_RECV, "Frame Length: %d\n", 
                     frameLength);

     /* Adjust the length to be a multiple of block size if appropriate */
     paddedLength = (frameLength + (endPoint->blockSize - 1)) &
                    (~(endPoint->blockSize - 1));

     /* 
      * Receive the frame(s). Pull an empty buffer from the head of the 
      * Pending Receive Queue.
      */
     element = removeFromMboxQueue(recvQueue);
     mboxBuffer = GET_MBOX_BUFFER(element);
     mboxBuffer->actualLength = paddedLength;
     dmode = (endPoint->blockSize > 1) ? HIF_BLOCK_BASIS : HIF_BYTE_BASIS;
     HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, 
                       HIF_ASYNCHRONOUS, dmode, HIF_FIXED_ADDRESS);
     address = HIF_MBOX_END_ADDR(endPointId);
     status = HIFReadWrite(target->device, address, mboxBuffer->buffer, 
                           mboxBuffer->actualLength, &request, element);
#ifndef HTC_SYNC
     if (status != A_OK) {
#else
     if (status != A_OK && status != A_PENDING) {
#endif
         HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_RECV, 
                         "Frame reception failed\n");
         if (!IS_ELEMENT_FREE(element)) {
             mboxBuffer->actualLength = 0;
             FRAME_EVENT(eventInfo, mboxBuffer->buffer, 
                         mboxBuffer->bufferLength, mboxBuffer->actualLength, 
                         A_ERROR, mboxBuffer->cookie);
             RECYCLE_DATA_REQUEST_ELEMENT(element);
             dispatchEvent(target, endPointId, HTC_BUFFER_RECEIVED, 
                           &eventInfo);
             HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, 
                             "htcReceiveFrame - Exit\n");
             return;
         }
     }
#ifdef HTC_SYNC
	else if (status == A_OK) {
		element->completionCB(element, status);
	}
#endif

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, 
                    "htcReceiveFrame - Exit\n");
}

A_UINT32
htcGetFrameLength(HTC_ENDPOINT *endPoint)
{
    HTC_TARGET *target;
    A_UINT32 frameLength;
    HTC_ENDPOINT_ID endPointId;

    /* Get the context */
    AR_DEBUG_ASSERT(endPoint != NULL);
    target = endPoint->target;
    AR_DEBUG_ASSERT(target != NULL);
    endPointId = GET_ENDPOINT_ID(endPoint);

    AR_DEBUG_ASSERT(target->table.rx_lookahead_valid & (1 << endPointId));

    /* The length is contained in the first two bytes - HTC_HEADER_LEN */
    frameLength = (target->table.rx_lookahead[endPointId] & 0xFFFF) +
                  HTC_HEADER_LEN;
    AR_DEBUG_ASSERT(frameLength);

    return frameLength;
}
