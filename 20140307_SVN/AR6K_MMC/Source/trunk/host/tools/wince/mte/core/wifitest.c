/*++
Copyright (c) 2008 Atheros Communications Inc.
All rights reserved.


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


 Main test entry point


--*/

#include "system.h"
#include "wifitest.h"
#include "u_hif.h"
#include "u_bmi.h"
#include "u_config.h"
#include "u_htc.h"
#include "htc_services.h"
#include "u_wmi.h"
#include <wmi.h>
#include "dhcp.h"

#define BOARD_XTAL_FREQ 26000000

#define MAX_MESSAGE_BUFFER_SIZE (1664 + HTC_HDR_LENGTH)

typedef enum _TEST_OPERATION_STATE {
    TEST_STATE_WAIT_WMI_READY = 0,
    TEST_STATE_WAIT_WMI_CONNECT,
    TEST_WAIT_DHCP_REPLY,
    TEST_WAIT_WMI_STATS,
    TEST_GEN_FAILURE,
    TEST_END_SUCCESS  
}TEST_OPERATION_STATE;

/* verbose */
#define MEDIUM_VERBOSE_MASK  (DBG_ZONE_INIT | DBG_ZONE_HIF | DBG_ZONE_HTC | DBG_ZONE_WMI)
#define NORMAL_VERBOSE_MASK  (DBG_ZONE_INIT | DBG_ZONE_HIF)

DWORD g_WiFiDbgZone = DBG_ZONE_ERR | 
                      NORMAL_VERBOSE_MASK
                      //MEDIUM_VERBOSE_MASK
                      ;

typedef struct _AR6K_WIFI_MTE {
    HTC_ENDPOINT_ID         ControlEp;
    HTC_ENDPOINT_ID         DataEp;
    A_UINT8                 Macaddr[ATH_MAC_LEN];
    A_BOOL                  WMIReady; 
    HTC_HANDLE              htcHandle;
    A_UINT32                MteStatus;
    A_BOOL                  APConnected;
    A_BOOL                  ReceivedAPDisconnect;
    A_BOOL                  ReceivedStats;
    int                     SSIDLength;
    A_UCHAR                 SSID[WMI_MAX_SSID_LEN];
    int                     WiFiChannel;
    HIF_DEVICE              *HifDevice;
    struct bmi_target_info  TargetInfo;                                                    
    TEST_OPERATION_STATE    TestState;
    A_UINT32                RSSI;
    A_BOOL                  GotDhcpOffer;
    A_UINT8                 ReceiveBuffer[MAX_MESSAGE_BUFFER_SIZE];
    A_UINT8                 *pReceivePayloadStart;  
    A_UINT8                 TransmitBuffer[MAX_MESSAGE_BUFFER_SIZE];
}AR6K_WIFI_MTE;

static AR6K_WIFI_MTE g_WifiMTE;


void InitializeMTE(void) 
{
    A_MEMZERO(&g_WifiMTE, sizeof(g_WifiMTE));
    g_WifiMTE.ControlEp = ENDPOINT_MAX;
    g_WifiMTE.DataEp = ENDPOINT_MAX;
    g_WifiMTE.pReceivePayloadStart = &g_WifiMTE.ReceiveBuffer[HTC_HDR_LENGTH]; 
}

static const DWORD g_ChannelLookUp[MAX_WIFI_CHANNELS + 1] = {
    0,    /* place holder for zero index */
    2412,
    2417,
    2422,
    2427,
    2432,
    2437,
    2442,
    2447,
    2452,
    2457,
    2462,
    2456,
    2461,
    2484
};
         


/**** WMI Callbacks *******/  
void ar6000_ready_event(A_UINT8 *MacAddress, A_UINT8 phyCap)
{
    A_ASSERT(!g_WifiMTE.WMIReady);
    A_MEMCPY(g_WifiMTE.Macaddr, MacAddress,ATH_MAC_LEN);
    g_WifiMTE.WMIReady = TRUE;
    SetMacForDHCP(g_WifiMTE.Macaddr);
    DBG_LOG_PRINT(DBG_ZONE_INIT, ("WMI Ready! MAC Address: %x:%x:%x:%x:%x:%x\r\n",
        g_WifiMTE.Macaddr[0],g_WifiMTE.Macaddr[1],g_WifiMTE.Macaddr[2],g_WifiMTE.Macaddr[3],g_WifiMTE.Macaddr[4],g_WifiMTE.Macaddr[5])); 

}

