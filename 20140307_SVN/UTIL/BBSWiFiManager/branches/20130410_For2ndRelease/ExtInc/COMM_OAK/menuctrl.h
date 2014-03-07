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
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

menuctrl.h

Abstract:  

Notes: 


--*/
#ifndef _MENUCTRL_H_
#define _MENUCTRL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* When creating a window of class "Menu", pass a PMENUCONTROLINFO as the 
   lpParam parameter to CreateWindow.
 */

#define MENUCONTROLCLASS TEXT("Menu")
 
typedef struct
{
    HINSTANCE hinst;        /* instance containing the menu resource */
    LPCTSTR   lpszMenuName; /* menu resource name or ID */
    HWND      hwndNotify;   /* the window that will receive menu notifications*/
} MENUCONTROLINFO, *PMENUCONTROLINFO, *LPMENUCONTROLINFO;

#ifdef __cplusplus
}
#endif

#endif /* _MENUCTRL_H_ */
