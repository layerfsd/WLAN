//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
// Copyright (c) Microsoft Corporation.  All rights reserved. 
//
// Use of this source code is subject to the terms of your Windows Mobile 
// Software Shared Source Premium Derivatives License Form or other 
// applicable Microsoft license agreement for the software. If you did not 
// accept the terms of such a license, you are not authorized to use this 
// source code.
//***   tshres.h -- Tpc SHell RESource-only stuff
// USAGE
//  see server for an e.g.
//  1. do InitHINST(hinst) in your PROCESS_ATTACH.
//  2. make sure all of your resource references (e.g. LoadString) use
//  HINST_RESDLL
//  3. Define a TSHRES_BASE for your component and renumber your resources.
//  You should give each component a range of at least 1000 values.
//  NOTE that only strings can have values off of TSHRES_BASE, menus and icons
//  cannot be defined as TSHRES_BASE+xxx, so you will have to make sure that
//  the constant values you assign these resources fall in the correct range
//  4. add your shres.rc into tshres/0409/shres.rc
//  5. link w/ tshres.lib
//  6. as a final sanity check, do "dumpbin release/mydll.dll" and make
//  sure there's no .rsrc section; and do "dumpbin release/tshres.dll"
//  and make sure it has grown from your additions.

EXTERN_C HINSTANCE g_hInst;       // code
EXTERN_C HINSTANCE g_hInstRes;    // resources
EXTERN_C HINSTANCE g_hInstResDPI ; // DPI-sensitive resources

#include "tshbases.h"

#define TSHRES_DLL TEXT("tshres.dll")

// Common resource strings. Use these instead of defining your own
#define IDS_DONE                (TSHRES_BASE_COMMON + 1)
#define IDS_CANCEL              (TSHRES_BASE_COMMON + 2)
#define IDS_YES                 (TSHRES_BASE_COMMON + 3)
#define IDS_NO                  (TSHRES_BASE_COMMON + 4)
#define IDS_OK                  (TSHRES_BASE_COMMON + 5)
#define IDS_INFO_HEADING        (TSHRES_BASE_COMMON + 6)
#define IDS_EXCLAMATION_HEADING (TSHRES_BASE_COMMON + 7)
#define IDS_ERROR_HEADING       (TSHRES_BASE_COMMON + 8)
#define IDS_QUESTION_HEADING    (TSHRES_BASE_COMMON + 9)
#define IDS_NONE                (TSHRES_BASE_COMMON + 10)
//#define IDS_SELECT_ITEMS      (TSHRES_BASE_COMMON + 11): see "Expanded picker resources"
#define IDS_SAVE                (TSHRES_BASE_COMMON + 12)
#define IDS_ON                  (TSHRES_BASE_COMMON + 13)
#define IDS_OFF                 (TSHRES_BASE_COMMON + 14)

// Ringtone folder. This resource string is needed by code in both aygshell and ossvcs. 
#define IDS_DIR_MYRINGTONES     (TSHRES_BASE_COMMON + 15)

// Common Command IDs. Use these instead of defining your own
#define IDM_DONE                (TSHRES_BASE_COMMON + 0)
#define IDM_CANCEL              (TSHRES_BASE_COMMON + 1)

// Common Control IDs. Use these insead of defining your own
#define IDC_EDIT                (TSHRES_BASE_COMMON + 100)

// Expanded picker resources
#define IDC_LIST                    (TSHRES_BASE_COMMON + 10)
#define IDR_MENUBAR_EXPANDED_PICKER 509
#define IDR_MENUBAR_EXPANDED_EDIT   510
#define IDS_SELECT_ITEMS            (TSHRES_BASE_COMMON + 11)


#define IDD_EXPANDED_LIST               512
#define IDD_EXPANDED_MULTISEL_LIST      513
#define IDD_EXPANDED_EDIT               514

#define HINST_THISDLL   g_hInst

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TSHRES_DELAY_LOAD
    HINSTANCE GetResourceLibrary();
    HINSTANCE GetResourceLibraryDPI();
#endif

HMODULE WINAPI TshResGetInstance();

#ifdef __cplusplus
}
#endif

/***********************************
/* TSHRES_DELAY_LOAD
/***********************************

//copy the following functions to your module to allow for
//tshres to be delay loaded 

HINSTANCE GetResourceLibrary()
{
    if(NULL == g_hInstRes)
    {
        //let's load the resource lib
        InitHINST(g_hInst);
    }

    return g_hInstRes;
}

HINSTANCE GetResourceLibraryDPI()
{
    if(NULL == g_hInstResDPI)
    {
        //let's load the resource lib
        g_hInstResDPI = DPI_LoadLibraryRes(_T("tshres.dll"));
    }

    return g_hInstResDPI;
}

/**********************************/


#ifndef TSHRES_DELAY_LOAD
    #define HINST_RESDLL    g_hInstRes
#else 
        //want to delay load the resources.
        //GetResourceLibrary must be implemented
    #define HINST_RESDLL    GetResourceLibrary()
#endif

#ifndef TSHRES_DELAY_LOAD
    #define HINST_RESDLLDPI    g_hInstResDPI
#else 
        //want to delay load the resources.
        //GetResourceLibrary must be implemented
    #define HINST_RESDLLDPI    GetResourceLibraryDPI()
#endif

#if 0 // now in ossvcs!miscsvcs.h
#if 1 // FEAT_DATALESS
#define LoadLibraryRes(p)   LoadLibraryEx((p), NULL, LOAD_LIBRARY_AS_DATAFILE)
#else
#define LoadLibraryRes(p)   LoadLibrary((p))
#endif
#endif

#define InitHINST(hinstCode) \
    { \
        g_hInst = (hinstCode); \
        g_hInstRes = LoadLibraryRes(TEXT("netui.dll")); /* aka LoadLibraryEx */ \
        /* NULL is o.k. */ \
        /*if (!g_hInstRes)*/ \
            /*g_hInstRes = (hinstCode);*/ \
    } \

