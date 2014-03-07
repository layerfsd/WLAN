//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/*++

Module Name:

resource.h

Abstract:

Functions:


Notes:


--*/
// Resources that are in tshres
#include <ossvcs.h>
#include "tshres.h"
#include <commctrl.h>


#define I_IMAGENONE		(-2)
#define NOMENU 0xFFFF
#define SS_SUNKEN           0x00001000L
#ifdef __cplusplus
extern "C" {
#endif

extern HINSTANCE g_hInstResDPI;

#ifdef __cplusplus
}
#endif

#define HINST_RESDLLDPI g_hInstResDPI

// Private static control style defined in os\inc\window.hpp.
#define SS_EDITCONTROL      0x00001000L

#ifndef IDC_STATIC
#define IDC_STATIC -1
#endif

// The compiler won't let menu names be additions, so we have to set them to a number
// This makes sure that we won't build if TSHRES_BASE_SIMUI is changed.
#if (TSHRES_BASE_NETUI == 12000)
#define IDS_NETUI_DIRECT_NAME            (TSHRES_BASE_NETUI+1)
#define IDS_NETUI_ERR_TITLE                (TSHRES_BASE_NETUI+2)
#define IDS_NETUI_IPBAD_FIELD_VALUE        (TSHRES_BASE_NETUI+3)
#define    IDS_NETUI_PORT_SETTINGS            (TSHRES_BASE_NETUI+4)
#define    IDS_NETUI_CALL_OPTIONS            (TSHRES_BASE_NETUI+5)
#define IDS_NETUI_DEVICE_PROPERTIES        (TSHRES_BASE_NETUI+6)
#define IDS_NETUI_PARAMETER_NONE        (TSHRES_BASE_NETUI+7)
#define IDS_NETUI_PARAMETER_ODD         (TSHRES_BASE_NETUI+8)
#define IDS_NETUI_PARAMETER_EVEN        (TSHRES_BASE_NETUI+9)
#define IDS_NETUI_PARAMETER_MARK        (TSHRES_BASE_NETUI+10)
#define IDS_NETUI_PARAMETER_SPACE       (TSHRES_BASE_NETUI+11)
#define IDS_NETUI_PARAMETER_SOFTWARE    (TSHRES_BASE_NETUI+12)
#define IDS_NETUI_PARAMETER_HARDWARE    (TSHRES_BASE_NETUI+13)
#define IDS_NETUI_SRVLOGON_TITLE        (TSHRES_BASE_NETUI+14)
#define IDS_NETUI_NETMSGBOX_TITLE       (TSHRES_BASE_NETUI+15)
#define    IDS_NETUI_NETWORK_IPADDR        (TSHRES_BASE_NETUI+16)
#define IDS_NETUI_NETWORK_NAMESERV        (TSHRES_BASE_NETUI+17)
#define IDS_NETUI_NETWORK_IP_TITLE        (TSHRES_BASE_NETUI+18)
#define IDS_NETUI_NETWORK_ERR_BADIP        (TSHRES_BASE_NETUI+19)
#define IDS_NETUI_PCCARD_TYPE_MFC        (TSHRES_BASE_NETUI+20)
#define IDS_NETUI_PCCARD_TYPE_MEMORY    (TSHRES_BASE_NETUI+21)
#define IDS_NETUI_PCCARD_TYPE_SERIAL    (TSHRES_BASE_NETUI+22)
#define IDS_NETUI_PCCARD_TYPE_PARALLEL  (TSHRES_BASE_NETUI+23)
#define IDS_NETUI_PCCARD_TYPE_DISK        (TSHRES_BASE_NETUI+24)
#define IDS_NETUI_PCCARD_TYPE_VIDEO        (TSHRES_BASE_NETUI+25)
#define IDS_NETUI_PCCARD_TYPE_NETWORK   (TSHRES_BASE_NETUI+26)
#define IDS_NETUI_PCCARD_TYPE_AIMS        (TSHRES_BASE_NETUI+27)
#define IDS_NETUI_PCCARD_TYPE_SCSI        (TSHRES_BASE_NETUI+28)
#define IDS_NETUI_PCCARD_TYPE_UNKNOWN   (TSHRES_BASE_NETUI+29)
#define IDS_NETUI_PCCARD_SOCKET            (TSHRES_BASE_NETUI+30)

