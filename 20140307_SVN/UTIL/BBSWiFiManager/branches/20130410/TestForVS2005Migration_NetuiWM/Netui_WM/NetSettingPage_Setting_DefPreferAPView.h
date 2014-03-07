#pragma once
#include "afxwin.h"



// CNetSettingPage_Setting_DefPreferAPView �� ���Դϴ�.

class CNetSettingPage_Setting_DefPreferAPView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Setting_DefPreferAPView)

protected:
	CNetSettingPage_Setting_DefPreferAPView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CNetSettingPage_Setting_DefPreferAPView();

public:
	enum { IDD = IDD_NETSETTINGPAGE_SETTING_DEFPREFERAPVIEW };
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
	afx_msg void OnBnClickedCheckForceAddtolist();
	// Enable Force add to Preferred List
	BOOL m_bEnable;
	// ������ üũ�� ��� ��Ʈ�� Disable ��Ű�� �Լ�
	BOOL EnableAllControls(BOOL bEnable);
private:
	// DHCP ���üũ
	BOOL m_bDHCP;

	// XML ���� ����/�ε�
	BBS_DEF_PREF_NETWORK_INFO m_defPrefNetInfo;
public:
	afx_msg void OnBnClickedCheckDhcp();
	afx_msg void OnCbnSelendokComboAuth();
	virtual void OnInitialUpdate();
	// �޺��ڽ� ��Ʈ�� ���� : �������
	CComboBox m_comboAuth;
	// �޺��ڽ� ��Ʈ�� ���� : ��ȣȭ���
	CComboBox m_comboEncr;
	afx_msg void OnCbnSelendokComboEncr();
private:
	CComboBox m_comboKeyIndex;
public:
	afx_msg void OnApply();
	afx_msg void OnDefault();
private:
	BOOL m_bHex;
};


