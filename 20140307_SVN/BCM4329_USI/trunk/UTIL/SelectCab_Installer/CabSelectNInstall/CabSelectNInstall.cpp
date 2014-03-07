// CabSelectNInstall.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "CabSelectNInstall.h"
#include <windows.h>
#include <commctrl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

using namespace std;

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])
#endif




#define REG_PRODUCTNAME_PATH	L"System\\Bluebird"
#define REG_PRODUCTNAME_VALUE	L"ProductName"

#define HM_SEPARATOR_TEXT		L"HM"
#define BIP_SEPARATOR_TEXT		L"BIP"
#define BM_SEPARATOR_TEXT		L"BM"


// Function to wait for the cab loader to finish.  This is necessary
// because if we try to unpack a cab while the loader is running 
// that second unpack attempt will silently fail to unload.
// No inputs or outputs needed.
void WaitOnLoader ()
{
    const TCHAR c_szWceLoadClass[] = TEXT("MSWCELOAD");  // Loader class name
    const int c_iLoadStartupWait   = 250;                // 250 ms pause
    const int c_iMaxInstallWait    = 600000;             // 10 minutes

    TCHAR  szClass[MAX_PATH];
    HWND   hwnd;
    DWORD  dwProcessID = 0;
    HANDLE hProcess = NULL;
    
    // It's possible that the caller of this function is executing fast
    // enough that this check will be executed before a previously invoked
    // wceload actually starts up, so pause a bit to be sure wceload has 
    // a chance to get going.
    Sleep(c_iLoadStartupWait);

    hwnd = GetWindow(GetDesktopWindow(), GW_HWNDLAST);

    do
    {
        // Look for the loader and if it is running wait for it to finish 
        GetClassName(hwnd, szClass, ARRAYSIZE(szClass));
        if (!_tcscmp(szClass, c_szWceLoadClass))
        {
            GetWindowThreadProcessId(hwnd, &dwProcessID);
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
            WaitForSingleObject(hProcess, c_iMaxInstallWait);
            CloseHandle(hProcess);
        }

		hwnd = GetWindow(hwnd, GW_HWNDPREV);
	} while (hwnd);
}



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 응용 프로그램의 동작은 여기에서 코딩합니다.


		// Wait for wceload to finish.  This code is called by Install_Exit 
		// in the MCSetup dll, so wceload is expected to be shutting down and 
		// shouldn't take long to do so.
		WaitOnLoader();

		WIN32_FIND_DATA wfd = {0,};
		HANDLE  hFind = NULL;
		HKEY hKey = NULL;
		CString strCurProduct = L"";
		BOOL bFindInstallCabFile = FALSE;
		TCHAR tszInstallDir[256] = {0,};


		wcscpy(tszInstallDir, argv[1]);


		// 플랫폼 확인(HM 시리즈인지, BIP&BM 시리즈인지)
		if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PRODUCTNAME_PATH, 0, 0, &hKey) == ERROR_SUCCESS )
		{
			DWORD dwType = REG_SZ;
			TCHAR tszPDName[128] = {0,};
			DWORD dwDataSize = sizeof(tszPDName);

			if( RegQueryValueEx(hKey, REG_PRODUCTNAME_VALUE, NULL, &dwType, (LPBYTE)tszPDName, &dwDataSize) == ERROR_SUCCESS )
			{
				if( wcsstr(tszPDName, HM_SEPARATOR_TEXT) != NULL )
				{
					// HM
					strCurProduct = L"_HM";
				}
				else if( (wcsstr(tszPDName, BIP_SEPARATOR_TEXT) != NULL) || (wcsstr(tszPDName, BM_SEPARATOR_TEXT) != NULL) )
				{
					// Naos
					strCurProduct = L"_NAOS";
				}
				else
				{
					RegCloseKey(hKey);
					goto UNKNOWN_PLATFORM;
				}
			}
			else
			{
				RegCloseKey(hKey);
				goto UNKNOWN_PLATFORM;
			}

			RegCloseKey(hKey);
		}
		else
		{
			goto UNKNOWN_PLATFORM;
		}


		TCHAR tszFindCabFile[256] = {0,};

		wsprintf(tszFindCabFile, L"%s\\*.cab", tszInstallDir);


		hFind = FindFirstFile(tszFindCabFile, &wfd);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			CString strFileName = L"";
			int nCompareResult = 0;
			TCHAR tszFullPathOfCabFile[256] = {0,};

			do
			{
				strFileName = wfd.cFileName;
				strFileName.MakeUpper();

				nCompareResult = strFileName.Find(strCurProduct);

				wsprintf(tszFullPathOfCabFile, L"%s\\%s",tszInstallDir, wfd.cFileName);

				if( nCompareResult == -1 )
				{
					// 다른 플랫폼 Cab 설치 파일은 제거
					DeleteFile(tszFullPathOfCabFile);
				}
				else
				{
					bFindInstallCabFile = TRUE;

					SHELLEXECUTEINFO sei = {0};
					TCHAR tszPara[256] = {0,};

					wsprintf(tszPara, L"/noui \"%s\"", tszFullPathOfCabFile);

					sei.cbSize = sizeof(sei);
					sei.nShow = SW_SHOWNORMAL;
					sei.lpFile = L"\\Windows\\wceload.exe";
					sei.lpParameters = tszPara;
					if( ShellExecuteEx(&sei) == FALSE )
					{
						MessageBox(GetForegroundWindow(), L"Can't Excuted Cab File", L"Warning", MB_OK);
					}
					WaitOnLoader();
				}

			}while( FindNextFile(hFind, &wfd) );

			FindClose(hFind);
		}

		if( bFindInstallCabFile == FALSE )
		{
			MessageBox(GetForegroundWindow(), L"Can't Find Install File", L"Warning", MB_OK);
			nRetCode = 1;
		}


	}

	RETAILMSG(1, (TEXT("[SelectCab_Installer] CabSelectNInstall Return(%d)  \r\n"), nRetCode ));

	return nRetCode;

UNKNOWN_PLATFORM:
	MessageBox(GetForegroundWindow(), L"Unknown Platform", L"Warning", MB_OK);
	return 1;
}
