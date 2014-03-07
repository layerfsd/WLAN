// WifiServiceDLL.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//
#include <windows.h>
#include "WiFiSvcMgr.h"

#include <pmpolicy.h>
#include <pm.h>


#define RK_SYSTEM_TIMEOUT_PATH	L"System\\CurrentControlSet\\Control\\Power\\Timeouts"
#define RV_BATT_SUSPEND_TIMEOUT	L"BattSuspendTimeout"
#define RV_AC_SUSPEND_TIMEOUT	L"ACSuspendTimeout"




HANDLE g_hFileMap = NULL;
static TCHAR* g_pBuffer = NULL;
static DWORD g_dwBufSize = DEFAULT_LOGBUF_SIZE;
static CRITICAL_SECTION		g_csLock = {0,};

static BOOL g_bCheckedUnattendedMode = FALSE;




static void PowerMonitorThread();
HANDLE g_hPowerMonitorThread = NULL;
HANDLE g_hEventClosePwrMonThread = NULL;


static void RepeatSysIdleTimerResetThread();
HANDLE g_hRepeatSysIdleTimerResetThread = NULL;
BOOL g_bRunningRepeatSysIdleTimerResetThread = FALSE;



void RepeatSysIdleTimerResetThread()
{
	HKEY hKey = NULL;
	DWORD dwBattSuspendTimeout = 0;
	DWORD dwACSuspendTimeout = 0;
	DWORD dwIdleTimerResetPeriod = 0;

	// Timer 백업 및 Unattendedmode 에서 Sleep 진입 되지 않도록 Timer Reset Thread 생성
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, RK_SYSTEM_TIMEOUT_PATH, 0, 0, &hKey) == ERROR_SUCCESS )
	{
		DWORD dwData = 0;
		DWORD dwDataSize = sizeof(dwData);

		if( RegQueryValueEx(hKey, RV_BATT_SUSPEND_TIMEOUT, NULL, NULL, (LPBYTE)&dwData, &dwDataSize) == ERROR_SUCCESS)
		{
			dwBattSuspendTimeout = dwData;
		}

		dwData = 0;
		if( RegQueryValueEx(hKey, RV_AC_SUSPEND_TIMEOUT, NULL, NULL, (LPBYTE)&dwData, &dwDataSize) == ERROR_SUCCESS)
		{
			dwACSuspendTimeout = dwData;
		}

		RegCloseKey(hKey);
		hKey = NULL;
	}

	RETAILMSG(1, (TEXT("[WSM] dwBattSuspendTimeout : %d \r\n"), dwBattSuspendTimeout ));
	RETAILMSG(1, (TEXT("[WSM] dwACSuspendTimeout : %d \r\n"), dwACSuspendTimeout ));


	dwIdleTimerResetPeriod = (dwBattSuspendTimeout < dwACSuspendTimeout)? dwBattSuspendTimeout:dwACSuspendTimeout;


	RETAILMSG(1, (TEXT("[WSM] dwIdleTimerResetPeriod : %d \r\n"), dwIdleTimerResetPeriod ));



	while(g_bRunningRepeatSysIdleTimerResetThread)
	{
		// dwIdleTimerResetPeriod 나누기 2 주기마다 리셋 시켜줌

		for(DWORD dwIdx = 0; dwIdx < dwIdleTimerResetPeriod / 2; dwIdx++)
		{
			if(g_bRunningRepeatSysIdleTimerResetThread == FALSE)
			{
				goto Exit;
			}
			Sleep(1000);
		}

		RETAILMSG(1, (TEXT("[WSM] System Idle Timer Reset! \r\n") ));
		SystemIdleTimerReset();
		
	}


Exit:

	RETAILMSG(1, (TEXT("[WSM] RepeatSysIdleTimerResetThread End \r\n") ));

	return;

}

