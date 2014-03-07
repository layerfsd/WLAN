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
// Filename:    wzcbubble.cpp
//
// Owner:        
//
// Purpose:     Notification bubble window for net card config 
//
//==========================================================================*

#include <windows.h>
#include <wpcpriv.h>
#include <cshguid.h>
#include <ehm.h>

#include "netuip.h"
#include "wzcuip.h"
#include "wzcmgr_p.h"
#include "wzcqcfg.h"
#include "connmgr_priv.h"
#include "wzcguid_p.h"
#include "appmetc.h"
#include "resource.h"
#include <bmphelpr.h>


//
// defines
//
//HTML Resource IDs
#define HTMLRES_WZC_HTML_HEADER                        _T("WZC_HTML_HEADER.HTM")
#define HTMLRES_WZC_HTML_MULTIPART_HEADER              _T("WZC_HTML_MULTIPART_HEADER.HTM")
#define HTMLRES_WZC_HTML_TAIL                          _T("WZC_HTML_TAIL.HTM")
#define HTMLRES_WZC_CONNECTION_BUBBLE_SINGLENET        _T("WZC_CONNECTION_BUBBLE_SINGLENET.HTM")
#define HTMLRES_WZC_CONNECTION_BUBBLE_WIRED            _T("WZC_CONNECTION_BUBBLE_WIRED.HTM")
#define HTMLRES_WZC_CONNECTION_BUBBLE_WIRELESS         _T("WZC_CONNECTION_BUBBLE_WIRELESS.HTM")
#define HTMLRES_WZC_CONNECTION_BUBBLE_MULTIPLENET      _T("WZC_CONNECTION_BUBBLE_MULTIPLENET.HTM")
#define HTMLRES_WZC_CONNECTION_BUBBLE_MULTIPLENET_OVER _T("WZC_CONNECTION_BUBBLE_MULTIPLENET_OVER.HTM")
#define HTMLRES_WZC_SSID_NETWORK_DEFAULT_RADIO_BUTTON  _T("WZC_SSID_NETWORK_DEFAULT_RADIO_BUTTON.HTM")
#define HTMLRES_WZC_SSID_NETWORK_RADIO_BUTTON          _T("WZC_SSID_NETWORK_RADIO_BUTTON.HTM")
#define HTMLRES_WZC_NETKEY_BUBBLE_HEAD                 _T("WZC_NETKEY_BUBBLE_HEAD.HTM")
#define HTMLRES_WZC_NETKEY_BUBBLE_TAIL                 _T("WZC_NETKEY_BUBBLE_TAIL.HTM")

#define BUBBLE_WINDOW_TIMEOUT 60

enum
{
    BUBBLE_WIRELESS_LIST = 0x1000,
    BUBBLE_WIRELESS_KEY,
    BUBBLE_WIRED
};

static const TCHAR c_szCommandSelect[]          = TEXT("ConnectBubble://Select"),
                   c_szCommandConnection[]      = TEXT("ConnectBubble://Connect"),
                   c_szCommandWiredConnection[] = TEXT("WiredConnectBubble://Connect"),
                   c_szCommandWepKeyInput[]     = TEXT("WepKeyBubble://WepKey");

static const TCHAR c_szInputCtrlNameWepKey[]      = TEXT("WEPKey"),
                   c_szInputCtrlNameMetaNetwork[] = TEXT("MetaNetwork");

#define COMMAND_SETTING IDM_WZC_COMMAND_SETTING
#define COMMAND_CANCEL  IDM_WZC_COMMAND_CANCEL

//
// Macros
//
#define SAFE_FREE(p) do{LocalFree(p);(p) = NULL;}while(0,0)

#define _LoadString(ID) LoadString(HINST_RESDLL, ID, NULL, 0)
#define _LoadHTML(ID)   LoadHTML(HINST_RESDLL, ID, NULL, 0)

