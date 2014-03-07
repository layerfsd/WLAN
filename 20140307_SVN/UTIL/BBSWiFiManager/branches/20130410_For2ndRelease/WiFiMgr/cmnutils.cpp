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
//==========================================================================*
//
// Filename:    cmnutils.cpp
//
// Purpose:     NetUI utility code
//
//==========================================================================*
//==========================================================================*

#include <windows.h>
#include <wpcpriv.h>
#include <ehm.h>
#include <windev.h>
#include <connmgr.h>
#include <comutil_priv.h>
#include <iphlpapi.h>
#include <eapol.h>
#include <nuiouser.h>

#include <regext.h>

#include "netuip.h"
#include "wzcuip.h"
#include "cmnutils.h"
#include "wzcmgr.h"
#include <resource.h>


///<file_topic_scope tref="connections"/>

static const TCHAR c_szRegKeySSWifi[]             = TEXT("System\\State\\Hardware");
static const TCHAR c_szRegValueSSWifi[]           = TEXT("Wifi");

extern LPCTSTR     g_szRegKeyBase;
static const TCHAR g_szRegKeyEAP[]        = TEXT("EapolParam");
static const TCHAR g_szRegValueEAPType[]  = TEXT("EapType");
static const TCHAR g_szRegValueEAPFlags[] = TEXT("EapFlags");
const char szAutoIPAddrMask[] = "169.254.";     // 169.254.x.x is AutoIP

const TCHAR c_szEllipsis[] = TEXT("...");

typedef struct _METANETWORKINFO
{
    TCHAR szGUID[48];
    TCHAR szName[MAX_PATH];
} METANETWORKINFO;

const GUID *g_rgpguidMetaNetworks[] =
{
    &IID_DestNetInternet,
    &IID_DestNetCorp
};

static METANETWORKINFO g_rgmni[ARRAYSIZE(g_rgpguidMetaNetworks)];

UINT GetMetaNetworkCount()
{
    return ARRAYSIZE(g_rgpguidMetaNetworks);
}

LPCTSTR GetMetaNetworkGUIDString(UINT uIndex)
{
    LPCTSTR pszGUID = NULL;

    if (uIndex < ARRAYSIZE(g_rgmni))
    {
        // Only recreate the string if we haven't already done it
        if (0 == g_rgmni[uIndex].szGUID[0])
        {
            // This ONLY fails if we don't have enough buffer
            VBR(SHStringFromGUID(*(g_rgpguidMetaNetworks[uIndex]),
                g_rgmni[uIndex].szGUID, ARRAYSIZE(g_rgmni[uIndex].szGUID)));
        }

        // Success
        pszGUID = g_rgmni[uIndex].szGUID;
    }

    // Return NULL if the network doesn't exist
    return pszGUID;
}

static UINT MatchMetaNetworkIndex(REFGUID rguid)
{
    UINT i;

    for (i = 0; i != ARRAYSIZE(g_rgpguidMetaNetworks); ++i)
    {
        if (IsEqualGUID(rguid, *(g_rgpguidMetaNetworks[i])))
        {
            break;
        }
    }

    return i;
}

HRESULT GetMetaNetworkIndex(LPCTSTR pszGUID, UINT *piMetaNetwork)
{
    HRESULT hr = S_OK;
    GUID    guid;
    UINT    i;

    ASSERT(NULL != pszGUID);
    ASSERT(NULL != piMetaNetwork);

    // Convert the string to a GUID
    CBR(SHGUIDFromString(pszGUID, &guid));

    // Get the GUID index
    i = MatchMetaNetworkIndex(guid);
    CBR(i < ARRAYSIZE(g_rgmni));

    // Success
    *piMetaNetwork = i;

Error:
    return hr;
}

static HRESULT EnumMetaNetworkNamesCallback(UINT iIndex, LPCTSTR pszName, LPVOID pvParam)
{
    ASSERT(NULL != pszName);
    ASSERT(iIndex < ARRAYSIZE(g_rgmni));

    return StringCchCopy(g_rgmni[iIndex].szName, ARRAYSIZE(g_rgmni[iIndex].szName), pszName);
}

