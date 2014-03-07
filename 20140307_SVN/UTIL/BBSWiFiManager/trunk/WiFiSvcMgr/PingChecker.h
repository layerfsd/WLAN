#pragma once

#include <BBS_CommFunc.h>
#include <BBS_NdisFuncs.h>
#include <BBS_Ping.h>
#include <BBS_ControlSvcMgrDrv.h>
#include "WiFiSvcMgr.h"


// 백그라운드 핑 상태 체크 시작 함수 (옵션 세팅후 스레드 생성)
BOOL WSM_PingCheckerStart(BBS_BKG_PINGCHECK_OPT bkgPingCheckOpt);

// 백그라운드 핑 상태 체크 종료 함수
BOOL WSM_PingCheckerStop();

// 현재 동작 상태 조회(TRUE : 동작중, FALSE : 비동작중)
BOOL WSM_IsPingCheckerOperating();

