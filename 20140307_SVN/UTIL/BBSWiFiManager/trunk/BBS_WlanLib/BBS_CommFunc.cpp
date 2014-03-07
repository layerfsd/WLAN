#include "stdafx.h"
#include "BBS_CommFunc.h"
#include <windev.h>
#include "BBS_ControlSvcMgrDrv.h"







/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Query's WZC information for the specified adapter.
// Callers MUST deallocate Intf with WZCDeleteInterfaceEx.
BOOL QueryWzcIntf(IN  LPCWSTR pAdapter, OUT INTF_ENTRY_EX *pIntf, IN DWORD IntfFlags, OUT DWORD *pOutFlags); // IntfFlags : usually INTF_ALL

// print WZC configuration item
void PrintConfigItem(IN const WZC_WLAN_CONFIG &wzcConfig, IN BOOL bDetailed); // Print detailed info

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//
// Get Default Information (IP, GateWay, DNS,  DHCP, etc..)
void GetWlanDefaultInformation(PBBS_ADAPTER_INFO pInformation)
{
	
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	PFIXED_INFO pNetworkParams = NULL;

	ULONG ulSizeAdapterInfo = 0;
	DWORD dwReturnvalueGetAdapterInfo = 0;
	TCHAR tszAdapterNameTemp[MAX_ADAPTER_NAME_LENGTH + 4] = {'\0',};
	ULONG uSizeNetworkParams = 0;
	DWORD dwReturnvalueGetNetworkParams;

	BOOL bFindWirelessAdapter = FALSE;

	pInformation->tszAdapterName[0] = L'\0';
	memset( pInformation, 0x0, sizeof(BBS_ADAPTER_INFO) );

	// -----------------------GetAdaptersInfo-----------------------------------------

	dwReturnvalueGetAdapterInfo = GetAdaptersInfo( pAdapterInfo, &ulSizeAdapterInfo );
	if ( dwReturnvalueGetAdapterInfo == ERROR_BUFFER_OVERFLOW)
	{
		if (!(pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulSizeAdapterInfo)))
		{
			return;
		}

		dwReturnvalueGetAdapterInfo = GetAdaptersInfo( pAdapterInfo, &ulSizeAdapterInfo);
		if (dwReturnvalueGetAdapterInfo != ERROR_SUCCESS)
		{
			free(pAdapterInfo);
			return;
		}

		while (pAdapterInfo != NULL)
		{
			mbstowcs( tszAdapterNameTemp, pAdapterInfo->AdapterName, MAX_ADAPTER_NAME_LENGTH + 4 );

			// Wireless 아답터 인지 확인
			// Nids의 OID_GEN_PHYSICAL_MEDIUM 로 NdisPhysicalMediumWirelessLan 임을 확인하는 방법이 있으나, 아답터 전원Off 상태에서는 조회 되지 않음
			// Summit 모듈도 확인하기 위해서, Wzctool은 사용하지 않음

			for(int i=0; i< sizeof(ManageableAdapters) / sizeof(ManageableAdapters[0]); i++)
			{
				if( _tcscmp(tszAdapterNameTemp, ManageableAdapters[i]) == 0 )
				{
					bFindWirelessAdapter = TRUE;
				}
			}

			if( bFindWirelessAdapter ) 
			{
				// Adapter Name
				_tcscpy(pInformation->tszAdapterName, tszAdapterNameTemp);

				// IPAddr
				mbstowcs( pInformation->tszIPAddr, pAdapterInfo->IpAddressList.IpAddress.String, sizeof(pAdapterInfo->IpAddressList.IpAddress.String) );

				// Subnet Mask
				mbstowcs( pInformation->tszSubNetMask, pAdapterInfo->IpAddressList.IpMask.String, sizeof(pAdapterInfo->IpAddressList.IpMask.String) );

				// Default GateWay
				mbstowcs( pInformation->tszDefaultGateWay, pAdapterInfo->GatewayList.IpAddress.String, sizeof(pAdapterInfo->GatewayList.IpAddress.String) );

				// Mac Address
				_stprintf( pInformation->tszMacAddr, _T("%02X:%02X:%02X:%02X:%02X:%02X"), pAdapterInfo->Address[0],pAdapterInfo->Address[1],pAdapterInfo->Address[2],pAdapterInfo->Address[3],pAdapterInfo->Address[4],pAdapterInfo->Address[5] );

				// DHCP Enabled
				pInformation->bDHCPEnabled = (BOOL)pAdapterInfo->DhcpEnabled;

				break;
			}
			pAdapterInfo = pAdapterInfo->Next;
		}
	}

	if(pAdapterInfo)
	{
		free(pAdapterInfo);
	}


	// ---------------------GetNetwork Params --------------------------

	dwReturnvalueGetNetworkParams = GetNetworkParams(pNetworkParams , &uSizeNetworkParams);
	if (dwReturnvalueGetNetworkParams == ERROR_BUFFER_OVERFLOW)
	{
		if (!(pNetworkParams = (PFIXED_INFO) malloc(uSizeNetworkParams)))
		{
			return;
		}

		dwReturnvalueGetNetworkParams = GetNetworkParams(pNetworkParams, &uSizeNetworkParams);
		if (dwReturnvalueGetNetworkParams != ERROR_SUCCESS)
		{
			free(pNetworkParams);
			return;
		}
	}

	// Dns Server
	mbstowcs( pInformation->tszDnsAddr, pNetworkParams->DnsServerList.IpAddress.String, sizeof(pNetworkParams->DnsServerList.IpAddress.String) );


	if(pNetworkParams)
	{
		free(pNetworkParams);
	}

	return;
}


