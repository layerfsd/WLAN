//------------------------------------------------------------------------------
// <copyright file="car6ktx.cpp" company="Atheros and Microsoft">
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
// AR6000 NDIS Miniport driver packet transmit functions.
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
extern "C" {
#include "bmi.h"
}

#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include "a_osapi.h"
//#include "ar6k_drv.h"
#include "ieee80211_node.h"

#include "wmi_host.h"

#define _P	L"\r[AR6K : CAR6K_TX] "L


enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0,
}Car6kTxDebug;



NDIS_STATUS
CAR6KMini::TransmitBufferBuild(
	OUT AR6K_TRANSMIT_BUFFER *pTxBuffer,
	IN  NDIS_PACKET          *pNdisPacket) 
//
//  This function is called to convert the data in an NDIS_PACKET
//  into a contiguous block of data in the format required by the
//  AR6K Wireless Module.
//
//  The format of a packet sent to the Wireless Module looks like:
//		WMI_DATA_HDR:
//			UINT8	  reserved (always 0)
//			UINT8     msgType = 0 (data)
//      802_3_HDR:
//			UINT8[6]  dstMac
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
{
	NDIS_STATUS		Status = NDIS_STATUS_INVALID_DATA;
	PNDIS_BUFFER    pNdisBuffer;
	PBYTE           pBufferData;
	ULONG           cbBufferData;
	USHORT          EthLength;
	PBYTE           pTxData;
	ULONG           cbTxDataSpaceRemaining;
	boolean         bIsDIX = false;

	NDIS_DEBUG_PRINTF(0, "%s() + Enter \r\n", __FUNCTION__);

	NdisQueryPacketFirstBuffer(pNdisPacket, &pNdisBuffer);
	if (NULL == pNdisBuffer)
		goto done;

	// The first buffer should contain the 802.3 MAC header
	NdisQueryBuffer(pNdisBuffer, &pBufferData, &cbBufferData);
	if (cbBufferData < sizeof(ETHERNET_MAC_HEADER))
		goto done;

	pTxBuffer->pNdisPacket = pNdisPacket;

	WMI_DATA_HDR_SET_MSG_TYPE( &(pTxBuffer->WMIDataHeader), DATA_MSGTYPE);

	// Copy the source and destination MAC addresses
	memcpy(&pTxBuffer->MACHeader.DestMACAddress[0], pBufferData, ETHERNET_MAC_ADDRESS_LENGTH * 2);
	pBufferData += ETHERNET_MAC_ADDRESS_LENGTH * 2;
	cbBufferData -= ETHERNET_MAC_ADDRESS_LENGTH * 2;

	// Check the 2 bytes after the dest and source MAC addresses to see if
	// they are a length (802.3 format packet) or a type (DIX format)
	EthLength = (pBufferData[0] << 8) | pBufferData[1];
	if (EthLength <= 0x0600)
	{
		// Length field is the length in bytes,
		// Packet is already in 802.3 format
		pTxData = (PBYTE)&pTxBuffer->MACHeader.Length;

	}
	else
	{
		// Packet is in DIX format, we will need to insert length field and SNAP header
		// to convert it into the 802.3 format that the AR6K requires.
		bIsDIX = true;

		pTxBuffer->SNAPHeader.dsap = 0xAA;
		pTxBuffer->SNAPHeader.ssap = 0xAA;
		pTxBuffer->SNAPHeader.cntl = 0x03;
		pTxBuffer->SNAPHeader.orgCode[0] = 0;
		pTxBuffer->SNAPHeader.orgCode[1] = 0;
		pTxBuffer->SNAPHeader.orgCode[2] = 0;

		pTxData = (PBYTE)&pTxBuffer->SNAPHeader.Type;
	}

	cbTxDataSpaceRemaining = (&pTxBuffer->PayloadData[0] - pTxData) + m_MaxTransmitBufferPayloadDataLength;

	// Copy remaining data from first buffer, and data from
	// all remaining buffers in the NDIS_PaCKET, into the
	// contiguous WMI_DATA output buffer

	while (TRUE)
	{
		if (cbBufferData > cbTxDataSpaceRemaining)
		{
			Status = NDIS_STATUS_BUFFER_TOO_SHORT;
			break;
		}
		NdisMoveMemory(pTxData, pBufferData, cbBufferData);
		pTxData += cbBufferData;
		cbTxDataSpaceRemaining -= cbBufferData;

		NdisGetNextBuffer(pNdisBuffer, &pNdisBuffer);

		if (pNdisBuffer == NULL)
		{
			// Copied all the buffers. Fill in the Ethernet length field if needed
			if (bIsDIX)
			{
				// For an 802.3 packet, the EthLength field is the number of bytes in the Data payload field
				EthLength = pTxData - (PBYTE)&pTxBuffer->SNAPHeader;
				pTxBuffer->MACHeader.Length = A_CPU2BE16(EthLength);
			}
			pTxBuffer->cbWMIData = pTxData - (PBYTE)&pTxBuffer->WMIDataHeader;
			Status = NDIS_STATUS_SUCCESS;
			break;
		}

		NdisQueryBuffer(pNdisBuffer, &pBufferData, &cbBufferData);
	}

	NDIS_DEBUG_PRINTF(0, " 2 :: pTxBuffer->pNdisPacket = %x \r\n", pTxBuffer->pNdisPacket);
done:
	return Status;
}


