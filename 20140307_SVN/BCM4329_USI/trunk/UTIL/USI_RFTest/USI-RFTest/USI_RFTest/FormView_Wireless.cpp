// FormView_Wireless.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "USI_RFTest.h"
#include "FormView_Wireless.h"



#define CHANNEL_NUM_MIN	(1)
#define CHANNEL_NUM_MAX	(14)

#define POWER_NUM_MIN	(68)
#define POWER_NUM_MAX	(80)

// CFormView_Wireless

IMPLEMENT_DYNCREATE(CFormView_Wireless, CFormView)

CFormView_Wireless::CFormView_Wireless()
	: CFormView(CFormView_Wireless::IDD)
{
	memset(&m_sei, 0, sizeof(SHELLEXECUTEINFO));
	
}

CFormView_Wireless::~CFormView_Wireless()
{
}

void CFormView_Wireless::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_CHANNEL, m_spinChannel);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
}

BEGIN_MESSAGE_MAP(CFormView_Wireless, CFormView)
	ON_BN_CLICKED(IDC_RADIO_MODULATION, &CFormView_Wireless::OnBnClickedRadioModulation)
	ON_BN_CLICKED(IDC_RADIO_NON_MODULATION, &CFormView_Wireless::OnBnClickedRadioNonModulation)
	ON_BN_CLICKED(IDC_BUTTON_WLAN_TX, &CFormView_Wireless::OnBnClickedButtonWlanTx)
	ON_BN_CLICKED(IDC_BUTTON_WLAN_RX, &CFormView_Wireless::OnBnClickedButtonWlanRx)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CFormView_Wireless::OnCbnSelchangeComboMode)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CFormView_Wireless 진단입니다.

#ifdef _DEBUG
void CFormView_Wireless::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormView_Wireless::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormView_Wireless 메시지 처리기입니다.

void CFormView_Wireless::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	// Default : Modulation 선택
	CheckRadioButton(IDC_RADIO_MODULATION, IDC_RADIO_NON_MODULATION, IDC_RADIO_MODULATION);
	OnBnClickedRadioModulation();

	// Mode
	m_comboMode.AddString(L"802.11b");
	m_comboMode.AddString(L"802.11g");
	m_comboMode.AddString(L"802.11n");
	m_comboMode.SetCurSel(0);

	// Default Rate
	SetDlgItemInt(IDC_EDIT_RATE, 11);

	// Default Power
	SetDlgItemInt(IDC_EDIT_POWER, POWER_NUM_MAX);


	m_spinChannel.SetRange(CHANNEL_NUM_MIN, CHANNEL_NUM_MAX);
	m_spinChannel.SetPos(0);


	// Wlan Init Test
	SendWlCmd(L"ver");



	SetCursor(hOldCursor);

}
void CFormView_Wireless::OnBnClickedRadioModulation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Channel Controls : Show
	GetDlgItem(IDC_STATIC_CHANNEL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_CHANNEL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SPIN_CHANNEL)->ShowWindow(SW_SHOW);

	// Mode Controls : Show
	GetDlgItem(IDC_STATIC_MODE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_MODE)->ShowWindow(SW_SHOW);

	// Rate Controls : Show
	GetDlgItem(IDC_STATIC_RATE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_RATE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_RATE_MBPS)->ShowWindow(SW_SHOW);
	
	// Controls : Show
	GetDlgItem(IDC_STATIC_POWER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_POWER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_POWER_QDBM)->ShowWindow(SW_SHOW);

}

void CFormView_Wireless::OnBnClickedRadioNonModulation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Channel Controls : Show
	GetDlgItem(IDC_STATIC_CHANNEL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_CHANNEL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SPIN_CHANNEL)->ShowWindow(SW_SHOW);

	// Mode Controls : Hide
	GetDlgItem(IDC_STATIC_MODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_MODE)->ShowWindow(SW_HIDE);

	// Rate Controls : Hide
	GetDlgItem(IDC_STATIC_RATE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_RATE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_RATE_MBPS)->ShowWindow(SW_HIDE);
	
	// Controls : Hide
	GetDlgItem(IDC_STATIC_POWER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POWER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_POWER_QDBM)->ShowWindow(SW_HIDE);



}


