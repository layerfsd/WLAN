//------------------------------------------------------------------------------
// <copyright file="athdrv_wince.h" company="Atheros">
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
// AR6K driver definitions
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef _ATHDRV_WINCE_H_
#define _ATHDRV_WINCE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <ntddndis.h>

#include "a_types.h"



/*                         ====BMI OIDS ====                                 */

#define OID_CAR6K_BMI_DONE                (0xFFE00001)
/*
 * arguments:
 *   UINT32 cmd
 * uses: BMI_DONE
 */

#define OID_CAR6K_BMI_READ_MEMORY         (OID_CAR6K_BMI_DONE + 1)
/*
 * arguments:
 *   union {
 *     struct {
 *       UINT32 cmd
 *       UINT32 address
 *       UINT32 length
 *     }
 *     char results[length]
 *   }
 * uses: BMI_READ_MEMORY
 */

#define OID_CAR6K_BMI_WRITE_MEMORY        (OID_CAR6K_BMI_READ_MEMORY + 1)
/*
 * arguments:
 *   UINT32 cmd
 *   UINT32 address
 *   UINT32 length
 *   char data[length]
 * uses: BMI_WRITE_MEMORY
 */

#define OID_CAR6K_BMI_EXECUTE             (OID_CAR6K_BMI_WRITE_MEMORY + 1)
/*
 * arguments:
 *   UINT32 cmd
 *   UINT32 TargetAddress
 *   UINT32 parameter
 * uses: BMI_EXECUTE
 */

#define OID_CAR6K_BMI_SET_APP_START       (OID_CAR6K_BMI_EXECUTE + 1)
/*
 * arguments:
 *   UINT32 cmd
 *   UINT32 TargetAddress
 * uses: BMI_SET_APP_START
 */

#define OID_CAR6K_BMI_READ_SOC_REGISTER   (OID_CAR6K_BMI_SET_APP_START + 1)
/*
 * arguments:
 *   union {
 *     struct {
 *       UINT32 cmd
 *       UINT32 TargetAddress, 32-bit aligned
 *     }
 *     UINT32 result
 *   }
 * uses: BMI_READ_SOC_REGISTER
 */

#define OID_CAR6K_BMI_WRITE_SOC_REGISTER  (OID_CAR6K_BMI_READ_SOC_REGISTER + 1)
/*
 * arguments:
 *     struct {
 *       UINT32 cmd
 *       UINT32 TargetAddress, 32-bit aligned
 *       UINT32 newValue
 *     }
 * uses: BMI_WRITE_SOC_REGISTER
 */

#define OID_CAR6K_BMI_TEST                (OID_CAR6K_BMI_WRITE_SOC_REGISTER + 1)
/*
 * arguments:
 *   UINT32 cmd
 *   UINT32 address
 *   UINT32 length
 *   UINT32 count
 */

#define OID_CAR6K_BMI_ROMPATCH_INSTALL    (OID_CAR6K_BMI_TEST + 1)
/*
 * arguments:
 *     union {
 *       struct {
 *         UINT32 cmd
 *         UINT32 ROM Address
 *         UINT32 RAM Address
 *         UINT32 number of bytes
 *         UINT32 activate? (0 or 1)
 *       }
 *       UINT32 resulting rompatch ID
 *     }
 * uses: BMI_ROMPATCH_INSTALL
 */

#define OID_CAR6K_BMI_ROMPATCH_UNINSTALL  (OID_CAR6K_BMI_ROMPATCH_INSTALL + 1)
/*
 * arguments:
 *     struct {
 *       UINT32 cmd
 *       UINT32 rompatch ID
 *     }
 * uses: BMI_ROMPATCH_UNINSTALL
 */

#define OID_CAR6K_BMI_ROMPATCH_ACTIVATE   (OID_CAR6K_BMI_ROMPATCH_UNINSTALL + 1)
/*
 * arguments:
 *     struct {
 *       UINT32 cmd
 *       UINT32 rompatch count
 *       UINT32 rompatch IDs[rompatch count]
 *     }
 * uses: BMI_ROMPATCH_ACTIVATE
 */

#define OID_CAR6K_BMI_ROMPATCH_DEACTIVATE (OID_CAR6K_BMI_ROMPATCH_ACTIVATE + 1)
/*
 * arguments:
 *     struct {
 *       UINT32 cmd
 *       UINT32 rompatch count
 *       UINT32 rompatch IDs[rompatch count]
 *     }
 * uses: BMI_ROMPATCH_DEACTIVATE
 */

