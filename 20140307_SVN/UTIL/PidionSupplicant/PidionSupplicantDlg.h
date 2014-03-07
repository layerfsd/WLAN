// PidionSupplicantDlg.h : header file
//

#pragma once
#include "afxcmn.h"

#include "PidionAdmin.h"

// CPidionSupplicantDlg dialog
class CPidionSupplicantDlg : public CDialog
{
// Construction
public:
	CPidionSupplicantDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PIDIONSUPPLICANT_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	// ���� �� ��Ʈ�� ����
	CTabCtrl m_ctrlMainTab;

	// Ÿ��Ʋ ��Ʈ�� ũ�� ���庯��
	DWORD m_dwMainTitleBmpWidth;
	DWORD m_dwMainTitleBmpHeight;

	// ����Ŭ�� Ȯ��
	DWORD m_dwFirstClickTick;
	

	// Dialog
	CPidionAdmin m_AdminDlg;
	
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
