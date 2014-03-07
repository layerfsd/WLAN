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
#include <windowsx.h>
#include <deviceid.h>   // For UNIQUE_DEVICEID
#include <wincrypt.h>
#include <inetreg.h>
#include "encrypt.h"

#define REGSTR_PATH_PASSWORDS   REGSTR_PATH_INTERNETSETTINGS TEXT("\\Passwords")
#define REGSTR_VAL_RANDOMKEY    TEXT("##Crypt_Key##")

#define MAX_RANDOMKEY_BYTE_LEN  4
#define MAX_ENTROPY_BYTE_LEN    sizeof(UNIQUE_DEVICEID)

/////////////////////////////////////////////////////////////////////////////
// GetRandomKey
/////////////////////////////////////////////////////////////////////////////
HRESULT GetRandomKey
(
    BYTE *  pbRandom,
    DWORD * pcbRandomLen
)
{
    HKEY    hKey;
    HRESULT hrResult;
    LONG    lRegResult;

    ASSERT(pbRandom != NULL);
    ASSERT(pcbRandomLen != NULL);
    ASSERT(*pcbRandomLen > 0);

    // First, check the registry to see if we have generated the key.
    lRegResult = RegOpenKeyEx(  HKEY_CURRENT_USER,
                                REGSTR_PATH_PASSWORDS,
                                0,
                                0,
                                &hKey);
    if (lRegResult == ERROR_SUCCESS)
    {
        DWORD dwType;
        lRegResult = RegQueryValueEx(   hKey,
                                        REGSTR_VAL_RANDOMKEY,
                                        NULL,
                                        &dwType,
                                        pbRandom,
                                        pcbRandomLen);
        if (lRegResult == ERROR_SUCCESS)
        {
            ASSERT(dwType == REG_BINARY);
            hrResult = S_OK;
        }
        else
            hrResult = HRESULT_FROM_WIN32(lRegResult);

        VERIFY(RegCloseKey(hKey) == ERROR_SUCCESS);
    }
    else
        hrResult = HRESULT_FROM_WIN32(lRegResult);

    // Nothing in the registry, yet. So, generate a random key.
    if (FAILED(hrResult))
    {
        HCRYPTPROV hProv = NULL;

        if  (
            CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)
            ||
            CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)
            )
        {
            // Generate the key
            if (CryptGenRandom(hProv, *pcbRandomLen, pbRandom))
            {
                // Save the key in the registry.
                lRegResult = RegCreateKeyEx(HKEY_CURRENT_USER,
                                            REGSTR_PATH_PASSWORDS,
                                            0,
                                            NULL,
                                            0,
                                            KEY_ALL_ACCESS,
                                            NULL,
                                            &hKey,
                                            NULL);
                if (lRegResult == ERROR_SUCCESS)
                {
                    lRegResult = RegSetValueEx( hKey,
                                                REGSTR_VAL_RANDOMKEY,
                                                0,
                                                REG_BINARY,
                                                pbRandom,
                                                *pcbRandomLen);

                    VERIFY(RegCloseKey(hKey) == ERROR_SUCCESS);
                }

                hrResult = HRESULT_FROM_WIN32(lRegResult);
            }
            else
            {
                hrResult = HRESULT_FROM_WIN32(GetLastError());
            }

            VERIFY(CryptReleaseContext(hProv, 0));
        }
        else
        {
            hrResult = HRESULT_FROM_WIN32(GetLastError());
        }
        
    }

    return hrResult;
}

