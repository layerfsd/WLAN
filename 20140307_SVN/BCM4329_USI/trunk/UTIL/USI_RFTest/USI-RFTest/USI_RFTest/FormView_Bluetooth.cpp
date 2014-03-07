// FormView_Bluetooth.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "USI_RFTest.h"
#include "FormView_Bluetooth.h"
#include "BTTestLib.h"


#define CHANNEL_NUM_MIN	(0)
#define CHANNEL_NUM_MAX	(78)


// CFormView_Bluetooth

IMPLEMENT_DYNCREATE(CFormView_Bluetooth, CFormView)

CFormView_Bluetooth::CFormView_Bluetooth()
	: CFormView(CFormView_Bluetooth::IDD)
{
	m_hBthPowerModule = NULL;
	m_bHopping = FALSE;
	m_hHoppingThread = NULL;
}

CFormView_Bluetooth::~CFormView_Bluetooth()
{
}

void CFormView_Bluetooth::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_CHANNEL, m_spinChannel);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
}

BEGIN_MESSAGE_MAP(CFormView_Bluetooth, CFormView)
	ON_EN_CHANGE(IDC_EDIT_CHANNEL, &CFormView_Bluetooth::OnEnChangeEditChannel)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CFormView_Bluetooth::OnBnClickedButtonTx)
	ON_BN_CLICKED(IDC_BUTTON_RX, &CFormView_Bluetooth::OnBnClickedButtonRx)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CFormView_Bluetooth::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_HOPPING, &CFormView_Bluetooth::OnBnClickedButtonHopping)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CFormView_Bluetooth 진단입니다.

#ifdef _DEBUG
void CFormView_Bluetooth::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormView_Bluetooth::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormView_Bluetooth 메시지 처리기입니다.

void CFormView_Bluetooth::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));







	m_spinChannel.SetRange(CHANNEL_NUM_MIN, CHANNEL_NUM_MAX);

	m_comboMode.AddString(L"Unmodulated");
	m_comboMode.AddString(L"PRBS9");
	m_comboMode.AddString(L"PRBS15");
	m_comboMode.AddString(L"ALL Zeroes");
	m_comboMode.AddString(L"ALL Ones");
	m_comboMode.AddString(L"Incrementing Symbols");

	m_comboMode.SetCurSel(0);
	

	m_comboType.AddString(L"GFSK");
	m_comboType.AddString(L"QPSK");
	m_comboType.AddString(L"8PSK");

	m_comboType.SetCurSel(0);
	

	GetDlgItem(IDC_BUTTON_HOPPING)->SetWindowText(L"Hopping Start");


	// bthpower On
	m_hBthPowerModule = (HMODULE)LoadLibrary(L"bthpower.dll"); 

	if( m_hBthPowerModule != NULL )
	{
		typedef BOOL(*PowerOn)(void);
		PowerOn bthPowerOn;
		
		bthPowerOn = (PowerOn)GetProcAddress(m_hBthPowerModule, L"PowerOn");
		
		bthPowerOn();
		Sleep(100);
	}


	SendBTCommand(_T("COM2: up"));


	SetCursor(hOldCursor);

}

void CFormView_Bluetooth::OnEnChangeEditChannel()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DWORD dwData = GetDlgItemInt(IDC_EDIT_CHANNEL);

	if( dwData < CHANNEL_NUM_MIN )
	{
		SetDlgItemInt(IDC_EDIT_CHANNEL, CHANNEL_NUM_MIN);
	}
	else if( dwData > CHANNEL_NUM_MAX )
	{
		SetDlgItemInt(IDC_EDIT_CHANNEL, CHANNEL_NUM_MAX);
	}
}

void CFormView_Bluetooth::OnBnClickedButtonTx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	DWORD dwChannel = 0;
	DWORD dwMode = 0;
	DWORD dwType = 0;
	CString strCommand = L"";
	TCHAR tszChannel[3] = L"";

	SendBTCommand(_T("COM2: reset"));
	Sleep(200);


	dwChannel = GetDlgItemInt(IDC_EDIT_CHANNEL);
	dwChannel += 2; // 커맨드로 전송시 0번채널은 2로 보낸다. (0번채널->2, 30번채널->32)

	_itot(dwChannel, tszChannel, 16);
	dwMode = m_comboMode.GetCurSel();
	dwType = m_comboType.GetCurSel();

	strCommand.Format(L"COM2: cmd 0x3f 0x0014 0x00 0x%s 0x%02d 0x%02d 0x09 0x00 0x00", tszChannel, dwMode, dwType);

	SendBTCommand(strCommand);

}

