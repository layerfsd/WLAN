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
#pragma once 
#include <Ril.h>

#ifdef _INCLUDE_RILVERSIONEX_
#include <rilversionex.h>
#endif

//
// Macro for constructing event IDs
//
#define EL_MAKE_EVENTID(id, sev)        ((((DWORD)(sev)) << 29) | ((DWORD)(id) & EL_EVENTID_MASK))



// API IDs   (This should match the table defined in the RIL driver)
enum APIID {
    APIID_NONE                          = -1,
    APIID_GETSUBSCRIBERNUMBERS          = 0,
    APIID_GETOPERATORLIST,              // 1
    APIID_GETPREFERREDOPERATORLIST,     // 2
    APIID_ADDPREFERREDOPERATOR,         // 3
    APIID_REMOVEPREFERREDOPERATOR,      // 4
    APIID_GETCURRENTOPERATOR,           // 5
    APIID_REGISTERONNETWORK,            // 6
    APIID_UNREGISTERFROMNETWORK,        // 7
    APIID_GETREGISTRATIONSTATUS,        // 8
    APIID_GETCALLERIDSETTINGS,          // 9
    APIID_SETCALLERIDSTATUS,            // 10
    APIID_GETHIDEIDSETTINGS,            // 11
    APIID_SETHIDEIDSTATUS,              // 12
    APIID_GETDIALEDIDSETTINGS,          // 13
    APIID_SETDIALEDIDSTATUS,            // 14
    APIID_GETCLOSEDGROUPSETTINGS,       // 15
    APIID_SETCLOSEDGROUPSETTINGS,       // 16
    APIID_GETCALLFORWARDINGSETTINGS,    // 17
    APIID_ADDCALLFORWARDING,            // 18
    APIID_REMOVECALLFORWARDING,         // 19
    APIID_SETCALLFORWARDINGSTATUS,      // 20
    APIID_GETCALLWAITINGSETTINGS,       // 21
    APIID_SETCALLWAITINGSTATUS,         // 22
    APIID_SENDSUPSERVICEDATA,           // 23
    APIID_CANCELSUPSERVICEDATASESSION,  // 24
    APIID_DIAL,                         // 25
    APIID_ANSWER,                       // 26
    APIID_HANGUP,                       // 27
    APIID_SENDDTMF,                     // 28
    APIID_SETDTMFMONITORING,            // 29
    APIID_GETCALLLIST,                  // 30
    APIID_MANAGECALLS,                  // 31
    APIID_TRANSFERCALL,                 // 32
    APIID_GETLINESTATUS,                // 33
    APIID_GETAUDIOGAIN,                 // 34
    APIID_SETAUDIOGAIN,                 // 35
    APIID_GETAUDIODEVICES,              // 36
    APIID_SETAUDIODEVICES,              // 37
    APIID_GETAUDIOMUTING,               // 38
    APIID_SETAUDIOMUTING,               // 39
    APIID_GETHSCSDOPTIONS,              // 40
    APIID_SETHSCSDOPTIONS,              // 41
    APIID_GETHSCSDCALLSETTINGS,         // 42
    APIID_GETDATACOMPRESSION,           // 43
    APIID_SETDATACOMPRESSION,           // 44
    APIID_GETERRORCORRECTION,           // 45
    APIID_SETERRORCORRECTION,           // 46
    APIID_GETBEARERSERVICEOPTIONS,      // 47
    APIID_SETBEARERSERVICEOPTIONS,      // 48
    APIID_GETRLPOPTIONS,                // 49
    APIID_SETRLPOPTIONS,                // 50
    APIID_GETMSGSERVICEOPTIONS,         // 51
    APIID_SETMSGSERVICEOPTIONS,         // 52
    APIID_GETMSGCONFIG,                 // 53
    APIID_SETMSGCONFIG,                 // 54
    APIID_RESTOREMSGCONFIG,             // 55
    APIID_SAVEMSGCONFIG,                // 56
    APIID_READMSG,                      // 57
    APIID_DELETEMSG,                    // 58
    APIID_WRITEMSG,                     // 59
    APIID_SENDMSG,                      // 60
    APIID_SENDSTOREDMSG,                // 61
    APIID_SENDMSGACKNOWLEDGEMENT,       // 62
    APIID_GETUSERIDENTITY,              // 63
    APIID_GETPHONELOCKEDSTATE,          // 64
    APIID_UNLOCKPHONE,                  // 65
    APIID_GETLOCKINGSTATUS,             // 66
    APIID_SETLOCKINGSTATUS,             // 67
    APIID_CHANGELOCKINGPASSWORD,        // 68
    APIID_GETCALLBARRINGSTATUS,         // 69
    APIID_SETCALLBARRINGSTATUS,         // 70
    APIID_CHANGECALLBARRINGPASSWORD,    // 71
    APIID_GETEQUIPMENTINFO,             // 72
    APIID_GETEQUIPMENTSTATE,            // 73
    APIID_SETEQUIPMENTSTATE,            // 74
    APIID_GETPHONEBOOKOPTIONS,          // 75
    APIID_SETPHONEBOOKOPTIONS,          // 76
    APIID_READPHONEBOOKENTRIES,         // 77
    APIID_WRITEPHONEBOOKENTRY,          // 78
    APIID_DELETEPHONEBOOKENTRY,         // 79
    APIID_SENDSIMCMD,                   // 80
    APIID_SENDRESTRICTEDSIMCMD,         // 81
    APIID_GETSIMRECORDSTATUS,           // 82
    APIID_GETSIMTOOLKITPROFILE,         // 83
    APIID_SETSIMTOOLKITPROFILE,         // 84
    APIID_SENDSIMTOOLKITENVELOPECMD,    // 85
    APIID_FETCHSIMTOOLKITCMD,           // 86
    APIID_SENDSIMTOOLKITCMDRESPONSE,    // 87
    APIID_TERMINATESIMTOOLKITSESSION,   // 88
    APIID_GETCOSTINFO,                  // 89
    APIID_SETCOSTINFO,                  // 90
    APIID_GETSIGNALQUALITY,             // 91
    APIID_GETCELLTOWERINFO,             // 92
    APIID_DEVSPECIFIC,                  // 93
    APIID_GETDEVCAPS,                   // 94
    APIID_GETHIDECONNECTEDIDSETTINGS,   // 95
    APIID_SETHIDECONNECTEDIDSTATUS,     // 96
    APIID_GETUUSREQUIREDSTATUS,         // 97
    APIID_CLEARCCBSREGISTRATION,        // 98
    APIID_GETSYSTEMTIME,                    // 99
    APIID_GETGPRSCONTEXTLIST,               // 100
    APIID_SETGPRSCONTEXT,                   // 101
    APIID_DELETEGPRSCONTEXT,                // 102
    APIID_GETREQUESTEDQUALITYOFSERVICELIST, // 103
    APIID_SETREQUESTEDQUALITYOFSERVICE,     // 104
    APIID_DELETEREQUESTEDQUALITYOFSERVICE,  // 105
    APIID_GETMINIMUMQUALITYOFSERVICELIST,   // 106
    APIID_SETMINIMUMQUALITYOFSERVICE,       // 107
    APIID_DELETEMINIMUMQUALITYOFSERVICE,    // 108
    APIID_SETGPRSATTACHED,                  // 109
    APIID_GETGPRSATTACHED,                  // 110
    APIID_SETGPRSCONTEXTACTIVATED,          // 111
    APIID_GETGPRSCONTEXTACTIVATEDLIST,      // 112
    APIID_ENTERGPRSDATAMODE,                // 113
    APIID_GETGPRSADDRESS,                   // 114
    APIID_GPRSANSWER,                       // 115
    APIID_GETGPRSREGISTRATIONSTATUS,        // 116
    APIID_GETGPRSCLASS,                     // 117
    APIID_SETGPRSCLASS,                     // 118
    APIID_GETMOSMSSERVICE,                  // 119
    APIID_SETMOSMSSERVICE,                  // 120
    APIID_GETCBMSGCONFIG,                   // 121
    APIID_SETCBMSGCONFIG,                   // 122
    APIID_GETCURRENTPRIVACYSTATUS,          // 123
    APIID_GETCURRENTSYSTEMTYPE,             // 124
    APIID_SENDAKEY,                         // 125
    APIID_SENDFLASH,                        // 126
    APIID_GETCURRENTADDRESSID,              // 127
    APIID_SETCURRENTADDRESSID,              // 128
    APIID_GETALLOPERATORSLIST,              // 129
    APIID_REBOOTRADIO,                      // 130
    APIID_REGISTERATCOMMANDLOGGING,         // 131
    APIID_ATCOMMANDLOGFILE,                 // 132
    APIID_GETATR,                           // 133
    APIID_SENDSIMTOOLKITEVENTDOWNLOAD,      // 134
    APIID_LOGEVENTTORADIO,                  // 135
    APIID_NDISSETGPRSCONTEXTACTIVATED,      // 136
    APIID_NDISSENDPACKET,                   // 137
    APIID_NDISRECEIVEPACKETDONE,            // 138
    APIID_TOTAL                             // 139
};