static HRESULT BuildBubbleHTML(__in LPCTSTR pszCurrent, __in LPCTSTR pszCommand, __out LPTSTR *ppszHTML, BOOL fMultipart)
{
    HRESULT hr = S_OK;
    LPCTSTR pszFormat;
    LPVOID  rgpvArgs[1];
    LPTSTR  pszOutput = NULL;

    // Load the HTML header
    if(fMultipart)
    {
        pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_HTML_MULTIPART_HEADER);
    }
    else
    {
        pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_HTML_HEADER);
    }
    CBRA(NULL != pszFormat);

    // Format the text
    rgpvArgs[0] = (LPVOID)pszCommand;
    CWR(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
        pszFormat, 0, 0, (LPTSTR)&pszOutput, 0, (va_list*)rgpvArgs));

    // Load the HTML tail
    pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_HTML_TAIL);
    CBRA(NULL != pszFormat);

    // Build the complete HTML
    hr = ReAllocAndConcat(&pszOutput, pszCurrent);
    CHR(hr);

    hr = ReAllocAndConcat(&pszOutput, pszFormat);
    CHR(hr);

    // Success
    *ppszHTML = pszOutput;
    pszOutput = NULL;

Error:
    LocalFree(pszOutput);
    return hr;
}

static HRESULT BuildSingleNetworkHTML(__in __opt PCWLC pConfig, __out LPTSTR *ppszHTML)
{
    HRESULT hr = S_OK;
    LPCTSTR pszFormat;
    LPVOID  rgpvArgs[2] = { 0 };
    HDC     hdc = NULL;
    CNetID  nid;
    TCHAR   szSSID[MAX_PATH];
    LPTSTR  pszNetText = NULL;
    LPTSTR  pszHTML = NULL;
    LPTSTR  pszOutput = NULL;
    LPTSTR  pszDisplay = NULL;

    // Do we have a wireless network?
    if (NULL != pConfig)
    {
        // Get a screen device context
        hdc = GetDC(NULL);
        CWR(NULL != hdc);

        // Load the wireless HTML
        pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_CONNECTION_BUBBLE_WIRELESS);
        CBRA(NULL != pszFormat);

        // Get the SSID string
        nid.Assign(pConfig);
        hr = nid.GetDisplayName(szSSID, ARRAYSIZE(szSSID));
        CHR(hr);

        hr = IsNetworkSecure(szSSID, pConfig[0].dwCtlFlags &
            WZCCTL_ONEX_ENABLED, pConfig[0].AuthenticationMode,
            pConfig[0].Privacy, &pszDisplay, GetSystemMetrics(SM_CXSCREEN) -
            AM_NETUI_WZCBUBBLESSIDPADDING, TRUE);
        CHR(hr);

        // Format the string
        rgpvArgs[0] = pszDisplay;
        CWR(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
            pszFormat, 0, 0, (LPTSTR)&pszHTML, 0, (va_list*)rgpvArgs));
        
    }
    else
    {
        // Load the wired HTML
        pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_CONNECTION_BUBBLE_WIRED);
        CBRA(NULL != pszFormat);

        // This is the first part of the HTML
        hr = ReAllocAndConcat(&pszHTML, pszFormat);
        CHR(hr);
    }

    // Load the metanetwork names
    CHREx(GetMetaNetworkName(0, (LPCTSTR*)&rgpvArgs[0]), E_PENDING);
    CHREx(GetMetaNetworkName(1, (LPCTSTR*)&rgpvArgs[1]), E_PENDING);

    // Load the "extra" VPN text
    pszFormat = (LPCTSTR)_LoadString(IDS_WZC_CONNECTION_BUBBLE_VPN);
    if (NULL != pszFormat)
    {
        CWR(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
            pszFormat, 0, 0, (LPTSTR)&pszNetText, 0, (va_list*)rgpvArgs));
        rgpvArgs[0] = pszNetText;
    }

    // Load the metanetwork selection HTML
    pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_CONNECTION_BUBBLE_SINGLENET);
    CBRA(NULL != pszFormat);

    // Format the text
    CWR(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
        pszFormat, 0, 0, (LPTSTR)&pszOutput, 0, (va_list*)rgpvArgs));

    // Append it to the previous text
    hr = ReAllocAndConcat(&pszHTML, pszOutput);
    CHR(hr);

    // Add the surrounding HTML
    pszFormat = (NULL != pConfig ? c_szCommandConnection : c_szCommandWiredConnection);
    hr = BuildBubbleHTML(pszHTML, pszFormat, ppszHTML, FALSE);
    CHR(hr);