// USB string defs
#define IDS_NETUI_USB_CAPTION           (TSHRES_BASE_NETUI+31)
#define IDS_NETUI_USB_CMDTEXT           (TSHRES_BASE_NETUI+32)

#define IDS_NETUI_NOT_CONNECTED         (TSHRES_BASE_NETUI+33)

#define IDD_NETUI_IPADDR                12033
#define IDD_NETUI_PORT_SETTINGS            12034
#define IDD_NETUI_PORT_SETTINGS2        12035
#define IDD_NETUI_DIALING_SETTINGS      12036
#define IDD_NETUI_DIALING_SETTINGS2     12037
#define IDD_NETUI_ADD_CONNECTION        12038
#define IDD_NETUI_REMOVE_CONNECTION     12039
#define IDD_NETUI_RESPWD                12040
#define IDD_NETUI_DRIVERNAME            12043
#define IDD_NETUI_NETWORK_IP2            12044
#define IDD_NETUI_NETWORK_NAMESERV2     12045

#define IDC_NETUI_IPADDRLABEL            12046
#define IDC_NETUI_IP_ADDR                12047

// Some of these are shared. Some are not.
#define IDC_NETUI_CMDTEXT               12048
#define IDC_NETUI_DRIVERNAME            12049

// Additional defines for IDD_NETUI_USERPWD
#define IDC_NETUI_PWD_DLGTITLE          12050

// IDD_NETUI_PORT_SETTINGS
#define    IDC_NETUI_CHK_MANUAL            12051
#define    IDC_NETUI_CHK_PREDIAL            12052
#define IDC_NETUI_CHK_POSTDIAL            12053
#define    IDC_NETUI_BAUD_RATE                12054
#define    IDC_NETUI_DATA_BITS                12055
#define    IDC_NETUI_PARITY                12056
#define    IDC_NETUI_STOP_BITS                12057
#define    IDC_NETUI_FLOW_CONTROL            12058

// IDD_NETUI_PORT_SETTINGS - Additional defines for Rapier
#define    IDC_NETUI_CONNPROP_PP1TITLE     12059
#define    IDC_NETUI_STATIC_BAUD           12060
#define    IDC_NETUI_STATIC_BITS           12061
#define IDC_NETUI_STATIC_PARITY         12062
#define    IDC_NETUI_STATIC_STOP           12063
#define    IDC_NETUI_STATIC_FLOW           12064

// IDD_NETUI_DIAL_SETTINGS
#define    IDC_NETUI_CHK_TIMEOUT            12065
#define IDC_NETUI_TEXT_SECONDS            12066
#define    IDC_NETUI_EDIT_TIMEOUT            12067
#define IDC_NETUI_CHK_BLIND_DIAL        12068
#define IDC_NETUI_EDIT_WAITBONG            12069
#define IDC_NETUI_EXTRA                    12070

// IDD_NETUI_DIAL_SETTINGS - Additional defines for Rapier
#define IDC_NETUI_CONNPROP_PP2TITLE     12071
#define IDC_NETUI_CARD_STATIC           12072
#define IDC_NETUI_SEC_STATIC            12073
#define IDC_NETUI_EXTRA_STATIC          12074

// IDD_NETUI_ADD_CONNECTION
#define IDC_NETUI_LOCALNAME             12075
#define IDC_NETUI_REMOTENAME            12076
#define IDC_NETUI_REMOTE_STATIC         12077


// IDD_NETUI_REMOVE_CONNECTION
#define IDC_NETUI_DISCONLIST            12078
#define IDC_NETUI_NETCONN_STATIC        12079

