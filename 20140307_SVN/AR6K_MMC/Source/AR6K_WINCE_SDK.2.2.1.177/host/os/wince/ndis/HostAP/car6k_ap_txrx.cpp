//------------------------------------------------------------------------------
// <copyright file="car6k_ap_txrx.cpp" company="Atheros">
//    Copyright (c) 2008-2009 Atheros Corporation.  All rights reserved.
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
//
// Author(s): ="Atheros"
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
#include "crypto_api.h"
#include "osapi_wince.h"
#include "wince_common_drv.h"
#include "car6k_ap.hpp"

void
CAR6KMini::
car6k_ap_rx_pwsave_flushpkt (MAC_ADDRESS *pSrcAddr, A_UINT16 powState)
{
    LIST_ENTRY  *pNode;
    NDIS_PACKET *pPacket;
    A_UINT8 prePowState = 0;
    STATION *Station = NULL;

    Station = GetStation ((A_UINT8 *)pSrcAddr,0);
    if(Station) {
        prePowState = STA_IS_PWR_SLEEP(Station);
        if(powState){
            STA_SET_PWR_SLEEP(Station);
        }else{
            STA_CLR_PWR_SLEEP(Station);
        }

        if (prePowState ^ STA_IS_PWR_SLEEP(Station)) {
            if (!STA_IS_PWR_SLEEP(Station)) {
                while (!IsListEmpty(&Station->ucastq)) {
                    pNode = RemoveHeadList(&Station->ucastq);
                    m_ucastq_cnt --;
                    pPacket = (PNDIS_PACKET) ((PBYTE)pNode - offsetof(NDIS_PACKET,MiniportReserved));
                    SendPackets(&pPacket,1);
                }
                /* Clear the PVB for this STA */
                wmi_set_pvb_cmd((wmi_t *)m_pWMI, Station->aid, 0);
            }
        }
    }else{
        RETAILMSG (1, (L" Packet from Non-Assoc Station \t"));
    }

}


NDIS_STATUS
CAR6KMini::
car6k_process_ap_rx (UINT32 *magic, PBYTE pData, ULONG cbData, BOOL* bisAPUnicast,
                     MAC_ADDRESS *pDestAddr, NDIS_PACKET *pPacket)
{
    NDIS_STATUS      Status;
    NDIS_PACKET     *pAirPacket;
    NDIS_BUFFER     *pAirBuffer;
    STATION         *Station;
    if((isGrp(pDestAddr)))
    {
        NdisAllocatePacket(&Status, &pAirPacket, m_TxPacketPool);
        magic = (UINT32 *) pAirPacket->ProtocolReserved;
        if (NDIS_STATUS_SUCCESS != Status)
        {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - NdisAllocatePacket failed\n");
            return NDIS_STATUS_FAILURE;
        }

        NdisAllocateBuffer(&Status, &pAirBuffer,m_TxBufferPool,
                pData, cbData);
        if (NDIS_STATUS_SUCCESS != Status)
        {
            NdisFreePacket(pAirPacket);
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - NdisAllocateBuffer failed\n");
            return NDIS_STATUS_FAILURE;
        }
        pAirBuffer->Next = NULL;
        NdisChainBufferAtFront(pAirPacket,pAirBuffer);
        if(isStationSleep()){
            *(magic) = TAG_MCAST_PCK_PS;
        }else{
            *(magic) = TAG_MCAST_PCK;
        }
        SendPackets(&pAirPacket,1);
    }else{
        Station = GetStation((A_UINT8 *)pDestAddr,0);
        if(Station)
        {
            *bisAPUnicast = TRUE;
            *(magic) = TAG_UCAST_PCK;
            SendPackets(&pPacket,1);
        }
    }
    return NDIS_STATUS_SUCCESS;
}


