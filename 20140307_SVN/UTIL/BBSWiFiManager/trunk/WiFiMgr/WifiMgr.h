// WifiMgr.h : WifiMgr DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"
#include <BBS_WifiXml.h>


// CWifiMgrApp
// �� Ŭ������ ������ ������ WifiMgr.cpp�� �����Ͻʽÿ�.
//

class CWifiMgrApp : public CWinApp
{
public:
	CWifiMgrApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