void PowerMonitorThread()
{

	MSGQUEUEOPTIONS PowerMsgQOptions = {0,};
	HANDLE hPowerMsgQ = NULL;
	HANDLE hPowerNotifications = NULL;
	HANDLE rgHandles[2] = {0};

	PowerMsgQOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
	PowerMsgQOptions.dwFlags = MSGQUEUE_NOPRECOMMIT ;
	PowerMsgQOptions.dwMaxMessages = 0;
	PowerMsgQOptions.cbMaxMessage = sizeof(POWER_BROADCAST) + sizeof(POWER_BROADCAST_POWER_INFO);
	PowerMsgQOptions.bReadAccess = TRUE;
	
	hPowerMsgQ = CreateMsgQueue(NULL, &PowerMsgQOptions);
	
	if( hPowerMsgQ == NULL )
	{
		RETAILMSG(1,(L"[WSM] CreateMsgQueue Failed(%d) \r\n", GetLastError() ));
		goto Exit;
	}

	// Request power notifications 
	hPowerNotifications = RequestPowerNotifications(hPowerMsgQ, PBT_TRANSITION | PBT_RESUME | PBT_POWERINFOCHANGE);

	if( hPowerNotifications == NULL )
	{
		RETAILMSG(1,(L"[WSM] RequestPowerNotifications Failed(%d) \r\n", GetLastError() ));
		goto Exit;
	}


	rgHandles[0] = hPowerMsgQ;
	rgHandles[1] = g_hEventClosePwrMonThread;

	// 1번째 핸들이 아닌경우에는 이 스레드는 Close 된다. (g_hEventClosePwrMonThread 이벤트 인경우 Close)
	while( WaitForMultipleObjects(2, rgHandles, FALSE, INFINITE) == WAIT_OBJECT_0 )
	{
		DWORD cbRead;
		DWORD dwFlags;
		POWER_BROADCAST *ppb = (POWER_BROADCAST*) new BYTE[sizeof(POWER_BROADCAST) + sizeof(POWER_BROADCAST_POWER_INFO)];


		// loop through in case there is more than 1 msg.
		while(ReadMsgQueue(hPowerMsgQ, ppb, sizeof(POWER_BROADCAST) + sizeof(POWER_BROADCAST_POWER_INFO), &cbRead, 0, &dwFlags))
		{

			if( ppb->Message == PBT_TRANSITION )
			{
				if( ppb->Length && wcscmp(ppb->SystemPowerState, L"unattended") == 0 )
				{
					if ( g_bCheckedUnattendedMode == TRUE )
					{
						// Create RepeatSysIdleTimerResetThread
						g_bRunningRepeatSysIdleTimerResetThread = TRUE;
						g_hRepeatSysIdleTimerResetThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RepeatSysIdleTimerResetThread, NULL, 0, NULL);

						if( g_hRepeatSysIdleTimerResetThread == INVALID_HANDLE_VALUE )
						{
							RETAILMSG(1, (TEXT("[ERR] Can't create g_hRepeatSysIdleTimerResetThread \r\n") ));
							g_hRepeatSysIdleTimerResetThread = NULL;
						}

					}
				}
				else
				{
					// Close RepeatSysIdleTimerResetThread

					g_bRunningRepeatSysIdleTimerResetThread = FALSE;

					if( g_hRepeatSysIdleTimerResetThread )
					{
						if( WaitForSingleObject(g_hRepeatSysIdleTimerResetThread, 1000) == WAIT_TIMEOUT )
						{
							DWORD dwCode = 0;

							GetExitCodeThread(g_hRepeatSysIdleTimerResetThread, &dwCode);

							if( dwCode == STILL_ACTIVE )
							{
								RETAILMSG(1, (TEXT("[WSM] g_hRepeatSysIdleTimerResetThread Terminate \r\n") ));
								TerminateThread(g_hRepeatSysIdleTimerResetThread, 0);
							}
						}
						CloseHandle(g_hRepeatSysIdleTimerResetThread);
						g_hRepeatSysIdleTimerResetThread = NULL;
					}

				}
			}

		}
		delete[] ppb;
	}


	RETAILMSG(1,(L"[WSM] PowerMonitorThread End \r\n"));

Exit:
    if (hPowerNotifications)
	{
        StopPowerNotifications(hPowerNotifications);
		hPowerNotifications = NULL;
	}

    if (hPowerMsgQ)
	{
        CloseMsgQueue(hPowerMsgQ);
		hPowerMsgQ = NULL;
	}

    return;

}


BOOL WSM_Init(DWORD dwContext)
{
	RETAILMSG(1, (TEXT("[WSM] WSM_Init : %d \r\n"), dwContext ));

	InitializeCriticalSection(&g_csLock);

	// Create Power Monitor Thread

	g_hEventClosePwrMonThread = CreateEvent(NULL, FALSE, FALSE, NULL);

	if( g_hEventClosePwrMonThread != NULL )
	{
		g_hPowerMonitorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PowerMonitorThread, NULL, 0, NULL);

		if( g_hPowerMonitorThread == INVALID_HANDLE_VALUE )
		{
			RETAILMSG(1,(L"[WSM] [ERR] Can't create PowerMonitorThread \r\n"));
			g_hPowerMonitorThread = NULL;
		}
	}

	return TRUE;
}

