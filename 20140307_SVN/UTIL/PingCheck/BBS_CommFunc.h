#pragma once
#include <Iphlpapi.h>
#include <ntddndis.h>
#include <nuiouser.h>
#include <eap.h>
#include <wzcsapi.h>


// Apdater 와 IP 정보를 담기 위한 구조체
typedef struct _BBS_ADAPTER_INFO {
	TCHAR	tszAdapterName[MAX_ADAPTER_NAME_LENGTH];
	TCHAR	tszIPAddr[16];
	TCHAR	tszSubNetMask[16];
	TCHAR	tszDefaultGateWay[16];
	TCHAR	tszDnsAddr[16];
	BOOL	bDHCPEnabled;
	TCHAR	tszHostName[MAX_HOSTNAME_LEN + 4];
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



// Get Default Information (IP, GateWay, DNS, HostName, DHCP, etc..)
void	GetWlanDefaultInformation(PBBS_ADAPTER_INFO pInformation);


// Get Current Network Info (BSSID, SSID and WZC_WLAN_CONFIG Information)
BOOL GetCurrentNetworkInfo(LPTSTR tszAdpaterName, PBBS_CUR_NET_INFO pCurNetWzcConfig);

// ReScan AP List
BOOL RefreshNetworkList(LPTSTR tszAdapterName);

// Translates from 802.11 frequency (in kHz) to channel-number.
int ChannelFreq2Number(long Frequency);







// NDIS Funcstions
BOOL DoNdis_RebindAdater(LPTSTR tszAdapter);