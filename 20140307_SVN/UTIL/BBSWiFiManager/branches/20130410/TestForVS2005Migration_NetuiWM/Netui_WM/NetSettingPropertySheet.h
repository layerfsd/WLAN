#pragma once

#include "NetSettingPage_Adapters.h"
#include "NetSettingPage_Wifi.h"
#include "NetSettingPage_Info.h"
#include "NetSettingPage_Setting.h"
#include "NetSettingPage_Tools.h"
#include "NetSettingPage_Profile.h"


// CNetSettingPropertySheet


class CNetSettingPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CNetSettingPropertySheet)

public:
	CNetSettingPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CNetSettingPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CNetSettingPropertySheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CNetSettingPage_Info		page_Info;
	CNetSettingPage_Wifi		page_Wifi;
	CNetSettingPage_Adapters	page_NetAdapter;	
	CNetSettingPage_Profile		page_Profile;
	CNetSettingPage_Setting		page_Setting;
	CNetSettingPage_Tools		page_Tools;
};


