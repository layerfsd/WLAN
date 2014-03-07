//------------------------------------------------------------------------------
// <copyright file="athtypes_wince.h" company="Atheros">
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
// This file contains the definitions of the basic atheros data types.
// It is used to map the data types in atheros files to a platform specific type.
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef _ATHTYPES_WINCE_H_
#define _ATHTYPES_WINCE_H_

#include <windows.h>

typedef signed char        A_INT8;
typedef signed short       A_INT16;
typedef signed int         A_INT32;
typedef signed __int64     A_INT64;

typedef unsigned char      A_UINT8;
typedef unsigned short     A_UINT16;
typedef unsigned int       A_UINT32;
typedef unsigned __int64   A_UINT64;

typedef unsigned int       A_BOOL;
typedef char            A_CHAR;
typedef unsigned char   A_UCHAR;
typedef void*           A_ATH_TIMER;

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef PBYTE
typedef unsigned char* PBYTE;
#endif

#endif /* _ATHTYPES_WINCE_H_ */
