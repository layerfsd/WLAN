#include "stdafx.h"
#include "BBS_BkgPingStatusCheck.h"
#include "BBS_CommFunc.h"
#include "BBS_Ping.h"



static BOOL			g_bIsOperating = FALSE;
static DWORD		g_dwBreakInterval = 0;
static DWORD		g_dwPingInterval = 0;
static DWORD		g_dwMaxCheckCount = 0;
static TCHAR		g_tszCustomDestAddr[64] = {'\0',};
static HANDLE		g_hCheckThread = NULL;

BOOL BkgPingStatusCheckStart(BBS_BKG_PINGCHECK_OPT bkgPingCheckOpt)
{

	NKDbgPrintfW( _T("\r\n\r\n[NETUI] ========== Ping Check Program Start! ==========\r\n\r\n") );

	// 스레드가 동작중이라면 종료시켜 준다.
	BkgPingStatusCheckStop();



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



	NKDbgPrintfW( _T("[NETUI] ---------- [Option Value] ---------- \r\n") );
	NKDbgPrintfW( _T("[NETUI] CheckNBreakInterval(%d), PingInterval(%d) \r\n"), g_dwBreakInterval, g_dwPingInterval );
	NKDbgPrintfW( _T("[NETUI] MaxCheckCount(%d), CustomDestIP(%s) \r\n"), g_dwMaxCheckCount, g_tszCustomDestAddr );
	NKDbgPrintfW( _T("[NETUI] ---------------------------------------- \r\n\r\n") );


	// 일단 동작중이 아니라고 표시, BkgPingStatusCheckThread 함수내에서 실행하면서 TRUE 설정
	g_bIsOperating = FALSE;
	
	g_hCheckThread = CreateThread(NULL, 0, BkgPingStatusCheckThread, NULL, 0, NULL);

	if(g_hCheckThread == NULL)
	{
		// ERROR
		return FALSE;
	}

	return TRUE;
}


BOOL BkgPingStatusCheckStop()
{
	BOOL bRet = TRUE;

	NKDbgPrintfW( _T("[NETUI] ===== [BkgPingStatusCheckStop] ===== \r\n") );

	// 이미 스레드가 동작중이라면, 종료시키고 새로 생성
	if( g_hCheckThread != NULL || g_bIsOperating == TRUE )
	{
		DWORD dwExitCode = 0;

		// 동작 상태 변경
		g_bIsOperating = FALSE;	

		// 2초까지 종료를 기다려 주고, 그안에 종료되지 않으면 강제 종료
		if( WaitForSingleObject(g_hCheckThread, 2000) == WAIT_TIMEOUT )
		{
			bRet = TerminateThread(g_hCheckThread, 0);
		}
		CloseHandle(g_hCheckThread);
		g_hCheckThread = NULL;

	}

	Sleep(500);


	return bRet;
}



BOOL IsBkgPingCheckOperating(PBBS_BKG_PINGCHECK_OPT pBkgPingCheckOpt)
{
	BOOL bRet = FALSE;
	if(g_bIsOperating == TRUE)
	{
		bRet = TRUE;

		pBkgPingCheckOpt->dwBreakInterval = g_dwBreakInterval;
		pBkgPingCheckOpt->dwPingInterval = g_dwPingInterval;
		pBkgPingCheckOpt->dwMaxCheckCount = g_dwMaxCheckCount;
		
		_tcsncpy(pBkgPingCheckOpt->tszCustomDestAddr, g_tszCustomDestAddr, _tcslen(g_tszCustomDestAddr));
	}
	else
	{
		bRet = FALSE;
	}
	
	return bRet;
}



