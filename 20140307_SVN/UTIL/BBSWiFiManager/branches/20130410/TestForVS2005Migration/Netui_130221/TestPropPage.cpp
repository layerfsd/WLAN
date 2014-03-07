// TestPropPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Netui_130221.h"
#include "TestPropPage.h"
#include <wzcmgr.h>

#define WM_SHWZC_CALLBACK (WM_APP)

#define SHWZCF_NET_CONNECT_MASK (SHWZCF_NET_CONNECTING | SHWZCF_NET_CONNECTED)

#define WZC_DEFAULT_SCAN_INTERVAL 60

UINT g_uMsgActivateHome = 0;	// the "home activated" system message code



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


// CTestPropPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTestPropPage, CPropertyPage)

CTestPropPage::CTestPropPage()
	: CPropertyPage(CTestPropPage::IDD)
{
	memset(&m_sWLanInfo, 0x0, sizeof(WLAN_INFO));
	m_bDefaultInitDone = FALSE;
}

CTestPropPage::~CTestPropPage()
{

}

void CTestPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WZC_LIST_NETWORKS, m_hCtrlNetList);
}


BEGIN_MESSAGE_MAP(CTestPropPage, CPropertyPage)
	ON_REGISTERED_MESSAGE(g_uMsgActivateHome, OnActivateHome)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTestPropPage 메시지 처리기입니다.

BOOL CTestPropPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// Caption
	TCHAR szCaption[128] = {'\0',};
    GetWindowText(szCaption, ARRAYSIZE(szCaption) );
    SetDlgItemText(IDC_WZC_HEADING, szCaption );


	
	m_sWLanInfo.hDlg = m_hWnd;
	



			// For MenuBar
		TBBUTTONINFO tbbi = {0};
		SHMENUBARINFO mbi = {0};

		mbi.cbSize = sizeof(SHMENUBARINFO);
		mbi.hwndParent = m_hWnd;
		mbi.nToolBarId = IDR_WZC_SKBAR_NETWORK;
		mbi.hInstRes = HINST_RESDLL;
		mbi.dwFlags = SHCMBF_HMENU | SHCMBF_HIDESIPBUTTON;

		SHCreateMenuBar(&mbi);
		m_sWLanInfo.hwndNetListMB = mbi.hwndMB;

		tbbi.cbSize = sizeof(TBBUTTONINFO);
		tbbi.dwMask = TBIF_COMMAND;
		tbbi.dwMask |= TBIF_BYINDEX;

		::SendMessage(mbi.hwndMB, TB_GETBUTTONINFO, 1, (LPARAM)&tbbi);
		::SendMessage(m_sWLanInfo.hwndNetListMB, SHCMBM_GETSUBMENU, 0, tbbi.idCommand);

		m_sWLanInfo.pNetworkObject = new CNetworkObject;
		VPR(m_sWLanInfo.pNetworkObject);
	
		SHLoadMenuExtensions((IUnknown*)m_sWLanInfo.pNetworkObject, TEXT("WiFi"), TEXT("Connections_Menu"), &m_sWLanInfo.hSoftKeyExt);





	// Initialize the dialog contents
	// Init ListView Control
	RECT		rc = {0,};
	HRESULT		hr  = S_OK;
	CImageList	ImageList;
	LV_COLUMN	lvc = {0};
	INT			nStatusColumnWidth = 0;

	m_sWLanInfo.hwndNetList = m_hCtrlNetList.m_hWnd;

	m_hCtrlNetList.GetClientRect(&rc);

	//
    // Change the Style and StyleEx    
	m_hCtrlNetList.SetExtendedStyle( m_hCtrlNetList.GetExtendedStyle() | LVS_SHAREIMAGELISTS );

    
	ListView_SetExtendedListViewStyleEx(m_hCtrlNetList.m_hWnd,
                                        LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE,
                                        LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE);

	//
    // Create state image lists
	//ImageList.Create(IDB_WZC_STATE, 16, 1, RGB(0, 128, 128));
    m_sWLanInfo.hImgs = DPI_ImageList_LoadImage(HINST_RESDLL, MAKEINTRESOURCE(IDB_WZC_STATE), AM_NETUI_WZCSTATEICONS_CX, 0, CLR_NONE, IMAGE_BITMAP, 0);
    CPR(m_sWLanInfo.hImgs);
	//m_hCtrlNetList.SetImageList(ImageList.m_hImageList, LVSIL_SMALL);
	ListView_SetImageList(m_hCtrlNetList.m_hWnd, m_sWLanInfo.hImgs, LVSIL_SMALL);

	//
    // Insert two columns network name and network state 
    lvc.mask = LVCF_FMT | LVCF_WIDTH;
    lvc.fmt  = LVCFMT_LEFT;

	//
    nStatusColumnWidth = CalculateStatusColumnWidth();
	

	 // Network name column
    lvc.cx   = rc.right - GetSystemMetrics(SM_CXVSCROLL) - nStatusColumnWidth;
    m_hCtrlNetList.InsertColumn(0, &lvc);
	    
    // Network state
    lvc.cx   = nStatusColumnWidth;
	m_hCtrlNetList.InsertColumn(1, &lvc);



	// Override notification UI
	hr = SetCallbackRegistration(TRUE, &m_sWLanInfo);
    CHR(hr);


	// Get the "home activate" message to ensure proper
    // notification behavior if the user leaves the CPL
    g_uMsgActivateHome = RegisterWindowMessage(MSG_ACTIVATE_HOME);
    CWR(0 != g_uMsgActivateHome);



	// Initialize the filter
    hr = InitFilterCombo();
    CHR(hr);

	m_sWLanInfo.dwHWFlags = SHWZCGetHardwareFlags();
	

	// Populate the network list
    hr = PopulateNetworkList();
    CHR(hr);

    // We assume that we have hardware if we're here
    m_sWLanInfo.fWirelessCardPresent = TRUE;
	
	ListView_SetItemState(m_sWLanInfo.hwndNetList, 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);



	RETAILMSG(1, (TEXT("[NETUI] WIRELESS_CARD_PRESEND : %d \r\n"), m_sWLanInfo.fWirelessCardPresent ));
	if (m_sWLanInfo.fWirelessCardPresent)
	{ // show listbox, hide textbox
		GetDlgItem(IDC_WZC_STATIC_NO_WIRELESS_CARD_PRESEND)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_WZC_LIST_NETWORKS)->ShowWindow(SW_HIDE);
	}

	RETAILMSG(1, (TEXT("[NETUI] OnInitDialog8 \r\n")));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.

