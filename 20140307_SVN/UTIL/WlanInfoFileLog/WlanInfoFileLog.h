// WlanInfoFileLog.h : main header file for the WLANINFOFILELOG application
//

#if !defined(AFX_WLANINFOFILELOG_H__B46F09D9_6981_4663_B825_0715C78DBEC5__INCLUDED_)
#define AFX_WLANINFOFILELOG_H__B46F09D9_6981_4663_B825_0715C78DBEC5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWlanInfoFileLogApp:
// See WlanInfoFileLog.cpp for the implementation of this class
//

class CWlanInfoFileLogApp : public CWinApp
{
public:
	CWlanInfoFileLogApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWlanInfoFileLogApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWlanInfoFileLogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WLANINFOFILELOG_H__B46F09D9_6981_4663_B825_0715C78DBEC5__INCLUDED_)
