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
//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module:  cdebug.h
//  date:    
//  author:  
//
//  purpose: 
//
//----------------------------------------------------------------------------
#ifndef __CDEBUG_H__
#define __CDEBUG_H__     

#ifdef __cplusplus

class CDebug {
private:
	TCHAR m_szBuffer[1024];
	BOOL  m_bPrevFmt, m_bDebugOutput;
	DWORD m_dwDebugLevel;
	
	static DWORD dwDebugLevel;
	static BOOL bDoOutput;
	
public:
	CDebug(BOOL bDebugOutput = TRUE);
	~CDebug();
	
	static VOID StartDebugging(VOID) { bDoOutput = TRUE;  }
	static VOID StopDebugging(VOID)  { bDoOutput = FALSE; }
	
	VOID Reset(VOID); 
	VOID DebugOutput(VOID);
	
	CDebug& operator <<(LPCTSTR psz);
#ifndef UNICODE
	CDebug& operator <<(LPCOLESTR psz);
#endif
	CDebug& operator <<(WORD w);
	CDebug& operator <<(DWORD dw);
	CDebug& operator <<(SCRIPTSTATE ss);
	CDebug& operator <<(BOOL b);
	CDebug& operator <<(IID iid);
	CDebug& operator <<(LPCLSID pclsid);
	CDebug& operator <<(const LPRECT prc);
	CDebug& operator <<(LPCRECT prc);
	CDebug& operator <<(LPCRECTL prc);
	CDebug& operator <<(SIZE size);
	CDebug& operator <<(const LPSIZEL psizel);
	CDebug& operator <<(const LPUNKNOWN punk);
	CDebug& operator <<(HRESULT hRes);
	
}; /* CDebug */

#endif

#endif /* __CDEBUG_H__ */
