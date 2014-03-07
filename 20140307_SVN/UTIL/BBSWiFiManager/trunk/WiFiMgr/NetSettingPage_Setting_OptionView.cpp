// NetSettingPage_Setting_OptionView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Setting_OptionView.h"

#include <wzcshell.h>


#define SHWZCM_GETSTATE	(WM_APP+6)
#define SHWZCM_SETSTATE	(WM_APP+7)

// CNetSettingPage_Setting_OptionView

IMPLEMENT_DYNCREATE(CNetSettingPage_Setting_OptionView, CFormView)

CNetSettingPage_Setting_OptionView::CNetSettingPage_Setting_OptionView()
	: CFormView(CNetSettingPage_Setting_OptionView::IDD)
{


	m_hwndSink = NULL;

	ZeroMemory(&m_wzcContext, sizeof(WZC_CONTEXT));

	ZeroMemory(&m_sBkgPingCheckOpt, sizeof(BBS_BKG_PINGCHECK_OPT));
	ZeroMemory(&m_sScanOption, sizeof(BBS_SCAN_OPTION_INFO));

	

	
}

CNetSettingPage_Setting_OptionView::~CNetSettingPage_Setting_OptionView()
{
}

void CNetSettingPage_Setting_OptionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NETWORKTOACCESS, m_comboNettoAccess);
	DDX_Control(pDX, IDC_COMBO_SCANTHRESHOLD, m_comboScanThreshold);
	DDX_Control(pDX, IDC_CHECK_AVA_NET_NOTI, m_chkbtnTurnOnAvaNetNofi);
}

BEGIN_MESSAGE_MAP(CNetSettingPage_Setting_OptionView, CFormView)
	ON_COMMAND(ID_DEFAULT, &CNetSettingPage_Setting_OptionView::OnDefault)
	ON_COMMAND(ID_APPLY, &CNetSettingPage_Setting_OptionView::OnApply)
	ON_BN_CLICKED(IDC_BUTTON_HELP_TMTX, &CNetSettingPage_Setting_OptionView::OnBnClickedButtonHelpTmtx)
END_MESSAGE_MAP()


// CNetSettingPage_Setting_OptionView �����Դϴ�.

#ifdef _DEBUG
void CNetSettingPage_Setting_OptionView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetSettingPage_Setting_OptionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNetSettingPage_Setting_OptionView �޽��� ó�����Դϴ�.

