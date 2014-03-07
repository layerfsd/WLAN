//------------------------------------------------------------------------------
// <copyright file="debug_wince.h" company="Atheros">
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
#ifndef _DEBUG_WINCE_H_
#define _DEBUG_WINCE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef DEBUG
#include <dbgapi.h>
#endif
#include <winbase.h>
#include "athdefs.h"
#include "athtypes_wince.h"

//buildercheck
#ifdef MMC_PATCH_RETAIL_DEBUG
#define RETAIL_DEBUG_PRINTF RETAILMSG
#else
#define RETAIL_DEBUG_PRINTF
#endif
extern ULONG	debugBssInfo;	//buildercheck
extern ULONG	debugFileLog;	//buildercheck
extern ULONG	debugErrorLog;	//buildercheck
extern HANDLE targetfailEvent;

#ifdef DEBUG
#define WINCEDEBUG DEBUGZONE
#else
#define WINCEDEBUG
#endif

void DisplayMsg(WCHAR * format, ...);
#define ATHR_DISPLAY_MSG DisplayMsg

#define DBG_INFO        0x00000001
#define DBG_ERROR       0x00000002
#define DBG_WARNING     0x00000004
#define DBG_SDIO        0x00000008
#define DBG_HIF         0x00000010
#define DBG_BMI         0x00000020
#define DBG_HTC         0x00000040
#define DBG_WMI         0x00000080
#define DBG_WMI2        0x00000100
#define DBG_DRIVER      0x00000200
#define DBG_MP_MAIN     0x00000400
#define DBG_MP_OID      0x00000800
#define DBG_MP_RECV     0x00001000
#define DBG_MP_SEND     0x00002000
#define DBG_STA_MAIN    0x00004000
#define DBG_ATH_HW      0x00008000
#define DBG_MISC_DRV    0x00010000
#define DBG_EEPROM      0x00020000
#define DBG_HTC_RAW     0x00040000
#define DBG_DEFAULTS    (DBG_ERROR|DBG_WARNING)

#if 0
#define ATH_LOG_INF      0x00000001
#define ATH_DEBUG_INF    0x00000001
#define ATH_DEBUG_TRC    0x00000002
#define ATH_LOG_TRC      0x00000002
#define ATH_LOG_ERR      0x00000004
#define ATH_DEBUG_ERR    0x00000004
#define ATH_LOG_WARN     0x00000008
#define ATH_DEBUG_WARN   0x00000008
#define ATH_DEBUG_HIF    0x00000010
#define ATH_DEBUG_BMI    0x00000020
#define ATH_DEBUG_HTC    0x00000040
#define ATH_DEBUG_WLAN   0x00000080
#define ATH_DEBUG_WMI    0x00000100
#define ATH_DEBUG_SEND   0x00000200
#define ATH_DEBUG_RECV   0x00000400
#define ATH_DEBUG_DUMP   0x00000800
#define ATH_DEBUG_MISC   0x00001000
#define ATH_DEBUG_IRQ    0x00002000
#define ATH_DEBUG_NDIS   0x00004000
#define ATH_DEBUG_ROAM   0x00008000
#define ATH_DEBUG_ANY    0xFFFFFFFF
#else
#define ATH_LOG_INF      0x00000000
#define ATH_DEBUG_INF    0x00000000
#define ATH_DEBUG_TRC    0x00000000
#define ATH_LOG_TRC      0x00000000
#define ATH_LOG_ERR      0x00000004
#define ATH_DEBUG_ERR    0x00000004
#define ATH_LOG_WARN     0x00000008
#define ATH_DEBUG_WARN   0x00000008
#define ATH_DEBUG_HIF    0x00000000
#define ATH_DEBUG_BMI    0x00000000
#define ATH_DEBUG_HTC    0x00000000
#define ATH_DEBUG_WLAN   0x00000000
#define ATH_DEBUG_WMI    0x00000000
#define ATH_DEBUG_SEND   0x00000000
#define ATH_DEBUG_RECV   0x00000000
#define ATH_DEBUG_DUMP   0x00000000
#define ATH_DEBUG_MISC   0x00000000
#define ATH_DEBUG_IRQ    0x00000000
#define ATH_DEBUG_NDIS   0x00000000
#define ATH_DEBUG_ROAM   0x00000000
#define ATH_DEBUG_ANY    0xFFFFFFFF
#endif
#define DBGFMT  "%s : "
#define DBGARG  "WMI"
#define DBGFN   A_DbgPrintf

