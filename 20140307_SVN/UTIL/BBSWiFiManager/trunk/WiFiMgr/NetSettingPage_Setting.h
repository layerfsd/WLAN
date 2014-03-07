#pragma once
#include "afxcmn.h"
#include <BBS_CommFunc.h>
#include "NetSettingPage_Setting_OptionView.h"
#include "NetSettingPage_Setting_MMCView.h"
#include "NetSettingPage_Setting_USIView.h"
#include "NetSettingPage_Setting_DefPreferAPView.h"

enum
{
    TAB_PAGE_OPTION = 0,
	TAB_PAGE_MODULE_SETTING,
	TAB_PAGE_DEFAULTAP,
};


enum
{
    ADAPTER_MMC = 1,
	ADAPTER_USI,
};

#define DEFAULT_ADAPTER_NAME_MMC	L"AR6K_SD1"
#define DEFAULT_ADAPTER_NAME_USI	L"BCMSDDHD1"

// CNetSettingPage_Setting 대화 상자입니다.

class CNetSettingPage_Setting : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Setting)

public:
	CNetSettingPage_Setting();
	virtual ~CNetSettingPage_Setting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NETSETTINGPAGE_SETTING };



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	
	virtual BOOL OnInitDialog();

	afx_msg void OnTcnSelchangeTabSettingMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();


	// View 전환 함수
	void ChangeView(INT nId);

	// Setting 페이지 초기화 작업
	void InitializeSettingPage();

private:

	// 메인 탭컨트롤 변수
	CTabCtrl m_tabMain;

	// 화면 해상도 비율 설정 저장을 위한 변수 (VGA or QVGA)
	DWORD m_dwScreenRate;


	// FormView : Open, MMC, USI 등 View를 담을 변수
	CView *m_pView;

	// 현재 모듈 아답터 확인
	DWORD m_dwAdapter;

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	
};
