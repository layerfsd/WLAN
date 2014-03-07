//------------------------------------------------------------------------------
// <copyright file="wmi.h" company="Atheros">
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
// This file contains the definitions of the WMI protocol specified in the
// Wireless Module Interface (WMI).  It includes definitions of all the
// commands and events. Commands are messages from the host to the WM.
// Events and Replies are messages from the WM to the host.
//
// Author(s): ="Atheros"
//==============================================================================
/*
 * Ownership of correctness in regards to WMI commands
 * belongs to the host driver and the WM is not required to validate
 * parameters for value, proper range, or any other checking.
 *
 */

#ifndef _WMI_H_
#define _WMI_H_

#include "wmix.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UNDER_CE
#pragma pack (1)
#endif

#define WMI_PROTOCOL_VERSION    0x0001
#define WMI_PROTOCOL_REVISION   0x0000

#define ATH_MAC_LEN             6               /* length of mac in bytes */
#define WMI_CMD_MAX_LEN         100
#define WMI_CONTROL_MSG_MAX_LEN     256
#define WMI_OPT_CONTROL_MSG_MAX_LEN 1536
#define IS_ETHERTYPE(_typeOrLen)        ((_typeOrLen) >= 0x0600)
#define RFC1042OUI      {0x00, 0x00, 0x00}

#define IP_ETHERTYPE 0x0800

/*
 * Data Path
 */
typedef struct {
    A_UINT8     dstMac[ATH_MAC_LEN];
    A_UINT8     srcMac[ATH_MAC_LEN];
    A_UINT16    typeOrLen;
} ATH_MAC_HDR;

typedef struct {
    A_UINT8     dsap;
    A_UINT8     ssap;
    A_UINT8     cntl;
    A_UINT8     orgCode[3];
    A_UINT16    etherType;
} ATH_LLC_SNAP_HDR;

typedef enum {
    DATA_MSGTYPE = 0x0,
    CNTL_MSGTYPE,
    SYNC_MSGTYPE
} WMI_MSG_TYPE;


#ifdef AR6K_FIRMWARE_1_0

typedef struct {
    A_UINT8     reserved1;
    A_UINT8     msgType;            /* see WMI_MSG_TYPE */
} WMI_DATA_HDR;
#define WMI_DATA_HDR_SET_MSG_TYPE(h, t) (h)->msgType = (t)
#define WMI_DATA_HDR_SET_UP(h, p)

#else // Firware 1.1

typedef struct {
    A_INT8      rssi;            
    A_UINT8     info;            /* WMI_MSG_TYPE in lower 2 bits - b1b0 */
                                 /* UP in next 3 bits - b4b3b2 */
#define WMI_DATA_HDR_MSG_TYPE_MASK  0x03
#define WMI_DATA_HDR_MSG_TYPE_SHIFT 0
#define WMI_DATA_HDR_UP_MASK        0x07
#define WMI_DATA_HDR_UP_SHIFT       2	
#define WMI_DATA_HDR_IS_MSG_TYPE(h, t)  (((h)->info & (WMI_DATA_HDR_MSG_TYPE_MASK)) == (t))
} WMI_DATA_HDR;

#define WMI_DATA_HDR_SET_MSG_TYPE(h, t) (h)->info = (((h)->info & ~(WMI_DATA_HDR_MSG_TYPE_MASK << WMI_DATA_HDR_MSG_TYPE_SHIFT)) | (t << WMI_DATA_HDR_MSG_TYPE_SHIFT))
#define WMI_DATA_HDR_SET_UP(h, p) (h)->info = (((h)->info & ~(WMI_DATA_HDR_UP_MASK << WMI_DATA_HDR_UP_SHIFT)) | (p << WMI_DATA_HDR_UP_SHIFT))

#endif //AR6K_FIRMWARE_1_1

/*
 * Control Path
 */
typedef struct {
    A_UINT16    commandId;
} WMI_CMD_HDR;        /* used for commands and events */

/*
 * List of Commnands
 */