Error:
    if (NULL != hdc)
    {
        ReleaseDC(NULL, hdc);
    }

    LocalFree(pszNetText);
    LocalFree(pszHTML);
    LocalFree(pszOutput);
    LocalFree(pszDisplay);
    
    return hr;
}

static HRESULT BuildMultiNetworkHTML(__in_ecount(cConfigs) PCWLC pConfigs, UINT cConfigs, __out LPTSTR *ppszHTML)
{
    HRESULT hr = S_OK;
    UINT    cDisplayConfigs;
    TCHAR   szIndex[2] = { 0 };
    LPCTSTR pszFormat;
    LPVOID  rgpvArgs[2];
    UINT    i;
    CNetID  nid;
    HDC     hdc = NULL;
    TCHAR   szSSID[64];
    LPTSTR  pszOutput = NULL;
    LPTSTR  pszList = NULL;
    LPTSTR  pszDisplay = NULL;

    // We need to have at least one network for this function to work
    CBRA(0 != cConfigs);

    // Determine how many networks to display
    cDisplayConfigs = (cConfigs > MAX_SSID_IN_BUBBLE_WINDOW ? MAX_SSID_IN_BUBBLE_WINDOW - 1 : cConfigs);

    // Get a screen device context
    hdc = GetDC(NULL);
    CWR(NULL != hdc);

    // Load the "default" network format string
    pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_SSID_NETWORK_DEFAULT_RADIO_BUTTON);
    CBRA(NULL != pszFormat);

    // Get the formatted string
    rgpvArgs[0] = szIndex;
    for (i = 0; i != cDisplayConfigs; ++i)
    {
        // Get the SSID string
        nid.Assign(&pConfigs[i]);
        hr = nid.GetDisplayName(szSSID, ARRAYSIZE(szSSID));
        CHR(hr);

        hr = IsNetworkSecure(szSSID, pConfigs[i].dwCtlFlags &
            WZCCTL_ONEX_ENABLED, pConfigs[i].AuthenticationMode,
            pConfigs[i].Privacy, &pszDisplay, GetSystemMetrics(SM_CXSCREEN) -
            (IsFESystem() ? AM_NETUI_WZCBUBBLESSIDPADDING_EA :
            AM_NETUI_WZCBUBBLESSIDPADDING), TRUE);
        CHR(hr);

        // Load the "unselected" radio button format text if necessary
        if (i == 1)
        {
            pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_SSID_NETWORK_RADIO_BUTTON);
            CBRA(NULL != pszFormat);
        }

        // Format the string
        szIndex[0] = '0' + i;
        rgpvArgs[1] = pszDisplay;

        CWR(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
            pszFormat, 0, 0, (LPTSTR)&pszOutput, 0, (va_list*)rgpvArgs));

        LocalFree(pszDisplay);
        pszDisplay = NULL;

        // Add this to our SSID list
        hr = ReAllocAndConcat(&pszList, pszOutput);
        CHR(hr);

        // Clean up
        SAFE_FREE(pszOutput);
    }

    // We'll use these arguments
    rgpvArgs[0] = pszList;
    rgpvArgs[1] = &szIndex[1];  // empty string

    // Load the final format text
    pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_CONNECTION_BUBBLE_MULTIPLENET);
    CBRA(NULL != pszFormat);

    // Format the final HTML
    CWR(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
        pszFormat, 0, 0, (LPTSTR)&pszOutput, 0, (va_list *)rgpvArgs));

    // If there are more available networks than we can display, append the
    // "show all networks" text
    if (cConfigs > cDisplayConfigs)
    {
        pszFormat = (LPCTSTR)_LoadHTML(HTMLRES_WZC_CONNECTION_BUBBLE_MULTIPLENET_OVER);
        CBRA(NULL != pszFormat);

        hr = ReAllocAndConcat(&pszOutput, pszFormat);
        CHR(hr);
    }

    // Add the surrounding HTML
    hr = BuildBubbleHTML(pszOutput, c_szCommandSelect, ppszHTML, FALSE);
    CHR(hr);

Error:
    if (NULL != pszDisplay)
    {
        LocalFree(pszDisplay);
    }

    if (NULL != hdc)
    {
        ReleaseDC(NULL, hdc);
    }

    LocalFree(pszOutput);
    LocalFree(pszList);
    return hr;
}

