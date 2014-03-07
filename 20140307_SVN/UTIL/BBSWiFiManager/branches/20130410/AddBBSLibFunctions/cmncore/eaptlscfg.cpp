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
// eaptlscfg.cpp : Implemention of EAP extension configuration UI
//

#include "windows.h"
#include "windowsx.h"
#include <commctrl.h>
#include "wcommctl.h"
#include <wincrypt.h>
#include <certui2.h>
#include <certuip.h>
#include <eapcfg2.h>
#include <winscard.h>
#include <netuip.h>
#include "wpcpriv.h"
#include "resource.h"
#include "ehm.h"
#include <netui.h>

#ifndef UNLEN
#define UNLEN       256                 // Maximum user name length
#endif


TCHAR const c_szPickCertHelpLink[] =
        TEXT("connect.htm#eaptlspickcert");

static const TCHAR  c_szRegistryKey[]        = TEXT("ControlPanel\\WiFi");
static const TCHAR  c_szValidateServerCert[] = TEXT("ValidateServerCert");

extern "C" HRESULT PickCertificateDlg(HWND hwndParent, PCTSTR pcszStoreName, PCTSTR pcszHelpLink, PCTSTR pcszDesc, PBYTE pbHashIn, DWORD cchHashIn, PBYTE* ppbHashOut, PDWORD pcchHashOut);

EAPTLS_CONN_PROPERTIES_V1*
InitConnectionData(BYTE *pConnectionDataIn, DWORD dwSizeOfConnectionDataIn, DWORD dwConnFlags)
{
    EAPTLS_CONN_PROPERTIES_V1* pConnProp = NULL;
    DWORD                      dwErr     = NO_ERROR;

    if (dwSizeOfConnectionDataIn < sizeof(EAPTLS_CONN_PROPERTIES_V1))
    {
        pConnProp = (EAPTLS_CONN_PROPERTIES_V1 *)LocalAlloc(LPTR, sizeof(EAPTLS_CONN_PROPERTIES_V1));

        if (NULL == pConnProp)
        {
            dwErr = GetLastError();
            DEBUGMSG(ZONE_ERROR, (TEXT("LocalAlloc failed and returned %d"), dwErr));
            goto LDone;
        }

        pConnProp->dwVersion = EAPTLS_CONN_PROPS_V1;
        pConnProp->dwSize = sizeof(EAPTLS_CONN_PROPERTIES_V1);
        pConnProp->fFlags = dwConnFlags;
    }
    else
    {
        DEBUGCHK(NULL != pConnectionDataIn);

        pConnProp = (EAPTLS_CONN_PROPERTIES_V1 *)LocalAlloc(LPTR, dwSizeOfConnectionDataIn);

        if (NULL == pConnProp)
        {
            dwErr = GetLastError();
            DEBUGMSG(ZONE_ERROR, (TEXT("LocalAlloc failed and returned %d"), dwErr));
            goto LDone;
        }

        CopyMemory(pConnProp, pConnectionDataIn, dwSizeOfConnectionDataIn);

        // If the user has mucked with the phonebook, we mustn't be affected.
        // The size must be correct.
        pConnProp->dwSize = dwSizeOfConnectionDataIn;
        // The Unicode string must be NULL terminated.
        ((BYTE*)pConnProp)[dwSizeOfConnectionDataIn - 2] = 0;
        ((BYTE*)pConnProp)[dwSizeOfConnectionDataIn - 1] = 0;
    }

LDone:
    if (dwErr != NO_ERROR)
    {
        LocalFree(pConnProp);
        pConnProp = NULL;
    }
    return(pConnProp);
}


