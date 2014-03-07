#include "NetuiDebugLog.h"


void NetuiDbgMsg(BOOL bTimeStamp, LPVOID strFormat, ...)
{
	TCHAR   tszMsgBuff[1024] = {'\0',};
	CHAR    szMsgBuff[1024] = {'\0',};
	CHAR    szTime[32] = {'\0',};
	DWORD	dwStrLen = 0;
	DWORD	dwBytesWritten = 0;
	HANDLE	hLogFile = NULL;    

	va_list lpStart;
	va_start(lpStart, strFormat);
	vsprintf(szMsgBuff, (LPSTR)strFormat, lpStart);
	va_end(lpStart);

	dwStrLen = strlen(szMsgBuff);

	if(dwStrLen < 2)
	{
		va_list lpStart;
		va_start(lpStart, strFormat);
		vswprintf(tszMsgBuff, (LPTSTR)strFormat, lpStart);
		va_end(lpStart);  

		WideCharToMultiByte( CP_ACP, 0, tszMsgBuff, _tcslen(tszMsgBuff), szMsgBuff, sizeof(szMsgBuff), &szMsgBuff[0], NULL);
	}


	hLogFile = CreateFile(_T("\\NetuiLog.txt"), GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	SetFilePointer(hLogFile , 0, NULL, FILE_END);

	if(bTimeStamp)
	{
		SYSTEMTIME sysTime = {0, };
		// 현재 시간 기록
		GetLocalTime(&sysTime);
		sprintf(szTime, "[%02d/%02d, %02d:%02d:%02d] ", sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		WriteFile(hLogFile, szTime, strlen(szTime), &dwBytesWritten, 0);
	}

	// 파일 로그 기록
	WriteFile(hLogFile, szMsgBuff, strlen(szMsgBuff), &dwBytesWritten, 0);

	CloseHandle(hLogFile);

}
