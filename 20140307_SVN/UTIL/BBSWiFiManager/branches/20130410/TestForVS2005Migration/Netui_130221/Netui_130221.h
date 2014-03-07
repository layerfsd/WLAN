// Netui_130221.h : Netui_130221 DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
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
// 이 클래스의 구현을 보려면 Netui_130221.cpp를 참조하십시오.
//

class CNetui_130221App : public CWinApp
{
public:
	CNetui_130221App();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

