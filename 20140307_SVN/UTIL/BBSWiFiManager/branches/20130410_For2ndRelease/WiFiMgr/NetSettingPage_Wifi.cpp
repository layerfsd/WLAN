// NetSettingPage_Wifi.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Wifi.h"
#include <wzcmgr.h>
#include <BBS_WifiXml.h>

#define WM_SHWZC_CALLBACK (WM_APP)

#define SHWZCF_NET_CONNECT_MASK (SHWZCF_NET_CONNECTING | SHWZCF_NET_CONNECTED)


UINT g_uMsgActivateHome = 0;	// the "home activated" system message code


TCHAR *g_strAuthentication[] = {	\
				_T("Open"),			\
				_T("Shared"),		\
				_T("AutoSwitch"),	\
				_T("WPA"),			\
				_T("WPAPSK"),		\
				_T("WPANone"),		\
				_T("WPA2"),			\
				_T("WPA2PSK"),		\
				_T("Max"),			\
			};

TCHAR *g_strEncryptionType[] = {	\
				_T("Wep"),			\
				_T("Disabled"),		\
				_T("WepNoKeys"),	\
				_T("UnSupported"),	\
				_T("Tkip"),			\
				_T("TkipNoKeys"),	\
				_T("Aes"),			\
				_T("AesNoKeys"),	\
			};



static const TCHAR gc_szMenuNamespace[] = TEXT("WiFi");
static const TCHAR gc_szSofkeyExtensions[] = TEXT("Connections_Menu");
static const TCHAR gc_szContextExtensions[] = TEXT("Connections_ContextMenu");


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



// CNetSettingPage_Wifi 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNetSettingPage_Wifi, CPropertyPage)

CNetSettingPage_Wifi::CNetSettingPage_Wifi()
	: CPropertyPage(CNetSettingPage_Wifi::IDD)
{
	memset(&m_sWLanInfo, 0x0, sizeof(WLAN_INFO));
	m_bDefaultInitDone = FALSE;
}

CNetSettingPage_Wifi::~CNetSettingPage_Wifi()
{
}

void CNetSettingPage_Wifi::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WZC_LIST_NETWORKS, m_hCtrlNetList);
}


BEGIN_MESSAGE_MAP(CNetSettingPage_Wifi, CPropertyPage)
	ON_REGISTERED_MESSAGE(g_uMsgActivateHome, OnActivateHome)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_INITMENUPOPUP()
	ON_BN_CLICKED(IDC_BUTTON_RESCAN, &CNetSettingPage_Wifi::OnBnClickedButtonRescan)
END_MESSAGE_MAP()


// CNetSettingPage_Wifi 메시지 처리기입니다.

BOOL CNetSettingPage_Wifi::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemText(IDC_WZC_HEADING, L"Configure Wireless Network" );
	
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

	SHLoadMenuExtensions((IUnknown*)m_sWLanInfo.pNetworkObject, gc_szMenuNamespace, gc_szSofkeyExtensions, &m_sWLanInfo.hSoftKeyExt);





	// Initialize the dialog contents
	// Init ListView Control
	RECT		rc = {0,};
	HRESULT		hr  = S_OK;
	LV_COLUMN	lvc = {0};
	INT			nSSIDColumnWidth = 0;
	INT			nStatusColumnWidth = 0;
	INT			nAuthColumnWidth = 0;
	INT			nEncrColumnWidth = 0;

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
	m_sWLanInfo.hImgs = DPI_ImageList_LoadImage(HINST_RESDLL, MAKEINTRESOURCE(IDB_WZC_STATE), AM_NETUI_WZCSTATEICONS_CX, 0, CLR_NONE, IMAGE_BITMAP, 0);
	CPR(m_sWLanInfo.hImgs);
	
	ListView_SetImageList(m_hCtrlNetList.m_hWnd, m_sWLanInfo.hImgs, LVSIL_SMALL);

	//
	// Insert two columns network name and network state 
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
	lvc.fmt  = LVCFMT_LEFT;

	// Calculate each column width
	CalculateColumnWidth(&nSSIDColumnWidth, &nStatusColumnWidth, &nAuthColumnWidth, &nEncrColumnWidth);


	// Network name column
	lvc.cx   = nSSIDColumnWidth;
	m_hCtrlNetList.InsertColumn(0, &lvc);

	// Network state
	lvc.cx   = nStatusColumnWidth;
	m_hCtrlNetList.InsertColumn(1, &lvc);

	// Authentication
	lvc.cx   = nAuthColumnWidth;
	m_hCtrlNetList.InsertColumn(2, &lvc);

	// Encryption
	lvc.cx   = nEncrColumnWidth;
	m_hCtrlNetList.InsertColumn(3, &lvc);

	

	// Override notification UI
	hr = SetCallbackRegistration(TRUE, &m_sWLanInfo);
	CHR(hr);


	// Get the "home activate" message to ensure proper
	// notification behavior if the user leaves the CPL
	g_uMsgActivateHome = RegisterWindowMessage(MSG_ACTIVATE_HOME);
	CWR(0 != g_uMsgActivateHome);


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




