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
#ifndef _TPCMGR_H_
#define _TPCMGR_H_


#include <windows.h>
#include <tapi.h>


// See the end of this file for an overview of TPCMgr


static const GUID IID_CSP_TPCMGR = { 0x11a2fb57, 0x7b12, 0x4d7d, { 0xa0, 0x3c, 0x38, 0x42, 0x45, 0xb1, 0x38, 0xed } };

// Window information
#define TPCMGR_WINDOW_NAME            TEXT("TAPI Manager")
#define TPCMGR_RESOURCE_MESSAGE_NAME  TEXT("TPCMGR resource message")


// Constants
#define MAX_OPERATOR_LENGTH                (64)
#define MAX_NUMBER_LENGTH                  (256)
#define MAX_NAME_LENGTH                    (64)
#define MAX_NUMBER_DTMF_DIGITS             (64)
#define INVALID_CALL_HANDLE                ((HCALL)0)
#define OFFERING_CALL_HANDLE               ((HCALL)1)
#define ALL_CALLS_HANDLE                   ((HCALL)2)
#define NEW_CALL_HANDLE                    ((HCALL)3)
#define INVALID_UNIQUE_CALL_ID             (0)
#define INVALID_ADDRESS_ID                 ((DWORD)-1)
#define DEFAULT_ADDRESS_ID                 (0)
#define MAX_DESCRIPTION_LENGTH             (256)


#define RESOURCE_PHONE_IN_USE_AMBIGUOUS     (0x00000001)
#define RESOURCE_PHONE_IDLE                 (0x00000002)

#define RESOURCE_PHONE_IN_USE_BY_TPCMGR     (0x00000004)
#define RESOURCE_PHONE_IDLE_BY_TPCMGR       (0x00000008)

#define RESOURCE_PHONE_IN_USE_BY_PTT        (0x00000010)   //Phone in use by media mode that can only be alien
#define RESOURCE_PHONE_IN_USE_BY_VT         (0x00000020)

#define RESOURCE_PHONE_IDLE_BY_ALIEN        (0x00000040)

#define RESOURCE_PHONE_IN_USE               (0x00000080)  //Phone in use by voice only

#define RESOURCE_DEFAULT_IN_USE             (0x00000000)
#define RESOURCE_GPRS_IN_USE                (0x10000000)




//use this to filter out only the resource type
#define FILTER_RESOURCE_TYPE_IN_USE(Status)  ((Status)&(0xf0000000))

//use this filter to get only if phone in use or idle, and what type of in use or idle
#define FILTER_PHONE_IN_USE_OR_IDLE(Status)  ((Status)&(0x0fffffff))

