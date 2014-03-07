//------------------------------------------------------------------------------
// <copyright file="htc_internal.h" company="Atheros">
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
// HTC internal specific declarations and prototypes
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef _HTC_INTERNAL_H_
#define _HTC_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Header files */
#include "athdefs.h"
#include "athtypes.h"
#include "AR6000/hw/mbox_host_reg.h"
#include "AR6000/hw/rtc_reg.h"
#include "htc.h"
#include "AR6000/AR6000_bmi.h"
#include "osapi.h"
#include "hif.h"

/* HTC operational parameters */
#define HTC_GLOBAL_EVENT_NUM_MAX           2 /* Target available/unavailable */
#define HTC_ENDPOINT_EVENT_NUM_MAX         5 /* Endpoint specific */
#define HTC_REG_REQUEST_LIST_SIZE          16
#define HTC_MESSAGE_SIZE_MAX               1536 - HTC_HEADER_LEN /* Default maximum message size for each mailbox */
#define HTC_TX_CREDITS_NUM_MAX             64
#define HTC_TARGET_RESPONSE_TIMEOUT        2000 /* in ms */

/* Useful macros */
#define GET_ENDPOINT_ID(endPoint) (endPoint - endPoint->target->endPoint)


/* ------- Event Related Data Structures ------- */
typedef struct htc_event_map HTC_EVENT_MAP;
typedef struct event_table_element EVENT_TABLE_ELEMENT;
typedef struct htc_endpoint_event_table HTC_ENDPOINT_EVENT_TABLE;
typedef struct htc_global_event_table HTC_GLOBAL_EVENT_TABLE;

#define FRAME_EVENT(_eventInfo, _buffer, _bufferLength,   \
                    _actualLength, _status, _cookie) do { \
    _eventInfo.buffer  = _buffer;                         \
    _eventInfo.bufferLength = _bufferLength;              \
    _eventInfo.actualLength = _actualLength;              \
    _eventInfo.status = _status;                          \
    _eventInfo.cookie = _cookie;                          \
} while (0)

struct event_table_element {
    HTC_EVENT_ID         id;
    HTC_EVENT_HANDLER    handler;
    void                *param;
};

struct htc_endpoint_event_table {
    EVENT_TABLE_ELEMENT element[HTC_ENDPOINT_EVENT_NUM_MAX];
};

struct htc_global_event_table {
    EVENT_TABLE_ELEMENT element[HTC_GLOBAL_EVENT_NUM_MAX];

};

/* ------ Mailbox Related Data Structures ------ */
typedef struct htc_queue_element HTC_QUEUE_ELEMENT, HTC_REG_REQUEST_ELEMENT, HTC_DATA_REQUEST_ELEMENT;
typedef struct htc_mbox_buffer HTC_MBOX_BUFFER;
typedef struct htc_reg_buffer HTC_REG_BUFFER;
typedef struct htc_data_request_queue HTC_DATA_REQUEST_QUEUE;
typedef struct htc_reg_request_list HTC_REG_REQUEST_LIST;
typedef struct htc_endpoint HTC_ENDPOINT;

typedef enum {
    INT_STATUS_REG,
    ERROR_INT_STATUS_REG,
    CPU_INT_STATUS_REG,
    RX_LOOKAHEAD_VALID_REG,
    RX_LOOKAHEAD0_REG,
    RX_LOOKAHEAD1_REG,
    RX_LOOKAHEAD2_REG,
    RX_LOOKAHEAD3_REG,
    TX_CREDIT_COUNTER_REG,
    TX_CREDIT_COUNTER_RESET_REG,
    TX_CREDIT_COUNTER_DECREMENT_REG,
    SCRATCH_REG,
    INT_STATUS_ENABLE_REG,
    CPU_INT_STATUS_ENABLE_REG,
    ERROR_STATUS_ENABLE_REG,
    COUNTER_INT_STATUS_READ_REG,
    COUNTER_INT_STATUS_ENABLE_REG,
    COUNTER_INT_STATUS_DISABLE_REG,
    INT_WLAN_REG,
    WINDOW_DATA_REG,
    WINDOW_WRITE_ADDR_REG,
    WINDOW_READ_ADDR_REG
} TARGET_REGISTERS;

