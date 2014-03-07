// NetSettingPage_Setting_USIView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "NetSettingPage_Setting_USIView.h"






#define DEFAULT_USI_PARAMS_REG_PATH		L"Comm\\BCMSDDHD1\\Parms"

// Roam
#define DEFAULT_VALUE_ROAMTRIGGER			L"-75"
#define DEFAULT_VALUE_ROAMDELTA				(0)
#define DEFAULT_VALUE_ROAMSCANFREQ			(10)
#define DEFAULT_VALUE_FULLROAMSCANINTERVAL	(60)

// Power
#define DEFAULT_VALUE_POWERSAVEMODE			(2)

// CNetSettingPage_Setting_USIView

IMPLEMENT_DYNCREATE(CNetSettingPage_Setting_USIView, CFormView)

CNetSettingPage_Setting_USIView::CNetSettingPage_Setting_USIView()
	: CFormView(CNetSettingPage_Setting_USIView::IDD)
{
	m_dwRoamScanFreq = DEFAULT_VALUE_ROAMSCANFREQ;
	m_dwFullRoamScanInterval = DEFAULT_VALUE_FULLROAMSCANINTERVAL;

	m_bRoamEnable = TRUE;

	m_strRoamTrigger = DEFAULT_VALUE_ROAMTRIGGER;
}

CNetSettingPage_Setting_USIView::~CNetSettingPage_Setting_USIView()
{
}

void CNetSettingPage_Setting_USIView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROAM_TRIGGER, m_comboRoamTrigger);
	DDX_Control(pDX, IDC_COMBO_POWERSAVEMODE, m_comboPowerSaveMode);
	DDX_Control(pDX, IDC_COMBO_ROAMDELTA, m_comboRoamDelta);
}

BEGIN_MESSAGE_MAP(CNetSettingPage_Setting_USIView, CFormView)
	ON_COMMAND(ID_DEFAULT, &CNetSettingPage_Setting_USIView::OnDefault)
	ON_COMMAND(ID_APPLY, &CNetSettingPage_Setting_USIView::OnApply)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CNetSettingPage_Setting_USIView 진단입니다.

#ifdef _DEBUG
void CNetSettingPage_Setting_USIView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetSettingPage_Setting_USIView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNetSettingPage_Setting_USIView 메시지 처리기입니다.

