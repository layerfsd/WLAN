//------------------------------------------------------------------------------
// <copyright file="wince_common_drv.h" company="Atheros">
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
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
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef _WINCE_COMMON_DRV_H_
#define _WINCE_COMMON_DRV_H_

#include <ndis.h>

#include "htc_api.h"
#include "hif.h"
#include "ieee80211.h"
#include "a_types.h"
#include "wmi.h"

#define ETHERNET_MAC_ADDRESS_LENGTH    6
#define ETH_HEADER_SIZE                14
#define MAX_EEPROM_SZ                  768

#ifdef OS_ROAM_MANAGEMENT
#define AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_CHANNEL_LIST 10000
#define AR6K_CTRL_FLAG      0x27
#define AR6K_DWELL_TIME     10
#define ROAM_BUFFER_SIZE    2048
#endif



#define WLAN_BINARY_REV1_STR    L"\\athwlan1_0.bin"
#define WLAN_BINARY_REV2_STR    L"\\athwlan2_0.bin.z77"
#define TCMD_BINARY_REV1_STR    L"\\athtcmd1_0.bin"
#define TCMD_BINARY_REV2_STR    L"\\athtcmd2_0.bin"
#define ART_BINARY_REV2_STR     L"\\device.bin"

#define AR6000_MAX_ENDPOINTS  4
#define AR6000_MAX_RX_BUFFERS 64
#define AR6000_ETH_ADDR_LEN   6

#define AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_READY           20000
#define AR6K_DEFAULT_MS_TO_WAIT_FOR_WMI_READY           20000
#define AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_GETSTATS        10000
#define AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_GETTXPWR        10000
#define AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_KEEPALIVEINTV   10000
#define AR6K_DEFAULT_MS_TO_WAIT_FOR_SCAN_COMPLETE         500
#define NDIS_SUSPEND_TIMEOUT_INTERVAL                   50
#define AR6K_ERROR_REPORT_MASK                          0x7F

#define MIC_ERROR_TIMEOUT_MSEC    60000
#define MIC_ERROR_BLOCKOUT_MSEC   60000
#define MAX_MIC_ERRORS            2

    /* the buffers should be aligned to a cache-line boundary to utilize a host controller's
     * DMA controller.  For the majority of WinMobile/CE processors, this is 16 or 32 bytes,
     *  */
#define RX_BUFFER_ALIGNMENT_BYTES 32
#define TX_BUFFER_ALIGNMENT_BYTES 32

#define AR6K_DEFAULT_NDIS_INITIALIZATION_THREAD_PRIORITY 200

#define NDIS_MAXPSP_LISTEN_INTERVAL     1000
#define NDIS_FASTPSP_LISTEN_INTERVAL    100

#define FAST_PSP                        2
#define MAX_PSP                         1
#define NO_PSP                          0

#define NdisInitializeBuffer(pBuffer, Address, Length) MmInitializeMdl(pBuffer, Address, Length)

/* NDIS wants RSSI indicated relative to -95 dBm. Target fw returns this relative value */
#define RSSI_TO_NDIS(x) (x)
#define RSSI_TO_ABS(x)  (x+95)
#define ABS_TO_RSSI(x)  (x-95)

#define WLAN_MAC_ADDR_SIZE 6
#define isGrp(macaddr) \
    ((BOOLEAN)((macaddr)->macAddress[0] & 0x01))
#define isBcast(macaddr) \
    (((macaddr)->macAddress[0] == 0xff))
#define A_MACADDR_COMP(m1, m2)                            \
        (!(NdisEqualMemory((void *)&((m1)->macAddress[0]),    \
        (void *)&((m2)->macAddress[0]),                       \
        WLAN_MAC_ADDR_SIZE) == 1))

#define NO_OF_PMKIDS_SUPPORTED        3



static NDIS_802_11_NETWORK_TYPE g_AR6KNetworkTypesSupported[] =
{
    Ndis802_11DS,
    Ndis802_11OFDM24,
    Ndis802_11OFDM5,
};

typedef enum _AR6K_SUSPEND_POWER_STATE{
    SUSPEND_MODE_REG_CUT_PWR=0,
    SUSPEND_MODE_REG_WARM_RESET=1,
    SUSPEND_MODE_REG_WLAN_DEEP_SLEEP=2
} AR6K_SUSPEND_POWER_STATE;

typedef enum {
    mode_11g = 1,
    mode_11aOnly,
    mode_11ag
} PHY_CAPABILITY;

typedef struct MAC_ADDRESS
{
    BYTE  macAddress[ETHERNET_MAC_ADDRESS_LENGTH];
} MAC_ADDRESS, *PMAC_ADDRESS;



