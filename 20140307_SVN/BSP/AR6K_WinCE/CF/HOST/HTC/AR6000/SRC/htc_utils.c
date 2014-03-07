//------------------------------------------------------------------------------
// <copyright file="htc_utils.c" company="Atheros">
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
// This file contains the utility routines used across the entire HTC module.
//
// Author(s): ="Atheros"
//==============================================================================


#include "htc_internal.h"

/* ------ Global Variable Declarations ------- */
extern HTC_TARGET *AtherosTargetList[HIF_MAX_DEVICES];
extern HTC_GLOBAL_EVENT_TABLE AtherosEventTable;
extern A_MUTEX_T instanceCS;

#ifdef DEBUG
extern A_UINT32 debughtc;
#endif

/* ------ Static Variables ------ */

/* ------ Functions ------ */
void
dispatchEvent(HTC_TARGET     *target, 
              HTC_ENDPOINT_ID endPointId,
              HTC_EVENT_ID    eventId, 
              HTC_EVENT_INFO *eventInfo)
{
    EVENT_TABLE_ELEMENT *eventElement;

    if (eventId == HTC_TARGET_AVAILABLE) {
        eventElement = &AtherosEventTable.element[0];
    } else if (eventId == HTC_TARGET_UNAVAILABLE) {
        eventElement = &AtherosEventTable.element[1];
    } else {
        eventElement = 
            &target->endPoint[endPointId].eventTable.element[eventId]; 
    }
    AR_DEBUG_ASSERT(eventElement != NULL);

    HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                    "dispatchEvent(endpoint: %d, eventId: 0x%d, handler: 0x%p)\n", endPointId, eventElement->id, eventElement->handler);
    if (eventElement->handler) {
        eventElement->handler(target, endPointId, eventId, eventInfo,
                              eventElement->param);
    }
}


A_STATUS 
addToEventTable(HTC_TARGET       *target,
                HTC_ENDPOINT_ID   endPointId,
                HTC_EVENT_ID      eventId,
                HTC_EVENT_HANDLER handler, 
                void             *param)
{
    EVENT_TABLE_ELEMENT *new;

    if (eventId == HTC_TARGET_AVAILABLE) {
        new = &AtherosEventTable.element[0];
    } else if (eventId == HTC_TARGET_UNAVAILABLE) {
        new = &AtherosEventTable.element[1];
    } else {
        new = &target->endPoint[endPointId].eventTable.element[eventId]; 
    }

    /* Store the event id, the corresponding handler and the param passed */
    new->id = eventId;
    new->handler = handler;
    new->param = param;

    HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                    "addToEventTable(endpoint: %d, eventId: 0x%d, handler: 0x%p)\n", endPointId, new->id, new->handler);

    return A_OK;
}


A_STATUS 
removeFromEventTable(HTC_TARGET *target,
                     HTC_ENDPOINT_ID endPointId,
                     HTC_EVENT_ID  eventId)
{
    EVENT_TABLE_ELEMENT *remove;

    if (eventId == HTC_TARGET_AVAILABLE) {
        remove = &AtherosEventTable.element[0];
    } else if (eventId == HTC_TARGET_UNAVAILABLE) {
        remove = &AtherosEventTable.element[1];
    } else {
        remove = &target->endPoint[endPointId].eventTable.element[eventId]; 
    }

    /* Remove the event handler */
    A_MEMZERO(remove, sizeof(EVENT_TABLE_ELEMENT));

    return A_OK;
}

A_STATUS
addToMboxQueue(HTC_DATA_REQUEST_QUEUE *queue,
               A_UCHAR        *buffer,
               A_UINT32        bufferLength,
               A_UINT32        actualLength,
               void           *cookie)
{
    A_STATUS status;
    HTC_DATA_REQUEST_ELEMENT *element;

    AR_DEBUG_ASSERT(queue != NULL);
    AR_DEBUG_ASSERT(bufferLength);

    HTC_DEBUG_PRINTF(ATH_LOG_SYNC, 
                    "Critical Section (queue): LOCK at line %d in file %s\n", __LINE__, __FILE__);
    A_MUTEX_LOCK(&instanceCS);
    element = GET_QUEUE_TAIL(queue);
    if (!(IS_DATA_QUEUE_FULL(queue)) && IS_ELEMENT_FREE(element)) {
        element->buffer.free = FALSE;
        FILL_MBOX_BUFFER(element, buffer, bufferLength, actualLength, cookie);
        queue->size += 1;

        HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                        "addToMboxQueue (index: %d, size: %d, bufferElement: 0x%p, bufferElement->buffer: 0x%p, bufferElement->cookie: 0x%p)\n", (queue->head + queue->size - 1) % HTC_DATA_REQUEST_RING_BUFFER_SIZE, queue->size, element, (GET_MBOX_BUFFER(element))->buffer, (GET_MBOX_BUFFER(element))->cookie);
        status = A_OK;
    } else {
        HTC_DEBUG_PRINTF(ATH_LOG_ERR, "Queue size: %d\n", queue->size);
        status = A_ERROR;
    }
    HTC_DEBUG_PRINTF(ATH_LOG_SYNC, 
                    "Critical Section (queue): UNLOCK at line %d in file %s\n", __LINE__, __FILE__);
    A_MUTEX_UNLOCK(&instanceCS);

    return status;
}