// Data types
typedef enum _tTPCCallState
    {
    CALL_STATE_UNKNOWN,                          // Call is in an indeterminate state
    CALL_STATE_AVAILABLE,                        // Incoming call available
    CALL_STATE_CALLING,                          // Attempting to setup a call
    CALL_STATE_TALKING,                          // In a voice call
    CALL_STATE_HOLD,                             // Call is on hold
    CALL_STATE_IDLE,                             // Call has been disconnected - TPCMgr has deallocated it and will not send any more information about it
    CALL_STATE_IDLE_BUSY,                        // Same as CALL_STATE_IDLE - Disconnect reason was that the other party was busy
    CALL_STATE_IDLE_NOANSWER,                    // Same as CALL_STATE_IDLE - Disconnect reason was that the other party did not answer
    CALL_STATE_IDLE_NODIALTONE,                  // Same as CALL_STATE_IDLE - Disconnect reason was network problems
    CALL_STATE_IDLE_UNREACHABLE,                 // Same as CALL_STATE_IDLE - Disconnect reason was network problems and/or the other party was unreachable
    CALL_STATE_IDLE_BADADDRESS,                  // Same as CALL_STATE_IDLE - Disconnect reason was that the number dialed was invalid
    CALL_STATE_IDLE_CANCELLED,                   // Same as CALL_STATE_IDLE - Disconnect reason was that the call was cancelled by the the user
    CALL_STATE_IDLE_PHONECONNECTIONFAILURE,      // Same as CALL_STATE_IDLE - Disconnect reason was that the phone connection failed
    CALL_STATE_IDLE_INVALIDSIMCARD,              // Same as CALL_STATE_IDLE - Disconnect reason was that the SIM card was invalid
    CALL_STATE_IDLE_SIMCARDBUSY,                 // Same as CALL_STATE_IDLE - Disconnect reason was that the SIM card was busy
    CALL_STATE_IDLE_NETWORKSERVICENOTAVAILABLE,  // Same as CALL_STATE_IDLE - Disconnect reason was that network service was not available
    CALL_STATE_IDLE_EMERGENCYONLY,               // Same as CALL_STATE_IDLE - Disconnect reason was that the radio is only handling emergency requests
    CALL_STATE_IDLE_ERROR,                       // Same as CALL_STATE_IDLE - Disconnect reason was that an error occurred
    CALL_STATE_IN_CONFERENCE,                    // Call is one member of a conference call
    CALL_STATE_IDLE_CONGESTION,                  // Same as CALL_STATE_IDLE - Disconnect reason was that the network is congested
    CALL_STATE_IDLE_TEMPFAILURE,                 // Same as CALL_STATE_IDLE - Disconnect reason was that the call was dropped
    CALL_STATE_IDLE_SIMCALLBLOCKED,              // Same as CALL_STATE_IDLE - Disconnect reason was that the call was blocked by USIM
    CALL_STATE_IDLE_SIMCALLMODIFIEDTOUSSD,       // Same as CALL_STATE_IDLE - Disconnect reason was that the call was modified by USIM to another calltype
    CALL_STATE_IDLE_DESTINATIONBARRED,           // Same as CALL_STATE_IDLE - Disconnect reason was that call was barred
    CALL_STATE_IDLE_FDNRESTRICT,                 // Same as CALL_STATE_IDLE - Disconnect reason was that call violates FDN rules
    CALL_STATE_NO_CHANGE,                        //internal state that is passed around to functions that update call info when the new info is not related to state
    } TPCCallState;

typedef enum _tTPCCallerIDState
    {
    CALLER_ID_STATE_UNAVAILABLE,  // Caller-ID information has not been provided by the network
    CALLER_ID_STATE_BLOCKED,      // Caller-ID information has been blocked by the caller
    CALLER_ID_STATE_VALID,        // Caller-ID information is valid
    CALLER_ID_STATE_AMBIGUOUS,    // Caller-ID information is ambiguous, we cannot trace the call id
    } TPCCallerIDState;

typedef enum _tTPCRegistrationState
    {
    REGISTRATION_STATE_UNREGISTERED,              // Not registered, not searching
    REGISTRATION_STATE_REGISTERING,               // Searching for a network
    REGISTRATION_STATE_REGISTERING_AFTER_DENIED,  // Searching for a network, last registration state was REGISTRATION_STATE_DENIED
    REGISTRATION_STATE_REGISTERED_HOME,           // Registered on home network
    REGISTRATION_STATE_REGISTERED_ROAM,           // Registered on a foreign network
    REGISTRATION_STATE_DENIED,                    // Registration denied
    } TPCRegistrationState;

typedef enum _tTPCDTMF_Duration
    {
    DTMF_DURATION_DEFAULT,      // Standard short tones
    DTMF_DURATION_LONG,         // Long tones
    } TPCDTMF_Duration;

// Call Modification Types
typedef enum _tTPCCallModificationType
    {
    LINECALLMODIFICATIONTYPE_UNKNOWN,            // Unknown
    LINECALLMODIFICATIONTYPE_BLOCKED,            // Blocked
    LINECALLMODIFICATIONTYPE_MODIFIED,           // Modified
    } TPCCallModificationType;

