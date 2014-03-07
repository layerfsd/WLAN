// USI_RFTestDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"

// CUSI_RFTestDlg ��ȭ ����
class CUSI_RFTestDlg : public CDialog
{
// �����Դϴ�.
public:
	CUSI_RFTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_USI_RFTEST_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl m_tabMain;
	DWORD m_dwScreenRate;
	CView *m_pView;
public:
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	void ChangeView(INT nID);

	BOOL GetCurrentFileVersion(LPTSTR tszFileVer);
};