// Translates from 802.11 frequency (in kHz) to channel-number.
int ChannelFreq2Number(long Frequency)
{
    long freq = (Frequency + 500) / 1000;
    int  chan;

    if (freq < 2412)        // Too low
        chan = 0;
    else
    if (freq < 2484)        // Channels 1-13
        chan = 1 + (freq - 2412) / 5;
    else
    if (freq == 2484)       // Channel 14
        chan = 14;
    else
    if (freq < 5000)        // Channels 15-26
        chan = 15 + ((freq - 2512) / 20);
    else
        chan = (freq - 5000) / 5;

    return chan;
} // ChannelFreq2Number()








/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ZeroConfig
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL QueryWzcContext(WZC_CONTEXT *pContext)
{
	DWORD dwOutFlags = 0;

    
    if( WZCQueryContext(NULL, 0, pContext, &dwOutFlags) != ERROR_SUCCESS )
    {
        return FALSE;
    }

    return TRUE;
} 

BOOL SetWzcParameter(WZC_CONTEXT wzcContext)
{
    DWORD dwOutFlags = 0;


	wzcContext.tmTr = ((wzcContext.tmTr==0)? TMMS_DEFAULT_TR :
			((wzcContext.tmTr<0) ? TMMS_INFINITE : wzcContext.tmTr));


	wzcContext.tmTp = ((wzcContext.tmTp==0)? BB_DEFAULT_TMTP :
			((wzcContext.tmTp<0) ? TMMS_INFINITE : wzcContext.tmTp));


	wzcContext.tmTc = ((wzcContext.tmTc==0)? TMMS_DEFAULT_TC :
			((wzcContext.tmTc<0) ? TMMS_INFINITE : wzcContext.tmTc));


	wzcContext.tmTf = ((wzcContext.tmTf==0)? BB_DEFAULT_TMTF :
			((wzcContext.tmTf<0) ? TMMS_INFINITE : wzcContext.tmTf));

	wzcContext.tmTd = ((wzcContext.tmTd==0)? TMMS_DEFAULT_TD :
			((wzcContext.tmTd<0) ? TMMS_INFINITE : wzcContext.tmTd));


	if ( WZCSetContext(NULL, 0x00, &wzcContext, NULL) != ERROR_SUCCESS )
	{
		return FALSE;
	}

    return TRUE;
}


// -----------------------------------------------------------------------------
void
EncryptWepKMaterial
// encrypt WEP key material
// note: this is simply for the security (to protect from memory scanning)
(
    IN OUT WZC_WLAN_CONFIG *pwzcConfig
)
{
    BYTE chFakeKeyMaterial[] = { 0x56, 0x09, 0x08, 0x98, 0x4D, 0x08, 0x11, 0x66, 0x42, 0x03, 0x01, 0x67, 0x66 };
    for (int i = 0; i < WZCCTL_MAX_WEPK_MATERIAL; i++)
        pwzcConfig->KeyMaterial[i] ^= chFakeKeyMaterial[(7*i)%13];
} // EncryptWepKMaterial()