HTC_DATA_REQUEST_ELEMENT *
removeFromMboxQueue(HTC_DATA_REQUEST_QUEUE *queue) {
    HTC_DATA_REQUEST_ELEMENT *element;
    AR_DEBUG_ASSERT(queue != NULL);

    HTC_DEBUG_PRINTF(ATH_LOG_SYNC, 
                    "Critical Section (queue): LOCK at line %d in file %s\n", __LINE__, __FILE__);
    A_MUTEX_LOCK(&instanceCS);
    if (!(IS_DATA_QUEUE_EMPTY(queue))) {
        element = GET_QUEUE_HEAD(queue);
        queue->head = (queue->head + 1) % HTC_DATA_REQUEST_RING_BUFFER_SIZE;
        queue->size -= 1;

        HTC_DEBUG_PRINTF(ATH_LOG_INF, 
                        "removeFromMboxQueue (index: %d, size: %d, bufferElement: 0x%p, bufferElement->buffer: 0x%p, bufferElement->cookie: 0x%p)\n", queue->head, queue->size, element, (GET_MBOX_BUFFER(element))->buffer, (GET_MBOX_BUFFER(element))->cookie);
    } else {
        element = NULL;
    }
    HTC_DEBUG_PRINTF(ATH_LOG_SYNC, 
                    "Critical Section (queue): UNLOCK at line %d in file %s\n", __LINE__, __FILE__);
    A_MUTEX_UNLOCK(&instanceCS);

    return element;
}

void
flushMboxQueue(HTC_ENDPOINT *endPoint,
               HTC_DATA_REQUEST_QUEUE *queue, 
               HTC_EVENT_ID eventId)
{
    HTC_DATA_REQUEST_ELEMENT *curr;
    HTC_EVENT_INFO eventInfo;
    HTC_ENDPOINT_EVENT_TABLE *eventTable;
    HTC_ENDPOINT_ID endPointId;
    EVENT_TABLE_ELEMENT *eventElement;
    HTC_MBOX_BUFFER *mboxBuffer;

    eventTable = &endPoint->eventTable;
    endPointId = GET_ENDPOINT_ID(endPoint);

    /* 
     * Release the buffer to WMI using the registered event handler. If WMI 
     * has not registered any callbacks for a particular endpoint then it 
     * means that its queues will not have any buffers so we skip that 
     * endpoint.
     */
    if ((eventElement = &eventTable->element[eventId]) != NULL) {
        while ((curr = removeFromMboxQueue(queue)) != NULL) {
            /* Frame the event and dispatch it */
            mboxBuffer = GET_MBOX_BUFFER(curr);
            FRAME_EVENT(eventInfo, mboxBuffer->buffer, 
                        mboxBuffer->bufferLength, mboxBuffer->actualLength, 
                        A_ECANCELED, mboxBuffer->cookie);
            if (eventElement->handler) {
                eventElement->handler(endPoint->target, endPointId, eventId, 
                                      &eventInfo, eventElement->param);
            }
            RECYCLE_DATA_REQUEST_ELEMENT(curr);
        }
    }

    /* Initialize the head and tail pointer */
    queue->head = 0;
    queue->size = 0;
}

HTC_REG_REQUEST_ELEMENT *
allocateRegRequestElement(HTC_TARGET *target) {
    A_UINT32 count;
    HTC_REG_REQUEST_ELEMENT *element;

    A_MUTEX_LOCK(&instanceCS);
    element = NULL;
    for (count = 0; count < HTC_REG_REQUEST_LIST_SIZE; count ++) {
        element = &target->regList.element[count];
        if (IS_ELEMENT_FREE(element)) {
            element->buffer.free = FALSE;
            break;
        }
    }
    A_MUTEX_UNLOCK(&instanceCS);

    return element;
}

void
freeRegRequestElement(HTC_REG_REQUEST_ELEMENT *element) {
    A_MUTEX_LOCK(&instanceCS);
    FILL_REG_BUFFER(element, NULL, 0, 0, 0);
    element->buffer.free = TRUE;
    A_MUTEX_UNLOCK(&instanceCS);
}

