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
// certui.cpp : common certificate functionality
//

#include <windows.h>
#include <wpcpriv.h>
#include <ehm.h>
#include <resource.h>
#include <netuip.h>
#include <certui2.h>
#include <certuip.h>
#include <ossvcs.h>

#define PC_NUM_COLUMNS  2
#define SHA1HASHLENGTH  40

static HINSTANCE g_hCrypt32;
static LONG g_refCount;

PFNCERTOPENSTORE pfnCertOpenStore;
PFNCERTENUMCERTIFICATESINSTORE pfnCertEnumCertificatesInStore;
PFNCERTFINDCERTIFICATEINSTORE pfnCertFindCertificateInStore;
PFNCERTGETCERTIFICATECONTEXTPROPERTY pfnCertGetCertificateContextProperty;
PFNCERTFREECERTIFICATECONTEXT pfnCertFreeCertificateContext;
PFNCERTGETENHANCEDKEYUSAGE pfnCertGetEnhancedKeyUsage;
PFNCERTGETNAMESTRINGW pfnCertGetNameStringW;
PFNCERTCLOSESTORE pfnCertCloseStore;
PFNCRYPTVERIFYCERTIFICATESIGNATURE pfnCryptVerifyCertificateSignature;
PFNCERTCOMPARECERTIFICATENAME pfnCertCompareCertificateName;

HRESULT LoadCAPI2()
{
    HRESULT hr = S_OK;
    BOOL    fDecrement = FALSE;

    // Have we already loaded crypt32.dll?
    CBREx(InterlockedIncrement(&g_refCount) == 1, NULL == g_hCrypt32 ? E_FAIL : S_FALSE);
    DEBUGCHK(g_hCrypt32 == NULL);
    fDecrement = TRUE;

    g_hCrypt32 = LoadLibrary(TEXT("crypt32.dll"));
    CWR(g_hCrypt32);

    // Load the crypt32 functions
    CBRA_LOAD(g_hCrypt32, CertOpenStore, PFNCERTOPENSTORE);
    CBRA_LOAD(g_hCrypt32, CertEnumCertificatesInStore, PFNCERTENUMCERTIFICATESINSTORE);
    CBRA_LOAD(g_hCrypt32, CertFindCertificateInStore, PFNCERTFINDCERTIFICATEINSTORE);
    CBRA_LOAD(g_hCrypt32, CertGetCertificateContextProperty, PFNCERTGETCERTIFICATECONTEXTPROPERTY);
    CBRA_LOAD(g_hCrypt32, CertFreeCertificateContext, PFNCERTFREECERTIFICATECONTEXT);
    CBRA_LOAD(g_hCrypt32, CertGetEnhancedKeyUsage, PFNCERTGETENHANCEDKEYUSAGE);
    CBRA_LOAD(g_hCrypt32, CertGetNameStringW, PFNCERTGETNAMESTRINGW);
    CBRA_LOAD(g_hCrypt32, CertCloseStore, PFNCERTCLOSESTORE);
    CBRA_LOAD(g_hCrypt32, CryptVerifyCertificateSignature, PFNCRYPTVERIFYCERTIFICATESIGNATURE);
    CBRA_LOAD(g_hCrypt32, CertCompareCertificateName, PFNCERTCOMPARECERTIFICATENAME);

    // Load any platform specific functions
    hr = LoadCAPI2_I(g_hCrypt32);
    CHR(hr);
    
Error:
    if (FAILED(hr) && fDecrement)
    {
        FreeCAPI2();
    }

    return hr;
}

// should be called once for every successful call to LoadCAPI2
void FreeCAPI2()
{
    LONG refCount;

    if ((refCount = InterlockedDecrement(&g_refCount)) <= 0)
    {
        DEBUGCHK(refCount == 0);

        if (g_hCrypt32)
        {
            // Do any platform specific cleanup
            FreeCAPI2_I();

            pfnCertOpenStore = NULL;
            pfnCertEnumCertificatesInStore = NULL;
            pfnCertFindCertificateInStore = NULL;
            pfnCertGetCertificateContextProperty = NULL;
            pfnCertFreeCertificateContext = NULL;
            pfnCertGetEnhancedKeyUsage = NULL;
            pfnCertGetNameStringW = NULL;
            pfnCertCloseStore = NULL;

            FreeLibrary(g_hCrypt32);
            g_hCrypt32 = NULL;
        }
    }
}


