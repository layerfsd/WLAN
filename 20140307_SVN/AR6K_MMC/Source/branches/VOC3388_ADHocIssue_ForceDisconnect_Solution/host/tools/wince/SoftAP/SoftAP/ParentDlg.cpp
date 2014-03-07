// ParentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoftAP.h"
#include "ParentDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _LogGenDlgSts(x)	{ ((CGeneralDlg*) m_ptbParent->m_pTabDlgs[_GENERAL_DLG_INDEX])->SetDlgItemText (IDC_lbStatus, x); _LogTrace(x); }
#define _LogGenDlgErr(x)	{TCHAR szErr [MAX_PATH]; _stprintf (szErr, _T ("%s, %u"), x, GetLastError ()); _LogGenDlgSts (szErr);}

#define _LogSecDlgSts(x)	{ ((CSecurityDlg*) m_ptbParent->m_pTabDlgs[_SECURITY_DLG_INDEX])->SetDlgItemText (IDC_lbStatus, x); _LogTrace(x); \
                              ((CGeneralDlg*) m_ptbParent->m_pTabDlgs[_GENERAL_DLG_INDEX])->SetDlgItemText (IDC_lbStatus, x);}
#define _LogSecDlgErr(x)	{TCHAR szErr [MAX_PATH]; _stprintf (szErr, _T ("%s, %u"), x, GetLastError ()); _LogSecDlgSts (szErr);}

// CParentDlg dialog

CParentDlg::CParentDlg (CWnd* pParent /*=NULL*/)
: CDialog (CParentDlg::IDD, pParent)
{
    printf (_Trace ("SoftAP Manager started..."));

    m_hIcon = AfxGetApp ()->LoadIcon (IDR_MAINFRAME);

    m_ptbParent = NULL;

    m_stParentArg.stGeneralArg.bHiddenSSID = FALSE;
    m_stParentArg.stGeneralArg.bInputOk = FALSE;
    m_stParentArg.stGeneralArg.bProtection = FALSE;
	m_stParentArg.stGeneralArg.bIntraBss = FALSE;
    m_stParentArg.stGeneralArg.bSoftAPMode = FALSE;
    m_stParentArg.stGeneralArg.bUpdate = FALSE;
    m_stParentArg.stGeneralArg.nOpModeCurSel = 0;

    memset (m_stParentArg.stGeneralArg.szChannel, NULL, sizeof (m_stParentArg.stGeneralArg.szChannel));
    memset (m_stParentArg.stGeneralArg.szDTIM, NULL, sizeof (m_stParentArg.stGeneralArg.szDTIM));
    memset (m_stParentArg.stGeneralArg.szPrivate, NULL, sizeof (m_stParentArg.stGeneralArg.szPrivate));
    memset (m_stParentArg.stGeneralArg.szPublic, NULL, sizeof (m_stParentArg.stGeneralArg.szPublic));
    memset (m_stParentArg.stGeneralArg.szSSID, NULL, sizeof (m_stParentArg.stGeneralArg.szSSID));

    m_stParentArg.stSecurityArg.bInputOk = FALSE;
    m_stParentArg.stSecurityArg.bSoftAPMode = FALSE;
    m_stParentArg.stSecurityArg.nAuthMode = 0;
    m_stParentArg.stSecurityArg.nCipher = 0;
    m_stParentArg.stSecurityArg.nKeyIndex = 0;
    memset (m_stParentArg.stSecurityArg.szKey, NULL, sizeof (m_stParentArg.stSecurityArg.szKey));
    m_stParentArg.stSecurityArg.bUpdate = FALSE;
    m_stParentArg.stSecurityArg.bKeyUpdate = FALSE;
    m_stParentArg.stSecurityArg.pnOpModeCurSel = &m_stParentArg.stGeneralArg.nOpModeCurSel;

    m_stParentArg.pConfig = new CConfig ();
}

