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
// Filename:    wzcmgr.cpp
//
// Purpose:     WZC Manager object code
//
//==========================================================================*

#include <windows.h>
#include <wpcpriv.h>
#include <msgqueue.h>
#include <miscsvcs_priv.h>
#include <ehm.h>

#include <pm.h>
#include <statstor.h>
#include <ndis.h>
#include <nuiouser.h>

#include "netuip.h"
#include "wzcuip.h"
#include "wzcmgr.h"
#include "connmgr_priv.h"
#include <resource.h>

#include <pwinuser.h>

//#include "wzcguid_p.h"

#include <BBS_CommFunc.h>
#include <BBS_WifiXml.h>
#include <BBS_ControlSvcMgrDrv.h>


// docbld tag for the entire file.  The WiFi topic is defined in wzcshell.cpp
///<topic_scope tref="WiFi">

LPCTSTR            g_szRegKeyBase                 = WZC_REGKEY_BASE;
LPCTSTR            g_szRegKeyCallbacks            = WZC_REGKEY_CALLBACKS;
extern LPCTSTR     g_szValueFormat;
extern LPCTSTR     g_szRegPathCallbacks;
static const TCHAR g_szWZCMsgQueueName[]          = WZC_MSGQUEUE_NAME;
static const TCHAR g_szRegKeyWZCSecure[]          = TEXT("Comm\\WiFi");
static const TCHAR g_szWZCNdisuioMsgQueueName[]   = TEXT("WzcNdisuioMsgQueue");
static const TCHAR g_szSinkClassWired[]           = TEXT("SinkWiredNetUI");
LPCTSTR            g_szSinkClassWireless          = TEXT("SinkWirelessNetUI");
static const TCHAR g_szRegValueScanInterval[]     = TEXT("ScanInterval");
static const TCHAR g_szRegValueScanConnected[]    = TEXT("ScanConnected");
static const TCHAR g_szRegValueUIDelayAfterBoot[] = TEXT("UIDelayAfterBoot");
extern LPCTSTR     g_szEventMessage;

static const TCHAR c_szRegKeyWZCPresets[]       = TEXT("Software\\Microsoft\\WZCSVC\\Presets");
static const TCHAR c_szRegKeyPresetPrefix[]     = TEXT("preset");
static const TCHAR c_szRegValueSSID[]           = TEXT("SSID");
static const TCHAR c_szRegValueAuthentication[] = TEXT("Authentication");
static const TCHAR c_szRegValuePrivacy[]        = TEXT("Privacy");
static const TCHAR c_szRegValueEnable8021x[]    = TEXT("Enable8021x");
static const TCHAR c_szRegValueEapType[]        = TEXT("EapType");
static const TCHAR c_szRegValueHidden[]         = TEXT("Hidden");

static const TCHAR c_szGSIMutex[] = TEXT("SYSTEM/ConfigManagerMutex");  // must match the name of the configmanager mutex

static const TCHAR c_szRegPathPowerMgrState[]   = PWRMGR_REG_KEY TEXT("\\State");
static const TCHAR c_szDefaultPowerControlTemplate[] = TEXT("Comm\\%s\\Parms");
static const TCHAR c_szRegValueDisableDefaultPowerControl[] = TEXT("DisableDefaultPowerControl");
static const TCHAR c_szPowerMgrDeviceTemplate[] = PMCLASS_NDIS_MINIPORT TEXT("\\%s");
static const TCHAR c_szRegValueFlightMode[]     = TEXT("Wireless");
static const TCHAR c_szDriverEventTemplate[]    = TEXT("SYSTEM\\netui-%s");

static const TCHAR c_szRegValueWifiTimeout[]    = TEXT("Timeout");
static const TCHAR c_szRegValuePersistAutoIP[]  = TEXT("PersistAutoIP");

#define FACILITY_WZC        0x07FF
#define SHWZC_E_INVALID     MAKE_HRESULT(1, FACILITY_WZC, WZC_FAILED_AUTH_NO_RETRY)
#define SHWZC_E_CANCELAUTH  MAKE_HRESULT(1, FACILITY_WZC, WZC_CANCEL_AUTH)

#define INTF_STANDARD_QUERY   (INTF_PREFLIST|INTF_BSSIDLIST|INTF_CM_MASK|INTF_FALLBACK|INTF_SSID|INTF_INFRAMODE)

enum
{
    WM_WZC_SCANCOMPLETE           = WM_USER,
    WM_WZC_CHECKUIBLOCK,
    WM_WZC_CHECKUI,
    WM_WZC_PRIVATE,
    WM_WZC_CONNECT_BEGIN,
    WM_WZC_CONNECT_STOPPED,
    WM_WZC_HANDLE_EAPOL_CLOSE,
    WM_WZC_UPDATE_8021X_STATE,
    WM_WZC_CONNECT_COMPLETE_SEND,
    WM_WZC_CONFIGURE,
    WM_WZC_CONNECT_STATUS,
    WM_WZC_REMOVECARD,
    WM_WZC_CLOSE,
    WM_WZC_POWERCHANGE,
    WM_WZC_AUTH_FAILED
};

// configuration messages
enum
{
    WZC_CM_CONNECT_RESULT,      // network has been connected or disconnected (lp: hrConnect)
    WZC_CM_UPDATE_ASSOCIATION   // update the connected network destination (lp: unused)
};

// types
typedef WZC_802_11_CONFIG_LIST       *PCFGLIST;
typedef const WZC_802_11_CONFIG_LIST *PCCFGLIST;

struct PRIVATEMSG
{
    UINT   msg;
    WPARAM wp;
    LPARAM lp;
};

struct CWZCManagerWireless::FINDOWNERINFO
{
    CNetID  nid;
    HWND    hwnd;
    BOOL    fFindNetID;
};

// globals
ATOM CWZCManagerWired::s_atmSinkClass = 0;

ATOM   CWZCManagerWireless::s_atmSinkClass = 0;
UINT   CWZCManagerWireless::s_uEventMsg = 0;
HANDLE CWZCManagerWireless::s_hStatusThread = NULL;
HANDLE CWZCManagerWireless::s_hStatusThreadStopEvent = NULL;
HANDLE CWZCManagerWireless::s_hWZCMsgQueue = NULL;
HANDLE CWZCManagerWireless::s_hWZCNdisuioMsgQueue = NULL;
UINT   CWZCManagerWireless::s_cRefStatusThread = 0;

HRESULT CWZCManager::RegisterClass(LPCTSTR pszClassName, ATOM *patm)
{
    HRESULT  hr = S_OK;
    WNDCLASS wc = { 0 };

    wc.style = CS_NOCLOSE;
    wc.lpfnWndProc = SinkWndProc;
    wc.hInstance = g_hInstResDPI;
    wc.lpszClassName = pszClassName;

    *patm = ::RegisterClass(&wc);
    CWR(*patm);

Error:
    return hr;
}

// Create the message sink window
HRESULT CWZCManager::CreateSinkWindow(ATOM atmSinkClass)
{
    HRESULT hr = S_OK;

    // If you hit this assert, somehow we're trying to create a sink window
    // when we already have one. Why on earth would we want to do that?
    ASSERT(m_hwndSink == NULL);

    // Create the window
    m_hwndSink = CreateWindow((LPCTSTR)atmSinkClass, NULL, WS_OVERLAPPED,
        -10, -10, 5, 5, NULL, NULL, g_hInstResDPI, this);
    CWR(m_hwndSink);

Error:
    return hr;
}

void CWZCManager::DestroyUI()
{
    if (m_hwndUI)
    {
        DestroyWindow(m_hwndUI);
        m_hwndUI = NULL;
    }
}

// Initialize a new manager object when a new network card is inserted
HRESULT CWZCManager::Initialize(LPCTSTR pszDeviceName)
{
    HRESULT hr = S_OK;

    // Copy the device name
    m_pszDeviceName = StrDup(pszDeviceName);
    CPR(m_pszDeviceName);

Error:
    return hr;
}

BOOL CWZCManager::IsDialogMessage_WZC(HWND hDlg, LPMSG lpMsg)
{
    CWZCManager *pmgr;
    BOOL        fResult = FALSE;

    // Get the object pointer from the window handle
    pmgr = reinterpret_cast<CWZCManager*>(GetWindowLong(hDlg, GWL_USERDATA));
    if (pmgr && pmgr->m_hwndUI)
    {
        // Pass the request on to the current UI window
        fResult = ::IsDialogMessage(pmgr->m_hwndUI, lpMsg);
    }

    return fResult;
}

void CWZCManager::OnClose()
{
    // Destroy this window
    DestroyWindow(m_hwndSink);
    m_hwndSink = NULL;
}

void CWZCManager::OnConnectionStatusUpdate(BOOL fConnected)
{
    // Just post a message, let the UI thread handle it
    PostMessage(m_hwndSink, WM_WZC_CONNECT_STATUS, fConnected, 0);
}

void CWZCManager::OnRemoveCard()
{
    // Destroy the UI window if it's still around
    DestroyUI();

    // Clean up any existing connection
    OnConnectResult(S_FALSE);

    // Delete this object (will destroy the sink window)
    delete this;

    // Uninitialize COM on this thread
    CoUninitialize();
}

HRESULT CWZCManager::PrivateMessageCallback(HWND hwndSink, LPVOID pvParam)
{
    // Send the message to the enumerated object
    return (HRESULT)SendMessage(hwndSink, WM_WZC_PRIVATE, 0, (LPARAM)pvParam);
}

void CWZCManager::RemoveCard()
{
    // This is being called from the context of another thread. We post a
    // message to the sink window to jump over into its context, from which
    // we'll clean up and exit its thread.
    PostMessage(m_hwndSink, WM_WZC_REMOVECARD, 0, 0);
}

HRESULT CWZCManager::SendAllPrivateMessage(UINT msg, WPARAM wp, LPARAM lp)
{
    PRIVATEMSG privmsg = { msg, wp, lp };
    return EnumerateWindows(PrivateMessageCallback, &privmsg);
}

void CWZCManager::PostConfigMessage(UINT uMsg, LPARAM lp)
{
    // Just post a configuration message to the message queue
    PostMessage(m_hwndSink, WM_WZC_CONFIGURE, (WPARAM)uMsg, lp);
}

HRESULT CWZCManager::TryConfigMessage(UINT uMsg, LPARAM lp)
{
    HRESULT hr = S_OK;
    DWORD   dwRes;

    // In order to do some Connection Manager operations, such as querying
    // "enabling networks" on PPC, we need to ensure that the defaultconfig XML
    // files have been processed
    WaitForFullBoot();

    // Create the configuration mutex
    if (NULL == m_hConfigMutex)
    {
        m_hConfigMutex = CreateMutex(NULL, FALSE, c_szGSIMutex);
        CWR(NULL != m_hConfigMutex);
    }

    // Try to get the configuration mutex. If we can't, that means that someone
    // else has it and we need to wait until they're done (since they may be
    // waiting for us).
    dwRes = WaitForSingleObject(m_hConfigMutex, 100);
    CWR(WAIT_FAILED != dwRes);
    if (WAIT_OBJECT_0 == dwRes)
    {
        // This means we have the mutex, so we can process the connection
        OnConfigMessage(uMsg, lp);
        ReleaseMutex(m_hConfigMutex);
        goto Exit;
    }

    // Someone else has the mutex, so try again later
    PostConfigMessage(uMsg, lp);
    hr = S_FALSE;

Error:
Exit:
    return hr;
}

void CWZCManager::OnConfigMessage(UINT uMsg, LPARAM lp)
{
    switch (uMsg)
    {
    case WZC_CM_CONNECT_RESULT:
        OnConnectResult((HRESULT)lp);
        break;

    case WZC_CM_UPDATE_ASSOCIATION:
        OnUpdateAssociation();
        break;

    default:
        ASSERT(FALSE);  // we don't know about this message!!
        break;
    }
}

void CWZCManager::OnConnectResult(HRESULT hrConnect)
{
    HRESULT hr = S_OK;

    // S_OK means we're connected, but we need to check the connection validity
    if (S_OK == hrConnect)
    {
        IPSTATE state = GetIPState(m_pszDeviceName, NULL, 0);
        ASSERT(IPSTATE_DISCONNECTED != state);  // this shouldn't happen, but we'll handle it
        if (!CheckIPState(state))
        {
            // This connection isn't useful to us... yet. We'll get another
            // connected notification if this changes.
            hrConnect = S_FALSE;
        }
    }

    // hrConnect is S_OK if we're connected
    if (S_OK == hrConnect &&
        !m_fConnected)
    {
        m_fConnected = TRUE;
        hr = OnConnect();
        if (FAILED(hr))
        {
            hrConnect = hr;
            ForceDisconnect();

            // We successfully connected, but then failed for some internal
            // reason. We want to know about this.
            ASSERT(FALSE);
        }
    }

    // hrConnect is S_FALSE if we've successfully disconnected, or it's an
    // error code if we failed to connect
    if (S_OK != hrConnect &&
        m_fConnected)
    {
        m_fConnected = FALSE;
        OnDisconnect();
    }

    // If we failed to connect, handle the error
    if (FAILED(hrConnect))
    {
        OnConnectFailed(hrConnect);
    }
}

HRESULT CWZCManagerWireless::OnUpdateAssociation()
{
    HRESULT hr = S_OK;
    PCWLC   pwlc;
    CNetID  nid;

    // Are we connected at all?
    pwlc = GetConnectedNetwork();
    CBREx(NULL != pwlc, S_FALSE);

    // Update the destination network
    nid.Assign(pwlc);
    hr = CM_SetCurrentNetwork(m_pszDeviceName, NULL, nid.GetAssociation());
    CHR(hr);

Error:
    return hr;
}

LRESULT CALLBACK CWZCManager::SinkWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CWZCManager *pmgr;
    LRESULT     lr;

    if (WM_CREATE == msg)
    {
        // Store the object pointer we sent to ourselves
        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
        SetWindowLong(hwnd, GWL_USERDATA, (LONG)lpcs->lpCreateParams);
        pmgr = (CWZCManager*)lpcs->lpCreateParams;
        pmgr->m_hwndSink = hwnd;
    }
    else
    {
        // Get the manager object
        pmgr = (CWZCManager*)GetWindowLong(hwnd, GWL_USERDATA);
        if (NULL == pmgr)
        {
            lr = DefWindowProc(hwnd, msg, wParam, lParam);
            goto Exit;
        }
    }

    // Handle the message
    lr = pmgr->OnMessage(msg, wParam, lParam);

Exit:
    if (msg == WM_DESTROY)
    {
        // PostQuitMessage tells the EthMan thread to exit
        PostQuitMessage(0);
    }

    return lr;
}

LRESULT CWZCManager::OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_TIMER:
        OnTimer(wParam);
        return 0;

    case WM_WZC_PRIVATE:
        {
            const PRIVATEMSG *pprivmsg = (PRIVATEMSG*)lParam;
            return OnPrivateMessage(pprivmsg->msg, pprivmsg->wp, pprivmsg->lp);
        }

    case WM_WZC_CONFIGURE:
        TryConfigMessage((UINT)wParam, lParam);
        return TRUE;

    case WM_WZC_CONNECT_STATUS:
        TryConfigMessage(WZC_CM_CONNECT_RESULT, wParam ? S_OK : S_FALSE);
        return TRUE;

    case WM_WZC_REMOVECARD:
        OnRemoveCard();
        return TRUE;

    case WM_CLOSE:
        // We don't close the window in response to a standard WM_CLOSE
        return 0;

    case WM_WZC_CLOSE:
        OnClose();
        return TRUE;

    case WM_DESTROY:
        OnDestroy();
        return 0;
    }

    // Default message handling
    return DefWindowProc(m_hwndSink, msg, wParam, lParam);
}

void CWZCManager::OnDisconnect()
{
    // Default processing
    CM_ClearCurrentNetwork(m_pszDeviceName);
}

HRESULT CWZCManagerWired::OnConnect()
{
    HRESULT hr = S_OK;

    // Default processing
    hr = CWZCManager::OnConnect();
    CHR(hr);

    // Notify the shell
    PostShellStatusMessage(WM_TASKBAR_ETHERNETCHANGED, TRUE, FALSE);

Error:
    return hr;
}

void CWZCManagerWired::OnDisconnect()
{
    // Default processing
    CWZCManager::OnDisconnect();

    // Notify the shell
    PostShellStatusMessage(WM_TASKBAR_ETHERNETCHANGED, FALSE, FALSE);
}

CWZCManager::~CWZCManager()
{
    // Free the name string
    LocalFree(m_pszDeviceName);

    // Close the configuration mutex handle
    if (NULL != m_hConfigMutex)
    {
        CloseHandle(m_hConfigMutex);
    }
}

