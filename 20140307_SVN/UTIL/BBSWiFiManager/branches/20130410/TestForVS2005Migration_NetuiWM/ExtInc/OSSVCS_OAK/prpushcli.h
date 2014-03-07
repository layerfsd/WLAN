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
/*---------------------------------------------------------------------------*\
 *
 * Push Router Client Interface
 *
 * (c) Copyright Microsoft Corp. 2000 All Rights Reserved
 *
 *  module: pushcli.h
 *  author: scottsh
 *
 *  purpose: Client Interface to Push Router
 *
 *	history: 10/10/2000  	Scott Shell (ScottSh)		Created
 *
\*---------------------------------------------------------------------------*/

#pragma once

#ifdef __cplusplus
extern "C" { 
#endif // __cplusplus

#ifndef PUSHROUTERAPI
#define PUSHROUTERAPI
#endif  

typedef DWORD PR_HANDLE;

//
// Error Return values
// 

#define PUSHRTR_E_ALREADYOPENED         0x80530001
#define PUSHRTR_S_NOMESSAGESPENDING     0x00530002
#define PUSHRTR_E_ALREADYREGISTERED     0x80530003
#define PUSHRTR_E_NOTFOUND              0x80530004
#define PUSHRTR_E_EXCEPTION             0x80530005

//
// Push Router Structures
//

// MAKE SURE THIS IS THE SAME SIZE AS THE EQUIVALENT STRUCT IN pushclient.h (sdk file)
typedef struct tagPushMsg {
    DWORD   cbSize;
	LPTSTR  szHeaders;          // RFC 822 headers
	LPBYTE  pbBody;
	DWORD   dwBodyLength;
	DWORD   dwSecurityRole;     // Should security role be a string instead of a number?
                                // Are security roles strictly ordered, or are they simply a list?
	BOOL    fResponseRequested;
	LPTSTR  szResponseAddress;  // URL or phone number
} PUSHMSG, *PPUSHMSG;

typedef struct tagResponseMsg {
    DWORD   cbSize;
	LPTSTR  szDestinationAddress;
	LPTSTR  szHeaders;
	LPBYTE  pbBody;
	DWORD   dwBodyLength;
	BOOL    fSignatureRequired;
} RESPONSEMSG, *PRESPONSEMSG;

//
// Push Router Functions
//

HRESULT PUSHROUTERAPI
PushRouter_RegisterClient(LPCTSTR szContentType, LPCTSTR szAppId, LPCTSTR szPath, LPCTSTR szParams);			

HRESULT PUSHROUTERAPI
PushRouter_UnRegisterClient(LPCTSTR szContentType, LPCTSTR szAppId);					

HRESULT PUSHROUTERAPI
PushRouter_Open(LPCTSTR szContentType, LPCTSTR szAppId, HANDLE *pMsgAvailableEvent, PR_HANDLE *phPushRouter);

HRESULT PUSHROUTERAPI
PushRouter_Close(PR_HANDLE hPushRouter);

HRESULT PUSHROUTERAPI
PushRouter_GetMessage(PR_HANDLE hPushRouter, PUSHMSG *pPushMsg);

HRESULT PUSHROUTERAPI
PushRouter_SendResponse(PRESPONSEMSG pRespMsg);

HRESULT PUSHROUTERAPI
PushRouter_FreeMessage(PPUSHMSG pMsg);


#ifdef __cplusplus
};
#endif // __cplusplus