#define SET_TX_CREDITS_AVAILABLE(endPoint, credits) \
                                    endPoint->txCreditsAvailable[0] = (credits)
#define SET_TX_CREDITS_CONSUMED(endPoint, credits) \
                                    endPoint->txCreditsConsumed = (credits)
#define GET_TX_CREDITS_AVAILABLE(endPoint) \
                                    endPoint->txCreditsAvailable[0]
#define GET_TX_CREDITS_CONSUMED(endPoint) \
                                    endPoint->txCreditsConsumed

#define IS_ELEMENT_FREE(element)	element->buffer.free
#define GET_MBOX_BUFFER(element)	&((element)->buffer.u.mboxBuffer)
#define GET_REG_BUFFER(element)		&((element)->buffer.u.regBuffer)
#define GET_QUEUE_TAIL(queue)		&queue->element[(queue->head + queue->size) % HTC_DATA_REQUEST_RING_BUFFER_SIZE]
#define GET_QUEUE_HEAD(queue)		&queue->element[queue->head]
#define IS_DATA_QUEUE_EMPTY(queue)      (!queue->size)
#define IS_DATA_QUEUE_FULL(queue)       (!(HTC_DATA_REQUEST_RING_BUFFER_SIZE - queue->size))

#define RECYCLE_DATA_REQUEST_ELEMENT(element) do { \
    FILL_MBOX_BUFFER(element, NULL, 0, 0, NULL); \
    (element)->buffer.free = TRUE; \
} while (0)

#define FILL_MBOX_BUFFER(element, _buffer, _bufferLength, _actualLength, _cookie) do { \
    (GET_MBOX_BUFFER(element))->buffer = _buffer; \
    (GET_MBOX_BUFFER(element))->bufferLength = _bufferLength; \
    (GET_MBOX_BUFFER(element))->actualLength = _actualLength; \
    (GET_MBOX_BUFFER(element))->cookie = _cookie; \
} while (0)

#define FILL_REG_BUFFER(element, _buffer, _length, _base, _offset) do { \
    (GET_REG_BUFFER(element))->buffer = _buffer; \
    (GET_REG_BUFFER(element))->length = _length; \
    (GET_REG_BUFFER(element))->base = _base; \
    (GET_REG_BUFFER(element))->offset = _offset; \
} while (0)

struct htc_queue_element {
    A_STATUS	(*completionCB)(HTC_QUEUE_ELEMENT *element, A_STATUS status);
    struct htc_buffer {
        /* In use or available */
        A_BOOL	free;
        union {
            struct htc_mbox_buffer {
                /* 
                 * Given by the caller and is associated with the buffer being 
                 * queued up.
                 */
                void			*cookie;

                /* 
                 * Pointer to the start of the buffer. In the transmit 
                 * direction this points to the start of the payload. In the 
                 * receive direction, however, the buffer when queued up 
                 * points to the start of the HTC header but when returned 
                 * to the caller points to the start of the payload 
                 */
                A_UCHAR			*buffer;

                /* Holds the length of the buffer */
                A_UINT32		bufferLength;

                /* Holds the length of the payload */
                A_UINT32		actualLength;

                HTC_ENDPOINT    *endPoint;
            } mboxBuffer;
            struct htc_reg_buffer {
                HTC_TARGET		*target;
#ifdef ONLY_16BIT
                A_UINT16        *buffer;
#else
                A_UCHAR			*buffer;
#endif
                A_UINT32		length;
                TARGET_REGISTERS	base;
                A_UINT32		offset;
            } regBuffer;
        } u;
    } buffer;
};

