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
#include "hif.h"
#include "wmi_api.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
extern "C" {
#include "bmi.h"
}

#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include <pm.h>
#ifdef WINDOWS_MOBILE
#include <wrlspwr.h>
#include <winreg.h>
#endif
#include "osapi.h"

CAR6KMini::CAR6KMini()
{
	int i = 0;

	m_pHTCTarget   = NULL;
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

}

void
CAR6KMini::TargetAvailableEventHandler(
	HTC_TARGET      *pHTCTarget,
	HIF_DEVICE      *pHIFDevice)
//
//  This function processes an HTC_TARGET_AVAILABLE event indication
//  from the HTC layer.
//
{
	A_STATUS        status;

	m_pHTCTarget = pHTCTarget;
	m_pHIFDevice = pHIFDevice;

	NdisMSleep(1000000);

   	BMIInit();

   	status = BMIGetTargetId(m_pHIFDevice, &m_TargetId);
	if (A_OK != status) {
		goto done;
	}
	// the following code will display the version of firmware and driver
	// also it checks whether version mismatch between drier and firmware
	// incase of version mismatch, this will false the flag and return status failure
	// notification to NDIS
	{
		BYTE  byFirmwareMajor = 0;
		BYTE  byFirmwareMinor = 0;

		WORD wFirmware = m_TargetId >> 24;
		byFirmwareMajor = wFirmware >> 4;
		byFirmwareMinor = wFirmware & 0x0F;

		RETAILMSG (1,(L"FIRMWARE MAJOR VERSION ==> %x\n", byFirmwareMajor));
		RETAILMSG (1,(L"FIRMWARE MINOR VERSION ==> %x\n", byFirmwareMinor));

    }

	if (m_Config.defaultApp) {
		status = StartEndpoints();
		if (A_OK != status) {
			goto done;
		}
	}

	m_tgtReady = true;
done:
	NdisSetEvent(&m_tgtReadyEvent);

	return;
}

static void
AR6KTargetAvailableEventHandler(
	HTC_TARGET      *pHTCTarget, 
	HTC_ENDPOINT_ID  eid,
	HTC_EVENT_ID     evId, 
	HTC_EVENT_INFO  *evInfo, 
	void            *Context)
//
//  This function processes an HTC_TARGET_AVAILABLE event indication
//  from the HTC layer.
//
{
	CAR6KMini *pAdapter = (CAR6KMini *)Context;

	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +AR6KTargetAvailableEventHandler");

	pAdapter->TargetAvailableEventHandler(pHTCTarget,(HIF_DEVICE *)evInfo->buffer);

	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -AR6KTargetAvailableEventHandler");
}

void
CAR6KMini::TargetUnavailableEventHandler(
	HTC_TARGET      *pHTCTarget)
//
//  This function processes an HTC_TARGET_UNAVAILABLE event indication
//  from the HTC layer.
//
{
	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +TargetUnavailableEventHandler");
	StopEndpoints();

	m_pHIFDevice = NULL;
	m_pHTCTarget = NULL;
	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -TargetUnavailableEventHandler");
}

void
AR6KTargetUnavailableEventHandler(
	HTC_TARGET       *pHTCTarget,
	HTC_ENDPOINT_ID   eid,
	HTC_EVENT_ID      evId,
	HTC_EVENT_INFO   *evInfo, 
	void             *Context)
//
//  This function processes an HTC_TARGET_UNAVAILABLE event indication
//  from the HTC layer.
//
{
	CAR6KMini *pAdapter = (CAR6KMini *)Context;

	pAdapter->TargetUnavailableEventHandler(pHTCTarget);
}

void
CAR6KMini::ReceiveWMIControlPacket(
	HTC_EVENT_INFO  *evInfo)
