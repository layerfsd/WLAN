//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/*

Replacing bitmaps and icons:


Registry settings (common to PPC and SP):

[HKEY_LOCAL_MACHINE\Security\ResOver\Bitmaps]
"BaseDll"="tv.dll"
"BaseId"=dword:4a37

+    "Basedll" defines the location of the dll with the icon and/or bitmap resources
+    The dll should include as many custom icons and/or bitmaps as needed to replace the defaults that are intended  to be replaced.
+    The dll should respect the identifiers at the enum for each case.
+    "BaseId: defines a hex value which is added to the resource number to help ensure there aren't any conflicts with other resources in the same dll.
+    This registry setting is optional (an example hex value '4a37' is shown)
*/

// Resource IDs we currently allow for overriding bitmaps.
/*
Location services icons:  Note that these icons require the following RIL notification:

    RIL_NOTIFY_LOCATION requires a single DWORD parameter
    +    0 - Location Off
    +    1 - Location On
    +    all other values reserved

OEMs are able to replace the default location icon with a custom icon for a specific Operator. These bitmaps must be 1 bit per pixel bitmaps.
This location services icon is located in the title bar on smartphone. 
In order to replace the location services icon for Pocket PC (or in the phone application on smartphone), see IDB_OVERRIDE_PH_ICONS
*/
#define IDB_OVERRIDE_LOCATION_SVCS_ON                  0x0001 // "Location Services on"                - SP icon
#define IDB_OVERRIDE_LOCATION_SVCS_OFF                 0x0002 // "Location Services off"               - SP icon

// IMPORTANT
// For PPC the resources for CALL_HISTORY must be Bitmaps.         (ex.  IDI_OVERRIDE_CALL_HISTORY_MISSED BITMAP DISCARDABLE "bmp1.bmp")
// For SP the resources for CALL_HISTORY must be Icons.            (ex.  IDI_OVERRIDE_CALL_HISTORY_MISSED ICON DISCARDABLE "ico1.ico")

// call log entries marked with the LGIF_OCUSTOMICO and LGIF_OCUSTOMICO1  bit.  see phlog.h for more details.
#define IDI_OVERRIDE_CALL_HISTORY_OEM_MISSED           0x0003 // OEM call history "missed"             - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OEM_IN               0x0004 // OEM call history "in"                 - PPC bitmap.    SP icon  
#define IDI_OVERRIDE_CALL_HISTORY_OEM_OUT              0x0005 // OEM call history "out"                - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OEM_IN_ROAMING       0x0113 // OEM call history "in_roaming"         - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OEM_OUT_ROAMING      0x0114 // OEM call history "out_roaming"        - PPC bitmap.    SP icon

// call log entries marked with the LGIF_ONOCUSTOMICO bit (default).  
#define IDI_OVERRIDE_CALL_HISTORY_MISSED               0x0006 // default call history "missed"         - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_IN                   0x0007 // default call history "in"             - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OUT                  0x0008 // default call history "out"            - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_IN_ROAMING           0x0115 // default call history "in_roaming"     - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OUT_ROAMING          0x0116 // default call history "out_roaming"    - PPC bitmap.    SP icon

// call log entries marked with the LGIF_OCUSTOMICO and LGIF_OCUSTOMICO2 bit.  see phlog.h for more details.
#define IDI_OVERRIDE_CALL_HISTORY_OEM2_MISSED          0x0117 // OEM2 call history "missed"             - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OEM2_IN              0x0118 // OEM2 call history "in"                 - PPC bitmap.    SP icon  
#define IDI_OVERRIDE_CALL_HISTORY_OEM2_OUT             0x0119 // OEM2 call history "out"                - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OEM2_IN_ROAMING      0x011A // OEM2 call history "in_roaming"         - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OEM2_OUT_ROAMING     0x011B // OEM2 call history "out_roaming"        - PPC bitmap.    SP icon