static HRESULT GetMetaNetworkNames()
{
    HRESULT hr = S_OK;
    UINT    i;

    if (0 == g_rgmni[0].szName[0])
    {
        // Enumerate the entries
        hr = EnumMetaNetworkNames(EnumMetaNetworkNamesCallback, NULL);
        CHR(hr);

        // Make sure we got them all
        for (i = 0; i != ARRAYSIZE(g_rgmni); ++i)
        {
            CBR(0 != g_rgmni[i].szName[0]);
        }
    }

Error:
    if (FAILED(hr))
    {
        // Make sure we retry later
        g_rgmni[0].szName[0] = TEXT('\0');
    }

    return hr;
}

HRESULT GetMetaNetworkName(UINT uIndex, LPCTSTR *ppszName)
{
    HRESULT hr = S_OK;

    // WHY must you send me an invalid index??
    CBRAEx(uIndex < ARRAYSIZE(g_rgmni), E_INVALIDARG);
    ASSERT(NULL != ppszName);

    // Make sure we have the network names
    hr = GetMetaNetworkNames();
    CHR(hr);

    // Success
    *ppszName = g_rgmni[uIndex].szName;

 Error:
    return hr;
}

// NOTE: Uses hard-coded value just like shell.cpp
BOOL IsAutoIPAddr(char* szAddress)
{
    return (0 == strncmp(szAddress, szAutoIPAddrMask, ARRAYSIZE(szAutoIPAddrMask)-1));
}

extern "C"
IPSTATE GetIPState(LPCTSTR pszAdapter, LPTSTR pszAddress, UINT cchAddress)
{
    HRESULT              hr = S_OK;
    IPSTATE              state = IPSTATE_DHCP;
    ULONG                ulAdapterIndex;
    PIP_ADAPTER_INFO     paiBuffer = NULL;
    ULONG                cbBuffer = 0;
    DWORD                dwError;
    PIP_ADAPTER_INFO     paiCurrent;
    PIP_PER_ADAPTER_INFO pper = NULL;

    // Check inputs
    CBRAEx(NULL != pszAdapter, E_INVALIDARG);

    // Get the adapter index
    CBR(NO_ERROR == GetAdapterIndex(const_cast<LPWSTR>(pszAdapter), &ulAdapterIndex));

    // Get the status of all connected adapters
    cbBuffer = sizeof(IP_ADAPTER_INFO);
    paiBuffer = (PIP_ADAPTER_INFO)LocalAlloc(LMEM_FIXED, cbBuffer);
    CPR(paiBuffer);

    while (ERROR_BUFFER_OVERFLOW == (dwError = GetAdaptersInfo(paiBuffer, &cbBuffer)))
    {
        PIP_ADAPTER_INFO paiTemp = (PIP_ADAPTER_INFO)LocalReAlloc(paiBuffer, cbBuffer, LMEM_MOVEABLE);
        CPR(paiTemp);
        paiBuffer = paiTemp;
    }

    CBR(NO_ERROR == dwError);

    // Find our adapter index
    for (paiCurrent = paiBuffer; paiCurrent; paiCurrent = paiCurrent->Next)
    {
        if (paiCurrent->Index == ulAdapterIndex)
        {
            break;
        }
    }

    CBR(NULL != paiCurrent);  // did we find it?
    CBR(NULL != paiCurrent->CurrentIpAddress);

    // Is this adapter using DHCP?
    if (!paiCurrent->DhcpEnabled)
    {
        state = IPSTATE_CONNECTED;
    }
    else
    {
        // Get the per-adapter info
        cbBuffer = sizeof(IP_PER_ADAPTER_INFO);
        pper = (PIP_PER_ADAPTER_INFO)LocalAlloc(LMEM_FIXED, cbBuffer);
        CPR(pper);

        while (ERROR_BUFFER_OVERFLOW == (dwError = GetPerAdapterInfo(ulAdapterIndex, pper, &cbBuffer)))
        {
            PIP_PER_ADAPTER_INFO pperTemp = (PIP_PER_ADAPTER_INFO)LocalReAlloc(pper, cbBuffer, LMEM_MOVEABLE);
            CPR(pperTemp);
            pper = pperTemp;
        }

        CBR(NO_ERROR == dwError);

        // Are we using an automatic IP?
        if (pper->AutoconfigActive || IsAutoIPAddr(paiCurrent->CurrentIpAddress->IpAddress.String))
        {
            state = IPSTATE_AUTOIP;
        }
    }

    // Optionally copy the IP address string to the output
    if (NULL != pszAddress && 0 != cchAddress)
    {
        UINT cchInput = strlen(paiCurrent->CurrentIpAddress->IpAddress.String);
        if (cchInput >= cchAddress)
        {
            cchInput = cchAddress - 1;
        }

        MultiByteToWideChar(CP_ACP, 0,
            paiCurrent->CurrentIpAddress->IpAddress.String, cchInput,
            pszAddress, cchAddress);
        pszAddress[cchAddress-1] = TEXT('\0');
    }

Error:
    LocalFree(pper);
    LocalFree(paiBuffer);
    return (SUCCEEDED(hr) ? state : IPSTATE_DISCONNECTED);
}
 
