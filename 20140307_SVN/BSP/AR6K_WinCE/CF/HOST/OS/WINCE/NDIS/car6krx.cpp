//------------------------------------------------------------------------------
// <copyright file="" company="Atheros and Microsoft">
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
// AR6000 NDIS Miniport received packet handling functions.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================


#include <windows.h>
#include <ndis.h>


#include "htc_internal.h"
#include "htc.h"
#include "wmi_api.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
#include "athdefs.h"
extern "C" {
#include "bmi.h"
}

#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include "osapi.h"


static const CAP_CONST bridgeTunnel = ETHERNET_TUNNEL;
static const CAP_CONST vrfc1042		= RFC1042SNAP;

void
CAR6KMini::ReceiveWMIDataPacket(
	HTC_EVENT_INFO  *evInfo)
//
//  This function processes data from an HTC_BUFFER_RECEIVED indication
//  not on the WMI_CONTROL_MBOX endpoint.
//
{
	ndis_mini_buf_t *pb = (ndis_mini_buf_t *)evInfo->cookie;
	NDIS_STATUS      Status;
	NDIS_PACKET     *pPacket;
	NDIS_BUFFER     *pBuffer;
	PBYTE            pData;
	ULONG            cbData;
	BOOL			 doDix = FALSE;
	USHORT			 etherType;
	SNAP_HEADER		 *pSnapHdr;
	MAC_ADDRESS		 *pDestAddr,*tempAddr;
	BOOL			 mcForUs = FALSE;
		
	
	NDIS_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, "AR6K: +ReceiveWMIDataPacket");

	    
    if (evInfo->status != A_OK) {
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - ReceiveWMIPacket Error in receiving : status = %x\n", evInfo->status);
		if (A_OK != HTCBufferReceive(m_pHTCTarget, ENDPOINT2, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb)) {
			a_netbuf_free(pb);
		}
		goto done;
    }
    
	// evInfo->actualLength is the length of the pb->data including
	//      2 bytes: WMI_DATA_HEADER [optional - only if HTC header is 46 00]
	//     14 bytes: 802.3 MAC header
	//      8 bytes: SNAP header (with EthType as last 2 bytes)
	//      N bytes: payload (e.g. IP packet)
	pData = evInfo->buffer; 
	cbData = evInfo->actualLength;

	// Remove the WMI_DATA_HDR.
	if (cbData < sizeof(WMI_DATA_HDR))
	{
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - ReceiveWMIPacket missing WMI header (%u bytes)\n", evInfo->actualLength);
		if ( A_OK != HTCBufferReceive(m_pHTCTarget, ENDPOINT2, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb)) {
			a_netbuf_free(pb);
		}
		goto done;
    }
	pData += sizeof(WMI_DATA_HDR);
	cbData -= sizeof(WMI_DATA_HDR);

	if (cbData < sizeof(ETHERNET_MAC_HEADER) + sizeof(SNAP_HEADER))
	{
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - ReceiveWMIPacket missing MAC + SNAP (%u bytes)\n", cbData);
		if ( A_OK != HTCBufferReceive(m_pHTCTarget, ENDPOINT2, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb)) {
			a_netbuf_free(pb);
		}
		goto done;
	}

#ifdef WMM
	Lock();
    wmi_implicit_create_pstream((wmi_t *)m_pWMI, pb, DNLINK_TRAFFIC);
	Unlock();
