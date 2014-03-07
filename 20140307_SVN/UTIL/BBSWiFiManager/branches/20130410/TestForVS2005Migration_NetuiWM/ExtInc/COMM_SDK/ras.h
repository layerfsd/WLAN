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
/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:

ras.h

Abstract:

Remote Access Service structures and defines

Notes: 

--*/

// @CESYSGEN IF CE_MODULES_PPP
#ifndef _RAS_H_
#define _RAS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNLEN
#include <lmcons.h>
#endif
#include <tapi.h>

#include "pshpack4.h"

#define RAS_MaxEntryName      20
#define RAS_MaxDeviceName     128
#define RAS_MaxDeviceType     16
#define RAS_MaxParamKey       32
#define RAS_MaxParamValue     128
#define RAS_MaxPhoneNumber    128
#define RAS_MaxCallbackNumber 48
#define RAS_MaxIpAddress      15
#define RAS_MaxIpxAddress     21

// Ras extensions

#define	RAS_MaxAreaCode			(10)
#define	RAS_MaxPadType			(32)
#define	RAS_MaxX25Address		(200)
#define	RAS_MaxFacilities		(200)
#define	RAS_MaxUserData			(200)

// RAS IP Address

typedef	struct	tagRasIpAddr
{
	BYTE	a;
	BYTE	b;
	BYTE	c;
	BYTE	d;
}
RASIPADDR;

// RAS Entry Definition

#define RASENTRYW struct tagRASENTRYW
RASENTRYW
{
	DWORD		dwSize;
	DWORD		dwfOptions;
	DWORD		dwCountryID;
	DWORD		dwCountryCode;
	WCHAR		szAreaCode[ RAS_MaxAreaCode + 1 ];
	WCHAR		szLocalPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
	DWORD		dwAlternateOffset;
	RASIPADDR	ipaddr;
	RASIPADDR	ipaddrDns;
	RASIPADDR	ipaddrDnsAlt;
	RASIPADDR	ipaddrWins;
	RASIPADDR	ipaddrWinsAlt;
	DWORD		dwFrameSize;
	DWORD		dwfNetProtocols;
	DWORD		dwFramingProtocol;
	WCHAR		szScript[ MAX_PATH ];
	WCHAR		szAutodialDll[ MAX_PATH ];
	WCHAR		szAutodialFunc[ MAX_PATH ];
	WCHAR		szDeviceType[ RAS_MaxDeviceType + 1 ];
	WCHAR		szDeviceName[ RAS_MaxDeviceName + 1 ];
	WCHAR		szX25PadType[ RAS_MaxPadType + 1 ];
	WCHAR		szX25Address[ RAS_MaxX25Address + 1 ];
	WCHAR		szX25Facilities[ RAS_MaxFacilities + 1 ];
	WCHAR		szX25UserData[ RAS_MaxUserData + 1 ];
	DWORD		dwChannels;
	DWORD		dwReserved1;
	DWORD		dwReserved2;

	DWORD		dwCustomAuthKey; // EAP extension type to use
};

#define RASENTRY RASENTRYW

#define LPRASENTRYW RASENTRYW*
#define LPRASENTRY  RASENTRY*

/* Describes Country Information
*/
#define RASCTRYINFO struct RASCTRYINFO
RASCTRYINFO
{
	DWORD       dwSize;
	DWORD       dwCountryID;
	DWORD       dwNextCountryID;
	DWORD       dwCountryCode;
	DWORD       dwCountryNameOffset;
};
typedef RASCTRYINFO * LPRASCTRYINFO;

#define RASCTRYINFOW   RASCTRYINFO
#define LPRASCTRYINFOW RASCTRYINFOW*
#define LPRASCTRYINFO  RASCTRYINFO*

/* Describes RAS Device Information
*/
#define RASDEVINFOW struct tagRASDEVINFOW
RASDEVINFOW
{
	DWORD       dwSize;
	WCHAR       szDeviceType[RAS_MaxDeviceType+1];
	WCHAR       szDeviceName[RAS_MaxDeviceName+1];
};

typedef RASDEVINFOW * LPRASDEVINFOW;

#define RASDEVINFO		RASDEVINFOW
#define LPRASDEVINFOW	RASDEVINFOW*
#define LPRASDEVINFO	RASDEVINFO*