static HRESULT BuildNetworkKeyHTML(__in PCWLC pConfig,
                                   __in LPCTSTR pszTemplate, __out LPTSTR *ppszHTML)
{
    HRESULT hr = S_OK;
    LPVOID  rgpvArgs[1];
    CNetID  nid;
    TCHAR   szSSID[64];
    LPTSTR  pszOutput = NULL;

    // Get the SSID string
    nid.Assign(pConfig);
    hr = nid.GetDisplayName(szSSID, ARRAYSIZE(szSSID));
    CHR(hr);

    // Check if it's too long
    hr = EscapeHtmlString(szSSID, ARRAYSIZE(szSSID));
    CHR(hr);

    // Format the string
    rgpvArgs[0] = szSSID;
    CWR(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
        pszTemplate, 0, 0, (LPTSTR)&pszOutput, 0, (va_list*)rgpvArgs));

    // Add the surrounding HTML
    hr = BuildBubbleHTML(pszOutput, c_szCommandWepKeyInput, ppszHTML, TRUE);
    CHR(hr);

Error:

    LocalFree(pszOutput);
    return hr;
}

HRESULT CWZCManagerWirelessPPC::ShowNewNetworksUI()
{
    HRESULT            hr = S_OK;
    PCWLC              pwlcList = m_airing.GetList();
    UINT               cConfigs = m_airing.GetCount();
    PCWLC              pwlcActive = m_airing.GetActiveNetwork();
    SHNOTIFICATIONDATA shnd = { 0 };
    LPTSTR             pszHTML = NULL;

    // Sanity check
    CBRA(NULL != pwlcList);

    // Set up the softkey data
    shnd.grfFlags = SHNF_HASMENU;
    shnd.skm.cskc = 3;
    shnd.skm.prgskc = (SOFTKEYCMD*) LocalAlloc(LMEM_FIXED, shnd.skm.cskc*sizeof(SOFTKEYCMD));
    CPR(shnd.skm.prgskc);

    // we don't know whether the bubble is for single or multiple networks, so don't set [0].wmCmd yet.
    shnd.skm.prgskc[0].grfFlags = NOTIF_SOFTKEY_FLAGS_SUBMIT_FORM | NOTIF_SOFTKEY_FLAGS_STAYOPEN;
    shnd.skm.prgskc[1].wpCmd = IDM_WZC_NEWNET_DISMISS;
    shnd.skm.prgskc[1].grfFlags = NOTIF_SOFTKEY_FLAGS_DISMISS;
    shnd.skm.prgskc[2].wpCmd = IDM_WZC_NEWNET_STOP_NOTIFICATIONS;
    shnd.skm.prgskc[2].grfFlags = NOTIF_SOFTKEY_FLAGS_DISMISS;

    //  Show List of Network(s).  
    if ( NULL == pwlcActive)
    {
        // Load strings
        if( cConfigs == 1)
        {
           shnd.pszTitle = (LPCTSTR)_LoadString(IDS_WZC_CONNECTION_BUBBLE_SINGLENET_TITLE);
        }
        else
        {
           shnd.pszTitle = (LPCTSTR)_LoadString(IDS_WZC_CONNECTION_BUBBLE_MULTIPLENET_TITLE);
        }
     
        CBRA(NULL != shnd.pszTitle);

        // Load the multiple networks detected menu
        shnd.skm.hMenu = LoadMenu(HINST_RESDLL, MAKEINTRESOURCE(IDR_WZC_MULTI_NEWNET_NOTIFICATION_MENU));
        CBRA(NULL != shnd.skm.hMenu);

        shnd.skm.prgskc[0].wpCmd = IDM_WZC_BUBBLE_SELECT;

        // Build Bubble UI with Network List
        hr = BuildMultiNetworkHTML(pwlcList, cConfigs, &pszHTML);
        CHR(hr);
    }
    else  // single network mode  : Used to Display Bubble  ("Connect to internet /Work")  This is only for  secure networks. 
    {
        // Load strings
        shnd.pszTitle = (LPCTSTR)_LoadString(IDS_WZC_CONNECTION_BUBBLE_SINGLENET_TITLE);
        CBRA(NULL != shnd.pszTitle);

        // Load the single network menu
        shnd.skm.hMenu = LoadMenu(HINST_RESDLL, MAKEINTRESOURCE(IDR_WZC_SINGLE_NEWNET_NOTIFICATION_MENU));
        CBRA(NULL != shnd.skm.hMenu);

        shnd.skm.prgskc[0].wpCmd = IDM_WZC_BUBBLE_CONNECT;

        // Build the HTML
        hr = BuildSingleNetworkHTML(pwlcActive ? pwlcActive : pwlcList, &pszHTML);
        CHR(hr);
    }

    // Set up the notification data
    shnd.cbStruct   = sizeof(SHNOTIFICATIONDATA);
    shnd.dwID       = BUBBLE_WIRELESS_LIST;
    shnd.clsid      = CLSID_WiFiBubble;
    shnd.npPriority = SHNP_INFORM;
    shnd.csDuration = BUBBLE_WINDOW_TIMEOUT;
    shnd.hicon      = DPI_LoadIconOver(HINST_RESDLL, IDI_WZC_CONNECTION_BUBBLE, IDI_OVERRIDE_WZC_CONNECTION_BUBBLE);
    shnd.hwndSink   = GetSinkWindow();
    shnd.pszHTML    = pszHTML;

    // Display the bubble
    CBR(ERROR_SUCCESS == SHNotificationAdd(&shnd));

Error:
    if (NULL != shnd.hicon)
    {
        DestroyIcon(shnd.hicon);
    }

    if (NULL != shnd.skm.hMenu)
    {
        DestroyMenu(shnd.skm.hMenu);
    }

    LocalFree(pszHTML);
    LocalFree(shnd.skm.prgskc);
    return hr;
}

