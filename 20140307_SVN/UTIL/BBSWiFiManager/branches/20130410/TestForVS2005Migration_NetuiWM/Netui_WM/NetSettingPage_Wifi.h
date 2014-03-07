#pragma once
#include <resource.h>
#include <wzcuip.h>
#include <networkObject.h>
#include <wpcpriv.h>
#include "afxcmn.h"
#include "appmetc.h"
#include "afxwin.h"


typedef struct _FINDGUID
{
    LPTSTR pszGUID;
    UINT   cchGUID;
    UINT   uIndex;
    UINT   uDesired;
} FINDGUID;


// CNetSettingPage_Wifi 대화 상자입니다.

class CNetSettingPage_Wifi : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Wifi)

public:
	CNetSettingPage_Wifi();
	virtual ~CNetSettingPage_Wifi();


	void CalculateColumnWidth(INT *nSSIDWidth, INT *nStatusWidth, INT *nAuthWidth, INT *nEncrWidth);

	HRESULT PopulateNetworkList();
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
	BOOL OnNetworkContextMenu(POINT pt, INT iItem);

	CListCtrl m_hCtrlNetList;
	WLAN_INFO m_sWLanInfo;
	BOOL m_bDefaultInitDone;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WZC_WLAN_NETWORK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HRESULT OnActivateHome(WPARAM wParam, LPARAM lParam);

	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnBnClickedButtonRescan();
};
