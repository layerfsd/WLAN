//------------------------------------------------------------------------------
// <copyright file="osapi.h" company="Atheros">
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
// This file contains the definitions of the basic atheros data types.
// It is used to map the data types in atheros files to a platform specific
// type.  
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _OSAPI_H_
#define _OSAPI_H_

#ifdef UNDER_CE
#define __KERNEL__
#endif

#ifdef __KERNEL__

#ifdef __linux__
#include "../os/linux/include/osapi_linux.h"

/*
 * OS specific network buffer acess routines
 */
void *a_netbuf_alloc(A_INT32 size);
void a_netbuf_free(void *bufPtr);
void *a_netbuf_to_data(void *bufPtr);
A_UINT32 a_netbuf_to_len(void *bufPtr);
A_STATUS a_netbuf_push(void *bufPtr, A_INT32 len);
A_STATUS a_netbuf_put(void *bufPtr, A_INT32 len);
A_STATUS a_netbuf_trim(void *bufPtr, A_INT32 len);
A_INT32 a_netbuf_headroom(void *bufPtr);
A_STATUS a_netbuf_pull(void *bufPtr, A_INT32 len);
A_UINT32 a_copy_to_user(void *to, const void *from, A_UINT32 n);
A_UINT32 a_copy_from_user(void *to, const void *from, A_UINT32 n);
#endif

#ifdef UNDER_CE
#include "../os/wince/include/osapi_wince.h"
#include "../os/wince/ndis/netbuf.h"
A_UINT32 a_copy_from_user(void *to, void *from, A_UINT32 n);
#endif

#endif /* __KERNEL__ */

#ifdef __GNUC__
#define __ATTRIB_PACK           __attribute__ ((packed))
#define __ATTRIB_PRINTF         __attribute__ ((format (printf, 1, 2)))
#define __ATTRIB_NORETURN       __attribute__ ((noreturn))
#ifndef INLINE
#define INLINE                  __inline__
#endif
#else /* Not GCC */
#define __ATTRIB_PACK
#define __ATTRIB_PRINTF
#define __ATTRIB_NORETURN
#ifndef INLINE
#define INLINE                  __inline
#endif
#endif /* End __GNUC__ */

#endif /* _OSAPI_H_ */
