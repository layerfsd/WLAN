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
//      MDD dialogs for WININET - PPC specific functions
//
//
/////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <wininet.h>
#include <wininetui.h>
#include <wpcpriv.h>
#include <appmet.h>

#include "netuip.h"
#include <resource.h>

#include "winetui.h"


BOOL SetCertDlgItem(HWND hDlg,
                    DWORD dwIconCtl,
                    DWORD dwTextCtl,
                    DWORD dwString,
                    BOOL  fError)
{
    ASSERT(hDlg);

    // Load small icon and set it
    HICON hicon = (HICON)LoadImageW(HINST_RESDLL,
                        MAKEINTRESOURCEW(fError ? IDI_WARN : IDI_SUCCESS),
                        IMAGE_ICON, AM_SM_CXSMICON, AM_SM_CYSMICON, 0);

    if(hicon)
    {
        HICON hiconOld = (HICON)SendDlgItemMessageW(hDlg,dwIconCtl,
                                STM_SETIMAGE,(WPARAM)IMAGE_ICON,
                                (LPARAM)hicon);

        if(hiconOld)
            DestroyIcon(hiconOld);
    }

    // The dialog displays the error string by default.  Replace this with the
    // success string if an error didn't occur.
    if(!fError)
    {
        WCHAR sz[512];
        if(LoadStringW(HINST_RESDLL,dwString,sz,ARRAYSIZE(sz)))
            SetDlgItemTextW(hDlg,dwTextCtl,sz);
    }
    return TRUE;
}

BOOL InitSecCertErrorsDlg(HWND hDlg,PERRORINFODLGTYPE pDlgInfo)
{
    ASSERT(pDlgInfo);

    // Get the errors that occured from the hInternetMapped object.
    DWORD    dwFlags;

    if(pDlgInfo->hInternetMapped)
    {
        DWORD   dwSize = sizeof(dwFlags);

        if ( !InternetQueryOption(pDlgInfo->hInternetMapped,
                         INTERNET_OPTION_SECURITY_FLAGS,
                         &dwFlags, 
                         &dwSize) )
        {
            dwFlags = (DWORD)-1;   // Display all errors
        }

    }
    else
        dwFlags = (DWORD)-1; // Display all errors.

    // If an error occured set the ignore flag so if the users selects to bypass
    // this error it gets ignored the next time through.  Then initialize the
    // dialog icons and text.
    if(dwFlags & DLG_FLAGS_INVALID_CA)
        pDlgInfo->dwDlgFlags |= DLG_FLAGS_IGNORE_INVALID_CA;

    SetCertDlgItem(hDlg,IDC_CERT_TRUST_ICON,IDC_CERT_TRUST_TEXT,
                        IDS_CERT_TRUST,dwFlags & DLG_FLAGS_INVALID_CA);

    if(dwFlags & DLG_FLAGS_SEC_CERT_DATE_INVALID)
        pDlgInfo->dwDlgFlags |= DLG_FLAGS_IGNORE_CERT_DATE_INVALID;

    SetCertDlgItem(hDlg,IDC_CERT_DATE_ICON,IDC_CERT_DATE_TEXT,
                        IDS_CERT_DATE,dwFlags & DLG_FLAGS_SEC_CERT_DATE_INVALID);

    if(dwFlags & DLG_FLAGS_SEC_CERT_CN_INVALID)
        pDlgInfo->dwDlgFlags |= DLG_FLAGS_IGNORE_CERT_CN_INVALID;

    SetCertDlgItem(hDlg,IDC_CERT_NAME_ICON,IDC_CERT_NAME_TEXT,
                        IDS_CERT_NAME,dwFlags & DLG_FLAGS_SEC_CERT_CN_INVALID);

    return TRUE;
}
