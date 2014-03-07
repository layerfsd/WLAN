// NetSettingPropertySheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Netui_130221.h"
#include "NetSettingPropertySheet.h"


// CNetSettingPropertySheet

IMPLEMENT_DYNAMIC(CNetSettingPropertySheet, CPropertySheet)

CNetSettingPropertySheet::CNetSettingPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	
}

CNetSettingPropertySheet::CNetSettingPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&page3);
	AddPage(&page1);
}

CNetSettingPropertySheet::~CNetSettingPropertySheet()
{
	RemovePage(&page3);
	RemovePage(&page1);
}


BEGIN_MESSAGE_MAP(CNetSettingPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CNetSettingPropertySheet 메시지 처리기입니다.
BOOL CNetSettingPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  여기에 특수화된 코드를 추가합니다.
	//AddPage(&page1);
	return bResult;
}