CParentDlg::~CParentDlg ()
{
    if (m_stParentArg.pConfig)
    {
        delete m_stParentArg.pConfig;
    }

    if (m_ptbParent)
    {
        delete m_ptbParent;
    }

    printf (_Trace ("SoftAP Manager exits..."));
}

void CParentDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange (pDX);
    DDX_Control (pDX, IDC_tabParent, *m_ptbParent);
}

BEGIN_MESSAGE_MAP (CParentDlg, CDialog)
    ON_BN_CLICKED (IDC_bnCommitMain, &CParentDlg::OnBnClicked_bnCommitMain)
#if defined (_DEVICE_RESOLUTION_AWARE) && !defined (WIN32_PLATFORM_WFSP)
    ON_WM_SIZE ()
#endif
    //}}AFX_MSG_MAP
END_MESSAGE_MAP ()


// CParentDlg message handlers

BOOL CParentDlg::OnInitDialog ()
{
    m_stParentArg.hbnCommitMain = ((CButton*)GetDlgItem (IDC_bnCommitMain))->m_hWnd;
    m_ptbParent = new CParentTab (&m_stParentArg);

    CDialog::OnInitDialog ();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon (m_hIcon, TRUE);			// Set big icon
    SetIcon (m_hIcon, FALSE);		// Set small icon

    enOpMode OpMode = m_stParentArg.pConfig->GetCurrentOpMode ();

    if (enOpMode_Station == OpMode)
    {
        _LogTrace (_T ("Current mode is Station"));
    }
    else if (enOpMode_SoftAP == OpMode)
    {
        if(FALSE == m_stParentArg.pConfig->GetAPModeStatus ())
        {
            _LogTrace (_T ("AP mode status query failed."));
            AfxMessageBox (_T ("AP mode status query failed, application will exit."));
            PostQuitMessage (0);
            return FALSE;
        }

        if(FALSE == m_stParentArg.pConfig->GetAPCryptStatus ())
        {
            _LogTrace (_T ("AP Crypt status query failed."));
            AfxMessageBox (_T ("AP Crypt status query failed, application will exit."));
            PostQuitMessage (0);
            return FALSE;
        }

        _LogTrace (_T ("Current mode is SoftAP"));
    }
    else
    {
        CDialog::ShowWindow (SW_HIDE);
        _LogTrace (_T ("SoftAP module not found, application will exit."));
        AfxMessageBox (_T ("SoftAP module not found, application will exit."));
        PostQuitMessage (0);
        return FALSE;
    }

    m_ptbParent->InitDialogs ();

    return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined (_DEVICE_RESOLUTION_AWARE) && !defined (WIN32_PLATFORM_WFSP)
void CParentDlg::OnSize (UINT /*nType*/, int /*cx*/, int /*cy*/)
{
    if (AfxIsDRAEnabled ())
    {
        DRA::RelayoutDialog(
            AfxGetResourceHandle(), 
            this->m_hWnd, 
            DRA::GetDisplayMode() != DRA::Portrait ? 
            MAKEINTRESOURCE(IDD_PARENT_DLG) : 
        MAKEINTRESOURCE(IDD_PARENT_DLG));
    }
}
#endif

void CParentDlg::OnBnClicked_bnCommitMain ()
{
    _LogGenDlgSts (_T (""));
    ((CButton*)GetDlgItem (IDC_bnCommitMain))->EnableWindow (0);

    if (m_stParentArg.stGeneralArg.bUpdate)//Update only if any arguments changed
    {
        m_stParentArg.stGeneralArg.bUpdate = FALSE;
        m_stParentArg.stGeneralArg.bInputOk = FALSE;
        ((CGeneralDlg*) m_ptbParent->m_pTabDlgs[_GENERAL_DLG_INDEX])->GetArguments ();

        if ( ! m_stParentArg.stGeneralArg.bSoftAPMode)//if Station mode selected
        {
            if (m_stParentArg.stGeneralArg.bInputOk)//If all input params are valid
            {
                if ( ! m_stParentArg.pConfig->SetStationConfig ())
                {
                    _LogGenDlgErr (_T ("Station mode config failed"));
                    ((CButton*)GetDlgItem (IDC_bnCommitMain))->EnableWindow (1);
                    return;
                }
                _LogTrace (_T ("Station mode config success"));

                //Station mode
                if (m_stParentArg.pConfig->SetStationMode ())
                {
                    _LogGenDlgSts (_T ("Station mode switch success"));
                }
                else
                {		
                    _LogGenDlgErr (_T ("Station mode switch failed"));
                }
            }
            ((CButton*)GetDlgItem (IDC_bnCommitMain))->EnableWindow (1);
            return;
        }

        if (m_stParentArg.stGeneralArg.bInputOk)//If all input params are valid
        {
            BOOL bUpdateAPMode = TRUE;
            BOOL bUpdateConfig = TRUE;

            if (enOpMode_SoftAP == m_stParentArg.pConfig->m_enOpMode)
            {
                bUpdateAPMode = FALSE;
                bUpdateConfig = FALSE;

                //SSID
                TCHAR szSSID[MAX_PATH];
                mbstowcs (szSSID, (char*)m_stParentArg.pConfig->m_APModeStatus.ssid, strlen ((char*)m_stParentArg.pConfig->m_APModeStatus.ssid));
                szSSID[strlen ((char*)m_stParentArg.pConfig->m_APModeStatus.ssid)] = NULL;

                if (_tcscmp (m_stParentArg.stGeneralArg.szSSID, szSSID))
                {
                    bUpdateAPMode = TRUE;
                }

                //Public interface/gateway
                if (_tcscmp (m_stParentArg.stGeneralArg.szPublic, m_stParentArg.pConfig->m_szPublicInterface))
                {
                    bUpdateConfig = TRUE;
                }
            }
            if (bUpdateAPMode)
            {
                if (enAuthMode_Open != m_stParentArg.stSecurityArg.nAuthMode ||
                    (enAuthMode_Open == m_stParentArg.stSecurityArg.nAuthMode && 
                    enCipherMode_Disabled != m_stParentArg.stSecurityArg.nCipher))
                {
                    ((CSecurityDlg*) m_ptbParent->m_pTabDlgs[_SECURITY_DLG_INDEX])->GetArguments ();
                    if ( ! m_stParentArg.stSecurityArg.bInputOk)//If any input param is not valid
                    {
                        m_stParentArg.stGeneralArg.bUpdate = TRUE;
                        m_stParentArg.stSecurityArg.bUpdate = TRUE;
                        ((CButton*)GetDlgItem (IDC_bnCommitMain))->EnableWindow (1);
                        return;
                    }
                }
            }
            APModeUpdate (bUpdateConfig, bUpdateAPMode);
        }//end-if all input params are valid

    }//end-if update only if arguments changed
    else if (enOpMode_Station == m_stParentArg.pConfig->m_enOpMode)
    {
        ((CButton*)GetDlgItem (IDC_bnCommitMain))->EnableWindow (1);
        return;//return if station
    }

    if (m_stParentArg.stSecurityArg.bUpdate)
        SecurityUpdate ();

    ((CButton*)GetDlgItem (IDC_bnCommitMain))->EnableWindow (1);
}

void CParentDlg::APModeUpdate (BOOL bUpdateConfig, BOOL bUpdateAPMode)
{
    if (bUpdateConfig)
    {
        if (FALSE == m_stParentArg.pConfig->SetAPConfig (
            m_stParentArg.stGeneralArg.szPrivate,                                                
            m_stParentArg.stGeneralArg.szPublic,
            bUpdateAPMode))
        {
            _LogGenDlgErr ( _T("SoftAP mode config failed"));
            return;
        }
        _LogGenDlgSts (_T ("SoftAP mode config success"));
    }

    if (bUpdateAPMode)
    {
        _LogGenDlgSts ( _T("Switching to SoftAP mode...please wait"));
        if (FALSE == m_stParentArg.pConfig->SetAPMode (
            m_stParentArg.stGeneralArg.szSSID, 
            m_stParentArg.stGeneralArg.szChannel, 
            m_stParentArg.stGeneralArg.szDTIM,
            m_stParentArg.stGeneralArg.nBeaconInterval,
            m_stParentArg.stGeneralArg.bHiddenSSID ,
            m_stParentArg.stGeneralArg.bProtection,
			m_stParentArg.stGeneralArg.bIntraBss))
        {
            _LogGenDlgErr (_T ("SoftAP mode switch failed"));
            return;
        }
        m_stParentArg.stSecurityArg.bUpdate = TRUE;
        _LogGenDlgSts (_T ("SoftAP mode switch success"));
        _LogSecDlgSts (_T ("SoftAP mode switch success"));

        return;
    }
    else
    {
        TCHAR szStatus[MAX_PATH];
        memset (szStatus, NULL, sizeof (szStatus));

        if (_ttoi (m_stParentArg.stGeneralArg.szChannel) != m_stParentArg.pConfig->m_APModeStatus.channel)
        {
            //Update Channel
            if(FALSE == m_stParentArg.pConfig->SetChannel (m_stParentArg.stGeneralArg.szChannel))
            {
                _LogGenDlgErr (_T ("Channel update failed"));
                return;
            }
            else
            {
                _LogGenDlgSts (_T ("Channel update success"));
                _tcscat (szStatus, _T("Channel,"));
            }
        }

        if (_ttoi (m_stParentArg.stGeneralArg.szDTIM) != m_stParentArg.pConfig->m_APModeStatus.dtim)
        {
            //Update DTIM
            if(FALSE == m_stParentArg.pConfig->SetDTIM (m_stParentArg.stGeneralArg.szDTIM))
            {
                _LogGenDlgErr (_T ("DTIM update failed"));
                return;
            }
            else
            {
                _LogGenDlgSts (_T ("DTIM update success"));
                _tcscat (szStatus, _T("DTIM,"));
            }
        }

        if (m_stParentArg.stGeneralArg.nBeaconInterval != m_stParentArg.pConfig->m_APModeStatus.beaconInterval)
        {
            //Update BeaconInterval
            if(FALSE == m_stParentArg.pConfig->SetBeaconInterval (m_stParentArg.stGeneralArg.nBeaconInterval))
            {
                _LogGenDlgErr (_T ("BeaconInterval update failed"));
                return;
            }
            else
            {
                _LogGenDlgSts (_T ("BeaconInterval update success"));
                _tcscat (szStatus, _T("Beacon,"));
            }
        }

        if (m_stParentArg.stGeneralArg.bHiddenSSID != m_stParentArg.pConfig->m_APModeStatus.hidden_ssid)
        {
            //Hidden ssid enable/disable
            if(FALSE == m_stParentArg.pConfig->SetHiddenSsid (m_stParentArg.stGeneralArg.bHiddenSSID))
            {
                _LogGenDlgErr (_T ("Hidden ssid update failed"));
                return;
            }
            else
            {
                _LogGenDlgSts (_T ("Hidden ssid update success"));
                _tcscat (szStatus, _T("Hidden,"));
            }
        }

	    if (m_stParentArg.stGeneralArg.bIntraBss != m_stParentArg.pConfig->m_APModeStatus.IntraBss)
        {
            //Intra BSS comm enable/disable
            if(FALSE == m_stParentArg.pConfig->SetIntraBss (m_stParentArg.stGeneralArg.bIntraBss))
            {
                _LogGenDlgErr (_T ("Intra BSS comm update failed"));
                return;
            }
            else
            {
                _LogGenDlgSts (_T ("Intra BSS comm update success"));
                _tcscat (szStatus, _T("IntraBss,"));
            }
        }

        if (m_stParentArg.stGeneralArg.bProtection != m_stParentArg.pConfig->m_APModeStatus.bgprotection)
        {
            //BG protection enable/disable
            if(FALSE == m_stParentArg.pConfig->SetBGProtection(m_stParentArg.stGeneralArg.bProtection))
            {
                _LogGenDlgErr (_T ("BG protection update failed"));
                return;
            }
            else
            {
                _LogGenDlgSts (_T ("BG protection update success"));
                _tcscat (szStatus, _T("b/gMode"));
            }
        }

        if ( _tcslen (szStatus) > 0)
        {
            _tcscat (szStatus, _T("-updated"));
            _LogGenDlgSts (szStatus);
        }
    }
}

void CParentDlg::SecurityUpdate ()
{
    BOOL bUpdateSecurity = FALSE;

    m_stParentArg.stSecurityArg.bInputOk = FALSE;
    ((CSecurityDlg*) m_ptbParent->m_pTabDlgs[_SECURITY_DLG_INDEX])->GetArguments ();

    if (m_stParentArg.stSecurityArg.bInputOk)//If all input params are valid
    {
        switch (m_stParentArg.stSecurityArg.nAuthMode)//Current crypt type
        {
        case enAuthMode_Open:
            if (enCipherMode_Disabled == m_stParentArg.stSecurityArg.nCipher)
            {
                if (NONE_CRYPT != m_stParentArg.pConfig->m_APCryptStatus.cryptType)
                {
                    //DisableSecurity
                    bUpdateSecurity = TRUE;
                    break;
                }
                else
                {
                    _LogTrace (_T ("Already Security is Open-Disabled"));
                    return;
                }
            }

        case enAuthMode_Shared:
            if (NONE_AUTH != m_stParentArg.pConfig->m_APCryptStatus.Authmode ||
                WEP_CRYPT != m_stParentArg.pConfig->m_APCryptStatus.cryptType)
            {
                bUpdateSecurity = TRUE;
            }
            else
            {
                UINT IsShared = (enAuthMode_Shared == m_stParentArg.stSecurityArg.nAuthMode) ? SHARED_AUTH : OPEN_AUTH;
                if (IsShared != m_stParentArg.pConfig->m_APCryptStatus.dot11_authmode ||
                    m_stParentArg.stSecurityArg.bKeyUpdate)
                {
                    bUpdateSecurity = TRUE;
                }
            }	
            break;

        case enAuthMode_WPA_PSK:
            if (WPA_PSK_AUTH != m_stParentArg.pConfig->m_APCryptStatus.Authmode ||
                TKIP_CRYPT != m_stParentArg.pConfig->m_APCryptStatus.cryptType)
            {
                bUpdateSecurity = TRUE;
            }
            else
            {
                bUpdateSecurity = m_stParentArg.stSecurityArg.bKeyUpdate;
            }
            break;

        case enAuthMode_WPA2_PSK:
            if (WPA2_PSK_AUTH != m_stParentArg.pConfig->m_APCryptStatus.Authmode ||
                AES_CRYPT != m_stParentArg.pConfig->m_APCryptStatus.cryptType)
            {
                bUpdateSecurity = TRUE;
            }
            else
            {
                bUpdateSecurity = m_stParentArg.stSecurityArg.bKeyUpdate;
            }
            break;

        default:
            return;
        }

        m_stParentArg.stSecurityArg.bUpdate = FALSE;
        m_stParentArg.stSecurityArg.bKeyUpdate = FALSE;

        if (bUpdateSecurity)
        {
            _LogStatus (_T ("Updating security profile..."));
            if (m_stParentArg.pConfig->SetSecurity (
                (enAuthMode)m_stParentArg.stSecurityArg.nAuthMode,
                (enCipherMode)m_stParentArg.stSecurityArg.nCipher, 
                m_stParentArg.stSecurityArg.nKeyIndex, 
                m_stParentArg.stSecurityArg.szKey))
            {
                _LogSecDlgSts (_T ("Profile update success"));
            }
            else
            {
                _LogSecDlgErr (_T ("Security profile update failed"));
            }
        }
    }
}


