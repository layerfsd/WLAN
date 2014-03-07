// Setup.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ce_setup.h"

#include <string>

#define REG_AR6K_SD1_PARMS_PATH		L"Comm\\AR6K_SD1\\Parms"


void DeleteValues(HKEY hk);

extern "C" BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}

extern "C" codeINSTALL_INIT Install_Init(HWND hwndParent, BOOL fFirstCall, BOOL fPreviouslyInstalled, LPCTSTR pszInstallDir)
{
	HKEY hKey = NULL;

	// Delete Prev DLL
	DeleteFile(_T("Windows\\AR6K_NDIS_SDIO.dll_Back"));
	MoveFile(_T("Windows\\AR6K_NDIS_SDIO.dll"), _T("Windows\\AR6K_NDIS_SDIO.dll_Back"));

	// Delete All Registry Value
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_AR6K_SD1_PARMS_PATH, 0, 0, &hKey))
	{
		DeleteValues(hKey);
		RegCloseKey(hKey);
	}

    return codeINSTALL_INIT_CONTINUE;
}

extern "C" codeINSTALL_EXIT Install_Exit(HWND hwndParent, LPCTSTR pszInstallDir, WORD cFailedDirs, WORD cFailedFiles,
                                         WORD cFailedRegKeys, WORD cFailedRegVals, WORD cFailedShortcuts)
{
    return codeINSTALL_EXIT_DONE;
}

extern "C" codeUNINSTALL_INIT Uninstall_Init(HWND hwndParent, LPCTSTR pszInstallDir)
{
    return codeUNINSTALL_INIT_CONTINUE;
}

extern "C" codeUNINSTALL_EXIT Uninstall_Exit(HWND hwndParent)
{
    return codeUNINSTALL_EXIT_DONE;
}

void DeleteValues(HKEY hk)
{
	DWORD dwIndex = 0;
	WCHAR szValueName[256];
	DWORD cbValueName = sizeof(szValueName) / sizeof(WCHAR);
	DWORD dwType = 0;

	if (!hk)
		return;

	while(ERROR_SUCCESS==RegEnumValue(hk, 0, szValueName, &cbValueName, NULL, &dwType, NULL, NULL))
	{
		LONG lRet = RegDeleteValue(hk, szValueName);
		cbValueName = sizeof(szValueName) / sizeof(WCHAR);
	}
}