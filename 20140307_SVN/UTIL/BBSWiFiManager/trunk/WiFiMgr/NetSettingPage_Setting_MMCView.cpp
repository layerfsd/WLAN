// NetSettingPage_Setting_MMCView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Setting_MMCView.h"




#define DEFAULT_MMC_PARAMS_REG_PATH		L"Comm\\AR6K_SD1\\Parms"

// Roam
#define DEFAULT_VALUE_ROAM_SCANSTART		(75)
#define DEFAULT_VALUE_ROAM_ROAMSTART		(80)
#define DEFAULT_VALUE_ROAM_TARGETRSSI		(60)


// Power
#define DEFAULT_VALUE_POWERSAVEMODE			(0)

// Scan
#define DEFAULT_VALUE_NODEAGE				(15000)
#define DEFAULT_VALUE_BKSCANPERIOD			(5)



TCHAR *RSSI_VALUE_TABLE[] = {
	L"-50",
	L"-55",
	L"-60",
	L"-65",
	L"-70",
	L"-71",
	L"-72",
	L"-73",
	L"-74",
	L"-75",
	L"-76",
	L"-77",
	L"-78",
	L"-79",
	L"-80",
	L"-81",
	L"-82",
	L"-83",
	L"-84",
	L"-85",
	L"-86",
	L"-87",
	L"-88",
	L"-89",
	L"-90",
};






// CNetSettingPage_Setting_MMCView

IMPLEMENT_DYNCREATE(CNetSettingPage_Setting_MMCView, CFormView)

CNetSettingPage_Setting_MMCView::CNetSettingPage_Setting_MMCView()
	: CFormView(CNetSettingPage_Setting_MMCView::IDD)
{
	m_dwScan_BKScanPrd = DEFAULT_VALUE_BKSCANPERIOD;
	m_dwScan_NodeAge = DEFAULT_VALUE_NODEAGE;

	m_nRoam_ScanStart = DEFAULT_VALUE_ROAM_SCANSTART;
	m_nRoam_RoamStart = DEFAULT_VALUE_ROAM_ROAMSTART;
	m_nRoam_TargetRSSI = DEFAULT_VALUE_ROAM_TARGETRSSI;
	
	m_bRoamEnable = TRUE;
	m_bBKScanEnable = TRUE;
}

CNetSettingPage_Setting_MMCView::~CNetSettingPage_Setting_MMCView()
{
}

void CNetSettingPage_Setting_MMCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROAM_SCANSTART, m_comboRoam_ScanStart);
	DDX_Control(pDX, IDC_COMBO_ROAM_ROAMSTART, m_comboRoam_RoamStart);
	DDX_Control(pDX, IDC_COMBO_ROAM_TARGETRSSI, m_comboRoam_TargetRSSI);
	DDX_Control(pDX, IDC_COMBO_SCAN_BKSCANPRD, m_comboScan_BKScanPrd);
	DDX_Control(pDX, IDC_COMBO_SCAN_NODEAGE, m_comboScan_NodeAge);
}

BEGIN_MESSAGE_MAP(CNetSettingPage_Setting_MMCView, CFormView)
	ON_WM_DESTROY()
	ON_COMMAND(ID_DEFAULT, &CNetSettingPage_Setting_MMCView::OnDefault)
	ON_COMMAND(ID_APPLY, &CNetSettingPage_Setting_MMCView::OnApply)
END_MESSAGE_MAP()


// CNetSettingPage_Setting_MMCView 진단입니다.

#ifdef _DEBUG
void CNetSettingPage_Setting_MMCView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetSettingPage_Setting_MMCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNetSettingPage_Setting_MMCView 메시지 처리기입니다.

