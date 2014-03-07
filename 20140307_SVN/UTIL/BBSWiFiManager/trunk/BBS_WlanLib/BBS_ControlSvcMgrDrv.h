#pragma once

#include "BBS_CommFunc.h"
#include "BBS_Ping.h"
#include "BBS_NdisFuncs.h"

// WiFiServiceManager(WifiSvcMgr) 드라이버를 컨트롤 하기 위한 기능 모음



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Log
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IOCTL_WIFISVCMGR_LOG_SET_BUFFERSIZE	(0x1000)
#define IOCTL_WIFISVCMGR_LOG_GET_BUFFERSIZE	(0x1001)
#define IOCTL_WIFISVCMGR_LOG_ADD_LOGDATA	(0x1002)
#define IOCTL_WIFISVCMGR_LOG_FLUSH			(0x1003)

#define WIFI_SERVICE_MANAGER_DRIVER_NAME	(L"WSM1:")



#ifndef EVENT_LOG_UPDATE
#define EVENT_LOG_UPDATE	L"UPDATE_WIFI_LOG_DATA"
#endif

#ifndef LOG_FILE_MAP_NAME
#define LOG_FILE_MAP_NAME	L"LOG_FILE_MAP"
#endif

#ifndef DEFAULT_LOGBUF_SIZE
#define DEFAULT_LOGBUF_SIZE (1024 * 512) // 512KB
#endif

// 총 버퍼 사이즈 할당 및 변경
BOOL SetLogBufSize(DWORD dwSize);

// 총 버퍼 사이즈 알아오는 함수
DWORD GetLogBufSize();

// 로그 데이터를 버퍼에 추가한다.
BOOL AddLogData(BOOL bTimeStamp, LPTSTR strBuf);

// 버퍼에 있는 문자열을 모두 가져온다.
TCHAR* GetLogData();

// 버퍼내용 Clear
BOOL FlushLogBuf();







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ping Checker
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IOCTL_WIFISVCMGR_PINGCHECKER_START		(0x2000)
#define IOCTL_WIFISVCMGR_PINGCHECKER_GET_STATUS	(0x2001)
#define IOCTL_WIFISVCMGR_PINGCHECKER_STOP		(0x2002)



#define DEFAULT_CHECK_N_BREAK_INTERVAL	(90)
#define DEFAULT_PING_INTERVAL			(5)
#define DEFAULT_MAX_CHECK_COUNT			(5)


// 상태 체크 옵션 정보를 담기 위한 구조체
typedef struct _BBS_BKG_PINGCHECK_OPT {
	BOOL	bOperating;
	DWORD	dwBreakInterval;
	DWORD	dwPingInterval;
	DWORD	dwMaxCheckCount;
	TCHAR	tszCustomDestAddr[64];
} BBS_BKG_PINGCHECK_OPT, *PBBS_BKG_PINGCHECK_OPT;

// Ping Cheker의 현재 상태를 얻어옴
BOOL GetPingCheckerStatus();

// Ping Cheker를 시작하는 함수
BOOL PingCheckerStart(BBS_BKG_PINGCHECK_OPT sPingCheckerOpt);

// Ping Cheker를 종료하는 함수
BOOL PingCheckerStop();




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UnattendedMode Management
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IOCTL_WIFISVCMGR_SET_UNATTENDEDMODE		(0x3000)

BOOL SetUnattendedMode(BOOL bMode);