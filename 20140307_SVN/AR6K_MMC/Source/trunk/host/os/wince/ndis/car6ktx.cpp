//------------------------------------------------------------------------------
// <copyright file="car6ktx.cpp" company="Atheros and Microsoft">
//    Copyright (c) 2004-2007 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
// 
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
//------------------------------------------------------------------------------
//==============================================================================
// AR6000 NDIS Miniport driver packet transmit functions.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#include <windows.h>
#include <ndis.h>

#include "athdefs.h"
#include "a_types.h"
#include "a_osapi.h"
#include "htc_api.h"
#include "hif.h"
#include "wmi.h"
#include "wlan_api.h"
#include "wmi_api.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
#include "common_drv.h"

extern "C" {
#include "bmi.h"
}

#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include "a_debug.h"

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
{
    NDIS_STATUS     Status = NDIS_STATUS_INVALID_DATA;
    PNDIS_BUFFER    pNdisBuffer;
    PBYTE           pBufferData;
    ULONG           cbBufferData;
    USHORT          EthLength;
    PBYTE           pTxData;
    ULONG           cbTxDataSpaceRemaining;
    boolean         bIsDIX = false;
        WMI_DATA_HDR *wmiDHead;

    NdisQueryPacketFirstBuffer(pNdisPacket, &pNdisBuffer);
    if (NULL == pNdisBuffer)
        goto done;

    // The first buffer should contain the 802.3 MAC header
    NdisQueryBuffer(pNdisBuffer, &pBufferData, &cbBufferData);
    if (cbBufferData < sizeof(ETHERNET_MAC_HEADER))
        goto done;

    // memzero the buffer before initializing it.
    A_MEMZERO(pTxBuffer, sizeof(AR6K_TRANSMIT_BUFFER));

    pTxBuffer->MgtInfo.pNdisPacket = pNdisPacket;

    WMI_DATA_HDR_SET_MSG_TYPE( &(pTxBuffer->DataFrame.WMIDataHeader), DATA_MSGTYPE);

    if(m_networkType == AP_NETWORK){
        wmiDHead =  &(pTxBuffer->DataFrame.WMIDataHeader);
        if(m_hasMoreData){
            WMI_DATA_HDR_SET_MORE_BIT(wmiDHead);
            m_hasMoreData = FALSE;
        }
        wmiDHead->rssi = 0;
    }


    // Copy the source and destination MAC addresses
    memcpy(&pTxBuffer->DataFrame.MACHeader.DestMACAddress[0], pBufferData, ETHERNET_MAC_ADDRESS_LENGTH);
    pBufferData += ETHERNET_MAC_ADDRESS_LENGTH;
    memcpy(&pTxBuffer->DataFrame.MACHeader.SrcMACAddress[0], pBufferData, ETHERNET_MAC_ADDRESS_LENGTH);
    pBufferData += ETHERNET_MAC_ADDRESS_LENGTH;
    cbBufferData -= ETHERNET_MAC_ADDRESS_LENGTH * 2;

    // Check the 2 bytes after the dest and source MAC addresses to see if
    // they are a length (802.3 format packet) or a type (DIX format)
    EthLength = (pBufferData[0] << 8) | pBufferData[1];
    if (EthLength <= 0x0600)
    {
        // Length field is the length in bytes,
        // Packet is already in 802.3 format
        pTxData = (PBYTE)&pTxBuffer->DataFrame.MACHeader.Length;

    }
    else
    {
        // Packet is in DIX format, we will need to insert length field and SNAP header
        // to convert it into the 802.3 format that the AR6K requires.
        bIsDIX = true;

        pTxBuffer->DataFrame.SNAPHeader.dsap = 0xAA;
        pTxBuffer->DataFrame.SNAPHeader.ssap = 0xAA;
        pTxBuffer->DataFrame.SNAPHeader.cntl = 0x03;
        pTxBuffer->DataFrame.SNAPHeader.orgCode[0] = 0;
        pTxBuffer->DataFrame.SNAPHeader.orgCode[1] = 0;
        pTxBuffer->DataFrame.SNAPHeader.orgCode[2] = 0;

        pTxData = (PBYTE)&pTxBuffer->DataFrame.SNAPHeader.Type;
    }

    cbTxDataSpaceRemaining = (&pTxBuffer->DataFrame.PayloadData[0] - pTxData) + m_MaxTransmitBufferPayloadDataLength;

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
                EthLength = pTxData - (PBYTE)&pTxBuffer->DataFrame.SNAPHeader;
                pTxBuffer->DataFrame.MACHeader.Length = A_CPU2BE16(EthLength);
            }
            pTxBuffer->MgtInfo.cbWMIData = pTxData - (PBYTE)&pTxBuffer->DataFrame.WMIDataHeader;
            Status = NDIS_STATUS_SUCCESS;
            break;
        }

        NdisQueryBuffer(pNdisBuffer, &pBufferData, &cbBufferData);
    }