void CNetSettingPage_Setting_MMCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	HKEY hKey = NULL;
	DWORD dwDisposition = 0;


	// 컨트롤 초기화
	// ScanStart (ScanThreshold), RoamStart (RoamThreshold), TargetRSSI (RoamFloorThre)
	for(int i=0; i<sizeof(RSSI_VALUE_TABLE) / sizeof(RSSI_VALUE_TABLE[0]); i++)
	{
		m_comboRoam_ScanStart.AddString(RSSI_VALUE_TABLE[i]);
		m_comboRoam_RoamStart.AddString(RSSI_VALUE_TABLE[i]);
		m_comboRoam_TargetRSSI.AddString(RSSI_VALUE_TABLE[i]);
	}

	
	m_comboScan_BKScanPrd.AddString(L"1");
	m_comboScan_BKScanPrd.AddString(L"2");
	m_comboScan_BKScanPrd.AddString(L"3");
	m_comboScan_BKScanPrd.AddString(L"4");
	m_comboScan_BKScanPrd.AddString(L"5");
	m_comboScan_BKScanPrd.AddString(L"6");
	m_comboScan_BKScanPrd.AddString(L"7");
	m_comboScan_BKScanPrd.AddString(L"8");
	m_comboScan_BKScanPrd.AddString(L"9");
	m_comboScan_BKScanPrd.AddString(L"10");
	m_comboScan_BKScanPrd.AddString(L"13");
	m_comboScan_BKScanPrd.AddString(L"15");
	m_comboScan_BKScanPrd.AddString(L"18");
	m_comboScan_BKScanPrd.AddString(L"20");
	m_comboScan_BKScanPrd.AddString(L"30");


	m_comboScan_NodeAge.AddString(L"1000");
	m_comboScan_NodeAge.AddString(L"2000");
	m_comboScan_NodeAge.AddString(L"3000");
	m_comboScan_NodeAge.AddString(L"4000");
	m_comboScan_NodeAge.AddString(L"5000");
	m_comboScan_NodeAge.AddString(L"6000");
	m_comboScan_NodeAge.AddString(L"7000");
	m_comboScan_NodeAge.AddString(L"8000");
	m_comboScan_NodeAge.AddString(L"9000");
	m_comboScan_NodeAge.AddString(L"10000");
	m_comboScan_NodeAge.AddString(L"15000");
	m_comboScan_NodeAge.AddString(L"20000");
	m_comboScan_NodeAge.AddString(L"25000");
	m_comboScan_NodeAge.AddString(L"30000");
	m_comboScan_NodeAge.AddString(L"40000");
	m_comboScan_NodeAge.AddString(L"50000");
	m_comboScan_NodeAge.AddString(L"60000");



	if( RegCreateKeyEx(HKEY_LOCAL_MACHINE, DEFAULT_MMC_PARAMS_REG_PATH, 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS )
	{

		DWORD dwType = 0;
		DWORD dwDataSize = 0;
		DWORD dwData = 0;
		TCHAR tszData[128] = {L'\0',};
		LONG lRet = 0;




		//
		// bkScanEnable
		// 
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"bkScanEnable", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || ( (dwData !=0) && (dwData !=1 ) )  )
		{
			dwData = 1; // roam Enable
			
			m_bBKScanEnable = TRUE;
			RegSetValueEx(hKey, L"bkScanEnable", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
		}
		else
		{
			if(dwData == 0)
			{
				m_bBKScanEnable = FALSE;
			}
			else
			{
				m_bBKScanEnable = TRUE;
			}
		}


		//
		// bkScanPeriod
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"bkScanPeriod", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || (dwData<0) )
		{
			dwData = m_dwScan_BKScanPrd;
			RegSetValueEx(hKey, L"bkScanPeriod", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
		}
		else
		{
			m_dwScan_BKScanPrd = dwData;
		}

		//
		// nodeAge
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"nodeAge", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || (dwData<0) )
		{
			dwData = m_dwScan_NodeAge;
			RegSetValueEx(hKey, L"nodeAge", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
		}
		else
		{
			m_dwScan_NodeAge = dwData;
		}








		



		//
		// Roam Enable
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"hostAssistedRoaming", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || ( (dwData !=0) && (dwData !=1 ) )  )
		{
			dwData = 1; // roam Enable
			
			m_bRoamEnable = TRUE;
			RegSetValueEx(hKey, L"hostAssistedRoaming", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
		}
		else
		{
			// 0인경우에만 Disable, 그외 Enable로 설정함
			if(dwData == 0)
			{
				m_bRoamEnable = FALSE;
			}
			else
			{
				m_bRoamEnable = TRUE;
			}

		}


		//
		// ScanStart (ScanThreshold)
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"ScanThreshold", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || (dwData<0) )
		{
			dwData = m_nRoam_ScanStart;
			RegSetValueEx(hKey, L"ScanThreshold", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
		}
		else
		{
			m_nRoam_ScanStart = dwData;
		}

		
		//
		// RoamStart (RoamThreshold)
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"RoamThreshold", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || (dwData<0) )
		{
			dwData = m_nRoam_RoamStart;
			RegSetValueEx(hKey, L"RoamThreshold", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
		}
		else
		{
			m_nRoam_RoamStart = dwData;
		}




		//
		// TargetRSSI (RoamFloorThre)
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"RoamFloorThre", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || (dwData<0) )
		{
			dwData = m_nRoam_TargetRSSI;
			RegSetValueEx(hKey, L"RoamFloorThre", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
		}
		else
		{
			m_nRoam_TargetRSSI = dwData;
		}





		UpdateControlState();

		RegCloseKey(hKey);
	}

}