// call log entries marked with the LGIF_OCUSTOMICO and LGIF_OCUSTOMICO3 bit.  see phlog.h for more details.
#define IDI_OVERRIDE_CALL_HISTORY_OEM3_MISSED          0x011C // OEM3 call history "missed"             - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OEM3_IN              0x011D // OEM3 call history "in"                 - PPC bitmap.    SP icon  
#define IDI_OVERRIDE_CALL_HISTORY_OEM3_OUT             0x011E // OEM3 call history "out"                - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OEM3_IN_ROAMING      0x011F // OEM3 call history "in_roaming"         - PPC bitmap.    SP icon
#define IDI_OVERRIDE_CALL_HISTORY_OEM3_OUT_ROAMING     0x0120 // OEM3 call history "out_roaming"        - PPC bitmap.    SP icon

// speed dial bitmap
#define IDB_OVERRIDE_SPEEDDIAL                         0x0009 // default speed dial bitmap                - PPC bitmap.

// SIM Security images (PPC only)
#define IDB_OVERRIDE_SIMSEC_PORT_DIALPAD               0x000A // the grid of number digits 0-9, *, #      - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_DIALPAD               0x000C // the grid of number digits 0-9, *, #      - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_DIALPAD               0x000E // the grid of number digits 0-9, *, #      - PPC bitmap.
    
#define IDB_OVERRIDE_SIMSEC_PORT_CMDPAD                0x0010 // the command button background            - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_CMDPAD                0x0011 // the command button background            - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_CMDPAD                0x0012 // the command button background            - PPC bitmap.

#define IDB_OVERRIDE_SIMSEC_PORT_DIALUP                0x0013 // a dialpad button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_DIALDOWN              0x0014 // a dialpad button pressed                 - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_CLEARUP               0x0015 // the clear button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_CLEARDOWN             0x0016 // the clear button pressed                 - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_CANCELUP              0x0017 // the cancel button                        - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_CANCELDOWN            0x0018 // the cancel button pressed                - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_BLANKUP               0x0019 // the blank button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_ENTERUP               0x001A // the enter button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_ENTERDOWN             0x001B // the enter button pressed                 - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_ENDUP                 0x001C // the end button                           - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_PORT_ENDDOWN               0x001D // the end button pressed                   - PPC bitmap.

#define IDB_OVERRIDE_SIMSEC_LAND_DIALUP                0x001E // a dialpad button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_DIALDOWN              0x001F // a dialpad button pressed                 - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_CLEARUP               0x0020 // the clear button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_CLEARDOWN             0x0021 // the clear button pressed                 - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_CANCELUP              0x0022 // the cancel button                        - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_CANCELDOWN            0x0023 // the cancel button pressed                - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_BLANKUP               0x0024 // the blank button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_ENTERUP               0x0025 // the enter button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_ENTERDOWN             0x0026 // the enter button pressed                 - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_ENDUP                 0x0027 // the end button                           - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_LAND_ENDDOWN               0x0028 // the end button pressed                   - PPC bitmap.

#define IDB_OVERRIDE_SIMSEC_SQRE_DIALUP                0x0029 // a dialpad button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_DIALDOWN              0x002A // a dialpad button pressed                 - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_CLEARUP               0x002B // the clear button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_CLEARDOWN             0x002C // the clear button pressed                 - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_CANCELUP              0x002D // the cancel button                        - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_CANCELDOWN            0x002E // the cancel button pressed                - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_BLANKUP               0x002F // the blank button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_ENTERUP               0x0030 // the enter button                         - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_ENTERDOWN             0x0031 // the enter button pressed                 - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_ENDUP                 0x0032 // the end button                           - PPC bitmap.
#define IDB_OVERRIDE_SIMSEC_SQRE_ENDDOWN               0x0033 // the end button pressed                   - PPC bitmap.

// Phone Icons used in the WC_STATUSICONS control.
// the default icon strips are public\apps\oak\samples\phonedialogs\statusico.bmp,
// statusico.192.bmp, and statusico.131.bmp.
#define IDB_OVERRIDE_PH_ICONS                           0x0034 //                                      - PPC bitmap.    SP icon

