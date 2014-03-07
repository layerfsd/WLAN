//------------------------------------------------------------------------------
// <copyright file="wmi.c" company="Atheros">
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
// This module implements the hardware independent layer of the
// Wireless Module Interface (WMI) protocol.
//
// Author(s): ="Atheros"
//==============================================================================


#include <athdefs.h>
#include <athtypes.h>
#include <osapi.h>
#include "htc.h"
#include <ieee80211.h>
#include <ieee80211_node.h>
#include <wlan_api.h>
#include <wmi_api.h>
#include "dset_api.h"
#include "gpio_api.h"
#include "wmi_host.h"
#include "athdrv.h"
#include <ar6000_api.h>

static A_STATUS wmi_ready_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);

static A_STATUS wmi_connect_event_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                     int len);
static A_STATUS wmi_disconnect_event_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                        int len);
static A_STATUS wmi_tkip_micerr_event_rx(struct wmi_t *wmip, A_UINT8 *datap, 
                                        int len);
static A_STATUS wmi_bssInfo_event_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                     int len);
static A_STATUS wmi_opt_frame_event_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                       int len);
static A_STATUS wmi_pstream_timeout_event_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                     int len);
static A_STATUS wmi_sync_point(struct wmi_t *wmip);

static A_STATUS wmi_create_pstream_reply_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                          int len);
static A_STATUS wmi_delete_pstream_reply_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                          int len);
static A_STATUS wmi_bitrate_reply_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                     int len);
static A_STATUS wmi_channelList_reply_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                         int len);
static A_STATUS wmi_regDomain_event_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                       int len);
static A_STATUS wmi_txPwr_reply_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_neighborReport_event_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                             int len);
static A_STATUS wmi_dset_open_req_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                     int len);
#if CONFIG_HOST_DSET_SUPPORT
static A_STATUS wmi_dset_close_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_dset_data_req_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                     int len);
#endif /* CONFIG_HOST_DSET_SUPPORT */
static A_STATUS wmi_scanComplete_rx(struct wmi_t *wmip, A_UINT8 *datap,
                                     int len);
static A_STATUS wmi_errorEvent_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_statsEvent_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_rssiThresholdEvent_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_reportErrorEvent_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_roam_tbl_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_cac_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_road_data_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
#if CONFIG_HOST_GPIO_SUPPORT
static A_STATUS wmi_gpio_intr_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_gpio_data_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
static A_STATUS wmi_gpio_ack_rx(struct wmi_t *wmip, A_UINT8 *datap, int len);
#endif /* CONFIG_HOST_GPIO_SUPPORT */

static const A_INT32 wmi_rateTable[] = {
    1000,
    2000,
    5500,
    11000,
    6000,
    9000,
    12000,
    18000,
    24000,
    36000,
    48000,
    54000,
    0};

void *
wmi_init(void *devt)
{
    struct wmi_t *wmip;

    A_UINT8 i;
    wmip = A_MALLOC(sizeof(struct wmi_t));
    if (wmip == NULL) {
        return (NULL);
    }
    A_MEMZERO(wmip, sizeof(*wmip));

    wmip->wmi_devt = devt;
    wlan_node_table_init(wmip, &wmip->wmi_scan_table);
    for (i = UPLINK_TRAFFIC; i < BIDIR_TRAFFIC; i++) {
        wmip->wmi_pstreamCmdInProgress[i][WMM_AC_BE] = FALSE;
        wmip->wmi_pstreamCmdInProgress[i][WMM_AC_BK] = FALSE;
        wmip->wmi_pstreamCmdInProgress[i][WMM_AC_VI] = FALSE;
        wmip->wmi_pstreamCmdInProgress[i][WMM_AC_VO] = FALSE;
    }
    wmip->wmi_cpstreamCmdInProgress = FALSE;
    wmi_qos_state_init(wmip);

    return (wmip);
}

void
wmi_qos_state_init(struct wmi_t *wmip)
{
    A_UINT8 i;

    if (wmip == NULL) {
        return;
    }

    /* Initialize QoS States */
    wmip->wmi_numQoSStream = 0;

    for (i = 0; i < WMI_MAX_NUM_PRI_STREAMS; i++) {
        wmip->wmi_priority[i].inUse = 0;
        wmip->wmi_priority[i].mbox = WMI_BEST_EFFORT_MBOX;
    }

    for (i = 0; i < WMM_NUM_AC; i++) {
        wmip->wmi_trafficClassMap[UPLINK_TRAFFIC][i] = WMI_NOT_MAPPED;
        wmip->wmi_trafficClassMap[DNLINK_TRAFFIC][i] = WMI_NOT_MAPPED;
    }
    wmip->wmi_trafficClassMap[UPLINK_TRAFFIC][WMM_AC_BE] = WMI_BEST_EFFORT_MBOX;
    wmip->wmi_trafficClassMap[DNLINK_TRAFFIC][WMM_AC_BE] = WMI_BEST_EFFORT_MBOX;

    for (i = 0; i < WMI_MBOX_COUNT; i++) {
        wmip->wmi_mboxMap[UPLINK_TRAFFIC][i].trafficClass = WMM_AC_BE;
        wmip->wmi_mboxMap[UPLINK_TRAFFIC][i].priorityNum = WMI_NOT_MAPPED;
        wmip->wmi_mboxMap[DNLINK_TRAFFIC][i].trafficClass = WMM_AC_BE;
        wmip->wmi_mboxMap[DNLINK_TRAFFIC][i].priorityNum = WMI_NOT_MAPPED;
    }

    wmip->wmi_olderPriRxMbox = WMI_HIGH_PRIORITY_MBOX;
    wmip->wmi_newerPriRxMbox = WMI_LOW_PRIORITY_MBOX;
    ar6000_set_numdataendpts(wmip->wmi_devt, 1);
}

void
wmi_shutdown(struct wmi_t *wmip)
{
    if (wmip != NULL) {
        wlan_node_table_cleanup(&wmip->wmi_scan_table);
        A_FREE(wmip);
    }
}

/*
 *  performs DIX to 802.3 encapsulation for transmit packets.
 *  uses passed in buffer.  Returns buffer or NULL if failed.
 *  Assumes the entire DIX header is contigous and that there is
 *  enough room in the buffer for a 802.3 mac header and LLC+SNAP headers.
 */
A_STATUS
wmi_dix_2_dot3(struct wmi_t *wmip, void *osbuf)
{
    A_UINT8          *datap;
    A_UINT16         typeorlen;
    ATH_MAC_HDR      macHdr;
    ATH_LLC_SNAP_HDR *llcHdr;

    AR_DEBUG_ASSERT(osbuf != NULL);

    if (a_netbuf_headroom(osbuf) <
        (sizeof(ATH_LLC_SNAP_HDR) + sizeof(WMI_DATA_HDR)))
    {
        return A_NO_MEMORY;
    }

    datap = a_netbuf_to_data(osbuf);

    typeorlen = *(A_UINT16 *)(datap + ATH_MAC_LEN + ATH_MAC_LEN);

    if (!IS_ETHERTYPE(A_BE2CPU16(typeorlen))) {
        /*
         * packet is already in 802.3 format - return success
         */
        WMI_DEBUG_PRINTF(ATH_LOG_INF,"packet already 802.3\n");
        return (A_OK);
    }

    /*
     * Save mac fields and length to be inserted later
     */
    A_MEMCPY(macHdr.dstMac, datap, ATH_MAC_LEN);
    A_MEMCPY(macHdr.srcMac, datap + ATH_MAC_LEN, ATH_MAC_LEN);
    macHdr.typeOrLen = A_CPU2BE16((A_UINT16)a_netbuf_to_len(osbuf) - sizeof(ATH_MAC_HDR) +
                                  sizeof(ATH_LLC_SNAP_HDR));

    /*
     * Make room for LLC+SNAP headers
     */
    if (a_netbuf_push(osbuf, sizeof(ATH_LLC_SNAP_HDR)) != A_OK) {
        return A_NO_MEMORY;
    }

    datap = a_netbuf_to_data(osbuf);

    A_MEMCPY(datap, &macHdr, sizeof (ATH_MAC_HDR));

    llcHdr = (ATH_LLC_SNAP_HDR *)(datap + sizeof(ATH_MAC_HDR));
    llcHdr->dsap      = 0xAA;
    llcHdr->ssap      = 0xAA;
    llcHdr->cntl      = 0x03;
    llcHdr->orgCode[0] = 0x0;
    llcHdr->orgCode[1] = 0x0;
    llcHdr->orgCode[2] = 0x0;
    llcHdr->etherType = typeorlen;

    return (A_OK);
}

/*
 * Adds a WMI data header
 * Assumes there is enough room in the buffer to add header.
 */
A_STATUS
wmi_data_hdr_add(struct wmi_t *wmip, void *osbuf, A_UINT8 msgType)
{
    WMI_DATA_HDR     *dtHdr;

    AR_DEBUG_ASSERT(osbuf != NULL);

    if (a_netbuf_push(osbuf, sizeof(WMI_DATA_HDR)) != A_OK) {
        return A_NO_MEMORY;
    }

    dtHdr = (WMI_DATA_HDR *)a_netbuf_to_data(osbuf);
#ifndef AR6K_FIRMWARE_1_0
    dtHdr->rssi = 0;
#else
    dtHdr->reserved1 = 0;
#endif
    WMI_DATA_HDR_SET_MSG_TYPE( dtHdr, msgType );
    return (A_OK);
}

A_UINT8 wmi_implicit_create_pstream(struct wmi_t *wmip, void *osbuf, A_UINT8 dir, A_UINT32 ndisUserPriority)
{
    A_UINT8         *datap;
    A_UINT8         trafficClass;
    ATH_LLC_SNAP_HDR *llcHdr;
    A_UINT16        ipType = IP_ETHERTYPE;
    WMI_CREATE_PSTREAM_CMD cmd;

    AR_DEBUG_ASSERT(osbuf != NULL);
    AR_DEBUG_ASSERT(dir == DNLINK_TRAFFIC || dir == UPLINK_TRAFFIC);
    datap = a_netbuf_to_data(osbuf);
    llcHdr = (ATH_LLC_SNAP_HDR *)(datap + sizeof(WMI_DATA_HDR) +
                                  sizeof(ATH_MAC_HDR));

    if (llcHdr->etherType == htons(ipType)) {
        /* Extract the endpoint info from the TOS field in the IP header */
        trafficClass = ar6000_trafficclass(((A_UINT8 *)llcHdr) + sizeof(ATH_LLC_SNAP_HDR),ndisUserPriority);
    } else {
        trafficClass = ar6000_trafficclass(NULL,ndisUserPriority);
    }

    /* Implicitly determine whether a create_pstream_cmd is needed for this trafficClass */
    if (!((trafficClass == WMM_AC_BE) || (wmip->wmi_trafficClassMap[dir][trafficClass] != WMI_NOT_MAPPED))) {
        A_UINT8 reqMbox;
        /*
         * If the data pkt is tagged with a priority that doesn't have a MBOX created,
         * we must first call create_pstream_cmd
         * Originally, assume traffic goes to BE MBox,
         * until the driver receives the reply from the create_pstream cmd
         */
        wmip->wmi_trafficClassMap[dir][trafficClass] = WMI_BEST_EFFORT_MBOX;
        if (dir == DNLINK_TRAFFIC) {
            /* determine which mailbox to use */
            reqMbox = wmip->wmi_olderPriRxMbox;
        } else {
            reqMbox = WMI_BEST_EFFORT_MBOX;
        }

        cmd.trafficClass = trafficClass;
        cmd.rxQueueNum = reqMbox;
        cmd.inactivityInt = WMI_IMPLICIT_PSTREAM_INACTIVITY_INT;

        if (wmi_create_pstream_cmd(wmip, &cmd) != A_OK) {
            wmip->wmi_trafficClassMap[dir][trafficClass] = WMI_NOT_MAPPED;
            trafficClass = WMM_AC_BE;
        }

    }
    return trafficClass;
}

