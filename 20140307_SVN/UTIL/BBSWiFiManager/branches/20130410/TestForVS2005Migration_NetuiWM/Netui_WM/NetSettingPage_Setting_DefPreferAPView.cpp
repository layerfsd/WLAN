// NetSettingPage_Setting_DefPreferAPView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Netui_WM.h"
#include "NetSettingPage_Setting_DefPreferAPView.h"



const TCHAR* AuthenStrTable[] =
{
	L"Open",
	L"Shared",
	L"WPA",
	L"WPA-PSK",
	L"WPA2",
	L"WPA2-PSK",
};

const TCHAR* EncryptionStrTable_WEP_DISABLE[] =
{
  L"Disabled",
  L"WEP"
};

const TCHAR* EncryptionStrTable_TKIP_AES[] =
{
  L"TKIP",
  L"AES"
};

const TCHAR* WEP_KeyIndexTable[] =
{
  L"1",
  L"2",
  L"3",
  L"4"
};




// CNetSettingPage_Setting_DefPreferAPView

IMPLEMENT_DYNCREATE(CNetSettingPage_Setting_DefPreferAPView, CFormView)

CNetSettingPage_Setting_DefPreferAPView::CNetSettingPage_Setting_DefPreferAPView()
	: CFormView(CNetSettingPage_Setting_DefPreferAPView::IDD)
	, m_bEnable(FALSE)
	, m_bDHCP(FALSE)
	, m_bHex(FALSE)
{
	memset(&m_defPrefNetInfo, 0, sizeof(BBS_DEF_PREF_NETWORK_INFO));
}

CNetSettingPage_Setting_DefPreferAPView::~CNetSettingPage_Setting_DefPreferAPView()
{
}

void CNetSettingPage_Setting_DefPreferAPView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_FORCE_ADDTOLIST, m_bEnable);
	DDX_Check(pDX, IDC_CHECK_DHCP, m_bDHCP);
	DDX_Control(pDX, IDC_COMBO_AUTH, m_comboAuth);
	DDX_Control(pDX, IDC_COMBO_ENCR, m_comboEncr);
	DDX_Control(pDX, IDC_COMBO_KEYINDEX, m_comboKeyIndex);
	DDX_Check(pDX, IDC_CHECK_HEX, m_bHex);
}

BEGIN_MESSAGE_MAP(CNetSettingPage_Setting_DefPreferAPView, CFormView)
	ON_BN_CLICKED(IDC_CHECK_FORCE_ADDTOLIST, &CNetSettingPage_Setting_DefPreferAPView::OnBnClickedCheckForceAddtolist)
	ON_BN_CLICKED(IDC_CHECK_DHCP, &CNetSettingPage_Setting_DefPreferAPView::OnBnClickedCheckDhcp)
	ON_CBN_SELENDOK(IDC_COMBO_AUTH, &CNetSettingPage_Setting_DefPreferAPView::OnCbnSelendokComboAuth)
	ON_CBN_SELENDOK(IDC_COMBO_ENCR, &CNetSettingPage_Setting_DefPreferAPView::OnCbnSelendokComboEncr)
	ON_COMMAND(ID_APPLY, &CNetSettingPage_Setting_DefPreferAPView::OnApply)
	ON_COMMAND(ID_DEFAULT, &CNetSettingPage_Setting_DefPreferAPView::OnDefault)
END_MESSAGE_MAP()


// CNetSettingPage_Setting_DefPreferAPView 진단입니다.

#ifdef _DEBUG
void CNetSettingPage_Setting_DefPreferAPView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetSettingPage_Setting_DefPreferAPView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNetSettingPage_Setting_DefPreferAPView 메시지 처리기입니다.

