/*
 * Copyright (c) 2004-2007 Atheros Communications Inc.
 * All rights reserved.
 * 
 * 
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
 * 
 * simplied HIF definitions
 */

#ifndef _U_HIF_H_
#define _U_HIF_H_


#include "athdefs.h"
#include "a_types.h"

#define ENDPOINT1            0
#define HTC_MAILBOX_NUM_MAX  4

typedef void HIF_DEVICE;

/*
 * direction - Direction of transfer (HIF_READ/HIF_WRITE).
 */
#define HIF_READ                    0x00000001
#define HIF_WRITE                   0x00000002
#define HIF_DIR_MASK                (HIF_READ | HIF_WRITE)

/*
 *     type - An interface may support different kind of read/write commands. 
 *            For example: SDIO supports CMD52/CMD53s. In case of MSIO it 
 *            translates to using different kinds of TPCs. The command type 
 *            is thus divided into a basic and an extended command and can 
 *            be specified using HIF_BASIC_IO/HIF_EXTENDED_IO.
 */
#define HIF_BASIC_IO                0x00000004
#define HIF_EXTENDED_IO             0x00000008
#define HIF_TYPE_MASK               (HIF_BASIC_IO | HIF_EXTENDED_IO)

/*
 *     emode - This indicates the whether the command is to be executed in a 
 *             blocking or non-blocking fashion (HIF_SYNCHRONOUS/
 *             HIF_ASYNCHRONOUS). The read/write data paths in HTC have been 
 *             implemented using the asynchronous mode allowing the the bus 
 *             driver to indicate the completion of operation through the 
 *             registered callback routine. The requirement primarily comes 
 *             from the contexts these operations get called from (a driver's 
 *             transmit context or the ISR context in case of receive). 
 *             Support for both of these modes is essential.
 */
#define HIF_SYNCHRONOUS             0x00000010
// -- NO ASYNC -- #define HIF_ASYNCHRONOUS            0x00000020
//#define HIF_EMODE_MASK              (HIF_SYNCHRONOUS | HIF_ASYNCHRONOUS)

/*
 *     dmode - An interface may support different kinds of commands based on 
 *             the tradeoff between the amount of data it can carry and the 
 *             setup time. Byte and Block modes are supported (HIF_BYTE_BASIS/
 *             HIF_BLOCK_BASIS). In case of latter, the data is rounded off 
 *             to the nearest block size by padding. The size of the block is 
 *             configurable at compile time using the HIF_BLOCK_SIZE and is 
 *             negotiated with the target during initialization after the 
 *             dragon interrupts are enabled.
 */
#define HIF_BYTE_BASIS              0x00000040
#define HIF_BLOCK_BASIS             0x00000080
#define HIF_DMODE_MASK              (HIF_BYTE_BASIS | HIF_BLOCK_BASIS)

/*
 *     amode - This indicates if the address has to be incremented on dragon 
 *             after every read/write operation (HIF?FIXED_ADDRESS/
 *             HIF_INCREMENTAL_ADDRESS).
 */
#define HIF_FIXED_ADDRESS           0x00000100
#define HIF_INCREMENTAL_ADDRESS     0x00000200
#define HIF_AMODE_MASK              (HIF_FIXED_ADDRESS | HIF_INCREMENTAL_ADDRESS)

#define HIF_WR_ASYNC_BYTE_FIX   \
    (HIF_WRITE | HIF_ASYNCHRONOUS | HIF_EXTENDED_IO | HIF_BYTE_BASIS | HIF_FIXED_ADDRESS)
#define HIF_WR_ASYNC_BYTE_INC   \
    (HIF_WRITE | HIF_ASYNCHRONOUS | HIF_EXTENDED_IO | HIF_BYTE_BASIS | HIF_INCREMENTAL_ADDRESS)
#define HIF_WR_ASYNC_BLOCK_INC  \
    (HIF_WRITE | HIF_ASYNCHRONOUS | HIF_EXTENDED_IO | HIF_BLOCK_BASIS | HIF_INCREMENTAL_ADDRESS)
#define HIF_WR_SYNC_BYTE_FIX    \
    (HIF_WRITE | HIF_SYNCHRONOUS | HIF_EXTENDED_IO | HIF_BYTE_BASIS | HIF_FIXED_ADDRESS)
