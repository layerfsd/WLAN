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
// File: vibrateos.h
//
// Purpose: OS-level implementation of API for controlling the vibrator (SDK export is in shell).
//

#pragma once

//
// Overview: A vibration "song" will consist of an array of "vibration notes", each note specifying an 
// amplitude, a frequency, and a duration.
//
// See vibrate.h in shell sdk for complete documentation of this API--these functions should only be called
// via the shell API, and should not be used directly!!!
//

HRESULT XVibrate(DWORD cvn, const VIBRATENOTE * rgvn, BOOL fRepeat, DWORD dwTimeout);

HRESULT XVibrateStop (void);

int XVibrateGetDeviceCaps (VIBRATEDEVICECAPS vdc);
