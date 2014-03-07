//------------------------------------------------------------------------------
// <copyright file="car6k.cpp" company="Atheros and Microsoft">
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
// AR6000 NDIS Miniport class.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================

#include <windows.h>
#include <ndis.h>

#include "htc_internal.h"
#include "htc.h"
#include "htc_wince.h"
#include "hif.h"
#include "wmi_api.h"
#include "netbuf.h"

extern "C" {
#include "bmi.h"
#include "ndisnetbuf.h"
//#include "ar6k_drv.h"

}

#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include <pm.h>
#ifdef WINDOWS_MOBILE
#include "wrlspwr.h"
#include <winreg.h>
#endif
#include "ieee80211_node.h"

#include "wmi_host.h"
#include "targaddrs.h"
#include "ar6000_api.h"
#include "wince_common_drv.h"
#include "host_version.h"


#define _P	L"\r[CAR6K.cpp] "L


enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0
}Car6kDebug;


#define HOST_INTEREST_ITEM_ADDRESS(type, item) \
        ((type == TARGET_TYPE_AR6001) ? AR6001_HOST_INTEREST_ITEM_ADDRESS(item) : \
        ((type == TARGET_TYPE_AR6002) ? AR6002_HOST_INTEREST_ITEM_ADDRESS(item) : \
        ((type == TARGET_TYPE_AR6003) ? AR6003_HOST_INTEREST_ITEM_ADDRESS(item) : 0)))

static int reduce_credit_dribble = 1 + HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_ONE_HALF;


static OSDRV_CALLBACKS s_callbacks;


CAR6KMini::CAR6KMini()
{
	int i = 0;

//	m_pHTCTarget   = NULL;	
	m_hHtcTarget	= NULL;
	m_tgtReady	   = false;
	m_HTCInited    = false;
	m_InitComplete = false;

	memset(m_PermanentAddress, 0, sizeof(m_PermanentAddress));
	memset(m_CurrentAddress, 0, sizeof(m_CurrentAddress));
	m_InfrastructureMode = Ndis802_11Infrastructure;
	m_AuthenticationMode = Ndis802_11AuthModeOpen;
	m_EncryptionStatus   = Ndis802_11EncryptionDisabled;

	m_RSSI         = 0;
	m_BitrateKpbs  = 54000;
	memset(m_ChannelList, 0, sizeof(m_ChannelList));
	m_cChannelList = 0;
	m_AssocReqLen = 0;
	m_AssocRespLen = 0;
	m_BeaconIeLen = 0;
	m_pAssocInfo = NULL;
	m_Halting = FALSE;

	m_AuthAlg = 0;

	m_pBSSIDList = NULL;
	m_cbBSSIDList = 0;
	m_pcbBSSIDListNeeded = NULL;

	m_pTransmitBufferArray = NULL;
	m_cbTransmitBufferArray = 0;
	m_TxPending = 0;
	m_RxPacketPool = NULL;


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
	//Default listen interval set to 10 beacon Intervals (100 TUs)
	m_ListenInterval = MIN_LISTEN_INTERVAL;

	m_NetworkTypeInUse = Ndis802_11OFDM24;
	m_RadioDisabled = FALSE;
	m_beaconInterval = 0;

#ifdef SUPPORT_WPA2
	m_pPMKID = NULL;
#endif

#ifdef NDIS_BUS_DRIVER
	m_pBusDriverHandle = NULL;
#endif

	m_PowerChangeEvent = NULL;
	m_ResumeWlanState = 1; // ON state
	m_LastSysState = POWER_STATE_ON;

	memset(&m_tgtStats, 0, sizeof(NDIS_802_11_STATISTICS));
	m_tgtStats.Length = sizeof(NDIS_802_11_STATISTICS);

	Init();
}

CAR6KMini::~CAR6KMini()
{
}

#define DBGLOG "[CAR6K]"

void 
CAR6KMini::Init()
{
	m_pWMI         = NULL;
	m_WMIReady     = false;
	m_HTCStarted   = false;

	m_Connected    = false;
	m_ConnectInProgress = false;
	m_ConnectedChannel  = 0;
	memset(m_PeerBSSID, 0, sizeof(m_PeerBSSID));

	m_WantToBeConnected = false;
	m_SSID.SsidLength = 0;
	m_WMIBssFilter  = NONE_BSS_FILTER;
	memset(m_SSID.Ssid, 0, sizeof(m_SSID.Ssid));

	m_GrpwiseCryptoIndex = 0;
	m_PairwiseCryptoType = NONE_CRYPT;
	m_GrpwiseCryptoType = NONE_CRYPT;


	m_enableuartprint	= 0;
	m_enabletimerwar	= 0;
	m_fwmode			= 1;
	m_mbox_yield_limit	= 99;
	m_processDot11Hdr	= 0;
	m_bIsDestroyProgress = false;
	m_RefClock			= 26000000;
	m_bDebugdriver		= true;
	m_WorkMode			= 0;		

	m_WMIEnabled		= false;
	memset(	m_RxBuffers, 0, sizeof(m_RxBuffers));
	memset(m_Ac2EpMapping, 0, sizeof(m_Ac2EpMapping));
	m_ControlEp 		= ENDPOINT_UNUSED;
	m_MgmtFilter		= 0;
	m_WlanState			= WLAN_ENABLED;

	m_SelectHiddenSsid	= 0;

	m_ScanCtrlFlag		= 0;

}



void
CAR6KMini::ReceiveWMIControlPacket(	HTC_PACKET  *evInfo)
//
//  This function processes data from an HTC_BUFFER_RECEIVED indication
//  on the WMI_CONTROL_MBOX endpoint.
//
{
	ndis_mini_buf_t *pb;
	
	NDIS_DEBUG_PRINTF(0, " %s() + Enter \r\n",__FUNCTION__);

	
	pb = (ndis_mini_buf_t *)evInfo->pPktContext;
	
	ASSERT(pb);

	if (evInfo->Status == A_OK) 
	{		
		Lock();
		wmi_control_rx((wmi_t *)m_pWMI, pb, m_PhyCapability);		
		Unlock();
	} 
  
	NDIS_DEBUG_PRINTF(0, "%s() - Exit \r\n",__FUNCTION__);
}

static void
AR6KHTCBufferReceivedEventHandler(
	HTC_TARGET      *target,
	HTC_ENDPOINT_ID  eid,
	HTC_EVENT_ID     evId,
	HTC_EVENT_INFO  *evInfo,
	void            *Context)
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
#if 0
	CAR6KMini *pAdapter = (CAR6KMini *)Context;

	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: AR6KHTCBufferReceivedEventHandler MBOX=%u \r\n", eid);

      
	if (evInfo->status == A_ECANCELED) {
		ndis_mini_buf_t *pb = (ndis_mini_buf_t *)evInfo->cookie;
		NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: AR6KHTCBufferReceivedEventHandler freeing buffer: pb = %x \r\n",(unsigned int)pb);
		a_netbuf_free(pb);
		return;
	}

	// Demux control packets from data traffic
	if (WMI_CONTROL_MBOX == eid)
	{
		// WMI Control packet (e.g. WMI_READY_EVENT)
		pAdapter->ReceiveWMIControlPacket(evInfo);
	}
	else
	{
		// WMI Data packet (e.g. IP data packet)
		pAdapter->ReceiveWMIDataPacket(evInfo);
	}
#else
	NDIS_DEBUG_PRINTF(1, "%s() is not used !! \r\n", __FUNCTION__);
#endif
}