HRESULT CWZCManagerWired::OnListInsert(LPVOID *ppVoid)
{
    HRESULT hr = S_OK;
    *ppVoid = NULL;

    // This function is protected by a critical section, so we can safely check
    // and set global variables
    if (0 == s_atmSinkClass)
    {
        // Register the sink window class
        hr = RegisterClass(g_szSinkClassWired, &s_atmSinkClass);
        CHR(hr);
    }

    // Create the sink window
    hr = CreateSinkWindow(s_atmSinkClass);
    CHR(hr);

Error:
    return hr;
}

BOOL CWZCManagerWired::CheckIPState(IPSTATE state)
{
    // Allow any connected status for a wired network card -- usually, an auto
    // IP means that the user can't use the connection, and so it would
    // probably be better to refuse those connections, but EDG swears this is a
    // valid scenario on some home networks
    return (state >= IPSTATE_AUTOIP);
}

CWZCManagerWired::~CWZCManagerWired()
{
    if (m_hwndSink)
    {
        // Close the sink window (will clean up in the context of the UI thread)
        SendMessage(m_hwndSink, WM_WZC_CLOSE, 0, 0);
        m_hwndSink = NULL;
    }

    // Unregister the sink window class. This will fail if there are still
    // other sink windows open.
    if (0 != s_atmSinkClass &&
        UnregisterClass((LPCTSTR)s_atmSinkClass, g_hInstResDPI))
    {
        s_atmSinkClass = 0;
    }
}

LRESULT CWZCManagerWireless::OnMessage(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_POWERBROADCAST:
        if (PBT_APMRESUMESUSPEND == wp)
        {
            // When we resume power, clear the dismissed network list
            m_nidlDismissed.Reset();
            ClearStateCache();
            CheckNewNetworks();
        }
        break;

    case WM_COPYDATA:
        if (0 != lp)
        {
            const COPYDATASTRUCT *pcds = (COPYDATASTRUCT*)lp;
            return ReceiveDataMessage(pcds->dwData, pcds->lpData, pcds->cbData);
        }
        return FALSE;

    case WM_WZC_SCANCOMPLETE:
        OnScanComplete();
        return TRUE;

    case WM_WZC_CONNECT_BEGIN:
        OnConnectBegin((CNetID*)lp);
        return TRUE;

    case WM_WZC_CONNECT_STOPPED:
        OnConnectStopped();
        return TRUE;

    case WM_WZC_HANDLE_EAPOL_CLOSE:
        OnEapolClose((CNetID*)lp);  
        return TRUE;

    case WM_WZC_UPDATE_8021X_STATE:
        // Don't set 8021x state to authenticated for non-8021x networks.
        if (WZC_AUTHSTATE8021X_AUTHENTICATED == (WZC_AUTHSTATE8021X) wp)
        {
            if (WZC_AUTHSTATE8021X_ASSOCIATED == m_asCurrent8021XState) 
            {
                m_asCurrent8021XState = WZC_AUTHSTATE8021X_AUTHENTICATED;
            }
        }
        else
        {
            m_asCurrent8021XState = (WZC_AUTHSTATE8021X) wp;
        }
        return TRUE;

    case WM_WZC_CONNECT_COMPLETE_SEND:
        OnConnectCompleteSent((HRESULT)wp, (CNetID*)lp);
        return TRUE;

    case WM_WZC_CHECKUIBLOCK:
        OnUnblockUIPost();
        return TRUE;

    case WM_WZC_POWERCHANGE:
        OnPowerChange((BOOL)wp);
        return TRUE;

    case WM_WZC_AUTH_FAILED:
        OnAuthenticationFailure((HRESULT)wp, (CNetID*)lp);

    case SHWZCM_GETNETLIST:
        return (LRESULT)SHWZCGetCurrentNetworksMessage_I(wp, lp);

    case SHWZCM_GETNETWORK:
        return (LRESULT)SHWZCGetNetwork_I(wp, lp);

    case SHWZCM_READNETWORK:
        return GetNetwork((PSHWZCNET)lp, (LPVOID)(((PSHWZCNET)lp)+1), wp);

    case SHWZCM_REGISTERCB:
        return (LRESULT)SHWZCRegisterCallbackWindow_I(wp, lp);

    case SHWZCM_UNREGISTERCB:
        return (LRESULT)SHWZCUnregisterCallbackWindow_I(wp, lp);

    case SHWZCM_CLEARNETWORK:
        return DeleteNetwork((CNetID*)lp, FALSE);

    case SHWZCM_SIGNALSTRENGTH:
        return GetSignalStrength(m_pszDeviceName);

    case SHWZCM_GETSTATE:
        return SHWZCGetState_I(wp, lp);

    case SHWZCM_SETSTATE:
        return SHWZCSetState_I(wp, lp);

    case SHWZCM_HWFLAGS:
        return SHWZCGetHardwareFlags_I(wp, lp);
    }

    // Is this a global event?
    if (s_uEventMsg == msg)
    {
        OnGlobalEvent(wp);
        return TRUE;
    }

    // Default processing
    return CWZCManager::OnMessage(msg, wp, lp);
}

HRESULT CWZCManagerWireless::BeginConnect(PWLC pwlc)
{
    HRESULT hr = S_OK;

    // This function is being called from the notification UI, so the user
    // hasn't ever decided whether the network uses 802.1x. We can
    // automatically detect that it's needed on certain networks, though.
    if ((WZCCTL_ONEX_ENABLED & pwlc->dwCtlFlags) &&
        !pwlc->EapolParams.bEnable8021x)
    {
        // Give it the default EAP settings
        GetEAPDefaults(pwlc);
    }

    // Get the current preferred list
    hr = m_intf.Query(INTF_PREFLIST);
    CHR(hr);

    // Push the configuration to the top of the preferred list
    hr = m_intf.SetMostPreferred(pwlc);
    CHR(hr);

    // Apply the changes
    hr = ApplyNetworkList(&m_intf);
    CHR(hr);

    // We're done with this network configuration
    WZCFreeConfig(&m_wlcKey);

Error:
    return hr;
}

DWORD CWZCManagerWireless::GetPasswordState(__in PCWLC pwlc)
{
    DWORD dwPasswordState = (DWORD)-1;

    // If we're encrypted, we probably need a password
    if (IsEncrypted(pwlc->Privacy))
    {
        dwPasswordState = PWDUI_WEP;
    }

    // Need a password for WPA-PSK, WPA-None, WPA2-PSK
    if (IsAnyWPAPSK(pwlc->AuthenticationMode) ||
        pwlc->AuthenticationMode == Ndis802_11AuthModeWPANone)
    {
        dwPasswordState = PWDUI_WPA;
    }
    else if (IsAnyWPA(pwlc->AuthenticationMode))
    {
        // No password for WPA[2]-Plain
        dwPasswordState = (DWORD)-1;        
    }

    // If we're about to the show the WEP password, but we're using 802.1x,
    // just connect
    if (PWDUI_WEP == dwPasswordState &&
        pwlc->EapolParams.bEnable8021x)
    {
        dwPasswordState = (DWORD)-1;
    }

    return dwPasswordState;
}

HRESULT CWZCManagerWireless::DoConnect(PWLC pwlc)
{
    HRESULT hr = S_OK;
    HRESULT hrConnect = E_FAIL;
    DWORD   dwPasswordState;
	BBS_SCAN_OPTION_INFO sScanOpt = {0,};
	
	RETAILMSG(1, (TEXT("[PSH] CWZCManagerWireless::DoConnect(%hs) \r\n"), pwlc->Ssid.Ssid));

	// LockSSID
	if( GetScanOpt(&sScanOpt) == TRUE)
	{
		if( _tcslen(sScanOpt.tszLockSSID) > 0 )
		{
			TCHAR tszSSID[NDIS_802_11_LENGTH_SSID] = {0,};

			mbstowcs(tszSSID, (char*)pwlc->Ssid.Ssid, pwlc->Ssid.SsidLength);
			RETAILMSG(1, (TEXT("[PSH] CWZCManagerWireless::OnConnectBegin() tszSSID : %s \r\n"), tszSSID));

			// tszLockSSID 가 걸려있고, 같지 않으면 에러로..
			if( _tcscmp(sScanOpt.tszLockSSID, tszSSID) != 0 )
			{
				TCHAR tszTemp[128] = L"";

				wsprintf(tszTemp, L"LockSSID '%s' ", sScanOpt.tszLockSSID);
				MessageBox(GetForegroundWindow(), tszTemp, L"ERROR", MB_ICONERROR);
				RETAILMSG(1, (TEXT("[PSH] goto Error \r\n") ));
				goto Error;
			}
		}
	}


    // Do we need a password?
    dwPasswordState = (WZCCTL_WEPK_PRESENT & pwlc->dwCtlFlags) ? (DWORD)-1 : GetPasswordState(pwlc);
    if ((DWORD)-1 != dwPasswordState)
    {
        // Copy this configuration
        WZCFreeConfig(&m_wlcKey);
        hr = WZCCopyConfigs(&m_wlcKey, pwlc, 1, TRUE);
        CHR(hr);

        // When connecting through notification UI, we always use the first key
        // index
        m_wlcKey.KeyIndex = 0;

        // Show the password dialog
        hr = ShowPasswordDialog(dwPasswordState);
        CHR(hr);
    }
    else
    {
        // Just connect directly
        hr = BeginConnect(pwlc);
        CHR(hr);
    }

Error:
    if (FAILED(hr))
    {
        // The user has told us s/he wants to connect, and we've failed, so we
        // need to put up some sort of error.
        TryConfigMessage(WZC_CM_CONNECT_RESULT, (LPARAM)hrConnect);
    }

    return hr;
}

HWND CWZCManagerWireless::FindOwnerWindow(const CNetID *pnid)
{
    HRESULT       hr = S_OK;
    FINDOWNERINFO foi;

    // Find the owner
    foi.nid = *pnid;
    foi.hwnd = NULL;
    foi.fFindNetID = TRUE;

    hr = SendAllPrivateMessage(PM_FINDOWNER, 0, (LPARAM)&foi);
    CHR(hr);

Error:
    return (SUCCEEDED(hr) ? foi.hwnd : NULL);
}

HWND CWZCManagerWireless::FindAdapterWindow(__in LPCTSTR pszAdapter)
{
    HRESULT hr = S_OK;
    HWND    hwndSink = NULL;

    // Send everyone a private message
    hr = SendAllPrivateMessage(PM_FINDADAPTER, (WPARAM)pszAdapter, (LPARAM)&hwndSink);
    CHR(hr);

Error:
    return (SUCCEEDED(hr) ? hwndSink : NULL);
}

BOOL CWZCManagerWireless::IsUIBlocked() const
{
    // Anytime you add a new condition to this list, you also need to call
    // OnUnblockUI when the condition changes so that normal processing can
    // resume. For examples, see the places that we call OnUnblockUI.

    // NOTE: Do not add a check for SHWZCGetWiFiNotifications() to this function, we
    // still want ShowNewNetworksUI() called if the user has disabled available network notifications
    return (m_fOverride ||
        !m_fValidScan ||
        !m_fShellReady ||
        !m_intf.GetPowerState() ||
        m_nidConnecting.IsValid() ||
        IsConnected());
}

void CWZCManagerWireless::OnUnblockUI()
{
    // Post a message to check the UI block
    PostMessage(m_hwndSink, WM_WZC_CHECKUIBLOCK, 0, 0);
    m_fCheckUIBlock = TRUE;
}

void CWZCManagerWireless::OnUnblockUIPost()
{
    // Have all of our blocks cleared?
    if (m_fCheckUIBlock && !IsUIBlocked())
    {
        // Set this flag so we don't process multiple unblock messages
        m_fCheckUIBlock = FALSE;

        // Check for new networks
        CheckNewNetworks();
    }
}

void CWZCManagerWireless::DestroyUI()
{
    // Default processing
    CWZCManager::DestroyUI();

    // Clear the network list UI cache
    m_airing.Clear();
}

void CWZCManagerWireless::OnRemoveCard()
{
    // Clear the UI cache
    ClearStateCache();

    // Default processing
    CWZCManager::OnRemoveCard();
}

void CWZCManagerWireless::OnFindOwner(FINDOWNERINFO *pfoi) const
{
    // If we have a matching network, we're the owner
    if (pfoi->fFindNetID && NULL != FindNetwork(&pfoi->nid))
    {
        pfoi->hwnd = m_hwndSink;
        pfoi->fFindNetID = FALSE;
    }

    // If the owner hasn't been found, we're the owner
    else if (NULL == pfoi->hwnd)
    {
        pfoi->hwnd = m_hwndSink;
    }
}

HRESULT CWZCManagerWireless::OnFindAdapter(LPCTSTR pszAdapter, HWND *phwndSink) const
{
    HRESULT hr = S_OK;

    // Check inputs
    CBRAEx(NULL != pszAdapter, E_INVALIDARG);
    CBRAEx(NULL != phwndSink, E_INVALIDARG);

    // If we've already found an adapter, just ignore
    if (NULL != *phwndSink)
    {
        ASSERT(InvStrCmpI(pszAdapter, m_pszDeviceName));  // huh?? this adapter matches the requested one
        goto Exit;
    }

    // If this adapter matches the requested adapter, return our sink window
    // NOTE: An empty adapter name matches all adapters
    if (TEXT('\0') == pszAdapter[0] ||
        !InvStrCmpI(pszAdapter, m_pszDeviceName))
    {
        *phwndSink = m_hwndSink;
    }

Error:
Exit:
    return hr;
}

PCWLC CWZCManagerWireless::FindNetwork(const CNetID *pnid, DWORD *pdwFlags) const
{
    DWORD dwFlags = 0;
    PCWLC pwlc = NULL;
    PCWLC pwlcConnected;
    PCWLC pwlc2;
    DWORD dwNetFlags;

    ASSERT(NULL != pnid);

    // Get the current connected network
    pwlcConnected = GetConnectedNetwork();

    // Find the network in the preferred list
    dwNetFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_HIDDEN;
    pwlc = m_intf.Find(pnid, &dwNetFlags);
    if (NULL != pwlc)
    {
        dwFlags |= SHWZCF_NET_PREFERRED;
        if (pwlc == pwlcConnected)
        {
            dwFlags |= SHWZCF_NET_CONNECTED;
        }
    }

    // Find the network in the broadcast list
    dwNetFlags = SHWZCF_NET_BROADCAST | SHWZCF_NET_HIDDEN;
    pwlc2 = m_intf.Find(pnid, &dwNetFlags);
    if (NULL != pwlc2)
    {
        dwFlags |= SHWZCF_NET_BROADCAST;
        if (pwlc2 == pwlcConnected)
        {
            dwFlags |= SHWZCF_NET_CONNECTED;
        }

        if (NULL == pwlc)
        {
            pwlc = pwlc2;
        }
    }

    if (pdwFlags)
    {
        *pdwFlags = dwFlags;
    }

    return pwlc;
}

HRESULT CWZCManagerWireless::GetNetwork(PSHWZCNET pwzcnet, LPVOID pvData, UINT cbData) const
{
    HRESULT hr = S_OK;
    CNetID  nid;
    DWORD   dwNetFlags;
    PCWLC   pwlc, pwlcBroadcast = NULL;
    DEVICETYPE dtType;
    UINT uResult = 1;

    // Check inputs
    CBRAEx(NULL != pwzcnet, E_INVALIDARG);

    // Create a network ID
    hr = nid.Assign(pwzcnet);
    CHR(hr);

    // Query the current network status
    hr = m_intf.Query(INTF_STANDARD_QUERY);
    CHR(hr);

    // Find the network
    dwNetFlags = SHWZCF_NET_HIDDEN | (pwzcnet->dwFlags & (SHWZCF_NET_PREFERRED | SHWZCF_NET_BROADCAST));
    pwlc = m_intf.Find(&nid, &dwNetFlags);
    CBR(NULL != pwlc);

    // Copy the network data
    SHWZCEncodeNetwork(pwzcnet, pwlc);
    dtType = GetDeviceInfo(m_intf.m_sIntfEntry.wszGuid, NULL);

    // we found the network in the preferred list
    // need to get the Rssi value from the broadcast list
    if (dwNetFlags & SHWZCF_NET_PREFERRED)
    {
        dwNetFlags = SHWZCF_NET_HIDDEN|SHWZCF_NET_BROADCAST;
        pwlcBroadcast = m_intf.Find(&nid, &dwNetFlags);
    }
    else
    {
        pwlcBroadcast = pwlc;
    }

    if (pwlcBroadcast != NULL)
    {
        uResult = GetSignalStrength(pwlcBroadcast->Rssi, dtType);
    }

    pwzcnet->nRssi = uResult;
    
    pwzcnet->dwFlags &= ~(SHWZCF_NET_PREFERRED | SHWZCF_NET_BROADCAST);

    // If there's no data buffer, we actually want the size of the data
    if (0 == cbData)
    {
        UINT *pcbEapData = (UINT*)pvData;
        *pcbEapData = pwlc->EapolParams.dwAuthDataLen;
    }
    else
    {
        // Copy the data
        CBRA(cbData == pwlc->EapolParams.dwAuthDataLen);
        memcpy(pvData, pwlc->EapolParams.pbAuthData, cbData);
    }

Error:
    return hr;
}

