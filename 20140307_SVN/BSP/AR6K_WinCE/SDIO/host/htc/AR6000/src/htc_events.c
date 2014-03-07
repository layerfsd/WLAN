//------------------------------------------------------------------------------
// <copyright file="htc_events.c" company="Atheros">
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
// This file contains the routines handling the different events and callbacks
// from the hardware interface layer.
//
// Author(s): ="Atheros"
//==============================================================================
 

#include "htc_internal.h"

/* ------ Global Variable Declarations ------- */
extern A_MUTEX_T instanceCS, counterCS, creditCS,txCS;
extern A_WAITQUEUE_HEAD htcEvent;

#ifdef DEBUG
extern A_UINT32 debughtc;
extern A_UINT32 txcreditsavailable[HTC_MAILBOX_NUM_MAX];
extern A_UINT32 txcreditsconsumed[HTC_MAILBOX_NUM_MAX];
extern A_UINT32 txcreditintrenable[HTC_MAILBOX_NUM_MAX];
extern A_UINT32 txcreditintrenableaggregate[HTC_MAILBOX_NUM_MAX];
#endif

extern A_UINT32 tx_complete[HTC_MAILBOX_NUM_MAX]; /* Num of tx complete */

/* ------ Static Variables ------ */


/* ------ Functions ------ */


A_STATUS htcInterruptEnabler(HIF_DEVICE *device) {

	A_STATUS status;
    A_UINT32 address;
    HIF_REQUEST request;
	HTC_TARGET *target;
	HTC_REG_REQUEST_ELEMENT *element;

    target = getTargetInstance(device);
    AR_DEBUG_ASSERT(target != NULL);
    HTC_DEBUG_PRINTF(ATH_LOG_TRC, 
                    "htcInterruptEnabler Enter target: 0x%p\n", target);

	target->table.int_status_enable = INT_STATUS_ENABLE_ERROR_SET(0x01) |
                                      INT_STATUS_ENABLE_CPU_SET(0x01) |
                                      INT_STATUS_ENABLE_COUNTER_SET(0x01) |
                                      INT_STATUS_ENABLE_MBOX_DATA_SET(0x0F);
	/* Reenable Dragon Interrupts */
	element = allocateRegRequestElement(target);
    AR_DEBUG_ASSERT(element != NULL);
#ifdef ONLY_16BIT
    FILL_REG_BUFFER(element, (A_UINT16 *)&target->table.int_status_enable, 2, 
                    INT_STATUS_ENABLE_REG, ENDPOINT_UNUSED);
#else
    FILL_REG_BUFFER(element, &target->table.int_status_enable, 1, 
                    INT_STATUS_ENABLE_REG, ENDPOINT_UNUSED);
#endif

	HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_ASYNCHRONOUS, 
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
    address = getRegAddr(INT_STATUS_ENABLE_REG, ENDPOINT_UNUSED);
#ifdef ONLY_16BIT
    status = HIFReadWrite(target->device, address, 
                          &target->table.int_status_enable, 2, 
                          &request, element);
#else
    status = HIFReadWrite(target->device, address, 
                          &target->table.int_status_enable, 1, 
                          &request, element);
#endif

#ifndef HTC_SYNC
	AR_DEBUG_ASSERT(status == A_OK);
#else
	AR_DEBUG_ASSERT(status == A_OK || status == A_PENDING);
	if ( status == A_OK ) {
		element->completionCB(element, status);
	}
#endif //HTC_SYNC


    HTC_DEBUG_PRINTF(ATH_LOG_TRC,"htcInterruptEnabler Exit\n");

	return A_OK;
}

A_STATUS
htcRWCompletionHandler(void *context, 
                       A_STATUS status)
{
    HTC_QUEUE_ELEMENT *element;

    element = (HTC_QUEUE_ELEMENT *)context;
    AR_DEBUG_ASSERT(element != NULL);

    return(element->completionCB(element, status));
}

A_STATUS 
htcTxCompletionCB(HTC_DATA_REQUEST_ELEMENT *element,
                  A_STATUS status) 
{
    HTC_TARGET *target;
    HTC_ENDPOINT_ID endPointId;
    HTC_ENDPOINT *endPoint;
    HTC_EVENT_INFO eventInfo;
    HTC_MBOX_BUFFER *mboxBuffer;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND, 
                    "htcTxCompletionCB - Enter\n");

    /* Get the context */
    mboxBuffer = GET_MBOX_BUFFER(element);
    AR_DEBUG_ASSERT(mboxBuffer != NULL);
    endPoint = mboxBuffer->endPoint;
    AR_DEBUG_ASSERT(endPoint != NULL);
    target = endPoint->target;
    AR_DEBUG_ASSERT(target != NULL);
    endPointId = GET_ENDPOINT_ID(endPoint);

    HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_SEND,
                    "mboxBuffer: 0x%p, buffer: 0x%p, endPoint(%d): 0x%p, target: 0x%p\n", mboxBuffer, mboxBuffer->buffer, endPointId, endPoint, target);

    /* Return the buffer to the user if the transmission was not successful */
    if (status != A_OK) {
        HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_SEND, 
                        "Frame transmission failed\n");
        HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_SEND, 
                        "EndPoint: %d, Tx credits available: %d\n", 
                         endPointId, GET_TX_CREDITS_AVAILABLE(endPoint));
        /* 
         * In the failure case it is possible that while queueing of the 
         * request itself it returned an error status in which case we 
         * would have dispatched an event and freed the element there 
         * itself. Ideally if it failed to queue the request then it 
         * should not generate a callback but we are being a little 
         * conservative.
         */
        if (!(IS_ELEMENT_FREE(element))) {
            mboxBuffer->buffer += HTC_HEADER_LEN;
            FRAME_EVENT(eventInfo, mboxBuffer->buffer, 
                        mboxBuffer->bufferLength, mboxBuffer->actualLength, 
                        A_ECANCELED, mboxBuffer->cookie);
            RECYCLE_DATA_REQUEST_ELEMENT(element);
            dispatchEvent(target, endPointId, HTC_BUFFER_SENT, &eventInfo);
            HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND, 
                            "htcTxCompletionCB - Exit\n");
        }
        return A_OK;
    }

    HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_SEND, 
                    "Frame transmission complete\n");

    /* 
     * The user should see the actual length and buffer length
     * to be the same. In case of block mode, we use the actual length
     * parameter to reflect the total number of bytes transmitted after
     * padding.
     */
    mboxBuffer->actualLength = mboxBuffer->bufferLength;
    mboxBuffer->buffer += HTC_HEADER_LEN;

    /* 
     * Return the transmit buffer to the user through the HTC_BUFFER_SENT 
     * event indicating that the frame was transmitted successfully.
     */
    FRAME_EVENT(eventInfo, mboxBuffer->buffer, mboxBuffer->bufferLength, 
                mboxBuffer->actualLength, A_OK, mboxBuffer->cookie);
    RECYCLE_DATA_REQUEST_ELEMENT(element);

    tx_complete[endPointId] += 1;

    dispatchEvent(target, endPointId, HTC_BUFFER_SENT, &eventInfo);

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND, 
                    "htcTxCompletionCB - Exit\n");

    return A_OK;
}

