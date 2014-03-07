//------------------------------------------------------------------------------
// <copyright file="netbuf.h" company="Atheros and Microsoft">
//    Copyright (c) 2004-2008 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
// 
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
//------------------------------------------------------------------------------
//==============================================================================
// OS Independent Net Buffer interfaces
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Network buffer support
 */
#define A_NETBUF_ALLOC(size) \
    a_netbuf_alloc(size)
#define A_NETBUF_ALLOC_RAW(size) \
    a_netbuf_alloc_raw(size)
#define A_NETBUF_FREE(bufPtr) \
    a_netbuf_free(bufPtr)
#define A_NETBUF_DATA(bufPtr) \
    a_netbuf_to_data(bufPtr)
#define A_NETBUF_LEN(bufPtr) \
    a_netbuf_to_len(bufPtr)
#define A_NETBUF_PUSH(bufPtr, len) \
    a_netbuf_push(bufPtr, len)
#define A_NETBUF_PUT(bufPtr, len) \
    a_netbuf_put(bufPtr, len)
#define A_NETBUF_TRIM(bufPtr,len) \
    a_netbuf_trim(bufPtr, len)
#define A_NETBUF_PULL(bufPtr, len) \
    a_netbuf_pull(bufPtr, len)
#define A_NETBUF_HEADROOM(bufPtr)\
    a_netbuf_headroom(bufPtr)
#define A_NETBUF_SETLEN(bufPtr,len) \
    a_netbuf_setlen(bufPtr, len)

/* Add data to end of a buffer  */
#define A_NETBUF_PUT_DATA(bufPtr, srcPtr,  len) \
    a_netbuf_put_data(bufPtr, srcPtr, len)

/* Add data to start of the  buffer */
#define A_NETBUF_PUSH_DATA(bufPtr, srcPtr,  len) \
    a_netbuf_push_data(bufPtr, srcPtr, len)

/* Remove data at start of the buffer */
#define A_NETBUF_PULL_DATA(bufPtr, dstPtr, len) \
    a_netbuf_pull_data(bufPtr, dstPtr, len)

/* Remove data from the end of the buffer */
#define A_NETBUF_TRIM_DATA(bufPtr, dstPtr, len) \
    a_netbuf_trim_data(bufPtr, dstPtr, len)

/* View data as "size" contiguous bytes of type "t" */
#define A_NETBUF_VIEW_DATA(bufPtr, t, size) \
    (t )( ((struct skbuf *)(bufPtr))->data)

/* return the beginning of the headroom for the buffer */
#define A_NETBUF_HEAD(bufPtr) \
        ((((struct sk_buff *)(bufPtr))->head))

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
