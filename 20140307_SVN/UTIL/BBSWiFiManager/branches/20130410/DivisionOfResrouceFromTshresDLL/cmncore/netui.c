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
/*++

Module Name:

netui.c

Abstract:

Functions:


Notes:


--*/
#include <windows.h>
#include <types.h>
#include <netui.h>
#include <netuip.h>
#include <resource.h>
#include <commctrl.h>
#include <wcommctl.h>
#include "wzcuip.h"
#include <wpcpriv.h>

#ifdef DEBUG
#define DBGZ_INIT       0x0001
#define DBGZ_USERPWD    0x0002
#define DBGZ_GETIP      0x0004
#define DBGZ_LINECONFIG 0x0008
#define DBGZ_ADDCONN    0x0010
#define DBGZ_DISCONN    0x0020
#define DBGZ_NETWORK    0x0040

#define DBGZ_INTERFACE  0x0400
#define DBGZ_MISC       0x0800
#define DBGZ_ALLOC      0x1000
#define DBGZ_FUNCTION   0x2000
#define DBGZ_WARN       0x4000
#define DBGZ_ERROR      0x8000

DBGPARAM dpCurSettings = {
    TEXT("NetUI"), {
    TEXT("Init"),TEXT("UserPWD"),TEXT("GetIP"),TEXT("LineConfig"),
    TEXT("WNetAddConn"),TEXT("WNetDiscon"),TEXT("Network"),TEXT(""),
    TEXT(""),TEXT(""),TEXT("Interface"),TEXT("Misc"),
    TEXT("Alloc"),TEXT("Function"),TEXT("Warning"),TEXT("Error") },
    DBGZ_ERROR|DBGZ_WARN|DBGZ_INTERFACE|DBGZ_NETWORK
};
#endif

// ----------------------------------------------------------------
//
// Global Data
//
// ----------------------------------------------------------------

HINSTANCE g_hInst;       // code
HINSTANCE g_hInstRes;    // resources
HINSTANCE g_hInstResDPI;

DECLARE_COMMCTRL_TABLE;

//
// ----------------------------------------------------------------
//
//
//  DllEntry : Process attach/detach api.
//
// ----------------------------------------------------------------
BOOL
DllEntry(
    HINSTANCE   hInstDll,             /*@parm Instance pointer. */
    DWORD  dwReason,                  /*@parm Reason routine is called. */
    LPVOID  lpReserved                /*@parm system parameter. */
    )
{
    UNREFERENCED_PARAMETER(lpReserved);
    
    switch (dwReason) {
    case DLL_PROCESS_ATTACH :
        DEBUGREGISTER(hInstDll);
        DisableThreadLibraryCalls(hInstDll);

        //InitHINST(hInstDll);
        //g_hInstResDPI = DPI_LoadLibraryRes(_T("tshres.dll"));
        g_hInstResDPI = hInstDll;
		g_hInstRes = hInstDll;
		g_hInstResDPI = hInstDll;
		
		
        InitCommCtrlTable();
        DEBUGMSG (ZONE_INIT, (TEXT("NETUI process attach\r\n")));
        InitializeWZCQCFG();
        break;
    case DLL_PROCESS_DETACH :
        DEBUGMSG (ZONE_INIT, (TEXT("NETUI detach called\r\n")));
        DeleteWZCQCFG();
        break;
    }

    return TRUE;
}

// 
// ----------------------------------------------------------------
//
//  GetNetString
//
// ----------------------------------------------------------------
int
GetNetString (UINT uID, LPTSTR lpBuffer, int nBufferMax)
{
    return LoadString (HINST_RESDLL, (uID+GETNETSTR_BASE), lpBuffer, nBufferMax);
}

// 
// ----------------------------------------------------------------
//
//  NetMsgBox
//
// ----------------------------------------------------------------
BOOL WINAPIV NetMsgBox (HWND hParent, DWORD dwFlags, TCHAR *szStr)
{
    TCHAR szTitle[200];
    DWORD dwStyle, dwId;
    int iRet;
    HCURSOR hCur;

    // Default title is "Windows CE Networking"
    if (dwFlags & NMB_FL_TITLEUSB)
        dwId = IDS_NETUI_USB_CAPTION;
    else
        dwId = IDS_NETUI_NETMSGBOX_TITLE;

    if (!LoadString(HINST_RESDLL, dwId,szTitle,
                    sizeof(szTitle)/sizeof(szTitle[0])))
        return FALSE;
    dwStyle = MB_SETFOREGROUND | MB_APPLMODAL;
    if (dwFlags & NMB_FL_OK)
        dwStyle |= MB_OK;
    if (dwFlags & NMB_FL_EXCLAIM)
        dwStyle |= MB_ICONEXCLAMATION;
    if (dwFlags & NMB_FL_YESNO)
        dwStyle |= MB_YESNO;
    if (dwFlags & NMB_FL_TOPMOST)
        dwStyle |= MB_TOPMOST;

    // Disable cursor, and save old value
    hCur = SetCursor(NULL);
    iRet = MessageBox( hParent, szStr, szTitle,dwStyle | MB_SETFOREGROUND);
    SetCursor(hCur);

    if (dwFlags & NMB_FL_YESNO)
        return ((iRet == IDYES) ? TRUE : FALSE);
    else
        return (iRet? TRUE:FALSE);
}

#if 0
BOOL
CenterWindow (HWND hWnd)
{
    RECT    rc;
    int     cxWin, cyWin;
    RECT    rcWorkArea;

    SystemParametersInfo(SPI_GETWORKAREA, 0, (void*)&rcWorkArea, 0);

    // Center the window.
    GetWindowRect (hWnd, &rc);

    DEBUGMSG (ZONE_MISC, (TEXT("Win Rect (%d,%d,%d,%d) Work Rect (%d,%d,%d,%d)\r\n"),
                          rc.left, rc.top, rc.bottom - rc.top,
                          rc.right-rc.left,
                          rcWorkArea.left, rcWorkArea.top,
                          rcWorkArea.bottom - rcWorkArea.top,
                          rcWorkArea.right-rcWorkArea.left));
    cxWin = rcWorkArea.left + ((rcWorkArea.right-rcWorkArea.left) -
                                  (rc.right - rc.left))/2;
    cyWin = rcWorkArea.top + ((rcWorkArea.bottom - rcWorkArea.top) -
                                 (rc.bottom - rc.top))/2;
    SetWindowPos (hWnd, NULL, cxWin, cyWin, 0, 0,
                  SWP_NOSIZE | SWP_NOZORDER);
    DEBUGMSG (ZONE_MISC, (TEXT("New Left/Top=%d,%d\r\n"),
                   cxWin, cyWin));
    return TRUE;
}
#endif

