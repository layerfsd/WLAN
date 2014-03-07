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

 DHCP definitions


--*/

#ifndef _DHCP_H_
#define _DHCP_H_

#include "athstartpack.h"

typedef PREPACK struct _IP_HDR {
    A_UINT8     Version_HdrLength;     /* [Version (4 bits)][Header Length (4 bits)] */
    A_UINT8     TOS;
    A_UINT16    TotalLength;
    A_UINT16    Identification;
    A_UINT16    Flags_FragmentOffset;  /* [Flags (3bits)][Fragment Offset 13 bits] */
    A_UINT8     TTL;
    A_UINT8     Protocol;
    A_UINT16    HeaderCheckSum;
    A_UINT32    SourceIP;
    A_UINT32    DestIP;
} IP_HDR;

#define IP_PROTOCOL_UDP     17

typedef PREPACK struct _UDP_HDR {
    A_UINT16 SourcePort;
    A_UINT16 DestPort;
    A_UINT16 Length;
    A_UINT16 CheckSum;
} POSTPACK UDP_HDR;

typedef PREPACK struct _UDP_PSEUDO_HDR {
    A_UINT32 SourceIP;
    A_UINT32 DestIP;
    A_UINT8  StuffZero;
    A_UINT8  Protocol;
    A_UINT16 Length;
} POSTPACK UDP_PSEUDO_HDR;

#define     DHCP_DISCOVER   1
#define     DHCP_OFFER      2
#define     DHCP_REQUEST    3
#define     DHCP_DECLINE    4
#define     DHCP_ACK        5
#define     DHCP_NAK        6
#define     DHCP_RELEASE    7

#define DHCP_SERVER_PORT    67
#define DHCP_CLIENT_PORT    68

typedef PREPACK struct _DHCP_MSG {

    A_UINT8     OpCode;
    A_UINT8     HWAddressType;
    A_UINT8     HWAddressLength;
    A_UINT8     Hops;
    A_UINT32    TransactionID;
    A_UINT16    Seconds;
    A_UINT16    Flags;
    A_UINT32    ClientIPAddress;       
    A_UINT32    YourIPAddress;        
    A_UINT32    ServerIPAddress;       
    A_UINT32    RelayIPAddress;        
    A_UINT16    ClientHWAddress[8];     
    A_UINT8     BootP_Data[192];
    A_UINT8     MagicCookie[4];
} POSTPACK DHCP_MSG;
  
typedef PREPACK struct _DHCP_OPTION_HDR {

   A_UINT8  Option;
   A_UINT8  Length;
} POSTPACK DHCP_OPTION_HDR;
  
typedef PREPACK struct _DHCP_PACKET {
    IP_HDR              IPHdr;
    UDP_HDR             UDPHdr;
    DHCP_MSG            DHCPMessage;
    A_UINT8             DHCPOptions[256];
}POSTPACK DHCP_PACKET;

#include "athendpack.h"

void     SetMacForDHCP(A_UINT8 *pMac);
A_STATUS SendDhcpDiscover(A_UINT8 *pTransmitBuffer);
A_BOOL   CheckDHCPOffer(A_UINT8 *pReceivedIPPacket, int PacketLength);

#endif
