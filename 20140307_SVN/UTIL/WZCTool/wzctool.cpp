//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft shared
// source or premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license agreement,
// you are not authorized to use this source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the SOURCE.RTF on your install media or the root of your tools installation.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
// ----------------------------------------------------------------------------
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
// ----------------------------------------------------------------------------
//
// Module Name:
//
//     wzctool.cpp
//
// Abstract:
//
//     Program to control WZC (wireless zero config) service to configure WiFi.
//
// ----------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#include <eapol.h>
#include <iphlpapi.h>
#include <ntddndis.h>

#include <wzcsapi.h>

// ========================================================================== //
// Macros:
// ========================================================================== //

// Retrieves the number elements in the specified array:
//
#ifndef COUNTOF
#define COUNTOF(arr)  (sizeof(arr) / sizeof(arr[0]))
#endif

// ========================================================================== //
// Logging:
// ========================================================================== //

// Determines whether to write output to the console:
//
static BOOL g_bOutputToConsole = TRUE;

#ifdef UNDER_CE
typedef int (__cdecl *PFN_wprintf)(const wchar_t *, ...);
static PFN_wprintf v_pfn_wprintf = NULL;
static HMODULE     v_hCoreDLL = NULL;
#endif

void OpenLogFile();
void CloseLogFile();
void SaveLogFile(CHAR *pwStr, DWORD dwType, DWORD dwLen);

// -----------------------------------------------------------------------------

void
PrintfMsg
// Logs the specified message to either the console or debug.
// Always adds a newline to the end of the message.
(
	LPCWSTR pFormat,
	...)
{
    HRESULT hr;
	va_list argList;
	WCHAR	buffer[1024];
	CHAR    buf[1024] ={};
	va_start(argList, pFormat);
  __try
    {
        hr = StringCchVPrintfExW(buffer, COUNTOF(buffer), NULL, NULL, STRSAFE_IGNORE_NULLS, pFormat, argList);
    }
  __except(1)
    {
        hr = StringCchPrintfW(buffer, COUNTOF(buffer), L"Error formatting \"%.128s\"", pFormat);
    }
    va_end(argList);

    if (SUCCEEDED(hr) || hr == STRSAFE_E_INSUFFICIENT_BUFFER)
    {
    	if (g_bOutputToConsole)
        {
#ifdef UNDER_CE
    		if (NULL == v_pfn_wprintf)
            {
    			// Since not all configs contain the wprintf function we'll
    			// try to find it.  If it's not there we'll default to using
    			// OutputDebugString.
    			v_hCoreDLL = LoadLibraryW(L"coredll.dll");
    			if (NULL != v_hCoreDLL)
                {
    				v_pfn_wprintf = (PFN_wprintf)GetProcAddressW(v_hCoreDLL, L"wprintf");
    			}
    			
    		}
    		if (NULL != v_pfn_wprintf)
            {
    			v_pfn_wprintf(L"%s\r\n", buffer);
				
				// Save File log
				wcscat(buffer, L"\r\n");
				wcstombs(buf, buffer, wcslen(buffer));
				//RETAILMSG(1, (L"[WZC] log : %d, %d \r\n", strlen(buf), wcslen(buffer)));
				SaveLogFile(buf, 0, wcslen(buffer));
			}
			else
    		{
    			// Couldn't find the entry point, revert to OutputDebugString()
    			g_bOutputToConsole = FALSE;
    		}
#else
    		_putts(buffer);
    		_putts(TEXT("\n"));
#endif
        }
    	else
    	{
    		OutputDebugStringW(buffer);
    	}
    }
} // PrintfMsg()



// -----------------------------------------------------------------------------
void
PrintfMsgList
// Logs the specified list of messages
(
    IN LPCWSTR *szMsgBlock
)
{
    for (LPCWSTR *sz1 = szMsgBlock ; *sz1 ; sz1++)
    {
        PrintfMsg(L"%s", *sz1);
    }
} // PrintfMsgList()

// -----------------------------------------------------------------------------
BOOL
ValidateMacAddress
// Validates the specified raw MAC address.
(
    IN  const PRAW_DATA prdMAC
)
{
    if (prdMAC == NULL
     || prdMAC->pData == NULL
     || prdMAC->dwDataLen != 6
     || (prdMAC->pData[0] == 0
      && prdMAC->pData[1] == 0
      && prdMAC->pData[2] == 0
      && prdMAC->pData[3] == 0
      && prdMAC->pData[4] == 0
      && prdMAC->pData[5] == 0))
        return FALSE;
    else
        return TRUE;
} // ValidateMacAddress()

// -----------------------------------------------------------------------------
LPWSTR
FormatMacAddress
// Formats the specified raw MAC address.
(
                            IN  const PRAW_DATA prdMAC,
  __out_ecount(BufferChars) OUT LPWSTR          pBuffer,
                            IN  int             BufferChars
)
{
    if (!ValidateMacAddress(prdMAC))
    {
        StringCchCopyW(pBuffer, BufferChars, L"<NULL>");
    }
    else
    {
        StringCchPrintfW(pBuffer, BufferChars,
                         L"%02X:%02X:%02X:%02X:%02X:%02X",
                         (UINT)prdMAC->pData[0],
                         (UINT)prdMAC->pData[1],
                         (UINT)prdMAC->pData[2],
                         (UINT)prdMAC->pData[3],
                         (UINT)prdMAC->pData[4],
                         (UINT)prdMAC->pData[5]);
    }
    return pBuffer;
} // FormatMacAddress()

// -----------------------------------------------------------------------------
LPWSTR
FormatMacAddress
// Formats the specified raw MAC address.
(
                            IN  const NDIS_802_11_MAC_ADDRESS &MAC,
  __out_ecount(BufferChars) OUT LPWSTR                         pBuffer,
                            IN  int                            BufferChars
)
{
    RAW_DATA rdBuffer;
    rdBuffer.dwDataLen = sizeof(MAC);
    rdBuffer.pData     = (BYTE *)(&MAC);
    return FormatMacAddress(&rdBuffer, pBuffer, BufferChars);
} // FormatMacAddress()

// -----------------------------------------------------------------------------
LPWSTR
FormatSsid
// Formats the specified raw SSID.
(
                            IN  const PRAW_DATA prdSsid,
  __out_ecount(BufferChars) OUT LPWSTR          pBuffer,
                            IN  int             BufferChars
)
{
    WCHAR outBuf[MAX_SSID_LEN+1];
    if (prdSsid == NULL
     || prdSsid->pData == NULL
     || prdSsid->dwDataLen == 0
     || prdSsid->pData[0] == (BYTE)0)
    {
        StringCchCopyW(pBuffer, BufferChars, L"<NULL>");
    }
    else
    {
        int ssidLen = static_cast<int>(prdSsid->dwDataLen);
        if (ssidLen > MAX_SSID_LEN)
            ssidLen = MAX_SSID_LEN;
        int wchars = MultiByteToWideChar(CP_ACP, 0,
                                        (LPCSTR)(prdSsid->pData), ssidLen,
                                         outBuf, MAX_SSID_LEN+1);
        if (0 >= wchars || wchars > MAX_SSID_LEN)
            StringCchCopyW(pBuffer, BufferChars, L"<INVALID SSID>");
        else
        {
            outBuf[wchars] = L'\0';
            StringCchCopyW(pBuffer, BufferChars, outBuf);
        }
    }
    return pBuffer;
} // FormatSsid()

// -----------------------------------------------------------------------------
LPWSTR
FormatSsid
// Formats the specified SSID.
(
                            IN  const NDIS_802_11_SSID &Ssid,
  __out_ecount(BufferChars) OUT LPWSTR                  pBuffer,
                            IN  int                     BufferChars
)
{
    RAW_DATA rdBuffer;
    rdBuffer.dwDataLen = Ssid.SsidLength;
    rdBuffer.pData     = const_cast<BYTE *>(Ssid.Ssid);
    return FormatSsid(&rdBuffer, pBuffer, BufferChars);
} // FormatSsid()

// ========================================================================== //
// Run-time Configuration:
// ========================================================================== //

// -----------------------------------------------------------------------------
int
WasOption
// look for argument like '-t' or '/t'.
// returns option index
// returns index of argv[] found, -1 if not found
(
    IN int     argc,    // number ars
    IN LPCWSTR argv[],  // arg array
    IN LPCWSTR szOption // to find ('t')
)
{
    for (int i=0; i<argc; i++)
    {
		//RETAILMSG(1, (TEXT("[WZC] argc: %d, argv[0] : %s "),argc, argv[i]));
        if (((*argv[i] == L'-') || (*argv[i] == L'/'))
         && !wcscmp(argv[i]+1, szOption))
            return i;
    }
    return -1;
} // WasOption()

// -----------------------------------------------------------------------------
int
GetOption
// look for argument like '-t 100' or '/t 100'.
// returns index of '100' if option ('t') is found
// returns -1 if not found
(
    IN  int      argc,        // number ars
    IN  LPCWSTR  argv[],      // arg array
    IN  LPCWSTR  pszOption,   // to find ('n')
    OUT LPCWSTR *ppszArgument // option value ('100')
)
{
    if (!ppszArgument)
        return -1;

    int i = WasOption(argc, argv, pszOption);
    if ((i < 0) || ((i+1) >= argc))
    {
        *ppszArgument = NULL;
        return -1;
    }

    *ppszArgument = argv[i+1];
    return i+1;
} // GetOption()

// ========================================================================== //
// WZC Utilities:
// ========================================================================== //

// First wireless card found by WZC query:
WCHAR g_WirelessCardName[MAX_PATH] = L"";

// -----------------------------------------------------------------------------
BOOL
EnumWirelessNetworkCard
// enumerate wireless network cards detected by WZC
(
   // arg none
)
{
    INTFS_KEY_TABLE IntfsTable;
    IntfsTable.dwNumIntfs = 0;
    IntfsTable.pIntfs = NULL;

    DWORD result = WZCEnumInterfaces(NULL, &IntfsTable);

    if (result != NO_ERROR)
    {
        PrintfMsg(L"WZCEnumInterfaces() error %u", result);
        return FALSE;
    }

    // print the GUIDs
    // note that in CE the GUIDs are simply the device instance name
    // i.e XWIFI11B1, CISCO1, ISLP21, ...
    if (!IntfsTable.dwNumIntfs)
    {
        PrintfMsg(L"system has no wireless card.");
        return FALSE;
    }

    for (unsigned int i=0; i < IntfsTable.dwNumIntfs; i++)
        PrintfMsg(L"wifi-card [%d] = %s", i, IntfsTable.pIntfs[i].wszGuid);

    LocalFree(IntfsTable.pIntfs); // need to free memory allocated by WZC for us.

    return TRUE;
} // EnumWirelessNetworkCard()

// -----------------------------------------------------------------------------
BOOL
GetFirstWirelessNetworkCard
// find the first wireless network cards
// found card name goes to g_WirelessCardName
(
   // arg none
)
{
    g_WirelessCardName[0] = L'\0';

    INTFS_KEY_TABLE IntfsTable;
    IntfsTable.dwNumIntfs = 0;
    IntfsTable.pIntfs = NULL;

    DWORD result = WZCEnumInterfaces(NULL, &IntfsTable);

    if (result != NO_ERROR)
    {
        PrintfMsg(L"WZCEnumInterfaces() error %u", result);
        return FALSE;
    }

    // print the GUIDs
    // note that in CE the GUIDs are simply the device instance name
    // i.e XWIFI11B1, CISCO1, ISLP2, ...
    if (!IntfsTable.dwNumIntfs)
    {
        PrintfMsg(L"system has no wireless card.");
        return FALSE;
    }

    wcsncpy(g_WirelessCardName, IntfsTable.pIntfs[0].wszGuid, MAX_PATH-1);
    PrintfMsg(L"wireless card found: %s", g_WirelessCardName);

    // need to free memory allocated by WZC for us.
    LocalFree(IntfsTable.pIntfs);

    return TRUE;
} // GetFirstWirelessNetworkCard()

// -----------------------------------------------------------------------------
BOOL
QueryWzcIntf
// Query's WZC information for the specified adapter.
// Callers MUST deallocate Intf with WZCDeleteInterfaceEx.
(
    IN  LPCWSTR        pAdapter,
    OUT INTF_ENTRY_EX *pIntf,
    IN  DWORD          IntfFlags, // usually INTF_ALL
    OUT DWORD         *pOutFlags
)
{
    memset(pIntf, 0x00, sizeof(INTF_ENTRY_EX));
    pIntf->wszGuid = const_cast<LPWSTR>(pAdapter);

    DWORD result = WZCQueryInterfaceEx(NULL,
                                       IntfFlags,
                                       pIntf,
                                       pOutFlags);
    if (NO_ERROR != result)
    {
        if (ERROR_FILE_NOT_FOUND == result)
            PrintfMsg(L"WZCQueryInterfaceEx() error %u: adapter \"%s\" not found", result, pAdapter);
        else
            PrintfMsg(L"WZCQueryInterfaceEx() error %u out-flags 0x%08X", result, *pOutFlags);
        return FALSE;
    }

    return TRUE;
} // QueryWzcIntf()

