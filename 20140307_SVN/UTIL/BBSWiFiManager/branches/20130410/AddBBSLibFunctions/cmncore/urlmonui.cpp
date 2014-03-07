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
//  URLMONUI.CPP
//
//  Description:
//      replacement dialogs for URLMON
//
//
/////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <urlmon.h>
#include <urlmonui.h>

#include "wpcpriv.h"
#include "netuip.h"
#include "resource.h"

enum eUMUITYPE {
    UMUI_ALERT = 0,
    UMUI_ALERT_FORMS
};

typedef struct tagUMDlgParams {

    eUMUITYPE   eDlgType;
    LPCTSTR     pszMsg;

} *PUMDLGPARAMS, UMDLGPARAMS;

eUMUITYPE
GetDialogType(DWORD dwAction, DWORD dwPermissions)
{
    ASSERT(dwPermissions != URLPOLICY_DISALLOW);

    if ( dwAction == URLACTION_HTML_SUBMIT_FORMS ||
         dwAction == URLACTION_HTML_SUBMIT_FORMS_FROM ||
         dwAction == URLACTION_HTML_SUBMIT_FORMS_TO
       )
        return UMUI_ALERT_FORMS;
    else
        return UMUI_ALERT;

}

INT_PTR 
UrlmonDialogProc
(
    HWND hDlg,
    UINT iMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    BOOL fRet = FALSE;

    if ( iMsg != WM_INITDIALOG )
    {
        fRet = SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(IDD_URLMON),
                              hDlg, iMsg, wParam, lParam );
    }

    switch(iMsg)
    {
        case WM_INITDIALOG:
        {
            //
            // Based on the dialog type, we show:
            // * an icon 
            // * a checkbox saying "don't tell me again"
            // 
            PUMDLGPARAMS    pumdlgparams = (PUMDLGPARAMS)lParam;
            ASSERT(pumdlgparams);

            SHINITDLGINFO shidi;
            SHMENUBARINFO   mbi;

            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
            shidi.hDlg = hDlg;
            SHInitDialog(&shidi);

            // softkeys
            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hDlg;
            mbi.nToolBarId = IDR_INETUI_DIALOGYESNO;
            mbi.hInstRes = HINST_RESDLL;
            SHCreateMenuBar(&mbi);

            if ( NULL != pumdlgparams )
            {
                SetDlgItemText(hDlg, IDC_URLMON_ALERT_TEXT, pumdlgparams->pszMsg);
            }

            // if this is a normal policy warning, don't show the DONTWARN checkbox
            if ( NULL != pumdlgparams && pumdlgparams->eDlgType == UMUI_ALERT )
            {
                ShowWindow(GetDlgItem(hDlg, IDC_DONT_WANT_WARNING), SW_HIDE);
            }

            SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(IDD_URLMON),
                                  hDlg, iMsg, wParam, lParam );

            fRet = TRUE;
            break;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                case IDYES:
                {
                    DWORD dwRet;
                    if(IsWindowVisible(GetDlgItem(hDlg, IDC_DONT_WANT_WARNING)) && 
                       SendDlgItemMessageW(hDlg,IDC_DONT_WANT_WARNING,BM_GETCHECK,0, 0)== BST_CHECKED)
                        dwRet = ZALERT_YESPERSIST;
                    else 
                        dwRet = ZALERT_YES;

                    EndDialog(hDlg,dwRet);
                    fRet = TRUE;
                    break;
                }

                case IDCANCEL:
                case IDNO:
                    EndDialog(hDlg,ZALERT_NO);
                    fRet = TRUE;
                    break;
            }
            break;
    }  /* end switch */
    return fRet;

}

/////////////////////////////////////////////////////////////////////////////
// LaunchUrlmonDialog
/////////////////////////////////////////////////////////////////////////////
int LaunchUrlmonDialog
(
    IN HWND     hWnd,
    IN DWORD    dwAction,
    IN DWORD    dwPermissions,
    IN LPCTSTR  lpszText
)
{
    int iRet = ERROR_CALL_NOT_IMPLEMENTED;

    UMDLGPARAMS umparams;
    
    umparams.eDlgType   = GetDialogType(dwAction, dwPermissions);
    umparams.pszMsg     = lpszText;

    iRet = DialogBoxIndirectParamW(HINST_RESDLL,
                                   (LPCDLGTEMPLATEW)LoadResource(HINST_RESDLL,
                                                    FindResourceW(HINST_RESDLL,
                                                        MAKEINTRESOURCEW(IDD_URLMON),
                                                        RT_DIALOG)),
                                   SHGetLastActiveWindow(hWnd), 
                                   (DLGPROC)UrlmonDialogProc, 
                                   (LPARAM)&umparams);

    return iRet;
}
