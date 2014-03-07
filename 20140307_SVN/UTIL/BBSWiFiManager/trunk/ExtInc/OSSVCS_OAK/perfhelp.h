//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/*****************************************************************************

    File: perfhelp.h
    
    Function that maximizes the size of text within a bounding rect.

    History:
        Created - KKennedy 7/13/00

*/
#ifndef _PERFHELP_H_
#define _PERFHELP_H_

/*
To add logging of a function simply add the PERFHELP_FUNCTION to the top with the complete function name.

    int Function()
    {
        PERFHELP_FUNCTION(Function);
        other locals


        return(9);
    }


To dump out the stats use the PERFHELP_DUMPSTATUS.

Typically, you will reset the stats, do the operation you are testing, and then
dump the stats.

*/

/*
    To enable perfhelp
            set ENABLE_PERFHELP=1
            do a full build
        OR
            Make sure you are in a SHIP or RETAIL build.
            set ENABLE_PERFHELP=1
            cd /d %_WINCEROOT%\private\ossvcs\services\misc
            build -c
            cd /d %_WINCEROOT%\private\ossvcs\services\dll
            build -c
            build -c in whatever directories you want to do your measuring in
            Run the build and watch the debug spew...
        
*/



#ifdef ENABLE_PERFHELP

struct PHFunctionSlot
{
    LONGLONG liTotal;
    UINT cCalls;
    TCHAR szName[100];
};

PHFunctionSlot* PerfHelpAssignSlot(const TCHAR* pszFunctionName);

void PerfHelpDeltaStart(int iDelta, const TCHAR* pszLabel);
void PerfHelpDeltaEnd(int iDelta);


void PerfHelpDumpStats();
void PerfHelpResetStats();

class PHTimer
{
    LONGLONG m_liStart;
    PHFunctionSlot* m_pSlot;
public:
    PHTimer(PHFunctionSlot* pSlot)
        {
        m_pSlot = pSlot;
        QueryPerformanceCounter((LARGE_INTEGER*)&m_liStart);
        }
    ~PHTimer()
        {
        LONGLONG liEnd;

        QueryPerformanceCounter((LARGE_INTEGER*)&liEnd);
        m_pSlot->liTotal += liEnd - m_liStart;
        m_pSlot->cCalls++;
        }
};


#define PERFHELP_FUNCTION(fn) \
    static PHFunctionSlot* s_ph_pfs; \
    if(!s_ph_pfs) s_ph_pfs = PerfHelpAssignSlot(TEXT( #fn )); \
    PHTimer pht___PerfHelp(s_ph_pfs);

#define PERFHELP_DELTA_START(i, label) PerfHelpDeltaStart(i, label);
#define PERFHELP_DELTA_END(i) PerfHelpDeltaEnd(i);

#define PERFHELP_DUMPSTATUS PerfHelpDumpStats();
#define PERFHELP_RESETSTATUS PerfHelpResetStats();


#else // ! ENABLE_PERFHELP

#define PERFHELP_FUNCTION(fn)
#define PERFHELP_DELTA_START(i, label)
#define PERFHELP_DELTA_END(i)
#define PERFHELP_RESETSTATUS
#define PERFHELP_DUMPSTATUS

#endif // ENABLE_PERFHELP



#ifdef PERFHELP_USECELOG
#include "celog.h"

#define LOGCEDATA(szBuf) CeLogData(TRUE, CELID_RAW_WCHAR, (PVOID) szBuf, (WORD)((_tcslen(szBuf)+1) * sizeof(TCHAR)), 0, CELZONE_ALWAYSON, 0, FALSE);
#define CELOGHELP_PERFMARKERDELTA(prefix, str) \
    { \
        TCHAR szBuf[128]; \
        StringCchPrintf(szBuf, ARRAYSIZE(szBuf), _T("%s:%s"), prefix, str); \
        LOGCEDATA(szBuf); \
    }
#define CELOGHELP_PERFMARKER(str) CELOGHELP_PERFMARKERDELTA(_T(""), str)

// Similar to PHTimer and PERFHELP_FUNCTION, but a little simpler
class PHLogger
{
private:
    const TCHAR *m_pszMarkerName;
public:
    PHLogger(const TCHAR *pszMarkerName)
    {
        m_pszMarkerName = pszMarkerName;
        CELOGHELP_PERFMARKERDELTA(_T("FunctionBegin"), m_pszMarkerName);
    }
    ~PHLogger()
    {
        CELOGHELP_PERFMARKERDELTA(_T("FunctionEnd"), m_pszMarkerName);
    }
};
#define CELOGHELP_FUNCTION(fn) PHLogger AutoMarker(_T(#fn));

#else // PERFHELP_USECELOG
// Noop these guys
#define CELOGHELP_PERFMARKERDELTA(prefix, str)
#define CELOGHELP_PERFMARKER(str)
#define CELOGHELP_FUNCTION(fn)
#endif // PERFHELP_USECELOG



#endif // _PERFHELP_H_

