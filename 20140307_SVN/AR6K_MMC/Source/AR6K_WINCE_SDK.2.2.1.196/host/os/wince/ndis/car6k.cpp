//------------------------------------------------------------------------------
// <copyright file="car6k.cpp" company="Atheros and Microsoft">
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
// AR6000 NDIS Miniport class.
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
#include "bmi_msg.h"
#include "targaddrs.h"
extern "C" {
#include "bmi.h"
#include "ar6000_api.h"
#include "ar6000_diag.h"
}
#include "common_drv.h"
#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include <pm.h>
#ifdef WINDOWS_MOBILE
#include <wrlspwr.h>
#include <winreg.h>
#endif
#include "a_debug.h"
#include "host_version.h"
#include "celog.h"
#include "wince_common_drv.h"
#include "stream_drv.h"

BYTE g_defaultSupportedRated[] = {
        0x82,    //1 Mbps
        0x84,    //2 Mbps
        0x8b,    //5.5
        0x96,    //11
        0x24,    //18
        0x30,    //24
        0x48,    //36
        0x6c,    //54
    };

CAR6KMini::CAR6KMini()
{
    int i = 0;

    m_pHTCTarget   = NULL;
    m_InitComplete = FALSE;
    m_TimerWAREnable = TRUE;

    memset(m_PermanentAddress, 0, sizeof(m_PermanentAddress));
    memset(m_CurrentAddress, 0, sizeof(m_CurrentAddress));
    m_InfrastructureMode = Ndis802_11Infrastructure;
    m_AuthenticationMode = Ndis802_11AuthModeOpen;
    m_EncryptionStatus   = Ndis802_11EncryptionDisabled;
    m_RTSThreshold = 0;

    m_RSSI         = 0;
    m_BitrateKpbs  = 54000;
    m_LinkrateKpbs = 54000;
    memset(m_ChannelList, 0, sizeof(m_ChannelList));
    m_cChannelList = 0;
    m_AssocReqLen = 0;
    m_AssocRespLen = 0;
    m_BeaconIeLen = 0;
    m_BeaconInterval = 0;
    m_pAssocInfo = NULL;
    m_Halting = FALSE;

    m_pBSSIDList = NULL;
    m_cbBSSIDList = 0;
    m_pcbBSSIDListNeeded = NULL;

    m_pTransmitBufferArray = NULL;
    m_cbTransmitBufferArray = 0;
    m_TxPending = 0;

    m_DirectRx2Ndis = FALSE;
    m_DevicePowerName = AR6K_DEVICE_POWER_NAME;
    m_DeviceInitEventName = AR6K_DEVICE_INIT_EVENT_NAME;
    m_DriverStateEvent = NULL;

    m_RxPacketPool = NULL;
    m_TxPacketPool = NULL;
    m_TxBufferPool = NULL;
    memset(&m_apCmd,0,sizeof(m_apCmd));
    memset(&m_apxtn,0,sizeof(m_apxtn));
    memset(&m_apCrypt,0,sizeof(m_apCrypt));
    m_apxtn.dtim           = DFT_AP_DTIM_PERIOD;
    m_apxtn.bgprotection   = TRUE ;
    m_apxtn.hidden_ssid    = FALSE ;
    m_apxtn.channel        = DFT_AP_CHANNEL;
    m_apxtn.beaconInterval = DFT_AP_BEACON_INTVL ;
    m_apCrypt.IsAscii = FALSE;
    m_apCrypt.cryptType = NONE_CRYPT;
    m_apCrypt.dot11_authmode = OPEN_AUTH;
    memset(m_keyCounter,0,32);
    replayCtr = 0;
    m_ap_num_sta = AP_MAX_NUM_STA;
    m_key_size = 0;
    m_crypt_type = 0;
    m_isSecure = FALSE ;
    m_current_grp_index = GROUPKEY_IDX_A;
    m_AP_conn_sta = 0;
    m_Grp_rekey_time = 0;
    m_grp_rekey_timer_enable = FALSE;
    m_ap_CounterMeasureOn = FALSE;
    m_ap_mic_failures = 0;
    m_ap_intrabss = TRUE;
    m_apxtn.IntraBss = TRUE;

    for (i=0;i<4;i++) {
        memset(m_Key[i], 0, sizeof(m_Key[i]));
        m_cbKey[i] = 0;
        m_KeyIndex[i] = 0;
        m_KeyRSC[i] = 0;
    }

    m_NumDataEndpts = 0;
    m_RegCode = 0;
    m_TxPwr = 0;
    m_PowerState = NdisDeviceStateD0;

    //Default PowerMode set to Fast_PSP <--> REC_POWER
    m_80211_PowerMode = Ndis802_11PowerModeFast_PSP;
    // Default adhoc power save is disabled
    m_IBSS_PSEnable = 0;
    //Default listen interval set to 10 beacon Intervals (100 TUs)
    m_ListenInterval = NDIS_FASTPSP_LISTEN_INTERVAL;
    m_NetworkTypeInUse = Ndis802_11OFDM24;
#ifdef CONFIG_HOST_TCMD_SUPPORT
    m_TcmdPm = TCMD_PM_WAKEUP;
    m_Tcmd_totalPkt = 0;
    m_Tcmd_rssiInDBm = 0;
    m_Tcmd_crcError = 0;
    m_Tcmd_secErr = 0;
#endif

    m_RadioDisabled = FALSE;
    m_beaconInterval = 0;

#ifdef SUPPORT_WPA2
    m_pPMKID = NULL;
#endif

    m_pBusDriverHandle = NULL;
    m_PowerChangeEvent = NULL;
    m_ResumeWlanState = 1; // ON state
    m_LastSysState = POWER_STATE_ON;

    memset(&m_tgtStats, 0, sizeof(NDIS_802_11_STATISTICS));
    m_tgtStats.Length = sizeof(NDIS_802_11_STATISTICS);

    memset(&m_tgtAllStats,0,sizeof(AR6K_TARGET_STATS));
    m_tgtAllStats.length = sizeof(AR6K_TARGET_STATS);

    // AP_NETWORK
    for (i=0;i<AP_MAX_NUM_STA;i++) {
        memset(&m_staList[i],0,sizeof(STATION));
    }
    m_staListIndex = 0;

    memset(&m_acl,0,sizeof(WMI_AP_ACL));
    m_acl.policy = AP_ACL_DISABLE;
    m_mcastq_cnt = 0;
    m_ucastq_cnt = 0;
    m_hasMoreData = FALSE;



    m_networkType = INFRA_NETWORK;
    m_bcastPacket = 0;
    m_WowMode = 0; // Wow not enabled

    m_WowListReply = NULL;

    Init();

    m_pSupportedRates = (PBYTE) malloc (sizeof(NDIS_802_11_RATES));

    if (m_pSupportedRates)
    {
        memset (m_pSupportedRates, 0, sizeof(NDIS_802_11_RATES));
        //
        // Add AR6K Supported Rates
        //
        for (i = 0; i < sizeof (NDIS_802_11_RATES); i++)
        {
            m_pSupportedRates[i] = g_defaultSupportedRated[i];
        }
    }

    m_bIsWiFiOnOff           = FALSE;
    m_bIsSuspend             = FALSE;
    m_ChannelHint            = 0;
    m_powerMode              = REC_POWER;
    m_PairwiseCryptoType     = NONE_CRYPT;
    m_GrpwiseCryptoType      = NONE_CRYPT;
    m_AuthAlg                = car6k802_11AuthAlgOpen;
    m_CCKMAuthenticationMode = car6k802_11AuthModeDisableCCKM;
    m_WPSEnable              = 0;
    m_SsidOidCompleted       = TRUE;
    m_bIsSwitchAPtoSTA       = FALSE;
    m_rssiEvent              = 0;

    A_INIT_TIMER(&m_CounterMeasureEnterTimer, ar6000_cmEnterTimeout, this);
    A_INIT_TIMER(&m_CounterMeasureExitTimer, ar6000_cmExitTimeout, this);
    A_INIT_TIMER(&m_rssiScanTimer, ar6000_cmRssiScanTimeout, this);

    A_INIT_TIMER(&m_Grp_rekeying_timer, ar6000_rekeyingTimeout, this);


    m_RxIndicationPendingCount = 0;
}

