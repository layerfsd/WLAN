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
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

l2tpcfg.h

Abstract:  

Functions:


Notes: 
    The netui library cannot be loaded before GWE has initialized (since it
    initializes common controls on DLL entry).  So, check that the WMGR API
    set has been registered before loading netui.dll.

--*/

#ifndef _L2TPCFG_H_
#define _L2TPCFG_H_
// @CESYSGEN IF CE_MODULES_PPP


#ifdef __cplusplus
extern "C" {
#endif

// 
// values for dwAuthType below
//
#define L2TP_IPSEC_AUTH_SIGNATURE            0x1
#define L2TP_IPSEC_AUTH_PRESHAREDKEY         0x2

#define CERT_HASH_SIZE          20

typedef struct _L2TP_CONFIG_DATA
{
    DWORD   dwVersion;      // should be 1
    DWORD   dwAuthType;     // see L2TP_IPSEC_AUTH_ types above
    DWORD   dwFlags;        // none defined yet. 
    DWORD   cbKey;          // size of pre-shared key
    DWORD   dwOffsetKey;    // offset of pre-shared key from start of struct
    DWORD   cMyCerts;         // number of client  certificates (MY store)
    DWORD   cRootCerts;     // number root certificates (ROOT store)
    DWORD   dwOffsetCertHashes; // offset to start of array of certificate SHA hashes
    // Certificate hash array has the client cert hashess (if any) followed by the root cert hashes (if any):
    // BYTE CertHashes[CERT_HASH_SIZE][cMyCerts + cRootCerts];
    // If there are no certificates supplied, default rules will apply.
    
} L2TP_CONFIG_DATA, *PL2TP_CONFIG_DATA;

#ifdef __cplusplus
}
#endif
// @CESYSGEN ENDIF
#endif //_L2TPCFG_H_

