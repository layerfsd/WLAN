/*
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 * 
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
 *
 * simplified WMI declarations and prototypes
 * 
 */

#ifndef U_WMI_H_
#define U_WMI_H_
#include <wmi.h>


#include "athstartpack.h"

#define ETHERNET_MAC_ADDRESS_LENGTH    6

typedef PREPACK struct ETHERNET_MAC_HEADER
{
    BYTE            DestMACAddress[ETHERNET_MAC_ADDRESS_LENGTH];
    BYTE            SrcMACAddress[ETHERNET_MAC_ADDRESS_LENGTH];
    USHORT          Length;
} POSTPACK ETHERNET_MAC_HEADER, *PETHERNET_MAC_HEADER;

typedef PREPACK struct SNAP_HEADER
{
    BYTE            dsap;
    BYTE            ssap;
    BYTE            cntl;
    BYTE            orgCode[3];
    A_UINT16        Type;
} POSTPACK SNAP_HEADER, *PSNAP_HEADER;

typedef PREPACK struct _AR6K_DATA_FRAME {
    HTC_FRAME_HDR            HTCFrameHeader;
    //
    // Remaining portion of the structure is the WMI_DATA to be transmitted,
    // which begins with a WMI_DATA_HDR but is variable length.
    //
    WMI_DATA_HDR             WMIDataHeader;

    ETHERNET_MAC_HEADER      MACHeader;

    SNAP_HEADER              SNAPHeader;

    //
    // Variable length payload (e.g. ARP packet, IP packet) follows
    //
    BYTE             PayloadData[1];
} POSTPACK AR6K_DATA_FRAME;

#define SET_MAC_SRC(pPacket,MAC) \
    A_MEMCPY(&(pPacket)->MACHeader.SrcMACAddress,(MAC),ETHERNET_MAC_ADDRESS_LENGTH)
    
#define SET_MAC_DEST(pPacket,MAC) \
    A_MEMCPY(&(pPacket)->MACHeader.DestMACAddress,(MAC),ETHERNET_MAC_ADDRESS_LENGTH)
          
#include "athendpack.h"

/***** WMI Apis ************************************************/

A_STATUS WMIInit(HTC_HANDLE      htcHandle, 
                 HTC_ENDPOINT_ID WmiControlEndpoint,
                 HTC_ENDPOINT_ID WmiDataEndpoint);

A_STATUS wmi_control_rx(A_UINT8 *pBuffer, int Length);

A_STATUS wmi_connect_cmd(NETWORK_TYPE    netType,
                         DOT11_AUTH_MODE dot11AuthMode, 
                         AUTH_MODE       authMode,
                         CRYPTO_TYPE     pairwiseCrypto, 
                         A_UINT8         pairwiseCryptoLen,
                         CRYPTO_TYPE     groupCrypto, 
                         A_UINT8         groupCryptoLen,
                         int             ssidLength, 
                         A_UCHAR         *ssid,
                         A_UINT8         *bssid, 
                         A_UINT16        channel, 
                         A_UINT32        ctrl_flags);

A_STATUS wmi_scanparams_cmd(A_UINT16 fg_start_sec,
                            A_UINT16 fg_end_sec, 
                            A_UINT16 bg_sec,
                            A_UINT16 minact_chdw_msec, 
                            A_UINT16 maxact_chdw_msec,
                            A_UINT16 pas_chdw_msec,
                            A_UINT8  shScanRatio, 
                            A_UINT8  scanCtrlFlags,
                            A_UINT32 max_dfsch_act_time,
                            A_UINT16 maxact_scan_per_ssid);


A_STATUS wmi_powermode_cmd(A_UINT8 powerMode);

A_STATUS wmi_get_stats_cmd(void);

A_STATUS wmi_disconnect_cmd(void);

    /* send data packet 
     * caller must set destination mac address in the frame AND set the ethernet Type */         
A_STATUS wmi_send_data(AR6K_DATA_FRAME *pFrame, A_UINT16 EtherType, int PayloadLength);

    /* get network frame information from a received wmi data packet */
void wmi_get_network_frame_info(A_UINT8         *pWMIDataPacket,  
                                int             Length, 
                                A_UINT8         **pNetworkBufferStart,
                                int             *pNetworkFrameLength,
                                A_UINT16        *pEtherType);
                                   
/******* WMI event Callbacks into the test application *********/
extern void ar6000_ready_event(A_UINT8 *MacAddress, A_UINT8 phyCap);



void ar6000_connect_event(A_UINT16 channel,
                          A_UINT8 *bssid, A_UINT16 listenInterval,
                          A_UINT16 beaconInterval, NETWORK_TYPE networkType,
                          A_UINT8 beaconIeLen, A_UINT8 assocReqLen,
                          A_UINT8 assocRespLen,A_UINT8 *assocInfo);
                          
void ar6000_disconnect_event(A_UINT8 reason,
                             A_UINT8 *bssid, A_UINT8 assocRespLen,
                             A_UINT8 *assocInfo, A_UINT16 protocolReasonStatus);

void ar6000_regDomain_event(A_UINT32 regCode);

void ar6000_targetStats_event(WMI_TARGET_STATS *pStats);
                             
#endif /*U_WMI_H_*/
