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
//
//==========================================================================*
//
// Filename:    WZCUI.cpp
//
// Owner:        
//
// Purpose:     Wireless zero config setting dialog for wireless network
//
//==========================================================================*

#include <windows.h>
#include <wpcpriv.h>
#include <types.h>
#include <wzcshell.h>
#include <ehm.h>

#include "netuip.h"
#include "wzcuip.h"
#include <resource.h>

#include "connmgr_plan.h"   // for the hangup
#include "connmgr_priv.h"

#include "wzcmsq.h"         // wzc logging message
#include "wzcintf.h"
#include "wzcmgr.h"

#include "appmetc.h"

#include <networkobject.h>

#include <shmenus.h>

///////////////////////////////////////////////////////////////////////////////////////////
///
static const TCHAR  c_szAccessTo[]    = TEXT("AccessTo");

// Used to help wifi notifications fire properly when leaving the CPL without closing
static const TCHAR gc_szMsgActivateHome[] = MSG_ACTIVATE_HOME;
UINT g_uMsgActivateHome;  // the "home activated" system message code

static const TCHAR gc_szMenuNamespace[] = TEXT("WiFi");
static const TCHAR gc_szSofkeyExtensions[] = TEXT("Connections_Menu");
static const TCHAR gc_szContextExtensions[] = TEXT("Connections_ContextMenu");

#define WM_SHWZC_CALLBACK (WM_APP)

#define SHWZCF_NET_CONNECT_MASK (SHWZCF_NET_CONNECTING | SHWZCF_NET_CONNECTED)

#define WZC_DEFAULT_SCAN_INTERVAL 60

// Externed from commctrl
void WINAPI ListView_SetItemSpacing(HWND hwndLV, int iHeight);

//==========================================================================*
// Function: CalculateStatusColumnWidth
//
// Purpose:  Calculate the with of state column
//           
// Arguments: 
//     [IN] psInfo  - pointer to the WLAN_INFO structure
//
// Return Values:
//     return the width of the column
//==========================================================================*
INT CalculateStatusColumnWidth(PWLAN_INFO psInfo)
{
    HRESULT hr;
    LPTSTR  pszStatus;
    SIZE    size;
    HFONT   hFont, hFontOld;
    HDC     hDC = GetDC(psInfo->hDlg);
    INT     nWidth = 0, i;

    // Get font from the list view
    hFont = (HFONT)SendMessage(psInfo->hwndNetList, WM_GETFONT, 0, 0);
    CWR(hFont);

    hFontOld = (HFONT)SelectObject(hDC, hFont);

    for (i = IDS_WZC_STATUS_FIRST; i <= IDS_WZC_STATUS_LAST; i++)
    {
        pszStatus = (LPTSTR)LoadString(HINST_RESDLL, i, NULL, 0);
        if (pszStatus)
        {
            // Get the length of the text
            GetTextExtentExPoint(hDC, pszStatus, _tcslen(pszStatus), 0, NULL, NULL, &size);

            if (size.cx > nWidth)
            {
                nWidth = size.cx;
            }
        }
    }

    SelectObject(hDC, hFontOld);

    nWidth += AM_NETUI_WZCLISTVIEWMARGIN_CX;

Error:
    ReleaseDC(psInfo->hDlg, hDC);
    return nWidth;
}

//==========================================================================*
// Function: InitNetworkListView
//
// Purpose:  Initialize the wireless network list view
//           
// Arguments: 
//     [IN] psInfo  - pointer to the WLAN_INFO structure
//
// Return Values:
//     HRESULT - S_OK for succeed
//==========================================================================*
static HRESULT InitNetworkListView(PWLAN_INFO psInfo)
{
    HRESULT   hr  = S_OK;
    LV_COLUMN lvc = {0};
    DWORD     dwStyle;
    RECT      rc;
    HWND      hwndNetList;
    INT       nStatusColumnWidth;
    
    hwndNetList = psInfo->hwndNetList = GetDlgItem(psInfo->hDlg, IDC_WZC_LIST_NETWORKS);
    GetClientRect(hwndNetList, &rc);

	RETAILMSG(1, (TEXT("[NETUI] InitNetworkListView hwndNetList %d \r\n"), hwndNetList ));


    //
    // Change the Style and StyleEx    
    dwStyle = GetWindowLong(hwndNetList, GWL_STYLE);
    SetWindowLong(hwndNetList, GWL_STYLE, (dwStyle | LVS_SHAREIMAGELISTS));
    ListView_SetExtendedListViewStyleEx(hwndNetList,
                                        LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE,
                                        LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE);

    //
    // Create state image lists
    psInfo->hImgs = DPI_ImageList_LoadImage(HINST_RESDLL, MAKEINTRESOURCE(IDB_WZC_STATE), AM_NETUI_WZCSTATEICONS_CX, 0, CLR_NONE, IMAGE_BITMAP, 0);

	RETAILMSG(1, (TEXT("[NETUI] DPI_ImageList_LoadImage(%d), IDB_WZC_STATE(%d) \r\n"), psInfo->hImgs, IDB_WZC_STATE ));

    CPR(psInfo->hImgs);
    ListView_SetImageList(hwndNetList, psInfo->hImgs, LVSIL_SMALL);

    //
    // Insert two columns network name and network state 
    lvc.mask = LVCF_FMT | LVCF_WIDTH;
    lvc.fmt  = LVCFMT_LEFT;

    //
    nStatusColumnWidth = CalculateStatusColumnWidth(psInfo);
    
    // Network name column
    lvc.cx   = rc.right - GetSystemMetrics(SM_CXVSCROLL) - nStatusColumnWidth;
    ListView_InsertColumn(hwndNetList, 0, &lvc);
    
    // Network state
    lvc.cx   = nStatusColumnWidth;
    ListView_InsertColumn(hwndNetList, 1, &lvc);

Error:
    return hr;
}

static LPARAM GetListItemLPARAM(HWND hwnd, INT nIndex)
{
    LVITEM lv = {0};
    BOOL   fRet;
    
    lv.iItem = nIndex;
    lv.mask  = LVIF_PARAM;
    fRet = ListView_GetItem(hwnd, &lv);

    return fRet ? lv.lParam : 0;
}


static void SetNetFilter(PWLAN_INFO pwiInfo, DWORD dwMode)
{
    switch(dwMode)
    {
    case SHWZCF_ALLOW_ALL:
        pwiInfo->nNetFilter = WZC_FILTER_ALL_AVAILABLE;
        break;

    case SHWZCF_ALLOW_AP:
        pwiInfo->nNetFilter = WZC_FILTER_ACCESS_POINT;
        break;

    case SHWZCF_ALLOW_ADHOC:
        pwiInfo->nNetFilter = WZC_FILTER_ADHOC;
        break;

    default:
        ASSERT(FALSE);
        break;
    }            
    
    SendDlgItemMessage(pwiInfo->hDlg, IDC_WZC_COMBO_FILTER, CB_SETCURSEL, pwiInfo->nNetFilter, 0L);
}