CAR6KMini::~CAR6KMini()
{
}

void
CAR6KMini::Init()
{
    m_pWMI         = NULL;
    m_WMIReady     = FALSE;
    m_HTCStarted   = FALSE;

    m_Connected    = FALSE;
    m_ConnectInProgress = FALSE;
    m_ConnectedChannel  = 0;
    memset(m_PeerBSSID, 0, sizeof(m_PeerBSSID));

    m_WantToBeConnected = FALSE;
    m_SSID.SsidLength = 0;
    m_WMIBssFilter  = NONE_BSS_FILTER;
    memset(m_SSID.Ssid, 0, sizeof(m_SSID.Ssid));

    m_MicErrorCount = 0;
    m_RSSIlevel = 0;
    m_roamTblEntryCount = 0;
    m_CounterMeasureOn = FALSE;
    m_txRate = 0;
    m_rxRate = 0;
}

A_STATUS
ar6000_set_host_app_area(A_UINT32 tgtType, HIF_DEVICE *device)
{
    A_UINT32 address, data;

    struct host_app_area_s host_app_area;

    /* Fetch the address of the host_app_area_s instance in the host interest area */
    address = HOST_INTEREST_ITEM_ADDRESS(tgtType, hi_app_host_interest);

    if (ar6000_ReadRegDiag(device, &address, &data) != A_OK)
    {
        return A_ERROR;
    }

    address = data;

    host_app_area.wmi_protocol_ver = WMI_PROTOCOL_VERSION;

    if (ar6000_WriteDataDiag (device, address, (A_UCHAR *)&host_app_area, sizeof(struct host_app_area_s)) != A_OK)
    {
        return A_ERROR;
    }

    return A_OK;
}

// function to recycle processed control packets that were received via HTC
void CAR6KMini::ReturnControlBufferCallBack(void *pContext, ndis_mini_buf_t *pBuf)
{
    CAR6KMini *pAdapter = (CAR6KMini *)pContext;

    a_netbuf_reset(pBuf);
    HTC_PACKET_RESET_RX(&pBuf->HtcPacket);
        /* return the control packet buffer back to HTC */
    HTCAddReceivePkt(pAdapter->m_pHTCTarget, &pBuf->HtcPacket);

}

void
CAR6KMini::ReceiveWMIControlPacket(HTC_PACKET *pPacket, A_BOOL DataPath)
//
//  This function processes data from an HTC_BUFFER_RECEIVED indication
//  on the WMI_CONTROL_MBOX endpoint.
//
{
    ndis_mini_buf_t *pb;

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC,"AR6K: +ReceiveWMIControlPacket");

    pb = (ndis_mini_buf_t *)(pPacket->pPktContext);
    ASSERT(pb);

    if (pPacket->Status == A_OK) {
        // Append something to tail of buffer?
        a_netbuf_put(pb, pPacket->ActualLength + HTC_HEADER_LEN);

        // Remove HTC_HEADER_LEN bytes from the start of buffer
        a_netbuf_pull(pb, HTC_HEADER_LEN);

        if (DataPath) {
                /* remove the data header first */
            wmi_data_hdr_remove((wmi_t *)m_pWMI, pb);
        }

            // wmi_control_rx() frees the net buffer now.  To avoid this we add a return buffer handler
            // so that the packet comes back to us so we can recycle it back to HTC

        pb->ReturnBufferHandler = CAR6KMini::ReturnControlBufferCallBack;
        pb->pContext = this;

        wmi_control_rx((wmi_t *)m_pWMI, pb, WMI_11G_MODE);

    } else {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - ReceiveWMIPacket Error in receiving : status = %x\n", pPacket->Status);
        HTC_PACKET_RESET_RX(pPacket);
        HTCAddReceivePkt(m_pHTCTarget, pPacket);

    }

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -ReceiveWMIControlPacket");
}

static void
AR6KHTCBufferReceivedEventHandler(void            *Context,
                                  HTC_PACKET      *pPacket)
//
//  This function is called by the HTC layer to indicate that a buffer has been received,
//  that is an HTC_BUFFER_RECEIVED event has occurred.
//
//  evInfo contains information about the particular buffer that has been received.
//  In particular, evInfo->cookie contains the "cookie" parameter that was passed
//  to HTCBufferReceive.
//
//  The received buffer may contain either network traffic, or control information.
//
//
{
    CAR6KMini *pAdapter = (CAR6KMini *)Context;
    ndis_mini_buf_t *pb = (ndis_mini_buf_t *)(pPacket->pPktContext);

    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: AR6KHTCBufferReceivedEventHandler ep=%u", pPacket->Endpoint);


    if (pPacket->Status == A_ECANCELED) {

        NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: AR6KHTCBufferReceivedEventHandler freeing buffer: pb = %x \n",(unsigned int)pb);
        a_netbuf_free(pb);
        return;
    }

    // Demux control packets from data traffic
    if (pPacket->Endpoint == pAdapter->m_ControlEp)
    {
        // WMI Control packet (e.g. WMI_READY_EVENT)
        pAdapter->ReceiveWMIControlPacket(pPacket, FALSE);
    }
    else
    {
        // WMI Data packet (e.g. IP data packet)
        pAdapter->ReceiveWMIDataPacket(pPacket);

    }
}

