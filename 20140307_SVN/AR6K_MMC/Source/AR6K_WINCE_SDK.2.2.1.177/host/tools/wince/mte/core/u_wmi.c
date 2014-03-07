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


 Simplified, single threaded (micro) implementation of WMI (for bootloaders)


--*/

#include "system.h"
#include "athdefs.h"
#include "a_types.h"
#include "u_htc.h"
#include "u_wmi.h"
#include "wmi.h"

static                  int cmdRecvNum = 0;
static A_BOOL           g_WMIloggingReq = FALSE;
static A_UINT8          g_bssid[ATH_MAC_LEN];
static A_UINT8          g_MACAddress[ATH_MAC_LEN];
static HTC_HANDLE       g_HTCHandle = NULL;
static HTC_ENDPOINT_ID  g_WMIControlEp;
static HTC_ENDPOINT_ID  g_WMIDataEp;

#define MAX_WMI_SEND_MESSAGE_BUFFER_SIZE (256 + HTC_HDR_LENGTH + sizeof(WMI_CMD_HDR))
static A_UCHAR g_WMISendMessageBuffer[MAX_WMI_SEND_MESSAGE_BUFFER_SIZE];
A_UCHAR     *g_pWMISendMessagePayloadStart = &g_WMISendMessageBuffer[HTC_HDR_LENGTH + sizeof(WMI_CMD_HDR)]; 

A_STATUS WMIInit(HTC_HANDLE      htcHandle, 
                 HTC_ENDPOINT_ID WmiControlEndpoint,
                 HTC_ENDPOINT_ID WmiDataEndpoint)
{           
    cmdRecvNum = 0;
    g_HTCHandle = htcHandle;
    g_WMIControlEp = WmiControlEndpoint;
    g_WMIDataEp = WmiDataEndpoint;
    return A_OK;
}

