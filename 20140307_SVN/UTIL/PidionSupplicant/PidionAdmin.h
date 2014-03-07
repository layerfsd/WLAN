#pragma once


// CPidionAdmin ��ȭ �����Դϴ�.

class CPidionAdmin : public CDialog
{
	DECLARE_DYNAMIC(CPidionAdmin)

public:
	CPidionAdmin(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPidionAdmin();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PIDIONADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnDestroy();


	// TITILE ǥ�õ� Name
	CString m_strAdminTitle;

	// ��Ʈ�ѷ� �ʱ�ȭ
	void InitControls();

	// ���Ͽ��� Password�� �о�´�.
	BOOL LoadPassword(CString &strPassword);

	// ����� Password ����
	void SavePassword(CString strPassword);

	// ADMIN PASSWORD to Ascii string
	CString AdminPW2Ascii(CString strAdminPW);
	
	// ���� ������ �������� ���������� Ȯ��
	BOOL m_bIsActive;

	BOOL IsActiveAdminPage(void);

	// Admin Password
	CString m_strAdminPassword;

	// ������ ���ڸ� �Է¹޵��� �Ѵ�.
	void GetOnlyText(int nID, CString &strPassword);

	// ���� �н�����
	CString m_strExistingPassword;

	// ���ο� �н�����1
	CString m_strNewPassword;

	// ���ο� �н�����2
	CString m_strConfirmPassword;

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	// ����Ʈ �ڽ� �ʱ�ȭ
	void ClearEditBox(void);
};
