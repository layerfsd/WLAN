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


Module Name:

    ndispwr.h

Abstract:

    Data structures, defines and function prototypes for ndispwr module
    which remembers power state of ndis adapters..


Environment:

    

Revision History:

    November 2001  :: Original version.

--*/

#ifndef __NDISPWR__H_
#define __NDISPWR__H_


//
//  Handy defines..
//

#define NDISPWR_DEVICE_NAME     TEXT("NPW1:")

#define REG_NDISPOWER_ROOT      TEXT("Comm\\NdisPower")

#define FSCTL_NDISPWR_BASE      FILE_DEVICE_NETWORK

#define _NDISPWR_CTL_CODE(_Function, _Method, _Access)  \
            CTL_CODE(FSCTL_NDISPWR_BASE, _Function, _Method, _Access)


//
//  Valid IOCTL for NPW1:
//

#define IOCTL_NPW_SAVE_POWER_STATE  \
            _NDISPWR_CTL_CODE(0x200, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_NPW_QUERY_SAVED_POWER_STATE  \
            _NDISPWR_CTL_CODE(0x201, METHOD_BUFFERED, FILE_ANY_ACCESS)            


typedef struct _NDISPWR_SAVEPOWERSTATE
{
    LPCWSTR                pwcAdapterName;
    CEDEVICE_POWER_STATE   CePowerState;

}   NDISPWR_SAVEPOWERSTATE, *PNDISPWR_SAVEPOWERSTATE;



#endif  //  __NDISPWR__H_
