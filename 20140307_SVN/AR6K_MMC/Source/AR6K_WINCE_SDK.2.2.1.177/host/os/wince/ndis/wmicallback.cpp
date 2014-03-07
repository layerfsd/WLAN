//------------------------------------------------------------------------------
// <copyright file="wmicallback.cpp" company="Atheros and Microsoft">
//    Copyright (c) 2004-2007 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
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
// Callback handlers for handling event indications from WMI
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#include <windows.h>
#include <ndis.h>

#include "a_config.h"
#include "athdefs.h"
#include "a_types.h"
#include "a_osapi.h"
#include "htc_api.h"
#include "hif.h"
#include "wmi.h"
#include "wlan_api.h"

#include "wmi_api.h"
#include "dset_api.h"
#include "dbglog_api.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
#include "common_drv.h"
#include "ieee80211.h"
#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include "a_debug.h"
#include "host_version.h"


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

#ifdef OS_ROAM_MANAGEMENT
    NdisSetEvent (&m_tgtChannelListEvent);
#endif

}

void
CAR6KMini::WMIReadyIndication(
    IN PBYTE pMacAddress,
    IN A_UINT8 phyCap,
    IN A_UINT32 vers)
{
    memcpy(m_PermanentAddress, pMacAddress, ETHERNET_MAC_ADDRESS_LENGTH);
    memcpy(m_CurrentAddress, pMacAddress, ETHERNET_MAC_ADDRESS_LENGTH);

    switch (phyCap)
    {
        case WMI_11A_CAPABILITY:
            m_NetworkTypeInUse = Ndis802_11OFDM5;
            m_80211PhyCapability = mode_11aOnly;
            break;
        case WMI_11G_CAPABILITY:
            m_NetworkTypeInUse = Ndis802_11OFDM24;
            m_80211PhyCapability = mode_11g;
            break;
        case WMI_11AG_CAPABILITY:
            m_NetworkTypeInUse = Ndis802_11OFDM5;
            m_80211PhyCapability = mode_11ag;
            break;
        default:
            m_NetworkTypeInUse = Ndis802_11DS;
            m_80211PhyCapability = mode_11g;
            break;
    }

    if(m_80211PhyCapability == mode_11g)
    {
        m_queryphyMode = WMI_11G_MODE;
    }
    if(m_80211PhyCapability == mode_11ag)
    {
        m_queryphyMode = WMI_11AG_MODE;
    }

    ATHR_DISPLAY_MSG (L"WLAN ID VERSION ==> %x\n", vers);
    logPrintf (ATH_LOG_INF, "WLAN ID VERSION ==> %x\n", vers);

    if ((AR6K_SW_VERSION == vers) || (0x270F == (vers & 0xFFFF)))
    {
        m_WMIReady = TRUE;
        NdisSetEvent(&m_WMIReadyEvent);
    }
    else
    {
        ATHR_DISPLAY_MSG (L"*** Driver (%x) WLAN (%x) version mismatch ***\n", AR6K_SW_VERSION, vers);
        logPrintf (ATH_LOG_INF, "*** Driver (%x) WLAN (%x) version mismatch ***\n", AR6K_SW_VERSION, vers);
    }
}


void
CAR6KMini::WMIConnectIndication(
    IN USHORT Channel,
    IN PBYTE  PeerBSSID,
    IN USHORT listenInterval,
    IN BYTE   assocReqLen,
    IN BYTE   assocRespLen,
    IN PBYTE  assocInfo,
    IN BYTE   beaconIeLen,
    IN USHORT beaconInterval,
    NETWORK_TYPE networkType)
{
    BYTE        len = 0;
    int         i = 0;
    PBYTE       pAssocReq;
    PBYTE       pAssocReqEnd;
    PBYTE       pBeaconIE;
    BYTE        byKeyType = 0;
#ifdef OS_ROAM_MANAGEMENT
    A_UNTIMEOUT(&m_disconnectIndicationTimer);
#endif
#ifdef NO_BCAST_PROBE_IN_CONNECT
    wmi_probedSsid_cmd((struct wmi_t *)m_pWMI, 0, ANY_SSID_FLAG, 0,NULL);
#endif


    memcpy(m_PeerBSSID, PeerBSSID, ETHERNET_MAC_ADDRESS_LENGTH);
    m_ConnectedChannel      = Channel;
    m_ConnectInProgress     = FALSE;
    m_AssocReqLen           = assocReqLen;
    m_AssocRespLen          = assocRespLen;
    m_BeaconIeLen           = beaconIeLen;
    m_BeaconInterval        = beaconInterval;
    m_WantToBeConnected     = FALSE;

    if (m_pAssocInfo != NULL) {
        A_FREE(m_pAssocInfo);
    }

    m_pAssocInfo = (PBYTE)A_MALLOC(m_AssocReqLen + m_AssocRespLen + m_BeaconIeLen);

    if (m_pAssocInfo)
    {
        memcpy(m_pAssocInfo, assocInfo, m_AssocReqLen + m_AssocRespLen + m_BeaconIeLen);

        //Get network type in use
        if (m_ConnectedChannel >= 2412 && m_ConnectedChannel <= 2484) {
            PBYTE pAssocRsp = m_pAssocInfo + m_BeaconIeLen + m_AssocReqLen;
            PBYTE pAssocRspEnd = pAssocRsp + m_AssocRespLen;

            m_NetworkTypeInUse = Ndis802_11DS;

            //Skip capability, status code and assoc. ID
            pAssocRsp += 6;
            if ((pAssocRsp + 2) < pAssocRspEnd && IEEE80211_ELEMID_RATES == pAssocRsp[0]) {
                pAssocRsp += (pAssocRsp[1] + 2);
                if ((pAssocRsp + 2) < pAssocRspEnd && IEEE80211_ELEMID_XRATES == pAssocRsp[0]) {
                    m_NetworkTypeInUse = Ndis802_11OFDM24;
                }
            }
        }
        else {
            m_NetworkTypeInUse = Ndis802_11OFDM5;
        }

        // Update the group wise crypto type from the ie info
    #define RSN_MULTICAST_CIPHER_OFFSET 7
    #define WPA_MULTICAST_CIPHER_OFFSET 11
        pBeaconIE = m_pAssocInfo;
        if (m_BeaconIeLen) {
            if ((m_AuthenticationMode == Ndis802_11AuthModeWPA2) ||
                (m_AuthenticationMode == Ndis802_11AuthModeWPA2PSK))
            {
                if (m_BeaconIeLen <= RSN_MULTICAST_CIPHER_OFFSET) {
                    // Default to AES if cipher suite not present
                    m_GrpwiseCryptoType = AES_CRYPT;
                } else {
                    byKeyType = *(pBeaconIE + RSN_MULTICAST_CIPHER_OFFSET);
                    switch (byKeyType)
                    {
                        case RSN_CSE_WEP40:
                        case RSN_CSE_WEP104:
                            m_GrpwiseCryptoType = WEP_CRYPT;
                            break;
                        case RSN_CSE_TKIP:
                            m_GrpwiseCryptoType = TKIP_CRYPT;
                            break;
                        case RSN_CSE_CCMP:
                            m_GrpwiseCryptoType = AES_CRYPT;
                            break;
                    }
                }
            } else
                if ((m_AuthenticationMode == Ndis802_11AuthModeWPA) ||
                    (m_AuthenticationMode == Ndis802_11AuthModeWPAPSK))
            {
                if (m_BeaconIeLen <= WPA_MULTICAST_CIPHER_OFFSET) {
                    // Default to TKIP if cipher suite not present
                    m_GrpwiseCryptoType = TKIP_CRYPT;
                } else {
                    byKeyType = *(pBeaconIE + WPA_MULTICAST_CIPHER_OFFSET);
                    switch (byKeyType)
                    {
                        case WPA_CSE_WEP40:
                        case WPA_CSE_WEP104:
                            m_GrpwiseCryptoType = WEP_CRYPT;
                            break;
                        case WPA_CSE_TKIP:
                            m_GrpwiseCryptoType = TKIP_CRYPT;
                            break;
                        case WPA_CSE_CCMP:
                            m_GrpwiseCryptoType = AES_CRYPT;
                            break;
                    }
                }
            }
        }

        // Get supported basic rates
        pAssocReq = m_pAssocInfo + m_BeaconIeLen;
        pAssocReqEnd = pAssocReq + m_AssocReqLen;

        // skip capability and listen interval
        pAssocReq += 4;

        if (((pAssocReq + 2) < pAssocReqEnd) &&
            (IEEE80211_ELEMID_SSID == pAssocReq[0]))
        {
            // Skip SSID
            pAssocReq += (pAssocReq[1] + 2);
            if ((pAssocReq + 2) < pAssocReqEnd &&
                IEEE80211_ELEMID_RATES == pAssocReq[0] &&
                (pAssocReq + pAssocReq[1] + 2) <= pAssocReqEnd)
            {
                // Get rates
                memset (m_pSupportedRates, 0, sizeof(NDIS_802_11_RATES));
                memcpy(m_pSupportedRates, &pAssocReq[2],
                      (pAssocReq[1] < sizeof(NDIS_802_11_RATES)) ? pAssocReq[1] : sizeof(NDIS_802_11_RATES));
            }
        }

    }

#ifdef OS_ROAM_MANAGEMENT
    m_RateInfo.TxDataFrames = 0;
    m_RateInfo.RxDataFrames = 0;
#endif

    // Indicate MEDIA_CONNECT to Ndis

    ConnectIndicationPostProc ();
    if (networkType == INFRA_NETWORK){
        m_Connected         = TRUE;
        m_ConnectInProgress = FALSE;
        NdisMIndicateStatus (m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_CONNECT, 0, 0);
        NdisMIndicateStatusComplete (m_MiniportAdapterHandle);
    }else{
        m_Connected = FALSE;
    }
    m_RSSIlevel = 0;
    if (m_Config.hostAssistedRoaming)
    {
        A_UNTIMEOUT(&m_rssiScanTimer);
        A_TIMEOUT_MS(&m_rssiScanTimer, 1000, 0);
    }

    return;
}