/////////////////////////////////////////////////////////////////////////////
// GetExtraEntropy
/////////////////////////////////////////////////////////////////////////////
HRESULT GetExtraEntropy
(
    BYTE *  pbEntropy,
    DWORD   dwEntropyLen
)
{
    DWORD cbOut;

    ZeroMemory(pbEntropy, dwEntropyLen);

    if (!KernelIoControl(   IOCTL_HAL_GET_DEVICEID,
                            NULL,
                            0,
                            pbEntropy,
                            dwEntropyLen,
                            &cbOut))
    {
        FillMemory(pbEntropy, dwEntropyLen, 0x8B);
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// SetRegistryCredentials
/////////////////////////////////////////////////////////////////////////////
HRESULT SetRegistryCredentials
(
    LPCTSTR pszRealm,
    BYTE *  pbCredentials,
    DWORD   cbCredentialsLen
)
{
    HKEY    hKey;
    LONG    lRegResult;

    lRegResult = RegOpenKeyEx(  HKEY_CURRENT_USER,
                                REGSTR_PATH_PASSWORDS,
                                0,
                                0,
                                &hKey);
    if (lRegResult == ERROR_SUCCESS)
    {
        lRegResult = RegSetValueEx( hKey,
                                    pszRealm,
                                    0,
                                    REG_BINARY,
                                    pbCredentials,
                                    cbCredentialsLen);

        VERIFY(RegCloseKey(hKey) == ERROR_SUCCESS);
    }

    return HRESULT_FROM_WIN32(lRegResult);
}

/////////////////////////////////////////////////////////////////////////////
// GetRegistryCredentials
/////////////////////////////////////////////////////////////////////////////
HRESULT GetRegistryCredentials
(
    LPCTSTR pszRealm,
    BYTE ** ppbCredentials,
    DWORD * pcbCredentialsLen
)
{
    HKEY    hKey;
    HRESULT hrResult;
    LONG    lRegResult;

    ASSERT(pszRealm != NULL);
    ASSERT(ppbCredentials != NULL);
    ASSERT(pcbCredentialsLen != NULL);
 
    *ppbCredentials     = NULL;
    *pcbCredentialsLen  = 0;

    lRegResult = RegOpenKeyEx(  HKEY_CURRENT_USER,
                                REGSTR_PATH_PASSWORDS,
                                0,
                                0,
                                &hKey);
    if (lRegResult == ERROR_SUCCESS)
    {
        DWORD dwType;
        
        // First, get the length of the credentials.
        lRegResult = RegQueryValueEx(   hKey,
                                        pszRealm,
                                        0,
                                        &dwType,
                                        NULL,
                                        pcbCredentialsLen);
        if (lRegResult == ERROR_SUCCESS)
        {
            ASSERT(dwType == REG_BINARY);

            *ppbCredentials = new BYTE [*pcbCredentialsLen];

            if (*ppbCredentials != NULL)
            {
                lRegResult = RegQueryValueEx(   hKey,
                                                pszRealm,
                                                0,
                                                &dwType,
                                                *ppbCredentials,
                                                pcbCredentialsLen);
                if (lRegResult == ERROR_SUCCESS)
                {
                    hrResult = S_OK;
                }
                else
                {
                    hrResult = HRESULT_FROM_WIN32(lRegResult);
                    if (SUCCEEDED(hrResult))
                    {
                        hrResult = E_FAIL;
                    }
                }
            }
            else
                hrResult = E_OUTOFMEMORY;
        }
        else
        {
            hrResult = HRESULT_FROM_WIN32(lRegResult);
            if (SUCCEEDED(hrResult))
            {
                hrResult = E_FAIL;
            }
        }

        VERIFY(RegCloseKey(hKey) == ERROR_SUCCESS);
    }
    else
    {
        hrResult = HRESULT_FROM_WIN32(lRegResult);
    }

    // Cleanup on failure.
    if (FAILED(hrResult))
    {
        if (*ppbCredentials != NULL)
        {
            delete [] (*ppbCredentials);
            *ppbCredentials = NULL;
        }

        *pcbCredentialsLen = 0;
    }

    return hrResult;
}

/////////////////////////////////////////////////////////////////////////////
// SetCachedCredentials
/////////////////////////////////////////////////////////////////////////////
HRESULT SetCachedCredentials
(
    LPCTSTR pszRealm,
    LPCTSTR pszUsername,
    LPCTSTR pszPassword
)
{
    BYTE    rgbRandomKey[MAX_RANDOMKEY_BYTE_LEN];
    DWORD   cbRandomKey = MAX_RANDOMKEY_BYTE_LEN;
    BYTE    rgbEntropy[MAX_ENTROPY_BYTE_LEN];
    DWORD   cbEntropy = MAX_ENTROPY_BYTE_LEN;
    HRESULT hrResult;

    ASSERT(pszRealm != NULL);
    ASSERT(pszUsername != NULL);
    ASSERT(pszPassword != NULL);

    if  (
        SUCCEEDED(hrResult = GetRandomKey(rgbRandomKey, &cbRandomKey))
        &&
        SUCCEEDED(hrResult = GetExtraEntropy(rgbEntropy, cbEntropy))
        )
    {
        HCRYPTPROV  hCryptProv = NULL;
        HCRYPTHASH  hCryptHash = NULL;
        HCRYPTKEY   hCryptKey = NULL;

        if  (
            (
                CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)
                ||
                CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0)
            )
            &&
            CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hCryptHash)
            &&
            CryptHashData(hCryptHash, rgbRandomKey, cbRandomKey, 0)
            &&
            CryptHashData(hCryptHash, rgbEntropy, cbEntropy, 0)
            &&
            CryptDeriveKey( hCryptProv,
                            CALG_RC4,
                            hCryptHash,
                            CRYPT_EXPORTABLE,
                            &hCryptKey)
            )
        {
            DWORD   cbUsername      = (_tcslen(pszUsername)+1) * sizeof(TCHAR);
            DWORD   cbPassword      = (_tcslen(pszPassword)+1) * sizeof(TCHAR);
            DWORD   cbCredentials   = (cbUsername + cbPassword);
            BYTE *  pbCredentials   = new BYTE [cbCredentials];

            if (pbCredentials != NULL)
            {
                DWORD cbEncrypted = cbCredentials;

                CopyMemory(pbCredentials, pszUsername, cbUsername);
                CopyMemory(pbCredentials+cbUsername, pszPassword, cbPassword);

                if (CryptEncrypt(   hCryptKey,
                                    0,
                                    TRUE,
                                    0,
                                    pbCredentials,
                                    &cbEncrypted,
                                    cbCredentials))
                {
                    hrResult = SetRegistryCredentials(  pszRealm,
                                                        pbCredentials,
                                                        cbCredentials);
                }
                else
                    hrResult = HRESULT_FROM_WIN32(GetLastError());

                // For security, clear the credential buffer
                memset(pbCredentials, 0, cbCredentials);
                delete [] pbCredentials;
            }
            else
                hrResult = E_OUTOFMEMORY;
        }
        else
            hrResult = HRESULT_FROM_WIN32(GetLastError());

        if (hCryptKey != NULL)
            VERIFY(CryptDestroyKey(hCryptKey));

        if (hCryptHash != NULL)
            VERIFY(CryptDestroyHash(hCryptHash));

        if (hCryptProv != NULL)
            VERIFY(CryptReleaseContext(hCryptProv, 0));
    }

    return hrResult;
}

