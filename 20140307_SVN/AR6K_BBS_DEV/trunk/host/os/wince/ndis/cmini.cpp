//------------------------------------------------------------------------------
// <copyright file="cmini.cpp" company="Atheros and Microsoft">
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
// NDIS Miniport class.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================

#include <windows.h>
#include <ndis.h>
#include "cmini.hpp"
#include "a_osapi.h"
#include "dbglog_api.h"

#define _P	L"\r[AR6K: CMINI] "L


enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0,
	DBG_OID		= 0
}CminiDebug;


CMiniport::CMiniport()
{
	m_MiniportAdapterHandle = NULL;

	NdisAllocateSpinLock(&m_Lock);
}

CMiniport::~CMiniport()
{
	NdisFreeSpinLock(&m_Lock);
}

NDIS_STATUS
CMiniport::Initialize(
	IN NDIS_HANDLE MiniportAdapterHandle,
	IN NDIS_HANDLE pConfigHandle)
//
//  Initialize is a required function that sets up a NIC for network I/O
//  operations, claims all hardware resources necessary to the NIC, and
//  allocates resources the driver needs to carry out network I/O operations.
//
{
	m_MiniportAdapterHandle = MiniportAdapterHandle;

	return NDIS_STATUS_SUCCESS;
}

void
CMiniport::Halt()
//
//  Halt is a required function that deallocates resources (e.g. those
//  allocated by Initialize) when the NIC is removed and halts the NIC.
//
{
}

BOOLEAN
CMiniport::CheckForHang()
//
//  CheckForHang is an optional function that reports the state of
//  the NIC or monitors the responsiveness of the underlying driver.
//
//  CheckForHang returns TRUE if the driver determines that the NIC
//  is not operating.
//
{
	// Never hangs
	return FALSE;
}

NDIS_STATUS
CMiniport::Reset(
	OUT PBOOLEAN pAddressingReset)
{
	// No hardware or data to reset
	return NDIS_STATUS_SUCCESS;
}

static const NDIS_PHYSICAL_ADDRESS g_HighestAcceptableAddress = {-1, -1};

PVOID
CMiniport::MemoryAllocate(
	IN UINT cbMemory)
//
//  Allocate a block of memory Length bytes long.
//
{
	PVOID       pMemory = NULL;

#if 0	//bluebird	
	NDIS_STATUS Status;

	Status = NdisAllocateMemory(&pMemory, cbMemory, 0, g_HighestAcceptableAddress);
    if (Status == NDIS_STATUS_FAILURE) {
	    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - NdisAllocateMemory for %d bytes failed error = %x\n", cbMemory, Status);
    }
#endif	//bluebird
	return pMemory;
}

void
CMiniport::MemoryFree(
	IN PVOID pMemory)
//
//  Free a block of memory allocated by MemoryAllocate.
//
{
	NdisFreeMemory(pMemory, 0, 0);
}

//
// Set of _GEN Oids supported for Query operations:
//
static MPOidInfo g_GenSupportedQueryOids[] =
{
	{ OID_GEN_VENDOR_DRIVER_VERSION, sizeof(ULONG) },
	{ OID_GEN_MAC_OPTIONS,           sizeof(ULONG) },
	{ OID_GEN_SUPPORTED_LIST,        0 },
	{ OID_GEN_HARDWARE_STATUS,       sizeof(NDIS_HARDWARE_STATUS) },
	{ OID_GEN_MEDIA_SUPPORTED,       sizeof(NDIS_MEDIUM) },
	{ OID_GEN_MEDIA_IN_USE,          sizeof(NDIS_MEDIUM) },
	{ OID_GEN_PHYSICAL_MEDIUM,       sizeof(NDIS_MEDIUM) },
	{ OID_GEN_MEDIA_CONNECT_STATUS,  sizeof(ULONG) },
	{ OID_GEN_MAXIMUM_LOOKAHEAD,     sizeof(ULONG) },
	{ OID_GEN_CURRENT_LOOKAHEAD,     sizeof(ULONG) },
	{ OID_GEN_MAXIMUM_FRAME_SIZE,    sizeof(ULONG) },
	{ OID_GEN_MAXIMUM_TOTAL_SIZE,    sizeof(ULONG) },
	{ OID_GEN_LINK_SPEED,            sizeof(ULONG) },
	{ OID_GEN_TRANSMIT_BUFFER_SPACE, sizeof(ULONG) },
	{ OID_GEN_RECEIVE_BUFFER_SPACE,  sizeof(ULONG) },
	{ OID_GEN_TRANSMIT_BLOCK_SIZE,   sizeof(ULONG) },
	{ OID_GEN_RECEIVE_BLOCK_SIZE,    sizeof(ULONG) },
	{ OID_GEN_VENDOR_ID,             sizeof(ULONG) },
	{ OID_GEN_VENDOR_DESCRIPTION,    0 },
	{ OID_GEN_DRIVER_VERSION,        sizeof(USHORT) },
	{ OID_GEN_CURRENT_PACKET_FILTER, sizeof(ULONG) },
	{ OID_GEN_MAXIMUM_SEND_PACKETS,  sizeof(ULONG) },
	{ OID_GEN_XMIT_OK,               sizeof(ULONG) },
	{ OID_GEN_RCV_OK,                sizeof(ULONG) },
	{ OID_GEN_XMIT_ERROR,            sizeof(ULONG) },
	{ OID_GEN_RCV_ERROR,             sizeof(ULONG) },
	{ OID_GEN_RCV_NO_BUFFER,         sizeof(ULONG) },
	{ OID_GEN_MEDIA_CAPABILITIES,    sizeof(ULONG) },
	{ OID_PNP_CAPABILITIES,          sizeof(NDIS_PNP_CAPABILITIES) },
	{ OID_PNP_QUERY_POWER,           0 },
	{ 0,                             0 }
};

