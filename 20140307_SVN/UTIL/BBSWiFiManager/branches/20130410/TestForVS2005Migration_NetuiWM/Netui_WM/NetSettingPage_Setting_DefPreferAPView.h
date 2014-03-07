#pragma once
#include "afxwin.h"



// CNetSettingPage_Setting_DefPreferAPView 폼 뷰입니다.

class CNetSettingPage_Setting_DefPreferAPView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Setting_DefPreferAPView)

protected:
	CNetSettingPage_Setting_DefPreferAPView();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckForceAddtolist();
	// Enable Force add to Preferred List
	BOOL m_bEnable;
	// 사용안함 체크시 모든 컨트롤 Disable 시키는 함수
	BOOL EnableAllControls(BOOL bEnable);
private:
	// DHCP 사용체크
	BOOL m_bDHCP;

	// XML 설정 저장/로드
	BBS_DEF_PREF_NETWORK_INFO m_defPrefNetInfo;
public:
	afx_msg void OnBnClickedCheckDhcp();
	afx_msg void OnCbnSelendokComboAuth();
	virtual void OnInitialUpdate();
	// 콤보박스 컨트롤 변수 : 인증방식
	CComboBox m_comboAuth;
	// 콤보박스 컨트롤 변수 : 암호화방식
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


