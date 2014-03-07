// NetSettingPage_Setting_OptionView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Netui_WM.h"
#include "NetSettingPage_Setting_OptionView.h"


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

	
}

CNetSettingPage_Setting_OptionView::~CNetSettingPage_Setting_OptionView()
{
}

void CNetSettingPage_Setting_OptionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NETWORKTOACCESS, m_comboNettoAccess);
}

BEGIN_MESSAGE_MAP(CNetSettingPage_Setting_OptionView, CFormView)
	ON_COMMAND(ID_DEFAULT, &CNetSettingPage_Setting_OptionView::OnDefault)
	ON_COMMAND(ID_APPLY, &CNetSettingPage_Setting_OptionView::OnApply)
	ON_STN_CLICKED(IDC_STATIC_TMTP, &CNetSettingPage_Setting_OptionView::OnStnClickedStaticTmtp)
	ON_STN_CLICKED(IDC_STATIC_TMTR, &CNetSettingPage_Setting_OptionView::OnStnClickedStaticTmtr)
	ON_STN_CLICKED(IDC_STATIC_TMTF, &CNetSettingPage_Setting_OptionView::OnStnClickedStaticTmtf)
	ON_STN_CLICKED(IDC_STATIC_TMTC, &CNetSettingPage_Setting_OptionView::OnStnClickedStaticTmtc)
END_MESSAGE_MAP()


// CNetSettingPage_Setting_OptionView 진단입니다.

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


// CNetSettingPage_Setting_OptionView 메시지 처리기입니다.

void CNetSettingPage_Setting_OptionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));


	// Apply MenuBar
	memset(&m_mbi, 0, sizeof(SHMENUBARINFO));
	m_mbi.cbSize = sizeof( SHMENUBARINFO );
	m_mbi.hwndParent = m_hWnd;
	m_mbi.dwFlags = SHCMBF_HMENU;
	m_mbi.nToolBarId = IDR_MENU_APPLY_DEFAULT;
	m_mbi.hInstRes = AfxGetInstanceHandle();
	SHCreateMenuBar( &m_mbi );



	// Initilize Force Restore form Invaild Ping Status
	if( IsBkgPingCheckOperating(&m_sBkgPingCheckOpt) )
	{
		CheckDlgButton(IDC_CHECK_FORCE_RESTORE_ENABLE, BST_CHECKED);

		SetDlgItemInt(IDC_EDIT_SET_PING_BREAKITV, m_sBkgPingCheckOpt.dwBreakInterval);
		SetDlgItemInt(IDC_EDIT_SET_PING_PINGITV, m_sBkgPingCheckOpt.dwPingInterval);
		SetDlgItemInt(IDC_EDIT_SET_PING_CHECKMAXCNT, m_sBkgPingCheckOpt.dwMaxCheckCount);
		SetDlgItemText(IDC_EDIT_SET_PING_CHECK_DESTADDR, m_sBkgPingCheckOpt.tszCustomDestAddr);

		GetDlgItem(IDC_EDIT_SET_PING_BREAKITV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_PINGITV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_CHECKMAXCNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_CHECK_DESTADDR)->EnableWindow(FALSE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_FORCE_RESTORE_ENABLE, BST_UNCHECKED);

		SetDlgItemInt(IDC_EDIT_SET_PING_BREAKITV, DEFAULT_CHECK_N_BREAK_INTERVAL);
		SetDlgItemInt(IDC_EDIT_SET_PING_PINGITV, DEFAULT_PING_INTERVAL);
		SetDlgItemInt(IDC_EDIT_SET_PING_CHECKMAXCNT, DEFAULT_MAX_CHECK_COUNT);
		SetDlgItemText(IDC_EDIT_SET_PING_CHECK_DESTADDR, _T("")); // Custom 주소가 지정되어있지 않으면 매 체크시마다 DefaultGateWay 검사해서 보낸다.

		GetDlgItem(IDC_EDIT_SET_PING_BREAKITV)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_PINGITV)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_CHECKMAXCNT)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_CHECK_DESTADDR)->EnableWindow();
	}



	// Initilize Network to Access ComboBox

	m_comboNettoAccess.AddString(L"Only ADHOC");
	m_comboNettoAccess.AddString(L"Only Access Points");
	m_comboNettoAccess.AddString(L"All Available");


	// Find a sink window
	m_hwndSink = ::FindWindow( TEXT("SinkWirelessNetUI"), NULL );

	if(m_hwndSink != NULL)
	{
		// Get the global flags
		// Send a message to the sink window
		DWORD   dwFlags = (DWORD)::SendMessage(m_hwndSink, SHWZCM_GETSTATE, 0, 0);

		// Success
		dwFlags--;

		m_comboNettoAccess.SetCurSel(dwFlags);

		RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Setting_OptionView dwFlags:%d \r\n"), dwFlags ));
	}


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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// Set Defalut Value (Force Restore form Invalid Ping Status)

	CheckDlgButton(IDC_CHECK_FORCE_RESTORE_ENABLE, BST_UNCHECKED);

	SetDlgItemInt(IDC_EDIT_SET_PING_BREAKITV, DEFAULT_CHECK_N_BREAK_INTERVAL);
	SetDlgItemInt(IDC_EDIT_SET_PING_PINGITV, DEFAULT_PING_INTERVAL);
	SetDlgItemInt(IDC_EDIT_SET_PING_CHECKMAXCNT, DEFAULT_MAX_CHECK_COUNT);
	SetDlgItemText(IDC_EDIT_SET_PING_CHECK_DESTADDR, _T("")); // Custom 주소가 지정되어있지 않으면 매 체크시마다 DefaultGateWay 검사해서 보낸다.


	// Default NetworkToAccess Value : All Available(2)
	m_comboNettoAccess.SetCurSel(2);

	// Set Default Context Timer
	SetDlgItemInt(IDC_EDIT_CONTEXT_TMTP, TMMS_DEFAULT_TP, FALSE);
	SetDlgItemInt(IDC_EDIT_CONTEXT_TMTF, TMMS_DEFAULT_TF, FALSE);
	SetDlgItemInt(IDC_EDIT_CONTEXT_TMTR, TMMS_DEFAULT_TR, FALSE);
	SetDlgItemInt(IDC_EDIT_CONTEXT_TMTC, TMMS_DEFAULT_TC, FALSE);

}


