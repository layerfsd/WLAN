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

#ifndef _connmgr_conntypes_h_
#define _connmgr_conntypes_h_

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Connection types | Describes possible connection types and subtypes
//
// @comm none
//
// -----------------------------------------------------------------------------

#define CM_CONNTYPE_UNKNOWN     0
#define CM_CONNTYPE_CELLULAR    1
#define CM_CONNTYPE_NIC         2
#define CM_CONNTYPE_BLUETOOTH   3
#define CM_CONNTYPE_UNIMODEM    4
#define CM_CONNTYPE_VPN         5
#define CM_CONNTYPE_PROXY       6
#define CM_CONNTYPE_PC          7
#define CM_CONNTYPE_MAX         8

#define CM_CONNSUBTYPE_UNKNOWN  0

#define CM_CONNSUBTYPE_CELLULAR_UNKNOWN 0
#define CM_CONNSUBTYPE_CELLULAR_CSD     1
#define CM_CONNSUBTYPE_CELLULAR_GPRS    2
#define CM_CONNSUBTYPE_CELLULAR_1XRTT   3   // @constdefine Not distinct from CSD.
#define CM_CONNSUBTYPE_CELLULAR_1XEVDO  4   // @constdefine Not distinct from CSD.
#define CM_CONNSUBTYPE_CELLULAR_1XEVDV  5   // @constdefine Not distinct from CSD.
#define CM_CONNSUBTYPE_CELLULAR_EDGE    6   // @constdefine Not distinct from GPRS.
#define CM_CONNSUBTYPE_CELLULAR_UMTS    7   // @constdefine Not distinct from GPRS.
#define CM_CONNSUBTYPE_CELLULAR_VOICE   8
#define CM_CONNSUBTYPE_CELLULAR_PTT     9   // @constdefine Push-to-Talk, not supported.
#define CM_CONNSUBTYPE_CELLULAR_HSDPA   10  // @constdefine High-Speed Downlink Packet Access (3.5G).
#define CM_CONNSUBTYPE_CELLULAR_MAX     11

#define CM_CONNSUBTYPE_NIC_UNKNOWN  0
#define CM_CONNSUBTYPE_NIC_ETHERNET 1
#define CM_CONNSUBTYPE_NIC_WIFI     2
#define CM_CONNSUBTYPE_NIC_MAX      3

#define CM_CONNSUBTYPE_BLUETOOTH_UNKNOWN    0
#define CM_CONNSUBTYPE_BLUETOOTH_RAS        1
#define CM_CONNSUBTYPE_BLUETOOTH_PAN        2
#define CM_CONNSUBTYPE_BLUETOOTH_MAX        3

#define CM_CONNSUBTYPE_UNIMODEM_UNKNOWN         0
#define CM_CONNSUBTYPE_UNIMODEM_CSD             1
#define CM_CONNSUBTYPE_UNIMODEM_OOB_CSD         2
//
// Derived from unimodem device types
//
#define CM_CONNSUBTYPE_UNIMODEM_NULL_MODEM      3   // Direct Cable Connect (DCC)
#define CM_CONNSUBTYPE_UNIMODEM_EXTERNAL_MODEM  4   // Serial port attached modem
#define CM_CONNSUBTYPE_UNIMODEM_INTERNAL_MODEM  5
#define CM_CONNSUBTYPE_UNIMODEM_PCMCIA_MODEM    6
#define CM_CONNSUBTYPE_UNIMODEM_IRCOMM_MODEM    7   // DCC over Irda
#define CM_CONNSUBTYPE_UNIMODEM_DYNAMIC_MODEM   8   // Bluetooth modem
#define CM_CONNSUBTYPE_UNIMODEM_DYNAMIC_PORT    9   // DCC over Bluetooth
#define CM_CONNSUBTYPE_UNIMODEM_MAX             10


#define CM_CONNSUBTYPE_VPN_UNKNOWN  0
#define CM_CONNSUBTYPE_VPN_L2TP     1
#define CM_CONNSUBTYPE_VPN_PPTP     2
#define CM_CONNSUBTYPE_VPN_MAX      3

#define CM_CONNSUBTYPE_PROXY_UNKNOWN    0
#define CM_CONNSUBTYPE_PROXY_NULL       1
#define CM_CONNSUBTYPE_PROXY_HTTP       2
#define CM_CONNSUBTYPE_PROXY_WAP        3
#define CM_CONNSUBTYPE_PROXY_SOCKS4     4
#define CM_CONNSUBTYPE_PROXY_SOCKS5     5
#define CM_CONNSUBTYPE_PROXY_MAX        6

#define CM_CONNSUBTYPE_PC_UNKNOWN            0
#define CM_CONNSUBTYPE_PC_DESKTOPPASSTHROUGH 1
#define CM_CONNSUBTYPE_PC_IR                 2
#define CM_CONNSUBTYPE_PC_MODEM_LINK         3
#define CM_CONNSUBTYPE_PC_MAX                4

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Connection options | Describes possible connection options
//
// @comm none
//
// -----------------------------------------------------------------------------

#define CM_DSF_BILLBYTIME       0x00000001  // @constdefine Connection is billed by time.
#define CM_DSF_ALWAYSON         0x00000002  // @constdefine Connection is always on.
#define CM_DSF_SUSPENDRESUME    0x00000004  // @constdefine Connection is suspend/resume capable.

#endif // _connmgr_conntypes_h_
