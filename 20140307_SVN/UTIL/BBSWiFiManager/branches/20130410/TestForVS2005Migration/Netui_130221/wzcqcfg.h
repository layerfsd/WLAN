//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//==========================================================================*
//
// Filename:    wzcqcfg.h
//
// Owner:        
//
// Purpose:    Header file for quick config the wired and wireless network cards
//
//==========================================================================*
//==========================================================================*
#pragma once

#include "wzcuip.h"

// Bubble info
typedef struct _BI {
    INT             nTimer;

    WZC_WLAN_CONFIG *pConfigs;
    UINT            cConfigs;
    UINT            uActiveConfig;
    WZC_WLAN_CONFIG *pDismissed;
    UINT            cDismissed;

    // The name of wireless adapter
    LPTSTR          pszDeviceName;

    WUSI            UpdateStateInfo;
    HANDLE          hWZCMsgThread;
    HANDLE          hEventStop;
    INT             nIndexEapolParam;
    DWORD           dwScanThreadID;
} BUBBLE_INFO, *PBUBBLE_INFO;


// Sysgen device info
typedef struct tagSysDeviceInfo
{
    LPTSTR      pszDeviceName;    // Name of the device
    BOOL        fWireless;        // Whether the device represents a wireless device
    WLAN_INFO   wiWLanInfo;        // Zero config structure
    HWND        hwndSink;         // The sink window handles bubble notification message
    BUBBLE_INFO biBubbleInfo;
    DWORD       dwSaveTmTf;       // The original value rescan timeout. We need to restore it 
                                  // when device is removed

    struct tagSysDeviceInfo* next;
} SysDeviceInfo, *PSysDeviceInfo;

// Global function definition
HRESULT GetDeviceByName(LPTSTR pszDeviceName, PSysDeviceInfo* ppSysIcon);
HRESULT RemoveDeviceFromDeviceList(LPTSTR pszDeviceName);
void    FreeDeviceInfo(PSysDeviceInfo psDeviceInfo);

