// MiscDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoftAP.h"
#include "MiscDlg.h"


// CMiscDlg dialog

IMPLEMENT_DYNAMIC (CMiscDlg, CDialog)

CMiscDlg::CMiscDlg (CWnd* pParent /*=NULL*/)
: CDialog(CMiscDlg::IDD, pParent)
{

}

CMiscDlg::CMiscDlg (CConfig *pConfig)
: CDialog (CMiscDlg::IDD, (CWnd*)NULL)
{
    m_pConfig = pConfig;

}

CMiscDlg::~CMiscDlg ()
{
}

void CMiscDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange (pDX);
}


BEGIN_MESSAGE_MAP (CMiscDlg, CDialog)
    ON_WM_SETFOCUS ()
    ON_BN_CLICKED (IDC_chCountry, &CMiscDlg::OnBnClicked_chCountry)
    ON_EN_CHANGE(IDC_txtMaxStations, &CMiscDlg::OnEnChange_txtMaxStations)
    ON_BN_CLICKED(IDC_bnCountryCode, &CMiscDlg::OnBnClicked_bnCountryCode)
    ON_BN_CLICKED(IDC_bnMaxStation, &CMiscDlg::OnBnClicked_bnMaxStation)
    ON_BN_CLICKED(IDC_bnAddMAC, &CMiscDlg::OnBnClicked_bnAddMac)
    ON_BN_CLICKED(IDC_bnDelMAC, &CMiscDlg::OnBnClicked_bnDelMac)
    ON_BN_CLICKED(IDC_bnRefresh, &CMiscDlg::OnBnClicked_bnRefresh)
    ON_BN_CLICKED(IDC_bnAclPolicy, &CMiscDlg::OnBnClicked_bnAclpolicy)
END_MESSAGE_MAP ()

// CMiscDlg message handlers

