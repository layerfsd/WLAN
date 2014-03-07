#pragma once
#include <resource.h>
#include <wzcuip.h>
#include <networkObject.h>
#include <wpcpriv.h>
#include "afxcmn.h"
#include "appmetc.h"

// CTestPropPage 대화 상자입니다.

typedef struct _FINDGUID
{
    LPTSTR pszGUID;
    UINT   cchGUID;
    UINT   uIndex;
    UINT   uDesired;
} FINDGUID;

class CTestPropPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTestPropPage)

public:
	CTestPropPage();
	virtual ~CTestPropPage();


	INT CalculateStatusColumnWidth();
	HRESULT InitFilterCombo();
	HRESULT PopulateNetworkList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WZC_WLAN_NETWORK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	afx_msg HRESULT OnActivateHome(WPARAM wParam, LPARAM lParam);
	HRESULT OnConnect();
	HRESULT OnNetworkProperties();
	HRESULT OnRemoveSetting();
	HRESULT OnAddNewSetting();
	HRESULT OnChangePowerState();
	LPARAM GetListItemLPARAM(INT nIndex);
	HRESULT ApplyProperties(const WLAN_CFG_PROPS *psProps);
	HRESULT RefreshNetListView();
	void DeleteAllItems();
	void RelayoutDlg();


private:
	WLAN_INFO m_sWLanInfo;
	BOOL m_bDefaultInitDone;
public:
	// // SysListView32 리스트 컨트롤 변수
	CListCtrl m_hCtrlNetList;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPaint();
};