// Call Types (defined as in RIL_CALLTYPE Ril.h)
typedef enum _tTPCCallType
    {
    LINECALLTYPE_UNKNOWN,            // Unknown
    LINECALLTYPE_VOICE,              // Voice call
    LINECALLTYPE_DATA,               // Data call
    LINECALLTYPE_FAX,                // Fax call
    LINECALLTYPE_PTT,                // PTT call
    LINECALLTYPE_VT,                 // VT call
    LINECALLTYPE_USSD,               // USSD call
    LINECALLTYPE_LAST,               // Last call
    } TPCCallType;

// Alpha Identifier Type
typedef enum _tTPCAlphaIdentifierType
    {
    LINE_ALPHAIDENTIFIERTYPE_UNKNOWN,   // Unknown
    LINE_ALPHAIDENTIFIERTYPE_PRESENT,   // Present
    LINE_ALPHAIDENTIFIERTYPE_NOTPRESENT,// Not Present
    } TPCAlphaIdentifierType;


// Structures
struct TPC_CALL_INFORMATION
    {
    DWORD dwValidFields;                 // Bit-mask indicating which fields of the structure are valid - Always valid
    DWORD dwChangedFields;               // Bit-mask indicating which fields of the structure have changed since the last update - Always valid
    HCALL hCall;                         // TAPI call handle for this call - Always valid (but possibly INVALID_CALL_HANDLE for
                                         //   CALL_STATE_AVAILABLE calls)
    LONG lUniqueCallID;                  // Unique ID that can be used to track a particular call (useful for incoming calls where
                                         //   hCall is initially OFFERING_CALL_HANDLE) - Always valid
    LONG lUniqueCallIDConferenceParent;  // Unique ID (see lUniqueCallID) of the conference call (if any) of which this call is a member
                                         //   If (lUniqueCallIDConferenceParent == lUniqueCallID), this call IS the conference call handle
                                         //   Always valid (but possibly INVALID_UNIQUE_CALL_ID for calls that are not a member of a conference)
    TPCCallState csCallState;            // Current call state
    DWORD dwAddressID;                   // Address identifier of the call (for hardware that supports multiple phone numbers per device)
    TPCCallerIDState cidsCallerIDState;  // Availability of caller-ID information (see the next two fields)
    TCHAR ptsNumber[MAX_NUMBER_LENGTH];  // Phone number to which the call is connected
    TCHAR ptsFullNumber[MAX_NAME_LENGTH];// Full original phone number dialed by the user
    TCHAR ptsName[MAX_NAME_LENGTH];      // Name of the entity to which the call is connected
    DWORD dwReserved;                    // This variable is used internally by TPCMgr and should NOT be read or written by the application
    };
#define CALL_STATE_FIELD       (1)
#define ADDRESS_ID_FIELD       (2)
#define CALLER_ID_STATE_FIELD  (4)
#define NUMBER_FIELD           (8)
#define NAME_FIELD             (16)
#define NUMBER_FULL_FIELD      (32)

// For ease of use, the following structure exposes an array of TPC_CALL_INFORMATION
// structures.  That array is of variable size (indicated by the iNumberCalls variable),
// so we create a structure that can have any number of TPC_CALL_INFORMATION structures.
// Unfortunately, this causes compiler warning C4200 because of the zero-sized array in
// the structure.  Using #pragma below saves the user from seeing that warning.
#pragma warning(disable:4200)
struct TPC_PHONE_CALL_STATUS
    {
    DWORD dwUserData;                     // Caller-specific DWORD passed into the registration function - Always valid
    unsigned int iNumberCalls;            // Number of calls in the ptpcciCalls array
    TPC_CALL_INFORMATION ptpcciCalls[0];  // Array of call-information structures
    };
#pragma warning(default:4200)

struct TPC_CALL_MODIFICATION_INFO
    {
    HCALL hCall;                                    // Handle of call to be modified
    TPCCallModificationType dwCallModificationType; // Modification type for this call
    TPCCallType dwCallType;                         // Type of call 
    TCHAR ptsNewNumber[MAX_NUMBER_LENGTH];          // New Phone number\USSD string to which the call has to be modified
    TPCAlphaIdentifierType dwAlphaIdentifierType;   // Type of alpha identifier
    TCHAR ptsAlphaIdentifierReason[MAX_DESCRIPTION_LENGTH]; // Alpha Identifier reason
    };