static void
AR6KHTCDataAvailableEventHandler(
	HTC_TARGET      *target,
	HTC_ENDPOINT_ID  eid,
	HTC_EVENT_ID     evId,
	HTC_EVENT_INFO  *evInfo,
	void            *Context)
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

	NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: +AR6KHTCDataAvailableEventHandler \r\n");

	// Provide HTC with some more receive buffers
	// TODO - Impose some maximum cap
	pAdapter->AllocateHTCPacketBuffers(eid, AR6000_MAX_RX_BUFFERS);

	NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: -AR6KHTCDataAvailableEventHandler \r\n");
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

	
	struct ar_cookie * ar_cookie = (struct ar_cookie *)cookie;
	void* pktbuf;
	PAR6K_TRANSMIT_BUFFER pTxBuffer;


	pktbuf = (void *) ar_cookie->arc_bp[0];
	pTxBuffer = (PAR6K_TRANSMIT_BUFFER) pktbuf;
		
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() +Enter\r\n", __FUNCTION__);	

	
	if ((PVOID)m_pTransmitBufferArray <= pktbuf 
	&&  pktbuf <= (PVOID)((PBYTE)m_pTransmitBufferArray + m_cbTransmitBufferArray))
	{		
		// This was a transmit packet buffer that we sent
		HTCDataSendCompleted(pktbuf);

	}
	else
	{
		// This was a WMI control message. Usually they are WMI_CONTROL_MBOX,
		// but WMI_SYNC messages may also be sent on data MBOXes

		A_NETBUF_FREE(pktbuf);
	}

	FreeCookie(ar_cookie);
}


static void
AR6KHTCBufferSentEventHandler(
	HTC_TARGET        *target,
	HTC_ENDPOINT_ID    eid,
	HTC_EVENT_ID       evId,
	HTC_EVENT_INFO    *evInfo,
	void              *Context)
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
	PVOID      cookie = evInfo->cookie;

	pAdapter->HTCBufferSentEvent(cookie);
}


static int findchar(char *data, char ch)
{
	unsigned int i;
	for(i=0; i< strlen(data); i++)
	{
		if(data[i] == ch)	return i;
	}

	return 0;
}


static int GetChannelList(char *list, A_UINT16 *chList)
{
	int ret=0,chCnt=0,len;
	char tempStr[5];

	len = strlen(list);
	if(len < 4)
		return 0;

	while(1)
	{
		ret = findchar(list, ',');	

		/* max channel freq is 2432 */
		if(ret >= 5) 
				break;

		memset(tempStr,0,5);
		
		if(ret)	{
			memcpy(tempStr,list,ret);	
			chList[chCnt] = atoi(tempStr);	// covert char to integer
			list = list+ret+1;				//move next posistion
			chCnt++;
		} else {
			chList[chCnt] = atoi(list);
			chCnt++;
			break;
		}
		
	}
	
	return chCnt;
}


A_STATUS
CAR6KMini::RegisterEndpointEventHandlers(
	HTC_ENDPOINT_ID endPointId)


//
//  Connect the specified endpoint to its event handling functions for
//  Send and Receive events.
//
{
	A_STATUS        status = A_ERROR;

	NDIS_DEBUG_PRINTF(DBG_TRACE, "[AR6K] Enter -> RegisterEndpointEventHandlers() \r\n");
	
    if (status != A_OK) {
	    NDIS_DEBUG_PRINTF(DBG_ERR, "AR6K: RegisterEndpointEventHandlers failed, error=%u \r\n", status);
    }
	return status;

	NDIS_DEBUG_PRINTF(DBG_TRACE,"[AR6K] Exit -> RegisterEndpointEventHandlers() \r\n");
}


A_STATUS
CAR6KMini::StartEndpoints()
//
//  This function initializes the endpoints used for communication
//  with the AR6000 SDIO card.
//
//  ENDPOINT1 is used for command/status packets.
//  ENDPOINT2 is used for data traffic (e.g. TCP/IP) packets.
//
{
	A_STATUS        status = A_OK;

	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() + Enter \r\n",__FUNCTION__);

	AR6KConfigureTarget(m_pHIFDevice);
	
	status = DeviceInit();
		
	if (status!= A_OK) 
	{
		if(m_bDebugdriver) 
			NDIS_DEBUG_PRINTF(DBG_ERR, "%s(): DeviceInit failed, ret=%d\n", __FUNCTION__, status);
		
		goto done;
	}
	
	if (m_WorkMode == 0)
	{


		/* Set Channel List */
		{
			UINT			chNum=0;
			A_UINT16 chan_list[16] = {0};
			
			chNum = GetChannelList((char*)m_Config.channelList, chan_list);
			NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "channelList Number = %d \r\n", chNum );		

			for(UINT i=0; i<chNum; i++)
			{
				NDIS_DEBUG_PRINTF(1, "%s() -> ChannelList[%d] = %d \r\n", __FUNCTION__, i, chan_list[i]);
			}

			if(chNum)
			{
				Lock();		
				if (wmi_set_channelParams_cmd((wmi_t *)m_pWMI, 1, WMI_11G_MODE, chNum, chan_list) != A_OK) 
				{
					if(m_bDebugdriver) 
						NDIS_DEBUG_PRINTF(DBG_ERR, "Fialed to configure channel number\n");
				}
				Unlock();				
			}
		}

		/* ATHENV V10.1 +++
	 	*/
		if (m_TxPwr > 0 ) 
		{

			Lock();
			if ( wmi_set_txPwr_cmd((wmi_t *)m_pWMI, m_TxPwr) != A_OK )	{
				if(m_bDebugdriver) 
					NDIS_DEBUG_PRINTF(DBG_ERR, "Failed to set txpwr : %d\n", m_TxPwr);
			} else 	{
				if(m_bDebugdriver) 
					NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "AR6K: set txpwr : %d successfully\n", m_TxPwr);
			}
			Unlock();
			
		}

		/*
			Set Roaming Param
		*/
		{
			WMI_SET_ROAM_CTRL_CMD cmd;
			A_MEMZERO(&cmd, sizeof(cmd));

			if(m_Config.hostAssistedRoaming == 1)
			{
				cmd.roamCtrlType = WMI_SET_LOWRSSI_SCAN_PARAMS;
	
				cmd.info.lrScanParams.lowrssi_scan_period	= (A_UINT16)m_Config.bkScanPeriod;
				cmd.info.lrScanParams.lowrssi_scan_threshold =  0 - (A_INT16)m_Config.ScanThreshold;
				cmd.info.lrScanParams.lowrssi_roam_threshold =  0 -(A_INT16)m_Config.RoamThreshold;
				cmd.info.lrScanParams.roam_rssi_floor = 0;
	
			}
			else // hostAssistedRoaming ==  0 or 2
			{
				cmd.roamCtrlType = WMI_SET_LOWRSSI_SCAN_PARAMS;
			}

			NDIS_DEBUG_PRINTF(1," hostAssist = %d, ScanThreshold = %d, roamThreshold = %d, rssiFloor = %d \r\n",
							m_Config.hostAssistedRoaming,
							cmd.info.lrScanParams.lowrssi_scan_threshold,
							cmd.info.lrScanParams.lowrssi_roam_threshold,
							cmd.info.lrScanParams.roam_rssi_floor);
		
			Lock();
			
			wmi_set_roam_ctrl_cmd((wmi_t *)m_pWMI, &cmd, sizeof(cmd));
			
			Unlock();
							

		}
	#if 0	
		/* ATHENV V10.1 ---
	 	*/
		/* start a scan immediately if it's normal WIFI mode */
		Lock();
		if (wmi_bssfilter_cmd((wmi_t *)m_pWMI, ALL_BSS_FILTER, 0) != A_OK)
			NDIS_DEBUG_PRINTF(DBG_ERR, "Fialed to set filter\n");
				
		if (wmi_startscan_cmd((wmi_t *)m_pWMI, WMI_LONG_SCAN, FALSE, FALSE, 0, 0, 0, NULL) != A_OK)
			NDIS_DEBUG_PRINTF(DBG_TRACE, "Failed to send scan cmd\n");

		/* configure disconnect timeout value */
		if (wmi_disctimeout_cmd((wmi_t *)m_pWMI, m_Config.discTimeout) != A_OK)
			NDIS_DEBUG_PRINTF(DBG_ERR, "Failed to disctimeout cmd\n");				
		Unlock();
	#endif
	
	}


	if (A_OK != status)
		goto done;
    
	status = configTargetParams();

