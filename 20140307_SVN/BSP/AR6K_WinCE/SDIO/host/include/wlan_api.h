//------------------------------------------------------------------------------
// <copyright file="wlan_api.h" company="Atheros">
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
// This file contains the API for the host wlan module
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _HOST_WLAN_API_H_
#define _HOST_WLAN_API_H_

#include "athtypes.h"
#include "osapi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ieee80211_node_table;
struct ieee80211_frame;

struct ieee80211_common_ie {
    A_UINT16    ie_chan;
    A_UINT8     *ie_tstamp;
    A_UINT8     *ie_ssid;
    A_UINT8     *ie_rates;
    A_UINT8     *ie_xrates;
    A_UINT8     *ie_country;
    A_UINT8     *ie_wpa;
    A_UINT8     *ie_wpa2;
    A_UINT8     *ie_wmm;
    A_UINT8     *ie_ath;
    A_UINT16    ie_capInfo;
    A_UINT16    ie_beaconInt;
    A_UINT8     *ie_tim;
    A_UINT8     *ie_chswitch;
    A_UINT8     ie_erp;
};

typedef struct bss {
    A_UINT8                      ni_macaddr[6];
    A_INT8                       ni_rssi;
    struct bss                   *ni_list_next;
    struct bss                   *ni_list_prev;
    struct bss                   *ni_hash_next;
    struct bss                   *ni_hash_prev;
    struct ieee80211_common_ie   ni_cie;
    A_UINT8                     *ni_buf;
    struct ieee80211_node_table *ni_table;
    A_UINT32                     ni_refcnt;
    A_INT32                      ni_scangen;
    A_UINT32                     ni_tstamp;
} bss_t;

typedef void wlan_node_iter_func(void *arg, bss_t *);

bss_t *wlan_node_alloc(struct ieee80211_node_table *nt, A_INT32 wh_size);
void wlan_node_free(bss_t *ni);
void wlan_setup_node(struct ieee80211_node_table *nt, bss_t *ni,
                const A_UINT8 *macaddr);
bss_t *wlan_find_node(struct ieee80211_node_table *nt, const A_UINT8 *macaddr);
void wlan_node_reclaim(struct ieee80211_node_table *nt, bss_t *ni);
void wlan_node_ret_n_remove(struct ieee80211_node_table *nt, bss_t *ni);
void wlan_free_allnodes(struct ieee80211_node_table *nt);
void wlan_iterate_nodes(struct ieee80211_node_table *nt, wlan_node_iter_func *f,
                        void *arg);

void wlan_node_table_init(void *wmip, struct ieee80211_node_table *nt);
void wlan_node_table_reset(struct ieee80211_node_table *nt);
void wlan_node_table_cleanup(struct ieee80211_node_table *nt);

A_STATUS wlan_parse_beacon(A_UINT8 *buf, A_INT32 framelen,
                           struct ieee80211_common_ie *cie);

A_UINT16 wlan_ieee2freq(A_INT32 chan);
A_UINT32 wlan_freq2ieee(A_UINT16 freq);


#ifdef __cplusplus
}
#endif

#endif /* _HOST_WLAN_API_H_ */
