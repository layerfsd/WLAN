//------------------------------------------------------------------------------
// <copyright file="car6k_ap_oid.cpp" company="Atheros">
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

static A_UINT8 null_mac[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

A_UINT8 mac_cmp_wild (A_UINT8 *mac, A_UINT8 *new_mac, A_UINT8 wild, A_UINT8 new_wild)
{
    A_UINT8 i;

    for(i=0;i<ATH_MAC_LEN;i++) {
        if((wild & 1<<i) && (new_wild & 1<<i)) continue;
        if(mac[i] != new_mac[i]) return 1;
    }
    if((A_MEMCMP(new_mac, null_mac, 6)==0) && new_wild &&
        (wild != new_wild)) {
        return 1;
    }

    return 0;
}

A_UINT8 acl_add_del_mac (WMI_AP_ACL *a, WMI_AP_ACL_MAC_CMD *acl)
{
    A_INT8    already_avail=-1, free_slot=-1, i;

    /* To check whether this mac is already there in our list */
    for(i=AP_ACL_SIZE-1;i>=0;i--)
    {
        if(mac_cmp_wild(a->acl_mac[i], acl->mac, a->wildcard[i],
            acl->wildcard)==0)
                already_avail = i;

        if(!((1 << i) & a->index))
            free_slot = i;
    }

    if(acl->action == ADD_MAC_ADDR)
    {
        /* Dont add mac if it is already available */
        if((already_avail >= 0) || (free_slot == -1))
            return 0;

        A_MEMCPY(a->acl_mac[free_slot], acl->mac, ATH_MAC_LEN);
        a->index = a->index | (1 << free_slot);
        acl->index = free_slot;
        a->wildcard[free_slot] = acl->wildcard;
        return 1;
    }
    else if(acl->action == DEL_MAC_ADDR)
    {
        if(acl->index > AP_ACL_SIZE)
            return 0;

        if(!(a->index & (1 << acl->index)))
            return 0;

        A_MEMZERO(a->acl_mac[acl->index],ATH_MAC_LEN);
        a->index = a->index & ~(1 << acl->index);
        a->wildcard[acl->index] = 0;
        return 1;
    }

    return 0;
}


NDIS_STATUS
CAR6KMini::Set802_11_apmode (WMI_CONNECT_CMD *wCmd)
{

    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    AP_MODE_CMD *papCmd = &m_apCmd;
    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    m_networkType = AP_NETWORK;
    memcpy(m_apCmd.ssid,wCmd->ssid,wCmd->ssidLength);
    m_apCmd.ssidLength = wCmd->ssidLength;
    m_apCmd.networkType = AP_NETWORK;
    m_apCmd.channel = wCmd->channel;
    m_apCmd.dot11AuthMode =  OPEN_AUTH;
    m_apCmd.authMode = NONE_AUTH;
    m_apCmd.pairwiseCryptoType = NONE_CRYPT;
    m_apCmd.pairwiseCryptoLen = 0;
    m_apCmd.groupCryptoType = NONE_CRYPT;
    m_apCmd.groupCryptoLen = 0;
    m_apCmd.ctrl_flags = 0;

    m_apCrypt.cryptType = NONE_CRYPT;
    m_apCrypt.IsAscii = FALSE;
    m_apCrypt.KeyLength = 0;
    m_apCrypt.KeyIndex = 0;
    m_wpa_buffer = (A_UINT8*) malloc (MAX_HNDSHK_PCK_SIZE);

    wmi_free_allnodes((wmi_t*)m_pWMI);

    m_mcastq_cnt = 0;
    m_ucastq_cnt = 0;

    m_bIsSwitchAPtoSTA = FALSE;

    InitializeListHead(&m_mcastq);
    NdisAllocateSpinLock(&m_mcastqLock);
    if (m_Config.intrabssEnable)
    {
        m_ap_intrabss = TRUE;
        m_apxtn.IntraBss = TRUE;
    }
    else
    {
        m_ap_intrabss = FALSE;
        m_apxtn.IntraBss = FALSE;
    }

    /* Issue Commit Command to switch AP Mode */
     status = wmi_ap_profile_commit((wmi_t*)m_pWMI,(WMI_CONNECT_CMD *)papCmd);
     if (status != A_OK)
     {
         NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"WMI Commit Command Failed %x \n",status);
         return NDIS_STATUS_FAILURE;
     }

     status = wmi_powermode_cmd((struct wmi_t *)m_pWMI,MAX_PERF_POWER);
     if (status != A_OK)
     {
         return NDIS_STATUS_FAILURE;
     }
     return ndis_status;
}

