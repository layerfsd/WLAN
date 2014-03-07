//------------------------------------------------------------------------------
// <copyright file="carkoid.cpp" company="Atheros and Microsoft">
//    Copyright (c) 2004-2008 Microsoft Corporation.  All rights reserved.
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
// NDIS 802.11 Miniport class.
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
#include "netbuf.h"
#include "ndisnetbuf.h"
#include "common_drv.h"
#include "targaddrs.h"
#include "a_drv.h"
#include "host_version.h"
extern "C" {
#include "bmi.h"
#include "ar6000_api.h"
}

#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include "ieee80211.h"
#include "platform.h"
#include "a_debug.h"
#include "wince_common_drv.h"

// bssid Info to be saved before suspend operation, to avoid
// connection issues when system resume
A_UINT8   preBssid[IEEE80211_ADDR_LEN];
bss_t *   preBss = NULL;

#ifdef OS_ROAM_MANAGEMENT
typedef struct ar6kScanIndication
{
    NDIS_802_11_STATUS_INDICATION     ind;
    NDIS_802_11_BSSID_SCAN_INFO_LIST  slist;

}*PAR6KSCANINDEVENT;
#endif

#ifdef SUPPORT_WPA2
static NDIS_802_11_AUTHENTICATION_ENCRYPTION g_WPA2AuthEncryptPairSupported[] =
{
    { Ndis802_11AuthModeOpen,    Ndis802_11EncryptionDisabled }, //open no-crypt
    { Ndis802_11AuthModeOpen,    Ndis802_11Encryption1Enabled }, //open WEP
    { Ndis802_11AuthModeShared,  Ndis802_11Encryption1Enabled }, //Shared WEP

    { Ndis802_11AuthModeWPA,     Ndis802_11Encryption2Enabled }, // WPA
    { Ndis802_11AuthModeWPA,     Ndis802_11Encryption3Enabled },
    { Ndis802_11AuthModeWPAPSK,  Ndis802_11Encryption2Enabled }, // WPA-PSK
    { Ndis802_11AuthModeWPAPSK,  Ndis802_11Encryption3Enabled },

    { (NDIS_802_11_AUTHENTICATION_MODE) Ndis802_11AuthModeWPA2,    Ndis802_11Encryption2Enabled }, // WPA2
    { (NDIS_802_11_AUTHENTICATION_MODE) Ndis802_11AuthModeWPA2,    Ndis802_11Encryption3Enabled },
    { (NDIS_802_11_AUTHENTICATION_MODE) Ndis802_11AuthModeWPA2PSK, Ndis802_11Encryption2Enabled }, // WPA2-PSK
    { (NDIS_802_11_AUTHENTICATION_MODE) Ndis802_11AuthModeWPA2PSK, Ndis802_11Encryption3Enabled },
};
#endif //SUPPORT_WPA2

A_STATUS
CAR6KMini::ar6000_get_target_stats()
{
    A_STATUS status = A_OK;
    NdisResetEvent(&m_tgtStatsEvent);

    status = wmi_get_stats_cmd((wmi_t*)m_pWMI);
    if ( status != A_OK)
        return status;

    if( !NdisWaitEvent(&m_tgtStatsEvent, AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_GETSTATS) ) {

        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - No TGT_GETSTATS event after %u ms\n", AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_GETSTATS);
        status = A_ERROR;
    }

    return status;
}

A_STATUS
CAR6KMini::ar6000_get_target_txPwr()
{
    A_STATUS status = A_OK;
    NdisResetEvent (&m_tgtTxPwrEvent);

    status = wmi_get_txPwr_cmd ((wmi_t*)m_pWMI);

    if ( status != A_OK)
        return status;

    if( !NdisWaitEvent(&m_tgtTxPwrEvent, AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_GETTXPWR) ) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - No TGT_GETTXPWR event after %u ms\n", AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_GETTXPWR);
        status = A_ERROR;
    }

    return status;
}

static MPOidInfo g_Car6kSupportedQueryOids[] =
{
    { OID_CAR6K_GET_PNP_POWER, sizeof(ULONG) },
    { OID_CAR6K_RESUME_WLAN_STATE, sizeof(ULONG) },
    { OID_CAR6K_FIRMWARE_VERSION, sizeof(ULONG) },
    { OID_CAR6K_GET_WOW_LIST, sizeof(A_UINT8) },
    { OID_CAR6K_DBGLOG_GET_DEBUG_LOGS, 0 },
    { OID_CAR6K_GET_QOS_QUEUE, sizeof (struct ar6000_queuereq) },
    { OID_CAR6K_802_11_AUTH_ALG_1, sizeof(CAR6K_802_11_AUTH_ALG) },
    { OID_CAR6K_802_11_AUTH_ALG, sizeof(CAR6K_802_11_AUTH_ALG) },
    { OID_CAR6K_802_11_CCKM_AUTHENTICATION_MODE, sizeof(CAR6K_802_11_CCKM_AUTHENTICATION_MODE) },
    { OID_CAR6K_GET_RD, sizeof(UINT) },
    { OID_CAR6K_GET_TGT_STATS, sizeof(AR6K_TARGET_STATS) },
    { OID_CAR6K_GET_WMODE, sizeof(CAR6K_GET_WMODE)},
    { OID_CAR6K_GET_BITRATE, sizeof(CAR6K_GET_BITRATE) },
    { OID_CAR6K_GET_CURRENT_MODE, 0 }, //Soft AP get oids
    { OID_CAR6K_GET_STA,sizeof(ap_get_sta)},
    { OID_CAR6K_GET_ACL,sizeof(WMI_AP_ACL)},
    { OID_CAR6K_GET_AP_STATUS,sizeof(AP_COMMIT_XTND)},
    { OID_CAR6K_GET_AP_CRYPT,sizeof(AP_MODE_CRYPT)},
    { OID_CAR6K_GET_NUM_STA, sizeof(AP_GET_NUM_STA_CMD) },
    { OID_CAR6K_GET_NODEAGE, 0 },
    { OID_CAR6K_GET_KEEPALIVE_INTV,sizeof(WMI_GET_KEEPALIVE_CMD) },
    { 0, 0 }
};

NDIS_STATUS
CAR6KMini::QueryInformation(
    IN  NDIS_OID Oid,
    OUT PVOID    Buffer,
    IN  ULONG    cbBuffer,
    OUT PULONG   pcbWritten,
    OUT PULONG   pcbNeeded)
//
// QueryInformation is a required function that returns information
// about the capabilities and staus of the driver and/or its NIC.
//
{
    NDIS_STATUS                 Status = NDIS_STATUS_SUCCESS;

    // Check that OID is supported and buffer size is reasonable.
    Status = CheckOidRequest(&g_Car6kSupportedQueryOids[0], Oid, cbBuffer, pcbNeeded);
    if (NDIS_STATUS_INVALID_OID == Status)
    {
        // OID is not a supported CAR6K OID. Check to see if it is an 802_11 OID.
        Status = C802_11Miniport::QueryInformation(Oid, Buffer, cbBuffer, pcbWritten, pcbNeeded);
        goto done;
    }
    if (NDIS_STATUS_SUCCESS != Status)
        goto done;

    switch (Oid)
    {
        case OID_CAR6K_GET_PNP_POWER:
            Status = GetPnpPower((ULONG *)Buffer);
            break;

        case OID_CAR6K_RESUME_WLAN_STATE:
            Status = GetResumeWlanState((ULONG *)Buffer);
            break;

        case OID_CAR6K_FIRMWARE_VERSION:
            Status = GetFirmwareVersion((ULONG *)Buffer);
            break;

        case OID_CAR6K_GET_WOW_LIST:
            Status = GetWowList((WMI_GET_WOW_LIST_REPLY *)Buffer, cbBuffer, pcbNeeded);
            break;

        case OID_CAR6K_DBGLOG_GET_DEBUG_LOGS:
            Status = GetDbgLogs();
            break;
        case OID_CAR6K_GET_QOS_QUEUE:
            Status = GetQOSQueue((struct ar6000_queuereq *)Buffer, pcbNeeded);
            break;

        case OID_CAR6K_802_11_AUTH_ALG:
        case OID_CAR6K_802_11_AUTH_ALG_1:
            Status = GetCAr6k802_11AuthAlg((CAR6K_802_11_AUTH_ALG *)Buffer);
            break;

        case OID_CAR6K_802_11_CCKM_AUTHENTICATION_MODE:
            Status = GetCAr6k802_11CCKMAuthMode((CAR6K_802_11_CCKM_AUTHENTICATION_MODE *)Buffer);
            break;

        case OID_CAR6K_GET_RD:
            Status = GetRegDomain((UINT *)Buffer);
            break;
        case OID_CAR6K_GET_TGT_STATS:
            Status = GetTgtStats((AR6K_TARGET_STATS *)Buffer);
            break;

        case OID_CAR6K_GET_WMODE:
            Status = GetWmode((CAR6K_GET_WMODE *)Buffer);
            break;

        case OID_CAR6K_GET_BITRATE:
            Status = GetBitRate ((CAR6K_GET_BITRATE *)Buffer);
            break;

        case OID_CAR6K_GET_STA:
            Status = Getsta((ap_get_sta *)Buffer);
            break;

         case OID_CAR6K_GET_NUM_STA:
            Status = Get802_11_numofsta((AP_GET_NUM_STA_CMD *)Buffer);
            break;

        case OID_CAR6K_GET_ACL:
            Status = GetAcl((WMI_AP_ACL *)Buffer);
            break;

        case OID_CAR6K_GET_AP_STATUS:
            Status = GetAPStatus((AP_COMMIT_XTND *)Buffer);
            break;

        case OID_CAR6K_GET_AP_CRYPT:
            Status = GetAPCrypt((AP_MODE_CRYPT *)Buffer);
            break;

        case OID_CAR6K_GET_CURRENT_MODE :
            Status = GetCurrentmode((UINT *)Buffer);
            break;

        case OID_CAR6K_GET_NODEAGE:
            Status = GetCAR6kWMINodeAge((A_UINT32 *) Buffer);
            break;

        case OID_CAR6K_GET_KEEPALIVE_INTV:
            Status = GetCAR6kWMIKeepAliveInt((WMI_GET_KEEPALIVE_CMD *) Buffer);
            break;

        default:
            Status = NDIS_STATUS_INVALID_OID;
            break;
    }

    if (NDIS_STATUS_SUCCESS == Status)
    {
        if (*pcbNeeded > cbBuffer)
            Status = NDIS_STATUS_INVALID_LENGTH;
        else
            *pcbWritten = *pcbNeeded;
    }

done:
    return Status;
}

static MPOidInfo g_Car6kSupportedSetOids[] =
{
    { OID_CAR6K_802_11_AUTH_ALG_1, sizeof(ULONG) },
    { OID_CAR6K_RESUME_WLAN_STATE, sizeof(ULONG) },
    { OID_CAR6K_BMI_DONE, 0 },
    { OID_CAR6K_BMI_WRITE_MEMORY, sizeof(BMI_WRITE_MEMORY_PARAM) },
    { OID_CAR6K_BMI_EXECUTE, sizeof(BMI_EXECUTE_PARAM) },
    { OID_CAR6K_BMI_SET_APP_START, sizeof(BMI_SET_APP_START_PARAM) },
    { OID_CAR6K_BMI_WRITE_SOC_REGISTER, sizeof(BMI_WRITE_SOC_REG_PARAM) },
    { OID_CAR6K_BMI_READ_MEMORY, sizeof(BMI_READ_MEMORY_PARAM) },
    { OID_CAR6K_BMI_READ_SOC_REGISTER, sizeof(BMI_READ_SOC_REG_PARAM) },
    { OID_CAR6K_BMI_ROMPATCH_INSTALL, sizeof(BMI_ROMPATCH_INSTALL_PARAM) },
    { OID_CAR6K_BMI_ROMPATCH_UNINSTALL, sizeof(BMI_ROMPATCH_UNINSTALL_PARAM) },
    { OID_CAR6K_BMI_ROMPATCH_ACTIVATE, sizeof(BMI_ROMPATCH_ACTIVATE_PARAM) },
    { OID_CAR6K_BMI_ROMPATCH_DEACTIVATE, sizeof(BMI_ROMPATCH_DEACTIVATE_PARAM) },
#ifdef CONFIG_HOST_TCMD_SUPPORT
    { OID_CAR6K_TCMD_CONT_TX, sizeof(TCMD_CONT_TX) },
    { OID_CAR6K_TCMD_CONT_RX, sizeof(TCMD_CONT_RX) },
    { OID_CAR6K_TCMD_PM, sizeof(TCMD_PM) },
#endif //CONFIG_HOST_TCMD_SUPPORT
    { OID_CAR6K_SET_HOST_SLEEP_MODE,  sizeof(WMI_SET_HOST_SLEEP_MODE_CMD) },
    { OID_CAR6K_SET_WOW_MODE, sizeof(WMI_SET_WOW_MODE_CMD) },
    { OID_CAR6K_ADD_WOW_PATTERN, sizeof(WMI_ADD_WOW_PATTERN_CMD) },
    { OID_CAR6K_DEL_WOW_PATTERN, sizeof(WMI_DEL_WOW_PATTERN_CMD) },
    { OID_CAR6K_DBGLOG_CFG_MODULE, sizeof(WMIX_DBGLOG_CFG_MODULE_CMD) },
    { OID_CAR6K_CREATE_QOS, sizeof(WMI_CREATE_PSTREAM_CMD) },
    { OID_CAR6K_DELETE_QOS, sizeof(WMI_DELETE_PSTREAM_CMD) },
    { OID_CAR6K_SET_WMM, sizeof(WMI_SET_WMM_CMD) },
    { OID_CAR6K_SET_TXOP, sizeof(WMI_SET_WMM_TXOP_CMD) },
    { OID_CAR6K_SET_BT_STATUS, sizeof (WMI_SET_BT_STATUS_CMD) },
    { OID_CAR6K_SET_BT_PARAMS, sizeof (WMI_SET_BT_PARAMS_CMD) },
    { OID_CAR6K_SET_ROAM_CTRL, sizeof (WMI_SET_ROAM_CTRL_CMD) },
    { OID_CAR6K_IBSS_CHANNEL, sizeof (USHORT) },
    { OID_CAR6K_SET_ATIM, sizeof (UCHAR) },
    { OID_CAR6K_SET_IBSSPM, sizeof (WMI_IBSS_PM_CAPS_CMD) },
    { OID_CAR6K_SET_PM, sizeof (WMI_POWER_PARAMS_CMD) },
    { OID_CAR6K_DUMP_HTC, 0 },
    { OID_CAR6K_SET_TXPOWER, sizeof (WMI_SET_TX_PWR_CMD) },
    { OID_CAR6K_SET_SCAN_PARAMS, sizeof(WMI_SCAN_PARAMS_CMD) },
    { OID_CAR6K_SET_CHANNEL_PARAMS, sizeof(WMI_CHANNEL_PARAMS_CMD) },
    { OID_CAR6K_SET_WPS_ENABLE, sizeof(A_INT32) },
    { OID_CAR6K_ADD_KRK, offsetof(NDIS_802_11_KEY, KeyMaterial[0]) },
    { OID_CAR6K_802_11_AUTH_ALG, sizeof(CAR6K_802_11_AUTH_ALG) },
    { OID_CAR6K_802_11_CCKM_AUTHENTICATION_MODE, sizeof(CAR6K_802_11_CCKM_AUTHENTICATION_MODE) },
    { OID_CAR6K_ABORT_SCAN, 0},
    { OID_CAR6K_RSSI_THRESHOLD, sizeof(USER_RSSI_PARAMS) },
#ifdef WAPI_ENABLE
    { OID_CAR6K_SET_ASSOCINFO,               sizeof(WMI_SET_ASSOC_INFO_CMD) },
    { OID_CAR6K_SET_WAPIMODE,                sizeof(int) },
    { OID_CAR6K_SET_WAPIKEY,                 sizeof(WPI_KEY) },
    { OID_CAR6K_SET_APPIE, sizeof(WMI_SET_APPIE_CMD) },
#endif /* WAPI_ENABLE */
    { OID_CAR6K_SET_APMODE, sizeof(WMI_CONNECT_CMD) }, // soft ap set oids
    { OID_CAR6K_SET_STAMODE,0},
    { OID_CAR6K_SET_NUM_STA,sizeof(WMI_AP_SET_NUM_STA_CMD)},
    { OID_CAR6K_ADD_DEL_ACL,sizeof(WMI_AP_ACL_MAC_CMD)},
    { OID_CAR6K_SET_HIDDEN_SSID, sizeof (WMI_AP_HIDDEN_SSID_CMD)},
    { OID_CAR6K_SET_BG_PROTECTION, sizeof (WMI_AP_PROT_SCAN_TIME_CMD)},
    { OID_CAR6K_SET_DTIM, sizeof (WMI_AP_SET_DTIM_CMD)},
    { OID_CAR6K_SET_RD, sizeof (WMI_SET_COUNTRY_CMD)},
    { OID_CAR6K_SET_APMODE_XTN, sizeof(AP_COMMIT_XTND) },
    { OID_CAR6K_SET_CONN_INACT_TIME,sizeof(WMI_AP_CONN_INACT_CMD)},
    { OID_CAR6K_REMOVE_STA,sizeof(WMI_AP_SET_MLME_CMD)},
    { OID_CAR6K_SET_CHANNEL, sizeof (AP_SET_CHANNEL_CMD)},
    { OID_CAR6K_SET_BCON_INTVL , sizeof(WMI_BEACON_INT_CMD)},
    { OID_CAR6K_SET_IP, sizeof(WMI_SET_IP_CMD)},
    { OID_CAR6K_SET_SECURITY, sizeof(AP_MODE_CRYPT)},
    { OID_CAR6K_SET_ACL_POLICY, sizeof (WMI_AP_ACL_POLICY_CMD)},
    { OID_CAR6K_SET_INTRABSS, sizeof (AP_SET_INTRA_BSS)},
    { OID_CAR6K_SET_FIX_RATES, sizeof(WMI_FIX_RATES_CMD) },
    { OID_CAR6K_LISTEN_INTVL,  sizeof(WMI_LISTEN_INT_CMD)},
    { OID_CAR6K_SET_NODEAGE, sizeof(A_UINT32) },
    { OID_CAR6K_SET_KEEPALIVE_INTV, sizeof(A_UINT8) },
    { 0, 0 }
};

