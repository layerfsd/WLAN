//------------------------------------------------------------------------------
// <copyright file="ar6000_api.h" company="Atheros">
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
// This file contains the API to access the OS dependent atheros host driver
// by the WMI or WLAN generic modules.
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _AR6000_API_H_
#define _AR6000_API_H_

#ifdef __cplusplus
extern "C" {
#endif

struct ar6_softc;

void ar6000_ready_event(void *devt, A_UINT8 *macAddr, A_UINT8 phyCap);
A_UINT8 ar6000_trafficclass(A_UINT8 *pktHdr, A_UINT32 ndisUserPriority);
A_STATUS ar6000_control_tx(void *devt, void *osbuf, int endPt);
void ar6000_connect_event(struct ar6_softc *ar, A_UINT16 channel,
                          A_UINT8 *bssid, A_UINT16 listenInterval,
                          A_UINT8 assocReqLen, A_UINT8 assocRespLen,
                          A_UINT8 *assocInfo, A_UINT8 beaconIeLen );
void ar6000_disconnect_event(struct ar6_softc *ar, A_UINT8 reason,
                             A_UINT8 *bssid, A_UINT8 assocRespLen,
                             A_UINT8 *assocInfo);
void ar6000_tkip_micerr_event(struct ar6_softc *ar, A_UINT8 keyid, 
                              A_BOOL ismcast);
void ar6000_bitrate_rx(void *devt, A_INT32 rateKbps);
void ar6000_channelList_rx(void *devt, A_INT8 numChan, A_UINT16 *chanList);
void ar6000_regDomain_event(struct ar6_softc *ar, A_UINT32 regCode);
void ar6000_txPwr_rx(void *devt, A_UINT8 txPwr);
void ar6000_neighborReport_event(struct ar6_softc *ar, int numAps,
                                 WMI_NEIGHBOR_INFO *info);
void ar6000_set_numdataendpts(struct ar6_softc *ar, A_UINT32 num);
void ar6000_scanComplete_event(struct ar6_softc *ar);
void ar6000_targetStats_event(struct ar6_softc *ar,  WMI_TARGET_STATS *pStats);
void ar6000_rssiThreshold_event(struct ar6_softc *, WMI_RSSI_THRESHOLD_VAL);
void ar6000_reportError_event(struct ar6_softc *, WMI_TARGET_ERROR_VAL errorVal);
void ar6000_roam_tbl_event(void *devt, WMI_TARGET_ROAM_TBL *roamTbl);
void ar6000_roam_data_event(void *devt, WMI_TARGET_ROAM_DATA *roamDat);
void ar6000_cac_event(void *devt, A_UINT8 ac, A_UINT8 cac_ind, A_UINT8 st, A_UINT8 *tspecSuggestion);


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

#ifdef __cplusplus
}
#endif

#endif /* _AR6000_API_H_ */