done:
	NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: -StartEndpoints status=%u \r\n", status);
	return status;
}


void
CAR6KMini::StopEndpoints()
//
//  Undoes the initialization done by StartEndpoints.
//
{
	NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K,"AR6K: +StopEndpoints \r\n");
	NDIS_DEBUG_PRINTF(1,"AR6K: +StopEndpoints \r\n");

	if(m_HTCStarted)
	{
		HTCStop(m_hHtcTarget);
		m_HTCStarted = FALSE;
	}

	if (m_WMIReady == TRUE)
	{
		if (!m_Config.byPassWmi)
		{
		
			m_WMIReady  = FALSE;
			m_Connected = FALSE;
			wmi_shutdown((wmi_t *)m_pWMI);
			m_WMIEnabled = FALSE;
			m_pWMI = NULL;
        }

         NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "%s(): WMI stopped\n", __FUNCTION__);
    }
    else
    {
        NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "%s(): WMI not ready ",  __FUNCTION__);

        /* Shut down WMI if we have started it */
        if(m_WMIEnabled == TRUE) {
            NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "%s(): Shut down WMI\n", __FUNCTION__);
            wmi_shutdown((wmi_t *)m_pWMI);
            m_WMIEnabled = FALSE;
            m_pWMI = NULL;
        }
    }

	CookieCleanUp();

	NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: -StopEndpoints \r\n");
}


void CAR6KMini::SetWmiHandle(struct wmi_t *wmi)
{
	m_pWMI = wmi;
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
		    m_ListenInterval = MAX_LISTEN_INTERVAL;
            m_80211_PowerMode = Ndis802_11PowerModeMAX_PSP;
		    break;
	    case 2:
		    m_ListenInterval = MIN_LISTEN_INTERVAL;
            m_80211_PowerMode = Ndis802_11PowerModeFast_PSP;
		    break;
        default:
            return A_ERROR;
	}

	if(m_Config.hostAssistedRoaming == 0)
	{
		m_ScanCtrlFlag = NO_ROAM_SCAN_CTRL_FLAGS;
	}
	else
		m_ScanCtrlFlag = DEFAULT_SCAN_CTRL_FLAGS;

	if(m_Config.dbgSetOid)		SetPrintMask(DBG_SET_OID);
	if(m_Config.dbgQueryOid)	SetPrintMask(DBG_QUERY_OID);
	if(m_Config.dbgBSSEvent)	SetPrintMask(DBG_BSS_EVENT);
	if(m_Config.dbgBSSList)		SetPrintMask(DBG_BSS_LIST);
	if(m_Config.dbgErrorLog)	SetPrintMask(DBG_ERR_LOG);

	RETAILMSG(1,(TEXT("[WLAN ]dbgSetOid(%d),dbgQueryOid(%d),dbgBSSEvent(%d),dbgBSSList(%d),dbgErrorLog(%d) \r\n"),
				m_Config.dbgSetOid,m_Config.dbgQueryOid,m_Config.dbgBSSEvent,m_Config.dbgBSSList,m_Config.dbgErrorLog));


	SetDbgFileLog(m_Config.dbgFileLog);

	{
		char buffer[256];
		wchar_t wBuf[512];
		
		SYSTEMTIME st;
		TCHAR szCurTime[128]={0,};
		
		GetLocalTime(&st);
		 

		memset(buffer,0,256);
		memset(wBuf,0,512);
		
		sprintf(buffer,"%s\\wlan_%04d%02d%02d.txt",m_Config.dbgLogDir,st.wYear,st.wMonth,st.wDay);

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, wBuf, 256 );
		
		DebugInit(m_Config.dbgLogLevel, wBuf);
		

	}


    return A_OK;
}


/* Configure the target from the registry settings only in wmi mode */
A_STATUS
CAR6KMini::configTargetParams()
{
	A_STATUS   wmiStatus = A_OK;
	A_UINT8 powerMode = 0;

	NDIS_DEBUG_PRINTF(DBG_TRACE, " Enter configTargetParams() need modified ~~~~~ !! \r\n");
//bluebird		

	if (!m_Config.byPassWmi) 
	{
	
		if( wmi_scanparams_cmd((wmi_t *)m_pWMI, 0, 0, (A_UINT16)m_Config.bkScanPeriod, 100, 110, 0, 1, m_ScanCtrlFlag, 0, 0) != A_OK )
			NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "AR6K2:ar6000_init(): Scan parameters setting failed\r\n");

		/* configure disconnect timeout value */
		if (wmi_disctimeout_cmd((wmi_t *)m_pWMI, m_Config.discTimeout) != A_OK)
			NDIS_DEBUG_PRINTF(DBG_ERR, "Failed to disctimeout cmd\n");				


		/* restore the target Power Save settings */
		
		if ( wmiStatus == A_OK ) {
			
			switch( m_80211_PowerMode ) {

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
				// this will never occur.
				break;
			}

			wmiStatus = wmi_powermode_cmd((struct wmi_t *)m_pWMI, powerMode);

		}
	}


	if(wmiStatus == A_OK)
	{
		wmiStatus = wmi_bssfilter_cmd((struct wmi_t *)m_pWMI,ALL_BSS_FILTER,0);
		if(wmiStatus != A_OK)
		{
			m_WMIBssFilter = ALL_BSS_FILTER;
			wmiStatus = wmi_startscan_cmd((struct wmi_t *)m_pWMI,WMI_LONG_SCAN,FALSE,FALSE,0,0,0,NULL);
			NDIS_DEBUG_PRINTF(DBG_TRACE," configTargetParams() !! wmiStatus = %d \r\n",wmiStatus);		
		}
	}


	NDIS_DEBUG_PRINTF(DBG_TRACE," Exit configTargetParams() !! wmiStatus = %d \r\n",wmiStatus);
	return wmiStatus;
}



A_STATUS 
CAR6KMini::AR6KConfigureTarget(HIF_DEVICE *device)
{
		A_UINT32 param;
	
		NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "ar6000_configure_target() + Enter \r\n");
		if (m_enableuartprint) 
		{
			param = 1;
			if (BMIWriteMemory(device,	HOST_INTEREST_ITEM_ADDRESS(m_TargetType, hi_serial_enable),
								(A_UCHAR *)&param,
								4)!= A_OK)
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "BMIWriteMemory for enableuartprint failed \n");
				return A_ERROR;
			}
			
			NDIS_DEBUG_PRINTF(DBG_TRACE, "Serial console prints enabled\n");
		}
	
		/* Tell target which HTC version it is used*/
		param = HTC_PROTOCOL_VERSION;
		if (BMIWriteMemory(device,	HOST_INTEREST_ITEM_ADDRESS(m_TargetType, hi_app_host_interest),
							(A_UCHAR *)&param,
							4)!= A_OK)
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, "BMIWriteMemory for htc version failed \n");
			return A_ERROR;
		}
	

		if (m_enabletimerwar) 
		{
			A_UINT32 param;
	
			if (BMIReadMemory(device,
								HOST_INTEREST_ITEM_ADDRESS(m_TargetType, hi_option_flag),
								(A_UCHAR *)&param,
								4)!= A_OK)
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "BMIReadMemory for enabletimerwar failed \n");
				return A_ERROR;
			}
	
			param |= HI_OPTION_TIMER_WAR;
	
			if (BMIWriteMemory(device,
								HOST_INTEREST_ITEM_ADDRESS(m_TargetType, hi_option_flag),
								(A_UCHAR *)&param,
								4) != A_OK)
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "BMIWriteMemory for enabletimerwar failed \n");
				return A_ERROR;
			}
			
			NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "Timer WAR enabled\n");
			
		}
	
		/* set the firmware mode to STA/IBSS/AP */
		{
			A_UINT32 param;
	
			if (BMIReadMemory(device,
								HOST_INTEREST_ITEM_ADDRESS(m_TargetType, hi_option_flag),
								(A_UCHAR *)&param,
								4)!= A_OK)
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "BMIReadMemory for setting fwmode failed \n");
				return A_ERROR;
			}
	
			param |= (m_fwmode << HI_OPTION_FW_MODE_SHIFT);
	
			if (BMIWriteMemory(device,
								HOST_INTEREST_ITEM_ADDRESS(m_TargetType, hi_option_flag),
								(A_UCHAR *)&param,
								4) != A_OK)
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "BMIWriteMemory for setting fwmode failed \n");
				return A_ERROR;
			}
			
			NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K,"Firmware mode set\n");
			
		}
		
		if (m_processDot11Hdr) 
		{
			A_UINT32 param;
	
			if (BMIReadMemory(device,
								HOST_INTEREST_ITEM_ADDRESS(m_TargetType, hi_option_flag),
								(A_UCHAR *)&param,
								4)!= A_OK)
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "BMIReadMemory for processDot11Hdr failed \n");
				return A_ERROR;
			}
	
			param |= HI_OPTION_RELAY_DOT11_HDR;
	
			if (BMIWriteMemory(device,
								HOST_INTEREST_ITEM_ADDRESS(m_TargetType, hi_option_flag),
								(A_UCHAR *)&param,
								4) != A_OK)
			{
				NDIS_DEBUG_PRINTF(DBG_ERR, "BMIWriteMemory for processDot11Hdr failed \n");
				return A_ERROR;
			}
			
			NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K,"processDot11Hdr enabled\n");
			
		}
		
	
		/* since BMIInit is called in the driver layer, we have to set the block
		* size here for the target */
	
		if (A_FAILED(ar6000_set_htc_params(device,
											m_TargetType,
											m_mbox_yield_limit,
											0 /* use default number of control buffers */
											))) 
		{
			return A_ERROR;
		}
	
	
		NDIS_DEBUG_PRINTF(DBG_TRACE, "ar6000_configure_target() - Exit \r\n");
		return A_OK;
	
}







