#pragma once



// CNetSettingPage_Tools_LogView 폼 뷰입니다.

class CNetSettingPage_Tools_LogView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Tools_LogView)

protected:
	CNetSettingPage_Tools_LogView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CNetSettingPage_Tools_LogView();

public:
	enum { IDD = IDD_NETSETTINGPAGE_TOOLS_LOGVIEW };
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
	afx_msg void OnDestroy();
};


