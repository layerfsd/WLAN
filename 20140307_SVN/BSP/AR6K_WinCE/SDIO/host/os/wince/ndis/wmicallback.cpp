//------------------------------------------------------------------------------
// <copyright file="wmicallback.cpp" company="Atheros and Microsoft">
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
// Callback handlers for handling event indications from WMI
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================

#include <windows.h>
#include <ndis.h>

#include "htc_internal.h"
#include "htc.h"
#include "wmi_api.h"
#include "dset_api.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
#include "ieee80211.h"
#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include "osapi.h"

// Handlers for WMI callbacks

void
CAR6KMini::WMIBitrateIndication(
	IN ULONG BitrateKbps)
{
	m_BitrateKpbs = BitrateKbps;
}

void
CAR6KMini::WMIChannelListIndication(
	IN USHORT *pChannelList,
	IN BYTE    numChannels)
{
	if (numChannels < sizeof(m_ChannelList) / sizeof(m_ChannelList[0]))
	{
		memcpy(m_ChannelList, pChannelList, numChannels * sizeof(USHORT));
		m_cChannelList = numChannels;
	}
}

void
CAR6KMini::WMIReadyIndication(
	IN PBYTE pMacAddress)
{
	memcpy(m_PermanentAddress, pMacAddress, ETHERNET_MAC_ADDRESS_LENGTH);
	memcpy(m_CurrentAddress, pMacAddress, ETHERNET_MAC_ADDRESS_LENGTH);
	m_WMIReady = true;
	NdisSetEvent(&m_WMIReadyEvent);
}

void
CAR6KMini::WMIConnectIndication(
	IN USHORT Channel,
	IN PBYTE  PeerBSSID,
	IN USHORT listenInterval,
	IN BYTE   assocReqLen,
	IN BYTE   assocRespLen, 
	IN PBYTE  assocInfo,
	IN BYTE   beaconIeLen)
{
	BYTE       len = 0;
	int        i = 0;
	A_STATUS wmiStatus;
	bss_t * bss;

	/* Set the Listen interval to appropriate value (100/1000 TUs) depending
	 * on the power mode setting. For REC Mode we always indicate 1000TUs to the
	 * AP in the conn. req. but reset it appropriately here upon conn Ind.
	 */
	if( m_80211_PowerMode == Ndis802_11PowerModeFast_PSP ) {

		wmiStatus = wmi_listeninterval_cmd((struct wmi_t *)m_pWMI, 
			m_ListenInterval, 0);
	
		if (wmiStatus != A_OK) {
			NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: ERROR - wmi_listeninterval_cmd failed = %u\n", wmiStatus);
		}
	}

	/* Set the Listen interval to appropriate value (100/1000 TUs) depending
	 * on the power mode setting. For REC Mode we always indicate 1000TUs to the
	 * AP in the conn. req. but reset it appropriately here upon conn Ind.
	 */
	if( m_80211_PowerMode == Ndis802_11PowerModeFast_PSP ){

		wmiStatus = wmi_listeninterval_cmd((struct wmi_t *)m_pWMI, m_ListenInterval,0);
	
		if (wmiStatus != A_OK) {
			NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: ERROR - wmi_listeninterval_cmd failed = %u\n", wmiStatus);
		}
	}

	memcpy(m_PeerBSSID, PeerBSSID, ETHERNET_MAC_ADDRESS_LENGTH);
	m_ConnectedChannel = Channel;
	m_Connected = true;
	m_ConnectInProgress = false;
	m_AssocReqLen = assocReqLen;
	m_AssocRespLen = assocRespLen;
	m_BeaconIeLen = beaconIeLen;
	if (m_pAssocInfo != NULL) {
		A_FREE(m_pAssocInfo);
	}

	m_pAssocInfo = (PBYTE)malloc(m_AssocReqLen + m_AssocRespLen + m_BeaconIeLen);
	memcpy(m_pAssocInfo, assocInfo, m_AssocReqLen + m_AssocRespLen + m_BeaconIeLen);

    // Send Command to Enable or disable Background Scan
	if (m_Config.bkScanEnable) {
	    m_WMIBssFilter = ALL_BUT_BSS_FILTER;
	} else {
	    m_WMIBssFilter = NONE_BSS_FILTER;
    }
	wmi_bssfilter_cmd((struct wmi_t *)m_pWMI, m_WMIBssFilter);

	//Add the key here for WEP encryption with open Auth or Autoswitch.
	// For Shared Auth, the keys are plumbed before the connect cmd is issued and
	// for WPA/WPA2, the keys are plumbed when the AddKey OID comes.

	if (m_AuthenticationMode == Ndis802_11AuthModeOpen ||
		m_AuthenticationMode == Ndis802_11AuthModeAutoSwitch ) {

		if (m_EncryptionStatus == Ndis802_11WEPEnabled) {
			NDIS_DEBUG_PRINTF(ATH_LOG_INF, "WEP encryption \n");
			for (i=0;i<4;i++) {
				if (m_cbKey[i] != 0) {
					NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: adding WEP keys @ index %d\n", i);
					wmiStatus = wmi_addKey_cmd((wmi_t *)m_pWMI, (A_UINT8)m_KeyIndex[i], 
						WEP_CRYPT, m_KeyUsage[i], (A_UINT8)m_cbKey[i], NULL, m_Key[i]);
                    if (wmiStatus != A_OK) {
					    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_addKey_cmd failed = %u\n", wmiStatus);
                    }
				}
			}
		}
	}

	// Indicate MEDIA_CONNECT to Ndis
	NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_CONNECT, 0, 0);
	NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
	bss=wmi_find_node((wmi_t *)m_pWMI,m_PeerBSSID);
    if (bss != NULL) {
	    m_beaconInterval = bss->ni_cie.ie_beaconInt;
    }
	return;
}

