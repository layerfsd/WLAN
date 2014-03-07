//------------------------------------------------------------------------------
// <copyright file="wmix.h" company="Atheros">
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
// This file contains extensions of the WMI protocol specified in the
// Wireless Module Interface (WMI).  It includes definitions of all
// extended commands and events.  Extensions include useful commands
// that are not directly related to wireless activities.  They may
// be hardware-specific, and they might not be supported on all
// implementations.
//
// Author(s): ="Atheros"
//============================================================================== 
/*
 * Extended WMIX commands are encapsulated in a WMI message with
 * cmd=WMI_EXTENSION_CMD.
 */

#ifndef _WMIX_H_
#define _WMIX_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Extended WMI commands are those that are needed during wireless
 * operation, but which are not really wireless commands.  This allows,
 * for instance, platform-specific commands.  Extended WMI commands are
 * embedded in a WMI command message with WMI_COMMAND_ID=WMI_EXTENSION_CMDID.
 * Extended WMI events are similarly embedded in a WMI event message with
 * WMI_EVENT_ID=WMI_EXTENSION_EVENTID.
 */
typedef struct {
    A_UINT16    commandId;
} __ATTRIB_PACK WMIX_CMD_HDR;

typedef enum {
    WMIX_DSETOPEN_REPLY_CMDID           = 0x2001,
    WMIX_DSETDATA_REPLY_CMDID,
    WMIX_GPIO_OUTPUT_SET_CMDID,
    WMIX_GPIO_INPUT_GET_CMDID,
    WMIX_GPIO_REGISTER_SET_CMDID,
    WMIX_GPIO_REGISTER_GET_CMDID,
    WMIX_GPIO_INTR_ACK_CMDID,
} WMIX_COMMAND_ID;

typedef enum {
    WMIX_DSETOPENREQ_EVENTID            = 0x3001,
    WMIX_DSETCLOSE_EVENTID,
    WMIX_DSETDATAREQ_EVENTID,
    WMIX_GPIO_INTR_EVENTID,
    WMIX_GPIO_DATA_EVENTID,
    WMIX_GPIO_ACK_EVENTID,
} WMIX_EVENT_ID;

/*
 * =============DataSet support=================
 */

/*
 * WMIX_DSETOPENREQ_EVENTID
 * DataSet Open Request Event
 */
typedef struct {
    A_UINT32 dset_id;
    A_UINT32 targ_dset_handle;  /* echo'ed, not used by Host, */
    A_UINT32 targ_reply_fn;     /* echo'ed, not used by Host, */
    A_UINT32 targ_reply_arg;    /* echo'ed, not used by Host, */
} __ATTRIB_PACK WMIX_DSETOPENREQ_EVENT;

/*
 * WMIX_DSETCLOSE_EVENTID
 * DataSet Close Event
 */
typedef struct {
    A_UINT32 access_cookie;
} __ATTRIB_PACK WMIX_DSETCLOSE_EVENT;

/*
 * WMIX_DSETDATAREQ_EVENTID
 * DataSet Data Request Event
 */
typedef struct {
    A_UINT32 access_cookie;
    A_UINT32 offset;
    A_UINT32 length;
    A_UINT32 targ_buf;         /* echo'ed, not used by Host, */
    A_UINT32 targ_reply_fn;    /* echo'ed, not used by Host, */
    A_UINT32 targ_reply_arg;   /* echo'ed, not used by Host, */
} __ATTRIB_PACK WMIX_DSETDATAREQ_EVENT;

typedef struct {
    A_UINT32              status;
    A_UINT32              targ_dset_handle;
    A_UINT32              targ_reply_fn;
    A_UINT32              targ_reply_arg;
    A_UINT32              access_cookie;
    A_UINT32              size;
    A_UINT32              version;
} __ATTRIB_PACK WMIX_DSETOPEN_REPLY_CMD;

typedef struct {
    A_UINT32              status;
    A_UINT32              targ_buf;
    A_UINT32              targ_reply_fn;
    A_UINT32              targ_reply_arg;
    A_UINT32              length;
    A_UINT8               buf[1];
} __ATTRIB_PACK WMIX_DSETDATA_REPLY_CMD;


/* 
 * =============GPIO support=================
 * All masks are 18-bit masks with bit N operating on GPIO pin N.
 */

#if defined(AR6000)
#include "AR6000/AR6000_gpio.h"
#endif /* AR6000 */

/*
 * Set GPIO pin output state.
 * In order for output to be driven, a pin must be enabled for output.
 * This can be done during initialization through the GPIO Configuration
 * DataSet, or during operation with the enable_mask.
 *
 * If a request is made to simultaneously set/clear or set/disable or
 * clear/disable or disable/enable, results are undefined.
 */
typedef struct {
    A_UINT32              set_mask;             /* pins to set */
    A_UINT32              clear_mask;           /* pins to clear */
    A_UINT32              enable_mask;          /* pins to enable for output */
    A_UINT32              disable_mask;         /* pins to disable/tristate */
} __ATTRIB_PACK WMIX_GPIO_OUTPUT_SET_CMD;

/* 
 * Set a GPIO register.  For debug/exceptional cases.
 * Values for gpioreg_id are GPIO_REGISTER_IDs, defined in a
 * platform-dependent header.
 */
typedef struct {
    A_UINT32              gpioreg_id;           /* GPIO register ID */
    A_UINT32              value;                /* value to write */
} __ATTRIB_PACK WMIX_GPIO_REGISTER_SET_CMD;

/* Get a GPIO register.  For debug/exceptional cases. */
typedef struct {
    A_UINT32              gpioreg_id;           /* GPIO register to read */
} __ATTRIB_PACK WMIX_GPIO_REGISTER_GET_CMD;

/*
 * Host acknowledges and re-arms GPIO interrupts.  A single
 * message should be used to acknowledge all interrupts that
 * were delivered in an earlier WMIX_GPIO_INTR_EVENT message.
 */
typedef struct {
    A_UINT32              ack_mask;             /* interrupts to acknowledge */
} __ATTRIB_PACK WMIX_GPIO_INTR_ACK_CMD;

/*
 * Target informs Host of GPIO interrupts that have ocurred since the
 * last WMIX_GIPO_INTR_ACK_CMD was received.  Additional information --
 * the current GPIO input values is provided -- in order to support
 * use of a GPIO interrupt as a Data Valid signal for other GPIO pins.
 */
typedef struct {
    A_UINT32              intr_mask;            /* pending GPIO interrupts */
    A_UINT32              input_values;         /* recent GPIO input values */
} __ATTRIB_PACK WMIX_GPIO_INTR_EVENT;

/*
 * Target responds to Host's earlier WMIX_GPIO_INPUT_GET_CMDID request
 * using a GPIO_DATA_EVENT with
 *   value set to the mask of GPIO pin inputs and
 *   reg_id set to GPIO_ID_NONE
 * 
 *
 * Target responds to Hosts's earlier WMIX_GPIO_REGISTER_GET_CMDID request
 * using a GPIO_DATA_EVENT with
 *   value set to the value of the requested register and
 *   reg_id identifying the register (reflects the original request)
 * NB: reg_id supports the future possibility of unsolicited
 * WMIX_GPIO_DATA_EVENTs (for polling GPIO input), and it may
 * simplify Host GPIO support.
 */
typedef struct {
    A_UINT32              value;
    A_UINT32              reg_id;
} __ATTRIB_PACK WMIX_GPIO_DATA_EVENT;

#ifdef __cplusplus
}
#endif

#endif /* _WMIX_H_ */