void
CAR6KMini::WMIDisconnectIndication(A_UINT8 reason)
{
#ifdef NO_BCAST_PROBE_IN_CONNECT
    wmi_probedSsid_cmd((struct wmi_t *)m_pWMI, 0, ANY_SSID_FLAG, 0,NULL);
#endif
    do
    {
        //
        // Ignore pyxis specific reason during IEEE Mode
        //
        if (m_Connected)
        {
            if (reason == NO_NETWORK_AVAIL)
            {
                // remove the current associated bssid node
                wmi_free_node ((wmi_t *)m_pWMI, m_PeerBSSID);

                //
                // In case any other same SSID nodes are present
                // remove it, since those nodes also not available now
                //
                IterateNodeAndRemoveSSID (&m_SSID);

                SendWMIDisconnectCommand ();
            }

            // Flush any pending NDIS packets
            FlushNdisPacketTransmitQueue();

#ifdef OS_ROAM_MANAGEMENT
        if (FALSE == m_osRoamControl)
        {
#endif
        m_RSSIlevel = 0;
        if (m_Config.hostAssistedRoaming)
        {
            A_UNTIMEOUT(&m_rssiScanTimer);
        }
            NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
        NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
#ifdef OS_ROAM_MANAGEMENT
        }
        else
        {
            A_UNTIMEOUT(&m_disconnectIndicationTimer);
            A_TIMEOUT_MS(&m_disconnectIndicationTimer, m_Config.discTimeout*1000, 0);
        }
#endif
        //
        // Reset Chanel and BSSID info
        //
        m_Connected         = FALSE;
        m_ChannelHint       = 0;
        m_ConnectedChannel  = 0;
        memset (m_PeerBSSID, 0, ETHERNET_MAC_ADDRESS_LENGTH);
    }

    if (m_bIsSwitchAPtoSTA)
    {
        m_bIsSwitchAPtoSTA = FALSE;
        NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
        NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
       //     configTargetParams();
        m_WantToBeConnected = TRUE;
    }


        {
            //
            // no need to send WMIConnect when target reply disconnect
            // reason other than DISCONNECT_CMD
            //
            if (DISCONNECT_CMD == reason)
            {
                m_ConnectInProgress = FALSE;
                if (m_WantToBeConnected)
                {
                    SendWMIConnectCommand (&m_SSID);
                }
            }
        }

    } while (FALSE);

    NdisSetEvent (&m_SuspendEvent);
}

A_STATUS
CAR6KMini::WMISendControlPacket(
    IN PVOID            osbuf,
    HTC_ENDPOINT_ID     Endpoint)
{
    A_STATUS    status;
    ndis_mini_buf_t *pNb;

    pNb = (ndis_mini_buf_t *)osbuf;

    SET_HTC_PACKET_INFO_TX(&pNb->HtcPacket,
                           osbuf,
                           a_netbuf_to_data(osbuf),
                           a_netbuf_to_len(osbuf),
                           Endpoint,
                           AR6K_CONTROL_PKT_TAG);

    status = HTCSendPkt(m_pHTCTarget, &pNb->HtcPacket);

    if (A_OK != status && A_PENDING != status && m_ControlEp != Endpoint)
        NdisInterlockedDecrement(&m_TxPending);

    return status;
}


void
CAR6KMini::WMIDsetOpenReq(
    A_UINT32 id,
    A_UINT32 targHandle,
    A_UINT32 targReplyFn,
    A_UINT32 targReplyArg)
{
    wmi_dset_open_reply((wmi_t *)m_pWMI, A_ENOTSUP, 0, 0, 0,
                          targHandle, targReplyFn, targReplyArg);
}