typedef enum {
    WMI_CONNECT_CMDID           = 0x0001,
    WMI_RECONNECT_CMDID,
    WMI_DISCONNECT_CMDID,
    WMI_SYNCHRONIZE_CMDID,
    WMI_CREATE_PSTREAM_CMDID,
    WMI_DELETE_PSTREAM_CMDID,
    WMI_START_SCAN_CMDID,
    WMI_SET_SCAN_PARAMS_CMDID,
    WMI_SET_BSS_FILTER_CMDID,
    WMI_SET_PROBED_SSID_CMDID,
    WMI_SET_LISTEN_INT_CMDID,
    WMI_SET_BMISS_TIME_CMDID,
    WMI_SET_DISC_TIMEOUT_CMDID,
    WMI_GET_CHANNEL_LIST_CMDID,
    WMI_SET_BEACON_INT_CMDID,
    WMI_GET_STATISTICS_CMDID,
    WMI_SET_CHANNEL_PARAMS_CMDID,
    WMI_SET_POWER_MODE_CMDID,
    WMI_SET_IBSS_PM_CAPS_CMDID,
    WMI_SET_POWER_PARAMS_CMDID,
#ifndef AR6K_FIRMWARE_1_0
    WMI_SET_POWERSAVE_TIMERS_CMDID,
#endif
    WMI_ADD_CIPHER_KEY_CMDID,
    WMI_DELETE_CIPHER_KEY_CMDID,
#ifndef AR6K_FIRMWARE_1_0
    WMI_ADD_KRK_CMDID,
    WMI_DELETE_KRK_CMDID,
#endif
    WMI_SET_PMKID_CMDID,
    WMI_SET_TX_PWR_CMDID,
    WMI_GET_TX_PWR_CMDID,
    WMI_SET_ASSOC_INFO_CMDID,
    WMI_ADD_BAD_AP_CMDID,
    WMI_DELETE_BAD_AP_CMDID,
    WMI_SET_TKIP_COUNTERMEASURES_CMDID,
    WMI_RSSI_THRESHOLD_PARAMS_CMDID,
    WMI_TARGET_ERROR_REPORT_BITMASK_CMDID,
    WMI_SET_ACCESS_PARAMS_CMDID,
    WMI_SET_RETRY_LIMITS_CMDID,
    WMI_SET_OPT_MODE_CMDID,
    WMI_OPT_TX_FRAME_CMDID,
#ifndef AR6K_FIRMWARE_1_0
    WMI_SET_VOICE_PKT_SIZE_CMDID,
    WMI_SET_MAX_SP_LEN_CMDID,
    WMI_SET_ROAM_CTRL_CMDID,
    WMI_GET_ROAM_TBL_CMDID,
    WMI_GET_ROAM_DATA_CMDID,
    WMI_ENABLE_RM_CMDID,
    WMI_SET_MAX_OFFHOME_DURATION_CMDID,
#endif
    WMI_EXTENSION_CMDID,                        /* Non-wireless extensions */

#ifndef AR6K_FIRMWARE_1_0
    WMI_SNR_THRESHOLD_PARAMS_CMDID,
    WMI_LQ_THRESHOLD_PARAMS_CMDID,
    WMI_SET_LPREAMBLE_CMDID,
    WMI_SET_RTS_CMDID,
    WMI_CLR_RSSI_SNR_CMDID,
    WMI_SET_FIXRATES_CMDID,
    WMI_GET_FIXRATES_CMDID,
    WMI_SET_AUTH_MODE_CMDID,
#endif

    /*
     * Developer commands starts at 0xF000
     */
    WMI_SET_BITRATE_CMDID = 0xF000,
    WMI_GET_BITRATE_CMDID,
} WMI_COMMAND_ID;

/*
 * Connect Command
 */
typedef enum {
    INFRA_NETWORK       = 0x01,
    ADHOC_NETWORK       = 0x02,
} NETWORK_TYPE;

typedef enum {
    OPEN_AUTH           = 0x01,
    SHARED_AUTH         = 0x02,
#ifdef AR6K_FIRMWARE_1_0
    SHARED_OPEN_AUTH    = 0x03,
#endif
    LEAP_AUTH           = 0x04,
} DOT11_AUTH_MODE;

typedef enum {
    NONE_AUTH           = 0x01, 
    WPA_AUTH            = 0x02,
    WPA_PSK_AUTH        = 0x03,
    WPA2_AUTH           = 0x04,
    WPA2_PSK_AUTH       = 0x05,
    WPA_AUTH_CCKM       = 0x06,
    WPA2_AUTH_CCKM      = 0x07,
} AUTH_MODE;


typedef enum {
    NONE_CRYPT          = 0x01,
    WEP_CRYPT           = 0x02,
    TKIP_CRYPT          = 0x03,
    AES_CRYPT           = 0x04,
} CRYPTO_TYPE;

#define WMI_MIN_CRYPTO_TYPE NONE_CRYPT
#define WMI_MAX_CRYPTO_TYPE (AES_CRYPT + 1)

#define WMI_MIN_KEY_INDEX   0
#define WMI_MAX_KEY_INDEX   3

#define WMI_MAX_KEY_LEN     32

#define WMI_MAX_SSID_LEN    32

typedef struct {
    A_UINT8     networkType;
    A_UINT8     dot11AuthMode;
    A_UINT8     authMode;
    A_UINT8     pairwiseCryptoType;
#ifndef AR6K_FIRMWARE_1_0
    A_UINT8     pairwiseCryptoLen;
#endif
    A_UINT8     groupCryptoType;
#ifndef AR6K_FIRMWARE_1_0
    A_UINT8     groupCryptoLen;
#endif
    A_UINT8     ssidLength;
    A_UCHAR     ssid[WMI_MAX_SSID_LEN];
    A_UINT16    channel;
    A_UINT8     bssid[ATH_MAC_LEN];
} WMI_CONNECT_CMD;

/*
 * WMI_RECONNECT_CMDID
 */
typedef struct {
    A_UINT16    channel;                    /* hint */
    A_UINT8     bssid[ATH_MAC_LEN];         /* mandatory if set */
} WMI_RECONNECT_CMD;

/*
 * WMI_ADD_CIPHER_KEY_CMDID
 */
typedef enum {
    PAIRWISE_USAGE      = 0x00,
    GROUP_USAGE         = 0x01,
    TX_USAGE            = 0x02,     /* default Tx Key - Static WEP only */
} KEY_USAGE;

typedef struct {
    A_UINT8     keyIndex;
    A_UINT8     keyType;
    A_UINT8     keyUsage;           /* KEY_USAGE */
    A_UINT8     keyLength;
    A_UINT8     keyRSC[8];          /* key replay sequence counter */
    A_UINT8     key[WMI_MAX_KEY_LEN];
} WMI_ADD_CIPHER_KEY_CMD;

/*
 * WMI_DELETE_CIPHER_KEY_CMDID
 */
typedef struct {
    A_UINT8     keyIndex;
} WMI_DELETE_CIPHER_KEY_CMD;

