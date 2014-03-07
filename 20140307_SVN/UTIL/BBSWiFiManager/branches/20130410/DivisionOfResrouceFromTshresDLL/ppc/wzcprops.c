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
// Filename:    WZCProps.cpp
//
// Owner:        
//
// Purpose:     Wireless network properties dialog
//
//==========================================================================*
//==========================================================================*

#include <windows.h>
#include <windowsx.h>
#include <types.h>
#include "netui.h"
#include <netuip.h>
#include "wzcuip.h"
#include "resource.h"
#include "Wpcpriv.h"
#include "ehm.h"
#include <shmenus.h>

#define WM_DELAY_SETFOCUS   (WM_USER + 1)
#define WM_NETUI_INVALIDKEY (WM_USER + 2)

extern CRITICAL_SECTION  g_csuiLock;

static const TCHAR c_szSSIDInvalidChars [] = TEXT("\\/?*<>\"|");

int CALLBACK NetworkWLanSettingCallback(HWND hwndDlg, UINT uMsg, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(hwndDlg);
    UNREFERENCED_PARAMETER(lParam);
    
    switch(uMsg)
    {
    case PSCB_GETVERSION:
        return COMCTL32_VERSION;

    case PSCB_NOPRECREATE:
        return 1;

    default:
        break;
    }
    
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
////
//// Helper functions
////
///////////////////////////////////////////////////////////////////////////////

// GetCurrentAuthMode returns the current Authmode that is selected in the authentication combo box
DWORD GetCurrentAuthMode(HWND hDlg)
{
    HWND hcbAuth = GetDlgItem(hDlg, IDC_WZC_COMBO_AUTHENTICATION);
    return SendMessage(hcbAuth, CB_GETITEMDATA, SendMessage(hcbAuth, CB_GETCURSEL, 0, 0), 0);
}

// GetCurrentEncryptionMode returns the current Authmode that is selected in the encryption combo box
DWORD GetCurrentEncryptionMode(HWND hDlg)
{
    HWND hcbEncryption  = GetDlgItem(hDlg, IDC_WZC_COMBO_DATA_ENCRYPTION);
    return SendMessage(hcbEncryption, CB_GETITEMDATA, SendMessage(hcbEncryption, CB_GETCURSEL, 0, 0), 0);
}

// UpdateAuthenticationMode updates the proppage struct with the current authentication mode
void UpdateAuthenticationMode(HWND hDlg, __out PSHWZCNET pwzcnet)
{
    pwzcnet->dwAuthentication = GetCurrentAuthMode(hDlg);
}

// UpdateEncryptionMode updates the proppage struct with the current encryption
void UpdateEncryptionMode(HWND hDlg, __out PSHWZCNET pwzcnet)
{
    pwzcnet->dwEncryptionType = GetCurrentEncryptionMode(hDlg);
}

static void UpdateAutoKeyMode(HWND hDlg, __out PSHWZCNET pwzcnet)
{
    BOOL bCheck = FALSE;

    DDX_Check(hDlg, IDC_WZC_CHECK_KEY_PROVIDED_AUTOMATICALLY, &bCheck, FALSE);
    pwzcnet->dwFlags &= ~SHWZCF_NET_KEYDATA;
    pwzcnet->dwFlags |= (bCheck ? 0 : SHWZCF_NET_KEYDATA);
}

// FindIndexFromAuthMode returns the index of the entry in the authmode combobox with the given authentication mode
int FindIndexFromComboData(HWND hcb, DWORD dwValue)
{
    UINT cItem = SendMessage(hcb, CB_GETCOUNT, 0, 0);
    UINT iItem;
    
    for (iItem = 0; iItem < cItem; ++iItem)
    {
        DWORD dwCur;

        dwCur = SendMessage(hcb, CB_GETITEMDATA, iItem, 0);

        if (dwCur == dwValue)
            return iItem;
    }

    // not found
    return -1;
}

static void DisplayError(HWND hDlg, UINT idsErrorString, UINT idFocus)
{
    MessageBoxWithResID(hDlg, idsErrorString, IDS_WZC_MSG_TITLE, MB_OK | MB_ICONSTOP);
    SetFocus(GetDlgItem(hDlg, idFocus));
}


///////////////////////////////////////////////////////////////////////////////
////
//// Wireless network property general page dialog
////
///////////////////////////////////////////////////////////////////////////////

static void UpdateDataGeneral(__in HWND hDlg, PSHWZCNET pwzcnet, INT *pnMeta, BOOL fSetControl)
{
   BOOL    bCheck = FALSE;

   DDX_Comb(hDlg, IDC_WZC_COMBO_CONNECT_TO, pnMeta, fSetControl);

   if (fSetControl)
   {
      // Set SSID
      DDX_Text(hDlg, IDC_WZC_EDIT_SSID, pwzcnet->szName, 0, TRUE);

      // Check ADHOC network
      bCheck = BOOLIFY(pwzcnet->dwFlags & SHWZCF_NET_ADHOC);
      DDX_Check(hDlg, IDC_WZC_CHECK_ADHOC, &bCheck, TRUE);

      // Hidden network checkbox
      bCheck = BOOLIFY(pwzcnet->dwFlags & SHWZCF_NET_REQUIRE_WZC_PROBE);
      DDX_Check(hDlg, IDC_WZC_CHECK_HIDDEN, &bCheck, TRUE);
      
   }
   else 
   {
      DDX_Text (hDlg, IDC_WZC_EDIT_SSID, pwzcnet->szName, ARRAYSIZE(pwzcnet->szName), FALSE);

      DDX_Check(hDlg, IDC_WZC_CHECK_ADHOC, &bCheck, FALSE);
      pwzcnet->dwFlags = (pwzcnet->dwFlags & ~SHWZCF_NET_ADHOC) | (bCheck ? SHWZCF_NET_ADHOC : 0);

      DDX_Check(hDlg, IDC_WZC_CHECK_HIDDEN, &bCheck, FALSE);
      pwzcnet->dwFlags = (pwzcnet->dwFlags & ~SHWZCF_NET_REQUIRE_WZC_PROBE) | (bCheck ? SHWZCF_NET_REQUIRE_WZC_PROBE : 0);
   }
}

void EnableGeneralControls(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    EnableWindow(GetDlgItem(hDlg, IDC_WZC_EDIT_SSID),        BOOLIFY(psProp->dwFlags & WZCDLG_PROPS_RWSSID));
    EnableWindow(GetDlgItem(hDlg, IDC_WZC_COMBO_CONNECT_TO), TRUE);
    
    // Disable the ad-hoc checkbox if this network was already marked as hidden
    EnableWindow(GetDlgItem(hDlg, IDC_WZC_CHECK_ADHOC),      BOOLIFY((psProp->dwFlags & WZCDLG_PROPS_RWINFR) &&
                                                                     !(psProp->wzcnet.dwFlags & SHWZCF_NET_REQUIRE_WZC_PROBE)));
    // Disable the hidden checkbox if this network was already marked ad-hoc
    EnableWindow(GetDlgItem(hDlg, IDC_WZC_CHECK_HIDDEN),     BOOLIFY(!(psProp->wzcnet.dwFlags & SHWZCF_NET_ADHOC)));
}


BOOL OnOKGeneral(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    HRESULT  hr = S_OK;
    SHWZCNET wzcnet;
    INT      nMeta;
    DWORD    dwMask;

    // Get the UI settings
    memcpy(&wzcnet, &psProp->wzcnet, sizeof(SHWZCNET));
    UpdateDataGeneral(hDlg, &wzcnet, &nMeta, FALSE);

    if (psProp->dwFlags & WZCDLG_PROPS_RWSSID) // SSID is changable
    {
        // Is the SSID empty?
        if (TEXT('\0') == wzcnet.szName[0])
        {
            MessageBoxWithResID(hDlg, IDS_WZC_NULL_SSID, IDS_WZC_MSG_TITLE, MB_ICONSTOP | MB_OK);
            CBR(FALSE);
        }

        // Is the name valid?
        hr = SHWZCValidateNetworkName(wzcnet.szName, NULL, 0);
        if (FAILED(hr))
        {
            MessageBoxWithResID(hDlg, IDS_WZC_INVALID_NAME, IDS_WZC_MSG_TITLE, MB_ICONSTOP | MB_OK);
            CHR(hr);
        }

        // Make sure that we do have a valid handle to shwzc
        // If the handle is null, most likely the parent window received an OnActivateHome.
        // We don't ever destroy the callback because the parent window will destroy it.
        hr = SetCallbackRegistration(TRUE, psProp->psWLanInfo);
        CHR(hr);
        // Is this a duplicate name?
        if (S_FALSE != SHWZCIsDuplicateName(psProp->psWLanInfo->hshwzc,
            wzcnet.szName, BOOLIFY(wzcnet.dwFlags & SHWZCF_NET_ADHOC)))
        {
            MessageBoxWithResID(hDlg, IDS_WZC_DUP_SSID, IDS_WZC_MSG_TITLE, MB_ICONSTOP | MB_OK);
            CBR(FALSE);
        }

        // Copy the name
        CHRA(StringCchCopy(psProp->wzcnet.szName, ARRAYSIZE(psProp->wzcnet.szName), wzcnet.szName));
    }

    // Apply the changes
    dwMask = SHWZCF_SETMASK_ADHOC | SHWZCF_SETMASK_REQUIRE_WZC_PROBE;
    hr = SHWZCCopySettings(&psProp->wzcnet, &wzcnet, dwMask);
    CHR(hr);

    psProp->uMeta = nMeta;
    psProp->dwMask |= dwMask;

Error:
    if (FAILED(hr))
    {
        PostMessage(hDlg, WM_DELAY_SETFOCUS, IDC_WZC_EDIT_SSID, 0L);
    }
    return SUCCEEDED(hr);
}

BOOL OnInitDialogGeneral(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    SHINITDLGINFO shidi = {0,};
	SHMENUBARINFO mbi = {0,};
	
	// Adding Scroll Bar support
    shidi.dwMask    = SHIDIM_FLAGS;
    shidi.hDlg      = hDlg;
    shidi.dwFlags   = SHIDIF_WANTSCROLLBAR | SHIDIF_SIZEDLG;

    SHInitDialog(&shidi);
	
	
	//Menubar
	mbi.cbSize = sizeof( SHMENUBARINFO );
	mbi.hwndParent = hDlg;
	mbi.dwFlags = SHCMBF_HMENU;
	mbi.nToolBarId = IDM_WIZARD_FIRST;
	mbi.hInstRes = HINST_RESDLL;
	SHCreateMenuBar( &mbi );

    //SHDoneButton(hDlg, SHDB_HIDE);

    EnableGeneralControls(hDlg, psProp);

    SendDlgItemMessage(hDlg, IDC_WZC_EDIT_SSID, EM_LIMITTEXT, MAX_SSID_LEN, 0);

    InitMetaNetworkCombo(GetDlgItem(hDlg, IDC_WZC_COMBO_CONNECT_TO), 0);
        
    UpdateDataGeneral(hDlg, &psProp->wzcnet, (INT*)&psProp->uMeta, TRUE);

    // check to see if this is a new network with the keydata bit set.  we want to activate the keytouched bit so 
    // the ui will process and check for the netkey when the "next" sk is pressed on the security page.
    if((psProp->wzcnet.szName[0] == TEXT('\0')) && (psProp->wzcnet.dwFlags & SHWZCF_NET_KEYDATA))
    {
        // this will force a password check in case the user doesn't input anything
        psProp->dwFlags |= WZCDLG_PROPS_KEYTOUCHED;                
    }

    return TRUE;
}

static HRESULT OnInvalidKey(HWND hdlg)
{
    HRESULT hr = S_OK;
    HWND    hwndPropSheet = GetParent(hdlg);
    HWND    hwndKeyPage;

    // Change the page to the network key page
    CBRA(SendMessage(hwndPropSheet, PSM_SETCURSEL, 1, (LPARAM)NULL));
    hwndKeyPage = (HWND)SendMessage(hwndPropSheet, PSM_GETCURRENTPAGEHWND, 0, 0);
    CBRA(NULL != hwndKeyPage && hwndKeyPage != hdlg);

    // Display the key error
    DisplayError(hwndKeyPage, IDS_WZC_INCORRECT_WEP_KEY, IDC_WZC_EDIT_WEP);
    
Error:
    return hr;
}

BOOL NetworkWLanGeneralPropsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    PWLAN_CFG_PROPS psProps = (PWLAN_CFG_PROPS)GetWindowLong(hDlg, GWL_USERDATA);
    BOOL            fRet = 0;

    fRet = SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(IDD_WZC_WLAN_GENERAL_PROP_PAGE), hDlg, message, wParam, lParam );

    switch(message)
    {
    case WM_INITDIALOG:
        psProps = (PWLAN_CFG_PROPS)lParam;
        SetWindowLong(hDlg, GWL_USERDATA, (LPARAM)psProps);
        
        fRet = OnInitDialogGeneral(hDlg, psProps);
        break;

    case WM_SETTINGCHANGE:
        SHHandleSipChange(hDlg, wParam, lParam, NULL);
        break;

    case WM_WINDOWPOSCHANGED:
    case WM_SIZE:
        SetDialogAutoScrollBarEx(hDlg);
        break;

    case WM_DESTROY:
        break;

    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case ID_CANCEL:
                EndDialog(hDlg, 0);
                break;
                
            case ID_NEXT:
                if (OnOKGeneral(hDlg, psProps))
                {
                    EndDialog(hDlg, IDD_WZC_WLAN_AUTHENTICATION_PAGE);
                }
                break;
                
            case IDC_WZC_CHECK_ADHOC:
            case IDC_WZC_CHECK_HIDDEN:
                {
                    // These are mutually exclusive checkboxes - a network cannot be both hidden and ad-hoc.
                    UINT uID = LOWORD(wParam);
                    UINT uID2 = (uID == IDC_WZC_CHECK_ADHOC) ? IDC_WZC_CHECK_HIDDEN : IDC_WZC_CHECK_ADHOC;

                    // See if we're allowed to change checkbox state
                    BOOL bRWAllowed = (uID2 == IDC_WZC_CHECK_ADHOC) ? BOOLIFY(psProps->dwFlags & WZCDLG_PROPS_RWINFR) : TRUE;

                    if (bRWAllowed)
                    {
                        if (SendDlgItemMessage(hDlg, uID, BM_GETCHECK, 0, 0))
                        {
                            // User checked one of the checkboxes (hidden or ad-hoc) - uncheck and disable 
                            // the other checkbox
                            SendDlgItemMessage(hDlg, uID2, BM_SETCHECK, BST_UNCHECKED, 0);
                            EnableWindow(GetDlgItem(hDlg, uID2), FALSE);
                        }
                        else
                        {
                            // User unchecked a checkbox - reenable the other checkbox
                            EnableWindow(GetDlgItem(hDlg, uID2), TRUE);
                        }
                    }
                    
                    break;
                }
            }            
        }
        break;
            

    case WM_DELAY_SETFOCUS:
        SetFocus(GetDlgItem(hDlg, wParam));
        break;

    case WM_NETUI_INVALIDKEY:
        OnInvalidKey(hDlg);
        break;

    case WM_ACTIVATE:
        // Pass message back to WiFi CPL main window unless the new/previous window is 
        // the parent window itself, in which case the CPL will handle it's own 
        // activation or deactivation. The CPL (psProps->psWLanInfo->hDlg) is a child
        // of the same parent as us.
        if (psProps && psProps->psWLanInfo)
        {
            HWND hwndPrevious = (HWND)lParam;
            if (hwndPrevious != GetParent(hDlg))
            {
                PostMessage (psProps->psWLanInfo->hDlg, message, wParam, lParam);
            }
        }
        break;
    }

    return fRet;
}


