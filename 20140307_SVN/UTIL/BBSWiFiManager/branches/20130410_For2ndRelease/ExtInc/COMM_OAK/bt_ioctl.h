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
//      Bluetooth IO Control interface
// 
// 
// Module Name:
// 
//      bt_ioctl.h
// 
// Abstract:
// 
//      This file defines IOCTL interface that implements stack APIs
// 
// 
//------------------------------------------------------------------------------
#if ! defined (__bt_ioctl_H__)
#define __bt_ioctl_H__      1

#define BT_IOCTL_BthWriteScanEnableMask                 10
#define BT_IOCTL_BthPerformInquiry                      11
#define BT_IOCTL_BthRemoteNameQuery                     12
#define BT_IOCTL_BthWritePageTimeout                    13
#define BT_IOCTL_BthReadLocalAddr                       14
#define BT_IOCTL_BthStackControl                        15
#define BT_IOCTL_BthSetPIN                              16
#define BT_IOCTL_BthRevokePIN                           17
#define BT_IOCTL_BthSetLinkKey                          18
#define BT_IOCTL_BthGetLinkKey                          19
#define BT_IOCTL_BthRevokeLinkKey                       20
#define BT_IOCTL_BthAuthenticate                        21
#define BT_IOCTL_BthSetEncryption                       22
#define BT_IOCTL_BthReadScanEnableMask                  23
#define BT_IOCTL_BthWriteCOD                            24
#define BT_IOCTL_BthReadCOD                             25
#define BT_IOCTL_BthReadPageTimeout                     26
#define BT_IOCTL_BthReadLocalVersion                    27
#define BT_IOCTL_BthReadRemoteVersion                   28
#define BT_IOCTL_BthSetSecurityUI                       29
#define BT_IOCTL_BthGetPINRequest                       30
#define BT_IOCTL_BthRefusePINRequest                    31
#define BT_IOCTL_BthWriteAuthenticationEnable           32
#define BT_IOCTL_BthReadAuthenticationEnable            33
#define BT_IOCTL_BthWriteLinkPolicySettings             34
#define BT_IOCTL_BthReadLinkPolicySettings              35
#define BT_IOCTL_BthEnterHoldMode                       36
#define BT_IOCTL_BthEnterSniffMode                      37
#define BT_IOCTL_BthExitSniffMode                       38
#define BT_IOCTL_BthEnterParkMode                       39
#define BT_IOCTL_BthExitParkMode                        40
#define BT_IOCTL_BthGetBasebandHandles                  41
#define BT_IOCTL_BthGetCurrentMode                      42
#define BT_IOCTL_BthCancelInquiry                       43
#define BT_IOCTL_BthGetHardwareStatus                   44
#define BT_IOCTL_BthTerminateIdleConnections            45
#define BT_IOCTL_BthGetAddress                          46
#define BT_IOCTL_BthSetInquiryFilter                    47
#define BT_IOCTL_BthClearInquiryFilter                  48
#define BT_IOCTL_BthCreateACLConnection                 49
#define BT_IOCTL_BthCreateSCOConnection                 50
#define BT_IOCTL_BthCloseConnection                     51
#define BT_IOCTL_BthGetBasebandConnections              52
#define BT_IOCTL_BthAcceptSCOConnections                53
#define BT_IOCTL_BthGetRemoteCOD                        54
#define BT_IOCTL_BthAnswerPairRequest                   55
#define BT_IOCTL_BthPairRequest                         56
#define BT_IOCTL_BthSwitchRole                          57
#define BT_IOCTL_BthGetRole                             58
#define BT_IOCTL_BthReadRSSI                            59
#define BT_IOCTL_BthGetQueuedHCIPacketCount             60
#define BT_IOCTL_BthActivatePAN                         61
#define BT_IOCTL_BthSetCODInquiryFilter                 62
#define BT_IOCTL_BthWritePageScanActivity               63
#define BT_IOCTL_BthWriteInquiryScanActivity            64
#define BT_IOCTL_BthReadPageScanActivity                65
#define BT_IOCTL_BthReadInquiryScanActivity             66
#define BT_IOCTL_BthWritePageScanType                   67
#define BT_IOCTL_BthWriteInquiryScanType                68
#define BT_IOCTL_BthReadPageScanType                    69
#define BT_IOCTL_BthReadInquiryScanType                 70
#define BT_IOCTL_BthCreateSynchronousConnection         71
#define BT_IOCTL_BthAcceptSynchronousConnections        72


