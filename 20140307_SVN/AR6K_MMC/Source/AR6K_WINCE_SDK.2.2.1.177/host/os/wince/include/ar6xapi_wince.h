//------------------------------------------------------------------------------
// <copyright file="ar6kapi_wince.h" company="Atheros">
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
// API declerations
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef _AR6XAPI_WINCE_H
#define _AR6XAPI_WINCE_H

#ifdef __cplusplus
extern "C" {
#endif

struct ar6_softc;

void ar6000_ready_event(void *devt, A_UINT8 *datap, A_UINT8 phyCap, A_UINT32 vers);
A_STATUS ar6000_control_tx(void *devt, void *osbuf, HTC_ENDPOINT_ID Endpoint);
void ar6000_connect_event(void *ar, A_UINT16 channel,
                          A_UINT8 *bssid, A_UINT16 listenInterval,
                          A_UINT16 beaconInterval, NETWORK_TYPE networkType,
                          A_UINT8 beaconIeLen, A_UINT8 assocReqLen,
                          A_UINT8 assocRespLen,A_UINT8 *assocInfo);
void ar6000_disconnect_event(void *ar, A_UINT8 reason,
                             A_UINT8 *bssid, A_UINT8 assocRespLen,
                             A_UINT8 *assocInfo, A_UINT16 protocolReasonStatus);
void ar6000_tkip_micerr_event(void *ar, A_UINT8 keyid,
                              A_BOOL ismcast);
void ar6000_bitrate_rx(void *devt, A_INT32 rateKbps);
void ar6000_channelList_rx(void *devt, A_INT8 numChan, A_UINT16 *chanList);
void ar6000_regDomain_event(void *ar, A_UINT32 regCode);
void ar6000_txPwr_rx(void *devt, A_UINT8 txPwr);
void ar6000_keepalive_rx(void *devt, A_UINT8 configured);
void ar6000_neighborReport_event(void *ar, int numAps,
                                 WMI_NEIGHBOR_INFO *info);
void ar6000_set_numdataendpts(void *ar, A_UINT32 num);
void ar6000_scanComplete_event(void *ar, A_STATUS status);
void ar6000_targetStats_event(void *ar,  WMI_TARGET_STATS *pStats);
void ar6000_rssiThreshold_event(void *ar,
                                WMI_RSSI_THRESHOLD_VAL newThreshold,
                                A_INT16 rssi);
void ar6000_reportError_event(void *, WMI_TARGET_ERROR_VAL errorVal);
void ar6000_cac_event(void *ar, A_UINT8 ac, A_UINT8 cac_indication,
                                A_UINT8 statusCode, A_UINT8 *tspecSuggestion);
void ar6000_channel_change_event(void *ar, A_UINT16 oldChannel, A_UINT16 newChannel);
void ar6000_hbChallengeResp_event(void *, A_UINT32 cookie, A_UINT32 source);
void
ar6000_roam_tbl_event(void *ar, WMI_TARGET_ROAM_TBL *pTbl);

void
ar6000_roam_data_event(void *ar, WMI_TARGET_ROAM_DATA *p);

void
ar6000_wow_list_event(void *ar, A_UINT8 num_filters, WMI_GET_WOW_LIST_REPLY *wow_reply);

void ar6000_dset_open_req(void *devt,
                          A_UINT32 id,
                          A_UINT32 targ_handle,
                          A_UINT32 targ_reply_fn,
                          A_UINT32 targ_reply_arg);
void ar6000_dset_close(void *devt, A_UINT32 access_cookie);
void ar6000_dset_data_req(void *devt,
                          A_UINT32 access_cookie,
                          A_UINT32 offset,
                          A_UINT32 length,
                          A_UINT32 targ_buf,
                          A_UINT32 targ_reply_fn,
                          A_UINT32 targ_reply_arg);

void ar6000_gpio_intr_rx(A_UINT32 intr_mask, A_UINT32 input_values);
void ar6000_gpio_data_rx(A_UINT32 reg_id, A_UINT32 value);
void ar6000_gpio_ack_rx(void);

void ar6000_dbglog_init_done(void *ar);

#ifdef SEND_EVENT_TO_APP
void ar6000_send_event_to_app(void *context, A_UINT16 eventId, A_UINT8 *datap, int len);
void ar6000_send_generic_event_to_app(void *context, A_UINT16 eventId, A_UINT8 *datap, int len);
#endif

#ifdef CONFIG_HOST_TCMD_SUPPORT
void ar6000_tcmd_rx_report_event(void *devt, A_UINT8 * results, int len);
#endif

void ar6000_tx_retry_err_event(void *devt);

void ar6000_snrThresholdEvent_rx(void *devt,
                                 WMI_SNR_THRESHOLD_VAL newThreshold,
                                 A_UINT8 snr);

void ar6000_lqThresholdEvent_rx(void *devt, WMI_LQ_THRESHOLD_VAL range, A_UINT8 lqVal);


void ar6000_ratemask_rx(void *devt, A_UINT16 ratemask);

A_STATUS ar6000_get_driver_cfg(void *dev,
                                A_UINT16 cfgParam,
                                void *result);
void ar6000_bssInfo_event_rx(void *ar, A_UINT8 *data, int len);

void ar6000_dbglog_event(void *ar, A_UINT32 dropped,
                         A_UINT8 *buffer, A_UINT32 length);

int ar6000_dbglog_get_debug_logs(void *ar);

void ar6000_indicate_tx_activity(void *devt, A_UINT8 TrafficClass, A_BOOL Active);

void ar6000_cmEnterTimeout(A_ATH_TIMER arg);
void ar6000_cmExitTimeout( A_ATH_TIMER arg);
void ar6000_cmRssiScanTimeout( A_ATH_TIMER arg);
void ar6000_rekeyingTimeout (A_ATH_TIMER arg);

HTC_ENDPOINT_ID  ar6000_ac2_endpoint_id ( void * devt, A_UINT8 ac);
A_UINT8 ar6000_endpoint_id2_ac (void * devt, HTC_ENDPOINT_ID ep );
void ar6000_pmkid_list_event(void *devt, A_UINT8 numPMKID,
                             WMI_PMKID *pmkidList, A_UINT8 *bssidList);

void ar6000_peer_event(void *ar, A_UINT8 eventCode, A_UINT8 *bssid);

A_UINT32 ar6000_getnodeAge (void);

A_UINT32 ar6000_getclkfreq (void);

#ifdef OS_ROAM_MANAGEMENT
void ar6000_scan_indication(void *context, void *si, A_UINT32 len);
#endif

void ar6000_pspoll_event (void *context, A_UINT16 aid);

void ar6000_dtimexpiry_event (void *context);


#ifdef __cplusplus
}
#endif
#endif