#define WMI_KRK_LEN     16
/*
 * WMI_ADD_KRK_CMDID
 */
typedef struct {
    A_UINT8     krk[WMI_KRK_LEN];
} WMI_ADD_KRK_CMD;

/*
 * WMI_SET_TKIP_COUNTERMEASURES_CMDID
 */
typedef enum {
    WMI_TKIP_CM_DISABLE = 0x0,
    WMI_TKIP_CM_ENABLE  = 0x1,
} WMI_TKIP_CM_CONTROL;

typedef struct {
    A_UINT8  cm_en;                     /* WMI_TKIP_CM_CONTROL */
} WMI_SET_TKIP_COUNTERMEASURES_CMD;

/*
 * WMI_SET_PMKID_CMDID
 */
typedef enum {
   PMKID_DISABLE = 0, 
   PMKID_ENABLE  = 1, 
} PMKID_ENABLE_FLG;

typedef struct {
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT8     enable;                 /* PMKID_ENABLE_FLG */
    A_UINT8     pmkid[16];
} WMI_SET_PMKID_CMD;

/*
 * WMI_START_SCAN_CMD
 */
typedef enum {
    WMI_LONG_SCAN  = 0,
    WMI_SHORT_SCAN = 1,
} WMI_SCAN_TYPE;

typedef struct {
    A_UINT8 scanType;           /* WMI_SCAN_TYPE */
} WMI_START_SCAN_CMD;

/*
 * WMI_SET_SCAN_PARAMS_CMDID
 */
#define WMI_SHORTSCANRATIO_DEFAULT      3
typedef struct {
    A_UINT16    fg_start_period;        /* seconds */
    A_UINT16    fg_end_period;          /* seconds */
    A_UINT16    bg_period;              /* seconds */
    A_UINT16    act_chdwell_time;       /* msec */
    A_UINT16    pas_chdwell_time;       /* msec */
    A_UINT8     shortScanRatio;         /* how many shorts scan for one long */
} WMI_SCAN_PARAMS_CMD;

/*
 * WMI_SET_BSS_FILTER_CMDID
 */
typedef enum {
    NONE_BSS_FILTER = 0x0,              /* no beacons forwarded */
    ALL_BSS_FILTER,                     /* all beacons forwarded */
    PROFILE_FILTER,                     /* only beacons matching profile */
    ALL_BUT_PROFILE_FILTER,             /* all but beacons matching profile */
    CURRENT_BSS_FILTER,                 /* only beacons matching current BSS */
    ALL_BUT_BSS_FILTER,                 /* all but beacons matching BSS */
    PROBED_SSID_FILTER,                 /* beacons matching probed ssid */
    LAST_BSS_FILTER,                    /* marker only */
} WMI_BSS_FILTER;

typedef struct {
    A_UINT8    bssFilter;                      /* see WMI_BSS_FILTER */
} WMI_BSS_FILTER_CMD;

/*
 * WMI_SET_PROBED_SSID_CMDID
 */
#define MAX_PROBED_SSID_INDEX   5

typedef enum {
    DISABLE_SSID_FLAG  = 0,                  /* disables entry */
    SPECIFIC_SSID_FLAG = 0x01,               /* probes specified ssid */
    ANY_SSID_FLAG      = 0x02,               /* probes for any ssid */
} WMI_SSID_FLAG;

typedef struct {
    A_UINT8     entryIndex;                     /* 0 to MAX_PROBED_SSID_INDEX */
    A_UINT8     flag;                           /* WMI_SSID_FLG */
    A_UINT8     ssidLength;
    A_UINT8     ssid[32];
} WMI_PROBED_SSID_CMD;

/*
 * WMI_SET_LISTEN_INT_CMDID
 * The Listen interval is between 100 and 1000 TUs
 */
#define MIN_LISTEN_INTERVAL 100
#define MAX_LISTEN_INTERVAL 1000
#define MIN_LISTEN_BEACONS 1
#define MAX_LISTEN_BEACONS 50

typedef struct {
    A_UINT16     listenInterval;
#ifndef AR6K_FIRMWARE_1_0
    A_UINT16     numBeacons;
#endif
} WMI_LISTEN_INT_CMD;

/*
 * WMI_SET_BEACON_INT_CMDID
 */
typedef struct {
    A_UINT16     beaconInterval;
} WMI_BEACON_INT_CMD;

/*
 * WMI_SET_BMISS_TIME_CMDID
 * valid values are between 1000 and 5000 TUs
 */

#define MIN_BMISS_TIME     1000
#define MAX_BMISS_TIME     5000
#define MIN_BMISS_BEACONS  5 
#define MAX_BMISS_BEACONS  50

typedef struct {
    A_UINT16     bmissTime;
#ifndef AR6K_FIRMWARE_1_0
    A_UINT16     numBeacons;
#endif
} WMI_BMISS_TIME_CMD;

/*
 * WMI_SET_POWER_MODE_CMDID
 */
typedef enum {
    REC_POWER = 0x01,
    MAX_PERF_POWER,
} WMI_POWER_MODE;

typedef struct {
    A_UINT8     powerMode;      /* WMI_POWER_MODE */
} WMI_POWER_MODE_CMD;

/*
 * WMI_SET_POWER_PARAMS_CMDID
 */
typedef enum {
    IGNORE_DTIM = 0x01,
    NORMAL_DTIM = 0x02,
    STICK_DTIM  = 0x03,
} WMI_DTIM_POLICY;

typedef struct {
    A_UINT16    idle_period;             /* msec */
    A_UINT16    pspoll_number;
    A_UINT16    dtim_policy;
} WMI_POWER_PARAMS_CMD;