// -----------------------------------------------------------------------------
BOOL
QueryWzcContext
// Query's WZC context information.
(
    OUT WZC_CONTEXT *pContext,
    IN  DWORD        InFlags,  // usually zero
    OUT DWORD       *pOutFlags
)
{
    DWORD result = WZCQueryContext(NULL, InFlags, pContext, pOutFlags);
    if (NO_ERROR != result)
    {
        PrintfMsg(L"WZCQueryContext() error %u out-flags 0x%08X", result, *pOutFlags);
        return FALSE;
    }

    return TRUE;
} // QueryWzcContext()

// -----------------------------------------------------------------------------
BOOL
DoRefreshWzc
// refresh WZC
// forces WZC to reconnect Preferred Networks List
(
    IN LPCWSTR pAdapter
)
{
    INTF_ENTRY_EX Intf;
    DWORD dwOutFlags;

    memset(&Intf, 0x00, sizeof(INTF_ENTRY_EX));
    Intf.wszGuid = const_cast<LPWSTR>(pAdapter);

    DWORD result = WZCRefreshInterfaceEx(NULL,
                                         INTF_ALL,
                                         &Intf,
                                         &dwOutFlags);
    if (NO_ERROR != result)
        PrintfMsg(L"WZCRefreshInterfaceEx error %u out-flags 0x%08X", result, dwOutFlags);
    else
        PrintfMsg(L"WZCRefreshInterfaceEx succeeded");

    return result? FALSE : TRUE;
} // DoRefreshWzc()

// ========================================================================== //
// WZC Parameters:
// ========================================================================== //

// -----------------------------------------------------------------------------
// Authentication modes:
//
struct AuthModeList_t
{
    NDIS_802_11_AUTHENTICATION_MODE mode;
    LPCWSTR                         realName;
    LPCWSTR                         aliasName;
}
s_AuthModeList[] =
{
  { Ndis802_11AuthModeOpen,      L"Ndis802_11AuthModeOpen",      L"open" },
  { Ndis802_11AuthModeShared,    L"Ndis802_11AuthModeShared",    L"shared" },
  { Ndis802_11AuthModeAutoSwitch,L"Ndis802_11AuthModeAutoSwitch",L"auto" },
  { Ndis802_11AuthModeWPA,       L"Ndis802_11AuthModeWPA",       L"wpa" },
  { Ndis802_11AuthModeWPAPSK,    L"Ndis802_11AuthModeWPAPSK",    L"wpa-psk" },
  { Ndis802_11AuthModeWPANone,   L"Ndis802_11AuthModeWPANone",   L"wpa-none" },
  { Ndis802_11AuthModeWPA2,      L"Ndis802_11AuthModeWPA2",      L"wpa2" },
  { Ndis802_11AuthModeWPA2PSK,   L"Ndis802_11AuthModeWPA2PSK",   L"wpa2-psk" },
};

// -----------------------------------------------------------------------------
LPCWSTR
AuthenticationMode2Text
// Translates the specified authentication-mode into text form.
(
    NDIS_802_11_AUTHENTICATION_MODE AuthMode
)
{
    return (0 <= AuthMode && AuthMode < COUNTOF(s_AuthModeList))
         ? s_AuthModeList[AuthMode].realName
         : L"<unknown mode>";
} // AuthenticationMode2Text()

// -----------------------------------------------------------------------------
NDIS_802_11_AUTHENTICATION_MODE
Text2AuthenticationMode
// Translates from text to authentication mode.
(
    LPCWSTR AuthName
)
{
    for (int ax = 0 ; ax < COUNTOF(s_AuthModeList) ; ++ax)
        if (wcsicmp(AuthName, s_AuthModeList[ax].realName) == 0)
            return s_AuthModeList[ax].mode;
    for (int ax = 0 ; ax < COUNTOF(s_AuthModeList) ; ++ax)
        if (wcsicmp(AuthName, s_AuthModeList[ax].aliasName) == 0)
            return s_AuthModeList[ax].mode;
    return Ndis802_11AuthModeMax;
} // Text2AuthenticationMode()

// -----------------------------------------------------------------------------
// Encryption modes:
//
struct CipherModeList_t
{
    ULONG   mode;
    LPCWSTR realName;
    LPCWSTR aliasName;
}
s_CipherModeList[] =
{
  { Ndis802_11WEPEnabled,           L"Ndis802_11WEPEnabled",           L"wep" },
  { Ndis802_11WEPDisabled,          L"Ndis802_11WEPDisabled",          L"disabled" },
  { Ndis802_11WEPKeyAbsent,         L"Ndis802_11WEPKeyAbsent",         L"wep-no-keys" },
  { Ndis802_11WEPNotSupported,      L"Ndis802_11WEPNotSupported",      L"unsupported" },
  { Ndis802_11Encryption2Enabled,   L"Ndis802_11Encryption2Enabled",   L"tkip" },
  { Ndis802_11Encryption2KeyAbsent, L"Ndis802_11Encryption2KeyAbsent", L"tkip-no-keys" },
  { Ndis802_11Encryption3Enabled,   L"Ndis802_11Encryption3Enabled",   L"aes" },
  { Ndis802_11Encryption3KeyAbsent, L"Ndis802_11Encryption3KeyAbsent", L"aes-no-keys" },
};

// -----------------------------------------------------------------------------
LPCWSTR
CipherMode2Text
// Translates the specified encryption-mode into text form.
(
    ULONG CipherMode)
{
    return (0 <= CipherMode && CipherMode < COUNTOF(s_CipherModeList))
         ? s_CipherModeList[CipherMode].realName
         : L"<unknown mode>";
} // CipherMode2Text()

// -----------------------------------------------------------------------------
ULONG
Text2CipherMode
// Translates the specified string to encription mode.
(
    LPCWSTR CipherName
)
{
    for (int cx = 0 ; cx < COUNTOF(s_CipherModeList) ; ++cx)
        if (wcsicmp(CipherName, s_CipherModeList[cx].realName) == 0)
            return s_CipherModeList[cx].mode;
    for (int cx = 0 ; cx < COUNTOF(s_CipherModeList) ; ++cx)
        if (wcsicmp(CipherName, s_CipherModeList[cx].aliasName) == 0)
            return s_CipherModeList[cx].mode;
    return ULONG_MAX;
} // Text2CipherMode()

// -----------------------------------------------------------------------------
// EAP-authentication modes:
//
struct EapAuthModeList_t
{
    DWORD   mode;
    LPCWSTR realName;
    LPCWSTR aliasName;
}
s_EapAuthModeList[] =
{
  { EAP_TYPE_TLS,  L"EAP_TYPE_TLS",  L"tls" },
  { EAP_TYPE_PEAP, L"EAP_TYPE_PEAP", L"peap" },
  { EAP_TYPE_MD5,  L"EAP_TYPE_MD5",  L"md5" },
};

// -----------------------------------------------------------------------------
LPCWSTR
EapAuthMode2Text
// Translates the specified encryption-mode into text form.
(
    DWORD EapAuthMode)
{
    return (0 <= EapAuthMode && EapAuthMode < COUNTOF(s_EapAuthModeList))
         ? s_EapAuthModeList[EapAuthMode].realName
         : L"<unknown mode>";
} // EapAuthMode2Text()

// -----------------------------------------------------------------------------
DWORD
Text2EapAuthMode
// Translates the specified string to encription mode.
(
    LPCWSTR EapAuthName
)
{
    for (int ex = 0 ; ex < COUNTOF(s_EapAuthModeList) ; ++ex)
        if (wcsicmp(EapAuthName, s_EapAuthModeList[ex].realName) == 0)
            return s_EapAuthModeList[ex].mode;
    for (int ex = 0 ; ex < COUNTOF(s_EapAuthModeList) ; ++ex)
        if (wcsicmp(EapAuthName, s_EapAuthModeList[ex].aliasName) == 0)
            return s_EapAuthModeList[ex].mode;
    return (DWORD)0;
} // Text2EapAuthMode()

// -----------------------------------------------------------------------------
LPCWSTR
InfrastructureMode2Text
// Translates the specified infrastructure-mode into text form.
(
    NDIS_802_11_NETWORK_INFRASTRUCTURE InfraMode)
{
    switch (InfraMode)
    {
        case Ndis802_11IBSS:           return L"Ndis802_11IBSS";
        case Ndis802_11Infrastructure: return L"Ndis802_11Infrastructure";
        case Ndis802_11AutoUnknown:    return L"Ndis802_11AutoUnknown";
        default:                       return L"<unknown mode>";
    }
} // InfrastructureMode2Text()

// -----------------------------------------------------------------------------
LPCWSTR
NetworkType2Text
// Translates the specified network-type into text form.
(
    NDIS_802_11_NETWORK_TYPE NetworkType)
{
    switch (NetworkType)
    {
        case Ndis802_11FH:       return L"Ndis802_11FH";
        case Ndis802_11DS:       return L"Ndis802_11DS";
        case Ndis802_11OFDM5:    return L"Ndis802_11OFDM5";
        case Ndis802_11OFDM24:   return L"Ndis802_11OFDM24";
        case Ndis802_11Automode: return L"Ndis802_11Automode";
        default:                 return L"<unknown type>";
    }
} // NetworkType2Text()

// -----------------------------------------------------------------------------
DWORD
FormatSupportedRate
// Translates specified supported-rate value into text form.
// Returns the number of characters written into the buffer.
(
                            IN  BYTE   RawValue,
  __out_ecount(BufferChars) OUT LPWSTR pBuffer,
                            IN  int    BufferChars
)
{
    double fRate = ((double)(RawValue & 0x7F)) * 0.5;
    LPWSTR pBuffEnd = pBuffer;
    StringCchPrintfExW(pBuffer, BufferChars, &pBuffEnd, NULL, 0, L"%.1f", fRate);
    return pBuffEnd - pBuffer;
} // FormatSupportedRate()

// -----------------------------------------------------------------------------
int
ChannelFreq2Number
// Translates from 802.11 frequency (in kHz) to channel-number.
(
    long Frequency)
{
    long freq = (Frequency + 500) / 1000;
    int  chan;

    if (freq < 2412)        // Too low
        chan = 0;
    else
    if (freq < 2484)        // Channels 1-13
        chan = 1 + (freq - 2412) / 5;
    else
    if (freq == 2484)       // Channel 14
        chan = 14;
    else
    if (freq < 5000)        // Channels 15-26
        chan = 15 + ((freq - 2512) / 20);
    else
        chan = (freq - 5000) / 5;

    return chan;
} // ChannelFreq2Number()

// ========================================================================== //
// WZC Query:
// ========================================================================== //

// -----------------------------------------------------------------------------
void
PrintConfigItem
// print WZC configuration item
(
    IN const WZC_WLAN_CONFIG &wzcConfig,
    IN BOOL                   bDetailed  // Print detailed info
)
{
    WCHAR buff[1024];
    DWORD buffChars = COUNTOF(buff);

    PrintfMsg(L"   Length                  = %d bytes.", wzcConfig.Length);
    PrintfMsg(L"   dwCtlFlags              = 0x%08X", wzcConfig.dwCtlFlags);

    if (bDetailed)
    {
        PrintfMsg(L"   MacAddress              = %s", FormatMacAddress(wzcConfig.MacAddress, buff, buffChars));
    }

    PrintfMsg(L"   SSID                    = %s", FormatSsid(wzcConfig.Ssid, buff, buffChars));

    PrintfMsg(L"   Authentication          = %d %s", wzcConfig.AuthenticationMode, AuthenticationMode2Text(wzcConfig.AuthenticationMode));

    LPCWSTR pCipherFormat;
    if (wzcConfig.Privacy == Ndis802_11WEPDisabled)
    {
        pCipherFormat = L"   Privacy                 = %d"
                        L" %s (wireless data is not encrypted)";
    }
    else
    {
        pCipherFormat = L"   Privacy                 = %d"
                        L" enabled (encrypted with [%s])";
    }
    PrintfMsg(pCipherFormat, wzcConfig.Privacy, CipherMode2Text(wzcConfig.Privacy));

    PrintfMsg(L"   Infrastructure          = %s", InfrastructureMode2Text(wzcConfig.InfrastructureMode));

    if (bDetailed)
    {
        PrintfMsg(L"   RSSI                    = %d dBm (0=excellent, -100=weak signal)", wzcConfig.Rssi);

        PrintfMsg(L"   NetworkTypeInUse        = %s", NetworkType2Text(wzcConfig.NetworkTypeInUse));

        PrintfMsg(L"   Configuration:");
        PrintfMsg(L"      Struct Length        = %d", wzcConfig.Configuration.Length);
        PrintfMsg(L"      BeaconPeriod         = %d kusec", wzcConfig.Configuration.BeaconPeriod);
        PrintfMsg(L"      ATIMWindow           = %d kusec", wzcConfig.Configuration.ATIMWindow);
        PrintfMsg(L"      DSConfig             = %d kHz (ch-%d)", wzcConfig.Configuration.DSConfig, ChannelFreq2Number(wzcConfig.Configuration.DSConfig));
        PrintfMsg(L"      FHConfig:");
        PrintfMsg(L"         Struct Length     = %d" ,wzcConfig.Configuration.FHConfig.Length);
        PrintfMsg(L"         HopPattern        = %d", wzcConfig.Configuration.FHConfig.HopPattern);
        PrintfMsg(L"         HopSet            = %d", wzcConfig.Configuration.FHConfig.HopSet);
        PrintfMsg(L"         DwellTime         = %d", wzcConfig.Configuration.FHConfig.DwellTime);

        buff[0] = L'\0';
        DWORD charsUsed = 0;
        DWORD charsLeft = buffChars;
        WCHAR separator = L' ';
        for (int j=0; j<NDIS_802_11_LENGTH_RATES; j++)
        {
            if (wzcConfig.SupportedRates[j])
            {
                buff[charsUsed] = separator;
                separator = L',';
                charsUsed++;
                charsLeft--;

                size_t chars = FormatSupportedRate(wzcConfig.SupportedRates[j],
                                                  &buff[charsUsed], charsLeft);
                charsUsed += chars;
                charsLeft -= chars;
            }
        }
        PrintfMsg(L"   SupportedRates          =%s (Mbit/s)", buff);

        PrintfMsg(L"   KeyIndex                = <not available> (beaconing packets don't have this info)"); // wzcConfig.KeyIndex
        PrintfMsg(L"   KeyLength               = <not available> (beaconing packets don't have this info)"); // wzcConfig.KeyLength
        PrintfMsg(L"   KeyMaterial             = <not available> (beaconing packets don't have this info)");

        PrintfMsg(L"   rdUserData length       = %d bytes.",  wzcConfig.rdUserData.dwDataLen);
    }
} // PrintConfigItem()

