// NetSettingPage_Adapters.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Netui_WM.h"
#include "NetSettingPage_Adapters.h"
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
#include <shlwapi.h>


#define MINIPORT_NAME_LEN   256 // Miniport name is used as a reg key name and the maximum size of a key name is 255 characters
#define DISPLAY_NAME_LEN    256




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
    return SUCCEEDED(hr) ? _wcsdup(pszDeviceName) : NULL ;
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


// CNetSettingPage_Adapters 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNetSettingPage_Adapters, CPropertyPage)

CNetSettingPage_Adapters::CNetSettingPage_Adapters()
	: CPropertyPage(CNetSettingPage_Adapters::IDD)
{
	memset( &m_mbi, 0x0, sizeof(SHMENUBARINFO) );
}

CNetSettingPage_Adapters::~CNetSettingPage_Adapters()
{
}

void CNetSettingPage_Adapters::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNetSettingPage_Adapters, CPropertyPage)
END_MESSAGE_MAP()


// CNetSettingPage_Adapters 메시지 처리기입니다.

BOOL CNetSettingPage_Adapters::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	LV_COLUMN lvc = {0};
    TCHAR     pszDisplay [DISPLAY_NAME_LEN], pszMiniport[MINIPORT_NAME_LEN];
	HWND      hwndList = ::GetDlgItem(m_hWnd, IDC_NETUI_NETCPL0_ADAPTERLIST);
    RECT      rcList;
    DWORD     dwStyle;
    HKEY      hKey = NULL;
    INT       i, nIndex, nResult;
	SHINITDLGINFO sidi = {0};

	::GetClientRect(hwndList, &rcList);

	InitMetaNetworkCombo(::GetDlgItem(m_hWnd, IDC_NETUI_COMBO_ADAPTER_FILTER),
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
	::SetFocus(hwndList);

    if (hKey)
    {
        RegCloseKey(hKey);
    }

	
	// Size the dialog: full screen.
	sidi.dwMask = SHIDIM_FLAGS;
	sidi.dwFlags = SHIDIF_SIZEDLG | SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN;
	sidi.hDlg = m_hWnd;
	::SHInitDialog(&sidi);
		
    m_mbi.cbSize = sizeof(SHMENUBARINFO);
    m_mbi.hwndParent = m_hWnd;
    m_mbi.nToolBarId = IDR_WZC_SKBAR_EDIT;
    m_mbi.hInstRes = HINST_RESDLL;
    m_mbi.dwFlags = SHCMBF_HMENU | SHCMBF_HIDESIPBUTTON;
    
    SHCreateMenuBar(&m_mbi);
    //SetWindowLong(m_hWnd, DWL_USER, (LONG)mbi.hwndMB);
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CNetSettingPage_Adapters::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	 switch(LOWORD(wParam))
    {
    case IDC_NETUI_COMBO_ADAPTER_FILTER:
        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            g_fNetSettingChange = TRUE;
        }
        break;

    case IDOK:
    	if (g_fNetSettingChange)
		{
			int nIndex;

			DDX_Comb(m_hWnd, IDC_NETUI_COMBO_ADAPTER_FILTER, &nIndex, FALSE);
			// Update the destination for all wired network cards
			CM_SetWiredDestination(nIndex);
		}

        break;
        
    case IDM_WZC_EDIT:
        OnEditAdapter(m_hWnd, -1);
        break;

    default:
        break;
    }    

	return CPropertyPage::OnCommand(wParam, lParam);
}

BOOL CNetSettingPage_Adapters::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	LPNMHDR pnmh = (LPNMHDR) lParam;
  
    switch(LOWORD(wParam))
    {
    case IDC_NETUI_NETCPL0_ADAPTERLIST:
        {
            NM_LISTVIEW* pnmlv = (NM_LISTVIEW*)lParam;                
            int          nItemSelected = pnmlv->iItem;           
              
            switch(pnmh->code)
            {
            case LVN_ITEMACTIVATE:
                OnEditAdapter(m_hWnd, nItemSelected);
                break;
            }
        }
        break;
        
    default:
        switch(pnmh->code)
        {
        case PSN_KILLACTIVE:
        case PSN_SETACTIVE:
			//ShowWindow((pnmh->code == PSN_SETACTIVE) ? SW_SHOW : SW_HIDE);
            SHSipPreference(m_hWnd, SIP_FORCEDOWN); 
			::ShowWindow(m_mbi.hwndMB, (pnmh->code == PSN_SETACTIVE ? SW_SHOW : SW_HIDE));
            break;

        case PSN_APPLY:
			::SendMessage(m_hWnd, WM_COMMAND, IDOK, 0L);
            break;
        }
        break;
        
    }

	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}