DWORD 
EapTlsInvokeConfigUI(
    IN  DWORD       dwEapTypeId,
    IN  HWND        hwndParent,
    IN  DWORD       dwFlags,
    IN  BYTE*       pConnectionDataIn,
    IN  DWORD       dwSizeOfConnectionDataIn,
    OUT BYTE**      ppConnectionDataOut,
    OUT DWORD*      pdwSizeOfConnectionDataOut
)
{
    UNREFERENCED_PARAMETER(dwFlags);
    
    EAPTLS_CONN_PROPERTIES_V1* pConnProp = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    HCERTSTORE     hMyStore = NULL;
    DWORD          dwErr = NO_ERROR;
    HRESULT        hr = S_OK;
    DWORD          cbHash = 0, dwConnFlags;
    BYTE           bHash[MAX_HASH_SIZE], *pbHashCert = NULL;
    BOOL           bSuccess, fCapi2 = FALSE;
    INT            cnCerts = 0, nSelected = -1;
    DWORD          dwValidServerClert, dwType, cb;

    hr = LoadCAPI2();
    CHR(hr);
    fCapi2 = TRUE;

    dwConnFlags = (dwEapTypeId == PPP_EAP_PEAP) ? EAPTLS_CONN_FLAG_NO_CLIENT_CERT : 0;

    cb = sizeof(dwValidServerClert);
    if (ERROR_SUCCESS == SHRegQueryValueEx(HKEY_CURRENT_USER, c_szValidateServerCert,
            (DWORD *)c_szRegistryKey, &dwType, (BYTE *)&dwValidServerClert, &cb))
    {
        CBR(REG_DWORD == dwType);

        // dwValidServerClert =  0 - don't validate server certification
        if (dwValidServerClert == 0)
        {
            dwConnFlags |= EAPTLS_CONN_FLAG_NO_VALIDATE_CERT;  // we do not validate server cert by default
        }
    }
    else
    {
        // We dvalidates server cert by default
    }

    pConnProp = InitConnectionData(pConnectionDataIn, dwSizeOfConnectionDataIn, dwConnFlags);
    CPR(pConnProp);

    hMyStore = pfnCertOpenStore(CERT_STORE_PROV_SYSTEM_W, CRYPT_ASN_ENCODING, 
                            0, CERT_SYSTEM_STORE_CURRENT_USER, TEXT("MY"));
    CBR(hMyStore);

    // Enum on personal cert store
    while (NULL != (pCertContext = pfnCertEnumCertificatesInStore(hMyStore, pCertContext)))
    {
        BYTE  bHashTemp[MAX_HASH_SIZE];
        DWORD cbHashTemp = ARRAYSIZE(bHashTemp);

        bSuccess = pfnCertGetCertificateContextProperty(pCertContext, CERT_HASH_PROP_ID,
                        bHashTemp, &cbHashTemp);
        if (   nSelected < 0 // There is matched cert
            && cbHashTemp  
            && cbHashTemp == pConnProp->UserHash.cbHash // The size of certs are equal
            && memcmp(bHashTemp, pConnProp->UserHash.pbHash, cbHashTemp) == 0)
        {
            nSelected = cnCerts;
            cbHash    = cbHashTemp;
            memcpy(bHash, bHashTemp, cbHash);
        }
        cnCerts++;
    }

    if (cnCerts == 0)
    {
        // There is no cert on the device, warrning!
        MessageBox(SHGetLastActiveWindow(hwndParent), 
            (PTSTR)LoadString(HINST_RESDLL, IDS_NETUI_EAPTLS_NO_CERT, NULL, 0), 
            (PTSTR)LoadString(HINST_RESDLL, IDS_NETUI_WRN_TITLE_WARNING, NULL, 0),
            MB_OK | MB_SETFOREGROUND);
        dwErr = ERROR_NOT_FOUND;
    }
    else if (cnCerts > 0)
    {
        DWORD cbHashCert = 0;
        
        // Need to let user choice one
        hr = PickCertificateDlg(NULL, TEXT("MY"), c_szPickCertHelpLink,
                (PTSTR)LoadString(HINST_RESDLL, IDS_NETUI_EAPTLS_DESC, NULL, 0),
                bHash, cbHash, 
                &pbHashCert, &cbHashCert);
        CHR(hr);
        CBRA(cbHashCert <= MAX_HASH_SIZE);

        memcpy(pConnProp->UserHash.pbHash, pbHashCert, cbHashCert);
        pConnProp->UserHash.cbHash = cbHashCert;

        *ppConnectionDataOut        = (BYTE*)pConnProp;
        *pdwSizeOfConnectionDataOut = pConnProp->dwSize;
    }

Error:
    pfnCertFreeCertificateContext(pCertContext);
    pfnCertCloseStore(hMyStore, 0);
    LocalFree(pbHashCert);

    if (fCapi2)
    {
        FreeCAPI2();
    }

    return FAILED(hr) ? ERROR_NOT_FOUND : dwErr;
}

