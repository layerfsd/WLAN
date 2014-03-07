//------------------------------------------------------------------------------
// <copyright file="dset.h" company="Atheros">
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
// DSET structures definition
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef _DSET_H_
#define _DSET_H_

/*
 * Structures used to communicate between an application and the kernel for
 * DataSet ioctls.
 *
 * Pass a dset_request_t with cmd=AR6000_XIOCTL_WMI_WAITDSETREQ in order to
 * wait for the next DataSet Request to arrive.  Once a request is received
 * from the Target, the kernel fills in information about the Request and
 * returns from the ioctl to the application.  When the ioctl returns,
 * cmd is set to either AR6000_OPEN_REQ or AR6000_DATA_REQ.
 * 
 * Pass a dset_open_reply_t with cmd=AR6000_IOCTL_WMI_DSET_OPEN_REPLY in order
 * to send a DataSet Open reply to the Target.  The targ_* fields should simply
 * be copied from the original Open Request.  The status field should be set
 * to 0 for success or non-zero for failure.
 *
 * Pass a dset_data_reply_t with cmd=AR6000_IOCTL_WMI_DSET_DATA_REPLY in order
 * to send a DataSet Data reply to the Target.  The targ_* fields should simply
 * be copied from the original Data Request.  The status field should be set
 * to 0 for success or non-zero for failure.  The buf field is a pointer
 * to a buffer that contains the requested data.
 */

/* A DataSet Request, either Open Request or Data Request */
typedef struct dset_request_s {
    unsigned int      cmd;
    union {
        struct open_req_s {
            unsigned int        id;
            unsigned int        targ_handle;
            unsigned int        targ_reply_fn;
            unsigned int        targ_reply_arg;
        } open_req;
        struct data_req_s {
            unsigned int        access_cookie;
            unsigned int        offset;
            unsigned int        length;
            unsigned int        targ_buf;
            unsigned int        targ_reply_fn;
            unsigned int        targ_reply_arg;
        } data_req;
    } u;
} dset_request_t;

/* 
 * Values in cmd on return from an AR6000_IOCTL_EXTENDED ioctl that had
 * cmd=AR6000_XIOCTL_WMI_WAITDSETREQ.
 */
#define AR6000_OPEN_REQ         1
#define AR6000_DATA_REQ         2

/* Open Reply from Application to Kernel (to be sent to Target) */
typedef struct dset_open_reply {
    unsigned int        cmd;
    unsigned int        status;
    unsigned int        targ_handle;
    unsigned int        targ_reply_fn;
    unsigned int        targ_reply_arg;
    unsigned int        access_cookie;
    unsigned int        size;
    unsigned int        version;
} dset_open_reply_t;

/* Data Reply from Application to Kernel (to be sent to Target) */
typedef struct dset_data_reply {
    unsigned int        cmd;
    unsigned int        status;
    char               *buf;
    unsigned int        length;
    unsigned int        targ_buf;
    unsigned int        targ_reply_fn;
    unsigned int        targ_reply_arg;
} dset_data_reply_t;

#endif /* _DSET_H_ */
