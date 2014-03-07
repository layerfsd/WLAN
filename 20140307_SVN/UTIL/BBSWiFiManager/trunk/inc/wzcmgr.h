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
// Filename:    wzcmgr.h
//
// Purpose:     WZC Manager declarations
//
//==========================================================================*
//==========================================================================*
#pragma once

#include <windows.h>
#include <wzcmsq.h>
#include <dpatemp.h>
#include <regext.h>
#include "wzcuip.h"
#include "wzcintf.h"


#define PWDUI_WEP           0x0000
#define PWDUI_WPA           0x0001
#define PWDUI_ERR_FORMAT    0x0002
#define PWDUI_ERR_GENERIC   0x0004

#define WZC_REGKEY_BASE      TEXT("ControlPanel\\WiFi")
#define WZC_REGKEY_CALLBACKS TEXT("Callback")
#define WZC_REGKEY_DISABLEFASTCONNECT TEXT("DisableFastConnect")

// Maximum delay (60 seconds) and default delay (10 seconds) between the home 
// screen being drawn and wifi notification UI if wifi adapter is turned on at boot
#define MAX_UI_DELAY_ON_BOOT        0x003C
#define DEFAULT_UI_DELAY_ON_BOOT    0x000A

enum TimerID
{
    IDT_SHELLSTATUS_CHECK = 100,
    IDT_UI_DELAY_AFTER_BOOT,
    IDT_WIFI_TIMEOUT
};

enum PrivateMessage
{
    PM_GETNETLIST,
    PM_FINDOWNER,
    PM_REGISTERCB,
    PM_UNREGISTERCB,
    PM_GETSTATE,
    PM_SETSTATE,
    PM_SETNETWORKS,
    PM_HWFLAGS,
    PM_FINDADAPTER
};

enum GlobalMessage
{
    SHWZCM_GETNETLIST = WM_APP,
    SHWZCM_GETNETWORK,
    SHWZCM_READNETWORK,
    SHWZCM_REGISTERCB,
    SHWZCM_UNREGISTERCB,
    SHWZCM_CLEARNETWORK,
    SHWZCM_GETSTATE,
    SHWZCM_SETSTATE,
    SHWZCM_HWFLAGS,
    SHWZCM_SIGNALSTRENGTH
};

enum GlobalEvent
{
    SHWZCE_RESCAN,
    SHWZCE_DELETEALL,
    SHWZCE_UPDATE_ASSOCIATION
};

// 8021X States
enum WZC_AUTHSTATE8021X
{
    WZC_AUTHSTATE8021X_RESET,           // No 8021X Authentication in progress
    WZC_AUTHSTATE8021X_ASSOCIATED,      // Associated, 8021X required, but not authenticated
    WZC_AUTHSTATE8021X_AUTHENTICATED    // Authenticated via 8021X
};

struct SETNETWORKHDR
{
    DWORD fSetState :1;  // set the state
    DWORD fComplete :1;  // this is the complete network list... delete unspecified networks
    DWORD dwState;       // the state to set
    UINT  cNetworks;     // the number of networks to set
};

struct SETNETWORKITEM
{
    SHWZCNET wzcnet;     // MUST be first in order for SHWZCNETEX to work
    UINT     cbEapData;
    WORD     wMask;
    TCHAR    szKey[65];
};

// This base class implements the core functionality of a network, whether it's
// wired or wireless
class CWZCManager
{
public:
    typedef HRESULT (*PFNENUMCALLBACK)(HWND hwndSink, LPVOID pvParam);

protected:
    LPTSTR m_pszDeviceName;
    HWND   m_hwndSink;
    HWND   m_hwndUI;
    BOOL   m_fConnected;

private:
    friend class CManagerList;
    CWZCManager *m_pmgrNext;

    HANDLE m_hConfigMutex;

public:
    static HRESULT Create(LPCTSTR pszDeviceName, BOOL fWireless, __notnull CWZCManager **ppmgr);
    static HRESULT EnumerateWindows(PFNENUMCALLBACK pfnCallback, LPVOID pvParam);
    void RemoveCard();

    HWND GetSinkWindow() const { return m_hwndSink; }
    static BOOL IsDialogMessage_WZC(HWND hDlg, LPMSG lpMsg);
    void OnConnectionStatusUpdate(BOOL fConnected);

    BOOL IsConnected() const { return m_fConnected; }