extern "C"
DWORD 
RasEapInvokeConfigUI(
    IN  DWORD       dwEapTypeId,
    IN  HWND        hwndParent,
    IN  DWORD       dwFlags,
    IN  BYTE*       pConnectionDataIn,
    IN  DWORD       dwSizeOfConnectionDataIn,
    OUT BYTE**      ppConnectionDataOut,
    OUT DWORD*      pdwSizeOfConnectionDataOut
)
{
    DWORD  dwErr;

    switch(dwEapTypeId)
    {
    case PPP_EAP_TLS:
        dwErr = EapTlsInvokeConfigUI(
            dwEapTypeId,
            hwndParent,
            dwFlags,
            pConnectionDataIn,
            dwSizeOfConnectionDataIn,
            ppConnectionDataOut,
            pdwSizeOfConnectionDataOut
        );
        break;
#if 0
    case PPP_EAP_PEAP:
        //Invoke the client config UI
        dwErr = EapPeapInvokeConfigUI(
            hwndParent,
            dwFlags,
            pConnectionDataIn,
            dwSizeOfConnectionDataIn,
            ppConnectionDataOut,
            pdwSizeOfConnectionDataOut
        );
        break;
#endif

    default:
        DEBUGMSG(ZONE_ERROR, (TEXT("EAP Type %d is not supported"), dwEapTypeId));
        dwErr = ERROR_NOT_SUPPORTED;
        break;
    }

    return dwErr;
}

// Copied from network.c 
HANDLE CheckPrevInstance(LPCTSTR pszMutexName/*, int iPage*/)
{
    HANDLE hMutex;

    // try to create a mutex object
    hMutex=CreateMutex(NULL, FALSE, pszMutexName);
    if(!hMutex) 
    {
        // Failed for unknown reasons. We now won't have single-instanceing
        ASSERT(FALSE);
        return INVALID_HANDLE_VALUE;
    }

    // We got the mutex. Now check whether it previously existed
    if(ERROR_ALREADY_EXISTS==GetLastError())
    {
    // release the handle so mutex will be freed when the other guy exits
        CloseHandle(hMutex);        
        return INVALID_HANDLE_VALUE;
    }
    else
    {
        // if we got here we have a first-time created mutex. Return the
        // handle so caller can free it before exiting
        return hMutex;
    }
}




/*

Returns:
    Error codes only from winerror.h, raserror.h or mprerror.h

Notes:
    RasEapInvokeInteractiveUI entry point called by the EAP-PPP engine by name.

*/