//==========================================================================*
// Function: InitFilterCombo
//
// Purpose:  Fill the combobox with 
//           (1) "All Available"             - no filter
//           (2) "Only access points"        - Ndis802_11Infrastructure only
//           (3) "Only computer to computer" - Ndis802_11IBSS
//           
// Arguments: 
//     [IN] psInfo    - pointer to the WLAN_INFO structure
//==========================================================================*
static BOOL  defaultInitDone=FALSE;
static HRESULT InitFilterCombo(PWLAN_INFO psInfo)
{
    HRESULT hr = S_OK;
    DWORD   dwState = SHWZCF_ALLOW_AP;
    INT     i, rgFilterList[] = { IDS_WZC_FILTER_ALL_AVAILABLE,
                                            IDS_WZC_FILTER_NON_ADHOC,
                                            IDS_WZC_FILTER_ADHOC                };

    // Populate the combo
    for (i = 0; i < ARRAYSIZE(rgFilterList); i++)
    {
        SendDlgItemMessage(psInfo->hDlg, IDC_WZC_COMBO_FILTER, 
                CB_ADDSTRING, 0, (LPARAM)LoadString(HINST_RESDLL, rgFilterList[i], NULL, 0));
    }

    // Get the current value, Deep Donw GetState uses "WZCQueryInterfaceEx" that always returns 2.. which is ALL 
    // Technically we should not depend on Platform Libraries on defaults.. This is only for PPC , 
    //  smart fone Calls SHWZCGetState(NULL,..) first time  . Please refer wificsp.cpp which is proper way of doing ..
    //  Simple OO advantage make code much better in wificsp.cpp.
    if( defaultInitDone ) 
    {
        hr = SHWZCGetState(psInfo->hshwzc, &dwState);
        CHR(hr);
    }
    else
    {
        defaultInitDone=TRUE; 
    }

    // Select the item from the list
    SetNetFilter(psInfo, dwState & SHWZCF_ALLOW_MASK);

Error:
    return hr;
}

//==========================================================================*
// Function: InsertFirstSettingsItem
//
// Purpose:  Add the first item of the list view ("Add New" or "Search for Networks")
//           
// Arguments: 
//     [IN] psInfo    - pointer to the WLAN_INFO structure
//
// Return Values:
//     HRESULT - S_OK for succeed
//==========================================================================*
static HRESULT InsertFirstSettingsItem(PWLAN_INFO psInfo)
{
    HRESULT hr = S_OK;
    LV_ITEM lv = { 0 };
    LPCTSTR pszText;

    pszText = (LPCTSTR)LoadString(HINST_RESDLL,
                                  (psInfo->dwHWFlags & SHWZCF_HW_ON) ? IDS_WZC_ADD_NEW_SETTINGS : IDS_WZC_SEARCH_NETWORKS,
                                  NULL,
                                  0);
    CBRA(NULL != pszText);

    // Fill list view structure and insert item
    lv.mask    = LVIF_TEXT | LVIF_IMAGE;
    lv.pszText = (LPTSTR)pszText;
    lv.iImage  = -1;

    CBR(0 <= ListView_InsertItem(psInfo->hwndNetList, &lv));

Error:
    return hr;
}

static int CALLBACK CompareNetworks(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    PCSHWZCNET pwzcnet1 = (PCSHWZCNET)lParam1;
    PCSHWZCNET pwzcnet2 = (PCSHWZCNET)lParam2;
    UINT       uScore1, uScore2;

    // If either of these is NULL, it's always less than the other network
    if (NULL == pwzcnet1) return -1;
    if (NULL == pwzcnet2) return 1;

    // Calculate the ranking group for each connection
    uScore1 = ((SHWZCF_NET_CONNECT_MASK & pwzcnet1->dwFlags) ? 3 : ((SHWZCF_NET_BROADCAST & pwzcnet1->dwFlags) ? ((SHWZCF_NET_PREFERRED & pwzcnet1->dwFlags) ? 2 : 1) : 0));
    uScore2 = ((SHWZCF_NET_CONNECT_MASK & pwzcnet2->dwFlags) ? 3 : ((SHWZCF_NET_BROADCAST & pwzcnet2->dwFlags) ? ((SHWZCF_NET_PREFERRED & pwzcnet2->dwFlags) ? 2 : 1) : 0));

    // Return value
    if (uScore1 > uScore2)
    {
        // Put pci1 above pci2
        return -1;
    }
    else if (uScore1 < uScore2)
    {
        // Put pci1 below pci2
        return 1;
    }
    else if (!(SHWZCF_NET_PREFERRED & pwzcnet1->dwFlags))
    {
        // They are both broadcasting but not preferred (otherwise they wouldn't be in the list)... sort alphabetically
        return _tcsicmp(pwzcnet1->szName, pwzcnet2->szName);
    }
    else
    {
        // They are both preferred and in the same rank... sort by preference (the connect attempt order)
        return (pwzcnet1 < pwzcnet2 ? -1 : (pwzcnet1 > pwzcnet2 ? 1 : 0));
    }
}

