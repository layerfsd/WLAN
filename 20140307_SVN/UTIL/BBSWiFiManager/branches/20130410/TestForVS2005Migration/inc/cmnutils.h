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
// Filename:    cmnutils.h
//
// Purpose:     NetUI utilities
//
//==========================================================================*
#pragma once

#include <eapol.h>
#include <wzcsapi.h>
#include <wzcshell.h>

// docbld tag, topic WiFi defined in wzcshell.cpp
/// <topic_scope tref="wifi">

// types
typedef WZC_802_11_CONFIG_LIST CFGLIST;
typedef CFGLIST                *PCFGLIST;
typedef const CFGLIST          *PCCFGLIST;

typedef NDIS_802_11_SSID       SSID;
typedef SSID                   *PSSID;
typedef const SSID             *PCSSID;

typedef WZC_WLAN_CONFIG        WLC;
typedef WLC                    *PWLC;
typedef const WLC              *PCWLC;

typedef enum _IPSTATE
{
    IPSTATE_DISCONNECTED,  // no IP address
    IPSTATE_AUTOIP,        // automatic address (DHCP is enabled, but we couldn't find a server)
    IPSTATE_CONNECTED,     // connected with a pre-configured IP address
    IPSTATE_DHCP           // connected with a dynamic IP address
} IPSTATE;

typedef enum _DEVICETYPE
{
    DEVICE_WIRED,
    DEVICE_WIRELESS,
    DEVICE_WIFI_NATIVE
} DEVICETYPE;


#ifdef __cplusplus

extern "C" {
#endif


/// <summary>
/// Get the current connected state of a network adapter, and optionally a
/// user-readable display string of its current IP address.
/// </summary>
/// <param name="pszAdapter">
/// Contains the name of the adapter to query.
/// </param>
/// <param name="pszAddress">
/// If the adapter is connected, the IP address string is returned in this
/// buffer. Since the address could be in IPv6 notation, the buffer should be
/// sufficiently long. If the buffer is not large enough to hold the address,
/// the string is truncated, and no error is returned. If the adapter is
/// disconnected (IPSTATE_DISCONNECTED), the contents of the buffer are not
/// changed, and do not contain a valid address string. This parameter can be
/// NULL if the IP address string is not needed.
/// </param>
/// <param name="cchAddress">
/// Contains the number of characters in the buffer pointed to by pszAddress.
/// This parameter is ignored if pszAddress is NULL.
/// </param>
IPSTATE GetIPState(__in LPCTSTR pszAdapter,
                   __out_ecount(cchAddress) __opt LPTSTR pszAddress,
                   UINT cchAddress);

/// <summary>
/// Get information about a network adapter, including its type.
/// </summary>
/// <param name="puNativeSignal">
/// If the device is a native Wi-Fi device, contains the signal strength on
/// exit. Set this variable to NULL if the signal strength is not required.
/// </param>
DEVICETYPE GetDeviceInfo(__in LPCTSTR pszAdapter, __out __opt int *pnRSSI);

/// <summary>
/// Get the default EAP parameters for the given network.
/// </summary>
HRESULT GetEAPDefaults(__inout PWLC pwlc);
HRESULT GetEAPDefaultType(__out DWORD *pdwType);
HRESULT GetEAPFlags(DWORD dwType, __out DWORD *pdwFlags);
HRESULT GetEAPIncremented(__inout PWLC pwlc);

/// <summary>
/// Post a status message to the Shell.
/// </summary>
void PostShellStatusMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Sets the Wifi state flags in the registry.
/// </summary>
HRESULT SetWirelessStateFlags(__in UINT uMask, __in UINT uValue);

/// <summary>
/// Gets the Wifi state flags in the registry.
/// </summary>
HRESULT GetWirelessStateFlags(__out DWORD *pdwStatus);

/// <summary>
/// Determines if a network is secure, based on authentication and encryption
/// properties. Optionally formats the SSID string for display in the UI.
/// </summary>
HRESULT IsNetworkSecure(__in LPCTSTR pszSSID, __in BOOL fUse8021x,
        __in DWORD dwAuthentication, __in DWORD dwEncryption,
        __out __opt LPTSTR *ppszSSIDDisplay, DWORD cxpExtent, __in BOOL fEscapeHtml);

/// <summary>
/// 
/// This method return TRUE for a secured network and false  for unsecured. 
/// </summary>
BOOL  IsSecure(PWLC pwlc);

/// <summary> 
/// Replace HTML characters with escape sequences. 
/// (ex. "&lt;" becomes "&amp;lt;")
/// </summary>
HRESULT EscapeHtmlString(LPTSTR pszBuffer, UINT cchBuffer);

/// <summary>
/// Pre-ellipsize a string to fit in a specified amount of space
/// (Eg. exchange.microsoft.com becomes ...ange.microsoft.com)
/// </summary>
HRESULT PreEllipsizeString(__in LPCTSTR pszInput, __out_ecount(cchEllipsized) LPTSTR pszEllipsized, UINT cchEllipsized, DWORD cxpExtent);

//  Get Index for Meta Network , Type is Internet 
UINT  GetMetaNetwork_InternetIndex(void);

/// </topic_scope>
#ifdef __cplusplus
}
#endif
