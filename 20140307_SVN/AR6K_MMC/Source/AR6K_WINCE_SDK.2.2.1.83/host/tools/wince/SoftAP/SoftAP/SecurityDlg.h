#pragma once

#include "Config.h"

typedef struct _ST_SECURITY_ARG{
	BOOL bSoftAPMode;
	TCHAR szKey [MAX_PATH];
	int nAuthMode;
	int nCipher;
	int nKeyIndex;
	BOOL bInputOk;
	BOOL bUpdate;
	BOOL bKeyUpdate;
	int *pnOpModeCurSel;
}ST_SECURITY_ARG;

// CSecurityDlg dialog

class CSecurityDlg : public CDialog
{
	CSecurityDlg (CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNAMIC (CSecurityDlg)

	CConfig *m_pConfig;

	// Generated message map functions
	virtual BOOL OnInitDialog ();
	afx_msg void OnCbnSelchange_cbAuth ();
	afx_msg void OnCbnSelchange_cbCipher ();
	afx_msg void OnSetFocus (CWnd* pOldWnd);
	afx_msg void OnCbnSelchange_cbKeyIndex ();
	afx_msg void OnEnChange_txtKey();

	void EnableControls (BOOL bEnable);
	void UpdateControls ();

protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP ()

public:
	ST_SECURITY_ARG *m_pstSecurityArg;

	// Dialog Data
	enum { IDD = IDD_SECURITY_DLG };

	CSecurityDlg (CConfig *pConfig, ST_SECURITY_ARG *pstSecurityArg);
	virtual ~CSecurityDlg ();
	void GetArguments ();
};
