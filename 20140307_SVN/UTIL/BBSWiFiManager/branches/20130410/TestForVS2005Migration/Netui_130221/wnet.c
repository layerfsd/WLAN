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

wnet.c

Abstract:
  This file contains the code for the connect/disconnect dialog functions
  in the WNet API set.

Functions:


Notes:


--*/
#include <windows.h>
#include <windowsx.h>
#include <types.h>
#include <winnetwk.h>
#include <ehm.h>
#include "netui.h"
#include "netuip.h"
#include <Resource.h>
#include "aygshell.h"
#include "wpcpriv.h"
#include <todaycmn.h>


#define DBGTEXT(fmt)     TEXT(fmt) TEXT("\r\n")

// In case the WNet APIs aren't loaded (redir module is not included
// in project cesysgen.bat), do a LoadLibrary to obtain function pointers
// for these functions.
typedef DWORD (APIENTRY *PFN_WNetOpenEnum)(DWORD dwScope,DWORD dwType,DWORD dwUsage,
                                           LPNETRESOURCE lpNetResource,LPHANDLE lphEnum);
typedef DWORD (APIENTRY *PFN_WNetEnumResource)(HANDLE  hEnum,LPDWORD lpcCount,
                                               LPVOID  lpBuffer,LPDWORD lpBufferSize);
typedef DWORD (APIENTRY *PFN_WNetCloseEnum)(HANDLE hEnum);
typedef DWORD (APIENTRY *PFN_WNetDisconnectDialog1)(LPDISCDLGSTRUCT lpConnDlgStruct);

static PFN_WNetOpenEnum pfnWNetOpenEnum;
static PFN_WNetEnumResource pfnWNetEnumResource;
static PFN_WNetCloseEnum pfnWNetCloseEnum;
static PFN_WNetDisconnectDialog1 pfnWNetDisconnectDialog1;


// Font\color table for DlgProcAddConnect.
static const CCtlTextDesc gs_rgTextDescAddConn[] =
{
    {IDC_NETUI_REMOTE_STATIC,     8,  FW_NORMAL,   STANDARD_TEXT_COLOR },
//    {IDC_NETUI_REMOTENAME,        9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_LOCAL_STATIC,      8,  FW_NORMAL,   STANDARD_TEXT_COLOR },
//    {IDC_NETUI_LOCALNAME,         9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDOK,                  8,  FW_BOLD,     STANDARD_TEXT_COLOR },
    {IDCANCEL,              8,  FW_BOLD,     STANDARD_TEXT_COLOR },
};


// Font\color table for DlgProcRemoveConnect.
static const CCtlTextDesc gs_rgTextDescRemoveConn[] =
{
    {IDC_NETUI_NETCONN_STATIC,    8,  FW_NORMAL,  STANDARD_TEXT_COLOR },
    {IDC_NETUI_DISCONLIST,        8,  FW_NORMAL,  STANDARD_TEXT_COLOR },
    {IDCANCEL,              8,  FW_BOLD,    STANDARD_TEXT_COLOR },
    {IDOK,                  8,  FW_BOLD,    STANDARD_TEXT_COLOR },
};