struct TPC_REGISTRATION_INFORMATION
    {
    DWORD dwValidFields;                         // Bit-mask indicating which fields of the structure are valid - Always valid
    DWORD dwUserData;                            // Caller-specific DWORD passed into the registration function - Always valid
    TPCRegistrationState rsRegistrationState;    // Network registration state
    TCHAR ptsOperatorName[MAX_OPERATOR_LENGTH];  // Name of the operator on which we are currently registered
    };
#define REGISTRATION_STATE_FIELD  (1)
#define OPERATOR_NAME_FIELD       (2)

struct TPC_REQUEST_RESULT
    {
    DWORD dwRequestID;  // Identifier of the TPC_REQ request for which hrResult applies
                        //   The dwRequestID value is returned as an HRESULT by the call to ConnMgrProviderMessage which generated
                        //   the TPC_REQ request.  Note that a FAILURE() result from the call to ConnMgrProviderMessage will NOT
                        //   result in a TPC_REQUEST_RESULT being generated for that request.
    HRESULT hrResult;   // Success/failure result of the specific request
    };

struct TPC_DIAL_DATA
    {
    HCALL hCall;                         // Handle of call on which to dial
                                         //   (note that the call handle can be one of the special values defined above)
    DWORD dwAddressID;                   // Address ID to use for the outgoing call (potentially DEFAULT_ADDRESS_ID)
    TCHAR ptsNumber[MAX_NUMBER_LENGTH];  // Number to dial
    TCHAR ptsFullNumber[MAX_NUMBER_LENGTH]; // Full Number, not to dial but before parsing
    TCHAR ptsName[MAX_NAME_LENGTH];      // Name corresponding to the number to dial
    DWORD dwOptions;                     // One or more of the DIAL_OPTION_* constants
                                         //   (note that not all combinations of options make sense)
    };
#define DIAL_OPTION_NONE               (0x00000000)
#define DIAL_OPTION_BLOCK_CALLER_ID    (0x00000001)
#define DIAL_OPTION_PRESENT_CALLER_ID  (0x00000002)

struct TPC_CALL_HANDLE
    {
    HCALL hCall;  // Handle of call to act upon
    };

struct TPC_SWAPHOLD_DATA
    {
    HCALL hCallActive;  // Handle of active call
    HCALL hCallHold;    // Handle of call on hold
    };

struct TPC_ADD_TO_CONFERENCE_DATA
    {
    HCALL hCallConference;  // Handle of conference call
    HCALL hCallAdd;         // Handle of call to add to conference
    };

struct DTMF_TONE_DATA
    {
    HCALL hCall;                                  // Handle of call on which to play tones
    TCHAR ptsDTMFDigits[MAX_NUMBER_DTMF_DIGITS];  // Sequence of DTMF digits
    };

struct TPC_MANAGE_CALLS_DATA
    {
    DWORD dwCommand;  // One of the LINECALLCMD_ constants defined in ExTAPIpriv.h
    DWORD dwID;       // Module call ID of the call to manage
    };

// Message types (from TPCMgr to the registered UI handler)
//    These messages are sent with a WM_COPYDATA message, so behave accordingly
//      (see MSDN documentation for this message for details).
//    IMPORTANT: Note that because TPCMgr is sending WM_COPYDATA messages to the
//    client application, client applications MUST regularly be processing window
//    messages in their WndProc message pump.  Otherwise TPCMgr's calls to SendMessage
//    will block waiting for the client application to process its WM_COPYDATA message
//    and TPCMgr will be unable to function properly.
//    The dwData parameter of the COPYDATASTRUCT will contain the message identifier.
//    The format of lpData is defined by each message.

#define TPC_MSG_REGISTRATION_STATE_CHANGE          (4000)
//      Purpose: Inform the UI of the new registration state
//         Data: TPC_REGISTRATION_INFORMATION structure
// Return value: None

#define TPC_MSG_PHONE_CALL_STATUS_CHANGE           (4001)
//      Purpose: Inform the UI of the new state for all calls on the phone
//         Data: TPC_PHONE_CALL_STATUS structure
// Return value: None