void
CAR6KMini::WMIDisconnectIndication()
{

    if (m_Connected) {
	    NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
	    NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
    }
    
	m_Connected = false;
	m_ConnectInProgress = false;
	m_ConnectedChannel = 0;
	memset(m_PeerBSSID, 0, ETHERNET_MAC_ADDRESS_LENGTH);


	if (m_WantToBeConnected)
	{
		// We have completed disconnecting and now want to connect to a new SSID.
		// Start that process.
		SendWMIConnectCommand(&m_SSID);
	}
}

A_STATUS
CAR6KMini::WMISendControlPacket(
	IN PVOID            osbuf,
	IN HTC_ENDPOINT_ID  endPoint)
{
	A_STATUS    status;

	if (WMI_CONTROL_MBOX != endPoint)
		NdisInterlockedIncrement(&m_TxPending);

	status = HTCBufferSend(m_pHTCTarget, endPoint, a_netbuf_to_data(osbuf), a_netbuf_to_len(osbuf), osbuf);
	
	if (A_OK != status && A_PENDING != status && WMI_CONTROL_MBOX != endPoint)
		NdisInterlockedDecrement(&m_TxPending);

	return status;
}

void
CAR6KMini::WMIDsetOpenReq(
    unsigned int id,
    unsigned int targHandle,
    unsigned int targReplyFn,
    unsigned int targReplyArg)
{
    wmi_dset_open_reply((wmi_t *)m_pWMI, A_ENOTSUP, 0, 0, 0,
                          targHandle, targReplyFn, targReplyArg);
}

void
CAR6KMini::WMIDsetDataReq(
   unsigned int accessCookie,
   unsigned int offset,
   unsigned int length,
   unsigned int targBuf,
   unsigned int targReplyFn,
   unsigned int targReplyArg)
{
    wmi_dset_data_reply((wmi_t *)m_pWMI, A_EBUSY, NULL, length, 
                            targBuf, targReplyFn, targReplyArg);
}

void 
CAR6KMini::WMISetNumDataEndpts(
        unsigned int num) 
{
    m_NumDataEndpts = num;
    return;
}

