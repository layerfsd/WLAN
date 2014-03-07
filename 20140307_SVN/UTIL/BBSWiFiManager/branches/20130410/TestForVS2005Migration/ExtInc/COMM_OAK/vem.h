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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:

    vem.h

Abstract:

    The main header for the generic software ethernet miniport driver.

Notes:

--*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////
//
// NDIS utility data types and functions


typedef struct
{
	NDIS_SPIN_LOCK  Lock;

	NDIS_HANDLE     hBufferPool;

	PNDIS_BUFFER	freeList;
	UINT            packetsTotal;
	UINT            packetsInUse;

	// Only used for fixed type buffer pools,
	// where each buffer will point to the same data.
	PVOID          VirtualAddress;
	UINT           Length;

} NdisBufferPool, *PNdisBufferPool;


PBYTE
ndisBufferGetExactlyNBytes(
	IN  PNDIS_BUFFER pBuffer,
    IN  UINT         offset,
	IN  UINT         cbBytes,
	__out_bcount(cbBytes) OUT PBYTE        pScratch);

PBYTE
ndisBufferGetUpToNBytes(
	IN  PNDIS_BUFFER pBuffer,
    IN  UINT         offset,
	IN  UINT         cbMaxBytes,
	__out_bcount(cbMaxBytes) OUT PBYTE        pScratch,
	OUT PUINT        pBytesRead);

BOOL
ndisBufferGetBEUshort(
	IN  PNDIS_BUFFER pBuffer,
    IN  DWORD        offset,
	OUT PUSHORT      pUshort);

PNDIS_BUFFER
ndisBufferPoolGetBuffer(
	IN OUT PNdisBufferPool pPool,
	IN     PVOID           VirtualAddress,
	IN     UINT            Length);

VOID
ndisBufferPoolFreeBuffer(
	IN OUT PNdisBufferPool pPool,
	IN     PNDIS_BUFFER    pBuffer);

NDIS_STATUS
ndisBufferPoolAllocate(
	PNdisBufferPool *ppPool);

VOID
ndisBufferPoolFree(
	PNdisBufferPool pPool);

PNDIS_BUFFER
ndisBufferPoolGetBufferFixed(
	IN OUT PNdisBufferPool pPool);

NDIS_STATUS
ndisBufferPoolAllocateFixed(
	OUT PNdisBufferPool *ppPool,
	IN  PVOID            VirtualAddress,
	IN  UINT             Length);



typedef struct _NdisBufferListEntry
{
	struct _NdisBufferListEntry *Next;
	BYTE                         Data[1];
} NdisBufferListEntry, *PNdisBufferListEntry;

typedef struct
{
	NDIS_SPIN_LOCK  Lock;
	PNdisBufferListEntry freeList;
	UINT Length;
} NdisBufferList, *PNdisBufferList;

PBYTE
ndisBufferListGetBuffer(
	IN  PNdisBufferList pList);

void
ndisBufferListFreeBuffer(
	IN  PNdisBufferList      pList,
	IN  PBYTE                pData);

NDIS_STATUS
ndisBufferListAllocate(
	OUT PNdisBufferList *ppList,
	IN  UINT             dataLength,
	IN  UINT             numStartingBuffers);

void
ndisBufferListFree(
	IN PNdisBufferList pList);

static inline void
encodeUshort(
	 PBYTE pMem,
	 USHORT value)
//
//  encode a Ushort into memory in big endian byte order
//
{
	pMem[0] = (BYTE)(value >> 8);
	pMem[1] = (BYTE)value;
}

static inline USHORT
extractUshort(
	PBYTE pMem)
//
//  read a big endian Ushort from memory as 2 consecutive bytes
//
{
	return (pMem[0] << 8) | pMem[1];
}

PBYTE
encodeUlong(
	PBYTE pValue,
	DWORD value);

void
DebugPrintNdisPacket(
	PSTR szHeader,
	PNDIS_PACKET pPacket);

////////////////////////////////////////////////
//
// Data structures for the IM device
//

typedef enum
{
	EthPacketTypeARP,
	EthPacketTypeIPv4,
	EthPacketTypeIPv6,
	EthPacketTypeCount
} EthPacketType;

#define ETH_MAC_ADDRESS_LENGTH	6

typedef NDIS_STATUS (*VEMOidRequestHandler)(IN PVOID pContext, IN PVOID Buffer, IN ULONG BufferLength, OUT PULONG BytesReadWritten, OUT PULONG BytesNeeded);

typedef struct
{
	NDIS_OID	         Oid;
	ULONG                MinBufferSize;
	VEMOidRequestHandler Handler;
} VEMOidHandlerEntry, *PVEMOidHandlerEntry;