HTC_ENDPOINT_ID
wmi_get_endpoint(struct wmi_t *wmip, A_UINT8 trafficClass)
{
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"1 pkt of traffic class(%d) to mbox(%d)\n",
                      trafficClass, wmip->wmi_trafficClassMap[UPLINK_TRAFFIC][trafficClass]);
    return wmip->wmi_trafficClassMap[UPLINK_TRAFFIC][trafficClass];
}

/*
 *  performs 802.3 to DIX encapsulation for received packets.
 *  Assumes the entire 802.3 header is contigous.
 */
A_STATUS
wmi_dot3_2_dix(struct wmi_t *wmip, void *osbuf)
{
    A_UINT8          *datap;
    ATH_MAC_HDR      macHdr;
    ATH_LLC_SNAP_HDR *llcHdr;

    AR_DEBUG_ASSERT(osbuf != NULL);
    datap = a_netbuf_to_data(osbuf);

    A_MEMCPY(&macHdr, datap, sizeof(ATH_MAC_HDR));
    llcHdr = (ATH_LLC_SNAP_HDR *)(datap + sizeof(ATH_MAC_HDR));
    macHdr.typeOrLen = llcHdr->etherType;

    if (a_netbuf_pull(osbuf, sizeof(ATH_LLC_SNAP_HDR)) != A_OK) {
        return A_NO_MEMORY;
    }

    datap = a_netbuf_to_data(osbuf);

    A_MEMCPY(datap, &macHdr, sizeof (ATH_MAC_HDR));

    return (A_OK);
}

/*
 * Removes a WMI data header
 */
A_STATUS
wmi_data_hdr_remove(struct wmi_t *wmip, void *osbuf)
{
    AR_DEBUG_ASSERT(osbuf != NULL);

    return (a_netbuf_pull(osbuf, sizeof(WMI_DATA_HDR)));
}

void
wmi_iterate_nodes(struct wmi_t *wmip, wlan_node_iter_func *f, void *arg)
{
    wlan_iterate_nodes(&wmip->wmi_scan_table, f, arg);
}

void 
wmi_free_allnodes(struct wmi_t *wmip)
{
	wlan_free_allnodes(&wmip->wmi_scan_table);
}

bss_t *
wmi_find_node(struct wmi_t *wmip, const A_UINT8 *macaddr) 
{
	bss_t *ni=NULL;
	ni=wlan_find_node(&wmip->wmi_scan_table,macaddr);
	return ni;
}

/*
 * WMI Extended Event received from Target.
 */
A_STATUS
wmi_control_rx_xtnd(struct wmi_t *wmip, void *osbuf)
{
    WMIX_CMD_HDR *cmd;
    A_UINT16 id;
    A_UINT8 *datap;
    A_UINT32 len;
    A_STATUS status = A_OK;

    if (a_netbuf_to_len(osbuf) < sizeof(WMIX_CMD_HDR)) {
        WMI_DEBUG_PRINTF(ATH_LOG_ERR, "wmi extended event rx: bad packet\n");
        wmip->wmi_stats.cmd_len_err++;
        return A_ERROR;
    }

    cmd = (WMIX_CMD_HDR *)a_netbuf_to_data(osbuf);
    id = cmd->commandId;

    if (a_netbuf_pull(osbuf, sizeof(WMIX_CMD_HDR)) != A_OK) {
        WMI_DEBUG_PRINTF(ATH_LOG_ERR, "wmi extended event rx: bad packet\n");
        wmip->wmi_stats.cmd_len_err++;
        return A_ERROR;
    }

    datap = a_netbuf_to_data(osbuf);
    len = a_netbuf_to_len(osbuf);

    switch (id) {
    case (WMIX_DSETOPENREQ_EVENTID):
        status = wmi_dset_open_req_rx(wmip, datap, len);
        break;
#if CONFIG_HOST_DSET_SUPPORT
    case (WMIX_DSETCLOSE_EVENTID):
        status = wmi_dset_close_rx(wmip, datap, len);
        break;
    case (WMIX_DSETDATAREQ_EVENTID):
        status = wmi_dset_data_req_rx(wmip, datap, len);
        break;
#endif /* CONFIG_HOST_DSET_SUPPORT */
#if CONFIG_HOST_GPIO_SUPPORT
    case (WMIX_GPIO_INTR_EVENTID):
        wmi_gpio_intr_rx(wmip, datap, len);
        break;
    case (WMIX_GPIO_DATA_EVENTID):
        wmi_gpio_data_rx(wmip, datap, len);
        break;
    case (WMIX_GPIO_ACK_EVENTID):
        wmi_gpio_ack_rx(wmip, datap, len);
        break;
#endif /* CONFIG_HOST_GPIO_SUPPORT */
    default:
        WMI_DEBUG_PRINTF(ATH_LOG_ERR,
			             "Host received unknown extended reply/event with id 0x%x\n",
                         id);
        wmip->wmi_stats.cmd_id_err++;
        status = A_ERROR;
        break;
    }

    return status;
}

/*
 * Control Path
 */
A_STATUS
wmi_control_rx(struct wmi_t *wmip, void *osbuf)
{
    WMI_CMD_HDR *cmd;
    A_UINT16 id;
    A_UINT8 *datap;
    A_UINT32 len;
    A_STATUS status = A_OK;

    AR_DEBUG_ASSERT(osbuf != NULL);
    if (a_netbuf_to_len(osbuf) < sizeof(WMI_CMD_HDR)) {
        WMI_DEBUG_PRINTF(ATH_LOG_ERR,"wmi event rx: bad packet\n");
        wmip->wmi_stats.cmd_len_err++;
        return A_ERROR;
    }

    cmd = (WMI_CMD_HDR *)a_netbuf_to_data(osbuf);
    id = cmd->commandId;

    if (a_netbuf_pull(osbuf, sizeof(WMI_CMD_HDR)) != A_OK) {
        WMI_DEBUG_PRINTF(ATH_LOG_ERR,"wmi event rx: bad packet\n");
        wmip->wmi_stats.cmd_len_err++;
        return A_ERROR;
    }

    datap = a_netbuf_to_data(osbuf);
    len = a_netbuf_to_len(osbuf);

    switch (id) {
    case (WMI_CREATE_PSTREAM_CMDID):
        status = wmi_create_pstream_reply_rx(wmip, datap, len);
        break;
    case (WMI_DELETE_PSTREAM_CMDID):
        status = wmi_delete_pstream_reply_rx(wmip, datap, len);
        break;
    case (WMI_GET_BITRATE_CMDID):
        status = wmi_bitrate_reply_rx(wmip, datap, len);
        break;
    case (WMI_GET_CHANNEL_LIST_CMDID):
        status = wmi_channelList_reply_rx(wmip, datap, len);
        break;
    case (WMI_GET_TX_PWR_CMDID):
        status = wmi_txPwr_reply_rx(wmip, datap, len);
        break;
    case (WMI_READY_EVENTID):
        status = wmi_ready_event_rx(wmip, datap, len);
        break;
    case (WMI_CONNECT_EVENTID):
        status = wmi_connect_event_rx(wmip, datap, len);
        break;
    case (WMI_DISCONNECT_EVENTID):
        status = wmi_disconnect_event_rx(wmip, datap, len);
        break;
    case (WMI_TKIP_MICERR_EVENTID):
        status = wmi_tkip_micerr_event_rx(wmip, datap, len);
        break;
    case (WMI_BSSINFO_EVENTID):
        status = wmi_bssInfo_event_rx(wmip, datap, len);
        break;
    case (WMI_REGDOMAIN_EVENTID):
        status = wmi_regDomain_event_rx(wmip, datap, len);
        break;
    case (WMI_PSTREAM_TIMEOUT_EVENTID):
        status = wmi_pstream_timeout_event_rx(wmip, datap, len);
        break;
    case (WMI_NEIGHBOR_REPORT_EVENTID):
        status = wmi_neighborReport_event_rx(wmip, datap, len);
        break;
    case (WMI_SCAN_COMPLETE_EVENTID):
        status = wmi_scanComplete_rx(wmip, datap, len);
        break;
    case (WMI_CMDERROR_EVENTID):
        status = wmi_errorEvent_rx(wmip, datap, len);
        break;
    case (WMI_REPORT_STATISTICS_EVENTID):
        status = wmi_statsEvent_rx(wmip, datap, len);
        break;
    case (WMI_RSSI_THRESHOLD_EVENTID):
        status = wmi_rssiThresholdEvent_rx(wmip, datap, len);
        break;
    case (WMI_ERROR_REPORT_EVENTID):
        status = wmi_reportErrorEvent_rx(wmip, datap, len);
        break;
    case (WMI_OPT_RX_FRAME_EVENTID):
        status = wmi_opt_frame_event_rx(wmip, datap, len);
        break;
    case (WMI_EXTENSION_EVENTID):
        status = wmi_control_rx_xtnd(wmip, osbuf);
        break;
#ifndef AR6K_FIRMWARE_1_0
    case (WMI_REPORT_ROAM_TBL_EVENTID):
        status = wmi_roam_tbl_event_rx(wmip, datap, len);
        break;
    case (WMI_CAC_EVENTID):
        status = wmi_cac_event_rx(wmip, datap, len);
        break;
    case (WMI_REPORT_ROAM_DATA_EVENTID):
        status = wmi_road_data_event_rx(wmip, datap, len);
        break;
#endif 
   default:
        WMI_DEBUG_PRINTF(ATH_LOG_ERR,"Host received unknown reply/event with id 0x%x\n",
                         id);
        wmip->wmi_stats.cmd_id_err++;
        status = A_ERROR;
        break;
    }


    return status;
}