#ifdef WINDOWS_MOBILE
A_STATUS setWiFiStator(DWORD value)
{
	HKEY handle;
	LONG result;
	DWORD data;
	A_STATUS status = A_ERROR;

	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SYSTEM\\STATE\\HARDWARE"),0,0,&handle);
	if (result == ERROR_SUCCESS) {
		//data = 0x1; //Start with WLAN module available
		data = value; //Start with WLAN module available
		result = RegSetValueEx(handle,_T("WiFi"),0,REG_DWORD,(LPBYTE)&data,sizeof(DWORD));
		if (result == ERROR_SUCCESS) {
			status = A_OK;
		}
      	RegCloseKey(handle);
	}

	return status;
}
#endif


A_UINT32 
CAR6KMini::getResetPowerState()
{
#ifdef WINDOWS_MOBILE
	BOOL          wirelessState = 0;
#endif
	A_UINT32      powerState;

	// Read the reset state configured thru the registry
	powerState = m_Config.resetPowerState;

	NDIS_DEBUG_PRINTF(DBG_TRACE, "m_Config.resetPowerState = %d \r\n",m_Config.resetPowerState);
#ifdef WINDOWS_MOBILE
	// For windows mobile read the flight mode status
	if (GetWirelessState(&wirelessState) == ERROR_SUCCESS) {
		powerState = powerState & (wirelessState ? 1 : 0);   
		NDIS_DEBUG_PRINTF(DBG_TRACE,"Reset wirelessState = %d \r\n",wirelessState);
	}
#endif

	NDIS_DEBUG_PRINTF(DBG_TRACE, "Reset Power State = %d \r\n",powerState);

	return powerState;
}


#if defined CF
#ifdef WINDOWS_MOBILE
#define AR6K_DEVICE_POWER_NAME TEXT("SYSTEM\\netui-AR6K1")
#else
#define AR6K_DEVICE_POWER_NAME _T("{98C5250D-C29A-4985-AE5F-AFE5367E5006}\\AR6K_CF1")
#endif
#elif defined SDIO
#define AR6K_DEVICE_POWER_NAME _T("{98C5250D-C29A-4985-AE5F-AFE5367E5006}\\AR6K_SD1")
#else
#error "Bus type not defined"
#endif


void
CAR6KMini::ResetPowerWorkItem()
{
	A_UINT32      powerState;
#ifdef WINDOWS_MOBILE
	RDD * pDevice = NULL;
	RDD * pTD;
	HRESULT hr = S_OK;
	HANDLE driverStateEvent;
#else
	CEDEVICE_POWER_STATE cePowerState;
	DWORD         status;
#endif
	A_STATUS      athStatus=A_ERROR;

	NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "ResetPowerWorkItem :: Entry \r\n");

	NdisResetEvent(&m_ResetPowerWorkItemExitedEvent);

	if (!m_Config.byPassWmi) 
	{
		// Dual-Purpose workitem 
		// Indicate disconnect after the MiniportInit complete successfully
		// This is needed for 802.11 miniports
		while (m_Halting == false) 
		{
			NdisMSleep(1000);
			//NDIS_DEBUG_PRINTF(DBG_TRACE,"---- TEST 1 m_InitComplste = %d, m_WMIReady = %d  ------ \r\n", m_InitComplete, m_WMIReady);
			if ((m_InitComplete) && (m_WMIReady)) 
			{
				NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
				NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
				break;
			}
		}
        
		powerState = getResetPowerState();
		if ((!powerState) && (m_Halting == false)) {    
#ifdef WINDOWS_MOBILE

			// Windows mobile notifies this event when the driver has completed initalization
			driverStateEvent = CreateEvent(NULL, FALSE, FALSE, AR6K_DEVICE_POWER_NAME);
			if (driverStateEvent != NULL) 
			{
				NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, " Skip driverStateEvent !! \r\n");
				WaitForSingleObject(driverStateEvent, INFINITE);

				CloseHandle(driverStateEvent);
			}

			// Wait till the WiFi entry is created
			while (m_Halting == false) 
			{
				
				hr = GetWirelessDevices(&pDevice, 0);

				NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, " hr = %d, pDevice = 0x%08x , pDevice = 0x%x\r\n", hr, pDevice,pDevice->DeviceType);
				if (hr == S_OK) 
				{
					if (pDevice) 
					{
						pTD = pDevice;
						// loop through the linked list of devices
						while (pTD) 
						{ 								
							if (pTD->DeviceType==POWER_MANAGED) 
							{
								hr=ChangeRadioState(pTD, 0, POWER_POST_SAVE);
								if (hr == S_OK) 
								{
									athStatus = A_OK;
									// Update the registry state
									UpdateRadioStoredState(pTD,0);	
								}
							}								
							pTD = pTD->pNext;
						}
	
						// Free the list of devices retrieved with GetWirelessDevices()
						FreeDeviceList(pDevice);
						pDevice = NULL;
						break;
					}
				}
				else 
				{
					break;
				}
				NdisMSleep(1000);
			} 
					
			if (pDevice) {
				FreeDeviceList(pDevice);
			}
#else //WINDOWS_MOBILE
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
				while (m_Halting == false) {
					status= pfnGetDevicePower((PVOID)AR6K_DEVICE_POWER_NAME, POWER_NAME, &cePowerState);
					if (status != ERROR_FILE_NOT_FOUND) {
						break;
					}
					NdisMSleep(1000);
				} 
            
	
				if ((status == ERROR_SUCCESS) && (m_Halting == false)) {
					cePowerState = D4;
					status = pfnDevicePowerNotify((PVOID)AR6K_DEVICE_POWER_NAME,cePowerState,POWER_NAME);
					if (status == ERROR_SUCCESS) {
						athStatus = A_OK;
					}
				}
			}
	
			if (NULL != hCoreDll) {
				FreeLibrary(hCoreDll);
			}
