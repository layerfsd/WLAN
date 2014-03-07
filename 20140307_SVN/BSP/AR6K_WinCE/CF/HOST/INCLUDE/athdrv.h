//------------------------------------------------------------------------------
// <copyright file="athdrv.h" company="Atheros">
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
//This file contains the definitions for the AR6000 driver.
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _ATHDRV_H_
#define _ATHDRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
/*
 * There are two types of ioctl's here: Standard ioctls and
 * eXtended ioctls.  All extended ioctls (XIOCTL) are multiplexed
 * off of the single ioctl command, AR6000_IOCTL_EXTENDED.  The
 * arguments for every XIOCTL starts with a 32-bit command word
 * that is used to select which extended ioctl is in use.  After
 * the command word are command-specific arguments.
 */

/* Linux standard Wireless Extensions, private ioctl interfaces */
#define IEEE80211_IOCTL_SETPARAM             (SIOCIWFIRSTPRIV+0)
#define IEEE80211_IOCTL_GETPARAM             (SIOCIWFIRSTPRIV+1)
#define IEEE80211_IOCTL_SETKEY               (SIOCIWFIRSTPRIV+2)
#define IEEE80211_IOCTL_SETWMMPARAMS         (SIOCIWFIRSTPRIV+3)
#define IEEE80211_IOCTL_DELKEY               (SIOCIWFIRSTPRIV+4)
#define IEEE80211_IOCTL_GETWMMPARAMS         (SIOCIWFIRSTPRIV+5)
#define IEEE80211_IOCTL_SETMLME              (SIOCIWFIRSTPRIV+6)
#define IEEE80211_IOCTL_SETOPTIE             (SIOCIWFIRSTPRIV+6)
#define IEEE80211_IOCTL_GETOPTIE             (SIOCIWFIRSTPRIV+7)
#define IEEE80211_IOCTL_ADDPMKID             (SIOCIWFIRSTPRIV+8)
//#define IEEE80211_IOCTL_SETAUTHALG           (SIOCIWFIRSTPRIV+10)
#define IEEE80211_IOCTL_LASTONE              (SIOCIWFIRSTPRIV+9)



/*                      ====WMI Ioctls====                                    */
/*
 * 
 * Many ioctls simply provide WMI services to application code:
 * an application makes such an ioctl call with a set of arguments
 * that are packaged into the corresponding WMI message, and sent
 * to the Target.
 */

#define AR6000_IOCTL_WMI_GETREV              (SIOCIWFIRSTPRIV+10)
/*
 * arguments:
 *   ar6000_version *revision
 */

#define AR6000_IOCTL_WMI_SETPWR              (SIOCIWFIRSTPRIV+11)
/*
 * arguments:
 *   WMI_POWER_MODE_CMD pwrModeCmd (see include/wmi.h)
 * uses: WMI_SET_POWER_MODE_CMDID
 */

#define AR6000_IOCTL_WMI_SETSCAN             (SIOCIWFIRSTPRIV+12)
/*
 * arguments:
 *   WMI_SCAN_PARAMS_CMD scanParams (see include/wmi.h)
 * uses: WMI_SET_SCAN_PARAMS_CMDID
 */

#define AR6000_IOCTL_WMI_SETLISTENINT        (SIOCIWFIRSTPRIV+13)
/*
 * arguments:
 *   UINT32 listenInterval
 * uses: WMI_SET_LISTEN_INT_CMDID
 */

#define AR6000_IOCTL_WMI_SETBSSFILTER        (SIOCIWFIRSTPRIV+14)
/*
 * arguments:
 *   WMI_BSS_FILTER filter (see include/wmi.h)
 * uses: WMI_SET_BSS_FILTER_CMDID
 */

#define AR6000_IOCTL_WMI_SET_CHANNELPARAMS   (SIOCIWFIRSTPRIV+16)
/*
 * arguments:
 *   WMI_CHANNEL_PARAMS_CMD chParams
 * uses: WMI_SET_CHANNEL_PARAMS_CMDID
 */

#define AR6000_IOCTL_WMI_SET_PROBEDSSID      (SIOCIWFIRSTPRIV+17)
/*
 * arguments:
 *   WMI_PROBED_SSID_CMD probedSsids (see include/wmi.h)
 * uses: WMI_SETPROBED_SSID_CMDID
 */