void CNetSettingPage_Wifi::CalculateColumnWidth(INT *nSSIDWidth, INT *nStatusWidth, INT *nAuthWidth, INT *nEncrWidth)
{
	HRESULT hr;
    LPTSTR  pszStatus;
    SIZE    size;
    HFONT   hFont, hFontOld;
	HDC     hDC = ::GetDC(m_sWLanInfo.hDlg);
    INT     i;

	*nSSIDWidth = 0;
	*nStatusWidth = 0;
	*nAuthWidth = 0;
	*nEncrWidth = 0;

    // Get font from the list view
	hFont = (HFONT)::SendMessage(m_sWLanInfo.hwndNetList, WM_GETFONT, 0, 0);
    CWR(hFont);

    hFontOld = (HFONT)SelectObject(hDC, hFont);


	// SSID Coulumn Width
	TCHAR tszTempSSID[] = _T("XXXXXXXXXXXXXXXX"); // MAX LENGTH ? 16
	GetTextExtentExPoint(hDC, tszTempSSID, _tcslen(tszTempSSID), 0, NULL, NULL, &size);
	*nSSIDWidth = size.cx;



	// Status Coulumn Width
    for (i = IDS_WZC_STATUS_FIRST; i <= IDS_WZC_STATUS_LAST; i++)
    {
        pszStatus = (LPTSTR)LoadString(HINST_RESDLL, i, NULL, 0);
        if (pszStatus)
        {
            // Get the length of the text
            GetTextExtentExPoint(hDC, pszStatus, _tcslen(pszStatus), 0, NULL, NULL, &size);

            if (size.cx > *nStatusWidth)
            {
                *nStatusWidth = size.cx;
            }
        }
    }


	// Authentication Coulumn Width
	for (i = Ndis802_11AuthModeOpen; i <= Ndis802_11AuthModeMax; i++)
    {
		GetTextExtentExPoint(hDC, g_strAuthentication[i], _tcslen(g_strAuthentication[i]), 0, NULL, NULL, &size);    
        
		if (size.cx > *nAuthWidth)
        {
            *nAuthWidth = size.cx;
        }
    }


	// Encryption Coulumn Width
	for (i = Ndis802_11WEPEnabled; i <= Ndis802_11Encryption3KeyAbsent; i++)
    {
		GetTextExtentExPoint(hDC, g_strEncryptionType[i], _tcslen(g_strEncryptionType[i]), 0, NULL, NULL, &size);    
        
		if (size.cx > *nEncrWidth)
        {
            *nEncrWidth = size.cx;
        }
    }


	
    SelectObject(hDC, hFontOld);

	*nSSIDWidth += AM_NETUI_WZCLISTVIEWMARGIN_CX;
    *nStatusWidth += AM_NETUI_WZCLISTVIEWMARGIN_CX;
	*nAuthWidth += AM_NETUI_WZCLISTVIEWMARGIN_CX;
	*nEncrWidth += AM_NETUI_WZCLISTVIEWMARGIN_CX;

Error:
	::ReleaseDC(m_sWLanInfo.hDlg, hDC);
    return;
}