DWORD
RasEapInvokeInteractiveUI(
    IN  DWORD           dwEapTypeId,
    IN  HWND            hwndParent,
    IN  BYTE*           pUIContextData,
    IN  DWORD           dwSizeofUIContextData,
    OUT BYTE**          ppDataFromInteractiveUI,
    OUT DWORD*          pdwSizeOfDataFromInteractiveUI
)
{
    HRESULT hr = E_FAIL;  // assume not supported

    UNREFERENCED_PARAMETER(dwSizeofUIContextData);
    
    if (PPP_EAP_TLS == dwEapTypeId)
    {
        EAPEXTUI_INPUT *pExtUIInput;
        *ppDataFromInteractiveUI = NULL;
        *pdwSizeOfDataFromInteractiveUI = 0;

        pExtUIInput = (EAPEXTUI_INPUT *)pUIContextData;

        if (NULL != pExtUIInput
            && pExtUIInput->dwVersion == 1
            && pExtUIInput->dwSize >= sizeof(EAPEXTUI_INPUT)
            )
        {
            hr = S_OK;  // assume success

            switch(pExtUIInput->dwCode)
            {
            default:
                CBR(FALSE);
            case EAPEXTUI_CODE_CLIENTAUTH_ERROR:                        
                break;
            case EAPEXTUI_CODE_SERVERAUTH_ERROR:
            {
                TCHAR   szMutexName[64];
                LPCTSTR pszErrorTitle;
                LPCTSTR pszErrorString;
                UINT    uStringID;
                UINT    uType = MB_OK;
                HANDLE  hMutex;

                if (pExtUIInput->fFlags & EAPEXTUI_FLAG_RESPONSE)
                {
                    // response expected
                    uType = MB_YESNO;
                }

                switch (pExtUIInput->dwStatus)
                {
                case SEC_E_UNTRUSTED_ROOT:
                    uStringID = IDS_NETUI_UNKNOWN_ROOT_ERROR;
                    break;
                case SEC_E_CERT_UNKNOWN:
                    uStringID = IDS_NETUI_UNKNOWN_CERT_ERROR;                    
                    break;
                case SEC_E_CERT_EXPIRED:
                    uStringID = IDS_NETUI_CERT_EXPIRED_ERROR;
                    break;
                default:
                    uStringID = IDS_NETUI_GENERIC_CERT_ERROR;
                    break;
                }

                pszErrorTitle = (LPCTSTR)LoadString(HINST_RESDLL, IDS_NETUI_SERVER_VALIDATION_ERROR, NULL, 0);
                CBRA(NULL != pszErrorTitle);

                pszErrorString = (LPCTSTR)LoadString(HINST_RESDLL, uStringID, NULL, 0);
                CBRA(NULL != pszErrorString);

                hr = StringCchPrintf(szMutexName, ARRAYSIZE(szMutexName),
                    TEXT("%s%X"), TEXT("IPInteractUI"), uStringID);
                CHR(hr);

                hMutex = CheckPrevInstance(szMutexName);
                if(INVALID_HANDLE_VALUE != hMutex)
                {
                    int nRet = MessageBox(SHGetLastActiveWindow(hwndParent), 
                        pszErrorString, pszErrorTitle,
                        uType | MB_TOPMOST | MB_SETFOREGROUND);

                    CloseHandle(hMutex);

                    if (pExtUIInput->fFlags & EAPEXTUI_FLAG_RESPONSE)
                    {
                        // response expected
                        EAPEXTUI_OUTPUT *pExtUIOutput = (EAPEXTUI_OUTPUT *)LocalAlloc(LPTR, sizeof(EAPEXTUI_OUTPUT));
                        if (pExtUIOutput)
                        {
                            pExtUIOutput->dwVersion =1;
                            pExtUIOutput->dwSize = sizeof(EAPEXTUI_OUTPUT);
                            pExtUIOutput->dwValue = nRet;
                            *ppDataFromInteractiveUI = (PBYTE)pExtUIOutput;
                            *pdwSizeOfDataFromInteractiveUI = pExtUIOutput->dwSize;
                        }
                    }
                }
            } // case  EAPEXTUI_CODE_SERVERAUTH_ERROR  
                break;
            }
        }
    }

Error:
    return SUCCEEDED(hr) ? NO_ERROR : ERROR_NOT_SUPPORTED;
}


//Get EKU Usage Blob out of the certificate Context

