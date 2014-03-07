//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//
// Forced include file for preprocessing dll def files.
//

#if defined(MIPS) || defined(SH3)
#define STDAPI(sz,c) sz
#define METHOD(x,y) x##A##y
#else
#if UNDER_CE
#define STDAPI(sz,c) sz
#else
#define STDAPI(sz,c) sz##@##c
#endif
#define METHOD(x,y) x##E##y
#endif

#ifdef UNDER_CE
#include <bldver.h>
#endif