//
// Event severity levels
//
#define EL_SEVERITY_CRITICAL            0x5
#define EL_EVENTID_MASK                 0x1FFFFFFF

//
// Event IDs
//
#define RILLOG_EVENT_LOWMEMORY          EL_MAKE_EVENTID(    6, EL_SEVERITY_CRITICAL     )
#define RILLOG_EVENT_TOOMANYTIMEOUTS    EL_MAKE_EVENTID(    7, EL_SEVERITY_CRITICAL     )
#define RILLOG_EVENT_GENERALREINIT      EL_MAKE_EVENTID(    9, EL_SEVERITY_CRITICAL     )


// Initializes Ril
HRESULT FRil_Initialize();

// Deinitializes Ril
HRESULT FRil_Deinitialize();

// Set the delay time which the Fake RIL will 
// wait before it returns the API response
HRESULT FRil_SetAPIExecTime(
    APIID api,     // @param Constant identifying a RIL API
    DWORD dwDuration    // @param Time taken by the Ril to execute the above API
);

// Dump a snapshot of the current network 
// configuration into an XML file
HRESULT FRil_DumpNetConfig(
    char* pszPath    // @param path to a file
);

// Load a network configuration from an
// XML file
HRESULT FRil_LoadNetConfig(
    char* pszPath    // @param path to a file
);

