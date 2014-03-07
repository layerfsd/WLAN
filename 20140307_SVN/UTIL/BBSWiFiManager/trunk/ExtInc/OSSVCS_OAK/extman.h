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
/*****************************************************************************

    File: ExtMan.h
    Purpose: Extension Manager

    History:
        Created - KK 6-13-2

*/

#pragma once

// definitions for dwParams parameter of SendEvent
#define SEP_ARGBITS 4 // count of bits in an SEP arg
#define SEP_ARG_MASK 0xF
#define SEP_FIRSTARG 0xF0000000
#define SEP_FIRSTARG_SHIFT 28

#define SEP_INT 1
#define SEP_UINT 2
#define SEP_STRING 3
#define SEP_BOOL 4
// #define SEP_ - must be less than 16
#define MAKE_SE_PARAMS(a, b, c, d, e, f, g, h) \
    (((a) << 28) | ((b) << 24) | ((c) << 20) | ((d) << 16) | ((e) << 12) | ((f) << 8) | ((g) << 4) | (h))
// To send an event do something like ...->SendEvent(id, MAKE_SE_PARAMS(SEP_INT, SEP_STRING, SEP_UINT,0,0,0,0,0), -5, TEXT("Hello"), 10)


class IExtensionManager :
    public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SendEvent(DISPID diEvent, DWORD dwParams, ...) = 0;
    virtual HRESULT STDMETHODCALLTYPE RefreshExtensions() = 0;
    virtual HRESULT STDMETHODCALLTYPE Shutdown() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetExtension(int iExtension, IUnknown** ppExt) = 0;
};
HRESULT CreateExtensionManager(const TCHAR* pszContext, const TCHAR* pszClass, IUnknown* pUnknownSite, IExtensionManager** ppExtMan);




