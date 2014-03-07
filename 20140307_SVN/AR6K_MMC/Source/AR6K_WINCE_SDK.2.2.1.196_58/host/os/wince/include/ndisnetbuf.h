//------------------------------------------------------------------------------
// <copyright file="ndisnetbuf.h" company="Atheros and Microsoft">
//    Copyright (c) 2004-2007 Microsoft Corporation.  All rights reserved.
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
// NDIS specific netbuf structures and constants
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#define AR6000_BUFFER_SIZE    1664
#define AR6000_DATA_OFFSET    64

#include "htc_packet.h"

struct ndis_mini_buf_;

    /* return packet handler callback */
typedef void (* NDIS_BUF_RETURN_HANDLER)(void *, struct ndis_mini_buf_ *);

typedef struct ndis_mini_buf_ {
    int           buf_dat_len;
    A_UINT8*      buf_dat;
    A_UINT8*      buf;
    int           buf_len;
    NDIS_BUFFER   NdisBuffer;
    HTC_PACKET    HtcPacket;
    NDIS_BUF_RETURN_HANDLER ReturnBufferHandler;
    void           *pContext;
} ndis_mini_buf_t;