///////////////////////////////////////////////////////////////////////////////
////
//// Wireless network key page and 802.1x page
////
///////////////////////////////////////////////////////////////////////////////
typedef struct _WZC_EAP_CB_ITEMDATA
{
    PEAP_EXTENSION_INFO pEapExtInfo;
    DWORD dwAuthDataLen;
    BYTE  *pbAuthData;

} WZC_EAP_CB_ITEMDATA, *PWZC_EAP_CB_ITEMDATA;

DWORD Init8021xControls(HWND hDlg, WLAN_CFG_PROPS *pWCP)
{
    DWORD dwError;
    BOOL  fEnabled;
    BOOL  bCurSelSet    = FALSE;
    HWND  hwndCbEAPType = GetDlgItem(hDlg, IDC_WZC_COMBO_EAP_TYPE);
    int   i;

    fEnabled = BOOLIFY(SHWZCF_NET_8021X & pWCP->wzcnet.dwFlags);
    SendMessage(GetDlgItem(hDlg, IDC_WZC_CHECK_ENABLE8021X), BM_SETCHECK, 
                fEnabled ? BST_CHECKED : BST_UNCHECKED, 0);

    if (pWCP->pEapExtInfo == NULL) 
    {
        // Load EAP extension info
        dwError = WZCEnumEapExtensions(&pWCP->dwNumEapExt,&pWCP->pEapExtInfo);
        if (dwError != ERROR_SUCCESS) 
        {
            DEBUGMSG(ZONE_ERROR,(TEXT("!WZCUI:Error 0x%X loading EAP extensions\r\n"),dwError));
            return dwError;
        }                  

        if (pWCP->dwNumEapExt > 0 && pWCP->pEapExtInfo)
        {
            PEAP_EXTENSION_INFO pCurExt;           
            PWCHAR              wszDefaultFriendlyName=NULL;
            
            for ( i = 0, pCurExt = pWCP->pEapExtInfo; i < (int)pWCP->dwNumEapExt; i++, pCurExt++) 
            {
                // Add EAP types to combo box
                WZC_EAP_CB_ITEMDATA *pEapCbItemData = (WZC_EAP_CB_ITEMDATA *)LocalAlloc(LPTR,sizeof(WZC_EAP_CB_ITEMDATA));
                int                  iListIndex;
                
                if (pEapCbItemData) 
                {
                    iListIndex = ComboBox_AddString(hwndCbEAPType, pCurExt->wszFriendlyName);
                    pEapCbItemData->pEapExtInfo = pCurExt;
                    ComboBox_SetItemData(hwndCbEAPType, iListIndex,pEapCbItemData);
                    
                    // For currently selected EAP type, store provider specific data
                    if (fEnabled && pWCP->wzcnet.dwEAPType == pCurExt->dwExtensionType)
                    {
                        // Get the current EAP data
                        pEapCbItemData->pbAuthData = (LPBYTE)pWCP->pvEapData;
                        pEapCbItemData->dwAuthDataLen = pWCP->cbEapData;
                        pWCP->pvEapData = NULL;  // so we don't free it twice

                        // Select this entry in the list
                        ComboBox_SetCurSel(hwndCbEAPType, iListIndex);
                        bCurSelSet = TRUE;
                    } 
                    else if (pCurExt->dwExtensionType == DEFAULT_EAP_TYPE) 
                    {
                        wszDefaultFriendlyName = pCurExt->wszFriendlyName;
                    }
                }
            }
            // No matching providers, set to default
            if (pWCP->dwNumEapExt && !bCurSelSet) 
            {
                UINT uResult=0;
                DEBUGMSG(ZONE_NETWORK,(TEXT("WZCUI: No matching providers for EAP type %d\r\n"),
                                       pWCP->wzcnet.dwEAPType));
                if (wszDefaultFriendlyName) 
                {
                    uResult = ComboBox_FindString(hwndCbEAPType, -1, wszDefaultFriendlyName);
                    if (uResult == CB_ERR)
                        uResult = 0;
                }
                ComboBox_SetCurSel(hwndCbEAPType, uResult);
            }
        }
    }
    
    return ERROR_SUCCESS;
}

