//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//



// Useful menus. Used primarily by wizards. 
// DO NOT CHANGE THESE NAMES as they are used by rcmltocpp

#define IDM_WIZARD_FIRST             0x50      // (RES_RCML_BASE + 0x000)
#define IDM_WIZARD_MID               0x51      // (RES_RCML_BASE + 0x001)
#define IDM_WIZARD_LAST              0x52      // (RES_RCML_BASE + 0x002)
#define IDM_CANCEL_RETRY             0x53      // (RES_RCML_BASE + 0x003)
#define IDM_CANCEL_SAVE              0x54      // (RES_RCML_BASE + 0x004)
#define IDM_EMPTY_MENU_BAR           0x55      // (RES_RCML_BASE + 0x005) // dummy:this creates an empty menu bar (there is no resource associated with it)


//IDs that are returned by the standard wizard menus
#define ID_CANCEL                    1010
#define ID_NEXT                      1011
#define ID_BACK                      1012
#define ID_FINISH                    1013
#define ID_TIMEOUT                   1014

#define SIP_ARROWWIDTH_CX   (SCALEX(7))
// This is essentially half the width of the MS_SIPBUTTON window
#define SIP_HALFWIDTH_CX    (SCALEX(30))
#define SIP_WIDTH_CX        (SIP_HALFWIDTH_CX * 2)

#define ALPHA_OPAQUE                 (0xFF)    // default opacity of taskbar & softkeybar
#define ALPHABLEND_TASKBAR_ENABLED() (ALPHA_OPAQUE > g_bTodayAlpha)
BOOL AlphaBlendHelper(HDC hdc, HDC hdcGradient, HDC hdcToday, HBITMAP hbmToday, BYTE bTodayAlpha, HWND hwnd, 
                                                       DWORD nXSrc, DWORD nYSrc, DWORD width, DWORD height);

void WINAPI SKDrawGradientBackgroundEx(HDC hdc, RECT rc, BOOL fSelected, int nXSrc, int nYSrc, BOOL bInitGradient);
