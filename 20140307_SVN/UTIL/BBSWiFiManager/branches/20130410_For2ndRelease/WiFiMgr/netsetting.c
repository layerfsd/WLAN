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
// Filename:    netsetting.c
//
// Owner:        
//
// Purpose:     Network Adapter Settings dialogs
//
//==========================================================================*
//==========================================================================*
#include <windows.h>
#include <windowsx.h>
#include <wzcshell.h>
#include <types.h>
#include "netui.h"
#include <netuip.h>
#include <resource.h>
#include "wzcuip.h"
#include "aygshell.h"
#include <commctrl.h>
#include <wcommctl.h>
#include "wpcpriv.h"
#include "ehm.h"
#include "nuiouser.h"
#include "connmgr_priv.h"
#include <pprsht.h>

#define MINIPORT_NAME_LEN   256 // Miniport name is used as a reg key name and the maximum size of a key name is 255 characters
#define DISPLAY_NAME_LEN    256

#define ZONE_NETREG (ZONE_NETWORK|ZONE_REG)

#define RK_COMM                             TEXT("COMM")
#define RV_GROUP                            TEXT("Group")
#define REGVALUE_NDIS                  TEXT("NDIS")
#define RSK_LINKAGE                       TEXT("Linkage")
#define RV_DISPLAYNAME                TEXT("DisplayName")

BOOL g_fNetSettingChange;

HRESULT RegValueSZ(HKEY hKey, LPCTSTR szName, LPTSTR* ppszValue, DWORD cbLen)
{
    HRESULT hr = S_OK;
    DWORD   dwRet, cbBufLen;
    BOOL    fDelete = FALSE;
    
    CBR(hKey != NULL && ppszValue != NULL);

    cbBufLen = 0;
    dwRet = RegQueryValueEx(hKey, szName, NULL, NULL, NULL, &cbBufLen);
    CBR(dwRet == ERROR_SUCCESS);

    if (!cbLen)
    {
        *ppszValue = (LPTSTR)LocalAlloc(LPTR, cbBufLen);
        CPR(*ppszValue);
        cbLen = cbBufLen;
        // This flag is to only free space
        // allocated here if RegQuery fails below
        // We should not free space being passed in.
        fDelete = TRUE; 
    }
        
    dwRet = RegQueryValueEx(hKey, szName, NULL, NULL, (LPBYTE)*ppszValue, &cbLen);
    
    if (dwRet != ERROR_SUCCESS && !cbLen)
    {
        if(fDelete) 
        {
            LocalFree(*ppszValue);

            // Avoid a potential double free in the calling function
            *ppszValue = NULL;
        }
    }
    CBR(dwRet == ERROR_SUCCESS);
    
Error:
    return hr;
}

//
//Execute an NDIS IO control operation.
//
BOOL
DoNdisIOControl(DWORD dwCommand, LPVOID pInBuffer, DWORD cbInBuffer, LPVOID pOutBuffer, DWORD *pcbOutBuffer OPTIONAL)
{
    HANDLE  hNdis;
    BOOL    bResult = FALSE;
    DWORD   cbOutBuffer;

    hNdis = CreateFile(DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL, OPEN_ALWAYS, 0, NULL);

    if (hNdis != INVALID_HANDLE_VALUE)
    {
        cbOutBuffer = 0;
        if (pcbOutBuffer)
        {
            cbOutBuffer = *pcbOutBuffer;
        }

        bResult = DeviceIoControl(hNdis,
                                  dwCommand,
                                  pInBuffer,
                                  cbInBuffer,
                                  pOutBuffer,
                                  cbOutBuffer,
                                  &cbOutBuffer,
                                  NULL);

        if (pcbOutBuffer)
        {
            *pcbOutBuffer = cbOutBuffer;
        }

        CloseHandle(hNdis);
    }

    return bResult;
}


