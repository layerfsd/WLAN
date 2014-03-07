//------------------------------------------------------------------------------
// <copyright file="ar6000_cf_internal.h" company="Atheros">
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
// AR6000 HIF CF internal definitions
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef __AR6000_INTERNAL_H__
#define __AR6000_INTERNAL_H__

#include <ndis.h>
#include "athtypes.h"

typedef struct cf_device{
    NDIS_HANDLE                miniportHandle;
    NDIS_HANDLE                wrapperConfigurationContext;
    A_UINT32                   mappedMemBase;
    A_UINT32                   memLen;
    A_UINT32                   deviceMemBase;
    A_UINT32                   sysIntr; 
    A_UINT32                   interruptNumber; 
    NDIS_MINIPORT_INTERRUPT    interruptObject;
    A_BOOL                     interruptRegistered;
    A_BOOL                     ndisMapped;
} CF_DEVICE;


#endif //__AR6000_INTERNAL_H__
    