HWND InitializeCustomItem(TODAYLISTITEM *pTodayListItem, HWND hwndParent)
{
	return (HWND)(0);
}
BOOL CALLBACK
DlgProcResourcePwd( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	PNETUI_RESPWD	pResPwd;
	DWORD			dwStyle;
	SIPINFO         si;
    TCHAR StrBuf[100], ResFmtBuf[RMLEN + 30];
    static BOOL     fReEnable = FALSE;

    switch( message ) {
    case WM_INITDIALOG:
		pResPwd = (PNETUI_RESPWD)lParam;
		SetWindowLong (hDlg, DWL_USER, (LONG)pResPwd);

        // Display resource we are connecting to
        if (LoadString(HINST_RESDLL,IDS_NETUI_RESFMT_STR,StrBuf,sizeof(StrBuf)/sizeof(StrBuf[0]))) {
            wsprintf(ResFmtBuf,StrBuf,pResPwd->szResourceName);
            SetDlgItemText(hDlg,IDC_NETUI_CMDTEXT,ResFmtBuf);
        }
		dwStyle = GetWindowLong (hDlg, GWL_EXSTYLE);
		SetWindowLong (hDlg, GWL_EXSTYLE, dwStyle | WS_EX_CAPTIONOKBTN|WS_EX_TOPMOST);
		
        SetDlgItemText( hDlg, IDC_NETUI_PASSWORD, pResPwd->szPassword );

        SetFocus(GetDlgItem(hDlg, IDC_NETUI_PASSWORD));

		return FALSE;

    case WM_COMMAND:
		pResPwd = (PNETUI_RESPWD)GetWindowLong (hDlg, DWL_USER);

        switch( LOWORD( wParam ) ) {
        case IDCANCEL: 				// Ignore values
            EndDialog( hDlg, FALSE );
            break;

        case IDOK: 					// Get the dialog values
            GetDlgItemText( hDlg, IDC_NETUI_PASSWORD,
							pResPwd->szPassword, PWLEN );
            EndDialog( hDlg, TRUE );
            break;

        case IDC_NETUI_PASSWORD:
            if ((HIWORD(wParam) == EN_SETFOCUS) ||
                ((HIWORD(wParam) == EN_KILLFOCUS) &&
                  fReEnable))
            {
                memset(&si, 0, sizeof(SIPINFO));
                si.cbSize = sizeof(SIPINFO);
                if (SHSipInfo(SPI_GETSIPINFO, 0, &si, 0))
                {
                    // Toggle password completion, but keep track of
                    // whether or not this flag was set a priori when setting focus
                    if (HIWORD(wParam) == EN_SETFOCUS)
                    {
                        fReEnable = !(si.fdwFlags & SIPF_DISABLECOMPLETION);
                        si.fdwFlags |= SIPF_DISABLECOMPLETION;
                    }
                    else
                    {
                        si.fdwFlags &= ~SIPF_DISABLECOMPLETION;
                    }
		            SHSipInfo(SPI_SETSIPINFO, 0, &si, 0);
                }
            }
            break;
        }
        return FALSE;

    case WM_DESTROY:
        return( FALSE );

    default:
        return( FALSE );
        break;
    }
}


/* GetResourcePassword
 *
 *    Dialog box to get resource password for servers using share level
 *    security.
 *
 * Return Value:
 *    Return TRUE if successful, FALSE if error or cancel.
 */

BOOL WINAPI GetResourcePassword (HWND hParent, PNETUI_RESPWD pResPwd)
{
	HRSRC hRsrc;
	HGLOBAL hGlob;
	int iRet;
    HCURSOR hCur;

    // For WC_SIPPREF
    SHInitExtraControls();

	hRsrc  = FindResource (HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_RESPWD),RT_DIALOG);
	hGlob = LoadResource (HINST_RESDLL, hRsrc);

    hCur = SetCursor(NULL);
	iRet = DialogBoxParam(HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_RESPWD),
                          SHGetLastActiveWindow(hParent), DlgProcResourcePwd,(LONG)pResPwd);
    SetCursor(hCur);

    if (iRet == -1) {
        RETAILMSG(1,(DBGTEXT("GetResourcePassword: DialogBox returned err %u"),
                     GetLastError()));
        return FALSE;
    }
    else
        return iRet;
}


/* DlgProcAddConnect
 *
 *    Dialog box procedure for WnetAddConnect. Very simple now, later
 *    integrate with browser.
 *
 * Return Value:
 *    Return TRUE if successful, FALSE if error or cancel.
 */
