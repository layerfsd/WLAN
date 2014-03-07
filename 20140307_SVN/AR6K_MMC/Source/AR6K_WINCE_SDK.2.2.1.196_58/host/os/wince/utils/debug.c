//------------------------------------------------------------------------------
// <copyright file="debug.c" company="Atheros">
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
//
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include <dbgapi.h>
#include "osapi_wince.h"
#include "athtypes_wince.h"

#include "hif.h"
#include "htc_api.h"
#include "stream_drv.h"
#include "common_drv.h"
#include "bmi.h"

#include "wince_common_drv.h"
#include "a_debug.h"


static char dbglogpath[512];

ULONG printMask = 0;

DBGPARAM dpCurSettings =
{
    TEXT("AR6K_WLAN_DRIVER"), // module name
{
    TEXT("Error"),
    TEXT("Warning"),
    TEXT("Info"),
    TEXT("Trace"),
    TEXT("Synchronisation"),
    TEXT("Transmit"),
    TEXT("Receive"),
    TEXT("HTC Buffer Dump"),
    TEXT("BMI Module"),
    TEXT("WMI Module"),
    TEXT("HIF Module"),
    TEXT("HTC Module"),
    TEXT("WLAN Module"),
    TEXT("NDIS Module"),
#ifdef OS_ROAM_MANAGEMENT
    TEXT("OSRoam"),
#else
    TEXT(""),
#endif
},
#ifdef OS_ROAM_MANAGEMENT
    0x00007F01,
#else
    0x00003F01,
#endif
};



void
DbgPrintf(A_INT32 mask, A_CHAR * format, ...)
{
    va_list     argList;    /* holds the arguement list passed in */
    A_CHAR      buffer[1024];
    wchar_t     wBuf[1024];

    if (mask & printMask)
    {
        /* get the argument list */
        va_start (argList, format);

        /*
         * using vprintf to perform the printing it is the same is printf, only
         * it takes a va_list or arguements
         */
        if (S_OK != StringCchVPrintfA (buffer, sizeof (buffer), format, argList))
        {
            return;
        }
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, wBuf, 256 );
        OutputDebugString (wBuf);
    }

    return;
}

void
logInit(char* logpath)
{
    FILE        *fStream = NULL;

	strcpy( dbglogpath, logpath );
	RETAILMSG(1, (TEXT("builder:ar6k2:loginit: create log file=%S\r\n"), dbglogpath)); 
			
    //-builder fStream = fopen("\\Windows\\ar6khost.log", "w");
    fStream = fopen(dbglogpath, "w");

    if (fStream)
    {
        fclose(fStream);
    }
}

void
dbgIntialize (A_UINT32 dbgPrintMask)
{
#ifdef DEBUG
    printMask = dbgPrintMask;
#endif
}

void
logPrintf(A_INT32 mask, A_CHAR * format, ...)
{
    va_list     argList;    /* holds the arguement list passed in */
    A_CHAR      buffer[1024];

    FILE        *fStream = NULL;

	if( mask == 0 )
		return;


    /* get the argument list */
    va_start(argList, format);

    /*
     * using vprintf to perform the printing it is the same is printf, only
     * it takes a va_list or arguements
     */
    if (S_OK != StringCchVPrintfA (buffer, sizeof (buffer), format, argList))
    {
        return;
    }

    //-builder fStream = fopen("\\Windows\\ar6khost.log", "a+");
    fStream = fopen(dbglogpath, "a+");
    if (fStream)
    {
        fprintf(fStream,"%i: %s\n",GetTickCount(),buffer);
        fclose(fStream);
    }
    return;
}


#define MAX_DEBUG_STRING 325

CHAR    g_DebugBuffer[MAX_DEBUG_STRING];
WCHAR   g_WideDebugBuffer[MAX_DEBUG_STRING];
CRITICAL_SECTION g_DebugPrintLock;

void OutputToDebugger(CHAR *pDebugStr)
{
    PWCHAR pBuffer;
    int len =0;

    pBuffer = g_WideDebugBuffer;

    while (*pDebugStr != (CHAR)0) {
        *pBuffer = (WCHAR)*pDebugStr;
        pDebugStr++;

        if (len >= MAX_DEBUG_STRING)
        {
            break;
        }

        pBuffer++;
        len++;
    }

    *pBuffer = (WCHAR)0;
    OutputDebugString(g_WideDebugBuffer);
}

void
A_DbgPrintf(A_CHAR * pDbgStr, ...)
{
    va_list       argumentList;
    int           debugChars,ii;

    va_start(argumentList, pDbgStr);

    EnterCriticalSection(&g_DebugPrintLock);

    g_DebugBuffer[MAX_DEBUG_STRING - 1] = (CHAR)0;

    do {

        debugChars = _vsnprintf(g_DebugBuffer,(MAX_DEBUG_STRING - 1), pDbgStr, argumentList);

        if (debugChars < 0) {
            RETAILMSG(TRUE,
                   (TEXT(" Debug string TOO LONG!!! \r\n")));
            break;
        }

        if (debugChars > 2) {
            for (ii = (debugChars - 3); ii < debugChars; ii++) {
                    /* replace the last line feed with carriage return + line feed */
                if ('\n' == g_DebugBuffer[ii]) {
                    if (debugChars < (MAX_DEBUG_STRING - 3)) {
                        g_DebugBuffer[ii] = '\r';
                        g_DebugBuffer[ii+1] = '\n';
                        g_DebugBuffer[ii+2] = (CHAR)0;
                        break;
                    }
                }
            }
        }

        OutputToDebugger(g_DebugBuffer);

    } while (FALSE);

    LeaveCriticalSection(&g_DebugPrintLock);

    va_end(argumentList);
}

void DisplayMsg(WCHAR * format, ...)
{
    va_list     argList;    /* holds the arguement list passed in */
    WCHAR       wbuffer[1024];

    /* get the argument list */
    va_start(argList, format);

    /*
     * using vprintf to perform the printing it is the same is printf, only
     * it takes a va_list or arguements
     */
    if (S_OK != StringCchVPrintfW (wbuffer, sizeof (wbuffer)/sizeof (wbuffer[0]) , format, argList))
    {
        return;
    }

    RETAILMSG(TRUE, (wbuffer));

    return;
}