NDIS_STATUS
CAR6KMini::TransmitBufferListAllocate()
//
//  Allocate m_MaxTransmitBuffers and put them on the m_TransmitBufferList
//
{
	NDIS_STATUS            Status = NDIS_STATUS_SUCCESS;
	PAR6K_TRANSMIT_BUFFER  pTransmitBuffer;
	ULONG                  cbTransmitBuffer = offsetof(AR6K_TRANSMIT_BUFFER, PayloadData) + m_MaxTransmitBufferPayloadDataLength;

	NDIS_DEBUG_PRINTF(DBG_TRACE, "%s() + Enter \r\n", __FUNCTION__);
	// ULONG align the size of a transmit buffer
	cbTransmitBuffer = (cbTransmitBuffer + 3) & ~3;

	// Allocate the block of storage to hold all the transmit buffers
	m_cbTransmitBufferArray = cbTransmitBuffer * m_MaxTransmitBuffers;
	m_pTransmitBufferArray = (PAR6K_TRANSMIT_BUFFER)A_MALLOC(m_cbTransmitBufferArray);
	
	if (m_pTransmitBufferArray == NULL) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "%s() : Buffer Alloc Err !!!!!!! \r\n", __FUNCTION__);	
		goto done;
	}
	// Put all the transmit buffers on the free list
	pTransmitBuffer = &m_pTransmitBufferArray[0];
	
	Lock();
	for (ULONG iBuffer = 0; iBuffer < m_MaxTransmitBuffers; iBuffer++)
	{
		pTransmitBuffer->pNdisPacket = NULL;
		pTransmitBuffer->cbWMIData = 0;
		InsertTailList(&m_TransmitBufferList, &pTransmitBuffer->node);
		pTransmitBuffer = (PAR6K_TRANSMIT_BUFFER)((PBYTE)pTransmitBuffer + cbTransmitBuffer);
	}
	Unlock();

done:
	return Status;
}

PAR6K_TRANSMIT_BUFFER 
CAR6KMini::TransmitBufferListRemoveHead()
//
//  This function is called to get the first available transmit buffer
//  from the free list. If none are available, then NULL will be returned.
//
{
	PAR6K_TRANSMIT_BUFFER pBuffer = NULL;

	NDIS_DEBUG_PRINTF(0, "%s() + Enter \r\n", __FUNCTION__);
    Lock();

	if (!IsListEmpty(&m_TransmitBufferList))
		pBuffer = (PAR6K_TRANSMIT_BUFFER)RemoveHeadList(&m_TransmitBufferList);

	Unlock();

	return pBuffer;
}

void 
CAR6KMini::TransmitBufferListInsertHead(
	PAR6K_TRANSMIT_BUFFER pBuffer)
//
//  This function is called to return a transmit buffer to the free list.
//
{
	NDIS_DEBUG_PRINTF(0, "%s() + Enter \r\n", __FUNCTION__);
	Lock();
	pBuffer->pNdisPacket = NULL;
	InsertHeadList(&m_TransmitBufferList, &pBuffer->node);
	Unlock();
}

PNDIS_PACKET 
CAR6KMini::TransmitNdisPacketListRemoveHead()
//
//  Get the first packet from the list of pending send packets.
//
{
	PNDIS_PACKET pPacket = NULL;
	PLIST_ENTRY pNode;

	NDIS_DEBUG_PRINTF(0, "%s() + Enter \r\n", __FUNCTION__);
    Lock();
	if (!IsListEmpty(&m_TransmitNdisPacketList))
	{
		pNode = RemoveHeadList(&m_TransmitNdisPacketList);
		pPacket = (PNDIS_PACKET)((PBYTE)pNode - offsetof(NDIS_PACKET, MiniportReservedEx));
	}
	Unlock();

	return pPacket;
}

void 
CAR6KMini::TransmitNdisPacketListInsertTail(
	PNDIS_PACKET pPacket)
