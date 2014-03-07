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
// bkldrv.h
// 
// Backlight Driver Header file
//

#pragma once

#define FILE_DEVICE_BACKLIGHT            0x300

#define IOCTL_BACKLIGHT_ON              CTL_CODE(FILE_DEVICE_BACKLIGHT, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BACKLIGHT_OFF             CTL_CODE(FILE_DEVICE_BACKLIGHT, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BACKLIGHT_TOGGLE          CTL_CODE(FILE_DEVICE_BACKLIGHT, 3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BACKLIGHT_RESETTIMER      CTL_CODE(FILE_DEVICE_BACKLIGHT, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)