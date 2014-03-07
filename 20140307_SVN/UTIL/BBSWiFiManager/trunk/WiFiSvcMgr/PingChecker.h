#pragma once

#include <BBS_CommFunc.h>
#include <BBS_NdisFuncs.h>
#include <BBS_Ping.h>
#include <BBS_ControlSvcMgrDrv.h>
#include "WiFiSvcMgr.h"


// ��׶��� �� ���� üũ ���� �Լ� (�ɼ� ������ ������ ����)
BOOL WSM_PingCheckerStart(BBS_BKG_PINGCHECK_OPT bkgPingCheckOpt);

// ��׶��� �� ���� üũ ���� �Լ�
BOOL WSM_PingCheckerStop();

// ���� ���� ���� ��ȸ(TRUE : ������, FALSE : ������)
BOOL WSM_IsPingCheckerOperating();