// IDD_NETUI_NETWORK_IP2
#define IDC_NETUI_ADAPTERNAME            12080
#define IDC_NETUI_USEDHCP                12081
#define IDC_NETUI_DONTUSEDHCP            12082
#define    IDC_NETUI_SUBNETLABEL             12083
#define IDC_NETUI_SUBNET_MASK            12084
#define IDC_NETUI_GATEWAYLABEL            12085
#define IDC_NETUI_GATEWAY                 12086
#define IDC_NETUI_DNS_LABEL                12087
#define IDC_NETUI_DNS_ADDR                12088
#define IDC_NETUI_DNS_ALTLABEL          12089
#define IDC_NETUI_DNS_ALTADDR            12090
#define IDC_NETUI_WINS_LABEL              12091
#define IDC_NETUI_WINS_ADDR                12092
#define IDC_NETUI_WINS_ALTLABEL            12093
#define IDC_NETUI_WINS_ALTADDR            12094
#define IDC_NETUI_NETADAPT_PP1TITLE     12095
#define IDC_NETUI_NETADAPT_PP2TITLE     12096
#define IDC_NETUI_AUTOMSG_STATIC        12097
#define IDC_NETUI_IP_ADDR_CUR           14000

// free                                        12098 .. 12133

#define IDS_NETUI_WRN_TITLE_WARNING            12134

// free                                        12135 .. 12144

#define IDC_NETUI_LOCAL_STATIC                 12145

// free                                        12146 .. 12157

#define IDS_NETUI_RESFMT_STR                   12158

#define IDC_NETUI_USER_STATIC                  12159
#define IDC_NETUI_PASSWRD_STATIC               12160
#define IDC_NETUI_DOMAIN_STATIC                12161
#define IDC_NETUI_USERNAME                     12162
#define IDC_NETUI_PASSWORD                     12163
#define IDC_NETUI_DOMAIN                       12164
#define IDC_NETUI_SAVEPASSWORD                 12165
#define IDC_NETUI_CANCEL_PSWD                  12166
#define IDC_NETUI_OK_PSWD                      12167
#define IDD_NETUI_USERPWD                       12168
#define IDR_NETUI_BAR_DIALOGOKCANCEL           12169

#define IDS_NETUI_ALLOWFAILOVER_FMT_STR        12170
#define IDD_NETUI_USERNAME                     12171
#define IDD_NETUI_REALM_AUTH                   12172
#define IDS_NETUI_SRVLOGON_AUTH_FAIL_TITLE     12173
#define IDS_NETUI_SRVLOGON_CHK_STR             12174
#define IDS_NETUI_WININET_PROXY                12175
#define IDS_NETUI_WININET_SITE                 12176
#define IDD_NETUI_NTLM_AUTH                    12177
#define IDR_NETUI_CERT_DIALOGOKCANCEL          12178

//IDD_NETUI_REALM_AUTH 
#define IDC_NETUI_SITE_OR_FIREWALL             12180
#define IDC_NETUI_SERVER_OR_PROXY              12181
#define IDC_NETUI_REALM_TAG                    12182
#define IDC_NETUI_REALM_FIELD                  12183

// Site and Proxy Details
#define IDR_NETUI_DETAILS_MENU                 12184
#define IDR_NETUI_DETAILS_BAR                  12185
#define IDM_NETUI_DETAILS                      12186
#define IDS_NETUI_WININET_PROXY_NAME           12187
#define IDS_NETUI_WININET_SITE_NAME            12188
#define IDS_NETUI_DETAILS                      12189
#define IDS_NETUI_MENU                         12190
#define IDD_NETUI_DETAILS                      12191
#define IDR_NETUI_DONE_BAR                     12192

#define IDR_NETUI_CANCEL_BAR                   12193

