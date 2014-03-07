//------------------------------------------------------------------------------
// <copyright file="htc.c" company="Atheros">
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
//    The software is licensed “as-is.” 
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// This file contains the HTC APIs that are exposed to higher layers.
//
// Author(s): ="Atheros"
//==============================================================================

#include "htc_internal.h"

/* ------ Global Variable Declarations ------- */
HTC_TARGET *AtherosTargetList[HIF_MAX_DEVICES];
HTC_GLOBAL_EVENT_TABLE AtherosEventTable;
A_MUTEX_T creditCS, counterCS, instanceCS,txCS;
A_WAITQUEUE_HEAD htcEvent;

#ifdef DEBUG
extern A_UINT32 txcreditsavailable[HTC_MAILBOX_NUM_MAX];
extern A_UINT32 txcreditsconsumed[HTC_MAILBOX_NUM_MAX];
extern A_UINT32 txcreditintrenable[HTC_MAILBOX_NUM_MAX];
extern A_UINT32 txcreditintrenableaggregate[HTC_MAILBOX_NUM_MAX];
#endif

extern int tx_attempt[HTC_MAILBOX_NUM_MAX];    /* Num of attempts to add */
extern int tx_post[HTC_MAILBOX_NUM_MAX];       /* Num of attemps succeded */
extern int tx_complete[HTC_MAILBOX_NUM_MAX];   /* Num of tx complete */

/* Initializes the HTC module */
A_STATUS
HTCInit(void)
{
    HTC_CALLBACKS htcCallbacks;
    static A_BOOL HTCInitialized = FALSE;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCInit: Enter\n");
    if (HTCInitialized) {
        HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCInit: Exit\n");
        return A_OK;
    }

    A_MEMZERO(&AtherosEventTable, sizeof(HTC_GLOBAL_EVENT_TABLE));
    A_MEMZERO(&htcCallbacks, sizeof(HTC_CALLBACKS));
    A_INIT_WAITQUEUE_HEAD(&htcEvent);

    htcCallbacks.deviceInsertedHandler = htcTargetInsertedHandler;
    htcCallbacks.deviceRemovedHandler = htcTargetRemovedHandler;
    htcCallbacks.rwCompletionHandler = htcRWCompletionHandler;
    htcCallbacks.deviceInterruptEnabler = htcInterruptEnabler;
    htcCallbacks.deviceInterruptDisabler = htcInterruptDisabler;
    htcCallbacks.dsrHandler = htcDSRHandler;

    HIFRegisterCallbacks(&htcCallbacks);
    HTCInitialized = TRUE;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCInit: Exit\n");
    return A_OK;
}


/* Enables Dragon interrupts */
A_STATUS
HTCStart(HTC_TARGET *target) 
{
    A_STATUS status;
    A_UINT32 address;
    HIF_REQUEST request;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCStart Enter\n");

    ResetEvent(htcEvent);

    /* Unmask the host controller interrupts */
    HIFUnMaskInterrupt(target->device);

    /* Enable all the interrupts except for the dragon interrupt */
    target->table.int_status_enable = INT_STATUS_ENABLE_ERROR_SET(0x01) |
                                      INT_STATUS_ENABLE_CPU_SET(0x01) |
                                      INT_STATUS_ENABLE_COUNTER_SET(0x01) |
                                      INT_STATUS_ENABLE_MBOX_DATA_SET(0x0F);

    /* Set up the CPU Interrupt Status Register */
    target->table.cpu_int_status_enable = CPU_INT_STATUS_ENABLE_BIT_SET(0x00);

    /* Set up the Error Interrupt Status Register */
    target->table.error_status_enable = 
                                  ERROR_STATUS_ENABLE_RX_UNDERFLOW_SET(0x01) |
                                  ERROR_STATUS_ENABLE_TX_OVERFLOW_SET(0x01);

    /* Set up the Counter Interrupt Status Register */
    target->table.counter_int_status_enable = 
                                    COUNTER_INT_STATUS_ENABLE_BIT_SET(0xFF);

    /* Write to the register */
    HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_SYNCHRONOUS,
                      HIF_BYTE_BASIS, HIF_INCREMENTAL_ADDRESS);
    address = getRegAddr(INT_STATUS_ENABLE_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, 
                          &target->table.int_status_enable, 4, &request, NULL);
    if (status != A_OK) {
        /* Can't write it for some reason */
        HTC_DEBUG_PRINTF(ATH_LOG_ERR,
                        "Failed to enable INT_STATUS_ENABLE | CPU_INT_STATUS_ENABLE | ERROR_STATUS_ENABLE | COUNTER_INT_STATUS_ENABLE, err: %d\n", status);
        HTCStop(target);
        return status;
    }

