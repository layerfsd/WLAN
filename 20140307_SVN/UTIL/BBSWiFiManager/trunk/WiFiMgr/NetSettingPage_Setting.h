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

// CNetSettingPage_Setting ��ȭ �����Դϴ�.

class CNetSettingPage_Setting : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Setting)

public:
	CNetSettingPage_Setting();
	virtual ~CNetSettingPage_Setting();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NETSETTINGPAGE_SETTING };



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	
	virtual BOOL OnInitDialog();

	afx_msg void OnTcnSelchangeTabSettingMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();


	// View ��ȯ �Լ�
	void ChangeView(INT nId);

	// Setting ������ �ʱ�ȭ �۾�
	void InitializeSettingPage();

private:

	// ���� ����Ʈ�� ����
	CTabCtrl m_tabMain;

	// ȭ�� �ػ� ���� ���� ������ ���� ���� (VGA or QVGA)
	DWORD m_dwScreenRate;


	// FormView : Open, MMC, USI �� View�� ���� ����
	CView *m_pView;

	// ���� ��� �ƴ��� Ȯ��
	DWORD m_dwAdapter;

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	
};
