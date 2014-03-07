#pragma once

#include "Config.h"

class CGeneralDlg;

typedef struct _ST_GENERAL_ARG{
    BOOL bSoftAPMode;
    TCHAR szSSID [MAX_PATH];
    TCHAR szChannel [MAX_PATH];
    TCHAR szDTIM [MAX_PATH];
    TCHAR szPublic [MAX_PATH];
    TCHAR szPrivate [MAX_PATH];
    BOOL bHiddenSSID;
    BOOL bProtection;
	BOOL bIntraBss;
    USHORT nBeaconInterval;
    BOOL bInputOk;
    BOOL bUpdate;
    int nOpModeCurSel;
}ST_GENERAL_ARG;

// CGeneralDlg dialog

class CGeneralDlg : public CDialog
{
    DECLARE_DYNAMIC (CGeneralDlg)
    CGeneralDlg (CWnd* pParent = NULL);   // standard constructor

    CConfig *m_pConfig;

    // Generated message map functions
    virtual BOOL OnInitDialog ();
    afx_msg void OnCbnSelchange_cbOpMode ();
    afx_msg void OnEnChange_txtSSID();
    afx_msg void OnEnChange_txtChannel();
    afx_msg void OnBnClicked_chHideSSID();
    afx_msg void OnBnClicked_chBGMode();
	afx_msg void OnBnClicked_chintrabss();
    afx_msg void OnEnChange_txtDTIM();
    afx_msg void OnCbnSelchange_cbPublicList();
    afx_msg void OnEnChange_txtBeaconInterval();

    void EnableControls (BOOL bEnable);
	void AddPublicInterface();

protected:
    virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP ()

public:
    ST_GENERAL_ARG *m_pstGeneralArg;

    // Dialog Data
    enum { IDD = IDD_GENERAL_DLG };

    CGeneralDlg (CConfig *pConfig, ST_GENERAL_ARG *pstGeneralArg);
    virtual ~CGeneralDlg ();
    void GetArguments ();
};
