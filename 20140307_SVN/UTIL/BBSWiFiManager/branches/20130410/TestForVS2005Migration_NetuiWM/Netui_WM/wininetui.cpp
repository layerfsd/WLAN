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
/////////////////////////////////////////////////////////////////////////////
//
//  WININETUI.CPP
//
//  Description:
//      replacement dialogs for WININET
//
//
/////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <wininet.h>
#include <wininetui.h>

#include "wpcpriv.h"
#include "netuip.h"
#include <resource.h>

#include "winetui.h"

#include "delayld.h"
#pragma warning(disable:4273) 

DELAY_LOAD_BOOL_FREE(WININET, InternetSetOptionW, 
                     (HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength),
                     (hInternet, dwOption, lpBuffer, dwBufferLength))
DELAY_LOAD_BOOL_FREE(WININET, InternetQueryOption, 
                     (HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, LPDWORD pdwBufferLength),
                     (hInternet, dwOption, lpBuffer, pdwBufferLength))

BOOL SetCertDlgItem(HWND hDlg,
                    DWORD dwIconCtl,
                    DWORD dwTextCtl,
                    DWORD dwString,
                    BOOL  fError);

BOOL InitSecCertErrorsDlg(HWND hDlg,PERRORINFODLGTYPE pDlgInfo);


/*
Routine Description:
    Updates several Global flags,and writes the update to the registry.
    The update is based on dwCtlId which is a dialog resource id.

    The update ALWAYS turns OFF the flag,since the only of turning
    it back on is to use the Ctl Pannel/Internet/Security PropSheet.

Arguments:
    dwCtlId    - Dialog ID to base update on.
    dwFlags    - Flags assoicated with the dialog.

Return Value:
    VOID
        none.
*/
VOID UpdateGlobalSecuritySettings(IN DWORD dwCtlId,IN DWORD dwFlags)
{
    const   TCHAR c_szRegPath[] = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings");

    DWORD   dwFalse = FALSE;
    HKEY    hKey;

    if ( ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,
                            c_szRegPath,
                            0,    // reserved
                            NULL,  // class
                            0,    // options
                            KEY_READ | KEY_WRITE,
                            NULL,  // security attributes
                            &hKey,
                            NULL) )
    {
        RETAILMSG(1, (TEXT("WININETUI: could not open regkey to ignore dialog gle=0x%x\n"),
                       GetLastError()));
        return;
    }

    switch( dwCtlId )
    {
/*
    case IDD_BAD_CN:

        RegSetValueEx(hKey, TEXT("WarnOnBadCertSending"),
                      0, REG_DWORD, &dwFalse, sizeof(dwFalse));
        RegSetValueEx(hKey, TEXT("WarnOnBadCertRecving"),
                      0, REG_DWORD, &dwFalse, sizeof(dwFalse));
        break;
    case IDD_HTTP_TO_HTTPS_ZONE_CROSSING:
*/
    case IDD_HTTPS_TO_HTTP_ZONE_CROSSING:
        RegSetValueEx(hKey, TEXT("WarnOnZoneCrossing"),
                      0, REG_DWORD, (LPBYTE)&dwFalse, sizeof(dwFalse));
        break;

    case IDD_WARN_ON_POST:
        RegSetValueEx(hKey, TEXT("WarnOnPost"),
                      0, REG_DWORD, (LPBYTE)&dwFalse, sizeof(dwFalse));
        break;

    case IDD_HTTP_POST_REDIRECT:
        RegSetValueEx(hKey, TEXT("WarnOnPostRedirect"),
                      0, REG_DWORD, (LPBYTE)&dwFalse, sizeof(dwFalse));
        break;
    }

    RegCloseKey(hKey);
}



