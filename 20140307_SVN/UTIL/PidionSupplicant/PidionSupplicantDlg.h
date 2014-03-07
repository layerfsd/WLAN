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
	// 메인 탭 컨트롤 변수
	CTabCtrl m_ctrlMainTab;

	// 타이틀 비트맵 크기 저장변수
	DWORD m_dwMainTitleBmpWidth;
	DWORD m_dwMainTitleBmpHeight;

	// 더블클릭 확인
	DWORD m_dwFirstClickTick;
	

	// Dialog
	CPidionAdmin m_AdminDlg;
	
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
