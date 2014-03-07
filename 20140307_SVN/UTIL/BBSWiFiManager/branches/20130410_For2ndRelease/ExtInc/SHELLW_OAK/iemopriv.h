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
/*************************************************************************
*
*  Copyright (C) 2004 Microsoft Corporation. All rights reserved.
*
*  MODULE : iemopriv.h 
*
*  PURPOSE: Private IE Mobile definitions
*
*************************************************************************/

#ifndef __IEMOPRIV_H__
#define __IEMOPRIV_H__

#ifdef __cplusplus
extern "C" {
#endif

// IE Verbs
#define IEV_NULL                    0       //  NOOP - do not use
#define IEV_VSCROLL_PAGEUP          1       //  Vertical Scroll Page Up
#define IEV_VSCROLL_PAGEDOWN        2       //  Vertical Scroll Page Down
#define IEV_VSCROLL_TOP             3       //  Vertical Scroll to top
#define IEV_VSCROLL_BOTTOM          4       //  Vertical Scroll to bottom
#define IEV_HSCROLL_PAGEUP          5       //  Horizontal Scroll Page Up
#define IEV_HSCROLL_PAGEDOWN        6       //  Horizontal Scroll Page Down
#define IEV_HSCROLL_TOP             7       //  Horizontal Scroll to top
#define IEV_HSCROLL_BOTTOM          8       //  Horizontal Scroll to bottom
#define IEV_LAYOUT_DEFAULT          9       //  Set Default layout
#define IEV_LAYOUT_DESKTOP          10      //  Set Desktop layout
#define IEV_LAYOUT_ONECOLUMN        11      //  Set One-Column layout
#define IEV_FULLSCREEN_TOGGLE       12      //  Toggle Full-Screen
#define IEV_SHOWPICTURES_TOGGLE     13      //  Toggle Show Pictures
#define IEV_ZOOMIN                  14      //  Zoom in the web page
#define IEV_ZOOMOUT                 15      //  Zoom out the web page

#ifdef __cplusplus
}
#endif
 
#endif // __IEMOPRIV_H__
