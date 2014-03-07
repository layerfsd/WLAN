//------------------------------------------------------------------------------
// <copyright file="version.h" company="Atheros and Microsoft">
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
// Version information
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#ifndef _VERSION_H
#define _VERSION_H

#include "host_version.h"

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

/*CopyRight (c) year information.*/
#define ATH_SW_COPYRIGHT   "Copyright c 2001-2004 Atheros Communications, Inc."

/*
// Used to add version information to driver file header.
*/
#undef VER_COMPANYNAME_STR
#define VER_COMPANYNAME_STR         "Atheros Communications Inc."
#define VER_FILEDESCRIPTION_STR     "NDIS Miniport driver for 32bit Windows"
#define VER_FILEVERSION_STR         ATH_VERS_STRING
#define VER_INTERNALNAME_STR        "ar6k_ndis.dll"
#define VER_LEGALCOPYRIGHT_STR      ATH_SW_COPYRIGHT
#define VER_ORIGINALFILENAME_STR    VER_INTERNALNAME_STR
#undef  VER_PRODUCTNAME_STR
#define VER_PRODUCTNAME_STR         "Atheros AR6000 WirelessLAN Adapter"

#endif

