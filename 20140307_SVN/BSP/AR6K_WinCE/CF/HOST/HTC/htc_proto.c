//------------------------------------------------------------------------------
// <copyright file="htc_proto.c" company="Atheros">
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
// This file implementes a htc layer to communicate with the prototype target
//
// Author(s): ="Atheros"
//==============================================================================


#include <athdefs.h>
#include <athtypes.h>
#include <osapi.h>
#include "htc.h"
#include "../../target/include/AR6000/AR6000_htc.h"

struct htc_target {
    void *target;
};

#define MYHTC_MAX_BUFFERS 32

typedef struct {
    HTC_EVENT_HANDLER event_handler[8];
    void *param[8];
    void *buffer[MYHTC_MAX_BUFFERS];
    void *buf_cookie[MYHTC_MAX_BUFFERS];
    int bufindex;
    HTC_initdone_fn_t target_init_func;
    HTC_recvdone_fn_t target_rx_func;
    HTC_senddone_fn_t target_sent_func;
    struct HTC_bufinfo_s  *target_bfinfos[AR6000_MBOX_COUNT][MYHTC_MAX_BUFFERS];
} HTC_T;

HTC_T myhtc;

A_STATUS
HTCInit(void)
{
    return A_OK;
}

A_STATUS
HTCEventReg(HTC_TARGET *target, HTC_ENDPOINT_ID endPointId,
            HTC_EVENT_ID eventId, HTC_EVENT_HANDLER eventHandler,
            void *param)
{
    /*
     * proto htc only supports one handler for an event for all endpoints.
     */
    myhtc.event_handler[eventId] = eventHandler;
    myhtc.param[eventId] = param;

    if (eventId == HTC_TARGET_AVAILABLE) {
        (*eventHandler)((HTC_TARGET *)&myhtc, ENDPOINT_UNUSED,
             HTC_TARGET_AVAILABLE, NULL, NULL);
    }

    return A_OK;
}

A_STATUS
HTCStart(HTC_TARGET *target)
{
    A_STATUS status = A_OK;

    if (myhtc.target_init_func != NULL) {
        (*myhtc.target_init_func)();
    } else {
        printk("HTCStart: no target init func!\n");
        status = A_ERROR;
    }

    return (status);
}

void
HTCStop(HTC_TARGET *target)
{
}

A_STATUS
HTCBufferSend(HTC_TARGET *target, HTC_ENDPOINT_ID endPointId,
              A_UCHAR *buffer, A_UINT32 length, void *cookie)
{
    int txEv = HTC_BUFFER_SENT;
    int i;
    HTC_EVENT_INFO evInfo;
    struct HTC_bufinfo_s *htcinfo;

    /*
     * find a target buffer
     */
    for (i=0; i < MYHTC_MAX_BUFFERS; i++) {
        htcinfo = myhtc.target_bfinfos[endPointId][i];
        if (htcinfo != NULL) {
            A_ASSERT(length <= htcinfo->actual_length);
            A_MEMCPY(htcinfo->buffer, buffer, length);
            htcinfo->actual_length = length;
            htcinfo->next = NULL;
            /*
             *  Call target receive routine
             */
            (*myhtc.target_rx_func)(endPointId, htcinfo);
            evInfo.status = A_OK;
            break;
        }
    }

    if (i >= MYHTC_MAX_BUFFERS) {
            printk("htc_proto: no buffer for target - dropping frame\n");
            evInfo.status = A_ERROR;
    }

    /*
     * Call event handler indicating buffer was sent.
     */
    evInfo.buffer       = buffer;
    evInfo.cookie       = cookie;
    evInfo.bufferLength = length;
    evInfo.actualLength = length;

    (*myhtc.event_handler[txEv])((HTC_TARGET *)&myhtc, endPointId,
             HTC_BUFFER_SENT, &evInfo, myhtc.param[txEv]);

    return A_OK;
}

A_STATUS
HTCBufferReceive(HTC_TARGET *target, HTC_ENDPOINT_ID endPointId,
                 A_UCHAR *buffer, A_UINT32 length, void *cookie)
{
    int i;

    for (i=0; i < MYHTC_MAX_BUFFERS; i++) {
        if (myhtc.buffer[i] == NULL) {
            myhtc.buffer[i] = buffer;
            myhtc.buf_cookie[i] = cookie;
            return A_OK;
        }
    }

    return A_NO_MEMORY;
}