static HRESULT PopulateNetworkList(PWLAN_INFO psInfo)
{
    HRESULT hr = S_OK;
    LPCTSTR pszConnected, pszConnecting, pszAvailable, pszUnavailable;
    LV_ITEM lv = { 0 };
    UINT    i, cConfigs;
    BOOL    fNetAvailable = FALSE;

    CBREx(psInfo, E_INVALIDARG);

    // Load strings
    pszConnected = (LPCTSTR)LoadString(HINST_RESDLL, IDS_WZC_ACTIVE, NULL, 0);
	
    CBRA(NULL != pszConnected);

    pszConnecting = (LPCTSTR)LoadString(HINST_RESDLL, IDS_WZC_CONNECTING, NULL, 0);
	
    CBRA(NULL != pszConnecting);

    pszAvailable = (LPCTSTR)LoadString(HINST_RESDLL, IDS_WZC_AIRING, NULL, 0); 
	
    CBRA(NULL != pszAvailable);

    pszUnavailable = (LPCTSTR)LoadString(HINST_RESDLL, IDS_WZC_SILENT, NULL, 0); 
	
    CBRA(NULL != pszUnavailable);

    // Add the first setting item
    hr = InsertFirstSettingsItem(psInfo);
    CHR(hr);

    // Get the network list
    hr = SHWZCGetAllNetworks(psInfo->hshwzc, &psInfo->hdsaNetworks);
    CHR(hr);

    // Are there any items?
    cConfigs = DSA_GetItemCount(psInfo->hdsaNetworks);
    CBREx(0 != cConfigs, S_FALSE);
    ListView_SetItemCount(psInfo->hwndNetList, cConfigs + 1);

    // Add the networks to the list
    for (i = 0; i != cConfigs; ++i)
    {
        PCSHWZCNET pwzcnet = (PSHWZCNET)DSA_GetItemPtr(psInfo->hdsaNetworks, i);
        BOOL       fAdhoc = (SHWZCF_NET_ADHOC & pwzcnet->dwFlags);
        BOOL       fSecure;
        LPCTSTR    pszDesc;
        int        nIndex;
        BOOL       fUseSignalStrength = FALSE;

        // If this network is AP, mark it
        if (FALSE == fAdhoc)
        {
            psInfo->fAPAir = TRUE;
            fUseSignalStrength = TRUE;
        }

        // network is considered secure if it's 802.1x, or not OPEN and without
        // encryption enabled
        fSecure = BOOLIFY(pwzcnet->dwFlags & SHWZCF_NET_8021X) ||
            !( ( (Ndis802_11Encryption1Enabled != pwzcnet->dwEncryptionType) &&
            (Ndis802_11Encryption2Enabled != pwzcnet->dwEncryptionType) &&
            (Ndis802_11Encryption3Enabled != pwzcnet->dwEncryptionType) ) &&
            (Ndis802_11AuthModeOpen == pwzcnet->dwAuthentication) );

        // Get the icon and the description for the network
        if (SHWZCF_NET_CONNECTED & pwzcnet->dwFlags)
        {
            lv.iImage = fAdhoc ? WZCIMG_ADHOC_ACTIVE : 0;
            pszDesc = pszConnected;
        }
        else if (SHWZCF_NET_CONNECTING & pwzcnet->dwFlags)
        {
            lv.iImage = fAdhoc ? WZCIMG_ADHOC_AIRING : 0;
            pszDesc = pszConnecting;
        }
        else if (SHWZCF_NET_BROADCAST & pwzcnet->dwFlags)
        {
            lv.iImage = fAdhoc ? WZCIMG_ADHOC_AIRING : 0;
            pszDesc = pszAvailable;
        }
        else
        {
            ASSERT(SHWZCF_NET_PREFERRED & pwzcnet->dwFlags);
            lv.iImage = fAdhoc ? WZCIMG_ADHOC_SILENT : WZCIMG_INFRA_SILENT;

            pszDesc = pszUnavailable;
            // no need to get signal strength
            fUseSignalStrength = FALSE;
        }

        if (SHWZCF_NET_BROADCAST & pwzcnet->dwFlags)
        {
            fNetAvailable = TRUE;
        }

        // Update the icon to have signal strength if we're in Infrastructure mode
        if (fUseSignalStrength)
        {
            UINT uiStrength = pwzcnet->nRssi;

            if (uiStrength)
            {
                lv.iImage = WZCIMG_SIGNAL_1 + uiStrength - 1;
                if (fSecure)
                {
                    lv.iImage += WZCIMG_SIGNAL_SECURE_1 - WZCIMG_SIGNAL_1;
                }
            }
            else
            {
                lv.iImage = WZCIMG_INFRA_SILENT;
            }
        }

        ASSERT(lv.iImage);

        // Insert the item into the listview
        lv.mask     = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
        lv.iItem    = i;
        lv.iSubItem = 0;
        lv.pszText  = (LPTSTR)pwzcnet->szName;
        lv.lParam   = (LPARAM)pwzcnet;

        nIndex = ListView_InsertItem(psInfo->hwndNetList, &lv);
        CBR(0 <= nIndex);

        // Set the state text
        lv.mask     = LVIF_TEXT;
        lv.iItem    = nIndex;
        lv.iSubItem = 1;
        lv.pszText  = (LPTSTR)pszDesc;

        CBR(ListView_SetItem(psInfo->hwndNetList, &lv));
    }

    // Sort the networks
    CBREx(ListView_SortItems(psInfo->hwndNetList, CompareNetworks, NULL), E_OUTOFMEMORY);

Error:

    if(psInfo)
    {
        SetCursor(psInfo->hOldCursor);
        psInfo->hOldCursor = NULL;

        // We just turned power on, but didn't find any broadcasting networks
        if((psInfo->bUserInitiatedSearch) && (!fNetAvailable) && (psInfo->dwHWFlags & SHWZCF_HW_ON))
        {
            psInfo->bUserInitiatedSearch = FALSE;
            SH_BOXEX sbex = {0};
            sbex.cbSize = sizeof(sbex);
            sbex.dwStyle = SHBEXF_CLOSE_ON_ACTIVATION_LOSS|SHBEXF_SUPRESS_DEFAULT_SOUND;
            sbex.nTimeOut = 0;
            sbex.hwndOwner = psInfo->hDlg;
            sbex.sbextype = sbextTextBox;
            sbex.info.ti.pszText = LoadStringEtc(HINST_RESDLL, IDS_WZC_WIFI_SEARCH_ERROR, NULL);
            sbex.info.ti.bi.cButtons = 1;
            
            SHBoxEx(&sbex);
        }

        psInfo->bUserInitiatedSearch = FALSE;
    }
    
    return hr;
}

static void DeleteAllItems(PWLAN_INFO psInfo)
{
    // Clear the list view
    ListView_DeleteAllItems(psInfo->hwndNetList);
    psInfo->fAPAir = FALSE;

    // Clear the network list
    if (NULL != psInfo->hdsaNetworks)
    {
        // The alternative here is to call DSA_DeleteAllItems, but this saves
        // our poor little heap, since most likely we'll have approximately the
        // same number of networks in our next query
        psInfo->hdsaNetworks->cItem = 0;
    }
}

static void RelayoutDlg(PWLAN_INFO psInfo)
{
    if (NULL != psInfo)
    {
        HWND hwndStatic = GetDlgItem(psInfo->hDlg, IDC_WZC_STATIC_NO_WIRELESS_CARD_PRESEND);
        BOOL fShowList  = psInfo->fWirelessCardPresent;
        RECT rect;

        GetWindowRect(psInfo->hwndNetList, &rect);
        MapWindowPoints(NULL, psInfo->hDlg, (LPPOINT)&rect, 2 );

        SetWindowPos(hwndStatic, NULL, 
           rect.left, rect.top, RECTWIDTH(rect), 
           fShowList ? 10 : RECTHEIGHT(rect),
           (fShowList ? SWP_HIDEWINDOW : SWP_SHOWWINDOW) | SWP_NOZORDER); 

        // In order to let RCML resize wireless network listview, it just hide
        // it behind the static text when wireless card is not present
        SetWindowPos(psInfo->hwndNetList, hwndStatic, 0, 0, 0, 0,
           (fShowList ? SWP_NOZORDER : 0) | SWP_NOMOVE | SWP_NOSIZE);

        // When the rectangle surrounding the listview is moved, our text get's 
        // clipped.  Plus when we setwindowpos ourself and change our width
        // we aren't invalidating ourself, so we don't clear out old text.
        InvalidateRect(hwndStatic, NULL, TRUE);

        // Don't want network list to get input if it hides below the static text
        EnableWindow(psInfo->hwndNetList, fShowList);
    }
}

