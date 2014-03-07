//------------------------------------------------------------------------------
// <copyright file="netbuf.h" company="Atheros and Microsoft">
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
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// Net Buffer utility function defintions
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif

extern void *
a_netbuf_alloc (A_UINT32 len);

extern A_STATUS
a_netbuf_pull(void *osbuf, A_UINT32 len);

extern A_UINT8 *
a_netbuf_to_data(void *osbuf);

extern A_UINT32
a_netbuf_to_len (void *osbuf);

extern A_STATUS
a_netbuf_push(void *osbuf, A_UINT32 len);

extern void
a_netbuf_free (void *osbuf);

extern A_UINT32
a_netbuf_headroom(void *osbuf);

extern void
a_netbuf_put(void *osbuf, A_UINT32 len);

extern void
a_netbuf_reset(void *osbuf);

#ifdef __cplusplus
}
#endif
