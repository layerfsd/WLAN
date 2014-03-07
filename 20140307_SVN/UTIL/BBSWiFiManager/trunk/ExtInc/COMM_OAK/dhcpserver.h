//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//


//  Include Files

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////// DHCP Packet Support ///////////////////////////////

#define CHADDR_LEN	16
#define MAC_LEN     6
#define SNAME_LEN	64
#define FILE_LEN	128
#define DHCP_MAX_OPTIONS_LEN	312

typedef BYTE IPAddr_t[4];

typedef struct DhcpPkt {
	
	BYTE   Op;
#define DHCP_OP_BOOTREQUEST	1
#define DHCP_OP_BOOTREPLY   2

	BYTE   Htype;
#define DHCP_HTYPE_10MB_ETHERNET	1

	BYTE   Hlen; // 6 for Ethernet MAC address length
	BYTE   Hops; // always 0

	// Transaction ID used by client to match replies with requests.
	BYTE            Xid[4];

	// Filled in by client, seconds elapsed since client
    // began address acquisition or renewal process
	BYTE            Secs[2];

	// Client sets BROADCAST flag if client requires
	// broadcast reply.
	BYTE            Flags[2];
	IPAddr_t        Ciaddr;
	IPAddr_t        Yiaddr;
	IPAddr_t        Siaddr;
	IPAddr_t        Giaddr;
	unsigned char	aChaddr[CHADDR_LEN];
	unsigned char	aSname[SNAME_LEN];
	unsigned char	aFile[FILE_LEN];
	//
	// Options follow, but per RFC2131:
	// The first four octets of the 'options' field of the DHCP message
    // contain the (decimal) values 99, 130, 83 and 99, respectively 
	//
	BYTE            magicCookie[4];

	//
	// Options is variable length.
	// Each option consists of:
	//		Type (1 byte)
	//		Length (1 byte) optional
	//		Value (Length bytes) optional
	// The option list is terminated by an option of Type 255
	//
	unsigned char	aOptions[DHCP_MAX_OPTIONS_LEN];

} DhcpPacket, *PDhcpPacket;

#define DHCP_MAX_PACKET_SIZE sizeof(DhcpPacket)

//	DHCP Message Types (after option DHCP_MSG_TYPE_OP)
#define DHCPDISCOVER	1
#define DHCPOFFER		2	
#define DHCPREQUEST		3
#define DHCPDECLINE		4
#define DHCPACK			5
#define DHCPNACK		6
#define DHCPRELEASE		7
#define DHCPINFORM		8
#define DHCP_MAX_MSG_TYPE	8

//	Option Definitions
#define DHCP_PAD_OP				0
#define DHCP_END_OP				255

#define DHCP_SUBNET_OP			1
#define DHCP_ROUTER_OP			3
#define DHCP_DNS_OP				6
#define DHCP_HOST_NAME_OP		12
#define DHCP_DOMAIN_NAME_OP		15
#define DHCP_NBT_SRVR_OP		44
#define DHCP_NBT_NODE_TYPE_OP	46
#define DHCP_REQ_IP_OP			50
#define DHCP_IP_LEASE_OP		51
#define DHCP_OVERLOAD_OP		52
#define DHCP_MSG_TYPE_OP		53
#define DHCP_SERVER_ID_OP		54
#define DHCP_PARAMETER_REQ_OP	55
#define DHCP_MESSAGE_OP			56
#define DHCP_T1_VALUE_OP		58
#define DHCP_T2_VALUE_OP		59
#define DHCP_CLIENT_ID_OP		61
#define DHCP_MSFT_AUTOCONF      251