#endif
			if (athStatus == A_ERROR) {
				NDIS_DEBUG_PRINTF(DBG_ERR,"ResetPowerWorkItem :: Reset power state set failed \r\n");
			}  else {
				NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K,"ResetPowerWorkItem :: Reset power state set success\r\n");
			}
		}
	}

	NdisSetEvent(&m_ResetPowerWorkItemExitedEvent);
    
	NDIS_DEBUG_PRINTF(DBG_TRACE,"ResetPowerWorkItem :: Exit \r\n");

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

    m_hHtcTarget = cmnGetHTCHandle();

    if (NULL == m_hHtcTarget)
    {
       Status = NDIS_STATUS_FAILURE;
       goto done;
    }

    m_pHIFDevice = (HIF_DEVICE *)HTCGetHifDevice(m_hHtcTarget);
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

    if ((byFirmwareMajor == ATH_SW_VER_MAJOR) &&
        (byFirmwareMinor == ATH_SW_VER_MINOR))
    {
        // Both versions are same
        bIsVersionMismatch = FALSE;
    }

    if (A_OK != StartEndpoints())
    {
        Status = NDIS_STATUS_FAILURE;
        goto done;
    }

done:
    return Status;
}



//#define	MEMORY_CHECK_USE 

#ifdef MEMORY_CHECK_USE

static A_TIMER                 g_MemoryCheck_timer;

static void MemoryUsingCheck(void *arg)
{
	MEMORYSTATUS state;

	state.dwLength=sizeof(&state);
	
	GlobalMemoryStatus(&state);

	NDIS_DEBUG_PRINTF(1, " Total Memory = %d.%03d.%d Byte,  Free Memory = %d.%03d.%d \r\n",
					state.dwTotalPhys/1000000,(state.dwTotalPhys/1000)%1000,state.dwTotalPhys%1000,
					state.dwAvailPhys/1000000,(state.dwAvailPhys/1000)%1000,state.dwAvailPhys%1000);
	
	//str.Format(_T("실제 메모리 %d byte,\n사용가능한 메모리 %d\n,메모리 사용률 %d"),state.dwTotalPhys,state.dwAvailPhys,state.dwMemoryLoad);

	A_TIMEOUT_MS(&g_MemoryCheck_timer, 2000, 0);
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
	NDIS_STATUS   Status;
	A_STATUS      athStatus = A_OK;

	NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K,"AR6K: +Initialize \r\n");


	NdisInitializeEvent(&m_tgtReadyEvent);
	NdisInitializeEvent(&m_WMIReadyEvent);
	NdisInitializeEvent(&m_RxPendingEvent);
	NdisInitializeEvent(&m_RxWorkItemExitedEvent);
	NdisInitializeEvent(&m_tgtStatsEvent);
	NdisInitializeEvent(&m_ResetPowerWorkItemExitedEvent);
	NdisInitializeEvent(&m_SuspendEvent);
    
	NdisSetEvent(&m_RxWorkItemExitedEvent);
	NdisSetEvent(&m_ResetPowerWorkItemExitedEvent);
	NdisResetEvent(&m_tgtReadyEvent);

	m_MaxTransmitBuffers = DEFAULT_MAX_TRANSMIT_BUFFERS;
	m_MaxTransmitBufferPayloadDataLength = DEFAULT_MAX_TRANSMIT_BUFFER_PAYLOAD_DATA_LENGTH;
	InitializeListHead(&m_TransmitBufferList);
	InitializeListHead(&m_TransmitNdisPacketList);
	InitializeListHead(&m_RxPendingPacketList);

	memset(m_DesiredAPBSSID, 0xFF, sizeof(m_DesiredAPBSSID));

	m_PowerChangeEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("AR6K_PowerChange"));
	if (m_PowerChangeEvent == NULL) {
		goto done;
	}

	// Perform base class initialization first
	Status = CMiniport::Initialize(MiniportAdapterHandle, ConfigHandle);
	if (NDIS_STATUS_SUCCESS != Status)
		goto done;

	m_ConfigHandle = ConfigHandle;
	Status = ParseRegistryParameters(ConfigHandle, &m_Config);
	if (NDIS_STATUS_SUCCESS != Status)
		goto done;

    if (configRegistryParameters() != A_OK)
        goto done;


	//
	// If the SDIO component is uninitialized, then we should not be
	// getting called. If we do, for some reason, then return an error.
	//

	Status = TransmitBufferListAllocate();
	if (NDIS_STATUS_SUCCESS != Status)
		goto done;

	NdisAllocatePacketPool(&Status, &m_RxPacketPool, AR6000_MAX_RX_BUFFERS, 16);
	if (NDIS_STATUS_SUCCESS != Status)
		goto done;

	// Create a work item to peform receive packet indications
	NdisInitializeWorkItem(&m_WorkItem, AR6KWorkItemFunction, this);
	Status = NdisScheduleWorkItem(&m_WorkItem);
	if (NDIS_STATUS_SUCCESS != Status)
		goto done;

    // Create a work item to power off wlan upon reset
	NdisInitializeWorkItem(&m_ResetPowerWorkItem, ResetPowerWorkItemFunction, this);
	Status = NdisScheduleWorkItem(&m_ResetPowerWorkItem);
	if (NDIS_STATUS_SUCCESS != Status)
		goto done;

	Status = TargetInitialize ();

#ifdef WINDOWS_MOBILE
	athStatus = setWiFiStator(1);
	if (athStatus != A_OK) {
		athStatus = A_ERROR;
		goto done;
    }
#endif

	m_InitComplete = true;

done:
	if (athStatus != A_OK)
		Status = NDIS_STATUS_FAILURE;

	if (NDIS_STATUS_SUCCESS != Status)
	{
		// If we fail Initialize, then our Halt won't be called by NDIS,
		// so clean up all resources allocated by Initialize.
		Halt();
	}
#ifdef MEMORY_CHECK_USE
	if(Status == A_OK)	{
		A_INIT_TIMER(&g_MemoryCheck_timer, MemoryUsingCheck, NULL);
		A_TIMEOUT_MS(&g_MemoryCheck_timer, 2000, 0);
	}
#endif

	NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: -Initialize status=%x \r\n", Status);

	return Status;
}

void
CAR6KMini::Halt()
//
// This function undoes Initialize.
// It stops the adapter and frees resources
//
{
	NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "AR6K: +Halt \r\n");

	m_Halting = true;
	if (m_Connected) {
		NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
		NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
		m_Connected = FALSE;
	}

    if (m_ConfigHandle)
    {
      //  NdisCloseConfiguration (m_ConfigHandle);
        m_ConfigHandle = NULL;
    }

	
	// Wakeup the Rx Work Item thread so it can exit
	NdisSetEvent(&m_RxPendingEvent);
	NdisWaitEvent(&m_RxWorkItemExitedEvent, 0);

	// Wakeup the Get Stats thread if any
	NdisSetEvent(&m_tgtStatsEvent);
	
    // Wakeup the SetPnpPower() OID thread that may be waiting for a SuspendEvent.
    NdisSetEvent(&m_SuspendEvent);

	NdisWaitEvent(&m_ResetPowerWorkItemExitedEvent, 0);

	NdisFreeEvent(&m_tgtReadyEvent);
	NdisFreeEvent(&m_WMIReadyEvent);
	NdisFreeEvent(&m_RxPendingEvent);
	NdisFreeEvent(&m_RxWorkItemExitedEvent);
	NdisFreeEvent(&m_tgtStatsEvent);
	NdisFreeEvent(&m_ResetPowerWorkItemExitedEvent);
	
    NdisFreeEvent(&m_SuspendEvent);

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
//bluebird
#if 0
	if (m_HTCInited) {
		HTCShutDown(m_hHtcTarget);
        m_HTCInited = false;
	}
#endif    
	CMiniport::Halt();

	if (m_pTransmitBufferArray)
		A_FREE(m_pTransmitBufferArray);

	if (m_RxPacketPool)
		NdisFreePacketPool(m_RxPacketPool);

	NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: -Halt \r\n");
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



