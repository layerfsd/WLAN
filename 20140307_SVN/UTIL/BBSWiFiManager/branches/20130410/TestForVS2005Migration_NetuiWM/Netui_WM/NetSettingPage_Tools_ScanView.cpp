// NetSettingPage_Tools_ScanView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Netui_WM.h"
#include "NetSettingPage_Tools_ScanView.h"



static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParam)
{
	// lParamSort contains a pointer to the list view control.
	// The lParam of an item is just its index.

	DWORD dwScore1 = 0;
	DWORD dwScore2 = 0;

	TCHAR tszTemp1[18] = {'\0',};
	TCHAR tszTemp2[18] = {'\0',};

	LPTSTR pStrBssid = (LPTSTR)lParam;

	PNDIS_WLAN_BSSID pWlanBssid1 = (PNDIS_WLAN_BSSID)lParam1;
	PNDIS_WLAN_BSSID pWlanBssid2 = (PNDIS_WLAN_BSSID)lParam2;


	wsprintf(tszTemp1, L"%02X:%02X:%02X:%02X:%02X:%02X", pWlanBssid1->MacAddress[0], pWlanBssid1->MacAddress[1], pWlanBssid1->MacAddress[2], pWlanBssid1->MacAddress[3], pWlanBssid1->MacAddress[4], pWlanBssid1->MacAddress[5] );
	wsprintf(tszTemp2, L"%02X:%02X:%02X:%02X:%02X:%02X", pWlanBssid2->MacAddress[0], pWlanBssid2->MacAddress[1], pWlanBssid2->MacAddress[2], pWlanBssid2->MacAddress[3], pWlanBssid2->MacAddress[4], pWlanBssid2->MacAddress[5] );


	if( _tcscmp(tszTemp1, pStrBssid) == 0 )
	{
		dwScore1++;
	}
	else if( _tcscmp(tszTemp2, pStrBssid) == 0 )
	{
		dwScore2++;
	}



	if( dwScore1 > dwScore2 )
	{
		return -1;
	}
	else if( dwScore1 <  dwScore2 )
	{
		return 1;
	}
	else
	{
		return strcmp( (const char*)pWlanBssid1->Ssid.Ssid, (const char*)pWlanBssid2->Ssid.Ssid);
	}
}


// CNetSettingPage_Tools_ScanView

IMPLEMENT_DYNCREATE(CNetSettingPage_Tools_ScanView, CFormView)

CNetSettingPage_Tools_ScanView::CNetSettingPage_Tools_ScanView()
	: CFormView(CNetSettingPage_Tools_ScanView::IDD)
{
	memset( &m_AdapterInfo, 0x0, sizeof(BBS_ADAPTER_INFO) );
	memset(m_tszBSSID, '\0', sizeof(m_tszBSSID));
}

CNetSettingPage_Tools_ScanView::~CNetSettingPage_Tools_ScanView()
{
}

void CNetSettingPage_Tools_ScanView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_APSCAN, m_listScanedAP);
	DDX_Control(pDX, IDC_COMBO_SCANNER_PERIOD, m_comboScanPeriod);
}

BEGIN_MESSAGE_MAP(CNetSettingPage_Tools_ScanView, CFormView)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_APSCAN, &CNetSettingPage_Tools_ScanView::OnNMCustomdrawListApscan)
	ON_WM_DESTROY()
	ON_CBN_SELENDOK(IDC_COMBO_SCANNER_PERIOD, &CNetSettingPage_Tools_ScanView::OnCbnSelendokComboScannerPeriod)
END_MESSAGE_MAP()


// CNetSettingPage_Tools_ScanView 진단입니다.

#ifdef _DEBUG
void CNetSettingPage_Tools_ScanView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetSettingPage_Tools_ScanView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNetSettingPage_Tools_ScanView 메시지 처리기입니다.

