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
// Filename:    wzcintf.cpp
//
// Purpose:     Wireless hardware interface code
//
//==========================================================================*

#include <windows.h>
#include <wpcpriv.h>
#include <wzcshell.h>
#include <ehm.h>

#include "netuip.h"
#include "wzcuip.h"
#include "wzcintf.h"
#include "wzcmgr.h"
#include "connmgr_priv.h"


CNetID::CNetID()
{
    // Initialize the object so that it will never be equal to any other
    Reset();
}

HRESULT CNetID::GetDisplayName(LPTSTR pszBuffer, UINT cchBuffer) const
{
    HRESULT hr = S_OK;
    UINT    cch;

    CBRAEx(NULL != pszBuffer && 0 != cchBuffer, E_INVALIDARG);

    // If you hit this assertion, you sent this function a network ID that has
    // not been assigned!! This is VERY bad -- the code should never do this!!
    CBRA((ULONG)-1 != Ssid.SsidLength);

    // Convert the SSID to Unicode
    cch = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)Ssid.Ssid, Ssid.SsidLength, pszBuffer, cchBuffer);
    CBREx(0 != cch, ERROR_INSUFFICIENT_BUFFER == GetLastError() ? SHWZC_E_INSUFFICIENT_BUFFER : E_FAIL);
    CBREx(cch < cchBuffer, SHWZC_E_INSUFFICIENT_BUFFER);

    // Safe: we just verified that we've got space in the error check above
    pszBuffer[cch] = TEXT('\0');

Error:
    return hr;
}

HRESULT CNetID::GetRegistryName(LPTSTR pszBuffer, UINT cchBuffer) const
{
    HRESULT     hr = S_OK;
    const UCHAR *pchCurrent, *pchEnd;
    size_t      cchOutput = cchBuffer;

    CBRAEx(NULL != pszBuffer && 0 != cchBuffer, E_INVALIDARG);
    *pszBuffer = TEXT('\0');

    // If you hit this assertion, you sent this function a network ID that has
    // not been assigned!! This is VERY bad -- the code should never do this!!
    CBRA((ULONG)-1 != Ssid.SsidLength);

    // In order to store information about SSIDs in the registry, we need to
    // ensure that we use a very limited set of characters. Specifically, we
    // need to be sure that locale conversion issues from ANSI don't result in
    // illegal registry characters. Therefore, we encode SSIDs using a simple
    // scheme: we compare each character to a set of acceptable values, and if
    // the character doesn't match, we just encode it using "%xx" notation.
    for (pchCurrent = Ssid.Ssid, pchEnd = pchCurrent + Ssid.SsidLength; pchCurrent != pchEnd; ++pchCurrent)
    {
        static const CHAR c_szAccept[] = "-_~!@$^&*+.,{}()[]<>#:; ?";
        if ((*pchCurrent >= '0' && *pchCurrent <= '9') ||
            (*pchCurrent >= 'A' && *pchCurrent <= 'Z') ||
            (*pchCurrent >= 'a' && *pchCurrent <= 'z') ||
            NULL != strchr(c_szAccept, *pchCurrent))
        {
            CBREx(1 < cchOutput, SHWZC_E_INSUFFICIENT_BUFFER);
            *pszBuffer++ = *pchCurrent;
            *pszBuffer = TEXT('\0');
            --cchOutput;
        }
        else
        {
            hr = StringCchPrintfEx(pszBuffer, cchOutput, &pszBuffer, &cchOutput, 0,
                TEXT("%%%02x"), *pchCurrent);
            CHR(hr);
        }
    }

Error:
    // Under every circumstance, the output buffer should be null terminated
    return hr;
}

void CNetID::Assign(PCWLC pwlc)
{
    memcpy(&Ssid, &pwlc->Ssid, sizeof(SSID));
    InfrastructureMode = pwlc->InfrastructureMode;
}

HRESULT CNetID::Assign(LPCTSTR pszName, BOOL fAdhoc)
{
    HRESULT hr = S_OK;

    // Convert the name into an SSID
    hr = SHWZCConvertName(pszName, &Ssid);
    CHR(hr);

    // Set the infrastructure mode
    InfrastructureMode = (fAdhoc ? Ndis802_11IBSS : Ndis802_11Infrastructure);

Error:
    return hr;
}