HRESULT CWZCManagerWireless::ApplyNetworkList(__in CWZCInterface *pintf, DWORD dwApplyFlags)
{
    HRESULT hr = S_OK;
    CNetID  nid;
    PCWLC   pwlc;

    // Apply the list to the interface
    hr = pintf->Apply(dwApplyFlags | INTF_PREFLIST);
    CHR(hr);

    // Are there any preferred networks?
    if (pintf->GetConfigs(SHWZCF_NET_PREFERRED | SHWZCF_NET_HIDDEN, &pwlc))
    {
        // Act like we're already connecting to the first network in the list
        nid.Assign(pwlc);
    }

    // Assign the connecting network
    AssignConnectingNetwork(&nid);

    if(0 != m_uWifiTimeoutPeriod)
    {
        StartWifiTimeoutTimer();
    }

Error:
    return hr;
}

void CWZCManagerWireless::AssignConnectingNetwork(__in const CNetID *pnid)
{
    // Are we clearing the connecting network?
    if (!pnid->IsValid())
    {
        OnConnectStopped();
    }

    // Has the connecting network changed?
    else if (pnid->Compare(&m_nidConnecting, FALSE))
    {
        if (m_intf.GetPowerState())
        {
            m_nidConnecting = *pnid;

            SetHardwareFlag(SHWZCF_HW_CONNECTING, TRUE, WSF_CONNECTING);
            PostCallback(SHWZC_EVENT_CONNECTING, TRUE);
        }
    }
}

HRESULT CWZCManagerWireless::SetNetworks(const SETNETWORKHDR *psnh, const SETNETWORKITEM *psni, const BYTE *pbEapData)
{
    HRESULT  hr = S_OK;
    int      i;
    CNetID   nid;
    DWORD    dwNetFlags;
    PWLC     pwlc;
    WLC      wlcNew;
    SHWZCNET wzcnet;
    DWORD    dwApplyFlags = INTF_PREFLIST;

    // Get the current preferred list
    hr = m_intf.Query(INTF_STANDARD_QUERY);
    CHR(hr);

    // We need to add the networks in reverse order so that the preference
    // order is maintained. Move the EAP data pointer to the end so we can
    // iterate through it backwards
    for (i = 0; (UINT)i != psnh->cNetworks; ++i)
    {
        if ((UINT)-1 != psni[i].cbEapData)
        {
            pbEapData += psni[i].cbEapData;
        }
    }

    // Add each of the networks to the list
    for (i = psnh->cNetworks - 1; i >= 0; --i)
    {
        BOOL fSetKey = FALSE;

        // Try to find the network in the list
        CHRA(nid.Assign(&psni[i].wzcnet));
        dwNetFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_BROADCAST | SHWZCF_NET_HIDDEN;
        pwlc = (PWLC)m_intf.Find(&nid, &dwNetFlags);

        // If the network doesn't already exist, create a new one
        if (NULL == pwlc)
        {
            ZeroMemory(&wlcNew, sizeof(WLC));
            wlcNew.Length = sizeof(WLC);

            CHRA(StringCchCopy(wzcnet.szName, ARRAYSIZE(wzcnet.szName), psni[i].wzcnet.szName));
            SHWZCGetDefaults(&wzcnet, BOOLIFY(SHWZCF_NET_ADHOC & psni[i].wzcnet.dwFlags));
            CHRA(SHWZCDecodeNetwork(&wlcNew, &wzcnet, NULL));
            pwlc = &wlcNew;
        }
        else
        {
            // Translate the network into an SHWZCNET
            SHWZCEncodeNetwork(&wzcnet, pwlc);
        }

        // Apply the changes
        CHRA(SHWZCCopySettings(&wzcnet, &psni[i].wzcnet, psni[i].wMask));
        if (SHWZCF_SETMASK_KEYDATA & psni->wMask)
        {
            // Remove the current key
            pwlc->dwCtlFlags &= ~WZCCTL_WEPK_PRESENT;
            pwlc->KeyLength = 0;
            memset (pwlc->KeyMaterial, 0, sizeof(pwlc->KeyMaterial));

            // Should we set a new key?
            fSetKey = ((SHWZCF_NET_KEYDATA & psni[i].wzcnet.dwFlags) && psni[i].szKey[0]);
        }

        // Set the network data
        hr = SHWZCDecodeNetwork(pwlc, &wzcnet, fSetKey ? psni[i].szKey : NULL);
        CHR(hr);

        // Copy the EAP data
        if (SHWZCF_SETMASK_8021X & psni[i].wMask)
        {
            // Remove the current data
            WZCFreeConfig(pwlc);

            // Add new data if it's available
            if (0 != psni[i].cbEapData)
            {
                DWORD cbData = psni[i].cbEapData;

                // This data will get copied when we call SetMostPreferred
                pwlc->EapolParams.pbAuthData = (LPBYTE)(pbEapData -= cbData);
                pwlc->EapolParams.dwAuthDataLen = cbData;
            }
        }

        // Push this network to the front of the preferred list
        hr = m_intf.SetMostPreferred(pwlc);
        CHR(hr);
    }

    // If this is the complete state, we delete any networks we didn't just set
    if (psnh->fComplete)
    {
        CHRA(m_intf.SetPreferredCount(psnh->cNetworks));
    }

    // Do we need to set the state as well?
    if (psnh->fSetState)
    {
        hr = m_intf.SetFilter(psnh->dwState, FALSE);
        CHR(hr);
        dwApplyFlags |= INTF_CM_MASK | INTF_FALLBACK;
    }

    // Save the network information
    hr = ApplyNetworkList(&m_intf, dwApplyFlags);
    CHR(hr);

Error:
    return hr;
}

HRESULT CWZCManagerWireless::DeleteNetwork(const CNetID *pnid, BOOL fKeyError)
{
    HRESULT hr = S_OK;

    // Check inputs
    CBRAEx(NULL != pnid, E_INVALIDARG);

    // Get the current preferred list
    hr = m_intf.Query(INTF_PREFLIST);
    CHR(hr);

    // Ask the user to update the key
    if (fKeyError)
    {
        DWORD dwNetFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_HIDDEN;
        PCWLC pwlc = m_intf.Find(pnid, &dwNetFlags);
        if (NULL != pwlc)
        {
            DWORD dwPasswordState = GetPasswordState(pwlc);
            if ((DWORD)-1 != dwPasswordState)
            {
                // Copy this configuration
                WZCFreeConfig(&m_wlcKey);
                hr = WZCCopyConfigs(&m_wlcKey, pwlc, 1, TRUE);
                CHR(hr);

                // Show the password dialog
                hr = ShowPasswordDialog((dwPasswordState & PWDUI_WPA) | PWDUI_ERR_GENERIC);
                CHR(hr);
            }
        }
    }

    // Remove the network
    hr = m_intf.RemovePreferred(pnid);
    CHR(hr);

    if (S_OK == hr)  // S_FALSE means the network wasn't in the list
    {
        // Apply the changes
        hr = ApplyNetworkList(&m_intf);
        CHR(hr);
    }

    // Remove the network from the preset list
    DeleteRegistryPreset(pnid);

Error:
    return hr;
}

HRESULT CWZCManagerWireless::DeleteAllNetworks()
{
    HRESULT hr = S_OK;

    // Query the current list
    hr = m_intf.Query(INTF_PREFLIST);
    CHR(hr);

    // Remove them from the interface data
    hr = m_intf.RemoveAllPreferred();
    CHR(hr);

    if (S_OK == hr)  // S_FALSE means there were none to remove
    {
        // Apply the changes
        hr = ApplyNetworkList(&m_intf);
        CHR(hr);
    }

Error:
    return hr;
}

void CWZCManagerWireless::ForceDisconnect()
{
    // Make sure we're not connected. This is an outstanding issue that
    // needs to be resolved. Right now, the only way to do this is to remove
    // the currently connected network from the preferred list, but that isn't
    // really what we want. We need to get this capability from EDG.
    ASSERT(FALSE);
}

HRESULT CWZCManagerWireless::ForceReconnect()
{
    HRESULT hr = S_OK;

    // Get the current preferred list
    hr = m_intf.Query(INTF_PREFLIST);
    CHR(hr);

    // Reapply it
    hr = ApplyNetworkList(&m_intf);
    CHR(hr);

Error:
    return hr;
}

PCWLC CWZCManagerWireless::GetConnectedNetwork() const
{
    return (IsConnected() ? m_intf.GetConnected() : NULL);
}

static HRESULT CollapseNetworks(HDSA hdsa, __in_ecount(cConfigs) PCWLC pwlc, UINT cConfigs, DWORD dwFlags, DEVICETYPE dtType, LPCTSTR pszAdapter)
{
    HRESULT hr = S_OK;
    PCWLC   pwlcEnd = pwlc + cConfigs;
    UINT    cInit = DSA_GetItemCount(hdsa);
    CNetID  nid;
    int     nIndex;
    PSHWZCNET pshwzcnet;

    for (; pwlc != pwlcEnd; ++pwlc)
    {
        // Try to find the network in the list
        nid.Assign(pwlc);
        nIndex = FindNetwork(&nid, (PCSHWZCNET)DSA_GetItemPtr(hdsa, 0), cInit, sizeof(SHWZCNET));

        // If it doesn't currently exist, add it
        if (-1 == nIndex)
        {
            SHWZCNET wzcnet = { 0 };

            SHWZCEncodeNetwork(&wzcnet, pwlc);
            nIndex = DSA_AppendItem(hdsa, &wzcnet);
            CBREx(0 <= nIndex, E_OUTOFMEMORY);
        }

        pshwzcnet = ((PSHWZCNET)DSA_GetItemPtr(hdsa, nIndex));
        if (dwFlags & SHWZCF_NET_BROADCAST)
        {
            pshwzcnet->nRssi = CWZCManagerWireless::GetSignalStrength(pwlc->Rssi, dtType);
        }

        if ((dwFlags & SHWZCF_NET_CONNECTED) && !(pshwzcnet->dwFlags & SHWZCF_NET_BROADCAST))
        {
            pshwzcnet->nRssi = CWZCManagerWireless::GetSignalStrength(pszAdapter);
        }

        // Set the flags
        pshwzcnet->dwFlags |= dwFlags;
    }

Error:
    return hr;
}

HRESULT CWZCManagerWireless::GetCurrentNetworks(HDSA hdsa, DWORD dwFlags) const
{
    HRESULT hr = S_OK;
    PCWLC   pwlc;
    UINT    cConfigs;
    DEVICETYPE dtType;

    // Check inputs
    CBRAEx(NULL != hdsa, E_INVALIDARG);

    // Query the device driver
    hr = m_intf.Query(INTF_STANDARD_QUERY);
    CHR(hr);

    dtType = GetDeviceInfo(m_intf.m_sIntfEntry.wszGuid, NULL);

    // Add preferred networks
    if (SHWZCF_NET_PREFERRED & dwFlags)
    {
        cConfigs = m_intf.GetConfigs(SHWZCF_NET_PREFERRED | SHWZCF_NET_HIDDEN, &pwlc);
        hr = CollapseNetworks(hdsa, pwlc, cConfigs, SHWZCF_NET_PREFERRED, dtType, m_pszDeviceName);
        CHR(hr);
    }

    // Add broadcast networks
    if (SHWZCF_NET_BROADCAST & dwFlags)
    {
        cConfigs = m_intf.GetConfigs(SHWZCF_NET_BROADCAST | SHWZCF_NET_HIDDEN, &pwlc);
        hr = CollapseNetworks(hdsa, pwlc, cConfigs, SHWZCF_NET_BROADCAST, dtType, m_pszDeviceName);
        CHR(hr);
    }

    // Add the connected network
    if ((SHWZCF_NET_CONNECTED & dwFlags) &&
        NULL != (pwlc = GetConnectedNetwork()))
    {
        hr = CollapseNetworks(hdsa, pwlc, 1, SHWZCF_NET_CONNECTED, dtType, m_pszDeviceName);
        CHR(hr);
    }

    // Add the connecting network -- only if we're not already connected
    else if ((SHWZCF_NET_CONNECTING & dwFlags) &&
        m_nidConnecting.IsValid())
    {
        DWORD dwNetFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_BROADCAST | SHWZCF_NET_HIDDEN;
        pwlc = m_intf.Find(&m_nidConnecting, &dwNetFlags);
        if (NULL != pwlc)
        {
            hr = CollapseNetworks(hdsa, pwlc, 1, SHWZCF_NET_CONNECTING, dtType, m_pszDeviceName);
            CHR(hr);
        }
    }

Error:
    return hr;
}

void CWZCManagerWireless::OnDestroy()
{
    // Default processing
    CWZCManager::OnDestroy();

    if(m_hWifiTimeoutRegNotify != NULL)
    {
        RegistryCloseNotification(m_hWifiTimeoutRegNotify);
        m_hWifiTimeoutRegNotify = NULL;
    }
}

DWORD PowerOffThread(LPVOID lpVoid)
{
    LPTSTR szDeviceName = (LPTSTR)lpVoid;

    ASSERT(lpVoid);
    
    SetDevicePower(szDeviceName, POWER_NAME, D4);
    LocalFree(szDeviceName);
    return 0;
}

HRESULT CWZCManagerWireless::SetDefaultPowerState()
{
    HRESULT hr = S_OK;
    DWORD dwPrefState = 1;
    DWORD dwDisable = 0;
    LPTSTR pszBuffer = NULL;

    pszBuffer = (LPTSTR)LocalAlloc(LMEM_FIXED, MAX_PATH * sizeof(TCHAR));
    CPR(pszBuffer);

    // Create Comm registry path for this device
    // Allow OEM who have done workarounds to disable default power control
    // [HKLM\\Comm\\"DeviceName"\\Parms] DisableDefaultPowerControl
    hr = StringCchPrintf(pszBuffer, MAX_PATH, c_szDefaultPowerControlTemplate,
        m_pszDeviceName);
    CHRA(hr);  // the device name is too long!!
    RegistryGetDWORD(HKEY_LOCAL_MACHINE, pszBuffer, c_szRegValueDisableDefaultPowerControl, &dwDisable);

    // Create the power manager registry value name for this device
    hr = StringCchPrintf(pszBuffer, MAX_PATH, c_szPowerMgrDeviceTemplate,
        m_pszDeviceName);
    CHRA(hr);  // the device name is too long!!
    RegistryGetDWORD(HKEY_LOCAL_MACHINE, c_szRegPathPowerMgrState, pszBuffer, &dwPrefState);

    CBREx(dwDisable == 0, S_FALSE);

    if(dwPrefState == 0)
    {
        HANDLE hThread = CreateThread(NULL, 0, PowerOffThread, pszBuffer, 0, NULL);
        if (hThread)
        {
            pszBuffer = NULL;
            CloseHandle(hThread);
        }
    }
Error:
    LocalFree(pszBuffer);
    return hr;
}

HRESULT CWZCManagerWireless::OnPowerChange(BOOL fOn)
{
    HRESULT              hr = S_OK;

    // Check the device power state
    CBREx(m_intf.GetPowerState() != fOn, S_FALSE);

    // Power status has changed
    m_intf.SetPowerState(fOn);
    ClearStateCache();
    m_fValidScan = FALSE;

    // Process the event
    hr = fOn ? OnPowerOn() : OnPowerOff();
    CHR(hr);

    SetHardwareFlag(SHWZCF_HW_ON, fOn, WSF_POWER_ON);

Error:
    return hr;
}

HRESULT CWZCManagerWireless::OnPowerOn()
{
    HRESULT       hr = S_OK;
    CWZCInterface intf(m_pszDeviceName);

    // Clear the dismissed list
    m_nidlDismissed.Reset();

    // Unblock the UI
    OnUnblockUI();

    // Reset the WZC state (ensure that the scanning thread is running, etc)
    hr = intf.Refresh(INTF_ALL);
    CHR(hr);

    // Reconnect to the preferred list
    hr = ForceReconnect();
    CHR(hr);

Error:
    return hr;
}

HRESULT CWZCManagerWireless::OnPowerOff()
{
    HRESULT hr = S_OK;

    // The device is now disconnected
    OnConnectionStatusUpdate(FALSE);
    OnConnectStopped();

    // Remove any current UI
    DestroyUI();

//Error:
    return hr;
}

UINT CWZCManagerWireless::GetSignalStrength(int nRSSI, DEVICETYPE dtType)
{
    UINT uResult;

    // This will give us a range from 0 to 5. For UI, we treat both values
    // 0 and 1 as 1 bar, and both 4 and 5 as 4 bars.
    uResult = CWZCInterface::GetSignalStrength(nRSSI, dtType);
    if (uResult < 1)
    {
        uResult = 1;
    }
    else if (uResult > 4)
    {
        uResult = 4;
    }
    return uResult;
}