void CNetSettingPage_Tools_ScanView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// Initialize ListControl
	m_listScanedAP.DeleteAllItems();
	for (int i=0 ;i < m_listScanedAP.GetItemCount() ;i++)
	{
		m_listScanedAP.DeleteColumn(0);
	}

	m_dwScreenRate = (GetSystemMetrics(SM_CXSCREEN) == 480)? 2 : 1;

	// Set List Font
	m_listScanedAP.SetFont(NULL, FALSE);
	m_listfont.DeleteObject();
	m_listfont.CreateFont(9 * m_dwScreenRate, 0, 0, 0, FW_EXTRABOLD, FALSE /*italic*/, FALSE /*underline*/, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_COMPAT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Tahoma")) ;
	m_listScanedAP.SetFont(&m_listfont);


	// Set Column Width
	DWORD dwListColumnWidthArr[4] = {0,};

	CalculateListColumnWidth(dwListColumnWidthArr, sizeof(dwListColumnWidthArr) / sizeof(dwListColumnWidthArr[0]));

	m_listScanedAP.InsertColumn(0, _T("SSID"), LVCFMT_LEFT, dwListColumnWidthArr[0]);
	m_listScanedAP.InsertColumn(1, _T("MAC"), LVCFMT_LEFT, dwListColumnWidthArr[1]);
	m_listScanedAP.InsertColumn(2, _T("dbm"), LVCFMT_LEFT, dwListColumnWidthArr[2]);
	m_listScanedAP.InsertColumn(3, _T("CH"), LVCFMT_LEFT, dwListColumnWidthArr[3]);


	GetWlanDefaultInformation(&m_AdapterInfo);
	UpdateScanAPList();




	m_comboScanPeriod.AddString(_T("1"));
	m_comboScanPeriod.AddString(_T("3"));
	m_comboScanPeriod.AddString(_T("5"));
	m_comboScanPeriod.AddString(_T("10"));
	m_comboScanPeriod.AddString(_T("20"));
	m_comboScanPeriod.AddString(_T("30"));
	m_comboScanPeriod.AddString(_T("60"));

	m_comboScanPeriod.SetCurSel(1);


	SetTimer(TIMERID_REFRESH_SCANNER, DEFAULT_TIMER_REFRESH_INTERVAL, NULL);

}


void CNetSettingPage_Tools_ScanView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch( nIDEvent )
	{
	case TIMERID_REFRESH_SCANNER:
		// Update List
		UpdateScanAPList();
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

void CNetSettingPage_Tools_ScanView::OnNMCustomdrawListApscan(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we 
	// set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the prepaint stage for an item. Here's where we set the
		// item's text color. Our return value will tell Windows to draw the
		// item itself, but it will use the new color we set here.
		// We'll cycle the colors through red, green, and light blue.

		COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);
		COLORREF crBk = GetSysColor(COLOR_WINDOW);

		TCHAR tszMac[18] = L"";

		PNDIS_WLAN_BSSID pWlanBssid = (PNDIS_WLAN_BSSID)pLVCD->nmcd.lItemlParam;

		if( pWlanBssid->Rssi > -75 )
		{
			crText = RGB(0,0,200);
		}
		else
		{
			crText = RGB(255,0,0);
		}

		wsprintf(tszMac, L"%02X:%02X:%02X:%02X:%02X:%02X", pWlanBssid->MacAddress[0], pWlanBssid->MacAddress[1], pWlanBssid->MacAddress[2], pWlanBssid->MacAddress[3], pWlanBssid->MacAddress[4], pWlanBssid->MacAddress[5] );

		
		// 현재 BSSID랑 동일?
		if( _tcscmp(m_tszBSSID, tszMac) == 0 )
		{
			crBk = RGB(230, 230, 230);
		}
		
		// Store the color back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBk;

		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;
	}
}


void CNetSettingPage_Tools_ScanView::UpdateScanAPList()
{

	m_listScanedAP.DeleteAllItems();


	memset(m_tszBSSID, '\0', sizeof(m_tszBSSID));

	if( _tcslen(m_AdapterInfo.tszAdapterName) != 0 )
	{
		BYTE pucMacAddr[6] = {0,};
		NDIS_WLAN_BSSID ndisWlanBssid[128] = {0,};
		DWORD dwNumofItems = 0;


		
		if( !GetBSSIDInfo(m_AdapterInfo.tszAdapterName, pucMacAddr) )
		{
			// ERROR or Disconnected
			RETAILMSG(1, (TEXT("[NETUI] GetBSSIDInfo returned FALSE \r\n") ));
		}

		wsprintf(m_tszBSSID, L"%02X:%02X:%02X:%02X:%02X:%02X", pucMacAddr[0], pucMacAddr[1], pucMacAddr[2], pucMacAddr[3], pucMacAddr[4], pucMacAddr[5] );

		if( !GetBBSIDListInfo(m_AdapterInfo.tszAdapterName, ndisWlanBssid, &dwNumofItems) )
		{
			// ERROR or Disconnected
		}


		if( dwNumofItems > 0 )
		{
			for(DWORD dwIdx = 0; dwIdx<dwNumofItems; dwIdx++)
			{
				TCHAR tszTemp[256] = {0,};
				LVITEM lvi = {0,};
				int nItemIdx = 0;

				mbstowcs(tszTemp, (char*)ndisWlanBssid[dwIdx].Ssid.Ssid, ndisWlanBssid[dwIdx].Ssid.SsidLength);
				// Insert the item into the listview
				// SSID
				lvi.mask     = LVIF_TEXT | LVIF_PARAM;
				lvi.iItem    = dwIdx;
				lvi.iSubItem = 0;
				lvi.pszText  = tszTemp;
				lvi.lParam = (LPARAM)&ndisWlanBssid[dwIdx];

				nItemIdx = m_listScanedAP.InsertItem(&lvi);

				// MAC
				wsprintf(tszTemp, L"%02X:%02X:%02X:%02X:%02X:%02X", ndisWlanBssid[dwIdx].MacAddress[0], ndisWlanBssid[dwIdx].MacAddress[1], ndisWlanBssid[dwIdx].MacAddress[2], ndisWlanBssid[dwIdx].MacAddress[3], ndisWlanBssid[dwIdx].MacAddress[4], ndisWlanBssid[dwIdx].MacAddress[5] );
				lvi.mask     = LVIF_TEXT;
				lvi.iItem    = nItemIdx;
				lvi.iSubItem = 1;
				lvi.pszText  = tszTemp;

				m_listScanedAP.SetItem(&lvi);

				 
				// RSSI
				_itot(ndisWlanBssid[dwIdx].Rssi, tszTemp, 10);
				lvi.mask     = LVIF_TEXT;
				lvi.iItem    = nItemIdx;
				lvi.iSubItem = 2;
				lvi.pszText  = tszTemp;

				m_listScanedAP.SetItem(&lvi);

				// Channel
				_itot(ChannelFreq2Number(ndisWlanBssid[dwIdx].Configuration.DSConfig), tszTemp, 10);
				lvi.mask     = LVIF_TEXT;
				lvi.iItem    = nItemIdx;
				lvi.iSubItem = 3;
				lvi.pszText  = tszTemp;

				m_listScanedAP.SetItem(&lvi);

			}
			
			m_listScanedAP.SortItems(MyCompareProc, (LPARAM)m_tszBSSID);
		}
		RefreshBSSIDList(m_AdapterInfo.tszAdapterName);

		

	}
	else
	{
		GetWlanDefaultInformation(&m_AdapterInfo);
	}
}


