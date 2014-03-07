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


    DHCP handling code


--*/

#include "system.h"
#include "athdefs.h"
#include "u_htc.h"
#include "u_wmi.h"
#include "dhcp.h"

A_UINT8 g_BroadCastMAC[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
A_UINT8 g_MyMAC[6];

A_UINT32 g_DHCPDiscoverCount = 0xAA55;

void SetMacForDHCP(A_UINT8 *pMac)
{
    A_MEMCPY(g_MyMAC, pMac,sizeof(g_MyMAC));
}

typedef struct _CHECKSUM_REGION {
    A_UINT8                 *pData;
    int                     Length;
    struct _CHECKSUM_REGION *pNext;
} CHECKSUM_REGION;

A_UINT16 CalcCheckSum(CHECKSUM_REGION *pRegion)
{
    A_UINT32 sum = 0;
    A_UINT32 carry;
    A_UINT16 wordData;
    int      length;
    A_UINT8  *pData;

    while (pRegion) {

        length = pRegion->Length;
        pData = pRegion->pData;

        while (length >= sizeof(A_UINT16)) {
                /* assemble word */
            wordData = (A_UINT16)pData[0] | (((A_UINT16)pData[1]) << 8);
            sum += wordData;
            pData += sizeof(A_UINT16);
            length -= sizeof(A_UINT16);
        }

        if (length) {
                /* add in any odd byte values , this auto pads */
            sum += (A_UINT16)*pData;
        }
            /* move to next region */
        pRegion = pRegion->pNext;
    }

    while (sum & 0xFFFF0000) {
        carry = sum >> 16;
        sum &= 0x0000FFFF;
        sum += carry;
    }

    return ~((A_UINT16)sum);
}

/* assumes caller has set up WMI and provides a large enough packet
 * Build and Send a DHCP discover packet */
A_STATUS SendDhcpDiscover(A_UINT8 *pTransmitBuffer)
{
    AR6K_DATA_FRAME *pFrame = (AR6K_DATA_FRAME *)pTransmitBuffer;
    DHCP_PACKET     *pPacket;
    UDP_PSEUDO_HDR   pseudoUDPHdr;
    CHECKSUM_REGION  regions[2];
    A_UINT16         totalIPLength;
    A_UINT16         optionsLength;
    A_UINT8          *pOption;

    pPacket = (DHCP_PACKET *)&pFrame->PayloadData;

    A_MEMZERO(pPacket,sizeof(DHCP_PACKET));
    A_MEMZERO(regions,sizeof(regions));
    A_MEMZERO(&pseudoUDPHdr,sizeof(pseudoUDPHdr));

    pPacket->DHCPMessage.OpCode = DHCP_DISCOVER;
    pPacket->DHCPMessage.HWAddressType = 0x1;  /* ethernet */
    pPacket->DHCPMessage.HWAddressLength = 6;
    pPacket->DHCPMessage.Hops = 0;
    pPacket->DHCPMessage.TransactionID = g_DHCPDiscoverCount++; /* something unique */
    pPacket->DHCPMessage.Seconds = HostToNetworkOrder_16((A_UINT16)A_GET_SECONDS_TICK());
    pPacket->DHCPMessage.Flags = HostToNetworkOrder_16(0);
    pPacket->DHCPMessage.ClientIPAddress = 0;
    pPacket->DHCPMessage.YourIPAddress = 0;
    pPacket->DHCPMessage.ServerIPAddress = 0;
    pPacket->DHCPMessage.RelayIPAddress = 0;
    A_MEMCPY(pPacket->DHCPMessage.ClientHWAddress,g_MyMAC,sizeof(g_MyMAC));
        /* we have options, set the magic cookie */
    pPacket->DHCPMessage.MagicCookie[0] = 0x63;
    pPacket->DHCPMessage.MagicCookie[1] = 0x82;
    pPacket->DHCPMessage.MagicCookie[2] = 0x53;
    pPacket->DHCPMessage.MagicCookie[3] = 0x63;

    pOption = &pPacket->DHCPOptions[0];
    optionsLength = 0;
    pOption[optionsLength] = 53;  /* option code */
    optionsLength++;
    pOption[optionsLength]= 1;   /* length */
    optionsLength++;
    pOption[optionsLength] = DHCP_DISCOVER;
    optionsLength++;
    pOption[optionsLength] = 0xff;
    optionsLength++;

        /* setup IP header */
    pPacket->IPHdr.Version_HdrLength = 0x45 ;     /* [Version (4 bits)][Header Length (4 bits)] */
    pPacket->IPHdr.TOS = 0;
    totalIPLength = sizeof(pPacket->IPHdr) + sizeof(pPacket->UDPHdr) + sizeof(pPacket->DHCPMessage) +
                    optionsLength;

    pPacket->IPHdr.TotalLength = HostToNetworkOrder_16(totalIPLength);
    pPacket->IPHdr.Identification = g_DHCPDiscoverCount++;
    pPacket->IPHdr.Flags_FragmentOffset = 0;
    pPacket->IPHdr.TTL = 32;
    pPacket->IPHdr.Protocol = IP_PROTOCOL_UDP;
    pPacket->IPHdr.HeaderCheckSum;
    pPacket->IPHdr.SourceIP = 0;
    pPacket->IPHdr.DestIP = 0xFFFFFFFF;
        /* checksum header */
    regions[0].pData = (A_UINT8 *)&pPacket->IPHdr;
    regions[0].Length = sizeof(pPacket->IPHdr);
    pPacket->IPHdr.HeaderCheckSum = CalcCheckSum(&regions[0]);


        /* setup pseudo header for checksum calculations */
    pseudoUDPHdr.DestIP = pPacket->IPHdr.DestIP;
    pseudoUDPHdr.SourceIP = pPacket->IPHdr.SourceIP;
    pseudoUDPHdr.Protocol = pPacket->IPHdr.Protocol;
    pseudoUDPHdr.Length = HostToNetworkOrder_16(sizeof(pPacket->UDPHdr) +
                                                sizeof(pPacket->DHCPMessage)+
                                                optionsLength);

        /* setup UDP header */
    pPacket->UDPHdr.SourcePort = HostToNetworkOrder_16(DHCP_CLIENT_PORT);
    pPacket->UDPHdr.DestPort = HostToNetworkOrder_16(DHCP_SERVER_PORT);
    pPacket->UDPHdr.Length = pseudoUDPHdr.Length;
        /* checksum includes the pseudo-header and the real UDP header plus data */
    regions[0].pData = (A_UINT8 *)&pseudoUDPHdr;
    regions[0].Length = sizeof(pseudoUDPHdr);
    regions[0].pNext = &regions[1];
    regions[1].pData = (A_UINT8 *)&pPacket->UDPHdr;
    regions[1].Length = sizeof(pPacket->UDPHdr) + sizeof(pPacket->DHCPMessage) + optionsLength;
    regions[1].pNext = NULL;
    pPacket->UDPHdr.CheckSum = CalcCheckSum(&regions[0]);

    if (0 == pPacket->UDPHdr.CheckSum) {
        pPacket->UDPHdr.CheckSum = 0xFFFF;
    }

        /* set destination as broadcast */
    SET_MAC_DEST(pFrame,g_BroadCastMAC);

    //DebugDumpBytes((A_UCHAR *)pPacket, totalIPLength, "DHCP Discover");

    return wmi_send_data(pFrame, 0x0800, totalIPLength);
}

A_BOOL CheckDHCPOffer(A_UINT8 *pReceivedIPPacket, int PacketLength)
{
    A_BOOL           offerFound = FALSE;
    A_BOOL           badPacket = TRUE;
    IP_HDR           *pIPHdr;
    int              IPhdrLength;
    int              UDPLength;
    CHECKSUM_REGION  regions[2];
    UDP_HDR          *pUdpHdr;
    UDP_PSEUDO_HDR   pseudoUDPHdr;
    DHCP_MSG         *pDhcpHdr;

    A_MEMZERO(regions,sizeof(regions));
    A_MEMZERO(&pseudoUDPHdr,sizeof(pseudoUDPHdr));

    pIPHdr = (IP_HDR *)pReceivedIPPacket;

    do {

        if (PacketLength < sizeof(IP_HDR)) {
            /* bad length */
            break;
        }

        if ((pIPHdr->Version_HdrLength & 0xF0) != 0x40) {
            /* not IPV4 */
            badPacket = FALSE;
            break;
        }

        IPhdrLength = (pIPHdr->Version_HdrLength & 0x0F) * sizeof(A_UINT32);

        if (IPhdrLength > PacketLength) {
                /* bad IP header, calculated length goes beyond the buffer */
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Bad IP header length \n"));
            break;
        }

            /* checksum the IP header */
        regions[0].pData = (A_UINT8 *)pIPHdr;
        regions[0].Length = IPhdrLength;
        if (CalcCheckSum(&regions[0]) != 0x00) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Bad IP checksum \n"));
            break;
        }

        /* IP header is good */

        if (pIPHdr->Protocol != IP_PROTOCOL_UDP) {
                /* not UDP */
            badPacket = FALSE;
            break;
        }

        UDPLength = PacketLength - IPhdrLength;
        pUdpHdr = (UDP_HDR *)(pReceivedIPPacket + IPhdrLength);

        if (UDPLength < sizeof(UDP_HDR)) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("No room left for UDP header \n"));
            break;
        }

        if ((int)NetworkOrderToHost_16(pUdpHdr->Length) > UDPLength) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,
                        ("Bad UDP length from header: %d, room:%d \n",
                        NetworkOrderToHost_16(pUdpHdr->Length), UDPLength));
            break;
        }

            /* use the length out of the UDP header */
        UDPLength = NetworkOrderToHost_16(pUdpHdr->Length);

            /* assemble the pseudo header for calculating UDP checksums */
        pseudoUDPHdr.DestIP = pIPHdr->DestIP;
        pseudoUDPHdr.SourceIP = pIPHdr->SourceIP;
        pseudoUDPHdr.Protocol = pIPHdr->Protocol;
        pseudoUDPHdr.Length = HostToNetworkOrder_16(UDPLength);

        regions[0].pData = (A_UINT8 *)&pseudoUDPHdr;
        regions[0].Length = sizeof(pseudoUDPHdr);
        regions[0].pNext = &regions[1];
        regions[1].pData = (A_UINT8 *)pUdpHdr;
        regions[1].Length = UDPLength;
        regions[1].pNext = NULL;

        if (CalcCheckSum(&regions[0]) != 0x00) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Bad UDP checksum \n"));
            break;
        }

        if (!((NetworkOrderToHost_16(pUdpHdr->SourcePort) == DHCP_SERVER_PORT) &&
              (NetworkOrderToHost_16(pUdpHdr->DestPort) == DHCP_CLIENT_PORT))) {
                /* not ours */
            badPacket = FALSE;
            break;
        }

        if ((UDPLength - sizeof(UDP_HDR)) < sizeof(DHCP_MSG)) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Bad DHCP message length \n"));
            break;
        }

        badPacket = FALSE;

        pDhcpHdr = (DHCP_MSG *)(pReceivedIPPacket + IPhdrLength + sizeof(UDP_HDR));

        if (pDhcpHdr->OpCode != DHCP_OFFER) {
            /* not what we are looking for */
            break;
        }

        if (A_MEMCMP(pDhcpHdr->ClientHWAddress,g_MyMAC,sizeof(g_MyMAC)) != 0) {
            /* not our request */
            break;
        }

        {
            A_UINT8 addrServer[4];
            A_UINT8 addrYours[4];

            A_MEMCPY(addrServer,&pIPHdr->SourceIP, sizeof(A_UINT32));
            A_MEMCPY(addrYours, &pDhcpHdr->YourIPAddress, sizeof(A_UINT32));

            AR_DEBUG_PRINTF(ATH_DEBUG_ANY,
                    ("GOT DHCP OFFER! Server: %d.%d.%d.%d, Offered: %d.%d.%d.%d\n",
                    addrServer[0],addrServer[1],addrServer[2],addrServer[3],
                    addrYours[0],addrYours[1],addrYours[2],addrYours[3]));

        }

        offerFound = TRUE;

    } while (FALSE);

    if (badPacket) {
        DebugDumpBytes(pReceivedIPPacket, PacketLength, "Bad IP Frame ");
    }

    return offerFound;
}
