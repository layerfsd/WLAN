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
/*
* Secure Loader - Verify signatures for signed images.
*
*/

#ifndef __SLVERIFY_H_
#define __SLVERIFY_H_

#include <mincrypt.h>

typedef struct PUBLICKEYDATA PUBLICKEYDATA;
struct PUBLICKEYDATA {
      BYTE **   rgpbPublicKeys;     // pointer to array of public keys
      DWORD *   rgdwKeyLengths;     // pointer to array of public key lengths
      DWORD     dwNumPublicKeys;     // number of public keys in rgpbPublicKeys
      WORD      wMinSearchIndex;    // starting array index of search
      WORD      wMaxSearchIndex;    // ending array index of search
};

/////////////////////////////////////////////////
// VerifyBinPacket
// Used to sign verify a .bin record
// Parameters:
// [IN] pbData    - data to verify
// [IN] cbDataLen - length of data
// [IN] pbSig     - signature corresponding to pbData
// [IN] cbSigLen  - length of pbSig
// [IN] dwRecAddress - record address.  used in signature verification. 
// [IN] pKeyData     - PUBLICKEYDATA structure which has public keys used for signature verification
// [IN/OUT] pwLastUsedKeyIndx - [OUT] if not NULL set to the key index in pKeyData that successfully verified the
//                      signature in last call to this function.
//                      [IN] when a non-NULL value is passed in and it falls between pKeyData->wMinSearchIndex
//                      & pKeyData->wMaxSearchIndex (both inclusive), the public key correspoding to this
//                      index in pKeyData->rgpbPublicKeys is used.
//                      (This parameter is for optimization so that we don't have to search through all the
//                       public keys each time the function is called)


HRESULT VerifyBinPacket(const BYTE * pbData, DWORD cbDataLen, 
                     const BYTE * pbSig,  DWORD cbSigLen,
                     DWORD dwRecAddress, DWORD dwRecLen, DWORD dwRecChkSum,  
                     const PUBLICKEYDATA * pKeyData, WORD* pwLastUsedKeyIndx);

/////////////////////////////////////////////////
// VerifyRawPacket
// Used to sign verify a raw image packet
// Parameters:
// [IN] pbData    - data to verify
// [IN] cbDataLen - length of data
// [IN] pbSig     - signature corresponding to pbData
// [IN] cbSigLen  - length of pbSig
// [IN] pKeyData     - PUBLICKEYDATA structure which has public keys used for signature verification
// [IN/OUT] pwLastUsedKeyIndx - [OUT] if not NULL set to the key index in pKeyData that successfully verified the
//                      signature in last call to this function.
//                      [IN] when a non-NULL value is passed in and it falls between pKeyData->wMinSearchIndex
//                      & pKeyData->wMaxSearchIndex (both inclusive), the public key correspoding to this
//                      index in pKeyData->rgpbPublicKeys is used.
//                      (This parameter is for optimization so that we don't have to search through all the
//                       public keys each time the function is called)

HRESULT VerifyRawPacket(const BYTE * pbData, DWORD cbDataLen, 
                     const BYTE * pbSig,  DWORD cbSigLen,
                     const PUBLICKEYDATA * pKeyData, WORD* pwLastUsedKeyIndx);

#endif
