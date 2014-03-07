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
/*---------------------------------------------------------------------------*\
 *
 * Wi-Fi Zero Configuration
 *
 *  module: wzcshell.cpp
 *
 *  purpose: Windows Mobile Wi-Fi shell interface
 *
 *  history: 6 Feb 2004     Created
 *
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include <wzcshell.h>
#include <wpcpriv.h>
#include <ehm.h>
#include <miscsvcs_priv.h>
#include <wrlspwr.h>
#include <ststore.h>
#include <snapi.h>

#include "netuip.h"
#include "wzcuip.h"
#include "wzcmgr.h"
#include "connmgr_priv.h"
#include "cellcoreresource.h"
#include "resource.h"


#define HR2LR(hr)   (LRESULT)(SUCCEEDED(hr)?(hr)+1:(hr))
#define CLREx(lr,e) do{LRESULT lrval=(lr);CHR((HRESULT)lrval);CBR(lrval);hr=(HRESULT)(lrval-1);}while(0,0)
#define CLR(lr)     CLREx(lr,E_FAIL)

// docbld tag for entire file

///<topic name="wifi" displayname="WiFi">
/// <topic_scope tref="connections"/>
/// <summary>Shell WiFi</summary>
///</topic>

///<topic name="wzcshell" displayname="WZC Shell">
/// <summary>The WZC Shell node contains common core APIs called by both the SP and PPC WiFi settings UI as well as some APIs
/// useful for shell in general (for example, the quicklist and wireless power manager use wzcshell directly).</summary>
/// <topic_scope tref="wifi"/>
/// </topic>

///<topic_scope tref="wzcshell">

typedef struct _SHWZCNETLIST
{
    DWORD    cNetworks;       // the number of items in the array
    SHWZCNET wzcnetArray[1];  // the network list
} SHWZCNETLIST, *PSHWZCNETLIST;

struct _TYPEINFO
{
    UINT  uStringID;
    DWORD dwType;
};

static const _TYPEINFO g_rgAuthTypes[] =
{
    { IDS_WZC_AUTHENTICATION_OPEN,     Ndis802_11AuthModeOpen },
    { IDS_WZC_AUTHENTICATION_SHARED,   Ndis802_11AuthModeShared },
    { IDS_WZC_AUTHENTICATION_WPA,      Ndis802_11AuthModeWPA },
    { IDS_WZC_AUTHENTICATION_WPA_PSK,  Ndis802_11AuthModeWPAPSK },
    { IDS_WZC_AUTHENTICATION_WPA_NONE, Ndis802_11AuthModeWPANone },
    { IDS_WZC_AUTHENTICATION_WPA2,     Ndis802_11AuthModeWPA2 },
    { IDS_WZC_AUTHENTICATION_WPA2_PSK, Ndis802_11AuthModeWPA2PSK }
};

static const UINT g_rguAuthInfraSupport[] = { 0, 1, 2, 3, 5, 6};
static const UINT g_rguAuthAdhocSupport[] = { 0, 1, 4 };

static const TCHAR g_szRegKeyWZCSecure[]       = TEXT("Comm\\Wifi");

static const _TYPEINFO g_rgEncryptTypes[] =
{
    { IDS_WZC_ENCRYPTION_DISABLED, Ndis802_11EncryptionDisabled },
    { IDS_WZC_ENCRYPTION_WEP,      Ndis802_11Encryption1Enabled },
    { IDS_WZC_ENCRYPTION_TKIP,     Ndis802_11Encryption2Enabled },
    { IDS_WZC_ENCRYPTION_AES,      Ndis802_11Encryption3Enabled }
};

struct ENCRYPTSUPPORT
{
    UINT iFirst;
    UINT iLast;
};

// these are indices into g_rgEncryptTypes
static const ENCRYPTSUPPORT g_rgEncryptSupport[ARRAYSIZE(g_rgAuthTypes)] =
{
    {0, 1}, {0, 1}, {2, 3}, {2, 3}, {2, 3}, {2, 3}, {2, 3}
};

// so are these
static const UINT s_rguDefEncrypt[ARRAYSIZE(g_rgAuthTypes)] = { 1, 1, 2, 2, 2, 2, 2 };

// Number of seconds that SendMessage is going to wait before timing out in 
// QueryNetworks
static const UINT g_uiSendMessageTimeout = 10000;

#define SHWZCF_CB_MASK     0x7FFFFFFF
#define SHWZCF_CB_SETSTATE 0x80000000

struct SHWZCNETEX
{
    SETNETWORKITEM sni;
    LPVOID         pvEapData;
    UINT           iMeta;
};

struct SHWZCCB
{
    HWND  hwndCallback;
    HDSA  hdsaNetworks;
    DWORD dwFlags;
};

struct SHWZCEAPEX : public _SHWZCEAP
{
    TCHAR  szConfigUIPath[MAX_PATH];
    LPVOID pvData;
    UINT   cbData;
};

enum SHWZCMSG
{
    SHWZCMSG_DATA_SETNETWORKS,
    SHWZCMSG_DATA_DELETENETWORK,
    SHWZCMSG_DATA_SIGNALSTRENGTH
};


extern LPCTSTR g_szRegKeyBase;
LPCTSTR        g_szValueFormat      = TEXT("%08X");
extern LPCTSTR g_szRegKeyCallbacks;
LPCTSTR        g_szRegPathCallbacks = WZC_REGKEY_BASE TEXT("\\") WZC_REGKEY_CALLBACKS;
LPCTSTR        g_szEventMessage     = TEXT("WZCEvent");
extern LPCTSTR g_szSinkClassWireless;

// Smartphone uses the same reg key as the PPC notifications reg key in order
// to simplify wzcshell.cpp.  The only key that matters on Smartphone is  "Options"
// where the flags (NOTIF_MESSAGE | NOTIF_SOUND) enables or disables WiFi notifications
// On PPC, the Sounds & Notifications control panel sets the value of Options.
LPCTSTR        g_szRegKeyWiFiNotifications = TEXT("ControlPanel\\Notifications\\{DDBD3B44-80B0-4b24-9DC4-839FEA6E559E}\\");

static HRESULT SetNetworks_I(__in_bcount(cbData) LPCVOID pvData, DWORD cbData);
static HRESULT DeleteNetwork_I(__in_bcount(cbData) LPCVOID pvData, DWORD cbData);
static HRESULT GetSignalStrength_I(__in_bcount(cbData) LPCVOID pvData, DWORD cbData);

static const UINT g_uMinWifiTimeout = 20;
static const UINT g_uMaxWifiTimeout = UINT_MAX / 1000;

static HWND SHWZCFindWindow()
{
    TCHAR chNull = 0;

    // Find windows with no title that match our class name
    return FindWindow(g_szSinkClassWireless, &chNull);
}

static HRESULT SendDataMessageEx(HWND hwndSink,
                                 DWORD dwCode, __in_bcount(cbData) LPCVOID pvData, DWORD cbData)
{
    HRESULT        hr = S_OK;
    COPYDATASTRUCT cds = { dwCode, cbData, (LPVOID)pvData };
    LRESULT        lr;

    // Send the message
    lr = SendMessage(hwndSink, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);
    CBREx(0 != lr, SHWZC_E_HARDWARE);

    // The return value is an encoded HRESULT
    hr = (HRESULT)(lr > 0 ? lr-1 : lr);

Error:
    return hr;
}

static HRESULT SendDataMessage(DWORD dwCode, __in_bcount(cbData) LPCVOID pvData, DWORD cbData)
{
    HRESULT hr = S_OK;
    HWND    hwndSink;

    // Find a sink window
    hwndSink = SHWZCFindWindow();
    CBREx(NULL != hwndSink, SHWZC_E_HARDWARE);

    // Send the data message
    hr = SendDataMessageEx(hwndSink, dwCode, pvData, cbData);
    CHR(hr);

Error:
    return hr;
}

LRESULT ReceiveDataMessage(DWORD dwCode, __in_bcount(cbData) LPCVOID pvData, DWORD cbData)
{
    HRESULT hr = S_OK;

    switch (dwCode)
    {
    case SHWZCMSG_DATA_SETNETWORKS:
        hr = SetNetworks_I(pvData, cbData);
        CHR(hr);
        break;

    case SHWZCMSG_DATA_DELETENETWORK:
        hr = DeleteNetwork_I(pvData, cbData);
        CHR(hr);
        break;

    case SHWZCMSG_DATA_SIGNALSTRENGTH:
        hr = GetSignalStrength_I(pvData, cbData);
        CHR(hr);
        break;

    default:
        // We don't know about this code
        CHRA(E_NOTIMPL);
    }

Error:
    // Encode the HRESULT into an LRESULT
    return (LRESULT)(SUCCEEDED(hr) ? hr+1 : hr);
}

static HRESULT GetSharedMemory(DWORD cbBytes, BOOL fCreate, __notnull LPVOID *ppvBuffer)
{
    HRESULT hr = S_OK;
    HANDLE  hMap = NULL;
    LPVOID  pvAlloced = NULL;

    ASSERT(NULL != ppvBuffer);

    // Check inputs
    if (0 == cbBytes)
    {
        *ppvBuffer = NULL;
        hr = S_FALSE;
        goto Exit;
    }

    // Create the object
    static const TCHAR s_szName[] = TEXT("SHWZCShared");
    hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
        PAGE_READWRITE, 0, cbBytes, s_szName);
    CWR(hMap);

    // Check the creation state
    CBR(fCreate == (ERROR_ALREADY_EXISTS != GetLastError()));

    // Map a view of the file
    pvAlloced = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, cbBytes);
    CWR(pvAlloced);

    // Success
    *ppvBuffer = pvAlloced;

Error:
    if (hMap)
    {
        CloseHandle(hMap);
    }

Exit:
    return hr;
}

static void FreeSharedMemory(LPVOID pvBuffer)
{
    if (pvBuffer)
    {
        VBR(UnmapViewOfFile(pvBuffer));
    }
}

static HRESULT PostEvent(UINT uEventID, LPARAM lp)
{
    HRESULT     hr = S_OK;
    static UINT s_uEventMsg = 0;

    // Get the registered message
    if (0 == s_uEventMsg)
    {
        s_uEventMsg = RegisterWindowMessage(g_szEventMessage);
        CBR(s_uEventMsg);
    }

    // Post it to all windows
    PostMessage(HWND_BROADCAST, s_uEventMsg, (WPARAM)uEventID, lp);

Error:
    return hr;
}

static UINT FindType(DWORD dwType, const _TYPEINFO *pTypes, UINT cTypes)
{
    UINT i;

    for (i = 0; i != cTypes; ++i)
    {
        if (pTypes[i].dwType == dwType)
        {
            break;
        }
    }

    return i;
}

static DWORD SHWZCGetEAPDefaultTypeDirect()
{
    DWORD dwType = DEFAULT_EAP_TYPE;
    VHR(GetEAPDefaultType(&dwType));
    return dwType;
}

static DWORD SHWZCGetEAPDefaultType()
{
    static DWORD dwDefEAPType = SHWZCGetEAPDefaultTypeDirect();
    return dwDefEAPType;
}

void SHWZCEncodeNetwork(PSHWZCNET pwzcnet, PCWLC pwlc)
{
    CNetID nid;
    UINT   i;

    ASSERT(NULL != pwzcnet);
    ASSERT(NULL != pwlc);

    // Initialize
    ZeroMemory(pwzcnet, sizeof(SHWZCNET));

    // Copy the network name (this should never fail!!)
    nid.Assign(pwlc);
    VHR(nid.GetDisplayName(pwzcnet->szName, ARRAYSIZE(pwzcnet->szName)));
    if (pwlc->InfrastructureMode == Ndis802_11IBSS)
    {
        // Set the adhoc flag
        pwzcnet->dwFlags |= SHWZCF_NET_ADHOC;
    }

    // Fill in security data
    pwzcnet->dwAuthentication = pwlc->AuthenticationMode;
    pwzcnet->dwEncryptionType = pwlc->Privacy;
    pwzcnet->dwEAPType = pwlc->EapolParams.dwEapType;

    // Network key information
    pwzcnet->dwKeyIndex = pwlc->KeyIndex + 1;
    if (0 != (WZCCTL_WEPK_PRESENT & pwlc->dwCtlFlags) &&
        0 != pwlc->KeyLength)
    {
        pwzcnet->dwFlags |= SHWZCF_NET_KEYDATA;
    }

    // Is 802.1x enabled?
    if (pwlc->EapolParams.bEnable8021x)
    {
        pwzcnet->dwFlags |= SHWZCF_NET_8021X;
    }

    // Is this network marked as visible by the driver
    if (pwlc->dwCtlFlags & WZCCTL_BROADCAST_SSID)
    {
        // WZC can already see this as a visible network - the access point does not need
        // to be probed if it isn't in range
        pwzcnet->dwFlags &= ~SHWZCF_NET_REQUIRE_WZC_PROBE;
    }
    else
    {
        pwzcnet->dwFlags |= SHWZCF_NET_REQUIRE_WZC_PROBE;
    }
    
    // Check the authentication type
    i = FindType(pwzcnet->dwAuthentication, g_rgAuthTypes, ARRAYSIZE(g_rgAuthTypes));
    if (ARRAYSIZE(g_rgAuthTypes) <= i)
    {
        // If you hit this assertion, it means the network has an
        // authentication type that our UI doesn't know about!
        pwzcnet->dwAuthentication = Ndis802_11AuthModeOpen;
        ASSERT(FALSE);
    }

    // Check the encryption type
    i = FindType(pwzcnet->dwEncryptionType, g_rgEncryptTypes, ARRAYSIZE(g_rgEncryptTypes));
    if (ARRAYSIZE(g_rgEncryptTypes) <= i)
    {
        // If you hit this assertion, it means the network has an
        // encryption type that our UI doesn't know about!
        pwzcnet->dwEncryptionType = Ndis802_11EncryptionDisabled;
        ASSERT(FALSE);
    }

    // If no EAP type was specified, the UI should treat it as the default (PEAP)
    if (0 == pwzcnet->dwEAPType)
    {
        pwzcnet->dwEAPType = SHWZCGetEAPDefaultType();
    }
}

HRESULT SHWZCDecodeNetwork(PWLC pwlc, const SHWZCNET *pwzcnet, LPCTSTR pszNetworkKey)
{
    HRESULT hr = S_OK;

    // Set the SSID string
    hr = SHWZCConvertName(pwzcnet->szName, &pwlc->Ssid);
    CHR(hr);

    // Set the network type
    pwlc->InfrastructureMode = ((pwzcnet->dwFlags & SHWZCF_NET_ADHOC) ? Ndis802_11IBSS : Ndis802_11Infrastructure);

    // Set the authentication type
    CBREx(ARRAYSIZE(g_rgAuthTypes) > FindType(pwzcnet->dwAuthentication,
        g_rgAuthTypes, ARRAYSIZE(g_rgAuthTypes)), E_INVALIDARG);
    pwlc->AuthenticationMode = (NDIS_802_11_AUTHENTICATION_MODE)pwzcnet->dwAuthentication;

    // Set the encryption type
    CBREx(ARRAYSIZE(g_rgEncryptTypes) > FindType(pwzcnet->dwEncryptionType,
        g_rgEncryptTypes, ARRAYSIZE(g_rgEncryptTypes)), E_INVALIDARG);
    pwlc->Privacy = pwzcnet->dwEncryptionType;

    // Set the network key information
    CBREx(1 <= pwzcnet->dwKeyIndex && pwzcnet->dwKeyIndex <= 4, E_INVALIDARG);
    pwlc->KeyIndex = pwzcnet->dwKeyIndex - 1;

    if (NULL != pszNetworkKey)
    {
        // Store the network key
        CBREx(0 < CWZCManagerWireless::WritePassword(pwlc,
            pszNetworkKey, pwlc->Privacy, pwlc->AuthenticationMode), E_INVALIDARG);
    }

    // Save the 802.1x information
    pwlc->EapolParams.bEnable8021x = BOOLIFY(pwzcnet->dwFlags & SHWZCF_NET_8021X);
    if (pwlc->EapolParams.bEnable8021x)
    {
        pwlc->EapolParams.dwEapType = pwzcnet->dwEAPType;
        pwlc->EapolParams.dwEapFlags = DEFAULT_EAP_STATE;
        if (! IsAnyWPAPSK(pwlc->AuthenticationMode))
        {
            // Get the registry configured state for this EAP type
            GetEAPFlags(pwlc->EapolParams.dwEapType, &pwlc->EapolParams.dwEapFlags);
        }
    }

    // Mark visible state so that the driver knows whether to probe for this network or not
    // If we don't explicitly mark the network as one that broadcasts its SSID, then WZC needs
    // to try and connect to it even if it isn't airing just in case it turns out to be a hidden
    // network
    if (pwzcnet->dwFlags & SHWZCF_NET_REQUIRE_WZC_PROBE)
    {
        // Clear broadcast flag when a probe is needed
        pwlc->dwCtlFlags &= ~WZCCTL_BROADCAST_SSID;
    }
    else
    {
        pwlc->dwCtlFlags |= WZCCTL_BROADCAST_SSID;
    }

Error:
    return hr;
}

static HRESULT QueryNetworks(HWND hwndSink, HDSA hdsa, DWORD dwFlags)
{
    HRESULT       hr = S_OK;
    UINT          cInitNetworks;
    const UINT    cbBase = sizeof(SHWZCNETLIST) - sizeof(SHWZCNET);
    UINT          cbState;
    UINT          cbNeed;
    PSHWZCNETLIST pwzcnlShared = NULL;

    // Check inputs
    CBRAEx(NULL != hwndSink, E_INVALIDARG);
    CBRAEx(NULL != hdsa, E_INVALIDARG);
    CBRAEx(0 != dwFlags, E_INVALIDARG);  // You should request broadcast, preferred, or both

    // Get the size of the initial allocation
    cInitNetworks = DSA_GetItemCount(hdsa);
    cbState = cbBase + sizeof(SHWZCNET) * (cInitNetworks + 4);  // big save for the 90% case

    for(;;)
    {
        UINT i;

        // Allocate shared memory
        hr = GetSharedMemory(cbState, TRUE, (void**)&pwzcnlShared);
        CHR(hr);

        // Copy the current networks
        pwzcnlShared->cNetworks = cInitNetworks;
        for (i = 0; i != cInitNetworks; ++i)
        {
            memcpy(&pwzcnlShared->wzcnetArray[i], DSA_GetItemPtr(hdsa, i), sizeof(SHWZCNET));
        }

        // Send a message to the sink window
        DWORD_PTR dwResult = 0;

        if (SendMessageTimeout(
                hwndSink, 
                SHWZCM_GETNETLIST, 
                cbState, 
                dwFlags,
                SMTO_NORMAL,
                g_uiSendMessageTimeout,
                &dwResult
                ) == 0)
        {
            DWORD error = GetLastError();
            if (error == 0)
            {
                hr = HRESULT_FROM_WIN32(ERROR_TIMEOUT);
            }
            else
            {
                hr = HRESULT_FROM_WIN32(error);
            }
        }

        hr = SUCCEEDED(hr) ? (HRESULT)dwResult : hr;
        CHR(hr);

        // How much memory do we need
        CBRA(UMul(pwzcnlShared->cNetworks, sizeof(SHWZCNET), &cbNeed));
        CBRA(UAdd(cbNeed, cbBase, &cbNeed));
        if (cbNeed <= cbState)
        {
            // Copy the data to the DSA
            CBREx(DSA_Grow(hdsa, pwzcnlShared->cNetworks), E_OUTOFMEMORY);
            hdsa->cItem = pwzcnlShared->cNetworks;
            for (i = 0; i != pwzcnlShared->cNetworks; ++i)
            {
                memcpy(DSA_GetItemPtr(hdsa, i), &pwzcnlShared->wzcnetArray[i], sizeof(SHWZCNET));
            }

            break;
        }

        // Free the shared memory
        FreeSharedMemory(pwzcnlShared);
        pwzcnlShared = NULL;
        cbState = cbNeed;
    }

Error:
    FreeSharedMemory(pwzcnlShared);
    return hr;
}

HRESULT SHWZCGetCurrentNetworksMessage_I(WPARAM wParam, LPARAM lParam)
{
    HRESULT        hr = S_OK;
    HDSA           hdsa = NULL;
    const UINT     cbBase = sizeof(SHWZCNETLIST) - sizeof(SHWZCNET);
    DWORD          cbState = wParam;
    PSHWZCNETLIST  pwzcnlShared = NULL;
    DWORD          dwFlags = lParam;

    // Get the shared memory
    hr = GetSharedMemory(cbState, FALSE, (void**)&pwzcnlShared);
    CHR(hr);

    // Build the current network list
    hdsa = DSA_Create(sizeof(SHWZCNET), 4);
    CWR(NULL != hdsa);

    if (0 != pwzcnlShared->cNetworks)
    {
        // Reset the state flags that we're querying
        for (UINT i = 0; i != pwzcnlShared->cNetworks; ++i)
        {
            pwzcnlShared->wzcnetArray[i].dwFlags &= ~dwFlags;
        }

        // Copy in the provided networks
        CBREx(DSA_SetRange(hdsa, 0, pwzcnlShared->cNetworks, pwzcnlShared->wzcnetArray), E_OUTOFMEMORY);
    }

    hr = CWZCManager::SendAllPrivateMessage(PM_GETNETLIST, (WPARAM)hdsa, (LPARAM)dwFlags);
    CHR(hr);

    // Copy the data
    pwzcnlShared->cNetworks = DSA_GetItemCount(hdsa);
    if (pwzcnlShared->cNetworks <= (cbState - cbBase) / sizeof(SHWZCNET))
    {
        memcpy(pwzcnlShared->wzcnetArray, DSA_GetItemPtr(hdsa, 0),
            sizeof(SHWZCNET) * pwzcnlShared->cNetworks);
    }

Error:
    if (NULL != hdsa)
    {
        DSA_Destroy(hdsa);
    }

    FreeSharedMemory(pwzcnlShared);
    return hr;
}

static HRESULT GetGlobalFlags(HWND hwndSink, DWORD *pdwFlags)
{
    HRESULT hr = S_OK;
    DWORD   dwFlags;

    // Check inputs
    CBRAEx(NULL != hwndSink, E_INVALIDARG);
    CBREx(NULL != pdwFlags, E_INVALIDARG);

    // Send a message to the sink window
    dwFlags = (DWORD)SendMessage(hwndSink, SHWZCM_GETSTATE, 0, 0);
    CBREx(0 != dwFlags, SHWZC_E_HARDWARE);

    // Success
    *pdwFlags = (dwFlags-1);

Error:
    return hr;
}

LRESULT SHWZCGetState_I(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = S_OK;
    DWORD   dwState = 0;

    // Send a private message to the UI threads
    hr = CWZCManager::SendAllPrivateMessage(PM_GETSTATE, (WPARAM)&dwState, 0);
    CHR(hr);

Error:
    return SUCCEEDED(hr) ? (dwState+1) : 0;
}

static HRESULT SetGlobalFlags(HWND hwndSink, DWORD dwFlags)
{
    HRESULT hr = S_OK;

    // Check inputs
    CBRAEx(NULL != hwndSink, E_INVALIDARG);

    // Send a message to the sink window
    CLR(SendMessage(hwndSink, SHWZCM_SETSTATE, dwFlags, 0));

Error:
    return hr;
}

LRESULT SHWZCSetState_I(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = S_OK;

    // Send a private message to the UI threads
    hr = CWZCManager::SendAllPrivateMessage(PM_SETSTATE, wParam, 0);
    CHR(hr);

Error:
    return HR2LR(hr);
}

static HRESULT RegisterCallback(HWND hwndSink, HWND hwndCallback, UINT uCallbackMsg,
                                UINT uScanInterval, DWORD dwFlags)
{
    typedef CWZCManagerWireless::CBINFO CBINFO;

    HRESULT hr = S_OK;
    HKEY    hkeyCPL = NULL;
    HKEY    hkeyCB = NULL;
    TCHAR   szName[64];
    CBINFO  cbi;

    // Check inputs
    CBRAEx(NULL != hwndCallback, E_INVALIDARG);
    CBRAEx(IsWindow(hwndCallback), E_INVALIDARG);

    // Create the base key
    CBR(ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, g_szRegKeyBase, 0, NULL,
        REG_OPTION_NON_VOLATILE, 0, NULL, &hkeyCPL, NULL));

    // Create the callback key
    CBR(ERROR_SUCCESS == RegCreateKeyEx(hkeyCPL, g_szRegKeyCallbacks, 0, NULL,
        REG_OPTION_VOLATILE, 0, NULL, &hkeyCB, NULL));

    // Create a value name from the window handle
    hr = StringCchPrintf(szName, ARRAYSIZE(szName), g_szValueFormat, hwndCallback);
    CHR(hr);

    // Fill in the data
    cbi.uCallbackMsg = uCallbackMsg;
    cbi.uScanInterval = uScanInterval;
    cbi.dwFlags = dwFlags;

    // Save the value
    CBR(ERROR_SUCCESS == RegSetValueEx(hkeyCB, szName, 0, REG_BINARY,
        (LPBYTE)&cbi, sizeof(CBINFO)));

    if (NULL != hwndSink)
    {
        // Send a message to the sink window
        hr = (HRESULT)SendMessage(hwndSink, SHWZCM_REGISTERCB, 0, (LPARAM)hwndCallback);
        CHR(hr);
    }

Error:
    if (NULL != hkeyCB)
    {
        RegCloseKey(hkeyCB);
    }

    if (NULL != hkeyCPL)
    {
        RegCloseKey(hkeyCPL);
    }

    return hr;
}

HRESULT SHWZCRegisterCallbackWindow_I(WPARAM wParam, LPARAM lParam)
{
    // Send a private message to each sink window
    return CWZCManager::SendAllPrivateMessage(PM_REGISTERCB, wParam, lParam);
}

HRESULT UnregisterCallback(HWND hwndCallback)
{
    HRESULT hr = S_OK;
    HKEY    hkey = NULL;
    TCHAR   szName[64];
    HWND    hwndSink;

    // Open the registry key
    CBREx(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        g_szRegPathCallbacks, 0, 0, &hkey), S_FALSE);

    // Create the value name
    hr = StringCchPrintf(szName, ARRAYSIZE(szName), g_szValueFormat, hwndCallback);
    CHR(hr);

    // Remove the value
    CBR(ERROR_SUCCESS == RegDeleteValue(hkey, szName));

    // Find a sink window
    hwndSink = SHWZCFindWindow();
    CBREx(NULL != hwndSink, S_FALSE);  // there are no WiFi cards installed

    // Post a message to the sink window
    PostMessage(hwndSink, SHWZCM_UNREGISTERCB, 0, (LPARAM)hwndCallback);

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

HRESULT SHWZCUnregisterCallbackWindow_I(WPARAM wParam, LPARAM lParam)
{
    // Send a private message to each sink window
    return CWZCManager::SendAllPrivateMessage(PM_UNREGISTERCB, wParam, lParam);
}

static HRESULT SetNetworks(HWND hwndSink, __in const SETNETWORKHDR *psnh, __in const SHWZCNETEX *pnetx)
{
    HRESULT        hr = S_OK;
    UINT           i;
    UINT           cbEapDataTotal = 0;
    UINT           cbNetData;
    LPVOID         pvNetData = NULL;
    SETNETWORKHDR  *psnhOut;
    SETNETWORKITEM *psniOut;
    BYTE           *pbEapDataOut;

    // Calculate the size of the EAP data
    for (i = 0; i != psnh->cNetworks; ++i)
    {
        if ((UINT)-1 != pnetx[i].sni.cbEapData)
        {
            CBRA(UAdd (cbEapDataTotal , pnetx[i].sni.cbEapData, &cbEapDataTotal));
        }
    }

    // Allocate the network info buffer
    CBRA(UMul (sizeof (SETNETWORKITEM), psnh->cNetworks, &cbNetData));
    CBRA(UAdd (sizeof(SETNETWORKHDR), cbNetData, &cbNetData));
    CBRA(UAdd (cbEapDataTotal, cbNetData, &cbNetData)); 

    pvNetData = LocalAlloc(LMEM_FIXED, cbNetData);
    CPR(pvNetData);

    // Fill in the header
    psnhOut = (SETNETWORKHDR*)pvNetData;
    psniOut = (SETNETWORKITEM*)(psnhOut + 1);
    pbEapDataOut = (BYTE*)(psniOut + psnh->cNetworks);

    *psnhOut = *psnh;

    // Copy the networks
    for (i = 0; i < psnh->cNetworks; ++i)
    {
        psniOut[i] = pnetx[i].sni;
        if (NULL != pnetx[i].pvEapData)
        {
            memcpy(pbEapDataOut, pnetx[i].pvEapData, pnetx[i].sni.cbEapData);
            pbEapDataOut += pnetx[i].sni.cbEapData;
        }
    }

    // Send a message to the sink window
    hr = SendDataMessageEx(hwndSink, SHWZCMSG_DATA_SETNETWORKS, pvNetData, cbNetData);
    CHR(hr);

Error:
    LocalFree(pvNetData);
    return hr;
}

HRESULT SetNetworks_I(__in_bcount(cbData) LPCVOID pvData, DWORD cbData)
{
    HRESULT hr = S_OK;

    UNREFERENCED_PARAMETER(cbData);

    // Set the network information
    hr = CWZCManager::SendAllPrivateMessage(PM_SETNETWORKS, 0, (LPARAM)pvData);
    CHR(hr);

Error:
    return hr;
}

static void InitExtraNetworkData(SHWZCNETEX *pnetx)
{
    pnetx->sni.wzcnet.dwFlags |= SHWZCF_NET_PREFERRED;
    pnetx->sni.cbEapData = (UINT)-1;
    pnetx->sni.wMask = 0;
    pnetx->sni.szKey[0] = TEXT('\0');
    pnetx->pvEapData = NULL;
    pnetx->iMeta = (UINT)-1;
}

/// <summary>Allocates and assigns a callback struct necessary for using many 
/// of the other WZC Shell functions.</summary>
/// <param name="hwndCallback">[in] Assigned to the hwndCallback field of the resulting struct.</param>
/// <param name="uCallbackMsg">[in] Assigned to the uCallbackMsg field of the resulting struct.</param>
/// <param name="uScanInterval">[in] Assigned to the uScanInterval field of the resulting struct.</param>
/// <param name="dwFlags">[in] Assigned to the dwFlags field of the resulting struct.</param>
///<param name="phshwzc">[out] Pointer to the created callback struct.</param>
extern "C"
HRESULT WINAPI SHWZCCreateCallback(HWND hwndCallback, UINT uCallbackMsg, UINT uScanInterval,
                                   DWORD dwFlags, HSHWZC *phshwzc)
{
    HRESULT hr = S_OK;
    HWND    hwndSink;
    SHWZCCB *pcb = NULL;

    // Create the callback struct
    pcb = (SHWZCCB*)LocalAlloc(LPTR, sizeof(SHWZCCB));
    CPR(pcb);

    pcb->hwndCallback = hwndCallback;
    pcb->dwFlags = SHWZCF_ALLOW_AP;

    // Create the DSA
    pcb->hdsaNetworks = DSA_Create(sizeof(SHWZCNETEX), 4);
    CWR(NULL != pcb->hdsaNetworks);

    // Find a sink window
    hwndSink = SHWZCFindWindow();
    if (NULL != hwndSink)
    {
        UINT i;

        // Query the current preferred networks
        hr = QueryNetworks(hwndSink, pcb->hdsaNetworks, SHWZCF_NET_PREFERRED);
        CHR(hr);

        // Initialize the extra information in each network
        for (i = 0; i != DSA_GetItemCount(pcb->hdsaNetworks); ++i)
        {
            InitExtraNetworkData((SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, i));
        }

        // Get the current global flags
        hr = GetGlobalFlags(hwndSink, &pcb->dwFlags);
        CHR(hr);
    }

    // Register the callback
    hr = RegisterCallback(hwndSink, hwndCallback, uCallbackMsg, uScanInterval, dwFlags);
    CHR(hr);

    // Return the callback struct
    *phshwzc = (HSHWZC)pcb;
    pcb = NULL;

Error:
    if (NULL != pcb)
    {
        VHR(SHWZCDestroyCallback((HSHWZC)pcb));
    }

    return hr;
}

/// <summary>Applies changes in network association / configuration data.</summary>
/// <param name="hshwzc">[in] Callback struct containing the data to be applied.</param>
extern "C"
HRESULT WINAPI SHWZCApplyChanges(HSHWZC hshwzc)
{
    HRESULT       hr = S_OK;
    SHWZCCB       *pcb = (SHWZCCB*)hshwzc;
    HWND          hwndSink;
    UINT          i, cNetworks;
    SHWZCNETEX    *pnetx;
    CNetID        nid;
    BOOL          fUpdateAssociation = FALSE;
    HDSA          hdsa = NULL;
    SETNETWORKHDR snh;

    // Check inputs
    CBREx(NULL != pcb, E_INVALIDARG);

    // Associate destinations for each network
    cNetworks = DSA_GetItemCount(pcb->hdsaNetworks);
    for (i = 0; i != cNetworks; ++i)
    {
        pnetx = (SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, i);
        if ((UINT)-1 != pnetx->iMeta)
        {
            CHRA(nid.Assign(&pnetx->sni.wzcnet));
            hr = nid.Associate(pnetx->iMeta);
            CHR(hr);
            fUpdateAssociation = TRUE;
        }
    }

    // If we changed any associations, we need the adapters to check their
    // current connections
    if (fUpdateAssociation)
    {
        PostEvent(SHWZCE_UPDATE_ASSOCIATION, 0);
    }

    // Create a DSA to hold the changed network configurations
    hdsa = DSA_Create(sizeof(SHWZCNETEX), 4);
    CWR(NULL != hdsa);
    CBREx(DSA_Grow(hdsa, cNetworks), E_OUTOFMEMORY);

    // Build the list
    for (i = 0; i != cNetworks; ++i)
    {
        pnetx = (SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, i);
        if (0 != pnetx->sni.wMask)
        {
            // Add this configuration to our updated list (already allocated
            // memory, so this should never fail)
            CBRAEx(0 <= DSA_AppendItem(hdsa, pnetx), E_OUTOFMEMORY);
        }
    }

    // Do we have any configurations to apply?
    CBREx(0 != DSA_GetItemCount(hdsa) ||
        0 != (SHWZCF_CB_SETSTATE & pcb->dwFlags), S_FALSE);

    // Find a sink window
    hwndSink = SHWZCFindWindow();
    CBREx(NULL != hwndSink, SHWZC_E_HARDWARE);

    // Fill in a network information header
    snh.fSetState = BOOLIFY(SHWZCF_CB_SETSTATE & pcb->dwFlags);
    snh.fComplete = FALSE;
    snh.dwState = (SHWZCF_CB_MASK & pcb->dwFlags);
    snh.cNetworks = DSA_GetItemCount(hdsa);

    // Set the network information
    hr = SetNetworks(hwndSink, &snh, (SHWZCNETEX*)DSA_GetItemPtr(hdsa, 0));
    CHR(hr);

Error:
    if (NULL != hdsa)
    {
        DSA_Destroy(hdsa);
    }

    return hr;
}

static int CALLBACK EnumDestroyNetList(LPVOID pvItem, LPVOID pvParam)
{
    SHWZCNETEX *pnetx = (SHWZCNETEX*)pvItem;
    LocalFree(pnetx->pvEapData);
    return TRUE;
}

extern "C"
HRESULT WINAPI SHWZCDestroyCallback(HSHWZC hshwzc)
{
    HRESULT hr = S_OK;
    SHWZCCB *pcb = (SHWZCCB*)hshwzc;

    // Check inputs
    CBREx(NULL != pcb, E_INVALIDARG);

    // Remove the callback from the registry
    VHR(UnregisterCallback(pcb->hwndCallback));

    // Clean up the DSA
    if (NULL != pcb->hdsaNetworks)
    {
        DSA_DestroyCallback(pcb->hdsaNetworks, EnumDestroyNetList, NULL);
    }

    // Clean up the callback struct
    LocalFree(pcb);

Error:
    return hr;
}

/// <summary>Posts a global SHWZCE_RESCAN event, causing a foreground scan.</summary>
extern "C"
HRESULT WINAPI SHWZCRescanNetworks()
{
    // Post a global event
    return PostEvent(SHWZCE_RESCAN, 0);
}

/// <summary>Retrieves the current network filter status. The filter status determines which types of 
/// WiFi networks may be accessed. Examples are all networks, adhoc only, or infrastructure only.</summary>
/// <param name="hshwzc">[in] The callback in which to retrieve the network filter.  If hshwzc is NULL, then the network filter will
/// always be set to SHWZCF_ALLOW_AP, which allows all networks.</param>
/// <param name="pdwState">[out] Set to the resulting network filter.</param>
extern "C"
HRESULT WINAPI SHWZCGetState(HSHWZC hshwzc, DWORD *pdwState)
{
    HRESULT hr = S_OK;
    SHWZCCB *pcb = (SHWZCCB*)hshwzc;
    HWND    hwndSink;

    // Check inputs
    CBREx(NULL != pdwState, E_INVALIDARG);

    // If we have a callback, use it
    if (NULL != pcb)
    {
        *pdwState = (SHWZCF_CB_MASK & pcb->dwFlags);
    }
    else
    {
        // Default value
        *pdwState = SHWZCF_ALLOW_AP;

        // Find a sink window
        hwndSink = SHWZCFindWindow();
        CBREx(NULL != hwndSink, S_FALSE);

        // Get the global flags
        hr = GetGlobalFlags(hwndSink, pdwState);
        CHR(hr);
    }

Error:
    return hr;
}

/// <summary>Sets the current network filter status. The filter status determines which types of 
/// WiFi networks may be accessed. Examples are all networks, adhoc only, or infrastructure only.</summary>
/// <param name="hshwzc">[in] The callback in which to set the network filter.  If hshwzc is NULL, then the input network filter will
/// be set as a global flag. </param>
/// <param name="dwState">[in] The desired network filter.</param>
extern "C"
HRESULT WINAPI SHWZCSetState(HSHWZC hshwzc, DWORD dwState)
{
    HRESULT hr = S_OK;
    SHWZCCB *pcb = (SHWZCCB*)hshwzc;
    HWND    hwndSink;

    // If we have a callback, use it
    if (NULL != pcb)
    {
        pcb->dwFlags = dwState | SHWZCF_CB_SETSTATE;
    }
    else
    {
        // Find a sink window
        hwndSink = SHWZCFindWindow();
        CBR(NULL != hwndSink);

        // Set the global flags
        hr = SetGlobalFlags(hwndSink, dwState);
        CHR(hr);
    }

Error:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCGetAllNetworks(HSHWZC hshwzc, HDSA *phdsaNetworks)
{
    HRESULT hr = S_OK;
    SHWZCCB *pcb = (SHWZCCB*)hshwzc;
    HDSA    hdsa = NULL;
    BOOL    fDestroy = TRUE;
    DWORD   dwFlags = SHWZCF_NET_BROADCAST | SHWZCF_NET_CONNECTED | SHWZCF_NET_CONNECTING;
    HWND    hwndSink;
    DWORD   dwFilter = SHWZCF_ALLOW_AP;

    // Check inputs
    CBREx(NULL != phdsaNetworks, E_INVALIDARG);

    if (NULL != *phdsaNetworks)
    {
        // Validate the DSA
        CBRAEx(sizeof(SHWZCNET) == (*phdsaNetworks)->cbItem, E_INVALIDARG);

        // Erase all the items in the list
        (*phdsaNetworks)->cItem = 0;
        hdsa = *phdsaNetworks;
        fDestroy = FALSE;
    }
    else
    {
        hdsa = DSA_Create(sizeof(SHWZCNET), 4);
        CWR(NULL != hdsa);
    }

    // If we don't have a callback, we need to enumerate preferred networks
    if (NULL == pcb)
    {
        dwFlags |= SHWZCF_NET_PREFERRED;
    }
    else
    {
        UINT cItems = DSA_GetItemCount(pcb->hdsaNetworks);
        UINT i;

        // Copy the preferred networks from the callback state
        CBREx(DSA_Grow(hdsa, cItems), E_OUTOFMEMORY);
        hdsa->cItem = cItems;
        for (i = 0; i != cItems; ++i)
        {
            memcpy(DSA_GetItemPtr(hdsa, i), &((SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, i))->sni.wzcnet, sizeof(SHWZCNET));
        }
    }

    // Find a sink window
    hwndSink = SHWZCFindWindow();
    if (NULL != hwndSink)
    {
        // Query the airing networks
        hr = QueryNetworks(hwndSink, hdsa, dwFlags);
        CHR(hr);
    }

    // Get the current network filter
    if (NULL != pcb)
    {
        dwFilter = (SHWZCF_ALLOW_MASK & pcb->dwFlags);
    }
    else if (NULL != hwndSink)
    {
        hr = GetGlobalFlags(hwndSink, &dwFilter);
        CHR(hr);
        dwFilter &= SHWZCF_ALLOW_MASK;
    }

    // Filter the networks
    if (dwFilter != SHWZCF_ALLOW_ALL)
    {
        DWORD dwMask = (SHWZCF_ALLOW_ADHOC == dwFilter ? SHWZCF_NET_ADHOC : 0);
        DWORD i = 0;

        while (i != DSA_GetItemCount(hdsa))
        {
            if ((SHWZCF_NET_ADHOC & ((PSHWZCNET)DSA_GetItemPtr(hdsa, i))->dwFlags) != dwMask)
            {
                // Delete the item (doesn't match the filter)
                DSA_DeleteItem(hdsa, i);
            }
            else
            {
                // Skip the item (matches the filter)
                ++i;
            }
        }
    }

    // Success
    *phdsaNetworks = hdsa;
    hdsa = NULL;

Error:
    if (NULL != hdsa)
    {
        if (fDestroy)
        {
            DSA_Destroy(hdsa);
        }
        else
        {
            DSA_DeleteAllItems(hdsa);
        }
    }

    return hr;
}

/// <summary>
/// Determines if we use Dynamic WEP (automatic key provided + 802.1x) by default for new networks 
/// on PPC and Smartphone
/// If the reg key at HKLM\ControlPanel\WiFi\PreferDynamicWEP is set, then the "Key is automatically
/// provided" and "Use IEEE 802.1x network access control" checkboxes are both checked for new networks. 
/// If the key is missing or set to 0, both checkboxes are unchecked.
/// </summary>
BOOL SHWZCDynamicWEPPreference()
{
    static DWORD s_dwDynamicWEP = (DWORD)-1;

    if((DWORD)-1 == s_dwDynamicWEP)
    {
        if(FAILED(RegistryGetDWORD(HKEY_LOCAL_MACHINE,  g_szRegKeyBase, TEXT("PreferDynamicWEP"), &s_dwDynamicWEP)))
        {
            s_dwDynamicWEP = 0;
        }
    }
    
    return BOOLIFY(s_dwDynamicWEP);
}

void SHWZCGetDefaults(PSHWZCNET pwzcnet, BOOL fAdhoc)
{
    // Fill in the UI defaults
    pwzcnet->dwFlags = (fAdhoc ? SHWZCF_NET_ADHOC : 0);
    pwzcnet->dwFlags |= (SHWZCDynamicWEPPreference() ? SHWZCF_NET_8021X : SHWZCF_NET_KEYDATA);
    pwzcnet->dwAuthentication = Ndis802_11AuthModeOpen;
    pwzcnet->dwEncryptionType = Ndis802_11Encryption1Enabled;  // WEP
    pwzcnet->dwKeyIndex       = 1;
    pwzcnet->dwEAPType        = SHWZCGetEAPDefaultType();
}

extern "C"
HRESULT WINAPI SHWZCCreateDefaultNetwork(PSHWZCNET pwzcnet)
{
    HRESULT hr = S_OK;

    // Check inputs
    CBREx(NULL != pwzcnet, E_INVALIDARG);

    // Create a default network configuration
    pwzcnet->szName[0] = TEXT('\0');
    SHWZCGetDefaults(pwzcnet, FALSE);

Error:
    return hr;
}

static SHWZCNETEX *FindNetworkCB(const CNetID *pnid, SHWZCCB *pcb)
{
    int nIndex = FindNetwork(pnid, &((SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, 0))->sni.wzcnet,
        DSA_GetItemCount(pcb->hdsaNetworks), sizeof(SHWZCNETEX));
    return (-1 == nIndex ? NULL : (SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, nIndex));
}

extern "C"
HRESULT WINAPI SHWZCGetNetwork(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc, PSHWZCNET pwzcnet,
                               LPVOID *ppvEapData, UINT *pcbEapData)
{
    HRESULT    hr = S_OK;
    SHWZCCB    *pcb = (SHWZCCB*)hshwzc;
    BOOL       fQueryEap = (NULL != ppvEapData && NULL != pcbEapData);
    SHWZCNETEX *pnetx = NULL;
    HWND       hwndSink;
    PSHWZCNET  pwzcnetShared = NULL;
    CNetID     nid;
    DWORD      dwFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_BROADCAST;

    // The caller must be trusted in order to query the EAP data
    CBREx(!fQueryEap || OEM_CERTIFY_TRUST == CeGetCallerTrust(), E_INVALIDARG);

    // Check inputs
    CBREx(NULL != pwzcnet, E_INVALIDARG);
    CBREx(NULL != pszName && TEXT('\0') != pszName[0], E_INVALIDARG);

    // Check the name
    hr = nid.Assign(pszName, fAdhoc);
    CHREx(hr, E_INVALIDARG);

    // Does the network exist in the callback configuration?
    if (NULL != pcb)
    {
        // Since the callback state represents the user's entire preferred list
        // (and they may have deleted some networks), we don't want to search
        // the "live" preferred list if we don't find the network
        dwFlags &= ~SHWZCF_NET_PREFERRED;

        // Look for the network in the callback state
        pnetx = FindNetworkCB(&nid, pcb);
        if (NULL != pnetx)
        {
            if (fQueryEap)
            {
                if ((UINT)-1 == pnetx->sni.cbEapData)
                {
                    // Get the EAP data
                    dwFlags |= SHWZCF_NET_PREFERRED;
                    goto Query;
                }
                else if (0 == pnetx->sni.cbEapData)
                {
                    if (ppvEapData)
                    {
                        *ppvEapData = NULL;
                    }
                }
                else
                {
                    if (ppvEapData)
                    {
                        // Copy the data buffer
                        *ppvEapData = LocalAlloc(LMEM_FIXED, pnetx->sni.cbEapData);
                        CPR(*ppvEapData);
                        memcpy(*ppvEapData, pnetx->pvEapData, pnetx->sni.cbEapData);
                    }
                }

                // Return the size of the data
                if (pcbEapData)
                {
                    *pcbEapData = pnetx->sni.cbEapData;
                }
            }

            // Return the network configuration
            memcpy(pwzcnet, pnetx, sizeof(SHWZCNET));
            hr = S_OK;
            goto Exit;
        }
    }

Query:
    // Find a sink window
    hwndSink = SHWZCFindWindow();
    if (NULL != hwndSink)
    {
        // Allocate shared memory
        hr = GetSharedMemory(sizeof(SHWZCNET) + sizeof(UINT), TRUE, (void**)&pwzcnetShared);
        CHR(hr);

        // Initialize the memory
        hr = nid.GetDisplayName(pwzcnetShared->szName, ARRAYSIZE(pwzcnetShared->szName));
        CHREx(hr, E_INVALIDARG);
        pwzcnetShared->dwFlags = dwFlags | (fAdhoc ? SHWZCF_NET_ADHOC : 0);

        // Send a message to the sink window
        if (SendMessage(hwndSink, SHWZCM_GETNETWORK, 0, 0))
        {
            // Do we need to get the EAP data?
            if (fQueryEap)
            {
                // Get the size of the data
                UINT cbEapData = *((UINT*)(pwzcnetShared + 1));

                if (0 == cbEapData)
                {
                    *ppvEapData = NULL;
                    if (NULL != pnetx)
                    {
                        pnetx->sni.cbEapData = 0;
                    }
                }
                else
                {
                    // Free the shared memory
                    FreeSharedMemory(pwzcnetShared);
                    pwzcnetShared = NULL;

                    // Allocate a new buffer
                    hr = GetSharedMemory(sizeof(SHWZCNET) + cbEapData, TRUE, (void**)&pwzcnetShared);
                    CHR(hr);

                    // Initialize the memory
                    CHRA(nid.GetDisplayName(pwzcnetShared->szName, ARRAYSIZE(pwzcnetShared->szName)));
                    pwzcnetShared->dwFlags = dwFlags | (fAdhoc ? SHWZCF_NET_ADHOC : 0);

                    // Resend the message
                    CBR(SendMessage(hwndSink, SHWZCM_GETNETWORK, cbEapData, 0));

                    // Do we need to cache the EAP data?
                    if (NULL != pnetx)
                    {
                        pnetx->pvEapData = LocalAlloc(LMEM_FIXED, cbEapData);
                        CPR(pnetx->pvEapData);
                        memcpy(pnetx->pvEapData, pwzcnetShared + 1, cbEapData);
                        pnetx->sni.cbEapData = cbEapData;
                    }

                    // Copy the EAP data
                    *ppvEapData = LocalAlloc(LMEM_FIXED, cbEapData);
                    CPR(*ppvEapData);
                    memcpy(*ppvEapData, pwzcnetShared + 1, cbEapData);
                }

                // Return the size of the data
                *pcbEapData = cbEapData;
            }

            // We successfully found the network data
            if (NULL != pnetx)
            {
                memcpy(pwzcnet, pnetx, sizeof(SHWZCNET));
            }
            else
            {
                memcpy(pwzcnet, pwzcnetShared, sizeof(SHWZCNET));
            }

            hr = S_OK;
            goto Exit;
        }
    }

    // We didn't find the network, so initialize defaults
    hr = nid.GetDisplayName(pwzcnet->szName, ARRAYSIZE(pwzcnet->szName));
    CHR(hr);
    SHWZCGetDefaults(pwzcnet, fAdhoc);
    hr = S_FALSE;

    if (fQueryEap)
    {
        *ppvEapData = NULL;
        *pcbEapData = 0;
    }

Error:
Exit:
    FreeSharedMemory(pwzcnetShared);
    return hr;
}

BOOL SHWZCGetNetwork_I(WPARAM wParam, LPARAM lParam)
{
    HRESULT   hr = S_OK;
    PSHWZCNET pwzcnetShared = NULL;
    CNetID    nid;
    HWND      hwndSink;

    // Get the shared memory
    hr = GetSharedMemory(sizeof(SHWZCNET), FALSE, (void**)&pwzcnetShared);
    CHR(hr);

    // Get the network ID
    hr = nid.Assign(pwzcnetShared);
    CHR(hr);

    // Try to find the network
    hwndSink = CWZCManagerWireless::FindOwnerWindow(&nid);
    CBR(NULL != hwndSink);

    // Get the network data
    hr = (HRESULT)SendMessage(hwndSink, SHWZCM_READNETWORK, wParam, (LPARAM)pwzcnetShared);
    CHR(hr);

Error:
    FreeSharedMemory(pwzcnetShared);
    return SUCCEEDED(hr);
}

void SHWZCValidateSettings(PSHWZCNET pwzcnet)
{
    BOOL       fAdhoc;
    const UINT *prguAuthSupport;
    UINT       cAuthSupport;
    UINT       iAuth, iEncrypt;
    DWORD      dwDisplayFlags = 0;

    ASSERT(NULL != pwzcnet);

    // Check the authentication mode based on the adhoc flag
    fAdhoc = BOOLIFY(SHWZCF_NET_ADHOC & pwzcnet->dwFlags);
    prguAuthSupport = fAdhoc ? g_rguAuthAdhocSupport : g_rguAuthInfraSupport;
    cAuthSupport = fAdhoc ? ARRAYSIZE(g_rguAuthAdhocSupport) : ARRAYSIZE(g_rguAuthInfraSupport);

    for (iAuth = 0; iAuth != cAuthSupport; ++iAuth)
    {
        if (g_rgAuthTypes[prguAuthSupport[iAuth]].dwType == pwzcnet->dwAuthentication)
        {
            break;
        }
    }

    // If this authentication mode isn't supported, switch to the default
    if (iAuth == cAuthSupport)
    {
        static const UINT s_rguDefAuth[2] = { 2, 4 };
        iAuth = s_rguDefAuth[fAdhoc];
    }
    else
    {
        iAuth = prguAuthSupport[iAuth];
    }

    // Set the authentication mode
    pwzcnet->dwAuthentication = g_rgAuthTypes[iAuth].dwType;

    // Check the encryption type
    for (iEncrypt = g_rgEncryptSupport[iAuth].iFirst; iEncrypt <= g_rgEncryptSupport[iAuth].iLast; ++iEncrypt)
    {
        if (g_rgEncryptTypes[iEncrypt].dwType == pwzcnet->dwEncryptionType)
        {
            break;
        }
    }

    // If the encryption type isn't supported, switch to the default
    if (iEncrypt > g_rgEncryptSupport[iAuth].iLast)
    {
        iEncrypt = s_rguDefEncrypt[iAuth];
    }

    // We should NEVER hit this assertion! If we did, it means that
    // g_rgEncryptSupport or s_rguDefEncrypt are messed up. We'll still
    // handle this case OK, but you should fix this immediately!
    ASSERT(iEncrypt < ARRAYSIZE(g_rgEncryptTypes));

    // Set the encryption type
    pwzcnet->dwEncryptionType = g_rgEncryptTypes[iEncrypt < ARRAYSIZE(g_rgEncryptTypes) ? iEncrypt : 0].dwType;

    // Get the UI display flags
    VHR(SHWZCGetDisplayFlags(pwzcnet, &dwDisplayFlags));

    // Certain settings require that we have no key
    static const SHWZCF_UI_AUTOKEY_FORCE = SHWZCF_UI_AUTOKEY_DISABLED | SHWZCF_UI_AUTOKEY_SET;

    if ((SHWZCF_UI_KEYDATA_DISABLED & dwDisplayFlags) ||
        SHWZCF_UI_AUTOKEY_FORCE == (SHWZCF_UI_AUTOKEY_FORCE & dwDisplayFlags))
    {
        pwzcnet->dwFlags &= (DWORD)~SHWZCF_NET_KEYDATA;
    }

    // Validate the key index
    if (SHWZCF_UI_KEYINDEX_DISABLED & dwDisplayFlags)
    {
        pwzcnet->dwKeyIndex = 1;
    }

    // Validate the 802.1x setting
    if (SHWZCF_UI_8021X_DISABLED & dwDisplayFlags)
    {
        // WPA-Plain, WPA-PSK, and all WPA2 require 802.1x, all others require it off
        if (Ndis802_11AuthModeWPA == pwzcnet->dwAuthentication ||
            Ndis802_11AuthModeWPAPSK == pwzcnet->dwAuthentication ||
            IsAnyWPA2((NDIS_802_11_AUTHENTICATION_MODE)pwzcnet->dwAuthentication))
        {
            pwzcnet->dwFlags |= SHWZCF_NET_8021X;
        }
        else
        {
            pwzcnet->dwFlags &= (DWORD)~SHWZCF_NET_8021X;
        }
    }

    // Validate the EAP type
    if (SHWZCF_UI_EAPTYPE_DISABLED & dwDisplayFlags)
    {
        if (IsAnyWPAPSK((NDIS_802_11_AUTHENTICATION_MODE)pwzcnet->dwAuthentication))
        {
            // WPA[2]-PSK requires a hard-coded EAP type
            pwzcnet->dwEAPType = DEFAULT_EAP_TYPE;
        }
        else
        {
            // All other types use the registry configured default
            pwzcnet->dwEAPType = SHWZCGetEAPDefaultType();
        }
    }
}

// Helper macros
#define COPYVALUE(n) pwzcnetDest->n = pwzcnetSource->n
#define COPYFLAG(n)  pwzcnetDest->dwFlags = (pwzcnetDest->dwFlags & (DWORD)~n) | (pwzcnetSource->dwFlags & n)

extern "C"
HRESULT WINAPI SHWZCCopySettings(SHWZCNET *pwzcnetDest, const SHWZCNET *pwzcnetSource, DWORD dwMask)
{
    HRESULT hr = S_OK;

    // Check inputs
    CBREx(NULL != pwzcnetDest, E_INVALIDARG);
    CBREx(NULL != pwzcnetSource, E_INVALIDARG);

    // Copy over the settings
    if (SHWZCF_SETMASK_ADHOC & dwMask)
    {
        COPYFLAG(SHWZCF_NET_ADHOC);
    }

    if (SHWZCF_SETMASK_REQUIRE_WZC_PROBE & dwMask)
    {
        COPYFLAG(SHWZCF_NET_REQUIRE_WZC_PROBE);
    }

    if (SHWZCF_SETMASK_KEYDATA & dwMask)
    {
        COPYFLAG(SHWZCF_NET_KEYDATA);
    }

    if (SHWZCF_SETMASK_AUTHENTICATION & dwMask)
    {
        COPYVALUE(dwAuthentication);
    }

    if (SHWZCF_SETMASK_ENCRYPTION & dwMask)
    {
        COPYVALUE(dwEncryptionType);
    }

    if (SHWZCF_SETMASK_KEYINDEX & dwMask)
    {
        COPYVALUE(dwKeyIndex);
    }

    if (SHWZCF_SETMASK_8021X & dwMask)
    {
        COPYFLAG(SHWZCF_NET_8021X);
        COPYVALUE(dwEAPType);
    }

    // Validate the settings
    SHWZCValidateSettings(pwzcnetDest);

Error:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCSetNetwork(HSHWZC hshwzc, const SHWZCNET *pwzcnet, LPCWSTR pszNetworkKey,
                               LPCVOID pvEapData, UINT cbEapData, DWORD dwMask)
{
    HRESULT    hr = S_OK;
    SHWZCCB    *pcb = (SHWZCCB*)hshwzc;
    SHWZCNETEX netx;
    LPVOID     pvEapDataCopy = NULL;

    ZeroMemory(&netx, sizeof(SHWZCNETEX));

    // Check inputs
    CBREx(NULL != pwzcnet, E_INVALIDARG);

    if (!(SHWZCF_SETMASK_8021X & dwMask) ||
        !(SHWZCF_NET_8021X & pwzcnet->dwFlags))
    {
        // We don't use the EAP data even if it's provided
        pvEapData = NULL;
    }

    // Sanity check
    if (NULL == pvEapData)
    {
        cbEapData = 0;
    }

    // We only accept EAP data from trusted processes
    CBREx(0 == cbEapData || OEM_CERTIFY_TRUST == CeGetCallerTrust(), E_INVALIDARG);

    // If we have callback state, use it
    if (NULL != pcb)
    {
        CNetID nid;

        // Get the network ID
        hr = nid.Assign(pwzcnet);
        CHR(hr);

        // Find the network in the current list
        int nIndex = FindNetwork(&nid, &((SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, 0))->sni.wzcnet,
            DSA_GetItemCount(pcb->hdsaNetworks), sizeof(SHWZCNETEX));
        if (0 <= nIndex)
        {
            // Copy the old configuration and then remove it from the list
            memcpy(&netx, DSA_GetItemPtr(pcb->hdsaNetworks, nIndex), sizeof(SHWZCNETEX));
            DSA_DeleteItem(pcb->hdsaNetworks, nIndex);

            // Copy the updated settings
            hr = SHWZCCopySettings(&netx.sni.wzcnet, pwzcnet, dwMask);
            CHR(hr);
        }
        else
        {
            // Allocate space in the list for another network
            CBREx(DSA_Grow(pcb->hdsaNetworks, DSA_GetItemCount(pcb->hdsaNetworks) + 1), E_OUTOFMEMORY);

            // Use the provided data as the base configuration
            memcpy(&netx.sni.wzcnet, pwzcnet, sizeof(SHWZCNET));
            InitExtraNetworkData(&netx);
        }

        // Copy the network key
        if (SHWZCF_SETMASK_KEYDATA & dwMask)
        {
            hr = StringCchCopyEx(netx.sni.szKey, ARRAYSIZE(netx.sni.szKey), pszNetworkKey,
                NULL, NULL, STRSAFE_IGNORE_NULLS);
            CHREx(hr, E_INVALIDARG);
        }

        // Copy the EAP data
        if (SHWZCF_SETMASK_8021X & dwMask)
        {
            LocalFree(netx.pvEapData);
            netx.pvEapData = NULL;
            netx.sni.cbEapData = cbEapData;

            if (0 != cbEapData)
            {
                pvEapDataCopy = LocalAlloc(LMEM_FIXED, cbEapData);
                CPR(pvEapDataCopy);
                memcpy(pvEapDataCopy, pvEapData, cbEapData);
                netx.pvEapData = pvEapDataCopy;
            }
        }

        // Set the update mask
        netx.sni.wMask |= dwMask;

        // Insert the network at the front of the preferred list (we already
        // allocated memory, so this should never fail)
        CBRAEx(0 <= DSA_InsertItem(pcb->hdsaNetworks, 0, &netx), E_OUTOFMEMORY);
        pvEapDataCopy = NULL;
    }
    else
    {
        HWND          hwndSink;
        SETNETWORKHDR snh;

        // Find a sink window
        hwndSink = SHWZCFindWindow();
        CBR(NULL != hwndSink);

        // Fill in the network data header
        snh.fSetState = FALSE;
        snh.fComplete = FALSE;
        snh.dwState = 0;
        snh.cNetworks = 1;

        // Create the network description
        netx.sni.wzcnet = *pwzcnet;
        netx.sni.cbEapData = cbEapData;
        netx.sni.wMask = (WORD)dwMask;
        netx.pvEapData = cbEapData ? (LPVOID)pvEapData : NULL;

        hr = StringCchCopyEx(netx.sni.szKey, ARRAYSIZE(netx.sni.szKey), pszNetworkKey,
            NULL, NULL, STRSAFE_IGNORE_NULLS);
        CHREx(hr, E_INVALIDARG);

        // Set the network
        hr = SetNetworks(hwndSink, &snh, &netx);
        CHR(hr);
    }

Error:
    LocalFree(pvEapDataCopy);
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCConnectNetwork(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc)
{
    HRESULT       hr = S_OK;
    SHWZCCB       *pcb = (SHWZCCB*)hshwzc;
    CNetID        nid;
    int           nIndex;
    HWND          hwndSink;
    SETNETWORKHDR snh;
    SHWZCNETEX    *pnetx;

    // Check inputs
    CBREx(NULL != pcb, E_INVALIDARG);

    // Get the network ID
    hr = nid.Assign(pszName, fAdhoc);
    CHR(hr);

    // Find the network in the current list
    nIndex = FindNetwork(&nid, &((SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, 0))->sni.wzcnet,
        DSA_GetItemCount(pcb->hdsaNetworks), sizeof(SHWZCNETEX));
    CBREx(0 <= nIndex, E_INVALIDARG);

    // Associate the network
    pnetx = (SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, nIndex);
    if ((UINT)-1 != pnetx->iMeta)
    {
        hr = nid.Associate(pnetx->iMeta);
        CHR(hr);
        pnetx->iMeta = (UINT)-1;
    }

    // Find a sink window
    hwndSink = SHWZCFindWindow();
    CBREx(NULL != hwndSink, SHWZC_E_HARDWARE);

    // Fill in the network data header
    snh.fSetState = BOOLIFY(SHWZCF_CB_SETSTATE & pcb->dwFlags);
    snh.fComplete = FALSE;
    snh.dwState = (SHWZCF_CB_MASK & pcb->dwFlags);
    snh.cNetworks = 1;

    // Connect the network
    hr = SetNetworks(hwndSink, &snh, pnetx);
    CHR(hr);

    // Reset the modification flags
    pnetx->sni.wMask = 0;
    pcb->dwFlags &= ~SHWZCF_CB_SETSTATE;

Error:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCDeleteNetwork(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc)
{
    HRESULT hr = S_OK;
    SHWZCCB *pcb = (SHWZCCB*)hshwzc;
    CNetID  nid;

    // Check inputs
    CBREx(NULL != pszName && TEXT('\0') != pszName[0], E_INVALIDARG);

    // Assign the network ID
    hr = nid.Assign(pszName, fAdhoc);
    CHR(hr);

    // Remove the metanetwork association
    hr = nid.ClearAssociation();
    CHR(hr);

    // Remove the network from the Wi-Fi hardware
    hr = SendDataMessage(SHWZCMSG_DATA_DELETENETWORK, &nid, sizeof(CNetID));
    CHR(hr);

    // If we have callback state, remove the network from there as well
    if (NULL != pcb)
    {
        int nIndex;

        // Find the network in the current list
        nIndex = FindNetwork(&nid, &((SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, 0))->sni.wzcnet,
            DSA_GetItemCount(pcb->hdsaNetworks), sizeof(SHWZCNETEX));
        CBREx(0 <= nIndex, S_FALSE);

        // Remove it
        EnumDestroyNetList(DSA_GetItemPtr(pcb->hdsaNetworks, nIndex), NULL);
        CBRA(DSA_DeleteItem(pcb->hdsaNetworks, nIndex));
    }

Error:
    return hr;
}

HRESULT DeleteNetwork_I(__in_bcount(cbData) LPCVOID pvData, DWORD cbData)
{
    HRESULT      hr = S_OK;
    const CNetID *pnid;
    HWND         hwndSink;

    // Check inputs
    CBRAEx(sizeof(CNetID) == cbData, E_INVALIDARG);
    pnid = (const CNetID*)pvData;

    // Try to find the network
    hwndSink = CWZCManagerWireless::FindOwnerWindow(pnid);
    CBR(NULL != hwndSink);

    // Delete the network
    hr = (HRESULT)SendMessage(hwndSink, SHWZCM_CLEARNETWORK, 0, (LPARAM)pnid);
    CHR(hr);

Error:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCDeleteAllNetworks(HSHWZC hshwzc)
{
    HRESULT hr = S_OK;
    SHWZCCB *pcb = (SHWZCCB*)hshwzc;

    if (NULL != pcb)
    {
        DSA_EnumCallback(pcb->hdsaNetworks, EnumDestroyNetList, NULL);
        CBRA(DSA_DeleteAllItems(pcb->hdsaNetworks));
    }
    else
    {
        hr = PostEvent(SHWZCE_DELETEALL, 0);
        CHR(hr);
    }

Error:
    return hr;
}

/// <summary>Computes network configuration settings for pwzcnet and stores the result in pdwFlags.  Flags
/// consist of SHWZCF_NET_ADHOC for adhoc networks, SHWZCF_NET_KEYDATA if a network key exists,
/// SHWZCF_UI_AUTOKEY_SET if the key is automatically provided, SHWZCF_UI_AUTOKEY_DISABLED if
/// the automatic key setting is not allowed, SHWZCF_UI_8021X_DISABLED if 8021x authentication is not allowed,
/// SHWZCF_UI_KEYDATA_DISABLED if network keys are not allowed, SHWZCF_UI_KEYINDEX_DISABLED if
/// specifying a key index is not allowed, and SHWZCF_UI_EAPTYPE_DISABLED if EAP type selection is not allowed.</summary>
/// <remarks>This function is a convenient way to map pwzcnet objects to a simple set of flags that would be relevent to
/// WiFi configuration UI.</remarks>
/// <param name="pwzcnet">[in] Pointer to a network configuration from which to query for state flags.</param>
/// <param name="pdwFlags">[out] Pointer to a DWORD that will receive the state flags.</param>
extern "C"
HRESULT WINAPI SHWZCGetDisplayFlags(const SHWZCNET *pwzcnet, DWORD *pdwFlags)
{
    HRESULT hr = S_OK;
    BOOL    fSimpleAuth;
    DWORD   dwFlags = 0;

    // Check inputs
    CBREx(NULL != pwzcnet, E_INVALIDARG);
    CBREx(NULL != pdwFlags, E_INVALIDARG);

    // Simple authentication is Open or Shared access point with encryption
    fSimpleAuth = !(SHWZCF_NET_ADHOC & pwzcnet->dwFlags) &&
        (Ndis802_11AuthModeOpen == pwzcnet->dwAuthentication ||
        Ndis802_11AuthModeShared == pwzcnet->dwAuthentication) &&
        Ndis802_11EncryptionDisabled != pwzcnet->dwEncryptionType;

    // If this is a WPA[2]-Plain network, or a simple authentication network 
    // with no key, we treat it as an automatic key.
    if (Ndis802_11AuthModeWPA == pwzcnet->dwAuthentication ||
        Ndis802_11AuthModeWPA2 == pwzcnet->dwAuthentication || 
        (fSimpleAuth && !(SHWZCF_NET_KEYDATA & pwzcnet->dwFlags)))
        {
            dwFlags |= SHWZCF_UI_AUTOKEY_SET;
        }

    // The autokey and 802.1x checkboxes are only enabled for simple
    // authentication networks, since in every other case we already know their
    // values, and they can't be changed.
    if (!fSimpleAuth)
    {
        dwFlags |= SHWZCF_UI_AUTOKEY_DISABLED | SHWZCF_UI_8021X_DISABLED;
    }

    // Normally, the network key input is enabled/disabled to match the state
    // of the autokey checkbox. But in the case of no encryption whatsoever,
    // it's always disabled.
    if (Ndis802_11EncryptionDisabled == pwzcnet->dwEncryptionType)
    {
        dwFlags |= SHWZCF_UI_KEYDATA_DISABLED | SHWZCF_UI_KEYINDEX_DISABLED;
    }

    // Normally, the key index control is enabled/disabled to match the state
    // of the autokey checkbox. But for WPA networks, it's always disabled.
    if (IsAnyWPA((NDIS_802_11_AUTHENTICATION_MODE)pwzcnet->dwAuthentication))
    {
        dwFlags |= SHWZCF_UI_KEYINDEX_DISABLED;
    }

    // Normally, the EAP type selection control is enabled or disabled based on
    // whether 802.1x is enabled, but for WPA[2]-PSK, it's always disabled, since
    // the type doesn't matter (we always use EAP-TLS).
    if (IsAnyWPAPSK((NDIS_802_11_AUTHENTICATION_MODE)pwzcnet->dwAuthentication) ||
        (SHWZCF_NET_ADHOC & pwzcnet->dwFlags))
    {
        dwFlags |= SHWZCF_UI_EAPTYPE_DISABLED;
    }

    // Success
    *pdwFlags = dwFlags;

Error:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCGetDestinationGUID(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc, LPTSTR pszGUID, UINT cchGUID)
{
    HRESULT hr = S_OK;

    // Check inputs
    CBREx(NULL != pszGUID, E_INVALIDARG);
    CBREx(0 != cchGUID, E_INVALIDARG);

    // If pszName is NULL, it means check the "wired" destination GUID
    if (NULL == pszName && FALSE == fAdhoc)
    {
        hr = GetNetworkDestGUID(NULL, FALSE, pszGUID, cchGUID);
    }
    else
    {
        SHWZCCB *pcb = (SHWZCCB*)hshwzc;
        CNetID  nid;

        // Convert to a CNetID
        hr = nid.Assign(pszName, fAdhoc);
        if(SUCCEEDED(hr))
        {
            // If we have callback state, use it
            if (NULL != pcb)
            {
                SHWZCNETEX *pnetx = FindNetworkCB(&nid, pcb);
                if (NULL != pnetx &&
                    (UINT)-1 != pnetx->iMeta)
                {
                    hr = StringCchCopy(pszGUID, cchGUID, GetMetaNetworkGUIDString(pnetx->iMeta));
                    CHR(hr);
                    goto Exit;
                }
            }

            // Get the associated GUID directly
            hr = nid.GetAssociation(pszGUID, cchGUID);
        }
    }

    if (FAILED(hr))
    {
        // If we failed to get an association, use the default
        hr = StringCchCopy(pszGUID, cchGUID, GetMetaNetworkGUIDString(0));
        CHR(hr);
        hr = S_FALSE;
    }

Error:
Exit:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCSetDestinationGUID(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc, LPCWSTR pszGUID)
{
    HRESULT hr = S_OK;
    UINT    iMetaNetwork;

    // Check inputs
    CBREx(NULL != pszGUID, E_INVALIDARG);

    // Get the metanetwork index
    hr = GetMetaNetworkIndex(pszGUID, &iMetaNetwork);
    CHREx(hr, E_INVALIDARG);

    // If pszName is NULL, it means set the "wired" destination GUID
    if (NULL == pszName && FALSE == fAdhoc)
    {
        hr = SetNetworkDestGUID(NULL, FALSE, pszGUID);
        CHR(hr);
    }
    else
    {
        SHWZCCB *pcb = (SHWZCCB*)hshwzc;
        CNetID  nid;

        // Get the network ID
        hr = nid.Assign(pszName, fAdhoc);
        CHR(hr);

        // If we have callback state, use it
        if (NULL != pcb)
        {
            SHWZCNETEX *pnetx = FindNetworkCB(&nid, pcb);
            if (NULL == pnetx)
            {
                SHWZCNETEX netx;

                CHRA(StringCchCopy(netx.sni.wzcnet.szName, ARRAYSIZE(netx.sni.wzcnet.szName), pszName));
                SHWZCGetDefaults(&netx.sni.wzcnet, fAdhoc);
                InitExtraNetworkData(&netx);

                // Append this network to the end of the list
                int nIndex = DSA_AppendItem(pcb->hdsaNetworks, &netx);
                CBREx(0 <= nIndex, E_OUTOFMEMORY);
                pnetx = (SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, nIndex);
            }

            // Associate the GUID
            pnetx->iMeta = iMetaNetwork;
        }
        else
        {
            // Set the associated GUID
            hr = nid.Associate(iMetaNetwork);
            CHR(hr);

            // Post an update event
            PostEvent(SHWZCE_UPDATE_ASSOCIATION, 0);
        }
    }

Error:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCEnumDestinations(PFN_WZCENUMDESTID pfnCallback, LPVOID pvParam)
{
    HRESULT hr = S_OK;
    UINT    i, cNetworks;
    LPCTSTR pszName;

    for (i = 0, cNetworks = GetMetaNetworkCount(); i != cNetworks; ++i)
    {
        // Get the name of the network
        hr = GetMetaNetworkName(i, &pszName);
        CHR(hr);

        // Execute the callback
        CBREx(pfnCallback(GetMetaNetworkGUIDString(i), pszName, pvParam), S_FALSE);
    }

Error:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCEnumAuthenticationTypes(BOOL fAdhoc,
                                            PFN_WZCENUMTYPE pfnCallback, LPVOID pvParam)
{
    HRESULT    hr = S_OK;
    BOOL       fSupportWPA;
    BOOL       fSupportWPA2;
    UINT       i;
    const UINT *prguSupport;
    UINT       cSupport;
    LPCTSTR    pszName;

    // Check inputs
    CBREx(NULL != pfnCallback, E_INVALIDARG);

    // Do we support WPA/WPA2?
    fSupportWPA = BOOLIFY(SHWZCF_HW_SUPPORT_WPA & SHWZCGetHardwareFlags());
    fSupportWPA2 = BOOLIFY(SHWZCF_HW_SUPPORT_WPA2 & SHWZCGetHardwareFlags());

    // Get the supported types
    prguSupport = fAdhoc ? g_rguAuthAdhocSupport : g_rguAuthInfraSupport;
    cSupport = fAdhoc ? ARRAYSIZE(g_rguAuthAdhocSupport) : ARRAYSIZE(g_rguAuthInfraSupport);

    for (i = 0; i != cSupport; ++i)
    {
        // Don't enumerate unsupported authentication types
        if (!fSupportWPA &&
            IsAnyWPA((NDIS_802_11_AUTHENTICATION_MODE)g_rgAuthTypes[prguSupport[i]].dwType))
        {
            continue;
        }
        if (!fSupportWPA2 &&
            IsAnyWPA2((NDIS_802_11_AUTHENTICATION_MODE)g_rgAuthTypes[prguSupport[i]].dwType))
        {
            continue;
        }

        // Load the name of the authentication type
        pszName = (LPCTSTR)LoadString(HINST_RESDLL, g_rgAuthTypes[prguSupport[i]].uStringID, NULL, 0);
        CBRA(NULL != pszName);

        // Execute the callback
        CBREx(pfnCallback(g_rgAuthTypes[prguSupport[i]].dwType, pszName, pvParam), S_FALSE);
    }

Error:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCEnumEncryptionTypes(const SHWZCNET *pwzcnet,
                                        PFN_WZCENUMTYPE pfnCallback, LPVOID pvParam)
{
    HRESULT         hr = S_OK;
    const _TYPEINFO *pti;
    const _TYPEINFO *ptiEnd;
    LPCTSTR         pszName;

    BOOL fAESCapable = (SHWZCF_HW_SUPPORT_WPA_AES & SHWZCGetHardwareFlags());

    // Check inputs
    CBREx(NULL != pfnCallback, E_INVALIDARG);

    // Determine which encryption types to enumerate
    if (NULL == pwzcnet)
    {
        pti = g_rgEncryptTypes;

        if (fAESCapable)
        {
            ptiEnd = pti + ARRAYSIZE(g_rgEncryptTypes);
        }
        else
        {
            // Remove AES which is the last array element
            ptiEnd = pti + ARRAYSIZE(g_rgEncryptTypes) - 1;
        }
    }
    else
    {
        // Find the current authentication mode
        UINT i = FindType(pwzcnet->dwAuthentication, g_rgAuthTypes, ARRAYSIZE(g_rgAuthTypes));
        CBREx(i < ARRAYSIZE(g_rgAuthTypes), E_INVALIDARG);

        // Set the encryption types to enumerate
        pti = g_rgEncryptTypes + g_rgEncryptSupport[i].iFirst;
        ptiEnd = g_rgEncryptTypes + (g_rgEncryptSupport[i].iLast+1);

        // For WPA, check if AES is supported
        if (IsAnyWPA((NDIS_802_11_AUTHENTICATION_MODE)pwzcnet->dwAuthentication) &&
            !fAESCapable)
        {
            // AES is not supported.  Modify ptiEnd to not include the last
            // array element which is AES.
            ptiEnd = g_rgEncryptTypes + g_rgEncryptSupport[i].iLast;
        }
    }

    // Enumerate all supported encryptions
    for (; pti != ptiEnd; ++pti)
    {
        // Load the name of the encryption type
        pszName = (LPCTSTR)LoadString(HINST_RESDLL, pti->uStringID, NULL, 0);
        CBRA(NULL != pszName);

        // Execute the callback
        CBREx(pfnCallback(pti->dwType, pszName, pvParam), S_FALSE);
    }

Error:
    return hr;
}

static int CALLBACK EnumCompareEAP(LPVOID pvCompare, LPVOID pvItem, LPARAM lParam)
{
    DWORD dwType1 = *((DWORD*)pvCompare);
    DWORD dwType2 = ((SHWZCEAPEX*)pvItem)->dwType;
    return (dwType1 < dwType2 ? -1 : (dwType1 > dwType2));
}

static int CALLBACK EnumDestroyEAP(LPVOID pvItem, LPVOID pvParam)
{
    // Free the EAP data
    LocalFree(((SHWZCEAPEX*)pvItem)->pvData);
    return TRUE;
}

extern "C"
HRESULT WINAPI SHWZCCreateEAPList(HDSA *phdsaEAP)
{
    HRESULT        hr = S_OK;
    EAP_EXTENSIONS ee = { 0 };
    UINT           i;
    HDSA           hdsa = NULL;
    SHWZCEAPEX     *peapx;

    // Check inputs
    CBREx(NULL != phdsaEAP, E_INVALIDARG);

    // Create the DPA
    hdsa = DSA_Create(sizeof(SHWZCEAPEX), 1);
    CWR(NULL != hdsa);

    // Enumerate all the types the OS knows about
    CBRA(ERROR_SUCCESS == WZCEnumEapExtensions(&ee.dwNumOfExtension, &ee.pEapExtensionInfo));
    CBREx(DSA_Grow(hdsa, ee.dwNumOfExtension), E_OUTOFMEMORY);
    hdsa->cItem = ee.dwNumOfExtension;

    for (i = 0; i != ee.dwNumOfExtension; ++i)
    {
        // Copy the useful data
        peapx = (SHWZCEAPEX*)DSA_GetItemPtr(hdsa, i);
        peapx->dwType = ee.pEapExtensionInfo[i].dwExtensionType;
        CHRA(StringCchCopy(peapx->szName, ARRAYSIZE(peapx->szName), ee.pEapExtensionInfo[i].wszFriendlyName));
        CHRA(StringCchCopy(peapx->szConfigUIPath, ARRAYSIZE(peapx->szConfigUIPath), ee.pEapExtensionInfo[i].wszConfigUIPath));
        peapx->fProperties = (TEXT('\0') != peapx->szConfigUIPath[0]);
    }

    // Sort the list
    CASSERT(0 == offsetof(SHWZCEAPEX, dwType));
    CBREx(DSA_Sort(hdsa, EnumCompareEAP, 0), E_OUTOFMEMORY);

    // Success
    *phdsaEAP = hdsa;
    hdsa = NULL;

Error:
    if (NULL != hdsa)
    {
        DSA_Destroy(hdsa);
    }

    LocalFree(ee.pEapExtensionInfo);
    return hr;
}

extern "C"
void WINAPI SHWZCDestroyEAPList(HDSA hdsaEAP)
{
    if (NULL != hdsaEAP)
    {
        DSA_DestroyCallback(hdsaEAP, EnumDestroyEAP, NULL);
    }
}

extern "C"
HRESULT WINAPI SHWZCSetEAPData(DWORD dwType, LPCVOID pvData, UINT cbData, HDSA hdsaEAP)
{
    HRESULT    hr = S_OK;
    int        nIndex;
    LPVOID     pvDataCopy = NULL;
    SHWZCEAPEX *peapx;

    // Check inputs
    CBREx(NULL != hdsaEAP, E_INVALIDARG);
    if (NULL == pvData)
    {
        cbData = 0;
    }

    // Find the type in the list
    nIndex = DSA_Search(hdsaEAP, &dwType, 0, EnumCompareEAP, 0, DSAS_SORTED);
    CBREx(0 <= nIndex, E_INVALIDARG);

    if (0 != cbData)
    {
        // Allocate a copy of the data
        pvDataCopy = LocalAlloc(LMEM_FIXED, cbData);
        CPR(pvDataCopy);
        memcpy(pvDataCopy, pvData, cbData);
    }

    // Save the data
    peapx = (SHWZCEAPEX*)DSA_GetItemPtr(hdsaEAP, nIndex);
    LocalFree(peapx->pvData);
    peapx->pvData = pvDataCopy;
    peapx->cbData = cbData;
    pvDataCopy = NULL;

Error:
    LocalFree(pvDataCopy);
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCGetEAPData(DWORD dwType, LPVOID *ppvData, UINT *pcbData, HDSA hdsaEAP)
{
    HRESULT    hr = S_OK;
    int        nIndex;
    SHWZCEAPEX *peapx;

    // Check inputs
    CBREx(NULL != hdsaEAP, E_INVALIDARG);
    CBREx(NULL != ppvData, E_INVALIDARG);
    CBREx(NULL != pcbData, E_INVALIDARG);

    // Find the type in the list
    nIndex = DSA_Search(hdsaEAP, &dwType, 0, EnumCompareEAP, 0, DSAS_SORTED);
    CBREx(0 <= nIndex, E_INVALIDARG);

    // Success
    peapx = (SHWZCEAPEX*)DSA_GetItemPtr(hdsaEAP, nIndex);
    *ppvData = peapx->pvData;
    *pcbData = peapx->cbData;

Error:
    return hr;
}

extern "C"
HRESULT WINAPI SHWZCIsEAPExtensionValid(DWORD dwType, HDSA hdsaEAP, BOOL *pfExtensionValid)
{
    HRESULT hr = S_OK;
    SHWZCEAPEX           *peapx;
    int nIndex = -1;

    CBRAEx(hdsaEAP && pfExtensionValid, E_INVALIDARG);
    *pfExtensionValid = FALSE;

    // Find the type in the list
    nIndex = DSA_Search(hdsaEAP, &dwType, 0, EnumCompareEAP, 0, DSAS_SORTED);
    CBREx(0 <= nIndex, E_INVALIDARG);

    // Is there any configuration UI?
    peapx = (SHWZCEAPEX*)DSA_GetItemPtr(hdsaEAP, nIndex);
    CBREx(peapx, E_INVALIDARG);

    if(TEXT('\0') != peapx->szConfigUIPath[0])
    {
        *pfExtensionValid = TRUE;
    }

Error:
    return hr;
}


extern "C"
HRESULT WINAPI SHWZCConfigureEAPData(HWND hwndOwner, DWORD dwType, HDSA hdsaEAP)
{
    HRESULT              hr = S_OK;
    int                  nIndex;
    SHWZCEAPEX           *peapx;
    HINSTANCE            hinst = NULL;
    RASEAPINVOKECONFIGUI pfnEapConfig = NULL;
    RASEAPFREE           pfnEapFreeMem = NULL;
    BYTE                 *pbDataOut = NULL;
    DWORD                cbDataOut = 0;
    LPVOID               pvDataCopy = NULL;

    // Check inputs
    CBREx(NULL != hdsaEAP, E_INVALIDARG);

    // Find the type in the list
    nIndex = DSA_Search(hdsaEAP, &dwType, 0, EnumCompareEAP, 0, DSAS_SORTED);
    CBREx(0 <= nIndex, E_INVALIDARG);

    // Is there any configuration UI?
    peapx = (SHWZCEAPEX*)DSA_GetItemPtr(hdsaEAP, nIndex);
    CBREx(TEXT('\0') != peapx->szConfigUIPath[0], S_FALSE);

    // Load the configuration UI DLL
    hinst = LoadLibrary(peapx->szConfigUIPath);
    CBREx(NULL != hinst, E_NOTIMPL);

    pfnEapConfig = (RASEAPINVOKECONFIGUI)GetProcAddress(hinst, TEXT("RasEapInvokeConfigUI"));
    CBREx(NULL != pfnEapConfig, E_NOTIMPL);

    pfnEapFreeMem = (RASEAPFREE)GetProcAddress(hinst, TEXT("RasEapFreeMemory"));
    CBREx(NULL != pfnEapFreeMem, E_NOTIMPL);

    __try
    {
        // Display the configuration UI
        CBR(ERROR_SUCCESS == pfnEapConfig(dwType, hwndOwner, 0,  // no flags currently defined
            (LPBYTE)peapx->pvData, peapx->cbData, &pbDataOut, &cbDataOut));
        if (NULL == pbDataOut)
        {
            cbDataOut = 0;
        }

        // Copy the EAP data
        if (0 != cbDataOut)
        {
            pvDataCopy = LocalAlloc(LMEM_FIXED, cbDataOut);
            CPR(pvDataCopy);
            memcpy(pvDataCopy, pbDataOut, cbDataOut);
        }
    }
    __except(WatsonReportFault(GetExceptionInformation(), EXCEPTION_EXECUTE_HANDLER))
    {
        CBR(FALSE);
    }

    // Success
    LocalFree(peapx->pvData);
    peapx->pvData = pvDataCopy;
    peapx->cbData = cbDataOut;
    pvDataCopy = NULL;

Error:
    if (NULL != pbDataOut)
    {
        __try
        {
            pfnEapFreeMem(pbDataOut);
        }
        __except(WatsonReportFault(GetExceptionInformation(), EXCEPTION_EXECUTE_HANDLER))
        {
            // Not much we can do, but we may want to know about this
            ASSERT(FALSE);
        }
    }

    if (NULL != hinst)
    {
        FreeLibrary(hinst);
    }

    LocalFree(pvDataCopy);
    return hr;
}

/// <summary>Validates a network key based on the encryption type and key length.</summary>
///<returns>Returns TRUE if pszNetworkKey passes basic key length validation given the input encryption type,
/// FALSE otherwise.</returns>
/// <remarks>This function does not query the access point and does not analyze the contents of the key.  It only
/// performs a sanity check to ensure the length of the network key matches the protocol. For example, WEP keys
/// may currently only have lengths of 5 or 10.</remarks>
/// <param name="pszNetworkKey">[in] Pointer to a string containing the network key.</param>
/// <param name="dwEncryptionType">[in] Specifies the encryption type to use for validating the input key.</param>
extern "C"
BOOL WINAPI SHWZCValidateKey(LPCWSTR pszNetworkKey, DWORD dwEncryptionType, DWORD dwAuthenticationType)
{
    // Simple thunk
    return (0 < CWZCManagerWireless::WritePassword(NULL, pszNetworkKey, dwEncryptionType, dwAuthenticationType));
}

extern "C"
UINT WINAPI SHWZCGetHardwareFlags()
{
    HRESULT hr = S_OK;
    HWND    hwndSink;
    DWORD   dwDisabled = 0;

    // Check to see if wifi has been disabled by policy
    if (SUCCEEDED(RegistryGetDWORD(HKEY_LOCAL_MACHINE, TEXT("Comm\\NetworkPolicy\\WiFi"), TEXT("Disabled"), &dwDisabled)) &&
        (1 == dwDisabled))
    {
        hr = (HRESULT)SHWZCF_HW_DISABLED;
        goto Exit;
    }

    // Find a sink window
    hwndSink = SHWZCFindWindow();
    CBR(NULL != hwndSink);

    // Send a message to the sink window (a positive return value contains the
    // flags, a negative return value is a standard HRESULT error)
    hr = (HRESULT)SendMessage(hwndSink, SHWZCM_HWFLAGS, 0, 0);
    CHR(hr);

Exit:
Error:
    return SUCCEEDED(hr) ? (UINT)hr : 0;
}

/// <summary>
/// Check for valid combination of authentication and encryption types
/// </summary>
/// <param name="dwAuthenticationType">[in]  Specifies authentication type.</param>
/// <param name="dwEncryptionType">[in] Specifies encryption type.</param>
/// <param name="fAdhoc">[in]  Specifies whether the network configuration is adhoc or infrastructure</param>
/// <returns>
/// TRUE if the configuration is valid, FALSE otherwise
/// </returns>
BOOL WINAPI SHWZCValidateSecurityConfig(DWORD dwAuthenticationType, DWORD dwEncryptionType, BOOL fAdhoc)
{
    BOOL bRet = TRUE;

    switch (dwAuthenticationType)
    {
        case Ndis802_11AuthModeOpen:
            // Auth type: Open, valid Encryption types: Disabled or WEP
            bRet = ((dwEncryptionType == Ndis802_11EncryptionDisabled) ||
                    (dwEncryptionType == Ndis802_11Encryption1Enabled));
            break;

        case Ndis802_11AuthModeShared:
            // Auth type: Shared, valid Encryption type: WEP
            bRet = (dwEncryptionType == Ndis802_11Encryption1Enabled);
            break;

        case Ndis802_11AuthModeWPA:
        case Ndis802_11AuthModeWPA2:
        case Ndis802_11AuthModeWPAPSK:
        case Ndis802_11AuthModeWPA2PSK:
            // Auth types WPA/WPA2/WPA-PSK or WPA2-PSK
            // These are only valid for infrastructure networks, and with
            // Encryption types TKIP and AES
            bRet = (!fAdhoc &&
                    ((dwEncryptionType == Ndis802_11Encryption2Enabled) ||
                     (dwEncryptionType == Ndis802_11Encryption3Enabled)));
            break;
            
        case Ndis802_11AuthModeWPANone:
            // Auth type WPA-None, only valid for adhoc networks, and with
            // Encryption types TKIP and AES
            bRet = (fAdhoc &&
                    ((dwEncryptionType == Ndis802_11Encryption2Enabled) ||
                     (dwEncryptionType == Ndis802_11Encryption3Enabled)));
            break;
    }

    return bRet;
}


LRESULT SHWZCGetHardwareFlags_I(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = S_OK;
    UINT    uFlags = 0;

    // Send a private message to the UI threads
    hr = CWZCManager::SendAllPrivateMessage(PM_HWFLAGS, (WPARAM)&uFlags, 0);
    CHR(hr);

    // Success
    hr = (HRESULT)uFlags;

Error:
    return (LRESULT)hr;
}

/// <summary>Retrieves the signal strength of a WiFi network matching an input name and type description. 
///  If no name is supplied, this function returns the signal strength returned by
///  the sink window following a SHWZCMSG_DATA_SIGNALSTRENGTH data message.</summary>
/// <param name="pszAdapter">[in] Pointer to a string containing the adapter name in which to query.</param>
/// <param name="puStrength">[out] Pointer to an int that will receive the resulting signal strength.</param>
/// <param name="pszName">[in] Pointer to a string containing the network SSID in which to query.</param>
/// <param name="fAdhoc">[in] Flag specifying whether to query against adhoc (TRUE) or infrastructure (FALSE) networks.</param>
extern "C"
HRESULT WINAPI SHWZCGetSignalStrength(LPCTSTR pszAdapter, UINT *puStrength, LPCTSTR pszName, BOOL fAdhoc)
{
    HRESULT hr = S_OK;
    size_t  cchAdapter = 0;

    // Check inputs
    CBREx(NULL != puStrength, E_INVALIDARG);

    if (pszName == NULL)
    {
        // Get the length of the string
        if (NULL != pszAdapter)
        {
            CHREx(StringCchLength(pszAdapter, MAX_PATH, &cchAdapter), E_INVALIDARG);
        }

        // Send a message to the sink window (a positive return value contains the
        // signal strength, a negative return value is a standard HRESULT error)
        // NOTE: cchAdapter will not be greater than MAX_PATH, so no integer
        // overflow is possible
        hr = SendDataMessage(SHWZCMSG_DATA_SIGNALSTRENGTH,
            cchAdapter ? (LPVOID)pszAdapter : (LPVOID)&cchAdapter,
            sizeof(TCHAR) * (cchAdapter + 1));
        CHR(hr);

        // Ok, we have a signal strength stored in hr. Zero means we're not
        // connected at all, other values are [1..n] signal strength.
        *puStrength = (UINT)hr;
        hr = (*puStrength ? S_OK : S_FALSE);
    }
    else
    {
        SHWZCNET wzcnet = {0};
        hr = SHWZCGetNetwork(NULL, pszName, fAdhoc, &wzcnet, NULL, NULL);
        CHR(hr);

        *puStrength = wzcnet.nRssi;
    }

Error:
    return hr;
}

HRESULT GetSignalStrength_I(__in_bcount(cbData) LPCVOID pvData, DWORD cbData)
{
    HRESULT hr = S_OK;
    HWND    hwndSink;

    // Sanity check
    CBRAEx(cbData >= sizeof(TCHAR) && 0 == (cbData % sizeof(TCHAR)), E_INVALIDARG);

    // Find the sink window that matches this adapter
    hwndSink = CWZCManagerWireless::FindAdapterWindow((LPCTSTR)pvData);
    CBREx(NULL != hwndSink, SHWZC_E_HARDWARE);

    // Get the signal strength
    hr = (HRESULT)SendMessage(hwndSink, SHWZCM_SIGNALSTRENGTH, 0, 0);
    CHR(hr);

Error:
    return hr;
}

HRESULT SHWZCConvertName(LPCTSTR pszName, PSSID pssid)
{
    HRESULT hr = S_OK;
    size_t  cchName;
    BOOL    fInvalid = FALSE;

    // Check inputs
    CBREx(NULL != pszName && TEXT('\0') != pszName[0], SHWZC_E_INVALID_NAME);
    hr = StringCchLength(pszName, STRSAFE_MAX_CCH, &cchName);
    CHREx(hr, SHWZC_E_INVALID_NAME);

    // Can the name be converted to ANSI?
    // Determine if we can use UTF-8 for network names to allow for
    // better international support.
    pssid->SsidLength = WideCharToMultiByte(CP_ACP, 0, pszName, cchName,
        (LPSTR)pssid->Ssid, ARRAYSIZE(pssid->Ssid), NULL, &fInvalid);

    // If we failed, find out why (empty names will fail here)
    CBREx(0 != pssid->SsidLength, ERROR_INSUFFICIENT_BUFFER == GetLastError() ?
        SHWZC_E_INSUFFICIENT_BUFFER : SHWZC_E_INVALID_NAME);

    // If the name contains invalid characters, it's invalid
    CBREx(!fInvalid, SHWZC_E_INVALID_NAME);

Error:
    return hr;
}

/// <summary>Determines whether a network name contains only valid characters for an SSID and if valid,
/// copies the name into an input buffer.</summary>
/// <returns>Returns S_OK if pszName is non-empty, non-nul, and contains only valid SSID characters. 
/// Returns SHWZC_E_INVALID_NAME otherwise. </returns>
/// <param name="pszName">[in] Pointer to a buffer containing the String to be validated.</param>
/// <param name="pszSSID">[out] Pointer to a buffer to receive a copy of pszName, NULL is allowed if no copying is desired.</param>
/// <param name="cchSSID">[in] Size of the destination buffer in characters, or 0 if the destination buffer is NULL.</param>
extern "C"
HRESULT WINAPI SHWZCValidateNetworkName(LPCTSTR pszName, LPSTR pszSSID, UINT cchSSID)
{
    HRESULT hr = S_OK;
    SSID    ssid;

    // Convert to an SSID
    hr = SHWZCConvertName(pszName, &ssid);
    CHR(hr);

    // Copy the buffer if necessary
    if (NULL != pszSSID && 0 != cchSSID)
    {
        // cchBuffer already includes the terminating null
        hr = StringCchCopyNA(pszSSID, cchSSID, (LPCSTR)ssid.Ssid, ssid.SsidLength);
        CHR(hr);
    }

Error:
    return hr;
}

/// <summary>Determines whether a network already exists in a given network list.</summary>
/// <param name="hshwzc">[in] Callback struct on which to query.</param>
/// <param name="pszName">[in] SSID on which to query.</param>
/// <param name="fAdhoc">[in] Flag determining whether to query against adhoc (TRUE) or infrastructure (FALSE) networks.</param>
/// <returns>Returns S_OK if a network with the same name and type can already 
/// by found in hshwzc, S_FALSE otherwise.</returns>
extern "C"
HRESULT WINAPI SHWZCIsDuplicateName(HSHWZC hshwzc, LPCTSTR pszName, BOOL fAdhoc)
{
    HRESULT hr = S_OK;
    SHWZCCB *pcb = (SHWZCCB*)hshwzc;
    CNetID  nid;
    int     nIndex;

    // We only support searching in the callback state
    CBREx(NULL != pcb, E_INVALIDARG);

    // Get the network ID
    hr = nid.Assign(pszName, fAdhoc);
    CHR(hr);

    // Find the network
    nIndex = FindNetwork(&nid, &((SHWZCNETEX*)DSA_GetItemPtr(pcb->hdsaNetworks, 0))->sni.wzcnet,
        DSA_GetItemCount(pcb->hdsaNetworks), sizeof(SHWZCNETEX));
    CBREx(0 <= nIndex, S_FALSE);

Error:
    return hr;
}

/// <summary>Determines whether a network is a pre-configured permanent network.</summary>
///<returns>Returns TRUE if the input network may not be deleted because it is listed in the registry
/// as a permanent network, FALSE otherwise.</returns>
/// <param name="pszName">[in] String matching the SSID of the network in question.</param>
/// <param name="fAdHoc">[in] Flag specifying whether the network in question is adhoc (TRUE) or infrastructure (FALSE).</param>
///<remarks>Adhoc networks may not be permanent, so an input with fAdHoc = TRUE will always return FALSE.</remarks>
extern "C"
BOOL WINAPI SHWZCIsPermanentNetwork(LPCTSTR pszName, BOOL fAdHoc)
{
    HRESULT hr = S_OK;
    HKEY    hKey = NULL;
   
    CPREx(pszName, E_INVALIDARG);
    
    if(fAdHoc)
    {
        // Ad hoc networks cannot be permanent
        return FALSE;
    }

    TCHAR szPermanentNetworkList[MAX_PATH];
    DWORD cbSize = sizeof(szPermanentNetworkList);
    DWORD dwType;

    CBR(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_szRegKeyWZCSecure, 0, 0, &hKey));

    CBR((ERROR_SUCCESS == RegQueryValueEx(hKey, TEXT("PermanentNetworks"), 
                                          NULL, &dwType,
                                         (LPBYTE)szPermanentNetworkList, &cbSize)) &&
        (REG_MULTI_SZ == dwType));

    TCHAR *pszPermanentNetwork = szPermanentNetworkList;
    TCHAR *pszEnd = szPermanentNetworkList + (cbSize/sizeof(TCHAR));
       
    while (pszPermanentNetwork < pszEnd)
    {
        if (pszName[0] && 0 == _tcscmp(pszName, pszPermanentNetwork))
        {
            // Found a match - the network name exists in the OEM configured list of permanent
            // non-editable networks.
            break;
        }                    
        else
        {
            pszPermanentNetwork += (_tcslen(pszPermanentNetwork) + 1);
        }
    }
    CBR(pszPermanentNetwork < pszEnd);  // Did we find a match before the loop ended?
        
Error:
    if (hKey)
    {
        RegCloseKey(hKey);
    }
    
    return SUCCEEDED(hr);
}

/// <summary>
/// Enables or disables WiFi new network notifications by toggling the (NOTIF_MESSAGE | NOTIF_SOUND) bits 
/// in the "Options" subkey for wireless networks detected notifications.
/// </summary>
/// <param name="fNotificationsOn">[in] Enables notifications if TRUE, disables notifications otherwise.</param>
extern "C"
HRESULT WINAPI SHWZCSetNotificationState(BOOL fNotificationsOn)
{
    HRESULT hr = S_OK;
    DWORD dwOptionValue = 0;
    DWORD dwType;
    DWORD cbOptionValue = SIZEOF(dwOptionValue);
    HKEY hkey = NULL;

    CBR(ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER,
        g_szRegKeyWiFiNotifications, 0, NULL, 0, 0, NULL, &hkey, &dwType));

    if(ERROR_SUCCESS != RegQueryValueEx(hkey, TEXT("Options"), 0, &dwType,
        (LPBYTE)&dwOptionValue, &cbOptionValue) &&
           REG_DWORD == dwType)
    {
        //failed to read the value, 
        //assume it is 0 and make best effort to continue
        dwOptionValue = 0;
    }
    

    if (fNotificationsOn)
    {
        dwOptionValue |= (NOTIF_MESSAGE | NOTIF_SOUND);
    }
    else
    {
        dwOptionValue &= ~(NOTIF_MESSAGE | NOTIF_SOUND|NOTIF_FLASH|NOTIF_VIBRATE);
    }
    
    CBR(ERROR_SUCCESS == RegSetValueEx(hkey, TEXT("Options"), 0, REG_DWORD,
        (LPBYTE)&dwOptionValue, sizeof(dwOptionValue)));

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }
    return hr;
}

/// <summary>
/// Determines whether or not WiFi new network notifications are enabled. Sets pfNotificationsOn
/// to true if the wireless networks detected subkey "Options" has the bits (NOTIF_MESSAGE | NOTIF_SOUND) enabled and
/// false otherwise.
/// </summary>
/// <param name="pfNotificationsOn">[out] Set to TRUE if notifications are enabled, FALSE otherwise</param>
extern "C"
HRESULT WINAPI SHWZCGetNotificationState(__in BOOL* pfNotificationsOn)
{
    HRESULT hr = S_OK;
    DWORD dwOptionValue = 0;
    DWORD dwType;
    DWORD cbOptionValue = SIZEOF(dwOptionValue);
    HKEY hkey = NULL;

    CBREx(pfNotificationsOn, E_INVALIDARG);
    CBR(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER,
        g_szRegKeyWiFiNotifications, 0, 0, &hkey));

    CBR(ERROR_SUCCESS == RegQueryValueEx(hkey, TEXT("Options"), 0, &dwType,
        (LPBYTE)&dwOptionValue, &cbOptionValue) &&
           REG_DWORD == dwType);

    *pfNotificationsOn = BOOLIFY(dwOptionValue & (NOTIF_MESSAGE | NOTIF_SOUND));

Error:
    if (FAILED(hr) && pfNotificationsOn)
    {
        //if we are unable to read the reg, assume enabled
        *pfNotificationsOn = TRUE;
        hr = S_OK;
    }

    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }
    return hr;
}

/// <summary>
/// Sets the WiFi timeout period on the current device
/// </summary>
/// <param name="uTimeoutPeriod">
/// [in] Changes the WiFi Timeout Period on the device if this value is >= g_uMinWifiTimeout and less than g_uMaxWifiTimeout
/// </param>
extern "C"
HRESULT WINAPI SHWZCSetWifiTimeoutPeriod(UINT uTimeoutPeriod)
{
    HRESULT hr = S_OK;
    DWORD dwTimeoutPeriod = 0;
    HKEY hKey = NULL;

    // validate the requested period.  Only save if it is greater than 20 seconds and not large
    // enough to cause an overflow when we convert seconds->milliseconds.  
    if((uTimeoutPeriod >= g_uMinWifiTimeout) && (uTimeoutPeriod < g_uMaxWifiTimeout))
    {
        dwTimeoutPeriod = uTimeoutPeriod;
    }

    CBR(ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE,
        g_szRegKeyBase, 0, NULL, 0, 0, NULL, &hKey, NULL));

    CBR(ERROR_SUCCESS == RegSetValueEx(hKey, TEXT("Timeout"), 0, REG_DWORD,
        (LPBYTE)&dwTimeoutPeriod, sizeof(dwTimeoutPeriod)));

Error:
    if (NULL != hKey)
    {
        RegCloseKey(hKey);
    }
    
    return hr;
}

/// <summary>
/// Returns the WiFi timeout period in seconds for the current device.
/// </summary>
/// <param name="*puTimeoutPeriod">
/// [out] This value is set to the device's WiFi timeout period.
/// </param>
extern "C"
HRESULT WINAPI SHWZCGetWifiTimeoutPeriod(UINT *puTimeoutPeriod)
{
    HRESULT hr = S_OK;
    DWORD dwTimeoutPeriod = 0;

    CBREx(puTimeoutPeriod, E_INVALIDARG);
    *puTimeoutPeriod = 0;
    
    CHR(RegistryGetDWORD(HKEY_LOCAL_MACHINE, g_szRegKeyBase, TEXT("Timeout"), &dwTimeoutPeriod));

    // sanity check value in registry.  Only allow the value if it is greater than 20 seconds 
    // and not large enough to cause an overflow when we convert seconds->milliseconds.  
    if(0 != dwTimeoutPeriod)
    {
        CBR((dwTimeoutPeriod >= g_uMinWifiTimeout) && (dwTimeoutPeriod < g_uMaxWifiTimeout));
        *puTimeoutPeriod = dwTimeoutPeriod;
    }

Error:
    if(FAILED(hr) && puTimeoutPeriod)
    {
        // if we hit an error, assume set to never and try to write to registry
        VHR(SHWZCSetWifiTimeoutPeriod(0));
        hr = S_OK;
    }

    return hr;
}

extern "C"
HRESULT WINAPI SHWZCSetPowerState(BOOL fOn)
{
    HRESULT hr = S_OK;
    RDD     *prdd = NULL;
    RDD     *prddNext;

    hr = GetWirelessDevices(&prdd, WIRELESS_NO_PHONE);
    CHR(hr);

    while (NULL != prdd)
    {
        // Set the device power state
        if (POWER_MANAGED == prdd->DeviceType)
        {
            if (fOn)
            {
                // Force the device into the "on" state
                prdd->dwDesired = 1;
                VHR(ChangeRadioState(prdd, prdd->dwDesired, POWER_POST_SAVE));
            }
            else if (0 != prdd->dwState)
            {
                // We only turn power off if it's not already off
                VHR(ChangeRadioState(prdd, 0, POWER_POST_SAVE));
            }
        }

        // Move on
        prddNext = prdd->pNext;
        delete prdd;
        prdd = prddNext;
    }

Error:
    return hr;
}

/// <summary>Determines whether a WiFi adapter is available and powered on.</summary>
/// <returns>S_OK if a WiFi adapter can be found and is powered on, S_FALSE othewise</returns>
extern "C"
HRESULT WINAPI SHWZCGetPowerState()
{
    HRESULT hr = S_OK;
    RDD     *prdd = NULL;
    RDD     *prddNext;
    BOOL    fOn = FALSE;

    hr = GetWirelessDevices(&prdd, WIRELESS_NO_PHONE);
    CHR(hr);

    while (NULL != prdd)
    {
        // Is the device power on?
        if (POWER_MANAGED == prdd->DeviceType &&
            0 != prdd->dwState)
        {
            fOn = TRUE;
        }

        // Move on
        prddNext = prdd->pNext;
        delete prdd;
        prdd = prddNext;
    }

Error:
    return (SUCCEEDED(hr) ? (fOn ? S_OK : S_FALSE) : hr);
}


HRESULT GetWiFiStatusStringFromFlags(DWORD dwWiFiFlags, DWORD dwDisplayFlags, LPTSTR pszStatus, DWORD cchStatus)
{
    // Max length of SSID is 32 characters.
    static const k_cchSsidDisplayMax = 33;

    HRESULT hr = S_FALSE;

    LPCTSTR pszWiFi = LoadStringEtcOver(HINST_RESDLL, IDS_WIRELESSCARD); // "Wi-Fi" in 0409
    LPCTSTR pszWiFiStatusFormat = (LPTSTR)LoadString(HINST_RESDLL, IDS_WZC_WIFI_STATUS_FORMAT, NULL, 0);
    LPTSTR pszWiFiStatus = NULL;

    CPR(pszWiFi);
    CPR(pszWiFiStatusFormat);

    TCHAR szSsid[k_cchSsidDisplayMax + 1] = {0};
    DWORD cbSsid = sizeof(szSsid);

    // Only bother with a string if we have Wi-Fi hardware
    if (dwWiFiFlags & WSF_HARDWARE_PRESENT)
    {
        if (dwWiFiFlags & WSF_POWER_ON)
        {
            if (dwWiFiFlags & WSF_CONNECTED)
            {
                // Query the SSID...
                LONG lResult = SHRegQueryValueEx(SN_CONNECTIONSNETWORKDESCRIPTIONS_ROOT, SN_CONNECTIONSNETWORKDESCRIPTIONS_VALUE, (DWORD*)SN_CONNECTIONSNETWORKDESCRIPTIONS_PATH, NULL, (LPBYTE)szSsid, &cbSsid);
                if (lResult != ERROR_SUCCESS && lResult != ERROR_MORE_DATA)
                {
                    // Error reading the registry key
                    szSsid[0] = TEXT('\0');
                }
                else
                {
                    // Let's search for a semicolon; if there is one, it is a delimiter
                    // in the list of SSIDs.
                    TCHAR *pch = _tcschr(szSsid, TEXT(';'));
                    if (pch)
                    {
                        *pch = TEXT('\0');
                    }
                    else
                    {
                        // Otherwise, let's forcibly delimit in case the result was ERROR_MORE_DATA
                        szSsid[k_cchSsidDisplayMax] = TEXT('\0');
                    }
                    
                    hr = S_OK;
                }
                
                pszWiFiStatus = szSsid;
            }
            else if (dwWiFiFlags & WSF_CONNECTING)
            {
                pszWiFiStatus = (LPTSTR)LoadString(HINST_RESDLL, IDS_WZC_CONNECTING, NULL, 0);
            }
            else if (dwWiFiFlags & WSF_NETWORKS_AVAILABLE)
            {
                pszWiFiStatus = (LPTSTR)LoadString(HINST_RESDLL, IDS_WZC_AIRING, NULL, 0);
            }
            else
            {
                pszWiFiStatus = (LPTSTR)LoadString(HINST_RESDLL, IDS_WZC_SILENT, NULL, 0);
            }                
        }
        else
        {
            pszWiFiStatus = (LPTSTR)LoadString(HINST_RESDLL, IDS_OFF, NULL, 0);
        }

        // Format the string
        if (dwDisplayFlags & SHWZCF_GWSS_INCLUDE_WIFI)
        {
            // We want this in "Wi-Fi: xxx" format
            LPVOID lpvValues[2] = // Store parameters
                {(LPVOID)pszWiFi, (LPVOID)pszWiFiStatus};

            DWORD dwResult = ::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                  pszWiFiStatusFormat,      // "%1!s!: %2!s!" in 0409
                  0,
                  0,
                  pszStatus,
                  cchStatus,
                  (va_list *) lpvValues);

            if (dwResult == 0)
            {
                hr = HRESULT_FROM_WIN32(GetLastError()); 
                CHR(hr);
            }
        }
        else
        {
            // just return the string
            hr = StringCchCopy(pszStatus, cchStatus, pszWiFiStatus);
        }            
    }
    else
    {
        hr = E_FAIL;
    }

Error:
    return hr;
}

/// <summary>
/// Returns a string to represent Wi-Fi status in human-readable format.
/// Possible strings in 0409:
/// "Available"
/// "Unavailable"
/// "Off"
/// An SSID
///
/// If dwFlags has the SHWZCGWSS_INCLUDE_WIFI flag set, then the string will 
/// be formatted with "%1!s!: %2!s!" (0409), with %1 being "Wi-Fi" and %2
/// being one of the strings above. It is possible for an OEM to override the
/// "Wi-Fi" portion of the string since the "Wi-Fi" string will be called via 
/// LoadStringOver().
/// Return values: S_OK if an SSID was set, S_FALSE if no SSID was set, 
///                or an HRESULT error code.
/// </summary>
/// <param name="dwFlags">[in] Flag for specific status string formatting.
/// Currently the only valid value other than 0 is SHWZCF_GWSS_INCLUDE_WIFI, which returns the status string in the format Wi-Fi: xxx</param>
/// <param name="pszStatus">[out] Pointer to a buffer to receive the status string.</param>
/// <param name="cchStatus">[out] Size of the returned status string, in characters.</param>
extern "C"
HRESULT WINAPI SHWZCGetWiFiStatusString(DWORD dwFlags, LPTSTR pszStatus, DWORD cchStatus)
{
    DWORD dwWiFiFlags = 0;
    DWORD cbWiFiFlags = sizeof(dwWiFiFlags);
    
    LONG lResult = SHRegQueryValueEx(SN_WIFISTATEHARDWAREPRESENT_ROOT, SN_WIFISTATEHARDWAREPRESENT_VALUE, (DWORD*)SN_WIFISTATEHARDWAREPRESENT_PATH, NULL, (LPBYTE)&dwWiFiFlags, &cbWiFiFlags);
    if (lResult == ERROR_SUCCESS)
    {
        return GetWiFiStatusStringFromFlags(dwWiFiFlags, dwFlags, pszStatus, cchStatus);
    }

    return E_FAIL;
}
///</topic_scope> wzcshell

