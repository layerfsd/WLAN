
#include<windows.h>
#include<winioctl.h>

#include "Config.h"
extern "C" {
#include "NAT.h"
}

CConfig::CConfig (void)
{
    m_dwPrevDhcp = 0;

    m_enOpMode = enOpMode_Err;

    ClearAPModeStatus();
}

CConfig::~CConfig (void)
{

}

/*
Retrives the current operation mode
*/
enOpMode CConfig::GetCurrentOpMode ()
{
    BOOL bStatus = FALSE;
    DWORD dwBytesReurned = 0;
    UINT nCurrentMode = 0, nSize = 0;
    enOpMode enOprMode = enOpMode_Err;

    PUCHAR pBuffer = NULL;
    PNDISUIO_QUERY_OID pModeOid;
    HANDLE hNdisUIO = NULL;

    WiFiOnOff(TRUE);

    nSize = sizeof (NDISUIO_QUERY_OID) + sizeof (UINT);

    if (NULL  == (pBuffer = (PUCHAR) malloc (nSize)))
    {
        _ERR ("malloc failed");
        return enOprMode;
    }

    memset (pBuffer, 0, nSize );
    pModeOid = (NDISUIO_QUERY_OID *)pBuffer;

    pModeOid->Oid = OID_CAR6K_GET_CURRENT_MODE;
    pModeOid->ptcDeviceName = (PTCHAR) malloc ( _tcslen(_szDeviceName) * sizeof (TCHAR) + 2 );

    if (NULL == pModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free( pBuffer);
        return enOprMode;
    }
    memset (pModeOid->ptcDeviceName, NULL, _tcslen (_szDeviceName) * sizeof (TCHAR) + 2);
    _tcscpy (pModeOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("Querying current operation mode"));

    hNdisUIO = CreateFile (NDISUIO_DEVICE_NAME,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

    // Pass the IOCTL to the device
    if (! DeviceIoControl (hNdisUIO, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer,
        sizeof (NDISUIO_QUERY_OID), pBuffer, nSize, &dwBytesReurned, NULL ))
    {
        _ERR ("Error opening device and query");
        enOprMode = enOpMode_Err;

        if (pModeOid->ptcDeviceName)
            free (pModeOid->ptcDeviceName);

        if (pBuffer)
            free (pBuffer);

        CloseHandle (hNdisUIO);
        return enOprMode;
    }
    CloseHandle (hNdisUIO);

    nCurrentMode = (*(UINT *)(pModeOid->Data));

    if (nCurrentMode == 0x10)
    {
        printf (_Trace ("Soft AP mode"));
        enOprMode = enOpMode_SoftAP;
    }
    else if (nCurrentMode == 0x01)
    {
        printf (_Trace ("Station mode - Infrastructure"));
        enOprMode = enOpMode_Station;
    }
    else if (nCurrentMode == 0x02)
    {
        printf (_Trace ("Station mode - ADHOC"));
        enOprMode = enOpMode_Station;
    }
    else if ( nCurrentMode == 0x04)
    {
        printf (_Trace ("Station mode - ADHOC Creator"));
        enOprMode = enOpMode_Station;
    }
    else
    {
        printf (_Trace ("Unknown mode"));
        enOprMode = enOpMode_Err;
    }

    m_enOpMode = enOprMode;

    if (pModeOid->ptcDeviceName)
        free (pModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return enOprMode;
}

/*
Switches to SoftAP mode
*/
BOOL CConfig::SetAPMode (TCHAR *pszSSID, TCHAR *pszChannel)
{
    PUCHAR pBuffer = NULL;
    BOOL IsSuccess = FALSE;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    WMI_CONNECT_CMD *pApCmd = (WMI_CONNECT_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (WMI_CONNECT_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (buf, NULL, sizeof (buf));
    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    // Send OID to the driver
    pApModeOid->Oid = OID_CAR6K_SET_APMODE;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free (pBuffer);
        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    wcstombs ((char*)pApCmd->ssid, pszSSID, _tcslen(pszSSID));
    pApCmd->ssidLength = strlen ((char*)pApCmd->ssid);

    printf (_Trace ("SSID %s, Length %d"), pApCmd->ssid, pApCmd->ssidLength);
    printf (_Trace ("Channel %d"), _ttoi (pszChannel));

    pApCmd->channel = IEEE2Freq (_ttoi (pszChannel));

    printf (_Trace ("Frequency %d"), pApCmd->channel);

    memcpy (pApModeOid->Data, pApCmd, sizeof (WMI_CONNECT_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    if (IsSuccess)
    {
        printf (_Trace ("Set AP mode Success"));
        if (enOpMode_SoftAP != m_enOpMode) /* if previous mode was not SoftAP*/
        {
            RebindAdapter (_szDeviceName);
            Sleep (100);
        }

        m_enOpMode = enOpMode_SoftAP;

        wcstombs ((char*)m_APModeStatus.ssid, pszSSID, _tcslen(pszSSID));
        m_APModeStatus.channel = (USHORT)_ttoi (pszChannel);

        //by default AP mode will have security open-disabled
        m_APCryptStatus.Authmode = NONE_AUTH;
        m_APCryptStatus.cryptType = NONE_CRYPT;
        m_APCryptStatus.dot11_authmode = OPEN_AUTH;
    }
    else
    {
        _ERR ("Set AP mode failed");
    }

    return IsSuccess;
}

/*
Switches to SoftAP mode
*/
BOOL CConfig::SetAPMode (TCHAR *pszSSID, TCHAR *pszChannel, TCHAR *pszDTIM, USHORT nBeaconInterval,
                         BOOL bIsHidden, BOOL bIsEnableProtection,BOOL bIsEnableIntraBSS)
{
    PUCHAR pBuffer = NULL;
    BOOL IsSuccess = FALSE;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    AP_COMMIT_XTND *pApCmd = (AP_COMMIT_XTND *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (AP_COMMIT_XTND);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (buf, NULL, sizeof (buf));
    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    // Send OID to the driver
    pApModeOid->Oid = OID_CAR6K_SET_APMODE_XTN;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free (pBuffer);
        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    wcstombs ((char*)pApCmd->ssid, pszSSID, _tcslen(pszSSID));
    pApCmd->ssidLength = strlen ((char*)pApCmd->ssid);

    printf (_Trace ("SSID %s, Length %d"), pApCmd->ssid, pApCmd->ssidLength);
    printf (_Trace ("Channel %d"), _ttoi (pszChannel));

    pApCmd->channel = IEEE2Freq (_ttoi (pszChannel));

    printf (_Trace ("Frequency %d"), pApCmd->channel);

    pApCmd->dtim = (A_UINT8)_ttoi(pszDTIM);
    printf (_Trace ("DTIM: %d"), pApCmd->dtim);

    pApCmd->beaconInterval = nBeaconInterval;
    printf (_Trace ("BeaconInterval: %d"), pApCmd->beaconInterval);

    printf (_Trace ("Is Hidden SSID: %d"), (A_UINT8)bIsHidden);
    pApCmd->hidden_ssid = (A_BOOL)bIsHidden;

	printf (_Trace ("Is Intra BSS Comm: %d"), (A_UINT8)bIsEnableIntraBSS);
    pApCmd->IntraBss = (A_BOOL)bIsEnableIntraBSS;

    printf (_Trace ("Is Protection enable: %u"), (A_UINT8)bIsEnableProtection);
    pApCmd->bgprotection = (A_BOOL)bIsEnableProtection;

    memcpy (pApModeOid->Data, pApCmd, sizeof (AP_COMMIT_XTND));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    if (IsSuccess)
    {
        if (enOpMode_SoftAP != m_enOpMode) /* if previous mode was not SoftAP*/
        {
            RebindAdapter (_szDeviceName);
            Sleep (100);
        }

        wcstombs ((char*)m_APModeStatus.ssid, pszSSID, _tcslen(pszSSID));
        m_APModeStatus.channel = (USHORT)_ttoi (pszChannel);
        m_APModeStatus.dtim = (A_UINT8)_ttoi(pszDTIM);
        m_APModeStatus.hidden_ssid = bIsHidden;
        m_APModeStatus.bgprotection = bIsEnableProtection;
		m_APModeStatus.IntraBss = bIsEnableIntraBSS;
        m_APModeStatus.beaconInterval = nBeaconInterval;

        _tcscpy(m_szCountryCode, _szDefaultCountryCode);
        m_APCryptStatus.cryptType = 0;
        m_enOpMode = enOpMode_SoftAP;
        printf (_Trace ("Set AP mode Success"));
    }
    else
        _ERR ("Set AP mode failed");

    return IsSuccess;
}

BOOL CConfig::SetHiddenSsid(BOOL bIsHidden)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    WMI_AP_HIDDEN_SSID_CMD *pHiddenSsidCmd = (WMI_AP_HIDDEN_SSID_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (WMI_AP_HIDDEN_SSID_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    // Send OID to the driver
    pApModeOid->Oid = OID_CAR6K_SET_HIDDEN_SSID;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("Is Hidden SSID: %d"), (A_UINT8)bIsHidden);

    pHiddenSsidCmd->hidden_ssid = (A_UINT8)bIsHidden;

    memcpy (pApModeOid->Data, pHiddenSsidCmd, sizeof (WMI_AP_HIDDEN_SSID_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
    {
        m_APModeStatus.hidden_ssid = bIsHidden;
        printf (_Trace ("Hidden SSID update success"));
    }
    else
    {
        _ERR ("Hidden SSID update failed");
    }

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

BOOL CConfig::SetIntraBss(BOOL bIsEnableIntraBSS)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    AP_SET_INTRA_BSS *pIntraBssCmd = (AP_SET_INTRA_BSS *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (AP_SET_INTRA_BSS);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    // Send OID to the driver
    pApModeOid->Oid = OID_CAR6K_SET_INTRABSS;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("Is Intra BSS Comm: %d"), (A_UINT8)bIsEnableIntraBSS);

    pIntraBssCmd->intrabss = (A_UINT8)bIsEnableIntraBSS;

    memcpy (pApModeOid->Data, pIntraBssCmd, sizeof (AP_SET_INTRA_BSS));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
    {
        m_APModeStatus.IntraBss = bIsEnableIntraBSS;
        printf (_Trace ("Intra BSS update success"));
    }
    else
    {
        _ERR ("Intra BSS update failed");
    }

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

BOOL CConfig::SetAclPolicy(int nPolicy)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    WMI_AP_ACL_POLICY_CMD *pACLpolicy = (WMI_AP_ACL_POLICY_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (WMI_AP_ACL_POLICY_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    // Send OID to the driver
    pApModeOid->Oid = OID_CAR6K_SET_ACL_POLICY;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("ACL Policy: %d"), nPolicy);

    pACLpolicy->policy = (A_UINT8)nPolicy;

    memcpy (pApModeOid->Data, pACLpolicy, sizeof (WMI_AP_ACL_POLICY_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
    {
        printf (_Trace ("ACL Policy update success"));
    }
    else
    {
        _ERR ("ACL Policy update failed");
    }

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

BOOL CConfig::SetBGProtection(BOOL bIsEnable)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    WMI_AP_PROT_SCAN_TIME_CMD *pBGScan = (WMI_AP_PROT_SCAN_TIME_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (WMI_AP_PROT_SCAN_TIME_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (buf, 0, sizeof (buf));
    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    pApModeOid->Oid = OID_CAR6K_SET_BG_PROTECTION;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("Is BG protection: %d"), (A_UINT8)bIsEnable);

    if(bIsEnable)
    {
        pBGScan->period_min = _BG_PROT_PERIOD;
        pBGScan->dwell_ms = _BG_PROT_DWELL;
    }

    memcpy (pApModeOid->Data, pBGScan, sizeof (WMI_AP_PROT_SCAN_TIME_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
    {
        m_APModeStatus.bgprotection = bIsEnable;
        printf (_Trace ("BG protection update success"));
    }
    else
    {
        _ERR ("BG protection update failed");
    }

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

/*
Updates the SoftAP's security profile
*/
BOOL CConfig::SetSecurity (enAuthMode enAuth, enCipherMode enCipher, short nKeyIndex, TCHAR *pszKey)
{
    BYTE tempbyte;
    UINT8 keyarr [32 + 1];
    unsigned int k = 0, tempch = 0, nSize = 0;
    BOOL IsAscii = FALSE, IsSuccess = FALSE;

    char buf [MAX_PATH];
    AP_MODE_CRYPT *pApkey = (AP_MODE_CRYPT *)&buf;

    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApkeyOid = NULL;

    memset (buf, NULL, sizeof (buf));
    wcstombs ((char*)pApkey->KeyMaterial, pszKey, _tcslen(pszKey));
    pApkey->KeyLength = strlen ((char *)pApkey->KeyMaterial);

    switch (enAuth)
    {
    case enAuthMode_Open:
        if (enCipherMode_Disabled == enCipher)//open-disabled
        {
            pApkey->Authmode = NONE_AUTH;
            pApkey->cryptType = NONE_CRYPT;
            pApkey->dot11_authmode = OPEN_AUTH;
            break;
        }
    case enAuthMode_Shared:
        {
            //Valid WEP key lengths are 5, 10, 13, 26
            switch (pApkey->KeyLength)
            {
            case _WEP40_ASCII_KEY_LEN: case _WEP104_ASCII_KEY_LEN:
                IsAscii = TRUE;
                break;
            case _WEP40_HEX_KEY_LEN: case _WEP104_HEX_KEY_LEN:
                IsAscii = FALSE;
                break;
            default:
                printf (_Trace ("Invalid WEP key length: %u"), pApkey->KeyLength);
                return FALSE;
            }

            pApkey->Authmode = NONE_AUTH;
            pApkey->cryptType = WEP_CRYPT;
            if (enAuthMode_Shared == enAuth)
            {
                pApkey->dot11_authmode = SHARED_AUTH;//shared wep
            }
            else
            {
                pApkey->dot11_authmode = OPEN_AUTH;//open wep
            }
        }
        break;

    case enAuthMode_WPA_PSK:
    case enAuthMode_WPA2_PSK:
        {
            //Valid WPA/WPA2 PSK length is 8 to 64
            if ((8 > pApkey->KeyLength) ||
                (8 < pApkey->KeyLength && 64 < pApkey->KeyLength))
            {
                printf (_Trace ("Invalid PSK length: %u"), pApkey->KeyLength);
                return FALSE;
            }

            IsAscii = ((64 == pApkey->KeyLength) ? FALSE : TRUE);
            pApkey->IsAscii = IsAscii;
            pApkey->dot11_authmode = OPEN_AUTH;

            if (enAuthMode_WPA_PSK == enAuth)
            {
                pApkey->Authmode = WPA_PSK_AUTH;
                pApkey->cryptType = TKIP_CRYPT;
            }
            else
            {
                pApkey->Authmode = WPA2_PSK_AUTH;
                pApkey->cryptType = AES_CRYPT;
            }
        }
        break;

    default:
        printf (_Trace ("Invalid auth mode: %u"), enAuth);
        return FALSE;
    }

    if (enCipherMode_Disabled != enCipher)
    {
        pApkey->KeyIndex = nKeyIndex;

        printf (_Trace ("SetSecurity; KeyIndex: %u KeyLen: %u"), pApkey->KeyIndex, pApkey->KeyLength);

        if ( ! IsAscii)
        {
            for (k = 0, tempbyte = 0 ; k < pApkey->KeyLength ; ++k)
            {
                tempch = (UINT8)(pApkey->KeyMaterial[k]);
                switch (tempch)
                {
                case L'0': case L'1': case L'2': case L'3': case L'4':
                case L'5': case L'6': case   L'7': case L'8': case L'9':
                    tempch -= L'0';
                    break;
                case L'a': case L'b': case L'c':
                case L'd': case L'e': case L'f':
                    tempch -= L'a' - 10;
                    break;
                case L'A': case L'B': case L'C':
                case L'D': case L'E': case L'F':
                    tempch -= L'A' - 10;
                    break;
                default:
                    printf (_Trace ("Invalid HEX key"));
                    return FALSE;
                }

                if ((k & 1) == 0)
                {
                    tempbyte = tempch;
                }
                else
                {
                    tempbyte <<= 4;
                    tempbyte |= tempch;
                    keyarr[k/2] = tempbyte;
                }
            }
            memcpy (pApkey->KeyMaterial,keyarr,_tcslen ((TCHAR *)pszKey)/2);
            pApkey->KeyLength /= 2;
        }

        pApkey->KeyIndex = pApkey->KeyIndex | _TX_USAGE;
    }
    pApkey->Length = sizeof (pApkey);

    nSize = sizeof (NDISUIO_SET_OID) + sizeof (AP_MODE_CRYPT) + pApkey->KeyLength + 1;

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        printf( _Trace("pApkey->KeyLength= %u, nSize= %u"), pApkey->KeyLength, nSize);
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize);
    pApkeyOid = (NDISUIO_SET_OID *)pBuffer;
    pApkeyOid->Oid = OID_CAR6K_SET_SECURITY;
    pApkeyOid->ptcDeviceName = (PTCHAR)malloc (_tcslen (_szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApkeyOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        free (pBuffer);
        return FALSE;
    }
    _tcscpy (pApkeyOid->ptcDeviceName, _szDeviceName);

    memcpy (pApkeyOid->Data, pApkey, (sizeof (AP_MODE_CRYPT) + pApkey->KeyLength + 1));

    // Send OID to the driver
    IsSuccess = NdisUioSetOid (pApkeyOid, nSize);
    if (IsSuccess)
    {
        printf (_Trace ("SoftAP security update success"));

        switch (enCipher)
        {
        case enCipherMode_Disabled:
            m_APCryptStatus.cryptType = NONE_CRYPT;
            m_APCryptStatus.dot11_authmode = OPEN_AUTH;

            m_nStationLimit = _MAX_STATIONS;
            m_nMaxStationLimit = _MAX_STATIONS;
            break;

        case enCipherMode_WEP:
            m_APCryptStatus.cryptType = WEP_CRYPT;

            if (enAuthMode_Shared == enAuth)
                m_APCryptStatus.dot11_authmode = SHARED_AUTH;//shared
            else
                m_APCryptStatus.dot11_authmode = OPEN_AUTH;//open

            m_nStationLimit = _MAX_STATIONS_WEP;
            m_nMaxStationLimit = _MAX_STATIONS_WEP;
            break;

        case enCipherMode_TKIP:
            m_APCryptStatus.cryptType = TKIP_CRYPT;

            m_nStationLimit = _MAX_STATIONS_TKIP;
            m_nMaxStationLimit = _MAX_STATIONS_TKIP;
            break;

        case enCipherMode_AES:
            m_APCryptStatus.cryptType = AES_CRYPT;

            m_nStationLimit = _MAX_STATIONS_AES;
            m_nMaxStationLimit = _MAX_STATIONS_AES;
            break;

        default:
            m_APCryptStatus.cryptType = NONE_CRYPT;
            m_APCryptStatus.Authmode = NONE_AUTH;
            m_APCryptStatus.dot11_authmode = OPEN_AUTH;

            m_nStationLimit = _MAX_STATIONS;
            m_nMaxStationLimit = _MAX_STATIONS;
            break;
        }
    }
    else
    {
        _ERR ("SoftAP security update failed");
    }

    if (pApkeyOid->ptcDeviceName)
        free (pApkeyOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

/*
Updates the station configuration
*/
BOOL CConfig::SetStationMode ()
{
    PNDISUIO_SET_OID stamodeOid;
    PUCHAR pBuffer = NULL;
    BOOL IsSuccess = FALSE;
    unsigned int nSize = sizeof (NDISUIO_SET_OID);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize);
    stamodeOid = (NDISUIO_SET_OID *)pBuffer;

    stamodeOid->Oid = OID_CAR6K_SET_STAMODE;
    stamodeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen (_szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == stamodeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free (pBuffer);
        return FALSE;
    }

    _tcscpy (stamodeOid->ptcDeviceName, _szDeviceName);

    // Send OID to the driver
    IsSuccess = NdisUioSetOid (stamodeOid, nSize);
    if (IsSuccess)
    {
        RebindAdapter(_szDeviceName);
        Sleep(100);
        printf (_Trace ("Switching to Statiton mode success"));
        m_enOpMode = enOpMode_Station;
        ClearAPModeStatus();
    }
    else
    {
        _ERR ("Switching to Statiton mode Failed");
    }

    if (stamodeOid->ptcDeviceName)
        free (stamodeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

/*
Sends Set OID to the driver
*/
BOOL CConfig::NdisUioSetOid (NDISUIO_SET_OID* pSetOid, unsigned int nSize)
{
    HANDLE hNdisUIO = NULL;
    BOOL bStatus = FALSE;
    DWORD dwBytesReurned = 0;

    hNdisUIO = CreateFile (NDISUIO_DEVICE_NAME,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

    if (INVALID_HANDLE_VALUE == hNdisUIO)
    {
        printf (_Trace ("Err: Failed to open NDISUIO_DEVICE_NAME %d"), GetLastError());
        return bStatus;
    }

    bStatus = DeviceIoControl (hNdisUIO, IOCTL_NDISUIO_SET_OID_VALUE, pSetOid, nSize,
        NULL, 0, &dwBytesReurned, NULL);

    CloseHandle (hNdisUIO);
    hNdisUIO = NULL;

    return bStatus;
}

/*
Converts ieee channel number to frequency
*/
A_UINT16 CConfig::IEEE2Freq (int iChannel)
{
    if (iChannel == 14) {
        return 2484;
    }
    if (iChannel < 14) {    /* 0-13 */
        return (2407 + (iChannel * 5));
    }
    if (iChannel < 27) {    /* 15-26 */
        return (2512 + ((iChannel - 15) * 20));
    }
    return (5000 + (iChannel * 5));
}
BOOL CConfig::SetChannel(TCHAR *pszChannel)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    AP_SET_CHANNEL_CMD *pChannelCmd = (AP_SET_CHANNEL_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (AP_SET_CHANNEL_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (buf, 0, sizeof (buf));
    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    pApModeOid->Oid = OID_CAR6K_SET_CHANNEL;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("Channel %d"), _ttoi (pszChannel));

    pChannelCmd->channel = IEEE2Freq (_ttoi (pszChannel));

    printf (_Trace ("Frequency %d"), pChannelCmd->channel);

    memcpy (pApModeOid->Data, pChannelCmd, sizeof (AP_SET_CHANNEL_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
    {
        m_APModeStatus.channel = (USHORT)_ttoi (pszChannel);
        printf (_Trace ("Channel update success"));
    }
    else
    {
        _ERR ("Channel update failed");
    }

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

BOOL CConfig::SetDTIM(TCHAR *pszDTIM)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    WMI_AP_SET_DTIM_CMD *pDTIMCmd = (WMI_AP_SET_DTIM_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (WMI_AP_SET_DTIM_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (buf, 0, sizeof (buf));
    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    pApModeOid->Oid = OID_CAR6K_SET_DTIM;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    pDTIMCmd->dtim = (A_UINT8)_ttoi(pszDTIM);
    printf (_Trace ("DTIM: %d"), pDTIMCmd->dtim);

    memcpy (pApModeOid->Data, pDTIMCmd, sizeof (WMI_AP_SET_DTIM_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
    {
        m_APModeStatus.dtim = pDTIMCmd->dtim;
        printf (_Trace ("DTIM update success"));
    }
    else
    {
        _ERR ("DTIM update failed");
    }

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

BOOL CConfig::SetBeaconInterval(USHORT nBeaconInterval)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    WMI_BEACON_INT_CMD *pBeaconIntrvl = (WMI_BEACON_INT_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (WMI_BEACON_INT_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (buf, 0, sizeof (buf));
    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    pApModeOid->Oid = OID_CAR6K_SET_BCON_INTVL;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("Beacon interval: %d"), nBeaconInterval);
    pBeaconIntrvl->beaconInterval = nBeaconInterval;

    memcpy (pApModeOid->Data, pBeaconIntrvl, sizeof (WMI_BEACON_INT_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
    {
        m_APModeStatus.beaconInterval = nBeaconInterval;
        printf (_Trace ("Beacon interval update success"));
    }
    else
    {
        _ERR ("Beacon interval update failed");
    }

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;}


BOOL CConfig::SetCountryCode(TCHAR *pszCountry)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    WMI_SET_COUNTRY_CMD *pCountryCmd = (WMI_SET_COUNTRY_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (WMI_SET_COUNTRY_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (buf, 0, sizeof (buf));
    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    pApModeOid->Oid = OID_CAR6K_SET_RD;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    wcstombs ((char*)pCountryCmd->countryCode, pszCountry, _tcslen(pszCountry));

    printf (_Trace ("Country code: %s"), pCountryCmd->countryCode);

    memcpy (pApModeOid->Data, pCountryCmd, sizeof (WMI_SET_COUNTRY_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
    {
        printf (_Trace ("Country code update success"));
        _tcscpy(m_szCountryCode, pszCountry);
    }
    else
        _ERR ("Country code update failed");

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

BOOL CConfig::SetStationLimit(TCHAR *pszStations)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    WMI_AP_SET_NUM_STA_CMD *pNumStaCmd = (WMI_AP_SET_NUM_STA_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (WMI_AP_SET_NUM_STA_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (buf, 0, sizeof (buf));
    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    pApModeOid->Oid = OID_CAR6K_SET_NUM_STA;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    pNumStaCmd->num_sta = (A_UINT8)_ttoi(pszStations);
    printf (_Trace ("Station limit: %d"), pNumStaCmd->num_sta);

    memcpy (pApModeOid->Data, pNumStaCmd, sizeof (WMI_AP_SET_NUM_STA_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
    {
        printf (_Trace ("Station limit update success"));
        m_nStationLimit = pNumStaCmd->num_sta;
    }
    else
        _ERR ("Station limit update failed");

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

BOOL CConfig::GetStations()
{
    ap_get_sta *pAPGetSta = NULL;
    DWORD dwBytesReurned = 0;

    PUCHAR pBuffer = NULL;
    PNDISUIO_QUERY_OID pQueryOid;
    HANDLE hNdisUIO = NULL;

    unsigned int nSize = sizeof (NDISUIO_QUERY_OID) + sizeof (ap_get_sta);

    if (NULL  == (pBuffer = (PUCHAR) malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize );
    pQueryOid = (NDISUIO_QUERY_OID *)pBuffer;

    pQueryOid->Oid = OID_CAR6K_GET_STA;
    pQueryOid->ptcDeviceName = (PTCHAR) malloc ( _tcslen(_szDeviceName) * sizeof (TCHAR) + 2 );

    if (NULL == pQueryOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free( pBuffer);
        return FALSE;
    }
    memset (pQueryOid->ptcDeviceName, NULL, _tcslen (_szDeviceName) * sizeof (TCHAR) + 2);
    _tcscpy (pQueryOid->ptcDeviceName, _szDeviceName);

    hNdisUIO = CreateFile (NDISUIO_DEVICE_NAME,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

    // Pass the IOCTL to the device
    if (! DeviceIoControl (hNdisUIO, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer,
        sizeof (NDISUIO_QUERY_OID), pBuffer, nSize, &dwBytesReurned, NULL ))
    {
        _ERR ("Get stations cmd failed");

        if (pQueryOid->ptcDeviceName)
            free (pQueryOid->ptcDeviceName);

        if (pBuffer)
            free (pBuffer);

        CloseHandle (hNdisUIO);
        return FALSE;
    }
    CloseHandle (hNdisUIO);


    pAPGetSta = (ap_get_sta *)(pQueryOid->Data);

    m_nStaCount = 0;
    for(int i = 0; i < _MAX_STATIONS; i++)
        memset(m_szStaList[i], NULL, sizeof(m_szStaList[i]));

    for(int i = 0; i < _MAX_STATIONS; i++)
    {
        if(!is_mac_null(pAPGetSta->sta[i].mac))
        {
            sprintf(m_szStaList[i], " %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
                pAPGetSta->sta[i].mac[0], pAPGetSta->sta[i].mac[1],
                pAPGetSta->sta[i].mac[2], pAPGetSta->sta[i].mac[3],
                pAPGetSta->sta[i].mac[4], pAPGetSta->sta[i].mac[5]);

            m_nStaCount++;
            printf(_Trace("MAC%u: %s, AID: %u"), m_nStaCount, m_szStaList[i], pAPGetSta->sta[i].aid);
        }
    }

    if (pQueryOid->ptcDeviceName)
        free (pQueryOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return TRUE;
}

int CConfig::is_mac_null(A_UINT8 *mac)
{
    if(mac[0]==0 && mac[1]==0 && mac[2]==0 &&
        mac[3]==0 && mac[4]==0 && mac[5]==0) {
            return 1;
    }
    return 0;
}

BOOL CConfig::GetACL()
{
    DWORD dwBytesReurned = 0;

    WMI_AP_ACL *pGetAcl = NULL;
    PUCHAR pBuffer = NULL;
    PNDISUIO_QUERY_OID pQueryOid;
    HANDLE hNdisUIO = NULL;

    unsigned int nSize = sizeof (NDISUIO_QUERY_OID) + sizeof (WMI_AP_ACL);

    if (NULL  == (pBuffer = (PUCHAR) malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize );
    pQueryOid = (NDISUIO_QUERY_OID *)pBuffer;

    pQueryOid->Oid = OID_CAR6K_GET_ACL;
    pQueryOid->ptcDeviceName = (PTCHAR) malloc ( _tcslen(_szDeviceName) * sizeof (TCHAR) + 2 );

    if (NULL == pQueryOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free( pBuffer);
        return FALSE;
    }
    memset (pQueryOid->ptcDeviceName, NULL, _tcslen (_szDeviceName) * sizeof (TCHAR) + 2);
    _tcscpy (pQueryOid->ptcDeviceName, _szDeviceName);

    hNdisUIO = CreateFile (NDISUIO_DEVICE_NAME,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

    // Pass the IOCTL to the device
    if (! DeviceIoControl (hNdisUIO, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer,
        sizeof (NDISUIO_QUERY_OID), pBuffer, nSize, &dwBytesReurned, NULL ))
    {
        _ERR ("Get ACL cmd failed");

        if (pQueryOid->ptcDeviceName)
            free (pQueryOid->ptcDeviceName);

        if (pBuffer)
            free (pBuffer);

        CloseHandle (hNdisUIO);
        return FALSE;
    }
    CloseHandle (hNdisUIO);

    pGetAcl = (WMI_AP_ACL *)(pQueryOid->Data);

    m_nACLCount = 0;
	m_nACLPolicy = pGetAcl->policy;
    for(int i = 0; i < AP_ACL_SIZE; i++)
        memset(m_szACLList[i], NULL, sizeof(m_szACLList[i]));

    printf(_Trace("-------------------------------------------------"));
    printf(_Trace("  ACL - Allow MAC List"));
    printf(_Trace("-------------------------------------------------"));
    for(int i = 0; i < AP_ACL_SIZE; i++)
    {
        if(!is_mac_null(pGetAcl->acl_mac[i]) || pGetAcl->wildcard[i])
        {
            char szBuff[10];
            int j = 0;
            for(j = 0; j < 5; j++)
            {
                memset(szBuff, NULL, sizeof (szBuff));

                if(pGetAcl->wildcard[i] & (1 << j))
                    sprintf(szBuff, "*:");
                else
                    sprintf(szBuff, "%2.2X:", pGetAcl->acl_mac[i][j]);

                strcat(m_szACLList[i], szBuff);
            }

            memset(szBuff, NULL, sizeof (szBuff));
            if(pGetAcl->wildcard[i] & (1 << j))
                sprintf(szBuff, "*");
            else
                sprintf(szBuff, "%2.2X", pGetAcl->acl_mac[i][5]);
            strcat(m_szACLList[i], szBuff);

            m_nACLCount++;
            printf(_Trace("Filter%d: %s"), i, m_szACLList[i]);
        }
    }

    printf(_Trace("Filter count: %d"), m_nACLCount);

    if (pQueryOid->ptcDeviceName)
        free (pQueryOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    printf(_Trace("Get ACL success"));

    return TRUE;
}

BOOL CConfig::UpdateACL(TCHAR *pszACL, int nAction)
{
    BOOL IsSuccess = FALSE;
    PUCHAR pBuffer = NULL;
    PNDISUIO_SET_OID pApModeOid = NULL;

    char buf[MAX_PATH];
    WMI_AP_ACL_MAC_CMD *pACL = (WMI_AP_ACL_MAC_CMD *)&buf;

    unsigned int nSize = sizeof (NDISUIO_SET_OID) + sizeof (WMI_AP_ACL_MAC_CMD);

    if (NULL  == (pBuffer = (PUCHAR)malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (buf, 0, sizeof (buf));
    memset (pBuffer, 0, nSize);
    pApModeOid = (NDISUIO_SET_OID *)pBuffer;

    pApModeOid->Oid = OID_CAR6K_ADD_DEL_ACL;
    pApModeOid->ptcDeviceName = (PTCHAR)malloc (_tcslen( _szDeviceName) * sizeof (TCHAR) + 2);

    if (NULL == pApModeOid->ptcDeviceName)
    {
        _ERR ("malloc failed");

        if (pBuffer)
            free (pBuffer);

        return FALSE;
    }

    _tcscpy (pApModeOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("Action: %d"), (A_UINT8)nAction);

    if(ADD_MAC_ADDR == nAction)
    {
        pACL->action = ADD_MAC_ADDR;
        if(FALSE == wmic_ether_aton_wild(pszACL, pACL->mac, &pACL->wildcard))
        {
            _ERR("bad mac address");
            return FALSE;
        }
        printf (_Trace ("Add MAC: %s"), pszACL);
    }
    else
    {
        pACL->action = DEL_MAC_ADDR;
        pACL->index = (A_UINT8)_ttoi(pszACL);
        printf (_Trace ("Del Index: %d"), pACL->index);
    }

    memcpy (pApModeOid->Data, pACL, sizeof (WMI_AP_ACL_MAC_CMD));

    IsSuccess = NdisUioSetOid (pApModeOid, nSize);
    if (IsSuccess)
        printf (_Trace ("ACL update success"));
    else
        _ERR ("ACL update failed");

    if (pApModeOid->ptcDeviceName)
        free (pApModeOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return IsSuccess;
}

BOOL CConfig::wmic_ether_aton_wild(TCHAR *orig, A_UINT8 *eth, A_UINT8 *wild)
{
    TCHAR *bufp;
    TCHAR val, c;
    int i=0;

    *wild = 0;
    for(bufp = orig; *bufp != '\0'; ++bufp) {
        c = *bufp++;
        if (isdigit(c)) val = c - '0';
        else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
        else if (c == '*')  { val = 0; *wild |= 1<<i; goto next; }
        else break;

        val <<= 4;
        c = *bufp++;
        if (isdigit(c)) val |= c - '0';
        else if (c >= 'a' && c <= 'f') val |= c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') val |= c - 'A' + 10;
        else break;

next:
        eth[i] = (unsigned char) (val & 0xFF);
        if(++i == ATH_MAC_LEN) {
            /* That's it.  Any trailing junk? */
            if (*bufp != '\0') {
            }
            return TRUE;
        }
        if (*bufp != ':')
            break;
    }

    return FALSE;
}

BOOL CConfig::GetAPModeStatus()
{
    DWORD dwBytesReurned = 0;
    UINT nSize = 0;

    PUCHAR pBuffer = NULL;
    PNDISUIO_QUERY_OID pApStatusOid = NULL;
    AP_COMMIT_XTND *pApStatus = NULL;
    HANDLE hNdisUIO = NULL;

    nSize = sizeof (NDISUIO_QUERY_OID) + sizeof (AP_COMMIT_XTND);

    if (NULL  == (pBuffer = (PUCHAR) malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize );
    pApStatusOid = (NDISUIO_QUERY_OID *)pBuffer;

    pApStatusOid->Oid = OID_CAR6K_GET_AP_STATUS;
    pApStatusOid->ptcDeviceName = (PTCHAR) malloc ( _tcslen(_szDeviceName) * sizeof (TCHAR) + 2 );

    if (NULL == pApStatusOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free( pBuffer);
        return FALSE;
    }
    memset (pApStatusOid->ptcDeviceName, NULL, _tcslen (_szDeviceName) * sizeof (TCHAR) + 2);
    _tcscpy (pApStatusOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("Querying AP-Mode status"));

    hNdisUIO = CreateFile (NDISUIO_DEVICE_NAME,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

    // Pass the IOCTL to the device
    if (! DeviceIoControl (hNdisUIO, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer,
        sizeof (NDISUIO_QUERY_OID), pBuffer, nSize, &dwBytesReurned, NULL ))
    {
        _ERR ("Error opening device and query");

        if (pApStatusOid->ptcDeviceName)
            free (pApStatusOid->ptcDeviceName);

        if (pBuffer)
            free (pBuffer);

        CloseHandle (hNdisUIO);
        return FALSE;
    }
    CloseHandle (hNdisUIO);

    pApStatus = ( AP_COMMIT_XTND *)(pApStatusOid->Data);

    printf (_Trace ("SSID: %s"), pApStatus->ssid);
    printf (_Trace ("Is Hidden ssid: %d"), pApStatus->hidden_ssid);
    printf (_Trace ("Frequency: %d"), pApStatus->channel);
    pApStatus->channel = Freq2Channel(pApStatus->channel);
    printf (_Trace ("Channel: %d"), pApStatus->channel);
    printf (_Trace ("DTIM: %d"), pApStatus->dtim);
    printf (_Trace ("Beacon interval: %d"), pApStatus->beaconInterval);
    printf (_Trace ("Is BG-Protection enable: %d"), pApStatus->bgprotection);
	printf (_Trace ("Is Intra BSS comm enable: %d"), pApStatus->IntraBss);
    memcpy(&m_APModeStatus, pApStatus, sizeof (AP_COMMIT_XTND));

    if (pApStatusOid->ptcDeviceName)
        free (pApStatusOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return TRUE;
}

BOOL CConfig::GetAPCryptStatus()
{
    DWORD dwBytesReurned = 0;
    UINT nSize = 0;

    PUCHAR pBuffer = NULL;
    PNDISUIO_QUERY_OID pApCryptStatusOid = NULL;
    AP_MODE_CRYPT *pApCryptStatus = NULL;
    HANDLE hNdisUIO = NULL;

    nSize = sizeof (NDISUIO_QUERY_OID) + sizeof (AP_MODE_CRYPT) + MAX_PATH;

    if (NULL  == (pBuffer = (PUCHAR) malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize );
    pApCryptStatusOid = (NDISUIO_QUERY_OID *)pBuffer;

    pApCryptStatusOid->Oid = OID_CAR6K_GET_AP_CRYPT;
    pApCryptStatusOid->ptcDeviceName = (PTCHAR) malloc ( _tcslen(_szDeviceName) * sizeof (TCHAR) + 2 );

    if (NULL == pApCryptStatusOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free( pBuffer);
        return FALSE;
    }
    memset (pApCryptStatusOid->ptcDeviceName, NULL, _tcslen (_szDeviceName) * sizeof (TCHAR) + 2);
    _tcscpy (pApCryptStatusOid->ptcDeviceName, _szDeviceName);

	printf (_Trace ("Querying AP-Crypt status"));

    hNdisUIO = CreateFile (NDISUIO_DEVICE_NAME,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

    // Pass the IOCTL to the device
    if (! DeviceIoControl (hNdisUIO, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer,
        sizeof (NDISUIO_QUERY_OID), pBuffer, nSize, &dwBytesReurned, NULL ))
    {
        _ERR ("Error opening device and query");

        if (pApCryptStatusOid->ptcDeviceName)
            free (pApCryptStatusOid->ptcDeviceName);

        if (pBuffer)
            free (pBuffer);

        CloseHandle (hNdisUIO);
        return FALSE;
    }
    CloseHandle (hNdisUIO);

    pApCryptStatus = ( AP_MODE_CRYPT *)(pApCryptStatusOid->Data);

    printf (_Trace ("KeyLength %d"), pApCryptStatus->KeyLength);
    printf (_Trace ("KeyIndex %d"), pApCryptStatus->KeyIndex);
    printf (_Trace ("dot11_authmode %u"), pApCryptStatus->dot11_authmode);
    printf (_Trace ("Authmode %d"), pApCryptStatus->Authmode);
    printf (_Trace ("cryptType %d"), pApCryptStatus->cryptType);

    m_APCryptStatus.Authmode = pApCryptStatus->Authmode;
    m_APCryptStatus.cryptType = pApCryptStatus->cryptType;
    m_APCryptStatus.dot11_authmode = pApCryptStatus->dot11_authmode;
    m_APCryptStatus.KeyIndex = pApCryptStatus->KeyIndex;
    m_APCryptStatus.KeyLength = pApCryptStatus->KeyLength;

    switch (m_APCryptStatus.Authmode)
    {
    case NONE_AUTH:
        m_nStationLimit = _MAX_STATIONS_WEP;
        m_nMaxStationLimit = _MAX_STATIONS_WEP;
        break;
    case WPA_PSK_AUTH:
    case WPA2_PSK_AUTH:
        if (TKIP_CRYPT == m_APCryptStatus.cryptType)
        {
            m_nStationLimit = _MAX_STATIONS_TKIP;
            m_nMaxStationLimit = _MAX_STATIONS_TKIP;
        }
        else//AES
        {
            m_nStationLimit = _MAX_STATIONS_AES;
            m_nMaxStationLimit = _MAX_STATIONS_AES;
        }
        break;
    default:
        printf (_Trace ("Invalid Authmode = %u"), m_APCryptStatus.Authmode);
        m_nStationLimit = _MAX_STATIONS_WEP;
        m_nMaxStationLimit = _MAX_STATIONS_WEP;
        break;
    }

    if (pApCryptStatusOid->ptcDeviceName)
        free (pApCryptStatusOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return TRUE;
}

int CConfig::Freq2Channel(A_UINT16 Freq)
{
    switch(Freq)
    {
    case 0:
        return 1;
    case 2412:
        return 1;
    case 2417:
        return 2;
    case 2422:
        return 3;
    case 2427:
        return 4;
    case 2432:
        return 5;
    case 2437:
        return 6;
    case 2442:
        return 7;
    case 2447:
        return 8;
    case 2452:
        return 9;
    case 2457:
        return 10;
    case 2462:
        return 11;
    case 2456:
        return 12;
    case 2461:
        return 13;
    case 2484:
        return 14;
    default:
        return 6;
    }
}

BOOL CConfig::GetCountryCode()
{
    DWORD dwBytesReurned = 0;
    UINT nSize = 0;

    PUCHAR pBuffer = NULL;
    PNDISUIO_QUERY_OID pApQueryOid = NULL;
    HANDLE hNdisUIO = NULL;
    UINT RegDomainCode = 0;

    nSize = sizeof (NDISUIO_QUERY_OID) + sizeof (UINT);

    if (NULL  == (pBuffer = (PUCHAR) malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize );
    pApQueryOid = (NDISUIO_QUERY_OID *)pBuffer;

    pApQueryOid->Oid = OID_CAR6K_GET_RD;
    pApQueryOid->ptcDeviceName = (PTCHAR) malloc ( _tcslen(_szDeviceName) * sizeof (TCHAR) + 2 );

    if (NULL == pApQueryOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free( pBuffer);
        return FALSE;
    }
    memset (pApQueryOid->ptcDeviceName, NULL, _tcslen (_szDeviceName) * sizeof (TCHAR) + 2);
    _tcscpy (pApQueryOid->ptcDeviceName, _szDeviceName);

    hNdisUIO = CreateFile (NDISUIO_DEVICE_NAME,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

    // Pass the IOCTL to the device
    if (! DeviceIoControl (hNdisUIO, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer,
        sizeof (NDISUIO_QUERY_OID), pBuffer, nSize, &dwBytesReurned, NULL ))
    {
        _ERR ("Error opening device and query");

        if (pApQueryOid->ptcDeviceName)
            free (pApQueryOid->ptcDeviceName);

        if (pBuffer)
            free (pBuffer);

        CloseHandle (hNdisUIO);
        return FALSE;
    }
    CloseHandle (hNdisUIO);

    RegDomainCode = (*(UINT *)(pApQueryOid->Data));

#define REGCODE_IS_CC_BITSET(x)   ((x) & 0x80000000)
#define REGCODE_GET_CODE(x)   ((x) & 0xFFF)
#define REGCODE_IS_WWR_BITSET(x)   ((x) & 0x40000000)

    if (REGCODE_IS_CC_BITSET(RegDomainCode))
        printf (_Trace  ("Country Code = %u"), REGCODE_GET_CODE(RegDomainCode));
    else if (REGCODE_IS_WWR_BITSET(RegDomainCode))
        printf (_Trace  ("WWR Roaming code = %u"), REGCODE_GET_CODE(RegDomainCode));
    else
        printf (_Trace  ("Regulatory Domain = %u"), REGCODE_GET_CODE(RegDomainCode));

    for (int i = 0; i < g_nMaxCountryCode; i++)
    {
        if (g_stCountryCodeTable[i].Code == REGCODE_GET_CODE(RegDomainCode))
        {
            _tcscpy (m_szCountryCode, g_stCountryCodeTable[i].szName);
            break;
        }
    }

    _tPRINTF (_T ("%sRegDomain: %u-%s"), _szAppNameW, REGCODE_GET_CODE(RegDomainCode), m_szCountryCode);

    if (pApQueryOid->ptcDeviceName)
        free (pApQueryOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return TRUE;
}

BOOL CConfig::GetStationLimit()
{
    DWORD dwBytesReurned = 0;
    UINT nSize = 0;

    PUCHAR pBuffer = NULL;
    PNDISUIO_QUERY_OID pApQueryOid = NULL;
    AP_GET_NUM_STA_CMD *pApNumSta = NULL;
    HANDLE hNdisUIO = NULL;

    nSize = sizeof (NDISUIO_QUERY_OID) + sizeof (AP_GET_NUM_STA_CMD);

    if (NULL  == (pBuffer = (PUCHAR) malloc (nSize)))
    {
        _ERR ("malloc failed");
        return FALSE;
    }

    memset (pBuffer, 0, nSize );
    pApQueryOid = (NDISUIO_QUERY_OID *)pBuffer;

    pApQueryOid->Oid = OID_CAR6K_GET_NUM_STA;
    pApQueryOid->ptcDeviceName = (PTCHAR) malloc ( _tcslen(_szDeviceName) * sizeof (TCHAR) + 2 );

    if (NULL == pApQueryOid->ptcDeviceName)
    {
        _ERR ("malloc failed");
        if (pBuffer)
            free( pBuffer);
        return FALSE;
    }
    memset (pApQueryOid->ptcDeviceName, NULL, _tcslen (_szDeviceName) * sizeof (TCHAR) + 2);
    _tcscpy (pApQueryOid->ptcDeviceName, _szDeviceName);

    printf (_Trace ("Querying station limit\n"));

    hNdisUIO = CreateFile (NDISUIO_DEVICE_NAME,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);

    // Pass the IOCTL to the device
    if (! DeviceIoControl (hNdisUIO, IOCTL_NDISUIO_QUERY_OID_VALUE, pBuffer,
        sizeof (NDISUIO_QUERY_OID), pBuffer, nSize, &dwBytesReurned, NULL ))
    {
        _ERR ("Error opening device and query");

        if (pApQueryOid->ptcDeviceName)
            free (pApQueryOid->ptcDeviceName);

        if (pBuffer)
            free (pBuffer);

        CloseHandle (hNdisUIO);
        return FALSE;
    }
    CloseHandle (hNdisUIO);

    pApNumSta = ( AP_GET_NUM_STA_CMD *)(pApQueryOid->Data);

    m_nStationLimit = pApNumSta->num_sta;

    printf (_Trace ("Station limit: %u"), m_nStationLimit);

    if (pApQueryOid->ptcDeviceName)
        free (pApQueryOid->ptcDeviceName);

    if (pBuffer)
        free (pBuffer);

    return TRUE;
}

/*
Updates the AP configuration
*/
BOOL CConfig::SetAPConfig (TCHAR *pszPrivate, TCHAR *pszPublic, BOOL bUpdateAll)
{
    DWORD Status = 0;
    BOOL bIsSuccess = FALSE;

    if (bUpdateAll)
    {
        //switch off wlan, sleep 100
        if (S_OK != WiFiOnOff(FALSE))
        {
            _ERR("WiFi OFF failed");
            return FALSE;
        }
        Sleep(200);

        //ar6k/params/tcpip - if (EnableDHCP == 1){make enabledhcp=autocfg=0, set static ip,subnetmask,gateway}else nothing and rebind ar6k
        if (FALSE == WiFiIPConfig(TRUE, &m_dwPrevDhcp))
        {
            _ERR("WiFiIPConfig failed");
            return FALSE;
        }
        Sleep(100);

        //comm//ConnectionSharing - set private = AR6K_SD1
        if (FALSE == SetInterface(_szPrivateKey, pszPrivate))
        {
            _ERR("Set private interface failed");
            return FALSE;
        }
    }

    //comm//ConnectionSharing - set public = VMINI1,
    if (FALSE == SetInterface(_szPublicKey, pszPublic))
    {
        _ERR("Set public interface failed");
        return FALSE;
    }
    Sleep(100);

    //open nat0, send enable dhcp ioctl
    if (FALSE == EnableDHCP(TRUE))
    {
        _ERR("Enable dhcp failed");
        return FALSE;
    }
    Sleep(100);

    //open ipd0, send enable iprouting ioctl
    if (FALSE == EnableIPRouting(TRUE))
    {
        _ERR("Enable ip routing failed");
        return FALSE;
    }
    Sleep(100);

    //Update the public gateway interface
   if (!_tcscmp (_szVMINI1, pszPublic))//If Public Gateway is "VMINI" only
   {
        //comm//vmini1//parms/tcpip - set autocfg, enabledhcp to 1 and rebind the vmini
        if (FALSE == PublicIPConfig(pszPublic, TRUE))
        {
            _ERR("Public ip config failed");
            return FALSE;
        }
    }

    //store public interface to revert changes when switching to station mode
    memset (m_szPublicInterface, NULL, sizeof (m_szPublicInterface));
    _tcscpy (m_szPublicInterface, pszPublic);
    AppRegSetString (_szAppPublicInterface, m_szPublicInterface);

    Sleep(100);

    /*GPRS related stuff is not tested yet*/
    //radio connect - gprs connect
    if (_tcsstr (pszPublic, _szGPRS) != NULL)
    {
        //connect if gprs is public interface
        ZeroMemory(&GPRSAdapter,sizeof(ConnectionCtrl));
        Status = RadioConnectDisConnect(&GPRSAdapter,GPRS_CONNECT);
        Sleep(2000);

        if(Status != S_OK)
        {
            _ERR ("GPRS connection Failure");
            //return FALSE;
        }
        else
        {
            _tPRINTF (_T ("%sGPRS connected to %s"), _szAppNameW ,GPRSAdapter.ConnectionName);
        }
    }

    if (bUpdateAll)
    {
        //now switch on wlan, sleep 2000
        if (S_OK != WiFiOnOff(TRUE))
        {
            _ERR("WiFi ON failed");
            return FALSE;
        }
        Sleep(3000);
    }

    return TRUE;
}

/*
Updates station configurations
*/
BOOL CConfig::SetStationConfig()
{
    //switch off wlan, sleep 3000
    if (S_OK != WiFiOnOff(FALSE))
    {
        _ERR("WiFi Off failed");
        return FALSE;
    }
    Sleep(3000);

    /*GPRS related stuff is not tested yet*/
    //Radio disconnect, sleep 100
    if (_tcsstr (m_szPublicInterface, _szGPRS) != NULL)
    {
        //disconnect if gprs was the public interface
        RadioConnectDisConnect(&GPRSAdapter,GPRS_DISCONNECT);
        _ERR("Disconnected GPRS Connection");
        Sleep(100);
    }

    //Update the public gateway interface
    if (!_tcscmp (_szVMINI1, m_szPublicInterface))//If Public Gateway is "VMINI" only
    {
        //comm//vmini1//parms/tcpip - set autocfg, enabledhcp to 1 and rebind the vmini
        if (FALSE == PublicIPConfig(m_szPublicInterface, FALSE))
        {
            _ERR("Public ip config failed");
            return FALSE;
        }
        Sleep(100);
    }

    //comm//ConnectionSharing - set private = "", public = "",
    if (FALSE == SetInterface(_szPrivateKey, _T("")))
    {
        _ERR("Set private interface failed");
        return FALSE;
    }

    //comm//ConnectionSharing - set public = "",
    if (FALSE == SetInterface(_szPublicKey, _T("")))
    {
        _ERR("Set public interface failed");
        return FALSE;
    }
    Sleep(100);

    //ar6k//parm//tcpip - if dhcp was enable previously then enabledhcp=1 , rebind ar6k
    if (FALSE == WiFiIPConfig(FALSE, &m_dwPrevDhcp))
    {
        _ERR("WiFiIPConfig failed");
        return FALSE;
    }
    Sleep(100);

    //open ipd0, send disable iprouting ioctl
    if (FALSE == EnableIPRouting(FALSE))
    {
        _ERR("Disable ip routing failed");
        return FALSE;
    }
    Sleep(100);

    //open nat0, send disable dhcp ioctl
    if (FALSE == EnableDHCP(FALSE))
    {
        _ERR("Disable dhcp failed");
        return FALSE;
    }
    Sleep(100);

    //now switch on wlan, sleep 2000
    if (S_OK != WiFiOnOff(TRUE))
    {
        _ERR("WiFi ON failed");
        return FALSE;
    }
    Sleep(2000);

    return TRUE;
}

void CConfig::ClearAPModeStatus()
{
    memset (m_szPublicInterface, NULL, sizeof (m_szPublicInterface));

    memset (&m_APModeStatus, NULL, sizeof (m_APModeStatus));
    m_APModeStatus.bgprotection = TRUE;
	m_APModeStatus.IntraBss = TRUE;
    m_APModeStatus.channel = Freq2Channel (DFT_AP_CHANNEL);
    m_APModeStatus.dtim = DFT_AP_DTIM_PERIOD;
    m_APModeStatus.hidden_ssid = FALSE;
    m_APModeStatus.beaconInterval = DFT_AP_BEACON_INTVL;//ms

    memset (&m_APCryptStatus, NULL, sizeof (m_APCryptStatus));
    m_APCryptStatus.Authmode = NONE_AUTH;
    m_APCryptStatus.cryptType = NONE_CRYPT;
    m_APCryptStatus.dot11_authmode = OPEN_AUTH;
    //m_APCryptStatus.KeyIndex = 0;
    //m_APCryptStatus.KeyLength = 0;
    //m_APCryptStatus.KeyMaterial = 0;
    //m_APCryptStatus.Length = 0;

    m_nStationLimit = _DEFAULT_STATIONS;
    m_nMaxStationLimit = _DEFAULT_STATIONS;

    m_nStaCount = 0;
    for (int i = 0; i < _MAX_STATIONS; i++)
        memset (m_szStaList[i], NULL, sizeof (m_szStaList[i]));

    m_nACLCount = 0;
    for (int i = 0; i < AP_ACL_SIZE; i++)
        memset (m_szACLList[i], NULL, sizeof (m_szACLList[i]));
	m_nACLPolicy = 0;
    AppRegGetString(_szAppPublicInterface, m_szPublicInterface, _szVMINI1);
    _tcscpy (m_szCountryCode, _szDefaultCountryCode);
}