static void
AR6KHTCDataAvailableEventHandler(
    void            *Context,
    HTC_ENDPOINT_ID Endpoint)
//
//  This function is called by the HTC layer to indicate that an HTC_DATA_AVAILABLE
//  event has occurred. HTC generates this event indication when it has data available
//  to be received, but no buffers available to receive the data into.
//
//  This function will allocate buffers and pass them to HTC for use in receiving
//  the packet data. HTC will shortly thereafter send an HTC_BUFFER_RECEIVED
//  indication containing the packet data in these buffers.
//
{
    CAR6KMini *pAdapter = (CAR6KMini *)Context;

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +AR6KHTCDataAvailableEventHandler");

    // Provide HTC with some more receive buffers
    // TODO - Impose some maximum cap
    pAdapter->AllocateHTCPacketBuffers(Endpoint, AR6000_MAX_RX_BUFFERS);

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -AR6KHTCDataAvailableEventHandler");
}

static HTC_SEND_FULL_ACTION AR6KHTCTXQueueFullEventHandler(void *Context, HTC_PACKET *pPacket)
{
    CAR6KMini *pAdapter = (CAR6KMini *)Context;

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +AR6KHTCTXQueueFullEventHandler");

    if (HTC_GET_ENDPOINT_FROM_PKT(pPacket) == pAdapter->GetControlEp()) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: Control Endpoint Getting FULL!!! ");
    }

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -AR6KHTCTXQueueFullEventHandler");
    /* don't drop these */
    return HTC_SEND_FULL_KEEP;
}

void
CAR6KMini::HTCControlSendCompleted(
    PVOID cookie)
//
//  This function is called when an HTCBufferSend request completes on
//  the WMI_CONTROL_MBOX. It frees the buffer (that was allocated by WMI)
//  now that it is no longer needed.
//
{
    a_netbuf_free(cookie);
}

void
CAR6KMini::HTCBufferSentEvent(
    PVOID cookie)
//
//  This function is called when HTC is indicating that an HTCBufferSend request
//  has completed and we can free/reuse the buffer we passed in to the call.
//
{
    if ((PVOID)m_pTransmitBufferArray <= cookie
    &&  cookie <= (PVOID)((PBYTE)m_pTransmitBufferArray + m_cbTransmitBufferArray))
    {
        // This was a transmit packet buffer that we sent
        HTCDataSendCompleted(cookie);
    }
    else
    {
        // This was a WMI control message. Usually they are WMI_CONTROL_MBOX,
        // but WMI_SYNC messages may also be sent on data MBOXes
        HTCControlSendCompleted(cookie);
    }
}

static void
AR6KHTCBufferSentEventHandler(void              *Context,
                              HTC_PACKET        *pPacket)
//
//  This function is called by the HTC layer to indicate that a buffer has been sent,
//  that is an HTC_BUFFER_SENT event has occurred.
//
//  evInfo contains information about the particular buffer that has been sent.
//  In particular, evInfo->cookie contains the "cookie" context that we passed in
//  to our call to HTCBufferSend.
//
{
    CAR6KMini *pAdapter = (CAR6KMini *)Context;
    PVOID      cookie = pPacket->pPktContext;

    pAdapter->HTCBufferSentEvent(cookie);
}

A_STATUS CAR6KMini::ConnectService(HTC_SERVICE_CONNECT_REQ  *pConnect,
                                   char                     *pDesc)
{
    A_STATUS                 status;
    HTC_SERVICE_CONNECT_RESP response;

    do {

        A_MEMZERO(&response,sizeof(response));

        status = HTCConnectService(m_pHTCTarget,
                                   pConnect,
                                   &response);

        if (A_FAILED(status)) {
            NDIS_DEBUG_PRINTF(ATH_LOG_TRC," Failed to connect to %s service status:%d \n", pDesc, status);
            break;
        }

        switch (pConnect->ServiceID) {
            case WMI_CONTROL_SVC :
                wmi_set_control_ep((wmi_t *)m_pWMI, response.Endpoint);
                m_ControlEp = response.Endpoint;
                break;
            case WMI_DATA_BE_SVC :
                arSetAc2EndpointIDMap(WMM_AC_BE, response.Endpoint);
                break;
            case WMI_DATA_BK_SVC :
                arSetAc2EndpointIDMap(WMM_AC_BK, response.Endpoint);
                break;
            case WMI_DATA_VI_SVC :
                arSetAc2EndpointIDMap(WMM_AC_VI, response.Endpoint);
                 break;
           case WMI_DATA_VO_SVC :
                arSetAc2EndpointIDMap(WMM_AC_VO, response.Endpoint);
                break;
           default:
                NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"ServiceID not mapped %d\n", pConnect->ServiceID);
                status = A_EINVAL;
                break;
        }

    } while (FALSE);

    return status;
}