Error:
	RETAILMSG(1, (TEXT("[NETUI] OnInitDialog ERROR \r\n")));
    return FALSE;
}

//==========================================================================*
// Function: CalculateStatusColumnWidth
//
// Purpose:  Calculate the with of state column
//           
// Arguments: 
//
// Return Values:
//     return the width of the column
//==========================================================================*
INT CTestPropPage::CalculateStatusColumnWidth()
{
	HRESULT hr;
    LPTSTR  pszStatus;
    SIZE    size;
    HFONT   hFont, hFontOld;
	HDC     hDC = ::GetDC(m_sWLanInfo.hDlg);
    INT     nWidth = 0, i;

    // Get font from the list view
	hFont = (HFONT)::SendMessage(m_sWLanInfo.hwndNetList, WM_GETFONT, 0, 0);
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
	::ReleaseDC(m_sWLanInfo.hDlg, hDC);
    return nWidth;
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
//==========================================================================*
HRESULT CTestPropPage::InitFilterCombo()
{
	HRESULT hr = S_OK;
    DWORD   dwState = SHWZCF_ALLOW_AP;
    INT     i, rgFilterList[] = { IDS_WZC_FILTER_ALL_AVAILABLE,
                                            IDS_WZC_FILTER_NON_ADHOC,
                                            IDS_WZC_FILTER_ADHOC                };

    // Populate the combo
    for (i = 0; i < ARRAYSIZE(rgFilterList); i++)
    {
        SendDlgItemMessage(IDC_WZC_COMBO_FILTER, CB_ADDSTRING, 0, (LPARAM)LoadString(HINST_RESDLL, rgFilterList[i], NULL, 0) );
    }

    // Get the current value, Deep Donw GetState uses "WZCQueryInterfaceEx" that always returns 2.. which is ALL 
    // Technically we should not depend on Platform Libraries on defaults.. This is only for PPC , 
    //  smart fone Calls SHWZCGetState(NULL,..) first time  . Please refer wificsp.cpp which is proper way of doing ..
    //  Simple OO advantage make code much better in wificsp.cpp.
    if( m_bDefaultInitDone ) 
    {
        hr = SHWZCGetState(m_sWLanInfo.hshwzc, &dwState);
        CHR(hr);
    }
    else
    {
        m_bDefaultInitDone=TRUE; 
    }

    // Select the item from the list
	
	switch(dwState & SHWZCF_ALLOW_MASK)
    {
    case SHWZCF_ALLOW_ALL:
        m_sWLanInfo.nNetFilter = WZC_FILTER_ALL_AVAILABLE;
        break;

    case SHWZCF_ALLOW_AP:
        m_sWLanInfo.nNetFilter = WZC_FILTER_ACCESS_POINT;
        break;

    case SHWZCF_ALLOW_ADHOC:
        m_sWLanInfo.nNetFilter = WZC_FILTER_ADHOC;
        break;

    default:
        ASSERT(FALSE);
        break;
    }            
    
    SendDlgItemMessage(IDC_WZC_COMBO_FILTER, CB_SETCURSEL, m_sWLanInfo.nNetFilter, 0L);


Error:
    return hr;

}




HRESULT CTestPropPage::OnActivateHome(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = S_OK;
	MessageBox(_T("OnActivateHome"), _T(""), MB_OK);
    // We always unregister callbacks when the home screen is activated. This
    // prevents us from permanently blocking Wi-Fi popups if we were
    // deactivated by a child dilaog, and then the user went Home without
    // closing the dialog.
    hr = SetCallbackRegistration(FALSE, &m_sWLanInfo);
    CHR(hr);

Error:
	MessageBox(_T("OnActivateHome"), _T("ERROR"), MB_OK);
    return hr;
}

HRESULT CTestPropPage::PopulateNetworkList()
{
	HRESULT hr = S_OK;
    LPCTSTR pszConnected, pszConnecting, pszAvailable, pszUnavailable, pszText;
    LV_ITEM lv = { 0 };
    UINT    i, cConfigs;
    BOOL    fNetAvailable = FALSE;

    CBREx(&m_sWLanInfo, E_INVALIDARG);

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
    pszText = (LPCTSTR)LoadString(HINST_RESDLL,
                                  (m_sWLanInfo.dwHWFlags & SHWZCF_HW_ON) ? IDS_WZC_ADD_NEW_SETTINGS : IDS_WZC_SEARCH_NETWORKS,
                                  NULL,
                                  0);
    CBRA(NULL != pszText);


    // Fill list view structure and insert item
    lv.mask    = LVIF_TEXT | LVIF_IMAGE;
    lv.pszText = (LPTSTR)pszText;
    lv.iImage  = -1;

    CBR( 0 <= m_hCtrlNetList.InsertItem(&lv) );
	


    // Get the network list
    hr = SHWZCGetAllNetworks(m_sWLanInfo.hshwzc, &m_sWLanInfo.hdsaNetworks);
    CHR(hr);

    // Are there any items?
    cConfigs = DSA_GetItemCount(m_sWLanInfo.hdsaNetworks);
    CBREx(0 != cConfigs, S_FALSE);
    ListView_SetItemCount(m_sWLanInfo.hwndNetList, cConfigs + 1);

    // Add the networks to the list
    for (i = 0; i != cConfigs; ++i)
    {
        PCSHWZCNET pwzcnet = (PSHWZCNET)DSA_GetItemPtr(m_sWLanInfo.hdsaNetworks, i);
        BOOL       fAdhoc = (SHWZCF_NET_ADHOC & pwzcnet->dwFlags);
        BOOL       fSecure;
        LPCTSTR    pszDesc;
        int        nIndex;
        BOOL       fUseSignalStrength = FALSE;

        // If this network is AP, mark it
        if (FALSE == fAdhoc)
        {
            m_sWLanInfo.fAPAir = TRUE;
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

		nIndex = m_hCtrlNetList.InsertItem(&lv);
        CBR(0 <= nIndex);

        // Set the state text
        lv.mask     = LVIF_TEXT;
        lv.iItem    = nIndex;
        lv.iSubItem = 1;
        lv.pszText  = (LPTSTR)pszDesc;

		
        CBR( m_hCtrlNetList.SetItem(&lv) );
    }

    // Sort the networks
    CBREx(m_hCtrlNetList.SortItems(CompareNetworks, NULL), E_OUTOFMEMORY);

Error:

    if(&m_sWLanInfo)
    {
        SetCursor(m_sWLanInfo.hOldCursor);
        m_sWLanInfo.hOldCursor = NULL;

        // We just turned power on, but didn't find any broadcasting networks
        if((m_sWLanInfo.bUserInitiatedSearch) && (!fNetAvailable) && (m_sWLanInfo.dwHWFlags & SHWZCF_HW_ON))
        {
            m_sWLanInfo.bUserInitiatedSearch = FALSE;
            SH_BOXEX sbex = {0};
            sbex.cbSize = sizeof(sbex);
            sbex.dwStyle = SHBEXF_CLOSE_ON_ACTIVATION_LOSS|SHBEXF_SUPRESS_DEFAULT_SOUND;
            sbex.nTimeOut = 0;
            sbex.hwndOwner = m_sWLanInfo.hDlg;
            sbex.sbextype = sbextTextBox;
            sbex.info.ti.pszText = LoadStringEtc(HINST_RESDLL, IDS_WZC_WIFI_SEARCH_ERROR, NULL);
            sbex.info.ti.bi.cButtons = 1;
            
            SHBoxEx(&sbex);
        }

        m_sWLanInfo.bUserInitiatedSearch = FALSE;
    }
    
    return hr;
}


LRESULT CTestPropPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	HRESULT hr = S_OK;

	if(message == WM_ACTIVATE)
	{
		BOOL fActivate = LOWORD(wParam);
		HWND hwndPrevious =  (HWND)lParam;


		m_sWLanInfo.bActivate = (fActivate != WA_INACTIVE);

		// The CPL should register for callbacks whenever
		// the user is working inside the wifi CPL.
		if(fActivate)
		{
			hr = SetCallbackRegistration(TRUE, &m_sWLanInfo);
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
		else if (NULL == hwndPrevious || ::GetParent(hwndPrevious) != ::GetParent(m_sWLanInfo.hDlg)) 
		{
			RETAILMSG(1, (TEXT("[NETUI] SetCallbackRegistration \r\n") ));
			
			delete m_sWLanInfo.pNetworkObject;

			hr = SetCallbackRegistration(FALSE, &m_sWLanInfo);
			CHR(hr);
		}


	}

	return CPropertyPage::WindowProc(message, wParam, lParam);

Error:
		RETAILMSG(1, (TEXT("[NETUI] WM_ACTIVATE ERROR \r\n")));
		return hr;
}

BOOL CTestPropPage::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

    switch(LOWORD(wParam))
    {
	case IDM_WZC_NETWORK_CONNECT:
		OnConnect();
		break;

    case IDS_WZC_ADD_NEW_SETTINGS:
        OnAddNewSetting();
        break;

    case IDM_WZC_NETWORK_REMOVE:
        OnRemoveSetting();
        break;

    case IDM_WZC_NETWORK_PROPERTIES:
        OnNetworkProperties();
        break;

    case IDM_WZC_ADVANCED:
        //DialogBox(HINST_RESDLL, MAKEINTRESOURCE(IDD_WZC_WLAN_ADVANCED), psWLanInfo->hDlg, WZCWLanAdvancedDlgProc);
        break;

    case IDC_WZC_COMBO_FILTER:
        if (HIWORD(wParam) == CBN_SELENDOK)
        {
            //OnChangeNetFilter(psWLanInfo);
        }
        break;

    case IDM_WZC_WIFI_POWERCMD:
        OnChangePowerState();
        break;

    case IDOK:
        // Since we're applying any changes when the dialog is deactivated, we
        // don't need to apply here -- we'll get a deactivate right away.
        break;

    default:
        if (LOWORD(wParam) >= IDM_WZC_MENU_EXT_FIRST && LOWORD(wParam) <= IDM_WZC_MENU_EXT_LAST)
        {
            SHInvokeContextMenuCommand(m_sWLanInfo.hwndNetList, LOWORD(wParam), m_sWLanInfo.hContextExt ? m_sWLanInfo.hContextExt : m_sWLanInfo.hSoftKeyExt);
        }
    }

	return CPropertyPage::OnCommand(wParam, lParam);
}


HRESULT CTestPropPage::OnChangePowerState()
{
    HRESULT hr = S_OK;
    
    CHR(SHWZCSetPowerState(!(m_sWLanInfo.dwHWFlags & SHWZCF_HW_ON)));

    if (!m_sWLanInfo.hOldCursor)
    {
        m_sWLanInfo.hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
    }

Error:
    return hr;
}


HRESULT CTestPropPage::OnAddNewSetting()
{
    HRESULT        hr      = S_OK;
    WLAN_CFG_PROPS sProps  = {0};
    LPTSTR         pszSSID = NULL;

    // Init    
    sProps.dwFlags = WZCDLG_PROPS_RWSSID | WZCDLG_PROPS_RWINFR;
    sProps.dwMask = SHWZCF_SETMASK_ALL;
    SHWZCCreateDefaultNetwork(&sProps.wzcnet);

    // Save the dialog info so we can check for duplicate names
    sProps.psWLanInfo = &m_sWLanInfo;

    hr = WZCCreateNetworkWLanPropsDlgs(m_sWLanInfo.hDlg, &sProps);
    CHR(hr);

    if (IDOK == sProps.nReturn)
    {
        hr = ApplyProperties(&sProps);
        CHR(hr);
    }

Error:
    LocalFree(sProps.pvEapData);
    return hr;
}


HRESULT CTestPropPage::OnConnect()
{
	HRESULT    hr = S_OK;
    PCSHWZCNET pwzcnet = (PCSHWZCNET)GetListItemLPARAM( m_hCtrlNetList.GetNextItem(-1, LVNI_SELECTED) );
    SHWZCNET   wzcnet;
    BOOL       fSecure;

    // The first item is "Add net settings...". It cannot be deleted.
    CBR(pwzcnet != NULL);
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
            CBR(OnNetworkProperties());
        }
        else
        {
            SHWZCSetNetwork(m_sWLanInfo.hshwzc, &wzcnet, NULL, NULL, 0, SHWZCF_SETMASK_ALL);
        }
    }

    // disable SK1 Connect while we're attempting to connect for this network
    SHEnableSoftkey(m_sWLanInfo.hwndNetListMB, 0, TRUE, FALSE);

    // Ensure that power is on. Ignore errors, this is a "Best-effort" attempt
    SHWZCSetPowerState(TRUE);
        
    // Now connect directly (we'll automatically get a refresh event)
    hr = SHWZCConnectNetwork(m_sWLanInfo.hshwzc, wzcnet.szName,
        BOOLIFY(SHWZCF_NET_ADHOC & wzcnet.dwFlags));
    CHR(hr);