// Resource IDs
#define GETNETSTR_BASE                            12200
#define IDS_NETUI_GETNETSTR_DIRECT_NAME              (GETNETSTR_BASE + NETUI_GETNETSTR_DIRECT_NAME)
#define IDS_NETUI_GETNETSTR_OPEN_FILE_CONF        (GETNETSTR_BASE + NETUI_GETNETSTR_OPEN_FILE_CONF)
#define IDS_NETUI_GETNETSTR_BAD_RES_NAME          (GETNETSTR_BASE + NETUI_GETNETSTR_BAD_RES_NAME)
#define IDS_NETUI_GETNETSTR_ERR_ADDCONNECT        (GETNETSTR_BASE + NETUI_GETNETSTR_ERR_ADDCONNECT)
#define IDS_NETUI_GETNETSTR_ERR_INV_COMPNAME      (GETNETSTR_BASE + NETUI_GETNETSTR_ERR_INV_COMPNAME)
#define IDS_NETUI_GETNETSTR_ERR_ADDNAME           (GETNETSTR_BASE + NETUI_GETNETSTR_ERR_ADDNAME)
#define IDS_NETUI_GETNETSTR_DISCON_ERROR          (GETNETSTR_BASE + NETUI_GETNETSTR_DISCON_ERROR)
#define IDS_NETUI_GETNETSTR_SAVE_PWD              (GETNETSTR_BASE + NETUI_GETNETSTR_SAVE_PWD)
#define IDS_NETUI_GETNETSTR_DFLT_LOCALNAME        (GETNETSTR_BASE + NETUI_GETNETSTR_DFLT_LOCALNAME)
#define IDS_NETUI_GETNETSTR_NO_IPADDR              (GETNETSTR_BASE + NETUI_GETNETSTR_NO_IPADDR)
#define IDS_NETUI_GETNETSTR_LEASE_EXPIRED          (GETNETSTR_BASE + NETUI_GETNETSTR_LEASE_EXPIRED)
#define IDS_NETUI_GETNETSTR_CACHED_LEASE          (GETNETSTR_BASE + NETUI_GETNETSTR_CACHED_LEASE)
#define IDS_NETUI_GETNETSTR_PRINT_COMPLETE        (GETNETSTR_BASE + NETUI_GETNETSTR_PRINT_COMPLETE)
#define IDS_NETUI_GETNETSTR_PRINT_STATUS          (GETNETSTR_BASE + NETUI_GETNETSTR_PRINT_STATUS)
#define IDS_NETUI_GETNETSTR_PRINT_STATUS_OFFLINE  (GETNETSTR_BASE + NETUI_GETNETSTR_PRINT_STATUS_OFFLINE)
#define IDS_NETUI_GETNETSTR_PRINT_STATUS_PAPER    (GETNETSTR_BASE + NETUI_GETNETSTR_PRINT_STATUS_PAPER)
#define IDS_NETUI_GETNETSTR_PRINT_STATUS_OTHER    (GETNETSTR_BASE + NETUI_GETNETSTR_PRINT_STATUS_OTHER)
#define IDS_NETUI_GETNETSTR_PRINT_STATUS_NETERR   (GETNETSTR_BASE + NETUI_GETNETSTR_PRINT_STATUS_NETERR)
#define IDS_NETUI_GETNETSTR_USB_INSTALL_FAILURE   (GETNETSTR_BASE + NETUI_GETNETSTR_USB_INSTALL_FAILURE)

#define IDS_NETUI_NETCARDS_SETTING_TITLE               12250
#define IDD_NETUI_ADAPTERS                             12251
#define IDC_NETUI_COMBO_ADAPTER_FILTER                 12252
#define IDC_NETUI_NETCPL0_ADAPTERLIST                  12253
#define IDS_NETUI_ADAPTERS_PAGE_TITLE                  12254
#define IDC_NETUI_STATIC_ADAPTERS                      12255
#define IDC_NETUI_STATIC_NETCARD_CONNECT_TO            12256
#define IDS_NETUI_ADAPTERCHG                           12257
#define IDS_NETUI_ADAPTERS                             12258

// Wireless zero config
#define IDD_WZCNOTIF                                   12260
#define IDI_WZC_CONNECTION_BUBBLE                      12261
#define IDS_WZC_SETTING_TITLE                          12262
#define IDC_WZC_HEADING                                12263
#define IDB_NETUI_HELPICON                             12264

#define IDS_WZC_CONNECTION_BUBBLE_SINGLENET_TITLE      12272
#define IDS_WZC_CONNECTION_BUBBLE_MULTIPLENET_TITLE    12274
#define IDS_WZC_CONNECTION_BUBBLE_VPN                  12275
#define IDS_WZC_WEP_BUBBLE_TITLE                       12278
#define IDS_WZC_WEP_BUBBLE_RETRY_TITLE                 12280
#define IDS_WZC_INVALID_CONFIG                         12281
#define IDC_WZC_STATIC_WIRELESS_NETWORK                12286
#define IDC_WZC_STATIC_NETWORK_TO_ACCESS               12287
#define IDM_WZC_COMMAND_SETTING                        12288  // MUST be 12288 since HTML resources use this value
#define IDM_WZC_COMMAND_CANCEL                         12289