#pragma pack(1)

typedef struct ETHERNET_MAC_HEADER
{
    BYTE            DestMACAddress[ETHERNET_MAC_ADDRESS_LENGTH];
    BYTE            SrcMACAddress[ETHERNET_MAC_ADDRESS_LENGTH];
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

typedef struct EAPOL_HEADER {
    BYTE     protoVers;
    BYTE     type;
    USHORT   bodyLength;
} EAPOL_HEADER, *PEAPOL_HEADER;

#define ETHERTYPE_EAPOL         0x888e

#define EAPOL_PROTOVERS         0x01
#define EAPOL_TYPE_EAP          0x00
#define EAPOL_TYPE_START        0x01
#define EAPOL_TYPE_LOGOFF       0x02
#define EAPOL_TYPE_KEYMSG       0x03
#define EAPOL_TYPE_ASF_ALERT    0x04

#define EAPOL_KEY_TYPE_RC4      0x01
#define EAPOL_KEY_TYPE_WPA2     0x02
#define EAPOL_KEY_TYPE_WPA      0xFE

/*
 *  EAPOL-Key message, type 1-RC4
 */
typedef struct EAPOL_KEY_RC4{
    BYTE     descType;
    USHORT   keyLength;
    ULONGLONG keyReplayCtr;
    BYTE     keyIV[16];
    BYTE     keyIndex;
#define KEY_USAGE_BROADCAST 0x00
#define KEY_USAGE_UNICAST   0x80
    BYTE     keySig[16];
} EAPOL_KEY_RC4, *PEAPOL_KEY_RC4;


typedef struct EAPOL_KEY_INFO {
    union {
        struct {
#ifdef BIG_ENDIAN
            USHORT      reserved:3,
                        encrKeyData:1,  // KeyData field is encrypted
                        request:1,      // STA wants a new key
                        error:1,        // STA saw a MIC error
                        secure:1,       // set if PW keys in use
                        mic:1,          // set iff msg is MICd
                        ack:1,          // STA sets this, AP clears it
                        keyUsage:1,     // if PW, this is install bit
                                        // if group, it's rx/both bit
                        keyIndex:2,     // 802.11 key index
                        keyType:1,      // 1 = PW, 0 = group
                        type:3;         // 1 for HMAC-MD5, 2 for AES-KEYWRAP
#else
            USHORT      type:3,
                        keyType:1,
                        keyIndex:2,
                        keyUsage:1,
                        ack:1,
                        mic:1,
                        secure:1,
                        error:1,
                        request:1,
                        encrKeyData:1,
                        reserved:3;
#endif
               } k ;
        USHORT word;
    }  u;
} EAPOL_KEY_INFO;

#define EKI_TYPE_TKIP   1       // HMAC-MD5 and RC4
#define EKI_TYPE_AES    2       // HMAC_SHA1 and AES-KEYWRAP

#define EKI_KEYTYPE_PAIRWISE        1
#define EKI_KEYTYPE_GROUP           0

#define EKI_MIC_SET      1
#define EKI_SECURE_SET   1
#define EKI_ERROR_SET    1
#define EKI_REQUEST_SET  1

typedef struct EAPOL_KEY_WPA {
    BYTE      descType;
    EAPOL_KEY_INFO   keyInfo;
    USHORT    keyLength;
    ULONGLONG replayCtr;
    BYTE      keyNonce[32];
    BYTE      keyIV[16];
    ULONGLONG keyRSC;
    ULONGLONG keyID; // Not used for WPAv2
    BYTE     keyMIC[16];
    USHORT    keyDataLength;
    BYTE     keyData[1]; // 0 or more octets
} __ATTRIB_PACK EAPOL_KEY_WPA;

#define CAP_TYPE_SIZE 6
typedef struct capType {
    A_UINT8        octets[CAP_TYPE_SIZE];
}CAP_CONST;

// Encapsulation header for RFC1042 and Ethernet_tunnel
#define RFC1042SNAP    {{0xAA,0xAA,0x03,0x00,0x00,0x00}}
#define ETHERNET_TUNNEL    {{0xAA,0xAA,0x03,0x00,0x00,0xF8}}

#define APPLE_TALK_ARP    0x80f3
#define NOVELL_IPX        0x8137
#define IPV4            0x8000

    /* the data frame header that is transmitted, must be packed! */
typedef struct _AR6K_TRANSMIT_DATA_FRAME {
    HTC_FRAME_HDR            HTCFrameHeader;
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
} AR6K_TRANSMIT_DATA_FRAME;

#pragma pack()


typedef struct AR6K_TRANSMIT_MGT_INFO {
    // When the buffer is not in use, the node is used to put the
    // buffer on the free list of transmit buffers.
    LIST_ENTRY      node;

    HTC_PACKET      HtcPacket;

    // When the buffer is in use, having been passed down to HTC
    // for transmission, pTxPacket points to the NDIS_PACKET that
    // it is transmitting.
    PNDIS_PACKET pNdisPacket;

    // When the buffer is in use, cbCWMIData contains the size of
    // the variable length WMI_DATA (including MAC Header, SNAP Header,
    // and Payload data) which follows.
    ULONG           cbWMIData;

} AR6K_TRANSMIT_MGT_INFO;

    /* this macro calculates the required padding so that any structure that follows the
     * AR6K_TRANSMIT_MGT_INFO +  Pad[] will be aligned to TX_BUFFER_ALIGNMENT_BYTES.
     * It is up to the packet allocator to guarantee that the start of AR6K_TRANSMIT_BUFFER is
     * also aligned.
     *
     */

#define TX_DATA_FRAME_PAD (((sizeof(AR6K_TRANSMIT_MGT_INFO) + TX_BUFFER_ALIGNMENT_BYTES) & (~(TX_BUFFER_ALIGNMENT_BYTES - 1)))  \
                            - sizeof(AR6K_TRANSMIT_MGT_INFO))