void CWZCManagerWirelessPPC::HideBubble()
{
    // Hide the bubbles
    SHNotificationRemove(&CLSID_WiFiBubble, BUBBLE_WIRELESS_LIST);
    SHNotificationRemove(&CLSID_WiFiPasswordBubble, BUBBLE_WIRELESS_KEY);

    //   Incase User Chooses to  Dismiss the selection  for any reason ( By Pressing Dismiss Button / Time Out )
    StartWifiTimeoutTimer();    
    // Unblock UI
    m_fInSelectUI = FALSE;
    m_nKeyState = -1;
    OnUnblockUI();        
}

HRESULT CWZCManagerWirelessPPC::ShowPasswordDialog(DWORD dwState)
{
    HRESULT            hr = S_OK;
    UINT               uTitleID, uTextID;
    LPTSTR             pszTemplate = NULL;
    LPTSTR             pszHTML = NULL;
    SHNOTIFICATIONDATA shnd = { 0 };

    // Determine which content to show (dwState is a
    // combination of PWDUI_xxx flags)
    switch (dwState)
    {
    case 0:
        uTitleID = IDS_WZC_NETKEY_BUBBLE_WEP_TITLE;
        uTextID  = IDS_WZC_NETKEY_BUBBLE_WEP;
        break;

    case 1:
        uTitleID = IDS_WZC_NETKEY_BUBBLE_WPA_TITLE;
        uTextID  = IDS_WZC_NETKEY_BUBBLE_WPA;
        break;

    case 2:
        uTitleID = IDS_WZC_NETKEY_BUBBLE_RETRY_TITLE;
        uTextID  = IDS_WZC_NETKEY_BUBBLE_WEP_ERR;
        break;

    case 3:
        uTitleID = IDS_WZC_NETKEY_BUBBLE_RETRY_TITLE;
        uTextID  = IDS_WZC_NETKEY_BUBBLE_WPAPSK_ERR;
        break;

    case 4:
    default:
        uTitleID = IDS_WZC_NETKEY_BUBBLE_RETRY_TITLE;
        uTextID  = IDS_WZC_NETKEY_BUBBLE_RETRY;
        break;
    }

    // Load strings
    shnd.pszTitle = (LPCTSTR)_LoadString(uTitleID);
    CBRA(NULL != shnd.pszTitle);

    shnd.rgskn[0].pszTitle = (LPCTSTR)_LoadString(IDS_WZC_CONNECTION_BUBBLE_CONNECT_BUTTON);
    CBRA(NULL != shnd.rgskn[0].pszTitle);

    shnd.rgskn[1].pszTitle = (LPCTSTR)_LoadString(IDS_WZC_CONNECTION_BUBBLE_DISMISS_BUTTON);
    CBRA(NULL != shnd.rgskn[1].pszTitle);

    // Create the HTML template
    hr = ReAllocAndConcat(&pszTemplate, (LPCTSTR)_LoadHTML(HTMLRES_WZC_NETKEY_BUBBLE_HEAD));
    CHR(hr);

    hr = ReAllocAndConcat(&pszTemplate, (LPCTSTR)_LoadString(uTextID));
    CHR(hr);

    hr = ReAllocAndConcat(&pszTemplate, (LPCTSTR)_LoadHTML(HTMLRES_WZC_NETKEY_BUBBLE_TAIL));
    CHR(hr);

    // Build the HTML
    hr = BuildNetworkKeyHTML(&m_wlcKey, pszTemplate, &pszHTML);
    CHR(hr);

    // Set up the softkey data
    shnd.rgskn[0].skc.wpCmd = IDOK;
    shnd.rgskn[0].skc.grfFlags = NOTIF_SOFTKEY_FLAGS_SUBMIT_FORM | NOTIF_SOFTKEY_FLAGS_STAYOPEN;
    shnd.rgskn[1].skc.wpCmd = COMMAND_CANCEL;

    // Set up the notification data
    shnd.cbStruct   = sizeof(SHNOTIFICATIONDATA);
    shnd.dwID       = BUBBLE_WIRELESS_KEY;
    shnd.clsid      = CLSID_WiFiPasswordBubble;
    shnd.npPriority = SHNP_INFORM;
    shnd.csDuration = BUBBLE_WINDOW_TIMEOUT;
    shnd.hicon      = DPI_LoadIconOver(HINST_RESDLL, IDI_WZC_CONNECTION_BUBBLE, IDI_OVERRIDE_WZC_CONNECTION_BUBBLE);
    shnd.hwndSink   = GetSinkWindow();
    shnd.pszHTML    = pszHTML;

    //   Stop Timer  to allow user to enter password/network key  instead of powering down Wifi card. 
    StopWifiTimeoutTimer();
    // Display the bubble
    CBR(ERROR_SUCCESS == SHNotificationAdd(&shnd));
    m_nKeyState = dwState;
    
Error:
    if (NULL != shnd.hicon)
    {
        DestroyIcon(shnd.hicon);
    }

    LocalFree(pszHTML);
    LocalFree(pszTemplate);
    return hr;
}