UINT CWZCManagerWireless::GetSignalStrength(__in LPCTSTR pszAdapter)
{
    HRESULT    hr = S_OK;
    DEVICETYPE dtType;
    int        nRSSI = 0;
    UINT       uResult = 1;

    // Get the signal strength
    dtType = GetDeviceInfo(pszAdapter, &nRSSI);
    CBR(DEVICE_WIRED != dtType);  // can happen if device is powered off

    uResult = GetSignalStrength(nRSSI, dtType);

Error:
    return uResult;
}

BOOL CWZCManagerWireless::IsEncrypted(ULONG ulPrivacy)
{
    // The three musketeers
    return (Ndis802_11Encryption1Enabled == ulPrivacy ||  // WEP
        Ndis802_11Encryption2Enabled == ulPrivacy ||      // TKIP
        Ndis802_11Encryption3Enabled == ulPrivacy);       // AES
}

HRESULT CWZCManagerWireless::LoadCallbackWindow(HWND hwnd)
{
    HRESULT hr = S_OK;
    HKEY    hkey = NULL;
    TCHAR   szBuffer[32];
    DWORD   dwType;
    CBINFO  cbi;
    DWORD   cbData;

    // Open the registry key
    CBR(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_szRegPathCallbacks, 0, 0, &hkey));

    // Create the value name
    hr = StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), g_szValueFormat, hwnd);
    CHR(hr);

    // Query the value
    cbData = sizeof(CBINFO);
    CBR(ERROR_SUCCESS == RegQueryValueEx(hkey, szBuffer, NULL, &dwType, (LPBYTE)&cbi, &cbData) &&
        REG_BINARY == dwType &&
        sizeof(CBINFO) == cbData);

    // Register the callback
    hr = RegisterCallbackWindow(hwnd, &cbi, TRUE);
    CHR(hr);

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

HRESULT CWZCManagerWireless::LoadRegistryData()
{
    HRESULT     hr = S_OK;
    DWORD       dwScanInterval = 0;
    DWORD       cbData = 0;
    DWORD       fScanConnected = 0;
    HKEY        hkeySecure = NULL;
    HKEY        hkeyCPL = NULL;
    HKEY        hkeyCB = NULL;
    WZC_CONTEXT ctxt = { 0 };
    BOOL        fUpdateInterval = FALSE;

    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_szRegKeyWZCSecure, 0, 0, &hkeySecure))
    {
        // Load the default scan interval from secure registry
        cbData = sizeof(dwScanInterval);
        RegQueryValueEx(hkeySecure, g_szRegValueScanInterval, NULL, NULL, (LPBYTE)&dwScanInterval, &cbData);
    }

    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_szRegKeyBase, 0, 0, &hkeyCPL))
    {
        // Determine whether we should also scan while the network is connected
        cbData = sizeof(fScanConnected);
        RegQueryValueEx(hkeyCPL, g_szRegValueScanConnected, NULL, NULL, (LPBYTE)&fScanConnected, &cbData);

        // Load the current callback windows
        if (ERROR_SUCCESS == RegOpenKeyEx(hkeyCPL, g_szRegKeyCallbacks, 0, 0, &hkeyCB))
        {
            DWORD  iValue = 0;
            TCHAR  szName[64];
            DWORD  cchName;
            LPTSTR pszEndName;
            CBINFO cbi = { 0 };
            DWORD  dwType;
            HWND   hwnd;

            for(;;)
            {
                // Query the value
                cchName = ARRAYSIZE(szName);
                cbData = sizeof(CBINFO);
                if (ERROR_SUCCESS != RegEnumValue(hkeyCB, iValue++, szName, &cchName, NULL, &dwType,
                    (LPBYTE)&cbi, &cbData))
                {
                    break;
                }

                // Convert the value name into an HWND
                hwnd = (HWND)_tcstoul(szName, &pszEndName, 16);
                if (REG_BINARY == dwType &&
                    sizeof(CBINFO) == cbData && 
                    NULL != hwnd &&
                    cchName == pszEndName - szName &&
                    IsWindow(hwnd))
                {
                    // Register the callback window
                    RegisterCallbackWindow(hwnd, &cbi, FALSE);
                    fUpdateInterval = TRUE;
                }
                else
                {
                    // The registry value is invalid - delete it
                    RegDeleteValue(hkeyCB, szName);
                }
            }
        }
    }

    // Notify that we've got a new adapter
    PostCallback(SHWZC_EVENT_ADAPTER, TRUE);

    // Get the current device context settings
    hr = WZCQueryContext(NULL, 0, &ctxt, NULL);
    CHR(hr);

    // Save the rescan timeout
    m_dwRescanTimeout = ctxt.tmTr;

    // Set the regular scan interval based on the registry settings
    ctxt.tmTf = dwScanInterval ? dwScanInterval * 1000 : TMMS_INFINITE;
    ctxt.tmTc = fScanConnected ? ctxt.tmTf : TMMS_INFINITE;
    hr = WZCSetContext(NULL, 0, &ctxt, NULL);
    CHR(hr);

    // Update the scan interval if necessary
    if (fUpdateInterval)
    {
        hr = UpdateScanInterval(FALSE);
        CHR(hr);
    }

    // Load the network presets
    hr = LoadRegistryPresets();
    CHR(hr);

Error:
    if (NULL != hkeySecure)
    {
        RegCloseKey(hkeySecure);
    }

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