static HRESULT RefreshNetListView(PWLAN_INFO psInfo)
{
    HRESULT hr = S_OK;
    CNetID nidOrig;

    // Save the selected mark if one exists.  We may be doing a refresh while
    // the user has brought up the ContextMenu but has not yet made a selection.
    INT nItemSelected = ListView_GetNextItem(psInfo->hwndNetList, -1, LVNI_SELECTED);

    if (nItemSelected >= 1)
    {
        PCSHWZCNET pwzcnet = (PCSHWZCNET)GetListItemLPARAM(psInfo->hwndNetList, nItemSelected);
        nidOrig.Assign(pwzcnet);
    }

    SendMessage(psInfo->hwndNetList, WM_SETREDRAW, (WPARAM)FALSE, 0L);

    DeleteAllItems(psInfo);

    // If wireless card is not present, we want to hide wireless network list
    RelayoutDlg(psInfo);
               
    if (psInfo->fWirelessCardPresent)
    {
        hr = PopulateNetworkList(psInfo);
        CHR(hr);

        nItemSelected = 0;
        // Now set the selection mark to the matching network, if it still exists in the list.
        // It isn't enough to do a ListView_FindItem for pwzcnet, the refresh might have actually
        // caused a different network configuration to be stored at that address!
        if (nidOrig.IsValid())
        {
            INT nItems = ListView_GetItemCount(psInfo->hwndNetList);
            INT i;
            CNetID  nidTemp;
            // Don't bother with index 0, it is "Add New..."
            for (i = 1; i < nItems; i++)
            {
                PCSHWZCNET pwzcnetTemp = (PCSHWZCNET) GetListItemLPARAM(psInfo->hwndNetList, i);
                nidTemp.Assign(pwzcnetTemp);
                if (0 == nidOrig.Compare(&nidTemp, FALSE))
                {
                    nItemSelected = i;
                    break;
                }
            }
        }

        ListView_SetItemState(psInfo->hwndNetList, nItemSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
    }

Error:
    SendMessage(psInfo->hwndNetList, WM_SETREDRAW, (WPARAM)TRUE, 0L);
    RedrawWindow(psInfo->hwndNetList, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);

    return hr;
}

static HRESULT OnChangeNetFilter(PWLAN_INFO psInfo)
{
    HRESULT hr = S_OK;
    INT     nNetFilter;
    
    nNetFilter = SendDlgItemMessage(psInfo->hDlg, IDC_WZC_COMBO_FILTER, CB_GETCURSEL, 0, 0L);
    ASSERT(WZC_FILTER_ALL_AVAILABLE <= nNetFilter && nNetFilter <= WZC_FILTER_ADHOC);

    if (nNetFilter != psInfo->nNetFilter)
    {
        DWORD dwState = 0;

        // Get the current state
        SHWZCGetState(psInfo->hshwzc, &dwState);
        dwState &= ~SHWZCF_ALLOW_MASK;

        // Convert to infrastructure mode
        psInfo->nNetFilter = (WZC_FILTER) nNetFilter;
        switch(nNetFilter)
        {
        case WZC_FILTER_ALL_AVAILABLE:
            dwState |= SHWZCF_ALLOW_ALL;
            break;

        case WZC_FILTER_ACCESS_POINT:
            dwState |= SHWZCF_ALLOW_AP;
            break;

        case WZC_FILTER_ADHOC:
            dwState |= SHWZCF_ALLOW_ADHOC;
            break;

        default:
            // There is no default case, see ASSERT above
            CBR(FALSE);
        }

        // Set the "Network to access" setting to WZC engine
        hr = SHWZCSetState(psInfo->hshwzc, dwState);
        CHR(hr);
        
        hr = RefreshNetListView(psInfo);
    }

Error:
    return hr;
}

//==========================================================================*
// Function: OnMenuPopup
//
// Purpose:  Enable and disable wireless network context menu items.
//           We need to show/hide connect or disconnect items according to
//           the connection state of the network.
//           The first listview item "add new setting..." doesn't have a context menu
//           
// Arguments: 
//     [IN] psInfo - pointer to the WLAN_INFO structure
//     [IN] hMenu  - HANDLE to the menu
//     [IN] iItem   - item need to show menu
//
// Return Values:
//     BOOL - TRUE for handle it successfully
//==========================================================================*
static void OnMenuPopup(PWLAN_INFO psInfo, HMENU hMenu, INT iItem)
{
    HRESULT    hr = S_OK;
    PCSHWZCNET pwzcnet;
    DWORD      dwState = 0, dwSize;
    BOOL       fEnableConnect = BOOLIFY(iItem);
    BOOL       fEnableRemove = BOOLIFY(iItem);
    BOOL       fEnableEdit = BOOLIFY(iItem);
    LPTSTR     pNetwork;

    // "Turn Wi-Fi Off/On"
    RemoveMenu(hMenu, IDM_WZC_WIFI_POWERCMD, MF_BYCOMMAND);
    InsertMenu(hMenu, (UINT)-1, MF_BYPOSITION|MF_STRING, IDM_WZC_WIFI_POWERCMD,
               LoadStringEtc(HINST_RESDLL, IDS_WZC_WIFI_POWERON + ((psInfo->dwHWFlags & SHWZCF_HW_ON) ? 1:0), NULL));

    pwzcnet = (PCSHWZCNET)GetListItemLPARAM(psInfo->hwndNetList, iItem);
    CBR(pwzcnet != NULL);

    dwSize = ARRAYSIZE(pwzcnet->szName);
    pNetwork = (LPTSTR)LocalAlloc(NONZEROLPTR, dwSize * sizeof(TCHAR));
    CPR(pNetwork);
    StringCchCopy(pNetwork, dwSize, pwzcnet->szName);
    
    ((CNetworkObject*)(psInfo->pNetworkObject))->SetData(pNetwork, CF_UNICODETEXT);
    SHQueryMenuExtensions(psInfo->hContextExt ? psInfo->hContextExt : psInfo->hSoftKeyExt,
                          hMenu, 0xFFFFFFFF, IDM_WZC_MENU_EXT_FIRST, IDM_WZC_MENU_EXT_LAST,
                          CMF_NORMAL);

    // Get the current state
    SHWZCGetState(psInfo->hshwzc, &dwState);

    // If the selected network is ad-hoc, but there are infrastructure networks
    // airing, we can't connect, since ad-hoc networks are always considered
    // connected, unless the hardware is limited to use ONLY ad-hoc networks.
    if ((SHWZCF_NET_ADHOC & pwzcnet->dwFlags) &&
        SHWZCF_ALLOW_ADHOC != (SHWZCF_ALLOW_MASK & dwState) &&
        psInfo->fAPAir)
    {
        fEnableConnect = FALSE;
    }

    // If we're already connected to the selected network, we can't
    // "re-connect"
    else if (SHWZCF_NET_CONNECT_MASK & pwzcnet->dwFlags)
    {
        fEnableConnect = FALSE;
    }

    // If the selected network isn't preferred, disable the "Remove Settings"
    if (0 == (SHWZCF_NET_PREFERRED & pwzcnet->dwFlags))
    {
        fEnableRemove = FALSE;
    }

    // If the selected network is a "permanent" non-editable network, disable both
    // "Remove Settings" and "Edit"
    if (SHWZCIsPermanentNetwork(pwzcnet->szName, BOOLIFY(SHWZCF_NET_ADHOC & pwzcnet->dwFlags)))
    {
        fEnableRemove = FALSE;
        fEnableEdit = FALSE;
    }

Error:
    EnableMenuItem(hMenu, IDM_WZC_NETWORK_CONNECT, fEnableConnect ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hMenu, IDM_WZC_NETWORK_REMOVE, fEnableRemove ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hMenu, IDM_WZC_NETWORK_PROPERTIES, fEnableEdit ? MF_ENABLED : MF_GRAYED);
    return;
}

//==========================================================================*
// Function: OnNetworkContextMenu
//
// Purpose:  Display wireless network context menu.
//           The menu has two items: connect/disconnect/remove
//           We need to show/hide connect or disconnect items according to
//           the connection state of the network.
//           The first listview item "add new setting..." doesn't have a context menu
//           
// Arguments: 
//     [IN] psInfo - pointer to the WLAN_INFO structure
//     [IN] pt     - position which menu need to locate
//     [IN] item   - item need to show context menu
//
// Return Values:
//     BOOL - TRUE for handle it successfully
//==========================================================================*
static BOOL OnNetworkContextMenu(PWLAN_INFO psInfo, POINT pt, INT iItem)
{
    HRESULT    hr    = S_OK;
    HMENU      hMenu = NULL;
    HMENU      hMenuTrack = NULL;
    IUnknown   *pIUnknown = NULL;
    HANDLE     hCMExtensions = NULL;
        
    // The first item is "Add new network..." item. It doesn't need a context menu.
    CBR(iItem > 0);

    hMenu = LoadMenu(HINST_RESDLL, MAKEINTRESOURCE(IDR_WZC_MENU_NETWORK));
    CWR(hMenu);

    hMenuTrack = GetSubMenu(hMenu, 0);
    CWR(hMenuTrack);

    InsertMenu(hMenuTrack, (UINT)-1, MF_BYPOSITION|MF_STRING, IDM_WZC_WIFI_POWERCMD,
               LoadStringEtc(HINST_RESDLL, IDS_WZC_WIFI_POWERON + ((psInfo->dwHWFlags & SHWZCF_HW_ON) ? 1:0), NULL));

    SHLoadMenuExtensions((IUnknown*)psInfo->pNetworkObject,
                         gc_szMenuNamespace,
                         gc_szContextExtensions,
                         &psInfo->hContextExt);

    TrackPopupMenu(hMenuTrack, TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0,
        psInfo->hDlg, NULL);

    SHFreeContextMenuExtensions(psInfo->hContextExt);
    psInfo->hContextExt = NULL;

Error:
    if (hMenuTrack)
    {
        CloseHandle(hMenuTrack);
    }
    if (hMenu)
    {
        DestroyMenu(hMenu);
    }
    return SUCCEEDED(hr);
}

typedef struct _FINDGUID
{
    LPTSTR pszGUID;
    UINT   cchGUID;
    UINT   uIndex;
    UINT   uDesired;
} FINDGUID;

static BOOL CALLBACK EnumFindGUID(LPCTSTR pszGUID, LPCWSTR pszName, LPVOID pvParam)
{
    FINDGUID *pfg = (FINDGUID*)pvParam;
    if (!_tcscmp(pszGUID, pfg->pszGUID))
    {
        return FALSE;
    }

    ++pfg->uIndex;
    return TRUE;
}

static BOOL CALLBACK EnumSetGUID(LPCTSTR pszGUID, LPCWSTR pszName, LPVOID pvParam)
{
    FINDGUID *pfg = (FINDGUID*)pvParam;
    if (pfg->uIndex == pfg->uDesired)
    {
        VHR(StringCchCopy(pfg->pszGUID, pfg->cchGUID, pszGUID));
        return FALSE;
    }

    ++pfg->uIndex;
    return TRUE;
}

static HRESULT ApplyProperties(PWLAN_INFO psInfo, const WLAN_CFG_PROPS *psProps)
{
    HRESULT  hr = S_OK;
    FINDGUID fg;
    TCHAR    szGUID[MAX_PATH];
    DWORD dwHardwareState;

    fg.pszGUID = szGUID;
    fg.cchGUID = ARRAYSIZE(szGUID);
    fg.uIndex = 0;
    fg.uDesired = psProps->uMeta;

    // Set the network data
    hr = SHWZCSetNetwork(psInfo->hshwzc, &psProps->wzcnet, psProps->pszKey,
        psProps->pvEapData, psProps->cbEapData, psProps->dwMask);
    CHR(hr);

    // Copy the GUID string
    CBR(S_FALSE == SHWZCEnumDestinations(EnumSetGUID, &fg));
    hr = SHWZCSetDestinationGUID(psInfo->hshwzc, psProps->wzcnet.szName,
        BOOLIFY(SHWZCF_NET_ADHOC & psProps->wzcnet.dwFlags), szGUID);
    CHR(hr);

    // Refresh the view
    RefreshNetListView(psInfo);

    dwHardwareState = SHWZCGetHardwareFlags();
    if (!(dwHardwareState & SHWZCF_HW_CONNECTED) && !(dwHardwareState & SHWZCF_HW_CONNECTING))
    {
        hr = SHWZCConnectNetwork(psInfo->hshwzc, psProps->wzcnet.szName,
            BOOLIFY(SHWZCF_NET_ADHOC & psProps->wzcnet.dwFlags));
        CHR(hr);
    }        
Error:
    return hr;
}

//==========================================================================*
// Function: OnNetworkProperties
//
// Purpose:  (1) Display the network properties dialog
//           (2) Move(insert) the network to the head of the prefered list
//           
// Arguments: 
//     [IN] psInfo - pointer to the WLAN_INFO structure
//     [IN] item   - item need to show context menu
//
// Return Values:
//     BOOL - TRUE for handle it successfully
//==========================================================================*
static BOOL OnNetworkProperties(PWLAN_INFO psInfo, INT iItem)
{
    HRESULT        hr = S_OK;
    WLAN_CFG_PROPS sProps = {0};
    PCSHWZCNET     pwzcnet = (PCSHWZCNET)GetListItemLPARAM(psInfo->hwndNetList, iItem);
    BOOL           fAdhoc;
    TCHAR          szGUID[MAX_PATH];
    FINDGUID       fg;

    CBR(iItem > 0 && pwzcnet != NULL);

    // If the selected network is a "Permanent" non-editable network, exit without 
    // showing the NetworkProperties pages
    fAdhoc = BOOLIFY(SHWZCF_NET_ADHOC & pwzcnet->dwFlags);
    CBREx (!SHWZCIsPermanentNetwork(pwzcnet->szName, fAdhoc), S_FALSE);

    // Get the current EAP data
    hr = SHWZCGetNetwork(psInfo->hshwzc, pwzcnet->szName, fAdhoc,
        &sProps.wzcnet, &sProps.pvEapData, &sProps.cbEapData);
    CHR(hr);

    // We don't care about the result, since if this fails we just won't match
    // with any networks and we'll use the default
    szGUID[0] = 0;
    SHWZCGetDestinationGUID(psInfo->hshwzc, pwzcnet->szName, fAdhoc,
        szGUID, ARRAYSIZE(szGUID));

    fg.pszGUID = szGUID;
    fg.uIndex = 0;
    if (S_FALSE == SHWZCEnumDestinations(EnumFindGUID, &fg))
    {
        sProps.uMeta = fg.uIndex;
    }

    sProps.psWLanInfo = psInfo;
    
    hr = WZCCreateNetworkWLanPropsDlgs(psInfo->hDlg, &sProps);
    CHR(hr);

    if (IDOK == sProps.nReturn)
    {
        hr = ApplyProperties(psInfo, &sProps);
        CHR(hr); 
    }

Error:
    LocalFree(sProps.pvEapData);
    return SUCCEEDED(hr);
}

static HRESULT OnAddNewSetting(PWLAN_INFO psInfo)
{
    HRESULT        hr      = S_OK;
    WLAN_CFG_PROPS sProps  = {0};
    LPTSTR         pszSSID = NULL;

    // Init    
    sProps.dwFlags = WZCDLG_PROPS_RWSSID | WZCDLG_PROPS_RWINFR;
    sProps.dwMask = SHWZCF_SETMASK_ALL;
    SHWZCCreateDefaultNetwork(&sProps.wzcnet);

    // Save the dialog info so we can check for duplicate names
    sProps.psWLanInfo = psInfo;

    hr = WZCCreateNetworkWLanPropsDlgs(psInfo->hDlg, &sProps);
    CHR(hr);

    if (IDOK == sProps.nReturn)
    {
        hr = ApplyProperties(psInfo, &sProps);
        CHR(hr);
    }

Error:
    LocalFree(sProps.pvEapData);
    return hr;
}

static HRESULT OnChangePowerState(PWLAN_INFO psInfo)
{
    HRESULT hr = S_OK;
    
    CHR(SHWZCSetPowerState(!(psInfo->dwHWFlags & SHWZCF_HW_ON)));

    if (!psInfo->hOldCursor)
    {
        psInfo->hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
    }

Error:
    return hr;
}

static HRESULT OnFirstSettingActivate(PWLAN_INFO psInfo)
{
    HRESULT hr = S_OK;

    CBREx(psInfo, E_INVALIDARG);

    if (psInfo->dwHWFlags & SHWZCF_HW_ON)
    {
        CHR(OnAddNewSetting(psInfo));
    }
    else
    {
        psInfo->bUserInitiatedSearch = TRUE;
        CHR(OnChangePowerState(psInfo));
    }

Error:
    return hr;
}

static HRESULT OnConnect(PWLAN_INFO psInfo, INT iItem)
{
    HRESULT    hr = S_OK;
    PCSHWZCNET pwzcnet = (PCSHWZCNET)GetListItemLPARAM(psInfo->hwndNetList, iItem);
    SHWZCNET   wzcnet;
    BOOL       fSecure;

    // The first item is "Add net settings...". It cannot be deleted.
    CBR(iItem > 0 && pwzcnet != NULL);
    memcpy(&wzcnet, pwzcnet, sizeof(SHWZCNET));

    // If the item isn't preferred
    if (!(SHWZCF_NET_PREFERRED & wzcnet.dwFlags))
    {
        DWORD dwNoFastConnect = 0;

        fSecure = BOOLIFY(wzcnet.dwFlags & SHWZCF_NET_8021X) ||
                !( ( (Ndis802_11Encryption1Enabled != wzcnet.dwEncryptionType) &&
                (Ndis802_11Encryption2Enabled != wzcnet.dwEncryptionType) &&
                (Ndis802_11Encryption3Enabled != wzcnet.dwEncryptionType) ) &&
                (Ndis802_11AuthModeOpen == wzcnet.dwAuthentication) );

        // If secure or fastconnect is disabled, show the settings wizard, else connect
        RegistryGetDWORD(HKEY_LOCAL_MACHINE, WZC_REGKEY_BASE, WZC_REGKEY_DISABLEFASTCONNECT, &dwNoFastConnect);
        if (fSecure || dwNoFastConnect != 0)
        {
            CBR(OnNetworkProperties(psInfo, iItem));
        }
        else
        {
            SHWZCSetNetwork(psInfo->hshwzc, &wzcnet, NULL, NULL, 0, SHWZCF_SETMASK_ALL);
        }
    }

    // disable SK1 Connect while we're attempting to connect for this network
    SHEnableSoftkey(psInfo->hwndNetListMB, 0, TRUE, FALSE);

    // Ensure that power is on. Ignore errors, this is a "Best-effort" attempt
    SHWZCSetPowerState(TRUE);
        
    // Now connect directly (we'll automatically get a refresh event)
    hr = SHWZCConnectNetwork(psInfo->hshwzc, wzcnet.szName,
        BOOLIFY(SHWZCF_NET_ADHOC & wzcnet.dwFlags));
    CHR(hr);

Error:
    if(FAILED(hr))
    {
        // re-enable SK since something went wrong
        SHEnableSoftkey(psInfo->hwndNetListMB, 0, TRUE, TRUE);
    }
    
    return hr;
}

static HRESULT OnRemoveSetting(PWLAN_INFO psInfo, INT iItem)
{
    HRESULT    hr = S_OK;
    PCSHWZCNET pwzcnet =(PCSHWZCNET)GetListItemLPARAM(psInfo->hwndNetList, iItem);

    // The first item is "Add net settings...". It cannot be deleted.
    CBR(iItem > 0 && pwzcnet != NULL);

    // Delete the network
    hr = SHWZCDeleteNetwork(psInfo->hshwzc, pwzcnet->szName,
        BOOLIFY(SHWZCF_NET_ADHOC & pwzcnet->dwFlags));
    CHR(hr);

    // Refresh list view
    RefreshNetListView(psInfo);

Error:
    return hr;
}


HRESULT SetCallbackRegistration(BOOL fRegistration, PWLAN_INFO psInfo) 
{
    HRESULT hr = S_OK;

    CBRA(psInfo);

    // Only perform the callback registration if it will not be redundant
    if (fRegistration && NULL == psInfo->hshwzc) 
    {
        hr = SHWZCCreateCallback(psInfo->hDlg, WM_SHWZC_CALLBACK, 
         WZC_DEFAULT_SCAN_INTERVAL, SHWZCF_CB_OVERRIDE, &psInfo->hshwzc);
        CHR(hr);
    }
    else if (!fRegistration && NULL != psInfo->hshwzc) 
    {
        // Apply any changes we made before destroying the callback
        hr = SHWZCApplyChanges(psInfo->hshwzc);
        CHR(hr);
        hr = SHWZCDestroyCallback(psInfo->hshwzc);
        CHR(hr);
        psInfo->hshwzc = NULL;
    }
    else 
    {
        hr = S_FALSE;
    }

Error:
    return hr;
}

static HRESULT OnActivate(UINT wParam, HWND hwndPrevious, PWLAN_INFO psInfo) 
{
    HRESULT hr = S_OK;

    BOOL fActivate = LOWORD(wParam);
    psInfo->bActivate = (fActivate != WA_INACTIVE);

    // The CPL should register for callbacks whenever
    // the user is working inside the wifi CPL.
    if (fActivate) 
    {
        hr = SetCallbackRegistration(TRUE, psInfo);
        CHR(hr);
    }
    // Normal wifi notifications should continue when the user leaves the wifi CPL,
    // so the CPL should stop registering for callbacks when the user leaves.
    // Unfortunately, just because this window receives a deactivate does not
    // mean the user has left the CPL, for example, the user might
    // have tapped a taskbar icon, received a reminder, or tapped to configure
    // one of the detected wifi networks.
    // We deactivate in any case the new window is not a child of the wifi CPL.
    // If the user leaves the wifi CPL while working in a child window, we
    // still have the OnActivateHome handler to stop overriding notifications
    // once the user returns to the Today screen.  

    // Clicking to configure an existing network or adding a new configuration
    // is still part of the wifi CPL, so we still want to override notifcations.
    // The configuration window has the same parent as psInfo->hDlg.  
    else if (NULL == hwndPrevious || GetParent(hwndPrevious) != GetParent(psInfo->hDlg)) 
    {
        hr = SetCallbackRegistration(FALSE, psInfo);
        CHR(hr);
    }

Error:
    return hr;
}


static HRESULT OnActivateHome(PWLAN_INFO psInfo)
{
    HRESULT hr = S_OK;

    // We always unregister callbacks when the home screen is activated. This
    // prevents us from permanently blocking Wi-Fi popups if we were
    // deactivated by a child dilaog, and then the user went Home without
    // closing the dialog.
    hr = SetCallbackRegistration(FALSE, psInfo);
    CHR(hr);

Error:
    return hr;
}


static BOOL OnCommand(PWLAN_INFO psWLanInfo, WPARAM wParam)
{
    BOOL fRet = TRUE;
    INT  nItemSelected;

    nItemSelected = ListView_GetNextItem(psWLanInfo->hwndNetList, -1, LVNI_SELECTED);

    switch(LOWORD(wParam))
    {
    case IDM_WZC_NETWORK_CONNECT:
        OnConnect(psWLanInfo, nItemSelected);
        break;

    case IDS_WZC_ADD_NEW_SETTINGS:
        OnAddNewSetting(psWLanInfo);
        break;

    case IDM_WZC_NETWORK_REMOVE:
        OnRemoveSetting(psWLanInfo, nItemSelected);
        break;

    case IDM_WZC_NETWORK_PROPERTIES:
        OnNetworkProperties(psWLanInfo, nItemSelected);
        break;

    case IDM_WZC_ADVANCED:
        DialogBox(HINST_RESDLL, MAKEINTRESOURCE(IDD_WZC_WLAN_ADVANCED), psWLanInfo->hDlg, WZCWLanAdvancedDlgProc);
        break;

    case IDC_WZC_COMBO_FILTER:
        if (HIWORD(wParam) == CBN_SELENDOK)
        {
            OnChangeNetFilter(psWLanInfo);
        }
        break;

    case IDM_WZC_WIFI_POWERCMD:
        OnChangePowerState(psWLanInfo);
        break;

    case IDOK:
        // Since we're applying any changes when the dialog is deactivated, we
        // don't need to apply here -- we'll get a deactivate right away.
        break;

    default:
        if (LOWORD(wParam) >= IDM_WZC_MENU_EXT_FIRST && LOWORD(wParam) <= IDM_WZC_MENU_EXT_LAST)
        {
            SHInvokeContextMenuCommand(psWLanInfo->hwndNetList,
                                       LOWORD(wParam),
                                       psWLanInfo->hContextExt ? psWLanInfo->hContextExt : psWLanInfo->hSoftKeyExt);
        }
        else
        {
            fRet = FALSE;
        }
    }

    return fRet;
}

static BOOL OnNotify(PWLAN_INFO psInfo, WPARAM wParam, LPARAM lParam)
{
    NM_LISTVIEW *pnmlv;
    LPNMHDR pnmh = (LPNMHDR) lParam; 
    BOOL    bRet = FALSE; 
    INT     nItemSelected ;

    if (NULL == pnmh)
    {
        goto Error;
    }

    switch(LOWORD(wParam))
    {
    case IDC_WZC_LIST_NETWORKS:
        pnmlv = (NM_LISTVIEW*)lParam;

        if (-1 != pnmlv->iItem)
        {
            nItemSelected = pnmlv->iItem;
        }
        else
        {
            // Always use a valid default item even if notification doesn't provide one
            nItemSelected = ListView_GetNextItem(psInfo->hwndNetList, -1, LVNI_SELECTED);
        }

        switch(pnmlv->hdr.code)
        {
        case GN_CONTEXTMENU:
            bRet = OnNetworkContextMenu(psInfo, ((NMRGINFO *)lParam)->ptAction, nItemSelected);
            break;
            
        case LVN_ITEMACTIVATE:
            if (nItemSelected == 0)
            {
                // User hit the first item which is "Add new setting..." or "Search for Networks"
                OnFirstSettingActivate(psInfo);
            }
            else if (nItemSelected > 0)
            {
                // User hit a wireless network, bring up the config properties dialog
                SendMessage(psInfo->hDlg, WM_COMMAND, MAKEWPARAM(IDM_WZC_NETWORK_PROPERTIES, 0), 0L);
            }                       
            bRet = TRUE;
            break;

        case NM_RECOGNIZEGESTURE:
        {
            DWORD dwItem = ((NMRGINFO*)lParam)->dwItemSpec;
            
            if (0 == dwItem || (DWORD)-1 == dwItem || psInfo->bConnecting)
            {
                // No context menu for "Add new URL" or 
                // if user hasn't clicked on an item or
                // We are connecting to a wireless network
                SetWindowLong(psInfo->hDlg, DWL_MSGRESULT, TRUE);
                return TRUE;
            }
        }
        case LVN_ITEMCHANGED:
        {
            if (pnmlv->uChanged & LVIF_STATE && pnmlv->uNewState & LVIS_SELECTED)
            {
                BOOL fEnable = BOOLIFY(nItemSelected);
                
                if(fEnable)
                {
                    PCSHWZCNET pwzcnet = (PCSHWZCNET)GetListItemLPARAM(psInfo->hwndNetList, nItemSelected);
                    ASSERT(pwzcnet);

                    fEnable = !BOOLIFY(SHWZCF_NET_CONNECT_MASK & pwzcnet->dwFlags);
                }

                // Enable SK1 if selected item is a network and user isn't already connecting to it.
                SHEnableSoftkey(psInfo->hwndNetListMB, 0, TRUE, fEnable);
            }
            break;
        }
        }
        break;

    default:        
        switch(pnmh->code)
        {
            case PSN_APPLY:
                bRet = SendMessage(psInfo->hDlg, WM_COMMAND, IDOK, 0L);
                break;
            case PSN_KILLACTIVE:
            case PSN_SETACTIVE:
                ShowWindow(psInfo->hwndNetListMB, (pnmh->code == PSN_SETACTIVE ? SW_SHOW : SW_HIDE));
                break;
        }
        break;
    }

Error:
    return bRet;
}


static void OnDestroy(PWLAN_INFO psInfo)
{
    DeleteAllItems(psInfo);
    if (NULL != psInfo->hdsaNetworks)
    {
        DSA_Destroy(psInfo->hdsaNetworks);
    }

    // Destroy the callback
    VHR(SetCallbackRegistration(FALSE, psInfo));

    // Free the image list
    if (psInfo->hImgs)
    {
        ImageList_Destroy(psInfo->hImgs);
    }

    SHFreeContextMenuExtensions(psInfo->hSoftKeyExt);
    ((IUnknown*)psInfo->pNetworkObject)->Release();
}

static HRESULT OnInitDialog(PWLAN_INFO psInfo)
{
    HRESULT hr = S_OK;
    TBBUTTONINFO tbbi = {0};
    HMENU   hMenu;

	
	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog1 \r\n") ));

	// Size the dialog: full screen.
	SHINITDLGINFO sidi = {0};
	sidi.dwMask = SHIDIM_FLAGS;
	sidi.dwFlags = SHIDIF_SIZEDLG | SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN;
	sidi.hDlg = psInfo->hDlg;
	SHInitDialog(&sidi);
	
    SHMENUBARINFO mbi = {0};
    mbi.cbSize = sizeof(SHMENUBARINFO);
    mbi.hwndParent = psInfo->hDlg;
    mbi.nToolBarId = IDR_WZC_SKBAR_NETWORK;
    mbi.hInstRes = HINST_RESDLL;
    mbi.dwFlags = SHCMBF_HMENU | SHCMBF_HIDESIPBUTTON;
    
    SHCreateMenuBar(&mbi);
    psInfo->hwndNetListMB = mbi.hwndMB;

    tbbi.cbSize = sizeof(TBBUTTONINFO);
    tbbi.dwMask = TBIF_COMMAND;
    tbbi.dwMask |= TBIF_BYINDEX;
    
	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog2 \r\n") ));

    VBR(SendMessage(mbi.hwndMB, TB_GETBUTTONINFO, 1, (LPARAM)&tbbi) == 1);
    hMenu = (HMENU)SendMessage(psInfo->hwndNetListMB, SHCMBM_GETSUBMENU, 0, tbbi.idCommand);
    VWR(hMenu);

    psInfo->pNetworkObject = new CNetworkObject;
    VPR(psInfo->pNetworkObject);
    SHLoadMenuExtensions((IUnknown*)psInfo->pNetworkObject,
                         gc_szMenuNamespace,
                         gc_szSofkeyExtensions,
                         &psInfo->hSoftKeyExt);

	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog3 \r\n") ));

    // Initialize the dialog contents
    hr = InitNetworkListView(psInfo);
    CHR(hr);

	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog4 \r\n") ));
    
    // Override notification UI
    hr = SetCallbackRegistration(TRUE, psInfo);
    CHR(hr);

	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog5 \r\n") ));

    // Get the "home activate" message to ensure proper
    // notification behavior if the user leaves the CPL
    g_uMsgActivateHome = RegisterWindowMessage(gc_szMsgActivateHome);
    CWR(0 != g_uMsgActivateHome);

	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog6 \r\n") ));

    // Initialize the filter
    hr = InitFilterCombo(psInfo);
    CHR(hr);

	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog7 \r\n") ));

    psInfo->dwHWFlags = SHWZCGetHardwareFlags();

	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog8 \r\n") ));

    // Populate the network list
    hr = PopulateNetworkList(psInfo);
    CHR(hr);

	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog9 \r\n") ));


    // We assume that we have hardware if we're here
    psInfo->fWirelessCardPresent = TRUE;

    ListView_SetItemState(psInfo->hwndNetList, 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	RETAILMSG(1, (TEXT("[NETUI] wzcui OnInitDialog10 \r\n") ));

Error:
    return hr;
};

BOOL WZCWLanNetworkDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    PWLAN_INFO psWLanInfo = (PWLAN_INFO)GetWindowLong(hDlg, GWL_USERDATA);
    BOOL   fRet = PropPageDialogHelper(hDlg, message, wParam, lParam);

    // Figure out whther to show the listbox or "insert card" text and hide 
    // the control we don't need. We do this before calling SHRCMLDialogProc
    // because these controls occupy the same position - if they're both
    // visible, the dialog resizing code gets confused.
    if (WM_INITDIALOG== message)
    {
        psWLanInfo = (PWLAN_INFO)(((PROPSHEETPAGE*)lParam)->lParam);
        if (psWLanInfo)
        {
            psWLanInfo->hDlg = hDlg;        
            SetWindowLong(hDlg, GWL_USERDATA, (LPARAM)psWLanInfo);
                        
            OnInitDialog(psWLanInfo);      
            
			RETAILMSG(1, (TEXT("[NETUI] WZCWLanNetworkDlgProc fWirelessCardPresent(%d) \r\n"), psWLanInfo->fWirelessCardPresent));

            if (psWLanInfo->fWirelessCardPresent)
            { // show listbox, hide textbox
                ShowWindow(GetDlgItem(hDlg,IDC_WZC_STATIC_NO_WIRELESS_CARD_PRESEND),SW_HIDE);
            }
            else
            {
                ShowWindow(GetDlgItem(hDlg,IDC_WZC_LIST_NETWORKS),SW_HIDE);
            }
        }
    }

    fRet = SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(IDD_WZC_WLAN_NETWORK), hDlg, message, wParam, lParam);

    // Check for the home screen being activated
    if (g_uMsgActivateHome == message)
    {
        OnActivateHome(psWLanInfo);
    }

    switch(message)
    {
    case WM_INITDIALOG:
        fRet = TRUE;
        break;

    case WM_COMMAND:
        fRet = OnCommand(psWLanInfo, wParam);
        break;

    case WM_ACTIVATE:
        OnActivate(wParam, (HWND)lParam, psWLanInfo);
        break;

    case WM_NOTIFY:
        fRet = OnNotify(psWLanInfo, wParam, lParam);
        break;

    case WM_DESTROY:
        OnDestroy(psWLanInfo);
        break;

    case WM_SIZE:
        RelayoutDlg(psWLanInfo);
        break;

    case WM_INITMENUPOPUP:
        OnMenuPopup(psWLanInfo, (HMENU) wParam, ListView_GetNextItem(psWLanInfo->hwndNetList, -1, LVNI_SELECTED));
        break;

    case WM_SHWZC_CALLBACK:
        psWLanInfo->dwHWFlags = SHWZCGetHardwareFlags();

        switch (wParam)
        {
        case SHWZC_EVENT_BROADCASTCHANGE:
        case SHWZC_EVENT_CONNECTING:
        case SHWZC_EVENT_CONNECTION:
            if (NULL == psWLanInfo->hwndNCPrsht)
            {
                RefreshNetListView(psWLanInfo);
            }
            break;
        }
        break;

    case WM_WARNING_NO_WIRELESS_CARD:
        if (psWLanInfo->hwndNCPrsht)
        {
            // IDOK actually is not save, this will save network settings while
            // unplug wireless network card. But IDCANCEL is refused by Network Property
            // property sheet. So we need to keep an eye on this one.
            PostMessage(psWLanInfo->hwndNCPrsht, WM_COMMAND, IDOK, 0);
        }
        break;
    }
    
    return fRet;
}