/* RASENTRY 'dwfOptions' bit flags.
*/
#define RASEO_UseCountryAndAreaCodes  0x00000001
#define RASEO_SpecificIpAddr          0x00000002
#define RASEO_SpecificNameServers     0x00000004
#define RASEO_IpHeaderCompression     0x00000008
#define RASEO_RemoteDefaultGateway    0x00000010
#define RASEO_DisableLcpExtensions    0x00000020
#define RASEO_TerminalBeforeDial      0x00000040
#define RASEO_TerminalAfterDial       0x00000080
#define RASEO_ModemLights             0x00000100
#define RASEO_SwCompression           0x00000200
#define RASEO_RequireEncryptedPw      0x00000400
#define RASEO_RequireMsEncryptedPw    0x00000800
#define RASEO_RequireDataEncryption   0x00001000
#define RASEO_NetworkLogon            0x00002000
#define RASEO_UseLogonCredentials     0x00004000
#define RASEO_PromoteAlternates       0x00008000
#define RASEO_SecureLocalFiles        0x00010000
#define RASEO_DialAsLocalCall         0x00020000

#define RASEO_ProhibitPAP             0x00040000
#define RASEO_ProhibitCHAP            0x00080000
#define RASEO_ProhibitMsCHAP          0x00100000
#define RASEO_ProhibitMsCHAP2         0x00200000
#define RASEO_ProhibitEAP             0x00400000
#define RASEO_PreviewUserPw           0x01000000
#define RASEO_NoUserPwRetryDialog     0x02000000
#define RASEO_CustomScript            0x80000000


/* RASENTRY 'dwfNetProtocols' bit flags. (session negotiated protocols)
*/
#define RASNP_NetBEUI       0x00000001  // Negotiate NetBEUI
#define RASNP_Ipx           0x00000002  // Negotiate IPX
#define RASNP_Ip            0x00000004  // Negotiate TCP/IP


/* RASENTRY 'dwFramingProtocols' (framing protocols used by the server)
*/
#define RASFP_Ppp           0x00000001  // Point-to-Point Protocol (PPP)
#define RASFP_Slip          0x00000002  // Serial Line Internet Protocol (SLIP)
#define RASFP_Ras           0x00000004  // Microsoft proprietary protocol


/* RASENTRY 'szDeviceType' strings
*/
#define RASDT_Direct        TEXT("direct")    // Direct Connect (WINCE Extension)
#define RASDT_Modem         TEXT("modem")     // Modem
#define RASDT_Isdn          TEXT("isdn")      // ISDN
#define RASDT_X25           TEXT("x25")       // X.25
#define RASDT_Vpn           TEXT("vpn")		  // PPTP
#define RASDT_PPPoE         TEXT("PPPoE")     // PPPoE


DECLARE_HANDLE( HRASCONN );
typedef HRASCONN *LPHRASCONN;

// Identifies an active RAS connection.  (See RasEnumConnections)

#define RASCONNW struct tagRASCONNW
RASCONNW
{
    DWORD    dwSize;
    HRASCONN hrasconn;
    WCHAR    szEntryName[ RAS_MaxEntryName + 1 ];
};

#define RASCONN RASCONNW

#define LPRASCONNW RASCONNW*
#define LPRASCONN  RASCONN*


// Enumerates intermediate states to a connection.  (See RasDial)

#define RASCS_PAUSED 0x1000
#define RASCS_DONE   0x2000

typedef enum tagRASCONNSTATE
{
    RASCS_OpenPort = 0,
    RASCS_PortOpened,
    RASCS_ConnectDevice,
    RASCS_DeviceConnected,
    RASCS_AllDevicesConnected,
    RASCS_Authenticate,
    RASCS_AuthNotify,
    RASCS_AuthRetry,
    RASCS_AuthCallback,
    RASCS_AuthChangePassword,
    RASCS_AuthProject,
    RASCS_AuthLinkSpeed,
    RASCS_AuthAck,
    RASCS_ReAuthenticate,
    RASCS_Authenticated,
    RASCS_PrepareForCallback,
    RASCS_WaitForModemReset,
    RASCS_WaitForCallback,
    RASCS_Projected,

    RASCS_Interactive 			= RASCS_PAUSED,
    RASCS_RetryAuthentication,
    RASCS_CallbackSetByCaller,
    RASCS_PasswordExpired,

    RASCS_Connected 			= RASCS_DONE,
    RASCS_Disconnected
} 
RASCONNSTATE, *LPRASCONNSTATE;


// Describes the status of a RAS connection.  (See RasConnectionStatus)

#define RASCONNSTATUSW struct tagRASCONNSTATUSW
RASCONNSTATUSW
{
    DWORD        dwSize;
    RASCONNSTATE rasconnstate;
    DWORD        dwError;
    WCHAR        szDeviceType[ RAS_MaxDeviceType + 1 ];
    WCHAR        szDeviceName[ RAS_MaxDeviceName + 1 ];
};

