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

    File: banner.h
    
    Public interface to the banner object.

    History:
        Created - KK 5/31/1 (who will spend a day in hell for this...)

*/
#pragma once



class IBannerDisplay :
    public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE InitializeFromSAN(const TCHAR* pszAppSAN) = 0;
    virtual HRESULT STDMETHODCALLTYPE InitializeRect(const RECT* prcDisplay, RECT* prcRemaining) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRect(RECT* prc) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnPaint(const PAINTSTRUCT* pps) = 0;
};

HRESULT SHCreateBannerDisplay(IBannerDisplay** ppbd);



