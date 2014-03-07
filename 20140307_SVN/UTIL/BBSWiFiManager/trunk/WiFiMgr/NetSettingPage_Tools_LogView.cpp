// NetSettingPage_Tools_LogView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Tools_LogView.h"




#define DEFAULT_LOG_FILE_NAME	L"BBS_WiFiMgrLog.txt"




// CNetSettingPage_Tools_LogView

IMPLEMENT_DYNCREATE(CNetSettingPage_Tools_LogView, CFormView)

CNetSettingPage_Tools_LogView::CNetSettingPage_Tools_LogView()
	: CFormView(CNetSettingPage_Tools_LogView::IDD)
	, m_strLogData(_T(""))
{
	m_dwScreenRate = 1;
	hLogMonitorThread = NULL;
	m_bMonitoring = FALSE;
	hLogEvent = NULL;

	m_strLogFilePath = L"";
}

CNetSettingPage_Tools_LogView::~CNetSettingPage_Tools_LogView()
{
}

void CNetSettingPage_Tools_LogView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
	DDX_Text(pDX, IDC_EDIT_LOG, m_strLogData);
}

BEGIN_MESSAGE_MAP(CNetSettingPage_Tools_LogView, CFormView)
	ON_WM_DESTROY()
	ON_COMMAND(ID_DEFAULT, &CNetSettingPage_Tools_LogView::OnDefault)
	ON_COMMAND(ID_APPLY, &CNetSettingPage_Tools_LogView::OnApply)
	ON_BN_CLICKED(IDC_BUTTON_DUMP, &CNetSettingPage_Tools_LogView::OnBnClickedButtonDump)
END_MESSAGE_MAP()


// CNetSettingPage_Tools_LogView 진단입니다.

#ifdef _DEBUG
void CNetSettingPage_Tools_LogView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetSettingPage_Tools_LogView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNetSettingPage_Tools_LogView 메시지 처리기입니다.

void CNetSettingPage_Tools_LogView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// Apply MenuBar
	SHMENUBARINFO mbi;
	memset(&mbi, 0, sizeof(SHMENUBARINFO));
	mbi.cbSize = sizeof( SHMENUBARINFO );
	mbi.hwndParent = m_hWnd;
	mbi.dwFlags = SHCMBF_HMENU;
	mbi.nToolBarId = IDR_MENU_APPLY_DEFAULT;
	mbi.hInstRes = AfxGetInstanceHandle();
	SHCreateMenuBar( &mbi );


	m_dwScreenRate = (GetSystemMetrics(SM_CXSCREEN) == 480)? 2 : 1;

	// EditBox Font
	m_editLog.SetFont(NULL, FALSE);
	m_fontEditBox.DeleteObject();
	m_fontEditBox.CreateFont(11 * m_dwScreenRate, 0, 0, 0, FW_DONTCARE, FALSE /*italic*/, FALSE /*underline*/, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Tahoma")) ;
	m_editLog.SetFont(&m_fontEditBox);

	// 문자제한 무제한
	m_editLog.SetLimitText(-1);

	DWORD dwLogSize = GetLogBufSize();
	

	SetDlgItemInt(IDC_EDIT_LOG_SIZE, dwLogSize / 1024);
	

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

	
	SetDlgItemText(IDC_EDIT_FILELOG_PATH, m_strLogFilePath.GetBuffer());


	hLogEvent = CreateEvent(NULL, FALSE, FALSE, EVENT_LOG_UPDATE);
	hLogMonitorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LogMonitorThreadHandler, this, 0, NULL);


	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnInitialUpdate() \r\n") ));
}

void CNetSettingPage_Tools_LogView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	DWORD dwCode = 0;

	m_bMonitoring = FALSE;
	SetEvent(hLogEvent);


	GetExitCodeThread(hLogMonitorThread, &dwCode);

	if( dwCode == STILL_ACTIVE )
	{

		RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnDestroy() dwCode : %d \r\n"), dwCode ));

		if( WaitForSingleObject(hLogMonitorThread, 500) == WAIT_TIMEOUT )
		{
			RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnDestroy() Terminate \r\n") ));
			TerminateThread(hLogMonitorThread, 0);
		}
	}

	CloseHandle(hLogEvent);
	CloseHandle(hLogMonitorThread);

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnDestroy() \r\n") ));
}