void CFormView_Wireless::OnBnClickedButtonWlanTx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strCmd = L"";
	DWORD dwChannel = 0;
	DWORD dwPower = 0;
	DWORD dwMode = 0;
	DWORD dwRate = 0;

	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));


	int nModulation = GetCheckedRadioButton(IDC_RADIO_MODULATION, IDC_RADIO_NON_MODULATION);


	dwChannel = GetDlgItemInt(IDC_EDIT_CHANNEL);
	dwMode = m_comboMode.GetCurSel();
	dwPower = GetDlgItemInt(IDC_EDIT_POWER);
	dwRate = GetDlgItemInt(IDC_EDIT_RATE);

	switch(nModulation)
	{
	case IDC_RADIO_MODULATION:
		SendWlCmd(L"down");
		SendWlCmd(L"clk 1");
		SendWlCmd(L"band b");
		SendWlCmd(L"country ALL");

		strCmd.Format(L"chanspec -c %d -b 2 -w 20 -s 0", dwChannel);
		SendWlCmd( strCmd.GetBuffer() );

		SendWlCmd(L"up");
		SendWlCmd(L"mpc 0");
		SendWlCmd(L"txant 0");
		SendWlCmd(L"antdiv 0");

		// b 일경우 11b, g/n 일경우 54b
		if( dwMode == 0 )
		{
			SendWlCmd(L"rateset 11b");
		}
		else
		{
			SendWlCmd(L"rateset 54b");
		}

		// n일경우에만 입력
		if( dwMode == 2 )
		{
			strCmd.Format(L"nrate -m %d -s 0", dwRate);
			SendWlCmd( strCmd.GetBuffer() );
		}
		
		SendWlCmd(L"join WillTxBtest imode adhoc");

		// b/g 일경우만 입력
		if( dwMode != 2 )
		{
			strCmd.Format(L"rate %d", dwRate);
			SendWlCmd( strCmd.GetBuffer() );
		}


		strCmd.Format(L"txpwr1 -q %d", dwPower);
		SendWlCmd( strCmd.GetBuffer() );

		SendWlCmd(L"up");

		Sleep(50);

		SendWlCmd(L"pkteng_start 00:90:4c:14:43:19 tx 40 1000 0");
		break;

	case IDC_RADIO_NON_MODULATION:
		

		SendWlCmd(L"out");
		strCmd.Format(L"fqacurcy %d", dwChannel);
		SendWlCmd(strCmd.GetBuffer());
		break;
	}


	SetCursor(hOldCursor);
}

void CFormView_Wireless::OnBnClickedButtonWlanRx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strCmd = L"";
	DWORD dwChannel = 0;



	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));



	dwChannel = GetDlgItemInt(IDC_EDIT_CHANNEL);
	strCmd.Format(L"channel %d", dwChannel);

	
	SendWlCmd(L"down");
	SendWlCmd(L"mpc 0");
	SendWlCmd(L"country ALL");
	SendWlCmd(L"legacylink 1");
	SendWlCmd(L"scansuppress 1");
	SendWlCmd( strCmd.GetBuffer() );
	SendWlCmd(L"bi 65535");
	SendWlCmd(L"up");
	SendWlCmd(L"join ee imode adhoc");



	SetCursor(hOldCursor);

}

BOOL CFormView_Wireless::SendWlCmd(LPTSTR strCmd)
{
	RETAILMSG(1, (TEXT("[PSH] %s\r\n"), strCmd));

	if( m_sei.lpFile == NULL )
	{
		m_sei.cbSize		= sizeof(SHELLEXECUTEINFO);
		m_sei.fMask			= 0;
		m_sei.hwnd			= NULL;
		m_sei.lpVerb		= NULL;
		m_sei.lpFile		= _T("\\Windows\\wl.exe");
		m_sei.lpDirectory	= NULL;
		m_sei.nShow			= SW_SHOW;
	}

	m_sei.lpParameters = strCmd;

	if(!ShellExecuteEx( &m_sei ))
	{
		MessageBox(L"ShellExecuteEx Failed", L"ERROR", MB_ICONERROR);
		return FALSE;
	}

	Sleep(10);

	return TRUE;
}

void CFormView_Wireless::OnCbnSelchangeComboMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_comboMode.GetCurSel() == 0 )
	{
		SetDlgItemInt(IDC_EDIT_RATE, 11);
	}
	else
	{
		SetDlgItemInt(IDC_EDIT_RATE, 54);
	}
}


void CFormView_Wireless::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	SendWlCmd(L"down");
	SendWlCmd(L"out");
}