// -----------------------------------------------------------------------------
void
PrintConfigList
// print WZC configuration list
// used when printing [Available Networks] and [Preferred Networks]
(
    IN LPCWSTR         pListName,
    IN const PRAW_DATA prdBSSIDList,
    IN BOOL            bDetailed    // Print detailed info
)
{
    if (prdBSSIDList == NULL || prdBSSIDList->dwDataLen == 0)
    {
        PrintfMsg(L"");
        PrintfMsg(L"%s List <EMPTY>", pListName);
    }
    else
    {
        const PWZC_802_11_CONFIG_LIST pConfigList = (const PWZC_802_11_CONFIG_LIST)prdBSSIDList->pData;

        PrintfMsg(L"");
        PrintfMsg(L"%s List [%d] entries:", pListName, pConfigList->NumberOfItems);

        for (UINT i = 0; i < pConfigList->NumberOfItems; i++)
        {
            PrintfMsg(L"");
            PrintfMsg(L"******** List Entry Number [%d] ********", i);
            PrintConfigItem(pConfigList->Config[i], bDetailed);
        }
    }
} // PrintConfigList()

#ifdef UNDER_CE

// -----------------------------------------------------------------------------
BOOL
PrintPmkCache
// Displays PMK cache information
(
    IN const INTF_ENTRY_EX &Intf
)
{
    WCHAR buff[80];
    DWORD buffChars = COUNTOF(buff);
    
	DWORD Flags = Intf.PMKCacheFlags;

	PrintfMsg(L"");
	PrintfMsg(L"PMKCache mode:");
	PrintfMsg(L"   Enabled              = %hs",
    		(Flags & INTF_ENTRY_PMKCACHE_FLAG_ENABLE)?               "ON" : "OFF");
	PrintfMsg(L"   Opportunistic        = %hs",
    		(Flags & INTF_ENTRY_PMKCACHE_FLAG_ENABLE_OPPORTUNISTIC)? "ON" : "OFF");
	PrintfMsg(L"   Preauth              = %hs",
    		(Flags & INTF_ENTRY_PMKCACHE_FLAG_ENABLE_PREAUTH)?       "ON" : "OFF");

	PrintfMsg(L"");
	PrintfMsg(L"rdPMKCache              = %u bytes", Intf.rdPMKCache.dwDataLen);
	if (Intf.rdPMKCache.dwDataLen)
	{
		const PNDIS_802_11_PMKID pCache = (const PNDIS_802_11_PMKID)Intf.rdPMKCache.pData;
	
		PrintfMsg(L"   BSSIDInfoCount           : [%u]", pCache->BSSIDInfoCount);

        for (ULONG px = 0 ; px < pCache->BSSIDInfoCount ; ++px)
        {
			PBYTE pId = &pCache->BSSIDInfo[px].PMKID[0];
            PrintfMsg(L"      BSSID=%s  PMKID=%02X%02X..%02X%02X",
                     FormatMacAddress(pCache->BSSIDInfo[px].BSSID, buff, buffChars),
                     pId[0], pId[1], pId[14], pId[15]);
        }
	}		

    return TRUE;
} // PrintPmkCache()

#endif /* UNDER_CE */

// -----------------------------------------------------------------------------
BOOL
PrintWzcIntf
// Queries the WiFi card and prints out the info available for the interface.
// wzctool -q cisco1
//     query cisco1 adapter parameters.
// wzctool -q
//     find wireless card, and query for that adapter.
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    WCHAR buff[1024];
    DWORD buffChars = COUNTOF(buff);

    LPCWSTR pAdapter = NULL;
    if (GetOption(argc, argv, L"q", &pAdapter) < 0)
    {
        GetFirstWirelessNetworkCard();
        if (!*g_WirelessCardName)    // wifi card not found
            return FALSE;
        pAdapter = g_WirelessCardName;
    }

    INTF_ENTRY_EX Intf;
    DWORD dwInFlags = INTF_ALL;
#ifdef UNDER_CE
    dwInFlags |= INTF_PMKCACHE;
#endif
    DWORD dwOutFlags;
    if (!QueryWzcIntf(pAdapter, &Intf, dwInFlags, &dwOutFlags))
        return FALSE;

    PrintfMsg(L"WZCQueryInterfaceEx() for %s", pAdapter);
    PrintfMsg(L"In flags used       = [0x%08X]", dwInFlags);
    PrintfMsg(L"Returned out flags  = [0x%08X]", dwOutFlags);

    // GUID (in CE, GUID=instance name)
    PrintfMsg(L"wzcGuid             = [%s]", Intf.wszGuid);

    // Description
    PrintfMsg(L"wzcDescr            = [%s]", Intf.wszDescr);

    // Print BSSID. BSSID is the MAC address of the AP I am connected.
    if (dwOutFlags & INTF_BSSID)
    {
        LPCWSTR associated;
        if (ValidateMacAddress(&Intf.rdBSSID))
            associated = L"wifi card is in associated state";
        else
            associated = L"wifi card is not associated";

        PrintfMsg(L"BSSID               = %s (%s)", FormatMacAddress(&Intf.rdBSSID, buff, buffChars), associated);
    }
    else
        PrintfMsg(L"BSSID               = <unknown> (not connected)");

    //	Print SSID status
    if (dwOutFlags & INTF_SSID)
    {
        PrintfMsg(L"SSID                = %s", FormatSsid(&(Intf.rdSSID), buff, buffChars));
    }
    else
        PrintfMsg(L"SSID                = <unknown>");

    //	Media Type
    if (dwOutFlags & INTF_NDISMEDIA)
        PrintfMsg(L"Media Type          = [%d]", Intf.ulMediaType);
    else
        PrintfMsg(L"Media Type          = <unknown>");

    //	Configuration Mode
    if (dwOutFlags & INTF_ALL_FLAGS)
    {
        PrintfMsg(L"Configuration Mode  = [%08X]", Intf.dwCtlFlags);
        if (Intf.dwCtlFlags & INTFCTL_ENABLED)
            PrintfMsg(L"   zero conf enabled for this interface");
        if (Intf.dwCtlFlags & INTFCTL_FALLBACK)
            PrintfMsg(L"   attempt to connect to visible non-preferred networks also");
        if (Intf.dwCtlFlags & INTFCTL_OIDSSUPP)
            PrintfMsg(L"   802.11 OIDs are supported by the driver/firmware");
        if (Intf.dwCtlFlags & INTFCTL_VOLATILE)
            PrintfMsg(L"   the service parameters are volatile");
        if (Intf.dwCtlFlags & INTFCTL_POLICY)
            PrintfMsg(L"   the service parameters policy enforced");
    }
    else
        PrintfMsg(L"Configuration Mode  = <unknown>");

    //	Print Infrastructure Mode
    if (dwOutFlags & INTF_INFRAMODE)
    {
        LPCWSTR pInfrType;
        if (Intf.nInfraMode == Ndis802_11IBSS)
            pInfrType = L"IBSS net (adhoc net)";
        else if (Intf.nInfraMode == Ndis802_11Infrastructure)
            pInfrType = L"Infrastructure net (connected to an Access Point)";
        else
            pInfrType = L"Ndis802_11AutoUnknown";

        PrintfMsg(L"Infrastructure Mode = [%d] %s", Intf.nInfraMode, pInfrType);
    }
    else
        PrintfMsg(L"Infrastructure Mode = <unknown>");


    //	Print Authentication Mode
    if (dwOutFlags & INTF_AUTHMODE)
        PrintfMsg(L"Authentication Mode = [%d] %s", Intf.nAuthMode, AuthenticationMode2Text((NDIS_802_11_AUTHENTICATION_MODE)Intf.nAuthMode));
    else
        PrintfMsg(L"Authentication Mode = <unknown>");

    //	Print WEP status
    if (dwOutFlags & INTF_WEPSTATUS)
        PrintfMsg(L"WEP (encryption)    = [%d] %s", Intf.nWepStatus, CipherMode2Text(Intf.nWepStatus));
    else
        PrintfMsg(L"WEP (encryption)    = <unknown>");

    if (dwOutFlags & INTF_CAPABILITIES)
    {
        PrintfMsg(L"Capabilities:");
        if (Intf.dwCapabilities & INTFCAP_SSN)
            PrintfMsg(L"   WPA/TKIP capable");
        if (Intf.dwCapabilities & INTFCAP_80211I)
            PrintfMsg(L"   WPA2/AES capable");
    }

	PrintfMsg(L"rdNicCapabilities   = %d bytes", Intf.rdNicCapabilities.dwDataLen);
	if (Intf.rdNicCapabilities.dwDataLen)
	{
		const PINTF_80211_CAPABILITY pCapability = (const PINTF_80211_CAPABILITY)Intf.rdNicCapabilities.pData;
	
		PrintfMsg(L"   dwNumOfPMKIDs            : [%d]", pCapability->dwNumOfPMKIDs);
		PrintfMsg(L"   dwNumOfAuthEncryptPairs  : [%d]", pCapability->dwNumOfAuthEncryptPairs);

        for (DWORD px = 0 ; px < pCapability->dwNumOfAuthEncryptPairs ; ++px)
        {
            PrintfMsg(L"      Pair[%d]", px+1);

            PrintfMsg(L"         AuthmodeSupported          [%s]",
                AuthenticationMode2Text(pCapability->AuthEncryptPair[px].AuthModeSupported));

            PrintfMsg(L"         EncryptStatusSupported     [%s]",
                CipherMode2Text(pCapability->AuthEncryptPair[px].EncryptStatusSupported));
        }
	}		

#ifdef UNDER_CE
    PrintPmkCache(Intf);
#endif

    PrintfMsg(L"");
    PrintfMsg(L"rdCtrlData              = %d bytes", Intf.rdCtrlData.dwDataLen);	

    PrintConfigList(L"[Available Networks]", &Intf.rdBSSIDList, TRUE);

    PrintConfigList(L"[Preferred Networks]", &Intf.rdStSSIDList, FALSE);	

    // Free memory block that WZC allocated.
    // WZCQueryInterfaceEx() always should be followed by this WZCDeleteIntfObj().
    // Note that the wzcGuid is ** NOT ** freed by WZCDeleteIntfObj()
    WZCDeleteIntfObjEx(&Intf);

    // Get context information.
    PrintfMsg(L"");
    PrintfMsg(L"Zero Config parameters:");

    WZC_CONTEXT wzcContext;
    if (!QueryWzcContext(&wzcContext, 0, &dwOutFlags))
        return FALSE;

    PrintfMsg(L"   tmTr = %d mili-seconds (Scan time out)",                   wzcContext.tmTr);
    PrintfMsg(L"   tmTp = %d mili-seconds (Association time out)",            wzcContext.tmTp);
    PrintfMsg(L"   tmTc = %d mili-seconds (Periodic scan when connected)",    wzcContext.tmTc);
    PrintfMsg(L"   tmTf = %d mili-seconds (Periodic scan when disconnected)", wzcContext.tmTf);

    return TRUE;
} // PrintWzcIntf()

// ========================================================================== //
// WZC Configuration:
// ========================================================================== //

// -----------------------------------------------------------------------------
void
EncryptWepKMaterial
// encrypt WEP key material
// note: this is simply for the security (to protect from memory scanning)
(
    IN OUT WZC_WLAN_CONFIG *pwzcConfig
)
{
    BYTE chFakeKeyMaterial[] = { 0x56, 0x09, 0x08, 0x98, 0x4D, 0x08, 0x11, 0x66, 0x42, 0x03, 0x01, 0x67, 0x66 };
    for (int i = 0; i < WZCCTL_MAX_WEPK_MATERIAL; i++)
        pwzcConfig->KeyMaterial[i] ^= chFakeKeyMaterial[(7*i)%13];
} // EncryptWepKMaterial()

