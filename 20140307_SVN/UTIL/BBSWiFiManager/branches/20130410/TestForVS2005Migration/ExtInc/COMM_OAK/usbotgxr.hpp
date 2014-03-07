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
// Module Name:  
//     USBOTGXR.h
// 
// Abstract: Provides Frame Work For OTG Transcever 
// 
// Notes: This file is create for ISP1301. May need modify for others.
//
#ifndef __USBOTGXR_HPP_
#define __USBOTGXR_HPP_

#include <Cmthread.h>
#include <Csync.h>
#include <cregedit.h>

#include <nkintr.h>

typedef union {
    struct {
        DWORD DP_PullUp:1;
        DWORD DM_PullUp:1;
        DWORD DP_PullDown:1;
        DWORD DM_PullDown:1;
        DWORD ID_PullDown:1;
        DWORD vbusDrv:1;
        DWORD vbusDischange:1;
        DWORD vbusCharge:1;
    } bit;
    DWORD ul;
} USBOTG_TRANSCEIVER_CTL, *PUSBOTG_TRANSCEIVER_CTL;

typedef union {
    struct {
        DWORD aBusValid:1;
        DWORD aSessValid:1;
        DWORD DP_HI:1;
        DWORD DM_HI:1;
        DWORD bDisACON:1;
        DWORD ID:1; // 0 -- GND
        DWORD ID_FLOAT:1; // 1 -- line is floating
        
        DWORD bSessEnd:1;
        DWORD bSessValid:1;
    } bit;
    DWORD ul;
} USBOTG_TRANSCEIVER_STATUS, *PUSBOTG_TRANSCEIVER_STATUS;


typedef union {
    struct {        
        DWORD aBusValid:1;
        DWORD aSessValid:1;
        DWORD DP_HI:1;
        DWORD DM_HI:1;
        DWORD bDisACON:1;
        DWORD ID:1;
    } bit;
    DWORD ul;
} USBOTG_TRANSCEIVER_STATUS_CHANGE, *PUSBOTG_TRANSCEIVER_STATUS_CHANGE;

typedef enum {
    BDIS_ACON = 1
} TRANCEVER_FUNCTION;

class USBOTGTransceiver: public CLockObject  {
public:
    USBOTGTransceiver() {;};
    virtual ~USBOTGTransceiver() {; };;
    virtual BOOL Init() { return TRUE; };
    virtual BOOL SetTransceiver (USBOTG_TRANSCEIVER_CTL) = 0;
    virtual BOOL GetTransceiver(PUSBOTG_TRANSCEIVER_STATUS)= 0;
    virtual BOOL EnableTransceiver(TRANCEVER_FUNCTION , BOOL fEnable) = 0 ;
    virtual BOOL EnableDataLineInterrupt(BOOL fEnable ) = 0 ;
    virtual BOOL StateChangeNotification ( USBOTG_TRANSCEIVER_STATUS_CHANGE, USBOTG_TRANSCEIVER_STATUS ) = 0;
};

#define TRANSCEIVER_4_PIN       1
#define TRANSCEIVER_3_PIN       2
#define TRANSCEIVER_6_PIN       3

class USBOTGTransceiverISP1301: public USBOTGTransceiver ,public  CMiniThread , CRegistryEdit{
public :
    USBOTGTransceiverISP1301(PTSTR lpActivePath, DWORD dwSysIntr = SYSINTR_UNDEFINED ) ;
    ~USBOTGTransceiverISP1301() ;
    virtual BOOL Init() ;
    virtual BOOL SetTransceiver (USBOTG_TRANSCEIVER_CTL);
    virtual BOOL GetTransceiver(PUSBOTG_TRANSCEIVER_STATUS );
    virtual BOOL EnableTransceiver(TRANCEVER_FUNCTION , BOOL fEnable);
    virtual BOOL EnableDataLineInterrupt(BOOL fEnable ) ;
private:
    virtual DWORD       ThreadRun(); // User have to implement this function.
// Private pure virtual function, Need to be finalized.
    virtual BOOL ReadISP1301W(UCHAR reg, USHORT *pData) = 0;
    virtual BOOL ReadISP1301(UCHAR reg, BYTE *pData) = 0;
    virtual BOOL WriteISP1301(UCHAR reg, BYTE Data) = 0;

//  Private function
    BOOL    GetISP1301Change(PUSBOTG_TRANSCEIVER_STATUS_CHANGE);
    DWORD   m_dwSysIntr;
    HANDLE  m_hISTEvent;
    BYTE    m_CtrSet, m_CtrClr;
    BYTE    m_ModeCtr1Set;
    BYTE    m_IntrLatch;
    BOOL    m_fDataLnIntrEnable;
};

#endif