/* This function does one time initialization for the lifetime of the device */
A_STATUS 
CAR6KMini::DeviceInit(void)
{

    A_STATUS    status;

    /* Do we need to finish the BMI phase */
	/* Start AR6002 Firmware */
	if(BMIDone(m_pHIFDevice) != A_OK)
	{
		NDIS_DEBUG_PRINTF(DBG_ERR, " Failed BMIDone(ar->arHifDevice) !! \r\n");
		return A_ERROR;
	}

	if (!m_Config.byPassWmi)
	{

		/* Indicate that WMI is enabled (although not ready yet) */
		m_WMIEnabled = TRUE;
		if ((m_pWMI= wmi_init(this)) == NULL)
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, "DeviceInit(): Failed to initialize WMI. \r\n");
			return(A_ERROR);
		}
		
		NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "DeviceInit(): Got WMI @ 0x%08x. \r\n",(unsigned int)m_pWMI);
	}

	do 
	{
		HTC_SERVICE_CONNECT_REQ connect;

		/* the reason we have to wait for the target here is that the driver layer
		* has to init BMI in order to set the host block size,
		*/
		status = HTCWaitTarget(m_hHtcTarget);

		if (A_FAILED(status)) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, " Failed HTCWaitTarget() \r\n");
			break;
		}

		A_MEMZERO(&connect,sizeof(connect));
		
		/* meta data is unused for now */
		connect.pMetaData = NULL;
		connect.MetaDataLength = 0;
		
		/* these fields are the same for all service endpoints */
		connect.EpCallbacks.pContext = this;
		connect.EpCallbacks.EpTxComplete = ar6k_tx_complete;//ar6000_tx_complete;
		connect.EpCallbacks.EpRecv = ar6k_rx;//ar6k_rx;
		connect.EpCallbacks.EpRecvRefill = ar6000_rx_refill;
		connect.EpCallbacks.EpSendFull = ar6000_tx_queue_full;
		
		/* set the max queue depth so that our ar6000_tx_queue_full handler gets called.
		* Linux has the peculiarity of not providing flow control between the
		* NIC and the network stack. There is no API to indicate that a TX packet
		* was sent which could provide some back pressure to the network stack.
		* Under linux you would have to wait till the network stack consumed all sk_buffs
		* before any back-flow kicked in. Which isn't very friendly.
		* So we have to manage this ourselves */
		connect.MaxSendQueueDepth = 32;

		/* connect to control service */
		connect.ServiceID = WMI_CONTROL_SVC;
		status = AR6KConnectSvc(&connect, "WMI CONTROL");
		
		if (A_FAILED(status)) 
		{
			NDIS_DEBUG_PRINTF(DBG_TRACE, "ar6000_connectservice() Break 1\r\n");
			break;
		}

		/* for the remaining data services set the connection flag to reduce dribbling,
		* if configured to do so */
		if (reduce_credit_dribble) 
		{
			connect.ConnectionFlags |= HTC_CONNECT_FLAGS_REDUCE_CREDIT_DRIBBLE;
			/* the credit dribble trigger threshold is (reduce_credit_dribble - 1) for a value
			* of 0-3 */
			connect.ConnectionFlags &= ~HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_MASK;
			connect.ConnectionFlags |= ((A_UINT16)reduce_credit_dribble - 1) & HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_MASK;
        }
		
		/* connect to best-effort service */
		connect.ServiceID = WMI_DATA_BE_SVC;

        status = AR6KConnectSvc(&connect, "WMI DATA BE");
        if (A_FAILED(status)) 
		{
            break;
        }

		/* connect to back-ground
             * map this to WMI LOW_PRI */
        connect.ServiceID = WMI_DATA_BK_SVC;
        status = AR6KConnectSvc(&connect, "WMI DATA BK");
        if (A_FAILED(status)) {
            break;
        }

		/* connect to Video service, map this to
             * to HI PRI */
        connect.ServiceID = WMI_DATA_VI_SVC;
        status = AR6KConnectSvc(&connect, "WMI DATA VI");
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
		status = AR6KConnectSvc(&connect, "WMI DATA VO");
		if (A_FAILED(status)) {
		    break;
		}

		A_ASSERT(Ac2EndpointID(WMM_AC_BE) != 0);
		A_ASSERT(Ac2EndpointID(WMM_AC_BK) != 0);
		A_ASSERT(Ac2EndpointID(WMM_AC_VI) != 0);
		A_ASSERT(Ac2EndpointID(WMM_AC_VO) != 0);

		    /* setup access class priority mappings */
		m_AcStreamPriMap[WMM_AC_BK] = 0; /* lowest  */
		m_AcStreamPriMap[WMM_AC_BE] = 1; /*         */
		m_AcStreamPriMap[WMM_AC_VI] = 2; /*         */
		m_AcStreamPriMap[WMM_AC_VO] = 3; /* highest */

    } while (FALSE);

	if (A_FAILED(status)) 
	{
		NDIS_DEBUG_PRINTF(DBG_ERR, "ar6000_init() Exti 1\r\n");
		return (A_ERROR);
	}

    /*
     * give our connected endpoints some buffers
     */
    ar6000_rx_refill(this, m_ControlEp);
    ar6000_rx_refill(this, Ac2EndpointID(WMM_AC_BE));
	
    /*
     * We will post the receive buffers only for SPE or endpoint ping testing so we are
     * making it conditional on the 'bypasswmi' flag.
     */
    if (m_Config.byPassWmi) 
	{
		ar6000_rx_refill(this,Ac2EndpointID(WMM_AC_BK));
		ar6000_rx_refill(this,Ac2EndpointID(WMM_AC_VI));
		ar6000_rx_refill(this,Ac2EndpointID(WMM_AC_VO));
    }

        /* setup credit distribution */
    ar6000_setup_credit_dist(m_hHtcTarget, &m_CreditStateInfo);

    /* Since cookies are used for HTC transports, they should be */
    /* initialized prior to enabling HTC.                        */
    CookieInit();


    /* start HTC */
    status = HTCStart(m_hHtcTarget);
	
    if (status != A_OK) 
	{
        if (m_WMIEnabled == TRUE) {
            wmi_shutdown((wmi_t *)m_pWMI);
            m_WMIEnabled = FALSE;
            m_pWMI = NULL;
        }
		NDIS_DEBUG_PRINTF(DBG_ERR, "HTC Start ERR !!!\n");
        CookieCleanUp();
        return A_ERROR;
    }

	m_HTCStarted = TRUE;
		
    if (!m_Config.byPassWmi) {
        
		NdisWaitEvent(&m_WMIReadyEvent, AR6K_DEFAULT_MS_TO_WAIT_FOR_WMI_READY);
		if(!m_WMIReady)
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, "Err No WMI Ready Event!!!\n");	
		}
		Sleep(1000);
		NDIS_DEBUG_PRINTF(DBG_TRACE, "SKIP writing wmi protocal version !!!\n");
	
    }

    m_NumDataEndPts = 1;

    if (m_Config.byPassWmi) 
	{
#if 0 //bluebird	
		/* for tests like endpoint ping, the MAC address needs to be non-zero otherwise
             * the data path through a raw socket is disabled */
        dev->dev_addr[0] = 0x00;
        dev->dev_addr[1] = 0x01;
        dev->dev_addr[2] = 0x02;
        dev->dev_addr[3] = 0xAA;
        dev->dev_addr[4] = 0xBB;
        dev->dev_addr[5] = 0xCC;
#endif //bluebird
    }




	

	/* Add by MMC 허진영,  don't go to sleep mode*/
	//builder:change default config

	wmi_set_nodeage((wmi_t *)m_pWMI, m_Config.nodeAge);

	if(m_Config.probeEnable) {
		/* Enable Hidden AP */
		//NDIS_DEBUG_PRINTF(1," %s() -> Enable Hidden AP \r\n",__FUNCTION__);
		//wmi_ap_set_hidden_ssid((wmi_t *)m_pWMI, 1);
	}


	if( wmi_disctimeout_cmd((wmi_t *)m_pWMI, m_Config.discTimeout) != A_OK )
		NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K2:ar6000_init(): Disconnect timeout setting failed\r\n" );
