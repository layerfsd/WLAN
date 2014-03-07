// PidionAdmin.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PidionSupplicant.h"
#include "PidionAdmin.h"


// CPidionAdmin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPidionAdmin, CDialog)

CPidionAdmin::CPidionAdmin(CWnd* pParent /*=NULL*/)
	: CDialog(CPidionAdmin::IDD, pParent)
	, m_strAdminPassword(_T(""))
	, m_strExistingPassword(_T(""))
	, m_strNewPassword(_T(""))
	, m_strConfirmPassword(_T(""))
{
	m_bIsActive = FALSE;
}

CPidionAdmin::~CPidionAdmin()
{
}

void CPidionAdmin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strAdminPassword);
	DDX_Text(pDX, IDC_EDIT_EXISTING_PW, m_strExistingPassword);
	DDX_Text(pDX, IDC_EDIT_NEW_PW, m_strNewPassword);
	DDX_Text(pDX, IDC_EDIT_NEW_CONFIRM_PW, m_strConfirmPassword);
}


BEGIN_MESSAGE_MAP(CPidionAdmin, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CPidionAdmin::OnBnClickedButtonOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CPidionAdmin::OnBnClickedButtonCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPidionAdmin 메시지 처리기입니다.

BOOL CPidionAdmin::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	

	// 변수 초기화
	m_bIsActive = TRUE;
	m_strAdminTitle = _T("");



	BringWindowToTop();


	MoveWindow(10 / g_dwScreenRate, 100 / g_dwScreenRate, 460 / g_dwScreenRate, 280 / g_dwScreenRate, TRUE);
	
	InitControls();

	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// 컨트롤러 초기화
void CPidionAdmin::InitControls()
{
	// 관리자 모드에 따라 다른 초기화
	if(g_dwAdminMode == PidSup_AdminMode_User)
	{

		m_strAdminTitle = _T("[Enter Admin Password]");

		// textbox
		GetDlgItem(IDC_EDIT_PASSWORD        )->MoveWindow(8 / g_dwScreenRate, 80 / g_dwScreenRate, 440 / g_dwScreenRate, 44 / g_dwScreenRate);
		GetDlgItem(IDC_EDIT_PASSWORD        )->ShowWindow(SW_SHOW);

		// button
		GetDlgItem(IDC_BUTTON_CANCEL		)->MoveWindow(108 / g_dwScreenRate, 140 / g_dwScreenRate, 160 / g_dwScreenRate, 40 / g_dwScreenRate);
		GetDlgItem(IDC_BUTTON_OK			)->MoveWindow(288 / g_dwScreenRate, 140 / g_dwScreenRate, 160 / g_dwScreenRate, 40 / g_dwScreenRate);
	}
	else if( (g_dwAdminMode & PidSup_AdminMode_ManualAdmin) != 0 )
	{
		m_strAdminTitle = _T("[Change Admin Password]");

		// static
		GetDlgItem(IDC_STATIC_EXSTING_PW	)->MoveWindow(8 / g_dwScreenRate, 70 / g_dwScreenRate, 160 / g_dwScreenRate, 40 / g_dwScreenRate);
		GetDlgItem(IDC_STATIC_EXSTING_PW	)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_NEW_PW		)->MoveWindow(8 / g_dwScreenRate, 120 / g_dwScreenRate, 160 / g_dwScreenRate, 40 / g_dwScreenRate);
		GetDlgItem(IDC_STATIC_NEW_PW		)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_NEW_CONFIRM_PW)->MoveWindow(8 / g_dwScreenRate, 170 / g_dwScreenRate, 160 / g_dwScreenRate, 40 / g_dwScreenRate);
		GetDlgItem(IDC_STATIC_NEW_CONFIRM_PW)->ShowWindow(SW_SHOW);


		// textbox
		GetDlgItem(IDC_EDIT_EXISTING_PW     )->MoveWindow(178 / g_dwScreenRate, 68 / g_dwScreenRate, 270 / g_dwScreenRate, 44 / g_dwScreenRate);
		GetDlgItem(IDC_EDIT_EXISTING_PW		)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_EDIT_NEW_PW			)->MoveWindow(178 / g_dwScreenRate, 118 / g_dwScreenRate, 270 / g_dwScreenRate, 44 / g_dwScreenRate);
		GetDlgItem(IDC_EDIT_NEW_PW			)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_EDIT_NEW_CONFIRM_PW  )->MoveWindow(178 / g_dwScreenRate, 168 / g_dwScreenRate, 270 / g_dwScreenRate, 44 / g_dwScreenRate);
		GetDlgItem(IDC_EDIT_NEW_CONFIRM_PW	)->ShowWindow(SW_SHOW);


		// button
		GetDlgItem(IDC_BUTTON_CANCEL        )->MoveWindow(108 / g_dwScreenRate, 230 / g_dwScreenRate, 160 / g_dwScreenRate, 40 / g_dwScreenRate);
		GetDlgItem(IDC_BUTTON_OK            )->MoveWindow(288 / g_dwScreenRate, 230 / g_dwScreenRate, 160 / g_dwScreenRate, 40 / g_dwScreenRate);
	}

}

