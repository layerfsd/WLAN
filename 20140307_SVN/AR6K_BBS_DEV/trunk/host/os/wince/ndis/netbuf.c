//------------------------------------------------------------------------------
// <copyright file="netbuf.c" company="Atheros and Microsoft">
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
// Net Buffer utility function library
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================

#include <ndis.h>
#include "bmi.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
#include "htc_packet.h"
#include "htc.h"
#include "dbglog_api.h"


DWORD DataBufferAlignmentBytes	= 0x20;
DWORD DataBufferAlignmentMask	= 0x1F;


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

	pb = A_MALLOC (sizeof(ndis_mini_buf_t)	+ \
					len + AR6000_DATA_OFFSET + sizeof(HTC_PACKET) + \
					DataBufferAlignmentBytes + DataBufferPadBytes);

	
	if (pb == NULL) {
		A_FREE(buff);
		return NULL;
	}
	
	memset(pb, 0, sizeof(ndis_mini_buf_t));    
	buff = (void *) ((A_UINT8 *)pb + sizeof(ndis_mini_buf_t));

	if ((DWORD)buff & DataBufferAlignmentMask)	  
	{ 	   
		//			 
		// not aligned, adjust buffer		  
		//			
		(DWORD)buff += DataBufferAlignmentBytes;			
		buff = (PUCHAR)((DWORD)buff & ~DataBufferAlignmentMask);		
	}	 

	pb->buf = buff;	
	pb->buf_len = len + AR6000_DATA_OFFSET + + sizeof(HTC_PACKET);    

	// Reserve headroom	  
	pb->buf_dat = pb->buf + AR6000_DATA_OFFSET + sizeof(HTC_PACKET);	 
	pb->buf_dat_len = 0;
	pb->head = pb->buf;

	return pb;
}



/*
 * Allocate an SKB w.o. any encapsulation requirement.
 */
void *
a_netbuf_alloc_raw(int len)
{
	void            *buff = NULL;
	ndis_mini_buf_t *pb = NULL;


	buff = A_MALLOC(len);
	if (buff == NULL) {
		return NULL;
	}
	pb = A_MALLOC(sizeof(ndis_mini_buf_t));
	if (pb == NULL) {
		A_FREE(buff);
		return NULL;
	}

	memset(pb, 0, sizeof(ndis_mini_buf_t));
	pb->buf = buff;
	pb->buf_len = len;
	// Reserve headroom
	pb->buf_dat = pb->buf;
	pb->buf_dat_len = 0;

	pb->head = pb->buf;
	
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

	pb->buf_dat += len;
	pb->buf_dat_len -= len;
	

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

	A_FREE(pb);
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
