//------------------------------------------------------------------------------
// <copyright file="wmi_config.c" company="Atheros">
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
// wmiconfig.c : WMI configuration application.
//
// Author(s): ="Atheros"
//==============================================================================
#include<stdio.h>
#include<stdlib.h>
#include <windows.h>
#include <winioctl.h>
#include <Ntddndis.h>
#include <Nuiouser.h>
#include <stdarg.h>

#include <a_drv.h>
#include <a_types.h>
#include <athdefs.h>
#include <a_osapi.h>
#include <wmi.h>
#include <Winsock2.h>

// Declare global const

// Option string
#define OPT_I_INTERFACE               __T("-i")
#define OPT_P_POWERMODE               __T("-p")
#define OPT_R_RSSI                    __T("-r")
#define OPT_R_TXPWR                   __T("-txpwr")
#define OPT_SET_TXPWR                 __T("-settxpwr")
#define OPT_R_BSSID                   __T("-bssid")

#define OPT_S_SCANLIST                __T("-s")
#define OPT_WLAN_SETTING              __T("-wlan")
#define OPT_HOST_MODE                 __T("-sethostmode")
#define OPT_WOW_MODE                  __T("-setwowmode")
#define OPT_GET_WOW_LIST              __T("-getwowlist")
#define OPT_ADD_WOW_PATTERN           __T("-addwowpattern")
#define OPT_DEL_WOW_PATTERN           __T("-delwowpattern")
#define OPT_DBGLOG_CFG_MODULE         __T("-setdbglogconfig")
#define OPT_GET_DBGLOGS               __T("-getdbglogs")
#define OPT_SCAN_PARAMS               __T("-scan")
#define OPT_ROAMCTRL                  __T("-roam")
#define OPT_WMODE                     __T("-wmode")
#define OPT_IBSSPM                    __T("-ibsspmcaps")
#define OPT_PMPARAMS                  __T("-pmparams")

#define WOW_HOST_AWAKE                __T("awake")
#define WOW_HOST_ASLEEP               __T("asleep")

#define WOW_ENABLE                    __T("enable")
#define WOW_DISABLE                   __T("disable")

#define PM_REC                        __T("rec")
#define PM_MAXPERF                    __T("maxperf")

#define WS_ENABLE                     __T("enable")
#define WS_DISABLE                    __T("disable")

#define OPT_CREATE_QOS                __T("-createqos")
#define OPT_DELETE_QOS                __T("-deleteqos")
#define OPT_GET_QOS_QUEUE             __T("-qosqueue")
#define OPT_SET_WMM                   __T("-setwmm")
#define OPT_SET_TXOP                  __T("-txop")
#define OPT_SET_BTSTATUS              __T("-setBTstatus")
#define OPT_SET_BTPARAMS              __T("-setBTparams")

#define OPT_SET_ATIM                  __T("-atim")

#define DBGLOG_MASK                   __T("-mmask")
#define DBGLOG_REP                    __T("-rep")
#define DBGLOG_TSR                    __T("-tsr")
#define DBGLOG_SIZE                   __T("-size")

#define ROAM_MODE_DEF                 __T("default")
#define ROAM_MODE_BIAS                __T("bssbias")
#define ROAM_MODE_LOCK                __T("lock")

#define WMODE_SCAN                    __T("sc")

#define OPT_IBSS_CHANNEL              __T("-ibssChannel")

#define RSSI_NDIS2ABS(x)              ((x) + 95)
/* following defines are required as wmi defined enum values are
 * different from ndis defined values*/
#define NDIS_MAX_PERF_POWER              0
#define NDIS_REC_POWER             1

#define SCANPARAMS_FGSTART            __T("-fgstart")
#define SCANPARAMS_FGEND              __T("-fgend")
#define SCANPARAMS_BG                 __T("-bg")
#define SCANPARAMS_MINACT             __T("-minact")
#define SCANPARAMS_MAXACT             __T("-maxact")
#define SCANPARAMS_PASSIVE            __T("-pas")
#define SCANPARAMS_SHORTSCANRATIO     __T("-sr")
#define SCANPARAMS_SCANCTRLFLAGS      __T("-scanctrlflags")
#define SCANPARAMS_MAXACT_SCAN_SSID   __T("-maxactscan_ssid")

#define IBSSPM_PS                     __T("-ps")
#define IBSSPM_TTL                    __T("-ttl")
#define IBSSPM_TO                     __T("-to")
#define IBSSPM_AW                     __T("-aw")

#define PM_IT                         __T("-it")
#define PM_NP                         __T("-np")
#define PM_DP                         __T("-dp")

#define DTIM_IGNORE                   __T("ignore")
#define DTIM_NORMAL                   __T("normal")
#define DTIM_STICK                    __T("stick")

#define OPT_DUMP_HTC_STATE            __T("-dumphtc")

#define OPT_WMI_GET_VERSION           __T("-version")

#define OPT_ABORT_SCAN                __T("-abortscan")

#define OPT_GET_RD                    __T("-getRD")

#define OPT_GET_TGT_STATS             __T("-getTgtstats")

#define OPT_RSSITHRESH                __T("-rssiThreshold")

#define OPT_GET_BITRATE               __T("-bitRate")

#define OPT_SET_BITRATE               __T("-setfixrates")

#define OPT_SET_APMODE                __T("-setAP")

#define APMODE_SSID                   __T("-ssid")

#define APMODE_CHANNEL               __T("-channel")

#define OPT_SET_STAMODE              __T("-setSTA")

#define OPT_GET_WMODE                 __T("-getWmode")

#define APKEY_INDEX                  __T("-keyIndex")

#define APKEY_KEY                    __T("-key")

#define APKEY_AUTH                   __T("-auth")

#define APKEY_AUTH_OPEN              __T("open")

#define APKEY_AUTH_SHARED            __T("shared")

#define APKEY_AUTH_WPA_PSK            __T("WPA-PSK")

#define APKEY_AUTH_WPA2_PSK           __T("WPA2-PSK")

#define APKEY_CRYPT                   __T("-crypt")

#define APKEY_CRYPT_NONE              __T("none")

#define APKEY_CRYPT_WEP               __T("WEP")

#define APKEY_CRYPT_TKIP              __T("TKIP")

#define APKEY_CRYPT_AES               __T("AES")

#define OPT_QUERY_CURRENT_MODE       __T("-getMode")

#define OPT_SET_NUMSTA               __T("-numsta")

#define OPT_GET_STA                  __T("-getsta")

#define OPT_ADD_ACL                  __T("-addacl")

#define OPT_DEL_ACL                  __T("-delacl")

#define OPT_GET_ACL                  __T("-getacl")

#define APMODE_DTIM                  __T("-dtim")

#define APMODE_BG_PROTECTION         __T("-protectionscan")

#define APMODE_HIDDEN_SSID           __T("-hiddenssid")

#define OPT_SET_COUNTRY              __T("-setcountry")

#define OPT_GET_COUNTRY_LIST         __T("-countrycodes")

#define OPT_CONN_INACT               __T("-conninact")

#define OPT_REM_STA                  __T("-removesta")

#define OPT_SET_BEACON_INT           __T("-beaconintvl")

#define OPT_SET_AP_SECURITY          __T("-setAPkey")

#define OPT_SET_IP                   __T("-ip")

#define SET_IP_OPTION                __T("none")

#define OPT_SET_ACL_POLICY           __T("-aclpolicy")

#define OPT_STA_ISOLATION            __T("-intrabss")

#define OPT_SET_NODEAGE               __T("-setnodeage")
#define OPT_GET_NODEAGE               __T("-getnodeage")
#define OPT_STA_LISTEN_INTVL         __T("-listen")
#define OPT_SET_KEEPALIVE         __T("-setkeepalive")
#define OPT_GET_KEEPALIVE         __T("-getkeepalive")

//To set the Power state D0/D3
#define OPT_SET_PWRSTATE             __T("-setpowerstate")

#define WIFI_ON                      __T("on")

#define WIFI_OFF                     __T("off")

//To set the IP address to the adapter
#define OPT_SET_IP_ADDRESS           __T("-setip")

#define DHCP_ON                      __T("dhcp")

#define DHCP_OFF                     __T("static")

#define OPT_DUMP_CHIP_MEM             __T("-dumpchipmem")


/* Registry key definitions */

#define Ar6kIPKey                       L"IpAddress"
#define Ar6kSubMaskKey                  L"Subnetmask"
#define Ar6kEnableDHCP                  L"EnableDHCP"


enum {
    WMI_GET_VERSION=501,     // something that doesn't collide with ascii
    WMI_SET_POWER_MODE,
    WMI_SET_IBSS_PM_CAPS,
    WMI_SET_PM_PARAMS,
    WMI_SET_SCAN_PARAMS,
    WMI_SET_LISTEN_INTERVAL,
    WMI_SET_BMISS_TIME,
    WMI_SET_BSS_FILTER,
    WMI_SET_RSSI_THRESHOLDS,
    WMI_SET_CHANNEL,
    WMI_SET_SSID,
    WMI_SET_BADAP,
    WMI_DELETE_BADAP,
    WMI_CREATE_QOS,
    WMI_DELETE_QOS,
    WMI_GET_QOS_QUEUE,
    WMI_GET_TARGET_STATS,
    WMI_SET_TARGET_ERROR_REPORTING_BITMASK,
    WMI_SET_AC_PARAMS,
    WMI_SET_ASSOC_IE,
    WMI_SET_DISC_TIMEOUT,
    WMI_SET_ADHOC_BSSID,
    WMI_SET_OPT_MODE,
    WMI_OPT_SEND_FRAME,
    WMI_SET_BEACON_INT,
    WMI_GET_RSSI,
    WMI_SET_VOICE_PKT_SIZE,
    WMI_SET_MAX_SP,
    WMI_GET_ROAM_TBL,
    WMI_SET_ROAM_CTRL,
    WMI_SET_POWERSAVE_TIMERS,
    WMI_SET_POWERSAVE_TIMERS_PSPOLLTIMEOUT,
    WMI_SET_POWERSAVE_TIMERS_TRIGGERTIMEOUT,
    WMI_GET_POWER_MODE,
    WMI_SET_WLAN_STATE,
    WMI_GET_ROAM_DATA,
    WMI_SET_BT_STATUS,
    WMI_SET_BT_PARAMS,
    WMI_SET_RETRYLIMITS,
    WMI_START_SCAN,
    WMI_SET_FIX_RATES,
    WMI_GET_FIX_RATES,
    WMI_SET_SNR_THRESHOLDS,
    WMI_CLR_RSSISNR,
    WMI_SET_LQ_THRESHOLDS,
    WMI_SET_AUTH_MODE,
    WMI_SET_REASSOC_MODE,
    WMI_SET_LPREAMBLE,
    WMI_SET_RTS,
    WMI_SET_WMM,
#ifdef USER_KEYS
    USER_SETKEYS,
#endif
    WMI_APSD_TIM_POLICY,
    WMI_SET_ERROR_DETECTION,
    WMI_GET_HB_CHALLENGE_RESP,
    WMI_SET_TXOP,
    DIAG_ADDR,
    DIAG_DATA,
    DIAG_READ,
    DIAG_WRITE,
    WMI_GET_RD,
    WMI_SET_KEEPALIVE,
    WMI_GET_KEEPALIVE,
    WMI_SET_APPIE,
    WMI_SET_MGMT_FRM_RX_FILTER,
    WMI_DBGLOG_CFG_MODULE,
    WMI_DBGLOG_GET_DEBUG_LOGS,
    WMI_SET_HOST_SLEEP_MODE,
    WMI_SET_WOW_MODE,
    WMI_GET_WOW_LIST,
    WMI_ADD_WOW_PATTERN,
    WMI_DEL_WOW_PATTERN,
    WMI_GET_SCANLIST,
    WMI_SET_WLAN_SETTING,
    WMI_SET_BTSTATUS,
    WMI_SET_BTPARAMS,
    WMI_SET_ROAMCTRL,
    WMI_GET_BSSID,
    WMI_SET_IBSS_CHANNEL,
    WMI_SET_ATIM,
    DUMP_HTC_STATE,
    WMI_GET_TXPOWER,
    WMI_SET_TXPOWER,
    WMI_ABORT_SCAN,
    WMI_GET_BITRATE,
    WMI_GET_TGT_STATS,
    WMI_GET_WMODE,
    WMI_SET_APMODE,
    WMI_SET_STAMODE,
    WMI_GET_CURRENT_MODE,
    WMI_SET_NUMSTA,
    WMI_GET_STA,
    WMI_AP_ACL_MAC_LIST1,
    WMI_AP_ACL_MAC_LIST2,
    WMI_AP_GET_ACL_LIST,
    WMI_AP_ACL_POLICY,
    WMI_AP_INACT_TIME,
    WMI_AP_REMOVE_STA,
    WMI_SET_COUNTRY,
    WMI_SET_BG_PROTECTION,
    WMI_SET_HIDDEN_SSID,
    WMI_SET_AP_DTIM,
    WMI_SET_IP,
    WMI_SET_AP_SECURITY,
    WMI_AP_INTRA_BSS,
    WMI_SET_NODEAGE,
    WMI_GET_NODEAGE,
    WMI_SET_POWER_STATE,
    WMI_SET_IP_ADDRESS,
    WMI_DUMP_CHIP_MEM,
};

typedef enum {
    WLAN_ENABLE = 0x1,
    WLAN_DISABLE,
} WLAN_SETTING;

typedef struct {
    A_UINT8  wlanSetting;
} WLAN_SETTING_CMD;

typedef struct {
    A_UINT8  ibssChannel;
} IBSS_CHANNEL_CMD;

typedef struct {
    BYTE ipAddr[32];
    BYTE subnetMsk[32];
} IP_ADDRESS;

A_UINT32 mercuryAdd[5][2] = {
            {0x20000, 0x200fc},
            {0x28000, 0x28800},
            {0x20800, 0x20a40},
            {0x21000, 0x212f0},
            {0x500000, 0x500000+184*1024},
};

int TelPrintf (CHAR *pFormat, ...)
{
    va_list     ArgList;
    CHAR        Buffer[256];
    int         RetVal;
    DWORD       dwReturn = 0;


    HANDLE hSerial = CreateFile(
       TEXT("TEL0:"),
       GENERIC_READ | GENERIC_WRITE,
       0,
       NULL,
       OPEN_EXISTING,
       0,
       NULL
    );

    if(hSerial == INVALID_HANDLE_VALUE)
        return 0;

    va_start (ArgList, pFormat);

    RetVal = StringCchVPrintfA (Buffer, 255, pFormat, ArgList);

    do
    {
       if (hSerial)
       {
            if (FALSE == WriteFile (hSerial, Buffer, strlen (Buffer), &dwReturn, NULL))
            {
                //
                // Telnet service is not enabled
                break;
            }

            if (FALSE == WriteFile (hSerial, "\r\n", 2, &dwReturn, NULL))
            {
                //
                // Telnet service is not enabled
                break;
            }
       }

    }while (FALSE);


    if (hSerial)
    {
       CloseHandle (hSerial);
       hSerial = NULL;
    }

    //
    // do a normal print statement
    //
    printf ("%s\n",Buffer);

    return RetVal;
}

#define ERRX(x)                           x ## ",errno:%u\n"
#define ERR(x)                            TelPrintf(ERRX(x),GetLastError())

