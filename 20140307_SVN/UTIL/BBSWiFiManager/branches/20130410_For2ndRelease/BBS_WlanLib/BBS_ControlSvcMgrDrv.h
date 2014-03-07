#pragma once

#include "BBS_CommFunc.h"
#include "BBS_Ping.h"
#include "BBS_NdisFuncs.h"

// WiFiServiceManager(WifiSvcMgr) ����̹��� ��Ʈ�� �ϱ� ���� ��� ����





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Log
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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