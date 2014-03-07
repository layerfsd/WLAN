//------------------------------------------------------------------------------
// <copyright file="car6k_ap.cpp" company="Atheros">
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
CAR6KMini::car6k_sendEapolKeyMsg (A_UINT8 descType,
                            A_BOOL secure, A_BOOL mic, A_BOOL ack, A_BOOL tx,
                            A_UINT32 index, A_UINT8 isPW,
                            A_BOOL doEncrypt,
                            A_UINT64 keyRSC,
                            const A_UINT8 nonce[],
                            const A_UINT32 keyLength,
                            A_UINT32 keyDataLen,
                            const A_UINT8 *keyData,
                            const A_UINT32 keyBufferLength,
                            A_UINT8  *bssid)
{
    STATION *station;
    station = GetStation(bssid,0);
    NDIS_STATUS Status;
    PNDIS_PACKET pPacket;
    PNDIS_BUFFER pBuffer;
    A_UINT32 *magic;
    A_UINT16 length;

    memset (m_wpa_buffer, 0 , sizeof (m_wpa_buffer));
    crypto_sendEapolKeyMsg ( descType,secure, mic, ack,  tx,
                             index, isPW,doEncrypt,keyRSC,
                             nonce,keyLength,keyDataLen,
                             keyData, keyBufferLength,
                             bssid,m_CurrentAddress,m_keyCounter,&replayCtr,
                             station,m_wpa_buffer,&length);

    NdisAllocatePacket (&Status, &pPacket, m_TxPacketPool);
    magic = (UINT32 *) pPacket->ProtocolReserved;
    if (NDIS_STATUS_SUCCESS != Status)
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K: ERROR - NdisAllocatePacket failed\n");
        return;
    }

    NdisAllocateBuffer (&Status, &pBuffer, m_TxBufferPool, m_wpa_buffer, length);

    if (NDIS_STATUS_SUCCESS != Status)
    {
        NdisFreePacket (pPacket);
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K: ERROR - NdisAllocateBuffer failed\n");
        return;
    }

    pBuffer->Next = NULL;
    NdisChainBufferAtFront (pPacket,pBuffer);
    *(magic) = TAG_WPA_SEND_PCK;
    SendPackets (&pPacket,1);
    return;
}

void
CAR6KMini::car6k_keyCounterInit(A_UINT8  *bssid)
{
    crypto_keyCounterInit(bssid,m_keyCounter);
    return;
}

void
CAR6KMini::car6k_keyCounterIncr()
{
    crypto_keyCounterIncr (m_keyCounter);
    return;
}

void
CAR6KMini::car6k_ANonceCalc (STATION *station)
{
    crypto_ANonceCalc (station, m_keyCounter);
    return;
}

void
CAR6KMini::car6k_GNonceCalc (A_UINT8  *bssid)
{
    crypto_GNonceCalc (bssid, m_keyCounter, GNonce);
    return;
}

void
CAR6KMini::car6k_GMKGenerate (A_UINT8  *bssid)
{
    crypto_GMKGenerate (bssid, m_keyCounter, &GMKLength, GMK);
    return;
}

void
CAR6KMini::car6k_GTKCalc (A_UINT8 Type, A_UINT8  *bssid, A_UINT16 keyIndex)
{
    crypto_GTKCalc (Type, bssid, keyIndex, m_keyCounter, GNonce, &GTKLength, GTK, GMK, &GMKLength);
    return;
}

void
CAR6KMini::car6k_PTKGenerate (STATION *station,A_UINT8 type)
{
    crypto_PTKGenerate (station, type, m_CurrentAddress, key);
    return;
}

A_STATUS
CAR6KMini::car6k_wpaMsgVerify (STATION *station, EAPOL_KEY_WPA *ekw, WPA_MSGNUM msgNum)
{
    A_STATUS status;
    status = crypto_wpaMsgVerify (station, ekw, msgNum, m_isSecure, m_crypt_type);
    return status;
}

void
CAR6KMini::car6k_statetrans (STATION* station)
{
    crypto_statetrans (station, m_apCmd, m_key_size, GTK, &replayCtr, m_pWMI, m_current_grp_index, m_AP_conn_sta);
    return;
}

