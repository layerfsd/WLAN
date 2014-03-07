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
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

netui.c

Abstract:  

Functions:


Notes: 


--*/
#include <windows.h>
#include <types.h>
#include "netui.h"
#include "netuip.h"
#include "resource.h"
#include <commctrl.h>
#include <wcommctl.h>

#ifdef DEBUG
#define DBGZ_INIT		0x0001
#define DBGZ_USERPWD	0x0002
#define DBGZ_GETIP  	0x0004
#define DBGZ_LINECONFIG 0x0008
#define DBGZ_ADDCONN    0x0010
#define DBGZ_DISCONN    0x0020
#define DBGZ_NETWORK	0x0040

#define DBGZ_INTERFACE	0x0400
#define DBGZ_MISC		0x0800
#define DBGZ_ALLOC		0x1000
#define DBGZ_FUNCTION	0x2000
#define DBGZ_WARN		0x4000
#define DBGZ_ERROR		0x8000

DBGPARAM dpCurSettings = {
	TEXT("NetUI"), {
	TEXT("Init"),TEXT("UserPWD"),TEXT("GetIP"),TEXT("LineConfig"),
	TEXT("WNetAddConn"),TEXT("WNetDiscon"),TEXT("Network"),TEXT(""),
	TEXT(""),TEXT(""),TEXT("Interface"),TEXT("Misc"),
	TEXT("Alloc"),TEXT("Function"),TEXT("Warning"),TEXT("Error") },
	DBGZ_ERROR|DBGZ_WARN 
}; 
#endif

// ----------------------------------------------------------------
//
// Global Data
//
// ----------------------------------------------------------------

HINSTANCE v_hInst;
CRITICAL_SECTION g_csNetUI;

DECLARE_COMMCTRL_TABLE;

extern void DeleteWZCQCFG();
extern void InitializeWZCQCFG();

// 
// ----------------------------------------------------------------
//	
//
//	DllEntry : Process attach/detach api.
//
// ----------------------------------------------------------------
BOOL
DllEntry(
	HINSTANCE	hInstDll,			  /*@parm Instance pointer. */
	DWORD	dwReason,				  /*@parm Reason routine is called. */
	LPVOID	lpReserved				  /*@parm system parameter. */
	)
{
	NetuiDbgMsg(TIME_STAMP_ON, "[netui.c] %s \r\n", __FUNCTION__ );
	switch (dwReason) {
	case DLL_PROCESS_ATTACH :
		DEBUGREGISTER(hInstDll);
		v_hInst = hInstDll;
		InitializeCriticalSection(&g_csNetUI);
		InitCommCtrlTable();
		InitializeWZCQCFG();
		DEBUGMSG (ZONE_INIT, (TEXT("NETUI process attach\r\n")));
		DisableThreadLibraryCalls((HMODULE) hInstDll);
		NetuiDbgMsg(TIME_STAMP_ON, _T("[netui.c] DllEntry - DLL_PROCESS_ATTACH \r\n") );
		
		break;
	case DLL_PROCESS_DETACH :
		DEBUGMSG (ZONE_INIT, (TEXT("NETUI detach called\r\n")));
		DeleteWZCQCFG();
		DeleteCriticalSection(&g_csNetUI);
		NetuiDbgMsg(TIME_STAMP_ON, _T("[netui.c] DllEntry - DLL_PROCESS_DETACH \r\n") );
		break;
	default :
		// I don't care about thread events.
		break;
	}

	return TRUE;
}


// 
// ----------------------------------------------------------------
//	
//	GetNetString
//
// ----------------------------------------------------------------
int
GetNetString (UINT uID, LPTSTR lpBuffer, int nBufferMax)
{
	return LoadString (v_hInst, (uID+GETNETSTR_BASE), lpBuffer, nBufferMax);
}

// 
// ----------------------------------------------------------------
//	
//	NetMsgBox
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
        dwId = IDS_USB_CAPTION;
    else
        dwId = IDS_NETMSGBOX_TITLE;

    if (!LoadString(v_hInst, dwId,szTitle,
                    sizeof(szTitle)/sizeof(szTitle[0])))
        return FALSE;
    dwStyle = MB_SETFOREGROUND | MB_APPLMODAL;
    if (dwFlags & NMB_FL_OK)
        dwStyle |= MB_OK;
    if (dwFlags & NMB_FL_EXCLAIM)
        dwStyle |= MB_ICONEXCLAMATION;
    if (dwFlags & NMB_FL_INFORMATION) 
        dwStyle |= MB_ICONINFORMATION;
    if (dwFlags & NMB_FL_YESNO)
        dwStyle |= MB_YESNO;
    if (dwFlags & NMB_FL_TOPMOST)
        dwStyle |= MB_TOPMOST;
    if (dwFlags & NMB_FL_DEFBUTTON2) 
        dwStyle |= MB_DEFBUTTON2;

    // Disable cursor, and save old value
    hCur = SetCursor(NULL);    
    iRet = MessageBox( hParent, szStr, szTitle,dwStyle);
    SetCursor(hCur);
    
    if (dwFlags & NMB_FL_YESNO)
        return ((iRet == IDYES) ? TRUE : FALSE);
    else
        return (iRet? TRUE:FALSE);
}


BOOL SetFullScreen(HWND hWnd)
{
	SHINITDLGINFO shidi;
	SHMENUBARINFO shmbi;

	ZeroMemory( &shidi, sizeof( shidi ) );
	shidi.dwMask  = SHIDIM_FLAGS;
	shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN;
	shidi.hDlg    = hWnd;

	// Initialize the dialog
	if ( !SHInitDialog( &shidi ) )
	{
		return FALSE;
	}

	memset(&shmbi, 0, sizeof(SHMENUBARINFO));
	shmbi.cbSize = sizeof( SHMENUBARINFO );
	shmbi.hwndParent = hWnd;
	shmbi.dwFlags = SHCMBF_HMENU;
	shmbi.nToolBarId = IDM_MENUBAR;
	shmbi.hInstRes = v_hInst;

	if (!SHCreateMenuBar( &shmbi ))
	{
		return FALSE;
	}
	
	return TRUE;
}