#define IDD_WZC_WLAN_NETWORK                           12290  // Wireless setting dialog
#define IDC_WZC_LIST_NETWORKS                          12291  // Wireless network list
#define IDC_WZC_COMBO_FILTER                           12295  // Wireless network filter for network list
#define IDS_WZC_FILTER_ALL_AVAILABLE                   12296
#define IDS_WZC_FILTER_NON_ADHOC                       12297
#define IDS_WZC_FILTER_ADHOC                           12298

#define IDS_WZC_WIFI_STATUS_FORMAT                     12299

#define IDS_WZC_ADD_NEW_SETTINGS                       12300
#define IDS_WZC_BLANK_SSID                             12301
#define IDB_WZC_STATE                                  12302
#define IDS_WZC_WLAN_NETWORK_PAGE_TITLE                12303
#define IDR_WZC_MENU_NETWORK                           12304
#define IDM_WZC_NETWORK_CONNECT                        12305
#define IDM_WZC_NETWORK_REMOVE                         12306
#define IDM_WZC_NETWORK_PROPERTIES                     12307
#define IDS_WZC_MSG_TITLE                              12308
#define IDS_WZC_NULL_SSID                              12309
#define IDS_WZC_DUP_SSID                               12310
#define IDS_WZC_INCORRECT_WEP_KEY                      12311
#define IDS_WZC_INVALID_NAME                           12312
#define IDC_WZC_STATIC_NO_WIRELESS_CARD_PRESEND        12313
#define IDS_WZC_BAD_SSID                               12314


#define IDS_WZC_STATUS_FIRST                           12315
#define IDS_WZC_ACTIVE                                 12315
#define IDS_WZC_AIRING                                 12316
#define IDS_WZC_SILENT                                 12317
#define IDS_WZC_CONNECTING                             12318
#define IDS_WZC_STATUS_LAST                            12318

#define IDS_WZC_SEARCH_NETWORKS                        12319

#define IDD_WZC_WLAN_GENERAL_PROP_PAGE                 12320
#define IDS_WZC_WLAN_GENERAL_PROP_PAGE_TITLE           12321
#define IDC_WZC_EDIT_SSID                              12322
#define IDC_WZC_COMBO_CONNECT_TO                       12323
#define IDC_WZC_CHECK_ADHOC                            12324
#define IDC_WZC_STATIC_NETWORK_NAME                    12325
#define IDC_WZC_STATIC_CONNECT_TO                      12326
#define IDC_WZC_STATIC_INFO_VPN                        12327
#define IDC_WZC_CHECK_HIDDEN                           12328

#define IDD_WZC_WLAN_AUTHENTICATION_PAGE               12330
#define IDS_WZC_WLAN_AUTHENTICATION_PAGE_TITLE         12331
//#define IDC_WZC_CHECK_ENABLED_WEP                      12332
//#define IDC_WZC_CHECK_AUTHENTICATION                   12333
#define IDC_WZC_CHECK_KEY_PROVIDED_AUTOMATICALLY       12334
#define IDC_STATIC_EDIT_WEP                            12335
#define IDC_WZC_EDIT_WEP                               12336
#define IDC_STATIC_WEP_INDEX                           12337 
#define IDC_WZC_COMBO_WEP_INDEX                        12338

#define IDC_WZC_COMBO_EAP_TYPE                         12339
#define IDC_WZC_CHECK_ENABLE8021X                      12340
#define IDC_WZC_STATIC_ENABLE8021X                     12341
#define IDC_WZC_BUTTON_EAP_PROPERTIES                  12342
#define IDS_WZC_WLAN_8021X_PAGE_TITLE                  12343
#define IDD_WZC_WLAN_8021X_PAGE                        12344

#define IDC_STATIC_AUTHENTICATION                      12345 
#define IDC_WZC_COMBO_AUTHENTICATION                   12346
#define IDC_STATIC_DATA_ENCRYPTION                     12347 
#define IDC_WZC_COMBO_DATA_ENCRYPTION                  12348

#define IDD_PICKCERT                                   12350
#define IDC_CERTLISTVIEW                               12351
#define IDC_CERTLISTVIEW_UPDOWN                        12352
#define IDC_BTN_CERTVIEW                               12353
#define IDS_NETUI_EAPTLS_NO_CERT                       12355
#define IDS_NETUI_EAPTLS_DESC                          12356

