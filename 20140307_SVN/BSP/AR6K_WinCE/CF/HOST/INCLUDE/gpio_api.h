//------------------------------------------------------------------------------
// <copyright file="gpio_api.h" company="Atheros">
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
// Host-side General Purpose I/O API.
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _GPIO_API_H_
#define _GPIO_API_H_

/*
 * Host-side GPIO support is optional.
 * If run-time access to GPIO pins is not required, then
 * this should be changed to 0.
 */
#define CONFIG_HOST_GPIO_SUPPORT 1

/*
 * Host-side General Purpose I/O API.
 *
 * $Id: //depot/sw/branches/kamet_ms_main/host/include/gpio_api.h#2 $
 */

/*
 * Send a command to the Target in order to change output on GPIO pins.
 */
A_STATUS wmi_gpio_output_set(struct wmi_t *wmip,
                             A_UINT32 set_mask,
                             A_UINT32 clear_mask,
                             A_UINT32 enable_mask,
                             A_UINT32 disable_mask);

/*
 * Send a command to the Target requesting input state of GPIO pins.
 */
A_STATUS wmi_gpio_input_get(struct wmi_t *wmip);

/* 
 * Send a command to the Target to change the value of a GPIO register.
 */
A_STATUS wmi_gpio_register_set(struct wmi_t *wmip,
                               A_UINT32 gpioreg_id,
                               A_UINT32 value);

/*
 * Send a command to the Target to fetch the value of a GPIO register.
 */
A_STATUS wmi_gpio_register_get(struct wmi_t *wmip, A_UINT32 gpioreg_id);

/* 
 * Send a command to the Target, acknowledging some GPIO interrupts.
 */
A_STATUS wmi_gpio_intr_ack(struct wmi_t *wmip, A_UINT32 ack_mask);

#endif /* _GPIO_API_H_ */
