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


Module Name: spseal.h

Purpose: Prototypes for security provider encryption routines.

--*/

#ifndef _SPSEAL_
#define _SPSEAL_

SECURITY_STATUS SEC_ENTRY
SealMessage(    PCtxtHandle         phContext,
                unsigned long       fQOP,
                PSecBufferDesc      pMessage,
                unsigned long       MessageSeqNo);

typedef SECURITY_STATUS
(SEC_ENTRY * SEAL_MESSAGE_FN)(
    PCtxtHandle, unsigned long, PSecBufferDesc, unsigned long);


SECURITY_STATUS SEC_ENTRY
UnsealMessage(  PCtxtHandle         phContext,
                PSecBufferDesc      pMessage,
                unsigned long       MessageSeqNo,
                unsigned long *     pfQOP);


typedef SECURITY_STATUS
(SEC_ENTRY * UNSEAL_MESSAGE_FN)(
    PCtxtHandle, PSecBufferDesc, unsigned long,
    unsigned long *);

#endif // _SPSEAL_