A_STATUS
CAR6KMini::StartEndpoints(A_BOOL isColdBoot)
//
//  This function initializes the endpoints used for communication
//  with the AR6000 SDIO card.
//
//  ENDPOINT1 is used for command/status packets.
//  ENDPOINT2 is used for data traffic (e.g. TCP/IP) packets.
//
{
    A_STATUS        status = A_ERROR;
    wchar_t         fileName[128];
    A_BOOL          bCompressed;

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +StartEndpoints");

    bCompressed = FALSE;

    if (m_TargetType == TARGET_TYPE_AR6002)
    {
        if (m_Config.tcmd)
        {
            if (m_TargetId == AR6002_VERSION_REV2)
            {
                wcscpy(fileName, TCMD_BINARY_REV2_STR);
            }
        }
        else
        {
            if (m_TargetId == AR6002_VERSION_REV2)
            {
                wcscpy(fileName, WLAN_BINARY_REV2_STR);
                bCompressed = TRUE;
            }
        }
    }

    status = configure_ar6000(m_pHIFDevice, m_TargetType, m_TargetId, m_Config.enableUARTprint, m_TimerWAREnable, m_Config.clkFreq, fileName, m_Config.binRoot, bCompressed, isColdBoot, m_Config.eepromFile);
    if (A_OK != status)
    {
       NDIS_DEBUG_PRINTF (ATH_LOG_ERR, ("Configure ar6000  Failed\n"));
       goto done;
    }

    ar6000_set_htc_params (m_pHIFDevice, m_TargetType, 0, 0);

    // Send a "BMI_DONE" command to the target, to get it out of
    // BMI mode and into normal operation.
    status = BMIDone(m_pHIFDevice);

    if (A_OK != status)
    {
        goto done;
    }

    if (!m_Config.byPassWmi) {
        NdisResetEvent(&m_WMIReadyEvent);
        m_pWMI = wmi_init(this);
        m_Wmi = m_pWMI ; // public
        if (NULL == m_pWMI)
        {
            status = A_ERROR;
            goto done;
        }
    }

    do {
        HTC_SERVICE_CONNECT_REQ connect;
            /*
             * The target is now running, wait for HTC to come on-line
             */
        status = HTCWaitTarget(m_pHTCTarget);

        if (A_FAILED(status))
        {
            break;
        }


        A_MEMZERO(&connect,sizeof(connect));
            /* meta data is unused for now */
        connect.pMetaData = NULL;
        connect.MetaDataLength = 0;
            /* these fields are the same for all service endpoints */
        connect.EpCallbacks.pContext = this;
        connect.EpCallbacks.EpTxComplete = AR6KHTCBufferSentEventHandler;
        connect.EpCallbacks.EpRecv = AR6KHTCBufferReceivedEventHandler;
        connect.EpCallbacks.EpRecvRefill = AR6KHTCDataAvailableEventHandler;
            /* set queue full handler for control service, let the handler deal with this
             * situation */
        connect.EpCallbacks.EpSendFull = AR6KHTCTXQueueFullEventHandler;
        connect.MaxSendQueueDepth = 64;

            /* connect to control service */
        connect.ServiceID = WMI_CONTROL_SVC;
        status = ConnectService(&connect,
                                "WMI CONTROL");
        if (A_FAILED(status))
        {
            break;
        }


        connect.EpCallbacks.EpSendFull = NULL;
        /* no need for a send full handler for DATA since NDIS requires our driver to indicate packet send
         * completions separately.  This provides back flow into the NDIS network stack */


            /* for the data services, set anti-credit dribbling if requested */
        if (m_Config.reduceCreditDribbleFlags != 0) {
                /* set credit dribble flags */
            connect.ConnectionFlags = HTC_CONNECT_FLAGS_REDUCE_CREDIT_DRIBBLE |
                                        (A_UINT16)(m_Config.reduceCreditDribbleFlags - 1);
        }

            /* connect to best-effort service */
        connect.ServiceID = WMI_DATA_BE_SVC;

        status = ConnectService(&connect,
                                "WMI DATA BE");
        if (A_FAILED(status))
        {
            break;
        }

            /* connect to back-ground
             * map this to WMI LOW_PRI */
        connect.ServiceID = WMI_DATA_BK_SVC;
        status = ConnectService(&connect,
                                "WMI DATA BK");
        if (A_FAILED(status))
        {
            break;
        }

            /* connect to Video service, map this to
             * to HI PRI */
        connect.ServiceID = WMI_DATA_VI_SVC;
        status = ConnectService(&connect,
                                "WMI DATA VI");
        if (A_FAILED(status))
        {
            break;
        }


            /* connect to VO service, this is currently not
             * mapped to a WMI priority stream due to historical reasons.
             * WMI originally defined 3 priorities over 3 mailboxes
             * We can change this when WMI is reworked so that priorities are not
             * dependent on mailboxes */
        connect.ServiceID = WMI_DATA_VO_SVC;
        status = ConnectService(&connect,
                                "WMI DATA VO");
        if (A_FAILED(status))
        {
            break;
        }

        ASSERT(m_ControlEp != 0);
        ASSERT(arAc2EndpointID(WMM_AC_BE) != 0);
        ASSERT(arAc2EndpointID(WMM_AC_BK) != 0);
        ASSERT(arAc2EndpointID(WMM_AC_VI) != 0);
        ASSERT(arAc2EndpointID(WMM_AC_VO) != 0);

    } while (FALSE);

    if (A_OK != status)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: HTC Setup failed, error=%u\n", status);
        goto done;
    }

    // Provide HTC with some initial receive buffers
    AllocateHTCPacketBuffers(m_ControlEp, AR6000_MAX_RX_BUFFERS);
    AllocateHTCPacketBuffers(arAc2EndpointID(WMM_AC_BE), AR6000_MAX_RX_BUFFERS);
    AllocateHTCPacketBuffers(arAc2EndpointID(WMM_AC_BK), AR6000_MAX_RX_BUFFERS);
    AllocateHTCPacketBuffers(arAc2EndpointID(WMM_AC_VO), AR6000_MAX_RX_BUFFERS);

    ar6000_setup_credit_dist(m_pHTCTarget, &m_CreditStateInfo);

    status = HTCStart(m_pHTCTarget);
    if (A_OK != status)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: HTCStart failed, error=%u\n", status);
        goto done;
    }

    m_HTCStarted = TRUE;

    if (!m_Config.byPassWmi) {
        NdisWaitEvent(&m_WMIReadyEvent, AR6K_DEFAULT_MS_TO_WAIT_FOR_WMI_READY);
        if (!m_WMIReady)
        {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - No WMI_READY event after %u ms, failing initialization\n", AR6K_DEFAULT_MS_TO_WAIT_FOR_WMI_READY);
            status = A_ERROR;
            goto done;
        }

        if ((ar6000_set_host_app_area(m_TargetType, m_pHIFDevice)) != A_OK)
        {
            NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "Unable to set the host app area\n");
            status = A_ERROR;
            goto done;
        }
    }

    if (!m_Config.tcmd) {
        status = configTargetParams();
        if (A_OK != status)
        {
            goto done;
        }
    }

    if (m_Config.enableDbglog)
    {

        //
        // Open AR6KTGT_DBGLOG named event which is created by the host, In order to open the event,
        // the below call will check whether the event i already created, otherwise, it just open the event
        // and notifies host whenever host receives debug logs using SetEvent
        //
        wmi_config_debug_module_cmd ((wmi_t *)m_pWMI,
                         (A_UINT16)(m_Config.DbglogFlags & DBG_LOG_FLAGS_LOG_MASK),
                         0x00,
                         0x00,
                         0x00,
                         (DBGLOG_MODULE_LOG_ENABLE_MASK|DBGLOG_TIMESTAMP_RESOLUTION_MASK)/*Enable Dbg log events*/);

    }
    else
    {
         wmi_config_debug_module_cmd ((wmi_t *)m_pWMI,
                             0x00,
                             0x00,
                             0x00,
                             0x00,
                             0xFFFFFFFF /*Disable Dbg log events*/);
    }

    if (m_Config.enableDirectRx2Ndis)
    {
        m_DirectRx2Ndis = TRUE;
    }

    status = configTargetParams();

