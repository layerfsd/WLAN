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
//  File:  SBL.c
//
//  Secure Boot Loader - verify the digital signature of a data packet
//

#ifndef _H_SBL
#define _H_SBL

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>

/////////////////////////////////////////////////
// Constants
//

#define SBL_FLAG_NORMAL         0x1
#define SBL_FLAG_END_FILE       0x2
#define SBL_FLAG_CHUNK_HEADER   0x4
#define SBL_FLAG_CHUNK          0x8

#define RANDOM_SEED_LENGTH      16
#define SBL_PACKET_HEADER_SIZE  9



/////////////////////////////////////////////////
// Structures
//

typedef struct _PUBLICKEYDATA {
    const BYTE **   rgpbPublicKeys;         // pointer to array of public keys
    const DWORD *   rgdwKeyLengths;         // pointer to array of public key lengths
    WORD            wNumPublicKeys;         // number of public keys in rgpbPublicKeys
    WORD            wMinSearchIndex;        // starting array index of search
    WORD            wMaxSearchIndex;        // ending array index of search
} PUBLICKEYDATA;

typedef struct _PACKETDATA {
    const BYTE * pbData;                    // data to verify
    DWORD dwDataLength;                     // length of data in bytes
    const BYTE * pbSig;                     // signature to verify
    DWORD dwSigLength;                      // length of signature in bytes
    DWORD dwRecAddress;                     // record address
    DWORD dwRecLength;                      // record length
    DWORD dwRecCheck;                       // record checksum
    BYTE  bRandomSeed[RANDOM_SEED_LENGTH];  // The global file random seed
    DWORD dwSequenceNumber;                 // The packet sequence number
    BYTE  bFlags;                           // Flags (EOF, Chunk, etc.)
} PACKETDATA;

/////////////////////////////////////////////////
// SBL_VerifyPacket
//
// Purpose: Attempts to verify the authenticity of the signature attached
// to a packet of data provided by the calling application. Returns
// S_OK on success, otherwise failure.
//
// Note: This method is used for verification of raw and .bin image files.
// For .bin images set RecAddress, RecLength, and RecCheck to their appropriate values.
// For raw images set RecAddress, RecLength, and RecCheck to 0.

HRESULT SBL_VerifyPacket(const PACKETDATA    * pPacketData, // PACKETDATA used in verification
                         const PUBLICKEYDATA * pKeyData     // PUBLICKEYDATA used in verification
                         );   

#ifdef __cplusplus
}
#endif

#endif
