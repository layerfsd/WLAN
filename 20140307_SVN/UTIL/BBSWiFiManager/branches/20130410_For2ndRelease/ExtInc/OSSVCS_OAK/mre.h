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
/*****************************************************************************

    File: MRE.h
    Purpose: Helper function for setting some statstor entries to the most
    recent event.  This is used to change the right softkey on the smartphone
    to be that event.
            
    Author(s): KK

    History:
        Created - KK 1-22-1

*/

const int MRE_EVENT_OVER    = 0x00000001; // The event described in the structure is over.  If it is not the current event, this call is ignored.
const int MRE_OVERWRITE     = 0x00000002; // Ignores the priority of the existing event and overwrites it.
const int MRE_IGNORE_PARAM  = 0x00000004; // Used with MRE_EVENT_OVER. The event that is over is compared to the current event ignoring the
                                          // parameters in the SHELLEXECUTEINFO of those events.

// iPriority
// Only events with a higner priority can replace events of a lower priority.
// Events of equal priority can replace each other.
// Generally, you should be pri=0 unless you are a special system event.

const int MREPriContacts = 0;
const int MREPriNotification = 200;
const int MREPriMissedCall = 300;
const int MREPriCallProgress = 300;

struct MREINFO
{
    UINT cbSize;
    DWORD dwFlags;
    int iPriority;
    DWORD tDontOverwrite; // Don't overwrite this event until (GetTickCount() >= tDontOverwrite).  Ignored if 0.
    TCHAR* pszLabel;
    SHELLEXECUTEINFO* psei;
};

HRESULT MRESetSoftkeyEvent(const MREINFO* pmi);

// Call this when statstor tells you that SDID_MRE changed.
HRESULT MREGetSoftkeyEvent(MREINFO** ppmi);