BOOL FGetEKUUsage ( 
    IN PCCERT_CONTEXT           pCertContext,
    OUT PCERT_ENHKEY_USAGE  *   ppUsage
    )
{
    BOOL                fRet = FALSE;
    DWORD               dwBytes = 0;
    DWORD               dwErr = ERROR_SUCCESS;
    PCERT_ENHKEY_USAGE  pUsage = NULL;

    DEBUGMSG(ZONE_ERROR, (TEXT("FGetEKUUsage")));

    if (!pfnCertGetEnhancedKeyUsage(pCertContext, 0, NULL, &dwBytes))
    {
        dwErr = GetLastError();

        if (CRYPT_E_NOT_FOUND == dwErr)
        {
            DEBUGMSG(ZONE_ERROR, (TEXT("No usage in cert")));
            fRet = TRUE;
            goto LDone;
        }

        DEBUGMSG(ZONE_ERROR, (TEXT("FGetEKUUsage failed and returned 0x%x"), dwErr));
        goto LDone;
    }

    pUsage = (PCERT_ENHKEY_USAGE)LocalAlloc(LPTR, dwBytes);

    if (NULL == pUsage)
    {
        dwErr = GetLastError();
        DEBUGMSG(ZONE_ERROR,(TEXT("LocalAlloc failed and returned %d"), dwErr));
        goto LDone;
    }

    if (!pfnCertGetEnhancedKeyUsage(pCertContext, 0, pUsage, &dwBytes))
    {
        dwErr = GetLastError();
        DEBUGMSG(ZONE_ERROR, (TEXT("FGetEKUUsage failed and returned 0x%x"), dwErr));
        goto LDone;
    }
    *ppUsage = pUsage;
    fRet = TRUE;
LDone:
    return fRet;
}



/*

Returns:
    TRUE if no enhanced key usages exist, or pCertContext has the 
    szOID_PKIX_KP_SERVER_AUTH or szOID_PKIX_KP_CLIENT_AUTH usage depending on 
    whether fMachine is TRUE or FALSE.

Notes:

*/

BOOL
FCheckUsage(
    IN  PCCERT_CONTEXT      pCertContext,
    IN  PCERT_ENHKEY_USAGE  pUsage,
    IN  BOOL                fMachine
)
{
    DWORD               dwIndex;
    BOOL                fRet        = FALSE;
    PCERT_ENHKEY_USAGE  pUsageInternal = pUsage;
    DEBUGMSG(ZONE_ERROR, (TEXT("FCheckUsage")));

    if ( NULL == pUsageInternal )
    {
        fRet = FGetEKUUsage(pCertContext, &pUsageInternal);
        if(!fRet)
        {
            goto LDone;
        }
    }

    for (dwIndex = 0; dwIndex < pUsageInternal->cUsageIdentifier; dwIndex++)
    {
        if (   (   fMachine
                && !strcmp(pUsageInternal->rgpszUsageIdentifier[dwIndex],
                            szOID_PKIX_KP_SERVER_AUTH))
            || (   !fMachine
                && !strcmp(pUsageInternal->rgpszUsageIdentifier[dwIndex],
                            szOID_PKIX_KP_CLIENT_AUTH)))
        {
            fRet = TRUE;
            break;
        }
    }

LDone:
    if ( NULL == pUsage )
    {
        LocalFree(pUsageInternal);
    }
    return(fRet);
}


PCCERT_CONTEXT
EnumClientCertificates(
    IN HCERTSTORE hCertStore,
    IN PCCERT_CONTEXT pPrevCertContext
    )
{
    PCCERT_CONTEXT pCertContext = pPrevCertContext;
    DWORD cbProp;
    
    for ( ; ; )
    {
        pCertContext = pfnCertEnumCertificatesInStore(hCertStore, pCertContext);
        if (NULL == pCertContext)
        {
            break;
        }
        
        // check for client certificate usage 
        if (!FCheckUsage( pCertContext, NULL, FALSE))
        {
            continue;
        }
        // check for presence of private key
        if (!pfnCertGetCertificateContextProperty(pCertContext, CERT_KEY_PROV_INFO_PROP_ID, NULL, &cbProp))
        {
            continue;
        }

        break;
    }

    return pCertContext;
}




HRESULT
CallGetIdentityDialog(
    HWND hwndParent,
    __in __out_ecount(cchIdentity) LPWSTR pwszIdentity,
    UINT cchIdentity)
{
    HRESULT hr = S_OK;
    NETUI_USERPWD user;
    WCHAR *pszUserNamePtr = NULL;
    ZeroMemory(&user,sizeof(user));
    user.dwFlags = NETUI_USERPWD_NOPWD;

    // Copy the current identity
    StringCchCopyW(user.szUserName, ARRAYSIZE(user.szUserName), pwszIdentity);
    pszUserNamePtr = wcsstr(user.szUserName, TEXT("@"));
    if (NULL != pszUserNamePtr)
    {
        *pszUserNamePtr = L'\0';
    }

    if(!CallGetUsernamePassword (hwndParent, &user))
    {
        CHR(hr = E_FAIL);
    }

    // Combine the username and domain
    if (TEXT('\0') != user.szDomain[0])
    {
        hr = StringCchPrintfExW(pwszIdentity, cchIdentity,
            &pwszIdentity, &cchIdentity, 0, L"%s\\", user.szDomain);
        CHR(hr);
    }

    hr = StringCchCopyW(pwszIdentity, cchIdentity, user.szUserName);
    CHR(hr);

Error:
    return hr;
}