done:
    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -StartEndpoints status=%u", status);
    return status;
}

void
CAR6KMini::StopEndpoints(A_BOOL bSuspend)
//
//  Undoes the initialization done by StartEndpoints.
//
{
    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +StopEndpoints");

    /* getting dbglog during suspend increases suspend time */
    if (!bSuspend || (m_Config.DbglogFlags & DBG_LOG_FLAGS_COLLECT_ON_SUSPEND)) {
        GetDbgLogs();
    }

    if (m_HTCStarted)
    {
        HTCStop(m_pHTCTarget);
        m_HTCStarted = FALSE;
    }

    if (!m_Config.byPassWmi)
    {
        if (m_pWMI)
        {
            wmi_shutdown((wmi_t *)m_pWMI);

            m_WMIReady = FALSE;
            m_pWMI = NULL;
        }
    }

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -StopEndpoints");
}

void
ResetPowerWorkItemFunction(
    PNDIS_WORK_ITEM WorkItem,
    PVOID           Context)
{
    CAR6KMini *pAdapter = (CAR6KMini *)Context;

    pAdapter->ResetPowerWorkItem();
}

void
AR6KWorkItemFunction(
    PNDIS_WORK_ITEM WorkItem,
    PVOID           Context)
//
//  This function is called after NdisScheduleWorkItem is called in MiniportInitialize.
//
{
    CAR6KMini *pAdapter = (CAR6KMini *)Context;

    pAdapter->IndicateReceivePackets();
}

/* Set the internal state variables based on the registry parameters */
A_STATUS
CAR6KMini::configRegistryParameters()
{
    switch(m_Config.powerSaveMode) {
        case 0:
            m_80211_PowerMode = Ndis802_11PowerModeCAM;
            break;
        case 1:
            m_ListenInterval = NDIS_MAXPSP_LISTEN_INTERVAL;
            m_80211_PowerMode = Ndis802_11PowerModeMAX_PSP;
            break;
        case 2:
            m_ListenInterval = NDIS_FASTPSP_LISTEN_INTERVAL;
            m_80211_PowerMode = Ndis802_11PowerModeFast_PSP;
            break;
        default:
            return A_ERROR;
    }
    if (m_Config.ibssPSEnable) {
        m_IBSS_PSEnable = 1;
    } else {
        m_IBSS_PSEnable = 0;
    }
    if (NdisEqualMemory(m_Config.swMacAddr, NullMacAddr,
                ETHERNET_MAC_ADDRESS_LENGTH) == 0)
    {
       ar6000_set_softmac_addr(m_Config.swMacAddr);
    }

    return A_OK;
}

/* Configure the target from the registry settings only in wmi mode */
A_STATUS
CAR6KMini::configTargetParams()
{
    A_STATUS   wmiStatus = A_OK;
    A_UINT16 bkenable, fgenable;

    if (!m_Config.byPassWmi) {

        if (m_Config.txPower)
        {
            //
            // Configure AR6K Tx Power, in case of non-zero dbM value in
            // the registry
            //
            wmi_set_txPwr_cmd ((wmi_t*)m_pWMI, m_Config.txPower);
        }

        wmi_set_nodeage((wmi_t *)m_pWMI, m_Config.nodeAge);
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
                                        0,0,0,WMI_SHORTSCANRATIO_DEFAULT, DEFAULT_SCAN_CTRL_FLAGS, 0, 0);

        if (wmiStatus == A_OK) {
            if (m_Config.discTimeout) {
                wmiStatus = wmi_disctimeout_cmd((wmi_t *)m_pWMI, m_Config.discTimeout);
            }
        }

        /* restore the target Power Save settings */
        if ( wmiStatus == A_OK ) {

            switch( m_80211_PowerMode ) {

            case Ndis802_11PowerModeCAM:

                m_powerMode = MAX_PERF_POWER;
                break;
            case Ndis802_11PowerModeMAX_PSP:

                m_powerMode = REC_POWER;
                break;
            case Ndis802_11PowerModeFast_PSP:

                m_powerMode = REC_POWER;
                break;
            default:
                // this will never occur.
                break;
            }

            wmiStatus = wmi_powermode_cmd((struct wmi_t *)m_pWMI, m_powerMode);
        }

        if (m_Config.BtCoexAntConfig != 0) {
            WMI_SET_BT_PARAMS_CMD btParamsCmd;

            A_MEMZERO(&btParamsCmd,sizeof(btParamsCmd));
            btParamsCmd.paramType = BT_PARAM_ANTENNA_CONFIG;
            btParamsCmd.info.antType = (A_UINT8)m_Config.BtCoexAntConfig;
                /* set BT Coexistence Antenna configuration */
            wmiStatus = wmi_set_bt_params_cmd((wmi_t*)m_pWMI, &btParamsCmd);
        }
        if (m_Config.BtCoex11aDifAnt != 0) {
            WMI_SET_BT_PARAMS_CMD btParamsCmd;

            A_MEMZERO(&btParamsCmd,sizeof(btParamsCmd));
            btParamsCmd.paramType = BT_PARAM_11A_SEPARATE_ANT;
            wmiStatus = wmi_set_bt_params_cmd((wmi_t*)m_pWMI, &btParamsCmd);
        }
        if (m_Config.BtDevType != 0) {
            WMI_SET_BT_PARAMS_CMD btParamsCmd;

            A_MEMZERO(&btParamsCmd,sizeof(btParamsCmd));
            btParamsCmd.paramType = BT_PARAM_COLOCATED_BT_DEVICE;
            btParamsCmd.info.coLocatedBtDev = (A_UINT8)m_Config.BtDevType;

                /* Indicate Co-located BT device type  */
            wmiStatus = wmi_set_bt_params_cmd((wmi_t*)m_pWMI, &btParamsCmd);
        }

            /* Set WMM mode at the target depending on the Registry config */
        wmiStatus = wmi_set_wmm_cmd((wmi_t *)m_pWMI, (WMI_WMM_STATUS)m_Config.wmmConfig);
    }

    if ( wmiStatus == A_OK )
    {
        //
        // Start fgscan, since MSFT issues SetScan every 1min only, and this will be an issue when turnoff/turn on
        // WLAN manager and suspend resume, so whenever targer startes WLAN, host manually do one scan to resolve
        // the timing issues
        //
        wmiStatus = wmi_bssfilter_cmd ((struct wmi_t *)m_pWMI, ALL_BSS_FILTER, 0);

        if (wmiStatus == A_OK)
        {
            m_WMIBssFilter = ALL_BSS_FILTER;

            wmiStatus = wmi_startscan_cmd ((wmi_t *)m_pWMI,WMI_LONG_SCAN,FALSE,FALSE, 0, 0, 0, NULL);

            if (wmiStatus != A_OK)
            {
                NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K: ERROR - wmi_startscan_cmd failed = %u\n", wmiStatus);
            }
        }
    }

    return wmiStatus;
}