#define BMI_OIDS_END_MARKER               (OID_CAR6K_BMI_ROMPATCH_DEACTIVATE)



/* Some version of devicescape supplicant use 0xFF000020 for setting Leap  */
#define OID_CAR6K_802_11_AUTH_ALG_1       0xFF000020

/*                         ====WMI OIDS ====                                 */
#define OID_CAR6K_FIRMWARE_VERSION        0xFF000021
#define OID_CAR6K_GET_PNP_POWER           (OID_CAR6K_FIRMWARE_VERSION + 1)
#define OID_CAR6K_RESUME_WLAN_STATE       (OID_CAR6K_GET_PNP_POWER + 1)
#define ATH_OID_802_11_SSID               (OID_CAR6K_RESUME_WLAN_STATE + 1)
#define ATH_OID_802_11_DISASSOCIATE       (ATH_OID_802_11_SSID + 1)
#define OID_CAR6K_SET_HOST_SLEEP_MODE     (ATH_OID_802_11_DISASSOCIATE + 1)
#define OID_CAR6K_SET_WOW_MODE            (OID_CAR6K_SET_HOST_SLEEP_MODE + 1)
#define OID_CAR6K_GET_WOW_LIST            (OID_CAR6K_SET_WOW_MODE + 1)
#define OID_CAR6K_ADD_WOW_PATTERN         (OID_CAR6K_GET_WOW_LIST + 1)
#define OID_CAR6K_DEL_WOW_PATTERN         (OID_CAR6K_ADD_WOW_PATTERN + 1)
#define OID_CAR6K_SET_SCAN_PARAMS         (OID_CAR6K_DEL_WOW_PATTERN + 1)
#define OID_CAR6K_SET_CHANNEL_PARAMS      (OID_CAR6K_SET_SCAN_PARAMS + 1)
#define OID_CAR6K_DBGLOG_CFG_MODULE       (OID_CAR6K_SET_CHANNEL_PARAMS + 1)
#define OID_CAR6K_DBGLOG_GET_DEBUG_LOGS   (OID_CAR6K_DBGLOG_CFG_MODULE + 1)
#define OID_CAR6K_CREATE_QOS              (OID_CAR6K_DBGLOG_GET_DEBUG_LOGS + 1)
#define OID_CAR6K_DELETE_QOS              (OID_CAR6K_CREATE_QOS + 1)
#define OID_CAR6K_GET_QOS_QUEUE           (OID_CAR6K_DELETE_QOS + 1)
#define OID_CAR6K_SET_WMM                 (OID_CAR6K_GET_QOS_QUEUE + 1)
#define OID_CAR6K_SET_TXOP                (OID_CAR6K_SET_WMM + 1)
#define OID_CAR6K_SET_BT_STATUS           (OID_CAR6K_SET_TXOP + 1)
#define OID_CAR6K_SET_BT_PARAMS           (OID_CAR6K_SET_BT_STATUS + 1)
#define OID_CAR6K_SET_WPS_ENABLE          (OID_CAR6K_SET_BT_PARAMS + 1)
#define OID_CAR6K_SET_ROAM_CTRL           (OID_CAR6K_SET_WPS_ENABLE + 1)
#define OID_CAR6K_IBSS_CHANNEL            (OID_CAR6K_SET_ROAM_CTRL + 1)
#define OID_CAR6K_SET_ATIM                (OID_CAR6K_IBSS_CHANNEL + 1)
#define OID_CAR6K_SET_IBSSPM              (OID_CAR6K_SET_ATIM + 1)
#define OID_CAR6K_SET_PM                  (OID_CAR6K_SET_IBSSPM + 1)
#define OID_CAR6K_DUMP_HTC                (OID_CAR6K_SET_PM + 1)
#define OID_CAR6K_SET_TXPOWER             (OID_CAR6K_DUMP_HTC +1)
#define OID_CAR6K_ABORT_SCAN              (OID_CAR6K_SET_TXPOWER +1)
#define OID_CAR6K_GET_RD                  (OID_CAR6K_ABORT_SCAN +1)
#define OID_CAR6K_RSSI_THRESHOLD          (OID_CAR6K_GET_RD +1)
#define OID_CAR6K_SET_APMODE              (OID_CAR6K_RSSI_THRESHOLD  +1)
#define OID_CAR6K_SET_STAMODE             (OID_CAR6K_SET_APMODE   +1)
#define OID_CAR6K_GET_CURRENT_MODE        (OID_CAR6K_SET_STAMODE +1)
#define OID_CAR6K_SET_NUM_STA             (OID_CAR6K_GET_CURRENT_MODE +1)
#define OID_CAR6K_GET_STA                 (OID_CAR6K_SET_NUM_STA  +1)
#define OID_CAR6K_ADD_DEL_ACL             (OID_CAR6K_GET_STA + 1)
#define OID_CAR6K_GET_ACL                 (OID_CAR6K_ADD_DEL_ACL + 1)
#define OID_CAR6K_SET_HIDDEN_SSID         (OID_CAR6K_GET_ACL + 1)
#define OID_CAR6K_SET_BG_PROTECTION       (OID_CAR6K_SET_HIDDEN_SSID + 1)
#define OID_CAR6K_SET_DTIM                (OID_CAR6K_SET_BG_PROTECTION + 1)
#define OID_CAR6K_SET_RD                  (OID_CAR6K_SET_DTIM + 1)
#define OID_CAR6K_SET_APMODE_XTN          (OID_CAR6K_SET_RD + 1)
#define OID_CAR6K_SET_CONN_INACT_TIME     (OID_CAR6K_SET_APMODE_XTN +1)
#define OID_CAR6K_REMOVE_STA              (OID_CAR6K_SET_CONN_INACT_TIME +1)
#define OID_CAR6K_GET_AP_STATUS           (OID_CAR6K_REMOVE_STA + 1)
#define OID_CAR6K_SET_CHANNEL             (OID_CAR6K_GET_AP_STATUS + 1)
#define OID_CAR6K_GET_AP_CRYPT            (OID_CAR6K_SET_CHANNEL + 1)
#define OID_CAR6K_GET_NUM_STA             (OID_CAR6K_GET_AP_CRYPT + 1)
#define OID_CAR6K_SET_ASSOCINFO           (OID_CAR6K_GET_NUM_STA + 1)
#define OID_CAR6K_SET_WAPIMODE            (OID_CAR6K_SET_ASSOCINFO + 1)
#define OID_CAR6K_SET_WAPIKEY             (OID_CAR6K_SET_WAPIMODE + 1)
#define OID_CAR6K_GET_BITRATE             (OID_CAR6K_SET_WAPIKEY +1)
#define OID_CAR6K_GET_TGT_STATS           (OID_CAR6K_GET_BITRATE + 1)
#define OID_CAR6K_GET_WMODE               (OID_CAR6K_GET_TGT_STATS +1)
#define OID_CAR6K_SET_APPIE               (OID_CAR6K_GET_WMODE + 1)
#define OID_CAR6K_SET_BCON_INTVL          (OID_CAR6K_SET_APPIE + 1)
#define OID_CAR6K_SET_IP                  (OID_CAR6K_SET_BCON_INTVL + 1)
#define OID_CAR6K_SET_SECURITY            (OID_CAR6K_SET_IP + 1)
#define OID_CAR6K_SET_ACL_POLICY          (OID_CAR6K_SET_SECURITY + 1)
#define OID_CAR6K_SET_INTRABSS            (OID_CAR6K_SET_ACL_POLICY + 1)
#define OID_CAR6K_SET_FIX_RATES           (OID_CAR6K_SET_INTRABSS +1)
#define OID_CAR6K_LISTEN_INTVL            (OID_CAR6K_SET_FIX_RATES + 1)
#define OID_CAR6K_SET_NODEAGE             (OID_CAR6K_LISTEN_INTVL + 1)
#define OID_CAR6K_GET_NODEAGE             (OID_CAR6K_SET_NODEAGE + 1)
#define OID_CAR6K_SET_KEEPALIVE_INTV      (OID_CAR6K_GET_NODEAGE + 1)
#define OID_CAR6K_GET_KEEPALIVE_INTV      (OID_CAR6K_SET_KEEPALIVE_INTV + 1)

