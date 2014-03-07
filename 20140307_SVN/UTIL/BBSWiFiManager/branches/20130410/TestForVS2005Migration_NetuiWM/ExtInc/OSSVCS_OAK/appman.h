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

    File: appman.h
    

    History:
        Created - KKennedy 12/11/1

*/
#ifndef _APPMAN_H_
#define _APPMAN_H_

#define SZ_APPMAN_CLASS_BASE TEXT("MSAppMan")

#define WM_APPMAN_WINDOWDESTROYED (WM_APP + 1)
#define WM_APPMAN_FORCEBASESTATE  (WM_APP + 2)

enum AMReason
    {
    AMR_Start,
    AMR_ReStart,
    AMR_Exit
    };

struct AMRStart
    {
    const TCHAR* pszParams; // in - Command line passed to the start
    HWND hwnd; // out - Set this to be the main hwnd of the app
    LPARAM lParam; // out - piece of data the app manager will give you whenever it calls you back.  Make sure you free anything in the AMR_Exit handler.
    };
struct AMRRestart
    {
    const TCHAR* pszParams; // in - Command line passed to the re-start
    LPARAM lParam; // in/out - piece of data the app manager will give you whenever it calls you back.  Make sure you free anything in the AMR_Exit handler.
    };
struct AMRExit
    {
    LPARAM lParam; // in/out - This is the last time you will see this LPARAM
    };

typedef HRESULT (*PFN_APP)(AMReason dwReason, void* pParams);


#endif // _APPMAN_H_
