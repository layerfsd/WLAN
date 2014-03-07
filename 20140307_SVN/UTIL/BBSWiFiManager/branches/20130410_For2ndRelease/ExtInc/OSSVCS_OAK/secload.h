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
//
// Security Loader Authentication Module 
//
//    Interface header file
//

#pragma once

#include "certhashlist.h"

#ifdef __cplusplus
extern "C" {
#endif 

// IOControl Codes for Security Loader Module
#define IOCTL_SECLOAD_POPULATE_CERTSTORE            CTL_CODE(FILE_DEVICE_HAL, 1001, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_POPULATE_CRL                  CTL_CODE(FILE_DEVICE_HAL, 1002, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_SET_UNSIGNED_POLICY           CTL_CODE(FILE_DEVICE_HAL, 1003, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_BEGIN_CERTIFY_MODULE          CTL_CODE(FILE_DEVICE_HAL, 1005, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_CERTIFY_MODULE                CTL_CODE(FILE_DEVICE_HAL, 1006, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_POPULATE_PEL                  CTL_CODE(FILE_DEVICE_HAL, 1007, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_ALLOC_FAILURE_INFO            CTL_CODE(FILE_DEVICE_HAL, 1008, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_FREE_FAILURE_INFO             CTL_CODE(FILE_DEVICE_HAL, 1009, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_CLEAR_FAILURE_INFO            CTL_CODE(FILE_DEVICE_HAL, 1010, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_SYNC_FAILURE_INFO             CTL_CODE(FILE_DEVICE_HAL, 1011, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_SET_PROMPT_POLICY             CTL_CODE(FILE_DEVICE_HAL, 1012, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SECLOAD_SET_PRIVILEGEDAPPS_POLICY     CTL_CODE(FILE_DEVICE_HAL, 1013, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// Function prototypes for functions exported from ossvcs.dll
//

// Indicates whether the platform supports two tier security or not. 
HRESULT LSM_IsTwoTierAvailable(BOOL* pfTwoTierAvailable);

// Adds a hash to the PEL.
HRESULT LSM_AddHashToPEL(const BYTE* pbyHash, DWORD cbyHash);

// Searches for a hash in the PEL
HRESULT LSM_IsHashInPEL(const BYTE* pbyHash, DWORD cbyHash);

// Builds a revocation list out of persistant storage
HRESULT
BuildRevocationList(HCERTHASHLIST* phRL);

// Stores the revocation list in persistant storage
HRESULT
StoreRevocationList(HCERTHASHLIST hRL);

// Transfers the Prompt Exclusion List from the registry into the 
// kernel loader security module.
HRESULT LSM_TransferPELToKernel();

//
// Function prototypes for functions exported from authcode.lib (which is linked into the OAL)
//

void 
LOADAUTH_Init();

BOOL 
IsSecModuleIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, 
                     LPVOID lpOutBuf, DWORD nOutBufSize, 
                     LPDWORD lpBytesReturned, BOOL *pRetVal);

//
// Function prototypes for functions exported from authcode_core.lib
//
void InitRevocationList();


#ifdef __cplusplus
};
#endif 