done:
    return Status;
}

#define DMA_ALIGNMENT_PAD  TX_BUFFER_ALIGNMENT_BYTES
#define DMA_ALIGNMENT_MASK (DMA_ALIGNMENT_PAD - 1)


NDIS_STATUS
CAR6KMini::TransmitBufferListAllocate()
//
//  Allocate m_MaxTransmitBuffers and put them on the m_TransmitBufferList
//
{
    NDIS_STATUS            Status = NDIS_STATUS_SUCCESS;
    PAR6K_TRANSMIT_BUFFER  pTransmitBuffer;
    ULONG                  cbTransmitBuffer =
        sizeof(AR6K_TRANSMIT_BUFFER) + m_MaxTransmitBufferPayloadDataLength - 1;

    cbTransmitBuffer = (cbTransmitBuffer + DMA_ALIGNMENT_PAD) & (~DMA_ALIGNMENT_MASK);

    if ((cbTransmitBuffer * m_MaxTransmitBuffers + DMA_ALIGNMENT_PAD) > ULONG_MAX)
    {
        goto done;
    }

    m_cbTransmitBufferArray = cbTransmitBuffer * m_MaxTransmitBuffers + DMA_ALIGNMENT_PAD;
    m_pTransmitBufferArray = (PAR6K_TRANSMIT_BUFFER)A_MALLOC(m_cbTransmitBufferArray);
    if (m_pTransmitBufferArray == NULL)
        goto done;

    // Put all the transmit buffers on the free list
    if ((ULONG)m_pTransmitBufferArray & DMA_ALIGNMENT_MASK) {
            /* shift buffer, A_MALLOC only guarantees  32-bit alignment */
        pTransmitBuffer = (PAR6K_TRANSMIT_BUFFER)
                           (((DWORD)m_pTransmitBufferArray + DMA_ALIGNMENT_PAD) & ~DMA_ALIGNMENT_MASK);
    } else {
        pTransmitBuffer = &m_pTransmitBufferArray[0];
    }

    for (ULONG iBuffer = 0; iBuffer < m_MaxTransmitBuffers; iBuffer++)
    {
        pTransmitBuffer->MgtInfo.pNdisPacket = NULL;
        pTransmitBuffer->MgtInfo.cbWMIData = 0;
        InsertTailList(&m_TransmitBufferList, &pTransmitBuffer->MgtInfo.node);
        pTransmitBuffer = (PAR6K_TRANSMIT_BUFFER)((PBYTE)pTransmitBuffer + cbTransmitBuffer);
    }

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
    Lock();
    pBuffer->MgtInfo.pNdisPacket = NULL;
    InsertHeadList(&m_TransmitBufferList, &pBuffer->MgtInfo.node);
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

    Lock();
    InsertHeadList(&m_TransmitNdisPacketList, pNode);
    Unlock();
}

