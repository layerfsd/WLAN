#pragma once

#include <ntddndis.h>
#include <nuiouser.h>
#include <windev.h>


// BSSID List 강제 재스캔 (갱신)
BOOL RefreshBSSIDList(LPTSTR strAdpater);

// BSSID LIST 정보 얻어오기
BOOL GetBBSIDListInfo(LPTSTR strAdapter, PNDIS_WLAN_BSSID pNdisWlanBSSID, DWORD *dwReturnedItems);

// 현재 BSSID(MAC) 얻어오기
BOOL GetBSSID(LPWSTR pAdapter, BYTE *pucMac);

// 현재 SSID(AP이름) 얻어오기
BOOL GetSSID(LPTSTR strAdapter, LPTSTR strSSID);

// 현재 SSID의 RSSI 감도 가져오기
BOOL GetRSSI(LPTSTR strAdapter, int* pnRSSI);

// NIC statistics 조회 (Bit Rate, Device State, Media State 등)
BOOL GetNICStatistics(LPTSTR strAdapter, NIC_STATISTICS* pns);


// REBIND
BOOL DoNdis_RebindAdater(LPTSTR tszAdapter);