BOOL WSM_Deinit()
{
	RETAILMSG(1, (TEXT("[WSM] WSM_Deinit \r\n") ));


	// Close Power Monitor Thread

	if( g_hEventClosePwrMonThread )
	{
		SetEvent(g_hEventClosePwrMonThread);

		if( g_hPowerMonitorThread )
		{
			if( WaitForSingleObject(g_hPowerMonitorThread, 1000) == WAIT_TIMEOUT )
			{
				DWORD dwCode = 0;

				GetExitCodeThread(g_hPowerMonitorThread, &dwCode);

				if( dwCode == STILL_ACTIVE )
				{
					RETAILMSG(1, (TEXT("[WSM] PowerMonitorThread Terminate \r\n") ));
					TerminateThread(g_hPowerMonitorThread, 0);
				}
			}
			CloseHandle(g_hPowerMonitorThread);
			g_hPowerMonitorThread = NULL;
		}

		CloseHandle(g_hEventClosePwrMonThread);
		g_hEventClosePwrMonThread = NULL;
	}



	DeleteCriticalSection(&g_csLock);

	if(g_pBuffer)
	{
		UnmapViewOfFile(g_pBuffer);
	}

	if(g_hFileMap)
	{
		CloseHandle(g_hFileMap);
	}
	return (TRUE);
}


DWORD WSM_Open(DWORD dwData, DWORD dwAccess, DWORD dwShareMode)
{
	RETAILMSG(1, (TEXT("[WSM] WSM_Open(%d,%d,%d) \r\n"),dwData, dwAccess, dwShareMode ));
	return dwData;
}

BOOL WSM_Close(DWORD dwData) 
{
	RETAILMSG(1, (TEXT("[WSM] WSM_Close \r\n") ));
	return (TRUE);
}

DWORD WSM_Read(DWORD dwData, LPVOID pBuf, DWORD Len)
{
	RETAILMSG(1, (TEXT("[WSM] WSM_Read \r\n") ));
	return (0);
}

DWORD WSM_Write(DWORD dwData, LPCVOID pBuf, DWORD Len)
{
	RETAILMSG(1, (TEXT("[WSM] WSM_Write \r\n") ));
	return (0);
}

BOOL WSM_IOControl(DWORD hOpenContext, DWORD dwCode, LPVOID pBufIn, DWORD dwLenIn, LPVOID pBufOut, DWORD dwLenOut, PDWORD pdwActualOut)
{
	DWORD dwSize = 0;
	BOOL bRet = TRUE;
	RETAILMSG(1, (TEXT("[WSM] WSM_IOControl : 0x%X \r\n"), dwCode ));
	switch( dwCode )
	{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Log
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case IOCTL_WIFISVCMGR_LOG_SET_BUFFERSIZE:
		{
			DWORD *pdwData = (DWORD*)pBufIn;
			
			dwSize = *pdwData;
			if( WSM_SetLogBufSize(dwSize) == TRUE )
			{
				if( pdwActualOut != NULL )
				{
					*pdwActualOut = g_dwBufSize;
				}
			}
			else
			{
				bRet = FALSE;
				if( pdwActualOut != NULL )
				{
					*pdwActualOut = 0;
				}
				
			}
		}
		break;

	case IOCTL_WIFISVCMGR_LOG_GET_BUFFERSIZE:
		{
			DWORD *pdwData = (DWORD*)pBufOut;
			*pdwData = g_dwBufSize;

			if( pdwActualOut != NULL )
			{
				*pdwActualOut = g_dwBufSize;
			}
		}
		break;

	case IOCTL_WIFISVCMGR_LOG_ADD_LOGDATA:
		if( pBufIn == NULL)
		{
			bRet = FALSE;
		}
		else
		{
			WSM_AddLogData((LPTSTR)pBufIn);
		}
		break;

	case IOCTL_WIFISVCMGR_LOG_FLUSH:

		if(g_pBuffer)
		{
			g_pBuffer[0] = L'\0';

			if(g_hFileMap)
			{
				if(g_pBuffer)
				{
					UnmapViewOfFile(g_pBuffer);
					g_pBuffer = NULL;
				}
				
				CloseHandle(g_hFileMap);
				g_hFileMap = NULL;
			}
		}
		else
		{
			bRet = FALSE;
		}
		break;




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ping Checker
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	case IOCTL_WIFISVCMGR_PINGCHECKER_START:
		{
			if( dwLenIn != sizeof(BBS_BKG_PINGCHECK_OPT) )
			{
				// Invalid Option Size
				bRet = FALSE;
				break;
			}
			else
			{
				PBBS_BKG_PINGCHECK_OPT pPingCheckOpt = (PBBS_BKG_PINGCHECK_OPT)pBufIn;

				if( WSM_PingCheckerStart(*pPingCheckOpt) == FALSE )
				{
					// Error Ping Start
					bRet = FALSE;
					break;
				}
				
			}

		}
		break;

	case IOCTL_WIFISVCMGR_PINGCHECKER_GET_STATUS:
		{
			BOOL *pbStatus = (BOOL*)pBufOut;

			*pbStatus = WSM_IsPingCheckerOperating();
		}
		break;

	case IOCTL_WIFISVCMGR_PINGCHECKER_STOP:
		{
			if( WSM_PingCheckerStop() == FALSE )
			{
				// Error Ping Stop
				bRet = FALSE;
				break;
			}
			
		}	
		break;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Unattended Mode Ref
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	case IOCTL_WIFISVCMGR_SET_UNATTENDEDMODE:
		{
			BOOL *pbData = (BOOL*)pBufIn;
			TCHAR tszLogBuf[128] = {0,};
			
			if( *pbData == TRUE )
			{
				// UnattendedMode 를 해제시킨경우만 세팅
				if( g_bCheckedUnattendedMode == FALSE )
				{
					if( PowerPolicyNotify(PPN_UNATTENDEDMODE, TRUE) == TRUE )
					{
						g_bCheckedUnattendedMode = TRUE;

						wsprintf(tszLogBuf, L"UNATTENDEDMODE TRUE \r\n" );
						WSM_AddLogData(tszLogBuf);

					}
					else
					{
						bRet = FALSE;
					}
				}

			}
			else
			{
				// UnattendedMode 를 설정 시킨경우만 해제
				if( g_bCheckedUnattendedMode == TRUE )
				{
					if( PowerPolicyNotify(PPN_UNATTENDEDMODE, FALSE) == TRUE )
					{
						DWORD dwCode = 0;

						g_bCheckedUnattendedMode = FALSE;

						wsprintf(tszLogBuf, L"UNATTENDEDMODE FALSE \r\n" );
						WSM_AddLogData(tszLogBuf);
						
					}
					else
					{
						bRet = FALSE;
					}
				}
			}
			
		}
		break;

	default:
		break;
	}

	return bRet;
}