void
CAR6KMini::SendNdisPacket(
    PNDIS_PACKET          pPacket,
    PAR6K_TRANSMIT_BUFFER pTxBuffer)
//
//  Send the specified NDIS_PACKET using the provided buffer.
//
{
    NDIS_STATUS Status;
    BOOL        micErrReportFrame;
    BOOL        bIsSendToHTC;
    A_STATUS    htcStatus = A_ERROR;
    HTC_ENDPOINT_ID endpointID;
    A_UINT8   accessClass;
    ndis_mini_buf_t osbuf;
    IEEE8021PPRIORITY ndisUserPriority;
    A_UINT8 *pData = NULL;

    Status = TransmitBufferBuild(pTxBuffer, pPacket);
    if (NDIS_STATUS_SUCCESS == Status)
    {
        ndisUserPriority=(IEEE8021PPRIORITY)NDIS_PER_PACKET_INFO_FROM_PACKET(pPacket,Ieee8021pPriority);

		/* wmi APIs work with buf structures. So create a dummy buf here to
		 * pass to wmi.
		 * The data pointer in the buf is hacked to point to the
		 * WMIDataHeader.
		 */
        memset(&osbuf, 0, sizeof(ndis_mini_buf_t));

        osbuf.buf_dat = (PBYTE)&pTxBuffer->DataFrame.WMIDataHeader;

        accessClass = wmi_implicit_create_pstream((wmi_t *)m_pWMI, &osbuf, (A_UINT32)ndisUserPriority, m_Config.wmmConfig);

		// Send the data to the device
        endpointID = arAc2EndpointID(accessClass);

        micErrReportFrame = FALSE;
        if(m_networkType != AP_NETWORK) {
            if (A_BE2CPU16(pTxBuffer->DataFrame.SNAPHeader.Type) == ETHERTYPE_EAPOL)
            {
                EAPOL_HEADER *epHdr;
                EAPOL_KEY_WPA *epKeyWpa;
                EAPOL_KEY_INFO epKeyInfo;
                epHdr = (EAPOL_HEADER *)&pTxBuffer->DataFrame.PayloadData[0];
                if (epHdr->type == EAPOL_TYPE_KEYMSG) {
                    epKeyWpa = (EAPOL_KEY_WPA *)(epHdr + 1);
                    if (epKeyWpa->descType == EAPOL_KEY_TYPE_WPA ||
                            epKeyWpa->descType == EAPOL_KEY_TYPE_WPA2) {
                        epKeyInfo.u.word = A_BE2CPU16(epKeyWpa->keyInfo.u.word);
                        if ((epKeyInfo.u.k.mic == EKI_MIC_SET ) &&
                                (epKeyInfo.u.k.error == EKI_ERROR_SET) &&
                                (epKeyInfo.u.k.request == EKI_REQUEST_SET)) {
                            micErrReportFrame = TRUE;
                        }
                    }
                }
            }
        }

        SET_HTC_PACKET_INFO_TX(&pTxBuffer->MgtInfo.HtcPacket,
                               pTxBuffer,
                              (PBYTE)&pTxBuffer->DataFrame.WMIDataHeader,
                               pTxBuffer->MgtInfo.cbWMIData,
                               endpointID,
                               AR6K_DATA_PKT_TAG);

        bIsSendToHTC = (!m_CounterMeasureOn) || micErrReportFrame;

        if (bIsSendToHTC)
        {
            htcStatus = HTCSendPkt (m_pHTCTarget, &pTxBuffer->MgtInfo.HtcPacket);
        }

        if(m_CounterMeasureOn) {
            if(micErrReportFrame) {
                //
                // mark the connected peerBssid is bad
                //
                wmi_addBadAp_cmd ((wmi_t *)m_pWMI, 0x00/*index*/, m_PeerBSSID);
                A_TIMEOUT_MS(&m_CounterMeasureExitTimer, MIC_ERROR_BLOCKOUT_MSEC, 0);
            }
        }

        if (bIsSendToHTC)
            Status = NDIS_STATUS_PENDING;
        else
            Status = NDIS_STATUS_FAILURE;
    }

    if (NDIS_STATUS_PENDING != Status)
    {
        // If the packet isn't queued, complete it immediately
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: NDIS - SEND Complete %x FAILURE=%x\n", pPacket, Status);
        if(pTxBuffer->MgtInfo.pNdisPacket) {
            if((TAG_UCAST_PCK == (*(UINT32 *)pPacket->ProtocolReserved))||
            (TAG_MCAST_PCK_PS == (*(UINT32 *)pPacket->ProtocolReserved))||
		    (TAG_MCAST_PCK    == (*(UINT32 *)pPacket->ProtocolReserved))){
                ReturnPacket(pPacket);
            }else{
                NdisMSendComplete(m_MiniportAdapterHandle, pPacket, Status);
            }
            pTxBuffer->MgtInfo.pNdisPacket = NULL;
        }
        TransmitBufferListInsertHead(pTxBuffer);
    }
}