#ifdef DEBUG
    txcreditintrenable[ENDPOINT1] += 1;
    txcreditintrenable[ENDPOINT2] += 1; 
    txcreditintrenable[ENDPOINT3] += 1; 
    txcreditintrenable[ENDPOINT4] += 1;
    txcreditintrenableaggregate[ENDPOINT1] += 1;
    txcreditintrenableaggregate[ENDPOINT2] += 1;
    txcreditintrenableaggregate[ENDPOINT3] += 1;
    txcreditintrenableaggregate[ENDPOINT4] += 1;
#endif /* DEBUG */

    /* Wait on a timed semaphore that will get signalled once the block
       size negotiation with the target has completed. Furthermore, we have
       to do it only once during the lifetime of the target detection */
    if (!target->ready) {
        HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                        "Waiting for the block size negotiation to finish\n");
        A_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(&htcEvent, (target->ready == TRUE), 
                                           HTC_TARGET_RESPONSE_TIMEOUT);

        if (target->ready) {
            status = A_OK;
        } else {
            status = A_ERROR;
            HTC_DEBUG_PRINTF(ATH_LOG_ERR, 
                                "Failed to negotiate the block sizes\n");
            HTCStop(target);
        }
    }

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCStart Exit\n");
    return status;
}

/* 
 * Provides an interface for the higher layer module to register for 
 * different events supported by the HTC module 
 */
A_STATUS 
HTCEventReg(HTC_TARGET *target, HTC_ENDPOINT_ID endPointId, 
            HTC_EVENT_ID eventId, HTC_EVENT_HANDLER eventHandler, 
            void *param) 
{
    /* 
     * Add the event handler against the specified event and store it in 
     * the event table 
     */
    A_STATUS status;
    HTC_ENDPOINT *endPoint;
    HTC_EVENT_INFO eventInfo;
    HTC_DATA_REQUEST_QUEUE *sendQueue, *recvQueue;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, 
                    "HTCEventReg: Enter (eventId: 0x%x, endPointId: %d)\n", 
                    eventId, endPointId);

    if (eventHandler) {
        if ((status = addToEventTable(target, endPointId, eventId, 
                                      eventHandler, param)) != A_OK)
        {
            HTC_DEBUG_PRINTF(ATH_LOG_ERR, 
                            "Could not add the event 0x%x to the event table\n", eventId);
            return status;
        }
    }

    switch(eventId) {
        case HTC_TARGET_AVAILABLE:
            if (eventHandler != NULL) {
                /* 
                 * Dispatch a Target Available event for all the targets 
                 * present. Iterate through the global list of targets but 
                 * currently we shall simply look for the first instance 
                 */
                target = AtherosTargetList[0];
                if (target != NULL) {
                    FRAME_EVENT(eventInfo, (A_UCHAR *)target->device, 
                                sizeof(HIF_DEVICE *), sizeof(HIF_DEVICE *), 
                                A_OK, NULL); 
                    dispatchEvent(target, ENDPOINT_UNUSED, eventId, &eventInfo);
                }
            } else {
                /* Initiate a shut down procedure */
            }
            break;

        case HTC_TARGET_UNAVAILABLE:
            break;

        case HTC_BUFFER_RECEIVED:
            if (eventHandler == NULL) {
                /* Flush the queue before unregistering the event handler */
                endPoint = &target->endPoint[endPointId];
                recvQueue = &endPoint->recvQueue;
                flushMboxQueue(endPoint, recvQueue, HTC_BUFFER_RECEIVED);
            }
            break;

        case HTC_SKB_RECEIVED:
            HTC_DEBUG_PRINTF(ATH_LOG_ERR, "skb not handled currently\n");
            break;

        case HTC_BUFFER_SENT:
            if (eventHandler == NULL) {
                /* Flush the queue before unregistering the event handler */
                endPoint = &target->endPoint[endPointId];
                sendQueue = &endPoint->sendQueue;
                flushMboxQueue(endPoint, sendQueue, HTC_BUFFER_SENT);
            }
            break;

        case HTC_SKB_SENT:
            HTC_DEBUG_PRINTF(ATH_LOG_ERR, "skb not handled currently\n");
            break;

        case HTC_DATA_AVAILABLE:
            /* 
             * Dispatch a data available event with the length. We are 
             * not handling this specific case currently because registering 
             * for HTC_DATA_AVAILABLE event is a part of the discipline 
             * that is imposed before one starts using HTC 
             */
            break;

        default:
            HTC_DEBUG_PRINTF(ATH_LOG_ERR, 
                            "Unknown Event ID: 0x%x\n", eventId);
            return A_EINVAL;
    }

    /* Check if its a call for registering the event or unregistering it */
    if (eventHandler == NULL) {
        if ((status = removeFromEventTable(target, endPointId, 
                                           eventId)) != A_OK)
        {
            HTC_DEBUG_PRINTF(ATH_LOG_ERR, 
                            "Could not remove the event 0x%x from the event table\n", eventId);
            return status;
        }
    }

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCEventReg: Exit\n");
    return A_OK;
}
 
