#pragma once

#include <ntddndis.h>
#include <nuiouser.h>
#include <windev.h>


// BSSID List ���� �罺ĵ (����)
BOOL RefreshBSSIDList(LPTSTR strAdpater);

// BSSID LIST ���� ������
BOOL GetBBSIDListInfo(LPTSTR strAdapter, PNDIS_WLAN_BSSID pNdisWlanBSSID, DWORD *dwReturnedItems);

// ���� BSSID(MAC) ������
BOOL GetBSSID(LPWSTR pAdapter, BYTE *pucMac);

// ���� SSID(AP�̸�) ������
BOOL GetSSID(LPTSTR strAdapter, LPTSTR strSSID);

// ���� SSID�� RSSI ���� ��������
BOOL GetRSSI(LPTSTR strAdapter, int* pnRSSI);

// NIC statistics ��ȸ (Bit Rate, Device State, Media State ��)
BOOL GetNICStatistics(LPTSTR strAdapter, NIC_STATISTICS* pns);


// REBIND
BOOL DoNdis_RebindAdater(LPTSTR tszAdapter);