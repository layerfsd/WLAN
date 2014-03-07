//------------------------------------------------------------------------------
// <copyright file="host_version.h" company="Atheros">
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
// This file contains the host driver version information 
//
// Author(s): ="Atheros"
//==============================================================================
 

#ifndef _HOST_VERSION_H_
#define _HOST_VERSION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <AR6000/AR6000_version.h>

/*
 * The version number is made up of major, minor, patch and build
 * numbers. These are 16 bit numbers.  The build and release script will
 * set the build number using a Perforce counter.  Here the build number is
 * set to 9999 so that builds done without the build-release script are easily
 * identifiable.
 */

#define ATH_SW_VER_MAJOR      __VER_MAJOR_
#define ATH_SW_VER_MINOR      __VER_MINOR_
#define ATH_SW_VER_PATCH      0
#define ATH_SW_VER_BUILD      __BUILD_NUMBER_

#ifdef __cplusplus
}
#endif

#endif /* _HOST_VERSION_H_ */