// Function is meant to be called in a loop:
//   - Caller passes in *phkeyRoot as NULL first time through the loop to open reg key
//   - Reg key is not re-opened as long as caller passes the same variable for phkeyRoot
//     for all subsequent iterations.
// This way we avoid opening registry key multiple times in a loop.
//
// returns -1 when done, 0 for skipped entry and 1 for good entry
INT GetAdapterEntry(HKEY* phkeyRoot, INT nIndex, LPTSTR pszDisplay, LPTSTR pszMiniport, BOOL fGetBaseInstance)
{
    HRESULT hr        = S_OK;
    LPTSTR  pszGroup  = NULL;
    DWORD   dwRet, dwLen;
    HKEY    hkeyChild = NULL, hkeyLinkage = NULL;
    
    if (*phkeyRoot == NULL)
    {
        if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, RK_COMM, 0, KEY_READ, phkeyRoot))
        {
            return -1;
        }
    }

    dwLen = MINIPORT_NAME_LEN;
    if (ERROR_SUCCESS != RegEnumKeyEx(*phkeyRoot, nIndex, pszMiniport, &dwLen, NULL, NULL, NULL, NULL))
    {
        // We reach the end
        return -1;
    }

    dwRet = RegOpenKeyEx(*phkeyRoot, pszMiniport, 0, KEY_READ, &hkeyChild);
    CBR(dwRet == ERROR_SUCCESS);

    hr = RegValueSZ(hkeyChild, RV_GROUP, &pszGroup, 0);
    CHR(hr);

    CBR(!_tcscmp(pszGroup, REGVALUE_NDIS));

    dwRet = RegOpenKeyEx(hkeyChild, RSK_LINKAGE, 0, KEY_READ, &hkeyLinkage);
    if (fGetBaseInstance)
    {
        // We want to find the registry for an adapter instance, 
        // it should have RSK_LINKAGE entry
        CBR(dwRet == ERROR_SUCCESS);
    }
    else
    {
        // We want to find the registry for an base adapter, 
        // it should NOT have RSK_LINKAGE entry
        CBR(dwRet != ERROR_SUCCESS);
    }

    hr = RegValueSZ(hkeyChild, RV_DISPLAYNAME, &pszDisplay, (DISPLAY_NAME_LEN * sizeof(TCHAR)));
    if (FAILED(hr))
    {
        _tcscpy(pszDisplay, pszMiniport);
    }

Error:
    if (hkeyChild)
    {
        RegCloseKey(hkeyChild);
    }
    if (hkeyLinkage)
    {
        RegCloseKey(hkeyLinkage);
    }
    if (pszGroup)
    {
        LocalFree(pszGroup);
    }
        
    return SUCCEEDED(hr) ? 1 : 0;
}

LPTSTR GetActiveAdapterName(LPCTSTR pszQueryDeviceName)
{
    HRESULT                 hr = E_FAIL;
    CHAR                    Buf[1024];
    DWORD                   i = 0, dwBytesWritten = 0, dwBufLength = sizeof(Buf);
    LPTSTR                  pszDeviceName = NULL;
    PNDISUIO_QUERY_BINDING  pQueryBinding;

    pQueryBinding = (PNDISUIO_QUERY_BINDING)Buf;

    for(pQueryBinding->BindingIndex = i; /*Nothing*/; pQueryBinding->BindingIndex = ++i)
    {
        // Get device name
        if (!DeviceIoControl(
                g_hNdisUio,
                IOCTL_NDISUIO_QUERY_BINDING,
                pQueryBinding,
                sizeof(NDISUIO_QUERY_BINDING),
                Buf,
                dwBufLength,
                &dwBytesWritten,
                NULL))
        {
            // There is no more device, break here.
            break;
        }

        CBR(pQueryBinding->DeviceNameOffset < sizeof(Buf));
        // Get Device name
        pszDeviceName = (PTCHAR)((PUCHAR)pQueryBinding + pQueryBinding->DeviceNameOffset);

        // Check if the active adapter match the specified device name
        if (pszQueryDeviceName != NULL)
        {
            // Check the active device name may has below format
            // 1. Same with pszQueryDeviceName
            // 2. <prefix> + "\" + pszQueryDeviceName
            //    For example, Passthru driver will insert "Pass\" at the head of binding adaper name
            INT nLen1 = _tcslen(pszQueryDeviceName);
            INT nLen2 = _tcslen(pszDeviceName);
            if (      nLen1 + 1 <  nLen2 
                   && pszDeviceName[nLen2 - nLen1 - 1] == TEXT('\\')
                   && !_tcsicmp(pszDeviceName + nLen2 - nLen1, pszQueryDeviceName)
                || !_tcsicmp(pszDeviceName, pszQueryDeviceName) )
            {
                hr = S_OK;
                break;
            }
        }
        else
        {
            // This is the device we want
            hr = S_OK;
            break;
        }
    }

Error:
    return SUCCEEDED(hr) ? StrDup(pszDeviceName) : NULL ;
}

