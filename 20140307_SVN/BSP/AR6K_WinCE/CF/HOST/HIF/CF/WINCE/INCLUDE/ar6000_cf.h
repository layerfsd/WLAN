//------------------------------------------------------------------------------
// <copyright file="ar6000_cf.h" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    The software is licensed “as-is.” 
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// AR6000 HIF CF definitions
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef _AR6000_CS_H_
#define _AR6000_CS_H_

#include "athtypes.h"
#include "athdefs.h"

typedef struct _CF_FUNCTION_ CF_FUNCTION;
typedef struct _CF_REQUEST_ CF_REQUEST;

typedef PVOID CF_DEVICE_HANDLE;

/* 
 * Structure used by the upper layer to register itself with the
 * device driver.
 */
typedef struct _CF_FUNCTION_ {
    /* name of registering driver */
	A_UCHAR       *pName;       
    
	/* callback functions provided by the Upper layer module to the driver.  */
	A_BOOL (*pProbe)(CF_DEVICE_HANDLE handle);
	VOID (*pRemove)(CF_DEVICE_HANDLE handle);
	VOID (*pIsr)(CF_DEVICE_HANDLE handle,A_BOOL *callDsr);
	VOID (*pInterruptPending)(CF_DEVICE_HANDLE handle, A_BOOL *intPending);
	VOID (*pDsr)(CF_DEVICE_HANDLE handle);
} CF_FUNCTION;

/* CFREQUEST request flags */
typedef A_UINT8 CF_REQUEST_FLAGS;

/* write operation */
#define CFREQ_FLAGS_DATA_WRITE         0x01
#define CFREQ_FLAGS_FIXED_ADDRESS	   0x02

typedef struct _CF_REQUEST_ {
    PVOID            pDataBuffer; /* starting address of buffer */
	A_UINT32	     address;     /* address to which data has to be written */
	A_UINT32	     length;      /* length of data to be written */
	CF_REQUEST_FLAGS flags;
} CF_REQUEST;

/* 
 * API function prototypes
 */
A_STATUS 
CF_RegisterFunction(CF_FUNCTION *pFunction);

A_STATUS 
CF_UnregisterFunction(CF_FUNCTION *pFunction);

A_STATUS 
CF_BusRequest(CF_DEVICE_HANDLE handle, CF_REQUEST *pReq, A_UINT32 bits);

A_STATUS
CF_UnMaskInterrupt(CF_DEVICE_HANDLE cfHandle);

A_STATUS
CF_MaskInterrupt(CF_DEVICE_HANDLE cfHandle);

#endif
