// Netui_WM.h : Netui_WM DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"
#include <BBS_WifiXml.h>


// CNetui_WMApp
// �� Ŭ������ ������ ������ Netui_WM.cpp�� �����Ͻʽÿ�.
//

class CNetui_WMApp : public CWinApp
{
public:
	CNetui_WMApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

