#ifndef _U_BMI_H_
#define _U_BMI_H_
/*
 * Copyright (c) 2008Atheros Communications Inc.
 * All rights reserved.
 *
 * 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
 *
 * simplified BMI declarations and prototypes
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "athdefs.h"
#include "a_types.h"
#include "u_hif.h"
#include "bmi_msg.h"

void BMIInit(void);

A_STATUS 
BMIDone(HIF_DEVICE *device);

A_STATUS 
BMIGetTargetInfo(HIF_DEVICE *device, struct bmi_target_info *targ_info);

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

A_STATUS 
BMIrompatchInstall(HIF_DEVICE *device, 
                   A_UINT32 ROM_addr, 
                   A_UINT32 RAM_addr,
                   A_UINT32 nbytes,
                   A_UINT32 do_activate,
                   A_UINT32 *patch_id);

A_STATUS 
BMIrompatchUninstall(HIF_DEVICE *device,
                     A_UINT32 rompatch_id);

A_STATUS 
BMIrompatchActivate(HIF_DEVICE *device, 
                    A_UINT32 rompatch_count,
                    A_UINT32 *rompatch_list);

A_STATUS 
BMIrompatchDeactivate(HIF_DEVICE *device, 
                      A_UINT32 rompatch_count,
                      A_UINT32 *rompatch_list);

A_STATUS
BMILZStreamStart(HIF_DEVICE *device,
                 A_UINT32 address);

A_STATUS
BMILZData(HIF_DEVICE *device,
          A_UCHAR *buffer, 
          A_UINT32 length);

#ifdef __cplusplus
}
#endif

#endif
