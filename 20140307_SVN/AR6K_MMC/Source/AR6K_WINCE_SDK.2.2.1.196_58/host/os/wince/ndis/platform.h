//------------------------------------------------------------------------------
// <copyright file="platform.h" company="Atheros">
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
// 
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
//------------------------------------------------------------------------------
//==============================================================================
// Platform power mgmt functions.
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

void plat_init();
void plat_exit();
void plat_suspend();
void plat_resume();

#ifdef __cplusplus
}
#endif

#endif