A_STATUS
htcBlkSzNegCompletionCB(HTC_DATA_REQUEST_ELEMENT *element,
                        A_STATUS status)
{
    HTC_TARGET *target;
    HTC_ENDPOINT *endPoint;
    HIF_REQUEST request;
    HTC_MBOX_BUFFER *mboxBuffer;
    HTC_REG_REQUEST_ELEMENT *regElement;
    A_UINT32 address;

    /* Get the context */
    mboxBuffer = GET_MBOX_BUFFER(element);
    AR_DEBUG_ASSERT(mboxBuffer != NULL);
    endPoint = mboxBuffer->endPoint;
    AR_DEBUG_ASSERT(endPoint != NULL);
    target = endPoint->target;
    AR_DEBUG_ASSERT(target != NULL);

    /* Recycle the request element */
    RECYCLE_DATA_REQUEST_ELEMENT(element);
    element->completionCB = htcTxCompletionCB;

    if (status == A_OK) {
        /* Mark the state to be ready */
        endPoint->enabled = TRUE;
        
        /* Set the state of the target as ready */
        if (target->endPoint[ENDPOINT1].enabled &&
            target->endPoint[ENDPOINT2].enabled &&
            target->endPoint[ENDPOINT3].enabled &&
            target->endPoint[ENDPOINT4].enabled )
        {
            /* Send the INT_WLAN interrupt to the target */
            target->table.int_wlan = 1;
            HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, 
                              HIF_ASYNCHRONOUS, HIF_BYTE_BASIS, 
                              HIF_FIXED_ADDRESS);
            address = getRegAddr(INT_WLAN_REG, ENDPOINT_UNUSED);
            regElement = allocateRegRequestElement(target);
            AR_DEBUG_ASSERT(regElement != NULL);
            FILL_REG_BUFFER(regElement, &target->table.int_wlan, sizeof(target->table.int_wlan),
                            INT_WLAN_REG, ENDPOINT_UNUSED);
            status = HIFReadWrite(target->device, address, 
                                  (A_UCHAR *)&target->table.int_wlan, 
                                  sizeof(target->table.int_wlan), &request, regElement);
#ifndef HTC_SYNC
            AR_DEBUG_ASSERT(status == A_OK);
#else
			AR_DEBUG_ASSERT(status == A_OK || status == A_PENDING);
			if(status == A_OK) {
				regElement->completionCB(regElement, status);
			}
#endif
        }
    }

    return A_OK;
}

A_STATUS
htcRxCompletionCB(HTC_DATA_REQUEST_ELEMENT *element, 
                  A_STATUS status)
{
    HTC_TARGET *target;
    HTC_ENDPOINT *endPoint;
    HTC_EVENT_INFO eventInfo;
    HTC_ENDPOINT_ID endPointId;
    HTC_MBOX_BUFFER *mboxBuffer;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, 
                    "htcRxCompletionCB - Enter\n");

    /* Get the context */
    mboxBuffer = GET_MBOX_BUFFER(element);
    AR_DEBUG_ASSERT(mboxBuffer != NULL);
    endPoint = mboxBuffer->endPoint;
    AR_DEBUG_ASSERT(endPoint != NULL);
    target = endPoint->target;
    AR_DEBUG_ASSERT(target != NULL);
    endPointId = GET_ENDPOINT_ID(endPoint);

    HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_RECV,
                    "mboxBuffer: 0x%p, buffer: 0x%p, endPoint(%d): 0x%p, target: 0x%p\n", mboxBuffer, mboxBuffer->buffer, endPointId, endPoint, target);

    /* Return the buffer to the user if the reception was not successful */
    if (status != A_OK) {
        HTC_DEBUG_PRINTF(ATH_LOG_ERR | ATH_LOG_RECV, 
                        "Frame reception failed\n");
        /* 
         * In the failure case it is possible that while queueing of the 
         * request itself it returned an error status in which case we 
         * would have dispatched an event and freed the element there 
         * itself. Ideally if it failed to queue the request then it 
         * should not generate a callback but we are being a little 
         * conservative.
         */
        if (!(IS_ELEMENT_FREE(element))) {
            mboxBuffer->actualLength = 0;
            mboxBuffer->buffer += HTC_HEADER_LEN;
            FRAME_EVENT(eventInfo, mboxBuffer->buffer, 
                        mboxBuffer->bufferLength, mboxBuffer->actualLength, 
                        A_ECANCELED, mboxBuffer->cookie);
            RECYCLE_DATA_REQUEST_ELEMENT(element);
            dispatchEvent(target, endPointId, HTC_BUFFER_RECEIVED, &eventInfo);
            HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, 
                            "htcRxCompletionCB - Exit\n");
        }
        return A_OK;
    }

    HTC_DEBUG_PRINTF(ATH_LOG_INF | ATH_LOG_RECV, 
                    "Frame reception complete\n");

    HTC_DEBUG_PRINTBUF(mboxBuffer->buffer, (A_UINT16)mboxBuffer->actualLength);

    /* 
     * Advance the pointer by the size of HTC header and pass the payload
     * pointer to the upper layer.
     */
    mboxBuffer->actualLength = ((mboxBuffer->buffer[0] << 0) | 
                                (mboxBuffer->buffer[1] << 8));
    mboxBuffer->buffer += HTC_HEADER_LEN;

    /* 
     * Frame the HTC_BUFFER_RECEIVED to the upper layer indicating that the 
     * packet has been succesfully received.
     */
    FRAME_EVENT(eventInfo, mboxBuffer->buffer, mboxBuffer->bufferLength, 
                mboxBuffer->actualLength, A_OK, mboxBuffer->cookie);

    /* Recycle the bufferElement structure */
    RECYCLE_DATA_REQUEST_ELEMENT(element);

    /* Dispatch the event */
    dispatchEvent(target, endPointId, HTC_BUFFER_RECEIVED, &eventInfo);

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, 
                    "htcRxCompletion - Exit\n");

    return A_OK;
}

