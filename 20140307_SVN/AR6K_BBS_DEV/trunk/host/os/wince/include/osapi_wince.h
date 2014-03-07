//------------------------------------------------------------------------------
// <copyright file="osapi_wince.h" company="Atheros">
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
// This file contains the definitions of the basic atheros apis for wince 
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _OSAPI_WINCE_H_
#define _OSAPI_WINCE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


//#ifndef DEBUG
//#define DEBUG
//#endif


#include <windows.h>
#include <winsock2.h>
#include <mmsystem.h>
#ifdef DEBUG
#include <dbgapi.h>
#endif
#include <winbase.h>
#include "athdefs.h"
#include "a_types.h"

/* 
 * Atomic operations
 */

typedef unsigned int A_ATOMIC_T;

#define A_ATOMIC_SET(counter, val) (*counter)=val
#define A_ATOMIC_INC(counter) InterlockedIncrement(counter)
#define A_ATOMIC_DEC(counter) InterlockedDecrement(counter)
#define A_ATOMIC_DEC_AND_TEST(counter) (!(InterlockedDecrement(counter)))
#define A_ATOMIC_CNT(counter) (*counter)

/* 
 * Mutual Exclusion 
 */
typedef CRITICAL_SECTION A_MUTEX_T;

#define A_MUTEX_INIT(_nt)      InitializeCriticalSection(_nt)
#define A_MUTEX_DESTROY(_nt)   DeleteCriticalSection(_nt)
#define A_MUTEX_LOCK(_nt)      EnterCriticalSection(_nt)
#define A_MUTEX_UNLOCK(_nt)    LeaveCriticalSection(_nt)
#define	A_MUTEX_LOCK_BH(_nt)   EnterCriticalSection(_nt)
#define	A_MUTEX_UNLOCK_BH(_nt) LeaveCriticalSection(_nt)
#define	A_MUTEX_LOCK_ASSERT(_nt)
#define A_IS_MUTEX_VALID(mutex)         TRUE  /* okay to return true, since A_MUTEX_DELETE does nothing */
#define A_MUTEX_DELETE(mutex)           /* spin locks are not kernel resources so nothing to free.. */

#define A_MEMZERO(addr,len) memset(addr, 0, len)
#define A_MEMCPY(dst, src, len) memcpy(dst, src, len)
#define A_MEMCMP(dst, src, len) memcmp(dst, src, len)

#define A_GET_MS(offset)    \
	(((jiffies / HZ) * 1000) + (offset))

#define A_SPRINTF         sprintf

#define __func__		_T(__FUNCTION__)
#define INLINE			__inline
/*
 * Malloc macros
 */

#define A_MALLOC(size) malloc(size)
#define A_FREE(addr) free(addr)

#define A_MALLOC_NOWAIT A_MALLOC

/*
 * Endian Macros
 */
#define A_BE2CPU8(x)       (x)
#define A_BE2CPU16(x)      ntohs(x)
#define A_BE2CPU32(x)      ntohl(x)

#define A_LE2CPU8(x)       (x)
#define A_LE2CPU16(x)      (x)
#define A_LE2CPU32(x)      (x)

#define A_CPU2BE8(x)       (x)
#define A_CPU2BE16(x)      htons(x)
#define A_CPU2BE32(x)      htonl(x)

/*
 * Timer Functions
 */
#define A_MDELAY(msecs)     Sleep(msecs)

typedef void (*A_TIMER_FN) (PVOID arg);

typedef struct a_timer_t {
    MMRESULT    timerId;
	A_TIMER_FN  callback;
	PVOID       arg;
} A_TIMER;


#define PREPACK
#define POSTPACK                __ATTRIB_PACK

extern A_STATUS
ar6000_init_timer(A_TIMER *pTimer, A_TIMER_FN callback, PVOID arg);

extern void
ar6000_set_timer(A_TIMER *pTimer, unsigned long timeout, A_INT32 periodic);

extern A_STATUS
ar6000_cancel_timer(A_TIMER *pTimer); 

extern A_STATUS
ar6000_delete_timer(A_TIMER *pTimer); 

extern A_UINT32
ar6000_ms_tickget();

#define A_INIT_TIMER ar6000_init_timer 

#define A_TIMEOUT_MS ar6000_set_timer

#define A_UNTIMEOUT  ar6000_cancel_timer

