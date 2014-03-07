// NetSettingPage_Profile.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Netui_WM.h"
#include "NetSettingPage_Profile.h"


// CNetSettingPage_Profile 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNetSettingPage_Profile, CPropertyPage)

CNetSettingPage_Profile::CNetSettingPage_Profile()
	: CPropertyPage(CNetSettingPage_Profile::IDD)
{

}

CNetSettingPage_Profile::~CNetSettingPage_Profile()
{
}

void CNetSettingPage_Profile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNetSettingPage_Profile, CPropertyPage)
END_MESSAGE_MAP()


// CNetSettingPage_Profile 메시지 처리기입니다.
