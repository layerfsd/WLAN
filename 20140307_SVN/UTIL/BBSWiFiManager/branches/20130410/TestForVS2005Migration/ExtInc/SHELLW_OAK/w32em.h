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
#ifndef _W32EM_H_
#define _W32EM_H_
// Copyright (c) 1994-1999, Microsoft Corp. All rights reserved.
//***   w32em.h -- emulated win32 (etc.) routines
// DESCRIPTION
//  CE is missing some win32 funcs.  we emulate them here.
//  All names are of the form XxxEM, where Xxx is the 'real' name.

#ifdef __cplusplus
extern "C" {
#endif

//***   win32
//
UINT GetMenuItemCountEM(HMENU hmenu);
UINT GetMenuItemIDEM(HMENU hmenu, int nPos);
UINT GetMenuStateEM(HMENU hmenu, UINT uId, UINT uFlags);

BOOL InsertMenuItemEM(HMENU hmDst, UINT uInsert, BOOL fByPos, MENUITEMINFO *pmiiSrc);

#define GetMenuItemCount    GetMenuItemCountEM
#define GetMenuItemID       GetMenuItemIDEM
#define GetMenuItemState    GetMenuItemStateEM
#define InsertMenuItem      InsertMenuItemEM

LONG RegOpenKeyEM(HKEY hk, LPCTSTR pszSubKey, PHKEY phkOut);
LONG RegQueryValueEM(HKEY hk, LPTSTR pszName, LPBYTE pbData, DWORD *pcbData);

#define RegOpenKey      RegOpenKeyEM
#define RegQueryValue   RegQueryValueEM

#ifndef IS_INTRESOURCE // just in case...
#define IS_INTRESOURCE(p)   (HIWORD(p) == 0)
#endif

//***   other
//

#ifdef __cplusplus
}
#endif

#endif // _W32EM_H_
