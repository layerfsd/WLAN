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


#ifndef _CCOREUTL_H_
#define _CCOREUTL_H_


#include <windows.h>
#include <simmgr.h>

#include "dialact.h"
#include "supsvcs.h"


#ifdef __cplusplus
extern "C" {
#endif

#define SCF_OVERRIDE_SIM    (0x00000001)
#define SCF_REFRESH_RECORD  (0x00000002)
#define SCF_ASYNC           (0x00000004) //NYI

HRESULT SimCacheReadRecord(DWORD dwFlags, DWORD dwAddress, DWORD dwRecordType, DWORD dwIndex, __out_bcount(dwBufferSize) LPBYTE lpData, DWORD dwBufferSize, LPDWORD lpdwBytesRead);
HRESULT SimCacheWriteRecord(DWORD dwFlags, DWORD dwAddress, DWORD dwRecordType, DWORD dwIndex, __out_bcount(dwByteCount) LPBYTE lpData, DWORD dwByteCount);
HRESULT SimCachePurgeRecord(DWORD dwFlags, DWORD dwAddress);
HRESULT SimCachePurgeAllRecords(DWORD dwFlags);
HRESULT SimCachePurgePhonebook(DWORD dwFlags);

HRESULT SimCacheInitPurge();
HRESULT SimCacheInitRefresh();

// INTERNAL to CCoreUtl.
HRESULT SimCacheGetRecordInfo(DWORD dwAddress, LPSIMRECORDINFO lpSimRecordInfo);

// INTERNAL to CCoreUtl
HRESULT SimCacheGetSimStatus();

//
// APIs for ccoreutl.dll
//
HRESULT ReadSimLanguage(LANGID *plidSim);
HRESULT WriteSimLanguage(LANGID lidSimNew);

//
// Convert a byte stream (code as EF-ADN) into a dial string
//
HRESULT ConvertBytesToDialString(LPBYTE lpbParse, DWORD dwLength, TCHAR *pwszAddress, DWORD *pdwAddrType, DWORD *pdwNumPlan);

//
// Convert a dial string into a byte stream (code as EF-ADN)
//
HRESULT ConvertDialStringToBytes(TCHAR *pwszAddress, LPBYTE rgb, DWORD dwSize, int *pcbUsed);

//
// Get string of emergency call numbers
//
HRESULT GetEmergencyCallList(TCHAR *pwszBuffer, UINT uLenBuf);

//
// Return the last error from the radio module
// HIWORD is one of the RADIO_ constants defined below
// LOWORD is the raw error code
// 0 for no error (or unable to retrieve error)
//
DWORD GetLastRadioError();


//
// Return the status of whether the current ICC/UICC is a SIM or USIM
// S_OK : USIM
// S_FALSE : SIM
// E_* failures
//
HRESULT UsimCheck();


//
// Registry values for language settings
//

#define SIM_LANGUAGE_REGKEY        TEXT("Security\\SimLang")
#define SIMLANG_IMSIVALUE_NAME     TEXT("Id")
#define SIMLANG_LANGVALUE_NAME     TEXT("Lang")

//
// SIM control Registry values
//
#define SIM_SECURE_REG_KEY         TEXT("Comm\\Cellular\\Sim")
#define SIM_REG_VALUE_CPHSFILES    TEXT("UseCPHSFiles")


//
// Error types
//
#define RADIO_EXTERROR                      0x00000001
#define RADIO_CMEERROR                      0x00000002
#define RADIO_CMSERROR                      0x00000003
#define RADIO_TIMEOUTERROR                  0x00000004

//
// Constants for use with the ReadSIMFlags/WriteSIMFlags APIs
//
#define SIMFLAG_VOICE_MESSAGE_WAITING_FLAG_IDENTIFIER 0x6F11L
#define SIMFLAG_CALL_FORWARDING_FLAG_IDENTIFIER       0x6F13L
#define SIMFLAG_CPHS_CUSTOMER_SERVICE_PROFILE         0x6F15L

#define SIMFLAG_VOICE_LINE1 0x00000001
#define SIMFLAG_VOICE_LINE2 0x00000002
#define SIMFLAG_FAX         0x00000004
#define SIMFLAG_DATA        0x00000008

//
// Read the values of various SIM flags
// Currently supported:
//   * CPHS-style "voice message waiting or call forwarding flags" (SIMFLAG_*)
//
HRESULT ReadSIMFlags(const DWORD dwId, DWORD* const lpdwFlags);

//
// Set the values of various SIM flags according to the input mask
// Currently supported:
//   * CPHS-style "voice message waiting or call forwarding flags" (SIMFLAG_*)
// Note:
//   Only the flags set in dwMask will be affected.  Flags that are set in dwMask
//   will be written according to the flag setting in dwFlags.
// Examples:
//   For both examples, dwId = SIMFLAG_VOICE_MESSAGE_WAITING_FLAG_IDENTIFIER
//   To clear voice line 1: dwFlags = 0, dwMask = SIMFLAG_VOICE_LINE1
//   To set fax and clear data: dwFlags = SIMFLAG_FAX, dwMask = SIMFLAG_FAX | SIMFLAG_DATA
//
HRESULT WriteSIMFlags(const DWORD dwId, const DWORD dwFlags, const DWORD dwMask);
//
// Read the CPHS customer service profile blob, and then searches for
//     the Service Group information specified by the group ID.
//    If found, pass out the group information, otherwise fail.
//
HRESULT ReadCPHSCustomerServiceProfileData(const DWORD dwGroupID, DWORD* pdwData);

HRESULT SHReadLineAddressCaps(LPTSTR szNumber, PDWORD pdwCallFwdModes, UINT nLineNumber);

#define SHGetPhoneNumber(sz, n) SHReadLineAddressCaps(sz, NULL, n)

#define PHSDDF_DEFAULTDURATION     0x00000001
#define PHSDDF_LONGDURATION        0x00000002
void PHSetDTMF_Duration(DWORD dwDurationFlag);

//
// Send A-Key to network and verify the result from Network.
//
#define MAX_AKEY_LEN                26
#define WM_RIL_SENDAKEY_RESULT      (WM_USER+501)

#define MAX_AREACODE_LEN            64

HRESULT NetworkSendAKey(HWND hWnd, LPCTSTR pszAKey);

//
// Phone get feature level
//
// Below defines should sync with the define SHDFL_PHONE_* in public\shellw\oak\inc\wpcpriv.h
#define PHGFL_NOPHONE                   0x00000000
#define PHGFL_PHONE                     0x00000001
#define PHGFL_RADIOPRESENT              0x00000002
#define PHGFL_RADIOON                   0x00000004
#define PHGFL_LOCKFACILITY              0x00000008
#define PHGFL_SIMSUPPORT                0x00000010
#define PHGFL_VOICEDIALCONNECTS         0x00000020
#define PHGFL_UNIQUECALLTRACKING        0x00000040
#define PHGFL_AUTOTHREEWAYCALLING       0x00000080
#define PHGFL_INTLPLUS                  0x00000100 // Convert "0" to "+"
#define PHGFL_SMSUNICODE                0x00000200
#define PHGFL_0340SMS                   0x00000400 // GSM SMS standard
#define PHGFL_SUPPORTVOICEPRIVACY       0x00000800
#define PHGFL_AKEY                      0x00001000
#define PHGFL_IS637SMS                  0x00002000 // CDMA SMS standard
#define PHGFL_SUPPORTGPRS               0x00004000
#define PHGFL_SUPPORTMOSMS              0x00008000
#define PHGFL_SUPPORTMULTILINES         0x00010000
#define PHGFL_PHONE_SUPPORTSPEAKERPHONE 0x00020000
#define PHGFL_SUPPORTWCMP               0x00040000
#define PHGFL_SUPPORTAUTOPREFIX         0x00080000
#define PHGFL_SUPPORTINTERNATIONALCODE  0x00100000
#define PHGFL_VTDIALCONNECTS            0x00200000 // Auto connects Video Telephony calls on dial success.
#define PHGFL_LAST_FEATURE              PHGFL_VTDIALCONNECTS

// Features that are current 'GSM', but could later be broken out with their own feature flag
#define PHGFL_CELLBROADCAST             PHGFL_0340SMS              // GSM feature

#define PHGFL_ALL_MASKS           ((DWORD)-1)

#define PHGMLFL_NOML_EX             0x00000000
#define PHGMLFL_ML_OUTGOING         0x00000001
#define PHGMLFL_ML_VMAIL            0x00000002
#define PHGMLFL_ML_FORWARD          0x00000004

#define PHGMLFL_ALL_MASKS           ((DWORD)-1)

BOOL PHGetFeatureLevel(DWORD *pdwFeatureLevel, DWORD dwFeatureLevelMask);
BOOL PHGetMLFeatureLevel(DWORD *pdwFeatureLevel, DWORD dwFeatureLevelMask);

BOOL PHIsNetworkSupportedFeature(DWORD dwPHFL);

#define PHNETWORKSPECIFICFUNCTION(dwPHFL) PHNETWORKSPECIFICFUNCTIONEX(dwPHFL, E_NOTIMPL)

#define PHNETWORKSPECIFICFUNCTIONEX(dwPHFL, ret) \
    { if(!PHIsNetworkSupportedFeature(dwPHFL)) return ret; }


// Helper fct in cellcore will delay load RestoreLastRadioTransmitReceiveState in ccoreutl
#define RADIO_POWER_ON 0x00000001
void SetRadioState( DWORD dwState );

//this sets the current outgoing line
HRESULT SetCurrentLine(HSUPSVCS hSupService, DWORD dwLineNumber);

//this gets the current outgoing line. 0 based index, so 0 == line1, 1 == line2, etc.
DWORD GetCurrentLine();

//helper to drop all connmgr calls, and then wait for the calls to properly end (for up to 20s)
HRESULT DropAllCallsSync(BOOL fSleepAfterClose);

#ifdef __cplusplus
}
#endif


#endif  // _CCOREUTL_H_
