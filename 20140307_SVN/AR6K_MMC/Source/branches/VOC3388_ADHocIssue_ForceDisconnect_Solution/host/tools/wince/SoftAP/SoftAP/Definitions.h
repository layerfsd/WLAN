
# pragma once

//Macro definitions

#define _szDeviceName       _T("AR6K_SD1\0\0")
#define _szAppName          "[SoftAP] "
#define _szAppNameW         _T(_szAppName)        
#define _ERRX(x)            _szAppName "ERR " ## x ## ",Err:%u, %s:%u\n"
#define _szLogFileName      "soft-ap.txt"
#if SOFT_AP_DBG
#define _Trace(x)           _szAppName ## x ## "\n"
#define _tPRINTF			_tprintf
#define _ERR(x)\
{\
	DWORD dwErr = GetLastError ();\
	FILE * fLog = fopen (_szLogFileName, "a+");\
	if (NULL != fLog)\
	{\
		fprintf(fLog, _ERRX (x), dwErr, __FILE__, __LINE__);\
		fclose(fLog);\
	}\
}
#else
#define _Trace(x)           ""
#define _tPRINTF	
#define _ERR(x)             printf (_ERRX (x),GetLastError (), __FILE__, __LINE__)
#endif
#define _LogDebugTrace(x)\
{\
    TCHAR szBuf [MAX_PATH]; char szErr[MAX_PATH];\
    _stprintf (szBuf, _T ("%s"), x);\
    wcstombs (szErr, szBuf, _tcslen(szBuf));\
    FILE * fLog = fopen (_szLogFileName, "a+");\
    if (NULL != fLog)\
    {\
        fprintf(fLog, "%s%s, %s:%u\n", _szAppName, szErr, __FILE__, __LINE__);\
        fclose(fLog);\
    }\
}


#if SOFT_AP_DBG
#define _LogTrace(x)        _LogDebugTrace(x)
#define _LogStatus(x)       {SetDlgItemText (IDC_lbStatus, x); _LogTrace(x);}
#define _LogError(x)        {TCHAR szErr [MAX_PATH]; _stprintf (szErr, _T ("%s, %u"), x, GetLastError ()); _LogStatus (szErr);}
#else
#define _LogTrace(x)		{}
#define _LogStatus(x)       {SetDlgItemText (IDC_lbStatus, x);}
#define _LogError(x)        {TCHAR szErr [MAX_PATH]; _stprintf (szErr, _T ("%s, %u"), x, GetLastError ()); _LogStatus (szErr); _LogDebugTrace (szErr);}
#endif


#define _ClearStatus        SetDlgItemText (IDC_lbStatus, _T(""))

#define _Is_Reg_Err(x)      if (ERROR_SUCCESS != x) \
{\
    _ERR ("Registry");\
    RegCloseKey (KeyHandler);\
    return FALSE;\
}

#define _szAR6K_SD1             _T("AR6K_SD1")
#define _szGW_Disabled          _T("None")
#define _szVMINI1               _T("VMINI1")
#define _szGPRS                 _T("WWAN1")
#define _szDefaultCountryCode   _T("US")
#define _szDisabledCountrycode  _T("FF")


#define _szPrivateKey                  L"PrivateInterface"
#define _szPublicKey                   L"PublicInterface"
#define _szSharingKey                  L"comm\\ConnectionSharing"
#define _szAr6kTcpIpKey                L"comm\\AR6K_SD1\\Parms\\Tcpip"
#define _szDHCPKey                     L"EnableDHCP"
#define _szAutocfgkey                  L"Autocfg"
#define _szGWKey                       L"DefaultGateway"
#define _szIPKey                       L"IPAddress"
#define _szSubMaskKey                  L"SubnetMask"
#define _szDNS                         L"DNS"
#define _szWINS                        L"WINS"
#define _szDefaultIP                   L"192.168.0.1"
#define _szDefaultMask                 L"255.255.255.0"
#define _szDefaultGW                   L"0.0.0.0"
#define _szInternalKey                 "comm\\ConnectionSharing\\InternalExpostedHost"

#define _szDhcpIPAddress               L"DhcpIPAddress"
#define _szDhcpSubnetMask              L"DhcpSubnetMask"
#define _szDhcpDNS                     L"DhcpDNS"
#define _szDhcpDefaultGateway          L"DhcpDefaultGateway"