A_STATUS wmi_control_rx(A_UINT8 *pBuffer, int Length)
{
    WMI_CMD_HDR *cmd;
    A_UINT16 id;
    A_UINT8 *datap;
    A_UINT32 len;
    A_STATUS status = A_OK;
    A_CHAR *pWmiDumpStr = "WMI message payload";

    if (Length < sizeof(WMI_CMD_HDR)) { 
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, 
               ("WMI: invalid length: %d \n",Length));
        return A_ERROR;
    }

    cmd = (WMI_CMD_HDR *)pBuffer;
    id = cmd->commandId;
    datap = pBuffer + sizeof(WMI_CMD_HDR);
    len = Length - sizeof(WMI_CMD_HDR);
    
    AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("---- WMI recv, MsgNo %d, Event ID: 0x%X (%d) ---------------------------\n", 
            cmdRecvNum, id , id));
            
    cmdRecvNum++;
    
    switch (id) {
   
        case (WMI_READY_EVENTID):
            {
                WMI_READY_EVENT *ev = (WMI_READY_EVENT *)datap;
                AR_DEBUG_PRINTF(ATH_DEBUG_WMI,("WMI_READY_EVENTID\n"));
                if (len < sizeof(WMI_READY_EVENT)) {
                    return A_EINVAL;
                }
                A_MEMCPY(g_MACAddress, ev->macaddr, ATH_MAC_LEN);          
                ar6000_ready_event(ev->macaddr, ev->phyCapability);

            }
            break;
     
        case (WMI_CONNECT_EVENTID):
            {
                WMI_CONNECT_EVENT *ev;
                
                AR_DEBUG_PRINTF(ATH_DEBUG_WMI,("WMI_CONNECT_EVENTID \n"));
                
                if (len < sizeof(WMI_CONNECT_EVENT)) {
                    return A_EINVAL;
                }
                ev = (WMI_CONNECT_EVENT *)datap;
                            
                A_MEMCPY(g_bssid, ev->bssid, ATH_MAC_LEN);
            
                ar6000_connect_event(ev->channel, ev->bssid,
                                     ev->listenInterval, ev->beaconInterval,
                                     ev->networkType, ev->beaconIeLen,
                                     ev->assocReqLen, ev->assocRespLen,
                                     ev->assocInfo);
            }                         
            break;
     
        case (WMI_DISCONNECT_EVENTID):
            {
                WMI_DISCONNECT_EVENT *ev;
            
                if (len < sizeof(WMI_DISCONNECT_EVENT)) {
                    return A_EINVAL;
                }
            
                ev = (WMI_DISCONNECT_EVENT *)datap;
            
                A_MEMZERO(g_bssid, ATH_MAC_LEN);
            
                ar6000_disconnect_event(ev->disconnectReason, ev->bssid,
                                        ev->assocRespLen, ev->assocInfo, ev->protocolReasonStatus);
            }
            break;
       
        case (WMI_REGDOMAIN_EVENTID):  
            {
                WMI_REG_DOMAIN_EVENT *ev;

                AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("WMI_REGDOMAIN_EVENTID\n"));      
                
                if (len < sizeof(*ev)) {
                    return A_EINVAL;
                }
                ev = (WMI_REG_DOMAIN_EVENT *)datap;
            
                ar6000_regDomain_event(ev->regDomain);
            }            
            
            break;
     
        case (WMI_EXTENSION_EVENTID):
            {
                A_UINT16 extId = datap[0] | ((A_UINT16)datap[1]) << 8;
                
                datap += sizeof(WMIX_CMD_HDR);
                len -= sizeof(WMIX_CMD_HDR);
                
                AR_DEBUG_PRINTF(ATH_DEBUG_WMI, 
                    ("WMI_EXTENSION_EVENTID - id: 0x%X (%d) length:%d \n",extId, extId, len));
                pWmiDumpStr = "WMI Extended Message payload";
            }
            break;
        
        case (WMI_REPORT_STATISTICS_EVENTID) :
            {
                WMI_TARGET_STATS *reply;

                AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("WMI_EXTENSION_EVENTID\n"));     
                                
                if (len < sizeof(*reply)) {
                    return A_EINVAL;
                }
                
                reply = (WMI_TARGET_STATS *)datap;
        
                ar6000_targetStats_event(reply);   
            }
                     
        default:
            AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("UNHANDLED WMI EVENT : 0x%X (%d) \n",id,id));
            break;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_WMI, 
      ("------------------------------------------------------------------------------------\n"));
        
    if (g_WMIloggingReq) {
        DebugDumpBytes(datap, len, pWmiDumpStr);  
    }
    
    return status;
}

static A_STATUS wmi_cmd_send(A_UINT8 *pBuffer, int Length, WMI_COMMAND_ID cmdId)
{
    WMI_CMD_HDR *cHdr;

    A_ASSERT(pBuffer != NULL);

    pBuffer -= sizeof(WMI_CMD_HDR); /* caller always provides headroom */
    Length += sizeof(WMI_CMD_HDR);
    cHdr = (WMI_CMD_HDR *)pBuffer;
    cHdr->commandId = cmdId;

    AR_DEBUG_PRINTF(ATH_DEBUG_WMI, ("WMI Send Command : 0x%X (%d) len:%d \n",cmdId,cmdId, Length));
    
    if (g_WMIloggingReq) {
        DebugDumpBytes(pBuffer, Length, "WMI Send Buffer Dump");  
    }
    
    return HTCSendMsg(g_HTCHandle, 
                      g_WMIControlEp, 
                      pBuffer, 
                      Length);

}

/* Send a "simple" wmi command -- one with no arguments */
static A_STATUS wmi_simple_cmd(unsigned int cmdid)
{
    A_UINT8         *pBuffer = g_pWMISendMessagePayloadStart;
    
    return wmi_cmd_send(pBuffer, 0, cmdid);
}