BOOL CMiscDlg::OnInitDialog ()
{
    CDialog::OnInitDialog ();

    ((CSpinButtonCtrl*)GetDlgItem(IDC_spinMaxStations))->SetRange(_MAX_STATIONS, _MIN_STATIONS);

    //Country codes
    CComboBox *pcbCountry =  (CComboBox*)GetDlgItem (IDC_cbCountry);
    while (pcbCountry->GetCount ())
        pcbCountry->DeleteString (0);

    for (int i = 0; i < g_nMaxCountryCode; i++)
        pcbCountry->AddString(g_stCountryCodeTable[i].szName);

    if ( ! m_pConfig->GetCountryCode())
        _LogTrace (_T("GetCountryCode failed"));

    EnableControls (FALSE);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMiscDlg::OnBnClicked_chCountry ()
{
    if(((CButton*)GetDlgItem (IDC_chCountry))->GetCheck () == BST_CHECKED)
    {
        ((CComboBox*)GetDlgItem (IDC_cbCountry))->EnableWindow (1);

        if ( ! _tcscmp (m_pConfig->m_szCountryCode, _szDisabledCountrycode))
        {
            ((CComboBox*)GetDlgItem (IDC_cbCountry))->SelectString (0, _szDefaultCountryCode);
        }
        else
        {
            ((CComboBox*)GetDlgItem (IDC_cbCountry))->SelectString (0, m_pConfig->m_szCountryCode);
        }
    }
    else
    {
        ((CComboBox*)GetDlgItem (IDC_cbCountry))->EnableWindow (0);
    }
}

void CMiscDlg::OnSetFocus (CWnd* pOldWnd)
{
    CDialog::OnSetFocus (pOldWnd);

    if (enOpMode_SoftAP == m_pConfig->m_enOpMode)
    {
        EnableControls (TRUE);
    }
    else
    {
        EnableControls (FALSE);
    }
}

void CMiscDlg::EnableControls(BOOL bEnable)
{
    ((CButton*)GetDlgItem (IDC_chCountry))->EnableWindow (bEnable);
    ((CComboBox*)GetDlgItem (IDC_cbCountry))->EnableWindow (bEnable);
    ((CButton*)GetDlgItem (IDC_bnCountryCode))->EnableWindow (bEnable);

    ((CSpinButtonCtrl*)GetDlgItem(IDC_spinMaxStations))->EnableWindow (bEnable);
    ((CButton*)GetDlgItem (IDC_bnMaxStation))->EnableWindow (bEnable);
    ((CEdit*)GetDlgItem (IDC_txtMaxStations))->EnableWindow (bEnable);

    ((CComboBox*)GetDlgItem (IDC_cbMACFilter))->EnableWindow (bEnable);
    ((CButton*)GetDlgItem (IDC_bnAddMAC))->EnableWindow (bEnable);
    ((CButton*)GetDlgItem (IDC_bnDelMAC))->EnableWindow (bEnable);

    ((CComboBox*)GetDlgItem (IDC_cbStattions))->EnableWindow (bEnable);
    ((CButton*)GetDlgItem (IDC_bnRefresh))->EnableWindow (bEnable);
    ((CComboBox*)GetDlgItem (IDC_cbACLPolicy))->EnableWindow (bEnable);
    ((CButton*)GetDlgItem (IDC_bnAclPolicy))->EnableWindow (bEnable);

    _ClearStatus;

    if(TRUE == bEnable)
    {
        if ( ! _tcscmp (m_pConfig->m_szCountryCode, _szDisabledCountrycode))
            ((CButton*)GetDlgItem (IDC_chCountry))->SetCheck (0);
        else
            ((CButton*)GetDlgItem (IDC_chCountry))->SetCheck (1);

        OnBnClicked_chCountry ();

        m_pConfig->GetStationLimit();
        printf ( _Trace ("Station limit= %u, Max limit= %u"), m_pConfig->m_nStationLimit, m_pConfig->m_nMaxStationLimit);
        ((CSpinButtonCtrl*)GetDlgItem(IDC_spinMaxStations))->SetRange(m_pConfig->m_nMaxStationLimit, _MIN_STATIONS);
        ((CSpinButtonCtrl*)GetDlgItem(IDC_spinMaxStations))->SetPos(m_pConfig->m_nStationLimit);

        RefreshMacFilter (TRUE);
        OnBnClicked_bnRefresh ();
    }
}

void CMiscDlg::OnEnChange_txtMaxStations()
{
    _ClearStatus;
}

void CMiscDlg::OnBnClicked_bnCountryCode()
{
    //update country code
    int iCheck = ((CButton*)GetDlgItem (IDC_chCountry))->GetCheck ();

    TCHAR szCountryCode[3];
    if(BST_CHECKED == iCheck)
    {
        int iCurSel = ((CComboBox*)GetDlgItem (IDC_cbCountry))->GetCurSel ();
        ((CComboBox*)GetDlgItem (IDC_cbCountry))->GetLBText(iCurSel, szCountryCode);
    }
    else
        _tcscpy(szCountryCode, _T(WMI_DISABLE_REGULATORY_CODE));

    if (0 == _tcscmp (m_pConfig->m_szCountryCode, szCountryCode))
    {
        return;//no change in country code
    }

    _LogStatus (_T ("Updating Country code... please wait"));

    if (FALSE == m_pConfig->SetCountryCode (szCountryCode))
    {
        _LogError (_T ("Country code update failed"));
        return;
    }
    else
    {
        _LogStatus (_T ("Country code updated"));
    }
}

void CMiscDlg::OnBnClicked_bnMaxStation()
{
    //update the station limit
    TCHAR szMaxStations[MAX_PATH] = {0, };

    GetDlgItemText (IDC_txtMaxStations, szMaxStations, MAX_PATH);

    if (m_pConfig->m_nStationLimit == _ttoi (szMaxStations))
    {
        return;//no change in station limit
    }

    _LogStatus (_T ("Updating station limit... please wait"));

    if (FALSE == m_pConfig->SetStationLimit (szMaxStations))
    {
        _LogError (_T ("Station limit update failed"));
        return;
    }
    else
    {
        _LogStatus (_T ("Station limit updated"));
    }
}

void CMiscDlg::OnBnClicked_bnAddMac()
{
    TCHAR szMAC[MAX_PATH];
    char szBuffer[MAX_PATH];
    A_UINT8 szMACFilter[ATH_MAC_LEN], cWildcard;

    memset(szMAC, NULL, sizeof (szMAC));

    GetDlgItemText(IDC_cbMACFilter, szMAC, MAX_PATH);

    if (! _tcscmp(szMAC, _T("00:00:00:00:00:00")) ||//When passed this mac addr, ar6k ioctl fails
        (FALSE == m_pConfig->wmic_ether_aton_wild(szMAC, szMACFilter, &cWildcard)))
    {
        _LogStatus (_T ("Please enter a valid MAC"));
        AfxMessageBox(_T("Please enter a valid MAC."));
        RefreshMacFilter(FALSE);
        return;
    }

    memset(szBuffer, NULL, sizeof (szBuffer));
    wcstombs(szBuffer, szMAC, _tcslen (szMAC));
    szBuffer[_tcslen (szMAC)] = NULL;

    for (int i = 0; i < AP_ACL_SIZE; i++)
    {
        if (! strcmp (szBuffer, m_pConfig->m_szACLList[i]))
        {
            _LogStatus (_T ("MAC already exists in filter list"));
            AfxMessageBox(_T("MAC already exists in filter list."));
            RefreshMacFilter(FALSE);
            return;
        }
    }

    _LogStatus (_T ("Adding MAC filter... please wait"));

    if (FALSE == m_pConfig->UpdateACL (szMAC, ADD_MAC_ADDR))
    {
        _LogError (_T ("MAC filter add failed"));
    }
    else
    {
        _LogStatus (_T ("MAC filter added"));
    }
    RefreshMacFilter(TRUE);
}

void CMiscDlg::OnBnClicked_bnDelMac()
{
    TCHAR szMAC[MAX_PATH];
    char szDelMAC[MAX_PATH];
    int nDelIndex = MAX_PATH;

    memset(szMAC, NULL, sizeof (szMAC));

    if (0 == ((CComboBox*)GetDlgItem (IDC_cbMACFilter))->GetCount ())
    {
        _LogStatus (_T ("MAC filters not present to delete"));
        AfxMessageBox(_T("MAC filters not present to delete."));
        return;
    }

    GetDlgItemText(IDC_cbMACFilter, szMAC, MAX_PATH);

    int nCurSel = ((CComboBox*)GetDlgItem (IDC_cbMACFilter))->GetCurSel ();

    memset(szDelMAC, NULL, sizeof (szDelMAC));
    wcstombs(szDelMAC, szMAC, _tcslen (szMAC));
    szDelMAC[_tcslen (szMAC)] = NULL;

    for (int i = 0; i < AP_ACL_SIZE; i++)
    {
        if (! strcmp (szDelMAC, m_pConfig->m_szACLList[i]))
        {
            nDelIndex = i;
            break;
        }
    }

    if (MAX_PATH == nDelIndex)
    {
        _LogStatus (_T ("MAC not found in list"));
        AfxMessageBox (_T ("MAC not found in list."));
        RefreshMacFilter(FALSE);
        return;
    }

    _stprintf(szMAC, _T("%u"), nDelIndex);

    _LogStatus (_T ("Deleting MAC filter... please wait"));

    if (FALSE == m_pConfig->UpdateACL (szMAC, DEL_MAC_ADDR))
    {
        _LogError (_T ("MAC filter delete failed"));
    }
    else
    {
        _LogStatus (_T ("MAC filter deleted"));
    }
    RefreshMacFilter(TRUE);
}


void CMiscDlg::RefreshMacFilter(BOOL bRetrive)
{
    //clear all mac-strings from combo box
    ((CComboBox*)GetDlgItem (IDC_cbMACFilter))->ResetContent ();

    if (bRetrive && (FALSE == m_pConfig->GetACL ()))
    {
        _LogStatus (_T("Get ACL list failed"));
    }
    else
    {
        int nACLTraceCount = 0;

        for (int i = 0; i < AP_ACL_SIZE; i++)
        {
            TCHAR szACL[MAX_PATH];

            if (strlen (m_pConfig->m_szACLList[i]))
            {
                memset (szACL, NULL, sizeof (szACL));
                mbstowcs (szACL, m_pConfig->m_szACLList[i], strlen(m_pConfig->m_szACLList[i]));
                szACL[strlen (m_pConfig->m_szACLList[i])] = NULL;

                ((CComboBox*)GetDlgItem (IDC_cbMACFilter))->AddString (szACL);

                nACLTraceCount++;

                if(m_pConfig->m_nACLCount <= nACLTraceCount)
                    break;
            }
        }

        if (m_pConfig->m_nACLCount)
        {
            ((CComboBox*)GetDlgItem (IDC_cbMACFilter))->SetCurSel (0);

            //enable the delete button
            ((CButton*)GetDlgItem (IDC_bnDelMAC))->EnableWindow (1);

            if(AP_ACL_SIZE <= m_pConfig->m_nACLCount)
            {
                //disable the add button
                ((CButton*)GetDlgItem (IDC_bnAddMAC))->EnableWindow (0);
            }
            else
            {
                //enable the add button
                ((CButton*)GetDlgItem (IDC_bnAddMAC))->EnableWindow (1);
            }
        }
        else
        {
            _LogStatus (_T("ACL list is empty"));

            //enable the add button
            ((CButton*)GetDlgItem (IDC_bnAddMAC))->EnableWindow (1);

            //disable the delete button
            ((CButton*)GetDlgItem (IDC_bnDelMAC))->EnableWindow (0);
        }
        ((CComboBox*)GetDlgItem (IDC_cbACLPolicy))->SetCurSel (m_pConfig->m_nACLPolicy);
    }
}


void CMiscDlg::OnBnClicked_bnRefresh()
{
    //clear all mac-strings from combo box
    ((CComboBox*)GetDlgItem (IDC_cbStattions))->ResetContent ();

    if (FALSE == m_pConfig->GetStations ())
    {
        _LogError (_T("Get station list failed"));
    }
    else
    {
        _LogStatus (_T("Station list refreshed"));
        int nStaCount = 0;

        for (int i = 0; i < (int)m_pConfig->m_nMaxStationLimit; i++)
        {
            TCHAR szSta[MAX_PATH];

            if (strlen (m_pConfig->m_szStaList[i]))
            {
                memset (szSta, NULL, sizeof (szSta));
                mbstowcs (szSta, m_pConfig->m_szStaList[i], strlen(m_pConfig->m_szStaList[i]));
                szSta[18] = NULL;
                ((CComboBox*)GetDlgItem (IDC_cbStattions))->AddString (szSta);
                nStaCount++;
                if(m_pConfig->m_nStaCount <= nStaCount)
                    break;
            }
        }

        if (m_pConfig->m_nStaCount)
            ((CComboBox*)GetDlgItem (IDC_cbStattions))->SetCurSel (0);
        else
            _LogStatus (_T("Station list is empty"));
    }
}

void CMiscDlg::OnBnClicked_bnAclpolicy()
{
    int iCurSel = ((CComboBox*)GetDlgItem (IDC_cbACLPolicy))->GetCurSel ();
     _LogStatus (_T ("Updating ACL policy.. please wait"));

    if (FALSE == m_pConfig->SetAclPolicy (iCurSel))
    {
        _LogError (_T ("ACL policy update failed"));
        return;
    }
    else
    {
        RefreshMacFilter (TRUE);
        _LogStatus (_T ("ACL policy updated"));
    }
}





