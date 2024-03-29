/*------------------------------------------------------------------------------ */
/* <copyright file="ar6k.h" company="Atheros"> */
/*    Copyright (c) 2007-2008 Atheros Corporation.  All rights reserved. */
/*  */
/* This program is free software; you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License version 2 as */
/* published by the Free Software Foundation; */
/* */
/* Software distributed under the License is distributed on an "AS */
/* IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or */
/* implied. See the License for the specific language governing */
/* rights and limitations under the License. */
/* */
/* */
/*------------------------------------------------------------------------------ */
/*============================================================================== */
/* AR6K device layer that handles register level I/O */
/* */
/* Author(s): ="Atheros" */
/*============================================================================== */


#ifndef AR6K_H_
#define AR6K_H_

#include "dbglog_api.h"

#define AR6K_MAILBOXES 4

/* HTC runs over mailbox 0 */
#define HTC_MAILBOX          0

#define AR6K_TARGET_DEBUG_INTR_MASK     0x01

#if 0
#define OTHER_INTS_ENABLED (INT_STATUS_ENABLE_ERROR_MASK |   \
                            INT_STATUS_ENABLE_CPU_MASK   |   \
                            INT_STATUS_ENABLE_COUNTER_MASK)
#else
#define OTHER_INTS_ENABLED (INT_STATUS_ENABLE_ERROR_MASK | INT_STATUS_ENABLE_CPU_MASK | INT_STATUS_ENABLE_COUNTER_MASK )

#endif

//#define MBOXHW_UNIT_TEST 1 

#include "athstartpack.h"
#pragma pack(push,1)
typedef struct _AR6K_IRQ_PROC_REGISTERS {
    A_UINT8                      host_int_status;
    A_UINT8                      cpu_int_status;
    A_UINT8                      error_int_status;
    A_UINT8                      counter_int_status;
    A_UINT8                      mbox_frame;
    A_UINT8                      rx_lookahead_valid;
    A_UINT8                      hole[2];
    A_UINT32                     rx_lookahead[2];
} AR6K_IRQ_PROC_REGISTERS;
#pragma pack(pop)


#define AR6K_IRQ_PROC_REGS_SIZE sizeof(AR6K_IRQ_PROC_REGISTERS)


#pragma pack(push,1)
typedef struct _AR6K_IRQ_ENABLE_REGISTERS {
    A_UINT8                      int_status_enable;
    A_UINT8                      cpu_int_status_enable;
    A_UINT8                      error_status_enable;
    A_UINT8                      counter_int_status_enable;
} AR6K_IRQ_ENABLE_REGISTERS;
#pragma pack(pop)


#include "athendpack.h"

#define AR6K_IRQ_ENABLE_REGS_SIZE sizeof(AR6K_IRQ_ENABLE_REGISTERS)

#define AR6K_REG_IO_BUFFER_SIZE     32
#define AR6K_MAX_REG_IO_BUFFERS     8

/* buffers for ASYNC I/O */
typedef struct _AR6K_ASYNC_REG_IO_BUFFER {
    HTC_PACKET    HtcPacket;   /* we use an HTC packet as a wrapper for our async register-based I/O */
    A_UINT8       Buffer[AR6K_REG_IO_BUFFER_SIZE];
} AR6K_ASYNC_REG_IO_BUFFER;

typedef enum _AR6K_TARGET_FAILURE_TYPE {
    AR6K_TARGET_ASSERT = 1,
    AR6K_TARGET_RX_ERROR,
    AR6K_TARGET_TX_ERROR   
} AR6K_TARGET_FAILURE_TYPE;

typedef struct _AR6K_DEVICE {
    A_MUTEX_T                   Lock;
    AR6K_IRQ_PROC_REGISTERS     IrqProcRegisters;
    AR6K_IRQ_ENABLE_REGISTERS   IrqEnableRegisters;
    void                        *HIFDevice;
    A_UINT32                    BlockSize;
    A_UINT32                    BlockMask;
    A_UINT32                    MailboxAddress;
    HIF_PENDING_EVENTS_FUNC     GetPendingEventsFunc;
    void                        *HTCContext;
    HTC_PACKET_QUEUE            RegisterIOList;
    AR6K_ASYNC_REG_IO_BUFFER    RegIOBuffers[AR6K_MAX_REG_IO_BUFFERS];
    void                        (*TargetFailureCallback)(void *Context, AR6K_TARGET_FAILURE_TYPE Type);
    A_STATUS                    (*MessagePendingCallback)(void *Context, A_UINT32 *LookAhead, A_BOOL *pAsyncProc);
    HIF_DEVICE_IRQ_PROCESSING_MODE  HifIRQProcessingMode;
    HIF_MASK_UNMASK_RECV_EVENT      HifMaskUmaskRecvEvent;
    A_BOOL                          HifAttached;
} AR6K_DEVICE;

#define IS_DEV_IRQ_PROCESSING_ASYNC_ALLOWED(pDev) ((pDev)->HifIRQProcessingMode != HIF_DEVICE_IRQ_SYNC_ONLY)

