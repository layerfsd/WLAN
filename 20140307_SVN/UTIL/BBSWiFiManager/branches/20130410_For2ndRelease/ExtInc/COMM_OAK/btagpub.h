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

#ifndef __BTAGPUB_H__
#define __BTAGPUB_H__

#include <windows.h>

#include <winsock2.h>
#include <ws2bth.h>


#define IOCTL_AG_OPEN_AUDIO         0x01
#define IOCTL_AG_CLOSE_AUDIO        0x02
#define IOCTL_AG_CLOSE_CONTROL      0x03
#define IOCTL_AG_SET_SPEAKER_VOL    0x04
#define IOCTL_AG_SET_MIC_VOL        0x05
#define IOCTL_AG_GET_SPEAKER_VOL    0x06
#define IOCTL_AG_GET_MIC_VOL        0x07
#define IOCTL_AG_GET_POWER_MODE     0x08
#define IOCTL_AG_SET_POWER_MODE     0x09
#define IOCTL_AG_OPEN_CONTROL       0x0A
#define IOCTL_AG_SET_USE_HF_AUDIO   0x0B
#define IOCTL_AG_SET_INBAND_RING    0x0C

#define MAX_SEND_BUF                256
#define MAX_PHONE_NUMBER            128
#define MAX_DISPLAY_NAME            128

#define RK_AUDIO_GATEWAY            _T("SOFTWARE\\Microsoft\\Bluetooth\\AudioGateway")
#define RK_AUDIO_GATEWAY_DEVICES    (RK_AUDIO_GATEWAY TEXT("\\Devices"))
#define RK_AG_SERVICE               _T("Services\\BTAGSVC")

#define NETWORK_FLAGS_DROP_ACTIVE   0x01
#define NETWORK_FLAGS_DROP_HOLD     0x02
#define NETWORK_FLAGS_DROP_OFFERING 0x04
#define NETWORK_FLAGS_DROP_OUTGOING 0x08
#define NETWORK_FLAGS_DROP_ALL      0x0f

#define NETWORK_FLAGS_STATE_ACTIVE          0x01
#define NETWORK_FLAGS_STATE_HOLD            0x02
#define NETWORK_FLAGS_STATE_OFFERING        0x04
#define NETWORK_FLAGS_STATE_OUTGOING        0x08
#define NETWORK_FLAGS_STATE_WAITING         0x10

// Values for the SIGNAL STRENGTH indicator for BT HandsFree 1.5
#define AG_SIGNAL_STRENGTH_NOT_CONNECTED  0  
#define AG_SIGNAL_STRENGTH_VERY_LOW       1
#define AG_SIGNAL_STRENGTH_LOW            2
#define AG_SIGNAL_STRENGTH_MED            3
#define AG_SIGNAL_STRENGTH_HI             4
#define AG_SIGNAL_STRENGTH_VERY_HI        5

// Values for the BATTERY STRENGTH indicator for BT HandsFree 1.5
#define AG_BATTERY_CHARGE_UNKNOWN         0
#define AG_BATTERY_CHARGE_VERY_LOW        1
#define AG_BATTERY_CHARGE_LOW             2
#define AG_BATTERY_CHARGE_MED             3
#define AG_BATTERY_CHARGE_HI              4
#define AG_BATTERY_CHARGE_VERY_HI         5

// Values for the OPERATOR SELECTION Mode value for BT HandsFree 1.5
#define AG_OPERATOR_MODE_AUTO             0
#define AG_OPERATOR_MODE_MANUAL           1
#define AG_OPERATOR_MODE_DEREGISTERED     2
#define AG_OPERATOR_MODE_MANUAL_AUTO      4

// Values for the CALLHELD indicator for BT HandsFree 1.5
#define AG_NETWORK_CALL_HELD_NONE           0
#define AG_NETWORK_CALL_HELD_AND_ACTIVE     1
#define AG_NETWORK_CALL_HELD_AND_NO_ACTIVE  2


//  Events [AG --> Phone Extension]
#define AG_PHONE_EVENT_VOICE_RECOG		        0x01
#define AG_PHONE_EVENT_SPEAKER_VOLUME	        0x02
#define AG_PHONE_EVENT_MIC_VOLUME		        0x03
#define AG_PHONE_EVENT_BT_CTRL			        0x04
#define AG_PHONE_EVENT_BT_AUDIO			        0x05

#define AG_PHONE_EVENT_SET_VOICE_RECOG          AG_PHONE_EVENT_VOICE_RECOG
#define AG_PHONE_EVENT_SET_SPEAKER_VOLUME       AG_PHONE_EVENT_SPEAKER_VOLUME
#define AG_PHONE_EVENT_SET_MIC_VOLUME           AG_PHONE_EVENT_MIC_VOLUME
#define AG_PHONE_EVENT_SET_BT_CTRL              AG_PHONE_EVENT_BT_CTRL
#define AG_PHONE_EVENT_SET_BT_AUDIO             AG_PHONE_EVENT_BT_AUDIO
#define AG_PHONE_EVENT_QUERY_SIGNAL_STRENGTH    0x06
#define AG_PHONE_EVENT_QUERY_BATT_LEVEL         0x07

