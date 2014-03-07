#include "BBS_WifiXml.h"



BOOL CreateDefaultWifiConfigXML(TiXmlDocument *pDoc)
{
	TiXmlElement		*pRoot = NULL;
	TiXmlComment		*pComment = NULL;
	TiXmlElement		*pElem = NULL;
	TiXmlElement		*pSub1Elem = NULL;
	TiXmlElement		*pSub2Elem = NULL;

		
	pDoc->InsertEndChild( TiXmlDeclaration("1.0", "UTF-8", "") );
	
	// Root
	pRoot = new TiXmlElement("BBS_WiFiConfig");
	pDoc->LinkEndChild(pRoot);

	
	// Comment
	pComment = new TiXmlComment();
	pComment->SetValue("For Wireless configuation");
	pRoot->LinkEndChild(pComment);

	// Setting
	pElem = new TiXmlElement("Setting");
	pRoot->LinkEndChild(pElem);

	// Setting - Option
	pSub1Elem = new TiXmlElement("Option");
	pElem->LinkEndChild(pSub1Elem);


	// Setting - Option - BkgPingCheck Option
	pSub2Elem = new TiXmlElement("BkgPingCheck");
	pSub1Elem->LinkEndChild(pSub2Elem);

	pSub2Elem->SetAttribute("Operating", "");
	pSub2Elem->SetAttribute("BreakInterval", "");
	pSub2Elem->SetAttribute("PingInterval", "");
	pSub2Elem->SetAttribute("MaxCheckCnt", "");
	pSub2Elem->SetAttribute("CustomDestAddr", "");

	
	// Setting - Option - Scan Option
	pSub2Elem = new TiXmlElement("ScanOption");
	pSub1Elem->LinkEndChild(pSub2Elem);

	pSub1Elem->SetAttribute("TurnOnAvailNetNoti", "");
	pSub2Elem->SetAttribute("ScanThreshold", "");
	pSub2Elem->SetAttribute("LockSSID", "");
	pSub2Elem->SetAttribute("NetToAccess", "");

	
	// Setting - Option - DefaultPreferNet Option
	pSub1Elem = new TiXmlElement("DefaultPreferNet");
	pElem->LinkEndChild(pSub1Elem);
	pSub1Elem->SetAttribute("Enable", "");
	pSub1Elem->SetAttribute("DHCP", "");
	pSub1Elem->SetAttribute("IP", "");
	pSub1Elem->SetAttribute("Subnet", "");
	pSub1Elem->SetAttribute("Gateway", "");
	pSub1Elem->SetAttribute("DNS", "");
	pSub1Elem->SetAttribute("SSID", "");
	pSub1Elem->SetAttribute("Auth", "");
	pSub1Elem->SetAttribute("Encr", "");
	pSub1Elem->SetAttribute("Key", "");
	pSub1Elem->SetAttribute("KeyIdx", "");
	pSub1Elem->SetAttribute("Hex_16", "");

	
	// 저장하려는 파일의 전체 경로중 없는 폴더가 있다면 폴더 자동생성
	CreateFolderIfNotFoundDir(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_W);

	// XML 파일형태로 저장.
	if( pDoc->SaveFile(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A) == FALSE )
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL SetBkgPingCheckOpt(BBS_BKG_PINGCHECK_OPT pingOpt)
{
	TiXmlDocument doc;
	char szCustomDest[128] = {'\0',};

	
	wcstombs(szCustomDest, pingOpt.tszCustomDestAddr, _tcslen(pingOpt.tszCustomDestAddr) );

	if( doc.LoadFile(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A) == FALSE )
	{
		CreateDefaultWifiConfigXML(&doc);
	}

	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option")->FirstChildElement("BkgPingCheck");

	pElem->SetAttribute("Operating", pingOpt.bOperating);
	pElem->SetAttribute("BreakInterval", pingOpt.dwBreakInterval);
	pElem->SetAttribute("PingInterval", pingOpt.dwPingInterval);
	pElem->SetAttribute("MaxCheckCnt", pingOpt.dwMaxCheckCount);
	pElem->SetAttribute("CustomDestAddr", szCustomDest);
	
	return doc.SaveFile();
}


BOOL GetBkgPingCheckOpt(PBBS_BKG_PINGCHECK_OPT pPingOpt)
{
	TiXmlDocument doc;
	const char *pszCustomDest = NULL;
	bool bOperating = false;
	int nBreakInt = 0;
	int nPingInt = 0;
	int nMaxCount = 0;

	if( doc.LoadFile(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A) == FALSE )
	{
		CreateDefaultWifiConfigXML(&doc);
	}

	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option")->FirstChildElement("BkgPingCheck");


	pElem->QueryBoolAttribute("Operating", &bOperating);
	pElem->QueryIntAttribute("BreakInterval",  &nBreakInt);
	pElem->QueryIntAttribute("PingInterval", &nPingInt);
	pElem->QueryIntAttribute("MaxCheckCnt", &nMaxCount);
	pszCustomDest = pElem->Attribute("CustomDestAddr");


	pPingOpt->bOperating = bOperating;
	pPingOpt->dwBreakInterval = nBreakInt;
	pPingOpt->dwPingInterval = nPingInt;
	pPingOpt->dwMaxCheckCount = nMaxCount;

	mbstowcs(pPingOpt->tszCustomDestAddr, pszCustomDest, strlen(pszCustomDest));

	return TRUE;
}



BOOL SetScanOpt(BBS_SCAN_OPTION_INFO scanOptInfo)
{
	TiXmlDocument doc;
	char szLockSSID[64] = {0,};

	if( doc.LoadFile(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A) == FALSE )
	{
		CreateDefaultWifiConfigXML(&doc);
	}

	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option")->FirstChildElement("ScanOption");

	wcstombs(szLockSSID, scanOptInfo.tszLockSSID, _tcslen(scanOptInfo.tszLockSSID));
	pElem->SetAttribute("TurnOnAvailNetNoti", scanOptInfo.bTurnOnAvailNetNoti);
	pElem->SetAttribute("ScanThreshold", scanOptInfo.dwScanThreShold);
	pElem->SetAttribute("LockSSID", szLockSSID);
	pElem->SetAttribute("NetToAccess", scanOptInfo.dwNetToAccess);
	
	return doc.SaveFile();
}

BOOL GetScanOpt(PBBS_SCAN_OPTION_INFO pScanOptInfo)
{
	TiXmlDocument doc;
	const char *pszLockSSID = NULL;
	int nScanThreShold = 0;
	int nNettoAccess = 0;
	bool bEnableLockSSID = false;
	

	if( doc.LoadFile(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A) == FALSE )
	{
		CreateDefaultWifiConfigXML(&doc);
	}

	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option")->FirstChildElement("ScanOption");



	pElem->Attribute("TurnOnAvailNetNoti", (int*)&pScanOptInfo->bTurnOnAvailNetNoti);
	
	if( pElem->QueryIntAttribute("ScanThreshold",  &nScanThreShold) == TIXML_WRONG_TYPE )
	{
		nScanThreShold = -1;
	}
	if( pElem->QueryIntAttribute("NetToAccess", &nNettoAccess) == TIXML_WRONG_TYPE )
	{
		nNettoAccess = -1;
	}
	pszLockSSID = pElem->Attribute("LockSSID");


	
	pScanOptInfo->dwScanThreShold = nScanThreShold;
	pScanOptInfo->dwNetToAccess = nNettoAccess;
	
	mbstowcs(pScanOptInfo->tszLockSSID, pszLockSSID, strlen(pszLockSSID));
	

	return TRUE;
}




BOOL SetDefPrefNetOpt(BBS_DEF_PREF_NETWORK_INFO defPrefNetInfo)
{
	TiXmlDocument doc;

	char szIPAddr[18] = {'\0',};
	char szSubNetMask[18] = {'\0',};
	char szGateWay[18] = {'\0',};
	char szDnsAddr[18] = {'\0',};
	char szSSID[255] = {'\0',};
	char szNetworkKey[128] = {'\0',};


	wcstombs(szIPAddr, defPrefNetInfo.tszIPAddr, _tcslen(defPrefNetInfo.tszIPAddr) );
	wcstombs(szSubNetMask, defPrefNetInfo.tszSubNetMask, _tcslen(defPrefNetInfo.tszSubNetMask) );
	wcstombs(szGateWay, defPrefNetInfo.tszGateWay, _tcslen(defPrefNetInfo.tszGateWay) );
	wcstombs(szDnsAddr, defPrefNetInfo.tszDnsAddr, _tcslen(defPrefNetInfo.tszDnsAddr) );
	wcstombs(szSSID, defPrefNetInfo.tszSSID, _tcslen(defPrefNetInfo.tszSSID) );
	wcstombs(szNetworkKey, defPrefNetInfo.tszNetworkKey, _tcslen(defPrefNetInfo.tszNetworkKey) );


	if( doc.LoadFile(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A) == FALSE )
	{
		CreateDefaultWifiConfigXML(&doc);
	}

	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChildElement("DefaultPreferNet");

	pElem->SetAttribute("Enable", defPrefNetInfo.bEnable);
	pElem->SetAttribute("DHCP", defPrefNetInfo.bDHCP);
	pElem->SetAttribute("IP", szIPAddr);
	pElem->SetAttribute("Subnet", szSubNetMask);
	pElem->SetAttribute("Gateway", szGateWay);
	pElem->SetAttribute("DNS", szDnsAddr);
	pElem->SetAttribute("SSID", szSSID);
	pElem->SetAttribute("Auth", defPrefNetInfo.dwAuthType);
	pElem->SetAttribute("Encr", defPrefNetInfo.dwEncrType);
	pElem->SetAttribute("Key", szNetworkKey);
	pElem->SetAttribute("KeyIdx", defPrefNetInfo.dwKeyIndex);
	pElem->SetAttribute("Hex_16", defPrefNetInfo.bHexKey);

	
	return doc.SaveFile();	
}

BOOL GetDefPrefNetOpt(BBS_DEF_PREF_NETWORK_INFO *pDefPrefNetInfo)
{
	TiXmlDocument doc;

	int bEnable = 32;


	const char *pszIPAddr = NULL;
	const char *pszSubNetMask = NULL;
	const char *pszGateWay = NULL;
	const char *pszDnsAddr = NULL;
	const char *pszSSID = NULL;
	const char *pszNetworkKey = NULL;


	if( doc.LoadFile(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A) == FALSE )
	{
		CreateDefaultWifiConfigXML(&doc);
	}

	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChildElement("DefaultPreferNet");



	pElem->Attribute("Enable", (int*)&pDefPrefNetInfo->bEnable);
	pElem->Attribute("DHCP", (int*)&pDefPrefNetInfo->bDHCP);
	pszIPAddr = pElem->Attribute("IP");
	pszSubNetMask = pElem->Attribute("Subnet");
	pszGateWay = pElem->Attribute("Gateway");
	pszDnsAddr = pElem->Attribute("DNS");
	pszSSID = pElem->Attribute("SSID");
	pElem->Attribute("Auth", (int*)&pDefPrefNetInfo->dwAuthType);
	pElem->Attribute("Encr", (int*)&pDefPrefNetInfo->dwEncrType);
	pszNetworkKey = pElem->Attribute("Key");
	pElem->Attribute("KeyIdx", (int*)&pDefPrefNetInfo->dwKeyIndex);
	pElem->Attribute("Hex_16", (int*)&pDefPrefNetInfo->bHexKey);


	mbstowcs(pDefPrefNetInfo->tszIPAddr, pszIPAddr, strlen(pszIPAddr));
	mbstowcs(pDefPrefNetInfo->tszSubNetMask, pszSubNetMask, strlen(pszSubNetMask));
	mbstowcs(pDefPrefNetInfo->tszGateWay, pszGateWay, strlen(pszGateWay));
	mbstowcs(pDefPrefNetInfo->tszDnsAddr, pszDnsAddr, strlen(pszDnsAddr));
	mbstowcs(pDefPrefNetInfo->tszSSID, pszSSID, strlen(pszSSID));
	mbstowcs(pDefPrefNetInfo->tszNetworkKey, pszNetworkKey, strlen(pszNetworkKey));

	return TRUE;	
}
