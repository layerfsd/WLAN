//------------------------------------------------------------------------------
// <copyright file="wlan_utils.c" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// This module implements frequently used wlan utilies
//
// Author(s): ="Atheros"
//=============================================================================

#include <athdefs.h>
#include <athtypes.h>
#include <osapi.h>

/*
 * converts ieee channel number to frequency
 */
A_UINT16
wlan_ieee2freq(int chan)
{
    if (chan == 14) {
        return 2484;
    }
    if (chan < 14) {    /* 0-13 */
        return (2407 + (chan*5));
    }
    if (chan < 27) {    /* 15-26 */
        return (2512 + ((chan-15)*20));
    }
    return (5000 + (chan*5));
}

/*
 * Converts MHz frequency to IEEE channel number.
 */
A_UINT32
wlan_freq2ieee(A_UINT16 freq)
{
    if (freq == 2484)
        return 14;
    if (freq < 2484)
        return (freq - 2407) / 5;
    if (freq < 5000)
        return 15 + ((freq - 2512) / 20);
    return (freq - 5000) / 5;
}