//
//  This function is called to add a packet to the list of packets
//  waiting for transmit buffers to become available so they can be sent.
//
{
	PLIST_ENTRY pNode = (PLIST_ENTRY)pPacket->MiniportReservedEx;

	NDIS_DEBUG_PRINTF(0, "%s() + Enter \r\n", __FUNCTION__);
	Lock();
	InsertHeadList(&m_TransmitNdisPacketList, pNode);
	Unlock();
}


A_STATUS
CAR6KMini::Ar6000SendPkt(HTC_ENDPOINT_ID endPointId,PVOID osbuf, WORD PktType)
{

	A_STATUS Status;
	struct ar_cookie *cookie;
	ndis_mini_buf_t *tmp_osbuf;

	tmp_osbuf = (ndis_mini_buf_t *)osbuf;
	NDIS_DEBUG_PRINTF(0, "%s() endPointId = %d \r\n", __FUNCTION__, endPointId);

	Lock();	
	cookie = AllocCookie();

	if(cookie == NULL)
	{
		NDIS_DEBUG_PRINTF(DBG_ERR, " %s(), Cookie malloc failed !! \r\n",__FUNCTION__);
		return A_ERROR;
	}

	cookie->arc_bp[0] = (A_UINT32)osbuf;
	cookie->arc_bp[1] = 0;
	
	SET_HTC_PACKET_INFO_TX(&cookie->HtcPkt,
						   cookie,
						   A_NETBUF_DATA(osbuf),
						   A_NETBUF_LEN(osbuf),
						   endPointId,
						   PktType);
	
	
	Status = HTCSendPkt(m_hHtcTarget, &cookie->HtcPkt);
	Unlock();
	NDIS_DEBUG_PRINTF(0, "%s() ->[%02x][%02x][%02x] HtcSendPkt End\r\n", __FUNCTION__, 
								tmp_osbuf->buf_dat[0],tmp_osbuf->buf_dat[1],tmp_osbuf->buf_dat[2]);

	return Status;
}



void
CAR6KMini::SendNdisPacket(
	PNDIS_PACKET          pPacket,
	PAR6K_TRANSMIT_BUFFER pTxBuffer)
//
//  Send the specified NDIS_PACKET using the provided buffer.
//
{

	NDIS_DEBUG_PRINTF(0, " %s() --> need modify~~~~ !!! \r\n",__FUNCTION__);


	NDIS_STATUS Status;
	A_STATUS	htcStatus;
#ifdef WMM
	HTC_ENDPOINT_ID endPointId = ENDPOINT_UNUSED;
	//void *osbuf;
	ndis_mini_buf_t osbuf;
	A_UINT8 *pData = NULL;
	IEEE8021PPRIORITY ndisUserPriority;
#endif


	Status = TransmitBufferBuild(pTxBuffer, pPacket);

	if (NDIS_STATUS_SUCCESS == Status)
	{
	
#ifdef WMM
		/* wmi APIs work with buf structures. So create a dummy buf here to pass to wmi.
		 * The data pointer in the buf is hacked to point to the WMIDataHeader.
		 */
		 
		ndisUserPriority=(IEEE8021PPRIORITY)NDIS_PER_PACKET_INFO_FROM_PACKET(pPacket,Ieee8021pPriority);

		memset(&osbuf, 0, sizeof(ndis_mini_buf_t));
		osbuf.buf_dat = (PBYTE)&pTxBuffer->WMIDataHeader;
		
#endif
	
		// Send the data to the device
#ifdef WMM

	{
		struct ar_cookie *cookie;
		A_UINT8            ac = 99;
		HTC_ENDPOINT_ID    eid = ENDPOINT_UNUSED ;

		Lock();
		ac = wmi_implicit_create_pstream((wmi_t *)m_pWMI, &osbuf, UPLINK_TRAFFIC, (A_UINT32)ndisUserPriority);


		eid = Ac2EndpointID(ac);


		cookie = AllocCookie();
			
		if(cookie == NULL)
			NDIS_DEBUG_PRINTF(DBG_ERR, " %s() Memory Alloc Fail !!!! \r\n", __FUNCTION__);

		cookie->arc_bp[0] = (A_UINT32)pTxBuffer;

		SET_HTC_PACKET_INFO_TX(&cookie->HtcPkt,
							   cookie,
							   (PBYTE)&pTxBuffer->WMIDataHeader,
							   pTxBuffer->cbWMIData,
							   eid,
							   AR6K_DATA_PKT_TAG);

		NDIS_DEBUG_PRINTF(0," %s() pTxBuffer = 0x%x, ndisPacket = 0x%x \r\n", __FUNCTION__, pTxBuffer,pTxBuffer->pNdisPacket);

		htcStatus = HTCSendPkt(m_hHtcTarget, &cookie->HtcPkt);
		Unlock();
		
	}

#else
		htcStatus = HTCBufferSend(m_hHtcTarget, ENDPOINT2, (PBYTE)&pTxBuffer->WMIDataHeader, pTxBuffer->cbWMIData, pTxBuffer);
#endif
		if (A_PENDING == htcStatus)
			Status = NDIS_STATUS_PENDING;
		else if (A_OK == htcStatus)
			Status = NDIS_STATUS_PENDING; // HTCBufferSend seems to return A_OK even when the send pends
		else
			Status = NDIS_STATUS_FAILURE;
	}
	
//done:
	if (NDIS_STATUS_PENDING != Status)
	{
		// If the packet isn't queued, complete it immediately
		NDIS_DEBUG_PRINTF(DBG_ERR, "AR6K: NDIS - SEND Complete %x FAILURE=%x\n", pPacket, Status);
		NdisMSendComplete(m_MiniportAdapterHandle, pPacket, Status);
		TransmitBufferListInsertHead(pTxBuffer);
	}

}


