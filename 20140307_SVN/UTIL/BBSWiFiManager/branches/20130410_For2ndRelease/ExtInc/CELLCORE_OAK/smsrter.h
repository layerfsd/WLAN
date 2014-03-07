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
//////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999-2000 Microsoft Corporation.  All rights reserved. //
//////////////////////////////////////////////////////////////////////////


#ifndef _SMSRTER_H_
#define _SMSRTER_H_


#include <windows.h>
#include <ril.h>
#include <sms.h>
#include <smsstoreitem.h>
#ifdef UNDER_CE
#include <windev.h>
#else  // UNDER_CE
#define CTL_CODE(DeviceType, Function, Method, Access) \
                (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define METHOD_BUFFERED                             0x00
#define FILE_ANY_ACCESS                             0x00
#endif // UNDER_CE


//
// Defines and forward declarations
//
#define MAX_EVENT_NAME_LENGTH    (128)
#define COOKIE_VALUE             (0x3579)
class CRouter;

//
// SMSRTER.dll IO-Control codes and structures
//
#define FILE_DEVICE_SMS                                0x400

// In:  SMSREGISTRATIONDATA*
// Out: HRESULT*
#define IOCTL_SMS_REGISTER                             CTL_CODE(FILE_DEVICE_SMS,  1, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSREGISTRATIONDATA*
// Out: HRESULT*
#define IOCTL_SMS_UNREGISTER                           CTL_CODE(FILE_DEVICE_SMS,  2, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGOPENPARAMS*
// Out: SMSMESSAGINGOPENRESULTS*
#define IOCTL_SMS_MESSAGING_OPEN                       CTL_CODE(FILE_DEVICE_SMS,  10, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGCLOSEPARAMS*
// Out: SMSMESSAGINGCLOSEPARAMS*
#define IOCTL_SMS_MESSAGING_CLOSE                      CTL_CODE(FILE_DEVICE_SMS,  11, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGSENDPARAMS*
// Out: SMSMESSAGINGSENDRESULTS*
#define IOCTL_SMS_MESSAGING_SEND                       CTL_CODE(FILE_DEVICE_SMS,  12, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGSENDPARAMS*
// Out: SMSMESSAGINGSENDRESULTS*
// #define IOCTL_SMS_MESSAGING_SENDPREENCODED             CTL_CODE(FILE_DEVICE_SMS,  13, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGGETSIZEPARAMS*
// Out: SMSMESSAGINGGETSIZERESULTS*
#define IOCTL_SMS_MESSAGING_GETSIZE                    CTL_CODE(FILE_DEVICE_SMS,  14, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGREADPARAMS*
// Out: SMSMESSAGINGREADRESULTS*
#define IOCTL_SMS_MESSAGING_READ                       CTL_CODE(FILE_DEVICE_SMS,  15, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGGETSTATUSPARAMS*
// Out: SMSMESSAGINGGETSTATUSRESULTS*
#define IOCTL_SMS_MESSAGING_GETSTATUS                  CTL_CODE(FILE_DEVICE_SMS,  16, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGGETSMSCPARAMS*
// Out: SMSMESSAGINGGETSMSCRESULTS*
#define IOCTL_SMS_MESSAGING_GETSMSC                    CTL_CODE(FILE_DEVICE_SMS,  17, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGSETSMSCPARAMS*
// Out: SMSMESSAGINGSETSMSCRESULTS*
#define IOCTL_SMS_MESSAGING_SETSMSC                    CTL_CODE(FILE_DEVICE_SMS,  18, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGGETBROADCASTMSGRANGESPARAMS*
// Out: SMSMESSAGINGGETBROADCASTMSGRANGESRESULTS*
#define IOCTL_SMS_MESSAGING_GETBROADCASTMSGRANGES      CTL_CODE(FILE_DEVICE_SMS,  19, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGSETBROADCASTMSGRANGESPARAMS*
// Out: SMSMESSAGINGSETBROADCASTMSGRANGESRESULTS*
#define IOCTL_SMS_MESSAGING_SETBROADCASTMSGRANGES      CTL_CODE(FILE_DEVICE_SMS,  20, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGGETPHONENUMBERPARAMS*
// Out: SMSMESSAGINGGETPHONENUMBERRESULTS*
#define IOCTL_SMS_MESSAGING_GETPHONENUMBER             CTL_CODE(FILE_DEVICE_SMS,  21, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGGETTIMEPARAMS*
// Out: SMSMESSAGINGGETTIMERESULTS*
#define IOCTL_SMS_MESSAGING_GETTIME                    CTL_CODE(FILE_DEVICE_SMS,  22, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSMESSAGINGRECEIVEALLMESSAGESFROMSIMPARAMS*
// Out: SMSMESSAGINGRECEIVEALLMESSAGESFROMSIMRESULTS*
#define IOCTL_SMS_MESSAGING_RECEIVEALLMESSAGESFROMSIM  CTL_CODE(FILE_DEVICE_SMS,  23, METHOD_BUFFERED, FILE_ANY_ACCESS)

// In:  SMSGETIOCONTROLPARAMS*
// Out: SMSGETIOCONTROLRESULTS*
#define IOCTL_SMS_GET_IO_CONTROL                       CTL_CODE(FILE_DEVICE_SMS,  30, METHOD_BUFFERED, FILE_ANY_ACCESS)


typedef struct smsroutermsginfo_tag {
    UINT cbSize;
    UINT cbMessageUID;                           // Length of message UID (non-zero iff the message is multi-part)
    BYTE rgbMessageUID[SMS_MAX_MESSAGEUID_SIZE]; // Message UID
    UINT nTotalParts;                            // Total number of parts in the message (valid only if cbMessageUID != 0)
    UINT iPartIndex;                             // Sequence number of this part (valid only if cbMessageUID != 0)
    BOOL fDoNotConcatenate;                      // Override to return segments as individual messages on read
} SMSROUTERMSGINFO, *LPSMSROUTERMSGINFO;

typedef struct smsroutermsginsiminfo_tag {
    SMSROUTERMSGINFO srmi;
    UINT cbSize;
    DWORD dwLocation;
    DWORD dwIndex;
} SMSROUTERMSGINFOEX, *LPSMSROUTERMSGINFOEX;

typedef struct smsmessagingopenparams_tag {
    UINT uiSize;
    TCHAR ptsReadMessageEventName[MAX_EVENT_NAME_LENGTH];
    TCHAR ptsReadStatusMessageEventName[MAX_EVENT_NAME_LENGTH];
    TCHAR ptsMessageProtocol[SMS_MAX_PROTOCOLNAME_LENGTH];
    DWORD dwMessageModes;
} SMSMESSAGINGOPENPARAMS;

typedef struct smsmessagingopenresults_tag {
    UINT uiSize;
    HRESULT hrResult;
    SMS_HANDLE smshHandle;
} SMSMESSAGINGOPENRESULTS;

typedef struct smsmessagingcloseparams_tag {
    UINT uiSize;
    SMS_HANDLE smshHandle;
} SMSMESSAGINGCLOSEPARAMS;

typedef struct smsmessagingcloseresults_tag {
    UINT uiSize;
    HRESULT hrResult;
} SMSMESSAGINGCLOSERESULTS;

typedef struct smsmessagingsendparams_tag {
    UINT uiSize;
    SMS_HANDLE smshHandle;
    bool bSMSCAddressValid;
    SMS_ADDRESS smsaSMSCAddress;
    SMS_ADDRESS smsaDestinationAddress;
    SYSTEMTIME stValidityPeriod;
    const BYTE* pbData;
    DWORD dwDataSize;
    const BYTE* pbProviderSpecificData;
    DWORD dwProviderSpecificDataSize;
    SMS_DATA_ENCODING smsdeDataEncoding;
    DWORD dwOptions;
} SMSMESSAGINGSENDPARAMS;

typedef struct smsmessagingsendresults_tag {
    UINT uiSize;
    HRESULT hrResult;
    SMS_MESSAGE_ID smsmidMessageID;
} SMSMESSAGINGSENDRESULTS;

/*
typedef struct smsmessagingsendpreencodedparams_tag {
    UINT uiSize;
    SMS_HANDLE smshHandle;
    bool bSMSCAddressValid;
    SMS_ADDRESS smsaSMSCAddress;
    const BYTE* pbData;
    DWORD dwDataSize;
} SMSMESSAGINGSENDPREENCODEDPARAMS;

typedef struct smsmessagingsendpreencodedresults_tag {
    UINT uiSize;
    HRESULT hrResult;
    SMS_MESSAGE_ID smsmidMessageID;
} SMSMESSAGINGSENDPREENCODEDRESULTS;
*/

typedef struct smsmessaginggetsizeparams_tag {
    UINT uiSize;
    SMS_HANDLE smshHandle;
} SMSMESSAGINGGETSIZEPARAMS;

typedef struct smsmessaginggetsizeresults_tag {
    UINT uiSize;
    HRESULT hrResult;
    DWORD dwMessageSize;
} SMSMESSAGINGGETSIZERESULTS;

typedef struct smsmessagingreadparams_tag {
    UINT uiSize;
    SMS_HANDLE smshHandle;
    BYTE* pbBuffer;
    DWORD dwBufferSize;
    BYTE* pbProviderSpecificBuffer;
    DWORD dwProviderSpecificBufferSize;
} SMSMESSAGINGREADPARAMS;

typedef struct smsmessagingreadresults_tag {
    UINT uiSize;
    HRESULT hrResult;
    TCHAR ptsMessageProtocol[SMS_MAX_PROTOCOLNAME_LENGTH];
    SMS_ADDRESS smsaSMSCAddress;
    SMS_ADDRESS smsaSourceAddress;
    SYSTEMTIME stReceiveTime;
    DWORD dwBytesWritten;
} SMSMESSAGINGREADRESULTS;

template <class T> class GrowableThingCollection;
typedef struct sms_manager_cookie_data_tag {
    DWORD dwCookieValue;
    CRouter* pcrRouter;
    SMS_MESSAGE_ID* psmsmidMessageID;
    GrowableThingCollection<DWORD>* pgtcMessagesInGroup;
    HANDLE hSMSStore;
    DWORD dwSendOptions;
} SMS_MANAGER_COOKIE_DATA;
typedef const SMS_MANAGER_COOKIE_DATA* SMS_MANAGER_COOKIE;

typedef struct smsmessaginggetstatusparams_tag {
    UINT uiSize;
    SMS_HANDLE smshHandle;
    SMS_MESSAGE_ID smsmidMessageID;
    SMS_STATUS_INFORMATION* psmssiStatusInformation;
    DWORD dwTimeout;
} SMSMESSAGINGGETSTATUSPARAMS;

typedef struct smsmessaginggetstatusresults_tag {
    UINT uiSize;
    HRESULT hrResult;
} SMSMESSAGINGGETSTATUSRESULTS;

typedef struct smsmessaginggetsmscparams_tag {
    UINT uiSize;
} SMSMESSAGINGGETSMSCPARAMS;

typedef struct smsmessaginggetsmscresults_tag {
    UINT uiSize;
    HRESULT hrResult;
    SMS_ADDRESS smsaSMSCAddress;
} SMSMESSAGINGGETSMSCRESULTS;

typedef struct smsmessagingsetsmscparams_tag {
    UINT uiSize;
    SMS_ADDRESS smsaSMSCAddress;
} SMSMESSAGINGSETSMSCPARAMS;

typedef struct smsmessagingsetsmscresults_tag {
    UINT uiSize;
    HRESULT hrResult;
} SMSMESSAGINGSETSMSCRESULTS;

typedef struct smsmessaginggetbroadcastmsgrangesparams_tag {
    UINT uiSize;
    SMS_BROADCAST_RANGES* psmsbrBroadcastRanges;
} SMSMESSAGINGGETBROADCASTMSGRANGESPARAMS;

typedef struct smsmessaginggetbroadcastmsgrangesresults_tag {
    UINT uiSize;
    HRESULT hrResult;
} SMSMESSAGINGGETBROADCASTMSGRANGESRESULTS;

typedef struct smsmessagingsetbroadcastmsgrangesparams_tag {
    UINT uiSize;
    const SMS_BROADCAST_RANGES* psmsbrBroadcastRanges;
} SMSMESSAGINGSETBROADCASTMSGRANGESPARAMS;

typedef struct smsmessagingsetbroadcastmsgrangesresults_tag {
    UINT uiSize;
    HRESULT hrResult;
} SMSMESSAGINGSETBROADCASTMSGRANGESRESULTS;

typedef struct smsmessaginggetphonenumberparams_tag {
    UINT uiSize;
} SMSMESSAGINGGETPHONENUMBERPARAMS;

typedef struct smsmessaginggetphonenumberresults_tag {
    UINT uiSize;
    HRESULT hrResult;
    SMS_ADDRESS smsaAddress;
} SMSMESSAGINGGETPHONENUMBERRESULTS;

typedef struct smsmessaginggettimeparams_tag {
    UINT uiSize;
    SYSTEMTIME* pstCurrentTime;
    DWORD* pdwErrorMargin;
} SMSMESSAGINGGETTIMEPARAMS;

typedef struct smsmessaginggettimeresults_tag {
    UINT uiSize;
    HRESULT hrResult;
} SMSMESSAGINGGETTIMERESULTS;

typedef struct smsmessagingreceiveallmessagesfromsimparams_tag {
    UINT uiSize;
} SMSMESSAGINGRECEIVEALLMESSAGESFROMSIMPARAMS;

typedef struct smsmessagingreceiveallmessagesfromsimresults_tag {
    UINT uiSize;
    HRESULT hrResult;
} SMSMESSAGINGRECEIVEALLMESSAGESFROMSIMRESULTS;

typedef BOOL (*IOControlFunction) (DWORD hOpenContext, DWORD dwCode, PBYTE pBufIn, DWORD dwLenIn, PBYTE pBufOut, DWORD dwLenOut, PDWORD pdwActualOut);

typedef struct smsgetiocontrolparams_tag {
    UINT uiSize;
    DWORD dwProcessID;
} SMSGETIOCONTROLPARAMS;

typedef struct smsgetiocontrolresults_tag {
    UINT uiSize;
    IOControlFunction pfIOControl;
    DWORD hOpenContext;
} SMSGETIOCONTROLRESULTS;


//
// Callback functions
//
typedef HRESULT (*SMS_MANAGER_SEND_MESSAGE) (
    SMS_MANAGER_COOKIE smsmcCookie,
    const RILMESSAGE* const prilmMessage,
    const TCHAR* const ptsMessageProtocol
);

typedef HRESULT (*SMS_MANAGER_GET_NEXT_MESSAGE) (
    SMS_MANAGER_COOKIE smsmcCookie,
    SMSSTOREITEM* const psmssiMessage
);

typedef HRESULT (*SMS_MANAGER_GET_NEXT_MESSAGE_IN_SIM) (
    SMS_MANAGER_COOKIE smsmcCookie,
    SMSSTOREITEMEX* const psmssiMessage
);

typedef BOOL (CALLBACK *SMSRECOGNIZECALLBACK) (
    const RILMESSAGE* prmMessage
);

typedef BOOL (CALLBACK *SMSRECOGNIZEINSIMCALLBACK) (
    const RILMESSAGE* prmMessage,
    DWORD dwLocation,
    DWORD dwIndex
);

typedef BOOL (CALLBACK *SMSINFOCALLBACK) (
    const RILMESSAGE* prmMessage,
    SMSROUTERMSGINFO* psmsriInfo
);

typedef BOOL (CALLBACK *SMSINFOINSIMCALLBACK) (
    const RILMESSAGE* prmMessage,
    SMSROUTERMSGINFOEX* psmsriInfo
);

typedef HRESULT (CALLBACK *SMSSENDCALLBACK) (
    SMS_MANAGER_COOKIE smsmcCookie,
    const BYTE* pbData,
    DWORD dwDataSize,
    const BYTE* pbProviderSpecificData,
    DWORD dwProviderSpecificDataSize,
    SMS_DATA_ENCODING smsdeDesiredDataEncoding,
    RILMESSAGE* prilmTemplate,
    SMS_MANAGER_SEND_MESSAGE smsmsmSendMessageFunction
);

typedef HRESULT (CALLBACK *SMSGETSIZECALLBACK) (
    SMS_MANAGER_COOKIE smsmcCookie,
    SMSSTOREITEM* psmssiMessage,
    SMS_MANAGER_GET_NEXT_MESSAGE smsmgnmGetNextMessageFunction,
    DWORD* pdwSize
);

typedef HRESULT (CALLBACK *SMSGETSIZEINSIMCALLBACK) (
    SMS_MANAGER_COOKIE smsmcCookie,
    SMSSTOREITEMEX* psmssiMessage,
    SMS_MANAGER_GET_NEXT_MESSAGE_IN_SIM smsmgnmGetNextMessageFunction,
    DWORD* pdwSize
);

typedef HRESULT (CALLBACK *SMSREADCALLBACK) (
    SMS_MANAGER_COOKIE smsmcCookie,
    SMSSTOREITEM* psmssiMessage,
    BYTE* pbBuffer,
    DWORD dwBufferSize,
    BYTE* pbProviderSpecificBuffer,
    DWORD dwProviderSpecificBufferSize,
    SMS_MANAGER_GET_NEXT_MESSAGE smsmgnmGetNextMessageFunction,
    DWORD* pdwBytesRead
);

typedef HRESULT (CALLBACK *SMSREADINSIMCALLBACK) (
    SMS_MANAGER_COOKIE smsmcCookie,
    SMSSTOREITEMEX* psmssiMessage,
    BYTE* pbBuffer,
    DWORD dwBufferSize,
    BYTE* pbProviderSpecificBuffer,
    DWORD dwProviderSpecificBufferSize,
    SMS_MANAGER_GET_NEXT_MESSAGE_IN_SIM smsmgnmGetNextMessageFunction,
    DWORD* pdwBytesRead
);

typedef HRESULT (CALLBACK *SMSIDENTIFYGROUPCALLBACK) (
    SMS_MESSAGE_ID smsmidMessageID,
    const DWORD* pdwMessageIDs,
    size_t stNumberMessageIDs
);

typedef HRESULT (CALLBACK *SMSGETSTATUSCALLBACK) (
    SMS_MESSAGE_ID smsmidMessageID,
    SMS_STATUS_INFORMATION* psmssiStatusInformation,
    DWORD dwTimeout
);

#endif // _SMSRTER_H_
