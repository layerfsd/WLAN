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

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <windows.h>

//  Events [AG --> Network Extension]
#define NETWORK_EVENT_SET_ADD_HELD_CALL_TO_CONF         0x01
#define NETWORK_EVENT_QUERY_SUBSCRIBER_NO               0x02       
#define NETWORK_EVENT_QUERY_CALL_LIST                   0x03
#define NETWORK_EVENT_QUERY_CURRENT_OPERATOR            0x04
#define NETWORK_EVENT_QUERY_SUPPORT_3WAY_CONF           0x05



//  Notification [Network Extension --> AG]
#define NETWORK_EVENT_CALL_IN           0x00
#define NETWORK_EVENT_CALL_OUT          0x01
#define NETWORK_EVENT_CALL_CONNECT      0x02
#define NETWORK_EVENT_CALL_DISCONNECT   0x03
#define NETWORK_EVENT_CALL_REJECT       0x04
#define NETWORK_EVENT_CALL_INFO         0x05
#define NETWORK_EVENT_CALL_BUSY         0x06
#define NETWORK_EVENT_RING              0x07
#define NETWORK_EVENT_FAILED            0xff

#define NETWORK_NOTIF_CALL_IN           NETWORK_EVENT_CALL_IN
#define NETWORK_NOTIF_CALL_OUT          NETWORK_EVENT_CALL_OUT
#define NETWORK_NOTIF_CALL_CONNECT      NETWORK_EVENT_CALL_CONNECT
#define NETWORK_NOTIF_CALL_DISCONNECT   NETWORK_EVENT_CALL_DISCONNECT
#define NETWORK_NOTIF_CALL_REJECT       NETWORK_EVENT_CALL_REJECT
#define NETWORK_NOTIF_CALL_INFO         NETWORK_EVENT_CALL_INFO
#define NETWORK_NOTIF_CALL_BUSY         NETWORK_EVENT_CALL_BUSY
#define NETWORK_NOTIF_RING              NETWORK_EVENT_RING
#define NETWORK_NOTIF_CALL_HELD         0x08                                //  Indicated when a call is held.


#define NETWORK_NOTIF_FAILED            0xff

typedef enum _NETWORK_CALL_TYPE {
    CALL_TYPE_ANSWER = 0x01,
    CALL_TYPE_DROP = 0x02,
    CALL_TYPE_DIAL = 0x03,
    CALL_TYPE_HOLD = 0x04,
    CALL_TYPE_SWAP = 0x05,
    CALL_TYPE_UNHOLD = 0x06,
} NETWORK_CALL_TYPE;

typedef struct _NetworkCallFailedInfo {
	USHORT usCallType;			// Type of call (see NETWORK_CALL_TYPE enum
	DWORD dwStatus;				// Status of the call
} NetworkCallFailedInfo;

void BthAGOnNetworkEvent(DWORD dwEvent, LPVOID lpvParam, DWORD cbParam);


#endif // __NETWORK_H__