A_UINT32
CAR6KMini::getResetPowerState()
{
    A_UINT32      powerState;

    // Read the reset state configured thru the registry
    powerState = m_Config.resetPowerState;

    // If the resetPowerState is 2, use the previous power state before the reset
    if (m_Config.resetPowerState == 2) {
        powerState = m_Config.currentPowerState;
    }


    return powerState;
}

void
CAR6KMini::ResetPowerWorkItem()
{
    A_UINT32  powerState = 0;
#ifdef WINDOWS_MOBILE
    RDD * pDevice = NULL;
    RDD * pTD = NULL;
    HRESULT hr = S_OK;
    BOOL wirelessState;
#else
    CEDEVICE_POWER_STATE cePowerState;
    DWORD         status = 0;
#endif
    A_STATUS      athStatus=A_ERROR;

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC,"ResetPowerWorkItem :: Entry\n");

    NdisResetEvent(&m_ResetPowerWorkItemExitedEvent);

    if (!m_Config.byPassWmi) {
        // Dual-Purpose workitem
        // Indicate disconnect after the MiniportInit complete successfully
        // This is needed for 802.11 miniports
        while (m_Halting == FALSE) {
            NdisMSleep(1000);
            if ((m_InitComplete) && (m_WMIReady) && bIsNdisRegistered) {
                NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
                NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
                break;
            }
        }

        powerState = getResetPowerState();
#ifdef WINDOWS_MOBILE
        if (m_Halting == FALSE) {
            // Windows mobile notifies this event when the driver has completed initalization
            m_DriverStateEvent = CreateEvent(NULL, FALSE, FALSE, m_DeviceInitEventName);
            if (m_DriverStateEvent != NULL) {
                WaitForSingleObject(m_DriverStateEvent, INFINITE);
                CloseHandle(m_DriverStateEvent);
                m_DriverStateEvent = NULL;
            }


            // Wait till the WiFi entry is created
            while (m_Halting == FALSE) {
                hr = GetWirelessDevices(&pDevice, WIRELESS_GET_DESIRED);
                if (hr == S_OK) {
                    if (pDevice) {
                        pTD = pDevice;
                        if (GetWirelessState(&wirelessState) == S_OK) {
                            powerState = powerState & wirelessState;
                        }
                        // loop through the linked list of devices
                        while ((pTD) && (!powerState)) {
                            if ((pTD->DeviceType==POWER_MANAGED)  &&
                                (_tcscmp(pTD->pszDeviceName, m_DevicePowerName) == 0)) {
                                hr=ChangeRadioState(pTD, 0, POWER_POST_SAVE);
                                if (hr == S_OK) {
                                    athStatus = A_OK;
                                    // Update the registry state
                                    UpdateRadioStoredState(pTD,0);
                                }
                                break;
                            }
                            pTD = pTD->pNext;
                        }

                        // Free the list of devices retrieved with GetWirelessDevices()
                        FreeDeviceList(pDevice);
                        pDevice = NULL;
                        break;
                    }
                } else {
                    break;
                }
                NdisMSleep(1000);
            }

            if (pDevice) {
                FreeDeviceList(pDevice);
            }
        }
#else //WINDOWS_MOBILE
        if ((!powerState) && (m_Halting == FALSE)) {
            // Don't call the Power Manager APIs directly.
            // Doing so assumes the PM is sysgen'ed into every image.
            typedef DWORD DevicePowerNotifyProto(PVOID, CEDEVICE_POWER_STATE, DWORD);
            typedef DWORD GetDevicePowerProto   (PVOID, DWORD, PCEDEVICE_POWER_STATE);
            DevicePowerNotifyProto *pfnDevicePowerNotify = NULL;
            GetDevicePowerProto    *pfnGetDevicePower    = NULL;

            HMODULE hCoreDll = (HMODULE) LoadLibrary(TEXT("coredll.dll"));
            if (NULL != hCoreDll) {
                pfnDevicePowerNotify = (DevicePowerNotifyProto *)
                    GetProcAddress(hCoreDll, TEXT("DevicePowerNotify"));
                pfnGetDevicePower = (GetDevicePowerProto *)
                    GetProcAddress(hCoreDll, TEXT("GetDevicePower"));
            }

            if ((NULL == pfnDevicePowerNotify) || (NULL == pfnGetDevicePower))
            {
                athStatus = A_ERROR;
            } else {
                // Improper way to find whether NDIS has created the power
                // reletionship for the miniport using RegisterPowerRelationship
                // with the power manager
                while (m_Halting == FALSE) {
                    status= pfnGetDevicePower((PVOID)m_DevicePowerName, POWER_NAME, &cePowerState);
                    if (status != ERROR_FILE_NOT_FOUND) {
                        break;
                    }
                    NdisMSleep(1000);
                }

                if ((status == ERROR_SUCCESS) && (m_Halting == FALSE)) {
                    cePowerState = D4;
                    status = pfnDevicePowerNotify((PVOID)m_DevicePowerName,cePowerState, POWER_NAME);
                    if (status == ERROR_SUCCESS) {
                        athStatus = A_OK;
                    }
                }
            }

            if (NULL != hCoreDll) {
                FreeLibrary(hCoreDll);
            }
        }
#endif
            if (athStatus == A_ERROR) {
                NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"ResetPowerWorkItem :: Reset power state set failed\n");
            }  else {
                NDIS_DEBUG_PRINTF(ATH_LOG_INF,"ResetPowerWorkItem :: Reset power state set success\n");
            }
    }

    NdisSetEvent(&m_ResetPowerWorkItemExitedEvent);

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC,"ResetPowerWorkItem :: Exit\n");

    return;
}

