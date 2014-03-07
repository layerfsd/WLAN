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
// ***************************************************************************
// PERFLIB.H
//
// Public macros for MDDPERF lib
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//

#pragma once

#ifndef _PERFLIB_H_
#define _PERFLIB_H_

// function pointer prototypes
typedef HRESULT (*PerfRegisterPtr)  (TCHAR *);
typedef HRESULT (*PerfDeRegisterPtr)();
typedef HRESULT (*PerfBeginPtr)     (TCHAR *, DWORD, PHANDLE);
typedef HRESULT (*PerfEndPtr)       (HANDLE);
typedef HRESULT (*PerfIncrementPtr) (HANDLE, DWORD);
typedef HRESULT (*PerfEnablePtr)    (TCHAR *);
typedef HRESULT (*PerfDisablePtr)   (TCHAR *);
typedef HRESULT (*PerfLogDataPtr)   (TCHAR *, TCHAR *);

// function pointer definitions
#ifdef MDDPERF_DECLARE_GLOBALS
BOOL                g_bMDDPerfEnabled   = FALSE;
HINSTANCE           g_hMDDPerfModule    = NULL;
PerfRegisterPtr     g_pfnPerfRegister   = NULL;
PerfDeRegisterPtr   g_pfnPerfDeRegister = NULL;
PerfBeginPtr        g_pfnPerfBegin      = NULL;
PerfEndPtr          g_pfnPerfEnd        = NULL;
PerfIncrementPtr    g_pfnPerfIncrement  = NULL;
PerfEnablePtr       g_pfnPerfEnable     = NULL;
PerfDisablePtr      g_pfnPerfDisable    = NULL;
PerfLogDataPtr      g_pfnPerfLogData    = NULL;
#else
extern BOOL                 g_bMDDPerfEnabled;
extern HINSTANCE            g_hMDDPerfModule;
extern PerfRegisterPtr      g_pfnPerfRegister;
extern PerfDeRegisterPtr    g_pfnPerfDeRegister;
extern PerfBeginPtr         g_pfnPerfBegin;
extern PerfEndPtr           g_pfnPerfEnd;
extern PerfIncrementPtr     g_pfnPerfIncrement;
extern PerfEnablePtr        g_pfnPerfEnable;
extern PerfDisablePtr       g_pfnPerfDisable;
extern PerfLogDataPtr       g_pfnPerfLogData;
#endif

// global constants
const TCHAR gc_tszPerfModuleName[]  = _T("MDDPERF.dll");
const TCHAR gc_tszRegisterProc[]    = _T("PerfRegister");
const TCHAR gc_tszDeRegisterProc[]  = _T("PerfDeRegister");
const TCHAR gc_tszBeginProc[]       = _T("PerfBegin");
const TCHAR gc_tszEndProc[]         = _T("PerfEnd");
const TCHAR gc_tszIncrementProc[]   = _T("PerfIncrement");
const TCHAR gc_tszEnableProc[]      = _T("PerfEnable");
const TCHAR gc_tszDisableProc[]     = _T("PerfDisable");
const TCHAR gc_tszLogDataProc[]     = _T("PerfLogData");

#define MDDPERF_INIT(AppName)                                                                               \
    if (!g_bMDDPerfEnabled)                                                                                 \
    {                                                                                                       \
        g_hMDDPerfModule = LoadLibrary(gc_tszPerfModuleName);                                               \
        if (g_hMDDPerfModule != NULL) {                                                                     \
            g_pfnPerfRegister = (PerfRegisterPtr)GetProcAddress(g_hMDDPerfModule, gc_tszRegisterProc);      \
            g_pfnPerfDeRegister = (PerfDeRegisterPtr)GetProcAddress(g_hMDDPerfModule, gc_tszDeRegisterProc);\
            g_pfnPerfBegin = (PerfBeginPtr)GetProcAddress(g_hMDDPerfModule, gc_tszBeginProc);               \
            g_pfnPerfEnd = (PerfEndPtr)GetProcAddress(g_hMDDPerfModule, gc_tszEndProc);                     \
            g_pfnPerfIncrement = (PerfIncrementPtr)GetProcAddress(g_hMDDPerfModule, gc_tszIncrementProc);   \
            g_pfnPerfEnable = (PerfEnablePtr)GetProcAddress(g_hMDDPerfModule, gc_tszEnableProc);            \
            g_pfnPerfDisable = (PerfDisablePtr)GetProcAddress(g_hMDDPerfModule, gc_tszDisableProc);         \
            g_pfnPerfLogData = (PerfLogDataPtr)GetProcAddress(g_hMDDPerfModule, gc_tszLogDataProc);         \
            if (g_pfnPerfRegister != NULL) {                                                                \
                g_bMDDPerfEnabled = (g_pfnPerfRegister(AppName) == S_OK) ? TRUE : FALSE;                    \
            }                                                                                               \
        }                                                                                                   \
    }

