#pragma once
#include "afxwin.h"
#include <eap.h>
#include <wzcsapi.h>
#include <BBS_BkgPingStatusCheck.h>
#include <BBS_CommFunc.h>


// CNetSettingPage_Setting_OptionView �� ���Դϴ�.

class CNetSettingPage_Setting_OptionView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Setting_OptionView)

protected:
	CNetSettingPage_Setting_OptionView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	virtual void OnInitialUpdate();



private:

	// ZeroConfig Context �� ������ ����
	WZC_CONTEXT m_wzcContext;

	// NetworkToAccess ��Ʈ�� ���� ��ȸ/���� �ϱ� ���� �ʿ��� �ڵ� "SinkWirelessNetUI" Ŭ������ Windows �� ������ �ڵ� ������
	HWND m_hwndSink;
	

public:

	// �ϴ� �޴���
	SHMENUBARINFO m_mbi;

	// �޺��ڽ� ��Ʈ�Ѻ��� : NetworkToAccess (��밡���� ��Ʈ��ũ Ÿ�� ����)
	CComboBox m_comboNettoAccess;

	// ��׶��� �� ���� üũ �ɼ� ����
	BBS_BKG_PINGCHECK_OPT m_sBkgPingCheckOpt;


	afx_msg void OnDefault();
	afx_msg void OnApply();
	afx_msg void OnStnClickedStaticTmtp();
	afx_msg void OnStnClickedStaticTmtr();
	afx_msg void OnStnClickedStaticTmtf();
	afx_msg void OnStnClickedStaticTmtc();
	
};