// SHELL32 icons
#define IDI_OVERRIDE_RADIOOFF                           0x0040 // radio off icon                       - PPC icon.
#define IDI_OVERRIDE_RADIOCONNECTED                     0x0041 // radio connected icon                 - PPC icon.
#define IDI_OVERRIDE_RADIOVOICECALL                     0x0042 // voice call icon                      - PPC icon.
#define IDI_OVERRIDE_NORADIONODATA                      0x0043 // no radio icon                        - PPC icon.
#define IDI_OVERRIDE_RADIOSEARCHING1                    0x0044 // radio searching icon 1               - PPC icon.
#define IDI_OVERRIDE_RADIOSEARCHING2                    0x0045 // radio searching icon 2               - PPC icon.
#define IDI_OVERRIDE_RADIOSEARCHING3                    0x0046 // radio searching icon 3               - PPC icon.
#define IDI_OVERRIDE_RADIONOSERVICE                     0x0047 // no radio service icon                - PPC icon.
#define IDI_OVERRIDE_NORADIOSIM                         0x0048 // the roaming icon                     - PPC icon.
#define IDI_OVERRIDE_RADIOONHOLD                        0x0049 // radio on hold icon                   - PPC icon
#define IDI_OVERRIDE_ROAMING                            0x004A // the roaming icon                     - PPC icon.
#define IDI_OVERRIDE_BTH_MODECHANGE                     0x004B // Bluetooth mode change icon           - PPC icon.
#define IDI_OVERRIDE_BLUETOOTH                          0x004C // Bluetooth icon                       - PPC icon.
#define IDI_OVERRIDE_WIFION                             0x004E // WiFi ON icon                         - PPC icon.
#define IDI_OVERRIDE_WIFIEMPTY                          0x004F // WiFi data call icon 0                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL1                      0x0050 // WiFi data call icon 1                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL2                      0x0051 // WiFi data call icon 2                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL3                      0x0052 // WiFi data call icon 3                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL4                      0x0053 // WiFi data call icon 4                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL5                      0x0054 // WiFi data call icon 5                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL6                      0x0055 // WiFi data call icon 6                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL7                      0x0056 // WiFi data call icon 7                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL8                      0x0057 // WiFi data call icon 8                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL9                      0x0058 // WiFi data call icon 9                - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL10                     0x0059 // WiFi data call icon 10               - PPC icon.
#define IDI_OVERRIDE_WIFIDATACALL11                     0x005A // WiFi data call icon 11               - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL1                0x005B // Data connectivity 1                  - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL2                0x005C // Data connectivity 2                  - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL3                0x005D // Data connectivity 3                  - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL4                0x005E // Data connectivity 4                  - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL5                0x005F // Data connectivity 5                  - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL6                0x0060 // Data connectivity 6                  - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL7                0x0061 // Data connectivity 7                  - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL8                0x0062 // Data connectivity 8                  - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL9                0x0063 // Data connectivity 9                  - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL10               0x0064 // Data connectivity 10                 - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL11               0x0065 // Data connectivity 11                 - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL12               0x0066 // Data connectivity 12                 - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL13               0x0067 // Data connectivity 13                 - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL14               0x0068 // Data connectivity 14                 - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL15               0x0069 // Data connectivity 15                 - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL16               0x006A // Data connectivity 16                 - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL17               0x006B // Data connectivity 17                 - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL18               0x006C // Data connectivity 18                 - PPC icon.
#define IDI_OVERRIDE_OTHERRADIODATACALL19               0x006D // Data connectivity 19                 - PPC icon.
#define IDI_OVERRIDE_GPRSRADIOEMPTY                     0x006E // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL1                 0x006F // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL2                 0x0070 // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL3                 0x0071 // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL4                 0x0072 // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL5                 0x0073 // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL6                 0x0074 // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL7                 0x0075 // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL8                 0x0076 // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL9                 0x0077 // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL10                0x0078 // GPRS connection                      - PPC icon.
#define IDI_OVERRIDE_GPRSRADIODATACALL11                0x0079 // GPRS connection                      - PPC icon.

// TSHRES bitmaps/icons
#define IDB_OVERRIDE_STATUS_MASKS                       0x007A // status mask in title bar             - PPC/SP bitmap.
    
