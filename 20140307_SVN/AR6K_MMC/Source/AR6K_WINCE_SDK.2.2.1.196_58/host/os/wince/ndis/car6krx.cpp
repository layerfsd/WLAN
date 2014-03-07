//------------------------------------------------------------------------------
// <copyright file="car6krx.cpp" company="Atheros and Microsoft">
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
// AR6000 NDIS Miniport received packet handling functions.
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
#include "wince_common_drv.h"
#include "crypto_api.h"

static const CAP_CONST bridgeTunnel = ETHERNET_TUNNEL;
static const CAP_CONST vrfc1042     = RFC1042SNAP;

void
CAR6KMini::ReceiveWMIDataPacket(HTC_PACKET *pHTCPacket)
//
//  This function processes data from an HTC_BUFFER_RECEIVED indication
//  not on the WMI_CONTROL_MBOX endpoint.
//
{
    ndis_mini_buf_t *pb = (ndis_mini_buf_t *)pHTCPacket->pPktContext;
    NDIS_STATUS      Status;
    NDIS_PACKET     *pPacket;
    NDIS_BUFFER     *pBuffer;
    PBYTE            pData;
    ULONG            cbData;
    BOOL             doDix = FALSE;
    USHORT           etherType;
    SNAP_HEADER      *pSnapHdr;
    MAC_ADDRESS      *pDestAddr,*tempAddr,*pSrcAddr;
    BOOL             mcForUs = FALSE;
    BOOL             bisAPUnicast = FALSE;
    A_UINT8          userPriority=0;
    STATION          *Station = NULL;

    UINT32           *magic;
    A_UINT16         powState = 0;
    A_UINT8          buf[512];
    BYTE  stationaddr[ETHERNET_MAC_ADDRESS_LENGTH];

    NDIS_DEBUG_PRINTF(ATH_LOG_TRC | ATH_DEBUG_RECV, "AR6K: +ReceiveWMIDataPacket");


    if (pHTCPacket->Status != A_OK) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - ReceiveWMIPacket Error in receiving : Status = %x\n", pHTCPacket->Status);
	logPrintf( (debugFileLog && debugErrorLog), "ReceiveWMIDataPacket(): bad packet ignored\r\n" );
        HTC_PACKET_RESET_RX(pHTCPacket);
        HTCAddReceivePkt(m_pHTCTarget, pHTCPacket);
        goto done;
    }

    // evInfo->actualLength is the length of the pb->data including
    //      2 bytes: WMI_DATA_HEADER [optional - only if HTC header is 46 00]
    //     14 bytes: 802.3 MAC header
    //      8 bytes: SNAP header (with EthType as last 2 bytes)
    //      N bytes: payload (e.g. IP packet)
    pData = pHTCPacket->pBuffer;
    cbData = pHTCPacket->ActualLength;

    if(AP_NETWORK == m_networkType) {
            powState = (((WMI_DATA_HDR *)pData)->info >> WMI_DATA_HDR_PS_SHIFT) & WMI_DATA_HDR_PS_MASK;
    }

    // Remove the WMI_DATA_HDR.
    if (cbData < sizeof(WMI_DATA_HDR))
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - ReceiveWMIPacket missing WMI header (%u bytes)\n", pHTCPacket->ActualLength);
	logPrintf( (debugFileLog && debugErrorLog), "ReceiveWMIDataPacket(): bad packet ignored: bad WMI header\r\n" );
        HTC_PACKET_RESET_RX(pHTCPacket);
        HTCAddReceivePkt(m_pHTCTarget, pHTCPacket);
        goto done;
    }
    pData += sizeof(WMI_DATA_HDR);
    cbData -= sizeof(WMI_DATA_HDR);

    if(m_networkType != AP_NETWORK) {
        if (cbData < sizeof(ETHERNET_MAC_HEADER) + sizeof(SNAP_HEADER))
        {
            NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - ReceiveWMIPacket missing MAC + SNAP (%u bytes)\n", cbData);
	logPrintf( (debugFileLog && debugErrorLog), "ReceiveWMIDataPacket(): bad packet ignored: bad MAC+SNAP header\r\n" );
            HTC_PACKET_RESET_RX(pHTCPacket);
            HTCAddReceivePkt(m_pHTCTarget, pHTCPacket);

            goto done;
        }
    }

    /* Extract the User Priority from the WMI Hdr */
    userPriority = (((WMI_DATA_HDR *)A_NETBUF_DATA(pb))->info >> WMI_DATA_HDR_UP_SHIFT) & WMI_DATA_HDR_UP_MASK;

    pDestAddr=(MAC_ADDRESS *)pData;
    pSrcAddr=(MAC_ADDRESS *)(pData + ETHERNET_MAC_ADDRESS_LENGTH);
    memcpy(stationaddr,pSrcAddr,ETHERNET_MAC_ADDRESS_LENGTH);
    if(m_networkType == AP_NETWORK) {
        car6k_ap_rx_pwsave_flushpkt (pSrcAddr, powState);
        /* Drop the NULL Frame */
        if(pHTCPacket->ActualLength < (sizeof (WMI_DATA_HDR) + sizeof(ATH_MAC_HDR) +
                    sizeof(ATH_LLC_SNAP_HDR))) {
            HTC_PACKET_RESET_RX(&pb->HtcPacket);
            HTCAddReceivePkt(m_pHTCTarget, &pb->HtcPacket);
            goto done;
        }
    }


    if(m_networkType != AP_NETWORK) {
        /* Apply NDIS receive filter */
        if (isGrp(pDestAddr)) {
            if (isBcast(pDestAddr)) {
                if (!(m_CurrentPacketFilter & NDIS_PACKET_TYPE_BROADCAST)) {
                    HTC_PACKET_RESET_RX(pHTCPacket);
                    HTCAddReceivePkt(m_pHTCTarget, pHTCPacket);
                    goto done;
                }
            } else {
                isMcForUs(pDestAddr,&mcForUs);
                if (!(m_CurrentPacketFilter & (NDIS_PACKET_TYPE_MULTICAST |
                                NDIS_PACKET_TYPE_ALL_MULTICAST)) || !(mcForUs))
                {
                    HTC_PACKET_RESET_RX(pHTCPacket);
                    HTCAddReceivePkt(m_pHTCTarget, pHTCPacket);
                    goto done;
                }
            }

        } else {
            tempAddr=(MAC_ADDRESS *) m_PermanentAddress;
            if ((A_MACADDR_COMP(pDestAddr,tempAddr) != 0) &&
                    !(m_CurrentPacketFilter & NDIS_PACKET_TYPE_PROMISCUOUS))
            {
                HTC_PACKET_RESET_RX(pHTCPacket);
                HTCAddReceivePkt(m_pHTCTarget, pHTCPacket);
                goto done;
            }
        }
    }

     // Allocate an NDIS_PACKET from our packet pool.
    NdisAllocatePacket(&Status, &pPacket, m_RxPacketPool);
    if (NDIS_STATUS_SUCCESS != Status)
    {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - NdisAllocatePacket failed\n");
	logPrintf( (debugFileLog && debugErrorLog), "ReceiveWMIDataPacket(): NdisAllocatePacket failed\r\n" );
        HTC_PACKET_RESET_RX(pHTCPacket);
        HTCAddReceivePkt(m_pHTCTarget, pHTCPacket);
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

    if ((ETHERTYPE_EAPOL == etherType) && (AP_NETWORK == m_networkType))
    {
        memcpy (buf, pData, cbData);
        car6k_ap_rx_process_EAPOL (stationaddr, buf);
    }

    //builder: debug print eapol rx
    if( ETHERTYPE_EAPOL == etherType )
    {
        char szbuf[128];
	int i;
	EAPOL_HEADER *epHdr;
        EAPOL_KEY_WPA *epKeyWpa;
        memcpy (buf, pData, cbData);
        epHdr = (EAPOL_HEADER *)(buf + 14);
        if (epHdr->type == EAPOL_TYPE_KEYMSG) 
		{
                epKeyWpa = (EAPOL_KEY_WPA *)(epHdr + 1);
		 for( i=0; i<32; i++ )
		    	{
		    	sprintf( szbuf+(i*2), "%02x", epKeyWpa->keyNonce[i] );
		    	}
		RETAILMSG(1, (TEXT("builderd:ar6k2:RX eapol: WPA Key Nonce= %S\r\n"), szbuf));
		logPrintf( debugFileLog, "builder:ar6k2:RX eapol: WPA Key Nonce= %s\r\n", szbuf );
		for( i=0; i<16; i++ )
		    	{
		    	sprintf( szbuf+(i*2), "%02x", epKeyWpa->keyIV[i] );
		    	}
		RETAILMSG(1, (TEXT("builderd:ar6k2:RX eapol: WPA Key IV= %S\r\n"), szbuf));
		logPrintf( debugFileLog, "builder:ar6k2:RX eapol: WPA Key IV= %s\r\n", szbuf );
		for( i=0; i<16; i++ )
		    	{
		    	sprintf( szbuf+(i*2), "%02x", epKeyWpa->keyMIC[i] );
		    	}
		RETAILMSG(1, (TEXT("builderd:ar6k2:RX eapol: WPA Key MIC= %S\r\n"), szbuf));	
		logPrintf( debugFileLog, "builder:ar6k2:RX eapol: WPA Key MIC= %s\r\n", szbuf );
                }
    }
	

    // Setup the fields of NDIS_BUFFER to point to our data.
    pBuffer = &pb->NdisBuffer;
    NdisInitializeBuffer(pBuffer, pData, cbData);

    // Chain the NDIS_BUFFER to the start of the NDIS_PACKET
    NdisChainBufferAtBack(pPacket, pBuffer);
    NDIS_SET_PACKET_HEADER_SIZE(pPacket, sizeof(ETHERNET_MAC_HEADER));
    NDIS_SET_PACKET_STATUS(pPacket, NDIS_STATUS_SUCCESS);
    // Setup the NDIS User Priority in the NDIS_PER_PACKET_INFO.
    // Commenting this out for now since we are not sure of the effect of this.
    // Refer Bug#33504
    //NDIS_PER_PACKET_INFO_FROM_PACKET(pPacket,Ieee8021pPriority) = (PVOID)userPriority;

    if((m_networkType == AP_NETWORK) && m_Connected && m_ap_intrabss) {
        magic = (UINT32 *) pPacket->ProtocolReserved;
        car6k_process_ap_rx (magic,pData,cbData,&bisAPUnicast,
                             pDestAddr,pPacket);
    }

    if (AP_NETWORK == m_networkType)
    {
        if (FALSE == bisAPUnicast)
        {
            NDIS_PACKET *PacketArray[1];
            PacketArray[0]=pPacket;
            NdisMIndicateReceivePacket(m_MiniportAdapterHandle, PacketArray, 1);
        }
        goto done;
    }

    if (m_DirectRx2Ndis)
    {
        NDIS_PACKET *PacketArray[1];
        PacketArray[0]=pPacket;

        //
        // WAR Due to RTP stack issue with MSFT, the following delay is needed
        // Otherwise, WMM Cert RTP throughput tests wont reach even 1mbps
        // This delya is only applicable in directRx2NDIS flow
        //
        NdisMSleep (1);
        NdisMIndicateReceivePacket(m_MiniportAdapterHandle, PacketArray, 1);
    }
    else
    {
        A_BOOL  indicate = TRUE;
        // Perform the indicate on the timer thread because tying up the
        // SDIO receive indication thread can result in a deadlock.
        PLIST_ENTRY pEntry = (PLIST_ENTRY)(pPacket->MiniportReserved);

        Lock();
		InsertTailList(&m_RxPendingPacketList, pEntry);

        if (m_Config.delayedRxIndThresh) {
            /* a little optimization here :
             *   check if HTC indicates that there are more RX packets comming down on this endpoint
             *   If there are, check our current count and delay the indication to the RX thread.
             *   This is intended to minimize thread trashing. */
            m_RxIndicationPendingCount++;
            if (!(pHTCPacket->PktInfo.AsRx.IndicationFlags & HTC_RX_FLAGS_INDICATE_MORE_PKTS) ||
                (m_RxIndicationPendingCount >= (LONG)m_Config.delayedRxIndThresh)) {
                m_RxIndicationPendingCount = 0;
                indicate = TRUE;
            } else {
                    /* not ready to indicate */
                indicate = FALSE;
            }
        }

        Unlock();

//	RETAILMSG(1, (TEXT("builderd:ar6k2:PendingRXPacket = %x\r\n"), (A_UINT32)pPacket));

        if (indicate) {
            NdisSetEvent(&m_RxPendingEvent);
        }
	}

done:
    NDIS_DEBUG_PRINTF(ATH_LOG_TRC | ATH_DEBUG_RECV, "AR6K: -ReceiveWMIDataPacket");
}

