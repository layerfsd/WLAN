//------------------------------------------------------------------------------
// <copyright file="wlanver.h" company="Atheros">
//    Copyright (c) 2008 Atheros Corporation.  All rights reserved.
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
//  Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef _WLANVERS_H_
#define _WLANVERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Global versioning information for Atheros driver/application builds */

/*
 * The version number is made up of major, minor, patch and build
 * numbers.  The build and release script will set the build number
 * using a Perforce counter.  Here the build number is set to 9999 so
 * that builds done without the build-release script are easily
 * identifiable.
 */

#define ATH_SW_VER_MAJOR      1
#define ATH_SW_VER_MINOR      0
#define ATH_SW_VER_PATCH      0
#define ATH_SW_VER_BUILD      99

/*CopyRight (c) year information.*/
#define ATH_SW_COPYRIGHT_CURRENT_YEAR           "Copyright (C) 2008 Atheros Communications, Inc."
#define ATH_SW_COPYRIGHT_CURRENT_YEAR_DRIVER    "2008 Atheros Communications, Inc. All Rights Reserved."

#define ATH_SW_COMPANY_NAME   "Atheros Communications, Inc."
#define ATH_SW_PRODUCT_NAME   "Driver for Atheros AR6002 Network Adapter"
#define ATH_SW_FILE_DESC      "Atheros Extensible Wireless LAN device driver"

#define ATH_SW_FILE_NAME             "ATHR.SYS"
#define ATH_SW_ORIGINALFILE_NAME     "ATHR.SYS"

/*
 * Since no expansion is done when the "stringification" operator is specified,
 * we have to use two macros to do what we we want (to put quotes around our
 * defined constants, above).  The argument of ATH_XSTR() is expanded at prescan
 * to be ATH_STR(<some_number>) which is expanded to a number and then fed into
 * ATH_STR() to be stringified.  Gotta love precompilers.
 */
#define ATH_STR(x)            #x
#define ATH_XSTR(x)           ATH_STR(x)

#define ATHEROS_SW_MAJOR      ATH_XSTR(ATH_SW_VER_MAJOR)
#define ATHEROS_SW_MINOR      ATH_XSTR(ATH_SW_VER_MINOR)
#define ATHEROS_SW_PATCH      ATH_XSTR(ATH_SW_VER_PATCH)
#define ATHEROS_SW_BUILD      ATH_XSTR(ATH_SW_VER_BUILD)

#define ATHEROS_SW_VERSION    ATHEROS_SW_MAJOR "." ATHEROS_SW_MINOR "." \
                              ATHEROS_SW_PATCH "." ATHEROS_SW_BUILD

/* The following two definitions are used in NDIS *.rc files */
#define ATH_VERS_STRING       ATHEROS_SW_VERSION "\0"
#define ATH_VERS_NUMERIC      ATH_SW_VER_MAJOR, ATH_SW_VER_MINOR, \
                              ATH_SW_VER_PATCH, ATH_SW_VER_BUILD

#ifdef __cplusplus
}
#endif

#endif /* _WLANVERS_H_ */
