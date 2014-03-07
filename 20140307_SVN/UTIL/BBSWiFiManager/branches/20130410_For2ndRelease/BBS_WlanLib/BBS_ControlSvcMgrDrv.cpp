#include "stdafx.h"
#include "BBS_ControlSvcMgrDrv.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Log
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IOCTL_WIFISVCMGR_LOG_SET_BUFFERSIZE	(0x1000)
#define IOCTL_WIFISVCMGR_LOG_GET_BUFFERSIZE	(0x1001)
#define IOCTL_WIFISVCMGR_LOG_ADD_LOGDATA	(0x1002)
#define IOCTL_WIFISVCMGR_LOG_FLUSH			(0x1003)

#define WIFI_SERVICE_MANAGER_DRIVER_NAME	(L"WSM1:")





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ping Checker
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IOCTL_WIFISVCMGR_PINGCHECKER_START		(0x2000)
#define IOCTL_WIFISVCMGR_PINGCHECKER_GET_STATUS	(0x2001)
#define IOCTL_WIFISVCMGR_PINGCHECKER_STOP		(0x2002)






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static CRITICAL_SECTION		g_csLock = {0,};
static BOOL bInitCS = FALSE;








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Log
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL SetLogBufSize(DWORD dwSize)
{
	BOOL bRet = FALSE;
	HANDLE hWSMDrv = NULL;
	DWORD dwBytesReturned = 0;
	
	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);
	
	hWSMDrv = CreateFile(WIFI_SERVICE_MANAGER_DRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	
	if( hWSMDrv != INVALID_HANDLE_VALUE )
	{
		if( DeviceIoControl(hWSMDrv, IOCTL_WIFISVCMGR_LOG_SET_BUFFERSIZE, &dwSize, sizeof(dwSize), NULL, 0, &dwBytesReturned, NULL) == TRUE )
		{
			if( dwSize == dwBytesReturned )
			{
				bRet = TRUE;
			}
		} 

		CloseHandle(hWSMDrv);
	}

	LeaveCriticalSection(&g_csLock);
	
	return bRet;
}


DWORD GetLogBufSize()
{
	HANDLE hWSMDrv = NULL;
	DWORD dwBufSize = 0;
	DWORD dwBytesReturned = 0;
	
	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);
	
	hWSMDrv = CreateFile(WIFI_SERVICE_MANAGER_DRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	
	
	if( hWSMDrv != INVALID_HANDLE_VALUE )
	{
		if( !DeviceIoControl(hWSMDrv, IOCTL_WIFISVCMGR_LOG_GET_BUFFERSIZE, NULL, 0, &dwBufSize, sizeof(dwBufSize), &dwBytesReturned, NULL) )
		{
			RETAILMSG(1, (TEXT("[NETUI] GetLogBufSize - DeviceIoControl Failed! \r\n") ));
		} 

		CloseHandle(hWSMDrv);
	}

	LeaveCriticalSection(&g_csLock);
	
	return dwBufSize;
}

BOOL FlushLogBuf()
{
	BOOL bRet = FALSE;
	HANDLE hWSMDrv = NULL;
	DWORD dwBytesReturned = 0;
	
	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);
	
	hWSMDrv = CreateFile(WIFI_SERVICE_MANAGER_DRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if( hWSMDrv != INVALID_HANDLE_VALUE )
	{
		if( DeviceIoControl(hWSMDrv, IOCTL_WIFISVCMGR_LOG_FLUSH, NULL, 0, NULL, 0, &dwBytesReturned, NULL) == TRUE )
		{
			bRet = TRUE;
		} 

		CloseHandle(hWSMDrv);
	}

	LeaveCriticalSection(&g_csLock);
	
	return bRet;
}

