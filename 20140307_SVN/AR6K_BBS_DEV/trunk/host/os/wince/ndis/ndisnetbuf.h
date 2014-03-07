//------------------------------------------------------------------------------
// <copyright file="ndisnetbuf.h" company="Atheros and Microsoft">
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
// NDIS specific netbuf structures and constants
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================

//#define AR6000_BUFFER_SIZE    1556
#define AR6000_DATA_OFFSET    64

typedef struct ndis_mini_buf_ {
	int				buf_dat_len;
	A_UINT8*		buf_dat;
	A_UINT8*		buf;
	A_UINT8*		head;	
	int				buf_len;
	NDIS_BUFFER		NdisBuffer;
} ndis_mini_buf_t;