void CNetSettingPage_Setting_OptionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	BOOL bNotificationState = TRUE;

	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Setting_OptionView::OnInitialUpdate() \r\n") ));

	// Apply MenuBar
	memset(&m_mbi, 0, sizeof(SHMENUBARINFO));
	m_mbi.cbSize = sizeof( SHMENUBARINFO );
	m_mbi.hwndParent = m_hWnd;
	m_mbi.dwFlags = SHCMBF_HMENU;
	m_mbi.nToolBarId = IDR_MENU_APPLY_DEFAULT;
	m_mbi.hInstRes = AfxGetInstanceHandle();
	SHCreateMenuBar( &m_mbi );

	
	// Scan Option
	m_comboScanThreshold.AddString(L"All");
	m_comboScanThreshold.AddString(L">=Very Low");
	m_comboScanThreshold.AddString(L">=Low");
	m_comboScanThreshold.AddString(L">=Good");
	m_comboScanThreshold.AddString(L">=Very Good");

	m_comboNettoAccess.AddString(L"Only ADHOC");
	m_comboNettoAccess.AddString(L"Only Access Points");
	m_comboNettoAccess.AddString(L"All Available");


	GetScanOpt(&m_sScanOption);
	

	RETAILMSG(1, (TEXT("[NETUI] GetScanOpt \r\n") ));
	RETAILMSG(1, (TEXT("[NETUI] bTurnOnAvailNetNoti : %d \r\n"), m_sScanOption.bTurnOnAvailNetNoti ));
	RETAILMSG(1, (TEXT("[NETUI] dwScanThreShold : %d \r\n"), m_sScanOption.dwScanThreShold ));
	RETAILMSG(1, (TEXT("[NETUI] tszLockSSID : %s \r\n"), m_sScanOption.tszLockSSID ));
	RETAILMSG(1, (TEXT("[NETUI] dwNetToAccess : %d \r\n"), m_sScanOption.dwNetToAccess ));


	// XML�� �ش簪�� �ʱ�ȭ ���� �ʾ� -1�� ���ϵǸ� �⺻ Default ������ �ٽ� �������ش�.
	if(m_sScanOption.dwScanThreShold == -1)
	{
		m_sScanOption.dwScanThreShold = DEFAULT_SCANOPT_SCANTHRESHOLD_VALUE;
	}
	
	if(m_sScanOption.dwNetToAccess == -1)
	{
		m_hwndSink = ::FindWindow( TEXT("SinkWirelessNetUI"), NULL );

		if(m_hwndSink != NULL)
		{
			DWORD dwFlags = (DWORD)::SendMessage(m_hwndSink, SHWZCM_GETSTATE, 0, 0);

			// Success
			dwFlags--;

			m_sScanOption.dwNetToAccess = dwFlags;
		}
		else
		{
			m_sScanOption.dwNetToAccess = DEFAULT_SCANOPT_NETTOACCESS_VALUE;
		}
	}

	
	// xml���� �о�� ������, ���� �������� ���°��� �о�ͼ� ���°� �ٸ���� �ٽ� xml ���� �������ش�.
	
	SHWZCGetNotificationState(&bNotificationState);
	if(bNotificationState != m_sScanOption.bTurnOnAvailNetNoti)
	{
		m_sScanOption.bTurnOnAvailNetNoti = bNotificationState;
	}
	

	SetScanOpt(m_sScanOption);

	m_chkbtnTurnOnAvaNetNofi.SetCheck(m_sScanOption.bTurnOnAvailNetNoti);
	m_comboScanThreshold.SetCurSel(m_sScanOption.dwScanThreShold);
	m_comboNettoAccess.SetCurSel(m_sScanOption.dwNetToAccess);
	SetDlgItemText(IDC_EDIT_LOCK_SSID, m_sScanOption.tszLockSSID);



	// Power Option
	int nAllowValue = -1;
	GetPowerOpt(&nAllowValue);


	// �� ���� ���¿����� �ʱⰪ ����
	if( nAllowValue == -1 )
	{
		SetPowerOpt(TRUE);
		nAllowValue = TRUE;
	}

	if(nAllowValue)
	{
		CheckDlgButton(IDC_CHECK_ALLOW_SUSPEND, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_ALLOW_SUSPEND, BST_UNCHECKED);
	}




	// Initilize Force Restore form Invaild Ping Status
	GetBkgPingCheckOpt(&m_sBkgPingCheckOpt);

	RETAILMSG(1, (TEXT("[NETUI] GetBkgPingCheckOpt \r\n") ));
	RETAILMSG(1, (TEXT("[NETUI] bOperating : %d \r\n"), m_sBkgPingCheckOpt.bOperating ));
	RETAILMSG(1, (TEXT("[NETUI] dwBreakInterval : %d \r\n"), m_sBkgPingCheckOpt.dwBreakInterval ));
	RETAILMSG(1, (TEXT("[NETUI] dwPingInterval : %d \r\n"), m_sBkgPingCheckOpt.dwPingInterval ));
	RETAILMSG(1, (TEXT("[NETUI] dwMaxCheckCount : %d \r\n"), m_sBkgPingCheckOpt.dwMaxCheckCount ));
	RETAILMSG(1, (TEXT("[NETUI] tszCustomDestAddr : %s \r\n"), m_sBkgPingCheckOpt.tszCustomDestAddr ));

	if( GetPingCheckerStatus() )
	{
		CheckDlgButton(IDC_CHECK_FORCE_RESTORE_ENABLE, BST_CHECKED);
		m_sBkgPingCheckOpt.bOperating = TRUE;

		GetDlgItem(IDC_EDIT_SET_PING_BREAKITV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_PINGITV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_CHECKMAXCNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_CHECK_DESTADDR)->EnableWindow(FALSE);
		
	}
	else
	{
		CheckDlgButton(IDC_CHECK_FORCE_RESTORE_ENABLE, BST_UNCHECKED);
		m_sBkgPingCheckOpt.bOperating = FALSE;
			
		SetDlgItemInt(IDC_EDIT_SET_PING_BREAKITV, DEFAULT_CHECK_N_BREAK_INTERVAL);
		SetDlgItemInt(IDC_EDIT_SET_PING_PINGITV, DEFAULT_PING_INTERVAL);
		SetDlgItemInt(IDC_EDIT_SET_PING_CHECKMAXCNT, DEFAULT_MAX_CHECK_COUNT);
		SetDlgItemText(IDC_EDIT_SET_PING_CHECK_DESTADDR, _T("")); // Custom �ּҰ� �����Ǿ����� ������ �� üũ�ø��� DefaultGateWay �˻��ؼ� ������.

		GetDlgItem(IDC_EDIT_SET_PING_BREAKITV)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_PINGITV)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_CHECKMAXCNT)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_CHECK_DESTADDR)->EnableWindow();
	}

	// ��� 0�ϰ��(�߸��� ���ϰ��) Default�� ����
	if( (m_sBkgPingCheckOpt.dwBreakInterval == 0) && (m_sBkgPingCheckOpt.dwPingInterval == 0) && (m_sBkgPingCheckOpt.dwMaxCheckCount == 0) )
	{
		m_sBkgPingCheckOpt.dwBreakInterval = DEFAULT_CHECK_N_BREAK_INTERVAL;
		m_sBkgPingCheckOpt.dwPingInterval = DEFAULT_PING_INTERVAL;
		m_sBkgPingCheckOpt.dwMaxCheckCount = DEFAULT_MAX_CHECK_COUNT;
		memset(m_sBkgPingCheckOpt.tszCustomDestAddr, 0, sizeof(m_sBkgPingCheckOpt.tszCustomDestAddr));

		SetBkgPingCheckOpt(m_sBkgPingCheckOpt);

	}

	SetDlgItemInt(IDC_EDIT_SET_PING_BREAKITV, m_sBkgPingCheckOpt.dwBreakInterval);
	SetDlgItemInt(IDC_EDIT_SET_PING_PINGITV, m_sBkgPingCheckOpt.dwPingInterval);
	SetDlgItemInt(IDC_EDIT_SET_PING_CHECKMAXCNT, m_sBkgPingCheckOpt.dwMaxCheckCount);
	SetDlgItemText(IDC_EDIT_SET_PING_CHECK_DESTADDR, m_sBkgPingCheckOpt.tszCustomDestAddr);

	


	// Get WZC Context Timer Value
	if( QueryWzcContext(&m_wzcContext) == TRUE)
	{
		SetDlgItemInt(IDC_EDIT_CONTEXT_TMTP, m_wzcContext.tmTp, FALSE);
		SetDlgItemInt(IDC_EDIT_CONTEXT_TMTF, m_wzcContext.tmTf, FALSE);
		SetDlgItemInt(IDC_EDIT_CONTEXT_TMTR, m_wzcContext.tmTr, FALSE);
		SetDlgItemInt(IDC_EDIT_CONTEXT_TMTC, m_wzcContext.tmTc, FALSE);
	}
	else
	{
		// ERROR
		SetDlgItemInt(IDC_EDIT_CONTEXT_TMTP, 0, FALSE);
		SetDlgItemInt(IDC_EDIT_CONTEXT_TMTF, 0, FALSE);
		SetDlgItemInt(IDC_EDIT_CONTEXT_TMTR, 0, FALSE);
		SetDlgItemInt(IDC_EDIT_CONTEXT_TMTC, 0, FALSE);
	}






	SetCursor(hOldCursor);

}

