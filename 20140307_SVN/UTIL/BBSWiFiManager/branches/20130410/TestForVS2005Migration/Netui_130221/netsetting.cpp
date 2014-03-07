
#include "stdafx.h"
#include "Netui_130221.h"
#include "NetSettingPropertySheet.h"

HRESULT WINAPI NetCardsSettingDialog(LPCTSTR pszClass)
{
	RETAILMSG(1, (TEXT("[NETUI] New NetCardsSettingDialog \r\n")));
	
	CNetSettingPropertySheet MySheet(_T("Setting"));

	MySheet.DoModal();

	return S_OK;

}
