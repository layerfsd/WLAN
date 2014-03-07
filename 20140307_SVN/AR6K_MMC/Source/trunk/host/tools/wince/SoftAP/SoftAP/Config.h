#pragma once

#include<Ntddndis.h>
#include<Nuiouser.h>

/*
To resolve "Cannot open include file: 'athdrv_wince.h'" compilation error,
add the following directories of Olca to VC++ include directories for 
the appropriate Windows Mobile SDK/platform

\olca\host\os\wince\include
\olca\host\include
\olca\include
*/
#include <osapi_wince.h>
#include <athdrv_wince.h>
#include <wmi.h>
#include "Definitions.h"
#include "WiFiCtrl.h"

//Macro definitions

#define A_UCHAR		unsigned char
#define A_UINT32	unsigned long
#define	A_UINT16	unsigned short
#define	A_UINT8		unsigned char

#define WMI_MAX_KEY_LEN     32
#define WMI_MAX_SSID_LEN    32
#define ATH_MAC_LEN         6
#define _BG_PROT_PERIOD		5 //minutes
#define _BG_PROT_DWELL		200 //milli-seconds

#define _MIN_CHANNEL	1
#define _MAX_CHANNEL	14

#define _MIN_DTIM		1
#define _MAX_DTIM		10
#define _DEFAULT_DTIM	5

#define _MIN_STATIONS		1
#define _MAX_STATIONS		AP_MAX_NUM_STA
#define _MAX_STATIONS_WEP   _MAX_STATIONS
#define _MAX_STATIONS_TKIP  6
#define _MAX_STATIONS_AES   _MAX_STATIONS
#define _DEFAULT_STATIONS	_MAX_STATIONS_WEP

#define _TX_USAGE		0x80000000
#define _GROUP_USAGE	0x40000000

#define _WEP40_ASCII_KEY_LEN    5
#define _WEP40_HEX_KEY_LEN      10

#define _WEP104_ASCII_KEY_LEN   13
#define _WEP104_HEX_KEY_LEN     26

#define _MIN_PSK_LEN    8
#define _MAX_PSK_LEN    64

//Enumeration definitions
typedef enum _enOpMode
{
    enOpMode_Err = 0,
    enOpMode_Station,
    enOpMode_SoftAP,
}enOpMode;

typedef enum _enAuthMode
{
    enAuthMode_Open = 0,
    enAuthMode_Shared,
    enAuthMode_WPA_PSK,
    enAuthMode_WPA2_PSK
}enAuthMode;

typedef enum _enCipherMode
{
    enCipherMode_Disabled = 0,
    enCipherMode_WEP,
    enCipherMode_TKIP,
    enCipherMode_AES
}enCipherMode;


class CConfig
{
private:
    DWORD m_dwPrevDhcp;

    A_UINT16 IEEE2Freq (int iChannel);
    int Freq2Channel(A_UINT16 Freq);
    BOOL NdisUioSetOid (NDISUIO_SET_OID* pSetOid, unsigned int iSize);

public:
    CConfig (void);
    ~CConfig (void);
    enOpMode m_enOpMode;

    AP_COMMIT_XTND m_APModeStatus;
    AP_MODE_CRYPT m_APCryptStatus;

    TCHAR m_szPublicInterface[MAX_PATH];
    TCHAR m_szCountryCode[MAX_PATH];

    int m_nStaCount;
    char m_szStaList[_MAX_STATIONS][MAX_PATH];

    int m_nACLCount;
    char m_szACLList[AP_ACL_SIZE][MAX_PATH];
	int m_nACLPolicy;

    UINT m_nStationLimit;
    UINT m_nMaxStationLimit;
    ConnectionCtrl GPRSAdapter; 

    enOpMode GetCurrentOpMode ();
    BOOL SetStationMode ();
    BOOL SetStationConfig ();
    BOOL SetAPMode (TCHAR *pszSSID, TCHAR *pszChannel);
    BOOL SetAPMode (TCHAR *pszSSID, TCHAR *pszChannel, TCHAR *pszDTIM, USHORT nBeaconInterval, 
                    BOOL bIsHidden, BOOL bIsEnableProtection, BOOL bIsEnableIntraBSS);
	BOOL SetAPConfig (TCHAR *pszPrivate, TCHAR *pszPublic, BOOL bUpdateAll);
    BOOL SetSecurity (enAuthMode enAuth, enCipherMode enCipher, short nKeyIndex, TCHAR *pszKey);
    BOOL SetHiddenSsid(BOOL bIsHidden);
    BOOL SetBGProtection(BOOL bIsEnable);
	BOOL SetIntraBss(BOOL bIsEnableIntraBSS);
    BOOL SetChannel(TCHAR *pszChannel);
    BOOL SetDTIM(TCHAR *pszDTIM);
    BOOL SetBeaconInterval(USHORT nBeaconInterval);
    BOOL SetCountryCode(TCHAR *pszCountry);
    BOOL SetStationLimit(TCHAR *pszStations);
	BOOL SetAclPolicy(int nPolicy);
    BOOL GetStations();
    int is_mac_null(A_UINT8 *mac);
    BOOL GetACL();
    BOOL UpdateACL(TCHAR *pszACL, int nAction);
    BOOL wmic_ether_aton_wild(TCHAR *orig, A_UINT8 *eth, A_UINT8 *wild);
    BOOL GetAPModeStatus();
    BOOL GetAPCryptStatus();
    BOOL GetCountryCode();
    BOOL GetStationLimit();
    void ClearAPModeStatus();
};
