//------------------------------------------------------------------------------
// <copyright file="debug.h" company="Atheros and Microsoft">
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
//    The software is licensed “as-is.” 
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// Debug zones defintions
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================

#ifndef _WINCE_NDIS_DEBUG_H
#define _WINCE_NDIS_DEBUG_H

#include "osapi.h"

#define ZONE_ERROR      ATH_LOG_ERR 
#define ZONE_WARN       ATH_LOG_WARN 
#define ZONE_FUNCTION   ATH_LOG_TRC 
#define ZONE_INIT       ATH_LOG_INF 
#define ZONE_NDIS       ATH_DEBUG_NET 
#define ZONE_RCV        ATH_LOG_RECV 
#define ZONE_XMIT       ATH_LOG_SEND

#endif