static A_STATUS
wmi_create_pstream_reply_rx(struct wmi_t *wmip, A_UINT8 *datap, int len) 
{
    WMI_CRE_PRIORITY_STREAM_REPLY *reply;
    A_INT8 priNum = 0, i;
    A_UINT8 dir;

    if (len < sizeof(WMI_CRE_PRIORITY_STREAM_REPLY)) {
        WMI_DEBUG_PRINTF(ATH_LOG_ERR,"wmi: create pstream reply too short (%d)!!\n", len);
        return A_EINVAL;
    }

    reply = (WMI_CRE_PRIORITY_STREAM_REPLY *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_ERR,"wmi: create pstream reply: status = %d, tx mbox = %d\n", reply->status, reply->txQueueNumber);

    dir = reply->trafficDirection;
    if (dir == BIDIR_TRAFFIC) {
        wmip->wmi_pstreamCmdInProgress[UPLINK_TRAFFIC][reply->trafficClass] = FALSE;
        wmip->wmi_pstreamCmdInProgress[DNLINK_TRAFFIC][reply->trafficClass] = FALSE;
    } else {
        wmip->wmi_pstreamCmdInProgress[dir][reply->trafficClass] = FALSE;
    }
    wmip->wmi_cpstreamCmdInProgress = FALSE;

    if (reply->status == A_FAILED_CREATE_REMOVE_PSTREAM_FIRST) {
        /* do nothing */
        return A_OK;
    }

    if (reply->status == A_SUCCEEDED) {
        if (reply->trafficDirection == DNLINK_TRAFFIC ||
            reply->trafficDirection == BIDIR_TRAFFIC) {
            wmip->wmi_olderPriRxMbox = wmip->wmi_newerPriRxMbox;
            wmip->wmi_newerPriRxMbox = reply->rxQueueNumber;
            wmip->wmi_mboxMap[DNLINK_TRAFFIC][reply->rxQueueNumber].trafficClass = reply->trafficClass;
            wmip->wmi_mboxMap[DNLINK_TRAFFIC][reply->rxQueueNumber].priorityNum = 0;
            wmip->wmi_trafficClassMap[DNLINK_TRAFFIC][reply->trafficClass] = reply->rxQueueNumber;
        }

        if (reply->trafficDirection == UPLINK_TRAFFIC ||
            reply->trafficDirection == BIDIR_TRAFFIC) {
            wmip->wmi_trafficClassMap[UPLINK_TRAFFIC][reply->trafficClass] = reply->txQueueNumber;
            wmip->wmi_numQoSStream++;
            for (i = 0; i < WMI_MAX_NUM_PRI_STREAMS; i++) {
                if (!wmip->wmi_priority[i].inUse) {
                    priNum = i;
                    wmip->wmi_priority[i].inUse = 1;
                    wmip->wmi_priority[i].mbox = reply->txQueueNumber;
                    break;
                }
            }
            wmip->wmi_mboxMap[UPLINK_TRAFFIC][reply->txQueueNumber].trafficClass = reply->trafficClass;
            wmip->wmi_mboxMap[UPLINK_TRAFFIC][reply->txQueueNumber].priorityNum = priNum;
            ar6000_set_numdataendpts(wmip->wmi_devt, wmip->wmi_numQoSStream+1);
        }
    }

    return reply->status;
}

static A_STATUS
wmi_delete_pstream_reply_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_DEL_PRIORITY_STREAM_REPLY *reply;
    A_UINT8 dir;

    if (len < sizeof(WMI_DEL_PRIORITY_STREAM_REPLY)) {
        return A_EINVAL;
    }

    reply = (WMI_DEL_PRIORITY_STREAM_REPLY *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: delete pstream reply: status = %d, rx mbox = %d\n", reply->status, reply->rxQueueNumber);

    dir = reply->trafficDirection;
    if (dir == BIDIR_TRAFFIC) {
        wmip->wmi_pstreamCmdInProgress[UPLINK_TRAFFIC][reply->trafficClass] = FALSE;
        wmip->wmi_pstreamCmdInProgress[DNLINK_TRAFFIC][reply->trafficClass] = FALSE;
    } else {
        wmip->wmi_pstreamCmdInProgress[dir][reply->trafficClass] = FALSE;
    }

    if (reply->status == A_OK) {
        /* update internal states for Rx Path */
        if (reply->trafficDirection == DNLINK_TRAFFIC || reply->trafficDirection == BIDIR_TRAFFIC) {
            A_UINT8 qNum = reply->rxQueueNumber;
            A_UINT8 class = wmip->wmi_mboxMap[DNLINK_TRAFFIC][qNum].trafficClass;
            wmip->wmi_trafficClassMap[DNLINK_TRAFFIC][class] = WMI_NOT_MAPPED;
            wmip->wmi_mboxMap[DNLINK_TRAFFIC][qNum].priorityNum = WMI_NOT_MAPPED;
            wmip->wmi_mboxMap[DNLINK_TRAFFIC][qNum].trafficClass = WMM_AC_BE;
            if (wmip->wmi_newerPriRxMbox == qNum) {
                wmip->wmi_newerPriRxMbox = wmip->wmi_olderPriRxMbox;
                wmip->wmi_olderPriRxMbox = qNum;
            }
        }
    }

    return reply->status;
}

static A_STATUS
wmi_ready_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_READY_EVENT *ev = (WMI_READY_EVENT *)datap;

    if (len < sizeof(WMI_READY_EVENT)) {
        return A_EINVAL;
    }
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: ready event\n");
    wmip->wmi_ready = TRUE;
    ar6000_ready_event(wmip->wmi_devt, ev->macaddr, ev->phyCapability);

    return A_OK;
}

static A_STATUS
wmi_connect_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_CONNECT_EVENT *ev;

    if (len < sizeof(WMI_CONNECT_EVENT)) {
        return A_EINVAL;
    }
    ev = (WMI_CONNECT_EVENT *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: connected event at freq %d ", ev->channel);
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"with bssid %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n",
                     ev->bssid[0], ev->bssid[1], ev->bssid[2],
                     ev->bssid[3], ev->bssid[4], ev->bssid[5]);

    A_MEMCPY(wmip->wmi_bssid, ev->bssid, ATH_MAC_LEN);

#ifndef AR6K_FIRMWARE_1_0
    ar6000_connect_event(wmip->wmi_devt, ev->channel, ev->bssid,
                         ev->listenInterval, ev->assocReqLen, ev->assocRespLen,
                         ev->assocInfo, ev->beaconIeLen);
#else
	    ar6000_connect_event(wmip->wmi_devt, ev->channel, ev->bssid,
                         ev->listenInterval, ev->assocReqLen, ev->assocRespLen,
                         ev->assocInfo, 0);
#endif
    return A_OK;
}

static A_STATUS
wmi_regDomain_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_REG_DOMAIN_EVENT *ev;

    if (len < sizeof(*ev)) {
        return A_EINVAL;
    }
    ev = (WMI_REG_DOMAIN_EVENT *)datap;

    ar6000_regDomain_event(wmip->wmi_devt, ev->regDomain);

    return A_OK;
}

static A_STATUS
wmi_neighborReport_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_NEIGHBOR_REPORT_EVENT *ev;
    int numAps;

    if (len < sizeof(*ev)) {
        return A_EINVAL;
    }
    ev = (WMI_NEIGHBOR_REPORT_EVENT *)datap;
    numAps = ev->numberOfAps;

    if (len < (A_INT32)(sizeof(*ev) + ((numAps - 1) * sizeof(WMI_NEIGHBOR_INFO)))) {
        return A_EINVAL;
    }

    ar6000_neighborReport_event(wmip->wmi_devt, numAps, ev->neighbor);

    return A_OK;
}

static A_STATUS
wmi_disconnect_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_DISCONNECT_EVENT *ev;

    if (len < sizeof(WMI_DISCONNECT_EVENT)) {
        return A_EINVAL;
    }
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: disconnected event\n");

    ev = (WMI_DISCONNECT_EVENT *)datap;

    A_MEMZERO(wmip->wmi_bssid, sizeof(wmip->wmi_bssid));

    ar6000_disconnect_event(wmip->wmi_devt, ev->disconnectReason, ev->bssid,
                            ev->assocRespLen, ev->assocInfo);

    return A_OK;
}

static A_STATUS
wmi_tkip_micerr_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_TKIP_MICERR_EVENT *ev;

    if (len < sizeof(*ev)) {
        return A_EINVAL;
    }
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: tkip micerr event\n");

    ev = (WMI_TKIP_MICERR_EVENT *)datap;
    ar6000_tkip_micerr_event(wmip->wmi_devt, ev->keyid, ev->ismcast);

    return A_OK;
}

static A_STATUS
wmi_bssInfo_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    bss_t *bss;
    WMI_BSS_INFO_HDR *bih;
    A_UINT8 *buf;

    if (len <= sizeof(WMI_BSS_INFO_HDR)) {
        return A_EINVAL;
    }

    bih = (WMI_BSS_INFO_HDR *)datap;
    buf = datap + sizeof(WMI_BSS_INFO_HDR);
    len -= sizeof(WMI_BSS_INFO_HDR);

    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: bssInfo event %2.2x:%2.2x\n",
                      bih->bssid[4], bih->bssid[5]);

    bss = wlan_find_node(&wmip->wmi_scan_table, bih->bssid);
    if (bss != NULL) {
        /*
         * Free up the node.  Not the most efficient process given
         * we are about to allocate a new node but it is simple and should be
         * adequate.
         */
        wlan_node_ret_n_remove(&wmip->wmi_scan_table, bss);
    }

    bss = wlan_node_alloc(&wmip->wmi_scan_table, len);
    if (bss == NULL) {
        return A_NO_MEMORY;
    }

    bss->ni_rssi        = bih->rssi;
    AR_DEBUG_ASSERT(bss->ni_buf != NULL);
    A_MEMCPY(bss->ni_buf, buf, len);

    if (wlan_parse_beacon(bss->ni_buf, len, &bss->ni_cie) != A_OK) {
        wlan_node_free(bss);
        return A_EINVAL;
    }

    /*
     * Update the frequency in ie_chan, overwriting of channel number
     * which is done in wlan_parse_beacon
     */
    bss->ni_cie.ie_chan = bih->channel;
    wlan_setup_node(&wmip->wmi_scan_table, bss, bih->bssid);

    return A_OK;
}

static A_STATUS
wmi_opt_frame_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    bss_t *bss;
    WMI_OPT_RX_INFO_HDR *bih;
    A_UINT8 *buf;

    if (len <= sizeof(WMI_OPT_RX_INFO_HDR)) {
        return A_EINVAL;
    }

    bih = (WMI_OPT_RX_INFO_HDR *)datap;
    buf = datap + sizeof(WMI_OPT_RX_INFO_HDR);
    len -= sizeof(WMI_OPT_RX_INFO_HDR);

    WMI_DEBUG_PRINTF(ATH_LOG_INF, 
                     "wmi: opt frame event %2.2x:%2.2x\n",
                      bih->bssid[4], bih->bssid[5]);

    bss = wlan_find_node(&wmip->wmi_scan_table, bih->bssid);
    if (bss != NULL) {
        /*
         * Free up the node.  Not the most efficient process given
         * we are about to allocate a new node but it is simple and should be
         * adequate.
         */
        wlan_node_ret_n_remove(&wmip->wmi_scan_table, bss);
    }

    bss = wlan_node_alloc(&wmip->wmi_scan_table, len);
    if (bss == NULL) {
        return A_NO_MEMORY;
    }
    
    bss->ni_rssi        = bih->rssi;
    bss->ni_cie.ie_chan = bih->channel;
    AR_DEBUG_ASSERT(bss->ni_buf != NULL);
    A_MEMCPY(bss->ni_buf, buf, len);
    wlan_setup_node(&wmip->wmi_scan_table, bss, bih->bssid);

    return A_OK;
}

static A_STATUS
wmi_pstream_timeout_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_PSTREAM_TIMEOUT_EVENT *ev;

    if (len < sizeof(WMI_PSTREAM_TIMEOUT_EVENT)) {
        return A_EINVAL;
    }

    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: pstream timeout event\n");

    ev = (WMI_PSTREAM_TIMEOUT_EVENT *)datap;

    wmi_delete_pstream_cmd(wmip, ev->txQueueNumber, ev->rxQueueNumber, ev->trafficDirection);

    return A_OK;
}

static A_STATUS
wmi_bitrate_reply_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_BIT_RATE_CMD *reply;
    A_INT32 rate;

    if (len < sizeof(WMI_BIT_RATE_CMD)) {
        return A_EINVAL;
    }
    reply = (WMI_BIT_RATE_CMD *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: get bit rate reply %d\n", reply->rateIndex);

    if (reply->rateIndex == RATE_AUTO) {
        rate = RATE_AUTO;
    } else {
        rate = wmi_rateTable[reply->rateIndex];
    }

    ar6000_bitrate_rx(wmip->wmi_devt, rate);

    return A_OK;
}