static UINT g_uiAutoOffTimer[IDS_WZC_AUTO_OFF_LAST-IDS_WZC_AUTO_OFF_FIRST+1] = {0, 30, 60, 300, 600, 1800};

HRESULT RefreshWifiAdvanceSettings(HWND hDlg)
{
    HRESULT hr = S_OK;
    HWND    hCombo;
    int     iRet;
    UINT    uiTime;
    BOOL    fNotification;

    hr = SHWZCGetNotificationState(&fNotification);
    CHR(hr);

    SendDlgItemMessage(hDlg, IDC_WZC_CHECK_NOTIFICATION, BM_SETCHECK, fNotification ? BST_CHECKED : BST_UNCHECKED, 0);

    hr = SHWZCGetWifiTimeoutPeriod(&uiTime);
    CHR(hr);

    hCombo = GetDlgItem(hDlg, IDC_WZC_COMBO_AUTO_OFF);
    CWR(hCombo);

    for (int i = 0; i <= (IDS_WZC_AUTO_OFF_LAST-IDS_WZC_AUTO_OFF_FIRST); i++)
    {
        iRet = SendMessage(hCombo, CB_INSERTSTRING, (WPARAM)-1, (LPARAM) LoadStringEtc(HINST_RESDLL, IDS_WZC_AUTO_OFF_FIRST+i, NULL));
        CBR(iRet != CB_ERR);

        if (uiTime == g_uiAutoOffTimer[i])
        {
            SendMessage(hCombo, CB_SETCURSEL, (WPARAM)iRet, (LPARAM)0);
        }

        CBR(CB_ERR != SendMessage(hCombo, CB_SETITEMDATA, (WPARAM)iRet, (LPARAM)g_uiAutoOffTimer[i]));
    }

Error:
    return hr;
}

