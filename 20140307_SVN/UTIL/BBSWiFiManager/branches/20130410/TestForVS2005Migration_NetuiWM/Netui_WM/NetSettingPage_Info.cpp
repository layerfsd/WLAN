// NetSettingPage_Info.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Netui_WM.h"
#include "NetSettingPage_Info.h"


// CNetSettingPage_Info 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNetSettingPage_Info, CPropertyPage)

CNetSettingPage_Info::CNetSettingPage_Info()
	: CPropertyPage(CNetSettingPage_Info::IDD)
{
	memset( &m_AdapterInfo, 0x0, sizeof(BBS_ADAPTER_INFO) );
	memset( &m_CurNetInfo, 0x0, sizeof(BBS_CUR_NET_INFO) );
}

CNetSettingPage_Info::~CNetSettingPage_Info()
{
}

void CNetSettingPage_Info::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNetSettingPage_Info, CPropertyPage)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CNetSettingPage_Info 메시지 처리기입니다.

BOOL CNetSettingPage_Info::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	// This dialog needs a scrollbar

	TCHAR tszFileVersion[128] = L"";

    SHINITDLGINFO shidi;
    shidi.dwMask = SHIDIM_FLAGS;
	shidi.hDlg = m_hWnd;
    shidi.dwFlags = SHIDIF_WANTSCROLLBAR | SHIDIF_SIZEDLGFULLSCREEN;
	::SHInitDialog(&shidi);


	ZeroMemory( &m_mbi, sizeof( m_mbi ) );
	m_mbi.cbSize = sizeof( SHMENUBARINFO );
	m_mbi.hwndParent = m_hWnd;
	m_mbi.hInstRes = AfxGetInstanceHandle();
	SHCreateMenuBar( &m_mbi );


	UpdateAdpaterIPInfo(&m_AdapterInfo);
	UpdateAdpaterNetworkInfo(&m_CurNetInfo);

	if( GetCurrentFileVersion(tszFileVersion) )
	{
		SetDlgItemText(IDC_STATIC_WIFIMGR_VERSION, tszFileVersion);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_WIFIMGR_VERSION, L"Unknown Version");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CNetSettingPage_Info::UpdateAdpaterIPInfo(PBBS_ADAPTER_INFO pIPInfo)
{
	CString strAdpater = _T("");

	memset( pIPInfo, 0x0, sizeof(BBS_ADAPTER_INFO) );
	GetWlanDefaultInformation(pIPInfo);


	//
	// Update Controls
	//

	strAdpater = pIPInfo->tszAdapterName;
	strAdpater += _T(" - ");
	strAdpater += pIPInfo->tszMacAddr;

	// Adapter Name / MacAddr
	SetDlgItemText(IDC_STATIC_ADAPTERNAME, strAdpater);
	
	// IP Information
	SetDlgItemText(IDC_STATIC_IPADDRESS, pIPInfo->tszIPAddr);
	SetDlgItemText(IDC_STATIC_SUBNETMASK, pIPInfo->tszSubNetMask);

	if( _tcscmp(pIPInfo->tszIPAddr, _T("0.0.0.0")) == 0 )
	{
		SetDlgItemText(IDC_STATIC_GATEWAY, _T("0.0.0.0") );
		SetDlgItemText(IDC_STATIC_DNS_SERVER, _T("0.0.0.0") );
		SetDlgItemText(IDC_STATIC_DHCP_ENABLED, _T("-") );
	}
	else
	{
		SetDlgItemText(IDC_STATIC_GATEWAY, pIPInfo->tszDefaultGateWay);
		SetDlgItemText(IDC_STATIC_DNS_SERVER, pIPInfo->tszDnsAddr);
		SetDlgItemText(IDC_STATIC_DHCP_ENABLED, (pIPInfo->bDHCPEnabled)? _T("Y") : _T("N") );
	}
	

	return TRUE;
}