static A_STATUS
wmi_channelList_reply_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_CHANNEL_LIST_REPLY *reply;

    if (len < sizeof(WMI_CHANNEL_LIST_REPLY)) {
        return A_EINVAL;
    }
    reply = (WMI_CHANNEL_LIST_REPLY *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: get channel list reply\n");

    ar6000_channelList_rx(wmip->wmi_devt, reply->numChannels,
                          reply->channelList);

    return A_OK;
}

static A_STATUS
wmi_txPwr_reply_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_TX_PWR_REPLY *reply;

    if (len < sizeof(*reply)) {
        return A_EINVAL;
    }
    reply = (WMI_TX_PWR_REPLY *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: get tx pwr reply\n");

    ar6000_txPwr_rx(wmip->wmi_devt, reply->dbM);

    return A_OK;
}

static A_STATUS
wmi_dset_open_req_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMIX_DSETOPENREQ_EVENT *dsetopenreq;

    if (len < sizeof(WMIX_DSETOPENREQ_EVENT)) {
        return A_EINVAL;
    }
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi_dset_open_req_rx: DataSet Open Request event\n");

    dsetopenreq = (WMIX_DSETOPENREQ_EVENT *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi_dset_open_req_rx: dset_id=0x%x\n", dsetopenreq->dset_id);
    ar6000_dset_open_req(wmip->wmi_devt,
                        dsetopenreq->dset_id,
                        dsetopenreq->targ_dset_handle,
                        dsetopenreq->targ_reply_fn,
                        dsetopenreq->targ_reply_arg);

    return A_OK;
}

#if CONFIG_HOST_DSET_SUPPORT
static A_STATUS
wmi_dset_close_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMIX_DSETCLOSE_EVENT *dsetclose;

    if (len < sizeof(WMIX_DSETCLOSE_EVENT)) {
        return A_EINVAL;
    }
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: DataSet Close event\n");

    dsetclose = (WMIX_DSETCLOSE_EVENT *)datap;
    ar6000_dset_close(wmip->wmi_devt, dsetclose->access_cookie);

    return A_OK;
}

static A_STATUS
wmi_dset_data_req_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMIX_DSETDATAREQ_EVENT *dsetdatareq;

    if (len < sizeof(WMIX_DSETDATAREQ_EVENT)) {
        return A_EINVAL;
    }
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi: DataSet Data Request event\n");

    dsetdatareq = (WMIX_DSETDATAREQ_EVENT *)datap;
    ar6000_dset_data_req(wmip->wmi_devt,
                         dsetdatareq->access_cookie,
                         dsetdatareq->offset,
                         dsetdatareq->length,
                         dsetdatareq->targ_buf,
                         dsetdatareq->targ_reply_fn,
                         dsetdatareq->targ_reply_arg);

    return A_OK;
}
#endif /* CONFIG_HOST_DSET_SUPPORT */

static A_STATUS
wmi_scanComplete_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    ar6000_scanComplete_event(wmip->wmi_devt);

    return A_OK;
}

/*
 * Target is reporting a programming error.  This is for
 * developer aid only.  Target only checks a few common violations
 * and it is responsibility of host to do all error checking.
 * Behavior of target after wmi error event is undefined.
 * A reset is recommended.
 */
static A_STATUS
wmi_errorEvent_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_CMD_ERROR_EVENT *ev;

    ev = (WMI_CMD_ERROR_EVENT *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF, "Programming Error: cmd=%d ", ev->commandId);
    switch (ev->errorCode) {
    case (INVALID_PARAM):
        WMI_DEBUG_PRINTF(ATH_LOG_INF,"Illegal Parameter\n");
        break;
    case (ILLEGAL_STATE):
        WMI_DEBUG_PRINTF(ATH_LOG_INF,"Illegal State\n");
        break;
    case (INTERNAL_ERROR):
        WMI_DEBUG_PRINTF(ATH_LOG_INF,"Internal Error\n");
        break;
    }

    return A_OK;
}


static A_STATUS
wmi_statsEvent_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_TARGET_STATS *reply;

    if (len < sizeof(*reply)) {
        return A_EINVAL;
    }
    reply = (WMI_TARGET_STATS *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi: target stats event\n");

    ar6000_targetStats_event(wmip->wmi_devt, reply);

    return A_OK;
}



static A_STATUS
wmi_rssiThresholdEvent_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_RSSI_THRESHOLD_EVENT *reply;

    if (len < sizeof(*reply)) {
        return A_EINVAL;
    }
    reply = (WMI_RSSI_THRESHOLD_EVENT *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi: rssi threshold event\n");

    ar6000_rssiThreshold_event(wmip->wmi_devt, reply->range);

    return A_OK;
}


static A_STATUS
wmi_reportErrorEvent_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_TARGET_ERROR_REPORT_EVENT *reply;

    if (len < sizeof(*reply)) {
        return A_EINVAL;
    }
    reply = (WMI_TARGET_ERROR_REPORT_EVENT *)datap;
    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi: report error event\n");

    ar6000_reportError_event(wmip->wmi_devt, reply->errorVal);

    return A_OK;
}

#ifndef AR6K_FIRMWARE_1_0

static A_STATUS
wmi_roam_tbl_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    if (len < sizeof(WMI_TARGET_ROAM_TBL)) {
        return A_EINVAL;
    }

    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi: report roam table event\n");
    ar6000_roam_tbl_event(wmip->wmi_devt, (WMI_TARGET_ROAM_TBL *)datap);
    return A_OK;
}

static A_STATUS
wmi_cac_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_CAC_EVENT *reply;

    if (len < sizeof(WMI_CAC_EVENT)) {
        return A_EINVAL;
    }

    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi: report cac event \n");
    reply = (WMI_CAC_EVENT *)datap;
    ar6000_cac_event(wmip->wmi_devt, reply->ac,
                reply->cac_indication, reply->statusCode,
                reply->tspecSuggestion);

    return A_OK;
}

static A_STATUS
wmi_road_data_event_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    if (len < sizeof(WMI_TARGET_ROAM_DATA)) {
        return A_EINVAL;
    }

    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi: report roam data event \n");
    ar6000_roam_data_event(wmip->wmi_devt, (WMI_TARGET_ROAM_DATA *)datap);
    return A_OK;
}

#endif

#if CONFIG_HOST_GPIO_SUPPORT
static A_STATUS
wmi_gpio_intr_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMIX_GPIO_INTR_EVENT *gpio_intr = (WMIX_GPIO_INTR_EVENT *)datap;

    WMI_DEBUG_PRINTF(ATH_LOG_INF, 
                     "wmi GPIO interrupt received intrmask=0x%x input=0x%x.\n",
                     gpio_intr->intr_mask, gpio_intr->input_values);

    ar6000_gpio_intr_rx(gpio_intr->intr_mask, gpio_intr->input_values);

    return A_OK;
}

static A_STATUS
wmi_gpio_data_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMIX_GPIO_DATA_EVENT *gpio_data = (WMIX_GPIO_DATA_EVENT *)datap;

    WMI_DEBUG_PRINTF(ATH_LOG_INF,
                     "wmi GPIO data received reg=%d value=0x%x.\n",
                     gpio_data->reg_id, gpio_data->value);

    ar6000_gpio_data_rx(gpio_data->reg_id, gpio_data->value);

    return A_OK;
}

static A_STATUS
wmi_gpio_ack_rx(struct wmi_t *wmip, A_UINT8 *datap, int len)
{
    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi GPIO ACK received.\n");

    ar6000_gpio_ack_rx();

    return A_OK;
}
#endif /* CONFIG_HOST_GPIO_SUPPORT */

/*
 * Called to send a wmi command. Command specific data is already built
 * on osbuf and current osbuf->data points to it.
 */
A_STATUS
wmi_cmd_send(struct wmi_t *wmip, void *osbuf, WMI_COMMAND_ID cmdId,
               WMI_SYNC_FLAG syncflag)
{
#define IS_LONG_CMD(cmdId) (cmdId == WMI_OPT_TX_FRAME_CMDID)
    WMI_CMD_HDR     *cHdr;
    A_UINT8         mbox = WMI_CONTROL_MBOX;

    AR_DEBUG_ASSERT(osbuf != NULL);

    if (syncflag >= END_WMIFLAG) {
        return A_EINVAL;
    }

    if ((syncflag == SYNC_BEFORE_WMIFLAG) || (syncflag == SYNC_BOTH_WMIFLAG)) {
        /*
         * We want to make sure all data currently queued is transmitted before
         * the cmd execution.  Establish a new sync point.
         */
        wmi_sync_point(wmip);
    }

    if (a_netbuf_push(osbuf, sizeof(WMI_CMD_HDR)) != A_OK) {
        return A_NO_MEMORY;
    }

    cHdr = (WMI_CMD_HDR *)a_netbuf_to_data(osbuf);
    cHdr->commandId = cmdId;

    /*
     * Send cmd, some via control pipe, others via data pipe
     */
    if (IS_LONG_CMD(cmdId)) {
        wmi_data_hdr_add(wmip, osbuf, CNTL_MSGTYPE); 
        mbox = WMI_BEST_EFFORT_MBOX;
    }
    ar6000_control_tx(wmip->wmi_devt, osbuf, mbox);

    if ((syncflag == SYNC_AFTER_WMIFLAG) || (syncflag == SYNC_BOTH_WMIFLAG)) {
        /*
         * We want to make sure all new data queued waits for the command to
         * execute. Establish a new sync point.
         */
        wmi_sync_point(wmip);
    }

    return (A_OK);
#undef IS_LONG_CMD
}

A_STATUS
wmi_cmd_send_xtnd(struct wmi_t *wmip, void *osbuf, WMI_COMMAND_ID cmdId,
                  WMI_SYNC_FLAG syncflag)
{
    WMIX_CMD_HDR     *cHdr;

    if (a_netbuf_push(osbuf, sizeof(WMIX_CMD_HDR)) != A_OK) {
        return A_NO_MEMORY;
    }

    cHdr = (WMIX_CMD_HDR *)a_netbuf_to_data(osbuf);
    cHdr->commandId = cmdId;

    return wmi_cmd_send(wmip, osbuf, WMI_EXTENSION_CMDID, syncflag);
}

A_STATUS
wmi_connect_cmd(struct wmi_t *wmip, NETWORK_TYPE netType, 
                DOT11_AUTH_MODE dot11AuthMode, 
                AUTH_MODE authMode,
                CRYPTO_TYPE pairwiseCrypto, A_UINT8 pairCryptoLen,
                CRYPTO_TYPE groupCrypto, A_UINT8 groupCryptoLen,
                int ssidLength, A_UCHAR *ssid,
                A_UINT8 *bssid, A_UINT16 channel)
{
    void *osbuf;
    WMI_CONNECT_CMD *cc;

    if ((pairwiseCrypto == NONE_CRYPT) && (groupCrypto != NONE_CRYPT)) {
        return A_EINVAL;
    }
    if ((pairwiseCrypto != NONE_CRYPT) && (groupCrypto == NONE_CRYPT)) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(WMI_CONNECT_CMD));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(WMI_CONNECT_CMD));

    cc = (WMI_CONNECT_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cc, sizeof(*cc));

    if (ssidLength <= 0) {
        ssidLength = 0;
    } else {
        if (ssidLength > sizeof(cc->ssid)) {
            ssidLength = sizeof(cc->ssid);
        }
        A_MEMCPY(cc->ssid, ssid, ssidLength);
    }
    cc->ssidLength          = ssidLength;
    cc->networkType         = netType;
    cc->dot11AuthMode       = dot11AuthMode;
    cc->authMode            = authMode;
    cc->pairwiseCryptoType  = pairwiseCrypto;
    cc->groupCryptoType     = groupCrypto;