HRESULT CWZCManagerWirelessPPC::OnMultiSelect(UINT uSel)
{
    HRESULT hr = S_OK;
    PWLC    pwlc;

    // Sanity check
    CBRA(uSel < m_airing.GetCount());
    // Set the "active" configuration
    m_airing.SetActive(uSel);    
    pwlc = m_airing.GetActiveNetwork();
    CBRA( NULL != pwlc );

    if ( IsSecure(pwlc) )
    {
         hr = ShowNewNetworksUI();
         CHR(hr);
         // The user has selected a network
         m_fInSelectUI = TRUE;
    }
    else
    {   
         // Incase of unsecure network go ahead connect  with default as internet.. 
        // Default assumtion is  Internet 
        CHR( OnConnectButton(GetMetaNetwork_InternetIndex()) );      
    }    

Error:
    return hr;
}

HRESULT CWZCManagerWirelessPPC::OnConnectButton(UINT uMeta)
{
    HRESULT hr = S_OK;
    PWLC    pwlc = m_airing.GetActiveNetwork();
    CNetID  nid;

    // When there's only one network available, the user gets the single
    // network bubble without selecting a particular network from a list. In
    // this case, we set the first (and only) network to be the "active"
    // network.
    if (NULL == pwlc)
    {
        m_airing.SetActive(0);
        pwlc = m_airing.GetActiveNetwork();
        CBRA(NULL != pwlc);
    }

    // Hide the list bubble 
    SHNotificationRemove(&CLSID_WiFiBubble, BUBBLE_WIRELESS_LIST);

    // Associate the connection with its destination
    nid.Assign(pwlc);
    hr = nid.Associate(uMeta);
    CHR(hr);

    // Connect
    hr = DoConnect(pwlc);
    CHR(hr);

    // We're not in the selected UI anymore
    m_fInSelectUI = FALSE;
    OnUnblockUI();

Error:
    return hr;
}