void
CAR6KMini::car6k_Sending_Msg_3_of_4 (STATION *station, A_UINT8 type)
{
    // get the WPA(2) IE from the beacon
    A_UINT8 buf[512];
    WPA_IE *wpaIe       = (WPA_IE *)buf;
    WPA2_IE *wpa2Ie     = (WPA2_IE *)buf;
    A_UINT16 gKeyIndex  = m_current_grp_index;
    int gtkAllowedLen, gtkLen;
    A_UINT8 *pBufGtk;

    A_UINT64 txSeqCtr = 0;

    memset (buf,0,sizeof(buf));

    if (type == EKI_TYPE_TKIP)
    {
         station->wpa_ie[1] = ATH_WPA_IE_LEN;
         memcpy (buf, station->wpa_ie, ATH_WPA_IE_LEN + 2);
         car6k_sendEapolKeyMsg (EKI_TYPE_TKIP, FALSE,
                                TRUE, TRUE, TRUE, 0, EKI_KEYTYPE_PAIRWISE,
                                FALSE, 0, station->ANonce, 32,
                                INFO_ELEMENT_SIZE(*wpaIe), (A_UINT8 *)wpaIe,
                                sizeof(buf),station->mac);
         m_isSecure = FALSE ;
    }
    else
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "WPA2: link secure\n");
        station->wpa_ie[1] = ATH_WPA2_IE_LEN;
        memcpy (buf, station->wpa_ie, ATH_WPA2_IE_LEN);

        pBufGtk = buf + INFO_ELEMENT_SIZE (*wpa2Ie);
        gtkAllowedLen = sizeof (buf) - INFO_ELEMENT_SIZE (*wpa2Ie);
        gtkLen = crypto_wpa2GTKInfoElementCreate (pBufGtk, gtkAllowedLen, GTK, m_current_grp_index);

        if (gtkLen == 0)
        {
            NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "Bad gtkLen\n");
            return;
        }

        car6k_sendEapolKeyMsg (EKI_TYPE_AES, TRUE,
                               TRUE, TRUE, TRUE, 0, EKI_KEYTYPE_PAIRWISE,
                               TRUE, txSeqCtr, station->ANonce, 16,
                               INFO_ELEMENT_SIZE(*wpa2Ie) + gtkLen,
                               (A_UINT8 *)wpa2Ie, sizeof(buf),station->mac);
        m_isSecure = TRUE ;

    }

    NDIS_DEBUG_PRINTF (ATH_LOG_INF, "Sent M3\n");
    station -> m_keyState = WPA_HNDSHK_MSG_3;
    return;
}

void
CAR6KMini::car6k_Sending_Group_Msg_1_of_2 (STATION *station, A_UINT8 type)
{
    A_UINT16 gKeyIndex = m_current_grp_index;
    A_UINT16 keyLength;

    if (type == EKI_TYPE_TKIP)
    {
        keyLength = TKIP_KEY_SIZE_BYTES;
    }
    else
    {
        keyLength = AES_KEY_SIZE_BYTES;
    }

    if (type == EKI_TYPE_TKIP)
    {
        car6k_sendEapolKeyMsg (EKI_TYPE_TKIP, TRUE, TRUE,
                               TRUE, FALSE, gKeyIndex,
                               EKI_KEYTYPE_GROUP, TRUE, 0,
                               GNonce, keyLength, GTKLength,
                               GTK[gKeyIndex - 1], sizeof(GTK[gKeyIndex - 1]), station->mac);
    }
    else if (type == EKI_TYPE_AES)
    {
        A_UINT8 buf[512];
        int gtkAllowedLen, gtkLen;
        A_UINT8 *pBufGtk;

        /* Add the GTK info at the end of the key data. */
        pBufGtk = buf;
        gtkAllowedLen = sizeof(buf);
        gtkLen = crypto_wpa2GTKInfoElementCreate (pBufGtk, gtkAllowedLen, GTK, m_current_grp_index);

        if (!gtkLen)
        {
            return;
        }
        car6k_sendEapolKeyMsg (EKI_TYPE_AES,
                         TRUE, TRUE,
                         TRUE, FALSE, 0,
                         EKI_KEYTYPE_GROUP, TRUE, 0,
                         GNonce, keyLength,
                         gtkLen, pBufGtk, sizeof(buf),station->mac);
    }
    else
    {
        return;
    }

    NDIS_DEBUG_PRINTF (ATH_LOG_INF, "Sent G1\n");
    station -> m_keyState = WPA_HNDSHK_MSG_G1;
    replayCtr = 0;
    station ->hnd_shk_completed = TRUE;
    return;
}

