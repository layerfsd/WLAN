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

Copyright (c) 2003 Microsoft Corporation

Module Name:

    ToolkitShell.h

Abstract:

    Include file for Windows CE SIM toolkit Shell API

Author:

    Tim Milligan (timmill)         07-July-2003

--*/

// Flags to SHToolkitQueryShell
#define SHTOOLKIT_PRIORITY_HIGH              1
#define SHTOOLKIT_PRIORITY_LOW               2

HRESULT WINAPI SHSetSimToolkitMenu(LPCTSTR pszMenu);
UINT WINAPI SHGetSimToolkitMenu(LPTSTR pszMenu, UINT cchBuf);
HRESULT WINAPI SHToolkitQueryShell(DWORD dwPrioirty, LPCTSTR lpszText);
HRESULT WINAPI SHShowSimToolkitUI();