NDIS_STATUS
CAR6KMini::SetInformation(
    IN  NDIS_OID Oid,
    IN  PVOID    Buffer,
    IN  ULONG    cbBuffer,
    OUT PULONG   pcbRead,
    OUT PULONG   pcbNeeded)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;


    // Check that OID is supported and buffer size is reasonable.
    Status = CheckOidRequest(&g_Car6kSupportedSetOids[0], Oid, cbBuffer, pcbNeeded);
    if (NDIS_STATUS_INVALID_OID == Status)
    {
        // OID is not a supported CAR6K OID. Check to see if it is an 802_11 OID.
        Status = C802_11Miniport::SetInformation(Oid, Buffer, cbBuffer, pcbRead, pcbNeeded);
        goto done;
    }

    if (NDIS_STATUS_SUCCESS != Status)
        goto done;

    switch (Oid)
    {

        case OID_CAR6K_RESUME_WLAN_STATE:
            Status = SetResumeWlanState(*(ULONG *)Buffer);
            break;

        case OID_CAR6K_BMI_DONE:
            Status = SetCAr6kBmiDone(Buffer, FALSE);
            break;

        case OID_CAR6K_BMI_READ_MEMORY:
            Status = SetCAr6kBmiReadMemory((BMI_READ_MEMORY_PARAM *)Buffer);
            break;

        case OID_CAR6K_BMI_WRITE_MEMORY:
            Status = SetCAr6kBmiWriteMemory((BMI_WRITE_MEMORY_PARAM *)Buffer);
            break;

        case OID_CAR6K_BMI_EXECUTE:
            Status = SetCAr6kBmiExecute((BMI_EXECUTE_PARAM *)Buffer);
            break;

        case OID_CAR6K_BMI_SET_APP_START:
            Status = SetCAr6kBmiSetAppStart((BMI_SET_APP_START_PARAM *)Buffer);
            break;

        case OID_CAR6K_BMI_READ_SOC_REGISTER:
            Status = SetCAr6kBmiReadSocRegister((BMI_READ_SOC_REG_PARAM *)Buffer);
            break;

        case OID_CAR6K_BMI_WRITE_SOC_REGISTER:
            Status = SetCAr6kBmiWriteSocRegister((BMI_WRITE_SOC_REG_PARAM *)Buffer);
            break;

        case OID_CAR6K_BMI_ROMPATCH_INSTALL:
            Status = SetCAr6kBmiRomPatchInstall((BMI_ROMPATCH_INSTALL_PARAM *)Buffer);
            break;

        case OID_CAR6K_BMI_ROMPATCH_UNINSTALL:
            Status = SetCAr6kBmiRomPatchUninstall((BMI_ROMPATCH_UNINSTALL_PARAM *)Buffer);
            break;

        case OID_CAR6K_BMI_ROMPATCH_ACTIVATE:
            Status = SetCAr6kBmiRomPatchActivate((BMI_ROMPATCH_ACTIVATE_PARAM *)Buffer);
            break;

        case OID_CAR6K_BMI_ROMPATCH_DEACTIVATE:
            Status = SetCAr6kBmiRomPatchDeactivate((BMI_ROMPATCH_DEACTIVATE_PARAM *)Buffer);
            break;

#ifdef CONFIG_HOST_TCMD_SUPPORT
            case OID_CAR6K_TCMD_CONT_TX:
            Status = SetCAr6kTcmdContTx((TCMD_CONT_TX*)Buffer);
            break;
            case OID_CAR6K_TCMD_CONT_RX:
            Status = SetCAr6kTcmdContRx((TCMD_CONT_RX*)Buffer);
            break;
            case OID_CAR6K_TCMD_PM:
            Status = SetCAr6kTcmdPm((TCMD_PM*)Buffer);
            break;
#endif //CONFIG_HOST_TCMD_SUPPORT

        case OID_CAR6K_SET_HOST_SLEEP_MODE:
            Status = SetCAr6kHostSleepMode((WMI_SET_HOST_SLEEP_MODE_CMD*)Buffer);
            break;

        case OID_CAR6K_SET_WOW_MODE:
            Status = SetCAr6kWowMode((WMI_SET_WOW_MODE_CMD*)Buffer);
            break;

        case OID_CAR6K_ADD_WOW_PATTERN:
            Status = SetCAr6kWowAddPattern((WMI_ADD_WOW_PATTERN_CMD*)Buffer);
            break;

        case OID_CAR6K_DEL_WOW_PATTERN:
            Status = SetCAr6kWowDelPattern((WMI_DEL_WOW_PATTERN_CMD*)Buffer);
            break;

        case OID_CAR6K_DBGLOG_CFG_MODULE:
            Status = SetCAr6kDbgLogCfgModule((WMIX_DBGLOG_CFG_MODULE_CMD *)Buffer);
            break;
        case OID_CAR6K_CREATE_QOS:
            Status = SetCAr6kCreateQOS((WMI_CREATE_PSTREAM_CMD*)Buffer);
            break;

        case OID_CAR6K_DELETE_QOS:
            Status = SetCAr6kDeleteQOS((WMI_DELETE_PSTREAM_CMD*)Buffer);
            break;

        case OID_CAR6K_SET_WMM:
            Status = SetCAr6kWMM((WMI_SET_WMM_CMD*)Buffer);
            break;

        case OID_CAR6K_SET_TXOP:
            Status = SetCAr6kTXOP((WMI_SET_WMM_TXOP_CMD*)Buffer);
            break;
        case OID_CAR6K_SET_BT_STATUS:
            Status = SetBTStatus((WMI_SET_BT_STATUS_CMD *)Buffer);
            break;
        case OID_CAR6K_SET_BT_PARAMS:
            Status = SetBTParams((WMI_SET_BT_PARAMS_CMD *)Buffer);
            break;
         case OID_CAR6K_SET_ROAM_CTRL:
             Status = SetRoamCtrlParams((WMI_SET_ROAM_CTRL_CMD *)Buffer);
            break;
        case OID_CAR6K_IBSS_CHANNEL:
            Status = SetCAr6kIbssChannel(*(USHORT *)Buffer);
            break;
         case OID_CAR6K_SET_ATIM:
            Status = SetCAr6kAtimWindow(*(BYTE *)Buffer);
            break;
         case OID_CAR6K_SET_IBSSPM:
            Status = SetCAr6kIBSSPm((WMI_IBSS_PM_CAPS_CMD *)Buffer);
            break;
         case OID_CAR6K_SET_PM:
            Status = SetCAr6kPm((WMI_POWER_PARAMS_CMD *)Buffer);
            break;
         case OID_CAR6K_SET_APPIE:
            Status = SetCAR6kAppIE((WMI_SET_APPIE_CMD *)Buffer);
            break;
         case OID_CAR6K_SET_FIX_RATES:
            Status = SetCAR6kFixRates((WMI_FIX_RATES_CMD *)Buffer);
            break;
         case OID_CAR6K_DUMP_HTC:
            HTCDumpCreditStates(m_pHTCTarget);
#ifdef HTC_EP_STAT_PROFILING
            {
                HTC_ENDPOINT_STATS stats;

                /* for now just dump best effort */
                if (HTCGetEndpointStatistics(m_pHTCTarget,
                                             arAc2EndpointID(WMM_AC_BE),
                                             HTC_EP_STAT_SAMPLE_AND_CLEAR,
                                             &stats)) {

                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "---- \r\nProfiling Endpoint : %d \r\n"), arAc2EndpointID(WMM_AC_BE));
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditLowIndications : %d \r\n"), stats.TxCreditLowIndications);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxIssued : %d \r\n"), stats.TxIssued);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditRpts : %d \r\n"), stats.TxCreditRpts);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditsRptsFromRx : %d \r\n"), stats.TxCreditRptsFromRx);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditsRptsFromOther : %d \r\n"), stats.TxCreditRptsFromOther);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditsRptsFromEp0 : %d \r\n"), stats.TxCreditRptsFromEp0);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditsFromRx : %d \r\n"), stats.TxCreditsFromRx);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditsFromOther : %d \r\n"), stats.TxCreditsFromOther);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditsFromEp0 : %d \r\n"), stats.TxCreditsFromEp0);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditsConsummed : %d \r\n"), stats.TxCreditsConsummed);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "TxCreditsReturned : %d \r\n"), stats.TxCreditsReturned);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "RxReceived : %d \r\n"), stats.RxReceived);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "RxLookAheads : %d \r\n"), stats.RxLookAheads);
                    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "---- \r\n");

                }
            }
