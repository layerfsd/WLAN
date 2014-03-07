// WlanInfoFileLogDlg.h : header file
//

#if !defined(AFX_WLANINFOFILELOGDLG_H__5FFB48BB_DEEB_4E7B_A1B5_015D5E07CE85__INCLUDED_)
#define AFX_WLANINFOFILELOGDLG_H__5FFB48BB_DEEB_4E7B_A1B5_015D5E07CE85__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CWlanInfoFileLogDlg dialog

class CWlanInfoFileLogDlg : public CDialog
{
// Construction
public:
	CWlanInfoFileLogDlg(CWnd* pParent = NULL);	// standard constructor
	void GetCurrentFileVersion();
// Dialog Data
	//{{AFX_DATA(CWlanInfoFileLogDlg)
	enum { IDD = IDD_WLANINFOFILELOG_DIALOG };
	CEdit	m_EditPingResult;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWlanInfoFileLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CWlanInfoFileLogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLogStart();
	afx_msg void OnButtonLogEnd();
	afx_msg void OnSetfocusEditIpadress();
	afx_msg void OnKillfocusEditIpadress();
	afx_msg void OnChangeEditIpadress();
	afx_msg void OnButtonSipOn();
	afx_msg void OnButtonSipOff();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	// 파일로그 체크 표시 여부
	BOOL m_bIsCheckLogEn;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WLANINFOFILELOGDLG_H__5FFB48BB_DEEB_4E7B_A1B5_015D5E07CE85__INCLUDED_)
