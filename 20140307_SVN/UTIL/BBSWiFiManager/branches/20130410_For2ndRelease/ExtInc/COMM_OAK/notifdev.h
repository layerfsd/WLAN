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

notifdev.h

Abstract:

Private portion of notify.h - Header for User Notifications

Notes: 


--*/
#ifndef _DEV_NOTIFY_H_
#define _DEV_NOTIFY_H_

#include <winioctl.h>
#include <notify.h>

#ifdef __cplusplus
extern "C"  {
#endif

#define NOTFIDEV_IOCTL_CeSetUserNotificationEx                  CTL_CODE(FILE_DEVICE_NOTIFY, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NOTFIDEV_IOCTL_CeClearUserNotification                  CTL_CODE(FILE_DEVICE_NOTIFY, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NOTFIDEV_IOCTL_CeRunAppAtTime                           CTL_CODE(FILE_DEVICE_NOTIFY, 3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NOTFIDEV_IOCTL_CeRunAppAtEvent                          CTL_CODE(FILE_DEVICE_NOTIFY, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NOTFIDEV_IOCTL_CeHandleAppNotifications                 CTL_CODE(FILE_DEVICE_NOTIFY, 5, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NOTFIDEV_IOCTL_CeGetUserNotificationPreferences         CTL_CODE(FILE_DEVICE_NOTIFY, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NOTFIDEV_IOCTL_CeEventHasOccurred                       CTL_CODE(FILE_DEVICE_NOTIFY, 7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NOTFIDEV_IOCTL_CeGetUserNotificationHandles             CTL_CODE(FILE_DEVICE_NOTIFY, 8, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NOTFIDEV_IOCTL_CeGetUserNotification                    CTL_CODE(FILE_DEVICE_NOTIFY, 9, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NOTFIDEV_IOCTL_CeGetNotificationThreadId                CTL_CODE(FILE_DEVICE_NOTIFY, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef union __notify_arg_buffer {
    struct {
        HANDLE h;
        CE_NOTIFICATION_TRIGGER *pcent;
        CE_USER_NOTIFICATION    *pceun;
    } CeSetUserNotificationEx_p;

    struct {
        HANDLE h;
    } CeClearUserNotification_p;

    struct {
        WCHAR      *lpszAppName;
        SYSTEMTIME *pstTime;
    } CeRunAppAtTime_p;

    struct {
        WCHAR      *lpszAppName;
        LONG       lWhichEvent;
    } CeRunAppAtEvent_p;

    struct {
        WCHAR      *lpszAppName;
    } CeHandleAppNotifications_p;

    struct {
        HWND                    hWndParent;
        CE_USER_NOTIFICATION    *pceun;
    } CeGetUserNotificationPreferences_p;

    struct {
        LONG    lWhichEvent;
        WCHAR   *pwszEndOfCommandLine;
    } CeEventHasOccurred_p;

    struct {
        HANDLE *rghNotifications;
        DWORD   cHandleCount;
        DWORD  *cHandlesNeeded;
    } CeGetUserNotificationHandles_p;

    struct {
        HANDLE  hNotification;
        DWORD   cBytes;
        DWORD  *pcBytesNeeded;
        BYTE   *pBuffer;
    } CeGetUserNotification_p;


} NotifyArgBuffer;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* _DEV_NOTIFY_H_ */
