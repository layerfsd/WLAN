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

#ifndef LASS_H
#define LASS_H
#include <windows.h>

#ifdef __cplusplus 
extern "C" { 
#endif 

//Acronyms:
// (LASS)- Local Authentication Sub System
// (LAP) Local Authentication Plugin



//
//  Config Info
//  Per Authentication Event.
// AEFrequencyType
//

#define FT_Every_FV 0  // Every  FrequencyValue Time. set to 0 to validate every time.
#define FT_Minutes_Idle  1// Minites idle -- Not Yet Supported
#define FT_Minutes_Since_Any_AE_Verified  2 // Minites since Any AE returned from VerifyUser Succesfully
#define FT_Minutes_Since_This_AE_Verified 3 // Minites since This AE returned from VerifyUser Succesfully.


typedef struct  _AuthenticationEvent 
{
    GUID AEKey; /*Key to make all entries unique.*/
    LPWSTR Name; /*Friendly Name*/
    int  FrequencyType; /*Tells us how to interpret FrequencyValue*/
    int  FrequencyValue;   /* Used with FrequencyType */
} AuthenticationEvent;


//
//
// Some authentication Events are pre-defined - and should have special handlers in 
// The LAPs 
// Examples: 
//  



//
//  LASS Exports: 
//




// VerifyUser
// 
//  returns
//     TRUE ->  User Verified.
//     FALSE -> User not verified. GetLastError contains error.
// GetLastError () returns 
//  ERROR_CANCELLED <= The user cancelled the verification Request.
//  ERROR_WRONG_PASSWORD <= Generic code for user failed authentication. If more applicable 
                        //  code the LAP  will set it.

// Flags for dwOptions feild in Verify User.
#define VU_NO_UI  0x1      /*Return true only if a call to the LAP isn't necassary.*/
#define VU_UNTIL_SUCCESS  0x2 /* Verify User will return only if user clikced cancel, or got password correct.*/
#define VU_HANDLE_MASTER_KEY  0x4 /* Handle MasterKey Encryption/Decryption if possible. */
#define VU_EARLY_BOOT  0x8 /* Perform VerifyUser in EARLY_BOOT environment */

// These flags are ignored if used by an untrusted callers.
#define VU_TRUSTED_CALLER_FLAGS (VU_EARLY_BOOT | VU_HANDLE_MASTER_KEY)

BOOL VerifyUser(const GUID *AEKey, /* Authenication Event Identifier */
                LPCWSTR wszAEDisplayText, /*Text Plugin will display, if null use from registry.*/
                HWND   hWndParent, /*Parent Window if Available-else use desktop window*/
                DWORD  dwOptions, /*Bitmask of possible options.*/
                PVOID pExtended /*Reserved, must be 0*/
                );

//
// Example 1: 
//     An app that wished to authenticate the user before calling a Secure Function.
//
// APP A:
// HWND hMyWindow;
// VerifyAndCallSecureFunction () 
// {
//    DWORD LastError;
//    GUID AEKeyForFoo = ...;
//
//    if (VerifyUser(&AEKeyForFoo,"App A",hMyWindow,VU_UNTIL_SUCCESS,0))  //  Call into LASS; This will  
//          CallSecureFunction()                //  call into the active LAP 
//                                              //  and show LAP specific UI.
//     else 
//          TellUserVerificationFailed(GetLastError()));  // <- display your own UI
// }
//
//


// Must be trusted to call this.
// Causes the LASS to re-read its configuration settings.
BOOL  LASSReloadConfig();

// Causes the LAP to enroll.
// Must be trusted to call this.
BOOL CreateEnrollmentConfigDialog(HWND hParentWindow);

// This function provides a method to query the LASS for values. 
BOOL LASSGetValue(DWORD ValueId,PVOID lpvOutBuffer, DWORD cbOutBuffer, DWORD* pcbReturned);

typedef HANDLE HLASS;

// This function starts an Async Verify User call.
HLASS VerifyUserAsync(const GUID *AEKey, /* Authenication Event Identifier */
                LPCWSTR wszAEDisplayText, /*Text Plugin will display, if null use from registry.*/
                HWND   hWndParent, /*Parent Window if Available-else use desktop window*/
                DWORD  dwOptions, /*Bitmask of possible options.*/
                PVOID pExtended /*Reserved, must be 0*/
                );

// This function picks up the results of an asyncronous API.
BOOL LASSGetResult(HLASS h);

// This function close an HLASS.
BOOL LASSClose(HLASS h);


// Valid values are: 
#define  LASS_VALUE_DEVICE_WIPE_THRESHOLD	 1 
//   Returns the device wipe threshold. Returns 0 if there is no device wipe
//   threshold set.	

#define  LASS_VALUE_AUTH_FAILURE_COUNT	  2
//   Returns the number of authentication failures.  If there have been no
//   failures This will return 0.



//
// This is a named event an application can wait on to 
// be notified when LASS has started. Before this event 
// is set, all LASS calls will fail.
// 

#define LASS_SRV_STARTED_EVENT_NAME L"LASS_SRV_STARTED"


// Paths used during installation - may move to another location.
//////////////////////////////////////////////////////////////////////////////
//  LAP Installation Information. - move to lap.h
//////////////////////////////////////////////////////////////////////////////
// Paths used during installation - may move to another location.
#define REG_LAP_ROOT L"Comm\\Security\\LASSD\\LAP"
#define REG_LOCKOUT_ROOT L"Comm\\Security\\LASSD\\LockOut"
#define REG_AE_ROOT L"Comm\\Security\\LASSD\\AE"
#define REG_LAP_ACTIVE_VALUE L"ActiveLap"

#define REG_AE_FRIENDLY_NAME_VALUE L"FriendlyName"
#define REG_AE_DISPLAY_TEXT_VALUE  L"DisplayText"
#define REG_AE_AUTH_FREQ_VALUE L"AEFrequencyValue"
#define REG_AE_AUTH_FREQ_TYPE L"AEFrequencyType"


#define REG_LASS_POLICY_ROOT L"Comm\\Security\\LASSD"
// Policy settings found under REG_LASS_POLICY_ROOT

#define REG_CODE_WORD L"CodeWord"
#define REG_CODE_WORD_FREQUENCY L"CodeWordFrequency"
#define REG_DEVICE_WIPE_THRESHOLD L"DeviceWipeThreshold"

#define REG_LOCKOUT_THRESHOLD L"Threshold"
#define REG_LOCKOUT_INITIAL_PENALTY L"InitialPenalty"
#define REG_LOCKOUT_INCRIMENTAL_PENALTY  L"IncrementalPenalty"

#ifdef __cplusplus 
}
#endif 

#endif //#ifndef LASS_H

