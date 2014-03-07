// PingCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PingCheck.h"
#include "PingCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 파일저장소 경로
#define RK_FLASH_STORE_PATH				L"Drivers\\BuiltIn\\FlshDrv"
#define RV_FLASH_STORE_NAME				L"FolderName"

#define LOG_FILE_NAME					L"PingCheck_Log.txt"


#define RK_PING_CHECK					L"SOFTWARE\\Bluebird\\PingCheck"

#define RV_CHECK_N_BREAK_INTERVAL		L"CheckAndBreakInterval"
#define RV_PING_INTERVAL				L"PingInterval"
#define RV_MAX_CHECK_COUNT				L"MaxCheckCount"
#define RV_PREV_DEST_IP					L"PrevCheckDestIP"
#define RV_CUSTOM_DEST_IP				L"CustomDestIP"

#define RV_ENABLE_FILE_LOG				L"EnableFileLog"

#define DEFAULT_CHECK_N_BREAK_INTERVAL	(90)
#define DEFAULT_PING_INTERVAL			(5)
#define DEFAULT_MAX_CHECK_COUNT			(5)



// CPingCheckDlg dialog

CPingCheckDlg::CPingCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPingCheckDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_dwCheckNBreakInterval = 0;
	m_dwPingInterval = 0;
	m_dwMaxCheckCount = 0;

	m_bPinging = FALSE;
	m_bEnableFileLog = FALSE;


	hPingThread = NULL;
	
	m_pPing = NULL;

	memset( m_tszPrevCheckDestIP, L'\0', sizeof(m_tszPrevCheckDestIP) );
	memset( m_tszCustomDestIP, L'\0', sizeof(m_tszCustomDestIP) );
	memset(&m_adapterInfo, 0, sizeof(BBS_ADAPTER_INFO));


	m_strLogFilePath = _T("");
	
}

void CPingCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPingCheckDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_MESSAGE(WM_STOP_CHECK, OnStopCheck)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPingCheckDlg message handlers

