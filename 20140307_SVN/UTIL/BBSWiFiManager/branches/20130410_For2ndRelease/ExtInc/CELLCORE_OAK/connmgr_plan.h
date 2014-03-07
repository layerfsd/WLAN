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
/*
	Copyright (c) Microsoft Corporation. All rights reserved.
*/

#pragma once

#include "connmgr.h"

// Proxy CSP config/settings
DEFINE_GUID(IID_ConnPln_IPlannerExtension, 0xcf22d188, 0xa9bf, 0x40fa, 0x8e, 0x1d, 0x1d, 0xa7, 0xe4, 0x63, 0x1d, 0x35);

#define CM_PLANNER_RELOADSETTINGS 0
#define CM_PLANNER_CANCELCALLS    1
#define CM_PLANNER_SETENABLEMASK  2
#define CM_PLANNER_QUERYCALLS     3
#define CM_PLANNER_GETCONNECTIONMEANS 4
#define CM_PLANNER_ALWAYSON_CONFIG 5
#define CM_PLANNER_IPADDR_CHANGE   6
#define CM_PLANNER_GPRSREG_STATUS  7
#define CM_PLANNER_PHONECAPS_CHANGE 8
#define CM_PLANNER_SECURE_LEVEL_CHANGE 9
#define CM_PLANNER_PHONERESOURCES_CHANGE 10
#define CM_PLANNER_SET_CR_IDLE    11
#define CM_PLANNER_SET_RESTRICTED_CONNECTION_SET 12

// Priority mask for canceling data calls with CM_PLANNER_CANCELCALLS (CMPCC)
#define CMPCC_ALL_DATA_CALLS           0xFFF9  // End-user initiated end request should end all data calls
#define CMPCC_DATA_CALLS_WITH_TIMEOUTS 0xFFF8  // Always-On calls should not be canceled, all other data calls should be canceled.

// this is a special success value which indicates that the connection is made
// on demand (e.g. network card)
#define S_CONNECTONDEMAND ((HRESULT)2)
// this is another special success value which indicates that the connection is
// blocked by voice call.
#define S_CONNECTBLOCKED  ((HRESULT)3)

typedef struct _CONNPLAN_MEANSINFO
{
    TCHAR szDescription[CONNMGR_MAX_DESC];   // description of the connection
    TCHAR szMeans[CONNMGR_MAX_DESC];         // phone #, VPN, etc.
    GUID guidEnabling;                       // GUID of the enabling network
    BOOL fBillByTime;                        // connection billed on time
} CONNPLAN_MEANSINFO;

// Quick dormant hint feature, custom check function.
typedef BOOL (*PFN_QUICK_DORMANT_HINT_CONDITIONAL)();

