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

#include <windows.h>
#include <wpcpriv.h>
#include <cred.h>
#include <ehm.h>
#include <miscsvcs_priv.h>
#include <assert.h>
#include <tpcshell.h> 
#include "netui.h"
#include "netuip.h"
#include "getuser.h"
#include <resource.h>
#include "cmnutils.h"
#include "encrypt.h"

const TCHAR c_szHelpApp[] = TEXT("peghelp");
const TCHAR c_szLogonNoPasswordHelpLink[] =
        TEXT("file:connect.htm#logonnopassword");
const TCHAR c_szLogonPasswordHelpLink[] =
        TEXT("file:connect.htm#logonpassword");

// split a string of the form domain\user into domain and user components
// if string is of the form user then domain is set to blank
// on input cchDomain and cchUser contain buffer sizes

static HRESULT SplitDomainUser(__in LPCTSTR pszSource,
                               __out_ecount(cchDomain) LPTSTR pszDomain, UINT cchDomain,
                               __out_ecount(cchUser) LPTSTR pszUser, UINT cchUser)
{
    HRESULT hr = S_OK;
    LPCTSTR pszSep;
    LPTSTR  pszUserEnd;

    // Is there a separator?
    pszSep = _tcschr(pszSource, TEXT('\\'));
    if (NULL != pszSep)
    {
        // Copy the domain portion
        hr = StringCchCopyN(pszDomain, cchDomain, pszSource, pszSep - pszSource);
        CHR(hr);

        // Copy the user portion
        hr = StringCchCopyEx(pszUser, cchUser, pszSep + 1, &pszUserEnd, NULL, 0);
        CHR(hr);
    }
    else
    {
        // The domain is blank
        CBREx(0 != cchDomain, STRSAFE_E_INSUFFICIENT_BUFFER);
        pszDomain[0] = TEXT('\0');

        // The entire string goes into the user buffer
        hr = StringCchCopyEx(pszUser, cchUser, pszSource, &pszUserEnd, NULL, 0);
        CHR(hr);
    }

Error:
    return hr;
}

// Make a string of the form domain\user from the input
// If user is null/empty then empty string is returned
// If domain is null/empty then just user is returned
// On input cchBuffer contains length of dest buffer

static HRESULT MakeDomainUser(__out_ecount(cchBuffer) LPTSTR pszBuffer, size_t cchBuffer,
                              __in __opt LPCTSTR pszDomain, __in __opt LPCTSTR pszUser, __out UINT *pcchLength)
{
    HRESULT hr = S_OK;
    LPTSTR  pszOrig = pszBuffer;

    // If there's no username, we return a blank string
    if (NULL == pszUser || TEXT('\0') == pszUser[0])
    {
        CBREx(0 != cchBuffer, STRSAFE_E_INSUFFICIENT_BUFFER);
        pszBuffer[0] = TEXT('\0');
        CBREx(FALSE, S_FALSE);
    }

    // Do we have a domain name?
    if (NULL != pszDomain && TEXT('\0') != pszDomain[0])
    {
        // Copy the domain name
        hr = StringCchCopyEx(pszBuffer, cchBuffer, pszDomain, &pszBuffer, &cchBuffer, 0);
        CHR(hr);

        // Append a slash
        CBREx(1 < cchBuffer, STRSAFE_E_INSUFFICIENT_BUFFER);
        *pszBuffer++ = TEXT('\\');
        *pszBuffer = TEXT('\0');
        --cchBuffer;
    }

    // Copy the username
    hr = StringCchCopyEx(pszBuffer, cchBuffer, pszUser, &pszBuffer, NULL, 0);
    CHR(hr);

    // Return the updated length
    *pcchLength = pszBuffer - pszOrig;

Error:
    return hr;
}

// Get the cached domain credential
// First we see if we can find a credential for the explicit target
// If not then try for target being blank L""
// pFoundMatch is set to TRUE if exact target match is found
// On input pDestLen contains length of dest buffer