HRESULT CNetSettingPage_Wifi::OnActivateHome(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = S_OK;
    // We always unregister callbacks when the home screen is activated. This
    // prevents us from permanently blocking Wi-Fi popups if we were
    // deactivated by a child dilaog, and then the user went Home without
    // closing the dialog.
    hr = SetCallbackRegistration(FALSE, &m_sWLanInfo);
    CHR(hr);

Error:
    return hr;
}

HRESULT CNetSettingPage_Wifi::PopulateNetworkList()
{
	HRESULT hr = S_OK;
    LPCTSTR pszConnected, pszConnecting, pszAvailable, pszUnavailable, pszText;
    LV_ITEM lv = { 0 };
    UINT    i, cConfigs;
    BOOL    fNetAvailable = FALSE;
	BBS_SCAN_OPTION_INFO sScanOption = {0,};

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

		
		GetScanOpt(&sScanOption);

		if(sScanOption.dwScanThreShold == -1)
		{
			sScanOption.dwScanThreShold = DEFAULT_SCANOPT_SCANTHRESHOLD_VALUE;

			if(sScanOption.dwNetToAccess == -1)
			{
				HWND hwndSink = ::FindWindow( TEXT("SinkWirelessNetUI"), NULL );

				if(hwndSink != NULL)
				{
					DWORD dwFlags = (DWORD)::SendMessage(hwndSink, SHWZCM_GETSTATE, 0, 0);

					// Success
					dwFlags--;

					sScanOption.dwNetToAccess = dwFlags;
				}
				else
				{
					sScanOption.dwNetToAccess = DEFAULT_SCANOPT_NETTOACCESS_VALUE;
				}
			}

			SetScanOpt(sScanOption);
		}
		
		
		if( sScanOption.dwScanThreShold > pwzcnet->nRssi )
		{
			RETAILMSG(1, (TEXT("[NETUI] Skip! %s(%d) \r\n"), pwzcnet->szName, pwzcnet->nRssi));
			continue;
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

		// Authentication
        lv.mask     = LVIF_TEXT;
        lv.iItem    = nIndex;
        lv.iSubItem = 2;
        lv.pszText  = g_strAuthentication[pwzcnet->dwAuthentication];
        CBR( m_hCtrlNetList.SetItem(&lv) );

		// Encryption
        lv.mask     = LVIF_TEXT;
        lv.iItem    = nIndex;
        lv.iSubItem = 3;
        lv.pszText  = g_strEncryptionType[pwzcnet->dwEncryptionType];
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


LRESULT CNetSettingPage_Wifi::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	HRESULT hr = S_OK;

	switch(message)
	{
		case WM_ACTIVATE:
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
			// The configuration window has the same parent as m_sWLanInfo.hDlg.  
			else if (NULL == hwndPrevious || ::GetParent(hwndPrevious) != ::GetParent(m_sWLanInfo.hDlg)) 
			{
				hr = SetCallbackRegistration(FALSE, &m_sWLanInfo);
				CHR(hr);
			}
			break;


		}
		case WM_SHWZC_CALLBACK:
			m_sWLanInfo.dwHWFlags = SHWZCGetHardwareFlags();

			switch (wParam)
			{
			case SHWZC_EVENT_BROADCASTCHANGE:
			case SHWZC_EVENT_CONNECTING:
			case SHWZC_EVENT_CONNECTION:
				if (NULL == m_sWLanInfo.hwndNCPrsht)
				{
					RefreshNetListView();
				}
				break;
			}
			break;
		case WM_WARNING_NO_WIRELESS_CARD:
			if (m_sWLanInfo.hwndNCPrsht)
			{
				// IDOK actually is not save, this will save network settings while
				// unplug wireless network card. But IDCANCEL is refused by Network Property
				// property sheet. So we need to keep an eye on this one.
				::PostMessage(m_sWLanInfo.hwndNCPrsht, WM_COMMAND, IDOK, 0);
			}
			break;
	}

	return CPropertyPage::WindowProc(message, wParam, lParam);

Error:
		RETAILMSG(1, (TEXT("[NETUI] WM_ACTIVATE ERROR \r\n")));
		return hr;
}

