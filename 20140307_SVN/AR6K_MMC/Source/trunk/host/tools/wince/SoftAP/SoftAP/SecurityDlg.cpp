// SecurityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoftAP.h"
#include "SecurityDlg.h"

#define szCipher_Disabled   _T("Disabled")
#define szCipher_WEP        _T("WEP")
#define szCipher_TKIP       _T("TKIP")
#define szCipher_AES        _T("AES")

// CSecurityDlg dialog

IMPLEMENT_DYNAMIC (CSecurityDlg, CDialog)

CSecurityDlg::CSecurityDlg (CWnd* pParent /*=NULL*/)
: CDialog (CSecurityDlg::IDD, pParent)
{

}

CSecurityDlg::CSecurityDlg (CConfig *pConfig, ST_SECURITY_ARG *pstSecurityArg)
: CDialog (CSecurityDlg::IDD, (CWnd*)NULL)
{
    m_pConfig = pConfig;
    m_pstSecurityArg = pstSecurityArg;
}

CSecurityDlg::~CSecurityDlg ()
{
}

void CSecurityDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange (pDX);
}


BEGIN_MESSAGE_MAP (CSecurityDlg, CDialog)
    ON_WM_SETFOCUS ()
    ON_CBN_SELCHANGE (IDC_cbAuth, &CSecurityDlg::OnCbnSelchange_cbAuth)
    ON_CBN_SELCHANGE (IDC_cbCipher, &CSecurityDlg::OnCbnSelchange_cbCipher)
    ON_CBN_SELCHANGE (IDC_cbKeyIndex, &CSecurityDlg::OnCbnSelchange_cbKeyIndex)
    ON_EN_CHANGE(IDC_txtKey, &CSecurityDlg::OnEnChange_txtKey)
    ON_EN_UPDATE(IDC_txtKey, &CSecurityDlg::OnEnChange_txtKey)
END_MESSAGE_MAP ()


// CSecurityDlg message handlers

