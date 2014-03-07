// NetSettingPage_Tools_PingView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Tools_PingView.h"



#define DEFAULT_LOG_FILE_NAME	L"BBS_PingResult.txt"

// CNetSettingPage_Tools_PingView

IMPLEMENT_DYNCREATE(CNetSettingPage_Tools_PingView, CFormView)

CNetSettingPage_Tools_PingView::CNetSettingPage_Tools_PingView()
	: CFormView(CNetSettingPage_Tools_PingView::IDD)
	, m_strPingResult(_T(""))
{

	memset(&m_mbi, 0, sizeof(SHMENUBARINFO));


	memset(&m_pingOption, 0, sizeof(BBS_PING_OPTION));
	memset(&m_pingResult, 0, sizeof(BBS_PING_RESULT));
	ZeroMemory(&m_adapterInfo, sizeof(BBS_ADAPTER_INFO));
	
	m_bIsPingOperating = FALSE;

	m_strResult = _T("");

	m_hPingThread = NULL;

	m_dwScreenRate = 1;

	m_strLogFilePath = L"";

}

CNetSettingPage_Tools_PingView::~CNetSettingPage_Tools_PingView()
{
}

void CNetSettingPage_Tools_PingView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PING_STATUS, m_editPingResult);
	DDX_Text(pDX, IDC_EDIT_PING_STATUS, m_strPingResult);
}

BEGIN_MESSAGE_MAP(CNetSettingPage_Tools_PingView, CFormView)
	ON_EN_CHANGE(IDC_EDIT_PING_SENDSIZE, &CNetSettingPage_Tools_PingView::OnEnChangeEditPingSendsize)
	ON_EN_CHANGE(IDC_EDIT_PING_TTL, &CNetSettingPage_Tools_PingView::OnEnChangeEditPingTtl)
	ON_EN_CHANGE(IDC_EDIT_PING_TOS, &CNetSettingPage_Tools_PingView::OnEnChangeEditPingTos)
	ON_EN_CHANGE(IDC_EDIT_PING_TIMEOUT, &CNetSettingPage_Tools_PingView::OnEnChangeEditPingTimeout)
	ON_BN_CLICKED(IDC_BUTTON_PING, &CNetSettingPage_Tools_PingView::OnBnClickedButtonPing)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_PING_FILELOG, &CNetSettingPage_Tools_PingView::OnBnClickedButtonPingFilelog)
END_MESSAGE_MAP()


// CNetSettingPage_Tools_PingView 진단입니다.

#ifdef _DEBUG
void CNetSettingPage_Tools_PingView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetSettingPage_Tools_PingView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNetSettingPage_Tools_PingView 메시지 처리기입니다.

void CNetSettingPage_Tools_PingView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_PingDlg Oninit \r\n") ));

	// Empty MenuBar
	
	m_mbi.cbSize = sizeof( SHMENUBARINFO );
	m_mbi.hwndParent = m_hWnd;
	m_mbi.dwFlags = SHCMBF_EMPTYBAR;
	m_mbi.hInstRes = AfxGetInstanceHandle();
	SHCreateMenuBar( &m_mbi );


	m_dwScreenRate = (GetSystemMetrics(SM_CXSCREEN) == 480)? 2 : 1;

	// EditBox Initialize
	m_editPingResult.SetFont(NULL, FALSE);
	m_fontPingResult.DeleteObject();
	m_fontPingResult.CreateFont(11 * m_dwScreenRate, 0, 0, 0, FW_DONTCARE, FALSE /*italic*/, FALSE /*underline*/, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Tahoma")) ;
	m_editPingResult.SetFont(&m_fontPingResult);

	// 문자제한 무제한
	m_editPingResult.SetLimitText(-1);


	// Set Default Value

	m_pingOption.dwSendBufSize = DEFAULT_SEND_SIZE;
	m_pingOption.dwTimeOut = DEFAULT_TIMEOUT;
	m_pingOption.ucTos = DEFAULT_TOS;
	m_pingOption.ucTtl = DEFAULT_TTL;

	SetDlgItemInt(IDC_EDIT_PING_SENDSIZE, m_pingOption.dwSendBufSize, FALSE);
	SetDlgItemInt(IDC_EDIT_PING_TTL, m_pingOption.ucTtl, FALSE);
	SetDlgItemInt(IDC_EDIT_PING_TOS, m_pingOption.ucTos, FALSE);
	SetDlgItemInt(IDC_EDIT_PING_TIMEOUT, m_pingOption.dwTimeOut, FALSE);

	
	// Set default Gateway
	GetWlanDefaultInformation(&m_adapterInfo);
	SetDlgItemText(IDC_EDIT_PING_DEST_ADDRESS, m_adapterInfo.tszDefaultGateWay);




	// For File Log

	// File Log Path
	TCHAR tszPSName[128] = L"";

	GetFlashStoreName(tszPSName);


	m_strLogFilePath = tszPSName;
	m_strLogFilePath += L"\\";
	m_strLogFilePath += L"System";
	m_strLogFilePath += L"\\";
	m_strLogFilePath += L"Log";
	m_strLogFilePath += L"\\";
	m_strLogFilePath += DEFAULT_LOG_FILE_NAME;

	
	SetDlgItemText(IDC_EDIT_PING_FILELOG_PATH, m_strLogFilePath.GetBuffer());


}