#pragma pack(1)

/* padding blob that needs to follow AR6K_TRANSMIT_MGT_INFO structure */
typedef struct _AR6K_DATAFRAME_PAD {
    UCHAR   _PadUnused[TX_DATA_FRAME_PAD];
} AR6K_DATAFRAME_PAD;

#pragma pack()

typedef struct AR6K_TRANSMIT_BUFFER
{
    AR6K_TRANSMIT_MGT_INFO      MgtInfo;        // management info not part of the data frame
    AR6K_DATAFRAME_PAD          AlignmentPad;   // padding to align start of HTC buffer
    AR6K_TRANSMIT_DATA_FRAME    DataFrame;      // actual data frame for HTC
} AR6K_TRANSMIT_BUFFER, *PAR6K_TRANSMIT_BUFFER;

#define DEFAULT_802_11_MAX_MULTICAST_ADDRESSES          32
#define DMA_ALIGNMENT_PAD                               TX_BUFFER_ALIGNMENT_BYTES
#define DMA_ALIGNMENT_MASK                              (DMA_ALIGNMENT_PAD - 1)
#define DEFAULT_MAX_TRANSMIT_BUFFERS                    64

#define DEFAULT_MAX_TRANSMIT_BUFFER_PAYLOAD_DATA_LENGTH 1500

