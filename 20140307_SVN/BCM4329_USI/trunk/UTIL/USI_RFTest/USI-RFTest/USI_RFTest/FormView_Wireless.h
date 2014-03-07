#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CFormView_Wireless �� ���Դϴ�.

class CFormView_Wireless : public CFormView
{
	DECLARE_DYNCREATE(CFormView_Wireless)

protected:
	CFormView_Wireless();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormView_Wireless();

public:
	enum { IDD = IDD_FORMVIEW_WIRELESS };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedRadioModulation();
	afx_msg void OnBnClickedRadioNonModulation();
private:
	CSpinButtonCtrl m_spinChannel;
	CComboBox m_comboMode;
	SHELLEXECUTEINFO m_sei;
public:
	afx_msg void OnBnClickedButtonWlanTx();
	afx_msg void OnBnClickedButtonWlanRx();
	BOOL SendWlCmd(LPTSTR strCmd);
	afx_msg void OnCbnSelchangeComboMode();
	afx_msg void OnDestroy();
};