static BOOL OnInitDialog(HWND hDlg)
{
    LV_COLUMN lvc = {0};
    TCHAR     pszDisplay [DISPLAY_NAME_LEN], pszMiniport[MINIPORT_NAME_LEN];
    HWND      hwndList = GetDlgItem(hDlg, IDC_NETUI_NETCPL0_ADAPTERLIST);
    RECT      rcList;
    DWORD     dwStyle;
    HKEY      hKey = NULL;
    INT       i, nIndex, nResult;
    SHMENUBARINFO mbi = {0};
	SHINITDLGINFO sidi = {0};

    GetClientRect(hwndList, &rcList);

    InitMetaNetworkCombo(GetDlgItem(hDlg, IDC_NETUI_COMBO_ADAPTER_FILTER),
        CM_GetWiredDestination());

    g_fNetSettingChange = FALSE;

    lvc.mask = LVCF_FMT | LVCF_WIDTH;
    lvc.fmt  = LVCFMT_LEFT;
    lvc.cx   = rcList.right - rcList.left;
    ListView_InsertColumn(hwndList, 0, &lvc);

    dwStyle = GetWindowLong(hwndList, GWL_STYLE);
    SetWindowLong(hwndList, GWL_STYLE, (dwStyle | LVS_SHAREIMAGELISTS));
    ListView_SetExtendedListViewStyleEx(hwndList,
                LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE,
                LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE);    

    for (hKey = NULL, i = nIndex = 0;  ;nIndex++)
    {
        nResult = GetAdapterEntry(&hKey, nIndex, pszDisplay, pszMiniport, FALSE);
        if (nResult > 0)
        {
            LV_ITEM lv    = {0};
            
            lv.mask       = LVIF_TEXT;
            lv.iItem      = i++;
            lv.pszText    = pszDisplay;
            lv.cchTextMax = _tcslen(pszDisplay) + 1;
            
            if (nIndex == 0)
            {
                lv.mask |= LVIF_STATE;
                lv.state = LVIS_SELECTED;
                lv.stateMask = LVIS_SELECTED;
            }
            
            ListView_InsertItem(hwndList, &lv);
        }
        else if (nResult < 0)
        {
            break;
        }    
    }
    ListView_SetItemState(hwndList, 0,
                          LVIS_SELECTED | LVIS_FOCUSED,
                          LVIS_SELECTED | LVIS_FOCUSED);
    SetFocus(hwndList);

    if (hKey)
    {
        RegCloseKey(hKey);
    }

	
	// Size the dialog: full screen.
	sidi.dwMask = SHIDIM_FLAGS;
	sidi.dwFlags = SHIDIF_SIZEDLG | SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN;
	sidi.hDlg = hDlg;
	SHInitDialog(&sidi);
	
    mbi.cbSize = sizeof(SHMENUBARINFO);
    mbi.hwndParent = hDlg;
    mbi.nToolBarId = IDR_WZC_SKBAR_EDIT;
    mbi.hInstRes = HINST_RESDLL;
    mbi.dwFlags = SHCMBF_HMENU | SHCMBF_HIDESIPBUTTON;
    
    SHCreateMenuBar(&mbi);
    SetWindowLong(hDlg, DWL_USER, (LONG)mbi.hwndMB);

    return TRUE;
}

