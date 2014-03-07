#include "BBS_WifiXml.h"




BOOL CheckValidXML(TiXmlDocument *pDoc)
{

	if( pDoc->LoadFile(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A) == FALSE )
	{
		pDoc->InsertEndChild( TiXmlDeclaration("1.0", "UTF-8", "") );

		// 저장하려는 파일의 전체 경로중 없는 폴더가 있다면 폴더 자동생성
		CreateFolderIfNotFoundDir(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_W);

		// XML 파일형태로 저장.
		if( pDoc->SaveFile(DEFAULT_BBS_WIFI_MANAGER_CONFIGXML_PATH_A) == FALSE )
		{
			return FALSE;
		}
	}


	TiXmlNode *pNode = pDoc->FirstChild("BBS_WiFiConfig");

	if(pNode == NULL)
	{
		TiXmlElement* pElem = new TiXmlElement("BBS_WiFiConfig");

		pDoc->LinkEndChild( pElem );
		pDoc->SaveFile();
	}

	pNode = pDoc->FirstChild("BBS_WiFiConfig")->FirstChild("Setting");
	if(pNode == NULL)
	{
		TiXmlElement* pElem = new TiXmlElement("Setting");

		pDoc->FirstChild("BBS_WiFiConfig")->LinkEndChild( pElem );
		pDoc->SaveFile();
	}

	pNode = pDoc->FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option");
	if(pNode == NULL)
	{
		TiXmlElement* pElem = new TiXmlElement("Option");

		pDoc->FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->LinkEndChild( pElem );
		pDoc->SaveFile();
	}

	pNode = pDoc->FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option")->FirstChild("BkgPingCheck");
	if(pNode == NULL)
	{
		TiXmlElement* pElem;
			
		pElem = new TiXmlElement("BkgPingCheck");
		pDoc->FirstChildElement("BBS_WiFiConfig")->FirstChild("Setting")->FirstChildElement("Option")->LinkEndChild( pElem );
		
		pElem->SetAttribute("Operating", "");
		pElem->SetAttribute("BreakInterval", "");
		pElem->SetAttribute("PingInterval", "");
		pElem->SetAttribute("MaxCheckCnt", "");	
		pElem->SetAttribute("CustomDestAddr", "");

		pDoc->SaveFile();
	}

	pNode = pDoc->FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option")->FirstChild("ScanOption");
	if(pNode == NULL)
	{
		TiXmlElement* pElem;
		
		pElem = new TiXmlElement("ScanOption");
		pDoc->FirstChildElement("BBS_WiFiConfig")->FirstChild("Setting")->FirstChildElement("Option")->LinkEndChild( pElem );;

		pElem->SetAttribute("TurnOnAvailNetNoti", "");
		pElem->SetAttribute("ScanThreshold", "");
		pElem->SetAttribute("LockSSID", "");
		pElem->SetAttribute("NetToAccess", "");

		pDoc->SaveFile();
	}

	pNode = pDoc->FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option")->FirstChild("PowerOption");
	if(pNode == NULL)
	{
		TiXmlElement* pElem;
		
		pElem = new TiXmlElement("PowerOption");
		pDoc->FirstChildElement("BBS_WiFiConfig")->FirstChild("Setting")->FirstChildElement("Option")->LinkEndChild( pElem );

		pElem->SetAttribute("AllowSuspend", "");

		pDoc->SaveFile();
	}

	pNode = pDoc->FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("DefaultPreferNet");
	if(pNode == NULL)
	{
		TiXmlElement* pElem;
		
		pElem = new TiXmlElement("DefaultPreferNet");
		pDoc->FirstChildElement("BBS_WiFiConfig")->FirstChild("Setting")->LinkEndChild( pElem );

		pElem->SetAttribute("Enable", "");
		pElem->SetAttribute("DHCP", "");
		pElem->SetAttribute("Hidden", "");
		pElem->SetAttribute("IP", "");
		pElem->SetAttribute("Subnet", "");
		pElem->SetAttribute("Gateway", "");
		pElem->SetAttribute("DNS", "");
		pElem->SetAttribute("SSID", "");
		pElem->SetAttribute("Auth", "");
		pElem->SetAttribute("Encr", "");
		pElem->SetAttribute("Key", "");
		pElem->SetAttribute("KeyIdx", "");
		pElem->SetAttribute("Hex_16", "");
		
		pDoc->SaveFile();
	}

	return TRUE;
	
	
}


