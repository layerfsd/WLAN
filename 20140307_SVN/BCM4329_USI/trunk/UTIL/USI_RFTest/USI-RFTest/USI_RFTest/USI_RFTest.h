// USI_RFTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

// CUSI_RFTestApp:
// �� Ŭ������ ������ ���ؼ��� USI_RFTest.cpp�� �����Ͻʽÿ�.
//

class CUSI_RFTestApp : public CWinApp
{
public:
	CUSI_RFTestApp();
	
// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CUSI_RFTestApp theApp;
