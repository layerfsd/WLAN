//------------------------------------------------------------------------------
// <copyright file="car6k.hpp" company="Atheros and Microsoft">
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
// AR6000 NDIS Miniport  Class.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================


#define NDIS_DRIVER_MAJOR_VERSION 4
#define NDIS_DRIVER_MINOR_VERSION 0

#define AR6000_MAX_ENDPOINTS  4
#define AR6000_MAX_RX_BUFFERS 16
#define AR6000_ETH_ADDR_LEN   6

#define AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_READY 20000
#define AR6K_DEFAULT_MS_TO_WAIT_FOR_WMI_READY 20000
#define AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_GETSTATS 10000

#define AR6K_DEFAULT_NDIS_INITIALIZATION_THREAD_PRIORITY 200

#define NdisInitializeBuffer(pBuffer, Address, Length) MmInitializeMdl(pBuffer, Address, Length)

/* NDIS wants RSSI indicated relative to -95 dBm */
#define RSSI_TO_NDIS(x) ((x) - 95)

#define WLAN_MAC_ADDR_SIZE 6
#define isGrp(macaddr) \
    ((BOOL)((macaddr)->macAddress[0] & 0x01))
#define isBcast(macaddr) \
    (((macaddr)->macAddress[0] == 0xff))
#define A_MACADDR_COMP(m1, m2)                            \
        (!(NdisEqualMemory((void *)&((m1)->macAddress[0]),    \
        (void *)&((m2)->macAddress[0]),                       \
        WLAN_MAC_ADDR_SIZE) == 1))

 
typedef struct MAC_ADDRESS
{
 	BYTE  macAddress[ETHERNET_MAC_ADDRESS_LENGTH];
} MAC_ADDRESS, *PMAC_ADDRESS;


#pragma pack(1)

typedef struct ETHERNET_MAC_HEADER
{
	BYTE			DestMACAddress[ETHERNET_MAC_ADDRESS_LENGTH];
	BYTE			SrcMACAddress[ETHERNET_MAC_ADDRESS_LENGTH];
	USHORT          Length;
} ETHERNET_MAC_HEADER, *PETHERNET_MAC_HEADER;

typedef struct SNAP_HEADER
{
	BYTE            dsap;
	BYTE            ssap;
	BYTE            cntl;
	BYTE            orgCode[3];
	USHORT          Type;
} SNAP_HEADER, *PSNAP_HEADER;

#define CAP_TYPE_SIZE 6
typedef struct capType {
	A_UINT8		octets[CAP_TYPE_SIZE];
}CAP_CONST;

// Encapsulation header for RFC1042 and Ethernet_tunnel
#define RFC1042SNAP	{{0xAA,0xAA,0x03,0x00,0x00,0x00}}
#define ETHERNET_TUNNEL	{{0xAA,0xAA,0x03,0x00,0x00,0xF8}}

#define APPLE_TALK_ARP	0x80f3
#define NOVELL_IPX		0x8137
#define IPV4			0x8000


#ifdef WMM
/* This stuff is used when we want a simple layer-3 visibility */
struct iphdr {
#ifdef BIG_ENDIAN
    A_UINT32    ip_v:4,                     /* version */
                ip_hl:4,            /* header length */
#else
    A_UINT32    ip_hl:4,                /* header length */
                ip_v:4,                 /* version */
#endif
                ip_tos:8,               /* type of service */
                ip_len:16;              /* total length */
    A_UINT16    ip_id;                  /* identification */
    A_INT16     ip_off;                 /* fragment offset field */
#define IP_DF 0x4000                    /* dont fragment flag */
#define IP_MF 0x2000                    /* more fragments flag */
#define IP_OFFMASK 0x1fff               /* mask for fragmenting bits */
    A_UINT8     ip_ttl;                 /* time to live */
    A_UINT8     ip_p;                   /* protocol */
    A_UINT16    ip_sum;                        /* checksum */
    A_UINT32    ip_src, ip_dst;  /* source and dest address */
};

#endif //WMM

// OIDs specific structures
typedef struct bmi_read_memory_param
{
	DWORD address;
	DWORD length;
	CHAR  buffer;
} BMI_READ_MEMORY_PARAM;

typedef struct bmi_read_soc_reg_param
{
	DWORD address;
	DWORD value;
} BMI_READ_SOC_REG_PARAM;