void ar6000_connect_event(A_UINT16 channel,
                          A_UINT8 *bssid, A_UINT16 listenInterval,
                          A_UINT16 beaconInterval, NETWORK_TYPE networkType,
                          A_UINT8 beaconIeLen, A_UINT8 assocReqLen,
                          A_UINT8 assocRespLen,A_UINT8 *assocInfo)
{
    DBG_LOG_PRINT(DBG_ZONE_INIT, 
        ("AP Connected (bssid %x:%x:%x:%x:%x:%x) , Channel: %d, beacon interval: %d \r\n",
             bssid[0], bssid[1], bssid[2],
             bssid[3], bssid[4], bssid[5],
             channel, beaconInterval));
    g_WifiMTE.APConnected = TRUE;
}
                          
void ar6000_disconnect_event(A_UINT8 reason,
                             A_UINT8 *bssid, A_UINT8 assocRespLen,
                             A_UINT8 *assocInfo, A_UINT16 protocolReasonStatus)
{
    A_CHAR *pReason = "UNKNOWN";
    
    g_WifiMTE.ReceivedAPDisconnect = TRUE;
    
    switch(reason) {
        case NO_NETWORK_AVAIL :
            pReason = "NO_NETWORK_AVAIL";
            break;
        case LOST_LINK :          
            pReason = "LOST_LINK  ";
            break;
        case DISCONNECT_CMD :
            pReason = "DISCONNECT_CMD  ";
            break;
        case ASSOC_FAILED :
            pReason = "ASSOC_FAILED  ";
            break;
        case NO_RESOURCES_AVAIL :
            pReason = "NO_RESOURCES_AVAIL  ";
            break;
        case CSERV_DISCONNECT :
            pReason = "CSERV_DISCONNECT  ";
            break;
        case INVALID_PROFILE : 
            pReason = "INVALID_PROFILE  ";
            break; 
        case AUTH_FAILED :
            pReason = "AUTH_FAILED  ";
            break; 
        case BSS_DISCONNECTED :
            pReason = "BSS_DISCONNECTED - OKAY";
                /* this one is not a real disconnect */
            g_WifiMTE.ReceivedAPDisconnect = FALSE;
            break;
        default:
            A_ASSERT(FALSE);
            break;
    }
    
    DBG_LOG_PRINT(DBG_ZONE_INIT, ("WMI Disconnect, Reason: %d (%s) \r\n", reason, pReason));  
}

void ar6000_regDomain_event(A_UINT32 regCode)
{    
    
}

void ar6000_targetStats_event(WMI_TARGET_STATS *pStats)
{
    g_WifiMTE.RSSI = pStats->cservStats.cs_rssi;
    
    DBG_LOG_PRINT(DBG_ZONE_INIT, ("Received Target Stats : RSSI : %d \r\n", g_WifiMTE.RSSI));  
    
    g_WifiMTE.ReceivedStats = TRUE;
    
}

/*********************************************************/

#define CONNECTION_TEST_EXPIRATION   5  /* 5 seconds */


/* status look up based on current state */  
A_UINT32 GetMTEStatus()
{
    switch (g_WifiMTE.TestState) {
        case TEST_STATE_WAIT_WMI_READY :
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("status: MTE_WIFI_STATUS_BOOT_FAILED \r\n"));   
            return MTE_WIFI_STATUS_BOOT_FAILED;   
        case TEST_STATE_WAIT_WMI_CONNECT :
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("status: MTE_WIFI_STATUS_ASSOC_FAILED \r\n")); 
            return MTE_WIFI_STATUS_ASSOC_FAILED;
        case TEST_WAIT_DHCP_REPLY :
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("status: MTE_WIFI_STATUS_DHCP_FAILED \r\n")); 
            return MTE_WIFI_STATUS_DHCP_FAILED;
        case TEST_WAIT_WMI_STATS:
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("status: MTE_WIFI_STATUS_WMI_FAILED \r\n")); 
            return MTE_WIFI_STATUS_WMI_FAILED;
        case TEST_END_SUCCESS:
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("status: MTE_WIFI_STATUS_SUCCESS \r\n")); 
            return MTE_WIFI_STATUS_SUCCESS;
        case TEST_GEN_FAILURE:
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("status: MTE_WIFI_STATUS_FAILED \r\n")); 
            return MTE_WIFI_STATUS_FAILED;
        default:
            A_ASSERT(FALSE);
            break;    
    }
    return MTE_WIFI_STATUS_FAILED;
}
                    
