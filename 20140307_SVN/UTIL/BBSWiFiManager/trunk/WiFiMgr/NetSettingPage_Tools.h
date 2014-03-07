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

// CNetSettingPage_Tools 대화 상자입니다.

class CNetSettingPage_Tools : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Tools)

public:
	CNetSettingPage_Tools();
	virtual ~CNetSettingPage_Tools();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NETSETTINGPAGE_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabToolsMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();


	void ChangeView(INT nId);

private:

	// Tools 탭컨트롤 변수
	CTabCtrl m_tabMain;

	// 화면 해상도 비율 설정 저장을 위한 변수 (VGA or QVGA)
	DWORD m_dwScreenRate;

	// FormView : Ping
	CView *m_pViewPing;

	// FormView : Etc
	CView *m_pView;

};