typedef struct {
    A_UINT8    power_saving;            
#ifndef AR6K_FIRMWARE_1_0
    A_UINT8    ttl; /* number of beacon periods */
#endif
    A_UINT16   atim_windows;          /* msec */
#ifndef AR6K_FIRMWARE_1_0
    A_UINT16   timeout_value;         /* msec */ 
#endif
} WMI_IBSS_PM_CAPS_CMD;

/*
 * WMI_SET_POWERSAVE_TIMERS_CMDID
 */
typedef struct {
    A_UINT16    psPollTimeout;          /* msec */
    A_UINT16    eospTimeout;            /* msec */
} WMI_POWERSAVE_TIMERS_CMD;

/*
 * WMI_SET_VOICE_PKT_SIZE_CMDID
 */
typedef struct {
    A_UINT16    voicePktSize;
} WMI_SET_VOICE_PKT_SIZE_CMD;

/*
 * WMI_SET_MAX_SP_LEN_CMDID
 */
typedef enum {
    DELIVER_ALL_PKT = 0x0,
    DELIVER_2_PKT = 0x1,
    DELIVER_4_PKT = 0x2,
    DELIVER_6_PKT = 0x3,
} APSD_SP_LEN_TYPE;

typedef struct {
    A_UINT8    maxSPLen;
} WMI_SET_MAX_SP_LEN_CMD;

/*
 * WMI_SET_DISC_TIMEOUT_CMDID
 */
typedef struct {
    A_UINT8     disconnectTimeout;          /* seconds */
} WMI_DISC_TIMEOUT_CMD;

typedef enum {
    UPLINK_TRAFFIC = 0,
    DNLINK_TRAFFIC = 1,
    BIDIR_TRAFFIC = 2,
} DIR_TYPE;
/*
 * WMI_CREATE_PSTREAM_CMDID
 */
typedef enum {
    DISABLE_FOR_THIS_AC = 0,
    ENABLE_FOR_THIS_AC  = 1,
    ENABLE_FOR_ALL_AC   = 2,
} VOICEPS_CAP_TYPE;

typedef enum {
    TRAFFIC_TYPE_APERIODIC = 0,
    TRAFFIC_TYPE_PERIODIC = 1,
}TRAFFIC_TYPE;

typedef struct {
    A_UINT8         trafficClass;
    A_UINT8         trafficDirection;        /* DIR_TYPE */
    A_UINT8         rxQueueNum;
#ifndef AR6K_FIRMWARE_1_0
    A_UINT8         trafficType;             /* TRAFFIC_TYPE */
#endif
    A_UINT8         voicePSCapability;       /* VOICEPS_CAP_TYPE */
#ifndef AR6K_FIRMWARE_1_0
    A_UINT8         tsid;
    A_UINT8         userPriority;            /* 802.1D user priority */
    A_UINT16        nominalMSDU;             /* in octects */
    A_UINT16        maxMSDU;                 /* in octects */
#endif
    A_UINT32        minServiceInt;           /* in milli-sec */
    A_UINT32        maxServiceInt;           /* in milli-sec */
    A_UINT32        inactivityInt;           /* in milli-sec */
#ifndef AR6K_FIRMWARE_1_0
    A_UINT32        suspensionInt;           /* in milli-sec */
    A_UINT32        serviceStartTime;
    A_UINT32        minDataRate;             /* in bps */
    A_UINT32        meanDataRate;            /* in bps */
    A_UINT32        peakDataRate;            /* in bps */
    A_UINT32        maxBurstSize;
    A_UINT32        delayBound;
    A_UINT32        minPhyRate;              /* in bps */
    A_UINT32        sba;
    A_UINT32        mediumTime;
#endif
} WMI_CREATE_PSTREAM_CMD;

/*
 * WMI_DELETE_PSTREAM_CMDID
 */
typedef struct {
    A_UINT8     txQueueNumber;
    A_UINT8     rxQueueNumber;
    A_UINT8     trafficDirection;
    A_UINT8     trafficClass;
} WMI_DELETE_PSTREAM_CMD;

/*
 * WMI_SET_CHANNEL_PARAMS_CMDID
 */
typedef enum {
    WMI_MIN_PHY_MODE = 0,
    WMI_11A_MODE  = 0x1,
    WMI_11G_MODE  = 0x2,
    WMI_11AG_MODE = 0x3,
    WMI_11B_MODE  = 0x4,
    WMI_11GONLY_MODE = 0x5,
    WMI_MAX_PHY_MODE
} WMI_PHY_MODE;

#define WMI_MAX_CHANNELS        32

typedef struct {
    A_UINT8     phyMode;                /* see WMI_PHY_MODE */
    A_UINT8     numChannels;            /* how many channels follow */
    A_UINT16    channelList[1];         /* channels in Mhz */
} WMI_CHANNEL_PARAMS_CMD;


/*
 *  WMI_RSSI_THRESHOLD_PARAMS_CMDID
 *  Setting the polltime to 0 would disable polling. 
 *  Threshold values are in the ascending order, and should agree to:
 *  (lowThreshold_lowerVal < lowThreshold_upperVal < highThreshold_lowerVal
 *      < highThreshold_upperVal)
 */

typedef struct WMI_RSSI_THRESHOLD_PARAMS{
    A_UINT8     highThreshold_upperVal;
    A_UINT8     highThreshold_lowerVal;
    A_UINT8     lowThreshold_upperVal;
    A_UINT8     lowThreshold_lowerVal;
    A_UINT32    pollTime;               /* Polling time in seconds */
} WMI_RSSI_THRESHOLD_PARAMS_CMD;

