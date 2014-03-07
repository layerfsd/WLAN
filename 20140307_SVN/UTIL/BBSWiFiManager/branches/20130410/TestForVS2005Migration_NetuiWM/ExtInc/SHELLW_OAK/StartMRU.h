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

    File: StartMRU.h
    
    Public interface to the Start MRU object.

    History:
        Created - KK 5-7-1

*/
#pragma once

// Flags for DrawItemIcon
// Bits are shifted over if we ever want to fold in the DCI_ flags in icocache.h
const DWORD DII_LARGE = 0x00010000; // Use the large icon
const DWORD DII_SMALL = 0x00020000; // Use the small icon


class IStartMRU :
    public IUnknown
{
public:
    // Used by clients to draw and run the MRU
    virtual HRESULT STDMETHODCALLTYPE OnMRUDataChange() = 0; // Clients need to be statstore aware and call this function when the SDID_START_MRU changes
    virtual HRESULT STDMETHODCALLTYPE GetItemCount(int *pcItems) = 0;
    virtual HRESULT STDMETHODCALLTYPE DrawItemIcon(int iItem, HDC hdc, const RECT* prcBounds, DWORD dwFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetItemDisplayName(int iItem, TCHAR* pszBuf, int cchBuf) = 0;
    virtual HRESULT STDMETHODCALLTYPE ExecItem(int iItem) = 0;
};

HRESULT SHGetStartMRU(IStartMRU** ppbs);

void SHStartMRUHookProc(const SHELLEXECUTEINFO* pei);