A_STATUS
htcRegCompletionCB(HTC_REG_REQUEST_ELEMENT *element,
                   A_STATUS status) 
{
    A_STATUS ret;
    HTC_TARGET *target;
    HTC_ENDPOINT *endPoint;
    HTC_REG_BUFFER *regBuffer;
#ifdef ONLY_16BIT
    A_UINT16 txCreditsConsumed;
    A_UINT16 txCreditsAvailable;
#else
    A_UINT8 txCreditsConsumed;
    A_UINT8 txCreditsAvailable;
#endif
    HTC_ENDPOINT_ID endPointId;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_SEND | ATH_LOG_RECV, 
                    "htcRegCompletion - Enter\n");
    AR_DEBUG_ASSERT(status == A_OK);

    /* Get the context */
    AR_DEBUG_ASSERT(element != NULL);
    regBuffer = GET_REG_BUFFER(element);
    AR_DEBUG_ASSERT(regBuffer != NULL);
    target = regBuffer->target;
    AR_DEBUG_ASSERT(target != NULL);

    /* Identify the register and the operation responsible for the callback */
    ret = A_OK;
    switch(regBuffer->base) {
    case TX_CREDIT_COUNTER_DECREMENT_REG:
        HTC_DEBUG_PRINTF(ATH_LOG_INF, "TX_CREDIT_COUNTER_DECREMENT_REG\n");
        endPointId = regBuffer->offset;
        endPoint = &target->endPoint[endPointId];

        HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                        "Critical Section (credit): LOCK at line %d in file %s\n", __LINE__, __FILE__);
        A_MUTEX_LOCK(&creditCS);

        /* Calculate the number of credits available */
#ifdef ONLY_16BIT
        AR_DEBUG_ASSERT((GET_TX_CREDITS_CONSUMED(endPoint)*2) == regBuffer->length);
#else
       	AR_DEBUG_ASSERT(GET_TX_CREDITS_CONSUMED(endPoint) == regBuffer->length);
#endif
        AR_DEBUG_ASSERT(regBuffer->buffer[0] >= 
                        GET_TX_CREDITS_CONSUMED(endPoint));
        SET_TX_CREDITS_AVAILABLE(endPoint, regBuffer->buffer[0] - 
                                 GET_TX_CREDITS_CONSUMED(endPoint));
        SET_TX_CREDITS_CONSUMED(endPoint, 0);
        txCreditsAvailable = GET_TX_CREDITS_AVAILABLE(endPoint);
        txCreditsConsumed = GET_TX_CREDITS_CONSUMED(endPoint);
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

        if (txCreditsAvailable) {
            htcSendFrame(endPoint);
        } else {
            /* 
             * Enable the Tx credit counter interrupt so that we can get the
             * credits posted by the target.
             */

            htcEnableCreditCounterInterrupt(target, endPointId);

#ifdef DEBUG
            txcreditintrenable[endPointId] += 1;
            txcreditintrenableaggregate[endPointId] += 1;
#endif /* DEBUG */
        }
        break;

    case TX_CREDIT_COUNTER_RESET_REG:
        HTC_DEBUG_PRINTF(ATH_LOG_INF, "TX_CREDIT_COUNTER_RESET_REG\n");
        endPointId = regBuffer->offset;

        /* 
         * Enable the Tx credit counter interrupt so that we can get the
         * credits posted by the target.
         */
        htcEnableCreditCounterInterrupt(target, endPointId);

#ifdef DEBUG
        txcreditintrenable[endPointId] += 1;
        txcreditintrenableaggregate[endPointId] += 1;
