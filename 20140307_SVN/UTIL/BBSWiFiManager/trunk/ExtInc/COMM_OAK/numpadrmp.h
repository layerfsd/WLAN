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
#ifndef _NUM_PAD_RMP_H_
#define _NUM_PAD_RMP_H_

#include <windows.h>
#include "DeviceLayout.h"

UINT
WINAPI
NumPadRemapVKey(
    const KEYBD_EVENT *pKbdEvents,
    UINT              cKbdEvents,
    KEYBD_EVENT       *pRmpKbdEvents,
    UINT              cMaxRmpKbdEvents
    );

#endif // NUM_PAD_RMP_H_