HRESULT CachedCredentialDomainUser(__in LPCTSTR pszTarget, UINT cchTarget,
                                   __out_ecount(cchDomain) LPTSTR pszDomain, UINT cchDomain,
                                   __out_ecount(cchUser) LPTSTR pszUser, UINT cchUser,
                                   BOOL *pfFoundMatch)
{
    HRESULT hr = S_OK;
    UINT    cchTargetPlus;
    PCRED pCred = NULL;
    DWORD dwErr = ERROR_INTERNAL_ERROR;
    PWCHAR domainuser;

    // Check for integer overflow
    CBRAEx(UAdd(cchTarget, 1, &cchTargetPlus), E_INVALIDARG);

    // Read the domain credential corresponding to the target
    dwErr = CredRead((LPTSTR)pszTarget,
                        cchTargetPlus,
                        CRED_TYPE_DOMAIN_PASSWORD,
                        CRED_FLAG_NO_DEFAULT|CRED_FLAG_NO_IMPLICIT_DEFAULT,
                        &pCred);

    if(ERROR_SUCCESS == dwErr && NULL != pCred){
        // read the domain/user for the cached cred
        *pfFoundMatch = TRUE;
        domainuser = pCred->wszUser;
    }else{
        *pfFoundMatch = FALSE;
        // Is there a default credential
        dwErr = CredRead(L"",
                            1,
                            CRED_TYPE_DOMAIN_PASSWORD,
                            CRED_FLAG_NO_DEFAULT|CRED_FLAG_NO_IMPLICIT_DEFAULT,
                            &pCred);
        if(ERROR_SUCCESS == dwErr && NULL != pCred){
            // read the domain/user for the cached cred
            domainuser = pCred->wszUser;
        }else{
            // no cached creds
            domainuser = L"";
        }
    }

    hr = SplitDomainUser(domainuser, pszDomain, cchDomain, pszUser, cchUser);
    CHR(hr);

Error:
    if (NULL != pCred)
    {
        CredFree((PBYTE)pCred);
    }

    return hr;
}

// Cache a credential for the specified target
// The credential cached is of type CRED_TYPE_DOMAIN_PASSWORD
// The credential is persisted 

HRESULT CacheDomainCredential(__in LPCTSTR pszTarget, UINT cchTarget,
                              __in LPCTSTR pszDomain,
                              __in LPCTSTR pszUserName,
                              __in LPCTSTR pszPass, UINT cchPass)
{
    HRESULT hr = S_OK;
    CRED    cred;
    TCHAR   szUser[UNLEN+DNLEN+2];
    UINT    cchUser;
    DWORD   dwErr;

    // We need to cast a DWORD pointer to a UINT pointer below
    CASSERT(sizeof(DWORD) == sizeof(UINT));

    // Combine the domain and username strings into a user string
    hr = MakeDomainUser(szUser, ARRAYSIZE(szUser), pszDomain, pszUserName, &cchUser);
    CHR(hr);

    // Set up the credential
    cred.dwVersion = CRED_VER_1;
    cred.dwType = CRED_TYPE_DOMAIN_PASSWORD;
    cred.wszTarget = (LPTSTR)pszTarget;
    cred.dwFlags = CRED_FLAG_DEFAULT|CRED_FLAG_PERSIST;
    cred.wszUser = szUser;
    cred.dwUserLen = cchUser + 1;  // safe from overflow
    cred.pBlob = (BYTE*)pszPass;

    // Check for integer overflow
    CBRAEx(UAdd(cchTarget, 1, (UINT*)&cred.dwTargetLen), E_INVALIDARG);
    if (NULL != pszPass)
    {
        UINT cchPassPlus;

        CBRAEx(UAdd(cchPass, 1, &cchPassPlus), E_INVALIDARG);
        CBRAEx(CharsToBytes(cchPassPlus, (UINT*)&cred.dwBlobSize), E_INVALIDARG);
    }
    else
    {
        cred.dwBlobSize = 0;
    }

    // Write the credential
    dwErr = CredWrite(&cred, 0);
    CBREx(ERROR_SUCCESS == dwErr, HRESULT_FROM_WIN32(dwErr));

Error:
    return hr;
}


