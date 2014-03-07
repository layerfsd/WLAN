// GeneralDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoftAP.h"
#include "GeneralDlg.h"


// CGeneralDlg dialog

IMPLEMENT_DYNAMIC (CGeneralDlg, CDialog)

CGeneralDlg::CGeneralDlg (CWnd* pParent /*=NULL*/)
: CDialog (CGeneralDlg::IDD, pParent)
{

}

CGeneralDlg::CGeneralDlg (CConfig *pConfig, ST_GENERAL_ARG *pstGeneralArg)
: CDialog (CGeneralDlg::IDD, (CWnd*)NULL)
{
    m_pConfig = pConfig;
    m_pstGeneralArg = pstGeneralArg;
}

CGeneralDlg::~CGeneralDlg ()
{
}

void CGeneralDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange (pDX);
}


BEGIN_MESSAGE_MAP (CGeneralDlg, CDialog)
    ON_CBN_SELCHANGE (IDC_cbOpMode, &CGeneralDlg::OnCbnSelchange_cbOpMode)
    ON_EN_CHANGE(IDC_txtSSID, &CGeneralDlg::OnEnChange_txtSSID)
    ON_EN_CHANGE(IDC_txtChannel, &CGeneralDlg::OnEnChange_txtChannel)
    ON_BN_CLICKED(IDC_chHideSSID, &CGeneralDlg::OnBnClicked_chHideSSID)
    ON_BN_CLICKED(IDC_chBGMode, &CGeneralDlg::OnBnClicked_chBGMode)
    ON_EN_CHANGE(IDC_txtDTIM, &CGeneralDlg::OnEnChange_txtDTIM)
    ON_CBN_SELCHANGE (IDC_cbPublicList, &CGeneralDlg::OnCbnSelchange_cbPublicList)
    ON_EN_CHANGE(IDC_txtBeaconInterval, &CGeneralDlg::OnEnChange_txtBeaconInterval)
	ON_BN_CLICKED(IDC_chIntraBss, &CGeneralDlg::OnBnClicked_chintrabss)
END_MESSAGE_MAP ()


// CGeneralDlg message handlers
 