//
// Configuration parameters that are passed by
// the user of the VEM service during instance creation/initialization.
//
typedef struct
{
	DWORD version;
#define VEM_CONFIG_VERSION_0	0	// Initial version, others may be added later

	DWORD flags;
#define VEM_CONFIG_FLAG_NO_DHCP_SERVER		 (1<<0) // Tells VEM to NOT intercept DHCP packets
                                                    // That is, VEM will not enable DHCP server emulation for the interface
                                                    // Presumably something at the MAC layer or below on the network will handle DHCP
#define VEM_CONFIG_FLAG_PASSTHRU_DHCP_INFORM (1<<1) // Tells VEM to NOT intercept DHCP inform packets
	                                                // This flag is ignored if NO_DHCP_SERVER is set, as all
	                                                // DHCP packets are passed through in that case.
#define VEM_CONFIG_FLAG_USE_STATIC_IP        (1<<2) // Configure the new interface to use a Static IP address rather than DHCP
#define VEM_CONFIG_FLAG_SPECIFIC_BINDINGS    (1<<3) // Control bindings through VEMAdd/DeleteBindings
#define VEM_CONFIG_FLAG_NO_DEFAULT_GATEWAY   (1<<4) // Do not add a default route for this adapter
#define VEM_CONFIG_FLAG_BEST_DEFAULT_GATEWAY (1<<5) // Set default route metric to 1 for this adapter
#define VEM_CONFIG_FLAG_SPECIFY_MAC_ADDRESSES (1<<6) // Use Local/PeerEthMACAddress fields

	//
	// Ethernet config information for the device
	//
	BYTE        LocalEthMACAddress[ETH_MAC_ADDRESS_LENGTH];
	BYTE        PeerEthMACAddress[ETH_MAC_ADDRESS_LENGTH];

	//
	// IP configuration information for the interface
	//
	DWORD       IfIPAddress;
	DWORD       IfSubnetMask;
	DWORD		GatewayIPAddress;
	DWORD       PrimaryDNSAddress;
	DWORD       SecondaryDNSAddress;

	// Function used to send IP packets
	NDIS_STATUS (*SendPacketHandler)(IN PVOID pContext, IN PNDIS_PACKET pPacket, IN PNDIS_BUFFER pBuffer, IN UINT headerOffset, IN EthPacketType Type);

	// Function used to return packets that were receive indicated
	NDIS_STATUS (*ReturnPacketHandler)(IN PVOID pContext, PNDIS_PACKET Packet);

	// Shutdown/cleanup handler
	NDIS_STATUS (*ShutdownHandler)(IN PVOID pContext);

	// Overrides/extensions to the basic set of OID set/get request handlers
	// provided by VEM.
	PVEMOidHandlerEntry pQueryHandlerTable;
	ULONG               numQueryHandlerTableEntries;

	PVEMOidHandlerEntry pSetHandlerTable;
	ULONG               numSetHandlerTableEntries;

	// Maximum frame size to send/receive
	// e.g. that can be passed to SendPacketHandler()
	DWORD               MaxFrameSize;

	// Speed in bits per second of the underlying interface
	DWORD               LinkBps;

	// Medium type, typically NdisMedium802_3
	NDIS_MEDIUM         MediumType;

} VEMAdapterConfigParams, *PVEMAdapterConfigParams;




//////////////////////////////////////////////////
//
// Data structures for the miniport device

#define DRIVER_MAJOR_VERSION 1
#define DRIVER_MINOR_VERSION 0


#define ETH_MAX_PAYLOAD_SIZE 1500
#define ETH_HEADER_SIZE        14
#define TX_BUF_SIZE          ETH_MAX_PAYLOAD_SIZE


#define ETH_TYPE_IP	  0x0800
#define ETH_TYPE_ARP  0x0806
#define ETH_TYPE_IPV6 0x86DD

typedef enum
{
	IPPayloadTypeNA,   // Not IP (e.g. ARP)
	IPPayloadTypeDHCP,
	IPPayloadTypePlugin,
	IPPayloadTypeCount
} IPPayloadType;

//
// Packet reserved info in each packet we indicate receives for.
// This info is used to figure out what to do when the packet is returned
// back to us via MiniportReturnPacket.
//
typedef struct
{
	EthPacketType ethPacketType;
	IPPayloadType ipPayloadType;
	PVOID         PluginData;
} VEMPacketReserved, *PVEMPacketReserved;

