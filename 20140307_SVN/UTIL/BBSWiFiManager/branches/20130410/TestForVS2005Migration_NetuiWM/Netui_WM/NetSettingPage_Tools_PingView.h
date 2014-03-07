#pragma once

#include <BBS_Ping.h>
#include <BBS_CommFunc.h>

// CNetSettingPage_Tools_PingView 폼 뷰입니다.

class CNetSettingPage_Tools_PingView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Tools_PingView)

protected:
	CNetSettingPage_Tools_PingView();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	// 옵션 컨트롤 Enable / Disable
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

	// Ping 컨트롤을 위한 PING 클래스 객체
	CBBS_Ping m_ping;
	
	// Ping 테스트중
	BOOL m_bIsPingOperating;

	// Ping 옵션
	BBS_PING_OPTION m_pingOption;

	// Ping 결과
	BBS_PING_RESULT m_pingResult;

	// Ping 결과 텍스트 표시창
	CEdit m_editPingResult;
	
	// PingThread 핸들
	HANDLE m_hPingThread;




};