void
CAR6KMini::WMIDsetDataReq(
    A_UINT32 accessCookie,
    A_UINT32 offset,
    A_UINT32 length,
    A_UINT32 targBuf,
    A_UINT32 targReplyFn,
    A_UINT32 targReplyArg)
{
    wmi_dset_data_reply((wmi_t *)m_pWMI, A_EBUSY, NULL, length,
                            targBuf, targReplyFn, targReplyArg);
}

void
CAR6KMini::WMISetNumDataEndpts(
    A_UINT32 num)
{
    m_NumDataEndpts = num;
    return;
}

void
CAR6KMini::WMIRegDomainEvent(
    A_UINT32 regCode)
{
    m_RegCode = regCode;
    return;
}

void
CAR6KMini::WMINeighborReportEvent(
   A_UINT32 numAps,
   WMI_NEIGHBOR_INFO *info)
{
   if(m_networkType == AP_NETWORK)
    {
        return;
    }
#ifdef SUPPORT_WPA2
    NDIS_802_11_PMKID_CANDIDATE_LIST *pOidList;
    PMKID_CANDIDATE                  *curCandidate;
    A_UINT32                      lBuf, i;

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
    A_UCHAR txPwr)
{
    m_TxPwr = txPwr;
    NdisSetEvent (&m_tgtTxPwrEvent);
    return;
}

void
CAR6KMini::WMIKeepAliveEvent(
    A_BOOL isConfigured)
{
    m_bkeepaliveConfigured = isConfigured;
    NdisSetEvent (&m_keepAliveEvent);
}

void
CAR6KMini::WMIMicErrEvent(
    A_UINT8 keyid,
    A_BOOL isMcast)
{
   if (m_networkType == AP_NETWORK)
   {
        car6k_ap_mic_event (keyid, isMcast);
        return;
   }
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

    m_MicErrorCount++;

    if(m_MicErrorCount >= MAX_MIC_ERRORS)
    {
        m_CounterMeasureOn = TRUE;
        A_UNTIMEOUT (&m_CounterMeasureEnterTimer);
    }
    else
    {
        A_TIMEOUT_MS(&m_CounterMeasureEnterTimer, MIC_ERROR_TIMEOUT_MSEC, 0);
    }

    NdisMIndicateStatus(m_MiniportAdapterHandle,
                        NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
                        &ar6kAuthEvent, sizeof(ar6kAuthEvent));
    NdisMIndicateStatusComplete(m_MiniportAdapterHandle);

    return;
}

void
CAR6KMini::counterMeasureEnterTimeout()
{
    if (m_networkType == AP_NETWORK)
    {
        m_ap_CounterMeasureOn = FALSE;
        m_ap_mic_failures = 0;
    }
    else
    {
        m_MicErrorCount = 0;
        m_CounterMeasureOn = FALSE;
    }
}

void
CAR6KMini::counterMeasureExitTimeout()
{
    if (m_networkType == AP_NETWORK)
    {
        m_ap_CounterMeasureOn = FALSE;
        m_ap_mic_failures = 0;
        wmi_set_tkip_countermeasures_cmd ((wmi_t *)m_pWMI, 0);
    }

    else
    {
        m_MicErrorCount = 0;
        m_CounterMeasureOn = FALSE;

        //
        // rollback the bad marked peerBssid
        //
       wmi_deleteBadAp_cmd ((wmi_t *)m_pWMI, 0x00/*index*/);
    }
}

void
CAR6KMini::rssiScanTimeout()
{
    static int reScanCounter=255;
    A_UINT16 fgenable, bkenable;
    // Return if WMI is not ready yet
    if (!m_WMIReady)
        return;

    if (!m_Connected)
        return;

    //get stats from target
    if (ar6000_get_target_stats() != A_OK)
        return;


    if (m_Config.bkScanEnable) {
        bkenable = (A_UINT16)m_Config.bkScanPeriod;
    } else {
        bkenable = 0xFFFF;
    }

    if (m_Config.fgScanEnable) {
        fgenable = 0;
    } else {
        fgenable = 0xFFFF;
    }

    //Post the RSSI value relative to the Standard Noise floor value.
    if (RSSI_TO_NDIS(m_RSSI) >= -60)
    {
        m_RSSIlevel = 0;
        reScanCounter = 10;
        A_TIMEOUT_MS(&m_rssiScanTimer, 1000, 0);
    }
    else if ((RSSI_TO_NDIS(m_RSSI) >= -65) && (RSSI_TO_NDIS(m_RSSI) < -60))
    {
        reScanCounter = 10;
        A_TIMEOUT_MS(&m_rssiScanTimer, 1000, 0);
    }
    else if ((RSSI_TO_NDIS(m_RSSI) >= -70) && (RSSI_TO_NDIS(m_RSSI) < -65))
    {
        reScanCounter = 10;
        if ((1 != m_RSSIlevel) && (2 != m_RSSIlevel))
        {
            m_RSSIlevel = 1;
            wmi_bssfilter_cmd ((struct wmi_t *)m_pWMI, PROBED_SSID_FILTER, 0);
            wmi_scanparams_cmd((wmi_t *)m_pWMI,fgenable,0,bkenable,
                                        0,0,20,WMI_SHORTSCANRATIO_DEFAULT, 0, 0, 0);
            wmi_startscan_cmd((wmi_t *)m_pWMI,WMI_LONG_SCAN,TRUE,FALSE,0,100,0,NULL);
        }
        A_TIMEOUT_MS(&m_rssiScanTimer, 1000, 0);
    }
    else if ((RSSI_TO_NDIS(m_RSSI) >= -75) && (RSSI_TO_NDIS(m_RSSI) < -70))
    {
        reScanCounter = 10;
        if ((2 != m_RSSIlevel) && (3 != m_RSSIlevel))
        {
            m_RSSIlevel = 2;
            wmi_startscan_cmd((wmi_t *)m_pWMI,WMI_SHORT_SCAN,TRUE,FALSE,0,100,0,NULL);
        }
        A_TIMEOUT_MS(&m_rssiScanTimer, 500, 0);
    }
    else if ((RSSI_TO_NDIS(m_RSSI) >= -80) && (RSSI_TO_NDIS(m_RSSI) < -75))
    {
        reScanCounter = 10;
        if ((3 != m_RSSIlevel) && (4 != m_RSSIlevel))
        {
            m_RSSIlevel = 3;
            wmi_bssfilter_cmd ((struct wmi_t *)m_pWMI, PROBED_SSID_FILTER, 0);
            wmi_scanparams_cmd((wmi_t *)m_pWMI,fgenable,0,bkenable,
                                        0,0,20,WMI_SHORTSCANRATIO_DEFAULT, 0, 0, 0);
            wmi_startscan_cmd((wmi_t *)m_pWMI,WMI_LONG_SCAN,TRUE,FALSE,0,100,0,NULL);
        }
        A_TIMEOUT_MS(&m_rssiScanTimer, 400, 0);
    }
    else if ((RSSI_TO_NDIS(m_RSSI) >= -85) && (RSSI_TO_NDIS(m_RSSI) < -80))
    {
        if ((4 != m_RSSIlevel) && (5 != m_RSSIlevel))
        {
            reScanCounter = 7;
            m_RSSIlevel = 4;
            wmi_startscan_cmd((wmi_t *)m_pWMI,WMI_SHORT_SCAN,TRUE,FALSE,0,100,0,NULL);
        }
        if ((0 == reScanCounter) && (4 == m_RSSIlevel))
        {
            reScanCounter = 7;
            if (m_roamTblEntryCount > 1)
            {
                wmi_startscan_cmd((wmi_t *)m_pWMI,WMI_SHORT_SCAN,TRUE,FALSE,0,100,0,NULL);
            }
        }
        reScanCounter--;
        A_TIMEOUT_MS(&m_rssiScanTimer, 400, 0);
    }
    else if (RSSI_TO_NDIS(m_RSSI) < -85)
    {
        if (5 != m_RSSIlevel)
        {
            m_RSSIlevel = 5;
            reScanCounter = 10;
            wmi_bssfilter_cmd ((struct wmi_t *)m_pWMI, PROBED_SSID_FILTER, 0);
            wmi_scanparams_cmd((wmi_t *)m_pWMI,fgenable,0,bkenable,
                                        0,0,20,WMI_SHORTSCANRATIO_DEFAULT, 0, 0, 0);
            wmi_startscan_cmd((wmi_t *)m_pWMI,WMI_LONG_SCAN,TRUE,FALSE,0,100,0,NULL);
        }
        if ((0 == reScanCounter) && (5 == m_RSSIlevel))
        {
            reScanCounter = 6;
            if (m_roamTblEntryCount > 1)
            {
                wmi_startscan_cmd((wmi_t *)m_pWMI,WMI_SHORT_SCAN,TRUE,FALSE,0,100,0,NULL);
            }
        }
        reScanCounter--;
        A_TIMEOUT_MS(&m_rssiScanTimer, 400, 0);
    }
    else
    {
        A_TIMEOUT_MS(&m_rssiScanTimer, 1000, 0);
    }

    wmi_get_roam_tbl_cmd((wmi_t *)m_pWMI);

}