#ifndef AR6K_FIRMWARE_1_0
    cc->pairwiseCryptoLen   = pairCryptoLen;
    cc->groupCryptoLen      = groupCryptoLen;
#endif
    cc->channel             = channel;

    if (bssid != NULL) {
        A_MEMCPY(cc->bssid, bssid, ATH_MAC_LEN);
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_CONNECT_CMDID, NO_SYNC_WMIFLAG));
}


A_STATUS
wmi_reconnect_cmd(struct wmi_t *wmip, A_UINT8 *bssid, A_UINT16 channel)
{
    void *osbuf;
    WMI_RECONNECT_CMD *cc;

    osbuf = a_netbuf_alloc(sizeof(WMI_RECONNECT_CMD));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(WMI_RECONNECT_CMD));

    cc = (WMI_RECONNECT_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cc, sizeof(*cc));

    cc->channel = channel;

    if (bssid != NULL) {
        A_MEMCPY(cc->bssid, bssid, ATH_MAC_LEN);
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_RECONNECT_CMDID, NO_SYNC_WMIFLAG));
}


A_STATUS
wmi_disconnect_cmd(struct wmi_t *wmip)
{
    void *osbuf;
    A_STATUS status;

    osbuf = a_netbuf_alloc(0);      /* no payload */
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    status = (wmi_cmd_send(wmip, osbuf, WMI_DISCONNECT_CMDID,
                         SYNC_BEFORE_WMIFLAG));
    wmi_qos_state_init(wmip);
    return status;
}

