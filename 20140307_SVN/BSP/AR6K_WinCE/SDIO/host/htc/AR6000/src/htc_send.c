//------------------------------------------------------------------------------
// <copyright file="htc_send.c" company="Atheros">
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
// This file contains the routines handling the transmit path.
//
// Author(s): ="Atheros"
//==============================================================================


#include "htc_internal.h"

/* ------ Global Variable Declarations ------- */
extern A_MUTEX_T creditCS,txCS;

#ifdef DEBUG
extern A_UINT32 debughtc;
extern A_UINT32 txcreditsavailable[HTC_MAILBOX_NUM_MAX];
extern A_UINT32 txcreditsconsumed[HTC_MAILBOX_NUM_MAX];
#ifdef HTC_SYNC
extern A_UINT32 txcreditintrenable[HTC_MAILBOX_NUM_MAX];
extern A_UINT32 txcreditintrenableaggregate[HTC_MAILBOX_NUM_MAX];
#endif
#endif

extern A_UINT32 tx_attempt[HTC_MAILBOX_NUM_MAX];  /* Num of attempts to add */
extern A_UINT32 tx_post[HTC_MAILBOX_NUM_MAX];     /* Num of attemps succeded */
extern A_UINT32 tx_complete[HTC_MAILBOX_NUM_MAX]; /* Num of tx complete */

void htcSendFrameLocked(HTC_ENDPOINT *endPoint);

/* ------ Functions ------ */
A_STATUS 
HTCBufferSend(HTC_TARGET *target, 
              HTC_ENDPOINT_ID endPointId,
              A_UCHAR *buffer, 
              A_UINT32 length,
              void *cookie)
{
    A_STATUS status;
    HTC_ENDPOINT *endPoint;
    HTC_DATA_REQUEST_QUEUE *sendQueue;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND, 
                    "HTCBufferSend: Enter (endPointId: %d, buffer: 0x%p, length: %d, cookie: 0x%p)\n", endPointId, buffer, length, cookie);

    AR_DEBUG_ASSERT((endPointId >= ENDPOINT1) && (endPointId <= ENDPOINT4));
    AR_DEBUG_ASSERT(length);

    /* Extract the end point instance */
    endPoint = &target->endPoint[endPointId];
    AR_DEBUG_ASSERT(endPoint != NULL);
    sendQueue = &endPoint->sendQueue;
    AR_DEBUG_ASSERT(sendQueue != NULL);
    HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_SEND, 
                    "mboxQueue: %p\n", sendQueue);

    /* 
     * Add this posted buffer to the pending send queue. We need to allocate 
     * a bufferElement to store the packet information and we borrow that 
     * buffer from the pending send queue. If circumstances allow us to 
     * transmit it right away then we dequeue it otherwise we let it remain 
     * to be picked in the interrupt handler context.
     */
    tx_attempt[endPointId] += 1;

    if (!endPoint->enabled) {
        HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_SEND, "Endpoint not enabled: %d\n", 
                                        GET_ENDPOINT_ID(endPoint));
        HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_SEND, 
                        "tx_attempt[%d] = %d, tx_post[%d] = %d, tx_complete[%d] = %d\n", endPointId, tx_attempt[endPointId], endPointId, tx_post[endPointId], endPointId, tx_complete[endPointId]);
        return A_ERROR;
    }

    status = addToMboxQueue(sendQueue, buffer, length, 0, cookie);
    if (status != A_OK) {
        HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_SEND,
                        "Mailbox (%d) PSQ full. Unable to add buffer\n", 
                         endPointId);
        HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_SEND, 
                        "tx_attempt[%d] = %d, tx_post[%d] = %d, tx_complete[%d] = %d\n", endPointId, tx_attempt[endPointId], endPointId, tx_post[endPointId], endPointId, tx_complete[endPointId]);
        return A_ERROR;
    }

    tx_post[endPointId] += 1;

    /* 
     * The frame shall be dequeued and sent if there are any credits 
     * available. 
     */
    htcSendFrame(endPoint);

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND, "HTCBufferSend: Exit\n");
    return A_OK;
}

void
htcSendFrame(HTC_ENDPOINT *endPoint) 
{
	A_MUTEX_LOCK(&txCS);	
	htcSendFrameLocked(endPoint);
	A_MUTEX_UNLOCK(&txCS);
}