void CNetSettingPage_Tools_LogView::OnDefault()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	SetDlgItemInt(IDC_EDIT_LOG_SIZE, DEFAULT_LOGBUF_SIZE);
}

void CNetSettingPage_Tools_LogView::OnApply()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	DWORD dwLogSize = 0;

	if( MessageBox(L"Logs will be deleted, It's OK?", L"Warning", MB_YESNO) == IDYES )
	{
		dwLogSize = GetDlgItemInt(IDC_EDIT_LOG_SIZE) * 1024;

		SetLogBufSize(dwLogSize);
	}
}



void CNetSettingPage_Tools_LogView::LogMonitorThreadHandler(CNetSettingPage_Tools_LogView* context)
{
	context->LogMonitorThread();
}

void CNetSettingPage_Tools_LogView::LogMonitorThread()
{
	
	m_bMonitoring = TRUE;


	m_editLog.SetSel( 0, -1 );
	m_editLog.ReplaceSel( GetLogData() );

	while(m_bMonitoring)
	{
		WaitForSingleObject(hLogEvent, INFINITE);
		
		RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::LogMonitorThread() GetLog! \r\n") ));

		// 스레드가 끝날땐, LogData 얻어올 필요가 없다. 괜히 딜레이 걸리기 때문에, 여기서 한번더 체크후 종료
		if(m_bMonitoring == FALSE)
		{
			break;
		}
		
		// Update
		
		m_editLog.SetSel( 0, -1 );
		m_editLog.ReplaceSel( GetLogData() );
		
		Sleep(100);
	}
	
	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::LogMonitorThread() End \r\n") ));

}