void
CAR6KMini::WMIRegDomainEvent(
       unsigned int regCode)
{
    m_RegCode = regCode;
    return;
}

void
CAR6KMini::WMINeighborReportEvent(
       unsigned int numAps,
	   WMI_NEIGHBOR_INFO *info)
{
#ifdef SUPPORT_WPA2
	NDIS_802_11_PMKID_CANDIDATE_LIST *pOidList;
    PMKID_CANDIDATE                  *curCandidate;
    unsigned int                      lBuf, i;

    // Return if Authentication Mode is not WPA2 or WPA2-PSK
    if((m_AuthenticationMode != Ndis802_11AuthModeWPA2) && (m_AuthenticationMode != Ndis802_11AuthModeWPA2PSK)) {
	    return;
    }
	    
	struct ar6kPmkIdIndication {
		NDIS_802_11_STATUS_INDICATION         ind;
		NDIS_802_11_PMKID_CANDIDATE_LIST      req;
	} *pAr6kPmkIdEvent;

    /*
     * XXX the buffer to be sent up is dynamically allocated.
     * The assumption is made that it can be freed after
     * NdisMIndicateStatusComplete is called.
     * This needs to be checked.
     */

    lBuf = sizeof(*pAr6kPmkIdEvent) + (numAps - 1) * sizeof(PMKID_CANDIDATE);
    pAr6kPmkIdEvent = (struct ar6kPmkIdIndication *)A_MALLOC(lBuf);
    if (!pAr6kPmkIdEvent) {
        return;
    }

    A_MEMZERO(pAr6kPmkIdEvent, lBuf);

	pAr6kPmkIdEvent->ind.StatusType = (NDIS_802_11_STATUS_TYPE)Ndis802_11StatusType_PMKID_CandidateList;
	pOidList = (NDIS_802_11_PMKID_CANDIDATE_LIST *)(&pAr6kPmkIdEvent->req);

    /* Fill up the OID's candidate list */
    curCandidate = pOidList->CandidateList;

    for (i=0; i < numAps; info++, curCandidate++, i++) {
        /* Get the preauth capability of the AP */
		if (info->bssFlags & WMI_PREAUTH_CAPABLE_BSS) {
			curCandidate->Flags = NDIS_802_11_PMKID_CANDIDATE_PREAUTH_ENABLED;
		} else {
			curCandidate->Flags = 0;
		}

        /* Get the Bssid */
        A_MEMCPY(&curCandidate->BSSID,
		 info->bssid,
                 sizeof(NDIS_802_11_MAC_ADDRESS));
    }

    /* Fill up the OID header */
    pOidList->Version = 1;
    pOidList->NumCandidates = numAps;

    NdisMIndicateStatus(m_MiniportAdapterHandle,
                        NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
                        pAr6kPmkIdEvent, lBuf);
    NdisMIndicateStatusComplete(m_MiniportAdapterHandle);

    /* Free the allocated buffer. Assumption described earlier */
    A_FREE(pAr6kPmkIdEvent);
#endif // SUPPORT_WPA2

	return;
}

void
CAR6KMini::WMITxPwr(
     unsigned char txPwr)
{
    m_TxPwr = txPwr;
    return;
}

void
CAR6KMini::WMIMicErrEvent(
	 A_UINT8 keyid, A_BOOL isMcast)
{
	struct ar6kAuthIndication {
		NDIS_802_11_STATUS_INDICATION         ind;
		NDIS_802_11_AUTHENTICATION_REQUEST    req;
	} ar6kAuthEvent;

	ar6kAuthEvent.ind.StatusType = Ndis802_11StatusType_Authentication;
	if (isMcast) {
		ar6kAuthEvent.req.Flags = NDIS_802_11_AUTH_REQUEST_GROUP_ERROR;
	} else {
		ar6kAuthEvent.req.Flags = NDIS_802_11_AUTH_REQUEST_PAIRWISE_ERROR;
	}

	memcpy(&ar6kAuthEvent.req.Bssid[0], m_PeerBSSID, ETHERNET_MAC_ADDRESS_LENGTH);
	ar6kAuthEvent.req.Length = sizeof(NDIS_802_11_AUTHENTICATION_REQUEST);

    NdisMIndicateStatus(m_MiniportAdapterHandle,
                        NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
                        &ar6kAuthEvent, sizeof(ar6kAuthEvent));
    NdisMIndicateStatusComplete(m_MiniportAdapterHandle);

	return;
}