HRESULT CNetID::Associate(UINT iMetaNetwork) const
{
    HRESULT hr = S_OK;
    LPCTSTR pszMetaNetworkGUID;
    TCHAR   szBuffer[MAX_PATH];

    // Get the metanetwork GUID
    pszMetaNetworkGUID = GetMetaNetworkGUIDString(iMetaNetwork);
    CBR(NULL != pszMetaNetworkGUID);

    // Get the registry name for the SSID
    hr = GetRegistryName(szBuffer, ARRAYSIZE(szBuffer));
    CHRA(hr);

    // Create the connection manager entry
    hr = SetNetworkDestGUID(szBuffer, IsAdhoc(), pszMetaNetworkGUID);
    CHR(hr);

Error:
    return hr;
}

int CNetID::Compare(const CNetID *pnid, BOOL fMatchSSIDOnly) const
{
    ASSERT(NULL != pnid);
    PF_ASSUME_NOTNULL(pnid);
    return Compare(&pnid->Ssid, pnid->InfrastructureMode, fMatchSSIDOnly);
}

int CNetID::Compare(PCWLC pwlc, BOOL fMatchSSIDOnly) const
{
    ASSERT(NULL != pwlc);
    PF_ASSUME_NOTNULL(pwlc);
    return Compare(&pwlc->Ssid, pwlc->InfrastructureMode, fMatchSSIDOnly);
}

int CNetID::Compare(PCSSID pssid, UINT uMode, BOOL fMatchSSIDOnly) const
{
    static const int rgnResult[2] = { -1, 1 };
    int nMatch;

    ASSERT(NULL != pssid);
    PF_ASSUME_NOTNULL(pssid);

    // Check for invalid objects
    if (Ssid.SsidLength > ARRAYSIZE(Ssid.Ssid))
    {
        return 1;
    }

    if (pssid->SsidLength > ARRAYSIZE(pssid->Ssid))
    {
        return -1;
    }

    // Compare the strings
    nMatch = memcmp(Ssid.Ssid, pssid->Ssid,
        Ssid.SsidLength < pssid->SsidLength ? Ssid.SsidLength : pssid->SsidLength);

    // If they match up to the shorter of the two
    if (!nMatch)
    {
        if (Ssid.SsidLength != pssid->SsidLength)
        {
            // If the lengths aren't the same, the longer string is greater
            nMatch = rgnResult[Ssid.SsidLength > pssid->SsidLength];
        }
        else if (!fMatchSSIDOnly &&
            InfrastructureMode != Ndis802_11AutoUnknown &&
            uMode != Ndis802_11AutoUnknown &&
            InfrastructureMode != uMode)
        {
            // If they still match, check the infrastructure mode
            nMatch = rgnResult[InfrastructureMode > uMode];
        }
    }

    return nMatch;
}

HRESULT CNetID::GetAssociation(LPTSTR pszGUID, UINT cchGUID) const
{
    HRESULT hr = S_OK;
    TCHAR   szBuffer[MAX_PATH];

    // Get the registry name for the SSID
    hr = GetRegistryName(szBuffer, ARRAYSIZE(szBuffer));
    CHRA(hr);

    // Get the connection manager entry
    hr = GetNetworkDestGUID(szBuffer, IsAdhoc(), pszGUID, cchGUID);
    CHR(hr);

Error:
    return hr;
}

UINT CNetID::GetAssociation() const
{
    HRESULT hr = S_OK;
    TCHAR   szGUID[64];
    UINT    iMetaNetwork = 0;

    // Get the associated GUID
    hr = GetAssociation(szGUID, ARRAYSIZE(szGUID));
    CHR(hr);

    // Get the GUID index
    hr = GetMetaNetworkIndex(szGUID, &iMetaNetwork);
    CHR(hr);

Error:
    return (SUCCEEDED(hr) ? iMetaNetwork : 0);
}

HRESULT CNetID::ClearAssociation() const
{
    HRESULT hr = S_OK;
    TCHAR   szBuffer[MAX_PATH];

    // Get the registry name for the SSID
    hr = GetRegistryName(szBuffer, ARRAYSIZE(szBuffer));
    CHRA(hr);

    // Clear the connection manager association
    hr = ClearNetworkDestGUID(szBuffer, IsAdhoc());
    CHR(hr);

Error:
    return hr;
}

void CNetID::Reset()
{
    // Set the length of the SSID string to an impossible value... this is
    // explicitly checked for in the comparison functions and is never equal to
    // any other netID (even another reset one)
    Ssid.SsidLength = (ULONG)-1;
}

