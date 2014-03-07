#include "stdafx.h"
#include "Global.h"




// ********************************************************************
// 전역변수

// QVGA 기준의 화면 비율을 확인할 수 있는 변수
// 1 : VGA
// 2 : QVGA
DWORD    g_dwScreenRate		= 1;


// 관리자 모드인지 확인하기 위한 변수
DWORD   g_dwAdminMode		= PidSup_AdminMode_User;


// Tab title을 나타낸다.
LPWSTR  PIDSUP_TABTITLE[PIDSUP_FULL_TABCNT]	= \
{	\
	L"Profile",	\
	L"Status",	\
	L"Ping",	\
	L"Roaming",	\
	L"Setting"	\
};


// 현재 실행파일의 파일 버전 얻어오기
BOOL GetCurrentFileVersion(LPTSTR tszFileVer)
{
	BOOL bRet = FALSE;
	DWORD dwInfoSize = 0;
	TCHAR tszFilePath[MAX_PATH] = {'\0',};

	char *pBuf = NULL;

	tszFileVer[0] = '\0';

	GetModuleFileName(NULL, tszFilePath, MAX_PATH);
	
	dwInfoSize = GetFileVersionInfoSize(tszFilePath, 0);
	
	pBuf = new char[dwInfoSize];
	
	if( dwInfoSize == 0 )
	{
		bRet = FALSE;
	}

	if(GetFileVersionInfo(tszFilePath, 0, dwInfoSize, pBuf) !=0 )
	{
		VS_FIXEDFILEINFO* pFineInfo = NULL;
		UINT bufLen = 0;
		if(VerQueryValue(pBuf, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
		{   
			bRet = TRUE;
			wsprintf(tszFileVer, _T("v%d.%d.%d.%d"), HIWORD(pFineInfo->dwFileVersionMS), LOWORD(pFineInfo->dwFileVersionMS), HIWORD(pFineInfo->dwFileVersionLS), LOWORD(pFineInfo->dwFileVersionLS) );
		}
	}
	
	delete pBuf;


	return bRet;
}