DWORD WINAPI BkgPingStatusCheckThread(LPVOID lpVoid)
{

	BBS_PING_OPTION pingOpt = {0,};
	BBS_CUR_NET_INFO netInfo;
	BBS_PING_RESULT pingResult;
	BBS_ADAPTER_INFO adapterInfo;
	DWORD dwPingFailCount = 0;
	CBBS_Ping ping;

	TCHAR tszLiteralIP[18] = {'\0',};

	memset(&pingOpt, 0, sizeof(BBS_PING_OPTION));
	memset(&netInfo, 0, sizeof(BBS_CUR_NET_INFO));
	memset(&pingResult, 0, sizeof(BBS_PING_RESULT));
	memset(&adapterInfo, 0, sizeof(BBS_ADAPTER_INFO));


	NKDbgPrintfW( _T("[NETUI] ===== [BkgPingStatusCheckThread Start] ===== \r\n") );

	g_bIsOperating = TRUE;

	while(g_bIsOperating == TRUE)
	{

		// 1초씩 g_dwBreakInterval 만큼 쉬어준다.
		// 1초씩 마다 스레드가 종료 조건이 걸려있는지 확인
		for(UINT i=1; i<=g_dwBreakInterval; i++)
		{
			// BreakTime Interval
			Sleep(1000);

			if(g_bIsOperating == FALSE)
			{
				return 0;
			}
		}

		// PIng 실패 횟수 초기화 필수!
		dwPingFailCount = 0;


		GetWlanDefaultInformation(&adapterInfo);

		GetCurrentNetworkInfo(adapterInfo.tszAdapterName, &netInfo);

		
		NKDbgPrintfW( _T("[NETUI] ---------- [Default Information] ---------- \r\n") );
		NKDbgPrintfW( _T("[NETUI] Adapter(%s), DHCP(%d) \r\n"), adapterInfo.tszAdapterName, adapterInfo.bDHCPEnabled );
		NKDbgPrintfW( _T("[NETUI] IPAddr(%s) \r\n"), adapterInfo.tszIPAddr );
		NKDbgPrintfW( _T("[NETUI] DefaultGateWay(%s) \r\n"), adapterInfo.tszDefaultGateWay );
		NKDbgPrintfW( _T("[NETUI] DnsAddr(%s) \r\n"), adapterInfo.tszDnsAddr );
		NKDbgPrintfW( _T("[NETUI] ---------------------------------------- \r\n\r\n") );



		// 아답터가 검색되었는지?
		if( _tcslen(adapterInfo.tszAdapterName) == 0 )
		{
			// 아답터가 검색되지 않았다면, 기본 대기시간만큼 다시 대기
			// ERROR
			NKDbgPrintfW( _T("[NETUI] [ERROR] Can't Find Adapter! \r\n") );
			continue;
		}


		NKDbgPrintfW( _T("[NETUI] SSID : %s \r\n"), netInfo.tszSSID );

		// 현재 접속된 SSID가 있을 때만 Ping 시도
		if( _tcslen(netInfo.tszSSID) != 0 )
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
			pingOpt.dwSendBufSize = 32;
			pingOpt.dwTimeOut = 1000;
			pingOpt.ucTos = 0;
			pingOpt.ucTtl = 32;


			NKDbgPrintfW( TEXT("[NETUI] Dest IP(%s) \r\n"), pingOpt.tszDestIP );

			

			if( ping.InitPing(pingOpt, tszLiteralIP) == TRUE)
			{
				HKEY hKey = NULL;
				DWORD dwDisposition = 0;


				NKDbgPrintfW( TEXT("[NETUI] Literal IP(%s) \r\n"), tszLiteralIP );

				for(DWORD dwLoop=0; dwLoop<g_dwMaxCheckCount; dwLoop++)
				{
					if( !ping.StartPing(&pingResult) )
					{
						NKDbgPrintfW( TEXT("[NETUI] Ping Failed!(%d) \r\n"), pingResult.dwErrCode );
						dwPingFailCount++;
					}
					else
					{
						NKDbgPrintfW( TEXT("[NETUI] Size(%d), Time(%d), TTL(%d) \r\n"), pingResult.dwDataSize, pingResult.dwTime, pingResult.ucTtl );
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


				NKDbgPrintfW( TEXT("[NETUI] Max[%d] / Fail[%d] \r\n"), g_dwMaxCheckCount, dwPingFailCount );

				// 최대 Ping 테스트 개수가 모두 Fail 되었을때!
				if( dwPingFailCount >= g_dwMaxCheckCount )
				{
					NKDbgPrintfW( TEXT("[NETUI] Rebind Adapter! \r\n") );

					if( DoNdis_RebindAdater(adapterInfo.tszAdapterName) == FALSE )
					{
						NKDbgPrintfW( TEXT("[NETUI] DoNdis_RebindAdater ERROR!! \r\n") );
					}
				}

			}
			else
			{
				NKDbgPrintfW( TEXT("[NETUI] Init Ping ERROR!! \r\n") );
			}

			

		}
		else
		{
			NKDbgPrintfW( TEXT("[NETUI] isn't be connected to any SSID \r\n") );
		}



	}

	return 0;
}