CWZCInterface::CWZCInterface(LPCTSTR pszDeviceName) : m_pcfgl(NULL), m_dwValidFlags(0), m_dwRecentFlags(0), m_fPower(TRUE)
{
    ZeroMemory(&m_sIntfEntry, sizeof(INTF_ENTRY_EX));
    m_sIntfEntry.wszGuid = const_cast<LPWSTR>(pszDeviceName);
    m_rgcHidden[1] = m_rgcHidden[0] = 0;
}

void CWZCInterface::Clear()
{
    if (NULL != m_pcfgl)
    {
        // Free the broadcast network list buffer
        LocalFree(m_pcfgl);
        m_pcfgl = NULL;
    }

    if (m_sIntfEntry.bInitialized)
    {
        // Clear the interface data
        LPWSTR wszGuid = m_sIntfEntry.wszGuid;
        WZCDeleteIntfObjEx(&m_sIntfEntry);
        ZeroMemory(&m_sIntfEntry, sizeof(INTF_ENTRY_EX));
        m_sIntfEntry.wszGuid = wszGuid;
    }

    // Reset the state
    m_dwValidFlags = 0;
    m_dwRecentFlags = 0;
    m_rgcHidden[1] = m_rgcHidden[0] = 0;
}

HRESULT CWZCInterface::Initialize(LPCTSTR pszDeviceName)
{
    HRESULT hr = S_OK;

    // Assign the adapter name
    m_sIntfEntry.wszGuid = (LPWSTR)pszDeviceName;

    // Query the WZC info for the adapter
    hr = Query(INTF_ALL);
    CHR(hr);

    // If the interface isn't enabled, we want to fail gracefully
    CBREx(INTFCTL_ENABLED & m_sIntfEntry.dwCtlFlags, E_NOTIMPL);

    // Check for zero config support
    if (!(m_sIntfEntry.dwCtlFlags & INTFCTL_OIDSSUPP))
    {
        RETAILMSG(1, (TEXT("Wi-Fi: '%s' driver does not support zero config"), m_sIntfEntry.wszGuid));
        CBREx(FALSE, E_NOTIMPL);
    }

Error:
    return hr;
}

HRESULT CWZCInterface::Query(DWORD dwFlags) const
{
    HRESULT hr = S_OK;

    // If we already queried for these values, we're done
    CBREx((m_dwRecentFlags & dwFlags) != dwFlags, S_FALSE);

    // Query the data
    hr = const_cast<CWZCInterface*>(this)->QueryDirect(dwFlags | m_dwValidFlags);
    CHR(hr);

Error:
    return hr;
}

HRESULT CWZCInterface::QueryDirect(DWORD dwFlags)
{
    HRESULT hr = S_OK;

    // Free the current data
    Clear();
    CBREx(0 != dwFlags, S_FALSE);

    // Query the new data
    hr = WZCQueryWirelessInfo(&m_sIntfEntry, dwFlags, &m_dwValidFlags);
    CHR(hr);

    // Copy the broadcast list so that we can modify it
    if (0 != (INTF_BSSIDLIST & m_dwValidFlags) && NULL != m_sIntfEntry.rdBSSIDList.pData)
    {
        m_pcfgl = (PCFGLIST)LocalAlloc(LMEM_FIXED, m_sIntfEntry.rdBSSIDList.dwDataLen);
        CPR(m_pcfgl);
        memcpy(m_pcfgl, m_sIntfEntry.rdBSSIDList.pData, m_sIntfEntry.rdBSSIDList.dwDataLen);
    }

    // Clean the data
    Sanitize();

    // Save the query flags
    m_dwRecentFlags = dwFlags | m_dwValidFlags;

Error:
    return hr;
}

HRESULT CWZCInterface::Apply(DWORD dwFlags)
{
    HRESULT hr = S_OK;
    DWORD   dwResult;

    ASSERT((dwFlags & m_dwValidFlags) == dwFlags);
    dwResult = WZCSetInterfaceEx(NULL, dwFlags & m_dwValidFlags, &m_sIntfEntry, NULL);
    CBR(ERROR_SUCCESS == dwResult);

Error:
    return hr;
}