void
CAR6KMini::
car6k_ap_rx_process_EAPOL (BYTE *stationaddr, A_UINT8 *buf)
{
   STATION *station = GetStation ((UINT8 *)stationaddr,0);

    if (station != NULL)
    {
        car6k_statetrans (station);
    }

    if(station -> m_keyState == WPA_HNDSHK_MSG_2)
    {
        memcpy (station->SNonce, (buf + 31), 32);
        car6k_PTKGenerate (station, m_crypt_type);
    }

    EAPOL_HEADER *epHdr;
    EAPOL_KEY_WPA *epKeyWpa;
    EAPOL_KEY_INFO epKeyInfo;
    A_UINT8  digest[20];
    A_UINT8  thisMIC[16];
    A_UINT16 eapolLen;
    epHdr = (EAPOL_HEADER *)(buf + 14);
    WMI_AP_SET_MLME_CMD pMlme;
    A_STATUS wmiStatus;
    A_BOOL error = FALSE;

    if (epHdr->type == EAPOL_TYPE_KEYMSG)
    {
       epKeyWpa = (EAPOL_KEY_WPA *)(epHdr + 1);
       if (m_crypt_type == EKI_TYPE_TKIP)
       {
           epKeyInfo.u.word = A_BE2CPU16 (epKeyWpa->keyInfo.u.word);
           if ((epKeyInfo.u.k.mic == EKI_MIC_SET ) && (epKeyInfo.u.k.error == EKI_ERROR_SET) &&
               (epKeyInfo.u.k.request == EKI_REQUEST_SET))
           {
               // Handling MIC Error report frame
               if (station != NULL)
               {
                   WMIMicErrEvent ((A_UINT8)station->aid, FALSE);
               }
           }
        }

       if (station -> m_keyState == WPA_HNDSHK_MSG_2)
        {
            if (car6k_wpaMsgVerify (station, epKeyWpa, (WPA_MSGNUM) 2) != A_OK)
            {
                NDIS_DEBUG_PRINTF (ATH_LOG_INF, "Message verification failed : : MSG 2 \n");
                memcpy (pMlme.mac, station->mac, ETHERNET_MAC_ADDRESS_LENGTH);
                pMlme.reason = WLAN_REASON_INVALID_IE;
                pMlme.cmd = WMI_AP_DEAUTH;
                wmiStatus = wmi_ap_set_mlme ((wmi_t*)m_pWMI, pMlme.cmd, pMlme.mac, pMlme.reason);
                if ( wmiStatus  != A_OK)
                {
                    NDIS_DEBUG_PRINTF (ATH_LOG_INF, "wmi_ap_set_mlme - failed \n");
                }
                replayCtr = 0;
                error = TRUE;
            }
        }

        if (station -> m_keyState == WPA_HNDSHK_MSG_4)
        {
            if (car6k_wpaMsgVerify (station, epKeyWpa,(WPA_MSGNUM) 4) != A_OK)
            {
                NDIS_DEBUG_PRINTF (ATH_LOG_INF, "Message verification failed : : MSG 4 \n");
                memcpy (pMlme.mac, station->mac, ETHERNET_MAC_ADDRESS_LENGTH);
                pMlme.reason = WLAN_REASON_INVALID_IE;
                pMlme.cmd = WMI_AP_DEAUTH;
                wmiStatus = wmi_ap_set_mlme ((wmi_t*)m_pWMI, pMlme.cmd, pMlme.mac, pMlme.reason);
                if ( wmiStatus  != A_OK)
                {
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "wmi_ap_set_mlme - failed \n");
                }
                replayCtr = 0;
                error = TRUE;
            }
        }

        // Now we have our PTK, verify the MIC in the frame

        memcpy (thisMIC, epKeyWpa->keyMIC, sizeof (epKeyWpa->keyMIC));
        memset (epKeyWpa->keyMIC, 0, sizeof (epKeyWpa->keyMIC));

        eapolLen = (sizeof (EAPOL_KEY_WPA) - 1) + A_BE2CPU16 (epKeyWpa->keyDataLength);
        eapolLen += sizeof(*epHdr);

        if (epKeyWpa->descType == EAPOL_KEY_TYPE_WPA)
        {
            hmac_md5 ((unsigned char *)epHdr, eapolLen,
                      &(station->PTK[0]), PACKET_MIC_SIZE, digest);
        }
        if (epKeyWpa->descType == EAPOL_KEY_TYPE_WPA2)
        {
            hmac_sha1 ((unsigned char *)epHdr, eapolLen,
                       &(station->PTK[0]), PACKET_MIC_SIZE, digest);
        }

        if (memcmp (digest, thisMIC, PACKET_MIC_SIZE) == 0)
        {
            NDIS_DEBUG_PRINTF (ATH_LOG_INF, "EAPOL Key MIC verified\n");
        }
        else
        {
            NDIS_DEBUG_PRINTF (ATH_LOG_INF, "MIC verification failed \n");
            memcpy (pMlme.mac, station->mac, ETHERNET_MAC_ADDRESS_LENGTH);
            pMlme.reason = WLAN_REASON_PAIRWISE_CIPHER_NOT_VALID;
            pMlme.cmd = WMI_AP_DEAUTH;
            wmiStatus = wmi_ap_set_mlme ((wmi_t*)m_pWMI, pMlme.cmd, pMlme.mac, pMlme.reason);
            if ( wmiStatus  != A_OK)
            {
                NDIS_DEBUG_PRINTF (ATH_LOG_INF, "wmi_ap_set_mlme - failed \n");
            }
            replayCtr = 0;
            error = TRUE;
        }
     }

     if ((station -> m_keyState == WPA_HNDSHK_MSG_2) && (!error))
     {
        car6k_Sending_Msg_3_of_4 (station, m_crypt_type);
     }

     if ((station -> m_keyState == WPA_HNDSHK_MSG_4) && (m_crypt_type == EKI_TYPE_TKIP) && (!error))
     {
         NDIS_DEBUG_PRINTF (ATH_LOG_INF, "Four way Handshake completed :: Initiating group handshake\n");
         car6k_Sending_Group_Msg_1_of_2 (station, m_crypt_type);
     }

}

