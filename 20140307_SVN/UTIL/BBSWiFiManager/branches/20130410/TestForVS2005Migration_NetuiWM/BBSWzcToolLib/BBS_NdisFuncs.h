#pragma once

#include <ntddndis.h>
#include <nuiouser.h>
#include <windev.h>


// BSSID List ���� �罺ĵ (����)
BOOL RefreshBSSIDList(LPTSTR strAdpater);

// BSSID LIST ���� ������
BOOL GetBBSIDListInfo(LPTSTR strAdapter, PNDIS_WLAN_BSSID pNdisWlanBSSID, DWORD *dwReturnedItems);

// ���� BSSID(MAC) ������
BOOL GetBSSIDInfo(LPWSTR pAdapter, BYTE *pucMac);