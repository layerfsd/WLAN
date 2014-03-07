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
/*****************************************************************************\
*                                                                             *
* initguid.h -  Definitions for controlling GUID initialization               *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*                                                                             *
\*****************************************************************************/

// Include after compobj.h to enable GUID initialization.  This
//              must be done once per exe/dll.
//
// After this file, include one or more of the GUID definition files.
//
// NOTE: ole2.lib contains references to all GUIDs defined by OLE.

#ifndef DEFINE_GUID
#error initguid: must include objbase.h first.
#endif

#undef DEFINE_GUID

#ifdef _MAC
#define __based(a)
#endif

#ifdef _WIN32
#define __based(a)
#endif

#ifdef __TURBOC__
#define __based(a)
#endif

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID __based(__segname("_CODE")) name \
                    = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