void CFormView_Bluetooth::OnBnClickedButtonRx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DWORD dwChannel = 0;
	CString strCommand = L"";
	TCHAR tszChannel[3] = L"";

	SendBTCommand(_T("COM2: reset"));
	Sleep(200);

	
	dwChannel = GetDlgItemInt(IDC_EDIT_CHANNEL);
	dwChannel += 2; // 커맨드로 전송시 0번채널은 2로 보낸다. (0번채널->2, 30번채널->32)

	_itot(dwChannel, tszChannel, 16);

	strCommand.Format(L"COM2: cmd 0x3f 0x002B 0x%s", tszChannel);

	SendBTCommand(strCommand);



}

void CFormView_Bluetooth::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendBTCommand(_T("COM2: reset"));
	Sleep(100);
}

void CFormView_Bluetooth::OnBnClickedButtonHopping()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	if( m_bHopping == TRUE )
	{
		m_bHopping = FALSE;

		GetDlgItem(IDC_BUTTON_HOPPING)->SetWindowText(L"Hopping Start");

		if( WaitForSingleObject(m_hHoppingThread, 1500) == WAIT_TIMEOUT )
		{
			TerminateThread(m_hHoppingThread, 0);
		}
		CloseHandle(m_hHoppingThread);
		m_hHoppingThread = NULL;

		
	}
	else
	{
		m_bHopping = TRUE;
		
		GetDlgItem(IDC_BUTTON_HOPPING)->SetWindowText(L"Hopping Stop");

		m_hHoppingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HoppingThreadHandler, this, 0, NULL);

		
	}

	SetCursor(hOldCursor);

}

void CFormView_Bluetooth::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	RETAILMSG(1, (TEXT("Destroy!\r\n") ));


	if( m_bHopping == TRUE )
	{
		m_bHopping = FALSE;

		if( WaitForSingleObject(m_hHoppingThread, 500) == WAIT_TIMEOUT )
		{
			TerminateThread(m_hHoppingThread, 0);
		}
		CloseHandle(m_hHoppingThread);
		m_hHoppingThread = NULL;
	}


	OnBnClickedButtonReset();


	// bthpower Off
	if( m_hBthPowerModule != NULL )
	{
		typedef BOOL(*PowerOff)(void);
		PowerOff bthPowerOff;
		
		bthPowerOff = (PowerOff)GetProcAddress(m_hBthPowerModule, L"PowerOff");

		bthPowerOff();
		FreeLibrary(m_hBthPowerModule);
	}
	
}

DWORD CFormView_Bluetooth::HoppingThreadHandler(CFormView_Bluetooth* hMain)
{
	hMain->HoppingThread();
	return 0;
}

void CFormView_Bluetooth::HoppingThread(void)
{
	DWORD dwChannel = CHANNEL_NUM_MIN + 2;
	DWORD dwMode = 0;
	DWORD dwType = 0;
	CString strCommand = L"";
	TCHAR tszChannel[3] = L"";

	while(m_bHopping)
	{
		SendBTCommand(_T("COM2: reset"));
		Sleep(100);

		dwMode = m_comboMode.GetCurSel();
		dwType = m_comboType.GetCurSel();

		_itot(dwChannel, tszChannel, 16);

		strCommand.Format(L"COM2: cmd 0x3f 0x0014 0x00 0x%s 0x%02d 0x%02d 0x09 0x00 0x00", tszChannel, dwMode, dwType);

		SendBTCommand(strCommand);

		dwChannel++;

		if( dwChannel > CHANNEL_NUM_MAX + 2 )
		{
			dwChannel = CHANNEL_NUM_MIN + 2;
		}

		Sleep(100);

	}
}

BOOL CFormView_Bluetooth::SendBTCommand(LPCTSTR str)
{
	TCHAR tszSep[] = L" ";
	TCHAR *token = NULL;
	DWORD dwArgc = 1;
	TCHAR tszCommand[128] = L"";
	TCHAR *tszArgv[32];


	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));


	wcscpy(tszCommand, str);

	token = _tcstok( tszCommand, tszSep );
	
	while( token != NULL )
	{
		tszArgv[dwArgc++] = token;
		token = _tcstok( NULL, tszSep );	
	}

	BTTestLibMain(dwArgc, tszArgv);


	SetCursor(hOldCursor);
	return TRUE;
}
