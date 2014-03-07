#pragma once

#include "NetSettingPage_Adapters.h"
#include "NetSettingPage_Wifi.h"
#include "NetSettingPage_Info.h"
#include "NetSettingPage_Setting.h"
#include "NetSettingPage_Tools.h"
#include "NetSettingPage_Profile.h"




typedef struct _DEVICE_ID {
    DWORD   dwSize;
    DWORD   dwPresetIDOffset;
    DWORD   dwPresetIDBytes;
    DWORD   dwPlatformIDOffset;
    DWORD   dwPlatformIDBytes;
} DEVICE_ID, *PDEVICE_ID;


#define IOCTL_HAL_GET_DEVICEID CTL_CODE(FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)


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

	PDEVICE_ID m_pDeviceID;

	BOOL m_bIsHardwarePresent;
public:
	BOOL GetDeviceID(void);
};


