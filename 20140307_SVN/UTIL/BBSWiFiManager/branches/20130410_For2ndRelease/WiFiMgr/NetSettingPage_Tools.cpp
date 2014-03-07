// NetSettingPage_Tools.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Tools.h"


// CNetSettingPage_Tools ��ȭ �����Դϴ�.

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


// CNetSettingPage_Tools �޽��� ó�����Դϴ�.

BOOL CNetSettingPage_Tools::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.


	m_dwScreenRate = (GetSystemMetrics(SM_CXSCREEN) == 480)? 2 : 1;



	m_tabMain.InsertItem(TAB_PAGE_PING, _T("Ping") );
	m_tabMain.InsertItem(TAB_PAGE_SCAN, _T("WiFiScan") );
	m_tabMain.InsertItem(TAB_PAGE_LOG, _T("Log") );

	// �⺻ ó��ȭ�� : Ping
	m_tabMain.SetCurSel( TAB_PAGE_PING );

	

	//
	// ������ ���ϱ�
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

	// Ping view ����
	m_pViewPing = (CView*)RUNTIME_CLASS(CNetSettingPage_Tools_PingView)->CreateObject();
	::ZeroMemory(&cc, sizeof(cc));
	m_pViewPing->Create(NULL, NULL, WS_CHILD, rect, &m_tabMain, IDD_NETSETTINGPAGE_TOOLS_PINGVIEW, &cc);
	m_pViewPing->OnInitialUpdate();
	m_pViewPing->ShowWindow(SW_SHOW);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CNetSettingPage_Tools::OnTcnSelchangeTabToolsMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.


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


	// ���� �� ������ ���ϱ�
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

	// �� ���õ� ID�� �´� Ping view ����
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
