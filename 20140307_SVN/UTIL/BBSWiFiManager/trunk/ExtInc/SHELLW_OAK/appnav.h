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

Copyright (c) 2004 Microsoft Corporation

Module Name:

    appnav.h

Abstract:

    Include file for Windows CE app navigation functions

--*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif




// Launch Calendar and display the new appointment window.
// If a compose form is already opened, then it is displayed. Confirmation dialogs should
// be dismissed with the equivalent of SK2.Cancel. 
// return values: S_OK for success, HRESULT error code on failure
HRESULT CalendarNewAppointment();

// Launch Contacts and display the new contacts window.
// If the Contacts app is already in a compose form or other dialog, then that dialog should 
// be dismissed first and edits saved. This is the equivalent of if the use had started 
// composing a new contact then went to the HomeScreen and then returned to Contacts. 
// return values: S_OK for success, HRESULT error code on failure
HRESULT ContactsNewContact();

#ifdef __cplusplus
}
#endif