/* Values from DeciveScape supplicant for CCKM/LEAP */
#define OID_CAR6K_802_11_AUTH_ALG_2          0xFF100001
#define OID_CAR6K_802_11_AUTH_ALG OID_CAR6K_802_11_AUTH_ALG_2
#define OID_CAR6K_802_11_CCKM_AUTHENTICATION_MODE 0xFF100002
#define OID_CAR6K_ADD_KRK                   0xFFEDC107

typedef enum {
    car6k802_11AuthAlgOpen = 1,
    car6k802_11AuthAlgShared = 2,
    car6k802_11AuthAlgReserved = 3,
    car6k802_11AuthAlgLeap = 4
}CAR6K_802_11_AUTH_ALG;

typedef enum {
   car6k802_11AuthModeDisableCCKM  = 1,
   car6k802_11AuthModeWPACCKM      = 2,
   car6k802_11AuthModeWPA2CCKM     = 3
}CAR6K_802_11_CCKM_AUTHENTICATION_MODE;


/* used by ATH_CAR6K_GET_QOS_QUEUE */
struct ar6000_queuereq{
    UINT8         trafficClass;
    UINT16        activeTsids;
};

typedef struct SET_WAITEVENTx
{
    HANDLE dbgLogEvent;

}SET_WAITEVENT;