void CNetSettingPage_Setting_OptionView::OnDefault()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// Set Defalut Value (Force Restore form Invalid Ping Status)

	CheckDlgButton(IDC_CHECK_FORCE_RESTORE_ENABLE, BST_UNCHECKED);

	SetDlgItemInt(IDC_EDIT_SET_PING_BREAKITV, DEFAULT_CHECK_N_BREAK_INTERVAL);
	SetDlgItemInt(IDC_EDIT_SET_PING_PINGITV, DEFAULT_PING_INTERVAL);
	SetDlgItemInt(IDC_EDIT_SET_PING_CHECKMAXCNT, DEFAULT_MAX_CHECK_COUNT);
	SetDlgItemText(IDC_EDIT_SET_PING_CHECK_DESTADDR, _T("")); // Custom �ּҰ� �����Ǿ����� ������ �� üũ�ø��� DefaultGateWay �˻��ؼ� ������.


	// Default NetworkToAccess Value : All Available(2)
	m_comboNettoAccess.SetCurSel(2);

	// Set Default Context Timer
	SetDlgItemInt(IDC_EDIT_CONTEXT_TMTP, BB_DEFAULT_TMTP, FALSE);
	SetDlgItemInt(IDC_EDIT_CONTEXT_TMTF, BB_DEFAULT_TMTF, FALSE);
	SetDlgItemInt(IDC_EDIT_CONTEXT_TMTR, TMMS_DEFAULT_TR, FALSE);
	SetDlgItemInt(IDC_EDIT_CONTEXT_TMTC, TMMS_DEFAULT_TC, FALSE);


	m_chkbtnTurnOnAvaNetNofi.SetCheck(BST_CHECKED);

	SetDlgItemText(IDC_EDIT_LOCK_SSID, L"");


	// Power Option
	CheckDlgButton(IDC_CHECK_ALLOW_SUSPEND, BST_CHECKED);

}