void CPidionAdmin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	TCHAR tszFileVer[32] = {'\0',};
	


    HDC hdc = ::GetDC(m_hWnd);


    // Text font 설정
    LOGFONTW logFont;

    logFont.lfHeight         = 30 / g_dwScreenRate;         // nHeight
    logFont.lfWidth          = 10 / g_dwScreenRate;          // nWidth
    logFont.lfEscapement     = 0;                           // nEscapement
    logFont.lfOrientation    = 0;                           // nOrientation
    logFont.lfWeight         = FW_BOLD;                     // nWeight
    logFont.lfItalic         = FALSE;                       // bItalic
    logFont.lfUnderline      = FALSE;                       // bUnderline
    logFont.lfStrikeOut      = 0;                           // cStrikeOut
    logFont.lfCharSet        = ANSI_CHARSET;                // nCharSet
    logFont.lfOutPrecision   = OUT_DEFAULT_PRECIS;          // nOutPrecision
    logFont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;         // nClipPrecision
    logFont.lfQuality        = DEFAULT_QUALITY;             // nQuality
    logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;    // nPitchAndFamily
    //logFont.lfFaceName       = L"Tahoma";                 // lpszFacename
    lstrcpyW(logFont.lfFaceName, L"Tahoma");

    // font handler 생성
    HFONT  hFont    = ::CreateFontIndirect(&logFont);
    HFONT  hOldFont = (HFONT)::SelectObject(hdc, hFont);

    // 테두리 출력 
    HPEN   hPen     = ::CreatePen(PS_SOLID, 4 / g_dwScreenRate, RGB(0, 0, 0));
    HPEN   hOldPen  = (HPEN)::SelectObject(hdc, hPen);

    // 배경 색 설정
    COLORREF bkColor = RGB(200, 200, 200);
    // 문자 색 설정
    COLORREF textColor = RGB(100, 150, 255);

    // dialog 테두리 설정
    HBRUSH hBrush   = ::CreateSolidBrush(bkColor);
    ::Rectangle(hdc, 2 / g_dwScreenRate, 2 / g_dwScreenRate, 458 / g_dwScreenRate, 278 / g_dwScreenRate);

    // Fill rectangle 크기 설정
    RECT rect;
    rect.left   = 4 / g_dwScreenRate;
    rect.top    = 4 / g_dwScreenRate;
    rect.right  = 456 / g_dwScreenRate;
    rect.bottom = 276 / g_dwScreenRate;

    ::FillRect(hdc, &rect, hBrush);

    // Title 색상
    ::SetTextColor(hdc, textColor);
    ::SetBkColor(hdc, bkColor);


    // Title 출력
    RECT textRect;
    textRect.left   = 10 / g_dwScreenRate;
    textRect.right  = 250 / g_dwScreenRate;
    textRect.top    = 0 / g_dwScreenRate;
    textRect.bottom = 50 / g_dwScreenRate;

	::DrawText(hdc, m_strAdminTitle, -1, &textRect, DT_VCENTER | DT_LEFT);

    // Version 출력
    textRect.left   = 250 / g_dwScreenRate;
    textRect.right  = 440 / g_dwScreenRate;
    textRect.top    = 0 / g_dwScreenRate;
    textRect.bottom = 50 / g_dwScreenRate;

	if( GetCurrentFileVersion(tszFileVer) )
	{
		::DrawText(hdc, tszFileVer, -1, &textRect, DT_VCENTER | DT_RIGHT);
	}

    // 기존 폰트로 변경
    ::SelectObject(hdc, hOldFont);

    // 기존 pen으로 변경
    ::SelectObject(hdc, hOldPen);

    // font pen 자원 해제
    CloseHandle(hBrush);
    CloseHandle(hPen);
    CloseHandle(hOldPen);

    CloseHandle(hFont);
    CloseHandle(hOldFont);

    // DC 자원 해제
    ::ReleaseDC(m_hWnd, hdc);

}