void CNetSettingPage_Tools_PingView::PingThreadHandler(CNetSettingPage_Tools_PingView* context)
{
	context->PingThread();
}

void CNetSettingPage_Tools_PingView::PingThread()
{
	DWORD dwTotalCount = 0;
	DWORD dwSuccessCount = 0;
	DWORD dwFailedCount = 0;

	int nRSSI = 0;
	TCHAR tszSSID[64] = {0,};

	while( m_bIsPingOperating == TRUE )
	{
		if( m_ping.StartPing(&m_pingResult) == TRUE )
		{
			m_strResult.Format(_T("[%d] Size(%d),Time(%dms),TTL(%d)\r\n"),dwTotalCount, m_pingResult.dwDataSize, m_pingResult.dwTime, m_pingResult.ucTtl );
			dwSuccessCount++;
		}
		else
		{
			if( m_pingResult.dwErrCode == 11010 )
			{
				m_strResult.Format(_T("Request timed out.\r\n") );
			}
			else
			{
				m_strResult.Format(_T("Error (%d) \r\n"), m_pingResult.dwErrCode);
			}

			dwFailedCount++;
		}

		m_editPingResult.ReplaceSel(m_strResult, TRUE);

		dwTotalCount = dwSuccessCount + dwFailedCount;

		
		GetSSID(m_adapterInfo.tszAdapterName, tszSSID);
		if( _tcslen(tszSSID) != 0)
		{
			GetRSSI(m_adapterInfo.tszAdapterName, &nRSSI);
		}
		

		SetDlgItemInt(IDC_STATIC_RSSI, nRSSI);
		SetDlgItemText(IDC_STATIC_SSID, tszSSID);

		Sleep(1000);



		
	}

	m_strResult.Format(_T("Total/Success/Failed(%d/%d/%d) \r\n"),dwTotalCount, dwSuccessCount, dwFailedCount );
	m_editPingResult.ReplaceSel(m_strResult, TRUE);

}



void CNetSettingPage_Tools_PingView::EnableOptionControls()
{
	GetDlgItem(IDC_EDIT_PING_SENDSIZE)		->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PING_TTL)			->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PING_TOS)			->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PING_TIMEOUT)		->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PING_DEST_ADDRESS)	->EnableWindow(TRUE);
}

void CNetSettingPage_Tools_PingView::DisableOptionControls()
{
	GetDlgItem(IDC_EDIT_PING_SENDSIZE)		->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PING_TTL)			->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PING_TOS)			->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PING_TIMEOUT)		->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PING_DEST_ADDRESS)	->EnableWindow(FALSE);
	
}