/*
 * Commit an address to either WINDOW_WRITE_ADDR_REG or to
 * WINDOW_READ_ADDR_REG.  We write the least significan byte (LSB)
 * last, since it triggers the read/write.
 */
static void
_WRITE_WINDOW_ADDR(HTC_TARGET *target, A_UINT32 whichreg, A_UINT32 value)
{
    A_UINT32 window_addr;
    HIF_REQUEST request;
    A_STATUS status;
    A_UINT32 address;

    window_addr = value;
    HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_SYNCHRONOUS,
                      HIF_BYTE_BASIS, HIF_INCREMENTAL_ADDRESS);

    address = getRegAddr(whichreg, ENDPOINT_UNUSED);
#ifdef ONLY_16BIT
    status = HIFReadWrite(target->device, address+2, 
                          (A_UCHAR *)&window_addr+2, 2, &request, NULL);
    AR_DEBUG_ASSERT(status == A_OK);

    status = HIFReadWrite(target->device, address, 
                          (A_UCHAR *)&window_addr, 2, &request, NULL);
    status = HIFReadWrite(target->device, address, 
                          (A_UCHAR *)&window_addr, 2, &request, NULL);
    AR_DEBUG_ASSERT(status == A_OK);
#else
    status = HIFReadWrite(target->device, address+1, 
                          (A_UCHAR *)&window_addr+1, 3, &request, NULL);
    AR_DEBUG_ASSERT(status == A_OK);

    status = HIFReadWrite(target->device, address, 
                          (A_UCHAR *)&window_addr, 1, &request, NULL);
    AR_DEBUG_ASSERT(status == A_OK);
#endif
}