void CPidionAdmin::OnBnClickedButtonCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClearEditBox();
	
	// sip 숨김
	SipShowIM(SIPF_OFF);

	DestroyWindow();
}

void CPidionAdmin::OnBnClickedButtonOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	// 대,소문자 관계없이 입력받기 위해 추가
	// PIDION or pidion
	m_strAdminPassword.MakeUpper();
	m_strExistingPassword.MakeUpper();

	// 관리자 모드에 따라 다른 동작
	if(g_dwAdminMode == PidSup_AdminMode_User)
	{

		CString strAdminPW  = L"";
		CString strAsciiPW  = L"";


		if (LoadPassword(strAsciiPW) == FALSE)
		{
			strAsciiPW = AdminPW2Ascii(PIDSUP_DEFAULT_PASSWORD);
			SavePassword(strAsciiPW);
		}

		strAdminPW = AdminPW2Ascii(m_strAdminPassword.Trim());

		if (strAdminPW.Compare(strAsciiPW) == 0)
		{
			g_dwAdminMode = PidSup_AdminMode_ManualAdmin;


			ClearEditBox();

			// sip 숨김
			SipShowIM(SIPF_OFF);

			//CPidionSupplicantDlg* pParent = (CPidionSupplicantDlg*)GetParent();
			//pParent->Invalidate();

			DestroyWindow();
		}
		else
		{
			m_strAdminPassword = L"";
			UpdateData(FALSE);
			MessageBox(L"Failed Admin Login", L"[ Pidion Supplicant ]");
		}

	}
	else if( (g_dwAdminMode & PidSup_AdminMode_ManualAdmin) != 0 ) // 비밀번호 변경 모드
	{

		CString strAdminPW  = L"";
		CString strAsciiPW  = L"";

		if (LoadPassword(strAsciiPW) == FALSE)
		{
			strAsciiPW = AdminPW2Ascii(PIDSUP_DEFAULT_PASSWORD);
			SavePassword(strAsciiPW);
		}

		strAdminPW = AdminPW2Ascii(m_strExistingPassword.Trim());

		m_strNewPassword        = m_strNewPassword.Trim();
		m_strConfirmPassword    = m_strConfirmPassword.Trim();


		if (strAdminPW.Compare(strAsciiPW) == 0)
		{
			// 기존 패스워드와 일치할 경우 계속 진행
			if (m_strNewPassword.Compare(m_strConfirmPassword) == 0 && m_strNewPassword.GetLength() != 0)
			{
				// 변경된 Password 입력
				strAsciiPW = AdminPW2Ascii(m_strNewPassword);
				SavePassword(strAsciiPW);


				ClearEditBox();

				// sip 숨김
				SipShowIM(SIPF_OFF);

				DestroyWindow();
			}
			else
			{
				// 새로 입력하는 Password와 확인 Password가 틀릴 경우
				m_strNewPassword      = L"";
				m_strConfirmPassword  = L"";
				UpdateData(FALSE);

				MessageBox(L"Check new password!!", L"[ Pidion Supplicant ]");
			}
		}
		else
		{
			ClearEditBox();
			// 기존 패스워드와 일치하지 않을 경우 경고창 출력
			MessageBox(L"Wrong Password!!", L"[ Pidion Supplicant ]");
		}
	}

}

// 파일에서 Password를 읽어온다.
BOOL CPidionAdmin::LoadPassword(CString &strPassword)
{
	BOOL    bResult     = FALSE;
    HANDLE  hFile       = NULL;
	DWORD   dwBytes     = 0;
	int     len         = 0;
    WCHAR   szBuf[2048] = {0,};

    hFile = CreateFile(PIDSUP_PASSWORD_PATH , GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hFile != NULL)
    {
	    SetFilePointer(hFile , 0, NULL, FILE_BEGIN);
        ReadFile(hFile, szBuf, 2048, &dwBytes, 0);

        if (dwBytes > 0)
        {
            //wcscpy((wchar_t*)strPassword, szBuf);
            //bResult = TRUE;
            strPassword = L"";
            for (DWORD i = 0; i < dwBytes / 2; i++)
            {
                strPassword.Format(L"%s%c", strPassword, szBuf[i]);
            }
            
            bResult = TRUE;
        }

	    CloseHandle(hFile);
    }

    return bResult;
}

