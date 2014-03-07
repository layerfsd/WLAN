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
#ifndef LAP_H
#define LAP_H

#ifdef __cplusplus 
extern "C" { 
#endif 

#include <windows.h>
#include <lass.h>

//
// All Local Authentication Plugin (LAP) must  implement the following interface:
//


// Support masterkey protection sementics
#define LAP_CAPABILITIES_MASTER_KEY 0x1 

typedef struct _InitLap
{
    DWORD size; // Used for version info contains size of this struct.
                // _InitLap will only grow as new functionality added.
                // A LAP should work even if InitLap.size >= sizeof(InitLap)

                // However, a LAP should not work if InitLap.size < sizeof(InitLap)

    DWORD capabilities; // Expect LAP will fill in with the LAP_CAPABILITIES_* it supports.
} InitLap;

// Sent to a LAP when it is activated.
//  Allows LAP to indicate its capabilities.
BOOL InitLAP(
        InitLap *il
        );


// Sent to a LAP when it is being unloaded.
void DeinitLAP();


//Tells the lap to verify the user.
#if 0
// From lass.h
BOOL VerifyUser(const GUID *AEKey, /* Authenication Event Identifier */
                LPCWSTR pwszAEDisplayText, /*Text Plugin will display, if null use from registry.*/
                HWND   hWndParent, /*Parent Window if Available-else use desktop window*/
                DWORD  dwOptions, /*Bitmask of possible options.*/
                PVOID pExtended /*Reserved, must be 0*/
                );

#endif 

// Causes the LAP to enroll.
// App calls CreateEnrollmentConfigDialog and then LASS will call LAPCreateEnrollmentConfigDialog
BOOL LAPCreateEnrollmentConfigDialog(HWND hParentWindow,DWORD dwOptions);

// An application calling VerifyUser with the VU_UNTIL_SUCCESS flag, can result
// in several calls to VerifyUser in the LAP. To prevent UI flash,
// VerifyUserStart will be called before any sequence of VerifyUser
// calls are made into the LAP.  After all VerifyUser calls have been made a call to
// VerifyUserStop will be made.

//
//   APP       |       |    LASS            |       | LAP   |
// ------------+       +--------------------+       +-------+
// VerifyUser()| --->  |                    |       |       |
//             |       |  VerifyUserStart() | --->  |       |
//             |       |                    | <---  |       |
//             |       |                    |       |       |
//             |       |  VerifyUser()      | --->  |       |
//             |       |                    | <---  |       |
//             |       |                    |       |       |
//             |       |  VerifyUser()      | --->  |       |
//             |       |                    | <---  |       |
//             |       |                    |       |       |
//             |       |  VerifyUserStop()  | --->  |       |
//             |       |                    | <---  |       |
//             |       |                    |       |       |
//             | <---  |                    |       |       |       
//


// VerifyUserStart gets the same paramaters as the 
// VerifyUser call.
VOID  VerifyUserStart(const GUID *AEKey, /* Authenication Event Identifier */
                LPCWSTR pwszAEDisplayText, /*Text Plugin will display, if null use from registry.*/
                HWND   hWndParent, /*Parent Window if Available-else use desktop window*/
                DWORD  dwOptions, /*Bitmask of possible options.*/
                PVOID pExtended /*Reserved, must be 0*/
                );

VOID  VerifyUserStop();

// Application calls to VerifyUser block.  This can be an issue in the
// following scenario: 

// APP1 calls VerifyUser, LAP displays UI. APP2 runs and displays UI on top of
// the LAP's UI. APP2 then calls VerifyUser and blocks.  At this point APP2 is
// blocked, but the LAP's UI is behind the APP2 UI. 


// To mitigate this issue, when a VerifyUser call is in progress, and
// VerifyUser is called again, VerifyUserToTop is called, requesting the LAP
// bring its VerifyUser UI to the top of the z order.


//    APP1     |      |   APP2     |       |    LASS            |       | LAP   |
// ------------+      +------------+       +--------------------+       +-------+
// VerifyUser()| ----------------------->  |                    |       |       |
//             |      |            |       |  VerifyUserStart() | --->  |       |
//             |      |            |       |                    | <---  |       |
//             |      |            |       |                    |       |       |
//             |      |            |       |  VerifyUser()      | --->  |       |
//             |      |            |       |                    |       |       |
//             |      |VerifyUser()| --->  |                    |       |       |
//             |      |            |       |  VerifyUserToTop() |  ---> |       |
//             |      |            |       |                    | <---  |       |
//             |      |            |       |                    |       |       |
//             |      |            |       |  VerifyUserStop()  | --->  |       |
//             |      |            |       |                    | <---  |       |
//             |      |            |       |                    |       |       |
//             | <------------------------ |                    |       |       |
//             |      |            |       |                    |       |       |
//             |      |            |       |                    |       |       |

VOID  VerifyUserToTop();

// To ensure that a 'live' user is authenticating with the device the user
// can be periodically prompted to enter a codeword.  When LAPDisplayCodeword
// is called the  LAP should display a dialog prompting for pwszCodeword to be
// entered.

BOOL LAPDisplayCodeword(HWND hwndParent, LPCWSTR pwszCodeword);

// To ensure the user is aware on the next failed verification their device
// will be wiped, the user is notified.  When LAPDispalyPreWipe is called the
// LAP should display a dialog telling the user thier device will be wiped if
// there is one more failed auth.

BOOL LAPDisplayPreWipe(HWND hwndParent);

// To allow an application to cancel an existing call to VerifyUserAsync
// this API requests the LAP shutdown any of it's verification UI if possible. 
// This API maybe called anytime between VerifyUserStart and VerifyUserStop

BOOL LAPCancelVerifySequence();

// This API Is called when the user is required to be blocked from continuing
// for cSecondsToLockout seconds. The simplest implementation of this API is to
// sleep for cSeconds seconds. A more complex implementation of this API would
// display a dialog for cSeconds seconds, that informs the user how much time
// they are locked out for.

BOOL LAPLockoutUser(HWND hwnd, LPCWSTR , DWORD cSecondsToLockout);

#ifdef __cplusplus 
}
#endif 

#endif  // LAP_H