void
PrintUsage (void)
{
    TelPrintf("Usage: wmiconfig -i <device> commands\r\n");
    TelPrintf("\r\n");
    TelPrintf("Commands:\r\n");
    TelPrintf("-p <mode> where <mode> is rec or maxperf to set power mode \r\n");
    TelPrintf("-r to get RSSI \r\n");
    TelPrintf("-bssid to get Associated BSSID \r\n");
    TelPrintf("-s to get SCAN LIST \r\n");
    TelPrintf("-wlan <setting> where <setting> is enable or disable to ON/OFF the device \r\n");
    TelPrintf("-sethostmode <awake/asleep> \r\n");
    TelPrintf("-setwowmode <enable/disable> \r\n");
    TelPrintf("-getwowlist <listid> \r\n");
    TelPrintf("-addwowpattern <list-id> <pattern-size> <pattern-offset> <pattern> <pattern-mask \r\n");
    TelPrintf("-delwowpattern <list-id> <pattern-id> \r\n");

    TelPrintf ("-createqos <user priority> <direction> <traffic class> <trafficType> <voice PS capability> \r\n");
    TelPrintf ("        <min service interval> <max service interval> <inactivity interval> <suspension interval> \r\n");
    TelPrintf ("        <service start time> <tsid> <nominal MSDU> <max MSDU> <min data rate> <mean data rate> \r\n");
    TelPrintf ("        <peak data rate> <max burst size> <delay bound> <min phy rate> <sba> <medium time>where:\r\n");
    TelPrintf ("        <user priority>         802.1D user priority range : 0-7        \r\n");
    TelPrintf ("        <direction>             is 0 for Tx(uplink) traffic,            \r\n");
    TelPrintf ("                                    1 for Rx(downlink) traffic,          \r\n");
    TelPrintf ("                                    2 for bi-directional traffic;        \r\n");
    TelPrintf ("        <traffic class>         is 1 for BK,                            \r\n");
    TelPrintf ("                                    2 for VI,                            \r\n");
    TelPrintf ("                                    3 for VO;                            \r\n");
    TelPrintf ("        <trafficType>           1-periodic, 0-aperiodic                 \r\n");
    TelPrintf ("        <voice PS capability>   specifies whether the voice power save mechanism \r\n");
    TelPrintf ("                            (APSD if AP supports it or legacy/simulated APSD \r\n");
    TelPrintf ("                                [using PS-Poll] ) should be used             \r\n");
    TelPrintf ("                            = 0 to disable voice power save for this traffic class,\r\n");
    TelPrintf ("                            = 1 to enable APSD voice power save for this traffic class,\r\n");
    TelPrintf ("                            = 2 to enable voice power save for ALL traffic classes,\r\n");
    TelPrintf ("        <min service interval>  in milliseconds                     \r\n");
    TelPrintf ("        <max service interval>  in milliseconds                    \r\n");
    TelPrintf ("        <inactivity interval>   in milliseconds;=0 means infinite inactivity interval\r\n");
    TelPrintf ("        <suspension interval>   in milliseconds \r\n");
    TelPrintf ("        <service start time>    service start time \r\n");
    TelPrintf ("        <tsid>                  TSID range: 0-15                    \r\n");
    TelPrintf ("        <nominal MSDU>          nominal MAC SDU size                \r\n");
    TelPrintf ("        <max MSDU>              maximum MAC SDU size                \r\n");
    TelPrintf ("        <min data rate>         min data rate in bps                \r\n");
    TelPrintf ("        <mean data rate>        mean data rate in bps               \r\n");
    TelPrintf ("        <peak data rate>        peak data rate in bps               \r\n");
    TelPrintf ("        <max burst size>        max burst size in bps               \r\n");
    TelPrintf ("        <delay bound>           delay bound                         \r\n");
    TelPrintf ("        <min phy rate>          min phy rate in bps                 \r\n");
    TelPrintf ("        <sba>                   surplus bandwidth allowance         \r\n");
    TelPrintf ("        <medium time>           medium time in TU of 32-us periods per sec    \r\n");
    TelPrintf ("-deleteqos <traffic class> <tsid> where:\r\n");
    TelPrintf ("        <traffic class> is 1 for BK, 2 for VI, 3 for VO;\r\n");
    TelPrintf ("        use --qosqueue option to get the <tsid> number \r\n");
    TelPrintf ("-qosqueue <traffic class>, where:\r\n");
    TelPrintf ("        <traffic class> is 1 for BK, 2 for VI, 3 for VO;\r\n");
    TelPrintf("-setdbglogconfig -mmask <mask> -rep <0/1> -tsr <tsr codes> -size <num>\n");
    TelPrintf("\t where <mask> is a 16 bit wide mask to selectively enable logging for different modules. Example: 0xFFFD enables logging for all modules except WMI. The mask is derived from the module ids defined in etna/include/dbglog.h header file\n");
    TelPrintf("\t <rep> is whether the target should generate log event to the host whenever the log buffer is full.\n");
    TelPrintf("\t <tsr> resolution of the debug timestamp (less than 16)\n");
    TelPrintf("\t\t 0: 31.25 us\n");
    TelPrintf("\t\t 1: 62.50 us\n");
    TelPrintf("\t\t 2: 125.0 us\n");
    TelPrintf("\t\t 3. 250.0 us\n");
    TelPrintf("\t\t 4. 500.0 us\n");
    TelPrintf("\t\t 5. 1.0 ms and so on.\n");
    TelPrintf("\t <size> size of the report in number of debug logs\n");
    TelPrintf("-getdbglogs\n");
    TelPrintf("--setBTstatus <streamType> <status>\n");
    TelPrintf("      where <streamType> is    1 - Bluetooth SCO stream\n");
    TelPrintf("                               2 - Bluetooth A2DP stream\n");
    TelPrintf("                               3 - Bluetooth Inquiry/low priority stream\n");
    TelPrintf("                               4 - Bluetooth E-SCO stream\n");
    TelPrintf("      where <status> is        1 - stream started\n");
    TelPrintf("                               2 - stream stopped\n");
    TelPrintf("                               3 - stream resumed\n");
    TelPrintf("                               4 - stream suspended\n");
    TelPrintf("  \n");
    TelPrintf("--setBTparams <paramType> <params>\n");
    TelPrintf("  where <paramType> is     1 - Bluetooth SCO stream parameters\n");
    TelPrintf("                           2-  Bluetooth A2DP stream parameters\n");
    TelPrintf("                           3 - Front end antenna configuration \n");
    TelPrintf("                           4 - Co-located Bluetooth configuration\n");
    TelPrintf("                           5 -Bluetooth ACL coex(non a2dp) parameters \n");
    TelPrintf("  where <params> for Bluetooth SCO are:\n ");
    TelPrintf("   <numScoCyclesForceTrigger> - number of Sco cyles, to force a trigger\n ");
    TelPrintf("   <stompScoRules> - Applicable for dual/splitter front end\n ");
    TelPrintf("                      1, Never stomp BT to receive downlink pkt\n ");
    TelPrintf("                      2, Always stomp BT to receive downlink pkt\n ");
    TelPrintf("                      3, Stomp BT only during low rssi conditions\n ");
    TelPrintf("   <dataResponseTimeout> - timeout for receiving downlink packet per PS-poll\n ");
    TelPrintf("   <stompDutyCyleVal> If Sco is stomped while waiting for downlink pkt, number sco cyles to not queue ps-poll-(Applicable only for switch FE)\n ");
    TelPrintf("   <psPollLatencyFraction> Fraction of idle SCO idle time.\n ");
    TelPrintf("                       1, if more than 3/4 idle duration is left, retrieve downlink pkt\n ");
    TelPrintf("                       2, if more than 1/2 idle duration is left, retrieve downlink pkt\n ");
    TelPrintf("                       3, if more 1/4 idle duration is left, retrieve dwnlink pkt\n ");
    TelPrintf("   <SCO slots> - number of Tx+Rx SCO slots : 2 for single-slot SCO, 6 for 3-slot SCO\n");
    TelPrintf("   <Idle SCO slots> - number of idle slots between two SCO Tx+Rx instances\n");
    TelPrintf("  where <params> for Bluetooth A2DP are \n");
    TelPrintf("    <a2dpWlanUsageLimit> Max duration wlan can use the medium ,whenever firmware detects medium for wlan (in msecs)\n");
    TelPrintf(" <a2dpBurstCntMin> Mininum number of bluetooth data frames to replenish wlan usage time \n");
    TelPrintf(" <a2dpDataRespTimeout> - timeout to receive downlink data per ps-poll\n");
    TelPrintf("<isColocatedBtRoleMaster> - Colocated Bluetooth Role \n");
    TelPrintf("                 0 -co-located role is slave (default)\n");
    TelPrintf("                 1 -co-located role is Master \n");
    TelPrintf("  where <params> for front end antenna configuration are\n");
    TelPrintf("   <antType> - Antenna configuration\n");
    TelPrintf("            1, Dual antenna configuration (BT and wlan have seperate antenna) \n");
    TelPrintf("            2, Single antenna splitter configuration \n");
    TelPrintf("            3, Single antenna switch  configuration \n");
    TelPrintf("  where <params> for co-located Bluetooth configuration are\n");
    TelPrintf("   <coLocatedBtDev> - Co-located Bluetooth configuration\n");
    TelPrintf("            0, Qualcomm BTS402x (default)\n");
    TelPrintf("            1, CSR Bluetooth\n");
    TelPrintf("            2, Atheros Bluetooth\n");
    TelPrintf(" where <params> for Bluetooth ACL coex (during bluetooth ftp/opp or any other ACL based profile other than a2dp)are \n");
    TelPrintf("  <aclWlanMediumUsageTime> - Wlan usage time (default 30 msecs)\n");
    TelPrintf("  <aclBtMediumUsageTime> - Bt usage time (default 30 msecs)\n");
    TelPrintf(" <aclDataRespTimeout> - timeout to receive downlink data per ps-poll\n");
    TelPrintf("  \n");
    TelPrintf("  where <params> for Bluetooth co-existence register parameters are:\n");
    TelPrintf("                      <mode> - co-existence mode\n");
    TelPrintf("               <sco weights> - Bluetooth and WLAN weights for SCO\n");
    TelPrintf("              <a2dp weights> - Bluetooth and WLAN weights for A2DP\n");
    TelPrintf("           <general weights> - Bluetooth and WLAN weights for profiles other than SCO and A2DP\n");
    TelPrintf("                     <mode2> - co-existence mode2\n");
    TelPrintf("                <set values> - if 1, above values are set in registers right away\n");
    TelPrintf("                                  0, registers are kept unmodified until BT status command is sent\n");
    TelPrintf("-scan -fgstart <sec> -fgend <sec> -bg <sec> -minact <msec> -maxact <msec> -pas <msec> -sr <short  scan ratio> -scanctrlflags <connScan> <scanConnected> <activeScan> <roamScan> <reportBSSINFO> <EnableAutoScan> -maxactscan_ssid=<Max no of active scan per probed ssid>");
    TelPrintf("--roam <roamctrl> <info>\n");
TelPrintf("       where <roamctrl> is   1  force a roam to specified bssid\n");
TelPrintf("                             2  set the roam mode \n");
TelPrintf("                             3  set the host bias of the specified BSSID\n");
TelPrintf("                             4 set the lowrssi scan parameters \n");
TelPrintf("      where <info> is BSSID<aa:bb:cc:dd:ee:ff> for roamctrl of 1\n");
TelPrintf("                      DEFAULT ,BSSBIAS or LOCK for roamctrl of 2\n");
TelPrintf("                      BSSID<aa:bb:cc:dd:ee:ff> <bias> for  roamctrl of 3\n");
TelPrintf("                             where <bias> is  a value between -256 and 255\n");
TelPrintf("                      <scan period> <scan threshold> <roam threshold> \n");
TelPrintf("                      <roam rssi floor> for roamctrl of 4\n");
TelPrintf("-ibssChannel <channel Number> where <channel number is IEEE channel number to connect to IBSS network \n");
TelPrintf("-wmode <mode> <list> sc <scan> where \n \t<mode> is a, g, b or ag \n");
TelPrintf("\t<list> is list of IEEE channel number or freq in mhz\n");
TelPrintf("\t<scan> is 0 to disable or 1 to enable scan after setting channel list\n");
TelPrintf("-ibsspmcaps -ps <enable/disable> -aw <ATIM Windows in millisecond> -to=<TIMEOUT in millisecond> -ttl <Time to live in number of beacon periods>\n");
TelPrintf("-pmparams -it <msec> -np <number of PS POLL> -dp <DTIM policy: ignore/normal/stick>\n");
TelPrintf("-dumphtc - dump htc credit state \n");
TelPrintf("-txpwr - prints the transmit power\n");
TelPrintf("-settxpwr - sets the transmit power\n");
TelPrintf("-version - get the host and target version\n");
TelPrintf("-abortscan - to abort the scan issued from the host\n");
TelPrintf("-getRD - get the regulatory domain code\n");
TelPrintf("-getTgtstats - get station stats \n");
TelPrintf("-rssiThreshold - to set rssi thresholds\n");
TelPrintf("-getWmode - to get wireless mode\n");
TelPrintf("-setAP -ssid <ssid name> -channel <channel number> -to set the station to AP mode \n");
TelPrintf("-setSTA - to set softAP to station mode \n");
TelPrintf("-setAPkey -keyIndex <index> -key <key value> -auth <mode> -crypt <type> - to set security keys in AP mode \n");
TelPrintf("        where <index> is a value between 1 to 4 , Index must be 1 for TKIP,AES \n");
TelPrintf("        where <key value> is either  5 or 13  key length (ascii), 10 or 26  key length (hex) for WEP (open/shared) (or) 8 to 63 (ascii) or 64 (hex) for TKIP,AES\n");
TelPrintf("        where <mode> is either 'open' for disabled, open wep, or 'shared' for shared wep or 'WPA-PSK' for TKIP or 'WPA2-PSK' for AES. WPA-PSK/AES & WPA2-PSK/TKIP are not supported \n");
TelPrintf("        where <type> is either 'none' for disabled, 'WEP' for wep crypt, 'TKIP' for TKIP crypt, 'AES' for AES crypt\n");
TelPrintf("        eg. -setAPkey -keyIndex 1 -key 1234567890 -auth open -crypt WEP\n");
TelPrintf("-getMode - to get the current mode (AP/STA) \n");
TelPrintf("-numsta <value> - to limit the number of stations that can connect to the AP \n");
TelPrintf("        where <value> is a value between 1 to 8.Default value is 8\n");
TelPrintf("-getsta - to get the list of connected STAs in soft AP mode \n");
TelPrintf("-hiddenssid <value>- to set hidden ssid option in soft AP mode. \n When this is set the soft AP will not advertise SSID in beacon \n");
TelPrintf("        where <value>  1 for enable 0 for disable \n");
TelPrintf("-addacl <mac addr> - to add a MAC address to soft AP's ACL allow list. \n When a MAC addresses are added to the list the soft AP will allow only the stations with those mac addresses to connect.\n");
TelPrintf("         where <mac addr> is of the format xx:xx:xx:xx:xx:xx \n");
TelPrintf("-getacl - to get the allow/deny list in softAP mode\n");
TelPrintf("-delacl <index> - to delete a mac address from the allow/deny list softAP mode\n");
TelPrintf("         where <index> of the mac address to be deleted can be got using -getacl command\n");
TelPrintf("-aclpolicy <policy> <retain list> to define the policy either allow/deny in AP mode \n ");
TelPrintf("         where <policy> may be 0 - Disable ACL, 1 - Allow MAC or 2 - Deny MAC \n");
TelPrintf("         where <retain list> may be 0 - Clear the current ACL list or 1 - Retain the current ACL list\n");
TelPrintf("-dtim <period> - to set the dtim period in soft AP mode \n");
TelPrintf("         where <period> is a value between 1-10 (both inclusive). Default value is 5 \n");
TelPrintf("-protectionscan <period> <dwell> -to set the BG mode protection timers \n");
TelPrintf("         where <period> is time in minutes at which the AP does a scan. Default value is 5 min\n");
TelPrintf("         where <dwell> is time in ms,duration of scan.Default value is 200 ms\n");
TelPrintf("         to disable protection set both the parameters to 0 \n");
TelPrintf("-countrycodes -prints the list of country codes \n");
TelPrintf("-setcountry <countrycode> - to set the country code in AP and Ad-Hoc mode\n");
TelPrintf("         where <countrycode> can be got using -countrycodes command\n");
TelPrintf("-conninact <value> - to set the STA communication time out value.\n              If there is no data activity for the time out period the station is removed by sending a deauth \n");
TelPrintf("         where <value> is time in minutes. default is 5 mins \n");
TelPrintf("-removesta <action> <reason> <mac> - to remove a connected station \n");
TelPrintf("         where <action> is either disassoc or deauth \n");
TelPrintf("                2 - Disassoc    \n");
TelPrintf("                3 - Deauth     \n");
TelPrintf("         where <reason> is protocol reason code. use 1 when in doubt \n");
TelPrintf("         where <mac> mac addr of a connected STA,in the format xx:xx:xx:xx:xx:xx  \n");
TelPrintf("-beaconintvl <value> - to set the beacon interval in AP/ADHOC mode \n");
TelPrintf("         where <value> is beacon interval in milli seconds \n");
TelPrintf("-ip <arg> -to set IP for ARP offloading   may be \n");
TelPrintf("         where <arg> may be none - resets ip (or) x.x.x.x - ip addr is dotted form (maximum of 2)\n");
TelPrintf("-intrabss <ctrl> - to enable/disable traffic between the connected stations  \n");
TelPrintf("         where <ctrl> 0 - Disable, 1 - Enable (default) \n");
    TelPrintf("-setfixrates <rate index> where: \n");
    TelPrintf("  <rate index> is {0 1M},{1 2M},{2 5.5M},{3 11M},{4 6M},{5 9M},{6 12M},{7 18M},{8 24M},{9 36M},{10 48M},{11 54M},\n");
    TelPrintf("  if want to config more rate index, can use blank to space out, such as: --setfixrates 0 1 2 \n");
TelPrintf("-listen <TUs> - To set listen interval in station mode. TUs, can  range from 15 to 5000 \n");
TelPrintf("-setnodeage <value in msec>\n");
TelPrintf("-getnodeage\n");
TelPrintf("-setkeepalive <keepalive interval in sec>\n");
TelPrintf("-getkeepalive\n");
    TelPrintf("-setpowerstate <on/off> - set power state on/off - D0/D3 power state \n");
    TelPrintf("-setip <dhcp/static> <ip_addr> <net_mask> \n");
    TelPrintf("     dhcp - To Enable DHCP ; static - To Disable DHCP\n");
    TelPrintf("     <ip_addr> and <net_mask> will not be considered when <dhcp> is on \n");
TelPrintf("-dumpchipmem - to dump chip memory\n");
}

typedef struct {
    _TCHAR *pOption;
    WMI_PHY_MODE phyMode;
} PHY_MODE_MAP;

PHY_MODE_MAP phyModeMap [] = { {__T("a"), WMI_11A_MODE},
           {__T("b"), WMI_11B_MODE},
           {__T("g"), WMI_11GONLY_MODE},
           {__T("bg"), WMI_11G_MODE},
           {__T("abg"), WMI_11AG_MODE},
           {NULL} };


char    *my_ctr[] = {
    "DB", "NA", "AL", "DZ", "AR", "AM", "AU", "AT", "AZ", "BH", "BY", "BE", "BZ", "BO", "BR", "BN",
    "BG", "CA", "CL", "CN", "CO", "CR", "HR", "CY", "CZ", "DK", "DO", "EC", "EG", "SV", "EE", "FI",
    "FR", "GE", "DE", "GR", "GT", "HN", "HK", "HU", "IS", "IN", "ID", "IR", "IE", "IL", "IT", "JP",
    "JO", "KZ", "KP", "KR", "K2", "KW", "LV", "LB", "LI", "LT", "LU",
    "MO", "MK", "MY", "MX", "MC", "MA", "NL", "NZ", "NO", "OM", "PK", "PA", "PE", "PH", "PL", "PT",
    "PR", "QA", "RO", "RU", "SA", "SG", "SK", "SI", "ZA", "ES", "SE", "CH", "SY", "TW", "TH", "TT",
    "TN", "TR", "UA", "AE", "GB", "US", "UY", "UZ", "VE", "VN", "YE", "ZW"
    };


A_INT8
getPhyMode ( _TCHAR* arg )
{
    int i;

    for (i=0; phyModeMap[i].pOption !=NULL; i++ ) {
    if ( _tcscmp(phyModeMap[i].pOption,arg) == 0 ) {
            return phyModeMap[i].phyMode;
        }
    }
    return -1;
}

A_UINT16
wmic_ieee2freq( int chan )
{
    if (chan == 14) {
        return 2484;
    }
    if (chan < 14) {
        return (2407 + (chan*5));
    }
    if ( chan <27 ) {
        return (2512 + ((chan-15)*20));
    }
    return (5000 + (chan*5));
}

static int is_mac_null(A_UINT8 *mac)
{
    if(mac[0]==0 && mac[1]==0 && mac[2]==0 &&
       mac[3]==0 && mac[4]==0 && mac[5]==0) {
        return 1;
    }
    return 0;
}

A_STATUS
wmic_ether_aton_wild(TCHAR *orig, A_UINT8 *eth, A_UINT8 *wild)
{
  TCHAR *bufp;
  TCHAR val, c;
  int i=0;

  *wild = 0;
  for(bufp = orig; *bufp != '\0'; ++bufp) {
    c = *bufp++;
    if (isdigit(c)) val = c - '0';
    else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
    else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
    else if (c == '*')  { val = 0; *wild |= 1<<i; goto next; }
    else break;

    val <<= 4;
    c = *bufp++;
    if (isdigit(c)) val |= c - '0';
    else if (c >= 'a' && c <= 'f') val |= c - 'a' + 10;
    else if (c >= 'A' && c <= 'F') val |= c - 'A' + 10;
    else break;

next:
    eth[i] = (unsigned char) (val & 0xFF);
    if(++i == ATH_MAC_LEN) {
        /* That's it.  Any trailing junk? */
        if (*bufp != '\0') {
        }
        return(A_OK);
    }
    if (*bufp != ':')
        break;
  }

  return(A_EINVAL);
}


void print_wild_mac(unsigned char *mac, char wildcard)
{
    int i;

    printf("    ");
    for(i=0;i<5;i++)
    {
        if(wildcard & (1<<i))
        printf("*:");
        else
        printf("%2.2X:", mac[i]);
    }

    if(wildcard & (1<<i))
    printf("*");
    else
    printf("%2.2X", mac[5]);
}


A_STATUS
validate_ap_key (AP_MODE_CRYPT *apsec)
{
    BOOL IsAscii = FALSE;
    UINT8 keyarr[32];
    UINT8 tempch;
    UINT8 k;
    BYTE tempbyte;
    A_STATUS status = A_OK;

    if (apsec->cryptType == NONE_CRYPT)
    {

        if((apsec->KeyIndex) || (apsec->IsAscii) || (apsec->Authmode != NONE_AUTH))
        {
           return(A_EINVAL);
        }
    }

    if (apsec->cryptType == WEP_CRYPT)
    {
        if ((apsec->KeyIndex < 0) || (apsec->KeyIndex > 3))
        {
           TelPrintf("Invalid WEP mode key Index \n");
           return(A_EINVAL);
        }
        switch (apsec->KeyLength)
        {
            case 5: case 13:
                IsAscii = TRUE;
                apsec->IsAscii = TRUE;
                break;
            case 10: case 26:
                IsAscii = FALSE;
                apsec->IsAscii = FALSE;
                break;
            default:
                TelPrintf("invalid key length  5 or 13 (ascii), 10 or 26 (hex)");
                return(A_EINVAL);
        }
        if (apsec->Authmode != NONE_AUTH)
        {
            TelPrintf("Invalid auth mode \n");
            return(A_EINVAL);
        }

        apsec->KeyIndex = apsec->KeyIndex | 0x80000000;

    }

    if ((apsec->cryptType == TKIP_CRYPT) || (apsec->cryptType == AES_CRYPT))
    {
        if (((apsec->cryptType == TKIP_CRYPT) && (apsec->Authmode != WPA_PSK_AUTH)) ||
            ((apsec->cryptType == AES_CRYPT) && (apsec->Authmode != WPA2_PSK_AUTH)))
        {
            TelPrintf("Invalid auth mode \n");
            return (A_EINVAL);
        }
        if (apsec->KeyIndex != 0)
        {
            apsec->KeyIndex = 0;
        }
        if ((apsec->KeyLength < 64) && (apsec->KeyLength >= 8))
        {
            IsAscii = TRUE;
            apsec->IsAscii = TRUE;

        }
        else if ((apsec->KeyLength > 64) || (apsec->KeyLength < 8))
        {
            TelPrintf("Invalid key length should be 8 to 63 length(ascii)or 64 (hex)");
            return (A_EINVAL);
        }
        else if(apsec->KeyLength == 64)
        {
            IsAscii = FALSE;
            apsec->IsAscii = FALSE;
        }
    }

    if (!IsAscii)
    {
        for (k = 0, tempbyte = 0 ; k < apsec->KeyLength ; ++k)
        {
            tempch = (UINT8)(apsec->KeyMaterial[k]);
            switch (tempch)
            {
                case L'0': case L'1': case L'2': case L'3': case L'4':
                case L'5': case L'6': case   L'7': case L'8': case L'9':
                    tempch -= L'0';
                    break;
                case L'a': case L'b': case L'c':
                case L'd': case L'e': case L'f':
                    tempch -= L'a' - 10;
                    break;
                case L'A': case L'B': case L'C':
                case L'D': case L'E': case L'F':
                    tempch -= L'A' - 10;
                    break;
                default:
                    TelPrintf("Invalid HEX WEP key \n");
                    return(A_EINVAL);
            }
            if ((k & 1) == 0) {
                tempbyte = tempch;
            }else{
                tempbyte = (tempbyte << 4);
                tempbyte |= tempch;
                keyarr[k/2] = tempbyte;
            }
        }
        memcpy(apsec->KeyMaterial,keyarr,(apsec->KeyLength/2));
        apsec->KeyLength = apsec->KeyLength/2;
    }

  return status;
}


static BOOL IssueSetOid(HANDLE  hAdapter,
                        TCHAR   *DevName,
                        DWORD   OidCode,
                        PUCHAR  pData,
                        int     Length)
{
    PNDISUIO_SET_OID Oid;
    DWORD            dwReturnedBytes;
    PUCHAR           buffer;
    unsigned int     len = sizeof(NDISUIO_SET_OID) + Length;
    BOOL             success;

    buffer = (PUCHAR) malloc(len);

    if (NULL == buffer) {
        return FALSE;
    }

    memset(buffer, 0, sizeof(NDISUIO_SET_OID));
    Oid = (NDISUIO_SET_OID *)buffer;
    Oid->Oid = OidCode;
    Oid->ptcDeviceName = DevName;

    if (Length > 0) {
        memcpy(Oid->Data,pData,Length);
    }

    // Pass the IOCTL to the device
    success = DeviceIoControl(hAdapter,
                              IOCTL_NDISUIO_SET_OID_VALUE,
                              buffer,
                              len,
                              NULL,
                              0,
                              &dwReturnedBytes,
                              NULL);

    free(buffer);

    return success;
}

void
wmi_get_version ()
{
    DWORD dwReturnedBytes   = 0;
    DWORD dwTargetVersion   = 0;
    DWORD dwHostVersion     = 0;
    BOOL  fRetVal           = FALSE;

    HANDLE hdrgAdapter     = NULL;

    //
    // Get the Target Type from AR6K_SD to check whether the inserted targtet is
    // AR6001 (Dragon) or AR6002 (Mercury)
    //
    hdrgAdapter = CreateFile(TEXT("DRG1:"),GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                    INVALID_HANDLE_VALUE);


    if (INVALID_HANDLE_VALUE != hdrgAdapter)
    {

        fRetVal = DeviceIoControl(
                        hdrgAdapter,
                        IOCTL_CAR6K_GET_FIRMWARE_VERSION,
                        NULL,
                        0,
                        &dwTargetVersion,
                        sizeof (DWORD),
                        &dwReturnedBytes,
                        NULL);

        if (fRetVal)
        {
            TelPrintf ("Target Version %x\n", dwTargetVersion);
        }

        dwReturnedBytes = 0;

        fRetVal = DeviceIoControl(
                        hdrgAdapter,
                        IOCTL_CAR6K_GET_HOST_VERSION,
                        NULL,
                        0,
                        &dwHostVersion,
                        sizeof (DWORD),
                        &dwReturnedBytes,
                        NULL);

        if (fRetVal)
        {
            TelPrintf ("Host driver Version %x\n", dwHostVersion);
        }
    }

    if (hdrgAdapter)
    {
        CloseHandle (hdrgAdapter);
        hdrgAdapter = NULL;
    }

}

void
wmi_dump_chip_mem ()
{
    DWORD dwReturnedBytes   = 0;
    A_UINT32 retData        = 0;
    A_UINT32 diagaddr        =0;
    BOOL  fRetVal           = FALSE;
    int i;

    HANDLE hdrgAdapter     = NULL;

    hdrgAdapter = CreateFile(TEXT("DRG1:"),GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                    INVALID_HANDLE_VALUE);


    if (INVALID_HANDLE_VALUE != hdrgAdapter)
    {
        for(i = 0; i < 5; i++)
        {
            printf("Address range = 0x%x, 0x%x\n",
            mercuryAdd[i][0], mercuryAdd[i][1]);
            for(diagaddr = mercuryAdd[i][0];
                diagaddr < mercuryAdd[i][1];
                diagaddr += 4)
            {
                fRetVal = DeviceIoControl(
                                hdrgAdapter,
                                IOCTL_CAR6K_DUMP_CHIP_MEM,
                                &diagaddr,
                                sizeof (A_UINT32),
                                &retData,
                                sizeof (A_UINT32),
                                &dwReturnedBytes,
                                NULL);

                if (fRetVal)
                {
                    printf("0x%04x:0x%04x\n", diagaddr, retData);
                }
            }
        }

    }

    if (hdrgAdapter)
    {
        CloseHandle (hdrgAdapter);
        hdrgAdapter = NULL;
    }

}

void
wmi_get_bssid (PHANDLE  hAdapter,
               PTCHAR   devName
               )
{
    PUCHAR buffer = NULL;
    PNDISUIO_QUERY_OID bssidOid;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;
    PUCHAR dat;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) )) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) );
    lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG);
    bssidOid = (NDISUIO_QUERY_OID *)buffer;

    //Send OID_802_11_BSSID Oid to the driver
    bssidOid->Oid = OID_802_11_BSSID;
    bssidOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );
    if (bssidOid->ptcDeviceName == NULL) {
        free(buffer);
        return;
    }
    _tcscpy( bssidOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                          &dwReturnedBytes, NULL ) )
    {

       ERR("Error opening device and query");
       free(bssidOid->ptcDeviceName);
       free(buffer);
       return;
    }

    dat=(PUCHAR)bssidOid->Data;
    TelPrintf ("Associated Mac address %02x-%02x-%02x-%02x-%02x-%02x\n",
                dat[0],dat[1],dat[2],dat[3],dat[4],dat[5]);

    if (bssidOid->ptcDeviceName)
    {
         free (bssidOid->ptcDeviceName);
         bssidOid->ptcDeviceName = NULL;
    }


    if (bssidOid)
    {
         free (bssidOid);
         bssidOid = NULL;
    }
}


