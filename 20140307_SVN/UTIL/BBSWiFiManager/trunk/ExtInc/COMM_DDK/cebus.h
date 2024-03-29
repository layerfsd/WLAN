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
// Abstract: Provides Liberary for Bus Access.
// 
// Notes: 
//
#ifndef __CEBUS_H_
#define __CEBUS_H_

#include <pm.h>


#define _BUSACCESS_CTL_CODE(_Function)  \
            CTL_CODE(FILE_DEVICE_BUS_EXTENDER, _Function, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_BUS_TRANSLATE_BUS_ADDRESS _BUSACCESS_CTL_CODE(1)
#define IOCTL_BUS_TRANSLATE_SYSTEM_ADDRESS _BUSACCESS_CTL_CODE(2)

#define IOCTL_BUS_GET_POWER_STATE   _BUSACCESS_CTL_CODE(3)
#define IOCTL_BUS_SET_POWER_STATE   _BUSACCESS_CTL_CODE(4)


#define IOCTL_BUS_GET_CONFIGURE_DATA    _BUSACCESS_CTL_CODE(5)
#define IOCTL_BUS_SET_CONFIGURE_DATA    _BUSACCESS_CTL_CODE(6)

#define IOCTL_BUS_ACTIVATE_CHILD    _BUSACCESS_CTL_CODE(16)
#define IOCTL_BUS_DEACTIVATE_CHILD  _BUSACCESS_CTL_CODE(17)
#define IOCTL_BUS_POSTINIT          _BUSACCESS_CTL_CODE(18)

#define IOCTL_BUS_IS_CHILD_REMOVED   _BUSACCESS_CTL_CODE(32)
#define IOCTL_BUS_NAME_PREFIX _BUSACCESS_CTL_CODE(33)

#define IOCTL_BUS_OEM_EXTENDER  0x800

typedef struct _CE_BUS_POWER_STATE {
    LPTSTR lpDeviceBusName;
    PCEDEVICE_POWER_STATE lpceDevicePowerState;
    PVOID  lpReserved; // THis is reserved for future.
} CE_BUS_POWER_STATE, *PCE_BUS_POWER_STATE;

// Structure for Translate Bus Address.
typedef struct _CE_BUS_TRANSLATE_BUS_ADDR {
    LPTSTR lpDeviceBusName;
    INTERFACE_TYPE  InterfaceType;
    ULONG BusNumber;
    PHYSICAL_ADDRESS BusAddress;
    PULONG AddressSpace;
    PPHYSICAL_ADDRESS TranslatedAddress;
} CE_BUS_TRANSLATE_BUS_ADDR,*PCE_BUS_TRANSLATE_BUS_ADDR;

typedef struct _CE_BUS_TRANSLATE_SYSTEM_ADDR {
    LPTSTR lpDeviceBusName;
    INTERFACE_TYPE  InterfaceType;
    ULONG BusNumber;
    PHYSICAL_ADDRESS SystemAddress;
    PPHYSICAL_ADDRESS TranslatedAddress;
} CE_BUS_TRANSLATE_SYSTEM_ADDR,*PCE_BUS_TRANSLATE_SYSTEM_ADDR;

typedef struct _CE_BUS_DEVICE_CONFIGURATION_DATA {
    LPTSTR lpDeviceBusName;
    DWORD dwSpace;
    DWORD dwOffset;
    DWORD dwLength;
    PVOID pBuffer;
} CE_BUS_DEVICE_CONFIGURATION_DATA,*PCE_BUS_DEVICE_CONFIGURATION_DATA ;

// Bus Extention for USB OTG
#define BUS_USBOTG_EXTENTION_FUNCTIONCODE 256
#define BUS_USBOTG_EXTENTION_FUNCTIONCODE_SIZE 16
#define IOCTL_BUS_USBOTG_BEGIN _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE)
#define IOCTL_BUS_USBOTG_END   _BUSACCESS_CTL_CODE(BUS_USBOTG_EXTENTION_FUNCTIONCODE+BUS_USBOTG_EXTENTION_FUNCTIONCODE_SIZE-1)

#endif