// -----------------------------------------------------------------------------
BOOL QueryWzcIntf
// Query's WZC information for the specified adapter.
// Callers MUST deallocate Intf with WZCDeleteInterfaceEx.
(
    IN  LPCWSTR        pAdapter,
    OUT INTF_ENTRY_EX *pIntf,
    IN  DWORD          IntfFlags, // usually INTF_ALL
    OUT DWORD         *pOutFlags
)
{
    memset(pIntf, 0x00, sizeof(INTF_ENTRY_EX));
    pIntf->wszGuid = const_cast<LPWSTR>(pAdapter);

    DWORD result = WZCQueryInterfaceEx(NULL,
                                       IntfFlags,
                                       pIntf,
                                       pOutFlags);
    if (NO_ERROR != result)
    {
        if (ERROR_FILE_NOT_FOUND == result)

            NKDbgPrintfW(L"WZCQueryInterfaceEx() error %u: adapter \"%s\" not found \r\n", result, pAdapter);
        else
            NKDbgPrintfW(L"WZCQueryInterfaceEx() error %u out-flags 0x%08X \r\n", result, *pOutFlags);
        return FALSE;
    }

    return TRUE;
} // QueryWzcIntf()


// -----------------------------------------------------------------------------
BOOL AddToPreferredNetworkList
// adding to the Preferred Networks List
// Preferred Networks is a list of SSIDs in preference order.
// WZC continuously scans available SSIDs and attempts to connect to the most preferable SSID.
(
    IN LPCWSTR                pAdapter,
    IN const WZC_WLAN_CONFIG &wzcConfig
)
{
    DWORD dwOutFlags = 0;
    INTF_ENTRY_EX Intf;
	TCHAR tszLogMsg[128] = L"";

    if (!QueryWzcIntf(pAdapter, &Intf, INTF_ALL, &dwOutFlags))
        return FALSE;

    WZC_802_11_CONFIG_LIST *pConfigList = (PWZC_802_11_CONFIG_LIST)Intf.rdStSSIDList.pData;
    if (NULL == pConfigList)   // empty [Preferred Networks] list case
    {
        DWORD dwDataLen = sizeof(WZC_802_11_CONFIG_LIST);
        WZC_802_11_CONFIG_LIST *pNewConfigList = (WZC_802_11_CONFIG_LIST *)LocalAlloc(LPTR, dwDataLen);
        pNewConfigList->NumberOfItems = 1;
        pNewConfigList->Index = 0;

        memcpy(pNewConfigList->Config, &wzcConfig, sizeof(wzcConfig));

        Intf.rdStSSIDList.pData = (BYTE*)pNewConfigList;
        Intf.rdStSSIDList.dwDataLen = dwDataLen;

		wsprintf(tszLogMsg, L"[AddPref] Preferred Networks List has no entries \r\n");
		AddLogData(1, tszLogMsg);
    }
    else
    {
        ULONG uiNumberOfItems = pConfigList->NumberOfItems;
        for (UINT i=0; i<uiNumberOfItems; i++)
        {
            if (memcmp(&wzcConfig.Ssid, &pConfigList->Config[i].Ssid, sizeof(NDIS_802_11_SSID)) == 0)
            {
				wsprintf(tszLogMsg, L"[AddPref] already in the Preferred Networks list \r\n");
				AddLogData(1, tszLogMsg);
                WZCDeleteIntfObjEx(&Intf);
                return TRUE;
            }
        }

		wsprintf(tszLogMsg, L"[AddPref] Preferred Networks List has [%d] entries \r\n", uiNumberOfItems);
		AddLogData(1, tszLogMsg);

        DWORD dwDataLen = sizeof(WZC_802_11_CONFIG_LIST) + (uiNumberOfItems+1)*sizeof(WZC_WLAN_CONFIG);
        WZC_802_11_CONFIG_LIST *pNewConfigList = (WZC_802_11_CONFIG_LIST *)LocalAlloc(LPTR, dwDataLen);
        pNewConfigList->NumberOfItems = uiNumberOfItems + 1;
        pNewConfigList->Index = 0;

        memcpy(pNewConfigList->Config, &wzcConfig, sizeof(wzcConfig));

        if (pConfigList->NumberOfItems)
        {
            pNewConfigList->Index = pConfigList->Index;
            memcpy(pNewConfigList->Config+1, pConfigList->Config, (uiNumberOfItems)*sizeof(WZC_WLAN_CONFIG));
            LocalFree(pConfigList);
            pConfigList = NULL;
        }

        Intf.rdStSSIDList.pData = (BYTE*)pNewConfigList;
        Intf.rdStSSIDList.dwDataLen = dwDataLen;
    }

    PrintConfigItem(wzcConfig, FALSE);

    DWORD result = WZCSetInterfaceEx(NULL, INTF_PREFLIST, &Intf, &dwOutFlags);
    if (NO_ERROR != result)
	{
		NKDbgPrintfW( L"WZCSetInterfaceEx() error %u, dwOutFlags=0x%08X \r\n", result, dwOutFlags);
	}

    WZCDeleteIntfObjEx(&Intf);

    return result? FALSE : TRUE;
}// AddToPreferredNetworkList()