NDIS_STATUS
CAR6KMini::TargetInitialize(void)
{
    BOOL            bIsVersionMismatch = FALSE;
    A_UINT32        param = 0;
    NDIS_STATUS Status;
    A_UINT8  byFirmwareMajor;
    A_UINT8  byFirmwareMinor;

    m_pHTCTarget = cmnGetHTCHandle();

    if (NULL == m_pHTCTarget)
    {
       Status = NDIS_STATUS_FAILURE;
       goto done;
    }

    m_pHIFDevice = (HIF_DEVICE *)HTCGetHifDevice(m_pHTCTarget);
    cmnGetTargetInfo(&m_TargetType, &m_TargetId);

    if (0 == m_TargetType || 0 == m_TargetId)
    {
        Status = NDIS_STATUS_FAILURE;
        goto done;
    }

    Status = NDIS_STATUS_SUCCESS;

    // the following code wil display the vrsion of firmware and driver
    // also it checks whether version mismatch between drier and firmware
    // incase of version mismatch, this will FALSE the flag and retun status failure
    // notification to NDIS

    byFirmwareMajor = (m_TargetId >> VER_MAJOR_BIT_OFFSET) & 0xF;
    byFirmwareMinor = (m_TargetId >> VER_MINOR_BIT_OFFSET) & 0xF;

    ATHR_DISPLAY_MSG (L"TARGET ID VERSION ==> %x\n", m_TargetId);
    logPrintf (ATH_DEBUG_INF, "TARGET ID VERSION ==> %x\n", m_TargetId);
    logPrintf (ATH_DEBUG_INF, "FIRMWARE MAJOR VERSION ==> %x\n", byFirmwareMajor);
    logPrintf (ATH_DEBUG_INF, "FIRMWARE MINOR VERSION ==> %x\n", byFirmwareMinor);
    logPrintf (ATH_DEBUG_INF, "DRIVER MAJOR VERSION %x\n", ATH_SW_VER_MAJOR);
    logPrintf (ATH_DEBUG_INF, "DRIVER MINOR VERSION %x\n", ATH_SW_VER_MINOR);
    logPrintf (ATH_DEBUG_INF, "DRIVER PATCH VERSION %x\n", ATH_SW_VER_PATCH);
    logPrintf (ATH_DEBUG_INF, "DRIVER BUILD_NO %x\n", ATH_SW_VER_BUILD);

    if ((byFirmwareMajor == ATH_SW_VER_MAJOR) &&
        (byFirmwareMinor == ATH_SW_VER_MINOR))
    {
        // Both versions are same
        bIsVersionMismatch = FALSE;
    }

    if (A_OK != StartEndpoints(TRUE))
    {
        Status = NDIS_STATUS_FAILURE;
        goto done;
    }

done:
    return Status;
}

#ifdef OS_ROAM_MANAGEMENT
static void disconnect_indication_timeout(A_ATH_TIMER Context)
{
    CAR6KMini *Adapter = (CAR6KMini *)Context;
    Adapter->disconnectIndicationTimeout();
}
#endif

#ifdef AR6K_NO_DOUBLELOAD
BOOL
removeRegKeyValues()
{
    HKEY handle;
    LONG result;
    A_STATUS status = A_ERROR;

    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Comm\\AR6K_SD1\\Parms"),0,0,&handle);
    if (result == ERROR_SUCCESS)
    {
        result = RegDeleteValue(handle,_T("BusNumber"));
        if (result == ERROR_SUCCESS)
        {
            result = RegDeleteValue(handle,_T("BusType"));
            if (result == ERROR_SUCCESS)
                status = A_OK;
        }
        RegCloseKey(handle);
    }

    if (A_OK != status)
        return FALSE;
    else
        return TRUE;
}
#endif

NDIS_STATUS
CAR6KMini::Initialize(
    IN NDIS_HANDLE MiniportAdapterHandle,
    IN NDIS_HANDLE ConfigHandle)
//
//  This function is called by NDIS when a new adapter
//  instance is being instantiated.
//
//  Read configuration settings from the registry and
//  initialize the AR6000 adapter.
//
{
    NDIS_STATUS   Status    = NDIS_STATUS_SUCCESS;
    A_STATUS      athStatus = A_OK;

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +Initialize");

    NdisInitializeEvent(&m_WMIReadyEvent);
    NdisInitializeEvent(&m_RxPendingEvent);
    NdisInitializeEvent(&m_RxWorkItemExitedEvent);
    NdisInitializeEvent(&m_tgtStatsEvent);
    NdisInitializeEvent(&m_ResetPowerWorkItemExitedEvent);
    NdisInitializeEvent(&m_SuspendEvent);
    NdisInitializeEvent(&m_tgtTxPwrEvent);
    NdisInitializeEvent(&m_ConnectEvent);
    NdisInitializeEvent(&m_keepAliveEvent);


    NdisInitializeEvent(&m_tgtWowListReplyEvent);
#ifdef CONFIG_HOST_TCMD_SUPPORT
    NdisInitializeEvent(&m_TcmdReportEvent);
#endif

#ifdef OS_ROAM_MANAGEMENT
    NdisInitializeEvent (&m_tgtChannelListEvent);
    m_osRoamControl         = FALSE;
    m_ChannelScan           = FALSE;
    m_bRoamScanIndication   = FALSE;
    m_psi                   = NULL;
    m_scanCount             = 0;
    A_INIT_TIMER(&m_disconnectIndicationTimer, disconnect_indication_timeout, this);

    A_MEMZERO(&m_RateInfo, sizeof(m_RateInfo));
#endif

    m_bkeepaliveConfigured = FALSE;
    NdisSetEvent(&m_RxWorkItemExitedEvent);
    NdisSetEvent(&m_ResetPowerWorkItemExitedEvent);

    m_MaxTransmitBuffers = DEFAULT_MAX_TRANSMIT_BUFFERS;
    m_MaxTransmitBufferPayloadDataLength = DEFAULT_MAX_TRANSMIT_BUFFER_PAYLOAD_DATA_LENGTH;
    InitializeListHead(&m_TransmitBufferList);
    InitializeListHead(&m_TransmitNdisPacketList);
    InitializeListHead(&m_RxPendingPacketList);

    memset(m_DesiredAPBSSID, 0xFF, sizeof(m_DesiredAPBSSID));

    m_PowerChangeEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("AR6K_PowerChange"));
    if (m_PowerChangeEvent == NULL)
    {
        Status    = NDIS_STATUS_RESOURCES;
        goto done;
    }

    // Perform base class initialization first
    Status = CMiniport::Initialize(MiniportAdapterHandle, ConfigHandle);
    if (NDIS_STATUS_SUCCESS != Status)
    {
        goto done;
    }

    m_ConfigHandle = ConfigHandle;
    Status = ParseRegistryParameters (ConfigHandle, &m_Config);
    if (NDIS_STATUS_SUCCESS != Status)
    {
        goto done;
    }

    if (configRegistryParameters() != A_OK)
    {
        Status = NDIS_STATUS_FAILURE;
        goto done;
    }

    dbgIntialize (m_Config.DbgPrintMask);
    //
    // If the SDIO component is uninitialized, then we should not be
    // getting called. If we do, for some reason, then return an error.
    //

    Status = TransmitBufferListAllocate();
    if (NDIS_STATUS_SUCCESS != Status)
    {
        goto done;
    }

    NdisAllocatePacketPool(&Status, &m_RxPacketPool, AR6000_MAX_RX_BUFFERS, 16);
    if (NDIS_STATUS_SUCCESS != Status)
    {
        goto done;
    }

    NdisAllocatePacketPool(&Status, &m_TxPacketPool, 32, 16);
    if (NDIS_STATUS_SUCCESS != Status)
    {
        goto done;
    }

    NdisAllocateBufferPool(&Status, &m_TxBufferPool, 32);
    if (NDIS_STATUS_SUCCESS != Status)
    {
        goto done;
    }

    // Create a work item to peform receive packet indications
    NdisInitializeWorkItem(&m_WorkItem, AR6KWorkItemFunction, this);
    Status = NdisScheduleWorkItem(&m_WorkItem);
    if (NDIS_STATUS_SUCCESS != Status)
    {
        goto done;
    }

    // Create a work item to power off wlan upon reset
    NdisInitializeWorkItem(&m_ResetPowerWorkItem, ResetPowerWorkItemFunction, this);
    Status = NdisScheduleWorkItem(&m_ResetPowerWorkItem);
    if (NDIS_STATUS_SUCCESS != Status)
    {
        goto done;
    }

    Status = TargetInitialize ();
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NDIS_DEBUG_PRINTF (ATH_LOG_ERR, "AR6K: ERROR - Adapter Initialize failed 0x%x\n", Status);
        goto done;
    }

    m_InitComplete = TRUE;