// Parse resource name of form \\server\resource into server
// on input len contains size of server buffer in chars
// on output len contains lenght of server string
// if server buffer is not big enough a truncated string is returned
// output string is always null terminated

void GetServerFromResource(PWCHAR resource, PWCHAR server, PDWORD len){
    PWCHAR t = NULL;
    DWORD i = 0;

    PREFAST_ASSERT(resource != NULL);
    PREFAST_ASSERT(server != NULL);
    PREFAST_ASSERT(len != NULL);

    // skip initial slashes
    t = resource;
    while(*t == L'\\' && *t != L'\0'){
        //skip
        t++;
    }

    if(*t == L'\0'){
        // we reached the end. so input string has all slashes.
        // output string is empty
        *server = L'\0';
        *len = 0;
        return;
    }

    // we are looking at first non slash char
    // copy all characters until another slash is encountered
    // or end of source string
    // or end of space in dest string
    i = 0;
    while(*t != L'\\' && *t != L'\0' && i < *len-1){
        server[i] = *t;
        t++;
        i++;
    }

    server[i] = L'\0';
    *len = i;

    return;
}


BOOL GetUsernamePassword(HWND hParent, PNETUI_USERPWD pUserPwd)
{
    return GetUsernamePasswordEx(hParent, pUserPwd, NULL);
}


BOOL
CloseUsernamePasswordDialog(
    IN   HWND hDlg)
//
//  Abort an attempt to have the user input data into a dialog, closing the dialog.
//
{
    BOOL bWorked;

    bWorked = EndDialog(hDlg, 0);

    DEBUGMSG (ZONE_WARN && !bWorked, (TEXT("NETUI: WARNING - EndDialog of UsernamePassword dialog failed, e=%u\n"), GetLastError()));

    return bWorked;
}

BOOL DisallowSavedPasswords()
{
    DWORD dwError;
    HKEY hRegKey;
    DWORD dwType;
    DWORD fDisallow = 0;
    DWORD dwSize;
    if ( ERROR_SUCCESS == ( dwError = RegOpenKeyEx (
                               HKEY_LOCAL_MACHINE,
                               L"Comm\\Security",
                               0,
                               KEY_READ,
                               &hRegKey ) ) )
    {
        dwSize = sizeof(fDisallow);
        dwError = RegQueryValueExW(
                               hRegKey,
                               L"DisallowSavedNetworkPasswords",
                               NULL,
                               &dwType,
                               (PBYTE)&fDisallow,
                               &dwSize );
        if (ERROR_SUCCESS == dwError)
        {
        DEBUGMSG (ZONE_USERPWD, (TEXT("DlgProcUserPwd: DisallowSavedNetworkPasswords = %d\r\n"), fDisallow));
        }
        RegCloseKey(hRegKey);
    }
    return BOOLIFY(fDisallow);
}


// ----------------------------------------------------------------
//
//  Details dialog box.
//
//  Called when site/proxy string is too long to fit in the username
//  password dialog and requires a separate "Details" display dialog.
//
// ----------------------------------------------------------------

