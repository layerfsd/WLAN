// PidionSupplicantDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PidionSupplicant.h"
#include "PidionSupplicantDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPidionSupplicantDlg dialog

CPidionSupplicantDlg::CPidionSupplicantDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPidionSupplicantDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPidionSupplicantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_ctrlMainTab);
}

BEGIN_MESSAGE_MAP(CPidionSupplicantDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CPidionSupplicantDlg message handlers

BOOL CPidionSupplicantDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// 변수 초기화
	m_dwFirstClickTick = 0;
	
	// 전체화면

	SHINITDLGINFO sid;
	SHMENUBARINFO g_mbi;

	ZeroMemory( &sid, sizeof( sid ) );
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLG;
	sid.hDlg    = m_hWnd;


	// Initialize the dialog
	::SHInitDialog( &sid );

	memset(&g_mbi, 0, sizeof(SHMENUBARINFO));
	g_mbi.cbSize = sizeof( SHMENUBARINFO );
	g_mbi.hwndParent = m_hWnd;
	//g_mbi.nToolBarId = IDR_MENUBAR;
	g_mbi.hInstRes = AfxGetInstanceHandle();

	::SHCreateMenuBar( &g_mbi );


	
	// 해상도 정보 얻어오기
	// 1 : VGA, 2:QVGA

	g_dwScreenRate = ( GetSystemMetrics(SM_CXSCREEN) == 480 )? 1 : 2;

	// 비트맵정보 얻어오기
	BITMAP bmpInfo;
	CBitmap bmp;

	bmp.LoadBitmap(IDB_BITMAP_MAINTITLE);
	bmp.GetBitmap(&bmpInfo);

	m_dwMainTitleBmpWidth = bmpInfo.bmWidth;
	m_dwMainTitleBmpHeight = bmpInfo.bmHeight;

	bmp.DeleteObject();



	RECT rc;
	GetClientRect(&rc);
	
	
	// 탭컨트롤 초기화
	m_ctrlMainTab.MoveWindow(rc.left, rc.top + (m_dwMainTitleBmpHeight / g_dwScreenRate), rc.right, rc.bottom - (m_dwMainTitleBmpHeight / g_dwScreenRate), TRUE);
	
	m_ctrlMainTab.InsertItem(0, L"Profile");
	m_ctrlMainTab.InsertItem(1, L"State");
	m_ctrlMainTab.InsertItem(2, L"Ping");
	m_ctrlMainTab.InsertItem(3, L"Setting");
	


	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CPidionSupplicantDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.


	// Tab 추가를 위한 Tab item 생성
    TC_ITEM tabItem;
    ZeroMemory(&tabItem, sizeof(TC_ITEM));

	 // tab에 text만 입력
    tabItem.mask    = TCIF_TEXT;


    int nTabCount   = 0;

    if (g_dwAdminMode == PidSup_AdminMode_User)
    {
        nTabCount   = PIDSUP_USERMODE_TABCNT;
    }
    else
    {
        nTabCount   = PIDSUP_FULL_TABCNT;
    }

    if (m_ctrlMainTab.GetItemCount() > 0)
    {
        m_ctrlMainTab.DeleteAllItems();
    }
	
    for (int i = 0; i < nTabCount; i++)
    {
        tabItem.pszText = PIDSUP_TABTITLE[i];
        m_ctrlMainTab.InsertItem(i, &tabItem);
    }
    m_ctrlMainTab.SetCurSel(PidSup_Tab_ProfilePage);

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc); 

	CBitmap bmp;
	CBitmap *pOldbmp; 

	bmp.LoadBitmap(IDB_BITMAP_MAINTITLE); 

	pOldbmp=MemDC.SelectObject(&bmp); 

	// Main Title
	dc.StretchBlt(0, 0, (m_dwMainTitleBmpWidth / g_dwScreenRate), (m_dwMainTitleBmpHeight / g_dwScreenRate), &MemDC, 0, 0, m_dwMainTitleBmpWidth, m_dwMainTitleBmpHeight, SRCCOPY);

	bmp.DeleteObject();

	MemDC.SelectObject(pOldbmp); 
	
	pOldbmp->DeleteObject();


}

void CPidionSupplicantDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 0.5초이내에 두번클릭 되었을 때만 관리자 로그인 창 진입 (더블클릭 효과) - 더블클릭 이벤트를 사용할 수 없음
	// 화면중 이 함수가 불리는 곳은 TITLE 비트맵을 클릭했을때 뿐이다. 나머지는 각 컨트롤 변수로 넘어감
	// 13.02.24 Shpark
	DWORD dwSecondTick = GetTickCount();

	if( (dwSecondTick - m_dwFirstClickTick) < 500 )
	{
		// 관리자 창 띄움
		// Create 두번 되는거 방지하기 위해, 동작중인지 상태 체크
		if(!m_AdminDlg.IsActiveAdminPage())
		{
			m_AdminDlg.Create(CPidionAdmin::IDD);
		}
	}

	m_dwFirstClickTick = GetTickCount();

	CDialog::OnLButtonUp(nFlags, point);
}