typedef BMI_READ_MEMORY_PARAM BMI_WRITE_MEMORY_PARAM;

typedef struct bmi_execute_param
{
	DWORD address;
	DWORD param;
} BMI_EXECUTE_PARAM;

typedef struct bmi_set_app_start_param
{
	DWORD address;
} BMI_SET_APP_START_PARAM;

typedef struct bmi_write_soc_reg_param
{
	DWORD address;
	DWORD param;
} BMI_WRITE_SOC_REG_PARAM;

#ifdef HTC_RAW_INTERFACE

typedef struct htc_raw_read_param
{
	DWORD endPointId;
	DWORD length;
	CHAR buffer;
} HTC_RAW_READ_PARAM;

typedef struct htc_raw_write_param
{
	DWORD endPointId;
	DWORD length;
	CHAR buffer;
} HTC_RAW_WRITE_PARAM;

#endif //HTC_RAW_INTERFACE

#ifdef TCMD
/*
 *  add TCMD_MODE besides wmi and bypasswmi
 *  in TCMD_MODE, only few TCMD releated wmi commands
 *  counld be hanlder
 */
enum {
    AR6000_WMI_MODE = 0,
    AR6000_BYPASS_MODE,
    AR6000_TCMD_MODE
};
#endif /* TCMD */

#pragma pack()

typedef struct AR6K_TRANSMIT_BUFFER
{
	// When the buffer is not in use, the node is used to put the
	// buffer on the free list of transmit buffers.
	LIST_ENTRY      node;

	// When the buffer is in use, having been passed down to HTC
	// for transmission, pTxPacket points to the NDIS_PACKET that
	// it is transmitting.
	PNDIS_PACKET    pNdisPacket;

	// When the buffer is in use, cbCWMIData contains the size of
	// the variable length WMI_DATA (including MAC Header, SNAP Header,
	// and Payload data) which follows.
	ULONG           cbWMIData;

	//
	// Remaining portion of the structure is the WMI_DATA to be transmitted,
	// which begins with a WMI_DATA_HDR but is variable length.
	//
	WMI_DATA_HDR             WMIDataHeader;

	ETHERNET_MAC_HEADER      MACHeader;

	SNAP_HEADER              SNAPHeader;

	// 
	// Variable length payload (e.g. ARP packet, IP packet) follows
	//
	BYTE             PayloadData[1];

} AR6K_TRANSMIT_BUFFER, *PAR6K_TRANSMIT_BUFFER;

typedef struct WlanConfig		// Station Configuration Structure for Reading values from Registry.
{
	
	ULONG 	bkScanEnable;           // Enable(1) or disble(0) Background Scan
	ULONG	bkScanPeriod;           // Background Scan Interval in seconds
	USHORT 	wpaEnabled;             // Enable(1) or disble(0) WPA
	UCHAR 	wpa2Enabled;            // Enable(1) or disble(0) WPA2
	UCHAR	defaultApp;             // Default APP in App (1) or APP loaded using BMI loader
	UCHAR	byPassWmi;              // ByPass(1) or Pass(0) WMI Phase
	UCHAR	discTimeout;            // Disconnect timeout
	ULONG   resetPowerState;        // Power State Upon Reset
    ULONG   ibssChannel;
    ULONG   powerSaveMode;
}WLAN_STA_CONFIG;

class CAR6KMini : public C802_11Miniport
{
private:

	// Number of transmit buffers that will be allocated during initialization
	// and put on the m_TransmitBufferList.
	ULONG      m_MaxTransmitBuffers;
#define DEFAULT_MAX_TRANSMIT_BUFFERS 30

	// Maximum length of data that can be copied into a transmit buffer
	ULONG      m_MaxTransmitBufferPayloadDataLength;
#define DEFAULT_MAX_TRANSMIT_BUFFER_PAYLOAD_DATA_LENGTH 1500

	// Pointer to the block of storage that holds all the transmit buffers:
	PAR6K_TRANSMIT_BUFFER  m_pTransmitBufferArray;
	ULONG                  m_cbTransmitBufferArray;

	// Transmit buffers that are not currently in use are on this list:
	LIST_ENTRY m_TransmitBufferList;

	// NDIS_PACKETS waiting to be transmitted are on this list:
	LIST_ENTRY m_TransmitNdisPacketList;


	// Pool of receive packet headers
	NDIS_HANDLE m_RxPacketPool;

