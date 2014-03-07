//------------------------------------------------------------------------------
// <copyright file="athtypes_wince.h" company="Atheros">
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
// This file contains the definitions of the basic atheros data types for wince
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _ATHTYPES_WINCE_H_
#define _ATHTYPES_WINCE_H_

#include <windows.h>

typedef signed char        A_INT8;
typedef signed short       A_INT16;
typedef signed int         A_INT32;
typedef signed long long   A_INT64;

typedef unsigned char      A_UINT8;
typedef unsigned short     A_UINT16;
typedef unsigned int       A_UINT32;
typedef unsigned long long A_UINT64;

typedef BOOL            A_BOOL;
typedef char            A_CHAR;
typedef unsigned char   A_UCHAR;

#endif /* _ATHTYPES_WINCE_H_ */