// Load the cert picker listview with certificate names. UI details in 
// platform specific code.
HRESULT PopulateCertList(HWND hCtlList, NETUI_PICKCERT* pPickCert)
{
    HRESULT hr = S_OK;

    CHRA(LoadCAPI2());
    CBRA(NULL != hCtlList);
    
    InitCertList(hCtlList);  

    for (int iIndex = 0; iIndex < pPickCert->nCerts; ++iIndex)
    {
        PCCERT_CONTEXT pCert = pPickCert->ppCerts[iIndex];
        TCHAR rgch[CRYPTUI_MAX_STRING_SIZE];

        pfnCertGetNameStringW(
                pCert,
                CERT_NAME_FRIENDLY_DISPLAY_TYPE,
                CERT_NAME_ISSUER_FLAG,
                NULL,
                rgch,
                ARRAYSIZEOF(rgch));
        
        InsertCertName(hCtlList, rgch, iIndex);
    }

    // Set default selection. If none already selected, select the 1st.
    int iSelected = 0;
    if (pPickCert->iSelected > 0 && pPickCert->iSelected < pPickCert->nCerts)
        iSelected = pPickCert->iSelected;

    SetSelectedCert(hCtlList, iSelected);

Error:
    FreeCAPI2();
    return hr;
}


// Shared by PPC and SP.
BOOL
CALLBACK
PickCertDialogProc(
    HWND hDlg, 
    UINT uMsg, 
    WPARAM wParam, 
    LPARAM lParam)
{
    BOOL fRet = TRUE;
    NETUI_PICKCERT* pPickCert ;
    LONG wCmdId;
    LONG wCmdCbn;

    switch (uMsg) 
    {
        case WM_NOTIFY :
        {
            break;
        }

        case WM_INITDIALOG :
        {
            // Expecting a NETUI_PICKCERT* in lParm.
            if (0 == lParam)
            {
                fRet = FALSE;
                break;
            }

            // Size the dialog: full screen.
            SHINITDLGINFO sidi = {0};
            sidi.dwMask = SHIDIM_FLAGS;
            sidi.dwFlags = GetPickCertDialogFlags();
            sidi.hDlg = hDlg;
            if (!SHInitDialog(&sidi))
            {
                fRet = FALSE;
                break;
            }

            // Softkeys.
            SHMENUBARINFO smbi = {0};
            smbi.cbSize = sizeof(SHMENUBARINFO);
            smbi.hwndParent = hDlg;
            smbi.nToolBarId = IDR_NETUI_CERT_DIALOGOKCANCEL;
            smbi.hInstRes = HINST_RESDLL;
            if (!SHCreateMenuBar(&smbi))
            {
                fRet = FALSE;
                break;
            }

            // Save pointer to cert container passed to us from wininet.
            pPickCert = (NETUI_PICKCERT*) lParam;
            SetWindowLong (hDlg, DWL_USER, lParam);
        
            // Control that contains list of cert names.
            HWND hCtlList = GetDlgItem(hDlg, IDC_CERTLISTVIEW);

            PopulateCertList(hCtlList, pPickCert);
        
            break;
        }

        case WM_HELP:
        {
            VERIFY(CreateProcess(   TEXT("peghelp.exe"),
                                    TEXT("file:wininet.htm"),
                                    NULL,
                                    NULL,
                                    FALSE,
                                    0,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL));

            break;
        }

        case WM_COMMAND :
        {
            wCmdId  = LOWORD(wParam);
            wCmdCbn = HIWORD(wParam);

            switch ( wCmdId ) 
            {
                case IDCANCEL:
                {
                    EndDialog(hDlg, IDCANCEL);
                    break;
                }

                case IDOK:
                { 
                    pPickCert = (NETUI_PICKCERT*)GetWindowLong(hDlg, DWL_USER);
                    if (NULL == pPickCert)
                    {
                        EndDialog(hDlg, IDCANCEL);
                        break;
                    }

                    // Control that contains list of cert names.
                    HWND hCtlList = GetDlgItem(hDlg, IDC_CERTLISTVIEW);

                    int iSelected = GetSelectedCert(hCtlList);
                    if (NULL != pPickCert && iSelected != -1)
                        pPickCert->iSelected = iSelected;
        
                    EndDialog(hDlg, (-1 != iSelected)?IDOK : IDCANCEL);
                    break;
                }

                default:
                {
                    fRet = FALSE;
                    break;
                }
            }
        }

        default:
        {
            fRet = FALSE;
            break;
        }
    }
    return fRet;
}

// Called by Wininet code when it needs the user to select a cert.
BOOL
PickCertificate (HWND hParent, NETUI_PICKCERT* pPickCert)
{
    int iRet = DialogBoxParam (HINST_RESDLL, 
                               MAKEINTRESOURCE(IDD_PICKCERT),
                               SHGetLastActiveWindow(hParent), 
                               PickCertDialogProc, 
                               (LONG)pPickCert);


    return (IDOK == iRet);
}