void Cleanup8021xControls(HWND hDlg, WLAN_CFG_PROPS *pWCP)
{
    PWZC_EAP_CB_ITEMDATA pEapCbItemData;
    HWND                 hwndCbEAPType = GetDlgItem(hDlg, IDC_WZC_COMBO_EAP_TYPE);
    int                  i, cnTypes;
    
    cnTypes = ComboBox_GetCount(hwndCbEAPType);
    for ( i = 0; i < cnTypes; i++) 
    {
        pEapCbItemData = (WZC_EAP_CB_ITEMDATA *)ComboBox_GetItemData(hwndCbEAPType, i);
        if (pEapCbItemData && ((int)pEapCbItemData != CB_ERR))
        {
            if (pEapCbItemData->pbAuthData) 
            {
                LocalFree(pEapCbItemData->pbAuthData);
            }
            LocalFree(pEapCbItemData);
        }
    }
    
    if (pWCP->pEapExtInfo) 
    {
        LocalFree(pWCP->pEapExtInfo);
        pWCP->pEapExtInfo = NULL;
    }
}


void Enable8021xControls(HWND hDlg, WLAN_CFG_PROPS *pWCP)
{
    DWORD dwDisplayFlags = 0;
    BOOL  f8021x = BOOLIFY(SHWZCF_NET_8021X & pWCP->wzcnet.dwFlags);
    BOOL  fEnabled;
    HWND  hwndCbEAPType = GetDlgItem(hDlg, IDC_WZC_COMBO_EAP_TYPE);

    // Get the UI flags for this network
    SHWZCGetDisplayFlags(&pWCP->wzcnet, &dwDisplayFlags);

    if (SHWZCF_UI_8021X_DISABLED & dwDisplayFlags)
    {
        // Disable 802.1x checkbox
        DDX_Check(hDlg, IDC_WZC_CHECK_ENABLE8021X, &f8021x, TRUE);
        EnableWindow(GetDlgItem(hDlg, IDC_WZC_CHECK_ENABLE8021X), FALSE);
    }
    else
    {
        // Enable 802.1x
        EnableWindow(GetDlgItem(hDlg, IDC_WZC_CHECK_ENABLE8021X), TRUE);
    }

    // Is the EAP combo enabled?
    fEnabled = !BOOLIFY(SHWZCF_UI_EAPTYPE_DISABLED & dwDisplayFlags);
    if (fEnabled)
    {
        // We also disable if 802.1x isn't checked
        DDX_Check(hDlg, IDC_WZC_CHECK_ENABLE8021X, &fEnabled, FALSE);
        EnableWindow(GetDlgItem(hDlg, IDC_WZC_STATIC_ENABLE8021X), TRUE);
    }
    else
    {
        PWZC_EAP_CB_ITEMDATA pEapCbItemData;
        UINT cItems = ComboBox_GetCount(hwndCbEAPType);
        UINT i;

        // The configuration doesn't allow custom EAP types, so set it to the
        // default
        for (i = 0; i != cItems; ++i)
        {
            pEapCbItemData = (PWZC_EAP_CB_ITEMDATA)ComboBox_GetItemData(hwndCbEAPType, i);
            if (DEFAULT_EAP_TYPE == pEapCbItemData->pEapExtInfo->dwExtensionType)
            {
                ComboBox_SetCurSel(hwndCbEAPType, i);
                break;
            }
        }

        // Also disable the EAP type label so the user knows it doesn't apply
        EnableWindow(GetDlgItem(hDlg, IDC_WZC_STATIC_ENABLE8021X), FALSE);
    }

    // Set the control state
    EnableWindow(hwndCbEAPType, fEnabled);

    // Determine whether to enable the properties button
    if (fEnabled)
    {
        PWZC_EAP_CB_ITEMDATA pEapCbItemData=NULL;
        int                  iListIndex = ComboBox_GetCurSel(hwndCbEAPType);
        
        if (iListIndex != CB_ERR) 
        {
            pEapCbItemData = (WZC_EAP_CB_ITEMDATA *)ComboBox_GetItemData(hwndCbEAPType, iListIndex);
        }
        
        fEnabled = (pEapCbItemData && ((int)pEapCbItemData != CB_ERR) && 
                   pEapCbItemData->pEapExtInfo->wszConfigUIPath && 
                   pEapCbItemData->pEapExtInfo->wszConfigUIPath[0]);
    }

    // Set the control state
    EnableWindow(GetDlgItem(hDlg, IDC_WZC_BUTTON_EAP_PROPERTIES), fEnabled);
}

