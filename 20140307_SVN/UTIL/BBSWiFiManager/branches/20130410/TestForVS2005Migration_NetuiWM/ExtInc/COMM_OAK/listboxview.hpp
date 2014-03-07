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
#ifndef __LISTBOXVIEW_HPP_INCLUDED__
#define __LISTBOXVIEW_HPP_INCLUDED__
/*


Abstract:

This header file defines the interface to the GWE component "lbctlview".
This component implements the customizable portions of the listbox user interface. 

The "lbctlview" component works in conjunction with the "lbctl" component.

*/

class ListBoxView_t
{
public:
    static
    int
    ItemHeight(
        HWND hwnd,      // [in]
        HDC hdc         // [in]
        );

    static
    void
    DrawBackground(
        HWND hwnd,      // [in]
        HWND hwndOwner, // [in]
        HDC hdc,        // [in]
        RECT* prc       // [in]
        );

    static
    void
    DrawFocus(
        HDC hdc,        // [in]
        RECT* prc,      // [in]
        bool bOn        // [in]
        );

    static
    void
    DrawSelection(
        HDC hdc,        // [in]
        RECT* prc,      // [in]
        COLORREF *pcrOldBackgroundColor,  // [out]
        COLORREF *pcrOldTextColor   // [out]
        );

    static
    COLORREF
    TextColor(
        HWND hwnd,      // [in]
        HDC hdc         // [in]
        );

    static
    int
    TopCoordinate(
        HDC hdc,        // [in]
        RECT* prc,      // [in]
        int ascent      // [in]
        );
};


#endif /* __LISTBOXVIEW_HPP_INCLUDED__ */