#define TPC_MSG_CALL_AVAILABLE                     (4002)
//      Purpose: Inform the UI that an incoming call is available
//         Data: None
// Return value: None

#define TPC_MSG_CALL_NO_LONGER_AVAILABLE           (4003)
//      Purpose: Inform the UI that no incoming call is currently available
//         Data: None
// Return value: None

#define TPC_MSG_EQUIPMENT_STATE_CHANGE             (4004)
//      Purpose: Inform the UI of the new registration state
//         Data: DWORD containing one of the LINEEQUIPSTATE_* constants defined in
//               ExTAPI.h and indicating the new equipment state
// Return value: None

#define TPC_MSG_REQUEST_RESULT                     (4005)
//      Purpose: Inform a client about the result of a previous request
//         Data: TPC_REQUEST_RESULT structure
// Return value: None
//        Notes: TPC_MSG_REQUEST_RESULT is sent to the window identified by the hwnd
//               parameter in the call to ConnMgrProviderMessage.  This does NOT
//               need to be the same window as the registered UI handler.

#define TPC_MSG_PHONE_CALL_MODIFICATION_CHANGE     (4006)
//      Purpose: Inform the UI that the USIM call control changed the call info
//         Data: TPC_CALL_MODIFICATION_INFO structure
// Return value: None

// Request types (from the UI handler to TPCMgr)
//    Requests MUST be sent via Connection Manager (#include <connmgr.h>) as follows:
//    HRESULT ConnMgrProviderMessage(
//                                  NULL,
//                                  &IID_CSP_TPCMGR,
//                                  dwRequestType,   // One of the TPC_REQ_* types below
//                                  (DWORD)hwnd,     // Window handle of the UI handler (optional for
//                                                      all but TPC_REQ_REGISTER_UI_HANDLER,
//                                                      TPC_REQ_UNREGISTER_UI_HANDLER,
//                                                      TPC_REQ_REGISTER_RESOURCE_NOTIFICATIONS, and
//                                                      TPC_REQ_UNREGISTER_RESOURCE_NOTIFICATIONS -
//                                                      use NULL if you are not interested in
//                                                      receiving TPC_MSG_REQUEST_RESULT messages)
//                                  pbData,          // TPC_REQ-specific data as defined below
//                                  dwDataSize       // Number of bytes of data pointed to by pbData
//                                  );
//    If ConnMgrProviderMessage returns a value that satisfies the SUCCESS() macro for HRESULTs,
//    its return value can be cast to a DWORD dwRequestID and a TPC_MSG_REQUEST_RESULT will eventually
//    be sent to the window identified by hwnd to indicate the success or failure of that call.  The
//    dwRequestID in the TPC_REQUEST_RESULT structure can be used by an application to determine to
//    which of many potential requests a particular TPC_MSG_REQUEST_RESULT applies.


#define TPC_REQ_REGISTER_UI_HANDLER                (5000)
//      Purpose: Registers the sending window as a UI handler with TPCMgr
//         Data: DWORD dwUserData - This DWORD will be passed with all callback messages
// Return value: HRESULT

#define TPC_REQ_UNREGISTER_UI_HANDLER              (5001)
//      Purpose: Unregisters the sending window as a UI handler with TPCMgr
//         Data: None
// Return value: HRESULT

#define TPC_REQ_DIAL_VOICE                         (5002)
//      Purpose: Request for TPCMgr to initiate a voice call
//         Data: TPC_DIAL_DATA structure (if TPC_DIAL_DATA.hCall is NEW_CALL_HANDLE, a new call
//               will be created and used to dial)
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_ANSWER_VOICE                       (5003)
//      Purpose: Request TPCMgr to answer an incoming call
//         Data: None
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_HANG_UP                            (5004)
//      Purpose: Request TPCMgr to hang-up a call
//         Data: TPC_CALL_HANDLE structure (if TPC_CALL_HANDLE.hCall is set to ALL_CALLS_HANDLE,
//               all calls will be hung-up; if set to OFFERING_CALL_HANDLE, the offering call (if
//               present) will be rejected)
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_PLAY_DTMF_TONES                    (5005)
//      Purpose: Request TPCMgr to play DTMF tones on a call
//         Data: DTMF_TONE_DATA structure
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()
//        Notes: DTMF digits must come from the set (['0' .. '9'] ['A' .. 'D'] '*' '#' ',')

