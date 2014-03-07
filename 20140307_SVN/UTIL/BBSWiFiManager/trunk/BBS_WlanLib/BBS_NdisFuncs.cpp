#include "stdafx.h"
#include "BBS_NdisFuncs.h"


static CRITICAL_SECTION		g_csLock = {0,};
static BOOL bInitCS = FALSE;

#define DEFAULT_BSSID_LIST_BUFSIZE	(8192)




BOOL RefreshBSSIDList(LPTSTR strAdpater)
{
	NDISUIO_SET_OID nso = {0,};
	DWORD dwBytesRet = 0;
	BOOL bRetval = FALSE;
	HANDLE hNdisUio = NULL;


	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	nso.ptcDeviceName = strAdpater;
	nso.Oid = OID_802_11_BSSID_LIST_SCAN;

	hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

	if( hNdisUio != INVALID_HANDLE_VALUE )
	{
		if( DeviceIoControl(hNdisUio, IOCTL_NDISUIO_SET_OID_VALUE, (void *)&nso, sizeof(NDISUIO_SET_OID), NULL, 0, &dwBytesRet, NULL) == TRUE )
		{
			bRetval = TRUE;
		}
	
		CloseHandle(hNdisUio);	
	}

	
	LeaveCriticalSection(&g_csLock);

	return bRetval;

}

BOOL GetSSID(LPTSTR strAdapter, LPTSTR strSSID)
{
	BYTE pBuffer[sizeof(NDISUIO_QUERY_OID) + sizeof(NDIS_802_11_SSID)] = {0,};
	PNDISUIO_QUERY_OID pNDISUIOQueryOid = {0,};
	DWORD dwBytesReturned = 0;
	HANDLE hNdisUio = NULL;
	BOOL bRet = FALSE;

	pNDISUIOQueryOid=(PNDISUIO_QUERY_OID) pBuffer;

	pNDISUIOQueryOid->ptcDeviceName = (PTCHAR) strAdapter;
	pNDISUIOQueryOid->Oid = OID_802_11_SSID;

	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	ZeroMemory(strSSID, sizeof(strSSID));


	hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

	if( hNdisUio != INVALID_HANDLE_VALUE )
	{
		if( DeviceIoControl(hNdisUio, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer, sizeof(pBuffer), pBuffer, sizeof(pBuffer), &dwBytesReturned, NULL) == TRUE )
		{
			if(dwBytesReturned > 0)
			{
				PNDIS_802_11_SSID pSSID =  (PNDIS_802_11_SSID)pNDISUIOQueryOid->Data;
				mbstowcs(strSSID, (char*)pSSID->Ssid, pSSID->SsidLength);

				bRet = TRUE;
			}
		}

		
		CloseHandle(hNdisUio);
	}
	

	LeaveCriticalSection(&g_csLock);


	return bRet;
}

BOOL GetRSSI(LPTSTR strAdapter, int* pnRSSI)
{
	BYTE pBuffer[sizeof(NDISUIO_QUERY_OID) + sizeof(NDIS_802_11_RSSI)] = {0,};
	PNDISUIO_QUERY_OID pNDISUIOQueryOid = {0,};
	DWORD dwBytesReturned = 0;
	HANDLE hNdisUio = NULL;
	BOOL bRet = FALSE;

	pNDISUIOQueryOid=(PNDISUIO_QUERY_OID) pBuffer;

	pNDISUIOQueryOid->ptcDeviceName = (PTCHAR) strAdapter;
	pNDISUIOQueryOid->Oid = OID_802_11_RSSI;


	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	ZeroMemory(pnRSSI, sizeof(pnRSSI));

	hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

	if( hNdisUio != INVALID_HANDLE_VALUE )
	{
		if( DeviceIoControl(hNdisUio, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer, sizeof(pBuffer), pBuffer, sizeof(pBuffer), &dwBytesReturned, NULL) == TRUE )
		{
			if(dwBytesReturned > 0)
			{

				memcpy(pnRSSI, (NDIS_802_11_RSSI*)pNDISUIOQueryOid->Data, sizeof(NDIS_802_11_RSSI));
				bRet = TRUE;
			}
		}

		
		CloseHandle(hNdisUio);
	}
	

	LeaveCriticalSection(&g_csLock);


	return bRet;
}




BOOL GetBSSID(LPTSTR strAdapter, BYTE *pucMac)
{
	BYTE pBuffer[sizeof(NDISUIO_QUERY_OID) + sizeof(NDIS_802_11_MAC_ADDRESS)] = {0,};
	PNDISUIO_QUERY_OID pNDISUIOQueryOid = {0,};
	DWORD dwBytesReturned = 0;
	HANDLE hNdisUio = NULL;
	BOOL bRet = FALSE;

	pNDISUIOQueryOid=(PNDISUIO_QUERY_OID) pBuffer;

	pNDISUIOQueryOid->ptcDeviceName = (PTCHAR) strAdapter;
	pNDISUIOQueryOid->Oid = OID_802_11_BSSID;


	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	ZeroMemory(pucMac, sizeof(pucMac));


	
	hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

	if( hNdisUio != INVALID_HANDLE_VALUE )
	{
		if( DeviceIoControl(hNdisUio, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer, sizeof(pBuffer), pBuffer, sizeof(pBuffer), &dwBytesReturned, NULL) == TRUE )
		{
			if(dwBytesReturned > 0)
			{
				memcpy(pucMac, pNDISUIOQueryOid->Data, sizeof(NDIS_802_11_MAC_ADDRESS));
				bRet = TRUE;
			}
		}

		
		CloseHandle(hNdisUio);
	}

	
	

	LeaveCriticalSection(&g_csLock);


	return bRet;
}