BOOL CALLBACK DlgProcDetails(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PNETUI_USERPWD pUserPwd;
    static  void*   s_pvFontManager;
    ULONG ulFontMgrRetVal;
    
    if (gc_fRunFontManager)
    {
        // FONT HANDLING STUFF
        // set font size, weight, and color per the table gs_rgTextDescLogOn
        if (SHRunFontManager(gc_rgTextDescLogOn,
                         gc_crgTextDescLogOn, // ARRAYSIZEOF(gc_rgTextDescLogOn)
                         &s_pvFontManager,
                         uMsg,
                         hDlg,
                         wParam,
                         lParam,
                         &ulFontMgrRetVal) == CTLTEXT_EXIT )
        {
            return (BOOL)ulFontMgrRetVal;
        }
    }
    
    switch (uMsg) 
    {            
        case WM_INITDIALOG :
        {
            SHINITDLGINFO shidi;
            SHMENUBARINFO mbi;
            UINT  uID;

            // Size the dialog
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
            shidi.hDlg = hDlg;
            SHInitDialog(&shidi);

            // Menubar for dialog
            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize      = sizeof(SHMENUBARINFO);
            mbi.hwndParent  = hDlg;
            mbi.nToolBarId  = IDR_NETUI_DONE_BAR;
            mbi.hInstRes    = HINST_RESDLL;
            SHCreateMenuBar(&mbi);
        
            pUserPwd = (PNETUI_USERPWD) lParam;

            // Load string for static label "Site Name:" or "Proxy Name:"
            uID = pUserPwd->dwFlags & NETUI_USERPWD_WININETPROXY ? IDS_NETUI_WININET_PROXY_NAME 
                                                                 : IDS_NETUI_WININET_SITE_NAME;
            SetDlgItemText(hDlg, IDC_NETUI_PWD_DLGTITLE, (LPCTSTR)LoadString(HINST_RESDLL, uID, NULL, 0));
            
            // Show site or proxy information
            SetDlgItemText(hDlg, IDC_NETUI_SERVER_OR_PROXY,pUserPwd->szResourceName);

            // Stretch the controls to fit the screen
            SHResizeDialogProc(hDlg, uMsg, wParam, lParam);

            // Any custom initialization
            OnDetailsInitDialog(hDlg, mbi.hwndMB);
            break;
        }
        
        case WM_COMMAND:
            switch ( LOWORD(wParam) ) 
            {
                case IDOK:
                case IDCANCEL:
                case IDM_DONE:
                    EndDialog(hDlg, FALSE); 
                    return TRUE;
                default:
                    break;
            }
            break;
        
        case WM_DESTROY:
            SHResizeDialogProc(hDlg, uMsg, wParam, lParam);
            if (g_fDestroyDetailsFonts)
            {
                HFONT hFont = NULL;
                // Free our font
                hFont = (HFONT) SendDlgItemMessage(hDlg, IDC_NETUI_PWD_DLGTITLE, WM_GETFONT, 0, 0);
                if (hFont)
                {
                    DeleteObject(hFont);
                }
            }
            break;

        case WM_ACTIVATE:
            // If we lost activation, we should dismiss ourselves
            if (LOWORD(wParam) == WA_INACTIVE)
            {
                PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
            }
            break;

        case WM_SIZE:
            // Stretch the controls in these dialogs
            SHResizeDialogProc(hDlg, uMsg, wParam, lParam);
            break;

        case WM_PAINT:
            if (gc_fDrawHeaderLine)
            {
                HDC hdcPaint;
                PAINTSTRUCT     PaintStruct;

                // Get the device context.
                hdcPaint = BeginPaint(hDlg, &PaintStruct);

                // Paint the line under the header text.
                SHHdrGrpSepLineDraw(hDlg, NULL, hdcPaint, SHD_HEADER);
                EndPaint(hDlg, &PaintStruct);
             }
             break;

        case WM_HOTKEY:
            // Handle backspace hotkey
            if ((HIWORD(lParam) == VK_TBACK) && gc_fHandleBackKey)
            {
                SHSendBackToFocusWindow(uMsg, wParam, lParam);
            }
            break;
    }
    return FALSE;
}

