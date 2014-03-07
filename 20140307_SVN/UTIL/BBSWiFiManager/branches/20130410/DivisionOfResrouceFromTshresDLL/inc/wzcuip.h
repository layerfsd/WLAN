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
// Filename:    wzcuip.h
//
// Owner:        
//
// Purpose:    Header file for wireless zero config 
//
//==========================================================================*
#pragma once

#include <eapol.h>
#include <wzcsapi.h>
#include <commctrl.h>
#include <wzcshell.h>
#include "iprtrmib.h"


#ifdef __cplusplus
extern "C" {
#endif

#define WM_DELAY_CONNECTION           (WM_USER + 100)
#define WM_WZCUI_REFRESH              (WM_USER + 101)
#define WM_DELAY_WEPKEY_INPUT         (WM_USER + 102)
#define WM_DELAY_SET_REFRESH_TIMER    (WM_USER + 103)
#define WM_WARNING_NO_WIRELESS_CARD   (WM_USER + 104)
#define WM_WZC_STATUS_UPDATE          (WM_USER + 106)
#define WM_UI_CLOSE                   (WM_USER + 107)
#define WM_DELAY_SELECT               (WM_USER + 108)


// Invalidate characters
static const TCHAR c_szSSIDInvalidCharList [] = TEXT("\\\"");

#define MAX_SSID_IN_BUBBLE_WINDOW 4  // the maximum number of networks in the bubble

// flags used to select state & item images
#define WZCIMG_PREFR_NOSEL     0    // empty check box
#define WZCIMG_PREFR_SELECT    1    // checked check box
#define WZCIMG_INFRA_SILENT    2    // infra icon + red cross
#define WZCIMG_ADHOC_AIRING    3    // adhoc icon
#define WZCIMG_ADHOC_ACTIVE    4    // adhoc icon + blue circle
#define WZCIMG_ADHOC_SILENT    5    // adhoc icon + red cross
#define WZCIMG_SIGNAL_1        6
#define WZCIMG_SIGNAL_2        7
#define WZCIMG_SIGNAL_3        8
#define WZCIMG_SIGNAL_4        9
#define WZCIMG_SIGNAL_SECURE_1 10
#define WZCIMG_SIGNAL_SECURE_2 11
#define WZCIMG_SIGNAL_SECURE_3 12
#define WZCIMG_SIGNAL_SECURE_4 13

typedef enum {
    WZC_FILTER_ALL_AVAILABLE = 0,
    WZC_FILTER_ACCESS_POINT  = 1,
    WZC_FILTER_ADHOC         = 2,
} WZC_FILTER;

typedef enum
{
    WZC_NS_ACTIVE     = 1,
    WZC_NS_AIRING     = 2,
    WZC_NS_PREFERRED  = 4,
    WZC_NS_CONNECTING = 8,
} WZC_NETWORK_STATE;

typedef struct tagSWZCNET_INFO
{
    WZC_WLAN_CONFIG   sConfig;
    INT               nMetaNetwork;
    WZC_NETWORK_STATE nState;
} SWZCNET_INFO, *PSWZCNET_INFO;

////////////////////////////////////////////////////////////////////////
// WIFI update state info structure
// 
typedef struct _WUSI
{
    HWND   hwndNotify;
    HANDLE hEventStop;
} WUSI, *PWUSI;


typedef struct _WLAN_INFO 
{
    HSHWZC           hshwzc;
    HDSA             hdsaNetworks;
    DWORD            fAPAir :1;

    HWND             hDlg;
    HWND             hwndNCPrsht;
    HWND             hwndNetList;
    HWND             hwndNetListMB;
    HANDLE           hSoftKeyExt;
    HANDLE           hContextExt;
    LPVOID           pNetworkObject; // pointer to a CNetworkObject
    HIMAGELIST       hImgs;
    WZC_FILTER       nNetFilter;
    
    BOOL             fWirelessCardPresent;

    BOOL             bActivate;
    BOOL             bConnecting;
    BOOL             bUserInitiatedSearch;
    DWORD            dwHWFlags;
    HCURSOR          hOldCursor;
} WLAN_INFO, *PWLAN_INFO;


#define WZC_STATUS_UPDATE_TIME  500

////////////////////////////////////////////////////////////////////////
// WLAN_CFG_PROPS related stuff
//
// flags used for WLAN_CFG_PROPS dwFlags
#define WZCDLG_PROPS_RWSSID       0x00000001    // enable SSID for writing
#define WZCDLG_PROPS_RWINFR       0x00000002    // enable Infrastructure Mode for writing
#define WZCDLG_PROPS_KEYTOUCHED   0x80000000    // key was changed

// utility macro to convert a hexa digit into its value
#define HEX(c)  ((c)<='9'?(c)-'0':(c)<='F'?(c)-'A'+0xA:(c)-'a'+0xA)

// Ubiquitous define for MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// defines used for the valid WEP Key indices
#define WZC_WEPKIDX_MIN     0
#define WZC_WEPKIDX_MAX     3

typedef struct _WLAN_CFG_PROPS 
{
    PWLAN_INFO psWLanInfo;
    DWORD      dwFlags;
    INT        nReturn;  // return code

    SHWZCNET   wzcnet;
    UINT       uMeta;
    TCHAR      pszKey[65];
    LPVOID     pvEapData;
    UINT       cbEapData;
    DWORD      dwMask;

    // 802.1x state
    DWORD               dwNumEapExt;  // number of EAP extensions in system
    PEAP_EXTENSION_INFO pEapExtInfo;  // Pointer to array describing available EAP extensions
} WLAN_CFG_PROPS, *PWLAN_CFG_PROPS;

extern HANDLE g_hNdisUio;
extern WLAN_INFO g_sWLanInfo;

// Macros
#define TMMS_WIRELESS_RESCAN_TF TMMS_INFINITE


////////////////////////////////////////////////////////////////////////
// function declarations

LPTSTR           StrDup(LPCTSTR pszIn);

BOOL             IsHexString(__in LPCSTR pcszString);

HRESULT          WZCCreateNetworkWLanPropsDlgs(HWND hwndOwner, PWLAN_CFG_PROPS psProps);

HRESULT          RefreshConnectionBubble(HWND hwndSink);
HRESULT          DisplayWiredConnectionBubble(HWND hwndSink);
void             CloseWiredConnectionBubble(void);

BOOL             WZCMatchConfigWithSsid(PWZC_WLAN_CONFIG pCfg1, LPCSTR pucSSID);
INT              WZCMatchConfig(__in const WZC_WLAN_CONFIG *pCfg1, __in const WZC_WLAN_CONFIG *pCfg2,
                                BOOL fMatchSSIDOnly);
HRESULT          WZCCopyConfigs(__notnull PWZC_WLAN_CONFIG pCfgDests,
                                __notnull const WZC_WLAN_CONFIG *pCfgSrcs,
                                UINT cuConfigs, BOOL fDeepCopy);
void             WZCFreeConfig(PWZC_WLAN_CONFIG pwzcConfig);
HRESULT          WZCInsertPreferredConfig(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, INT nIndex, 
                    PWZC_WLAN_CONFIG pwzcConfig);
INT              FindTopPosInPreferredList(PINTF_ENTRY_EX pIntfEntry,  DWORD dwOIDFlags, 
                    PWZC_WLAN_CONFIG pwzcConfig);
HRESULT          WZCRemoveNonBroadcastingConfigs(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, 
                    BOOL fFreeConfig);
HRESULT          WZCRemovePreferredConfig(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, 
                    INT nIndex, BOOL fFreeConfig);
BOOL             WZCIsPreferredNetworkAiring(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, 
                    NDIS_802_11_NETWORK_INFRASTRUCTURE nMode);
BOOL             WZCGetConnectedConfig(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, 
                    PWZC_WLAN_CONFIG psConfig);
HRESULT          WZCUpdatePreferredConfig(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, 
                    PWZC_WLAN_CONFIG psConfig, INT nNewPosition, BOOL fFreeConfig); 
HRESULT          WZCDupRAW_DATA(const RAW_DATA *pRawData, PRAW_DATA pDupRawData);
PWZC_WLAN_CONFIG WZCGetAiringConfigs(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags);
UINT             WZCGetNumAiringConfigs(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags);
BOOL             WZCIsPreferredConfigsEmpty(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags);
PWZC_WLAN_CONFIG WZCGetPreferredConfigs(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags);
UINT             WZCGetNumPreferredConfigs(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags);
BOOL             WZCIsConfigInList(__in const WZC_WLAN_CONFIG *pConfig,
                                   __in const WZC_WLAN_CONFIG *pConfigList,
                                   INT sizeConfigList, BOOL fMatchSSIDOnly);
INT              WZCFindConfigInList(__in const WZC_WLAN_CONFIG *pConfig,
                                     __in const WZC_WLAN_CONFIG *pConfigList,
                                     INT sizeConfigList, BOOL fMatchSSIDOnly);
BOOL             IsAvaibleWirelessNetChanged(LPCTSTR pcszAdapter,
                    PWZC_WLAN_CONFIG pConfigures, UINT cuConfigures);
HRESULT          WZCConnect(PINTF_ENTRY_EX pIntfEntry, PDWORD pdwOIDFlags, 
                    PWZC_WLAN_CONFIG psConfig);
INT              CheckWepKMaterial(PWZC_WLAN_CONFIG pwzcConfig, LPCTSTR pszWepKey);
void             CopyWepKMaterial(PWZC_WLAN_CONFIG pwzcConfig,LPSTR szKMat);
HRESULT          WZCQueryWirelessInfo(__notnull PINTF_ENTRY_EX pIntfEntry, DWORD dwInOIDFlags, 
                    __notnull PDWORD pdwOutOIDFlags);

DWORD WINAPI     WZCUpdateStatusThread(LPVOID pv);

void             InitializeWZCQCFG(void);
void             DeleteWZCQCFG(void);

BOOL             WZCWLanNetworkDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);
BOOL CALLBACK    NotifyDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT          NICInsertedProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HRESULT SetCallbackRegistration(BOOL fRegistration, PWLAN_INFO psInfo);

