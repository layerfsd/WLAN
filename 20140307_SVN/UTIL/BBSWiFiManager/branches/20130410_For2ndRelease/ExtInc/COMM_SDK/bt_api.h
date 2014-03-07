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
//------------------------------------------------------------------------------
// 
//      Bluetooth client API declarations
// 
// 
// Module Name:
// 
//      bt_api.h
// 
// Abstract:
// 
//      This file defines client-level APIs to Bluetooth stack
// 
// 
//------------------------------------------------------------------------------
//
//	Blietooth client API
//
#if ! defined (__bt_api_H__)
#define __bt_api_H__		1

#include <ws2bth.h>
#include <winioctl.h>

#if defined (__cplusplus)
extern "C" {
#endif

//
//		COD classes
//

#define GET_MAJOR_SERVICE_COD(dwCOD)                            (dwCOD & 0x00FFE000) //bits 13-23
#define BTH_COD_MAJOR_SERVICE_CLASS_INFORMATION                 0x00800000
#define BTH_COD_MAJOR_SERVICE_CLASS_TELEPHONY                   0x00400000
#define BTH_COD_MAJOR_SERVICE_CLASS_AUDIO                       0x00200000
#define BTH_COD_MAJOR_SERVICE_CLASS_OBEX                        0x00100000
#define BTH_COD_MAJOR_SERVICE_CLASS_CAPTURE                     0x00080000
#define BTH_COD_MAJOR_SERVICE_CLASS_RENDERING                   0x00040000
#define BTH_COD_MAJOR_SERVICE_CLASS_NETWORK                     0x00020000
#define BTH_COD_MAJOR_SERVICE_CLASS_POSITIONING                 0x00010000
#define BTH_COD_MAJOR_SERVICE_CLASS_LIMITED_DISC                0x00002000

#define GET_MAJOR_DEVICE_COD(dwCOD)                             (dwCOD & 0x00001F00) //bits 8-12
#define BTH_COD_MAJOR_DEVICE_CLASS_MISC                         0x00000000
#define BTH_COD_MAJOR_DEVICE_CLASS_COMPUTER                     0x00000100
#define BTH_COD_MAJOR_DEVICE_CLASS_PHONE                        0x00000200
#define BTH_COD_MAJOR_DEVICE_CLASS_LAP                          0x00000300
#define BTH_COD_MAJOR_DEVICE_CLASS_AV                           0x00000400
#define BTH_COD_MAJOR_DEVICE_CLASS_PERIPHERAL                   0x00000500
#define BTH_COD_MAJOR_DEVICE_CLASS_IMAGING                      0x00000600
#define BTH_COD_MAJOR_DEVICE_CLASS_UNCLASSIFIED	                0x00001F00

#define GET_MINOR_COMPUTER_COD(dwCOD)                           (dwCOD & 0x000000FC) //bits 2-7
#define BTH_COD_MINOR_COMPUTER_UNCLASSIFIED			0x00000000
#define BTH_COD_MINOR_COMPUTER_DESKTOP				0x00000004
#define BTH_COD_MINOR_COMPUTER_SERVER				0x00000008
#define BTH_COD_MINOR_COMPUTER_LAPTOP				0x0000000C
#define BTH_COD_MINOR_COMPUTER_HANDHELD                         0x00000010
#define BTH_COD_MINOR_COMPUTER_PDA                              0x00000014
#define BTH_COD_MINOR_COMPUTER_WEARABLE                         0x00000018

#define GET_MINOR_PHONE_COD(dwCOD)                              (dwCOD & 0x000000FC) //bits 2-7
#define BTH_COD_MINOR_PHONE_UNCLASSIFIED                        0x00000000
#define BTH_COD_MINOR_PHONE_CELL                                0x00000004
#define BTH_COD_MINOR_PHONE_CORDLESS                            0x00000008
#define BTH_COD_MINOR_PHONE_SMART                               0x0000000C
#define BTH_COD_MINOR_PHONE_WIRED                               0x00000010
#define BTH_COD_MINOR_PHONE_ISDN                                0x00000014

#define GET_MINOR_LAP_COD(dwCOD)                                (dwCOD & 0x000000E0) //bits 5-7
#define BTH_COD_MINOR_LAP_AVAILABLE                             0x00000000
#define BTH_COD_MINOR_LAP_1_17                                  0x00000020
#define BTH_COD_MINOR_LAP_17_33                                 0x00000040
#define BTH_COD_MINOR_LAP_33_50                                 0x00000060
#define BTH_COD_MINOR_LAP_50_67                                 0x00000080
#define BTH_COD_MINOR_LAP_67_83                                 0x000000A0
#define BTH_COD_MINOR_LAP_83_99                                 0x000000C0
#define BTH_COD_MINOR_LAP_NO_SERVICE	                        0x000000E0

#define GET_MINOR_AV_COD(dwCOD)                                 (dwCOD & 0x000000FC) //bits 2-7
#define BTH_COD_MINOR_AV_UNCLASSIFIED                           0x00000000
#define BTH_COD_MINOR_AV_HEADSET                                0x00000004
#define BTH_COD_MINOR_AV_HANDSFREE                              0x00000008
// reserved                                                     0x0000000C
#define BTH_COD_MINOR_AV_MICROPHONE                             0x00000010
#define BTH_COD_MINOR_AV_LOUDSPEAKER                            0x00000014
#define BTH_COD_MINOR_AV_HEADPHONE                              0x00000018
#define BTH_COD_MINOR_AV_PORTABLE                               0x0000001C
#define BTH_COD_MINOR_AV_CAR                                    0x00000020
#define BTH_COD_MINOR_AV_SETTOP                                 0x00000024
#define BTH_COD_MINOR_AV_HIFI                                   0x00000028
#define BTH_COD_MINOR_AV_VCR                                    0x0000002C
#define BTH_COD_MINOR_AV_CAMERA                                 0x00000030
#define BTH_COD_MINOR_AV_CAMCORDER                              0x00000034
#define BTH_COD_MINOR_AV_DISPLAY                                0x00000038
#define BTH_COD_MINOR_AV_DISPLAYSPEAKER                         0x0000003C
#define BTH_COD_MINOR_AV_CONFERENCING                           0x00000040
// reserved                                                     0x00000044
#define BTH_COD_MINOR_AV_GAMING                                 0x00000048

#define GET_MINOR_FN_PERIPHERAL_COD(dwCOD)                      (dwCOD & 0x0000003C) //bits 2-5
#define BTH_COD_MINOR_FN_PERIPHERAL_UNCLASSIFIED                0x00000000
#define BTH_COD_MINOR_FN_PERIPHERAL_JOYSTICK                    0x00000004
#define BTH_COD_MINOR_FN_PERIPHERAL_GAMEPAD                     0x00000008
#define BTH_COD_MINOR_FN_PERIPHERAL_REMOTE                      0x0000000C
#define BTH_COD_MINOR_FN_PERIPHERAL_SENSING                     0x00000010
#define BTH_COD_MINOR_FN_PERIPHERAL_TABLET                      0x00000014
#define BTH_COD_MINOR_FN_PERIPHERAL_CARDREADER                  0x0000001C

#define GET_MINOR_PERIPHERAL_COD(dwCOD)                         (dwCOD & 0x000000C0) //bits 6-7
#define BTH_COD_MINOR_PERIPHERAL_KEYBOARD                       0x00000040
#define BTH_COD_MINOR_PERIPHERAL_POINTING                       0x00000080
#define BTH_COD_MINOR_PERIPHERAL_COMBO                          0x000000C0

#define GET_MINOR_FN_IMAGING_COD(dwCOD)                         (dwCOD & 0x000000F0) //bits 4-7
#define BTH_COD_MINOR_FN_IMAGING_DISPLAY                        0x00000010
#define BTH_COD_MINOR_FN_IMAGING_CAMERA                         0x00000020
#define BTH_COD_MINOR_FN_IMAGING_SCANNER                        0x00000040
#define BTH_COD_MINOR_FN_IMAGING_PRINTER                        0x00000080

// Defined for backwards compatibility
#define BTH_COD_MAJOR_DEVICE_CLASS_AUDIO                        BTH_COD_MAJOR_DEVICE_CLASS_AV
#define BTH_COD_MINOR_AUDIO_UNCLASSIFIED                        BTH_COD_MINOR_AV_UNCLASSIFIED
#define BTH_COD_MINOR_AUDIO_HEADSET                             BTH_COD_MINOR_AV_HEADSET

#define BTH_NAMEDEVENT_PAIRING_CHANGED              L"system/events/bluetooth/PairingChange"
#define BTH_NAMEDEVENT_HARDWARE_CHANGED             L"system/events/bluetooth/HardwareChange"
#define BTH_NAMEDEVENT_DEVICEID_CHANGED             L"system/events/bluetooth/DeviceIdChange"
#define BTH_NAMEDEVENT_CONNECTIVITY_CHANGED         L"system/events/bluetooth/ConnectivityChange"
#define BTH_NAMEDEVENT_SECURITY_CHANGED             L"system/events/bluetooth/SecurityChange"
#define BTH_NAMEDEVENT_CONNECTIONS_CHANGED          L"system/events/bluetooth/ConnectionsChange"
#define BTH_NAMEDEVENT_BASEBAND_CHANGED             L"system/events/bluetooth/BasebandChange"
#define BTH_NAMEDEVENT_STACK_INITED                 L"system/events/bluetooth/StackInitialized"
#define BTH_NAMEDEVENT_PAN_REFRESH                  L"system/events/bluetooth/pan/refresh"

#if ! defined (__bt_ddi_H__)
//
//  Attention: also defined in bt_ddi.h! Keep in sync!
//
//	Hardware Status
//
#define HCI_HARDWARE_UNKNOWN							0
#define HCI_HARDWARE_NOT_PRESENT						1
#define HCI_HARDWARE_INITIALIZING						2
#define HCI_HARDWARE_RUNNING							3
#define HCI_HARDWARE_SHUTDOWN							4
#define HCI_HARDWARE_ERROR								5
#endif

#define BTH_GET_BASEBAND_CONNECTIONS_EX_API_VERSION      1

typedef struct _BASEBAND_CONNECTION
{
	USHORT		hConnection;
	BT_ADDR 	baAddress;
	int 		cDataPacketsPending;
	UINT		fLinkType       : 1;
	UINT		fEncrypted		: 1;
	UINT		fAuthenticated	: 1;
	UINT		fMode			: 3;
} BASEBAND_CONNECTION, *PBASEBAND_CONNECTION;

typedef struct _BASEBAND_CONNECTION_EX
{
    USHORT      hConnection;    
    BT_ADDR     baAddress;
    int         cDataPacketsPending;
    UCHAR       link_type;
    UCHAR       mode;
    UINT        fEncrypted      : 1;
    BOOL        fAuthenticated  : 1;
} BASEBAND_CONNECTION_EX, *PBASEBAND_CONNECTION_EX;
    


#define HCI_PAGE_SCAN_TYPE_STANDARD         0x00
#define HCI_PAGE_SCAN_TYPE_INTERLACED       0x01

#define HCI_INQUIRY_SCAN_TYPE_STANDARD      0x00
#define HCI_INQUIRY_SCAN_TYPE_INTERLACED    0x01


//
// Warning: To use the following Bth* APIs you must link to btdrt.lib.  
//          This library may or may not be available in your SDK.
//
// To preserve Win32 compatibility, consider using the Winsock equivalent 
// of these functions.
//

//
//	Management APIs
//
int BthWriteScanEnableMask
(
unsigned char	mask
);

int BthReadScanEnableMask
(
unsigned char	*pmask
);

int BthWritePageTimeout
(
unsigned short timeout
);

int BthReadPageTimeout
(
unsigned short *ptimeout
);

int BthWriteCOD
(
unsigned int	cod
);

int BthReadCOD
(
unsigned int	*pcod
);

int BthGetRemoteCOD
(
BT_ADDR			*pbt,
unsigned int	*pcod
);

int BthWriteAuthenticationEnable
(
unsigned char	ae
);

int BthReadAuthenticationEnable
(
unsigned char	*pae
);

int BthWriteLinkPolicySettings
(
BT_ADDR			*pba,
unsigned short	lps
);

int BthReadLinkPolicySettings
(
BT_ADDR			*pba,
unsigned short	*plps
);

int BthEnterHoldMode
(
BT_ADDR			*pba,
unsigned short	hold_mode_max,
unsigned short	hold_mode_min,
unsigned short  *pinterval
);

int BthEnterSniffMode
(
BT_ADDR			*pba,
unsigned short	sniff_mode_max,
unsigned short	sniff_mode_min,
unsigned short	sniff_attempt,
unsigned short	sniff_timeout,
unsigned short  *pinterval
);

int BthExitSniffMode
(
BT_ADDR			*pba
);

int BthEnterParkMode
(
BT_ADDR			*pba,
unsigned short  beacon_max,
unsigned short  beacon_min,
unsigned short  *pinterval
);

int BthExitParkMode
(
BT_ADDR			*pba
);

int BthGetCurrentMode
(
BT_ADDR			*pba,
unsigned char	*pmode
);

int BthGetBasebandHandles
(
int				cHandles,
__out_ecount(cHandles) unsigned short	*pHandles,
int				*pcHandlesReturned
);

int BthGetBasebandConnections
(
int					cConnections,
__out_ecount(cConnections) BASEBAND_CONNECTION	*pConnections,
int					*pcConnectionsReturned
);

int BthGetBasebandConnectionsEx
(
DWORD dwApiVersion,
int cConnections,
__out_ecount(cConnections) BASEBAND_CONNECTION_EX *pConnections,
int *pcConnectionsReturned
);

int BthGetAddress
(
unsigned short	handle,
BT_ADDR			*pba
);

int BthReadLocalAddr
(
BT_ADDR         *pba
);

int BthGetHardwareStatus
(
int				*pistatus
);

int BthReadLocalVersion
(
unsigned char	*phci_version,
unsigned short	*phci_revision,
unsigned char	*plmp_version,
unsigned short	*plmp_subversion,
unsigned short	*pmanufacturer,
unsigned char	*plmp_features
);

int BthReadRemoteVersion
(
BT_ADDR			*pba,
unsigned char	*plmp_version,
unsigned short	*plmp_subversion,
unsigned short	*pmanufacturer,
unsigned char	*plmp_features
);

int BthPerformInquiry
(
unsigned int	 LAP,
unsigned char	 length,
unsigned char	 num_responses,
unsigned int	 cBuffer,
unsigned int	 *pcDiscoveredDevices,
__out_ecount(cBuffer) BthInquiryResult *InquiryList
);

int BthCancelInquiry
(
void
);

int BthRemoteNameQuery
(
BT_ADDR			*pba,
unsigned int	cBuffer,
unsigned int	*pcRequired,
__out_ecount(cBuffer) WCHAR			*szString
);

int BthTerminateIdleConnections
(
void
);

int BthSetInquiryFilter
(
BT_ADDR			*pba
);

int BthSetCODInquiryFilter
(
 unsigned int cod,
 unsigned int codMask
);

int BthClearInquiryFilter
(
void
);

int BthSwitchRole
(
BT_ADDR* pbt,     
USHORT usRole     
);

int BthGetRole
(
BT_ADDR* pbt,     
USHORT* pusRole
);

int BthReadRSSI
(
BT_ADDR* pbt, 
BYTE* pbRSSI
);


//
//	Security manager APIs
//
int BthSetPIN
(
BT_ADDR			*pba,
int				cPinLength,
unsigned char	*ppin
);

int BthRevokePIN
(
BT_ADDR			*pba
);

int BthSetLinkKey
(
BT_ADDR			*pba,
unsigned char   key[16]
);

int BthGetLinkKey
(
BT_ADDR			*pba,
unsigned char   key[16]
);

int BthRevokeLinkKey
(
BT_ADDR			*pba
);

int BthAuthenticate
(
BT_ADDR			*pba
);

int BthSetEncryption
(
BT_ADDR			*pba,
int				fOn
);

int BthSetSecurityUI
(
HANDLE		hEvent,
DWORD		dwStoreTimeout,
DWORD		dwProcTimeout
);

int BthGetPINRequest
(
BT_ADDR *pbt
);

int BthRefusePINRequest
(
BT_ADDR	*pbt
);

int BthAnswerPairRequest 
(
BT_ADDR *pba,
int cPinLength,
__in_bcount(cPinLength) unsigned char *ppin
);

int BthPairRequest 
(
BT_ADDR *pba,
int cPinLength,
__in_bcount(cPinLength) unsigned char *ppin
);

//
//  Connection APIs
//

int BthCreateACLConnection
(
BT_ADDR			*pbt,
unsigned short	*phandle
);

int BthCreateSCOConnection
(
BT_ADDR			*pbt,
unsigned short	*phandle
);

int BthCloseConnection
(
unsigned short	handle
);

int BthAcceptSCOConnections
(
BOOL fAccept
);

int BthWritePageScanActivity
(
unsigned short pageScanInterval,
unsigned short pageScanWindow
);

int BthWriteInquiryScanActivity
(
unsigned short inquiryScanInterval,
unsigned short inquiryScanWindow
);

int BthReadPageScanActivity
(
unsigned short* pPageScanInterval,
unsigned short* pPageScanWindow
);

int BthReadInquiryScanActivity
(
unsigned short* pInquiryScanInterval,
unsigned short* pInquiryScanWindow
);

int BthWritePageScanType
(
unsigned char pageScanType
);

int BthWriteInquiryScanType
(
unsigned char inquiryScanType
);

int BthReadPageScanType
(
unsigned char* pPageScanType
);

int BthReadInquiryScanType
(
unsigned char* pInquiryScanType
);

int BthCreateSynchronousConnection
(
BT_ADDR         *pbt,
unsigned short  *pHandle,
unsigned int    txBandwidth,
unsigned int    rxBandwidth,
unsigned short  maxLatency,
unsigned short  voiceSetting,
unsigned char   retransmit
);

int BthAcceptSynchronousConnections
(
BOOL fAccept
);

BOOL BthIsProfileAllowed
(
GUID uuidService
);


//
//	SDP Name Service APIs
//
typedef struct _SdpAttributeRange SdpAttributeRange;
typedef struct _SdpQueryUuid      SdpQueryUuid;
typedef struct _WSAQuerySetW *LPWSAQUERYSET;
typedef enum _WSAESETSERVICEOP WSAESETSERVICEOP;


int BthNsSetService(LPWSAQUERYSET pSet, WSAESETSERVICEOP op, DWORD dwFlags);
int BthNsLookupServiceBegin(LPWSAQUERYSET pQuerySet, DWORD dwFlags, LPHANDLE lphLookup);
int BthNsLookupServiceNext(HANDLE hLookup, DWORD dwFlags, LPDWORD lpdwBufferLength, __out_bcount_opt(*lpdwBufferLength) LPWSAQUERYSET pResults);
int BthNsLookupServiceEnd(HANDLE hLookup);

//
//	RFCOMM Apis
//
#define RFCOMM_PORT_FLAGS_REMOTE_DCB    0x00000001
#define RFCOMM_PORT_FLAGS_KEEP_DCD      0x00000002
#define RFCOMM_PORT_FLAGS_AUTHENTICATE	0x00000004
#define RFCOMM_PORT_FLAGS_ENCRYPT       0x00000008

#if ! defined (__bt_ddi_H__)
//
//  Attention: also defined in bt_ddi.h! Keep in sync!
//
//	channel:
//		RFCOMM_CHANNEL_ALL			accept connection on all channels (default upper layer)
//		RFCOMM_CHANNEL_CLIENT_ONLY	do not accept connections at all (client only)
//		...or channel to restrict connections on
//
#define RFCOMM_CHANNEL_ALL			0x00
#define RFCOMM_CHANNEL_MULTIPLE		0xfe
#define RFCOMM_CHANNEL_CLIENT_ONLY	0xff
#endif

typedef struct _portemu_port_params {
	int				channel;
	int				flocal;
	BT_ADDR			device;
	int				imtu;
	int				iminmtu;
	int				imaxmtu;
	int				isendquota;
	int				irecvquota;
	GUID			uuidService;
	unsigned int	uiportflags;
} PORTEMUPortParams;

//	Bluetooth serial IOCTLs are cross-defined in pegdser.h to reserve spot there.
#define IOCTL_BLUETOOTH_GET_RFCOMM_CHANNEL	CTL_CODE(FILE_DEVICE_SERIAL_PORT,24,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_BLUETOOTH_GET_PEER_DEVICE 	CTL_CODE(FILE_DEVICE_SERIAL_PORT,25,METHOD_BUFFERED,FILE_ANY_ACCESS)

//
// Bluetooth notification system
//

HANDLE RequestBluetoothNotifications
(
DWORD dwClass,	// class of notifications to register for
HANDLE hMsgQ	// message queue created by caller
);

BOOL StopBluetoothNotifications
(
HANDLE h		// Handle returned from RequestBluetoothNotifications
);

typedef struct _BTEVENT {
    DWORD dwEventId;        // Event ID    
    DWORD dwReserved;       // Reserved
    BYTE baEventData[64];   // Event Data
} BTEVENT, *PBTEVENT;

//
// Class of events to notify connections going up/down
// and other connection-related changes (role, mode).
//
#define BTE_CLASS_CONNECTIONS       1

#define BTE_CONNECTION              100
#define BTE_DISCONNECTION           101
#define BTE_ROLE_SWITCH             102
#define BTE_MODE_CHANGE             103
#define BTE_PAGE_TIMEOUT            104
#define BTE_CONNECTION_FAILED       105

typedef struct {
    DWORD dwSize;         // To keep track of version
    USHORT hConnection;   // Baseband connection handle
    BT_ADDR bta;          // Address of remote device
    UCHAR ucLinkType;     // Link Type (ACL/SCO)
    UCHAR ucEncryptMode;  // Encryption mode
} BT_CONNECT_EVENT, *PBT_CONNECT_EVENT;

typedef struct {
    DWORD dwSize;         // To keep track of version
    UCHAR ucStatus;       // Connection status
    BT_ADDR bta;          // Address of remote device
    UCHAR ucLinkType;     // Link type (ACL/SCO)
} BT_CONNECTION_FAILED_EVENT, *PBT_CONNECTION_FAILED_EVENT;

typedef struct {
    DWORD dwSize;         // To keep track of version
    USHORT hConnection;   // Baseband connection handle
    UCHAR ucReason;       // Reason for disconnection
} BT_DISCONNECT_EVENT, *PBT_DISCONNECT_EVENT;

typedef struct {
    DWORD dwSize;         // To keep track of version
    BT_ADDR bta;          // Address of remote device
    UINT fRole : 1;       // New Role (master/slave)
} BT_ROLE_SWITCH_EVENT, *PBT_ROLE_SWITCH_EVENT;

typedef struct {
    DWORD dwSize;         // To keep track of version
    USHORT hConnection;   // Baseband connection handle
    BT_ADDR bta;          // Address of remote device
    BYTE bMode;           // Power mode (sniff, etc)
    USHORT usInterval;    // Power mode interval 
} BT_MODE_CHANGE_EVENT, *PBT_MODE_CHANGE_EVENT;

//
// Class of events to notify changes in pairing.
//
#define BTE_CLASS_PAIRING		2

#define BTE_KEY_NOTIFY			200
#define BTE_KEY_REVOKED			201

typedef struct {
    DWORD dwSize;        // To keep track of version
    BT_ADDR bta;         // Address of remote device
    UCHAR link_key[16];  // Link key data
    UCHAR key_type;      // Link key type
} BT_LINK_KEY_EVENT, *PBT_LINK_KEY_EVENT;

//
// Class of events to notify changes specific to the 
// local device (cod, name)
//
#define BTE_CLASS_DEVICE		4

#define BTE_LOCAL_NAME			300
#define BTE_COD					301

//
// Class of events to notify change of state of the
// core stack.
//
#define BTE_CLASS_STACK			8

#define BTE_STACK_UP			400
#define BTE_STACK_DOWN			401

//
// Class of events to notify change of state of AVDTP
//
#define BTE_CLASS_AVDTP			16

#define BTE_AVDTP_STATE			500

#define BT_AVDTP_STATE_DISCONNECTED     0
#define BT_AVDTP_STATE_SUSPENDED        1
#define BT_AVDTP_STATE_STREAMING        2

typedef struct {
    DWORD dwSize;       // To keep track of version
    BT_ADDR bta;        // Address of remote device
    DWORD dwState;      // New state of the AVDTP stream
} BT_AVDTP_STATE_CHANGE, *PBT_AVDTP_STATE_CHANGE;

//
// Class of events to notify change of PAN state
//
#define BTE_CLASS_PAN           32

#define BTE_PAN_CONNECTIONS     600

typedef struct {
    DWORD dwSize;       // To keep track of version
    DWORD NumConnections; // Number of peers connected
} BT_PAN_NUM_CONNECTIONS, *PBT_PAN_NUM_CONNECTIONS;

//
// PAN APIs
//

int BthActivatePAN (BOOL fActivate);


#if defined (UNDER_CE)
#define rfRegisterDevice RegisterDevice
#define rfDeregisterDevice DeregisterDevice
#define rfCreateFile CreateFile
#define rfReadFile ReadFile
#define rfWriteFile WriteFile
#define rfCloseHandle CloseHandle
#else 
HANDLE rfRegisterDevice
(
LPCWSTR lpszType,
DWORD	dwIndex,
LPCWSTR lpszLib,
DWORD	dwInfo
);

BOOL rfDeregisterDevice
(
HANDLE hDevice
);

HANDLE rfCreateFile
(
LPCTSTR lpFileName,                         // file name
DWORD dwDesiredAccess,                      // access mode
DWORD dwShareMode,                          // share mode
LPSECURITY_ATTRIBUTES lpSecurityAttributes, // SD
DWORD dwCreationDisposition,                // how to create
DWORD dwFlagsAndAttributes,                 // file attributes
HANDLE hTemplateFile                        // handle to template file
);

BOOL rfReadFile(
  HANDLE hFile,                // handle to file
  LPVOID lpBuffer,             // data buffer
  DWORD nNumberOfBytesToRead,  // number of bytes to read
  LPDWORD lpNumberOfBytesRead, // number of bytes read
  LPOVERLAPPED lpOverlapped    // overlapped buffer
  );

BOOL rfWriteFile(
  HANDLE hFile,                    // handle to file
  LPCVOID lpBuffer,                // data buffer
  DWORD nNumberOfBytesToWrite,     // number of bytes to write
  LPDWORD lpNumberOfBytesWritten,  // number of bytes written
  LPOVERLAPPED lpOverlapped        // overlapped buffer
  );

BOOL rfCloseHandle (HANDLE hFile);
#endif

#if defined (__cplusplus)
};			// __cplusplus
#endif

#endif		/* __bt_api_H__ */