#define IDS_WZC_CONNECTION_BUBBLE_CONNECT_BUTTON       12357
#define IDS_WZC_CONNECTION_BUBBLE_DISMISS_BUTTON       12358

#define IDD_CERTDETAILS                                12360
#define IDC_CERTS_ISSUEDTO                             12361
#define IDC_CERTS_ISSUEDBY                             12362
#define IDC_CERTS_VALIDFROM                            12363
#define IDC_CERTS_VALIDTO                              12364
#define IDC_CERTS_USAGE                                12365
#define IDS_NETUI_SELFISSUED                           12366
#define IDC_STATIC_CERTS_ISSUEDTO                      12367
#define IDC_STATIC_CERTS_ISSUEDBY                      12368
#define IDC_STATIC_CERTS_VALIDFROM                     12369
#define IDC_STATIC_CERTS_VALIDTO                       12370
#define IDC_STATIC_CERTS_USAGE                         12371

#define IDS_WZC_DHCP_WARNING                           12380
#define IDS_ALL_PURPOSES                               12381

#define IDS_NIC_INSERTION_TITLE                        12390
#define IDS_NIC_DESCRIPTION                            12391

#define IDS_NETUI_SERVER_VALIDATION_ERROR              12400
#define IDS_NETUI_UNKNOWN_ROOT_ERROR                   12401
#define IDS_NETUI_CERT_EXPIRED_ERROR                   12402
#define IDS_NETUI_UNKNOWN_CERT_ERROR                   12403
#define IDS_NETUI_GENERIC_CERT_ERROR                   12404

#define IDD_WZC_WLAN_ADVANCED                          12405
#define IDC_WZC_STATIC_AUTO_OFF                        12406
#define IDC_WZC_COMBO_AUTO_OFF                         12407
#define IDC_WZC_CHECK_NOTIFICATION                     12408

#define IDS_WZC_AUTO_OFF_NEVER                         12410
#define IDS_WZC_AUTO_OFF_30SEC                         12411
#define IDS_WZC_AUTO_OFF_01MIN                         12412
#define IDS_WZC_AUTO_OFF_05MIN                         12413
#define IDS_WZC_AUTO_OFF_10MIN                         12414
#define IDS_WZC_AUTO_OFF_30MIN                         12415

#define IDS_WZC_AUTO_OFF_FIRST                         IDS_WZC_AUTO_OFF_NEVER
#define IDS_WZC_AUTO_OFF_LAST                          IDS_WZC_AUTO_OFF_30MIN

#define INETRESOURCE_BASE   IDS_NETUI_GENERIC_CERT_ERROR
#include "inetresource.h"

// Wireless Zero Config Strings
#define IDS_WZC_AUTHENTICATION_OPEN                    12500
#define IDS_WZC_AUTHENTICATION_SHARED                  12501
#define IDS_WZC_AUTHENTICATION_WPA                     12502
#define IDS_WZC_AUTHENTICATION_WPA_PSK                 12503
#define IDS_WZC_AUTHENTICATION_WPA_NONE                12504

#define IDS_WZC_ENCRYPTION_DISABLED                    12505
#define IDS_WZC_ENCRYPTION_WEP                         12506
#define IDS_WZC_ENCRYPTION_TKIP                        12507
#define IDS_WZC_ENCRYPTION_AES                         12508

#define IDS_WZC_AUTHENTICATION_WPA2                    12509
#define IDS_WZC_AUTHENTICATION_WPA2_PSK                12510

#define IDS_WZC_NETKEY_BUBBLE_WPA_TITLE                12600
#define IDS_WZC_NETKEY_BUBBLE_WEP_TITLE                12601
#define IDS_WZC_NETKEY_BUBBLE_RETRY_TITLE              12602
#define IDS_WZC_NETKEY_BUBBLE_WPA_LABEL                12603
#define IDS_WZC_NETKEY_BUBBLE_WEP_LABEL                12604
#define IDS_WZC_NETKEY_BUBBLE_WPA                      12605
#define IDS_WZC_NETKEY_BUBBLE_WEP                      12606
#define IDS_WZC_NETKEY_BUBBLE_WPAPSK_ERR               12607
#define IDS_WZC_NETKEY_BUBBLE_WEP_ERR                  12608
#define IDS_WZC_NETKEY_BUBBLE_RETRY                    12609
#define IDS_WZC_NETKEY_BUBBLE_RETRY_LABEL              12610

