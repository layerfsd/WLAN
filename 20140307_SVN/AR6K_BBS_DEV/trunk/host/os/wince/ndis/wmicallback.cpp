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

#define _P	L"\r[AR6K: WMICB] "L


enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0,
}WmiCbDebug;


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
	NDIS_DEBUG_PRINTF(1, " \r %s() : \r\n", __FUNCTION__);
	memcpy(m_PermanentAddress, pMacAddress, ETHERNET_MAC_ADDRESS_LENGTH);
	memcpy(m_CurrentAddress, pMacAddress, ETHERNET_MAC_ADDRESS_LENGTH);
	m_WMIReady = true;
	NdisSetEvent(&m_WMIReadyEvent);
}


void
CAR6KMini::WMIConnectIndication(
	IN USHORT		Channel, 
	IN PBYTE 		PeerBSSID,
	IN USHORT		listenInterval,
	IN USHORT		beaconInterval,
	NETWORK_TYPE	networkType, 
	IN BYTE			assocReqLen, 
	IN BYTE			assocRespLen,
	IN PBYTE		assocInfo,
	IN BYTE			beaconIeLen)
	
{
	BYTE       len = 0;
	int        i = 0;
	A_STATUS wmiStatus;
	bss_t * bss;

	NDIS_DEBUG_PRINTF(1,"WMIConnectIndication Enter , %02x:%02x:%02x:%02x:%02x:%02x \r\n",
							PeerBSSID[0],PeerBSSID[1],PeerBSSID[2],PeerBSSID[3],PeerBSSID[4],PeerBSSID[5]);

	/* Set the Listen interval to appropriate value (100/1000 TUs) depending
	 * on the power mode setting. For REC Mode we always indicate 1000TUs to the
	 * AP in the conn. req. but reset it appropriately here upon conn Ind.
	 */
	if( m_80211_PowerMode == Ndis802_11PowerModeFast_PSP ) 
	{
	

		wmiStatus = wmi_listeninterval_cmd((struct wmi_t *)m_pWMI, 
			m_ListenInterval, 0);
	
		if (wmiStatus != A_OK) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR_LOG,"AR6K: ERROR - wmi_listeninterval_cmd failed = %u \r\n", wmiStatus);
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
	if(m_pAssocInfo == NULL ) 
	{
		NDIS_DEBUG_PRINTF(DBG_ERR, " %s() -> malloc faile!! size = %d \r\n",__FUNCTION__, m_AssocReqLen + m_AssocRespLen + m_BeaconIeLen);
		return;
	}
	else 
	{
		memcpy(m_pAssocInfo, assocInfo, m_AssocReqLen + m_AssocRespLen + m_BeaconIeLen);
	}


    // Send Command to Enable or disable Background Scan
	if (m_Config.bkScanEnable) 
	{
	    m_WMIBssFilter = ALL_BUT_BSS_FILTER;
	} 
	else 
	{
	    m_WMIBssFilter = NONE_BSS_FILTER;
    }
	
	wmi_bssfilter_cmd((struct wmi_t *)m_pWMI, m_WMIBssFilter,0);

	//Add the key here for WEP encryption with open Auth or Autoswitch.
	// For Shared Auth, the keys are plumbed before the connect cmd is issued and
	// for WPA/WPA2, the keys are plumbed when the AddKey OID comes.

	if (m_AuthenticationMode == Ndis802_11AuthModeOpen ||
		m_AuthenticationMode == Ndis802_11AuthModeAutoSwitch ) 

	{

		if (m_EncryptionStatus == Ndis802_11WEPEnabled) 
		{
			
			NDIS_DEBUG_PRINTF(DBG_TRACE, "WEP encryption  \r\n");
			
			for (i=0;i<4;i++) 
			{
				if (m_cbKey[i] != 0) 
				{
					NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: adding WEP keys @ index %d \r\n", i);

					wmiStatus = wmi_addKey_cmd((wmi_t *)m_pWMI, (A_UINT8)m_KeyIndex[i], 	
						WEP_CRYPT, m_KeyUsage[i], (A_UINT8)m_cbKey[i], NULL, m_Key[i],
						KEY_OP_INIT_VAL,NULL,NO_SYNC_WMIFLAG);

                    if (wmiStatus != A_OK) 
					{
					    NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: ERROR - wmi_addKey_cmd failed = %u \r\n", wmiStatus);
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

	NDIS_DEBUG_PRINTF(DBG_TRACE,"%s() - Exit !! \r\n",__FUNCTION__);
	return;
}

void
CAR6KMini::WMIDisconnectIndication()
{

	NDIS_DEBUG_PRINTF(DBG_TRACE," \r %s : \r\n", __FUNCTION__);
	
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

	NdisSetEvent (&m_SuspendEvent);
}

A_STATUS
CAR6KMini::WMISendControlPacket(
	IN PVOID            osbuf,
	IN HTC_ENDPOINT_ID  endPoint)
{
	A_STATUS    status = A_OK;

	if (WMI_CONTROL_MBOX != endPoint)
		NdisInterlockedIncrement(&m_TxPending);

	status = Ar6000SendPkt(endPoint,osbuf,AR6K_CONTROL_PKT_TAG);
	
	if (A_OK != status && A_PENDING != status && WMI_CONTROL_MBOX != endPoint) 
	{
		NDIS_DEBUG_PRINTF(DBG_ERR, " %s(), Ar6000SendPkt Fail, status = %d \r\n", __FUNCTION__,status);
		NdisInterlockedDecrement(&m_TxPending);
	}

	return status;
}

void
CAR6KMini::WMIDsetOpenReq(
    unsigned int id,
    unsigned int targHandle,
    unsigned int targReplyFn,
    unsigned int targReplyArg)
{
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() + Enter \r\n", __FUNCTION__);
	Lock();
    wmi_dset_open_reply((wmi_t *)m_pWMI, A_ENOTSUP, 0, 0, 0,
                          targHandle, targReplyFn, targReplyArg);
	Unlock();
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
	Lock();
    wmi_dset_data_reply((wmi_t *)m_pWMI, A_EBUSY, NULL, length, 
                            targBuf, targReplyFn, targReplyArg);
	Unlock();
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
		NDIS_DEBUG_PRINTF(DBG_ERR, "%s() malloc failed \r\n", __FUNCTION__);
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
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() +Enter m_RSSI = %d \r\n", __FUNCTION__,pTargetStats->cservStats.cs_aveBeacon_rssi);
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

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
	Adapter->WMIBitrateIndication(rateKbps);
}

extern "C" void
ar6000_channelList_rx(
	void     *Context, 
	A_INT8    numChan,
	A_UINT16 *chanList)
{
	CAR6KMini *Adapter = (CAR6KMini *)Context;

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
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

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
	Adapter->WMIReadyIndication(macAddr);
}


extern "C" void
ar6000_Set_WMI_Handle(	void     *Context, struct wmi_t *wmi)
//
//  This function is called by WMI when the device sends
//  a WMI_READY packet. In addition to letting us know
//  that the device is ready to receive WMI requests,
//  it contains the MAC address of the device.
//
{
	NDIS_DEBUG_PRINTF(DBG_TRACE, "\r %s() + Enter  \r\n",__FUNCTION__ );
	CAR6KMini *Adapter = (CAR6KMini *)Context;
	Adapter->SetWmiHandle(wmi);
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
ar6000_connect_event(void *Context, A_UINT16 channel, A_UINT8 *bssid,
                     A_UINT16 listenInterval, A_UINT16 beaconInterval,
                     NETWORK_TYPE networkType, A_UINT8 beaconIeLen,
                     A_UINT8 assocReqLen, A_UINT8 assocRespLen,
                     A_UINT8 *assocInfo)
{
	CAR6KMini *Adapter = (CAR6KMini *)Context;

	Adapter->WMIConnectIndication(channel, bssid, listenInterval, 
									beaconInterval,networkType,assocReqLen, 
									assocRespLen, assocInfo, beaconIeLen);	
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

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
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

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
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

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
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

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
	Adapter->WMITargetStatsEvent(pStats);

	return;
}

extern "C" void
ar6000_rssiThreshold_event(
       void *context)
{
	NDIS_DEBUG_PRINTF(DBG_TRACE, "%s() -> Enter ## \r\n", __FUNCTION__);
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

    NDIS_DEBUG_PRINTF(DBG_ERR_LOG, "AR6000 Error on Target. Error = 0x%x \r\n", errorVal);

    /* One error is reported at a time, and errorval is a bitmask */
    if (errorVal & (errorVal - 1)) {
		return;
	}

    NDIS_DEBUG_PRINTF(DBG_ERR_LOG, "AR6000 Error type = ");
    switch(errorVal) {
        case WMI_TARGET_PM_ERR_FAIL:
        case WMI_TARGET_KEY_NOT_FOUND:
        case WMI_TARGET_DECRYPTION_ERR:
        case WMI_TARGET_BMISS:
        case WMI_PSDISABLE_NODE_JOIN:
            NDIS_DEBUG_PRINTF(DBG_ERR_LOG, "%s \r\n", errString[errorVal]);
            break;
        default:
            NDIS_DEBUG_PRINTF(DBG_ERR_LOG,  "INVALID \r\n");
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

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
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

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
    Adapter->WMISetNumDataEndpts(num);
	return;
}

extern "C" void
ar6000_regDomain_event(
        void *context,
        A_UINT32 regCode)
{
	CAR6KMini *Adapter = (CAR6KMini *)context;

	NDIS_DEBUG_PRINTF(DBG_TRACE," \r %s : \r\n", __FUNCTION__);
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

	NDIS_DEBUG_PRINTF(DBG_TRACE," \r %s() : \r\n", __FUNCTION__);
	Adapter->WMINeighborReportEvent(numAps, info);
	return;
}

extern "C" void
ar6000_txPwr_rx(
        void *context, 
        A_UINT8 txPwr)
{
	CAR6KMini *Adapter = (CAR6KMini *)context;

	NDIS_DEBUG_PRINTF(DBG_TRACE, " \r %s : \r\n", __FUNCTION__);
    Adapter->WMITxPwr(txPwr);
	return;
}


extern "C" void
ar6000_roam_tbl_event( void *devt, WMI_TARGET_ROAM_TBL *pTbl )
{
    A_UINT8 i;

    NDIS_DEBUG_PRINTF(DBG_TRACE,"ROAM TABLE NO OF ENTRIES is %d ROAM MODE is %d\n",
              pTbl->numEntries, pTbl->roamMode);
	
    for (i= 0; i < pTbl->numEntries; i++) {
        NDIS_DEBUG_PRINTF(DBG_TRACE, "[%d]bssid %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x ", i,
            pTbl->bssRoamInfo[i].bssid[0], pTbl->bssRoamInfo[i].bssid[1],
            pTbl->bssRoamInfo[i].bssid[2],
            pTbl->bssRoamInfo[i].bssid[3],
            pTbl->bssRoamInfo[i].bssid[4],
            pTbl->bssRoamInfo[i].bssid[5]);
		
        NDIS_DEBUG_PRINTF(DBG_TRACE, "RSSI %d RSSIDT %d LAST RSSI %d UTIL %d ROAM_UTIL %d BIAS %d \r\n",
            pTbl->bssRoamInfo[i].rssi,
            pTbl->bssRoamInfo[i].rssidt,
            pTbl->bssRoamInfo[i].last_rssi,
            pTbl->bssRoamInfo[i].util,
            pTbl->bssRoamInfo[i].roam_util,
            pTbl->bssRoamInfo[i].bias);
    }

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

extern "C" void ar6k_tx_complete(void *Context, HTC_PACKET *pPacket)
{
	CAR6KMini *Adapter = (CAR6KMini *)Context;
	void 	*cookie = (void *)pPacket->pPktContext;

	Adapter->HTCBufferSentEvent(cookie);

}

extern "C" void ar6k_rx(void *Context, HTC_PACKET *pPacket)
{

	CAR6KMini *Adapter = (CAR6KMini *)Context;
	
	struct ndis_mini_buf_t * skb = (struct ndis_mini_buf_t *)pPacket->pPktContext;
	
	A_STATUS        status = pPacket->Status;
	HTC_ENDPOINT_ID   ept = pPacket->Endpoint;
	
	NDIS_DEBUG_PRINTF(0, " %s() + Enter \r\n",__FUNCTION__);

	A_ASSERT((status != A_OK) ||
			(pPacket->pBuffer == (A_NETBUF_DATA(skb) + HTC_HEADER_LEN)));

	
	Adapter->m_RxBuffers[ept]--;

	if (A_SUCCESS(status)) 
	{
		Adapter->Lock();
		A_NETBUF_PUT(skb, pPacket->ActualLength +  HTC_HEADER_LEN);
		A_NETBUF_PULL(skb, HTC_HEADER_LEN);		
		Adapter->Unlock();
	}

	if (status != A_OK) 
	{
		A_NETBUF_FREE(skb);
	}
	else if (Adapter->m_WMIEnabled == TRUE) 
	{
		if (ept == Adapter->m_ControlEp) 
		{
			/*
			* this is a wmi control msg
			*/
			//wmi_control_rx((wmi_t *)ar->arWmi, skb, ar->arPhyCapability);  /* Bug 82893 */
			Adapter->ReceiveWMIControlPacket(pPacket);
		}
		else
		{
			// WMI Data packet (e.g. IP data packet)
			Adapter->ReceiveWMIDataPacket(pPacket);
		}
	}

	if (status != A_ECANCELED) 
	{
		/*
		* HTC provides A_ECANCELED status when it doesn't want to be refilled
		* (probably due to a shutdown)
		*/
		Adapter->AR6KRxRefill(ept);
    }
	NDIS_DEBUG_PRINTF(0, "%s() -Exit \r\n", __FUNCTION__);
}

extern "C"
void ar6000_rx_refill(void *Context, HTC_ENDPOINT_ID Endpoint)
{
	CAR6KMini *Adapter = (CAR6KMini *)Context;

	Adapter->AR6KRxRefill(Endpoint);
}

extern "C"
HTC_SEND_FULL_ACTION ar6000_tx_queue_full(void *Context, HTC_PACKET *pPacket)
{
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet ~ !! \r\n", __FUNCTION__); 
	return HTC_SEND_FULL_KEEP;
}

extern "C"
void ar6000_peer_event(
    void *context,
    A_UINT8 eventCode,
    A_UINT8 *macAddr)
{
    A_UINT8 pos=0;

    //for (pos=0;pos<6;pos++)
    //    printk("%02x: ",*(macAddr+pos));
    //printk("\n");
}



/* indicate tx activity or inactivity on a WMI stream */
extern "C"
void ar6000_indicate_tx_activity(void *devt, A_UINT8 TrafficClass, A_BOOL Active)
{


	CAR6KMini *Adapter = (CAR6KMini *)devt;
	
	Adapter->IndicateTxActivity(TrafficClass,Active);
}



extern "C" void
ar6000_ratemask_rx(void *devt, A_UINT16 ratemask)
{
	
#if 0	//bluebird
    AR_SOFTC_T *ar = (AR_SOFTC_T *)devt;

    ar->arRateMask = ratemask;
    wake_up(&arEvent);
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif //bluebird	
}

extern "C" void
ar6000_keepalive_rx(void *devt, A_UINT8 configured)
{
	
#if 0 //bluebird
    AR_SOFTC_T *ar = (AR_SOFTC_T *)devt;

    ar->arKeepaliveConfigured = configured;
    wake_up(&arEvent);
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif //bluebird	
}


extern "C" void
ar6000_channel_change_event(void *dev, A_UINT16 oldChannel,
                            A_UINT16 newChannel)
{

//    A_PRINTF("Channel Change notification\nOld Channel: %d, New Channel: %d\n",
//             oldChannel, newChannel);
}



extern "C" void
ar6000_hbChallengeResp_event(void *dev, A_UINT32 cookie, A_UINT32 source)
{
	
#if 0	//bluebird
    if (source == APP_HB_CHALLENGE) {
        /* Report it to the app in case it wants a positive acknowledgement */
        ar6000_send_event_to_app(ar, WMIX_HB_CHALLENGE_RESP_EVENTID,
                                 (A_UINT8 *)&cookie, sizeof(cookie));
    } else {
        /* This would ignore the replys that come in after their due time */
        if (cookie == ar->arHBChallengeResp.seqNum) {
            ar->arHBChallengeResp.outstanding = FALSE;
        }
    }
#else	
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif //bluebird	
}


extern "C" void
ar6000_tx_retry_err_event(void *devt)
{
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() : Tx retries reach maximum \r\n", __FUNCTION__);
}


extern "C" void
ar6000_lqThresholdEvent_rx(void *devt, WMI_LQ_THRESHOLD_VAL newThreshold, A_UINT8 lq)
{
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() :lq threshold range %d, lq %d \r\n", __FUNCTION__,newThreshold, lq);
}



extern "C" void
ar6000_dbglog_event(void *dev, A_UINT32 dropped, A_INT8 *buffer, A_UINT32 length)
{

#if 0 //bluebird
#ifdef REPORT_DEBUG_LOGS_TO_APP
    #define MAX_WIRELESS_EVENT_SIZE 252
    /*
     * Break it up into chunks of MAX_WIRELESS_EVENT_SIZE bytes of messages.
     * There seems to be a limitation on the length of message that could be
     * transmitted to the user app via this mechanism.
     */
    A_UINT32 send, sent;

    sent = 0;
    send = dbglog_get_debug_fragment(&buffer[sent], length - sent,
                                     MAX_WIRELESS_EVENT_SIZE);
    while (send) {
        ar6000_send_event_to_app(ar, WMIX_DBGLOG_EVENTID, &buffer[sent], send);
        sent += send;
        send = dbglog_get_debug_fragment(&buffer[sent], length - sent,
                                         MAX_WIRELESS_EVENT_SIZE);
    }
#else
    AR_DEBUG_PRINTF("Dropped logs: 0x%x\nDebug info length: %d\n",
                    dropped, length);

    /* Interpret the debug logs */
    dbglog_parse_debug_logs(buffer, length);
#endif /* REPORT_DEBUG_LOGS_TO_APP */
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif //bluebird
}


extern "C" HTC_ENDPOINT_ID
ar6000_ac2_endpoint_id ( void * devt, A_UINT8 ac)
{
	
    CAR6KMini *Adapter = (CAR6KMini *)devt;

    return(Adapter->Ac2EndpointID(ac));

}


extern "C" void
ar6000_wow_list_event(struct ar6_softc *ar, A_UINT8 num_filters, WMI_GET_WOW_LIST_REPLY *wow_reply)
{
#if 0 //bluebird
    A_UINT8 i,j;

    /*Each event now contains exactly one filter, see bug 26613*/
    A_PRINTF("WOW pattern %d of %d patterns\n", wow_reply->this_filter_num,                 wow_reply->num_filters);
    A_PRINTF("wow mode = %s host mode = %s\n",
            (wow_reply->wow_mode == 0? "disabled":"enabled"),
            (wow_reply->host_mode == 1 ? "awake":"asleep"));


    /*If there are no patterns, the reply will only contain generic
      WoW information. Pattern information will exist only if there are
      patterns present. Bug 26716*/

   /* If this event contains pattern information, display it*/
    if (wow_reply->this_filter_num) {
        i=0;
        A_PRINTF("id=%d size=%d offset=%d\n",
                    wow_reply->wow_filters[i].wow_filter_id,
                    wow_reply->wow_filters[i].wow_filter_size,
                    wow_reply->wow_filters[i].wow_filter_offset);
       A_PRINTF("wow pattern = ");
       for (j=0; j< wow_reply->wow_filters[i].wow_filter_size; j++) {
             A_PRINTF("%2.2x",wow_reply->wow_filters[i].wow_filter_pattern[j]);
        }

        A_PRINTF("\nwow mask = ");
        for (j=0; j< wow_reply->wow_filters[i].wow_filter_size; j++) {
            A_PRINTF("%2.2x",wow_reply->wow_filters[i].wow_filter_mask[j]);
        }
        A_PRINTF("\n");
    }
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif //bluebird	
}


extern "C" void
ar6000_pmkid_list_event(void *devt, A_UINT8 numPMKID, WMI_PMKID *pmkidList,
                        A_UINT8 *bssidList)
{
	
#if 0 //bluebird
    A_UINT8 i, j;

    A_PRINTF("Number of Cached PMKIDs is %d\n", numPMKID);

    for (i = 0; i < numPMKID; i++) {
        A_PRINTF("\nBSSID %d ", i);
            for (j = 0; j < ATH_MAC_LEN; j++) {
                A_PRINTF("%2.2x", bssidList[j]);
            }
        bssidList += (ATH_MAC_LEN + WMI_PMKID_LEN);
        A_PRINTF("\nPMKID %d ", i);
            for (j = 0; j < WMI_PMKID_LEN; j++) {
                A_PRINTF("%2.2x", pmkidList->pmkid[j]);
            }
        pmkidList = (WMI_PMKID *)((A_UINT8 *)pmkidList + ATH_MAC_LEN +
                                  WMI_PMKID_LEN);
    }
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif //bluebird	
}

extern "C"
void ar6000_pspoll_event(void *dev,A_UINT8 aid)
{
	
#if 0 //bluebird
    sta_t *conn=NULL;
    A_BOOL isPsqEmpty = FALSE;

    conn = ieee80211_find_conn_for_aid(ar, aid);

    /* If the PS q for this STA is not empty, dequeue and send a pkt from
     * the head of the q. Also update the More data bit in the WMI_DATA_HDR
     * if there are more pkts for this STA in the PS q. If there are no more
     * pkts for this STA, update the PVB for this STA.
     */
    A_MUTEX_LOCK(&conn->psqLock);
    isPsqEmpty  = A_NETBUF_QUEUE_EMPTY(&conn->psq);
    A_MUTEX_UNLOCK(&conn->psqLock);

    if (isPsqEmpty) {
        /* TODO:No buffered pkts for this STA. Send out a NULL data frame */
    } else {
        struct sk_buff *skb = NULL;

        A_MUTEX_LOCK(&conn->psqLock);
        skb = A_NETBUF_DEQUEUE(&conn->psq);
        A_MUTEX_UNLOCK(&conn->psqLock);
        /* Set the STA flag to PSPolled, so that the frame will go out */
        STA_SET_PS_POLLED(conn);
        ar6000_data_tx(skb, ar->arNetDev);
        STA_CLR_PS_POLLED(conn);

        /* Clear the PVB for this STA if the queue has become empty */
        A_MUTEX_LOCK(&conn->psqLock);
        isPsqEmpty  = A_NETBUF_QUEUE_EMPTY(&conn->psq);
        A_MUTEX_UNLOCK(&conn->psqLock);

        if (isPsqEmpty) {
            wmi_set_pvb_cmd(ar->arWmi, conn->aid, 0);
        }
    }
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif //bluebird
}


extern "C"
void ar6000_dtimexpiry_event(void *dev)
{
	
#if 0 //bluebird
    A_BOOL isMcastQueued = FALSE;
    struct sk_buff *skb = NULL;

    /* If there are no associated STAs, ignore the DTIM expiry event.
     * There can be potential race conditions where the last associated
     * STA may disconnect & before the host could clear the 'Indicate DTIM'
     * request to the firmware, the firmware would have just indicated a DTIM
     * expiry event. The race is between 'clear DTIM expiry cmd' going
     * from the host to the firmware & the DTIM expiry event happening from
     * the firmware to the host.
     */
    if (ar->sta_list_index == 0) {
        return;
    }

    A_MUTEX_LOCK(&ar->mcastpsqLock);
    isMcastQueued = A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq);
    A_MUTEX_UNLOCK(&ar->mcastpsqLock);

    A_ASSERT(isMcastQueued == FALSE);

    /* Flush the mcast psq to the target */
    /* Set the STA flag to DTIMExpired, so that the frame will go out */
    ar->DTIMExpired = TRUE;

    A_MUTEX_LOCK(&ar->mcastpsqLock);
    while (!A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq)) {
        skb = A_NETBUF_DEQUEUE(&ar->mcastpsq);
        A_MUTEX_UNLOCK(&ar->mcastpsqLock);

        ar6000_data_tx(skb, ar->arNetDev);

        A_MUTEX_LOCK(&ar->mcastpsqLock);
    }
    A_MUTEX_UNLOCK(&ar->mcastpsqLock);

    /* Reset the DTIMExpired flag back to 0 */
    ar->DTIMExpired = FALSE;

    /* Clear the LSB of the BitMapCtl field of the TIM IE */
    wmi_set_pvb_cmd(ar->arWmi, MCAST_AID, 0);
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif

}


extern "C" void
ar6000_bssInfo_event_rx(void *dev, A_UINT8 *datap, int len)
{
	CAR6KMini *Adapter = (CAR6KMini *)dev;
	
	Adapter->BssInfoEventRx(datap, len);

}



extern "C" void
ar6000_dbglog_init_done(void *ar)
{

	CAR6KMini *Adapter = (CAR6KMini *)ar;
	
    Adapter->m_DbglogInitDone = TRUE;

}


extern "C" void
ar6000_snrThresholdEvent_rx(void *devt, WMI_SNR_THRESHOLD_VAL newThreshold, A_UINT8 snr)
{
	
#if 0 //bluebird
    WMI_SNR_THRESHOLD_EVENT event;
    AR_SOFTC_T *ar = (AR_SOFTC_T *)devt;

    event.range = newThreshold;
    event.snr = snr;

    ar6000_send_event_to_app(ar, WMI_SNR_THRESHOLD_EVENTID, (A_UINT8 *)&event,
                             sizeof(WMI_SNR_THRESHOLD_EVENT));
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif //bluebird	
}


extern "C" A_UINT32
a_copy_from_user(void *to, const void *from, A_UINT32 n)
{
#if 0 //bluebird
    return(copy_from_user(to, from, n));
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
return -1;
#endif //bluebird
}


unsigned int wlanNodeCaching = 1;
unsigned int logWmiRawMsgs = 0;


extern "C" A_STATUS
ar6000_get_driver_cfg(struct net_device *dev,
                        A_UINT16 cfgParam,
                        void *result)
{

    A_STATUS    ret = A_OK;


#if 1 //bluebird
    switch(cfgParam)
    {
        case AR6000_DRIVER_CFG_GET_WLANNODECACHING:
           *((A_UINT32 *)result) = wlanNodeCaching;
           break;
        case AR6000_DRIVER_CFG_LOG_RAW_WMI_MSGS:
           *((A_UINT32 *)result) = logWmiRawMsgs;
            break;
        default:
           ret = A_ERROR;
           break;
    }
#else
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() is not ready yet~ \r\n", __FUNCTION__);
#endif //bluebird

    return ret;
}

extern "C" 
void HifRwComplete(void)
{
}


extern "C"
void CheckNDISPktAddress(void *cookie)
{

	struct ar_cookie * ar_cookie = (struct ar_cookie *)cookie;
	void* pktbuf;
	PAR6K_TRANSMIT_BUFFER pTxBuffer;


	pktbuf = (void *) ar_cookie->arc_bp[0];
	pTxBuffer = (PAR6K_TRANSMIT_BUFFER) pktbuf;
		
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() pktbuf =  0x%x, pTxBuffer->NDISBuffer = 0x%x \r\n", __FUNCTION__, (A_UINT32)pktbuf, pTxBuffer->pNdisPacket);	

	
}
	
