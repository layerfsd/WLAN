// NetSettingPropertySheet.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Netui_WM.h"
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
	
	AddPage(&page_Info);
	AddPage(&page_Wifi);
	AddPage(&page_NetAdapter);
	//AddPage(&page_Profile);
	AddPage(&page_Setting);
	AddPage(&page_Tools);

}

CNetSettingPropertySheet::~CNetSettingPropertySheet()
{
	RemovePage(&page_Info);
	RemovePage(&page_Wifi);
	RemovePage(&page_NetAdapter);
	//RemovePage(&page_Profile);
	RemovePage(&page_Setting);
	RemovePage(&page_Tools);
	
}


BEGIN_MESSAGE_MAP(CNetSettingPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CNetSettingPropertySheet �޽��� ó�����Դϴ�.
BOOL CNetSettingPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  ���⿡ Ư��ȭ�� �ڵ带 �߰��մϴ�.
	return bResult;
}
