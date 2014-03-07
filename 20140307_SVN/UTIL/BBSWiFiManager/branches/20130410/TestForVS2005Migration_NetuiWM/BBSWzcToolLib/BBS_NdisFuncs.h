#pragma once

#include <ntddndis.h>
#include <nuiouser.h>
#include <windev.h>


// BSSID List 강제 재스캔 (갱신)
BOOL RefreshBSSIDList(LPTSTR strAdpater);

// BSSID LIST 정보 얻어오기
BOOL GetBBSIDListInfo(LPTSTR strAdapter, PNDIS_WLAN_BSSID pNdisWlanBSSID, DWORD *dwReturnedItems);

// 현재 BSSID(MAC) 얻어오기
BOOL GetBSSIDInfo(LPWSTR pAdapter, BYTE *pucMac);