void OnEditAdapter(HWND hDlg, int nItemSelected)
{
    LV_ITEM      lv    = {0};
    TCHAR        pszDisplay [DISPLAY_NAME_LEN], pszMiniport[MINIPORT_NAME_LEN], pszSelected[DISPLAY_NAME_LEN];
    HWND         hwndList = GetDlgItem(hDlg, IDC_NETUI_NETCPL0_ADAPTERLIST);
    HKEY         hKey = NULL;
    INT          nResult, nIndex;
    LPTSTR       pszActiveAdapter;
    BOOL         fDone = FALSE;
    int          i;
    
    if (nItemSelected == -1)
    {
        nItemSelected = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
    }

    lv.mask = LVIF_TEXT;
    lv.iItem      = nItemSelected;
    lv.pszText    = pszSelected;
    lv.cchTextMax = ARRAYSIZE(pszSelected);
    ListView_GetItem(hwndList, &lv);

    for (i = 0; !fDone && i < 2; i++)
    {
        // First try the instances of adapter, 
        // Then  try the base adapter.
        BOOL fAdapterInstance = i ? FALSE : TRUE;
        for (hKey = NULL, nIndex = 0; !fDone && nItemSelected >= 0; nIndex++)
        {
            // Look for adapter that matches display text of selected listview item
            nResult = GetAdapterEntry(&hKey, nIndex, pszDisplay, pszMiniport, fAdapterInstance);
            pszActiveAdapter = NULL;
            if (   nResult > 0
                && !_tcscmp(pszDisplay, pszSelected))
            {
                LPTSTR pszBind;
                pszActiveAdapter = GetActiveAdapterName(pszMiniport);
                if (fAdapterInstance && pszActiveAdapter == NULL)
                {
                    // Skip this adapter instance
                    continue;
                }
                pszBind = pszActiveAdapter ? pszActiveAdapter : pszMiniport;
                if (AdapterIPProperties(hDlg, pszBind))
                {
                    TCHAR multiSz[MINIPORT_NAME_LEN + 1];
                    HCURSOR hCursor = NULL;
                    
                    // because we building a multisz we need to leave room for the double null at the end so copy one less than the buffer.
                    StringCchCopy(multiSz, (sizeof(multiSz) / sizeof(TCHAR))-1, pszBind);
                    multiSz[_tcslen(multiSz)+1] = _T('\0'); // Multi sz needs an extra null
                    
                    hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
                    DoNdisIOControl(IOCTL_NDIS_REBIND_ADAPTER, multiSz,
                                  (_tcslen(multiSz)+2) * sizeof(TCHAR),
                                  NULL, NULL);
                    SetCursor(hCursor);
                    fDone = TRUE;
                }
                LocalFree(pszActiveAdapter);
            }
            else if (nResult < 0)
            {
                break;
            }
        }
        if (hKey)
        {
            RegCloseKey(hKey);
        }
    }
}

BOOL OnNotify(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
    BOOL    fRet = FALSE;
    LPNMHDR pnmh = (LPNMHDR) lParam;

    if (NULL == pnmh)
    {
        goto Error;
    }
    
    switch(LOWORD(wParam))
    {
    case IDC_NETUI_NETCPL0_ADAPTERLIST:
        {

            NM_LISTVIEW* pnmlv = (NM_LISTVIEW*)lParam;                
            int          nItemSelected = pnmlv->iItem;           
              
            switch(pnmh->code)
            {
            case LVN_ITEMACTIVATE:
                OnEditAdapter(hDlg, nItemSelected);
                fRet = TRUE;
                break;
            }
        }
        break;
        
    default:
        switch(pnmh->code)
        {
        case PSN_KILLACTIVE:
        case PSN_SETACTIVE:
            ShowWindow((HWND)GetWindowLong(hDlg, DWL_USER), (pnmh->code == PSN_SETACTIVE) ? SW_SHOW : SW_HIDE);
            SHSipPreference(hDlg, SIP_FORCEDOWN); 
            break;

        case PSN_APPLY:
            fRet = SendMessage(hDlg, WM_COMMAND, IDOK, 0L);
            break;
        }
        break;
        
    }

Error:
    return fRet;
}

BOOL OnOK(HWND hDlg)
{
    HRESULT hr = S_OK;
    
    if (g_fNetSettingChange)
    {
        int nIndex;

        DDX_Comb(hDlg, IDC_NETUI_COMBO_ADAPTER_FILTER, &nIndex, FALSE);
        CBRA(0 <= nIndex && (UINT)nIndex < GetMetaNetworkCount());

        // Update the destination for all wired network cards
        hr = CM_SetWiredDestination(nIndex);
        CHR(hr);
    }

Error:
    return TRUE;  // this just means we handled the command
}