#ifndef EIO
#define EIO A_ECOMM
#endif

#define DBG_MODULE             (DBG_MP_MAIN|DBG_MP_OID|DBG_MP_RECV|DBG_MP_SEND|DBG_HTC|DBG_BMI|DBG_HTC_RAW)
#define DBG_LEVEL              ATH_DEBUG_TRC


#ifdef ATH_DEBUG
#define ATH_DEBUG_PRINTF(_Comp, _Level, Fmt)                      \
    if ((_Comp & DBG_MODULE) && (_Level & DBG_LEVEL)) \
    DbgPrint Fmt;


#define ATH_FUNCTION_ENTRY                 \
    DbgPrint(("==> " __FUNCTION__ "\n"))

#define ATH_FUNCTION_EXIT                  \
    DbgPrint(("<== " __FUNCTION__ "\n"))

#define A_ASSERT(_Exp) ASSERT(_Exp)
#else
#define ATH_DEBUG_PRINTF(_Comp, _Level, Fmt)
#define ATH_FUNCTION_ENTRY
#define ATH_FUNCTION_EXIT
#define A_ASSERT
#endif // DEBUG

#ifdef DEBUG

    extern A_INT32 debughtc;
    extern ULONG printMask;

    extern void DebugDumpBytes(A_UCHAR *buffer, A_UINT16 length, char *pDescription);

   	#define AR_DEBUG_LVL_CHECK(lvl) (debughtc & (lvl))
    #define AR_DEBUG_PRINTBUF(buffer, length, desc) do {   		\
        if (debughtc & ATH_DEBUG_DUMP) {             			\
            DebugDumpBytes(buffer, length,desc);                \
        }                                            			\
    } while(0)
    #define PRINTX_ARG(arg) arg
    #define AR_DEBUG_PRINTF(flags, args)             			\
        if (flags&printMask) A_DbgPrintf args

    void
    DbgPrintf(A_INT32 flags, A_CHAR *format, ...);

    void
    A_DbgPrintf(A_CHAR * format, ...);

    #define A_DPRINTF(f, a) \
        if(printMask&(f)) DBGFN a

//	#define BMI_DEBUG_PRINTF  DbgPrintf
	#define BMI_DEBUG_PRINTF
	#define WMI_DEBUG_PRINTF  DbgPrintf
	#define HIF_DEBUG_PRINTF  DbgPrintf
	#define HTC_DEBUG_PRINTF  DbgPrintf
	#define WLAN_DEBUG_PRINTF DbgPrintf
	#define NET_DEBUG_PRINTF  DbgPrintf
   	#define NDIS_DEBUG_PRINTF DbgPrintf

    #define AR_DEBUG_ASSERT(test)                   \
    do {                                            \
        if (!(test)) {                               \
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Debug Assert Caught, File %s, Line: %d, Test:%s \n",__FILE__, __LINE__,#test));         \
        }                                            \
    } while(0)
#else

	#define BMI_DEBUG_PRINTF
	#define WMI_DEBUG_PRINTF
	#define HIF_DEBUG_PRINTF
	#define HTC_DEBUG_PRINTF
	#define WLAN_DEBUG_PRINTF
	#define NET_DEBUG_PRINTF
   	#define NDIS_DEBUG_PRINTF

    #define A_DPRINTF(f, a)
    #define AR_DEBUG_PRINTF(flags, args)
    #define AR_DEBUG_PRINTBUF(buffer, length,description)
    #define AR_DEBUG_ASSERT(test)
    #define AR_DEBUG_LVL_CHECK(lvl) 0

#ifdef LOG_TO_FILE
    void
    DbgPrintf(A_INT32 flags, A_CHAR *format, ...);
#endif

#endif // DEBUG

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DEBUG_WINCE_H_ */