done:
    if (NDIS_STATUS_SUCCESS != Status)
    {
        // If we fail Initialize, then our Halt won't be called by NDIS,
        // so clean up all resources allocated by Initialize.
        Halt();
    }
    #ifdef AR6K_NO_DOUBLELOAD
    removeRegKeyValues();
    #endif

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -Initialize status=%x", Status);

    return Status;
}

void
CAR6KMini::Halt()
//
// This function undoes Initialize.
// It stops the adapter and frees resources
//
{
    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +Halt");

    m_Halting = TRUE;
    if (m_Connected) {
        NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
        NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
        m_Connected = FALSE;
    }

    if (m_pHTCTarget)
    {
        StopEndpoints (FALSE);

        ar6000_reset_device (m_pHIFDevice, m_TargetType, TRUE);

        m_pHIFDevice = NULL;
        m_pHTCTarget = NULL;
    }

    if (m_ConfigHandle)
    {
        NdisCloseConfiguration (m_ConfigHandle);
        m_ConfigHandle = NULL;
    }


    // Wakeup the Rx Work Item thread so it can exit
    NdisSetEvent(&m_RxPendingEvent);
    NdisWaitEvent(&m_RxWorkItemExitedEvent, 0);

    // Wakeup the Get Stats thread if any
    NdisSetEvent(&m_tgtStatsEvent);
    NdisSetEvent(&m_tgtTxPwrEvent);


    // Wakeup the SetPnpPower() OID thread that may be waiting for a SuspendEvent.
    NdisSetEvent(&m_SuspendEvent);
    NdisSetEvent(&m_tgtWowListReplyEvent);

    NdisSetEvent(&m_keepAliveEvent);

#ifdef OS_ROAM_MANAGEMENT
    // Wakeup the thread that may be waiting for the channel list OID.
    NdisSetEvent(&m_tgtChannelListEvent);
#endif

    NdisSetEvent(&m_ConnectEvent);
    if (m_DriverStateEvent != NULL) {
            /* make sure this event is set in order to unblock the reset power work item
             * which may be stuck because we are halting/unloading */
        SetEvent(m_DriverStateEvent);
    }

    NdisWaitEvent(&m_ResetPowerWorkItemExitedEvent, 0);

    NdisFreeEvent(&m_WMIReadyEvent);
    NdisFreeEvent(&m_RxPendingEvent);
    NdisFreeEvent(&m_RxWorkItemExitedEvent);
    NdisFreeEvent(&m_tgtStatsEvent);
    NdisFreeEvent(&m_tgtTxPwrEvent);
    NdisFreeEvent(&m_keepAliveEvent);

    NdisFreeEvent(&m_ResetPowerWorkItemExitedEvent);
    NdisFreeEvent(&m_SuspendEvent);
    NdisFreeEvent(&m_tgtWowListReplyEvent);
    NdisFreeEvent(&m_ConnectEvent);

#ifdef CONFIG_HOST_TCMD_SUPPORT
    NdisFreeEvent(&m_TcmdReportEvent);
#endif

#ifdef OS_ROAM_MANAGEMENT
    NdisFreeEvent(&m_tgtChannelListEvent);
#endif

    A_UNTIMEOUT(&m_CounterMeasureEnterTimer);
    A_UNTIMEOUT(&m_CounterMeasureExitTimer);
    A_UNTIMEOUT(&m_rssiScanTimer);
    A_UNTIMEOUT(&m_Grp_rekeying_timer);
    A_DELETE_TIMER(&m_CounterMeasureEnterTimer);
    A_DELETE_TIMER(&m_CounterMeasureExitTimer);
    A_DELETE_TIMER(&m_rssiScanTimer);
    A_DELETE_TIMER(&m_Grp_rekeying_timer);

#ifdef SUPPORT_WPA2
    if( m_pPMKID ) {
        A_FREE(m_pPMKID);
        m_pPMKID = NULL;
    }
#endif //SUPPORT_WPA2



    if (m_pAssocInfo != NULL) {
        A_FREE(m_pAssocInfo);
        m_pAssocInfo = NULL;
    }

    if (m_PowerChangeEvent) {
        CloseHandle(m_PowerChangeEvent);
        m_PowerChangeEvent = NULL;
    }

    CMiniport::Halt();

    if (m_pTransmitBufferArray)
        A_FREE(m_pTransmitBufferArray);

    if (m_RxPacketPool)
        NdisFreePacketPool(m_RxPacketPool);

    if (m_pSupportedRates)
    {
        free (m_pSupportedRates);
        m_pSupportedRates = NULL;
    }

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -Halt");
}

BOOLEAN
CAR6KMini::CheckForHang()
//
//  This function is called periodically (default every 2 seconds) by NDIS
//  to check whether the adapter is in a hung state in which it is no longer
//  sending/receiving. If this function returns TRUE, then NDIS will reset
//  the adapter.
//
{
    return FALSE;
}

NDIS_STATUS
CAR6KMini::Reset(
    OUT PBOOLEAN pAddressingReset)
//
//  This function is called by NDIS to reset the adapter.
//  If addressing information needs to be reset (e.g. the multicast list),
//  then *pAddressingReset should be set to TRUE.
//
{
    return NDIS_STATUS_SUCCESS;
}
