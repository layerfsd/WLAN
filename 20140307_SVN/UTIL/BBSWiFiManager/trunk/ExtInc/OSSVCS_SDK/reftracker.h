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
/*++

Copyright (c) Microsoft Corporation

Module Name:

    reftracker.h

Abstract:

    Tracks references on objects using LMEMDEBUG

--*/

#pragma once

#include <windows.h>
#include <lmemdebug.h>

extern PFN_LMEMAddTrackedItem      g_pfnAddTrackedItem;
extern PFN_LMEMRemoveTrackedItem   g_pfnRemoveTrackedItem;
extern PFN_LMEMDumpTrackedItem     g_pfnDumpTrackedItem;
extern HINSTANCE                   g_hLibInst;

#define USE_REFTRACKER  \
       PFN_LMEMAddTrackedItem      g_pfnAddTrackedItem;     \
       PFN_LMEMRemoveTrackedItem   g_pfnRemoveTrackedItem;  \
       PFN_LMEMDumpTrackedItem     g_pfnDumpTrackedItem;  \
       HINSTANCE                   g_hLibInst;

#define INIT_REFTRACKER() \
    if (NULL != (g_hLibInst = LoadLibraryW(LMEMDEBUG_DLL))) { \
        g_pfnAddTrackedItem = (PFN_LMEMAddTrackedItem)GetProcAddressW(g_hLibInst, L"LMEMAddTrackedItem"); \
        g_pfnRemoveTrackedItem = (PFN_LMEMRemoveTrackedItem)GetProcAddressW(g_hLibInst, L"LMEMRemoveTrackedItem"); \
        g_pfnDumpTrackedItem = (PFN_LMEMDumpTrackedItem)GetProcAddressW(g_hLibInst, L"LMEMDumpTrackedItem"); \
        if ( !g_pfnAddTrackedItem || !g_pfnRemoveTrackedItem || !g_pfnDumpTrackedItem ) \
        { \
            FreeLibrary(g_hLibInst); \
            g_hLibInst = NULL; \
            g_pfnAddTrackedItem = NULL ;\
            g_pfnRemoveTrackedItem = NULL ;\
            g_pfnDumpTrackedItem = NULL; \
        } \
    }

#define DEINIT_REFTRACKER() \
    if ( g_hLibInst ) \
    { \
        FreeLibrary(g_hLibInst); \
        g_hLibInst = NULL; \
        g_pfnAddTrackedItem = NULL ;\
        g_pfnRemoveTrackedItem = NULL ;\
        g_pfnDumpTrackedItem = NULL; \
    } \



#define REFTRACKER_INCREF(obj) \
    if ( g_pfnAddTrackedItem ) g_pfnAddTrackedItem(NULL, (obj), LMEM_ALLOC_SIZE_INCREF, __LINE__, __FILE__);

#define REFTRACKER_DECREF(obj, count) \
    if ( (count)<0 )    DebugBreak(); \
    else if ( ((count)==0) && g_pfnRemoveTrackedItem ) g_pfnRemoveTrackedItem(NULL, (obj)); \
    else if ( g_pfnAddTrackedItem ) g_pfnAddTrackedItem(NULL, (obj), LMEM_ALLOC_SIZE_DECREF, __LINE__, __FILE__);

#define REFTRACKER_DUMPINFO(obj) \
    if ( g_pfnDumpTrackedItem ) g_pfnDumpTrackedItem(NULL, (obj));