void
HTCShutDown(HTC_TARGET *target)
{
    int j, rxEv = HTC_BUFFER_RECEIVED;
    HTC_EVENT_INFO evInfo;

    /*
     * free up queued rx buffers
     */
    for (j=0; j < MYHTC_MAX_BUFFERS; j++) {
        if (myhtc.buffer[j] != NULL) {
            evInfo.buffer = myhtc.buffer[j];
            evInfo.cookie = myhtc.buf_cookie[j];
            myhtc.buffer[j] = NULL;
            myhtc.buf_cookie[j] = NULL;

            evInfo.bufferLength = 1500; /* XXX not correct */
            evInfo.actualLength = 1500; /* XXX not correct */
            evInfo.status       = A_ECANCELED;

            (*myhtc.event_handler[rxEv])((HTC_TARGET *)&myhtc,
                 ENDPOINT1, HTC_BUFFER_RECEIVED, &evInfo,
                 myhtc.param[rxEv]);
        }
    }
}

/*
 * Target HTC pseudo routines
 */
void
HTC_init(HTC_initdone_fn_t tinit_func, HTC_recvdone_fn_t trx_func,
         HTC_senddone_fn_t tsent_func)
{
    myhtc.target_init_func = tinit_func;
    myhtc.target_rx_func = trx_func;
    myhtc.target_sent_func = tsent_func;
}
EXPORT_SYMBOL(HTC_init);

void
HTC_mbox_bufsz_set(int mbox, int bufsz)
{
}
EXPORT_SYMBOL(HTC_mbox_bufsz_set);

void
HTC_descriptor(struct AR6000_DMA_desc_s *descriptor)
{
}
EXPORT_SYMBOL(HTC_descriptor);

void
HTC_receive(int mbox, struct HTC_bufinfo_s *bufinfo)
{
    int i;

    for (i = 0; i < MYHTC_MAX_BUFFERS; i++) {
        if (myhtc.target_bfinfos[mbox][i] == NULL) {
            /*
             * found a slot
             */
            myhtc.target_bfinfos[mbox][i] = bufinfo;
            break;
        }
    }
}
EXPORT_SYMBOL(HTC_receive);

void
HTC_send(int mbox, struct HTC_bufinfo_s *bufinfo_orig)
{
    A_UINT8 *bufPtr = NULL, *bufCookie = NULL;
    A_UINT8 *buf;
    int i, rxEv = HTC_BUFFER_RECEIVED;
    HTC_EVENT_INFO evInfo;
    struct HTC_bufinfo_s *bufinfo;
    int totallen = HTC_HDR_SZ;

    /*
     * look for a host buffer to receive on
     */
    for (i=0; i < MYHTC_MAX_BUFFERS; i++) {
        if (myhtc.buffer[i] != NULL) {
            bufPtr = myhtc.buffer[i];
            bufCookie = myhtc.buf_cookie[i];
            myhtc.buffer[i] = NULL;
            myhtc.buf_cookie[i] = NULL;
            break;
        }
    }

    if (bufPtr == NULL) {
        printk("htc_proto: no buffer to receive in\n");
        return;
    }

    buf = bufPtr;

    for (bufinfo = bufinfo_orig; bufinfo != NULL; bufinfo = bufinfo->next) {
        A_MEMCPY(buf, bufinfo->buffer, bufinfo->actual_length);
        buf      += bufinfo->actual_length;
        totallen += bufinfo->actual_length;
        if (totallen > 1500) {
            printk("htc_proto: total len max exceeded\n");
            return;
        }
    }

    /*
     * Call the event handler indicating a new buffer has been
     * received
     */
    evInfo.buffer = bufPtr;
    evInfo.cookie = bufCookie;
    evInfo.bufferLength = totallen;   /* XXX not correct */
    evInfo.actualLength = totallen;
    evInfo.status       = A_OK;

    (*myhtc.event_handler[rxEv])((HTC_TARGET *)&myhtc, mbox,
             rxEv, &evInfo, myhtc.param[rxEv]);

    /*
     * Call target tx complete routine
     */
    (*myhtc.target_sent_func)(mbox, bufinfo_orig);
}

EXPORT_SYMBOL(HTC_send);

