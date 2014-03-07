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
// pickcert.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include <wpcpriv.h>
#include <ehm.h>

#include "windowsx.h"
#include <commctrl.h>
#include "wcommctl.h"
#include <wincrypt.h>
#include "certui2.h"
#include "certuip.h"
#include <winscard.h>
#include <netuip.h>
#include "resource.h"
#include "aygshell.h"

//#include "CertLayoutHelper.h"


PFNCERTSETCERTIFICATECONTEXTPROPERTY pfnCertSetCertificateContextProperty;
PFNCRYPTFINDOIDINFO pfnCryptFindOIDInfo;


DWORD GetPickCertDialogFlags()
{
    return SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_DONEBUTTON;
}

HRESULT LoadCAPI2_I(HINSTANCE hinstCrypt32)
{
    HRESULT hr = S_OK;

    CBRA_LOAD(hinstCrypt32, CertSetCertificateContextProperty, PFNCERTSETCERTIFICATECONTEXTPROPERTY);
    CBRA_LOAD(hinstCrypt32, CryptFindOIDInfo, PFNCRYPTFINDOIDINFO);

Error:
    return hr;
}

void FreeCAPI2_I()
{
    // Clear the function pointers
    pfnCertSetCertificateContextProperty = NULL;
    pfnCryptFindOIDInfo = NULL;
}


// Init the cert list for the cert picker dialog.
void InitCertList(HWND hCtlList)
{
    // Causes selection to be full length of control, not just the text.
    ListView_SetExtendedListViewStyle(hCtlList, LVS_EX_FULLROWSELECT);

    // This list needs at least one column to function.
    RECT rect;
    GetClientRect(hCtlList, &rect);
    LVCOLUMN lvColumn = {0};
    lvColumn.mask = LVCF_WIDTH;
    lvColumn.cx = rect.right;
    ListView_InsertColumn(hCtlList, 0, &lvColumn);
}

// Insert a cert name into the list in the next position. Assumes
// iIndex will come in sequential order and that the list is unsorted.
HRESULT InsertCertName(HWND hCtlList, PTCHAR szCertName, int iIndex)
{
    HRESULT hr = S_OK;

    LVITEM lvI = {0};
    lvI.iItem = iIndex;
    lvI.mask = LVIF_TEXT;
    lvI.pszText = szCertName;

    // It is important that the list is unsorted
    // because we return the index of the selected cert
    // and expect that index to be the same as that cert's
    // index in pPickCert->ppCerts.
    int iPos = ListView_InsertItem(hCtlList, &lvI);
    CBRA(iPos == iIndex);

Error:
    return hr;
}

// Set one of the certs as selected.
void SetSelectedCert(HWND hCtlList, int iSelected)
{
    ListView_SetItemState(hCtlList, iSelected, LVIS_SELECTED, LVIS_SELECTED);
}

// Get the selected cert from the cert picker dialog.
int GetSelectedCert(HWND hCtlList)
{
    return ListView_GetNextItem(hCtlList, -1, LVNI_SELECTED);
}

HRESULT LayoutCertDetails (HWND hDlg)
{

    HRESULT hr = S_OK;
	/*
    CDialogLayoutHelper dlhLayoutHelper (hDlg);

    hr = dlhLayoutHelper.Init (IDC_STATIC_CERTS_ISSUEDTO);
    CHR(hr);

    hr = dlhLayoutHelper.SetControlHeight (IDC_CERTS_ISSUEDTO);
    CHR(hr);

    hr = dlhLayoutHelper.PushUpDown (IDC_STATIC_CERTS_ISSUEDBY);
    CHR(hr);

    hr = dlhLayoutHelper.PushUpDown (IDC_CERTS_ISSUEDBY);
    CHR(hr);
    
    hr = dlhLayoutHelper.SetControlHeight (IDC_CERTS_ISSUEDBY);
    CHR(hr);

    hr = dlhLayoutHelper.PushUpDown (IDC_STATIC_CERTS_VALIDFROM);
    CHR(hr);

    hr = dlhLayoutHelper.PushUpDown (IDC_CERTS_VALIDFROM);
    CHR(hr);

    hr = dlhLayoutHelper.PushUpDown (IDC_STATIC_CERTS_VALIDTO);
    CHR(hr);

    hr = dlhLayoutHelper.PushUpDown (IDC_CERTS_VALIDTO);
    CHR(hr);

    hr = dlhLayoutHelper.PushUpDown (IDC_STATIC_CERTS_USAGE);
    CHR(hr);

    hr = dlhLayoutHelper.PushUpDown (IDC_CERTS_USAGE);
    CHR(hr);
*/
//Error:
    return hr;
}