//
// Set of _GEN Oids supported for Set operations:
//
static MPOidInfo g_GenSupportedSetOids[] =
{
	{ OID_GEN_CURRENT_PACKET_FILTER, sizeof(ULONG) },
	{ OID_GEN_CURRENT_LOOKAHEAD,     sizeof(ULONG) },
	{ OID_GEN_PROTOCOL_OPTIONS,      sizeof(ULONG) },
	{ OID_PNP_SET_POWER,             sizeof(NDIS_DEVICE_POWER_STATE) },
	{ 0,                             0 }
};

NDIS_STATUS
CMiniport::CheckOidRequest(
	IN     MPOidInfo               *pOidInfo,
	IN     NDIS_OID                 Oid,
	IN     ULONG                    BufferLength,
	OUT    PULONG                   BytesNeeded)
//
//  Check to see that an OID request:
//		1. Is for a supported OID, and
//		2. That the provided buffer is not smaller than the minimum required space
//
{
	NDIS_STATUS    Status = NDIS_STATUS_INVALID_OID;

	for ( ; 0 != pOidInfo->Oid; pOidInfo++)
	{
		if (Oid == pOidInfo->Oid)
		{
			// Match found
			*BytesNeeded = pOidInfo->MinBufferSize;
			if (BufferLength < pOidInfo->MinBufferSize)
			{
				// The supplied buffer is not big enough
				Status = NDIS_STATUS_BUFFER_TOO_SHORT;
			}
			else
			{
				// Oid is supported, buffer size is ok.
				Status = NDIS_STATUS_SUCCESS;
			}
			break;
		}
	}

	return Status;
}

void
CMiniport::AddOidsToList(
	IN     MPOidInfo               *pOidInfo,
	IN OUT PNDIS_OID                pCurrentList,
	IN OUT PULONG                   pCurrentListCount,
	IN OUT PULONG                   pOidSlotsAvailable,
	IN OUT PULONG                   TotalOidCount)
//
//  Add the oids from the specified MPOidInfo to the array,
//  if they don't already appear in the array.
//
{
	ULONG     CurrentListCount = *pCurrentListCount;
	PNDIS_OID pEndList = pCurrentList + CurrentListCount;

	//
	// Iterate through the set of Oids in pOidInfo
	//
	for ( ; 0 != pOidInfo->Oid; pOidInfo++)
	{
		//
		// Check to see if the Oid is already in our list
		//
		for (ULONG i = 0; i < CurrentListCount; i++)
		{
			if (pOidInfo->Oid == pCurrentList[i])
			{
				// This Oid is already in the list, skip it
				break;
			}
        }
		if (i == CurrentListCount)
		{
			// Reached the end of the list without finding the oid,
			// thus it is new and needs to be added to the list
			*TotalOidCount += 1;
			if (*pOidSlotsAvailable)
			{
				*pEndList++ = pOidInfo->Oid;
				*pOidSlotsAvailable -= 1;
			}
		}
	}

	// Set the number of OIDs now in the list
	*pCurrentListCount = pEndList - pCurrentList;
}

