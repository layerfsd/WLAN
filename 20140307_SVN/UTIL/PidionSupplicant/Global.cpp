#include "stdafx.h"
#include "Global.h"




// ********************************************************************
// ��������

// QVGA ������ ȭ�� ������ Ȯ���� �� �ִ� ����
// 1 : VGA
// 2 : QVGA
DWORD    g_dwScreenRate		= 1;


// ������ ������� Ȯ���ϱ� ���� ����
DWORD   g_dwAdminMode		= PidSup_AdminMode_User;


// Tab title�� ��Ÿ����.
LPWSTR  PIDSUP_TABTITLE[PIDSUP_FULL_TABCNT]	= \
{	\
	L"Profile",	\
	L"Status",	\
	L"Ping",	\
	L"Roaming",	\
	L"Setting"	\
};


// ���� ���������� ���� ���� ������
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
