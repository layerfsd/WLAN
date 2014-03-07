// NetSettingPropertySheet.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPropertySheet.h"


#define DEFAULT_MANUFACTURER_ID		L"001346997DC5"

// CNetSettingPropertySheet

IMPLEMENT_DYNAMIC(CNetSettingPropertySheet, CPropertySheet)

CNetSettingPropertySheet::CNetSettingPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_pDeviceID = NULL;
}

CNetSettingPropertySheet::CNetSettingPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	
	AddPage(&page_Info);
	AddPage(&page_Wifi);
	//AddPage(&page_Profile);
	AddPage(&page_Setting);
	AddPage(&page_Tools);
	AddPage(&page_NetAdapter);

}

CNetSettingPropertySheet::~CNetSettingPropertySheet()
{
	RemovePage(&page_Info);
	RemovePage(&page_Wifi);
	//RemovePage(&page_Profile);
	RemovePage(&page_Setting);
	RemovePage(&page_Tools);
	RemovePage(&page_NetAdapter);
	
}


BEGIN_MESSAGE_MAP(CNetSettingPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CNetSettingPropertySheet �޽��� ó�����Դϴ�.
BOOL CNetSettingPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  ���⿡ Ư��ȭ�� �ڵ带 �߰��մϴ�.
	TCHAR szTemp[4] = {'\0',};
	TCHAR tszMFID[24] = {'\0',};
	DWORD dwIdx = 0;
	INT nCompareResult = 0;


	// ManuFacturer ID �� �����岨 ���� Ȯ��

	if( !GetDeviceID() )
	{
		goto InitError;
	}


	// Get Manufacture ID
	for( dwIdx = m_pDeviceID->dwPresetIDOffset ; dwIdx < m_pDeviceID->dwPresetIDOffset + 6 ; dwIdx++)
	{
		wsprintf( szTemp, L"%02X", *((PBYTE)m_pDeviceID + dwIdx) );
		wcscat( tszMFID, szTemp );
	}


	if( _tcscmp(tszMFID, DEFAULT_MANUFACTURER_ID) != 0 )
	{
		goto InitError;
	}



	return bResult;

	
InitError:
	MessageBox(L"Invalid Device ID", L"ERROR", MB_ICONERROR);
	PostQuitMessage(0);
	return FALSE;
}

BOOL CNetSettingPropertySheet::GetDeviceID(void)
{
	DWORD	len;
	BOOL	ret;
	
	m_pDeviceID = (PDEVICE_ID)LocalAlloc(LPTR, sizeof(DEVICE_ID));
	if ( m_pDeviceID == NULL )
		return FALSE;

	m_pDeviceID->dwSize = sizeof(DEVICE_ID);
	len = sizeof(DEVICE_ID);
	ret = KernelIoControl(IOCTL_HAL_GET_DEVICEID, NULL, 0, (LPVOID)m_pDeviceID, len, &len);

	/* 
	 * DEVICE_ID ����ü��ŭ �޸𸮸� �Ҵ����� �� 
	 * ���۰� �����ؼ� ������ ���°�찡 ����
	 * �׷� ��쿣 �޸𸮸� dwSize ��ŭ ���Ҵ��ؾ���.
	 */
	if( !ret )
	{
		if( GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			len = m_pDeviceID->dwSize;
			LocalFree(m_pDeviceID);
			m_pDeviceID = (PDEVICE_ID)LocalAlloc(LPTR, len);
			if( m_pDeviceID == NULL )
				return FALSE;
			m_pDeviceID->dwSize = len;
			ret = KernelIoControl( IOCTL_HAL_GET_DEVICEID, NULL, 0, (LPVOID)m_pDeviceID, len, &len);

			if( !ret )
			{
				LocalFree(m_pDeviceID);
				return FALSE;
			}
		}
		else
		{
			LocalFree(m_pDeviceID);
			return FALSE;
		}
	}


	return TRUE;


}