// ----------------------------------------------------------------
//
//  Username Password dialog box.
//
//  Called by PPP to prompt for the username on a password
//  failure. Also used by redir to logon to a network server.
//
// ----------------------------------------------------------------
BOOL CALLBACK
DlgProcUserPwd( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    BOOL            fDisallowSavedPasswords;
    USERPWD_DLGINFO *pDlgInfo;
    PNETUI_USERPWD  pUserPwd;
    WCHAR server[RMLEN] = L"";
    DWORD serverLen = RMLEN;
    BOOL bFoundMatch = FALSE;
    static BOOL fReEnable = FALSE;
    static  void*   s_pvFontManager;
    ULONG ulFontMgrRetVal;
    
    if (gc_fRunFontManager)
    {
        // FONT HANDLING STUFF
        // set font size, weight, and color per the table gs_rgTextDescLogOn
        if (SHRunFontManager(gc_rgTextDescLogOn,
                         gc_crgTextDescLogOn, // ARRAYSIZEOF(gc_rgTextDescLogOn)
                         &s_pvFontManager,
                         uMsg,
                         hDlg,
                         wParam,
                         lParam,
                         &ulFontMgrRetVal) == CTLTEXT_EXIT )
        {
            return (BOOL)ulFontMgrRetVal;
        }
    }

    DEBUGMSG (0, (TEXT("DlgProcUserPwd: Got message 0x%x\r\n"),uMsg));
    switch( uMsg )
    {
        case WM_INITDIALOG:
        {    
            SHINITDLGINFO   shidi;
            SHMENUBARINFO   mbi;
            BOOL bEllipsized = FALSE;
            TCHAR szResourceName[RMLEN];    
            HWND hCtrl = NULL;

            DEBUGMSG (ZONE_USERPWD, (TEXT("DlgProcUserPwd: INITDIALOG\r\n")));
            pDlgInfo = (USERPWD_DLGINFO*)lParam;
            pUserPwd = pDlgInfo->pUserPwd;
            SetWindowLong (hDlg, DWL_USER, (LONG)pUserPwd);
            if (NULL != pDlgInfo->phDlg)
            {
                *pDlgInfo->phDlg = hDlg;
            }

            // Size the dialog.
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_WANTSCROLLBAR;
            shidi.hDlg = hDlg;
            SHInitDialog(&shidi);
    
            if (pUserPwd->dwFlags & NETUI_USERPWD_WININET)
            {
                // IE like auth dialogs showing Site or Proxy information
                // Does the site name fit?
                HWND hCtrl = NULL;
                RECT rcCtrl;
                hCtrl = GetDlgItem(hDlg, IDC_NETUI_SERVER_OR_PROXY);
                GetClientRect(hCtrl, &rcCtrl);
                if (S_OK == PreEllipsizeString(pUserPwd->szResourceName, szResourceName, ARRAYSIZE(szResourceName), RECTWIDTH(rcCtrl) - 2*GetSystemMetrics(SM_CXBORDER)))
                {
                    bEllipsized = TRUE;
                }
            }

            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hDlg;
            mbi.nToolBarId = bEllipsized ? IDR_NETUI_DETAILS_BAR : IDR_NETUI_BAR_DIALOGOKCANCEL;
            mbi.hInstRes = HINST_RESDLL;
            
            SHCreateMenuBar(&mbi);
    
            // For connecting to net server, we modify the dialog a little
            if (pUserPwd->dwFlags & NETUI_USERPWD_WININET)
            {
                // IE like auth dialogs
                UINT  uID = 0;

                uID = pUserPwd->dwFlags & NETUI_USERPWD_WININETPROXY ? IDS_NETUI_WININET_PROXY:IDS_NETUI_WININET_SITE;
                SetDlgItemText(hDlg, IDC_NETUI_SITE_OR_FIREWALL, (LPTSTR)LoadString(HINST_RESDLL, uID, NULL, 0));
    
                SetDlgItemText(hDlg, IDC_NETUI_SERVER_OR_PROXY, bEllipsized ? szResourceName : pUserPwd->szResourceName);

                if (pUserPwd->dwFlags & NETUI_USERPWD_REALM)
                {
                    SetDlgItemText(hDlg, IDC_NETUI_REALM_FIELD, pUserPwd->szRealm);
                }
            }
            else if (pUserPwd->dwFlags & NETUI_USERPWD_SERVERLOGON)
            {
                OnServerLogon(hDlg);
            }

            // For non-realm cases
            if(!(pUserPwd->dwFlags & NETUI_USERPWD_REALM)) 
            {
                // if the caller hasn't supplied a username and password and there is a system default, use that.
                if(pUserPwd->szUserName[0] == TEXT('\0') && pUserPwd->szDomain[0] == TEXT('\0'))
                {
                    // Get the server name from resource
                    GetServerFromResource(pUserPwd->szResourceName,server,&serverLen);            

                    // Get the cached credential
                    CachedCredentialDomainUser(server, serverLen,
                        pUserPwd->szDomain, ARRAYSIZE(pUserPwd->szDomain), 
                        pUserPwd->szUserName, ARRAYSIZE(pUserPwd->szUserName),
                        &bFoundMatch);
                }
            }

            SetDlgItemText( hDlg, IDC_NETUI_USERNAME, pUserPwd->szUserName );
            SetDlgItemText( hDlg, IDC_NETUI_PASSWORD, pUserPwd->szPassword );
            if(!(pUserPwd->dwFlags & NETUI_USERPWD_REALM))
            {
                SetDlgItemText( hDlg, IDC_NETUI_DOMAIN, pUserPwd->szDomain );
            }
    
            if (pUserPwd->szUserName[0] == TEXT('\0') || (pUserPwd->dwFlags & NETUI_USERPWD_NOPWD))
            {
                SetFocus(GetDlgItem(hDlg, IDC_NETUI_USERNAME));
            }
            else
            {
                SetFocus(GetDlgItem(hDlg, IDC_NETUI_PASSWORD));
                // Select the password text
                SendMessage(GetDlgItem(hDlg, IDC_NETUI_PASSWORD), EM_SETSEL, 0, -1);
            }

            fDisallowSavedPasswords = DisallowSavedPasswords();

            // clear and disable the saved password flag based on system policy
            hCtrl = GetDlgItem(hDlg, IDC_NETUI_SAVEPASSWORD);
            ASSERT(hCtrl);
            EnableWindow(hCtrl, !fDisallowSavedPasswords);
            if (fDisallowSavedPasswords)
            {    
                pUserPwd->dwFlags &= ~ NETUI_USERPWD_SAVEPWD;
            }

            // Show or hide the save password checkbox based on "show save password"
            // and "realm" flags
            ShowWindow(GetDlgItem(hDlg, IDC_NETUI_SAVEPASSWORD), 
                (pUserPwd->dwFlags & (NETUI_USERPWD_SHOW_SAVEPWD | NETUI_USERPWD_REALM)) ? SW_SHOW : SW_HIDE);
        
            if ((pUserPwd->dwFlags & NETUI_USERPWD_SHOW_SAVEPWD) &&
                (pUserPwd->dwFlags & NETUI_USERPWD_SAVEPWD))
            {
                // Check the save password box
                SendMessage(GetDlgItem(hDlg, IDC_NETUI_SAVEPASSWORD),
                            BM_SETCHECK,1,0);
            }
    
            // Stretch the controls to fit the screen
            SHResizeDialogProc(hDlg, uMsg, wParam, lParam);

            SetTimer( hDlg, 1, MAX_TIMEOUT, NULL );

            // custom initialization
            OnUserPwdInitDialog(hDlg, mbi.hwndMB);

            return FALSE;
        }

        case WM_HELP:
            if (gc_fIsHelpPresent)
            {
                pUserPwd = (PNETUI_USERPWD)GetWindowLong (hDlg, DWL_USER);
                CreateProcess(c_szHelpApp, (pUserPwd->dwFlags & NETUI_USERPWD_NOPWD) ?
                    c_szLogonNoPasswordHelpLink : c_szLogonPasswordHelpLink, NULL, NULL,
                    FALSE, 0, NULL, NULL, NULL, NULL);
            }
            break;

        case WM_HOTKEY:
        {
            // Handle backspace hotkey
            if ((HIWORD(lParam) == VK_TBACK) && gc_fHandleBackKey)
            {
                SHSendBackToFocusWindow(uMsg, wParam, lParam);
            }
            break;
        }

        case WM_TIMER:
            PostMessage(hDlg, WM_COMMAND, IDC_NETUI_CANCEL_PSWD, 0);
            break;

        case WM_ACTIVATE:
            if (LOWORD(wParam) == WA_INACTIVE)
            {
                HWND hwndActive = (HWND) lParam;
                OnUserPwdInactivate(hDlg, hwndActive);
            }
            return FALSE;

        case WM_COMMAND:
            pUserPwd = (PNETUI_USERPWD)GetWindowLong (hDlg, DWL_USER);

            switch( LOWORD( wParam ) )
            {
                case IDCANCEL:
                case IDC_NETUI_CANCEL_PSWD:              // Ignore values
					EndDialog( hDlg, IDCANCEL );
                break;

                case IDOK:
                case IDC_NETUI_OK_PSWD:                  // Get the dialog values
                {
                    DWORD cchUser = 0;
                    DWORD cchDomain = 0;
                    DWORD cchPassword = 0;
                    int   nResult = IDOK;

                    DEBUGMSG (ZONE_USERPWD, (TEXT("User pressed OK\r\n")));
                    cchUser = GetDlgItemText( hDlg, IDC_NETUI_USERNAME,
                                    pUserPwd->szUserName, UNLEN + 1);

                    if (!(pUserPwd->dwFlags & NETUI_USERPWD_REALM))
                    {
                        cchDomain = GetDlgItemText( hDlg, IDC_NETUI_DOMAIN,
                                pUserPwd->szDomain, DNLEN + 1 );
                    }

                    if (!(pUserPwd->dwFlags & NETUI_USERPWD_NOPWD))
                    {
                        cchPassword = GetDlgItemText( hDlg, IDC_NETUI_PASSWORD,
                                       pUserPwd->szPassword, PWLEN + 1);

                        // Get the server name from the resource name
                        GetServerFromResource(pUserPwd->szResourceName,server,&serverLen);
        
                        if (SendMessage(GetDlgItem(hDlg, IDC_NETUI_SAVEPASSWORD),
                                 BM_GETCHECK, 0, 0))
                        {
                            if (pUserPwd->dwFlags & NETUI_USERPWD_REALM)
                            {
                                if (pUserPwd->szUserName[0])
                                {
                                    SetCachedCredentials(pUserPwd->szRealm, pUserPwd->szUserName, pUserPwd->szPassword);
                                }
                            }
                            else
                            {
                                pUserPwd->dwFlags |= NETUI_USERPWD_SAVEPWD;

                                if (pUserPwd->szUserName[0] != TEXT('\0'))
                                {
                                    // Cache the cred for the specific target
                                    CacheDomainCredential(server,
                                                 serverLen,
                                                 pUserPwd->szDomain,
                                                 pUserPwd->szUserName,
                                                 pUserPwd->szPassword,
                                                 cchPassword);

                                    // Update the default cred. 
                                    // Default cred reflects last written target.
                                    CacheDomainCredential(L"",
                                                 0,
                                                 pUserPwd->szDomain,
                                                 pUserPwd->szUserName,
                                                 pUserPwd->szPassword,
                                                 cchPassword);
                                 }
                             }
                         }
                         else
                         {
                            if(!(pUserPwd->dwFlags & NETUI_USERPWD_REALM))
                            {
                                if(pUserPwd->szUserName[0] != L'\0')
                                {
                                    // Save the cred for the target but without the password
                                    CacheDomainCredential(server,
                                                  serverLen,
                                                  pUserPwd->szDomain,
                                                  pUserPwd->szUserName,
                                                  NULL,
                                                  0);
                                }
                            }
                            pUserPwd->dwFlags &= ~(NETUI_USERPWD_SAVEPWD);
                        }
                    }
            
                    EndDialog( hDlg, nResult );
                    break;
                }

                case IDM_NETUI_DETAILS:
                {
                    // Pop up dialog box with the full site name
                    BOOL bRet;
                    bRet = DialogBoxParam(HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_DETAILS), 
                                          hDlg, DlgProcDetails, (LPARAM)pUserPwd);
                               
                    return bRet;
                }

                case IDC_NETUI_SAVEPASSWORD:
                    switch(HIWORD(wParam))
                    {
                        case BN_CLICKED:
                        {
                            if (SendMessage((HWND) lParam, BM_GETCHECK, 0, 0) == BST_CHECKED )
                            {
                                // The use wants to save their password. Warn them.
                                SHSavePWWarning(hDlg);
                            }
                            break;
                        }
                    }
                    break;

                case IDC_NETUI_PASSWORD:
                    if ((HIWORD(wParam) == EN_SETFOCUS) ||
                        ((HIWORD(wParam) == EN_KILLFOCUS) && fReEnable) && 
                        gc_fIsSipPresent)
                    {
                        SIPINFO     si;
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

                default:
                    // Reset timer to give us more time
                    SetTimer( hDlg, 1, MAX_TIMEOUT, NULL );

            }
            break;    

        case WM_PAINT:
            if (gc_fDrawHeaderLine)
            {
                HDC hdcPaint;
                PAINTSTRUCT     PaintStruct;

                // Get the device context.
                hdcPaint = BeginPaint(hDlg, &PaintStruct);

                // Paint the line under the header text.
                SHHdrGrpSepLineDraw(hDlg, NULL, hdcPaint, SHD_HEADER);
                EndPaint(hDlg, &PaintStruct);
            }
            return FALSE;

        case WM_DESTROY:
        {
            SHResizeDialogProc(hDlg, uMsg, wParam, lParam);
            if (g_fDestroyUserPwdFonts)
            {
                HFONT hFont = NULL;

                // Free our font
                hFont = (HFONT) SendDlgItemMessage(hDlg, IDC_NETUI_USER_STATIC, WM_GETFONT, 0, 0);
                if (hFont)
                {
                    DeleteObject(hFont);
                }
            }
            KillTimer(hDlg,1);
            return FALSE;
        }

        case WM_SIZE:
            // Stretch the controls in these dialogs
            SHResizeDialogProc(hDlg, uMsg, wParam, lParam);
            return FALSE;

        default:
            return FALSE;
            break;
    }
    return TRUE;
}