// -----------------------------------------------------------------------------
BOOL
InterpretEncryptionKeyValue
// interpret key value then fill wzcConfig.KeyLength and KeyMaterial[]
// wzcConfig.Privacy should be initialized before calling.
// key is interpreted differently based on the wzcConfig.Privacy
// wzcConfig.Privacy could be one of these
//      Ndis802_11WEPEnabled = WEP key
//      Ndis802_11Encryption2Enabled = TKIP/WPA key
(
    IN OUT WZC_WLAN_CONFIG &wzcConfig,
    IN     LPCWSTR          szEncryptionKey,
    IN     BOOL            &bNeed8021X // this becomes TRUE if szEncryptionKey is "auto"
)
{
    if (wzcConfig.Privacy == Ndis802_11WEPEnabled)
    {
        // WEP key : valid forms are
        // -key 1/0x1234567890 [index=1,40-bit(10-digit hexa)]
        // -key 4/zxcvb [index=4, 40-bit(5-char)
        // -key 3/0x12345678901234567890123456 [index=3, 104-bit(26-digit hexa)
        // -key 2/abcdefghij123 [index=2, 104-bit(13-char)
        // -key auto    [key comes from successful EAP]
        if (wcsicmp(szEncryptionKey, L"auto") == 0)
            bNeed8021X = TRUE;
        else
        {
            if ((szEncryptionKey[0] <  L'1')
             || (szEncryptionKey[0] >  L'4')
             || (szEncryptionKey[1] != L'/'))
            {
                PrintfMsg(L"invalid key index");
                return FALSE;
            }
            wzcConfig.KeyIndex = szEncryptionKey[0] - L'1';

            szEncryptionKey += 2;
            UINT keyLength = wcslen(szEncryptionKey);

            BOOL bAsciiKey;
            switch (keyLength)
            {
                case 5: case 13:
                    bAsciiKey = TRUE;
                    break;
                case 12: case 28:
                    bAsciiKey = FALSE;
                    if ((szEncryptionKey[0] != L'0')
                     || (szEncryptionKey[1] != L'x'))
                    {
                        PrintfMsg(L"invalid HEX key prefix (s/b \"0x\")");
                        return FALSE;
                    }
                    break;
                default:
                    PrintfMsg(L"invalid key length (s/b 5 or 13 (ascii), 10 or 26 (hex)");
                    return FALSE;
            }

            if (bAsciiKey)
            {
                wzcConfig.KeyLength = keyLength;
                for (UINT cx = 0 ; cx < keyLength; ++cx)
                    wzcConfig.KeyMaterial[cx] = (UCHAR)szEncryptionKey[cx];
            }
            else
            {
                szEncryptionKey += 2;
                keyLength       -= 2;
                wzcConfig.KeyLength = keyLength / 2;
                for (UINT cx = 0, abyte = 0 ; cx < keyLength ; ++cx)
                {
                    UINT ch = static_cast<UINT>(szEncryptionKey[cx]);
                    switch (ch)
                    {
                        case L'0': case L'1': case L'2': case L'3': case L'4':
                        case L'5': case L'6': case L'7': case L'8': case L'9':
                            ch -= L'0';
                            break;
                        case L'a': case L'b': case L'c':
                        case L'd': case L'e': case L'f':
                            ch -= L'a' - 10;
                            break;
                        case L'A': case L'B': case L'C':
                        case L'D': case L'E': case L'F':
                            ch -= L'A' - 10;
                            break;
                        default:
                            PrintfMsg(L"invalid HEX WEP key");
                            return FALSE;
                    }
                    if ((cx & 1) == 0)
                    {
                        abyte = ch;
                    }
                    else
                    {
                        abyte <<= 4;
                        abyte |= ch;
                        wzcConfig.KeyMaterial[cx/2] = static_cast<BYTE>(abyte);
                    }
                }
            }

            EncryptWepKMaterial(&wzcConfig);

            wzcConfig.dwCtlFlags |= WZCCTL_WEPK_PRESENT;
        }
    }
    else
    if (wzcConfig.Privacy == Ndis802_11Encryption2Enabled
     || wzcConfig.Privacy == Ndis802_11Encryption3Enabled)
    {
        // TKIP key
        // -key 12345678   [8-char]
        // -key HelloWorld [10-char]
        // -key abcdefghij1234567890abcdefghij1234567890abcdefghij1234567890abc [63-char]
        // -key auto    [key comes from successful EAP]
        if (wcsicmp(szEncryptionKey, L"auto") == 0)
            bNeed8021X = TRUE;
        else
        {
            wzcConfig.KeyLength = wcslen(szEncryptionKey);
            if ((wzcConfig.KeyLength<8) || (wzcConfig.KeyLength>63))
            {
                PrintfMsg(L"WPA-PSK/TKIP key should be 8-63 char long string");
                return FALSE;
            }

            // WPA/TKIP pre-shared key takes 256 bit key.
            // Everything else is incorrect format.
            // Translates a user password (8 to 63 ascii chars) into a 256 bit network key.
            // We do this for WPA-PSK and WPA-None.

            char szEncryptionKeyValue8[64]; // longest key is 63
            memset(szEncryptionKeyValue8, 0, sizeof(szEncryptionKeyValue8));
            WideCharToMultiByte(CP_ACP,
                                0,
                                szEncryptionKey,
                                wzcConfig.KeyLength+1,
                                szEncryptionKeyValue8,
                                wzcConfig.KeyLength+1,
                                NULL,
                                NULL);
            WZCPassword2Key(&wzcConfig, szEncryptionKeyValue8);

            EncryptWepKMaterial(&wzcConfig);

            wzcConfig.dwCtlFlags |= WZCCTL_WEPK_XFORMAT
                                 |  WZCCTL_WEPK_PRESENT
                                 |  WZCCTL_ONEX_ENABLED;
        }

        wzcConfig.EapolParams.dwEapFlags = EAPOL_ENABLED;
        wzcConfig.EapolParams.dwEapType = DEFAULT_EAP_TYPE;
        wzcConfig.EapolParams.bEnable8021x = TRUE;
        wzcConfig.WPAMCastCipher = Ndis802_11Encryption2Enabled;
    }

    return TRUE;
} // InterpretEncryptionKeyValue()

// -----------------------------------------------------------------------------
BOOL
AddToPreferredNetworkList
// adding to the Preferred Networks List
// Preferred Networks is a list of SSIDs in preference order.
// WZC continuously scans available SSIDs and attempts to connect to the most preferable SSID.
(
    IN LPCWSTR                pAdapter,
    IN const WZC_WLAN_CONFIG &wzcConfig,
    IN LPCWSTR                szSsidToConnect
)
{
    DWORD dwOutFlags = 0;
    INTF_ENTRY_EX Intf;
    if (!QueryWzcIntf(pAdapter, &Intf, INTF_ALL, &dwOutFlags))
        return FALSE;

    WZC_802_11_CONFIG_LIST *pConfigList = (PWZC_802_11_CONFIG_LIST)Intf.rdStSSIDList.pData;
    if (NULL == pConfigList)   // empty [Preferred Networks] list case
    {
        DWORD dwDataLen = sizeof(WZC_802_11_CONFIG_LIST);
        WZC_802_11_CONFIG_LIST *pNewConfigList = (WZC_802_11_CONFIG_LIST *)LocalAlloc(LPTR, dwDataLen);
        pNewConfigList->NumberOfItems = 1;
        pNewConfigList->Index = 0;

        memcpy(pNewConfigList->Config, &wzcConfig, sizeof(wzcConfig));

        Intf.rdStSSIDList.pData = (BYTE*)pNewConfigList;
        Intf.rdStSSIDList.dwDataLen = dwDataLen;

        PrintfMsg(L"Preferred Networks List has no entries");
    }
    else
    {
        ULONG uiNumberOfItems = pConfigList->NumberOfItems;
        for (UINT i=0; i<uiNumberOfItems; i++)
        {
            if (memcmp(&wzcConfig.Ssid, &pConfigList->Config[i].Ssid, sizeof(NDIS_802_11_SSID)) == 0)
            {
                PrintfMsg(L"%s is already in the Preferred Networks list", szSsidToConnect);
                WZCDeleteIntfObjEx(&Intf);
                return TRUE;
            }
        }

        PrintfMsg(L"Preferred Networks List has [%d] entries", uiNumberOfItems);

        DWORD dwDataLen = sizeof(WZC_802_11_CONFIG_LIST) + (uiNumberOfItems+1)*sizeof(WZC_WLAN_CONFIG);
        WZC_802_11_CONFIG_LIST *pNewConfigList = (WZC_802_11_CONFIG_LIST *)LocalAlloc(LPTR, dwDataLen);
        pNewConfigList->NumberOfItems = uiNumberOfItems + 1;
        pNewConfigList->Index = 0;

        memcpy(pNewConfigList->Config, &wzcConfig, sizeof(wzcConfig));

        if (pConfigList->NumberOfItems)
        {
            pNewConfigList->Index = pConfigList->Index;
            memcpy(pNewConfigList->Config+1, pConfigList->Config, (uiNumberOfItems)*sizeof(WZC_WLAN_CONFIG));
            LocalFree(pConfigList);
            pConfigList = NULL;
        }

        Intf.rdStSSIDList.pData = (BYTE*)pNewConfigList;
        Intf.rdStSSIDList.dwDataLen = dwDataLen;
    }

    PrintfMsg(L"adding %s to top of List:", szSsidToConnect); // this will be the most preferable SSID
    PrintConfigItem(wzcConfig, FALSE);

    DWORD result = WZCSetInterfaceEx(NULL, INTF_PREFLIST, &Intf, &dwOutFlags);
    if (NO_ERROR != result)
        PrintfMsg(L"WZCSetInterfaceEx() error %u, dwOutFlags=0x%08X", result, dwOutFlags);

    WZCDeleteIntfObjEx(&Intf);

    return result? FALSE : TRUE;
}// AddToPreferredNetworkList()

// ========================================================================== //
// WZC Connections:
// ========================================================================== //

// -----------------------------------------------------------------------------
BOOL
GetIPAddresses
// Gets the IP addresses for GetIPAddresses.
(
                             LPCWSTR pAdapterName,
  __out_ecount(AddressChars) LPSTR   pIPAddress,
                             int     AddressChars,
                             BOOL    bAcceptTempIPAddr
)
{
    DWORD result;

    IP_ADAPTER_INFO *pAdapterInfo = NULL;
    ULONG             adapterSize = 0;

    // Get the adapter information table.
    result = GetAdaptersInfo(pAdapterInfo, &adapterSize);
    if (ERROR_BUFFER_OVERFLOW == result)
    {
        pAdapterInfo = (IP_ADAPTER_INFO *) LocalAlloc(LPTR, adapterSize);
        if (NULL == pAdapterInfo)
            result = ERROR_OUTOFMEMORY;
        else
        {
            result = GetAdaptersInfo(pAdapterInfo, &adapterSize);
        }
    }

    if (NO_ERROR != result)
    {
        if (pAdapterInfo) LocalFree(pAdapterInfo);
        return FALSE;
    }

    // Translate the adapter name to ASCII.
    char asciiName[128];
    int asChars = WideCharToMultiByte(CP_ACP, 0, pAdapterName, -1,
                                      asciiName, COUNTOF(asciiName), NULL, NULL);
    if (0 >= asChars)
    {
        LocalFree(pAdapterInfo);
        return FALSE;
    }

    // Find the adapter in the list.
    BOOL bRtn = FALSE;
    for (; NULL != pAdapterInfo ; pAdapterInfo = pAdapterInfo->Next)
    {
        if (strncmp(pAdapterInfo->AdapterName, asciiName,
                                       COUNTOF(asciiName)) != 0)
            continue;

        // Check each of its IP addresses.
        IP_ADDR_STRING *pAddressList = &(pAdapterInfo->IpAddressList);
        for (; NULL != pAddressList ; pAddressList = pAddressList->Next)
        {
            // Check whether it's a real IP address
            const char *pAddress = pAddressList->IpAddress.String;
            if (strncmp(pAddress, "0.0.", 4) == 0)
                continue;
            if (strncmp(pAddress, "169.", 4) == 0 && !bAcceptTempIPAddr)
                continue;
            result = NO_ERROR;

            // Get the IP address.
            strncpy(pIPAddress, pAddress, AddressChars);
            bRtn = TRUE;
            break;
        }
        break;
    }

    return bRtn;
} // GetIPAddresses()

// -----------------------------------------------------------------------------
inline long
SubtractTickCounts
// Safely calculates tick-count deltas in presence of negative counts.
// Negative tick-counts ocurr when debug images first start up.
(
    IN DWORD TickCount1,
    IN DWORD TickCount2)
{
    return (long)((TickCount1 > TickCount2)
               ?  (TickCount1 - TickCount2)
               : ~(TickCount2 - TickCount1) + 1);
} // SubtractTickCounts()