NDIS_STATUS
CAR6KMini::Set_car6k_security (AP_MODE_CRYPT * apsec)
{

    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    switch (apsec->Authmode)
    {
        case NONE_AUTH:
        ndis_status = configap_open (apsec);
        if (ndis_status != NDIS_STATUS_SUCCESS)
        {
            printf ("AR6K: ERROR - configure in open auth = %u\n", ndis_status);
            return NDIS_STATUS_FAILURE;
        }
        break;

        case WPA_PSK_AUTH:
        ndis_status = configap_wpa (apsec);
        if (ndis_status != NDIS_STATUS_SUCCESS)
        {
            printf ("AR6K: ERROR - configure in wpa psk auth = %u\n", ndis_status);
            return NDIS_STATUS_FAILURE;
        }
        break;

        case WPA2_PSK_AUTH:
        ndis_status = configap_wpa2 (apsec);
        if (ndis_status != NDIS_STATUS_SUCCESS)
        {
            printf ("AR6K: ERROR - configure in wpa2 psk auth = %u\n", ndis_status);
            return NDIS_STATUS_FAILURE;
        }
        break;
    }

    if (apsec->cryptType != WEP_CRYPT)
    {
        status = wmi_ap_profile_commit ((wmi_t*)m_pWMI, (WMI_CONNECT_CMD *)&m_apCmd);
        if (status != A_OK)
        {
            printf ("AR6K: ERROR - wmi_ap_profile_commit = %u\n", status);
            return NDIS_STATUS_FAILURE;
        }
    }

    return ndis_status;
}

NDIS_STATUS
CAR6KMini::configap_open (AP_MODE_CRYPT * apsec)
{
    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    BYTE         KeyIndex;
    A_UINT8      KeyUsage = 0;

    if (apsec->cryptType == NONE_CRYPT)
    {
        m_apCmd.dot11AuthMode      = OPEN_AUTH;
        m_apCmd.authMode           = NONE_AUTH;
        m_apCmd.pairwiseCryptoType = NONE_CRYPT;
        m_apCmd.pairwiseCryptoLen  = 0;
        m_apCmd.groupCryptoType    = NONE_CRYPT;
        m_apCmd.groupCryptoLen     = 0;
        m_apCmd.ctrl_flags         = 0;

        m_apCrypt.cryptType        = NONE_CRYPT;
        m_apCrypt.Authmode         = NONE_AUTH;
        m_apCrypt.IsAscii          = FALSE;
        m_apCrypt.KeyLength        = 0;
        m_apCrypt.KeyIndex         = 0;
        m_apCrypt.dot11_authmode   = OPEN_AUTH;

    }

    if (apsec->cryptType == WEP_CRYPT)
    {
        m_apCrypt.cryptType = WEP_CRYPT ;
        m_apCrypt.IsAscii   = apsec->IsAscii;
        m_apCrypt.Authmode  = NONE_AUTH;


        if (apsec->dot11_authmode == SHARED_AUTH)
        {
            m_apCmd.dot11AuthMode    = SHARED_AUTH;

            m_apCrypt.dot11_authmode = SHARED_AUTH;

        }
        if (apsec->dot11_authmode == OPEN_AUTH)
        {
           m_apCmd.dot11AuthMode    = OPEN_AUTH;

           m_apCrypt.dot11_authmode = OPEN_AUTH;
        }

        m_apCmd.authMode            = NONE_AUTH;
        m_apCmd.pairwiseCryptoType  = WEP_CRYPT;
        m_apCmd.groupCryptoType     = WEP_CRYPT;
        m_apCmd.pairwiseCryptoLen   = 0;
        m_apCmd.groupCryptoLen      = 0;


        if (apsec->KeyIndex & 0x80000000)
        {
            KeyUsage |= TX_USAGE;
        }

        if (!(apsec->KeyIndex & 0x40000000))
        {
            KeyUsage |= GROUP_USAGE;
        }

        KeyIndex = (BYTE)(apsec->KeyIndex & 0xff);

        if (KeyUsage & TX_USAGE && !(KeyUsage & GROUP_USAGE))
        {
            if (KeyIndex != 0)
            {
                return NDIS_STATUS_INVALID_DATA;
            }
        }

        if (KeyIndex < WMI_MIN_KEY_INDEX || KeyIndex > WMI_MAX_KEY_INDEX)
        {
            return NDIS_STATUS_INVALID_DATA;
        }

        m_apCrypt.KeyIndex = KeyIndex;
        m_apCrypt.KeyLength = apsec->KeyLength;

        if (apsec->KeyLength <= sizeof(m_Key[KeyIndex]))
        {
            m_cbKey[KeyIndex] = apsec->KeyLength;
            memcpy(m_Key[KeyIndex], apsec->KeyMaterial, apsec->KeyLength);
            m_KeyIndex[KeyIndex] = KeyIndex;
            m_KeyRSC[KeyIndex] = 0;
            m_KeyUsage[KeyIndex] = KeyUsage;
        }

        status = wmi_ap_profile_commit ((wmi_t*)m_pWMI, (WMI_CONNECT_CMD *)&m_apCmd);
        if (status != A_OK)
        {
            printf ("AR6K: ERROR - wmi_ap_profile_commit = %u\n", status);
            return NDIS_STATUS_FAILURE;
        }

        status = wmi_addKey_cmd((wmi_t *)m_pWMI,
                               (A_UINT8)KeyIndex,
                               WEP_CRYPT,
                               m_KeyUsage[KeyIndex],
                               (A_UINT8)m_cbKey[KeyIndex],
                               (A_UINT8*)&m_KeyRSC[KeyIndex],
                               m_Key[KeyIndex], KEY_OP_INIT_VAL, NULL, SYNC_BOTH_WMIFLAG);

        if (status != A_OK)
        {
            NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K: ERROR - wmi_addKey_cmd failed = %u\n", status);
            return NDIS_STATUS_FAILURE;
        }

    }

    if ((apsec->cryptType == TKIP_CRYPT) || (apsec->cryptType == AES_CRYPT))
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K: ERROR - not supported in auth mode open = %u\n", status);
        return NDIS_STATUS_FAILURE;
    }

    return ndis_status;
}