typedef BOOL (*PFNCHECKWLC)(__in PCWLC pwlc, __in __opt LPVOID pvParam);
static void CleanCfgList(__in PWLC pwlcBegin, __inout PCWLC *ppwlcEnd,
                         __in PFNCHECKWLC pfnCondition, __in __opt LPVOID pvParam)
{
    PWLC  pwlc = pwlcBegin;
    PCWLC pwlcEnd = *ppwlcEnd;

    ASSERT(NULL != ppwlcEnd);

    while (pwlc != pwlcEnd)
    {
        if (pfnCondition(pwlc, pvParam))
        {
            WZCFreeConfig(pwlc);
            if (--pwlcEnd != pwlc)
            {
                WZCCopyConfigs(pwlc, pwlc + 1, pwlcEnd - pwlc, FALSE);
            }
        }
        else
        {
            ++pwlc;
        }
    }

    *ppwlcEnd = pwlcEnd;
}

static BOOL CheckSSID(__in PCWLC pwlc, __in __opt LPVOID pvParam)
{
    UNREFERENCED_PARAMETER(pvParam);

    // Check for invalid characters
    const UCHAR *pucSSID = pwlc->Ssid.Ssid;
    const UCHAR *pucSSIDEnd = pucSSID + pwlc->Ssid.SsidLength;

    while (pucSSID != pucSSIDEnd && 0 != *pucSSID)
    {
        ++pucSSID;
    }

    return (pucSSID != pucSSIDEnd || 0 == pwlc->Ssid.SsidLength);
}

void CWZCInterface::Sanitize()
{
    // Not sure why we always have to do this... seems like this should be controlled
    if (m_sIntfEntry.nAuthMode < 0) m_sIntfEntry.nAuthMode = 0;
    if (m_sIntfEntry.nInfraMode < 0) m_sIntfEntry.nInfraMode = 0;

    // Check if the broadcast SSID list exists
    if (NULL != m_pcfgl)
    {
        PWLC  pwlcBegin = m_pcfgl->Config;
        PCWLC pwlcEnd = pwlcBegin + m_pcfgl->NumberOfItems;

        // When the device is powered off, for some reason the driver continues
        // to report the list of broadcast networks as they were before the
        // device was powered down. We adjust those values here.
        if (!GetPowerState())
        {
            for (PWLC pwlc = pwlcBegin; pwlc != pwlcEnd; ++pwlc)
            {
                // Technically not necessary, since these are broadcast
                // networks and they don't have any extra user data
                WZCFreeConfig(pwlc);
            }

            // No broadcast networks
            pwlcEnd = pwlcBegin;
        }

        // We remove empty SSIDs because these are access points that have been
        // configured to not broadcast their name, and we don't want to display
        // them as airing in our UI. We also remove SSIDs with invalid
        // characters; the only invalid character is '\0'. Technically, even
        // this character isn't invalid according to the 802.11 spec, but in
        // reality this would be an extreme edge case, and our code gets a lot
        // cleaner if we rule it out. If we want to support it in the future,
        // we need to figure out how to encode it for storage in the registry,
        // XML, and passing between functions.
        CleanCfgList(pwlcBegin, &pwlcEnd, CheckSSID, NULL);

        if (pwlcBegin + 1 < pwlcEnd)
        {
            PWLC   pwlc1, pwlc2;
            CNetID nid1;

            // Remove duplicate entries. This is where we decide which access point to
            // use if there are multiple choices...
            for (pwlc1 = pwlcBegin + 1; pwlc1 != pwlcEnd; ++pwlc1)
            {
                nid1.Assign(pwlc1);

                for (pwlc2 = pwlcBegin; pwlc2 != pwlc1; ++pwlc2)
                {
                    if (!nid1.Compare(pwlc2, FALSE))
                    {
                        // Decide which one to use based on their quality
                        UINT uValue1 = GetNetworkQuality(pwlc1);
                        UINT uValue2 = GetNetworkQuality(pwlc2);
                        PWLC pwlcDelete = (uValue2 < uValue1 ? pwlc2 : pwlc1);

                        WZCFreeConfig(pwlcDelete);
                        if (--pwlcEnd != pwlcDelete)
                        {
                            WZCCopyConfigs(pwlcDelete, pwlcDelete + 1, pwlcEnd - pwlcDelete, FALSE);
                        }

                        // We removed a network, and we need to decrement pwlc1
                        // so that when it gets incremented in the for loop it
                        // will point at the right one
                        --pwlc1;
                        break;
                    }
                }
            }
        }

        // The number of entries may have changed
        m_pcfgl->NumberOfItems = pwlcEnd - pwlcBegin;
    }

    // If the device is powered off, it's also not connected :)
    if (!GetPowerState())
    {
        m_dwValidFlags &= ~INTF_SSID;
    }
}