void
CAR6KMini::WMITargetStatsEvent(
	WMI_TARGET_STATS *pTargetStats)
{
	//Update the RSSI for now.
	m_RSSI = pTargetStats->cservStats.cs_aveBeacon_rssi;

	//Update other target Stats.
	m_tgtStats.TransmittedFragmentCount.QuadPart += pTargetStats->txrxStats.tx_stats.tx_packets;
	m_tgtStats.MulticastTransmittedFrameCount.QuadPart += (pTargetStats->txrxStats.tx_stats.tx_multicast_pkts + \
								pTargetStats->txrxStats.tx_stats.tx_broadcast_pkts);
	m_tgtStats.FailedCount.QuadPart += pTargetStats->txrxStats.tx_stats.tx_failed_cnt;
	m_tgtStats.RetryCount.QuadPart += pTargetStats->txrxStats.tx_stats.tx_retry_cnt;
	m_tgtStats.RTSSuccessCount.QuadPart += pTargetStats->txrxStats.tx_stats.tx_rts_success_cnt;
	m_tgtStats.RTSFailureCount.QuadPart += pTargetStats->txrxStats.tx_stats.tx_rts_fail_cnt;
	m_tgtStats.FrameDuplicateCount.QuadPart += pTargetStats->txrxStats.rx_stats.rx_duplicate_frames;
	m_tgtStats.ReceivedFragmentCount.QuadPart += pTargetStats->txrxStats.rx_stats.rx_packets;
	m_tgtStats.MulticastReceivedFrameCount.QuadPart += (pTargetStats->txrxStats.rx_stats.rx_multicast_pkts + \
								pTargetStats->txrxStats.rx_stats.rx_broadcast_pkts);			
	m_tgtStats.FCSErrorCount.QuadPart += pTargetStats->txrxStats.rx_stats.rx_crcerr;

	//post targetStats event
	NdisSetEvent(&m_tgtStatsEvent);

	return;
}

extern "C" void
ar6000_bitrate_rx(
	void   *Context, 
	A_INT32 rateKbps)
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

	Adapter->WMIBitrateIndication(rateKbps);
}

extern "C" void
ar6000_channelList_rx(
	void     *Context, 
	A_INT8    numChan,
	A_UINT16 *chanList)
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

	Adapter->WMIChannelListIndication(chanList, numChan);
}

extern "C" void
ar6000_ready_event(
	void     *Context, 
	A_UINT8 *macAddr,
    A_UINT8 phyCap)
//
//  This function is called by WMI when the device sends
//  a WMI_READY packet. In addition to letting us know
//  that the device is ready to receive WMI requests,
//  it contains the MAC address of the device.
//
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

	Adapter->WMIReadyIndication(macAddr);
}

extern "C" A_UINT8
ar6000_iphdr_to_tos(A_UINT8 *pkt)
//
//  This function is called by WMI to extract the
//  type of service (TOS) field from an IP packet header.
//
{
	return 1;
}

extern "C" void
ar6000_connect_event(
	void     *Context,
	A_UINT16  channel,
	A_UINT8  *bssid,
	A_UINT16  listenInterval,
	A_UINT8   assocReqLen,
	A_UINT8   assocRespLen,
	A_UINT8  *assocInfo,
        A_UINT8   beaconIeLen)
//
//  This function is called by WMI after receiving a
//  WMI_CONNECT event packet from the device.
//
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Connect EVENT from target\n");
	Adapter->WMIConnectIndication(channel, bssid, listenInterval, 
		                          assocReqLen, assocRespLen, 
                                          assocInfo, beaconIeLen);
}

