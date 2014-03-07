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
/*---------------------------------------------------------------------------*\
 *
 * (c) Copyright Microsoft Corp. 1994 All Rights Reserved
 *
 *  module: path.h
 *  date:
 *  author: tonykit
 *
 *  purpose: 
 *
\*---------------------------------------------------------------------------*/
#ifndef __PATH_H__
#define __PATH_H__

/////////////////////////////////////////////////////////////////////////////

#define GCT_INVALID             0x0000
#define GCT_LFNCHAR             0x0001
#define GCT_SHORTCHAR           0x0002
#define GCT_WILD                0x0004
#define GCT_SEPERATOR           0x0008

#ifdef __cplusplus
extern "C" {
#endif

UINT           PathGetCharType(TCHAR ch);
BOOL           PathIsRelative(__in LPCTSTR lpszPath);
LPTSTR         PathCombine(__out_ecount(MAX_PATH) LPTSTR lpszDest,
                           __in __opt LPCTSTR lpszDir,
                           __in __opt LPCTSTR lpszFile);
LPTSTR          PathAddBackslash(__in LPTSTR lpszPath);
LPTSTR          PathRemoveBackslash(__in LPTSTR lpszPath);
WINSHELLAPI BOOL    WINAPI PathIsValidPath(__in LPCTSTR pPath);
WINSHELLAPI BOOL    WINAPI PathIsValidFileName(__in LPCTSTR pPath);
WINSHELLAPI void    WINAPI PathRemoveBlanks(__inout LPTSTR lpszString);
WINSHELLAPI LPTSTR  WINAPI PathFindExtension(__in LPCTSTR pszPath);     // Now in ossvcs, pathutils.h
WINSHELLAPI LPTSTR  WINAPI PathFindFileName(__in LPCTSTR pPath);
WINSHELLAPI void    WINAPI PathStripPath(__inout LPTSTR pszPath);
WINSHELLAPI void    WINAPI PathCompactSlashes(__inout LPTSTR lpszPath);
WINSHELLAPI BOOL    WINAPI PathToManySlashes(__in LPCTSTR pszPath);

WINSHELLAPI BOOL    WINAPI PathCompactPath(HDC hDC, __inout LPTSTR lpszPath, UINT dx);
WINSHELLAPI BOOL    WINAPI PathIsSystemPath(__in LPCTSTR pszPath);
WINSHELLAPI BOOL    WINAPI PathIsDirectory(__in LPCTSTR pszPath);
WINSHELLAPI HRESULT WINAPI PathIsGUID(__in LPCTSTR pszPath);
WINSHELLAPI BOOL    WINAPI PathIsExtension(__in LPCTSTR szFile, __in LPCTSTR szExt);
WINSHELLAPI BOOL    WINAPI PathIsExe(__in LPCTSTR szFile);
WINSHELLAPI BOOL    WINAPI PathIsLink(__in LPCTSTR szFile);
WINSHELLAPI BOOL    WINAPI PathMakePretty(__inout LPTSTR lpPath);
WINSHELLAPI BOOL    WINAPI PathFileExists(__in LPCTSTR lpszPath);
WINSHELLAPI void    WINAPI PathRemoveExtension(__inout LPTSTR pszPath);
WINSHELLAPI BOOL    WINAPI PathRemoveFileSpec(__inout LPTSTR pFile);
WINSHELLAPI void    WINAPI PathRemoveArgs(__inout LPTSTR pszPath);
WINSHELLAPI LPTSTR  WINAPI PathGetArgs(__in LPCTSTR pszPath);
WINSHELLAPI void    WINAPI PathRemoveQuotesAndArgs(__inout LPTSTR pszPath);
WINSHELLAPI BOOL    WINAPI PathOnExtList(__in LPCTSTR pszExtList, __in LPCTSTR pszExt);
WINSHELLAPI BOOL    WINAPI PathMatchSpec(__in LPCTSTR pszFileParam, __in __opt LPCTSTR pszSpec);
WINSHELLAPI BOOL    WINAPI PathMakeUniqueName(__in LPCTSTR lpszPath,
                                              __in __opt LPCTSTR lpszT1,
                                              __in __opt LPCTSTR lpszT2,
                                              BOOL fLink,
                                              __out_ecount(cchUniqueName) LPTSTR  lpszUniqueName,
                                              const size_t cchUniqueName);
WINSHELLAPI BOOL    WINAPI PathIsRemovableDevice(__in LPCTSTR pszPath);
LPTSTR PathFindNextComponent(__in LPCTSTR pszPath);
WINSHELLAPI BOOL    WINAPI PathIsPrefix(__in LPCTSTR pszPrefix,
                                        __in LPCTSTR pszPath);

#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////
	
#endif /* __PATH_H__ */