/*
Routine Description:
    Supports Yes/No,Ok/Cancel decisions for the authentication UI.

Arguments:
    hwnd    - standard dialog params
    msg     - "
    wparam  - "
    lparam  - "

Return Value:
    BOOL
        TRUE    - we handled message
        FALSE   - Windows should handle message
*/
INT_PTR CALLBACK OkCancelDialogProc(HWND hwnd,
                                    UINT msg,
                                    WPARAM wparam,
                                    LPARAM lparam)
{
    BOOL              fRet = FALSE;
    PERRORINFODLGTYPE pDlgInfo;

    if ( msg != WM_INITDIALOG )
    {
        pDlgInfo = (PERRORINFODLGTYPE) GetWindowLongW(hwnd,DWL_USER);
        fRet = SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(pDlgInfo->dwDlgId),
                                     hwnd, msg, wparam, lparam );
    }

    switch(msg)
    {
    case WM_INITDIALOG:
        ASSERT(lparam);

        SetWindowLongW(hwnd,DWL_USER,lparam);

        pDlgInfo = (PERRORINFODLGTYPE)lparam;

        SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(pDlgInfo->dwDlgId),
                              hwnd, msg, wparam, lparam );

        SHINITDLGINFO shidi;
        SHMENUBARINFO   mbi;

        // Some of these dialogs need a scrollbar
        shidi.dwMask = SHIDIM_FLAGS;
        shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_WANTSCROLLBAR;
        shidi.hDlg = hwnd;
        SHInitDialog(&shidi);

        // softkeys
        memset(&mbi, 0, sizeof(SHMENUBARINFO));
        mbi.cbSize = sizeof(SHMENUBARINFO);
        mbi.hwndParent = hwnd;
        mbi.nToolBarId = IDR_INETUI_DIALOGYESNO;
        mbi.hInstRes = HINST_RESDLL;
        SHCreateMenuBar(&mbi);

        SHSetWindowBits(hwnd, GWL_STYLE, WS_NONAVDONEBUTTON,   WS_NONAVDONEBUTTON);

        if(IDD_SEC_CERT_ERRORS == pDlgInfo->dwDlgId)
        {
            InitSecCertErrorsDlg(hwnd,pDlgInfo);
        }
        else if(IDD_REVOCATION_PROBLEM == pDlgInfo->dwDlgId)
        {
            DWORD dwFlags = 0, dwSize = sizeof(dwFlags);

            if(pDlgInfo->hInternetMapped)
            {
                InternetQueryOption(pDlgInfo->hInternetMapped,
                                 INTERNET_OPTION_SECURITY_FLAGS,
                                 &dwFlags, 
                                 &dwSize);
            }
            
            if(dwFlags & DLG_FLAGS_SEC_CERT_REV_FAILED)
                pDlgInfo->dwDlgFlags |= DLG_FLAGS_IGNORE_FAILED_REVOCATION;
        }

        fRet = TRUE;

        // set this dialog as foreground if necessary
        if(pDlgInfo->dwDlgFlags & DLG_FLAGS_BRING_TO_FOREGROUND)
            SetForegroundWindow(hwnd);

        break;

    case WM_COMMAND:
        {
            WORD    wID = LOWORD(wparam);

            pDlgInfo = (PERRORINFODLGTYPE) GetWindowLongW(hwnd,DWL_USER);
            switch(wID)
            {
            case IDOK:
            case IDYES:
                ASSERT(pDlgInfo);
                ASSERT(pDlgInfo->dwDlgId != 0);

                //  Save flags,and change any global vars,and registry values if needed.
                if(pDlgInfo->hInternetMapped)
                {
                    VERIFY(InternetSetOptionW(pDlgInfo->hInternetMapped,
                                             INTERNET_OPTION_SECURITY_FLAGS,
                                             &(pDlgInfo->dwDlgFlags), 
                                             sizeof(pDlgInfo->dwDlgFlags)));
                }


                // If the user checked the "overide" check-box
                //  let us map it,and force a general
                //  override of all errors of this type.

                if(SendDlgItemMessageW(hwnd,IDC_DONT_WANT_WARNING,BM_GETCHECK,(WPARAM)0,(LPARAM)0)==BST_CHECKED)
                    UpdateGlobalSecuritySettings(pDlgInfo->dwDlgId,pDlgInfo->dwDlgFlags);

                EndDialog(hwnd,TRUE);
                break;

            case IDCANCEL:
            case IDNO:
                EndDialog(hwnd,FALSE);
                break;
            }
            fRet = TRUE;
            break;
        }
    }
    return fRet;
}

