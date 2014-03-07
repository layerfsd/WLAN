// Netui_WM.h : Netui_WM DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"
#include <BBS_WifiXml.h>


// CNetui_WMApp
// 이 클래스의 구현을 보려면 Netui_WM.cpp를 참조하십시오.
//

class CNetui_WMApp : public CWinApp
{
public:
	CNetui_WMApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