/// <remarks>
/// Note that this is essentially a duplicate of IsDeviceWireless in ethman
/// (which is used to give us fWireless in AddNetUISystrayIcon).
/// </remarks>
extern "C"
DEVICETYPE GetDeviceInfo(__in LPCTSTR pszAdapter, __out __opt int *pnRSSI)
{
    HRESULT               hr = S_OK;
    NATIVEWIFI_STATISTICS nws = { 0 };
    DWORD                 cbWritten = 0;
    DEVICETYPE            dtType = DEVICE_WIRED;
    int                   nRSSI = -92;  // no signal

    nws.ptcDeviceName = (PTCHAR)pszAdapter;
    if (DeviceIoControl(g_hNdisUio, IOCTL_NDISUIO_NATIVEWIFI_STATISTICS, NULL, 0,
        &nws, sizeof(NATIVEWIFI_STATISTICS), &cbWritten, NULL))
    {
        if (nws.PhysicalMediaType == NdisPhysicalMediumWirelessLan)
        {
            // Old-style wireless device
            dtType = DEVICE_WIRELESS;
            if (NULL != pnRSSI)
            {
                BYTE               rgbBuffer[sizeof(NDISUIO_QUERY_OID)+sizeof(DWORD)];
                PNDISUIO_QUERY_OID pQuery = (PNDISUIO_QUERY_OID)&rgbBuffer[0];

                // Query the device for its RSSI
                pQuery->ptcDeviceName = (PTCHAR)pszAdapter;
                pQuery->Oid = OID_802_11_RSSI;

                if (DeviceIoControl(g_hNdisUio, IOCTL_NDISUIO_QUERY_OID_VALUE,
                    (LPVOID)pQuery, sizeof(rgbBuffer),
                    (LPVOID)pQuery, sizeof(rgbBuffer),
                    &cbWritten, NULL))
                {
                    nRSSI = *((INT32*)&pQuery->Data);
                }
            }
        }
        else if (nws.PhysicalMediaType == NdisPhysicalMediumNativeWifi && nws.bStationMode)
        {
            // Native Wi-Fi device
            dtType = DEVICE_WIFI_NATIVE;
            nRSSI = nws.ulRSSI;
        }
    }

    // Return the signal strength if requested
    if (NULL != pnRSSI)
    {
        *pnRSSI = nRSSI;
    }

    return dtType;
}

static HRESULT OpenEAPRegistryKey(UINT iIndex, __out HKEY *phkey)
{
    HRESULT hr = S_OK;
    TCHAR   szKey[MAX_PATH];

    // Build the registry path
    hr = StringCchPrintf(szKey, ARRAYSIZE(szKey), TEXT("%s\\%s%u"),
        g_szRegKeyBase, g_szRegKeyEAP, iIndex + 1);
    CHRA(hr);

    // Open the registry key
    CBR(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, 0, phkey));

Error:
    return hr;
}

