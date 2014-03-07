//------------------------------------------------------------------------------
// <copyright file="htc_raw_drv.c" company="Atheros">
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
// Author(s): ="Atheros"
//==============================================================================
#ifndef HTC_RAW_H_
#define HTC_RAW_H_

#include "htc_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* HTC RAW streams */
typedef enum _HTC_RAW_STREAM_ID {
    HTC_RAW_STREAM_NOT_MAPPED = -1,
    HTC_RAW_STREAM_0 = 0,
    HTC_RAW_STREAM_1 = 1,
    HTC_RAW_STREAM_2 = 2,
    HTC_RAW_STREAM_3 = 3,
    HTC_RAW_STREAM_NUM_MAX
} HTC_RAW_STREAM_ID;

A_INT32 ar6000_htc_raw_open(HTC_HANDLE);

size_t ar6000_htc_raw_read(HTC_HANDLE, HTC_RAW_STREAM_ID StreamID, char *buffer, A_INT32 length);

size_t ar6000_htc_raw_write(HTC_HANDLE, HTC_RAW_STREAM_ID StreamID, char *buffer, A_INT32 length);

A_INT32 ar6000_htc_raw_close(HTC_HANDLE);

#ifdef __cplusplus
}
#endif

#endif /*HTC_RAW_H_*/
