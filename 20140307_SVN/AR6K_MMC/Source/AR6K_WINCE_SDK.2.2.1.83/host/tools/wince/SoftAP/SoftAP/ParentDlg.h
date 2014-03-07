// ParentDlg.h : header file
//

#include "ParentTab.h"

#pragma once

// CParentDlg dialog
class CParentDlg : public CDialog
{
	CParentTab *m_ptbParent;
	ST_PARENT_ARG m_stParentArg;
	
	void APModeUpdate (BOOL bUpdateConfig, BOOL bUpdateAPMode); 
	void SecurityUpdate ();

	// Construction
public:
	CParentDlg (CWnd* pParent = NULL);	// standard constructor
	virtual ~CParentDlg ();
	
	// Dialog Data
	enum { IDD = IDD_PARENT_DLG };

protected:
	virtual void DoDataExchange (CDataExchange* pDX);	// DDX/DDV support

	// Implementation
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClicked_bnCommitMain ();


#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP ()
};
