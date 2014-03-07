//------------------------------------------------------------------------------
// <copyright file="dset_api.h" company="Atheros">
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
// Host-side DataSet API.
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _DSET_API_H_
#define _DSET_API_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Host-side DataSet support is optional, and is not
 * currently required for correct operation.  To disable
 * Host-side DataSet support, set this to 0.
 */
#define CONFIG_HOST_DSET_SUPPORT 1

/* Called to send a DataSet Open Reply back to the Target. */
A_STATUS wmi_dset_open_reply(struct wmi_t *wmip,
                             A_UINT32 status,
                             A_UINT32 access_cookie,
                             A_UINT32 size,
                             A_UINT32 version,
                             A_UINT32 targ_handle,
                             A_UINT32 targ_reply_fn,
                             A_UINT32 targ_reply_arg);

/* Called to send a DataSet Data Reply back to the Target. */
A_STATUS wmi_dset_data_reply(struct wmi_t *wmip,
                             A_UINT32 status,
                             A_UINT8 *host_buf,
                             A_UINT32 length,
                             A_UINT32 targ_buf,
                             A_UINT32 targ_reply_fn,
                             A_UINT32 targ_reply_arg);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _DSET_API_H_ */