typedef HRESULT (*PRESETENUMPROC)(HKEY hkeyParent, __in LPCTSTR pszKeyName, void *pvParam);
static HRESULT EnumRegistryPresets(__in PRESETENUMPROC proc, void *pvParam)
{
    HRESULT hr = S_OK;
    LONG    lr;
    HKEY    hkey = NULL;
    DWORD   iChild;
    TCHAR   szKeyName[MAX_PATH];
    DWORD   cchKeyName;

    // Open the preset registry key
    lr = RegOpenKeyEx(HKEY_LOCAL_MACHINE, c_szRegKeyWZCPresets, 0, 0, &hkey);
    CBREx(ERROR_SUCCESS == lr, S_FALSE);

    // Enumerate each child key
    for (iChild = 0, cchKeyName = ARRAYSIZE(szKeyName);
        ERROR_SUCCESS == RegEnumKeyEx(hkey, iChild, szKeyName, &cchKeyName, NULL, NULL, NULL, NULL);
        ++iChild, cchKeyName = ARRAYSIZE(szKeyName))
    {
        // Skip keys that don't start with "preset"
        if (InvStrCmpNI(szKeyName, c_szRegKeyPresetPrefix, ARRAYSIZE(c_szRegKeyPresetPrefix)-1))
        {
            continue;
        }

        // Enumerate the key
        hr = proc(hkey, szKeyName, pvParam);
        CHR(hr);

        // If the callback returns S_FALSE, we can stop enumeration
        if (S_FALSE == hr)
        {
            break;
        }
    }

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

static HRESULT OpenPresetKey(HKEY hkeyParent, __in LPCTSTR pszKeyName,
                             __out HKEY *phkey, __out CNetID *pnid)
{
    HRESULT hr = S_OK;
    LONG    lr;
    HKEY    hkey = NULL;
    TCHAR   szName[ARRAYSIZE(((PSHWZCNET)NULL)->szName)];
    DWORD   cbData;
    DWORD   dwType;

    // Open the registry key
    lr = RegOpenKeyEx(hkeyParent, pszKeyName, 0, 0, &hkey);
    if (ERROR_SUCCESS != lr)
    {
        DEBUGMSG(ZONE_WARN, (TEXT("Wi-Fi: Registry Preset: Could not open %s [err: %d]\r\n"), pszKeyName, lr));
        CBR(FALSE);
    }

    // Read the network name
    cbData = sizeof(szName);
    lr = RegQueryValueEx(hkey, c_szRegValueSSID, NULL, &dwType, (BYTE*)szName, &cbData);
    CBR(ERROR_SUCCESS == lr && REG_SZ == dwType);

    // Create the network ID (all presets are access point networks)
    hr = pnid->Assign(szName, FALSE);
    CHR(hr);

    // Return the key
    *phkey = hkey;
    hkey = NULL;

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

struct PRESETLOADINFO
{
    HDSA          hdsa;
    CWZCInterface *pintf;
};

static HRESULT LoadRegistryPreset(HKEY hkeyParent, __in LPCTSTR pszKeyName, void *pvParam)
{
    HRESULT        hr = S_OK;
    PRESETLOADINFO *ppli = (PRESETLOADINFO*)pvParam;
    HKEY           hkey = NULL;
    CNetID         nid;
    DWORD          dwNetFlags;
    SETNETWORKITEM sni;
    LONG           lr;
    DWORD          dwType;
    DWORD          cbData;
    DWORD          dwValue;

    // Open the preset key
    CBREx(SUCCEEDED(OpenPresetKey(hkeyParent, pszKeyName, &hkey, &nid)), S_FALSE);

    // Initialize the output if necessary
    if (NULL == ppli->hdsa)
    {
        // Create the network list
        ppli->hdsa = DSA_Create(sizeof(SETNETWORKITEM), 4);
        CWR(NULL != ppli->hdsa);

        // Query the current preferred network list
        hr = ppli->pintf->Query(INTF_PREFLIST);
        CHR(hr);
    }

    // If this network is already in the preferred list, we don't add it
    dwNetFlags = SHWZCF_NET_PREFERRED;
    CBREx(NULL == ppli->pintf->Find(&nid, &dwNetFlags), S_FALSE);

    // Copy the network name
    ZeroMemory(&sni, sizeof(SETNETWORKITEM));
    hr = nid.GetDisplayName(sni.wzcnet.szName, ARRAYSIZE(sni.wzcnet.szName));
    CHRAEx(hr, S_FALSE);

    // Read the authentication mode
    cbData = sizeof(dwValue);
    lr = RegQueryValueEx(hkey, c_szRegValueAuthentication, NULL, &dwType, (BYTE*)&dwValue, &cbData);
    if (ERROR_SUCCESS == lr && REG_DWORD == dwType)
    {
        sni.wzcnet.dwAuthentication = dwValue;
        sni.wMask |= SHWZCF_SETMASK_AUTHENTICATION;
    }

    // Read the encryption mode
    cbData = sizeof(dwValue);
    lr = RegQueryValueEx(hkey, c_szRegValuePrivacy, NULL, &dwType, (BYTE*)&dwValue, &cbData);
    if (ERROR_SUCCESS == lr && REG_DWORD == dwType)
    {
        sni.wzcnet.dwEncryptionType = dwValue;
        sni.wMask |= SHWZCF_SETMASK_ENCRYPTION;
    }

    // Read the 802.1x flag
    cbData = sizeof(dwValue);
    lr = RegQueryValueEx(hkey, c_szRegValueEnable8021x, NULL, &dwType, (BYTE*)&dwValue, &cbData);
    if (ERROR_SUCCESS == lr && REG_DWORD == dwType && dwValue)
    {
        // Read the EAP plug-in ID
        cbData = sizeof(dwValue);
        lr = RegQueryValueEx(hkey, c_szRegValueEapType, NULL, &dwType, (BYTE*)&dwValue, &cbData);
        if (ERROR_SUCCESS == lr && REG_DWORD == dwType)
        {
            sni.wzcnet.dwFlags |= SHWZCF_NET_8021X;
            sni.wzcnet.dwEAPType = dwValue;
            sni.wMask |= SHWZCF_SETMASK_8021X;
        }
    }

    // Read the Hidden flag if there is one
    cbData = sizeof(dwValue);
    lr = RegQueryValueEx(hkey, c_szRegValueHidden, NULL, &dwType, (BYTE*)&dwValue, &cbData);
    if (ERROR_SUCCESS == lr && REG_DWORD == dwType && dwValue)
    {
        // Network has been explicitly marked as hidden - WZC will need to probe for it to
        // be able to connect since the access point will not broadcast this SSID
        sni.wzcnet.dwFlags |= SHWZCF_NET_REQUIRE_WZC_PROBE;
        sni.wMask |= SHWZCF_SETMASK_REQUIRE_WZC_PROBE;
    }      

    // Append the valid configuration to our list
    DEBUGMSG(TRUE, (TEXT("Wi-Fi: Registry Preset: Successfully loaded %s\r\n"), pszKeyName));
    CBREx(0 <= DSA_AppendItem(ppli->hdsa, &sni), E_OUTOFMEMORY);

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    // Always continue enumeration unless we failed
    return SUCCEEDED(hr) ? S_OK : hr;
}

HRESULT CWZCManagerWireless::LoadRegistryPresets()
{
    HRESULT        hr = S_OK;
    PRESETLOADINFO pli = { 0 };
    SETNETWORKHDR  snh;

    // Enumerate the presets
    pli.pintf = &m_intf;
    hr = EnumRegistryPresets(LoadRegistryPreset, &pli);
    CHR(hr);

    // Did we get any presets?
    CBREx(NULL != pli.hdsa, S_FALSE);

    // Create the structure header
    ZeroMemory(&snh, sizeof(SETNETWORKHDR));
    snh.cNetworks = DSA_GetItemCount(pli.hdsa);
    CBREx(0 != snh.cNetworks, S_FALSE);

    // Save the networks
    hr = SetNetworks(&snh, (const SETNETWORKITEM*)DSA_GetItemPtr(pli.hdsa, 0), NULL);
    CHR(hr);

Error:
    if (NULL != pli.hdsa)
    {
        DSA_Destroy(pli.hdsa);
    }

    return hr;
}

static HRESULT DeleteRegistryPresetMatch(HKEY hkeyParent, __in LPCTSTR pszKeyName, void *pvParam)
{
    HRESULT hr = S_OK;
    HKEY    hkey = NULL;
    CNetID  nid;
    LONG    lr;

    // Is this the key we're looking for?
    if (SUCCEEDED(OpenPresetKey(hkeyParent, pszKeyName, &hkey, &nid)) &&
        0 == nid.Compare((const CNetID*)pvParam, FALSE))
    {
        // Close the key so we can delete it
        RegCloseKey(hkey);
        hkey = NULL;

        // Delete the key
        lr = RegDeleteKey(hkeyParent, pszKeyName);
        if (ERROR_SUCCESS != lr)
        {
            DEBUGMSG(ZONE_WARN, (TEXT("Wi-Fi: Registry Preset: Could not delete %s [err: %d]\r\n"), pszKeyName, lr));
            CBR(FALSE);
        }

        // Stop enumeration
        hr = S_FALSE;
    }

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

HRESULT CWZCManagerWireless::DeleteRegistryPreset(__in const CNetID *pnid)
{
    HRESULT hr = S_OK;

    // We don't need to delete ad-hoc networks, since all presets are access
    // point networks
    CBREx(Ndis802_11IBSS != pnid->InfrastructureMode, S_FALSE);

    // Enumerate the presets and try to find a match
    hr = EnumRegistryPresets(DeleteRegistryPresetMatch, (void*)pnid);
    CHR(hr);

    // S_FALSE means the enumeration was cancelled, which means we found a
    // match -- confusing eh?
    hr = (S_FALSE == hr ? S_OK : S_FALSE);

Error:
    return hr;
}

// We need to get out of this function ASAP, since the notification thread has
// SENT us this message, not posted it. Don't do ANYTHING here that could cause
// this window to go away or that thread to be destroyed, since that would
// cause a deadlock.
HRESULT CWZCManagerWireless::OnConnectBegin(const CNetID *pnid)
{
    HRESULT hr = S_OK;
    CNetID  nid;

    // Check inputs
    CBREx(NULL != pnid, E_INVALIDARG);

    // If we're currently connected to this network, we'll ignore the
    // connecting message, since sometimes the hardware will just stay
    // connected and we'll never get a "connected" message.
    if (!IsConnected() ||
        FAILED(m_intf.GetConnected(&nid)) ||
        nid.Compare(pnid, FALSE))
    {
        // Save this as our connecting network
        AssignConnectingNetwork(pnid);
        TryConfigMessage(WZC_CM_CONNECT_RESULT, S_FALSE);
    }

    // Don't put up UI until we exhaust the preferred list
    m_fValidScan = FALSE;

Error:
    return hr;
}

///<summary>Sets the EAP type of pnid to the GetEAPIncremented() value if pnid is the first network in the preferred list
///and the 8021x authentication state of this object is WZC_AUTHSTATE8021X_ASSOCIATED.</summary>
///<param name="pnid">The network that just received a WZC_CLOSE_EAPOL_SESSION message</param>
///<remarks>We need to get out of this function ASAP, since the notification thread has
/// SENT us this message, not posted it. Don't do ANYTHING here that could cause
/// this window to go away or that thread to be destroyed, since that would
/// cause a deadlock.</remarks>
HRESULT CWZCManagerWireless::OnEapolClose(const CNetID *pnid)
{
    HRESULT hr = S_OK;
    PWLC    pwlc;
    PCWLC pwlcFirst;
    DWORD   dwNetFlags;

    // Sanity check
    CBREx(NULL != pnid, E_INVALIDARG);

    CBREx(WZC_AUTHSTATE8021X_ASSOCIATED == m_asCurrent8021XState, S_FALSE);

    // Find the configuration in the current preferred list
    hr = m_intf.Query(INTF_PREFLIST);
    CHR(hr);

    dwNetFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_HIDDEN;
    pwlc = (PWLC)m_intf.Find(pnid, &dwNetFlags);
    CBREx(NULL != pwlc, E_INVALIDARG);

    // If this is a WPA[2]-PSK network, we're not really using 802.1x, just goto Exit
    if (IsAnyWPAPSK(pwlc->AuthenticationMode))
    {
        goto Exit;
    }

    // If the network isn't the first network in the list, then most likely the EAP session 
    // closed because the user manually switched networks. Don't change the EAP
    // type for those cases.
    m_intf.GetConfigs(SHWZCF_NET_PREFERRED, &pwlcFirst);
    CBREx(pwlc == pwlcFirst, S_FALSE);

    // Increment the EAP type
    hr = GetEAPIncremented(pwlc);
    CHR(hr);

    // Update the configuration
    RETAILMSG(1, (TEXT("Wi-Fi: Trying next EAP type for '%S'...\r\n"), pnid->Ssid));
    hr = ApplyNetworkList(&m_intf);
    CHR(hr);

Error:
Exit:
    m_asCurrent8021XState = WZC_AUTHSTATE8021X_RESET;
    return hr;
}

/// <summary>
/// Respond to an authentication failure notification from WZC
/// </summary>
/// <returns>
/// HRESULT
///  S_OK on success
///  Failure code otherwise
/// </returns>
/// <param name="hrConnect">
/// Connect result
/// </param>
/// <param name="pnid">
/// CNetId corresponding to the network that WZC was trying to connect to
/// </param>
/// <remarks>
/// On authentication failure, we either pop up a password retry dialog so that the user can try
/// to connect again, or simply delete network settings and end the connection process.
/// </remarks>
HRESULT CWZCManagerWireless::OnAuthenticationFailure(HRESULT hrConnect, CNetID *pnid)
{
    HRESULT hr = S_OK;
    DWORD dwNetFlags;
    PCWLC pwlc = NULL; 

    // Check inputs
    CBREx(NULL != pnid, E_INVALIDARG);
    
    // Get the current preferred list
    hr = m_intf.Query(INTF_PREFLIST);
    CHR(hr);

    dwNetFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_HIDDEN;
    pwlc = (PWLC)m_intf.Find(pnid, &dwNetFlags);
    CBREx(NULL != pwlc, E_INVALIDARG);

    // SHWZC_E_INVALID corresponds to a WZC_FAILED_AUTH_NO_RETRY message from WZC.
    // In the case of WPA-PSK networks, this is sent if an incorrect key is entered. 
    if ((SHWZC_E_INVALID == hrConnect) &&
        IsAnyWPAPSK(pwlc->AuthenticationMode))
    {
        // Ask the user for another key
        VHR(DeleteNetwork(pnid, TRUE));
    }
    else
    { 
        // Not a WPA-PSK network - don't allow retry and instead just delete the network
        // from the preferred list
        VHR(DeleteNetwork(pnid, FALSE));
        // End of connection process - post the connect result
        PostConfigMessage(WZC_CM_CONNECT_RESULT, (LPARAM)hrConnect);
    }
    
Error:
    LocalFree(pnid);
    return hr;
}

// We need to get out of this function ASAP, since the notification thread has
// SENT us this message, not posted it. Don't do ANYTHING here that could cause
// this window to go away or that thread to be destroyed, since that would
// cause a deadlock.
HRESULT CWZCManagerWireless::OnConnectCompleteSent(HRESULT hrConnect, const CNetID *pnid)
{
    HRESULT hr = S_OK;

    // Check inputs
    CBREx(NULL != pnid, E_INVALIDARG);

    // If we have completely invalid data, we remove the connection from the preferred list
    if (FACILITY_WZC == HRESULT_FACILITY(hrConnect) &&
        SHWZC_E_CANCELAUTH != hrConnect)
    {
        // Remove the network from the preferred list
        VHR(DeleteNetwork(pnid, FALSE));
    }

    // Post a notification and let the notification thread get back to its business
    PostConfigMessage(WZC_CM_CONNECT_RESULT, (LPARAM)hrConnect);

Error:
    return hr;
}

void CWZCManagerWireless::OnConnectResult(HRESULT hrConnect)
{
    BOOL fConnectedInit = IsConnected();
    BOOL fConnected;

    // Default processing
    CWZCManager::OnConnectResult(hrConnect);

    // Has our connection status changed?
    fConnected = IsConnected();
    if (fConnectedInit != fConnected)
    {
        // Clear the airing list. Otherwise, we may not put up new UI after we
        // disconnect, since the airing networks may be the same as before we
        // connected.
        m_airing.Clear();

        // If we've successfully connected, we've completed the connection
        // attempt. If we never manage to connect, we'll call this when we get
        // WM_WZC_CONNECT_STOPPED.
        if (fConnected)
        {
            OnConnectStopped();
        }

        // Our connection status changed... update any override UI
        PostCallback(SHWZC_EVENT_CONNECTION, fConnected);
    }
}

void CWZCManagerWireless::OnConnectStopped()
{
    // TODO: Move the contents of this function into AssignConnectingNetwork so
    // that all of our "connecting" messages are centralized in one place.
    if (m_nidConnecting.IsValid())
    {
        // Reset the connecting network ID
        m_nidConnecting.Reset();

        // Notify the Shell and callbacks
        SetHardwareFlag(SHWZCF_HW_CONNECTING, FALSE, WSF_CONNECTING);
        PostCallback(SHWZC_EVENT_CONNECTING, FALSE);

        // We cleared the connecting NetID
        OnUnblockUI();
    }
}

BOOL CWZCManagerWireless::CheckIPState(IPSTATE state)
{
    CWZCInterface intf(m_pszDeviceName);
    CNetID        nid;
    BOOL          fAdhoc;
    BOOL          fValid;

    // Get the currently connected network
    intf.GetConnected(&nid);

    // We only treat an auto-assigned IP as valid in adhoc mode
    fAdhoc = (Ndis802_11IBSS == intf.GetInfraMode());
    fValid = (state >= (fAdhoc ? IPSTATE_AUTOIP : IPSTATE_CONNECTED));
    
    // Auto-Assigned IP on infra networks is not supported
    if ((!fValid) && (!fAdhoc) && (IPSTATE_AUTOIP == state))
    {
        DWORD dwPersistAutoIP = 0;
        RegistryGetDWORD(HKEY_CURRENT_USER, g_szRegKeyBase, c_szRegValuePersistAutoIP, &dwPersistAutoIP);

        if (!dwPersistAutoIP)
        {
            DWORD dwNetFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_HIDDEN;
            PCWLC pwlc = m_intf.Find(&nid, &dwNetFlags);

            if (pwlc)
            {
                // If we were trying to connect to a DHCP WEP network, but got an auto IP
                // address, it probably means the network key was bad
                if(Ndis802_11Encryption1Enabled == pwlc->Privacy)  // WEP network
                {
                    // Ask the user for another key
                    DeleteNetwork(&nid, TRUE);
                }
                else
                {
                    // Auto-Assigned IP on infra networks is not supported so we cancel
                    // the connection and remove the network from the preferred list.
                    DeleteNetwork(&nid, FALSE);

                    // Prompt Error to user
                    MessageBox(NULL, 
                        (PTSTR)LoadString(HINST_RESDLL, IDS_WZC_DHCP_WARNING, NULL, 0), 
                        (PTSTR)LoadString(HINST_RESDLL, IDS_NETUI_WRN_TITLE_WARNING, NULL, 0),
                        MB_OK | MB_SETFOREGROUND);

                    fValid = m_fConnected;
                }
            }
        }
        else
        {
            //Allow Auto-Assigned IP on infra networks if the registry is set
            fValid = TRUE;
        }
    }
    // Done
    return fValid;
}

HRESULT CWZCManagerWireless::OnConnect()
{
    HRESULT hr = S_OK;
    CNetID  nid;
    TCHAR   szDesc[64];

    // Default processing
    hr = CWZCManager::OnConnect();
    CHR(hr);

    // Clear the device state cache
    ClearStateCache();

    // We need to know which network we're actually connected to, so that we
    // can tell Connection Manager which destination network we're on.
    hr = m_intf.GetConnected(&nid);
    CHR(hr);
    CBRA(S_FALSE != hr);  // couldn't find a connection!!

    // Get the Unicode description of the SSID
    hr = nid.GetDisplayName(szDesc, ARRAYSIZE(szDesc));
    CHRA(hr);

    // Set up this connection in the Connection Manager
    hr = CM_SetCurrentNetwork(m_pszDeviceName, szDesc, nid.GetAssociation());
    CHR(hr);

    // Notify the shell
    SetHardwareFlag(SHWZCF_HW_CONNECTED, TRUE, WSF_CONNECTED);
    PostShellStatusMessage(WM_TASKBAR_ETHERNETCHANGED, TRUE, TRUE);

    DestroyUI();
    
Error:
    return hr;
}

void CWZCManagerWireless::OnDisconnect()
{
    // Default processing
    CWZCManager::OnDisconnect();

    // Clear the state cache
    ClearStateCache();

    // Notify the shell
    SetHardwareFlag(SHWZCF_HW_CONNECTED, FALSE, WSF_CONNECTED);
    PostShellStatusMessage(WM_TASKBAR_ETHERNETCHANGED, FALSE, TRUE);
}

HRESULT CWZCManagerWireless::OnListInsert(LPVOID *ppVoid)
{
    HRESULT hr = S_OK;
    TCHAR   szBuffer[MAX_PATH];
    BOOL    fStatusThreadAdded = FALSE;
    LPVOID  pvPostParam = NULL;
    HKEY hKey = NULL;

    // This function is protected by a critical section, so we can safely check
    // and set global variables
    if (0 == s_atmSinkClass)
    {
        // Register the sink window class
        hr = RegisterClass(g_szSinkClassWireless, &s_atmSinkClass);
        CHR(hr);
    }

    // Create the sink window
    hr = CreateSinkWindow(s_atmSinkClass);
    CHR(hr);

    // Get the registered event message
    if (0 == s_uEventMsg)
    {
        s_uEventMsg = RegisterWindowMessage(g_szEventMessage);
        CWR(s_uEventMsg);
    }

    // Create the WZC status thread if necessary
    if (NULL == s_hStatusThread)
    {
        DWORD dwThreadID;
        MSGQUEUEOPTIONS msgqo;

        // Open a handle to the WZC message queue
        msgqo.dwSize = sizeof(MSGQUEUEOPTIONS);
        msgqo.dwFlags = MSGQUEUE_ALLOW_BROKEN;
        msgqo.dwMaxMessages = DEFAULT_MAX_MESSAGES;
        msgqo.bReadAccess = TRUE;

        if (NULL == s_hWZCMsgQueue)
        {
            msgqo.cbMaxMessage = sizeof(LOG_ENTRY);

            s_hWZCMsgQueue = CreateMsgQueue(g_szWZCMsgQueueName, &msgqo);
            CWR(s_hWZCMsgQueue);
        }

        if (NULL == s_hWZCNdisuioMsgQueue)
        {
            // Open a handle to the WZC NDISUIO message queue
            msgqo.cbMaxMessage = sizeof(NDISUIO_DEVICE_NOTIFICATION);

            s_hWZCNdisuioMsgQueue = CreateMsgQueue(g_szWZCNdisuioMsgQueueName, &msgqo);
            CWR(s_hWZCNdisuioMsgQueue);
        }

        // Create a stop event
        s_hStatusThreadStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        CWR(s_hStatusThreadStopEvent);

        // Create the status update thread
        s_hStatusThread = CreateThread(NULL, 0, UpdateStatusThread, s_hStatusThreadStopEvent, 0, &dwThreadID);
        CWR(s_hStatusThread);
    }

    // Increment the thread refcount
    ++s_cRefStatusThread;
    fStatusThreadAdded = TRUE;

    // Start the network interface. This will start posting messages to our
    // sink window from the thread we just created. If there are no preferred
    // networks to connect to immediately, we'll get a message to start polling
    // for new networks.
    hr = m_intf.Initialize(m_pszDeviceName);
    CHR(hr);

    // WPA support
    SetHardwareFlag(SHWZCF_HW_SUPPORT_WPA, BOOLIFY(INTFCAP_SSN & m_intf.m_sIntfEntry.dwCapabilities), 0);
    SetHardwareFlag(SHWZCF_HW_SUPPORT_WPA2, BOOLIFY(INTFCAP_80211I & m_intf.m_sIntfEntry.dwCapabilities), 0);
    SetHardwareFlag(SHWZCF_HW_SUPPORT_WPA_AES, BOOLIFY(IsWPAAESCapable(m_intf.m_sIntfEntry.rdNicCapabilities.pData)), 0);

    // Power is always on at this point
    SetHardwareFlag(SHWZCF_HW_ON, TRUE, WSF_POWER_ON);

    // Load the registry data
    hr = LoadRegistryData();
    CHR(hr);

    hr = SetDefaultPowerState();
    CHR(hr);

    // We create an event for the driver to know that's it's been fully loaded
    // by netui
    CHRA(StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), c_szDriverEventTemplate, m_pszDeviceName));
    m_hEventDriverStatus = CreateEvent(NULL, TRUE, FALSE, szBuffer);
    CWREx(NULL != m_hEventDriverStatus, HRESULT_FROM_WIN32(GetLastError()));
    SetEvent(m_hEventDriverStatus);

    // If the shell isn't ready, set a timer to periodically ping shell status to find out 
    // when it's ready so that we can start showing notification UI if there are any new networks. 
    // If the shell's up already, we don't need to wait on it.
    if(!IsShellReady())
    {
        SetTimer(m_hwndSink, IDT_SHELLSTATUS_CHECK, 500, 0);
    }
    else
    {
        m_fShellReady = TRUE;
    }

    // Create the registry callback for wifi timeout.  
    if(m_hWifiTimeoutRegNotify == NULL)
    {
        // update the timeout period from the registry and convert the value from seconds to milliseconds
        SHWZCGetWifiTimeoutPeriod(&m_uWifiTimeoutPeriod);
        m_uWifiTimeoutPeriod *= 1000;
        
        CBR(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_szRegKeyBase, 0, NULL, &hKey));
        CHR(RegistryNotifyCallback(hKey,
                                   NULL,
                                   c_szRegValueWifiTimeout,
                                   WifiTimeoutRegCallback,
                                   (DWORD)this,
                                   NULL,
                                   &m_hWifiTimeoutRegNotify));
        RegCloseKey(hKey);
        hKey = NULL;
    }

Error:
    if (FAILED(hr) && fStatusThreadAdded)
    {
        pvPostParam = OnListRemove();
    }

    if(hKey != NULL)
    {
        RegCloseKey(hKey);
    }

    *ppVoid = pvPostParam;
    return hr;
}

void CWZCManagerWireless::OnTimer(UINT uTimerID)
{
    switch (uTimerID)
    {
    case IDT_SHELLSTATUS_CHECK:
        if(IsShellReady())
        {
            // The Home screen just came up
            KillTimer(m_hwndSink, IDT_SHELLSTATUS_CHECK);

            // Determine how long we should wait after the home screen draws before  
            // checking the new networks list and showing wifi notification UI if the
            // device boots with wifi turned on.            
            DWORD dwUIDelayAfterBoot = 0;
            if (SUCCEEDED(RegistryGetDWORD(HKEY_LOCAL_MACHINE, 
                                           g_szRegKeyBase, 
                                           g_szRegValueUIDelayAfterBoot, 
                                           &dwUIDelayAfterBoot)))
            {
                if (dwUIDelayAfterBoot > MAX_UI_DELAY_ON_BOOT)
                {
                    dwUIDelayAfterBoot = MAX_UI_DELAY_ON_BOOT;
                }
            }
            else
            {
                dwUIDelayAfterBoot = DEFAULT_UI_DELAY_ON_BOOT;
            }

            SetTimer(m_hwndSink, IDT_UI_DELAY_AFTER_BOOT, dwUIDelayAfterBoot*1000, 0);
        }
        break;

    case IDT_UI_DELAY_AFTER_BOOT:
        // Unblock Wifi UI so notifications can pop up if needed
        KillTimer(m_hwndSink, IDT_UI_DELAY_AFTER_BOOT);
        m_fShellReady = TRUE;
        OnUnblockUI();
        break;

    case IDT_WIFI_TIMEOUT:
        // Stop the timer and then shutdown wifi
        StopWifiTimeoutTimer();
        SHWZCSetPowerState(FALSE);
        break;

    }
    return;
}

