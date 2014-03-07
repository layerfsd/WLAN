// PidionAdmin.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PidionSupplicant.h"
#include "PidionAdmin.h"


// CPidionAdmin ��ȭ �����Դϴ�.

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


// CPidionAdmin �޽��� ó�����Դϴ�.

BOOL CPidionAdmin::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	

	// ���� �ʱ�ȭ
	m_bIsActive = TRUE;
	m_strAdminTitle = _T("");



	BringWindowToTop();


	MoveWindow(10 / g_dwScreenRate, 100 / g_dwScreenRate, 460 / g_dwScreenRate, 280 / g_dwScreenRate, TRUE);
	
	InitControls();

	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

// ��Ʈ�ѷ� �ʱ�ȭ
void CPidionAdmin::InitControls()
{
	// ������ ��忡 ���� �ٸ� �ʱ�ȭ
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.

	TCHAR tszFileVer[32] = {'\0',};
	


    HDC hdc = ::GetDC(m_hWnd);


    // Text font ����
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

    // font handler ����
    HFONT  hFont    = ::CreateFontIndirect(&logFont);
    HFONT  hOldFont = (HFONT)::SelectObject(hdc, hFont);

    // �׵θ� ��� 
    HPEN   hPen     = ::CreatePen(PS_SOLID, 4 / g_dwScreenRate, RGB(0, 0, 0));
    HPEN   hOldPen  = (HPEN)::SelectObject(hdc, hPen);

    // ��� �� ����
    COLORREF bkColor = RGB(200, 200, 200);
    // ���� �� ����
    COLORREF textColor = RGB(100, 150, 255);

    // dialog �׵θ� ����
    HBRUSH hBrush   = ::CreateSolidBrush(bkColor);
    ::Rectangle(hdc, 2 / g_dwScreenRate, 2 / g_dwScreenRate, 458 / g_dwScreenRate, 278 / g_dwScreenRate);

    // Fill rectangle ũ�� ����
    RECT rect;
    rect.left   = 4 / g_dwScreenRate;
    rect.top    = 4 / g_dwScreenRate;
    rect.right  = 456 / g_dwScreenRate;
    rect.bottom = 276 / g_dwScreenRate;

    ::FillRect(hdc, &rect, hBrush);

    // Title ����
    ::SetTextColor(hdc, textColor);
    ::SetBkColor(hdc, bkColor);


    // Title ���
    RECT textRect;
    textRect.left   = 10 / g_dwScreenRate;
    textRect.right  = 250 / g_dwScreenRate;
    textRect.top    = 0 / g_dwScreenRate;
    textRect.bottom = 50 / g_dwScreenRate;

	::DrawText(hdc, m_strAdminTitle, -1, &textRect, DT_VCENTER | DT_LEFT);

    // Version ���
    textRect.left   = 250 / g_dwScreenRate;
    textRect.right  = 440 / g_dwScreenRate;
    textRect.top    = 0 / g_dwScreenRate;
    textRect.bottom = 50 / g_dwScreenRate;

	if( GetCurrentFileVersion(tszFileVer) )
	{
		::DrawText(hdc, tszFileVer, -1, &textRect, DT_VCENTER | DT_RIGHT);
	}

    // ���� ��Ʈ�� ����
    ::SelectObject(hdc, hOldFont);

    // ���� pen���� ����
    ::SelectObject(hdc, hOldPen);

    // font pen �ڿ� ����
    CloseHandle(hBrush);
    CloseHandle(hPen);
    CloseHandle(hOldPen);

    CloseHandle(hFont);
    CloseHandle(hOldFont);

    // DC �ڿ� ����
    ::ReleaseDC(m_hWnd, hdc);

}

void CPidionAdmin::OnBnClickedButtonCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClearEditBox();
	
	// sip ����
	SipShowIM(SIPF_OFF);

	DestroyWindow();
}

void CPidionAdmin::OnBnClickedButtonOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);
	// ��,�ҹ��� ������� �Է¹ޱ� ���� �߰�
	// PIDION or pidion
	m_strAdminPassword.MakeUpper();
	m_strExistingPassword.MakeUpper();

	// ������ ��忡 ���� �ٸ� ����
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

			// sip ����
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
	else if( (g_dwAdminMode & PidSup_AdminMode_ManualAdmin) != 0 ) // ��й�ȣ ���� ���
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
			// ���� �н������ ��ġ�� ��� ��� ����
			if (m_strNewPassword.Compare(m_strConfirmPassword) == 0 && m_strNewPassword.GetLength() != 0)
			{
				// ����� Password �Է�
				strAsciiPW = AdminPW2Ascii(m_strNewPassword);
				SavePassword(strAsciiPW);


				ClearEditBox();

				// sip ����
				SipShowIM(SIPF_OFF);

				DestroyWindow();
			}
			else
			{
				// ���� �Է��ϴ� Password�� Ȯ�� Password�� Ʋ�� ���
				m_strNewPassword      = L"";
				m_strConfirmPassword  = L"";
				UpdateData(FALSE);

				MessageBox(L"Check new password!!", L"[ Pidion Supplicant ]");
			}
		}
		else
		{
			ClearEditBox();
			// ���� �н������ ��ġ���� ���� ��� ���â ���
			MessageBox(L"Wrong Password!!", L"[ Pidion Supplicant ]");
		}
	}

}

// ���Ͽ��� Password�� �о�´�.
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

// ����� Password�� �����Ѵ�.
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

    // ��ȯ�� Ascii ����
    CString strAscii = L"";

    // ������ ��ȣ 1�� -> Ascii 2�� (ex_'0' -> '30');
    for (int i = 0; i < nAdminLen; i++)
    {
        strAscii.Format(L"%s%02X", strAscii, static_cast<UINT>(strAdmin[i]/*strAdmin.GetAt(i)*/));
    }

    return strAscii;
}


// ���� ������ �������� ���������� Ȯ��
BOOL CPidionAdmin::IsActiveAdminPage(void)
{
	return m_bIsActive;
}

void CPidionAdmin::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_bIsActive = FALSE;
}




LRESULT CPidionAdmin::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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


// ������ ���ڸ� �Է¹޵��� �Ѵ�.
void CPidionAdmin::GetOnlyText(int nID, CString &strMessage)
{
	UpdateData(TRUE);

    int nStart  = 0;
    int nEnd    = 0;

    // ���� carot�� ��ġ Ȯ��
    ((CEdit*)(GetDlgItem(nID)))->GetSel(nStart, nEnd);

    // ���� ���� ���õ� ���̸� ����
    if (nStart != nEnd)
    {
        return;
    }

    if (nEnd > 0)
    {
        WCHAR ch = strMessage.GetAt(nEnd - 1);

        if ((ch >= 'a' && ch <= 'z') ||     // �ҹ���
            (ch >= 'A' && ch <= 'Z') ||     // �빮��
            (ch >= '0' && ch <= '9'))       // ����
        {
            // �ƹ� ���۵� ������ �ʴ´�.
        }
        else
        {
            // �ش� character ����
            strMessage.Remove(ch);
            nEnd--;
        }
    }

    UpdateData(FALSE);

    // ���� carot�� ��ġ ����
    ((CEdit*)(GetDlgItem(nID)))->SetSel(nEnd, nEnd);

}


// ����Ʈ �ڽ� �ʱ�ȭ
void CPidionAdmin::ClearEditBox(void)
{
	UpdateData(TRUE);
	m_strAdminPassword		= L"";
	m_strExistingPassword	= L"";
    m_strNewPassword		= L"";
    m_strConfirmPassword	= L"";
    UpdateData(FALSE);
}