#endif /* DEBUG */
        break;

    case COUNTER_INT_STATUS_ENABLE_REG:
        HTC_DEBUG_PRINTF(ATH_LOG_INF, "COUNTER_INT_STATUS_ENABLE: 0x%x\n",
                        target->table.counter_int_status_enable);
        break;

    case COUNTER_INT_STATUS_DISABLE_REG:
        HTC_DEBUG_PRINTF(ATH_LOG_INF, "COUNTER_INT_STATUS_DISABLE:0x%x\n",
                        target->table.counter_int_status_enable);
		
        HIFAckInterrupt(target->device);
        HTC_DEBUG_PRINTF(ATH_LOG_TRC, "htcDSRHandler - ACK\n");
        break;


    case INT_WLAN_REG:
        HTC_DEBUG_PRINTF(ATH_LOG_INF, "INT_WLAN: 0x%x\n",
                        target->table.int_wlan);
        target->table.int_wlan = 0;

        /* Mark the target state as ready and signal the waiting sem */
        target->ready = TRUE;
        A_WAKE_UP(&htcEvent);
        break;

	case INT_STATUS_ENABLE_REG:
        HTC_DEBUG_PRINTF(ATH_LOG_INF, "INT_STATUS_ENABLE: 0x%x\n",
                        target->table.int_status_enable);
        break;


    default:
        HTC_DEBUG_PRINTF(ATH_LOG_ERR, 
                        "Invalid register address: %d\n", regBuffer->base);
    }

    /* Free the register request structure */
    freeRegRequestElement(element);

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "htcRegCompletion - Exit\n");

    return ret;
}


A_STATUS
htcTargetInsertedHandler(HIF_DEVICE *device)
{
    HTC_TARGET *target;
    HTC_ENDPOINT *endPoint;
    A_UINT8 count1, count2;
    HTC_EVENT_INFO eventInfo;
    HTC_REG_BUFFER *regBuffer;
    HTC_QUEUE_ELEMENT *element;
    HTC_MBOX_BUFFER *mboxBuffer;
    HTC_REG_REQUEST_LIST *regList;
    HTC_DATA_REQUEST_QUEUE *sendQueue, *recvQueue;
	HIF_REQUEST request;
	A_STATUS status;
    A_UINT32 address;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "htcTargetInserted - Enter\n");

    /* Initialize the locks */
    A_MUTEX_INIT(&instanceCS);
    A_MUTEX_INIT(&creditCS);
    A_MUTEX_INIT(&counterCS);
    A_MUTEX_INIT(&txCS);

    /* Allocate target memory */
    if ((target = (HTC_TARGET *)A_MALLOC(sizeof(HTC_TARGET))) == NULL) {
        HTC_DEBUG_PRINTF(ATH_LOG_ERR, "Unable to allocate memory\n");
        return A_ERROR;
    }
    A_MEMZERO(target, sizeof(HTC_TARGET));
    target->device = device;
    target->ready = FALSE;

    /* Initialize the endpoints, mbox queues, event table */
    for (count1 = ENDPOINT1; count1 <= ENDPOINT4; count1 ++) {
        endPoint = &target->endPoint[count1];
        HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                        "endPoint[%d]: %p\n", count1, endPoint);
        A_MEMZERO(endPoint->txCreditsAvailable, HTC_TX_CREDITS_NUM_MAX);
        endPoint->txCreditsConsumed = 0;
        endPoint->txCreditsIntrEnable = FALSE;
        endPoint->rxLengthPending = 0;
        endPoint->target = target;
        endPoint->enabled = FALSE;
        for (count2 = 0; count2<HTC_DATA_REQUEST_RING_BUFFER_SIZE; count2 ++) {
            /* Send Queue */
            sendQueue = &endPoint->sendQueue;
            sendQueue->head = sendQueue->size = 0;
            element = &sendQueue->element[count2];
            A_MEMZERO(element, sizeof(HTC_DATA_REQUEST_ELEMENT));
            element->buffer.free = TRUE;
            element->completionCB = htcTxCompletionCB;
            mboxBuffer = GET_MBOX_BUFFER(element);
            mboxBuffer->endPoint = endPoint;

            /* Receive Queue */
            recvQueue = &endPoint->recvQueue;
            recvQueue->head = recvQueue->size = 0;
            element = &recvQueue->element[count2];
            A_MEMZERO(element, sizeof(HTC_DATA_REQUEST_ELEMENT));
            element->buffer.free = TRUE;
            element->completionCB = htcRxCompletionCB;
            mboxBuffer = GET_MBOX_BUFFER(element);
            mboxBuffer->endPoint = endPoint;
        }
        A_MEMZERO(&target->endPoint[count1].eventTable, 
                  sizeof(HTC_ENDPOINT_EVENT_TABLE));
    }

    /* Populate the block size for each of the end points */
    endPoint = &target->endPoint[ENDPOINT1];
    endPoint->blockSize = HIF_MBOX0_BLOCK_SIZE;
    endPoint = &target->endPoint[ENDPOINT2];
    endPoint->blockSize = HIF_MBOX1_BLOCK_SIZE;
    endPoint = &target->endPoint[ENDPOINT3];
    endPoint->blockSize = HIF_MBOX2_BLOCK_SIZE;
    endPoint = &target->endPoint[ENDPOINT4];
    endPoint->blockSize = HIF_MBOX3_BLOCK_SIZE;

    /* Initialize the shadow copy of the target register table */
    A_MEMZERO(&target->table, sizeof(HTC_REGISTER_TABLE));

    /* Initialize the register request list */
    regList = &target->regList;
    for (count1 = 0; count1 < HTC_REG_REQUEST_LIST_SIZE; count1 ++) {
        element = &regList->element[count1];
        A_MEMZERO(element, sizeof(HTC_REG_REQUEST_ELEMENT));
        element->buffer.free = TRUE;
        element->completionCB = htcRegCompletionCB;
        regBuffer = GET_REG_BUFFER(element);
        regBuffer->target = target;
    }

    /* Add the target instance to the global list */
    addTargetInstance(target);

    /* Disable all the dragon interrupts */
    target->table.int_status_enable = 0;
    target->table.cpu_int_status_enable = 0;
    target->table.error_status_enable = 0;
    target->table.counter_int_status_enable = 0;
    HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_SYNCHRONOUS,
                      HIF_BYTE_BASIS, HIF_INCREMENTAL_ADDRESS);
    address = getRegAddr(INT_STATUS_ENABLE_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, 
                          &target->table.int_status_enable, 4, &request, NULL);
    AR_DEBUG_ASSERT(status == A_OK);

    /* 
     * Frame a TARGET_AVAILABLE event and send it to the host. Return the
     * HIF_DEVICE handle as a parameter with the event.
     */
    FRAME_EVENT(eventInfo, (A_UCHAR *)device, sizeof(HIF_DEVICE *), 
                sizeof(HIF_DEVICE *), A_OK, NULL);
    dispatchEvent(target, ENDPOINT_UNUSED, HTC_TARGET_AVAILABLE, &eventInfo);

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "htcTargetInserted - Exit\n");

    return A_OK;
}