/////////////////////////////////////////////////////////////////////////////
// GetCachedCredentials
/////////////////////////////////////////////////////////////////////////////
HRESULT GetCachedCredentials
(
    LPTSTR  pszRealm,
    LPTSTR  pszUsername,
    DWORD   cchUsername,
    LPTSTR  pszPassword,
    DWORD   cchPassword
)
{
    BYTE *  pbCredentials;
    DWORD   dwCredentialsLen;
    HRESULT hrResult;

    ASSERT(pszRealm != NULL);
    ASSERT(pszUsername != NULL);
    ASSERT(cchUsername > 0);
    ASSERT(pszPassword != NULL);
    ASSERT(cchPassword > 0);

    // First, attempt to get the cached credentials from the registry.
    if (SUCCEEDED(hrResult = GetRegistryCredentials(pszRealm,
                                                    &pbCredentials,
                                                    &dwCredentialsLen)))
    {
        ASSERT(pbCredentials != NULL);
        ASSERT(dwCredentialsLen > 0);

        BYTE    rgbRandomKey[MAX_RANDOMKEY_BYTE_LEN];
        DWORD   cbRandomKey = MAX_RANDOMKEY_BYTE_LEN;
        BYTE    rgbEntropy[MAX_ENTROPY_BYTE_LEN];
        DWORD   cbEntropy = MAX_ENTROPY_BYTE_LEN;

        if  (
            SUCCEEDED(hrResult = GetRandomKey(rgbRandomKey, &cbRandomKey))
            &&
            SUCCEEDED(hrResult = GetExtraEntropy(rgbEntropy, cbEntropy))
            )
        {
            HCRYPTPROV  hCryptProv = NULL;
            HCRYPTHASH  hCryptHash = NULL;
            HCRYPTKEY   hCryptKey = NULL;

            if  (
                (
                    CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)
                    ||
                    CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0)
                )
                &&
                CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hCryptHash)
                &&
                CryptHashData(hCryptHash, rgbRandomKey, cbRandomKey, 0)
                &&
                CryptHashData(hCryptHash, rgbEntropy, cbEntropy, 0)
                &&
                CryptDeriveKey( hCryptProv,
                                CALG_RC4,
                                hCryptHash,
                                CRYPT_EXPORTABLE,
                                &hCryptKey)
                )

            {
                if (CryptDecrypt(   hCryptKey,
                                    0,
                                    TRUE,
                                    0,
                                    pbCredentials,
                                    &dwCredentialsLen))
                {
                    TCHAR * pszCredentials = (TCHAR *)pbCredentials;

                    // Parse off Username
                    while   (
                            (*pszCredentials != TEXT('\0'))
                            &&
                            (cchUsername > 1)
                            )
                    {
                        *pszUsername++ = *pszCredentials++;
                        cchUsername--;
                    }
                    *pszUsername = TEXT('\0');

                    // Skip NULL
                    pszCredentials++;

                    // Parse off Password
                    while   (
                            (*pszCredentials != TEXT('\0'))
                            &&
                            (cchPassword > 1)
                            )
                    {
                        *pszPassword++ = *pszCredentials++;
                        cchPassword--;
                    }
                    *pszPassword = TEXT('\0');

                    hrResult = S_OK;
                }
                else
                    hrResult = HRESULT_FROM_WIN32(GetLastError()); 
            }
            else
                hrResult = HRESULT_FROM_WIN32(GetLastError());

            if (hCryptKey != NULL)
                VERIFY(CryptDestroyKey(hCryptKey));

            if (hCryptHash != NULL)
                VERIFY(CryptDestroyHash(hCryptHash));

            if (hCryptProv != NULL)
                VERIFY(CryptReleaseContext(hCryptProv, 0));
        }

        if (pbCredentials != NULL)
        {
            // For security, clear the credential buffer
            memset(pbCredentials, 0, dwCredentialsLen);
            delete [] pbCredentials;
        }
    }

    return hrResult;
}

