#include "stdafx.h"
#include "PingChecker.h"



static BOOL			g_bIsOperating = FALSE;
static DWORD		g_dwBreakInterval = 0;
static DWORD		g_dwPingInterval = 0;
static DWORD		g_dwMaxCheckCount = 0;
static TCHAR		g_tszCustomDestAddr[64] = {'\0',};
static HANDLE		g_hCheckThread = NULL;


// 주어진 옵션대로 상태 체크를 수행하는 스레드
DWORD WINAPI WSM_PingCheckerThread(LPVOID lpVoid);


BOOL WSM_PingCheckerStart(BBS_BKG_PINGCHECK_OPT bkgPingCheckOpt)
{
	TCHAR tszLogBuf[128] = L"";
	SYSTEMTIME sysTime = {0,};
			
	NKDbgPrintfW( _T("\r\n\r\n[PING] ========== Ping Check Program Start! ==========\r\n\r\n") );

	GetLocalTime(&sysTime);
	wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] PingChecker Start\r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	WSM_AddLogData(tszLogBuf);
	// 스레드가 동작중이라면 종료시켜 준다.
	WSM_PingCheckerStop();



	g_dwBreakInterval = bkgPingCheckOpt.dwBreakInterval;
	g_dwPingInterval = bkgPingCheckOpt.dwPingInterval;
	g_dwMaxCheckCount = bkgPingCheckOpt.dwMaxCheckCount;

	if( _tcslen(bkgPingCheckOpt.tszCustomDestAddr) == 0 )
	{
		memset(g_tszCustomDestAddr, L'\0', sizeof(g_tszCustomDestAddr));
	}
	else
	{
		_tcscpy(g_tszCustomDestAddr, bkgPingCheckOpt.tszCustomDestAddr);
	}



	NKDbgPrintfW( _T("[PING] ---------- [Option Value] ---------- \r\n") );
	NKDbgPrintfW( _T("[PING] CheckNBreakInterval(%d), PingInterval(%d) \r\n"), g_dwBreakInterval, g_dwPingInterval );
	NKDbgPrintfW( _T("[PING] MaxCheckCount(%d), CustomDestIP(%s) \r\n"), g_dwMaxCheckCount, g_tszCustomDestAddr );
	NKDbgPrintfW( _T("[PING] ---------------------------------------- \r\n\r\n") );

	GetLocalTime(&sysTime);
	wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] BreakInt(%d), PingInt(%d), Max(%d), IP(%s)\r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, g_dwBreakInterval, g_dwPingInterval, g_dwMaxCheckCount, g_tszCustomDestAddr);
	WSM_AddLogData(tszLogBuf);


	// 일단 동작중이 아니라고 표시, PingCheckerThread 함수내에서 실행하면서 TRUE 설정
	g_bIsOperating = FALSE;
	
	g_hCheckThread = CreateThread(NULL, 0, WSM_PingCheckerThread, NULL, 0, NULL);

	if(g_hCheckThread == NULL)
	{
		GetLocalTime(&sysTime);
		wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] Failed CreateThread \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond );

		WSM_AddLogData(tszLogBuf);
		return FALSE;
	}

	return TRUE;
}


BOOL WSM_PingCheckerStop()
{
	BOOL bRet = TRUE;
	TCHAR tszLogBuf[128] = L"";
	SYSTEMTIME sysTime = {0,};

	NKDbgPrintfW( _T("[PING] ===== [PingCheckerStop] ===== \r\n") );
	
	
	GetLocalTime(&sysTime);
	wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] PingChecker Stop \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
	WSM_AddLogData(tszLogBuf);

	// 이미 스레드가 동작중이라면, 종료시키고 새로 생성
	if( g_hCheckThread != NULL || g_bIsOperating == TRUE )
	{
		DWORD dwExitCode = 0;

		// 동작 상태 변경
		g_bIsOperating = FALSE;	

		GetExitCodeThread(g_hCheckThread, &dwExitCode);

		if( dwExitCode == STILL_ACTIVE )
		{
			// 2초까지 종료를 기다려 주고, 그안에 종료되지 않으면 강제 종료
			if( WaitForSingleObject(g_hCheckThread, 2000) == WAIT_TIMEOUT )
			{
				TerminateThread(g_hCheckThread, 0);
			}
		}

		CloseHandle(g_hCheckThread);
		g_hCheckThread = NULL;

	}


	return bRet;
}