A_STATUS RunConnectionTest()
{
    A_STATUS        status = A_OK;
    HTC_ENDPOINT_ID endpointID;
    int             length;
    DWORD           startTime;
    DWORD           currentTime;
    DWORD           dhcpRequestTimeStart;
    int             dhcpAttempt = 0;
        
        /* set initial status and state */
    g_WifiMTE.TestState = TEST_STATE_WAIT_WMI_READY;
    
    startTime = A_GET_SECONDS_TICK();
    
    while (A_SUCCESS(status) && (g_WifiMTE.TestState != TEST_GEN_FAILURE)) {

        currentTime = A_GET_SECONDS_TICK();
        A_ASSERT(startTime <= currentTime);
        
        if ((currentTime - startTime) >= CONNECTION_TEST_EXPIRATION) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("Time Expired! Current State: %d \r\n", g_WifiMTE.TestState));   
            status = A_ERROR;
            break;    
        }
        
        endpointID = ENDPOINT_MAX;
        
           /* poll for a message */
        status = HTCGetMessage(g_WifiMTE.htcHandle,                
                               &endpointID, 
                               g_WifiMTE.pReceivePayloadStart,  /* reuse transmit buffer */
                               MAX_MESSAGE_BUFFER_SIZE,
                               &length,
                               0); /* no timeout */
                               
        if (A_FAILED(status)) {
            break;    
        }            
                
        if (endpointID == ENDPOINT_MAX) {
                /* polling was successful, but no message was retrieved */
            continue;    
        }
        
        if (endpointID == g_WifiMTE.ControlEp) {
                /* process control message */
            status = wmi_control_rx(g_WifiMTE.pReceivePayloadStart, length);
            
        } else if (endpointID == g_WifiMTE.DataEp) {
           A_UINT8  *pNetFrameStart; 
           int      networkFrameLength;
           A_UINT16 etherType;
           
                /* process data message */
                
                /* get the frame information, the payload is the WMI message */
            wmi_get_network_frame_info(g_WifiMTE.pReceivePayloadStart, 
                                       length, 
                                       &pNetFrameStart,
                                       &networkFrameLength,
                                       &etherType);
            
            if ((networkFrameLength != 0) && (etherType == 0x0800)) {  
                    /* an IP packet arrived */
                if (!g_WifiMTE.GotDhcpOffer) {
                        /* run it through DHCP */
                    if (CheckDHCPOffer(pNetFrameStart,networkFrameLength)) {
                        g_WifiMTE.GotDhcpOffer = TRUE;              
                    }
                }
            }                     
                                    
        } else {
            A_ASSERT(FALSE);
            status = A_EPROTO;
            break;    
        }
        
        if (A_FAILED(status)) {
            break;    
        }   
        
        /* run through state machine */
        
        switch (g_WifiMTE.TestState) {
            case TEST_STATE_WAIT_WMI_READY :
                if (g_WifiMTE.WMIReady) {
                    DBG_LOG_PRINT(DBG_ZONE_INIT, ("WMI is ready\n"));
                        /* next state is to wait for connection */
                    g_WifiMTE.TestState = TEST_STATE_WAIT_WMI_CONNECT;
                    
                    status = ar6000_set_wmi_protocol_ver(g_WifiMTE.HifDevice, 
                                                         g_WifiMTE.TargetInfo.target_type, 
                                                         WMI_PROTOCOL_VERSION); 
                    
                    if (A_FAILED(status)) {
                        A_ASSERT(FALSE);
                        break;    
                    }          
        
                        /* make sure we are fully awake */
                    DBG_LOG_PRINT(DBG_ZONE_INIT, ("Changing power mode\n"));
                    status = wmi_powermode_cmd(MAX_PERF_POWER);
                    
                    if (A_FAILED(status)) {
                        break;    
                    }   
                    DBG_LOG_PRINT(DBG_ZONE_INIT, ("Sending connect\n"));
                        /* connect to SSID */
                    status = wmi_connect_cmd(INFRA_NETWORK,
                                             OPEN_AUTH, 
                                             NONE_AUTH,
                                             NONE_CRYPT, 0,
                                             NONE_CRYPT, 0,
                                             g_WifiMTE.SSIDLength, 
                                             g_WifiMTE.SSID,
                                             NULL, 
                                             g_WifiMTE.WiFiChannel, 
                                             0);
                }
                break;
            case TEST_STATE_WAIT_WMI_CONNECT :
                if (g_WifiMTE.APConnected) {
                        /* next state */
                    g_WifiMTE.TestState = TEST_WAIT_DHCP_REPLY;   
                        /* capture start time to retry DHCP request */
                    dhcpRequestTimeStart = A_GET_SECONDS_TICK();
                    DBG_LOG_PRINT(DBG_ZONE_INIT, ("Sending DHCP Discover... \r\n"));   
                    status = SendDhcpDiscover(g_WifiMTE.TransmitBuffer);
                    if (A_FAILED(status)) {
                        break;   
                    }
                }            
                break;
            
            case TEST_WAIT_DHCP_REPLY :
                if ((A_GET_SECONDS_TICK() - dhcpRequestTimeStart) > 1) {
                     dhcpAttempt++;
                        /* time expired, resend DHCP again */
                    DBG_LOG_PRINT(DBG_ZONE_INIT, ("Retrying DHCP request.. re-attempt : %d \r\n", dhcpAttempt));   
                    dhcpRequestTimeStart = A_GET_SECONDS_TICK();
                       /* resend again */
                    status = SendDhcpDiscover(g_WifiMTE.TransmitBuffer);
                    if (A_FAILED(status)) {
                        break;   
                    }
                }
                
                if (g_WifiMTE.GotDhcpOffer) {
                        /* issue get status command and wait for response */
                    g_WifiMTE.TestState = TEST_WAIT_WMI_STATS;
                    status = wmi_get_stats_cmd();
                }
            
                break;
            
            case TEST_WAIT_WMI_STATS:
                if (g_WifiMTE.ReceivedStats) {                    
                    g_WifiMTE.TestState = TEST_END_SUCCESS;
                }       
                break;           
            
            case TEST_END_SUCCESS:
                
                break;
            
            default:
                g_WifiMTE.TestState = TEST_GEN_FAILURE;
                A_ASSERT(FALSE);
                break;    
        }
        
        if (g_WifiMTE.TestState == TEST_END_SUCCESS) {
                /* all done */
            break;    
        }
    
    }
    
    if (g_WifiMTE.APConnected) {
            /* if we were connected, issue the disconnect command to disconnect from the AP */
        wmi_disconnect_cmd();
            /* before exiting make sure the target sends out the disconnect */    
        A_MDELAY(200);
    }
    
    g_WifiMTE.MteStatus = GetMTEStatus();   
        
    return status;
}

