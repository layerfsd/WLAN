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
#ifndef _LAYMGR_H_
#define _LAYMGR_H_


#include <keybddbg.h>

// wPddMask bit masks
#define PS2_AT_PDD  0x0002
#define PS2_NOP_PDD 0x0004

// 0x0100, 0x0200, 0x0400, and 0x0800 are reserved for OEM use.


#ifndef dim
#define dim(x) (sizeof(x)/sizeof(x[0]))
#endif


#endif // _LAYMGR_H_
