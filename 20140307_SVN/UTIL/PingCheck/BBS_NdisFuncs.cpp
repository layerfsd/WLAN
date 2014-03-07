#include "stdafx.h"
#include "BBS_NdisFuncs.h"


static CRITICAL_SECTION		g_csLock = {0,};


#define DEFAULT_BSSID_LIST_BUFSIZE	(8192)




BOOL RefreshBSSIDList(LPTSTR strAdpater)
{
	NDISUIO_SET_OID nso = {0,};
	DWORD dwBytesRet = 0;
	BOOL bRetval = FALSE;
	HANDLE hNdisUio = NULL;


	EnterCriticalSection(&g_csLock);

	nso.ptcDeviceName = strAdpater;
	nso.Oid = OID_802_11_BSSID_LIST_SCAN;

	hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

	if( hNdisUio != INVALID_HANDLE_VALUE )
	{
		if( !DeviceIoControl(hNdisUio, IOCTL_NDISUIO_SET_OID_VALUE, (void *)&nso, sizeof(NDISUIO_SET_OID), NULL, 0, &dwBytesRet, NULL) )
		{
			bRetval = FALSE;
		}
		else
		{
			bRetval = TRUE;
		}
	
		CloseHandle(hNdisUio);	
	}

	
	LeaveCriticalSection(&g_csLock);

	return bRetval;

}



BOOL GetBSSIDInfo(LPTSTR strAdapter, BYTE *pucMac)
{
	BYTE pBuffer[sizeof(NDISUIO_QUERY_OID) + sizeof(NDIS_802_11_MAC_ADDRESS)] = {0,};
	PNDISUIO_QUERY_OID pNDISUIOQueryOid = {0,};
	DWORD dwBytesReturned = 0;
	HANDLE hNdisUio = NULL;
	BOOL bRet = FALSE;

	pNDISUIOQueryOid=(PNDISUIO_QUERY_OID) pBuffer;

	pNDISUIOQueryOid->ptcDeviceName = (PTCHAR) strAdapter;
	pNDISUIOQueryOid->Oid = OID_802_11_BSSID;


	EnterCriticalSection(&g_csLock);


	hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

	if( hNdisUio != INVALID_HANDLE_VALUE )
	{
		if( !DeviceIoControl(hNdisUio, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer, sizeof(pBuffer), pBuffer, sizeof(pBuffer), &dwBytesReturned, NULL) )
		{
			bRet = FALSE;
		}
		else
		{
			if(dwBytesReturned > 0)
			{
				memcpy(pucMac, pNDISUIOQueryOid->Data, sizeof(NDIS_802_11_MAC_ADDRESS));
				bRet = TRUE;
			}
			else
			{
				ZeroMemory(pucMac, sizeof(pucMac));
				bRet = FALSE;
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

			// 얻어올 AP가 많아서 버퍼사이즈가 모자랄 경우 버퍼 사이즈 2배로 설정하고 다시한번
			if( dwErr == ERROR_INVALID_USER_BUFFER )
			{
				free(pucQueryBuf);
				
				pucQueryBuf = (BYTE*)malloc(DEFAULT_BSSID_LIST_BUFSIZE * 2);

				pQueryOid = (PNDISUIO_QUERY_OID) &pucQueryBuf[0];
				pQueryOid->Oid = OID_802_11_BSSID_LIST;
				pQueryOid->ptcDeviceName = strAdapter;

				if(! DeviceIoControl(hNdisUio, IOCTL_NDISUIO_QUERY_OID_VALUE, (LPVOID) &pucQueryBuf[0], DEFAULT_BSSID_LIST_BUFSIZE*2, (LPVOID) &pucQueryBuf[0], DEFAULT_BSSID_LIST_BUFSIZE*2, &dwBytesRet, NULL) )
				{
					bQuerySuccess = FALSE;
				}
				else
				{
					bQuerySuccess = TRUE;
				}
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
		else
		{
			bRet = FALSE;
		}


		
		CloseHandle(hNdisUio);
	}


	free(pucQueryBuf);

	LeaveCriticalSection(&g_csLock);

	return bRet;
}