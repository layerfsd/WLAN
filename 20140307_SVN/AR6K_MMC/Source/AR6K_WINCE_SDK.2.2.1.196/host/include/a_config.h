//------------------------------------------------------------------------------
// <copyright file="a_config.h" company="Atheros">
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
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
// This file contains software configuration options that enables
// specific software "features"
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef _A_CONFIG_H_
#define _A_CONFIG_H_

#ifdef UNDER_NWIFI
#include "../os/windows/common/include/config_wince.h"
#endif

#ifdef ATHR_CE_LEGACY
#include "../os/wince/include/config_wince.h"
#endif

#ifdef  __linux__
#if !defined(LINUX_EMULATION)
#include "../os/linux/include/config_linux.h"
#endif
#endif

#ifdef REXOS
#include "../os/rexos/include/common/config_rexos.h"
#endif

#endif