UINT CWZCInterface::GetInfraMode() const
{
    HRESULT hr = S_OK;

    // Get the current mode
    hr = Query(INTF_INFRAMODE);
    CHR(hr);

Error:
    return SUCCEEDED(hr) ? m_sIntfEntry.nInfraMode : Ndis802_11AutoUnknown;
}

HRESULT CWZCInterface::GetFilter(DWORD *pdwFlags)
{
    HRESULT hr = S_OK;

    // Check inputs
    CBRAEx(NULL != pdwFlags, E_INVALIDARG);

    // Query the state if necessary
    hr = Query(INTF_CM_MASK | INTF_FALLBACK);
    CHR(hr);

    // Convert to the flags format
    *pdwFlags = (m_sIntfEntry.dwCtlFlags & INTFCTL_CM_MASK);
    if (INTFCTL_FALLBACK & m_sIntfEntry.dwCtlFlags)
    {
        *pdwFlags |= SHWZCF_CONNECT_ANY;
    }

Error:
    return hr;
}

HRESULT CWZCInterface::SetFilter(DWORD dwFlags, BOOL fApply)
{
    HRESULT hr = S_OK;
    int     nInfraMode = dwFlags & SHWZCF_ALLOW_MASK;
    DWORD   dwApplyFlags = INTF_CM_MASK | INTF_FALLBACK;

    // If we're connected, we may need to disconnect
    if (Ndis802_11AutoUnknown != nInfraMode &&
        NULL != GetConnected() &&
        m_sIntfEntry.nInfraMode != nInfraMode)
    {
        // We need to force a reconnect
        dwApplyFlags |= INTF_PREFLIST;
    }

    // Update the interface
    m_sIntfEntry.dwCtlFlags &= ~(INTFCTL_CM_MASK | INTFCTL_FALLBACK);
    m_sIntfEntry.dwCtlFlags |= nInfraMode | ((SHWZCF_CONNECT_ANY & dwFlags) ? INTFCTL_FALLBACK : 0);
    m_dwValidFlags |= INTF_CM_MASK | INTF_FALLBACK;

    if (fApply)
    {
        hr = Apply(dwApplyFlags);
        CHR(hr);
    }

Error:
    return hr;
}

PCWLC CWZCInterface::Find(const CNetID *pnid, DWORD *pdwNetFlags) const
{
    DWORD dwInFlags = pdwNetFlags ? *pdwNetFlags : (DWORD)-1;
    DWORD dwOutFlags = 0;
    PCWLC pwlcResult = NULL;

    while (dwInFlags & (SHWZCF_NET_PREFERRED | SHWZCF_NET_BROADCAST))
    {
        PCWLC pwlc, pwlcEnd;
        UINT  cConfigs;

        // Which group should we search?
        if (dwInFlags & SHWZCF_NET_PREFERRED)
        {
            dwOutFlags = SHWZCF_NET_PREFERRED | (dwInFlags & SHWZCF_NET_HIDDEN);
            dwInFlags &= ~SHWZCF_NET_PREFERRED;
        }
        else
        {
            dwOutFlags = SHWZCF_NET_BROADCAST | (dwInFlags & SHWZCF_NET_HIDDEN);
            dwInFlags &= ~SHWZCF_NET_BROADCAST;
        }

        cConfigs = GetConfigs(dwOutFlags, &pwlc);
        for (pwlcEnd = pwlc + cConfigs; pwlc != pwlcEnd; ++pwlc)
        {
            if (!pnid->Compare(pwlc, FALSE))
            {
                // If the caller allowed hidden networks, find out if it really is
                if (dwOutFlags & SHWZCF_NET_HIDDEN)
                {
                    PCWLC pwlcVisible;

                    if (GetConfigs(dwOutFlags & ~SHWZCF_NET_HIDDEN, &pwlcVisible) &&
                        pwlc >= pwlcVisible)
                    {
                        dwOutFlags &= ~SHWZCF_NET_HIDDEN;
                    }
                }

                // We're done
                pwlcResult = pwlc;
                goto Exit;
            }
        }
    }

Exit:
    if (pdwNetFlags)
    {
        *pdwNetFlags = dwOutFlags;
    }

    return pwlcResult;
}