BOOL GetBBSIDListInfo(LPTSTR strAdapter, PNDIS_WLAN_BSSID pNdisWlanBSSID, DWORD *dwReturnedItems)
{
	DWORD dwBytesRet = 0;
	BYTE *pucQueryBuf = NULL;

	PNDIS_802_11_BSSID_LIST pBssidList = {0,};
	PNDIS_WLAN_BSSID pBssid = {0,};
	PNDISUIO_QUERY_OID pQueryOid = {0,};

	HANDLE hNdisUio = NULL;

	BOOL bRet = FALSE;



	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	pucQueryBuf = (BYTE*)malloc(DEFAULT_BSSID_LIST_BUFSIZE);

	pQueryOid = (PNDISUIO_QUERY_OID) &pucQueryBuf[0];
	pQueryOid->Oid = OID_802_11_BSSID_LIST;
	pQueryOid->ptcDeviceName = strAdapter;
	


	hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);
	
	if( hNdisUio != INVALID_HANDLE_VALUE )
	{
		// DeviceIoControl이 성공했는지 검사용
		BOOL bQuerySuccess = FALSE;
		

		if(! DeviceIoControl(hNdisUio, IOCTL_NDISUIO_QUERY_OID_VALUE, (LPVOID) &pucQueryBuf[0], DEFAULT_BSSID_LIST_BUFSIZE, (LPVOID) &pucQueryBuf[0], DEFAULT_BSSID_LIST_BUFSIZE, &dwBytesRet, NULL) )
		{
			DWORD dwErr = GetLastError();

			bQuerySuccess =  FALSE;

			// 얻어올 AP가 많아서 버퍼사이즈가 모자랄 경우
			if( dwErr == ERROR_INVALID_USER_BUFFER )
			{
				// 버퍼 사이즈 부족시 2, 4, 6, 8배씩 증가시키면서 재시도
				for(int i=2; i<10; i+=2)
				{
					free(pucQueryBuf);

					pucQueryBuf = (BYTE*)malloc(DEFAULT_BSSID_LIST_BUFSIZE * i);

					pQueryOid = (PNDISUIO_QUERY_OID) &pucQueryBuf[0];
					pQueryOid->Oid = OID_802_11_BSSID_LIST;
					pQueryOid->ptcDeviceName = strAdapter;

					if( DeviceIoControl(hNdisUio, IOCTL_NDISUIO_QUERY_OID_VALUE, (LPVOID) &pucQueryBuf[0], DEFAULT_BSSID_LIST_BUFSIZE * i, (LPVOID) &pucQueryBuf[0], DEFAULT_BSSID_LIST_BUFSIZE * i, &dwBytesRet, NULL) == TRUE )
					{
						bQuerySuccess = TRUE;
						break;
					}

				}
			}
			else
			{
				// Error Unknown!
			}

		}
		else
		{
			bQuerySuccess = TRUE;
		}



		if(bQuerySuccess)
		{
			pBssidList = (PNDIS_802_11_BSSID_LIST)pQueryOid->Data;

			pBssid = pBssidList->Bssid;

			*dwReturnedItems = pBssidList->NumberOfItems;

			for(DWORD dwIdx=0; dwIdx< pBssidList->NumberOfItems; dwIdx++)
			{
				BYTE *pByte = NULL;

				memcpy( &pNdisWlanBSSID[dwIdx], pBssid, sizeof(NDIS_WLAN_BSSID) );

				pByte = (BYTE*)pBssid;
				pByte += pBssid->Length;
				pBssid = (PNDIS_WLAN_BSSID)pByte;
			}

			bRet = TRUE;
		}


		
		CloseHandle(hNdisUio);
	}


	free(pucQueryBuf);

	LeaveCriticalSection(&g_csLock);

	return bRet;
}


BOOL GetNICStatistics(LPTSTR strAdapter, NIC_STATISTICS* pns)
{
	HANDLE hNdisUio = NULL;
    DWORD dwBytesWritten = 0;
	BOOL bRetval = FALSE;

	memset(pns, 0, sizeof(NIC_STATISTICS) );
    
	if( !bInitCS )
	{
		InitializeCriticalSection(&g_csLock);
		bInitCS = TRUE;
	}
	EnterCriticalSection(&g_csLock);

	hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

	if( hNdisUio != INVALID_HANDLE_VALUE )
	{
		pns->ptcDeviceName = strAdapter;

		if( DeviceIoControl(hNdisUio, IOCTL_NDISUIO_NIC_STATISTICS, NULL, 0, pns, sizeof(NIC_STATISTICS), &dwBytesWritten, NULL) == TRUE );
		{
			bRetval = TRUE;
		}

		CloseHandle(hNdisUio);	
	}


	LeaveCriticalSection(&g_csLock);

	return bRetval;
    
}



BOOL DoNdis_RebindAdater(LPTSTR tszAdapter)
{
	HANDLE hNdis;
	BOOL bResult = FALSE;
	DWORD cbOutBuffer = 0;

	hNdis = CreateFile(DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);


	if (INVALID_HANDLE_VALUE != hNdis)
	{
		bResult = DeviceIoControl(hNdis, IOCTL_NDIS_REBIND_ADAPTER, (LPVOID)tszAdapter, (_tcslen(tszAdapter) + 2) * sizeof(TCHAR), NULL, 0, &cbOutBuffer, NULL);

		CloseHandle(hNdis);
	} 

	return bResult;
}