void EnableKeyEntryControls(HWND hDlg, DWORD dwDisplayFlags)
{
    BOOL fDisableAll = (BST_CHECKED == SendDlgItemMessage(hDlg, IDC_WZC_CHECK_KEY_PROVIDED_AUTOMATICALLY, BM_GETCHECK, 0, 0));
    BOOL fEnable;

    // Enable the key edit controls
    fEnable = !(fDisableAll || (SHWZCF_UI_KEYDATA_DISABLED & dwDisplayFlags));
    EnableWindow(GetDlgItem(hDlg, IDC_STATIC_EDIT_WEP), fEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_WZC_EDIT_WEP), fEnable);

    // Enable the key index controls
    fEnable = !(fDisableAll || (SHWZCF_UI_KEYINDEX_DISABLED & dwDisplayFlags));
    EnableWindow(GetDlgItem(hDlg, IDC_WZC_COMBO_WEP_INDEX), fEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_STATIC_WEP_INDEX), fEnable);
}

void EnableWepKControls(HWND hDlg, PCSHWZCNET pwzcnet)
{
    DWORD dwDisplayFlags = 0;

    // Get the UI flags for this network
    SHWZCGetDisplayFlags(pwzcnet, &dwDisplayFlags);

    if (SHWZCF_UI_AUTOKEY_DISABLED & dwDisplayFlags)
    {
        // Is the automatic key set?
        SendDlgItemMessage(hDlg, IDC_WZC_CHECK_KEY_PROVIDED_AUTOMATICALLY, BM_SETCHECK,
            (SHWZCF_UI_AUTOKEY_SET & dwDisplayFlags) ? BST_CHECKED : BST_UNCHECKED, 0);

        // Disable the control
        EnableWindow(GetDlgItem(hDlg, IDC_WZC_CHECK_KEY_PROVIDED_AUTOMATICALLY), FALSE);
    }
    else
    {      
        // Is the automatic key set?
        SendDlgItemMessage(hDlg, IDC_WZC_CHECK_KEY_PROVIDED_AUTOMATICALLY, BM_SETCHECK,
            (SHWZCF_NET_KEYDATA & pwzcnet->dwFlags) ? BST_UNCHECKED : BST_CHECKED, 0);

        // Enable the control
        EnableWindow(GetDlgItem(hDlg, IDC_WZC_CHECK_KEY_PROVIDED_AUTOMATICALLY), TRUE);
    }

    // Enable the key controls
    EnableKeyEntryControls(hDlg, dwDisplayFlags);
}

static BOOL CALLBACK EnumEncryption(DWORD dwType, LPCTSTR pszName, LPVOID pvParam)
{
    HRESULT hr = S_OK;
    HWND    hcbEncryption = (HWND)pvParam;
    int     nIndex;

    // Add the item
    nIndex = SendMessage(hcbEncryption, CB_ADDSTRING, 0, (LPARAM)pszName);
    CBR(CB_ERR != nIndex);

    // Save the type
    CBR(CB_ERR != SendMessage(hcbEncryption, CB_SETITEMDATA, nIndex, dwType));

Error:
    return SUCCEEDED(hr);
}

