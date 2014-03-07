// NetSettingPage_Setting.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Setting.h"


// CNetSettingPage_Setting ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNetSettingPage_Setting, CPropertyPage)

CNetSettingPage_Setting::CNetSettingPage_Setting()
	: CPropertyPage(CNetSettingPage_Setting::IDD)
{
	m_dwScreenRate = 1;
	m_pView = NULL;
	m_dwAdapter = 0;
}

CNetSettingPage_Setting::~CNetSettingPage_Setting()
{
}

void CNetSettingPage_Setting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SETTING_MAIN, m_tabMain);
}


BEGIN_MESSAGE_MAP(CNetSettingPage_Setting, CPropertyPage)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTING_MAIN, &CNetSettingPage_Setting::OnTcnSelchangeTabSettingMain)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CNetSettingPage_Setting �޽��� ó�����Դϴ�.

BOOL CNetSettingPage_Setting::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// Get Screen Rate (VGA or QVGA)
	m_dwScreenRate = (GetSystemMetrics(SM_CXSCREEN) == 480)? 2 : 1;

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CNetSettingPage_Setting::OnTcnSelchangeTabSettingMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int nSelectedTab = m_tabMain.GetCurSel();

	RETAILMSG(1, (TEXT("[NETUI] OnTcnSelchangeTabSettingMain \r\n") ));


	switch(nSelectedTab)
	{
	case TAB_PAGE_OPTION:
		ChangeView(IDD_NETSETTINGPAGE_SETTING_OPTIONVIEW);
		break;

	case TAB_PAGE_MODULE_SETTING:
		switch( m_dwAdapter )
		{
		case ADAPTER_MMC:
			ChangeView(IDD_NETSETTINGPAGE_SETTING_MMCVIEW);
			break;
		case ADAPTER_USI:
			ChangeView(IDD_NETSETTINGPAGE_SETTING_USIVIEW);
			break;
		}
		break;

	case TAB_PAGE_DEFAULTAP:
		ChangeView(IDD_NETSETTINGPAGE_SETTING_DEFPREFERAPVIEW);
		break;
	}

	*pResult = 0;
}


void CNetSettingPage_Setting::ChangeView(INT nId)
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


	if( m_pView != NULL )
	{
		m_pView->DestroyWindow();
		m_pView = NULL;
	}

	//
	// Create FormView
	//
	CCreateContext cc;


	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Setting ChangeView - nId(%d) \r\n"), nId ));
	// �� ���õ� ID�� �´� Ping view ����
	switch(nId)
	{
	case IDD_NETSETTINGPAGE_SETTING_OPTIONVIEW:
		m_pView = (CView*)RUNTIME_CLASS(CNetSettingPage_Setting_OptionView)->CreateObject();
		break;
	case IDD_NETSETTINGPAGE_SETTING_MMCVIEW:
		m_pView = (CView*)RUNTIME_CLASS(CNetSettingPage_Setting_MMCView)->CreateObject();
		break;
	case IDD_NETSETTINGPAGE_SETTING_USIVIEW:
		m_pView = (CView*)RUNTIME_CLASS(CNetSettingPage_Setting_USIView)->CreateObject();
		break;
	case IDD_NETSETTINGPAGE_SETTING_DEFPREFERAPVIEW:
		m_pView = (CView*)RUNTIME_CLASS(CNetSettingPage_Setting_DefPreferAPView)->CreateObject();
		break;
	default : // ������ �߸��Ǿ�������, �⺻ Option View�� �����ش�.
		m_pView = (CView*)RUNTIME_CLASS(CNetSettingPage_Setting_OptionView)->CreateObject();
		break;
	}


	::ZeroMemory(&cc, sizeof(cc));
	m_pView->Create(NULL, NULL, WS_CHILD, rect, &m_tabMain, nId, &cc);
	m_pView->OnInitialUpdate();
	m_pView->ShowWindow(SW_SHOW);
}



void CNetSettingPage_Setting::OnDestroy()
{
	CPropertyPage::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	if( m_pView != NULL )
	{
		m_pView->DestroyWindow();
		m_pView = NULL;
	}
}


BOOL CNetSettingPage_Setting::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	LPNMHDR pnmh = (LPNMHDR) lParam; 

	switch(pnmh->code)
	{
	case PSN_KILLACTIVE:
		RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Setting - PSN_KILLACTIVE \r\n") ));

		if( m_pView != NULL )
		{
			m_pView->DestroyWindow();
			m_pView = NULL;
		}
		break;
	case PSN_SETACTIVE:
		RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Setting - PSN_SETACTIVE \r\n") ));
		InitializeSettingPage();
		break;
	}


	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

// Setting ������ �ʱ�ȭ �۾�
void CNetSettingPage_Setting::InitializeSettingPage()
{
	// Get Wlan Module Type
	BBS_ADAPTER_INFO adaterInfo;
	memset( &adaterInfo, 0, sizeof(BBS_ADAPTER_INFO) );

	GetWlanDefaultInformation(&adaterInfo);

	// �ƴ��� �̸��� �˻��Ǿ����� Ȯ��
	if( _tcslen(adaterInfo.tszAdapterName) != 0 )
	{
		if( _tcscmp(adaterInfo.tszAdapterName, DEFAULT_ADAPTER_NAME_MMC) == 0 )
		{
			m_dwAdapter = ADAPTER_MMC;
		}
		else if( _tcscmp(adaterInfo.tszAdapterName, DEFAULT_ADAPTER_NAME_USI) == 0 )
		{
			m_dwAdapter = ADAPTER_USI;
		}
		else // �������� �ʴ� �ƴ��� �̸�
		{
			m_dwAdapter = 0;
		}
	}


	// Main Tab Init
	m_tabMain.DeleteAllItems();
	m_tabMain.InsertItem(TAB_PAGE_OPTION, _T("Option") );

	// �ƴ��� �̸��� �˻��� ��쿡�� �ƴ��� ���� ������ Enable
	if( m_dwAdapter != 0 ) 
	{
		m_tabMain.InsertItem(TAB_PAGE_MODULE_SETTING, adaterInfo.tszAdapterName );
	}

	m_tabMain.InsertItem(TAB_PAGE_DEFAULTAP, _T("DefaultPreferNet") );

	m_tabMain.SetCurSel( TAB_PAGE_OPTION );

	
	// Create Fist View
	ChangeView(IDD_NETSETTINGPAGE_SETTING_OPTIONVIEW);
}