#define AR6000_IOCTL_WMI_SET_PMPARAMS        (SIOCIWFIRSTPRIV+18)
/*
 * arguments:
 *   WMI_POWER_PARAMS_CMD powerParams (see include/wmi.h)
 * uses: WMI_SET_POWER_PARAMS_CMDID
 */

#define AR6000_IOCTL_WMI_SET_BADAP           (SIOCIWFIRSTPRIV+19)
/*
 * arguments:
 *   WMI_ADD_BAD_AP_CMD badAPs (see include/wmi.h)
 * uses: WMI_ADD_BAD_AP_CMDID
 */

#define AR6000_IOCTL_WMI_GET_QOS_QUEUE       (SIOCIWFIRSTPRIV+20)
/*
 * arguments:
 *   ar6000_queuereq queueRequest (see below)
 */

#define AR6000_IOCTL_WMI_CREATE_QOS          (SIOCIWFIRSTPRIV+21)
/*
 * arguments:
 *   WMI_CREATE_PSTREAM createPstreamCmd (see include/wmi.h)
 * uses: WMI_CREATE_PSTREAM_CMDID
 */

#define AR6000_IOCTL_WMI_DELETE_QOS          (SIOCIWFIRSTPRIV+22)
/*
 * arguments:
 *   WMI_DELETE_PSTREAM_CMD deletePstreamCmd (see include/wmi.h)
 * uses: WMI_DELETE_PSTREAM_CMDID
 */

#define AR6000_IOCTL_WMI_SET_LINKTHRESHOLD   (SIOCIWFIRSTPRIV+23)
/*
 * arguments:
 *   WMI_RSSI_THRESHOLD_PARAMS_CMD thresholdParams (see include/wmi.h)
 * uses: WMI_RSSI_THRESHOLD_PARAMS_CMDID
 */

#define AR6000_IOCTL_WMI_SET_ERROR_REPORT_BITMASK (SIOCIWFIRSTPRIV+24)
/*
 * arguments:
 *   WMI_TARGET_ERROR_REPORT_BITMASK errorReportBitMask (see include/wmi.h)
 * uses: WMI_TARGET_ERROR_REPORT_BITMASK_CMDID
 */

#define AR6000_IOCTL_WMI_GET_TARGET_STATS    (SIOCIWFIRSTPRIV+25)
/*
 * arguments:
 *   TARGET_STATS *targetStats (see below)
 * uses: WMI_GET_STATISTICS_CMDID
 */

#define AR6000_IOCTL_WMI_SET_ASSOC_INFO      (SIOCIWFIRSTPRIV+26)
/*
 * arguments:
 *   WMI_SET_ASSOC_INFO_CMD setAssocInfoCmd
 * uses: WMI_SET_ASSOC_INFO_CMDID
 */

#define AR6000_IOCTL_WMI_SET_ACCESS_PARAMS   (SIOCIWFIRSTPRIV+27)
/*
 * arguments:
 *   WMI_SET_ACCESS_PARAMS_CMD setAccessParams (see include/wmi.h)
 * uses: WMI_SET_ACCESS_PARAMS_CMDID
 */

#define AR6000_IOCTL_WMI_SET_BMISS_TIME      (SIOCIWFIRSTPRIV+28)
/*
 * arguments:
 *   UINT32 beaconMissTime
 * uses: WMI_SET_BMISS_TIME_CMDID
 */

#define AR6000_IOCTL_WMI_SET_DISC_TIMEOUT    (SIOCIWFIRSTPRIV+29)
/*
 * arguments:
 *   WMI_DISC_TIMEOUT_CMD disconnectTimeoutCmd (see include/wmi.h)
 * uses: WMI_SET_DISC_TIMEOUT_CMDID
 */

#define AR6000_IOCTL_WMI_SET_IBSS_PM_CAPS    (SIOCIWFIRSTPRIV+30)
/*
 * arguments:
 *   WMI_IBSS_PM_CAPS_CMD ibssPowerMgmtCapsCmd
 * uses: WMI_SET_IBSS_PM_CAPS_CMDID
 */

/*
 * There is a very small space available for driver-private
 * wireless ioctls.  In order to circumvent this limitation,
 * we multiplex a bunch of ioctls (XIOCTLs) on top of a 
 * single AR6000_IOCTL_EXTENDED ioctl.
 */
#define AR6000_IOCTL_EXTENDED                (SIOCIWFIRSTPRIV+31)


/*                         ====BMI Extended Ioctls====                        */