typedef struct WlanConfig               // Station Configuration Structure for Reading values from Registry.
{
    ULONG   bkScanEnable;               // Enable(1) or disble(0) Background Scan
    ULONG   bkScanPeriod;               // Background Scan Interval in seconds
    UCHAR   byPassWmi;                  // ByPass(1) or Pass(0) WMI Phase
    UCHAR   discTimeout;                // Disconnect timeout
    ULONG   resetPowerState;            // Power State Upon Reset
    ULONG   currentPowerState;          // Current Power State
    ULONG   ibssChannel;
    ULONG   powerSaveMode;
    ULONG   wmmConfig;                  // Enables/Disables WMM mode
    ULONG   wowEnable;                  // Enables/Disables WOW mode
    ULONG   enableUARTprint;            // Enables/Disables target UART prints.
    ULONG   skipFlash;                  // forces target to load from ROM
    ULONG   eepromUsed;                 // eeprom used to store board data
    ULONG   connectCtrlFlags;           // WMI connection flags
    ULONG   reduceCreditDribbleFlags;   // credit dribble reduction flags
    USHORT  atimWindow;                 // Set Atim window for IBSS
    UCHAR   tcmd;                       // Enable/Disable TCMD feature
    ULONG   clkFreq;                    // Configure clock frequency
    ULONG   BtCoexAntConfig;            // Bluetooth Coexistence Antenna Configuration
    ULONG   BtCoex11aDifAnt;            //Bluetooth Coexistence 11a using different antenna than 11g
    ULONG   BtDevType;                  // Co-located BT device type
    ULONG   nodeAge;                    // Configure Node Age Value
    ULONG   suspendMode;                // Power States whether deep sleep or cut power
    ULONG   wifiOnOffMode;              // WiFi ON/OFF Power state
    wchar_t binRoot[128];               // Binary RootDirectory
    wchar_t eepromFile[128];            // EEPROM file support
    ULONG   enableDbglog;               // Enable/Disable dbgLog support
    ULONG   tspecCompliance;            // Specifies if compliance w.r.t
                                        // Atheros TSpec parameters is needed.
    ULONG   ibssPSEnable;               // IBSS Power save
    ULONG   resumeDelay;                // delay in us to wait during resume
    ULONG   fgScanEnable;               // Enable(1) or disble(0) Foreground Scan
    BYTE    swMacAddr[ETHERNET_MAC_ADDRESS_LENGTH]; // Soft mac address
    ULONG   suspendDelay;               // delay in sec to wait during suspend

    UCHAR   art;                        // Enable/Disable ART feature
    A_UINT8 txPower;                    // Configure Tx Power, units of dbM
    ULONG   DbglogFlags;                // log mask and flags used with enableDbgLog
                                        //        Bits:    Meaning
                                        //        0..15  - Log Mask
                                        //        31     - Grab log on suspend.
    ULONG   hostAssistedRoaming;        // host will update the roaming table when RSSI is low even during high speed traffic
    ULONG   rekeyingPeriod;             // configuring the rekeying period in AP mode
    A_UINT32 DbgPrintMask;              // Filtering mask to enable debug messages
    ULONG   delayedRxIndThresh;         // delayed RX indication threshold count
    ULONG   intrabssEnable;             // Enable(1) or disble(0) Intra BSS communication in AP mode
    UCHAR   enableDirectRx2Ndis;        // Enable direct RX indication to NDIS rather Work queue
    UCHAR   enableActiveScan;           // enable active scan or not and default is enabled
    A_UINT8   firmwareRoaming;          // Enable(1) or disble(0) firmware roaming
    A_UINT16    actDwellTime;           //active dwell time
    A_UINT16    pasDwellTime;           // passive dwell time
    A_UINT32    drainTime;              // channel drian time
    A_UINT32    forceScanInterval;      // channel force scan interval

}WLAN_STA_CONFIG;

#define DBG_LOG_FLAGS_LOG_MASK              0xFFFF
#define DBG_LOG_FLAGS_COLLECT_ON_SUSPEND    (1 << 31)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HOST_INTEREST_ITEM_ADDRESS(target, item)    \
(((target) == TARGET_TYPE_AR6001) ?     \
   AR6001_HOST_INTEREST_ITEM_ADDRESS(item) :    \
   AR6002_HOST_INTEREST_ITEM_ADDRESS(item))

A_STATUS drvInit();

void drvDeInit();

HTC_HANDLE cmnGetHTCHandle();

void cmnGetTargetInfo(A_UINT32 *,A_UINT32 *);

A_STATUS eeprom_ar6000_transfer (HIF_DEVICE *hifDevice,
                                 A_UINT32 TargetType,
                                 wchar_t     *fileRoot,
                                 wchar_t     *eepromFile);

A_STATUS eeprom_update_macaddr(HIF_DEVICE *device,
                              A_UINT32 TargetType,
                              A_UCHAR  *mac_addr);

A_STATUS eeprom_update_rd(HIF_DEVICE *device,
                          A_UINT32 TargetType,
                          A_UINT32 rd);

A_STATUS early_init_ar6000 (HIF_DEVICE *hifDevice,
                           A_UINT32    TargetType,
                           A_UINT32    TargetVersion);

A_STATUS configure_ar6000(HIF_DEVICE *hifDevice,
                           A_UINT32    TargetType,
                           A_UINT32    TargetVersion,
                           A_BOOL      enableUART,
                           A_BOOL      timerWAR,
                           A_UINT32    clkFreq,
                           wchar_t     *filePath,
                           wchar_t     *fileRoot,
                           A_BOOL      bCompressed,
                           A_BOOL      isColdBoot,
                           wchar_t     *eepromFile);

void ar6000_set_softmac_addr(PBYTE p);

void config_exit();

//
// Common Dbglog Prototypes
//
A_STATUS GetDbgLogs();


A_STATUS drvDbgLogbuffer (DWORD dwBufferSize,
                          PBYTE   pOutBuf,
                          PDWORD  pBytesReturned);

void drvParseDebugLogs(A_INT8 *datap,
                       A_UINT32 len);

A_UINT32 dbglog_get_debug_hdr_ptr(A_UINT32 tgtType,
                                  HIF_DEVICE *device);

#ifdef HTC_EPPING_TEST
void SetupEpTest(void *HtcHandle, A_UINT32 TargetType, A_UINT32 TargetVersion);
void CleanupEpTest();
void RunPingTest();
#endif

#ifdef WINCE_ART
void ART_START();
void ART_STOP();
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