	// Set to true when MiniportHalt is called
	boolean     m_Halting;

	// List of NDIS_PACKETs waiting to be passed to NdisMIndicateReceivePacket
	LIST_ENTRY  m_RxPendingPacketList;
	// Event which is signalled when the m_RxPendingPacketList is non empty
	NDIS_EVENT  m_RxPendingEvent;
	// This event is Reset as long as the Rx Work Item handler is running
	NDIS_EVENT  m_RxWorkItemExitedEvent;
	// This event is used to indicate whether the reset power work item has exited
	NDIS_EVENT  m_ResetPowerWorkItemExitedEvent;

	// The address of the NIC encoded in the hardware.
	BYTE  m_PermanentAddress[ETHERNET_MAC_ADDRESS_LENGTH];

	// The address the NIC is currently using
	BYTE  m_CurrentAddress[ETHERNET_MAC_ADDRESS_LENGTH];

	// Infrastructure / ad hoc mode
	NDIS_802_11_NETWORK_INFRASTRUCTURE m_InfrastructureMode;

	ULONG                              m_AuthAlg;

	NDIS_802_11_AUTHENTICATION_MODE    m_AuthenticationMode;

	NDIS_802_11_ENCRYPTION_STATUS      m_EncryptionStatus;

	// HTC Target pointer, sent to us from HTC by an
	// HTC_TARGET_AVAILABLE event indication and passed
	// to HTC on subsequent requests to htc_xxx functions.
	HTC_TARGET                         *m_pHTCTarget;

    // HIF device pointer 
    // Used during BMI
    HIF_DEVICE                          *m_pHIFDevice;

	// Context returned by wmi_init, used for calls into the wmi_xxx
	// library functions.
	PVOID                               m_pWMI;

	// Boolean indicating whether a WMI_READY event has been received.
	// No WMI messages should be issued until a WMI_READY event is
	// received after bootup or a WMI_RESET request.
	boolean                             m_tgtReady;
	boolean                             m_WMIReady;
	boolean                             m_HTCInited;
	boolean                             m_HTCStarted;
	boolean                             m_InitComplete;

	// RSSI is set via a TARGET_STATS event from WMI
	NDIS_802_11_RSSI                    m_RSSI;
	NDIS_802_11_STATISTICS				m_tgtStats;

	ULONG                               m_BitrateKpbs;

	USHORT                              m_ChannelList[32];
	ULONG                               m_cChannelList;

	// Boolean indicating whether m_SSID is valid and that
	// we want to be associated with it.
	boolean                             m_WantToBeConnected;
	// SSID to which we are to associate
	NDIS_802_11_SSID                    m_SSID;

	// Boolean indicating we are currently trying to associate to m_SSID
	boolean                             m_ConnectInProgress;

	// Boolean indicating whether we are connected to a peer (e.g. AP)
	boolean                             m_Connected;
	// BSSID of the peer to which we are connected
	BYTE                                m_PeerBSSID[ETHERNET_MAC_ADDRESS_LENGTH];
	// Channel used to connect to the peer
	USHORT                              m_ConnectedChannel;

	LONG                                m_TxPending;

	// Type of BSSs for which we want INFO EVENTs indicated
	WMI_BSS_FILTER                      m_WMIBssFilter;

	NDIS_WORK_ITEM                      m_WorkItem;
	NDIS_WORK_ITEM                      m_ResetPowerWorkItem;

	NDIS_EVENT                          m_tgtReadyEvent;
	NDIS_EVENT                          m_WMIReadyEvent;
	NDIS_EVENT							m_tgtStatsEvent;

	// Variables used to support BSSID_LIST retrieval from WMI
	NDIS_WLAN_BSSID_EX                 *m_pBSSIDList;
	ULONG                               m_cbBSSIDList;
	PULONG                              m_pcbBSSIDListNeeded;
	ULONG                               m_BSSIDListNumberOfItems;

	//Variables used to store the association information provided by WMI
	BYTE                                m_AssocReqLen;
	BYTE                                m_AssocRespLen;
	PBYTE                               m_pAssocInfo;
	USHORT                              m_ListenInterval;

#ifdef OID_802_11_PMKID
	//Variables used to cache the PMKID set by supplicant
	NDIS_802_11_PMKID                  *m_pPMKID;
#endif