BOOL
CALLBACK
ShowCertDialogProc(
    HWND hDlg, 
    UINT uMsg, 
    WPARAM wParam, 
    LPARAM lParam)
{
    HRESULT hr = S_OK;
    PCCERT_CONTEXT pCertContext;
    PCCRYPT_OID_INFO pOidInfo;
    PCERT_ENHKEY_USAGE pUsage = NULL;
    LPWSTR pwszBuf = NULL;
    LPWSTR pwszSubject = NULL;
    LPWSTR pwch;
    DWORD  cch;
    UINT i;
    BOOL fAllPurposes = FALSE;

    LRESULT lr = SHRCMLDialogProc(HINST_RESDLL, MAKEINTRESOURCE(IDD_CERTDETAILS), hDlg, uMsg, wParam, lParam); 

    switch (uMsg)
    {
        case WM_INITDIALOG:
        {
            // This dialog needs a scrollbar
            SHINITDLGINFO shidi;
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.hDlg = hDlg;
            shidi.dwFlags = SHIDIF_WANTSCROLLBAR | SHIDIF_SIZEDLGFULLSCREEN | 
                SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_EMPTYMENU;
            SHInitDialog(&shidi);

             // Validate cert context from LPARAM
            pCertContext = (PCCERT_CONTEXT)lParam;
            CBR(NULL != pCertContext && NULL != pCertContext->pCertInfo);

            // "Issued To"
            cch = pfnCertGetNameStringW(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 
                0, NULL, NULL, NULL);
            CWR(cch); // Returns 1 for zero-length string in case there is no issued to name
            pwszSubject = (LPTSTR)LocalAlloc(LMEM_FIXED, cch * sizeof(TCHAR)); // cch includes the null terminator
            CPR(pwszSubject);
            pfnCertGetNameStringW(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 
                0, NULL, pwszSubject, cch);
            CWR(cch);
            SetDlgItemTextW(hDlg, IDC_CERTS_ISSUEDTO, pwszSubject);

            // "Issued By"
            cch = pfnCertGetNameStringW(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 
                CERT_NAME_ISSUER_FLAG, NULL, NULL, NULL);
            CWR(cch); // Returns 1 for zero-length string in case there is no issued by name
            pwszBuf = (LPTSTR)LocalAlloc(LMEM_FIXED, cch * sizeof(TCHAR)); // cch includes the null terminator
            CPR(pwszBuf);
            pfnCertGetNameStringW(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 
                CERT_NAME_ISSUER_FLAG, NULL, pwszBuf, cch);
            CWR(cch);

            // Special case if this is a self-issued cert: 1) self-signed and 2) subject == issuer
            if (pfnCryptVerifyCertificateSignature(NULL, 
                                                X509_ASN_ENCODING, 
                                                pCertContext->pbCertEncoded, 
                                                pCertContext->cbCertEncoded, 
                                                &(pCertContext->pCertInfo->SubjectPublicKeyInfo)) &&
                pfnCertCompareCertificateName(PKCS_7_ASN_ENCODING | X509_ASN_ENCODING, 
                                           &(pCertContext->pCertInfo->Issuer), 
                                           &(pCertContext->pCertInfo->Subject)))
            {
                // Use special "self-issued" string
                LPCTSTR pszSelfIssued = (LPCTSTR)LoadString(HINST_RESDLL, IDS_NETUI_SELFISSUED, NULL, NULL);
                CPR(pszSelfIssued);
                SetDlgItemText(hDlg, IDC_CERTS_ISSUEDBY, pszSelfIssued);
            }
            else
            {
                SetDlgItemTextW(hDlg, IDC_CERTS_ISSUEDBY, pwszBuf);
            }
            LocalFree(pwszBuf);
            pwszBuf = NULL;
            LocalFree(pwszSubject);
            pwszSubject = NULL;

            // "Valid From"
            // Not MBCS safe
            cch = FormatDate(pCertContext->pCertInfo->NotBefore, DATE_SHORTDATE, NULL, 0);
            CWR(cch);
            pwszBuf = (LPTSTR)LocalAlloc(LMEM_FIXED, cch * sizeof(TCHAR)); // cch includes the null terminator
            CPR(pwszBuf);
            cch = FormatDate(pCertContext->pCertInfo->NotBefore, DATE_SHORTDATE, pwszBuf, cch);
            CWR(cch);
            SetDlgItemTextW(hDlg, IDC_CERTS_VALIDFROM, pwszBuf);
            LocalFree(pwszBuf);
            pwszBuf = NULL;

            // "Valid To"
            // Not MBCS safe
            cch = FormatDate(pCertContext->pCertInfo->NotAfter, DATE_SHORTDATE, NULL, 0);
            CWR(cch);
            pwszBuf = (LPTSTR)LocalAlloc(LMEM_FIXED, cch * sizeof(TCHAR)); // cch includes the null terminator
            CPR(pwszBuf);
            cch = FormatDate(pCertContext->pCertInfo->NotAfter, DATE_SHORTDATE, pwszBuf, cch);
            CWR(cch);
            SetDlgItemTextW(hDlg, IDC_CERTS_VALIDTO, pwszBuf);
            LocalFree(pwszBuf);
            pwszBuf = NULL;

            // "Intended Purpose(s)"
            CWR(pfnCertGetEnhancedKeyUsage(pCertContext, 0, NULL, &cch));
            pUsage = (PCERT_ENHKEY_USAGE)LocalAlloc(LMEM_FIXED, cch);
            CPR(pUsage);
            CWR(pfnCertGetEnhancedKeyUsage(pCertContext, 0, pUsage, &cch));
                 
            if(pUsage->cUsageIdentifier == 0 && (GetLastError() == CRYPT_E_NOT_FOUND))
            {
                fAllPurposes = TRUE;
            }
            for(i=0, cch=1; i < pUsage->cUsageIdentifier; i++)
            {
                pOidInfo = pfnCryptFindOIDInfo(
                                CRYPT_OID_INFO_OID_KEY,
                                pUsage->rgpszUsageIdentifier[i],
                                CRYPT_ENHKEY_USAGE_OID_GROUP_ID);
                if(pOidInfo)
                {
                    cch += wcslen(pOidInfo->pwszName) + 2 ;  // 2 for \r\n
                }
                else
                {
                    cch += strlen(pUsage->rgpszUsageIdentifier[i]) + 2;
                }
            }

            if(fAllPurposes)
            {
                cch = CRYPTUI_MAX_STRING_SIZE;
            }

            pwszBuf = (LPWSTR)LocalAlloc(LMEM_FIXED, cch * sizeof(WCHAR));
            CPR(pwszBuf);
            pwch = pwszBuf;
            *pwszBuf = '\0';
            if (fAllPurposes)
            {
                LoadStringW(HINST_RESDLL, IDS_ALL_PURPOSES, pwszBuf, cch);
            }
            else
            {
                for (i=0; i < pUsage->cUsageIdentifier; i++)
                {
                    pOidInfo = pfnCryptFindOIDInfo(
                        CRYPT_OID_INFO_OID_KEY,
                        pUsage->rgpszUsageIdentifier[i],
                        CRYPT_ENHKEY_USAGE_OID_GROUP_ID);
                    if (pOidInfo)
                    {
                        CHR(StringCchCopyW(pwch, cch - (pwch - pwszBuf), pOidInfo->pwszName));
                    }
                    else
                    {
                        // Couldn't find a descriptive name for the OID.
                        // Display the OID itself
                        mbstowcs(pwch, pUsage->rgpszUsageIdentifier[i], cch - (pwch - pwszBuf));
                    }
                    pwch += wcslen(pwch);
                    StringCchCopyW(pwch, cch - (pwch - pwszBuf), L"\r\n");
                    pwch += 2;
                }
            }
            SetDlgItemTextW(hDlg, IDC_CERTS_USAGE, pwszBuf);
    
            CHR(LayoutCertDetails (hDlg));

            break;
        }

        case WM_SETTINGCHANGE:
            if (wParam == SETTINGCHANGE_RESET)
            {
                LayoutCertDetails (hDlg);
            }
            break;

        case WM_COMMAND:
        {
            EndDialog(hDlg, IDOK);
            break;
        }

        case FCM_CLOSEWINDOW:
        {
            EndDialog(hDlg, IDOK);          
            SetWindowLong(hDlg, DWL_MSGRESULT, TRUE);
            break;
        }
    }

Error:
    LocalFree(pwszSubject);
    LocalFree(pwszBuf);
    LocalFree(pUsage);

    return (SUCCEEDED(hr) ? lr : FALSE);
}

BOOL
ShowCertificate(HWND hParent, PCCERT_CONTEXT pCert)
{
    BOOL fRet;
    fRet = SUCCEEDED(LoadCAPI2());
    if (fRet)
    {
        fRet = (IDOK == DialogBoxParam(HINST_RESDLL, MAKEINTRESOURCE(IDD_CERTDETAILS),
                              SHGetLastActiveWindow(hParent), ShowCertDialogProc, (LONG) pCert));
        FreeCAPI2();
    }
    return fRet;
}