PNDIS_OID
CMiniport::GetSupportedOidList(
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
{
	PNDIS_OID   pOidList = pOidBuffer;
	ULONG       OidListCount = 0;

	AddOidsToList(&g_GenSupportedQueryOids[0], pOidList, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);
	AddOidsToList(&g_GenSupportedSetOids[0],   pOidList, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);

	return pOidList + OidListCount;
}

NDIS_STATUS
CMiniport::QueryInformation(
	IN  NDIS_OID Oid, 
	OUT PVOID    Buffer, 
	IN  ULONG    cbBuffer,
	OUT PULONG   pcbWritten,
	OUT PULONG   pcbNeeded)
//
// QueryInformation is a required function that returns information
// about the capabilities and status of the driver and/or its NIC.
//
{
	NDIS_STATUS             Status = NDIS_STATUS_SUCCESS;
	ULONG                   GenericULong;
	ULONG                   GenericUShort;
	NDIS_HARDWARE_STATUS    HardwareStatus;
	NDIS_MEDIUM             Medium;
	NDIS_PHYSICAL_MEDIUM	PhysicalMedium;
	NDIS_PNP_CAPABILITIES   PnpCaps;
	PVOID                   MoveSource = (PVOID)&GenericULong;
	ULONG                   MoveBytes = sizeof(GenericULong);

	// Check that OID is supported and buffer size is reasonable.
	NDIS_DEBUG_PRINTF(0, " %s() + Enter \r\n",__FUNCTION__);
	Status = CheckOidRequest(&g_GenSupportedQueryOids[0], Oid, cbBuffer, pcbNeeded);
	if (NDIS_STATUS_SUCCESS != Status) {
		NDIS_DEBUG_PRINTF(0, " %s() Not Support OID(%0x%x) \r\n",__FUNCTION__, Oid);
		goto done;
	}

	NDIS_DEBUG_PRINTF(0, " %s() ########### Oid = 0x%x \r\n",__FUNCTION__, Oid);

	switch (Oid)
	{
		case OID_GEN_VENDOR_DRIVER_VERSION:	
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_VENDOR_DRIVER_VERSION \r\n",__FUNCTION__);
			GenericULong = GetGenVendorDriverVersion();
			break;

		case OID_GEN_MAC_OPTIONS:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_MAC_OPTIONS \r\n",__FUNCTION__);
			// A bitmask that defines optional properties of the underlying driver or its NIC.
			GenericULong = GetGenMacOptions();
			break;

		case OID_GEN_SUPPORTED_LIST:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_SUPPORTED_LIST \r\n",__FUNCTION__);
			// An array of OIDs for objects that the underlying driver or its NIC supports.
			{
				ULONG TotalNumberOfSupportedOids = 0;
				ULONG MaxOidsToAddToBuffer = cbBuffer / sizeof(NDIS_OID);
                ULONG i,j;
                NDIS_OID temp;
                NDIS_OID *pOids;

				MoveBytes = 0;
				*pcbWritten = 0;
                pOids = (NDIS_OID *)Buffer;
				// Build list of supported OIDs
				GetSupportedOidList(pOids, &MaxOidsToAddToBuffer, &TotalNumberOfSupportedOids);
				*pcbNeeded = TotalNumberOfSupportedOids * sizeof(NDIS_OID);
				if (*pcbNeeded <= cbBuffer) {
                    // Sort the list 
                    for (i=0;i<TotalNumberOfSupportedOids;i++) {
                        for (j=0;j<(TotalNumberOfSupportedOids-i-1);j++) {
                            if (pOids[j] > pOids[j+1]) {
                                temp = pOids[j];
                                pOids[j] = pOids[j+1];
                                pOids[j+1] = temp;
                            }
                        }
                    }
					*pcbWritten = TotalNumberOfSupportedOids * sizeof(NDIS_OID);
                } else {
					Status = NDIS_STATUS_BUFFER_TOO_SHORT;
                }
			}
			break;

		case OID_GEN_HARDWARE_STATUS:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_HARDWARE_STATUS \r\n",__FUNCTION__);
			// The current hardware status of the underlying NIC 
			HardwareStatus = GetGenHardwareStatus();
			MoveSource = (PVOID)(&HardwareStatus);
			MoveBytes = sizeof(HardwareStatus);
			break;

		case OID_GEN_MEDIA_SUPPORTED:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_MEDIA_SUPPORTED \r\n",__FUNCTION__);
			// The media types that the NIC can support but not necessarily
			// the media types that the NIC currently uses.
			Medium = GetGenMediaSupported();
			MoveSource = (PVOID) (&Medium);
			MoveBytes = sizeof(Medium);
			break;

		case OID_GEN_MEDIA_IN_USE:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_MEDIA_IN_USE \r\n",__FUNCTION__);
			// A complete list of the media types that the NIC currently uses.
			Medium = GetGenMediaInUse();
			MoveSource = (PVOID) (&Medium);
			MoveBytes = sizeof(Medium);
			break;

		case OID_GEN_PHYSICAL_MEDIUM:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_PHYSICAL_MEDIUM \r\n",__FUNCTION__);
			// A complete list of the media types that the NIC currently uses.
			PhysicalMedium = GetGenPhysicalMedium();
			MoveSource = (PVOID) (&PhysicalMedium);
			MoveBytes = sizeof(PhysicalMedium);
			break;

		case OID_GEN_MEDIA_CONNECT_STATUS:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_MEDIA_CONNECT_STATUS \r\n",__FUNCTION__);
			// Whether the media is currently connected and able to Tx/Rx packets.
			// This is set by the plugin via VEMSetMediaState.
			GenericULong = GetGenMediaConnectStatus();                 
			break;

		case OID_GEN_MAXIMUM_LOOKAHEAD:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_MAXIMUM_LOOKAHEAD \r\n",__FUNCTION__);
			// The maximum number of bytes that the NIC can provide as lookahead data.
			// This specification does not include a header. 
			GenericULong = GetGenMaximumLookahead();
			break;

		case OID_GEN_CURRENT_LOOKAHEAD:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_CURRENT_LOOKAHEAD \r\n",__FUNCTION__);
			// The number of bytes of received packet data that will be indicated to the protocol driver.
			GenericULong = GetGenCurrentLookahead();
			break;

		case OID_GEN_MAXIMUM_FRAME_SIZE:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_MAXIMUM_FRAME_SIZE \r\n",__FUNCTION__);
			// The maximum network packet size, in bytes, that the NIC supports.
			// This specification does not include a header.
			GenericULong = GetGenMaximumFrameSize();
			break;

		case OID_GEN_MAXIMUM_TOTAL_SIZE:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_MAXIMUM_TOTAL_SIZE \r\n",__FUNCTION__);
			// The maximum total packet length, in bytes, the NIC supports.
			// This specification includes the header.
			GenericULong = GetGenMaximumTotalSize();
			break;

		case OID_GEN_LINK_SPEED:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_LINK_SPEED \r\n",__FUNCTION__);
			// Link speed in units of 100 bps.
			GenericULong = GetGenLinkSpeed();
			break;

		case OID_GEN_TRANSMIT_BUFFER_SPACE:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_TRANSMIT_BUFFER_SPACE \r\n",__FUNCTION__);
			// The amount of memory, in bytes, on the NIC that is available for buffering Tx/Rx data.
			GetGenTransmitBufferSpace();
			break;
			

		case OID_GEN_RECEIVE_BUFFER_SPACE:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_RECEIVE_BUFFER_SPACE \r\n",__FUNCTION__);
			// The amount of memory, in bytes, on the NIC that is available for buffering Tx/Rx data.
			GenericULong = GetGenReceiveBufferSpace();
			break;

		case OID_GEN_TRANSMIT_BLOCK_SIZE:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_TRANSMIT_BLOCK_SIZE \r\n",__FUNCTION__);
			// Minimum amount of storage, in bytes, that a single packet
			// occupies in the Tx/Rx buffer space of the NIC.
			GenericULong = GetGenTransmitBlockSize();
			break;

		case OID_GEN_RECEIVE_BLOCK_SIZE:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_RECEIVE_BLOCK_SIZE \r\n",__FUNCTION__);
			// Minimum amount of storage, in bytes, that a single packet
			// occupies in the Tx/Rx buffer space of the NIC.
			GenericULong = GetGenReceiveBlockSize();
			break;

		case OID_GEN_VENDOR_ID:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_VENDOR_ID \r\n",__FUNCTION__);
			// The OID_GEN_VENDOR_ID OID specifies a three-byte IEEE-registered vendor code,
			// followed by a single byte that the vendor assigns to identify a particular NIC.
			// Vendors without an IEEE registered code should use 0xFFFFFF for the vendor code.
			GenericULong = GetGenVendorId();
			break;

		case OID_GEN_VENDOR_DESCRIPTION:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_VENDOR_DESCRIPTION \r\n",__FUNCTION__);
			// The OID_GEN_VENDOR_DESCRIPTION OID points to a zero-terminated string describing the NIC.
			MoveSource = (PVOID)GetGenVendorDescription();
			MoveBytes = strlen((char *)MoveSource) + 1;
			break;

		case OID_GEN_DRIVER_VERSION:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_DRIVER_VERSION \r\n",__FUNCTION__);
			// The NDIS version in use by the NIC driver.
			// The high byte is the major version number; the low byte is the minor version number.
			GenericUShort = GetGenDriverVersion();
			MoveSource = (PVOID)(&GenericUShort);
			MoveBytes = sizeof(GenericUShort);
			break;

		case OID_GEN_CURRENT_PACKET_FILTER:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_CURRENT_PACKET_FILTER \r\n",__FUNCTION__);
			// The types of net packets for which a protocol receives indications from a NIC driver. 
			// Queries for this should be intercepted and handled by the NDIS layer, they shouldn't
			// make it down to the miniport...
			GenericULong = GetGenCurrentPacketFilter();
			break;

		case OID_GEN_MAXIMUM_SEND_PACKETS:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_MAXIMUM_SEND_PACKETS \r\n",__FUNCTION__);
			// For serialized miniports:
			//    The maximum number of send packet descriptors that a miniport driver's 
			//    MiniportSendPackets function can accept in the array at its PacketArray parameter.
			//
			// For deserialized miniports:
			//    NDIS ignores any value returned by a deserialized driver in response to a
			//    query of OID_GEN_MAXIMUM_SEND_PACKETS.
			GenericULong = 1;
			break;

		//
		// Statistics OIDs...
		//
		case OID_GEN_XMIT_OK:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_XMIT_OK \r\n",__FUNCTION__);
			GenericULong = GetGenXmitOk();
			break;

		case OID_GEN_RCV_OK:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_RCV_OK \r\n",__FUNCTION__);
			GenericULong = GetGenRcvOk();
			break;

		case OID_GEN_XMIT_ERROR:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_XMIT_ERROR \r\n",__FUNCTION__);
			GenericULong = GetGenXmitError();
			break;

		case OID_GEN_RCV_ERROR:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_RCV_ERROR \r\n",__FUNCTION__);
			GenericULong = GetGenRcvError();
			break;

		case OID_GEN_RCV_NO_BUFFER:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_RCV_NO_BUFFER \r\n",__FUNCTION__);
			GenericULong = GetGenRcvNoBuffer();
			break;

		case OID_GEN_MEDIA_CAPABILITIES:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_GEN_MEDIA_CAPABILITIES \r\n",__FUNCTION__);
			GenericULong = GetGenMediaCapabilities();
			break;

		case OID_PNP_CAPABILITIES: 
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_PNP_CAPABILITIES \r\n",__FUNCTION__);
			MoveBytes = sizeof(NDIS_PNP_CAPABILITIES);
			MoveSource = &PnpCaps;
			GetPnpCapabilities(&PnpCaps);
			break;
           
		case OID_PNP_QUERY_POWER:
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> OID_PNP_QUERY_POWER \r\n",__FUNCTION__);
			Status = QueryPnpPower((NDIS_DEVICE_POWER_STATE *)Buffer);
			MoveBytes = 0;
			MoveSource = NULL;
			break;
      
		default:
			// Should never get here, CheckOidRequest should have caught unsupported Oids
			NDIS_DEBUG_PRINTF(DBG_QUERY_OID, " %s() -> default, oid = %d \r\n",__FUNCTION__, Oid);
			ASSERT(FALSE);
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
			if (MoveBytes)
			{
				memcpy(Buffer, MoveSource, MoveBytes);
				*pcbWritten = MoveBytes;
			}
		}
	}

done:
	return Status;
}