#if 1

	if( wmi_scanparams_cmd((wmi_t *)m_pWMI, 0, 0, (A_UINT16)m_Config.bkScanPeriod, 100, 110, 0, 1, m_ScanCtrlFlag, 0, 0) != A_OK )
		NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K2:ar6000_init(): Scan parameters setting failed\r\n");
		
	if( wmi_bmisstime_cmd((wmi_t *)m_pWMI, 0, 10) != A_OK )
		NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K2:ar6000_init(): Beacon miss timeout setting failed\r\n" );
			
	if( wmi_powermode_cmd((wmi_t *)m_pWMI, MAX_PERF_POWER) != A_OK )
		NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K2:ar6000_init(): Power mode setting failed\r\n" );
	
	if( wmi_pmparams_cmd((wmi_t *)m_pWMI, 100, 0, 2) != A_OK )
		NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K2:ar6000_init(): Power management setting failed\r\n" );
	
	if( wmi_listeninterval_cmd((wmi_t *)m_pWMI, 0, MIN_LISTEN_BEACONS) != A_OK )
		NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K2:ar6000_init(): Listen interval setting failed\r\n" );
#endif
    return(A_OK);
}





void
CAR6KMini::AR6KRxRefill(HTC_ENDPOINT_ID Endpoint)
{

	int         RxBuffers;
	int         buffersToRefill;
	HTC_PACKET  *pPacket;


	buffersToRefill = (int)AR6000_MAX_RX_BUFFERS - m_RxBuffers[Endpoint];

	if (buffersToRefill <= 0) 
	{
		/* fast return, nothing to fill */
		NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() : buffersToRefill <= 0 !! \r\n", __FUNCTION__);	
		return;
	}

	NDIS_DEBUG_PRINTF(DBG_TRACE,"%s(): providing htc with %d buffers at eid=%d \r\n",__FUNCTION__, buffersToRefill, Endpoint);

	for (RxBuffers = 0; RxBuffers < buffersToRefill; RxBuffers++) 
	{
		void        *osBuf;
		
		osBuf = A_NETBUF_ALLOC(AR6000_BUFFER_SIZE);

		if (NULL == osBuf) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, " %s(), malloc failed \r\n", __FUNCTION__);
			break;
		}
		
		/*	the HTC packet wrapper is at the head of the reserved area
		* 	linxue --> in the skb, wince --> in ndis_mini_buf_t  */
		
		pPacket = 	(HTC_PACKET *)A_NETBUF_HEAD(osBuf);
		/* set re-fill info */
		SET_HTC_PACKET_INFO_RX_REFILL(pPacket,osBuf,A_NETBUF_DATA(osBuf),AR6000_BUFFER_SIZE,Endpoint);		

		
		/* add this packet */
		HTCAddReceivePkt(m_hHtcTarget, pPacket);
    }

	/* update count */
	Lock();
	m_RxBuffers[Endpoint] += RxBuffers;
	Unlock();
	
}



/* connect to a service */
A_STATUS 
CAR6KMini::AR6KConnectSvc(HTC_SERVICE_CONNECT_REQ  *pConnect, char *pDesc)
{

	A_STATUS                 status;
	HTC_SERVICE_CONNECT_RESP response;

	do 
	{

		A_MEMZERO(&response,sizeof(response));

		status = HTCConnectService(m_hHtcTarget, pConnect, &response);

		if (A_FAILED(status)) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, " Failed to connect to %s service status:%d \n",pDesc, status);
		    break;
		}

		switch (pConnect->ServiceID) 
		{
			case WMI_CONTROL_SVC :
				if (m_WMIEnabled) {
					/* set control endpoint for WMI use */
					wmi_set_control_ep((wmi_t *)m_pWMI, response.Endpoint);
				}
				
				/* save EP for fast lookup */
				m_ControlEp = response.Endpoint;
				break;
				 
			case WMI_DATA_BE_SVC :
				SetAc2EndpointIDMap(WMM_AC_BE, response.Endpoint);
				break;
				 
			case WMI_DATA_BK_SVC :
				SetAc2EndpointIDMap(WMM_AC_BK, response.Endpoint);
				break;
				 
			case WMI_DATA_VI_SVC :
				SetAc2EndpointIDMap(WMM_AC_VI, response.Endpoint);
				break;
				  
			case WMI_DATA_VO_SVC :
				SetAc2EndpointIDMap(WMM_AC_VO, response.Endpoint);
				break;
				 
			default:
                NDIS_DEBUG_PRINTF(DBG_ERR, "ServiceID not mapped %d\n", pConnect->ServiceID);
                status = A_EINVAL;
            	break;
        }

    } while (FALSE);

    return status;

}


void
CAR6KMini::IndicateTxActivity(A_UINT8 TrafficClass, A_BOOL Active)
{

	HTC_ENDPOINT_ID eid ;
	int i;

	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() + Enter \r\n", __FUNCTION__);
		
	if (m_WMIEnabled) 
	{
		eid = Ac2EndpointID(TrafficClass);

		Lock();

		m_AcStreamActive[TrafficClass] = Active;

		if (Active) 
		{
			/* when a stream goes active, keep track of the active stream with the highest priority */
			if (m_AcStreamPriMap[TrafficClass] > m_HiAcStreamActivePri) 
			{
				/* set the new highest active priority */
				m_HiAcStreamActivePri = m_AcStreamPriMap[TrafficClass];
			}

		} 
		else 
		{
			/* when a stream goes inactive, we may have to search for the next active stream
			* that is the highest priority */

			if (m_HiAcStreamActivePri == m_AcStreamPriMap[TrafficClass]) 
			{

				/* the highest priority stream just went inactive */
				/* reset and search for the "next" highest "active" priority stream */
				m_HiAcStreamActivePri = 0;
				for (i = 0; i < WMM_NUM_AC; i++) 
				{
					if (m_AcStreamActive[i]) 
					{
						if (m_AcStreamPriMap[i] > m_HiAcStreamActivePri) 
						{
							/* set the new highest active priority */
							m_HiAcStreamActivePri = m_AcStreamPriMap[i];
						}
					}
				}
			}
		}

		Unlock();
	} 
	else 
	{
		/* for mbox ping testing, the traffic class is mapped directly as a stream ID,
		* see handling of AR6000_XIOCTL_TRAFFIC_ACTIVITY_CHANGE in ioctl.c */
		eid = (HTC_ENDPOINT_ID)TrafficClass;
	}

	/* notify HTC, this may cause credit distribution changes */
	HTCIndicateActivityChange(m_hHtcTarget, eid, Active);

}



void 
CAR6KMini::BssInfoEventRx(A_UINT8 *datap, int len)
{

    struct ndis_mini_buf_t *skb;
    WMI_BSS_INFO_HDR *bih = (WMI_BSS_INFO_HDR *)datap;


    if (!m_MgmtFilter) {
        return;
    }
    if (((m_MgmtFilter & IEEE80211_FILTER_TYPE_BEACON) &&
        (bih->frameType != BEACON_FTYPE))  ||
        ((m_MgmtFilter & IEEE80211_FILTER_TYPE_PROBE_RESP) &&
        (bih->frameType != PROBERESP_FTYPE)))
    {
        return;
    }

    if ((skb = (ndis_mini_buf_t *)A_NETBUF_ALLOC_RAW(len)) != NULL) 
	{
#if 0
        A_NETBUF_PUT(skb, len);
        A_MEMCPY(A_NETBUF_DATA(skb), datap, len);
        skb->dev = ar->arNetDev;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
        A_MEMCPY(skb_mac_header(skb), A_NETBUF_DATA(skb), 6);
#else
        skb->mac.raw = A_NETBUF_DATA(skb);
#endif
        skb->ip_summed = CHECKSUM_NONE;
        skb->pkt_type = PACKET_OTHERHOST;
        skb->protocol = __constant_htons(0x0019);
        netif_rx(skb);
	
#else
	NDIS_DEBUG_PRINTF(1, "%s() is not ready yet ~~~ #### \r\n",__FUNCTION__);
#endif
	}		

}