A_STATUS
wmi_connect_cmd(NETWORK_TYPE netType,
                DOT11_AUTH_MODE dot11AuthMode, AUTH_MODE authMode,
                CRYPTO_TYPE pairwiseCrypto, A_UINT8 pairwiseCryptoLen,
                CRYPTO_TYPE groupCrypto, A_UINT8 groupCryptoLen,
                int ssidLength, A_UCHAR *ssid,
                A_UINT8 *bssid, A_UINT16 channel, A_UINT32 ctrl_flags)
{
    A_UINT8         *pBuffer = g_pWMISendMessagePayloadStart;
    WMI_CONNECT_CMD *cc;

    if ((pairwiseCrypto == NONE_CRYPT) && (groupCrypto != NONE_CRYPT)) {
        return A_EINVAL;
    }
    if ((pairwiseCrypto != NONE_CRYPT) && (groupCrypto == NONE_CRYPT)) {
        return A_EINVAL;
    }

    cc = (WMI_CONNECT_CMD *)pBuffer;
    A_MEMZERO(cc, sizeof(*cc));

    if (ssidLength)
    {
        A_MEMCPY(cc->ssid, ssid, ssidLength);
    }

    cc->ssidLength          = ssidLength;
    cc->networkType         = netType;
    cc->dot11AuthMode       = dot11AuthMode;
    cc->authMode            = authMode;
    cc->pairwiseCryptoType  = pairwiseCrypto;
    cc->pairwiseCryptoLen   = pairwiseCryptoLen;
    cc->groupCryptoType     = groupCrypto;
    cc->groupCryptoLen      = groupCryptoLen;
    cc->channel             = channel;
    cc->ctrl_flags          = ctrl_flags;

    if (bssid != NULL) {
        A_MEMCPY(cc->bssid, bssid, ATH_MAC_LEN);
    }

    return wmi_cmd_send(pBuffer, sizeof(WMI_CONNECT_CMD), WMI_CONNECT_CMDID);
}

A_STATUS
wmi_scanparams_cmd(A_UINT16 fg_start_sec,
                   A_UINT16 fg_end_sec, 
                   A_UINT16 bg_sec,
                   A_UINT16 minact_chdw_msec, 
                   A_UINT16 maxact_chdw_msec,
                   A_UINT16 pas_chdw_msec,
                   A_UINT8 shScanRatio, 
                   A_UINT8 scanCtrlFlags,
                   A_UINT32 max_dfsch_act_time,

                   A_UINT16 maxact_scan_per_ssid)
{
    A_UINT8             *pBuffer = g_pWMISendMessagePayloadStart;
    WMI_SCAN_PARAMS_CMD *sc;

    sc = (WMI_SCAN_PARAMS_CMD *)pBuffer;
    A_MEMZERO(sc, sizeof(*sc));
    sc->fg_start_period  = fg_start_sec;
    sc->fg_end_period    = fg_end_sec;
    sc->bg_period        = bg_sec;
    sc->minact_chdwell_time = minact_chdw_msec;
    sc->maxact_chdwell_time = maxact_chdw_msec;
    sc->pas_chdwell_time = pas_chdw_msec;
    sc->shortScanRatio   = shScanRatio;
    sc->scanCtrlFlags    = scanCtrlFlags;
    sc->max_dfsch_act_time = max_dfsch_act_time;
    sc->maxact_scan_per_ssid = maxact_scan_per_ssid;

    return wmi_cmd_send(pBuffer, sizeof(WMI_SCAN_PARAMS_CMD), WMI_SET_SCAN_PARAMS_CMDID);
}

A_STATUS wmi_powermode_cmd(A_UINT8 powerMode)
{
    A_UINT8             *pBuffer = g_pWMISendMessagePayloadStart;
    WMI_POWER_MODE_CMD  *cmd;

    cmd = (WMI_POWER_MODE_CMD *)pBuffer;
    A_MEMZERO(cmd, sizeof(*cmd));
    cmd->powerMode = powerMode;
    
    return wmi_cmd_send(pBuffer, sizeof(WMI_POWER_MODE_CMD), WMI_SET_POWER_MODE_CMDID);
}

A_STATUS wmi_get_stats_cmd(void)
{
    return wmi_simple_cmd(WMI_GET_STATISTICS_CMDID);
}

