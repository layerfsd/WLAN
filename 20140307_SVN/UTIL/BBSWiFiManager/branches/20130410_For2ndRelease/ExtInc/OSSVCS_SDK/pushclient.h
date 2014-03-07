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
////////////////////////////////////////////////////////////////////////////////
//			Copyright © 2002, Microsoft Corporation
////////////////////////////////////////////////////////////////////////////////
//
// @doc
//
// @module	pushclient.h | 
//			Push Router Client Interface
//
// @comm    There are six methods in the Push Router Client Interface. 
//          They are defined in pushclient.h. These are APIs on the Push Router proxy dll. 
//          Push Router clients call these APIs. 
//
//          A push router client is a standalone executable that gets registered 
//          with the Push Router and then gets launched if an incoming message matches the  
//          application ID/content-type combination that is registered for that particular client.  
//          Once lauched, the client must call PushRouter_Open to get a handle to Push Router and an event  
//          handle that the client uses to determine when messages arrive for the client. When the event 
//          handle is signaled, the client calls PushRouter_GetMessage to retrieve the message. 
//          Note that, in addition to returning standard Windows HRESULTS, these API's can 
//          also return Push Router-specific HRESULTS, defined in pushclient.h and prefixed by PUSHRTR_.
//
// @ex      The following client example shows how to use these APIs to retrieve 
//          a push message from the Push Router. |
/*

#include "windows.h"
#include "pushclient.h"         

TCHAR const c_szAppId[] = TEXT("TestAppId");
TCHAR const c_szPath[] = TEXT("pushclnt.exe");
TCHAR const c_szContentType[] = TEXT("text/xml");
TCHAR const c_szParams[] = TEXT("/c");

HRESULT
HandleMessage(PUSHMSG *pPushMsg)
{
    // Handle the message here
    return S_OK;

}

int 
WINAPI
WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPTSTR szCmdLine, int nCmdShow)
{
    HRESULT         hr              = NOERROR;
    HANDLE          hevtMsgAvailable= NULL;
    HPUSHROUTER     hPushRouter     = NULL;
    PUSHMSG         PushMsg;

    memset(&PushMsg, 0, sizeof(PUSHMSG));

    if(!_tcsicmp(szCmdLine, TEXT("/register"))) 
    {
        hr = PushRouter_RegisterClient(c_szContentType, c_szAppId, c_szPath, c_szParams);
        goto Exit;
    } 
    else if(!_tcsicmp(szCmdLine, TEXT("/unregister"))) 
    {
        hr = PushRouter_UnRegisterClient(c_szContentType, c_szAppId);
        goto Exit;
    }
    else if (!_tcsicmp(szCmdLine, TEXT("/c")))
    {
        // Handle the "/c" parameter here.
    }

    hr = PushRouter_Open(c_szContentType, c_szAppId, &hevtMsgAvailable, &hPushRouter);
    if (FAILED(hr))
    {
        goto Exit;
    }

    while(WaitForSingleObject(hevtMsgAvailable, 10000) != WAIT_TIMEOUT) 
    {
        memset(&PushMsg, 0, sizeof(PUSHMSG));
        PushMsg.cbSize = sizeof(PUSHMSG);

        hr = PushRouter_GetMessage(hPushRouter, &PushMsg);

        if(SUCCEEDED(hr)) 
        {
            hr = HandleMessage(&PushMsg);
            if(FAILED(hr)) 
            {
                goto Exit;
            }

            PushRouter_FreeMessage(&PushMsg);
        }
    }

    hr = PushRouter_Close(hPushRouter);
                                              
Exit:
    PushRouter_FreeMessage(&PushMsg);
    return 0;

}

*/
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __cplusplus
extern "C" { 
#endif // __cplusplus

// @type HPUSHROUTER | Handle to the Push Router.
typedef DWORD HPUSHROUTER;

//
// Push Router-specific Error Return values
// 

// Another client is already opened with same application id/content-type combination.
#define PUSHRTR_E_ALREADYOPENED         0x80530001

// No messages are pending in client's queue.
#define PUSHRTR_S_NOMESSAGESPENDING     0x00530002

// Another client is already registered with same application id/content-type combination.
#define PUSHRTR_E_ALREADYREGISTERED     0x80530003

// No registration exists for the client to be opened or unregistered.
#define PUSHRTR_E_NOTFOUND              0x80530004

// An exception occurred in a pushrouter API.
#define PUSHRTR_E_EXCEPTION             0x80530005
//
// Push Router Structures
//

////////////////////////////////////////////////////////////////////////////////
//
// @struct	PUSHMSG |
//			Message structure returned to the client from PushRouter_GetMessage 
//
// @comm    The cbSize member needs to be initialized by the push client to be the size of the 
//          PUSHMSG structure before the PUSHMSG structure can be passed into 
//          PushRouter_GetMessage.
////////////////////////////////////////////////////////////////////////////////

typedef struct tagPushMsg {
    DWORD   cbSize;             // @field size of PUSHMSG structure
	LPTSTR  szHeaders;          // @field RFC 822 headers
	LPBYTE  pbBody;             // @field push message body
	DWORD   cbBodyLength;       // @field size of the message body (in bytes)
	DWORD   dwSecurityRole;     // @field Security role(s) assigned to the message by Push Router
	DWORD   dwReserved1;        // @field Reserved; set to 0
	DWORD   dwReserved2;        // @field Reserved; set to 0
} PUSHMSG, *LPPUSHMSG;

//
// Push Router Functions
//

////////////////////////////////////////////////////////////////////////////////
//
// @func	HRESULT | PushRouter_RegisterClient |
//			This method registers a client in the Push Router's Registration 
//          Table. All clients that require any interaction with the Push Router 
//          must register using this method.
//
// @syntax  PushRouter_RegisterClient(LPCTSTR szContentType, LPCTSTR szAppId, LPCTSTR szPath, LPCTSTR szParams);
//
// @parm	LPCTSTR | szContentType |
//			Content-type of messages routed to the client
//
// @parm	LPCTSTR | szAppId |
//			Application ID of the client
//
// @parm	LPCTSTR | szPath |
//			Path to the client's executable file
//
// @parm	LPCTSTR | szParams |
//			Command-line parameters to be passed to the client upon launch
//
// @rvalue	S_OK | Success.
// @rvalue	PUSHRTR_E_ALREADYREGISTERED | Another client is already registered with same application id/content-type combination.
// @rvalue	PUSHRTR_E_EXCEPTION | An exception occurred in a pushrouter API.
//
// @rdesc	HRESULT: An HRESULT describing any errors that occurred.
//
////////////////////////////////////////////////////////////////////////////////

HRESULT 
PushRouter_RegisterClient(LPCTSTR szContentType, LPCTSTR szAppId, LPCTSTR szPath, LPCTSTR szParams);			


////////////////////////////////////////////////////////////////////////////////
//
// @func	HRESULT | PushRouter_UnRegisterClient |
//			This method deletes a client entry from the Registration table in the 
//          Push Router. If the client no longer wants to receive push messages 
//          from push router, the client should use this method to unregister itself.
//
// @comm    This API needs to be called once per Application ID/Content-Type combination.
//
// @syntax  PushRouter_UnRegisterClient(LPCTSTR szContentType, LPCTSTR szAppId);
//
// @parm	LPCTSTR | szContentType |
//			Content-type of messages routed to the client
//
// @parm	LPCTSTR | szAppId |
//			Application ID of the client
//
// @rvalue	S_OK | Success.
// @rvalue	PUSHRTR_E_NOTFOUND | No registration exists for the client to be unregistered.
// @rvalue	PUSHRTR_E_EXCEPTION | An exception occurred in a pushrouter API.
//
// @rdesc	HRESULT: An HRESULT describing any errors that occurred.
//
////////////////////////////////////////////////////////////////////////////////

HRESULT 
PushRouter_UnRegisterClient(LPCTSTR szContentType, LPCTSTR szAppId);					


////////////////////////////////////////////////////////////////////////////////
//
// @func	HRESULT | PushRouter_Open |
//			This method opens a communication channel to a client through which 
//          a push message can be passed.  
//
// @comm    CloseHandle should not be called on the message handle returned by PushRouter_Open.
//
// @syntax  PushRouter_Open(LPCTSTR szContentType, LPCTSTR szAppId, HANDLE *pMsgAvailableEvent, HPUSHROUTER *phPushRouter);
//
// @parm	LPCTSTR | szContentType |
//			Content-type of messages routed to the client
//
// @parm	LPCTSTR | szAppId |
//			Application ID of the client
//
// @parm	HANDLE | *pMsgAvailableEvent |
//			Handle to event signaled by the Push Router when a message is available
//
// @parm	HPUSHROUTER | *phPushRouter |
//			Handle to the Push Router
//
// @rvalue	S_OK | Success.
// @rvalue	PUSHRTR_E_NOTFOUND | No registration exists for the client to be opened.
// @rvalue	PUSHRTR_E_ALREADYOPENED | Another client is already opened with same application id/content-type combination.
// @rvalue	PUSHRTR_E_EXCEPTION | An exception occurred in a pushrouter API.
//
// @rdesc	HRESULT: An HRESULT describing any errors that occurred.
//
////////////////////////////////////////////////////////////////////////////////

HRESULT 
PushRouter_Open(LPCTSTR szContentType, LPCTSTR szAppId, HANDLE *pMsgAvailableEvent, HPUSHROUTER *phPushRouter);


////////////////////////////////////////////////////////////////////////////////
//
// @func	HRESULT | PushRouter_Close |
//			This method closes the communication with the client.
//
// @syntax  PushRouter_Close(HPUSHROUTER hPushRouter);
//
// @parm	HPUSHROUTER | hPushRouter |
//			Handle to the Push Router
//
// @rvalue	S_OK | Success.
// @rvalue	PUSHRTR_E_EXCEPTION | An exception occurred in a pushrouter API.
//
// @rdesc	HRESULT: An HRESULT describing any errors that occurred.
//
////////////////////////////////////////////////////////////////////////////////

HRESULT 
PushRouter_Close(HPUSHROUTER hPushRouter);


////////////////////////////////////////////////////////////////////////////////
//
// @func	HRESULT | PushRouter_GetMessage |
//			This method passes the message from the Push Router to the client.
//
// @comm    The cbSize member needs to be initialized by the push client to be the size of the 
//          PUSHMSG structure before the PUSHMSG structure can be passed into 
//          PushRouter_GetMessage.
//
// @syntax  PushRouter_GetMessage(HPUSHROUTER hPushRouter, LPPUSHMSG pPushMsg);
//
// @parm	HPUSHROUTER | hPushRouter |
//			Handle to the Push Router
//
// @parm	LPPUSHMSG | pPushMsg |
//			Message structure filled out by the Push Router
//
// @rvalue	S_OK | Success.
// @rvalue	PUSHRTR_S_NOMESSAGESPENDING | No messages are pending in client's queue.
// @rvalue	PUSHRTR_E_EXCEPTION | An exception occurred in a pushrouter API.
//
// @rdesc	HRESULT: An HRESULT describing any errors that occurred.
//
////////////////////////////////////////////////////////////////////////////////

HRESULT 
PushRouter_GetMessage(HPUSHROUTER hPushRouter, LPPUSHMSG pPushMsg);


////////////////////////////////////////////////////////////////////////////////
//
// @func	HRESULT | PushRouter_FreeMessage |
//			This method frees up the memory allocated in the client process space 
//          by the Push Router.
//
// @comm    If pMsg is NULL, nothing is done.  
//
// @syntax  PushRouter_FreeMessage(LPPUSHMSG pPushMsg);
//
// @parm	LPPUSHMSG | pPushMsg |
//			Message structure filled out by the Push Router
//
// @rvalue	S_OK | Success.
//
// @rdesc	HRESULT: An HRESULT describing any errors that occurred.
//
////////////////////////////////////////////////////////////////////////////////

HRESULT PushRouter_FreeMessage(LPPUSHMSG pPushMsg);


#ifdef __cplusplus
};
#endif // __cplusplus