void
CAR6KMini::FlushNdisPacketTransmitQueue()
{
    PNDIS_PACKET          pNdisPacket;

    NDIS_DEBUG_PRINTF(ATH_DEBUG_SEND, "AR6K: NDIS - Flushing ndis TX packets \n");

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
    PAR6K_TRANSMIT_BUFFER pTxBuffer     = (PAR6K_TRANSMIT_BUFFER)cookie;
    PNDIS_PACKET          pNdisPacket   = pTxBuffer->MgtInfo.pNdisPacket;
    A_STATUS              htcStatus     = pTxBuffer->MgtInfo.HtcPacket.Status;

    NDIS_STATUS           Status        = (htcStatus == A_OK)?NDIS_STATUS_SUCCESS:NDIS_STATUS_FAILURE;
	UINT32          	  magic;

    NDIS_DEBUG_PRINTF(ATH_DEBUG_SEND, "AR6K: NDIS - SEND Complete %x htcStatus %x\n", pNdisPacket, htcStatus);

    if(pNdisPacket)
    {
        magic = (*(UINT32 *)pNdisPacket->ProtocolReserved);
        if((TAG_MCAST_PCK_PS != magic ) && (TAG_MCAST_PCK != magic ) && (TAG_UCAST_PCK != magic ) && (TAG_WPA_SEND_PCK != magic))
        {
            NdisMSendComplete(m_MiniportAdapterHandle, pNdisPacket, Status);
        }else
        {
            ReturnPacket(pNdisPacket); // packet to Asso-STA
        }
    }


    if (A_ECANCELED == htcStatus) {
            /* just free the packet */
        TransmitBufferListInsertHead(pTxBuffer);
            /* stop packet flow, HTC is shutting down */
        return;
    }

    pNdisPacket = TransmitNdisPacketListRemoveHead();
    if (pNdisPacket)
    {
        // pending list is not empty, use the buffer to send the first queued packet
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
    A_BOOL status = TRUE;
    if (!m_Connected || !m_HTCStarted) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: NDIS - SEND Packet failed - Not connected to the network \n");
        for (i = 0; i < NumberOfPackets; i++)
        {
            pNdisPacket = PacketArray[i];
            NdisMSendComplete(m_MiniportAdapterHandle, pNdisPacket, NDIS_STATUS_FAILURE);
        }
        return;
    }

    for (i = 0; i < NumberOfPackets; i++)
    {
        pNdisPacket = PacketArray[i];
        NDIS_DEBUG_PRINTF(ATH_DEBUG_SEND, "AR6K: NDIS - SEND Packet %x\n", pNdisPacket);

        if(m_networkType == AP_NETWORK) {
            status = car6k_ap_tx_pwsave_qpacket (pNdisPacket);
            if (status != TRUE)
            {
                return;
            }
        }

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
