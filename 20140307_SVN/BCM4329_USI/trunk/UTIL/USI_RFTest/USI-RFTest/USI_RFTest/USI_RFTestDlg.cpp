// USI_RFTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "USI_RFTest.h"
#include "USI_RFTestDlg.h"
#include "FormView_Wireless.h"
#include "FormView_Bluetooth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUSI_RFTestDlg ��ȭ ����

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


// CUSI_RFTestDlg �޽��� ó����

BOOL CUSI_RFTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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


	// �� ���õ� ID�� �´� Ping view ����
	switch(nID)
	{
	case IDD_FORMVIEW_WIRELESS:
		m_pView = (CView*)RUNTIME_CLASS(CFormView_Wireless)->CreateObject();
		break;
	case IDD_FORMVIEW_BLUETOOTH:
		m_pView = (CView*)RUNTIME_CLASS(CFormView_Bluetooth)->CreateObject();
		break;
	default : // ������ �߸��Ǿ�������, �⺻ View�� �����ش�.
		m_pView = (CView*)RUNTIME_CLASS(CFormView_Wireless)->CreateObject();
		break;
	}


	::ZeroMemory(&cc, sizeof(cc));
	m_pView->Create(NULL, NULL, WS_CHILD, rect, &m_tabMain, nID, &cc);
	m_pView->OnInitialUpdate();
	m_pView->ShowWindow(SW_SHOW);

}


// ���� ���������� ���� ���� ������
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