    // utility functions
    static HRESULT SendAllPrivateMessage(UINT msg, WPARAM wp, LPARAM lp);

protected:
    virtual HRESULT Initialize(LPCTSTR pszDeviceName);
    HRESULT RegisterClass(LPCTSTR pszClassName, ATOM *patm);
    HRESULT CreateSinkWindow(ATOM atmSinkClass);

    // These two functions are protected by a critical section
    virtual HRESULT OnListInsert(__notnull LPVOID *ppVoid) { return S_OK; }
    virtual LPVOID OnListRemove() { return NULL; }

    // This function is OUTSIDE the critical section, and it's only called if
    // OnListRemove returns non-NULL or OnListInsert returns non-NULL in ppVoid
    virtual void OnListCleanup(LPVOID pvParam) {}

    // message handlers
    virtual LRESULT OnMessage(UINT msg, WPARAM wp, LPARAM lp);
    virtual void OnClose();
    virtual void OnDestroy() {}
    virtual void OnTimer(UINT uTimerID) {}
    virtual HRESULT OnPrivateMessage(UINT msg, WPARAM wp, LPARAM lp) { return S_OK; }
    virtual void OnGlobalEvent(UINT uEventID) {}

    // configuration functions
    void PostConfigMessage(UINT uMsg, LPARAM lp);
    HRESULT TryConfigMessage(UINT uMsg, LPARAM lp);
    void OnConfigMessage(UINT uMsg, LPARAM lp);

    // connection functions
    virtual void OnConnectResult(HRESULT hrConnect);
    virtual HRESULT OnConnect() { return S_OK; }
    virtual void OnDisconnect();
    virtual void OnConnectFailed(HRESULT hrConnect) {}
    virtual BOOL CheckIPState(IPSTATE state) = 0;
    virtual HRESULT OnUpdateAssociation() { return S_FALSE; }

    // Hardware management
    virtual void OnRemoveCard();

    // UI functions
    virtual void DestroyUI();

    // utility functions
    virtual void ForceDisconnect() {}

private:
    static LRESULT CALLBACK SinkWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static HRESULT PrivateMessageCallback(HWND hwndSink, LPVOID pvParam);

public:
    CWZCManager() : m_pszDeviceName(NULL), m_hwndSink(NULL), m_hwndUI(NULL), m_pmgrNext(NULL), m_hConfigMutex(NULL), m_fConnected(FALSE) {}
    virtual ~CWZCManager();
};

class CWZCManagerWired : public CWZCManager
{
private:
    static ATOM s_atmSinkClass;

protected:
    // protected by a critical section
    virtual HRESULT OnListInsert(__notnull LPVOID *ppVoid);

    // connection functions
    virtual BOOL CheckIPState(IPSTATE state);
    virtual HRESULT OnConnect();
    virtual void OnDisconnect();

public:
    virtual ~CWZCManagerWired();
};

class CWZCManagerWireless : public CWZCManager
{
public:
    struct CBINFO
    {
        UINT  uCallbackMsg;
        UINT  uScanInterval;
        DWORD dwFlags;
    };

protected:

    typedef DPATemplate<CNetID> CNetIDListBase;
    class CNetIDList : public CNetIDListBase
    {
    public:
        HRESULT SortedInsert(PCWLC pwlc, UINT cConfigs);
        BOOL Find(const CNetID *pnid) const { int nItem; return (Valid() && SUCCEEDED(Search(pnid, CompareProc, NULL, TRUE, &nItem))); }

    private:
        static int CALLBACK CompareProc(const CNetID *pnid1, const CNetID *pnid2, LPARAM lParam);
    };

    class CAiringList
    {
    private:
        PCFGLIST m_pcfgl;
        UINT     m_uActive;

    public:
        HRESULT Assign(CWZCInterface *pintf);
        PWLC GetList() const { return (m_pcfgl ? m_pcfgl->Config + m_pcfgl->Index : NULL); }
        UINT GetCount() const { return (m_pcfgl ? m_pcfgl->NumberOfItems - m_pcfgl->Index : 0); }
        void Clear();

        void SetActive(UINT uActive) { m_uActive = uActive; }
        UINT GetActive() const { return m_uActive; }
        PWLC GetActiveNetwork() const { return (m_uActive < GetCount() ? GetList() + m_uActive : NULL); }

    public:
        CAiringList() : m_pcfgl(NULL), m_uActive((UINT)-1) {}
        ~CAiringList() { Clear(); }
    };

private:

    class CCallback : public CBINFO
    {
    public:
        const HWND hwndCallback;
        CCallback  *pcbNext;

    public:
        CCallback(HWND hwnd, const CBINFO *pcbi) : CBINFO(*pcbi), hwndCallback(hwnd), pcbNext(NULL) {}
    };

    struct FINDOWNERINFO;

protected:
    CAiringList m_airing;
    WLC         m_wlcKey;

private:
    CWZCInterface m_intf;
    CNetIDList    m_nidlDismissed;

    CCallback *m_pcbHead;
    UINT      m_uScanInterval;
    DWORD     m_dwRescanTimeout;
    HANDLE    m_hThreadScan;
    HANDLE    m_hEventStopScan;
    HANDLE    m_hEventDriverStatus;
    CNetID    m_nidConnecting;
    WZC_AUTHSTATE8021X m_asCurrent8021XState;
    UINT      m_uHardwareFlags;
    UINT        m_uWifiTimeoutPeriod;
    HREGNOTIFY  m_hWifiTimeoutRegNotify; 

    DWORD m_fOverride :1;
    DWORD m_fCheckUIBlock :1;
    DWORD m_fValidScan :1;
    DWORD m_fShellReady :1;

    static ATOM   s_atmSinkClass;
    static UINT   s_uEventMsg;
    static HANDLE s_hStatusThread;
    static HANDLE s_hStatusThreadStopEvent;
    static HANDLE s_hWZCMsgQueue;
    static HANDLE s_hWZCNdisuioMsgQueue;
    static UINT   s_cRefStatusThread;

public:
    static HWND FindOwnerWindow(__in const CNetID *pnid);
    static HWND FindAdapterWindow(__in LPCTSTR pszAdapter);

    // Returns TRUE if OK, FALSE for generic error, and -1 for length error
    static int WritePassword(PWLC pwlc, LPCTSTR pszPassword, ULONG ulEncryption, ULONG ulAuthenticationType);

    // Returns a value suitable for UI display
    static UINT GetSignalStrength(__in LPCTSTR pszAdapter);
    static UINT GetSignalStrength(int nRSSI, DEVICETYPE dtType);

    // utility functions
    static BOOL IsEncrypted(ULONG ulPrivacy);

protected:

    // These two functions are protected by a critical section
    virtual HRESULT OnListInsert(__notnull LPVOID *ppVoid);
    virtual LPVOID OnListRemove();

    // This function is OUTSIDE the critical section, and it's only called if
    // OnListRemove returns non-NULL or OnListInsert returns non-NULL in ppVoid
    virtual void OnListCleanup(LPVOID pvParam);

    // message handlers
    virtual LRESULT OnMessage(UINT msg, WPARAM wp, LPARAM lp);
    virtual HRESULT OnPrivateMessage(UINT msg, WPARAM wp, LPARAM lp);
    HRESULT OnScanComplete();
    virtual void OnGlobalEvent(UINT uEventID);
    virtual void OnDestroy();
    virtual void OnTimer(UINT uTimerID);

    // power notifications
    virtual HRESULT OnPowerOn();
    virtual HRESULT OnPowerOff();

    // state message handlers
    static void OnWZCMessage(HWND hwndSink, __notnull const LOG_ENTRY *ple);
    HRESULT OnEapolClose(const CNetID *pnid);
    virtual void OnConnectResult(HRESULT hrConnect);
    virtual HRESULT OnConnect();
    virtual void OnDisconnect();
    virtual BOOL CheckIPState(IPSTATE state);
    virtual HRESULT OnUpdateAssociation();

    // notification functions
    HRESULT DoForegroundScan();
    HRESULT QueryInterface(CWZCInterface *pintf) const;

    // connection functions
    HRESULT DoConnect(PWLC pwlc);
    HRESULT BeginConnect(PWLC pwlc);

    // SHWZC functions
    PCWLC FindNetwork(__in const CNetID *pnid, __out __opt DWORD *pdwFlags = NULL) const;
    HRESULT GetNetwork(PSHWZCNET pwzcnet, LPVOID pvData, UINT cbData) const;
    HRESULT SetNetworks(__in const SETNETWORKHDR *psnh, __in const SETNETWORKITEM *psni, __in __opt const BYTE *pbEapData);
    HRESULT DeleteNetwork(const CNetID *pnid, BOOL fKeyError);
    HRESULT DeleteAllNetworks();