static BOOL CALLBACK
DlgProcAddConnect(
    HWND hDlg,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
    ADDCONNECT_DLGPARAMS *pDlgParams;

    // FONT HANDLING STUFF
    // set font size, weight, and color per the table gs_rgTextDescAddConn
    static  void*   s_pvFontManager;
    ULONG ulFontMgrRetVal;

    if (SHRunFontManager(gs_rgTextDescAddConn,
                         ARRAYSIZEOF(gs_rgTextDescAddConn),
                         &s_pvFontManager,
                         message,
                         hDlg,
                         wParam,
                         lParam,
                         &ulFontMgrRetVal) == CTLTEXT_EXIT )
    {
        return (BOOL)ulFontMgrRetVal;
    }

    switch( message ) {

        case WM_INITDIALOG:
            // Param passed in lParam
            pDlgParams = (ADDCONNECT_DLGPARAMS *)lParam;

            DEBUGMSG(ZONE_ADDCONN,(DBGTEXT("+AddConnect: %s, ReadOnly: %s"),pDlgParams->RemoteName,
                                   pDlgParams->bReadOnly? TEXT("TRUE"):TEXT("FALSE")));

            SetDlgItemText( hDlg, IDC_NETUI_REMOTENAME, pDlgParams->RemoteName );
            SetDlgItemText( hDlg, IDC_NETUI_LOCALNAME,  pDlgParams->LocalName );

            if (pDlgParams->RemoteName[0]) {
                SendMessage(GetDlgItem(hDlg,IDC_NETUI_REMOTENAME),
                            EM_SETREADONLY,(WPARAM)pDlgParams->bReadOnly,0);
                SetFocus(GetDlgItem(hDlg, IDC_NETUI_LOCALNAME));
            } else {
                SetFocus(GetDlgItem(hDlg, IDC_NETUI_REMOTENAME));
            }

            // Set userdata for window so we can retrieve param later
            SetWindowLong(hDlg, GWL_USERDATA, (LONG) pDlgParams);
            return( FALSE );

        case WM_COMMAND:
            pDlgParams = (ADDCONNECT_DLGPARAMS *)GetWindowLong(hDlg,GWL_USERDATA);

            switch( LOWORD( wParam ) ) {
                case IDCANCEL: 				// Ignore values
                    DEBUGMSG(ZONE_ADDCONN,(DBGTEXT("-AddConnect: CANCELLED")));
                    EndDialog( hDlg, FALSE );
                    break;

                case IDOK: 					// Get the dialog values
                    GetDlgItemText( hDlg, IDC_NETUI_LOCALNAME,
                                    pDlgParams->LocalName,
                                    sizeof(pDlgParams->LocalName)/sizeof(pDlgParams->LocalName[0]));
                    GetDlgItemText( hDlg, IDC_NETUI_REMOTENAME,
                                    pDlgParams->RemoteName,
                                    sizeof(pDlgParams->RemoteName)/sizeof(pDlgParams->RemoteName[0]));
                    DEBUGMSG(ZONE_ADDCONN,(DBGTEXT("-AddConnect: Remote: %s, Local: %s"),
                                           pDlgParams->RemoteName, pDlgParams->LocalName));
                    EndDialog( hDlg, TRUE );
                    break;
            }

        case WM_DESTROY:
            return( FALSE );

        default:
            return( FALSE );
    }
}


BOOL WINAPI ConnectionDialog (HWND hParent, PADDCONNECT_DLGPARAMS pDlgParams)
{
	HRSRC hRsrc;
	HGLOBAL hGlob;
	int iRet;
    HCURSOR hCur;

    if( pDlgParams == NULL )
	    return FALSE;
	
    // For WC_SIPPREF
    SHInitExtraControls();

	hRsrc  = FindResource (HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_ADD_CONNECTION),RT_DIALOG);
	hGlob = LoadResource (HINST_RESDLL, hRsrc);

    hCur = SetCursor(NULL);
	iRet = DialogBoxParam (HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_ADD_CONNECTION),
						   SHGetLastActiveWindow(hParent), DlgProcAddConnect, (LONG)pDlgParams);
    SetCursor(hCur);

    if (iRet == -1) {
        RETAILMSG(1,(DBGTEXT("ConnectionDialog: DialogBoxParams returned err %u"),
                     GetLastError()));
        return FALSE;
    }
    else
        return iRet;
}


static void
DisconnectDialogDone(HWND hDlg, BOOL bResult)
{
    int i,Count;
    NETRESOURCE *pNetRes;
    HWND hListBoxWnd = GetDlgItem(hDlg,IDC_NETUI_DISCONLIST);

    // Free all the NETRESOURCE structs
    if ((Count = ListBox_GetCount(hListBoxWnd)) == LB_ERR)
        DEBUGMSG(ZONE_ERROR,(DBGTEXT("!DisconnectDlgDone: LB_GETCOUNT failed")));
    else {
        for (i=0; i<Count; i++) {
            pNetRes = (NETRESOURCE *)ListBox_GetItemData(hListBoxWnd,i);
            if ((LRESULT)pNetRes == LB_ERR) {
                DEBUGMSG(ZONE_ERROR,(DBGTEXT("!DisconnectDlgDone: LB_GETITEMDATA returned err %d"),
                                     GetLastError()));
            }
            else if (pNetRes == NULL) {
                DEBUGMSG(ZONE_ERROR,(DBGTEXT("!DisconnectDlgDone: LB_GETITEMDATA %d returned NULL"),i));
            }
            else
                LocalFree(pNetRes);
        }
    }

    if (!EndDialog(hDlg, bResult))
        DEBUGMSG(ZONE_ERROR,(DBGTEXT("!DisconnectDlgDone: Error in EndDialog(%X): %u"),
                             hDlg,GetLastError()));
}

/* DlgProcRemoveConnect
 *
 *  Dialog box procedure for WNetDisconnectDialog() function. Construct a
 *  list of all connected and remembered resources, and allow the user to
 *  select one to attempt to delete.
 */