Error:
    if(FAILED(hr))
    {
        // re-enable SK since something went wrong
        SHEnableSoftkey(m_sWLanInfo.hwndNetListMB, 0, TRUE, TRUE);
    }
    
    return hr;
}


HRESULT CTestPropPage::ApplyProperties(const WLAN_CFG_PROPS *psProps)
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
    hr = SHWZCSetNetwork(m_sWLanInfo.hshwzc, &psProps->wzcnet, psProps->pszKey,
        psProps->pvEapData, psProps->cbEapData, psProps->dwMask);
    CHR(hr);

    // Copy the GUID string
    CBR(S_FALSE == SHWZCEnumDestinations(EnumSetGUID, &fg));
    hr = SHWZCSetDestinationGUID(m_sWLanInfo.hshwzc, psProps->wzcnet.szName,
        BOOLIFY(SHWZCF_NET_ADHOC & psProps->wzcnet.dwFlags), szGUID);
    CHR(hr);

    // Refresh the view
    RefreshNetListView();

    dwHardwareState = SHWZCGetHardwareFlags();
    if (!(dwHardwareState & SHWZCF_HW_CONNECTED) && !(dwHardwareState & SHWZCF_HW_CONNECTING))
    {
        hr = SHWZCConnectNetwork(m_sWLanInfo.hshwzc, psProps->wzcnet.szName,
            BOOLIFY(SHWZCF_NET_ADHOC & psProps->wzcnet.dwFlags));
        CHR(hr);
    }        