/* This is a FIFO queue of the pending data read/write requests. When a request
has to be issued, the element at the head of the queue is dequeued and
processed. New requests are added at the tail of the queue. The queue can only
support a fixed number of requests and stops adding new requests once the total
number of requests that are pending to be processed and the ones that are still
under process reach the queue capacity */
struct htc_data_request_queue {
    A_UINT32                  head;
    A_UINT32                  size;
    HTC_DATA_REQUEST_ELEMENT  element[HTC_DATA_REQUEST_RING_BUFFER_SIZE];
};

/* This is a list of 'free' register read/write requests. When a request has to 
be issued an element is taken from this list and after the completion of the 
request is added back to the list */
struct htc_reg_request_list {
    HTC_REG_REQUEST_ELEMENT  element[HTC_REG_REQUEST_LIST_SIZE];
};

struct htc_endpoint {
    /* Enabled or Disabled */
    A_BOOL                   enabled;

    /*
     * Used to hold the length of the frame received from the target in 
     * case there are no buffers that have been queued up to receive the 
     * data.
     */
    A_UINT32                 rxLengthPending; 

#ifdef ONLY_16BIT
    /* Number of frames for which the target has space for at any time */
    A_UINT16                  txCreditsAvailable[1 + HTC_TX_CREDITS_NUM_MAX];

    /* 
     * Number of frames that have been sent since the transmit credits 
     * were last updated.
     */
    A_UINT16                  txCreditsConsumed; 
#else
    /* Number of frames for which the target has space for at any time */
    A_UINT8                  txCreditsAvailable[1 + HTC_TX_CREDITS_NUM_MAX];

    /* 
     * Number of frames that have been sent since the transmit credits 
     * were last updated.
     */
    A_UINT8                  txCreditsConsumed; 
#endif

    A_BOOL                   txCreditsIntrEnable;

    /* Pending Send Queue */
    HTC_DATA_REQUEST_QUEUE   sendQueue; 

    /* Pending Receive Queue */
    HTC_DATA_REQUEST_QUEUE   recvQueue; 

    /* Inverse reference to the target */
    HTC_TARGET              *target;

    /* Block size configured for the endpoint */
    A_UINT32                 blockSize;

    /* Event Table */
    HTC_ENDPOINT_EVENT_TABLE eventTable; 
};

/* ------- Target Related Data structures ------- */
typedef struct htc_register_table HTC_REGISTER_TABLE;

/* 
 * The following Register table only contain those registers that are used 
 * in HTC. It does not reflect the actual register layout in the hardware 
 */
struct htc_register_table {
    A_UINT8                      host_int_status;
    A_UINT8                      cpu_int_status;
    A_UINT8                      error_int_status;
    A_UINT8                      counter_int_status;
    A_UINT8                      mbox_frame;
    A_UINT8                      rx_lookahead_valid;
    A_UINT8                      hole[2];
    A_UINT32                     rx_lookahead[HTC_MAILBOX_NUM_MAX];
    A_UINT8                      int_status_enable;
    A_UINT8                      cpu_int_status_enable;
    A_UINT8                      error_status_enable;
    A_UINT8                      counter_int_status_enable;
#ifdef ONLY_16BIT
    A_UINT16                     int_wlan;
#else
    A_UINT8                      int_wlan;
#endif
};

struct htc_target {
    A_BOOL                       ready;
    void                        *device; /* Handle to the device instance 
                                            reported by the bus driver */
    HTC_ENDPOINT                 endPoint[HTC_MAILBOX_NUM_MAX];
    HTC_REGISTER_TABLE           table;
    HTC_REG_REQUEST_LIST         regList;
};


/* ------- Function Prototypes for Receive -------- */
void
htcReceiveFrame(HTC_ENDPOINT *endPoint);

A_UINT32
htcGetFrameLength(HTC_ENDPOINT *endPoint);


/* ------- Function Prototypes for Transmit -------- */
void 
htcSendFrame(HTC_ENDPOINT *endPoint);

void
htcSendBlkSize(HTC_ENDPOINT *endPoint);


