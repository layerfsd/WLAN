#pragma once

#include "BBS_CommFunc.h"
#include "BBS_Ping.h"
#include "BBS_NdisFuncs.h"

// WiFiServiceManager(WifiSvcMgr) ����̹��� ��Ʈ�� �ϱ� ���� ��� ����



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

// �� ���� ������ �Ҵ� �� ����
BOOL SetLogBufSize(DWORD dwSize);

// �� ���� ������ �˾ƿ��� �Լ�
DWORD GetLogBufSize();

// �α� �����͸� ���ۿ� �߰��Ѵ�.
BOOL AddLogData(BOOL bTimeStamp, LPTSTR strBuf);

// ���ۿ� �ִ� ���ڿ��� ��� �����´�.
TCHAR* GetLogData();

// ���۳��� Clear
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


// ���� üũ �ɼ� ������ ��� ���� ����ü
typedef struct _BBS_BKG_PINGCHECK_OPT {
	BOOL	bOperating;
	DWORD	dwBreakInterval;
	DWORD	dwPingInterval;
	DWORD	dwMaxCheckCount;
	TCHAR	tszCustomDestAddr[64];
} BBS_BKG_PINGCHECK_OPT, *PBBS_BKG_PINGCHECK_OPT;

// Ping Cheker�� ���� ���¸� ����
BOOL GetPingCheckerStatus();

// Ping Cheker�� �����ϴ� �Լ�
BOOL PingCheckerStart(BBS_BKG_PINGCHECK_OPT sPingCheckerOpt);

// Ping Cheker�� �����ϴ� �Լ�
BOOL PingCheckerStop();




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UnattendedMode Management
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IOCTL_WIFISVCMGR_SET_UNATTENDEDMODE		(0x3000)

BOOL SetUnattendedMode(BOOL bMode);