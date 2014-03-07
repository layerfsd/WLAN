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
/*++

Copyright (c) 2001 Microsoft Corporation

Module Name:

    simpolcy.h

Abstract:

    Functions for simpolcy.dll

--*/

#define WM_SIMTOOLKITMENU  (WM_USER + 1)
// wParam not used
// lParam is pointer to string for menu name

// Call StartSimToolkit with callback hwnd.  This hwnd will receive the message above
// with the menu name from the SIM card.
HRESULT StartSimToolkit(HWND hWnd);


// Call ShowSimUI to present UI to the user for running a SIM app.  This will
// load SIMUI.DLL
HRESULT ShowSimUI(void);