void
wmi_set_power_mode (
                   PHANDLE  hAdapter,
                   PTCHAR   devName,
                   WMI_POWER_MODE_CMD  *pwrCmd
                   )
{
    PNDISUIO_SET_OID powerModeOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_SET_OID) )) == NULL)  {
        ERR("malloc failed\n");
        return;
    }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
    powerModeOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_set_power_mode()\n");

    // Send OID to the driver
    powerModeOid->Oid = OID_802_11_POWER_MODE;
    powerModeOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (powerModeOid->ptcDeviceName == NULL) {
       free(buffer);
       return;
    }
    _tcscpy(powerModeOid->ptcDeviceName, devName);

    (*(unsigned int *)(powerModeOid->Data)) = pwrCmd->powerMode;

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,    buffer,
                          sizeof(NDISUIO_SET_OID), NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Set Power Mode failed");
    } else {
       TelPrintf("wmi_set_power_mode() successful\n");
    }
    free(powerModeOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_wlan_setting (
                   PHANDLE  hAdapter,
                   PTCHAR   devName,
                   WLAN_SETTING_CMD  *wsCmd
                   )
{
    PNDISUIO_SET_OID wlanSettingOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    NDIS_802_11_SSID *ssid;
    const UCHAR dummySsid[] = "dummySsidToPowerOnTheDevice";

    if (wsCmd->wlanSetting == WLAN_DISABLE) {
        TelPrintf("wmi_set_wlan_disable\n");
        if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_SET_OID) )) == NULL)  {
           ERR("malloc failed\n");
           return;
        }
        memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
        wlanSettingOid = (NDISUIO_SET_OID *)buffer;

        // Send OID to the driver
        wlanSettingOid->Oid = ATH_OID_802_11_DISASSOCIATE;
        wlanSettingOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
        if (wlanSettingOid->ptcDeviceName == NULL) {
           ERR("malloc failed\n");
           goto done;
        }
        _tcscpy(wlanSettingOid->ptcDeviceName, devName);

        (*(unsigned int *)(wlanSettingOid->Data)) = 0;

        // Pass the IOCTL to the device
        if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,    buffer,
                          sizeof(NDISUIO_SET_OID), NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
           ERR("Set wlan setting failed");
        } else {
           TelPrintf("wmi_set_power_disable successful\n");
        }
    } else {
        TelPrintf("wmi_set_wlan_enable\n");
        if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_SET_OID) + sizeof(NDIS_802_11_SSID) )) == NULL)  {
           ERR("malloc failed\n");
           return;
        }
        memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(NDIS_802_11_SSID) );
        wlanSettingOid = (NDISUIO_SET_OID *)buffer;
        // Send OID to the driver
        wlanSettingOid->Oid = ATH_OID_802_11_SSID;
        wlanSettingOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
        if (wlanSettingOid->ptcDeviceName == NULL) {
           ERR("malloc failed\n");
           goto done;
        }
        _tcscpy(wlanSettingOid->ptcDeviceName, devName);

        ssid=(NDIS_802_11_SSID *)wlanSettingOid->Data;
        ssid->SsidLength=sizeof(dummySsid);
        memcpy(ssid->Ssid,dummySsid,sizeof(dummySsid));
        (*(unsigned int *)wlanSettingOid->Data) = ssid->SsidLength;
        memcpy((wlanSettingOid->Data)+sizeof(unsigned int),ssid->Ssid,ssid->SsidLength);

        // Pass the IOCTL to the device
        if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,    buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(NDIS_802_11_SSID), NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
           ERR("Set wlan setting failed");
        } else {
           TelPrintf("wmi_set_power_enable successful\n");
        }
    }
done:
    if (wlanSettingOid->ptcDeviceName)
       free(wlanSettingOid->ptcDeviceName);
    if (buffer)
       free(buffer);
}

void
wmi_set_host_sleep_mode_cmd (
             PHANDLE hAdapter,
             PTCHAR devName,
             WMI_SET_HOST_SLEEP_MODE_CMD *hostSleepModeCmd
             )
{

    PNDISUIO_SET_OID hostSleepModeOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+4)) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+4 );
    hostSleepModeOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_set_host_sleep_mode()\n");

    // Send OID to the driver
    hostSleepModeOid->Oid = OID_CAR6K_SET_HOST_SLEEP_MODE;
    hostSleepModeOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (hostSleepModeOid->ptcDeviceName == NULL) {
       ERR("malloc failed\n");
       free(buffer);
       return;
     }
    _tcscpy(hostSleepModeOid->ptcDeviceName, devName);

    (*(unsigned int *)(hostSleepModeOid->Data)) = hostSleepModeCmd->awake;
    (*(unsigned int *)(hostSleepModeOid->Data+4)) = hostSleepModeCmd->asleep;

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+4, NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Set Host Sleep Mode failed");
    } else {
       TelPrintf("wmi_set_host_sleep_mode() successful\n");
    }
    free(hostSleepModeOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_wow_mode_cmd(
            PHANDLE hAdapter,
            PTCHAR devName,
            WMI_SET_WOW_MODE_CMD *wowModeCmd
            )
{
    PNDISUIO_SET_OID wowModeOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
    wowModeOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_set_wow_mode()\n");

    // Send OID to the driver
    wowModeOid->Oid = OID_CAR6K_SET_WOW_MODE;
    wowModeOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (wowModeOid->ptcDeviceName == NULL) {
       ERR("malloc failed\n");
       free(buffer);
       return;
    }
    _tcscpy(wowModeOid->ptcDeviceName, devName);

    (*(unsigned int *)(wowModeOid->Data)) = wowModeCmd->enable_wow;

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID), NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Set Wow Mode failed");
    } else {
       TelPrintf("wmi_set_wow_mode() successful\n");
    }
    free(wowModeOid->ptcDeviceName);
    free(buffer);
}

void
wmi_add_wow_pattern_cmd(
            PHANDLE hAdapter,
            PTCHAR devName,
            WMI_ADD_WOW_PATTERN_CMD *addWowPatternCmd
            )
{
    PNDISUIO_SET_OID addWowPatternOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+addWowPatternCmd->filter_size*2)) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+addWowPatternCmd->filter_size*2 );
    addWowPatternOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_add_wow_pattern()\n");

    // Send OID to the driver
    addWowPatternOid->Oid = OID_CAR6K_ADD_WOW_PATTERN;
    addWowPatternOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (addWowPatternOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
    }
    _tcscpy(addWowPatternOid->ptcDeviceName, devName);

    (*(addWowPatternOid->Data)) = addWowPatternCmd->filter_list_id;
    (*(addWowPatternOid->Data+1)) = addWowPatternCmd->filter_size;
    (*(addWowPatternOid->Data+2)) = addWowPatternCmd->filter_offset;

    // Copy the Pattern
    memcpy(addWowPatternOid->Data+3, addWowPatternCmd->filter, addWowPatternCmd->filter_size);

    // Copy the Pattern Mask
    memcpy(addWowPatternOid->Data+3+addWowPatternCmd->filter_size, addWowPatternCmd->filter+addWowPatternCmd->filter_size, addWowPatternCmd->filter_size);

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+addWowPatternCmd->filter_size*2,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Add Wow Pattern failed");
    } else {
       TelPrintf("wmi_add_wow_pattern_cmd() successful\n");
    }
    free(addWowPatternOid->ptcDeviceName);
    free(buffer);
}

void
wmi_del_wow_pattern_cmd(
            PHANDLE hAdapter,
            PTCHAR devName,
            WMI_DEL_WOW_PATTERN_CMD *delWowPatternCmd
            )
{
    PNDISUIO_SET_OID delWowPatternOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+4)) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+4 );
    delWowPatternOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_del_wow_pattern()\n");

    // Send OID to the driver
    delWowPatternOid->Oid = OID_CAR6K_DEL_WOW_PATTERN;
    delWowPatternOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (delWowPatternOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
    }
    _tcscpy(delWowPatternOid->ptcDeviceName, devName);

    (*((A_UINT16 *)(delWowPatternOid->Data))) = delWowPatternCmd->filter_list_id;
    (*((A_UINT16 *)(delWowPatternOid->Data+2))) = delWowPatternCmd->filter_id;

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+4, NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Del Wow Pattern failed");
    } else {
       TelPrintf("wmi_del_wow_pattern_cmd() successful\n");
    }
    free(delWowPatternOid->ptcDeviceName);
    free(buffer);
}


void
wmi_get_wow_list_cmd(
            PHANDLE hAdapter,
            PTCHAR devName,
            WMI_GET_WOW_LIST_CMD *getWowListCmd
            )
{
    PNDISUIO_QUERY_OID getWowPatternListOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;
    WMI_GET_WOW_LIST_REPLY *reply=NULL;
    A_UINT8 count,i;


    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_QUERY_OID) + 1024)) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) );
    getWowPatternListOid = (NDISUIO_QUERY_OID *)buffer;

    TelPrintf("wmi_get_wow_list()\n");

    // Send OID to the driver
    getWowPatternListOid->Oid = OID_CAR6K_GET_WOW_LIST;
    getWowPatternListOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if ( getWowPatternListOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
    }
    _tcscpy(getWowPatternListOid->ptcDeviceName, devName);

    (*(getWowPatternListOid->Data)) = getWowListCmd->filter_list_id;

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID), buffer,1024,
                          &dwReturnedBytes, NULL ) ) {

          //  TelPrintf("size of Buffer needed: %d\n",dwReturnedBytes);

        //   free(buffer);

        //   buffer = (PUCHAR) malloc(sizeof(NDISUIO_QUERY_OID)+dwReturnedBytes);
          // memset(buffer,0,sizeof(NDISUIO_QUERY_OID)+dwReturnedBytes);
           //getWowPatternListOid = (NDISUIO_QUERY_OID *)buffer;

           // Send OID to the driver
           //getWowPatternListOid->Oid = OID_CAR6K_GET_WOW_LIST;
           //getWowPatternListOid->ptcDeviceName = (PTCHAR)
           //             malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
           //_tcscpy(getWowPatternListOid->ptcDeviceName, devName);

          // (*(getWowPatternListOid->Data)) = getWowListCmd->filter_list_id;

          /* if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                                   sizeof(NDISUIO_QUERY_OID), buffer,
                                   sizeof(NDISUIO_QUERY_OID)+dwReturnedBytes,
                                   &dwReturnedBytes, NULL) ) {
           */
               ERR("Get Wow Pattern List failed");
           } else {
                TelPrintf("wmi_get_wow_list_cmd() successful\n");

                // Print the current filter list.
                reply = (WMI_GET_WOW_LIST_REPLY *)getWowPatternListOid->Data;
                TelPrintf("No. of installed filters: %d\n",reply->num_filters);
                TelPrintf("Host Sleep Mode: %d\n", reply->host_mode);
                TelPrintf("Wow Mode: %d\n", reply->wow_mode);

                for (count =0 ;count < reply->num_filters ;count++) {

                    TelPrintf("Is Valid filter: %d\n",
                                        reply->wow_filters[count].wow_valid_filter);
                    TelPrintf("filter ID: %d\n",
                                        reply->wow_filters[count].wow_filter_id);
                    TelPrintf("filter size: %d\n",
                                        reply->wow_filters[count].wow_filter_size);
                    TelPrintf("filter offset: %d\n",
                                        reply->wow_filters[count].wow_filter_offset);
                    TelPrintf("Filter Mask: \n");
                    for (i=0; i< reply->wow_filters[count].wow_filter_size; i++) {

                        TelPrintf("%X:", reply->wow_filters[count].wow_filter_mask[i]);
                    }

                    TelPrintf("Filter Pattern: \n");
                    for (i=0; i< reply->wow_filters[count].wow_filter_size; i++) {

                        TelPrintf("%X:", reply->wow_filters[count].wow_filter_pattern[i]);
                    }
               }
           }

   /* } else {
       TelPrintf("wmi_get_wow_list_cmd() successful\n");
    } */

    free( getWowPatternListOid->ptcDeviceName );
    free( buffer);
}

void
wmi_get_txpower (
             PHANDLE  hAdapter,
             PTCHAR   devName
             )
{
    PUCHAR buffer = NULL;
    NDIS_802_11_TX_POWER_LEVEL txPowerLevel;
    PNDISUIO_QUERY_OID txpwrOid;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) )) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) );
    lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG);
    txpwrOid = (NDISUIO_QUERY_OID *)buffer;

    //Send OID_802_11_TX_POWER_LEVEL Oid to the driver
    txpwrOid->Oid = OID_802_11_TX_POWER_LEVEL;
    txpwrOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );
    if (txpwrOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy( txpwrOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Error opening device and query");
       goto done;
    }

    txPowerLevel=(*(NDIS_802_11_TX_POWER_LEVEL *)(txpwrOid->Data));
    TelPrintf ("TX POWER LEVEL is \t %d \n",txPowerLevel);
done:
    free(txpwrOid->ptcDeviceName);
    free(buffer);
}
void
wmi_set_txpower (
                   PHANDLE  hAdapter,
                   PTCHAR   devName,
                   WMI_SET_TX_PWR_CMD  *txpwrCmd
                   )
{
    PNDISUIO_SET_OID txpowerOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_SET_OID) )) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
    txpowerOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    txpowerOid->Oid = OID_CAR6K_SET_TXPOWER;
    txpowerOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (txpowerOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(txpowerOid->ptcDeviceName, devName);

    (*(unsigned int *)(txpowerOid->Data)) = txpwrCmd->dbM;
    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,    buffer,
                          sizeof(NDISUIO_SET_OID), NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Set Tx Power failed");
    } else {
       TelPrintf("wmi_set_txpower() successful\n");
    }
    free(txpowerOid->ptcDeviceName);
    free(buffer);
}

void
wmi_get_stats (
             PHANDLE  hAdapter,
             PTCHAR   devName
             )
{
    PUCHAR buffer;
    PNDISUIO_QUERY_OID bmissOid;
    AR6K_TARGET_STATS *tgtStats;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(AR6K_TARGET_STATS) )) == NULL)  {
        ERR("malloc failed\n");
        return;
     }


    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(AR6K_TARGET_STATS) );
    lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof(AR6K_TARGET_STATS);
    bmissOid = (NDISUIO_QUERY_OID *)buffer;

    bmissOid->Oid = OID_CAR6K_GET_TGT_STATS;
    bmissOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );

    if (bmissOid->ptcDeviceName == NULL) {
       ERR("malloc failed\n");
       free(buffer);
       return;
    }
    _tcscpy( bmissOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID)+sizeof(AR6K_TARGET_STATS), buffer, lenReq,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Error opening device and query");
       goto done;
    }

    tgtStats = (AR6K_TARGET_STATS *)bmissOid->Data;

    printf("Statistics \n");
    printf("------------\n");
    printf("\ntx_packets = %llu\n"
           "tx_bytes = %llu\n"
           "tx_unicast_pkts  = %llu\n"
           "tx_unicast_bytes = %llu\n"
           "tx_multicast_pkts = %llu\n"
           "tx_multicast_bytes = %llu\n"
           "tx_broadcast_pkts = %llu\n"
           "tx_broadcast_bytes = %llu\n"
       "tx_rts_success_cnt = %llu\n",
       tgtStats->tx_packets,
       tgtStats->tx_bytes,
           tgtStats->tx_unicast_pkts,
           tgtStats->tx_unicast_bytes,
           tgtStats->tx_multicast_pkts,
           tgtStats->tx_multicast_bytes,
           tgtStats->tx_broadcast_pkts,
           tgtStats->tx_broadcast_bytes,
       tgtStats->tx_rts_success_cnt);

    printf("\ntx_packet_per_ac[%d] = %llu\n"
           "tx_packet_per_ac[%d] = %llu\n"
           "tx_packet_per_ac[%d] = %llu\n"
           "tx_packet_per_ac[%d] = %llu\n"
           "tx_errors = %llu\n"
           "tx_failed_cnt = %llu\n"
           "tx_retry_cnt = %llu\n"
           "tx_mult_retry_cnt = %llu\n"
           "tx_rts_fail_cnt = %llu\n"
           "tx_unicast_rate = %d Kbps\n",
       0, tgtStats->tx_packet_per_ac[0],
           1, tgtStats->tx_packet_per_ac[1],
           2, tgtStats->tx_packet_per_ac[2],
           3, tgtStats->tx_packet_per_ac[3],
           tgtStats->tx_errors,
           tgtStats->tx_failed_cnt,
           tgtStats->tx_retry_cnt,
           tgtStats->tx_mult_retry_cnt,
           tgtStats->tx_rts_fail_cnt,
           tgtStats->tx_unicast_rate);

    printf("\nrx_packets = %llu\n"
           "rx_bytes = %llu\n"
           "rx_unicast_pkts = %llu\n"
           "rx_unicast_bytes = %llu\n"
           "rx_multicast_pkts = %llu\n"
           "rx_multicast_bytes = %llu\n"
           "rx_broadcast_pkts = %llu\n"
           "rx_broadcast_bytes = %llu\n"
           "rx_fragment_pkt = %llu\n",
       tgtStats->rx_packets,
           tgtStats->rx_bytes,
           tgtStats->rx_unicast_pkts,
           tgtStats->rx_unicast_bytes,
           tgtStats->rx_multicast_pkts,
           tgtStats->rx_multicast_bytes,
           tgtStats->rx_broadcast_pkts,
           tgtStats->rx_broadcast_bytes,
           tgtStats->rx_fragment_pkt);


    printf("\nrx_errors = %llu\n"
           "rx_crcerr = %llu\n"
           "rx_key_cache_miss = %llu\n"
           "rx_decrypt_err = %llu\n"
           "rx_duplicate_frames = %llu\n"
           "rx_unicast_rate = %d Kbps\n",
       tgtStats->rx_errors,
           tgtStats->rx_crcerr,
           tgtStats->rx_key_cache_miss,
           tgtStats->rx_decrypt_err,
           tgtStats->rx_duplicate_frames,
           tgtStats->rx_unicast_rate);

    printf("\ntkip_local_mic_failure = %llu\n"
           "tkip_counter_measures_invoked = %llu\n"
           "tkip_replays = %llu\n"
           "tkip_format_errors = %llu\n"
           "ccmp_format_errors = %llu\n"
           "ccmp_replays = %llu\n"
           "power_save_failure_cnt = %llu\n"
           "noise_floor_calibation = %d\n",
            tgtStats->tkip_local_mic_failure,
           tgtStats->tkip_counter_measures_invoked,
           tgtStats->tkip_replays,
           tgtStats->tkip_format_errors,
           tgtStats->ccmp_format_errors,
           tgtStats->ccmp_replays,
           tgtStats->power_save_failure_cnt,
           tgtStats->noise_floor_calibation);

    printf("\ncs_bmiss_cnt = %llu\n"
           "cs_lowRssi_cnt = %llu\n"
           "cs_connect_cnt = %llu\n"
           "cs_disconnect_cnt = %llu\n"
           "cs_aveBeacon_snr= %d\n"
           "cs_aveBeacon_rssi = %d\n"
           "cs_lastRoam_msec = %d\n"
           "cs_rssi = %d\n"
           "cs_snr = %d\n",
       tgtStats->cs_bmiss_cnt,
           tgtStats->cs_lowRssi_cnt,
           tgtStats->cs_connect_cnt,
           tgtStats->cs_disconnect_cnt,
           tgtStats->cs_aveBeacon_snr,
           tgtStats->cs_aveBeacon_rssi,
           tgtStats->cs_lastRoam_msec,
           tgtStats->cs_rssi,
           tgtStats->cs_snr);

    printf("\nlqVal = %d\n"
           "wow_num_pkts_dropped = %d\n"
           "wow_num_host_pkt_wakeups = %d\n"
           "wow_num_host_event_wakeups = %d\n"
           "wow_num_events_discarded = %d\n",
       tgtStats->lqVal,
           tgtStats->wow_num_pkts_dropped,
           tgtStats->wow_num_host_pkt_wakeups,
           tgtStats->wow_num_host_event_wakeups,
           tgtStats->wow_num_events_discarded);

done:
    free(bmissOid->ptcDeviceName);
    free(buffer);
}

void
wmi_get_rssi (
             PHANDLE  hAdapter,
             PTCHAR   devName
             )
{
    PUCHAR buffer = NULL;
    NDIS_802_11_RSSI rssi,rssiAbs;
    PNDISUIO_QUERY_OID rssiOid;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) )) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) );
    lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG);
    rssiOid = (NDISUIO_QUERY_OID *)buffer;

    //Send OID_802_11_RSSI Oid to the driver
    rssiOid->Oid = OID_802_11_RSSI;
    rssiOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );
    if (rssiOid->ptcDeviceName == NULL) {
       ERR("malloc failed\n");
       free(buffer);
       return;
    }
    _tcscpy( rssiOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Error opening device and query");
       goto done;
    }

    rssi=(*(NDIS_802_11_RSSI *)(rssiOid->Data));
    rssiAbs=RSSI_NDIS2ABS(rssi);
    TelPrintf ("RSSI is \t %d \n",rssiAbs);
done:
    free(rssiOid->ptcDeviceName);
    free(buffer);
}

void
wmi_get_scan_list (
                  PHANDLE  hAdapter,
                  PTCHAR   devName
                  )
{
    PUCHAR buffer = NULL;
    unsigned int lengthReq,length,i;
    UCHAR ssidName[32];
    PNDISUIO_QUERY_OID scanListOid;
    NDIS_WLAN_BSSID_EX *pBSSID;
    NDIS_802_11_BSSID_LIST_EX *List;
    NDIS_802_11_SSID ssid;
    ULONG noOfBSSIDFound;
    ULONG security;
    NDIS_802_11_RSSI rssi1,rssiAbs1;
    DWORD dwReturnedBytes = 0;

    lengthReq = 32768;
    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + lengthReq ))  == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + lengthReq );
    scanListOid = (NDISUIO_QUERY_OID *)buffer;

    //Send OID_802_11_BSSID_LIST Oid to the driver
    scanListOid->Oid = OID_802_11_BSSID_LIST;
    scanListOid->ptcDeviceName = (PTCHAR) malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (scanListOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(scanListOid->ptcDeviceName, devName);

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                           sizeof(NDISUIO_QUERY_OID), buffer, lengthReq,
                           &dwReturnedBytes,NULL ) ) {
       ERR("Error WMI_GET_SCANLIST failed");
       goto done;
    }

    List=(NDIS_802_11_BSSID_LIST_EX *)scanListOid->Data;
    noOfBSSIDFound=List->NumberOfItems;
    pBSSID=(NDIS_WLAN_BSSID_EX *)List->Bssid;
    TelPrintf("\n No of BSSID Found = %u",noOfBSSIDFound);

    while ( noOfBSSIDFound > 0 ) {
       ssid = pBSSID->Ssid;
       length=ssid.SsidLength;
       for ( i=0; i<=length; i++ )
      ssidName[i]=ssid.Ssid[i];
           TelPrintf ("\n SSID is %s \t",ssidName);
           security=(ULONG)pBSSID->Privacy;
           if ( security )
              TelPrintf ("\t Network is Secured");
           else
              TelPrintf ("\t Network is Open");

           rssi1=(NDIS_802_11_RSSI)pBSSID->Rssi;
           rssiAbs1=RSSI_NDIS2ABS(rssi1);
           TelPrintf ("\t RSSI value is %d ",rssiAbs1);
           noOfBSSIDFound--;
           pBSSID=(NDIS_WLAN_BSSID_EX *)(((unsigned int)pBSSID)+(pBSSID->Length));
    } // while
done:
    free( scanListOid->ptcDeviceName);
    free(buffer);
}

static int
tchar_to_hex(const TCHAR *orig, A_UINT8 *hex)
{
   const TCHAR *bufp;
   int i;
   A_BOOL validPattern = TRUE;

   i = 0;
   for (bufp = orig; *bufp != '\0'; ++bufp) {
      unsigned int val;
      TCHAR c = *bufp++;

      if (_istdigit(c)) val = c - __T('0');
      else if (c >= __T('a') && c <= __T('f')) val = c - __T('a') + 10;
      else if (c >= __T('A') && c <= __T('F')) val = c - __T('A') + 10;
      else { validPattern = FALSE; break; }

      val <<= 4;
      c = *bufp;
      if (_istdigit(c)) val |= c - __T('0');
      else if (c >= __T('a') && c <= __T('f')) val |= c - __T('a') + 10;
      else if (c >= __T('A') && c <= __T('F')) val |= c - __T('A') + 10;
      else { validPattern = FALSE; break; }

      hex[i++] = (unsigned char) (val & 0377);

      /* if (*bufp != '\0' && *bufp != ':') {
         validPattern = FALSE;
         break;
      } */
   }

    if (validPattern == TRUE) {
        return 0;
    } else {
        return(-1);
    }
}

void
wmi_create_qos_cmd (
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_CREATE_PSTREAM_CMD *crePStreamCmd
             )
{
    PNDISUIO_SET_OID crePStreamOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;
    PUCHAR pdat = NULL;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_CREATE_PSTREAM_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID) + sizeof(WMI_CREATE_PSTREAM_CMD));
    crePStreamOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_create_qos_cmd()\n");

    // Send OID to the driver
    crePStreamOid->Oid = OID_CAR6K_CREATE_QOS;
    crePStreamOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (crePStreamOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(crePStreamOid->ptcDeviceName, devName);

     pdat = (PUCHAR)crePStreamOid->Data;
    // Copy the Pattern
     memcpy(pdat, crePStreamCmd, sizeof (WMI_CREATE_PSTREAM_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID) + sizeof(WMI_CREATE_PSTREAM_CMD),
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Create QOS Failed");
    } else {
       TelPrintf("Create qos successful\n");
    }
    free(crePStreamOid->ptcDeviceName);
    free(buffer);
}

