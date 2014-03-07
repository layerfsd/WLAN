#pragma once

#include <windows.h>
#include "tinyxml.h"
#include "tinystr.h"
#include <BBS_ControlSvcMgrDrv.h>


#define DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A	("ProgramStore\\System\\WifiConfig.xml")
#define DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_W	_T("ProgramStore\\System\\WifiConfig.xml")

#define DEFAULT_SCANOPT_SCANTHRESHOLD_VALUE	(0)
#define DEFAULT_SCANOPT_NETTOACCESS_VALUE	(2)
#define DEFAULT_TURNON_AVAIL_NET_NOTI_VALUE	(0)

// DefaultPreferNet 정보 세팅값을 담기위한 구조체
typedef struct _BBS_DEF_PREF_NETWORK_INFO {
	BOOL	bEnable;
	BOOL	bDHCP;
	BOOL	bHidden;
	TCHAR	tszIPAddr[18];
	TCHAR	tszSubNetMask[18];
	TCHAR	tszGateWay[18];
	TCHAR	tszDnsAddr[18];
	TCHAR	tszSSID[255];
	DWORD	dwAuthType;
	DWORD	dwEncrType;
	TCHAR	tszNetworkKey[128];
	DWORD	dwKeyIndex;
	BOOL	bHexKey;
} BBS_DEF_PREF_NETWORK_INFO, *PBBS_DEF_PREF_NETWORK_INFO;


// Scan Option 정보 세팅값을 담기위한 구조체
typedef struct _BBS_SCAN_OPTION_INFO {
	BOOL bTurnOnAvailNetNoti;
	DWORD dwScanThreShold;
	TCHAR tszLockSSID[64];
	DWORD dwNetToAccess;
} BBS_SCAN_OPTION_INFO, *PBBS_SCAN_OPTION_INFO;


// XML 유무 및 XML 내부 있어야할 Node가 다 있는지 확인 및 없으면 추가해주는 함수
BOOL CheckValidXML(TiXmlDocument *pDoc);


// Setting
BOOL SetBkgPingCheckOpt(BBS_BKG_PINGCHECK_OPT pingOpt);
BOOL GetBkgPingCheckOpt(PBBS_BKG_PINGCHECK_OPT pPingOpt);

BOOL SetScanOpt(BBS_SCAN_OPTION_INFO scanOptInfo);
BOOL GetScanOpt(PBBS_SCAN_OPTION_INFO pScanOptInfo);

BOOL SetPowerOpt(BOOL bAllowSuspendMode);
BOOL GetPowerOpt(int *pnAllowSuspendMode);

BOOL SetDefPrefNetOpt(BBS_DEF_PREF_NETWORK_INFO defPrefNetInfo);
BOOL GetDefPrefNetOpt(PBBS_DEF_PREF_NETWORK_INFO pDefPrefNetInfo);