void
CAR6KMini::WMIRoamTbleUpdate(A_UINT32 roamTblEntryCount)
{
    m_roamTblEntryCount = roamTblEntryCount;
}

void
CAR6KMini::WMITargetStatsEvent(
    WMI_TARGET_STATS *pTargetStats)
{
 if(m_networkType == AP_NETWORK)
    {
            return;
    }
    int ac = 0;

    // Update the RSSI for now.
    if (m_Connected) {
        bss_t *pWmiNode = NULL;

        pWmiNode = wmi_find_node((wmi_t *)m_pWMI,m_PeerBSSID);
        if (pWmiNode)
        {
            pWmiNode->ni_rssi = pTargetStats->cservStats.cs_aveBeacon_rssi;
            wmi_node_return((wmi_t *)m_pWMI, pWmiNode);
        }
        m_RSSI = pTargetStats->cservStats.cs_aveBeacon_rssi;
        //
        // Check whether the current mode is adhoc mode (in case of single node Target returns
        // -95 or -96 RSSI values) since there is no specific event from
        // taregt to indicate whenever joiner is available,
        // host has to check below condition in order to avoid
        // null RSSI
        //
        if ((Ndis802_11IBSS == m_InfrastructureMode) && (RSSI_TO_ABS(m_RSSI) <= 0))
        {
            m_RSSI = 0; // Max RSSI value
        }
    }

    // Update other target Stats.
    if (pTargetStats->txrxStats.tx_stats.tx_packets > pTargetStats->txrxStats.tx_stats.tx_failed_cnt)
    {
        m_tgtStats.TransmittedFragmentCount.QuadPart += (pTargetStats->txrxStats.tx_stats.tx_packets -
                                                         pTargetStats->txrxStats.tx_stats.tx_failed_cnt);
    }
    else
    {
        m_tgtStats.TransmittedFragmentCount.QuadPart += 0;
    }

    m_tgtStats.MulticastTransmittedFrameCount.QuadPart   += (pTargetStats->txrxStats.tx_stats.tx_multicast_pkts + \
                                                             pTargetStats->txrxStats.tx_stats.tx_broadcast_pkts);

    m_tgtStats.FailedCount.QuadPart     += pTargetStats->txrxStats.tx_stats.tx_retry_cnt;
    m_tgtStats.RetryCount.QuadPart      += pTargetStats->txrxStats.tx_stats.tx_mult_retry_cnt;
    m_tgtStats.MultipleRetryCount.QuadPart      += pTargetStats->txrxStats.tx_stats.tx_mult_retry_cnt;

    m_tgtStats.RTSSuccessCount.QuadPart += pTargetStats->txrxStats.tx_stats.tx_rts_success_cnt;
    m_tgtStats.RTSFailureCount.QuadPart += pTargetStats->txrxStats.tx_stats.tx_rts_fail_cnt;

    m_tgtStats.FrameDuplicateCount.QuadPart         += pTargetStats->txrxStats.rx_stats.rx_duplicate_frames;
    m_tgtStats.ReceivedFragmentCount.QuadPart       += pTargetStats->txrxStats.rx_stats.rx_packets;
    m_tgtStats.MulticastReceivedFrameCount.QuadPart += (pTargetStats->txrxStats.rx_stats.rx_multicast_pkts + \
                                pTargetStats->txrxStats.rx_stats.rx_broadcast_pkts);

    m_tgtStats.FCSErrorCount.QuadPart += pTargetStats->txrxStats.rx_stats.rx_crcerr;

    m_txRate = wmi_get_rate (pTargetStats->txrxStats.tx_stats.tx_unicast_rate);
    m_rxRate = wmi_get_rate (pTargetStats->txrxStats.rx_stats.rx_unicast_rate);
#ifdef OS_ROAM_MANAGEMENT

    m_RateInfo.TxRateFiltered   = m_txRate;
    m_RateInfo.RxRateFiltered   = m_rxRate;
    m_RateInfo.TxDataFrames    += pTargetStats->txrxStats.tx_stats.tx_unicast_pkts;
    m_RateInfo.RxDataFrames    += pTargetStats->txrxStats.rx_stats.rx_unicast_pkts;
    m_RateInfo.RssiFiltered     = RSSI_TO_NDIS(pTargetStats->cservStats.cs_aveBeacon_rssi);

    if (m_Connected && (0 == pTargetStats->cservStats.cs_bmiss_cnt))
    {
        m_RateInfo.LastRssiBeaconTime = A_MS_TICKGET();
    }
#endif
    /* RX/TX Stats */
    m_tgtAllStats.tx_packets          += pTargetStats->txrxStats.tx_stats.tx_packets;
    m_tgtAllStats.tx_bytes            += pTargetStats->txrxStats.tx_stats.tx_bytes;
    m_tgtAllStats.tx_unicast_pkts     += pTargetStats->txrxStats.tx_stats.tx_unicast_pkts;
    m_tgtAllStats.tx_unicast_bytes    += pTargetStats->txrxStats.tx_stats.tx_unicast_bytes;
    m_tgtAllStats.tx_multicast_pkts   += pTargetStats->txrxStats.tx_stats.tx_multicast_pkts;
    m_tgtAllStats.tx_multicast_bytes  += pTargetStats->txrxStats.tx_stats.tx_multicast_bytes;
    m_tgtAllStats.tx_broadcast_pkts   += pTargetStats->txrxStats.tx_stats.tx_broadcast_pkts;
    m_tgtAllStats.tx_broadcast_bytes  += pTargetStats->txrxStats.tx_stats.tx_broadcast_bytes;
    m_tgtAllStats.tx_rts_success_cnt  += pTargetStats->txrxStats.tx_stats.tx_rts_success_cnt;

    for (ac = 0; ac < WMM_NUM_AC; ac++)
        m_tgtAllStats.tx_packet_per_ac[ac] += pTargetStats->txrxStats.tx_stats.tx_packet_per_ac[ac];

    m_tgtAllStats.tx_errors           += pTargetStats->txrxStats.tx_stats.tx_errors;
    m_tgtAllStats.tx_failed_cnt       += pTargetStats->txrxStats.tx_stats.tx_failed_cnt;
    m_tgtAllStats.tx_retry_cnt        += pTargetStats->txrxStats.tx_stats.tx_retry_cnt;
    m_tgtAllStats.tx_mult_retry_cnt   += pTargetStats->txrxStats.tx_stats.tx_mult_retry_cnt;
    m_tgtAllStats.tx_rts_fail_cnt     += pTargetStats->txrxStats.tx_stats.tx_rts_fail_cnt;
    m_tgtAllStats.tx_unicast_rate      = wmi_get_rate(pTargetStats->txrxStats.tx_stats.tx_unicast_rate);

    m_tgtAllStats.rx_packets          += pTargetStats->txrxStats.rx_stats.rx_packets;
    m_tgtAllStats.rx_bytes            += pTargetStats->txrxStats.rx_stats.rx_bytes;
    m_tgtAllStats.rx_unicast_pkts     += pTargetStats->txrxStats.rx_stats.rx_unicast_pkts;
    m_tgtAllStats.rx_unicast_bytes    += pTargetStats->txrxStats.rx_stats.rx_unicast_bytes;
    m_tgtAllStats.rx_multicast_pkts   += pTargetStats->txrxStats.rx_stats.rx_multicast_pkts;
    m_tgtAllStats.rx_multicast_bytes  += pTargetStats->txrxStats.rx_stats.rx_multicast_bytes;
    m_tgtAllStats.rx_broadcast_pkts   += pTargetStats->txrxStats.rx_stats.rx_broadcast_pkts;
    m_tgtAllStats.rx_broadcast_bytes  += pTargetStats->txrxStats.rx_stats.rx_broadcast_bytes;
    m_tgtAllStats.rx_fragment_pkt     += pTargetStats->txrxStats.rx_stats.rx_fragment_pkt;
    m_tgtAllStats.rx_errors           += pTargetStats->txrxStats.rx_stats.rx_errors;
    m_tgtAllStats.rx_crcerr           += pTargetStats->txrxStats.rx_stats.rx_crcerr;
    m_tgtAllStats.rx_key_cache_miss   += pTargetStats->txrxStats.rx_stats.rx_key_cache_miss;
    m_tgtAllStats.rx_decrypt_err      += pTargetStats->txrxStats.rx_stats.rx_decrypt_err;
    m_tgtAllStats.rx_duplicate_frames += pTargetStats->txrxStats.rx_stats.rx_duplicate_frames;
    m_tgtAllStats.rx_unicast_rate      = wmi_get_rate(pTargetStats->txrxStats.rx_stats.rx_unicast_rate);


    /* Cserv Stats */
    m_tgtAllStats.cs_bmiss_cnt += pTargetStats->cservStats.cs_bmiss_cnt;
    m_tgtAllStats.cs_lowRssi_cnt += pTargetStats->cservStats.cs_lowRssi_cnt;
    m_tgtAllStats.cs_connect_cnt += pTargetStats->cservStats.cs_connect_cnt;
    m_tgtAllStats.cs_disconnect_cnt += pTargetStats->cservStats.cs_disconnect_cnt;

    m_tgtAllStats.cs_aveBeacon_snr = pTargetStats->cservStats.cs_aveBeacon_snr;
    m_tgtAllStats.cs_aveBeacon_rssi = pTargetStats->cservStats.cs_aveBeacon_rssi;
    m_tgtAllStats.cs_lastRoam_msec = pTargetStats->cservStats.cs_lastRoam_msec;
    m_tgtAllStats.cs_snr = pTargetStats->cservStats.cs_snr;
    m_tgtAllStats.cs_rssi = pTargetStats->cservStats.cs_rssi;


    /* WoW Stats */
    m_tgtAllStats.wow_num_pkts_dropped += pTargetStats->wowStats.wow_num_pkts_dropped;
    m_tgtAllStats.wow_num_host_pkt_wakeups += pTargetStats->wowStats.wow_num_host_pkt_wakeups;
    m_tgtAllStats.wow_num_host_event_wakeups += pTargetStats->wowStats.wow_num_host_event_wakeups;
    m_tgtAllStats.wow_num_events_discarded  += pTargetStats->wowStats.wow_num_events_discarded;


    /* TKIP,CCMP  Stats */
    m_tgtAllStats.tkip_local_mic_failure += pTargetStats->txrxStats.tkipCcmpStats.tkip_local_mic_failure;
    m_tgtAllStats.tkip_counter_measures_invoked += pTargetStats->txrxStats.tkipCcmpStats.tkip_counter_measures_invoked;
    m_tgtAllStats.tkip_replays += pTargetStats->txrxStats.tkipCcmpStats.tkip_replays;
    m_tgtAllStats.tkip_format_errors += pTargetStats->txrxStats.tkipCcmpStats.tkip_format_errors;
    m_tgtAllStats.ccmp_format_errors += pTargetStats->txrxStats.tkipCcmpStats.ccmp_format_errors;
    m_tgtAllStats.ccmp_replays += pTargetStats->txrxStats.tkipCcmpStats.ccmp_replays;

    /* misc stats */
    m_tgtAllStats.power_save_failure_cnt += pTargetStats->pmStats.power_save_failure_cnt;
    m_tgtAllStats.noise_floor_calibation = pTargetStats->noise_floor_calibation;
    m_tgtAllStats.lqVal              = pTargetStats->lqVal;

    //post targetStats event
    NdisSetEvent(&m_tgtStatsEvent);

    return;
}


