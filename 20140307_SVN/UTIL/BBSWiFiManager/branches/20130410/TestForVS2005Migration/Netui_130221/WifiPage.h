#pragma once

#include <wzcuip.h>
#include <resource.h>
#include <networkobject.h>
#include <wpcpriv.h>
#include "appmetc.h"



// CWifiPage 대화 상자입니다.

class CWifiPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWifiPage)

public:
	CWifiPage();
	virtual ~CWifiPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WZC_WLAN_NETWORK };

	PWLAN_INFO	m_psWLanInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnDestroy();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HRESULT CWifiPage::OnActivateHome(WPARAM wParam, LPARAM lParam);

	HRESULT RefreshNetListView();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
};