void
CAR6KMini::car6k_rekeying_Sending_Group_Msg_1_of_2 ()
{
    A_UINT8 i;
    A_STATUS wmiStatus;

    m_current_grp_index = (m_current_grp_index == GROUPKEY_IDX_A)? GROUPKEY_IDX_B : GROUPKEY_IDX_A;
    car6k_GMKGenerate (m_CurrentAddress);
    car6k_GNonceCalc (m_CurrentAddress);
    car6k_GTKCalc (m_crypt_type,m_CurrentAddress,m_current_grp_index);

    if(m_AP_conn_sta != 0)
    {
        wmiStatus = wmi_addKey_cmd  ((wmi_t *)m_pWMI,
                                     m_current_grp_index,
                                     (CRYPTO_TYPE)m_apCmd.groupCryptoType,
                                     GROUP_USAGE,
                                     m_key_size,
                                     (UINT8 *)&replayCtr,
                                     &GTK[m_current_grp_index - 1][0],KEY_OP_INIT_VAL,NULL,
                                     SYNC_BOTH_WMIFLAG);
         if (wmiStatus != A_OK)
         {
             printf("AR6K: ERROR - wmi_addKey_cmd failed = %u\n", wmiStatus);

         }

        for(i = 0; i < AP_MAX_NUM_STA; i++)
        {
            STATION *station;
            station = GetStation (m_staList[i].mac, 0);
            if (station == NULL)
            {
                continue ;
            }
            if (IS_MAC_NULL (station->mac))
            {
                continue ;
            }
            car6k_Sending_Group_Msg_1_of_2 (station, m_crypt_type);
        }
    }
 }

 STATION *
 CAR6KMini::GetStation(A_UINT8 *sta,A_UINT16 aid)
 {
     STATION *adstation = NULL;
     A_UINT8 i, maxstation;

     switch(m_networkType)
     {
         case AP_NETWORK:
         maxstation = AP_MAX_NUM_STA;
             break;
         default:
         maxstation = 0;
             break;
     }

     for(i =0 ;i < maxstation; i++)
     {
         if(!sta)
         {
             if(m_staList[i].aid == aid)
             {
                 adstation = &m_staList[i];
                 break;
             }
         }
         else
         {
             if(IEEE80211_ADDR_EQ(sta,m_staList[i].mac))
             {
                 adstation = &m_staList[i];
                 break;
             }
         }
     }
     return adstation;
 }


 void
 CAR6KMini::addNewStation(A_UINT8 *mac,A_UINT16 aid,A_UINT8 *wpaie,A_UINT8 ielen)
 {
     A_INT8 free_slot=-1,i;

     for(i=0;i<AP_MAX_NUM_STA;i++)
     {
         if(A_MEMCMP(m_staList[i].mac,mac,6) == 0)
         {
             NDIS_DEBUG_PRINTF(ATH_LOG_TRC,"AR6K: Station Already Available");
             return ; /* Station already available cond. */
         }
         if(!((1 << i) & m_staListIndex))
         {
             free_slot = i;
             break;
         }
     }

     if(free_slot >= 0)
     {
         A_MEMCPY(m_staList[free_slot].mac,mac,6);
         A_MEMCPY(m_staList[free_slot].wpa_ie,wpaie,ielen);
         m_staList[free_slot].aid = aid;
         m_staList[free_slot].hnd_shk_completed = FALSE;
         m_staList[i].m_keyState = 0;
         m_staList[i].PTKlength = 0;
         A_MEMZERO(m_staList[i].ANonce, sizeof (m_staList[i].ANonce));
         A_MEMZERO(m_staList[i].SNonce, sizeof (m_staList[i].SNonce));
         A_MEMZERO(m_staList[i].PTK, sizeof (m_staList[i].PTK));
         m_staListIndex = m_staListIndex | (1 << free_slot);
         InitializeListHead(&m_staList[free_slot].ucastq);
         m_AP_conn_sta ++;
     }
     else
     {
         NDIS_DEBUG_PRINTF(ATH_LOG_TRC,"AR6K: Error Adding New Station");
     }

   if (m_AP_conn_sta == 1)
   {
       NdisMIndicateStatus (m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_CONNECT, 0, 0);
       NdisMIndicateStatusComplete (m_MiniportAdapterHandle);
   }

 }


 void
 CAR6KMini::removeNewStation(A_UINT8 *mac)
 {
     A_INT8 i;
     A_UINT8 sta_mac[6] = {0};

     A_MEMCPY(sta_mac,mac,ETHERNET_MAC_ADDRESS_LENGTH);

     if(IS_MAC_NULL(sta_mac))
     {
         return ;
     }

     for(i=0; i < AP_MAX_NUM_STA; i++)
     {
         if(A_MEMCMP(m_staList[i].mac, sta_mac, 6)==0)
         {
             /* Zero out the state fields */
             A_MEMZERO(m_staList[i].mac, 6);
             A_MEMZERO(m_staList[i].wpa_ie, IEEE80211_MAX_IE);
             m_staList[i].aid = 0;
             m_staList[i].flags = 0;
             m_staList[i].m_keyState = 0;
             m_staList[i].PTKlength = 0;
             m_staList[i].hnd_shk_completed = FALSE;
             A_MEMZERO(m_staList[i].ANonce, sizeof (m_staList[i].ANonce));
             A_MEMZERO(m_staList[i].SNonce, sizeof (m_staList[i].SNonce));
             A_MEMZERO(m_staList[i].PTK, sizeof (m_staList[i].PTK));
             m_staListIndex = m_staListIndex & ~(1 << i);

             flushNdisPacketQ(&(m_staList[i].ucastq));
             m_AP_conn_sta -- ;
         }
     }
 }

 void
 CAR6KMini::ndis2ether(NDIS_PACKET *pPacket,ETHERNET_MAC_HEADER *ethHead)
 {
     PNDIS_BUFFER pBuffer;
     PBYTE pBuffData;
     ULONG cbBuffData;
     ETHERNET_MAC_HEADER *pEthHead;

     NdisQueryPacketFirstBuffer(pPacket,&pBuffer);
     if(NULL == pBuffer)
     {
         return;
     }

     NdisQueryBuffer(pBuffer,&pBuffData,&cbBuffData);
     pEthHead = (ETHERNET_MAC_HEADER *)pBuffData;

     memcpy(ethHead,pEthHead,sizeof(ETHERNET_MAC_HEADER));
 }

 void
 CAR6KMini::flushNdisPacketQ(LIST_ENTRY *packetQ)
 {
     LIST_ENTRY *pNode;
     NDIS_PACKET *pNdisPacket;

     while(!(IsListEmpty(packetQ)))
     {
         //LOCK
         pNode = RemoveHeadList(packetQ);
         pNdisPacket = (PNDIS_PACKET) ((PBYTE)pNode - offsetof(NDIS_PACKET,MiniportReserved));
         if(!(TAG_UCAST_PCK == (*(UINT32 *)pNdisPacket->ProtocolReserved)) &&
            !(TAG_WPA_SEND_PCK == (*(UINT32 *)pNdisPacket->ProtocolReserved)))
         {
             NdisMSendComplete(m_MiniportAdapterHandle, pNdisPacket, NDIS_STATUS_FAILURE);
         }
         else
         {
             ReturnPacket(pNdisPacket); // packet to Asso-STA
         }
     }
 }

 BOOL
 CAR6KMini::isStationSleep(void)
 {
     A_UINT8 sta =0;
     for (sta=0; sta<AP_MAX_NUM_STA; sta++)
     {
         if (STA_IS_PWR_SLEEP((&m_staList[sta])))
         {
            return TRUE;
         }
      }
     return FALSE;
 }















