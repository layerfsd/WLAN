//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//
// DESCRIPTION:
//     This file defines the Management Transition API for Device Management.
//     This API determines if the Management can be changed on the device or
//     not.

#ifndef DEVICE_MANAGEMENT_APIS_H
#define DEVICE_MANAGEMENT_APIS_H

#ifdef __cplusplus
extern "C" {
#endif

/// <summary>
///     This API is used to determine if a device is allowed to change
///     Management.
/// </summary>
/// <parm name="pfTransitionEnabled">
///     [out] TRUE if allowed to transition, FALSE otherwise.
/// </parm>
/// <returns>
///     HRESULT indicating success or failure. Caller should check the HRESULT
///     and confirm that it is S_OK before using value in pfTransitionEnabled
/// </returns>
HRESULT IsManagementTransitionEnabled(__out BOOL* pfTransitionEnabled);

#ifdef __cplusplus
}
#endif

#endif  /* DEVICE_MANAGEMENT_APIS_H */