NDIS_STATUS
CMiniport::SetInformation(
	IN  NDIS_OID Oid, 
	OUT PVOID    Buffer, 
	IN  ULONG    cbBuffer,
	OUT PULONG   pcbRead,
	OUT PULONG   pcbNeeded)
{
	NDIS_STATUS Status;

	// Check that OID is supported and buffer size is reasonable.
	Status = CheckOidRequest(&g_GenSupportedSetOids[0], Oid, cbBuffer, pcbNeeded);
	if (NDIS_STATUS_SUCCESS != Status)
		goto done;

	*pcbRead = *pcbNeeded;

	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() + Enter, OID(%d) \r\n", __FUNCTION__, Oid);

	switch (Oid)
	{
		case OID_GEN_CURRENT_PACKET_FILTER:
			NDIS_DEBUG_PRINTF(DBG_SET_OID, " OID_GEN_CURRENT_PACKET_FILTER \r\n");
			// A bitmask specifying the type(s) of received packets that should
			// be indicated to protocol drivers, e.g.:
			//    NDIS_PACKET_TYPE_DIRECTED
			//    NDIS_PACKET_TYPE_BROADCAST
			//    NDIS_PACKET_TYPE_MULTICAST
			//    NDIS_PACKET_TYPE_ALL_LOCAL
			Status = SetGenCurrentPacketFilter(*(PULONG)Buffer);
			break;

		case OID_GEN_CURRENT_LOOKAHEAD:
			NDIS_DEBUG_PRINTF(DBG_SET_OID, " OID_GEN_CURRENT_LOOKAHEAD \r\n");
			// The number of bytes of received packet data, excluding the header,
			// that will be indicated to protocol drivers.
			Status = SetGenCurrentLookahead(*(PULONG)Buffer);
			break;

		case OID_GEN_PROTOCOL_OPTIONS:
			NDIS_DEBUG_PRINTF(DBG_SET_OID, " OID_GEN_PROTOCOL_OPTIONS \r\n");
			// A bitmask that defines optional properties of the protocol driver, e.g.:
			//    NDIS_PROT_OPTION_ESTIMATED_LENGTH
			//    NDIS_PROT_OPTION_NO_LOOPBACK
			Status = SetGenProtocolOptions(*(PULONG)Buffer);
			break;

		case OID_PNP_SET_POWER:
			NDIS_DEBUG_PRINTF(DBG_SET_OID, " OID_PNP_SET_POWER \r\n");
			Status = SetPnpPower((NDIS_DEVICE_POWER_STATE *)Buffer);
			break;

		default:
			// Should never get here, CheckOidRequest should have caught unsupported Oids
			ASSERT(FALSE);
			Status = NDIS_STATUS_INVALID_OID;
			break;
	}

done:
	return Status;
}