/*
 *  WMI_TARGET_ERROR_REPORT_BITMASK_CMDID
 *  Sets the error reporting event bitmask in target. Target clears it
 *  upon an error. Subsequent errors are counted, but not reported
 *  via event, unless the bitmask is set again. 
 */
typedef struct {
    A_UINT32    bitmask;
} WMI_TARGET_ERROR_REPORT_BITMASK;

/*
 * WMI_SET_TX_PWR_CMDID
 */
typedef struct {
    A_UINT8     dbM;                  /* in dbM units */
} WMI_SET_TX_PWR_CMD, WMI_TX_PWR_REPLY;

/*
 * WMI_SET_ASSOC_INFO_CMDID
 * 
 * A maximum of 2 private IEs can be sent in the [Re]Assoc request.
 * A 3rd one, the CCX version IE can also be set from the host.
 */
#define WMI_MAX_ASSOC_INFO_TYPE    2
#define WMI_CCX_VER_IE             2 /* ieType to set CCX Version IE */

#define WMI_MAX_ASSOC_INFO_LEN     240 

typedef struct {
    A_UINT8     ieType;
    A_UINT8     bufferSize;
    A_UINT8     assocInfo[1];       /* up to WMI_MAX_ASSOC_INFO_LEN */
} WMI_SET_ASSOC_INFO_CMD;


/*
 * WMI_GET_TX_PWR_CMDID does not take any parameters
 */

/*
 * WMI_ADD_BAD_AP_CMDID
 */
#define WMI_MAX_BAD_AP_INDEX      1

typedef struct {
    A_UINT8     badApIndex;         /* 0 to WMI_MAX_BAD_AP_INDEX */
    A_UINT8     bssid[ATH_MAC_LEN];
} WMI_ADD_BAD_AP_CMD;

/*
 * WMI_DELETE_BAD_AP_CMDID
 */
typedef struct {
    A_UINT8     badApIndex;         /* 0 to WMI_MAX_BAD_AP_INDEX */
} WMI_DELETE_BAD_AP_CMD;

/*
 * WMI_SET_ACCESS_PARAMS_CMDID
 */
#define WMI_DEFAULT_TXOP_ACPARAM    0       /* implies one MSDU */
#define WMI_DEFAULT_ECWMIN_ACPARAM  4       /* corresponds to CWmin of 15 */
#define WMI_DEFAULT_ECWMAX_ACPARAM  10      /* corresponds to CWmax of 1023 */
#define WMI_MAX_CW_ACPARAM          15      /* maximum eCWmin or eCWmax */
#define WMI_DEFAULT_AIFSN_ACPARAM   2
#define WMI_MAX_AIFSN_ACPARAM       15
typedef struct {
    A_UINT16 txop;                      /* in units of 32 usec */
    A_UINT8  eCWmin;
    A_UINT8  eCWmax;
    A_UINT8  aifsn;
} WMI_SET_ACCESS_PARAMS_CMD;


/*
 * WMI_SET_RETRY_LIMITS_CMDID
 *
 * This command is used to customize the number of retries the
 * wlan device will perform on a given frame.
 */
#define WMI_MIN_RETRIES 2
#define WMI_MAX_RETRIES 13
typedef enum {
    MGMT_FRAMETYPE    = 0,
    CONTROL_FRAMETYPE = 1,
    DATA_FRAMETYPE    = 2
} WMI_FRAMETYPE;

typedef struct {
    A_UINT8 frameType;                      /* WMI_FRAMETYPE */
    A_UINT8 trafficClass;                   /* applies only to DATA_FRAMETYPE */
    A_UINT8 maxRetries;
} WMI_SET_RETRY_LIMITS_CMD;

/*
 * WMI_SET_ROAM_CTRL_CMDID
 *
 * This command is used to influence the Roaming behaviour
 * Set the host biases of the BSSs before setting the roam mode as bias
 * based.
 */

/*
 * Different types of Roam Control
 */

typedef enum {
        WMI_FORCE_ROAM          = 1,      /* Roam to the specified BSSID */
        WMI_SET_ROAM_MODE       = 2,      /* default ,progd bias, no roam */
        WMI_SET_HOST_BIAS       = 3,     /* Set the Host Bias */
} WMI_ROAM_CTRL_TYPE;

#define WMI_MIN_ROAM_CTRL_TYPE WMI_FORCE_ROAM
#define WMI_MAX_ROAM_CTRL_TYPE WMI_SET_HOST_BIAS

/*
 * ROAM MODES
 */

typedef enum {
        WMI_DEFAULT_ROAM_MODE   = 1,  /* RSSI based ROAM */
        WMI_HOST_BIAS_ROAM_MODE = 2, /* HOST BIAS based ROAM */
        WMI_LOCK_BSS_MODE  = 3,  /* Lock to the Current BSS - no Roam */
        WMI_DISABLE_ROAM = 4    /* disables all auto-nomous roaming
                                   behaviour */
} WMI_ROAM_MODE;

/*
 * BSS HOST BIAS INFO
 */

typedef struct {
        A_UINT8 bssid[ATH_MAC_LEN];
        A_INT8  bias; 
} WMI_BSS_BIAS;

typedef struct {
        A_UINT8 numBss;
        WMI_BSS_BIAS bssBias[1];
} WMI_BSS_BIAS_INFO;