void 
htcSendFrameLocked(HTC_ENDPOINT *endPoint) 
{
    A_STATUS status;
    A_UINT32 address;
    HIF_DATAMODE dmode;
    HTC_TARGET *target;
    HIF_REQUEST request;
    A_UINT32 frameLength;
    A_UINT32 paddedLength;
    HTC_EVENT_INFO eventInfo;
#ifdef ONLY_16BIT
    A_UINT16 txCreditsConsumed;
    A_UINT16 txCreditsAvailable;
#else
    A_UINT8 txCreditsConsumed;
    A_UINT8 txCreditsAvailable;
#endif
    HTC_ENDPOINT_ID endPointId;
    HTC_QUEUE_ELEMENT *element;
    HTC_MBOX_BUFFER *mboxBuffer;
    HTC_REG_REQUEST_LIST *regList;
    HTC_DATA_REQUEST_QUEUE *sendQueue;
#ifdef HTC_SYNC
    HTC_REG_BUFFER *regBuffer;
#endif

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND, "htcSendFrame - Enter\n");

    /* Get the context */
    AR_DEBUG_ASSERT(endPoint != NULL);
    endPointId = GET_ENDPOINT_ID(endPoint);
    target = endPoint->target;
    AR_DEBUG_ASSERT(target != NULL);
    sendQueue = &endPoint->sendQueue;
    AR_DEBUG_ASSERT(sendQueue != NULL);
    regList = &target->regList;
    AR_DEBUG_ASSERT(regList != NULL);

    /* 
     * Transmit the frames as long as we have the credits available and
     * the queue is not out of them 
     */
    HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                    "Critical Section (credit): LOCK at line %d in file %s\n", __LINE__, __FILE__);
    A_MUTEX_LOCK(&creditCS);
    txCreditsAvailable = GET_TX_CREDITS_AVAILABLE(endPoint);
    txCreditsConsumed = GET_TX_CREDITS_CONSUMED(endPoint);
    SET_TX_CREDITS_AVAILABLE(endPoint, 0);
    SET_TX_CREDITS_CONSUMED(endPoint, txCreditsConsumed + txCreditsAvailable);
    HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                    "Critical Section (credit): UNLOCK at line %d in file %s\n", __LINE__, __FILE__);
    A_MUTEX_UNLOCK(&creditCS);

    if (!txCreditsAvailable) {
        return;
    }

    /* 
     * Send the packet only when there are packets to be sent and there
     * are positive number of credits available.
     */
    while((!IS_DATA_QUEUE_EMPTY(sendQueue)) && txCreditsAvailable)
    {
        /* Get the request buffer from the Pending Send Queue */
        element = removeFromMboxQueue(sendQueue);
        mboxBuffer = GET_MBOX_BUFFER(element);

        /* 
         * Prepend the actual length in the first 2 bytes of the outgoing 
         * packet.
         */
        mboxBuffer->buffer -= HTC_HEADER_LEN;
        A_MEMCPY(mboxBuffer->buffer, &mboxBuffer->bufferLength, HTC_HEADER_LEN);

        /* 
         * Adjust the length in the block mode only when its not an integral 
         * multiple of the block size. Assumption is that the block size is
         * a power of 2.
         */
        frameLength = mboxBuffer->bufferLength + HTC_HEADER_LEN;
        paddedLength = (frameLength + (endPoint->blockSize - 1)) & 
                       (~(endPoint->blockSize - 1));
        mboxBuffer->actualLength = paddedLength;
        HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_SEND,  
                        "Original frame length: %d, Padded frame length: %d\n", frameLength, paddedLength);

        HTC_DEBUG_PRINTBUF(mboxBuffer->buffer, (A_UINT16)mboxBuffer->actualLength);
        HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_SEND,  
                        "Available Tx credits: %d\n", txCreditsAvailable);

        /* Frame the interface request */
        dmode = (endPoint->blockSize > 1) ? HIF_BLOCK_BASIS : HIF_BYTE_BASIS;
        HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, 
                          HIF_ASYNCHRONOUS, dmode, HIF_INCREMENTAL_ADDRESS);
        address = HIF_MBOX_START_ADDR(endPointId);

        /* Send the data to the bus driver */
        status = HIFReadWrite(target->device, address, mboxBuffer->buffer, 
                              mboxBuffer->actualLength, &request, element);
