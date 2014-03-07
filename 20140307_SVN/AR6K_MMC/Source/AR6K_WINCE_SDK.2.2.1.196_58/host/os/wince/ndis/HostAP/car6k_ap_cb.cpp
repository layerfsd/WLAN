//------------------------------------------------------------------------------
// <copyright file="car6k_ap_cb.cpp" company="Atheros">
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
CAR6KMini::group_rekeyingTimeout ()
{
    car6k_rekeying_Sending_Group_Msg_1_of_2 ();

    if (m_Config.rekeyingPeriod)
    {
        A_TIMEOUT_MS(&m_Grp_rekeying_timer, (m_Config.rekeyingPeriod * 1000), 0);
    }
}

void
CAR6KMini::car6k_ap_mic_event (A_UINT8 keyid, A_BOOL isMcast)
{
    int i;
    A_STATUS wmiStatus = A_OK;
    STATION *station;
    station = GetStation (NULL, keyid);
    if ((station == NULL) || (IS_MAC_NULL (station->mac)))
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_INF, "MIC error from a non assoc station ignore \n");
        return;
    }
    m_ap_mic_failures++;
    if(m_ap_mic_failures >= MAX_MIC_ERRORS)
    {
        m_ap_CounterMeasureOn = TRUE;
        A_UNTIMEOUT (&m_CounterMeasureEnterTimer);
        wmi_set_tkip_countermeasures_cmd ((wmi_t *)m_pWMI, 1);
        m_current_grp_index = (m_current_grp_index == GROUPKEY_IDX_A)? GROUPKEY_IDX_B : GROUPKEY_IDX_A;
        car6k_GMKGenerate (m_CurrentAddress);
        car6k_GNonceCalc (m_CurrentAddress);
        car6k_GTKCalc (m_crypt_type,m_CurrentAddress,m_current_grp_index);
        A_UNTIMEOUT (&m_CounterMeasureExitTimer);
        A_TIMEOUT_MS(&m_CounterMeasureExitTimer, MIC_ERROR_BLOCKOUT_MSEC, 0);

        for(i = 0; i < AP_MAX_NUM_STA; i++)
        {
            STATION *station_temp;
            station_temp = GetStation (m_staList[i].mac, 0);
            WMI_AP_SET_MLME_CMD pMlme;
            if (station_temp == NULL)
            {
                continue ;
            }
            if (IS_MAC_NULL (station_temp->mac))
            {
                continue ;
            }
            memcpy (pMlme.mac, station_temp->mac, ETHERNET_MAC_ADDRESS_LENGTH);
            pMlme.reason = WLAN_REASON_MICHAEL_MIC_FAILURE;
            pMlme.cmd = WMI_AP_DEAUTH;
            wmiStatus = wmi_ap_set_mlme((wmi_t*)m_pWMI, pMlme.cmd, pMlme.mac, pMlme.reason);
            if ( wmiStatus  != A_OK)
            {
                 return;
            }
            removeNewStation (station_temp->mac);
        }

    }
    else
    {
        A_TIMEOUT_MS(&m_CounterMeasureEnterTimer, MIC_ERROR_TIMEOUT_MSEC, 0);
    }
}

void
CAR6KMini::car6k_ap_connect_event (A_UINT16  channel, A_UINT8  *bssid,
                                   A_UINT8  *assocInfo, A_UINT8 assocRespLen)
{
    addNewStation(bssid,channel,assocInfo,assocRespLen);
    m_Connected  = TRUE;

    if (m_ap_CounterMeasureOn)
    {
        return;
    }

    if ((m_apCmd.authMode == WPA_PSK_AUTH) || (m_apCmd.authMode == WPA2_PSK_AUTH))
    {
        STATION *station;
        station = GetStation (bssid, 0);
        A_STATUS wmiStatus = A_OK;
        if (station ->hnd_shk_completed == TRUE)
        {
            WMI_AP_SET_MLME_CMD pMlme;
            memcpy (pMlme.mac, station->mac, ETHERNET_MAC_ADDRESS_LENGTH);
            pMlme.reason = WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT;
            pMlme.cmd = WMI_AP_DEAUTH;
            wmiStatus = wmi_ap_set_mlme ((wmi_t*)m_pWMI, pMlme.cmd, pMlme.mac, pMlme.reason);
            if ( wmiStatus  != A_OK)
            {
                NDIS_DEBUG_PRINTF(ATH_LOG_INF, "wmi_ap_set_mlme - failed \n");
            }

            return;
        }
        station -> m_keyState = WPA_HNDSHK_MSG_1;

        memset (station ->ANonce, 0, sizeof (station ->ANonce));
        memset (station ->SNonce, 0, sizeof (station ->SNonce));
        station ->PTKlength = 0;
        memset (station ->PTK, 0, sizeof (station ->PTK));
        station ->hnd_shk_completed = FALSE;

        car6k_ANonceCalc (station);
        car6k_sendEapolKeyMsg (m_crypt_type, FALSE, FALSE,
                               TRUE, FALSE, 0 , 1, FALSE, 0, station ->ANonce,
                               m_key_size, 0, NULL, 0, bssid);

        NDIS_DEBUG_PRINTF (ATH_LOG_INF, "Four Way Handshake Initiated\n");
        NDIS_DEBUG_PRINTF (ATH_LOG_INF, "Sent M1 \n");

        if (m_Config.rekeyingPeriod)
        {
            A_TIMEOUT_MS (&m_Grp_rekeying_timer, m_Config.rekeyingPeriod * 1000, 0);
        }
     }
}

