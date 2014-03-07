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
// File: vibrate.h
//
// Purpose: API for controlling the vibrator.
//

#pragma once

//
// Overview: A vibration "song" will consist of an array of "vibration notes", each note specifying an 
// amplitude, a frequency, and a duration.
//


//
// Definition of a vibrate note.
//
// wDuration is the duration of the note in milliseconds.
// bAmplitude and bFrequency are on a scale of zero to 7, with 0xFF meaning Default.  There are no set 
//    units; the driver can interpret the numbers in whatever units it likes.
//    However, bAmplitude=0 always means "off". 
//
typedef struct
{
    WORD wDuration;
    BYTE bAmplitude;  
    BYTE bFrequency;
} VIBRATENOTE; 

//
// Starts playing the vibration song specified by rgvn.
//
// cvn == number of elements in rgvn
// rgvn == an array of VIBRATENOTEs that define the song
// fRepeat == should the song be repeated (until the timeout expires)
// dwTimeout == the maximum amount of time the song should play, in milliseconds, 
//    including any repetition.  May be INFINITE.
//
// NOTE: If rgvn == NULL, (and therefore cvn should equal 0) that means you want a default amplitude, 
//    a default frequency, and an infinite duration for that note (of course, the dwTimeout parameter 
//    will still stop your song).  So to play a simple vibrate of 2 seconds you do:
// 
//       Vibrate(0, NULL, TRUE/*don't care*/, 2000);
//
// Return values:
//    E_FAIL == failure, probably device not present (use VibrateGetDeviceCaps)
//    E_NOTIMPL == the feature you are attempting to use is not yet implemented
//    S_OK == hurray, everything's great
//
HRESULT Vibrate(DWORD cvn, const VIBRATENOTE * rgvn, BOOL fRepeat, DWORD dwTimeout);

//
// Stops all current songs in this process.
//
// Return values:
//    E_FAIL == failure, probably device not present (use VibrateGetDeviceCaps)
//    S_OK == hurray, everything's great
//
HRESULT VibrateStop (void);

//
// List of vibrator device capabilities.
//
typedef enum 
{
    VDC_AMPLITUDE,
    VDC_FREQUENCY,
    VDC_LAST
} VIBRATEDEVICECAPS;

//
// Returns the number of steps (0 to 7) that the hardware supports.  Zero means the device has no vibrator.  
// One means the vibrator only supports one level. That is, it only supports on/off. 
// An application can always construct its VIBRATENOTEs as if the device supports all 8 levels.  
// The Vibrate function will map the level specified in the note to one of the levels 
// the device actually supports.
//
int VibrateGetDeviceCaps (VIBRATEDEVICECAPS vdc);
