#pragma once





#define DEFAULT_CHECK_N_BREAK_INTERVAL	(90)
#define DEFAULT_PING_INTERVAL			(5)
#define DEFAULT_MAX_CHECK_COUNT			(5)




// 상태 체크 옵션 정보를 담기 위한 구조체
typedef struct _BBS_BKG_PINGCHECK_OPT {
	DWORD	dwBreakInterval;
	DWORD	dwPingInterval;
	DWORD	dwMaxCheckCount;
	TCHAR	tszCustomDestAddr[64];
} BBS_BKG_PINGCHECK_OPT, *PBBS_BKG_PINGCHECK_OPT;



// 백그라운드 핑 상태 체크 시작 함수 (옵션 세팅후 스레드 생성)
BOOL BkgPingStatusCheckStart(BBS_BKG_PINGCHECK_OPT bkgPingCheckOpt);

// 백그라운드 핑 상태 체크 종료 함수
BOOL BkgPingStatusCheckStop();


// 현재 동작 상태 조회(TRUE : 동작중, FALSE : 비동작중)
BOOL IsBkgPingCheckOperating(PBBS_BKG_PINGCHECK_OPT pBkgPingCheckOpt);


// 주어진 옵션대로 상태 체크를 수행하는 스레드
static DWORD WINAPI BkgPingStatusCheckThread(LPVOID lpVoid);