// Force APIs to fail with certain error codes
// and set the delay time before the failure
HRESULT FRil_FailAPI(
    APIID api,    // @param Constant identifying a RIL API
    DWORD dwCount,    // @param constant specifying the number of times this API will fail
    DWORD dwDuration, // @param delay before this API fail
    DWORD dwError // @param Error code that the API will fail with
);

// Add a network operator
HRESULT FRil_AddOp(
    const RILOPERATORINFO * lpOpInfo, // @param pointer to operator info struct
    DWORD dwSignalStrength    // @param Signal Strength of this Op -- 0 < CSQ < 32
);

// Remove a network operator
HRESULT FRil_RemOp(
    char* pszOpNumName  //@param Operator Numeric name
);

// Set an operator as the Home operator
HRESULT FRil_SetHomeOp(
    char* pszOpNumName  //@param Operator Numeric name
);

// Deny/Un deny operators
HRESULT FRil_SetOpDeniedStatus(
    char* pszOpNumName,  //@param Operator Numeric name
    BOOL fDeny    // @param True ? Denied
);

// Set the signal strength of some operator
HRESULT FRil_SetOpSignalStrength(
    char* pszOpNumName,  //@param Operator Numeric name
    int nSignalStrength        // @param signal strength
);

// Signal An Event
HRESULT FRil_SignalEvent(
    DWORD dwEventID // @ param Event ID to be Signaled
);

// Simulate receiving a call
HRESULT FRil_ReceiveCall(
    TCHAR* tszAddress, // @param phone number of the calling party
    DWORD dwAddType,
    DWORD dwRestriction,
    DWORD dwDuration,     // duration before hanging up
    UINT nLineNumber
);

// Simulate receiving a message
HRESULT FRil_ReceiveMsg(
    RILMESSAGE* lpRilMsg  // @param Ril message struct
);

HRESULT FRil_ReceiveNotification(
    DWORD dwNotificationCode,
    void *pData,
    UINT cbSizeofData    
);

HRESULT FRil_SetRegistrationStatus(
    DWORD dwRegState
);

//Simulate a remote hang up: finds call with address that matches wszAddress and
// terminates the call remotely.  Note that if wszAddress is NULL, this function
// hangs up the first caller-ID-witheld call, if any exist.
HRESULT FRil_RemoteHangUp(
    TCHAR* tszAddress
);


//FRil_NetCongestion:
//  Simulates congested network conditions

HRESULT FRil_SetNetworkCongestion(
    BOOL    fEnabled,   //enabled: yes/no?
    DWORD   dwError,    //error to throw in RIL
    DWORD   dwDuration  //time to wait before fail
);

//FRil_SetErrorGenerator
//  Set the probability that the smart error generator will fail API commands
HRESULT FRil_SetErrorGenerator (
    INT iChanceofFailure  // percentage chance of failure: 0-100
);

//FRil_GetReadyState
//   Reads the ReadyState flags.
HRESULT FRil_GetReadyState(
    DWORD* pdwReadyState
);

#ifdef _INCLUDE_RILVERSIONEX_
// FRil_SetDrvVersion
//   Set the content of the version struct that is returned by the return to the proxy
HRESULT FRil_SetDrvVersion (
    BOOL                fUseHook,          // return this structure or the built in one
    RILVERSIONEX  RILVERSION      // version structure
);
#endif  //_INCLUDE_RILVERSIONEX_