#define _szVMINIKey                    L"comm\\VMINI1\\parms\\Tcpip"
#define _szConnectShareKey             L"Comm\\ConnectionSharing\\Driver"
#define _szIPDevInstanceName           L"IPD0:"
#define _szNATDevInstanceName          L"NAT0:"

#define _IPROUNTER_DISABLE_CODE  0x128088
#define _IPROUNTER_ENABLE_CODE   0x128084
#define _DHCP_ENABLE_CODE        0x128074

#define _szAppRegKey            _T("Software\\Atheros\\SoftAP")
#define _szAppPublicInterface   _T("PublicInterface")
#define _szAppCountryCode       _T("CountryCode")
#define _szAppPublicDhcpStatus  _T("PublicDhcpStatus")

/*
* Numbering from ISO 3166
*/
typedef enum _enCountryCode {
    A_CTRY_ALBANIA              = 8,       /* Albania */
    A_CTRY_ALGERIA              = 12,      /* Algeria */
    A_CTRY_ARGENTINA            = 32,      /* Argentina */
    A_CTRY_ARMENIA              = 51,      /* Armenia */
    A_CTRY_AUSTRALIA            = 36,      /* Australia */
    A_CTRY_AUSTRIA              = 40,      /* Austria */
    A_CTRY_AZERBAIJAN           = 31,      /* Azerbaijan */
    A_CTRY_BAHRAIN              = 48,      /* Bahrain */
    A_CTRY_BELARUS              = 112,     /* Belarus */
    A_CTRY_BELGIUM              = 56,      /* Belgium */
    A_CTRY_BELIZE               = 84,      /* Belize */
    A_CTRY_BOLIVIA              = 68,      /* Bolivia */
    A_CTRY_BOSNIA_HERZEGOWANIA  = 70,      /* Bosnia & Herzegowania */
    A_CTRY_BRAZIL               = 76,      /* Brazil */
    A_CTRY_BRUNEI_DARUSSALAM    = 96,      /* Brunei Darussalam */
    A_CTRY_BULGARIA             = 100,     /* Bulgaria */
    A_CTRY_CANADA               = 124,     /* Canada */
    A_CTRY_CHILE                = 152,     /* Chile */
    A_CTRY_CHINA                = 156,     /* People's Republic of China */
    A_CTRY_COLOMBIA             = 170,     /* Colombia */
    A_CTRY_COSTA_RICA           = 188,     /* Costa Rica */
    A_CTRY_CROATIA              = 191,     /* Croatia */
    A_CTRY_CYPRUS               = 196,
    A_CTRY_CZECH                = 203,     /* Czech Republic */
    A_CTRY_DENMARK              = 208,     /* Denmark */
    A_CTRY_DOMINICAN_REPUBLIC   = 214,     /* Dominican Republic */
    A_CTRY_ECUADOR              = 218,     /* Ecuador */
    A_CTRY_EGYPT                = 818,     /* Egypt */
    A_CTRY_EL_SALVADOR          = 222,     /* El Salvador */
    A_CTRY_ESTONIA              = 233,     /* Estonia */
    A_CTRY_FAEROE_ISLANDS       = 234,     /* Faeroe Islands */
    A_CTRY_FINLAND              = 246,     /* Finland */
    A_CTRY_FRANCE               = 250,     /* France */
    A_CTRY_FRANCE2              = 255,     /* France2 */
    A_CTRY_GEORGIA              = 268,     /* Georgia */
    A_CTRY_GERMANY              = 276,     /* Germany */
    A_CTRY_GREECE               = 300,     /* Greece */
    A_CTRY_GUATEMALA            = 320,     /* Guatemala */
    A_CTRY_HONDURAS             = 340,     /* Honduras */
    A_CTRY_HONG_KONG            = 344,     /* Hong Kong S.A.R., P.R.C. */
    A_CTRY_HUNGARY              = 348,     /* Hungary */
    A_CTRY_ICELAND              = 352,     /* Iceland */
    A_CTRY_INDIA                = 356,     /* India */
    A_CTRY_INDONESIA            = 360,     /* Indonesia */
    A_CTRY_IRAN                 = 364,     /* Iran */
    A_CTRY_IRAQ                 = 368,     /* Iraq */
    A_CTRY_IRELAND              = 372,     /* Ireland */
    A_CTRY_ISRAEL               = 376,     /* Israel */
    A_CTRY_ITALY                = 380,     /* Italy */
    A_CTRY_JAMAICA              = 388,     /* Jamaica */
    A_CTRY_JAPAN                = 392,     /* Japan */
    A_CTRY_JAPAN1               = 393,     /* Japan (JP1) */
    A_CTRY_JAPAN2               = 394,     /* Japan (JP0) */
    A_CTRY_JAPAN3               = 395,     /* Japan (JP1-1) */
    A_CTRY_JAPAN4               = 396,     /* Japan (JE1) */
    A_CTRY_JAPAN5               = 397,     /* Japan (JE2) */
    A_CTRY_JAPAN6               = 399,     /* Japan (JP6) */
    A_CTRY_JORDAN               = 400,     /* Jordan */
    A_CTRY_KAZAKHSTAN           = 398,     /* Kazakhstan */
    A_CTRY_KENYA                = 404,     /* Kenya */
    A_CTRY_KOREA_NORTH          = 408,     /* North Korea */
    A_CTRY_KOREA_ROC            = 410,     /* South Korea */
    A_CTRY_KOREA_ROC2           = 411,     /* South Korea */
    A_CTRY_KOREA_ROC3           = 412,     /* South Korea */
    A_CTRY_KUWAIT               = 414,     /* Kuwait */
    A_CTRY_LATVIA               = 428,     /* Latvia */
    A_CTRY_LEBANON              = 422,     /* Lebanon */
    A_CTRY_LIBYA                = 434,     /* Libya */
    A_CTRY_LIECHTENSTEIN        = 438,     /* Liechtenstein */
    A_CTRY_LITHUANIA            = 440,     /* Lithuania */
    A_CTRY_LUXEMBOURG           = 442,     /* Luxembourg */
    A_CTRY_MACAU                = 446,     /* Macau */
    A_CTRY_MACEDONIA            = 807,     /* the Former Yugoslav Republic of Macedonia */
    A_CTRY_MALAYSIA             = 458,     /* Malaysia */
    A_CTRY_MALTA                = 470,     /* Malta */
    A_CTRY_MEXICO               = 484,     /* Mexico */
    A_CTRY_MONACO               = 492,     /* Principality of Monaco */
    A_CTRY_MOROCCO              = 504,     /* Morocco */
    A_CTRY_NETHERLANDS          = 528,     /* Netherlands */
    A_CTRY_NETHERLAND_ANTILLES  = 530,     /* Netherlands-Antilles */
    A_CTRY_NEW_ZEALAND          = 554,     /* New Zealand */
    A_CTRY_NICARAGUA            = 558,     /* Nicaragua */
    A_CTRY_NORWAY               = 578,     /* Norway */
    A_CTRY_OMAN                 = 512,     /* Oman */
    A_CTRY_PAKISTAN             = 586,     /* Islamic Republic of Pakistan */
    A_CTRY_PANAMA               = 591,     /* Panama */
    A_CTRY_PARAGUAY             = 600,     /* Paraguay */
    A_CTRY_PERU                 = 604,     /* Peru */
    A_CTRY_PHILIPPINES          = 608,     /* Republic of the Philippines */
    A_CTRY_POLAND               = 616,     /* Poland */
    A_CTRY_PORTUGAL             = 620,     /* Portugal */
    A_CTRY_PUERTO_RICO          = 630,     /* Puerto Rico */
    A_CTRY_QATAR                = 634,     /* Qatar */
    A_CTRY_ROMANIA              = 642,     /* Romania */
    A_CTRY_RUSSIA               = 643,     /* Russia */
    A_CTRY_SAUDI_ARABIA         = 682,     /* Saudi Arabia */
    A_CTRY_MONTENEGRO           = 891,     /* Montenegro */
    A_CTRY_SINGAPORE            = 702,     /* Singapore */
    A_CTRY_SLOVAKIA             = 703,     /* Slovak Republic */
    A_CTRY_SLOVENIA             = 705,     /* Slovenia */
    A_CTRY_SOUTH_AFRICA         = 710,     /* South Africa */
    A_CTRY_SPAIN                = 724,     /* Spain */
    A_CTRY_SRILANKA             = 144,     /* Sri Lanka */
    A_CTRY_SWEDEN               = 752,     /* Sweden */
    A_CTRY_SWITZERLAND          = 756,     /* Switzerland */
    A_CTRY_SYRIA                = 760,     /* Syria */
    A_CTRY_TAIWAN               = 158,     /* Taiwan */
    A_CTRY_THAILAND             = 764,     /* Thailand */
    A_CTRY_TRINIDAD_Y_TOBAGO    = 780,     /* Trinidad y Tobago */
    A_CTRY_TUNISIA              = 788,     /* Tunisia */
    A_CTRY_TURKEY               = 792,     /* Turkey */
    A_CTRY_UAE                  = 784,     /* U.A.E. */
    A_CTRY_UKRAINE              = 804,     /* Ukraine */
    A_CTRY_UNITED_KINGDOM       = 826,     /* United Kingdom */
    A_CTRY_UNITED_STATES        = 840,     /* United States */
    A_CTRY_UNITED_STATES_PS     = 842,     /* United States - public safety */
    A_CTRY_URUGUAY              = 858,     /* Uruguay */
    A_CTRY_UZBEKISTAN           = 860,     /* Uzbekistan */
    A_CTRY_VENEZUELA            = 862,     /* Venezuela */
    A_CTRY_VIET_NAM             = 704,     /* Viet Nam */
    A_CTRY_YEMEN                = 887,     /* Yemen */
    A_CTRY_ZIMBABWE             = 716      /* Zimbabwe */
}enCountryCode;

