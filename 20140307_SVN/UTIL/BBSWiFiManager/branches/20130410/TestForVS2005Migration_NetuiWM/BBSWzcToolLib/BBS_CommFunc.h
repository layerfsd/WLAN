#pragma once
#include <Iphlpapi.h>
#include <ntddndis.h>
#include <nuiouser.h>
#include <eap.h>
#include <wzcsapi.h>


static const TCHAR* AuthenModeStrTable[] =
{
	L"open",
	L"shared",
	L"auto",
	L"wpa",
	L"wpa-psk",
	L"wpa-none",
	L"wpa2",
	L"wpa2-psk",
};

static const TCHAR* EncryptStrTable[] =
{
  L"wep",
  L"disabled",
  L"wep-no-keys",
  L"unsupported",
  L"tkip",
  L"tkip-no-keys",
  L"aes",
  L"aes-no-keys",
};


static const TCHAR* ManageableAdapters[] =
{
	L"SDCCF10G1",
	L"SDCCF10AG1",
	L"SDCSD30AG1",
	L"SDCSD40N1",
	L"BCMSDDHD1",
	L"AR6K_SD1",
};



// Apdater 와 IP 정보를 담기 위한 구조체
typedef struct _BBS_ADAPTER_INFO {
	TCHAR	tszAdapterName[MAX_ADAPTER_NAME_LENGTH];
	TCHAR	tszIPAddr[16];
	TCHAR	tszSubNetMask[16];
	TCHAR	tszDefaultGateWay[16];
	TCHAR	tszDnsAddr[16];
	BOOL	bDHCPEnabled;
	TCHAR	tszMacAddr[20];
} BBS_ADAPTER_INFO, *PBBS_ADAPTER_INFO;


// Network 정보를 담기위한 구조체
typedef struct _BBS_CUR_NET_INFO {
	TCHAR	tszBSSID[20];
	TCHAR	tszSSID[MAX_ADAPTER_NAME_LENGTH];
	TCHAR	tszInfra[32];
	TCHAR	tszAuth[32];
	TCHAR	tszEncr[32];
	WZC_WLAN_CONFIG wzcConfig;
} BBS_CUR_NET_INFO, *PBBS_CUR_NET_INFO;



// Get Default Information (IP, GateWay, DNS, DHCP, etc..)
void	GetWlanDefaultInformation(PBBS_ADAPTER_INFO pInformation);

// Get Current Network Info (BSSID, SSID and WZC_WLAN_CONFIG Information)
BOOL GetCurrentNetworkInfo(LPTSTR tszAdpaterName, PBBS_CUR_NET_INFO pCurNetWzcConfig);

// ReScan AP List
BOOL RefreshNetworkList(LPTSTR tszAdapterName);

// Translates from 802.11 frequency (in kHz) to channel-number.
int ChannelFreq2Number(long Frequency);






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ZeroConfig
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Query's WZC context information.
BOOL QueryWzcContext(WZC_CONTEXT *pContext);

// set WZC parameters.
BOOL SetWzcParameter(WZC_CONTEXT wzcContext);

// adding to the Preferred Networks List
// Preferred Networks is a list of SSIDs in preference order.
// WZC continuously scans available SSIDs and attempts to connect to the most preferable SSID.
BOOL AddToPreferredNetworkList(IN LPCWSTR pAdapter, IN const WZC_WLAN_CONFIG &wzcConfig);

// encrypt WEP key material
// note: this is simply for the security (to protect from memory scanning)
void EncryptWepKMaterial(IN OUT WZC_WLAN_CONFIG *pwzcConfig);








/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NDIS Funcstions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL DoNdis_RebindAdater(LPTSTR tszAdapter);