A_STATUS
htcTargetRemovedHandler(HIF_DEVICE *device)
{
    HTC_TARGET *target;
    HTC_EVENT_INFO eventInfo;

    /* Get the target instance bound to this device */
    target = getTargetInstance(device);

    if (target != NULL) {
        /* Frame a TARGET_UNAVAILABLE event and send it to the host */
        FRAME_EVENT(eventInfo, NULL, 0, 0, A_OK, NULL);
        dispatchEvent(target, ENDPOINT_UNUSED, HTC_TARGET_UNAVAILABLE, 
                      &eventInfo);
    }

    A_MUTEX_DESTROY(&txCS);
    A_MUTEX_DESTROY(&counterCS);
    A_MUTEX_DESTROY(&creditCS);
    A_MUTEX_DESTROY(&instanceCS);
       
    return A_OK;
}

/* For shared interrupts, it is unsafe to update the shadow copies of the int_status and int_status_enable values 
 * since, the ar6kisrhandler can be called at any time while the DSR is running due to shared nature of interrupts.
 * Hence modified to simply read the values off the hardware directly 
 */

A_STATUS
htcInterruptPending(HIF_DEVICE *device, A_BOOL *intPending)
{
    A_STATUS status;
    A_UINT32 address;
    HTC_TARGET *target;
    HIF_REQUEST request;
    A_UCHAR      intStatus[2] = {0,0};
    A_UCHAR      intMask[2] = {0,0};

    target = getTargetInstance(device);
    AR_DEBUG_ASSERT(target != NULL);
    HTC_DEBUG_PRINTF(ATH_LOG_TRC, 
                    "htcInterruptPending Enter target: 0x%p\n", target);

    // get the current interrupt status register
    HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, HIF_SYNCHRONOUS,
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
    address = getRegAddr(INT_STATUS_REG, ENDPOINT_UNUSED);

#ifdef ONLY_16BIT
    status = HIFReadWrite(target->device, address, 
                          intStatus, 2, &request, NULL);
#else
    status = HIFReadWrite(target->device, address, 
                          intStatus, 1, &request, NULL);
#endif
    AR_DEBUG_ASSERT(status == A_OK);

    // get the interrupt enable register value 
    HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, HIF_SYNCHRONOUS,
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);

    address = getRegAddr(INT_STATUS_ENABLE_REG, ENDPOINT_UNUSED);

#ifdef ONLY_16BIT
    status = HIFReadWrite(target->device, address, 
                          intMask, 2, &request, NULL);
#else
    status = HIFReadWrite(target->device, address, 
                          intMask, 1, &request, NULL);
#endif
    AR_DEBUG_ASSERT(status == A_OK);
    if (!((intMask[0] & intStatus[0]) == 0)) {
	    *intPending = TRUE;
    } else {
	    *intPending = FALSE;
    }
    return A_OK;
}


A_STATUS
htcInterruptDisabler(HIF_DEVICE *device,A_BOOL *callDsr)
{
    A_STATUS status;
    A_UINT32 address;
    HTC_TARGET *target;
    HIF_REQUEST request;
    A_BOOL interruptPending;

    target = getTargetInstance(device);
    AR_DEBUG_ASSERT(target != NULL);
	
    HTC_DEBUG_PRINTF(ATH_LOG_TRC, 
	                    "htcInterruptDisabler Enter target: 0x%p\n", target);
    
    // Check for spurious interrupt
    status = htcInterruptPending (device, &interruptPending);
		
    if (!interruptPending){
        *callDsr=FALSE; 
    } else {
	    /* 
         * Disable the interrupts from Dragon.
         *  We do the interrupt servicing in the bottom half and reenable the
         *  Dragon interrupts at the end of the bottom-half
	     */
   
        target->table.int_status_enable = 0;
        HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_SYNCHRONOUS,
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
        address = getRegAddr(INT_STATUS_ENABLE_REG, ENDPOINT_UNUSED);
#ifdef ONLY_16BIT
        status = HIFReadWrite(target->device, address, 
                          &target->table.int_status_enable, 2, &request, NULL);
#else
        status = HIFReadWrite(target->device, address, 
                          &target->table.int_status_enable, 1, &request, NULL);
#endif
        AR_DEBUG_ASSERT(status == A_OK);
        *callDsr=TRUE;
    }

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "htcInterruptDisabler: Exit\n");
    return A_OK;
}