LPVOID CWZCManagerWireless::OnListRemove()
{
    LPVOID pvParam = NULL;

    if (NULL != m_hEventDriverStatus)
    {
        // Reset and close the driver status event
        ResetEvent(m_hEventDriverStatus);
        CloseHandle(m_hEventDriverStatus);
        m_hEventDriverStatus = NULL;
    }

    // This function is protected by a critical section, so we can safely check
    // and set global variables
    if (--s_cRefStatusThread == 0)
    {
        // Tell the status update thread to quit (it closes this event handle)
        SetEvent(s_hStatusThreadStopEvent);
        s_hStatusThreadStopEvent = NULL;

        // The thread will also close the message queue handle
        s_hWZCMsgQueue = NULL;
        s_hWZCNdisuioMsgQueue = NULL;

        // We want to post-process this after we exit the critical section, to
        // close down the thread
        pvParam = (LPVOID)s_hStatusThread;
        s_hStatusThread = NULL;
    }
    else
    {
        // Power is always "off" at this point
        SetHardwareFlag(SHWZCF_HW_ON, FALSE, WSF_POWER_ON);
    }

    return pvParam;
}


void CWZCManagerWireless::OnListCleanup(LPVOID pvParam)
{
    HANDLE hStatusThread = (HANDLE)pvParam;

    // Wait for the thread for a bit (to make sure it can finish its
    // current message before this object gets deleted)
    WaitForSingleObject(hStatusThread, 1000);

    // Close the thread handle (the stop event handle gets closed in the thread itself)
    CloseHandle(hStatusThread);
}

HRESULT CWZCManagerWireless::DismissAiringList()
{
    HRESULT hr = S_OK;

    // Dismiss the networks
    hr = m_nidlDismissed.SortedInsert(m_airing.GetList(), m_airing.GetCount());
    CHR(hr);

    // We don't really need the list any more
    m_airing.Clear();

Error:
    return hr;
}

void CWZCManagerWireless::ClearStateCache()
{
    // Clear the device state cache
    m_intf.Clear();

    // Tell callback UI that there may be changes
    PostCallback(SHWZC_EVENT_BROADCASTCHANGE);
}

HRESULT CWZCManagerWireless::CheckNewNetworks()
{
    HRESULT hr = S_OK;

    // Get the airing list
    hr = QueryInterface(&m_intf);
    CHR(hr);

    // Are there any visible networks?
    if (m_intf.GetConfigs(SHWZCF_NET_BROADCAST))
    {
        // Check for override UI
        if (m_fOverride)
        {
            // Just dismiss all the networks
            PCWLC pwlc;
            UINT cConfigs = m_intf.GetConfigs(SHWZCF_NET_BROADCAST, &pwlc);
            hr = m_nidlDismissed.SortedInsert(pwlc, cConfigs);
            CHR(hr);
        }
        else
        {
            hr = OnCheckNewNetworks();
            CHR(hr);
        }
    }

    SetHardwareFlag(SHWZCF_HW_NETAVAIL, BOOLIFY(m_intf.GetConfigs(SHWZCF_NET_BROADCAST | SHWZCF_NET_HIDDEN)), WSF_NETWORKS_AVAILABLE);

Error:
    return hr;
}

HRESULT CWZCManagerWireless::OnCheckNewNetworks()
{
    HRESULT hr = S_OK;
    PCWLC   pwlc, pwlcEnd;
    CNetID  nid;
    DWORD   dwNetFlags;
    UINT    cSeen = 0;

    // If we're already connected, we're done
    CBREx(!IsConnected(), S_FALSE);

    // If the UI is blocked, we're done
    CBREx(!IsUIBlocked(), S_FALSE);

    // Have there been any changes since our previous airing list?
    for (pwlc = m_airing.GetList(), pwlcEnd = pwlc + m_airing.GetCount(); pwlc != pwlcEnd; ++pwlc)
    {
        nid.Assign(pwlc);
        dwNetFlags = SHWZCF_NET_BROADCAST;
        if (NULL != m_intf.Find(&nid, &dwNetFlags))
        {
            ++cSeen;
        }
    }

    // If the list is the same, we're done
    CBREx(cSeen != m_airing.GetCount() ||
        cSeen != m_intf.GetConfigs(SHWZCF_NET_BROADCAST), S_OK);

    // Replace the current airing list
    hr = m_airing.Assign(&m_intf);
    CHR(hr);

    // Show the new networks
    hr = ShowNewNetworksUI();
    CHR(hr);

Error:
    return hr;
}

HRESULT CWZCManagerWireless::SetHardwareFlag(UINT uFlag, BOOL fSet, DWORD dwRegFlag)
{
    HRESULT hr = S_OK;
    DWORD dwWifiStatus = 0;

    // Update our flags
    m_uHardwareFlags = fSet ? (m_uHardwareFlags | uFlag) : (m_uHardwareFlags & ~uFlag);

    // Does this flag map to a registry flag and need to be set?
    // Only call SHWZCGetHardwareFlags when fSet==FALSE as it's exspensive
    if (0 != dwRegFlag && (fSet || !BOOLIFY(uFlag & SHWZCGetHardwareFlags())))
    {
        // Update the registry state
        hr = SetWirelessStateFlags(dwRegFlag, fSet ? dwRegFlag : 0);
        CHR(hr);

        if(m_uWifiTimeoutPeriod != 0)
        {
            switch(dwRegFlag)
            {
            case WSF_POWER_ON:
                if(fSet)
                {
                    // turn on timer - power is on, but not connected
                    StartWifiTimeoutTimer();
                }
                else
                {
                    // turn off timer - power is off
                    StopWifiTimeoutTimer();
                }
                break;
                
            case WSF_CONNECTED:
                if(fSet)
                {
                    // turn off timer - wifi is connected to a network
                    StopWifiTimeoutTimer();
                }
                else
                {
                    // turn on timer - wifi is not connected to a network
                    StartWifiTimeoutTimer();
                }    
                break;

            default:
                break;
            }
        }
    }

Error:
    return hr;
}

HRESULT CWZCManagerWireless::OnPrivateMessage(UINT msg, WPARAM wp, LPARAM lp)
{
    HRESULT hr = S_OK;

    switch (msg)
    {
    case PM_GETNETLIST:
        hr = GetCurrentNetworks((HDSA)wp, (DWORD)lp);
        break;

    case PM_FINDOWNER:
        OnFindOwner((FINDOWNERINFO*)lp);
        break;

    case PM_FINDADAPTER:
        hr = OnFindAdapter((LPCTSTR)wp, (HWND*)lp);
        break;

    case PM_REGISTERCB:
        hr = LoadCallbackWindow((HWND)lp);
        break;

    case PM_UNREGISTERCB:
        UnregisterCallbackWindow((HWND)lp);
        break;

    case PM_GETSTATE:
        hr = m_intf.GetFilter((DWORD*)wp);
        break;

    case PM_SETSTATE:
        hr = m_intf.SetFilter((DWORD)wp, TRUE);
        CHR(hr);

        ClearStateCache();
        hr = CheckNewNetworks();
        break;

    case PM_SETNETWORKS:
    {
        // Get the header
        const SETNETWORKHDR *psnh = (const SETNETWORKHDR*)lp;
        CBRAEx(NULL != psnh, E_INVALIDARG);

        // Get the network data
        const SETNETWORKITEM *psni = (const SETNETWORKITEM*)(psnh + 1);
        const BYTE *pbEapData = (const BYTE*)(psni + psnh->cNetworks);

        // Set the networks
        hr = SetNetworks(psnh, psni, pbEapData);
        break;
    }

    case PM_HWFLAGS:
        CBRAEx(NULL != (UINT*)wp, E_INVALIDARG);
        *((UINT*)wp) |= SHWZCF_HW_PRESENT | GetHardwareFlags();
        break;

    default:
        hr = CWZCManager::OnPrivateMessage(msg, wp, lp);
        break;
    }

Error:
    return hr;
}

void CWZCManagerWireless::OnGlobalEvent(UINT uEventID)
{
    switch (uEventID)
    {
    case SHWZCE_RESCAN:
        DoForegroundScan();
        break;

    case SHWZCE_DELETEALL:
        DeleteAllNetworks();
        break;

    case SHWZCE_UPDATE_ASSOCIATION:
        TryConfigMessage(WZC_CM_UPDATE_ASSOCIATION, 0);
        break;

    default:
        CWZCManager::OnGlobalEvent(uEventID);
        break;
    }
}

HRESULT CWZCManagerWireless::OnScanComplete()
{
    HRESULT hr = S_OK;

    // We just finished a hardware scan, but couldn't find any preferred
    // networks to connect
    DEBUGMSG(TRUE, (TEXT("Wi-Fi: Completed hardware scan for networks\r\n")));

    // Invalidate the device state cache
    ClearStateCache();

    // Set the "valid scan" bit and put up new UI if necessary
    m_fValidScan = TRUE;
    hr = CheckNewNetworks();
    CHR(hr);

Error:
    return hr;
}

/// <summary>
/// Send or Post a network message to the WZCManager sink window in response to a WZC notification
/// </summary>
/// <returns>
/// LRESULT from SendMessage or PostMessage call
/// </returns>
/// <param name="hwndSink">
/// WZCManager sink window
/// </param>
/// <param name="ple">
/// Log entry that contains notification info from WZC
/// </param>
/// <param name="uMsg">
/// Message to be send to the sink
/// </param>
/// <param name="wParam">
/// Additional information sent with network message
/// </param>
/// <param name="fSend">
/// BOOL value that determines whether the message should be Sent or Posted
/// </param>
static LRESULT DispatchNetworkMessage(HWND hwndSink, const LOG_ENTRY *ple, UINT uMsg, WPARAM wParam, BOOL fSend)
{
    CNetID nid;

    // Sanity check
    CASSERT(sizeof(nid.Ssid.Ssid[0]) == sizeof(ple->pucSSID[0]));
    CASSERT(ARRAYSIZE(nid.Ssid.Ssid) >= ARRAYSIZE(ple->pucSSID) - 1);

    // Create the network ID
    nid.Ssid.SsidLength = strlen((LPCSTR)ple->pucSSID);
    memcpy(nid.Ssid.Ssid, ple->pucSSID, nid.Ssid.SsidLength * sizeof(nid.Ssid.Ssid[0]));
    nid.InfrastructureMode = ple->dwInfrastructureMode;

    // Send or Post notification
    if (fSend)
    {   
        // Send notification. 
        // NOTE: Please don't do any blocking operations in this message
        // handler, since the status thread is waiting for it to return
        return SendMessage(hwndSink, uMsg, wParam, (LPARAM)&nid);
    }
    else
    {
        // Post notification.
        // Note: We need to make a copy of nid on the heap before we post it.
        // The function that handles uMsg MUST free this after it's done
        CNetID* pnidCopy = (CNetID*)LocalAlloc(LMEM_FIXED, sizeof(CNetID));
        memcpy(pnidCopy, &nid, sizeof(CNetID));
        return PostMessage(hwndSink, uMsg, wParam, (LPARAM)pnidCopy);
    }

        
}