BOOL CPingCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	DWORD dwType = 0;
	DWORD dwData = 0;
	DWORD dwDataSize = 0;
	TCHAR tszData[128] = {'\0',};
	LONG lRet = 0;


	HKEY hKey = NULL;
	DWORD dwDisposition = 0;



	// Get Log Folder Name
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, RK_FLASH_STORE_PATH, 0, 0, &hKey) == ERROR_SUCCESS )
	{
		TCHAR tszData[128] = {'\0',};
		dwDataSize = sizeof(tszData);
		lRet = RegQueryValueEx(hKey, RV_FLASH_STORE_NAME, NULL, NULL, (LPBYTE)&tszData, &dwDataSize);


		if( (lRet != ERROR_SUCCESS) || ( _tcslen(tszData) <= 0 ) )
		{
			m_strLogFilePath = LOG_FILE_NAME;
		}
		else
		{
			m_strLogFilePath = tszData;
			m_strLogFilePath += _T("\\");
			m_strLogFilePath += LOG_FILE_NAME;
		}

		RegCloseKey(hKey);
		hKey = NULL;
	}



	

	if( RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_PING_CHECK, 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS )
	{
		

		//
		// CHECK FILE LOG ENABLE
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, RV_ENABLE_FILE_LOG, NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);

		if( (lRet == ERROR_SUCCESS) && (dwData==1) )
		{
			m_bEnableFileLog = TRUE;
		}
		else
		{
			m_bEnableFileLog = FALSE;
			RegSetValueEx(hKey, RV_ENABLE_FILE_LOG, 0, REG_DWORD, (LPBYTE)&m_bEnableFileLog, sizeof(m_bEnableFileLog) );
		}



		PingCheckDbgMsg(1, _T("\r\n\r\n========== Ping Check Program Start! ==========\r\n\r\n") );

		//
		// CHECK_N_BREAK_INTERVAL
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, RV_CHECK_N_BREAK_INTERVAL, NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);

		if( (lRet != ERROR_SUCCESS) || (dwData<=0) )
		{
			m_dwCheckNBreakInterval = DEFAULT_CHECK_N_BREAK_INTERVAL;
			RegSetValueEx(hKey, RV_CHECK_N_BREAK_INTERVAL, 0, REG_DWORD, (LPBYTE)&m_dwCheckNBreakInterval, sizeof(m_dwCheckNBreakInterval) );
		}
		else
		{
			m_dwCheckNBreakInterval = dwData;
		}
		


		//
		// PING_INTERVAL
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, RV_PING_INTERVAL, NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);

		if( (lRet != ERROR_SUCCESS) || (dwData<=0) )
		{
			m_dwPingInterval = DEFAULT_PING_INTERVAL;
			RegSetValueEx(hKey, RV_PING_INTERVAL, 0, REG_DWORD, (LPBYTE)&m_dwPingInterval, sizeof(m_dwPingInterval) );
		}
		else
		{
			m_dwPingInterval = dwData;
		}





		//
		// MAX_CHECK_COUNT
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, RV_MAX_CHECK_COUNT, NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);

		if( (lRet != ERROR_SUCCESS) || (dwData<=0) )
		{
			m_dwMaxCheckCount = DEFAULT_MAX_CHECK_COUNT;
			RegSetValueEx(hKey, RV_MAX_CHECK_COUNT, 0, REG_DWORD, (LPBYTE)&m_dwMaxCheckCount, sizeof(m_dwMaxCheckCount) );
		}
		else
		{
			m_dwMaxCheckCount = dwData;
		}

		


		//
		// PREV_DEST_IP
		//
		RegSetValueEx(hKey, RV_PREV_DEST_IP, 0, REG_SZ, (LPBYTE) m_tszPrevCheckDestIP, _tcslen(m_tszPrevCheckDestIP) * sizeof(TCHAR) );



		//
		// CUSTOM_DEST_IP
		//
		dwDataSize = sizeof(tszData);
		lRet = RegQueryValueEx(hKey, RV_CUSTOM_DEST_IP, NULL, NULL, (LPBYTE)&tszData, &dwDataSize);

		if( (lRet != ERROR_SUCCESS) || ( _tcslen(tszData) <= 0 ) )
		{
			memset( m_tszCustomDestIP, L'\0', sizeof(m_tszCustomDestIP) );
			RegSetValueEx(hKey, RV_CUSTOM_DEST_IP, 0, REG_SZ, (LPBYTE) m_tszCustomDestIP, _tcslen(m_tszCustomDestIP) * sizeof(TCHAR) );
		}
		else
		{
			wcscpy(m_tszCustomDestIP, tszData);
		}

		RegCloseKey(hKey);




		PingCheckDbgMsg(1, _T("---------- [Option Value] ---------- \r\n") );
		PingCheckDbgMsg(1, _T("CheckNBreakInterval(%d), PingInterval(%d) \r\n"), m_dwCheckNBreakInterval, m_dwPingInterval );
		PingCheckDbgMsg(1, _T("MaxCheckCount(%d), CustomDestIP(%s) \r\n"), m_dwMaxCheckCount, m_tszCustomDestIP );
		PingCheckDbgMsg(1, _T("---------------------------------------- \r\n\r\n") );



		m_adapterInfo.tszAdapterName[0] = L'\0';

		GetWlanDefaultInformation(&m_adapterInfo);

			
		// 기본정보 출력

		PingCheckDbgMsg(1, _T("---------- [Default Information] ---------- \r\n") );
		PingCheckDbgMsg(1, _T("Adapter(%s), HostName(%s), DHCP(%d) \r\n"), m_adapterInfo.tszAdapterName, m_adapterInfo.tszHostName, m_adapterInfo.bDHCPEnabled );
		PingCheckDbgMsg(1, _T("IPAddr(%s) \r\n"), m_adapterInfo.tszIPAddr );
		PingCheckDbgMsg(1, _T("DefaultGateWay(%s) \r\n"), m_adapterInfo.tszDefaultGateWay );
		PingCheckDbgMsg(1, _T("DnsAddr(%s) \r\n"), m_adapterInfo.tszDnsAddr );
		PingCheckDbgMsg(1, _T("---------------------------------------- \r\n\r\n") );
		


		
		
		m_bPinging = TRUE;
		m_pPing = new CBBS_Ping();

		hPingThread = CreateThread(NULL, 0, PingCheckThreadHandler, this, 0, NULL);

	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CPingCheckDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_PINGCHECK_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_PINGCHECK_DIALOG));
	}
}
#endif

void CPingCheckDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.


	// PingCheck 프로그램은 항상 BackGround로 동작
	ShowWindow(SW_HIDE);
	ShowWindow(SW_MINIMIZE);
	
	

}

DWORD CPingCheckDlg::PingCheckThreadHandler(LPVOID pvarg)
{
	CPingCheckDlg* temp = (CPingCheckDlg*)pvarg;
	temp->PingCheckThread();

	return 0;
}

void CPingCheckDlg::PingCheckThread(void)
{
	BBS_PING_OPTION pingOpt;
	BBS_PING_RESULT pingResult;
	DWORD dwPingFailCount = 0;

	BYTE pucMacAddr[6] = {'\0',};
	TCHAR tszMacAddr[18] = {'\0',};

	TCHAR tszLiteralIP[18] = {'\0',};

	memset(&pingOpt, 0, sizeof(BBS_PING_OPTION));
	memset(&pingResult, 0, sizeof(BBS_PING_RESULT));


	PingCheckDbgMsg(1, _T("===== [PingCheckThread Start] ===== \r\n") );

	while(m_bPinging == TRUE)
	{

		Sleep(m_dwCheckNBreakInterval * 1000);

		// PIng 실패 횟수 초기화 필수!
		dwPingFailCount = 0;


		GetWlanDefaultInformation(&m_adapterInfo);


		// 아답터가 검색되었는지?
		if( _tcslen(m_adapterInfo.tszAdapterName) == 0 )
		{
			// 아답터가 검색되지 않았다면, 기본 대기시간만큼 다시 대기
			PingCheckDbgMsg(1, _T("[ERROR] Can't Find Adapter! \r\n") );
			continue;
		}

		GetBSSIDInfo(m_adapterInfo.tszAdapterName, pucMacAddr);

		wsprintf(tszMacAddr, L"%02X:%02X:%02X:%02X:%02X:%02X", pucMacAddr[0], pucMacAddr[1], pucMacAddr[2], pucMacAddr[3], pucMacAddr[4], pucMacAddr[5] );

		PingCheckDbgMsg(1, _T("Cur BSSID(%s) \r\n"), tszMacAddr );

		// 현재 접속된 MAC이 있을 때만 Ping 시도
		if( _tcscmp(tszMacAddr, L"00:00:00:00:00:00") != 0 )
		{

			// Custom Dest IP를 지정 되어있으면, 읽어온 값으로, 지정되지 않았다면, 기본 DefaultGateWay로 시도
			if( _tcslen(m_tszCustomDestIP) > 0 )
			{
				_tcsncpy(pingOpt.tszDestIP, m_tszCustomDestIP, _tcslen(m_tszCustomDestIP) );
			}
			else
			{
				_tcsncpy(pingOpt.tszDestIP, m_adapterInfo.tszDefaultGateWay, _tcslen(m_adapterInfo.tszDefaultGateWay) );
			}


			pingOpt.dwSendBufSize = 32;
			pingOpt.dwTimeOut = 1000;
			pingOpt.ucTos = 0;
			pingOpt.ucTtl = 32;

			PingCheckDbgMsg(1, _T("Dest IP(%s) \r\n"), pingOpt.tszDestIP );

			if( m_pPing->InitPing(pingOpt, tszLiteralIP) == TRUE)
			{
				HKEY hKey = NULL;
				DWORD dwDisposition = 0;

				PingCheckDbgMsg(1, _T("Literal IP(%s) \r\n"), tszLiteralIP );

				// 마지막 테스트 했던 아이피 저장
				_tcsncpy( m_tszPrevCheckDestIP, tszLiteralIP, _tcslen(tszLiteralIP) );
				if( RegCreateKeyEx(HKEY_LOCAL_MACHINE, RK_PING_CHECK, 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS )
				{
					RegSetValueEx(hKey, RV_PREV_DEST_IP, 0, REG_SZ, (LPBYTE) m_tszPrevCheckDestIP, _tcslen(m_tszPrevCheckDestIP) * sizeof(TCHAR) );
				}
				RegCloseKey(hKey);

				for(DWORD dwLoop=0; dwLoop<m_dwMaxCheckCount; dwLoop++)
				{

					NDIS_WLAN_BSSID ndisWlanBssidArr[128] = {0,};
					DWORD dwNumofItems = 0;


					// 주변 AP정보 확인
					GetBBSIDListInfo(m_adapterInfo.tszAdapterName, ndisWlanBssidArr, &dwNumofItems);

					PingCheckDbgMsg(1, _T("Num of SSIDs : %d \r\n"), dwNumofItems );

					PingCheckDbgMsg(1, _T("=============================================================== \r\n"), dwNumofItems );
					if( dwNumofItems > 0 )
					{
						TCHAR tszTemp[256] = {0,};

						for(DWORD dwIdx = 0; dwIdx<dwNumofItems; dwIdx++)
						{
							TCHAR tszMac[18] = {'\0',};
							CString strLogMsg = _T("");

							// MAC
							wsprintf(tszMac, L"%02X:%02X:%02X:%02X:%02X:%02X", ndisWlanBssidArr[dwIdx].MacAddress[0], ndisWlanBssidArr[dwIdx].MacAddress[1], ndisWlanBssidArr[dwIdx].MacAddress[2], ndisWlanBssidArr[dwIdx].MacAddress[3], ndisWlanBssidArr[dwIdx].MacAddress[4], ndisWlanBssidArr[dwIdx].MacAddress[5] );

							// BSSID == MAC
							if( _tcscmp(tszMacAddr, tszMac) == 0 )
							{
								strLogMsg += L"[";
							}

							// SSID
							memset(tszTemp, '\0', sizeof(tszTemp));
							mbstowcs(tszTemp, (char*)ndisWlanBssidArr[dwIdx].Ssid.Ssid, ndisWlanBssidArr[dwIdx].Ssid.SsidLength);
							strLogMsg += tszTemp;

							// BSSID == MAC
							if( _tcscmp(tszMacAddr, tszMac) == 0 )
							{
								strLogMsg += L"]";
							}

							// RSSI
							memset(tszTemp, '\0', sizeof(tszTemp));
							wsprintf(tszTemp, L" / %d", ndisWlanBssidArr[dwIdx].Rssi );
							strLogMsg += tszTemp;

							// MAC
							memset(tszTemp, '\0', sizeof(tszTemp));
							wsprintf(tszTemp, L" / %s", tszMac );
							strLogMsg += tszTemp;

							PingCheckDbgMsg(1, _T("%s \r\n"), strLogMsg );
						}

					}

					// Refresh Scan List
					RefreshBSSIDList(m_adapterInfo.tszAdapterName);

					PingCheckDbgMsg(1, _T("=============================================================== \r\n"), dwNumofItems );




					if( !m_pPing->StartPing(&pingResult) )
					{
						PingCheckDbgMsg(1, _T("Ping Failed!(%d) \r\n"), pingResult.dwErrCode );
						dwPingFailCount++;
					}
					else
					{
						PingCheckDbgMsg(1, _T("Size(%d), Time(%d), TTL(%d) \r\n"), pingResult.dwDataSize, pingResult.dwTime, pingResult.ucTtl );
					}
					
					Sleep(m_dwPingInterval * 1000);
				}

				m_pPing->DeinitPing();


				PingCheckDbgMsg(1, _T("Max[%d] / Fail[%d] \r\n"), m_dwMaxCheckCount, dwPingFailCount );


				// 최대 Ping 테스트 개수가 모두 Fail 되었을때!
				if( dwPingFailCount >= m_dwMaxCheckCount )
				{
					PingCheckDbgMsg(1, _T("Rebind Adapter! \r\n") );

					if( DoNdis_RebindAdater(m_adapterInfo.tszAdapterName) == FALSE )
					{
						PingCheckDbgMsg(1, _T("DoNdis_RebindAdater ERROR!! \r\n") );
					}
				}

			}
			else
			{
				PingCheckDbgMsg(1, _T("Init Ping ERROR!! \r\n") );
			}


		}
		else
		{
			PingCheckDbgMsg(1, _T("isn't be connected to any SSID \r\n") );
		}
		

		PingCheckDbgMsg(1, _T("\r\n") );
	}
}

LRESULT CPingCheckDlg::OnStopCheck(WPARAM wParam, LPARAM lParam)
{
	m_bPinging = FALSE;
	

	if(m_pPing)
	{
		delete m_pPing;
	}

	// 이프로그램은 쓰레드가 종료될때까지 기다려주기에는 너무 오래 걸리기 때문에, WaitForSingleObject를 사용하여 기다리지 않고, 바로 TerminateThread를 이용하여 종료 시켜준다.
	if(hPingThread)
	{
		TerminateThread(hPingThread, 0);
		CloseHandle(hPingThread);
		hPingThread = NULL;
	}

	return 0;
}

void CPingCheckDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnStopCheck(0,0);
}