HRESULT PopulateEncryptionBox(HWND hDlg, PCSHWZCNET pwzcnet)
{
    HRESULT hr = S_OK;
    HWND    hcbEncryption = GetDlgItem(hDlg, IDC_WZC_COMBO_DATA_ENCRYPTION);
    int     nIndex;

    // Clear the encryption combo box
    SendMessage(hcbEncryption, CB_RESETCONTENT, 0, 0);

    // Enumerate the available encryption types
    hr = SHWZCEnumEncryptionTypes(pwzcnet, EnumEncryption, hcbEncryption);
    CHR(hr);
    CBR(S_OK == hr);  // S_FALSE means failure

    nIndex = FindIndexFromComboData(hcbEncryption, pwzcnet->dwEncryptionType);
    CBRA(0 <= nIndex);  // huh??
    
    SendMessage(hcbEncryption, CB_SETCURSEL, nIndex, 0);
    EnableWepKControls(hDlg, pwzcnet);

Error:
    return hr;
}

HRESULT UpdateDataAuthentication(HWND hDlg, PSHWZCNET pwzcnet, BOOL fSetControl)
{
    HRESULT hr = S_OK;
    BOOL    bCheck = FALSE;
    INT     nIndex = 0;

    CBRA(hDlg && pwzcnet);

    if (fSetControl)
    {
        DWORD dwDisplayFlags = 0;
        
        HWND hcbAuth = GetDlgItem(hDlg, IDC_WZC_COMBO_AUTHENTICATION);
        CWRA(hcbAuth);
        nIndex = FindIndexFromComboData(hcbAuth, pwzcnet->dwAuthentication);
        CBRA(0 <= nIndex);  // huh??

        SendMessage(hcbAuth, CB_SETCURSEL, nIndex, 0);

        // Get the display flags
        SHWZCGetDisplayFlags(pwzcnet, &dwDisplayFlags);
        bCheck = BOOLIFY(SHWZCF_UI_AUTOKEY_SET & dwDisplayFlags);      
        DDX_Check(hDlg, IDC_WZC_CHECK_KEY_PROVIDED_AUTOMATICALLY, &bCheck, TRUE);

        nIndex = pwzcnet->dwKeyIndex - 1;
        if (nIndex < 0) nIndex = 0;
        DDX_Comb(hDlg, IDC_WZC_COMBO_WEP_INDEX, &nIndex, TRUE);

        // If the auth mode changed then we should re-set the encryption mode
        PopulateEncryptionBox(hDlg, pwzcnet);
    }
    else 
    {
        UpdateAuthenticationMode(hDlg, pwzcnet);
        UpdateEncryptionMode(hDlg, pwzcnet);
        UpdateAutoKeyMode(hDlg, pwzcnet);

        DDX_Comb(hDlg, IDC_WZC_COMBO_WEP_INDEX, &nIndex, FALSE);
        pwzcnet->dwKeyIndex = nIndex + 1;
        ASSERT(0 <= nIndex && nIndex <= 3);
    }

Error:
    return hr;
}

static BOOL CALLBACK EnumAuthentication(DWORD dwType, LPCTSTR pszName, LPVOID pvParam)
{
    HRESULT hr = S_OK;
    HWND    hcbAuth = (HWND)pvParam;
    int     nIndex;

    // Add the item
    nIndex = SendMessage(hcbAuth, CB_ADDSTRING, 0, (LPARAM)pszName);
    CBR(CB_ERR != nIndex);

    // Set the type
    CBR(CB_ERR != SendMessage(hcbAuth, CB_SETITEMDATA, nIndex, dwType));

Error:
    return SUCCEEDED(hr);
}

HRESULT EnableControlsAuthentication(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    HRESULT hr = S_OK;
    HWND    hcbAuth = GetDlgItem(hDlg, IDC_WZC_COMBO_AUTHENTICATION);

    // Clear the authentication combo box
    SendMessage(hcbAuth, CB_RESETCONTENT, 0, 0);

    // Populate the authentication combo box
    hr = SHWZCEnumAuthenticationTypes(BOOLIFY(SHWZCF_NET_ADHOC & psProp->wzcnet.dwFlags),
        EnumAuthentication, hcbAuth);
    CHR(hr);
    CBR(S_OK == hr);  // S_FALSE is failure

Error:
    return hr;
}

void EnableWepKEditControl(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    
    if(SHWZCF_NET_KEYDATA & psProp->wzcnet.dwFlags)
    {
        LPCTSTR pszKey = NULL;
        DWORD dwFlags;
        if(!(psProp->dwFlags & WZCDLG_PROPS_KEYTOUCHED))
        {
            // Set the key to a default value
            pszKey = TEXT("*************");  // 13 stars
        }
        else
        {
            // Reuse what's stored in the props
            pszKey = psProp->pszKey;
        }

        // SetWindowText marks this as edited, but it might be setting "*************"
        // preserve the flags around the call
        dwFlags = psProp->dwFlags;
        SetWindowText(GetDlgItem(hDlg, IDC_WZC_EDIT_WEP), pszKey);
        psProp->dwFlags = dwFlags;
    }

    SendDlgItemMessage(hDlg, IDC_WZC_EDIT_WEP, EM_LIMITTEXT, 64, 0);

    // show the current key index
    ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_WZC_COMBO_WEP_INDEX),
        psProp->wzcnet.dwKeyIndex - 1);
}

void InitWepKIndexControl(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    HWND  hwndCombo = GetDlgItem(hDlg, IDC_WZC_COMBO_WEP_INDEX);
    TCHAR szIndex[] = TEXT("1");
    INT   i;
    
    for (i = 0; i < 4; i++)
    {
        ComboBox_AddString(hwndCombo, szIndex);
        szIndex[0]++;
    }

    // Select the current 
  
}

BOOL OnInitDialogAuthentication(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    SHINITDLGINFO shidi = {0,};
	SHMENUBARINFO mbi = {0,};
	
	// Adding Scroll Bar support
    shidi.dwMask    = SHIDIM_FLAGS;
    shidi.hDlg      = hDlg;
    shidi.dwFlags   = SHIDIF_WANTSCROLLBAR | SHIDIF_SIZEDLG;

    SHInitDialog(&shidi);
	
	
	//Menubar
	mbi.cbSize = sizeof( SHMENUBARINFO );
	mbi.hwndParent = hDlg;
	mbi.dwFlags = SHCMBF_HMENU;
	mbi.nToolBarId = IDM_WIZARD_MID;
	mbi.hInstRes = HINST_RESDLL;
	SHCreateMenuBar( &mbi );
	
    //SHDoneButton(hDlg, SHDB_HIDE);
    
    InitWepKIndexControl(hDlg, psProp);
    EnableControlsAuthentication(hDlg, psProp);
    UpdateDataAuthentication(hDlg, &psProp->wzcnet, TRUE);

    return FALSE;
}


BOOL OnDestroyAuthentication(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    return TRUE;
}