Error:
    return hr;
}

void CTestPropPage::DeleteAllItems()
{
    // Clear the list view
	m_hCtrlNetList.DeleteAllItems();
    m_sWLanInfo.fAPAir = FALSE;

    // Clear the network list
    if (NULL != m_sWLanInfo.hdsaNetworks)
    {
        // The alternative here is to call DSA_DeleteAllItems, but this saves
        // our poor little heap, since most likely we'll have approximately the
        // same number of networks in our next query
        m_sWLanInfo.hdsaNetworks->cItem = 0;
    }
}


void CTestPropPage::RelayoutDlg()
{
    if (NULL != &m_sWLanInfo)
    {
        CWnd *phStatic = GetDlgItem(IDC_WZC_STATIC_NO_WIRELESS_CARD_PRESEND);
        BOOL fShowList  = m_sWLanInfo.fWirelessCardPresent;
        RECT rect;

		m_hCtrlNetList.GetWindowRect(&rect);
        
		MapWindowPoints(NULL, (LPPOINT)&rect, 2 );

		phStatic->SetWindowPos(NULL, rect.left, rect.top, RECTWIDTH(rect), fShowList ? 10 : RECTHEIGHT(rect), (fShowList ? SWP_HIDEWINDOW : SWP_SHOWWINDOW) | SWP_NOZORDER); 
		
        // In order to let RCML resize wireless network listview, it just hide
        // it behind the static text when wireless card is not present
		m_hCtrlNetList.SetWindowPos(phStatic, 0, 0, 0, 0, (fShowList ? SWP_NOZORDER : 0) | SWP_NOMOVE | SWP_NOSIZE);

        // When the rectangle surrounding the listview is moved, our text get's 
        // clipped.  Plus when we setwindowpos ourself and change our width
        // we aren't invalidating ourself, so we don't clear out old text.
		phStatic->InvalidateRect(NULL, FALSE);

        // Don't want network list to get input if it hides below the static text
		m_hCtrlNetList.EnableWindow(fShowList);
    }
}