    HRESULT SetHardwareFlag(UINT uFlag, BOOL fSet, DWORD dwRegFlag);
    UINT GetHardwareFlags() const { return m_uHardwareFlags; }
    virtual void OnRemoveCard();

    HRESULT DismissAiringList();
    void ClearStateCache();

    // UI functions
    virtual HRESULT OnCheckNewNetworks();
    virtual BOOL IsUIBlocked() const;
    void OnUnblockUI();

    virtual HRESULT ShowNewNetworksUI() = 0;
    virtual HRESULT ShowPasswordDialog(DWORD dwState) = 0;
    virtual void DestroyUI();

    // utility functions
    HRESULT ForceReconnect();
    virtual void ForceDisconnect();
    PCWLC GetConnectedNetwork() const;
    HRESULT RefreshWifiTimeout();
    
    virtual void StopWifiTimeoutTimer();
    virtual void StartWifiTimeoutTimer();

private:
    static DWORD WINAPI UpdateStatusThread(LPVOID pvParam);
    static DWORD WINAPI ScanThreadThunk(LPVOID pvParam) { return SUCCEEDED(static_cast<CWZCManagerWireless*>(pvParam)->ScanThread()); }
    static HWND FindDevice(LPCTSTR pszDeviceName);

    HRESULT GetCurrentNetworks(HDSA hdsa, DWORD dwFlags) const;
    HRESULT ApplyNetworkList(__in CWZCInterface *pintf, DWORD dwApplyFlags = 0);
    void AssignConnectingNetwork(__in const CNetID *pnid);

    HRESULT LoadRegistryData();
    HRESULT LoadRegistryPresets();
    static HRESULT DeleteRegistryPreset(__in const CNetID *pnid);

    HRESULT ScanThread();
    HRESULT StartScanThread();
    HRESULT StopScanThread();
    HRESULT UpdateScanInterval(BOOL fForceReset);

    HRESULT LoadCallbackWindow(HWND hwnd);
    HRESULT RegisterCallbackWindow(HWND hwnd, const CBINFO *pcbi, BOOL fResetInterval);
    void UnregisterCallbackWindow(HWND hwnd);
    void PostCallback(UINT msg, LPARAM lp = 0) const;

    void OnFindOwner(FINDOWNERINFO *pfoi) const;
    HRESULT OnFindAdapter(LPCTSTR pszAdapter, HWND *phwndSink) const;

    HRESULT OnPowerChange(BOOL fOn);
    HRESULT SetDefaultPowerState();

    void OnUnblockUIPost();
    HRESULT UpdateAiringList(HDSA hdsa, BOOL fDismiss);
    HRESULT CheckNewNetworks();

    static DWORD GetPasswordState(__in PCWLC pwlc);
    HRESULT OnConnectBegin(__in __opt const CNetID *pnid);
    void OnConnectStopped();
    HRESULT OnAuthenticationFailure(HRESULT hrConnect, __in __opt CNetID *pnid);
    HRESULT OnConnectCompleteSent(HRESULT hrConnect, __in __opt const CNetID *pnid);
    static void WifiTimeoutRegCallback(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData);

public:
    CWZCManagerWireless();
    virtual ~CWZCManagerWireless();
};

// wzcshell message handlers
HRESULT SHWZCRegisterCallbackWindow_I(WPARAM wParam, LPARAM lParam);
HRESULT SHWZCGetCurrentNetworksMessage_I(WPARAM wParam, LPARAM lParam);
HRESULT SHWZCUnregisterCallbackWindow_I(WPARAM wParam, LPARAM lParam);
BOOL    SHWZCGetNetwork_I(WPARAM wParam, LPARAM lParam);
LRESULT SHWZCGetState_I(WPARAM wParam, LPARAM lParam);
LRESULT SHWZCSetState_I(WPARAM wParam, LPARAM lParam);
LRESULT SHWZCGetHardwareFlags_I(WPARAM wParam, LPARAM lParam);
LRESULT ReceiveDataMessage(DWORD dwCode, __in_bcount(cbData) LPCVOID pvData, DWORD cbData);

// wzcshell utility functions
void SHWZCGetDefaults(__inout PSHWZCNET pwzcnet, BOOL fAdhoc);
void SHWZCValidateSettings(__notnull PSHWZCNET pwzcnet);
HRESULT UnregisterCallback(HWND hwndCallback);