BOOL OnOKAuthentication(HWND hDlg, PWLAN_CFG_PROPS psProp, BOOL fValidate)
{
    HRESULT  hr = S_OK;
    SHWZCNET wzcnet;
    DWORD    dwDisplayFlags = 0;
    BOOL     fApplyKey;
    BOOL     fVerified = FALSE;
    DWORD    dwMask = SHWZCF_SETMASK_AUTHENTICATION | SHWZCF_SETMASK_ENCRYPTION | SHWZCF_SETMASK_KEYINDEX;

    // Get the UI settings
    memcpy(&wzcnet, &psProp->wzcnet, sizeof(SHWZCNET));
    hr = UpdateDataAuthentication(hDlg, &wzcnet, FALSE);
    CHR(hr);

    // Check for valid authentication and encryption configuration
    if (fValidate && 
        !SHWZCValidateSecurityConfig(wzcnet.dwAuthentication, 
                                     wzcnet.dwEncryptionType,
                                     BOOLIFY(SHWZCF_NET_ADHOC & wzcnet.dwFlags)))
    {
        DisplayError(hDlg, IDS_WZC_INVALID_CONFIG, IDC_WZC_COMBO_AUTHENTICATION);
        CBR(FALSE);
    }

    // Get the display flags
    hr = SHWZCGetDisplayFlags(&wzcnet, &dwDisplayFlags);
    CHR(hr);

    // Only verify the key if the key material is touched
    if (!(SHWZCF_UI_KEYDATA_DISABLED & dwDisplayFlags) &&
        (SHWZCF_NET_KEYDATA & wzcnet.dwFlags) &&
        (WZCDLG_PROPS_KEYTOUCHED & psProp->dwFlags))
    {
        TCHAR szKeyBuf[WZCCTL_MAX_WEPK_MATERIAL * 2 + 1];
        DDX_Text(hDlg, IDC_WZC_EDIT_WEP, szKeyBuf, ARRAYSIZE(szKeyBuf), FALSE);

        if(fValidate)
        {        
            BOOL fDisplayError = FALSE;
            
            if(TEXT('\0') == szKeyBuf[0])
            {
                // only adhoc networks can have an empty string as a key
                if(BOOLIFY(SHWZCF_NET_ADHOC & psProp->wzcnet.dwFlags))
                {
                    // make sure apply key doesn't get set.  This way if the user validates this state, the wifi netkey will not display *'s when 
                    // when editing the network.  
                    wzcnet.dwFlags &= ~SHWZCF_NET_KEYDATA;
                }
                else
                {
                    fDisplayError = TRUE;
                }
            }
            else 
            {
                if (SHWZCValidateKey(szKeyBuf, wzcnet.dwEncryptionType, wzcnet.dwAuthentication))
                {
                    fVerified = TRUE;
                }
                else
                {
                    fDisplayError = TRUE;
                }
            }

            if(fDisplayError)
            {
                DisplayError(hDlg, IDS_WZC_INCORRECT_WEP_KEY, IDC_WZC_EDIT_WEP);
                CBR(FALSE);
            }
        }

        // Copy the key data into the buffer
        hr = StringCchCopy(psProp->pszKey, ARRAYSIZE(psProp->pszKey), szKeyBuf);
        CHRA(hr);  // this should always work
    }

    // Apply the changes to the network
    fApplyKey = (!(SHWZCF_NET_KEYDATA & wzcnet.dwFlags) || (WZCDLG_PROPS_KEYTOUCHED & psProp->dwFlags));
    dwMask |= (fApplyKey ? SHWZCF_SETMASK_KEYDATA : 0);
    hr = SHWZCCopySettings(&psProp->wzcnet, &wzcnet, dwMask);
    CHR(hr);

    // Success
    psProp->dwMask |= dwMask;

    if(fVerified)
    {   
        // at this point we can say the WEP key is untouched since it's been verified
        psProp->dwFlags &= ~WZCDLG_PROPS_KEYTOUCHED;
    }

Error:
    return SUCCEEDED(hr);
}


void OnEAPProps(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    HRESULT hr = S_OK;
    HINSTANCE hinstEAPDll = NULL;
    WZC_EAP_CB_ITEMDATA *pEapCbItemData;
    PEAP_EXTENSION_INFO pCurExt=NULL;
    HWND hwndCbEAPType = GetDlgItem(hDlg, IDC_WZC_COMBO_EAP_TYPE);
    int iListIndex = ComboBox_GetCurSel(hwndCbEAPType);
    DWORD dwError = (DWORD)-1;
    UCHAR *pConnectionDataOut=NULL;
    DWORD dwSizeOfConnectionDataOut=0;

    UNREFERENCED_PARAMETER(psProp);

    pEapCbItemData = (WZC_EAP_CB_ITEMDATA *)ComboBox_GetItemData(hwndCbEAPType,iListIndex);
    if (pEapCbItemData)
    {
        pCurExt = pEapCbItemData->pEapExtInfo;
    }

    if (pCurExt && pCurExt->wszConfigUIPath) 
    {
        RASEAPINVOKECONFIGUI pfnEapConfig;
        RASEAPFREE           pfnEapFreeMem;

        hinstEAPDll = LoadLibrary(pCurExt->wszConfigUIPath);
        CBR(hinstEAPDll != NULL);
        
        pfnEapConfig = (RASEAPINVOKECONFIGUI)GetProcAddress(hinstEAPDll,TEXT("RasEapInvokeConfigUI"));
        pfnEapFreeMem= (RASEAPFREE)GetProcAddress(hinstEAPDll,TEXT("RasEapFreeMemory"));
        CBR((pfnEapConfig != NULL) && (pfnEapFreeMem != NULL));
            
        // Call into config UI
        __try {
            dwError = pfnEapConfig(pCurExt->dwExtensionType,
                                   hDlg,
                                   0, // flags TBD
                                   pEapCbItemData->pbAuthData,
                                   pEapCbItemData->dwAuthDataLen,
                                   &pConnectionDataOut,
                                   &dwSizeOfConnectionDataOut);
        } 
        __except(WatsonReportFault(GetExceptionInformation(), EXCEPTION_EXECUTE_HANDLER)) 
        {
        }

        CBR(dwError == ERROR_SUCCESS);
        
        if (dwSizeOfConnectionDataOut && pConnectionDataOut) 
        {
            
            if (pEapCbItemData->pbAuthData) 
            {
                LocalFree(pEapCbItemData->pbAuthData);
            }

            pEapCbItemData->pbAuthData = LocalAlloc(LPTR, dwSizeOfConnectionDataOut);
            CPR(pEapCbItemData->pbAuthData);

            pEapCbItemData->dwAuthDataLen = dwSizeOfConnectionDataOut;

            __try 
            {
                memcpy(pEapCbItemData->pbAuthData,
                       pConnectionDataOut,dwSizeOfConnectionDataOut);
            } 
            __except(WatsonReportFault(GetExceptionInformation(), EXCEPTION_EXECUTE_HANDLER)) 
            {
                  LocalFree(pEapCbItemData->pbAuthData);
                  pEapCbItemData->pbAuthData    = NULL;
                  pEapCbItemData->dwAuthDataLen = 0;
            }
            
            __try 
            {
                pfnEapFreeMem(pConnectionDataOut);
            } 
            __except(WatsonReportFault(GetExceptionInformation(), EXCEPTION_EXECUTE_HANDLER)) 
            {
                  ;
            }
        }
    }
    else 
    {
        ASSERT(0); // control shouldn't be enabled in this case
    }
    
Error:
    if (hinstEAPDll)
    {
        FreeLibrary(hinstEAPDll);
    }
}