ULONG
CMiniport::GetGenVendorId()
//
// The OID_GEN_VENDOR_ID OID specifies a three-byte IEEE-registered vendor code,
// followed by a single byte that the vendor assigns to identify a particular NIC.
// Vendors without an IEEE registered code should use 0xFFFFFF for the vendor code.
//
{
	// Default to unknown
	return 0xFFFFFFFF;
}

const char *
CMiniport::GetGenVendorDescription()
// The OID_GEN_VENDOR_DESCRIPTION OID returns a zero-terminated string describing the NIC.
{
	return "Vendor Description Not Specified";
}

ULONG
CMiniport::GetGenMediaCapabilities()
// The OID_GEN_MEDIA_CAPABILITIES OID indicates whether transmits and receives are supported.
{
	// Default implementation indicates that this miniport can both send and receive.
	return NDIS_MEDIA_CAP_TRANSMIT | NDIS_MEDIA_CAP_RECEIVE;
}

ULONG
CMiniport::GetGenCurrentPacketFilter()
{
	return m_CurrentPacketFilter;
}

NDIS_STATUS
CMiniport::SetGenCurrentPacketFilter(
	ULONG PacketFilter)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	// Check to see that no unsupported types have been requested
	if (PacketFilter & ~GetSupportedPacketTypes())
	{
		Status = NDIS_STATUS_INVALID_DATA;
	}
	else
	{
		m_CurrentPacketFilter = PacketFilter;
	}
	return Status;
}

NDIS_STATUS
CMiniport::SetGenCurrentLookahead(
	ULONG Lookahead)
{
	m_CurrentLookahead = Lookahead;
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CMiniport::SetGenProtocolOptions(
	ULONG ProtocolOptions)
{
	m_ProtocolOptions = ProtocolOptions;
	return NDIS_STATUS_SUCCESS;
}
