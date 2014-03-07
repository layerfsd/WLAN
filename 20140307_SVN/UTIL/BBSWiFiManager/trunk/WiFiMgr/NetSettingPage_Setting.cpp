// NetSettingPage_Setting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Setting.h"


// CNetSettingPage_Setting 대화 상자입니다.

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


// CNetSettingPage_Setting 메시지 처리기입니다.

BOOL CNetSettingPage_Setting::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// Get Screen Rate (VGA or QVGA)
	m_dwScreenRate = (GetSystemMetrics(SM_CXSCREEN) == 480)? 2 : 1;

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNetSettingPage_Setting::OnTcnSelchangeTabSettingMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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


	// 메인 탭 사이즈 구하기
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
	// 각 선택된 ID에 맞는 Ping view 생성
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
	default : // 지정이 잘못되었을때는, 기본 Option View를 보여준다.
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if( m_pView != NULL )
	{
		m_pView->DestroyWindow();
		m_pView = NULL;
	}
}


BOOL CNetSettingPage_Setting::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
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

// Setting 페이지 초기화 작업
void CNetSettingPage_Setting::InitializeSettingPage()
{
	// Get Wlan Module Type
	BBS_ADAPTER_INFO adaterInfo;
	memset( &adaterInfo, 0, sizeof(BBS_ADAPTER_INFO) );

	GetWlanDefaultInformation(&adaterInfo);

	// 아답터 이름이 검색되었는지 확인
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
		else // 지원하지 않는 아답터 이름
		{
			m_dwAdapter = 0;
		}
	}


	// Main Tab Init
	m_tabMain.DeleteAllItems();
	m_tabMain.InsertItem(TAB_PAGE_OPTION, _T("Option") );

	// 아답터 이름이 검색된 경우에만 아답터 설정 페이지 Enable
	if( m_dwAdapter != 0 ) 
	{
		m_tabMain.InsertItem(TAB_PAGE_MODULE_SETTING, adaterInfo.tszAdapterName );
	}

	m_tabMain.InsertItem(TAB_PAGE_DEFAULTAP, _T("DefaultPreferNet") );

	m_tabMain.SetCurSel( TAB_PAGE_OPTION );

	
	// Create Fist View
	ChangeView(IDD_NETSETTINGPAGE_SETTING_OPTIONVIEW);
}