A_STATUS
htcDSRHandler(HIF_DEVICE *device)
{
    A_STATUS status;
    A_UINT32 address;
    HTC_TARGET *target;
    HIF_REQUEST request;
    A_UCHAR host_int_status;

    target = getTargetInstance(device);
    AR_DEBUG_ASSERT(target != NULL);
    HTC_DEBUG_PRINTF(ATH_LOG_TRC, 
                    "htcDsrHandler: Enter (target: 0x%p\n", target);

    /* 
     * Read the first 28 bytes of the HTC register table. This will yield us
     * the value of different int status registers and the lookahead 
     * registers.
     *    length = sizeof(int_status) + sizeof(cpu_int_status) + 
     *             sizeof(error_int_status) + sizeof(counter_int_status) + 
     *             sizeof(mbox_frame) + sizeof(rx_lookahead_valid) + 
     *             sizeof(hole) +  sizeof(rx_lookahead) +
     *             sizeof(int_status_enable) + sizeof(cpu_int_status_enable) +
     *             sizeof(error_status_enable) + 
     *             sizeof(counter_int_status_enable);
     */
    HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, HIF_SYNCHRONOUS, 
                      HIF_BYTE_BASIS, HIF_INCREMENTAL_ADDRESS);
    address = getRegAddr(INT_STATUS_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, 
                          &target->table.host_int_status, 28, 
                          &request, NULL);
    AR_DEBUG_ASSERT(status == A_OK);

#ifdef DEBUG
    dumpRegisters(target);
#endif /* DEBUG */
    
    /* Update only those registers that are enabled */
	/* This is not required as we have Already checked for 
	 * spuriours interrupt in htcInterruptDisabler
	 */
    host_int_status = target->table.host_int_status;// &
                      //target->table.int_status_enable;
		
    HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                    "Valid interrupt source(s) in INT_STATUS: 0x%x\n", 
                    host_int_status);

    if (HOST_INT_STATUS_CPU_GET(host_int_status)) {
        /* CPU Interrupt */
        htcServiceCPUInterrupt(target);
    }
 
    if (HOST_INT_STATUS_ERROR_GET(host_int_status)) {
        /* Error Interrupt */
        htcServiceErrorInterrupt(target);
    }

    if (HOST_INT_STATUS_MBOX_DATA_GET(host_int_status)) {
        /* Mailbox Interrupt */
        htcServiceMailboxInterrupt(target);
    }


    if (HOST_INT_STATUS_COUNTER_GET(host_int_status)) {
        /* Counter Interrupt */
        htcServiceCounterInterrupt(target);
    } else {
        /* Ack the interrupt */
        HIFAckInterrupt(target->device);

    }

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "htcDSRHandler: Exit\n");
    return A_OK;
}

void
htcServiceCPUInterrupt(HTC_TARGET *target)
{
    A_STATUS status;
    A_UINT32 address;
    HIF_REQUEST request;
    A_UINT8 cpu_int_status;

    HTC_DEBUG_PRINTF(ATH_LOG_INF, "CPU Interrupt\n");
    cpu_int_status = target->table.cpu_int_status &
                     target->table.cpu_int_status_enable;
    AR_DEBUG_ASSERT(cpu_int_status);
    HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                    "Valid interrupt source(s) in CPU_INT_STATUS: 0x%x\n",
                    cpu_int_status);

    /* Figure out the interrupt number */
    HTC_DEBUG_PRINTF(ATH_LOG_INF, "Interrupt Number: 0x%x\n", 
                    htcGetBitNumSet(cpu_int_status));

    /* Clear the interrupt */
    target->table.cpu_int_status = cpu_int_status; /* W1C */
    HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_SYNCHRONOUS, 
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
    address = getRegAddr(CPU_INT_STATUS_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, 
                          &target->table.cpu_int_status, 1, &request, NULL);

    AR_DEBUG_ASSERT(status == A_OK);
}


void
htcServiceErrorInterrupt(HTC_TARGET *target)
{
    A_STATUS status;
    A_UINT32 address;
    HIF_REQUEST request;
    A_UINT8 error_int_status;

    HTC_DEBUG_PRINTF(ATH_LOG_INF, "Error Interrupt\n");
    error_int_status = target->table.error_int_status &
                       target->table.error_status_enable;
    AR_DEBUG_ASSERT(error_int_status);
    HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                    "Valid interrupt source(s) in ERROR_INT_STATUS: 0x%x\n",
                    error_int_status);

    if (ERROR_INT_STATUS_WAKEUP_GET(error_int_status)) {
        /* Wakeup */
        HTC_DEBUG_PRINTF(ATH_LOG_INF, "Wakeup\n");
    }
        
    if (ERROR_INT_STATUS_RX_UNDERFLOW_GET(error_int_status)) {
        /* Rx Underflow */
        HTC_DEBUG_PRINTF(ATH_LOG_INF, "Rx Underflow\n");
    }
        
    if (ERROR_INT_STATUS_TX_OVERFLOW_GET(error_int_status)) {
        /* Tx Overflow */
        HTC_DEBUG_PRINTF(ATH_LOG_INF, "Tx Overflow\n");
    }

    /* Clear the interrupt */
    target->table.error_int_status = error_int_status; /* W1C */
    HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_SYNCHRONOUS, 
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
    address = getRegAddr(ERROR_INT_STATUS_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, 
                          &target->table.error_int_status, 1,
                          &request, NULL);

    AR_DEBUG_ASSERT(status == A_OK);
}