#endif //WMM

	
	pDestAddr=(MAC_ADDRESS *)pData;
 	
 	/* Apply NDIS receive filter */
     if (isGrp(pDestAddr)) {
         if (isBcast(pDestAddr)) {
			 if (!(m_CurrentPacketFilter & NDIS_PACKET_TYPE_BROADCAST)) {
                 if ( A_OK != HTCBufferReceive(m_pHTCTarget, ENDPOINT2, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb)) {
					a_netbuf_free(pb);
				 }			
				 goto done;
             }
         } else {
			 isMcForUs(pDestAddr,&mcForUs);
			 if (!(m_CurrentPacketFilter & (NDIS_PACKET_TYPE_MULTICAST |
				 NDIS_PACKET_TYPE_ALL_MULTICAST)) || !(mcForUs))
             {
				 if ( A_OK != HTCBufferReceive(m_pHTCTarget, ENDPOINT2, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb)) {
					a_netbuf_free(pb);
				 }
				 goto done;
             }
		}
 		
     } else {
		tempAddr=(MAC_ADDRESS *) m_PermanentAddress;
		if ((A_MACADDR_COMP(pDestAddr,tempAddr) != 0) &&
             !(m_CurrentPacketFilter & NDIS_PACKET_TYPE_PROMISCUOUS))
        {
			if ( A_OK != HTCBufferReceive(m_pHTCTarget, ENDPOINT2, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb)) {
				a_netbuf_free(pb);
			}
			goto done;
        }
	}

	 // Allocate an NDIS_PACKET from our packet pool.
	NdisAllocatePacket(&Status, &pPacket, m_RxPacketPool);
	if (NDIS_STATUS_SUCCESS != Status)
	{
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - NdisAllocatePacket failed\n");
		if ( A_OK != HTCBufferReceive(m_pHTCTarget, ENDPOINT2, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb)) {
			a_netbuf_free(pb);
		}
		goto done;
	}

	// Check for ethernetType in SNAP header for NOVELL_IPX, APPLE_TALK_ARP etc
	// remove 802.3 length and SNAP header if it is not of these types 
	
	pSnapHdr=(SNAP_HEADER *)(pData+sizeof(ETHERNET_MAC_HEADER));
	etherType=A_BE2CPU16(pSnapHdr->Type);
	
	doDix=((A_MEMCMP(pSnapHdr,&bridgeTunnel, sizeof(CAP_CONST))) == 0);
	if((!doDix) && ((A_MEMCMP(pSnapHdr,&vrfc1042, sizeof(CAP_CONST))) == 0))
	{
		doDix = ((etherType != APPLE_TALK_ARP) && (etherType != NOVELL_IPX));
	}
	// Get rid of the 802.3 length and SNAP header by copying
	// the 802.3 DestMACAddr and SrcMACAddr forward so they
	// immediately precede the EthType at the end of the SNAP header.
	// That gives us a DIX packet.

	if (doDix) {
	memmove(pData + sizeof(SNAP_HEADER), pData, ETHERNET_MAC_ADDRESS_LENGTH * 2);
	pData += sizeof(SNAP_HEADER);
	cbData -= sizeof(SNAP_HEADER);
	}


	

	// Setup the fields of NDIS_BUFFER to point to our data.
	pBuffer = &pb->NdisBuffer;
	NdisInitializeBuffer(pBuffer, pData, cbData);

	// Chain the NDIS_BUFFER to the start of the NDIS_PACKET
	NdisChainBufferAtBack(pPacket, pBuffer);
	NDIS_SET_PACKET_HEADER_SIZE(pPacket, sizeof(ETHERNET_MAC_HEADER));
	NDIS_SET_PACKET_STATUS(pPacket, NDIS_STATUS_SUCCESS);

#ifdef NDIS_BUS_DRIVER
	NDIS_PACKET *PacketArray[1];
	PacketArray[0]=pPacket;
	NdisMIndicateReceivePacket(m_MiniportAdapterHandle, PacketArray, 1);
#else
	// Perform the indicate on the timer thread because tying up the
	// SDIO receive indication thread can result in a deadlock.
	PLIST_ENTRY pEntry = (PLIST_ENTRY)(pPacket->MiniportReserved);
	Lock();
	InsertTailList(&m_RxPendingPacketList, pEntry);
	NdisSetEvent(&m_RxPendingEvent);
	Unlock();
#endif	

done:
	NDIS_DEBUG_PRINTF(ATH_LOG_TRC | ATH_LOG_RECV, "AR6K: -ReceiveWMIDataPacket");
}

#define MAX_PACKETS_PER_INDICATE	10

