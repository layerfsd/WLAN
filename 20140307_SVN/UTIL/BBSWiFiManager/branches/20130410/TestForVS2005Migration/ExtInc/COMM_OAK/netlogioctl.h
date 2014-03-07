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

#ifndef _NETLOGIOCTL_H
#define _NETLOGIOCTL_H
typedef struct _NETLOG_GLOBAL_STATE
{
    BOOL		bLogUSB;  //  Not IOCTL Controled.
    BOOL        bStopped; //  Default we are stopped. 
    DWORD       dwMaxPacketSize; // Log Packet upto len - dwMaxPacketSize;
    DWORD       dwHalfCaptureSize; // Size at which we need to roll over to alternate packetcapture.
    WCHAR       wszCapFileBaseName[MAX_PATH+1]; // Set using an IOCTL
    DWORD       dwCapFileIndex; // a 0 or 1 depending on file rollover.
} NETLOG_GLOBAL_STATE;
// Specifies IOCTL interface between netlog and netlogctl.

#define   IOCTL_NETLOG_START                        1
#define   IOCTL_NETLOG_STOP                         2
#define   IOCTL_NETLOG_FILE_NAME_CHANGE             3
#define   IOCTL_NETLOG_FILE_SET_PARAM               4
#define   IOCTL_NETLOG_CAPTURE_PACKET_SIZE          5
#define   IOCTL_NETLOG_HALF_CAPTURE_SIZE            6
#define   IOCTL_NETLOG_LOG_USB                      7
#define   IOCTL_NETLOG_GET_STATE                    8

#endif 
