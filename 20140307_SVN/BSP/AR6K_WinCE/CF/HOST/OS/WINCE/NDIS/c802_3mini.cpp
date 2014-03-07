//------------------------------------------------------------------------------
// <copyright file="c802_3mini.cpp" company="Atheros and Microsoft">
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
//    The software is licensed “as-is.” 
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// NDIS 802.3 Miniport class.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================

#include <windows.h>
#include <ndis.h>

#include "cmini.hpp"
#include "c802_3mini.hpp"

C802_3Miniport::C802_3Miniport()
{
}

C802_3Miniport::~C802_3Miniport()
{
}

NDIS_MEDIUM
C802_3Miniport::GetSupportedNdisMediumType()
{
	return NdisMedium802_3;
}

static MPOidInfo g_802_3SupportedQueryOids[] =
{
	{ OID_802_3_PERMANENT_ADDRESS,   sizeof(ETHERNET_MAC_ADDRESS_LENGTH) },
	{ OID_802_3_CURRENT_ADDRESS,     sizeof(ETHERNET_MAC_ADDRESS_LENGTH) },
	{ OID_802_3_MULTICAST_LIST,      0 },
	{ OID_802_3_MAXIMUM_LIST_SIZE,   sizeof(ULONG) },
	{ OID_802_3_MAC_OPTIONS,         sizeof(ULONG) },
	{ OID_802_3_RCV_ERROR_ALIGNMENT, sizeof(ULONG) },
	{ OID_802_3_XMIT_ONE_COLLISION,  sizeof(ULONG) },
	{ OID_802_3_XMIT_MORE_COLLISIONS,sizeof(ULONG) },
	{ 0,                             0 }
};


NDIS_STATUS
C802_3Miniport::QueryInformation(
	IN  NDIS_OID Oid, 
	OUT PVOID    Buffer, 
	IN  ULONG    cbBuffer,
	OUT PULONG   pcbWritten,
	OUT PULONG   pcbNeeded)
//
// QueryInformation is a required function that returns information
// about the capabilities and staus of the driver and/or its NIC.
//
{
	NDIS_STATUS             Status = NDIS_STATUS_SUCCESS;
	ULONG                   GenericULong;
	ULONG                   NumMulticastAddresses;
	PVOID                   MoveSource = (PVOID)&GenericULong;
	ULONG                   MoveBytes = sizeof(GenericULong);

	// Check that OID is supported and buffer size is reasonable.
	Status = CheckOidRequest(&g_802_3SupportedQueryOids[0], Oid, cbBuffer, pcbNeeded);
	if (NDIS_STATUS_INVALID_OID == Status)
	{
		// OID is not a supported 802_3 OID. Check to see if it is a generic OID.
		Status = CMiniport::QueryInformation(Oid, Buffer, cbBuffer, pcbWritten, pcbNeeded);
		goto done;
	}
	if (NDIS_STATUS_SUCCESS != Status)
		goto done;

	switch (Oid)
	{
		//
		// 802_3 OIDS
		//
		case OID_802_3_PERMANENT_ADDRESS:
			// The address of the NIC encoded in the hardware.
			MoveSource = (PVOID)Get802_3PermanentAddress();
			MoveBytes = ETHERNET_MAC_ADDRESS_LENGTH;
			break;

		case OID_802_3_CURRENT_ADDRESS:
			// The address the NIC is currently using.
			MoveSource = (PVOID)Get802_3CurrentAddress();
			MoveBytes = ETHERNET_MAC_ADDRESS_LENGTH;
			break;

		case OID_802_3_MULTICAST_LIST:
			// The multicast address list on the NIC enabled for packet reception.
			Get802_3MulticastList((PBYTE *)&MoveSource, &NumMulticastAddresses);
			MoveBytes = NumMulticastAddresses * ETHERNET_MAC_ADDRESS_LENGTH;
			break;

		case OID_802_3_MAXIMUM_LIST_SIZE:
			// The maximum number of multicast addresses the NIC driver can manage.
			GenericULong = Get802_3MaxMulticastListSize();
			break;

		case OID_802_3_MAC_OPTIONS:
			// The maximum number of multicast addresses the NIC driver can manage.
			GenericULong = Get802_3MacOptions();
			break;

		case OID_802_3_RCV_ERROR_ALIGNMENT:
			GenericULong = Get802_3RcvErrorAlignment();
			break;

		case OID_802_3_XMIT_ONE_COLLISION:
			GenericULong = Get802_3XmitOneCollision();
			break;

		case OID_802_3_XMIT_MORE_COLLISIONS:
			GenericULong = Get802_3XmitMoreCollision();
			break;

		default:
			Status = NDIS_STATUS_INVALID_OID;
			break;
	}

	if (Status == NDIS_STATUS_SUCCESS)
	{
		if (MoveBytes > cbBuffer)
		{
			// Not enough room in Buffer. 
			*pcbNeeded = MoveBytes;
			Status = NDIS_STATUS_INVALID_LENGTH;
		}
		else
		{
			// Store result.
			memcpy(Buffer, MoveSource, MoveBytes);
			*pcbWritten = MoveBytes;
		}
	}

done:
	return Status;
}

