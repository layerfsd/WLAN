//------------------------------------------------------------------------------
// <copyright file="wmi_api.h" company="Atheros">
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
// This file contains the definitions for the Wireless Module Interface (WMI).
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _WMI_API_H_
#define _WMI_API_H_

#include "wlan_api.h"
#include "wmi.h"
#include "htc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WMI_HIGH_PRIORITY_MBOX  ENDPOINT4
#define WMI_LOW_PRIORITY_MBOX   ENDPOINT3
#define WMI_BEST_EFFORT_MBOX    ENDPOINT2
#define WMI_CONTROL_MBOX        ENDPOINT1
#define WMI_MBOX_COUNT          4
#define WMI_MAX_NUM_PRI_STREAMS 2
#define WMI_NOT_MAPPED          (-1)
#define WMI_IMPLICIT_PSTREAM_INACTIVITY_INT 5000 /* 5 seconds */

struct wmi_t;

void *wmi_init(void *devt);
    
void wmi_qos_state_init(struct wmi_t *wmip);    
void wmi_shutdown(struct wmi_t *wmip);
A_INT8 wmi_get_mapped_qos_queue(struct wmi_t *wmip, A_UINT8 dir, A_UINT8 trafficClass);
A_STATUS wmi_dix_2_dot3(struct wmi_t *wmip, void *osbuf);
A_STATUS wmi_data_hdr_add(struct wmi_t *wmip, void *osbuf, A_UINT8 msgType);
A_STATUS wmi_dot3_2_dix(struct wmi_t *wmip, void *osbuf);
A_STATUS wmi_data_hdr_remove(struct wmi_t *wmip, void *osbuf);
A_STATUS wmi_syncpoint(struct wmi_t *wmip);
A_STATUS wmi_syncpoint_reset(struct wmi_t *wmip);
HTC_ENDPOINT_ID wmi_get_endpoint(struct wmi_t *wmip, A_UINT8 trafficClass);
A_UINT8 wmi_implicit_create_pstream(struct wmi_t *wmip, void *osbuf, A_UINT8 dir, A_UINT32 ndisUserPriority);

A_STATUS wmi_control_rx(struct wmi_t *wmip, void *osbuf);
void wmi_iterate_nodes(struct wmi_t *wmip, wlan_node_iter_func *f, void *arg);
void wmi_free_allnodes(struct wmi_t *wmip);
bss_t *wmi_find_node(struct wmi_t *wmip, const A_UINT8 *macaddr);

typedef enum {
    NO_SYNC_WMIFLAG = 0,
    SYNC_BEFORE_WMIFLAG,            /* transmit all queued data before cmd */
    SYNC_AFTER_WMIFLAG,             /* any new data waits until cmd execs */
    SYNC_BOTH_WMIFLAG,
    END_WMIFLAG                     /* end marker */
} WMI_SYNC_FLAG;

A_STATUS wmi_cmd_send(struct wmi_t *wmip, void *osbuf, WMI_COMMAND_ID cmdId,
                      WMI_SYNC_FLAG flag);
A_STATUS wmi_connect_cmd(struct wmi_t *wmip,
                         NETWORK_TYPE netType,
                         DOT11_AUTH_MODE dot11AuthMode,
                         AUTH_MODE authMode,
                         CRYPTO_TYPE pairwiseCrypto,
                         A_UINT8 pairCryptoLen,
                         CRYPTO_TYPE groupCrypto,
                         A_UINT8 groupCryptoLen,
                         int ssidLength,
                         A_UCHAR *ssid,
                         A_UINT8 *bssid,
                         A_UINT16 channel);
A_STATUS wmi_reconnect_cmd(struct wmi_t *wmip,
                           A_UINT8 *bssid,
                           A_UINT16 channel);
A_STATUS wmi_disconnect_cmd(struct wmi_t *wmip);
A_STATUS wmi_getrev_cmd(struct wmi_t *wmip);
A_STATUS wmi_startscan_cmd(struct wmi_t *wmip, WMI_SCAN_TYPE scanType);
A_STATUS wmi_scanparams_cmd(struct wmi_t *wmip, A_UINT16 fg_start_sec,
                            A_UINT16 fg_end_sec, A_UINT16 bg_sec,
                            A_UINT16 act_chdw_msec, A_UINT16 pas_chdw_msec,
                            A_UINT8 shScanRatio);
A_STATUS wmi_bssfilter_cmd(struct wmi_t *wmip, A_UINT8 filter);
A_STATUS wmi_probedSsid_cmd(struct wmi_t *wmip, A_UINT8 index, A_UINT8 flag,
                            A_UINT8 ssidLength, A_UCHAR *ssid);
A_STATUS wmi_listeninterval_cmd(struct wmi_t *wmip, A_UINT16 listenInterval, A_UINT16 numBeacons);
A_STATUS wmi_bmisstime_cmd(struct wmi_t *wmip, A_UINT16 bmissTime, A_UINT16 numBeacons);
A_STATUS wmi_associnfo_cmd(struct wmi_t *wmip, A_UINT8 ieType,
                           A_UINT8 ieLen, A_UINT8 *ieInfo);
