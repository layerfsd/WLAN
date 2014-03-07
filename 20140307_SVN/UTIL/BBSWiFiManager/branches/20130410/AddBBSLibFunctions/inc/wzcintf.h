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
// Filename:    wzcintf.h
//
// Purpose:     Wireless hardware interface declarations
//
//==========================================================================*
#pragma once

#include "cmnutils.h"


// network flags
#define SHWZCF_NET_HIDDEN   0x80000000  // dismissed or unavailable

// This class needs to encapsulate everything that makes a network unique
class CNetID
{
public:
    SSID Ssid;
    UINT InfrastructureMode;

public:
    void Assign(PCWLC pwlc);
    HRESULT Assign(LPCTSTR pszName, BOOL fAdhoc);
    HRESULT Assign(PCSHWZCNET pwzcnet) { return Assign(pwzcnet->szName, BOOLIFY(pwzcnet->dwFlags & SHWZCF_NET_ADHOC)); }
    void Reset();
    BOOL IsValid() const { return (Ssid.SsidLength != (ULONG)-1); }

    int Compare(__notnull const CNetID *pnid, BOOL fMatchSSIDOnly) const;
    int Compare(__notnull PCWLC pwlc, BOOL fMatchSSIDOnly) const;
    BOOL IsAdhoc() const { return (Ndis802_11IBSS == InfrastructureMode); }

    HRESULT Associate(UINT iMetaNetwork) const;
    HRESULT GetAssociation(LPTSTR pszGUID, UINT cchGUID) const;
    UINT GetAssociation() const;
    HRESULT ClearAssociation() const;

    HRESULT GetDisplayName(__out_ecount(cchName) LPTSTR pszName, UINT cchName) const;
    HRESULT GetRegistryName(__out_ecount(cchBuffer) LPTSTR pszBuffer, UINT cchBuffer) const;

private:
    int Compare(__notnull PCSSID pssid, UINT uMode, BOOL fMatchSSIDOnly) const;

public:
    CNetID();
};

class CWZCInterface
{
public:
    INTF_ENTRY_EX m_sIntfEntry;
    PCFGLIST      m_pcfgl;
    DWORD         m_dwValidFlags;
    DWORD         m_dwRecentFlags;
    UINT          m_rgcHidden[2];
    DWORD         m_fPower :1;

public:
    HRESULT Initialize(LPCWSTR pszDeviceName);
    void SetPowerState(BOOL fPower) { m_fPower = fPower; }
    BOOL GetPowerState() const { return m_fPower; }

    HRESULT Query(DWORD dwFlags) const;
    HRESULT QueryDirect(DWORD dwFlags);
    void Clear();

    HRESULT GetFilter(DWORD *pdwFilter);
    HRESULT SetFilter(DWORD dwFilter, BOOL fApply);

    UINT GetConfigs(DWORD dwNetFlags, __out __opt PCWLC *ppwlc = NULL) const;
    PCWLC Find(__in const CNetID *pnid, __inout __opt DWORD *pdwNetFlags = NULL) const;

    HRESULT GetConnected(__out CNetID *pnid) const;
    PCWLC GetConnected() const;

    void Hide(UINT iConfig, BOOL fPreferred);
    UINT GetInfraMode() const;
    static UINT GetSignalStrength(int nRSSI, DEVICETYPE dtType);

    HRESULT SetMostPreferred(PCWLC pwlc);
    HRESULT RemovePreferred(__notnull const CNetID *pnid);
    HRESULT SetPreferredCount(UINT cConfigs);
    HRESULT RemoveAllPreferred();
    HRESULT Apply(DWORD dwFlags);

    HRESULT Refresh(DWORD dwFlags);
    HRESULT Rescan() { return Refresh(INTF_LIST_SCAN); }

private:
    void Sanitize();
    static UINT GetNetworkQuality(__notnull PCWLC pwlc);

public:
    CWZCInterface(LPCTSTR pszDeviceName);
    ~CWZCInterface() { Clear(); }

private:
    // Copy constructor not supported
    CWZCInterface(const CWZCInterface &obj) {}
};

// wzcshell utility functions
HRESULT SHWZCConvertName(__in __opt LPCTSTR pszName, __out PSSID pssid);
void    SHWZCEncodeNetwork(__out PSHWZCNET pwzcnet, __in PCWLC pwlc);
HRESULT SHWZCDecodeNetwork(__out PWLC pwlc, __in const SHWZCNET *pwzcnet, __in __opt LPCTSTR pszNetworkKey);

int FindNetwork(__in const CNetID *pnid, __in_ecount(cConfigs) PCSHWZCNET pwzcnet, UINT cConfigs, UINT cbItem);