BOOL OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
    BOOL bRet = FALSE;

    UNREFERENCED_PARAMETER(lParam);
    
    switch(LOWORD(wParam))
    {
    case IDC_NETUI_COMBO_ADAPTER_FILTER:
        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            g_fNetSettingChange = TRUE;
        }
        break;

    case IDOK:
        bRet = OnOK(hDlg);
        break;
        
    case IDM_WZC_EDIT:
        OnEditAdapter(hDlg, -1);
        break;

    default:
        break;
    }    

    return bRet;
}
    
BOOL NetCardAdaptersDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    BOOL  fRet = PropPageDialogHelper(hDlg, message, wParam, lParam);;
    fRet = SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_ADAPTERS), hDlg, message, wParam, lParam );
    
    switch(message)
    {
    case WM_INITDIALOG:
        fRet = OnInitDialog(hDlg);            
        break;
        
    case WM_NOTIFY: 
        fRet = OnNotify(hDlg, wParam, lParam);
        break;

    case WM_COMMAND:
        fRet = OnCommand(hDlg, wParam, lParam);
        break;
    }

    return fRet;
}

BOOL NetCardInformationDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	
    //SHRCMLDialogProc( HINST_RESDLL, MAKEINTRESOURCE(IDD_BBS_INFORMATION), hDlg, message, wParam, lParam );
    
    switch(message)
    {
    case WM_INITDIALOG:
        //fRet = OnInitDialog(hDlg);
		// Size the dialog: full screen.
		{
		SHINITDLGINFO sidi = {0};
		sidi.dwMask = SHIDIM_FLAGS;
		sidi.dwFlags = SHIDIF_SIZEDLG | SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN;
		sidi.hDlg = hDlg;
		SHInitDialog(&sidi);
		}
        break;
        
    case WM_NOTIFY: 
        //fRet = OnNotify(hDlg, wParam, lParam);
        break;

    case WM_COMMAND:
        //fRet = OnCommand(hDlg, wParam, lParam);
        break;
    }

    return TRUE;
}

int CALLBACK NetCardsSettingCallback( HWND hwndDlg, UINT uMsg, LPARAM lParam)
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