// 변경된 Password을 저장한다.
void CPidionAdmin::SavePassword(CString strPassword)
{
    HANDLE  hFile       = NULL;
	DWORD   dwBytes     = 0;
	int     len         = 0;
    WCHAR   szBuf[2048] = {0,};

    len = wcslen(strPassword);

    int nPos = 0;

    for (nPos = 0; nPos < len; nPos++)
    {
        szBuf[nPos] = (WCHAR)strPassword[nPos];
    }
    szBuf[nPos] = L'\0';

    DeleteFile(PIDSUP_PASSWORD_PATH);


    hFile = CreateFile(PIDSUP_PASSWORD_PATH , GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
    SetFilePointer(hFile , 0, NULL, FILE_BEGIN);

    len = wcslen(szBuf) * 2;
    WriteFile(hFile , szBuf, len, &dwBytes, 0);

	CloseHandle(hFile);
}



// ADMIN PASSWORD to Ascii string
CString CPidionAdmin::AdminPW2Ascii(CString strAdmin)
{
    int nAdminLen = wcslen(strAdmin);

    // 반환할 Ascii 문자
    CString strAscii = L"";

    // 관리자 암호 1자 -> Ascii 2자 (ex_'0' -> '30');
    for (int i = 0; i < nAdminLen; i++)
    {
        strAscii.Format(L"%s%02X", strAscii, static_cast<UINT>(strAdmin[i]/*strAdmin.GetAt(i)*/));
    }

    return strAscii;
}


// 현재 관리자 페이지가 동작중인지 확인
BOOL CPidionAdmin::IsActiveAdminPage(void)
{
	return m_bIsActive;
}

void CPidionAdmin::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bIsActive = FALSE;
}




LRESULT CPidionAdmin::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(message == WM_COMMAND)
	{
		if( HIWORD(wParam) == EN_UPDATE )
		{
			switch(LOWORD(wParam))
			{
			case IDC_EDIT_PASSWORD:
				GetOnlyText(IDC_EDIT_PASSWORD, m_strAdminPassword);
				break;

			case IDC_EDIT_EXISTING_PW:
				GetOnlyText(IDC_EDIT_EXISTING_PW, m_strExistingPassword);
				break;

			case IDC_EDIT_NEW_PW:
				GetOnlyText(IDC_EDIT_NEW_PW, m_strNewPassword);
				break;

			case IDC_EDIT_NEW_CONFIRM_PW:
				GetOnlyText(IDC_EDIT_NEW_CONFIRM_PW, m_strConfirmPassword);
				break;
			}
		}
	}
	else if(message == WM_CTLCOLORSTATIC)
	{
		HDC hdcStatic = (HDC) wParam;
        SetBkMode(hdcStatic, TRANSPARENT);
        SetTextColor(hdcStatic, RGB(0,0,0));
        return (INT_PTR)CreateSolidBrush(RGB(200,200,200)); 
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}


// 영문과 숫자만 입력받도록 한다.
void CPidionAdmin::GetOnlyText(int nID, CString &strMessage)
{
	UpdateData(TRUE);

    int nStart  = 0;
    int nEnd    = 0;

    // 현재 carot의 위치 확인
    ((CEdit*)(GetDlgItem(nID)))->GetSel(nStart, nEnd);

    // 여러 블럭이 선택된 것이면 종료
    if (nStart != nEnd)
    {
        return;
    }

    if (nEnd > 0)
    {
        WCHAR ch = strMessage.GetAt(nEnd - 1);

        if ((ch >= 'a' && ch <= 'z') ||     // 소문자
            (ch >= 'A' && ch <= 'Z') ||     // 대문자
            (ch >= '0' && ch <= '9'))       // 숫자
        {
            // 아무 동작도 취하지 않는다.
        }
        else
        {
            // 해당 character 삭제
            strMessage.Remove(ch);
            nEnd--;
        }
    }

    UpdateData(FALSE);

    // 기존 carot의 위치 유지
    ((CEdit*)(GetDlgItem(nID)))->SetSel(nEnd, nEnd);

}


// 에디트 박스 초기화
void CPidionAdmin::ClearEditBox(void)
{
	UpdateData(TRUE);
	m_strAdminPassword		= L"";
	m_strExistingPassword	= L"";
    m_strNewPassword		= L"";
    m_strConfirmPassword	= L"";
    UpdateData(FALSE);
}