// -----------------------------------------------------------------------------
BOOL
WaitForConnection
// Waits for the specified connection to complete.
(
    IN LPCWSTR                pAdapter,
    IN const WZC_WLAN_CONFIG &wzcConfig,
    IN DWORD                  Timeout
)
{
    WCHAR buff[128];
    DWORD buffChars = COUNTOF(buff);

    const long ConnectTimeMS     = Timeout * 1000;
    const long StableTimeMS      = 6*1000;  // ms connection must remain stable
    const long CheckIntervalMS   = 250;     // granularity between state checks
    const long RefreshIntervalMS = 20*1000; // time between WZC refreshes

    PrintfMsg(L"Waiting %u seconds for connection to \"%s\"", Timeout, FormatSsid(wzcConfig.Ssid, buff, buffChars));

    // Ad Hoc uses a temporary IP address.
    // Allow those to satisfy GetIPAddress.
    BOOL bAcceptTempIPAddr = wzcConfig.InfrastructureMode == Ndis802_11IBSS;

    enum States_e { Idle, Disconnected, Associated, Connected, CorrectSSID };
    States_e lastState = Idle, state = Disconnected;

    DWORD currentTime, startTime, changeStart, ssidStart, stableStart;
    currentTime = startTime = GetTickCount();

    do
    {
        // Track the state-change times.
        if (lastState != state)
        {
            lastState = state;
            changeStart = currentTime;
        }

        // Delay for a while before the next check.
        Sleep(CheckIntervalMS);
        currentTime = GetTickCount();

        // If it has been too long since the last change, refresh WZC.
        long changeDuration;
        changeDuration = SubtractTickCounts(currentTime, changeStart);
        if (RefreshIntervalMS <= changeDuration)
        {
            if (!DoRefreshWzc(pAdapter))
                return FALSE;

            changeStart = currentTime;
            continue;
        }

        // Get the current association status.
        INTF_ENTRY_EX Intf;
        DWORD dwOutFlags;
        if (!QueryWzcIntf(pAdapter, &Intf, INTF_ALL, &dwOutFlags))
            return FALSE;
        currentTime = GetTickCount();

        do
        {
            // Make sure we have a connected-BSSID.
            if ((dwOutFlags & INTF_BSSID) == 0
             || !ValidateMacAddress(&Intf.rdBSSID))
            {
                state = Disconnected;
                break;
            }

            // Make sure we have a connected-SSID.
            if ((dwOutFlags & INTF_SSID) == 0
             || (Intf.rdSSID.dwDataLen == 0)
             || (Intf.rdSSID.pData == NULL)
             || (Intf.rdSSID.pData[0] == (BYTE)0))
            {
                state = Disconnected;
                break;
            }

            // Change to Associated (waiting for DHCP) state.
            if (Associated > state)
            {
                long time = SubtractTickCounts(currentTime, startTime);
                PrintfMsg(L"  associated with BSSID %s after %.2f seconds",
                         FormatMacAddress(&(Intf.rdBSSID), buff, buffChars),
                        (double)time / 1000.0);

                state = Associated;
            }

            // Get the current IP assignment (if any).
            char ipAddress[64];
            BOOL bRtn = GetIPAddresses(pAdapter,
                                       ipAddress,
                                       COUNTOF(ipAddress),
                                       bAcceptTempIPAddr);
            currentTime = GetTickCount();
            if (!bRtn)
            {
                state = Associated;
                break;
            }

            // Change to Connected-to-Access-Point state.
            if (Connected > state)
            {
                long time = SubtractTickCounts(currentTime, startTime);
                PrintfMsg(L"  connected with IP %hs after %.02f seconds", ipAddress,
                       (double)time / 1000.0);

                ssidStart = currentTime;
                state = Connected;
            }

            // Check the associated SSID.
            if ((Intf.rdSSID.dwDataLen != wzcConfig.Ssid.SsidLength)
             || memcmp(Intf.rdSSID.pData, wzcConfig.Ssid.Ssid, wzcConfig.Ssid.SsidLength) != 0)
            {
                long time = SubtractTickCounts(currentTime, ssidStart);
                if (StableTimeMS <= time)
                {
                    ssidStart = currentTime;
                    FormatSsid(&(Intf.rdSSID), buff, buffChars);
                    PrintfMsg(L"  still associated with SSID %s", buff);
                }
                break;
            }

            // Change to Connected-to-correct-SSID state.
            if (CorrectSSID > state)
            {
                stableStart = currentTime;
                state = CorrectSSID;
                break;
            }

            // Delay a few seconds in case the connection drops.
            long stableDuration;
            stableDuration = SubtractTickCounts(currentTime, stableStart);
            if (StableTimeMS <= stableDuration)
            {
                PrintfMsg(L"  connection remained stable for %ld seconds",
                        StableTimeMS / 1000);
                WZCDeleteIntfObjEx(&Intf);
                return TRUE;
            }
        }
        while (false);

        // Deallocate the WZC INTF memory.
        WZCDeleteIntfObjEx(&Intf);
    }
    while (ConnectTimeMS > SubtractTickCounts(currentTime, startTime));

    PrintfMsg(L"Waited limit of %ld seconds for connection to finish!", Timeout);
    return FALSE;
} // WaitForConnection()

// -----------------------------------------------------------------------------
BOOL
ConnectFromRegistry
// configure WZC data as the registry setting
// WZCTOOL registry is under HKEY_CURRENT_USER\Comm\WZCTOOL
// sample:
// [HKEY_CURRENT_USER\Comm\WZCTOOL]
//    "SSID"           = "CE8021X",
//    "authentication" = dword:0  ; open (Ndis802_11AuthModeOpen)
//    "encryption"     = dword:0  ; WEP (Ndis802_11WEPEnabled)
//    "key"            = "auto"   ; key generated automatically by EAP
//    "eap"            = "tls"    ; EAP type is TLS (certificate based authentication)
//    "adhoc"          = dword:0  ; CE8021X is an infrastructure network
//    "timeout"        = dword:80 ; waits 80 seconds for connection
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    BOOL bRtn = FALSE;

    if (WasOption(argc, argv, L"?")>0)
    {
        LPCWSTR szOptionHelp[] = {
            L"registry datail:",
            L"  registry path  = HKEY_CURRENT_USER\\Comm\\WZCTOOL",
            L"  SSID           = REG_SZ (max 32 char)",
            L"  authentication = REG_DWORD",
            L"  encryption     = REG_DWORD",
            L"  key            = REG_SZ",
            L"  eap            = REG_SZ",
            L"  adhoc          = REG_DWORD",
            L"  hidden         = REG_DWORD",
            L"  timeout        = REG_DWORD",
            L"",
            NULL };
        PrintfMsgList(szOptionHelp);

        PrintfMsg(L"authentication =");
        for (int ax = 0 ; ax < COUNTOF(s_AuthModeList) ; ++ax)
            PrintfMsg(L" %4d = %-8s (%s)", s_AuthModeList[ax].mode,
                                          s_AuthModeList[ax].aliasName,
                                          s_AuthModeList[ax].realName);
        PrintfMsg(L"");

        PrintfMsg(L"encryption =");
        for (int cx = 0 ; cx < COUNTOF(s_CipherModeList) ; ++cx)
            PrintfMsg(L" %4d = %-12s (%s)", s_CipherModeList[cx].mode,
                                           s_CipherModeList[cx].aliasName,
                                           s_CipherModeList[cx].realName);
        PrintfMsg(L"");

        PrintfMsg(L"eap =");
        for (int ex = 0 ; ex < COUNTOF(s_EapAuthModeList) ; ++ex)
            PrintfMsg(L" %4d = %-4s (%s\r\n)", s_EapAuthModeList[ex].mode,
                                          s_EapAuthModeList[ex].aliasName,
                                          s_EapAuthModeList[ex].realName);
        PrintfMsg(L"");

        PrintfMsg(L"adhoc =");
        PrintfMsg(L"   1 = adhoc net");
        PrintfMsg(L"   if \"adhoc\" value not exists or its value is 0 = inrastructure net");
        PrintfMsg(L"");

        PrintfMsg(L"hidden =");
        PrintfMsg(L"   1 = SSID is not broadcast by APs");
        PrintfMsg(L"   if \"hidden\" value not exists or its value is 0 = broadcast SSID");
        PrintfMsg(L"");

        PrintfMsg(L"timeout =");
        PrintfMsg(L"   N = seconds to wait for connection to be established");
        PrintfMsg(L"   if \"timeout\" value exists and is non-zero the tool will");
        PrintfMsg(L"   wait that long for the connection.");
        PrintfMsg(L"");

        LPCWSTR szRegExample[] = {
            L"registry examples:",
            L"",
            L"HKEY_CURRENT_USER\\Comm\\WZCTOOL",
            L"   \"SSID\" = \"CEWEP40\"",
            L"   \"authentication\" = dword:0  ;OPEN=authentication",
            L"   \"encryption\" = dword:0      ;WEP=encryption",
            L"   \"key\" = \"1/0x1234567890     ;key index=1, 40-bit key\"",
            L"   \"adhoc\" = dword:0",
            L"",
            L"HKEY_CURRENT_USER\\Comm\\WZCTOOL",
            L"   \"SSID\" = \"CEWPA\"",
            L"   \"authentication\" = dword:3  ;WPA=authentication",
            L"   \"encryption\" = dword:6      ;AES=encryption",
            L"   \"eap\" = dword:25            ;EAP=PEAP",
            L"   \"timeout\" = dword:80        ;wait 80 seconds for connection",
            L"",
            NULL };
        PrintfMsgList(szRegExample);

        LPCWSTR szNotes[] = {
            L"Notes:",
            L"  Storing encryption key in registry as a clear text form could be a security concern.",
            L"  Somebody could read keys by scanning registry.",
            L"  Possible way to avoid is encrypt the key values so that only your software can understand.",
            L"",
            NULL };
        PrintfMsgList(szNotes);

        return TRUE;
    }

    HKEY hKey1 = NULL;
    LONG rc = RegOpenKeyEx(HKEY_CURRENT_USER, L"Comm\\WZCTOOL", 0, 0, &hKey1);
    if (rc != NO_ERROR)
        return FALSE;

    do
    {
        WZC_WLAN_CONFIG wzcConfig;
        memset(&wzcConfig, 0, sizeof(wzcConfig));
        wzcConfig.Length = sizeof(wzcConfig);
        wzcConfig.dwCtlFlags = 0;

        BYTE   ucbData[MAX_PATH];
        DWORD *pdwData = (DWORD *)ucbData;
        DWORD dwDataSize = sizeof(ucbData);
        DWORD dwType;

        PrintfMsg(L"registry datail:");
        PrintfMsg(L"  registry path  = HKEY_CURRENT_USER\\Comm\\WZCTOOL");

        // SSID
        dwDataSize = sizeof(ucbData);
        if ((NO_ERROR != RegQueryValueEx(hKey1, L"SSID", NULL, &dwType, ucbData, &dwDataSize))
         || (dwType != REG_SZ)
         || (((LPCWSTR)ucbData)[0] == L'\0'))
        {
            PrintfMsg(L"error no SSID is given. Check usage.");
            break;
        }

        WCHAR szSsidToConnect[MAX_SSID_LEN+1];
        memcpy(szSsidToConnect, ucbData, sizeof(szSsidToConnect));
        szSsidToConnect[MAX_SSID_LEN] = L'\0';
        PrintfMsg(L"  SSID           = \"%s\"", szSsidToConnect);

        int ssidChars = WideCharToMultiByte(CP_ACP, 0,
                                            szSsidToConnect,
                                     wcslen(szSsidToConnect),
                                    (LPSTR)(wzcConfig.Ssid.Ssid),
                                            MAX_SSID_LEN,
                                            NULL, NULL);
        if (0 >= ssidChars)
        {
            PrintfMsg(L"SSID is too long or invalid. Max ASCII length in %d chars.", MAX_SSID_LEN);
            break;
        }
        wzcConfig.Ssid.SsidLength = ssidChars;

        // Authentication
        wzcConfig.AuthenticationMode = Ndis802_11AuthModeOpen;
        dwDataSize = sizeof(ucbData);
        if (
            (NO_ERROR == RegQueryValueEx(hKey1, L"authentication", NULL, &dwType, ucbData, &dwDataSize))
         && (dwType == REG_DWORD))
        {
            PrintfMsg(L"  authentication = %u (%s)", *pdwData, AuthenticationMode2Text((NDIS_802_11_AUTHENTICATION_MODE)*pdwData));
            wzcConfig.AuthenticationMode = (NDIS_802_11_AUTHENTICATION_MODE)(*pdwData);
        }

        // Encryption
        wzcConfig.Privacy = Ndis802_11WEPDisabled;
        dwDataSize = sizeof(ucbData);
        if ((NO_ERROR == RegQueryValueEx(hKey1, L"encryption", NULL, &dwType, ucbData, &dwDataSize))
         && (dwType == REG_DWORD))
        {
            PrintfMsg(L"  encryption     = %u (%s)", *pdwData, CipherMode2Text(*pdwData));
            wzcConfig.Privacy = *pdwData;
        }

        // Key
        LPWSTR szEncryptionKey = (LPWSTR)ucbData;
        dwDataSize = sizeof(ucbData);
        BOOL bNeed8021X = FALSE;
        if ((NO_ERROR == RegQueryValueEx(hKey1, L"key", NULL, &dwType, ucbData, &dwDataSize))
         && (dwType == REG_SZ))
        {
            PrintfMsg(L"  key            = \"%s\"", szEncryptionKey);
            if (!InterpretEncryptionKeyValue(wzcConfig, szEncryptionKey, bNeed8021X))
                break;
        }

        if (bNeed8021X)
        {
            dwDataSize = sizeof(ucbData);
            if ((NO_ERROR == RegQueryValueEx(hKey1, L"eap", NULL, &dwType, ucbData, &dwDataSize))
             && (dwType == REG_DWORD))
            {
                PrintfMsg(L"  eap            = %u (%s)", *pdwData, EapAuthMode2Text(*pdwData));
                wzcConfig.EapolParams.dwEapType = *pdwData;
                wzcConfig.EapolParams.dwEapFlags = EAPOL_ENABLED;
                wzcConfig.EapolParams.bEnable8021x  = TRUE;
                wzcConfig.EapolParams.dwAuthDataLen = 0;
                wzcConfig.EapolParams.pbAuthData    = 0;
            }
            else
            {
                PrintfMsg(L"need eap-type for this option");
                break;
            }
        }

        // adhoc? or infrastructure net?
        wzcConfig.InfrastructureMode = Ndis802_11Infrastructure;
        dwDataSize = sizeof(ucbData);
        if ((NO_ERROR == RegQueryValueEx(hKey1, L"adhoc", NULL, &dwType, ucbData, &dwDataSize))
         && (dwType == REG_DWORD))
        {
            PrintfMsg(L"  adhoc          = %u", *pdwData);
            if (0 != *pdwData)
                wzcConfig.InfrastructureMode = Ndis802_11IBSS;
        }

        // Default to connecting to only APs that are beaconing.
        wzcConfig.dwCtlFlags |= WZCCTL_BROADCAST_SSID;
        dwDataSize = sizeof(ucbData);
        if ((NO_ERROR == RegQueryValueEx(hKey1, L"hidden", NULL, &dwType, ucbData, &dwDataSize))
         && (dwType == REG_DWORD))
        {
            PrintfMsg(L"  hidden         = %u", *pdwData);
            if (0 != *pdwData)
                wzcConfig.dwCtlFlags &= ~WZCCTL_BROADCAST_SSID;
        }

        DWORD timeout = 0;
        dwDataSize = sizeof(ucbData);
        if ((NO_ERROR == RegQueryValueEx(hKey1, L"timeout", NULL, &dwType, ucbData, &dwDataSize))
         && (dwType == REG_DWORD))
        {
            PrintfMsg(L"  timeout        = %u", *pdwData);
            timeout = *pdwData;
        }

        LPCWSTR pAdapter = NULL;
        if ((GetOption(argc, argv, L"registry", &pAdapter) < 0)
         || (*pAdapter == L'-'))
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            pAdapter = g_WirelessCardName;
        }

        if (!AddToPreferredNetworkList(pAdapter, wzcConfig, szSsidToConnect)
         ||(0 != timeout && !WaitForConnection(pAdapter, wzcConfig, timeout)))
            break;

        bRtn = TRUE;
    }
    while (false);

    RegCloseKey(hKey1);

    return bRtn;
} // ConnectFromRegistry()