BOOL OnCommandAuthentication(HWND hDlg, PWLAN_CFG_PROPS psProp, WPARAM wParam, LPARAM lParam)
{
    BOOL bRet = FALSE;

    switch(LOWORD(wParam))
    {
    case ID_BACK:
    case ID_NEXT:
        bRet = OnOKAuthentication(hDlg, psProp, LOWORD(wParam) == ID_NEXT);
        if (bRet)
        {
            EndDialog(hDlg, (LOWORD(wParam) == ID_BACK) ? IDD_WZC_WLAN_GENERAL_PROP_PAGE : IDD_WZC_WLAN_8021X_PAGE);
        }
        break;

    case IDC_WZC_CHECK_KEY_PROVIDED_AUTOMATICALLY:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            DWORD dwDisplayFlags = 0;

            UpdateAutoKeyMode(hDlg, &psProp->wzcnet);
            SHWZCGetDisplayFlags(&psProp->wzcnet, &dwDisplayFlags);
            EnableKeyEntryControls(hDlg, dwDisplayFlags);
            psProp->dwFlags |= WZCDLG_PROPS_KEYTOUCHED;
            bRet = TRUE;
        }
        break;

    case IDC_WZC_COMBO_AUTHENTICATION:
        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            SHWZCNET wzcnet;

            // Get the UI settings
            memcpy(&wzcnet, &psProp->wzcnet, sizeof(SHWZCNET));
            UpdateAuthenticationMode(hDlg, &wzcnet);

            VHR(SHWZCCopySettings(&psProp->wzcnet, &wzcnet, SHWZCF_SETMASK_AUTHENTICATION));
            PopulateEncryptionBox(hDlg, &psProp->wzcnet);
            bRet = TRUE;
        }
        break;
        
    case IDC_WZC_COMBO_DATA_ENCRYPTION:
        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            SHWZCNET wzcnet;

            // Get the UI settings
            memcpy(&wzcnet, &psProp->wzcnet, sizeof(SHWZCNET));
            UpdateEncryptionMode(hDlg, &wzcnet);

            VHR(SHWZCCopySettings(&psProp->wzcnet, &wzcnet, SHWZCF_SETMASK_ENCRYPTION));
            EnableWepKControls(hDlg, &psProp->wzcnet);
            bRet = TRUE;
        }
    
        break;
    
    case IDC_WZC_EDIT_WEP:
        if (HIWORD(wParam) == EN_CHANGE)
        {
            psProp->dwFlags |= WZCDLG_PROPS_KEYTOUCHED;
        }
        break;
    }
        
    return bRet;
}

BOOL NetworkWLanAuthenticationPropsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    PWLAN_CFG_PROPS psProps = (PWLAN_CFG_PROPS)GetWindowLong(hDlg, GWL_USERDATA);
    BOOL            fRet = 0;

    fRet = SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(IDD_WZC_WLAN_AUTHENTICATION_PAGE), hDlg, message, wParam, lParam );

    switch(message)
    {
    case WM_INITDIALOG:
        psProps = (PWLAN_CFG_PROPS)lParam;
        SetWindowLong(hDlg, GWL_USERDATA, (LPARAM)psProps);

        fRet = OnInitDialogAuthentication(hDlg, psProps);
        EnableWepKEditControl(hDlg, psProps);
        break;

    case WM_SETTINGCHANGE:
        SHHandleSipChange(hDlg, wParam, lParam, NULL);
        break;

    case WM_WINDOWPOSCHANGED:
    case WM_SIZE:
        SetDialogAutoScrollBarEx(hDlg);
        break;

    case WM_DESTROY:
        fRet = OnDestroyAuthentication(hDlg, psProps);
        break;
        
    case WM_COMMAND:
        fRet = OnCommandAuthentication(hDlg, psProps, wParam, lParam);
        break;

    case WM_NOTIFY:       
        break;

    case WM_ACTIVATE:
        // Pass message back to WiFi CPL main window unless the new/previous window is 
        // the parent window itself, in which case the CPL will handle it's own 
        // activation or deactivation. The CPL (psProps->psWLanInfo->hDlg) is a child
        // of the same parent as us.
        if (psProps && psProps->psWLanInfo)
        {
            HWND hwndPrevious = (HWND)lParam;
            if (hwndPrevious != GetParent(hDlg))
            {
                PostMessage (psProps->psWLanInfo->hDlg, message, wParam, lParam);
            }
        }
        break;
    }
    
    return fRet;
}


void UpdateData8021x(__in HWND hDlg,PSHWZCNET pwzcnet, BOOL fSetControl)
{
   BOOL bCheck = FALSE;

   if (fSetControl)
   {
       bCheck = BOOLIFY(SHWZCF_NET_8021X & pwzcnet->dwFlags);
       DDX_Check(hDlg, IDC_WZC_CHECK_ENABLE8021X, &bCheck, TRUE);      
   }
   else 
   {        
       DDX_Check(hDlg, IDC_WZC_CHECK_ENABLE8021X, &bCheck, FALSE);
       pwzcnet->dwFlags &= ~SHWZCF_NET_8021X;
       pwzcnet->dwFlags |= (bCheck ? SHWZCF_NET_8021X : 0);
   }
}

BOOL OnInitDialog8021x(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
	SHINITDLGINFO shidi = {0,};
	SHMENUBARINFO mbi = {0,};
	
	// Adding Scroll Bar support
    shidi.dwMask    = SHIDIM_FLAGS;
    shidi.hDlg      = hDlg;
    shidi.dwFlags   = SHIDIF_WANTSCROLLBAR | SHIDIF_SIZEDLG;

    SHInitDialog(&shidi);
	
	
	//Menubar
	mbi.cbSize = sizeof( SHMENUBARINFO );
	mbi.hwndParent = hDlg;
	mbi.dwFlags = SHCMBF_HMENU;
	mbi.nToolBarId = IDM_WIZARD_LAST;
	mbi.hInstRes = HINST_RESDLL;
	SHCreateMenuBar( &mbi );
	
    //SHDoneButton(hDlg, SHDB_HIDE);
    
    UpdateData8021x(hDlg, &psProp->wzcnet, TRUE);
    Init8021xControls(hDlg, psProp);    
    Enable8021xControls(hDlg, psProp);

    return FALSE;
}