/*

Notes:
    RasEapGetIdentity entry point called by the EAP-PPP engine by name.

*/

DWORD APIENTRY
EapTlsGetIdentity(
    IN  DWORD           dwEapTypeId,
    IN  HWND            hwndParent,
    IN  DWORD           dwFlags,
    IN  const WCHAR*    pwszPhonebook,
    IN  const WCHAR*    pwszEntry,
    IN  BYTE*           pConnectionDataIn,
    IN  DWORD           dwSizeOfConnectionDataIn,
    IN  BYTE*           pUserDataIn,
    IN  DWORD           dwSizeOfUserDataIn,
    OUT BYTE**          ppUserDataOut,
    OUT DWORD*          pcbSizeOfUserDataOut,
    OUT WCHAR**         ppwszIdentity
)
{
    UNREFERENCED_PARAMETER(dwEapTypeId);
    UNREFERENCED_PARAMETER(dwFlags);
    UNREFERENCED_PARAMETER(pwszPhonebook);
    UNREFERENCED_PARAMETER(pwszEntry);
    UNREFERENCED_PARAMETER(pUserDataIn);
    UNREFERENCED_PARAMETER(dwSizeOfUserDataIn);
    
    DWORD               dwErr           = NO_ERROR;
    EAPTLS_CONN_PROPERTIES_V1* pConnProp = NULL;
    HCERTSTORE      hCertStore          = NULL;
    PCCERT_CONTEXT  pCertContext        = NULL;
    CRYPT_HASH_BLOB HashBlob;
    PWCHAR pszUPN = NULL;
    UINT  cchBuffer;
    DWORD cchUPN;

    *ppwszIdentity = NULL;
    *pcbSizeOfUserDataOut = 0;
    *ppUserDataOut = NULL;

    if(FAILED(LoadCAPI2()))
    {
        return ERROR_CANCELLED; //Just get out, nothing here can be done
    }        
    
    pConnProp = InitConnectionData(pConnectionDataIn, dwSizeOfConnectionDataIn, 0);
    if (!pConnProp)
    {
        dwErr = ERROR_CANCELLED;
        goto LDone;
    }
    
    // Open the "MY" certificate store.
    hCertStore = pfnCertOpenStore(
                        CERT_STORE_PROV_SYSTEM_A,
                        X509_ASN_ENCODING,
                        0,
                        CERT_SYSTEM_STORE_CURRENT_USER | CERT_STORE_READONLY_FLAG ,
                        "MY");

    if (NULL == hCertStore)
    {
        dwErr = ERROR_CANCELLED;
        DEBUGMSG(ZONE_ERROR, (TEXT("CertOpenStore failed and returned 0x%x"), dwErr));
        goto LDone;
    }

        // The distinction between UserProp.Hash and ConnProp.UserHash is bogus in CE.
        // They should both be the same.
    if (pConnProp->UserHash.cbHash)
    {
       HashBlob.cbData = pConnProp->UserHash.cbHash;
       HashBlob.pbData = pConnProp->UserHash.pbHash;

       pCertContext = pfnCertFindCertificateInStore(hCertStore, X509_ASN_ENCODING,
                           0, CERT_FIND_HASH, &HashBlob, NULL);

    }
    else
    {        
       // if there is only one client certificate and it has the right UPN name we can guess.
       pCertContext = EnumClientCertificates(hCertStore,  NULL);
       if(!pCertContext)
       {
            // There is no cert on the device, warrning!
            MessageBox(SHGetLastActiveWindow(hwndParent), 
                (PTSTR)LoadString(HINST_RESDLL, IDS_NETUI_EAPTLS_NO_CERT, NULL, 0), 
                (PTSTR)LoadString(HINST_RESDLL, IDS_NETUI_WRN_TITLE_WARNING, NULL, 0),
                MB_OK | MB_SETFOREGROUND);
            dwErr = ERROR_CANCELLED;
            goto LDone;
       }
    }

    // Allocate a buffer for the UPN name
    cchBuffer = DNLEN + 1 + UNLEN + 1;  // domain, backslash, username, null terminator
    pszUPN = (PWCHAR)LocalAlloc(LPTR, cchBuffer * sizeof(pszUPN[0]));  // safe multiplication
    if (!pszUPN)
    {
        dwErr = ERROR_OUTOFMEMORY;
        goto LDone;
    }

    // get the UPN name "joe@redmond.corp.microsoft.com"
    if (pCertContext)
    {
       cchUPN = pfnCertGetNameStringW(pCertContext, CERT_NAME_UPN_TYPE, 0, NULL, pszUPN, UNLEN   );
       if (cchUPN < 2)
       {
           cchUPN = pfnCertGetNameStringW(pCertContext, CERT_NAME_EMAIL_TYPE, 0, NULL, pszUPN, UNLEN   );
       }
            
       pfnCertFreeCertificateContext(pCertContext);
       pCertContext = NULL;
    }
    if (FAILED(CallGetIdentityDialog(hwndParent, pszUPN, cchBuffer)))
    {
       dwErr = ERROR_CANCELLED;
    }
    else
    {
       *ppwszIdentity = pszUPN;
    }
     
LDone:

    if (dwErr)
    {        
        LocalFree(pszUPN);        
    }
    if (pCertContext)
    {
        pfnCertFreeCertificateContext(pCertContext);
    }
        
    if (hCertStore)
    {
        pfnCertCloseStore(hCertStore, 0);
    }

    FreeCAPI2();
    return(dwErr);
}


