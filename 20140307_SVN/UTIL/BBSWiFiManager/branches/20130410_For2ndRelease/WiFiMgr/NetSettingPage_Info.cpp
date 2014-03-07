// NetSettingPage_Info.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Info.h"


// CNetSettingPage_Info 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNetSettingPage_Info, CPropertyPage)

CNetSettingPage_Info::CNetSettingPage_Info()
	: CPropertyPage(CNetSettingPage_Info::IDD)
{
	memset( &m_AdapterInfo, 0x0, sizeof(BBS_ADAPTER_INFO) );

	bPingResult = FALSE;
	bSuccessGettingBSSID = FALSE;

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




	GetWlanDefaultInformation(&m_AdapterInfo);

	// Version

	CString strFirmware = L"";
	CString strDriver = L"";
	CString strNetui = L"";
	CString strSDIODrv = L"";

	// Firmware
	if( GetFirmwareVersion(m_AdapterInfo.tszAdapterName, strFirmware) )
	{
		SetDlgItemText(IDC_STATIC_FIRMWARE_VERSION, strFirmware.GetBuffer() );
	}
	else
	{
		SetDlgItemText(IDC_STATIC_FIRMWARE_VERSION, L"Unknown Version");
	}
	

	// Wlan Driver
	if( GetDriverVersion(m_AdapterInfo.tszAdapterName, strDriver) )
	{
		SetDlgItemText(IDC_STATIC_DRV_VERSION, strDriver.GetBuffer() );
	}
	else
	{
		SetDlgItemText(IDC_STATIC_DRV_VERSION, L"Unknown Version");
	}

	// Netui(BBSWiFiMgr)
	if( GetnetuiFileVersion(strNetui) )
	{
		SetDlgItemText(IDC_STATIC_WIFIMGR_VERSION, strNetui.GetBuffer() );
	}
	else
	{
		SetDlgItemText(IDC_STATIC_WIFIMGR_VERSION, L"Unknown Version");
	}

	// SDIO Driver
	if( GetSDIODrvFileVersion(strSDIODrv) )
	{
		SetDlgItemText(IDC_STATIC_SDIODRV_VERSION, strSDIODrv.GetBuffer() );
	}
	else
	{
		SetDlgItemText(IDC_STATIC_SDIODRV_VERSION, L"Unknown Version");
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
	SetDlgItemText(IDC_STATIC_ADAPTER, L"Adapter : ");
	SetDlgItemText(IDC_STATIC_ADAPTERNAME, strAdpater);
	SetDlgItemText(IDC_STATIC_BK, L"");
	
	// IP Information
	SetDlgItemText(IDC_STATIC_IPADDRESS, pIPInfo->tszIPAddr);
	SetDlgItemText(IDC_STATIC_SUBNETMASK, pIPInfo->tszSubNetMask);

	if( _tcscmp(pIPInfo->tszIPAddr, _T("0.0.0.0")) == 0 )
	{
		SetDlgItemText(IDC_STATIC_GATEWAY, _T("-") );
		SetDlgItemText(IDC_STATIC_DNS_SERVER, _T("-") );
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






BOOL CNetSettingPage_Info::UpdateAdpaterNetworkInfo()
{
	NDIS_802_11_MAC_ADDRESS macBSSID = {0,};
	TCHAR tszTemp[256] = L"";

	// SSID
	SetDlgItemText(IDC_STATIC_SSID, _T("-") );

	// BSSID(MAC)
	SetDlgItemText(IDC_STATIC_BSSID, _T("-") );

	// Privacy(WEP/OPEN)
	SetDlgItemText(IDC_STATIC_PRIVACY, _T("-") );

	// Infrastructure
	SetDlgItemText(IDC_STATIC_INFRA, _T("-") );

	// RSSI
	SetDlgItemText(IDC_STATIC_RSSI, _T("-") );

	// NetworkType
	SetDlgItemText(IDC_STATIC_NETWORK_TYPE, _T("-") );

	// Channel (DSConfig)
	SetDlgItemText(IDC_STATIC_CHANNEL, _T("-") );

	// Beacon
	SetDlgItemText(IDC_STATIC_BEACON, _T("-") );

	// BitRate (LinkSpeed)
	SetDlgItemText(IDC_STATIC_LINKSPEED, _T("-") );

	// PacketsSent
	SetDlgItemText(IDC_STATIC_PACKETS_SENT, _T("-") );

	// PacketsReceived
	SetDlgItemText(IDC_STATIC_PACKETS_RECEIVED, _T("-") );


	if( _tcslen(m_AdapterInfo.tszAdapterName) != 0 )
	{
		NDIS_WLAN_BSSID ndisWlanBssid[128] = {0,};
		NIC_STATISTICS nicStatistics = {0,};
		DWORD dwNumofItems = 0;

		// Get Cur BSSID
		if( !GetBSSID(m_AdapterInfo.tszAdapterName, (BYTE*)macBSSID) )
		{
			// ERROR or Disconnected
			bSuccessGettingBSSID = FALSE;
			return FALSE;
		}

		bSuccessGettingBSSID = TRUE;

		// Get BSSID LIST Information
		if( GetBBSIDListInfo(m_AdapterInfo.tszAdapterName, ndisWlanBssid, &dwNumofItems) == TRUE )
		{
			if( dwNumofItems > 0 )
			{
				for(DWORD dwIdx = 0; dwIdx<dwNumofItems; dwIdx++)
				{

					if( memcmp(ndisWlanBssid[dwIdx].MacAddress, macBSSID, sizeof(NDIS_802_11_MAC_ADDRESS)) == 0 )
					{
						// SSID
						mbstowcs(tszTemp, (char*)ndisWlanBssid[dwIdx].Ssid.Ssid, ndisWlanBssid[dwIdx].Ssid.SsidLength);
						SetDlgItemText(IDC_STATIC_SSID, tszTemp );

						// RSSI
						SetDlgItemInt(IDC_STATIC_RSSI, ndisWlanBssid[dwIdx].Rssi, TRUE);

						// BSSID
						wsprintf(tszTemp, L"%02X:%02X:%02X:%02X:%02X:%02X", ndisWlanBssid[dwIdx].MacAddress[0], ndisWlanBssid[dwIdx].MacAddress[1], ndisWlanBssid[dwIdx].MacAddress[2], ndisWlanBssid[dwIdx].MacAddress[3], ndisWlanBssid[dwIdx].MacAddress[4], ndisWlanBssid[dwIdx].MacAddress[5] );
						SetDlgItemText(IDC_STATIC_BSSID, tszTemp );

						// Privacy
						SetDlgItemText(IDC_STATIC_PRIVACY, (ndisWlanBssid[dwIdx].Privacy)? L"WEP":L"OPEN" );

						// Channel
						wsprintf(tszTemp, L"%d (%d Mhz)", ChannelFreq2Number(ndisWlanBssid[dwIdx].Configuration.DSConfig), (ndisWlanBssid[dwIdx].Configuration.DSConfig / 1000) );
						SetDlgItemText(IDC_STATIC_CHANNEL, tszTemp);

						// Beacon
						SetDlgItemInt(IDC_STATIC_BEACON, ndisWlanBssid[dwIdx].Configuration.BeaconPeriod);

						// Infrastructure
						switch(ndisWlanBssid[dwIdx].InfrastructureMode)
						{
						case Ndis802_11IBSS:
							wsprintf(tszTemp, L"Peer");
							break;

						case Ndis802_11Infrastructure:
							wsprintf(tszTemp, L"AP");
							break;

						default:
							wsprintf(tszTemp, L"Unknown");
							break;

						}
						SetDlgItemText(IDC_STATIC_INFRA, tszTemp );


						// NetworkType
						switch(ndisWlanBssid[dwIdx].NetworkTypeInUse)
						{
						case Ndis802_11FH:
							wsprintf(tszTemp, L"FH");
							break;

						case Ndis802_11DS:
							wsprintf(tszTemp, L"DS");
							break;

						case Ndis802_11OFDM5:
							wsprintf(tszTemp, L"OFDM5");
							break;

						case Ndis802_11OFDM24:
							wsprintf(tszTemp, L"OFDM24");
							break;

						case Ndis802_11Automode:
							wsprintf(tszTemp, L"Automode");
							break;

						default:
							wsprintf(tszTemp, L"Unknown");
							break;
						}
						SetDlgItemText(IDC_STATIC_NETWORK_TYPE, tszTemp );

						break;
					}
				}

			}
		}

		
		// Get NIC State
		if( GetNICStatistics(m_AdapterInfo.tszAdapterName, &nicStatistics) == TRUE )
		{
			double dBitRate = 0;
			double dPacketsSent = 0;
			double dPacketsRecv = 0;


			// BitRate (LinkSpeed)

			dBitRate = (nicStatistics.LinkSpeed) / 10000;

			wsprintf(tszTemp, L"%0.1f (Mbit/s)", dBitRate);
			SetDlgItemText(IDC_STATIC_LINKSPEED, tszTemp );

			// PacketsSent
			if( nicStatistics.PacketsSent > 1000000)
			{
				wsprintf(tszTemp, L"%d MB", nicStatistics.PacketsSent / 1000000);
			}
			else if(  nicStatistics.PacketsSent > 1000 )
			{
				wsprintf(tszTemp, L"%d MB", nicStatistics.PacketsSent / 1000);
			}
			else
			{
				wsprintf(tszTemp, L"%d B", nicStatistics.PacketsSent);
			}
			SetDlgItemText(IDC_STATIC_PACKETS_SENT, tszTemp );
			

			// PacketsReceived
			if( nicStatistics.PacketsReceived > 1000000)
			{
				wsprintf(tszTemp, L"%d MB", nicStatistics.PacketsReceived / 1000000);
			}
			else if(  nicStatistics.PacketsSent > 1000 )
			{
				wsprintf(tszTemp, L"%d MB", nicStatistics.PacketsReceived / 1000);
			}
			else
			{
				wsprintf(tszTemp, L"%d B", nicStatistics.PacketsReceived);
			}
			SetDlgItemText(IDC_STATIC_PACKETS_RECEIVED, tszTemp );

		}
		

	}
	else
	{
		GetWlanDefaultInformation(&m_AdapterInfo);
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
			RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Info PSN_KILLACTIVE1 \r\n")  ));
			::ShowWindow(m_mbi.hwndMB, SW_HIDE);
			KillTimer(UPDATE_TIMERID_INFORMATION);

			ClearAllControls();
			RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Info PSN_KILLACTIVE2 \r\n")  ));
			break;
        case PSN_SETACTIVE:
			RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Info PSN_SETACTIVE \r\n")  ));
			::ShowWindow(m_mbi.hwndMB, SW_SHOW);
			
			UpdateAdpaterIPInfo(&m_AdapterInfo);
			UpdateAdpaterNetworkInfo();

			SetTimer(UPDATE_TIMERID_INFORMATION, TIMER_INTERVAL_INFORMATION, NULL);
            break;
    }
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void CNetSettingPage_Info::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	BBS_PING_RESULT pingResult = {0,};

	// 통신가능 상태 체크를 위한 ping옵션 구조체
	BBS_PING_OPTION pingOption = {0,};


	// Ping 클래스
	CBBS_Ping ping; 

	pingOption.dwSendBufSize = DEFAULT_SEND_SIZE;
	pingOption.dwTimeOut = DEFAULT_TIMEOUT;
	pingOption.ucTos = DEFAULT_TOS;
	pingOption.ucTtl = DEFAULT_TTL;

	
	switch( nIDEvent )
	{
	case UPDATE_TIMERID_INFORMATION:

		RefreshBSSIDList(m_AdapterInfo.tszAdapterName);
		Sleep(2000);	// 리스캔 이후 바로 APLIST 를 조회하면 한번씩 조회안되는 현상이 발생됨(TMTR 시간만큼 기다렸다가 조회해줘야 하나, 일단 항상 2초 대기
		UpdateAdpaterIPInfo(&m_AdapterInfo);
		UpdateAdpaterNetworkInfo();
		
		
		// 기본 DefaultGateWay로 사용가능 상태 체크
		if( bSuccessGettingBSSID == TRUE )
		{
			_tcscpy(pingOption.tszDestIP, m_AdapterInfo.tszDefaultGateWay);

			ping.InitPing(pingOption, NULL);

			
			bPingResult = TRUE;
			if( ping.StartPing(&pingResult) == FALSE )
			{
				Sleep(500);
				if(ping.StartPing(&pingResult) == FALSE)
				{
					bPingResult = FALSE;
				}	
			}

			// Ping Deinit
			ping.DeinitPing();
		}

		break;
	}

	CPropertyPage::OnTimer(nIDEvent);
}

HBRUSH CNetSettingPage_Info::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	switch( pWnd->GetDlgCtrlID() )
	{
	case IDC_STATIC_RSSI:
		if( GetDlgItemInt(IDC_STATIC_RSSI) < -75 )
		{
			pDC->SetTextColor(RGB(250, 0, 0));
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 200));
		}
		break;
		
	case IDC_STATIC_ADAPTERNAME:
	case IDC_STATIC_ADAPTER:
	case IDC_STATIC_BK:
		// BSSID 가 확인된상태에서 DefaultGateWay로의 Ping 가능상태 여부 확인
		
		if(bSuccessGettingBSSID)
		{
			if(bPingResult == TRUE)
			{
				HBRUSH br = CreateSolidBrush(RGB(0,255,0));
				pDC->SetBkMode(TRANSPARENT);
				return (HBRUSH)br;
			}
			else
			{
				HBRUSH br = CreateSolidBrush(RGB(255,255,0));
				pDC->SetBkMode(TRANSPARENT);
				return (HBRUSH)br;
			}
		}
		break;
		
		
	}


	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CNetSettingPage_Info::ClearAllControls()
{

	SetDlgItemText(IDC_STATIC_IPADDRESS, L"-");
	SetDlgItemText(IDC_STATIC_SUBNETMASK, L"-");

	SetDlgItemText(IDC_STATIC_GATEWAY, _T("-") );
	SetDlgItemText(IDC_STATIC_DNS_SERVER, _T("-") );
	SetDlgItemText(IDC_STATIC_DHCP_ENABLED, _T("-") );



	// SSID
	SetDlgItemText(IDC_STATIC_SSID, _T("-") );

	// BSSID(MAC)
	SetDlgItemText(IDC_STATIC_BSSID, _T("-") );

	// Privacy(WEP/OPEN)
	SetDlgItemText(IDC_STATIC_PRIVACY, _T("-") );

	// Infrastructure
	SetDlgItemText(IDC_STATIC_INFRA, _T("-") );

	// RSSI
	SetDlgItemText(IDC_STATIC_RSSI, _T("-") );

	// NetworkType
	SetDlgItemText(IDC_STATIC_NETWORK_TYPE, _T("-") );

	// Channel (DSConfig)
	SetDlgItemText(IDC_STATIC_CHANNEL, _T("-") );

	// Beacon
	SetDlgItemText(IDC_STATIC_BEACON, _T("-") );

	// BitRate (LinkSpeed)
	SetDlgItemText(IDC_STATIC_LINKSPEED, _T("-") );

	// PacketsSent
	SetDlgItemText(IDC_STATIC_PACKETS_SENT, _T("-") );

	// PacketsReceived
	SetDlgItemText(IDC_STATIC_PACKETS_RECEIVED, _T("-") );
}
