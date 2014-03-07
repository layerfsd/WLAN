#pragma once
#include "afxwin.h"
#include <eap.h>
#include <wzcsapi.h>
#include <BBS_BkgPingStatusCheck.h>
#include <BBS_CommFunc.h>


// CNetSettingPage_Setting_OptionView 폼 뷰입니다.

class CNetSettingPage_Setting_OptionView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Setting_OptionView)

protected:
	CNetSettingPage_Setting_OptionView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CNetSettingPage_Setting_OptionView();

	

public:
	enum { IDD = IDD_NETSETTINGPAGE_SETTING_OPTIONVIEW };
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
	virtual void OnInitialUpdate();



private:

	// ZeroConfig Context 값 저장할 변수
	WZC_CONTEXT m_wzcContext;

	// NetworkToAccess 컨트롤 상태 조회/세팅 하기 위해 필요한 핸들 "SinkWirelessNetUI" 클래스의 Windows 를 저장할 핸들 변수임
	HWND m_hwndSink;
	

public:

	// 하단 메뉴바
	SHMENUBARINFO m_mbi;

	// 콤보박스 컨트롤변수 : NetworkToAccess (사용가능한 네트워크 타입 선택)
	CComboBox m_comboNettoAccess;

	// 백그라운드 핑 상태 체크 옵션 변수
	BBS_BKG_PINGCHECK_OPT m_sBkgPingCheckOpt;


	afx_msg void OnDefault();
	afx_msg void OnApply();
	afx_msg void OnStnClickedStaticTmtp();
	afx_msg void OnStnClickedStaticTmtr();
	afx_msg void OnStnClickedStaticTmtf();
	afx_msg void OnStnClickedStaticTmtc();
	
};


