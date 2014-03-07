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

pegdpar.h

Abstract:  

Notes: 


--*/
//	@doc	EXTERNAL SERIALDEVICE

#ifndef __PEGDPAR_H__
#define __PEGDPAR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "windev.h"

//
// DeviceIOControl dwIoControlCode values for Serial ports
//
#define IOCTL_PARALLEL_SET_TIMEOUTS       CTL_CODE(FILE_DEVICE_PARALLEL_PORT,1,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PARALLEL_GET_TIMEOUTS       CTL_CODE(FILE_DEVICE_PARALLEL_PORT,2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PARALLEL_GETDEVICEID        CTL_CODE(FILE_DEVICE_PARALLEL_PORT,3,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PARALLEL_WRITE              CTL_CODE(FILE_DEVICE_PARALLEL_PORT,4,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PARALLEL_GET_ECP_REV_TRANS  CTL_CODE(FILE_DEVICE_PARALLEL_PORT,5,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PARALLEL_STATUS             CTL_CODE(FILE_DEVICE_PARALLEL_PORT,6,METHOD_BUFFERED,FILE_ANY_ACCESS)

#ifdef __cplusplus
}
#endif

#endif // __PEGDPAR_H_
