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

Copyright (c) 2002 Microsoft Corporation

Module Name:

    paygshel.h

Abstract:

    Miscellaneous Shell API exposed in OAK only

--*/


#ifndef __PAYGSHEL_H__
#define __PAYGSHEL_H__

#ifdef __cplusplus
extern "C" {
#endif


// Device State Info
typedef struct _DEVICESTATEINFO {
    DWORD dwMask;               // In: Members to retrieve. This can be one of the DSIM_* values.
    DWORD dwState;              // Out: Returns device state. This can be one or more of DSIS_* values. 
    DWORD dwStateMask;          // In: Value that specifies which bits of the dwState member are be retrieved. This can can be one or more of DSIS_* values.  
} DEVICESTATEINFO;

#define DSIM_STATE              0x00000001          // Retrieves the dwState member.

#define DSIS_DEVICE_LOCKED      0x00000001          // Device is device locked.
#define DSIS_SIM_LOCKED         0x00000002          // SIM is locked and the device is displaying a SIM password dialog.
#define DSIS_KEY_LOCKED         0x00000004          // Device is key locked.
#define DSIS_IN_VOICE_CALL      0x00000008          // User of the device is in a voice call.
#define DSIS_IN_DATA_CALL       0x00000010          // User of device is in a data call       
#define DSIS_AT_HOME            0x00000020          // User of the device is at the Home screen. If the device is locked, the user is not considered to be at the home screen.

// Retrieves information about the current state of the device
HRESULT SHGetDeviceStateInfo(
    DEVICESTATEINFO * pdsi      // In/Out: Device State Information
);

HRESULT VerifyTrust(PBYTE pbyFile, DWORD cbyFile, DWORD dwReserved, LPCTSTR pszModuleName,  DWORD* pTrustLevel);
HRESULT VerifyFileTrust(LPCTSTR pszModuleName,  DWORD* pTrustLevel);

HRESULT DisplayNotRunnableAppDialog(LPCTSTR pszModuleName);

//////////////////////////////////////////////////////////////////////////////
//
// MakeNameNice
//
//   This function will Modify the input string such that if it contains any path 
//   or any file extension or any trailing space it will be removed.
//
//   This functionality will not work if input string conatain any of the string 
//   specified at registry key HKLM\SYSTEM\SHELL value's IgnoreAutoFormatStrings
//   which is a REG_MULTI_SZ value
//
// Parameters:
//  psz
//   [in/out] string which need to be changed for path or file extensions or trailing space.
//
// Return value:
//  void.
//
void MakeNameNice(LPTSTR psz);


#ifdef __cplusplus
}
#endif

#endif // __PAYGSHEL_H__