#endif

            break;
         case OID_CAR6K_SET_TXPOWER:
            Status = ar6000_set_target_txPwr((WMI_SET_TX_PWR_CMD *)Buffer);
            break;

        case OID_CAR6K_802_11_AUTH_ALG:
        case OID_CAR6K_802_11_AUTH_ALG_1:
            Status = SetCAr6k802_11AuthAlg(*(CAR6K_802_11_AUTH_ALG *)Buffer);
            break;

        case OID_CAR6K_ADD_KRK:
            Status = Set802_11AddKey((NDIS_802_11_KEY *)Buffer, TRUE);
            break;

        case OID_CAR6K_802_11_CCKM_AUTHENTICATION_MODE:
            Status = SetCAr6k802_11CCKMAuthMode(*(CAR6K_802_11_CCKM_AUTHENTICATION_MODE *)Buffer);
            break;

        case OID_CAR6K_SET_WPS_ENABLE:
            m_WPSEnable = *(int *)Buffer;
            Status = SetWPS(m_WPSEnable);
            break;

        case OID_CAR6K_SET_SCAN_PARAMS:
            Status = Set802_11ScanParams((WMI_SCAN_PARAMS_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_CHANNEL_PARAMS:
            Status = Set802_11ChannelParams((WMI_CHANNEL_PARAMS_CMD *)Buffer);
            break;

        case OID_CAR6K_ABORT_SCAN:
             Status = ar6000_abort_scan();
             break;

        case OID_CAR6K_RSSI_THRESHOLD:
            Status = Set802_11_rssi_threshold((USER_RSSI_PARAMS *)Buffer);
            break;

        case OID_CAR6K_LISTEN_INTVL:
            Status = Set_car6k_listen_intvl ((WMI_LISTEN_INT_CMD *)Buffer);
            break;

#ifdef WAPI_ENABLE
        case OID_CAR6K_SET_ASSOCINFO:
            Status = Set802_11AssocInfo((WMI_SET_ASSOC_INFO_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_WAPIMODE:
            Status = SetWapiMode(*(int *)Buffer);
            break;

        case OID_CAR6K_SET_WAPIKEY:
            Status = SetWapiKey((WPI_KEY*)Buffer);
            break;
#endif /* WAPI_ENABLE */

        case OID_CAR6K_SET_APMODE:
            Status = Set802_11_apmode((WMI_CONNECT_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_APMODE_XTN:
            Status = Set802_11_apmode_xtn((AP_COMMIT_XTND *)Buffer);
            break;


        case OID_CAR6K_SET_CONN_INACT_TIME:
            Status = Set802_11_conn_inact_time((WMI_AP_CONN_INACT_CMD *)Buffer);
            break;


         case OID_CAR6K_REMOVE_STA:
             Status = Set802_11_remove_sta((WMI_AP_SET_MLME_CMD *)Buffer);
            break;


        case OID_CAR6K_SET_STAMODE:
            Status = Set802_11_stamode();
            break;

         case OID_CAR6K_SET_NUM_STA:
            Status = Set802_11_numofsta((WMI_AP_SET_NUM_STA_CMD *)Buffer);
            break;

        case OID_CAR6K_ADD_DEL_ACL:
            Status = Set802_11_adddel_acl((WMI_AP_ACL_MAC_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_HIDDEN_SSID:
            Status = Set802_11_HiddenSsid((WMI_AP_HIDDEN_SSID_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_BG_PROTECTION:
            Status = Set802_11_BGProtection((WMI_AP_PROT_SCAN_TIME_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_DTIM:
            Status = Set802_11_DTIM((WMI_AP_SET_DTIM_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_RD:
            Status = Set802_11_RegDomain((WMI_SET_COUNTRY_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_CHANNEL:
            Status = Set802_11_Channel ((AP_SET_CHANNEL_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_BCON_INTVL:
            Status = Set802_11_bcon_intvl ((WMI_BEACON_INT_CMD *)Buffer);
            break;

         case OID_CAR6K_SET_SECURITY:
            Status = Set_car6k_security ((AP_MODE_CRYPT *)Buffer);
            break;

        case OID_CAR6K_SET_IP:
            Status = Set_car6k_ip ((WMI_SET_IP_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_ACL_POLICY:
            Status = Set_car6k_acl_policy ((WMI_AP_ACL_POLICY_CMD *)Buffer);
            break;

        case OID_CAR6K_SET_INTRABSS:
            Status = Set_car6k_intrabss ((AP_SET_INTRA_BSS *)Buffer);
            break;

        case OID_CAR6K_SET_NODEAGE:
            Status = SetCAR6kWMINodeAge(*(A_UINT32 *) Buffer);
            break;

        case OID_CAR6K_SET_KEEPALIVE_INTV:
            Status = SetCAR6kWMIKeepAliveInt(*(A_UINT8 *) Buffer);
            break;

        default:
            ASSERT(FALSE);
            Status = NDIS_STATUS_INVALID_OID;
            break;
    }

done:
    return Status;
}

///////////// OID_GEN Query Handlers ////////////////////


PNDIS_OID
CAR6KMini::GetSupportedOidList(
    OUT     PNDIS_OID  pOidBuffer,
    IN  OUT PULONG     pMaxOidsToAddToBuffer,
    IN  OUT PULONG     pTotalNumberOfSupportedOids)
//
//  Add the OIDs supported by the driver to pOidBuffer, but
//  do not add more than MaxOidsToAddToBuffer.
//
//  TotalNumberOfSupportedOids is set to the number of Oids
//  that the driver supports.
//
//  Returns a pointer to the next unused slot in pOidBuffer
//  after the oids have been added.
//
{
    PNDIS_OID   p_Car6kOidListStart;
    ULONG       OidListCount = 0;

    // First fill in base class supported OIDs
    p_Car6kOidListStart = C802_11Miniport::GetSupportedOidList(pOidBuffer, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);

    // Now append CAR6K OIDs
    AddOidsToList(&g_Car6kSupportedQueryOids[0], p_Car6kOidListStart, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);
    AddOidsToList(&g_Car6kSupportedSetOids[0],   p_Car6kOidListStart, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);

    return p_Car6kOidListStart + OidListCount;
}

ULONG
CAR6KMini::GetGenVendorDriverVersion()
{
    return ATH_SW_VER_MAJOR << 16 | ATH_SW_VER_MINOR;
}

ULONG
CAR6KMini::GetGenMacOptions()
// A bitmask that defines optional properties of the underlying driver or its NIC.
{
    return NDIS_MAC_OPTION_NO_LOOPBACK;
}

NDIS_HARDWARE_STATUS
CAR6KMini::GetGenHardwareStatus()
// The current hardware status of the underlying NIC
{
    NDIS_HARDWARE_STATUS HardwareStatus;

    if (!m_WMIReady)
        HardwareStatus =  NdisHardwareStatusInitializing;
    else
        HardwareStatus = NdisHardwareStatusReady;

    return HardwareStatus;
}

NDIS_MEDIA_STATE
CAR6KMini::GetGenMediaConnectStatus()
// Whether the media is currently connected and able to Tx/Rx packets.
{
    NDIS_MEDIA_STATE MediaState;

    if (m_Connected)
        MediaState = NdisMediaStateConnected;
    else
        MediaState = NdisMediaStateDisconnected;

    return MediaState;
}

ULONG
CAR6KMini::GetGenLinkSpeed()
// Link speed in units of 100 bps.
{
    //
    // since we support either a or g, otherwise both
    // and the link speed should contain only
    // 54mbps, i.e maximum as per MSFT defined
    //
    switch (m_80211PhyCapability)
    {
        case mode_11aOnly:
            break;
        case mode_11g:
            break;
        case mode_11ag:
            break;
        default:
            break;
    }
    return m_LinkrateKpbs * 10;
}

NDIS_STATUS
CAR6KMini::GetBitRate (CAR6K_GET_BITRATE *pBitRate)
{
    A_STATUS    wmiStatus  = A_OK;
    NDIS_STATUS ndisStatus = NDIS_STATUS_ADAPTER_NOT_READY;

    do
    {
        //
        // Reset the input buffer
        //
        pBitRate->ul_txRate = 0;
        pBitRate->ul_rxRate = 0;

        if (!m_WMIReady)
        {
            break;
        }

        if (!m_Connected)
        {
            break;
        }

        // get stats from target
        if (ar6000_get_target_stats() != A_OK)
        {
            ndisStatus = NDIS_STATUS_FAILURE;
            break;
        }

        pBitRate->ul_txRate = m_txRate;
        pBitRate->ul_rxRate = m_rxRate;

        ndisStatus = NDIS_STATUS_SUCCESS;

     }while (FALSE);

    return ndisStatus;
}

NDIS_STATUS
CAR6KMini::GetWmode(CAR6K_GET_WMODE *pMode)
{
    A_STATUS    wmiStatus  = A_OK;
    NDIS_STATUS ndisStatus = NDIS_STATUS_ADAPTER_NOT_READY;
    pMode->wmode = 0;

    do
    {

        pMode->wmode = 0;

        if (!m_WMIReady)
        {
            break;
        }

        pMode->wmode = m_queryphyMode;
        ndisStatus = NDIS_STATUS_SUCCESS;

     }while (FALSE);

    return ndisStatus;
}

ULONG
CAR6KMini::GetGenTransmitBufferSpace()
// The amount of memory, in bytes, on the NIC that is available for buffering Tx data.
{
    return 1512 * 16;
}

ULONG
CAR6KMini::GetGenReceiveBufferSpace()
// The amount of memory, in bytes, on the NIC that is available for buffering Rx data.
{
    return 1512 * 16;
}

ULONG
CAR6KMini::GetGenTransmitBlockSize()
// Minimum amount of storage, in bytes, that a single packet
// occupies in the Tx buffer space of the NIC.
{
    return 1500;
}

ULONG
CAR6KMini::GetGenReceiveBlockSize()
// Minimum amount of storage, in bytes, that a single packet
// occupies in the Rx buffer space of the NIC.
{
    return 1500;
}

#define ATHEROS_IEEE_VENDOR_CODE           0x00037F
#define ATHEROS_AR6001_NIC_CODE       0x00 // Placeholder

ULONG
CAR6KMini::GetGenVendorId()
// The OID_GEN_VENDOR_ID OID specifies a three-byte IEEE-registered vendor code,
// followed by a single byte that the vendor assigns to identify a particular NIC.
// Vendors without an IEEE registered code should use 0xFFFFFF for the vendor code.
{
    return (ATHEROS_IEEE_VENDOR_CODE << 8) | ATHEROS_AR6001_NIC_CODE;
}

const char *
CAR6KMini::GetGenVendorDescription()
// The OID_GEN_VENDOR_DESCRIPTION OID points to a zero-terminated string describing the NIC.
{
    return "Atheros AR6001 802.11 Network Adapter";
}

USHORT
CAR6KMini::GetGenDriverVersion()
// The NDIS version in use by the NIC driver.
// The high byte is the major version number; the low byte is the minor version number.
{
    return (m_NdisDriverMajorVersion << 8) | m_NdisDriverMinorVersion;
}

ULONG
CAR6KMini::GetSupportedPacketTypes()
// Return the types of packets supported by the miniport,
// used in conjuction with OID_GEN_CURRENT_PACKET_FILTER requests.
{
    return NDIS_PACKET_TYPE_ALL_MULTICAST
         | NDIS_PACKET_TYPE_ALL_LOCAL
         | NDIS_PACKET_TYPE_BROADCAST
         | NDIS_PACKET_TYPE_DIRECTED
         | NDIS_PACKET_TYPE_MULTICAST
         ;
}

ULONG
CAR6KMini::GetGenXmitOk()
// Number of packets transmitted ok.
{
    return 0;
}

ULONG
CAR6KMini::GetGenRcvOk()
// Number of packets received ok.
{
    return 0;
}

ULONG
CAR6KMini::GetGenXmitError()
// Number of transmitted packets that had errors.
{
    return 0;
}

ULONG
CAR6KMini::GetGenRcvError()
// Number of received packets that had FCS/framing errors.
{
    return 0;
}

NDIS_PHYSICAL_MEDIUM
CAR6KMini::GetGenPhysicalMedium()
// The maximum number of multicast addresses the NIC driver can manage.
{
    //
    // If m_WPSEnable is set - medium is unspecified so that wzc is disabled
    // wzc should be disabled during wps protocol,
    // otherwise wps can not get access to NDISUIO
    //
    if(m_networkType == AP_NETWORK)
    {
        return NdisPhysicalMediumUnspecified;
    }

    if(m_WPSEnable)
    {
        return NdisPhysicalMediumUnspecified;
    }

    if(0x00 == m_Config.tcmd)
    {
        return NdisPhysicalMediumWirelessLan;
    }

    return NdisPhysicalMediumUnspecified;

}

ULONG
CAR6KMini::GetGenRcvNoBuffer()
// Number of packets that could not be received because no buffer space was available.
{
    return 0;
}

void
CAR6KMini::GetPnpCapabilities(
    OUT NDIS_PNP_CAPABILITIES *pCaps
    )
{
    // Wake on wireless event and power mgmt are supported
    PNDIS_PM_WAKE_UP_CAPABILITIES pWakeCaps = &pCaps->WakeUpCapabilities;

    // Wake up events are supported when the device is in D0,D1,D2 or D3 states. Even
    // when the host device goes to the lowest power state, the wlan chip can wake up
    // the host if host enables this capability in the chip.

    if (m_Config.wowEnable) {
        pWakeCaps->MinMagicPacketWakeUp = NdisDeviceStateD3;
        pWakeCaps->MinPatternWakeUp     = NdisDeviceStateD3;
        pWakeCaps->MinLinkChangeWakeUp  = NdisDeviceStateD3;
    } else {
        pWakeCaps->MinMagicPacketWakeUp = NdisDeviceStateUnspecified;
        pWakeCaps->MinPatternWakeUp     = NdisDeviceStateUnspecified;
        pWakeCaps->MinLinkChangeWakeUp  = NdisDeviceStateUnspecified;
    }

    return;
}

NDIS_STATUS
CAR6KMini::QueryPnpPower(
        IN NDIS_DEVICE_POWER_STATE *pDevPowerState
        )
{
    NDIS_STATUS Status;

    if (*pDevPowerState == NdisDeviceStateD3) {
        Status = NDIS_STATUS_SUCCESS;
    } else {
        Status = NDIS_STATUS_NOT_SUPPORTED;
    }

    return Status;
}


BYTE *
CAR6KMini::Get802_3PermanentAddress()
// The address of the NIC encoded in the hardware.
{
    return &m_PermanentAddress[0];
}

BYTE *
CAR6KMini::Get802_3CurrentAddress()
// The address the NIC is currently using.
{
    return &m_CurrentAddress[0];
}


ULONG
CAR6KMini::Get802_3MacOptions()
{
    return 0;
}

ULONG
CAR6KMini::Get802_3RcvErrorAlignment()
// Number of received packets that had alignment errors
{
    return 0;
}

ULONG
CAR6KMini::Get802_3XmitOneCollision()
// Number of transmitted packets that were transmitted after one collision
{
    // Since AR6K is a wireless NIC and not using the Ethernet physical layer
    // there are no collisions.
    return 0;
}

NDIS_STATUS
CAR6KMini::GetWowList(WMI_GET_WOW_LIST_REPLY *Buffer, ULONG cbBuffer, PULONG pcbNeeded)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_STATUS wmiStatus;
    WMI_GET_WOW_LIST_CMD wmiGetWowListCmd;

    NdisResetEvent(&m_tgtWowListReplyEvent);

    wmiGetWowListCmd.filter_list_id = 0; //default filter list id
    wmiStatus = wmi_get_wow_list_cmd((wmi_t *)m_pWMI, &wmiGetWowListCmd);

    if (wmiStatus != A_OK) {
        return NDIS_STATUS_FAILURE;
    }

    if ( NdisWaitEvent(&m_tgtWowListReplyEvent, 10000) != TRUE ) {

            return NDIS_STATUS_FAILURE;
         }

    if (!m_WowListReply) {

        return NDIS_STATUS_FAILURE;
    }

    if (m_WowListReply->num_filters > 1) {
        *pcbNeeded = sizeof(WMI_GET_WOW_LIST_REPLY) + (m_WowListReply->num_filters - 1) * sizeof(WOW_FILTER);
    } else {
        *pcbNeeded = sizeof(WMI_GET_WOW_LIST_REPLY);
    }

    if (cbBuffer < *pcbNeeded) {
        // Free the buffer allocated in WMIWowListEvent callback.
        A_FREE(m_WowListReply);
        m_WowListReply = NULL;
        return NDIS_STATUS_INVALID_LENGTH;
    }

    /* Copy information into the Buffer */
    memcpy((A_UINT8 *)Buffer, (A_UINT8 *)(m_WowListReply), sizeof(WMI_GET_WOW_LIST_REPLY) );

    if (m_WowListReply->num_filters > 1) {
        memcpy((A_UINT8 *)Buffer + sizeof(WMI_GET_WOW_LIST_REPLY), (A_UINT8 *)(m_WowListReply)+sizeof(WMI_GET_WOW_LIST_REPLY), (m_WowListReply->num_filters-1)*sizeof(WOW_FILTER) );
    }

    // Free the buffer allocated in WMIWowListEvent callback.
    A_FREE(m_WowListReply);
    m_WowListReply = NULL;
    return Status;
}

ULONG
CAR6KMini::Get802_3XmitMoreCollision()
// Number of transmitted packets that were transmitted after more than one collision
{
    // Since AR6K is a wireless NIC and not using the Ethernet physical layer
    // there are no collisions.
    return 0;
}

void CAR6KMini::ChangeCurrentPowerState (DWORD data)
{
    HKEY handle = NULL;
    NDIS_STATUS    Status = NDIS_STATUS_SUCCESS;

    NDIS_STRING  SysRstPwrStateStr = NDIS_STRING_CONST("currentPowerState");

    NDIS_CONFIGURATION_PARAMETER ParameterValue;

    ParameterValue.ParameterData.IntegerData = data;
    ParameterValue.ParameterType = NdisParameterInteger;

    NdisWriteConfiguration (
                    &Status,
                    m_ConfigHandle,
                    &SysRstPwrStateStr,
                    &ParameterValue);

    if (Status != NDIS_STATUS_SUCCESS)
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "ChangeCurrentPowerState Failed\n");
    }

}

NDIS_STATUS
 CAR6KMini::SetPnpPower(
         IN NDIS_DEVICE_POWER_STATE *pDevPowerState
         )
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    WCHAR       sysPwrStateName[32];
    DWORD       sysPwrState = POWER_STATE_ON;
    DWORD       result;
    ULONG       pwrMgmtMode;
    WMI_SET_HOST_SLEEP_MODE_CMD hostSleepMode;
    WMI_SET_WOW_MODE_CMD wowMode = {FALSE};

    // Already in the requested power state
    if (m_PowerState == *pDevPowerState) {
        return NDIS_STATUS_SUCCESS;
    }

#ifdef UNDER_CE
    // Don't call the Power Manager APIs directly.
    // Doing so assumes the PM is sysgen'ed into every image.
    typedef DWORD GetSystemPowerStateProto(LPWSTR, DWORD, PDWORD);
    GetSystemPowerStateProto *pfnGetSystemPowerState = NULL;

    HMODULE hCoreDll = (HMODULE) LoadLibrary(TEXT("coredll.dll"));
    if (NULL != hCoreDll) {
        pfnGetSystemPowerState = (GetSystemPowerStateProto *)
            GetProcAddress(hCoreDll, TEXT("GetSystemPowerState"));
    }

    if (NULL == pfnGetSystemPowerState) {
        result = ERROR_SUCCESS;
    } else {
        result = pfnGetSystemPowerState(sysPwrStateName, sizeof(sysPwrStateName) / sizeof(WCHAR), &sysPwrState);
    }

    if (NULL != hCoreDll) {
        FreeLibrary(hCoreDll);
    }
#else /*!UNDER_CE*/
    result = GetSystemPowerState(sysPwrStateName, sizeof(sysPwrStateName) / sizeof(WCHAR), &sysPwrState);
#endif /*UNDER_CE*/

    if (result != ERROR_SUCCESS)
    {
        return NDIS_STATUS_FAILURE;
    }

    // Coming out of standby and the resume state is off, keep
    // the WLAN module in off state by failing the OID.
    if ((!(m_LastSysState & POWER_STATE_ON)) &&
        (m_ResumeWlanState == 0) &&
        (*pDevPowerState == NdisDeviceStateD0))
    {
        m_LastSysState = sysPwrState;
        return NDIS_STATUS_FAILURE;
    }
    m_LastSysState = sysPwrState;

    /* The following depicts the power state transitions that happen when NDIS sets the device state.
     *
     *    D0 - Device is fully powered
     *    D1 - power save state (Standby)
     *    D2 - low power state (Idle)
     *    D3 - Lowest power state
     *    D4 - Power off
     *
     * If host wake up is enabled, we dont power cycle the chip anymore when
     * transitioning between D1,D2 and D3 states. In this case, the chip needs to be
     * awake to wake up the host. If host wake up is not enabled we can power cycle the
     * chip as we did previous to host wake up implementation...?
     * Also need to determine if the host has to put the target in REC Mode when it
     * transitions to D3 state. This would enable power save at the target & that wont
     * drain the host power.
     */

    if (*pDevPowerState == NdisDeviceStateD3)
    {
        if (m_Config.suspendDelay)
        {
            // The below delay in Seconds controlled by the registry
            // Customer can configure non-zero value, so that they can delay this flow
            // and the default value is zero.
            // WaitForSingleObject (m_PowerNotifyEvent, (m_Config.suspendDelay * 1000));
            //
            Sleep (m_Config.suspendDelay * 1000);
        }

        if (!m_WMIReady)
        {
            Status = NDIS_STATUS_SUCCESS;
        }
        else
        {
            if (sysPwrState == POWER_STATE_SUSPEND)
            {
                m_bIsSuspend = TRUE;
            }
            else
            {
                m_bIsWiFiOnOff = TRUE;
                ChangeCurrentPowerState (0x0);
            }
			
#ifdef SUPPORT_UMA_WCM_LIB
            if (m_bIsWiFiOnOff && !m_bIsSuspend) {
                        HANDLE preOff = CreateEvent (NULL, FALSE, FALSE, L"ATHRWLANNOTIFYOFF");
                        if (preOff) {
                              DWORD dwRetCode  = GetLastError ();
                            if (ERROR_ALREADY_EXISTS == dwRetCode) {
                                    HANDLE confirmOff = CreateEvent (NULL, FALSE, FALSE, L"ATHRWLANCONFIRMOFF");
                                    if (confirmOff) {
                                        RETAILMSG(1, (TEXT("Notify wifi off\n")));
                                        ResetEvent(confirmOff);
                                        SetEvent(preOff);
                                        dwRetCode = WaitForSingleObject(confirmOff, 3000);
                                        switch (dwRetCode) {
                                        case WAIT_OBJECT_0:
                                            RETAILMSG(1, (TEXT("Wait for confirm successfully\n")));
                                            break;
                                        case WAIT_TIMEOUT:
                                              /* fall through */
                                        default:
                                            RETAILMSG(1, (TEXT("Wait for confirm error %d\n"), dwRetCode));
                                            break;
                                        }
                                        CloseHandle(confirmOff);
                                    }
                              }
                              CloseHandle(preOff);
                        }
                    }
#endif // SUPPORT_UMA_WCM_LIB
            /*
             * If the user switches off WiFi from the Wireless LAN Manager,then
             * cut off power to WiFi. Do not enable WoW.This user action can be
             * got from the sysPwrState. If the host goes to Suspend mode & we
             * enterhere because of that, then sysPwrState will be
             * POWER_STATE_SUSPEND.
             * If the user switches of WiFi in the Wireless Manager, then the
             * sysPwrState will be POWER_STATE_ON.
             */
            if ((m_Config.wowEnable) &&
                (m_bIsSuspend))
            {
                /*
                 * New Implementation - Put the chip in lowest power mode,
                 * since the the driver can now Wakeup the chip.
                 * TODO: Does the tgt do these stuff when WOW is enabled
                 * and host goes to Sleep..?
                 */
                WMI_SET_HOST_SLEEP_MODE_CMD hostSleepMode = {FALSE, TRUE};
                wowMode.enable_wow = TRUE;
                wmi_set_wow_mode_cmd((wmi_t *)m_pWMI, &wowMode);
                wmi_set_host_sleep_mode_cmd((wmi_t *)m_pWMI, &hostSleepMode);
            }
            else
            {
                if (m_bIsSuspend) {
                    pwrMgmtMode = m_Config.suspendMode;
                } else {
                    pwrMgmtMode = m_Config.wifiOnOffMode;
                }

                if (TRUE == m_bIsSuspend &&
                    POWER_MANAGEMENT_MODE_CUT_PWR == pwrMgmtMode)
                {
					// to avoid connection issues during resume (#Bug 36347), below code saved the bssid info
					// and add the node after resume
					preBss = wmi_rm_current_bss ((wmi_t *)m_pWMI, preBssid);
				}

                if (m_Connected) {
                    // Flush any pending NDIS packets
                    FlushNdisPacketTransmitQueue();
                }

                if ((m_ConnectInProgress) || (m_Connected)) {
                    NdisResetEvent (&m_SuspendEvent);
                    m_WantToBeConnected = FALSE;
                    SendWMIDisconnectCommand ();
                    NdisWaitEvent (&m_SuspendEvent, NDIS_SUSPEND_TIMEOUT_INTERVAL);
                }

                if (pwrMgmtMode == POWER_MANAGEMENT_MODE_WLAN_DEEP_SLEEP) {
                    // Disable all scan and remain in low power state
                    wmi_scanparams_cmd((wmi_t *)m_pWMI,0xFFFF,0,0,0,0,0,0,0,0,0);
                    if (m_80211_PowerMode ==Ndis802_11PowerModeCAM) {
                        wmi_powermode_cmd((struct wmi_t *)m_pWMI, REC_POWER);
                    }
                    hostSleepMode.awake = FALSE;
                    hostSleepMode.asleep = TRUE;
                    wmi_set_host_sleep_mode_cmd((wmi_t *)m_pWMI, &hostSleepMode);
                } else {

                    StopEndpoints (TRUE);
                    if (pwrMgmtMode == POWER_MANAGEMENT_MODE_CUT_PWR) {
                        // Platform going to cut the power. Dont wait for completion
                        ar6000_reset_device (m_pHIFDevice, m_TargetType, FALSE);
                        HIF_DEVICE_POWER_CHANGE_TYPE powerChange = HIF_DEVICE_POWER_CUT;
                        HIFConfigureDevice(m_pHIFDevice,HIF_DEVICE_POWER_STATE_CHANGE,
                                 &powerChange, sizeof(HIF_DEVICE_POWER_CHANGE_TYPE));
                        plat_suspend ();
                    } else {
                        ar6000_reset_device (m_pHIFDevice, m_TargetType, TRUE);
                    }
                }
            }
        }
    }
    else if (*pDevPowerState == NdisDeviceStateD0)
    {
        if ((m_Config.wowEnable) &&
            (m_bIsSuspend))
        {
            /*
             * If Host Wakeup is enabled, revert the target to its original state
             * before the host going to sleep. Also Indicate the target that host
             * is now awake so that it can send all pkts up to the host.
             */
             WMI_SET_HOST_SLEEP_MODE_CMD hostSleepMode = {TRUE, FALSE};
             wmi_set_host_sleep_mode_cmd ((wmi_t *)m_pWMI, &hostSleepMode);
        }
        else
        {
            if (m_bIsSuspend) {
                pwrMgmtMode = m_Config.suspendMode;
            } else {
                pwrMgmtMode = m_Config.wifiOnOffMode;
            }

            if (pwrMgmtMode == POWER_MANAGEMENT_MODE_WLAN_DEEP_SLEEP) {
                hostSleepMode.awake = TRUE;
                hostSleepMode.asleep = FALSE;
                wmi_set_host_sleep_mode_cmd ((wmi_t *)m_pWMI, &hostSleepMode);
                if (m_80211_PowerMode ==Ndis802_11PowerModeCAM) {
                    wmi_powermode_cmd((struct wmi_t *)m_pWMI, MAX_PERF_POWER);
                }
            } else {
                A_BOOL isColdBoot = FALSE;

                if (pwrMgmtMode == POWER_MANAGEMENT_MODE_CUT_PWR) {
                    HIF_DEVICE_POWER_CHANGE_TYPE powerChange = HIF_DEVICE_POWER_UP;

                    plat_resume ();
                    HIFConfigureDevice(m_pHIFDevice,HIF_DEVICE_POWER_STATE_CHANGE,
                             &powerChange, sizeof(HIF_DEVICE_POWER_CHANGE_TYPE));
                    isColdBoot = TRUE;
                }
                Init();

                BMIInit ();

                if (m_Config.resumeDelay) {
                    NdisMSleep(m_Config.resumeDelay);
                }

                if (StartEndpoints(isColdBoot) != A_OK)
                {
                    Status = NDIS_STATUS_FAILURE;
                    logPrintf (ATH_LOG_INF, "WiFi ReEnabled and StartEndpont failed\n");
                }

                if (TRUE == m_bIsSuspend &&
                    POWER_MANAGEMENT_MODE_CUT_PWR == pwrMgmtMode)
                {
                    if (preBss != NULL)
                    {
                        // to avoid connection issues during resume (#Bug 36347), below code
                        // add the previous connected node during suspend after resume
                        wmi_add_current_bss ((wmi_t *)m_pWMI, preBssid, preBss);
                        preBss = NULL;
                    }
                }
            }
        }

        if (m_bIsSuspend) {
            m_bIsSuspend = FALSE;
        } else {
            m_bIsWiFiOnOff = FALSE;
            ChangeCurrentPowerState (0x1);
        }
    }

    if (Status == NDIS_STATUS_SUCCESS)
    {
        m_PowerState = *pDevPowerState;
    }

    // Set the event and notify the apps abt the status change
    SetEvent (m_PowerChangeEvent);

    return Status;


}

NDIS_STATUS
CAR6KMini::SetWakeupCapabilities(
    IN ULONG flags)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    WMI_SET_WOW_MODE_CMD wowMode = {FALSE};
    A_STATUS wmiStatus;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (!m_Config.wowEnable) {
        return NDIS_STATUS_NOT_SUPPORTED;
    }

    /* Check the Wakeup Capabilities flags set by NDIS. If at least one wakeup
     * pattern (magic | pattern | link status change) is enabled, enable WoW
     * at the target.
     * TODO: How to selectively enable/disable specific things (magic,pattern,
     * link change) at the target..? All 3 types are impl. as patterns.
     */

    m_WowMode = (NDIS_PNP_WAKE_UP_MAGIC_PACKET | NDIS_PNP_WAKE_UP_PATTERN_MATCH |
                    NDIS_PNP_WAKE_UP_LINK_CHANGE);

    if (m_WowMode & flags) {
        wowMode.enable_wow = TRUE;
    } else {
        wowMode.enable_wow = FALSE;
    }

    wmiStatus = wmi_set_wow_mode_cmd((wmi_t *)m_pWMI, &wowMode);
    if (wmiStatus != A_OK) {
        Status =  NDIS_STATUS_FAILURE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::AddWakeupPattern(
     IN NDIS_PM_PACKET_PATTERN *Buffer)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    WMI_ADD_WOW_PATTERN_CMD wmiAddWowPatternCmd;
    A_UINT8 *BitMask, *Pattern, *ByteMask;
    A_UINT32 count=0;
    A_STATUS wmiStatus;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (!m_Config.wowEnable) {
        return NDIS_STATUS_NOT_SUPPORTED;
    }

    /* Mask starts immediately following the NDIS_PM_PACKET_PATTERN structure
     * in the infomation Buffer of the NDIS_REQUEST packet
     */
    Pattern = (A_UINT8 *)Buffer + Buffer->PatternOffset;

    // Build the Bit Mask
    BitMask = (A_UINT8 *) A_MALLOC(Buffer->MaskSize);

    if ( !BitMask ) {
        return NDIS_STATUS_FAILURE;
    }

    A_MEMCPY(BitMask, (A_UINT8 *)Buffer + sizeof(NDIS_PM_PACKET_PATTERN),
                   Buffer->MaskSize);

    // Build the Byte Mask
    ByteMask = (A_UINT8 *) A_MALLOC(Buffer->PatternSize);

    if ( ByteMask == NULL ) {

        A_FREE(BitMask);
        return NDIS_STATUS_FAILURE;
    }

    for (count =0; count < Buffer->PatternSize; count++) {

        *(ByteMask+count) = ( *(BitMask + (count/8)) & (1<<(count%8)) )?0xFF:0x00;
    }

    // Send WMI cmd to effect this.
    //
    /* TODO: What are filter lists..? Who manages filter lists-host or tgt..?
     * pattern & mask sizes are assumed to be 64 bytes. NDIS does'nt put this
     * restriction.? We need to be generic in accepting variable size pattern/masks.
     */

    /* Add this pattern to the default list. No way to group patterns here */
    wmiAddWowPatternCmd.filter_list_id = 0; //default filter list

    // Ndis specifies masks that starts with the first byte of the packet.
    // In the tgt WLAN buffer pkt starts with the duration ID field of 2 bytes.
    // NDIS assumes start of pkt as DST. MAC Addr.
    // Hence setting offset as 2.
    wmiAddWowPatternCmd.filter_offset =  2;

    //When wifi connect to AP, Windows mobile will set two pattern - 1.MAC address, 2.part of ARP packet
    //the part of ARP pattern like
    //00 00 00 00 00 00 00 00 00 00 00 00 08 06 00 00 00 00 00 00 00 01 ... ... ip address
    //So, we check the pattern here, if the pattern is part of ARP packet
    //we should change the offset
    if( Buffer->PatternSize > 22 &&
        Pattern[12] == 0x08 &&
        Pattern[13] == 0x06 &&
        Pattern[21] == 0x01 )
    {
        wmiAddWowPatternCmd.filter_offset =  10;
    }                

    /*TODO: What should this be for diff mask/pattern sizes. */
    wmiAddWowPatternCmd.filter_size = (A_UINT8)Buffer->PatternSize;

    wmiStatus = wmi_add_wow_pattern_cmd((wmi_t *)m_pWMI, &wmiAddWowPatternCmd,
                        Pattern, ByteMask, (A_UINT8)Buffer->PatternSize );

    if (wmiStatus != A_OK) {

        Status = NDIS_STATUS_FAILURE;
    }

    A_FREE(BitMask);
    A_FREE(ByteMask);

    return Status;
}

NDIS_STATUS
CAR6KMini::RemoveWakeupPattern(
     IN NDIS_PM_PACKET_PATTERN *Buffer)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    WMI_GET_WOW_LIST_CMD wmiGetWowListCmd;
    WMI_DEL_WOW_PATTERN_CMD wmiDelWowPatternCmd;
    WOW_FILTER *pWowFilter = NULL;
    A_UINT8 *NdisPattern = (A_UINT8 *)Buffer + Buffer->PatternOffset;
    A_BOOL isPatternMatch = FALSE;
    A_BOOL isMaskMatch = FALSE;
    A_UINT8 i,j;
    A_UINT8 *BitMask, *ByteMask;
    A_UINT32 count=0;
    A_STATUS wmiStatus;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (!m_Config.wowEnable) {
        return NDIS_STATUS_NOT_SUPPORTED;
    }

    /* Need to send the filter_list_id and filter_id to delete a pattern
     * So get the pattern list for the default filter_list_id (0) and compare
     * with the ndis given pattern and pass the filter_id to delete it.
     */
    NdisResetEvent(&m_tgtWowListReplyEvent);

    wmiGetWowListCmd.filter_list_id = 0; //default filter list id
    wmiStatus=wmi_get_wow_list_cmd((wmi_t *)m_pWMI, &wmiGetWowListCmd);

    if (wmiStatus != A_OK) {

        return NDIS_STATUS_FAILURE;
    }

    /* Wait for wow_list_reply_rx from target */
    if ( NdisWaitEvent(&m_tgtWowListReplyEvent, 10000) != TRUE ) {

        return NDIS_STATUS_FAILURE;
    }

    if (!m_WowListReply) {

        return NDIS_STATUS_FAILURE;
    }

    // Build the Bit Mask
    BitMask = (A_UINT8 *) A_MALLOC(Buffer->MaskSize);

    if ( !BitMask ) {

        return NDIS_STATUS_FAILURE;
    }

    A_MEMCPY(BitMask, (A_UINT8 *)Buffer + sizeof(NDIS_PM_PACKET_PATTERN), Buffer->MaskSize);

    // Build the Byte Mask
    ByteMask = (A_UINT8 *) A_MALLOC(Buffer->PatternSize);

    if ( ByteMask == NULL ) {

        A_FREE(BitMask);
        return NDIS_STATUS_FAILURE;
    }

    for (count =0; count < Buffer->PatternSize; count++) {

         *(ByteMask+count) = ( *(BitMask + (count/8)) & (1<<(count%8)) )?0xFF:0x00;
    }

    /* Iterate through the list of filters and delete the one that matches the
     * Ndis given pattern, Mask & Offset
     */
    for(A_UINT16 filterCnt=0; filterCnt < m_WowListReply->num_filters;
                    filterCnt++) {
        pWowFilter = &m_WowListReply->wow_filters[filterCnt];
        NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Matching FilterId: %d\n", pWowFilter->wow_filter_id);

        NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Buffer->PatternSize: %d, FilterSize:%d\n",Buffer->PatternSize, pWowFilter->wow_filter_size);

        if (pWowFilter->wow_valid_filter &&
                pWowFilter->wow_filter_size == Buffer->PatternSize)
                    //remove the offset here because we might set offset to 10 for ARP packet
                    /*&&
                    pWowFilter->wow_filter_offset == 2 ) */
        {

            // Match Pattern
            for(i=0, j=0; i<pWowFilter->wow_filter_size &&
                        (pWowFilter->wow_filter_pattern[i] == NdisPattern[j]); i++,j++);
            NDIS_DEBUG_PRINTF (ATH_LOG_INF, "%x == %x\n", pWowFilter->wow_filter_pattern[i],NdisPattern[j]);
            if ( i == pWowFilter->wow_filter_size - 1 &&
                        pWowFilter->wow_filter_pattern[i] == NdisPattern[j]) {
                /* match */
                isPatternMatch = TRUE;
            }

            // Match Mask
            for(i=0, j=0; i<pWowFilter->wow_filter_size &&
                        (pWowFilter->wow_filter_mask[i] == ByteMask[j]); i++,j++);

            NDIS_DEBUG_PRINTF (ATH_LOG_INF, "%x == %x\n", pWowFilter->wow_filter_mask[i],ByteMask[j]);

            if ( i == pWowFilter->wow_filter_size - 1 &&
                        pWowFilter->wow_filter_mask[i] == ByteMask[j]) {
                /* match */
                isMaskMatch = TRUE;
            }

            if (isPatternMatch && isMaskMatch)
                break;

        } //end if
    } // end for

    if (isPatternMatch && isMaskMatch) {

        // Send WMI cmd to effect this.
        NDIS_DEBUG_PRINTF (ATH_LOG_INF, "Matching filter found: %d\n", pWowFilter->wow_filter_id);
        wmiDelWowPatternCmd.filter_list_id = 0; //default filter list id
        wmiDelWowPatternCmd.filter_id = pWowFilter->wow_filter_id;

        wmiStatus = wmi_del_wow_pattern_cmd((wmi_t *)m_pWMI, &wmiDelWowPatternCmd);
        if (wmiStatus != A_OK) {
            Status = NDIS_STATUS_FAILURE;
        }
    }
    else
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, " WOW Pattern Match :: No Match\n");
    }

    // Free the buffer allocated in WMIWowListEvent callback.
    A_FREE(m_WowListReply);
    m_WowListReply = NULL;

    A_FREE(BitMask);
    A_FREE(ByteMask);

    return Status;
}

// AR6K 802.11 OID Query Handlers

NDIS_STATUS
CAR6KMini::Get802_11AssociatedAPBSSID(
    OUT NDIS_802_11_MAC_ADDRESS *BSSID)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    // If the adapter is not associated, return NDIS_STATUS_ADAPTER_NOT_READY
    if (!m_Connected) {
        Status = NDIS_STATUS_ADAPTER_NOT_READY;
    } else {
        memcpy (BSSID, m_PeerBSSID, ETHERNET_MAC_ADDRESS_LENGTH);
    }

    return Status;
}

void
CAR6KMini::Get802_11SSID(
    OUT NDIS_802_11_SSID *pSSID)
{
    // If the adapter is not associated, return previously connected SSID, otherwise
    // return 0 length SSID
    if (0x00 == m_SSID.SsidLength)
        pSSID->SsidLength = 0;
    else
        *pSSID = m_SSID;
}

void
CAR6KMini::Get802_11NetworkTypesSupported(
    OUT NDIS_802_11_NETWORK_TYPE **ppTypes,
    OUT PULONG                     pNumTypes)
{
    *ppTypes = &g_AR6KNetworkTypesSupported[0];
    if (m_80211PhyCapability == mode_11ag)
        *pNumTypes = sizeof(g_AR6KNetworkTypesSupported) / sizeof(g_AR6KNetworkTypesSupported[0]);
    else
        *pNumTypes = 2;
}

void
CAR6KMini::Get802_11NetworkTypeInUse(
    OUT NDIS_802_11_NETWORK_TYPE *pType)
{
    *pType = m_NetworkTypeInUse;
}

NDIS_STATUS
CAR6KMini::Get802_11TxPowerLevel(
    OUT NDIS_802_11_TX_POWER_LEVEL *pLevel)
{
    A_STATUS status = A_OK;

    status = ar6000_get_target_txPwr ();

    if (A_OK != status)
    {
        return NDIS_STATUS_NOT_SUPPORTED;
    }
    *pLevel = m_TxPwr;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11RSSI(
    OUT NDIS_802_11_RSSI *pRSSI)
{
    // Return if WMI is not ready yet
    if (!m_WMIReady)
        return NDIS_STATUS_ADAPTER_NOT_READY;

    //
    // no need to send target_statistics command during disconnected state
    //
    if (FALSE == m_Connected)
        return NDIS_STATUS_ADAPTER_NOT_READY;

    // get stats from target
    if (ar6000_get_target_stats() != A_OK)
        return NDIS_STATUS_FAILURE;

    // Post the RSSI value relative to the Standard Noise floor value.
    *pRSSI = RSSI_TO_NDIS(m_RSSI);

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11RSSITrigger(
    OUT NDIS_802_11_RSSI *pTrigger)
{
   // Return if WMI is not ready yet
    if (!m_WMIReady)
        return NDIS_STATUS_ADAPTER_NOT_READY;

    //
    // no need to send target_statistics command during disconnected state
    //
    if (FALSE == m_Connected)
        return NDIS_STATUS_ADAPTER_NOT_READY;

    if (!m_rssiEvent)
        return NDIS_STATUS_FAILURE;

    *pTrigger = m_rssiEvent;

    return NDIS_STATUS_SUCCESS;
}

void
CAR6KMini::Get802_11InfrastructureMode(
    OUT NDIS_802_11_NETWORK_INFRASTRUCTURE *pMode)
{
    *pMode = m_InfrastructureMode;
}

NDIS_STATUS
CAR6KMini::Get802_11FragmentationThreshold(
    OUT NDIS_802_11_FRAGMENTATION_THRESHOLD *pThreshold)
{
    return NDIS_STATUS_NOT_SUPPORTED;
}

NDIS_STATUS
CAR6KMini::Get802_11RTSThreshold(
    OUT NDIS_802_11_RTS_THRESHOLD *pThreshold)
{
    *pThreshold = m_RTSThreshold;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11NumberOfAntennas(
    OUT ULONG *pNumberOfAntennas)
{
    return NDIS_STATUS_NOT_SUPPORTED;
}

NDIS_STATUS
CAR6KMini::Get802_11RxAntennaSelected(
    OUT NDIS_802_11_ANTENNA *pAntenna)
{
    return NDIS_STATUS_NOT_SUPPORTED;
}

NDIS_STATUS
CAR6KMini::Get802_11TxAntennaSelected(
    OUT NDIS_802_11_ANTENNA *pAntenna)
{
    return NDIS_STATUS_NOT_SUPPORTED;
}

void
CAR6KMini::Get802_11SupportedRates(
    OUT BYTE *pRates)
//
//  When queried, the OID_802_11_SUPPORTED_RATES OID requests that
//  the miniport driver return the underlying NIC's data rate set,
//  which includes the data rates that the NIC's radio supports.
//
//  Data rate sets are encoded as eight bytes. Each byte describes
//  a single rate in units of 0.5 Mbps. Rates from the IEEE 802.11
//  BSSBasicRateSet that are included in the supported rates are used,
//  for example, for control and broadcast frames.
//
//  Each supported rate from the BSSBasicRateSet is encoded as a byte
//  with the most significant bit (bit 7) set to 1. For example, a 1Mbps
//  rate in the BSSBasicRateSet is encoded as 0x82.
//
//  Rates that are not included in the BSSBasicRateSet are encoded with
//  the most significant bit set to zero. For example, a 2Mbps rate
//  that does not belong to the BSSBasicRateSet is encoded as 0x04.
//
//  If the device supports less than eight rates, the driver must set
//  the unused entries at the end of the array to zero.
//
//  If the device supports more than eight rates, the miniport driver returns
//  the highest eight rates.
//  EX : -
//  pRates[0] = 0x80 | 22;   // 11 Mbps
//  pRates[1] = 0x80 | 108;  // 54 Mbps

{
    memset(pRates, 0, sizeof(NDIS_802_11_RATES));

    if (m_pSupportedRates)
    {
        memcpy (pRates, m_pSupportedRates, sizeof(NDIS_802_11_RATES));
    }

}

NDIS_STATUS
CAR6KMini::Get802_11DesiredRates(
    OUT NDIS_802_11_RATES *pRates)
{
    return NDIS_STATUS_NOT_SUPPORTED;
}

void
CAR6KMini::Get802_11Configuration(
    OUT NDIS_802_11_CONFIGURATION *pConfiguration)
{
    pConfiguration->Length = sizeof(NDIS_802_11_CONFIGURATION);

    // returns a value for BeaconPeriod based on the following:
    // o If the device is in infrastructure mode and is associated, the driver returns the current beacon period of the associated access point.
    // o If the device is in ad hoc mode, the driver returns the IBSS beacon period.
    // o If the device is not associated, the driver returns 0.
    if (m_Connected)
        pConfiguration->BeaconPeriod = m_BeaconInterval;
    else
    pConfiguration->BeaconPeriod = 0;

    if (m_InfrastructureMode == Ndis802_11IBSS)
    {
        // adhoc mode
        // The announcement traffic information message (ATIM) window in Kmicroseconds (1024 microseconds).
        // The ATIM window is a short time period immediately after the transmission of each beacon in an ad hoc network.
        // During the ATIM window, any station within the ad hoc network can indicate
        // the need to transfer data to another station during the following data-transmission window.
        // If the driver's network mode is set for infrastructure networks, the driver returns 0 for ATIMWindow.
        pConfiguration->ATIMWindow = m_Config.atimWindow;
    }
    else
    {
        // Infra structure mode
        // The announcement traffic information message (ATIM) window in Kmicroseconds (1024 microseconds).
        // The ATIM window is a short time period immediately after the transmission of each beacon in an ad hoc network.
        // During the ATIM window, any station within the ad hoc
        // network can indicate the need to transfer data to another station during the following data-transmission window.
        // If the driver's network mode is set for infrastructure networks, the driver returns 0 for ATIMWindow.
        pConfiguration->ATIMWindow = 0;
    }

    if (m_Connected) {
        // DSConfig contains the current radio frequency in kHz.
        pConfiguration->DSConfig = m_ConnectedChannel * 1000;
    } else {
        pConfiguration->DSConfig = 0;
    }

    pConfiguration->FHConfig.Length     = sizeof(NDIS_802_11_CONFIGURATION_FH);
    pConfiguration->FHConfig.HopPattern = 0;
    pConfiguration->FHConfig.HopSet     = 0;
    pConfiguration->FHConfig.DwellTime     = 0;
}

NDIS_STATUS
CAR6KMini::Get802_11Statistics(
    OUT NDIS_802_11_STATISTICS *pStatistics)
{
    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    //get stats from target
    if (ar6000_get_target_stats() != A_OK)
        return NDIS_STATUS_FAILURE;

    *pStatistics = m_tgtStats;

    //
    // TODO *** TODO **** TODO
    // As Microsoft recommended, Cleanup Target State has been added
    // In order to support this, NDIS_802_11 STATISTICS structure needs to be
    // updated. So the below code has been commented out for time being
    // Note - The below code is an alternate solution of Target Statistics reset
    // Microsoft may use this or they can implement this behaviour using
    // SetInformation call.
    //
    // To implement this in Queryinformation,ClearState memebr has been
    // added in to NDIS 802_11_STATISTICS structure.
    /*
    if (0x01 == pStatistics->ClearState)
    {
        A_MEMZERO (&m_tgtStats, sizeof (NDIS_802_11_STATISTICS));
        break;
    }
    */

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::ar6000_set_target_txPwr(WMI_SET_TX_PWR_CMD *settxPwr)
{

    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    status = wmi_set_txPwr_cmd ((wmi_t*)m_pWMI, settxPwr->dbM);
    if (status != A_OK)
        return NDIS_STATUS_FAILURE;

    return ndis_status;
}

NDIS_STATUS
CAR6KMini::Set_car6k_ip (WMI_SET_IP_CMD *pIP)
{
    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady)
    {
         return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    wmiStatus = wmi_set_ip_cmd ((wmi_t*)m_pWMI, pIP);

    if (wmiStatus != A_OK)
    {
        return NDIS_STATUS_FAILURE;
    }
    return ndis_status;
}

NDIS_STATUS
CAR6KMini::ar6000_abort_scan()
{

    A_STATUS status = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;
    status = wmi_abort_scan_cmd ((wmi_t*)m_pWMI);
    if (status != A_OK)
        return NDIS_STATUS_FAILURE;

    return ndis_status;
}

NDIS_STATUS
CAR6KMini::Set802_11_rssi_threshold(USER_RSSI_PARAMS *rssiParams)
{
    A_STATUS   wmiStatus = A_OK;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

#define SWAP_THOLD(thold1, thold2) do { \
    USER_RSSI_THOLD tmpThold;           \
    tmpThold.tag = thold1.tag;          \
    tmpThold.rssi = thold1.rssi;        \
    thold1.tag = thold2.tag;            \
    thold1.rssi = thold2.rssi;          \
    thold2.tag = tmpThold.tag;          \
    thold2.rssi = tmpThold.rssi;        \
} while (0)

    WMI_RSSI_THRESHOLD_PARAMS_CMD cmd;
    A_INT32 i, j;

    int ret = 0;
    cmd.weight = rssiParams->weight;
    cmd.pollTime = rssiParams->pollTime;

    A_MEMCPY(rssi_map, rssiParams->tholds, sizeof(rssi_map));
    /*
     *  only 6 elements, so use bubble sorting, in ascending order
     */
    for (i = 5; i > 0; i--) {
        for (j = 0; j < i; j++) { /* above tholds */
            if (rssi_map[j+1].rssi < rssi_map[j].rssi) {
                SWAP_THOLD(rssi_map[j+1], rssi_map[j]);
            } else if (rssi_map[j+1].rssi == rssi_map[j].rssi) {
                return NDIS_STATUS_NOT_SUPPORTED;
            }
        }
    }
    for (i = 11; i > 6; i--) {
        for (j = 6; j < i; j++) { /* below tholds */
            if (rssi_map[j+1].rssi < rssi_map[j].rssi) {
                SWAP_THOLD(rssi_map[j+1], rssi_map[j]);
            } else if (rssi_map[j+1].rssi == rssi_map[j].rssi) {
                return NDIS_STATUS_NOT_SUPPORTED;
            }
        }
    }

    cmd.thresholdAbove1_Val = rssi_map[0].rssi;
    cmd.thresholdAbove2_Val = rssi_map[1].rssi;
    cmd.thresholdAbove3_Val = rssi_map[2].rssi;
    cmd.thresholdAbove4_Val = rssi_map[3].rssi;
    cmd.thresholdAbove5_Val = rssi_map[4].rssi;
    cmd.thresholdAbove6_Val = rssi_map[5].rssi;
    cmd.thresholdBelow1_Val = rssi_map[6].rssi;
    cmd.thresholdBelow2_Val = rssi_map[7].rssi;
    cmd.thresholdBelow3_Val = rssi_map[8].rssi;
    cmd.thresholdBelow4_Val = rssi_map[9].rssi;
    cmd.thresholdBelow5_Val = rssi_map[10].rssi;
    cmd.thresholdBelow6_Val = rssi_map[11].rssi;

    if( wmi_set_rssi_threshold_params((wmi_t *)m_pWMI, &cmd) != A_OK ) {
        return NDIS_STATUS_FAILURE;
    }
      return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set_car6k_listen_intvl (WMI_LISTEN_INT_CMD *listenCmd)
{
    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS ndis_status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady)
    {
         return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if ((listenCmd->listenInterval < MIN_LISTEN_INTERVAL) ||
        (listenCmd->listenInterval > MAX_LISTEN_INTERVAL))
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Invalid data\n");
        return NDIS_STATUS_INVALID_DATA;
    }

    wmiStatus = wmi_listeninterval_cmd((wmi_t*)m_pWMI, listenCmd->listenInterval, listenCmd->numBeacons);
    if (wmiStatus != A_OK)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: ERROR - wmi_listeninterval_cmd failed = %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    m_ListenInterval = listenCmd->listenInterval;
    return ndis_status;
}


NDIS_STATUS
CAR6KMini::Get802_11PowerMode(
    OUT NDIS_802_11_POWER_MODE *pMode)
{

    *pMode = m_80211_PowerMode;

    return NDIS_STATUS_SUCCESS;
}

void
CAR6KMini::WMIBSSIDInfoAdd(
    IN  bss_t *pWmiBss)
//
//  This function is called after Get802_11BSSIDList is called,
//  to add this node to the BSSID list being returned.
//
{
    NDIS_WLAN_BSSID_EX        *pBSSID = m_pBSSIDList;
    PBYTE                      pIE;
    ULONG                      spaceNeeded = 0;
    NDIS_802_11_FIXED_IEs     *pFixed;
    NDIS_802_11_VARIABLE_IEs  *pVar;
#ifdef NDIS_802_11_LENGTH_RATES_EX
    ULONG                      RateSize;
#endif

#ifdef OS_ROAM_MANAGEMENT
    if (TRUE == m_bRoamScanIndication)
    {
        spaceNeeded = offsetof(struct ar6kScanIndication, slist) +
               offsetof(NDIS_802_11_BSSID_SCAN_INFO_LIST, BssidScanInfo);

        spaceNeeded += offsetof(NDIS_802_11_BSSID_SCAN_INFO, Bssid);
    }
#endif


    // Check to see that there is space available to add the node
    spaceNeeded += offsetof(NDIS_WLAN_BSSID_EX, IEs);

    spaceNeeded += sizeof(NDIS_802_11_FIXED_IEs);


#ifdef SUPPORT_WPA2
    if (pWmiBss->ni_cie.ie_rsn) {
        pIE = pWmiBss->ni_cie.ie_rsn;
        spaceNeeded = spaceNeeded + pIE[1] + 2;
    }
#endif

#ifdef WAPI_ENABLE
    if (pWmiBss->ni_cie.ie_wapi) {
        pIE = pWmiBss->ni_cie.ie_wapi;
        spaceNeeded = spaceNeeded + pIE[1] + 2;
    }
#endif /* WAPI_ENABLE */

    if (pWmiBss->ni_cie.ie_wpa) {
        pIE = pWmiBss->ni_cie.ie_wpa;
        spaceNeeded = spaceNeeded + pIE[1] + 2;
    }

    if (pWmiBss->ni_cie.ie_wsc) {
        pIE = pWmiBss->ni_cie.ie_wsc;
        spaceNeeded = spaceNeeded + pIE[1] + 2;
    }

    // spaceNeeded must be a multiple of 4 to maintain alignment.
    spaceNeeded = (spaceNeeded + 3) & ~3;

    if (*m_pcbBSSIDListNeeded + spaceNeeded < m_cbBSSIDList)
    {
        memset (pBSSID, 0, spaceNeeded);

        pBSSID->Length = spaceNeeded; // must be multiple of 4
        memcpy(pBSSID->MacAddress, pWmiBss->ni_macaddr, ETHERNET_MAC_ADDRESS_LENGTH);
        pBSSID->Ssid.SsidLength = 0;
        pIE = pWmiBss->ni_cie.ie_ssid;
        if (pIE)
        {
            // Format of SSID IE is:
            //  Type   (1 octet)
            //  Length (1 octet)
            //  SSID (Length octets)
            //
            //  Validation of the IE should have occurred within WMI.
            //
            ASSERT(pIE[0] == IEEE80211_ELEMID_SSID);
            ASSERT(pIE[1] <= 32);

            if (pIE[1] <= 32)
            {
                pBSSID->Ssid.SsidLength = pIE[1];
                memcpy(pBSSID->Ssid.Ssid, &pIE[2], pBSSID->Ssid.SsidLength);
            }
        }
        pBSSID->Privacy = (pWmiBss->ni_cie.ie_capInfo & 0x10) ? 1 : 0;
        //Post the RSSI value relative to the Standard Noise floor value.
        pBSSID->Rssi = RSSI_TO_NDIS(pWmiBss->ni_rssi);

        //
        // pBSSID->NetworkTypeInUse = m_NetworkTypeInUse;
        // NetworkType support added as MS suggested
        //
        if (pWmiBss->ni_cie.ie_chan >= 2412 && pWmiBss->ni_cie.ie_chan <= 2484) {

            if (pWmiBss->ni_cie.ie_rates && pWmiBss->ni_cie.ie_xrates) {
                pBSSID->NetworkTypeInUse = Ndis802_11OFDM24;
            }
            else {
                pBSSID->NetworkTypeInUse = Ndis802_11DS;
            }
        }
        else {
            pBSSID->NetworkTypeInUse = Ndis802_11OFDM5;
        }

        pBSSID->Configuration.Length = sizeof(pBSSID->Configuration);
        pBSSID->Configuration.BeaconPeriod = pWmiBss->ni_cie.ie_beaconInt; // Units are Kmicroseconds (1024 us)
        pBSSID->Configuration.ATIMWindow =  0;
        pBSSID->Configuration.DSConfig =  pWmiBss->ni_cie.ie_chan * 1000;
        pBSSID->InfrastructureMode = ((pWmiBss->ni_cie.ie_capInfo & 0x03) == 0x01 ) ? Ndis802_11Infrastructure : Ndis802_11IBSS;

#ifdef NDIS_802_11_LENGTH_RATES_EX
        //
        // Supported Rates have been added as MS suggested
        //
        RateSize = 0;
        pIE = pWmiBss->ni_cie.ie_rates;
        if (pIE) {
            RateSize = (pIE[1] < NDIS_802_11_LENGTH_RATES_EX) ? pIE[1] : NDIS_802_11_LENGTH_RATES_EX;
            memcpy(pBSSID->SupportedRates, &pIE[2], RateSize);
        }

        pIE = pWmiBss->ni_cie.ie_xrates;
        if (pIE && RateSize < NDIS_802_11_LENGTH_RATES_EX) {
            memcpy(pBSSID->SupportedRates + RateSize, &pIE[2],
                   (pIE[1] < (NDIS_802_11_LENGTH_RATES_EX - RateSize)) ? pIE[1] : (NDIS_802_11_LENGTH_RATES_EX - RateSize));
        }
#endif

        //
        // Normal Infra IEEE Scan list starts here
        //
        {
            // Copy the fixed IEs
            pBSSID->IELength = sizeof(NDIS_802_11_FIXED_IEs);

            pFixed = (NDIS_802_11_FIXED_IEs *)pBSSID->IEs;
            memcpy(pFixed->Timestamp, pWmiBss->ni_cie.ie_tstamp, sizeof(pFixed->Timestamp));
            pFixed->BeaconInterval = pWmiBss->ni_cie.ie_beaconInt;
            pFixed->Capabilities = pWmiBss->ni_cie.ie_capInfo;

            // Copy selected variable IEs

            pVar = (NDIS_802_11_VARIABLE_IEs *)((PBYTE)pFixed + sizeof(NDIS_802_11_FIXED_IEs));

    #ifdef SUPPORT_WPA2
            // Copy the WPAv2 IE
            if (pWmiBss->ni_cie.ie_rsn)
            {
                pIE = pWmiBss->ni_cie.ie_rsn;
                pBSSID->IELength += pIE[1] + 2;
                memcpy(pVar, pIE, pIE[1] + 2);
                pVar = (NDIS_802_11_VARIABLE_IEs *)((PBYTE)pVar + pIE[1] + 2);
            }
    #endif

            // Copy the WPAv1 IE
            if (pWmiBss->ni_cie.ie_wpa) {
                pIE = pWmiBss->ni_cie.ie_wpa;
                pBSSID->IELength += pIE[1] + 2;
                memcpy(pVar, pIE, pIE[1] + 2);
                pVar = (NDIS_802_11_VARIABLE_IEs *)((PBYTE)pVar + pIE[1] + 2);
            }

#ifdef WAPI_ENABLE
            if (pWmiBss->ni_cie.ie_wapi) {
                pIE = pWmiBss->ni_cie.ie_wapi;
                pBSSID->IELength += pIE[1] + 2;
                memcpy(pVar, pIE, pIE[1] + 2);
                pVar = (NDIS_802_11_VARIABLE_IEs *)((PBYTE)pVar + pIE[1] + 2);
            }
#endif /* WAPI_ENABLE */

            // Copy the WPS IE
            if (pWmiBss->ni_cie.ie_wsc) {
                pIE = pWmiBss->ni_cie.ie_wsc;
                pBSSID->IELength += pIE[1] + 2;
                memcpy(pVar, pIE, pIE[1] + 2);
                pVar = (NDIS_802_11_VARIABLE_IEs *)((PBYTE)pVar + pIE[1] + 2);
            }
        }

#ifdef OS_ROAM_MANAGEMENT
        if (TRUE == m_bRoamScanIndication)
        {
            // Advance buffer pointer
            m_psi = (PNDIS_802_11_BSSID_SCAN_INFO)((BYTE*)m_psi + pBSSID->Length + FIELD_OFFSET(NDIS_802_11_BSSID_SCAN_INFO, Bssid));
        }
        else
#endif
        {
            // Advance buffer pointer to next space beyond what we just added
            m_pBSSIDList = (NDIS_WLAN_BSSID_EX *)((PBYTE)pBSSID + pBSSID->Length);
        }

        m_BSSIDListNumberOfItems++;
    }
    *m_pcbBSSIDListNeeded += spaceNeeded;
}

void
AR6KWMIIterateNodeCallback(
    IN  PVOID Context,
    IN  bss_t *pBssNode)
//
//  This function is called by WMI after we call wmi_iterate_nodes
//
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;

    Adapter->WMIBSSIDInfoAdd(pBssNode);
}

void
CAR6KMini::Get802_11BSSIDList(
    OUT NDIS_802_11_BSSID_LIST_EX *pList,
    IN  ULONG                      cbBuffer,
    OUT ULONG                     *pcbNeeded)
//
//  Return a list containing all of the detected BSSIDs and their attributes
//
{
    pList->NumberOfItems = 0;
    //Return if WMI is not ready yet
    if (!m_WMIReady)
        return;

    if (m_RadioDisabled)
        return;

    *pcbNeeded = offsetof(NDIS_802_11_BSSID_LIST_EX, Bssid);

    m_pBSSIDList = &pList->Bssid[0];
    m_cbBSSIDList = cbBuffer;
    m_pcbBSSIDListNeeded = pcbNeeded;
    m_BSSIDListNumberOfItems = 0;

    wmi_iterate_nodes((wmi_t *)m_pWMI, AR6KWMIIterateNodeCallback, this);

    m_pBSSIDList = NULL;
    m_cbBSSIDList = 0;
    m_pcbBSSIDListNeeded = NULL;
    pList->NumberOfItems = m_BSSIDListNumberOfItems;
}


void
CAR6KMini::Get802_11AuthenticationMode(
    OUT NDIS_802_11_AUTHENTICATION_MODE *pMode)
{
    *pMode = m_AuthenticationMode;
}

NDIS_STATUS
CAR6KMini::Get802_11PrivacyFilter(
    OUT NDIS_802_11_PRIVACY_FILTER *pFilter)
{
    return NDIS_STATUS_NOT_SUPPORTED;
}

void
CAR6KMini::Get802_11EncryptionStatus(
    OUT NDIS_802_11_ENCRYPTION_STATUS *pStatus)
{
    *pStatus = m_EncryptionStatus;
    switch (m_EncryptionStatus) {
    case Ndis802_11WEPEnabled:
        if (m_cbKey == 0) {
            *pStatus = Ndis802_11WEPKeyAbsent;
        }
        break;
    case Ndis802_11Encryption2Enabled:
        if (m_cbKey == 0) {
            *pStatus = Ndis802_11Encryption2KeyAbsent;
        }
        break;
    case Ndis802_11Encryption3Enabled:
        if (m_cbKey == 0) {
            *pStatus = Ndis802_11Encryption3KeyAbsent;
        }
        break;
    } //switch
    return;
}

/*
 * The target combines association request and response
 * into a single byte stream and notifies the same in
 * connect_event. assocReqLen is the length in bytes of
 * association request and assocRespLen is the length in
 * bytes of association response. There is one glitch
 * though. The association request that is passed by the
 * target contains only the Variable IEs and the fixed
 * IEs like listen Interval and AP address are passed
 * as arguments to the connect_event. However the association
 * response passed to the host contains the full 802.11
 * frame as seen on the wire. That is, in essence the request
 * and response parsing will not be symmetrical.
 * One pending kludge to be fixed - WZC requires Capabilities
 * IE for both request and response as it is not returned
 * in requests, it is hardcoded as 0x3104
 */
NDIS_STATUS
CAR6KMini::Get802_11AssociationInformation(
    OUT NDIS_802_11_ASSOCIATION_INFORMATION *pAssocInfo,
    IN  ULONG                                cbBuffer,
    OUT ULONG                               *pcbNeeded)
{
    PBYTE  ptr;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    if (m_AssocReqLen == 0 && m_AssocRespLen == 0) {
        *pcbNeeded = 0;
        return NDIS_STATUS_SUCCESS;
    }
    *pcbNeeded = m_AssocReqLen + m_AssocRespLen + sizeof(*pAssocInfo);

    if (cbBuffer < *pcbNeeded) {
        return NDIS_STATUS_INVALID_LENGTH;
    }

    memcpy(pAssocInfo->RequestFixedIEs.CurrentAPAddress,
        m_PeerBSSID, ETHERNET_MAC_ADDRESS_LENGTH);

    pAssocInfo->AvailableRequestFixedIEs =
        NDIS_802_11_AI_REQFI_CAPABILITIES |
        NDIS_802_11_AI_REQFI_LISTENINTERVAL |
        NDIS_802_11_AI_REQFI_CURRENTAPADDRESS;

    pAssocInfo->OffsetRequestIEs = sizeof(*pAssocInfo);
    pAssocInfo->RequestIELength = m_AssocReqLen - 4;
    ptr = m_pAssocInfo + m_BeaconIeLen;
    pAssocInfo->RequestFixedIEs.Capabilities = ((*(ptr + 1)) << 8) | *ptr;
    ptr += 2;
    pAssocInfo->RequestFixedIEs.ListenInterval = ((*(ptr + 1)) << 8) | *ptr;
    ptr += 2;
    memcpy((PBYTE)pAssocInfo + pAssocInfo->OffsetRequestIEs, ptr,
        pAssocInfo->RequestIELength);


    pAssocInfo->AvailableResponseFixedIEs =
        NDIS_802_11_AI_RESFI_CAPABILITIES |
        NDIS_802_11_AI_RESFI_STATUSCODE   |
        NDIS_802_11_AI_RESFI_ASSOCIATIONID;

    pAssocInfo->OffsetResponseIEs = sizeof(*pAssocInfo) +
                        pAssocInfo->RequestIELength;
    pAssocInfo->ResponseIELength = m_AssocRespLen - 6;
    ptr = m_pAssocInfo + m_BeaconIeLen + m_AssocReqLen;
    pAssocInfo->ResponseFixedIEs.Capabilities = ((*(ptr + 1)) << 8) | *ptr;
    ptr += 2;
    pAssocInfo->ResponseFixedIEs.StatusCode = ((*(ptr + 1)) << 8) | *ptr;
    ptr += 2;
    pAssocInfo->ResponseFixedIEs.AssociationId = ((*(ptr + 1)) << 8) | *ptr;
    ptr += 2;
    memcpy((PBYTE)pAssocInfo + pAssocInfo->OffsetResponseIEs, ptr,
        pAssocInfo->ResponseIELength);

    pAssocInfo->Length = sizeof(*pAssocInfo);

    return NDIS_STATUS_SUCCESS;
}

#ifdef SUPPORT_WPA2
NDIS_STATUS
CAR6KMini::Get802_11PMKID(OUT NDIS_802_11_PMKID *pPMKID,
                          IN  ULONG              cbBuffer,
                          OUT ULONG             *pcbNeeded)
{
    A_UINT32 i = 0;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (m_pPMKID == NULL) {
        pPMKID->BSSIDInfoCount = 0;
        return NDIS_STATUS_SUCCESS;
    }

    *pcbNeeded = m_pPMKID->Length;

    if (cbBuffer < m_pPMKID->Length) {
        return NDIS_STATUS_INVALID_LENGTH;
    }

    pPMKID->Length = m_pPMKID->Length;
    pPMKID->BSSIDInfoCount = m_pPMKID->BSSIDInfoCount;

    for (i=0; i<pPMKID->BSSIDInfoCount; i++) {
        memcpy(pPMKID->BSSIDInfo[i].BSSID,
            m_pPMKID->BSSIDInfo[i].BSSID, 6);
        memcpy(pPMKID->BSSIDInfo[i].PMKID,
            m_pPMKID->BSSIDInfo[i].PMKID, 16);
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11Capability(OUT NDIS_802_11_CAPABILITY *pCap,
                           IN ULONG                   cbBuffer,
                           OUT ULONG                  *pcbNeeded)
{
    ULONG i = 0;
    ULONG lenNeeded;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    lenNeeded = sizeof(pCap->Version) + sizeof(pCap->NoOfPMKIDs) +
                sizeof(pCap->NoOfAuthEncryptPairsSupported) + sizeof(pCap->Length) +
                (11 * sizeof(NDIS_802_11_AUTHENTICATION_ENCRYPTION));

    *pcbNeeded = lenNeeded;

    if (cbBuffer < lenNeeded) {
        return NDIS_STATUS_INVALID_LENGTH;
    }

    pCap->Version         = 2;
    pCap->NoOfPMKIDs     = 3; //no of PMKIDs that can be cached, min of 3 and max of 16.

    pCap->NoOfAuthEncryptPairsSupported = 11; //excluding WEP shared and WPA-None

    pCap->Length = lenNeeded;

    for (i = 0; i < 11; i++) {
        pCap->AuthenticationEncryptionSupported[i].AuthModeSupported =
            g_WPA2AuthEncryptPairSupported[i].AuthModeSupported;
        pCap->AuthenticationEncryptionSupported[i].EncryptStatusSupported =
            g_WPA2AuthEncryptPairSupported[i].EncryptStatusSupported;
    }

    return NDIS_STATUS_SUCCESS;
}
#endif //SUPPORT_WPA2

NDIS_STATUS
CAR6KMini::GetCAr6k802_11AuthAlg(
    CAR6K_802_11_AUTH_ALG *pAuthAlg)
{
    *pAuthAlg = m_AuthAlg;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::GetCAr6k802_11CCKMAuthMode(
    CAR6K_802_11_CCKM_AUTHENTICATION_MODE *pCCKMAuthenticationMode)
{
    *pCCKMAuthenticationMode = m_CCKMAuthenticationMode;
    return NDIS_STATUS_SUCCESS;
}

// AR6K 802.11 OID Set Handlers

NDIS_STATUS
CAR6KMini::Set802_11AssociatedAPBSSID(
    IN NDIS_802_11_MAC_ADDRESS *BSSID)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    NDIS_802_11_SSID *SSID, ssid;
    bss_t *pBss;
    PBYTE pIE;
    BOOL bIsSameSSID = FALSE;

    if (!m_WMIReady)
    {
        Status = NDIS_STATUS_ADAPTER_NOT_READY;
        goto done;
    }


    SSID        = &ssid;
    memset( &ssid, 0, sizeof(NDIS_802_11_SSID));
    memcpy (m_DesiredAPBSSID, BSSID, sizeof(NDIS_802_11_MAC_ADDRESS));

    pBss = wmi_find_node ((wmi_t *)m_pWMI, (A_UINT8 *)m_DesiredAPBSSID);

    if (pBss)
    {
        pIE = pBss->ni_cie.ie_ssid;

        if (pIE)
        {
            if (pIE[1] <= 32)
            {
                SSID->SsidLength = pIE[1];
                memcpy(SSID->Ssid, &pIE[2], SSID->SsidLength);
            }
        }
        wmi_node_return ((wmi_t *)m_pWMI, pBss);
    }
    else
    {
        Status = NDIS_STATUS_ADAPTER_NOT_READY;
        goto done;
    }

    if (m_CounterMeasureOn)
    {
        goto done;
    }

    if (SSID->SsidLength == m_SSID.SsidLength   &&
        0 == memcmp (SSID->Ssid, m_SSID.Ssid, m_SSID.SsidLength))
    {
        bIsSameSSID = TRUE;
    }


    if ((TRUE  == m_ConnectInProgress)  || (TRUE  == m_Connected))
    {
        if (FALSE == bIsSameSSID)
            Status = SendWMIDisconnectCommand ();
        else
            Status = SendWMIReconnectCommand (&m_DesiredAPBSSID);
    }
    else
    {
        // No current connection,
        Status = SendWMIConnectCommand (SSID);
    }

done:
     return Status;
}

bool
IsBroadcastAddress(
    IN PBYTE pMACAddress)
//
//  Return TRUE if the MAC address is all 1's (i.e. FF FF FF FF FF FF)
//
{
    bool IsBroadcast = TRUE;

    for (UINT i = 0; i < sizeof(NDIS_802_11_MAC_ADDRESS); i++)
    {
        if (pMACAddress[i] != 0xFF)
        {
            IsBroadcast = FALSE;
            break;
        }
    }

    return IsBroadcast;
}

NDIS_STATUS
CAR6KMini::SendWMIReconnectCommand(
    IN NDIS_802_11_MAC_ADDRESS *BSSID)
//
//  Request that the Wireless Module associate with the specified SSID.
//
{
    NDIS_STATUS  Status = NDIS_STATUS_SUCCESS;
    A_STATUS     wmiStatus;

    wmiStatus = wmi_reconnect_cmd((wmi_t *)m_pWMI, *BSSID, 0);

    if (wmiStatus != A_OK) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: ERROR - wmi_connect_cmd failed = %u\n", wmiStatus);
        Status = NDIS_STATUS_FAILURE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SendWMIConnectCommand(
    IN NDIS_802_11_SSID *SSID)
//
//  Request that the Wireless Module associate with the specified SSID.
//
{
    NDIS_STATUS  Status = NDIS_STATUS_SUCCESS;
    A_STATUS     wmiStatus;
    NETWORK_TYPE wmiNetworkType = m_InfrastructureMode == Ndis802_11Infrastructure ? INFRA_NETWORK : ADHOC_NETWORK;
    DOT11_AUTH_MODE wmiDot11AuthMode = OPEN_AUTH;
    AUTH_MODE    wmiAuthMode = NONE_AUTH;
    BYTE        *pAPBSSID;
    CRYPTO_TYPE keyType = NONE_CRYPT;
    BYTE KeyIndex = 0;

    //
    // update the Channel Hint from BssInfo, During 802.11a mode, some times no. of
    // passive channels are more, and so that the connection takes longer than WZC
    // connection time (2~3sec), so better driver find the channel in case of non-hidden
    // ssid, otherwise, it always be a zero
    //
    GetChannelHint (SSID);

    //
    // Assign Channel number from the registry in case of IBSS
    //
    m_ChannelHint = (m_InfrastructureMode == Ndis802_11IBSS) ? (A_UINT16)(m_Config.ibssChannel/1000) : m_ChannelHint;
    /* set the default atim window for ibss mode */
    if ((m_InfrastructureMode == Ndis802_11IBSS) && (m_IBSS_PSEnable)) {
        wmi_ibsspmcaps_cmd((wmi_t *)m_pWMI, 1, 0, m_Config.atimWindow, 0);
    }

#ifndef OS_ROAM_MANAGEMENT
    //
    // CONNECT commands can only be sent in the DISCONNECTED state.
    // And roaming manager in disabled state
    //
    //
    ASSERT (false == m_Connected);
    ASSERT (false == m_ConnectInProgress);
#endif

    switch(m_AuthenticationMode)
    {
    case Ndis802_11AuthModeOpen:
        wmiDot11AuthMode = OPEN_AUTH;
        break;

    case Ndis802_11AuthModeShared:
        wmiDot11AuthMode = SHARED_AUTH;
        break;

    case Ndis802_11AuthModeWPA:
        wmiAuthMode = WPA_AUTH;
        break;

    case Ndis802_11AuthModeWPAPSK:
        wmiAuthMode = WPA_PSK_AUTH;
        break;

    case Ndis802_11AuthModeWPA2PSK:
        wmiAuthMode = WPA2_PSK_AUTH;
        break;

    case Ndis802_11AuthModeWPA2:
        wmiAuthMode = WPA2_AUTH;
        break;

    default:
        break;
    }

    if (m_AuthAlg == car6k802_11AuthAlgLeap) {
        wmiDot11AuthMode = LEAP_AUTH;
    }

    // Set the authmode to WPA/WPA2 based on the CCKM mode
    switch(m_CCKMAuthenticationMode)
    {
        case car6k802_11AuthModeWPACCKM:
            m_AuthenticationMode = Ndis802_11AuthModeWPA;
            wmiAuthMode = WPA_AUTH_CCKM;
            break;

        case car6k802_11AuthModeWPA2CCKM:
            m_AuthenticationMode = (NDIS_802_11_AUTHENTICATION_MODE)Ndis802_11AuthModeWPA2;
            wmiAuthMode = WPA2_AUTH_CCKM;
            break;
    }

    switch (m_EncryptionStatus)
    {
    case Ndis802_11WEPEnabled:
        m_PairwiseCryptoType = WEP_CRYPT;
        m_GrpwiseCryptoType = WEP_CRYPT;
        break;

    case Ndis802_11WEPDisabled:
        m_PairwiseCryptoType = NONE_CRYPT;
        m_GrpwiseCryptoType = NONE_CRYPT;
        break;

    case Ndis802_11Encryption2Enabled:
        m_PairwiseCryptoType = TKIP_CRYPT;
        // Correct group key will be updated on connect indication
        m_GrpwiseCryptoType = TKIP_CRYPT;
        break;

    case Ndis802_11Encryption3Enabled:
        m_PairwiseCryptoType = AES_CRYPT;
        // Correct group key will be updated on connect indication
        m_GrpwiseCryptoType = AES_CRYPT;
        break;

    default:
        break;
    }

    pAPBSSID = NULL;

    if (!IsBroadcastAddress (m_DesiredAPBSSID))
    {
        pAPBSSID  = m_DesiredAPBSSID;
    }

    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Connect command from Supplicant\n");


    if (m_WMIBssFilter !=  ALL_BSS_FILTER) {
        wmiStatus = wmi_bssfilter_cmd((struct wmi_t *)m_pWMI, ALL_BSS_FILTER, 0);
        if (wmiStatus == A_OK) {
            m_WMIBssFilter = ALL_BSS_FILTER;
        } else {
            return NDIS_STATUS_FAILURE;
        }
    }

    //Add the keys to the NIC, before issuing the connect cmd.
    //Add the keys here only if the Auth mode is either SharedAuth or AutoSwitch. Only for
    //Shared Auth the WEP keys are plumbed before the connection.

    if (m_AuthenticationMode == Ndis802_11AuthModeShared ||
        m_AuthenticationMode == Ndis802_11AuthModeAutoSwitch ) {

        for (KeyIndex = 0; KeyIndex < 4 ; KeyIndex++ ) {

            if ( m_cbKey[KeyIndex] != 0 ) {

                if( (m_KeyUsage[KeyIndex] & GROUP_USAGE) == GROUP_USAGE )
                {
                    keyType = m_GrpwiseCryptoType;
                } else {

                    keyType = m_PairwiseCryptoType;
                }

                wmiStatus = wmi_addKey_cmd((wmi_t *)m_pWMI,
                                           (A_UINT8)KeyIndex,
                                           WEP_CRYPT, // Only for WEP the keys are added before connect
                                           m_KeyUsage[KeyIndex],
                                           (A_UINT8)m_cbKey[KeyIndex],
                                           (A_UINT8*)&m_KeyRSC[KeyIndex],
                                           m_Key[KeyIndex],KEY_OP_INIT_VAL,NULL,SYNC_BOTH_WMIFLAG);

                if (wmiStatus != A_OK) {
                    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_addKey_cmd failed = %u\n", wmiStatus);
                    return NDIS_STATUS_FAILURE;
                }
            }
        }
    }

    /* Set the Listen Interval to 1000TUs. This value will be indicated to the
     * AP in the conn. request. Later set it back locally at the STA to 100/1000 TUs depending on the
     * power mode.
     */
 if(m_networkType != AP_NETWORK) {

     if (m_ListenInterval < NDIS_MAXPSP_LISTEN_INTERVAL)
     {
        //Increase the listen interval to 1000 TUs to ensure max buffering capacity at the AP.
        wmiStatus = wmi_listeninterval_cmd((struct wmi_t *)m_pWMI,
                                                                    NDIS_MAXPSP_LISTEN_INTERVAL, 0 );

        if (wmiStatus != A_OK) {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: ERROR - wmi_listeninterval_cmd failed = %u\n", wmiStatus);
            return NDIS_STATUS_FAILURE;
        }

     }
}

#ifdef WAPI_ENABLE
    if (m_wapimode != 0) {
        m_PairwiseCryptoType = WAPI_CRYPT;
        m_GrpwiseCryptoType = WAPI_CRYPT;
    }
#endif /* WAPI_ENABLE */

#ifdef NO_BCAST_PROBE_IN_CONNECT
    if (SSID->SsidLength > 0)
    {
        wmi_probedSsid_cmd((struct wmi_t *)m_pWMI, 0, SPECIFIC_SSID_FLAG, (A_UINT8)SSID->SsidLength, SSID->Ssid);
    }
    else
    {
        wmi_probedSsid_cmd((struct wmi_t *)m_pWMI, 0, ANY_SSID_FLAG, 0,NULL);
    }
#endif
    wmiStatus = wmi_connect_cmd((wmi_t *)m_pWMI, wmiNetworkType,
                wmiDot11AuthMode, wmiAuthMode,
                m_PairwiseCryptoType, 0,
                m_GrpwiseCryptoType, 0,
                m_SSID.SsidLength, m_SSID.Ssid,
                pAPBSSID, m_ChannelHint,
                m_Config.connectCtrlFlags | CONNECT_IGNORE_WPAx_GROUP_CIPHER);

    if (wmiStatus != A_OK) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: ERROR - wmi_connect_cmd failed = %u\n", wmiStatus);
        Status = NDIS_STATUS_FAILURE;
    } else {
        m_ConnectInProgress = TRUE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SendWMIDisconnectCommand()
//
//  Request that the Wireless Module disassociate from the current wireless
//  network. This request may be issued in any state. It will cancel an ongoing
//  CONNECT command if one is in progress. A DISCONNECTED_EVENT will always be
//  generated, even if the current state is DISCONNECTED.
//
{
    NDIS_STATUS  Status = NDIS_STATUS_SUCCESS;
    A_STATUS     wmiStatus;

    wmiStatus = wmi_setPmkid_cmd((wmi_t*)m_pWMI, m_PeerBSSID, NULL, 0);
    wmiStatus = wmi_disconnect_cmd((wmi_t *)m_pWMI);
    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Disconnect from supplicant\n");

    if (wmiStatus != A_OK) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_disconnect_cmd failed = %u\n", wmiStatus);
        Status = NDIS_STATUS_FAILURE;
    }

    return Status;
}


NDIS_STATUS
CAR6KMini::Set802_11SSID(
    IN NDIS_802_11_SSID *SSID)
//
//  This request informs the driver of the SSID to associate to. If the first
//  character of the SSID string is zero, then it can associate with any SSID.
//  If the SSID is invalid (contains non printable ASCII characters), then the
//  adapter should not associate with any SSID.
//
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_STATUS     wmiStatus;
    BOOLEAN     foundOpenSsid = FALSE;
    NDIS_802_11_SSID *lSSID;
    BOOL bWaitRet         = FALSE;
    NDIS_802_11_BSSID_LIST_EX *pBSSIDListEx = NULL;

    if (!m_WMIReady) {
        Status = NDIS_STATUS_ADAPTER_NOT_READY;
        goto done;
    }

    if (m_RadioDisabled) {
        if(!m_ForceDisable) {
            m_RadioDisabled = FALSE;
            configRegistryParameters();
            configTargetParams();
            // leave the scan parameter to firmware defaults
            wmiStatus = wmi_startscan_cmd((wmi_t *)m_pWMI,WMI_LONG_SCAN,FALSE,FALSE,0,0,0,NULL);
            if (wmiStatus != A_OK)
                    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_startscan_cmd failed = %u\n", wmiStatus);
        } else {
            goto done;
        }
    }

    if (m_CounterMeasureOn)
        goto done;

    if (SSID->SsidLength > 32)
    {
        // Bad length
        Status = NDIS_STATUS_INVALID_DATA;
        goto done;
    }

    // SSID may be NULL to connect to any available SSID
    if (SSID->SsidLength == 0) {
        ULONG dwSize = 2048;
        ULONG NoOfItems;
        ULONG pcbNeeded=0;
        NDIS_802_11_RSSI rssi = -255;

        pBSSIDListEx = (NDIS_802_11_BSSID_LIST_EX *)A_MALLOC(dwSize);
		if(pBSSIDListEx == NULL)
			goto done;
        this->Get802_11BSSIDList(pBSSIDListEx,dwSize,&pcbNeeded);
        foundOpenSsid= FALSE;

        if(pBSSIDListEx) {
            NoOfItems = pBSSIDListEx->NumberOfItems;
            NDIS_WLAN_BSSID_EX *pBSSID = (NDIS_WLAN_BSSID_EX *) &pBSSIDListEx->Bssid[0];
            while (NoOfItems) {
                // Select a best open AP based on RSSI
                if ((!pBSSID->Privacy) &&
                    (pBSSID->Ssid.SsidLength))
                {
                    if (pBSSID->Rssi > rssi) {
                        rssi = pBSSID->Rssi;
                        lSSID=&pBSSID->Ssid;
                        foundOpenSsid= TRUE;
                    }
                }
                pBSSID = (NDIS_WLAN_BSSID_EX *)((PBYTE)pBSSID + pBSSID->Length);
                NoOfItems--;
            }
        }

        if (foundOpenSsid) {
            m_AuthenticationMode=Ndis802_11AuthModeOpen;
            m_EncryptionStatus=Ndis802_11WEPDisabled;
        } else {
            /* no open SSID AP is found, no connection possible */
            goto done;
        }
    }
    else {
        lSSID=SSID;
    }

    m_WantToBeConnected = SSIDIsValid(lSSID);
    if (FALSE == m_WantToBeConnected)
    {
        // We want to abort any connection attempt in progress, and
        // dissassociate from any SSID to which we are current associated.
        if (m_ConnectInProgress || m_Connected)
        {
            Status = SendWMIDisconnectCommand();
        }
    }
    else
    {
        // The set SSID maybe same as the one we are already connected or different.
        // In any case we issue a disconnect if we are already connected. This is because
        // there may be a change in the Conn. profile (such as encryption key) that warrants
        // a disconnect & reconnect.
        m_SSID = *lSSID;
        if ((m_ConnectInProgress) || (m_Connected))
        {
            SendWMIDisconnectCommand ();
        } else {
            Status = SendWMIConnectCommand(&m_SSID);
        }
    }

done:
    if(pBSSIDListEx) {
        A_FREE(pBSSIDListEx);
    }
    return Status;
}

NDIS_STATUS
CAR6KMini::Set802_11NetworkTypeInUse(
    IN NDIS_802_11_NETWORK_TYPE Type)
{
    if (m_80211PhyCapability == mode_11ag)
    {
        if (Type == Ndis802_11DS         ||
            Type == Ndis802_11OFDM24     ||
            Type == Ndis802_11OFDM5)
        {
            m_NetworkTypeInUse = Type;
            return NDIS_STATUS_SUCCESS;
        }
    }
    else
    {
        if (Type == Ndis802_11DS || Type == Ndis802_11OFDM24)
        {
            m_NetworkTypeInUse = Type;
            return NDIS_STATUS_SUCCESS;
        }
    }
    return NDIS_STATUS_INVALID_DATA;
}

NDIS_STATUS
CAR6KMini::Set802_11TxPowerLevel(
    IN NDIS_802_11_TX_POWER_LEVEL Level)
{
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RSSITrigger(
    IN NDIS_802_11_RSSI Trigger)
{
    A_STATUS wmiStatus = A_OK;
    WMI_RSSI_THRESHOLD_PARAMS_CMD cmd;

    //
    // To adjust rssi trigger value, so that AR6K deliver the event
    // when the current RSSI reaches the same or greater or lesser
    //
    A_INT16 rssi = m_rssiEvent = (A_INT16)Trigger;

    cmd.weight   = DEF_RSSI_TRIGGER_WEIGHT;
    cmd.pollTime = DEF_RSSI_TRIGGER_POLLTIME;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if ( (Trigger > -10) || (Trigger < -200) ) {
        return NDIS_STATUS_INVALID_DATA;
    }

    cmd.thresholdAbove1_Val = cmd.thresholdBelow6_Val = rssi;
    cmd.thresholdAbove2_Val = rssi + 1; cmd.thresholdBelow5_Val = rssi - 1;
    cmd.thresholdAbove3_Val = rssi + 2; cmd.thresholdBelow4_Val = rssi - 2;
    cmd.thresholdAbove4_Val = rssi + 3; cmd.thresholdBelow3_Val = rssi - 3;
    cmd.thresholdAbove5_Val = rssi + 4; cmd.thresholdBelow2_Val = rssi - 4;
    cmd.thresholdAbove6_Val = rssi + 5; cmd.thresholdBelow1_Val = rssi - 5;

    if( wmi_set_rssi_threshold_params((wmi_t *)m_pWMI, &cmd) != A_OK ) {
        return NDIS_STATUS_FAILURE;
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11InfrastructureMode(
    IN NDIS_802_11_NETWORK_INFRASTRUCTURE Mode)
{
    A_INT32 i;
    A_STATUS wmiStatus;
    BYTE KeyIndex;
    NDIS_802_11_SSID trigSSID;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }



    m_InfrastructureMode = Mode;

#ifdef WAPI_ENABLE
    m_wapimode = 0;
#endif /* WAPI_ENABLE */

    // Clear all the Keys as per OID Recommendation
    for (i=0;i<4;i++) {
        if (m_Key[i]) {
            KeyIndex=(BYTE)(m_KeyIndex[i]);
            memset(m_Key[i],0,sizeof(m_Key[i]));
            m_cbKey[i] = 0;
            m_KeyIndex[i] = 0;
            m_KeyRSC[i] = 0;

            wmiStatus = wmi_deleteKey_cmd((wmi_t *)m_pWMI, KeyIndex);
            if (wmiStatus != A_OK) {
                m_PairwiseCryptoType = NONE_CRYPT;
                return NDIS_STATUS_FAILURE;
            }
        }
    }
    //
    // flag has to be re-initiated whenever zeroconfig issues delete key command, otherwise, this may
    // causes unnecessary target crashes
    //
    m_PairwiseCryptoType = NONE_CRYPT;

    if(m_bIsSwitchAPtoSTA) {

       memcpy (trigSSID.Ssid,"buffalo\0",8);
       trigSSID.SsidLength = 7;
       Set802_11SSID (&trigSSID);
    }



    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11FragmentationThreshold(
    IN NDIS_802_11_FRAGMENTATION_THRESHOLD Threshold)
{
    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if ( (Threshold < 256) || (Threshold > 2346) ) {
        return NDIS_STATUS_INVALID_DATA;
    }
    //
    // to be done
    // m_FragmentationThreshold = Threshold;
    //
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RTSThreshold(
    IN NDIS_802_11_RTS_THRESHOLD Threshold)
{
    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if ( (Threshold < 0) || (Threshold > 2347) ) {
        return NDIS_STATUS_INVALID_DATA;
    }
    if ( A_OK != wmi_set_rts_cmd( (wmi_t *)m_pWMI, (A_UINT16)Threshold)) {
        return NDIS_STATUS_FAILURE;
    }
    m_RTSThreshold = Threshold;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RxAntennaSelected(
    IN ULONG Antenna)
{
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11TxAntennaSelected(
    IN ULONG Antenna)
{
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11DesiredRates(
    IN NDIS_802_11_RATES *Rates)
{
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11Configuration(
    IN NDIS_802_11_CONFIGURATION *pConfiguration)
//
//  Set the underlying NIC's radio configuration parameters to the specified values
//
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    // This OID can only be set when the device is not associated
    // within the desired SSID. The miniport driver must return
    // NDIS_STATUS_NOT_ACCEPTED if it is currently associated.
    if (m_Connected)
        Status = NDIS_STATUS_NOT_ACCEPTED;
    else
    {
        if ( m_InfrastructureMode == Ndis802_11IBSS ) {
                m_Config.ibssChannel = pConfiguration->DSConfig;
        }
        // BeaconPeriod specifies the beacon interval for ad hoc mode.
        // BeaconPeriod is ignored in infrastructure mode
        // wmi_set_adhoc_beacon_interval(BeaconPeriod);
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11Disassociate()
//
//  Disassociate from the current service set and turn off the radio.
//
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_STATUS wmiStatus;
    m_WantToBeConnected = FALSE;
    m_SSID.SsidLength = 0;

    // Only need to do something if we are connected
    if (m_Connected)
    {
        if (!m_WMIReady) {
            // Hardware is not in a state where we can send it commands
            Status = NDIS_STATUS_ADAPTER_NOT_READY;
        }
        else {
            Status = SendWMIDisconnectCommand();
        }
    }
    wmiStatus =    wmi_scanparams_cmd((wmi_t *)m_pWMI,0xFFFF,0,0,0,0,0,0,0,0,0);
    if ( wmiStatus != A_OK ) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_scanparams_cmd failed = %u\n", wmiStatus);
    }
    wmi_free_allnodes((wmi_t *)m_pWMI);

    if (m_Connected && Status==NDIS_STATUS_SUCCESS) {
        m_Connected = FALSE;
        NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
        NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
    }
    m_RadioDisabled = TRUE;
    return Status;
}

NDIS_STATUS
CAR6KMini::Set802_11PowerMode(
    IN NDIS_802_11_POWER_MODE PowerMode)
{
    A_STATUS Status = A_OK;
    A_UINT8 powerMode = 0;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    // return if PowerMode @ target is already the same as what we get here.
    if (PowerMode == m_80211_PowerMode)
        return NDIS_STATUS_SUCCESS;

    //Update the global power mode ;
    m_80211_PowerMode = PowerMode;

    /* PowerMode var is changed from ndis power modes values to wmi power mode
     * values, so the update was done here rather at the end of the fn */

    switch( PowerMode ) {

    case Ndis802_11PowerModeCAM:

        powerMode = MAX_PERF_POWER;

        break;
    case Ndis802_11PowerModeMAX_PSP:

        powerMode = REC_POWER;

        break;
    case Ndis802_11PowerModeFast_PSP:

        powerMode = REC_POWER;

        break;
    default:
        return NDIS_STATUS_INVALID_DATA;
    }

    Status = wmi_powermode_cmd((struct wmi_t *)m_pWMI, powerMode);

    if ( Status != A_OK ) {

        return NDIS_STATUS_FAILURE;
    }

    //Update the global power mode & listen Interval vars with the set value;
    m_80211_PowerMode = PowerMode;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11BSSIDListScan()
//
//  Perform a network scan of BSSIDs and SSIDs.
//
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_STATUS    wmiStatus;

    if (!m_WMIReady)
    {
        Status = NDIS_STATUS_ADAPTER_NOT_READY;
        goto done;
    }

    if (m_ConnectInProgress || m_RadioDisabled || !(m_Config.fgScanEnable))
    {
        goto done;
    }
#ifdef NO_BCAST_PROBE_IN_CONNECT
    wmi_probedSsid_cmd((struct wmi_t *)m_pWMI, 0, ANY_SSID_FLAG, 0,NULL);
#endif


    if  (m_Connected) {
        // Get the target stats to update the connected BSS RSSI
        if (wmi_get_stats_cmd((wmi_t*)m_pWMI) != A_OK) {
            Status = NDIS_STATUS_FAILURE;
        }
#ifdef ALLOW_SCAN_IN_CONNECTED
        if  (Ndis802_11Infrastructure != m_InfrastructureMode)
        {
#endif
            goto done;
#ifdef ALLOW_SCAN_IN_CONNECTED
        }
#endif
    }

#ifdef ALLOW_SCAN_IN_CONNECTED
    if ((!m_Connected) && (m_WMIBssFilter !=  ALL_BSS_FILTER)) {
#else
    if (m_WMIBssFilter !=  ALL_BSS_FILTER) {
#endif
        wmiStatus = wmi_bssfilter_cmd((struct wmi_t *)m_pWMI, ALL_BSS_FILTER, 0);
        if (wmiStatus == A_OK) {
            m_WMIBssFilter = ALL_BSS_FILTER;
        } else {
            Status = NDIS_STATUS_FAILURE;
            goto done;
        }
    }

    wmiStatus = wmi_startscan_cmd((struct wmi_t *)m_pWMI, WMI_LONG_SCAN, FALSE, FALSE, 0, 0, 0, NULL );

    if (A_OK != wmiStatus)
        Status = NDIS_STATUS_FAILURE;

done:
    return Status;
}

NDIS_STATUS
CAR6KMini::Set802_11PrivacyFilter(
    IN NDIS_802_11_PRIVACY_FILTER Filter)
{
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11ReloadDefaults(
    IN NDIS_802_11_RELOAD_DEFAULTS Filter)
{
    return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::SetCAr6k802_11AuthAlg(
    CAR6K_802_11_AUTH_ALG AuthAlg)
{
    m_AuthAlg = AuthAlg;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6k802_11CCKMAuthMode(
    CAR6K_802_11_CCKM_AUTHENTICATION_MODE CCKMAuthenticationMode)
{
    m_CCKMAuthenticationMode = CCKMAuthenticationMode;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetResumeWlanState(ULONG state)
{
    m_ResumeWlanState = state;
    return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::Set802_11AuthenticationMode(
    IN NDIS_802_11_AUTHENTICATION_MODE Mode)
{
    if ( m_AuthenticationMode == Ndis802_11AuthModeAutoSwitch ) {
        return NDIS_STATUS_NOT_SUPPORTED;
    }
    if ((Mode == Ndis802_11AuthModeOpen) || (Mode == Ndis802_11AuthModeShared) ||
        (Mode == Ndis802_11AuthModeWPA) || (Mode == Ndis802_11AuthModeWPAPSK) ||
        (Mode == Ndis802_11AuthModeWPA2PSK) || (Mode == Ndis802_11AuthModeWPA2)) {
            m_AuthenticationMode = Mode;
            return NDIS_STATUS_SUCCESS;
    } else {
        return NDIS_STATUS_INVALID_DATA;
    }

}

NDIS_STATUS
CAR6KMini::Set802_11EncryptionStatus(
    IN NDIS_802_11_ENCRYPTION_STATUS EncryptionStatus)
{
    m_EncryptionStatus = EncryptionStatus;

#ifdef WAPI_ENABLE
    m_wapimode = 0;  //clean wapi mode
#endif /* WAPI_ENABLE */

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11AddWEP(
    IN NDIS_802_11_WEP *pWEP)
{
    BYTE         KeyIndex;
    A_UINT8      KeyUsage = 0;
    A_STATUS     wmiStatus;
    AP_MODE_CMD *papCmd = &m_apCmd;

    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: AddWEP from Supplicant\n");

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (pWEP->KeyIndex & 0x80000000) {
        KeyUsage |= TX_USAGE;
    }

    if (!(pWEP->KeyIndex & 0x40000000)) {
        KeyUsage |= GROUP_USAGE;
    }

    KeyIndex = (BYTE)(pWEP->KeyIndex & 0xff);

    if (KeyUsage & TX_USAGE && !(KeyUsage & GROUP_USAGE)) {
        //pairwise key and index is not 0
        if (KeyIndex != 0) {
            return NDIS_STATUS_INVALID_DATA;
        }
    }

    if (KeyIndex < WMI_MIN_KEY_INDEX ||
        KeyIndex > WMI_MAX_KEY_INDEX) {
        return NDIS_STATUS_INVALID_DATA;
    }

    if (pWEP->KeyLength <= sizeof(m_Key[KeyIndex])) {
        m_cbKey[KeyIndex] = pWEP->KeyLength;
        memcpy(m_Key[KeyIndex], pWEP->KeyMaterial, pWEP->KeyLength);
        m_KeyIndex[KeyIndex] = KeyIndex;
        m_KeyRSC[KeyIndex] = 0;
        m_KeyUsage[KeyIndex] = KeyUsage;
    }

    // Add the key to the target
    if (m_EncryptionStatus == Ndis802_11WEPEnabled) {
        wmiStatus = wmi_addKey_cmd((wmi_t *)m_pWMI,
                                (A_UINT8)KeyIndex,
                                WEP_CRYPT,
                                m_KeyUsage[KeyIndex],
                                (A_UINT8)m_cbKey[KeyIndex],
                                (A_UINT8*)&m_KeyRSC[KeyIndex],
                                m_Key[KeyIndex],KEY_OP_INIT_VAL,NULL,SYNC_BOTH_WMIFLAG);
        if (wmiStatus != A_OK) {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_addKey_cmd failed = %u\n", wmiStatus);
            return NDIS_STATUS_FAILURE;
        }
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RemoveWEP(
    IN NDIS_802_11_KEY_INDEX KeyIndex)
{
    A_STATUS wmiStatus;

    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: RemoveWEP from Supplicant\n");

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    if (KeyIndex >= 4) {
        return NDIS_STATUS_FAILURE;
    }

    memset(m_Key[KeyIndex], 0, sizeof(m_Key[KeyIndex]));
    m_cbKey[KeyIndex] = 0;

    wmiStatus = wmi_deleteKey_cmd((wmi_t *)m_pWMI, (BYTE)KeyIndex);
    if (wmiStatus != A_OK) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_deleteKey_cmd failed = %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11AddKey(
    IN NDIS_802_11_KEY *pKey, IN BOOL bAddKrk)
{
    BYTE         KeyIndex, tempKeyVal, tempKeyMaterial[32];
    A_UINT8      KeyUsage = 0;
    A_STATUS     wmiStatus;
    A_INT32          i = 0;
    CRYPTO_TYPE  keyType = NONE_CRYPT;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    if (pKey->KeyIndex & 0x80000000) {
        KeyUsage |= TX_USAGE;
    }

    if (!(pKey->KeyIndex & 0x40000000)) {
        KeyUsage |= GROUP_USAGE;
    }

    KeyIndex = (BYTE)(pKey->KeyIndex & 0xff);
    if (KeyUsage & TX_USAGE && !(KeyUsage & GROUP_USAGE)) {
        //pairwise key and index is not 0
        if (KeyIndex != 0) {
            return NDIS_STATUS_INVALID_DATA;
        }
    }

    if (KeyIndex < WMI_MIN_KEY_INDEX ||
        KeyIndex > WMI_MAX_KEY_INDEX) {
        return NDIS_STATUS_INVALID_DATA;
    }

    if( (KeyUsage & GROUP_USAGE) == GROUP_USAGE )
    {
        keyType = m_GrpwiseCryptoType;
    } else {

        keyType = m_PairwiseCryptoType;
    }

    /* Set the keyType to WEP_CRYPT for the case of this OID being called because of a profile
     * switch to 802.1x/SharedAuth/AutoSwitch from a connected profile. This is because the encryption status
     * OID is got only after the AddKey OID.
     */
    if ((m_AuthenticationMode == Ndis802_11AuthModeShared ||
        m_AuthenticationMode == Ndis802_11AuthModeAutoSwitch || m_AuthenticationMode == Ndis802_11AuthModeOpen)) {

        keyType = WEP_CRYPT;
    }

    // TODO - to check BSSID in pKey and the AP address for pairwise key

    memcpy(tempKeyMaterial, pKey->KeyMaterial, pKey->KeyLength);
    if (keyType == TKIP_CRYPT) {

    // if its TKIP and bit 28 is set swap tx and rx mic
        if (!(pKey->KeyIndex & 0x10000000)) {
            for (i=0;i<8; i++) {
                tempKeyVal = tempKeyMaterial[16+i]; //rx
                tempKeyMaterial[16+i] =
                    tempKeyMaterial[24+i]; //rx = tx
                tempKeyMaterial[24+i] = tempKeyVal; //tx = rx
            }
        }
    }

    if (pKey->KeyLength <= sizeof(m_Key[KeyIndex]))
    {
        m_cbKey[KeyIndex] = pKey->KeyLength;
        memcpy(m_Key[KeyIndex], tempKeyMaterial, pKey->KeyLength);
        m_KeyIndex[KeyIndex] = KeyIndex;

        m_KeyRSC[KeyIndex] = pKey->KeyRSC;

        m_KeyUsage[KeyIndex] = KeyUsage;
    }

    // Add the key to the target only if Encryption is enabled.
    if (NONE_CRYPT != m_PairwiseCryptoType) {
    if(bAddKrk)
    {
        wmiStatus = wmi_add_krk_cmd((wmi_t *)m_pWMI, m_Key[KeyIndex]);
    }
    else
    {
        wmiStatus = wmi_addKey_cmd((wmi_t *)m_pWMI,
                   (A_UINT8)KeyIndex,
                   keyType,
                   m_KeyUsage[KeyIndex],
                   (A_UINT8)m_cbKey[KeyIndex],
                   (A_UINT8*)&m_KeyRSC[KeyIndex],
                   m_Key[KeyIndex],KEY_OP_INIT_VAL,NULL,SYNC_BOTH_WMIFLAG);
    }

        if (wmiStatus != A_OK) {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_addKey_cmd failed = %u\n", wmiStatus);
            return NDIS_STATUS_FAILURE;
        }
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RemoveKey(
    IN NDIS_802_11_REMOVE_KEY *pKey)
{
    A_STATUS wmiStatus;
    BYTE     KeyIndex;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    KeyIndex = (BYTE)(pKey->KeyIndex & 0xff);

    memset(m_Key[KeyIndex], 0, sizeof(m_Key[KeyIndex]));
    m_cbKey[KeyIndex] = 0;

    wmiStatus = wmi_deleteKey_cmd((wmi_t *)m_pWMI, KeyIndex);
    if (wmiStatus != A_OK) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_deleteKey_cmd failed = %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11AssociationInformation(
    IN NDIS_802_11_ASSOCIATION_INFORMATION *pInfo,
    IN ULONG                                cbInfo)
{
    return NDIS_STATUS_SUCCESS;
}

#ifdef WAPI_ENABLE
NDIS_STATUS
CAR6KMini::Set802_11AssocInfo(
    IN WMI_SET_ASSOC_INFO_CMD *pAssocInfo)
{
    A_STATUS wmiStatus;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (pAssocInfo->ieType > WMI_MAX_ASSOC_INFO_TYPE) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_associnfo_cmd failed, ieType invalid\n");
        return NDIS_STATUS_FAILURE;
    }

    wmiStatus = wmi_set_appie_cmd((wmi_t *)m_pWMI, WMI_FRAME_ASSOC_REQ,
                                  pAssocInfo->bufferSize, pAssocInfo->assocInfo);

    if (wmiStatus != A_OK) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_associnfo_cmd failed = %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetWapiMode(IN INT status)
{
    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    m_wapimode = status;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetWapiKey(IN WPI_KEY * pWpiKey)
{
    A_UINT8      KeyUsage = 0;
    A_UINT8      KeyIndex;
    A_UINT8      KeyLen;
    A_UINT8      Key[32];   //store EK and CK together
    A_STATUS     wmiStatus;
    CRYPTO_TYPE  keyType = WAPI_CRYPT;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (!m_wapimode) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: SetWapiKey but WAPI is not enabled\n");
        return NDIS_STATUS_FAILURE;
    }

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: Add WPI Key from Supplicant\n");
    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "KeyType %d, KeyId %d, lenWPIEK %d lenWPICK %d\n",
                      pWpiKey->KeyType, pWpiKey->KeyID, pWpiKey->lenWPIEK, pWpiKey->lenWPICK);

    keyType = WAPI_CRYPT;
    if (pWpiKey->KeyType == ktGroupKey) {
        KeyUsage |= GROUP_USAGE;
        if (pWpiKey->KeyID < 0 || pWpiKey->KeyID > 3) {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: SetWapiKey b/mcast keyIdx should be [0-3]\n");
            return NDIS_STATUS_FAILURE;
        }
    } else {
        if (pWpiKey->KeyID < 4 || pWpiKey->KeyID > 7) {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: SetWapiKey ucast keyIdx should be [4-7]\n");
            return NDIS_STATUS_FAILURE;
        }
    }
    if (pWpiKey->PN == NULL) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: SetWapiKey PN NULL\n");
    }
    KeyIndex = pWpiKey->KeyID;
    KeyLen = pWpiKey->lenWPIEK + pWpiKey->lenWPICK;

    memcpy(Key, pWpiKey->WPIEK, 16);
    memcpy(&Key[16], pWpiKey->WPICK, 16);

    wmiStatus = wmi_addKey_cmd((wmi_t *)m_pWMI,
                               (A_UINT8)KeyIndex,
                               keyType,
                               KeyUsage,
                               KeyLen,
                               pWpiKey->PN,
                               Key, KEY_OP_INIT_WAPIPN,NULL,SYNC_BEFORE_WMIFLAG);

    if (wmiStatus != A_OK) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_addKey_cmd failed = %u\n", wmiStatus);
        return NDIS_STATUS_FAILURE;
    }

    return NDIS_STATUS_SUCCESS;
}
#endif /* WAPI_ENABLE */

NDIS_STATUS
CAR6KMini::Set802_11Test(
    IN NDIS_802_11_TEST *pTest,
    IN ULONG cbBuffer)
{
    switch(pTest->Type) {
        case 1:

            //
            // Aunthentication indication
            //
            NdisMIndicateStatus(m_MiniportAdapterHandle,
                        NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
                        &pTest->AuthenticationEvent, cbBuffer - 2*sizeof(ULONG));
            NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
            break;
        case 2:
            //
            // RSSI trigger indication
            //
            break;
        default:
            return NDIS_STATUS_INVALID_DATA;
    }
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::GetPnpPower(ULONG *state)
{
    if (m_PowerState == NdisDeviceStateD3) {
        *state = 0; // OFF State
    } else {
        *state = 1; // ON State
    }
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::QueryWakeupCapabilities(ULONG *pWowMode)
{
    if (m_Config.wowEnable) {
        return NDIS_STATUS_NOT_SUPPORTED;
    }

    *pWowMode = m_WowMode;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::GetResumeWlanState(ULONG *state)
{
    *state = m_ResumeWlanState;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::GetFirmwareVersion(ULONG *version)
{
    *version = AR6K_SW_VERSION;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::GetRegDomain(UINT *regdomain)
{
    *regdomain = m_RegCode;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::GetTgtStats(AR6K_TARGET_STATS *getStats)
{
    // Return if WMI is not ready yet
    if (!m_WMIReady)
        return NDIS_STATUS_ADAPTER_NOT_READY;
    // get stats from target
    if (ar6000_get_target_stats() != A_OK)
        return NDIS_STATUS_FAILURE;
    memcpy(getStats,&m_tgtAllStats,sizeof(AR6K_TARGET_STATS));
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiDone(
    PVOID Buffer, BOOL bIsDrg)
{
    A_STATUS status;

    if (FALSE == bIsDrg)
    {
        // Call StartEndpoints to get the target out of the BMI phase.
        status = StartEndpoints(TRUE);
    }
    else
    {
        // Send a "BMI_DONE" command to the target, to get it out of
        // BMI mode and into normal operation.
        status = BMIDone (m_pHIFDevice);
    }

    if ( status != A_OK)
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiWriteMemory(
    BMI_WRITE_MEMORY_PARAM *Buffer
    )
{
    A_STATUS status;

    status = BMIWriteMemory (m_pHIFDevice, Buffer->address, Buffer->databuffer, Buffer->length);

    if (status != A_OK )
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::SetCAr6kBmiReadMemory(
    BMI_READ_MEMORY_PARAM *Buffer)
{
    A_STATUS status;

    status = BMIReadMemory(m_pHIFDevice, Buffer->address, Buffer->databuffer, Buffer->length);
    if ( status != A_OK)
    {
        return NDIS_STATUS_FAILURE;
    }


    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiExecute(
    BMI_EXECUTE_PARAM *Buffer)
{
    A_STATUS status;

    status = BMIExecute(m_pHIFDevice, Buffer->address, (A_UINT32 *)&Buffer->param);
    if ( status != A_OK)
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiSetAppStart(
    BMI_SET_APP_START_PARAM *Buffer)
{
    A_STATUS status;

    status = BMISetAppStart(m_pHIFDevice, Buffer->address);
    if ( status != A_OK)
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiWriteSocRegister(
    BMI_WRITE_SOC_REG_PARAM *Buffer)
{
    A_STATUS status;

    status = BMIWriteSOCRegister(m_pHIFDevice, Buffer->address, Buffer->param);

    if ( status != A_OK)
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiReadSocRegister(
    BMI_READ_SOC_REG_PARAM *Buffer)
{
    A_STATUS status;

    status = BMIReadSOCRegister(m_pHIFDevice, Buffer->address, (A_UINT32 *)&Buffer->value);

    if ( status != A_OK)
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiRomPatchInstall(
    BMI_ROMPATCH_INSTALL_PARAM *Buffer)
{
    A_STATUS status;

    status = BMIrompatchInstall(m_pHIFDevice, Buffer->romAddr, Buffer->ramAddr, Buffer->nBytes, Buffer->doActivate, (A_UINT32*)&Buffer->romPatchId );

    if ( status != A_OK)
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiRomPatchUninstall(
    BMI_ROMPATCH_UNINSTALL_PARAM *Buffer)
{
    A_STATUS status;

    status = BMIrompatchUninstall(m_pHIFDevice, Buffer->romPatchId );

    if ( status != A_OK)
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiRomPatchActivate(
    BMI_ROMPATCH_ACTIVATE_PARAM *Buffer)
{
    A_STATUS status;

    status = BMIrompatchActivate(m_pHIFDevice, Buffer->romPatchCount, (A_UINT32*)&Buffer->buffer );

    if ( status != A_OK)
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiRomPatchDeactivate(
    BMI_ROMPATCH_DEACTIVATE_PARAM *Buffer)
{
    A_STATUS status;

    status = BMIrompatchDeactivate(m_pHIFDevice, Buffer->romPatchCount, (A_UINT32*)&Buffer->buffer );

    if ( status != A_OK)
        return NDIS_STATUS_FAILURE;

    return NDIS_STATUS_SUCCESS;
}

#ifdef CONFIG_HOST_TCMD_SUPPORT
NDIS_STATUS
CAR6KMini::SetCAr6kTcmdContTx( TCMD_CONT_TX *tcmd )
{
    if ( m_TcmdPm == TCMD_PM_SLEEP ) {
        return NDIS_STATUS_FAILURE;
    }
    wmi_test_cmd((wmi_t*)m_pWMI,(A_UINT8*)tcmd,sizeof(TCMD_CONT_TX));
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kTcmdContRx( TCMD_CONT_RX *tcmd )
{
    if ( m_TcmdPm == TCMD_PM_SLEEP ) {
        return NDIS_STATUS_FAILURE;
    }
    switch( tcmd->act ) {
        case TCMD_CONT_RX_PROMIS:
        case TCMD_CONT_RX_FILTER:
        case TCMD_CONT_RX_SETMAC:
        case TCMD_CONT_RX_SET_ANT_SWITCH_TABLE:
            wmi_test_cmd((wmi_t*)m_pWMI, (A_UINT8*)tcmd,
                    sizeof(TCMD_CONT_RX));
            break;
        case TCMD_CONT_RX_REPORT:
            if ( !m_WMIReady )
                return NDIS_STATUS_ADAPTER_NOT_READY;
            NdisResetEvent(&m_TcmdReportEvent);
            wmi_test_cmd((wmi_t*)m_pWMI, (A_UINT8*)tcmd,
                    sizeof(TCMD_CONT_RX));
            if ( !NdisWaitEvent(&m_TcmdReportEvent, 10000) ) {
                return NDIS_STATUS_FAILURE;
            }
            tcmd->u.report.totalPkt = m_Tcmd_totalPkt;
            tcmd->u.report.rssiInDBm = m_Tcmd_rssiInDBm;
            tcmd->u.report.crcErrPkt = m_Tcmd_crcError;
            tcmd->u.report.secErrPkt = m_Tcmd_secErr;
            break;
        default:
            return NDIS_STATUS_FAILURE;
    }
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kTcmdPm( TCMD_PM *tcmd )
{
    m_TcmdPm = (TCMD_PM_MODE)tcmd->mode;
    wmi_test_cmd((wmi_t*)m_pWMI, (A_UINT8*)tcmd, sizeof(TCMD_PM));
    return NDIS_STATUS_SUCCESS;
}
#endif //CONFIG_HOST_TCMD_SUPPORT

NDIS_STATUS
CAR6KMini::SetCAr6kHostSleepMode(WMI_SET_HOST_SLEEP_MODE_CMD *cmd)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_STATUS wmiStatus;

    wmiStatus = wmi_set_host_sleep_mode_cmd((wmi_t *)m_pWMI, cmd);
    if (wmiStatus != A_OK) {
       return NDIS_STATUS_FAILURE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SetCAr6kWowMode(WMI_SET_WOW_MODE_CMD *cmd)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_STATUS wmiStatus;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    wmiStatus = wmi_set_wow_mode_cmd((wmi_t *)m_pWMI, cmd);
    if (wmiStatus != A_OK) {
        return NDIS_STATUS_FAILURE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SetCAr6kWowAddPattern(WMI_ADD_WOW_PATTERN_CMD *cmd)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_STATUS wmiStatus;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    wmiStatus = wmi_add_wow_pattern_cmd((wmi_t *)m_pWMI, cmd, cmd->filter,
                     cmd->filter+cmd->filter_size, cmd->filter_size);
    if (wmiStatus != A_OK) {
        return NDIS_STATUS_FAILURE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SetCAr6kWowDelPattern(WMI_DEL_WOW_PATTERN_CMD *cmd)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_STATUS wmiStatus;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    wmiStatus = wmi_del_wow_pattern_cmd((wmi_t *)m_pWMI, cmd);
    if (wmiStatus != A_OK) {
        return NDIS_STATUS_FAILURE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SetCAr6kDbgLogCfgModule(WMIX_DBGLOG_CFG_MODULE_CMD *cfg)
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_STATUS wmiStatus;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    wmiStatus = wmi_config_debug_module_cmd((wmi_t *)m_pWMI, cfg->config.cfgmmask, cfg->config.cfgtsr,
                    cfg->config.cfgrep, cfg->config.cfgsize, cfg->config.cfgvalid);

    if (wmiStatus != A_OK) {
        return NDIS_STATUS_FAILURE;

    }


    return Status;
}

#ifdef SUPPORT_WPA2
NDIS_STATUS
CAR6KMini::Set802_11PMKID(OUT NDIS_802_11_PMKID* pPMKID,
                          IN  ULONG              cbBuffer)
{
    A_STATUS status;
    A_UINT32 i = 0, count = 0;
    BSSID_INFO  *Temp;
    NDIS_802_11_PMKID *tempPMKID = NULL;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    count = pPMKID->BSSIDInfoCount;

    if(count > NO_OF_PMKIDS_SUPPORTED) {
        return NDIS_STATUS_INVALID_DATA;
    }

    // Microsoft recommands to overwrite PMKIDs everytime, so flush PMKIDs before setting
    if (m_pPMKID) {
        // This means supplicant is flushing the PMKIDs
        for (i = 0; i < m_pPMKID->BSSIDInfoCount; i++) {
            Temp = &m_pPMKID->BSSIDInfo[i];
            status = wmi_setPmkid_cmd((wmi_t*)m_pWMI, Temp->BSSID, NULL, 0);
        }
    }

    if (count == 0) {
        A_FREE(m_pPMKID);
        m_pPMKID = NULL;
        return NDIS_STATUS_SUCCESS;
    }

    tempPMKID = (NDIS_802_11_PMKID *)A_MALLOC(pPMKID->Length);

    if (NULL != tempPMKID)
    {
        tempPMKID->Length = pPMKID->Length;
        tempPMKID->BSSIDInfoCount = count;

        for  (i = 0; i < count; i++) {
            Temp = &pPMKID->BSSIDInfo[i];
            // Do not use PMKID when CCKM is enabled
            if (m_CCKMAuthenticationMode == car6k802_11AuthModeDisableCCKM) {
                status = wmi_setPmkid_cmd((wmi_t *)m_pWMI, Temp->BSSID, Temp->PMKID, 1);

                if (status != A_OK) {
                    /*
                    * Should we rollback to the original state??
                    */
                    A_FREE(tempPMKID);
                    return NDIS_STATUS_FAILURE;
                }
            }

            memcpy(tempPMKID->BSSIDInfo[i].BSSID, Temp->BSSID, 6);
            memcpy(tempPMKID->BSSIDInfo[i].PMKID, Temp->PMKID, 16);
        }

        if (m_pPMKID != NULL) {
            A_FREE(m_pPMKID);
        }

        m_pPMKID = tempPMKID;
    }
    return NDIS_STATUS_SUCCESS;
}
#endif //SUPPORT_WPA2

NDIS_STATUS
CAR6KMini::Set802_11ChannelParams(WMI_CHANNEL_PARAMS_CMD *buf)
{
    A_STATUS   wmiStatus = A_OK;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    wmiStatus = wmi_set_channelParams_cmd((wmi_t *)m_pWMI, buf->scanParam,(WMI_PHY_MODE)buf->phyMode,
                                          buf->numChannels, buf->channelList);
    if(wmiStatus == A_OK)
    {
        m_queryphyMode = (WMI_PHY_MODE)buf->phyMode;
    }

    NDIS_STATUS Status  = NDIS_STATUS_SUCCESS;

    return ((wmiStatus == A_OK)?NDIS_STATUS_SUCCESS:NDIS_STATUS_FAILURE);
}

NDIS_STATUS
CAR6KMini::Set802_11ScanParams(WMI_SCAN_PARAMS_CMD *buf)
{
    A_STATUS   wmiStatus = A_OK;
    A_UINT16 minact_chdwell_time;
    A_UINT16 maxact_chdwell_time;
    A_UINT8     scanCtrlFlags;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    minact_chdwell_time = 0;
    scanCtrlFlags=0;
    minact_chdwell_time = buf->minact_chdwell_time;
    scanCtrlFlags=buf->scanCtrlFlags;


    maxact_chdwell_time=buf->maxact_chdwell_time;
    wmiStatus = wmi_scanparams_cmd((wmi_t *)m_pWMI,buf->fg_start_period,buf->fg_end_period,buf->bg_period,
                                   minact_chdwell_time,maxact_chdwell_time,
                                   buf->pas_chdwell_time,buf->shortScanRatio,
                                   scanCtrlFlags, buf->max_dfsch_act_time,
                                   buf->maxact_scan_per_ssid);

    return ((wmiStatus == A_OK)?NDIS_STATUS_SUCCESS:NDIS_STATUS_FAILURE);
}


NDIS_STATUS
CAR6KMini::SetCAr6kCreateQOS (WMI_CREATE_PSTREAM_CMD *cmd)
{
    NDIS_STATUS Status  = NDIS_STATUS_SUCCESS;
    A_STATUS wmiStatus = A_OK;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    wmiStatus = wmi_verify_tspec_params (cmd, m_Config.tspecCompliance);

    if (wmiStatus == A_OK)
        wmiStatus = wmi_create_pstream_cmd((wmi_t*)m_pWMI, cmd);


    switch (wmiStatus) {

        case A_OK:
            Status = NDIS_STATUS_SUCCESS;
            break;
        case A_EBUSY :
            Status = NDIS_STATUS_MEDIA_BUSY;
            break;
        case A_NO_MEMORY:
            Status =  NDIS_STATUS_BUFFER_OVERFLOW;
            break;
        case A_EINVAL:
            Status =  NDIS_STATUS_INVALID_DATA;
            break;

        default:
            Status =  NDIS_STATUS_FAILURE;
            break;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SetCAr6kDeleteQOS(WMI_DELETE_PSTREAM_CMD *cmd)
{

    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    wmiStatus = wmi_delete_pstream_cmd((wmi_t*)m_pWMI, cmd->trafficClass, cmd->tsid);

    switch (wmiStatus) {

        case A_OK:
            Status = NDIS_STATUS_SUCCESS;
            break;
        case A_EBUSY :
            Status = NDIS_STATUS_MEDIA_BUSY;
            break;
        case A_NO_MEMORY:
            Status =  NDIS_STATUS_BUFFER_OVERFLOW;
            break;
        case A_EINVAL:
            Status =  NDIS_STATUS_INVALID_DATA;
            break;

        default:
            Status =  NDIS_STATUS_FAILURE;
            break;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SetCAr6kWMM(WMI_SET_WMM_CMD *cmd)
{
    A_STATUS wmiStatus = A_OK;

    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    WMI_WMM_STATUS wmmStatus = WMI_WMM_DISABLED;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (0x01 == cmd->status)
    {
        wmmStatus = WMI_WMM_ENABLED;
    }
    else
    {
        wmmStatus = WMI_WMM_DISABLED;
    }

    wmiStatus = wmi_set_wmm_cmd((wmi_t*)m_pWMI, wmmStatus);

    switch (wmiStatus) {

        case A_OK:
            Status = NDIS_STATUS_SUCCESS;
            break;
        case A_EBUSY :
            Status = NDIS_STATUS_MEDIA_BUSY;
            break;
        case A_NO_MEMORY:
            Status =  NDIS_STATUS_BUFFER_OVERFLOW;
            break;
        case A_EINVAL:
            Status =  NDIS_STATUS_INVALID_DATA;
            break;

        default:
            Status =  NDIS_STATUS_FAILURE;
            break;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SetCAr6kTXOP(WMI_SET_WMM_TXOP_CMD *cmd)
{

    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    WMI_TXOP_CFG txopStatus = WMI_TXOP_DISABLED;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (0x01 == cmd->txopEnable)
    {
        txopStatus = WMI_TXOP_ENABLED;
    }
    else
    {
        txopStatus = WMI_TXOP_DISABLED;
    }

    wmiStatus = wmi_set_wmm_txop((wmi_t*)m_pWMI, txopStatus);

    switch (wmiStatus) {

        case A_OK:
            Status = NDIS_STATUS_SUCCESS;
            break;
        case A_EBUSY :
            Status = NDIS_STATUS_MEDIA_BUSY;
            break;
        case A_NO_MEMORY:
            Status =  NDIS_STATUS_BUFFER_OVERFLOW;
            break;
        case A_EINVAL:
            Status =  NDIS_STATUS_INVALID_DATA;
            break;

        default:
            Status =  NDIS_STATUS_FAILURE;
            break;
    }

    return Status;
}


NDIS_STATUS
CAR6KMini::SetBTStatus(WMI_SET_BT_STATUS_CMD *cmd)
{

    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    wmiStatus = wmi_set_bt_status_cmd((wmi_t*)m_pWMI, cmd->streamType, cmd->status);

    if (A_OK != wmiStatus)
    {
        Status = NDIS_STATUS_FAILURE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SetBTParams(WMI_SET_BT_PARAMS_CMD *cmd)
{

    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    if (!m_WMIReady) {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    wmiStatus = wmi_set_bt_params_cmd((wmi_t*)m_pWMI, cmd);

    if (A_OK != wmiStatus)
    {
        Status = NDIS_STATUS_FAILURE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::SetRoamCtrlParams(WMI_SET_ROAM_CTRL_CMD *cmd)
{
    A_STATUS wmiStatus = A_OK;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    A_INT32 size = sizeof(WMI_SET_ROAM_CTRL_CMD);

    if (!m_WMIReady)
    {
        return NDIS_STATUS_FAILURE;
    }

    if (cmd->roamCtrlType == WMI_SET_HOST_BIAS) {
        if (cmd->info.bssBiasInfo.numBss > 1) {
            size += (cmd->info.bssBiasInfo.numBss - 1) * sizeof(WMI_BSS_BIAS);
        }
    }

    wmiStatus = wmi_set_roam_ctrl_cmd((wmi_t*)m_pWMI, cmd, size);

    if (A_OK != wmiStatus)
    {
        Status = NDIS_STATUS_FAILURE;
    }

    return Status;
}

NDIS_STATUS
CAR6KMini::GetQOSQueue(struct ar6000_queuereq *qreq, PULONG pcbNeeded)
{

    qreq->activeTsids = wmi_get_mapped_qos_queue((wmi_t*)m_pWMI, qreq->trafficClass);

    *pcbNeeded = sizeof(struct ar6000_queuereq);

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetWPS(
    IN A_INT32 status)
{
    wmi_set_wsc_status_cmd((wmi_t *)m_pWMI, status);

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kIbssChannel(USHORT chan)
{
    A_UINT32 chanInMHz;

    do
    {
        if (chan == 14) {
            chanInMHz = 2484;
            break;
        }
        if (chan < 14) {    /* 0-13 */
            chanInMHz = (2407 + (chan * 5));
            break;
        }
        if (chan < 27) {    /* 15-26 */
            chanInMHz = (2512 + ((chan - 15) * 20));
            break;
        }

        chanInMHz = (5000 + (chan * 5));

    } while (FALSE);

    m_Config.ibssChannel = chanInMHz * 1000;

    return NDIS_STATUS_SUCCESS;
}

void
CAR6KMini::GetChannelHint (IN NDIS_802_11_SSID *SSID)
{
    bss_t *pWmiSsidnode = NULL;

    //
    // set SSIDMatch flag is TRUE, so that the below function need not check whether the profile
    // is WPA or WPA2
    //
    pWmiSsidnode = wmi_find_Ssidnode ((struct wmi_t *)m_pWMI, SSID->Ssid,
                                      SSID->SsidLength, FALSE, TRUE);

    m_ChannelHint = (pWmiSsidnode)?pWmiSsidnode->ni_cie.ie_chan:0;

    //
    // decrement the reference count
    //
    wmi_node_return ((struct wmi_t *)m_pWMI, pWmiSsidnode);
}

NDIS_STATUS
CAR6KMini::SetCAr6kAtimWindow (BYTE atimWindow)
{
    if (m_Connected)
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K::Set ATIM can not be set in connected state\n");
        return NDIS_STATUS_NOT_SUPPORTED;
    }
    m_Config.atimWindow = atimWindow;
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kIBSSPm (WMI_IBSS_PM_CAPS_CMD * adhocPm)
{
    if (m_Connected)
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K::Set IBSSPM can not be set in connected state\n");
        return NDIS_STATUS_NOT_SUPPORTED;
    }

    wmi_ibsspmcaps_cmd((struct wmi_t *)m_pWMI,
                        adhocPm->power_saving,
                        adhocPm->ttl,
                        adhocPm->atim_windows,
                        adhocPm->timeout_value);

    m_IBSS_PSEnable = adhocPm->power_saving;
    m_Config.ibssPSEnable = m_IBSS_PSEnable;

    if (m_IBSS_PSEnable && adhocPm->atim_windows) {
        m_Config.atimWindow = adhocPm->atim_windows;
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kPm (WMI_POWER_PARAMS_CMD * pm)
{
    wmi_pmparams_cmd((struct wmi_t *)m_pWMI, pm->idle_period,
                         pm->pspoll_number, pm->dtim_policy);

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAR6kAppIE (WMI_SET_APPIE_CMD * appIE)
{
    wmi_set_appie_cmd((struct wmi_t *)m_pWMI, appIE->mgmtFrmType,
                          appIE->ieLen, appIE->ieInfo);

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAR6kFixRates (WMI_FIX_RATES_CMD * fixRates)
{
    wmi_set_fixrates_cmd((struct wmi_t *)m_pWMI, fixRates->fixRateMask);

    return NDIS_STATUS_SUCCESS;
}

void
CAR6KMini::ConnectIndicationPostProc ()
{
    A_UINT32   i = 0;

    // Step 1 Rollback Listen interval
    if( m_80211_PowerMode == Ndis802_11PowerModeFast_PSP) {

        if (m_ListenInterval < NDIS_MAXPSP_LISTEN_INTERVAL)
        {
            wmi_listeninterval_cmd((wmi_t *)m_pWMI, m_ListenInterval,0);
        }

    }

    // Step 2 BSS Filter
    if (m_Config.bkScanEnable)
    {
#ifdef OS_ROAM_MANAGEMENT
        if (m_osRoamControl)
        {
            m_WMIBssFilter = ALL_BSS_FILTER;
        }
        else
        {
            m_WMIBssFilter = ALL_BUT_BSS_FILTER;
        }
#else
        m_WMIBssFilter = ALL_BUT_BSS_FILTER;
#endif
    }
    else
    {
        m_WMIBssFilter = NONE_BSS_FILTER;
    }

    wmi_bssfilter_cmd ((struct wmi_t *)m_pWMI, m_WMIBssFilter, 0);

    // Add the key here for WEP encryption with open Auth or Autoswitch.
    // For Shared Auth, the keys are plumbed before the connect cmd is issued and
    // for WPA/WPA2, the keys are plumbed when the AddKey OID comes.

    if (((m_AuthenticationMode == Ndis802_11AuthModeOpen) ||
        (m_AuthenticationMode == Ndis802_11AuthModeAutoSwitch)) &&
        (m_EncryptionStatus == Ndis802_11WEPEnabled))
    {
        for (i = 0; i < 4; i++)
        {
            if (m_cbKey[i] != 0) {
                wmi_addKey_cmd ((wmi_t *)m_pWMI,
                                (A_UINT8)m_KeyIndex[i],
                                WEP_CRYPT,
                                m_KeyUsage[i],
                                (A_UINT8)m_cbKey[i],
                                NULL,
                                m_Key[i],
                                KEY_OP_INIT_VAL,NULL,SYNC_BOTH_WMIFLAG);
            }
        }
    }

    return;
}

#ifdef OS_ROAM_MANAGEMENT
A_STATUS
CAR6KMini::ar6000_get_channel_list()
{
    A_STATUS status = A_OK;

    NdisResetEvent (&m_tgtChannelListEvent);

    status = wmi_get_channelList_cmd ((wmi_t*)m_pWMI);

    if ( status != A_OK)
    {
        return status;
    }

    if( !NdisWaitEvent(&m_tgtChannelListEvent, AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_CHANNEL_LIST) )
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - No CHANNEL_LIST event after %u ms\n", AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_CHANNEL_LIST);
        status = A_ERROR;
    }

    return status;
}

NDIS_STATUS
CAR6KMini::Get802_11RoamControl(OUT PNDIS_802_11_NIC_ROAM_CONTROL pCtl)
{
    //
    // MSFT set this variable as FALSE, means AR6K supports
    // Roamcontrol by default
    //
    if (m_osRoamControl)
    {
        *pCtl = Ndis802_11NicRoamControlOff;
    }
    else
    {
        *pCtl = Ndis802_11NicRoamControlOn;
    }
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11ChannelList(OUT PNDIS_802_11_CHANNEL_LIST pList,
                                IN ULONG cbList,
                                OUT ULONG *pcbNeeded)
{
    ULONG cbNeeded;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }


    if (0 == m_cChannelList)
    {
        //get channel list from target
        if (ar6000_get_channel_list() != A_OK)
        {
            return NDIS_STATUS_FAILURE;
        }
    }

    cbNeeded = m_cChannelList * sizeof(pList->ChannelFrequency[0]) +
               FIELD_OFFSET(NDIS_802_11_CHANNEL_LIST, ChannelFrequency);

    if (cbList < cbNeeded)
    {
        *pcbNeeded = cbNeeded;
        return NDIS_STATUS_BUFFER_TOO_SHORT;
    }

    pList->NumberOfChannels = m_cChannelList;

    for (ULONG i = 0; i < m_cChannelList; i++)
    {
        pList->ChannelFrequency[i] = m_ChannelList[i] * 1000;
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11RateRssiFilter (OUT PNDIS_802_11_BSSID_RATE_RSSI_FILTER pFilter)
{
    pFilter->TxRateFilterParam  = 1;
    pFilter->RxRateFilterParam  = 1;
    pFilter->RssiFilterParam    = 10;

    if (m_RateInfo.TxRateFiltered)
    {
        pFilter->TxRateFilterParam  = m_RateInfo.TxRateFiltered;
    }

    if (m_RateInfo.RxRateFiltered)
    {
        pFilter->RxRateFilterParam  = m_RateInfo.RxRateFiltered;
    }
    if (m_RateInfo.RssiFiltered)
    {
        pFilter->RssiFilterParam    = m_RateInfo.RssiFiltered;
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11RateRssiInfo(OUT PNDIS_802_11_BSSID_RATE_RSSI_INFO pInfo)
{

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    //
    // get stats from target
    //
    if (ar6000_get_target_stats() != A_OK)
    {
        return NDIS_STATUS_FAILURE;
    }

    memcpy(pInfo, &m_RateInfo, sizeof(NDIS_802_11_BSSID_RATE_RSSI_INFO));

    if ((FALSE == m_Connected) && (TRUE == m_osRoamControl))
        pInfo->RssiFiltered = 0;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RoamControl(IN PNDIS_802_11_NIC_ROAM_CONTROL pCtl)
{
    A_STATUS wmiStatus              = A_OK;
    WMI_SET_ROAM_CTRL_CMD roamCmd   = {0};
    int size                        = sizeof(WMI_SET_ROAM_CTRL_CMD);


    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (Ndis802_11NicRoamControlOff == *pCtl)
    {

        if (!m_osRoamControl)
        {
            RETAILMSG (1,(L"Roaming Manager ON %x\n"));
            //
            // Disable roaming
            //
            roamCmd.roamCtrlType            = WMI_SET_ROAM_MODE;
            roamCmd.info.roamMode           = WMI_LOCK_BSS_MODE;
            roamCmd.info.bssBiasInfo.numBss = 1;

            wmiStatus = wmi_set_roam_ctrl_cmd ((wmi_t *)m_pWMI, &roamCmd, size);

            if (wmiStatus != A_OK)
            {
                return NDIS_STATUS_FAILURE;
            }

            // Set bss filter
            if (m_WMIBssFilter != PROBED_SSID_FILTER)
            {
                m_WMIBssFilter = PROBED_SSID_FILTER;
                wmi_bssfilter_cmd ((struct wmi_t *)m_pWMI, m_WMIBssFilter, 0);
            }

            wmiStatus = wmi_disctimeout_cmd((wmi_t *)m_pWMI, 0x00);
            if (wmiStatus != A_OK)
            {
                return NDIS_STATUS_FAILURE;
            }

            m_osRoamControl = TRUE;
        }

        NDIS_DEBUG_PRINTF (ATH_DEBUG_ROAM, "AR6K: OS roam control ON\n");

        //
        // Enable scan indication
        //
        wmi_scan_indication ((wmi_t *)m_pWMI);
    }
    else
    {
        if (m_osRoamControl)
        {
            // Restore bss filter
            if ((m_Connected) && (ALL_BSS_FILTER == m_WMIBssFilter))
            {
                if (m_Config.bkScanEnable)
                {
                    m_WMIBssFilter = ALL_BUT_BSS_FILTER;
                }
                else
                {
                    m_WMIBssFilter = NONE_BSS_FILTER;
                }

                wmi_bssfilter_cmd ((struct wmi_t *)m_pWMI, m_WMIBssFilter, 0);
            }

            //
            // Restore roaming
            //
            roamCmd.roamCtrlType            = WMI_SET_ROAM_MODE;
            roamCmd.info.roamMode           = WMI_DEFAULT_ROAM_MODE;
            roamCmd.info.bssBiasInfo.numBss = 0;

            wmiStatus = wmi_set_roam_ctrl_cmd((wmi_t *)m_pWMI, &roamCmd, size);

            if (m_Config.discTimeout)
            {
                 wmiStatus = wmi_disctimeout_cmd((wmi_t *)m_pWMI, m_Config.discTimeout);

            }
            if (wmiStatus != A_OK)
            {
                return NDIS_STATUS_FAILURE;
            }
            m_osRoamControl = FALSE;

        }

        NDIS_DEBUG_PRINTF (ATH_DEBUG_ROAM, "AR6K: OS roam control OFF\n");
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11ChannelScan(IN PNDIS_802_11_BSSID_CHANNEL_SCAN pChInfo)
{
    A_STATUS   wmiStatus = A_OK;
    A_UINT16 chList[1];
    A_UINT16 fgenable, bkenable;

    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    chList[0] = (A_UINT16)(pChInfo->ChannelFrequency/1000);

    if (A_OK == wmiStatus)
    {

        if (m_scanCount > 11)
    {
             wmi_free_allnodes((struct wmi_t *)m_pWMI);
             m_scanCount = 0;
    }
             m_scanCount++;
        //
        // Start the scan when roaming manager is started and theconnection is established with AP
        //

        wmiStatus = wmi_startscan_cmd ((struct wmi_t *)m_pWMI, WMI_LONG_SCAN,
                            TRUE,
                            FALSE,
                            AR6K_DWELL_TIME,
                            AR6K_DWELL_TIME + (A_UINT16)pChInfo->ScanMaxTime,
                            1,
                            chList);

    }

    if (wmiStatus != A_OK)
    {
        if (m_Connected)
        {
            //
            // Restore scan parameters in case of error
            //
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
             wmiStatus = wmi_scanparams_cmd((wmi_t *)m_pWMI,fgenable,0,bkenable,
                                        0,0,0,WMI_SHORTSCANRATIO_DEFAULT, 0, 0, 0);
        }
        return NDIS_STATUS_FAILURE;
    }

    m_ChannelScan = TRUE;

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RateRssiFilter (IN PNDIS_802_11_BSSID_RATE_RSSI_FILTER pFilter)
{

    m_RateInfo.TxRateFiltered   = pFilter->TxRateFilterParam;
    m_RateInfo.RxRateFiltered   = pFilter->RxRateFilterParam;
    m_RateInfo.RssiFiltered     = pFilter->RssiFilterParam;

    return NDIS_STATUS_SUCCESS;
}

#endif //OS_ROAM_MANAGEMENT

NDIS_STATUS
CAR6KMini::SetCAR6kWMINodeAge(A_UINT32 nodeage)
{
    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (!m_Config.byPassWmi) {
        wmi_set_nodeage((wmi_t *)m_pWMI, nodeage);
        m_Config.nodeAge = nodeage;
    }

    return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::GetCAR6kWMINodeAge(A_UINT32* nodeage)
{
    if (!m_WMIReady)
    {
        return NDIS_STATUS_ADAPTER_NOT_READY;
    }

    if (nodeage) {
        *nodeage = m_Config.nodeAge;
    }

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAR6kWMIKeepAliveInt(A_UINT8 ucInterval)
{
    NDIS_STATUS status = NDIS_STATUS_SUCCESS;
    A_STATUS    wmiStatus  = A_OK;
    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: Keep Alive Interval  %d \r\n",ucInterval);
    do
    {
        if(!m_WMIReady)
        {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - WMI Event is not ready \n");
            status =  NDIS_STATUS_ADAPTER_NOT_READY;
            break;
        }
        wmiStatus = wmi_set_keepalive_cmd((wmi_t *)m_pWMI,ucInterval);

        if(wmiStatus != A_OK)
        {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - Set KeepAlive Interval Failed. Status %d \n",wmiStatus);
            status =  NDIS_STATUS_FAILURE;
        }
    }while(FALSE);
    return status;
}


NDIS_STATUS
CAR6KMini::GetCAR6kWMIKeepAliveInt(WMI_GET_KEEPALIVE_CMD *getKeepAlive)
{
    NDIS_STATUS status = NDIS_STATUS_SUCCESS;
    A_STATUS    wmiStatus  = A_OK;
    do
    {
        if(!m_WMIReady)
        {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - WMI Event is not ready \n");
            status = NDIS_STATUS_ADAPTER_NOT_READY;
            break;
        }
        NdisResetEvent(&m_keepAliveEvent);
        if(getKeepAlive)
        {
            getKeepAlive->keepaliveInterval = wmi_get_keepalive_cmd((wmi_t *)m_pWMI);
            getKeepAlive->configured = FALSE;
            wmiStatus = wmi_get_keepalive_configured((wmi_t *)m_pWMI);
            if(wmiStatus != A_OK)
            {
                NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - Get keep alive configure failed. Status %d \n",wmiStatus);
                status = NDIS_STATUS_FAILURE;
                break;
            }
            if( !NdisWaitEvent(&m_keepAliveEvent, AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_KEEPALIVEINTV) )
            {
                NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - No KEEP ALIVE event after %u ms\n", AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_KEEPALIVEINTV);
                status = NDIS_STATUS_FAILURE;
                break;
            }
            getKeepAlive->configured = m_bkeepaliveConfigured;
        }
        else
        {
            status = NDIS_STATUS_INVALID_DATA;
        }
    }while(FALSE);
    return status;
}

