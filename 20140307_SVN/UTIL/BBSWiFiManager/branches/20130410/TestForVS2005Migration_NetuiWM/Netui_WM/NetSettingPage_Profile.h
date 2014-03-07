#pragma once


// CNetSettingPage_Profile 대화 상자입니다.

class CNetSettingPage_Profile : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Profile)

public:
	CNetSettingPage_Profile();
	virtual ~CNetSettingPage_Profile();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NETSETTINGPAGE_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