BOOL WSM_IsPingCheckerOperating()
{
	BOOL bRet = g_bIsOperating;
	
	return bRet;
}



DWORD WINAPI WSM_PingCheckerThread(LPVOID lpVoid)
{

	BBS_PING_OPTION pingOpt = {0,};
	BBS_PING_RESULT pingResult = {0,};
	BBS_ADAPTER_INFO adapterInfo = {0,};
	BYTE pucBSSID[6] = {0,};
	DWORD dwPingFailCount = 0;
	CBBS_Ping ping;

	TCHAR tszLiteralIP[18] = {'\0',};

	TCHAR tszLogBuf[128] = L"";
	SYSTEMTIME sysTime = {0,};

	NKDbgPrintfW( _T("[PING] ===== [PingCheckerThread Start] ===== \r\n") );

	g_bIsOperating = TRUE;

	while(g_bIsOperating == TRUE)
	{
		BOOL bSuccessGetBSSID = FALSE;

		// 0.5초씩 g_dwBreakInterval 만큼 쉬어준다.
		// 0.5초씩 마다 스레드가 종료 조건이 걸려있는지 확인
		for(UINT i=1; i<=(g_dwBreakInterval * 2); i++)
		{
			// BreakTime Interval
			Sleep(500);

			if(g_bIsOperating == FALSE)
			{
				return 0;
			}
		}

		// PIng 실패 횟수 초기화 필수!
		dwPingFailCount = 0;


		GetWlanDefaultInformation(&adapterInfo);
	
		
		NKDbgPrintfW( _T("[PING] ---------- [Default Information] ---------- \r\n") );
		NKDbgPrintfW( _T("[PING] Adapter(%s), DHCP(%d) \r\n"), adapterInfo.tszAdapterName, adapterInfo.bDHCPEnabled );
		NKDbgPrintfW( _T("[PING] IPAddr(%s) \r\n"), adapterInfo.tszIPAddr );
		NKDbgPrintfW( _T("[PING] DefaultGateWay(%s) \r\n"), adapterInfo.tszDefaultGateWay );
		NKDbgPrintfW( _T("[PING] DnsAddr(%s) \r\n"), adapterInfo.tszDnsAddr );
		NKDbgPrintfW( _T("[PING] ---------------------------------------- \r\n\r\n") );


		// 아답터가 검색되었는지?
		if( _tcslen(adapterInfo.tszAdapterName) == 0 )
		{
			// 아답터가 검색되지 않았다면, 기본 대기시간만큼 다시 대기
			// ERROR
			NKDbgPrintfW( _T("[PING] [ERROR] Can't Find Adapter! \r\n") );

			GetLocalTime(&sysTime);
			wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] Can't Find Adapter! \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
			WSM_AddLogData(tszLogBuf);
			continue;
		}

		bSuccessGetBSSID = GetBSSID(adapterInfo.tszAdapterName, pucBSSID);
		NKDbgPrintfW( _T("[PING] BSSID(%02X:%02X:%02X:%02X:%02X:%02X) \r\n"), pucBSSID[0], pucBSSID[1], pucBSSID[2], pucBSSID[3], pucBSSID[4], pucBSSID[5] );


		GetLocalTime(&sysTime);
		wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] BSSID(%02X:%02X:%02X:%02X:%02X:%02X) \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond,pucBSSID[0], pucBSSID[1], pucBSSID[2], pucBSSID[3], pucBSSID[4], pucBSSID[5] );
		WSM_AddLogData(tszLogBuf);

		// 현재 접속된 SSID가 있을 때만 Ping 시도
		if( bSuccessGetBSSID == TRUE )
		{
			
			// Custom Dest IP를 지정 되어있으면, 읽어온 값으로, 지정되지 않았다면, 기본 DefaultGateWay로 시도
			if( _tcslen(g_tszCustomDestAddr) > 0 )
			{
				_tcscpy(pingOpt.tszDestIP, g_tszCustomDestAddr);
			}
			else
			{
				_tcscpy(pingOpt.tszDestIP, adapterInfo.tszDefaultGateWay);
			}
			pingOpt.dwSendBufSize = DEFAULT_SEND_SIZE;
			pingOpt.dwTimeOut = DEFAULT_TIMEOUT;
			pingOpt.ucTos = DEFAULT_TOS;
			pingOpt.ucTtl = DEFAULT_TTL;


			NKDbgPrintfW( TEXT("[PING] Dest IP(%s) \r\n"), pingOpt.tszDestIP );

			GetLocalTime(&sysTime);
			wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] Dest IP(%s) \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, pingOpt.tszDestIP  );
			WSM_AddLogData(tszLogBuf);

			

			if( ping.InitPing(pingOpt, tszLiteralIP) == TRUE)
			{
				HKEY hKey = NULL;
				DWORD dwDisposition = 0;


				NKDbgPrintfW( TEXT("[PING] Literal IP(%s) \r\n"), tszLiteralIP );

				for(DWORD dwLoop=0; dwLoop<g_dwMaxCheckCount; dwLoop++)
				{
					if( !ping.StartPing(&pingResult) )
					{
						NKDbgPrintfW( TEXT("[PING] Ping Failed!(%d) \r\n"), pingResult.dwErrCode );

						GetLocalTime(&sysTime);
						wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] Ping Failed!(%d) \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, pingOpt.tszDestIP, pingResult.dwErrCode  );
						WSM_AddLogData(tszLogBuf);

						dwPingFailCount++;
					}
					else
					{
						NKDbgPrintfW( TEXT("[PING] Size(%d), Time(%d), TTL(%d) \r\n"), pingResult.dwDataSize, pingResult.dwTime, pingResult.ucTtl );
					}


					// 1초씩 g_dwBreakInterval 만큼 쉬어준다.
					// 1초씩 마다 스레드가 종료 조건이 걸려있는지 확인
					for(UINT i=1; i<=g_dwPingInterval; i++)
					{
						// Ping Interval
						Sleep(1000);

						if(g_bIsOperating == FALSE)
						{
							return 0;
						}
					}
				}

				ping.DeinitPing();


				NKDbgPrintfW( TEXT("[PING] Max[%d] / Fail[%d] \r\n"), g_dwMaxCheckCount, dwPingFailCount );

				// 최대 Ping 테스트 개수가 모두 Fail 되었을때!
				if( dwPingFailCount >= g_dwMaxCheckCount )
				{
					GetLocalTime(&sysTime);

					NKDbgPrintfW( TEXT("[PING] Rebind Adapter! \r\n") );
					wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] Rebind Adapter! \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
					WSM_AddLogData(tszLogBuf);

					if( DoNdis_RebindAdater(adapterInfo.tszAdapterName) == FALSE )
					{
						NKDbgPrintfW( TEXT("[PING] DoNdis_RebindAdater ERROR!! \r\n") );

						wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] DoNdis_RebindAdater ERROR!! \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
						WSM_AddLogData(tszLogBuf);
					}
					
				}

			}
			else
			{
				NKDbgPrintfW( TEXT("[PING] Init Ping ERROR!! \r\n") );

				GetLocalTime(&sysTime);
				wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] Init Ping ERROR!!(May be invalid addr) \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
				WSM_AddLogData(tszLogBuf);
				
			}

			

		}
		else
		{

			NKDbgPrintfW( TEXT("[PING] isn't be connected to any SSID \r\n") );

			GetLocalTime(&sysTime);
			wsprintf(tszLogBuf, L"[%02d:%02d%:%02d] [PCK] isn't be connected to any SSID \r\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
			WSM_AddLogData(tszLogBuf);
		}



	}

	return 0;
}