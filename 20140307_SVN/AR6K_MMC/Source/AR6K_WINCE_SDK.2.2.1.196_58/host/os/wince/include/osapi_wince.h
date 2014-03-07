//------------------------------------------------------------------------------
// <copyright file="osapi_wince.h" company="Atheros">
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
//
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

#include <winbase.h>
#include "athdefs.h"
#include "athtypes_wince.h"

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

#define PREPACK
#define POSTPACK                __ATTRIB_PACK

/*
 * Atomic operations
 */

typedef unsigned int A_ATOMIC_T;

#define A_ATOMIC_SET(counter, val) (*counter)=val
#define A_ATOMIC_INC(counter) InterlockedIncrement(counter)
#define A_ATOMIC_DEC(counter) InterlockedDecrement(counter)
#define A_ATOMIC_DEC_AND_TEST(counter) (!(InterlockedDecrement(counter)))
#define A_ATOMIC_CNT(counter) (*counter)

void
logPrintf(A_INT32 mask, A_CHAR * format, ...);

void
logInit(char* logpath);

void
dbgIntialize (A_UINT32 dbgPrintMask);
/*
 * Mutual Exclusion
 */
typedef CRITICAL_SECTION A_MUTEX_T;

#define A_MUTEX_INIT(_nt)      		InitializeCriticalSection(_nt)
#define A_MUTEX_DESTROY(_nt)   		DeleteCriticalSection(_nt)
#define A_MUTEX_LOCK(_nt)      		EnterCriticalSection(_nt)
#define A_MUTEX_UNLOCK(_nt)    		LeaveCriticalSection(_nt)
#define A_MUTEX_LOCK_BH(_nt)   		EnterCriticalSection(_nt)
#define A_MUTEX_UNLOCK_BH(_nt) 		LeaveCriticalSection(_nt)
#define A_MUTEX_LOCK_ASSERT(_nt)
#define A_IS_MUTEX_VALID(_nt)     	TRUE  /* okay to return true, since A_MUTEX_DELETE does nothing */
#define A_MUTEX_DELETE(_nt)       	DeleteCriticalSection(_nt) /* spin locks are not kernel resources so nothing to free.. */


#define A_MEMZERO(addr,len) 		memset(addr, 0, len)
#define A_MEMCPY(dst, src, len) 	memcpy(dst, src, len)
#define A_MEMCMP(dst, src, len) 	memcmp(dst, src, len)
#define A_SPRINTF(buf, format, str)	StringCchPrintfA (buf, STRSAFE_MAX_CCH, format, str)


/*
 * Malloc macros
 */

#define A_MALLOC(size)  malloc(size)
#define A_MALLOC_NOWAIT(size) malloc(size)
#define A_FREE(addr) free(addr)

#define A_swab64(x) \
        ((A_UINT64)( \
                (A_UINT64)(((A_UINT64)(x) & (A_UINT64)0x00000000000000ff) << 56) | \
                (A_UINT64)(((A_UINT64)(x) & (A_UINT64)0x000000000000ff00) << 40) | \
                (A_UINT64)(((A_UINT64)(x) & (A_UINT64)0x0000000000ff0000) << 24) | \
                (A_UINT64)(((A_UINT64)(x) & (A_UINT64)0x00000000ff000000) <<  8) | \
                (A_UINT64)(((A_UINT64)(x) & (A_UINT64)0x000000ff00000000) >>  8) | \
                (A_UINT64)(((A_UINT64)(x) & (A_UINT64)0x0000ff0000000000) >> 24) | \
                (A_UINT64)(((A_UINT64)(x) & (A_UINT64)0x00ff000000000000) >> 40) | \
                (A_UINT64)(((A_UINT64)(x) & (A_UINT64)0xff00000000000000) >> 56) ))

/*
 * Endian Macros
 */
#define A_BE2CPU8(x)       (x)
#define A_BE2CPU16(x)      ntohs(x)
#define A_BE2CPU32(x)      ntohl(x)

#define A_LE2CPU8(x)       (x)
#define A_LE2CPU16(x)      (x)
#define A_LE2CPU32(x)      (x)
#define A_CPU2LE64(x)      A_swab64((x))

#define A_CPU2BE8(x)       (x)
#define A_CPU2BE16(x)      htons(x)
#define A_CPU2BE32(x)      htonl(x)
#define A_CPU2BE64(x)      A_swab64((x))


/*
 * Timer Functions
 */
#define A_MDELAY(msecs)     Sleep(msecs)

/* Get current time in ms adding a constant offset (in ms) */
#define A_GET_MS(offset)    (GetTickCount() + offset)

typedef void (*A_TIMER_FN) (A_ATH_TIMER arg);

typedef struct a_timer_t {
    MMRESULT    	timerId;
    A_TIMER_FN  	callback;
    A_ATH_TIMER     arg;
} A_TIMER;


extern A_STATUS
ar6000_init_timer(A_TIMER *pTimer, A_TIMER_FN callback, A_ATH_TIMER arg);

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

/* In WM/WinCE, WLAN Rx and Tx run in different contexts, so no need to check
 * for any commands/data queued for WLAN */
#define A_CHECK_DRV_TX()

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OSAPI_WINCE_H_ */
