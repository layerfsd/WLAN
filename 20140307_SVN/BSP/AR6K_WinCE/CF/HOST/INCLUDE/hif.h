//------------------------------------------------------------------------------
// <copyright file="hif.h" company="Atheros">
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
// HIF specific declarations and prototypes
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _HIF_H_
#define _HIF_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Header files */
#include "athdefs.h"
#include "athtypes.h"
#include "osapi.h"

typedef struct htc_callbacks HTC_CALLBACKS;
typedef struct hif_request HIF_REQUEST;
typedef struct hif_device HIF_DEVICE;

typedef enum {
    HIF_READ,
    HIF_WRITE
} HIF_DIRECTION;

typedef enum {
    HIF_BASIC_IO,
    HIF_EXTENDED_IO
} HIF_CMDTYPE;

typedef enum {
    HIF_SYNCHRONOUS,
    HIF_ASYNCHRONOUS
} HIF_EXECMODE;

typedef enum {
    HIF_BYTE_BASIS,
    HIF_BLOCK_BASIS
} HIF_DATAMODE;

typedef enum {
    HIF_FIXED_ADDRESS,
    HIF_INCREMENTAL_ADDRESS
} HIF_ADDRMODE;

#define HIF_MAX_DEVICES                 1

#if defined(SDIO)
#define HIF_MBOX_BLOCK_SIZE             128
#define HIF_MBOX_BASE_ADDR              0x800
#define HIF_MBOX_WIDTH                  0x800
#define HIF_MBOX0_BLOCK_SIZE            1
#define HIF_MBOX1_BLOCK_SIZE            HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX2_BLOCK_SIZE            HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX3_BLOCK_SIZE            HIF_MBOX_BLOCK_SIZE
#elif defined(MSIO)
#define HIF_MBOX_BASE_ADDR              0x800
#define HIF_MBOX_WIDTH                  0x800
#define HIF_MBOX0_BLOCK_SIZE            1
#define HIF_MBOX1_BLOCK_SIZE            1
#define HIF_MBOX2_BLOCK_SIZE            1
#define HIF_MBOX3_BLOCK_SIZE            1
#elif defined(CF)
#define HIF_MBOX_BLOCK_SIZE             2
#define HIF_MBOX_BASE_ADDR              0x0
#define HIF_MBOX_WIDTH                  0x100
#ifdef ONLY_16BIT
#define HIF_MBOX0_BLOCK_SIZE            HIF_MBOX_BLOCK_SIZE
#else
#define HIF_MBOX0_BLOCK_SIZE            1
#endif
#define HIF_MBOX1_BLOCK_SIZE            HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX2_BLOCK_SIZE            HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX3_BLOCK_SIZE            HIF_MBOX_BLOCK_SIZE
#else
#error "Unsupported interface"
#endif

#define HIF_FRAME_REQUEST(request, _direction, _type,    \
                          _emode, _dmode, _amode) do {   \
    (request)->direction = _direction;                   \
    (request)->type = _type;                             \
    (request)->emode = _emode;                           \
    (request)->dmode = _dmode;                           \
    (request)->amode = _amode;                           \
} while(0)

#define HIF_MBOX_START_ADDR(mbox)                        \
    HIF_MBOX_BASE_ADDR + mbox * HIF_MBOX_WIDTH

#define HIF_MBOX_END_ADDR(mbox)	                         \
    HIF_MBOX_START_ADDR(mbox) + HIF_MBOX_WIDTH - 1

struct htc_callbacks {
    A_UCHAR *name;
    A_UINT32 id;
    A_STATUS (* deviceInsertedHandler)(HIF_DEVICE *device);
    A_STATUS (* deviceRemovedHandler)(HIF_DEVICE *device);
    A_STATUS (* deviceSuspendHandler)(HIF_DEVICE *device);
    A_STATUS (* deviceResumeHandler)(HIF_DEVICE *device);
    A_STATUS (* deviceWakeupHandler)(HIF_DEVICE *device);
    A_STATUS (* rwCompletionHandler)(void *context, A_STATUS status);
    A_STATUS (* deviceInterruptDisabler)(HIF_DEVICE *device,A_BOOL *callDsr);
    A_STATUS (* deviceInterruptEnabler)(HIF_DEVICE *device);
    A_STATUS (* dsrHandler)(HIF_DEVICE *device);
};

/* 
 * The request structure captures different attributes that can precisely 
 * characterize a command and its behavior for different physical interfaces.
 */
