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
//      Bluetooth Layered Device Driver Interface
// 
// 
// Module Name:
// 
//      bt_ddi.h
// 
// Abstract:
// 
//      This file defines interfaces between layers of Bluetooth device driver
// 
// 
//------------------------------------------------------------------------------
#if ! defined (__bt_actp_H__)
#define __bt_actp_H__		1

#include <svsutil.hxx>

#include <bt_buffer.h>
#include <bt_ddi.h>

//
//	------------------------------------ Stack Common ------------------------------------
//

int avctp_InitializeOnce (void);
int avctp_CreateDriverInstance (void);
int avctp_CloseDriverInstance (void);

//
// ------------------------------------ AVCTP -----------------------------------
//


enum AVCT_RSP {
    avct_connect_success_e = 0,
	avct_connect_failed_e,
	avct_no_resources
};

enum AVCT_Type {
	avct_cmd_msg_e = 1,
	avct_rsp_msg_e
};

const ushort REQUEST_ACCEPTED = 0x0000;

#define AVCTP_PSM	0x0017


typedef struct _LocalSideConfig {
    unsigned short  MTU;
    unsigned short  FlushTO;
    int             FutureUse[8];

} LocalSideConfig;

typedef int (*AVCT_ConnectReq_In)		(HANDLE hDeviceContext, BD_ADDR *pAddr,
	ushort PID, LocalSideConfig *pLSCR);
typedef int (*AVCT_ConnectRsp_In)		(HANDLE hDeviceContext, BD_ADDR *pAddr,
	ushort ConnectResult, ushort Status, LocalSideConfig *pLSCR);
typedef int (*AVCT_DisconnectReq_In) (HANDLE hDeviceContext, BD_ADDR *pAddr,
	ushort PID);
typedef int (*AVCT_SendMessage_In)  	(HANDLE hDeviceContext, BD_ADDR *pAddr,
	uchar Transaction, uchar Type, ushort PID, BD_BUFFER *pBdSendBuf);

typedef int (*AVCT_Connect_Ind)		(BD_ADDR *pAddr);
typedef int (*AVCT_Disconnect_Ind)   	(BD_ADDR *pAddr);
typedef int (*AVCT_MessageRec_Ind) 	(BD_ADDR *pAddr, 
	uchar Transaction, uchar Type, uchar *pData, ushort cData);

typedef int (*AVCT_Connect_Out)		(BD_ADDR *pAddr, 
	AVCT_RSP ConnectResult, ushort ConfigResult, ushort Status);
typedef int (*AVCT_Disconnect_Out)   	(BD_ADDR *pAddr, 
	ushort DisconnectResult);


typedef struct _AVCT_INTERFACE {
    AVCT_ConnectReq_In		AvctConnectReqIn;
    AVCT_ConnectRsp_In		AvctConnectRspIn;
    AVCT_DisconnectReq_In	AvctDisconnectReqIn;
    AVCT_SendMessage_In     AvctSendMessageIn;

    BT_LAYER_IO_CONTROL     AvctIoctl;
    BT_LAYER_ABORT_CALL    	AvctAbortCall;
} AVCT_INTERFACE, *PAVCT_INTERFACE;

typedef struct _AVCT_EVENT_INDICATION {
    AVCT_Connect_Ind     		AvctConnectInd;
    AVCT_Disconnect_Ind     	AvctDisconnectInd;
    AVCT_MessageRec_Ind     	AvctMessageRecInd;

    BT_LAYER_STACK_EVENT_IND	AvctStackEvent;
} AVCT_EVENT_INDICATION, *PAVCT_EVENT_INDICATION;

typedef struct _AVCT_CALLBACKS {
    AVCT_Connect_Out			AvctConnectOut;
    AVCT_Disconnect_Out       	AvctDisconnectOut;

    BT_LAYER_CALL_ABORTED       AvctCallAborted;
} AVCT_CALLBACKS, *PAVCT_CALLBACKS;


int AVCT_EstablishDeviceContext
(
uint					Version,			/* IN */
ushort					ProfileID,			/* IN */
void*                   pUserContext,       /* IN */
PAVCT_EVENT_INDICATION 	pInd,               /* IN */
PAVCT_CALLBACKS        	pCall,              /* IN */
PAVCT_INTERFACE        	pInt,               /* OUT */
int*                    pcDataHeaders,      /* OUT */
int*                    pcDataTrailers,     /* OUT */
HANDLE*                 phDeviceContext    /* OUT */
);


int AVCT_CloseDeviceContext
(
HANDLE hDeviceContext       /* IN */
);

int avct_InitializeOnce (void);
int avct_CreateDriverInstance (void);
int avct_CloseDriverInstance (void);
int avct_UninitializeOnce (void);



#endif	// __bt_actp_H__
