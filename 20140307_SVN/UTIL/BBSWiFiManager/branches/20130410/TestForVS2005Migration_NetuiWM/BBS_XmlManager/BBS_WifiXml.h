#pragma once

#include <windows.h>
#include "tinyxml.h"
#include "tinystr.h"


#define DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A	("ProgramStore\\System\\WifiConfig.xml")
#define DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_W	_T("ProgramStore\\System\\WifiConfig.xml")


// DefaultPreferNet 정보 세팅값을 담기위한 구조체
typedef struct _BBS_DEF_PREF_NETWORK_INFO {
	BOOL	bEnable;
	BOOL	bDHCP;
	TCHAR	tszIPAddr[16];
	TCHAR	tszSubNetMask[16];
	TCHAR	tszGateWay[16];
	TCHAR	tszDnsAddr[16];
	TCHAR	tszSSID[255];
	DWORD	dwAuthType;
	DWORD	dwEncrType;
	TCHAR	tszNetworkKey[128];
	DWORD	dwKeyIndex;
	BOOL	bHexKey;
} BBS_DEF_PREF_NETWORK_INFO, *PBBS_DEF_PREF_NETWORK_INFO;



// Setting-Option
BOOL CreateDefaultWifiConfigXML(BOOL *pDoc);

BOOL SetBkgPingCheckOpt(BOOL bOperating, int nBreakItv, int nPingItv, int nMaxCheckCnt, LPTSTR tszCustomDest);
BOOL GetBkgPingCheckOpt(BOOL *pbOperating, DWORD *pdwBreakItv, DWORD *pdwPingItv, DWORD *pdwMaxCheckCnt, LPTSTR tszCustomDest);

BOOL SetScanOpt(int nScanThreshold, BOOL bEnableLockSSD, char *szLockSSID, int *nNetToAccess);
BOOL GetScanOpt(DWORD *pdwScanThreshold, BOOL *pbEnableLockSSD, LPTSTR tszLockSSID, DWORD *pdwNetToAccess);

BOOL SetDefPrefNetOpt(BBS_DEF_PREF_NETWORK_INFO defPrefNetInfo);
BOOL GetDefPrefNetOpt(BBS_DEF_PREF_NETWORK_INFO *pDefPrefNetInfo);