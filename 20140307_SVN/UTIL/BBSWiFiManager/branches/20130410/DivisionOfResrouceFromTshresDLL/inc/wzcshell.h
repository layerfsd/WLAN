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
// Copyright (c) Microsoft Corporation.  All rights reserved. 
//
// Use of this source code is subject to the terms of your Windows Mobile 
// Software Shared Source Premium Derivatives License Form or other 
// applicable Microsoft license agreement for the software. If you did not 
// accept the terms of such a license, you are not authorized to use this 
// source code.
/*---------------------------------------------------------------------------*\
 *
 * Wi-Fi Zero Configuration
 *
 *
 *  module: wzcshell.h
 *
 *  purpose: Windows Mobile Wi-Fi shell interface
 *
 *  history: 6 Feb 2004     Created
 *
\*---------------------------------------------------------------------------*/

#ifndef __INCLUDED_WZCSHELL_H__
#define __INCLUDED_WZCSHELL_H__
#pragma once


#include <pcommctr.h>
#include <ehm.h>
#include <guts.h>

///<file_topic_scope tref="wzcshell"/>

// callback creation flags
#define SHWZCF_CB_OVERRIDE               0x0001  // disable the default notification UI
#define SHWZCF_CB_SHARED                 0x0002  // share this handle throughout this process

// global flags
#define SHWZCF_ALLOW_MASK                0x0003
#define SHWZCF_ALLOW_ADHOC               0x0000
#define SHWZCF_ALLOW_AP                  0x0001
#define SHWZCF_ALLOW_ALL                 0x0002
#define SHWZCF_CONNECT_ANY               0x0004

// display flags
#define SHWZCF_UI_AUTOKEY_SET            0x0001
#define SHWZCF_UI_AUTOKEY_DISABLED       0x0002
#define SHWZCF_UI_KEYDATA_DISABLED       0x0004
#define SHWZCF_UI_KEYINDEX_DISABLED      0x0008
#define SHWZCF_UI_8021X_DISABLED         0x0010
#define SHWZCF_UI_EAPTYPE_DISABLED       0x0020

// hardware state flags
#define SHWZCF_HW_PRESENT                0x0001
#define SHWZCF_HW_CONNECTED              0x0002
#define SHWZCF_HW_SUPPORT_WPA            0x0004
#define SHWZCF_HW_CONNECTING             0x0008
#define SHWZCF_HW_ON                     0x0010
#define SHWZCF_HW_NETAVAIL               0x0020
#define SHWZCF_HW_SUPPORT_WPA2           0x0040
#define SHWZCF_HW_SUPPORT_WPA_AES        0x0080
#define SHWZCF_HW_DISABLED               0x0100


// network flags
#define SHWZCF_NET_BROADCAST             0x0001  // network is currently on-the-air
#define SHWZCF_NET_PREFERRED             0x0002  // we automatically connect to this network
#define SHWZCF_NET_CONNECTING            0x0004  // the network is connecting
#define SHWZCF_NET_CONNECTED             0x0008  // the network is currently connected
#define SHWZCF_NET_ADHOC                 0x0010  // the network is a device-to-device network
#define SHWZCF_NET_KEYDATA               0x0020  // there is key data associated with this network
#define SHWZCF_NET_8021X                 0x0040  // the network is using 802.1x
#define SHWZCF_NET_REQUIRE_WZC_PROBE     0x0080  // the network may not visibly broadcast its SSID, so
                                                 // WZC needs to probe for it to reliably connect

// SetNetwork mask flags
#define SHWZCF_SETMASK_ALL               0x007F
#define SHWZCF_SETMASK_ADHOC             0x0001  // set the adhoc/infrastructure bit
#define SHWZCF_SETMASK_AUTHENTICATION    0x0002  // set the authentication type
#define SHWZCF_SETMASK_ENCRYPTION        0x0004  // set the encryption type
#define SHWZCF_SETMASK_KEYDATA           0x0008  // set the network key data
#define SHWZCF_SETMASK_KEYINDEX          0x0010  // set the network key index
#define SHWZCF_SETMASK_8021X             0x0020  // set the 802.1x data
#define SHWZCF_SETMASK_REQUIRE_WZC_PROBE 0x0040  // set the hidden/broadcast bit

// SHWZCGetWiFiStatusString flags
#define SHWZCF_GWSS_INCLUDE_WIFI         0x0001

// the handle type
typedef HANDLE HSHWZC;