#define A_DELETE_TIMER ar6000_delete_timer

#define A_MS_TICKGET  ar6000_ms_tickget

/*
 * Wait Queue related functions
 */
typedef HANDLE A_WAITQUEUE_HEAD;

#define A_INIT_WAITQUEUE_HEAD(pHandle)  do { \
    *pHandle = CreateEvent(NULL,FALSE,FALSE,NULL); \
} while (0)

#define A_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pHandle, condition, timeout) do { \
    if (!(condition)) { \
        WaitForSingleObject(*pHandle,timeout); \
    } \
} while (0)

#define A_WAKE_UP(pHandle) SetEvent(*pHandle)

#define A_DELETE_WAITQUEUE_HEAD(pHandle)  CloseHandle(*pHandle)

#define A_RESET_WAIT_EVENT(pHandle) ResetEvent(*pHandle)
/*
 * Debug related macros
 */

#define A_CHECK_DRV_TX()

//#define ATHR_DISPLAY_MSG(args...)

#define DBG_ERROR       0x00000002


/* about Debug setting */
// 0 ~ 3	reserved, 0:ERR, 1:TRACE, 2:reserved, 3:reserved

#define DBG_SET_OID_LOG		"[WLAN: SET_OID]"
#define DBG_QUERY_OID_LOG	"[WLAN: QUERY_OID]"

/* about registery debug flag */
#define DBG_SET_OID			1<<4
#define DBG_QUERY_OID		1<<5
#define DBG_BSS_EVENT		1<<6
#define DBG_BSS_LIST		1<<7
#define DBG_ERR_LOG			1<<8

/* about debug level 
*	0 : disable debug message
*	1 : normal debug message 
*	2 : include ndis layer debug message(about car6k class)
*	3 : include wmi, htc layer debug message.
*	4 : include 2 & hif layer debug message.
*/
#define DBG_LEVEL_CAR6K		1<<9
#define DBG_LEVEL_WMI_HTC	1<<10
#define DBG_LEVEL_HIF		1<<11


#define AR_DEBUG_LVL_CHECK(x) 0

#define AR_DEBUG_PRINTF
#define AR_DEBUG_PRINTBUF
#define A_DPRINTF

#define HTC_DEBUG_PRINTBUF(buffer, length)
#define BMI_DEBUG_PRINTF	
#define WMI_DEBUG_PRINTF	
#define HIF_DEBUG_PRINTF	
#define HTC_DEBUG_PRINTF	
#define WLAN_DEBUG_PRINTF	
#define NDIS_DEBUG_PRINTF					DbgPrintf



#define AR_DEBUG_ASSERT(test) do {               \
    if (!(test)) {                               \
        RETAILMSG(1,(L"Debug Assert Caught, Function %s \r\n",_T(__FUNCTION__)));         \
    }                                            \
} while(0)


#define DBGFMT  "%s() : "
#define DBGARG  __FUNCTION__

#define __ATTRIB_PACK

#define PREPACK
#define POSTPACK                __ATTRIB_PACK

#define A_ASSERT	AR_DEBUG_ASSERT
/*
 * Network buffer support
 */

//typedef struct sk_buff_head A_NETBUF_QUEUE_T;


#define A_NETBUF_ALLOC(size) \
    a_netbuf_alloc(size)
#define A_NETBUF_ALLOC_RAW(size) \
	a_netbuf_alloc_raw(size)
#define A_NETBUF_HEADROOM(bufPtr)\
    a_netbuf_headroom(bufPtr)
#define A_NETBUF_DATA(bufPtr) \
    a_netbuf_to_data(bufPtr)
#define A_NETBUF_LEN(bufPtr) \
    a_netbuf_to_len(bufPtr)
#define A_NETBUF_PUSH(bufPtr, len) \
    a_netbuf_push(bufPtr, len)
#define A_NETBUF_PUT(bufPtr, len) \
    a_netbuf_put(bufPtr, len)	
#define A_NETBUF_PULL(bufPtr, len) \
    a_netbuf_pull(bufPtr, len)
#define A_NETBUF_FREE(bufPtr) \
    a_netbuf_free(bufPtr)	

/* return the beginning of the headroom for the buffer */
#define A_NETBUF_HEAD(bufPtr) \
        (((ndis_mini_buf_t*)(bufPtr))->head);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OSAPI_WINCE_H_ */