void CWZCManagerWireless::OnWZCMessage(HWND hwndSink, const LOG_ENTRY *ple)
{
	BBS_DEF_PREF_NETWORK_INFO defPrefNetInfo = {0,};
	BBS_BKG_PINGCHECK_OPT sPingCheckOpt = {0,};
	WZC_WLAN_CONFIG wzcConfig = {0,};
	TCHAR tszLogMsg[256] = L"";
	BYTE pucMacAddr[6] = {0,};

	switch (ple->dwLogId)
	{
	case WZC_STARTED:
		NKDbgPrintfW( _T("[WiFi]: Started service for '%s'\r\n"), ple->ptcAdapterName);
		wsprintf(tszLogMsg, L"WZC Start (%s) \r\n", ple->ptcAdapterName);
		AddLogData(1, tszLogMsg);
		

		// Ping Checker
		GetBkgPingCheckOpt(&sPingCheckOpt);

		if( sPingCheckOpt.bOperating == TRUE )
		{
			if( !GetPingCheckerStatus() )
			{
				PingCheckerStart(sPingCheckOpt);
			}
		}
		

		// Custom SSID 추가 
		// Load setting value from XML
		memset(&defPrefNetInfo, 0, sizeof(BBS_DEF_PREF_NETWORK_INFO));
		GetDefPrefNetOpt(&defPrefNetInfo);

		if(defPrefNetInfo.bEnable == TRUE)
		{
			HKEY hKey = NULL;
			TCHAR tszTemp[256] = {0,};
			DWORD dwData = 0;
			DWORD dwSize = 0;
			int nLen = 0;

			wsprintf(tszLogMsg, L"[DefPrefNet] Enabled! DHCP(%d) \r\n", defPrefNetInfo.bDHCP );
			AddLogData(1, tszLogMsg);

			if(defPrefNetInfo.bDHCP == FALSE)
			{					

				wsprintf(tszTemp, L"Comm\\%s\\Parms\\TcpIp", ple->ptcAdapterName);
				if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, tszTemp, 0, 0, &hKey) == ERROR_SUCCESS )
				{
					// DHCP
					dwData = 0;
					RegSetValueEx(hKey, L"EnableDHCP", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
				
					// IP Address
					memset(tszTemp, 0, sizeof(tszTemp));
					wsprintf(tszTemp, L"%s", defPrefNetInfo.tszIPAddr);
					nLen = _tcslen(tszTemp)+1;
					tszTemp[nLen++] = TEXT('\0');
					RegSetValueEx(hKey, L"IpAddress", 0, REG_MULTI_SZ, (LPBYTE)tszTemp, nLen * sizeof(TCHAR) );

					// Subnet
					memset(tszTemp, 0, sizeof(tszTemp));
					wsprintf(tszTemp, L"%s", defPrefNetInfo.tszSubNetMask);
					nLen = _tcslen(tszTemp)+1;
					tszTemp[nLen++] = TEXT('\0');
					RegSetValueEx(hKey, L"Subnetmask", 0, REG_MULTI_SZ, (LPBYTE)tszTemp, nLen * sizeof(TCHAR) );

					// GateWay
					memset(tszTemp, 0, sizeof(tszTemp));
					wsprintf(tszTemp, L"%s", defPrefNetInfo.tszGateWay);
					nLen = _tcslen(tszTemp)+1;
					tszTemp[nLen++] = TEXT('\0');
					RegSetValueEx(hKey, L"DefaultGateway", 0, REG_MULTI_SZ, (LPBYTE)tszTemp, nLen * sizeof(TCHAR) );

					// DNS
					memset(tszTemp, 0, sizeof(tszTemp));
					wsprintf(tszTemp, L"%s", defPrefNetInfo.tszDnsAddr);
					nLen = _tcslen(tszTemp)+1;
					tszTemp[nLen++] = TEXT('\0');
					RegSetValueEx(hKey, L"DNS", 0, REG_MULTI_SZ, (LPBYTE)tszTemp, nLen * sizeof(TCHAR) );

					RegCloseKey(hKey);
				}
			}
			else
			{
				wsprintf(tszTemp, L"Comm\\%s\\Parms\\TcpIp", ple->ptcAdapterName);
				if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, tszTemp, 0, 0, &hKey) == ERROR_SUCCESS )
				{
					// DHCP
					dwData = 1;
					RegSetValueEx(hKey, L"EnableDHCP", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );

					RegCloseKey(hKey);
				}
			}
				

			memset(&wzcConfig, 0, sizeof(wzcConfig));
			wzcConfig.Length = sizeof(wzcConfig);
			wzcConfig.dwCtlFlags = 0;

			// SSID
			WCHAR szSsidToConnect[MAX_SSID_LEN+1];
			memcpy(szSsidToConnect, defPrefNetInfo.tszSSID, sizeof(szSsidToConnect));
			szSsidToConnect[MAX_SSID_LEN] = L'\0';
			NKDbgPrintfW(L"[NETUI]  SSID           = \"%s\" \r\n", szSsidToConnect);

			int ssidChars = WideCharToMultiByte(CP_ACP, 0, szSsidToConnect, wcslen(szSsidToConnect), (LPSTR)(wzcConfig.Ssid.Ssid), MAX_SSID_LEN, NULL, NULL);
			if (0 >= ssidChars)
			{
				NKDbgPrintfW(L"[NETUI] SSID is too long or invalid. Max ASCII length in %d chars.\r\n", MAX_SSID_LEN);
				goto ConnectError;
			}

			wzcConfig.Ssid.SsidLength = ssidChars;

			// Authentication & Encryption
			switch(defPrefNetInfo.dwAuthType)
			{
			case 0:
				wzcConfig.AuthenticationMode = Ndis802_11AuthModeOpen;
				if(defPrefNetInfo.dwEncrType == 1)
				{
					wzcConfig.Privacy = Ndis802_11WEPEnabled;
				}
				else
				{
					wzcConfig.Privacy = Ndis802_11WEPDisabled;
				}
				break;
			case 1:
				wzcConfig.AuthenticationMode = Ndis802_11AuthModeShared;
				if(defPrefNetInfo.dwEncrType == 1)
				{
					wzcConfig.Privacy = Ndis802_11WEPEnabled;
				}
				else
				{
					wzcConfig.Privacy = Ndis802_11WEPDisabled;
				}
				break;
			case 2:
				wzcConfig.AuthenticationMode = Ndis802_11AuthModeWPA;
				wzcConfig.Privacy = Ndis802_11WEPDisabled;
				break;
			case 3:
				wzcConfig.AuthenticationMode = Ndis802_11AuthModeWPAPSK;
				if(defPrefNetInfo.dwEncrType == 1)
				{
					wzcConfig.Privacy = Ndis802_11Encryption3Enabled;
				}
				else
				{
					wzcConfig.Privacy = Ndis802_11Encryption2Enabled;
				}
				break;
			case 4:
				wzcConfig.AuthenticationMode = Ndis802_11AuthModeWPA2;
				wzcConfig.Privacy = Ndis802_11WEPDisabled;
				break;
			case 5:
				wzcConfig.AuthenticationMode = Ndis802_11AuthModeWPA2PSK;
				if(defPrefNetInfo.dwEncrType == 1)
				{
					wzcConfig.Privacy = Ndis802_11Encryption3Enabled;
				}
				else
				{
					wzcConfig.Privacy = Ndis802_11Encryption2Enabled;
				}
				break;
			default:
				wzcConfig.AuthenticationMode = Ndis802_11AuthModeOpen;
				wzcConfig.Privacy = Ndis802_11WEPDisabled;
				break;
			}



			// Key
			BOOL bNeed8021X = FALSE;
			char szEncryptionKeyValue8[64] = {'\0',}; // longest key is 63


			NKDbgPrintfW(L"[NETUI]  key            = \"%s\" \r\n", defPrefNetInfo.tszNetworkKey);

			// 네트워크키가 입력되어 있지 않으면 auto로 설정 (EAP모드?)
			if( wcslen(defPrefNetInfo.tszNetworkKey) <= 0)
			{
				wcscpy(defPrefNetInfo.tszNetworkKey, L"auto");
			}

			if(wzcConfig.Privacy == Ndis802_11WEPEnabled)
			{
				if (wcsicmp(defPrefNetInfo.tszNetworkKey, L"auto") == 0)
				{
					bNeed8021X = TRUE;
				}
				else
				{
					DWORD dwkeyLength = 0;
					BOOL bAsciiKey = FALSE;

					// 현재 KeyIndex는 0부터 입력되어있기 때문에 0부터 3까지 인덱스가 맞는지 확인 (UI상으로는 1~4)
					if( (defPrefNetInfo.dwKeyIndex < 0) || (defPrefNetInfo.dwKeyIndex > 3) )
					{
						wsprintf(tszLogMsg, L"[DefPrefNet] invalid key index \r\n");
						AddLogData(1, tszLogMsg);
						goto ConnectError;
					}

					wzcConfig.KeyIndex = defPrefNetInfo.dwKeyIndex;

					dwkeyLength = wcslen(defPrefNetInfo.tszNetworkKey);

					// 16진수 (HEX값?)
					if(defPrefNetInfo.bHexKey == TRUE)
					{
						//16진수 10자리, 26자리가 아니면 잘못입력된 키값
						if( (dwkeyLength != 10) && (dwkeyLength != 26) )
						{
							wsprintf(tszLogMsg, L"[DefPrefNet] invalid key length (s/b 10 or 26 (hex) \r\n");
							AddLogData(1, tszLogMsg);
							break;
						}

						wzcConfig.KeyLength = dwkeyLength / 2;
						for (UINT cx = 0, abyte = 0 ; cx < dwkeyLength ; ++cx)
						{
							UINT ch = static_cast<UINT>(defPrefNetInfo.tszNetworkKey[cx]);
							switch (ch)
							{
							case L'0': case L'1': case L'2': case L'3': case L'4':
							case L'5': case L'6': case L'7': case L'8': case L'9':
								ch -= L'0';
								break;
							case L'a': case L'b': case L'c':
							case L'd': case L'e': case L'f':
								ch -= L'a' - 10;
								break;
							case L'A': case L'B': case L'C':
							case L'D': case L'E': case L'F':
								ch -= L'A' - 10;
								break;
							default:
								wsprintf(tszLogMsg, L"[DefPrefNet] invalid HEX WEP key \r\n");
								AddLogData(1, tszLogMsg);
								goto ConnectError;
							}
							if ((cx & 1) == 0)
							{
								abyte = ch;
							}
							else
							{
								abyte <<= 4;
								abyte |= ch;
								wzcConfig.KeyMaterial[cx/2] = static_cast<BYTE>(abyte);
							}
						}
					}
					// 10진수 (ASCII 값?)
					else
					{
						//10진수 5자리, 13자리가 아니면 잘못입력된 키값
						if( (dwkeyLength != 5) && (dwkeyLength != 13) )
						{
							wsprintf(tszLogMsg, L"[DefPrefNet] invalid key length (s/b 5 or 13 (ascii) \r\n");
							AddLogData(1, tszLogMsg);
							break;
						}

						wzcConfig.KeyLength = dwkeyLength;
						for (UINT cx = 0 ; cx < dwkeyLength; ++cx)
						{
							wzcConfig.KeyMaterial[cx] = (UCHAR)defPrefNetInfo.tszNetworkKey[cx];
						}
					}


					EncryptWepKMaterial(&wzcConfig);

					wzcConfig.dwCtlFlags |= WZCCTL_WEPK_PRESENT;


				}
			}
			else if( (wzcConfig.Privacy == Ndis802_11Encryption2Enabled) || (wzcConfig.Privacy == Ndis802_11Encryption3Enabled) )
			{
				if (wcsicmp(defPrefNetInfo.tszNetworkKey, L"auto") == 0)
				{
					bNeed8021X = TRUE;
				}
				else
				{
					wzcConfig.KeyLength = wcslen(defPrefNetInfo.tszNetworkKey);
					if ((wzcConfig.KeyLength<8) || (wzcConfig.KeyLength>63))
					{
						wsprintf(tszLogMsg, L"[DefPrefNet] WPA-PSK/TKIP key should be 8-63 char long string \r\n");
						AddLogData(1, tszLogMsg);
						goto ConnectError;
					}

					WideCharToMultiByte(CP_ACP, 0, defPrefNetInfo.tszNetworkKey, wzcConfig.KeyLength+1, szEncryptionKeyValue8, wzcConfig.KeyLength+1, NULL, NULL);
					WZCPassword2Key(&wzcConfig, szEncryptionKeyValue8);
					EncryptWepKMaterial(&wzcConfig);
					wzcConfig.dwCtlFlags |= WZCCTL_WEPK_XFORMAT |  WZCCTL_WEPK_PRESENT |  WZCCTL_ONEX_ENABLED;
				}

				wzcConfig.EapolParams.dwEapFlags = EAPOL_ENABLED;
				wzcConfig.EapolParams.dwEapType = DEFAULT_EAP_TYPE;
				wzcConfig.EapolParams.bEnable8021x = TRUE;
				wzcConfig.WPAMCastCipher = Ndis802_11Encryption2Enabled;
			}
			
			// Need8021X? (EAP Mode wasn't support yet)
			if (bNeed8021X)
			{
			}

			// adhoc? or infrastructure net?
			wzcConfig.InfrastructureMode = Ndis802_11Infrastructure;

			// Default to connecting to only APs that are beaconing.
			wzcConfig.dwCtlFlags |= WZCCTL_BROADCAST_SSID;


			AddToPreferredNetworkList(ple->ptcAdapterName, wzcConfig);

		}


ConnectError:

        break;

    case WZC_ASSOCIATING:
		NKDbgPrintfW( _T("[WiFi]: '%s' is attempting to connect to '%S'...\r\n"), ple->ptcAdapterName, ple->pucSSID);
        DispatchNetworkMessage(hwndSink, ple, WM_WZC_CONNECT_BEGIN, 0, TRUE);

		wsprintf(tszLogMsg, L"'%s' is attempting to connect to '%S'... \r\n", ple->ptcAdapterName, ple->pucSSID);
		AddLogData(1, tszLogMsg);
        break;

    case WZC_AUTHENTICATING:
		NKDbgPrintfW( _T("[WiFi]: '%s' is attempting to authenticate on '%S'...\r\n"), ple->ptcAdapterName, ple->pucSSID);
		
		wsprintf(tszLogMsg, L"'%s' is attempting to authenticate on '%S'... \r\n", ple->ptcAdapterName, ple->pucSSID);
		AddLogData(1, tszLogMsg);
        break;

    case WZC_ASSOCIATED_NO_8021X:
    case WZC_ASSOCIATED_REQUIRES_8021X:
        DispatchNetworkMessage(hwndSink, ple, WM_WZC_UPDATE_8021X_STATE, 
                           (ple->dwLogId == WZC_ASSOCIATED_REQUIRES_8021X) ? 
                                            WZC_AUTHSTATE8021X_ASSOCIATED : WZC_AUTHSTATE8021X_RESET, TRUE);
		NKDbgPrintfW( _T("[WiFi]: '%s' has associated successfully with '%S'\r\n"), ple->ptcAdapterName, ple->pucSSID);

		wsprintf(tszLogMsg, L"'%s' has associated successfully with '%S' \r\n", ple->ptcAdapterName, ple->pucSSID);
		AddLogData(1, tszLogMsg);

		GetBSSID((LPTSTR)ple->ptcAdapterName, pucMacAddr);
		wsprintf(tszLogMsg, L"[%02X:%02X:%02X:%02X:%02X:%02X] \r\n", pucMacAddr[0],pucMacAddr[1],pucMacAddr[2],pucMacAddr[3],pucMacAddr[4],pucMacAddr[5] );
		AddLogData(1, tszLogMsg);

		NKDbgPrintfW( _T("[netui] pucMacAddr : '%s' [%02X:%02X:%02X:%02X:%02X:%02X]...\r\n"), tszLogMsg, ple->pucBSSID[0], ple->pucBSSID[1], ple->pucBSSID[2], ple->pucBSSID[3], ple->pucBSSID[4], ple->pucBSSID[5] );

        break;

    case WZC_AUTHENTICATED:
        // We send the authenticated message in all cases in order to update 8021X state, 
        // but the handling code should ignore it if it wasn't previously sent an 8021X associated message
        DispatchNetworkMessage(hwndSink, ple, WM_WZC_UPDATE_8021X_STATE, WZC_AUTHSTATE8021X_AUTHENTICATED, TRUE);
		NKDbgPrintfW( _T("[WiFi]: '%s' has authenticated successfully on '%S'\r\n"), ple->ptcAdapterName, ple->pucSSID);
		
		wsprintf(tszLogMsg, L"'%s' has authenticated successfully on '%S' \r\n", ple->ptcAdapterName, ple->pucSSID);
		AddLogData(1, tszLogMsg);
        break;

    case WZC_CLOSE_EAPOL_SESSION:
        // We receive this message from the OS whenever the EAP session closes, even when we are closing a successfully
        // authenticated connection. There is no need to send an update 8021x message; this handler already resets the state.
        DispatchNetworkMessage(hwndSink, ple, WM_WZC_HANDLE_EAPOL_CLOSE, 0, TRUE);
		NKDbgPrintfW( _T("[WiFi]: WZC_CLOSE_EAPOL_SESSION \r\n") );
		
		wsprintf(tszLogMsg, L"WZC_CLOSE_EAPOL_SESSION \r\n");
		AddLogData(1, tszLogMsg);
        break;

    case WZC_PREFERED_LIST_EXHAUSTED:
		NKDbgPrintfW( _T("[WiFi]: WZC_PREFERED_LIST_EXHAUSTED \r\n") );
        SendMessage(hwndSink, WM_WZC_CONNECT_STOPPED, 0, 0);

        // We don't have any preferred networks
        PostMessage(hwndSink, WM_WZC_SCANCOMPLETE, TRUE, 0);

		wsprintf(tszLogMsg, L"WZC_PREFERED_LIST_EXHAUSTED \r\n");
		AddLogData(1, tszLogMsg);
        break;

    case WZC_FAILED_AUTH_NO_RETRY:
		NKDbgPrintfW( _T("[WiFi]: WZC_FAILED_AUTH_NO_RETRY \r\n") );
        DispatchNetworkMessage(hwndSink, ple, WM_WZC_AUTH_FAILED, (WPARAM) MAKE_HRESULT(1, FACILITY_WZC, ple->dwLogId), FALSE);
		
		wsprintf(tszLogMsg, L"WZC_FAILED_AUTH_NO_RETRY \r\n");
		AddLogData(1, tszLogMsg);
        break;
        
    case WZC_FAILED_ASSOCIATION:
    case WZC_CANCEL_AUTH:
        {
            BOOL    fGeneric = (WZC_FAILED_ASSOCIATION == ple->dwLogId);
            HRESULT hrConnect = (fGeneric ? E_FAIL : MAKE_HRESULT(1, FACILITY_WZC, ple->dwLogId));

            // Send the error message
            DispatchNetworkMessage(hwndSink, ple, WM_WZC_CONNECT_COMPLETE_SEND, (WPARAM)hrConnect, TRUE);
        }
		NKDbgPrintfW( _T("[WiFi]: WZC_CANCEL_AUTH \r\n") );
		
		wsprintf(tszLogMsg, L"WZC_CANCEL_AUTH \r\n");
		AddLogData(1, tszLogMsg);
        break;
    }
}

void CWZCManagerWireless::PostCallback(UINT msg, LPARAM lp) const
{
    CCallback *pcb;
    CCallback *pcbNext = NULL;

    for (pcb = m_pcbHead; pcb; pcb = pcbNext)
    {
        // Save the next object so that if this one gets removed we're OK
        pcbNext = pcb->pcbNext;

        // Check window validity
        if (IsWindow(pcb->hwndCallback))
        {
            // Post the message
            PostMessage(pcb->hwndCallback, pcb->uCallbackMsg, msg, lp);
        }
        else
        {
            // If the window's gone bad, remove it
            UnregisterCallback(pcb->hwndCallback);
        }
    }
}

HRESULT CWZCManagerWireless::RegisterCallbackWindow(HWND hwnd, const CBINFO *pcbi, BOOL fResetInterval)
{
    HRESULT   hr = S_OK;
    CCallback *pcb = NULL;
    BOOL      fForceReset = FALSE;

    // Try to find the object in the list
    for (pcb = m_pcbHead; pcb; pcb = pcb->pcbNext)
    {
        if (hwnd == pcb->hwndCallback)
        {
            break;
        }
    }

    // Is it already in the list?
    CBREx(NULL == pcb, S_FALSE);

    // Create a new callback object
    pcb = new CCallback(hwnd, pcbi);
    CPR(pcb);

    // Add the object to the list
    pcb->pcbNext = m_pcbHead;
    m_pcbHead = pcb;

    // Is it a UI override?
    if (pcb->dwFlags & SHWZCF_CB_OVERRIDE)
    {
        // Hide any existing UI and dismiss the current networks
        DismissAiringList();
        DestroyUI();
        m_fOverride = TRUE;
        fForceReset = TRUE;
    }

    // Update the scan interval
    if (fResetInterval)
    {
        UpdateScanInterval(fForceReset);
    }

Error:
    return hr;
}

HRESULT CWZCManagerWireless::DoForegroundScan()
{
    HRESULT       hr = S_OK;
    CWZCInterface intf(m_pszDeviceName);

    hr = intf.Rescan();
    CHR(hr);

    hr = OnScanComplete();
    CHR(hr);

Error:
    return hr;
}

void CWZCManagerWireless::UnregisterCallbackWindow(HWND hwnd)
{
    CCallback **ppcb;
    CCallback *pcbResult = NULL;

    // Step through the items and look for the matching object
    for (ppcb = &m_pcbHead; *ppcb; ppcb = &((*ppcb)->pcbNext))
    {
        if (hwnd == (*ppcb)->hwndCallback)
        {
            // Remove the matching object
            pcbResult = *ppcb;
            *ppcb = pcbResult->pcbNext;
            pcbResult->pcbNext = NULL;
            break;
        }
    }

    if (NULL != pcbResult)
    {
        // Is this the last UI override?
        if (pcbResult->dwFlags & SHWZCF_CB_OVERRIDE)
        {
            CCallback *pcb = m_pcbHead;
            for(; pcb; pcb = pcb->pcbNext)
            {
                if (pcb->dwFlags & SHWZCF_CB_OVERRIDE)
                {
                    break;
                }
            }

            if (NULL == pcb)
            {
                // Clear the override state
                m_fOverride = FALSE;
                OnUnblockUI();
            }
        }

        // Delete the object
        delete pcbResult;

        // Update the scan interval
        UpdateScanInterval(FALSE);
    }
}