#define MDDPERF_DEINIT()                                    \
    if (g_bMDDPerfEnabled && (g_pfnPerfDeRegister != NULL)) \
    {                                                       \
        g_pfnPerfDeRegister();                              \
        g_bMDDPerfEnabled = !FreeLibrary(g_hMDDPerfModule); \
    }
    
#define MDDPERFSHIP_BEGIN(PerfPath, PerfOptions, HandleSuffix)                  \
    HANDLE hPerfInstance ## HandleSuffix = INVALID_HANDLE_VALUE;                                       \
    if (g_bMDDPerfEnabled && (g_pfnPerfBegin != NULL))                          \
    {                                                                           \
        g_pfnPerfBegin(PerfPath, PerfOptions, &hPerfInstance ## HandleSuffix);  \
    }
    
#define MDDPERFSHIP_END(HandleSuffix)                   \
    if (g_bMDDPerfEnabled && (g_pfnPerfEnd != NULL))    \
    {                                                   \
        g_pfnPerfEnd(hPerfInstance ## HandleSuffix);    \
    }

#define MDDPERFSHIP_INCREMENT(HandleSuffix, IncrementValue)                 \
    if (g_bMDDPerfEnabled && (g_pfnPerfIncrement != NULL))                  \
    {                                                                       \
        g_pfnPerfIncrement(hPerfInstance ## HandleSuffix, IncrementValue);  \
    }

#define MDDPERFSHIP_ENABLE(PerfPath)                    \
    if (g_bMDDPerfEnabled && (g_pfnPerfEnable != NULL)) \
    {                                                   \
        g_pfnPerfEnable(PerfPath);                      \
    }
    
#define MDDPERFSHIP_DISABLE(PerfPath)                       \
    if (g_bMDDPerfEnabled && (g_pfnPerfDisable != NULL))    \
    {                                                       \
        g_pfnPerfDisable(PerfPath);                         \
    }

#define MDDPERFSHIP_LOGDATA(RegPath, LogFile)               \
    if (g_bMDDPerfEnabled && (g_pfnPerfLogData != NULL))    \
    {                                                       \
        g_pfnPerfLogData(RegPath, LogFile);                 \
    }

#define MDDPERFSHIP_START_SINGLE(PerfPath)          \
    {                                               \
    MDDPERFSHIP_BEGIN(PerfPath, 0, Single);    

#define MDDPERFSHIP_STOP_SINGLE()   \
    MDDPERFSHIP_END(Single);        \
    }

// We have the following conflict of interest:
// We want most performance counters to disappear in WINCESHIP builds, but
// the build lab only produces WINCESHIP builds (and it really only makes
// sense to test performance on WINCESHIP builds).  So, for now we will encourage
// the use of the non-shipping macros, and when it comes time to ship the
// product, all we have to do is swap the "#if 0" with the "#ifdef SHIP_BUILD"

// #ifdef SHIP_BUILD
#if 0
#define MDDPERF_BEGIN(PerfPath, PerfOptions, HandleSuffix)
#define MDDPERF_END(HandleSuffix)
#define MDDPERF_INCREMENT(HandleSuffix, IncrementValue)
#define MDDPERF_ENABLE(PerfPath)
#define MDDPERF_DISABLE(PerfPath)
#define MDDPERF_LOGDATA(RegPath, LogFile)
#define MDDPERF_START_SINGLE(PerfPath)
#define MDDPERF_STOP_SINGLE()
#else
#define MDDPERF_BEGIN(PerfPath, PerfOptions, HandleSuffix)  \
    MDDPERFSHIP_BEGIN(PerfPath, PerfOptions, HandleSuffix)
#define MDDPERF_END(HandleSuffix)   \
    MDDPERFSHIP_END(HandleSuffix)
#define MDDPERF_INCREMENT(HandleSuffix, IncrementValue) \
    MDDPERFSHIP_INCREMENT(HandleSuffix, IncrementValue)
#define MDDPERF_ENABLE(PerfPath) \
    MDDPERFSHIP_ENABLE(PerfPath)
#define MDDPERF_DISABLE(PerfPath) \
    MDDPERFSHIP_DISABLE(PerfPath)
#define MDDPERF_LOGDATA(RegPath, LogFile) \
    MDDPERFSHIP_LOGDATA(RegPath, LogFile)
#define MDDPERF_START_SINGLE(PerfPath)  \
    MDDPERFSHIP_START_SINGLE(PerfPath)
#define MDDPERF_STOP_SINGLE()   \
    MDDPERFSHIP_STOP_SINGLE()
#endif // SHIP_BUILD

#endif // _PERFLIB_H_