HTC_TARGET *
getTargetInstance(void *device)
{
    return AtherosTargetList[0];
}

void
addTargetInstance(HTC_TARGET *target)
{
    AtherosTargetList[0] = target;
}

void
delTargetInstance(HTC_TARGET *target)
{
    AtherosTargetList[0] = NULL;
}

A_UINT32 
getRegAddr(TARGET_REGISTERS base, 
           HTC_ENDPOINT_ID endPointId) 
{
    A_UINT32 address;

    switch(base) {
    case TX_CREDIT_COUNTER_RESET_REG:
        address = COUNT_DEC_ADDRESS + endPointId * 4;
        break;

    case TX_CREDIT_COUNTER_DECREMENT_REG:
        address = COUNT_DEC_ADDRESS + (HTC_MAILBOX_NUM_MAX + endPointId) * 4;
        break;

    case TX_CREDIT_COUNTER_REG:
        address = COUNT_ADDRESS + (HTC_MAILBOX_NUM_MAX + endPointId) * 4;
        break;

    case INT_STATUS_ENABLE_REG:
        address = INT_STATUS_ENABLE_ADDRESS;
        break;

    case ERROR_STATUS_ENABLE_REG:
		address = ERROR_STATUS_ENABLE_ADDRESS;
		break;

    case COUNTER_INT_STATUS_ENABLE_REG:
    case COUNTER_INT_STATUS_DISABLE_REG:
        address = COUNTER_INT_STATUS_ENABLE_ADDRESS;
        break;

    case INT_STATUS_REG:
        address = HOST_INT_STATUS_ADDRESS;
        break;

    case CPU_INT_STATUS_REG:
        address = CPU_INT_STATUS_ADDRESS;
        break;

    case ERROR_INT_STATUS_REG:
        address = ERROR_INT_STATUS_ADDRESS;
        break;

    case INT_WLAN_REG:
        address = INT_WLAN_ADDRESS;
        break;

    case WINDOW_DATA_REG:
        address = WINDOW_DATA_ADDRESS;
        break;

    case WINDOW_WRITE_ADDR_REG:
        address = WINDOW_WRITE_ADDR_ADDRESS;
        break;

    case WINDOW_READ_ADDR_REG:
        address = WINDOW_READ_ADDR_ADDRESS;
        break;

    default:
        HTC_DEBUG_PRINTF(ATH_LOG_ERR, "Invalid register: %d\n", base);
        AR_DEBUG_ASSERT(0);
        address = 0;
        break;
    }

    return address;
}

void
dumpBytes(A_UCHAR *buffer, A_UINT16 length)
{
    A_CHAR stream[60];
    A_UINT32 i;
    A_UINT16 offset, count;

    HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Dumping %d Bytes : ------>\n", length);

    count = 0;
    offset = 0;
    for(i = 0; i < length; i++) {
        sprintf(stream + offset, "%2x ", buffer[i]);
	count ++;
	offset += 3;

	if(count == 16) {
	    count = 0;
	    offset = 0;
	    HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "[H]: %s\n", stream);
	    A_MEMZERO(stream, 60);
	}
    }

    if(offset != 0) {
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "[H]: %s\n", stream);
    }
}

void
dumpRegisters(HTC_TARGET *target)
{
    HTC_REGISTER_TABLE *reg;

    reg = &target->table;
    HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "\n<------- Register Table -------->\n");
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Int Status:                0x%x\n", reg->host_int_status);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "CPU Int Status:            0x%x\n", reg->cpu_int_status);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Error Int Status:          0x%x\n", reg->error_int_status);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Counter Int Status:        0x%x\n", reg->counter_int_status);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Mbox Frame:                0x%x\n", reg->mbox_frame);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Rx Lookahead Valid:        0x%x\n", reg->rx_lookahead_valid);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Rx Lookahead 0:            0x%x\n", reg->rx_lookahead[ENDPOINT1]);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Rx Lookahead 1:            0x%x\n", reg->rx_lookahead[ENDPOINT2]);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Rx Lookahead 2:            0x%x\n", reg->rx_lookahead[ENDPOINT3]);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Rx Lookahead 3:            0x%x\n", reg->rx_lookahead[ENDPOINT4]);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Int Status Enable:         0x%x\n", reg->int_status_enable);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "Counter Int Status Enable: 0x%x\n", reg->counter_int_status_enable);
	HTC_DEBUG_PRINTF(ATH_LOG_DUMP, "<------------------------------->\n");
}

A_UINT8
htcGetBitNumSet(A_UINT32 data)
{
    A_UINT8 count;

    count = 0;
    while(!(data % 2)) {
        count += 1;
        data >>= 1;
    }

    return count;
}

