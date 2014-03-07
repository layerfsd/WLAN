// NetSettingPage_Tools_LogView.cpp : 구현 파일입니다.
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


// CNetSettingPage_Tools_LogView 진단입니다.

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


// CNetSettingPage_Tools_LogView 메시지 처리기입니다.

void CNetSettingPage_Tools_LogView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnInitialUpdate() \r\n") ));
}

void CNetSettingPage_Tools_LogView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools_LogView::OnDestroy() \r\n") ));
}