//	Unused/unsupported Options
#define DHCP_TIME_OFFSET_OP			2
#define DHCP_TIME_SRVR_OP			4
#define DHCP_NAME_SRVR_OP			5
#define DHCP_LOG_SRVR_OP			7
#define DHCP_COOKIE_SRVR_OP			8
#define DHCP_LPR_SRVR_OP			9
#define DHCP_IMPRESS_SRVR_OP		10
#define DHCP_RES_LOC_SRVR_OP		11	// Resource location server option
#define DHCP_BOOTFILE_SIZE_OP		13
#define DHCP_MERIT_DUMP_FILE_OP		14
#define DHCP_SWAP_SRVR_OP			16
#define DHCP_ROOT_PATH_OP			17
#define DHCP_EXT_PATH_OP			18
#define DHCP_IP_FORWARDING_OP		19
#define DHCP_NONLOCAL_SRCROUTE_OP	20
#define DHCP_POLICY_FILTER_OP		21
#define DHCP_MAX_DGRAM_REASSEMBLE_SIZE_OP	22
#define DHCP_IP_TTL_OP				23
#define DHCP_PATH_MTU_TO_OP			24
#define DHCP_PATH_MTU_PLATEAU_OP	25
#define DHCP_INTERFACE_MTU_OP		26
#define DHCP_ALL_SUBNETS_LOCAL_OP	27
#define DHCP_BCAST_ADDR_OP			28
#define DHCP_MASK_DISCOVERY_OP		29
#define DHCP_MASK_SUPPLIER_OP		30
#define DHCP_ROUTER_DISCOVERY_OP	31
#define DHCP_ROUTER_SOLICIT_ADDR_OP	32
#define DHCP_STATIC_ROUTE_OP		33
#define DHCP_TRAILER_ENCAPSULATE_OP	34
#define DHCP_ARP_CACHE_TO_OP			35
#define DHCP_ETHERNET_ENCAPSULATE_OP	36
#define DHCP_TCP_TTL_OP					37
#define DHCP_TCP_KEEPALIVE_INTVL_OP		38
#define DHCP_TCP_KEEPALIVE_GARBAGE_OP	39
#define DHCP_NIS_DOMAIN_OP				40
#define DHCP_NETWORK_INFO_SRVRS_OP	41
#define DHCP_NETWORK_TIME_SRVRS_OP	42
#define DHCP_VENDOR_SPECIFIC_OP		43
#define DHCP_NBDD_OP				45
#define DHCP_NBT_SCOPE_OP			47
#define DHCP_XWINDOW_FONT_SRVR_OP	48
#define DHCP_XWINDOW_DISPLAY_MGR_OP	49
#define DHCP_NISP_DOMAIN_OP			64
#define DHCP_NISP_SRVRS_OP			65
#define DHCP_MOBILE_IP_AGENT_OP		68
#define DHCP_SMTP_SRVR_OP			69
#define DHCP_POP3_SRVR_OP			70
#define DHCP_NNTP_SRVR_OP			71
#define DHCP_WWW_SRVR_OP			72
#define DHCP_DEFAULT_FINGER_OP		73
#define DHCP_DEFAULT_IRC_SRVR_OP	74	//	default Internet relay chat server
#define DHCP_STREETTALK_SRVR_OP		75
#define DHCP_STDA_SRVR_OP			76
#define DHCP_TFTP_SRVR_NAME_OP		66
#define DHCP_BOOTFILE_NAME_OP		67
#define DHCP_MAX_DHCP_MSG_OP		57
#define DHCP_VENDOR_CLASS_ID_OP		60

//
// Data structure used to decode DHCP option values
//
typedef struct
{
	
	BYTE   MessageType;  // DHCPDISCOVER, DHCPOFFER, etc

	PBYTE  RequestedIPAddr;

	PBYTE  ServerID;

	BYTE   ClientIDType;
	BYTE   cbClientID;
	PBYTE  ClientID;

	BYTE   cbParameterRequestList;
	PBYTE  ParameterRequestList;
} DhcpOptions, *PDhcpOptions;

typedef void (*pfnDhcpSendPacketHandler)(
	IN  PVOID              pContext,
	IN  DWORD              SrcIpAddr,
	IN  DWORD              DestIpAddr,
	IN  PBYTE              pDhcpPacket,
	IN  DWORD              cbDhcpPacket);

//
// Data structure describing DHCP server context info
//
typedef struct
{
	// IP Address of this DHCP server
	DWORD   IPAddrDhcpServer;

	// The IP address pool from which the server can allocate leases
	DWORD	BaseIPAddress;
	DWORD   NumIPAddresses;

	DWORD   RouterIPAddress;
	DWORD   SubnetMask;

	// The time for which we grant a lease
	DWORD   dwLeaseTimeSecs;

#define MAX_DNS_SERVERS	4
	DWORD   DNSIPAddress[MAX_DNS_SERVERS];
	DWORD   dwNumDNSServers;
	PSTR    szDomainName;

	// Context pointer to pass to callback functions
	PVOID   Context;

	// Space to reserve for IP/UDP/whatever headers to
	// precede the DHCP payload.

	DWORD   cbSendHeaderReserve;

	// Function to call to send a DHCP packet.
	pfnDhcpSendPacketHandler SendPacketHandler;

#define MAX_WINS_SERVERS	4
	DWORD   WINSIPAddress[MAX_WINS_SERVERS];
	DWORD   dwNumWINSServers;

    //  Only respond to DHCP Requests from aDHCPClientMACAddr
    BOOL    fOnlyServeDHCPToDHCPClientMAC;
	unsigned char	aDHCPClientMAC[MAC_LEN];

} DhcpServerSession, *PDhcpServerSession;

typedef BOOL (*pfnDhcpPacketHandler)(
	IN  PDhcpServerSession pSession,
	IN  PDhcpPacket        pDiscover,
	IN  PDhcpOptions       pOptions);


#define DHCP_DEFAULT_SERVER_IP_ADDR  0xC0A80001
#define DHCP_DEFAULT_BASE_IP_ADDR    0xC0A80002
#define DHCP_DEFAULT_NUM_IP_ADDR     1
#define DHCP_DEFAULT_ROUTER_IP_ADDR  DHCP_DEFAULT_SERVER_IP_ADDR
#define DHCP_DEFAULT_SUBNET_MASK     0xFFFF0000
#define DHCP_DEFAULT_LEASE_TIME_SECS (60 * 60 * 24 * 30) // 30 days



void
DHCPServerFreePacket(
	IN  PDhcpServerSession pSession,
	IN  PBYTE              pPacket);

BOOL
DHCPServerProcessPacket(
	IN  PDhcpServerSession pSession,
	IN  PBYTE              pPacket,
	IN  DWORD              cbPacket);

PDhcpServerSession
DHCPServerSessionNew(
	IN PVOID                    pContext,
    IN pfnDhcpSendPacketHandler SendPacketHandler,
	IN DWORD                    cbSendHeaderReserve);

void
DHCPServerSessionDelete(
	IN PDhcpServerSession pSession);

#ifdef __cplusplus
}
#endif