typedef struct _SHWZCNET
{
    TCHAR szName[48];                // user-readable network name
    DWORD dwFlags;                   // a combination of network flags
    DWORD dwAuthentication;          // the type of authentication required (or 0 for none)
    DWORD dwEncryptionType;          // the encryption type
    DWORD dwKeyIndex;                // the currently selected key index (valid only if SHWZCF_NET_KEYVALID is set)
    DWORD dwEAPType;                 // the EAP type (valid only if SHWZCF_NET_8021X is set)
    DWORD nRssi;                     // the RSSI of the network, from 1-4
} SHWZCNET, *PSHWZCNET;
typedef const SHWZCNET *PCSHWZCNET;

typedef struct _SHWZCEAP
{
    DWORD dwType;
    TCHAR szName[MAX_PATH];
    DWORD fProperties :1;
} SHWZCEAP, *PSHWZCEAP;

// error codes
#define SHWZC_E_INSUFFICIENT_BUFFER    HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)
#define SHWZC_E_INVALID_NAME           HRESULT_FROM_WIN32(ERROR_INVALID_NAME)
#define SHWZC_E_HARDWARE               HRESULT_FROM_WIN32(ERROR_NOT_READY)

// network events
#define SHWZC_EVENT_ADAPTER             0  // lParam is TRUE if a new adapter was added, FALSE if removed
#define SHWZC_EVENT_BROADCASTCHANGE     1  // lParam is not used
#define SHWZC_EVENT_CONNECTING          2  // lParam is TRUE if connecting, FALSE if not
#define SHWZC_EVENT_CONNECTION          3  // lParam is TRUE if the network was connected, FALSE if disconnected

// callback function prototypes
typedef BOOL (CALLBACK *PFN_WZCENUMDESTID)(LPCWSTR pszGUID, LPCWSTR pszName, LPVOID pvParam);
typedef BOOL (CALLBACK *PFN_WZCENUMTYPE)(DWORD dwType, LPCWSTR pszName, LPVOID pvParam);


#ifdef __cplusplus

