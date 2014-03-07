//------------------------------------------------------------------------------
// <copyright file="netbuf.c" company="Atheros and Microsoft">
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
// Net Buffer utility function library
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#include <ndis.h>
#include "athdefs.h"
#include "a_types.h"
#include "a_osapi.h"
#include "htc_api.h"
#include "hif.h"
#include "wmi.h"
#include "wlan_api.h"
#include "wmi_api.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
#include "common_drv.h"

//
// buffer alignment defaults
//
DWORD DataBufferAlignmentBytes = 0x20;
DWORD DataBufferAlignmentMask  = 0x1F;

/* need to pad to 128 bytes, our minimum SDIO block size.
 * this is an issue on Windows CE since some host controllers will
 * copy the data from our buffer into a common buffer.
 * Block padding could extend the buffer past the mapped address range and cause an
 * exception */
DWORD DataBufferPadBytes = 0x80;

void *
a_netbuf_alloc (A_UINT32 len)
{
    void            *buff = NULL;
    ndis_mini_buf_t *pb = NULL;

    pb = A_MALLOC (sizeof(ndis_mini_buf_t)  + \
                   len + AR6000_DATA_OFFSET + \
                   DataBufferAlignmentBytes + DataBufferPadBytes);

    if (pb == NULL) {
//	RETAILMSG(1, (TEXT("builderd:ar6k2:a_netbuf_alloc failed len = %d\r\n"), sizeof(ndis_mini_buf_t)  + \
//                   len + AR6000_DATA_OFFSET + \
//                   DataBufferAlignmentBytes + DataBufferPadBytes));

        return NULL;
    }

//RETAILMSG(1, (TEXT("builderd:ar6k2:a_netbuf_alloc = %x, len = %d\r\n"), (A_UINT32)pb, sizeof(ndis_mini_buf_t)  + \
//                   len + AR6000_DATA_OFFSET + \
//                   DataBufferAlignmentBytes + DataBufferPadBytes));

    memset(pb, 0, sizeof(ndis_mini_buf_t));
    buff = (void *) ((A_UINT8 *)pb + sizeof(ndis_mini_buf_t));

    //
        // check buffer alignment
    //
        if ((DWORD)buff & DataBufferAlignmentMask)
    {
        //
            // not aligned, adjust buffer
        //
            (DWORD)buff += DataBufferAlignmentBytes;
            buff = (PUCHAR)((DWORD)buff & ~DataBufferAlignmentMask);
        }

    pb->buf = buff;
    pb->buf_len = len + AR6000_DATA_OFFSET;

    // Reserve headroom
    pb->buf_dat = pb->buf + AR6000_DATA_OFFSET;
    pb->buf_dat_len = 0;
    return pb;
}

void
a_netbuf_put(void *osbuf, A_UINT32 len)
{
    ndis_mini_buf_t *pb = osbuf;

    pb->buf_dat_len += len;
    if((pb->buf_dat + pb->buf_dat_len) > (pb->buf + pb->buf_len)) {
        DEBUGCHK(0);
    }
    return;
}

A_STATUS
a_netbuf_pull(void *osbuf, A_UINT32 len)
{
    ndis_mini_buf_t *pb = osbuf;
    if ((pb->buf_dat_len - len) < 0)
        return A_ERROR;

    pb->buf_dat_len -= len;
    pb->buf_dat += len;

    return A_OK;
}

A_UINT8 *
a_netbuf_to_data(void *osbuf)
{
    ndis_mini_buf_t *pb = osbuf;
    return pb->buf_dat;
}

A_UINT32
a_netbuf_to_len (void *osbuf)
{
    ndis_mini_buf_t *pb = osbuf;

    return pb->buf_dat_len;
}

A_STATUS
a_netbuf_push(void *osbuf, A_UINT32 len)
{
    ndis_mini_buf_t *pb = osbuf;

    if( pb->buf_dat < pb->buf) {
        return A_ERROR;
    }
    pb->buf_dat -= len;
    pb->buf_dat_len += len;

    return A_OK;
}

void
a_netbuf_free(void *osbuf)
{
    ndis_mini_buf_t *pb = osbuf;

        /* check if the buffer needs to be returned somewhere else */
    if (pb->ReturnBufferHandler != NULL) {
        NDIS_BUF_RETURN_HANDLER handler = pb->ReturnBufferHandler;

        pb->ReturnBufferHandler = NULL;
            /* call return buffer handler */
        handler(pb->pContext,pb);
    } else {
        A_FREE(pb);
	//RETAILMSG(1, (TEXT("builderd:ar6k2:a_netbuf_free = %x\r\n"), (A_UINT32)pb));
    }
}

A_UINT32
a_netbuf_headroom(void *osbuf)
{
    ndis_mini_buf_t *pb = osbuf;

    if (pb == NULL) {
        return 0;
    }
    return (pb->buf_dat - pb->buf);
}

void
a_netbuf_reset(void *osbuf)
{
    ndis_mini_buf_t *pb = osbuf;

    pb->buf_dat = pb->buf + AR6000_DATA_OFFSET;
    pb->buf_dat_len = 0;

    return;
}