//
// This structure contains all the information about a single
// adapter that this driver is controlling.
//
typedef struct _VEM_ADAPTER {

    //
    // This is the handle given by the wrapper for calling ndis
    // functions.
    //
    NDIS_HANDLE MiniportAdapterHandle;

    //
    // Flag that is set when we are shutting down the interface
    //
    BOOL  ShuttingDown;

	//
	// Statistics values
	//
	ULONG FramesXmitGood;
	ULONG FramesRcvGood;

	// Current Packet Filter setting
	DWORD PacketFilter;

	// Current Lookahead size
	DWORD LookAhead;

	/////////////////////////////////
	// Ethernet layer data
	//

	// Our 'pretend' local adapter MAC address
	BYTE        EthMACAddr[ETH_MAC_ADDRESS_LENGTH];

	// Our 'pretend' peer MAC address (same for all peers)
	BYTE        EthPeerMACAddr[ETH_MAC_ADDRESS_LENGTH];

	NDIS_HANDLE hEthPacketPool;

	// We only need 2 types of headers to indicate packets, one
	// for IP and one for ARP.
	// Note: This assumes no SNAP header needed at this time... (ETH_HEADER_SIZE doesn't include SNAP space)
	BYTE        EthHeaderBuffer[EthPacketTypeCount][ETH_HEADER_SIZE];

	PNdisBufferPool pBufferPool[EthPacketTypeCount];

	// NdisMediaStateConnected or NdisMediaStateDisconnected
	NDIS_MEDIA_STATE EthMediaState;

	//
	/////////////////////////////////

	/////////////////////////////////
	// ARP layer data
	//
	PNdisBufferPool pArpBufferPool;

	PNdisBufferList pArpBufferList;

	//
	/////////////////////////////////

	/////////////////////////////////
	// IP layer data
	//
	PNdisBufferPool pIPBufferPool;

	//
	/////////////////////////////////

	/////////////////////////////////
	// DHCP layer data

	PDhcpServerSession pDhcpSession;

	//
	/////////////////////////////////

	/////////////////////////////////
	// IM layer data

	// Name
	NDIS_STRING AdapterName;

	// Info passed into us by the client requesting creation of VEM device
	VEMAdapterConfigParams VEMConfigParams;

	// The opaque context passed into VEMDeviceCreate, that we pass
	// back to the SendPacketHandler, etc. VEMConfigParams callback handlers.
	PVOID       pCallbackContext;

	//
	/////////////////////////////////

} VEM_ADAPTER, *PVEM_ADAPTER;

NDIS_STATUS
VEMDeviceCreate(
	IN  PWSTR					wszDeviceName,
	IN  PVEMAdapterConfigParams pConfigParams,
	IN  PVOID                   pCallbackContext,
	OUT PVEM_ADAPTER           *ppContext);

NDIS_STATUS
VEMDeviceDestroy(
	IN PVEM_ADAPTER Adapter);


NDIS_STATUS
VEMReceivePacket(
	 IN PVEM_ADAPTER  Adapter,
	 IN PNDIS_PACKET  pPacket,
	 IN EthPacketType Type);

PNDIS_PACKET
VEMGetNDISPacket(
	 IN PVEM_ADAPTER Adapter);

void
VEMFreeNDISPacket(
	 IN PVEM_ADAPTER Adapter,
	 IN PNDIS_PACKET Packet);

PNDIS_BUFFER
VEMGetNDISBuffer(
	 IN PVEM_ADAPTER Adapter,
	 IN PVOID        pBufferData,
	 IN DWORD        cbBufferData);

void
VEMFreeNDISBuffer(
	IN PVEM_ADAPTER Adapter,
	IN PNDIS_BUFFER pBuffer);

void
VEMSendComplete(
	IN PVEM_ADAPTER Adapter,
	IN PNDIS_PACKET Packet,
	IN NDIS_STATUS  Status);

void
VEMIndicateStatus(
	IN PVEM_ADAPTER     Adapter,
	IN NDIS_STATUS      Status,
	IN PVOID			StatusBuffer,
	IN UINT				StatusBufferSize);

void
VEMSetIPConfig(
	IN PVEM_ADAPTER		Adapter,
	IN DWORD            IfIPAddress,
	IN DWORD            IfSubnetMask,
	IN DWORD            GatewayIPAddress,
	IN DWORD            PrimaryDNSAddress,
	IN DWORD            SecondaryDNSAddress);

void
VEMSetWINSConfig(
	IN PVEM_ADAPTER		Adapter,
	IN DWORD 		    PrimaryWINSAddress,
	IN DWORD 		    SecondaryWINSAddress);

void
VEMSetDomain(
	IN PVEM_ADAPTER		Adapter,
	IN PSTR 		    szDomain);

void
VEMSetDontAddDefaultGateway(
	IN PVEM_ADAPTER		Adapter,
	IN DWORD 		    dwDontAddDefaultGateway);

void
VEMAddBindings(
	IN PVEM_ADAPTER		Adapter,
	IN PWSTR            mwszProtocolNamesToAdd);

void
VEMDeleteBindings(
	IN PVEM_ADAPTER		Adapter,
	IN PWSTR            mwszProtocolNamesToDelete);

void
VEMSetMediaState(
	IN PVEM_ADAPTER     Adapter,
	IN NDIS_MEDIA_STATE MediaState);

void
VEMQueryInformationComplete(
	IN PVEM_ADAPTER		Adapter,
	IN NDIS_STATUS		Status);

void
VEMSetInformationComplete(
	IN PVEM_ADAPTER		Adapter,
	IN NDIS_STATUS		Status);



#ifdef __cplusplus
}
#endif