#define RASCONNSTATUS RASCONNSTATUSW

#define LPRASCONNSTATUSW RASCONNSTATUSW*
#define LPRASCONNSTATUS  RASCONNSTATUS*

// Describes connection establishment parameters.  (See RasDial)

#define RASDIALPARAMSW struct tagRASDIALPARAMSW
RASDIALPARAMSW
{
    DWORD dwSize;
    WCHAR szEntryName[ RAS_MaxEntryName + 1 ];
    WCHAR szPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
    WCHAR szCallbackNumber[ RAS_MaxCallbackNumber + 1 ];
    WCHAR szUserName[ UNLEN + 1 ];
    WCHAR szPassword[ PWLEN + 1 ];
    WCHAR szDomain[ DNLEN + 1 ];
};

#define RASDIALPARAMS RASDIALPARAMSW

#define LPRASDIALPARAMSW RASDIALPARAMSW*
#define LPRASDIALPARAMS  RASDIALPARAMS*

// Describes extended connection establishment options.  (See RasDial)

#define RASDIALEXTENSIONS struct tagRASDIALEXTENSIONS
RASDIALEXTENSIONS
{
    DWORD dwSize;
    DWORD dwfOptions;
    HWND  hwndParent;
    DWORD reserved;
};

#define LPRASDIALEXTENSIONS RASDIALEXTENSIONS*

// 'dwfOptions' bit flags.

#define RDEOPT_UsePrefixSuffix           0x00000001
#define RDEOPT_PausedStates              0x00000002
#define RDEOPT_IgnoreModemSpeaker        0x00000004
#define RDEOPT_SetModemSpeaker           0x00000008
#define RDEOPT_IgnoreSoftwareCompression 0x00000010
#define RDEOPT_SetSoftwareCompression    0x00000020


/* Describes an enumerated RAS phone book entry name.  (See RasEntryEnum)
*/
#define RASENTRYNAMEW struct tagRASENTRYNAMEW
RASENTRYNAMEW
{
    DWORD dwSize;
    WCHAR szEntryName[ RAS_MaxEntryName + 1 ];
};

#define RASENTRYNAME RASENTRYNAMEW

#define LPRASENTRYNAMEW RASENTRYNAMEW*
#define LPRASENTRYNAME  RASENTRYNAME*

//
// Custom Script Dll Support
//
typedef struct tagRASCOMMSETTINGS {
  DWORD   dwSize;
  BYTE    bParity;
  BYTE    bStop;
  WORD    wAlign;
} RASCOMMSETTINGS;

typedef DWORD (APIENTRY *PFNRASSETCOMMSETTINGS) (
  HANDLE    hPort,
  RASCOMMSETTINGS    * pRasCommSettings,
  PVOID    pvReserved
);

typedef struct RASCUSTOMSCRIPTEXTENSIONS {
  DWORD dwSize;
  PFNRASSETCOMMSETTINGS   pfRasSetCommSettings;
} RASCUSTOMSCRIPTEXTENSIONS;

typedef DWORD (APIENTRY *PFNRASGETBUFFER) (
  PBYTE *   ppBuffer,
  PDWORD    pdwSize
);

typedef DWORD (APIENTRY *PFNRASFREEBUFFER) (
  PBYTE    pBuffer
);

typedef DWORD (APIENTRY *PFNRASSENDBUFFER) (
  HANDLE    hPort,
  PBYTE     pBuffer,
  DWORD     dwSize
);

typedef DWORD (APIENTRY *PFNRASRECEIVEBUFFER) (
  HANDLE    hPort,
  PBYTE     pBuffer,
  PDWORD    pdwSize,
  DWORD     dwTimeoutMilliseconds,
  HANDLE    hEvent
);

typedef DWORD (APIENTRY *PFNRASRETRIEVEBUFFER ) (
  HANDLE    hPort,
  PBYTE    pBuffer,
  PDWORD   pdwSize
);

// Protocol code to projection data structure mapping.

typedef enum tagRASPROJECTION
{
    RASP_Amb = 0x10000,
    RASP_PppNbf = 0x803F,
    RASP_PppIpx = 0x802B,
    RASP_PppIp = 0x8021,
    RASP_PppIpV6 = 0x8057,
    RASP_PppLcp = 0xC021,
    RASP_Slip = 0x20000
}
RASPROJECTION, *LPRASPROJECTION;