void CNetSettingPage_Tools_PingView::OnEnChangeEditPingSendsize()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DWORD dwValue = GetDlgItemInt(IDC_EDIT_PING_SENDSIZE);
	if( dwValue < 0)
	{
		SetDlgItemInt(IDC_EDIT_PING_SENDSIZE, 0);
	}
	else if( dwValue > 0xfff7 )
	{
		SetDlgItemInt(IDC_EDIT_PING_SENDSIZE, 0xfff7);
	}



}

void CNetSettingPage_Tools_PingView::OnEnChangeEditPingTtl()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DWORD dwValue = GetDlgItemInt(IDC_EDIT_PING_TTL);
	if( dwValue < 0)
	{
		SetDlgItemInt(IDC_EDIT_PING_TTL, 0);
	}
	else if( dwValue > 0xff )
	{
		SetDlgItemInt(IDC_EDIT_PING_TTL, 0xff);
	}

}

void CNetSettingPage_Tools_PingView::OnEnChangeEditPingTos()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DWORD dwValue = GetDlgItemInt(IDC_EDIT_PING_TOS);
	if( dwValue < 0)
	{
		SetDlgItemInt(IDC_EDIT_PING_TOS, 0);
	}
	else if( dwValue > 0xff )
	{
		SetDlgItemInt(IDC_EDIT_PING_TOS, 0xff);
	}

}

void CNetSettingPage_Tools_PingView::OnEnChangeEditPingTimeout()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DWORD dwValue = GetDlgItemInt(IDC_EDIT_PING_TIMEOUT);
	if( dwValue < 0)
	{
		SetDlgItemInt(IDC_EDIT_PING_TIMEOUT, 0);
	}

}

void CNetSettingPage_Tools_PingView::OnBnClickedButtonPing()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR tszLiteralIP[256] = {'\0',};
	DWORD dwCode = 0;

	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));



	if(m_bIsPingOperating == TRUE)
	{
		m_bIsPingOperating = FALSE;

		GetExitCodeThread(m_hPingThread, &dwCode);

		if( dwCode == STILL_ACTIVE )
		{

			RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnBnClickedButtonPing() dwCode : %d \r\n"), dwCode ));

			if( WaitForSingleObject(m_hPingThread, 2000) == WAIT_TIMEOUT )
			{
				RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnBnClickedButtonPing() Terminate \r\n") ));
				TerminateThread(m_hPingThread, 0);
			}
		}

		CloseHandle(m_hPingThread);

		m_ping.DeinitPing();

		// Change Control State
		GetDlgItem(IDC_BUTTON_PING)	->SetWindowText( _T("Start") );
		EnableOptionControls();


		SetDlgItemText(IDC_STATIC_RSSI, L"");
		SetDlgItemText(IDC_STATIC_SSID, L"");

		
	}
	else
	{
		CString strIP = _T("");

		m_editPingResult.ReplaceSel(_T(""));

		m_pingOption.dwSendBufSize = GetDlgItemInt(IDC_EDIT_PING_SENDSIZE);
		m_pingOption.ucTtl = GetDlgItemInt(IDC_EDIT_PING_TTL);
		m_pingOption.ucTos = GetDlgItemInt(IDC_EDIT_PING_TOS);
		m_pingOption.dwTimeOut = GetDlgItemInt(IDC_EDIT_PING_TIMEOUT);
		GetDlgItemText(IDC_EDIT_PING_DEST_ADDRESS, strIP);


		_tcscpy(m_pingOption.tszDestIP, strIP.GetBuffer());


		if( m_ping.InitPing(m_pingOption, tszLiteralIP) == FALSE )
		{
			m_strResult = _T("InitPing Failed! \r\n");
			m_editPingResult.ReplaceSel(m_strResult, TRUE);
		}
		else
		{
			m_bIsPingOperating = TRUE;

			
			// EditBox Clear
			m_editPingResult.SetSel(0, -1);
			m_editPingResult.Clear();
			m_editPingResult.ReplaceSel(_T(""));


			m_strResult.Format(_T("Host : %s[%s] \r\n"),m_pingOption.tszDestIP , tszLiteralIP);
			m_editPingResult.ReplaceSel(m_strResult, TRUE);

		
			m_hPingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PingThreadHandler, this, 0, NULL);
			
			if( m_hPingThread != NULL )
			{
				// Change Control State
				GetDlgItem(IDC_BUTTON_PING)	->SetWindowText( _T("Stop") );
				DisableOptionControls();	
			}
			else
			{
				m_strResult.Format(_T("Create PingThread Error!\r\n") );
				m_editPingResult.ReplaceSel(m_strResult, TRUE);
			}
			
		}
	}




	SetCursor(hOldCursor);


}