typedef struct CAR6K_GET_BITRATEx
{
    ULONG ul_txRate;
    ULONG ul_rxRate;
}CAR6K_GET_BITRATE;

typedef struct CAR6K_GET_WMODEx
{
    BYTE wmode;
}CAR6K_GET_WMODE;

#ifndef OID_802_11_PMKID

// Changes for backward compatiblilty with WinCE 5.0
#define OID_802_11_CAPABILITY             0x0d010122
#define OID_802_11_PMKID                  0x0d010123

enum NDIS_802_11_STATUS_TYPE_WPA2 {
   Ndis802_11StatusType_PMKID_CandidateList = 2,
   Ndis802_11StatusType_MediaStreamMode,
   Ndis802_11StatusType_RadioState,
   //Ndis802_11StatusTypeMax
};

enum NDIS_802_11_AUTHENTICATION_MODE_WPA2 {
   Ndis802_11AuthModeWPA2=6,
   Ndis802_11AuthModeWPA2PSK,
};

typedef UCHAR NDIS_802_11_PMKID_VALUE[16];

typedef struct BSSID_INFO {
   NDIS_802_11_MAC_ADDRESS BSSID;
   NDIS_802_11_PMKID_VALUE PMKID;
} BSSID_INFO;

typedef struct NDIS_802_11_PMKID {
   ULONG Length;
   ULONG BSSIDInfoCount;
   BSSID_INFO BSSIDInfo[1];
} NDIS_802_11_PMKID;

typedef struct PMKID_CANDIDATE {
   NDIS_802_11_MAC_ADDRESS BSSID;
   ULONG Flags;
} PMKID_CANDIDATE;

#define NDIS_802_11_PMKID_CANDIDATE_PREAUTH_ENABLED 0x01

typedef struct NDIS_802_11_PMKID_CANDIDATE_LIST {
   ULONG Version;
   ULONG NumCandidates;
   PMKID_CANDIDATE CandidateList[1];
} NDIS_802_11_PMKID_CANDIDATE_LIST;

typedef struct NDIS_802_11_AUTHENTICATION_ENCRYPTION {
   NDIS_802_11_AUTHENTICATION_MODE AuthModeSupported;
   NDIS_802_11_ENCRYPTION_STATUS EncryptStatusSupported;
} NDIS_802_11_AUTHENTICATION_ENCRYPTION;

typedef struct NDIS_802_11_CAPABILITY {
   ULONG Length;
   ULONG Version;
   ULONG NoOfPMKIDs;
   ULONG NoOfAuthEncryptPairsSupported;
   NDIS_802_11_AUTHENTICATION_ENCRYPTION
   AuthenticationEncryptionSupported[1];
} NDIS_802_11_CAPABILITY;

#endif //OID_802_11_PMKID

#ifndef OID_802_11_NIC_ROAM_CONTROL
// Changes for backward compatiblilty with WinCE and Windows Mobile without Fast Roaming support

#define OID_802_11_NIC_ROAM_CONTROL             0x0D010127
#define OID_802_11_CHANNEL_LIST                 0x0D010128
#define OID_802_11_BSSID_CHANNEL_SCAN           0x0D010129
#define OID_802_11_BSSID_RATE_RSSI_FILTER       0x0D01012A
#define OID_802_11_BSSID_RATE_RSSI_INFO         0x0D01012B

#define Ndis802_11StatusType_BssidScanInfoList 0x3

//
//  Data structures used with fast roaming OIDs
//
typedef enum _NDIS_802_11_NIC_ROAM_CONTROL
{
    Ndis802_11NicRoamControlOn,
    Ndis802_11NicRoamControlOff
} NDIS_802_11_NIC_ROAM_CONTROL, * PNDIS_802_11_NIC_ROAM_CONTROL;

typedef struct _NDIS_802_11_CHANNEL_LIST
{
    ULONG NumberOfChannels;
    ULONG ChannelFrequency [1]; //variable size
} NDIS_802_11_CHANNEL_LIST, * PNDIS_802_11_CHANNEL_LIST;

