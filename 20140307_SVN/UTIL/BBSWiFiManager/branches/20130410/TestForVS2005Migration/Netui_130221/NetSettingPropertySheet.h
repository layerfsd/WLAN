#pragma once

#include "NetCardPage.h"
#include "WifiPage.h"
// CNetSettingPropertySheet
#include "TestPropPage.h"
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
	CNetCardPage page1;
	//CWifiPage page2;
	CTestPropPage page3;
};