void CNetSettingPage_Setting_OptionView::OnApply()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	TCHAR tszLockSSID[64] = L"";

	// Scan Option

	m_sScanOption.dwScanThreShold = m_comboScanThreshold.GetCurSel();
	m_sScanOption.dwNetToAccess = m_comboNettoAccess.GetCurSel();


	m_hwndSink = ::FindWindow( TEXT("SinkWirelessNetUI"), NULL );

	if(m_hwndSink != NULL)
	{
		DWORD dwFlags = (DWORD)::SendMessage(m_hwndSink, SHWZCM_GETSTATE, 0, 0);

		// Success
		dwFlags--;

		// ���� ���� �ٸ���쿡�� ��������
		if( dwFlags != m_sScanOption.dwNetToAccess )
		{
			::SendMessage(m_hwndSink, SHWZCM_SETSTATE, m_sScanOption.dwNetToAccess, 0);
		}
	}

	GetDlgItemText(IDC_EDIT_LOCK_SSID, tszLockSSID, sizeof(tszLockSSID) / sizeof(TCHAR) );

	_tcscpy(m_sScanOption.tszLockSSID, tszLockSSID);


	

	if( m_chkbtnTurnOnAvaNetNofi.GetCheck() == BST_CHECKED )
	{
		m_sScanOption.bTurnOnAvailNetNoti = TRUE;
	}
	else
	{
		m_sScanOption.bTurnOnAvailNetNoti = FALSE;
	}
	
	SHWZCSetNotificationState(m_sScanOption.bTurnOnAvailNetNoti);
	
	SetScanOpt(m_sScanOption);


	// Power Option
	BBS_ADAPTER_INFO adapterInfo = {0,};
	BYTE pucBSSID[6] = {0,};
	BOOL bConnected = FALSE;

	GetWlanDefaultInformation(&adapterInfo);

	// BSSID �� �˻��Ǹ� ��Ʈ��ũ�� ����Ǿ��ִ� ����
	bConnected = GetBSSID(adapterInfo.tszAdapterName, pucBSSID);

	
	// BSSID�� �˻��� ���¶�� �翬����� ��������� �ٷ� �����Ѵ�.
	if( IsDlgButtonChecked(IDC_CHECK_ALLOW_SUSPEND) == BST_CHECKED )
	{
		SetPowerOpt(TRUE);

		if(bConnected)
		{
			SetUnattendedMode(FALSE);
		}
	}
	else
	{
		SetPowerOpt(FALSE);
		
		if(bConnected)
		{
			SetUnattendedMode(TRUE);
		}
	}


	// WZC Context Timer
	m_wzcContext.tmTp = GetDlgItemInt(IDC_EDIT_CONTEXT_TMTP, NULL, FALSE);
	m_wzcContext.tmTf = GetDlgItemInt(IDC_EDIT_CONTEXT_TMTF, NULL, FALSE);
	m_wzcContext.tmTr = GetDlgItemInt(IDC_EDIT_CONTEXT_TMTR, NULL, FALSE);
	m_wzcContext.tmTc = GetDlgItemInt(IDC_EDIT_CONTEXT_TMTC, NULL, FALSE);

	if( SetWzcParameter(m_wzcContext) == FALSE )
	{
		// ERROR
	}

	// WM�� ��� �Ʒ��� ������Ʈ�� ��� �ȹٲ��ָ�, ������ ������, �� ������Ʈ�� ������ �ٽ� �����Ѵ�.
	// ����, TMTF ���� �����ϸ�, �Ʒ��� ��ε� �����ʿ�
	HKEY hKey = NULL;
	DWORD dwDisposition = 0;

	if( RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"Comm\\WiFi", 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS )
	{
		DWORD dwData = 0;


		dwData = m_wzcContext.tmTf / 1000; // �ʴ����� ����Ǳ� ������ 1000�� ������
		RegSetValueEx(hKey, L"ScanInterval", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
	
		RegCloseKey(hKey);
	}



	m_sBkgPingCheckOpt.bOperating = IsDlgButtonChecked(IDC_CHECK_FORCE_RESTORE_ENABLE);
	m_sBkgPingCheckOpt.dwBreakInterval = GetDlgItemInt(IDC_EDIT_SET_PING_BREAKITV);
	m_sBkgPingCheckOpt.dwPingInterval = GetDlgItemInt(IDC_EDIT_SET_PING_PINGITV);
	m_sBkgPingCheckOpt.dwMaxCheckCount = GetDlgItemInt(IDC_EDIT_SET_PING_CHECKMAXCNT);
	GetDlgItemText(IDC_EDIT_SET_PING_CHECK_DESTADDR, m_sBkgPingCheckOpt.tszCustomDestAddr, sizeof(m_sBkgPingCheckOpt.tszCustomDestAddr)/sizeof(TCHAR) );


	if( m_sBkgPingCheckOpt.bOperating == TRUE)
	{
		if( GetPingCheckerStatus() )
		{
			PingCheckerStop();
		}

		RETAILMSG(1, (TEXT("[NETUI] tszCustomDestAddr : %s \r\n"), m_sBkgPingCheckOpt.tszCustomDestAddr));

		if( PingCheckerStart(m_sBkgPingCheckOpt) == FALSE )
		{
			// ERROR
		}

		GetDlgItem(IDC_EDIT_SET_PING_BREAKITV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_PINGITV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_CHECKMAXCNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_CHECK_DESTADDR)->EnableWindow(FALSE);

	}
	else
	{
		if( PingCheckerStop() == FALSE )
		{
			// ERROR
		}

		GetDlgItem(IDC_EDIT_SET_PING_BREAKITV)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_PINGITV)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_CHECKMAXCNT)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_CHECK_DESTADDR)->EnableWindow();

	}


	// Save setting value to XML
	SetBkgPingCheckOpt(m_sBkgPingCheckOpt);


	SetCursor(hOldCursor);
}

void CNetSettingPage_Setting_OptionView::OnBnClickedButtonHelpTmtx()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDescriptionTMTxDialog dialog;

	dialog.DoModal();	
}