void
wmi_delete_qos_cmd (
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_DELETE_PSTREAM_CMD *delPStreamCmd
             )
{
    PNDISUIO_SET_OID delPStreamOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+ sizeof(WMI_DELETE_PSTREAM_CMD)))  == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID) + sizeof(WMI_DELETE_PSTREAM_CMD));
    delPStreamOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_delete_qos_cmd()\n");

    // Send OID to the driver
    delPStreamOid->Oid = OID_CAR6K_DELETE_QOS;
    delPStreamOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (delPStreamOid->ptcDeviceName == NULL ) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(delPStreamOid->ptcDeviceName, devName);

    pdat = (PUCHAR)delPStreamOid->Data;
    // Copy the Pattern
    memcpy(pdat, delPStreamCmd, sizeof (WMI_DELETE_PSTREAM_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID) + sizeof(WMI_DELETE_PSTREAM_CMD),
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Delete QOS Failed");
    } else {
       TelPrintf("Delete qos successful\n");
    }
    free(delPStreamOid->ptcDeviceName);
    free(buffer);
}



void
wmi_set_wmm_cmd (
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_SET_WMM_CMD        *setWmmCmd
             )
{
    PNDISUIO_SET_OID setWMMOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ( (buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)))  == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
    setWMMOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_set_wmm_cmd()\n");

    // Send OID to the driver
    setWMMOid->Oid = OID_CAR6K_SET_WMM;
    setWMMOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if ( setWMMOid->ptcDeviceName== NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(setWMMOid->ptcDeviceName, devName);

    // Copy the Pattern
    memcpy(setWMMOid->Data, setWmmCmd, sizeof (WMI_SET_WMM_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set WMM Failed");
    } else {
       TelPrintf("Set WMM OID successful\n");
    }
    free(setWMMOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_txop_cmd (
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_SET_WMM_TXOP_CMD   *pTxOp
             )
{
    PNDISUIO_SET_OID setTxOpOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ( (buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID));
    setTxOpOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_set_txop_cmd()\n");

    // Send OID to the driver
    setTxOpOid->Oid = OID_CAR6K_SET_TXOP;
    setTxOpOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (setTxOpOid->ptcDeviceName ==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(setTxOpOid->ptcDeviceName, devName);

    // Copy the Pattern
    memcpy(setTxOpOid->Data, pTxOp, sizeof (WMI_SET_WMM_TXOP_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set TXOP Failed");
    } else {
       TelPrintf("Set TXOP OID successful\n");
    }
    free(setTxOpOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_btstatus_cmd (
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_SET_BT_STATUS_CMD  *pBTStatus
             )
{
    PNDISUIO_SET_OID setBtStatusOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ( (buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID));
    setBtStatusOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_btstatus_cmd()\n");

    // Send OID to the driver
    setBtStatusOid->Oid = OID_CAR6K_SET_BT_STATUS;
    setBtStatusOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (setBtStatusOid->ptcDeviceName ==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(setBtStatusOid->ptcDeviceName, devName);

    // Copy the Pattern
    memcpy(setBtStatusOid->Data, pBTStatus, sizeof (WMI_SET_BT_STATUS_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set BT Status Failed");
    } else {
       TelPrintf("Set BTStatus OID successful\n");
    }
    free(setBtStatusOid->ptcDeviceName);
    free(buffer);
}


void
wmi_set_btparams_cmd (
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_SET_BT_PARAMS_CMD  *pBtParmCmd
             )
{
    PNDISUIO_SET_OID setBtParamsOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ( (buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_SET_BT_PARAMS_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(WMI_SET_BT_PARAMS_CMD));
    setBtParamsOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_btparams_cmd()\n");

    // Send OID to the driver
    setBtParamsOid->Oid = OID_CAR6K_SET_BT_PARAMS;
    setBtParamsOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (setBtParamsOid->ptcDeviceName==NULL ) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(setBtParamsOid->ptcDeviceName, devName);

     pdat = (PUCHAR)setBtParamsOid->Data;
    // Copy the Pattern
     memcpy(pdat, pBtParmCmd, sizeof (WMI_SET_BT_PARAMS_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(WMI_SET_BT_PARAMS_CMD) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set BT Params Failed");
    } else {
       TelPrintf("Set BTParams OID successful\n");
    }
    free(setBtParamsOid->ptcDeviceName);
    free(buffer);
}

void
wmi_get_qos_queue_cmd (
             PHANDLE  hAdapter,
             PTCHAR   devName,
             struct ar6000_queuereq *getQosQueueCmd
             )
{
    PNDISUIO_QUERY_OID qosQueueOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;
    int i;

    if ( (buffer = (PUCHAR) malloc(sizeof(NDISUIO_QUERY_OID)+sizeof (struct ar6000_queuereq)))  == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID)+sizeof (struct ar6000_queuereq) );
    qosQueueOid = (NDISUIO_QUERY_OID *)buffer;

    TelPrintf("wmi_get_qos_queue_cmd()\n");

    // Send OID to the driver
    qosQueueOid->Oid = OID_CAR6K_GET_QOS_QUEUE;
    qosQueueOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (qosQueueOid->ptcDeviceName==NULL){
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(qosQueueOid->ptcDeviceName, devName);

    // Copy the Pattern
    memcpy(qosQueueOid->Data, getQosQueueCmd, sizeof (struct ar6000_queuereq));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID)+sizeof (struct ar6000_queuereq) ,
                          buffer, sizeof(NDISUIO_QUERY_OID)+sizeof (struct ar6000_queuereq), &dwReturnedBytes, NULL ) )
    {
       ERR("Query QOS Failed");
    } else {
       TelPrintf("Query QOS successful\n");
       getQosQueueCmd = (struct ar6000_queuereq *)qosQueueOid->Data;
       for (i=0; i<=15; i++) {
     if ( getQosQueueCmd->activeTsids & (1<<i)) {
        TelPrintf("TsID: %d\n",i);
    }
       }
    }
    free(qosQueueOid->ptcDeviceName);
    free(buffer);
}

void wmi_dbglog_cfg_module_cmd (
            PHANDLE hAdapter,
            PTCHAR devName,
            WMIX_DBGLOG_CFG_MODULE_CMD *dbgLogCfgModuleCmd
            )
{
    PNDISUIO_SET_OID dbgLogCfgModuleOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ( (buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+4))  == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+4 );
    dbgLogCfgModuleOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_dbglog_cfg_module()\n");

    // Send OID to the driver
    dbgLogCfgModuleOid->Oid = OID_CAR6K_DBGLOG_CFG_MODULE;
    dbgLogCfgModuleOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (dbgLogCfgModuleOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(dbgLogCfgModuleOid->ptcDeviceName, devName);

    ((*(A_UINT32 *)(dbgLogCfgModuleOid->Data))) = dbgLogCfgModuleCmd->config.cfgvalid;
    ((*(A_UINT32 *)(dbgLogCfgModuleOid->Data+4))) = dbgLogCfgModuleCmd->config.cfgvalue;

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+4, NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Dbg Log Module Config cmd failed");
    } else {
       TelPrintf("Dbg Log Module Config cmd successful\n");
    }
    free(dbgLogCfgModuleOid->ptcDeviceName);
    free(buffer);
}

void wmi_dbglog_get_debug_logs(
                PHANDLE hAdapter,
                PTCHAR  devName
                )
{
    PUCHAR buffer = NULL;
    PNDISUIO_QUERY_OID dbgLogGetDebugLogsOid;
    unsigned int lenReq=0;
    DWORD dwReturnedBytes = 0;

    if ( (buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID));
    lenReq = sizeof(NDISUIO_QUERY_OID);
    dbgLogGetDebugLogsOid = (NDISUIO_QUERY_OID *)buffer;

    //Send OID_CAR6K_DBGLOG_GET_DEBUG_LOGS Oid to the driver
    dbgLogGetDebugLogsOid->Oid = OID_CAR6K_DBGLOG_GET_DEBUG_LOGS;
    dbgLogGetDebugLogsOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );
    if (dbgLogGetDebugLogsOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy( dbgLogGetDebugLogsOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Error opening device and query");
    }
    free(dbgLogGetDebugLogsOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_scan_params_cmd (
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_SCAN_PARAMS_CMD    *pScanCmd
             )
{
    PNDISUIO_SET_OID scanParamsOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ( pScanCmd->maxact_chdwell_time) {
    if ( pScanCmd->maxact_chdwell_time < 5) {
        TelPrintf("Max active channel dwell time should be between 5-65535 msecs \n");
    return;
    }
    if (pScanCmd->minact_chdwell_time &&
       (pScanCmd->maxact_chdwell_time < pScanCmd->minact_chdwell_time)) {
        TelPrintf("Max active channel dwell time should be greater that minimun \n");
    }
    }

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_SCAN_PARAMS_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(WMI_SCAN_PARAMS_CMD));
    scanParamsOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_set_scan_params_cmd()\n");

    // Send OID to the driver
    scanParamsOid->Oid = OID_CAR6K_SET_SCAN_PARAMS;
    scanParamsOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (scanParamsOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(scanParamsOid->ptcDeviceName, devName);
     pdat = (PUCHAR)scanParamsOid->Data;
     memcpy(pdat, pScanCmd, sizeof (WMI_SCAN_PARAMS_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(WMI_SCAN_PARAMS_CMD) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set SCAN Params Failed");
    } else {
       TelPrintf("Set SCAN OID successful\n");
    }
    free(scanParamsOid->ptcDeviceName);
    free(buffer);
}

void wmi_set_roam_cmd (
    PHANDLE hAdapter,
    PTCHAR devName,
    WMI_SET_ROAM_CTRL_CMD *roamdata
    )
{
     PNDISUIO_SET_OID roam;
     PUCHAR buffer = NULL;
     PUCHAR pdat = NULL;
     DWORD dwReturnedBytes = 0;
     int size = sizeof(NDISUIO_SET_OID)+sizeof(WMI_SET_ROAM_CTRL_CMD);

     if ( (buffer = (PUCHAR) malloc(size)) == NULL ) {
        ERR("malloc failed\n");
        return;
     }
     memset( buffer, 0, size );
     roam = (NDISUIO_SET_OID *)buffer;

     // Send OID to the driver
     roam->Oid = OID_CAR6K_SET_ROAM_CTRL;
     roam->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
     if (roam->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
     _tcscpy(roam->ptcDeviceName, devName);

     pdat = (PUCHAR)roam->Data;
     memcpy(pdat, roamdata, sizeof(WMI_SET_ROAM_CTRL_CMD));

     // Pass the IOCTL to the device
     if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                           size, NULL, 0, &dwReturnedBytes, NULL ) ) {
        ERR("wmi_set_roam_cmd cmd failed");
     }
     free(roam->ptcDeviceName);
     free(buffer);
 }

 static int
 wmic_ether_aton(const TCHAR *orig, A_UINT8 *eth)
 {
    const TCHAR *bufp;
    int i;

    i = 0;
    for (bufp = orig; *bufp != '\0'; ++bufp) {
       unsigned int val;
       TCHAR c = *bufp++;

       if (_istdigit(c)) val = c - __T('0');
       else if (c >= __T('a') && c <= __T('f')) val = c - __T('a') + 10;
       else if (c >= __T('A') && c <= __T('F')) val = c - __T('A') + 10;
       else break;

       val <<= 4;
       c = *bufp++;
       if (_istdigit(c)) val |= c - __T('0');
       else if (c >= __T('a') && c <= __T('f')) val |= c - __T('a') + 10;
       else if (c >= __T('A') && c <= __T('F')) val |= c - __T('A') + 10;
       else break;

       eth[i] = (unsigned char) (val & 0377);
       if (++i == ATH_MAC_LEN) {
          return 0;
       }
       if (*bufp != ':')
          break;
    }
    return(-1);
 }

void
wmi_set_ibss_channel (
                   PHANDLE  hAdapter,
                   PTCHAR   devName,
                   IBSS_CHANNEL_CMD *ibssChannelCmd
                   )
{
    PNDISUIO_SET_OID ibssChannelOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ( (buffer = (PUCHAR) malloc( sizeof(NDISUIO_SET_OID) )) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
    ibssChannelOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    ibssChannelOid->Oid = OID_CAR6K_IBSS_CHANNEL;
    ibssChannelOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
     if (ibssChannelOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(ibssChannelOid->ptcDeviceName, devName);

    (*(unsigned int *)(ibssChannelOid->Data)) = ibssChannelCmd->ibssChannel;

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,    buffer,
                          sizeof(NDISUIO_SET_OID), NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Set IBSS Channel failed");
    } else {
       TelPrintf("wmi_set_ibss_channel() successful\n");
    }
    free(ibssChannelOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_channel (
                 PHANDLE  hAdapter,
                 PTCHAR   devName,
                 WMI_CHANNEL_PARAMS_CMD *chParamCmd
                )
{
    PNDISUIO_SET_OID ChannelOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;
    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(WMI_CHANNEL_PARAMS_CMD) + chParamCmd->numChannels * sizeof(A_UINT16);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

    memset( buffer, 0, len);
    ChannelOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    ChannelOid->Oid = OID_CAR6K_SET_CHANNEL_PARAMS;
    ChannelOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
     if (ChannelOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(ChannelOid->ptcDeviceName, devName);
    memcpy(ChannelOid->Data,chParamCmd,sizeof(WMI_CHANNEL_PARAMS_CMD)+ chParamCmd->numChannels * sizeof(A_UINT16));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set wireless mode and Channel failed");
    } else {
       TelPrintf("wmi_set_channel() successful\n");
    }
    free(ChannelOid->ptcDeviceName);
    free(buffer);
}
void
wmi_set_atim (
                 PHANDLE  hAdapter,
                 PTCHAR   devName,
                 BYTE atimWindow
                )
{
    PNDISUIO_SET_OID AtimOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;
    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(BYTE);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

    memset( buffer, 0, len);
    AtimOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    AtimOid->Oid = OID_CAR6K_SET_ATIM;
    AtimOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
     if (AtimOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(AtimOid->ptcDeviceName, devName);

    memcpy(AtimOid->Data, &atimWindow, sizeof(BYTE));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) )
                          {
       ERR("Set atim window failed");
    } else {
       TelPrintf("Set atim window successful\n");
    }
    free(AtimOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_ibsspm (
               PHANDLE  hAdapter,
               PTCHAR   devName,
           WMI_IBSS_PM_CAPS_CMD *adhocPmCmd
               )
{
    PNDISUIO_SET_OID Oid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;
    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(WMI_IBSS_PM_CAPS_CMD);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
       ERR("malloc failed");
       return;
    }

    memset( buffer, 0, len);
    Oid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    Oid->Oid = OID_CAR6K_SET_IBSSPM;
    Oid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
     if (Oid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(Oid->ptcDeviceName, devName);
    pdat = (PUCHAR)Oid->Data;
    memcpy(pdat,adhocPmCmd,sizeof(WMI_IBSS_PM_CAPS_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set ibss power mode failed");
    } else {
       TelPrintf("wmi_set_ibsspm() successful\n");
    }
    free(Oid->ptcDeviceName);
    free(buffer);
}


static void
DumpHtcState (PHANDLE  hAdapter,
              PTCHAR   devName)
{
    if (!IssueSetOid(hAdapter,
                     devName,
                     OID_CAR6K_DUMP_HTC,
                     NULL,
                     0)) {
        ERR("Failed to issue OID_CAR6K_DUMP_HTC");
    }

}


void
wmi_set_pmparams (
               PHANDLE  hAdapter,
               PTCHAR   devName,
           WMI_POWER_PARAMS_CMD *pm
               )
{
    PNDISUIO_SET_OID Oid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;
    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(WMI_POWER_PARAMS_CMD);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
        ERR("malloc failed");
        return;
    }

    memset( buffer, 0, len);
    Oid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    Oid->Oid = OID_CAR6K_SET_PM;
    Oid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
     if (Oid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(Oid->ptcDeviceName, devName);
    pdat = (PUCHAR)Oid->Data;
    memcpy(pdat,pm,sizeof(WMI_POWER_PARAMS_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set power parameters failed");
    } else {
       TelPrintf("wmi_set_pmparams() successful\n");
    }
    free(Oid->ptcDeviceName);
    free(buffer);
}

void
wmi_abort_scan (
                PHANDLE  hAdapter,
                PTCHAR   devName
               )
{
    PNDISUIO_SET_OID abortscanOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_SET_OID) )) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
    abortscanOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    abortscanOid->Oid = OID_CAR6K_ABORT_SCAN;
    abortscanOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (abortscanOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(abortscanOid->ptcDeviceName, devName);

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,    buffer,
                          sizeof(NDISUIO_SET_OID), NULL, 0,
                          &dwReturnedBytes, NULL ) ) {
       ERR("AbortScan failed");
    } else {
       TelPrintf("wmi_abort_scan() successful\n");
    }
    free(abortscanOid->ptcDeviceName);
    free(buffer);
}

void
wmi_get_bitrate (
             PHANDLE  hAdapter,
             PTCHAR   devName
             )
{
    PUCHAR buffer = NULL;
    ulong tx_bitrate = 0;
    PNDISUIO_QUERY_OID txOid;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;
    CAR6K_GET_BITRATE *pBitRate = NULL;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(UINT) )) == NULL)  {
        ERR("malloc failed\n");
        return;
    }

    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(UINT) );
    lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof(UINT);
    txOid = (NDISUIO_QUERY_OID *)buffer;

    // Send OID_CAR6K_GET_TX_BITRATE Oid to the driver
    txOid->Oid = OID_CAR6K_GET_BITRATE;
    txOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );
    if (txOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy( txOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Error opening device and query");
       goto done;
    }

    pBitRate = (CAR6K_GET_BITRATE *) txOid->Data;

    if (pBitRate)
    {
        TelPrintf ("Tx Bit Rate == > %d\n", pBitRate->ul_txRate);
        TelPrintf ("Rx Bit Rate == > %d\n", pBitRate->ul_rxRate);
    }

done:
    free(txOid->ptcDeviceName);
    free(buffer);
}

void
wmi_get_wmode (
             PHANDLE  hAdapter,
             PTCHAR   devName
             )
{
    PUCHAR buffer = NULL;
    PNDISUIO_QUERY_OID wmodeOid;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;
    CAR6K_GET_WMODE *pMode = NULL;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(UINT) )) == NULL)  {
        ERR("malloc failed\n");
        return;
    }

    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(UINT) );
    lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof(UINT);
    wmodeOid = (NDISUIO_QUERY_OID *)buffer;

    // Send OID_CAR6K_GET_WMODE Oid to the driver
    wmodeOid->Oid = OID_CAR6K_GET_WMODE;
    wmodeOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );
    if (wmodeOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy( wmodeOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Error opening device and query");
       goto done;
    }

    pMode = (CAR6K_GET_WMODE *) wmodeOid->Data;

    if (pMode)
    {
        switch(pMode->wmode)
        {
            case WMI_11A_MODE:
            TelPrintf ("11A ONLY MODE \n");
            break;
            case WMI_11G_MODE:
            TelPrintf ("11BG MODE \n");
            break;
            case WMI_11AG_MODE:
            TelPrintf ("11ABG MODE \n");
            break;
            case WMI_11B_MODE:
            TelPrintf ("11B ONLY MODE\n");
            break;
            case WMI_11GONLY_MODE:
            TelPrintf ("11G ONLY MODE\n");
            break;
            default:
            TelPrintf ("UNKNOWN MODE\n");

        }
    }

done:
    free(wmodeOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_fix_rates (
                 PHANDLE  hAdapter,
                 PTCHAR   devName,
                 WMI_FIX_RATES_CMD *setFixRatesCmd
                )
{
    PNDISUIO_SET_OID FixRatesOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;
    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(WMI_FIX_RATES_CMD);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

    memset( buffer, 0, len);
    FixRatesOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    FixRatesOid->Oid = OID_CAR6K_SET_FIX_RATES;
    FixRatesOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
     if (FixRatesOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(FixRatesOid->ptcDeviceName, devName);
    memcpy(FixRatesOid->Data,&(setFixRatesCmd->fixRateMask),sizeof(WMI_FIX_RATES_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set wireless mode and Channel failed");
    } else {
       TelPrintf("wmi_set_fix_rates() successful\n");
    }
    free(FixRatesOid->ptcDeviceName);
    free(buffer);
}

void
wmi_get_reg_domain (
             PHANDLE  hAdapter,
             PTCHAR   devName
             )
{
    PUCHAR buffer = NULL;
    UINT regdomaincode;
    PNDISUIO_QUERY_OID rdOid;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(UINT) )) == NULL)  {
        ERR("malloc failed\n");
        return;
     }
    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(UINT) );
    lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof(UINT);
    rdOid = (NDISUIO_QUERY_OID *)buffer;

    //Send OID_802_11_TX_POWER_LEVEL Oid to the driver
    rdOid->Oid = OID_CAR6K_GET_RD;
    rdOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );
    if (rdOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy( rdOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                          sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                          &dwReturnedBytes, NULL ) ) {
       ERR("Error opening device and query");
       goto done;
    }

#define REGCODE_IS_CC_BITSET(x)   ((x) & 0x80000000)
#define REGCODE_GET_CODE(x)   ((x) & 0xFFF)
#define REGCODE_IS_WWR_BITSET(x)   ((x) & 0x40000000)

    regdomaincode=(*(UINT *)(rdOid->Data));
    if (REGCODE_IS_CC_BITSET(regdomaincode))
        TelPrintf ("Country Code = ");
    else if (REGCODE_IS_WWR_BITSET(regdomaincode))
        TelPrintf ("WWR Roaming code = ");
    else
        TelPrintf ("Regulatory Domain = ");

    TelPrintf ("0x%x\n", REGCODE_GET_CODE(regdomaincode));
done:
    free(rdOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_rssi_threshold(
             PHANDLE  hAdapter,
             PTCHAR   devName,
             USER_RSSI_PARAMS *rssiThresholdParam
             )
{
    PNDISUIO_SET_OID rssiThresholdOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(USER_RSSI_PARAMS))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(USER_RSSI_PARAMS));
    rssiThresholdOid = (NDISUIO_SET_OID *)buffer;

    TelPrintf("wmi_set_rssi_threshold()\n");

    // Send OID to the driver
    rssiThresholdOid->Oid = OID_CAR6K_RSSI_THRESHOLD;
    rssiThresholdOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (rssiThresholdOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(rssiThresholdOid->ptcDeviceName, devName);
     pdat = (PUCHAR)rssiThresholdOid->Data;
     memcpy(pdat, rssiThresholdParam, sizeof (USER_RSSI_PARAMS));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(USER_RSSI_PARAMS) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("set rssi threshold");
    } else {
       TelPrintf("set rssi threshold success\n");
    }
    free(rssiThresholdOid->ptcDeviceName);
    free(buffer);
}
void
wmi_get_current_mode(
                    PHANDLE  hAdapter,
                    PTCHAR   devName
                    )
{
    PUCHAR buffer = NULL;
    UINT currentmode;
    PNDISUIO_QUERY_OID modeOid;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(UINT) )) == NULL)  {
        ERR("malloc failed\n");
        return;
    }

    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(UINT) );
    lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof(UINT);
    modeOid = (NDISUIO_QUERY_OID *)buffer;

    //Send OID_CAR6K_GET_CURRENT_MODE Oid to the driver
    modeOid->Oid = OID_CAR6K_GET_CURRENT_MODE;
    modeOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );

    if (modeOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
    }
    _tcscpy( modeOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                &dwReturnedBytes, NULL ) ) {
        ERR("Error opening device and query");
        goto done;
    }

    currentmode = (*(UINT *)(modeOid->Data));

    if ( currentmode == 0x10)
        TelPrintf ("Soft AP mode \n");
    else if ( currentmode == 0x01)
        TelPrintf ("station mode - Infrastructure \n");
    else if ( currentmode == 0x02)
        TelPrintf ("station mode - ADHOC \n");
    else if ( currentmode == 0x04)
        TelPrintf ("station mode - ADHOC Creator \n");
    else
        TelPrintf ("Unknown mode \n");