#define TPC_REQ_HOLD                               (5006)
//      Purpose: Request TPCMgr to put a call on hold
//         Data: TPC_CALL_HANDLE structure
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_UNHOLD                             (5007)
//      Purpose: Request TPCMgr to take a call off hold
//         Data: TPC_CALL_HANDLE structure
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_SWAPHOLD                           (5008)
//      Purpose: Request TPCMgr to swap an active call with a call on hold
//         Data: TPC_SWAPHOLD_DATA structure
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_CONFERENCE_AND_CONSULT             (5009)
//      Purpose: Request TPCMgr to place a call in a conference and create a consultation call handle
//         Data: TPC_CALL_HANDLE structure
//               If a conference call already exists, provide the conference call's handle
//               Otherwise, provide the call handle of the call you want placed in the new conference call
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_ADD_TO_CONFERENCE                  (5010)
//      Purpose: Request TPCMgr to add a call to the a conference-call
//         Data: TPC_ADD_TO_CONFERENCE_DATA structure
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_REMOVE_FROM_CONFERENCE             (5011)
//      Purpose: Request TPCMgr to remove a call from the conference-call of which it is a member
//         Data: TPC_CALL_HANDLE structure
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_GET_SIGNAL_STRENGTH                (5012)
//      Purpose: Request TPCMgr to return signal-strength information
//         Data: None
// Return value: A value in the range [0x0000 .. 0xffff] representing signal-strength
//        Notes: 0xffff represents the strongest signal, 0x0000 the weakest

#define TPC_REQ_MUTE_PHONE                         (5013)
//      Purpose: Request TPCMgr to mute the phone (disable the microphone)
//         Data: None
// Return value: HRESULT

#define TPC_REQ_UNMUTE_PHONE                       (5014)
//      Purpose: Request TPCMgr to un-mute the phone (enable the microphone)
//         Data: None
// Return value: HRESULT

#define TPC_REQ_IS_PHONE_MUTED                     (5015)
//      Purpose: Request TPCMgr to retrieve the mute state of the phone microphone
//         Data: None
// Return value: HRESULT - S_OK if the phone is muted, S_FALSE if it is not muted

#define TPC_REQ_MANAGE_CALLS                       (5016)
//      Purpose: Request TPCMgr to call ExTAPI's lineManageCalls API
//         Data: TPC_MANAGE_CALLS_DATA structure
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_REDIRECT                           (5017)
//      Purpose: Request TPCMgr to call TAPI's lineRedirect API
//         Data: TPC_DIAL_DATA structure
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_SEND_FLASH                         (5018)
//      Purpose: Request TPCMgr to send a hookflash
//         Data: DTMF_TONE_DATA structure, representing the payload, if any
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS()

#define TPC_REQ_SET_DTMF_DURATION                  (5019)
//      Purpose: Causes TPCMgr to update the DTMF duration
//         Data: TPCDTMF_Duration
// Return value: HRESULT

#define TPC_REQ_IS_PHONE_MUTED_ASYNC               (5020)
//      Purpose: Request TPCMgr to retrieve the mute state of the phone microphone
//         Data: None
// Return value: Contained in TPC_MSG_REQUEST_RESULT message if ConnMgrProviderMessage returns SUCCESS() 

#define TPC_REQ_REGISTER_RESOURCE_NOTIFICATIONS    (6000)
//      Purpose: Registers the sending window with TPCMgr for resource notifications
//         Data: None
// Return value: HRESULT

#define TPC_REQ_UNREGISTER_RESOURCE_NOTIFICATIONS  (6001)
//      Purpose: Unregisters the sending window with TPCMgr for resource notifications
//         Data: None
// Return value: HRESULT