#define AR6000_XIOCTL_BMI_DONE                 1
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_BMI_DONE)
 * uses: BMI_DONE
 */

#define AR6000_XIOCTL_BMI_READ_MEMORY          2
/*
 * arguments:
 *   union {
 *     struct {
 *       UINT32 cmd (AR6000_XIOCTL_BMI_READ_MEMORY)
 *       UINT32 address
 *       UINT32 length
 *     }
 *     char results[length]
 *   }
 * uses: BMI_READ_MEMORY
 */

#define AR6000_XIOCTL_BMI_WRITE_MEMORY         3
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_BMI_WRITE_MEMORY)
 *   UINT32 address
 *   UINT32 length
 *   char data[length]
 * uses: BMI_WRITE_MEMORY
 */

#define AR6000_XIOCTL_BMI_EXECUTE              4
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_BMI_EXECUTE)
 *   UINT32 TargetAddress
 *   UINT32 parameter
 * uses: BMI_EXECUTE
 */

#define AR6000_XIOCTL_BMI_SET_APP_START        5
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_BMI_SET_APP_START)
 *   UINT32 TargetAddress
 * uses: BMI_SET_APP_START
 */

#define AR6000_XIOCTL_BMI_READ_SOC_REGISTER    6
/*
 * arguments:
 *   union {
 *     struct {
 *       UINT32 cmd (AR6000_XIOCTL_BMI_READ_SOC_REGISTER)
 *       UINT32 TargetAddress, 32-bit aligned
 *     }
 *     UINT32 result
 *   }
 * uses: BMI_READ_SOC_REGISTER
 */

#define AR6000_XIOCTL_BMI_WRITE_SOC_REGISTER   7
/*
 * arguments:
 *     struct {
 *       UINT32 cmd (AR6000_XIOCTL_BMI_WRITE_SOC_REGISTER)
 *       UINT32 TargetAddress, 32-bit aligned
 *       UINT32 newValue
 *     }
 * uses: BMI_WRITE_SOC_REGISTER
 */

#define AR6000_XIOCTL_BMI_TEST                 8
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_BMI_TEST)
 *   UINT32 address
 *   UINT32 length
 *   UINT32 count
 */



/*                      ====DataSet Extended Ioctls====                       */

#define AR6000_XIOCTL_WMI_DSET_WAIT_REQ        9
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_WMI_DSET_WAIT_REQ)
 */

#define AR6000_XIOCTL_WMI_DSET_OPEN_REPLY     10
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_WMI_DSET_OPEN_REPLY)
 *   dset_open_reply_t (see host/include/app/dset.h)
 * uses: WMIX_DSETOPEN_REPLY_CMDID
 */

#define AR6000_XIOCTL_WMI_DSET_DATA_REPLY     11
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_WMI_DSET_DATA_REPLY)
 *   dset_data_reply_t (see host/include/app/dset.h)
 * uses: WMIX_DSETDATA_REPLY_CMDID
 */

#define AR6000_XIOCTL_FORCE_TARGET_RESET      12
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_FORCE_TARGET_RESET)
 */


#ifdef HTC_RAW_INTERFACE
/* HTC Raw Interface Ioctls */
#define AR6000_XIOCTL_HTC_RAW_OPEN            13
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_HTC_RAW_OPEN)
 */

#define AR6000_XIOCTL_HTC_RAW_CLOSE           14
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_HTC_RAW_CLOSE)
 */

#define AR6000_XIOCTL_HTC_RAW_READ            15
/*
 * arguments:
 *   union {
 *     struct {
 *       UINT32 cmd (AR6000_XIOCTL_HTC_RAW_READ)
 *       UINT32 mailboxID
 *       UINT32 length
 *     }
 *     results[length]
 *   } 
 */

#define AR6000_XIOCTL_HTC_RAW_WRITE           16
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_HTC_RAW_WRITE)
 *   UINT32 mailboxID
 *   UINT32 length
 *   char buffer[length]
 */
#endif /* HTC_RAW_INTERFACE */

#define AR6000_XIOCTL_CHECK_TARGET_READY      17
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_CHECK_TARGET_READY)
 */



/*                ====GPIO (General Purpose I/O) Extended Ioctls====          */

#define AR6000_XIOCTL_GPIO_OUTPUT_SET         18
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_GPIO_OUTPUT_SET)
 *   ar6000_gpio_output_set_cmd_s (see below)
 * uses: WMIX_GPIO_OUTPUT_SET_CMDID
 */