done:
    free(modeOid->ptcDeviceName);
    free(buffer);
}

void
wmi_get_sta(
            PHANDLE  hAdapter,
            PTCHAR   devName
            )
{
    PUCHAR buffer = NULL;
    ap_get_sta *staList;
    PNDISUIO_QUERY_OID staOid;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;
    int i;
    int flag = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(ap_get_sta) )) == NULL)  {
            ERR("malloc failed\n");
            return;
     }

     memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof( ap_get_sta) );
     lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof(ap_get_sta);
     staOid = (NDISUIO_QUERY_OID *)buffer;

     //Send OID_CAR6K_GET_STA Oid to the driver
     staOid->Oid = OID_CAR6K_GET_STA;
     staOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );

     if (staOid->ptcDeviceName == NULL) {
         ERR("malloc failed\n");
         free(buffer);
         return;
      }
     _tcscpy( staOid->ptcDeviceName, devName );

     // Pass the IOCTL to the device
     if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                           sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                           &dwReturnedBytes, NULL ) ) {
        ERR("Error opening device and query");
        goto done;
    }

    staList = (ap_get_sta *)(staOid->Data);

    for(i=0;i<AP_MAX_NUM_STA;i++) {
        if(!is_mac_null(staList->sta[i].mac)) {
            printf(" %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X %5d\n",
                    staList->sta[i].mac[0], staList->sta[i].mac[1],
                    staList->sta[i].mac[2], staList->sta[i].mac[3],
                    staList->sta[i].mac[4], staList->sta[i].mac[5],
                    staList->sta[i].aid);
            flag = 1;
        }
    }

    if(!flag) {
        printf("          Empty             \n");
    }
    printf("-----------------------------\n");
done:
    free(staOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_listen_interval(
	                   PHANDLE hAdapter,
	                   PTCHAR devName,
	                   WMI_LISTEN_INT_CMD *listenCmd
	                   )
{
    PNDISUIO_SET_OID ListenIntvlOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;
    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(WMI_LISTEN_INT_CMD);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

    memset( buffer, 0, len);
    ListenIntvlOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    ListenIntvlOid->Oid = OID_CAR6K_LISTEN_INTVL;
    ListenIntvlOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
     if (ListenIntvlOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(ListenIntvlOid->ptcDeviceName, devName);
    memcpy(ListenIntvlOid->Data,&(listenCmd->listenInterval),sizeof(WMI_LISTEN_INT_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set listen interval failed");
    } else {
       TelPrintf("wmi_set_listen_interval() successful\n");
    }
    free(ListenIntvlOid->ptcDeviceName);
    free(buffer);
}

void
wmi_adddel_acl (
               PHANDLE  hAdapter,
               PTCHAR   devName,
               WMI_AP_ACL_MAC_CMD *pACL
               )
{
    PNDISUIO_SET_OID aclOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;
    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(WMI_AP_ACL_MAC_CMD);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
        ERR("malloc failed");
        return;
    }

    memset( buffer, 0, len);
    aclOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    aclOid->Oid = OID_CAR6K_ADD_DEL_ACL;
    aclOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
     if (aclOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(aclOid->ptcDeviceName, devName);
    pdat = (PUCHAR)aclOid->Data;
    memcpy(pdat,pACL,sizeof(WMI_AP_ACL_MAC_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Add del ACL parameters failed");
    } else {
       TelPrintf("wmi_adddel_acl() successful\n");
    }
    free(aclOid->ptcDeviceName);
    free(buffer);
}

void
wmi_get_acl(
           PHANDLE  hAdapter,
           PTCHAR   devName
           )

{
    PUCHAR buffer = NULL;
    WMI_AP_ACL * pGetAcl;
    PNDISUIO_QUERY_OID GetAclOid;
    unsigned int lenReq;
    DWORD dwReturnedBytes = 0;
    int flag = 0;
    int i;
    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof( WMI_AP_ACL) )) == NULL)  {
        ERR("malloc failed\n");
        return;
    }

    memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof( WMI_AP_ACL) );
    lenReq = sizeof(NDISUIO_QUERY_OID) + sizeof( WMI_AP_ACL);
    GetAclOid = (NDISUIO_QUERY_OID *)buffer;

    //Send OID_CAR6K_GET_ACL Oid to the driver
    GetAclOid->Oid = OID_CAR6K_GET_ACL;
    GetAclOid->ptcDeviceName = (PTCHAR) malloc( _tcslen(devName) * sizeof(TCHAR) + 2 );

    if (GetAclOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
    }
    _tcscpy( GetAclOid->ptcDeviceName, devName );

    // Pass the IOCTL to the device
    if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                sizeof(NDISUIO_QUERY_OID), buffer, lenReq,
                &dwReturnedBytes, NULL ) ) {
        ERR("Error opening device and query");
        goto done;
    }

    pGetAcl = ( WMI_AP_ACL *)(GetAclOid->Data);

    printf("------------------------------------------------- \n");
    printf(" ACL Policy - %d\n", pGetAcl->policy & ~AP_ACL_RETAIN_LIST_MASK);
    printf("-------------------------------------------------\n");
    printf(" Index       MAC \n");
    for(i=0;i<AP_ACL_SIZE;i++) {
        if(!is_mac_null(pGetAcl->acl_mac[i]) || pGetAcl->wildcard[i])
        {
            printf("   %d ",i);
            print_wild_mac(pGetAcl->acl_mac[i], pGetAcl->wildcard[i]);
            flag = 1;
        }
    }
    if(!flag) {
        printf("       Empty         \n ");
    }
    printf("-------------------------------------------------\n");
done:
    free(GetAclOid->ptcDeviceName);
    free(buffer);
}

void
wmi_ap_acl_policy (
                  PHANDLE  hAdapter,
                  PTCHAR   devName,
                  WMI_AP_ACL_POLICY_CMD *pACLpolicy
                  )
{
    PNDISUIO_SET_OID aclPolicyOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc (sizeof (NDISUIO_SET_OID) + sizeof (WMI_AP_ACL_POLICY_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
    }

    memset(buffer, 0, sizeof (NDISUIO_SET_OID) + sizeof (WMI_AP_ACL_POLICY_CMD));
    aclPolicyOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    aclPolicyOid->Oid = OID_CAR6K_SET_ACL_POLICY;
    aclPolicyOid->ptcDeviceName = (PTCHAR)
        malloc (_tcslen(devName) * sizeof(TCHAR) + 2);

    if (aclPolicyOid->ptcDeviceName==NULL) {
        ERR ("malloc failed\n");
        free (buffer);
        return;
    }
    _tcscpy (aclPolicyOid->ptcDeviceName, devName);
    pdat = (PUCHAR)aclPolicyOid->Data;
    memcpy (pdat, pACLpolicy, sizeof (WMI_AP_ACL_POLICY_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl (hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                sizeof (NDISUIO_SET_OID) + sizeof (WMI_AP_ACL_POLICY_CMD) ,
                NULL, 0, &dwReturnedBytes, NULL)) {
        ERR ("set ACL policy in  AP mode failed");
    } else {
        TelPrintf ("set ACL policy in  AP mode success\n");
    }
    free (aclPolicyOid->ptcDeviceName);
    free (buffer);
}

void
wmi_set_ap_inact_time(
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_AP_CONN_INACT_CMD *pInact
             )
{
    PNDISUIO_SET_OID coninactOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_CONN_INACT_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
    }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_CONN_INACT_CMD));
    coninactOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    coninactOid->Oid = OID_CAR6K_SET_CONN_INACT_TIME;
    coninactOid->ptcDeviceName = (PTCHAR)
        malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

    if (coninactOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
    }
    _tcscpy(coninactOid->ptcDeviceName, devName);
    pdat = (PUCHAR)coninactOid->Data;
    memcpy(pdat, pInact, sizeof (WMI_AP_CONN_INACT_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_CONN_INACT_CMD) ,
                NULL, 0, &dwReturnedBytes, NULL ) ) {
        ERR("set connection inactivity time in AP mode failed");
    } else {
        TelPrintf("set connection inactivity time in AP mode success\n");
    }
    free(coninactOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_bg_protection(
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_AP_PROT_SCAN_TIME_CMD *pProt
             )
{
    PNDISUIO_SET_OID bgprotOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_PROT_SCAN_TIME_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_PROT_SCAN_TIME_CMD));
    bgprotOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    bgprotOid->Oid = OID_CAR6K_SET_BG_PROTECTION;
    bgprotOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (bgprotOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(bgprotOid->ptcDeviceName, devName);
     pdat = (PUCHAR)bgprotOid->Data;
     memcpy(pdat, pProt, sizeof (WMI_AP_PROT_SCAN_TIME_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_PROT_SCAN_TIME_CMD) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("set protection scan time in AP mode failed");
    } else {
       TelPrintf("set protection scan time in AP mode success\n");
    }
    free(bgprotOid->ptcDeviceName);
    free(buffer);
}


void
wmi_set_hidden_ssid(
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_AP_HIDDEN_SSID_CMD *pHidden
             )
{
    PNDISUIO_SET_OID hiddenOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_HIDDEN_SSID_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_HIDDEN_SSID_CMD));
    hiddenOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    hiddenOid->Oid = OID_CAR6K_SET_HIDDEN_SSID;
    hiddenOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
    if (hiddenOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(hiddenOid->ptcDeviceName, devName);
     pdat = (PUCHAR)hiddenOid->Data;
     memcpy(pdat, pHidden, sizeof (WMI_AP_HIDDEN_SSID_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_HIDDEN_SSID_CMD) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set/Reset hidden SSID in AP mode failed");
    } else {
       TelPrintf("Set/Reset hidden SSID in AP mode success\n");
    }
    free(hiddenOid->ptcDeviceName);
    free(buffer);
}


void
wmi_set_dtim(
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_AP_SET_DTIM_CMD *pDtim
             )
{
    PNDISUIO_SET_OID dtimOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_SET_DTIM_CMD ))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_SET_DTIM_CMD ));
    dtimOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    dtimOid->Oid = OID_CAR6K_SET_DTIM;
    dtimOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

    if (dtimOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(dtimOid->ptcDeviceName, devName);
     pdat = (PUCHAR)dtimOid->Data;
     memcpy(pdat, pDtim, sizeof (WMI_AP_SET_DTIM_CMD ));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_SET_DTIM_CMD ) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("DTIM period set in AP mode failed");
    } else {
       TelPrintf("DTIM period set in AP mode success\n");
    }
    free(dtimOid->ptcDeviceName);
    free(buffer);
}


void
 wmi_set_country(
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_SET_COUNTRY_CMD *pCountry
             )
{
    PNDISUIO_SET_OID countryOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_SET_COUNTRY_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(WMI_SET_COUNTRY_CMD));
    countryOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    countryOid->Oid = OID_CAR6K_SET_RD;
    countryOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

    if (countryOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(countryOid->ptcDeviceName, devName);
     pdat = (PUCHAR)countryOid->Data;
     memcpy(pdat, pCountry, sizeof (WMI_SET_COUNTRY_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(WMI_SET_COUNTRY_CMD) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("set country code failed");
    } else {
       TelPrintf("set country code success\n");
    }
    free(countryOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_beacon_int(
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_BEACON_INT_CMD *bconIntvl
             )
{
    PNDISUIO_SET_OID bconIntvlOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_BEACON_INT_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
     }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(WMI_BEACON_INT_CMD));
    bconIntvlOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    bconIntvlOid->Oid = OID_CAR6K_SET_BCON_INTVL;
    bconIntvlOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

    if (bconIntvlOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(bconIntvlOid->ptcDeviceName, devName);
     pdat = (PUCHAR)bconIntvlOid->Data;
     memcpy(pdat, bconIntvl, sizeof (WMI_BEACON_INT_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(WMI_BEACON_INT_CMD) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("set beacon interval in AP/ADHOC mode failed");
    } else {
       TelPrintf("set beacon interval in AP/ADHOC mode success\n");
    }
    free(bconIntvlOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_apmode (
                 PHANDLE  hAdapter,
                 PTCHAR   devName,
                 WMI_CONNECT_CMD *apCmd
               )

{
    PNDISUIO_SET_OID apmodeOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(WMI_CONNECT_CMD);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

    memset( buffer, 0, len);
    apmodeOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    apmodeOid->Oid = OID_CAR6K_SET_APMODE;
    apmodeOid->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

     if (apmodeOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(apmodeOid->ptcDeviceName, devName);
   memcpy(apmodeOid->Data,apCmd,sizeof(WMI_CONNECT_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR("Set AP mode failed");
    } else {
       TelPrintf("wmi_set_apmode() successful\n");
    }

    free(apmodeOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_stamode (
                 PHANDLE  hAdapter,
                 PTCHAR   devName
               )

{
    PNDISUIO_SET_OID stamodeOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    unsigned int len = sizeof(NDISUIO_SET_OID);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

    memset( buffer, 0, len);
    stamodeOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    stamodeOid->Oid = OID_CAR6K_SET_STAMODE;
    stamodeOid->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

     if (stamodeOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(stamodeOid->ptcDeviceName, devName);

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) ) {
      ERR("Set STA mode failed");
    } else {
       TelPrintf("wmi_set_stamode() successful\n");
    }

    free(stamodeOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_ap_security(
             PHANDLE  hAdapter,
             PTCHAR   devName,
             AP_MODE_CRYPT *apsec
             )
{
    PNDISUIO_SET_OID apsecOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(AP_MODE_CRYPT) + apsec->KeyLength + 1;

    if ((buffer = (PUCHAR) malloc(len)) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

    memset( buffer, 0, len);
    apsecOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    apsecOid->Oid = OID_CAR6K_SET_SECURITY;
    apsecOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

    if (apsecOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(apsecOid->ptcDeviceName, devName);
     pdat = (PUCHAR)apsecOid->Data;
     memcpy(pdat, apsec, sizeof (AP_MODE_CRYPT) + apsec->KeyLength + 1);

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) )
    {
       ERR("Setting security to AP mode failed");
    }else {
       TelPrintf("set security in AP mode success\n");
    }
    free(apsecOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_ap_intrabss (
                    PHANDLE  hAdapter,
                    PTCHAR   devName,
                    AP_SET_INTRA_BSS *pIntra
                    )
{
    PNDISUIO_SET_OID intrabssOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(AP_SET_INTRA_BSS);

    if ((buffer = (PUCHAR) malloc(len)) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

    memset( buffer, 0, len);
    intrabssOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    intrabssOid->Oid = OID_CAR6K_SET_INTRABSS;
    intrabssOid->ptcDeviceName = (PTCHAR)
               malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

    if (intrabssOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy(intrabssOid->ptcDeviceName, devName);
     pdat = (PUCHAR)intrabssOid->Data;
     memcpy(pdat, pIntra, sizeof (AP_SET_INTRA_BSS));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                          len, NULL, 0, &dwReturnedBytes, NULL ) )
    {
       ERR("Enable/Disable intra BSS traffic in AP mode Failure");
    }else {
       TelPrintf("Enable/Disable intra BSS traffic in AP mode success\n");
    }
    free(intrabssOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_ip (
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_SET_IP_CMD *pIP
             )
{
    PNDISUIO_SET_OID ipOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc (sizeof (NDISUIO_SET_OID) + sizeof (WMI_SET_IP_CMD))) == NULL)  {
        ERR ("malloc failed\n");
        return;
     }

    memset (buffer, 0, sizeof (NDISUIO_SET_OID) + sizeof (WMI_SET_IP_CMD));
    ipOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    ipOid->Oid = OID_CAR6K_SET_IP;
    ipOid->ptcDeviceName = (PTCHAR)
                           malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

    if (ipOid->ptcDeviceName == NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
     }
    _tcscpy (ipOid->ptcDeviceName, devName);
     pdat = (PUCHAR)ipOid->Data;
     memcpy (pdat, pIP, sizeof (WMI_SET_IP_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl (hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                          sizeof(NDISUIO_SET_OID)+sizeof(WMI_SET_IP_CMD) ,
                          NULL, 0, &dwReturnedBytes, NULL ) ) {
       ERR ("set IP Address failed");
    } else {
       TelPrintf ("set IP Address success\n");
    }
    free (ipOid->ptcDeviceName);
    free (buffer);
}

void
wmi_ap_remove_sta(
             PHANDLE  hAdapter,
             PTCHAR   devName,
             WMI_AP_SET_MLME_CMD *pMlme
             )
{
    PNDISUIO_SET_OID remstaOid;
    PUCHAR buffer = NULL;
    PUCHAR pdat = NULL;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc(sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_SET_MLME_CMD))) == NULL)  {
        ERR("malloc failed\n");
        return;
    }

    memset( buffer, 0, sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_SET_MLME_CMD));
    remstaOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    remstaOid->Oid = OID_CAR6K_REMOVE_STA;
    remstaOid->ptcDeviceName = (PTCHAR)
        malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

    if (remstaOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
    }
    _tcscpy(remstaOid->ptcDeviceName, devName);
    pdat = (PUCHAR)remstaOid->Data;
    memcpy(pdat, pMlme, sizeof (WMI_AP_SET_MLME_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                sizeof(NDISUIO_SET_OID)+sizeof(WMI_AP_SET_MLME_CMD) ,
                NULL, 0, &dwReturnedBytes, NULL ) ) {
        ERR("remove station in AP mode failed");
    } else {
        TelPrintf("remove station in AP mode  success\n");
    }
    free(remstaOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_numsta(
                  PHANDLE  hAdapter,
                  PTCHAR   devName,
                  WMI_AP_SET_NUM_STA_CMD *numofsta
               )

{
    PNDISUIO_SET_OID numofstaOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    unsigned int len = sizeof(NDISUIO_SET_OID) + sizeof(WMI_AP_SET_NUM_STA_CMD);

    if ( (buffer = (PUCHAR) malloc(len)) == NULL ) {
        ERR("malloc failed\n");
        return;
    }

    memset( buffer, 0, len);
    numofstaOid = (NDISUIO_SET_OID *)buffer;

    // Send OID to the driver
    numofstaOid->Oid = OID_CAR6K_SET_NUM_STA;
    numofstaOid->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + 2);

    if (numofstaOid->ptcDeviceName==NULL) {
        ERR("malloc failed\n");
        free(buffer);
        return;
    }
    _tcscpy(numofstaOid->ptcDeviceName, devName);
    memcpy(numofstaOid->Data,numofsta,sizeof(WMI_AP_SET_NUM_STA_CMD));

    // Pass the IOCTL to the device
    if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,
                len, NULL, 0, &dwReturnedBytes, NULL ) ) {
        ERR("Set num of stations in AP mode failed");
    } else {
        TelPrintf("wmi_set_numsta() successful\n");
    }

    free(numofstaOid->ptcDeviceName);
    free(buffer);
}

void
wmi_set_nodeage_cmd (
                   PHANDLE  hAdapter,
                   PTCHAR   devName,
                   A_UINT32 nodeage
                   )
{
    PNDISUIO_SET_OID nodeageOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;

    if ( (buffer = (PUCHAR) malloc( sizeof(NDISUIO_SET_OID) )) == NULL ) {
       ERR("malloc failed\n");
       return;
    }

	memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
	nodeageOid = (NDISUIO_SET_OID *)buffer;

	// Send OID to the driver
	nodeageOid->Oid = OID_CAR6K_SET_NODEAGE;
	nodeageOid->ptcDeviceName = (PTCHAR)
			   malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
	 if (nodeageOid->ptcDeviceName==NULL) {
		ERR("malloc failed\n");
		free(buffer);
		return;
	 }
	_tcscpy(nodeageOid->ptcDeviceName, devName);

	(*(A_UINT32 *)(nodeageOid->Data)) = nodeage;

	// Pass the IOCTL to the device
	if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
						  sizeof(NDISUIO_SET_OID), NULL, 0,
						  &dwReturnedBytes, NULL ) ) {
	   ERR("Set Node Age failed");
	} else {
	   TelPrintf("wmi_set_nodeage() successful\n");
	}
	free(nodeageOid->ptcDeviceName);
	free(buffer);
}

void
wmi_get_nodeage_cmd (
				   PHANDLE	hAdapter,
				   PTCHAR	devName
				   )
{
	PNDISUIO_QUERY_OID nodeageOid;
	PUCHAR buffer = NULL;
	DWORD dwReturnedBytes = 0;

	if ( (buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) )) == NULL ) {
	   ERR("malloc failed\n");
	   return;
	}

	memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) );
	nodeageOid = (NDISUIO_QUERY_OID *)buffer;

	// Send OID to the driver
	nodeageOid->Oid = OID_CAR6K_GET_NODEAGE;
	nodeageOid->ptcDeviceName = (PTCHAR)
			   malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
	 if (nodeageOid->ptcDeviceName==NULL) {
		ERR("malloc failed\n");
		free(buffer);
		return;
	 }
	_tcscpy(nodeageOid->ptcDeviceName, devName);

	// Pass the IOCTL to the device
	if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
						  sizeof(NDISUIO_QUERY_OID), buffer,
						  sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG),
						  &dwReturnedBytes, NULL ) ) {
	   ERR("Get Node Age failed");
	} else {
	   TelPrintf("nodeage is %d\n", *(A_UINT32 *)(nodeageOid->Data));
	}

	free(nodeageOid->ptcDeviceName);
	free(buffer);
}

void
wmi_set_keepalive (
                   PHANDLE  hAdapter,
                   PTCHAR   devName,
                   A_UINT8  keepAliveIntv
                   )
{
    PNDISUIO_SET_OID keepAliveOid;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;
    do
    {
        if ( (buffer = (PUCHAR) malloc( sizeof(NDISUIO_SET_OID) )) == NULL ) {
            ERR("malloc failed\n");
            break;
        }

        memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
        keepAliveOid = (NDISUIO_SET_OID *)buffer;

        // Send OID to the driver
        keepAliveOid->Oid = OID_CAR6K_SET_KEEPALIVE_INTV;
        keepAliveOid->ptcDeviceName = (PTCHAR) malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
        if (keepAliveOid->ptcDeviceName==NULL)
        {
            ERR("malloc failed\n");
            break;
        }
        _tcscpy(keepAliveOid->ptcDeviceName, devName);
        (*(A_UINT8 *)(keepAliveOid->Data)) = keepAliveIntv;

        // Pass the IOCTL to the device
        if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, buffer,
                                sizeof(NDISUIO_SET_OID), NULL, 0,
                                &dwReturnedBytes, NULL ) ) {
            ERR("Set Keep Alive failed");
        }
        else
        {
            TelPrintf("wmi_set_keepalive() successful\n");
        }
    }while(FALSE);
    if(buffer)
    {
    	if(keepAliveOid->ptcDeviceName)
    	{
        	free(keepAliveOid->ptcDeviceName);
    	}
        free(buffer);
    }
}


void
wmi_get_keepalive (
                   PHANDLE hAdapter,
                   PTCHAR  devName
                  )
{
    PNDISUIO_QUERY_OID keepAliveOid;
    WMI_GET_KEEPALIVE_CMD *getKeepAlive;
    PUCHAR buffer = NULL;
    DWORD dwReturnedBytes = 0;
    do
    {
        if ( (buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) )) == NULL )
        {
            ERR("malloc failed\n");
            break;
        }

        memset( buffer, 0, sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG) );
        keepAliveOid = (NDISUIO_QUERY_OID *)buffer;

        // Send OID to the driver
        keepAliveOid->Oid = OID_CAR6K_GET_KEEPALIVE_INTV;
        keepAliveOid->ptcDeviceName = (PTCHAR) malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
        if (keepAliveOid->ptcDeviceName==NULL) {
            ERR("malloc failed\n");
            break;
        }
        _tcscpy(keepAliveOid->ptcDeviceName, devName);

        // Pass the IOCTL to the device
        if (!DeviceIoControl( hAdapter, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
                                sizeof(NDISUIO_QUERY_OID), buffer,
                                sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG),
                                &dwReturnedBytes, NULL ) ) {
            ERR("Get Keep Alive failed");
        }
        else
        {
            getKeepAlive = (WMI_GET_KEEPALIVE_CMD * )keepAliveOid->Data;
            TelPrintf("Keepalive interval is %d secs \r\n ",getKeepAlive->keepaliveInterval);
            TelPrintf("Configuration status : %s\r\n",(getKeepAlive->configured ? "configured" : "not configured"));
        }
    }while(FALSE);

    if(buffer)
    {
        if(keepAliveOid->ptcDeviceName)
        {
            free(keepAliveOid->ptcDeviceName);
        }
        free(buffer);
    }
}