// -----------------------------------------------------------------------------
void
PrintConfigItem
// print WZC configuration item
(
    IN const WZC_WLAN_CONFIG &wzcConfig,
    IN BOOL                   bDetailed  // Print detailed info
)
{
	/*
    WCHAR buff[1024];
    DWORD buffChars = COUNTOF(buff);

    NKDbgPrintfW(L"   Length                  = %d bytes.", wzcConfig.Length);
    NKDbgPrintfW(L"   dwCtlFlags              = 0x%08X", wzcConfig.dwCtlFlags);

    if (bDetailed)
    {
        NKDbgPrintfW(L"   MacAddress              = %s", FormatMacAddress(wzcConfig.MacAddress, buff, buffChars));
    }

    NKDbgPrintfW(L"   SSID                    = %s", FormatSsid(wzcConfig.Ssid, buff, buffChars));

    NKDbgPrintfW(L"   Authentication          = %d %s", wzcConfig.AuthenticationMode, AuthenticationMode2Text(wzcConfig.AuthenticationMode));

    LPCWSTR pCipherFormat;
    if (wzcConfig.Privacy == Ndis802_11WEPDisabled)
    {
        pCipherFormat = L"   Privacy                 = %d"
                        L" %s (wireless data is not encrypted)";
    }
    else
    {
        pCipherFormat = L"   Privacy                 = %d"
                        L" enabled (encrypted with [%s])";
    }
    NKDbgPrintfW(pCipherFormat, wzcConfig.Privacy, CipherMode2Text(wzcConfig.Privacy));

    NKDbgPrintfW(L"   Infrastructure          = %s", InfrastructureMode2Text(wzcConfig.InfrastructureMode));

    if (bDetailed)
    {
        NKDbgPrintfW(L"   RSSI                    = %d dBm (0=excellent, -100=weak signal)", wzcConfig.Rssi);

        NKDbgPrintfW(L"   NetworkTypeInUse        = %s", NetworkType2Text(wzcConfig.NetworkTypeInUse));

        NKDbgPrintfW(L"   Configuration:");
        NKDbgPrintfW(L"      Struct Length        = %d", wzcConfig.Configuration.Length);
        NKDbgPrintfW(L"      BeaconPeriod         = %d kusec", wzcConfig.Configuration.BeaconPeriod);
        NKDbgPrintfW(L"      ATIMWindow           = %d kusec", wzcConfig.Configuration.ATIMWindow);
        NKDbgPrintfW(L"      DSConfig             = %d kHz (ch-%d)", wzcConfig.Configuration.DSConfig, ChannelFreq2Number(wzcConfig.Configuration.DSConfig));
        NKDbgPrintfW(L"      FHConfig:");
        NKDbgPrintfW(L"         Struct Length     = %d" ,wzcConfig.Configuration.FHConfig.Length);
        NKDbgPrintfW(L"         HopPattern        = %d", wzcConfig.Configuration.FHConfig.HopPattern);
        NKDbgPrintfW(L"         HopSet            = %d", wzcConfig.Configuration.FHConfig.HopSet);
        NKDbgPrintfW(L"         DwellTime         = %d", wzcConfig.Configuration.FHConfig.DwellTime);

        buff[0] = L'\0';
        DWORD charsUsed = 0;
        DWORD charsLeft = buffChars;
        WCHAR separator = L' ';
        for (int j=0; j<NDIS_802_11_LENGTH_RATES; j++)
        {
            if (wzcConfig.SupportedRates[j])
            {
                buff[charsUsed] = separator;
                separator = L',';
                charsUsed++;
                charsLeft--;

                size_t chars = FormatSupportedRate(wzcConfig.SupportedRates[j],
                                                  &buff[charsUsed], charsLeft);
                charsUsed += chars;
                charsLeft -= chars;
            }
        }
        NKDbgPrintfW(L"   SupportedRates          =%s (Mbit/s)", buff);

        NKDbgPrintfW(L"   KeyIndex                = <not available> (beaconing packets don't have this info)"); // wzcConfig.KeyIndex
        NKDbgPrintfW(L"   KeyLength               = <not available> (beaconing packets don't have this info)"); // wzcConfig.KeyLength
        NKDbgPrintfW(L"   KeyMaterial             = <not available> (beaconing packets don't have this info)");

        NKDbgPrintfW(L"   rdUserData length       = %d bytes.",  wzcConfig.rdUserData.dwDataLen);
    }
	*/
} // PrintConfigItem()










