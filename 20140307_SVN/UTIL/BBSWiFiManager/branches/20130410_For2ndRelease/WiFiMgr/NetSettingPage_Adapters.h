#pragma once


// CNetSettingPage_Adapters ��ȭ �����Դϴ�.

class CNetSettingPage_Adapters : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Adapters)

public:
	CNetSettingPage_Adapters();
	virtual ~CNetSettingPage_Adapters();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NETUI_ADAPTERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

private :
	SHMENUBARINFO m_mbi;
};