static MPOidInfo g_802_3SupportedSetOids[] =
{
	{ OID_802_3_MULTICAST_LIST,   0 },
	{ 0,                          0 }
};

NDIS_STATUS
C802_3Miniport::SetInformation(
	IN  NDIS_OID Oid, 
	IN  PVOID    Buffer, 
	IN  ULONG    cbBuffer,
	OUT PULONG   pcbRead,
	OUT PULONG   pcbNeeded)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	ULONG NumMulticastAddresses;

	switch (Oid)
	{
		case OID_802_3_MULTICAST_LIST:
			if (0 != (cbBuffer % ETHERNET_MAC_ADDRESS_LENGTH))
				Status = NDIS_STATUS_INVALID_LENGTH;

			NumMulticastAddresses = cbBuffer / ETHERNET_MAC_ADDRESS_LENGTH;
			Status = Set802_3MulticastList((PBYTE)Buffer, NumMulticastAddresses);
			if (NDIS_STATUS_SUCCESS == Status)
					*pcbRead = *pcbNeeded;
			break;

		default:
			// Not an Oid we handle/recognize, pass it on to our base class.
			Status = CMiniport::SetInformation(Oid, Buffer, cbBuffer, pcbRead, pcbNeeded);
			break;
	}

	return Status;
}

ULONG
C802_3Miniport::GetSupportedPacketTypes()
// Return the types of packets supported by the miniport,
// used in conjuction with OID_GEN_CURRENT_PACKET_FILTER requests.
{
	return NDIS_PACKET_TYPE_ALL_MULTICAST
		 | NDIS_PACKET_TYPE_ALL_LOCAL
		 | NDIS_PACKET_TYPE_BROADCAST
		 | NDIS_PACKET_TYPE_DIRECTED
		 | NDIS_PACKET_TYPE_MULTICAST
		 | NDIS_PACKET_TYPE_PROMISCUOUS
		 ;
}

PNDIS_OID
C802_3Miniport::GetSupportedOidList(
	OUT     PNDIS_OID  pOidBuffer, 
	IN  OUT PULONG     pMaxOidsToAddToBuffer,
	IN  OUT PULONG     pTotalNumberOfSupportedOids)
//
//  Add the OIDs supported by the driver to pOidBuffer, but
//  do not add more than MaxOidsToAddToBuffer.
//
//  TotalNumberOfSupportedOids is set to the number of Oids
//  that the driver supports.
//
//  Returns a pointer to the next unused slot in pOidBuffer
//  after the oids have been added.
//
{
	PNDIS_OID   p802_3OidListStart;
	ULONG       OidListCount = 0;

	// First fill in base class supported OIDs
	p802_3OidListStart = CMiniport::GetSupportedOidList(pOidBuffer, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);

	// Now append 802.3 OIDs
	AddOidsToList(&g_802_3SupportedQueryOids[0], p802_3OidListStart, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);
	AddOidsToList(&g_802_3SupportedSetOids[0],   p802_3OidListStart, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);

	return p802_3OidListStart + OidListCount;
}

NDIS_HARDWARE_STATUS
C802_3Miniport::GetGenHardwareStatus()
// The current hardware status of the underlying NIC 
{
	return NdisHardwareStatusReady;
}

NDIS_MEDIUM
C802_3Miniport::GetGenMediaSupported()
// The media types that the NIC can support but not necessarily
// the media types that the NIC currently uses.
{
	return NdisMedium802_3;
}

NDIS_MEDIUM
C802_3Miniport::GetGenMediaInUse()
// A complete list of the media types that the NIC currently uses.
{
	return NdisMedium802_3;
}

ULONG
C802_3Miniport::GetGenMaximumLookahead()
// The maximum number of bytes that the NIC can provide as lookahead data.
// This specification does not include a header. 
{
	return 1500;
}

ULONG
C802_3Miniport::GetGenCurrentLookahead()
// The number of bytes of received packet data that will be indicated to the protocol driver.
{
	return 1500;
}

ULONG
C802_3Miniport::GetGenMaximumFrameSize()
// The maximum network packet size, in bytes, that the NIC supports.
// This specification does not include a header.
{
	return 1500;
}

ULONG
C802_3Miniport::GetGenMaximumTotalSize()
// The maximum total packet length, in bytes, the NIC supports.
// This specification includes the header.
{
	return ETH_HEADER_SIZE + 1500;
}