typedef struct {
    A_UINT8   roamCtrlType;
    union {
        A_UINT8 bssid[ATH_MAC_LEN]; /* WMI_FORCE_ROAM */
        A_UINT8 roamMode;           /* WMI_SET_ROAM_MODE  */
        WMI_BSS_BIAS_INFO bssBiasInfo; /* WMI_SET_HOST_BIAS */
    } info;
} WMI_SET_ROAM_CTRL_CMD;

/*
 * WMI_ENABLE_RM_CMDID
 */
typedef struct {
        A_BOOL enable_radio_measurements;
} WMI_ENABLE_RM_CMD;

/*
 * WMI_SET_MAX_OFFHOME_DURATION_CMDID
 */
typedef struct {
        A_UINT8 max_offhome_duration;
} WMI_SET_MAX_OFFHOME_DURATION_CMD;



/*
 * Command Replies
 */

/*
 * WMI_GET_CHANNEL_LIST_CMDID reply
 */
typedef struct {
    A_UINT8     reserved1;
    A_UINT8     numChannels;            /* number of channels in reply */
    A_UINT16    channelList[1];         /* channel in Mhz */
} WMI_CHANNEL_LIST_REPLY;

typedef enum {
    A_SUCCEEDED = A_OK,
    A_MBOX1_OK = 1,
    A_MBOX2_OK = 2,
    A_MBOX3_OK = 3,
    A_FAILED_DELETE_INVALID_MBOX = 253,
    A_FAILED_CREATE_REMOVE_PSTREAM_FIRST = 254,
} PSTREAM_REPLY_STATUS;

typedef struct {
    A_UINT8     status;                 /* PSTREAM_REPLY_STATUS */
    A_UINT8     txQueueNumber;
    A_UINT8     rxQueueNumber;
    A_UINT8     trafficClass;
    A_UINT8     trafficDirection;       /* DIR_TYPE */
} WMI_CRE_PRIORITY_STREAM_REPLY;

typedef struct {
    A_UINT8     status;                 /* PSTREAM_REPLY_STATUS */
    A_UINT8     txQueueNumber;
    A_UINT8     rxQueueNumber;
    A_UINT8     trafficDirection;       /* DIR_TYPE */
    A_UINT8     trafficClass;
} WMI_DEL_PRIORITY_STREAM_REPLY;


/*
 * List of Events (target to host)
 */
typedef enum {
    WMI_READY_EVENTID           = 0x1001,
    WMI_CONNECT_EVENTID,
    WMI_DISCONNECT_EVENTID,
    WMI_BSSINFO_EVENTID,
    WMI_CMDERROR_EVENTID,
    WMI_REGDOMAIN_EVENTID,
    WMI_PSTREAM_TIMEOUT_EVENTID,
    WMI_NEIGHBOR_REPORT_EVENTID,
    WMI_TKIP_MICERR_EVENTID,
    WMI_SCAN_COMPLETE_EVENTID,
    WMI_REPORT_STATISTICS_EVENTID,
    WMI_RSSI_THRESHOLD_EVENTID,
    WMI_ERROR_REPORT_EVENTID,
    WMI_OPT_RX_FRAME_EVENTID,
#ifndef AR6K_FIRMWARE_1_0
    WMI_REPORT_ROAM_TBL_EVENTID,
#endif
    WMI_EXTENSION_EVENTID,
#ifndef AR6K_FIRMWARE_1_0
    WMI_CAC_EVENTID,
    WMI_SNR_THRESHOLD_EVENTID,
    WMI_LQ_THRESHOLD_EVENTID,
    WMI_TX_RETRY_ERR_EVENTID,
    WMI_REPORT_ROAM_DATA_EVENTID,
#endif
} WMI_EVENT_ID;

typedef enum {
    WMI_11A_CAPABILITY   = 1,
    WMI_11G_CAPABILITY   = 2,
    WMI_11AG_CAPABILITY  = 3,
} WMI_PHY_CAPABILITY;

typedef struct {
    A_UINT8     macaddr[ATH_MAC_LEN];
    A_UINT8     phyCapability;              /* WMI_PHY_CAPABILITY */
} WMI_READY_EVENT;

/*
 * Connect Event
 */
typedef struct {
    A_UINT16    channel;
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT16    listenInterval;
#ifndef AR6K_FIRMWARE_1_0
    A_UINT8     beaconIeLen;
#endif
    A_UINT8     assocReqLen;
    A_UINT8     assocRespLen;
    A_UINT8     assocInfo[1];
} WMI_CONNECT_EVENT;

/*
 * Disconnect Event
 */
typedef enum {
    NO_NETWORK_AVAIL   = 0x01,
    LOST_LINK          = 0x02,     /* bmiss */
    DISCONNECT_CMD     = 0x03,
    BSS_DISCONNECTED   = 0x04,
    AUTH_FAILED        = 0x05,
    ASSOC_FAILED       = 0x06,
    NO_RESOURCES_AVAIL = 0x07,
    CSERV_DISCONNECT   = 0x08,
    INVALID_PROFILE    = 0x0a,
} WMI_DISCONNECT_REASON;

typedef struct {
    A_UINT8     disconnectReason;      /* see WMI_DISCONNECT_REASON */
    A_UINT8     bssid[ATH_MAC_LEN];    /* set if known */
    A_UINT8     assocRespLen;
    A_UINT8     assocInfo[1];
} WMI_DISCONNECT_EVENT;

/*
 * BSS Info Event.
 * Mechanism used to inform host of the presence and characteristic of
 * wireless networks present.  Consists of bss info header followed by
 * the beacon or probe-response frame body.  The 802.11 header is not included.
 */
