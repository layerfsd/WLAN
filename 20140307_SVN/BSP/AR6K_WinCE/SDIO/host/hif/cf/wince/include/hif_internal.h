//------------------------------------------------------------------------------
// <copyright file="hif_internal.h" company="Atheros">
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
// HIF internal definitions
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef _HIF_INTERNAL_H
#define _HIF_INTERNAL_H

#include "ar6000_cf.h"
#include "hif.h"

#define HIF_IS_MBOX_ADDR(addr) (addr >= HIF_MBOX_START_ADDR(0) && addr <= HIF_MBOX_END_ADDR(3)) ? 1:0

typedef struct hif_device {
    CF_DEVICE_HANDLE cfHandle;
} HIF_DEVICE;


#endif