// phDlg gets the handle to the dialog before this function even returns, and
// may be used to cancel the dialog before this function completes
BOOL GetUsernamePasswordEx (HWND hParent, PNETUI_USERPWD pUserPwd, HWND *phDlg)
{
    HCURSOR hCur;
    HGLOBAL hGlob;
    HRSRC   hRsrc;
    int     idd = IDD_NETUI_USERPWD, iRet;
    USERPWD_DLGINFO updi;

    DEBUGMSG (ZONE_USERPWD, (TEXT("GetUsernamePasswordEx: Enter\r\n")));

    if (pUserPwd->dwFlags & NETUI_USERPWD_REALM)
    {
        if ((pUserPwd->dwFlags & NETUI_USERPWD_AUTH_FAIL) ||
            FAILED(GetCachedCredentials(  pUserPwd->szRealm,
                                          pUserPwd->szUserName, 
                                          sizeof(pUserPwd->szUserName) / sizeof(TCHAR),
                                          pUserPwd->szPassword,
                                          sizeof(pUserPwd->szPassword) / sizeof(TCHAR))))
        {
            // Clear Credential if we save it before
            ClearCachedCredential(pUserPwd->szRealm);
            idd = IDD_NETUI_REALM_AUTH;
        }
        else
        {
            return TRUE;
        }
    }
    else if (pUserPwd->dwFlags & NETUI_USERPWD_WININET)
    {
        idd = IDD_NETUI_NTLM_AUTH;
    }
    else if (pUserPwd->dwFlags & NETUI_USERPWD_NOPWD)
    {
        idd = IDD_NETUI_USERNAME;
    }

    SHInitExtraControls();

    hRsrc = FindResource (HINST_RESDLL, MAKEINTRESOURCE(idd), RT_DIALOG);
    if (hRsrc == NULL)
    {
        return FALSE;
    }
    hGlob = LoadResource (HINST_RESDLL, hRsrc);
    if (hGlob == NULL)
    {
        return FALSE;
    }

    updi.phDlg = phDlg;
    updi.pUserPwd = pUserPwd;

    hCur = SetCursor(NULL);
    iRet = DialogBoxParam (HINST_RESDLL, MAKEINTRESOURCE(idd),
                           GetUsernamePwdDlgParent(hParent), DlgProcUserPwd, (LPARAM)&updi);
    SetCursor(hCur);

    if (iRet != IDOK)
    {
        DEBUGMSG(ZONE_ERROR, (TEXT("GetUsernamePasswordEx: DialogBoxParam returned err %u\r\n"),
                              GetLastError()));
        return FALSE;
    }
    else
        return TRUE;
}