void
CAR6KMini::IndicateReceivePackets()
//
//  This function is called on the worker thread to indicate received
//  packets.
//
{
	NDIS_PACKET *pPacket;
	NDIS_PACKET *PacketArray[MAX_PACKETS_PER_INDICATE];
	ULONG        NumPacketsToIndicate;
	LIST_ENTRY  *pNode;
	boolean      bEventSignalled;

	NdisResetEvent(&m_RxWorkItemExitedEvent);

	while (false == m_Halting)
	{
		// Wait for packets to be placed on the list
		bEventSignalled = NdisWaitEvent(&m_RxPendingEvent, 0);
		if (m_Halting || !bEventSignalled)
			break;

		NumPacketsToIndicate = 0;

		//
		// Get up to MAX_PACKET_PER_INDICATE packets from the RxPending list.
		// Put the packet pointers into an array.
		//
		Lock();
		while (!IsListEmpty(&m_RxPendingPacketList))
		{
			pNode = RemoveHeadList(&m_RxPendingPacketList);
			pPacket = (PNDIS_PACKET)((PBYTE)pNode - offsetof(NDIS_PACKET, MiniportReserved));
			PacketArray[NumPacketsToIndicate++] = pPacket;
			if (NumPacketsToIndicate == MAX_PACKETS_PER_INDICATE) {
				Unlock();
				NdisMIndicateReceivePacket(m_MiniportAdapterHandle, PacketArray, NumPacketsToIndicate);
				Lock();
				NumPacketsToIndicate = 0;
			}
		}
		NdisResetEvent(&m_RxPendingEvent);
		Unlock();

		//
		// Indicate the received packets up to protocol drivers bound to us.
		//
		if (NumPacketsToIndicate)
			NdisMIndicateReceivePacket(m_MiniportAdapterHandle, PacketArray, NumPacketsToIndicate);
	}

	NdisSetEvent(&m_RxWorkItemExitedEvent);
}

void
CAR6KMini::AllocateHTCPacketBuffers(
	HTC_ENDPOINT_ID  eid,
	UINT             numBuffers)
//
//  This function is called to allocate packet buffers and transfer
//  ownership of them to HTC so that HTC can use them to indicate
//  packet data to us via HTC_BUFFER_RECEIVED indications.
//
{
	ndis_mini_buf_t *pb = NULL;

	for (UINT i = 0; i < numBuffers; i++)
	{
		pb = (ndis_mini_buf_t *)a_netbuf_alloc(AR6000_BUFFER_SIZE);
		if (NULL == pb)
		{
			NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: OOM in AllocateHTCPacketBuffers\n");
			break;
		}
		// Pass ownership of the buffer to HTC
		if ( A_OK != HTCBufferReceive(m_pHTCTarget, eid, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb)) {
			a_netbuf_free(pb);
		}
	}
}

void
CAR6KMini::ReturnPacket(
	IN PNDIS_PACKET Packet)
//
//  After the miniport passes packets up to NDIS via
//  NdisMIndicateReceivePackets, NDIS will some time later
//  return those packets (one at a time) to the miniport
//  by calling this ReturnPacket handler.
//
{
	NDIS_BUFFER *pBuffer;
	A_STATUS status = A_OK;

	// Remove the NDIS_BUFFER from the packet (should be exactly 1 buffer)
	NdisUnchainBufferAtFront(Packet, &pBuffer);

	// Return the NDIS_PACKET to our pool
	NdisFreePacket(Packet);

	// Return the pb containing the NDIS_BUFFER to the HTC layer
	// to receive a new packet
	ndis_mini_buf_t *pb = (ndis_mini_buf_t *)((PBYTE)pBuffer - offsetof(ndis_mini_buf_t, NdisBuffer));
	status = HTCBufferReceive(m_pHTCTarget, ENDPOINT2, a_netbuf_to_data(pb), AR6000_BUFFER_SIZE, pb);

	if ( status != A_OK ) {
		//HTC did'nt accept the buffer. Free it.
		a_netbuf_free(pb);
	}
}

/*****************************************************************************
  * isMcForUs - Filter multicast frames that are not wanted
  *
  * Description:
  * If we're not receiving all m/c pkts, filter such a pkt
  * through the NDIS-supplied list.
  * Note: Promiscuous mode is not currently supported
  */
 
 void
 CAR6KMini::isMcForUs(MAC_ADDRESS *pAddr, BOOL *mcForUs)
 {
     UINT i;
 	MAC_ADDRESS	     *tempAddr;
 
	if (m_CurrentPacketFilter & NDIS_PACKET_TYPE_ALL_MULTICAST) {
		*mcForUs = TRUE;
        return;
     }
 	 for (i = 0; i < m_numMulticastAddresses; i++) {
		 tempAddr=(MAC_ADDRESS *) m_MulticastAddresses[i];
		 if ((A_MACADDR_COMP(pAddr, tempAddr) == 0)) {
			 *mcForUs=TRUE;
             return;
         }
 	 }
	 *mcForUs = FALSE; 
     return;
 }