void CNetSettingPage_Setting_USIView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	


	// RoamTrigger
	m_comboRoamTrigger.AddString(L"-50");
	m_comboRoamTrigger.AddString(L"-55");
	m_comboRoamTrigger.AddString(L"-60");
	m_comboRoamTrigger.AddString(L"-65");
	m_comboRoamTrigger.AddString(L"-70");
	m_comboRoamTrigger.AddString(L"-71");
	m_comboRoamTrigger.AddString(L"-72");
	m_comboRoamTrigger.AddString(L"-73");
	m_comboRoamTrigger.AddString(L"-74");
	m_comboRoamTrigger.AddString(L"-75");
	m_comboRoamTrigger.AddString(L"-76");
	m_comboRoamTrigger.AddString(L"-77");
	m_comboRoamTrigger.AddString(L"-78");
	m_comboRoamTrigger.AddString(L"-79");
	m_comboRoamTrigger.AddString(L"-80");
	m_comboRoamTrigger.AddString(L"-81");
	m_comboRoamTrigger.AddString(L"-82");
	m_comboRoamTrigger.AddString(L"-83");
	m_comboRoamTrigger.AddString(L"-84");
	m_comboRoamTrigger.AddString(L"-85");
	m_comboRoamTrigger.AddString(L"-86");
	m_comboRoamTrigger.AddString(L"-87");
	m_comboRoamTrigger.AddString(L"-88");
	m_comboRoamTrigger.AddString(L"-89");
	m_comboRoamTrigger.AddString(L"-90");

	// RoamDelta

	m_comboRoamDelta.AddString(L"0(10dbm)");
	m_comboRoamDelta.AddString(L"1(20dbm)");
	m_comboRoamDelta.AddString(L"2(30dbm)");

	// PowerSaveMode
	m_comboPowerSaveMode.AddString(L"CAM(0)");
	m_comboPowerSaveMode.AddString(L"PS(1)");
	m_comboPowerSaveMode.AddString(L"FastPS(2)");




	HKEY hKey = NULL;
	DWORD dwDisposition = 0;


	// 초기값 레지스트리에서 얻어와서 갱신시켜줌
	// wl 커맨드를 이용하여 USI 드라이버에서 직접 Set/Get을 할 수 있지만, 추가 라이브러리 구현이 필요함
	// 일정상 일단 레지스트리를 이용한 읽어오기 / 적용후 리셋을 이용한 방법 사용하도록 함
	if( RegCreateKeyEx(HKEY_LOCAL_MACHINE, DEFAULT_USI_PARAMS_REG_PATH, 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS )
	{

		DWORD dwType = 0;
		DWORD dwDataSize = 0;
		DWORD dwData = 0;
		TCHAR tszData[128] = {L'\0',};
		LONG lRet = 0;


		//
		// Roam Enable
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"roam_off", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || (dwData<0) )
		{
			dwData = 0; // roam Enable
			
			m_bRoamEnable = TRUE;
			RegSetValueEx(hKey, L"roam_off", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );
		}
		else
		{
			if(dwData == 0)
			{
				m_bRoamEnable = TRUE;
			}
			else
			{
				m_bRoamEnable = FALSE;
			}

		}
		
		//
		// RoamTrigger
		//
		dwType = REG_SZ;
		dwDataSize = sizeof(tszData);
		lRet = RegQueryValueEx(hKey, L"RoamTrigger", NULL, &dwType, (LPBYTE)&tszData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || ( _tcslen(tszData) <= 0 ) )
		{
			_tcscpy(tszData, m_strRoamTrigger.GetBuffer(0));
			RegSetValueEx(hKey, L"RoamTrigger", 0, REG_SZ, (LPBYTE)tszData, _tcslen(tszData) * sizeof(TCHAR) );
		}
		else
		{
			m_strRoamTrigger = tszData;
		}

		


		//
		// RoamDelta (use driver default for 1(moderate), convert 0(aggressive) and 2(conservative) )
		//
		dwType = REG_SZ;
		dwDataSize = sizeof(tszData);
		lRet = RegQueryValueEx(hKey, L"RoamDelta", NULL, &dwType, (LPBYTE)&tszData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || ( _tcslen(tszData) <= 0 )  )
		{
			m_comboRoamDelta.SetCurSel(DEFAULT_VALUE_ROAMDELTA);
			_itot(m_comboRoamDelta.GetCurSel(), tszData, 10);
			RegSetValueEx(hKey, L"RoamDelta", 0, REG_SZ, (LPBYTE)tszData, _tcslen(tszData) * sizeof(TCHAR) );
		}
		else
		{
			dwData = _ttoi(tszData);
			if(dwData != 0 && dwData != 1 && dwData != 2)
			{
				m_comboRoamDelta.SetCurSel(DEFAULT_VALUE_ROAMDELTA);
			}
			else
			{
				m_comboRoamDelta.SetCurSel(dwData);
			}
		}


		//
		// RoamScanFreq
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"RoamScanFreq", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || (dwData<0) )
		{
			RegSetValueEx(hKey, L"RoamScanFreq", 0, REG_DWORD, (LPBYTE)&m_dwRoamScanFreq, sizeof(m_dwRoamScanFreq) );
		}
		else
		{
			m_dwRoamScanFreq = dwData;
		}


		//
		// FullRoamScanInterval
		//
		dwType = REG_DWORD;
		dwDataSize = sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, L"FullRoamScanInterval", NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
		if( (lRet != ERROR_SUCCESS) || (dwData<0) )
		{
			RegSetValueEx(hKey, L"FullRoamScanInterval", 0, REG_DWORD, (LPBYTE)&m_dwFullRoamScanInterval, sizeof(m_dwFullRoamScanInterval) );
		}
		else
		{
			m_dwFullRoamScanInterval = dwData;
		}








		//
		// PowerSaveMode
		//
		dwType = REG_SZ;
		dwDataSize = sizeof(tszData);
		lRet = RegQueryValueEx(hKey, L"PowerSaveMode", NULL, &dwType, (LPBYTE)&tszData, &dwDataSize);		

		if( (lRet != ERROR_SUCCESS) || ( _tcslen(tszData) <= 0 ) )
		{
			m_comboPowerSaveMode.SetCurSel(DEFAULT_VALUE_POWERSAVEMODE);
			_itot(m_comboPowerSaveMode.GetCurSel(), tszData, 10);
			RegSetValueEx(hKey, L"PowerSaveMode", 0, REG_SZ, (LPBYTE)tszData, _tcslen(tszData) * sizeof(TCHAR) );
		}
		else
		{
			dwData = _ttoi(tszData);
			if(dwData != 0 && dwData != 1 && dwData != 2)
			{
				m_comboPowerSaveMode.SetCurSel(DEFAULT_VALUE_POWERSAVEMODE);
			}
			else
			{
				m_comboPowerSaveMode.SetCurSel(dwData);
			}
		}



		RegCloseKey(hKey);
	}


	// Set Data to Controls
	if(m_bRoamEnable)
	{
		CheckDlgButton(IDC_CHECK_ROAM_ENABLE, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_ROAM_ENABLE, BST_UNCHECKED);
	}


	// 콤보 박스 선택을 위한 비교
	for(int i = 0; i< m_comboRoamTrigger.GetCount(); i++)
	{
		TCHAR tszTemp[32] = L"";

		m_comboRoamTrigger.GetLBText(i, tszTemp);
		if( _tcscmp(tszTemp, m_strRoamTrigger.GetBuffer(0)) == 0)
		{
			m_comboRoamTrigger.SetCurSel(i);
			break;
		}
	}

	SetDlgItemInt(IDC_EDIT_ROAM_SCANFREQ, m_dwRoamScanFreq);
	SetDlgItemInt(IDC_EDIT_ROAM_FULLROAMSCNITV, m_dwFullRoamScanInterval);
	
}

