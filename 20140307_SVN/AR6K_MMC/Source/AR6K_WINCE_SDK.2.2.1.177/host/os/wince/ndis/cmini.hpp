//------------------------------------------------------------------------------
// <copyright file="cmini.hpp" company="Atheros and Microsoft">
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
// NDIS Miniport class.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#define ETHERNET_MAC_ADDRESS_LENGTH    6
#define ETH_HEADER_SIZE                14

typedef struct
{
    NDIS_OID             Oid;
    ULONG                MinBufferSize;
} MPOidInfo, *PMPOidInfo;


class CMiniport
{
protected:
    //
    // This is the handle given by the wrapper for calling ndis
    // functions.
    //
    NDIS_HANDLE m_MiniportAdapterHandle;

    // Lock used to synchronize access to miniport data
    NDIS_SPIN_LOCK                      m_Lock;

    DWORD m_CurrentPacketFilter;
    DWORD m_CurrentLookahead;
    DWORD m_ProtocolOptions;

public:
    CMiniport();
    virtual ~CMiniport();

    __inline void               Lock() {NdisAcquireSpinLock(&m_Lock);}
    __inline void               Unlock() {NdisReleaseSpinLock(&m_Lock);}
    PVOID                       MemoryAllocate(IN UINT Length);
    void                        MemoryFree(IN PVOID pMemory);

    virtual NDIS_STATUS         Initialize(IN NDIS_HANDLE MiniportAdapterHandle, IN NDIS_HANDLE pConfigHandle);
    virtual void                Halt();
    virtual BOOLEAN             CheckForHang() = NULL;
    virtual NDIS_STATUS         Reset(OUT PBOOLEAN pAddressingReset) = NULL;
    virtual NDIS_STATUS         QueryInformation(IN NDIS_OID Oid, OUT PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbWritten, OUT PULONG pcbNeeded);
    virtual NDIS_STATUS         SetInformation(  IN NDIS_OID Oid, IN  PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbRead, OUT PULONG pcbNeeded);

    virtual NDIS_MEDIUM         GetSupportedNdisMediumType() = NULL;
    virtual void                SendPackets(IN PPNDIS_PACKET PacketArray, IN UINT NumberOfPackets) = NULL;
    virtual void                ReturnPacket(IN PNDIS_PACKET Packet) = NULL;

    NDIS_STATUS                 CheckOidRequest(
                                    IN     MPOidInfo               *pOidInfo,
                                    IN     NDIS_OID                 Oid,
                                    IN     ULONG                    BufferLength,
                                    OUT    PULONG                   BytesNeeded);

    void                        AddOidsToList(
                                    IN     MPOidInfo               *pOidInfo,
                                    IN OUT PNDIS_OID                pCurrentList,
                                    IN OUT PULONG                   pCurrentListCount,
                                    IN OUT PULONG                   pOidSlotsAvailable,
                                    IN OUT PULONG                   TotalOidCount);

    virtual PNDIS_OID           GetSupportedOidList(OUT PNDIS_OID pOidBuffer, IN OUT PULONG pMaxOidsToAddToBuffer, IN OUT PULONG pTotalNumberOfSupportedOids);

    // Return the types of packets supported by the miniport,
    // used in conjuction with OID_GEN_CURRENT_PACKET_FILTER requests.
    virtual ULONG                 GetSupportedPacketTypes() = NULL;

    // OID_GEN Query Handlers

    virtual ULONG                 GetGenVendorDriverVersion() = NULL;
    virtual ULONG                 GetGenMacOptions() = NULL;
    virtual NDIS_HARDWARE_STATUS  GetGenHardwareStatus() = NULL;
    virtual NDIS_MEDIUM           GetGenMediaSupported() = NULL;
    virtual NDIS_MEDIUM           GetGenMediaInUse() = NULL;
    virtual NDIS_PHYSICAL_MEDIUM  GetGenPhysicalMedium() = NULL;
    virtual NDIS_MEDIA_STATE      GetGenMediaConnectStatus() = NULL;
    virtual ULONG                 GetGenMaximumLookahead() = NULL;
    virtual ULONG                 GetGenCurrentLookahead() = NULL;
    virtual ULONG                 GetGenMaximumFrameSize() = NULL;
    virtual ULONG                 GetGenMaximumTotalSize() = NULL;
    virtual ULONG                 GetGenLinkSpeed() = NULL;
    virtual ULONG                 GetGenTransmitBufferSpace() = NULL;
    virtual ULONG                 GetGenReceiveBufferSpace() = NULL;
    virtual ULONG                 GetGenTransmitBlockSize() = NULL;
    virtual ULONG                 GetGenReceiveBlockSize() = NULL;
    virtual ULONG                 GetGenVendorId();
    virtual const char *          GetGenVendorDescription();
    virtual USHORT                GetGenDriverVersion() = NULL;
    virtual ULONG                 GetGenCurrentPacketFilter();
    virtual ULONG                 GetGenXmitOk() = NULL;
    virtual ULONG                 GetGenRcvOk() = NULL;
    virtual ULONG                 GetGenXmitError() = NULL;
    virtual ULONG                 GetGenRcvError() = NULL;
    virtual ULONG                 GetGenRcvNoBuffer() = NULL;
    virtual ULONG                 GetGenMediaCapabilities();
    virtual void                  GetPnpCapabilities(NDIS_PNP_CAPABILITIES *) = NULL;
    virtual NDIS_STATUS           QueryPnpPower(NDIS_DEVICE_POWER_STATE *) = NULL;
	virtual NDIS_STATUS           QueryWakeupCapabilities(ULONG *) = NULL;

    // OID_GEN Set Handlers

    virtual NDIS_STATUS           SetGenCurrentPacketFilter(ULONG);
    virtual NDIS_STATUS           SetGenCurrentLookahead(ULONG);
    virtual NDIS_STATUS           SetGenProtocolOptions(ULONG);
    virtual NDIS_STATUS           SetPnpPower(NDIS_DEVICE_POWER_STATE *) = NULL;
	virtual NDIS_STATUS           SetWakeupCapabilities(ULONG) = NULL;
	virtual NDIS_STATUS           AddWakeupPattern(NDIS_PM_PACKET_PATTERN *) = NULL;
	virtual NDIS_STATUS           RemoveWakeupPattern(NDIS_PM_PACKET_PATTERN *) = NULL;
};