void
CAR6KMini::WMIRssiThresholdEvent(
    A_UINT16 rssi)
{
    NDIS_802_11_RSSI rssi_level = 0;

    rssi_level = ABS_TO_RSSI (rssi);

    //
    // save the current RSSI level as rssiTriggering Event
    //
    m_rssiEvent = (A_INT16)rssi_level;

    NdisMIndicateStatus(m_MiniportAdapterHandle,
                        NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
                        &rssi_level, sizeof (NDIS_802_11_RSSI));

    NdisMIndicateStatusComplete (m_MiniportAdapterHandle);
}


void
CAR6KMini::WMIWowListEvent(
    WMI_GET_WOW_LIST_REPLY *wowList)
{

        /* Alocate for the global ptr, copy data &  post wowListReplyGot event. The  function that
         * calls wmi_get_wow_list should free this.
     */

        /* if there are no filters, allocate for WMI_GET_WOW_LIST_REPLY & copy, set event & return
          */

           if(m_networkType == AP_NETWORK)
            {
                return;
            }
        if ( wowList->num_filters == 0) {

            m_WowListReply = (WMI_GET_WOW_LIST_REPLY *) A_MALLOC(sizeof(WMI_GET_WOW_LIST_REPLY));
            if (!m_WowListReply) {
                NdisSetEvent(&m_tgtWowListReplyEvent);
                return;
            }
            A_MEMZERO((A_UINT8 *)m_WowListReply, sizeof(WMI_GET_WOW_LIST_REPLY));
            A_MEMCPY((A_UINT8 *)m_WowListReply, (A_UINT8 *) wowList, sizeof(WMI_GET_WOW_LIST_REPLY));

            NdisSetEvent(&m_tgtWowListReplyEvent);
            return;
        }

        if ( wowList->this_filter_num == 1 ) {

        if ( wowList->num_filters > 1) {

            m_WowListReply = (WMI_GET_WOW_LIST_REPLY *) A_MALLOC(sizeof(WMI_GET_WOW_LIST_REPLY) + sizeof(WOW_FILTER) * (wowList->num_filters-1));
            if (!m_WowListReply) {
                NdisSetEvent(&m_tgtWowListReplyEvent);
                return;
            }
            A_MEMZERO((A_UINT8 *)m_WowListReply, sizeof(WMI_GET_WOW_LIST_REPLY) + sizeof(WOW_FILTER) * (wowList->num_filters-1));

        } else {

           m_WowListReply = (WMI_GET_WOW_LIST_REPLY *) A_MALLOC(sizeof(WMI_GET_WOW_LIST_REPLY));
           if (!m_WowListReply) {
               NdisSetEvent(&m_tgtWowListReplyEvent);
               return;
           }
           A_MEMZERO((A_UINT8 *)m_WowListReply,sizeof(WMI_GET_WOW_LIST_REPLY));

        }

        A_MEMCPY((A_UINT8 *)m_WowListReply, (A_UINT8 *) wowList, sizeof(WMI_GET_WOW_LIST_REPLY));

        } else { // 2nd filter onwards

        A_MEMCPY((A_UINT8 *)m_WowListReply+sizeof(WMI_GET_WOW_LIST_REPLY)+(wowList->this_filter_num-2) *sizeof(WOW_FILTER), (A_UINT8 *) wowList->wow_filters, sizeof(WOW_FILTER));


        } //endif


        // Set the event only after we get all the filter events.
        if ( wowList->this_filter_num == wowList->num_filters ) {
            NdisSetEvent(&m_tgtWowListReplyEvent);
        }

    return;
}