	// This index is used to rotate the Grpwise cipher type.
	USHORT                              m_GrpwiseCryptoIndex;
	CRYPTO_TYPE                         m_PairwiseCryptoType;
	CRYPTO_TYPE                         m_GrpwiseCryptoType;

	// MAC Address of the AP that we want to associate with,
	// or 0xFFFFFFFFFFFF if we can associate with any AP
	NDIS_802_11_MAC_ADDRESS             m_DesiredAPBSSID;

	BYTE                                m_Key[4][32];
	ULONG                               m_cbKey[4];
	ULONGLONG                           m_KeyRSC[4];
	ULONG                               m_KeyIndex[4];
	BYTE                                m_KeyUsage[4];

	UINT                                m_NumDataEndpts;
	UINT                                m_RegCode;
	UCHAR                               m_TxPwr;

	WLAN_STA_CONFIG                     m_Config;

#ifdef TCMD 
	NDIS_EVENT                         m_rxReportEvent;
	USHORT                             m_tcmdRxReport;
	UINT                               m_tcmdRxTotalPkt;
	UINT                               m_tcmdRxRssi;
	UINT                               m_tcmdPm;
#endif //TCMD

	NDIS_DEVICE_POWER_STATE            m_PowerState;
	NDIS_802_11_POWER_MODE             m_80211_PowerMode;
	ULONG                              m_ResumeWlanState;
	DWORD                              m_LastSysState;

	HANDLE                             m_PowerChangeEvent;

	A_UINT32						   m_TargetId;
    
public:
	CAR6KMini();
	~CAR6KMini();
	void				Init();

#ifdef NDIS_BUS_DRIVER
    PVOID               m_pBusDriverHandle;
#endif

	NDIS_STATUS         Initialize(IN NDIS_HANDLE MiniportAdapterHandle, IN NDIS_HANDLE pConfigHandle);
	void                Halt();
	BOOLEAN             CheckForHang();
	NDIS_STATUS         Reset(OUT PBOOLEAN pAddressingReset);
	virtual void        SendPackets(IN PPNDIS_PACKET PacketArray, IN UINT NumberOfPackets);
	virtual void        ReturnPacket(IN PNDIS_PACKET Packet);
	virtual NDIS_STATUS SetInformation(IN NDIS_OID Oid, IN  PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbRead, OUT PULONG pcbNeeded);
	virtual NDIS_STATUS QueryInformation(IN NDIS_OID Oid, OUT PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbWritten, OUT PULONG pcbNeeded);

	// OID_GEN Query Handlers

	virtual PNDIS_OID             GetSupportedOidList(OUT PNDIS_OID pOidBuffer, IN OUT PULONG pMaxOidsToAddToBuffer, IN OUT PULONG pTotalNumberOfSupportedOids);
	virtual ULONG                 GetGenVendorDriverVersion();
	virtual ULONG                 GetGenMacOptions();
	virtual NDIS_HARDWARE_STATUS  GetGenHardwareStatus();
	virtual NDIS_MEDIA_STATE      GetGenMediaConnectStatus();
	virtual ULONG                 GetGenLinkSpeed();
	virtual ULONG                 GetGenTransmitBufferSpace();
	virtual ULONG                 GetGenReceiveBufferSpace();
	virtual ULONG                 GetGenTransmitBlockSize();
	virtual ULONG                 GetGenReceiveBlockSize();
	virtual ULONG                 GetGenVendorId();
	virtual const char *          GetGenVendorDescription();
	virtual USHORT                GetGenDriverVersion();
	virtual ULONG		      	  GetSupportedPacketTypes();
	virtual ULONG                 GetGenXmitOk();
	virtual ULONG                 GetGenRcvOk();
	virtual ULONG                 GetGenXmitError();
	virtual ULONG                 GetGenRcvError();
	virtual ULONG                 GetGenRcvNoBuffer();
	virtual void                  GetPnpCapabilities(NDIS_PNP_CAPABILITIES *);
	virtual NDIS_STATUS           QueryPnpPower(NDIS_DEVICE_POWER_STATE *);
    

	// OID_802_3 Query Handlers
	virtual BYTE *                Get802_3PermanentAddress();
	virtual BYTE *                Get802_3CurrentAddress();
	virtual ULONG                 Get802_3MacOptions();
	virtual ULONG                 Get802_3RcvErrorAlignment();
	virtual ULONG                 Get802_3XmitOneCollision();
	virtual ULONG                 Get802_3XmitMoreCollision();