HRESULT WINAPI NetCardsSettingDialog1(LPCTSTR pszClass)
{
    HRESULT         hr        = S_OK;
    PROPSHEETPAGE   psp[3]    = {0};
    PROPSHEETHEADER psh       = {0};
    WLAN_INFO       sWLanInfo = {0};
    INT             cnPages	= 0;
	INT             nDefaultPages	= 0;
    HWND            hWndOwner = NULL;
    SHMENUBARINFO   cbi       = {0};
	INT				nIndex = 0;
    //
    // Create property sheet pages
    //
    SHInitExtraControls();

	RETAILMSG(1, (TEXT("[NETUI] NetCardsSettingDialog \r\n")));

	
	RETAILMSG(1, (TEXT("[NETUI] HINST_RESDLL : 0x%X \r\n"), HINST_RESDLL));


    {
        INITCOMMONCONTROLSEX    icce;

        icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icce.dwICC  = ICC_TOOLTIP_CLASSES|ICC_CAPEDIT_CLASS|ICC_LISTVIEW_CLASSES;

        // We need the common control functions
        if (!ValidCommCtrlFunc(InitCommonControlsEx))
        {
            DEBUGMSG (ZONE_ERROR|ZONE_NETWORK,
                      (TEXT("!AdapterIPProperties: Common Controls not loaded\r\n")));
            return FALSE;
        }


        // Initialize the common controls.
        DoCallCommCtrlFunc(InitCommonControlsEx)(&icce);
    }

	RETAILMSG(1, (TEXT("[NETUI] NetCardsSettingDialog1 \r\n")));

	RETAILMSG(1, (TEXT("[NETUI] IDS_NETUI_NETCARDS_SETTING_TITLE : %s \r\n"), (LPTSTR)LoadString(HINST_RESDLL, IDS_NETUI_NETCARDS_SETTING_TITLE, NULL, 0) ));


    // Don't want a NULL parent
    hWndOwner = SHCreateWorkerWindow((LPTSTR)pszClass,
                          NULL,
                          NULL,
                          WS_EX_NOACTIVATE,
                          WS_OVERLAPPED | WS_VISIBLE,
                          (LPTSTR)LoadString(HINST_RESDLL, IDS_NETUI_NETCARDS_SETTING_TITLE, NULL, 0),
                          0, 0);
    SetWindowPos(hWndOwner, NULL, -10, -10, 5, 5, SWP_NOZORDER | SWP_NOACTIVATE);

    // Create a blank menubar for the dialog
    cbi.cbSize = sizeof(SHMENUBARINFO);
    cbi.hwndParent = hWndOwner;
    cbi.dwFlags = SHCMBF_EMPTYBAR;

    SHCreateMenuBarInternal(&cbi);

	RETAILMSG(1, (TEXT("[NETUI] NetCardsSettingDialog2 \r\n")));
    

	
	// Default "Network adapters"
	psp[cnPages].pszTemplate	= MAKEINTRESOURCE(IDD_NETUI_ADAPTERS);
	psp[cnPages].pfnDlgProc  	= NetCardAdaptersDlgProc;
	psp[cnPages].pszTitle    	= (LPCTSTR)LoadString(HINST_RESDLL, IDS_NETUI_ADAPTERS_PAGE_TITLE, NULL, 0);	
	

	RETAILMSG(1, (TEXT("[NETUI] pszTitle %s, \r\n"), psp[cnPages].pszTitle));

	cnPages++;
	
    // Should we show the Wi-Fi UI?
    if (SHWZCF_HW_PRESENT & SHWZCGetHardwareFlags())
    {
        // "Wireless"
		psp[cnPages].pszTemplate	= MAKEINTRESOURCE(IDD_WZC_WLAN_NETWORK);
		psp[cnPages].pfnDlgProc		= WZCWLanNetworkDlgProc;
		psp[cnPages].pszTitle		= (LPCTSTR)LoadString(HINST_RESDLL, IDS_WZC_WLAN_NETWORK_PAGE_TITLE, NULL, 0);
		psp[cnPages].lParam			= (LPARAM)&sWLanInfo; 
		
		RETAILMSG(1, (TEXT("[NETUI] pszTemplate %s, \r\n"), psp[cnPages].pszTitle));
		RETAILMSG(1, (TEXT("[NETUI] pszTitle %s, \r\n"), psp[cnPages].pszTitle));

		nDefaultPages = cnPages;
		cnPages++;

    }
	
#if 0
	// "Information"
	// Information 탭 표시 여부를 확인하는 플래그가 필요함!
	if(1)
	{
	
		psp[cnPages].pszTemplate	= MAKEINTRESOURCE(IDD_BBS_INFORMATION);
		psp[cnPages].pfnDlgProc  	= NULL;
		psp[cnPages].pszTitle    	= L"Info";
		
		nDefaultPages = cnPages;
		
		cnPages++;
		
	}
#endif

	// For common
	for(nIndex=0; nIndex<cnPages; nIndex++)
	{
		psp[nIndex].dwSize = sizeof(PROPSHEETPAGE);
		psp[nIndex].hInstance = HINST_RESDLL;
		psp[nIndex].dwFlags = PSP_USETITLE;		
	}

	
    //
    // Create property sheet header   
    //
    psh.dwSize        = sizeof(PROPSHEETHEADER);
    psh.dwFlags       = PSH_PROPSHEETPAGE 
        | PSH_MAXIMIZE 
        | PSH_USECALLBACK 
        | PSH_USEICONID
        | PSH_NOAPPLYNOW;
    psh.hwndParent    = hWndOwner;
    psh.hInstance     = HINST_RESDLL;
    psh.pszIcon       = NULL;
    psh.pszCaption    = MAKEINTRESOURCE(IDS_NETUI_NETCARDS_SETTING_TITLE);
    psh.nPages        = cnPages;
    psh.nStartPage    = nDefaultPages;
    psh.ppsp          = psp;
    psh.pfnCallback   = NetCardsSettingCallback;    

    // Display the property sheet

    if (PropertySheet(&psh) == -1)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }

    DestroyWindow(hWndOwner);

    return hr;
}
