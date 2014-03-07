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
//          Copyright © 2002, Microsoft Corporation
////////////////////////////////////////////////////////////////////////////////
//
// @doc
//
// @module  prpushsrc.h | 
//          Push Router Source APIs
//
// @comm    This header defines the source interface for the Push Router.  
//          This API accepts a push message and submits it into the Push Router. 
//          The Push Router authenticates the message, and then routes the message 
//          to a registered push client, based on the message’s Content-Type header 
//          or X-Wap-Application-Id header, or both.  (See the pushclient.h header file
//          for push client APIs.)
//
//          The Push Router identifies a message as a WAP provisioning push message
//          if the value of its Content-Type header is one of the following: 
//              text/vnd.wap.connectivity-xml (for XML content)
//              application/vnd.wap.connectivity-wbxml (for WBXML content)
//
// @ex      The following client example shows how to use the PushRouter_SubmitPush API. |
/*

#include <windows.h>
#include <prpushsrc.h>         

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPTSTR szCmdLine, int nCmdShow)
{
    HRESULT     hr = S_OK;
    SRCPUSHMSG  msg = {0};
    TCHAR const c_szHeaders[] = TEXT("Content-Type: text/vnd.wap.connectivity-xml\r\nX-Wap-Application-Id: x-wap-microsoft:cfgmgr.ua\r\n\r\n");    
    char const  c_aszBody[] = 
        "<wap-provisioningdoc>"
            "<characteristic type=\"BrowserFavorite\">"   
                "<characteristic type=\"MSN\">"   
                    "<parm name=\"URL\" value=\"http://www.msn.com\"/>"  
                "</characteristic>"
            "</characteristic>"  
        "</wap-provisioningdoc>";

    msg.cbSize = sizeof(msg);
    msg.pszHeaders = c_szHeaders;
    msg.pbBody = (LPBYTE)c_aszBody;    // UTF-8 data
    msg.cbBody = sizeof(c_aszBody) - 1; // -1 for NULL-terminator
    msg.pszSenderAddress = TEXT("14252223333"); // Address (here, telephone number) of push proxy gateway

    hr = PushRouter_SubmitPush(&msg);
    return 0;
}


*/
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __cplusplus
extern "C" { 
#endif // __cplusplus

#ifndef PUSHROUTERAPI
#define PUSHROUTERAPI
#endif 

//
// Push Router Structures
//

////////////////////////////////////////////////////////////////////////////////
//
// @struct  PUSHMSG |
//          Message structure to send into the Push Router 
//
// @comm    The cbSize member needs to be initialized by the API's client to be the size of the 
//          SRCPUSHMSG structure before the SRCPUSHMSG structure can be passed into 
//          PushRouter_SubmitPush.
////////////////////////////////////////////////////////////////////////////////

typedef struct tagSrcPushMsg {
    DWORD   cbSize;             // @field size of SRCPUSHMSG structure
    LPCTSTR pszHeaders;         // @field RFC 822 headers 
    LPBYTE  pbBody;             // @field push message body
    DWORD   cbBody;             // @field size of the message body (in bytes)
    DWORD   dwReserved;         // @field Reserved; set to 0
    LPCTSTR pszSenderAddress;   // @field address of message sender/source
} SRCPUSHMSG, *PSRCPUSHMSG;

//
// Push Router Functions
//

////////////////////////////////////////////////////////////////////////////////
//
// @func    HRESULT | PushRouter_SubmitPush |
//          This method pushes a message into the pushrouter.
//
// @syntax  PushRouter_SubmitPush(PSRCPUSHMSG pMsg);
//
// @parm    PSRCPUSHMSG | pMsg |
//          Message to be routed to appropriate client.
//
// @rvalue  S_OK | Success.
//
// @rdesc   HRESULT: An HRESULT describing any errors that occurred.
//
////////////////////////////////////////////////////////////////////////////////

HRESULT PUSHROUTERAPI PushRouter_SubmitPush(PSRCPUSHMSG pMsg);


#ifdef __cplusplus
};
#endif // __cplusplus