/* Init cookie queue */
void
CAR6KMini::CookieInit(void)
{

	A_UINT32    i;

	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() + Enter \r\n", __FUNCTION__);

	m_CookieList = NULL;
	A_MEMZERO(m_Cookie_mem, sizeof(m_Cookie_mem));

	for (i = 0; i < MAX_COOKIE_NUM; i++) 
	{
		FreeCookie(&m_Cookie_mem[i]);
	}
	
}

void
CAR6KMini::CookieCleanUp(void)
{
    /* It is gone .... */
    m_CookieList = NULL;
}



/* Init cookie queue */
void
CAR6KMini::FreeCookie(struct ar_cookie * cookie)
{
	/* Insert first */
	A_ASSERT(cookie != NULL);
	cookie->arc_list_next = m_CookieList;
	m_CookieList = cookie;

}




/* cleanup cookie queue */
struct ar_cookie *
CAR6KMini::AllocCookie(void)
{
    struct ar_cookie   *cookie;

    cookie = m_CookieList;
    if(cookie != NULL)
    {
        m_CookieList = cookie->arc_list_next;
    }

    return cookie;
}


void 
CAR6KMini::TxDataCleanup(void)
{
	/* flush all the data (non-control) streams
	* we only flush packets that are tagged as data, we leave any control packets that
	* were in the TX queues alone */
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() + Enter \r\n", __FUNCTION__);
	HTCFlushEndpoint(m_hHtcTarget, Ac2EndpointID(WMM_AC_BE), AR6K_DATA_PKT_TAG);
	HTCFlushEndpoint(m_hHtcTarget, Ac2EndpointID(WMM_AC_BK), AR6K_DATA_PKT_TAG);
	HTCFlushEndpoint(m_hHtcTarget, Ac2EndpointID(WMM_AC_VI), AR6K_DATA_PKT_TAG);
	HTCFlushEndpoint(m_hHtcTarget, Ac2EndpointID(WMM_AC_VO), AR6K_DATA_PKT_TAG);

}



HTC_ENDPOINT_ID 
CAR6KMini::Ac2EndpointID(BYTE ac)
{
	return m_Ac2EpMapping[(ac)];	
}

void
CAR6KMini::SetAc2EndpointIDMap(BYTE ac, HTC_ENDPOINT_ID ep)
{
	m_Ac2EpMapping[ac] = (ep); 
	m_Ep2AcMapping[ep] = (ac); 

}


// bssid Info to be saved before suspend operation, to avoid
// connection issues when system resume


A_STATUS 
CAR6KMini::SetAR6KPowerStatus(AR6000_WLAN_STATE state)
{
	A_STATUS status = A_OK;
	AR6000_WLAN_STATE oldstate = m_WlanState;

	NDIS_DEBUG_PRINTF(DBG_LEVEL_CAR6K, "%s(%s) Enter \r\n", __FUNCTION__, (state==WLAN_ENABLED)? "enable": "disable");
	NDIS_DEBUG_PRINTF(1, "%s(%s) Enter \r\n", __FUNCTION__, (state==WLAN_ENABLED)? "enable": "disable");
	
	if (m_WMIReady == FALSE || (state!=WLAN_DISABLED && state!=WLAN_ENABLED)) 
	{
		NDIS_DEBUG_PRINTF(DBG_ERR, " %s() Error status \r\n", __FUNCTION__);
	    return A_ERROR;
	}

    if (state == m_WlanState) {
		NDIS_DEBUG_PRINTF(DBG_ERR, " %s()status is samed \r\n", __FUNCTION__);
        return A_OK;
    }

	if (m_bIsDestroyProgress) {
		NDIS_DEBUG_PRINTF(DBG_ERR, " %s()m_bIsDestroyProgress is true. \r\n", __FUNCTION__);
	    return A_ERROR;
	}

	m_WlanState = state;
	
    do 
	{
        if (m_WlanState == WLAN_ENABLED) 
		{
			NDIS_DEBUG_PRINTF(1, "%s() AR6K Module Power On !!!!! \r\n", __FUNCTION__);
           // A_UINT16 fg_start_period = (ar->scParams.fg_start_period==0) ? 1 : ar->scParams.fg_start_period;
            WMI_SET_HOST_SLEEP_MODE_CMD hostSleepMode = { TRUE, FALSE};
			
            /*WMI_REPORT_SLEEP_STATE_EVENT  wmiSleepEvent ; */

            HIFUnMaskInterrupt((HIF_DEVICE*)HTCGetHifDevice(m_hHtcTarget));

    	    /*wmiSleepEvent.sleepState = WMI_REPORT_SLEEP_STATUS_IS_AWAKE; */
			hostSleepMode.awake = TRUE;
            hostSleepMode.asleep = FALSE;
            if ((status=wmi_set_host_sleep_mode_cmd((wmi_t *)m_pWMI, &hostSleepMode)) != A_OK) {
				NDIS_DEBUG_PRINTF(DBG_ERR, " %s() : wmi_set_host_sleep_mode_cmd failed \r\n", __FUNCTION__);
               break;    
            }

			if (m_80211_PowerMode ==Ndis802_11PowerModeCAM) {
				wmi_powermode_cmd((struct wmi_t *)m_pWMI, MAX_PERF_POWER);
			}
			
			
			if( wmi_scanparams_cmd((wmi_t *)m_pWMI, 0, 0, (A_UINT16)m_Config.bkScanPeriod, 100, 110, 0, 1, m_ScanCtrlFlag, 0, 0) != A_OK )
			{
				NDIS_DEBUG_PRINTF(DBG_ERR," %s() Set Scan Param Fail \r\n", __FUNCTION__);
			}
			
		
        }
		else 
		{

            WMI_SET_HOST_SLEEP_MODE_CMD hostSleepMode;


			if (m_Connected) {
				// Flush any pending NDIS packets
				FlushNdisPacketTransmitQueue();
			}

			NDIS_DEBUG_PRINTF(1, " %s() m_Connected(%d),m_ConnectInProgress(%d) \r\n",__FUNCTION__,m_Connected,m_ConnectInProgress);			

            /* Disconnect from the AP and disable foreground scanning */           
            if (m_Connected == TRUE || m_ConnectInProgress == TRUE) 
			{
				NdisResetEvent (&m_SuspendEvent);
				m_WantToBeConnected = FALSE;
				SendWMIDisconnectCommand ();
				NdisWaitEvent (&m_SuspendEvent, 50);
            } 

            if ((status=wmi_scanparams_cmd((wmi_t *)m_pWMI, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0, 0)) != A_OK) {
               break;
            }
			

			if (m_80211_PowerMode == Ndis802_11PowerModeCAM) {
	            wmi_powermode_cmd((wmi_t *)m_pWMI, REC_POWER);
			}

            hostSleepMode.awake = FALSE;
            hostSleepMode.asleep = TRUE;
            if ((status=wmi_set_host_sleep_mode_cmd((wmi_t *)m_pWMI, &hostSleepMode))!=A_OK) {
                break;
            }
			
			TxDataCleanup();
					
            A_MDELAY(250);
            HIFMaskInterrupt((HIF_DEVICE*)HTCGetHifDevice(m_hHtcTarget));

		
        }
    } while (0);
	
    if (status!=A_OK) {
        NDIS_DEBUG_PRINTF(DBG_ERR, "Fail to setup WLAN state %d\n", m_WlanState);
        m_WlanState = oldstate;
    }

	NDIS_DEBUG_PRINTF(1, " %s() : Exit, status = %d \r\n", __FUNCTION__, status);
	return status;
}




