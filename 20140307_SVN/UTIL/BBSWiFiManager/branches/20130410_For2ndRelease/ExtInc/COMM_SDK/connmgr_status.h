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

#ifndef _connmgr_status_h_
#define _connmgr_status_h_

#include <winsock2.h> // Need this for SOCKADDR_STORAGE

#ifdef __cplusplus
extern "C" {
#endif

#include <winsock2.h>
#include <connmgr_conntypes.h>

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct CONNMGR_CONNECTION_IPADDR | Address information for an active connection
//
// @comm None
//
// -----------------------------------------------------------------------------

typedef struct _CONNMGR_CONNECTION_IPADDR
{
    DWORD cIPAddr;                  // @field Count of addresses in array.
    SOCKADDR_STORAGE IPAddr[1];     // @field Array of IPv4 and/or IPv6 addresses.
} CONNMGR_CONNECTION_IPADDR;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct CONNMGR_CONNECTION_DETAILED_STATUS | Information about available connections
//
// @comm None
//
// -----------------------------------------------------------------------------

#define CONNMGRDETAILEDSTATUS_VERSION   1

#define CONNMGRDETAILEDSTATUS_PARAM_TYPE            0x00000001
#define CONNMGRDETAILEDSTATUS_PARAM_SUBTYPE         0x00000002
#define CONNMGRDETAILEDSTATUS_PARAM_DESTNET         0x00000004
#define CONNMGRDETAILEDSTATUS_PARAM_SOURCENET       0x00000008
#define CONNMGRDETAILEDSTATUS_PARAM_FLAGS           0x00000010
#define CONNMGRDETAILEDSTATUS_PARAM_SECURE          0x00000020
#define CONNMGRDETAILEDSTATUS_PARAM_DESCRIPTION     0x00000040
#define CONNMGRDETAILEDSTATUS_PARAM_ADAPTERNAME     0x00000080
#define CONNMGRDETAILEDSTATUS_PARAM_CONNSTATUS      0x00000100
#define CONNMGRDETAILEDSTATUS_PARAM_LASTCONNECT     0x00000200
#define CONNMGRDETAILEDSTATUS_PARAM_SIGNALQUALITY   0x00000400
#define CONNMGRDETAILEDSTATUS_PARAM_IPADDR          0x00000800

typedef struct _CONNMGR_CONNECTION_DETAILED_STATUS
{
    struct _CONNMGR_CONNECTION_DETAILED_STATUS* pNext;
    
    DWORD dwVer;                // @field Structure version; current is CONNMGRDETAILEDSTATUS_VERSION.
    DWORD dwParams;             // @field Combination of CONNMGRDETAILEDSTATUS_PARAM_* values.
    
    DWORD dwType;               // @field One of CM_CONNTYPE_* values.
    DWORD dwSubtype;            // @field One of CM_CONNSUBTYPE_* values.
    
    DWORD dwFlags;              // @field Combination of CM_DSF_* flags.
    DWORD dwSecure;             // @field Secure level (0 == not-secure) of connection.

    GUID guidDestNet;           // @field GUID of destination network.
    GUID guidSourceNet;         // @field GUID of source network.
    
    TCHAR* szDescription;       // @field Name of connection, 0-terminated string or NULL if N/A.
    TCHAR* szAdapterName;       // @field Name of adapter, 0-terminated or NULL if N/A.
    
    DWORD dwConnectionStatus;   // @field One of CONNMGR_STATUS_*.
    SYSTEMTIME LastConnectTime; // @field Time connection was last established.
    DWORD dwSignalQuality;      // @field Signal quality normalized in the range 0-255.
    
    CONNMGR_CONNECTION_IPADDR*  pIPAddr; // @field Available IP addrs, or NULL if N/A.
// End of version 1 fields.

} CONNMGR_CONNECTION_DETAILED_STATUS;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func ConnMgrQueryDetailedStatus | Returns detailed information about all existing connections.
//
// @comm Standard semantics here, call once to find out buffer size, then call
// again to get array of CONNMGR_CONNECTION_DETAILED_STATUS structures.
// Note: since connections can change often, make sure to allow for possibility
// that between first and second call, the necessary amount of space needed for
// the return structure changes.
//
// -----------------------------------------------------------------------------

HRESULT WINAPI ConnMgrQueryDetailedStatus
(
    CONNMGR_CONNECTION_DETAILED_STATUS *pStatusBuffer,	// @parm Pointer to buffer to be filled status info.
    DWORD *pcbBufferSize    // @parm IN: Size of buffer.  OUT: size of buffer needed.
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func ConnMgrRegisterForStatusNotification | Registers a window handle for connection status notifications.
//
// @comm Will post a notification when,
// 1) an inactive connection is created or deleted, ex. via user input.
// 2) any of of the active connections changes connection state, i.e.
//    active to inactive, or inactive to active.
// Note: Notifications will not be generated for other changes, ex. change in
// signal quality.
//
// -----------------------------------------------------------------------------

#define CONNMGR_STATUS_CHANGE_NOTIFICATION_MSG TEXT("CONNMGR_STATUS_CHANGE_NOTIFICATION_MSG")

HRESULT WINAPI ConnMgrRegisterForStatusChangeNotification
(
    BOOL fEnable,   // @parm Turn on or turn off notifications
    HWND hWnd       // @parm Window to post status change notifications to
);

#ifdef __cplusplus
}
#endif

#endif // _connmgr_status_h_