// TAPRES icons
#define IDI_OVERRIDE_GRPS                               0x007B // GPRS icon                            - PPC icon.
#define IDI_OVERRIDE_1XRTT                              0x007C // 1XRTT icon                           - PPC icon.
#define IDI_OVERRIDE_SPEAKERPHONE                       0x007D // Speaker phone icon                   - PPC icon.
#define IDI_OVERRIDE_BTHEADSET                          0x007E // Bluetooth head set icon              - PPC icon.
#define IDI_OVERRIDE_MISSED_CALLS_NOTIF                 0x007F // For missed call notificaiton         - PPC icon.

// Shell32 icons (Part II)
#define IDI_OVERRIDE_BATTERY_LOW                        0x0080 // Main battery low icon                - PPC icon.
#define IDI_OVERRIDE_BATTERY_VERYLOW                    0x0081 // Main battery very low icon           - PPC icon.
#define IDI_OVERRIDE_BKUPBATTERY_LOW                    0x0082 // Backup battery low icon              - PPC icon.
#define IDI_OVERRIDE_BKUPBATTERY_VERYLOW                0x0083 // Backup battery very low icon         - PPC icon.

#define IDI_OVERRIDE_RADIOCONNECTED_I                   0x0084 // Radio signl bar icon - one bar       - PPC icon.
#define IDI_OVERRIDE_RADIOCONNECTED_II                  0x0085 // Radio signl bar icon - two bar       - PPC icon.
#define IDI_OVERRIDE_RADIOCONNECTED_III                 0x0086 // Radio signl bar icon - three bar     - PPC icon.
#define IDI_OVERRIDE_RADIOCONNECTED_IV                  0x0087 // Radio signl bar icon - four bar      - PPC icon.

// netuimdd
#define IDI_OVERRIDE_WZC_CONNECTION_BUBBLE              0x0089 // WZC Connection bubble

// TAPRES icons Part II
#define IDI_OVERRIDE_VOICEMAIL_GSM                      0x008A // GSM Voicemail
#define IDI_OVERRIDE_VOICEMAIL_CDMA                     0x008B // CDMA Voicemail

// TAPRES bitmaps in bubble
#define IDB_OVERRIDE_ROAMING                            0x008C // Roaming
#define IDB_OVERRIDE_ANALOG                             0x008D // Analog
#define IDB_OVERRIDE_CALL_FORWARDING                    0x008E // Call Forwarding
#define IDB_OVERRIDE_CALL_FORWARDING_LINE1              0x008F // Call Forwarding Line 1
#define IDB_OVERRIDE_CALL_FORWARDING_LINE2              0x0090 // Call Forwarding Line 2
#define IDB_OVERRIDE_CALL_FORWARDING_LINES12            0x0091 // Call Forwarding Lines 1,2
#define IDI_OVERRIDE_EDGE                               0x0092 // EDGE icon                            - PPC icon.
#define IDI_OVERRIDE_EVDO                               0x0093 // EVDO icon                            - PPC icon.
#define IDB_OVERRIDE_VOICE_PRIVACY_DROP                 0x0094 // Voice Privacy Drop
#define IDB_OVERRIDE_LOCATION_ON                        0x0095 // CDMA: Location Service Availble
#define IDB_OVERRIDE_LOCATION_OFF                       0x0096 // CDMA: Location Service Not Available

// SHELLRES bitmaps in bubble
#define IDB_OVERRIDE_HIGH_BLACK                         0x0097 // battery high in clock bubble
#define IDB_OVERRIDE_LOW_BLACK                          0x0098 // battery low in clock bubble
#define IDB_OVERRIDE_VERYLOW_BLACK                      0x0099 // battery very low in clock bubble
#define IDB_OVERRIDE_CHARGING_BLACK                     0x009a // battery charging in clock bubble

// SHELL32 icons (Part III)
#define IDI_OVERRIDE_RADIOVOICECALL_I                   0x009b // Voice call with signl bar icon - one bar       - PPC icon.
#define IDI_OVERRIDE_RADIOVOICECALL_II                  0x009c // Voice call with signl bar icon - two bar       - PPC icon.
#define IDI_OVERRIDE_RADIOVOICECALL_III                 0x009d // Voice call with signl bar icon - three bar     - PPC icon.
#define IDI_OVERRIDE_RADIOVOICECALL_IV                  0x009e // Voice call with signl bar icon - four bar      - PPC icon.

