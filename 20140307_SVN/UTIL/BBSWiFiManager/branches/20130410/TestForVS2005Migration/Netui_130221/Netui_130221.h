// Netui_130221.h : Netui_130221 DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include <resource.h>
#endif
#ifdef POCKETPC2003_UI_MODEL
#include "resource.h"
#endif 
#ifdef SMARTPHONE2003_UI_MODEL
#include "resourcesp.h"
#endif

// CNetui_130221App
// �� Ŭ������ ������ ������ Netui_130221.cpp�� �����Ͻʽÿ�.
//

class CNetui_130221App : public CWinApp
{
public:
	CNetui_130221App();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

