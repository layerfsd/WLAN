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
#ifndef __FEXPFRAMEPUB_H_
#define __FEXPFRAMEPUB_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WM_SETFOCUSITEMINFO     (WM_USER + 4351)


// LPARAM for WM_SETFOCUSITEMINFO
struct SLVFocusInfo
{
    LPCTSTR pszSK1;
    LPCTSTR pszHeader;
    LPCTSTR pszFilename;
};

#ifdef __cplusplus
}
#endif

#endif /* __FEXPFRAMEPUB_H__ */
