#pragma once

#include <BBS_Ping.h>
#include <BBS_CommFunc.h>

// CNetSettingPage_Tools_PingView �� ���Դϴ�.

class CNetSettingPage_Tools_PingView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Tools_PingView)

protected:
	CNetSettingPage_Tools_PingView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CNetSettingPage_Tools_PingView();

public:
	enum { IDD = IDD_NETSETTINGPAGE_TOOLS_PINGVIEW };
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

	// �ɼ� ��Ʈ�� Enable / Disable
	void EnableOptionControls();
	void DisableOptionControls();

	static void PingThreadHandler(CNetSettingPage_Tools_PingView* context);
	void PingThread();


	afx_msg void OnEnChangeEditPingSendsize();
	afx_msg void OnEnChangeEditPingTtl();
	afx_msg void OnEnChangeEditPingTos();
	afx_msg void OnEnChangeEditPingTimeout();
	afx_msg void OnBnClickedButtonPing();
	afx_msg void OnDestroy();




private:

	
	CString m_strPingResult;

	// Ping ��Ʈ���� ���� PING Ŭ���� ��ü
	CBBS_Ping m_ping;
	
	// Ping �׽�Ʈ��
	BOOL m_bIsPingOperating;

	// Ping �ɼ�
	BBS_PING_OPTION m_pingOption;

	// Ping ���
	BBS_PING_RESULT m_pingResult;

	// Ping ��� �ؽ�Ʈ ǥ��â
	CEdit m_editPingResult;
	
	// PingThread �ڵ�
	HANDLE m_hPingThread;




};