/* 
** Describes the result of a RAS AMB (Authentication Message Block)
** projection.  This protocol is used with NT 3.1 and OS/2 1.3 downlevel
** RAS servers.
*/

typedef struct tagRASAMBA
{
    DWORD dwSize;
    DWORD dwError;
    TCHAR  szNetBiosError[ NETBIOS_NAME_LEN + 1 ];
    BYTE  bLana;
} 
RASAMB, *LPRASAMB;

// Describes the result of a PPP NBF (NetBEUI) projection.

typedef struct tagRASPPPNBFW
{
    DWORD dwSize;
    DWORD dwError;
    DWORD dwNetBiosError;
    TCHAR szNetBiosError[ NETBIOS_NAME_LEN + 1 ];
    TCHAR szWorkstationName[ NETBIOS_NAME_LEN + 1 ];
    BYTE  bLana;
} 
RASPPPNBF, *LPRASPPPNBF;


// Describes the results of a PPP IPX (Internetwork Packet Exchange)
// projection.

typedef struct tagRASIPX
{
    DWORD dwSize;
    DWORD dwError;
    TCHAR szIpxAddress[ RAS_MaxIpxAddress + 1 ];
} 
RASPPPIPX, *LPRASPPPIPX;

// Describes the results of an PPP IP (Internet) projection.

#define RASPPPIPW struct tagRASPPPIPW
RASPPPIPW
{
    DWORD dwSize;
    DWORD dwError;
    WCHAR szIpAddress[ RAS_MaxIpAddress + 1 ];
	WCHAR szServerIpAddress[ RAS_MaxIpAddress + 1 ];
	DWORD dwOptions;
	DWORD dwServerOptions;
};

#define RASPPPIP RASPPPIPW

#define LPRASPPPIPW RASPPPIPW*
#define LPRASPPPIP  RASPPPIP*

//
// RASPPPIP 'dwOptions' and 'dwServerOptions' flags.
//
#define RASIPO_VJ       0x00000001		// Indicates that VJ compression is on

// Describes the results of an PPP IPV6 (Internet) projection.

#define RASPPPIPV6 struct tagRASPPPIPV6
RASPPPIPV6
{
    DWORD dwSize;
    DWORD dwError;
    BYTE  LocalInterfaceIdentifier[8];
    BYTE  PeerInterfaceIdentifier[8];
	BYTE  LocalCompressionProtocol[2];
	BYTE  PeerCompressionProtocol[2];
};

#define LPRASPPPIPV6 RASPPPIPV6*

// Describes results of a GET_WINS/DNS I/O control. If
// the requested address has been negotiated the boolean
// valid is TRUE and the IpAddress contains the address of
// the requested server.  If Valid is false IpAddress is
// zero.

typedef	struct	tagRASPPPAddr
{
	DWORD	dwSize;
	DWORD	dwError;
	BOOL	Valid;
	DWORD	IpAddress;
}
RASPPPADDR;

/* Describes the results of a SLIP (Serial Line IP) projection.
*/
#define RASSLIPW struct tagRASSLIPW
RASSLIPW
{
    DWORD dwSize;
    DWORD dwError;
    WCHAR szIpAddress[ RAS_MaxIpAddress + 1 ];
};

#define RASSLIP RASSLIPW

#define LPRASSLIPW RASSLIPW*
#define LPRASSLIP  RASSLIP*


// 	RAS Control Structures
//
// 	Control Request Enumeration