//
//  This function processes data from an HTC_BUFFER_RECEIVED indication
//  on the WMI_CONTROL_MBOX endpoint.
//
{
	ndis_mini_buf_t *pb;

	NDIS_DEBUG_PRINTF(ATH_LOG_TRC,"AR6K: +ReceiveWMIControlPacket");

	pb = (ndis_mini_buf_t *)evInfo->cookie;
	ASSERT(pb);

	if (evInfo->status == A_OK) {
		// Append something to tail of buffer?
		a_netbuf_put(pb, (A_UINT8)(evInfo->actualLength + HTC_HEADER_LEN));

		// Remove HTC_HEADER_LEN bytes from the start of buffer
		a_netbuf_pull(pb, HTC_HEADER_LEN);

		Lock();

		wmi_control_rx((wmi_t *)m_pWMI, pb);

		Unlock();
	} else {
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - ReceiveWMIPacket Error in receiving : status = %x\n", evInfo->status);
	}

	a_netbuf_reset(pb);
    
	if ( A_OK != HTCBufferReceive(m_pHTCTarget, ENDPOINT1, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb)) {
		a_netbuf_free(pb);
	}
    
	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -ReceiveWMIControlPacket");
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
	CAR6KMini *pAdapter = (CAR6KMini *)Context;

	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: AR6KHTCBufferReceivedEventHandler MBOX=%u", eid);

      
	if (evInfo->status == A_ECANCELED) {
		ndis_mini_buf_t *pb = (ndis_mini_buf_t *)evInfo->cookie;
		NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: AR6KHTCBufferReceivedEventHandler freeing buffer: pb = %x \n",(unsigned int)pb);
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

	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +AR6KHTCDataAvailableEventHandler");

	// Provide HTC with some more receive buffers
	// TODO - Impose some maximum cap
	pAdapter->AllocateHTCPacketBuffers(eid, AR6000_MAX_RX_BUFFERS);

	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -AR6KHTCDataAvailableEventHandler");
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

A_STATUS
CAR6KMini::RegisterEndpointEventHandlers(
	HTC_ENDPOINT_ID endPointId)


//
//  Connect the specified endpoint to its event handling functions for
//  Send and Receive events.
//
{
	A_STATUS        status;

	status = HTCEventReg(m_pHTCTarget, endPointId, HTC_BUFFER_RECEIVED, AR6KHTCBufferReceivedEventHandler, this);
	if (A_OK != status)
		goto done;

	status = HTCEventReg(m_pHTCTarget, endPointId, HTC_DATA_AVAILABLE, AR6KHTCDataAvailableEventHandler, this);
	if (A_OK != status)
		goto done;

	status = HTCEventReg(m_pHTCTarget, endPointId, HTC_BUFFER_SENT, AR6KHTCBufferSentEventHandler, this);
	if (A_OK != status)
		goto done;

	// Provide HTC with some initial receive buffers
	AllocateHTCPacketBuffers(endPointId, AR6000_MAX_RX_BUFFERS);

done:
    if (status != A_OK) {
	    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: RegisterEndpointEventHandlers failed, error=%u\n", status);
    }
	return status;
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
	A_STATUS        status = A_ERROR;

	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +StartEndpoints");

	if (NULL == m_pHTCTarget)
		goto done;

#ifdef FLASH_18V
	// Change the flash access time for 1.8V flash to 150ns
	status = BMIWriteSOCRegister(m_pHIFDevice, 
                        0xac004004,
                        0x920100d1);
	if (A_OK != status) {
		goto done;
	}
#endif


	// Send a "BMI_DONE" command to the target, to get it out of
	// BMI mode and into normal operation.
	status = BMIDone(m_pHIFDevice);
	if (A_OK != status) {
		goto done;
	}
	
	if (!m_Config.byPassWmi) {
		NdisResetEvent(&m_WMIReadyEvent);
		m_pWMI = wmi_init(this);
		if (NULL == m_pWMI)
			goto done;
	}

	// Install handlers for HTC Send/Receive Events
	status = RegisterEndpointEventHandlers(ENDPOINT1);
	if (A_OK != status)
		goto done;

	status = RegisterEndpointEventHandlers(ENDPOINT2);
	if (A_OK != status)
		goto done;

#ifdef WMM
	status = RegisterEndpointEventHandlers(ENDPOINT3);
	if (A_OK != status)
		goto done;

	status = RegisterEndpointEventHandlers(ENDPOINT4);
	if (A_OK != status)
		goto done;
#endif //WMM

	status = HTCStart(m_pHTCTarget);
	if (A_OK != status) {
	    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: HTCStart failed, error=%u\n", status);
		goto done;
    }

	m_HTCStarted = true;

	if (!m_Config.byPassWmi) {
		NdisWaitEvent(&m_WMIReadyEvent, AR6K_DEFAULT_MS_TO_WAIT_FOR_WMI_READY);
		if (!m_WMIReady) {
			NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - No WMI_READY event after %u ms, failing initialization\n", AR6K_DEFAULT_MS_TO_WAIT_FOR_WMI_READY);
			status = A_ERROR;
		}
	}

	if (A_OK != status)
		goto done;
    
	status = configTargetParams();

done:
	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: -StartEndpoints status=%u", status);
	return status;
}

void
CAR6KMini::StopEndpoints()
//
//  Undoes the initialization done by StartEndpoints.
//
{
	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +StopEndpoints");
	if (m_HTCStarted) {
		HTCStop(m_pHTCTarget);
		m_HTCStarted = false;
	}

	if (!m_Config.byPassWmi) {
		if (m_pWMI) {
			wmi_shutdown((wmi_t *)m_pWMI);
			m_WMIReady = false;
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

    return A_OK;
}

/* Configure the target from the registry settings only in wmi mode */
A_STATUS
CAR6KMini::configTargetParams()
{
	A_STATUS   wmiStatus = A_OK;
	A_UINT8 powerMode = 0;


	if (!m_Config.byPassWmi) {
		if (m_Config.bkScanEnable) {
			wmiStatus = wmi_scanparams_cmd((wmi_t *)m_pWMI,0,0,(A_UINT16)m_Config.bkScanPeriod,
										70,0,WMI_SHORTSCANRATIO_DEFAULT);
		} else {
			wmiStatus = wmi_scanparams_cmd((wmi_t *)m_pWMI,0,0,0,
										70,0,WMI_SHORTSCANRATIO_DEFAULT);
		}
		if (wmiStatus == A_OK) {
			if (m_Config.discTimeout) {
				wmiStatus = wmi_disctimeout_cmd((wmi_t *)m_pWMI, m_Config.discTimeout);
			}
		}

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

	return wmiStatus;
}

#ifdef WINDOWS_MOBILE
A_STATUS setWiFiStator()
{
	HKEY handle;
	LONG result;
	DWORD data;
	A_STATUS status = A_ERROR;

	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SYSTEM\\STATE\\HARDWARE"),0,0,&handle);
	if (result == ERROR_SUCCESS) {
		data = 0x1; //Start with WLAN module available
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
	BOOL          wirelessState;
#endif
	A_UINT32      powerState;

	// Read the reset state configured thru the registry
	powerState = m_Config.resetPowerState;
        
#ifdef WINDOWS_MOBILE
	// For windows mobile read the flight mode status
	if (GetWirelessState(&wirelessState) == ERROR_SUCCESS) {
		powerState = powerState & (wirelessState ? 1 : 0);   
	}
#endif
	NDIS_DEBUG_PRINTF(ATH_LOG_INF,"Reset Power State = %d \n",powerState);

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

	NDIS_DEBUG_PRINTF(ATH_LOG_TRC,"ResetPowerWorkItem :: Entry\n");

	NdisResetEvent(&m_ResetPowerWorkItemExitedEvent);

	if (!m_Config.byPassWmi) {
		// Dual-Purpose workitem 
		// Indicate disconnect after the MiniportInit complete successfully
		// This is needed for 802.11 miniports
		while (m_Halting == false) {
			NdisMSleep(1000);
			if ((m_InitComplete) && (m_WMIReady)) {
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
			if (driverStateEvent != NULL) {
				WaitForSingleObject(driverStateEvent, INFINITE);
				CloseHandle(driverStateEvent);
			}
				
			// Wait till the WiFi entry is created
			while (m_Halting == false) {
				hr = GetWirelessDevices(&pDevice, 0);
				if (hr == S_OK) {
					if (pDevice) {
						pTD = pDevice;
						// loop through the linked list of devices
						while (pTD) { 								
							if (pTD->DeviceType==POWER_MANAGED) {
								hr=ChangeRadioState(pTD, 0, POWER_POST_SAVE);
								if (hr == S_OK) {
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
				} else {
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
				NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"ResetPowerWorkItem :: Reset power state set failed\n");
			}  else {
				NDIS_DEBUG_PRINTF(ATH_LOG_INF,"ResetPowerWorkItem :: Reset power state set success\n");
			}
		}
	}

	NdisSetEvent(&m_ResetPowerWorkItemExitedEvent);
    
	NDIS_DEBUG_PRINTF(ATH_LOG_TRC,"ResetPowerWorkItem :: Exit\n");

	return;
}

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

	NDIS_DEBUG_PRINTF(ATH_LOG_TRC, "AR6K: +Initialize");

	NdisInitializeEvent(&m_tgtReadyEvent);
	NdisInitializeEvent(&m_WMIReadyEvent);
	NdisInitializeEvent(&m_RxPendingEvent);
	NdisInitializeEvent(&m_RxWorkItemExitedEvent);
	NdisInitializeEvent(&m_tgtStatsEvent);
	NdisInitializeEvent(&m_ResetPowerWorkItemExitedEvent);
    
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

	// Register with HTC
	athStatus = HTCInit();
	if (athStatus != A_OK)
		goto done;
	m_HTCInited = true;

	athStatus = HTCEventReg(NULL, ENDPOINT_UNUSED, HTC_TARGET_AVAILABLE, AR6KTargetAvailableEventHandler, this);
	if (athStatus != A_OK)
		goto done;

	athStatus = HTCEventReg(NULL, ENDPOINT_UNUSED, HTC_TARGET_UNAVAILABLE, AR6KTargetUnavailableEventHandler, this);
	if (athStatus != A_OK)
		goto done;

	NdisWaitEvent(&m_tgtReadyEvent, AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_READY);
	if (!m_tgtReady)
	{
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - No TGT_READY event after %u ms, failing initialization\n", AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_READY);
		athStatus = A_ERROR;
		goto done;
	}

#ifdef WINDOWS_MOBILE
	athStatus = setWiFiStator();
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

	m_Halting = true;
	if (m_Connected) {
		NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
		NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
		m_Connected = FALSE;
	}
    
	// Wakeup the Rx Work Item thread so it can exit
	NdisSetEvent(&m_RxPendingEvent);
	NdisWaitEvent(&m_RxWorkItemExitedEvent, 0);

	// Wakeup the Get Stats thread if any
	NdisSetEvent(&m_tgtStatsEvent);

	NdisWaitEvent(&m_ResetPowerWorkItemExitedEvent, 0);

	NdisFreeEvent(&m_tgtReadyEvent);
	NdisFreeEvent(&m_WMIReadyEvent);
	NdisFreeEvent(&m_RxPendingEvent);
	NdisFreeEvent(&m_RxWorkItemExitedEvent);
	NdisFreeEvent(&m_tgtStatsEvent);
	NdisFreeEvent(&m_ResetPowerWorkItemExitedEvent);

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

	if (m_HTCInited) {
		HTCShutDown(m_pHTCTarget);
        m_HTCInited = false;
	}
    
	CMiniport::Halt();

	if (m_pTransmitBufferArray)
		A_FREE(m_pTransmitBufferArray);

	if (m_RxPacketPool)
		NdisFreePacketPool(m_RxPacketPool);

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
