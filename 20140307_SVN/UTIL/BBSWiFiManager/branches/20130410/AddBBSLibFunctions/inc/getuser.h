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
// Header declarations for getuser code.
#pragma once

typedef struct _USERPWD_DLGINFO
{
    HWND           *phDlg;
    PNETUI_USERPWD pUserPwd;
} USERPWD_DLGINFO;

#define MAX_TIMEOUT 4*60*1000  // 4 minutes

// Globals that define TPC/PPC platform differences
BOOL g_fDestroyDetailsFonts;
BOOL g_fDestroyUserPwdFonts;

extern const BOOL gc_fDrawHeaderLine;
extern const BOOL gc_fIsHelpPresent;
extern const BOOL gc_fHandleBackKey;
extern const BOOL gc_fIsSipPresent;
extern const BOOL gc_fRunFontManager;

// Font\color table for Network Log On dialogs.
extern const CCtlTextDesc gc_rgTextDescLogOn[];
// Size of the font/color table
extern const size_t gc_crgTextDescLogOn;

HRESULT CachedCredentialDomainUser(__in LPCTSTR pszTarget, UINT cchTarget,
                                   __out_ecount(cchDomain) LPTSTR pszDomain, UINT cchDomain,
                                   __out_ecount(cchUser) LPTSTR pszUser, UINT cchUser,
                                   BOOL *pfFoundMatch);

HRESULT CacheDomainCredential(__in LPCTSTR pszTarget, UINT cchTarget,
                              __in LPCTSTR pszDomain,
                              __in LPCTSTR pszUserName,
                              __in LPCTSTR pszPass, UINT cchPass);

void GetServerFromResource(PWCHAR resource, PWCHAR server, PDWORD len);

BOOL DisallowSavedPasswords();

// Functions used by the UserPassword dialog
BOOL OnUserPwdInitDialog(HWND hDlg, HWND hwndMenuBar);
BOOL OnServerLogon(HWND hDlg);
BOOL OnUserPwdInactivate(HWND hDlg, HWND hwndActive);

// Functions used by the Details dialog
BOOL OnDetailsInitDialog(HWND hDlg, HWND hwndMenuBar);

//functions used by GetUsernamePasswordEx
HWND GetUsernamePwdDlgParent(HWND hParent);