#define HIF_WR_SYNC_BYTE_INC    \
    (HIF_WRITE | HIF_SYNCHRONOUS | HIF_EXTENDED_IO | HIF_BYTE_BASIS | HIF_INCREMENTAL_ADDRESS)
#define HIF_WR_SYNC_BLOCK_INC  \
    (HIF_WRITE | HIF_SYNCHRONOUS | HIF_EXTENDED_IO | HIF_BLOCK_BASIS | HIF_INCREMENTAL_ADDRESS)
#define HIF_RD_SYNC_BYTE_INC    \
    (HIF_READ | HIF_SYNCHRONOUS | HIF_EXTENDED_IO | HIF_BYTE_BASIS | HIF_INCREMENTAL_ADDRESS)
#define HIF_RD_SYNC_BYTE_FIX    \
    (HIF_READ | HIF_SYNCHRONOUS | HIF_EXTENDED_IO | HIF_BYTE_BASIS | HIF_FIXED_ADDRESS)
#define HIF_RD_ASYNC_BYTE_FIX   \
    (HIF_READ | HIF_ASYNCHRONOUS | HIF_EXTENDED_IO | HIF_BYTE_BASIS | HIF_FIXED_ADDRESS)
#define HIF_RD_ASYNC_BLOCK_FIX  \
    (HIF_READ | HIF_ASYNCHRONOUS | HIF_EXTENDED_IO | HIF_BLOCK_BASIS | HIF_FIXED_ADDRESS)
#define HIF_RD_ASYNC_BYTE_INC   \
    (HIF_READ | HIF_ASYNCHRONOUS | HIF_EXTENDED_IO | HIF_BYTE_BASIS | HIF_INCREMENTAL_ADDRESS)
#define HIF_RD_ASYNC_BLOCK_INC  \
    (HIF_READ | HIF_ASYNCHRONOUS | HIF_EXTENDED_IO | HIF_BLOCK_BASIS | HIF_INCREMENTAL_ADDRESS)
#define HIF_RD_SYNC_BLOCK_INC  \
    (HIF_READ | HIF_SYNCHRONOUS | HIF_EXTENDED_IO | HIF_BLOCK_BASIS | HIF_INCREMENTAL_ADDRESS)


typedef enum {
    HIF_DEVICE_GET_MBOX_BLOCK_SIZE,
    HIF_DEVICE_GET_MBOX_ADDR,
} HIF_DEVICE_CONFIG_OPCODE;

/*
 * HIF CONFIGURE definitions:
 * 
 *   HIF_DEVICE_GET_MBOX_BLOCK_SIZE
 *   input : none
 *   output : array of 4 A_UINT32s
 *   notes: block size is returned for each mailbox (4)
 * 
 *   HIF_DEVICE_GET_MBOX_ADDR
 *   input : none
 *   output : array of 4 A_UINT32
 *   notes: address is returned for each mailbox (4) in the array
 * 
 */

A_STATUS HIFInit(void);

/*
 * This API is used to provide the read/write interface over the specific bus
 * interface (THIS API is ALWAYS SYNCHRONOUS).
 * address - Starting address in the dragon's address space. For mailbox
 *           writes, it refers to the start of the mbox boundary. It should
 *           be ensured that the last byte falls on the mailbox's EOM. For 
 *           mailbox reads, it refers to the end of the mbox boundary.
 * buffer - Pointer to the buffer containg the data to be transmitted or
 *          received.
 * length - Amount of data to be transmitted or received.
 * request - Characterizes the attributes of the command.
 */
A_STATUS HIFReadWrite(HIF_DEVICE    *device,
                      A_UINT32       address, 
                      A_UCHAR       *buffer, 
                      A_UINT32       length, 
                      A_UINT32       request,
                      void          *Context);


void HIFShutDown();

A_STATUS HIFConfigureDevice(HIF_DEVICE    *device,
                            HIF_DEVICE_CONFIG_OPCODE opcode, 
                            void     *config, 
                            A_UINT32 configLen);

HIF_DEVICE    *HIFGetDevice(void);

#ifdef __cplusplus
}
#endif

#endif