void
CAR6KMini::WMIErrorEvent(
    WMI_TARGET_ERROR_VAL errorVal)
{
    //
    // reset the error flag and inform target to re-issue the error
    // report event
    //
    wmi_set_error_report_bitmask ((wmi_t *)m_pWMI, AR6K_ERROR_REPORT_MASK);
}

extern "C" void
ar6000_cmEnterTimeout(
    A_ATH_TIMER Context)
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;
    Adapter->counterMeasureEnterTimeout();
}

extern "C" void
ar6000_cmExitTimeout(
    A_ATH_TIMER Context)
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;
    Adapter->counterMeasureExitTimeout();
}

extern "C" void
ar6000_cmRssiScanTimeout(
    A_ATH_TIMER Context)
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;
    Adapter->rssiScanTimeout();
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
    A_UINT8 phyCap,
    A_UINT32 vers)
//
//  This function is called by WMI when the device sends
//  a WMI_READY packet. In addition to letting us know
//  that the device is ready to receive WMI requests,
//  it contains the MAC address of the device.
//
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

    Adapter->WMIReadyIndication(macAddr, phyCap, vers);
}

extern "C" void
ar6000_connect_event(
    void     *Context,
    A_UINT16  channel,
    A_UINT8  *bssid,
    A_UINT16  listenInterval,
    A_UINT16  beaconInterval,
    NETWORK_TYPE networkType,
    A_UINT8   beaconIeLen,
    A_UINT8   assocReqLen,
    A_UINT8   assocRespLen,
    A_UINT8  *assocInfo)
//
//  This function is called by WMI after receiving a
//  WMI_CONNECT event packet from the device.
//
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;
    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Connect EVENT from target\n");

    if(Adapter->m_networkType == AP_NETWORK)
    {
        Adapter->car6k_ap_connect_event (channel,bssid,assocInfo,assocRespLen);
        return;
    }

    Adapter->WMIConnectIndication(channel, bssid, listenInterval,
                                  assocReqLen, assocRespLen,
                                          assocInfo, beaconIeLen,
                      beaconInterval,networkType);
}

extern "C" void
ar6000_disconnect_event(
    void *Context,
    A_UINT8 reason,
    A_UINT8 *bssid,
    A_UINT8 assocRespLen,
    A_UINT8 *assocInfo,
    A_UINT16 protocolReason )
//
//  This function is called by WMI after receiving a
//  WMI_DISCONNECT event packet from the device.
//
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "DisConnect EVENT from target\n");

    if(Adapter->m_networkType == AP_NETWORK)
    {
        Adapter->car6k_ap_disconnect_event (reason, bssid);
        return;
    }
    Adapter->WMIDisconnectIndication(reason);
}

extern "C" A_STATUS
ar6000_control_tx(
    void     *Context,
    void     *osbuf,
    HTC_ENDPOINT_ID     Endpoint)
//
//  This function is called by WMI to send a control
//  packet to the device.
//
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

    return Adapter->WMISendControlPacket(osbuf, Endpoint);
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
    CAR6KMini *Adapter = (CAR6KMini *)context;

#ifdef OS_ROAM_MANAGEMENT
    Adapter->WMIScanCompleteEvent();
#endif
    return;
}