struct hif_request {
    HIF_DIRECTION       direction; /* HIF_READ/HIF_WRITE */
    HIF_CMDTYPE	        type; /* HIF_BASIC_IO/HIF_EXTENDED_IO */
    HIF_EXECMODE        emode; /* HIF_SYNCHRONOUS/HIF_ASYNCHRONOUS */
    HIF_DATAMODE        dmode; /* HIF_BYTE_BASIS/HIF_BLOCK_BASIS */
    HIF_ADDRMODE        amode; /* HIF_FIXED_ADDRESS/HIF_INCREMENTAL_ADDRESS */
};

/* 
 * This API is used by the HTC layer to register different callback routines
 * with the HIF layer. Support for following events has been captured - DSR,
 * Read/Write completion, Device insertion/removal, Device suspension/
 * resumption/wakeup. In addition to this, the API is also used to register
 * the name and the revision of the chip. The latter can be used to verify
 * the revision of the chip read from the device before reporting it to HTC.
 */
void
HIFRegisterCallbacks(HTC_CALLBACKS *callbacks);

/*
 * This API is used to provide the read/write interface over the specific bus
 * interface.
 * address - Starting address in the dragon's address space. For mailbox
 *           writes, it refers to the start of the mbox boundary. It should
 *           be ensured that the last byte falls on the mailbox's EOM. For 
 *           mailbox reads, it refers to the end of the mbox boundary.
 * buffer - Pointer to the buffer containg the data to be transmitted or
 *          received.
 * length - Amount of data to be transmitted or received.
 * request - Characterizes the attributes of the command.
 *     direction - Direction of transfer (HIF_READ/HIF_WRITE).
 *     type - An interface may support different kind of read/write commands. 
 *            For example: SDIO supports CMD52/CMD53s. In case of MSIO it 
 *            translates to using different kinds of TPCs. The command type 
 *            is thus divided into a basic and an extended command and can 
 *            be specified using HIF_BASIC_IO/HIF_EXTENDED_IO.
 *     emode - This indicates the whether the command is to be executed in a 
 *             blocking or non-blocking fashion (HIF_SYNCHRONOUS/
 *             HIF_ASYNCHRONOUS). The read/write data paths in HTC have been 
 *             implemented using the asynchronous mode allowing the the bus 
 *             driver to indicate the completion of operation through the 
 *             registered callback routine. The requirement primarily comes 
 *             from the contexts these operations get called from (a driver's 
 *             transmit context or the ISR context in case of receive). 
 *             Support for both of these modes is essential.
 *     dmode - An interface may support different kinds of commands based on 
 *             the tradeoff between the amount of data it can carry and the 
 *             setup time. Byte and Block modes are supported (HIF_BYTE_BASIS/
 *             HIF_BLOCK_BASIS). In case of latter, the data is rounded off 
 *             to the nearest block size by padding. The size of the block is 
 *             configurable at compile time using the HIF_BLOCK_SIZE and is 
 *             negotiated with the target during initialization after the 
 *             dragon interrupts are enabled.
 *     amode - This indicates if the address has to be incremented on dragon 
 *             after every read/write operation (HIF?FIXED_ADDRESS/
 *             HIF_INCREMENTAL_ADDRESS).
 */
A_STATUS 
HIFReadWrite(HIF_DEVICE    *device, 
             A_UINT32       address, 
             A_UCHAR       *buffer, 
             A_UINT32       length, 
             HIF_REQUEST   *request, 
             void          *context);

#if 0
Not required since the hif layer can automatically do all this once the device
is discovered and then report the instance to the HTC layer only after
everything is successfully finished.
/*
 * This is called right after the device insertion event is reported to HTC.
 * It is expected to perform atleast the following functions:
 *   i) Configure the interface - bus width, clock rate, operational current.
 *  ii) Enable the interrupts and unmask any IRQs.
 * iii) Enable dragon by writing to the IO Enable bit if the interface supports
 *      one.
 */
A_STATUS
HIFInitializeInterface(void *device);
#endif

/* 
 * This can be initiated from the unload driver context ie when the HTCShutdown
 * routine is called.
 */
void
HIFShutDownDevice(HIF_DEVICE *device);

/*
 * This should translate to an acknowledgment to the bus driver indicating that
 * the previous interrupt request has been serviced and the all the relevant
 * sources have been cleared. HTC is ready to process more interrupts.
 * This should prevent the bus driver from raising an interrupt unless the
 * previous one has been serviced and acknowledged using the previous API.
 */
void
HIFAckInterrupt(HIF_DEVICE *device);

void
HIFMaskInterrupt(HIF_DEVICE *device);

void
HIFUnMaskInterrupt(HIF_DEVICE *device);

#ifdef __cplusplus
}
#endif

#endif /* _HIF_H_ */