void CGeneralDlg :: AddPublicInterface () 
{ 
	HKEY     hTestKey;
    TCHAR    achKey[MAX_PATH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
 
    DWORD i, retCode; 

	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
        TEXT("Comm"),
        0,
        KEY_READ,
        &hTestKey) == ERROR_SUCCESS
        )
    {
		retCode = RegQueryInfoKey(
			hTestKey,                    // key handle 
			achClass,                // buffer for class name 
			&cchClassName,           // size of class string 
			NULL,                    // reserved 
			&cSubKeys,               // number of subkeys 
			&cbMaxSubKey,            // longest subkey size 
			&cchMaxClass,            // longest class string 
			&cValues,                // number of values for this key 
			&cchMaxValue,            // longest value name 
			&cbMaxValueData,         // longest value data 
			&cbSecurityDescriptor,   // security descriptor 
			&ftLastWriteTime);       // last write time 
	 
		// Enumerate the subkeys, until RegEnumKeyEx fails.
	    
		if (cSubKeys)
		{
			for (i=0; i<cSubKeys; i++) 
			{ 
				cbName = MAX_PATH;
				retCode = RegEnumKeyEx(hTestKey, i,
						 achKey, 
						 &cbName, 
						 NULL, 
						 NULL, 
						 NULL, 
						 &ftLastWriteTime); 
				if (retCode == ERROR_SUCCESS) 
				{
					if ((_tcsstr(achKey,_szVMINI1) != NULL)|| (_tcsstr (achKey,_szGPRS) != NULL))
					((CComboBox*)GetDlgItem (IDC_cbPublicList))->AddString (achKey);
				}
			}
		} 
    }   
    RegCloseKey(hTestKey);
}
BOOL CGeneralDlg::OnInitDialog ()
{
    CDialog::OnInitDialog ();

    ((CComboBox*)GetDlgItem (IDC_cbOpMode))->AddString (_T("Station"));
    ((CComboBox*)GetDlgItem (IDC_cbOpMode))->AddString (_T("SoftAP"));

    ((CSpinButtonCtrl*)GetDlgItem(IDC_spinChannel))->SetRange(_MAX_CHANNEL, _MIN_CHANNEL);
    ((CSpinButtonCtrl*)GetDlgItem(IDC_spinDTIM))->SetRange(_MAX_DTIM, _MIN_DTIM);

    ((CComboBox*)GetDlgItem (IDC_cbPublicList))->AddString (_szGW_Disabled);
    // PSK : Available public interfaces added
	
	AddPublicInterface();

    // PSK : printf to be done 
    ((CComboBox*)GetDlgItem (IDC_cbPublicList))->SetCurSel (0);

    if (enOpMode_Station == m_pConfig->m_enOpMode)
    {
        m_pstGeneralArg->nOpModeCurSel = 0;
        EnableControls (0);
        _LogStatus (_T ("Current mode is Station"));
    }
    else if (enOpMode_SoftAP == m_pConfig->m_enOpMode)
    {
        m_pstGeneralArg->nOpModeCurSel = 1;
        EnableControls (1);
        _LogStatus (_T ("Current mode is SoftAP"));
    }
    ((CComboBox*)GetDlgItem (IDC_cbOpMode))->SetCurSel (m_pstGeneralArg->nOpModeCurSel);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGeneralDlg::GetArguments ()
{
    _ClearStatus;

    m_pstGeneralArg->bInputOk = FALSE;

    switch (((CComboBox*)GetDlgItem (IDC_cbOpMode))->GetCurSel ())
    {
    case 0: //Station mode
        { 
            _LogTrace (_T ("Station mode selected"));

            if (enOpMode_Station == m_pConfig->m_enOpMode)
            {
                _LogStatus (_T ("Already in station mode."));
                ((CComboBox*)GetDlgItem (IDC_cbOpMode))->SetFocus ();
                return;
            }

            _LogStatus (_T ("Switching to station mode...please wait"));

            m_pstGeneralArg->bSoftAPMode = FALSE;//Station mode
            m_pstGeneralArg->bInputOk = TRUE;
        }
        break;

    case 1: //AP mode
        {
            memset (m_pstGeneralArg->szSSID, NULL, sizeof (m_pstGeneralArg->szSSID));
            memset (m_pstGeneralArg->szChannel, NULL, sizeof (m_pstGeneralArg->szChannel));
            memset (m_pstGeneralArg->szDTIM, NULL, sizeof (m_pstGeneralArg->szDTIM));

            _LogTrace (_T ("AP mode selected"));

            if (!GetDlgItemText (IDC_txtSSID, m_pstGeneralArg->szSSID, MAX_PATH))
            {
                AfxMessageBox (_T ("Please enter a ssid."));
                ((CEdit*)GetDlgItem (IDC_txtSSID))->SetFocus ();
                return;
            }

            if(_tcslen(m_pstGeneralArg->szSSID) > WMI_MAX_SSID_LEN)
            {
                AfxMessageBox (_T ("SSID length exceeds 32 characters limit."));
                ((CEdit*)GetDlgItem (IDC_txtSSID))->SetFocus ();
                return;
            }

            if (!GetDlgItemText (IDC_txtChannel, m_pstGeneralArg->szChannel, MAX_PATH))
            {
                AfxMessageBox (_T ("Please enter a channel."));
                ((CEdit*)GetDlgItem (IDC_txtChannel))->SetFocus ();
                return;
            }

            if (!GetDlgItemText (IDC_txtDTIM, m_pstGeneralArg->szDTIM, MAX_PATH))
            {
                AfxMessageBox (_T ("Please enter a DTIM."));
                ((CEdit*)GetDlgItem (IDC_txtDTIM))->SetFocus ();
                return;
            }
            
            TCHAR szBuffer [MAX_PATH];
            GetDlgItemText (IDC_txtBeaconInterval, szBuffer, MAX_PATH);
            m_pstGeneralArg->nBeaconInterval = (USHORT)_ttoi(szBuffer);

            if (BEACON_INTVL_MIN > m_pstGeneralArg->nBeaconInterval || BEACON_INTVL_MAX < m_pstGeneralArg->nBeaconInterval)
            {
                AfxMessageBox (_T ("Please enter a beacon interval ranging from 20 to 4000 ms."));
                SetDlgItemText (IDC_txtBeaconInterval, _T (""));
                ((CEdit*)GetDlgItem (IDC_txtBeaconInterval))->SetFocus ();
                return;
            }

            memset (m_pstGeneralArg->szPublic, NULL, sizeof (m_pstGeneralArg->szPublic));
            memset (m_pstGeneralArg->szPrivate, NULL, sizeof (m_pstGeneralArg->szPrivate));

            //By default private interface would be AR6K_SD1
            _tcscpy (m_pstGeneralArg->szPrivate, _szAR6K_SD1);

            //Retrive the selected public interface/gateway
            int iCurSel = ((CComboBox*)GetDlgItem (IDC_cbPublicList))->GetCurSel ();
            ((CComboBox*)GetDlgItem (IDC_cbPublicList))->GetLBText (iCurSel, m_pstGeneralArg->szPublic);

            //Hidden ssid enable/disable
            m_pstGeneralArg->bHiddenSSID = (((CButton*)GetDlgItem (IDC_chHideSSID))->GetCheck ()) ? TRUE : FALSE;

            //b/g protection enable/disable
            m_pstGeneralArg->bProtection = (((CButton*)GetDlgItem (IDC_chBGMode))->GetCheck ()) ? TRUE : FALSE;
			
			//intra Bss enable/disable
            m_pstGeneralArg->bIntraBss = (((CButton*)GetDlgItem (IDC_chIntraBss))->GetCheck ()) ? TRUE : FALSE;

            m_pstGeneralArg->bSoftAPMode = TRUE;//SoftAP mode
            m_pstGeneralArg->bInputOk = TRUE;
        }
        break;

    default:
        return;
    }
}

void CGeneralDlg::OnCbnSelchange_cbOpMode ()
{	
    m_pstGeneralArg->nOpModeCurSel = ((CComboBox*)GetDlgItem (IDC_cbOpMode))->GetCurSel ();
    EnableControls (m_pstGeneralArg->nOpModeCurSel);
    _ClearStatus;
    m_pstGeneralArg->bUpdate = TRUE;
}

void CGeneralDlg::EnableControls (BOOL bEnable)
{
    //SSID
    ((CEdit*)GetDlgItem (IDC_txtSSID))->EnableWindow (bEnable);
    SetDlgItemText (IDC_txtSSID, _T (""));

    //Channel
    ((CEdit*)GetDlgItem (IDC_txtChannel))->EnableWindow (bEnable);
    ((CSpinButtonCtrl*)GetDlgItem(IDC_spinChannel))->EnableWindow (bEnable);

    //DTIM
    ((CEdit*)GetDlgItem (IDC_txtDTIM))->EnableWindow (bEnable);
    ((CSpinButtonCtrl*)GetDlgItem(IDC_spinDTIM))->EnableWindow (bEnable);

    //Beacon interval in milli-seconds
    ((CEdit*)GetDlgItem (IDC_txtBeaconInterval))->EnableWindow (bEnable);
    SetDlgItemText (IDC_txtBeaconInterval, _T (""));

    //Public 
    ((CComboBox*)GetDlgItem (IDC_cbPublicList))->EnableWindow (bEnable);

    //Hidden SSID
    ((CButton*)GetDlgItem (IDC_chHideSSID))->EnableWindow (bEnable);

    //b/g Protection mode
    ((CButton*)GetDlgItem (IDC_chBGMode))->EnableWindow (bEnable);
	
	//Intra Bss
    ((CButton*)GetDlgItem (IDC_chIntraBss))->EnableWindow (bEnable);

    _ClearStatus;

    if(TRUE == bEnable)
    {
        TCHAR szBuffer[MAX_PATH];

        //SSID
        mbstowcs (szBuffer, (char*)m_pConfig->m_APModeStatus.ssid, strlen ((char*)m_pConfig->m_APModeStatus.ssid));
        szBuffer[strlen ((char*)m_pConfig->m_APModeStatus.ssid)] = NULL;
        SetDlgItemText (IDC_txtSSID, szBuffer);

        //Channel
        ((CSpinButtonCtrl*)GetDlgItem(IDC_spinChannel))->SetPos (m_pConfig->m_APModeStatus.channel);

        //DTIM
        ((CSpinButtonCtrl*)GetDlgItem(IDC_spinDTIM))->SetPos (m_pConfig->m_APModeStatus.dtim);

        //Beacon interval in milli-seconds
        memset (szBuffer, NULL, sizeof (szBuffer));
        _itot (m_pConfig->m_APModeStatus.beaconInterval, szBuffer, 10);
        SetDlgItemText (IDC_txtBeaconInterval, szBuffer);

        //Hidden SSID
        ((CButton*)GetDlgItem (IDC_chHideSSID))->SetCheck (m_pConfig->m_APModeStatus.hidden_ssid);

        //b/g protection 
        ((CButton*)GetDlgItem (IDC_chBGMode))->SetCheck (m_pConfig->m_APModeStatus.bgprotection);
		
		 //Intra Bss
        ((CButton*)GetDlgItem (IDC_chIntraBss))->SetCheck (m_pConfig->m_APModeStatus.IntraBss);

        //Public gateway interface
        ((CComboBox*)GetDlgItem (IDC_cbPublicList))->SelectString (0, m_pConfig->m_szPublicInterface);		
    }
}

void CGeneralDlg::OnEnChange_txtSSID()
{
    _ClearStatus;
    m_pstGeneralArg->bUpdate = TRUE;
}

void CGeneralDlg::OnEnChange_txtChannel()
{
    _ClearStatus;
    m_pstGeneralArg->bUpdate = TRUE;
}

void CGeneralDlg::OnBnClicked_chHideSSID()
{
    _ClearStatus;
    m_pstGeneralArg->bUpdate = TRUE;
}

void CGeneralDlg::OnBnClicked_chBGMode()
{
    _ClearStatus;
    m_pstGeneralArg->bUpdate = TRUE;
}

void CGeneralDlg::OnEnChange_txtDTIM()
{
    _ClearStatus;
    m_pstGeneralArg->bUpdate = TRUE;
}

void CGeneralDlg::OnCbnSelchange_cbPublicList()
{
    m_pstGeneralArg->bUpdate = TRUE;
}

void CGeneralDlg::OnEnChange_txtBeaconInterval()
{
    _ClearStatus;
    m_pstGeneralArg->bUpdate = TRUE;
}

void CGeneralDlg::OnBnClicked_chintrabss()
{
	_ClearStatus;
    m_pstGeneralArg->bUpdate = TRUE;
}