extern "C" void
ar6000_targetStats_event(
    void *context,
    WMI_TARGET_STATS *pStats)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;

    Adapter->WMITargetStatsEvent(pStats);

    return;
}

extern "C" void
ar6000_rssiThreshold_event(
    void *context,
    WMI_RSSI_THRESHOLD_VAL range,
    A_UINT16 rssi)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;

    Adapter->WMIRssiThresholdEvent (rssi);

    return;
}

extern "C" void
ar6000_reportError_event(
    void *context,
    WMI_TARGET_ERROR_VAL errorVal)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;

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
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "%s\n", "WMI_TARGET_PM_ERR_FAIL" );
            break;
        case WMI_TARGET_KEY_NOT_FOUND:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "%s\n", "WMI_TARGET_KEY_NOT_FOUND" );
            break;
        case WMI_TARGET_DECRYPTION_ERR:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "%s\n", "WMI_TARGET_DECRYPTION_ERR");
            break;
        case WMI_TARGET_BMISS:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "%s\n", "WMI_TARGET_BMISS");
            break;
        case WMI_PSDISABLE_NODE_JOIN:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "%s\n", "WMI_PSDISABLE_NODE_JOIN");
            break;
        case WMI_TARGET_COM_ERR:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "%s\n", "WMI_TARGET_COM_ERR");
            break;
        case WMI_TARGET_FATAL_ERR:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "%s\n", "WMI_TARGET_FATAL_ERR");
            break;
        default:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR, "INVALID\n");
            break;
    }

    if (Adapter)
    {
        Adapter->WMIErrorEvent (errorVal);
    }
}

extern "C" void
ar6000_gpio_intr_rx(
    A_UINT32 intr_mask,
    A_UINT32 input_values)
{
    return;
}

extern "C" void
ar6000_gpio_data_rx(
    A_UINT32 reg_id,
    A_UINT32 value)
{
    return;
}

extern "C" void
ar6000_gpio_ack_rx(void)
{
    return;
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
a_copy_from_user(
    void *to,
    const void *from,
    A_UINT32 n)
{
    NdisMoveMemory(to, from, n);
    return 0;
}

extern "C" void
ar6000_roam_tbl_event(
    void *context,
    WMI_TARGET_ROAM_TBL *roamTbl )
{
#if 0
    A_UINT8 i;
#endif

    CAR6KMini *Adapter = (CAR6KMini *)context;

    Adapter->WMIRoamTbleUpdate(roamTbl->numEntries);

#if 0
    RETAILMSG (1,(L"ROAM TABLE NO OF ENTRIES is %d ROAM MODE is %d\n",
              roamTbl->numEntries, roamTbl->roamMode));

    for (i= 0; i < roamTbl->numEntries; i++) {
        RETAILMSG (1,(L"[%d]bssid %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x ", i,
            roamTbl->bssRoamInfo[i].bssid[0], roamTbl->bssRoamInfo[i].bssid[1],
            roamTbl->bssRoamInfo[i].bssid[2],
            roamTbl->bssRoamInfo[i].bssid[3],
            roamTbl->bssRoamInfo[i].bssid[4],
            roamTbl->bssRoamInfo[i].bssid[5]));
        RETAILMSG (1,(L"RSSI %d RSSIDT %d LAST RSSI %d UTIL %d ROAM_UTIL %d BIAS %d\n",
            roamTbl->bssRoamInfo[i].rssi,
            roamTbl->bssRoamInfo[i].rssidt,
            roamTbl->bssRoamInfo[i].last_rssi,
            roamTbl->bssRoamInfo[i].util,
            roamTbl->bssRoamInfo[i].roam_util,
            roamTbl->bssRoamInfo[i].bias));
    }
#endif

}

extern "C" void
ar6000_roam_data_event(
    void *context,
    WMI_TARGET_ROAM_DATA *roamDat )
{
}

extern "C" void
ar6000_cac_event(
    void *context,
    A_UINT8 ac,
    A_UINT8 cac_ind,
    A_UINT8 st,
    A_UINT8 *tspecSuggestion )
{
}

extern "C" void
ar6000_ratemask_rx(
    void *context,
    A_UINT16 ratemask)
{
}

extern "C" void
ar6000_hbChallengeResp_event(
    void *context,
    A_UINT32 cookie,
    A_UINT32 source)
{
}

extern "C" void
ar6000_tx_retry_err_event(
    void *context)
{
}

extern "C" void
ar6000_snrThresholdEvent_rx(
    void *context,
    WMI_SNR_THRESHOLD_VAL newThreshold,
    A_UINT8 snr)
{
}

extern "C" void
ar6000_lqThresholdEvent_rx(
    void *context,
    WMI_LQ_THRESHOLD_VAL newThreshold,
    A_UINT8 lq)
{
}

extern "C" void
ar6000_keepalive_event_rx(
    void *context,
    A_UINT8 keepalive )
{
}
extern "C" void
ar6000_wow_list_event(
    void *context,
    A_UINT16 num_filters,
    WMI_GET_WOW_LIST_REPLY *wow_reply)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;

    Adapter->WMIWowListEvent(wow_reply);

    return;
}

#ifdef CONFIG_HOST_TCMD_SUPPORT
void
CAR6KMini::WMITcmdReportEvent(
    TCMD_CONT_RX* rep)
{
    m_Tcmd_totalPkt = rep->u.report.totalPkt;
    m_Tcmd_rssiInDBm = rep->u.report.rssiInDBm;
    m_Tcmd_crcError = rep->u.report.crcErrPkt;
    m_Tcmd_secErr = rep->u.report.secErrPkt;
    NdisSetEvent(&m_TcmdReportEvent);
        return;
}

extern "C" void
ar6000_tcmd_rx_report_event(
    void *context,
    A_UINT8* datap)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;

    Adapter->WMITcmdReportEvent((TCMD_CONT_RX*)datap);
    return;
}
#endif // TCMD

extern "C" A_STATUS
ar6000_get_driver_cfg(
    void *context,
    A_UINT16 cfgParam,
    void *result)
{
    A_STATUS  ret = A_OK;

    switch(cfgParam) {
        case AR6000_DRIVER_CFG_GET_WLANNODECACHING:
           *((A_UINT32 *)result) = 1; /* possible to get via registry? */
           break;
        case AR6000_DRIVER_CFG_LOG_RAW_WMI_MSGS:
           *((A_UINT32 *)result) = 0; /* possible to get via registry? */
            break;
        default:
            WMI_DEBUG_PRINTF(ATH_LOG_ERR,
                     "ar6000_get_driver_cfg ; unhandled CFG :0x%x\n", cfgParam);
            ret = A_EINVAL;
            break;
    }

    return ret;
}

extern "C" void
ar6000_keepalive_rx(
    void *devt,
    A_UINT8 configured)
{
    CAR6KMini *Adapter = (CAR6KMini *)devt;
    if(Adapter)
    {
        Adapter->WMIKeepAliveEvent(configured);
    }
    return;
}