typedef struct _NDIS_802_11_BSSID_CHANNEL_SCAN
{
    ULONG ChannelFrequency;  //channel frequency in kHz
    NDIS_802_11_SSID Ssid;   //SSID for which to scan
    ULONG ScanMinTime;       //min duration for scan in ms
    ULONG ScanMaxTime;       //max duration for scan in ms
} NDIS_802_11_BSSID_CHANNEL_SCAN, * PNDIS_802_11_BSSID_CHANNEL_SCAN;

typedef struct _NDIS_802_11_BSSID_RATE_RSSI_FILTER
{
    ULONG TxRateFilterParam;      //filter parameter for transmission rate
    ULONG RxRateFilterParam;     //filter parameter for receive rate
    ULONG RssiFilterParam;   //filter parameter for receive signal strength indicator
} NDIS_802_11_BSSID_RATE_RSSI_FILTER, * PNDIS_802_11_BSSID_RATE_RSSI_FILTER;

typedef struct _NDIS_802_11_BSSID_RATE_RSSI_INFO
{
    ULONG TxRateFiltered;      //filtered transmission rate in Kbits/s
    ULONG RxRateFiltered;     //filtered receive rate in Kbits/s
    ULONG TxDataFrames;   //number of transmitted data frames
    ULONG RxDataFrames;   //number of received data frames
    NDIS_802_11_RSSI RssiFiltered; //filtered RSSI in dBm
    ULONG LastRssiBeaconTime; //time in ms of the beacon used to determine RSSI
} NDIS_802_11_BSSID_RATE_RSSI_INFO, * PNDIS_802_11_BSSID_RATE_RSSI_INFO;

typedef struct  _NDIS_802_11_BSSID_SCAN_INFO
{
    ULONG ScanTime; //time of scan in ms
    NDIS_WLAN_BSSID_EX Bssid;
} NDIS_802_11_BSSID_SCAN_INFO , *PNDIS_802_11_BSSID_SCAN_INFO;

typedef struct  _NDIS_802_11_BSSID_SCAN_INFO_LIST
{
    ULONG Version;      //the version of the structure
    ULONG NumItems;  //number of elements in BssidScanInfo array
    NDIS_802_11_BSSID_SCAN_INFO BssidScanInfo[1];  //scan information for each BSSID found during scan
} NDIS_802_11_BSSID_SCAN_INFO_LIST , *PNDIS_802_11_BSSID_SCAN_INFO_LIST;
#endif


/* used by the OID_CAR6K_DBGLOG_CFG_MODULE */
typedef struct ar6000_dbglog_module_config_s {
    unsigned int valid;
    unsigned short mmask;
    unsigned short tsr;
    BOOL   rep;
    unsigned short size;
} DBGLOG_MODULE_CONFIG;


#define WMI_OIDS_END_MARKER               (OID_CAR6K_FIRMWARE_VERSION + 100)



#ifdef HTC_RAW_INTERFACE
/*                         ====HTC RAW I/F OIDS ====                        */

#define OID_CAR6K_HTC_RAW_OPEN            (WMI_OIDS_END_MARKER + 1)
#define OID_CAR6K_HTC_RAW_CLOSE           (OID_CAR6K_HTC_RAW_OPEN + 1)
#define OID_CAR6K_HTC_RAW_READ            (OID_CAR6K_HTC_RAW_CLOSE + 1)
#define OID_CAR6K_HTC_RAW_WRITE           (OID_CAR6K_HTC_RAW_READ + 1)

#endif //HTC_RAW_INTERFACE

/*                         ====Test Command OIDS ====                        */
#ifdef CONFIG_HOST_TCMD_SUPPORT

#define OID_CAR6K_TCMD_CONT_TX            (WMI_OIDS_END_MARKER + 110)
#define OID_CAR6K_TCMD_CONT_RX            (OID_CAR6K_TCMD_CONT_TX + 1)
#define OID_CAR6K_TCMD_PM                 (OID_CAR6K_TCMD_CONT_RX + 1)

#endif //CONFIG_HOST_TCMD_SUPPORT

/* Should driver perform wlan node caching? */
#define AR6000_DRIVER_CFG_GET_WLANNODECACHING   0x8001
/*Should we log raw WMI msgs */
#define AR6000_DRIVER_CFG_LOG_RAW_WMI_MSGS      0x8002
#ifdef __cplusplus
}
#endif
//
// DRG IOCTL Definitions
//