A_STATUS wmi_set_cmd(PHANDLE    hAdapter,
                    PTCHAR      devName,
                    UCHAR       *data,
                    A_UINT32    dataLen,
                    NDIS_OID    Oid,
                    DWORD       *dwReturnedBytes)
{
    PNDISUIO_SET_OID    genOid;
    PUCHAR              buffer  = NULL;
    unsigned int        len =  sizeof(NDISUIO_SET_OID) + dataLen;
    A_STATUS            status = A_OK;
    do
    {
        if((buffer = (PUCHAR)malloc(len)) == NULL)
        {
            ERR("malloc failed\n");
            status = A_ERROR;
            break;
        }
        memset(buffer,0,len);
        genOid = (NDISUIO_SET_OID *)buffer;

        genOid->Oid = Oid;
        genOid->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + 2);
        if(genOid->ptcDeviceName == NULL)
        {
            ERR("malloc failed\n");
            status = A_ERROR;
            break;
        }
        _tcscpy(genOid->ptcDeviceName, devName);
        memcpy(genOid->Data,data,dataLen);

        if (!DeviceIoControl(hAdapter, IOCTL_NDISUIO_SET_OID_VALUE,buffer,len, NULL, 0, dwReturnedBytes, NULL ) )
        {
            ERR("Set command failed");
            status = A_ERROR;
            break;
        }
        else
        {
            TelPrintf("Set command successful\n");
        }
    }while(FALSE);

    if(genOid)
    {
    	if(genOid->ptcDeviceName)
    	{
        	free(genOid->ptcDeviceName);
    	}
        free(genOid);
    }

    return status;

}


void wmi_set_power_state(
                        PHANDLE hAdapter,
                        PTCHAR devName,
                        NDIS_DEVICE_POWER_STATE *pwrState
                        )
{
    DWORD dwReturnedBytes = 0;
    if(wmi_set_cmd(hAdapter,devName,(UCHAR *)pwrState,sizeof(NDIS_DEVICE_POWER_STATE),OID_PNP_SET_POWER,&dwReturnedBytes) != A_OK)
    {
        TelPrintf("Set Power state Failed\n");
    }
    else
    {
        TelPrintf("Set Power state successful\n");
    }

}

void RebindAdapter(TCHAR *pszAdapterName)
{
    TCHAR   szAdapterName[50]   = {0,};
    DWORD   dwReturnedBytes;
    HANDLE  NDSHandler          = INVALID_HANDLE_VALUE;
    do
    {
        _stprintf(szAdapterName, _T("%s\0\0"), pszAdapterName);
        TelPrintf("Opening NDS0 to rebind \n");

        NDSHandler  = CreateFile (L"NDS0:", 0xF0000000, 3, NULL, 3, 0, NULL);

        if (INVALID_HANDLE_VALUE == NDSHandler)
        {
            ERR ("Error opening NDS0 for rebind");
            break;
        }

        if (! DeviceIoControl ( NDSHandler, IOCTL_NDIS_REBIND_ADAPTER, szAdapterName,
                    (_tcslen (szAdapterName) * sizeof (TCHAR)), NULL, 0, &dwReturnedBytes, NULL))
        {
            ERR ("Rebind ioctl failed");
            CloseHandle(NDSHandler);
            break;
        }

        TelPrintf("Rebind success \n");
        CloseHandle(NDSHandler);
    }while(FALSE);
    return;
}