#define IDI_OVERRIDE_VOLUME_ON                          0x00a0 // volume on
#define IDI_OVERRIDE_VOLUME_VIBRATEONLY                 0x00a1 // volume vibrate only
#define IDI_OVERRIDE_VOLUME_OFF                         0x00a2 // volume off
#define IDI_OVERRIDE_VOLUME_SPKRPHONE                   0x00a3 // speak phone

// TSHRES bitmaps in bubble
#define IDB_OVERRIDE_BLUETOOTH_ON                       0x00a4 // blue tooth on
#define IDB_OVERRIDE_BLUETOOTH_DISCOV                   0x00a5 // blue tooth discovered

// SHELL32 icons (Part IV)
#define IDI_OVERRIDE_EDGERADIOEMPTY                     0x00a6 // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL1                 0x00a7 // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL2                 0x00a8 // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL3                 0x00a9 // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL4                 0x00aa // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL5                 0x00ab // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL6                 0x00ac // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL7                 0x00ad // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL8                 0x00ae // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL9                 0x00af // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL10                0x00b0 // EDGE connection                      - PPC icon.
#define IDI_OVERRIDE_EDGERADIODATACALL11                0x00b1 // EDGE connection                      - PPC icon.

#define IDB_OVERRIDE_ALARM_PENDING                      0x00b2 // Alarm Pending                        - PPC/SP bitmap
#define IDI_OVERRIDE_BATTERY_HIGH                       0x00b3 // battery high in taskbar
#define IDI_OVERRIDE_BATTERY_CHARGING                   0x00b4 // battery charging in taskbar

// netuimdd (part II) - this icon is for the Radios plugin
#define IDI_OVERRIDE_TODAY_RADIO_ICON                   0x00b5 // Today screen wireless plugin icon       - PPC icon

#define IDB_OVERRIDE_WRLSMGR_ICONS_OFF                  0x00b6 // WRLSMGR icons
#define IDB_OVERRIDE_WRLSMGR_ICONS                      0x00b7 // WRLSMGR icons
#define IDI_OVERRIDE_WRLSMGR_CPLICON                    0x00b8 // WRLSMGR CPL Icon                     - PPC icon

// Auxiliary Display Bitmaps
#define IDB_OVERRIDE_INCOMINGCALL                       0x00b9
#define IDB_OVERRIDE_STATUSICONS                        0x00ba
#define IDB_OVERRIDE_TICK                               0x00bb
#define IDB_OVERRIDE_OPENANIM                           0x00bc
#define IDB_OVERRIDE_CALLENDED                          0x00bd
#define IDB_OVERRIDE_VOLUME                             0x00be
#define IDB_OVERRIDE_LOCK                               0x00bf
#define IDB_OVERRIDE_CLOCK_SPECIAL_ONE                  0x00c0
#define IDB_OVERRIDE_ERROR                              0x00c1
#define IDB_OVERRIDE_APPOINTMENT                        0x00c2
#define IDB_OVERRIDE_CLOCK_DIGITS                       0x00c3
#define IDB_OVERRIDE_CLOCK_COLON                        0x00c4
#define IDB_OVERRIDE_VOICEMAIL                          0x00c5
#define IDB_OVERRIDE_MESSAGE                            0x00c6
#define IDB_OVERRIDE_MISSEDCALL                         0x00c7

//TAPRES icons part II
#define IDI_OVERRIDE_UMTS                               0x00c8  //UMTS (3G) icon
#define IDI_OVERRIDE_1XRTTRADIOEMPTY                    0x00c9            
#define IDI_OVERRIDE_1XRTTRADIODATACALL1                0x00ca
#define IDI_OVERRIDE_1XRTTRADIODATACALL2                0x00cb
#define IDI_OVERRIDE_1XRTTRADIODATACALL3                0x00cc
#define IDI_OVERRIDE_1XRTTRADIODATACALL4                0x00cd
#define IDI_OVERRIDE_1XRTTRADIODATACALL5                0x00ce
#define IDI_OVERRIDE_1XRTTRADIODATACALL6                0x00cf
#define IDI_OVERRIDE_1XRTTRADIODATACALL7                0x00d0
#define IDI_OVERRIDE_1XRTTRADIODATACALL8                0x00d1
#define IDI_OVERRIDE_1XRTTRADIODATACALL9                0x00d2
#define IDI_OVERRIDE_1XRTTRADIODATACALL10               0x00d3
#define IDI_OVERRIDE_1XRTTRADIODATACALL11               0x00d4