static HRESULT GetEAPParams(UINT iIndex, __inout WZC_EAPOL_PARAMS *pwzceap)
{
    HRESULT hr = S_OK;
    HKEY    hkey = NULL;
    DWORD   dwValueType;
    DWORD   cbSize;

    // Open the default type key
    hr = OpenEAPRegistryKey(iIndex, &hkey);
    CHR(hr);

    // Get the type code
    cbSize = sizeof(pwzceap->dwEapType);
    CBR(ERROR_SUCCESS == RegQueryValueEx(hkey, g_szRegValueEAPType, NULL, &dwValueType,
        (BYTE*)&pwzceap->dwEapType, &cbSize) && REG_DWORD == dwValueType);

    // Get the flags
    cbSize = sizeof(pwzceap->dwEapFlags);
    CBR(ERROR_SUCCESS == RegQueryValueEx(hkey, g_szRegValueEAPFlags, NULL, &dwValueType,
        (BYTE*)&pwzceap->dwEapFlags, &cbSize) && REG_DWORD == dwValueType);

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

extern "C"
HRESULT GetEAPDefaults(PWLC pwlc)
{
    HRESULT hr = S_OK;

    // Adhoc networks can't use 802.1x
    CBREx(Ndis802_11IBSS != pwlc->InfrastructureMode, E_INVALIDARG);

    // Initialize the defaults
    pwlc->EapolParams.bEnable8021x = TRUE;
    pwlc->EapolParams.dwEapType = DEFAULT_EAP_TYPE;
    pwlc->EapolParams.dwEapFlags = DEFAULT_EAP_STATE;

    // WPA[2]-PSK always uses the defaults, but for other types read the defaults from the registry
    if (! IsAnyWPAPSK(pwlc->AuthenticationMode))
    {
        // Use the first state index
        hr = GetEAPParams(0, &pwlc->EapolParams);
        CHREx(hr, S_FALSE);
    }

Error:
    return hr;
}

extern "C"
HRESULT GetEAPDefaultType(DWORD *pdwType)
{
    HRESULT hr = S_OK;
    HKEY    hkey = NULL;
    DWORD   dwValueType;
    DWORD   cbSize;

    // Open the default type key
    hr = OpenEAPRegistryKey(0, &hkey);
    CHR(hr);

    // Get the type code
    cbSize = sizeof(*pdwType);
    CBR(ERROR_SUCCESS == RegQueryValueEx(hkey, g_szRegValueEAPType, NULL, &dwValueType,
        (BYTE*)pdwType, &cbSize) && REG_DWORD == dwValueType);

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

static HRESULT FindEAPRegistryIndex(DWORD dwFindType, __out __opt UINT *piIndex, __out __opt HKEY *phkey)
{
    HRESULT hr = S_OK;
    UINT    iIndex;
    HKEY    hkey = NULL;
    DWORD   dwEAPType;
    DWORD   dwValueType;
    DWORD   cbSize;

    for (iIndex = 0;; ++iIndex)
    {
        // Try to open the registry key at this index
        hr = OpenEAPRegistryKey(iIndex, &hkey);
        CHR(hr);

        // Get the associated type
        cbSize = sizeof(dwEAPType);
        CBR(ERROR_SUCCESS == RegQueryValueEx(hkey, g_szRegValueEAPType, NULL, &dwValueType,
            (BYTE*)&dwEAPType, &cbSize) && REG_DWORD == dwValueType);

        // Do they match?
        if (dwEAPType == dwFindType)
        {
            break;
        }

        // Close the registry key
        RegCloseKey(hkey);
        hkey = NULL;
    }

    // We got a match
    if (NULL != piIndex)
    {
        *piIndex = iIndex;
    }

    if (NULL != phkey)
    {
        *phkey = hkey;
        hkey = NULL;
    }

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

extern "C"
HRESULT GetEAPFlags(DWORD dwType, DWORD *pdwFlags)
{
    HRESULT hr = S_OK;
    HKEY    hkey = NULL;
    DWORD   dwValueType;
    DWORD   cbSize;

    // Find the registry index that matches this type
    hr = FindEAPRegistryIndex(dwType, NULL, &hkey);
    CHR(hr);

    // Get the default flags for this type
    cbSize = sizeof(*pdwFlags);
    CBR(ERROR_SUCCESS == RegQueryValueEx(hkey, g_szRegValueEAPFlags, NULL, &dwValueType,
        (BYTE*)pdwFlags, &cbSize) && REG_DWORD == dwValueType);

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

extern "C"
HRESULT GetEAPIncremented(PWLC pwlc)
{
    HRESULT hr = S_OK;
    UINT    iIndex;

    // If EAP hasn't been enabled, just start with the initial defaults
    if (!pwlc->EapolParams.bEnable8021x)
    {
        hr = GetEAPDefaults(pwlc);
        CHR(hr);
        goto Exit;
    }

    // WPA[2]-PSK doesn't increment through the EAP types
    CBR(! IsAnyWPAPSK(pwlc->AuthenticationMode));
    
    // Find the index of the current type
    hr = FindEAPRegistryIndex(pwlc->EapolParams.dwEapType, &iIndex, NULL);
    CHR(hr);

    // Set the parameters for the next type
    hr = GetEAPParams(iIndex + 1, &pwlc->EapolParams);
    CHR(hr);

Error:
Exit:
    return hr;
}

extern "C"
HRESULT SetWirelessStateFlags(UINT uMask, UINT uValue)
{
    HRESULT hr = S_OK;
    DWORD   dwData = 0;
    HKEY    hKey = NULL;

    CBR(ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, c_szRegKeySSWifi,
                                        0, NULL, REG_OPTION_VOLATILE, 0, NULL, &hKey, NULL));

    RegistryGetDWORD(hKey, NULL, c_szRegValueSSWifi, &dwData);
    dwData = (dwData & ~uMask) | (uMask & uValue);
    CHR(RegistrySetDWORD(hKey, NULL, c_szRegValueSSWifi, dwData));

Error:
    RegCloseKey(hKey);
    return hr;
}

extern "C"
HRESULT GetWirelessStateFlags(DWORD *pdwStatus)
{
    HRESULT hr = S_OK;

    CBR(pdwStatus);   
    *pdwStatus = 0;
    
    CHR(RegistryGetDWORD(HKEY_LOCAL_MACHINE, c_szRegKeySSWifi, c_szRegValueSSWifi, pdwStatus));
    
Error:
    return hr;
}

/// <summary>
/// Determines if a network is secure, based on authentication and encryption
/// properties. Optionally formats the SSID string for display in the UI.
/// </summary>
/// <returns>
///   HRESULT
///     S_OK if secure.
///     S_FALSE if not secure.
/// </returns>
/// <param name="pszSSID">
/// The network's SSID.
/// </param>
/// <param name="fUse8021x">
/// TRUE if the network uses 802.1x, FALSE otherwise.
/// </param>
/// <param name="dwAuthentication">
/// Authentication type for the network. See Ndis802_11AuthModeXXXX in
/// ntddndis.h
/// </param>
/// <param name="dwEncryption">
/// Encryption type for the network. See Ndis802_11EncryptionXXXX in ntddndis.h
/// </param>
/// <param name="ppszSSIDDisplay">
/// Formatted SSID for display. String is allocated and must be freed by the
/// caller. Can be NULL if the UI string is not required.
/// </param>
/// <param name="cxpExtent">
/// Max width for the display string, in pixels. The SSID will be ellipsized
/// to allow formatted text to fit in the specified width.
/// </param>
/// <param name="fEscapeHtml">
/// If true <paramref name="pszBuffer"/> will have HTML characters escaped in
/// the resultant <paramref name="ppszSSIDDisplay"/> string.
/// </param>
extern "C"
HRESULT IsNetworkSecure(__in __opt LPCTSTR pszSSID, BOOL fUse8021x,
        DWORD dwAuthentication, DWORD dwEncryption,
        __out __opt LPTSTR *ppszSSIDDisplay, DWORD cxpExtent, BOOL fEscapeHtml)
{
    HRESULT hr = S_FALSE;
    BOOL fSecure;
    HDC hdcScreen = NULL;

    // if 802.1x or NOT open and unencrypted then it's secure
    fSecure = fUse8021x || !( (Ndis802_11AuthModeOpen == dwAuthentication) &&
        !CWZCManagerWireless::IsEncrypted(dwEncryption) );
    
    hr = fSecure ? S_OK : S_FALSE;

    if (NULL != ppszSSIDDisplay)
    {
        TCHAR szSSIDEllipsized[MAX_SSID_LEN + ARRAYSIZE(c_szEllipsis)];
        LPVOID rgpvArgs[] = {szSSIDEllipsized};
        int cchTry, cchSSID, cchFit;
        SIZE  size;

        CPREx(pszSSID, E_INVALIDARG);

        // caller wants the string

        *ppszSSIDDisplay = NULL;
        hdcScreen = GetDC(NULL);
        cchSSID = lstrlen(pszSSID);

        // work backwards from the full string less one char
        for (cchTry = cchSSID; cchTry >= 0; cchTry--)
        {
            hr = StringCchCopyN(szSSIDEllipsized,
                ARRAYSIZE(szSSIDEllipsized), pszSSID, cchTry);
            CHR(hr);
            if (cchTry < cchSSID)
            {
                // second and successive times, ellipsize
                hr = StringCchCat(szSSIDEllipsized,
                    ARRAYSIZE(szSSIDEllipsized), c_szEllipsis);
                VHR(hr);
            }
            LocalFree(*ppszSSIDDisplay);

            CWR(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                (LPTSTR)LoadString(HINST_RESDLL,
                fSecure ? IDS_WZC_NETSEL_FORMAT_SECURE_NETWORK :
                IDS_WZC_NETSEL_FORMAT_OPEN_NETWORK, NULL, 0), 0, 0,
                (LPTSTR)ppszSSIDDisplay, 0, (va_list *)rgpvArgs));
            VBR(GetTextExtentExPoint(hdcScreen, *ppszSSIDDisplay,
                lstrlen(*ppszSSIDDisplay), cxpExtent, &cchFit, NULL,
                &size));
            // need to continue lopping?
            if (size.cx <= (int)cxpExtent)
            {
                if (fEscapeHtml)
                {
                    // Create the escaped version of the SSID
                    hr = EscapeHtmlString(szSSIDEllipsized, ARRAYSIZE(szSSIDEllipsized));
                    CHR(hr);

                    CWR(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                        FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                        (LPTSTR)LoadString(HINST_RESDLL,
                        fSecure ? IDS_WZC_NETSEL_FORMAT_SECURE_NETWORK :
                        IDS_WZC_NETSEL_FORMAT_OPEN_NETWORK, NULL, 0), 0, 0,
                        (LPTSTR)ppszSSIDDisplay, 0, (va_list *)rgpvArgs));
                }
                break;
            }
            
        }
        ASSERT(cchTry >= 0);

    }

Error:
    if (FAILED(hr) && (NULL != ppszSSIDDisplay) && (NULL != *ppszSSIDDisplay))
    {
        LocalFree(*ppszSSIDDisplay);
        *ppszSSIDDisplay = NULL;
    }

    if (NULL != hdcScreen)
    {
        ReleaseDC(NULL, hdcScreen);
    }

    return hr;
}

/// <summary>
/// Pre-ellipsize a string to fit in a specified amount of space
/// (Eg. exchange.microsoft.com becomes ...ange.microsoft.com)
/// </summary>
/// <returns>
/// HRESULT
///  S_OK if string was successfully pre-ellipsized
///  S_FALSE if string did not need to be changed
///  Failure code otherwise
/// </returns>
/// <param name="pszInput">
/// String that may need to be ellipsized if it is too long
/// </param>
/// <param name="pszEllipsized">
/// Output buffer to hold pre-ellipsized string
/// </param>
/// <param name="cchEllipsized">
/// Size of output buffer <paramref name="pszEllipsized"/>
/// </param>
/// <param name="cxpExtent">
/// Max width for the display string, in pixels. The string will be pre-ellipsized
/// to allow formatted text to fit in the specified width.
/// </param>
extern "C"
HRESULT PreEllipsizeString(__in LPCTSTR pszInput, __out_ecount(cchEllipsized) LPTSTR pszEllipsized, UINT cchEllipsized, DWORD cxpExtent)
{
    BOOL fEllipsized = FALSE;
    HDC hdcScreen = NULL;
    LPTSTR pszReversedInput = NULL;
    UINT cchFit, cchInput, cchEllipsis, cchReversedInput;
    INT  cxpEllipsizedExtent;
    SIZE size;
    HRESULT hr = S_OK;

    hdcScreen = GetDC(NULL);

    hr = StringCchLength(pszInput, STRSAFE_MAX_CCH, &cchInput);
    CHR(hr);

    hr = StringCchLength(c_szEllipsis, STRSAFE_MAX_CCH, &cchEllipsis);
    CHR(hr);

    cchReversedInput = cchInput + cchEllipsis;
    pszReversedInput = (LPTSTR)LocalAlloc(LMEM_FIXED, cchReversedInput * sizeof(TCHAR));

    // Copy the input string to the temp reversed string buffer
    hr = StringCchCopy(pszReversedInput, cchReversedInput, pszInput);
    CHR(hr);
    
    // Reverse the string
    _tcsrev(pszReversedInput);

    // Get extent of ellipsis
    VBR(GetTextExtentPoint(hdcScreen, c_szEllipsis,
            cchEllipsis, &size));

    // Subtract space used up by ellipsis from width available for display string
    cxpEllipsizedExtent = (int)cxpExtent - size.cx;

    // Get number of chars of the reversed display string that fit in available space
    VBR(GetTextExtentExPoint(hdcScreen, pszReversedInput,
                cchInput, cxpEllipsizedExtent, (LPINT)&cchFit, NULL,
                &size));

    // Check if string needs to be truncated and ellipsized
    if (size.cx > (int)cxpExtent)
    {
        // Theres a problem in GetTextExtentExPoint() in that 
        // it tells us we can fit an extra character when we can't. 
        // Subtract one to be safe until this is fixed.
        cchFit--;
        
        // The original string wouldn't have fit in the space available
        // Truncate it at cchFit and concatenate an ellipsis
        pszReversedInput[cchFit] = 0;
        hr = StringCchCat(pszReversedInput, cchReversedInput, c_szEllipsis);
        CHR(hr); 

        // Final number of characters that fit in the available space includes
        // the characters from the truncated string as well as the ellipsis
        cchFit = cchFit + cchEllipsis;
        CBR(cchFit <= cchEllipsized);

        // Reverse the temp buffer again and copy it to the output buffer
        _tcsrev(pszReversedInput);
        hr = StringCchCopy(pszEllipsized, cchEllipsized, pszReversedInput);
        CHR(hr);

    }
    else
    {
        // The original string fits in the available space - no change was needed
        hr = S_FALSE;   
    }

Error:
    if (pszReversedInput)
    {
        LocalFree(pszReversedInput);
    }

    if (NULL != hdcScreen)
    {
        ReleaseDC(NULL, hdcScreen);
    }
    return hr;
}


/// <summary>
/// Replace HTML characters with escape sequences. 
/// (ex. "<" becomes "&lt;")
/// </summary>
/// <returns>
///   HRESULT
///     S_OK for success
/// </returns>
/// <param name="pszBuffer">
/// Buffer to be escaped (null terminated).
/// </param>
/// <param name="cchBuffer">
/// Size of <paramref name="pszBuffer"/> (in characters).
/// </param>
HRESULT EscapeHtmlString(LPTSTR pszBuffer, UINT cchBuffer)
{
    HRESULT hr = S_OK;
    INT     cchEscape = 0;
    TCHAR   szEscape[MAX_PATH];

    cchEscape = SHEscapeBubbleHtml(pszBuffer, szEscape, ARRAYSIZE(szEscape));
    CBR(cchEscape < ARRAYSIZE(szEscape));

    hr = StringCchCopy(pszBuffer, cchBuffer, szEscape);
    CHR(hr);

Error:
    return hr;
}



extern "C"
BOOL  IsSecure( PWLC  pwlc )
{
   BOOL fUse8021x;   
   BOOL fSecure;    
   
   fUse8021x = pwlc[0].dwCtlFlags & WZCCTL_ONEX_ENABLED;   
   // if 802.1x or NOT open and unencrypted then it's secure
   fSecure = fUse8021x || !( (Ndis802_11AuthModeOpen == pwlc[0].AuthenticationMode) &&
       !CWZCManagerWireless::IsEncrypted(pwlc[0].Privacy) );
      
   return fSecure;
}

extern "C"
UINT  GetMetaNetwork_InternetIndex(void)
{

    return  MatchMetaNetworkIndex(IID_DestNetInternet);

}