	// OID_802_11 Query Handlers
	virtual NDIS_STATUS           Get802_11AssociatedAPBSSID(OUT NDIS_802_11_MAC_ADDRESS *);
	virtual void                  Get802_11SSID(OUT NDIS_802_11_SSID *);
	virtual void                  Get802_11NetworkTypesSupported(OUT NDIS_802_11_NETWORK_TYPE **, OUT PULONG pNumTypes);
	virtual void                  Get802_11NetworkTypeInUse(OUT PNDIS_802_11_NETWORK_TYPE);
	virtual NDIS_STATUS           Get802_11TxPowerLevel(OUT NDIS_802_11_TX_POWER_LEVEL *);
	virtual NDIS_STATUS           Get802_11RSSI(OUT NDIS_802_11_RSSI *);
	virtual NDIS_STATUS           Get802_11RSSITrigger(OUT NDIS_802_11_RSSI *);
	virtual void                  Get802_11InfrastructureMode(OUT NDIS_802_11_NETWORK_INFRASTRUCTURE *);
	virtual NDIS_STATUS           Get802_11FragmentationThreshold(OUT NDIS_802_11_FRAGMENTATION_THRESHOLD *pThreshold);
	virtual NDIS_STATUS           Get802_11RTSThreshold(OUT NDIS_802_11_RTS_THRESHOLD *pThreshold);
	virtual NDIS_STATUS           Get802_11NumberOfAntennas(OUT ULONG *pNumberOfAntennas);
	virtual NDIS_STATUS           Get802_11RxAntennaSelected(OUT NDIS_802_11_ANTENNA *pAntenna);
	virtual NDIS_STATUS           Get802_11TxAntennaSelected(OUT NDIS_802_11_ANTENNA *pAntenna);
	virtual void                  Get802_11SupportedRates(OUT BYTE *Rates);
	virtual NDIS_STATUS           Get802_11DesiredRates(OUT NDIS_802_11_RATES *Rates);
	virtual void                  Get802_11Configuration(OUT PNDIS_802_11_CONFIGURATION pConfiguration);
	virtual NDIS_STATUS           Get802_11Statistics(OUT PNDIS_802_11_STATISTICS pStatistics);
	virtual NDIS_STATUS           Get802_11PowerMode(OUT PNDIS_802_11_POWER_MODE pPowerMode);
	virtual void                  Get802_11BSSIDList(OUT PNDIS_802_11_BSSID_LIST_EX pList, ULONG cbBuffer, PULONG pcbNeeded);
	virtual void                  Get802_11AuthenticationMode(OUT NDIS_802_11_AUTHENTICATION_MODE *pMode);
	virtual NDIS_STATUS           Get802_11PrivacyFilter(OUT PNDIS_802_11_PRIVACY_FILTER pFilter);
	virtual void                  Get802_11EncryptionStatus(OUT PNDIS_802_11_ENCRYPTION_STATUS pStatus);
	virtual NDIS_STATUS           Get802_11AssociationInformation(OUT PNDIS_802_11_ASSOCIATION_INFORMATION pInfo, ULONG cbBuffer, PULONG pcbNeeded);
#ifdef OID_802_11_PMKID
	virtual NDIS_STATUS           Get802_11PMKID(OUT NDIS_802_11_PMKID *pPMKID, IN  ULONG cbBuffer, OUT ULONG *pcbNeeded);
	virtual NDIS_STATUS           Get802_11Capability(OUT NDIS_802_11_CAPABILITY *pCap, IN ULONG cbBuffer, OUT ULONG *pcbNeeded);
#endif
	A_STATUS                      ar6000_get_target_stats(void);
	// OID_802_11 Set Handlers
	virtual NDIS_STATUS           SetPnpPower(NDIS_DEVICE_POWER_STATE *);
    