typedef	enum	tagRasCntlEnum
{
	                                            //                                                 pBufIn                 pBufOut
	RASCNTL_SET_DEBUG,							// obsolete
	RASCNTL_LOCK_STATUS,						// obsolete
	RASCNTL_PRINT_CS,							// obsolete
	RASCNTL_STATISTICS,							// Get statistics
	RASCNTL_ENUMDEV,							// Enum Devices
	RASCNTL_GETPROJINFO,						// RasGetProjectionInfoW
	RASCNTL_GETDISPPHONE,						// RasGetDispPhoneNumW
	RASCNTL_DEVCONFIGDIALOGEDIT,				// RasDevConfigDialogEditW
	RASCNTL_SERVER_GET_STATUS,					// Get status of PPP server and lines               NULL                   RASCNTL_SERVERSTATUS + dwNumLines * RASCNTL_SERVERLINE
	RASCNTL_SERVER_ENABLE,					    // Turn the PPP server on                           NULL                   NULL
	RASCNTL_SERVER_DISABLE,					    // Turn the PPP server off                          NULL                   NULL
	RASCNTL_SERVER_GET_PARAMETERS,				// Get global server parameters                     NULL                   RASCNTL_SERVERSTATUS
	RASCNTL_SERVER_SET_PARAMETERS,				// Set global server parameters                     RASCNTL_SERVERSTATUS   NULL
	RASCNTL_SERVER_LINE_ADD,					// Add a line to be managed by the PPP server       RASCNTL_SERVERLINE     NULL
	RASCNTL_SERVER_LINE_REMOVE,					// Remove a line being managed by the PPP server    RASCNTL_SERVERLINE     NULL
	RASCNTL_SERVER_LINE_ENABLE,				    // Enable management of a line                      RASCNTL_SERVERLINE     NULL
	RASCNTL_SERVER_LINE_DISABLE,			    // Disable management of a line                     RASCNTL_SERVERLINE     NULL
	RASCNTL_SERVER_LINE_GET_PARAMETERS,		    // Get line parameters                              RASCNTL_SERVERLINE     RASCNTL_SERVERLINE
	RASCNTL_SERVER_LINE_SET_PARAMETERS,		    // Set line parameters                              RASCNTL_SERVERLINE     NULL
	RASCNTL_SERVER_USER_SET_CREDENTIALS,		// Allow a username/password                        RASCNTL_SERVERUSERCREDENTIALS NULL
	RASCNTL_SERVER_USER_DELETE_CREDENTIALS,		// Remove a username                                RASCNTL_SERVERUSERCREDENTIALS NULL
	RASCNTL_EAP_GET_USER_DATA,					// Get a ras entry's EAP user data
	RASCNTL_EAP_SET_USER_DATA,					// Set a ras entry's EAP user data
	RASCNTL_EAP_GET_CONNECTION_DATA,			// Get a ras entry's EAP conn data
	RASCNTL_EAP_SET_CONNECTION_DATA,			// Set a ras entry's EAP conn data
	RASCNTL_ENABLE_LOGGING,					    // Load logging extension dll if present
	RASCNTL_DISABLE_LOGGING,					// Unload logging extension dll if loaded, stop logging
	RASCNTL_SERVER_LINE_GET_CONNECTION_INFO,    // Get state info on a server lines                 RASCNTL_SERVERLINE               RASCNTL_SERVERCONNECTION
	RASCNTL_SERVER_GET_IPV6_NET_PREFIX,         // Get IPV6 Network prefix pool                     NULL                             RASCNTL_SERVER_IPV6_NET_PREFIX
	RASCNTL_SERVER_SET_IPV6_NET_PREFIX,         // Set IPV6 Network prefix pool                     PRASCNTL_SERVER_IPV6_NET_PREFIX  NULL
	RASCNTL_LAYER_OPEN,                         // Open LCP/Auth/CCP/IPCP                           DWORD (Layer Id)                 NULL
	RASCNTL_LAYER_CLOSE,                        // Close LCP/Auth/CCP/IPCP                          DWORD (Layer Id)                 NULL
	RASCNTL_LAYER_RENEGOTIATE,                  // Renegotiate LCP/Auth/CCP/IPCP                    DWORD (Layer Id)                 NULL
	RASCNTL_LAYER_PARAMETER_GET,                // Get LCP/Auth/CCP/IPCP value                      RASCNTL_LAYER_PARAMETER          RASCNTL_LAYER_PARAMETER
	RASCNTL_LAYER_PARAMETER_SET,                // Set LCP/Auth/CCP/IPCP value                      RASCNTL_LAYER_PARAMETER          NULL
	// Add others here
}
RasCntlEnum_t;

typedef struct _RASCNTL_LAYER_PARAMETER
{
	DWORD dwProtocolType;
	DWORD dwParameterId;
	DWORD dwValueType;
#define RASCNTL_LAYER_PARAMETER_TYPE_NONE	0
#define RASCNTL_LAYER_PARAMETER_TYPE_DWORD	1
#define RASCNTL_LAYER_PARAMETER_TYPE_BYTES	2
	DWORD dwValueSize;
	union
	{
		DWORD dwValue;       // for most (simple integer) values
		BYTE  bytesValue[1]; // array of bytes (size 1 is placeholder)
	};
} RASCNTL_LAYER_PARAMETER, *PRASCNTL_LAYER_PARAMETER;

typedef struct tagRasCntlDevConfigDlgEdit
{
	WCHAR	szDeviceName[RAS_MaxDeviceName + 1];
	WCHAR	szDeviceType[RAS_MaxDeviceType + 1];
	HWND	hWndOwner;
	DWORD	dwSize;
	BYTE	DataBuf[1];
} RASCNTL_DEVCFGDLGED, *PRASCNTL_DEVCFGDLGED;

