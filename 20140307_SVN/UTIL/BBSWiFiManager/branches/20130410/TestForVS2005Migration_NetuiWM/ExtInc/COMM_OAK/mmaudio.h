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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

mmaudio.h

--*/
#ifndef _INC_MMAUDIO
#define _INC_MMAUDIO   // to prevent multiple inclusion of mmaudio.h

#include "pshpack1.h"   // Assume byte packing throughout

#ifdef __cplusplus
extern "C" {            // Assume C declarations for C++
#endif  // __cplusplus



// -----------------------------------------------------------------------------
//                              FileHeader
// -----------------------------------------------------------------------------
typedef struct
{
   DWORD   dwRiff;     // Type of file header.
   DWORD   dwSize;     // Size of file header.
   DWORD   dwWave;     // Type of wave.
} RIFF_FILEHEADER, *PRIFF_FILEHEADER;



// -----------------------------------------------------------------------------
//                              ChunkHeader
// -----------------------------------------------------------------------------
typedef struct
{
   DWORD   dwCKID;        // Type Identification for current chunk header.
   DWORD   dwSize;        // Size of current chunk header.
} RIFF_CHUNKHEADER, *PRIFF_CHUNKHEADER;
//typedef ChunkHeader UNALIGNED *FPChunkHeader;



/*  Chunk Types  */
#define RIFF_FILE       mmioFOURCC('R','I','F','F')
#define RIFF_WAVE       mmioFOURCC('W','A','V','E')
#define RIFF_FORMAT     mmioFOURCC('f','m','t',' ')
#define RIFF_CHANNEL    mmioFOURCC('d','a','t','a')


#ifndef mmioFOURCC
#define mmioFOURCC( ch0, ch1, ch2, ch3 )                                \
                ( (DWORD)(BYTE)(ch0) | ( (DWORD)(BYTE)(ch1) << 8 ) |    \
                ( (DWORD)(BYTE)(ch2) << 16 ) | ( (DWORD)(BYTE)(ch3) << 24 ) )
/* This macro is machine byte-order and word-order dependent!! */
/* The characters are BYTES, so compatible with ANSI, not at all with UNICODE */
#endif






#ifdef __cplusplus
}                       // End of extern "C" {
#endif  // __cplusplus

#include "poppack.h"        /* Revert to default packing */

#endif // _INC_MMAUDIO
