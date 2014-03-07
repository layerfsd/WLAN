//------------------------------------------------------------------------------
// <copyright file="athtypes.h" company="Atheros">
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
// This file contains the definitions of the basic atheros data types.
// It is used to map the data types in atheros files to a platform specific 
// type.
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _ATHTYPES_H_
#define _ATHTYPES_H_

#ifdef __linux__
#include "../os/linux/include/athtypes_linux.h"
#endif

#ifdef UNDER_CE
#include "../os/wince/include/athtypes_wince.h"
#endif

#endif /* _ATHTYPES_H_ */