#define IDS_CONNECT                                    12652
#define IDR_NEWNET_DIALOGYESNO                         12653
#define IDR_WZC_SINGLE_NEWNET_NOTIFICATION_MENU        12654
#define IDR_WZC_MULTI_NEWNET_NOTIFICATION_MENU         12655
#define IDM_WZC_NEWNET_DISMISS                         12656
#define IDM_WZC_NEWNET_STOP_NOTIFICATIONS              12657
#define IDM_WZC_BUBBLE_CONNECT                         12658
#define IDM_WZC_BUBBLE_SELECT                          12659

#define IDS_WZC_NETSEL_TITLE                           12660
#define IDS_WZC_NETSEL_LABEL0                          12661
#define IDS_WZC_NETSEL_LABEL1                          12662
#define IDS_WZC_NETSEL_LABEL2                          12663
#define IDS_WZC_META_INTERNET                          12664
#define IDS_WZC_META_WORK                              12665
#define IDS_WZC_NETSEL_FORMAT_SECURE_NETWORK           12666
#define IDS_WZC_NETSEL_FORMAT_OPEN_NETWORK             12667

#define IDS_WZC_CREDUI_TITLE                           12670
#define IDS_WZC_CREDUI_RESOURCE                        12671
#define IDS_WZC_CREDUI_USERNAME                        12672
#define IDS_WZC_CREDUI_PASSWORD                        12673
#define IDS_WZC_CREDUI_DOMAIN                          12674
#define IDS_WZC_CREDUI_SAVEPWD                         12675

#define IDS_WZC_WIFI_POWERON                           12676
#define IDS_WZC_WIFI_POWEROFF                          12677
#define IDM_WZC_WIFI_POWERCMD                          IDS_WZC_WIFI_POWERON

#define IDS_WZC_WIFI_SEARCH_ERROR                      12678

// Wireless Zero Config Dialogs and Controls
#define IDD_WZC_BLANK                                  12500
#define IDR_WZC_SKBAR_CONNECTCANCEL                    12501
#define ID_WZC_CONNECT                                 12502
#define IDR_WZC_SKBAR_DONECANCEL                       12503
#define IDD_WZC_NEWNET_ALERT                           12504
#define IDC_WZC_NEWNET_ALERT_TEXT                      12505
#define IDC_WZC_CHECK_CANCEL_NOTIFY                    12506
#define IDC_WZC_NEWNET_TITLE                           12507
#define IDC_WZC_NEWNET_LINE                            12508
#define IDR_WZC_SKBAR_NETWORK                          12509
#define IDR_WZC_SKBAR_EDIT                             12510
#define IDM_WZC_EDIT                                   12511

#define IDM_WZC_ADVANCED                               12512

#define IDM_WZC_MENU_EXT_FIRST                         12520
// reserve 10
#define IDM_WZC_MENU_EXT_LAST                          12530

// Wireless Zero Config Images and Icons
#define IDI_WZC_STATUS_WIFI_CONNECTED                  12500
#define IDI_WZC_STATUS_LAN_CONNECTED                   12501

// Radio plugin
#define IDI_TODAY_RADIO                                12502

// CertUI stuff
#define IDC_CERTS_STATIC_ISSUEDTO                      12700
#define IDC_CERTS_STATIC_ISSUEDTO_OUT                  12701
#define IDC_CERTS_STATIC_ISSUEDBY                      12702
#define IDC_CERTS_STATIC_ISSUEDBY_OUT                  12703
#define IDC_CERTS_STATIC_VALIDFROM                     12704
#define IDC_CERTS_STATIC_VALIDFROM_OUT                 12705
#define IDC_CERTS_STATIC_VALIDTO                       12706
#define IDC_CERTS_STATIC_VALIDTO_OUT                   12707
#define IDC_CERTS_STATIC_TEMPLATE                      12708
#define IDC_CERTS_TEMPLATE_NAME                        12709

#define IDR_CERTDETAILS_CMDBAR                         12710

#endif

