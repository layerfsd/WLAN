// SetupDLL.cpp : DLL 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>

#include "SetupDLL.h"

#define INSTALLER_EXE_NAME		L"CabSelectNInstall.exe"



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}




#ifndef ARRAYSIZE
#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])
#endif

codeINSTALL_INIT Install_Init(
    HWND        hwndParent,
    BOOL        fFirstCall,
    BOOL        fPreviouslyInstalled,
    LPCTSTR     pszInstallDir
)
{
    return codeINSTALL_INIT_CONTINUE;
}

codeINSTALL_EXIT Install_Exit(
    HWND    hwndParent,
    LPCTSTR pszInstallDir,
    WORD    cFailedDirs,
    WORD    cFailedFiles,
    WORD    cFailedRegKeys,
    WORD    cFailedRegVals,
    WORD    cFailedShortcuts
)
{

	WIN32_FIND_DATA wfd = {0,};
	HANDLE  hFind = NULL;
	TCHAR tszExeFullPath[256] = {0,};
	BOOL bFindExeFile = FALSE;

	wsprintf(tszExeFullPath, L"%s\\%s", pszInstallDir, INSTALLER_EXE_NAME);

	hFind = FindFirstFile(tszExeFullPath, &wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if( wcscmp(wfd.cFileName, INSTALLER_EXE_NAME) == 0 )
			{
				bFindExeFile = TRUE;
				TCHAR tszInstallDir[256] = {0,};

				SHELLEXECUTEINFO sei = {0};
				sei.cbSize = sizeof(sei);
				sei.nShow = SW_SHOWNORMAL;
				sei.lpFile = tszExeFullPath;

				wsprintf(tszInstallDir, L"\"%s\"", pszInstallDir);
				sei.lpParameters = tszInstallDir;
				ShellExecuteEx(&sei);

				RETAILMSG(1, (TEXT("[SelectCab_Installer] SetupDLL Wait For end of process\r\n") ));

				WaitForSingleObject(sei.hProcess, 30000); //30초 동안 실행된 프로세스 종료를 기다려준다.
				
				RETAILMSG(1, (TEXT("[SelectCab_Installer] end of process\r\n") ));
				break;
			}
		}while( FindNextFile(hFind, &wfd) );

		FindClose(hFind);
	}

	if( bFindExeFile == FALSE )
	{
		MessageBox(GetForegroundWindow(), L"Can't Find Install File", L"Warning", MB_OK);
	}


	return codeINSTALL_EXIT_DONE;
}


codeUNINSTALL_INIT Uninstall_Init(
    HWND        hwndParent,
    LPCTSTR     pszInstallDir
)
{
    return codeUNINSTALL_INIT_CONTINUE;
}

codeUNINSTALL_EXIT Uninstall_Exit(
    HWND    hwndParent
)
{
    return codeUNINSTALL_EXIT_DONE;
}
