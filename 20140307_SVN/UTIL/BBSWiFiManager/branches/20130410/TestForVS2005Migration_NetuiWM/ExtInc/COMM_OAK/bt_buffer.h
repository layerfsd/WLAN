//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//------------------------------------------------------------------------------
// 
//      Bluetooth Data Buffer
// 
// 
// Module Name:
// 
//      bt_buffer.h
// 
// Abstract:
// 
//      This file defines Bluetooth data buffer format
// 
// 
//------------------------------------------------------------------------------
#if ! defined (__bt_buffer_H__)
#define __bt_buffer_H__		1

typedef void (*BD_BUFFER_FREE) (struct BD_BUFFER *pBuff);

struct BD_BUFFER {
	int				cSize;
	int				cStart;
	int				cEnd;

	BD_BUFFER_FREE	pFree;

	int				fMustCopy;

	unsigned char	*pBuffer;
};

inline int BufferTotal (BD_BUFFER *pB) {
	return pB->cEnd - pB->cStart;
}

inline int BufferGetByte (BD_BUFFER *pB, unsigned char *puc) {
	if (pB->cEnd - pB->cStart < 1)
		return FALSE;

	*puc = pB->pBuffer[pB->cStart++];

	return TRUE;
}

inline int BufferGetShort (BD_BUFFER *pB, unsigned short *pus) {
	if (pB->cEnd - pB->cStart < 2)
		return FALSE;

	*pus = pB->pBuffer[pB->cStart] | (pB->pBuffer[pB->cStart + 1] << 8);
	pB->cStart += 2;

	return TRUE;
}

inline int BufferGetChunk (BD_BUFFER *pB, int cChunk, __out_bcount_opt(cChunk) unsigned char *pChunk) {
	if ((pB->cEnd - pB->cStart < cChunk) || (cChunk <= 0))
		return FALSE;

	memcpy (pChunk, pB->pBuffer + pB->cStart, cChunk);
	pB->cStart += cChunk;

	return TRUE;
}

void BufferFree (BD_BUFFER *pBuf);
BD_BUFFER *BufferAlloc (int cSize);
BD_BUFFER *BufferCopy (BD_BUFFER *pBuffer);
BD_BUFFER *BufferCompress (BD_BUFFER *pBuffer);

#endif	// __bt_buffer_H__

