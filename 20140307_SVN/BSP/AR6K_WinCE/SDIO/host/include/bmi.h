//------------------------------------------------------------------------------
// <copyright file="bmi.h" company="Atheros">
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
// BMI declarations and prototypes
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _BMI_H_
#define _BMI_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Header files */
#include "athdefs.h"
#include "athtypes.h"
#include "hif.h"
#include "osapi.h"

void
BMIInit(void);

A_STATUS 
BMIDone(HIF_DEVICE *device);

A_STATUS 
BMIGetTargetId(HIF_DEVICE *device, A_UINT32 *id);

A_STATUS 
BMIReadMemory(HIF_DEVICE *device, 
              A_UINT32 address, 
              A_UCHAR *buffer, 
              A_UINT32 length);

A_STATUS 
BMIWriteMemory(HIF_DEVICE *device, 
               A_UINT32 address, 
               A_UCHAR *buffer, 
               A_UINT32 length);

A_STATUS 
BMIExecute(HIF_DEVICE *device, 
           A_UINT32 address, 
           A_UINT32 *param);

A_STATUS 
BMISetAppStart(HIF_DEVICE *device, 
               A_UINT32 address);

A_STATUS 
BMIReadSOCRegister(HIF_DEVICE *device, 
                   A_UINT32 address, 
                   A_UINT32 *param);

A_STATUS 
BMIWriteSOCRegister(HIF_DEVICE *device, 
                    A_UINT32 address, 
                    A_UINT32 param);

#ifdef __cplusplus
}
#endif

#endif /* _BMI_H_ */
