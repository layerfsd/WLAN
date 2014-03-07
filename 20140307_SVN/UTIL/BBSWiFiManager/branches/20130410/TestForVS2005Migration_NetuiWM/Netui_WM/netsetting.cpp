
#include "stdafx.h"
#include "Netui_WM.h"
#include "NetSettingPropertySheet.h"


#define BBS_WLAN_NAME	_T("BBS_WIFI_MANAGER")



HRESULT WINAPI NetCardsSettingDialog(LPCTSTR pszClass)
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, BBS_WLAN_NAME);
	
	if(!hMutex) 
    {
		RETAILMSG(1, (TEXT("[NETUI] NetCardsSettingDialog  !hMutex  \r\n")));
        return S_FALSE;
    }

    if(ERROR_ALREADY_EXISTS==GetLastError())
    {
		RETAILMSG(1, (TEXT("[NETUI] NetCardsSettingDialog  ERROR_ALREADY_EXISTS  \r\n")));
		
		HWND hWnd = FindWindow(NULL, BBS_WLAN_NAME);
		SetForegroundWindow(hWnd);
		ShowWindow(hWnd, SW_SHOW);

		CloseHandle(hMutex);
        return S_OK;
    }
    else
    {
		RETAILMSG(1, (TEXT("[NETUI] NetCardsSettingDialog  New!  \r\n")));
		CNetSettingPropertySheet MySheet( BBS_WLAN_NAME );
		MySheet.DoModal();
    }


	RETAILMSG(1, (TEXT("[NETUI] NetCardsSettingDialog  Close!  \r\n")));
	
	
	ReleaseMutex(hMutex);
	CloseHandle(hMutex);

	return S_OK;

}