void
HTCStop(HTC_TARGET *target)
{
    A_UINT32 count;
    A_STATUS status;
    A_UINT32 address;
    HIF_REQUEST request;
    A_UINT32 window_data;
    HTC_ENDPOINT *endPoint;
    HTC_REG_REQUEST_LIST *regList;
    HTC_REG_REQUEST_ELEMENT *element;
    HTC_DATA_REQUEST_QUEUE *sendQueue;
    HTC_DATA_REQUEST_QUEUE *recvQueue;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCStop: Enter");

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

    /* Disable the host controller interrupts */
    HIFMaskInterrupt(target->device);

    /* Flush all the queues and return the buffers to their owner */
    for (count = ENDPOINT1; count <= ENDPOINT4; count ++) {
        endPoint = &target->endPoint[count];

        /* Decrement the number of credits consumed */
        if (endPoint->txCreditsConsumed) {
            HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, 
                              HIF_SYNCHRONOUS, HIF_BYTE_BASIS, 
                              HIF_FIXED_ADDRESS);
            address = getRegAddr(TX_CREDIT_COUNTER_DECREMENT_REG, count);
#ifdef ONLY_16BIT
            status = HIFReadWrite(target->device, address, 
                                  (A_UCHAR *)endPoint->txCreditsAvailable, 
                                  endPoint->txCreditsConsumed * 2, &request, NULL);
#else
            status = HIFReadWrite(target->device, address, 
                                  endPoint->txCreditsAvailable, 
                                  endPoint->txCreditsConsumed, &request, NULL);
#endif
            AR_DEBUG_ASSERT(status == A_OK);
        }

        SET_TX_CREDITS_AVAILABLE(endPoint, 0);
        SET_TX_CREDITS_CONSUMED(endPoint, 0);

#ifdef DEBUG
        txcreditsavailable[count] = GET_TX_CREDITS_AVAILABLE(endPoint);
        txcreditsconsumed[count] = GET_TX_CREDITS_CONSUMED(endPoint);
#endif

        endPoint->txCreditsIntrEnable = FALSE;
        endPoint->rxLengthPending = 0;
        endPoint->enabled = FALSE;

        /* Flush the Pending Receive Queue */
        HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                        "Flushing the recv queue & returning the buffers\n");

        recvQueue = &endPoint->recvQueue;
        flushMboxQueue(endPoint, recvQueue, HTC_BUFFER_RECEIVED);

        /* Flush the Pending Send Queue */
        HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                        "Flushing the send queue & returning the buffers\n");
        sendQueue = &endPoint->sendQueue;
        flushMboxQueue(endPoint, sendQueue, HTC_BUFFER_SENT);
    }

    /* Clear the tx counters */
    memset(tx_attempt, 0, sizeof(tx_attempt));
    memset(tx_post, 0, sizeof(tx_post));
    memset(tx_complete, 0, sizeof(tx_complete));

    /* Attempting a force reset of the target */
    window_data = RESET_CONTROL_COLD_RST_MASK;
    HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, HIF_SYNCHRONOUS,
                      HIF_BYTE_BASIS, HIF_INCREMENTAL_ADDRESS);
    address = getRegAddr(WINDOW_DATA_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, (A_UCHAR *)&window_data, 
                          4, &request, NULL);
    AR_DEBUG_ASSERT(status == A_OK);
    
    _WRITE_WINDOW_ADDR(target, WINDOW_WRITE_ADDR_REG, RESET_CONTROL_ADDRESS);

    /* 
     * Read back the RESET CAUSE register to ensure that the cold reset 
     * went through.
     */
    A_MDELAY(2000); /* 2 Second delay to allow dragon to settle down */
    _WRITE_WINDOW_ADDR(target, WINDOW_READ_ADDR_REG, RESET_CAUSE_ADDRESS);
    window_data = 0;
    HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, HIF_SYNCHRONOUS,
                      HIF_BYTE_BASIS, HIF_INCREMENTAL_ADDRESS);
    address = getRegAddr(WINDOW_DATA_REG, ENDPOINT_UNUSED);
    status = HIFReadWrite(target->device, address, (A_UCHAR *)&window_data, 
                          4, &request, NULL);
    AR_DEBUG_ASSERT(status == A_OK);     
    HTC_DEBUG_PRINTF(ATH_LOG_INF, "window data: %d\n", window_data);
    window_data &= RESET_CAUSE_LAST_MASK;

    if (window_data != 2) {
        HTC_DEBUG_PRINTF(ATH_LOG_ERR, "Unable to cold reset the target\n");
    }

    /* 
     * Ensure that all the pending asynchronous register read/writes have 
     * been finished.
     */
    regList = &target->regList;
    for (count = 0; count < HTC_REG_REQUEST_LIST_SIZE; count ++) {
        element = &regList->element[count];
        AR_DEBUG_ASSERT(IS_ELEMENT_FREE(element));
    }

    /* Initialize the shadow copy of the target register table */
    A_MEMZERO(&target->table, sizeof(HTC_REGISTER_TABLE));
    target->ready = FALSE;

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCStop: Exit");
}


void
HTCShutDown(HTC_TARGET *target)
{
    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCShutDown: Enter\n");

    if (target != NULL) {
        HIFShutDownDevice(target->device);
        delTargetInstance(target);
        A_MEMZERO(target, sizeof(HTC_TARGET));
        A_FREE(target);
    } else {
        HIFShutDownDevice(NULL);
    }
    A_DELETE_WAITQUEUE_HEAD(&htcEvent);

    HTC_DEBUG_PRINTF(ATH_LOG_TRC, "HTCShutDown: Exit\n");
}