NDIS_STATUS
CAR6KMini::configap_wpa (AP_MODE_CRYPT * apsec)
{
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    A_STATUS status = A_OK;

    A_UINT8 passKey[CFG_MAX_PASSPHRASE];

    if ((apsec->cryptType == NONE_CRYPT) || (apsec->cryptType == WEP_CRYPT)
        || (apsec->cryptType == AES_CRYPT))
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - not supported in auth mode open = %u\n", status);
        return NDIS_STATUS_FAILURE;
    }

    m_apCrypt.KeyLength         = apsec->KeyLength;
    m_apCrypt.KeyIndex          = 0;
    m_apCrypt.IsAscii           = apsec->IsAscii;
    m_apCrypt.dot11_authmode    = OPEN_AUTH;
    m_apCrypt.cryptType         = TKIP_CRYPT;
    m_apCrypt.Authmode          = WPA_PSK_AUTH;

    m_crypt_type                = EKI_TYPE_TKIP;
    m_key_size                  = TKIP_KEY_SIZE_BYTES ;

    m_apCmd.authMode            = WPA_PSK_AUTH;
    m_apCmd.pairwiseCryptoType  = TKIP_CRYPT;
    m_apCmd.groupCryptoType     = TKIP_CRYPT;
    m_apCmd.dot11AuthMode       = OPEN_AUTH;
    m_apCmd.pairwiseCryptoLen   = m_key_size;
    m_apCmd.groupCryptoLen      = m_key_size;

    if (! apsec->IsAscii)
    {
        memcpy (key, (UINT8 *)apsec->KeyMaterial, MAX_WPA_ASCII_KEY_SIZE);
    }

    else
    {
        PasswordHash ((char *)apsec->KeyMaterial, m_apCmd.ssid, m_apCmd.ssidLength, passKey);
        memcpy (key, passKey, CFG_MAX_PASSPHRASE);
    }

    car6k_keyCounterInit (m_CurrentAddress);
    car6k_GMKGenerate (m_CurrentAddress);
    car6k_GNonceCalc (m_CurrentAddress);
    car6k_GTKCalc (m_crypt_type, m_CurrentAddress, m_current_grp_index);

    return ndis_status;

}

