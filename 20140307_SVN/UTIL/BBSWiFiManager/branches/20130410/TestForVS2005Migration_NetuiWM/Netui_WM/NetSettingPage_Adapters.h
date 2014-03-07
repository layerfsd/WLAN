#pragma once


// CNetSettingPage_Adapters 대화 상자입니다.

class CNetSettingPage_Adapters : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Adapters)

public:
	CNetSettingPage_Adapters();
	virtual ~CNetSettingPage_Adapters();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NETUI_ADAPTERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

private :
	SHMENUBARINFO m_mbi;
};
