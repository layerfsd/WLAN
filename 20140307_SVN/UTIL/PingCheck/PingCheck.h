// PingCheck.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 



#include "BBS_Ping.h"
#include "BBS_CommFunc.h"
#include "BBS_NdisFuncs.h"

#define DEFAULT_MUTEX_NAME	_T("PING_CHECK")
#define WM_STOP_CHECK	WM_USER + 100


// CPingCheckApp:
// See PingCheck.cpp for the implementation of this class
//

class CPingCheckApp : public CWinApp
{
public:
	CPingCheckApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPingCheckApp theApp;