/*
Routine Description:
    Maps a Wininet Error Code to an internal Dlg Resource Id.

Arguments:
    dwError     -   A Wininet defined error code with an expected
                    assoicated dlg.
    lpdwDlgId   -   Pointer to location where Dlg Id result will be returend.
                    This ID can be used for creating a Dlg Resource.
    lpdwDlgFlags-   Pointer to DWORD flags used to store various capiblites
                    for Dialog.

Return Value:
    DWORD
        Success - ERROR_SUCCESS
        Failure - ERROR_INVALID_PARAMETER

Comments:
    none.
*/
DWORD MapWininetErrorToDlgId(IN  DWORD       dwError,
                             OUT LPDWORD     lpdwDlgId,
                             OUT LPDWORD     lpdwDlgFlags,
                             OUT DLGPROC     *ppDlgProc)
{
    typedef struct
    {
        DWORD   dwWininetError;
        DWORD   dwDlgId;
        DLGPROC pDlgProc;
        DWORD   dwDlgFlags;
    } ErrorToDlgIdMappingType;

    ErrorToDlgIdMappingType MapErrorToDlg[] = 
    {
        { ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR,   IDD_HTTPS_TO_HTTP_ZONE_CROSSING,OkCancelDialogProc,0},
        { ERROR_INTERNET_MIXED_SECURITY,           IDD_MIXED_SECURITY,             OkCancelDialogProc,0},
        { ERROR_HTTP_REDIRECT_NEEDS_CONFIRMATION,  IDD_HTTP_POST_REDIRECT,         OkCancelDialogProc,0},
        { ERROR_INTERNET_CHG_POST_IS_NON_SECURE,   IDD_WARN_ON_POST,               OkCancelDialogProc,0},
        { ERROR_INTERNET_POST_IS_NON_SECURE,       IDD_WARN_ON_POST,               OkCancelDialogProc,0},

        { ERROR_INTERNET_HTTPS_HTTP_SUBMIT_REDIR,  IDD_HTTPS_TO_HTTP_SUBMIT_REDIRECT,OkCancelDialogProc,0},
        { ERROR_INTERNET_SEC_CERT_ERRORS,          IDD_SEC_CERT_ERRORS,            OkCancelDialogProc,DLG_FLAGS_CAN_HAVE_CERT_INFO},
        { ERROR_INTERNET_SEC_CERT_REV_FAILED,      IDD_REVOCATION_PROBLEM,         OkCancelDialogProc,DLG_FLAGS_CAN_HAVE_CERT_INFO},

        // because URLMON sets INTERNET_ERROR_MASK_COMBINED_SEC_CERT for us
        // we will never see these-- they are grouped into SEC_CERT_ERRORS
//      { ERROR_INTERNET_INVALID_CA,               IDD_INVALID_CA,                 OkCancelDialogProc,(DLG_FLAGS_CAN_HAVE_CERT_INFO | DLG_FLAGS_IGNORE_INVALID_CA)},
//      { ERROR_INTERNET_SEC_CERT_CN_INVALID,      IDD_BAD_CN,                     OkCancelDialogProc,(DLG_FLAGS_CAN_HAVE_CERT_INFO | DLG_FLAGS_IGNORE_CERT_CN_INVALID)},
//      { ERROR_INTERNET_SEC_CERT_DATE_INVALID,    IDD_CERT_EXPIRED,               OkCancelDialogProc,(DLG_FLAGS_CAN_HAVE_CERT_INFO | DLG_FLAGS_IGNORE_CERT_DATE_INVALID)},

//#ifndef WINCE   // leave these in here just for the record
//      { ERROR_INTERNET_BAD_AUTO_PROXY_SCRIPT,    IDD_SCRIPT_ERROR,               OkCancelDialogProc,0},
//      { ERROR_INTERNET_UNABLE_TO_DOWNLOAD_SCRIPT,IDD_FAILED_DOWNLOAD,            OkCancelDialogProc,(DLG_FLAGS_BRING_TO_FOREGROUND)},
//#endif
    };


    ASSERT(lpdwDlgId);
    ASSERT(lpdwDlgFlags);

    *lpdwDlgId    = 0;
    *lpdwDlgFlags = 0;
    *ppDlgProc    = 0;

    for( DWORD i = 0; i < ARRAYSIZE(MapErrorToDlg); i++ )
    {
        if(  dwError == MapErrorToDlg[i].dwWininetError )
        {
            *lpdwDlgId    = MapErrorToDlg[i].dwDlgId;
            *lpdwDlgFlags = MapErrorToDlg[i].dwDlgFlags;
            *ppDlgProc    = MapErrorToDlg[i].pDlgProc;
            return ERROR_SUCCESS;
        }
    }
    ASSERT(FALSE);
    RETAILMSG(1, (TEXT("BAD NEWS: We got a WININET error code 0x%x that we don't have a dialog for!!!"),
                 dwError));
    return ERROR_INVALID_PARAMETER;
}



/////////////////////////////////////////////////////////////////////////////
// LaunchWininetDialog
/////////////////////////////////////////////////////////////////////////////
DWORD LaunchWininetDialog
(
    HWND    hwnd,
    DWORD   dwError,
    LPVOID  lpInfo
)
{
    DWORD dwRet = ERROR_CALL_NOT_IMPLEMENTED;

    PERRORINFODLGTYPE   pDlgInfo = (PERRORINFODLGTYPE)lpInfo;


    switch (dwError)
    {
        //
        // NOTE NOTE!  If you remove any of these codes we handle w/o UI
        // then you will have to add UI resources to display the error
        //
        case ERROR_HTTP_COOKIE_NEEDS_CONFIRMATION:
        {
            // decline
            dwRet = ERROR_CANCELLED;
            break;
        }
        
        case ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR:
        {
            // accept
            dwRet = ERROR_SUCCESS;
            break;
        }

        default:
            DLGPROC dlgProc;

            if ( ERROR_SUCCESS == MapWininetErrorToDlgId(dwError, &pDlgInfo->dwDlgId, &pDlgInfo->dwDlgFlags, &dlgProc) )
            {
                if ( TRUE == DialogBoxIndirectParamW(HINST_RESDLL,
                                (LPCDLGTEMPLATEW)LoadResource(HINST_RESDLL,FindResourceW(HINST_RESDLL,MAKEINTRESOURCEW(pDlgInfo->dwDlgId),RT_DIALOG)),
                                SHGetLastActiveWindow(hwnd),(DLGPROC)dlgProc,(LPARAM)lpInfo) )
                {
                    dwRet = ERROR_SUCCESS;
                }
                else
                {
                    dwRet = ERROR_CANCELLED;
                }
            }
            else
            {
                // this is REALLY bad, we encountered a WININET error we didn't have
                // dialogs for-- but it's better to be safe and cancel than let the 
                // user do something unsecure

                // should we have this registry enabled?
                // just in the small chance that we'd be denying some important site
                dwRet = ERROR_CANCELLED;
            }
            break;
    }

    return dwRet;
}
