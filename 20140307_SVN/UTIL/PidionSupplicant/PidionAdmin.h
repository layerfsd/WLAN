#pragma once


// CPidionAdmin 대화 상자입니다.

class CPidionAdmin : public CDialog
{
	DECLARE_DYNAMIC(CPidionAdmin)

public:
	CPidionAdmin(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPidionAdmin();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PIDIONADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnDestroy();


	// TITILE 표시될 Name
	CString m_strAdminTitle;

	// 컨트롤러 초기화
	void InitControls();

	// 파일에서 Password를 읽어온다.
	BOOL LoadPassword(CString &strPassword);

	// 변경된 Password 저장
	void SavePassword(CString strPassword);

	// ADMIN PASSWORD to Ascii string
	CString AdminPW2Ascii(CString strAdminPW);
	
	// 현재 관리자 페이지가 동작중인지 확인
	BOOL m_bIsActive;

	BOOL IsActiveAdminPage(void);

	// Admin Password
	CString m_strAdminPassword;

	// 영문과 숫자만 입력받도록 한다.
	void GetOnlyText(int nID, CString &strPassword);

	// 기존 패스워드
	CString m_strExistingPassword;

	// 새로운 패스워드1
	CString m_strNewPassword;

	// 새로운 패스워드2
	CString m_strConfirmPassword;

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	// 에디트 박스 초기화
	void ClearEditBox(void);
};