// -----------------------------------------------------------------------------
BOOL
ConnectWzc
// configure WZC as given arguments.
// as a result of this configuration, WZC will atempt to connect given SSID.
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    if (WasOption(argc, argv, L"?")>0)
    {
        PrintfMsg(L"-c options:");
        PrintfMsg(L"-ssid   SSID to connect. SSID is the name of wireless network");
        PrintfMsg(L"");

        PrintfMsg(L"-auth   authentication mode:");
        for (int ax = 0 ; ax < COUNTOF(s_AuthModeList) ; ++ax)
            PrintfMsg(L"            %-8s = %s", s_AuthModeList[ax].aliasName,
                                               s_AuthModeList[ax].realName);
        PrintfMsg(L"        '-authentication' is same as -auth");
        PrintfMsg(L"        default is open-authentication");
        PrintfMsg(L"");

        PrintfMsg(L"-encr   encription mode:");
        for (int cx = 0 ; cx < COUNTOF(s_CipherModeList) ; ++cx)
            PrintfMsg(L"            %-12s = %s", s_CipherModeList[cx].aliasName,
                                                s_CipherModeList[cx].realName);
        PrintfMsg(L"        '-encryption' is same as -encr");
        PrintfMsg(L"        WEP and TKIP uld have '-key' option");
        PrintfMsg(L"        default is encryption-disabled");
        PrintfMsg(L"");

        PrintfMsg(L"-key    key value.");
        PrintfMsg(L"        for WEP-key, use '#/<key-value>' form.");
        PrintfMsg(L"        '#' is key-index (1-4), '<key-value>' is WEP key value (40-bit or 104-bit).");
        PrintfMsg(L"        40-bit is either '10-digit hexa numbers' (ex: 0x1234567890) or '5-char ASCII string' (ex: zxcvb)");
        PrintfMsg(L"        104-bit is either '26-digit hexa numbers' (ex: 0x12345678901234567890123)");
        PrintfMsg(L"                       or '13-char ASCII string' (ex: abcdefghijklm)");
        PrintfMsg(L"        for TKIP-key, use '<key-value>' form. (no key index)");
        PrintfMsg(L"        TKIP-key can be 8-63 char ASCII string (ex: asdfghjk)");
        PrintfMsg(L"");

        PrintfMsg(L"-eap    EAP mode:");
        for (int ex = 0 ; ex < COUNTOF(s_EapAuthModeList) ; ++ex)
            PrintfMsg(L"            %-4s = %s", s_EapAuthModeList[ex].aliasName,
                                               s_EapAuthModeList[ex].realName);
        PrintfMsg(L"        this is for 802.1X (EAP). both AP and STA will get keys automatically after the successful EAP.");
        PrintfMsg(L"        so, '-eap' should have '-key auto' option.");
        PrintfMsg(L"        UI dialogs will popup and ask user credentials (like certificate or user-name/password).");
        PrintfMsg(L"");

        PrintfMsg(L"-adhoc  connects to an adhoc net.");
        PrintfMsg(L"        if this is not given, by default connecting to an AP (infrastructure net).");
        PrintfMsg(L"");

        PrintfMsg(L"-hidden connects to an AP that is not broadcasting its beacon.");

        PrintfMsg(L"-timeout waits specified seconds for connection to complete");

        PrintfMsg(L"-example  shows example usage.");

        PrintfMsg(L"note");
        PrintfMsg(L"1. options are case insensitive. (ie. '-auth'=='-AUTH', 'open'=='OPEN')");
        PrintfMsg(L"2. but the SSID and key-values are case sensitive. (ie. 'TEST1'!='test1', 'abcdefgh'!='ABCDEFGH'");
        PrintfMsg(L"3. giving WEP key in a command line could be a security concern.");
        PrintfMsg(L"   somebody could watch keys over your shoulder.");
        PrintfMsg(L"");

        return TRUE;
    }
    else
    if ((WasOption(argc, argv, L"ex")>0) || (WasOption(argc, argv, L"example")>0))
    {
        LPCWSTR szOptionExamples[] = {
            L"examples:",
            L"wzctool -c -ssid CEOPEN -auth open -encr disabled",
            L"wzctool -c -ssid CEOPEN    (same as above)",
            L"wzctool -c -ssid CESHARED -auth open -encr WEP -key 1/0x1234567890",
            L"wzctool -c -ssid CESHARED2 -auth open -encr WEP -key 4/zxcvb",
            L"wzctool -c -ssid CESHARED3 -auth shared -encr WEP -key 1/0x1234567890",
            L"wzctool -c -ssid CE8021X -auth open -encr wep -key auto -eap tls",
            L"wzctool -c -ssid WPAPSK -auth wpa-psk -encr tkip -key qwertyuiop",
            L"wzctool -c -ssid WPA -auth wpa -encr tkip -key auto -eap peap",
            L"wzctool -c -ssid CEAD1 -adhoc",
            L"wzctool -c -ssid CEADWEP104 -adhoc -auth open -encr WEP -key 1/abcdefghijabc",
            NULL };
        PrintfMsgList(szOptionExamples);

        return TRUE;
    }

    WZC_WLAN_CONFIG wzcConfig;
    memset(&wzcConfig, 0, sizeof(wzcConfig));
    wzcConfig.Length = sizeof(wzcConfig);
    wzcConfig.dwCtlFlags = 0;

    LPCWSTR pAdapter = NULL;
    if ((GetOption(argc, argv, L"c", &pAdapter) < 0)
     || (*pAdapter == L'-'))
    {
        GetFirstWirelessNetworkCard();
        if (!*g_WirelessCardName)    // wifi card not found
            return FALSE;
        pAdapter = g_WirelessCardName;
    }

    // SSID
    LPCWSTR szSsidToConnect = NULL;
    if (GetOption(argc, argv, L"ssid", &szSsidToConnect) < 0 || L'\0' == szSsidToConnect[0])
    {
        PrintfMsg(L"no SSID is given");
        return FALSE;
    }
    int ssidChars = WideCharToMultiByte(CP_ACP, 0,
                                        szSsidToConnect,
                                 wcslen(szSsidToConnect),
                                (LPSTR)(wzcConfig.Ssid.Ssid),
                                        MAX_SSID_LEN,
                                        NULL, NULL);
    if (0 >= ssidChars)
    {
        PrintfMsg(L"SSID is too long or invalid. Max length %d chars.", MAX_SSID_LEN);
        return FALSE;
    }
    wzcConfig.Ssid.SsidLength = ssidChars;

    // adhoc? or infrastructure net?
    wzcConfig.InfrastructureMode =
        (WasOption(argc, argv, L"adhoc") > 0) ? Ndis802_11IBSS : Ndis802_11Infrastructure;

    // Default to connecting to only APs that are beaconing.
    wzcConfig.dwCtlFlags |= WZCCTL_BROADCAST_SSID;
    if (WasOption(argc, argv, L"hidden") > 0)
        wzcConfig.dwCtlFlags &= ~WZCCTL_BROADCAST_SSID;

    // Authentication
    wzcConfig.AuthenticationMode = Ndis802_11AuthModeOpen;
    LPCWSTR szAuthMode = NULL;
    if ((GetOption(argc, argv, L"auth",           &szAuthMode) > 0)
     || (GetOption(argc, argv, L"authentication", &szAuthMode) > 0))
    {
        NDIS_802_11_AUTHENTICATION_MODE authMode = Text2AuthenticationMode(szAuthMode);
        if (Ndis802_11AuthModeMax != authMode)
            wzcConfig.AuthenticationMode = (NDIS_802_11_AUTHENTICATION_MODE)authMode;
        else
            PrintfMsg(L"unknown auth mode, assuming 'open'");
    }
    else
        PrintfMsg(L"missing auth mode (-auth), assuming 'open'");

    // Encryption
    wzcConfig.Privacy = Ndis802_11WEPDisabled;
    LPCWSTR szCipherMode = NULL;
    if ((GetOption(argc, argv, L"encr",       &szCipherMode) > 0)
     || (GetOption(argc, argv, L"encryption", &szCipherMode) > 0))
    {
        ULONG cipherMode = Text2CipherMode(szCipherMode);
        if (ULONG_MAX != cipherMode)
            wzcConfig.Privacy = cipherMode;
        else
            PrintfMsg(L"unknown encryption mode, assuming 'disabled'");
    }
    else
        PrintfMsg(L"missing encryption mode (-encr), assuming 'disabled'");

    // Key
    LPCWSTR szEncryptionKey = NULL;
    if (GetOption(argc, argv, L"key", &szEncryptionKey) < 0)
        szEncryptionKey = L"auto";
    BOOL bNeed8021X = FALSE;
    if (!InterpretEncryptionKeyValue(wzcConfig, szEncryptionKey, bNeed8021X))
        return FALSE;

    if (bNeed8021X)
    {
        wzcConfig.EapolParams.dwEapType = EAP_TYPE_TLS;
        LPCWSTR szEapAuthMode = NULL;
        if (GetOption(argc, argv, L"eap", &szEapAuthMode) > 0)
        {
            DWORD eapAuthMode = Text2EapAuthMode(szEapAuthMode);
            if (0 != eapAuthMode)
                wzcConfig.EapolParams.dwEapType = eapAuthMode;
            else
                PrintfMsg(L"unknown EAP authentication mode, assuming 'TLS'");
        }
        else
            PrintfMsg(L"missing EAP authentication mode (-eap), assuming 'TLS'");

        wzcConfig.EapolParams.dwEapFlags = EAPOL_ENABLED;
        wzcConfig.EapolParams.bEnable8021x  = TRUE;
        wzcConfig.EapolParams.dwAuthDataLen = 0;
        wzcConfig.EapolParams.pbAuthData    = 0;
    }

    LPCWSTR szTimeout = NULL;
    DWORD timeout = 0;
    if (GetOption(argc, argv, L"timeout", &szTimeout) > 0)
    {
        LPWSTR szTimeoutEnd = NULL;
        timeout = wcstoul(szTimeout, &szTimeoutEnd, 10);
        if (NULL == szTimeoutEnd || L'\0' != szTimeoutEnd[0])
        {
            PrintfMsg(L"bad timeout value (s/b number of seconds to wait)");
            return FALSE;
        }
    }

    if (!AddToPreferredNetworkList(pAdapter, wzcConfig, szSsidToConnect)
     ||(0 != timeout && !WaitForConnection(pAdapter, wzcConfig, timeout)))
        return FALSE;

    return TRUE;
} // ConnectWzc()

// ========================================================================== //
// PMK Cache:
// ========================================================================== //

#ifdef UNDER_CE