	virtual NDIS_STATUS           Set802_11AssociatedAPBSSID(IN NDIS_802_11_MAC_ADDRESS *);
	virtual NDIS_STATUS           Set802_11SSID(IN NDIS_802_11_SSID *);
	virtual NDIS_STATUS           Set802_11NetworkTypeInUse(IN NDIS_802_11_NETWORK_TYPE);
	virtual NDIS_STATUS           Set802_11TxPowerLevel(NDIS_802_11_TX_POWER_LEVEL);
	virtual NDIS_STATUS           Set802_11RSSITrigger(NDIS_802_11_RSSI);
	virtual NDIS_STATUS           Set802_11InfrastructureMode(NDIS_802_11_NETWORK_INFRASTRUCTURE);
	virtual NDIS_STATUS           Set802_11FragmentationThreshold(NDIS_802_11_FRAGMENTATION_THRESHOLD);
	virtual NDIS_STATUS           Set802_11RTSThreshold(NDIS_802_11_RTS_THRESHOLD);
	virtual NDIS_STATUS           Set802_11RxAntennaSelected(ULONG Antenna);
	virtual NDIS_STATUS           Set802_11TxAntennaSelected(ULONG Antenna);
	virtual NDIS_STATUS           Set802_11DesiredRates(NDIS_802_11_RATES *Rates);
	virtual NDIS_STATUS           Set802_11Configuration(IN NDIS_802_11_CONFIGURATION *pConfiguration);
	virtual NDIS_STATUS           Set802_11Disassociate();
	virtual NDIS_STATUS           Set802_11PowerMode(IN NDIS_802_11_POWER_MODE PowerMode);
	virtual NDIS_STATUS           Set802_11BSSIDListScan();
	virtual NDIS_STATUS           Set802_11PrivacyFilter(IN NDIS_802_11_PRIVACY_FILTER Filter);
	virtual NDIS_STATUS           Set802_11ReloadDefaults(IN NDIS_802_11_RELOAD_DEFAULTS);


	virtual NDIS_STATUS           Set802_11AuthenticationMode(IN NDIS_802_11_AUTHENTICATION_MODE Mode);
	virtual NDIS_STATUS           Set802_11EncryptionStatus(IN NDIS_802_11_ENCRYPTION_STATUS EncryptionStatus);
	virtual NDIS_STATUS           Set802_11AddWEP(IN NDIS_802_11_WEP *pWEP);
	virtual NDIS_STATUS           Set802_11RemoveWEP(IN NDIS_802_11_KEY_INDEX);
	virtual NDIS_STATUS           Set802_11AddKey(IN NDIS_802_11_KEY *);
	virtual NDIS_STATUS           Set802_11RemoveKey(IN NDIS_802_11_REMOVE_KEY *);
	virtual NDIS_STATUS           Set802_11AssociationInformation(IN NDIS_802_11_ASSOCIATION_INFORMATION *pInfo, ULONG cbInfo);
#ifdef OID_802_11_PMKID
	virtual NDIS_STATUS           Set802_11PMKID(OUT NDIS_802_11_PMKID* pPMKID, IN  ULONG cbBuffer);
#endif
	virtual NDIS_STATUS           Set802_11Test(IN NDIS_802_11_TEST *);
    
	//OID_CAR6K query Handlers
	NDIS_STATUS                   GetPnpPower(ULONG *);
	NDIS_STATUS                   GetResumeWlanState(ULONG *);
	NDIS_STATUS                   GetFirmwareVersion(ULONG *);

	//OID_CAR6K Set Handlers
	NDIS_STATUS                   SetCAr6k802_11AuthAlg(ULONG);
	NDIS_STATUS                   SetResumeWlanState(ULONG);

	NDIS_STATUS SetCAr6kBmiDone(PVOID);
	NDIS_STATUS SetCAr6kBmiReadMemory(BMI_READ_MEMORY_PARAM *);
	NDIS_STATUS SetCAr6kBmiWriteMemory(BMI_WRITE_MEMORY_PARAM *);
	NDIS_STATUS SetCAr6kBmiExecute(BMI_EXECUTE_PARAM *);
	NDIS_STATUS SetCAr6kBmiSetAppStart(BMI_SET_APP_START_PARAM *);
	NDIS_STATUS SetCAr6kBmiReadSocRegister(BMI_READ_SOC_REG_PARAM *);
	NDIS_STATUS SetCAr6kBmiWriteSocRegister(BMI_WRITE_SOC_REG_PARAM *);

#ifdef HTC_RAW_INTERFACE

	NDIS_STATUS SetCAr6kHtcRawOpen(void);
	NDIS_STATUS SetCAr6kHtcRawClose(void);
	NDIS_STATUS SetCAr6kHtcRawRead(HTC_RAW_READ_PARAM *);
	NDIS_STATUS SetCAr6kHtcRawWrite(HTC_RAW_WRITE_PARAM *);

#endif

#ifdef TCMD
	A_STATUS ar6000_ioctl_tcmd_get_rx_report(PUCHAR rq);
	NDIS_STATUS SetCAr6kTCmdContTx(WMI_TCMD_CONT_TX_CMD *);
	NDIS_STATUS SetCAr6kTCmdContRx(WMI_TCMD_CONT_RX_CMD *);
	NDIS_STATUS SetCAr6kTCmdPm(WMI_TCMD_PM_CMD *);
#endif //TCMD