void
htcServiceCounterInterrupt(HTC_TARGET *target)
{
    A_STATUS status;
    A_UINT32 address;
    HIF_REQUEST request;
    HTC_ENDPOINT *endPoint;
    HTC_ENDPOINT_ID endPointId;
    A_UINT8 counter_int_status;
    A_UINT8 reset_credit_int_status;
    A_UINT8 update_credit_int_status;
    HTC_REG_REQUEST_ELEMENT *element;

    HTC_DEBUG_PRINTF(ATH_LOG_INF, "Counter Interrupt\n");

    counter_int_status = target->table.counter_int_status &
                         target->table.counter_int_status_enable;
    AR_DEBUG_ASSERT(counter_int_status);
    HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                    "Valid interrupt source(s) in COUNTER_INT_STATUS: 0x%x\n",
                    counter_int_status);

    /* Service the reset credit counter interrupt */
    reset_credit_int_status = (counter_int_status & 0x0F);
    while(reset_credit_int_status) {
        endPointId = htcGetBitNumSet(reset_credit_int_status);
        endPoint = &target->endPoint[endPointId];
        AR_DEBUG_ASSERT(endPoint != NULL);

        HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                        "endPoint(%d): %p\n", endPointId, endPoint);

        /* Initialize the number of credits available to zero */
	    HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                   "Critical Section (credit): LOCK at line %d in file %s\n", __LINE__, __FILE__);
		A_MUTEX_LOCK(&creditCS);
        SET_TX_CREDITS_AVAILABLE(endPoint, 0);
	    HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                  "Critical Section (credit): UNLOCK at line %d in file %s\n", __LINE__, __FILE__);
		A_MUTEX_UNLOCK(&creditCS);

        /* Clear the interrupt */
        HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, 
                          HIF_ASYNCHRONOUS, HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
        address = getRegAddr(TX_CREDIT_COUNTER_RESET_REG, endPointId);
        element = allocateRegRequestElement(target);
        AR_DEBUG_ASSERT(element != NULL);
        FILL_REG_BUFFER(element, &endPoint->txCreditsAvailable[1], sizeof(endPoint->txCreditsAvailable[1]),
                        TX_CREDIT_COUNTER_RESET_REG, endPointId);
        status = HIFReadWrite(target->device, address, 
                              (A_UCHAR *)&endPoint->txCreditsAvailable[1],
                              sizeof(endPoint->txCreditsAvailable[1]), &request, element);

#ifndef HTC_SYNC
        AR_DEBUG_ASSERT(status == A_OK);
#else
        AR_DEBUG_ASSERT(status == A_OK || status == A_PENDING);
		if (status == A_OK) {
        	/* Enable the Tx credit counter interrupt so that we can get the
             * credits posted by the target */
        	htcEnableCreditCounterInterrupt(target, endPointId);
		}
#endif
        reset_credit_int_status &= 
            ~(1 << htcGetBitNumSet(reset_credit_int_status));
    }

    /* Disable the credit counter interrupt */
    htcDisableCreditCounterInterrupt(target, ENDPOINT_UNUSED);

    /* Service the credit counter interrupt */
    update_credit_int_status = counter_int_status & 0xF0;
    while(update_credit_int_status) {
        endPointId = htcGetBitNumSet(update_credit_int_status) - 
                     HTC_MAILBOX_NUM_MAX;
        endPoint = &target->endPoint[endPointId];
        AR_DEBUG_ASSERT(endPoint != NULL);

        /* This is the minimum number of credits that we would have got */
        AR_DEBUG_ASSERT(GET_TX_CREDITS_AVAILABLE(endPoint) == 0);
	    HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                   "Critical Section (credit): LOCK at line %d in file %s\n", __LINE__, __FILE__);
		A_MUTEX_LOCK(&creditCS);
        SET_TX_CREDITS_AVAILABLE(endPoint, 1);
		HTC_DEBUG_PRINTF(ATH_LOG_SYNC,
                   "Critical Section (credit): UNLOCK at line %d in file %s\n", __LINE__, __FILE__);
		A_MUTEX_UNLOCK(&creditCS);


#ifdef DEBUG
        txcreditsavailable[endPointId] = GET_TX_CREDITS_AVAILABLE(endPoint);
        txcreditintrenable[endPointId] -= 1;
#endif /* DEBUG */

        HTC_DEBUG_PRINTF(ATH_LOG_INF, "Tx Credits Available: %d\n", 
                                        GET_TX_CREDITS_AVAILABLE(endPoint));

        if (!target->ready) {
            htcSendBlkSize(endPoint);
        } else {
            htcSendFrame(endPoint);
        }

        update_credit_int_status &= 
            ~(1 << htcGetBitNumSet(update_credit_int_status));
    }
}

void
htcEnableCreditCounterInterrupt(HTC_TARGET *target, 
                                HTC_ENDPOINT_ID endPointId)
{
    A_STATUS status;
    A_UINT32 address;
    HIF_REQUEST request;
    HTC_ENDPOINT *endPoint;
    HTC_REG_REQUEST_ELEMENT *element;
#ifdef ONLY_16BIT
	A_UINT16 counter_int_enable16;
#endif
	A_UCHAR	counter_int_enable;


    endPoint = &target->endPoint[endPointId];
    AR_DEBUG_ASSERT(endPoint != NULL);

    A_MUTEX_LOCK(&counterCS);

    endPoint->txCreditsIntrEnable = TRUE;
    HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, 
                      HIF_ASYNCHRONOUS, HIF_BYTE_BASIS, 
                      HIF_FIXED_ADDRESS);
    element = allocateRegRequestElement(target);
    AR_DEBUG_ASSERT(element != NULL);

    counter_int_enable = ((target->endPoint[0].txCreditsIntrEnable << (4)) |
                         (target->endPoint[1].txCreditsIntrEnable << (5)) |
                         (target->endPoint[2].txCreditsIntrEnable << (6)) |
                         (target->endPoint[3].txCreditsIntrEnable << (7)) |
                         0x0F);
	

#ifdef ONLY_16BIT
    counter_int_enable16 = (counter_int_enable << 8) | target->table.error_status_enable;

    FILL_REG_BUFFER(element, NULL, 2, COUNTER_INT_STATUS_ENABLE_REG, counter_int_enable16);
	address = getRegAddr(ERROR_STATUS_ENABLE_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, 
                          (A_UCHAR *)&((GET_REG_BUFFER(element))->offset),
                          2, &request, element);
#else
    FILL_REG_BUFFER(element, NULL, 1, COUNTER_INT_STATUS_ENABLE_REG, counter_int_enable);
    address = getRegAddr(COUNTER_INT_STATUS_ENABLE_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, 
                          (A_UCHAR *)&((GET_REG_BUFFER(element))->offset),
                          1, &request, element);