// -----------------------------------------------------------------------------
BOOL
SetAdapterPMKCache
//  Set the PMK Cache flags specified by the mask "FlagsToSet"
//  to the value specified by the mask "Flags".
(
     IN LPCWSTR pAdapter,
	 IN DWORD   Flags,
	 IN DWORD   FlagsToSet
)
{
	DWORD dwOutFlags = 0;
    INTF_ENTRY_EX Intf;
    if (!QueryWzcIntf(pAdapter, &Intf, INTF_PMKCACHE, &dwOutFlags))
        return FALSE;

	Intf.PMKCacheFlags = (Intf.PMKCacheFlags & ~FlagsToSet) | Flags;
    DWORD result = WZCSetInterfaceEx(NULL, INTF_PMKCACHE, &Intf, &dwOutFlags);
    if (NO_ERROR != result)
        PrintfMsg(L"WZCSetInterfaceEx() error %u dwOutFlags=0x%08X", result, dwOutFlags);

    WZCDeleteIntfObjEx(&Intf);

    return result? FALSE : TRUE;
} // SetAdapterPMKCache()

// -----------------------------------------------------------------------------
BOOL
PmkCacheOps
// Query the WiFi card PMK Cache.
// wzctool -cache cisco1
//     query cisco1 adapter parameters.
// wzctool -cache
//     find wireless card, and query for that adapter.
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
	DWORD Flags = 0;
	DWORD FlagsToSet = 0;
    
    LPCWSTR pAdapter = NULL;
    if ((GetOption(argc, argv, L"cache", &pAdapter) < 0)
     || (*pAdapter == L'-'))
    {
        GetFirstWirelessNetworkCard();
        if (!*g_WirelessCardName)    // wifi card not found
            return FALSE;
        pAdapter = g_WirelessCardName;
    }

    if (WasOption(argc, argv, L"?")>0)
    {
        LPCWSTR szHelp[] = {
            L"-cache options:",
            L"",
            L"-enable [on|off]",
            L"      Enables or disables all PMK caching",
            L"",
            L"-opportunistic [on|off]",
            L"      Enables or disables opportunistic PMK caching",
            L"",
            L"-preauth [on|off]",
            L"      Enables or disables pre-authentication",
            L"",
            L"-flush",
            L"      Flushes the adapter's current PMK cache",
            L"",
            NULL };

        PrintfMsgList(szHelp);
        PrintfMsg(L"current cache values:");
    }
    else
    {        
    	LPCWSTR pArgValue;
    	if ((GetOption(argc, argv, L"enable", &pArgValue) > 0))
    	{
    		FlagsToSet |= INTF_ENTRY_PMKCACHE_FLAG_ENABLE;
    		if (0 == wcsicmp(pArgValue, L"on"))
    			Flags |= INTF_ENTRY_PMKCACHE_FLAG_ENABLE;
            PrintfMsg(L"turning %hs PMK caching",
                    (Flags & INTF_ENTRY_PMKCACHE_FLAG_ENABLE)? "ON" : "OFF");
    	}
    	if ((GetOption(argc, argv, L"opportunistic", &pArgValue) > 0))
    	{
    		FlagsToSet |= INTF_ENTRY_PMKCACHE_FLAG_ENABLE_OPPORTUNISTIC;
    		if (0 == wcsicmp(pArgValue, L"on"))
    			Flags |= INTF_ENTRY_PMKCACHE_FLAG_ENABLE_OPPORTUNISTIC;
            PrintfMsg(L"turning %hs opportunistic PMK caching",
                    (Flags & INTF_ENTRY_PMKCACHE_FLAG_ENABLE_OPPORTUNISTIC)? "ON" : "OFF");
        }
    	if ((GetOption(argc, argv, L"preauth", &pArgValue) > 0))
    	{
    		FlagsToSet |= INTF_ENTRY_PMKCACHE_FLAG_ENABLE_PREAUTH;
    		if (0 == wcsicmp(pArgValue, L"on"))
    			Flags |= INTF_ENTRY_PMKCACHE_FLAG_ENABLE_PREAUTH;
            PrintfMsg(L"turning %hs preauthentication",
                    (Flags & INTF_ENTRY_PMKCACHE_FLAG_ENABLE_PREAUTH)? "ON" : "OFF");
        }
    	if ((WasOption(argc, argv, L"flush") > 0))
    	{
    		FlagsToSet |= INTF_ENTRY_PMKCACHE_FLAG_FLUSH;
    		Flags |= INTF_ENTRY_PMKCACHE_FLAG_FLUSH;
            PrintfMsg(L"flushing PMK cache information");
        }
    }
    
	if (FlagsToSet)
	{
		return SetAdapterPMKCache(pAdapter, Flags, FlagsToSet);
	}
	else
	{
        INTF_ENTRY_EX Intf;
        DWORD dwOutFlags;
        if (!QueryWzcIntf(pAdapter, &Intf, INTF_PMKCACHE, &dwOutFlags))
            return FALSE;
        
		BOOL bRtn = PrintPmkCache(Intf);
        WZCDeleteIntfObjEx(&Intf);
        return bRtn;
	}
} // PmkCacheOps()

#endif /* UNDER_CE */

// ========================================================================== //
// Packet-Latency:
// ========================================================================== //

#ifdef UNDER_CE

// -----------------------------------------------------------------------------
BOOL
LatencyOps
//  Set/Query Latency Sensitive Mode value
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    DWORD result;
    BOOL  bEnabled;
    ULONG PacketLatencyMs;

    LPCWSTR pArgValue;
    LPWSTR  pAdapter = NULL;
    if ((GetOption(argc, argv, L"adapter", &pArgValue) > 0)
     && (*pArgValue != L'-'))
    {
        StringCchCopyW(g_WirelessCardName, COUNTOF(g_WirelessCardName), pArgValue);
        pAdapter = g_WirelessCardName;
    }

    if (WasOption(argc, argv, L"?")>0)
    {
        LPCWSTR szHelp[] = {
            L"-latency options:",
            L"interval  Milliseconds delay expected between each packet",
            L"      Adapter(s) will expect packets to arrive at this rate.",
            L"      Zero (default) disables packet-latency mode.",
            L"",
            L"-adapter  Name of WiFi adapter to configure",
            L"      By default sets parameters for all adapters.",
            L"",
            L"usage:",
            L"wzctool -latency",
            L"      displays current latency settings",
            L"wzctool -latency 0",
            L"      disables latency mode for all adapters",
            L"wzctool -latency 20 -adapter PCI/AR5211",
            L"      sets PCI/AR5211 latency to 20 ms per packet",
            L"",
            NULL };

        PrintfMsgList(szHelp);
        PrintfMsg(L"current latency values:");
    }
    else
    {        
    	if ((GetOption(argc, argv, L"latency", &pArgValue) > 0)
         && (pArgValue && *pArgValue != L'-'))
        {
            PacketLatencyMs = _wtol(pArgValue);
            result = WZCSetLatencySensitiveMode(NULL, pAdapter,
                                                PacketLatencyMs > 0 ? TRUE : FALSE,
                                                PacketLatencyMs);
	        if (NO_ERROR != result)
		    {
                if (ERROR_NOT_SUPPORTED == result)
            	    PrintfMsg(L"latency mode not supported");
                else
    		        PrintfMsg(L"WZCSetLatencySensitiveMode returned error=%u", result);
                return FALSE;
		    }
        }
    }
    
    result = WZCQueryLatencySensitiveMode(NULL, pAdapter, &bEnabled, &PacketLatencyMs);
    if (NO_ERROR != result)
    {
        if (ERROR_NOT_SUPPORTED == result)
    	    PrintfMsg(L"latency mode not supported");
        else
    	    PrintfMsg(L"WZCQueryLatencySensitiveMode returned error=%u", result);
        return FALSE;
	}

    PrintfMsg(L"Latency Sensitive Mode for adapter %s: Enable=%hs Latency=%u ms",
             pAdapter ? pAdapter : L"[any]",
             bEnabled ? "ON" : "OFF",
             PacketLatencyMs);

    return TRUE;
} // LatencyOps()

#endif /* UNDER_CE */

// ========================================================================== //
// WZC Utility Operations:
// ========================================================================== //

// -----------------------------------------------------------------------------
BOOL
ResetPreferredList
// reset the [Preferred Networks], so wireless will be disconnected
// wzctool -reset cisco1
//      reset CISCO1 adapter.
// wzctool -reset
//      reset the first wireless adapter found in the system
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    LPCWSTR pAdapter = NULL;
    if ((GetOption(argc, argv, L"reset", &pAdapter) < 0)
     || (*pAdapter == L'-'))
    {
        GetFirstWirelessNetworkCard();
        if (!*g_WirelessCardName)    // wifi card not found
            return FALSE;
        pAdapter = g_WirelessCardName;
    }

    DWORD dwInFlags = 0;
    INTF_ENTRY_EX Intf;
    memset(&Intf, 0x00, sizeof(INTF_ENTRY_EX));
    Intf.wszGuid = const_cast<LPWSTR>(pAdapter);
    DWORD result = WZCSetInterfaceEx(NULL, INTF_PREFLIST, &Intf, &dwInFlags);
    if (result)
        PrintfMsg(L"WZCSetInterfaceEx() error %u dwOutFlags=0x%08X", result, dwInFlags);
    else
        PrintfMsg(L"WZC reseting [Preferred Networks]");

    return result? FALSE : TRUE;
} // ResetPreferredList()

// -----------------------------------------------------------------------------
BOOL
SetWzcParameter
// set WZC parameters.
// WZC has 4 timer parameters; tmTr, tmTp, tmTc, tmTf
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    WZC_CONTEXT wzcContext;
    DWORD dwOutFlags;
    if (!QueryWzcContext(&wzcContext, 0, &dwOutFlags))
        return FALSE;

    if (WasOption(argc, argv, L"?")>0)
    {
        LPCWSTR szHelp[] = {
            L"tmTr  Scan time out.",
            L"      WZC requests BSSID scan to the miniport driver then waits for 'tmTr'",
            L"      until wireless miniport finishes scanning.",
            L"      default = 3 sec (3000 ms)",
            L"tmTp  Association time out.",
            L"      WZC requests wifi adapter to associate to the given SSID.",
            L"      If wifi adapter does not finish association within time 'tmTp',",
            L"      WZC tries next SSID in the [Preferred Networks].",
            L"      default = 2 sec (2000 ms)",
            L"tmTc  Periodic scan when connected.",
            L"      The scanning requires channel switching on wireless card.",
            L"      in order to listen beaconing packets on all channels.",
            L"      The channel switching is not preferable when STA is in connected-state",
            L"      since wireless packets transfer is blocked during the scanning.",
            L"      This timer is set to possible maximum value INFINITE by default.",
            L"      default = INFINITE (0x70000000=1879048192)",
            L"tmTf  Periodic scan when disconnected.",
            L"      This is the interval that WZC sends scanning requests",
            L"      to find connection candidate SSIDs given by [Preferred Networks].",
            L"      default = 1 min (60000 ms)",
            L"",
            L"usage:",
            L"wzctool -set -tmtr 1000",
            L"      set 'tmTr' timer to 1000 mili-second",
            L"wzctool -set -tmtr 1000 -tmtf 2000",
            L"      set 'tmTr' = 1000 mili-second, 'tmTf' = 2000 mili-second",
            L"wzctool -set -tmtr 0",
            L"      will set back to the default value.",
            L"wzctool -set -tmtr -1",
            L"      will set to INFINITE number.",
            L"",
            NULL };

        PrintfMsgList(szHelp);
        PrintfMsg(L"current parameter values:");
    }
    else
    {
        LPCWSTR szVal = NULL;
        int iValue = 0;
        if (GetOption(argc, argv, L"tmtr", &szVal) > 0)
        {
            iValue = _wtoi(szVal);
            wzcContext.tmTr = ((iValue==0)? TMMS_DEFAULT_TR :
                              ((iValue<0) ? TMMS_INFINITE : iValue));
        }
        if (GetOption(argc, argv, L"tmtp", &szVal) > 0)
        {
            iValue = _wtoi(szVal);
            wzcContext.tmTp = ((iValue==0)? TMMS_DEFAULT_TP :
                              ((iValue<0) ? TMMS_INFINITE : iValue));
        }
        if (GetOption(argc, argv, L"tmtc", &szVal) > 0)
        {
            iValue = _wtoi(szVal);
            wzcContext.tmTc = ((iValue==0)? TMMS_DEFAULT_TC :
                              ((iValue<0) ? TMMS_INFINITE : iValue));
        }
        if (GetOption(argc, argv, L"tmtf", &szVal) > 0)
        {
            iValue = _wtoi(szVal);
            wzcContext.tmTf = ((iValue==0)? TMMS_DEFAULT_TF :
                              ((iValue<0) ? TMMS_INFINITE : iValue));
        }

        DWORD result = WZCSetContext(NULL, 0x00, &wzcContext, NULL);
        if (NO_ERROR != result)
        {
            PrintfMsg(L"WZCSetContext() error %u", result);
            return FALSE;
        }
    }

    PrintfMsg(L"tmTr = %d mili-seconds (Scan time out)",                   wzcContext.tmTr);
    PrintfMsg(L"tmTp = %d mili-seconds (Association time out)",            wzcContext.tmTp);
    PrintfMsg(L"tmTc = %d mili-seconds (Periodic scan when connected",    wzcContext.tmTc);
    PrintfMsg(L"tmTf = %d mili-seconds (Periodic scan when disconnected)", wzcContext.tmTf);

    return TRUE;
} // SetWzcParameter()

