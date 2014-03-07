/*
 * Copyright (c) 2004-2007 Atheros Communications Inc.
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
 * device configuration APIs
 */

#ifndef _U_CONFIG_H_
#define _U_CONFIG_H_

A_STATUS ar6000_prepare_target(HIF_DEVICE *hifDevice,
                               A_UINT32    TargetType,
                               A_UINT32    TargetVersion);
                                        
A_STATUS DownloadFirmware(HIF_DEVICE *hifDevice,
                          A_UINT32   TargetType,
                          A_UINT32   TargetVersion,
                          A_UCHAR    *pAthwlan,
                          A_UINT32   AthWlanLength,
                          A_BOOL     AthWlanbCompressed,
                          A_UCHAR    *pDataPatch,
                          A_UINT32   DataPatchLength);
                          
A_STATUS ar6000_enable_target_console(HIF_DEVICE *hifDevice, 
                                      A_UINT32    TargetType);                         
                                     

A_STATUS ar6000_set_htc_params(HIF_DEVICE *hifDevice,
                               A_UINT32    TargetType,
                               A_UINT32    MboxIsrYieldValue,
                               A_UINT8     HtcControlBuffers);

A_STATUS eeprom_ar6000_transfer (HIF_DEVICE *device, A_UINT32 TargetType);

A_STATUS ar6000_set_wmi_protocol_ver(HIF_DEVICE *hifDevice, 
                                     A_UINT32    TargetType, 
                                     A_UINT32    WmiProtocolVersion);

A_STATUS ar6000_configure_clock (HIF_DEVICE *hifDevice,
                                 A_UINT32    TargetType,
                                 A_UINT32    TargetVersion,
                                 A_UINT32    clkFreq);
                                                                                                      
#endif