A_STATUS
wmi_startscan_cmd(struct wmi_t *wmip, WMI_SCAN_TYPE scanType)
{
    void *osbuf;
    WMI_START_SCAN_CMD *sc;

    if ((scanType != WMI_LONG_SCAN) && (scanType != WMI_SHORT_SCAN)) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*sc));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*sc));

    sc = (WMI_START_SCAN_CMD *)(a_netbuf_to_data(osbuf));
    sc->scanType = scanType;

    return (wmi_cmd_send(wmip, osbuf, WMI_START_SCAN_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_scanparams_cmd(struct wmi_t *wmip, A_UINT16 fg_start_sec,
                   A_UINT16 fg_end_sec, A_UINT16 bg_sec,
                   A_UINT16 act_chdw_msec, A_UINT16 pas_chdw_msec,
                   A_UINT8 shScanRatio)
{
    void *osbuf;
    WMI_SCAN_PARAMS_CMD *sc;

    osbuf = a_netbuf_alloc(sizeof(*sc));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*sc));

    sc = (WMI_SCAN_PARAMS_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(sc, sizeof(*sc));
    sc->fg_start_period  = fg_start_sec;
    sc->fg_end_period    = fg_end_sec;
    sc->bg_period        = bg_sec;
    sc->act_chdwell_time = act_chdw_msec;
    sc->pas_chdwell_time = pas_chdw_msec;
    sc->shortScanRatio   = shScanRatio;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_SCAN_PARAMS_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_bssfilter_cmd(struct wmi_t *wmip, A_UINT8 filter)
{
    void *osbuf;
    WMI_BSS_FILTER_CMD *cmd;

    if (filter >= LAST_BSS_FILTER) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_BSS_FILTER_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->bssFilter = filter;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_BSS_FILTER_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_probedSsid_cmd(struct wmi_t *wmip, A_UINT8 index, A_UINT8 flag,
                   A_UINT8 ssidLength, A_UCHAR *ssid)
{
    void *osbuf;
    WMI_PROBED_SSID_CMD *cmd;

    if (index > MAX_PROBED_SSID_INDEX) {
        return A_EINVAL;
    }
    if (ssidLength > sizeof(cmd->ssid)) {
        return A_EINVAL;
    }
    if ((flag & (DISABLE_SSID_FLAG | ANY_SSID_FLAG)) && (ssidLength > 0)) {
        return A_EINVAL;
    }
    if ((flag & SPECIFIC_SSID_FLAG) && !ssidLength) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_PROBED_SSID_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->entryIndex = index;
    cmd->flag       = flag;
    cmd->ssidLength = ssidLength;
    A_MEMCPY(cmd->ssid, ssid, ssidLength);

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_PROBED_SSID_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_listeninterval_cmd(struct wmi_t *wmip, A_UINT16 listenInterval, A_UINT16 numBeacons )
{
    void *osbuf;
    WMI_LISTEN_INT_CMD *cmd;

    if ( listenInterval ) {
       if ((listenInterval<MIN_LISTEN_INTERVAL) || 
                (listenInterval>MAX_LISTEN_INTERVAL)) {
        return A_EINVAL;
       }
    }
#ifndef AR6K_FIRMWARE_1_0
    if ( numBeacons ) {
       if ((numBeacons<MIN_LISTEN_BEACONS) || (numBeacons>MAX_LISTEN_BEACONS)) {
        return A_EINVAL;
       }
    }
#endif

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_LISTEN_INT_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->listenInterval = listenInterval;
#ifndef AR6K_FIRMWARE_1_0
    cmd->numBeacons = numBeacons;
#endif

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_LISTEN_INT_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_bmisstime_cmd(struct wmi_t *wmip, A_UINT16 bmissTime, A_UINT16 numBeacons)
{
    void *osbuf;
    WMI_BMISS_TIME_CMD *cmd;

    if ( (bmissTime<MIN_BMISS_TIME) || (bmissTime>MAX_BMISS_TIME) ) {
       return A_EINVAL;
    }
#ifndef AR6K_FIRMWARE_1_0
	if ( (numBeacons<MIN_BMISS_BEACONS) || (numBeacons>MAX_BMISS_BEACONS) ) {
       return A_EINVAL;
	}
#endif

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_BMISS_TIME_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->bmissTime= bmissTime;
#ifndef AR6K_FIRMWARE_1_0
    cmd->numBeacons= numBeacons;
#endif

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_BMISS_TIME_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS 
wmi_associnfo_cmd(struct wmi_t *wmip, A_UINT8 ieType,
                     A_UINT8 ieLen, A_UINT8 *ieInfo)
{
    void *osbuf;
    WMI_SET_ASSOC_INFO_CMD *cmd;
    A_UINT16 cmdLen;

    if (ieType > WMI_MAX_ASSOC_INFO_TYPE) {
        return A_EINVAL;
    }

    cmdLen = sizeof(*cmd) + ieLen - 1;
    osbuf = a_netbuf_alloc(cmdLen);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, cmdLen);

    cmd = (WMI_SET_ASSOC_INFO_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, cmdLen);
    cmd->ieType = ieType;
    cmd->bufferSize = ieLen;
    A_MEMCPY(cmd->assocInfo, ieInfo, ieLen);

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_ASSOC_INFO_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_powermode_cmd(struct wmi_t *wmip, A_UINT8 powerMode)
{
    void *osbuf;
    WMI_POWER_MODE_CMD *cmd;

    if ( (powerMode != REC_POWER) && (powerMode != MAX_PERF_POWER) ) {
       return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_POWER_MODE_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->powerMode = powerMode;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_POWER_MODE_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_ibsspmcaps_cmd(struct wmi_t *wmip, A_UINT8 pmEnable, A_UINT8 ttl, A_UINT16 atim_windows, A_UINT16 timeout_value)
{
    void *osbuf;
    WMI_IBSS_PM_CAPS_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_IBSS_PM_CAPS_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->power_saving = pmEnable;
    cmd->atim_windows = atim_windows;
#ifndef AR6K_FIRMWARE_1_0
    cmd->ttl = ttl;
    cmd->timeout_value = timeout_value;
#endif

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_IBSS_PM_CAPS_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_pmparams_cmd(struct wmi_t *wmip, A_UINT16 idlePeriod,
                 A_UINT16 psPollNum, A_UINT16 dtimPolicy)
{
    void *osbuf;
    WMI_POWER_PARAMS_CMD *pm;

    if ( dtimPolicy > STICK_DTIM ) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*pm));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*pm));

    pm = (WMI_POWER_PARAMS_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(pm, sizeof(*pm));
    pm->idle_period   = idlePeriod;
    pm->pspoll_number = psPollNum;
    pm->dtim_policy   = dtimPolicy;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_POWER_PARAMS_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_disctimeout_cmd(struct wmi_t *wmip, A_UINT8 timeout)
{
    void *osbuf;
    WMI_DISC_TIMEOUT_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_DISC_TIMEOUT_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->disconnectTimeout = timeout;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_DISC_TIMEOUT_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_addKey_cmd(struct wmi_t *wmip, A_UINT8 keyIndex, CRYPTO_TYPE keyType,
               A_UINT8 keyUsage, A_UINT8 keyLength, A_UINT8 *keyRSC,
               A_UINT8 *keyMaterial)
{
    void *osbuf;
    WMI_ADD_CIPHER_KEY_CMD *cmd;

    if ((keyIndex > WMI_MAX_KEY_INDEX) || (keyLength > WMI_MAX_KEY_LEN) ||
        (keyMaterial == NULL))
    {
        return A_EINVAL;
    }

    if ((WEP_CRYPT != keyType) && (NULL == keyRSC)) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_ADD_CIPHER_KEY_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->keyIndex = keyIndex;
    cmd->keyType  = keyType;
    cmd->keyUsage = keyUsage;
    cmd->keyLength = keyLength;
    A_MEMCPY(cmd->key, keyMaterial, keyLength);
    if (NULL != keyRSC) {
        A_MEMCPY(cmd->keyRSC, keyRSC, sizeof(cmd->keyRSC));
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_ADD_CIPHER_KEY_CMDID,
                         SYNC_BEFORE_WMIFLAG)); 
}

A_STATUS
wmi_deleteKey_cmd(struct wmi_t *wmip, A_UINT8 keyIndex)
{
    void *osbuf;
    WMI_DELETE_CIPHER_KEY_CMD *cmd;

    if (keyIndex > WMI_MAX_KEY_INDEX) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_DELETE_CIPHER_KEY_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->keyIndex = keyIndex;

    return (wmi_cmd_send(wmip, osbuf, WMI_DELETE_CIPHER_KEY_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_setPmkid_cmd(struct wmi_t *wmip, A_UINT8 *bssid, A_UINT8 *pmkId,
                 A_BOOL set)
{
    void *osbuf;
    WMI_SET_PMKID_CMD *cmd;

    if (bssid == NULL) {
        return A_EINVAL;
    }

    if ((set == TRUE) && (pmkId == NULL)) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_SET_PMKID_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMCPY(cmd->bssid, bssid, sizeof(cmd->bssid));
    if (set == TRUE) {
        A_MEMCPY(cmd->pmkid, pmkId, sizeof(cmd->pmkid));
        cmd->enable = PMKID_ENABLE;
    } else {
        A_MEMZERO(cmd->pmkid, sizeof(cmd->pmkid));
        cmd->enable = PMKID_DISABLE;
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_PMKID_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_set_tkip_countermeasures_cmd(struct wmi_t *wmip, A_BOOL en)
{
    void *osbuf;
    WMI_SET_TKIP_COUNTERMEASURES_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_SET_TKIP_COUNTERMEASURES_CMD *)(a_netbuf_to_data(osbuf));
    cmd->cm_en = (en == TRUE)? WMI_TKIP_CM_ENABLE : WMI_TKIP_CM_DISABLE;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_TKIP_COUNTERMEASURES_CMDID, 
            NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_dataSync_send(struct wmi_t *wmip, void *osbuf, int endPoint)
{
    WMI_DATA_HDR     *dtHdr;

    AR_DEBUG_ASSERT(endPoint != WMI_CONTROL_MBOX);
    AR_DEBUG_ASSERT(osbuf != NULL);

    if (a_netbuf_push(osbuf, sizeof(WMI_DATA_HDR)) != A_OK) {
        return A_NO_MEMORY;
    }

    dtHdr = (WMI_DATA_HDR *)a_netbuf_to_data(osbuf);
#ifndef AR6K_FIRMWARE_1_0
    dtHdr->rssi = 0;
#else
    dtHdr->reserved1 = 0;
#endif
    WMI_DATA_HDR_SET_MSG_TYPE( dtHdr, SYNC_MSGTYPE);

    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi sync pkt sent on mbox %d\n", endPoint);

    return (ar6000_control_tx(wmip->wmi_devt, osbuf, endPoint));
}

static A_STATUS
wmi_sync_point(struct wmi_t *wmip)
{
    void *cmd_osbuf, *be_osbuf, *pri_osbuf[WMI_MAX_NUM_PRI_STREAMS];
    A_UINT8 i;
    A_STATUS status;

    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi_sync_point\n");
    /*
     * We allocate all network buffers needed so we will be able to
     * send all required frames.
     */
    cmd_osbuf = a_netbuf_alloc(0);      /* no payload */
    if (cmd_osbuf == NULL) {
        return A_NO_MEMORY;
    }
    be_osbuf = a_netbuf_alloc(0);      /* no payload */
    if (be_osbuf == NULL) {
        a_netbuf_free(cmd_osbuf);
        return A_NO_MEMORY;
    }

    for (i = 0; i < wmip->wmi_numQoSStream; i++) {
        pri_osbuf[i] = a_netbuf_alloc(0);      /* no payload */
        if (pri_osbuf[i] == NULL) {
            A_UINT8 j;        
            a_netbuf_free(be_osbuf);
            a_netbuf_free(cmd_osbuf);
            /* free previously allocated bufs */
            for (j = 0; j < i; j++) {
                a_netbuf_free(pri_osbuf[j]);
            }
            return A_NO_MEMORY;
        }
    }

    /*
     * Send sync cmd followed by sync data messages on all endpoints being
     * used
     */
    status = wmi_cmd_send(wmip, cmd_osbuf, WMI_SYNCHRONIZE_CMDID,
                          NO_SYNC_WMIFLAG);

    if (status == A_OK) {
        status = wmi_dataSync_send(wmip, be_osbuf, WMI_BEST_EFFORT_MBOX);
    }

    if (status == A_OK) {
        A_UINT8 priIndex = 0;
        for (i = 0; i < wmip->wmi_numQoSStream; i++) {
            while (priIndex < WMI_MAX_NUM_PRI_STREAMS &&
                  (!wmip->wmi_priority[priIndex].inUse)) {
                priIndex++;
            }
            if (priIndex >= WMI_MAX_NUM_PRI_STREAMS) {
                break;
            }
            status = wmi_dataSync_send(wmip, pri_osbuf[i], wmip->wmi_priority[priIndex].mbox);
            if (status != A_OK) {
                break;
            }
            priIndex++;
        }
    }

    return (status);
}

A_STATUS
wmi_create_pstream_cmd(struct wmi_t *wmip, WMI_CREATE_PSTREAM_CMD *params)
{
    void *osbuf;
    WMI_CREATE_PSTREAM_CMD *cmd;

#ifndef AR6K_FIRMWARE_1_0
    /* Validate all the parameters. */
    if( !((params->userPriority < 8) &&
         (params->trafficDirection == BIDIR_TRAFFIC ||
            params->trafficDirection == UPLINK_TRAFFIC ||
            params->trafficDirection == DNLINK_TRAFFIC ) &&
         (params->userPriority <= 0x7) &&
                  (params->trafficClass != WMM_AC_BE)  &&
         (params->trafficType == TRAFFIC_TYPE_APERIODIC ||
            params->trafficType == TRAFFIC_TYPE_PERIODIC ) &&
         (params->voicePSCapability == DISABLE_FOR_THIS_AC  ||
            params->voicePSCapability == ENABLE_FOR_THIS_AC ||
            params->voicePSCapability == ENABLE_FOR_ALL_AC) &&
         (params->tsid < 15)) )
    {
        return  A_EINVAL;
    }
#endif

    if (params->trafficDirection == BIDIR_TRAFFIC) {
        if (wmip->wmi_pstreamCmdInProgress[UPLINK_TRAFFIC][params->trafficClass]
          || wmip->wmi_pstreamCmdInProgress[DNLINK_TRAFFIC][params->trafficClass]
          || wmip->wmi_cpstreamCmdInProgress) {
            WMI_DEBUG_PRINTF(ATH_LOG_ERR,"create %d too busy !\n",params->trafficClass);
            return A_EBUSY;
        }
        wmip->wmi_pstreamCmdInProgress[UPLINK_TRAFFIC][params->trafficClass] = TRUE;
        wmip->wmi_pstreamCmdInProgress[DNLINK_TRAFFIC][params->trafficClass] = TRUE;
    } else {
        if (wmip->wmi_pstreamCmdInProgress[params->trafficDirection][params->trafficClass]
         || wmip->wmi_cpstreamCmdInProgress) {
            WMI_DEBUG_PRINTF(ATH_LOG_ERR,"create %d too busy !\n",params->trafficClass);
            return A_EBUSY;
        }
        wmip->wmi_pstreamCmdInProgress[params->trafficDirection][params->trafficClass] = TRUE;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    wmip->wmi_cpstreamCmdInProgress = TRUE;
    a_netbuf_put(osbuf, sizeof(*cmd));

    WMI_DEBUG_PRINTF(ATH_LOG_INF,"Sending create_pstream_cmd: ac=%d, rxQ=%d, dir=%d\n", 
                     params->trafficClass, params->rxQueueNum, params->trafficDirection);

    cmd = (WMI_CREATE_PSTREAM_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    A_MEMCPY(cmd, params, sizeof(*cmd));

    if (params->rxQueueNum == 0xFF) {
        if(wmip->wmi_trafficClassMap[DNLINK_TRAFFIC][params->trafficClass] != WMI_NOT_MAPPED)
            cmd->rxQueueNum = wmip->wmi_trafficClassMap[DNLINK_TRAFFIC][params->trafficClass];
        else
            cmd->rxQueueNum = wmip->wmi_olderPriRxMbox;
    } else {
        cmd->rxQueueNum = params->rxQueueNum;
    }
    
    /* mike: should be SYNC_BEFORE_WMIFLAG */
    return (wmi_cmd_send(wmip, osbuf, WMI_CREATE_PSTREAM_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_delete_pstream_cmd(struct wmi_t *wmip, A_UINT8 txQueueNumber, A_UINT8 rxQueueNumber, A_UINT8 dir)
{
    void *osbuf;
    WMI_DELETE_PSTREAM_CMD *cmd;
    A_STATUS status;
    A_UINT8 class;

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    if (dir == BIDIR_TRAFFIC) {
        class = wmip->wmi_mboxMap[UPLINK_TRAFFIC][txQueueNumber].trafficClass;
        if (wmip->wmi_pstreamCmdInProgress[UPLINK_TRAFFIC][class]
         || wmip->wmi_pstreamCmdInProgress[DNLINK_TRAFFIC][class]) {
            return A_EBUSY;
        }
        wmip->wmi_pstreamCmdInProgress[UPLINK_TRAFFIC][class] = TRUE;
        wmip->wmi_pstreamCmdInProgress[DNLINK_TRAFFIC][class] = TRUE;
    } else {
        if (dir == UPLINK_TRAFFIC) {
            class = wmip->wmi_mboxMap[UPLINK_TRAFFIC][txQueueNumber].trafficClass;
        } else {
            class = wmip->wmi_mboxMap[DNLINK_TRAFFIC][rxQueueNumber].trafficClass;
        }
        if (wmip->wmi_pstreamCmdInProgress[dir][class]) {
            return A_EBUSY;
        }
        wmip->wmi_pstreamCmdInProgress[dir][class] = TRUE;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_DELETE_PSTREAM_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->txQueueNumber = txQueueNumber;
    cmd->rxQueueNumber = rxQueueNumber;
    cmd->trafficDirection = dir;
    cmd->trafficClass = class;
    WMI_DEBUG_PRINTF(ATH_LOG_INF,"Sending delete_pstream_cmd: txQ=%d, rxQ=%d, dir=%d\n", 
                     txQueueNumber, rxQueueNumber, dir);
    status = (wmi_cmd_send(wmip, osbuf, WMI_DELETE_PSTREAM_CMDID,
                         (dir == UPLINK_TRAFFIC || dir == BIDIR_TRAFFIC) ? SYNC_BEFORE_WMIFLAG : NO_SYNC_WMIFLAG));

    if (class != WMM_AC_BE) {
        /* Update internal states */
        if (dir == UPLINK_TRAFFIC || dir == BIDIR_TRAFFIC) {
            wmip->wmi_numQoSStream--;
            ar6000_set_numdataendpts(wmip->wmi_devt, wmip->wmi_numQoSStream+1);
            wmip->wmi_priority[wmip->wmi_mboxMap[UPLINK_TRAFFIC][txQueueNumber].priorityNum].inUse = 0;
            wmip->wmi_trafficClassMap[UPLINK_TRAFFIC][class] = WMI_NOT_MAPPED;
            wmip->wmi_mboxMap[UPLINK_TRAFFIC][txQueueNumber].priorityNum = WMI_NOT_MAPPED;
            wmip->wmi_mboxMap[UPLINK_TRAFFIC][txQueueNumber].trafficClass = WMM_AC_BE;
        }
    }

    return status;
}


/*
 * used to set the bit rate.  rate is in Kbps.  If rate == -1
 * then auto selection is used.
 */
A_STATUS
wmi_set_bitrate_cmd(struct wmi_t *wmip, A_INT32 rate)
{
    void *osbuf;
    WMI_BIT_RATE_CMD *cmd;
    A_INT8 i;

    if (rate != -1) {
        for (i=0;;i++) {
            if (wmi_rateTable[i] == 0) {
                return A_EINVAL;
            }
            if (wmi_rateTable[i] == rate) {
                break;
            }
        }
    } else {
        i = -1;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_BIT_RATE_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));

    cmd->rateIndex = i;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_BITRATE_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_get_bitrate_cmd(struct wmi_t *wmip)
{
    void *osbuf;

    osbuf = a_netbuf_alloc(0);      /* no payload */
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_GET_BITRATE_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_get_channelList_cmd(struct wmi_t *wmip)
{
    void *osbuf;

    osbuf = a_netbuf_alloc(0);      /* no payload */
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_GET_CHANNEL_LIST_CMDID,
                         NO_SYNC_WMIFLAG));
}

/*
 * used to generate a wmi sey channel Parameters cmd.
 * mode should always be specified and corresponds to the phy mode of the
 * wlan.
 * numChan should alway sbe specified. If zero indicates that all available
 * channels should be used.
 * channelList is an array of channel frequencies (in Mhz) which the radio
 * should limit its operation to.  It should be NULL if numChan == 0.  Size of
 * array should correspond to numChan entries.
 */
A_STATUS
wmi_set_channelParams_cmd(struct wmi_t *wmip, WMI_PHY_MODE mode, A_INT8 numChan,
                          A_UINT16 *channelList)
{
    void *osbuf;
    WMI_CHANNEL_PARAMS_CMD *cmd;
    A_INT8 size;

    if ((mode <= WMI_MIN_PHY_MODE) || (mode >= WMI_MAX_PHY_MODE)) {
       return A_EINVAL;
    }

    size = sizeof (*cmd);

    if (numChan) {
        if (numChan > WMI_MAX_CHANNELS) {
            return A_EINVAL;
        }
        size += sizeof(A_UINT16) * (numChan - 1);
    }

    osbuf = a_netbuf_alloc(size);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, size);

    cmd = (WMI_CHANNEL_PARAMS_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, size);

    cmd->phyMode     = mode;
    cmd->numChannels = numChan;
    A_MEMCPY(cmd->channelList, channelList, numChan * sizeof(A_UINT16));

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_CHANNEL_PARAMS_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_set_link_threshold_params(struct wmi_t *wmip, 
                                A_UINT8     highThreshold_upperVal,
                                A_UINT8     highThreshold_lowerVal,
                                A_UINT8     lowThreshold_upperVal,
                                A_UINT8     lowThreshold_lowerVal,
                                A_UINT32    pollTime)
{
    void    *osbuf;
    A_INT8  size;
    WMI_RSSI_THRESHOLD_PARAMS_CMD *cmd;

    /* These values are in ascending order */
    if( highThreshold_upperVal <= highThreshold_lowerVal ||
        lowThreshold_upperVal  <= lowThreshold_lowerVal  ||
        highThreshold_lowerVal <= lowThreshold_upperVal)
        return A_EINVAL;

    size = sizeof (*cmd);

    osbuf = a_netbuf_alloc(size);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, size);

    cmd = (WMI_RSSI_THRESHOLD_PARAMS_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, size);

    cmd->highThreshold_upperVal = highThreshold_upperVal;
    cmd->highThreshold_lowerVal = highThreshold_lowerVal;
    cmd->lowThreshold_upperVal  = lowThreshold_upperVal;
    cmd->lowThreshold_lowerVal  = lowThreshold_lowerVal;
    cmd->pollTime               = pollTime;

    return (wmi_cmd_send(wmip, osbuf, WMI_RSSI_THRESHOLD_PARAMS_CMDID,
                            NO_SYNC_WMIFLAG));
}


A_STATUS
wmi_set_error_report_bitmask(struct wmi_t *wmip, A_UINT32 mask)
{
    void    *osbuf;
    A_INT8  size;
    WMI_TARGET_ERROR_REPORT_BITMASK *cmd;

    size = sizeof (*cmd);

    osbuf = a_netbuf_alloc(size);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, size);

    cmd = (WMI_TARGET_ERROR_REPORT_BITMASK *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, size);

    cmd->bitmask = mask;

    return (wmi_cmd_send(wmip, osbuf, WMI_TARGET_ERROR_REPORT_BITMASK_CMDID,
                            NO_SYNC_WMIFLAG));
}


A_STATUS
wmi_get_stats_cmd(struct wmi_t *wmip)
{
    void *osbuf;

    osbuf = a_netbuf_alloc(0);      /* no payload */
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_GET_STATISTICS_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_addBadAp_cmd(struct wmi_t *wmip, A_UINT8 apIndex, A_UINT8 *bssid)
{
    void *osbuf;
    WMI_ADD_BAD_AP_CMD *cmd;

    if ((bssid == NULL) || (apIndex > WMI_MAX_BAD_AP_INDEX)) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_ADD_BAD_AP_CMD *)(a_netbuf_to_data(osbuf));
    cmd->badApIndex = apIndex;
    A_MEMCPY(cmd->bssid, bssid, sizeof(cmd->bssid));

    return (wmi_cmd_send(wmip, osbuf, WMI_ADD_BAD_AP_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_deleteBadAp_cmd(struct wmi_t *wmip, A_UINT8 apIndex)
{
    void *osbuf;
    WMI_DELETE_BAD_AP_CMD *cmd;

    if (apIndex > WMI_MAX_BAD_AP_INDEX) {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_DELETE_BAD_AP_CMD *)(a_netbuf_to_data(osbuf));
    cmd->badApIndex = apIndex;

    return (wmi_cmd_send(wmip, osbuf, WMI_DELETE_BAD_AP_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_set_txPwr_cmd(struct wmi_t *wmip, A_UINT8 dbM)
{
    void *osbuf;
    WMI_SET_TX_PWR_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_SET_TX_PWR_CMD *)(a_netbuf_to_data(osbuf));
    cmd->dbM = dbM;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_TX_PWR_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_get_txPwr_cmd(struct wmi_t *wmip)
{
    void *osbuf;

    osbuf = a_netbuf_alloc(0);      /* no payload */
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_GET_TX_PWR_CMDID, NO_SYNC_WMIFLAG));
}

A_INT8
wmi_get_mapped_qos_queue(struct wmi_t *wmip, A_UINT8 dir, A_UINT8 trafficClass)
{
    if ((dir == UPLINK_TRAFFIC || dir == DNLINK_TRAFFIC) &&
        (trafficClass <= WMM_AC_VO)) {
        return wmip->wmi_trafficClassMap[dir][trafficClass];
    } else {
        return WMI_NOT_MAPPED;
    }
}

A_STATUS
wmi_dset_open_reply(struct wmi_t *wmip,
                    A_UINT32 status,
                    A_UINT32 access_cookie,
                    A_UINT32 dset_size,
                    A_UINT32 dset_version,
                    A_UINT32 targ_handle,
                    A_UINT32 targ_reply_fn,
                    A_UINT32 targ_reply_arg)
{
    void *osbuf;
    WMIX_DSETOPEN_REPLY_CMD *open_reply;

    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi_dset_open_reply: wmip=0x%x\n", (int)wmip);

    osbuf = a_netbuf_alloc(sizeof(*open_reply));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*open_reply));
    open_reply = (WMIX_DSETOPEN_REPLY_CMD *)(a_netbuf_to_data(osbuf));

    open_reply->status                   = status;
    open_reply->targ_dset_handle         = targ_handle;
    open_reply->targ_reply_fn            = targ_reply_fn;
    open_reply->targ_reply_arg           = targ_reply_arg;
    open_reply->access_cookie            = access_cookie;
    open_reply->size                     = dset_size;
    open_reply->version                  = dset_version;

    return (wmi_cmd_send_xtnd(wmip, osbuf, WMIX_DSETOPEN_REPLY_CMDID,
                             NO_SYNC_WMIFLAG));
}

#if CONFIG_HOST_DSET_SUPPORT
A_STATUS
wmi_dset_data_reply(struct wmi_t *wmip,
                    A_UINT32 status,
                    A_UINT8 *user_buf,
                    A_UINT32 length,
                    A_UINT32 targ_buf,
                    A_UINT32 targ_reply_fn,
                    A_UINT32 targ_reply_arg)
{
    void *osbuf;
    WMIX_DSETDATA_REPLY_CMD *data_reply;
    int size;

    size = sizeof(*data_reply) + length;

    WMI_DEBUG_PRINTF(ATH_LOG_INF,"wmi_dset_data_reply: length=%d status=%d\n", length, status);

    osbuf = a_netbuf_alloc(size);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }
    a_netbuf_put(osbuf, size);
    data_reply = (WMIX_DSETDATA_REPLY_CMD *)(a_netbuf_to_data(osbuf));

    data_reply->status                     = status;
    data_reply->targ_buf                   = targ_buf;
    data_reply->targ_reply_fn              = targ_reply_fn;
    data_reply->targ_reply_arg             = targ_reply_arg;
    data_reply->length                     = length;

    if (status == A_OK) {
        if (a_copy_from_user(data_reply->buf, user_buf, length)) {
            return A_ERROR;
        }
    }

    return (wmi_cmd_send_xtnd(wmip, osbuf, WMIX_DSETDATA_REPLY_CMDID,
                             NO_SYNC_WMIFLAG));
}
#endif /* CONFIG_HOST_DSET_SUPPORT */

#if CONFIG_HOST_GPIO_SUPPORT
/* Send a command to Target to change GPIO output pins. */
A_STATUS
wmi_gpio_output_set(struct wmi_t *wmip,
                    A_UINT32 set_mask,
                    A_UINT32 clear_mask,
                    A_UINT32 enable_mask,
                    A_UINT32 disable_mask)
{
    void *osbuf;
    WMIX_GPIO_OUTPUT_SET_CMD *output_set;
    int size;

    size = sizeof(*output_set);

    WMI_DEBUG_PRINTF(ATH_LOG_INF,
		             "wmi_gpio_output_set: set=0x%x clear=0x%x enb=0x%x dis=0x%x\n",
                     set_mask, clear_mask, enable_mask, disable_mask);

    osbuf = a_netbuf_alloc(size);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }
    a_netbuf_put(osbuf, size);
    output_set = (WMIX_GPIO_OUTPUT_SET_CMD *)(a_netbuf_to_data(osbuf));

    output_set->set_mask                   = set_mask;
    output_set->clear_mask                 = clear_mask;
    output_set->enable_mask                = enable_mask;
    output_set->disable_mask               = disable_mask;

    return (wmi_cmd_send_xtnd(wmip, osbuf, WMIX_GPIO_OUTPUT_SET_CMDID,
                             NO_SYNC_WMIFLAG));
}

/* Send a command to the Target requesting state of the GPIO input pins */
A_STATUS
wmi_gpio_input_get(struct wmi_t *wmip)
{
    void *osbuf;

    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi_gpio_input_get\n");

    osbuf = a_netbuf_alloc(0);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    return (wmi_cmd_send_xtnd(wmip, osbuf, WMIX_GPIO_INPUT_GET_CMDID,
                             NO_SYNC_WMIFLAG));
}

/* Send a command to the Target that changes the value of a GPIO register. */
A_STATUS
wmi_gpio_register_set(struct wmi_t *wmip,
                      A_UINT32 gpioreg_id,
                      A_UINT32 value)
{
    void *osbuf;
    WMIX_GPIO_REGISTER_SET_CMD *register_set;
    int size;

    size = sizeof(*register_set);

    WMI_DEBUG_PRINTF(ATH_LOG_INF, 
		"wmi_gpio_register_set: reg=%d value=0x%x\n",
        gpioreg_id, value);

    osbuf = a_netbuf_alloc(size);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }
    a_netbuf_put(osbuf, size);
    register_set = (WMIX_GPIO_REGISTER_SET_CMD *)(a_netbuf_to_data(osbuf));

    register_set->gpioreg_id               = gpioreg_id;
    register_set->value                    = value;

    return (wmi_cmd_send_xtnd(wmip, osbuf, WMIX_GPIO_REGISTER_SET_CMDID,
                             NO_SYNC_WMIFLAG));
}

/* Send a command to the Target to fetch the value of a GPIO register. */
A_STATUS
wmi_gpio_register_get(struct wmi_t *wmip,
                      A_UINT32 gpioreg_id)
{
    void *osbuf;
    WMIX_GPIO_REGISTER_GET_CMD *register_get;
    int size;

    size = sizeof(*register_get);

    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi_gpio_register_get: reg=%d\n", gpioreg_id);

    osbuf = a_netbuf_alloc(size);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }
    a_netbuf_put(osbuf, size);
    register_get = (WMIX_GPIO_REGISTER_GET_CMD *)(a_netbuf_to_data(osbuf));

    register_get->gpioreg_id               = gpioreg_id;

    return (wmi_cmd_send_xtnd(wmip, osbuf, WMIX_GPIO_REGISTER_GET_CMDID,
                             NO_SYNC_WMIFLAG));
}

/* Send a command to the Target acknowledging some GPIO interrupts. */
A_STATUS
wmi_gpio_intr_ack(struct wmi_t *wmip,
                  A_UINT32 ack_mask)
{
    void *osbuf;
    WMIX_GPIO_INTR_ACK_CMD *intr_ack;
    int size;

    size = sizeof(*intr_ack);

    WMI_DEBUG_PRINTF(ATH_LOG_INF, "wmi_gpio_intr_ack: ack_mask=0x%x\n", ack_mask);

    osbuf = a_netbuf_alloc(size);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }
    a_netbuf_put(osbuf, size);
    intr_ack = (WMIX_GPIO_INTR_ACK_CMD *)(a_netbuf_to_data(osbuf));

    intr_ack->ack_mask               = ack_mask;

    return (wmi_cmd_send_xtnd(wmip, osbuf, WMIX_GPIO_INTR_ACK_CMDID,
                             NO_SYNC_WMIFLAG));
}
#endif /* CONFIG_HOST_GPIO_SUPPORT */

A_STATUS
wmi_set_access_params_cmd(struct wmi_t *wmip, A_UINT16 txop, A_UINT8 eCWmin,
                          A_UINT8 eCWmax, A_UINT8 aifsn)
{
    void *osbuf;
    WMI_SET_ACCESS_PARAMS_CMD *cmd;

    if ((eCWmin > WMI_MAX_CW_ACPARAM) || (eCWmax > WMI_MAX_CW_ACPARAM) ||
        (aifsn > WMI_MAX_AIFSN_ACPARAM))
    {
        return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_SET_ACCESS_PARAMS_CMD *)(a_netbuf_to_data(osbuf));
    cmd->txop   = txop;
    cmd->eCWmin = eCWmin;
    cmd->eCWmax = eCWmax;
    cmd->aifsn  = aifsn;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_ACCESS_PARAMS_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_set_retry_limits_cmd(struct wmi_t *wmip, A_UINT8 frameType,
                         A_UINT8 trafficClass, A_UINT8 maxRetries)
{
    void *osbuf;
    WMI_SET_RETRY_LIMITS_CMD *cmd;

    if ((frameType != MGMT_FRAMETYPE) && (frameType != CONTROL_FRAMETYPE) &&
        (frameType != DATA_FRAMETYPE))
    {
        return A_EINVAL;
    }

    if (maxRetries > WMI_MAX_RETRIES) {
        return A_EINVAL;
    }

    if (frameType != DATA_FRAMETYPE) {
        trafficClass = 0;
    }

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_SET_RETRY_LIMITS_CMD *)(a_netbuf_to_data(osbuf));
    cmd->frameType    = frameType;
    cmd->trafficClass = trafficClass;
    cmd->maxRetries   = maxRetries;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_RETRY_LIMITS_CMDID,
                         NO_SYNC_WMIFLAG));
}

void
wmi_get_current_bssid(struct wmi_t *wmip, A_UINT8 *bssid)
{
    if (bssid != NULL) {
        A_MEMCPY(bssid, wmip->wmi_bssid, ATH_MAC_LEN);
    }
}

A_STATUS
wmi_set_opt_mode_cmd(struct wmi_t *wmip, A_UINT8 optMode)
{
    void *osbuf;
    WMI_SET_OPT_MODE_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_SET_OPT_MODE_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->optMode = optMode;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_OPT_MODE_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_opt_tx_frame_cmd(struct wmi_t *wmip,
                      A_UINT8 frmType,
                      A_UINT8 *dstMacAddr,
                      A_UINT8 *bssid,
                      A_UINT16 optIEDataLen,
                      A_UINT8 *optIEData)
{
    void *osbuf;
    WMI_OPT_TX_FRAME_CMD *cmd;
    osbuf = a_netbuf_alloc(optIEDataLen + sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, (optIEDataLen + sizeof(*cmd)));

    cmd = (WMI_OPT_TX_FRAME_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, (optIEDataLen + sizeof(*cmd)));
    
    cmd->frmType 	= frmType;
    cmd->optIEDataLen 	= optIEDataLen;
    cmd->optIEData     = (A_UINT8 *)((int)cmd + sizeof(*cmd));
    A_MEMCPY(cmd->bssid, bssid, sizeof(cmd->bssid));
    A_MEMCPY(cmd->dstAddr, dstMacAddr, sizeof(cmd->dstAddr));    
    A_MEMCPY(cmd->optIEData, optIEData, optIEDataLen);

    return (wmi_cmd_send(wmip, osbuf, WMI_OPT_TX_FRAME_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_set_adhoc_bconIntvl_cmd(struct wmi_t *wmip, A_UINT16 intvl)
{
    void *osbuf;
    WMI_BEACON_INT_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_BEACON_INT_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->beaconInterval = intvl;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_BEACON_INT_CMDID,
            NO_SYNC_WMIFLAG));
}

#ifndef AR6K_FIRMWARE_1_0

A_STATUS
wmi_set_powersave_timers_cmd(struct wmi_t *wmip, A_UINT16 psPollTimeout, A_UINT16 eospTimeout )
{
    void *osbuf;
    WMI_POWERSAVE_TIMERS_CMD  *cmd;

    if ( !psPollTimeout || !eospTimeout ) {
       return A_EINVAL;
    }

    osbuf = a_netbuf_alloc(sizeof(WMI_POWERSAVE_TIMERS_CMD));
    if (osbuf == NULL) {
       return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(WMI_POWERSAVE_TIMERS_CMD));
    cmd = (WMI_POWERSAVE_TIMERS_CMD*) a_netbuf_to_data(osbuf);
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->psPollTimeout = psPollTimeout;
    cmd->eospTimeout = eospTimeout;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_POWERSAVE_TIMERS_CMDID,
              NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_add_krk_cmd(struct wmi_t *wmip, A_UINT8 *krk)
{
    void *osbuf;
    WMI_ADD_KRK_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(*cmd));

    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_ADD_KRK_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    A_MEMCPY(cmd->krk, krk, WMI_KRK_LEN);

    return (wmi_cmd_send(wmip, osbuf, WMI_ADD_KRK_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_delete_krk_cmd(struct wmi_t *wmip)
{
    void *osbuf;

    osbuf = a_netbuf_alloc(0);

    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_DELETE_KRK_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_set_voice_pkt_size_cmd(struct wmi_t *wmip, A_UINT16 voicePktSize)
{
    void *osbuf;
    WMI_SET_VOICE_PKT_SIZE_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_SET_VOICE_PKT_SIZE_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->voicePktSize = voicePktSize;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_VOICE_PKT_SIZE_CMDID,
            NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_set_max_sp_len_cmd(struct wmi_t *wmip, A_UINT8 maxSPLen)
{
    void *osbuf;
    WMI_SET_MAX_SP_LEN_CMD *cmd;

    /* maxSPLen is a two-bit value. If user trys to set anything
     * other than this, then its invalid
     */
    if(maxSPLen & ~0x03)
        return  A_EINVAL;

    osbuf = a_netbuf_alloc(sizeof(*cmd));
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(*cmd));

    cmd = (WMI_SET_MAX_SP_LEN_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->maxSPLen = maxSPLen;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_MAX_SP_LEN_CMDID,
            NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_set_roam_ctrl_cmd(struct wmi_t *wmip, A_UINT8 roamCtrl, A_UINT8 *bssid, A_UINT8 roamMode, A_UINT8 numBss, WMI_BSS_BIAS_INFO *info )
{
    void *osbuf;
    WMI_SET_ROAM_CTRL_CMD *cmd;
    int size;

    if ( roamCtrl > WMI_MAX_ROAM_CTRL_TYPE) {
       return A_EINVAL;
    }

    if ((roamCtrl==WMI_FORCE_ROAM) && (bssid==NULL) ) {
       return A_EINVAL;
    } else if ((roamCtrl==WMI_SET_ROAM_MODE) && (roamMode>WMI_DISABLE_ROAM)) {
       return A_EINVAL;
    } else if (numBss==0) {
       return A_EINVAL;
    }

    size = sizeof(WMI_SET_ROAM_CTRL_CMD);
    if (numBss > 1) {
       size += (numBss-1) * sizeof(WMI_BSS_BIAS_INFO);
    }

    osbuf = a_netbuf_alloc(size);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, size);
    cmd = (WMI_SET_ROAM_CTRL_CMD *)(a_netbuf_to_data(osbuf));
    A_MEMZERO(cmd, size);

    cmd->roamCtrlType = roamCtrl;
    if ( roamCtrl==WMI_FORCE_ROAM ) {
      A_MEMCPY(cmd->info.bssid, bssid, ATH_MAC_LEN);
    } else if ( roamCtrl==WMI_SET_ROAM_MODE ) {
      cmd->info.roamMode = roamMode;
    } else {
      cmd->info.bssBiasInfo.numBss = numBss;
      A_MEMCPY(&cmd->info.bssBiasInfo.bssBias, info, 
               numBss * sizeof(WMI_BSS_BIAS_INFO));
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_ROAM_CTRL_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_get_roam_tbl_cmd(struct wmi_t *wmip)
{
    void *osbuf;

    osbuf = a_netbuf_alloc(0);      /* no payload */
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    return (wmi_cmd_send(wmip, osbuf, WMI_GET_ROAM_TBL_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_get_roam_data_cmd(struct wmi_t *wmip, A_UINT8 roamDataType)
{
    void *osbuf;
    WMI_TARGET_ROAM_DATA *cmd;

    osbuf = a_netbuf_alloc(sizeof(WMI_TARGET_ROAM_DATA)); 
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(WMI_TARGET_ROAM_DATA));
    cmd = (WMI_TARGET_ROAM_DATA *)(a_netbuf_to_data(osbuf));
    cmd->roamDataType = roamDataType;

    return (wmi_cmd_send(wmip, osbuf, WMI_GET_ROAM_DATA_CMDID,
                         NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_enable_rm_cmd(struct wmi_t *wmip, A_BOOL enable)
{
    void *osbuf;
    WMI_ENABLE_RM_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(WMI_ENABLE_RM_CMD));
    if (osbuf==NULL) {
       return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(WMI_ENABLE_RM_CMD));
    cmd = (WMI_ENABLE_RM_CMD*) a_netbuf_to_data(osbuf);
    cmd->enable_radio_measurements = enable;

    return (wmi_cmd_send(wmip, osbuf, WMI_ENABLE_RM_CMDID, NO_SYNC_WMIFLAG));
}

A_STATUS
wmi_set_max_offhome_duration_cmd(struct wmi_t *wmip, A_UINT8 duration)
{
    void *osbuf;
    WMI_SET_MAX_OFFHOME_DURATION_CMD *cmd;

    osbuf = a_netbuf_alloc(sizeof(WMI_SET_MAX_OFFHOME_DURATION_CMD));
    if (osbuf==NULL) {
       return A_NO_MEMORY;
    }

    a_netbuf_put(osbuf, sizeof(WMI_SET_MAX_OFFHOME_DURATION_CMD));
    cmd = (WMI_SET_MAX_OFFHOME_DURATION_CMD*) a_netbuf_to_data(osbuf);
    cmd->max_offhome_duration = duration;

    return (wmi_cmd_send(wmip, osbuf, WMI_SET_MAX_OFFHOME_DURATION_CMDID, 
                NO_SYNC_WMIFLAG));
}

#endif 