// -----------------------------------------------------------------------------
BOOL
RefreshWzc
// refresh WZC
// forces WZC to reconnect [Preferred Networks]
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    LPCWSTR pAdapter = NULL;
    if ((GetOption(argc, argv, L"refresh", &pAdapter) < 0)
     || (*pAdapter == L'-'))
    {
        GetFirstWirelessNetworkCard();
        if (!*g_WirelessCardName)    // wifi card not found
            return FALSE;
        pAdapter = g_WirelessCardName;
    }

    INTF_ENTRY_EX Intf;
    memset(&Intf, 0x00, sizeof(INTF_ENTRY_EX));
    Intf.wszGuid = const_cast<LPWSTR>(pAdapter);
    DWORD dwOutFlags;
    DWORD result = WZCRefreshInterfaceEx(
                        NULL,
                        INTF_ALL,
                        &Intf,
                        &dwOutFlags);
    if (result != NO_ERROR)
        PrintfMsg(L"WZCRefreshInterfaceEx error %u dwOutFlags=0x%08X", result, dwOutFlags);
    else
        PrintfMsg(L"WZCRefreshInterfaceEx successful");

    return result? FALSE : TRUE;
} // RefreshWzc()

// -----------------------------------------------------------------------------
BOOL
EnableDisableWzcSvc
// enable or disable WZC service
// options
// we can also provide specific adapter to apply this option.
// "-disablewzcsvc cisco1" will disable WZC service on cisco1 card.
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    BOOL bEnable = TRUE;
    LPCWSTR pAdapter = NULL;
    if (WasOption(argc, argv, L"enablewzcsvc")>0)
    {
        bEnable = TRUE;
        if ((GetOption(argc, argv, L"enablewzcsvc", &pAdapter) < 0)
         || (*pAdapter == L'-'))
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                return FALSE;
            pAdapter = g_WirelessCardName;
        }
    }
    else if (WasOption(argc, argv, L"disablewzcsvc")>0)
    {
        bEnable = FALSE;
        if ((GetOption(argc, argv, L"disablewzcsvc", &pAdapter) < 0)
         || (*pAdapter == L'-'))
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                return FALSE;
            pAdapter = g_WirelessCardName;
        }
    }
    else
    {
        PrintfMsg(L"unknown option");
        return FALSE;
    }

    DWORD dwInFlags = 0;
    INTF_ENTRY_EX Intf;
    memset(&Intf, 0x00, sizeof(INTF_ENTRY_EX));
    Intf.wszGuid = const_cast<LPWSTR>(pAdapter);
    if (bEnable)
        Intf.dwCtlFlags |= INTFCTL_ENABLED;
    else
        Intf.dwCtlFlags &= ~INTFCTL_ENABLED;

    // work item for INTFCTL_FALLBACK

    DWORD result = WZCSetInterfaceEx(NULL, INTF_ENABLED, &Intf, &dwInFlags);
    if (result)
        PrintfMsg(L"WZCSetInterfaceEx() error %u dwInFlags=0x%08X", result, dwInFlags);
    else
        PrintfMsg(bEnable ? L"enabled" : L"disabled");

    return result? FALSE : TRUE;
} // EnableDisableWzcSvc

// ========================================================================== //
// Main:
// ========================================================================== //

LPCWSTR g_szHelp[] =
{
    L"wzctool usage:",
    L"options:",
    L" -d              Sends output to debug log",
    L" -e              Enumerate wireless cards",
    L" -q <Card Name>  Query wireless card",
    L" -c <Card Name> -ssid AP-SSID -auth open -encr wep -key 1/0x1234567890",
    L"     connect to AP-SSID with given parameters",
    L"     Use -c -? for details",
    L" -set <parameter> Set WZC variables",
    L"     Use -set -? for details",
    L" -registry <Card Name> configure from registry information",
    L"     Use -registry -? for details",
#ifdef UNDER_CE
    L" -cache [<VOIP packet latency in ms>]",
    L"     Sets VoIP packet-latency mode",
    L"     Use -c -? for details",
    L" -latency [<VOIP packet latency in ms>]",
    L"     Sets VoIP packet-latency mode",
    L"     Use -c -? for details",
#endif
    L" -reset <Card Name> Reset WZC configuration data",
    L"     Wireless card will disconnect if it was connected",
    L" -refresh        Refresh entries",
    L"     Causes immediate re-scan and connection re-try",     
    L" -enablewzcsvc <Card Name> enable WZC service",
    L"     Tells WZC to take over as suplicant",
    L" -disablewzcsvc <Card Name> disable WZC service",
    L"     Tells WZC to let another supplicant take over",
    L"if no arg is given, wzctool will read and set settings in the registry.",
    L"     Use '-registry -?' for details.",
    L"if no <Card Name> is given, wzctool will use the first WiFi card it finds.",
    L" -?  shows this help message",
    NULL
};



int
wzcmain
// main function for WCHAR console application
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)

{

	RETAILMSG(1, (TEXT("[WZC] Wireless configuration tool.\r\n")));
    BOOL succeeded;
    int  minimumArgs = 1;

    if (WasOption(argc, argv, L"d") > 0)
    {
        g_bOutputToConsole = FALSE;
        minimumArgs++;
    }

    if (minimumArgs >= argc)
    {
        PrintfMsg(L"[WZC] trying to read registry and configure wireless settings");
        succeeded = ConnectFromRegistry(argc, argv);
    }
    else
    if (WasOption(argc, argv, L"e") > 0)
        succeeded = EnumWirelessNetworkCard();
    else
    if (WasOption(argc, argv, L"q") > 0)
        succeeded = PrintWzcIntf(argc, argv);
    else
    if (WasOption(argc, argv, L"c") > 0)
        succeeded = ConnectWzc(argc, argv);
    else
    if (WasOption(argc, argv, L"reset") > 0)
        succeeded = ResetPreferredList(argc, argv);
    else
    if (WasOption(argc, argv, L"set") > 0)
        succeeded = SetWzcParameter(argc, argv);
    else
    if (WasOption(argc, argv, L"refresh") > 0)
        succeeded = RefreshWzc(argc, argv);
    else
    if (WasOption(argc, argv, L"registry") > 0)
        succeeded = ConnectFromRegistry(argc, argv);
    else
    if (WasOption(argc, argv, L"enablewzcsvc") > 0)
        succeeded = EnableDisableWzcSvc(argc, argv);
    else
    if (WasOption(argc, argv, L"disablewzcsvc") > 0)
        succeeded = EnableDisableWzcSvc(argc, argv);
#ifdef UNDER_CE
    else
    if (WasOption(argc, argv, L"cache") > 0)
        succeeded = PmkCacheOps(argc, argv);
    else
    if (WasOption(argc, argv, L"latency") > 0)
        succeeded = LatencyOps(argc, argv);
#endif
    else
    {
        if (WasOption(argc, argv, L"?")    > 0
         || WasOption(argc, argv, L"help") > 0)
            succeeded = TRUE;
        else
        {
            succeeded = FALSE;
            PrintfMsg(L"unknown command");
        }
        PrintfMsgList(g_szHelp);
    }

    return succeeded? 0 : 1;
} // wmain()

// -----------------------------------------------------------------------------
int WINAPI WinMain(    HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    //LPTSTR    lpCmdLine,
					LPWSTR		lpCmdLine,
                    int       nCmdShow)
{
	int          argc;
	int          retVal;
	WCHAR**      argv;
	unsigned int i;
	int          j;
	int    pos = 0;

	// parse a few of the command line arguments
	// a space delimites an argument except when it is inside a quote
	RETAILMSG(1, (TEXT("[WZC] WinMain: %d\r\n"), wcslen(lpCmdLine)));

	argc = 1;
	for (i = 0; i < wcslen(lpCmdLine); i++)
	{
		while (lpCmdLine[i] == ' ' && i < wcslen(lpCmdLine))
		{
			i++;
		}
	
	if (lpCmdLine[i] == '\"')
	{
		i++;
		while (lpCmdLine[i] != '\"' && i < wcslen(lpCmdLine))
		{
			i++;
			pos++;
		}
		argc++;
		pos = 0;
	}
	else
	{
		while (lpCmdLine[i] != ' ' && i < wcslen(lpCmdLine))
		{
			i++;
			pos++;
		}
		argc++;
		pos = 0;
		}
	}

	argv = (WCHAR**)malloc(sizeof(WCHAR*)* (argc+1));

	argv[0] = (WCHAR*)malloc(1024);
	GetModuleFileName(0, argv[0],1024);

	for(j=1; j<argc; j++)
	{
		argv[j] = (WCHAR*)malloc(wcslen(lpCmdLine)+10);
	}
	argv[argc] = 0;

	argc = 1;
	pos = 0;
	for (i = 0; i < wcslen(lpCmdLine); i++)
	{
		while (lpCmdLine[i] == ' ' && i < wcslen(lpCmdLine))
		{
			i++;
		}
		if (lpCmdLine[i] == '\"')
		{
			i++;
			while (lpCmdLine[i] != '\"' && i < wcslen(lpCmdLine))
			{
				argv[argc][pos] = lpCmdLine[i];
				i++;
				pos++;
			}
			argv[argc][pos] = '\0';
			argc++;
			pos = 0;
		}
		else
		{
			while (lpCmdLine[i] != ' ' && i < wcslen(lpCmdLine))
			{
				argv[argc][pos] = lpCmdLine[i];
				i++;
				pos++;
			}
			argv[argc][pos] = '\0';
			argc++;
			pos = 0;
		}
	}
	argv[argc] = 0;

	// argc = wcslen(lpCmdLine);
	RETAILMSG(1, (L"[WZC] Filename(%s) - argc : %d\r\n", argv[0], argc));

	// Init Filelog
	OpenLogFile();

	// Initialize the processes and start the application.
	retVal = wzcmain(argc, (LPCWSTR *)argv);

	// Save File
	SaveLogFile(0, 1, 0);

	// Delete arguments
	for(j=0; j<argc; j++)
	{
		free(argv[j]);
	}
	free(argv);

	CloseLogFile();

	return retVal;
}

//----------------------------------------------------------------------------------------------------------
#define RAM_BUFF_SIZE		4*1024

HANDLE	hFile			 = NULL;
CHAR	*pwStrFileBuffer = NULL;
CHAR	*pwStrFileCurrentBuffer = NULL;
DWORD	FileBufferCount	 = 0;

void OpenLogFile()
{
	SYSTEMTIME	systemTime;
	GetLocalTime(&systemTime);
	
	WCHAR wszFileName[256]={};

	wsprintf(wszFileName, L"\\ProgramStore\\System\\WZCTool_log.txt");

	hFile = CreateFile(wszFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE,
							0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	
	/////////////////////////////////////////////////////////////////////////////////
	pwStrFileBuffer = new CHAR[RAM_BUFF_SIZE];
	pwStrFileCurrentBuffer = pwStrFileBuffer;
	FileBufferCount = 0;

	ZeroMemory(pwStrFileBuffer, RAM_BUFF_SIZE);

	PrintfMsg(L"\r\n======================================================================================");
	PrintfMsg(L" WZCTool : %d.%d.%d - %d:%d:%d",
				systemTime.wYear, systemTime.wMonth, systemTime.wDay,
				systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	PrintfMsg(L"======================================================================================\r\n");
}

void SaveLogFile(CHAR *pwStr, DWORD dwType, DWORD dwLen)
{
	DWORD dwWrite = 0;

	// RAM
	if(dwType == 0){
			
		// Overflow Buffer Size
		if((FileBufferCount + dwLen) > RAM_BUFF_SIZE)
		{
			if(FileBufferCount){

				// 
				SetFilePointer(hFile, 0, NULL, FILE_END);

				WriteFile(hFile, pwStrFileBuffer, FileBufferCount, &dwWrite, NULL);
			
				// Init File Buffer.
				FileBufferCount = 0;
				pwStrFileCurrentBuffer = pwStrFileBuffer;
				ZeroMemory(pwStrFileBuffer, RAM_BUFF_SIZE);
			}
		}

		// File Logs
		sprintf(pwStrFileCurrentBuffer, "%s\r\n", pwStr);
		
		pwStrFileCurrentBuffer += dwLen;

		FileBufferCount += dwLen;

	}
	// File
	else if(dwType == 1){
		
		if(FileBufferCount){
			
			SetFilePointer(hFile, 0, NULL, FILE_END);

			WriteFile(hFile, pwStrFileBuffer, FileBufferCount, &dwWrite, NULL);
			
			if(FileBufferCount == dwWrite){

				// Init File Buffer.
				FileBufferCount = 0;
				pwStrFileCurrentBuffer = pwStrFileBuffer;
				ZeroMemory(pwStrFileBuffer, RAM_BUFF_SIZE);
			}
			else {
				 RETAILMSG(1, (L"[WZC] SaveLogFile() Write - error : %d -> %d", FileBufferCount, dwWrite));
			}
		}
	}
}

void CloseLogFile()
{
	if(hFile)
		CloseHandle(hFile);

	if(pwStrFileBuffer)
		delete pwStrFileBuffer;
}	