void
CAR6KMini::FlushNdisPacketTransmitQueue()
{
    PNDIS_PACKET          pNdisPacket;

    NDIS_DEBUG_PRINTF(1, "AR6K: NDIS - Flushing ndis TX packets \r\n");

        /* unload NDIS packets waiting for free transmit buffers */
    while (1) {
        pNdisPacket = TransmitNdisPacketListRemoveHead();
        if (pNdisPacket != NULL) {
            NdisMSendComplete(m_MiniportAdapterHandle, pNdisPacket, NDIS_STATUS_FAILURE);
        } else {
            break;
        }
    }

}


void
CAR6KMini::HTCDataSendCompleted(
	PVOID cookie)
//
//  This function is called when an HTCBufferSend request completes on
//  the an endpoint used for data packets. It completes the send and
//  returns the transmit buffer back to the free list.
//
{
	PAR6K_TRANSMIT_BUFFER pTxBuffer = (PAR6K_TRANSMIT_BUFFER)cookie;
	PNDIS_PACKET          pNdisPacket = pTxBuffer->pNdisPacket;

	NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: NDIS - SEND Complete, %x \r\n", pNdisPacket);
	
	NdisMSendComplete(m_MiniportAdapterHandle, pNdisPacket, NDIS_STATUS_SUCCESS);

	pNdisPacket = TransmitNdisPacketListRemoveHead();

	if (pNdisPacket)
	{
		// pending list is not empty, use the buffer to send the first queued packet
		NDIS_DEBUG_PRINTF(DBG_TRACE, "%s() ############# \r\n", __FUNCTION__);
		SendNdisPacket(pNdisPacket, pTxBuffer);
	}
	else
	{
		// nothing pending to transmit, just add the buffer to the free list
		TransmitBufferListInsertHead(pTxBuffer);
	}

}

void
CAR6KMini::SendPackets(
	IN PPNDIS_PACKET PacketArray,
	IN UINT          NumberOfPackets)
//
//  This function is called by NDIS to send packets through
//  the adapter to the network.
//
{
	UINT                  i;
	PAR6K_TRANSMIT_BUFFER pTxBuffer;
	PNDIS_PACKET          pNdisPacket;

    if (!m_Connected) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "AR6K: NDIS - SEND Packet failed - Not connected to the network \r\n");
	    for (i = 0; i < NumberOfPackets; i++)
	    {
		    pNdisPacket = PacketArray[i];
			Lock();
	        NdisMSendComplete(m_MiniportAdapterHandle, pNdisPacket, NDIS_STATUS_FAILURE);
			Unlock();
        }
        return;
    }

	
	for (i = 0; i < NumberOfPackets; i++)
	{
		pNdisPacket = PacketArray[i];

		NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: NDIS - #### -> %x \r\n", pNdisPacket);
		
		pTxBuffer = TransmitBufferListRemoveHead();
		if (pTxBuffer == NULL)
		{
			// Out of transmit buffers, we will have to queue
			// up the remaining packets to be transmitted when
			// buffers become available.
			break;
		}

		SendNdisPacket(pNdisPacket, pTxBuffer);
	}

	// Add to the queue all the packets that were not transmitted in the
	// above loop because all the transmit buffers were in use.
	for (; i < NumberOfPackets; i++)
	{
		pNdisPacket = PacketArray[i];
		TransmitNdisPacketListInsertTail(pNdisPacket);
	}
	
}
