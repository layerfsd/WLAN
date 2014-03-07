//------------------------------------------------------------------------------
// <copyright file="debug.c" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// Debug zone defintions
//
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include <dbgapi.h>
#include "a_types.h"
#include "osapi_wince.h"

static A_UINT32 s_printMask 	= 1;	//default  flag

static A_UINT32	s_dbgLevel		= 1;	
static A_UINT32 s_dbgFileLog	= 0;


void SetPrintMask(A_UINT32 flag)
{
	s_printMask = s_printMask |  flag;
	RETAILMSG(0,(L"printMask = 0x%08x \r\n", s_printMask));
}

void SetDbgFileLog(A_UINT32 value)
{
	s_dbgFileLog = value;
	RETAILMSG(0,(L"s_dbgFileLog = 0x%08x \r\n", s_dbgFileLog));
}


HANDLE		hFile = NULL;

BOOL DbgWriteFile(LPCTSTR pszText)
{
	DWORD	dwWritten;

	SetFilePointer(hFile, 0, NULL, FILE_END);       /* append! */
	WriteFile(hFile, pszText, _tcslen(pszText) * sizeof(TCHAR), 
					 &dwWritten, NULL);
	RETAILMSG(0,(L" DbgWriteFile()-> dwWriten = %d \r\n", dwWritten));
	return 1;
}


void DebugInit(A_UINT32 level, LPCTSTR fileName)
{

	s_dbgLevel = level;
	
	hFile = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
	  // Your error-handling code goes here.
	 RETAILMSG(0,(L"Could not open ONE.TXT"));
	  return;
	}

	RETAILMSG(1,(L"[WLAN] s_dbgLevel = %d, s_dbgFileLog = %d \r\n",s_dbgLevel, s_dbgFileLog));

	if(s_dbgLevel == 2)	{
		SetPrintMask(DBG_LEVEL_CAR6K);
	}
	else if (s_dbgLevel == 3) {
		SetPrintMask(DBG_LEVEL_CAR6K);
		SetPrintMask(DBG_LEVEL_WMI_HTC);
	}
	else if (s_dbgLevel == 4) {
		SetPrintMask(DBG_LEVEL_CAR6K);
		SetPrintMask(DBG_LEVEL_WMI_HTC);
		SetPrintMask(DBG_LEVEL_HIF);
	}
	

}



void
DbgPrintf(A_INT32 mask, A_CHAR * format, ...)
{
    va_list argList;    /* holds the arguement list passed in */
    A_CHAR    buffer[1024];
    wchar_t wBuf[1024];
	//int sdt;
	SYSTEMTIME tSystemtime;

	if(s_dbgLevel == 0) return;


	GetLocalTime(&tSystemtime);
	
	if (mask & s_printMask) 
	{
		/* get the argument list */
		va_start(argList, format);

		/*
		 * using vprintf to perform the printing it is the same is printf, only
		 * it takes a va_list or arguements
		 */
	 	sprintf(buffer,"[WLAN %02d:%02d:%02d] ",tSystemtime.wHour,tSystemtime.wMinute,tSystemtime.wSecond);
		vsprintf(&buffer[16], format, argList);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, wBuf, 256 );

		if(s_dbgFileLog) {
			DbgWriteFile(wBuf);	
		}
		
		OutputDebugString(wBuf);
		
	}


	return;
}







