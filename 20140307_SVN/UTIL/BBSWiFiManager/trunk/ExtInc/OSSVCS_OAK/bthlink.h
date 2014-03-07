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
/**
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.


Abstract:
        Windows CE Bluetooth stack layer sample

**/
#if ! defined (__bthlink_HXX__)
#define __bthlink_HXX__         1

#include <ossvcs.h>
#include <ehm.h>

#include <pkfuncs.h>

#pragma warning(push)
#pragma warning(default : 4100)
// Some functions in these headers generate level 4 warnings that we want to
// get rid of, but we don't own the header so we will just ignore them.
#include <svsutil.hxx>

#include <bt_debug.h>
#pragma warning(pop)

#include <bt_tdbg.h>
#include <bt_api.h>
#include <bt_buffer.h>
#include <bt_ddi.h>


#define BT_SECURITY_NOTIFICATION   TEXT("BT_EVENT_SECURITY")


#define BTHLINK_IOCTL_CONNECT                   1
#define BTHLINK_IOCTL_DISCONNECT                2
#define BTHLINK_GET_HANDLES                     3
#define BTHLINK_GET_ADDRESS                     4
#define BTHLINK_SET_FILTER_CLEAR                5
#define BTHLINK_SET_FILTER_INQ_BA               6
#define BTHLINK_SET_MODE                        7
#define BTHLINK_GET_MODE                        8
#define BTHLINK_SET_LOCALNAME                   9
#define BTHLINK_GET_STACK_EVENT_HANDLES         10
#define BTHLINK_GET_CACHED_HARDWARE_STATUS      11
#define BTHLINK_DEVICE_CHANGED                  12

#define NUM_STACK_EVENT_HANDLES                 4
#define BTHLINK_MAX_HANDLES                     256

#define CONNECTABILITY_POWER_ON                 1 << 0
#define CONNECTABILITY_DISCOVERABLE             1 << 1
#define CONNECTABILITY_CONNECTABLE              1 << 2

typedef union {
        struct  {
                BD_ADDR         b;
                unsigned short  h;
        } CONNECT;

        struct {
                unsigned short  h;
        } DISCONNECT;

        struct {
                int             cReturned;
                unsigned short  aHandles[BTHLINK_MAX_HANDLES];
        } GETHANDLES;

        struct {
                HANDLE  hStackEventHandles[NUM_STACK_EVENT_HANDLES];
        } GETEVENTHANDLES;

        struct {
                unsigned short          h;
                BD_ADDR                 b;
        } GETADDR;

        struct {
                BD_ADDR                 b;
        } BA;

        struct {
                DWORD                   dw;
        } SETMODE;

        struct {
                DWORD                   dw;
        } GETMODE;

        struct {
                int                     i;
        } GETCACHEDHARDWARESTATUS;

        struct {
                WCHAR                   pwszLocalName[MAX_PATH+1];
        } SETLOCALNAME;
} BTHLINK_PARMS;

struct Link {
        Link                    *pNext;
        BD_ADDR                 b;
        unsigned short  h;

        HANDLE                  hProcOwner;
};

struct SCall {
        SCall                   *pNext;

        HANDLE                  hEvent;
        HANDLE                  hProcOwner;

        int                     iResult;

        unsigned int    fWhat                   : 8;

        SCall (void) {
                memset (this, 0, sizeof(*this));
        }
};

static int bthlink_CloseDriverInstance (void);

static int bthlink_ConnectionCompleteEvent      (void *pUserContext, void *pCallContext, unsigned char status, unsigned short connection_handle, BD_ADDR *pba, unsigned char link_type, unsigned char encryption_mode);
static int bthlink_DisconnectionCompleteEvent   (void *pUserContext, void *pCallContext, unsigned char status, unsigned short connection_handle, unsigned char reason);
static int bthlink_hStackEvent                  (void *pUserContext, int iEvent, void *pEventContext);

static int bthlink_SetEventFilter_Out   (void *pCallContext, unsigned char status);
static int bthlink_CreateConnection_Out (void *pCallContext, unsigned char status);
static int bthlink_Disconnect_Out       (void *pCallContext, unsigned char status);
static int bthlink_hCallAborted         (void *pCallContext, int iError);
static int bthlink_LocalNameDone        (void *pCallContext, unsigned char status);
static DWORD WINAPI QueryHardwareStatus (LPVOID lpVoid);


#define DEBUG_LAYER_TRACE                       0x00000040

class BTHLINK : public SVSSynch, public SVSRefObj {
public:
	SCall                   *pCalls;
	Link                    *pLinks;
	
	unsigned int    fIsRunning : 1;
	unsigned int    fConnected : 1;
	int             iCachedHardwareStatus;
	
	
	HANDLE                  hHCI;
	HCI_INTERFACE   hci_if;
	
	BTHLINK (void);
	
	~BTHLINK (void);

    void HCIEstablishDeviceContext();
};

extern BTHLINK *gpLayerState;

#endif          // __bthlink_HXX__