/*

Notes:
    RasEapGetIdentity entry point called by the EAP-PPP engine by name.

*/

extern "C"
DWORD APIENTRY
RasEapGetIdentity(
    IN  DWORD           dwEapTypeId,
    IN  HWND            hwndParent,
    IN  DWORD           dwFlags,
    IN  const WCHAR*    pwszPhonebook,
    IN  const WCHAR*    pwszEntry,
    IN  BYTE*           pConnectionDataIn,
    IN  DWORD           dwSizeOfConnectionDataIn,
    IN  BYTE*           pUserDataIn,
    IN  DWORD           dwSizeOfUserDataIn,
    OUT BYTE**          ppUserDataOut,
    OUT DWORD*          pcbSizeOfUserDataOut,
    OUT WCHAR**         ppwszIdentity
)
{
    DWORD               dwErr           = NO_ERROR;

    switch(dwEapTypeId)
    {
    case PPP_EAP_TLS:
        dwErr = EapTlsGetIdentity(
                    dwEapTypeId,
                    hwndParent,
                    dwFlags,
                    pwszPhonebook,
                    pwszEntry,
                    pConnectionDataIn,
                    dwSizeOfConnectionDataIn,
                    pUserDataIn,
                    dwSizeOfUserDataIn,
                    ppUserDataOut,
                    pcbSizeOfUserDataOut,
                    ppwszIdentity);
        break;

    case PPP_EAP_PEAP:
        // PEAP uses the default username dialog, so this
        // should not be called.
        dwErr = ERROR_NOT_SUPPORTED;
        break;

    default:
        DEBUGMSG(ZONE_ERROR, (TEXT("EAP Type %d is not supported"), dwEapTypeId));
        dwErr = ERROR_NOT_SUPPORTED;
        break;
    }

    return dwErr;
}




/*

/*
RasEapFreeMemory (PUBLIC)
Returns:

Notes:
    Called to free memory.

*/

extern "C"
DWORD 
RasEapFreeMemory(
    IN  BYTE*   pMemory
)
{
    LocalFree(pMemory);
    return NO_ERROR;
}

