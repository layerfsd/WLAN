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
 * Configuration Host
 *
 * (c) Copyright Microsoft Corp. 2000 All Rights Reserved
 *
 *  module: cfghost.h
 *  author: scottsh
 *
 *  purpose: cfghost message class
 *
 *  history: 10/26/2000     Scott Shell (ScottSh)       Created
 *           01/24/2001     Hung Dang (hdang)           Added UI
 *
\*---------------------------------------------------------------------------*/

#pragma once

#define CMRF_NO_RESPONSE            0x00000000
#define CMRF_SHORT_RESPONSE         0x00000001
#define CMRF_LONG_RESPONSE          0x00000002
#define CMRF_INCLUDE_METADATA       0x00000010

#define CMRF_FLAG_MASK              0x00000003
#define CMRF_SUCCESS_MASK           0x00000003
#define CMRF_SUCCESS_SHIFT          0
#define CMRF_FAILURE_MASK           0x0000000C
#define CMRF_FAILURE_SHIFT          2


class CCfgMsg
{
public:
    CCfgMsg();
    ~CCfgMsg();

    HRESULT     Initialize(PPUSHMSG pPushMsg);
    HRESULT     Process(PR_HANDLE hPushRouter);
    HRESULT     Release();
    LPTSTR      GetHeaderValue(LPCTSTR pszTargetHeader);

#ifdef DEBUG
    HRESULT     Display();
#endif

private:
    void        SetSuccessResponse(DWORD dw) { m_dwResponseFlags = (m_dwResponseFlags & ~CMRF_SUCCESS_MASK) | ((dw & CMRF_FLAG_MASK) << CMRF_SUCCESS_SHIFT); }
    void        SetFailureResponse(DWORD dw) { m_dwResponseFlags = (m_dwResponseFlags & ~CMRF_FAILURE_MASK) | ((dw & CMRF_FLAG_MASK) << CMRF_FAILURE_SHIFT); }
    void        SetMetadata(BOOL f) { m_dwResponseFlags = (m_dwResponseFlags & ~CMRF_INCLUDE_METADATA) | (f ? CMRF_INCLUDE_METADATA : 0); }
    DWORD       GetSuccessResponse() { return (m_dwResponseFlags & CMRF_SUCCESS_MASK) >> CMRF_SUCCESS_SHIFT; }
    DWORD       GetFailureResponse() { return (m_dwResponseFlags & CMRF_FAILURE_MASK) >> CMRF_FAILURE_SHIFT; }
    BOOL        GetMetadata() { return m_dwResponseFlags & CMRF_INCLUDE_METADATA; }

    LPTSTR      m_szHeaders;
    BSTR        m_bstrConfigXML;
    DWORD       m_dwSecurityRole;     
    LPTSTR      m_szResponseAddress;  
    DWORD       m_dwResponseFlags;
};


#define RESPONSE_CODE_SUCCESS   200
#define RESPONSE_CODE_FAILURE   400

class CCfgHostApp
{
public:
    HRESULT Initialize(HINSTANCE hInstance);
    HRESULT ProcessUpdateResult(HRESULT hrCfgResult, CCfgMsg *pMsg);
    int     GetUserPermission(PPUSHMSG pAuthMsg);

    CCfgHostApp() {}
    ~CCfgHostApp() {}

private:
    HRESULT     OnSuccessfulConfig(LPTSTR wszInitiator, CCfgMsg *pMsg);
    HRESULT     OnFailedConfig(LPTSTR wszInitiator, CCfgMsg *pMsg);
    HRESULT     DisplayOOM();
    HRESULT     SendNotification(LPTSTR pszSender, LPTSTR pszSubject, LPTSTR pszBody, DWORD dwFlags);

    HINSTANCE   m_hInstance;
};