/* indicate tx activity or inactivity on a WMI stream */
extern "C"
void ar6000_indicate_tx_activity(
    void *devt,
    A_UINT8 TrafficClass,
    A_BOOL Active)
{
    CAR6KMini *Adapter = (CAR6KMini *)devt;

        /* notify HTC, this may cause credit distribution changes */
    HTCIndicateActivityChange(Adapter->GetHTCHandle(),
                              Adapter->arAc2EndpointID(TrafficClass),
                              Active);
}

extern "C" void
ar6000_bssInfo_event_rx(
    void *ar,
    A_UINT8 *datap,
    int len)
{
}

extern "C" HTC_ENDPOINT_ID
ar6000_ac2_endpoint_id (
    void * devt,
    A_UINT8 ac)
{
    CAR6KMini *Adapter = (CAR6KMini *)devt;
    return (Adapter->arAc2EndpointID(ac));
}

extern "C" A_UINT8
ar6000_endpoint_id2_ac(
    void * devt,
    HTC_ENDPOINT_ID ep )
{
    CAR6KMini *Adapter = (CAR6KMini *)devt;
    return (Adapter->arEndpoint2Ac(ep));
}

extern "C" void
ar6000_pmkid_list_event(
    void *devt,
    A_UINT8 numPMKID,
    WMI_PMKID *pmkidList)
{
    A_UINT8 i, j;

    for (i = 0; i < numPMKID; i++) {
        for (j = 0; j < WMI_PMKID_LEN; j++) {
            NDIS_DEBUG_PRINTF(ATH_DEBUG_WLAN, "%2.2x", pmkidList->pmkid[j]);
        }
        pmkidList++;
    }

}

extern "C" void
ar6000_peer_event (void *Context,
                        A_UINT8 eventCode,
                        A_UINT8 *bssid)
{
   CAR6KMini *Adapter = (CAR6KMini *)Context;
   Adapter->WMIPeerEvent(eventCode);
   return;
}

extern "C" void
ar6000_channel_change_event(
    void *dvt,
    A_UINT16 oldChannel,
    A_UINT16 newChannel)
{
    NDIS_DEBUG_PRINTF(ATH_DEBUG_WLAN, "Channel Change notification\nOld Channel: %d, New Channel: %d\n",
             oldChannel, newChannel);
}

#ifdef OS_ROAM_MANAGEMENT
void
CAR6KMini::disconnectIndicationTimeout()
{
    NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
    NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
}

void
CAR6KMini::WMIPeerEvent(A_UINT8 eventCode)
{
   if(PEER_FIRST_NODE_JOIN_EVENT == eventCode){
       m_Connected         = TRUE;
       m_ConnectInProgress = FALSE;
       NdisMIndicateStatus (m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_CONNECT, 0, 0);
       NdisMIndicateStatusComplete (m_MiniportAdapterHandle);
   }

   if(PEER_LAST_NODE_LEAVE_EVENT == eventCode){
       NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
       NdisMIndicateStatusComplete(m_MiniportAdapterHandle);

   }

}

void
CAR6KMini::WMIScanCompleteEvent()
{
    A_UINT16 fgenable, bkenable;

    if (m_Config.hostAssistedRoaming)
    {
        wmi_bssfilter_cmd ((struct wmi_t *)m_pWMI, m_WMIBssFilter, 0);
            // Restore scan parameters
            if (m_Config.bkScanEnable) {
                bkenable = (A_UINT16)m_Config.bkScanPeriod;
            } else {
                bkenable = 0xFFFF;
            }

            if (m_Config.fgScanEnable) {
                fgenable = 0;
            } else {
                fgenable = 0xFFFF;
            }
            wmi_scanparams_cmd((wmi_t *)m_pWMI,fgenable,0,bkenable,
                               0,0,0,WMI_SHORTSCANRATIO_DEFAULT, 0, 0, 0);
    }
#ifdef OS_ROAM_MANAGEMENT
    if ((m_ChannelScan) && (!m_osRoamControl))
#else
    if (m_ChannelScan)
#endif
    {

        //
        // Restore channel parameters
        //
        wmi_set_channelParams_cmd ((wmi_t *)m_pWMI, TRUE, (WMI_PHY_MODE)m_80211PhyCapability, 0, NULL);

        if (m_Connected)
        {
            // Restore scan parameters
            if (m_Config.bkScanEnable) {
                bkenable = (A_UINT16)m_Config.bkScanPeriod;
            } else {
                bkenable = 0xFFFF;
            }

            if (m_Config.fgScanEnable) {
                fgenable = 0;
            } else {
                fgenable = 0xFFFF;
            }
            wmi_scanparams_cmd((wmi_t *)m_pWMI,fgenable,0,bkenable,
                                0,0,0,WMI_SHORTSCANRATIO_DEFAULT, 0, 0, 0);
        }

        m_ChannelScan = FALSE;
    }
#ifdef OS_ROAM_MANAGEMENT
    if (m_osRoamControl)
    {
        wmi_scan_indication ((wmi_t *)m_pWMI);
    }
#endif
}

void
CAR6KMini::WMIScanIndication (void *si, ULONG len)
{
    NdisMIndicateStatus (m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_SPECIFIC_INDICATION, si, len);
    NdisMIndicateStatusComplete (m_MiniportAdapterHandle);
}

extern "C" void
ar6000_scan_indication(
       void *context, void *si, A_UINT32 len)
{
    CAR6KMini *Adapter = (CAR6KMini *)context;
    Adapter->WMIScanIndication(si, len);
}

#endif //OS_ROAM_MANAGEMENT

void
CAR6KMini::IterateNodeAndRemoveSSID (IN NDIS_802_11_SSID *SSID)
{
    bss_t *pWmiSsidnode = NULL;

    do
    {
        //
        // Find the nodes based on SSID and remove it
        // NOTE :: This case will not work out for Hidden-SSID
        //
        pWmiSsidnode = wmi_find_Ssidnode ((wmi_t *)m_pWMI, SSID->Ssid,
                                          SSID->SsidLength, FALSE, TRUE);

        if (pWmiSsidnode)
        {
            wmi_free_node ((wmi_t *)m_pWMI, pWmiSsidnode->ni_macaddr);
        }

    }while (pWmiSsidnode);

}

extern "C" void
ar6000_wlanversion_event (void *context, A_UINT32 version)
{
    // nothing to do
}

#ifdef SEND_EVENT_TO_APP

extern "C" void
ar6000_send_event_to_app(void *context, A_UINT16 eventId, A_UINT8 *datap, int len)
{
    // nothing to do as of now
}

extern "C" void
ar6000_send_generic_event_to_app(void *context, A_UINT16 eventId, A_UINT8 *datap, int len)
{
    // nothing to do as of now
}

#endif /* SEND_EVENT_TO_APP */