A_STATUS wmi_disconnect_cmd(void)
{
    return wmi_simple_cmd(WMI_DISCONNECT_CMDID);
}



//  The format of a packet sent to the Wireless Module looks like:
//      WMI_DATA_HDR:
//          UINT8     reserved (always 0)
//          UINT8     msgType = 0 (data)
//      802_3_HDR:
//          UINT8[6]  dstMac
//          UINT8[6]  srcMac
//          UINT8[2]  length
//      SNAP_HDR:
//          UINT8     DSAP = 0xAA
//          UINT8     SSAP = 0xAA
//          UINT8     CNTL = 0x03
//          UINT8[3]  CODE = {0x00 0x00 0x00)
//          UINT8[2]  EthType (e.g. 0x08 00 for IP packet)
//      PAYLOAD:
//          UINT8[length] packet payload that follows 802_3 header (e.g. IP packet)

#define DATA_FRAME_HEADERS_LENGTH  (sizeof(WMI_DATA_HDR) +          \
                                    sizeof(ETHERNET_MAC_HEADER) +   \
                                    sizeof(SNAP_HEADER))

A_STATUS wmi_send_data(AR6K_DATA_FRAME *pFrame, A_UINT16 EtherType, int PayloadLength)
{ 
        /* set WMI data header */
    WMI_DATA_HDR_SET_MSG_TYPE(&(pFrame->WMIDataHeader), DATA_MSGTYPE);
    
    /* note, caller must set destination MAC field */
      
    SET_MAC_SRC(pFrame,g_MACAddress); 
    pFrame->SNAPHeader.dsap = 0xAA;
    pFrame->SNAPHeader.ssap = 0xAA;
    pFrame->SNAPHeader.cntl = 0x03;
    pFrame->SNAPHeader.orgCode[0] = 0;
    pFrame->SNAPHeader.orgCode[1] = 0;
    pFrame->SNAPHeader.orgCode[2] = 0;
    pFrame->SNAPHeader.Type = HostToNetworkOrder_16(EtherType);
    pFrame->MACHeader.Length = HostToNetworkOrder_16(PayloadLength);
    
    /*DebugDumpBytes((A_UINT8 *)&(pFrame->WMIDataHeader), 
                     PayloadLength + DATA_FRAME_HEADERS_LENGTH, 
                     "WMI Data Packet");  */
    
    return HTCSendMsg(g_HTCHandle, 
                      g_WMIDataEp, 
                      (A_UINT8 *)&pFrame->WMIDataHeader,  /* start at the WMI header, we have HTC head room here */
                      PayloadLength + DATA_FRAME_HEADERS_LENGTH);
}

void wmi_get_network_frame_info(A_UINT8         *pWMIDataPacket, 
                                int             Length, 
                                A_UINT8         **pNetworkBufferStart,
                                int             *pNetworkFrameLength,
                                A_UINT16        *pEtherType)
{
    WMI_DATA_HDR  *pHdr;
    SNAP_HEADER   *pSnapHeader;
    
    *pNetworkFrameLength = 0;
    
    if (Length > DATA_FRAME_HEADERS_LENGTH) {
        
        pHdr = (WMI_DATA_HDR  *)pWMIDataPacket;
        
        if (WMI_DATA_HDR_IS_MSG_TYPE(pHdr, DATA_MSGTYPE)) {
            
            /*DebugDumpBytes(pWMIDataPacket + sizeof(WMI_DATA_HDR), 
                           (A_UINT16)(Length - sizeof(WMI_DATA_HDR)), 
                           "Received Network Frame");*/
                           
            pSnapHeader = (SNAP_HEADER *)(pWMIDataPacket + sizeof(WMI_DATA_HDR) + sizeof(ETHERNET_MAC_HEADER));
            *pNetworkBufferStart = pWMIDataPacket + DATA_FRAME_HEADERS_LENGTH;
            *pNetworkFrameLength = Length - DATA_FRAME_HEADERS_LENGTH;
            *pEtherType = NetworkOrderToHost_16(pSnapHeader->Type);
        }
        
    }
    
}