BOOL CNetSettingPage_Wifi::OnCommand(WPARAM wParam, LPARAM lParam)
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

    case IDM_WZC_WIFI_POWERCMD:
        OnChangePowerState();
        break;

    case IDOK:
        // Since we're applying any changes when the dialog is deactivated, we don't need to apply here -- we'll get a deactivate right away.
        break;

    default:
        if (LOWORD(wParam) >= IDM_WZC_MENU_EXT_FIRST && LOWORD(wParam) <= IDM_WZC_MENU_EXT_LAST)
        {
            SHInvokeContextMenuCommand(m_sWLanInfo.hwndNetList, LOWORD(wParam), m_sWLanInfo.hContextExt ? m_sWLanInfo.hContextExt : m_sWLanInfo.hSoftKeyExt);
        }
    }

	return CPropertyPage::OnCommand(wParam, lParam);
}


HRESULT CNetSettingPage_Wifi::OnChangePowerState()
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


HRESULT CNetSettingPage_Wifi::OnAddNewSetting()
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


HRESULT CNetSettingPage_Wifi::OnConnect()
{
	HRESULT    hr = S_OK;
    PCSHWZCNET pwzcnet = (PCSHWZCNET)GetListItemLPARAM( m_hCtrlNetList.GetNextItem(-1, LVNI_SELECTED) );
    SHWZCNET   wzcnet;
    BOOL       fSecure;
	BBS_SCAN_OPTION_INFO sScanOpt = {0,};




	// LockSSID
	if( GetScanOpt(&sScanOpt) == TRUE)
	{
		if( _tcslen(sScanOpt.tszLockSSID) > 0 )
		{
			

			// tszLockSSID 가 걸려있고, 같지 않으면 에러로..
			if( _tcscmp(sScanOpt.tszLockSSID, pwzcnet->szName) != 0 )
			{
				TCHAR tszTemp[128] = L"";

				wsprintf(tszTemp, L"LockSSID '%s' ", sScanOpt.tszLockSSID);
				MessageBox(tszTemp, L"ERROR", MB_ICONERROR);
				goto Error;
			}
		}
	}



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


HRESULT CNetSettingPage_Wifi::ApplyProperties(const WLAN_CFG_PROPS *psProps)
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

void CNetSettingPage_Wifi::DeleteAllItems()
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


void CNetSettingPage_Wifi::RelayoutDlg()
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

HRESULT CNetSettingPage_Wifi::RefreshNetListView()
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


BOOL CNetSettingPage_Wifi::OnNetworkContextMenu(POINT pt, INT iItem)
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
               LoadStringEtc(HINST_RESDLL, IDS_WZC_WIFI_POWERON + ((m_sWLanInfo.dwHWFlags & SHWZCF_HW_ON) ? 1:0), NULL));

    SHLoadMenuExtensions((IUnknown*)m_sWLanInfo.pNetworkObject,
                         gc_szMenuNamespace,
                         gc_szContextExtensions,
                         &m_sWLanInfo.hContextExt);

    //TrackPopupMenu(hMenuTrack, TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, m_sWLanInfo.hDlg, NULL);
	SHTrackPopupMenu(hMenuTrack, TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, m_sWLanInfo.hDlg, NULL);

    SHFreeContextMenuExtensions(m_sWLanInfo.hContextExt);
    m_sWLanInfo.hContextExt = NULL;

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

HRESULT CNetSettingPage_Wifi::OnNetworkProperties()
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



HRESULT CNetSettingPage_Wifi::OnRemoveSetting()
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

LPARAM CNetSettingPage_Wifi::GetListItemLPARAM(INT nIndex)
{
    LVITEM lv = {0};
    BOOL   fRet;

	lv.iItem = nIndex;
    lv.mask  = LVIF_PARAM;
	fRet = m_hCtrlNetList.GetItem(&lv);

    return fRet ? lv.lParam : 0;
}