UINT CWZCInterface::GetConfigs(DWORD dwNetFlags, PCWLC *ppwlc) const
{
    UINT      cConfigs = 0;
    PCWLC     pwlcResult = NULL;
    BOOL      fPreferred = BOOLIFY(dwNetFlags & SHWZCF_NET_PREFERRED);
    DWORD     dwValidMask = (fPreferred ? INTF_PREFLIST : INTF_BSSIDLIST);
    PCCFGLIST pcfgl = fPreferred ? (PCCFGLIST)m_sIntfEntry.rdStSSIDList.pData : m_pcfgl;

    // Check if the SSID list exists
    if (0 != (m_dwValidFlags & dwValidMask) && NULL != pcfgl)
    {
        // How many configs should we skip?
        DWORD cSkip = (dwNetFlags & SHWZCF_NET_HIDDEN) ? 0 : m_rgcHidden[fPreferred];
        cConfigs = pcfgl->NumberOfItems - cSkip;
        pwlcResult = pcfgl->Config + cSkip;
    }

    if (ppwlc)
    {
        *ppwlc = pwlcResult;
    }

    return cConfigs;
}

HRESULT CWZCInterface::GetConnected(__out CNetID *pnid) const
{
    HRESULT hr = S_OK;

    hr = Query(INTF_SSID | INTF_INFRAMODE);
    CHR(hr);

    // Copy the connected network into a network ID
    CBRA(sizeof(pnid->Ssid.Ssid) >= m_sIntfEntry.rdSSID.dwDataLen);
    memcpy(&pnid->Ssid.Ssid, m_sIntfEntry.rdSSID.pData, m_sIntfEntry.rdSSID.dwDataLen);
    pnid->Ssid.SsidLength = m_sIntfEntry.rdSSID.dwDataLen;
    pnid->InfrastructureMode = m_sIntfEntry.nInfraMode;

    // Reset the network ID if we're not connected
    if (0 == (INTF_SSID & m_dwValidFlags) ||
        0 == pnid->Ssid.SsidLength)
    {
        pnid->Reset();
        hr = S_FALSE;
    }

Error:
    return hr;
}

PCWLC CWZCInterface::GetConnected() const
{
    HRESULT hr = S_OK;
    CNetID  nid;
    DWORD   dwNetFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_BROADCAST | SHWZCF_NET_HIDDEN;
    PCWLC   pwlc = NULL;

    // We need the preferred list and the broadcast list, since a connected
    // network can potentially be in one or the other, or both. If it's in
    // both, we'll return the configuration form the preferred list.
    hr = Query(INTF_SSID | INTF_INFRAMODE | INTF_PREFLIST | INTF_BSSIDLIST);
    CHR(hr);

    // Get the connected network ID
    hr = GetConnected(&nid);
    CHR(hr);

    // Find the network in the available list(s)
    pwlc = Find(&nid, &dwNetFlags);

Error:
    return pwlc;
}

// Get the signal strength for a given RSSI value. The returned value is from 0
// to 5, representing 6 states (no signal, very low, low, good, very good,
// excellent).
UINT CWZCInterface::GetSignalStrength(int nRSSI, DEVICETYPE dtType)
{
    UINT uSignalStrength = 0;

    if (DEVICE_WIRELESS == dtType)
    {
        // The RSSI is non-linear, and we use an ancient codex to decipher its mysteries
        static const LONG lRSSIThresholds[] = { -90, -81, -71, -67, -57 };
        for (uSignalStrength = 0; uSignalStrength != ARRAYSIZE(lRSSIThresholds); ++uSignalStrength)
        {
            if (nRSSI < lRSSIThresholds[uSignalStrength])
            {
                break;
            }
        }
    }
    else if (DEVICE_WIFI_NATIVE == dtType)
    {
        // The RSSI is linear from STA_MIN_QUALITY to STA_MAX_QUALITY
        uSignalStrength = ((UINT)nRSSI - STA_MIN_QUALITY) * 5 / (STA_MAX_QUALITY - STA_MIN_QUALITY);
    }

    return uSignalStrength;
}

// Come up with an arbitrary scale number (used only in comparison) to describe
// the relative quality of a given configuration. We use this number to
// determine which access point to connect when there are multiple choices all
// broadcasting the same SSID.
UINT CWZCInterface::GetNetworkQuality(PCWLC pwlc)
{
    UINT uSignalStrength;
    UINT uMaxRate;

    // Convert RSSI data to int from 0 to 5
    uSignalStrength = GetSignalStrength(pwlc->Rssi, DEVICE_WIRELESS);

    // Determine the maximum transfer rate of the network
    for (uMaxRate = ARRAYSIZE(pwlc->SupportedRates); uMaxRate; --uMaxRate)
    {
        if (pwlc->SupportedRates[uMaxRate-1])
        {
            break;
        }
    }

    // For now, we'll try this with some combination of the supported transfer
    // rates and the signal strength... may need to be tweaked later
    return uSignalStrength * uMaxRate;
}