void wmi_set_ip_address(PHANDLE hAdapter,PTCHAR devName,A_BOOL dhcpEnable,IP_ADDRESS *ipAddress)
{
    HKEY KeyHandler;
    LONG regStatus  = ERROR_SUCCESS;
    A_STATUS status = A_OK;
    DWORD pData = dhcpEnable;
    TCHAR szTcpIpInterfaceKey[MAX_PATH] = {0,};
    TCHAR adapterName[MAX_PATH] = {0,};

    do
    {
        _stprintf (szTcpIpInterfaceKey, _T ("comm\\%s\\parms\\Tcpip"), devName);
        regStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTcpIpInterfaceKey, 0, 0 ,&KeyHandler);
        if(regStatus != ERROR_SUCCESS)
        {
            ERR("Not able to Open Registry\n");
            status = A_ERROR;
            break;
        }
        if(dhcpEnable == FALSE)
        {

            regStatus = RegSetValueEx(KeyHandler,Ar6kIPKey,0,REG_SZ,ipAddress->ipAddr,sizeof(ipAddress->ipAddr));
            if(regStatus != ERROR_SUCCESS)
            {
                ERR("Set IP Failed\n");
                status = A_ERROR;
                break;
            }
            regStatus = RegSetValueEx(KeyHandler,Ar6kSubMaskKey,0,REG_SZ,ipAddress->subnetMsk,sizeof(ipAddress->subnetMsk));
            if(regStatus != ERROR_SUCCESS)
            {
                ERR("Set Mask Failed\n");
                status = A_ERROR;
                break;
            }
        }
        regStatus = RegSetValueEx(KeyHandler,Ar6kEnableDHCP,0,REG_DWORD,(const BYTE *)&pData,sizeof (DWORD));
        if(regStatus != ERROR_SUCCESS)
        {
            ERR("Set DHCP Failed\n");
            status = A_ERROR;
            break;
        }
    }while(FALSE);
    CloseHandle (KeyHandler);
    _stprintf (adapterName, _T ("%s"),devName);

    if(status == A_OK)
    {
        RebindAdapter(adapterName);
    }
}
int
_tmain (
       int argc,
       _TCHAR *argv[]
       )
{
    int i, cmd=0, index;
    char buf[256];
    HANDLE hAdapter;
    TCHAR  devName[128];
    DWORD dwReturnedBytes = 0;
    int arg = 0;
    BYTE atimWindow = 0;
    UINT k;
    BOOL match=FALSE;
    char a[3];
    char *temp = a;
    char temp1[16];
    A_UINT8 policy, retain;

    WMI_POWER_MODE_CMD  *pwrCmd = (WMI_POWER_MODE_CMD *)buf;
    WMI_SET_TX_PWR_CMD  *txpwrCmd = (WMI_SET_TX_PWR_CMD *)buf;
    WLAN_SETTING_CMD *wsCmd = (WLAN_SETTING_CMD *)buf;
    WMI_SET_HOST_SLEEP_MODE_CMD *hostSleepModeCmd =
                            (WMI_SET_HOST_SLEEP_MODE_CMD *)buf;

    WMI_SET_WOW_MODE_CMD    *wowModeCmd       = (WMI_SET_WOW_MODE_CMD *)buf;
    WMI_GET_WOW_LIST_CMD    *getWowListCmd    = (WMI_GET_WOW_LIST_CMD *)buf;
    WMI_ADD_WOW_PATTERN_CMD *addWowPatternCmd = (WMI_ADD_WOW_PATTERN_CMD *)buf;
    WMI_DEL_WOW_PATTERN_CMD *delWowPatternCmd = (WMI_DEL_WOW_PATTERN_CMD *)buf;
    WMI_CREATE_PSTREAM_CMD  *crePStreamCmd    = (WMI_CREATE_PSTREAM_CMD *)buf;
    WMI_DELETE_PSTREAM_CMD  *delPStreamCmd    = (WMI_DELETE_PSTREAM_CMD *)buf;
    struct ar6000_queuereq  *getQosQueueCmd   = (struct ar6000_queuereq *)buf;
    WMI_SET_WMM_CMD         *setWmmCmd        = (WMI_SET_WMM_CMD *)(buf);
    WMI_SET_WMM_TXOP_CMD    *pTxOp            = (WMI_SET_WMM_TXOP_CMD *)(buf);
    WMIX_DBGLOG_CFG_MODULE_CMD *dbgLogCfgModuleCmd = (WMIX_DBGLOG_CFG_MODULE_CMD *)buf;
    WMI_SET_BT_PARAMS_CMD *pBtParmCmd    = (WMI_SET_BT_PARAMS_CMD *) (buf);
    WMI_SET_BT_STATUS_CMD *pBtStatCmd    = (WMI_SET_BT_STATUS_CMD *) (buf);
    WMI_SCAN_PARAMS_CMD *sParamCmd       = (WMI_SCAN_PARAMS_CMD *) (buf);
    WMI_SET_ROAM_CTRL_CMD   *rmctl  = (WMI_SET_ROAM_CTRL_CMD *) (buf);
    IBSS_CHANNEL_CMD *ibssChannelCmd = (IBSS_CHANNEL_CMD *)buf;
    WMI_CHANNEL_PARAMS_CMD *chParamCmd = (WMI_CHANNEL_PARAMS_CMD*)buf;
    WMI_IBSS_PM_CAPS_CMD *adhocPmCmd   = (WMI_IBSS_PM_CAPS_CMD *)buf;
    WMI_POWER_PARAMS_CMD *pmParamCmd = (WMI_POWER_PARAMS_CMD*)buf;
    USER_RSSI_PARAMS *rssiThresholdParam = (USER_RSSI_PARAMS *)buf;
    WMI_CONNECT_CMD *apCmd = (WMI_CONNECT_CMD *)buf;
    NDIS_802_11_WEP *apkey = (NDIS_802_11_WEP*)buf;
    WMI_AP_SET_NUM_STA_CMD *numofsta = (WMI_AP_SET_NUM_STA_CMD*)buf;
    WMI_AP_ACL_MAC_CMD *pACL = (WMI_AP_ACL_MAC_CMD *)buf;
    A_INT16 threshold[26];
    AP_COMMIT_XTND *xtnapCmd = (AP_COMMIT_XTND *)buf;
    WMI_AP_CONN_INACT_CMD *pInact = (WMI_AP_CONN_INACT_CMD *)buf;
    WMI_AP_SET_MLME_CMD *pMlme = (WMI_AP_SET_MLME_CMD *)buf;
    WMI_AP_PROT_SCAN_TIME_CMD *pProt = (WMI_AP_PROT_SCAN_TIME_CMD *)buf;
    WMI_AP_HIDDEN_SSID_CMD *pHidden = (WMI_AP_HIDDEN_SSID_CMD *)buf;
    WMI_AP_SET_DTIM_CMD *pDtim = (WMI_AP_SET_DTIM_CMD *)buf;
    WMI_SET_COUNTRY_CMD *pCountry = (WMI_SET_COUNTRY_CMD *)(buf + sizeof(int));
    WMI_BEACON_INT_CMD *bconIntvl     = (WMI_BEACON_INT_CMD *)buf;
    WMI_SET_IP_CMD *pIP = (WMI_SET_IP_CMD*)buf;
    AP_MODE_CRYPT *apsec = (AP_MODE_CRYPT *)buf;
    WMI_AP_ACL_POLICY_CMD *pACLpolicy = (WMI_AP_ACL_POLICY_CMD *)buf;
    AP_SET_INTRA_BSS *pIntra = (AP_SET_INTRA_BSS *)buf;
    WMI_FIX_RATES_CMD *setFixRatesCmd  = (WMI_FIX_RATES_CMD *)(buf);
    WMI_LISTEN_INT_CMD *listenCmd      = (WMI_LISTEN_INT_CMD*)buf;
    A_UINT32 nodeage;
    A_UINT8  keepAliveIntv;
    NDIS_DEVICE_POWER_STATE devPower;
    IP_ADDRESS ipAddress;
    A_BOOL  dhcpEnable = TRUE;

    memset( buf, 0, sizeof(buf) );
    memset( devName, 0, sizeof(devName));

    arg = argc;
    for ( i=0; i < argc; ++i ) {

        if ( _tcscmp( argv[i], OPT_I_INTERFACE ) == 0 ) {
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            _tcsncpy( devName, argv[i], 127);
            devName[127]=__T('\0');
        }

        if ( _tcscmp( argv[i], OPT_P_POWERMODE ) == 0 ) {

            TelPrintf("Power Mode:");
            cmd = WMI_SET_POWER_MODE;
            /* ndis defined values for power mode are different from
            * wmi defined power mode values. This command will be executed through
            * and NDIS OID, so, map the power mode values accordingly */
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            if ( _tcscmp( argv[i], PM_REC ) == 0 ) {
                pwrCmd->powerMode = Ndis802_11PowerModeFast_PSP;
                break;
            } else if ( _tcscmp( argv[i], PM_MAXPERF ) == 0 ) {
                pwrCmd->powerMode = Ndis802_11PowerModeCAM;
                break;
            } else {
                PrintUsage();
                return 0;
            }
        }

        if ( _tcscmp( argv[i], OPT_R_TXPWR ) == 0 ) {
            cmd = WMI_GET_TXPOWER;
            break;
        }

        if ( _tcscmp( argv[i], OPT_SET_TXPWR ) == 0 ) {
            cmd = WMI_SET_TXPOWER;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            txpwrCmd->dbM = (A_UINT8)_ttoi(argv[i]);
            break;
        }


        if ( _tcscmp( argv[i], OPT_R_RSSI ) == 0 ) {
            cmd = WMI_GET_RSSI;
            break;
        }

        if ( _tcscmp( argv[i], OPT_R_BSSID ) == 0 ) {
            cmd = WMI_GET_BSSID;
            break;
        }

        if ( _tcscmp( argv[i], OPT_S_SCANLIST ) == 0 ) {
            cmd = WMI_GET_SCANLIST;
            break;
        }

        if ( _tcscmp( argv[i], OPT_WLAN_SETTING ) == 0 ) {

            cmd = WMI_SET_WLAN_SETTING;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            if ( _tcscmp( argv[i], WS_ENABLE ) == 0 ) {
                wsCmd->wlanSetting = WLAN_ENABLE;
                TelPrintf("wlan enable\n");
                break;
            } else if ( _tcscmp( argv[i], WS_DISABLE ) == 0 ) {
                wsCmd->wlanSetting = WLAN_DISABLE;
                TelPrintf("wlan disable\n");
                break;
            } else {
                PrintUsage();
                return 0;
            }
        }


        if ( _tcscmp( argv[i], OPT_HOST_MODE ) == 0 ) {

            TelPrintf("Host Mode Setting: \n");
            cmd = WMI_SET_HOST_SLEEP_MODE;
            i++;

            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            if (_tcscmp(argv[i], WOW_HOST_AWAKE) == 0) {

                hostSleepModeCmd->awake = TRUE;
                hostSleepModeCmd->asleep = FALSE;
                TelPrintf("Host Sleep Mode awake\n");
                break;
            } else if ( _tcscmp(argv[i], WOW_HOST_ASLEEP) == 0 ) {

                hostSleepModeCmd->awake = FALSE;
                hostSleepModeCmd->asleep = TRUE;
                TelPrintf("Host Sleep Mode asleep\n");
                break;
            } else {
                PrintUsage();
                return 0;
            }
        } //OPT_HOST_MODE

        if ( _tcscmp( argv[i], OPT_WOW_MODE ) == 0 ) {

            TelPrintf("Wow Mode Setting: \n");
            cmd = WMI_SET_WOW_MODE;
            i++;

            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            if (_tcscmp(argv[i], WOW_ENABLE) == 0) {

                wowModeCmd->enable_wow = TRUE;
                TelPrintf("Wow Mode enable\n");
                break;
            } else if ( _tcscmp(argv[i], WOW_DISABLE) == 0 ) {

                wowModeCmd->enable_wow = FALSE;
                TelPrintf("Wow Mode disable\n");
                break;
            } else {
                PrintUsage();
                return 0;
            }
        } //OPT_WOW_MODE

        if ( _tcscmp(argv[i], OPT_GET_WOW_LIST) == 0 ) {

            TelPrintf("Get Wow List for id: \n");
            cmd = WMI_GET_WOW_LIST;
            i++;

            /* if (!_ttoi(argv[i])) {
                TelPrintf("Filter List id must be a non-zero number\n");
                PrintUsage();
                return 0;
            } */
            if (!argv[i]) {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }

            getWowListCmd->filter_list_id = (A_UINT8)_ttoi(argv[i]);
            TelPrintf("%d\n",getWowListCmd->filter_list_id);
            break;
        } // OPT_GET_WOW_LIST

        if ( _tcscmp(argv[i], OPT_ADD_WOW_PATTERN) == 0 ) {

            A_UINT8 pattern[64];
            A_UINT8 mask[64];

            TelPrintf("Add Wow Pattern: \n");
            cmd = WMI_ADD_WOW_PATTERN;
            if((i + 4) >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            i++;

            /* if (!_ttoi(argv[i])) {


            } */

            addWowPatternCmd->filter_list_id = (A_UINT8)_ttoi(argv[i]);
            i++;

            addWowPatternCmd->filter_size = (A_UINT8)_ttoi(argv[i]);
            i++;

            addWowPatternCmd->filter_offset = (A_UINT8)_ttoi(argv[i]);
            i++;

            if ( tchar_to_hex(argv[i], pattern) == -1 ) {
                TelPrintf("Invalid pattern format\n");
                PrintUsage();
                return 0;
            }

            // Copy the Pattern
            memcpy(addWowPatternCmd->filter, pattern, addWowPatternCmd->filter_size);

            i++;

            // Copy the Pattern Mask
            if ( tchar_to_hex(argv[i], mask) == -1 ) {
                TelPrintf("Invalid mask format\n");
                PrintUsage();
                return 0;
            }

            memcpy(addWowPatternCmd->filter+addWowPatternCmd->filter_size, mask,
                    addWowPatternCmd->filter_size);

            break;

        } //OPT_ADD_WOW_PATTERN

        if ( _tcscmp(argv[i], OPT_DEL_WOW_PATTERN) == 0 ) {

            TelPrintf("Delete WoW Pattern: \n");
            cmd = WMI_DEL_WOW_PATTERN;
            i++;

            if((i+1) >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            delWowPatternCmd->filter_list_id = (A_UINT16)_ttoi(argv[i++]);
            delWowPatternCmd->filter_id = (A_UINT16)_ttoi(argv[i]);

            break;
        } //OPT_DEL_WOW_PATTERN


        if ( _tcscmp(argv[i], OPT_CREATE_QOS) == 0 ) {

            cmd = WMI_CREATE_QOS;

            if (arg <= 7)
            {
                TelPrintf ("Please enter userpriority, traffic direction, traffic class, traffic type\n");
                PrintUsage();
                return 0;
            }
            i++;
            crePStreamCmd->userPriority     = (A_UINT8)_ttoi(argv[i++]);

            crePStreamCmd->trafficDirection = (A_UINT8)_ttoi(argv[i++]);


            crePStreamCmd->trafficClass     = (A_UINT8)_ttoi(argv[i++]);


            crePStreamCmd->trafficType      = (A_UINT8)_ttoi(argv[i++]);
            crePStreamCmd->voicePSCapability = (i >= arg)?0:(A_UINT8)_ttoi(argv[i++]);


            crePStreamCmd->minServiceInt    = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->maxServiceInt    = (i >= arg)?0:_ttoi(argv[i++]);

            crePStreamCmd->inactivityInt    = (i >= arg)?0:_ttoi(argv[i++]);

            crePStreamCmd->suspensionInt    = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->serviceStartTime = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->tsid             = (i >= arg)?0:(A_UINT8)_ttoi(argv[i++]);
            crePStreamCmd->nominalMSDU      = (i >= arg)?0:(A_UINT16)_ttoi(argv[i++]);
            crePStreamCmd->maxMSDU          = (i >= arg)?0:(A_UINT16)_ttoi(argv[i++]);
            crePStreamCmd->minDataRate      = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->meanDataRate     = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->peakDataRate     = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->maxBurstSize     = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->delayBound       = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->minPhyRate       = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->sba              = (i >= arg)?0:_ttoi(argv[i++]);
            crePStreamCmd->mediumTime       = (i >= arg)?0:_ttoi(argv[i]);
            if (arg == (i+2))
            {
                i++;
                crePStreamCmd->nominalPHY       = (i >= arg)?0:(A_UINT8)_ttoi(argv[i]);
            }

            if (crePStreamCmd->trafficClass > 3) {
                TelPrintf("bad traffic class (%d)\n", crePStreamCmd->trafficClass);
                TelPrintf("Traffic class should be 1(BK), 2(VI) or 3(VO)\n");
            } else if (crePStreamCmd->trafficDirection > BIDIR_TRAFFIC) {
                TelPrintf("bad traffic direction (%d)\n", crePStreamCmd->trafficDirection);
                TelPrintf("Traffic class should be 0(uplink), 1(dnlink) or 2(bi-dir)\n");
            }
            break;
        }

        if ( _tcscmp(argv[i], OPT_DELETE_QOS) == 0 ) {

            cmd = WMI_DELETE_QOS;
            i++;
            if((i + 1) >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            delPStreamCmd->trafficClass = (A_UINT8)_ttoi(argv[i++]);
            delPStreamCmd->tsid = (A_UINT8)_ttoi(argv[i]);
            break;
        }
        if ( _tcscmp(argv[i], OPT_GET_QOS_QUEUE) == 0 ) {
            cmd = WMI_GET_QOS_QUEUE;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            getQosQueueCmd->trafficClass = (A_UINT8)_ttoi(argv[i]);
            break;
        }
        if ( _tcscmp(argv[i], OPT_SET_WMM) == 0 ) {
            cmd = WMI_SET_WMM;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            setWmmCmd->status = (A_UINT8)_ttoi(argv[i]);
            setWmmCmd->status = 1;
            break;
        }
        if ( _tcscmp(argv[i], OPT_SET_TXOP) == 0 ) {
            cmd = WMI_SET_TXOP;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            pTxOp->txopEnable = (A_UINT8)_ttoi(argv[i]);
            break;
        }

        if ( _tcscmp(argv[i], OPT_SET_BTSTATUS) == 0 ) {

            cmd = WMI_SET_BTSTATUS;
            i++;
            if((i + 1) >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            pBtStatCmd->streamType = (A_UINT8)_ttoi(argv[i++]);
            pBtStatCmd->status = (A_UINT8)_ttoi(argv[i]);
            if (pBtStatCmd->streamType >= BT_STREAM_MAX ||
                pBtStatCmd->status >= BT_STATUS_MAX)
            {
                TelPrintf("Invalid parameters.\n");
                PrintUsage();
                return 0;
            }

            break;
        }
        if ( _tcscmp(argv[i], OPT_SET_BTPARAMS) == 0 ) {
        cmd = WMI_SET_BTPARAMS;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }

            pBtParmCmd->paramType = (A_UINT8)_ttoi(argv[i++]);
            if (pBtParmCmd->paramType >= BT_PARAM_MAX)
            {
                TelPrintf("Invalid parameters.\n");
                PrintUsage();
                return 0;
            }
            if (BT_PARAM_SCO == pBtParmCmd->paramType)
            {
                pBtParmCmd->info.scoParams.numScoCyclesForceTrigger =
                    (i >= arg) ? 0: _ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.dataResponseTimeout =
                    (i >= arg) ? 0:_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.stompScoRules =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.scoOptFlags =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.p2lrpOptModeBound=
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.p2lrpNonOptModeBound =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.stompDutyCyleVal =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.stompDutyCyleMaxVal =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams. psPollLatencyFraction =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.noSCOSlots =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.noIdleSlots =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.scoParams.reserved8 =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i]);
            }
            else if (BT_PARAM_A2DP == pBtParmCmd->paramType)
            {
                pBtParmCmd->info.a2dpParams.a2dpWlanUsageLimit =
                    (i >= arg) ? 0: _ttoi(argv[i++]);
                pBtParmCmd->info.a2dpParams.a2dpBurstCntMin =
                    (i >= arg) ? 0:_ttoi(argv[i++]);
                pBtParmCmd->info.a2dpParams.a2dpDataRespTimeout =
                    (i >= arg) ? 0:_ttoi(argv[i++]);
                pBtParmCmd->info.a2dpParams.a2dpOptFlags =
                    (i >= arg) ? 0:_ttoi(argv[i++]);
                pBtParmCmd->info.a2dpParams.p2lrpOptModeBound =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.a2dpParams.p2lrpNonOptModeBound =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.a2dpParams.reserved16 =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.a2dpParams.isCoLocatedBtRoleMaster =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i++]);
                pBtParmCmd->info.a2dpParams.reserved8 =
                    (i >= arg) ? 0:(A_UINT8)_ttoi(argv[i]);
            }
            else if (BT_PARAM_COLOCATED_BT_DEVICE == pBtParmCmd->paramType)
            {
                pBtParmCmd->info.coLocatedBtDev = (i > arg) ? 0:
                    (A_UINT8)_ttoi(argv[i]);
            }
            else if(BT_PARAM_ANTENNA_CONFIG == pBtParmCmd->paramType)
            {
                pBtParmCmd->info.antType   = (i > arg)?  0:
                    (A_UINT8)_ttoi(argv[i]);
            }
            else if(BT_PARAM_ACLCOEX == pBtParmCmd->paramType)
            {
                pBtParmCmd->info.aclCoexParams.aclWlanMediumUsageTime =
                    (i >= arg)? 0: _ttoi(argv[i++]);
                pBtParmCmd->info.aclCoexParams.aclBtMediumUsageTime =
                    (i >= arg)? 0: _ttoi(argv[i++]);
                pBtParmCmd->info.aclCoexParams.aclDataRespTimeout =
                    (i >= arg)? 0: _ttoi(argv[i++]);
                pBtParmCmd->info.aclCoexParams.aclDetectTimeout =
                    (i >= arg)? 0: _ttoi(argv[i++]);
                pBtParmCmd->info.aclCoexParams.aclmaxPktCnt =
                    (i >= arg)? 0: _ttoi(argv[i]);
            }
            else
            {
                TelPrintf("Invalid parameters.\n");
                PrintUsage();
                return 0;
            }
            break;
        }

        if ( _tcscmp(argv[i], OPT_DBGLOG_CFG_MODULE) == 0 ) {

            TelPrintf("Debug log Config Module Cmd\n");
            cmd = WMI_DBGLOG_CFG_MODULE;
            i++;

            dbgLogCfgModuleCmd->config.cfgvalid = 0;

            for (;i<argc;i++) {

                if ( _tcscmp(argv[i], DBGLOG_MASK) == 0 ) {

                    i++;
                    if (argv[i]) {
                        dbgLogCfgModuleCmd->config.cfgmmask = _ttoi(argv[i]);
                        dbgLogCfgModuleCmd->config.cfgvalid |= DBGLOG_MODULE_LOG_ENABLE_MASK;
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], DBGLOG_REP) == 0 ) {

                    i++;
                    if (argv[i]) {
                        dbgLogCfgModuleCmd->config.cfgrep = _ttoi(argv[i]);
                        dbgLogCfgModuleCmd->config.cfgvalid |= DBGLOG_REPORTING_ENABLED_MASK;
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], DBGLOG_TSR) == 0 ) {

                    i++;
                    if (argv[i]) {
                        dbgLogCfgModuleCmd->config.cfgtsr = _ttoi(argv[i]);
                        dbgLogCfgModuleCmd->config.cfgvalid |= DBGLOG_TIMESTAMP_RESOLUTION_MASK;
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], DBGLOG_SIZE) == 0 ) {

                    i++;
                    if (argv[i]) {
                        dbgLogCfgModuleCmd->config.cfgsize = _ttoi(argv[i]);
                        dbgLogCfgModuleCmd->config.cfgvalid |= DBGLOG_REPORT_SIZE_MASK;
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else {

                    PrintUsage();
                    return 0;
                }
            } //end for

            break;
        } //OPT_DBGLOG_CFG_MODULE

        if ( _tcscmp(argv[i], OPT_GET_DBGLOGS) == 0 ) {

            TelPrintf("Debug log Get Debug Logs Cmd\n");
            cmd = WMI_DBGLOG_GET_DEBUG_LOGS;

            break;
        } //OPT_GET_DBGLOGS

        if ( _tcscmp(argv[i], OPT_SCAN_PARAMS) == 0 ) {
            TelPrintf("Scan Params cmd\n");
            cmd = WMI_SET_SCAN_PARAMS;
            sParamCmd->scanCtrlFlags = DEFAULT_SCAN_CTRL_FLAGS;
            sParamCmd->shortScanRatio = WMI_SHORTSCANRATIO_DEFAULT;
            sParamCmd->max_dfsch_act_time = 0;
            sParamCmd->maxact_scan_per_ssid = 0;

            i++;
            for (;i<argc;i++) {
                if ( _tcscmp(argv[i], SCANPARAMS_FGSTART) == 0 ) {
                    i++;
                    if (argv[i]) {
                        sParamCmd->fg_start_period = (A_UINT16)_ttoi(argv[i++]);
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], SCANPARAMS_FGEND) == 0 ) {
                    i++;
                    if (argv[i]) {
                        sParamCmd->fg_end_period = (A_UINT16)_ttoi(argv[i++]);
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], SCANPARAMS_BG) == 0 ) {
                    i++;
                    if (argv[i]) {
                        sParamCmd->bg_period = (A_UINT16)_ttoi(argv[i++]);
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], SCANPARAMS_MINACT) == 0 ) {
                    i++;
                    if (argv[i]) {
                        sParamCmd->minact_chdwell_time =(A_UINT16) _ttoi(argv[i++]);
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], SCANPARAMS_MAXACT) == 0 ) {
                    i++;
                    if (argv[i]) {
                        sParamCmd->maxact_chdwell_time =(A_UINT16) _ttoi(argv[i++]);
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], SCANPARAMS_PASSIVE) == 0 ) {
                    i++;
                    if (argv[i]) {
                        sParamCmd->pas_chdwell_time = (A_UINT16)_ttoi(argv[i++]);
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], SCANPARAMS_SHORTSCANRATIO) == 0 ) {
                    i++;
                    if (argv[i]) {
                        sParamCmd->shortScanRatio = (A_UINT8)_ttoi(argv[i++]);
                    } else {
                        PrintUsage();
                        return 0;
                    }
                } else if ( _tcscmp(argv[i], SCANPARAMS_SCANCTRLFLAGS) == 0 ) {
                    if ( i++ - argc <= 6 ) {
                        PrintUsage();
                        return 0;
                    }
                    sParamCmd->scanCtrlFlags = 0;
                    if (_ttoi(argv[i++])) {
                        sParamCmd->scanCtrlFlags |= CONNECT_SCAN_CTRL_FLAGS;
                    }
                    if (_ttoi(argv[i++])) {
                        sParamCmd->scanCtrlFlags |= SCAN_CONNECTED_CTRL_FLAGS;
                    }
                    if (_ttoi(argv[i++])) {
                        sParamCmd->scanCtrlFlags |= ACTIVE_SCAN_CTRL_FLAGS;
                    }
                    if (_ttoi(argv[i++])) {
                        sParamCmd->scanCtrlFlags |= ROAM_SCAN_CTRL_FLAGS;
                    }
                    if (_ttoi(argv[i++])) {
                        sParamCmd->scanCtrlFlags |= REPORT_BSSINFO_CTRL_FLAGS;
                    }
                    if (_ttoi(argv[i++])) {
                        sParamCmd->scanCtrlFlags |= ENABLE_AUTO_CTRL_FLAGS;
                    }
                    if (argc - i) {
                        if (_ttoi(argv[i++])) {
                            sParamCmd->scanCtrlFlags |= ENABLE_SCAN_ABORT_EVENT;
                        }
                    }
                }
                else if ( _tcscmp(argv[i], SCANPARAMS_MAXACT_SCAN_SSID) == 0 ) {
                    i++;
                    if (argv[i]) {
                        sParamCmd->maxact_scan_per_ssid = (A_UINT8)_ttoi(argv[i++]);
                    } else {
                        PrintUsage();
                        return 0;
                    }
                }
            }
            continue;
        } //OPT_SCAN_PARAMS

        if ( _tcscmp( argv[i], OPT_ROAMCTRL ) == 0 ) {
            int numArgs,j=0;
            WMI_BSS_BIAS *pBssBias;
            A_INT32 bias;

            TelPrintf("Roam Ctrl Setting: \n");
            cmd = WMI_SET_ROAMCTRL;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            rmctl->roamCtrlType = (A_UINT8)_ttoi(argv[i]);
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            numArgs = argc - i;
            switch (rmctl->roamCtrlType) {
                case WMI_FORCE_ROAM:
                if (numArgs != 1) {
                    TelPrintf("BSSID to roam not given\n");
                } else if (wmic_ether_aton(argv[i], rmctl->info.bssid) != A_OK) {
                    TelPrintf("BSSID %s not in correct format\n", argv[i]);
                }
                break;
            case WMI_SET_ROAM_MODE:
                if (numArgs != 1) {
                    fprintf(stderr, "roam mode(default, bssbias, lock) not "
                            " given\n");
                } else {
                    if (_tcscmp(argv[i], ROAM_MODE_DEF) == 0) {
                        rmctl->info.roamMode = WMI_DEFAULT_ROAM_MODE;
                    } else if (_tcscmp(argv[i], ROAM_MODE_BIAS) == 0) {
                        rmctl->info.roamMode = WMI_HOST_BIAS_ROAM_MODE;
                    } else if (_tcscmp(argv[i], ROAM_MODE_LOCK) == 0) {
                        rmctl->info.roamMode = WMI_LOCK_BSS_MODE;
                    } else {
                        TelPrintf("roam mode(default, bssbias, lock) not given\n");
                    }
                }
                break;
            case WMI_SET_HOST_BIAS:
                if ((numArgs & 0x01) || (numArgs > 25) ) {
                    TelPrintf("roam bias too many entries or bss bias not input for every BSSID\n");
                } else {
                    rmctl->info.bssBiasInfo.numBss = numArgs >> 1;
                    pBssBias = rmctl->info.bssBiasInfo.bssBias;
                    while (j < rmctl->info.bssBiasInfo.numBss) {
                        if (wmic_ether_aton(argv[i + 2 * j], pBssBias[j].bssid) != A_OK)
                        {
                            TelPrintf("BSSID %s not in correct format\n", argv[i + 2 * j]);
                            rmctl->info.bssBiasInfo.numBss = 0;
                            break;
                        }
                        bias  = _ttoi(argv[i + 2 * j + 1]);
                        if ((bias < -256) || (bias > 255)) {
                            TelPrintf("bias value %d is not in range\n", bias);
                            rmctl->info.bssBiasInfo.numBss = 0;
                            break;
                        }
                        pBssBias[j].bias = bias;
                        j++;
                    }
                }
                break;
            case WMI_SET_LOWRSSI_SCAN_PARAMS:
                if (numArgs != 4) {
                    TelPrintf("not enough arguments\n");
                } else {
                    rmctl->info.lrScanParams.lowrssi_scan_period = (A_UINT16)_ttoi(argv[i]);
                    if ((A_UINT16)_ttoi(argv[i+1]) >= (A_UINT16)_ttoi(argv[i+2])) {
                        rmctl->info.lrScanParams.lowrssi_scan_threshold = (A_UINT16)_ttoi(argv[i+1]);
                        rmctl->info.lrScanParams.lowrssi_roam_threshold = (A_UINT16)_ttoi(argv[i+2]);
                    } else {
                        TelPrintf("Scan threshold should be greater than equal to roam threshold\n");
                    }
                    rmctl->info.lrScanParams.roam_rssi_floor = (A_UINT8)_ttoi(argv[i+3]);
                }
                break;
            default:
                TelPrintf("roamctrl type %d out if range should be between %d and %d\n",
                    rmctl->roamCtrlType, WMI_MIN_ROAM_CTRL_TYPE, WMI_MAX_ROAM_CTRL_TYPE);
                break;
            }

        } //OPT_ROAMCTRL

        if ( _tcscmp( argv[i], OPT_IBSS_CHANNEL ) == 0 ) {
            cmd = WMI_SET_IBSS_CHANNEL;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            ibssChannelCmd->ibssChannel = (A_UINT8)_ttoi(argv[i]);
            TelPrintf("channel at wmiconfig is %d \n",ibssChannelCmd->ibssChannel);
            break;
        }

        if ( _tcscmp( argv[i], OPT_WMODE ) == 0 ) {
            A_INT8 phyMode;
            A_UINT16 *clist;
            int channel, chindex;

            cmd = WMI_SET_CHANNEL;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            chParamCmd->numChannels = 0;
            chParamCmd->scanParam = 0;
            phyMode = (A_INT8) getPhyMode(argv[i++]);
            if (phyMode == -1 ) {
                TelPrintf("Incorrect Phy Mode\n");
                return 0;
            }
            chParamCmd->phyMode = (WMI_PHY_MODE) phyMode;
            clist = chParamCmd->channelList;
            chindex = 0;
            for (; i<argc; i++) {
                if (_tcscmp(argv[i], WMODE_SCAN) == 0 ){
                    ++i;
                    if(i >= argc)
                    {
                        TelPrintf("Not enough arguments\r\n");
                        PrintUsage();
                        return 0;
                    }
                    chParamCmd->scanParam = (A_UINT8)_ttoi(argv[i++]);
                    break;
                } else {
                    channel = (A_UINT16)_ttoi(argv[i]);
                    if ( !channel )
                        break;
                    if ( channel < 255 ) {
                        clist[chindex++] = wmic_ieee2freq(channel);
                    } else {
                        clist[chindex++] = channel;
                    }
                }
            }
            chParamCmd->numChannels = chindex;
            break;
        } // OPT_WMODE

        if ( _tcscmp(argv[i], OPT_SET_ATIM) == 0 ) {
            cmd = WMI_SET_ATIM;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            atimWindow = (A_UINT8)_ttoi(argv[i]);
        }

        if ( _tcscmp( argv[i], OPT_IBSSPM ) == 0 ) {
            cmd = WMI_SET_IBSS_PM_CAPS;
            i++;
            for (; i<argc; i++) {
                if((i + 1) >= argc)
                {
                    TelPrintf("Not enough arguments\r\n");
                    PrintUsage();
                    return 0;
                }
                if (_tcscmp(argv[i], IBSSPM_PS) == 0 ){
                    ++i;
                    if (_tcscmp(argv[i], WS_ENABLE) == 0) {
                        adhocPmCmd->power_saving = 1;
                    } else if (_tcscmp(argv[i], WS_DISABLE) == 0 ) {
                        adhocPmCmd->power_saving = 0;
                    } else {
                        TelPrintf("improper arg for option -ps\n");
                        return 0;
                    }
                } else if (_tcscmp(argv[i], IBSSPM_AW) == 0) {
                    ++i;
                    adhocPmCmd->atim_windows = (A_UINT16)_ttoi(argv[i]);
                } else if (_tcscmp(argv[i], IBSSPM_TO) == 0) {
                    ++i;
                    adhocPmCmd->timeout_value = (A_UINT16)_ttoi(argv[i]);
                } else if (_tcscmp(argv[i], IBSSPM_TTL) == 0) {
                    ++i;
                    adhocPmCmd->ttl = (A_UINT8)_ttoi(argv[i]);
                }
            }
            break;
        } // OPT_IBSSPM

        if ( _tcscmp( argv[i], OPT_PMPARAMS ) == 0 ) {
            cmd = WMI_SET_PM_PARAMS;
            i++;
            if((i + 1) >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            for (; i<argc; i++) {
                if (_tcscmp(argv[i], PM_DP) == 0 ){
                ++i;
                if (_tcscmp(argv[i], DTIM_IGNORE) == 0) {
                        pmParamCmd->dtim_policy = IGNORE_DTIM;
                    } else if (_tcscmp(argv[i], DTIM_NORMAL) == 0 ){
                        pmParamCmd->dtim_policy = NORMAL_DTIM;
                    } else if (_tcscmp(argv[i], DTIM_STICK) == 0 ) {
                        pmParamCmd->dtim_policy = STICK_DTIM;
                    } else {
                        TelPrintf("improper arg for option -dp\n");
                        return 0;
                    }
                } else if (_tcscmp(argv[i], PM_NP) == 0) {
                    ++i;
                    pmParamCmd->pspoll_number = (A_UINT16)_ttoi(argv[i]);
                } else if (_tcscmp(argv[i], PM_IT) == 0) {
                    ++i;
                    pmParamCmd->idle_period = (A_UINT16)_ttoi(argv[i]);
                }
            }
            break;
        } // OPT_PMPARAMS

        if ( _tcscmp(argv[i], OPT_DUMP_HTC_STATE) == 0 ) {
            TelPrintf("Dump HTC state command\n");
            cmd = DUMP_HTC_STATE;
            break;
        }

        if ( _tcscmp(argv[i], OPT_WMI_GET_VERSION) == 0 ) {
            TelPrintf("Display Target and Host versions\n");
            cmd = WMI_GET_VERSION;
            break;
        }

        if ( _tcscmp(argv[i], OPT_DUMP_CHIP_MEM) == 0 ) {
            TelPrintf("Dump chip mem\n");
            cmd = WMI_DUMP_CHIP_MEM;
            break;
        }

        if ( _tcscmp( argv[i], OPT_ABORT_SCAN ) == 0 ) {
        cmd = WMI_ABORT_SCAN;
        break;
        }
        if ( _tcscmp(argv[i], OPT_GET_RD) == 0 ) {

            TelPrintf("Display Regulatory Domain code\n");
            cmd = WMI_GET_RD;
            break;
        }

        if ( _tcscmp(argv[i], OPT_GET_TGT_STATS) == 0 ) {

            TelPrintf("Display Station Statistics \n");
            cmd = WMI_GET_TGT_STATS;
            break;
        }

        if ( _tcscmp(argv[i], OPT_GET_BITRATE) == 0 ) {

            TelPrintf("Display Rx and Tx Rate \n");
            cmd = WMI_GET_BITRATE;
            break;
        }

        if ( _tcscmp(argv[i], OPT_SET_BITRATE) == 0 ) {
            cmd = WMI_SET_FIX_RATES;
            setFixRatesCmd->fixRateMask = 0;
            i++;
            for (; i<argc; i++) {
                if ((A_UINT16)_ttoi(argv[i])<0 || (A_UINT16)_ttoi(argv[i])>11)
                {
                    TelPrintf("incorrect value for rates parameters\n");
                    return -1;
                }
                setFixRatesCmd->fixRateMask |= 1<<(A_UINT16)_ttoi(argv[i]);
            }
            TelPrintf("fixRateMask 0x%x \n", setFixRatesCmd->fixRateMask);
            break;
        }

        if ( _tcscmp(argv[i], OPT_GET_WMODE) == 0 ) {

            TelPrintf("Display wireless mode \n");
            cmd = WMI_GET_WMODE;
            break;
        }

        if ( _tcscmp( argv[i], OPT_RSSITHRESH) == 0 ) {
            cmd = WMI_SET_RSSI_THRESHOLDS;
            memset(threshold, 0, sizeof(threshold));
            i = i+2;
            index = i;
            for (i; i<=argc; i++)
                threshold[i-index] = (A_INT16)_ttoi(argv[i-1]);

            rssiThresholdParam->weight                 = (A_UINT8)threshold[0];
            rssiThresholdParam->pollTime               = (A_UINT32)threshold[1];
            rssiThresholdParam->tholds[0].tag          = threshold[2];
            rssiThresholdParam->tholds[0].rssi         = 0 - threshold[3];
            rssiThresholdParam->tholds[1].tag          = threshold[4];
            rssiThresholdParam->tholds[1].rssi         = 0 - threshold[5];
            rssiThresholdParam->tholds[2].tag          = threshold[6];
            rssiThresholdParam->tholds[2].rssi         = 0 - threshold[7];
            rssiThresholdParam->tholds[3].tag          = threshold[8];
            rssiThresholdParam->tholds[3].rssi         = 0 - threshold[9];
            rssiThresholdParam->tholds[4].tag          = threshold[10];
            rssiThresholdParam->tholds[4].rssi         = 0 - threshold[11];
            rssiThresholdParam->tholds[5].tag          = threshold[12];
            rssiThresholdParam->tholds[5].rssi         = 0 - threshold[13];
            rssiThresholdParam->tholds[6].tag          = threshold[14];
            rssiThresholdParam->tholds[6].rssi         = 0 - threshold[15];
            rssiThresholdParam->tholds[7].tag          = threshold[16];
            rssiThresholdParam->tholds[7].rssi         = 0 - threshold[17];
            rssiThresholdParam->tholds[8].tag          = threshold[18];
            rssiThresholdParam->tholds[8].rssi         = 0 - threshold[19];
            rssiThresholdParam->tholds[9].tag          = threshold[20];
            rssiThresholdParam->tholds[9].rssi         = 0 - threshold[21];
            rssiThresholdParam->tholds[10].tag         = threshold[22];
            rssiThresholdParam->tholds[10].rssi        = 0 - threshold[23];
            rssiThresholdParam->tholds[11].tag         = threshold[24];
            rssiThresholdParam->tholds[11].rssi        = 0 - threshold[25];
            break;
        }

		if ( _tcscmp( argv[i], OPT_SET_NODEAGE ) == 0 ) {
            cmd = WMI_SET_NODEAGE;
            i++;
            if(i >= argc)
            {
                PrintUsage();
                TelPrintf("Not enough argument\r\n");
                return 0;
            }
            nodeage = (A_UINT32)_ttoi(argv[i]);
            TelPrintf("node age is %d \n", nodeage);
            break;
        }

	    if ( _tcscmp( argv[i], OPT_GET_NODEAGE ) == 0 ) {
            cmd = WMI_GET_NODEAGE;
            i++;
            break;
        }

		if ( _tcscmp(argv[i], OPT_STA_LISTEN_INTVL) == 0 ) {

			printf("set listen interval of station in TUs\n");
			cmd = WMI_SET_LISTEN_INTERVAL;
			memset(listenCmd,0,sizeof(listenCmd));
			if (argc <=4)
			{
				PrintUsage();
				return 0;
			}
			i++;
			listenCmd->listenInterval = (A_UINT16)_ttoi(argv[i]);
			if ((listenCmd->listenInterval < MIN_LISTEN_INTERVAL) ||
				(listenCmd->listenInterval > MAX_LISTEN_INTERVAL))
			{
				printf("Listen Interval out of range\n");
				return 0;
			}
			break;
        }

        if ( _tcscmp(argv[i], OPT_QUERY_CURRENT_MODE) == 0 ) {

            printf("Get current network Type (AP/STATION) \n");
            cmd = WMI_GET_CURRENT_MODE;
            break;
        }

        if ( _tcscmp(argv[i], OPT_GET_COUNTRY_LIST) == 0 )
        {

            printf("get list of country codes \n");
             for(k = 0; k < sizeof(my_ctr)/sizeof(my_ctr[0]); k++)
             {
                 printf("%s\t",my_ctr[k]);
                 if ((k+1)%5 == 0)
                 {
                     printf("\n");
                 }
             }
             printf("\n");
             return 0;
        }

        if ( _tcscmp(argv[i], OPT_SET_NUMSTA) == 0 ) {

            printf("set  max num of stations in softAP mode\n");
            cmd = WMI_SET_NUMSTA;
            memset(numofsta,0,sizeof(numofsta));
            if (argc <=4)
            {
                PrintUsage();
                return 0;
            }
            i++;
            numofsta->num_sta = (A_UINT8)_ttoi(argv[i]);
            if ((numofsta->num_sta < 1) ||  (numofsta->num_sta > AP_MAX_NUM_STA))
            {
                TelPrintf("Max No. of stations should be between 1 to 8 \n");
                return 0;
            }

            break;
        }


        if ( _tcscmp(argv[i], APMODE_BG_PROTECTION) == 0 ) {

            printf("set b/g protection scan in softAP mode\n");
            cmd = WMI_SET_BG_PROTECTION;
            memset(pProt,0,sizeof(pProt));
            if (argc <=5)
            {
                PrintUsage();
                return 0;
            }
            i++;
            pProt->period_min = (A_UINT8)_ttoi(argv[i]);
            i++;
            pProt->dwell_ms = (A_UINT8)_ttoi(argv[i]);
            break;
        }


        if ( _tcscmp(argv[i], APMODE_HIDDEN_SSID) == 0 ) {

            printf("set SSID to hidden in softAP mode\n");
            cmd = WMI_SET_HIDDEN_SSID;
            memset(pHidden,0,sizeof(pHidden));
            if (argc <=4)
            {
                PrintUsage();
                return 0;
            }
            i++;
            pHidden->hidden_ssid = (A_UINT8)_ttoi(argv[i]);
            break;
        }

        if ( _tcscmp(argv[i], APMODE_DTIM) == 0 ) {

            printf("set DTIM period in softAP mode\n");
            cmd = WMI_SET_AP_DTIM;
            memset(pDtim,0,sizeof(pDtim));
            if (argc <=4)
            {
                PrintUsage();
                return 0;
            }
            i++;
            pDtim->dtim =(A_UINT8)_ttoi(argv[i]);
            if ((pDtim->dtim < 1) ||  (pDtim->dtim > 10))
            {
                TelPrintf("DTIM value should be between 1 and 10 \n");
                return 0;
            }
            break;
        }

        if ( _tcscmp(argv[i], OPT_SET_COUNTRY ) == 0 ) {

            printf("set country code\n");
            cmd = WMI_SET_COUNTRY;
            memset(pCountry,0,sizeof(pCountry));
            if (argc <=4)
            {
                PrintUsage();
                return 0;
            }
            i++;
            wcstombs(temp,argv[i],_tcslen((TCHAR *)argv[i]));
            k = 0;
            for(k = 0; k < sizeof(my_ctr)/sizeof(my_ctr[0]); k++) {
                if(!strcmp(temp, my_ctr[k])) {
                    match = 1;
                    break;
                }
            }

            if (!match) {
                cmd = 0;
            } else {
                memcpy(pCountry->countryCode,my_ctr[k], 2);
                *(pCountry->countryCode + 2)=0x20;
            }

            break;
        }

       if ( _tcscmp(argv[i], OPT_GET_STA) == 0 ) {

           printf("To get the list of stations connected to softAP\n");
           cmd = WMI_GET_STA;
           break;
        }


       if ( _tcscmp(argv[i], OPT_ADD_ACL) == 0 ) {

           printf("To add station with the given mac to softAP's allow/deny list\n");
           cmd = WMI_AP_ACL_MAC_LIST1;
           memset(pACL,0,sizeof(pACL));
           if (argc <=4)
           {
               PrintUsage();
               return 0;
            }
           i++;
           pACL->action = ADD_MAC_ADDR;
           if(wmic_ether_aton_wild(argv[i], pACL->mac, &pACL->wildcard) != A_OK) {
               printf("bad mac address\n");
               exit (0);
           }
           break;
        }

       if ( _tcscmp(argv[i], OPT_DEL_ACL) == 0 ) {

           printf("To del station with the given mac from softAP's allow/deny list\n");
           cmd = WMI_AP_ACL_MAC_LIST2;
           memset(pACL,0,sizeof(pACL));
           if (argc <=4)
           {
               PrintUsage();
               return 0;
            }
           i++;
           pACL->action = DEL_MAC_ADDR;
           pACL->index = (A_UINT8)_ttoi(argv[i]);
           break;
        }

      if ( _tcscmp(argv[i], OPT_GET_ACL) == 0 ) {

          printf("To get softAP's allow/deny list\n");
          cmd = WMI_AP_GET_ACL_LIST;
          break;
        }

      if ( _tcscmp(argv[i], OPT_SET_ACL_POLICY) == 0 ) {

          printf ("To define softAP's ACL policy\n");
          cmd = WMI_AP_ACL_POLICY;
          memset (pACLpolicy, 0, sizeof(pACLpolicy));
          if (argc <=5)
          {
              PrintUsage();
              return 0;
          }
          i++;
          policy = (A_UINT8)_ttoi(argv[i]);
          i++;
          retain = (A_UINT8)_ttoi(argv[i]);
          pACLpolicy->policy = policy | (retain?AP_ACL_RETAIN_LIST_MASK:0);
          break;
        }

      if ( _tcscmp(argv[i], OPT_STA_ISOLATION) == 0 ) {

         printf ("To enable/disable intrabss traffic\n");
         cmd = WMI_AP_INTRA_BSS;
         memset (pIntra, 0, sizeof(pIntra));
         if (argc <=4)
         {
             PrintUsage();
             return 0;
         }
         i++;
         pIntra->intrabss = (A_UINT8)_ttoi(argv[i]);
         break;
        }

      if( _tcscmp(argv[i], OPT_CONN_INACT) == 0){

          printf("To set the connection inactivity time in AP mode");
          cmd = WMI_AP_INACT_TIME;
          memset(pInact,0,sizeof(pInact));
          if (argc <=4)
          {
              PrintUsage();
              return 0;
           }
          i++;
          pInact->period = (A_UINT16)_ttoi(argv[i]);
          break;

       }

     if( _tcscmp(argv[i], OPT_REM_STA) == 0){

         printf("To remove an associated station in AP mode");
         cmd = WMI_AP_REMOVE_STA;
        if (argc <=6)
         {
             PrintUsage();
             return 0;
         }
         i++;
         pMlme->cmd = (A_UINT8)_ttoi(argv[i]);
         i++;
         pMlme->reason = (A_UINT16)_ttoi(argv[i]);
         i++;
         if(wmic_ether_aton(argv[i], pMlme->mac) != A_OK) {
             printf("bad mac address\n");
             exit (0);
            }
         break;

       }


    if ( _tcscmp(argv[i], OPT_SET_APMODE) == 0 ) {
          TelPrintf("Set STA to AP Mode \n");
          cmd = WMI_SET_APMODE;
          memset(apCmd,0,sizeof(apCmd));
          if (argc <=5)
          {
              PrintUsage();
              return 0;
          }
          i++;
          for (;i<argc;i++)
          {
              if ( _tcscmp(argv[i], APMODE_SSID) == 0 )
              {
                  i++;
                  if (argv[i])
                  {
                      wcstombs(apCmd->ssid,argv[i],_tcslen((TCHAR *)argv[i]));
                      apCmd->ssidLength = _tcslen((TCHAR *)argv[i]);
                  }
                  else
                  {
                      PrintUsage();
                      return 0;
                  }
              }
               else if ( _tcscmp(argv[i], APMODE_CHANNEL) == 0 )
               {
                    i++;
                    if (argv[i])
                    {
                        apCmd->channel = wmic_ieee2freq((A_UINT16)_ttoi(argv[i]));
                    }
                    else
                    {
                        PrintUsage();
                        return 0;
                    }
                }
            }
            break;
       }


    if ( _tcscmp(argv[i], OPT_SET_STAMODE) == 0 ) {

        printf("set AP to STA mode\n");
        cmd = WMI_SET_STAMODE;
        break;
    }

    if( _tcscmp(argv[i], OPT_SET_BEACON_INT) == 0){

        TelPrintf("To set beacon interval in AP mode and ADHOC mode");
        cmd = WMI_SET_BEACON_INT;
        if (argc <=4)
        {
            PrintUsage();
            return 0;
        }
        i++;
        bconIntvl->beaconInterval = (A_UINT16)_ttoi(argv[i]);
        break;

      }

    if( _tcscmp(argv[i], OPT_SET_IP) == 0){
       printf("To set IP Address");
       cmd = WMI_SET_IP;
       memset (pIP, 0, sizeof(pIP));
       if (argc <=4)
       {
            PrintUsage();
            return 0;
       }
       i++;
       if( _tcscmp(argv[i], SET_IP_OPTION) == 0)
       {
            break;
       }
       memset(temp1,0,sizeof(temp1));
       wcstombs(temp1,argv[i],_tcslen((TCHAR *)argv[i]));
       pIP->ips[0] = inet_addr((const char *)temp1);
       if (!pIP->ips[0] || ((pIP->ips[0] & 0xf0) >= 0xe0))
       {
           printf("Invalid IP\n");
           return 0;
       }
       i++ ;
       if (i < argc)
       {
           memset(temp1,0,sizeof(temp1));
           wcstombs(temp1,argv[i],_tcslen((TCHAR *)argv[i]));
           pIP->ips[1] = inet_addr((const char *)temp1);
           if (!pIP->ips[1] || ((pIP->ips[1] & 0xf0) >= 0xe0))
           {
               printf("Invalid IP\n");
               return 0;
           }
       }
       break;
      }

      if ( _tcscmp(argv[i], OPT_SET_AP_SECURITY) == 0 ) {
          TelPrintf("Set AP key \n");
          cmd = WMI_SET_AP_SECURITY;
          memset(apsec,0,sizeof(apsec));
          if (argc <=5) {
              PrintUsage();
              return 0;
          }
          i++;
          for (;i<argc;i++)
          {
              if ( _tcscmp(argv[i], APKEY_INDEX) == 0 )
              {
                  i++;
                  if (argv[i])
                  {

                      apsec->KeyIndex = (A_UINT8)_ttoi(argv[i]);
                      apsec->KeyIndex--;
                  }
                  else
                  {
                      PrintUsage();
                      return 0;
                  }
              }
              else if ( _tcscmp(argv[i], APKEY_KEY) == 0 )
              {
                  i++;
                  if (argv[i])
                  {
                      wcstombs(apsec->KeyMaterial,argv[i],_tcslen((TCHAR *)argv[i]));
                      apsec->KeyLength = _tcslen((TCHAR *)argv[i]);
                  }
                  else
                  {
                      PrintUsage();
                      return 0;
                  }
              }
              else if ( _tcscmp(argv[i], APKEY_AUTH) == 0 )
              {
                  i++;
                  if(argv[i])
                  {
                      if ( _tcscmp(argv[i], APKEY_AUTH_SHARED) == 0 )
                      {
                          apsec->dot11_authmode = SHARED_AUTH ;
                          apsec->Authmode = NONE_AUTH;
                      }
                      else if (_tcscmp(argv[i], APKEY_AUTH_OPEN) == 0 )
                      {
                          apsec->dot11_authmode = OPEN_AUTH ;
                          apsec->Authmode = NONE_AUTH;
                      }
                      else if (_tcscmp(argv[i], APKEY_AUTH_WPA_PSK) == 0 )
                      {
                          apsec->dot11_authmode = OPEN_AUTH ;
                          apsec->Authmode = WPA_PSK_AUTH;
                      }
                      else if (_tcscmp(argv[i], APKEY_AUTH_WPA2_PSK) == 0 )
                      {
                          apsec->dot11_authmode = OPEN_AUTH ;
                          apsec->Authmode = WPA2_PSK_AUTH;
                      }
                  }
                  else
                  {
                      PrintUsage();
                      return 0;
                  }
              }
              else if ( _tcscmp(argv[i], APKEY_CRYPT) == 0 )
              {
                  i++;
                  if(argv[i])
                  {
                      if ( _tcscmp(argv[i], APKEY_CRYPT_NONE) == 0 )
                      {
                          apsec->cryptType = NONE_CRYPT ;
                      }
                      else if (_tcscmp(argv[i], APKEY_CRYPT_WEP) == 0 )
                      {
                          apsec->cryptType = WEP_CRYPT ;
                      }
                      else if (_tcscmp(argv[i], APKEY_CRYPT_TKIP) == 0 )
                      {
                          apsec->cryptType = TKIP_CRYPT ;
                      }
                      else if (_tcscmp(argv[i], APKEY_CRYPT_AES) == 0 )
                      {
                          apsec->cryptType = AES_CRYPT ;
                      }
                  }
                  else
                  {
                      PrintUsage();
                      return 0;
                  }
              }

          } // for


          if((validate_ap_key (apsec)) != A_OK)
          {
              PrintUsage();
              return 0;
          }

          apsec->Length = sizeof(apkey);
          break;
     }

    if ( _tcscmp(argv[i], OPT_SET_KEEPALIVE) == 0 ) {
        TelPrintf("Set Keep Alive Interval \n");
        cmd = WMI_SET_KEEPALIVE;
         i++;
         if(i >= argc)
         {
             TelPrintf("Not enough arguments\r\n");
              PrintUsage();
              return 0;
         }
         keepAliveIntv = (A_UINT8)_ttoi(argv[i]);
         TelPrintf("Keep Alive Interval is %d \n", keepAliveIntv);
          break;
     }

     if ( _tcscmp( argv[i], OPT_GET_KEEPALIVE ) == 0 )
     {
         TelPrintf("Get Keep Alive Interval \n");
         cmd = WMI_GET_KEEPALIVE;
         break;
     }


        if ( _tcscmp( argv[i], OPT_SET_PWRSTATE ) == 0 ) {
            cmd = WMI_SET_POWER_STATE;
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\r\n");
                PrintUsage();
                return 0;
            }
            memset(&devPower,0,sizeof(devPower));
            if (_tcscmp(argv[i], WIFI_ON) == 0) {
                devPower = NdisDeviceStateD0;
            } else if (_tcscmp(argv[i], WIFI_OFF) == 0 ) {
                devPower = NdisDeviceStateD3;
            }else{
                TelPrintf("Invalid parameters.\n");
                PrintUsage();
                return 0;
            }
            break;
        }
        if (_tcscmp(argv[i], OPT_SET_IP_ADDRESS) == 0) {
            cmd = WMI_SET_IP_ADDRESS;

            memset(&ipAddress,0,sizeof(ipAddress));
            i++;
            if(i >= argc)
            {
                TelPrintf("Not enough arguments\n");
                PrintUsage();
                return 0;
            }
            if (_tcscmp(argv[i], DHCP_ON) == 0) {
                dhcpEnable = TRUE;
            } else if (_tcscmp(argv[i], DHCP_OFF) == 0 ) {
                dhcpEnable = FALSE;
            }else{
                TelPrintf("Invalid parameters.\n");
                PrintUsage();
                return 0;
            }
            if(dhcpEnable == FALSE)
            {
                if(argc <= 6){
                    PrintUsage();
                    return 0;
                }
                i++;
                memcpy(ipAddress.ipAddr,argv[i],2 * _tcslen((TCHAR *)argv[i]));
                i++;
                if(i >= argc)
                {
                    TelPrintf("Not enough arguments\n");
                    PrintUsage();
                    break;
                }
                memcpy(ipAddress.subnetMsk,argv[i],2 * _tcslen((TCHAR *)argv[i]));
            }
            break;
        }
    } // end For

    if ( _tcslen(devName) == 0 ) {
       PrintUsage();
       return -1;
    }

   TelPrintf("Interface:%s\n",devName);


   // Create the NDISUIO handle. This handle is not associated with a
   // device now. The device will be specified when an IOCTL is sent
   // to a device.

   hAdapter = CreateFile( NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL, OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                          INVALID_HANDLE_VALUE );

   TelPrintf("Handle: %p\n",(hAdapter));