#define MAX_IF_NAME_LEN	256

//
//	RASCNTL_SERVERSTATUS is...
//
//	..Returned by:
//		RASCNTL_SERVER_GET_STATUS
//		RASCNTL_SERVER_GET_PARAMETERS
//  ..Passed to:
//		RASCNTL_SERVER_SET_PARAMETERS
//
typedef struct tagRasCntlServerStatus
{
	BOOL	bEnable;
	DWORD	bmFlags;
#define	PPPSRV_FLAG_REQUIRE_DATA_ENCRYPTION			(1<<1)	// Require encryption on this connection.
#define	PPPSRV_FLAG_ALLOW_UNAUTHENTICATED_ACCESS	(1<<2)	// Do not require authentication on the connection
#define	PPPSRV_FLAG_NO_VJ_HEADER_COMPRESSION		(1<<3)	// Prevent VJ TCP/IP header compression
#define	PPPSRV_FLAG_NO_DATA_COMPRESSION				(1<<4)	// Prevent MS data compression
#define	PPPSRV_FLAG_ADD_CLIENT_SUBNET				(1<<5)	// Add a subnet route for a client connection

	BOOL	bUseDhcpAddresses;		// Obtain addresses from DHCP server rather than static pool
	DWORD	dwStaticIpAddrStart;	// If using static IP address pool, this is the first address
	DWORD	dwStaticIpAddrCount;	// Number of static IP addresses following IpAddrStart in pool
	DWORD	bmAuthenticationMethods;// Bitmask of authentication methods to be disallowed, see
									// RASEO_ProhibitXxx in ras.h

	DWORD	dwNumLines;

	BOOL    bUseAutoIpAddresses;    // TRUE if IP addresses for clients should be generated from AutoIp pool
	DWORD	dwAutoIpSubnet;         // Defines AutoIP address pool
	DWORD	dwAutoIpSubnetMask;         	

	WCHAR   wszDhcpInterface[MAX_IF_NAME_LEN+1];
} RASCNTL_SERVERSTATUS, *PRASCNTL_SERVERSTATUS;

//
//	Structure passed to the following IOCTLs
//		 RASCNTL_SERVER_LINE_GET_PARAMETERS
//		 RASCNTL_SERVER_LINE_SET_PARAMETERS
//		 RASCNTL_SERVER_LINE_ENABLE
//		 RASCNTL_SERVER_LINE_DISABLE
//
typedef struct tagRasCntlServerLine
{
	RASDEVINFO	rasDevInfo;				// szDeviceType and szDeviceName of the line
	BOOL		bEnable;
	DWORD		bmFlags;
	UINT		DisconnectIdleSeconds;
	DWORD		dwDevConfigSize;
	BYTE		DevConfig[1];			// Variable size (dwDevConfigSize bytes) array of device config info
} RASCNTL_SERVERLINE, *PRASCNTL_SERVERLINE;

//
//	Structure passed to the following IOCTLs
//		RASCNTL_SERVER_USER_SET_CREDENTIALS			 Allow a username/password
//		RASCNTL_SERVER_USER_DELETE_CREDENTIALS		 Remove a username
//
typedef struct tagRasCntlServerUser
{
	TCHAR		tszUserName[UNLEN + 1];
	TCHAR		tszDomainName[DNLEN + 1];			// may be null
	BYTE		password[PWLEN];
	DWORD		cbPassword;
} RASCNTL_SERVERUSERCREDENTIALS, *PRASCNTL_SERVERUSERCREDENTIALS;

//
//  Structure containing info describing an enabled line's connection status
//
//  Used with the IOCTLS:
//	    RASCNTL_SERVER_LINE_GET_CONNECTION_INFO
//
typedef struct tagRasCntlServerConnection
{
    RASDEVINFO	 rasDevInfo;		                 // szDeviceType and szDeviceName of the line
    HRASCONN     hrasconn;                           // handle that can be used in RasGetLinkStatistics (NULL if line is not enabled)
    DWORD        dwServerIpAddress;                  // IP Address for server line IP interface
    DWORD        dwClientIpAddress;                  // IP Address that will be assigned to client connecting to this line
    RASCONNSTATE RasConnState;                       // RASCS_Disconnected, etc.
    TCHAR        tszUserName[DNLEN + 1 + UNLEN + 1]; // Name of user logged in to the port
} RASCNTL_SERVERCONNECTION, *PRASCNTL_SERVERCONNECTION;