#define IDI_OVERRIDE_UMTSRADIOEMPTY                     0x00d5
#define IDI_OVERRIDE_UMTSRADIODATACALL1                 0x00d6
#define IDI_OVERRIDE_UMTSRADIODATACALL2                 0x00d7
#define IDI_OVERRIDE_UMTSRADIODATACALL3                 0x00d8
#define IDI_OVERRIDE_UMTSRADIODATACALL4                 0x00d9
#define IDI_OVERRIDE_UMTSRADIODATACALL5                 0x00da
#define IDI_OVERRIDE_UMTSRADIODATACALL6                 0x00db
#define IDI_OVERRIDE_UMTSRADIODATACALL7                 0x00dc
#define IDI_OVERRIDE_UMTSRADIODATACALL8                 0x00dd
#define IDI_OVERRIDE_UMTSRADIODATACALL9                 0x00de
#define IDI_OVERRIDE_UMTSRADIODATACALL10                0x00df
#define IDI_OVERRIDE_UMTSRADIODATACALL11                0x00e0


#define IDI_OVERRIDE_EVDORADIOEMPTY                     0x00e1
#define IDI_OVERRIDE_EVDORADIODATACALL1                 0x00e2
#define IDI_OVERRIDE_EVDORADIODATACALL2                 0x00e3
#define IDI_OVERRIDE_EVDORADIODATACALL3                 0x00e4
#define IDI_OVERRIDE_EVDORADIODATACALL4                 0x00e5
#define IDI_OVERRIDE_EVDORADIODATACALL5                 0x00e6
#define IDI_OVERRIDE_EVDORADIODATACALL6                 0x00e7
#define IDI_OVERRIDE_EVDORADIODATACALL7                 0x00e8
#define IDI_OVERRIDE_EVDORADIODATACALL8                 0x00e9
#define IDI_OVERRIDE_EVDORADIODATACALL9                 0x00ea
#define IDI_OVERRIDE_EVDORADIODATACALL10                0x00eb
#define IDI_OVERRIDE_EVDORADIODATACALL11                0x00ec

#define IDI_OVERRIDE_EVDVRADIOEMPTY                     0x00ed
#define IDI_OVERRIDE_EVDVRADIODATACALL1                 0x00ee
#define IDI_OVERRIDE_EVDVRADIODATACALL2                 0x00ef
#define IDI_OVERRIDE_EVDVRADIODATACALL3                 0x00f0
#define IDI_OVERRIDE_EVDVRADIODATACALL4                 0x00f1
#define IDI_OVERRIDE_EVDVRADIODATACALL5                 0x00f2
#define IDI_OVERRIDE_EVDVRADIODATACALL6                 0x00f3
#define IDI_OVERRIDE_EVDVRADIODATACALL7                 0x00f4
#define IDI_OVERRIDE_EVDVRADIODATACALL8                 0x00f5
#define IDI_OVERRIDE_EVDVRADIODATACALL9                 0x00f6
#define IDI_OVERRIDE_EVDVRADIODATACALL10                0x00f7
#define IDI_OVERRIDE_EVDVRADIODATACALL11                0x00f8