void CNetSettingPage_Setting_DefPreferAPView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 인증방식 콤보박스 초기화
	for(int i=0;i< sizeof(AuthenStrTable) / sizeof(AuthenStrTable[0]); i++)
	{
		m_comboAuth.AddString(AuthenStrTable[i]);
	}

	// 암호화방식 콤보박스 초기화
	for(int i=0;i< sizeof(EncryptionStrTable_WEP_DISABLE) / sizeof(EncryptionStrTable_WEP_DISABLE[0]); i++)
	{
		m_comboEncr.AddString(EncryptionStrTable_WEP_DISABLE[i]);

		RETAILMSG(1, (TEXT("[NETUI] %s(%d) \r\n"), EncryptionStrTable_WEP_DISABLE[i], sizeof(EncryptionStrTable_WEP_DISABLE) / sizeof(EncryptionStrTable_WEP_DISABLE[0]) ));
	}
	

	// 키인덱스 콤보박스 초기화
	for(int i=0;i< sizeof(WEP_KeyIndexTable) / sizeof(WEP_KeyIndexTable[0]); i++)
	{
		m_comboKeyIndex.AddString(WEP_KeyIndexTable[i]);
	}


	// Load setting value from XML
	GetDefPrefNetOpt(&m_defPrefNetInfo);


	if(m_defPrefNetInfo.bEnable)
	{
		m_bEnable = TRUE;
		CheckDlgButton(IDC_CHECK_FORCE_ADDTOLIST, BST_CHECKED);
	}
	else
	{
		m_bEnable = FALSE;
		CheckDlgButton(IDC_CHECK_FORCE_ADDTOLIST, BST_UNCHECKED);
	}
	OnBnClickedCheckForceAddtolist();


	if(m_defPrefNetInfo.bEnable)
	{
		m_bDHCP = TRUE;
		CheckDlgButton(IDC_CHECK_DHCP, BST_CHECKED);
	}
	else
	{
		m_bDHCP = FALSE;
		CheckDlgButton(IDC_CHECK_DHCP, BST_UNCHECKED);
	}
	OnBnClickedCheckDhcp();


	SetDlgItemText(IDC_EDIT_IPADDR, m_defPrefNetInfo.tszIPAddr);
	SetDlgItemText(IDC_EDIT_SUBNETMASK, m_defPrefNetInfo.tszSubNetMask);
	SetDlgItemText(IDC_EDIT_GATEWAY, m_defPrefNetInfo.tszGateWay);
	SetDlgItemText(IDC_EDIT_DNS, m_defPrefNetInfo.tszDnsAddr);
	SetDlgItemText(IDC_EDIT_SSID, m_defPrefNetInfo.tszSSID);

	if(m_defPrefNetInfo.dwAuthType < 0)
	{
		m_comboAuth.SetCurSel(0);
	}
	else
	{
		m_comboAuth.SetCurSel(m_defPrefNetInfo.dwAuthType);
	}
	OnCbnSelendokComboAuth();


	if(m_defPrefNetInfo.dwEncrType < 0)
	{
		m_comboEncr.SetCurSel(1);
	}
	else
	{
		m_comboEncr.SetCurSel(m_defPrefNetInfo.dwEncrType);
	}
	OnCbnSelendokComboEncr();


	SetDlgItemText(IDC_EDIT_NETWORK_KEY, m_defPrefNetInfo.tszNetworkKey);

	if(m_defPrefNetInfo.dwKeyIndex < 0)
	{
		m_comboKeyIndex.SetCurSel(0);
	}
	else
	{
		m_comboKeyIndex.SetCurSel(m_defPrefNetInfo.dwKeyIndex);
	}



	if(m_defPrefNetInfo.bHexKey)
	{
		m_bHex = TRUE;
		CheckDlgButton(IDC_CHECK_HEX, BST_CHECKED);
	}
	else
	{
		m_bHex = FALSE;
		CheckDlgButton(IDC_CHECK_HEX, BST_UNCHECKED);
	}


	UpdateData(TRUE);

}






void CNetSettingPage_Setting_DefPreferAPView::OnBnClickedCheckForceAddtolist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	// 사용상태 확인
	if(m_bEnable)
	{
		EnableAllControls(TRUE);
	}
	else
	{
		EnableAllControls(FALSE);
	}


	UpdateData(FALSE);
}

// 사용안함 체크시 모든 컨트롤 Disable 시키는 함수
BOOL CNetSettingPage_Setting_DefPreferAPView::EnableAllControls(BOOL bEnable)
{
	GetDlgItem(IDC_CHECK_DHCP)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_IPADDR)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SUBNETMASK)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_GATEWAY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_DNS)->EnableWindow(bEnable);

	GetDlgItem(IDC_EDIT_SSID)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_AUTH)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_ENCR)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_NETWORK_KEY)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_KEYINDEX)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_HEX)->EnableWindow(bEnable);

	return TRUE;
}

void CNetSettingPage_Setting_DefPreferAPView::OnBnClickedCheckDhcp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// DHCP 사용시 IP주소 등은 입력 받지 않음

	UpdateData(TRUE);

	if(m_bDHCP)
	{
		GetDlgItem(IDC_EDIT_IPADDR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SUBNETMASK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GATEWAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DNS)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_IPADDR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SUBNETMASK)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GATEWAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DNS)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

