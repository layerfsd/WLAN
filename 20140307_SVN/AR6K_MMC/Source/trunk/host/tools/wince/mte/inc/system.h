
/*++
Copyright (c) 2008 Atheros Communications Inc.
All rights reserved.


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

 System definitions


--*/

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <windows.h>
#include <a_types.h>

#define PREPACK
#define POSTPACK
#define __ATTRIB_PACK

#define DBG_ZONE_ERR     (1 << 0)
#define DBG_ZONE_INIT    (1 << 1)
#define DBG_ZONE_TRC     (1 << 2)
#define DBG_ZONE_HIF     (1 << 3)
#define DBG_ZONE_HIF_TRC (1 << 4)
#define DBG_ZONE_BMI     (1 << 5)
#define DBG_ZONE_HTC     (1 << 6)
#define DBG_ZONE_HTC_TRC (1 << 7)
#define DBG_ZONE_WMI     (1 << 8)

extern DWORD g_WiFiDbgZone;


typedef void (*A_OUTPUT_DEBUG_FUNC)(char *,...);

extern A_OUTPUT_DEBUG_FUNC g_pDebugOutputFunc;

#define DBG_LOG_PRINT(z,s) if (g_WiFiDbgZone & (z)) (*g_pDebugOutputFunc) s

#define ATH_DEBUG_WMI DBG_ZONE_WMI
#define ATH_DEBUG_BMI DBG_ZONE_BMI
#define ATH_DEBUG_ERR DBG_ZONE_ERR
#define ATH_LOG_ERR   DBG_ZONE_ERR
#define ATH_LOG_INF   DBG_ZONE_TRC
#define ATH_DEBUG_ANY 0xFFFFFFFF

#define AR_DEBUG_PRINTF(z,s) if (g_WiFiDbgZone & (z)) (*g_pDebugOutputFunc) s

#define A_MDELAY(msec) A_DelayMS((msec))
#define A_MEMZERO(addr,len) memset(addr, 0, len)
#define A_MEMCPY(dst, src, len) memcpy(dst, src, len)
#define A_MEMCMP(dst, src, len) memcmp(dst, src, len)

#define HostToNetworkOrder_16(val) ((((val) << 8) & 0xFF00) | (((val) >> 8) & 0x00FF))
#define NetworkOrderToHost_16(val) ((((val) << 8) & 0xFF00) | (((val) >> 8) & 0x00FF))

#define A_ASSERT(test)                                \
    do {                                             \
        if (!(test)) {                               \
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Debug Assert Caught, File %s, Line: %d, Test:%s \n",__FILE__, __LINE__,#test));         \
        }                                            \
    } while(0)

#define AR_DEBUG_ASSERT A_ASSERT
void DebugDumpBytes(A_UCHAR *buffer, A_UINT16 length, char *pDescription);

#define A_GET_SECONDS_TICK() A_GetSecsTick()

typedef struct _AR6K_FIRMWARE_INFO {
    A_UINT8     *pAthwlan;
    A_UINT32    AthwlanLength;
    A_BOOL      AthwlanCompressed;
    A_UINT8     *pDataPatch;
    A_UINT32    DataPatchLength;
} AR6K_FIRMWARE_INFO;

A_BOOL GetFirmwareInfo(AR6K_FIRMWARE_INFO *pInfo);
void 	A_DelayMS(int Milliseconds);
DWORD 	A_GetSecsTick();

#endif