#ifndef HTC_SYNC
        if (status != A_OK) {
#else
		if (status != A_OK && status != A_PENDING) {
#endif

            HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_SEND, 
                            "Frame transmission failed\n");
            HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_SEND, 
                            "EndPoint: %d, Tx credits available: %d\n", 
                            endPointId, GET_TX_CREDITS_AVAILABLE(endPoint));
            /* 
             * We need to check just in case the callback routine was called
             * with the error status before we reach this point and in that
             * context we fee up the buffer so its just a conservative design.
             */
            if (!IS_ELEMENT_FREE(element)) {
                mboxBuffer->buffer += HTC_HEADER_LEN;
                FRAME_EVENT(eventInfo, mboxBuffer->buffer, 
                            mboxBuffer->bufferLength, 
                            mboxBuffer->actualLength, 
                            A_ECANCELED, mboxBuffer->cookie);
                RECYCLE_DATA_REQUEST_ELEMENT(element);
                dispatchEvent(target, endPointId, HTC_BUFFER_SENT, &eventInfo);
            }
            HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND, 
                            "htcSendFrame - Exit\n");
            return;
        }
#ifdef HTC_SYNC
		else if (status == A_OK) {
			element->completionCB(element, status);
		}
#endif
        txCreditsAvailable -= 1;
        txCreditsConsumed += 1;

#ifdef DEBUG
        txcreditsavailable[endPointId] = txCreditsAvailable;
        txcreditsconsumed[endPointId] = txCreditsConsumed;
#endif /* DEBUG */

        if (!txCreditsAvailable) {

            AR_DEBUG_ASSERT(txCreditsConsumed);

            /* 
             * Instead of taking an interrupt we can just poll for more
             * credits that might have been queued up by now.
             */
            HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, 
                              HIF_ASYNCHRONOUS, HIF_BYTE_BASIS, 
                              HIF_FIXED_ADDRESS);
            address = getRegAddr(TX_CREDIT_COUNTER_DECREMENT_REG, endPointId);
            element = allocateRegRequestElement(target);
            AR_DEBUG_ASSERT(element != NULL);
#ifdef ONLY_16BIT
            FILL_REG_BUFFER(element, &endPoint->txCreditsAvailable[1], 
                            txCreditsConsumed*2, TX_CREDIT_COUNTER_DECREMENT_REG,
                            endPointId);
            status = HIFReadWrite(target->device, address, 
                                  (A_UCHAR *)&endPoint->txCreditsAvailable[1],
                                  txCreditsConsumed*2, &request, element);
#else
            FILL_REG_BUFFER(element, &endPoint->txCreditsAvailable[1], 
                            txCreditsConsumed, TX_CREDIT_COUNTER_DECREMENT_REG,
                            endPointId);
            status = HIFReadWrite(target->device, address, 
                                  &endPoint->txCreditsAvailable[1],
                                  txCreditsConsumed, &request, element);
#endif
#ifndef HTC_SYNC
            AR_DEBUG_ASSERT(status == A_OK);
            HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND, 
                            "htcSendFrame - Exit\n");
            return;
#else
            AR_DEBUG_ASSERT(status == A_OK || status == A_PENDING);
			if ( status == A_OK ) {

				HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                        "Critical Section (credit): LOCK at line %d in file %s							 \n", __LINE__, __FILE__);
        		A_MUTEX_LOCK(&creditCS);

    			regBuffer = GET_REG_BUFFER(element);
        		/* Calculate the number of credits available */
#ifdef ONLY_16BIT
        		AR_DEBUG_ASSERT((GET_TX_CREDITS_CONSUMED(endPoint) * 2) == \
					regBuffer->length);
#else
        		AR_DEBUG_ASSERT(GET_TX_CREDITS_CONSUMED(endPoint) == \
					regBuffer->length);
#endif
        		SET_TX_CREDITS_AVAILABLE(endPoint, regBuffer->buffer[0] - 
                	GET_TX_CREDITS_CONSUMED(endPoint));
        		SET_TX_CREDITS_CONSUMED(endPoint, 0);
        		txCreditsAvailable = GET_TX_CREDITS_AVAILABLE(endPoint);
        		txCreditsConsumed = GET_TX_CREDITS_CONSUMED(endPoint);
		        SET_TX_CREDITS_AVAILABLE(endPoint, 0);
    			SET_TX_CREDITS_CONSUMED(endPoint, txCreditsConsumed + txCreditsAvailable);
        		HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                        "Critical Section (credit): UNLOCK at line %d in file %s\n", __LINE__, __FILE__);
        		A_MUTEX_UNLOCK(&creditCS);

        		HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_SEND, 
                        "Pulling %d tx credits from the target\n", 
                        txCreditsAvailable);

				#ifdef DEBUG
        			txcreditsavailable[endPointId] = txCreditsAvailable;
        			txcreditsconsumed[endPointId] = txCreditsConsumed;
				#endif /* DEBUG */

    			freeRegRequestElement(element);

				if (!txCreditsAvailable) {

            	/* Enable the Tx credit counter interrupt so that we can get 
				 * the credits posted by the target */

            		htcEnableCreditCounterInterrupt(target, endPointId);

					/* Counter Interrupts have been enabled if 
				 	 * txCreditsAvailable is still 0 after polling. We need to 
				 	 * return here as there is nothing we can send till we get 
				 	 * a Counter Interrupt.
			 	 	 */
					return;
				}
			}