#define A_CTRY_DEBUG      0
#define A_CTRY_DEFAULT    0x1ff

typedef struct _ST_COUNTRY_CODE_TABLE
{
    enCountryCode Code;
    TCHAR   szName[3];
}ST_COUNTRY_CODE_TABLE;

static ST_COUNTRY_CODE_TABLE g_stCountryCodeTable[] = {
    {(enCountryCode)A_CTRY_DEBUG,       L"DB"},
    {(enCountryCode)A_CTRY_DEFAULT,     L"NA"},
    {A_CTRY_ALBANIA,     L"AL"},
    {A_CTRY_ALGERIA,     L"DZ"},
    {A_CTRY_ARGENTINA,   L"AR"},
    {A_CTRY_ARMENIA,     L"AM"},
    {A_CTRY_AUSTRALIA,   L"AU"},
    {A_CTRY_AUSTRIA,     L"AT"},
    {A_CTRY_AZERBAIJAN,  L"AZ"},
    {A_CTRY_BAHRAIN,     L"BH"},
    {A_CTRY_BELARUS,     L"BY"},
    {A_CTRY_BELGIUM,     L"BE"},
    {A_CTRY_BELIZE,      L"BZ"},
    {A_CTRY_BOLIVIA,     L"BO"},
    {A_CTRY_BOSNIA_HERZEGOWANIA, L"BA"},
    {A_CTRY_BRAZIL,      L"BR"},
    {A_CTRY_BRUNEI_DARUSSALAM, L"BN"},
    {A_CTRY_BULGARIA,    L"BG"},
    {A_CTRY_CANADA,      L"CA"},
    {A_CTRY_CHILE,       L"CL"},
    {A_CTRY_CHINA,       L"CN"},
    {A_CTRY_COLOMBIA,    L"CO"},
    {A_CTRY_COSTA_RICA,  L"CR"},
    {A_CTRY_CROATIA,     L"HR"},
    {A_CTRY_CYPRUS,      L"CY"},
    {A_CTRY_CZECH,       L"CZ"},
    {A_CTRY_DENMARK,     L"DK"},
    {A_CTRY_DOMINICAN_REPUBLIC, L"DO"},
    {A_CTRY_ECUADOR,     L"EC"},
    {A_CTRY_EGYPT,       L"EG"},
    {A_CTRY_EL_SALVADOR, L"SV"},
    {A_CTRY_ESTONIA,     L"EE"},
    {A_CTRY_FINLAND,     L"FI"},
    {A_CTRY_FRANCE,      L"FR"},
    {A_CTRY_FRANCE2,     L"F2"},
    {A_CTRY_GEORGIA,     L"GE"},
    {A_CTRY_GERMANY,     L"DE"},
    {A_CTRY_GREECE,      L"GR"},
    {A_CTRY_GUATEMALA,   L"GT"},
    {A_CTRY_HONDURAS,    L"HN"},
    {A_CTRY_HONG_KONG,   L"HK"},
    {A_CTRY_HUNGARY,     L"HU"},
    {A_CTRY_ICELAND,     L"IS"},
    {A_CTRY_INDIA,       L"IN"},
    {A_CTRY_INDONESIA,   L"ID"},
    {A_CTRY_IRAN,        L"IR"},
    {A_CTRY_IRELAND,     L"IE"},
    {A_CTRY_ISRAEL,      L"IL"},
    {A_CTRY_ITALY,       L"IT"},
    {A_CTRY_JAMAICA,     L"JM"},
    {A_CTRY_JAPAN,       L"JP"},
    {A_CTRY_JORDAN,      L"JO"},
    {A_CTRY_KAZAKHSTAN,  L"KZ"},
    {A_CTRY_KENYA,       L"KE"},
    {A_CTRY_KOREA_NORTH, L"KP"},
    {A_CTRY_KOREA_ROC,   L"KR"},
    {A_CTRY_KOREA_ROC2,  L"K2"},
    {A_CTRY_KOREA_ROC3,  L"K3"},
    {A_CTRY_KUWAIT,      L"KW"},
    {A_CTRY_LATVIA,      L"LV"},
    {A_CTRY_LEBANON,     L"LB"},
    {A_CTRY_LIECHTENSTEIN, L"LI"},
    {A_CTRY_LITHUANIA,   L"LT"},
    {A_CTRY_LUXEMBOURG,  L"LU"},
    {A_CTRY_MACAU,       L"MO"},
    {A_CTRY_MACEDONIA,   L"MK"},
    {A_CTRY_MALAYSIA,    L"MY"},
    {A_CTRY_MALTA,       L"MT"},
    {A_CTRY_MEXICO,      L"MX"},
    {A_CTRY_MONACO,      L"MC"},
    {A_CTRY_MOROCCO,     L"MA"},
    {A_CTRY_NEW_ZEALAND, L"NZ"},
    {A_CTRY_NETHERLANDS, L"NL"},
    {A_CTRY_NETHERLAND_ANTILLES, L"AN"},
    {A_CTRY_NORWAY,      L"NO"},
    {A_CTRY_OMAN,        L"OM"},
    {A_CTRY_PAKISTAN,    L"PK"},
    {A_CTRY_PANAMA,      L"PA"},
    {A_CTRY_PERU,        L"PE"},
    {A_CTRY_PHILIPPINES, L"PH"},
    {A_CTRY_POLAND,      L"PL"},
    {A_CTRY_PORTUGAL,    L"PT"},
    {A_CTRY_PUERTO_RICO, L"PR"},
    {A_CTRY_QATAR,       L"QA"},
    {A_CTRY_ROMANIA,     L"RO"},
    {A_CTRY_RUSSIA,      L"RU"},
    {A_CTRY_SAUDI_ARABIA,L"SA"},
    {A_CTRY_MONTENEGRO,  L"CS"},
    {A_CTRY_SINGAPORE,   L"SG"},
    {A_CTRY_SLOVAKIA,    L"SK"},
    {A_CTRY_SLOVENIA,    L"SI"},
    {A_CTRY_SOUTH_AFRICA,L"ZA"},
    {A_CTRY_SPAIN,       L"ES"},
    {A_CTRY_SRILANKA,    L"LK"},
    {A_CTRY_SWEDEN,      L"SE"},
    {A_CTRY_SWITZERLAND, L"CH"},
    {A_CTRY_SYRIA,       L"SY"},
    {A_CTRY_TAIWAN,      L"TW"},
    {A_CTRY_THAILAND,    L"TH"},
    {A_CTRY_TRINIDAD_Y_TOBAGO, L"TT"},
    {A_CTRY_TUNISIA,     L"TN"},
    {A_CTRY_TURKEY,      L"TR"},
    {A_CTRY_UKRAINE,     L"UA"},
    {A_CTRY_UAE,         L"AE"},
    {A_CTRY_UNITED_KINGDOM, L"GB"},
    {A_CTRY_UNITED_STATES, L"US"},
    {A_CTRY_URUGUAY,     L"UY"},
    {A_CTRY_UZBEKISTAN,  L"UZ"},
    {A_CTRY_VENEZUELA,   L"VE"},
    {A_CTRY_VIET_NAM,    L"VN"},
    {A_CTRY_YEMEN,       L"YE"},
    {A_CTRY_ZIMBABWE,    L"ZW"}
};
static int g_nMaxCountryCode = sizeof (g_stCountryCodeTable) / sizeof (ST_COUNTRY_CODE_TABLE);