#define AR6000_XIOCTL_GPIO_INPUT_GET          19
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_GPIO_INPUT_GET)
 * uses: WMIX_GPIO_INPUT_GET_CMDID
 */

#define AR6000_XIOCTL_GPIO_REGISTER_SET       20
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_GPIO_REGISTER_SET)
 *   ar6000_gpio_register_cmd_s (see below)
 * uses: WMIX_GPIO_REGISTER_SET_CMDID
 */

#define AR6000_XIOCTL_GPIO_REGISTER_GET       21
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_GPIO_REGISTER_GET)
 *   ar6000_gpio_register_cmd_s (see below)
 * uses: WMIX_GPIO_REGISTER_GET_CMDID
 */

#define AR6000_XIOCTL_GPIO_INTR_ACK           22
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_GPIO_INTR_ACK)
 *   ar6000_cpio_intr_ack_cmd_s (see below)
 * uses: WMIX_GPIO_INTR_ACK_CMDID
 */

#define AR6000_XIOCTL_GPIO_INTR_WAIT          23
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_GPIO_INTR_WAIT)
 */



/*                    ====more wireless commands====                          */

#define AR6000_XIOCTL_SET_ADHOC_BSSID         24
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_SET_ADHOC_BSSID)
 *   WMI_SET_ADHOC_BSSID_CMD setAdHocBssidCmd (see include/wmi.h)
 */

#define AR6000_XIOCTL_SET_OPT_MODE            25
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_SET_OPT_MODE)
 *   WMI_SET_OPT_MODE_CMD setOptModeCmd (see include/wmi.h)
 * uses: WMI_SET_OPT_MODE_CMDID
 */

#define AR6000_XIOCTL_OPT_SEND_FRAME          26
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_OPT_SEND_FRAME)
 *   WMI_OPT_TX_FRAME_CMD optTxFrameCmd (see include/wmi.h)
 * uses: WMI_OPT_TX_FRAME_CMDID
 */

#define AR6000_XIOCTL_SET_ADHOC_BEACON_INTVAL 27
/*
 * arguments:
 *   UINT32 cmd (AR6000_XIOCTL_SET_ADHOC_BEACON_INTVAL)
 *   WMI_BEACON_INT_CMD beaconIntCmd (see include/wmi.h)
 * uses: WMI_SET_BEACON_INT_CMDID
 */


#define IEEE80211_IOCTL_SETAUTHALG           28





/* used by AR6000_IOCTL_WMI_GETREV */
struct ar6000_version {
    A_UINT32        host_ver;
    A_UINT32        target_ver;
};

/* used by AR6000_IOCTL_WMI_GET_QOS_QUEUE */
struct ar6000_queuereq{
    A_UINT8         trafficDirection;
    A_UINT8         trafficClass;
    A_INT8          queueNumber;
};

/* used by AR6000_IOCTL_WMI_GET_TARGET_STATS */
typedef struct targetStats_t {
    A_UINT64    tx_packets;           
    A_UINT64    tx_bytes;             
    A_UINT64    tx_unicast_pkts;      
    A_UINT64    tx_unicast_bytes;     
    A_UINT64    tx_multicast_pkts;    
    A_UINT64    tx_multicast_bytes;   
    A_UINT64    tx_broadcast_pkts;    
    A_UINT64    tx_broadcast_bytes;   
    A_UINT64    tx_rts_success_cnt;
 
    A_UINT64    tx_errors;            
    A_UINT64    tx_failed_cnt;
    A_UINT64    tx_retry_cnt;
    A_UINT64    tx_rts_fail_cnt;
    A_UINT64    rx_packets;           
    A_UINT64    rx_bytes;             
    A_UINT64    rx_unicast_pkts;      
    A_UINT64    rx_unicast_bytes;     
    A_UINT64    rx_multicast_pkts;    
    A_UINT64    rx_multicast_bytes;   
    A_UINT64    rx_broadcast_pkts;    
    A_UINT64    rx_broadcast_bytes;   
    A_UINT64    rx_fragment_pkt;
 
    A_UINT64    rx_errors;            
    A_UINT64    rx_crcerr;            
    A_UINT64    rx_key_cache_miss;    
    A_UINT64    rx_decrypt_err;       
    A_UINT64    rx_duplicate_frames;

    A_UINT64    tkip_local_mic_failure;
    A_UINT64    tkip_counter_measures_invoked;
    A_UINT64    tkip_replays;
    A_UINT64    tkip_format_errors; 
    A_UINT64    ccmp_format_errors;
    A_UINT64    ccmp_replays;

    A_UINT64    power_save_failure_cnt;
    A_INT16     noise_floor_calibation;

    A_UINT64    cs_bmiss_cnt;
    A_UINT64    cs_lowRssi_cnt;
    A_UINT64    cs_connect_cnt;
    A_UINT64    cs_disconnect_cnt;
    A_UINT8     cs_aveBeacon_rssi;
    A_UINT8     cs_lastRoam_msec;
}TARGET_STATS;