#endif
        }
    }

    HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                    "Critical Section (credit): LOCK at line %d in file %s\n", __LINE__, __FILE__);
    A_MUTEX_LOCK(&creditCS);
    SET_TX_CREDITS_AVAILABLE(endPoint, txCreditsAvailable);
    SET_TX_CREDITS_CONSUMED(endPoint, txCreditsConsumed);
    HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                    "Critical Section (credit): UNLOCK at line %d in file %s\n", __LINE__, __FILE__);
    A_MUTEX_UNLOCK(&creditCS);

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND, "htcSendFrame - Exit\n");
}

void
htcSendBlkSize(HTC_ENDPOINT *endPoint)
{
    A_STATUS status;
    A_UINT32 address;
    HTC_TARGET *target;
    HIF_REQUEST request;
    HTC_ENDPOINT_ID endPointId;
    HTC_QUEUE_ELEMENT *element;
    HTC_MBOX_BUFFER *mboxBuffer;
    HTC_DATA_REQUEST_QUEUE *sendQueue;
    HTC_REG_REQUEST_LIST *regList;

    /* Get the context */
    AR_DEBUG_ASSERT(endPoint != NULL);
    target = endPoint->target;
    AR_DEBUG_ASSERT(target != NULL);
    regList = &target->regList;
    AR_DEBUG_ASSERT(regList != NULL);
    sendQueue = &endPoint->sendQueue;
    AR_DEBUG_ASSERT(sendQueue != NULL);
    endPointId = GET_ENDPOINT_ID(endPoint);

    /* Decrement the tx credit count */
    AR_DEBUG_ASSERT(endPoint->txCreditsConsumed == 0);
    endPoint->txCreditsConsumed = 1;
    HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, HIF_ASYNCHRONOUS, 
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
    address = getRegAddr(TX_CREDIT_COUNTER_DECREMENT_REG, endPointId);
    element = allocateRegRequestElement(target);
    AR_DEBUG_ASSERT(element != NULL);
#ifdef ONLY_16BIT
    FILL_REG_BUFFER(element, &endPoint->txCreditsAvailable[1],
                    endPoint->txCreditsConsumed*2,
                    TX_CREDIT_COUNTER_DECREMENT_REG, endPointId);
    status = HIFReadWrite(target->device, address, 
                          (A_UCHAR *)&endPoint->txCreditsAvailable[1], 
                          endPoint->txCreditsConsumed*2, &request, element);
#else
    FILL_REG_BUFFER(element, &endPoint->txCreditsAvailable[1],
                    endPoint->txCreditsConsumed,
                    TX_CREDIT_COUNTER_DECREMENT_REG, endPointId);
    status = HIFReadWrite(target->device, address, 
                          &endPoint->txCreditsAvailable[1], 
                          endPoint->txCreditsConsumed, &request, element);
#endif

#ifndef HTC_SYNC
    AR_DEBUG_ASSERT(status == A_OK);
#else
    AR_DEBUG_ASSERT(status == A_OK || status == A_PENDING);
	if (status == A_OK) {
		element->completionCB(element, status);
	}
#endif

    /* Negotiate the maximum block size for the endpoint */
    addToMboxQueue(sendQueue, (A_UCHAR *)&endPoint->blockSize, 
                   sizeof(endPoint->blockSize), sizeof(endPoint->blockSize), 
                   NULL);
    element = removeFromMboxQueue(sendQueue);
    element->completionCB = htcBlkSzNegCompletionCB;
    mboxBuffer = GET_MBOX_BUFFER(element);
    HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_ASYNCHRONOUS, 
                      HIF_BYTE_BASIS, HIF_INCREMENTAL_ADDRESS);
    address = HIF_MBOX_START_ADDR(endPointId);
    status = HIFReadWrite(target->device, address, mboxBuffer->buffer, 
                          mboxBuffer->bufferLength, &request, element);
#ifndef HTC_SYNC
    AR_DEBUG_ASSERT(status == A_OK);
#else
	AR_DEBUG_ASSERT(status == A_OK || status == A_PENDING);
	if (status == A_OK) {
		element->completionCB(element, status);
	}
#endif

    HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_SEND , 
                    "Mailbox(%d), Block size: %d\n", 
                    endPointId, endPoint->blockSize);
}