// Generic helper functions
BOOL IsAnyWPA(NDIS_802_11_AUTHENTICATION_MODE nam);
BOOL IsAnyWPAPSK(NDIS_802_11_AUTHENTICATION_MODE nam);
BOOL IsAnyWPA2(NDIS_802_11_AUTHENTICATION_MODE nam);
BOOL IsWPAAESCapable(LPBYTE pParam);

// Dialog helper functions
HRESULT          DDX_Text(HWND hDlg, INT nEditID,  LPTSTR pszString, INT nMaxLenBuf, 
                    BOOL fSetControl);
BOOL             DDX_Check(HWND hDlg, INT nCheckID, BOOL*  pfCheck,   BOOL fSetControl);
BOOL             DDX_Comb(HWND hDlg, INT nIDComb, INT* pnIndex, BOOL fSetControl);

INT              MessageBoxWithResID(HWND hwnd, INT nIDText, INT nIDCaption, UINT uFlags);
BOOL             PropPageDialogHelper(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT          ReAllocAndConcat(LPTSTR *ppszDest, LPCTSTR pszAppend);

LPTSTR           GetHTMLInputValue(LPCTSTR pszLink, LPCTSTR pszInputName);

UINT             GetMetaNetworkCount();
HRESULT          InitMetaNetworkCombo(HWND hwndCombo, UINT iSel);
LPCTSTR          GetMetaNetworkGUIDString(UINT uIndex);
HRESULT          GetMetaNetworkIndex(__notnull LPCTSTR pszGUID, __notnull UINT *piMetaNetwork);
HRESULT          GetMetaNetworkName(UINT uIndex, __notnull LPCTSTR *ppszName);


typedef HRESULT (*PFNENUMNAMECB)(UINT iIndex, __notnull LPCTSTR pszName, LPVOID pvParam);
HRESULT EnumMetaNetworkNames(PFNENUMNAMECB pfnCallback, LPVOID pvParam);

// extracted the function "IsNetConnected" definition from private\shellw\cplmain\cmpubapi.h
BOOL    IsNetConnected(PMIB_IPADDRROW pIpAddrRow, BOOL * pbIsCradled, BOOL * pbDoingDHCP, BOOL *pfIsWiFi);

BOOL            WZCWLanAdvancedDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);

#ifdef __cplusplus
}  // extern "C"
#endif
