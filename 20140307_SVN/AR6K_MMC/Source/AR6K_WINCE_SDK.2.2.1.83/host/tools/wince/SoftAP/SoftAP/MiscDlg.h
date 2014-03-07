#pragma once

#include "Config.h"

// CMiscDlg dialog

class CMiscDlg : public CDialog
{
    DECLARE_DYNAMIC (CMiscDlg)

    CMiscDlg (CWnd* pParent = NULL);   // standard constructor
    CConfig *m_pConfig;

    // Generated message map functions
    virtual BOOL OnInitDialog ();
    afx_msg void OnBnClicked_chCountry ();
    afx_msg void OnSetFocus (CWnd* pOldWnd);
    afx_msg void OnEnChange_txtMaxStations();
    afx_msg void OnBnClicked_bnCountryCode();
    afx_msg void OnBnClicked_bnMaxStation();
    afx_msg void OnBnClicked_bnAddMac();
    afx_msg void OnBnClicked_bnDelMac();
    afx_msg void OnBnClicked_bnRefresh();
	afx_msg void OnBnClicked_bnAclpolicy();

    void EnableControls (BOOL bEnable);
    void RefreshMacFilter (BOOL bRetrive);

protected:
    virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP ()

public:
    // Dialog Data
    enum { IDD = IDD_MISC_DLG };

    CMiscDlg (CConfig *pConfig);
    virtual ~CMiscDlg ();
};