void CNetSettingPage_Setting_OptionView::OnApply()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));


	if(m_hwndSink != NULL)
	{
		DWORD dwFlags = (DWORD)::SendMessage(m_hwndSink, SHWZCM_GETSTATE, 0, 0);

		// Success
		dwFlags--;

		// 현재 값과 다를경우에만 세팅해줌
		if( dwFlags != m_comboNettoAccess.GetCurSel() )
		{
			::SendMessage(m_hwndSink, SHWZCM_SETSTATE, m_comboNettoAccess.GetCurSel(), 0);
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

	// WM의 경우 아래의 레지스트리 경로 안바꿔주면, 부팅할 때마다, 이 레지스트리 값으로 다시 세팅한다.
	// 따라서, TMTF 값을 변경하면, 아래의 경로도 변경필요
	HKEY hKey = NULL;
	DWORD dwDisposition = 0;

	if( RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"Comm\\WiFi", 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS )
	{
		DWORD dwData = 0;


		dwData = m_wzcContext.tmTf / 1000; // 초단위로 저장되기 때문에 1000을 나눠줌
		RegSetValueEx(hKey, L"ScanInterval", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
	
		RegCloseKey(hKey);
	}





	if( IsDlgButtonChecked(IDC_CHECK_FORCE_RESTORE_ENABLE) == BST_CHECKED)
	{


		if( IsBkgPingCheckOperating(&m_sBkgPingCheckOpt) )
		{
			BkgPingStatusCheckStop();
		}

		m_sBkgPingCheckOpt.dwBreakInterval = GetDlgItemInt(IDC_EDIT_SET_PING_BREAKITV);
		m_sBkgPingCheckOpt.dwPingInterval = GetDlgItemInt(IDC_EDIT_SET_PING_PINGITV);
		m_sBkgPingCheckOpt.dwMaxCheckCount = GetDlgItemInt(IDC_EDIT_SET_PING_CHECKMAXCNT);
		GetDlgItemText(IDC_EDIT_SET_PING_CHECK_DESTADDR, m_sBkgPingCheckOpt.tszCustomDestAddr, sizeof(m_sBkgPingCheckOpt.tszCustomDestAddr)/sizeof(TCHAR) );

		RETAILMSG(1, (TEXT("[NETUI] tszCustomDestAddr : %s \r\n"), m_sBkgPingCheckOpt.tszCustomDestAddr));

		if( BkgPingStatusCheckStart(m_sBkgPingCheckOpt) == FALSE )
		{
			// ERROR
		}

		GetDlgItem(IDC_EDIT_SET_PING_BREAKITV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_PINGITV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_CHECKMAXCNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SET_PING_CHECK_DESTADDR)->EnableWindow(FALSE);

		// Save setting value to XML
		SetBkgPingCheckOpt(TRUE, m_sBkgPingCheckOpt.dwBreakInterval, m_sBkgPingCheckOpt.dwPingInterval, m_sBkgPingCheckOpt.dwMaxCheckCount, m_sBkgPingCheckOpt.tszCustomDestAddr);
	}
	else
	{
		if( BkgPingStatusCheckStop() == FALSE )
		{
			// ERROR
		}

		GetDlgItem(IDC_EDIT_SET_PING_BREAKITV)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_PINGITV)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_CHECKMAXCNT)->EnableWindow();
		GetDlgItem(IDC_EDIT_SET_PING_CHECK_DESTADDR)->EnableWindow();

		// Save setting value to XML
		SetBkgPingCheckOpt(FALSE, m_sBkgPingCheckOpt.dwBreakInterval, m_sBkgPingCheckOpt.dwPingInterval, m_sBkgPingCheckOpt.dwMaxCheckCount, m_sBkgPingCheckOpt.tszCustomDestAddr);
	}



	SetCursor(hOldCursor);
}

void CNetSettingPage_Setting_OptionView::OnStnClickedStaticTmtp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"Timeout value in milliseconds to expect a media connect for a selected configuration");
}

void CNetSettingPage_Setting_OptionView::OnStnClickedStaticTmtr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"Timeout value in milliseconds until a rescan operation completes");
}

void CNetSettingPage_Setting_OptionView::OnStnClickedStaticTmtf()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"Timeout value in milliseconds to recover from a failed configuration");
}

void CNetSettingPage_Setting_OptionView::OnStnClickedStaticTmtc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"Timeout value in milliseconds to retry a valid configuration");
}
