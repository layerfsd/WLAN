//------------------------------------------------------------------------------
// <copyright file="osapi.h" company="Atheros">
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
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
// Author(s): ="Atheros"
//==============================================================================
#ifndef _OSAPI_WINCE_H_
#define _OSAPI_WINCE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <windows.h>
#include <winsock2.h>
#include <mmsystem.h>
#ifdef DEBUG
#include <dbgapi.h>
#endif
#include <winbase.h>
#include "athdefs.h"
#include "athtypes.h"

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

#define A_MEMZERO(addr,len) memset(addr, 0, len)
#define A_MEMCPY(dst, src, len) memcpy(dst, src, len)
#define A_MEMCMP(dst, src, len) memcmp(dst, src, len)

/*
 * Malloc macros
 */

#define A_MALLOC(size) malloc(size)
#define A_FREE(addr) free(addr)

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
/*
 * Debug related macros
 */

#ifdef DEBUG
#define WINCEDEBUG DEBUGZONE
#else
#define WINCEDEBUG
#endif

#define	ATH_LOG_ERR     WINCEDEBUG(0)
#define ATH_LOG_WARN    WINCEDEBUG(1)
#define ATH_LOG_INF     WINCEDEBUG(2)
#define ATH_LOG_TRC     WINCEDEBUG(3)
#define ATH_LOG_SYNC    WINCEDEBUG(4)
#define ATH_LOG_SEND    WINCEDEBUG(5)
#define ATH_LOG_RECV    WINCEDEBUG(6)
#define ATH_LOG_DUMP    WINCEDEBUG(7)
#define	ATH_DEBUG_BMI   WINCEDEBUG(8)
#define	ATH_DEBUG_WMI   WINCEDEBUG(9)
#define	ATH_DEBUG_HIF   WINCEDEBUG(10)
#define	ATH_DEBUG_HTC   WINCEDEBUG(11)
#define	ATH_DEBUG_WLAN  WINCEDEBUG(12)
#define	ATH_DEBUG_NET   WINCEDEBUG(13)

#ifdef DEBUG

extern void
dumpBytes(A_UCHAR *buffer, A_UINT16 length);

#define HTC_DEBUG_PRINTBUF(buffer, length) do {   \
    if (ATH_LOG_DUMP) {             \
        dumpBytes(buffer, length);               \
    }                                            \
} while(0)

void
DbgPrintf(A_INT32 flags, A_CHAR *format, ...);

#define BMI_DEBUG_PRINTF if (ATH_DEBUG_BMI) DbgPrintf
#define WMI_DEBUG_PRINTF if (ATH_DEBUG_WMI) DbgPrintf
#define HIF_DEBUG_PRINTF if (ATH_DEBUG_HIF) DbgPrintf
#define HTC_DEBUG_PRINTF if (ATH_DEBUG_HTC) DbgPrintf
#define WLAN_DEBUG_PRINTF if (ATH_DEBUG_WLAN) DbgPrintf
#define NET_DEBUG_PRINTF if (ATH_DEBUG_NET) DbgPrintf


#define AR_DEBUG_ASSERT(test) do {               \
    if (!(test)) {                               \
        DbgPrintf(ATH_LOG_ERR, ("Debug Assert Caught, File %s, Line: %d, Test:%s \n",__FILE__, __LINE__,#test));         \
    }                                            \
} while(0)
#else

#define HTC_DEBUG_PRINTBUF(buffer, length)
#define BMI_DEBUG_PRINTF
#define WMI_DEBUG_PRINTF
#define HIF_DEBUG_PRINTF
#define HTC_DEBUG_PRINTF
#define WLAN_DEBUG_PRINTF
#define NET_DEBUG_PRINTF
#define AR_DEBUG_ASSERT(test)

#endif // DEBUG

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OSAPI_WINCE_H_ */
