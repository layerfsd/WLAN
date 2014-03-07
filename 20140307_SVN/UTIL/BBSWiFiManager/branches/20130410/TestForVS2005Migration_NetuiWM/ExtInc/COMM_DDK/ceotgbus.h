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
// Module Name:  
//     CEBUS.h
// 
// Abstract: Provides Liberary for OTG Bus Access.
// 
// Notes: 
//
#ifndef __CEOTGBUS_H_
#define __CEOTGBUS_H_
#include <cebus.h>

#define IOCTL_BUS_USBOTG_GETOTG_ENABLE_BIT  _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE)
#define IOCTL_BUS_USBOTG_REQUEST_BUS        _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+1)
#define IOCTL_BUS_USBOTG_DROP_BUS           _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+2)
#define IOCTL_BUS_USBOTG_RESUME             _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+3)
#define IOCTL_BUS_USBOTG_SUSPEND            _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+4)
#define IOCTL_BUS_USBOTG_HNP_ENABLE         _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+5)
#define IOCTL_BUS_USBOTG_HNP_DISABLE        _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+6)
#define IOCTL_BUS_USBOTG_GET_HOST_PORT      _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+7)
#define IOCTL_BUS_USBOTG_NOTIFY_DETACH      _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+8)
#define IOCTL_BUS_USBOTG_GET_A_ATTACH_EVENT _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+9)
#define IOCTL_BUS_USBOTG_USBFN_ACTIVE       _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+10)
#define IOCTL_BUS_USBOTG_HCD_ACCEPT         _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+11)

// For IOCTL_BUS_USBOTG_GETOTG_ENABLE_BIT The output on *pOutBuf.
#define USBOTG_HCD_ENABLE 1
#define USBOTG_DEVICE_ENABLE 2

#define USBOTG_CONNECT_DEFAULT_A 0x10

#endif