// 리스트박스의 Column 너비를 지정
void CNetSettingPage_Tools_ScanView::CalculateListColumnWidth(DWORD* pdwListColumnWidthArr, DWORD dwNumofColumns)
{
    HFONT   hFont = NULL;
	HFONT	hFontOld = NULL;
	CFont	*pFont = NULL;
	CFont	*pFontOld = NULL;
	CSize	size;
	CDC*	pCDC = NULL;
	CString	strTemp = L"";

	
	for(DWORD dwIdx = 0; dwIdx<dwNumofColumns; dwIdx++)
	{
		pdwListColumnWidthArr[dwIdx] = 0;
	}

	

	pCDC = GetDC();

    // Get font from the list view
	pFont = m_listScanedAP.GetFont();

	pFontOld = pCDC->SelectObject(pFont);



	// SSID Coulumn Width
	strTemp = L"XXXXXXXXXXXX";
	size = pCDC->GetTextExtent(strTemp);
	pdwListColumnWidthArr[0] = size.cx;


	// MAC Coulumn Width
	if( m_dwScreenRate = 2 ) // VGA?;
	{
		strTemp = L"XX:XX:XX:XX:XX:XX:XX";
	}
	else	// QVGA?
	{
		strTemp = L"XX:XX:XX:XX:XX:XX";
	}
	size = pCDC->GetTextExtent(strTemp);
	pdwListColumnWidthArr[1] = size.cx;

	// RSSI Coulumn Width
	strTemp = L"XXXXX";
	size = pCDC->GetTextExtent(strTemp);
	pdwListColumnWidthArr[2] = size.cx;

	// Channel Coulumn Width
	strTemp = L"XXXX";
	size = pCDC->GetTextExtent(strTemp);
	pdwListColumnWidthArr[3] = size.cx;

	

	pCDC->SelectObject(pFontOld);


	for(DWORD dwIdx = 0; dwIdx<dwNumofColumns; dwIdx++)
	{
		pdwListColumnWidthArr[dwIdx] += (4 * m_dwScreenRate);
	}

	ReleaseDC(pCDC);

    return;
}

void CNetSettingPage_Tools_ScanView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	KillTimer(TIMERID_REFRESH_SCANNER);
}

void CNetSettingPage_Tools_ScanView::OnCbnSelendokComboScannerPeriod()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR tszTemp[32] = L"";
	DWORD dwPeriod = 0;

	m_comboScanPeriod.GetLBText(m_comboScanPeriod.GetCurSel(), tszTemp);

	RETAILMSG(1, (TEXT("[NETUI] tszTemp : %s \r\n"),tszTemp  ));


	dwPeriod = _ttoi(tszTemp);


	RETAILMSG(1, (TEXT("[NETUI] dwPeriod : %s \r\n"),dwPeriod  ));

	KillTimer(TIMERID_REFRESH_SCANNER);
	

	SetTimer(TIMERID_REFRESH_SCANNER, dwPeriod * 1000, NULL);


}