//////////////////////////////////////////////////////////////////////////////
//
// ShowCertificateFromHash
//
//  Displays a dialog with certificate details, if the cert exists. The dialog
//  is displayed modally if a parent exists, and execution returns to the 
//  caller after the user dismisses the dialog.  
//  Function declaration is in:
//      \public\shellw\oak\inc\netui.h
//
//  Parameters:
//      HWND    hParent - Handle to a window that is the parent of the 
//                        details dialog. If NULL, this dialog has no parent.
//      LPCTSTR pszHash - Hex SHA1 Hash of the certificate to display. 
//                        Example: D23209AD23D314232174E40D7F9D62139786633A
//
//  Return Value:  
//      HRESULT. Typical return values are described below. 
//      S_OK           - The certificate was found and the dialog was displayed 
//                       normally.
//      E_INVALIDARG   - The pszHash parameter is missing or is not a valid
//                       hex string.
//      CRYPT_E_NOT_FOUND - The specified certificate could not be found on
//                           the device.
//
HRESULT WINAPI ShowCertificateFromHash(__in __opt HWND hParent, __in LPCTSTR pszHash)
{
    HRESULT          hr         = S_OK;     // Return value
    CRYPT_HASH_BLOB  blobHash   = {0};      // Blob to hold binary hash
    PCCERT_CONTEXT   pCtx       = NULL;     // Loaded certificate context
    HCERTSTORE       hCertStore = NULL;     // Certificate store
    BOOL             fCapi2     = FALSE;    // Flag if the CAPI library is loaded

    // Check hash for 0-9, a-f -- also check that hash length is exactly 40
    // any other input is invalid and could also be a security risk
    LPCTSTR c_szValidChars = TEXT("0123456789abcdefABCDEF");
    size_t  cchHash = 0;
    size_t  iFirstNonHex = _tcsspn(pszHash, c_szValidChars);
    StringCchLength(pszHash, STRSAFE_MAX_CCH, &cchHash);

    CBRAEx(TEXT('\0') != pszHash[0] && 
        cchHash == iFirstNonHex &&
        cchHash == SHA1HASHLENGTH, E_INVALIDARG);

    // Translate hash string to blob
    CHR(HexStringToBinary(pszHash, NULL, &blobHash.cbData, TRUE));
    blobHash.pbData = (LPBYTE)LocalAlloc(LMEM_FIXED, blobHash.cbData);
    CPR(blobHash.pbData);
    CHR(HexStringToBinary(pszHash, blobHash.pbData, &blobHash.cbData, TRUE));

    // Load CAPI functions
    CHRA(LoadCAPI2());
    fCapi2 = TRUE;

    // Loop through system stores looking for this certificate
    LPCTSTR c_szSystemStores[] = {TEXT("MY"), TEXT("ROOT"), TEXT("CA")}; // In order of likelihood
    for(DWORD dwStore=0; dwStore < ARRAYSIZE(c_szSystemStores); dwStore++)
    {
        // Open the system store
        hCertStore = pfnCertOpenStore(CERT_STORE_PROV_SYSTEM, 0, NULL,
            CERT_SYSTEM_STORE_CURRENT_USER | CERT_STORE_READONLY_FLAG, 
            c_szSystemStores[dwStore]);
        if(NULL != hCertStore)
        {
            // Search for the specified certificate in this store
            pCtx = pfnCertFindCertificateInStore(hCertStore, X509_ASN_ENCODING, 
                0, CERT_FIND_SHA1_HASH, &blobHash, NULL);
            if(NULL != pCtx)
            {
                // Certificate was found...show the dialog
                if(!ShowCertificate(hParent, pCtx))
                {   
                    // Unknown error
                    hr = E_FAIL;
                }
                pfnCertFreeCertificateContext(pCtx);
                break;
            }
            pfnCertCloseStore(hCertStore, 0);
            hCertStore = NULL;
        }
    }
    
    // Let the user know if the certificate was not found
    CBREx(NULL != pCtx, CRYPT_E_NOT_FOUND);
    
Error:
    if (NULL != hCertStore)
    {
        pfnCertCloseStore(hCertStore, 0);
    }
    LocalFree(blobHash.pbData);
    if(fCapi2)
    {
        FreeCAPI2();
    }
    return hr;
}

extern "C"
int FormatDate(FILETIME ft, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    HRESULT    hr = S_OK;
    SYSTEMTIME st;
    FILETIME   ftLocal;
    int        nRes = 0;

    CWR(FileTimeToLocalFileTime(&ft, &ftLocal));
    CWR(FileTimeToSystemTime(&ftLocal, &st));

    nRes = GetDateFormat(LOCALE_USER_DEFAULT, dwFlags, &st, NULL, pszBuffer, cchBuffer);
    CWR(0 != nRes);

Error:
    return nRes;
}