NDIS_STATUS
CAR6KMini::configap_wpa2 (AP_MODE_CRYPT * apsec)
{
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    A_STATUS status = A_OK;

    A_UINT8 passKey[CFG_MAX_PASSPHRASE];

    if ((apsec->cryptType == NONE_CRYPT) || (apsec->cryptType == WEP_CRYPT)
        || (apsec->cryptType == TKIP_CRYPT))
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - not supported in auth mode open = %u\n", status);
        return NDIS_STATUS_FAILURE;
    }

    m_apCrypt.KeyLength        = apsec->KeyLength;
    m_apCrypt.KeyIndex         = 0;
    m_apCrypt.IsAscii          = apsec->IsAscii;
    m_apCrypt.dot11_authmode   = OPEN_AUTH;
    m_crypt_type               = EKI_TYPE_AES;
    m_key_size                 = AES_KEY_SIZE_BYTES;

    m_apCmd.authMode           = WPA2_PSK_AUTH;
    m_apCmd.dot11AuthMode      = OPEN_AUTH;
    m_apCmd.pairwiseCryptoType = AES_CRYPT;
    m_apCmd.groupCryptoType    = AES_CRYPT;
    m_apCmd.pairwiseCryptoLen  = m_key_size;
    m_apCmd.groupCryptoLen     = m_key_size;

    m_apCrypt.cryptType        = AES_CRYPT;
    m_apCrypt.Authmode         = WPA2_PSK_AUTH;


    if (! apsec->IsAscii)
    {
        memcpy (key, (UINT8 *)apsec->KeyMaterial, MAX_WPA_ASCII_KEY_SIZE);
    }

    else
    {
        PasswordHash ((char *)apsec->KeyMaterial, m_apCmd.ssid, m_apCmd.ssidLength, passKey);
        memcpy (key, passKey, CFG_MAX_PASSPHRASE);
    }

    car6k_keyCounterInit (m_CurrentAddress);
    car6k_GMKGenerate (m_CurrentAddress);
    car6k_GNonceCalc (m_CurrentAddress);
    car6k_GTKCalc (m_crypt_type, m_CurrentAddress, m_current_grp_index);

    return ndis_status;

}