HRESULT CWZCManagerWirelessPPC::OnWepKeyInput(LPCTSTR pszKey)
{
    HRESULT hr = S_OK;
    int     nResult;

    // Allow empty WEP keys
    if (TEXT('\0') == pszKey[0] &&
        Ndis802_11Encryption1Enabled == m_wlcKey.Privacy)
    {
        // We don't need to set a password... we'll treat this as an automatic WEP key
        GetEAPDefaults(&m_wlcKey);
        nResult = TRUE;
    }
    else
    {
        // Check validity of the password
        nResult = CWZCManagerWireless::WritePassword(&m_wlcKey, 
                                                     pszKey, 
                                                     m_wlcKey.Privacy, 
                                                     m_wlcKey.AuthenticationMode);
    }

    if (nResult <= 0)
    {
        // Refresh the bubble with the new contents
        hr = ShowPasswordDialog((m_nKeyState & PWDUI_WPA) | (nResult == -1 ? PWDUI_ERR_FORMAT : PWDUI_ERR_GENERIC));
        CHR(hr);
        goto Exit;
    }

    // Hide the key bubble
    SHNotificationRemove(&CLSID_WiFiPasswordBubble, BUBBLE_WIRELESS_KEY);
    // User already entered Network Key and ready to connect , start off Inactivity Timer
    StartWifiTimeoutTimer();
    // Connect to the network
    hr = BeginConnect(&m_wlcKey);
    CHR(hr);

    // Unblock new UI
    m_nKeyState = -1;
    OnUnblockUI();

Exit:
Error:
    return hr;
}

LRESULT CWZCManagerWirelessPPC::OnNotify(int idCtrl, NMSHN *pnmshn)
{
    LRESULT lr = 0;

    if (SHNN_LINKSEL == pnmshn->hdr.code &&
        NULL != pnmshn->pszLink)
    {
        // The user selected a network from the list
        if (!_tcsncmp(pnmshn->pszLink, c_szCommandSelect, ARRAYSIZE(c_szCommandSelect) - 1))
        {
            // Get the selected network
            LPTSTR pszIndex = GetHTMLInputValue(pnmshn->pszLink, TEXT("SSID"));
            if (NULL != pszIndex)
            {
                OnMultiSelect(_ttoi(pszIndex));
                LocalFree(pszIndex);
            }
        }

        // The user pressed connect after choosing Internet or Work
        else if (!_tcsncmp(pnmshn->pszLink, c_szCommandConnection, ARRAYSIZE(c_szCommandConnection) - 1))
        {
            // Get the selected metanetwork
            LPTSTR pszIndex = GetHTMLInputValue(pnmshn->pszLink, c_szInputCtrlNameMetaNetwork);
            if (NULL != pszIndex)
            {
                OnConnectButton(_ttoi(pszIndex));
                LocalFree(pszIndex);
            }
        }

        // The user entered the network key
        else if (!_tcsncmp(pnmshn->pszLink, c_szCommandWepKeyInput, ARRAYSIZE(c_szCommandWepKeyInput) - 1))
        {
            // Get the key text
            LPTSTR pszKey = GetHTMLInputValue(pnmshn->pszLink, c_szInputCtrlNameWepKey);
            if (NULL != pszKey)
            {
                OnWepKeyInput(pszKey);
                LocalFree(pszKey);
            }
        }
    }

    return lr;
}
    
void CWZCManagerWirelessPPC::OnCommand(WORD wNotifyCode, WORD wID, HWND hwndCtl)
{
    switch (wID)
    {
    case COMMAND_SETTING:
        // Go to WiFi setting control panel page
        PostShellStatusMessage(WM_TASKBAR_LAUNCHWIFISETTINGS, 0, 0);
        // Fall through

    case IDM_WZC_NEWNET_DISMISS:
        HideBubble();
        DismissAiringList();
        break;

    case IDM_WZC_NEWNET_STOP_NOTIFICATIONS:
        SHWZCSetNotificationState(FALSE);
        HideBubble();
        DismissAiringList();
        break;
    }
    
}