/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void GetFlashStoreName(LPTSTR strName)
{
	DWORD dwDataSize = 0;
	TCHAR tszData[128] = {'\0',};
	LONG lRet = 0;
	HKEY hKey = NULL;

	_tcscpy(strName, L"");
	// Get Log Folder Name
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, RK_FLASH_STORE_PATH, 0, 0, &hKey) == ERROR_SUCCESS )
	{
		TCHAR tszData[128] = {'\0',};
		dwDataSize = sizeof(tszData);
		lRet = RegQueryValueEx(hKey, RV_FLASH_STORE_NAME, NULL, NULL, (LPBYTE)&tszData, &dwDataSize);
		if( (lRet == ERROR_SUCCESS) || ( _tcslen(tszData) > 0 ) )
		{
			_tcscpy(strName, tszData);
		}
		RegCloseKey(hKey);
		hKey = NULL;
	}
}

BOOL CreateFolderIfNotFoundDir(LPTSTR strFullPath)
{
	CString strCopyPath = L"";
	CString strDirectory = L"";
	
	WIN32_FIND_DATA findData = {0,};
	HANDLE hFind = NULL;

	TCHAR *pToken = NULL;
	TCHAR tszSep[] = L"\\";
	

	// 마지막 \\ 문자 다음에는 파일명임
	// 폴더만 검색하여, 없는 폴더 생성하기 위해서, 마지막 \\ 이후의 파일명은 포함시키지 않는다.
	strCopyPath = strFullPath;
	strCopyPath = strCopyPath.Left(strCopyPath.ReverseFind(L'\\'));
	

	pToken = _tcstok( strCopyPath.GetBuffer(), tszSep );

	while(pToken)
	{
		strDirectory += tszSep;
		strDirectory += pToken;

		hFind = FindFirstFile(strDirectory, &findData);

		if(hFind == INVALID_HANDLE_VALUE)
		{
			DWORD dwErr = GetLastError();
			if( dwErr == ERROR_PATH_NOT_FOUND || dwErr == ERROR_NO_MORE_FILES )
			{
				if( !CreateDirectory(strDirectory, NULL) )
				{
					FindClose(hFind);
					return FALSE;
				}
			}

		}
		FindClose(hFind);

		pToken = _tcstok( NULL, tszSep );
	}

	return TRUE;
}



int GetRoamingTriggerValue(LPTSTR strAdapter)
{
	int nRet = 0;
	CString strReg = L"";
	HKEY hKey = NULL;

	strReg.Format(L"Comm\\%s\\Parms", strAdapter);



	// MMC?
	if( _tcscmp(strAdapter, ManageableAdapters[0]) == 0 )
	{
		if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, strReg, 0, 0, &hKey) == ERROR_SUCCESS )
		{
			DWORD dwData = 0;
			DWORD dwType = REG_DWORD;
			DWORD dwDataSize = sizeof(dwData);

			if( RegQueryValueEx(hKey, L"RoamThreshold", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize) == ERROR_SUCCESS )
			{
				nRet = dwData;
			}
		}
	}
	// USI?
	else if( _tcscmp(strAdapter, ManageableAdapters[1]) == 0 )
	{

		if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, strReg, 0, 0, &hKey) == ERROR_SUCCESS )
		{
			TCHAR tszData[128] = {0,};
			DWORD dwType = REG_SZ;
			DWORD dwDataSize = sizeof(tszData);

			if( RegQueryValueEx(hKey, L"RoamTrigger", NULL, &dwType, (LPBYTE)&tszData, &dwDataSize) == ERROR_SUCCESS )
			{
				nRet = _ttoi(tszData);
			}
		}
	}
	else
	{
		nRet = 0;
	}
	

	if(hKey)
	{
		RegCloseKey(hKey);
	}

	return nRet;

}