void
CAR6KMini::car6k_ap_disconnect_event (A_UINT8 reason, A_UINT8 *bssid)
{
    if ((m_apCmd.authMode == WPA_PSK_AUTH) ||
        (m_apCmd.authMode == WPA2_PSK_AUTH))
    {
        STATION *station;
        A_BOOL hndshk_completed = FALSE;
        station = GetStation (bssid, 0);
        if ((station == NULL) || (IS_MAC_NULL (station->mac)))
        {
            return;
        }
        else
        {
            hndshk_completed = station ->hnd_shk_completed;
            removeNewStation (bssid);
            if ((reason != DISCONNECT_CMD) && (hndshk_completed))
            {
                car6k_rekeying_Sending_Group_Msg_1_of_2 ();
            }
        }
    }
    else
    {
        removeNewStation(bssid);
    }
}

extern "C" void
ar6000_rekeyingTimeout(
    A_ATH_TIMER Context)
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;
    Adapter->group_rekeyingTimeout();
}

extern "C" void
ar6000_pspoll_event (void *context, A_UINT16 aid)
{
    STATION *conSta = NULL;
    BOOL qEmpty;
    LIST_ENTRY *pNode;
    NDIS_PACKET *pNdisPacket;
    CAR6KMini *Adapter = (CAR6KMini *)context;
    conSta = Adapter->GetStation(NULL,aid);
    qEmpty = IsListEmpty(&conSta->ucastq);

    if(FALSE == qEmpty)
    {
        pNode = RemoveHeadList(&conSta->ucastq);
        Adapter->m_ucastq_cnt --;
        pNdisPacket = (PNDIS_PACKET) ((PBYTE)pNode - offsetof(NDIS_PACKET,MiniportReserved));
        STA_SET_PS_POLLED(conSta);
        Adapter->SendPackets(&pNdisPacket,1);
        STA_CLR_PS_POLLED(conSta);
        qEmpty = IsListEmpty(&conSta->ucastq);

        if(qEmpty)
        {
            wmi_set_pvb_cmd((wmi_t *)Adapter->m_Wmi,conSta->aid,0);
        }
    }
}

extern "C" void
ar6000_dtimexpiry_event (void *context)
{
    LIST_ENTRY *pNode;
    NDIS_PACKET *pNdisPacket;
    CAR6KMini *Adapter = (CAR6KMini *)context;
    /* Flush mcast packets to STA */
    Adapter->m_dtimExpire = TRUE;
    NdisAcquireSpinLock(&Adapter->m_mcastqLock);
    while(!(IsListEmpty(&Adapter->m_mcastq)))
    {
        pNode = RemoveHeadList(&Adapter->m_mcastq);
        Adapter->m_mcastq_cnt --;
        pNdisPacket = (PNDIS_PACKET) ((PBYTE)pNode - offsetof(NDIS_PACKET,MiniportReserved));
        Adapter->SendPackets(&pNdisPacket,1);
    }

    NdisReleaseSpinLock(&Adapter->m_mcastqLock);

    Adapter->m_dtimExpire = FALSE;

    wmi_set_pvb_cmd((wmi_t *)Adapter->m_Wmi,MCAST_AID,0);
}











