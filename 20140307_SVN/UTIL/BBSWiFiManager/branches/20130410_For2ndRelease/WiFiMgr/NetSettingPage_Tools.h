#pragma once
#include "afxcmn.h"

#include "NetSettingPage_Tools_PingView.h"
#include "NetSettingPage_Tools_LogView.h"
#include "NetSettingPage_Tools_ScanView.h"


enum
{
    TAB_PAGE_PING = 0,
	TAB_PAGE_SCAN,
	TAB_PAGE_LOG,
	

};

// CNetSettingPage_Tools ��ȭ �����Դϴ�.

class CNetSettingPage_Tools : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Tools)

public:
	CNetSettingPage_Tools();
	virtual ~CNetSettingPage_Tools();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NETSETTINGPAGE_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabToolsMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();


	void ChangeView(INT nId);

private:

	// Tools ����Ʈ�� ����
	CTabCtrl m_tabMain;

	// ȭ�� �ػ� ���� ���� ������ ���� ���� (VGA or QVGA)
	DWORD m_dwScreenRate;

	// FormView : Ping
	CView *m_pViewPing;

	// FormView : Etc
	CView *m_pView;

};
