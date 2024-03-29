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

    wzcsapi.h
    
Abstract:
    API for Wireless Zero Configuration interface.

Notes: 


--*/
#ifndef _WZCAPI_H_
#define _WZCAPI_H_

#ifdef UNDER_CE
#ifdef MIDL_PASS
#undef MIDL_PASS
#endif
#endif

#pragma once

# ifdef     __cplusplus
extern "C" {
# endif

#ifndef UNDER_CE
PVOID
MIDL_user_allocate(size_t NumBytes);

VOID
MIDL_user_free(void * MemPointer);
#endif	//	UNDER_CE

//---------------------------------------
// Macros for handling additional attributes on WZC_WLAN_CONFIG structures
// Coding of additional attributes in the Reserved bytes of WZC_WLAN_CONFIG objects:
// Reserved
// [1]      [0]
// ---SSSAA CCCCCCCC
// SSS = [0-7; used: 0-6] selection set category, one of VPI, VI, PI, VPA, VA, PA, N
// AA  = [0-3; used: 0-3] authentication mode, NDIS_802_11_AUTHENTICATION_MODE value
// CCCCCCCC = [0-255] retry counter for this object.
//
#define NWB_AUTHMODE_MASK       0x03
#define NWB_SELCATEG_MASK       0x1C

#ifndef UNDER_CE

#define NWB_SET_AUTHMODE(pNWB, nAM)     (pNWB)->Reserved[1] = (((pNWB)->Reserved[1] & ~NWB_AUTHMODE_MASK) | ((nAM) & NWB_AUTHMODE_MASK))
#define NWB_GET_AUTHMODE(pNWB)          ((pNWB)->Reserved[1] & NWB_AUTHMODE_MASK)
#define NWB_SET_SELCATEG(pNWB, nSC)     (pNWB)->Reserved[1] = (((pNWB)->Reserved[1] & ~NWB_SELCATEG_MASK) | (((nSC)<<2) & NWB_SELCATEG_MASK))
#define NWB_GET_SELCATEG(pNWB)          (((pNWB)->Reserved[1] & NWB_SELCATEG_MASK)>>2)

#else

#define NWB_SET_AUTHMODE(pNWB, nAM)     (pNWB)->Reserved[1] = (UCHAR)(((pNWB)->Reserved[1] & ~NWB_AUTHMODE_MASK) | ((nAM) & NWB_AUTHMODE_MASK))
#define NWB_GET_AUTHMODE(pNWB)          ((pNWB)->Reserved[1] & NWB_AUTHMODE_MASK)
#define NWB_SET_SELCATEG(pNWB, nSC)     (pNWB)->Reserved[1] = (UCHAR)(((pNWB)->Reserved[1] & ~NWB_SELCATEG_MASK) | (((nSC)<<2) & NWB_SELCATEG_MASK))
#define NWB_GET_SELCATEG(pNWB)          (((pNWB)->Reserved[1] & NWB_SELCATEG_MASK)>>2)

#endif


//---------------------------------------
// [P]RAW_DATA: generic description of a BLOB
typedef struct
{
    DWORD   dwDataLen;
#if defined(MIDL_PASS)
    [unique, size_is(dwDataLen)] LPBYTE pData;
#else
    LPBYTE  pData;
#endif
} RAW_DATA, *PRAW_DATA;

#include <windot11.h>
#include <wlclient.h>

#if !defined(MIDL_PASS)

#include <ntddndis.h>


#ifdef UNDER_CE
typedef struct _WZC_EAPOL_PARAMS
{
	BOOL	bEnable8021x;
    DWORD   dwEapFlags;
    DWORD   dwEapType;    
    DWORD   dwAuthDataLen;				
    BYTE    *pbAuthData;				   // Pointer to provider specific config blob

}	WZC_EAPOL_PARAMS, *PWZC_EAPOL_PARAMS;


typedef struct _EAP_EXTENSIONS
{
	DWORD					dwNumOfExtension;
	PEAP_EXTENSION_INFO		pEapExtensionInfo;

}	EAP_EXTENSIONS, *PEAP_EXTENSIONS;

#endif


#define WZCCTL_MAX_WEPK_MATERIAL   32
#define WZCCTL_WEPK_PRESENT        0x0001  // specifies whether the configuration includes or not a WEP key
#define WZCCTL_WEPK_XFORMAT        0x0002  // the WEP Key material (if any) is entered as hexadecimal digits
#define WZCCTL_VOLATILE            0x0004  // this configuration should not be stored
#define WZCCTL_POLICY              0x0008  // this configuration is enforced by the policy
#define WZCCTL_ONEX_ENABLED        0x0010  // for this configuration 802.1X should be enabled
#define WZCCTL_DOMAIN_NAME         0x0020
#define WZCCTL_GUEST_CREDENTIALS   0x0040
#define WZCCTL_TEMP_CONFIG         0x0080
#define WZCCTL_MANUAL_CONNECT      0x0100
#define WZCCTL_DISCONNECTED        0x0200
#define WZCCTL_MEDIA_CONNECTED     0x0400
#define WZCCTL_EX_CAPABILITIES_IE  0x0800
#define WZCCTL_WPS_NETWORK         0x1000
#define WZCCTL_CFG_NOT_SUPPORTED   0x2000
#define WZCCTL_BROADCAST_SSID      0x4000



//---------------------------------------
// [P]WZC_WLAN_CONFIG: like NDIS_WLAN_BSSID, but contains all the additional
// data that defines a [Preferred] Wireless Zero Configuration
typedef struct
{
    ULONG                               Length;             // Length of this structure
    DWORD                               dwCtlFlags;         // control flags (NON-NDIS) see WZC_WEPK* constants
    // fields from the NDIS_WLAN_BSSID structure
    NDIS_802_11_MAC_ADDRESS             MacAddress;         // BSSID
    UCHAR                               Reserved[2];
    NDIS_802_11_SSID                    Ssid;               // SSID
    ULONG                               Privacy;            // WEP encryption requirement
    NDIS_802_11_RSSI                    Rssi;               // receive signal strength in dBm
    NDIS_802_11_NETWORK_TYPE            NetworkTypeInUse;
    NDIS_802_11_CONFIGURATION           Configuration;
    NDIS_802_11_NETWORK_INFRASTRUCTURE  InfrastructureMode;
    NDIS_802_11_RATES                   SupportedRates;
    // fields from NDIS_802_11_WEP structure
    ULONG   KeyIndex;                               // 0 is the per-client key, 1-N are the global keys
    ULONG   KeyLength;                              // length of key in bytes
    UCHAR   KeyMaterial[WZCCTL_MAX_WEPK_MATERIAL];  // variable length depending on above field
    // aditional field for the Authentication mode
    NDIS_802_11_AUTHENTICATION_MODE     AuthenticationMode;
    RAW_DATA                            rdUserData;         // upper level buffer, attached to this config

#ifdef UNDER_CE
    WZC_EAPOL_PARAMS                    EapolParams;		// 802.1x parameters
#endif

    RAW_DATA                            rdNetworkData;      // upper level buffer, attached to this config
    ULONG                               WPAMCastCipher;     // Multicast Cipher Suite for WPA. 
    ULONG                               ulMediaType;        // 

} WZC_WLAN_CONFIG, *PWZC_WLAN_CONFIG;

//---------------------------------------
// [P]WZC_802_11_CONFIG_LIST: like NDIS_802_11_BSSID_LIST but indexes a
// set of [Preferred] Wireless Zero Configurations
typedef struct
{
    ULONG           NumberOfItems;  // number of elements in the array below
    ULONG           Index;          // [start] index in the array below
    WZC_WLAN_CONFIG Config[1];      // array of WZC_WLAN_CONFIGs
} WZC_802_11_CONFIG_LIST, *PWZC_802_11_CONFIG_LIST;

// WZC dialog codes have the 16th bit set to 1. This is what quickly sepparates them from EAPOL signals.
#define WZCDLG_IS_WZC(x)         (((x) & 0x00010000) == 0x00010000)
#define WZCDLG_FAILED            0x00010001     // 802.11 automatic configuration failed

// Dialog BLOB passed through the UI pipe to netman and wzcdlg
typedef struct _WZCDLG_DATA
{
    DWORD       dwCode;
    DWORD       lParam; // long numeric data
} WZCDLG_DATA, *PWZCDLG_DATA;

#endif

//---------------------------------------
// [P]INTF_ENTRY: describes the key info for one interface
// this is used in conjunction with [P]INTFS_KEY_TABLE and WZCEnumInterfaces
typedef struct
{
#if defined(MIDL_PASS)
    [unique, string] LPWSTR wszGuid;
#else
    LPWSTR wszGuid;
#endif
} INTF_KEY_ENTRY, *PINTF_KEY_ENTRY;

//---------------------------------------
// [P]INTFS_KEY_TABLE: describes the table of key info for all interfaces
// this is used in conjunction with [P]INTF_KEY_ENTRY and WZCEnumInterfaces
typedef struct
{
    DWORD dwNumIntfs;
#if defined(MIDL_PASS)
    [size_is(dwNumIntfs)] PINTF_KEY_ENTRY pIntfs;
#else
    PINTF_KEY_ENTRY pIntfs;
#endif
} INTFS_KEY_TABLE, *PINTFS_KEY_TABLE;

//---------------------------------------
// Bits used in conjunction with INTF_ENTRY, WZCQueryInterface
// and WZCSetInterface. They point to the relevant information
// that is requested from the service or to the relevant information
// to be set down to the interface. On the output, they point to
// the information that was processed (queried/set) successfully.
#ifdef UNDER_CE
#define INTF_ALL            0x7fffffff
#else
#define INTF_ALL            0xffffffff
#endif

#define INTF_ALL_FLAGS      0x0000ffff
#define INTF_CM_MASK        0x00000007   // mask for the configuration mode (NDIS_802_11_NETWORK_INFRASTRUCTURE value)
#define INTF_ENABLED        0x00008000   // zero conf enabled for this interface
#define INTF_FALLBACK       0x00004000   // attempt to connect to visible non-preferred networks also
#define INTF_OIDSSUPP       0x00002000   // 802.11 OIDs are supported by the driver/firmware (can't be set)
#define INTF_VOLATILE       0x00001000   // the service parameters are volatile.
#define INTF_POLICY         0x00000800   // the service parameters are enforced by the policy.

#define INTF_DESCR          0x00010000
#define INTF_NDISMEDIA      0x00020000
#define INTF_PREFLIST       0x00040000
#define INTF_CAPABILITIES   0x00080000


#ifdef UNDER_CE
#define INTF_ALL_OIDS       0x7ff00000
#else
#define INTF_ALL_OIDS       0xfff00000
#endif
#define INTF_HANDLE         0x00100000
#define INTF_INFRAMODE      0x00200000
#define INTF_AUTHMODE       0x00400000
#define INTF_WEPSTATUS      0x00800000
#define INTF_SSID           0x01000000
#define INTF_BSSID          0x02000000
#define INTF_BSSIDLIST      0x04000000
#define INTF_LIST_SCAN      0x08000000
#define INTF_ADDWEPKEY      0x10000000
#define INTF_REMWEPKEY      0x20000000
#define INTF_LDDEFWKEY      0x40000000  // reload the default WEP_KEY

#ifdef UNDER_CE
#define INTF_PMKCACHE		0x80000000  // Check the status of the PMK Cache (get), or configure it (set)
#endif

//---------------------------------------
// Bits used to specify particular control options for the interface
// entry
#define INTFCTL_CM_MASK     0x0007   // mask for the configuration mode (NDIS_802_11_NETWORK_INFRASTRUCTURE value)
#define INTFCTL_ENABLED     0x8000   // zero conf enabled for this interface
#define INTFCTL_FALLBACK    0x4000   // attempt to connect to visible non-preferred networks also
#define INTFCTL_OIDSSUPP    0x2000   // 802.11 OIDs are supported by the driver/firmware (can't be set)
#define INTFCTL_VOLATILE    0x1000   // the service parameters are volatile.
#define INTFCTL_POLICY      0x0800   // the service parameters policy enforced.

#ifdef UNDER_CE
#define INTFCTL_8021XSUPP   0x1000   // 802.1x support enabled
#endif

//---------------------------------------
// Bits used to specify various capabilities of the interface
#define INTFCAP_MAX_CIPHER_MASK     0x000000ff
#define INTFCAP_SSN                 0x00000100
#define INTFCAP_80211I              0x00000200


//---------------------------------------
// [P]INTF_ENTRY: contains everything an RPC client needs to know
// about an interface. It is used in conjunction with RpcQueryInterface.
// Flags below are to be used to specify what info is queried for the
// interface. Guid field is not covered since this is the key of the
// structure so it has to be specified eather way.
typedef struct
{    
#if defined(MIDL_PASS)
    [string] LPWSTR wszGuid;
#else
    LPWSTR          wszGuid;
#endif
#if defined(MIDL_PASS)
    [string] LPWSTR wszDescr;
#else
    LPWSTR          wszDescr;
#endif
    ULONG           ulMediaState;
    ULONG           ulMediaType;
    ULONG           ulPhysicalMediaType;
    INT             nInfraMode;
    INT             nAuthMode;
    INT             nWepStatus;
    DWORD           dwCtlFlags;     // control flags (see INTFCTL_* defines)
    DWORD           dwCapabilities; // capabilities flags (see INTFCAP_* defines)
    RAW_DATA        rdSSID;         // encapsulates the SSID raw binary
    RAW_DATA        rdBSSID;        // encapsulates the BSSID raw binary
    RAW_DATA        rdBSSIDList;    // encapsulates one WZC_802_11_CONFIG_LIST structure
    RAW_DATA        rdStSSIDList;   // encapsulates one WZC_802_11_CONFIG_LIST structure
    RAW_DATA        rdCtrlData;     // data for various control actions on the interface

#ifdef UNDER_CE
    BOOL            bInitialized;   //  To track caller that freeing
                                    //  the same structure more than one time..
#endif                                    
    DWORD           nWPAMCastCipher;

} INTF_ENTRY, *PINTF_ENTRY;


#ifdef UNDER_CE

typedef struct
{
	DWORD dwNumOfPMKIDs;
	DWORD dwNumOfAuthEncryptPairs;
	NDIS_802_11_AUTHENTICATION_ENCRYPTION AuthEncryptPair[1];
} INTF_80211_CAPABILITY, *PINTF_80211_CAPABILITY;


//  INTF_ENTRY_EX contains the rdNicCapabilities and also ulVersion.
//  This is to prevent existing modules using WZC from breaking.

#define INTF_ENTRY_V1       0x00000001

typedef struct
{    
#if defined(MIDL_PASS)
    [string] LPWSTR wszGuid;
#else
    LPWSTR          wszGuid;
#endif
#if defined(MIDL_PASS)
    [string] LPWSTR wszDescr;
#else
    LPWSTR          wszDescr;
#endif
    ULONG           ulMediaState;
    ULONG           ulMediaType;
    ULONG           ulPhysicalMediaType;
    INT             nInfraMode;
    INT             nAuthMode;
    INT             nWepStatus;
    DWORD           dwCtlFlags;     // control flags (see INTFCTL_* defines)
    DWORD           dwCapabilities; // capabilities flags (see INTFCAP_* defines)
    RAW_DATA        rdSSID;         // encapsulates the SSID raw binary
    RAW_DATA        rdBSSID;        // encapsulates the BSSID raw binary
    RAW_DATA        rdBSSIDList;    // encapsulates one WZC_802_11_CONFIG_LIST structure
    RAW_DATA        rdStSSIDList;   // encapsulates one WZC_802_11_CONFIG_LIST structure
    RAW_DATA        rdCtrlData;     // data for various control actions on the interface  
#ifdef UNDER_CE
    BOOL            bInitialized;   //  To track caller that freeing
                                    //  the same structure more than one time..
#endif                                    
    DWORD           nWPAMCastCipher;

    ULONG           ulVersion;
    RAW_DATA        rdNicCapabilities;  // points to INTF_80211_CAPABILITY.
    RAW_DATA        rdPMKCache;         // points to NDIS_802_11_PMKID.
	ULONG           PMKCacheFlags;
#define		INTF_ENTRY_PMKCACHE_FLAG_ENABLE					(1<<0)  // Enable/disable the cache
#define		INTF_ENTRY_PMKCACHE_FLAG_ENABLE_OPPORTUNISTIC	(1<<1)  // Enable/disable opportunistic PMKIDs
#define		INTF_ENTRY_PMKCACHE_FLAG_ENABLE_PREAUTH			(1<<2)  // Enable/disable preauthentication
#define		INTF_ENTRY_PMKCACHE_FLAG_FLUSH					(1<<31) // Set only, causes PMK

} INTF_ENTRY_EX, *PINTF_ENTRY_EX;

#endif



    

//---------------------------------------
// Defines and datastucture for handling the WZC Service Context (generic service params)

// Default values for WZC internal timers (WZC_CONTEXT.tmT* fields)

#ifdef UNDER_CE

#define TMMS_INFINITE   0x70000000        
#define TMMS_DEFAULT_TR	0x00000bb8      // Timeout until a rescan completes: ms (3sec)
#define TMMS_DEFAULT_TC TMMS_INFINITE   // Timeout to retry a valid configuration: ms (Don't rescan!) 
#define TMMS_DEFAULT_TP 0x000007d0      // Timeout to expect a media connect for a selected config: ms (2sec)
#define TMMS_DEFAULT_TF 0x0000ea60      // Timeout to recover from a failed configuration: ms (1min)
#define TMMS_DEFAULT_TD 0x00001388      // Timeout to delay the {SSr} processing: ms (5sec)

#else   //  UNDER_CE

#define TMMS_DEFAULT_TR	0x00000bb8      // Timeout until a rescan completes: ms (3sec)
#define TMMS_DEFAULT_TC 0x0000ea60      // Timeout to retry a valid configuration: ms (1min) 
#define TMMS_DEFAULT_TP 0x000007d0      // Timeout to expect a media connect for a selected config: ms (2sec)
#define TMMS_DEFAULT_TF 0x0000ea60      // Timeout to recover from a failed configuration: ms (1min)
#define TMMS_DEFAULT_TD 0x00001388      // Timeout to delay the {SSr} processing: ms (5sec)

#endif  //  UNDER_CE

// Default turns logging on
#define WZC_CTXT_LOGGING_ON      0x00000001 

// Structure: WZC_CONTEXT holds all global service options that may be customized
typedef struct _wzc_context_t
{
#ifndef UNDER_CE
  //
  //    This flag is not used in CE.
  //    Application calling WZCSetContext() should get the initial value using
  //    WZCQueryContext().
  //    Any entry set to 0xffffffff will automatically be replaced with default value hard
  //    coded in ZeroConfiguration (see TMMS_DEFAULT_XXX above).
  //  
  DWORD dwFlags;            //service flags (see WZC_CTXT_*)
#endif

  //Service specific timers  
  DWORD tmTr;               //Rescan timeout
  DWORD tmTc;               //Retry valid config timeout
  DWORD tmTp;               //Timeout to expect a media connect
  DWORD tmTf;               //Timeout to recover from an invalid config
  DWORD tmTd;               //Timeout to delay {Ssr} processing
} WZC_CONTEXT, *PWZC_CONTEXT;

// Context control flags (see WZCSet/QueryContext() calls)
#define WZC_CONTEXT_CTL_LOG         0x00000001
#define WZC_CONTEXT_CTL_TIMER_TR	0x00000002
#define WZC_CONTEXT_CTL_TIMER_TC	0x00000004
#define WZC_CONTEXT_CTL_TIMER_TP	0x00000008
#define WZC_CONTEXT_CTL_TIMER_TF	0x00000010
#define WZC_CONTEXT_CTL_TIMER_TD	0x00000020


//---------------------------------------
// Utility Rpc memory management routines
#define RpcCAlloc(nBytes)   MIDL_user_allocate(nBytes)
#define RpcFree(pMem)       MIDL_user_free(pMem)


//---------------------------------------
// GetSPResModule: Utility function used to return
// the handle to the module having WZC UI resources
// (needed for XP.QFE & XP.SP1 builds)
HINSTANCE
WZCGetSPResModule();

//---------------------------------------
// WZCDeleteIntfObj: cleans an INTF_ENTRY object that is
// allocated within any RPC call.
// 
// Parameters
// pIntf
//     [in] pointer to the INTF_ENTRY object to delete
VOID
WZCDeleteIntfObj(
    PINTF_ENTRY pIntf);


//  The Ex version requires PINTF_ENTRY_EX.
VOID
WZCDeleteIntfObjEx(
    PINTF_ENTRY_EX pIntf);


//---------------------------------------
// WZCPassword2Key: Translates a user password (8 to 63 ascii chars)
// into a 256 bit network key)
VOID
WZCPassword2Key(
    PWZC_WLAN_CONFIG pwzcConfig,
    LPCSTR cszPassword); 

//---------------------------------------
    


#ifdef UNDER_CE

//
//  Only in CE..
//  This function enumerates all the EAP extensions currently installed	
//  in the system..
//

DWORD
WZCEnumEapExtensions(
    DWORD               *pdwNumOfExtensions,
    PEAP_EXTENSION_INFO *pEapExtensions);

#endif

//---------------------------------------
// WZCEnumInterfaces: provides the table of key
// information for all the interfaces that are managed.
// For all subsequent calls the clients need to identify
// the Interface it operates on by providing the respective
// key info.
//
// Parameters:
//   pSrvAddr
//     [in] WZC Server to contact
//   pIntf
//     [out] table of key info for all interfaces
// Returned value:
//     Win32 error code 
DWORD
WZCEnumInterfaces(
    LPWSTR           pSrvAddr,
    PINTFS_KEY_TABLE pIntfs);


//---------------------------------------
// WZCQueryIterface: provides detailed information for a
// given interface.
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   dwInFlags:
//     [in]  Fields to be queried (bitmask of INTF_*)
//   pIntf:
//     [in]  Key of the interface to query
//     [out] Requested data from the interface.
//   pdwOutFlags
//     [out] Fields successfully retrieved (bitmask of INTF_*)
//
// Returned value:
//     Win32 error code 
DWORD
WZCQueryInterface(
    LPWSTR              pSrvAddr,
    DWORD               dwInFlags,
    PINTF_ENTRY         pIntf,
    LPDWORD             pdwOutFlags);


//  The Ex version requires PINTF_ENTRY_EX.
DWORD
WZCQueryInterfaceEx(
    LPWSTR              pSrvAddr,
    DWORD               dwInFlags,
    PINTF_ENTRY_EX      pIntfEx,
    LPDWORD             pdwOutFlags);



//---------------------------------------
// WZCSetIterface: sets specific information on the interface
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   dwInFlags:
//     [in]  Fields to be set (bitmask of INTF_*)
//   pIntf:
//     [in]  Key of the interface to query and data to be set
//   pdwOutFlags:
//     [out] Fields successfully set (bitmask of INTF_*)
//
// Returned value:
//     Win32 error code 
DWORD
WZCSetInterface(
    LPWSTR              pSrvAddr,
    DWORD               dwInFlags,
    PINTF_ENTRY         pIntf,
    LPDWORD             pdwOutFlags);

//  The Ex version requires PINTF_ENTRY_EX.
DWORD
WZCSetInterfaceEx(
    LPWSTR              pSrvAddr,
    DWORD               dwInFlags,
    PINTF_ENTRY_EX      pIntfEx,
    LPDWORD             pdwOutFlags);

//---------------------------------------
// WZCRefreshInterface: refreshes specific information for the interface
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   dwInFlags:
//     [in]  Fields to be refreshed and specific refresh actions to be
//           taken (bitmask of INTF_*)
//   pIntf:
//     [in]  Key of the interface to be refreshed
//   pdwOutFlags:
//     [out] Fields successfully refreshed (bitmask of INTF_*)
//
// Returned value:
//     Win32 error code 
DWORD
WZCRefreshInterface(
    LPWSTR              pSrvAddr,
    DWORD               dwInFlags,
    PINTF_ENTRY         pIntf,
    LPDWORD             pdwOutFlags);

//  The Ex version requires PINTF_ENTRY_EX.
DWORD
WZCRefreshInterfaceEx(
    LPWSTR              pSrvAddr,
    DWORD               dwInFlags,
    PINTF_ENTRY_EX      pIntfEx,
    LPDWORD             pdwOutFlags);


//---------------------------------------
// WZCQueryContext: retrieves the WZC service parameters
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   dwInFlags:
//     [in]  Fields to be retrieved (bitmask of WZC_CONTEXT_CTL*)
//   pContext:
//     [in]  Placeholder for the service parameters
//   pdwOutFlags:
//     [out] Fields successfully retrieved (bitmask of WZC_CONTEXT_CTL*)
//
// Returned value:
//     Win32 error code 


#ifdef UNDER_CE
//
//  In CE, only pContext parameter is necessary.
//
#endif

DWORD
WZCQueryContext(
    LPWSTR              pSrvAddr,
    DWORD               dwInFlags,
    PWZC_CONTEXT        pContext,
    LPDWORD             pdwOutFlags);


//---------------------------------------
// WZCSetContext: sets specific WZC service parameters
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   dwInFlags:
//     [in]  Fields to be set (bitmask of WZC_CONTEXT_CTL*)
//   pContext:
//     [in]  Context buffer containing the specific parameters to be set
//   pdwOutFlags:
//     [out] Fields successfully set (bitmask of WZC_CONTEXT_CTL*)
//
// Returned value:
//     Win32 error code 

#ifdef UNDER_CE
//
//  In CE, only pContext parameter is necessary.
//
#endif

DWORD
WZCSetContext(
    LPWSTR              pSrvAddr,
    DWORD               dwInFlags,
    PWZC_CONTEXT        pContext,
    LPDWORD             pdwOutFlags);



#ifdef UNDER_CE
//
//  Support for OID_802_11_LATENCY_SENSITIVE_MODE NDIS OID.
//

DWORD
WZCSetLatencySensitiveMode(
    LPWSTR              pSrvAddr,         // Should always be NULL on CE
    PWSTR               wszInterfaceGuid, // Interface name
    BOOL                bEnable,          // Set to TRUE when a VOIP call starts, FALSE when VOIP call ends
    ULONG               PacketLatencyMs); // Codec dependent expected packet period in milliseconds (e.g. set to 20 if VOIP packet will be sent every 20ms.)

DWORD
WZCQueryLatencySensitiveMode(
    LPWSTR              pSrvAddr,           // Should always be NULL on CE
    PWSTR               wszInterfaceGuid,
    BOOL                *pbEnable,          
    ULONG               *pPacketLatencyMs);

#endif
//
// EAPOL-related definitions (not used in CE).
//

//============================================================================================
//
// EAPOL-related definitions
//


#define EAPOL_DISABLED                  0
#define EAPOL_ENABLED                   0x80000000

#define EAPOL_MACHINE_AUTH_DISABLED     0
#define EAPOL_MACHINE_AUTH_ENABLED      0x40000000

#define EAPOL_GUEST_AUTH_DISABLED       0
#define EAPOL_GUEST_AUTH_ENABLED        0x20000000

#define EAP_TYPE_MD5                    4
#define EAP_TYPE_TLS                    13
#define EAP_TYPE_PEAP                   25
#define EAP_TYPE_MSCHAPv2               26

#define DEFAULT_EAP_TYPE                EAP_TYPE_TLS
#define DEFAULT_EAPOL_STATE             EAPOL_ENABLED
#define DEFAULT_MACHINE_AUTH_STATE      EAPOL_MACHINE_AUTH_ENABLED
#define DEFAULT_GUEST_AUTH_STATE        EAPOL_GUEST_AUTH_DISABLED

#define DEFAULT_EAP_STATE               (DEFAULT_EAPOL_STATE | DEFAULT_MACHINE_AUTH_STATE | DEFAULT_GUEST_AUTH_STATE)

#define IS_EAPOL_ENABLED(x) \
    ((x & EAPOL_ENABLED)?1:0)
#define IS_MACHINE_AUTH_ENABLED(x) \
    ((x & EAPOL_MACHINE_AUTH_ENABLED)?1:0)
#define IS_GUEST_AUTH_ENABLED(x) \
    ((x & EAPOL_GUEST_AUTH_ENABLED)?1:0)

// Supplicant modes of operation depending on network state and 
// administrator decision

#define     SUPPLICANT_MODE_0       0
#define     SUPPLICANT_MODE_1       1
#define     SUPPLICANT_MODE_2       2
#define     SUPPLICANT_MODE_3       3
#define     MAX_SUPPLICANT_MODE     SUPPLICANT_MODE_3
#define     EAPOL_DEFAULT_SUPPLICANT_MODE   SUPPLICANT_MODE_2

// Auth modes of operation depending on administrator decision

#define     EAPOL_AUTH_MODE_0       0
#define     EAPOL_AUTH_MODE_1       1
#define     EAPOL_AUTH_MODE_2       2
#define     MAX_EAPOL_AUTH_MODE     EAPOL_AUTH_MODE_2
#define     EAPOL_DEFAULT_AUTH_MODE   EAPOL_AUTH_MODE_1

#define     GUID_STRING_LEN_WITH_TERM   39

//
// Heap-related functions
//
#define MALLOC(s)               HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (s))
#define FREE(p)                 HeapFree(GetProcessHeap(), 0, (p))


// Double-threaded linked list node control block.  There is one node for each
// entry in a list.
//
// Applications should not access this structure directly.
//
typedef struct
_DTLNODE
{
    struct _DTLNODE* pdtlnodePrev; // Address of previous node or NULL if none
    struct _DTLNODE* pdtlnodeNext; // Address of next node or NULL if none
    VOID*    pData;        // Address of user's data
    LONG_PTR lNodeId;      // User-defined node identification code
}
DTLNODE;


//
// Double-threaded linked list control block.  There is one for each list.
//
// Applications should not access this structure directly.
//

typedef struct
_DTLLIST
{
    struct _DTLNODE* pdtlnodeFirst; // Address of first node or NULL if none
    struct _DTLNODE* pdtlnodeLast;  // Address of last node or NULL if none
    LONG     lNodes;        // Number of nodes in list
    LONG_PTR lListId;       // User-defined list identification code
}
DTLLIST;


// List node free function.  See FreeList.
//
typedef VOID (*PDESTROYNODE)( IN DTLNODE* );

#define DtlGetFirstNode( pdtllist )   ((pdtllist)->pdtlnodeFirst)
#define DtlGetNextNode( pdtlnode )    ((pdtlnode)->pdtlnodeNext)
#define DtlGetData( pdtlnode )        ((pdtlnode)->pData)

typedef enum _EAPTLS_CONNPROP_ATTRIBUTE_TYPE_
{

    ecatMinimum = 0,            //Undefined
    ecatFlagRegistryCert,       //Value is a pointer to BOOL
    ecatFlagScard,              //Value is a pointer to BOOL
    ecatFlagValidateServer,     //Value is a pointer to BOOL
    ecatFlagValidateName,       //Value is a pointer to BOOL
    ecatFlagDiffUser,           //Value is a pointer to BOOL
    ecatServerNames,            //Value is a pointer to NULL 
                                //terminated string of semi
                                //colon delimited server names
    ecatRootHashes              //Value is a pointer to 
                                //SHA1 hashes of Root certs.

}EAPTLS_CONNPROP_ATTRIBUTE_TYPE;

typedef struct _EAPTLS_CONNPROP_ATTRIBUTE
{

    EAPTLS_CONNPROP_ATTRIBUTE_TYPE  ecaType;
    DWORD                           dwLength;   //includes byte length of the value
                                                //if it is a LPWSTR, it includes
                                                //the null termination.
    PVOID                           Value;

}EAPTLS_CONNPROP_ATTRIBUTE, *PEAPTLS_CONNPROP_ATTRIBUTE;


// EAP configuration DLL entrypoints.  These definitions must match the
// raseapif.h prototypes for RasEapInvokeConfigUI and RasEapFreeUserData.

typedef DWORD (APIENTRY * RASEAPFREE)( PBYTE );
typedef DWORD (APIENTRY * RASEAPINVOKECONFIGUI)( DWORD, HWND, DWORD, PBYTE, DWORD, PBYTE*, DWORD*);
typedef DWORD (APIENTRY * RASEAPGETIDENTITY)( DWORD, HWND, DWORD, const WCHAR*, const WCHAR*, PBYTE, DWORD, PBYTE, DWORD, PBYTE*, DWORD*, WCHAR** );
typedef DWORD (APIENTRY * RASEAPINVOKEINTERACTIVEUI)( DWORD, HWND, PBYTE, DWORD, PBYTE*, DWORD* );
typedef DWORD (APIENTRY * RASEAPCREATECONNPROP)( PEAPTLS_CONNPROP_ATTRIBUTE, PVOID*, DWORD*, PVOID*, DWORD*);


#define RAS_EAP_VALUENAME_HIDEPEAPMSCHAPv2       TEXT("HidePEAPMSCHAPv2")

// Flags

#define EAPCFG_FLAG_RequireUsername   0x1
#define EAPCFG_FLAG_RequirePassword   0x2

// EAP configuration package definition.

typedef struct
_EAPCFG
{
    // The package's unique EAP algorithm code.
    //
    DWORD dwKey;

    // The friendly name of the package suitable for display to the user.
    //
    TCHAR* pszFriendlyName;

    // The SystemRoot-relative path to the package's configuration DLL.  May
    // be NULL indicating there is none.
    //
    TCHAR* pszConfigDll;

    // The SystemRoot-relative path to the package's identity DLL.  May
    // be NULL indicating there is none.
    //
    TCHAR* pszIdentityDll;

    // Flags that specify what standard credentials are required at dial
    // time.
    //
    DWORD dwStdCredentialFlags;

    // True if user is to be forced to run the configuration API for the
    // package, i.e. defaults are not sufficient.
    //
    BOOL fForceConfig;

    // True if the package provides MPPE encryption keys, false if not.
    //
    BOOL fProvidesMppeKeys;

    // The package's default configuration blob, which can be overwritten by
    // the configuration DLL.  May be NULL and 0 indicating there is none.
    //
    BYTE* pData;
    DWORD cbData;

    // EAP per user data to be stored in HKCU. This data is returned from
    // the EapInvokeConfigUI entrypoint in the eap dll.
    //
    BYTE* pUserData;
    DWORD cbUserData;

    // Set when the configuration DLL has been called on the package.  This is
    // not a registry setting.  It is provided for the convenience of the UI
    // only.
    //
    BOOL fConfigDllCalled;

    // Specifies the class ID of the configuration UI for remote machines.
    // Not used
    GUID guidConfigCLSID;
} EAPCFG;

VOID     DtlDestroyList( DTLLIST*, PDESTROYNODE );

DTLNODE *
CreateEapcfgNode(
    void);

VOID
DestroyEapcfgNode(
    IN OUT DTLNODE* pNode);

DTLNODE*
EapcfgNodeFromKey(
    IN DTLLIST* pList,
    IN DWORD dwKey);

#define     EAPOL_MUTUAL_AUTH_EAP_ONLY      0x00000001

DTLLIST*
ReadEapcfgList(IN DWORD   dwFlags);

#define MAX_SSID_LEN    32

//
// Structure : EAPOL_INTF_PARAMS
//

typedef struct _EAPOL_INTF_PARAMS
{
    DWORD   dwVersion;
    DWORD   dwReserved2;
    DWORD   dwEapFlags;
    DWORD   dwEapType;
    DWORD   dwSizeOfSSID;
    BYTE    bSSID[MAX_SSID_LEN];
} EAPOL_INTF_PARAMS, *PEAPOL_INTF_PARAMS;


//
// EAPOL states
//

typedef enum _EAPOL_STATE 
{
    EAPOLSTATE_LOGOFF = 0,
    EAPOLSTATE_DISCONNECTED,
    EAPOLSTATE_CONNECTING,
    EAPOLSTATE_ACQUIRED,
    EAPOLSTATE_AUTHENTICATING,
    EAPOLSTATE_HELD,
    EAPOLSTATE_AUTHENTICATED,
    EAPOLSTATE_UNDEFINED
} EAPOL_STATE;

//
// EAP UI State 
//

typedef enum _EAPUISTATE 
{
    EAPUISTATE_WAITING_FOR_IDENTITY = 1,
    EAPUISTATE_WAITING_FOR_UI_RESPONSE
} EAPUISTATE;


//
// Structure : EAPOL_INTF_STATE
//

typedef struct _EAPOL_INTF_STATE
{
#if defined(MIDL_PASS)
    [unique, string]    LPWSTR    pwszLocalMACAddr;
#else
    LPWSTR      pwszLocalMACAddr;
#endif
#if defined(MIDL_PASS)
    [unique, string]    LPWSTR    pwszRemoteMACAddr;
#else
    LPWSTR      pwszRemoteMACAddr;
#endif
    DWORD   dwSizeOfSSID;
    BYTE    bSSID[MAX_SSID_LEN+1];
#if defined(MIDL_PASS)
    [unique, string]    LPSTR    pszEapIdentity;
#else
    LPSTR       pszEapIdentity;
#endif
    EAPOL_STATE     dwState;
    EAPUISTATE      dwEapUIState;
    DWORD   dwEAPOLAuthMode;
    DWORD   dwEAPOLAuthenticationType;
    DWORD   dwEapType;
    DWORD   dwFailCount;
    DWORD   dwPhysicalMediumType;
} EAPOL_INTF_STATE, *PEAPOL_INTF_STATE;


#define     EAPOL_VERSION_1             1
#define     EAPOL_VERSION_2             2
#define     EAPOL_VERSION_3             3

#define     EAPOL_CURRENT_VERSION       EAPOL_VERSION_3

//
// Structure : EAPOL_AUTH_DATA
//
typedef struct _EAPOL_AUTH_DATA
{
    DWORD   dwEapType;
    DWORD   dwSize;
    BYTE    bData[1];
} EAPOL_AUTH_DATA, *PEAPOL_AUTH_DATA;

DWORD
WZCGetEapUserInfo (
        IN  WCHAR           *pwszGUID,
        IN  DWORD           dwEapTypeId,
        IN  DWORD           dwSizOfSSID,
        IN  BYTE            *pbSSID,
        IN  OUT PBYTE       pbUserInfo,
        IN  OUT DWORD       *pdwInfoSize
        );

// Structure used to define the UI Response.
// Currently it contains upto 3 blobs.
// If more are required, add to the structure

#define NUM_RESP_BLOBS 3

typedef struct _EAPOLUI_RESP
{
    RAW_DATA    rdData0;
    RAW_DATA    rdData1;
    RAW_DATA    rdData2;
} EAPOLUI_RESP, *PEAPOLUI_RESP;


//
// EAPOL Policy related parameters
//

#define     EAPOL_CERT_TYPE_SMARTCARD   1
#define     EAPOL_CERT_TYPE_MC_CERT     2

typedef struct _EAPOL_POLICY_DATA {
    BYTE  pbWirelessSSID[32];
    DWORD dwWirelessSSIDLen;
    DWORD dwEnable8021x;
    DWORD dw8021xMode;
    DWORD dwEAPType;
    DWORD dwMachineAuthentication;
    DWORD dwMachineAuthenticationType;
    DWORD dwGuestAuthentication;
    DWORD dwIEEE8021xMaxStart;
    DWORD dwIEEE8021xStartPeriod;
    DWORD dwIEEE8021xAuthPeriod;
    DWORD dwIEEE8021xHeldPeriod;
    DWORD dwEAPDataLen;
    LPBYTE pbEAPData;
} EAPOL_POLICY_DATA, *PEAPOL_POLICY_DATA;


typedef struct _EAPOL_POLICY_LIST  {
	DWORD			    dwNumberOfItems;
	EAPOL_POLICY_DATA	EAPOLPolicy[1];
} EAPOL_POLICY_LIST, *PEAPOL_POLICY_LIST;


#if !defined(MIDL_PASS)

//---------------------------------------
// WZCEapolGetCustomAuthData: Get EAP-specific configuration data for interface
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   pwszGuid:
//     [in]  Interface GUID
//   dwEapTypeId:
//     [in]  EAP type Id
//   dwSizeOfSSID:
//     [in]  Size of SSID for which data is to be stored
//   pbSSID:
//     [in]  SSID for which data is to be stored
//   pbConnInfo:
//     [in out]  Connection EAP info
//   pdwInfoSize:
//     [in out]  Size of pbConnInfo
//
// Returned value:
//     Win32 error code 
DWORD
WZCEapolGetCustomAuthData (
    IN  LPWSTR        pSrvAddr,
    IN  PWCHAR        pwszGuid,
    IN  DWORD         dwEapTypeId,
    IN  DWORD         dwSizeOfSSID,
    IN  BYTE          *pbSSID,
    IN OUT PBYTE      pbConnInfo,
    IN OUT PDWORD     pdwInfoSize
    );

//---------------------------------------
// WZCEapolSetCustomAuthData: Set EAP-specific configuration data for interface
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   pwszGuid:
//     [in]  Interface GUID
//   dwEapTypeId:
//     [in]  EAP type Id
//   dwSizeOfSSID:
//     [in]  Size of SSID for which data is to be stored
//   pbSSID:
//     [in]  SSID for which data is to be stored
//   pbConnInfo:
//     [in]  Connection EAP info
//   pdwInfoSize:
//     [in]  Size of pbConnInfo
//
// Returned value:
//     Win32 error code 
DWORD
WZCEapolSetCustomAuthData (
    IN  LPWSTR        pSrvAddr,
    IN  PWCHAR        pwszGuid,
    IN  DWORD         dwEapTypeId,
    IN  DWORD         dwSizeOfSSID,
    IN  BYTE          *pbSSID,
    IN  PBYTE         pbConnInfo,
    IN  DWORD         dwInfoSize
    );

//---------------------------------------
// WZCEapolGetInterfaceParams: Get configuration parameters for interface
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   pwszGuid:
//     [in]  Interface GUID
//   pIntfParams:
//     [in out]  Interface Parameters
//
// Returned value:
//     Win32 error code 
DWORD
WZCEapolGetInterfaceParams (
    IN  LPWSTR          pSrvAddr,
    IN  PWCHAR          pwszGuid,
    IN OUT EAPOL_INTF_PARAMS   *pIntfParams
    );

//---------------------------------------
// WZCEapolSetInterfaceParams: Set configuration parameters for interface
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   pwszGuid:
//     [in]  Interface GUID
//   pIntfParams:
//     [in]  Interface parameters
// Returned value:
//     Win32 error code 
DWORD
WZCEapolSetInterfaceParams (
    IN  LPWSTR        pSrvAddr,
    IN  PWCHAR        pwszGuid,
    IN  EAPOL_INTF_PARAMS   *pIntfParams
    );

//---------------------------------------
// WZCEapolReAuthenticate : Restart 802.1X authentication on an interface 
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   pwszGuid:
//     [in]  Interface GUID
// Returned value:
//     Win32 error code 
DWORD
WZCEapolReAuthenticate (
    IN  LPWSTR        pSrvAddr,
    IN  PWCHAR        pwszGuid
    );

//---------------------------------------
// WZCEapolQueryState: Query the interface 802.1X/EAPOL state
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   pwszGuid:
//     [in]  Interface GUID
//   pIntfState:
//     [in out]  EAPOL Interface State
//
// Returned value:
//     Win32 error code 
DWORD
WZCEapolQueryState (
    IN  LPWSTR              pSrvAddr,
    IN  PWCHAR              pwszGuid,
    IN OUT EAPOL_INTF_STATE *pIntfState
    );

#endif // MIDL_PASS

//
// Free EAPOL interface state information on the client side obtained via
// RPC query
//

DWORD
WZCEapolFreeState (
    IN  EAPOL_INTF_STATE    *pIntfState
    );


//
// Structure: EAPOL_EAP_UI_CONTEXT
//

typedef struct _EAPOL_EAP_UI_CONTEXT
{
    DWORD       dwEAPOLUIMsgType;
    WCHAR       wszGUID[39];
    DWORD       dwSessionId;
    DWORD       dwContextId;
    DWORD       dwEapId;
    DWORD       dwEapTypeId;
    DWORD       dwEapFlags;
    WCHAR       wszSSID[MAX_SSID_LEN+1];
    DWORD       dwSizeOfSSID;
    BYTE        bSSID[MAX_SSID_LEN];
    DWORD       dwEAPOLState;
    DWORD       dwRetCode;
    DWORD       dwSizeOfEapUIData;
    BYTE        bEapUIData[1];
} EAPOL_EAP_UI_CONTEXT, *PEAPOL_EAP_UI_CONTEXT;

//
// Defines for messaging between Service and Dialog DLL
//

#define     EAPOLUI_GET_USERIDENTITY            0x00000001
#define     EAPOLUI_GET_USERNAMEPASSWORD        0x00000002
#define     EAPOLUI_INVOKEINTERACTIVEUI         0x00000004
#define     EAPOLUI_EAP_NOTIFICATION            0x00000008
#define     EAPOLUI_REAUTHENTICATE              0x00000010
#define     EAPOLUI_CREATEBALLOON               0x00000020
#define     EAPOLUI_CLEANUP                     0x00000040
#define     EAPOLUI_DUMMY                       0x00000080

#define     NUM_EAPOL_DLG_MSGS      8


//---------------------------------------
// WZCEapolUIResponse: Send Dlg response to Service
// 
// Parameters:
//   pSrvAddr:
//     [in]  WZC Server to contact
//   EapolUIContext:
//     [in]  EAPOLUI Context data
//   EapolUI:
//     [in]  EAPOLUI response data
//
// Returned value:
//     Win32 error code 
DWORD
WZCEapolUIResponse (
    IN  LPWSTR                  pSrvAddr,
    IN  EAPOL_EAP_UI_CONTEXT    EapolUIContext,
    IN  EAPOLUI_RESP            EapolUIResp
    );



#ifdef UNDER_CE

//
//  In CE, CreateFile(ZEROCONFIG_DEVICE_NAME) then IOCTL to it..
//

#define ZEROCONFIG_DEVICE_NAME      TEXT("ZCF1:")

#define FSCTL_ZC_BASE               FILE_DEVICE_NETWORK

#define _ZC_CTL_CODE(_Function, _Method, _Access)  \
            CTL_CODE(FSCTL_ZC_BASE, _Function, _Method, _Access)


#define IOCTL_ZC_ENUM_INTERFACES                    \
            _ZC_CTL_CODE(0x300, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_QUERY_INTERFACE                    \
            _ZC_CTL_CODE(0x301, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_SET_INTERFACE                      \
            _ZC_CTL_CODE(0x302, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_REFRESH_INTERFACE                  \
            _ZC_CTL_CODE(0x303, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_ENUM_EAP_EXTENSIONS                \
            _ZC_CTL_CODE(0x304, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_QUERY_CONTEXT                      \
            _ZC_CTL_CODE(0x30A, METHOD_BUFFERED, FILE_ANY_ACCESS)
            
#define IOCTL_ZC_SET_CONTEXT                        \
            _ZC_CTL_CODE(0x30B, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_READ_COMPLETION                    \
            _ZC_CTL_CODE(0x30C, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_PORT_INDICATION                    \
            _ZC_CTL_CODE(0x30D, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_QUERY_INTERFACE_EX                 \
            _ZC_CTL_CODE(0x30E, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_SET_INTERFACE_EX                 \
            _ZC_CTL_CODE(0x30F, METHOD_BUFFERED, FILE_ANY_ACCESS)

#ifdef UNDER_CE

#define IOCTL_ZC_SET_LATENCY_SENSITIVE_MODE                      \
            _ZC_CTL_CODE(0x310, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ZC_QUERY_LATENCY_SENSITIVE_MODE                      \
            _ZC_CTL_CODE(0x311, METHOD_BUFFERED, FILE_ANY_ACCESS)

struct LatencySensitiveModeData
{
    DWORD   Size;
    BOOL    bEnable;
    ULONG   PacketLatencyMs;
    WCHAR   wszInterfaceGuid[MAX_PATH];
};
    
#endif

typedef struct
{
    DWORD   dwNumIntfs;
    DWORD   dwBufferSize;    
    PBYTE   pbBuffer;

}   CE_ENUM_INTERFACE, *PCE_ENUM_INTERFACE;


typedef struct
{
    DWORD               dwInFlags;
    DWORD               dwOutFlags;
    LPWSTR              wszGuid;
    DWORD               dwBufferSize;    
    PBYTE               pbBuffer;    

}   CE_QUERY_INTERFACE, *PCE_QUERY_INTERFACE;


typedef struct
{
    DWORD               dwInFlags;    
    DWORD               dwOutFlags;
    PINTF_ENTRY         pIntf;

}   CE_SET_INTERFACE, *PCE_SET_INTERFACE;


typedef struct
{
    DWORD               dwInFlags;
    DWORD               dwOutFlags;
    PINTF_ENTRY         pIntf;    

}   CE_REFRESH_INTERFACE, *PCE_REFRESH_INTERFACE;


typedef struct
{
    DWORD   dwNumExtensions;
    DWORD   dwBufferSize;    
    PBYTE   pbBuffer;

}   CE_ENUM_EAP_EXTENSIONS, *PCE_ENUM_EAP_EXTENSIONS;

typedef struct
{
    PWCHAR              pwszGuid;
    DWORD               dwEapTypeId;
    RAW_DATA            rdSSID;
    RAW_DATA            rdConnInfo;

}   CE_EAPOL_CUSTOM_AUTH_DATA, *PCE_EAPOL_CUSTOM_AUTH_DATA;


typedef struct
{
    PWCHAR              pwszGuid;
    EAPOL_INTF_PARAMS   *pIntfParams;

}   CE_EAPOL_INTERFACE_PARAMS, *PCE_EAPOL_INTERFACE_PARAMS;


typedef struct
{
    EAPOL_EAP_UI_CONTEXT    *pEapolUIContext;
    EAPOLUI_RESP            *pEapolUIResp;

}   CE_EAPOL_UI_RESPONSE, *PCE_EAPOL_UI_RESPONSE;

#endif  //  UNDER_CE


DWORD
WZCElReadCompletionRoutineWifi(
    LPWSTR pServerName,
    PDOT11_RECEIVE_UPCALL pDot11RecvUpcall
    );

#define WZC_PORTFLAGS_UP                 0x00000001
#define WZC_PORTFLAGS_DOWN               0x00000002
#define WZC_PORTFLAGS_SUCCESSFUL_ROAM    0x00000004
#define WZC_PORTFLAGS_PEERMAC_AVAILABLE  0x00000008

typedef struct _WZC_PORT_INFO {
    GUID gAdatperId;
    RAW_DATA PeerMacAddress;
    DWORD dwFlags;
} WZC_PORT_INFO, *PWZC_PORT_INFO;


DWORD
WzcPortIndication(
    IN LPWSTR pServerName,
    IN PWZC_PORT_INFO pWzcPortInfo
    );



# ifdef     __cplusplus
}
# endif


#endif	//	_WZCAPI_H_

