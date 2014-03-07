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
#ifndef __MENUCONTROLVIEW_HPP_INCLUDED__
#define __MENUCONTROLVIEW_HPP_INCLUDED__
/*


Abstract:

This header file defines the interface to the GWE component "menuview".
This component implements the customizable portions of the menu control user interface.

The "menuview" component works in conjunction with the "menu" component.

*/


class MenuView_t
{
protected:

    static const UINT8 s_cxMenuArrowMargin;
    static const UINT8 s_cxMenuRightMargin;
    static const UINT8 s_cxMenuAcceleratorMargin;
    static const UINT8 s_cyMenuVerticalMargin;
    static const UINT8 s_cxMenuNPMnemoMargin;

    static const UINT8 s_cxColumnSeparator;
    static const UINT8 s_cyColumnSeparatorMargin;

public:

    static
    int
    CascadeArrowWidth(
        void
        );

    static
    void
    ColumnComputeWidth(
        long& xColumn,              // [in/out]
        long cxMaxString,           // [in]
        long cxMaxAccel,            // [in]
        long* pcxTotal,             // [out]
        long* pxAccel               // [out]
        );

    static
    void
    DrawCascadeArrow(
        HDC hdc,                    // [in]
        long x,                     // [in]
        long y,                     // [in]
        long cx,                    // [in]
        long cy,                    // [in]
        UINT menustate,             // [in]
        BOOL bRightToLeft           // [in]
        );

    static
    void
    DrawCheckmark(
        HDC hdc,                    // [in]
        long x,                     // [in]
        long y,                     // [in]
        UINT menustate              // [in]
        );

    static
    void
    DrawColumnBreak(
        HDC hdc,                    // [in]
        long x,                     // [in]
        long y,                     // [in]
        long cx,                    // [in]
        long cy,                    // [in]
        long cxMenu,                // [in]
        long cyMenu,                // [in]
        UINT menustate              // [in]
        );

    static
    void
    DrawScrollArrows(
        HDC hdc,                    // [in]
        long cxMenu,                // [in]
        long cyMenu,                // [in]
        BOOL bUpArrowEnabled,       // [in]
        BOOL bDownArrowEnabled      // [in]
        );

    static
    void
    DrawSeparator(
        HDC hdc,                    // [in]
        long x,                     // [in]
        long y,                     // [in]
        long cx,                    // [in]
        long cy,                    // [in]
        long cxMenu                 // [in]
        );

    static
    void
    DrawString(
        HDC hdc,                    // [in]
        WCHAR* pszItemString,       // [in]
        int cchItemString,          // [in]
        BOOL bPopup,                // [in]
        UINT menustate,             // [in]
        UINT uDrawAction,           // [in]
        BOOL bBigFillRect,          // [in]
        long x,                     // [in]
        long y,                     // [in]
        long cx,                    // [in]
        long cy,                    // [in]
        long cxMenu,                // [in]
        long ichAccel,              // [in]
        long xAccel,                // [in]
        WCHAR chMnemo,              // [in]
        long cxArrow                // [in]
        );

    static
    void
    EraseBackground(
        HDC hdc,                    // [in]
        RECT* prc                   // [in]
        );

    static
    HFONT
    MenuBarFont(
        void
        );

    static
    HFONT
    PopupMenuFont(
        void
        );

    static
    void
    NotPopupStringItemComputeSize(
        HDC hdc,                    // [in]
        WCHAR* pszItemString,       // [in]
        int cchItemString,          // [in]
        long cxMenu,                // [in]
        long cyMenu,                // [in]
        long* pcx,                  // [out]
        long* pcy                   // [out]
        );

    static
    void
    PopupStringItemComputeSize(
        HDC hdc,                    // [in]
        WCHAR* pszItemString,       // [in]
        int cchItemString,          // [in]
        int ichAccel,               // [in]
        WCHAR chMnemo,              // [in]
        long cxMenu,                // [in]
        long cyMenu,                // [in]
        long* pcx,                  // [out]
        long* pcy                   // [out]
        );

    static
    int
    ScrollArrowHeight(
        void
        );

    static
    void
    SeparatorItemComputeSize(
        BOOL bPopup,                // [in]
        long cxMenu,                // [in]
        long cyMenu,                // [in]
        long* pcx,                  // [out]
        long* pcy                   // [out]
        );

    static
    void
    SetupColors(
        HDC hdc,                    // [in]
        UINT menustate              // [in]
        );

    static
    void
    SingleColumnComputeWidth(
        long cxMaxString,           // [in]
        long cxMaxAccel,            // [in]
        long* pcxTotal,             // [out]
        long* pxAccel               // [out]
        );

};

#endif /* __MENUCONTROLVIEW_HPP_INCLUDED__ */
