#pragma once


// CNetSettingPage_Profile ��ȭ �����Դϴ�.

class CNetSettingPage_Profile : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Profile)

public:
	CNetSettingPage_Profile();
	virtual ~CNetSettingPage_Profile();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NETSETTINGPAGE_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
