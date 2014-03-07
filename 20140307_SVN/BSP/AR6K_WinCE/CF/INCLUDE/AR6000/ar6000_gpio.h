//------------------------------------------------------------------------------
// <copyright file="AR6000_gpio.h" company="Atheros">
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
// AR6000 GPIO
//
// Author(s): ="Atheros"
//==============================================================================
#define GPIO_PIN_COUNT 18

/*
 * Possible values for WMIX_GPIO_SET_REGISTER_CMDID.
 * NB: These match hardware order, so that addresses can
 * easily be computed.
 */
#define GPIO_ID_OUT             0x00000000
#define GPIO_ID_OUT_W1TS        0x00000001
#define GPIO_ID_OUT_W1TC        0x00000002
#define GPIO_ID_ENABLE          0x00000003
#define GPIO_ID_ENABLE_W1TS     0x00000004
#define GPIO_ID_ENABLE_W1TC     0x00000005
#define GPIO_ID_IN              0x00000006
#define GPIO_ID_STATUS          0x00000007
#define GPIO_ID_STATUS_W1TS     0x00000008
#define GPIO_ID_STATUS_W1TC     0x00000009
#define GPIO_ID_PIN0            0x0000000a
#define GPIO_ID_PIN(n)          (GPIO_ID_PIN0+(n))

#define GPIO_LAST_REGISTER_ID   GPIO_ID_PIN(17)
#define GPIO_ID_NONE            0xffffffff