typedef enum {
    BEACON_FTYPE = 0x1,
    PROBERESP_FTYPE,
} WMI_BI_FTYPE;

typedef struct {
    A_UINT16    channel;
    A_UINT8     frameType;          /* see WMI_BI_FTYPE */
    A_INT8      rssi;
    A_UINT8     bssid[ATH_MAC_LEN];
} WMI_BSS_INFO_HDR;

/*
 * Command Error Event
 */
typedef enum {
    INVALID_PARAM  = 0x01,
    ILLEGAL_STATE  = 0x02,
    INTERNAL_ERROR = 0x03,
} WMI_ERROR_CODE;

typedef struct {
    A_UINT16    commandId;
    A_UINT8     errorCode;
} WMI_CMD_ERROR_EVENT;

/*
 * New Regulatory Domain Event
 */
typedef struct {
    A_UINT32    regDomain;
} WMI_REG_DOMAIN_EVENT;

typedef struct {
    A_UINT8     txQueueNumber;
    A_UINT8     rxQueueNumber;
    A_UINT8     trafficDirection;
} WMI_PSTREAM_TIMEOUT_EVENT;

/*
 * The WMI_NEIGHBOR_REPORT Event is generated by the target to inform
 * the host of BSS's it has found that matches the current profile.
 * It can be used by the host to cache PMKs and/to initiate pre-authentication
 * if the BSS supports it.  The first bssid is always the current associated
 * BSS.
 * The bssid and bssFlags information repeats according to the number
 * or APs reported.
 */
typedef enum {
    WMI_DEFAULT_BSS_FLAGS   = 0x00,
    WMI_PREAUTH_CAPABLE_BSS = 0x01,
    WMI_PMKID_VALID_BSS     = 0x02,
} WMI_BSS_FLAGS;

typedef struct {
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT8     bssFlags;            /* see WMI_BSS_FLAGS */
} WMI_NEIGHBOR_INFO;

typedef struct {
    A_INT8      numberOfAps;
    WMI_NEIGHBOR_INFO neighbor[1];
} WMI_NEIGHBOR_REPORT_EVENT;

/*
 * TKIP MIC Error Event
 */
typedef struct {
    A_UINT8 keyid;
    A_UINT8 ismcast;
} WMI_TKIP_MICERR_EVENT;

/*
 * WMI_SCAN_COMPLETE_EVENTID - no parameters
 */
#ifdef AR6K_FIRMWARE_1_0
#define MAX_OPT_DATA_LEN 1500
#else
#define MAX_OPT_DATA_LEN 1400
#endif

/*
 * WMI_SET_ADHOC_BSSID_CMDID
 */
typedef struct {
    A_UINT8     bssid[ATH_MAC_LEN];
} WMI_SET_ADHOC_BSSID_CMD;

/*
 * WMI_SET_OPT_MODE_CMDID
 */
typedef enum {
    SPECIAL_OFF,
    SPECIAL_ON,
} OPT_MODE_TYPE;

typedef struct {
    A_UINT8     optMode;
} WMI_SET_OPT_MODE_CMD;

/*
 * WMI_TX_OPT_FRAME_CMDID
 */
typedef enum {
    OPT_PROBE_REQ   = 0x01,
    OPT_PROBE_RESP  = 0x02,
    OPT_CPPP_START  = 0x03,
    OPT_CPPP_STOP   = 0x04,
} WMI_OPT_FTYPE;

typedef struct {
    A_UINT8     frmType;
    A_UINT8     dstAddr[ATH_MAC_LEN];
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT16    optIEDataLen;
    A_UINT8     *optIEData;
} WMI_OPT_TX_FRAME_CMD;

/*
 * Special frame receive Event.
 * Mechanism used to inform host of the receiption of the special frames.
 * Consists of special frame info header followed by special frame body.
 * The 802.11 header is not included.
 */
typedef struct {
    A_UINT16    channel;
    A_UINT8     frameType;          /* see WMI_OPT_FTYPE */
    A_INT8      rssi;
    A_UINT8     srcAddr[ATH_MAC_LEN];
    A_UINT8     bssid[ATH_MAC_LEN];
} WMI_OPT_RX_INFO_HDR;

/*
 * Reporting statistics.
 */
typedef struct {
    A_UINT32   tx_packets;           
    A_UINT32   tx_bytes;             
    A_UINT32   tx_unicast_pkts;      
    A_UINT32   tx_unicast_bytes;     
    A_UINT32   tx_multicast_pkts;    
    A_UINT32   tx_multicast_bytes;   
    A_UINT32   tx_broadcast_pkts;    
    A_UINT32   tx_broadcast_bytes;   
    A_UINT32   tx_rts_success_cnt;
#ifndef AR6K_FIRMWARE_1_0
    A_UINT32   tx_packet_per_ac[4];
#endif
    A_UINT32   tx_errors;            
    A_UINT32   tx_failed_cnt;
    A_UINT32   tx_retry_cnt;
    A_UINT32   tx_rts_fail_cnt;
}tx_stats_t;

typedef struct {
    A_UINT32   rx_packets;           
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
}rx_stats_t;

typedef struct {
    A_UINT32   tkip_local_mic_failure;
    A_UINT32   tkip_counter_measures_invoked;
    A_UINT32   tkip_replays;
    A_UINT32   tkip_format_errors; 
    A_UINT32   ccmp_format_errors;
    A_UINT32   ccmp_replays;
}tkip_ccmp_stats_t;