HRESULT SaveWifiAdvancedSettings(HWND hDlg)
{
    HRESULT hr = S_OK;
    UINT    uiTime;
    INT     iSel;
    HWND    hCombo;
    BOOL    fNotification;

    fNotification = (BST_CHECKED == SendDlgItemMessage(hDlg, IDC_WZC_CHECK_NOTIFICATION, BM_GETCHECK, 0, 0));

    SHWZCSetNotificationState(fNotification);

    hCombo = GetDlgItem(hDlg, IDC_WZC_COMBO_AUTO_OFF);
    CWR(hCombo);

    iSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);

    uiTime = SendMessage(hCombo, CB_GETITEMDATA, iSel, 0);

    SHWZCSetWifiTimeoutPeriod(uiTime);

Error:
    return hr;
}

BOOL WZCWLanAdvancedDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	SHINITDLGINFO shidi = {0,};
	SHMENUBARINFO mbi = {0,};
	
    BOOL fRet = PropPageDialogHelper(hDlg, message, wParam, lParam);
    fRet = SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(IDD_WZC_WLAN_ADVANCED), hDlg, message, wParam, lParam);

    switch (message)
    {
    case WM_INITDIALOG:
		// Adding Scroll Bar support
		shidi.dwMask    = SHIDIM_FLAGS;
		shidi.hDlg      = hDlg;
		shidi.dwFlags   = SHIDIF_WANTSCROLLBAR | SHIDIF_SIZEDLG;

		SHInitDialog(&shidi);
		
		
		//Menubar
		mbi.cbSize = sizeof( SHMENUBARINFO );
		mbi.hwndParent = hDlg;
		mbi.dwFlags = SHCMBF_HMENU;
		mbi.nToolBarId = IDM_CANCEL_SAVE;
		mbi.hInstRes = HINST_RESDLL;
		SHCreateMenuBar( &mbi );

        RefreshWifiAdvanceSettings(hDlg);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            SaveWifiAdvancedSettings(hDlg);
            EndDialog(hDlg, 1);
            break;
        case ID_CANCEL:
            EndDialog(hDlg, 0);
            break;
        }
        break;
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_ACTIVE)
        {
            RefreshWifiAdvanceSettings(hDlg);
        }
        break;
    default:
        break;
    }

    return fRet;
}

///////////////////////////////////////////////////////////////////////////////////////////
///
