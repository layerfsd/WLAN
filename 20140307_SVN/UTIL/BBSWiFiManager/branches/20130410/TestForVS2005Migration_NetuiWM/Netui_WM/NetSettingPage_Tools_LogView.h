#pragma once



// CNetSettingPage_Tools_LogView �� ���Դϴ�.

class CNetSettingPage_Tools_LogView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Tools_LogView)

protected:
	CNetSettingPage_Tools_LogView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
};