BOOL OnDestroy8021x(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    Cleanup8021xControls(hDlg, psProp);
    return FALSE;
}

static BOOL OnApply8021x(HWND hDlg, PWLAN_CFG_PROPS psProp)
{
    HRESULT              hr = S_OK;
    SHWZCNET             wzcnet;
    PWZC_EAP_CB_ITEMDATA pEapData = NULL;
    DWORD                dwMask = 0;

    // Get the UI settings
    memcpy(&wzcnet, &psProp->wzcnet, sizeof(SHWZCNET));
    UpdateData8021x(hDlg, &wzcnet, FALSE);

    if (SHWZCF_NET_8021X & wzcnet.dwFlags)
    {
        HWND hcbEAPType = GetDlgItem(hDlg, IDC_WZC_COMBO_EAP_TYPE);
        int  nIndex;

        // Get the current selection
        nIndex = ComboBox_GetCurSel(hcbEAPType);
        CBR(CB_ERR != nIndex);

        // Get the EAP data
        pEapData = (PWZC_EAP_CB_ITEMDATA)ComboBox_GetItemData(hcbEAPType, nIndex);
        CBRA(CB_ERR != (int)pEapData && NULL != pEapData);
        wzcnet.dwEAPType = pEapData->pEapExtInfo->dwExtensionType;
    }

    // Apply the settings
    dwMask = SHWZCF_SETMASK_8021X;
    hr = SHWZCCopySettings(&psProp->wzcnet, &wzcnet, dwMask);
    CHR(hr);

    // Save the new EAP data
    LocalFree(psProp->pvEapData);
    psProp->pvEapData = NULL;
    psProp->cbEapData = 0;

    if (NULL != pEapData)
    {
        psProp->pvEapData = pEapData->pbAuthData;
        psProp->cbEapData = pEapData->dwAuthDataLen;
        pEapData->pbAuthData = NULL;  // so we don't free it twice
    }

    // Success
    psProp->dwMask |= dwMask;

Error:
    return SUCCEEDED(hr);
}

BOOL OnCommand8021x(HWND hDlg, PWLAN_CFG_PROPS psProp, WPARAM wParam, LPARAM lParam)
{
    BOOL bRet = FALSE;

    switch(LOWORD(wParam))
    {
    case IDC_WZC_COMBO_EAP_TYPE:
        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
             Enable8021xControls(hDlg, psProp);
             bRet = TRUE;
        }
        break;

    case IDC_WZC_CHECK_ENABLE8021X:
        Enable8021xControls(hDlg, psProp);
        bRet = TRUE;
        break;

    case IDC_WZC_BUTTON_EAP_PROPERTIES:
        OnEAPProps(hDlg, psProp);
        bRet = TRUE;
        break;

    case ID_BACK:
        OnApply8021x(hDlg, psProp);
        EndDialog(hDlg, IDD_WZC_WLAN_AUTHENTICATION_PAGE);
        break;

    case ID_FINISH:
        if (OnApply8021x(hDlg, psProp))
        {
            psProp->nReturn = IDOK;
            EndDialog(hDlg, 0);
        }
        break;
    }

    return bRet;
}

BOOL NetworkWLan8021xPropsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    PWLAN_CFG_PROPS psProps = (PWLAN_CFG_PROPS)GetWindowLong(hDlg, GWL_USERDATA);
    BOOL            fRet = 0;

    fRet = SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(IDD_WZC_WLAN_8021X_PAGE), hDlg, message, wParam, lParam );

    switch(message)
    {
    case WM_INITDIALOG:
        psProps = (PWLAN_CFG_PROPS)lParam;
        SetWindowLong(hDlg, GWL_USERDATA, (LPARAM)psProps);

        fRet = OnInitDialog8021x(hDlg, psProps);
        break;

    case WM_DESTROY:
        fRet = OnDestroy8021x(hDlg, psProps);
        break;
        
    case WM_COMMAND:
        fRet = OnCommand8021x(hDlg, psProps, wParam, lParam);
        break;

    case WM_NOTIFY:
        break;

    case WM_ACTIVATE:
        // Pass message back to WiFi CPL main window unless the new/previous window is 
        // the parent window itself, in which case the CPL will handle it's own 
        // activation or deactivation. The CPL (psProps->psWLanInfo->hDlg) is a child
        // of the same parent as us.
        if (psProps && psProps->psWLanInfo)
        {
            HWND hwndPrevious = (HWND)lParam;
            if (hwndPrevious != GetParent(hDlg))
            {
                PostMessage (psProps->psWLanInfo->hDlg, message, wParam, lParam);
            }
        }
        break;
    }
    
    return fRet;
}


HRESULT WZCCreateNetworkWLanPropsDlgs(HWND hwndOwner, PWLAN_CFG_PROPS psProps)
{
    HRESULT         hr            = S_OK;
    int             iNextPage     = IDD_WZC_WLAN_GENERAL_PROP_PAGE;
    DLGPROC         pfnDlgProc    = NULL;
    RECT            rcWorker;
    HWND            hwndWorker;

    SHInitExtraControls();

    // Create the worker window to cover up the calling app.
    hwndWorker = SHCreateWorkerWindow((LPTSTR)WORKERWINDOW_CLASS,
                          NULL,
                          hwndOwner,
                          WS_EX_NOACTIVATE,
                          WS_POPUP | WS_VISIBLE,
                          (LPTSTR)LoadString(HINST_RESDLL, IDS_WZC_SETTING_TITLE, NULL, 0),
                          0, 0);

    SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorker, 0);
    SetWindowPos(hwndWorker, HWND_TOPMOST, rcWorker.left, rcWorker.top,
                 rcWorker.right - rcWorker.left, rcWorker.bottom - rcWorker.top,
                 SWP_NOZORDER | SWP_NOACTIVATE);

    // Don't want a NULL parent
    if (NULL == hwndOwner)
    {
        hwndOwner = hwndWorker;
    }

    psProps->psWLanInfo->hwndNCPrsht = hwndOwner;

    while (iNextPage)
    {
        switch (iNextPage)
        {
        case IDD_WZC_WLAN_GENERAL_PROP_PAGE:
            pfnDlgProc = NetworkWLanGeneralPropsDlgProc;
            break;
        case IDD_WZC_WLAN_AUTHENTICATION_PAGE:
            pfnDlgProc = NetworkWLanAuthenticationPropsDlgProc;
            break;
        case IDD_WZC_WLAN_8021X_PAGE:
            pfnDlgProc = NetworkWLan8021xPropsDlgProc;
            break;
        default:
            pfnDlgProc = NULL;
            break;
        }

        iNextPage = DialogBoxParam(HINST_RESDLL,
                                   MAKEINTRESOURCE(iNextPage),
                                   hwndOwner,
                                   pfnDlgProc,
                                   (LPARAM)psProps);
    }

    psProps->psWLanInfo->hwndNCPrsht = NULL;

    DestroyWindow(hwndWorker);
    
    return hr;
}

