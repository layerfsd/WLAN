/*------------------------------------------------------------------------------ */
/* <copyright file="athdrv_linux.h" company="Atheros"> */
/*    Copyright (c) 2004-2009 Atheros Corporation.  All rights reserved. */
/*  */
/* This program is free software; you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License version 2 as */
/* published by the Free Software Foundation; */
/* */
/* Software distributed under the License is distributed on an "AS */
/* IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or */
/* implied. See the License for the specific language governing */
/* rights and limitations under the License. */
/* */
/* */
/*------------------------------------------------------------------------------ */
/*============================================================================== */
/* Author(s): ="Atheros" */
/*============================================================================== */

#ifndef _ATHDRV_LINUX_H
#define _ATHDRV_LINUX_H

#ifdef __cplusplus
extern "C" {
#endif

#define AR6000_DRIVER_CFG_BASE                  0x8000

/* Should driver perform wlan node caching? */
#define AR6000_DRIVER_CFG_GET_WLANNODECACHING   0x8001
/*Should we log raw WMI msgs */
#define AR6000_DRIVER_CFG_LOG_RAW_WMI_MSGS      0x8002


#define AR6000_XIOCTRL_WMI_SET_WLAN_STATE                       35
typedef enum {
    WLAN_DISABLED,
    WLAN_ENABLED
} AR6000_WLAN_STATE;



/* used by AR6000_IOCTL_WMI_GET_TARGET_STATS */
typedef struct targetStats_t {
    A_UINT64    tx_packets;
    A_UINT64    tx_bytes;
    A_UINT64    tx_unicast_pkts;
    A_UINT64    tx_unicast_bytes;
    A_UINT64    tx_multicast_pkts;
    A_UINT64    tx_multicast_bytes;
    A_UINT64    tx_broadcast_pkts;
    A_UINT64    tx_broadcast_bytes;
    A_UINT64    tx_rts_success_cnt;
    A_UINT64    tx_packet_per_ac[4];

    A_UINT64    tx_errors;
    A_UINT64    tx_failed_cnt;
    A_UINT64    tx_retry_cnt;
    A_UINT64    tx_mult_retry_cnt;
    A_UINT64    tx_rts_fail_cnt;

    A_UINT64    rx_packets;
    A_UINT64    rx_bytes;
    A_UINT64    rx_unicast_pkts;
    A_UINT64    rx_unicast_bytes;
    A_UINT64    rx_multicast_pkts;
    A_UINT64    rx_multicast_bytes;
    A_UINT64    rx_broadcast_pkts;
    A_UINT64    rx_broadcast_bytes;
    A_UINT64    rx_fragment_pkt;

    A_UINT64    rx_errors;
    A_UINT64    rx_crcerr;
    A_UINT64    rx_key_cache_miss;
    A_UINT64    rx_decrypt_err;
    A_UINT64    rx_duplicate_frames;

    A_UINT64    tkip_local_mic_failure;
    A_UINT64    tkip_counter_measures_invoked;
    A_UINT64    tkip_replays;
    A_UINT64    tkip_format_errors;
    A_UINT64    ccmp_format_errors;
    A_UINT64    ccmp_replays;

    A_UINT64    power_save_failure_cnt;

    A_UINT64    cs_bmiss_cnt;
    A_UINT64    cs_lowRssi_cnt;
    A_UINT64    cs_connect_cnt;
    A_UINT64    cs_disconnect_cnt;

    A_INT32     tx_unicast_rate;
    A_INT32     rx_unicast_rate;

    A_UINT32    lq_val;

    A_UINT32    wow_num_pkts_dropped;
    A_UINT16    wow_num_events_discarded;

    A_INT16     noise_floor_calibation;
    A_INT16     cs_rssi;
    A_INT16     cs_aveBeacon_rssi;
    A_UINT8     cs_aveBeacon_snr;
    A_UINT8     cs_lastRoam_msec;
    A_UINT8     cs_snr;

    A_UINT8     wow_num_host_pkt_wakeups;
    A_UINT8     wow_num_host_event_wakeups;

    A_UINT32   arp_received;
    A_UINT32   arp_matched;
    A_UINT32   arp_replied;
}TARGET_STATS;



/* used by the AR6000_XIOCTL_DBGLOG_CFG_MODULE */
typedef struct ar6000_dbglog_module_config_s {
    A_UINT32 valid;
    A_UINT16 mmask;
    A_UINT16 tsr;
    A_BOOL   rep;
    A_UINT16 size;
} DBGLOG_MODULE_CONFIG;

typedef struct user_rssi_thold_t {
    A_INT16     tag;
    A_INT16     rssi;
} USER_RSSI_THOLD;

typedef struct user_rssi_params_t {
    A_UINT8            weight;
    A_UINT32           pollTime;
    USER_RSSI_THOLD    tholds[12];
} USER_RSSI_PARAMS;



#ifdef __cplusplus
}
#endif
#endif