extern "C" {
#endif

/// <summary>
/// Register for network event notifications, and optionally disable the
/// default Wi-Fi notification UI.
/// </summary>
/// <param name="hwndCallback">
/// The window handle that should receive callbacks. This parameter cannot be
/// NULL.
/// </param>
/// <param name="uCallbackMsg">
/// The message identifier that the window will receive when a network event
/// occurs. The message parameters will contain the values defined above.
/// </param>
/// <param name="uScanInterval">
/// The number of seconds to wait between hardware network scans, or 0 if no
/// scanning is required.
/// </param>
/// <param name="dwFlags">
/// A combination of SHWZCF_CB_xxx flags.
/// </param>
/// <param name="phshwzc">
/// If successful, contains a handle on output. The handle can be used to make
/// changes to the network configuration, and should be freed when no longer
/// needed by calling SHWZCDestroyCallback.
/// </param>
HRESULT WINAPI SHWZCCreateCallback(HWND hwndCallback, UINT uCallbackMsg, UINT uScanInterval,
                                   DWORD dwFlags, HSHWZC *phshwzc);

/// <summary>
/// Apply any recent changes to the network configuration.
/// </summary>
HRESULT WINAPI SHWZCApplyChanges(HSHWZC hshwzc);

/// <summary>
/// Destroy a callback object, and remove its associated window from the
/// callback list. This function also resumes the default scanning behavior,
/// and restores the normal notifications UI if no other overrides are present.
/// Any pending changes are lost.
/// </summary>
HRESULT WINAPI SHWZCDestroyCallback(HSHWZC hshwzc);

/// <summary>
/// Get the display data for a network.
/// </summary>
HRESULT WINAPI SHWZCGetDisplayFlags(const SHWZCNET *pwzcnet, DWORD *pdwFlags);

/// <summary>
/// Get the current global state.
/// </summary>
HRESULT WINAPI SHWZCGetState(HSHWZC hshwzc, DWORD *pdwState);

/// <summary>
/// Set the global state.
/// </summary>
HRESULT WINAPI SHWZCSetState(HSHWZC hshwzc, DWORD dwState);

/// <summary>
/// Start a rescan of the available networks, and return immediately.
/// </summary>
HRESULT WINAPI SHWZCRescanNetworks();

/// <summary>
/// Get the current network list.
/// </summary>
HRESULT WINAPI SHWZCGetAllNetworks(HSHWZC hshwzc, HDSA *phdsaNetworks);

/// <summary>
/// Get the state of one network.
/// </summary>
/// <returns>
/// S_OK if successful, S_FALSE if the network was not found (but suitable
/// defaults were loaded).
/// </returns>
HRESULT WINAPI SHWZCGetNetwork(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc, PSHWZCNET pwzcnet,
                               LPVOID *ppvEapData, UINT *pcbEapData);

/// <summary>
/// Change network settings.
/// </summary>
HRESULT WINAPI SHWZCCopySettings(SHWZCNET *pwzcnetDest, const SHWZCNET *pwzcnetSource, DWORD dwMask);

/// <summary>
/// Set the state of one network.
/// </summary>
HRESULT WINAPI SHWZCSetNetwork(HSHWZC hshwzc, const SHWZCNET *pwzcnet, LPCWSTR pszNetworkKey,
                               LPCVOID pvEapData, UINT cbEapData, DWORD dwMask);

/// <summary>
/// Remove a network from the preferred list.
/// </summary>
HRESULT WINAPI SHWZCDeleteNetwork(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc);

/// <summary>
/// Remove all networks from the preferred list.
/// </summary>
HRESULT WINAPI SHWZCDeleteAllNetworks(HSHWZC hshwzc);

/// <summary>
/// Immediately move a network to the top of the preferred list and attempt to connect it.
/// </summary>
HRESULT WINAPI SHWZCConnectNetwork(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc);

/// <summary>
/// Get the destination GUID for a network.
/// </summary>
HRESULT WINAPI SHWZCGetDestinationGUID(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc, LPWSTR pszGUID, UINT cchGUID);

/// <summary>
/// Set the destination GUID for a network.
/// </summary>
HRESULT WINAPI SHWZCSetDestinationGUID(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc, LPCWSTR pszGUID);

/// <summary>
/// Enumerate the available Wi-Fi destination network GUIDs.
/// </summary>
HRESULT WINAPI SHWZCEnumDestinations(PFN_WZCENUMDESTID pfnCallback, LPVOID pvParam);

/// <summary>
/// Enumerate the available Wi-Fi authentication type codes.
/// </summary>
HRESULT WINAPI SHWZCEnumAuthenticationTypes(BOOL fAdhoc,
                                            PFN_WZCENUMTYPE pfnCallback, LPVOID pvParam);

/// <summary>
/// Enumerate the available Wi-Fi encryption type codes.
/// </summary>
HRESULT WINAPI SHWZCEnumEncryptionTypes(const SHWZCNET *pwzcnet,
                                        PFN_WZCENUMTYPE pfnCallback, LPVOID pvParam);

/// <summary>
/// Get a list of available EAP types.
/// </summary>
HRESULT WINAPI SHWZCCreateEAPList(HDSA *phdsaEAP);

/// <summary>
/// Destroy the EAP data list.
/// </summary>
void WINAPI SHWZCDestroyEAPList(HDSA hdsaEAP);

/// <summary>
/// Checks to see if a path exists for the EAP Extension UI. 
/// [in] dwType - The EAP type to check
/// [in] hdsaEAP - The EAP data list
/// [out] pfExtensionValid - Set to FALSE by default.  Is set to TRUE if the EAP has a path to an extension UI.
/// </summary>
HRESULT WINAPI SHWZCIsEAPExtensionValid(DWORD dwType, HDSA hdsaEAP, BOOL *pfExtensionValid);

/// <summary>
/// Display the configuration UI for the specified EAP type, and update the
/// user data as necessary.
/// </summary>
HRESULT WINAPI SHWZCConfigureEAPData(HWND hwndOwner, DWORD dwType, HDSA hdsaEAP);

/// <summary>
/// Set the user data for a specified EAP type.
/// </summary>
HRESULT WINAPI SHWZCSetEAPData(DWORD dwType, LPCVOID pvData, UINT cbData, HDSA hdsaEAP);

/// <summary>
/// Retrieve the user data for a specified EAP type.
/// </summary>
HRESULT WINAPI SHWZCGetEAPData(DWORD dwType, LPVOID *ppvData, UINT *pcbData, HDSA hdsaEAP);

/// <summary>
/// Get the default network configuration.
/// </summary>
HRESULT WINAPI SHWZCCreateDefaultNetwork(PSHWZCNET pwzcnet);

/// <summary>
/// Perform a quick sanity check of a network key to see if it's valid before
/// using it in a network configuration. This checks things like length,
/// acceptable characters, etc.
/// </summary>
BOOL WINAPI SHWZCValidateKey(LPCWSTR pszNetworkKey, DWORD dwEncryptionType, DWORD dwAuthenticationType);

/// <summary>
/// Check for valid combination of authentication and encryption types
/// </summary>
BOOL WINAPI SHWZCValidateSecurityConfig(DWORD dwEncryptionType, DWORD dwAuthenticationType, BOOL fAdhoc);

/// <summary>
/// Get the current hardware state.
/// </summary>
UINT WINAPI SHWZCGetHardwareFlags();

/// <summary>
/// Get the current signal strength for a wireless adapter.
/// Optionally, get the signal strength for a wireless network detected by the adapter
/// </summary>
/// <param name="pszAdapter">
/// The name of the wireless adapter to query. If this parameter is NULL, the
/// first wireless adapter found will be queried.
/// </param>
/// <param name="pszName">
/// The name of the wireless network.  If the parameter is NULL, then the signal strength of 
/// the currently connected network will be returned.
/// </param>
/// <param name="fAdhoc">
/// Whether the queried wireless network is Ad-hoc or not. This parameter is ignored
/// if pszName is NULL.
/// </param>
/// <returns>
/// If pszName is NULL, returns S_OK if successful and the specified adapter is present and
/// connected, or S_FALSE if successful but the adapter is not connected. In
/// this case, *puStrength will be set to zero. 
/// If pszName is not NULL, return S_OK if successful and the specified wireless 
/// network is available, or S_FALSE if the specified wireless is not available. In this 
/// case, *puStrength will be set to zero.
/// Returns SHWZC_E_HARDWARE if the specified adapter is not present.
/// </returns>
HRESULT WINAPI SHWZCGetSignalStrength(LPCTSTR pszAdapter, UINT *puStrength, LPCTSTR pszName, BOOL fAdhoc);

/// <summary>
/// Determine if a name is a valid SSID name, and optionally copy the SSID text
/// into a buffer.
/// </summary>
HRESULT WINAPI SHWZCValidateNetworkName(LPCTSTR pszName, LPSTR pszSSID, UINT cchSSID);

/// <summary>
/// Determine if a configured network already exists.
/// </summary>
HRESULT WINAPI SHWZCIsDuplicateName(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc);

/// <summary>
/// Determine if a network is a Permanent network - that is, it isn't an Ad Hoc network and its
/// name is listed in an OEM configured list of permanent non-editable SSIDs.
/// </summary>
BOOL WINAPI SHWZCIsPermanentNetwork(LPCTSTR pszName, BOOL fAdHoc);

/// <summary>
/// Turn on or off WiFi new network notifications
/// </summary>
HRESULT WINAPI SHWZCSetNotificationState(BOOL fNotificationsOn);

/// <summary>
/// Determines whether or not WiFi new network notifications are enabled
/// </summary>
HRESULT WINAPI SHWZCGetNotificationState(BOOL* fNotificationsOn);

/// <summary>
/// Sets all managed (WiFi) devices on or off
/// </summary>
HRESULT WINAPI SHWZCSetPowerState(BOOL fOn);

/// <summary>
/// returns S_OK if any managed (WiFi) devices are on, else S_FALSE
/// </summary>
HRESULT WINAPI SHWZCGetPowerState();

/// <summary>
/// Sets the current WiFi timeout period in seconds
/// </summary>
HRESULT WINAPI SHWZCSetWifiTimeoutPeriod(UINT uTimeoutPeriod);

/// <summary>
/// Determines the current WiFi timeout period in seconds
/// </summary>
HRESULT WINAPI SHWZCGetWifiTimeoutPeriod(UINT *puTimeoutPeriod);

// <summary>
// Returns a string to represent Wi-Fi status in human-readable format.
// Possible strings in 0409:
//    "Available"
//    "Unavailable"
//    "Off"
//    An SSID
//
// If dwFlags has the SHWZC_GWSS_INCLUDE_WIFI flag set, then the string will 
// be formatted with "%1!s!: %2!s!" (0409), with %1 being "Wi-Fi" and %2
// being one of the strings above. It is possible for an OEM to override the
// "Wi-Fi" portion of the string since the "Wi-Fi" string will be called via 
// LoadStringOver().
// Return values: S_OK if an SSID was set, S_FALSE if no SSID was set, 
//                or an HRESULT error code.
// </summary>
HRESULT WINAPI SHWZCGetWiFiStatusString(DWORD dwFlags, LPTSTR pszStatus, DWORD cchStatus);



// Helper function used by SHWZCGetWiFiStatusString
HRESULT GetWiFiStatusStringFromFlags(DWORD dwWiFiFlags, DWORD dwDisplayFlags, LPTSTR pszStatus, DWORD cchStatus);




#ifdef __cplusplus
}
#endif
#endif  // __INCLUDED_WZCSHELL_H__
