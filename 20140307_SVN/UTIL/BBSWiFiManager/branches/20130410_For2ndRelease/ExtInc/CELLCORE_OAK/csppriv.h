//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#pragma once
#include "windows.h"
#include "connmgr.h"

//
///  Shared Provider Messages
//

#define PROVMSG_NET_CFGCHANGE           0

#define PROVMSG_RAS_GETCSDDESC          1
#define PROVMSG_RAS_GETCSDMODEM         2
#define PROVMSG_RAS_GETCSDLOCATION      3
#define PROVMSG_RAS_GETCSDPHONE         4

#define PROVMSG_RAS_GETVPNDESC          5
#define PROVMSG_RAS_GETVPNSERVER        6

#define PROVMSG_RAS_GETDESTMETANETWORK  7
#define PROVMSG_RAS_SETVOICECALLSTATUS  8

#define PROVMSG_RAS_CFGCHANGE           9

#define PROVMSG_SECURE_LEVEL_CHANGE     10
#define PROVMSG_NEW_CONNECTION_CREATED  11

#define PROVMSG_UPDATE_STATUS           12          //  Msg for connmgr.exe, used by CSP to update its connection status.

#define PROVMSG_GET_CONNECTION_DESCRIPTION      13  // Gets connection description.
#define PROVMSG_GET_CONNECTION_ACCESS_STRING    14  // Gets connection access string (APN, dial string).
#define PROVMSG_GET_CONNECTION_DEVICE_NAME      15  // Gets connection device name.
#define PROVMSG_GET_CONNECTION_DEVICE_TYPE      16  // Gets connection device type.

#define PROVMSG_UPDATE_STATSTORE_FLAGS  17          // Msg for connmgr.exe, used to update statstore connection status.

#define PROVMSG_NET_SET_MULTIHOMING_WITH_DTPT   18  // Enables/disables multihoming support with DTPT.

#define PROVMSG_GET_IS_PROCESS_USING_DTPT       19  // Used to check if a process is using DTPT.

#define PROVMSG_GET_IP_ADDRESS_USED_BY_CR       20  // Gets CR process binding address.

#define PROVMSG_NET_SET_BASE_CONNECTION_TYPE    21  // Sets base connection type of an adapter (VPN).

typedef struct _PROVMSG_UPDATE_STATSTORE_FLAGS_PARAMS
{
    DWORD Status[2];        // Status transition: Status[0] (old) --> Status[1] (new).
    DWORD ConnType[2];      // Connection type transition.
    DWORD ConnSubtype[2];   // Connection subtype transition.
} PROVMSG_UPDATE_STATSTORE_FLAGS_PARAMS;

//
///  CSPRAS Shell Status Messages

#define WM_CSPRAS_SSM_DIALING           (WM_USER + 945)
#define WM_CSPRAS_SSM_CONNECTED         (WM_USER + 946)
#define WM_CSPRAS_SSM_DISCONNECTED      (WM_USER + 947)

typedef struct tagRASConfigChangeParams {

    TCHAR    szDescription[CONNMGR_MAX_DESC];
    TCHAR    szDestId[64];

} RASCONFIG_CHANGE_PARAMS, *PRASCONFIG_CHANGE_PARAMS;