// Resource notifications
//    If registered with TPCMgr (via TPC_REQ_REGISTER_RESOURCE_NOTIFICATIONS), an application
//    will receive window messages that inform it of changing resource availability information.
//    The ID of the window message that will be sent by TPCMgr to the application can be
//    obtained with the following code:
//       UINT uiResourceMessage = RegisterWindowMessage(TPCMGR_RESOURCE_MESSAGE_NAME);
//    Resource messages will have the following properties:
//       uMsg   = uiResourceMessage
//       wParam = [UNUSED - ignore]
//       lParam = RESOURCE_PHONE_IN_USE or RESOURCE_PHONE_IDLE
//    Resource message meanings:
//       * RESOURCE_PHONE_IN_USE means that one or more active calls are ongoing and that the
//         phone equipment can not be used for other purposes (i.e., making a data call)
//       * RESOURCE_PHONE_IN_USE_BY_TPCMGR means the same as RESOURCE_PHONE_IN_USE and indicates
//         that TPCMgr is responsible for the resource use
//       * RESOURCE_PHONE_IDLE means that the phone is not currently being used
//       * RESOURCE_PHONE_IDLE_BY_TPCMGR means that the phone is not currently being used by TPCMgr
//    Note that when a call is incoming (the phone is ringing), TPCMgr will report the
//       RESOURCE_PHONE_IN_USE states.
//    Note that RESOURCE_PHONE_IN_USE_BY_TPCMGR will always be accompanied by a RESOURCE_PHONE_IN_USE
//       message, but that the opposite is not necessarily true (i.e., when another application is
//       using the phone).


// TPCMgr overview

/*

New calls can be created by the following events:
 Event                          | Causes...
--------------------------------+----------
 TPC_REQ_DIAL_VOICE             | New call in CALL_STATE_CALLING state
 TPC_REQ_CONFERENCE_AND_CONSULT | New conference call (lUniqueCallIDConferenceParent == lUniqueCallID)
                                | Possible transitions to CALL_STATE_IN_CONFERENCE for existing calls
 [Incoming call arrives]        | New call in CALL_STATE_AVAILABLE state (with OFFERING_CALL_HANDLE call handle)

State changes are restricted to the following:
 State                    | Transitions into...      | When...
--------------------------+--------------------------+--------
 CALL_STATE_AVAILABLE     | CALL_STATE_TALKING       | [TPC_REQ_ANSWER_VOICE succeeds]
                          | CALL_STATE_IDLE*         | TPC_REQ_HANG_UP succeeds
                          |                          | [Remote side hangs up]
 CALL_STATE_CALLING       | CALL_STATE_TALKING       | [Remote side answers ringing phone]
                          | CALL_STATE_IDLE*         | TPC_REQ_HANG_UP succeeds
                          |                          | [Remote side hangs up]
 CALL_STATE_TALKING       | CALL_STATE_HOLD          | TPC_REQ_HOLD or TPC_REQ_SWAPHOLD succeeds
                          | CALL_STATE_IN_CONFERENCE | TPC_REQ_CONFERENCE_AND_CONSULT succeeds
                          |                          | TPC_REQ_ADD_TO_CONFERENCE succeeds
                          | CALL_STATE_IDLE*         | TPC_REQ_HANG_UP succeeds
                          |                          | [Remote side hangs up]
 CALL_STATE_HOLD          | CALL_STATE_TALKING       | TPC_REQ_UNHOLD or TPC_REQ_SWAPHOLD succeeds
                          | CALL_STATE_IN_CONFERENCE | TPC_REQ_CONFERENCE_AND_CONSULT succeeds
                          |                          | TPC_REQ_ADD_TO_CONFERENCE succeeds
                          | CALL_STATE_IDLE*         | TPC_REQ_HANG_UP succeeds
                          |                          | [Remote side hangs up]
 CALL_STATE_IN_CONFERENCE | CALL_STATE_IDLE*         | [Remote side hangs up]
                          |                          | TPC_REQ_ANSWER_VOICE succeeds

*/


#endif // _TPCMGR_H_