//
//  Structure containing info describing the pool of IPV6 network prefixes that
//  can be assigned by the server.
//  Used with the IOCTLS:
//	    RASCNTL_SERVER_GET_IPV6_NET_PREFIX
//	    RASCNTL_SERVER_SET_IPV6_NET_PREFIX
//
typedef struct tagRasCntlServerIPV6NetPrefix
{
	BYTE         IPV6NetPrefix[16];
	DWORD        IPV6NetPrefixBitLength;
	DWORD        IPV6NetPrefixCount;
} RASCNTL_SERVER_IPV6_NET_PREFIX, *PRASCNTL_SERVER_IPV6_NET_PREFIX;

// 
// RasDial message notifications are sent with Message ID set to
// WM_RASDIALEVENT
//

#define RASDIALEVENT    	"RasDialEvent"

#define WM_RASDIALEVENT 	0xCCCD


typedef struct _RAS_STATS
{
    DWORD   dwSize;
    DWORD   dwBytesXmited;
    DWORD   dwBytesRcved;
    DWORD   dwFramesXmited;
    DWORD   dwFramesRcved;
    DWORD   dwCrcErr;
    DWORD   dwTimeoutErr;
    DWORD   dwAlignmentErr;
    DWORD   dwHardwareOverrunErr;
    DWORD   dwFramingErr;
    DWORD   dwBufferOverrunErr;
    DWORD   dwCompressionRatioIn;
    DWORD   dwCompressionRatioOut;
    DWORD   dwBps;
    DWORD   dwConnectDuration;

} RAS_STATS, *PRAS_STATS;


#ifdef UNDER_CE

// This structure is used by the RnaApp application on WINCE
// to signal when a connection has occured.

typedef struct tagRNAAppInfo {
	DWORD	dwSize;					// The size of this structure
	DWORD	hWndRNAApp;				// The handle of the RNAApp window
	DWORD	Context;				// Context value specified on CmdLine
	DWORD	ErrorCode;				// Last error code
	TCHAR	RasEntryName[RAS_MaxEntryName+1];
} RNAAPP_INFO, *PRNAAPP_INFO;


#define RNA_RASCMD		WM_USER+1
#define	RNA_ADDREF		1
#define RNA_DELREF		2
#endif

#include "poppack.h"

#ifndef RASAPI
#define RASAPI
#endif

DWORD RASAPI RasDial (LPRASDIALEXTENSIONS dialExtensions,
	       LPTSTR phoneBookPath,
	       LPRASDIALPARAMS rasDialParam,
	       DWORD NotifierType,
	       LPVOID notifier,
	       LPHRASCONN pRasConn);
#define RasDialW RasDial

DWORD RASAPI RasHangUp (HRASCONN Session);
#define RasHangUpW RasHangUp

DWORD RASAPI RasEnumEntries (LPWSTR Reserved,
		      LPWSTR lpszPhoneBookPath,
		      LPRASENTRYNAME lprasentryname,
		      LPDWORD lpcb,
		      LPDWORD lpcEntries);
#define	RasEnumEntriesW RasEnumEntries

DWORD RASAPI RasGetEntryDialParams (LPWSTR lpszPhoneBook,
			     LPRASDIALPARAMS lpRasDialParams,	
			     LPBOOL lpfPassword);
#define RasGetEntryDialParamsW RasGetEntryDialParams

DWORD RASAPI RasSetEntryDialParams (LPWSTR lpszPhoneBook,
			     LPRASDIALPARAMS lpRasDialParams,
			     BOOL fRemovePassword);
#define RasSetEntryDialParamsW RasSetEntryDialParams

DWORD RASAPI RasGetEntryProperties (LPWSTR lpszPhoneBook,
			     LPWSTR szEntry,
			     LPRASENTRY lpEntry,
			     LPDWORD lpdwEntrySize,
			     LPBYTE lpb,
			     LPDWORD lpdwSize);
#define RasGetEntryPropertiesW RasGetEntryProperties

DWORD RASAPI RasSetEntryProperties (LPWSTR lpszPhoneBook,		
			     LPWSTR szEntry,
			     LPRASENTRY lpEntry,
			     DWORD dwEntrySize,
			     LPBYTE lpb,
			     DWORD dwSize);
#define RasSetEntryPropertiesW RasSetEntryProperties

DWORD RASAPI RasValidateEntryName (LPCWSTR lpszPhonebook,		
			    LPCWSTR lpszEntry);
#define RasValidateEntryNameW RasValidateEntryName