#if 0
   /* NOTE: Wireless Zero-Config (WZC) exclusively owns the adapter for read/write operations,
    * The call to open the device will always fail.  This code is now commented out since wmiconfig
    * currently does not need to read/write packets to the device.  Should we
    * need to read/write the device sometime in the future (which requires disabling WZC) this
    * code can be re-instated */

   // Associate the handle with the adapter
   if ( !DeviceIoControl( hAdapter, IOCTL_NDISUIO_OPEN_DEVICE, devName,
                          _tcslen(devName) * sizeof(TCHAR), NULL, 0,
                        &dwReturnedBytes, NULL ) ) {
      ERR("IOCTL_NDISUIO_OPEN_DEVICE failed, continuing..");
   }
#endif

   switch( cmd ) {
      case WMI_SET_POWER_MODE:
         wmi_set_power_mode( hAdapter, devName, pwrCmd );
         break;
      case WMI_GET_RSSI:
         wmi_get_rssi( hAdapter, devName );
         break;

      case WMI_GET_TXPOWER:
         wmi_get_txpower( hAdapter, devName );
         break;
      case WMI_SET_TXPOWER:
         wmi_set_txpower( hAdapter, devName, txpwrCmd);
         break;
      case WMI_GET_BSSID:
         wmi_get_bssid( hAdapter, devName );
         break;

      case WMI_GET_SCANLIST:
         wmi_get_scan_list( hAdapter, devName );
         break;
      case WMI_SET_WLAN_SETTING:
         wmi_set_wlan_setting( hAdapter, devName, wsCmd );
         break;
        case WMI_SET_HOST_SLEEP_MODE:
            wmi_set_host_sleep_mode_cmd(hAdapter, devName, hostSleepModeCmd);
            break;
        case WMI_SET_WOW_MODE:
            wmi_set_wow_mode_cmd(hAdapter, devName, wowModeCmd);
            break;
        case WMI_ADD_WOW_PATTERN:
            wmi_add_wow_pattern_cmd(hAdapter, devName, addWowPatternCmd);
            break;
        case WMI_DEL_WOW_PATTERN:
            wmi_del_wow_pattern_cmd(hAdapter, devName, delWowPatternCmd);
            break;
        case WMI_GET_WOW_LIST:
            wmi_get_wow_list_cmd(hAdapter, devName, getWowListCmd);
            break;
        case WMI_CREATE_QOS:
            wmi_create_qos_cmd(hAdapter, devName, crePStreamCmd);
            break;
        case WMI_DELETE_QOS:
            wmi_delete_qos_cmd(hAdapter, devName, delPStreamCmd);
            break;
        case WMI_GET_QOS_QUEUE:
            wmi_get_qos_queue_cmd(hAdapter, devName, getQosQueueCmd);
            break;
        case WMI_SET_WMM:
            wmi_set_wmm_cmd(hAdapter, devName, setWmmCmd);
            break;
        case WMI_SET_TXOP:
            wmi_set_txop_cmd(hAdapter, devName, pTxOp);
            break;
        case WMI_DBGLOG_CFG_MODULE:
            wmi_dbglog_cfg_module_cmd(hAdapter, devName, dbgLogCfgModuleCmd);
            break;
        case WMI_DBGLOG_GET_DEBUG_LOGS:
            wmi_dbglog_get_debug_logs(hAdapter, devName);
            break;
        case WMI_SET_BTSTATUS:
            wmi_set_btstatus_cmd(hAdapter, devName, pBtStatCmd);
            break;
        case WMI_SET_BTPARAMS:
            wmi_set_btparams_cmd(hAdapter, devName, pBtParmCmd);
            break;
        case WMI_SET_SCAN_PARAMS:
            wmi_set_scan_params_cmd(hAdapter, devName, sParamCmd);
            break;
        case WMI_SET_ROAMCTRL:
            wmi_set_roam_cmd(hAdapter, devName, rmctl);
            break;
        case WMI_SET_IBSS_CHANNEL:
            wmi_set_ibss_channel ( hAdapter, devName, ibssChannelCmd );
        break;
        case WMI_SET_CHANNEL:
            wmi_set_channel ( hAdapter, devName, chParamCmd );
        break;

        case WMI_SET_ATIM:
            wmi_set_atim ( hAdapter, devName, atimWindow);
            break;

        case WMI_SET_IBSS_PM_CAPS:
            wmi_set_ibsspm ( hAdapter, devName, adhocPmCmd);
            break;

        case WMI_SET_PM_PARAMS:
            wmi_set_pmparams ( hAdapter, devName, pmParamCmd);
        break;

        case DUMP_HTC_STATE:
            DumpHtcState(hAdapter,devName);
            break;

        case WMI_GET_VERSION:
            wmi_get_version ();
            break;

        case WMI_DUMP_CHIP_MEM:
            wmi_dump_chip_mem ();
            break;

        case WMI_ABORT_SCAN:
             wmi_abort_scan( hAdapter, devName );
         break;

        case WMI_GET_RD:
            wmi_get_reg_domain(hAdapter, devName);
            break;

        case WMI_GET_BITRATE:
            wmi_get_bitrate (hAdapter, devName);
            break;

        case WMI_SET_FIX_RATES:
            wmi_set_fix_rates (hAdapter, devName, setFixRatesCmd);
            break;

        case WMI_GET_WMODE:
            wmi_get_wmode (hAdapter, devName);
            break;

    case WMI_GET_TGT_STATS:
            wmi_get_stats(hAdapter,devName);
            break;

        case WMI_SET_RSSI_THRESHOLDS:
         wmi_set_rssi_threshold(hAdapter, devName, rssiThresholdParam);
             break;

		case WMI_SET_LISTEN_INTERVAL:
			 wmi_set_listen_interval(hAdapter, devName, listenCmd);
             break;

        case WMI_SET_APMODE: /* Soft AP */
           wmi_set_apmode(hAdapter,devName,apCmd);
             break;

        case WMI_SET_STAMODE: /* Soft AP */
           wmi_set_stamode(hAdapter,devName);
            break;

        case WMI_GET_CURRENT_MODE: /* Soft AP */
           wmi_get_current_mode(hAdapter, devName);
            break;

        case WMI_SET_NUMSTA: /* Soft AP */
           wmi_set_numsta(hAdapter, devName,numofsta);
            break;

        case WMI_GET_STA: /* Soft AP */
            wmi_get_sta(hAdapter, devName);
            break;

        case WMI_AP_ACL_MAC_LIST1: /* Soft AP */
        case WMI_AP_ACL_MAC_LIST2: /* Soft AP */
           wmi_adddel_acl(hAdapter, devName,pACL);
            break;

         case WMI_AP_GET_ACL_LIST: /* Soft AP */
            wmi_get_acl(hAdapter, devName);
            break;

        case WMI_AP_ACL_POLICY: /* Soft AP */
            wmi_ap_acl_policy (hAdapter, devName, pACLpolicy);
            break;

          case WMI_AP_INACT_TIME: /* Soft AP */
             wmi_set_ap_inact_time(hAdapter, devName,pInact);
            break;

          case WMI_AP_REMOVE_STA: /* Soft AP */
             wmi_ap_remove_sta(hAdapter, devName,pMlme);
            break;

            case WMI_SET_BG_PROTECTION: /* Soft AP */
               wmi_set_bg_protection(hAdapter, devName,pProt);
            break;

            case WMI_SET_HIDDEN_SSID: /* Soft AP */
               wmi_set_hidden_ssid(hAdapter, devName,pHidden);
            break;

            case WMI_SET_AP_DTIM: /* Soft AP */
               wmi_set_dtim(hAdapter, devName,pDtim);
            break;

            case WMI_SET_COUNTRY:
               wmi_set_country(hAdapter, devName,pCountry);
            break;

            case WMI_SET_BEACON_INT:/*Soft AP */
               wmi_set_beacon_int(hAdapter, devName,bconIntvl);
            break;

            case WMI_SET_IP:
                wmi_set_ip (hAdapter, devName, pIP);
            break;

            case WMI_SET_AP_SECURITY:
                wmi_set_ap_security (hAdapter, devName, apsec);
            break;

            case WMI_AP_INTRA_BSS:
                wmi_set_ap_intrabss (hAdapter, devName, pIntra);
            break;

	    case WMI_SET_NODEAGE:
			 wmi_set_nodeage_cmd(hAdapter, devName, nodeage);
		     break;
		case WMI_GET_NODEAGE:
			 wmi_get_nodeage_cmd(hAdapter, devName);
			 break;
            case WMI_SET_KEEPALIVE:
                 wmi_set_keepalive(hAdapter, devName, keepAliveIntv);
                 break;

            case WMI_GET_KEEPALIVE:
                 wmi_get_keepalive(hAdapter, devName);
                 break;
        case WMI_SET_POWER_STATE:
            wmi_set_power_state(hAdapter,devName,&devPower);
            break;
        case WMI_SET_IP_ADDRESS:
            wmi_set_ip_address(hAdapter,devName,dhcpEnable,&ipAddress);
            break;
        default:
            PrintUsage();
            break;
   }

    if (hAdapter)
    {
       CloseHandle(hAdapter);
       hAdapter = NULL;
    }

   return 0;
}




