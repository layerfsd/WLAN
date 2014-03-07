#include "stdafx.h"
#include "BBS_Version.h"



static CRITICAL_SECTION		g_csLock = {0,};
static BOOL bInitCS = FALSE;

#define DEFAULT_ADAPTERNAME_MMC		L"AR6K_SD1"
#define DEFAULT_ADAPTERNAME_USI		L"BCMSDDHD1"
#define USI_DLL_FILE_PATH			L"windows\\bcmsddhd.dll"
#define MMC_DLL_FILE_PATH			L"windows\\AR6K_NDIS_SDIO.dll"


#define NETUI_DLL_FILE_PATH			L"windows\\netui.dll"
#define WIFISVCMGR_DLL_FILE_PATH	L"windows\\WiFiSvcMgr.dll"

#define SDHC2_DLL_FILE_PATH			L"windows\\sdhc2.dll"
#define SDMMC_CH3_DLL_FILE_PATH		L"windows\\sdmmc_ch3.dll"


BOOL GetFirmwareVersion(LPTSTR strAdapter, CString &strFirmware)
{
	BOOL bRet = FALSE;

	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	RETAILMSG(1, (TEXT("[NETUI] GetFirmwareVersion \r\n") ));

	strFirmware = L"";

	if( wcscmp(strAdapter, DEFAULT_ADAPTERNAME_USI) == 0)	// USI
	{
		BYTE pBuffer[sizeof(NDISUIO_QUERY_OID) + WLC_IOCTL_SMLEN] = {0,};
		PNDISUIO_QUERY_OID pNDISUIOQueryOid = {0,};
		DWORD dwBytesReturned = 0;
		HANDLE hNdisUio = NULL;	

		pNDISUIOQueryOid=(PNDISUIO_QUERY_OID) pBuffer;

		pNDISUIOQueryOid->ptcDeviceName = (PTCHAR) strAdapter;
		pNDISUIOQueryOid->Oid = WL_OID_BASE + WLC_GET_VAR;
		memcpy(pNDISUIOQueryOid->Data, "ver", WLC_IOCTL_SMLEN);

		hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

		if( hNdisUio != INVALID_HANDLE_VALUE )
		{
			char szFirmwareSep[] = "version ";
			char szNvramSep[] = "nvram file version ";
			char szSeps[] = " \n )";
			char* pVer = NULL;

			if( DeviceIoControl(hNdisUio, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer, sizeof(pBuffer), pBuffer, sizeof(pBuffer), &dwBytesReturned, NULL) == TRUE )
			{
				if(dwBytesReturned > 0)
				{
					TCHAR tszVer_Nvram[20] = L"";
					TCHAR tszVer_Firmware[20] = L"";

					// NVRAM
					pVer = strstr((char*)pNDISUIOQueryOid->Data, szNvramSep);
					pVer += strlen(szNvramSep);
					pVer = strtok(pVer, szSeps);
					mbstowcs(tszVer_Nvram, pVer, strlen(pVer));

					// Firmware
					pVer = strstr((char*)pNDISUIOQueryOid->Data, szFirmwareSep);
					pVer += strlen(szFirmwareSep);
					pVer = strtok(pVer, szSeps);
					mbstowcs(tszVer_Firmware, pVer, strlen(pVer));

					strFirmware = tszVer_Firmware;
					strFirmware += L" / ";
					strFirmware += tszVer_Nvram;

					bRet = TRUE;

				}
			}


			CloseHandle(hNdisUio);
		}


	}
	else if( wcscmp(strAdapter, DEFAULT_ADAPTERNAME_MMC) == 0)	// MMC
	{
		HANDLE hdrgAdapter = NULL;
		DWORD dwReturnedBytes = 0;
		DWORD dwFirmware = 0;

		hdrgAdapter = CreateFile(TEXT("DRG1:"),GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			INVALID_HANDLE_VALUE);

		if( INVALID_HANDLE_VALUE != hdrgAdapter )
		{
			if( DeviceIoControl(hdrgAdapter, IOCTL_CAR6K_GET_FIRMWARE_VERSION, NULL, 0, &dwFirmware, sizeof(dwFirmware), &dwReturnedBytes, NULL) == TRUE )
			{
				strFirmware.Format(L"%d.%d.%d.%d", \
					(dwFirmware >> VER_MAJOR_BIT_OFFSET) & 0xF,	\
					(dwFirmware >> VER_MINOR_BIT_OFFSET) & 0xF,	\
					(dwFirmware >> VER_PATCH_BIT_OFFSET) & 0xFF,	\
					(dwFirmware >> VER_BUILD_NUM_BIT_OFFSET) & 0xFFFF );

				bRet = TRUE;
			}

			CloseHandle( hdrgAdapter );
		}
	}
	else // ETC
	{
		bRet = FALSE;
	}


	
	RETAILMSG(1, (TEXT("[NETUI] GetFirmwareVersion : %s(%d) \r\n"), strFirmware, bRet ));

	LeaveCriticalSection(&g_csLock);

	return bRet;
}