void CNetSettingPage_Setting_MMCView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CNetSettingPage_Setting_MMCView::OnDefault()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dwScan_BKScanPrd = DEFAULT_VALUE_BKSCANPERIOD;
	m_dwScan_NodeAge = DEFAULT_VALUE_NODEAGE;

	m_nRoam_ScanStart = DEFAULT_VALUE_ROAM_SCANSTART;
	m_nRoam_RoamStart = DEFAULT_VALUE_ROAM_ROAMSTART;
	m_nRoam_TargetRSSI = DEFAULT_VALUE_ROAM_TARGETRSSI;
	
	m_bRoamEnable = TRUE;
	m_bBKScanEnable = TRUE;


	UpdateControlState();
}

void CNetSettingPage_Setting_MMCView::OnApply()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	HKEY hKey = NULL;
	DWORD dwDisposition = 0;

	// Get Controls Status

	if( IsDlgButtonChecked(IDC_CHECK_SCAN_BKSCANENABLE) == BST_CHECKED)
	{
		m_bBKScanEnable = TRUE;
	}
	else
	{
		m_bBKScanEnable = FALSE;
	}


	if( IsDlgButtonChecked(IDC_CHECK_ROAM_ENABLE) == BST_CHECKED)
	{
		m_bRoamEnable = TRUE;
	}
	else
	{
		m_bRoamEnable = FALSE;
	}

	TCHAR tszTemp[32] = L"";


	m_comboScan_BKScanPrd.GetLBText(m_comboScan_BKScanPrd.GetCurSel(), tszTemp);
	m_dwScan_BKScanPrd = _ttoi(tszTemp);

	m_comboScan_NodeAge.GetLBText(m_comboScan_NodeAge.GetCurSel(), tszTemp);
	m_dwScan_NodeAge = _ttoi(tszTemp);


	m_comboRoam_ScanStart.GetLBText(m_comboRoam_ScanStart.GetCurSel(), tszTemp);
	m_nRoam_ScanStart = _ttoi(tszTemp);

	m_comboRoam_RoamStart.GetLBText(m_comboRoam_RoamStart.GetCurSel(), tszTemp);
	m_nRoam_RoamStart = _ttoi(tszTemp);

	m_comboRoam_TargetRSSI.GetLBText(m_comboRoam_TargetRSSI.GetCurSel(), tszTemp);
	m_nRoam_TargetRSSI = _ttoi(tszTemp);

	// Set Registry

	if( RegCreateKeyEx(HKEY_LOCAL_MACHINE, DEFAULT_MMC_PARAMS_REG_PATH, 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS )
	{
		DWORD dwType = 0;
		DWORD dwDataSize = 0;
		DWORD dwData = 0;
		TCHAR tszData[128] = {L'\0',};
		LONG lRet = 0;



		//
		// bkScanEnable
		// 
		if(m_bBKScanEnable == TRUE)
		{
			dwData = 1;
		}
		else
		{
			dwData = 0;
		}
		RegSetValueEx(hKey, L"bkScanEnable", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );



		//
		// bkScanPeriod
		//
		dwData = m_dwScan_BKScanPrd;
		RegSetValueEx(hKey, L"bkScanPeriod", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
	

		//
		// nodeAge
		//
		dwData = m_dwScan_NodeAge;
		RegSetValueEx(hKey, L"nodeAge", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );












		//
		// Roam Enable
		//
		if(m_bRoamEnable == TRUE)
		{
			dwData = 1;
		}
		else
		{
			dwData = 0;
		}
		RegSetValueEx(hKey, L"hostAssistedRoaming", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );


		//
		// ScanStart (ScanThreshold)
		//
		dwData = -m_nRoam_ScanStart;
		RegSetValueEx(hKey, L"ScanThreshold", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );


		//
		// RoamStart (RoamThreshold)
		//
		dwData = -m_nRoam_RoamStart;
		RegSetValueEx(hKey, L"RoamThreshold", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );


		//
		// TargetRSSI (RoamFloorThre)
		//
		dwData = -m_nRoam_TargetRSSI;
		RegSetValueEx(hKey, L"RoamFloorThre", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );





		RegCloseKey(hKey);
	}



	if( MessageBox(_T("Now, System will be reboot for apply"), _T("Warning"), MB_YESNO) == IDYES )
	{
		Sleep(500);
		KernelIoControl(IOCTL_HAL_REBOOT, NULL , NULL , NULL, NULL ,NULL);
	}
}


BOOL CNetSettingPage_Setting_MMCView::UpdateControlState()
{
	// Update Controls

	if(m_bBKScanEnable)
	{
		CheckDlgButton(IDC_CHECK_SCAN_BKSCANENABLE, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_SCAN_BKSCANENABLE, BST_UNCHECKED);
	}

	for(int i = 0; i< m_comboScan_BKScanPrd.GetCount(); i++)
	{
		TCHAR tszTemp[32] = L"";
		TCHAR tszValue[32] = L"";

		wsprintf(tszValue, L"%d", m_dwScan_BKScanPrd);

		m_comboScan_BKScanPrd.GetLBText(i, tszTemp);
		if( _tcscmp(tszTemp, tszValue) == 0)
		{
			m_comboScan_BKScanPrd.SetCurSel(i);
			break;
		}
	}

	for(int i = 0; i< m_comboScan_NodeAge.GetCount(); i++)
	{
		TCHAR tszTemp[32] = L"";
		TCHAR tszValue[32] = L"";

		wsprintf(tszValue, L"%d", m_dwScan_NodeAge);

		m_comboScan_NodeAge.GetLBText(i, tszTemp);
		if( _tcscmp(tszTemp, tszValue) == 0)
		{
			m_comboScan_NodeAge.SetCurSel(i);
			break;
		}
	}







	if(m_bRoamEnable)
	{
		CheckDlgButton(IDC_CHECK_ROAM_ENABLE, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_ROAM_ENABLE, BST_UNCHECKED);
	}



	for(int i = 0; i< m_comboRoam_ScanStart.GetCount(); i++)
	{
		TCHAR tszTemp[32] = L"";
		TCHAR tszValue[32] = L"";

		wsprintf(tszValue, L"%d", -m_nRoam_ScanStart);

		m_comboRoam_ScanStart.GetLBText(i, tszTemp);
		if( _tcscmp(tszTemp, tszValue) == 0)
		{
			m_comboRoam_ScanStart.SetCurSel(i);
			break;
		}
	}


	for(int i = 0; i< m_comboRoam_RoamStart.GetCount(); i++)
	{
		TCHAR tszTemp[32] = L"";
		TCHAR tszValue[32] = L"";

		wsprintf(tszValue, L"%d", -m_nRoam_RoamStart);

		m_comboRoam_RoamStart.GetLBText(i, tszTemp);
		if( _tcscmp(tszTemp, tszValue) == 0)
		{
			m_comboRoam_RoamStart.SetCurSel(i);
			break;
		}
	}


	for(int i = 0; i< m_comboRoam_TargetRSSI.GetCount(); i++)
	{
		TCHAR tszTemp[32] = L"";
		TCHAR tszValue[32] = L"";

		wsprintf(tszValue, L"%d", -m_nRoam_TargetRSSI);

		m_comboRoam_TargetRSSI.GetLBText(i, tszTemp);
		if( _tcscmp(tszTemp, tszValue) == 0)
		{
			m_comboRoam_TargetRSSI.SetCurSel(i);
			break;
		}
	}


	return TRUE;
}
