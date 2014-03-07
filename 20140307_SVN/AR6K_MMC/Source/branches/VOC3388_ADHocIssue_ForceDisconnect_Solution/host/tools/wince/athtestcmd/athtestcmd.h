//------------------------------------------------------------------------------
// <copyright file="athtestcmd.h" company="Atheros">
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
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
// Author(s): ="Atheros"
//==============================================================================
#ifndef _TCMD_H_
#define _TCMD_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
    TCMD_CONT_TX=801,     /* something that doesn't collide with ascii */
    TCMD_CONT_RX,
    TCMD_PM
};

#ifdef __cplusplus
}
#endif

#endif /* _TCMD_H_ */