#define IDI_OVERRIDE_HSDPARADIOEMPTY                     0x00f9
#define IDI_OVERRIDE_HSDPARADIODATACALL1                 0x00fa
#define IDI_OVERRIDE_HSDPARADIODATACALL2                 0x00fb
#define IDI_OVERRIDE_HSDPARADIODATACALL3                 0x00fc
#define IDI_OVERRIDE_HSDPARADIODATACALL4                 0x00fd
#define IDI_OVERRIDE_HSDPARADIODATACALL5                 0x00fe
#define IDI_OVERRIDE_HSDPARADIODATACALL6                 0x00ff
#define IDI_OVERRIDE_HSDPARADIODATACALL7                 0x0100
#define IDI_OVERRIDE_HSDPARADIODATACALL8                 0x0101
#define IDI_OVERRIDE_HSDPARADIODATACALL9                 0x0102
#define IDI_OVERRIDE_HSDPARADIODATACALL10                0x0103
#define IDI_OVERRIDE_HSDPARADIODATACALL11                0x0104

#define IDI_OVERRIDE_1XEVDV                             0x0105
#define IDI_OVERRIDE_HSDPA                              0x0106

#define IDI_OVERRIDE_RADIO_CONNECTED_GPRS               0x0107 // Connected to GPRS - PPC icon
#define IDI_OVERRIDE_RADIO_CONNECTED_EDGE               0x0108 // Connected to EDGE PPC icon
#define IDI_OVERRIDE_RADIO_CONNECTED_1XRTT              0x0109 // Connected to 1XRTT - PPC icon
#define IDI_OVERRIDE_RADIO_CONNECTED_CSD                0x010a // Connected to CSD - PPC icon
#define IDI_OVERRIDE_RADIO_CONNECTED_EVDO               0x010b // Connected to EVDO - PPC icon
#define IDI_OVERRIDE_RADIO_CONNECTED_EVDV               0x010c // Connected to EVDV PPC icon
#define IDI_OVERRIDE_RADIO_CONNECTED_HSDPA              0x010d // Connected to HSDPA - PPC icon
#define IDI_OVERRIDE_RADIO_CONNECTED_UMTS               0x010e // Connected to UMTS - PPC icon

// SHELLRES bitmaps in bubble (Part II)
#define IDB_OVERRIDE_FULL_BLACK                         0x010f // battery full in clock bubble
#define IDB_OVERRIDE_MED_BLACK                          0x0110 // battery med in clock bubble

// Shell32 icons (Part VI)
#define IDI_OVERRIDE_BATTERY_FULL                       0x0111 // battery full in taskbar
#define IDI_OVERRIDE_BATTERY_MED                        0x0112 // battery med in taskbar

// 0x0113 - 0x0116 are used by Callhistory Roaming icons.

// More tapres icons
#define IDI_OVERRIDE_IMSIATTACHED_HSDPA                 0x0117
#define IDI_OVERRIDE_IMSIATTACHED_UMTS                  0x0118
#define IDI_OVERRIDE_IMSIATTACHED_EDGE                  0x0119
#define IDI_OVERRIDE_IMSIATTACHED_GRPS                  0x011a

// Outlook bitmaps

//Please use the IDB_OVERRIDE_CONTSCRD_ICONS bit strip to provide the override icons
//#define IDB_OVERRIDE_CONTSCRD_RADIOICONS              0x121  // image list of contact summary card radio icons (radio and txt2)
//#define IDB_OVERRIDE_CONTSCRD_CALLICONS               0x122  // image list of contact summary card last call icons

// Outlook Contact Summary Card Icons bitmap
// the default icon strips are public\apps\oak\samples\ContactSummaryCardIcons\res\,
// contscrdicons.96.bmp, contscrdicons.128.bmp, contscrdicons.131.bmp, contscrdicons.192.bmp
#define IDB_OVERRIDE_CONTSCRD_ICONS                     0x121  // image list of contact summary card icons

// DRM Mini Icons
#define IDI_OVERRIDE_DRMEXPIRED                         0x123
#define IDI_OVERRIDE_DRMNONEXPIRED                      0x124

// Smart Dial, Contacts, CallLog icons
#define IDI_OVERRIDE_CONTACTS_LEFTARROW                 0x125  // left arrow icon used in matched contacts in Smart dial, contact list and call log
#define IDI_OVERRIDE_CONTACTS_RIGHTARROW                0x126  // right arrow icon used in matched contacts in Smart dial, contact list and call log

#define ID_RESOVER_OVERRIDE_MAX                         0x0127