BOOL CNetSettingPage_Info::UpdateAdpaterNetworkInfo(PBBS_CUR_NET_INFO pNetworkInfo)
{
	CString strAuthPriv = _T("");
	CString strNetworkType = _T("");
	CString strChannel = _T("");

	memset( pNetworkInfo, 0x0, sizeof(BBS_CUR_NET_INFO) );

	if( _tcslen(m_AdapterInfo.tszAdapterName) != 0 )
	{
		GetCurrentNetworkInfo( m_AdapterInfo.tszAdapterName, pNetworkInfo );
	}

	//
	// Update Controls (Default Information)
	//
	
	// SSID 이름이 있을 경우에만 정보 표시
	if( _tcslen(pNetworkInfo->tszSSID) != 0)
	{
		// SSID
		SetDlgItemText(IDC_STATIC_SSID, pNetworkInfo->tszSSID);

		// BSSID(MAC)
		SetDlgItemText(IDC_STATIC_BSSID, pNetworkInfo->tszBSSID);

		// Authentication / Privacy(Encrytion)
		strAuthPriv = pNetworkInfo->tszAuth;
		strAuthPriv += _T(" / ");
		strAuthPriv += pNetworkInfo->tszEncr;
		SetDlgItemText(IDC_STATIC_AUTH_PRIV, strAuthPriv);

		// Infrastructure
		SetDlgItemText(IDC_STATIC_INFRA, pNetworkInfo->tszInfra);
		
		// RSSI
		SetDlgItemInt(IDC_STATIC_RSSI, pNetworkInfo->wzcConfig.Rssi, TRUE);
		
		// NetworkTypeInUse
		switch( pNetworkInfo->wzcConfig.NetworkTypeInUse )
		{
			case Ndis802_11FH:
				strNetworkType = L"Ndis802_11FH";
				break;
			case Ndis802_11DS:
				strNetworkType = L"Ndis802_11DS";
				break;
			case Ndis802_11OFDM5:
				strNetworkType = L"Ndis802_11OFDM5";
				break;
			case Ndis802_11OFDM24:
				strNetworkType = L"Ndis802_11OFDM24";
				break;
			case Ndis802_11Automode:
				strNetworkType = L"Ndis802_11Automode";
				break;
		}
		SetDlgItemText(IDC_STATIC_NETWORK_TYPE, strNetworkType);

		// Channel (DSConfig)
		strChannel.Format(_T("%dkHz (ch-%d)"), pNetworkInfo->wzcConfig.Configuration.DSConfig, ChannelFreq2Number(pNetworkInfo->wzcConfig.Configuration.DSConfig) );
		SetDlgItemText(IDC_STATIC_CHANNEL, strChannel);
	}
	else
	{
		// SSID
		SetDlgItemText(IDC_STATIC_SSID, _T("-") );

		// BSSID(MAC)
		SetDlgItemText(IDC_STATIC_BSSID, _T("-") );

		// Authentication / Privacy(Encrytion)
		SetDlgItemText(IDC_STATIC_AUTH_PRIV, _T("-") );

		// Infrastructure
		SetDlgItemText(IDC_STATIC_INFRA, _T("-") );

		// RSSI
		SetDlgItemText(IDC_STATIC_RSSI, _T("") );

		// NetworkTypeInUse
		SetDlgItemText(IDC_STATIC_NETWORK_TYPE, _T("-") );


		// Channel (DSConfig)
		SetDlgItemText(IDC_STATIC_CHANNEL, _T("-") );


	}

	return TRUE;
}


BOOL CNetSettingPage_Info::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	LPNMHDR pnmh = (LPNMHDR) lParam; 

	switch(pnmh->code)
    {
        case PSN_KILLACTIVE:
			::ShowWindow(m_mbi.hwndMB, SW_HIDE);
			KillTimer(UPDATE_TIMERID_IP_INFORMATION);
			KillTimer(UPDATE_TIMERID_NETWORK_INFORMATION);
			break;
        case PSN_SETACTIVE:
			::ShowWindow(m_mbi.hwndMB, SW_SHOW);
			SetTimer(UPDATE_TIMERID_IP_INFORMATION, TIMER_INTERVAL_IP_INFORMATION, NULL);
			SetTimer(UPDATE_TIMERID_NETWORK_INFORMATION, TIMER_INTERVAL_NETWORK_INFORMATION, NULL);
            break;
    }
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void CNetSettingPage_Info::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	switch( nIDEvent )
	{
	case UPDATE_TIMERID_IP_INFORMATION:
		UpdateAdpaterIPInfo(&m_AdapterInfo);
		break;
	case UPDATE_TIMERID_NETWORK_INFORMATION:
		UpdateAdpaterNetworkInfo(&m_CurNetInfo);
		break;
	}

	CPropertyPage::OnTimer(nIDEvent);
}

HBRUSH CNetSettingPage_Info::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_RSSI)
	{
		if( GetDlgItemInt(IDC_STATIC_RSSI) < -75 )
		{
			pDC->SetTextColor(RGB(250, 0, 0));
		}
		else
		{
			pDC->SetTextColor(RGB(0, 200, 0));
		}
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL CNetSettingPage_Info::GetCurrentFileVersion(LPTSTR tszFileVer)
{
	BOOL bRet = FALSE;
	DWORD dwInfoSize = 0;
	TCHAR tszFilePath[MAX_PATH] = L"windows\\netui.dll";

	char *pBuf = NULL;

	tszFileVer[0] = '\0';

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Info::GetCurrentFileVersion()  tszFilePath : %s \r\n"), tszFilePath ));

	dwInfoSize = GetFileVersionInfoSize(tszFilePath, 0);

	pBuf = new char[dwInfoSize];

	if( dwInfoSize == 0 )
	{
		bRet = FALSE;
	}

	if(GetFileVersionInfo(tszFilePath, 0, dwInfoSize, pBuf) !=0 )
	{
		VS_FIXEDFILEINFO* pFineInfo = NULL;
		UINT bufLen = 0;
		if(VerQueryValue(pBuf, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
		{   
			bRet = TRUE;
			wsprintf(tszFileVer, _T("v%d.%d.%d.%d"), HIWORD(pFineInfo->dwFileVersionMS), LOWORD(pFineInfo->dwFileVersionMS), HIWORD(pFineInfo->dwFileVersionLS), LOWORD(pFineInfo->dwFileVersionLS) );
		}
	}

	delete pBuf;

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Info::GetCurrentFileVersion()  tszFileVer : %s \r\n"), tszFileVer ));

	return bRet;
}