NDIS_STATUS
CAR6KMini::Set802_11_apmode_xtn (AP_COMMIT_XTND *wCmd)
{

    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }
    m_networkType = AP_NETWORK;
    memcpy(m_apCmd.ssid,wCmd->ssid,wCmd->ssidLength);
    m_apCmd.ssidLength = wCmd->ssidLength;
    m_apCmd.networkType = AP_NETWORK;
    m_apCmd.dot11AuthMode =  OPEN_AUTH;
    m_apCmd.authMode = NONE_AUTH;
    m_apCmd.pairwiseCryptoType = NONE_CRYPT;
    m_apCmd.pairwiseCryptoLen = 0;
    m_apCmd.groupCryptoType = NONE_CRYPT;
    m_apCmd.groupCryptoLen = 0;
    m_apCmd.ctrl_flags = 0;

    m_apCrypt.cryptType = NONE_CRYPT;
    m_apCrypt.IsAscii = FALSE;
    m_apCrypt.KeyLength = 0;
    m_apCrypt.KeyIndex = 0;

    wmi_free_allnodes((wmi_t*)m_pWMI);
    m_wpa_buffer = (A_UINT8*) malloc (MAX_HNDSHK_PCK_SIZE);

    m_bIsSwitchAPtoSTA = FALSE;

    InitializeListHead(&m_mcastq);
    NdisAllocateSpinLock(&m_mcastqLock);
    if (m_Config.intrabssEnable)
    {
        m_ap_intrabss = TRUE;
        m_apxtn.IntraBss = TRUE;
    }
    else
    {
        m_ap_intrabss = FALSE;
        m_apxtn.IntraBss = FALSE;
    }

     memcpy( m_apxtn.ssid,m_apCmd.ssid,m_apCmd.ssidLength);
     m_apxtn.ssidLength = m_apCmd.ssidLength;

    if(wCmd->channel != m_apxtn.channel)
    {
        m_apCmd.channel = wCmd->channel;
        m_apxtn.channel = wCmd->channel;
    }
    else
    {
        m_apCmd.channel = m_apxtn.channel;
    }

    if(wCmd ->hidden_ssid != m_apxtn.hidden_ssid)
    {
        wmiStatus = wmi_ap_set_hidden_ssid((wmi_t*)m_pWMI, wCmd->hidden_ssid);
        m_apxtn.hidden_ssid  = wCmd ->hidden_ssid;
        if(A_OK != wmiStatus)
        {
             NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_set_hidden_ssid failed= %u\n", wmiStatus);
             return NDIS_STATUS_FAILURE;
        }
    }

    if((wCmd ->bgprotection == 0) && (wCmd ->bgprotection != m_apxtn.bgprotection))
    {
        wmiStatus = wmi_ap_bgscan_time((wmi_t*)m_pWMI, 0, 0);
        m_apxtn.bgprotection = wCmd ->bgprotection ;
        if(A_OK != wmiStatus)
        {
             NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_bgscan_time failed= %u\n", wmiStatus);
             return NDIS_STATUS_FAILURE;
        }
    }

    else if ((wCmd ->bgprotection == 1) && (wCmd ->bgprotection != m_apxtn.bgprotection))
    {
        wmiStatus = wmi_ap_bgscan_time((wmi_t*)m_pWMI, 5, 200);
        m_apxtn.bgprotection = wCmd ->bgprotection ;
        if(A_OK != wmiStatus)
        {
             NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_bgscan_time failed= %u\n", wmiStatus);
             return NDIS_STATUS_FAILURE;
        }

    }

    if(wCmd ->dtim != m_apxtn.dtim)
    {
        wmiStatus = wmi_ap_set_dtim((wmi_t*)m_pWMI,wCmd->dtim);
        m_apxtn.dtim = wCmd ->dtim;
        if(A_OK != wmiStatus)
        {

             NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_set_dtim failed= %u\n", wmiStatus);
             return NDIS_STATUS_FAILURE;
        }
    }

    if (! m_Config.intrabssEnable)
    {
        wCmd ->IntraBss = m_Config.intrabssEnable;
    }

    if(wCmd ->IntraBss != m_apxtn.IntraBss)
    {
        m_ap_intrabss = wCmd ->IntraBss;
        m_apxtn.IntraBss = wCmd ->IntraBss;
    }

    if (wCmd ->beaconInterval != m_apxtn.beaconInterval)
    {
        if ((wCmd->beaconInterval < BEACON_INTVL_MIN) || (wCmd->beaconInterval > BEACON_INTVL_MAX))
        {
            return NDIS_STATUS_INVALID_DATA;
        }

        wmiStatus = wmi_set_adhoc_bconIntvl_cmd ((wmi_t*)m_pWMI, wCmd->beaconInterval);
        if (A_OK != wmiStatus)
        {
             NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_set_adhoc_bconIntvl_cmd failed= %u\n", wmiStatus);
             return NDIS_STATUS_FAILURE;
        }

        m_apxtn.beaconInterval = wCmd ->beaconInterval;
    }

    return  wmiStatus;
}



NDIS_STATUS
CAR6KMini::Set802_11_stamode ()
{

    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    A_UINT32 i;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (m_networkType == INFRA_NETWORK)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: Already in Station mode");
        return NDIS_STATUS_FAILURE;
    }


    if (m_networkType == AP_NETWORK)
    {
        m_bIsSwitchAPtoSTA = TRUE;
        m_networkType = INFRA_NETWORK;

        for (i = 0; i < AP_MAX_NUM_STA; i++)
        {
            removeNewStation(m_staList[i].mac);
            memset (&m_staList[i], 0, sizeof (STATION));
        }

        NdisFreeSpinLock(&m_mcastqLock);
        m_staListIndex      = 0;
        m_Connected         = TRUE;
        m_ChannelHint       = 0;
        m_ConnectedChannel  = 0;
        m_WantToBeConnected = 0;
        memset (m_PeerBSSID, 0, ETHERNET_MAC_ADDRESS_LENGTH);

        m_AuthenticationMode = Ndis802_11AuthModeOpen;
        m_EncryptionStatus   = Ndis802_11EncryptionDisabled;
        m_InfrastructureMode = Ndis802_11Infrastructure;
        free (m_wpa_buffer);
        return NDIS_STATUS_SUCCESS;
    }

    return ndis_status;
}


 NDIS_STATUS
 CAR6KMini::Set802_11_numofsta (WMI_AP_SET_NUM_STA_CMD * numofsta)
 {

    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    if (m_networkType == AP_NETWORK)
    {

        if(numofsta->num_sta > AP_MAX_NUM_STA)
        {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR," value greater than maximum allowed station \n");
            return NDIS_STATUS_FAILURE;
        }

        if(m_ap_num_sta == numofsta->num_sta)
        {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: Max station limit is already %u", m_ap_num_sta);
            return NDIS_STATUS_SUCCESS;
        }
        else
        {
            status = wmi_ap_set_num_sta((wmi_t*)m_pWMI, numofsta->num_sta);
            if (status != A_OK)
            {
                return NDIS_STATUS_FAILURE;
            }
        }

        m_ap_num_sta = numofsta->num_sta;
    }

    return ndis_status;
}

 NDIS_STATUS
 CAR6KMini::Set802_11_adddel_acl (WMI_AP_ACL_MAC_CMD * pACL)
 {

    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        return NDIS_STATUS_FAILURE;
    }

    if (acl_add_del_mac(&m_acl, pACL))
    {
        status = wmi_ap_acl_mac_list((wmi_t*)m_pWMI, pACL);

        if (status != A_OK)
        {
            return NDIS_STATUS_FAILURE;
        }
    }
    else
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ACL List Error");
        return NDIS_STATUS_FAILURE;
    }

    return ndis_status;
}