/////////////////////////////////////////////////////////////////////////////
// ClearCachedCredential
/////////////////////////////////////////////////////////////////////////////
HRESULT ClearCachedCredential
(
    LPCTSTR  pszRealm
)
{
    HRESULT hrResult;
    HKEY    hKey;
    LONG    lRegResult;

    ASSERT(pszRealm != NULL);

    // Delete the cached credentials from the registry.
    lRegResult = RegOpenKeyEx(  HKEY_CURRENT_USER,
                                REGSTR_PATH_PASSWORDS,
                                0,
                                0,
                                &hKey);
    if (lRegResult == ERROR_SUCCESS)
    {
        lRegResult = RegDeleteValue(hKey,
                                    pszRealm);
        if (lRegResult == ERROR_SUCCESS)
        {
            hrResult = S_OK;
        }
        else
        {
            hrResult = HRESULT_FROM_WIN32(lRegResult);
            if (SUCCEEDED(hrResult))
            {
                hrResult = E_FAIL;
            }
        }

        VERIFY(RegCloseKey(hKey) == ERROR_SUCCESS);
    }
    else
    {
        hrResult = HRESULT_FROM_WIN32(lRegResult);
    }
    
    return hrResult;
}

/////////////////////////////////////////////////////////////////////////////
// GetNetworkCredentials
/////////////////////////////////////////////////////////////////////////////
HRESULT GetNetworkCredentials
(
    LPCTSTR pszUsername,
    LPCTSTR pszPassword
)
{
    return E_NOTIMPL;
}

#ifdef FULL_DEBUG
/////////////////////////////////////////////////////////////////////////////
// TestCredentials
/////////////////////////////////////////////////////////////////////////////
HRESULT TestCredentials
(
)
{
    TCHAR   szUsername[UNLEN];
    TCHAR   szPassword[PWLEN];
    HRESULT hrResult;

    // This one should fail.
    hrResult = GetCachedCredentials(TEXT("realm"),
                                    szUsername,
                                    ARRAYSIZE(szUsername),
                                    szPassword,
                                    ARRAYSIZE(szPassword));

    // These should pass
    hrResult = SetCachedCredentials(TEXT("REDMOND"),
                                    TEXT("test"),
                                    TEXT("fubartesting"));

    hrResult = SetCachedCredentials(TEXT("HOMERUNSS"),
                                    TEXT("12345"),
                                    TEXT("!@#$%^&"));

    // These should pass, too.
    hrResult = GetCachedCredentials(TEXT("REDMOND"),
                                    szUsername,
                                    ARRAYSIZE(szUsername),
                                    szPassword,
                                    ARRAYSIZE(szPassword));

    hrResult = GetCachedCredentials(TEXT("HOMERUNSS"),
                                    szUsername,
                                    ARRAYSIZE(szUsername),
                                    szPassword,
                                    ARRAYSIZE(szPassword));

    return S_OK;
}
#endif  // FULL_DEBUG
