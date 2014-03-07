// WifiServiceDLL.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//
#include <windows.h>
#include "WiFiSvcMgr.h"





HANDLE g_hFileMap = NULL;
static TCHAR* g_pBuffer = NULL;
static DWORD g_dwBufSize = DEFAULT_LOGBUF_SIZE;
static CRITICAL_SECTION		g_csLock = {0,};


BOOL WSM_Init(DWORD dwContext)
{
	RETAILMSG(1, (TEXT("[WSM] WSM_Init : %d \r\n"), dwContext ));

	InitializeCriticalSection(&g_csLock);

	return dwContext;
}

BOOL WSM_Deinit()
{
	RETAILMSG(1, (TEXT("[WSM] WSM_Deinit \r\n") ));

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