void CNetSettingPage_Wifi::OnDestroy()
{
	CPropertyPage::OnDestroy();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DeleteAllItems();

	
    if (NULL != m_sWLanInfo.hdsaNetworks)
    {
        DSA_Destroy(m_sWLanInfo.hdsaNetworks);
    }

	
    // Destroy the callback
    VHR(SetCallbackRegistration(FALSE, &m_sWLanInfo));


    // Free the image list
    if (m_sWLanInfo.hImgs)
    {
        ImageList_Destroy(m_sWLanInfo.hImgs);
    }


    SHFreeContextMenuExtensions(m_sWLanInfo.hSoftKeyExt);
	
	((IUnknown*)m_sWLanInfo.pNetworkObject)->Release();
	
}

void CNetSettingPage_Wifi::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	RelayoutDlg();
}

void CNetSettingPage_Wifi::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CPropertyPage::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	RETAILMSG(1, (TEXT("OnInitMenuPopup \r\n") ));
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	INT			iItem = m_hCtrlNetList.GetNextItem(-1, LVNI_SELECTED);
	HRESULT		hr = S_OK;
    PCSHWZCNET	pwzcnet;
    DWORD		dwState = 0, dwSize;
    BOOL		fEnableConnect = BOOLIFY(iItem);
    BOOL		fEnableRemove = BOOLIFY(iItem);
    BOOL		fEnableEdit = BOOLIFY(iItem);
    LPTSTR		pNetwork;

	RETAILMSG(1, (TEXT("[NETUI] OnInitMenuPopup iItem : %d \r\n"), iItem ));


	// "Turn Wi-Fi Off/On"
    pPopupMenu->RemoveMenu(IDM_WZC_WIFI_POWERCMD, MF_BYCOMMAND);
	pPopupMenu->InsertMenu((UINT)-1, MF_BYPOSITION|MF_STRING, IDM_WZC_WIFI_POWERCMD, LoadStringEtc(HINST_RESDLL, IDS_WZC_WIFI_POWERON + ((m_sWLanInfo.dwHWFlags & SHWZCF_HW_ON) ? 1:0), NULL));

    pwzcnet = (PCSHWZCNET)GetListItemLPARAM( iItem );
    CBR(pwzcnet != NULL);

    dwSize = ARRAYSIZE(pwzcnet->szName);
    pNetwork = (LPTSTR)LocalAlloc(NONZEROLPTR, dwSize * sizeof(TCHAR));
    CPR(pNetwork);
    StringCchCopy(pNetwork, dwSize, pwzcnet->szName);
    
    ((CNetworkObject*)(m_sWLanInfo.pNetworkObject))->SetData(pNetwork, CF_UNICODETEXT);
    SHQueryMenuExtensions(m_sWLanInfo.hContextExt ? m_sWLanInfo.hContextExt : m_sWLanInfo.hSoftKeyExt,
		pPopupMenu->m_hMenu, 0xFFFFFFFF, IDM_WZC_MENU_EXT_FIRST, IDM_WZC_MENU_EXT_LAST,
		CMF_NORMAL);

    // Get the current state
    SHWZCGetState(m_sWLanInfo.hshwzc, &dwState);

    // If the selected network is ad-hoc, but there are infrastructure networks
    // airing, we can't connect, since ad-hoc networks are always considered
    // connected, unless the hardware is limited to use ONLY ad-hoc networks.
    if ((SHWZCF_NET_ADHOC & pwzcnet->dwFlags) &&
        SHWZCF_ALLOW_ADHOC != (SHWZCF_ALLOW_MASK & dwState) &&
        m_sWLanInfo.fAPAir)
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
	pPopupMenu->EnableMenuItem(IDM_WZC_NETWORK_CONNECT, fEnableConnect ? MF_ENABLED : MF_GRAYED);
	pPopupMenu->EnableMenuItem(IDM_WZC_NETWORK_REMOVE, fEnableRemove ? MF_ENABLED : MF_GRAYED);
	pPopupMenu->EnableMenuItem(IDM_WZC_NETWORK_PROPERTIES, fEnableEdit ? MF_ENABLED : MF_GRAYED);
    return;

}