//  Notification [Phone Extension --> AG]
#define AG_PHONE_NOTIF_CELLULAR_SIGNAL_STRENGTH 0x01
#define AG_PHONE_NOTIF_ROAM                     0x02
#define AG_PHONE_NOTIF_BATT_STRENGTH            0x03


typedef void (*PFN_PhoneExtServiceCallback) (BOOL fHaveService);
typedef DWORD (*PFN_SendATCommand) (LPSTR szCommand, DWORD cbCommand);
typedef DWORD (*PFN_BthAGOnVoiceTag) (BOOL fOn);

#define MAX_PAIRINGS    4

typedef struct _AG_DEVICE {
    BT_ADDR bta;
    GUID service;
} AG_DEVICE, *PAG_DEVICE;


typedef struct _SUBSCRIBER_NUMBER_LIST {
    DWORD   dwBufferSize; //  [in/out] in: pcBuffer size : out: 0 if error.
    CHAR    *pcBuffer;    //  [out]    multisz string.
} SUBSCRIBER_NUMBER_LIST, *PSUBSCRIBER_NUMBER_LIST;


typedef struct _ONE_CALL {
    CHAR        Number[128];
    DWORD       CallID;         //  Identification per 3GPP TS 22.030
    DWORD       dwStatus;       //  NETWORK_FLAGS_STATE_xxx
    BOOL        bMO;            //  True if mobile originated
    BOOL        bMultiparty;    //  True if multiparty
} ONE_CALL, *PONE_CALL;


typedef struct _CALL_LIST {
    DWORD       dwMaxCalls;     //  IN/OUT 
    ONE_CALL    *pOneCallArray;
} CALL_LIST, *PCALL_LIST;


typedef struct _ONE_OPERATOR {
    DWORD    dwMode;     // [0] auto, [1] man, [3] deregister from n/w, [4] man/auto
    CHAR     Name[17];   // max 16 characters long (GSM MoU SE.13)
} ONE_OPERATOR, *PONE_OPERATOR;


// AG functions.
void BthAGOnPhoneEvent(DWORD dwEvent, LPVOID lpvParam, DWORD cbParam);

// Gets the list of devices from the registry
DWORD GetBTAddrList(PAG_DEVICE pDevices, USHORT usNumDevices);

// Functions implemented by Network component   
DWORD BthAGNetworkInit(HINSTANCE hInstance);
void BthAGNetworkDeinit(void);
DWORD BthAGNetworkDialNumber(LPWSTR pwszNumber, BOOL fIsContact);
DWORD BthAGNetworkDropCall(DWORD dwFlags);
DWORD BthAGNetworkAnswerCall(void);
DWORD BthAGNetworkTransmitDTMF(LPWSTR pwszDTMF);
DWORD BthAGNetworkHoldCall(void);
DWORD BthAGNetworkUnholdCall(void);
DWORD BthAGNetworkSwapCall(void);
DWORD BthAGNetworkDropSwapCall(void);
DWORD BthAGNetworkGetCallState(PDWORD pdwFlags);
BOOL  BthAGNetworkExtRequest(DWORD dwEvent, DWORD dwParam, VOID* pvParam2);

// Functions implemented by PhoneExt component
DWORD BthAGPhoneExtInit(void);
void BthAGPhoneExtDeinit(void);
BOOL BthAGOverrideCallIn(BOOL fHandsfree);
BOOL BthAGOverrideCallOut(BOOL fHandsfree);
void BthAGPhoneExtEvent(DWORD dwEvent, DWORD dwParam, VOID* pvParam2);
BOOL BthAGPhoneExtRequest (DWORD dwEvent, DWORD dwParam, VOID* pvParam2);
BOOL BthAGGetNameByPhoneNumber(LPCWSTR pwszNumber, LPWSTR pwszName);
BOOL BthAGGetSpeedDial(unsigned short usIndex, LPWSTR pwszNumber);
BOOL BthAGGetLastDialed(LPWSTR pwszNumber, BOOL* pfIsContact);
DWORD BthAGSetServiceCallback(PFN_PhoneExtServiceCallback pfn);


// Network & PhoneExt libs may use these debug zones
#define ZONE_NETWORK        DEBUGZONE(5)
#define ZONE_PHONEUI        DEBUGZONE(6)
#define ZONE_BOND           DEBUGZONE(7)
#define ZONE_WARN           DEBUGZONE(14)
#define ZONE_ERROR          DEBUGZONE(15)



#endif // __BTAGPUB_H__