#define IOCTL_CAR6K_GET_FIRMWARE_VERSION    CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF1,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_GET_HOST_VERSION        CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF2,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_NDIS_REGISTER           CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF3,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_GET_TARGET_TYPE         CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF4,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_CONFIGURE_AR6000        CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF5,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_ART_REGISTER            CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF6, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_EPPING_REGISTER         CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF7, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_MACADDR_UPDATE          CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF8, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_RD_UPDATE               CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF9, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_DONE                CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1F1, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_READ_MEMORY         CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1F2, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_WRITE_MEMORY        CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1F3, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_EXECUTE             CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1F4, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_SET_APP_START       CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1F5, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_READ_SOC_REGISTER   CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1F6, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_WRITE_SOC_REGISTER  CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1F7, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_ROMPATCH_INSTALL    CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1F8, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_ROMPATCH_UNINSTALL  CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1F9, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_ROMPATCH_ACTIVATE   CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1FA, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_BMI_ROMPATCH_DEACTIVATE CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1FB, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_HTC_RAW_OPEN            CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1FC, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_HTC_RAW_CLOSE           CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1FD, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_HTC_RAW_READ            CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1FE, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_HTC_RAW_WRITE           CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x1FF, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_EP_PINGTEST             CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x2F1, METHOD_NEITHER, FILE_ANY_ACCESS)




#define IOCTL_CAR6K_GET_DBGLOG              CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x2F2, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_GRAB_DBGLOG             CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x2F3, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_DUMP_CHIP_MEM           CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x2F4, METHOD_NEITHER, FILE_ANY_ACCESS)

#ifdef AR6K_MTE_DRV_TEST
#define IOCTL_CAR6K_BUS_PASSTHRU_CMD        CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0x2F6, METHOD_NEITHER, FILE_ANY_ACCESS)
#endif



//
// Windows CE Ioctl / Oids common structure definitions
//
#define MAX_BUF         (8*1024)

#pragma pack (1)
// OIDs specific structures
typedef struct bmi_read_memory_param
{
    DWORD address;
    DWORD length;
    UCHAR databuffer[1];   /* start of variable length data buffer */
} BMI_READ_MEMORY_PARAM;

typedef BMI_READ_MEMORY_PARAM BMI_WRITE_MEMORY_PARAM;

typedef struct bmi_read_soc_reg_param
{
    DWORD address;
    DWORD value;
} BMI_READ_SOC_REG_PARAM;

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

typedef struct bmi_rompatch_install_param
{
    DWORD romAddr;
    DWORD ramAddr;
    DWORD nBytes;
    DWORD doActivate;
    DWORD romPatchId;
} BMI_ROMPATCH_INSTALL_PARAM;

typedef struct bmi_rompatch_uninstall_param
{
    DWORD romPatchId;
} BMI_ROMPATCH_UNINSTALL_PARAM;

typedef struct bmi_rompatch_activate_param
{
    DWORD romPatchCount;
    DWORD buffer[1];
} BMI_ROMPATCH_ACTIVATE_PARAM;

typedef BMI_ROMPATCH_ACTIVATE_PARAM  BMI_ROMPATCH_DEACTIVATE_PARAM;

typedef struct htc_raw_read_param
{
    DWORD endPointId;
    DWORD length;
    CHAR buffer[1];
} HTC_RAW_READ_PARAM;

typedef struct htc_raw_write_param
{
    DWORD endPointId;
    DWORD length;
    CHAR buffer[1];
} HTC_RAW_WRITE_PARAM;

typedef struct configure_ar6000_param
{
    BOOL      enableUART;
    BOOL      timerWAR;
    UINT32    clkFreq;
    wchar_t   filePath[128];
    wchar_t   fileRoot[128];
    BOOL      bCompressed;
    BOOL      isColdBoot;
    wchar_t   eepromFile[128];
} CONFIGURE_AR6000_PARAM;

typedef struct user_rssi_thold_t {
    A_INT16     tag;
    A_INT16     rssi;
} USER_RSSI_THOLD;

typedef struct user_rssi_params_t {
    A_UINT8            weight;
    A_UINT32           pollTime;
    USER_RSSI_THOLD    tholds[12];
} USER_RSSI_PARAMS;

#ifdef WAPI_ENABLE
typedef enum  _KEY_TYPE
{
    ktPairwiseKey = 0,
    ktGroupKey
}KEY_TYPE;

typedef enum _WPI_PROTECT_TYPE
{
    None,
    Rx,
    Tx,
    Rx_Tx
} WPI_PROTECT_TYPE, *PWPI_PROTECT_TYPE;


