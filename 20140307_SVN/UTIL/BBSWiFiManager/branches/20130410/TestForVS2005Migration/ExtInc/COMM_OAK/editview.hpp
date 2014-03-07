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
#ifndef __EDITVIEW_HPP_INCLUDED__
#define __EDITVIEW_HPP_INCLUDED__
/*


Abstract:

This header file defines the interface to the GWE component "edctlview".
This component implements the customizable portions of the editcontrol user interface. 

The "edctlview" component works in conjunction with the "edctl" component.

*/

class EditView_t
{
public:
    static
    HBRUSH
    BackgroundBrush(
        HWND hwnd,    // [in]
        HDC hdc       // [in]
        );

    static
    void
    DrawBackground(
        HWND hwnd,    // [in]
        HDC hdc,      // [in]
        RECT* prc     // [in]
        );

    static
    void
    DrawSelection(
        HDC hdc,      // [in]
        RECT* prc,    // [in]
        COLORREF* pcrOldBackgroundColor,  // [out]
        COLORREF* pcrOldTextColor   // [out]
        );

    static
    COLORREF
    TextColor(
        HWND hwnd,    // [in]
        HDC hdc       // [in]
        );
};


#endif /* __EDITVIEW_HPP_INCLUDED__ */
