//------------------------------------------------------------------------------
// <copyright file="floodtest.h" company="Atheros">
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
// Floodtest Application Message Interface
//
// Author(s): ="Atheros"
//==============================================================================

#define FLOOD_TX 0 /* From Host to Target */
#define FLOOD_RX 1 /* From Target to Host */

struct floodtest_control_s {
    A_UINT32 direction;        /* FLOOD_RX or FLOOD_TX */
    A_UINT32 message_size;     /* in bytes, for SEND/RX only */
    A_UINT32 duration;         /* in seconds, for SEND/RX only */
};

