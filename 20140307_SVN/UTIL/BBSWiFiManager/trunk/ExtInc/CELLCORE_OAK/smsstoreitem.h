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
//////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999-2000 Microsoft Corporation.  All rights reserved. //
//////////////////////////////////////////////////////////////////////////

#ifndef _SMSSTOREITEM_H_
#define _SMSSTOREITEM_H_


#include <windows.h>
#include <ril.h>
#include <sms.h>

//
//
//
typedef struct smsstoreitem_tag {
    DWORD cbSize;
    RILMESSAGE rmMessage;                        // Message data
    UINT cbMessageUID;                           // Length of message UID (non-zero iff the message is multi-part)
    BYTE rgbMessageUID[SMS_MAX_MESSAGEUID_SIZE]; // Message UID (valid only if cbMessageUID != 0)
    UINT nTotalParts;                            // Total number of parts in the message (valid only if cbMessageUID != 0)
    UINT iPartIndex;                             // Sequence number of this part (valid only if cbMessageUID != 0)
    BOOL fDoNotConcatenate;                      // Override to return segments as individual messages on read
} SMSSTOREITEM, *LPSMSSTOREITEM;

typedef struct smsstoreitemex_tag {
    SMSSTOREITEM ssi;
    DWORD cbSize;
    DWORD dwLocation;
    DWORD dwIndex;
} SMSSTOREITEMEX, *LPSMSSTOREITEMEX;

#define MULTIPART_SMSSTOREITEM(si) (0 != (si).cbMessageUID)

#endif // _SMSSTOREITEM_H_
