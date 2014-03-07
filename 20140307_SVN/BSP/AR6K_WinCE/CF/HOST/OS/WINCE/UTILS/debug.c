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
//    The software is licensed “as-is.” 
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
#include "athtypes.h"

boolean printMask;
DBGPARAM dpCurSettings =
{
    TEXT("AR6K_WLAN_DRIVER"), // module name
{
    TEXT("Error"), 
    TEXT("Warning"), 
    TEXT("Info"),
    TEXT("Trace"),
    TEXT("Synchronization"), 
    TEXT("Transmit"), 
    TEXT("Receive"), 
    TEXT("HTC Buffer Dump"),
    TEXT("BMI Module"),
    TEXT("WMI Module"),
    TEXT("HIF Module"), 
    TEXT("HTC Module"), 
    TEXT("WLAN Module"),
    TEXT("NDIS Module"),                  
    TEXT(""),
    TEXT(""),
},
    0x00000001,
};

void
DbgPrintf(A_INT32 mask, A_CHAR * format, ...)
{
    va_list argList;    /* holds the arguement list passed in */
    A_CHAR    buffer[1024];
    wchar_t wBuf[1024];

	if (mask && printMask) {
		/* get the argument list */
		va_start(argList, format);

		/*
		 * using vprintf to perform the printing it is the same is printf, only
		 * it takes a va_list or arguements
		 */
		vsprintf(buffer, format, argList);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, wBuf, 256 );
		OutputDebugString(wBuf);
	}
	return;
}