BOOL APIENTRY WSM_DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	RETAILMSG(1, (TEXT("[WSM] WSM_DllMain \r\n") ));
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls( (HMODULE) hModule );
			break;
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}






///////////////////////////////////////////////////////////////////////////////////////////////
// Log
///////////////////////////////////////////////////////////////////////////////////////////////

BOOL WSM_SetLogBufSize(DWORD dwSize)
{
	BOOL bRet = FALSE;


	if(dwSize <= 0)
	{
		dwSize = DEFAULT_LOGBUF_SIZE;
	}

	RETAILMSG(1, (TEXT("[WSM] SetLogBufSize dwSize(%d) \r\n"), dwSize ));


	if(g_hFileMap)
	{
		if(g_pBuffer)
		{
			UnmapViewOfFile(g_pBuffer);
			g_pBuffer = NULL;
		}
		
		CloseHandle(g_hFileMap);
		g_hFileMap = NULL;
	}


	g_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, dwSize, LOG_FILE_MAP_NAME);

	if( g_hFileMap != NULL)
	{
		g_pBuffer = (TCHAR*)MapViewOfFile(g_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		if(g_pBuffer != NULL)
		{
			bRet = TRUE;
		}
		else
		{
			DWORD dwErr = GetLastError();
			RETAILMSG(1, (TEXT("[WSM] MapViewOfFile Error : %d \r\n"), dwErr ));
		}
	}
	else
	{
		DWORD dwErr = GetLastError();
		RETAILMSG(1, (TEXT("[WSM] CreateFileMapping Error : %d \r\n"), dwErr ));
	}

	g_dwBufSize = dwSize;

	return bRet;
}

BOOL WSM_AddLogData(LPTSTR strBuf)
{
	int nlen = 0;
	int nGlbBufferLen = 0;
	int nspare = 0;


	EnterCriticalSection(&g_csLock);

	// 버퍼 사이즈 할당이 되지 않았다면, 기본사이즈로 할당
	if(g_pBuffer == NULL)
	{
		if( WSM_SetLogBufSize(DEFAULT_LOGBUF_SIZE) == FALSE )
		{
			return FALSE;
		}
	}

	nlen = _tcslen(strBuf) * sizeof(TCHAR);
	nGlbBufferLen =_tcslen(g_pBuffer) * sizeof(TCHAR);

	nspare = g_dwBufSize - nGlbBufferLen;


	RETAILMSG(1, (TEXT("[WSM] g_dwBufSize(%d), nlen(%d), nspare(%d), nGlbBufferLen(%d) \r\n"), g_dwBufSize,  nlen, nspare, nGlbBufferLen ));

	if( nspare < nlen)
	{
		wcscpy(&g_pBuffer[0], &g_pBuffer[nlen - nspare]);
	}
	
	wcscat(g_pBuffer, strBuf);

	LeaveCriticalSection(&g_csLock);

	return TRUE;
}