A_BOOL
CAR6KMini::
car6k_ap_tx_pwsave_qpacket (PNDIS_PACKET pNdisPacket)
{
    STATION *conSta;
    ETHERNET_MAC_HEADER   etherInfo;
    ndis2ether(pNdisPacket,&etherInfo);
    if(IEEE80211_IS_MULTICAST(etherInfo.DestMACAddress) &&
            isStationSleep()){
        if(m_dtimExpire == FALSE){
            BOOL mcastq = FALSE;
            //LOCK
            NdisAcquireSpinLock(&m_mcastqLock);
            if(m_mcastq_cnt > ATH_PWR_MAX_QUE_LEN){
                flushNdisPacketQ(&m_mcastq);
                m_mcastq_cnt = 0; //Resetting the Q
            }
            mcastq = IsListEmpty(&m_mcastq);
            PLIST_ENTRY pEntry = (PLIST_ENTRY)(pNdisPacket->MiniportReserved);
            InsertTailList(&m_mcastq,pEntry);
            m_mcastq_cnt ++;
            if(mcastq){
                wmi_set_pvb_cmd((wmi_t *)m_pWMI,MCAST_AID,1);
            }

            NdisReleaseSpinLock(&m_mcastqLock);
            return FALSE;
        }else{
            NdisAcquireSpinLock(&m_mcastqLock);
            if(!IsListEmpty(&m_mcastq)){
                m_hasMoreData = TRUE;
            }
            NdisReleaseSpinLock(&m_mcastqLock);
        }
    }else{
        conSta = GetStation(etherInfo.DestMACAddress,0);
        if(conSta && STA_IS_PWR_SLEEP(conSta)){
            /* Tx Not from pspoll */
            if(!STA_IS_PS_POLLED(conSta)) {
                BOOL ucastq = FALSE;
                //LOCK
                if(m_ucastq_cnt > ATH_PWR_MAX_QUE_LEN){
                    flushNdisPacketQ(&conSta->ucastq);
                    m_ucastq_cnt = 0;//Resetting the Q
                }
                ucastq = IsListEmpty(&conSta->ucastq);
                PLIST_ENTRY pEntry = (PLIST_ENTRY)      \
                                     (pNdisPacket->MiniportReserved);
                if (((*(UINT32 *)pNdisPacket->ProtocolReserved) == TAG_WPA_SEND_PCK) &&
                conSta->hnd_shk_completed)
                {
                    InsertHeadList(&conSta->ucastq,pEntry);
                }else{
                    InsertTailList(&conSta->ucastq,pEntry);
                }
                m_ucastq_cnt ++;
                /* First Packet */
                if(ucastq){
                    wmi_set_pvb_cmd((wmi_t *)m_pWMI,conSta->aid,1);
                }
                return FALSE;
            }else{
                if(!IsListEmpty(&conSta->ucastq)){
                    m_hasMoreData = TRUE;
                }
            }
        }
    }
    return TRUE;
}