BOOL CSecurityDlg::OnInitDialog ()
{
    CDialog::OnInitDialog ();

    ((CComboBox*)GetDlgItem (IDC_cbCipher))->AddString (szCipher_Disabled);
    ((CComboBox*)GetDlgItem (IDC_cbCipher))->AddString (szCipher_WEP);

    ((CComboBox*)GetDlgItem (IDC_cbAuth))->SetCurSel (0);
    ((CComboBox*)GetDlgItem (IDC_cbCipher))->SetCurSel (0);
    ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->SetCurSel (0);

    UpdateControls ();

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSecurityDlg::GetArguments ()
{
    int nKeyLen = 0;

    m_pstSecurityArg->bInputOk = FALSE;
    SetDlgItemText (IDC_lbStatus, _T (""));

    memset (m_pstSecurityArg->szKey, NULL, sizeof (m_pstSecurityArg->szKey));

    _LogTrace (_T ("Security profile update validation"));

    m_pstSecurityArg->nAuthMode = ((CComboBox*)GetDlgItem (IDC_cbAuth))->GetCurSel ();
    m_pstSecurityArg->nCipher = ((CComboBox*)GetDlgItem (IDC_cbCipher))->GetCurSel ();

    m_pstSecurityArg->nKeyIndex = ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->GetCurSel ();
    nKeyLen = GetDlgItemText (IDC_txtKey, m_pstSecurityArg->szKey, MAX_PATH);

    switch (m_pstSecurityArg->nAuthMode)
    {
    case enAuthMode_Open:
        if (enCipherMode_Disabled == m_pstSecurityArg->nCipher)//open-disabled
        {
            break;
        }
    case enAuthMode_Shared:
        {
            //Valid WEP key lengths are 5, 10, 13, 26
            switch (nKeyLen)
            {
            case _WEP40_ASCII_KEY_LEN: case _WEP104_ASCII_KEY_LEN:
            case _WEP40_HEX_KEY_LEN: case _WEP104_HEX_KEY_LEN:
                break;
            default:
                _LogTrace (_T ("Invalid key length, Please enter a valid key - ASCII(5 or 13) or HEX(10 or 26) key"));
                AfxMessageBox (_T ("Invalid key length, Please enter a valid ASCII(5 or 13) or HEX(10 or 26) key."));
                SetDlgItemText (IDC_txtKey, _T(""));
                m_pstSecurityArg->bUpdate = FALSE;
                m_pstSecurityArg->bKeyUpdate = FALSE;
                ((CEdit*)GetDlgItem (IDC_txtKey))->SetFocus ();
                return;
            }
            m_pstSecurityArg->nCipher = enCipherMode_WEP;
        }
        break;

    case enAuthMode_WPA_PSK:
    case enAuthMode_WPA2_PSK:
        {
            //Valid WPA/WPA2 key length is 8 to 64
            if ((_MIN_PSK_LEN > nKeyLen) || 
                (_MIN_PSK_LEN < nKeyLen && _MAX_PSK_LEN < nKeyLen))
            {
                _LogTrace (_T ("Invalid key length, Please enter a valid key - ASCII(8 to 63) or HEX(64) key"));
                AfxMessageBox (_T ("Invalid key length, Please enter a valid key - ASCII(8 to 63) or HEX(64) key."));
                SetDlgItemText (IDC_txtKey, _T(""));
                m_pstSecurityArg->bUpdate = FALSE;
                m_pstSecurityArg->bKeyUpdate = FALSE;
                ((CEdit*)GetDlgItem (IDC_txtKey))->SetFocus ();
                return;
            }

            if (enAuthMode_WPA_PSK == m_pstSecurityArg->nAuthMode)
            {
                m_pstSecurityArg->nCipher = enCipherMode_TKIP;
            }
            else
            {
                m_pstSecurityArg->nCipher = enCipherMode_AES;
            }
        }
        break;

    default:
        printf (_Trace ("Invalid auth mode: %u"), m_pstSecurityArg->nAuthMode);
        _LogStatus (_T ("Invalid authentication mode"));
        SetDlgItemText (IDC_txtKey, _T(""));
        m_pstSecurityArg->bUpdate = FALSE;
        m_pstSecurityArg->bKeyUpdate = FALSE;
        ((CEdit*)GetDlgItem (IDC_cbAuth))->SetFocus ();
        return;
    }
    m_pstSecurityArg->bInputOk = TRUE;
}

void CSecurityDlg::OnCbnSelchange_cbAuth ()
{
    int nAuth = ((CComboBox*)GetDlgItem (IDC_cbAuth))->GetCurSel ();
    CComboBox *pcbCipher = (CComboBox*)GetDlgItem (IDC_cbCipher);

    while (pcbCipher->GetCount ())
        pcbCipher->DeleteString (0);

    switch (nAuth)
    {
    case enAuthMode_Open:
        pcbCipher->AddString (szCipher_Disabled);
        pcbCipher->AddString (szCipher_WEP);
        break;
    case enAuthMode_Shared:
        pcbCipher->AddString (szCipher_WEP);
        break;
    case enAuthMode_WPA_PSK:
        pcbCipher->AddString (szCipher_TKIP);
        break;
    case enAuthMode_WPA2_PSK:
        pcbCipher->AddString (szCipher_AES);
        break;
    default:
        return;
    }
    pcbCipher->SetCurSel (0);
    m_pstSecurityArg->bUpdate = TRUE;
    OnCbnSelchange_cbCipher ();
}

void CSecurityDlg::OnCbnSelchange_cbCipher ()
{
    TCHAR szCurSel [MAX_PATH] = {0,};

    int iCurSel = ((CComboBox*)GetDlgItem (IDC_cbCipher))->GetCurSel ();
    ((CComboBox*)GetDlgItem (IDC_cbCipher))->GetLBText (iCurSel, szCurSel);

    if (0 == _tcscmp (szCipher_Disabled, szCurSel))
    {
        ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->EnableWindow (0);
        ((CEdit*)GetDlgItem (IDC_txtKey))->EnableWindow (0);
        m_pstSecurityArg->nCipher = enCipherMode_Disabled;
    }
    else // wep, tkip, aes
    {
        ((CEdit*)GetDlgItem (IDC_txtKey))->EnableWindow (1);

        if (0 == _tcscmp (szCipher_TKIP, szCurSel))
        {	
            m_pstSecurityArg->nCipher = enCipherMode_TKIP;
            ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->SetCurSel (0);
            ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->EnableWindow (0);
        }
        else if (0 == _tcscmp (szCipher_AES, szCurSel) )
        {
            m_pstSecurityArg->nCipher = enCipherMode_AES;
            ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->SetCurSel (0);
            ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->EnableWindow (0);
        }
        else
        {
            m_pstSecurityArg->nCipher = enCipherMode_WEP;
            ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->EnableWindow (1);
        }
    }
    SetDlgItemText (IDC_txtKey, _T(""));
    m_pstSecurityArg->bUpdate = FALSE;
    m_pstSecurityArg->bKeyUpdate = FALSE;
    SetDlgItemText (IDC_lbStatus, _T(""));

    m_pstSecurityArg->bUpdate = TRUE;
}

void CSecurityDlg::EnableControls (BOOL bEnable)
{
    ((CComboBox*)GetDlgItem (IDC_cbAuth))->EnableWindow (bEnable);
    ((CComboBox*)GetDlgItem (IDC_cbCipher))->EnableWindow (bEnable);
    ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->EnableWindow (bEnable);
    ((CEdit*)GetDlgItem (IDC_txtKey))->EnableWindow (bEnable);
}

void CSecurityDlg::OnCbnSelchange_cbKeyIndex ()
{
    SetDlgItemText (IDC_txtKey, _T (""));
    SetDlgItemText (IDC_lbStatus, _T(""));
    m_pstSecurityArg->bKeyUpdate = FALSE;
}

void CSecurityDlg::OnSetFocus (CWnd* pOldWnd)
{
    CDialog::OnSetFocus (pOldWnd);

    UpdateControls ();
}

void CSecurityDlg::UpdateControls ()
{
    if ((*m_pstSecurityArg->pnOpModeCurSel))
    {
        EnableControls (TRUE);

        CComboBox *pcbCipher = (CComboBox*)GetDlgItem (IDC_cbCipher);

        while (pcbCipher->GetCount ())
            pcbCipher->DeleteString (0);

        if (NONE_CRYPT == m_pConfig->m_APCryptStatus.cryptType)
        {
            //Open disabled
            ((CComboBox*)GetDlgItem (IDC_cbAuth))->SetCurSel (0);
            pcbCipher->AddString (szCipher_Disabled);
            pcbCipher->AddString (szCipher_WEP);
            pcbCipher->SetCurSel (0);
        }
        else if (WEP_CRYPT == m_pConfig->m_APCryptStatus.cryptType)
        {
            //Open/Shared, WEP
            if (OPEN_AUTH == m_pConfig->m_APCryptStatus.dot11_authmode)
            {
                ((CComboBox*)GetDlgItem (IDC_cbAuth))->SetCurSel (0);//Open
                pcbCipher->AddString (szCipher_Disabled);
                pcbCipher->AddString (szCipher_WEP);
                pcbCipher->SetCurSel (1);//WEP
            }
            else
            {
                ((CComboBox*)GetDlgItem (IDC_cbAuth))->SetCurSel (1);//Shared
                pcbCipher->AddString (szCipher_WEP);
                pcbCipher->SetCurSel (0);//WEP
            }
        }
        else if (TKIP_CRYPT == m_pConfig->m_APCryptStatus.cryptType)
        {
            ((CComboBox*)GetDlgItem (IDC_cbAuth))->SetCurSel (2);//WPA-PSK
            pcbCipher->AddString (szCipher_TKIP);
            pcbCipher->SetCurSel (0);
            ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->SetCurSel (0);
            ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->EnableWindow (0);
        }
        else if (AES_CRYPT == m_pConfig->m_APCryptStatus.cryptType)
        {
            ((CComboBox*)GetDlgItem (IDC_cbAuth))->SetCurSel (3);//WPA2-PSK
            pcbCipher->AddString (szCipher_AES);
            pcbCipher->SetCurSel (0);
            ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->SetCurSel (0);
            ((CComboBox*)GetDlgItem (IDC_cbKeyIndex))->EnableWindow (0);
        }
        SetDlgItemText (IDC_txtKey, _T(""));
        m_pstSecurityArg->bUpdate = FALSE;
        m_pstSecurityArg->bKeyUpdate = FALSE;
        SetDlgItemText (IDC_lbStatus, _T(""));
        m_pstSecurityArg->bUpdate = FALSE;
    }
    else
    {
        EnableControls (FALSE);
    }
}

void CSecurityDlg::OnEnChange_txtKey()
{
    m_pstSecurityArg->bKeyUpdate = TRUE;
    m_pstSecurityArg->bUpdate = TRUE;
}
