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

serhw.h

Abstract:

Holds exports from hardware dependent interface to be made available
only to hardware independent interface. OEMs may implement these
functions however they see fit as long as the signatures and
functionality are consistent with these.


Notes: 


--*/

#ifndef __SERHW_H__
#define __SERHW_H__

#ifdef __cplusplus
extern "C" {
#endif



/* Exports from MDD layer
 */
VOID	EvaluateEventFlag(PVOID pHead, ULONG fdwEventMask);


typedef enum {
    INTR_NONE	=   0,
    INTR_LINE	=   1,
    INTR_RX	=   2,
    INTR_TX	=   4,
    INTR_MODEM	=   8
    } INTERRUPT_TYPE;

#ifdef USE_NEW_SERIAL_MODEL
typedef struct __HWOBJ HWOBJ, *PHWOBJ;

typedef struct __HW_VTBL    {
    PVOID	(*HWInit)(ULONG Identifier, PVOID pMDDContext, PHWOBJ pHWObj);
    BOOL	(*HWPostInit)(PVOID pHead);
    ULONG	(*HWDeinit)(PVOID pHead);
    BOOL	(*HWOpen)(PVOID pHead);
    ULONG	(*HWClose)(PVOID pHead);
    INTERRUPT_TYPE (*HWGetIntrType)(PVOID pHead);
    ULONG	(*HWRxIntrHandler)(PVOID pHead, PUCHAR pTarget, PULONG pBytes);
    VOID	(*HWTxIntrHandler)(PVOID pHead, PUCHAR pSrc, PULONG pBytes);
    VOID	(*HWModemIntrHandler)(PVOID pHead);
    VOID	(*HWLineIntrHandler)(PVOID pHead);
    ULONG	(*HWGetRxBufferSize)(PVOID pHead);
    BOOL	(*HWPowerOff)(PVOID pHead);
    BOOL	(*HWPowerOn)(PVOID pHead);
    VOID	(*HWClearDTR)(PVOID pHead);
    VOID 	(*HWSetDTR)(PVOID pHead);
    VOID	(*HWClearRTS)(PVOID pHead);
    VOID	(*HWSetRTS)(PVOID pHead);
    BOOL	(*HWEnableIR)(PVOID pHead, ULONG BaudRate);
    BOOL	(*HWDisableIR)(PVOID pHead);
    VOID	(*HWClearBreak)(PVOID pHead);
    VOID	(*HWSetBreak)(PVOID pHead);
    BOOL	(*HWXmitComChar)(PVOID pHead, UCHAR ComChar);
    ULONG	(*HWGetStatus)(PVOID pHead, LPCOMSTAT lpStat);
    VOID	(*HWReset)(PVOID pHead);
    VOID	(*HWGetModemStatus)(PVOID pHead, PULONG pModemStatus);
    VOID	(*HWGetCommProperties)(PVOID pHead, LPCOMMPROP pCommProp);
    VOID	(*HWPurgeComm)(PVOID pHead, DWORD fdwAction);
    BOOL	(*HWSetDCB)(PVOID pHead, LPDCB pDCB);
    BOOL	(*HWSetCommTimeouts)(PVOID pHead, LPCOMMTIMEOUTS lpCommTO);
    BOOL    (*HWIoctl)(PVOID pHead, DWORD dwCode,PBYTE pBufIn,DWORD dwLenIn,
                       PBYTE pBufOut,DWORD dwLenOut,PDWORD pdwActualOut);
    } HW_VTBL, *PHW_VTBL;

#else

typedef struct __HW_VTBL    {
    PVOID	(*HWInit)(ULONG Identifier, PVOID pMDDContext);
    ULONG	(*HWDeinit)(PVOID pHead);
    BOOL	(*HWOpen)(PVOID pHead);
    ULONG	(*HWClose)(PVOID pHead);
    ULONG	(*HWGetBytes)(PVOID pHead, PUCHAR pTarget, PULONG pBytes);
    PVOID	(*HWGetRxStart)(PVOID pHead);
    INTERRUPT_TYPE (*HWGetIntrType)(PVOID pHead);
    VOID	(*HWOtherIntrHandler)(PVOID pHead);
    VOID	(*HWLineIntrHandler)(PVOID pHead);
    ULONG	(*HWGetRxBufferSize)(PVOID pHead);
	VOID	(*HWTxIntrHandler)(PVOID pHead);
	ULONG	(*HWPutBytes)(PVOID pHead, PUCHAR pSrc, ULONG NumBytes, PULONG pBytesSent);
    BOOL	(*HWPowerOff)(PVOID pHead);
    BOOL	(*HWPowerOn)(PVOID pHead);
    VOID	(*HWClearDTR)(PVOID pHead);
    VOID 	(*HWSetDTR)(PVOID pHead);
    VOID	(*HWClearRTS)(PVOID pHead);
    VOID	(*HWSetRTS)(PVOID pHead);
    BOOL	(*HWEnableIR)(PVOID pHead, ULONG BaudRate);
    BOOL	(*HWDisableIR)(PVOID pHead);
    VOID	(*HWClearBreak)(PVOID pHead);
    VOID	(*HWSetBreak)(PVOID pHead);
    BOOL	(*HWXmitComChar)(PVOID pHead, UCHAR ComChar);
    ULONG	(*HWGetStatus)(PVOID pHead, LPCOMSTAT lpStat);
    VOID	(*HWReset)(PVOID pHead);
    VOID	(*HWGetModemStatus)(PVOID pHead, PULONG pModemStatus);
    VOID	(*HWGetCommProperties)(PVOID pHead, LPCOMMPROP pCommProp);
    VOID	(*HWPurgeComm)(PVOID pHead, DWORD fdwAction);
    BOOL	(*HWSetDCB)(PVOID pHead, LPDCB pDCB);
    BOOL	(*HWSetCommTimeouts)(PVOID pHead, LPCOMMTIMEOUTS lpCommTO);
    BOOL    (*HWIoctl)(PVOID pHead, DWORD dwCode,PBYTE pBufIn,DWORD dwLenIn,
                       PBYTE pBufOut,DWORD dwLenOut,PDWORD pdwActualOut);
    } HW_VTBL, *PHW_VTBL;
#endif

// Define for BindFlags.  Note we only have two mutually exclusive values,
// but this is a bitfield so we may add others later
#define     THREAD_IN_PDD   0   /* PDD is responsible for Intr Thread */
#define     THREAD_AT_INIT  1   /* MDD Spins Intr Thread at init time */
#define     THREAD_AT_OPEN  2   /* MDD Spins Intr Thread at open time */

// The next define is only for compares.  NEVER set both OPEN and INIT for thread
#define     THREAD_IN_MDD (THREAD_AT_INIT | THREAD_AT_OPEN)

#ifdef USE_NEW_SERIAL_MODEL
typedef struct __HWOBJ {
    ULONG	BindFlags; // Flags controlling MDD behaviour.  Se above.
    DWORD   dwIntID;   // Interrupt Identifier used if THREAD_AT_INIT or THREAD_AT_OPEN
    PHW_VTBL	pFuncTbl;
    } HWOBJ, *PHWOBJ;

#else
typedef struct __HWOBJ {
    PVOID	pDeviceParent;
    ULONG	BindFlags; // Flags controlling MDD behaviour.  Se above.
    DWORD   dwIntID;   // Interrupt Identifier used if THREAD_AT_INIT or THREAD_AT_OPEN
    PVOID	pReserved;
    PHW_VTBL	pFuncTbl;
    } HWOBJ, *PHWOBJ;
#endif
    
typedef struct __DEVICE_LIST {
    LPSTR	DllName;
    ULONG	NumberOfDevices;
    PHWOBJ	*DeviceArray;		// Points to an array of pointers
	} DEVICE_LIST, *PDEVICE_LIST;

#ifdef USE_NEW_SERIAL_MODEL
PHWOBJ	GetSerialObject(DWORD DeviceArrayIndex);
#else
PDEVICE_LIST	GetSerialObject(VOID);
#endif

#ifdef __cplusplus
}	
#endif

#endif /* __SERHW_H__ */
