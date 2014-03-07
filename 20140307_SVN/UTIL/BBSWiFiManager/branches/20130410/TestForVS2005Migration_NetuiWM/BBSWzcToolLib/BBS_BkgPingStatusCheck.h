#pragma once





#define DEFAULT_CHECK_N_BREAK_INTERVAL	(90)
#define DEFAULT_PING_INTERVAL			(5)
#define DEFAULT_MAX_CHECK_COUNT			(5)




// ���� üũ �ɼ� ������ ��� ���� ����ü
typedef struct _BBS_BKG_PINGCHECK_OPT {
	DWORD	dwBreakInterval;
	DWORD	dwPingInterval;
	DWORD	dwMaxCheckCount;
	TCHAR	tszCustomDestAddr[64];
} BBS_BKG_PINGCHECK_OPT, *PBBS_BKG_PINGCHECK_OPT;



// ��׶��� �� ���� üũ ���� �Լ� (�ɼ� ������ ������ ����)
BOOL BkgPingStatusCheckStart(BBS_BKG_PINGCHECK_OPT bkgPingCheckOpt);

// ��׶��� �� ���� üũ ���� �Լ�
BOOL BkgPingStatusCheckStop();


// ���� ���� ���� ��ȸ(TRUE : ������, FALSE : ������)
BOOL IsBkgPingCheckOperating(PBBS_BKG_PINGCHECK_OPT pBkgPingCheckOpt);


// �־��� �ɼǴ�� ���� üũ�� �����ϴ� ������
static DWORD WINAPI BkgPingStatusCheckThread(LPVOID lpVoid);