// Move the specified network index from the _visible_ broadcast list to the
// invisible broadcast list
void CWZCInterface::Hide(UINT iConfig, BOOL fPreferred)
{
    // Copy the config from its current location
    if (iConfig)
    {
        WLC  wlcSwap;
        PWLC pwlcVisible;
        UINT cConfigs;

        // If you hit this assertion, you called this function on an SSID index
        // that you don't even know exists! That's a no-no...
        cConfigs = GetConfigs(fPreferred ? SHWZCF_NET_PREFERRED : SHWZCF_NET_BROADCAST, (PCWLC*)&pwlcVisible);
        ASSERT(iConfig < cConfigs);

        WZCCopyConfigs(&wlcSwap, pwlcVisible + iConfig, 1, FALSE);
        WZCCopyConfigs(pwlcVisible + 1, pwlcVisible, iConfig, FALSE);
        WZCCopyConfigs(pwlcVisible, &wlcSwap, 1, FALSE);
    }

    // Increment the number of hidden networks
    ++m_rgcHidden[fPreferred];
}

HRESULT CWZCInterface::Refresh(DWORD dwFlags)
{
    HRESULT hr = S_OK;
    DWORD   dwResult;

    // Update the data
    Clear();

    dwResult = WZCRefreshInterfaceEx(NULL, dwFlags, &m_sIntfEntry, &m_dwValidFlags);
    CBR(ERROR_SUCCESS == dwResult);

Error:
    return hr;
}

HRESULT CWZCInterface::SetMostPreferred(PCWLC pwlc)
{
    HRESULT hr = S_OK;
    WLC     wlcNew;
    BOOL    fFreeConfig = FALSE;
    UINT    cConfigs;
    PWLC    pwlcPref, pwlcPrefBegin;
    PCWLC   pwlcPrefEnd;
    PWLC    pwlcDest;
    CNetID  nid;

    // Create a full copy of the config
    hr = WZCCopyConfigs(&wlcNew, pwlc, 1, TRUE);
    CHR(hr);
    fFreeConfig = TRUE;

    // Try to find the configuration in the current preferred list
    cConfigs = GetConfigs(SHWZCF_NET_PREFERRED | SHWZCF_NET_HIDDEN, (PCWLC*)&pwlcPrefBegin);
    pwlcPrefEnd = pwlcPrefBegin + cConfigs;
    nid.Assign(pwlc);

    for (pwlcPref = pwlcPrefBegin; pwlcPref != pwlcPrefEnd; ++pwlcPref)
    {
        if (!nid.Compare(pwlcPref, FALSE))
        {
            break;
        }
    }

    // Determine where to insert the network
    if (Ndis802_11IBSS == wlcNew.InfrastructureMode)
    {
        // Ad-hoc networks must always be placed AFTER all of the AP networks,
        // since we connect to them no matter what -- even if no other devices
        // have associated with the network. Putting them behind the AP
        // networks means we'll try the AP networks before doing that.
        for (pwlcDest = pwlcPrefBegin; pwlcDest != pwlcPrefEnd; ++pwlcDest)
        {
            if (Ndis802_11IBSS == pwlcDest->InfrastructureMode)
            {
                break;
            }
        }
    }
    else
    {
        // AP networks always go to the front of the list
        pwlcDest = pwlcPrefBegin;
    }

    // This should never happen -- somehow the network is being moved BACK in the list??
    CBRA(pwlcDest <= pwlcPref);

    if (pwlcPref != pwlcPrefEnd)
    {
        WLC wlcOld;

        // Move the configuration
        WZCCopyConfigs(&wlcOld, pwlcPref, 1, FALSE);
        WZCCopyConfigs(pwlcDest + 1, pwlcDest, pwlcPref - pwlcDest, FALSE);
        WZCCopyConfigs(pwlcDest, &wlcNew, 1, FALSE);
        fFreeConfig = FALSE;

        // Free the old configuration
        WZCFreeConfig(&wlcOld);
    }
    else
    {
        // Insert the configuration
        hr = WZCInsertPreferredConfig(&m_sIntfEntry, m_dwValidFlags,
            pwlcDest - pwlcPrefBegin, &wlcNew);
        CHR(hr);

        // We now definitely have a valid preferred list
        m_dwValidFlags |= INTF_PREFLIST;
        fFreeConfig = FALSE;
    }

Error:
    if (fFreeConfig)
    {
        WZCFreeConfig(&wlcNew);
    }

    return hr;
}

