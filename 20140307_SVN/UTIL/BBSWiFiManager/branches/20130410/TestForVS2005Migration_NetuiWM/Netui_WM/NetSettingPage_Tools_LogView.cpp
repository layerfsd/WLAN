// NetSettingPage_Tools_LogView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Netui_WM.h"
#include "NetSettingPage_Tools_LogView.h"


// CNetSettingPage_Tools_LogView

IMPLEMENT_DYNCREATE(CNetSettingPage_Tools_LogView, CFormView)

CNetSettingPage_Tools_LogView::CNetSettingPage_Tools_LogView()
	: CFormView(CNetSettingPage_Tools_LogView::IDD)
{

}

CNetSettingPage_Tools_LogView::~CNetSettingPage_Tools_LogView()
{
}

void CNetSettingPage_Tools_LogView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetSettingPage_Tools_LogView, CFormView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CNetSettingPage_Tools_LogView �����Դϴ�.

#ifdef _DEBUG
void CNetSettingPage_Tools_LogView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetSettingPage_Tools_LogView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNetSettingPage_Tools_LogView �޽��� ó�����Դϴ�.

void CNetSettingPage_Tools_LogView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnInitialUpdate() \r\n") ));
}

void CNetSettingPage_Tools_LogView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnDestroy() \r\n") ));
}
