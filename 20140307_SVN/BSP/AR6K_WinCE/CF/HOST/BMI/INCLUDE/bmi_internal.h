//------------------------------------------------------------------------------
// <copyright file="bmi_internal.h" company="Atheros">
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
// BMI APIs 
//
// Author(s): ="Atheros"
//=============================================================================

#include "athdefs.h"
#include "athtypes.h"
#include "osapi.h"
#include "AR6000/hw/mbox_host_reg.h"
#include "AR6000/AR6000_bmi.h"

#define BMI_COMMUNICATION_TIMEOUT       1000

A_STATUS 
bmiBufferSend(HIF_DEVICE *device, 
              A_UCHAR *buffer, 
              A_UINT32 length);

A_STATUS 
bmiBufferReceive(HIF_DEVICE *device, 
                 A_UCHAR *buffer, 
                 A_UINT32 length);
