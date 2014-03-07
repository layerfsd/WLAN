#pragma once

#include <wzcuip.h>
#include <resource.h>
#include <networkobject.h>
#include <wpcpriv.h>
#include "appmetc.h"



// CWifiPage ��ȭ �����Դϴ�.

class CWifiPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWifiPage)

public:
	CWifiPage();
	virtual ~CWifiPage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WZC_WLAN_NETWORK };

	PWLAN_INFO	m_psWLanInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