void CNetSettingPage_Tools_LogView::OnBnClickedButtonDump()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// 생성하려는 전체 Path 중 없는 폴더가 있으면, 폴더 생성해주는 함수 호출 (없는 폴더에 파일 생성시 오류발생)

	HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	DWORD dwLen = 0;
	char *pLogBuffer = NULL;
	HANDLE hLogFile = NULL;
	
	// 덤프전 필요한 로그 추가
	BBS_ADAPTER_INFO adapterInfo = {0,};
	SYSTEMTIME systime = {0,};
	CString strFirmware = L"";
	CString strDriver = L"";
	CString strSDIODrv = L"";
	CString strNetui = L"";
	CString strWiFiSvcMgr = L"";
	CString strTemp = L"";
	CString strExtraLog = L"";

	GetLocalTime(&systime);

	GetWlanDefaultInformation(&adapterInfo);

	GetFirmwareVersion(adapterInfo.tszAdapterName, strFirmware);
	GetDriverVersion(adapterInfo.tszAdapterName, strDriver);
	GetSDIODrvFileVersion(strSDIODrv);
	GetnetuiFileVersion(strNetui);
	GetWiFiSvcMgrFileVersion(strWiFiSvcMgr);


	// Export 시작알림(날짜/시간)
	strTemp.Format(L"================================ Output File Log(%04d/%02d/%02d %02d:%02d:%02d) ================================\r\n", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);

	strExtraLog = L"=================================================================================================\r\n";
	strExtraLog += strTemp;
	strExtraLog += L"=================================================================================================\r\n\r\n";
	

	// 아답터, 버전정보
	strTemp.Format(L"Adapter : %s \r\n", adapterInfo.tszAdapterName );
	strExtraLog += strTemp;

	strTemp.Format(L"Firmware : %s \r\n", strFirmware.GetBuffer() );
	strExtraLog += strTemp;

	strTemp.Format(L"Driver : %s \r\n", strDriver.GetBuffer() );
	strExtraLog += strTemp;

	strTemp.Format(L"SDIODrv : %s \r\n", strSDIODrv.GetBuffer() );
	strExtraLog += strTemp;

	strTemp.Format(L"BBSWiFIMgr : %s \r\n", strNetui.GetBuffer() );
	strExtraLog += strTemp;

	strTemp.Format(L"WiFiSvcMgr : %s \r\n", strWiFiSvcMgr.GetBuffer() );
	strExtraLog += strTemp;


	// IP정보
	strTemp.Format(L"IP : %s \r\n", adapterInfo.tszIPAddr );
	strExtraLog += strTemp;
	strTemp.Format(L"DHCP : %d \r\n", adapterInfo.bDHCPEnabled );
	strExtraLog += strTemp;
	strTemp.Format(L"Gateway : %s \r\n", adapterInfo.tszDefaultGateWay );
	strExtraLog += strTemp;

	// AP 정보
	BYTE pucBSSID[6] = {0,};
	TCHAR tszSSID[32] = {0,};

	if( GetSSID(adapterInfo.tszAdapterName, tszSSID) )
	{
		strTemp.Format(L"SSID : %s \r\n", tszSSID );
		strExtraLog += strTemp;
	}

	if( GetBSSID(adapterInfo.tszAdapterName, pucBSSID) )
	{
		strTemp.Format(L"BSSID : [%02X:%02X:%02X:%02X:%02X:%02X] \r\n", pucBSSID[0],pucBSSID[1],pucBSSID[2],pucBSSID[3],pucBSSID[4],pucBSSID[5] );
		strExtraLog += strTemp;
	}


	strExtraLog += L"=================================================================================================\r\n";
	// 주변 SSID 정보
	// Get BSSID LIST Information
	NDIS_WLAN_BSSID ndisWlanBssid[128] = {0,};
	DWORD dwNumofItems = 0;

	if( GetBBSIDListInfo(adapterInfo.tszAdapterName, ndisWlanBssid, &dwNumofItems) == TRUE )
	{
		TCHAR tszTemp[32] = L"";

		if( dwNumofItems > 0 )
		{
			for(DWORD dwIdx = 0; dwIdx<dwNumofItems; dwIdx++)
			{

				// SSID
				mbstowcs(tszTemp, (char*)ndisWlanBssid[dwIdx].Ssid.Ssid, ndisWlanBssid[dwIdx].Ssid.SsidLength);
				strTemp.Format(L"%s_%d_", tszTemp, ndisWlanBssid[dwIdx].Rssi );
				strExtraLog += strTemp;

				// BSSID
				wsprintf(tszTemp, L"[%02X:%02X:%02X:%02X:%02X:%02X] \r\n", ndisWlanBssid[dwIdx].MacAddress[0], ndisWlanBssid[dwIdx].MacAddress[1], ndisWlanBssid[dwIdx].MacAddress[2], ndisWlanBssid[dwIdx].MacAddress[3], ndisWlanBssid[dwIdx].MacAddress[4], ndisWlanBssid[dwIdx].MacAddress[5] );
				strExtraLog += tszTemp;

				memset(tszTemp, 0x00, sizeof(tszTemp));
				strTemp = L"";
			}
		}
	}
	

	// 설치된 프로그램 리스트 얻기
	strExtraLog += L"=================================================================================================\r\n";

	WIN32_FIND_DATA wfd = {0,};
	HANDLE  hFind = NULL;

	hFind = FindFirstFile(_T("\\Windows\\AppMgr\\*"), &wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{

		do
		{
			if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				SYSTEMTIME sysTime = {0,};

				// . 과 ..폴더 이름은 Skip
				if( (_tcscmp(wfd.cFileName, L".") == 0) || (_tcscmp(wfd.cFileName, L"..") == 0) )
				{
					continue;
				}

				FileTimeToSystemTime(&wfd.ftCreationTime, &sysTime);
				strTemp.Format(L"%s - [%04d/%02d/%02d, %02d:%02d:%02d]\r\n", wfd.cFileName, sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
				strExtraLog += strTemp;
			}

		}while( FindNextFile(hFind, &wfd) );


		FindClose(hFind);
	}
	strExtraLog += L"=================================================================================================\r\n\r\n";








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
	
	dwLen = m_strLogData.GetLength();

	pLogBuffer = new char[dwLen];
	


	hLogFile = CreateFile(m_strLogFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if( hLogFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwBytesReturned = 0;
		
		wcstombs(pLogBuffer, m_strLogData.GetBuffer(), dwLen );

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