static BOOL CALLBACK
DlgProcRemoveConnect(
    HWND hDlg,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{

    HWND hListBoxWnd = GetDlgItem(hDlg,IDC_NETUI_DISCONLIST);

    // FONT HANDLING STUFF
    // set font size, weight, and color per the table gs_rgTextDescRemoveConn
    static  void*   s_pvFontManager;
    ULONG ulFontMgrRetVal;

    if (SHRunFontManager(gs_rgTextDescRemoveConn,
                         ARRAYSIZEOF(gs_rgTextDescRemoveConn),
                         &s_pvFontManager,
                         message,
                         hDlg,
                         wParam,
                         lParam,
                         &ulFontMgrRetVal) == CTLTEXT_EXIT )
    {
        return (BOOL)ulFontMgrRetVal;
    }

    switch( message ) {

        case WM_INITDIALOG:
        {
            DWORD Status, BufSize, NumElements;
            NETRESOURCE *pNetRes;
            HANDLE hEnum;
            DWORD dwScope = RESOURCE_CONNECTED;

    StartEnum:
            // Populate listbox with list of resources.  Since we don't automatically
            // try to connect our remebered resources, we have to enumerate connected
            // and remembered resources separately.
            if ((Status = pfnWNetOpenEnum(dwScope,RESOURCETYPE_ANY,
                                          0, // Doesn't matter
                                          NULL,&hEnum)) != ERROR_SUCCESS) {
                DEBUGMSG(ZONE_ERROR,(DBGTEXT("DlgProcRemoveConnect: Error %u in WNetOpenEnum(%d)"),
                                     GetLastError(),dwScope));
                return FALSE;
            }
            while (Status == ERROR_SUCCESS) {
                DWORD iRet;
                WCHAR StrBuf[100];

                // First, call with 0 buffer to determine needed size (this is not very efficient,
                // but we want a separate buffer for each NETRESOURCE struct).
                BufSize = 0;
                pNetRes = NULL;
                NumElements = 1;
                Status = pfnWNetEnumResource(hEnum,&NumElements,pNetRes,&BufSize);
                if (Status == ERROR_NO_MORE_ITEMS)
                    break;
                else if (Status != ERROR_MORE_DATA) {
                    DEBUGMSG(ZONE_ERROR,(DBGTEXT("DlgProcRemoveConnect: WNetEnumResource(0) returned %u\r\n"),Status));
                }
                else
                    DEBUGMSG(ZONE_DISCONN,(DBGTEXT("DlgProcRemoveConnect: Using buffer size %u"),BufSize));

                if ((pNetRes = LocalAlloc(0,BufSize)) == NULL) {
                    DEBUGMSG(ZONE_ERROR,(DBGTEXT("DlgProcRemoveConnect: LocalAlloc(%d) failed"),BufSize));
                    Status = ERROR_NOT_ENOUGH_MEMORY;
                    break;
                }
                NumElements = 1;
                Status = pfnWNetEnumResource(hEnum,&NumElements,pNetRes,&BufSize);
                if (Status != ERROR_SUCCESS) {
                    DEBUGMSG(ZONE_ERROR,(DBGTEXT("DlgProcRemoveConnect: Error %u in WNetEnumResource()"),
                                         GetLastError()));
                    break;
                }

                // Use TAB to separate local, remote names
                wsprintf(StrBuf,L"%s\t%s",pNetRes->lpLocalName? pNetRes->lpLocalName : L"(none)",pNetRes->lpRemoteName);
                // If remembered resource is also connected, we'll get duplicated entries
                if (ListBox_FindString(hListBoxWnd,0,StrBuf) != LB_ERR) {
                    DEBUGMSG(ZONE_DISCONN,(DBGTEXT("Entry %s already added to listbox"),StrBuf));
                    LocalFree(pNetRes);
                    continue;
                }
                DEBUGMSG(ZONE_DISCONN,(DBGTEXT("Adding entry %s to listbox"),StrBuf));
                iRet = ListBox_AddString(hListBoxWnd,StrBuf);
                if ((iRet == LB_ERR) || (iRet == LB_ERRSPACE)) {
                    DEBUGMSG(ZONE_ERROR,(DBGTEXT("DlgProcRemoveConnect: Error adding string to listbox: %d"),iRet));
                    LocalFree(pNetRes);
                    continue;
                }
                ListBox_SetItemData(hListBoxWnd,iRet,pNetRes);
            }
            pfnWNetCloseEnum(hEnum);
            if (Status != ERROR_NO_MORE_ITEMS) {
                // Some error occurred, clean up
                DisconnectDialogDone(hDlg,FALSE);
                return FALSE;
            }
            // List both connected and remembered resources
            if (dwScope == RESOURCE_CONNECTED) {
                dwScope = RESOURCE_REMEMBERED;
                goto StartEnum;
            }

            ListBox_SetCurSel(hListBoxWnd,0);

            return FALSE;
        }
        case WM_COMMAND:
            switch( LOWORD( wParam ) ) {
                case IDCANCEL:
                    // Clean up
                    DisconnectDialogDone(hDlg, FALSE);
                    break;

                case IDOK:
                {
                    BOOL bRet = TRUE;
                    NETRESOURCE *pNetRes;
                    int Index;
                    Index = ListBox_GetCurSel(hListBoxWnd);
                    pNetRes = (NETRESOURCE *)ListBox_GetItemData(hListBoxWnd,Index);
                    if (((LRESULT)pNetRes == LB_ERR) || (pNetRes == NULL)) {
                        DEBUGMSG(ZONE_ERROR,(DBGTEXT("!DisconnectDlgDone: LB_GETITEMDATA returned err or NULL: %d"),
                                             GetLastError()));
                    }
                    else {
                        // Disconnect selected resource
                        DISCDLGSTRUCT DlgStruct;
                        DlgStruct.cbStructure = sizeof(DlgStruct);
                        DlgStruct.hwndOwner   = hDlg;
                        DlgStruct.lpLocalName = pNetRes->lpLocalName;
                        DlgStruct.lpRemoteName= pNetRes->lpRemoteName;
                        DlgStruct.dwFlags     = DISC_UPDATE_PROFILE;

                        DEBUGMSG(ZONE_DISCONN,(DBGTEXT("Got Selection %s -> %s"),
                                           pNetRes->lpLocalName,pNetRes->lpRemoteName));
                        if (pfnWNetDisconnectDialog1(&DlgStruct) != ERROR_SUCCESS)
                            bRet = FALSE;
                    }

                    // Clean up
                    DisconnectDialogDone(hDlg, bRet);
                    break;
                }
            }

        case WM_DESTROY:
            return( FALSE );

        default:
            return( FALSE );
    }
}

BOOL WINAPI DisconnectDialog (HWND hParent, DWORD dwType)
{
    HRESULT hr = S_OK;
	HRSRC hRsrc;
	HGLOBAL hGlob;
	int iRet;
    HINSTANCE hCoreDllInst = NULL;
    HCURSOR hCur;

    UNREFERENCED_PARAMETER(dwType);

    // Load WNet function pointers, or return error if not present
    hCoreDllInst = LoadLibrary(TEXT("coredll.dll"));
    CWR(hCoreDllInst);

    pfnWNetOpenEnum = (PFN_WNetOpenEnum)GetProcAddress(hCoreDllInst,TEXT("WNetOpenEnumW"));
    CPREx(pfnWNetOpenEnum, E_FAIL);
    
    pfnWNetEnumResource = (PFN_WNetEnumResource)GetProcAddress(hCoreDllInst,TEXT("WNetEnumResourceW"));
    CPREx(pfnWNetEnumResource, E_FAIL);
    
    pfnWNetCloseEnum = (PFN_WNetCloseEnum)GetProcAddress(hCoreDllInst,TEXT("WNetCloseEnum"));
    CPREx(pfnWNetCloseEnum, E_FAIL);
    
    pfnWNetDisconnectDialog1 = (PFN_WNetDisconnectDialog1) GetProcAddress(hCoreDllInst,TEXT("WNetDisconnectDialog1W"));
    CPREx(pfnWNetDisconnectDialog1, E_FAIL);

	hRsrc  = FindResource (HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_REMOVE_CONNECTION),RT_DIALOG);
	hGlob = LoadResource (HINST_RESDLL, hRsrc);

	hCur = SetCursor(NULL);
	iRet = DialogBox(HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_REMOVE_CONNECTION),
						   SHGetLastActiveWindow(hParent), DlgProcRemoveConnect);
    SetCursor(hCur);

    FreeLibrary(hCoreDllInst);

    if (iRet == -1) {
        RETAILMSG(1,(DBGTEXT("DisconnectDialog: DialogBox returned err %u"),
                     GetLastError()));
        return FALSE;
    }
    else
        return TRUE;

Error:
    if (NULL != hCoreDllInst)
    {
        FreeLibrary(hCoreDllInst);
    }

    return FALSE;
}