void CNetSettingPage_Tools_PingView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_PingView::OnDestroy() \r\n") ));

	m_ping.DeinitPing();


	// Ping Thread 종료 -> 2초 기다려보고 그래도 종료 안되면 강제종료
	if( m_bIsPingOperating == TRUE || m_hPingThread != NULL )
	{

		m_bIsPingOperating = FALSE;

		if( WaitForSingleObject(m_hPingThread, 2000) != WAIT_OBJECT_0 )
		{
			RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_PingDlg WaitForSingleObject(!WAIT_OBJECT_0) \r\n") ));

			TerminateThread(m_hPingThread, 0);
			CloseHandle(m_hPingThread);
		}
	}


}

void CNetSettingPage_Tools_PingView::OnBnClickedButtonPingFilelog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));


	DWORD dwLen = 0;
	char *pLogBuffer = NULL;
	HANDLE hLogFile = NULL;
	
	// 덤프전 필요한 로그 추가
	BBS_ADAPTER_INFO adapterInfo = {0,};
	SYSTEMTIME systime = {0,};
	CString strTemp = L"";
	CString strExtraLog = L"";

	GetLocalTime(&systime);

	GetWlanDefaultInformation(&adapterInfo);


	strTemp.Format(L"================================ Output Ping Log(%04d/%02d/%02d %02d:%02d:%02d) ================================\r\n", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);

	strExtraLog = L"=================================================================================================\r\n";
	strExtraLog += strTemp;
	strExtraLog += L"=================================================================================================\r\n";
	
	strTemp.Format(L"IP : %s \r\n", adapterInfo.tszIPAddr );
	strExtraLog += strTemp;

	strTemp.Format(L"Subnet : %s \r\n", adapterInfo.tszSubNetMask );
	strExtraLog += strTemp;

	strTemp.Format(L"GateWay : %s \r\n", adapterInfo.tszDefaultGateWay );
	strExtraLog += strTemp;

	strTemp.Format(L"DNS : %s \r\n", adapterInfo.tszDnsAddr );
	strExtraLog += strTemp;

	strTemp.Format(L"DHCP : %d \r\n", adapterInfo.bDHCPEnabled );
	strExtraLog += strTemp;
	

	CreateFolderIfNotFoundDir( m_strLogFilePath.GetBuffer() );


	// Save ExtraLog
	dwLen = strExtraLog.GetLength();

	pLogBuffer = new char[dwLen];


	hLogFile = CreateFile(m_strLogFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if( hLogFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwBytesReturned = 0;
		
		wcstombs(pLogBuffer, strExtraLog.GetBuffer(), dwLen );

		SetFilePointer(hLogFile, 0, NULL, FILE_END);
		if( !WriteFile(hLogFile, pLogBuffer, strlen(pLogBuffer), &dwBytesReturned, NULL) )
		{
			// ERROR
		}

		CloseHandle(hLogFile);
	}

	delete pLogBuffer;







	// Save MainLog
	UpdateData();
	
	dwLen = m_strPingResult.GetLength();

	pLogBuffer = new char[dwLen];
	


	hLogFile = CreateFile(m_strLogFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if( hLogFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwBytesReturned = 0;
		
		wcstombs(pLogBuffer, m_strPingResult.GetBuffer(), dwLen );

		SetFilePointer(hLogFile, 0, NULL, FILE_END);
		if( !WriteFile(hLogFile, pLogBuffer, strlen(pLogBuffer), &dwBytesReturned, NULL) )
		{
			// ERROR
		}

		CloseHandle(hLogFile);
	}

	delete pLogBuffer;


	SetCursor(hOldCursor);
}
