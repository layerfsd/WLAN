//------------------------------------------------------------------------------
// <copyright file="c802_3mini.hpp" company="Atheros and Microsoft">
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
// NDIS 802.3 Miniport class.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#define ETHERNET_MAC_ADDRESS_LENGTH    6
#define ETH_HEADER_SIZE                14


class C802_3Miniport : public CMiniport
{
public:
    C802_3Miniport();
    virtual ~C802_3Miniport();

    virtual NDIS_STATUS         QueryInformation(IN NDIS_OID Oid, OUT PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbWritten, OUT PULONG pcbNeeded);
    virtual NDIS_STATUS         SetInformation(  IN NDIS_OID Oid, IN  PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbRead, OUT PULONG pcbNeeded);

    virtual NDIS_MEDIUM         GetSupportedNdisMediumType();
    virtual PNDIS_OID           GetSupportedOidList(OUT PNDIS_OID pOidBuffer, IN OUT PULONG pMaxOidsToAddToBuffer, IN OUT PULONG pTotalNumberOfSupportedOids);

    virtual ULONG               GetSupportedPacketTypes();

    // OID_GEN Query Handlers

    virtual NDIS_HARDWARE_STATUS  GetGenHardwareStatus();
    virtual NDIS_MEDIUM           GetGenMediaSupported();
    virtual NDIS_MEDIUM           GetGenMediaInUse();
    virtual ULONG                 GetGenMaximumLookahead();
    virtual ULONG                 GetGenCurrentLookahead();
    virtual ULONG                 GetGenMaximumFrameSize();
    virtual ULONG                 GetGenMaximumTotalSize();

    // OID_802_3 Query Handlers
    virtual BYTE *                Get802_3PermanentAddress() = NULL;
    virtual BYTE *                Get802_3CurrentAddress() = NULL;
    virtual void                  Get802_3MulticastList(OUT PBYTE *pMulticastAddreses, OUT PULONG pNumMulticastAddresses) = NULL;
    virtual ULONG                 Get802_3MaxMulticastListSize() = NULL;
    virtual ULONG                 Get802_3MacOptions() = NULL;
    virtual ULONG                 Get802_3RcvErrorAlignment() = NULL;
    virtual ULONG                 Get802_3XmitOneCollision() = NULL;
    virtual ULONG                 Get802_3XmitMoreCollision() = NULL;

    // OID_802_3 Set Handlers
    virtual NDIS_STATUS           Set802_3MulticastList(IN PBYTE pMulticastAddreses, IN ULONG NumMulticastAddresses) = NULL;
};