A_STATUS wmi_powermode_cmd(struct wmi_t *wmip, A_UINT8 powerMode);
A_STATUS wmi_ibsspmcaps_cmd(struct wmi_t *wmip, A_UINT8 pmEnable, A_UINT8 ttl, A_UINT16 atim_windows, A_UINT16 timeout_value);
A_STATUS wmi_pmparams_cmd(struct wmi_t *wmip, A_UINT16 idlePeriod,
                           A_UINT16 psPollNum, A_UINT16 dtimPolicy);
A_STATUS wmi_disctimeout_cmd(struct wmi_t *wmip, A_UINT8 timeout);
A_STATUS wmi_sync_cmd(struct wmi_t *wmip, A_UINT8 syncNumber);
A_STATUS wmi_create_pstream_cmd(struct wmi_t *wmip, WMI_CREATE_PSTREAM_CMD *params);
A_STATUS wmi_delete_pstream_cmd(struct wmi_t *wmip, A_UINT8 txQueueNumber, 
                                A_UINT8 rxQueueNumber, A_UINT8 trafficDirection);
A_STATUS wmi_set_bitrate_cmd(struct wmi_t *wmip, A_INT32 rate);
A_STATUS wmi_get_bitrate_cmd(struct wmi_t *wmip);
A_STATUS wmi_get_regDomain_cmd(struct wmi_t *wmip);
A_STATUS wmi_get_channelList_cmd(struct wmi_t *wmip);
A_STATUS wmi_set_channelParams_cmd(struct wmi_t *wmip, WMI_PHY_MODE mode,
                                   A_INT8 numChan, A_UINT16 *channelList);
A_STATUS wmi_set_link_threshold_params(struct wmi_t *wmip, 
                                A_UINT8 highThreshold_upperVal,
                                A_UINT8 highThreshold_lowerVal,
                                A_UINT8 lowThreshold_upperVal,
                                A_UINT8 lowThreshold_lowerVal,
                                A_UINT32 pollTime);
A_STATUS wmi_set_error_report_bitmask(struct wmi_t *wmip, A_UINT32 bitmask);

A_STATUS wmi_get_stats_cmd(struct wmi_t *wmip);
A_STATUS wmi_addKey_cmd(struct wmi_t *wmip, A_UINT8 keyIndex, 
                        CRYPTO_TYPE keyType, A_UINT8 keyUsage, 
                        A_UINT8 keyLength,
                        A_UINT8 *keyRSC, A_UINT8 *keyMaterial);
A_STATUS wmi_deleteKey_cmd(struct wmi_t *wmip, A_UINT8 keyIndex);
A_STATUS wmi_set_txPwr_cmd(struct wmi_t *wmip, A_UINT8 dbM);
A_STATUS wmi_get_txPwr_cmd(struct wmi_t *wmip);
A_STATUS wmi_addBadAp_cmd(struct wmi_t *wmip, A_UINT8 apIndex, A_UINT8 *bssid);
A_STATUS wmi_deleteBadAp_cmd(struct wmi_t *wmip, A_UINT8 apIndex);
A_STATUS wmi_set_tkip_countermeasures_cmd(struct wmi_t *wmip, A_BOOL en);
A_STATUS wmi_setPmkid_cmd(struct wmi_t *wmip, A_UINT8 *bssid, A_UINT8 *pmkId,
                          A_BOOL set);
A_STATUS wmi_set_access_params_cmd(struct wmi_t *wmip, A_UINT16 txop,
                                   A_UINT8 eCWmin, A_UINT8 eCWmax,
                                   A_UINT8 aifsn);
A_STATUS wmi_set_retry_limits_cmd(struct wmi_t *wmip, A_UINT8 frameType,
                                  A_UINT8 trafficClass, A_UINT8 maxRetries);

void wmi_get_current_bssid(struct wmi_t *wmip, A_UINT8 *bssid);

A_STATUS wmi_set_opt_mode_cmd(struct wmi_t *wmip, A_UINT8 optMode);
A_STATUS wmi_opt_tx_frame_cmd(struct wmi_t *wmip,
                              A_UINT8 frmType,
                              A_UINT8 *dstMacAddr,
                              A_UINT8 *bssid,
                              A_UINT16 optIEDataLen,
                              A_UINT8 *optIEData);

A_STATUS wmi_set_adhoc_bconIntvl_cmd(struct wmi_t *wmip, A_UINT16 intvl);
A_STATUS wmi_set_powersave_timers_cmd(struct wmi_t *wmip, A_UINT16 psPollTimeout, A_UINT16 eospTimeout );
A_STATUS wmi_add_krk_cmd(struct wmi_t *wmip, A_UINT8 *krk);
A_STATUS wmi_delete_krk_cmd(struct wmi_t *wmip);
A_STATUS wmi_set_voice_pkt_size_cmd(struct wmi_t *wmip, A_UINT16 voicePktSize);
A_STATUS wmi_set_max_sp_len_cmd(struct wmi_t *wmip, A_UINT8 maxSPLen);
A_STATUS wmi_set_roam_ctrl_cmd(struct wmi_t *wmip, A_UINT8 roamCtrl, A_UINT8 *bssid, A_UINT8 roamMode, A_UINT8 numBss, WMI_BSS_BIAS_INFO *info );
A_STATUS wmi_get_roam_tbl_cmd(struct wmi_t *wmip);
A_STATUS wmi_get_roam_data_cmd(struct wmi_t *wmip, A_UINT8 roamDataType);

        
#ifdef __cplusplus
}
#endif

#endif /* _WMI_API_H_ */
