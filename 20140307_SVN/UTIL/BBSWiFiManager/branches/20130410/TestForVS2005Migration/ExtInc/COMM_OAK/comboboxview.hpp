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
#ifndef __COMBOBOXVIEW_HPP_INCLUDED__
#define __COMBOBOXVIEW_HPP_INCLUDED__
/*


Abstract:

This header file defines the interface to the GWE component "cmbctlview".
This component implements the customizable portions of the combobox user interface. 

The "cmbctlview" component works in conjunction with the "cmbctl" component.

*/

class ComboBoxView_t
{
public:

    static
    HBRUSH
    BackgroundBrush(
        HWND hwnd,      // [in]
        HWND hwndEdit,  // [in]
        HDC hdc         // [in]
        );

    static
    void
    DrawBackground(
        HWND hwnd,      // [in]
        HWND hwndEdit,  // [in]
        HDC hdc,        // [in]
        RECT* prc       // [in]
        );

    static
    void
    DrawFocus(
        HDC hdc,        // [in]
        RECT *prc       // [in]
        );

    static
    void
    DrawSelection(
        HDC hdc,        // [in]
        RECT *prc       // [in]
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
        RECT *lprc      // [in]
        );

    static
    void
    WindowStyleCustomization(
        HWND hwnd,      // [in]
        DWORD Style,    // [in]
        DWORD ExStyle,  // [in]
        DWORD *pStyle,  // [out]
        DWORD *pExStyle // [out]
        );
};

#endif /* __COMBOBOXVIEW_HPP_INCLUDED__ */