HRESULT CWZCInterface::RemovePreferred(const CNetID *pnid)
{
    HRESULT  hr = S_OK;
    PCFGLIST pcfgl = (PCFGLIST)m_sIntfEntry.rdStSSIDList.pData;
    PWLC     pwlc;
    PCWLC    pwlcEnd;

    // Sanity check
    ASSERT(NULL != pnid);
    CBREx(0 != (m_dwValidFlags & INTF_PREFLIST) && NULL != pcfgl, S_FALSE);

    // Find the item
    for (pwlc = pcfgl->Config, pwlcEnd = pcfgl->Config + pcfgl->NumberOfItems; pwlc != pwlcEnd; ++pwlc)
    {
        if (!pnid->Compare(pwlc, FALSE))
        {
            break;
        }
    }

    CBREx(pwlc != pwlcEnd, S_FALSE);

    // Remove the item
    WZCFreeConfig(pwlc);
    if (--pwlcEnd != pwlc)
    {
        WZCCopyConfigs(pwlc, pwlc + 1, pwlcEnd - pwlc, FALSE);
    }

    // Update the item count
    pcfgl->NumberOfItems -= 1;

Error:
    return hr;
}

HRESULT CWZCInterface::SetPreferredCount(UINT cConfigs)
{
    HRESULT  hr = S_OK;
    PCFGLIST pcfgl = (PCFGLIST)m_sIntfEntry.rdStSSIDList.pData;
    UINT     i;

    // Sanity check
    CBREx(0 != (m_dwValidFlags & INTF_PREFLIST) && NULL != pcfgl, S_FALSE);
    CBREx(pcfgl->NumberOfItems > cConfigs, S_FALSE);

    // Free all extra items
    for (i = cConfigs; i != pcfgl->NumberOfItems; ++i)
    {
        WZCFreeConfig(&pcfgl->Config[i]);
    }

    // Set the count
    pcfgl->NumberOfItems = cConfigs;

Error:
    return hr;
}

HRESULT CWZCInterface::RemoveAllPreferred()
{
    HRESULT  hr = S_OK;
    PCFGLIST pcfgl = (PCFGLIST)m_sIntfEntry.rdStSSIDList.pData;
    PWLC     pwlc;
    PCWLC    pwlcEnd;

    // Sanity check
    CBREx(0 != (m_dwValidFlags & INTF_PREFLIST) && NULL != pcfgl, S_FALSE);
    CBREx(0 != pcfgl->NumberOfItems, S_FALSE);

    // Free all the items
    for (pwlc = pcfgl->Config, pwlcEnd = pcfgl->Config + pcfgl->NumberOfItems; pwlc != pwlcEnd; ++pwlc)
    {
        WZCFreeConfig(pwlc);
    }

    // Update the item count
    pcfgl->NumberOfItems = 0;

Error:
    return hr;
}

int FindNetwork(const CNetID *pnid, PCSHWZCNET pwzcnet, UINT cConfigs, UINT cbItem)
{
    PCSHWZCNET pwzcnetSearch = pwzcnet;
    PCSHWZCNET pwzcnetEnd = (PCSHWZCNET)((LPBYTE)pwzcnet + cbItem * cConfigs);
    CNetID     nid2;

    for (; pwzcnetSearch != pwzcnetEnd; pwzcnetSearch = (PCSHWZCNET)((LPBYTE)pwzcnetSearch + cbItem))
    {
        if (SUCCEEDED(nid2.Assign(pwzcnetSearch)) &&
            0 == nid2.Compare(pnid, FALSE))
        {
            return (int)(((LPBYTE)pwzcnetSearch - (LPBYTE)pwzcnet) / cbItem);
        }
    }

    // Not found
    return -1;
}

extern "C"
INT WZCMatchConfig(PCWLC pCfg1, PCWLC pCfg2, BOOL fMatchSSIDOnly)
{
    CNetID nid;

    nid.Assign(pCfg1);
    return nid.Compare(pCfg2, fMatchSSIDOnly);
}
