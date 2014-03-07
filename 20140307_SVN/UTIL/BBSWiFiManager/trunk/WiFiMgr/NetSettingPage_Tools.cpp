// NetSettingPage_Tools.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Tools.h"


// CNetSettingPage_Tools 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNetSettingPage_Tools, CPropertyPage)

CNetSettingPage_Tools::CNetSettingPage_Tools()
	: CPropertyPage(CNetSettingPage_Tools::IDD)
{
	m_dwScreenRate = 1;

	m_pViewPing = NULL;
	m_pView = NULL;

}

CNetSettingPage_Tools::~CNetSettingPage_Tools()
{
}

void CNetSettingPage_Tools::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_TOOLS_MAIN, m_tabMain);
}


BEGIN_MESSAGE_MAP(CNetSettingPage_Tools, CPropertyPage)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TOOLS_MAIN, &CNetSettingPage_Tools::OnTcnSelchangeTabToolsMain)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CNetSettingPage_Tools 메시지 처리기입니다.

BOOL CNetSettingPage_Tools::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	m_dwScreenRate = (GetSystemMetrics(SM_CXSCREEN) == 480)? 2 : 1;



	m_tabMain.InsertItem(TAB_PAGE_PING, _T("Ping") );
	m_tabMain.InsertItem(TAB_PAGE_SCAN, _T("WiFiScan") );
	m_tabMain.InsertItem(TAB_PAGE_LOG, _T("Log") );

	// 기본 처음화면 : Ping
	m_tabMain.SetCurSel( TAB_PAGE_PING );

	

	//
	// 사이즈 구하기
	//
	RECT tabItemRect = {0,};
	RECT rect = {0,};

	m_tabMain.GetClientRect(&rect);
	m_tabMain.GetItemRect(0, &tabItemRect);
	
	rect.left += (5 * m_dwScreenRate);
	rect.top += (5 * m_dwScreenRate) + tabItemRect.bottom;
	rect.right -= rect.left;
	rect.bottom -= (5 * m_dwScreenRate);


	//
	// Create FormView
	//
	CCreateContext cc;

	// Ping view 생성
	m_pViewPing = (CView*)RUNTIME_CLASS(CNetSettingPage_Tools_PingView)->CreateObject();
	::ZeroMemory(&cc, sizeof(cc));
	m_pViewPing->Create(NULL, NULL, WS_CHILD, rect, &m_tabMain, IDD_NETSETTINGPAGE_TOOLS_PINGVIEW, &cc);
	m_pViewPing->OnInitialUpdate();
	m_pViewPing->ShowWindow(SW_SHOW);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNetSettingPage_Tools::OnTcnSelchangeTabToolsMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nSelectedTab = m_tabMain.GetCurSel();

	m_pViewPing->ShowWindow(SW_HIDE);
	
	if( m_pView != NULL )
	{
		m_pView->DestroyWindow();
		m_pView = NULL;
	}


	switch( nSelectedTab )
	{
	case TAB_PAGE_PING:
		m_pViewPing->ShowWindow(SW_SHOW);
		break;
	case TAB_PAGE_SCAN:
		ChangeView(IDD_NETSETTINGPAGE_TOOLS_SCANVIEW);
		break;
	case TAB_PAGE_LOG:
		ChangeView(IDD_NETSETTINGPAGE_TOOLS_LOGVIEW);
		break;
	}
	

	*pResult = 0;
}



void CNetSettingPage_Tools::OnDestroy()
{
	CPropertyPage::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.


	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Tools::OnDestroy() \r\n") ));

	if( m_pViewPing )
	{
		m_pViewPing->DestroyWindow();
	}

	if( m_pView )
	{
		m_pView->DestroyWindow();
	}
}


void CNetSettingPage_Tools::ChangeView(INT nId)
{
	RECT tabItemRect = {0,};
	RECT rect = {0,};


	// 메인 탭 사이즈 구하기
	m_tabMain.GetClientRect(&rect);
	m_tabMain.GetItemRect(0, &tabItemRect);
	
	rect.left += (5 * m_dwScreenRate);
	rect.top += (5 * m_dwScreenRate) + tabItemRect.bottom;
	rect.right -= rect.left;
	rect.bottom -= (5 * m_dwScreenRate);

	//
	// Create FormView
	//
	CCreateContext cc;

	// 각 선택된 ID에 맞는 Ping view 생성
	switch(nId)
	{
	case IDD_NETSETTINGPAGE_TOOLS_LOGVIEW:
		m_pView = (CView*)RUNTIME_CLASS(CNetSettingPage_Tools_LogView)->CreateObject();
		break;
		
	case IDD_NETSETTINGPAGE_TOOLS_SCANVIEW:
		m_pView = (CView*)RUNTIME_CLASS(CNetSettingPage_Tools_ScanView)->CreateObject();
		break;

	}

	::ZeroMemory(&cc, sizeof(cc));
	m_pView->Create(NULL, NULL, WS_CHILD, rect, &m_tabMain, nId, &cc);
	m_pView->OnInitialUpdate();
	m_pView->ShowWindow(SW_SHOW);
}