A_STATUS DevSetup(AR6K_DEVICE *pDev);
void     DevCleanup(AR6K_DEVICE *pDev);
A_STATUS DevUnmaskInterrupts(AR6K_DEVICE *pDev);
A_STATUS DevMaskInterrupts(AR6K_DEVICE *pDev);
A_STATUS DevPollMboxMsgRecv(AR6K_DEVICE *pDev,
                            A_UINT32    *pLookAhead,
                            int          TimeoutMS);
A_STATUS DevRWCompletionHandler(void *context, A_STATUS status);
A_STATUS DevDsrHandler(void *context);
A_STATUS DevCheckPendingRecvMsgsAsync(void *context);
void     DevDumpRegisters(AR6K_IRQ_PROC_REGISTERS   *pIrqProcRegs,
                          AR6K_IRQ_ENABLE_REGISTERS *pIrqEnableRegs);

#define DEV_STOP_RECV_ASYNC TRUE
#define DEV_STOP_RECV_SYNC  FALSE
#define DEV_ENABLE_RECV_ASYNC TRUE
#define DEV_ENABLE_RECV_SYNC  FALSE
A_STATUS DevStopRecv(AR6K_DEVICE *pDev, A_BOOL ASyncMode);
A_STATUS DevEnableRecv(AR6K_DEVICE *pDev, A_BOOL ASyncMode);
A_STATUS DevEnableInterrupts(AR6K_DEVICE *pDev);
A_STATUS DevDisableInterrupts(AR6K_DEVICE *pDev);

static __inline A_STATUS DevSendPacket(AR6K_DEVICE *pDev, HTC_PACKET *pPacket, A_UINT32 SendLength) {
    A_UINT32 paddedLength;
    A_BOOL   sync = (pPacket->Completion == NULL) ? TRUE : FALSE;
    A_STATUS status;

       /* adjust the length to be a multiple of block size if appropriate */
    paddedLength = (SendLength + (pDev->BlockMask)) &
                    (~(pDev->BlockMask));
	   
#if 0 /* BufferLength may not be set in , fix this... */
    if (paddedLength > pPacket->BufferLength) {
        AR_DEBUG_ASSERT(FALSE);
        if (pPacket->Completion != NULL) {
            COMPLETE_HTC_PACKET(pPacket,A_EINVAL);
        }
        return A_EINVAL;
    }
#endif
    NDIS_DEBUG_PRINTF(0,
                "DevSendPacket, Padded Length: %d Mbox:0x%X (mode:%s)\n",
                paddedLength,
                pDev->MailboxAddress,
                sync ? "SYNC" : "ASYNC");

    status = HIFReadWrite((HIF_DEVICE *)pDev->HIFDevice,
                          pDev->MailboxAddress,
                          pPacket->pBuffer,
                          paddedLength,     /* the padded length */
                          sync ? HIF_WR_SYNC_BLOCK_INC : HIF_WR_ASYNC_BLOCK_INC,
                          sync ? NULL : pPacket);  /* pass the packet as the context to the HIF request */


    if (sync) {
        pPacket->Status = status;
    }

    return status;
}

static __inline A_STATUS DevRecvPacket(AR6K_DEVICE *pDev, HTC_PACKET *pPacket, A_UINT32 RecvLength) 
{
	A_UINT32 paddedLength;
	A_STATUS status;
	A_BOOL   sync = (pPacket->Completion == NULL) ? TRUE : FALSE;

	/* adjust the length to be a multiple of block size if appropriate */
	paddedLength = (RecvLength + (pDev->BlockMask)) & (~(pDev->BlockMask));
		
	if (paddedLength > pPacket->BufferLength) 
	{
		AR_DEBUG_ASSERT(FALSE);
		NDIS_DEBUG_PRINTF(0,
	            "DevRecvPacket, Not enough space for padlen:%d recvlen:%d bufferlen:%d \r\n",
	                paddedLength,RecvLength,pPacket->BufferLength);
	    if (pPacket->Completion != NULL) {
	        COMPLETE_HTC_PACKET(pPacket,A_EINVAL);
	    }
	    return A_EINVAL;
	}

	NDIS_DEBUG_PRINTF(0,
	            "DevRecvPacket, Padded Length: %d Mbox:0x%X (mode:%s) \r\n",
	            paddedLength,
	            pDev->MailboxAddress,
	            sync ? "SYNC" : "ASYNC");

	status = HIFReadWrite((HIF_DEVICE *)pDev->HIFDevice,
	                      pDev->MailboxAddress,
	                      pPacket->pBuffer,
	                      paddedLength,
	                      sync ? HIF_RD_SYNC_BLOCK_INC : HIF_RD_ASYNC_BLOCK_INC,
	                      sync ? NULL : pPacket);  /* pass the packet as the context to the HIF request */

	if (sync) {
	    pPacket->Status = status;
	}

    return status;
}

#ifdef MBOXHW_UNIT_TEST
A_STATUS DoMboxHWTest(AR6K_DEVICE *pDev);
#endif

#endif /*AR6K_H_*/