void CNetSettingPage_Setting_DefPreferAPView::OnCbnSelendokComboAuth()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	int nSelect = m_comboAuth.GetCurSel();
	int nEncrCount = m_comboEncr.GetCount();


	RETAILMSG(1, (TEXT("[NETUI] CNetSettingPage_Setting_DefPreferAPView::OnCbnSelendokComboAuth \r\n") ));
	RETAILMSG(1, (TEXT("[NETUI] nSelect(%d), GetCount(%d) \r\n"), nSelect, nEncrCount ));


	GetDlgItem(IDC_COMBO_KEYINDEX)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_HEX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_NETWORK_KEY)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_ENCR)->EnableWindow(TRUE);

	// Delete
	m_comboEncr.ResetContent();
	switch(nSelect)
	{
		// Open, Disable
	case 0:
	case 1:
		for(int i=0;i< sizeof(EncryptionStrTable_WEP_DISABLE) / sizeof(EncryptionStrTable_WEP_DISABLE[0]); i++)
		{
			RETAILMSG(1, (TEXT("[NETUI] %s(%d) \r\n"), EncryptionStrTable_WEP_DISABLE[i], i ));
			m_comboEncr.AddString(EncryptionStrTable_WEP_DISABLE[i]);
		}
		m_comboEncr.SetCurSel(1);
		break;

		// WPA / WPA2
	case 2:
	case 4:
		GetDlgItem(IDC_COMBO_KEYINDEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NETWORK_KEY)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ENCR)->EnableWindow(FALSE);
		break;

		// WPA-PSK / WPA2-PSK
	default:
		for(int i=0;i< sizeof(EncryptionStrTable_TKIP_AES) / sizeof(EncryptionStrTable_TKIP_AES[0]); i++)
		{
			m_comboEncr.AddString(EncryptionStrTable_TKIP_AES[i]);
		}
		m_comboEncr.SetCurSel(0);
		
		GetDlgItem(IDC_COMBO_KEYINDEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEX)->EnableWindow(FALSE);

		break;

	}
}


void CNetSettingPage_Setting_DefPreferAPView::OnCbnSelendokComboEncr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR tszBuf[128] = {'\0',};
	int nSelect = m_comboEncr.GetCurSel();


	m_comboEncr.GetLBText(nSelect, tszBuf);


	// Disabled?
	if( _tcscmp(tszBuf, EncryptionStrTable_WEP_DISABLE[0]) == 0 )
	{
		GetDlgItem(IDC_COMBO_KEYINDEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NETWORK_KEY)->EnableWindow(FALSE);
	}
	// WEP?
	else if(  _tcscmp(tszBuf, EncryptionStrTable_WEP_DISABLE[1]) == 0 )
	{
		GetDlgItem(IDC_COMBO_KEYINDEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_HEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_NETWORK_KEY)->EnableWindow(TRUE);
	}

}

void CNetSettingPage_Setting_DefPreferAPView::OnApply()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	TCHAR tszIPAddr[16] = L"";
	TCHAR tszSubnetMask[16] = L"";
	TCHAR tszGateWay[16] = L"";
	TCHAR tszDnsAddr[16] = L"";
	TCHAR tszSSID[255] = L"";
	TCHAR tszNetworkKey[128] = L"";

	UpdateData(TRUE);


	GetDlgItemText(IDC_EDIT_IPADDR, tszIPAddr, sizeof(tszIPAddr) / sizeof(tszIPAddr[0]));
	GetDlgItemText(IDC_EDIT_SUBNETMASK, tszSubnetMask, sizeof(tszSubnetMask) / sizeof(tszSubnetMask[0]));
	GetDlgItemText(IDC_EDIT_GATEWAY, tszGateWay, sizeof(tszGateWay) / sizeof(tszGateWay[0]));
	GetDlgItemText(IDC_EDIT_DNS, tszDnsAddr, sizeof(tszDnsAddr) / sizeof(tszDnsAddr[0]));
	GetDlgItemText(IDC_EDIT_SSID, tszSSID, sizeof(tszSSID) / sizeof(tszSSID[0]));
	GetDlgItemText(IDC_EDIT_NETWORK_KEY, tszNetworkKey, sizeof(tszNetworkKey) / sizeof(tszNetworkKey[0]));


	m_defPrefNetInfo.bEnable = m_bEnable;
	m_defPrefNetInfo.bDHCP = m_bDHCP;
	_tcscpy(m_defPrefNetInfo.tszIPAddr, tszIPAddr );
	_tcscpy(m_defPrefNetInfo.tszSubNetMask,tszSubnetMask  );
	_tcscpy(m_defPrefNetInfo.tszGateWay, tszGateWay );
	_tcscpy(m_defPrefNetInfo.tszDnsAddr, tszDnsAddr );
	_tcscpy(m_defPrefNetInfo.tszSSID, tszSSID );
	m_defPrefNetInfo.dwAuthType = m_comboAuth.GetCurSel();
	m_defPrefNetInfo.dwEncrType = m_comboEncr.GetCurSel();
	_tcscpy(m_defPrefNetInfo.tszNetworkKey, tszNetworkKey );
	m_defPrefNetInfo.dwKeyIndex = m_comboKeyIndex.GetCurSel();
	m_defPrefNetInfo.bHexKey = m_bHex;

	UpdateData(FALSE);

	// Save setting value to XML
	SetDefPrefNetOpt(m_defPrefNetInfo);
}

void CNetSettingPage_Setting_DefPreferAPView::OnDefault()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