NDIS_STATUS
CAR6KMini::Set802_11_HiddenSsid (WMI_AP_HIDDEN_SSID_CMD *pHiddenSsidCmd)
{
    A_STATUS wmiStatus = A_OK;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    if (NULL == pHiddenSsidCmd)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - Null argument\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    if (pHiddenSsidCmd->hidden_ssid == m_apxtn.hidden_ssid)
    {
        return NDIS_STATUS_SUCCESS;
    }

    wmiStatus = wmi_ap_set_hidden_ssid((wmi_t*)m_pWMI, pHiddenSsidCmd->hidden_ssid);
    if (A_OK != wmiStatus)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_set_hidden_ssid failed= %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    wmiStatus = wmi_ap_profile_commit((wmi_t*)m_pWMI,(WMI_CONNECT_CMD *)&m_apCmd);

    if (A_OK != wmiStatus)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_profile_commit failed= %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    /* Update hidden_ssid in AP mode status structure */
    m_apxtn.hidden_ssid  = pHiddenSsidCmd->hidden_ssid;

    return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::Set802_11_BGProtection (WMI_AP_PROT_SCAN_TIME_CMD *pBGScan)
{
    A_STATUS wmiStatus = A_OK;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    if(NULL == pBGScan)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - Null argument\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    wmiStatus = wmi_ap_bgscan_time((wmi_t*)m_pWMI, pBGScan->period_min, pBGScan->dwell_ms);
    if(A_OK != wmiStatus)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_bgscan_time failed= %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    wmiStatus = wmi_ap_profile_commit((wmi_t*)m_pWMI,(WMI_CONNECT_CMD *)&m_apCmd);
    if(A_OK != wmiStatus)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_profile_commit failed= %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    /* Update bgprotection in AP mode status structure */
    if (pBGScan->period_min && pBGScan->dwell_ms)
    {
        m_apxtn.bgprotection = TRUE;
    }
    else
    {
        m_apxtn.bgprotection = FALSE;
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11_DTIM (WMI_AP_SET_DTIM_CMD *pDTIMCmd)
{
    A_STATUS wmiStatus = A_OK;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        return NDIS_STATUS_FAILURE;
    }

    if(NULL == pDTIMCmd)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - Null argument\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    if (pDTIMCmd->dtim == m_apxtn.dtim)
    {
        return NDIS_STATUS_SUCCESS;
    }

    wmiStatus = wmi_ap_set_dtim((wmi_t*)m_pWMI, pDTIMCmd->dtim);
    if(A_OK != wmiStatus)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_set_dtim failed= %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    wmiStatus = wmi_ap_profile_commit((wmi_t*)m_pWMI,(WMI_CONNECT_CMD *)&m_apCmd);
    if(A_OK != wmiStatus)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_profile_commit failed= %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    /* Update dtim in AP mode status structure */
    m_apxtn.dtim = pDTIMCmd->dtim;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11_RegDomain (WMI_SET_COUNTRY_CMD *pCountryCmd)
{
    A_STATUS wmiStatus = A_OK;
    if (!m_WMIReady)
    {
         return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    /* Applicable to AP and Ad-Hoc mode */


    if(NULL == pCountryCmd)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - Null argument\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    wmiStatus = wmi_set_country((wmi_t*)m_pWMI, pCountryCmd->countryCode);
    if(A_OK != wmiStatus)
    {

         NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_set_country failed= %u\n", wmiStatus);
         return NDIS_STATUS_FAILURE;
    }

    if (AP_NETWORK == m_networkType)
    {
        wmiStatus = wmi_ap_profile_commit((wmi_t*)m_pWMI,(WMI_CONNECT_CMD *)&m_apCmd);
        if(A_OK != wmiStatus)
        {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_profile_commit failed= %u\n", wmiStatus);
            return NDIS_STATUS_FAILURE;
        }
    }

    return NDIS_STATUS_SUCCESS;

}

NDIS_STATUS
CAR6KMini::Set802_11_Channel (AP_SET_CHANNEL_CMD *pChannelCmd)
{
    A_STATUS wmiStatus = A_OK;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    if(NULL == pChannelCmd)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - Null argument\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    if (pChannelCmd->channel == m_apxtn.channel)
    {
        return NDIS_STATUS_SUCCESS;
    }

    m_apCmd.channel = pChannelCmd->channel;

    /* Issue Commit Command to update the channel */
    wmiStatus = wmi_ap_profile_commit((wmi_t*)m_pWMI,(WMI_CONNECT_CMD *)&m_apCmd);
    if (wmiStatus != A_OK)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"WMI Commit Command Failed %x \n",wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    /* Update channel info of query structure */
    m_apxtn.channel = pChannelCmd->channel;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11_bcon_intvl (WMI_BEACON_INT_CMD *bIntvlCmd)
{
    A_STATUS wmiStatus = A_OK;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    if ((AP_NETWORK != m_networkType) && (Ndis802_11IBSS != m_InfrastructureMode))
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR,"Not applicable in infratructure mode \n");
        return NDIS_STATUS_FAILURE;
    }

    if (NULL == bIntvlCmd)
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K: ERROR - Null argument\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    if (bIntvlCmd->beaconInterval == m_apxtn.beaconInterval)
    {
        return NDIS_STATUS_SUCCESS;
    }

    if ((bIntvlCmd->beaconInterval < BEACON_INTVL_MIN) || (bIntvlCmd->beaconInterval > BEACON_INTVL_MAX))
    {
        return NDIS_STATUS_INVALID_DATA;
    }

    wmiStatus = wmi_set_adhoc_bconIntvl_cmd ((wmi_t*)m_pWMI, bIntvlCmd->beaconInterval);

    if (A_OK != wmiStatus)
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "WMI set beacon interval Command Failed %x \n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    if (AP_NETWORK == m_networkType)
    {
        wmiStatus = wmi_ap_profile_commit ((wmi_t*)m_pWMI,(WMI_CONNECT_CMD *)&m_apCmd);

        if (A_OK != wmiStatus)
        {

             NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K: ERROR - wmi_ap_profile_commit failed= %u\n", wmiStatus);
             return NDIS_STATUS_FAILURE;
        }

        m_apxtn.beaconInterval  = bIntvlCmd->beaconInterval;
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11_conn_inact_time (WMI_AP_CONN_INACT_CMD *pInact)
{
    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    if (!m_WMIReady)
    {
         return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
         NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
         return NDIS_STATUS_FAILURE;
    }

    wmiStatus = wmi_ap_conn_inact_time((wmi_t*)m_pWMI, pInact->period);
    if ( wmiStatus  != A_OK)
    {
        return NDIS_STATUS_FAILURE;
    }
    return ndis_status;
}

NDIS_STATUS
CAR6KMini::Set802_11_remove_sta (WMI_AP_SET_MLME_CMD *pMlme)
{
    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }
    if (AP_NETWORK != m_networkType)
    {
         NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
         return NDIS_STATUS_FAILURE;
    }
    switch (pMlme->cmd)
    {
      case WMI_AP_MLME_AUTHORIZE :
      case WMI_AP_MLME_UNAUTHORIZE:
          break;
      case WMI_AP_DEAUTH :
      case WMI_AP_DISASSOC:
          removeNewStation(pMlme->mac);
          break;
      default:
          return 0;
   }

   wmi_ap_set_mlme((wmi_t*)m_pWMI, pMlme->cmd, pMlme->mac,pMlme->reason);
   if ( wmiStatus  != A_OK)
   {
        return NDIS_STATUS_FAILURE;
   }
   return ndis_status;
}


NDIS_STATUS
CAR6KMini::Getsta (ap_get_sta *staList)
{
    int i;
    A_MEMZERO(staList, sizeof(staList));

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    for(i=0;i<AP_MAX_NUM_STA;i++)
    {
        A_MEMCPY(staList->sta[i].mac, &m_staList[i].mac, ATH_MAC_LEN);
        staList->sta[i].aid = m_staList[i].aid;
    }
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11_numofsta (AP_GET_NUM_STA_CMD * numofsta)
{

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    if(NULL == numofsta)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - Get802_11_numofsta failed due to null argument\n");
        return NDIS_STATUS_FAILURE;
    }

    numofsta->num_sta = m_ap_num_sta;
    return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::GetAcl (WMI_AP_ACL * pGetAcl)
{
     int i = 0;
     A_MEMZERO(pGetAcl, sizeof(pGetAcl));

     if (!m_WMIReady)
     {
         return NDIS_STATUS_ADAPTER_NOT_READY;
     }

     if (AP_NETWORK != m_networkType)
     {
         NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
         return NDIS_STATUS_FAILURE;
     }

     for(i=0;i<AP_ACL_SIZE;i++)
     {
        A_MEMCPY(pGetAcl->acl_mac[i], &m_acl.acl_mac[i], ATH_MAC_LEN);
        pGetAcl->wildcard[i] =  m_acl.wildcard[i];
        pGetAcl->policy = m_acl.policy;

     }
     return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::GetAPStatus (AP_COMMIT_XTND *pApstatus)
{

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    A_MEMZERO(pApstatus, sizeof(pApstatus));
    A_MEMCPY(pApstatus->ssid,m_apxtn.ssid,m_apxtn.ssidLength);
    pApstatus->ssidLength = m_apxtn.ssidLength;
    pApstatus->channel = m_apxtn.channel;
    pApstatus->dtim = m_apxtn.dtim;
    pApstatus->bgprotection = m_apxtn.bgprotection;
    pApstatus->hidden_ssid = m_apxtn.hidden_ssid;
    pApstatus->beaconInterval = m_apxtn.beaconInterval;
    pApstatus->IntraBss = m_apxtn.IntraBss;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::GetAPCrypt (AP_MODE_CRYPT *pApcrypt)
{
    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    A_MEMZERO(pApcrypt, sizeof(pApcrypt));
    pApcrypt->KeyLength      = m_apCrypt.KeyLength;
    pApcrypt->KeyIndex       = m_apCrypt.KeyIndex;
    pApcrypt->IsAscii        = m_apCrypt.IsAscii;
    pApcrypt->cryptType      = m_apCrypt.cryptType;
    pApcrypt->Authmode       = m_apCrypt.Authmode;
    pApcrypt->dot11_authmode = m_apCrypt.dot11_authmode;

    return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::GetCurrentmode (UINT *mode)
{
    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }
    *mode = m_networkType;
    return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::Set_car6k_acl_policy (WMI_AP_ACL_POLICY_CMD *pACLpolicy)
{
    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    if(pACLpolicy->policy == m_acl.policy)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: The ACL policy is already %d", m_acl.policy);
        return NDIS_STATUS_SUCCESS;
    }

    if(!(pACLpolicy->policy & AP_ACL_RETAIN_LIST_MASK))
    {
        /* clear ACL list */
        memset(&m_acl,0,sizeof(WMI_AP_ACL));
    }
    if (pACLpolicy->policy & AP_ACL_RETAIN_LIST_MASK)
    {
        m_acl.policy = pACLpolicy->policy & ~ AP_ACL_RETAIN_LIST_MASK;
    }
    else
    {
        m_acl.policy = pACLpolicy->policy;
    }
    status = wmi_ap_set_acl_policy ((wmi_t*)m_pWMI, pACLpolicy->policy);
    if (status != A_OK)
    {
        return NDIS_STATUS_FAILURE;
    }

    return ndis_status;
}


NDIS_STATUS
CAR6KMini::Set_car6k_intrabss (AP_SET_INTRA_BSS *pIntra)
{
    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (AP_NETWORK != m_networkType)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"Not applicable in station mode \n");
        return NDIS_STATUS_FAILURE;
    }

    if (pIntra->intrabss == m_ap_intrabss)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: The Intra BSS traffic is already %d", m_ap_intrabss);
        return NDIS_STATUS_SUCCESS;
    }
    m_ap_intrabss = pIntra->intrabss;
    m_apxtn.IntraBss = pIntra->intrabss;
    return ndis_status;
}