	void        TargetAvailableEventHandler(HTC_TARGET *pHTCTarget, HIF_DEVICE *pHIFDevice);
	void        TargetUnavailableEventHandler(HTC_TARGET *pHTCTarget);
	A_STATUS    RegisterEndpointEventHandlers(HTC_ENDPOINT_ID endPointId);
	A_STATUS    StartEndpoints();
	void        StopEndpoints();
	A_STATUS    configTargetParams();
	A_UINT32    getResetPowerState();

// Parse Registry
	NDIS_STATUS           ParseRegistryParameters(NDIS_HANDLE ConfigHandle, WLAN_STA_CONFIG *pConfig);
    A_STATUS              configRegistryParameters();
	void                  ResetPowerWorkItem();
	void				  isMcForUs(MAC_ADDRESS *pAddr, BOOL *mcForUs);

private:
	NDIS_STATUS           TransmitBufferListAllocate();
	PAR6K_TRANSMIT_BUFFER TransmitBufferListRemoveHead();
	void                  TransmitBufferListInsertHead(PAR6K_TRANSMIT_BUFFER pBuffer);
	PNDIS_PACKET          TransmitNdisPacketListRemoveHead();
	void                  TransmitNdisPacketListInsertTail(PNDIS_PACKET pBuffer);
	NDIS_STATUS           TransmitBufferBuild(PAR6K_TRANSMIT_BUFFER pBuffer, PNDIS_PACKET pPacket);
	void                  SendNdisPacket(PNDIS_PACKET, PAR6K_TRANSMIT_BUFFER);
public:
	void                  IndicateReceivePackets();

public:
	void      HTCBufferSentEvent(PVOID cookie);
	void      HTCControlSendCompleted(PVOID cookie);
	void      HTCDataSendCompleted(PVOID cookie);
	void      ReceiveWMIControlPacket(HTC_EVENT_INFO  *evInfo);
	void      ReceiveWMIDataPacket(HTC_EVENT_INFO  *evInfo);
	void      AllocateHTCPacketBuffers(HTC_ENDPOINT_ID  eid, UINT numBuffers);

	// WMI Wrappers
	NDIS_STATUS SendWMIConnectCommand(IN NDIS_802_11_SSID *SSID);
	NDIS_STATUS SendWMIDisconnectCommand();

	// WMI Callback functions
	void      WMILinkQualityIndication(IN BYTE RSSI);
	void      WMIBitrateIndication(IN ULONG BitrateKbps);
	void      WMIChannelListIndication(IN USHORT *pChannelList, IN BYTE numChannels);
	void      WMIReadyIndication(IN PBYTE pMacAddress);
	void      WMIConnectIndication(IN USHORT Channel, IN PBYTE PeerBSSID, 
		                           IN USHORT listenInterval, IN BYTE assocReqLen, 
								   IN BYTE assocRespLen, IN PBYTE assocInfo);
	void      WMIDisconnectIndication();
	A_STATUS  WMISendControlPacket(IN PVOID osbuf, IN HTC_ENDPOINT_ID endPointId);

	void      WMIBSSIDInfoAdd(IN  bss_t *pWmiBss);

	void      WMIDsetOpenReq(IN UINT, IN UINT, IN UINT, IN UINT);
	void      WMIDsetDataReq(IN UINT, IN UINT, IN UINT, IN UINT, IN UINT, IN UINT);
	void      WMISetNumDataEndpts(IN UINT); 
	void      WMIRegDomainEvent(IN UINT);
	void      WMINeighborReportEvent(IN UINT, IN WMI_NEIGHBOR_INFO *info);
	void      WMITxPwr(IN UCHAR);
	void      WMIMicErrEvent(A_UINT8 keyid, A_BOOL isMcast);
	void	  WMITargetStatsEvent(WMI_TARGET_STATS *);
#ifdef TCMD
	void	  WMIRxReportEvent(A_UINT32 totalPkt,A_UINT32 rssi);
#endif //TCMD
};


