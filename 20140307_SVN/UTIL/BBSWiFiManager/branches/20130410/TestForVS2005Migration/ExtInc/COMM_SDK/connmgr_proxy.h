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

#pragma once

// Proxy CSP config/settings
DEFINE_GUID(IID_ConnPrv_IProxyExtension, 0xaf96b0bd, 0xa481, 0x482c, 0xa0, 0x94, 0xa8, 0x44, 0x87, 0x67, 0xa0, 0xc0);

#define CMPROXY_PROXYSERVER_MAXSIZE 256
#define CMPROXY_USERNAME_MAXSIZE 32
#define CMPROXY_PASSWORD_MAXSIZE 32
#define CMPROXY_EXTRAINFO_MAXSIZE 256
#define CMPROXY_PROXYOVERRIDE_MAXSIZE 64

typedef struct _PROXY_CONFIG
{
    DWORD dwType;
    DWORD dwEnable;
    TCHAR szProxyServer[CMPROXY_PROXYSERVER_MAXSIZE];
    TCHAR szUsername[CMPROXY_USERNAME_MAXSIZE];
    TCHAR szPassword[CMPROXY_PASSWORD_MAXSIZE];
    TCHAR szProxyOverride[CMPROXY_PROXYOVERRIDE_MAXSIZE];
    TCHAR szExtraInfo[CMPROXY_EXTRAINFO_MAXSIZE];
} PROXY_CONFIG;