typedef struct {
    A_UINT32   power_save_failure_cnt;
}pm_stats_t;

typedef struct {
    A_UINT32    cs_bmiss_cnt;
    A_UINT32    cs_lowRssi_cnt;
    A_UINT16    cs_connect_cnt;
    A_UINT16    cs_disconnect_cnt;
    A_UINT8     cs_aveBeacon_rssi;
    A_UINT8     cs_lastRoam_msec;
}cserv_stats_t;

typedef struct {
    tx_stats_t          tx_stats;
    rx_stats_t          rx_stats;
    tkip_ccmp_stats_t   tkipCcmpStats;
}wlan_net_stats_t;

typedef struct {
    wlan_net_stats_t    txrxStats;
    cserv_stats_t       cservStats;
    pm_stats_t          pmStats;
    A_INT16             noise_floor_calibation;
}WMI_TARGET_STATS;

/*
 * WMI_RSSI_THRESHOLD_EVENTID.
 * Indicate the RSSI events to host. Events are indicated when we breach a 
 * thresold value.
 */
typedef enum{
    WMI_RSSI_LOWTHRESHOLD_BELOW_LOWERVAL=1,
    WMI_RSSI_LOWTHRESHOLD_LOWERVAL,
    WMI_RSSI_LOWTHRESHOLD_UPPERVAL,
    WMI_RSSI_HIGHTHRESHOLD_LOWERVAL,
    WMI_RSSI_HIGHTHRESHOLD_HIGHERVAL
}WMI_RSSI_THRESHOLD_VAL;

typedef struct {
    A_UINT8 range;
}WMI_RSSI_THRESHOLD_EVENT;

/*
 *  WMI_ERROR_REPORT_EVENTID
 */
typedef enum{
    WMI_TARGET_PM_ERR_FAIL      = 0x00000001,
    WMI_TARGET_KEY_NOT_FOUND    = 0x00000002,
    WMI_TARGET_DECRYPTION_ERR   = 0x00000004,
    WMI_TARGET_BMISS            = 0x00000008,
    WMI_PSDISABLE_NODE_JOIN     = 0x00000010
}WMI_TARGET_ERROR_VAL;

typedef struct {
    A_UINT32 errorVal;
}WMI_TARGET_ERROR_REPORT_EVENT;

/*
 * WMI_REPORT_ROAM_TBL_EVENTID
 */
#define MAX_ROAM_TBL_CAND   5

typedef struct {
    A_UINT8 bssid[ATH_MAC_LEN];
    A_INT8  rssi;
    A_INT8  rssidt;
    A_INT8  last_rssi;
    A_INT32 roam_util;
    A_INT8  util;
    A_INT8  bias;
} WMI_BSS_ROAM_INFO;


typedef struct {
    A_UINT8  roamMode;
    A_UINT8  numEntries;
    WMI_BSS_ROAM_INFO bssRoamInfo[1];
} WMI_TARGET_ROAM_TBL;

/*
 *  WMI_CAC_EVENTID
 */
typedef enum {
    CAC_INDICATION_ADMISSION = 0x00,
    CAC_INDICATION_ADMISSION_RESP = 0x01,
    CAC_INDICATION_DELETE = 0x02,
    CAC_INDICATION_NO_RESP = 0x03,
}CAC_INDICATION;

#define WMM_TSPEC_IE_LEN   63

typedef struct {
    A_UINT8 ac;
    A_UINT8 cac_indication;
    A_UINT8 statusCode;
    A_UINT8 tspecSuggestion[WMM_TSPEC_IE_LEN];
}WMI_CAC_EVENT;

/*
 * developer commands
 */

/*
 * WMI_SET_BITRATE_CMDID
 *
 * Get bit rate cmd uses same definition as set bit rate cmd
 */
typedef enum {
    RATE_AUTO   = -1,
    RATE_1Mb    = 0,
    RATE_2Mb    = 1,
    RATE_5_5Mb  = 2,
    RATE_11Mb   = 3,
    RATE_6Mb    = 4,
    RATE_9Mb    = 5,
    RATE_12Mb   = 6,
    RATE_18Mb   = 7,
    RATE_24Mb   = 8,
    RATE_36Mb   = 9,
    RATE_48Mb   = 10,
    RATE_54Mb   = 11,
} WMI_BIT_RATE;

typedef struct {
    A_INT8      rateIndex;          /* see WMI_BIT_RATE */
} WMI_BIT_RATE_CMD, WMI_BIT_RATE_REPLY;

typedef enum {
    ROAM_DATA_TIME = 1,            /* Get The Roam Time Data */
} ROAM_DATA_TYPE;

typedef struct {
    A_UINT32        disassoc_time;
    A_UINT8         disassoc_bssid[ATH_MAC_LEN];
    A_INT8          disassoc_bss_rssi;
    A_UINT32        no_txrx_time;
    A_UINT32        assoc_time;
    A_UINT8         assoc_bssid[ATH_MAC_LEN];
    A_INT8          assoc_bss_rssi;
    A_UINT32        allow_txrx_time;
    A_UINT32        last_data_txrx_time;
    A_UINT32        first_data_txrx_time;
} WMI_TARGET_ROAM_TIME;

typedef struct {
    A_UINT8 roamDataType;
    union {
        WMI_TARGET_ROAM_TIME roamTime;
    } u;
} WMI_TARGET_ROAM_DATA;

#ifdef UNDER_CE
#pragma pack()
#endif

#ifdef __cplusplus
}
#endif

#endif /* _WMI_H_ */