#define BT_IOCTL_BthNsSetService                        80
#define BT_IOCTL_BthNsLookupServiceBegin                81
#define BT_IOCTL_BthNsLookupServiceNext                 82
#define BT_IOCTL_BthNsLookupServiceEnd                  83

#define BT_IOCTL_RequestBluetoothNotifications          100
#define BT_IOCTL_StopBluetoothNotifications             101

typedef union {
    struct {
        // in-parameters
        BT_ADDR             bt;             // Remote only
        // out-parameters
        unsigned char       hci_version;    // Local only
        unsigned short      hci_revision;   // Local only
        unsigned char       lmp_version;
        unsigned short      manufacturer_name;
        unsigned short      lmp_subversion;
        unsigned char       lmp_features[8];
    } BthReadVersion_p;

    struct {
        // in-parameters for set, out-parameters for read
        unsigned char       mask;
    } BthScanEnableMask_p;

    struct {
        // in-parameters for set, out-parameters for read
        unsigned int        cod;
    } BthCOD_p;

    struct {
        // in-parameters for set, out-parameters for read
        unsigned char       ae;
    } BthAuthenticationEnable_p;

    struct {
        // in-parameters for set, out-parameters for read
        BT_ADDR             bt;
        unsigned char       mode;
    } BthCurrentMode_p;

    struct {
        // in-parameters
        int cMaxHandles;
        // out-parameters
        unsigned short *pHandles;
        int cHandlesReturned;
    } BthBasebandHandles_p;

    struct {
        // in-parameters
        int cMaxConnections;
        // out-parameters
        PBASEBAND_CONNECTION_DATA pConnections;
        int cConnectionsReturned;
    } BthBasebandConnections_p;

    struct {
        // in-parameters for set, out-parameters for read
        BT_ADDR             bt;
        unsigned short      lps;
    } BthLinkPolicySettings_p;

    struct {
        // in-parameters
        BT_ADDR         ba;
        unsigned short  hold_mode_max;
        unsigned short  hold_mode_min;
        // out-parameters
        unsigned short  interval;
    } BthHold_p;

    struct {
        // in-parameters
        BT_ADDR         ba;
        unsigned short  sniff_mode_max;
        unsigned short  sniff_mode_min;
        unsigned short  sniff_attempt;
        unsigned short  sniff_timeout;
        // out-parameters
        unsigned short  interval;
    } BthSniff_p;

    struct {
        // in-parameters
        BT_ADDR         ba;
        unsigned short  beacon_max;
        unsigned short  beacon_min;
        // out-parameters
        unsigned short  interval;
    } BthPark_p;

    struct {
        // in-parameters
        unsigned int    LAP;
        unsigned char   length;
        unsigned char   num_responses;
        unsigned int    cBuffer;
        // out-parameters
        unsigned int    *pcDiscoveredDevices;
        BthInquiryResult*inquiry_list;
    } BthPerformInquiry_p;

    struct {
        // in-parameters
        BT_ADDR         ba;
        unsigned int    cBuffer;
        // out-parameters
        unsigned int    *pcRequired;
        WCHAR           *pszString;
    } BthRemoteNameQuery_p;

        struct {
        // in-parameters
        unsigned short      timeout;
    } BthPageTimeout_p;

    struct {
        // in-parameters
        BT_ADDR             ba;
        int                 cDataLength;    // 0 for link key
        // in-out-parameters
        unsigned char       data[16];
    } BthSecurity_p;

    struct {
        // in-parameters
        BT_ADDR             ba;
    } BthAuthenticate_p;

    struct {
        // in-parameters
        BT_ADDR             ba;
        int                 fOn;
    } BthSetEncryption_p;

    struct {
        // out-parameters
        BT_ADDR             ba;
    } BthReadLocalAddr_p;

    struct {
        // out-parameters
        int                 iHwStatus;
    } BthGetHardwareStatus_p;

    struct {
        int                 iProp;
        int                 iVal;
    } BthStackControl_p;

    struct {
        // in-parameters
        HANDLE  hEvent;
        DWORD   dwStoreTimeout;
        DWORD   dwProcessTimeout;
    } BthSetSecurityUI_p;

    struct {
        // in-parameters
        BT_ADDR             ba;
    } BthRefusePINRequest_p;

    struct {
        // out-parameters
        BT_ADDR             ba;
    } BthGetPINRequest_p;

    struct {
        LPWSAQUERYSET pSet;
        WSAESETSERVICEOP op;
        DWORD dwFlags;
    } BthNsSetService_p;

    struct {
        LPWSAQUERYSET pQuerySet;
        DWORD dwFlags;
        HANDLE hLookup;
    } BthNsLookupServiceBegin_p;

    struct {
        HANDLE hLookup;
        DWORD dwFlags;
        DWORD dwBufferLength;
        LPWSAQUERYSET pResults;
    } BthNsLookupServiceNext_p;

    struct {
        HANDLE hLookup;
    } BthNsLookupServiceEnd_p;

    struct {
        BT_ADDR ba;
        unsigned short handle;
    } BthGetAddress_p;

    struct {
        BT_ADDR ba;
    } BthSetInquiry_p;

    struct {
        unsigned int cod;
        unsigned int codMask;
    } BthSetCODInquiry_p;

    struct {
        BT_ADDR ba;
        unsigned short handle;
    } BthCreateConnection_p;

    struct {
        unsigned short handle;
    } BthCloseConnection_p;

    struct {
        BOOL fAccept;
    } BthAcceptSCOConnections_p;

    struct {
        BT_ADDR ba;
        unsigned int cod;
    } BthRemoteCOD_p;

    struct {
        DWORD dwClass;
        HANDLE hMsgQ;
        HANDLE hOut;
    } RequestBluetoothNotifications_p;

    struct {
        HANDLE h;
    } StopBluetoothNotifications_p;

    struct {
        // in-parameters
        BT_ADDR ba;
        unsigned short usRole;
    } BthSwitchRole_p;

    struct {
        // in-parameters
        BT_ADDR ba; 
        // out-parameters
        unsigned short usRole;
    } BthGetRole_p;

    struct {
        // in-parameters
        BT_ADDR ba; 
        BYTE iRSSI; 
    } BthReadRSSI_p;

    struct {
        // in-parameters
        BT_ADDR ba;
        // out-parameters
        int cPackets;
    } BthGetQueuedHCIPacketCount_p;

    struct {
        // in-parameters
        BOOL fActivate;
    } BthActivatePAN_p;
    
    struct {
        // in/out-parameters
        unsigned short scanInterval;
        unsigned short scanWindow;
    } BthScanActivity_p;
    
    struct {
        // in/out parameters
        unsigned char scanType;
    } BthScanType_p;
    
    struct {
        // in-parameters
        BT_ADDR ba;
        unsigned int txBandwidth;
        unsigned int rxBandwidth;
        unsigned short maxLatency;
        unsigned short voiceSetting;
        unsigned char retransmit;        
        // out-paramteres
        unsigned short handle;
    } BthCreateSynchronousConnection_p;
    
    struct {
        BOOL fAccept;
    } BthAcceptSynchronousConnections_p;
    
} BTAPICALL;

#endif      /* __bt_ioctl_H__ */