HRESULT CTestPropPage::RefreshNetListView()
{
    HRESULT hr = S_OK;
    CNetID nidOrig;

    // Save the selected mark if one exists.  We may be doing a refresh while
    // the user has brought up the ContextMenu but has not yet made a selection.
    INT nItemSelected = ListView_GetNextItem(m_sWLanInfo.hwndNetList, -1, LVNI_SELECTED);

    if (nItemSelected >= 1)
    {
        PCSHWZCNET pwzcnet = (PCSHWZCNET)GetListItemLPARAM( m_hCtrlNetList.GetNextItem(-1, LVNI_SELECTED) );
        nidOrig.Assign(pwzcnet);
    }

	m_hCtrlNetList.SetRedraw(FALSE);

    DeleteAllItems();

    // If wireless card is not present, we want to hide wireless network list
    RelayoutDlg();
               
    if (m_sWLanInfo.fWirelessCardPresent)
    {
        hr = PopulateNetworkList();
        CHR(hr);

        nItemSelected = 0;
        // Now set the selection mark to the matching network, if it still exists in the list.
        // It isn't enough to do a ListView_FindItem for pwzcnet, the refresh might have actually
        // caused a different network configuration to be stored at that address!
        if (nidOrig.IsValid())
        {
            INT nItems = ListView_GetItemCount(m_sWLanInfo.hwndNetList);
            INT i;
            CNetID  nidTemp;
            // Don't bother with index 0, it is "Add New..."
            for (i = 1; i < nItems; i++)
            {
                PCSHWZCNET pwzcnetTemp = (PCSHWZCNET) GetListItemLPARAM( i );
                nidTemp.Assign(pwzcnetTemp);
                if (0 == nidOrig.Compare(&nidTemp, FALSE))
                {
                    nItemSelected = i;
                    break;
                }
            }
        }

        ListView_SetItemState(m_sWLanInfo.hwndNetList, nItemSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
    }

Error:
	::SendMessage(m_sWLanInfo.hwndNetList, WM_SETREDRAW, (WPARAM)TRUE, 0L);
	::RedrawWindow(m_sWLanInfo.hwndNetList, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);

    return hr;
}


HRESULT CTestPropPage::OnNetworkProperties()
{
	HRESULT        hr = S_OK;
    WLAN_CFG_PROPS sProps = {0};
    PCSHWZCNET     pwzcnet = (PCSHWZCNET)GetListItemLPARAM( m_hCtrlNetList.GetNextItem(-1, LVNI_SELECTED) );
    BOOL           fAdhoc = FALSE;
	TCHAR          szGUID[MAX_PATH] = {'\0',};
    FINDGUID       fg;

    CBR(pwzcnet != NULL);

    // If the selected network is a "Permanent" non-editable network, exit without 
    // showing the NetworkProperties pages
    fAdhoc = BOOLIFY(SHWZCF_NET_ADHOC & pwzcnet->dwFlags);
    CBREx (!SHWZCIsPermanentNetwork(pwzcnet->szName, fAdhoc), S_FALSE);

    // Get the current EAP data
    hr = SHWZCGetNetwork(m_sWLanInfo.hshwzc, pwzcnet->szName, fAdhoc,
        &sProps.wzcnet, &sProps.pvEapData, &sProps.cbEapData);
    CHR(hr);

    // We don't care about the result, since if this fails we just won't match
    // with any networks and we'll use the default
    szGUID[0] = 0;
    SHWZCGetDestinationGUID(m_sWLanInfo.hshwzc, pwzcnet->szName, fAdhoc,
        szGUID, ARRAYSIZE(szGUID));

    fg.pszGUID = szGUID;
    fg.uIndex = 0;
    if (S_FALSE == SHWZCEnumDestinations(EnumFindGUID, &fg))
    {
        sProps.uMeta = fg.uIndex;
    }

    sProps.psWLanInfo = &m_sWLanInfo;
    
    hr = WZCCreateNetworkWLanPropsDlgs(m_sWLanInfo.hDlg, &sProps);
    CHR(hr);

    if (IDOK == sProps.nReturn)
    {
        hr = ApplyProperties(&sProps);
        CHR(hr); 
    }

Error:
    LocalFree(sProps.pvEapData);
    return SUCCEEDED(hr);
}



HRESULT CTestPropPage::OnRemoveSetting()
{
	HRESULT    hr = S_OK;
    PCSHWZCNET pwzcnet =(PCSHWZCNET)GetListItemLPARAM( m_hCtrlNetList.GetNextItem(-1, LVNI_SELECTED) );

    // The first item is "Add net settings...". It cannot be deleted.
    CBR(pwzcnet != NULL);

    // Delete the network
    hr = SHWZCDeleteNetwork(m_sWLanInfo.hshwzc, pwzcnet->szName, BOOLIFY(SHWZCF_NET_ADHOC & pwzcnet->dwFlags));
    CHR(hr);

    // Refresh list view
    RefreshNetListView();

Error:
    return hr;
}

LPARAM CTestPropPage::GetListItemLPARAM(INT nIndex)
{
    LVITEM lv = {0};
    BOOL   fRet;

	lv.iItem = nIndex;
    lv.mask  = LVIF_PARAM;
	fRet = m_hCtrlNetList.GetItem(&lv);

    return fRet ? lv.lParam : 0;
}

void CTestPropPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CPropertyPage::OnPaint()을(를) 호출하지 마십시오.
	RETAILMSG(1, (TEXT("[NETUI] OnPaint \r\n") ));
}