void CNetSettingPage_Setting_USIView::OnDefault()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.


	m_strRoamTrigger = DEFAULT_VALUE_ROAMTRIGGER;

	m_dwRoamScanFreq = DEFAULT_VALUE_ROAMSCANFREQ;
	m_dwFullRoamScanInterval = DEFAULT_VALUE_FULLROAMSCANINTERVAL;

	m_comboRoamDelta.SetCurSel(DEFAULT_VALUE_ROAMDELTA);
	m_comboPowerSaveMode.SetCurSel(DEFAULT_VALUE_POWERSAVEMODE);

	m_bRoamEnable = TRUE;



	// Set Data to Controls
	if(m_bRoamEnable)
	{
		CheckDlgButton(IDC_CHECK_ROAM_ENABLE, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_ROAM_ENABLE, BST_UNCHECKED);
	}

	// 콤보 박스 선택을 위한 비교
	for(int i = 0; i< m_comboRoamTrigger.GetCount(); i++)
	{
		TCHAR tszTemp[32] = L"";

		m_comboRoamTrigger.GetLBText(i, tszTemp);
		if( _tcscmp(tszTemp, m_strRoamTrigger.GetBuffer(0)) == 0)
		{
			m_comboRoamTrigger.SetCurSel(i);
			break;
		}
	}

	SetDlgItemInt(IDC_EDIT_ROAM_SCANFREQ, m_dwRoamScanFreq);
	SetDlgItemInt(IDC_EDIT_ROAM_FULLROAMSCNITV, m_dwFullRoamScanInterval);

}

void CNetSettingPage_Setting_USIView::OnApply()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HKEY hKey = NULL;
	DWORD dwDisposition = 0;


	// Get Controls Status
	if( IsDlgButtonChecked(IDC_CHECK_ROAM_ENABLE) == BST_CHECKED)
	{
		m_bRoamEnable = TRUE;
	}
	else
	{
		m_bRoamEnable = FALSE;
	}


	TCHAR tszTemp[32] = L"";

	m_comboRoamTrigger.GetLBText(m_comboRoamTrigger.GetCurSel(), tszTemp);
	m_strRoamTrigger = tszTemp;

	m_dwRoamScanFreq = GetDlgItemInt(IDC_EDIT_ROAM_SCANFREQ);
	m_dwFullRoamScanInterval = GetDlgItemInt(IDC_EDIT_ROAM_FULLROAMSCNITV);

	
	// Set Registry

	if( RegCreateKeyEx(HKEY_LOCAL_MACHINE, DEFAULT_USI_PARAMS_REG_PATH, 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS )
	{
		DWORD dwType = 0;
		DWORD dwDataSize = 0;
		DWORD dwData = 0;
		TCHAR tszData[128] = {L'\0',};
		LONG lRet = 0;


		//
		// Roam Enable
		//
		if(m_bRoamEnable == TRUE)
		{
			dwData = 0;
		}
		else
		{
			dwData = 1;
		}
		RegSetValueEx(hKey, L"roam_off", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData) );

		
		//
		// RoamTrigger
		//
		_tcscpy(tszData, m_strRoamTrigger.GetBuffer(0));
		RegSetValueEx(hKey, L"RoamTrigger", 0, REG_SZ, (LPBYTE)tszData, _tcslen(tszData) * sizeof(TCHAR) );


		//
		// RoamDelta
		//
		_itot(m_comboRoamDelta.GetCurSel(), tszData, 10);
		RegSetValueEx(hKey, L"RoamDelta", 0, REG_SZ, (LPBYTE)tszData, _tcslen(tszData) * sizeof(TCHAR) );



		//
		// RoamScanFreq
		//
		RegSetValueEx(hKey, L"RoamScanFreq", 0, REG_DWORD, (LPBYTE)&m_dwRoamScanFreq, sizeof(m_dwRoamScanFreq) );


		//
		// FullRoamScanInterval
		//
		RegSetValueEx(hKey, L"FullRoamScanInterval", 0, REG_DWORD, (LPBYTE)&m_dwFullRoamScanInterval, sizeof(m_dwFullRoamScanInterval) );







		//
		// PowerSaveMode
		//
		_itot(m_comboPowerSaveMode.GetCurSel(), tszData, 10);
		RegSetValueEx(hKey, L"PowerSaveMode", 0, REG_SZ, (LPBYTE)tszData, _tcslen(tszData) * sizeof(TCHAR) );


		RegCloseKey(hKey);
	}



	if( MessageBox(_T("Now, System will be reboot for apply"), _T("Warning"), MB_YESNO) == IDYES )
	{
		Sleep(500);
		KernelIoControl(IOCTL_HAL_REBOOT, NULL , NULL , NULL, NULL ,NULL);
	}


}

void CNetSettingPage_Setting_USIView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

