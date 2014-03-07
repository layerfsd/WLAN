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
/*****************************************************************************/
/*                                                                           */
/*  TodayCmn.h                                                               */
/*                                                                           */
/*  Today screen common header file                                          */
/*                                                                           */
/*  Copyright (c) Microsoft Corporation. All rights reserved.                */
/*****************************************************************************/

#ifndef _TODAYCMN_H_
#define _TODAYCMN_H_

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* list item types */
typedef enum _TODAYLISTITEMTYPE
{
    tlitOwnerInfo = 0,  /* name, company */
    tlitAppointments,   /* today's appointments, events */
    tlitMail,           /* message information */
    tlitTasks,          /* overdue and upcoming tasks */
    tlitCustom,         /* other */
    tlitReserved1,      /* reserved value */
    tlitReserved2,      /* reserved value */
    tlitReserved3,      /* reserved value */
    tlitNil             /* sentinel */

} TODAYLISTITEMTYPE;


#define MAX_ITEMNAME    32

enum TODAY_SELECTABILITY
{
    TODAY_SELECTABILITY_NOTSELECTABLE = 0,
    TODAY_SELECTABILITY_SELECTABLE,
    TODAY_SELECTABILITY_CUSTOMSELECTION,
    TODAY_SELECTABILITY_LAST
};

/* information for a single today item */
typedef struct _TODAYLISTITEM
{
    TCHAR               szName[MAX_ITEMNAME];
    TODAYLISTITEMTYPE   tlit;
    DWORD               dwOrder;
    DWORD               cyp;
    BOOL                fEnabled;
    BOOL                fOptions;
    DWORD               grfFlags;
    TCHAR               szDLLPath[MAX_PATH];
    HINSTANCE           hinstDLL;
    HWND                hwndCustom;
    BOOL                fSizeOnDraw;
    BYTE                *prgbCachedData;
    DWORD               cbCachedData;
    DWORD               dwSelectability;
} TODAYLISTITEM;



#define TODAYM_GETCOLOR            (WM_USER + 100) // wParam == TODAYCOLOR_*
#define TODAYM_TOOKSELECTION       (WM_USER + 102)
#define TODAYCOLOR_TEXT            0x10000004
#define TODAYCOLOR_HIGHLIGHT       0x10000022
#define TODAYCOLOR_HIGHLIGHTEDTEXT 0x10000023

#define TODAYM_REQUESTREFRESH      (WM_USER + 103)
#define TODAYREFRESH_NOHEIGHTCHANGE 0x00000001

#define TODAYM_DRAWWATERMARK       (WM_USER + 101) // lParam == TODAYDRAWWATERMARKINFO*
typedef struct
{
    HDC hdc;
    RECT rc;
    HWND hwnd;
} TODAYDRAWWATERMARKINFO;

// the following block of message id #'s is reserved for internal use
#define TODAYM_RESERVEDSTART      (WM_USER + 300)
#define TODAYM_RESERVEDEND        (WM_USER + 399)

/* maximum number of today items */
#define k_cTodayItemsMax          30


/* custom DLL resources */

#define IDI_ICON                  128
#define IDD_TODAY_CUSTOM          500

/* custom DLL functions */

#define ORDINAL_INITIALIZEITEM    240
typedef HWND (*PFNCUSTOMINITIALIZEITEM)(TODAYLISTITEM *, HWND);

#define ORDINAL_OPTIONSDIALOGPROC 241
typedef BOOL (*PFNCUSTOMOPTIONSDLGPROC)(HWND, UINT, UINT, LONG);

/* custom DLL messages */

#define WM_TODAYCUSTOM_CLEARCACHE           (WM_USER + 242)
#define WM_TODAYCUSTOM_QUERYREFRESHCACHE    (WM_USER + 243)
#define WM_TODAYCUSTOM_RECEIVEDSELECTION    (WM_USER + 244)
#define WM_TODAYCUSTOM_LOSTSELECTION        (WM_USER + 245)
#define WM_TODAYCUSTOM_USERNAVIGATION       (WM_USER + 246)
#define WM_TODAYCUSTOM_ACTION               (WM_USER + 247)
#define WM_TODAYCUSTOM_CLOSEDATABASES       (WM_USER + 248)

/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* !_TODAYCMN_H_ */

