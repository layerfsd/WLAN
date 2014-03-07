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
// Copyright (c) 1999-1999, Microsoft Corp. All rights reserved.
#ifndef _APPMET_H_
#define _APPMET_H_ 1
//***   appmet.h -- app metrics
//

//***   generic/shared stuff -- {

//BUGBUG all this should move to appmet2.h

#define CX_SCREEN           208     // BUGBUG eventually, may be dynamic?
#define CX_BORDER           2
#define CY_BORDER           2
#define CX_ACCUM            100
#define CY_ACCUM            20      // height of accumulator
#define CX_ICON             16
#define CY_ICON             16

// }

//***   DT_XNAME, etc. -- typical DT_* bits for a listview display name, etc.
//
#define DT_XNAME    (DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX)
#define DT_XMETHOD  (DT_RIGHT|DT_NOPREFIX)
#define DT_XSLITEM  (DT_VCENTER|DT_SINGLELINE)

//***   VCTR_CORNER -- calc how to center item in rect
// DESCRIPTION
//  use VCTR_CORNER(rcDraw, CX_ICON) to find where to put corner of icon if
// want to center it in a rect.
// NOTES
//  todo: does this belong here?
#define VCTR_CORNER(rcDraw, cyItem) \
    (rcDraw.top + (RECTHEIGHT(rcDraw) - cyItem) / 2)


// build the enumeration
#define APPMET_FIRST_INCLUDE 1
#define APPMET_INIT_ENUM 1
#include "appmet2.h"
#undef APPMET_FIRST_INCLUDE
#undef APPMET_INIT_ENUM

// build the constants
#define APPMET_INIT_CONSTANTS 1
#include "appmet2.h"
#undef APPMET_INIT_CONSTANTS

// Public interface to app metrics.  See appmet2.h for documentation.
BOOL OnSettingChange(WPARAM wParam, LPARAM lParam);
void AMIgnoreMetricChanges();

int GetAppMetric(enAppMetricID id);
#define GetAppMetricConst(id) GetAppMetric(ami ## id)
#define GetAppColorConst(id) CAM_C_ ## id

#ifdef __WPCPRIV_H__
#define AM_APPBARHEIGHT SHGetMetric(SHUI_APPBARHEIGHT)
#define AM_SM_CXICON    SHGetMetric(SHUI_SM_CXICON)
#define AM_SM_CYICON    SHGetMetric(SHUI_SM_CXICON)
#define AM_SM_CXSMICON  SHGetMetric(SHUI_SM_CXSMICON)
#define AM_SM_CYSMICON  SHGetMetric(SHUI_SM_CYSMICON)
#define AM_SM_CXBORDER  SHGetMetric(SHUI_SM_CXBORDER)
#define AM_SM_CYBORDER  SHGetMetric(SHUI_SM_CXBORDER) /* Assume y is same as x */
#endif

#endif // _APPMET_H_