DWORD RASAPI RasDeleteEntry (LPWSTR lpszPhonebook,
		      LPWSTR lpszEntry);
#define RasDeleteEntryW RasDeleteEntry

DWORD RASAPI RasRenameEntry (LPWSTR lpszPhonebook,
		      LPWSTR lpszOldEntry,
		      LPWSTR lpszNewEntry);
#define RasRenameEntryW RasRenameEntry

DWORD RASAPI RasEnumConnections (__out_bcount(*lpcb) LPRASCONN lprasconn,
			  LPDWORD lpcb,
			  LPDWORD lpcConnections);
#define	RasEnumConnectionsW	RasEnumConnections

DWORD RASAPI RasGetConnectStatus (HRASCONN rasconn,				
			   LPRASCONNSTATUS lprasconnstatus);
#define RasGetConnectStatusW	RasGetConnectStatus

DWORD RASAPI RasGetEntryDevConfig (LPCTSTR szPhonebook,
			    LPCTSTR szEntry,
			    LPDWORD pdwDeviceID,
			    LPDWORD pdwSize,
			    LPVARSTRING pDeviceConfig);
#define RasGetEntryDevConfigW RasGetEntryDevConfig

DWORD RASAPI RasSetEntryDevConfig (LPCTSTR szPhonebook,
			    LPCTSTR szEntry,
			    DWORD dwDeviceID,
				LPVARSTRING lpDeviceConfig);
#define RasSetEntryDevConfigW RasSetEntryDevConfig


DWORD APIENTRY RasEnumDevicesW( LPRASDEVINFOW, LPDWORD, LPDWORD );
#define RasEnumDevices          RasEnumDevicesW

DWORD APIENTRY RasGetProjectionInfoW( HRASCONN, RASPROJECTION, LPVOID,
                   LPDWORD );
#define RasGetProjectionInfo    RasGetProjectionInfoW

DWORD APIENTRY RasGetLinkStatistics(HRASCONN hRasConn,
                                    DWORD dwSubEntry,
                                    RAS_STATS *lpStatistics);

DWORD APIENTRY RasGetDispPhoneNumW(LPCWSTR szPhonebook,
								   LPCWSTR szEntry,
								   LPWSTR szPhoneNum,
								   DWORD dwPhoneNumLen);
#define RasGetDispPhoneNum	RasGetDispPhoneNumW

DWORD APIENTRY RasDevConfigDialogEditW (LPCWSTR szDeviceName, LPCWSTR szDeviceType,
										HWND hWndOwner, LPVOID lpDeviceConfigIn,
										DWORD dwSize, LPVARSTRING lpDeviceConfigOut);
#define RasDevConfigDialogEdit RasDevConfigDialogEditW

DWORD APIENTRY RasIOControl(LPVOID hRasConn, DWORD dwCode, PBYTE pBufIn,
                            DWORD dwLenIn, PBYTE pBufOut, DWORD dwLenOut,
                            PDWORD pdwActualOut);

DWORD
RasGetEapUserData (
	IN		HANDLE	hToken,           // access token for user 
	IN		LPCTSTR pszPhonebook,     // path to phone book to use 
	IN		LPCTSTR pszEntry,         // name of entry in phone book 
	OUT		PBYTE   pbEapData,        // retrieved data for the user 
	IN	OUT	PDWORD  pdwSizeofEapData);// size of retrieved data 

DWORD
RasSetEapUserData(
	IN	HANDLE	hToken,           // access token for user
	IN	LPCTSTR pszPhonebook,     // path to phone book to use
	IN	LPCTSTR pszEntry,         // name of entry in phone book
	IN	PBYTE	pbEapData,        // data to store for the user
	IN	DWORD	dwSizeofEapData); // size of data

DWORD
RasGetEapConnectionData (
	IN		LPCTSTR pszPhonebook,     // path to phone book to use 
	IN		LPCTSTR pszEntry,         // name of entry in phone book 
	OUT		PBYTE   pbEapData,        // retrieved data for the user 
	IN	OUT	PDWORD  pdwSizeofEapData);// size of retrieved data 

DWORD
RasSetEapConnectionData(
	IN	LPCTSTR pszPhonebook,    // path to phone book to use
	IN	LPCTSTR pszEntry,        // name of entry in phone book
	IN	PBYTE	pbEapData,       // data to store for the connection
	IN	DWORD	dwSizeofEapData);// size of data

#ifdef __cplusplus
}
#endif


#ifdef WINCEOEM
#include <pras.h>	// internal defines 
#endif

#endif // _RAS_H_

// @CESYSGEN ENDIF
