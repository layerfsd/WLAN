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
/*-------------------------------------------------------------------------
  File: intshare.h

  This is the public header file for the PC-Link library.
-------------------------------------------------------------------------*/

#ifndef __INTSHARE_H__
#define __INTSHARE_H__

#include <windows.h>

typedef enum _InternetSharingConnectionType
{
    InternetSharing_Type_RNDIS = 0,
    InternetSharing_Type_BTPAN,
    InternetSharing_Type_GENERIC_NDIS,
    InternetSharing_Type_CELLULAR_DATA,
    InternetSharing_Type_UNDEFINED = 0xff
} InternetSharingConnectionType;

#ifdef __cplusplus
extern "C" {
#endif

DWORD InternetSharingEnable(const InternetSharingConnectionType PrivateType, LPCWSTR szPrivateInstance, 
        const InternetSharingConnectionType PublicType, LPCWSTR szPublicConnection);

DWORD InternetSharingDisable(void);

HANDLE RegisterForInternetSharingNotifications(HANDLE hMsgQ);

BOOL DeregisterForInternetSharingNotifications(HANDLE h);

#ifdef __cplusplus
}
#endif

// Internet Sharing connection events
#define INTERNET_SHARING_EVENT_DISCONNECTED   0
#define INTERNET_SHARING_EVENT_CONNECTED      1
#define INTERNET_SHARING_EVENT_CONNECTING     2

// The following are connection sources (see InternetSharingNotification)
typedef enum _InternetSharingEventSource
{
    InternetSharing_Source_UNDEFINED = 0,
    InternetSharing_Source_PRIVATE,
    InternetSharing_Source_PUBLIC
} InternetSharingEventSource;

// Notification structure for PC-Link message queue
typedef struct _InternetSharingNotification
{
    DWORD dwSize;                    // For version
    InternetSharingEventSource EventSource;   // Source of the event (private/public)
    DWORD dwEvent;                   // Event that occurred (see below)
    DWORD dwStatus;                  // Optional status
} InternetSharingNotification;

#endif // __INTSHARE_H__