#define MAX_PACKETS_PER_INDICATE    16

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

        NdisResetEvent(&m_RxPendingEvent);

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
//		RETAILMSG(1, (TEXT("builderd:ar6k2:RXindicatePacket = %x\r\n"), (A_UINT32)pPacket));
            if (NumPacketsToIndicate == MAX_PACKETS_PER_INDICATE) {
                Unlock();
                NdisMIndicateReceivePacket(m_MiniportAdapterHandle, PacketArray, NumPacketsToIndicate);
                Lock();
                NumPacketsToIndicate = 0;
            }
        }
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
CAR6KMini::AllocateHTCPacketBuffers(HTC_ENDPOINT_ID  eid, UINT numBuffers)

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
        SET_HTC_PACKET_INFO_RX_REFILL(&pb->HtcPacket,
                                      pb,
                                      A_NETBUF_DATA(pb),
                                      AR6000_BUFFER_SIZE,
                                      eid);
        HTCAddReceivePkt(m_pHTCTarget, &pb->HtcPacket);
    }
	//RETAILMSG(1, (TEXT("builderd:ar6k2:AllocateHTCPacketBuffers[%d] = %d\r\n"), eid, i));
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

    if((TAG_MCAST_PCK_PS == (*(UINT32 *)Packet->ProtocolReserved)) ||
       (TAG_MCAST_PCK    == (*(UINT32 *)Packet->ProtocolReserved)) ||
       (TAG_WPA_SEND_PCK == (*(UINT32 *)Packet->ProtocolReserved)))
    {
        // Return the NDIS_PACKET to our pool
        //RETAILMSG(1, (TEXT("builderd:ar6k2:ReturnPacket = %x\r\n"), (A_UINT32)Packet));
        NdisFreePacket(Packet);
        NdisFreeBuffer(pBuffer);
    }else {
    	//RETAILMSG(1, (TEXT("builderd:ar6k2:ReturnPacket = %x\r\n"), (A_UINT32)Packet));
        // Return the NDIS_PACKET to our pool
        NdisFreePacket(Packet);

        // Return the pb containing the NDIS_BUFFER to the HTC layer
        // to receive a new packet
        ndis_mini_buf_t *pb = (ndis_mini_buf_t *)((PBYTE)pBuffer - offsetof(ndis_mini_buf_t, NdisBuffer));

        HTC_PACKET_RESET_RX(&pb->HtcPacket);
        HTCAddReceivePkt(m_pHTCTarget, &pb->HtcPacket);
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
    MAC_ADDRESS      *tempAddr;

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
