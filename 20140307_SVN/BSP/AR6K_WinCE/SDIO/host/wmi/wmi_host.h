//------------------------------------------------------------------------------
// <copyright file="wmi_host.h" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// WMI host definitions
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef _WMI_HOST_H_
#define _WMI_HOST_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * IP QoS Field definitions according to 802.1p
 */
#define BEST_EFFORT_PRI         0
#define BACKGROUND_PRI          1
#define EXCELLENT_EFFORT_PRI    3
#define CONTROLLED_LOAD_PRI     4
#define VIDEO_PRI               5
#define VOICE_PRI               6
#define NETWORK_CONTROL_PRI     7

struct wmi_stats {
    A_UINT32    cmd_len_err;
    A_UINT32    cmd_id_err;
};

struct wmi_priority_state {
    A_UINT8     inUse;
    A_UINT8     mbox;
};

struct wmi_mbox_state {
    A_UINT8     trafficClass;
    A_INT8      priorityNum;
};

struct wmi_t {
    A_BOOL                          wmi_ready;
    A_BOOL                          wmi_numQoSStream;
    struct wmi_priority_state       wmi_priority[WMI_MAX_NUM_PRI_STREAMS];
    struct wmi_mbox_state           wmi_mboxMap[2][WMI_MBOX_COUNT];
    A_INT8                          wmi_trafficClassMap[2][WMM_NUM_AC];
    A_UINT8                         wmi_olderPriRxMbox;
    A_UINT8                         wmi_newerPriRxMbox;
    void                           *wmi_devt;
    struct wmi_stats                wmi_stats;
    struct ieee80211_node_table     wmi_scan_table;
    A_BOOL                          wmi_pstreamCmdInProgress[2][WMM_NUM_AC];
    A_BOOL                          wmi_cpstreamCmdInProgress;
    A_UINT8                         wmi_bssid[ATH_MAC_LEN];
};

#ifdef __cplusplus
}
#endif

#endif /* _WMI_HOST_H_ */