LRESULT CWZCManagerWiredPPC::OnNotify(int idCtrl, NMSHN *pnmshn)
{
    LRESULT lr = 0;
    LPCTSTR pszText;

    if (SHNN_LINKSEL == pnmshn->hdr.code &&
        NULL != pnmshn->pszLink &&
        !_tcsncmp(pnmshn->pszLink, c_szCommandWiredConnection, ARRAYSIZE(c_szCommandWiredConnection)-1))
    {
        // Get the selected meta network
        LPCTSTR pszIndex = GetHTMLInputValue(pnmshn->pszLink, c_szInputCtrlNameMetaNetwork);
        if (NULL != pszIndex)
        {
            // Save the wired association
            UINT iMeta = _ttoi(pszIndex);
            SetNetworkDestGUID(NULL, FALSE, GetMetaNetworkGUIDString(iMeta));

            // Load the wired network description
            pszText = (LPCTSTR)LoadString(HINST_RESDLL, IDS_NIC_DESCRIPTION, NULL, 0);
            ASSERT(NULL != pszText);

            // Tell Connection Manager
            CM_SetCurrentNetwork(m_pszDeviceName, pszText, iMeta);
        }

        // Hide the bubble
        HideConnectionBubble();
    }

    return lr;
}

void CWZCManagerWiredPPC::OnCommand(WORD wNotifyCode, WORD wID, HWND hwndCtl)
{
    switch (wID)
    {
    case COMMAND_SETTING:
        // Go to Connections Settings page
        PostShellStatusMessage(WM_TASKBAR_LAUNCHCONNSETTINGS, 0, 0);
        break;
    }
}

HRESULT CWZCManagerWiredPPC::ShowConnectionBubble()
{
    HRESULT            hr = S_OK;
    SHNOTIFICATIONDATA shnd = { 0 };
    LPTSTR             pszHTML = NULL;

    // Build the HTML
    hr = BuildSingleNetworkHTML(NULL, &pszHTML);
    CHR(hr);

    // Set up the softkey data
    shnd.rgskn[0].pszTitle = (LPCTSTR)_LoadString(IDS_WZC_CONNECTION_BUBBLE_CONNECT_BUTTON);
    shnd.rgskn[0].skc.wpCmd = IDOK;
    shnd.rgskn[0].skc.grfFlags = NOTIF_SOFTKEY_FLAGS_SUBMIT_FORM;

    // Set up the notification data
    shnd.cbStruct   = sizeof(SHNOTIFICATIONDATA);
    shnd.dwID       = BUBBLE_WIRED;
    shnd.clsid      = CLSID_SHNAPI_ConnectionBegin;
    shnd.npPriority = SHNP_INFORM;
    shnd.csDuration = BUBBLE_WINDOW_TIMEOUT;
    shnd.hicon      = DPI_LoadIconOver(HINST_RESDLL, IDI_WZC_CONNECTION_BUBBLE, IDI_OVERRIDE_WZC_CONNECTION_BUBBLE);
    shnd.pszTitle   = (LPCTSTR)_LoadString(IDS_NIC_INSERTION_TITLE);
    shnd.hwndSink   = m_hwndSink;
    shnd.pszHTML    = pszHTML;

    // Display the bubble
    CBR(ERROR_SUCCESS == SHNotificationAdd(&shnd));

Error:
    if (E_PENDING == hr)
    {
        // This means we couldn't get the metanetworks, which can happen if
        // defaultconfig.xml hasn't been loaded yet. Set a timer to try again.
        VBR(SetTimer(m_hwndSink, c_tidConnectionBubbleRetry, c_csConnectionBubbleRetry * 1000, 0));
        hr = S_FALSE;
    }

    if (NULL != shnd.hicon)
    {
        DestroyIcon(shnd.hicon);
    }

    LocalFree(pszHTML);
    return hr;
}

void CWZCManagerWiredPPC::HideConnectionBubble()
{
    // Hide the bubble
    SHNotificationRemove(&CLSID_SHNAPI_ConnectionBegin, BUBBLE_WIRED);
}