BOOL CNetSettingPage_Wifi::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	NM_LISTVIEW *pnmlv;
    LPNMHDR pnmh = (LPNMHDR) lParam; 
    BOOL    bRet = FALSE; 
    INT     nItemSelected ;
	HRESULT hr = S_OK;

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
			nItemSelected = m_hCtrlNetList.GetNextItem(-1, LVNI_SELECTED);
		}

		switch(pnmlv->hdr.code)
		{

// 길게 눌러 나오는 메뉴 및 클릭하여 연결창 띄우는 부분은 제거함
// Connect 메뉴 또는 메뉴버튼을 통해 설정할 수 있기 때문에, 다른 문제를 발생할 여지가 있으므로 제거
#if 0
		case GN_CONTEXTMENU:
			bRet = OnNetworkContextMenu( ((NMRGINFO *)lParam)->ptAction, nItemSelected );
			break;

		case LVN_ITEMACTIVATE:
			if (nItemSelected == 0)
			{
				// User hit the first item which is "Add new setting..." or "Search for Networks"
				if (m_sWLanInfo.dwHWFlags & SHWZCF_HW_ON)
				{
					CHR(OnAddNewSetting());
				}
				else
				{
					m_sWLanInfo.bUserInitiatedSearch = TRUE;
					CHR(OnChangePowerState());
				}
			}
			else if (nItemSelected > 0)
			{
				// User hit a wireless network, bring up the config properties dialog
				//SendMessage(psInfo->hDlg, WM_COMMAND, MAKEWPARAM(IDM_WZC_NETWORK_PROPERTIES, 0), 0L);
				OnNetworkProperties();
			}                       
			bRet = TRUE;
			break;
#endif
		case NM_RECOGNIZEGESTURE:
			{
				DWORD dwItem = ((NMRGINFO*)lParam)->dwItemSpec;

				if (0 == dwItem || (DWORD)-1 == dwItem || m_sWLanInfo.bConnecting)
				{
					// No context menu for "Add new URL" or 
					// if user hasn't clicked on an item or
					// We are connecting to a wireless network
					SetWindowLong(m_sWLanInfo.hDlg, DWL_MSGRESULT, TRUE);
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
						PCSHWZCNET pwzcnet = (PCSHWZCNET)GetListItemLPARAM(nItemSelected);
						ASSERT(pwzcnet);

						fEnable = !BOOLIFY(SHWZCF_NET_CONNECT_MASK & pwzcnet->dwFlags);
					}

					// Enable SK1 if selected item is a network and user isn't already connecting to it.
					SHEnableSoftkey(m_sWLanInfo.hwndNetListMB, 0, TRUE, fEnable);
				}
				break;
			}
		}

		break;
		

	default:        
		switch(pnmh->code)
		{
		case PSN_APPLY:
			bRet = ::SendMessage(m_sWLanInfo.hDlg, WM_COMMAND, IDOK, 0L);
			break;
		case PSN_KILLACTIVE:
			::ShowWindow(m_sWLanInfo.hwndNetListMB, SW_HIDE);
			break;
		case PSN_SETACTIVE:
			
			HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

			::ShowWindow(m_sWLanInfo.hwndNetListMB, SW_SHOW);
			RefreshNetListView();

			SetCursor(hOldCursor);

			break;
		}
		break;
	}



	return CPropertyPage::OnNotify(wParam, lParam, pResult);

Error:
	return bRet;
}


void CNetSettingPage_Wifi::OnBnClickedButtonRescan()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	GetDlgItem(IDC_BUTTON_RESCAN)->EnableWindow(FALSE);

	SHWZCRescanNetworks();
	RefreshNetListView();

	GetDlgItem(IDC_BUTTON_RESCAN)->EnableWindow(TRUE);

	SetCursor(hOldCursor);
}
