//------------------------------------------------------------------------------
// <copyright file="car6k.hpp" company="Atheros and Microsoft">
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
// 802.11 Miniport Driver Class.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#include "dbglog_api.h"
#ifdef CONFIG_HOST_TCMD_SUPPORT
#include "testcmd.h"
#endif

#include "ieee80211.h"

#define IS_MAC_NULL(mac) (mac[0]==0 && mac[1]==0 && mac[2]==0 && mac[3]==0 && mac[4]==0 && mac[5]==0)

#define TAG_MCAST_PCK_PS 0x55aa
#define TAG_MCAST_PCK    0xa5a5
#define TAG_UCAST_PCK    0x5a5a
#define TAG_WPA_SEND_PCK 0x5a5b
#define DEF_RSSI_TRIGGER_WEIGHT     1
#define DEF_RSSI_TRIGGER_POLLTIME   5

static BYTE NullMacAddr[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
#define NTID                      17

typedef enum wpaMsgNum {
    WPA_MSG1 = 1,
    WPA_MSG2,
    WPA_MSG3,
    WPA_MSG4,
} WPA_MSGNUM;

#define GROUPKEY_IDX_A            1
#define GROUPKEY_IDX_B            2

typedef enum _AR6K_POWER_MANAGEMENT_MODE{
    POWER_MANAGEMENT_MODE_CUT_PWR=0,
    POWER_MANAGEMANT_MODE_WARM_RESET=1,
    POWER_MANAGEMENT_MODE_WLAN_DEEP_SLEEP=2
} AR6K_POWER_MANAGEMENT_MODE;


typedef struct {
    A_UINT8     networkType;
    A_UINT8     dot11AuthMode;
    A_UINT8     authMode;
    A_UINT8     pairwiseCryptoType;
    A_UINT8     pairwiseCryptoLen;
    A_UINT8     groupCryptoType;
    A_UINT8     groupCryptoLen;
    A_UINT8     ssidLength;
    A_UCHAR     ssid[WMI_MAX_SSID_LEN];
    A_UINT16    channel;
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT32    ctrl_flags;
} AP_MODE_CMD;

class CAR6KMini : public C802_11Miniport
{
private:

    // Number of transmit buffers that will be allocated during initialization
    // and put on the m_TransmitBufferList.
    ULONG      m_MaxTransmitBuffers;

    // Maximum length of data that can be copied into a transmit buffer
    ULONG      m_MaxTransmitBufferPayloadDataLength;

    // Pointer to the block of storage that holds all the transmit buffers:
    PAR6K_TRANSMIT_BUFFER  m_pTransmitBufferArray;
    ULONG                  m_cbTransmitBufferArray;

    // Transmit buffers that are not currently in use are on this list:
    LIST_ENTRY m_TransmitBufferList;

    // NDIS_PACKETS waiting to be transmitted are on this list:
    LIST_ENTRY m_TransmitNdisPacketList;


    // Pool of receive packet headers
    NDIS_HANDLE m_RxPacketPool;

    // Added for AP Network
    NDIS_HANDLE m_TxPacketPool;

    NDIS_HANDLE m_TxBufferPool;

    // Set to true when MiniportHalt is called
    BOOL     m_Halting;

    // List of NDIS_PACKETs waiting to be passed to NdisMIndicateReceivePacket
    LIST_ENTRY  m_RxPendingPacketList;
    // Event which is signalled when the m_RxPendingPacketList is non empty
    NDIS_EVENT  m_RxPendingEvent;
    // This event is Reset as long as the Rx Work Item handler is running
    NDIS_EVENT  m_RxWorkItemExitedEvent;
    // This event is used to indicate whether the reset power work item has exited
    NDIS_EVENT  m_ResetPowerWorkItemExitedEvent;
    // This event is used to indicate WMIDisconnectInd has been got from the fw and we have
    // indicated NDIS_MEDIA_DISCONN to Ndis.
    NDIS_EVENT  m_SuspendEvent;

    // The address of the NIC encoded in the hardware.
    BYTE  m_PermanentAddress[ETHERNET_MAC_ADDRESS_LENGTH];

    // Infrastructure / ad hoc mode
    NDIS_802_11_NETWORK_INFRASTRUCTURE m_InfrastructureMode;

    NDIS_802_11_AUTHENTICATION_MODE    m_AuthenticationMode;

    NDIS_802_11_ENCRYPTION_STATUS      m_EncryptionStatus;

    NDIS_802_11_RTS_THRESHOLD          m_RTSThreshold;

    // HTC Target pointer, sent to us from HTC by an
    // HTC_TARGET_AVAILABLE event indication and passed
    // to HTC on subsequent requests to htc_xxx functions.
    HTC_HANDLE                         m_pHTCTarget;

    // HIF device pointer
    // Used during BMI
    HIF_DEVICE                         *m_pHIFDevice;

    // Context returned by wmi_init, used for calls into the wmi_xxx
    // library functions.
    PVOID                               m_pWMI;

    // Boolean indicating whether a WMI_READY event has been received.
    // No WMI messages should be issued until a WMI_READY event is
    // received after bootup or a WMI_RESET request.
    BOOL                             m_WMIReady;
    BOOL                             m_HTCStarted;
    BOOL                             m_InitComplete;
    //Enables/Disables Timer WAR in fw.
    BOOL                             m_TimerWAREnable;

    // RSSI is set via a TARGET_STATS event from WMI
    NDIS_802_11_RSSI                    m_RSSI;
    A_INT16                             m_rssiEvent;
    NDIS_802_11_STATISTICS              m_tgtStats;

    AR6K_TARGET_STATS                   m_tgtAllStats;
    ULONG                               m_BitrateKpbs;
    ULONG                               m_LinkrateKpbs;
    USHORT                              m_ChannelList[32];
    ULONG                               m_cChannelList;

    // Boolean indicating whether m_SSID is valid and that
    // we want to be associated with it.
    BOOL                                m_WantToBeConnected;
    // SSID to which we are to associate
    NDIS_802_11_SSID                    m_SSID;

    // Boolean indicating we are currently trying to associate to m_SSID
    BOOL                                m_ConnectInProgress;

    // BSSID of the peer to which we are connected
    BYTE                                m_PeerBSSID[ETHERNET_MAC_ADDRESS_LENGTH];
    // Channel used to connect to the peer
    USHORT                              m_ConnectedChannel;

    BOOL                                m_RadioDisabled;
    USHORT                              m_beaconInterval;
    NDIS_802_11_NETWORK_TYPE            m_NetworkTypeInUse;
    USHORT                              m_80211PhyCapability;
    WMI_PHY_MODE                        m_queryphyMode;

    LONG                                m_TxPending;
    A_BOOL                              m_DirectRx2Ndis;
    LPTSTR                              m_DevicePowerName;
    LPTSTR                              m_DeviceInitEventName;

    // Type of BSSs for which we want INFO EVENTs indicated
    WMI_BSS_FILTER                      m_WMIBssFilter;

    NDIS_WORK_ITEM                      m_WorkItem;
    NDIS_WORK_ITEM                      m_ResetPowerWorkItem;

    NDIS_EVENT                          m_tgtReadyEvent;
    NDIS_EVENT                          m_WMIReadyEvent;
    NDIS_EVENT                          m_tgtStatsEvent;
    NDIS_EVENT                          m_tgtTxPwrEvent;
    NDIS_EVENT                          m_keepAliveEvent;

#ifdef OS_ROAM_MANAGEMENT
    NDIS_EVENT                          m_tgtChannelListEvent;
    ULONG                               m_scanCount;
#endif

    // Variables used to support BSSID_LIST retrieval from WMI
    NDIS_WLAN_BSSID_EX                 *m_pBSSIDList;
    ULONG                               m_cbBSSIDList;
    PULONG                              m_pcbBSSIDListNeeded;
    ULONG                               m_BSSIDListNumberOfItems;

    //Variables used to store the association information provided by WMI
    BYTE                                m_AssocReqLen;
    BYTE                                m_AssocRespLen;
    BYTE                                m_BeaconIeLen;
    USHORT                              m_BeaconInterval;
    PBYTE                               m_pAssocInfo;
    USHORT                              m_ListenInterval;

#ifdef SUPPORT_WPA2
    //Variables used to cache the PMKID set by supplicant
    NDIS_802_11_PMKID                  *m_pPMKID;
#endif

#ifdef WAPI_ENABLE
    //This to set wapimode
    UINT                                m_wapimode;
#endif /* WAPI_ENABLE */

    CRYPTO_TYPE                         m_PairwiseCryptoType;
    CRYPTO_TYPE                         m_GrpwiseCryptoType;
    CAR6K_802_11_AUTH_ALG               m_AuthAlg;
    CAR6K_802_11_CCKM_AUTHENTICATION_MODE m_CCKMAuthenticationMode;

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

    NDIS_DEVICE_POWER_STATE             m_PowerState;
    NDIS_802_11_POWER_MODE              m_80211_PowerMode;
    BYTE                                m_IBSS_PSEnable;
    ULONG                               m_ResumeWlanState;
    DWORD                               m_LastSysState;

    HANDLE                              m_PowerChangeEvent;
    HANDLE                              m_DriverStateEvent;


    COMMON_CREDIT_STATE_INFO            m_CreditStateInfo;
    HTC_ENDPOINT_ID                     m_arAc2EpMapping[WMM_NUM_AC];
    A_UINT8                             m_arEp2AcMapping[ENDPOINT_MAX];
    HTC_ENDPOINT_ID                     m_ControlEp;

#ifdef CONFIG_HOST_TCMD_SUPPORT
    TCMD_PM_MODE                        m_TcmdPm;
    ULONG                               m_Tcmd_totalPkt;
    ULONG                               m_Tcmd_rssiInDBm;
    ULONG                               m_Tcmd_crcError;
    ULONG                               m_Tcmd_secErr;
    NDIS_EVENT                          m_TcmdReportEvent;
#endif
    NDIS_HANDLE                         m_ConfigHandle;
    USHORT                              m_MicErrorCount;
    BOOL                                m_CounterMeasureOn;
    A_UINT32                            m_RSSIlevel;
    A_UINT32                            m_roamTblEntryCount;
    A_TIMER                             m_CounterMeasureEnterTimer;
    A_TIMER                             m_CounterMeasureExitTimer;
    A_TIMER                             m_rssiScanTimer;
    PBYTE                               m_pSupportedRates;

    A_UINT8                             m_WowMode;
    WMI_GET_WOW_LIST_REPLY              *m_WowListReply;
    NDIS_EVENT                          m_tgtWowListReplyEvent;

    A_UINT32                            m_dbgLogHdrPtr;
    A_UINT8                             m_logBuffer[DBGLOG_HOST_LOG_BUFFER_SIZE];
    A_UINT32                            m_logCnt;
    A_UINT32                            m_logBufferSize;
    HANDLE                              m_hDbgHostEvent;
    A_INT32                             m_WPSEnable;
    A_UINT32                            m_TargetId;
    A_UINT32                            m_TargetType;
    BOOL                                m_bIsWiFiOnOff;
    BOOL                                m_bIsSuspend;
    A_UINT16                            m_ChannelHint;
    A_UINT8                             m_powerMode;
    BOOL                                m_bIsSwitchAPtoSTA;
    A_UINT8                             m_keyCounter[32];


    /* AP Network */
    A_UINT8                             m_staListIndex;
    A_UINT8                             m_bcastPacket;
    WMI_AP_ACL                          m_acl;


    // This event is used to indicate the target has completed the WMIConnectionIndication call
    NDIS_EVENT            m_ConnectEvent;

    NDIS_STATUS           TransmitBufferListAllocate();
    PAR6K_TRANSMIT_BUFFER TransmitBufferListRemoveHead();
    void                  TransmitBufferListInsertHead(PAR6K_TRANSMIT_BUFFER pBuffer);
    PNDIS_PACKET          TransmitNdisPacketListRemoveHead();
    void                  TransmitNdisPacketListInsertTail(PNDIS_PACKET pBuffer);
    NDIS_STATUS           TransmitBufferBuild(PAR6K_TRANSMIT_BUFFER pBuffer, PNDIS_PACKET pPacket);
    void                  SendNdisPacket(PNDIS_PACKET, PAR6K_TRANSMIT_BUFFER);
    void                  ChangeCurrentPowerState (DWORD data);
    void                  FlushNdisPacketTransmitQueue();
    USER_RSSI_THOLD       rssi_map[12];
    ULONG                 m_txRate;
    ULONG                 m_rxRate;

#ifdef OS_ROAM_MANAGEMENT
    boolean                             m_osRoamControl;
    boolean                             m_ChannelScan;
    NDIS_802_11_BSSID_RATE_RSSI_INFO    m_RateInfo;
    boolean                             m_bRoamScanIndication;
    PNDIS_802_11_BSSID_SCAN_INFO        m_psi;
    A_TIMER                 m_disconnectIndicationTimer;
#endif


    A_BOOL                              m_bkeepaliveConfigured;

protected:
    AP_MODE_CRYPT                       m_apCrypt; //moved from public to protected section

public:

    //
    // public members needed to access target specific callback functions
    //

    BOOL                                m_SsidOidCompleted;
    LONG                                m_RxIndicationPendingCount;

    CAR6KMini();
    ~CAR6KMini();
    void                Init();
    PVOID               m_pBusDriverHandle;

    friend static void
    AR6KHTCBufferReceivedEventHandler ( void            *Context,
                                  HTC_PACKET      *pPacket);

    A_UINT8                             m_networkType;
    AP_MODE_CMD                         m_apCmd;
    AP_COMMIT_XTND                      m_apxtn;
    A_UINT8                             m_ap_num_sta;
    BOOL                                m_Connected;
    BOOL                                m_ADHOC_CREATOR;
    LIST_ENTRY                          m_mcastq;
    NDIS_SPIN_LOCK                      m_mcastqLock;
    A_UINT8                             m_mcastq_cnt;
    A_UINT8                             m_ucastq_cnt;
    BOOL                                m_dtimExpire;
    BOOL                                m_hasMoreData;
    A_UINT8                             GNonce[32];
    A_UINT8                             key[63];
    ULONGLONG                           replayCtr;
    A_UINT8                             GMK[32];
    A_UINT16                            GMKLength;
    A_UINT8                             GTK[2][32];
    A_UINT16                            GTKLength;
    A_UINT8                             m_key_size;
    A_UINT8                             m_crypt_type;
    A_BOOL                              m_isSecure;
    A_UINT8                             m_current_grp_index;
    STATION                             m_staList[AP_MAX_NUM_STA];
    A_UINT8                             m_AP_conn_sta;
    A_UINT8                             *m_wpa_buffer;
    A_TIMER                             m_Grp_rekeying_timer;
    A_UINT32                            m_Grp_rekey_time;
    A_BOOL                              m_grp_rekey_timer_enable;
    A_UINT8                             m_ap_mic_failures;
    A_BOOL                              m_ap_CounterMeasureOn;
    A_BOOL                              m_ap_intrabss;

    // The address the NIC is currently using
    BYTE                                m_CurrentAddress[ETHERNET_MAC_ADDRESS_LENGTH];


    PVOID                               m_Wmi; // public acces - need to check
    // NDIS version used
    BYTE                                m_NdisDriverMajorVersion;
    BYTE                                m_NdisDriverMinorVersion;

    NDIS_STATUS         Initialize(IN NDIS_HANDLE MiniportAdapterHandle, IN NDIS_HANDLE pConfigHandle);
    NDIS_STATUS         TargetInitialize();
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
    virtual ULONG                 GetSupportedPacketTypes();
    virtual ULONG                 GetGenXmitOk();
    virtual ULONG                 GetGenRcvOk();
    virtual ULONG                 GetGenXmitError();
    virtual ULONG                 GetGenRcvError();
    virtual NDIS_PHYSICAL_MEDIUM  GetGenPhysicalMedium();
    virtual ULONG                 GetGenRcvNoBuffer();
    virtual void                  GetPnpCapabilities(NDIS_PNP_CAPABILITIES *);
    virtual NDIS_STATUS           QueryPnpPower(NDIS_DEVICE_POWER_STATE *);
    virtual NDIS_STATUS           QueryWakeupCapabilities(ULONG *);


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
#ifdef SUPPORT_WPA2
    virtual NDIS_STATUS           Get802_11PMKID(OUT NDIS_802_11_PMKID *pPMKID, IN  ULONG cbBuffer, OUT ULONG *pcbNeeded);
    virtual NDIS_STATUS           Get802_11Capability(OUT NDIS_802_11_CAPABILITY *pCap, IN ULONG cbBuffer, OUT ULONG *pcbNeeded);
#endif

#ifdef OS_ROAM_MANAGEMENT
    A_STATUS                      ar6000_get_channel_list(void);
    virtual NDIS_STATUS           Get802_11RoamControl(OUT PNDIS_802_11_NIC_ROAM_CONTROL pCtl);
    virtual NDIS_STATUS           Get802_11ChannelList(OUT PNDIS_802_11_CHANNEL_LIST pList, IN ULONG cbList, OUT ULONG *pcbNeeded);
    virtual NDIS_STATUS           Get802_11RateRssiFilter(OUT PNDIS_802_11_BSSID_RATE_RSSI_FILTER pFilter);
    virtual NDIS_STATUS           Get802_11RateRssiInfo(OUT PNDIS_802_11_BSSID_RATE_RSSI_INFO pInfo);
#endif


    // OID_802_11 Set Handlers
    virtual NDIS_STATUS           SetPnpPower(NDIS_DEVICE_POWER_STATE *);
    virtual NDIS_STATUS           SetWakeupCapabilities(ULONG);
    virtual NDIS_STATUS           AddWakeupPattern(NDIS_PM_PACKET_PATTERN *);
    virtual NDIS_STATUS           RemoveWakeupPattern(NDIS_PM_PACKET_PATTERN *);

    virtual NDIS_STATUS           Set802_11AssociatedAPBSSID(IN NDIS_802_11_MAC_ADDRESS *);
    virtual NDIS_STATUS           Set802_11SSID(IN NDIS_802_11_SSID *);
    virtual NDIS_STATUS           Set802_11ScanParams(IN WMI_SCAN_PARAMS_CMD *);
    virtual NDIS_STATUS           Set802_11ChannelParams(IN WMI_CHANNEL_PARAMS_CMD *);
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
    virtual NDIS_STATUS           SetWPS(IN int status);

    virtual NDIS_STATUS           Set802_11AuthenticationMode(IN NDIS_802_11_AUTHENTICATION_MODE Mode);
    virtual NDIS_STATUS           Set802_11EncryptionStatus(IN NDIS_802_11_ENCRYPTION_STATUS EncryptionStatus);
    virtual NDIS_STATUS           Set802_11AddWEP(IN NDIS_802_11_WEP *pWEP);
    virtual NDIS_STATUS           Set802_11RemoveWEP(IN NDIS_802_11_KEY_INDEX);
    virtual NDIS_STATUS           Set802_11AddKey(IN NDIS_802_11_KEY *, IN BOOL);
    virtual NDIS_STATUS           Set802_11RemoveKey(IN NDIS_802_11_REMOVE_KEY *);
    virtual NDIS_STATUS           Set802_11AssociationInformation(IN NDIS_802_11_ASSOCIATION_INFORMATION *pInfo, ULONG cbInfo);
#ifdef SUPPORT_WPA2
    virtual NDIS_STATUS           Set802_11PMKID(OUT NDIS_802_11_PMKID* pPMKID, IN  ULONG cbBuffer);
#endif
    virtual NDIS_STATUS           Set802_11Test(IN NDIS_802_11_TEST *, IN ULONG cbBuffer);

#ifdef OS_ROAM_MANAGEMENT
    virtual NDIS_STATUS           Set802_11RoamControl(IN PNDIS_802_11_NIC_ROAM_CONTROL pCtl);
    virtual NDIS_STATUS           Set802_11RateRssiFilter(IN PNDIS_802_11_BSSID_RATE_RSSI_FILTER pFilter);
    virtual NDIS_STATUS           Set802_11ChannelScan(IN PNDIS_802_11_BSSID_CHANNEL_SCAN pChInfo);
#endif

    //OID_CAR6K query Handlers
    NDIS_STATUS                   GetPnpPower(ULONG *);
    NDIS_STATUS                   GetResumeWlanState(ULONG *);
    NDIS_STATUS                   GetFirmwareVersion(ULONG *);
    NDIS_STATUS                   GetWowList(WMI_GET_WOW_LIST_REPLY *, ULONG, PULONG);
    NDIS_STATUS                   GetQOSQueue (struct ar6000_queuereq *, PULONG);
    NDIS_STATUS                   GetCAr6k802_11AuthAlg(CAR6K_802_11_AUTH_ALG *);
    NDIS_STATUS                   GetCAr6k802_11CCKMAuthMode(CAR6K_802_11_CCKM_AUTHENTICATION_MODE *);
    NDIS_STATUS                   GetWmode(CAR6K_GET_WMODE *);
    NDIS_STATUS                   GetBitRate (CAR6K_GET_BITRATE *);
    //OID_CAR6K Set Handlers
    NDIS_STATUS                   SetCAr6k802_11AuthAlg(CAR6K_802_11_AUTH_ALG);
    NDIS_STATUS                   SetCAr6k802_11CCKMAuthMode(CAR6K_802_11_CCKM_AUTHENTICATION_MODE);
    NDIS_STATUS                   SetResumeWlanState(ULONG);

    NDIS_STATUS SetCAr6kBmiDone(PVOID, BOOL);
    NDIS_STATUS SetCAr6kBmiReadMemory(BMI_READ_MEMORY_PARAM *);
    NDIS_STATUS SetCAr6kBmiWriteMemory(BMI_WRITE_MEMORY_PARAM *);
    NDIS_STATUS SetCAr6kBmiExecute(BMI_EXECUTE_PARAM *);
    NDIS_STATUS SetCAr6kBmiSetAppStart(BMI_SET_APP_START_PARAM *);
    NDIS_STATUS SetCAr6kBmiReadSocRegister(BMI_READ_SOC_REG_PARAM *);
    NDIS_STATUS SetCAr6kBmiWriteSocRegister(BMI_WRITE_SOC_REG_PARAM *);
    NDIS_STATUS SetCAr6kBmiRomPatchInstall(BMI_ROMPATCH_INSTALL_PARAM *);
    NDIS_STATUS SetCAr6kBmiRomPatchUninstall(BMI_ROMPATCH_UNINSTALL_PARAM *);
    NDIS_STATUS SetCAr6kBmiRomPatchActivate(BMI_ROMPATCH_ACTIVATE_PARAM *);
    NDIS_STATUS SetCAr6kBmiRomPatchDeactivate(BMI_ROMPATCH_DEACTIVATE_PARAM *);
    NDIS_STATUS SetCAr6kHostSleepMode(WMI_SET_HOST_SLEEP_MODE_CMD *);
    NDIS_STATUS SetCAr6kWowMode(WMI_SET_WOW_MODE_CMD*);
    NDIS_STATUS SetCAr6kWowAddPattern(WMI_ADD_WOW_PATTERN_CMD*);
    NDIS_STATUS SetCAr6kWowDelPattern(WMI_DEL_WOW_PATTERN_CMD*);
    NDIS_STATUS SetCAr6kDbgLogCfgModule(WMIX_DBGLOG_CFG_MODULE_CMD *);
    NDIS_STATUS SetCAr6kCreateQOS (WMI_CREATE_PSTREAM_CMD *);
    NDIS_STATUS SetCAr6kDeleteQOS (WMI_DELETE_PSTREAM_CMD *);
    NDIS_STATUS SetCAr6kTXOP (WMI_SET_WMM_TXOP_CMD *);
    NDIS_STATUS SetCAr6kWMM (WMI_SET_WMM_CMD *);
    NDIS_STATUS SetBTStatus (WMI_SET_BT_STATUS_CMD *);
    NDIS_STATUS SetBTParams (WMI_SET_BT_PARAMS_CMD *);
    NDIS_STATUS SetRoamCtrlParams(WMI_SET_ROAM_CTRL_CMD *);
    NDIS_STATUS SetCAr6kIbssChannel (USHORT);
    NDIS_STATUS SetCAr6kAtimWindow (BYTE);
    NDIS_STATUS SetCAr6kIBSSPm (WMI_IBSS_PM_CAPS_CMD *);
    NDIS_STATUS SetCAr6kPm (WMI_POWER_PARAMS_CMD *);
    NDIS_STATUS SetCAR6kAppIE (WMI_SET_APPIE_CMD *);
    NDIS_STATUS SetCAR6kFixRates (WMI_FIX_RATES_CMD * fixRates);

#ifdef CONFIG_HOST_TCMD_SUPPORT
    NDIS_STATUS SetCAr6kTcmdContTx( TCMD_CONT_TX* );
    NDIS_STATUS SetCAr6kTcmdContRx( TCMD_CONT_RX* );
    NDIS_STATUS SetCAr6kTcmdPm( TCMD_PM* );
#endif
    NDIS_STATUS SetCAR6kWMINodeAge(A_UINT32 nodeage);
    NDIS_STATUS GetCAR6kWMINodeAge(A_UINT32* nodeage);

    NDIS_STATUS SetCAR6kWMIKeepAliveInt(A_UINT8 ucInterval);
    NDIS_STATUS GetCAR6kWMIKeepAliveInt(WMI_GET_KEEPALIVE_CMD *getKeepAlive);

    A_STATUS    ar6000_get_target_stats (void);
    A_STATUS    StartEndpoints(A_BOOL);
    void        StopEndpoints(A_BOOL);
    A_STATUS    configTargetParams();
    A_UINT32    getResetPowerState();

// Parse Registry
    NDIS_STATUS           ParseRegistryParameters(NDIS_HANDLE ConfigHandle, WLAN_STA_CONFIG *pConfig);
    A_STATUS              configRegistryParameters();
    void                  ResetPowerWorkItem();
    void                  isMcForUs(MAC_ADDRESS *pAddr, BOOL *mcForUs);

public:
    void      IndicateReceivePackets();
    void      HTCBufferSentEvent(PVOID cookie);
    void      HTCControlSendCompleted(PVOID cookie);
    void      HTCDataSendCompleted(PVOID cookie);
    void      ReceiveWMIControlPacket(HTC_PACKET *pHTCPacket, A_BOOL DataPath);
    void      ReceiveWMIDataPacket(HTC_PACKET *pHTCPacket);
    void      AllocateHTCPacketBuffers(HTC_ENDPOINT_ID  eid, UINT numBuffers);

    // WMI Wrappers
    NDIS_STATUS SendWMIConnectCommand(IN NDIS_802_11_SSID *SSID);
    NDIS_STATUS SendWMIReconnectCommand(IN NDIS_802_11_MAC_ADDRESS *BSSID);

    NDIS_STATUS SendWMIDisconnectCommand();

    // WMI Callback functions
    void      WMILinkQualityIndication(IN BYTE RSSI);
    void      WMIBitrateIndication(IN ULONG BitrateKbps);
    void      WMIChannelListIndication(IN USHORT *pChannelList, IN BYTE numChannels);
    void      WMIReadyIndication(IN PBYTE pMacAddress, IN A_UINT8 phyCap, A_UINT32 vers);

    void      WMIConnectIndication(IN USHORT Channel, IN PBYTE PeerBSSID,
                                   IN USHORT listenInterval, IN BYTE assocReqLen,
                                   IN BYTE assocRespLen, IN PBYTE assocInfo, IN BYTE beaconIeLen, IN USHORT beaconInterval,NETWORK_TYPE ntType);
    void      WMIDisconnectIndication(A_UINT8 reason);
    A_STATUS  WMISendControlPacket(IN PVOID osbuf, IN HTC_ENDPOINT_ID Endpoint);

    void      WMIBSSIDInfoAdd(IN  bss_t *pWmiBss);

    void      WMIDsetOpenReq(IN UINT, IN UINT, IN UINT, IN UINT);
    void      WMIDsetDataReq(IN UINT, IN UINT, IN UINT, IN UINT, IN UINT, IN UINT);
    void      WMISetNumDataEndpts(IN UINT);
    void      WMIRegDomainEvent(IN UINT);
    void      WMINeighborReportEvent(IN UINT, IN WMI_NEIGHBOR_INFO *info);
    void      WMITxPwr(IN UCHAR);
    void      WMIMicErrEvent(A_UINT8 keyid, A_BOOL isMcast);
    void      WMITargetStatsEvent(WMI_TARGET_STATS *);
    void      WMIRssiThresholdEvent(A_UINT16);
    void      WMIKeepAliveEvent(IN A_BOOL);

    void      WMIWowListEvent(WMI_GET_WOW_LIST_REPLY *);
#ifdef CONFIG_HOST_TCMD_SUPPORT
    void      WMITcmdReportEvent(TCMD_CONT_RX* rep);
#endif

#ifdef OS_ROAM_MANAGEMENT
    void        disconnectIndicationTimeout();
    void        WMIScanCompleteEvent(void);
    void        WMIScanIndication(void *si, ULONG len);
#endif
    void        WMIPeerEvent(A_UINT8 eventCode);
   void sendEapolKeyMsg (A_UINT8 descType, A_BOOL secure, A_BOOL mic, A_BOOL ack, A_BOOL tx, A_UINT32 index, A_UINT8 isPW,
                         A_BOOL doEncrypt, A_UINT64 keyRSC, const A_UINT8 nonce[], const A_UINT32 keyLength,
                         A_UINT32 keyDataLen, const A_UINT8 *keyData, const A_UINT32 keyBufferLength, A_UINT8  *bssid);

   void car6k_keyCounterInit (A_UINT8  *bssid);
   void car6k_keyCounterIncr ();
   void car6k_GMKGenerate (A_UINT8  *bssid);
   void car6k_GNonceCalc (A_UINT8  *bssid);
   void car6k_GTKCalc (A_UINT8 Type,A_UINT8  *bssid,A_UINT16 keyIndex);
   void car6k_ANonceCalc (STATION *station);
   void car6k_PTKGenerate (STATION *station,A_UINT8 type);
   void car6k_sendEapolKeyMsg (A_UINT8 descType,A_BOOL secure, A_BOOL mic, A_BOOL ack, A_BOOL tx, A_UINT32 index, A_UINT8 isPW,
                               A_BOOL doEncrypt,A_UINT64 keyRSC,const A_UINT8 nonce[],const A_UINT32 keyLength,
                               A_UINT32 keyDataLen, const A_UINT8 *keyData,const A_UINT32 keyBufferLength,A_UINT8  *bssid);

   void car6k_Sending_Msg_3_of_4 (STATION *station,A_UINT8 type);
   void car6k_Sending_Group_Msg_1_of_2(STATION* station,A_UINT8 type);
   void car6k_statetrans (STATION* station);
   void car6k_rekeying_Sending_Group_Msg_1_of_2 ();
   A_STATUS car6k_wpaMsgVerify (STATION *station, EAPOL_KEY_WPA *ekw, WPA_MSGNUM msgNum);

    void        counterMeasureEnterTimeout();
    void        counterMeasureExitTimeout();
    void        rssiScanTimeout();
    void        group_rekeyingTimeout();
    void        WMIRoamTbleUpdate(A_UINT32 roamTblEntryCount);

    void        GetChannelHint (IN NDIS_802_11_SSID *SSID);

    void        IterateNodeAndRemoveSSID (IN NDIS_802_11_SSID *SSID);

    HTC_ENDPOINT_ID arAc2EndpointID(A_UINT8 AC) {
        return m_arAc2EpMapping[AC];
    }

    void arSetAc2EndpointIDMap(A_UINT8 AC, HTC_ENDPOINT_ID Ep) {
        m_arAc2EpMapping[AC] = Ep;
        m_arEp2AcMapping[Ep] = AC;
    }

    A_UINT8 arEndpoint2Ac(HTC_ENDPOINT_ID Ep) {
        return  m_arEp2AcMapping[Ep];
    }

    A_STATUS ConnectService(HTC_SERVICE_CONNECT_REQ  *pConnect,
                            char                     *pDesc);

    A_STATUS SetHTCBlockSize();

    HTC_HANDLE  GetHTCHandle() { return m_pHTCTarget;}

    static void ReturnControlBufferCallBack(void *pContext, ndis_mini_buf_t *pBuf);

    HTC_ENDPOINT_ID  GetControlEp() { return m_ControlEp;}

    void ConnectIndicationPostProc ();

    void WMIErrorEvent (WMI_TARGET_ERROR_VAL errorVal);

    A_STATUS ar6000_get_target_txPwr ();

    NDIS_STATUS ar6000_set_target_txPwr (WMI_SET_TX_PWR_CMD *Buffer);

    NDIS_STATUS ar6000_abort_scan ();

    NDIS_STATUS GetRegDomain(UINT *);

    NDIS_STATUS Getsta(ap_get_sta *staList);

    NDIS_STATUS Get802_11_numofsta(AP_GET_NUM_STA_CMD * numofsta);

    NDIS_STATUS GetCurrentmode(UINT *);

    NDIS_STATUS GetTgtStats(AR6K_TARGET_STATS *);
    NDIS_STATUS Set802_11_rssi_threshold(USER_RSSI_PARAMS *rssiParams);
    NDIS_STATUS Set_car6k_listen_intvl (WMI_LISTEN_INT_CMD *listenCmd);


#ifdef WAPI_ENABLE
    NDIS_STATUS Set802_11AssocInfo(IN WMI_SET_ASSOC_INFO_CMD *pAssocInfo);
    NDIS_STATUS SetWapiMode(IN int status);
    NDIS_STATUS SetWapiKey(IN WPI_KEY * pWpiKey);
#endif /* WAPI_ENABLE */

    NDIS_STATUS Set_car6k_ip (WMI_SET_IP_CMD *pIP);
/* AP Network API */
    NDIS_STATUS Set802_11_apmode(WMI_CONNECT_CMD *);

    NDIS_STATUS Set802_11_apmode_xtn(AP_COMMIT_XTND *);

    NDIS_STATUS Set802_11_stamode();

    NDIS_STATUS Set802_11_numofsta(WMI_AP_SET_NUM_STA_CMD * numofsta);

    NDIS_STATUS Set802_11_adddel_acl(WMI_AP_ACL_MAC_CMD * pACL);

    NDIS_STATUS GetAcl(WMI_AP_ACL * pGetAcl);

    NDIS_STATUS Set802_11_HiddenSsid(WMI_AP_HIDDEN_SSID_CMD *pHiddenSsidCmd);

    NDIS_STATUS Set802_11_BGProtection(WMI_AP_PROT_SCAN_TIME_CMD *pBGScan);

    NDIS_STATUS Set802_11_DTIM(WMI_AP_SET_DTIM_CMD *pDTIMCmd);

    NDIS_STATUS Set802_11_RegDomain(WMI_SET_COUNTRY_CMD *pCountryCmd);

    NDIS_STATUS Set802_11_conn_inact_time(WMI_AP_CONN_INACT_CMD *pInact);

    NDIS_STATUS Set802_11_remove_sta(WMI_AP_SET_MLME_CMD *pMlme);

    NDIS_STATUS GetAPStatus(AP_COMMIT_XTND * pApstatus);

    NDIS_STATUS GetAPCrypt(AP_MODE_CRYPT  *pApcrypt);

    NDIS_STATUS Set802_11_Channel (AP_SET_CHANNEL_CMD *pChannelCmd);

    NDIS_STATUS Set802_11_bcon_intvl (WMI_BEACON_INT_CMD *bIntvlCmd);

    NDIS_STATUS Set_car6k_security (AP_MODE_CRYPT * apsec);

    NDIS_STATUS configap_open (AP_MODE_CRYPT * apsec);

    NDIS_STATUS configap_wpa (AP_MODE_CRYPT * apsec);

    NDIS_STATUS configap_wpa2 (AP_MODE_CRYPT * apsec);

    STATION * GetStation(A_UINT8 *mac,A_UINT16 aid);

    void addNewStation(A_UINT8 *mac,A_UINT16 aid,A_UINT8 *wpaie,A_UINT8 ielen);

    void removeNewStation(A_UINT8 *mac);

    void ndis2ether(NDIS_PACKET *pPacket,ETHERNET_MAC_HEADER *ethHead);

    void flushNdisPacketQ(LIST_ENTRY *packetQ);

    BOOL isStationSleep(void);

    void car6k_ap_rx_pwsave_flushpkt(MAC_ADDRESS *pSrcAddr, A_UINT16 powState);

    void car6k_ap_rx_process_EAPOL (BYTE *stationaddr, A_UINT8 *buf);

    NDIS_STATUS car6k_process_ap_rx (UINT32 *magic, PBYTE pData, ULONG cbData, BOOL* bisAPUnicast,
                                     MAC_ADDRESS *pDestAddr, NDIS_PACKET *pPacket);

    A_BOOL car6k_ap_tx_pwsave_qpacket( PNDIS_PACKET pNdisPacket);

    void car6k_ap_connect_event (A_UINT16  channel, A_UINT8  *bssid,
                             A_UINT8  *assocInfo, A_UINT8 assocRespLen);

    void car6k_ap_disconnect_event (A_UINT8 reason, A_UINT8 *bssid);

    void car6k_ap_mic_event (A_UINT8 keyid, A_BOOL isMcast);

    NDIS_STATUS Set_car6k_acl_policy (WMI_AP_ACL_POLICY_CMD *pACLpolicy);

    NDIS_STATUS Set_car6k_intrabss (AP_SET_INTRA_BSS *pIntra);

};