A_BOOL RunAR6kTest(A_UINT32 Channel, A_CHAR *pSSID, MTE_WIFI_TEST_STATUS *pTestStatus)
{         
    AR6K_FIRMWARE_INFO  firmware;
    A_STATUS            status = A_OK;
    
    A_MEMZERO(&firmware,sizeof(firmware));
    
    InitializeMTE();
    
    if (!GetFirmwareInfo(&firmware)) {
        DBG_LOG_PRINT(DBG_ZONE_ERR, ("Target Firmware missing!! Aborting.. \r\n"));   
        return FALSE;    
    }
    
    g_WifiMTE.SSIDLength = strlen(pSSID);
    A_ASSERT(g_WifiMTE.SSIDLength <= 32);
    A_MEMCPY(g_WifiMTE.SSID,pSSID,g_WifiMTE.SSIDLength);    
    
    A_ASSERT(Channel <= MAX_WIFI_CHANNELS);
    A_ASSERT(Channel > 0);
        /* look up the channel hint (in Mhz) */
    g_WifiMTE.WiFiChannel = g_ChannelLookUp[Channel];
    
    DBG_LOG_PRINT(DBG_ZONE_INIT, 
            ("Ar6k WiFi MTE Test Start..(SSID: %s , channel:%d) \r\n", g_WifiMTE.SSID, g_WifiMTE.WiFiChannel));
           
    do {
           
            /* init test status */
        g_WifiMTE.MteStatus = MTE_WIFI_STATUS_SDIO_INIT_FAILED;
                
        if (A_FAILED(HIFInit())) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** HIFInit Failed!!! \r\n"));
            break;    
        }
        
        BMIInit();

            /* all failures are setup failures this point forward */
        g_WifiMTE.MteStatus = MTE_WIFI_STATUS_SETUP_FAILED;
            
        g_WifiMTE.HifDevice = HIFGetDevice();
        
        DBG_LOG_PRINT(DBG_ZONE_ERR, ("Getting Target ID... \r\n"));
        status = BMIGetTargetInfo(g_WifiMTE.HifDevice, 
                                  &g_WifiMTE.TargetInfo);
                                        
        if (A_FAILED(status)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** Failed to Get Target Information \r\n"));
            break;    
        }
        
        DBG_LOG_PRINT(DBG_ZONE_INIT, ("TARGET_TYPE:0x%X  TARGET_VER : 0x%X \r\n",
               g_WifiMTE.TargetInfo.target_type, g_WifiMTE.TargetInfo.target_ver));        
        
        status = ar6000_prepare_target(g_WifiMTE.HifDevice,
                                       g_WifiMTE.TargetInfo.target_type,
                                       g_WifiMTE.TargetInfo.target_ver);
        
        if (A_FAILED(status)) {            
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** Failed to prepare target \r\n"));
            break;    
        }   
        
        status = ar6000_configure_clock (g_WifiMTE.HifDevice,
                                         g_WifiMTE.TargetInfo.target_type,
                                         g_WifiMTE.TargetInfo.target_ver,
                                         BOARD_XTAL_FREQ);
        
        if (A_FAILED(status)) {            
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** Failed to configure target XTAL \r\n"));
            break;    
        }   
                                 
        DBG_LOG_PRINT(DBG_ZONE_INIT, ("Download Firmware ... (firmware length: %d, %s) (datapatch len: %d) \r\n",
               firmware.AthwlanLength, 
               firmware.AthwlanCompressed ? "COMPRESSED" : "UNCOMPRESSED",
               firmware.DataPatchLength));    
               
        status = DownloadFirmware(g_WifiMTE.HifDevice,
                                  g_WifiMTE.TargetInfo.target_type,
                                  g_WifiMTE.TargetInfo.target_ver,
                                  firmware.pAthwlan,
                                  firmware.AthwlanLength,
                                  firmware.AthwlanCompressed,
                                  firmware.pDataPatch,
                                  firmware.DataPatchLength);
        
        if (A_FAILED(status)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** Failed to download firmware \r\n"));
            break;    
        }   
        
        DBG_LOG_PRINT(DBG_ZONE_INIT, ("Firmware Download Complete.. \r\n"));
        
        ar6000_enable_target_console(g_WifiMTE.HifDevice,g_WifiMTE.TargetInfo.target_type);
            
        status = ar6000_set_htc_params(g_WifiMTE.HifDevice,
                                       g_WifiMTE.TargetInfo.target_type,
                                       0,
                                       0);
                                       
        if (A_FAILED(status)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** Failed to set HTC Params \r\n"));
            break;    
        } 
        
        DBG_LOG_PRINT(DBG_ZONE_INIT, ("Transferring EEPROM settings .. \r\n"));
        
        status = eeprom_ar6000_transfer(g_WifiMTE.HifDevice, g_WifiMTE.TargetInfo.target_type);
       
        if (A_FAILED(status)) {            
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** Failed to transfer EEPROM settings \r\n"));
            break;    
        }    
                                   
        DBG_LOG_PRINT(DBG_ZONE_INIT, ("Starting Target .. \r\n"));      
               
        status = BMIDone(g_WifiMTE.HifDevice);       
        
        if (A_FAILED(status)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** Target failed to start \r\n"));
            break;    
        }           
        
            /* from here on out, all mte failures are boot failures */
        g_WifiMTE.MteStatus = MTE_WIFI_STATUS_BOOT_FAILED;
        
        DBG_LOG_PRINT(DBG_ZONE_INIT, ("Connecting HTC .. \r\n"));      
                
            /* connect HTC layer */
        g_WifiMTE.htcHandle =  HTCConnect(g_WifiMTE.HifDevice);
        
        if (g_WifiMTE.htcHandle == NULL) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** HTC connection failed \r\n"));    
            break;    
        }
            
        status = HTCConnectService(g_WifiMTE.htcHandle, 
                                   WMI_CONTROL_SVC,
                                   &g_WifiMTE.ControlEp);
        
        if (A_FAILED(status)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** Failed to connect to WMI control service \r\n"));
            break;    
        }           
        
        status = HTCConnectService(g_WifiMTE.htcHandle, 
                                   WMI_DATA_BE_SVC,
                                   &g_WifiMTE.DataEp);
        
        if (A_FAILED(status)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** Failed to connect to WMI best effort data service \r\n"));
            break;    
        }      
                              
        DBG_LOG_PRINT(DBG_ZONE_INIT, ("HTC is connected (wmi_ctrl:%d, data:%d) ... \r\n",
            g_WifiMTE.ControlEp, g_WifiMTE.DataEp));    
        
        status = HTCStart(g_WifiMTE.htcHandle);
        
        if (A_FAILED(status)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** failed to start HTC \r\n"));
            break;    
        }     
        
        status = WMIInit(g_WifiMTE.htcHandle,g_WifiMTE.ControlEp,g_WifiMTE.DataEp);
        
        if (A_FAILED(status)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR, ("** failed to init WMI \r\n"));
            break;    
        }    
                
        status = RunConnectionTest();
                      
    } while (FALSE);
    
    pTestStatus->StatusCode = g_WifiMTE.MteStatus;
    pTestStatus->RSSI = g_WifiMTE.RSSI;    
    
    return (A_SUCCESS(status));
}