HRESULT CWZCManagerWireless::QueryInterface(CWZCInterface *pintf) const
{
    HRESULT hr = S_OK;
    PCWLC   pwlc;
    UINT    cConfigs;
    UINT    i;
    CNetID  nid;
    DWORD   dwNetFlags;

    // Query the device driver
    hr = pintf->Query(INTF_STANDARD_QUERY);
    CHR(hr);

    // Hide broadcast networks that the user has dismissed or that are already
    // configured
    cConfigs = pintf->GetConfigs(SHWZCF_NET_BROADCAST, &pwlc);
    for (i = 0; i != cConfigs;)
    {
        nid.Assign(pwlc + i);
        dwNetFlags = SHWZCF_NET_PREFERRED | SHWZCF_NET_HIDDEN;
        if (m_nidlDismissed.Find(&nid) ||
            NULL != pintf->Find(&nid, &dwNetFlags))
        {
            // This network has been dismissed or is configured, so hide it
            pintf->Hide(i, FALSE);
            cConfigs = pintf->GetConfigs(SHWZCF_NET_BROADCAST, &pwlc);
        }
        else
        {
            // The network has not been dismissed, skip it
            ++i;
        }
    }

Error:
    return hr;
}

HRESULT CWZCManagerWireless::UpdateScanInterval(BOOL fForceReset)
{
    HRESULT   hr = S_OK;
    UINT      csMinimum = (UINT)-1;
    CCallback *pcb;

    // Find the minimum requested interval
    for (pcb = m_pcbHead; pcb; pcb = pcb->pcbNext)
    {
        UINT uInterval = pcb->uScanInterval - 1;
        if (uInterval < csMinimum)
        {
            csMinimum = uInterval;
        }
    }

    // Did the interval change?
    CBREx(++csMinimum != m_uScanInterval || fForceReset, S_FALSE);
    VHR(StopScanThread());

    // Set the interval
    m_uScanInterval = csMinimum;

    // Start the scan thread if necessary
    if (0 != m_uScanInterval)
    {
        hr = StartScanThread();
        CHR(hr);
    }

Error:
    return hr;
}

DWORD WINAPI CWZCManagerWireless::UpdateStatusThread(LPVOID pvParam)
{
    LOG_ENTRY le;
    DWORD     dwBytes;
    DWORD     dwFlags;
    HWND      hwndSink;
    NDISUIO_REQUEST_NOTIFICATION ndisuiorn;
    NDISUIO_DEVICE_NOTIFICATION  ndisDeviceNotif;

    // Do NOT access s_hStatusThreadStopEvent or s_hStatusThread directly, as
    // they will change before this thread exits. Instead we use the input
    // parameter to get the stop event handle
    HANDLE rghWait[3] = { (HANDLE)pvParam, s_hWZCMsgQueue, s_hWZCNdisuioMsgQueue};

    // The message queue should ALWAYS be valid
    ASSERT(NULL != s_hWZCMsgQueue);
    ASSERT(NULL != s_hWZCNdisuioMsgQueue);

    // Notify the Shell that Wi-Fi is live
    SetWirelessStateFlags(WSF_HARDWARE_PRESENT, WSF_HARDWARE_PRESENT);

    // Request device notifications
    ndisuiorn.hMsgQueue = rghWait[2];
    ndisuiorn.dwNotificationTypes = (NDISUIO_NOTIFICATION_DEVICE_POWER_UP |
                                     NDISUIO_NOTIFICATION_DEVICE_POWER_DOWN);

    VERIFY(DeviceIoControl(g_hNdisUio, IOCTL_NDISUIO_REQUEST_NOTIFICATION,
                           &ndisuiorn, sizeof(ndisuiorn),
                           NULL, 0x00, NULL, NULL));

    while (1)
    {
        switch(WaitForMultipleObjects(ARRAYSIZE(rghWait), rghWait, FALSE, INFINITE))
        {
        case WAIT_OBJECT_0+1:
            // Read ALL of the messages in the queue before resuming the wait
            while (ReadMsgQueue(rghWait[1], &le, sizeof(LOG_ENTRY), &dwBytes, 0, &dwFlags))
            {
                // Validate the message
                if (sizeof(LOG_ENTRY) != dwBytes ||
                    CURRENT_LOGGING_VERSION != le.dwCurrentLogVersion)
                {
                    continue;
                }

                // Dispatch the message
                hwndSink = FindDevice(le.ptcAdapterName);
                if (hwndSink)
                {
                    OnWZCMessage(hwndSink, &le);
                }
            }
            continue;
            break;

        case WAIT_OBJECT_0+2:
            // Read ALL of the messages in the queue before resuming the wait
            while (ReadMsgQueue(rghWait[2], &ndisDeviceNotif, sizeof(NDISUIO_DEVICE_NOTIFICATION), &dwBytes, 0, &dwFlags))
            {
                // Validate the message
                if (sizeof(NDISUIO_DEVICE_NOTIFICATION) != dwBytes)
                {
                    continue;
                }

                // Dispatch the message
                hwndSink = FindDevice(ndisDeviceNotif.ptcDeviceName);
                if (hwndSink)
                {
                    PostMessage(hwndSink, WM_WZC_POWERCHANGE, (ndisDeviceNotif.dwNotificationType == NDISUIO_NOTIFICATION_DEVICE_POWER_UP), 0);
                }
            }
            
            continue;
            break;
        default:
            break;
        }
        break;
    }

    // Going down... clear all flags
    SetWirelessStateFlags((UINT)-1, 0);

    //  Cancel the notification request.
    DeviceIoControl(g_hNdisUio, IOCTL_NDISUIO_CANCEL_NOTIFICATION,
                    NULL, 0x00, NULL, 0x00, NULL, NULL);

    // Clean up the handles
    CloseHandle(rghWait[0]);
    CloseMsgQueue(rghWait[1]);
    CloseMsgQueue(rghWait[2]);

    return 0;
}

HRESULT CWZCManagerWireless::ScanThread()
{
    HRESULT       hr = S_OK;
    CWZCInterface intf(m_pszDeviceName);
    HANDLE        hEvent = m_hEventStopScan;

    // Continue scanning until the stop event is set
    do
    {
        // Scan for new networks
        VHR(intf.Rescan());
        Sleep(m_dwRescanTimeout);
        PostMessage(m_hwndSink, WM_WZC_SCANCOMPLETE, FALSE, 0);
    }
    while (WAIT_TIMEOUT == WaitForSingleObject(hEvent, m_uScanInterval * 1000));

//Error:
    CloseHandle(hEvent);
    return hr;
}

HRESULT CWZCManagerWireless::StartScanThread()
{
    HRESULT hr = S_OK;

    // If the thread is already running, we're done
    CBREx(NULL == m_hThreadScan, S_FALSE);

    // Create the thread stop event
    m_hEventStopScan = CreateEvent(NULL, TRUE, FALSE, NULL);
    CWR(NULL != m_hEventStopScan);

    // Create the thread
    m_hThreadScan = CreateThread(NULL, 0, ScanThreadThunk, this, 0, NULL);
    CWR(NULL != m_hThreadScan);

Error:
    // Close the stop event if necessary (normally it's closed by the thread
    // itself)
    if (FAILED(hr) && NULL != m_hEventStopScan)
    {
        CloseHandle(m_hEventStopScan);
        m_hEventStopScan = NULL;
    }

    return hr;
}

HRESULT CWZCManagerWireless::StopScanThread()
{
    HRESULT hr = S_OK;

    // Set the scan thread stop event
    CBREx(NULL != m_hThreadScan, S_FALSE);
    SetEvent(m_hEventStopScan);

    // Wait for the thread to exit (abandon the thread if it seems hung)
    VERIFY(WAIT_OBJECT_0 == WaitForSingleObject(m_hThreadScan, 5000));
    CloseHandle(m_hThreadScan);

    // Reset the thread variables
    m_hThreadScan = NULL;
    m_hEventStopScan = NULL;

Error:
    return hr;
}

int CWZCManagerWireless::WritePassword(PWLC pwlc, LPCTSTR pszPassword, ULONG ulEncryption, ULONG ulAuthenticationType)
{
    int  nResult = TRUE;
    WLC  wlcTemp = { 0 };
    CHAR szBuffer[65];  // maximum length is 64
    UINT cchPassword = 0;
    UINT i;

    // Check inputs
    if (NULL == pwlc)
    {
        // Just use a temporary buffer
        pwlc = &wlcTemp;
    }

    // Initialize the key flags
    pwlc->dwCtlFlags &= ~(WZCCTL_WEPK_XFORMAT | WZCCTL_WEPK_PRESENT);

    if (NULL != pszPassword)
    {
        // Convert the password back into ASCII characters
        cchPassword = WideCharToMultiByte(CP_ACP, 0, pszPassword, -1, szBuffer, ARRAYSIZE(szBuffer), NULL, NULL);
        if (0 == cchPassword)
        {
            nResult = FALSE;
            goto Error;
        }

        // Result includes terminating null
        cchPassword -= 1;
    }

    if (Ndis802_11EncryptionDisabled == ulEncryption)  // No encryption
    {
        // The only valid key is an empty one
        if (0 != cchPassword)
        {
            nResult = FALSE;  // not really a length error, kinda generic
            goto Error;
        }
    }
    else if ((Ndis802_11Encryption2Enabled == ulEncryption) ||  // TKIP
             (Ndis802_11Encryption3Enabled == ulEncryption))    // AES
    {
        // TKIP and AES keys have to be at least 8 characters, and no more than 
        // 64 characters
        if (cchPassword < 8 || cchPassword > 64)
        {
            nResult = -1;
            goto Error;
        }

        // If the string is less than 64 characters, it needs to be converted
        if (cchPassword < 64)
        {
            WZCPassword2Key(pwlc, szBuffer);
            goto Success;
        }
        else
        {
            ASSERT(64 == cchPassword);
            
            //
            // For 64 length inputs for WPA[2]-PSK / WPA-NONE - TKIP/AES, we 
            // need to ensure that the string is HEX
            //

            if ((Ndis802_11AuthModeWPANone == ulAuthenticationType || 
                 IsAnyWPAPSK((NDIS_802_11_AUTHENTICATION_MODE)ulAuthenticationType)) &&
                !IsHexString((LPCSTR)szBuffer))
            {
                nResult = FALSE;
                goto Error;
            }
        }
    }
    else if (Ndis802_11Encryption1Enabled == ulEncryption)  // WEP
    {
        switch (cchPassword)
        {
        default:
            nResult = -1;
            goto Error;

        case 5:
        case 13:
            // all valid ASCII inputs
            memcpy(pwlc->KeyMaterial, szBuffer, cchPassword);
            pwlc->KeyLength = cchPassword;
            goto Success;

        case 10:
        case 26:
            // Ensure that we've got valid hex input
            if (!IsHexString((LPCSTR)szBuffer))
            {
                nResult = FALSE;
                goto Error;
            }
            break;
        }
    }

    // Convert from hex to binary
    for (i = 0; i != cchPassword; i += 2)
    {
        pwlc->KeyMaterial[i>>1] = HEX(szBuffer[i]) << 4 | HEX(szBuffer[i+1]);
    }

    pwlc->KeyLength = cchPassword / 2;

Success:
    // Set the flags
    pwlc->dwCtlFlags |= WZCCTL_WEPK_PRESENT;

    // Hash the key before sending off
    static const BYTE rgbFakeKeyMaterial[] = {0x56, 0x09, 0x08, 0x98, 0x4D, 0x08, 0x11, 0x66, 0x42, 0x03, 0x01, 0x67, 0x66};
    for (i = 0; i != WZCCTL_MAX_WEPK_MATERIAL; ++i)
    {
        pwlc->KeyMaterial[i] ^= rgbFakeKeyMaterial[(7*i)%13];
    }

Error:
    return nResult;
}

HRESULT CWZCManagerWireless::RefreshWifiTimeout()
{
    HRESULT hr;
    
    m_uWifiTimeoutPeriod = 0;
    
    // get the updated timeout value and convert from seconds to milliseconds
    CHR(SHWZCGetWifiTimeoutPeriod(&m_uWifiTimeoutPeriod));
    m_uWifiTimeoutPeriod *= 1000;

    if(m_uWifiTimeoutPeriod != 0)
    {
        StartWifiTimeoutTimer();
    }
    else
    {
        // since option selected was Never, just kill the timer.
        StopWifiTimeoutTimer();
    }

    Error:
        return hr;
}

void CWZCManagerWireless::StopWifiTimeoutTimer()
{
    KillTimer(m_hwndSink, IDT_WIFI_TIMEOUT);
}

void CWZCManagerWireless::StartWifiTimeoutTimer()
{
    HRESULT hr=S_OK;
    DWORD dwWifiStatus = 0;
        
    CBR((m_uWifiTimeoutPeriod>0));    
    
    GetWirelessStateFlags(&dwWifiStatus);
    if(!(dwWifiStatus & WSF_CONNECTED) && (dwWifiStatus & WSF_POWER_ON))
    {
       SetTimer(m_hwndSink, IDT_WIFI_TIMEOUT, m_uWifiTimeoutPeriod, NULL);
    }    
Error:
    return;
}

void CWZCManagerWireless::WifiTimeoutRegCallback(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData)
{
    CWZCManagerWireless* pMW = (CWZCManagerWireless*) dwUserData;
    
    if(pMW)
    {
        pMW->RefreshWifiTimeout();
    }
}

CWZCManagerWireless::CWZCManagerWireless() :
    m_intf(NULL),
    m_fOverride(FALSE),
    m_fCheckUIBlock(FALSE),
    m_pcbHead(NULL),
    m_uScanInterval(0),
    m_dwRescanTimeout(0),
    m_hThreadScan(NULL),
    m_hEventStopScan(NULL),
    m_hEventDriverStatus(NULL),
    m_fValidScan(FALSE),
    m_fShellReady(FALSE),
    m_asCurrent8021XState(WZC_AUTHSTATE8021X_RESET),
    m_uHardwareFlags(0),
    m_uWifiTimeoutPeriod(0),
    m_hWifiTimeoutRegNotify(NULL)
{
    ZeroMemory(&m_wlcKey, sizeof(WLC));
    m_wlcKey.Length = sizeof(WLC);
}

CWZCManagerWireless::~CWZCManagerWireless()
{
    CCallback *pcb;
    CCallback *pcbNext = NULL;

    // Notify callbacks
    PostCallback(SHWZC_EVENT_ADAPTER, FALSE);

    // Delete the callback objects
    for (pcb = m_pcbHead; pcb; pcb = pcbNext)
    {
        pcbNext = pcb->pcbNext;
        delete pcb;
    }

    // Stop the scanning thread
    StopScanThread();

    // Free the key configuration
    WZCFreeConfig(&m_wlcKey);

    if (m_hwndSink)
    {
        // Close the sink window (will clean up in the context of the UI thread)
        SendMessage(m_hwndSink, WM_WZC_CLOSE, 0, 0);
        m_hwndSink = NULL;
    }

    // Unregister the sink window class. This will fail if there are still
    // other sink windows open.
    if (0 != s_atmSinkClass &&
        UnregisterClass((LPCTSTR)s_atmSinkClass, g_hInstResDPI))
    {
        s_atmSinkClass = 0;
    }
}

HRESULT CWZCManagerWireless::CNetIDList::SortedInsert(PCWLC pwlc, UINT cConfigs)
{
    HRESULT hr = S_OK;
    PCWLC   pwlcEnd = pwlc + cConfigs;
    CNetID  *pnid = NULL;

    if (!Valid())
    {
        hr = Initialize(cConfigs);
        CHR(hr);
    }

    // Add the SSIDs to the list
    for (; pwlc != pwlcEnd; ++pwlc)
    {
        // Create a new netID object
        pnid = new CNetID;
        CPR(pnid);

        pnid->Assign(pwlc);
        hr = SortedInsertPtr(pnid, CompareProc, NULL);
        CHR(hr);
        pnid = NULL;  // will be deleted by list object
    }

Error:
    if (pnid)
    {
        delete pnid;
    }

    return hr;
}

int CALLBACK CWZCManagerWireless::CNetIDList::CompareProc(const CNetID* pnid1, const CNetID* pnid2, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    return pnid1->Compare(pnid2, FALSE);
}

HRESULT CWZCManagerWireless::CAiringList::Assign(CWZCInterface *pintf)
{
    HRESULT hr = S_OK;
    UINT    cVisible;
    PCWLC   pwlc;
    UINT    cbVisible;
    UINT    cbAlloc;

    // Clear the current list
    Clear();

    // Are there any visible networks?
    cVisible = pintf->GetConfigs(SHWZCF_NET_BROADCAST, &pwlc);
    CBREx(0 != cVisible, S_FALSE);

    // How many bytes do we need to allocate?
    CBRA(UMul(sizeof(WLC), cVisible, &cbVisible));
    CBRA(UAdd(sizeof(CFGLIST), cbVisible, &cbAlloc));

    // Allocate a buffer for the visible networks
    m_pcfgl = (PCFGLIST)LocalAlloc(LMEM_FIXED, cbAlloc);
    CPR(m_pcfgl);

    // Fill in the data
    m_pcfgl->NumberOfItems = cVisible;
    m_pcfgl->Index = 0;
    memcpy(m_pcfgl->Config, pwlc, cbVisible);

Error:
    return hr;
}

void CWZCManagerWireless::CAiringList::Clear()
{
    LocalFree(m_pcfgl);
    m_pcfgl = NULL;
    m_uActive = (UINT)-1;
}

///</topic_scope>
