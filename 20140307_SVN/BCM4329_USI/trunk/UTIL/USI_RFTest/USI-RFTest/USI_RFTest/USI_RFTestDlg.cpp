// USI_RFTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "USI_RFTest.h"
#include "USI_RFTestDlg.h"
#include "FormView_Wireless.h"
#include "FormView_Bluetooth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUSI_RFTestDlg 대화 상자

CUSI_RFTestDlg::CUSI_RFTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUSI_RFTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwScreenRate = 1;
	m_pView = NULL;
}

void CUSI_RFTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);
}

BEGIN_MESSAGE_MAP(CUSI_RFTestDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CUSI_RFTestDlg::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()


// CUSI_RFTestDlg 메시지 처리기

BOOL CUSI_RFTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	// Get Screen Rate (VGA or QVGA)
	m_dwScreenRate = (GetSystemMetrics(SM_CXSCREEN) == 480)? 2 : 1;

	m_tabMain.InsertItem(0, _T("Wireless") );
	m_tabMain.InsertItem(1, _T("Bluetooth") );
	m_tabMain.SetCurSel(0);

	// Create Fist View
	ChangeView(IDD_FORMVIEW_WIRELESS);



	// GetFile Version
	TCHAR tszVer[MAX_PATH] = L"";
	
	GetCurrentFileVersion(tszVer);
	SetDlgItemText(IDC_STATIC_VERSION, tszVer);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CUSI_RFTestDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_USI_RFTEST_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_USI_RFTEST_DIALOG));
	}
}
#endif


void CUSI_RFTestDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nSelectedTab = m_tabMain.GetCurSel();

	switch(nSelectedTab)
	{
	case 0:
		ChangeView(IDD_FORMVIEW_WIRELESS);
		break;

	case 1:
		ChangeView(IDD_FORMVIEW_BLUETOOTH);
		break;

	default :
		ChangeView(IDD_FORMVIEW_WIRELESS);
		break;
	}


	*pResult = 0;
}

void CUSI_RFTestDlg::ChangeView(INT nID)
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


	// 각 선택된 ID에 맞는 Ping view 생성
	switch(nID)
	{
	case IDD_FORMVIEW_WIRELESS:
		m_pView = (CView*)RUNTIME_CLASS(CFormView_Wireless)->CreateObject();
		break;
	case IDD_FORMVIEW_BLUETOOTH:
		m_pView = (CView*)RUNTIME_CLASS(CFormView_Bluetooth)->CreateObject();
		break;
	default : // 지정이 잘못되었을때는, 기본 View를 보여준다.
		m_pView = (CView*)RUNTIME_CLASS(CFormView_Wireless)->CreateObject();
		break;
	}


	::ZeroMemory(&cc, sizeof(cc));
	m_pView->Create(NULL, NULL, WS_CHILD, rect, &m_tabMain, nID, &cc);
	m_pView->OnInitialUpdate();
	m_pView->ShowWindow(SW_SHOW);

}


// 현재 실행파일의 파일 버전 얻어오기
BOOL CUSI_RFTestDlg::GetCurrentFileVersion(LPTSTR tszFileVer)
{
	BOOL bRet = FALSE;
	DWORD dwInfoSize = 0;
	TCHAR tszFilePath[MAX_PATH] = {'\0',};

	char *pBuf = NULL;

	tszFileVer[0] = '\0';

	GetModuleFileName(NULL, tszFilePath, MAX_PATH);

	dwInfoSize = GetFileVersionInfoSize(tszFilePath, 0);

	pBuf = new char[dwInfoSize];

	if( dwInfoSize == 0 )
	{
		bRet = FALSE;
	}

	if(GetFileVersionInfo(tszFilePath, 0, dwInfoSize, pBuf) !=0 )
	{
		VS_FIXEDFILEINFO* pFineInfo = NULL;
		UINT bufLen = 0;
		if(VerQueryValue(pBuf, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
		{   
			bRet = TRUE;
			wsprintf(tszFileVer, _T("v%d.%d.%d.%d"), HIWORD(pFineInfo->dwFileVersionMS), LOWORD(pFineInfo->dwFileVersionMS), HIWORD(pFineInfo->dwFileVersionLS), LOWORD(pFineInfo->dwFileVersionLS) );
		}
	}

	delete pBuf;


	return bRet;
}