/* ------- Function Prototypes for Events and Callbacks  ------- */
A_STATUS
htcRWCompletionHandler(void *element, 
                       A_STATUS status);

A_STATUS 
htcTxCompletionCB(HTC_DATA_REQUEST_ELEMENT *element,
                  A_STATUS status);

A_STATUS
htcBlkSzNegCompletionCB(HTC_DATA_REQUEST_ELEMENT *element,
                        A_STATUS status);

A_STATUS
htcRxCompletionCB(HTC_DATA_REQUEST_ELEMENT *element, 
                  A_STATUS status);

A_STATUS
htcRegCompletionCB(HTC_REG_REQUEST_ELEMENT *element,
                   A_STATUS status);

A_STATUS
htcTargetInsertedHandler(HIF_DEVICE *device);

A_STATUS
htcTargetRemovedHandler(HIF_DEVICE *device);

A_STATUS
htcDSRHandler(HIF_DEVICE *device);

A_STATUS
htcInterruptDisabler(HIF_DEVICE *device,A_BOOL *callDsr);

A_STATUS
htcInterruptPending(HIF_DEVICE *device, A_BOOL *intPending);

A_STATUS
htcInterruptEnabler(HIF_DEVICE *device);
#ifdef HTC_SYNC
A_STATUS
htcISRHandler(HIF_DEVICE *device);
#endif

void
htcServiceCPUInterrupt(HTC_TARGET *target);

void
htcServiceErrorInterrupt(HTC_TARGET *target);

void
htcServiceCounterInterrupt(HTC_TARGET *target);

void
htcServiceMailboxInterrupt(HTC_TARGET *target);

void
htcEnableCreditCounterInterrupt(HTC_TARGET *target, 
                                HTC_ENDPOINT_ID endPointId);

void
htcDisableCreditCounterInterrupt(HTC_TARGET *target, 
                                 HTC_ENDPOINT_ID endPointId);

/* ------- Function Prototypes for Utility routines ------- */
A_STATUS
addToMboxQueue(HTC_DATA_REQUEST_QUEUE *queue,
               A_UCHAR        *buffer,
               A_UINT32        bufferLength,
               A_UINT32        actualLength,
               void           *cookie);

HTC_DATA_REQUEST_ELEMENT *
removeFromMboxQueue(HTC_DATA_REQUEST_QUEUE *queue);

void
flushMboxQueue(HTC_ENDPOINT *endPoint,
               HTC_DATA_REQUEST_QUEUE *queue, 
               HTC_EVENT_ID eventId);

HTC_REG_REQUEST_ELEMENT *
allocateRegRequestElement(HTC_TARGET *target);

void
freeRegRequestElement(HTC_REG_REQUEST_ELEMENT *element);

A_STATUS 
addToEventTable(HTC_TARGET       *target,
                HTC_ENDPOINT_ID   endPointId,
                HTC_EVENT_ID      eventId,
                HTC_EVENT_HANDLER handler, 
                void             *param);

A_STATUS 
removeFromEventTable(HTC_TARGET *target,
                     HTC_ENDPOINT_ID endPointId,
                     HTC_EVENT_ID  eventId);

void
dispatchEvent(HTC_TARGET     *target, 
              HTC_ENDPOINT_ID endPointId,
              HTC_EVENT_ID    eventId, 
              HTC_EVENT_INFO *eventInfo);

HTC_TARGET *
getTargetInstance(void *device);

void
addTargetInstance(HTC_TARGET *target);

void
delTargetInstance(HTC_TARGET *target);

A_UINT32 
getRegAddr(TARGET_REGISTERS base, 
           HTC_ENDPOINT_ID endPointId);

A_UINT8
htcGetBitNumSet(A_UINT32 data);

void
dumpBytes(A_UCHAR *buffer, A_UINT16 length);

void
dumpRegisters(HTC_TARGET *target);

#ifdef __cplusplus
}
#endif

#endif /* _HTC_INTERNAL_H_ */