void CPingCheckDlg::PingCheckDbgMsg(BOOL bTimeStamp, LPVOID strFormat, ...)
{

	// EnableFile Log 레지스트리가 1일 경우에만 로그 기록
	if(m_bEnableFileLog == TRUE)
	{
		TCHAR   tszMsgBuff[1024] = {'\0',};
		CHAR    szMsgBuff[1024] = {'\0',};
		CHAR    szTime[32] = {'\0',};
		DWORD dwStrLen = 0;
		DWORD dwBytesWritten = 0;
		HANDLE hLogFile = NULL;    
	
		va_list lpStart;
		va_start(lpStart, strFormat);
		vsprintf(szMsgBuff, (LPSTR)strFormat, lpStart);
		va_end(lpStart);

		dwStrLen = strlen(szMsgBuff);

		if(dwStrLen < 2)
		{
			va_list lpStart;
			va_start(lpStart, strFormat);
			vswprintf(tszMsgBuff, (LPTSTR)strFormat, lpStart);
			va_end(lpStart);  

			WideCharToMultiByte( CP_ACP, 0, tszMsgBuff, _tcslen(tszMsgBuff), szMsgBuff, sizeof(szMsgBuff), &szMsgBuff[0], NULL);
		}


		hLogFile = CreateFile(m_strLogFilePath, GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		SetFilePointer(hLogFile , 0, NULL, FILE_END);

		if(bTimeStamp)
		{
			SYSTEMTIME sysTime = {0, };
			// 현재 시간 기록
			GetLocalTime(&sysTime);
			sprintf(szTime, "[%02d/%02d, %02d:%02d:%02d] ", sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			WriteFile(hLogFile, szTime, strlen(szTime), &dwBytesWritten, 0);
		}

		// 파일 로그 기록
		WriteFile(hLogFile, szMsgBuff, strlen(szMsgBuff), &dwBytesWritten, 0);

		CloseHandle(hLogFile);
	}
}