typedef struct  _WPI_KEY
{
    KEY_TYPE           KeyType;
    WPI_PROTECT_TYPE   Direction;
    BYTE               KeyID;
    BYTE               AddrIndex[12];
    int                lenWPIEK;
    BYTE               WPIEK[256];
    int                lenWPICK;
    BYTE               WPICK[256];
    BYTE               PN[16];
} WPI_KEY;
#endif /* WAPI_ENABLE */


#pragma pack()

#ifdef AR6K_MTE_DRV_TEST
#define SDIO_CMD_RESPONSE_NONE  0
#define SDIO_CMD_RESPONSE_R1    1
#define SDIO_CMD_RESPONSE_R1b   2
#define SDIO_CMD_RESPONSE_R4    4
#define SDIO_CMD_RESPONSE_R5    5
#define SDIO_CMD_RESPONSE_R6    6


#pragma pack (1)

typedef struct _AR6K_SDIO_BUS_REQUEST {
    DWORD   Command;
    DWORD   Argument;
    UCHAR   ResponseBuffer[20];
    DWORD   ResponseType;
    DWORD   DataTransfer;
    DWORD   DataWrite;
    DWORD   DataBlocks;
    DWORD   DataBlockLength;
    UCHAR   Buffer[1];  /* start of read/write buffer */
} AR6K_SDIO_BUS_REQUEST;


typedef struct _AR6K_SDIO_CLOCK_REQUEST {
    DWORD ClockRate;
} AR6K_SDIO_CLOCK_REQUEST;

typedef struct _AR6K_SDIO_BUS_WIDTH_REQUEST {
    UCHAR BusWidth;  /* 1 = 1bit, 4 = 4 bit */
} AR6K_SDIO_BUS_WIDTH_REQUEST;

typedef enum _AR6K_SDIO_CMD_TYPE {
    AR6K_CMD_TYPE_BUS = 0,
    AR6K_CMD_TYPE_CLK_CONFIG,
    AR6K_CMD_TYPE_BUS_WIDTH_CONFIG,
    AR6K_CMD_TYPE_FULL_RESET
}AR6K_SDIO_CMD_TYPE;

typedef struct _AR6K_SDIO_CMD {
    AR6K_SDIO_CMD_TYPE              CommandType; /* must be first */
    union _SDIO_CMD {
        AR6K_SDIO_BUS_REQUEST       AsBusRequest;
        AR6K_SDIO_CLOCK_REQUEST     AsClockRequest;
        AR6K_SDIO_BUS_WIDTH_REQUEST AsBusWidthRequest;
    } Request;
} AR6K_SDIO_CMD;

#pragma pack()

#endif //AR6K_MTE_DRV_TEST

/* AP Network related Data structures */
#define WLAN_REASON_INVALID_IE                  13
#define WLAN_REASON_MICHAEL_MIC_FAILURE         14
#define WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT      15
#define WLAN_REASON_PAIRWISE_CIPHER_NOT_VALID   19

typedef struct station{
    A_UINT16  flags;
    A_UINT8   mac[6];
    A_UINT16   aid;
    A_UINT8   wpa_ie[256];
    LIST_ENTRY ucastq;
    A_UINT8   m_keyState;
    A_UINT8   ANonce[32];
    A_UINT8   SNonce[32];
    A_UINT8   PTK[64];
    A_UINT16  PTKlength;
    A_BOOL    hnd_shk_completed;
}STATION;

#define MAX_NUM_STA 8

typedef struct {
    STATION sta[MAX_NUM_STA];
}ap_get_sta;

#define DFT_AP_BEACON_INTVL 100
#define DFT_AP_DTIM_PERIOD  5
#define DFT_AP_CHANNEL      2412

#define BEACON_INTVL_MIN   20
#define BEACON_INTVL_MAX   4000

typedef struct {
    A_UCHAR     ssid[32];
    A_UINT8     ssidLength;
    A_UINT16    channel;
    A_UINT8     dtim;
    BOOL        bgprotection;
    BOOL        hidden_ssid;
    BOOL        IntraBss;
    A_UINT16    beaconInterval;
}AP_COMMIT_XTND;

#define MAX_WPA_ASCII_KEY_SIZE 32
#define PACKET_MIC_SIZE        16
typedef struct {
    ULONG    Length;
    ULONG    KeyIndex;
    ULONG    KeyLength;
    A_UINT8  cryptType;
    A_UINT8  Authmode;
    A_UINT8  dot11_authmode;
    A_BOOL   IsAscii;
    UCHAR    KeyMaterial [1];
}AP_MODE_CRYPT;

typedef struct {
    A_UINT8     num_sta;
} AP_GET_NUM_STA_CMD;