BOOL SetBkgPingCheckOpt(BBS_BKG_PINGCHECK_OPT pingOpt)
{
	TiXmlDocument doc;
	char szCustomDest[128] = {'\0',};

	if( CheckValidXML(&doc) == FALSE )
	{
		return FALSE;
	}

	wcstombs(szCustomDest, pingOpt.tszCustomDestAddr, _tcslen(pingOpt.tszCustomDestAddr) );

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

	if( CheckValidXML(&doc) == FALSE )
	{
		return FALSE;
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

	if( CheckValidXML(&doc) == FALSE )
	{
		return FALSE;
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
	

	if( CheckValidXML(&doc) == FALSE )
	{
		return FALSE;
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


BOOL SetPowerOpt(BOOL bAllowSuspendMode)
{
	TiXmlDocument doc;

	if( CheckValidXML(&doc) == FALSE )
	{
		return FALSE;
	}

	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option")->FirstChildElement("PowerOption");

	pElem->SetAttribute("AllowSuspend", bAllowSuspendMode);
	
	return doc.SaveFile();
}

BOOL GetPowerOpt(int *pnAllowSuspendMode)
{
	TiXmlDocument doc;

	int nAllowSuspendMode = 0;
	int nResult = TIXML_NO_ATTRIBUTE;


	if( CheckValidXML(&doc) == FALSE )
	{
		return FALSE;
	}

	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChild("Option")->FirstChildElement("PowerOption");

	nResult = pElem->QueryIntAttribute("AllowSuspend",  pnAllowSuspendMode);
	// 초기값이 설정되지 않았을때는, 연결된 상태에서 SuspendMode 허용 기본 설정
	if( nResult == TIXML_WRONG_TYPE )
	{
		*pnAllowSuspendMode = -1;
	}
	else if ( nResult == TIXML_NO_ATTRIBUTE )
	{
		RETAILMSG(1, (TEXT("[PSH] ERROR \r\n") ));
	}

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


	if( CheckValidXML(&doc) == FALSE )
	{
		return FALSE;
	}


	wcstombs(szIPAddr, defPrefNetInfo.tszIPAddr, _tcslen(defPrefNetInfo.tszIPAddr) );
	wcstombs(szSubNetMask, defPrefNetInfo.tszSubNetMask, _tcslen(defPrefNetInfo.tszSubNetMask) );
	wcstombs(szGateWay, defPrefNetInfo.tszGateWay, _tcslen(defPrefNetInfo.tszGateWay) );
	wcstombs(szDnsAddr, defPrefNetInfo.tszDnsAddr, _tcslen(defPrefNetInfo.tszDnsAddr) );
	wcstombs(szSSID, defPrefNetInfo.tszSSID, _tcslen(defPrefNetInfo.tszSSID) );
	wcstombs(szNetworkKey, defPrefNetInfo.tszNetworkKey, _tcslen(defPrefNetInfo.tszNetworkKey) );


	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChildElement("DefaultPreferNet");


	pElem->SetAttribute("Enable", defPrefNetInfo.bEnable);
	pElem->SetAttribute("DHCP", defPrefNetInfo.bDHCP);
	pElem->SetAttribute("Hidden", defPrefNetInfo.bHidden);
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

	const char *pszIPAddr = NULL;
	const char *pszSubNetMask = NULL;
	const char *pszGateWay = NULL;
	const char *pszDnsAddr = NULL;
	const char *pszSSID = NULL;
	const char *pszNetworkKey = NULL;


	if( CheckValidXML(&doc) == FALSE )
	{
		return FALSE;
	}

	TiXmlElement* pElem = doc.FirstChild("BBS_WiFiConfig")->FirstChild("Setting")->FirstChildElement("DefaultPreferNet");



	pElem->Attribute("Enable", (int*)&pDefPrefNetInfo->bEnable);
	pElem->Attribute("DHCP", (int*)&pDefPrefNetInfo->bDHCP);
	pElem->Attribute("Hidden", (int*)&pDefPrefNetInfo->bHidden);
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
