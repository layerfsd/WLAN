#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CFormView_Bluetooth �� ���Դϴ�.

class CFormView_Bluetooth : public CFormView
{
	DECLARE_DYNCREATE(CFormView_Bluetooth)

protected:
	CFormView_Bluetooth();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormView_Bluetooth();

public:
	enum { IDD = IDD_FORMVIEW_BLUETOOTH };
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
	afx_msg void OnEnChangeEditChannel();

private:
	CSpinButtonCtrl m_spinChannel;
	CComboBox m_comboMode;
	CComboBox m_comboType;
	HMODULE m_hBthPowerModule;
	BOOL m_bHopping;
	HANDLE m_hHoppingThread;
	
public:
	afx_msg void OnBnClickedButtonTx();
	afx_msg void OnBnClickedButtonRx();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonHopping();
	afx_msg void OnDestroy();
	static DWORD HoppingThreadHandler(CFormView_Bluetooth* hMain);
	void HoppingThread(void);
	BOOL SendBTCommand(LPCTSTR str);
};