/* used by AR6000_XIOCTL_GPIO_OUTPUT_SET */
struct ar6000_gpio_output_set_cmd_s {
    A_UINT32 set_mask;
    A_UINT32 clear_mask;
    A_UINT32 enable_mask;
    A_UINT32 disable_mask;
};

/*
 * used by AR6000_XIOCTL_GPIO_REGISTER_GET and AR6000_XIOCTL_GPIO_REGISTER_SET
 */
struct ar6000_gpio_register_cmd_s {
    A_UINT32 gpioreg_id;
    A_UINT32 value;
};

/* used by AR6000_XIOCTL_GPIO_INTR_ACK */
struct ar6000_gpio_intr_ack_cmd_s {
    A_UINT32 ack_mask;
};

/* used by AR6000_XIOCTL_GPIO_INTR_WAIT */
struct ar6000_gpio_intr_wait_cmd_s {
    A_UINT32 intr_mask;
    A_UINT32 input_values;
};

#endif //__linux__

#ifdef UNDER_CE
#include <ndis.h>
/*                         ====WinCE OIDS for BMI====                        */
#define OID_CAR6K_BMI_DONE 0xFF000001
#define OID_CAR6K_BMI_WRITE_MEMORY 0xFF000002
#define OID_CAR6K_BMI_TEST 0xFF000003
#define OID_CAR6K_BMI_EXECUTE 0xFF000004
#define OID_CAR6K_BMI_SET_APP_START 0xFF000005
#define OID_CAR6K_BMI_WRITE_SOC_REGISTER 0xFF000006
#define OID_CAR6K_BMI_READ_MEMORY 0xFF000007
#define OID_CAR6K_BMI_READ_SOC_REGISTER 0xFF000008

/*                         ====WinCE OIDS for HTC RAW I/F====                */
#ifdef HTC_RAW_INTERFACE
#define OID_CAR6K_HTC_RAW_OPEN 0xFF000009
#define OID_CAR6K_HTC_RAW_CLOSE 0xFF000010
#define OID_CAR6K_HTC_RAW_READ 0xFF000011
#define OID_CAR6K_HTC_RAW_WRITE 0xFF000012
#endif //HTC_RAW_INTERFACE

#ifdef TCMD
/*       ====extended commands for radio test ====                          */

#define OID_CAR6K_TCMD_CONT_TX	0xFF000013

/*
 * arguments:
 *   WMI_TCMD_CONT_TX_CMD contTxCmd (see include/wmi.h)
 * uses: WMI_TCMD_CONT_TX_CMDID
 */

#define OID_CAR6K_TCMD_CONT_RX	0xFF000014
/*
 * arguments:
 *   WMI_TCMD_CONT_RX_CMD rxCmd (see include/wmi.h)
 * uses: WMI_TCMD_CONT_RX_CMDID
 */

#define OID_CAR6K_TCMD_PM	0xFF000015
/*
 * arguments:
 *   WMI_TCMD_PM_CMD pmCmd (see include/wmi.h)
 * uses: WMI_TCMD_PM_CMDID
 */

#endif /* TCMD */

#define OID_CAR6K_802_11_AUTH_ALG 0xFF000020
#define OID_CAR6K_GET_PNP_POWER   0xFF000021
#define OID_CAR6K_RESUME_WLAN_STATE 0xFF000022
#define OID_CAR6K_FIRMWARE_VERSION	0xFF000023


// Changes for backward compatiblilty with WinCE 5.0
#ifndef OID_802_11_PMKID


#define OID_802_11_CAPABILITY 			0x0d010122
#define OID_802_11_PMKID 			0x0d010123

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


#endif //UNDER_CE

#ifdef __cplusplus
}
#endif

#endif /* _ATHDRV_H_ */