#endif

#ifndef HTC_SYNC
    AR_DEBUG_ASSERT(status == A_OK);
#else
    AR_DEBUG_ASSERT(status == A_OK || status == A_PENDING);
    if (status == A_OK) { 
        element->completionCB(element, status);
    }
#endif

    A_MUTEX_UNLOCK(&counterCS);
}

void
htcDisableCreditCounterInterrupt(HTC_TARGET *target, 
                                 HTC_ENDPOINT_ID unused)
{
    A_STATUS status;
    A_UINT32 address;
    HIF_REQUEST request;
    HTC_ENDPOINT *endPoint;
    HTC_ENDPOINT_ID endPointId;
    A_UINT8 update_credit_int_status;
    HTC_REG_REQUEST_ELEMENT *element;
#ifdef ONLY_16BIT
	A_UINT16 counter_int_enable16;
#endif
	A_UCHAR	counter_int_enable;

    A_MUTEX_LOCK(&counterCS);

    /* The Tx credit counter update bits are reflected in the upper nibble */
    update_credit_int_status = target->table.counter_int_status & 0xF0;

    while(update_credit_int_status) {
        endPointId = htcGetBitNumSet(update_credit_int_status) - 
                     HTC_MAILBOX_NUM_MAX;
        endPoint = &target->endPoint[endPointId];
        AR_DEBUG_ASSERT(endPoint != NULL);

        HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                        "endPoint(%d): %p\n", endPointId, endPoint);

        /* Disable the tx credit interrupt */
        endPoint->txCreditsIntrEnable = FALSE;

        update_credit_int_status &= 
            ~(1 << htcGetBitNumSet(update_credit_int_status));
    }


    HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_ASYNCHRONOUS, 
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);

    element = allocateRegRequestElement(target);
    AR_DEBUG_ASSERT(element != NULL);

    counter_int_enable = ((target->endPoint[0].txCreditsIntrEnable << (4)) |
						 (target->endPoint[1].txCreditsIntrEnable << (5)) |
						 (target->endPoint[2].txCreditsIntrEnable << (6)) |
                         (target->endPoint[3].txCreditsIntrEnable << (7)) | 0x0F);

#ifdef ONLY_16BIT
    counter_int_enable16 = (counter_int_enable << 8) | target->table.error_status_enable;

    FILL_REG_BUFFER(element, NULL, 2, COUNTER_INT_STATUS_DISABLE_REG, 
					counter_int_enable16);
	address = getRegAddr(ERROR_STATUS_ENABLE_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, 
                          (A_UCHAR *)&((GET_REG_BUFFER(element))->offset),
                          2, &request, element);
#else
    FILL_REG_BUFFER(element, NULL, 1, COUNTER_INT_STATUS_DISABLE_REG, 
					counter_int_enable);
	address = getRegAddr(COUNTER_INT_STATUS_DISABLE_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, 
                          (A_UCHAR *)&((GET_REG_BUFFER(element))->offset),
                          1, &request, element);
#endif

#ifndef HTC_SYNC
    AR_DEBUG_ASSERT(status == A_OK);
#else
    AR_DEBUG_ASSERT(status == A_OK || status == A_PENDING);
	if ( status == A_OK ) {
		element->completionCB(element, status);
	}
#endif

    A_MUTEX_UNLOCK(&counterCS);
}

void
htcServiceMailboxInterrupt(HTC_TARGET *target)
{
    A_STATUS status;
    A_UINT32 address;
    HIF_REQUEST request;
    HTC_ENDPOINT *endPoint;
    HTC_ENDPOINT_ID endPointId;
    A_UINT8 mailbox_int_status;

    HTC_DEBUG_PRINTF(ATH_LOG_INF, "Mailbox Interrupt\n");

    /* The Rx interrupt bits are reflected in the lower nibble */
    mailbox_int_status = target->table.host_int_status & 
                         HOST_INT_STATUS_MBOX_DATA_MASK;
    HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                    "Valid mailbox interrupt source(s) in INT_STATUS: 0x%x\n",
                    mailbox_int_status);

    /* Disable the receive interrupt for all four mailboxes */
    target->table.int_status_enable &= ~(HOST_INT_STATUS_MBOX_DATA_MASK);

    do {
        while(mailbox_int_status) {
            endPointId = htcGetBitNumSet(mailbox_int_status);
            endPoint = &target->endPoint[endPointId];
            AR_DEBUG_ASSERT(endPoint != NULL);
    
            HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                            "endPoint(%d): %p\n", endPointId, endPoint);

            /* Service the Rx interrupt */
            htcReceiveFrame(endPoint);
            mailbox_int_status &= ~(1 << htcGetBitNumSet(mailbox_int_status));
        }

        /* 
         * Read the register table again. Repeat the process until there are
         * no more valid packets queued up on receive. It is assumed that
         * the following request will be serialized along with the request
         * above and will be completed in the order in which it is received
         * by the bus driver.
         */
        HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, 
                          HIF_SYNCHRONOUS, HIF_BYTE_BASIS, 
                          HIF_INCREMENTAL_ADDRESS);
        address = getRegAddr(INT_STATUS_REG, ENDPOINT_UNUSED);
        status = HIFReadWrite(target->device, address, 
                              &target->table.host_int_status, 
                              24, &request, NULL);
        AR_DEBUG_ASSERT(status == A_OK);

	// Hack for CF card surprise removal crash.
	
	if(target->table.host_int_status == 0xff) {
	    target->table.host_int_status = 0;
	    return;
	}
        mailbox_int_status = target->table.host_int_status & 
                             HOST_INT_STATUS_MBOX_DATA_MASK;
    } while (mailbox_int_status);

    target->table.int_status_enable |= HOST_INT_STATUS_MBOX_DATA_MASK;
}