extern "C" void
ar6000_disconnect_event(
	    void *Context,
        A_UINT8 reason,
        A_UINT8 *bssid, A_UINT8 assocRespLen,
        A_UINT8 *assocInfo)
//
//  This function is called by WMI after receiving a
//  WMI_DISCONNECT event packet from the device.
//
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "DisConnect EVENT from target\n");
	Adapter->WMIDisconnectIndication();
}

extern "C" A_STATUS
ar6000_control_tx(
	void     *Context,
	void     *osbuf, 
	int       endPt)
//
//  This function is called by WMI to send a control
//  packet to the device.
//
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

	return Adapter->WMISendControlPacket(osbuf, (HTC_ENDPOINT_ID)endPt);
}

extern "C" void
ar6000_dset_open_req(
    void *context,
    A_UINT32 id,
    A_UINT32 targHandle,
    A_UINT32 targReplyFn,
    A_UINT32 targReplyArg)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;

    Adapter->WMIDsetOpenReq(id, targHandle, targReplyFn, targReplyArg);
}

extern "C" void
ar6000_dset_close(
    void *context,
    A_UINT32 access_cookie)
{
    return;
}

extern "C" void
ar6000_dset_data_req(
   void *context,
   A_UINT32 accessCookie,
   A_UINT32 offset,
   A_UINT32 length,
   A_UINT32 targBuf,
   A_UINT32 targReplyFn,
   A_UINT32 targReplyArg)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;

    Adapter->WMIDsetDataReq(accessCookie, offset, length, targBuf, targReplyFn, targReplyArg);
}

extern "C" void
ar6000_scanComplete_event(
       void *context)
{
    return;
}

extern "C" void
ar6000_targetStats_event(
	   void *context, WMI_TARGET_STATS *pStats)
{
	CAR6KMini *Adapter = (CAR6KMini *)context;

	Adapter->WMITargetStatsEvent(pStats);

	return;
}

extern "C" void
ar6000_rssiThreshold_event(
       void *context)
{
	return;
}

extern "C" void
ar6000_reportError_event(
       void *context, WMI_TARGET_ERROR_VAL errorVal)
{
    char *errString[] = {
		"",
        "WMI_TARGET_PM_ERR_FAIL", //0x0001
        "WMI_TARGET_KEY_NOT_FOUND", //0x0002
		"",
        "WMI_TARGET_DECRYPTION_ERR", //0x0004
		"",
		"",
		"",
        "WMI_TARGET_BMISS", //0x0008
		"",
		"",
		"",
		"",
		"",
		"",
		"",
        "WMI_PSDISABLE_NODE_JOIN" //0x0010
};

    WMI_DEBUG_PRINTF(ATH_LOG_ERR,
		             "AR6000 Error on Target. Error = 0x%x\n", 
					 errorVal);

    /* One error is reported at a time, and errorval is a bitmask */
    if (errorVal & (errorVal - 1)) {
		return;
	}

    WMI_DEBUG_PRINTF(ATH_LOG_ERR, "AR6000 Error type = ");
    switch(errorVal) {
        case WMI_TARGET_PM_ERR_FAIL:
        case WMI_TARGET_KEY_NOT_FOUND:
        case WMI_TARGET_DECRYPTION_ERR:
        case WMI_TARGET_BMISS:
        case WMI_PSDISABLE_NODE_JOIN:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "%s\n", errString[errorVal]);
            break;
        default:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "INVALID\n");
            break;
    }
}

extern "C" void
ar6000_gpio_intr_rx(A_UINT32 intr_mask, A_UINT32 input_values)
{
	return;
}

extern "C" void
ar6000_gpio_data_rx(A_UINT32 reg_id, A_UINT32 value)
{
	return;
}

extern "C" void
ar6000_gpio_ack_rx(void)
{
	return;
}