typedef struct {
    A_BOOL  intrabss;
} AP_SET_INTRA_BSS;

typedef struct {
    A_UINT16 channel;
} AP_SET_CHANNEL_CMD;
#define STA_PWR_MGMT_MASK 0x1
#define STA_PWR_MGMT_SHIFT 0x0
#define STA_PWR_MGMT_AWAKE 0x0
#define STA_PWR_MGMT_SLEEP 0x1

#define STA_SET_PWR_SLEEP(sta) (sta->flags |= (STA_PWR_MGMT_MASK << STA_PWR_MGMT_SHIFT))
#define STA_CLR_PWR_SLEEP(sta) (sta->flags &= ~(STA_PWR_MGMT_MASK << STA_PWR_MGMT_SHIFT))
#define STA_IS_PWR_SLEEP(sta) ((sta->flags >> STA_PWR_MGMT_SHIFT) & STA_PWR_MGMT_MASK)

#define STA_PS_POLLED_MASK 0x1
#define STA_PS_POLLED_SHIFT 0x1
#define STA_SET_PS_POLLED(sta) (sta->flags |= (STA_PS_POLLED_MASK << STA_PS_POLLED_SHIFT))
#define STA_CLR_PS_POLLED(sta) (sta->flags &= ~(STA_PS_POLLED_MASK << STA_PS_POLLED_SHIFT))
#define STA_IS_PS_POLLED(sta) (sta->flags & (STA_PS_POLLED_MASK << STA_PS_POLLED_SHIFT))

// Grab more Target stats to NDIS
// Holds target stats (fields not available in NDIS_802_11_STATS)

typedef struct _AR6K_TARGET_STATS{
    A_UINT32   tx_packets; /* Tx Stats */
    A_UINT32   tx_bytes;
    A_UINT32   tx_unicast_pkts;
    A_UINT32   tx_unicast_bytes;
    A_UINT32   tx_multicast_pkts;
    A_UINT32   tx_multicast_bytes;
    A_UINT32   tx_broadcast_pkts;
    A_UINT32   tx_broadcast_bytes;
    A_UINT32   tx_rts_success_cnt;
    A_UINT32   tx_packet_per_ac[4];
    A_UINT32   tx_errors_per_ac[4];
    A_UINT32   tx_errors;
    A_UINT32   tx_failed_cnt;
    A_UINT32   tx_retry_cnt;
    A_UINT32   tx_mult_retry_cnt;
    A_UINT32   tx_rts_fail_cnt;
    A_INT32    tx_unicast_rate;
    A_UINT32   rx_packets; /* Rx Stats */
    A_UINT32   rx_bytes;
    A_UINT32   rx_unicast_pkts;
    A_UINT32   rx_unicast_bytes;
    A_UINT32   rx_multicast_pkts;
    A_UINT32   rx_multicast_bytes;
    A_UINT32   rx_broadcast_pkts;
    A_UINT32   rx_broadcast_bytes;
    A_UINT32   rx_fragment_pkt;
    A_UINT32   rx_errors;
    A_UINT32   rx_crcerr;
    A_UINT32   rx_key_cache_miss;
    A_UINT32   rx_decrypt_err;
    A_UINT32   rx_duplicate_frames;
    A_INT32    rx_unicast_rate;
    A_UINT32   power_save_failure_cnt; /* power save count */
    A_UINT32   tkip_local_mic_failure; /* Tkip, ccmp*/
    A_UINT32   tkip_counter_measures_invoked;
    A_UINT32   tkip_replays;
    A_UINT32   tkip_format_errors;
    A_UINT32   ccmp_format_errors;
    A_UINT32   ccmp_replays;
    A_UINT32    cs_bmiss_cnt; /* Cserv stats */
    A_UINT32    cs_lowRssi_cnt;
    A_UINT16    cs_connect_cnt;
    A_UINT16    cs_disconnect_cnt;
    A_INT16     cs_aveBeacon_rssi;
    A_UINT16    cs_roam_count;
    A_INT16     cs_rssi;
    A_UINT8     cs_snr;
    A_UINT8     cs_aveBeacon_snr;
    A_UINT8     cs_lastRoam_msec;
    A_UINT32    wow_num_pkts_dropped; /* WOW */
    A_UINT16    wow_num_events_discarded;
    A_UINT8     wow_num_host_pkt_wakeups;
    A_UINT8     wow_num_host_event_wakeups;
    A_UINT32    lqVal;
    A_INT32     noise_floor_calibation;
    A_INT32     length;
} AR6K_TARGET_STATS;

#endif /* _ATHDRV_WINCE_H_ */