BOOL AddLogData(BOOL bTimeStamp, LPTSTR strBuf)
{
	BOOL bRet = FALSE;
	HANDLE hWSMDrv = NULL;
	DWORD dwBytesReturned = 0;
	CString strLog = L"";
	
	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);
	
	hWSMDrv = CreateFile(WIFI_SERVICE_MANAGER_DRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if( hWSMDrv != INVALID_HANDLE_VALUE )
	{
		// 시간 기록을 위해 추가
		if( bTimeStamp == TRUE)
		{
			SYSTEMTIME sysTime = {0,};
			
			GetLocalTime(&sysTime);
			strLog.Format(L"[%02d:%02d%:%02d] ", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			strLog += strBuf;
		}
		else
		{
			strLog = strBuf;
		}
		if( DeviceIoControl(hWSMDrv, IOCTL_WIFISVCMGR_LOG_ADD_LOGDATA, strLog.GetBuffer(), strLog.GetLength(), NULL, 0, NULL, NULL) == TRUE )
		{
			bRet = TRUE;

			HANDLE hLogEvent = CreateEvent(NULL, FALSE, FALSE, EVENT_LOG_UPDATE);
			PulseEvent(hLogEvent);
			CloseHandle(hLogEvent);
		} 

		CloseHandle(hWSMDrv);
	}

	LeaveCriticalSection(&g_csLock);
	
	return bRet;
}

TCHAR* GetLogData()
{
	HANDLE hFileMap = NULL;
	TCHAR* ptszLogData = NULL;
		
	hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0, LOG_FILE_MAP_NAME);

	if(hFileMap)
	{
		ptszLogData = (TCHAR*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		UnmapViewOfFile(ptszLogData);
		CloseHandle(hFileMap);
	}

	return ptszLogData;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ping Checker
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



BOOL GetPingCheckerStatus()
{
	HANDLE hWSMDrv = NULL;
	BOOL bStatus = FALSE;
	DWORD dwBytesReturned = 0;
	
	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);
	
	hWSMDrv = CreateFile(WIFI_SERVICE_MANAGER_DRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	
	if( hWSMDrv != INVALID_HANDLE_VALUE )
	{
		if( !DeviceIoControl(hWSMDrv, IOCTL_WIFISVCMGR_PINGCHECKER_GET_STATUS, NULL, 0, &bStatus, sizeof(bStatus), &dwBytesReturned, NULL) )
		{
			RETAILMSG(1, (TEXT("[NETUI] GetPingCheckerStatus - DeviceIoControl Failed! \r\n") ));
		} 

		CloseHandle(hWSMDrv);
	}

	LeaveCriticalSection(&g_csLock);
	
	return bStatus;
}

BOOL PingCheckerStart(BBS_BKG_PINGCHECK_OPT sPingCheckerOpt)
{
	HANDLE hWSMDrv = NULL;
	BOOL bRet = TRUE;
	DWORD dwBytesReturned = 0;

	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	hWSMDrv = CreateFile(WIFI_SERVICE_MANAGER_DRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


	if( hWSMDrv != INVALID_HANDLE_VALUE )
	{
		if( !DeviceIoControl(hWSMDrv, IOCTL_WIFISVCMGR_PINGCHECKER_START, &sPingCheckerOpt, sizeof(sPingCheckerOpt), NULL, 0, &dwBytesReturned, NULL) )
		{
			bRet = FALSE;
			RETAILMSG(1, (TEXT("[NETUI] PingCheckerStart - DeviceIoControl Failed! \r\n") ));
		} 

		CloseHandle(hWSMDrv);
	}

	LeaveCriticalSection(&g_csLock);

	return bRet;
}

BOOL PingCheckerStop()
{
	HANDLE hWSMDrv = NULL;
	BOOL bRet = TRUE;
	DWORD dwBytesReturned = 0;


	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	hWSMDrv = CreateFile(WIFI_SERVICE_MANAGER_DRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


	if( hWSMDrv != INVALID_HANDLE_VALUE )
	{
		if( !DeviceIoControl(hWSMDrv, IOCTL_WIFISVCMGR_PINGCHECKER_STOP, NULL, 0, NULL, 0, &dwBytesReturned, NULL) )
		{
			bRet = FALSE;
			RETAILMSG(1, (TEXT("[NETUI] PingCheckerStop - DeviceIoControl Failed! \r\n") ));
		} 

		CloseHandle(hWSMDrv);
	}

	LeaveCriticalSection(&g_csLock);

	return bRet;
}