extern "C" A_UINT8
ar6000_trafficclass(A_UINT8 *pktHdr, A_UINT32 ndisUserPriority)
{
    A_UINT8 trafficClass = WMM_AC_BE;
#ifdef WMM
    A_UINT16 pMode,vMode;
    A_UINT8 pClass,vClass;
    struct iphdr *ipHdr = (struct iphdr *)pktHdr;

    if(ipHdr) {
        switch(ipHdr->ip_tos) {
	        case 0x08:					/* Background */
		        pMode = WMM_AC_TO_TID(WMM_AC_BK);
		    	pClass = WMM_AC_BK;
		    	break;
	    	case 0x28:					/* Video */
	    	case 0x80:
	        case 0xa0:
		    	pMode = WMM_AC_TO_TID(WMM_AC_VI);
		    	pClass = WMM_AC_VI;
		    	break;
	        case 0x38:					/* Voice */
	        case 0xc0:
	        case 0xe0:
		        pMode = WMM_AC_TO_TID(WMM_AC_VO);
		    	pClass = WMM_AC_VO;
		        break;
	        case 0x00:
	        case 0x18:
	        default:					/* All others / Best Effort */
		    	pMode = WMM_AC_TO_TID(WMM_AC_BE);
		    	pClass = WMM_AC_BE;
        }
    } else {
        pMode = 0;
        pClass = WMM_AC_BE;
    }
    
    vMode = ndisUserPriority & 0x7;
    switch(vMode) {
	    case 0:
	    case 3:						/* Best Effort */
   	    	vClass = WMM_AC_BE;
	    	break;
	    case 1:
	    case 2:						/* Background */
 	    	vClass = WMM_AC_BK;
	    	break;
	    case 4:
	    case 5:						/* video */
            	vClass = WMM_AC_VI;
	    	break;
            case 6:
	    case 7:						/* Voice */
   	    	vClass = WMM_AC_VO;
	    	break;
    }
    trafficClass = (pMode > vMode) ? pClass : vClass;	
#endif //WMM
    return trafficClass;
}

extern "C" void 
ar6000_tkip_micerr_event(
        void *context,
        A_UINT8 keyid, 
        A_BOOL isMcast)
{
	CAR6KMini *Adapter = (CAR6KMini *)context;

	Adapter->WMIMicErrEvent(keyid, isMcast);
    return;
}

extern "C" void 
ar6000_set_numdataendpts(
        void *context, 
        A_UINT32 num) 
{
    CAR6KMini *Adapter = (CAR6KMini *)context;
    
    if (num <= (HTC_MAILBOX_NUM_MAX - 1)) {
		return;
	}
    Adapter->WMISetNumDataEndpts(num);
	return;
}

extern "C" void
ar6000_regDomain_event(
        void *context,
        A_UINT32 regCode)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;

    Adapter->WMIRegDomainEvent(regCode);
	return;
}

extern "C" void
ar6000_neighborReport_event(
        void *context, 
        A_INT32 numAps, 
        WMI_NEIGHBOR_INFO *info)
{
	CAR6KMini *Adapter = (CAR6KMini *)context;

	Adapter->WMINeighborReportEvent(numAps, info);
	return;
}

extern "C" void
ar6000_txPwr_rx(
        void *context, 
        A_UINT8 txPwr)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;

    Adapter->WMITxPwr(txPwr);
	return;
}

extern "C" A_UINT32
a_copy_from_user(void *to, void *from, A_UINT32 n)
{
    NdisMoveMemory(to, from, n);
	return 0;
}

extern "C" void
ar6000_roam_tbl_event( void *devt, WMI_TARGET_ROAM_TBL *roamTbl )
{
}

extern "C" void
ar6000_roam_data_event( void *devt, WMI_TARGET_ROAM_DATA *roamDat )
{
}

extern "C" void
ar6000_cac_event( void *devt, A_UINT8 ac, A_UINT8 cac_ind, 
                  A_UINT8 st, A_UINT8 *tspecSuggestion )
{
}