BOOL GetDriverVersion(LPTSTR strAdapter, CString &strDriver)
{
	BOOL bRet = FALSE;

	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	RETAILMSG(1, (TEXT("[NETUI] GetDriverVersion \r\n") ));

	strDriver = L"";

	if( wcscmp(strAdapter, DEFAULT_ADAPTERNAME_USI) == 0)	// USI
	{
		DWORD dwInfoSize = 0;
		char *pBuf = NULL;

		dwInfoSize = GetFileVersionInfoSize(USI_DLL_FILE_PATH, 0);

		pBuf = new char[dwInfoSize];

		if( dwInfoSize == 0 )
		{
			bRet = FALSE;
			goto error;
		}

		if(GetFileVersionInfo(USI_DLL_FILE_PATH, 0, dwInfoSize, pBuf) !=0 )
		{
			VS_FIXEDFILEINFO* pFineInfo = NULL;
			UINT bufLen = 0;
			if(VerQueryValue(pBuf, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
			{   
				bRet = TRUE;
				strDriver.Format(L"%d.%d.%d.%d", HIWORD(pFineInfo->dwFileVersionMS), LOWORD(pFineInfo->dwFileVersionMS), HIWORD(pFineInfo->dwFileVersionLS), LOWORD(pFineInfo->dwFileVersionLS) );
			}
		}

		delete pBuf;
	}
	else if( wcscmp(strAdapter, DEFAULT_ADAPTERNAME_MMC) == 0)	// MMC
	{
		HANDLE hdrgAdapter = NULL;
		DWORD dwReturnedBytes = 0;
		DWORD dwDriver = 0;

		hdrgAdapter = CreateFile(TEXT("DRG1:"),GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			INVALID_HANDLE_VALUE);

		if( INVALID_HANDLE_VALUE != hdrgAdapter )
		{
			if( DeviceIoControl(hdrgAdapter, IOCTL_CAR6K_GET_HOST_VERSION, NULL, 0, &dwDriver, sizeof(dwDriver), &dwReturnedBytes, NULL) == TRUE )
			{
				TCHAR tszTemp[128] = {0,};
				wsprintf(tszTemp, L"%d.%d.%d.%d(%d)", \
					(dwDriver >> VER_MAJOR_BIT_OFFSET) & 0xF,	\
					(dwDriver >> VER_MINOR_BIT_OFFSET) & 0xF,	\
					(dwDriver >> VER_PATCH_BIT_OFFSET) & 0xFF,	\
					(dwDriver >> VER_BUILD_NUM_BIT_OFFSET) & 0xFF,	\
					(dwDriver >> VER_MMC_BUILD_NUM_BIT_OFFSET) & 0xFF );
				
				bRet = TRUE;

				strDriver = tszTemp;

			}

			CloseHandle( hdrgAdapter );
		}

		


		// If Exist File Version
		{
			DWORD dwInfoSize = 0;
			char *pBuf = NULL;

			dwInfoSize = GetFileVersionInfoSize(MMC_DLL_FILE_PATH, 0);

			pBuf = new char[dwInfoSize];

			if( dwInfoSize == 0 )
			{
				goto error;
			}

			if(GetFileVersionInfo(MMC_DLL_FILE_PATH, 0, dwInfoSize, pBuf) !=0 )
			{
				VS_FIXEDFILEINFO* pFineInfo = NULL;
				UINT bufLen = 0;
				TCHAR tszTemp[128] = {0,};

				if(VerQueryValue(pBuf, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
				{   
					wsprintf(tszTemp, L"-%d.%d.%d.%d", HIWORD(pFineInfo->dwFileVersionMS), LOWORD(pFineInfo->dwFileVersionMS), HIWORD(pFineInfo->dwFileVersionLS), LOWORD(pFineInfo->dwFileVersionLS) );
				}

				strDriver += tszTemp;
			}

			delete pBuf;
		}


	}
	else // ETC
	{
		bRet = FALSE;
	}


	
	RETAILMSG(1, (TEXT("[NETUI] GetDriverVersion : %s \r\n"), strDriver ));

error:

	LeaveCriticalSection(&g_csLock);

	return bRet;
}


BOOL GetnetuiFileVersion(CString &strNetuiVer)
{
	BOOL bRet = FALSE;
	DWORD dwInfoSize = 0;

	char *pBuf = NULL;

	strNetuiVer = L"";


	dwInfoSize = GetFileVersionInfoSize(NETUI_DLL_FILE_PATH, 0);

	pBuf = new char[dwInfoSize];

	if( dwInfoSize == 0 )
	{
		bRet = FALSE;
	}

	if(GetFileVersionInfo(NETUI_DLL_FILE_PATH, 0, dwInfoSize, pBuf) !=0 )
	{
		VS_FIXEDFILEINFO* pFineInfo = NULL;
		UINT bufLen = 0;
		if(VerQueryValue(pBuf, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
		{
			bRet = TRUE;
			strNetuiVer.Format(_T("%d.%d.%d.%d"), HIWORD(pFineInfo->dwFileVersionMS), LOWORD(pFineInfo->dwFileVersionMS), HIWORD(pFineInfo->dwFileVersionLS), LOWORD(pFineInfo->dwFileVersionLS) );
		}
	}

	delete pBuf;

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Info::GetCurrentFileVersion()  strNetuiVer : %s \r\n"), strNetuiVer ));

	return bRet;
}


BOOL GetSDIODrvFileVersion(CString &strSDIODrvVer)
{
	BOOL bRet = FALSE;
	DWORD dwInfoSize = 0;
	CString strFilePath = L"";
	char *pBuf = NULL;


	strSDIODrvVer = L"";

	// sdmmc_ch3.dll 검사후 없으면 sdhc2.dll 검사
	strFilePath = SDMMC_CH3_DLL_FILE_PATH;
	dwInfoSize = GetFileVersionInfoSize(strFilePath.GetBuffer(), 0);

	if( dwInfoSize == 0 )
	{
		strFilePath = SDHC2_DLL_FILE_PATH;
		dwInfoSize = GetFileVersionInfoSize(strFilePath.GetBuffer(), 0);

		if(dwInfoSize == 0)
		{
			return FALSE;
		}
	}

	pBuf = new char[dwInfoSize];

	if(GetFileVersionInfo(strFilePath.GetBuffer(), 0, dwInfoSize, pBuf) !=0 )
	{
		VS_FIXEDFILEINFO* pFineInfo = NULL;
		UINT bufLen = 0;
		if(VerQueryValue(pBuf, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
		{
			bRet = TRUE;
			strSDIODrvVer.Format(_T("%d.%d.%d.%d"), HIWORD(pFineInfo->dwFileVersionMS), LOWORD(pFineInfo->dwFileVersionMS), HIWORD(pFineInfo->dwFileVersionLS), LOWORD(pFineInfo->dwFileVersionLS) );
		}
	}

	delete pBuf;

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Info::GetCurrentFileVersion()  strSDIODrvVer : %s \r\n"), strSDIODrvVer ));

	return bRet;
}

BOOL GetWiFiSvcMgrFileVersion(CString &strWiFiSvcMgrVer)
{
	BOOL bRet = FALSE;
	DWORD dwInfoSize = 0;
	char *pBuf = NULL;


	strWiFiSvcMgrVer = L"";

	// sdmmc_ch3.dll 검사후 없으면 sdhc2.dll 검사
	dwInfoSize = GetFileVersionInfoSize(WIFISVCMGR_DLL_FILE_PATH, 0);

	if( dwInfoSize == 0 )
	{
		return FALSE;
	}

	pBuf = new char[dwInfoSize];

	if(GetFileVersionInfo(WIFISVCMGR_DLL_FILE_PATH, 0, dwInfoSize, pBuf) !=0 )
	{
		VS_FIXEDFILEINFO* pFineInfo = NULL;
		UINT bufLen = 0;
		if(VerQueryValue(pBuf, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
		{
			bRet = TRUE;
			strWiFiSvcMgrVer.Format(_T("%d.%d.%d.%d"), HIWORD(pFineInfo->dwFileVersionMS), LOWORD(pFineInfo->dwFileVersionMS), HIWORD(pFineInfo->dwFileVersionLS), LOWORD(pFineInfo->dwFileVersionLS) );
		}
	}

	delete pBuf;

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Info::GetCurrentFileVersion()  strWiFiSvcMgrVer : %s \r\n"), strWiFiSvcMgrVer ));

	return bRet;

}