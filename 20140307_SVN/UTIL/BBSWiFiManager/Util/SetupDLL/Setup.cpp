// Setup.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ce_setup.h"

#include <string>




extern "C" BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}

extern "C" codeINSTALL_INIT Install_Init(HWND hwndParent, BOOL fFirstCall, BOOL fPreviouslyInstalled, LPCTSTR pszInstallDir)
{
	HKEY hKey = NULL;

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
	// Delete Prev DLL
	DeleteFile(_T("Windows\\netui.dll_Back"));
	MoveFile(_T("Windows\\netui.dll"), _T("Windows\\netui.dll_Back"));

    return codeUNINSTALL_EXIT_DONE;
}
