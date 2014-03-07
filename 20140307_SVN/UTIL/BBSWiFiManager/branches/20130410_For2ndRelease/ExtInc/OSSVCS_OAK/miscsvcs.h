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
//  *************************************************************************
//
//  miscsvcs.h
//
//  Misc OS Services API declarations
//
//  Owner: romans@microsoft.com
//
//  Copyright 1999-2000 Microsoft Corporation, All Rights Reserved.
//
//  Project: Stinger
//
//  Platform: WinCE
//
//  History: 2000-06-15 [romans] Created
//
//  *************************************************************************

#pragma once

#include <windows.h>
#include "tapi.h"
#include "devnotify.h"
#include "wincrypt.h"
#include <errorrep.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// To enable autodoc tags
//@doc

//  *************************************************************************
//
//  Global Counter APIs
//
//  *************************************************************************


//  *************************************************************************
//  GCounter_Open
//
//  Purpose:
//      Opens/creates a global counter
//
//  Parameters:
//      tszName     [in]  global counter name
//      ulStart     [in]  starting value assigned when the counter is created
//                        (ignored, if the counter already exists)
//      phCounter   [out] handle to the opened global counter
//
//  Returns:
//      HRESULT, error code
//
//  Side Effects:
//      none
//  *************************************************************************
HRESULT GCounter_Open(const LPCTSTR tszName, const ULONG ulStart, HANDLE* const phCounter);

//  *************************************************************************
//  GCounter_Close
//
//  Purpose:
//      Closes a global counter
//
//  Parameters:
//      hCounter    [in]  handle to the global counter to be closed
//
//  Returns:
//      HRESULT, error code
//
//  Side Effects:
//      none
//  *************************************************************************
HRESULT GCounter_Close(const HANDLE hCounter);

//  *************************************************************************
//  GCounter_Delete
//
//  Purpose:
//      Deletes a global counter from the system
//
//  Parameters:
//      hCounter    [in]  handle to the global counter to be deleted
//
//  Returns:
//      HRESULT, error code
//
//  Side Effects:
//      none
//  *************************************************************************
HRESULT GCounter_Delete(const HANDLE hCounter);

//  *************************************************************************
//  GCounter_GetValue
//
//  Purpose:
//      Retrieves current value of a global counter
//
//  Parameters:
//      hCounter    [in]  handle to the global counter
//      pulValue    [out] value of the global counter
//
//  Returns:
//      HRESULT, error code
//
//  Side Effects:
//      none
//  *************************************************************************
HRESULT GCounter_GetValue(const HANDLE hCounter, ULONG* const pulValue);

//  *************************************************************************
//  GCounter_SetValue
//
//  Purpose:
//      Set value of a global counter
//
//  Parameters:
//      hCounter    [in]  handle to the global counter
//      ulValue     [in]  value to be set
//
//  Returns:
//      HRESULT, error code
//
//  Side Effects:
//      none
//  *************************************************************************
HRESULT GCounter_SetValue(const HANDLE hCounter, const ULONG ulValue);

//  *************************************************************************
//  GCounter_IncrementValue
//
//  Purpose:
//      Increments value of a global counter
//
//  Parameters:
//      hCounter    [in]  handle to the global counter
//      pulValue    [out] new value of the global counter
//
//  Returns:
//      HRESULT, error code
//
//  Side Effects:
//      none
//  *************************************************************************
HRESULT GCounter_IncrementValue(const HANDLE hCounter, ULONG* const pulValue);

//  *************************************************************************
//  CreateWorkerWindow
//
//  Purpose:
//      Helper function to create a worker window
//
//  Parameters:
//      pwszClass   Class name (NULL preferred, unless you need a unique class)
//      pfnWndProc  Window proc for worker window
//      hwndParent  Parent
//      dwExStyle   WS_EX_* styles
//      dwFlags     WS_* styles
//      pwszTitle   Window title
//      hmenu       Menu or child ID
//      p           Comes through in lParam of CREATESTRUCT during WM_CREATE processing
//
//  Returns:
//      HWND of worker window
//
//  Side Effects:
//      none
//  *************************************************************************
HWND CreateWorkerWindow(TCHAR *pwszClass, WNDPROC pfnWndProc, HWND hwndParent, 
                        DWORD dwExStyle, DWORD dwFlags, TCHAR *pwszTitle,
                        HMENU hmenu, void * p);



//
// Purpose: tells where the vk_key came from... whether software, rocker, or dpad
//
// returns: one of the below.
#define KEY_SOURCE_UNKNOWN_HARDWARE  0x0000
#define KEY_SOURCE_DPAD              0x0001
#define KEY_SOURCE_ROCKER            0x0002
#define KEY_SOURCE_SOFTWARE          0x0003

// Note that the drivers which provide these two VK codes do not link with this
// header so it is NOT safe to change these values.
#define KEY_SOURCE_ROCKER_VK         VK_F20
#define KEY_SOURCE_DPAD_VK           VK_F21

DWORD GetKeyEventSource(int vKey);


//  *************************************************************************
//  IsFESystem
//
//  Purpose:
//      Helper function to determine if the user's locale is set for FE.
//
//  Parameters:
//      None
//
//  Returns:
//      BOOL        TRUE if the current locale if Far East, FALSE otherwise
//
//  Side Effects:
//      none
//  *************************************************************************
BOOL IsFESystem();

// List of restrictions which are supported.
typedef enum
{
    REST_NONE               = 0x00000000,   // No restrictions
    REST_NORUNDLG           = 0x00000001,   // Disallow Shell Run Dlg
    REST_NOAUTORUN          = 0x00000002,   // No CF Autorun -  !! Must match definition of SECPOLICY_CFAUTORUN in secpol.h !!
    REST_DISALLOWRUN        = 0x00000003,   // Apps specifically disallowed
    REST_RESTRICTRUN        = 0x00000004,   // Apps specifically allowed to run
    REST_NODOWNLOAD         = 0x00000005,   // Don't allow any downloads(inbox, pie, fexplore, etc)
    REST_PASSWDPERIOD       = 0x00000006,   // Password Active Period
    REST_NOPASSWDPERIOD     = 0x00000007,   // Disallow setting of active period
    REST_NOWEAKPASSWORD     = 0x00000008,   // Require Strong password
    REST_NORAPIREGMOD       = 0x00000009,   // Disallow RAPI modifications to "protected" reg keys
    REST_NOEXTERNALEXES     = 0x0000000A,   // Don't allow foreign exes on the device(fexplore, doclist)
} RESTRICTIONS;

#define REG_REST_SIGNATURE  TEXT("Restriction")

#define REST_VALUE_DENIED       0x00000001
#define REST_VALUE_ALWAYSSAFE   0x00000002

//  *************************************************************************
//  SHRestricted
//
//  Purpose:
//      Retrieves value of specified restriction if set
//
//  Parameters:
//      rest    [in]  Requested restrition
//
//  Returns:
//      DWORD   0 if the restriction is not in place
//              non-zero indicates that the restriction is in place
//
//  Side Effects:
//      none
//  *************************************************************************
DWORD WINAPI SHRestricted(RESTRICTIONS rest);

BOOL WINAPI SHSetRestriction(RESTRICTIONS rest, DWORD dw);

//  *************************************************************************
//  QueryPolicyImpl
//
//  Purpose:
//      Retrieves value of specified policy.  If the policy setting does not
//      exist, the return value is 0.
//
//  Parameters:
//      DWORD dwPolicyId    [in]  Policy identifier
//
//  Returns:
//      DWORD   Value of the policy
//              0 may be either the value of the policy or an indication that
//              the policy does not exist
//
//  Side Effects:
//      none
//
//  Notes: This is the actual implementation of QueryPolicy. New code must
//         use the public API version QueryPolicy.
//  *************************************************************************
DWORD WINAPI QueryPolicyImpl (DWORD dwPolicyId);


//  *************************************************************************
//  SetPolicy
//
//  Purpose:
//      Sets the value of the specified policy.
//      This API can only be called by trusted applications
//
//  Parameters:
//      DWORD dwPolicyId     [in]  Identifier of policy to set
//      DWORD dwValue        [in]  New Value for the policy
//
//  Returns:
//      BOOL   Policy was updated successfully
//             Policy was NOT updated successfully
//
//  Side Effects:
//      none
//  *************************************************************************
BOOL WINAPI SetPolicy(DWORD dwPolicyId, DWORD dwValue);

// Used for MD5 hash function
typedef struct hash_t {
    DWORD   hashdata[4];
} hash_t;

BOOL MD5HashString(LPCWSTR psz, hash_t *lphash);


//  *************************************************************************
//  RestoreLastRadioTransmitReceiveState
//
//  Purpose:
//      Restores the last transmit/receive state of the device's radio module.
//
//  Details:
//      If the user turns the device off with the radio in full power mode,
//      he/she turns the device back on, the shell should call this API to
//      restore that state.  Note that before calling this API, the radio
//      will be powered off for safety.
//
//  Parameters:
//      rlrtrsoOption  [in]  One of:
//                           RLRTRS_DEFAULT
//                              Default behavior
//                           RLRTRS_FORCE_RADIO_POWER_OFF
//                              For reasons of safety, ensure that the
//                              radio power remains off (e.g., user powers
//                              device on in an airplane).
//
//  Returns:
//      HRESULT
//
//  Side Effects:
//      This API will also register the radio module on the network if it is
//      possible to do so.
//  *************************************************************************
typedef enum
{
    RLRTRS_DEFAULT = 0,
    RLRTRS_FORCE_RADIO_POWER_OFF,
    RLRTRS_FORCE_RADIO_POWER_ON,
    RLRTRS_RADIO_REGISTER,
    RLRTRS_DEVICE_POWER_OFF,
    RLRTRS_DEVICE_POWER_ON,
} RLRTRS_OPTION;
HRESULT RestoreLastRadioTransmitReceiveState(RLRTRS_OPTION rlrtrsoOption);
HRESULT RestoreLastRadioTransmitReceiveStateSync(RLRTRS_OPTION rlrtrsoOption);


//  *************************************************************************
//  UnregisterFromNetwork
//
//  Purpose:
//      Unregisters the radio module from the network if it is
//      possible to do so, and waits for that operation to complete.
//
//  Returns:
//      HRESULT
//
//  *************************************************************************
HRESULT WINAPI UnregisterFromNetwork();

//
// Get/Set/Increment the phone volume.
//
HRESULT GetPhoneVolume(DWORD *pdwVol);
HRESULT SetPhoneVolume(DWORD dwVol);
// lChange can be positive or negative, pdwNewUIVol is scaled to the UI scale (5 increments), use GetPhoneVolume to get the actual value
HRESULT IncrementPhoneVolume(LONG lChange, DWORD* pdwNewUIVol);
// Initializes the phone volume to persisted value (called when the radio turns on)
HRESULT InitializePhoneVolume();

// Translate from UI scale to wave device scale and back
#define MAX_UI_VOLUME        5
#define MAX_SPEAKER_VOL      0xFFFF  // this is the max for our wave drivers...
#define DEFAULT_SPEAKER_VOL  0xCCCC  // if anything goes wrong, use this value as a reasonable audible default
#define MIN_SPEAKER_VOL      0x1111  // minimum desirable volume in most cases
DWORD UIVolumeToSpeakerVolume(DWORD dwUIVol);
DWORD SpeakerVolumeToUIVolume(DWORD dwSpeakerVol);

#ifndef NO_VALIDATEAPIPERMISSION
//  *************************************************************************
//  ValidateAPIPermission
//
//  Purpose:
//      Validate that the caller has permission to call a named API
//
//  Details:
//      Uses the CeGetCallerTrust OS API and the settings in HKLM\Secure\APIs
//      to ensure that the caller has permission to call a particular API.
//      See the documentation of CeGetCallerTrust for more information.
//      
//  Parameters:
//      pszAPIModule  [in]  The name of the API's module
//      pszAPIName    [in]  The name of the API
//      vtOption      [in]  The trust level to check
//                          VAP_TRUST_CURRENT - Current process
//                          VAP_TRUST_CALLER - Caller process (for drivers)
//
//  Returns:
//      HRESULT
//
//  Side Effects:
//      None
//
//  Example:
//      If pszAPIModule is "Module" and pszAPIName is "APIName",
//      HKLM\Security\APIs\Module will be checked for a DWORD value named
//      "APIName" - if no such value is present, HKLM\Security\APIs\Module
//      will be checked for a default value - if no such value is present,
//      HKLM\Security\APIs will be checked for a default value - if no such
//      value is found, E_ACCESSDENIED will be returned.  If a value was
//      found at any of these steps, it will be treated as the minimum
//      acceptable trust level of a caller of the APIName API and checked
//      against the return value from CeGetCallerTrust to verify that the
//      caller meets the minimum trust level.  If the caller is acceptable,
//      S_OK will be returned - otherwise E_ACCESSDENIED will be returned.
//  *************************************************************************
typedef enum
{
    VAP_TRUST_CURRENT=1,
    VAP_TRUST_CALLER=2,
} VAP_TRUST;
HRESULT ValidateAPIPermission(LPCWSTR pszAPIModule, LPCWSTR pszAPIName, VAP_TRUST vtOption);
#else // NO_VALIDATEAPIPERMISSION
#define ValidateAPIPermission(m, n, o) (S_OK)
// Avoid warning C4127: conditional expression is constant
#pragma warning(disable : 4127)
#endif // NO_VALIDATEAPIPERMISSION


//  *************************************************************************
//  GetDataDirectory
//
//  Purpose:
//      Retrieves the default data directory path.
//
//  Parameters:
//      pszBuffer   [out]   Pointer to the buffer to receive the null-terminated 
//                          string containing the path. 
//                          This path does not end with a backslash.
//      cchSize  [in]       Specifies the maximum size, in characters, of the 
//                          string buffer specified by the pszBuffer parameter. 
//                          This value should be set to MAX_PATH to allow 
//                          sufficient room for the path. 
//
//  Returns:
//      DWORD  If the function succeeds, the return value is the number of characters copied into the
//             the buffer.  Otherwise, the return value is the size of the buffer required to
//             hold the path.
//
//  Side Effects:
//      none
//  *************************************************************************
DWORD GetDataDirectory(
  LPTSTR pszBuffer,  // buffer for Data directory
  DWORD cchSize     // number of bytes available in pszBuffer
);

//  *************************************************************************
//  MountDefaultDBVol
//
//  Purpose:
//      Creates a default Database Volume.  If no path found in registry
//      returns CREATE_SYSTEMGUID(pceguid)
//
//  Parameters:
//      pceguid     [out]   the guid(handle) for volume created.
//
//  Returns:
//      BOOL   FALSE if unsuccessful
//             TRUE if successful
//
//  Side Effects:
//      none
//  *************************************************************************
BOOL MountDefaultDBVol(PCEGUID pceguid);

//  *************************************************************************
//  MountRegistryNamedDBVol
//
//  Purpose:
//      Mounts a database volume named in a specific location of the registry
//      (HKEY_CURRENT_USER\Software\Microsoft\Windows)
//
//  Parameters:
//      pszName     [in]    the identifier of the volume
//      pceguid     [out]   the guid (handle) for the volume
//
//  Returns:
//      BOOL   FALSE if unsuccessful
//             TRUE if successful
//
//  Side Effects:
//      none
//  *************************************************************************
BOOL MountRegistryNamedDBVol(LPCWSTR pszName, PCEGUID pceguid);

//  *************************************************************************
//  ReadMailVolumeName
//
//  Purpose:
//      Reads in the name of the mail volume based on a combination of two 
//      registry keys
//
//  Parameters:
//      pszName     [in]    a buffer to hold the name of the mail database
//      cchSize     [in]    size of the buffer in characters
//
//  Returns:
//      Number of characters added to pszName (or needed in pszName)
//
//  Side Effects:
//      none
//  *************************************************************************
DWORD ReadMailVolumeName(LPWSTR pszName, DWORD cchSize);

//  *************************************************************************
//@func BOOL | IsCompactFlashPath | Helper function to determine if the specified directory is a  
//                                  removable storage card.
//
//@parm LPCTSTR | lpszFileName     | IN - a directory file name
//@parm DWORD   | dwFileAttributes | IN - the directory's file attributes
//
//@rdesc Returns TRUE iff the directory is a removable storage card
//
//@comm This helper function is created to abstract the details for what
//      determines that a directory is in fact removable storage. Prior
//      implementations depended on the file attributes to distinguish
//      removable storage, but this is not sufficient since the mount
//      directory for persistent storage has the same file attributes yet
//      it is not removable.
//      
//  *************************************************************************
BOOL IsCompactFlashPath(LPCTSTR lpszFileName, DWORD dwFileAttributes);

//  *************************************************************************
//@func BOOL | IsCompactFlashPath | Helper function to determine if the specified directory is a  
//                                  removable storage card.
//
//@parm LPWIN32_FIND_DATA | lpfdCF | IN - a pointer to a WIN32_FIND_DATA structure
//
//@rdesc Returns TRUE iff the directory is a removable storage card
//
//@comm This helper function exists to reduce code churn in existing code
//      that used to call its own version of IsCompactFlash.
//      
//  *************************************************************************
static __inline BOOL IsCompactFlash(WIN32_FIND_DATA *lpfdCF)
{
    return IsCompactFlashPath(lpfdCF->cFileName, lpfdCF->dwFileAttributes);
}

//  *************************************************************************
//@func HANDLE | FindFirstStorageCard | Searches for a storage card.
//
//@parm LPWIN32_FIND_DATA | lpFindData | OUT - a pointer to a WIN32_FIND_DATA structure
//
//@rdesc Returns a handle that can be used for subsequent calls to FindNextStorageCard;
//       INVALID_HANDLE_VALUE indicates that no storage cards were found
//
//@comm This helper function is created to abstract the details for finding
//      removable storage cards. It is designed to mimic the behavior of
//      FindFirstFile and FindNextFile except that no path is necessary
//      in the call to FindFirstStorageCard. A valid handle returned from
//      this function must be closed using FindClose. These functions also
//      rely on the OS functions for error handling, such as checking for
//      invalid handles and pointers.
//      
//  *************************************************************************
HANDLE FindFirstStorageCard(LPWIN32_FIND_DATA lpFindData);

//  *************************************************************************
//@func BOOL | FindNextStorageCard | Continues a search for a storage card.
//
//@parm HANDLE            | hFind      | IN - a handle returned from a call to FindFirstStorageCard
//@parm LPWIN32_FIND_DATA | lpFindData | OUT - a pointer to a WIN32_FIND_DATA structure
//
//@rdesc Returns TRUE if another storage card has been found; FALSE if no more storage cards could be found
//
//@comm This helper function is created to abstract the details for finding
//      removable storage cards. It is designed to mimic the behavior of
//      FindFirstFile and FindNextFile except that no path is necessary
//      in the call to FindFirstStorageCard.
//      
//  *************************************************************************
BOOL FindNextStorageCard(HANDLE hFind, LPWIN32_FIND_DATA lpFindData);


//self explanatory
LONGLONG LONGLONGFromSystemTime(SYSTEMTIME syst, WORD wYear);

//this calculates if a timezone is currently in DST
HRESULT CalcIfDST(TIME_ZONE_INFORMATION *ptzi, SYSTEMTIME *pstCur, BOOL *pfDST);

//  *************************************************************************
// Trust Marshaller Helper functions
//  *************************************************************************

// Untrusted marshaller's application name .
#define UMARSHALLER_APPNAME         TEXT("umarshaller.exe")

// Trusted marshaller's application name .
#define TMARSHALLER_APPNAME         TEXT("tmarshaller.exe")

// Trustmarshaller MutexName Prefix
#define TRUSTMARSHALLER_MUTEXNAME_PREFIX    TEXT("TM-")

// Window class name of TrustMarshaller's hidden window
#define TRUSTMARSHALLER_WINCLASSNAME    TEXT("TrustMarshaller")
#define TRUSTMARSHALLER_CLIENT_WINCLASSNAME    TEXT("TrustMarshallerClient")

// $BUGBUG:  Random facility number
#define TRUSTMARSHALLER_FACILITY    0x2A

// Method Types
typedef enum 
{ 
    eInstall_Init, 
    eInstall_Exit, 
    eUninstall_Init, 
    eUninstall_Exit,
    eDllRegister, 
    eDllUnregister,
    eProcessConfig,
    eProveTrust,
    eConfigReturnValue,
    eProveTrustReturnValue,
    eWipeVolumes
}METHOD_TYPE ;

// Max number of windows to be found.
#define MAX_ENUMWINDOWS     32

// Flags for enumerating windows
#define WPDFLAG_FINDFIRST   0x01

typedef struct WINDOWSPROCDATA {
    DWORD   dwProcId;
    DWORD   dwHwndCount;
    DWORD   dwFlags;

    HWND    hwnd[MAX_ENUMWINDOWS];
} *PWINDOWSPROCDATA;

// Data pertaining to a launched TrustMarshaller
typedef struct TRUSTMARSHALLERDATA {
    DWORD   dwUMProcId;
    HWND    hwndUM;
    HANDLE  hmtxUM;
    HWND    hwndUMClient;
    LPVOID  lpvGenericData;
} *PTRUSTMARSHALLERDATA;

BOOL CALLBACK GetWinHandleProc(HWND hwnd, LPARAM lParam);
HRESULT GetExeProcessId(LPTSTR pszExeFile, DWORD* pdwProcID);

// TrustMarshaller API's
HRESULT TrustMarshallerLaunch(HANDLE *phTrustMarshaller, BOOL fTrusted);
HRESULT TrustMarshallerClose(HANDLE hTrustMarshaller);
HRESULT TrustMarshallerInvokeFunction(HANDLE hTrustMarshaller, METHOD_TYPE mt, VARIANT *avarParams, DWORD dwParamCount, DWORD *pdwRetVal);
HRESULT TrustMarshallerInvokeFunctionRV(HANDLE hTrustMarshaller, METHOD_TYPE mt, VARIANT *avarParams, DWORD dwParamCount, VARIANT* avarRetVal, HRESULT* pHr);

// Marshaller Helper functions for Parameter retrieval and setting
HRESULT GetParam(DWORD dwParamIndex, LPBYTE pbParams, DWORD cbParams, LPBYTE *ppbOutParam, DWORD *pcbOutParam, DWORD cbParamCheck);
HRESULT SetParams(VARIANT *avarParams, DWORD dwParamCount, DWORD *pcbParams, LPBYTE pbParams);


//  *************************************************************************
//  Unsupported helper functions that you shouldn't use unless you know
//  exactly what they do.
//
//  *************************************************************************
int wcsncpyX(TCHAR *pszDst, const TCHAR *pszSrc, int cchMax);
int wcsncpyXIndirect(TCHAR** ppszDst, const TCHAR* pszSrc, int* pcchDst);
int wcsncpyXIgnore(TCHAR *pszDst, const TCHAR *pszSrc, int cchMax, const TCHAR *pszIgnore);
const TCHAR *wcstail(const TCHAR *pszSrc, int cch);
int ustrcmp(const TCHAR *psz1, const TCHAR *psz2);
void GetSystemTimeAsULL(ULONGLONG* pull);
DWORD GetPlatformBuildNumber( );
TCHAR *LoadStringEtc(HINSTANCE hinst, int idsStr, TCHAR *pszDef);
int LoadHTML(HINSTANCE hInstance, LPCTSTR lpName, LPTSTR lpBuffer, int cchBufMax);
TCHAR *LoadHTMLEtc(HINSTANCE hinst, TCHAR* pszIDsStr, TCHAR *pszDef);
LPTSTR BigStrcat(UINT count, ... ); //must LocalFree the ret
BOOL IsCurrentTimeInBusyAppointment();
BOOL IsCurrentTimeInAppointment();
HRESULT GetProfileLabel(const TCHAR* pszProfile, BSTR* pbstrLabel);
#ifndef SHIP_BUILD
BOOL IsMonkeyProofed();
#endif

#define TAPI_LOW_VERSION    0x00020000
#define TAPI_HIGH_VERSION   0x00020000
#define EXTAPI_LOW_VERSION  0x00010000
#define EXTAPI_HIGH_VERSION 0x00010000

DWORD GetPhoneDeviceId(HPHONEAPP hPhoneApp, DWORD dwNumDevs, DWORD dwAPIVersionLow,
DWORD dwAPIVersionHigh, const TCHAR *pszDeviceName);

DWORD GetLineDeviceId(HLINEAPP hLineApp, DWORD dwNumDevs, DWORD dwAPIVersionLow,
DWORD dwAPIVersionHigh, const TCHAR *pszDeviceName);





//  *************************************************************************
//  CreateStreamOnFile
//
//  Purpose:
//      Create an IStream Interface on an existing file, or a temporary file
//
//  Parameters:
//      pszFileName    [in]  Pointer to the NULL terminated full path name.
//      dwFlags        [in]  Flags for specifying specific behaviors.
//      ppstm          [out] The newly created IStream object.
//
//  Returns:
//      HRESULT, error code
//
//  Side Effects:
//      none
//  *************************************************************************
HRESULT CreateStreamOnFile(LPCTSTR pszFileName, DWORD dwFlags, IStream** ppstm);

typedef enum // Flags for CreateStreamOnFile
{
    CSOF_READONLY           = 0x00000001,   // Open the file for Read Only
    CSOF_DELETEONCLOSE      = 0x00000002,   // Delete the file when the object is finally released
} CSOF_FLAGS;


//  *************************************************************************
//  CreateStreamOnHandle
//
//  Purpose:
//      Create an IStream Interface on an existing handle.
//
//  Parameters:
//      hFile          [in]  A valid file handle.
//      ppstm          [out] The newly created IStream object.
//
//  Returns:
//      HRESULT, error code
//
//  Side Effects:
//      ppstm will call CloseHandle on hFile when released.
//  *************************************************************************
HRESULT CreateStreamOnHandle(HANDLE hFile, IStream** ppstm);


//  *************************************************************************
//  CreateBufferedStreamOnHandle
//
//  Purpose:
//      Create an IStream Interface on an existing handle.
//
//  Parameters:
//      hFile          [in]  A valid file handle.
//      cbBufferSize   [in]  The size of the buffer in bytes.
//      ppstm          [out] The newly created IStream object.
//
//  Returns:
//      HRESULT, error code
//
//  Side Effects:
//      ppstm will call CloseHandle on hFile when released. The buffered stream
//      is not thread safe!!!
//  *************************************************************************
HRESULT CreateBufferedStreamOnHandle(HANDLE hFile, DWORD cbBufferSize, IStream** ppstm);



//  *************************************************************************
//
//  Function for registering with a device to get a callback
//
//  *************************************************************************
typedef enum tagRegisterDeviceCallbackDevice
{
    rdcdHeadset = 0,
    rdcdCarkit,
    rdcdSpeaker,
    
    rdcdLast  // for bounds checking
}RegisterDeviceCallbackDevice;
HRESULT SHRegisterDeviceCallback(RegisterDeviceCallbackDevice rdcd, HWND hwndCallback, UINT uMsg, BOOL fRegister);

//  *************************************************************************
//
//  Functions for get/set speakerphone mode
//
//  *************************************************************************
HRESULT SHGetSpeakerMode(SPEAKERMODE *pmode);
HRESULT SHSetSpeakerMode(SPEAKERMODE mode);

BOOL WINAPI Reg_GetDWord(HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue,LPDWORD pdw);
BOOL WINAPI Reg_SetDWord(HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dw);
HRESULT Reg_GetDWordEx(HKEY hkey, const TCHAR* pszSubKey, const TCHAR* pszValue, DWORD dwDefault, BOOL fSetDefault, DWORD* pdw);
void Reg_GetVals(HKEY hk, LPCTSTR pszSubkey, LPCTSTR const pszNameTab[], DWORD **dwValTab, int cTab);


typedef enum tagBASE64_OPTIONS
{
    // Ignore invalid character in the stream. This is strictly conformant
    // with the Base64 spec. 
    BASE64_OPTION_IGNOREINVALIDCHARACTER,

    // In some cases, invalid characters cause a canonicalization issue, so we
    // want to fail if the encode stream contains them.
    BASE64_OPTION_ERRORONINVALIDCHARACTER
} BASE64_OPTIONS;

HRESULT EncodeBase64(
    __in_ecount(cb)     UCHAR *pb,
                        ULONG cb,
    __out_ecount(cbOut) UCHAR *pbOut,
                        ULONG cbOut,
    __out               ULONG *pcbUsed);

HRESULT DecodeBase64(
    __in_ecount(cbEnc)  UCHAR* pbEnc, 
                        ULONG cbEnc, 
    __out_ecount(cbDec) UCHAR* pbDec,
                        ULONG cbDec, 
    __out               ULONG* pcbUsed, 
                        BASE64_OPTIONS eDecodeOptions);

HRESULT EncodeBase64W(
    __in_ecount(cb) __opt   LPBYTE  pb, 
                            DWORD   cb, 
    __deref_out             LPTSTR  *ppszEnc);

HRESULT DecodeBase64W(
    __in_opt        LPCTSTR         pszEnc,
    __deref_out_opt LPBYTE*         ppbDec,
    __out           DWORD*          pcbDec,
                    BASE64_OPTIONS  eDecodeOptions);

#ifndef _FN_CRC32_ // for imgdecmp
ULONG crc32(ULONG crc, const BYTE *pbBuf, UINT cbBuf);
#endif

//  *************************************************************************
//
//  Function blocks until we are fully booted
//
//  *************************************************************************

void WaitForFullBoot();

//  *************************************************************************
//
//  Function to flush information to the flash filesystem before power off
//
//  *************************************************************************

void FlushSystemOnPowerOff();

//  *************************************************************************
//
//  Function loads xml configuration file from ROM
//
//  *************************************************************************
HRESULT LoadConfigFile(LPCTSTR pszFile, LPTSTR *pszOutXML);

void LocalFreeOssvcsMem(LPBYTE pb);

HRESULT CreateDirectoryPath(LPCWSTR _pszPath);

//  *************************************************************************
//  GetFileVersionsInfo
//
//  Purpose:
//      Retrieves file version info from resource segment (usually has to be a DLL)
//
//  Parameters:
//      const TCHAR* pszFileFullPath - [in] path of file version to query
//      DWORD* pdwVersionMajor - [out] major version
//      DWORD* pdwVersionMinor - [out] minor version
//
//  Returns:
//      HRESULT - 
//
//  Side Effects:
//      none
//  *************************************************************************
HRESULT GetFileVersionsInfo(const TCHAR* pszFileFullPath, DWORD* pdwVersionMajor, DWORD* pdwVersionMinor);

//  *************************************************************************
//  ProcessRoles
//
//  Purpose:
//      Applies role-mapping policies to a set of roles.
//
//  Parameters:
//      DWORD   dwRoles     [in]    Set of roles to which policies are applied.
//
//  Returns:
//      DWORD -- final mapped roles
//
//  Side Effects:
//      none
//  *************************************************************************
DWORD ProcessRoles(DWORD dwRoles);

//  *************************************************************************
//  BinaryToHexStringEx
//
//  Purpose:
//      Turns bytes into hexidecimal string equivalents.
//
//  Parameters:
//      LPBYTE      pbData          [in]   Buffer holding binary data
//      DWORD       cbData          [in]   Number of bytes in binary buffer
//      LPTSTR      szHexStr        [out]  Output string
//      DWORD*      pcchHexStr      [out]  Length of pszHexStr, including null-terminator
//      BOOL        fLSBFirst       [in]   Flag indicating translation of least
//                                         significant bit first
//      BOOL        fSpaceSeperated [in]   Flag indicating insertion of a space
//                                         character between byte values in output
//
//  Returns:
//      HRESULT indicating success
//
//  Comments:
//      If szHexStr is NULL, pcchHexStr will contain the length needed 
//      to hold the string (including the NULL-terminator).  
//      If szHexStr is non-NULL, pcchHexStr MUST contain the length 
//      of the string buffer (in characters).
//      This function NULL-terminates.
//  *************************************************************************
HRESULT BinaryToHexStringEx(
        __in_ecount(cbData)             LPBYTE  pbData, 
                                        DWORD   cbData, 
        __in_ecount(*pcchHexStr) __opt  LPTSTR  szHexStr, 
        __inout                         DWORD  *pcchHexStr, 
                                        BOOL    fLSBFirst,
                                        BOOL    fSpaceSeparated
);

//  *************************************************************************
// BinaryToHexString
//
// Purpose:
//      Turns bytes into hexidecimal string equivalents.
//
// Parameters:
//      LPBYTE      pbData      [in]   Buffer holding binary data
//      DWORD       cbData      [in]   Number of bytes in binary buffer
//      LPTSTR      szHexStr    [out]  Output string
//      DWORD*      pcchHexStr  [out]  Length of pszHexStr, including null-terminator
//      BOOL        fLSBFirst   [in]   Flag indicating translation of least
//                                     significant bit first
//
// Returns:
//      HRESULT indicating success
//
// Comments:
//      If szHexStr is NULL, pcchHexStr will contain the length needed 
//      to hold the string (including the NULL-terminator).  
//      If szHexStr is non-NULL, pcchHexStr MUST contain the length 
//      of the string buffer (in characters).
//      This function NULL-terminates.
//
// See Also:
//      BinaryToHexStringEx()
//  *************************************************************************
HRESULT BinaryToHexString(
    __in_ecount(cbData)             LPBYTE  pbData, 
                                    DWORD   cbData, 
    __in_ecount(*pcchHexStr) __opt  LPTSTR  szHexStr, 
    __inout                         DWORD   *pcchHexStr, 
                                    BOOL    fLSBFirst
);

//  *************************************************************************
// HexStringToBinary
//
// Purpose:
//      Turns hashes into string equivalents.
//
// Parameters:
//      LPTSTR      szHexStr    [in]   Input string
//      LPBYTE      pbData      [out]  Buffer to hold binary data
//      DWORD*      pcbData     [out]  Number of bytes in binary buffer
//      DWORD*      pcchHexStr  [in]   Length of pszHexStr, including null-terminator
//      BOOL        fLSBFirst   [in]   Flag indicating translation of least
//                                     significant bit first
// Returns:
//      HRESULT indicating success
//  *************************************************************************
HRESULT HexStringToBinary(LPCTSTR szHexStr, LPBYTE pbData, DWORD *pcbData, BOOL fLSBFirst);


// **************************************************************************
// UnicodeToMB
// 
// Purpose: Convert Unicode buffer to multibyte.
//
// Parameters:
//      LPCTSTR pszIn       [in]    Unicode buffer (NULL-terminated) to convert
//      UINT    unCodePage  [in]    Code page of multibyte encoding
//      LPSTR*  ppaszOut    [out]   Returned multibyte buffer
//      DWORD*  pcchOut     [out]   Length of returned multibyte buffer
//
// Return Values:
//      HRESULT, indicating success or failure
//
// Comments:
//      Caller must free allocated memory using delete[].
// **************************************************************************
HRESULT UnicodeToMB(
    __in        LPCTSTR pszIn, 
                UINT    unCodePage, 
    __deref_out LPSTR*  ppaszOut, 
    __out_opt   DWORD*  pcchOut);


// **************************************************************************
// MBToUnicode
// 
// Purpose: Convert multibyte buffer to Unicode.
//
// Parameters:
//      LPCSTR  paszIn      [in]    Multibyte buffer (NULL-terminated) to convert
//      UINT    unCodePage  [in]    Code page of multibyte encoding
//      LPTSTR* ppszOut     [out]   Returned Unicode buffer
//      DWORD*  pcchOut     [out]   Length of returned Unicode buffer
//
// Return Values:
//      HRESULT, indicating success or failure
//
// Comments:
//      Caller must free allocated memory using delete[].
// **************************************************************************
HRESULT MBToUnicode(
    __in        LPCSTR  paszIn, 
                UINT    unCodePage, 
    __deref_out LPTSTR* ppszOut, 
    __out_opt   DWORD*  pcchOut);


// **************************************************************************
// OpenLine
// 
// Purpose: Open a line.
//
// Parameters:
//      HLINEAPP*   phLineApp   [out]
//      HLINE*      phLine      [out]
//
// Return Values:
//      HRESULT, indicating success or failure
//
// Comments:
//      Note that the caller needs the hLineApp and hLine back to shut down the line
//      when they are done.
// **************************************************************************
HRESULT OpenLine(LPCTSTR pszNewLineName, HLINEAPP *phLineApp, HLINE* phLine);


// **************************************************************************
// GetHeader
// 
// Purpose: Return the value of a specified header.
//
// Arguments:
//    IN LPCTSTR pszAllHeaders – all headers in http format
//    IN LPCTSTR pszHeaderName – name of header whose value is desired
//    OUT LPTSTR* ppszHeaderValue – value of header
//
// Return Values:
//    HRESULT
//    returns an error code indicating success or failure
//
// Description:  
//    Parse the headers for a specific one.  Each header is assumed to be 
//    in the following format:
//      field-name ":" field-value "\r\n"
//
//      The returned header value needs to be deallocated with delete[].
// **************************************************************************
HRESULT GetHeader(LPCTSTR pszAllHeaders, LPCTSTR pszHeaderName, LPTSTR *ppszHeaderValue);


// **************************************************************************
// EscapeStringA
//
// Purpose: Escapes characters in a string according to a translation table.
//
// Arguments:
// IN   LPCSTR pszIn         - String to be translated (source)
// OUT  LPSTR  pszOut        - String after translation (destination)
// IN   int     cchOut        - Number of characters in destination string passed.
// IN   LPCSTR ppszTable     - Translation table
// IN   int     cchTabEntries - Number of entries in the translation table
//
// Return Values: Returns the number of characters copied into pszOut if pszOut
//       is not NULL. If pszOut is NULL, it returns the required size of pszOut.
//
// Description:  The first character in each of the translation table entry is  
//       replaced by the rest of the entry string.  For example,  
//       an HTML escape table would use the following table:
//
//      const LPCSTR s_rgszHTMLEscapes[3] =
//      {
//          "<&lt;",
//          ">&gt;",
//          "&&amp;"
//      };
//
// **************************************************************************
int EscapeStringA(LPCSTR pszIn, LPSTR pszOut, int cchOut, const LPCSTR *ppszTable, int cchTabEntries);


// **************************************************************************
// EscapeStringW
//
// Purpose: Escapes characters in a string according to a translation table. This is
//               the wide char version of the function
//
// Arguments:
// IN   LPCTSTR pszIn         - String to be translated (source)
// OUT  LPTSTR  pszOut        - String after translation (destination)
// IN   int     cchOut        - Number of characters in destination string passed.
// IN   LPCTSTR ppszTable     - Translation table
// IN   int     cchTabEntries - Number of entries in the translation table
//
// Return Values: Returns the number of characters copied into pszOut if pszOut
//       is not NULL. If pszOut is NULL, it returns the required size of pszOut.
//
// Description:  The first character in each of the translation table entry is  
//       replaced by the rest of the entry string.  For example,  
//       SHEscapeBubbleHtml uses the following table:
//
//      const LPCTSTR s_arszHTMLEscapes[3] =
//      {
//          TEXT("<&lt;"),
//          TEXT(">&gt;"),
//          TEXT("&&amp;")
//      };
//
// **************************************************************************
int EscapeStringW(LPCTSTR pszIn, LPTSTR pszOut, int cchOut, const LPCTSTR *ppszTable, int cchTabEntries);

#ifdef UNICODE
#define EscapeString  EscapeStringW
#else
#define EscapeString  EscapeStringA
#endif // !UNICODE

// **************************************************************************
// CopyString
// 
// Purpose: Make a copy of a string.
//
// Arguments:
//    IN LPCTSTR pszSource – source string
//    IN DWORD cchSourceLen – length of source string (without NULL-terminating character)
//    OUT LPTSTR* ppszOut – copy of source string
//
// Return Values:
//    HRESULT
//    returns an error code indicating success or failure
//
// Description:  
//    If pszSource has a NULL-terminating character, then cchSourceLen 
//    can be STRLEN_NULL_TERMINATED.  Caller must deallocate string with LocalFree.
// **************************************************************************
HRESULT CopyString(LPCTSTR pszSource, DWORD cchSourceLen, LPTSTR* ppszOut);

#define STRLEN_NULL_TERMINATED  ((DWORD)-1)


//***************************************************************************
// Function Name: SaveBlobToRegistry
//
// Purpose: Saves the specified data to the system registry.
//
// Arguments:
//  IN HKEY           hKeyRoot - Handle to a currently open key or one of the following 
//                              predefined reserved handle values: HKEY_CLASSES_ROOT, 
//                              HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_USERS
//
//  IN LPCTSTR     pszKeyPath - Specifies the path to the registry key. This is a subkey 
//                                              of the key identified by the hKey parameter.
//  IN const byte* pbData - Pointer to the data to save.
//  IN DWORD       cbData - Size in bytes of the data.
//
// Return Value:
//  NOERROR on success, otherwise a failure code. 
//
// Comments: Use this function to store data in a registry key,
//          even if it is larger than the WinCE limit of 4KB per data 
//          value in a registry key.
//          This function stores the data in the registry 
//          key by dividing the data into 4KB size pieces.  Each 
//          piece of data is stored in a separate registry value in the 
//          registry key.  The concatenated values form the entire data.
//          The names of the values are generated by having a base 
//          name for the registry value and concatenating a hexadecimal 
//          number to the end of it.  The hexadecimal number is incremented 
//          for each additional value that is needed to store the data.  The 
//          total size of the data being stored is saved in a separate value in 
//          the same registry key.
//
// **************************************************************************
HRESULT SaveBlobToRegistry(HKEY hKeyRoot, LPCTSTR pszKeyPath, const LPBYTE pbData, DWORD cbData);


//***************************************************************************
// Function Name: LoadBlobFromRegistry
//
// Purpose: Initializes a buffer from data stored in the system registry.
//
// Arguments:
//  IN HKEY        hKeyRoot - Handle to a currently open key or one of the following 
//                              predefined reserved handle values: HKEY_CLASSES_ROOT, 
//                              HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_USERS
//
//  IN LPCTSTR   pszKeyPath - Specifies the path to the registry key. This is a subkey 
//                                              of the key identified by the hKey parameter.
//  OUT LPBYTE* ppbData - On success, will contain a pointer to a pointer to the data. 
//                       The caller is responsible for calling LocalFree()
//                       when they're finished with the data.
//  OUT LPDWORD pcbData - On success, will contain the size of the data
//                       in bytes. 
//    
// Return Value:
//  NOERROR if the data was loaded from the registry, otherwise 
//  a failure code.
//
// Comments: Use this function to read in data stored in the registry by
//          SaveBlobToRegistry().  It gets the total size of the data from a
//          registry value and then uses the size to determine how many
//          values are used to store the data.  It then concatenates the data
//          from all the values to form the entire data.
//
// **************************************************************************
HRESULT LoadBlobFromRegistry(HKEY hKeyRoot, LPCTSTR pszKeyPath, LPBYTE* ppbData, LPDWORD pcbData);



// *******************************************************************************************
// CalcHash
//
// Purpose:
//      Calculate the MD-5 hash of a string.
//
// Parameters:
//      LPCTSTR pszIn       [in]    String to hash
//      ALG_ID  alg         [in]    Hashing algorithm
//      LPTSTR* ppszHash    [out]   String representing hash of input string
//
// Returns:
//      HRESULT indicating success
// *******************************************************************************************
HRESULT CalcHash(LPCTSTR pszIn, ALG_ID alg, LPTSTR *ppszHash);


//  *************************************************************************
//  VolatileGlobalValue keys
//
//  IMPORTANT: if you add a value to this table, check the implementation of these functions.
//  The current implementation uses StatStore, so a new value must be added to the table
//  of non-notifying values, otherwise all StatStore clients will be notified on every change
//  of this value (bad perf!).
//
//  *************************************************************************
#define VGV_NULL        0
#define VGV_DTMFMODE    1

#define VGV_LAST        VGV_DTMFMODE  // for bounds checking, update if you add a new value

//  *************************************************************************
// SetVolatileGlobalValue
//
// Purpose:
//      Sets a value that can be used across processes but that is volatile enough that
//      we don't want to use the registry (because registry writes causes flush to flash).
//
// Parameters:
//      DWORD   dwKey       [in] Key to value
//      DWORD   dwValue     [in] new value
//      DWORD*  pdwOldValue [out] old value
// Returns:
//      HRESULT indicating success
//  *************************************************************************
HRESULT SetVolatileGlobalValue(DWORD dwKey, DWORD dwNewValue, DWORD* pdwOldValue);

//  *************************************************************************
// GetVolatileGlobalValue
//
// Purpose:
//      Gets a volatile global value.
//
// Parameters:
//      DWORD   dwKey       [in] Key to value
//      DWORD*  pdwValue    [out] value
// Returns:
//      HRESULT indicating success
//  *************************************************************************
HRESULT GetVolatileGlobalValue(DWORD dwKey, DWORD* pdwValue);


//  *************************************************************************
//  FilteredGetAddrInfo
//
//  Purpose:
//      Helper function that depending on the value of a certain reg key will
//      strip out all non-ipv6 or non-ipv4 host entries in the returned addrinfo
//      linked list. The linked list is compatable with regular addrinfo lists though
//      and can be passed to freeaddrinfo() to free it. In WINCESHIP builds this 
//      compiles down into a direct call to getaddrinfo(). 
//
//      If <INSERT REG KEY> is set to a value of 1 it will filter out all IPv4 entries.
//      2 will filter out all IPv6 entries. 
//
//  Parameters:
//      const char FAR *nodename [in] Pointer to a NULL-terminated string containing a 
//                                    host (node) name or a numeric host address string. 
//                                    The numeric host address string is a dotted-decimal 
//                                    IPv4 address or an IPv6 hex address. 
//      const char FAR *servname [in] Pointer to a NULL-terminated string containing either 
//                                    a service name or port number. 
//      const struct addrinfo FAR *hints [in] Pointer to an addrinfo structure that provides 
//                                            hints about the type of socket the caller supports.
//      struct addrinfo FAR       **res  [out] Pointer to a linked list of one or more addrinfo 
//                                             structures containing response information about 
//                                             the host. 
//
//  Returns:
//      int - zero for success, failure returns standard
//            windows socket error code. 
//
//  Side Effects:
//      none
//  *************************************************************************
//#ifndef SHIP_BUILD
struct addrinfo; // prototype

int FilteredGetAddrInfo(const char FAR * nodename,
                        const char FAR * servname,
                        const struct addrinfo FAR * hints,
                        struct addrinfo FAR * FAR * res);
//#else 
//#define FilteredGetAddrInfo getaddrinfo
//#endif // !SHIP_BUILD

// **************************************************************************
// Function Name: BMHFindStrI
// 
// Purpose: case insensitive substring search using the Boyer-Moore-Horspool
//          algorithm. We require the user to pass in the table of skip values
//          so we can get improved performance when the user is able to
//          precompute the table or save an older instance of the table.
//
// Arguments:
//      IN  szSearchIn  -   The string to search in
//      IN  szSearchFor -   The substring to search for. This string must be 
//                          less than 256 characters long.
//      IN  rgSkipCount -   The skip (delta) table
//      IN  cSkipArraySize  -   The number of entries in the skip table. Any
//                              value found in szSearchIn whose value is
//                              is outside of the range of the table is assumed
//                              to have the maximum skip value (the length of
//                              the search string).
//      IN  cchSearcInLength    -   The length of the szSearchIn string
//      IN  cchSearchForLength  -   The length of the szSearchFor string
//
// Return Values:
//      If the substring is not found then NULL is returned. If the substring 
//      is found then a pointer to the first occurrence of the substring in
//      szSearchIn is returned.
//
// Side effects:
//      None, but be careful with using the returned string pointer as the
//      string it points to is also pointed to by szSearchIn.
// ***************************************************************************
TCHAR *BMHFindStrI (const TCHAR *szSearchIn, const TCHAR *szSearchFor, 
                        const BYTE *rgSkipCount, const int cSkipArraySize,
                        const DWORD cchSearchInLength, const DWORD cchSearchForLength);

// **************************************************************************
// Function Name: BuildSkipArray
// 
// Purpose: Build the skip array for a case insensitive search using BMHFindStrI
//
// Arguments:
//      IN szSearchFor      - string to search for (max string length = 255 chars)
//      IN pbSkipArray      - pointer to byte array
//      IN cSkipArraySize   - number of elements in the skip array. For BMHStrFind
//                            to work properly all characters in szSearchFor must
//                            have a value smaller than cSkipArraySize
//
// Return Values: 
//      HRESULT - S_OK if array could be built, error otherwise
//
// Side effects: none
// 
// Description: Used to create the skip array needed by BMHFindStrI. If you are
//              looking for the same string multiple times you can use this
//              routine once to precompute the array and save it. (or even
//              hardcode it)
// ***************************************************************************
HRESULT BuildSkipArray (const TCHAR *szSearchFor, BYTE *pbSkipArray, const DWORD cSkipArraySize);

// **************************************************************************
// Function Name: VariantChangeTypeMDD
// 
// Purpose: work around OS bug parsing Japanese short dates.
//
//   for documentation, see "VariantChangeType()" in MSDN, this has the exact same semantics
//
// ***************************************************************************
HRESULT VariantChangeTypeMDD(VARIANT* pvarDest, VARIANT* pvarSrc, unsigned short wFlags, VARTYPE vt);

// **************************************************************************
// Function Name: FixupPath
// 
// Purpose: Replaces outdated path elements with the correct path elements.
//
// ***************************************************************************
HRESULT FixupPath(const TCHAR* pszPath, TCHAR* pszPathOut, int cchOut);

//***   LoadLibraryRes -- load a .res file (w/o calling DllMain, etc.)
//
#if 1 // FEAT_DATALESS
#define LoadLibraryRes(p)   LoadLibraryEx((p), NULL, LOAD_LIBRARY_AS_DATAFILE)
#else
#define LoadLibraryRes(p)   LoadLibrary((p))
#endif

// **********************************************************************
// Function name: SearchSortedArray
//
// Purpose: a standard binary search, covering the 95% case.  Uses a
// caller-defined memcmp style comparison function, and allows the
// caller to pass in an LPARAM to provide the comparison with
// additional information.  Doesn't handle a block of storage
// containing differently size items.
//
// Returns: a pointer to the start of the item in the array on
// success, and NULL on failure
//
// **********************************************************************
    
typedef int (* PFNSSACOMPARE)(void *const p1,
                              void *const p2,
                              const LPARAM lp);

PVOID SearchSortedArray(void * const pvData,
                        const DWORD cData,
                        const DWORD cbDatum,
                        void * const pvToFind,
                        PFNSSACOMPARE pfnCompare,
                        const LPARAM lp);

// ******************************************************************************************
//
// WatsonReportFault
//
// Purpose:
//    Sends a report to pWatson if there is a fault.
//
// Parameters:
//  pep         - [in] Exception information from GetExceptionInformation();
//  iExpression - [in] Exception expresion (EXCEPTION_EXECUTE_HANDLER,EXCEPTION_CONTINUE_SEARCH,
//                                          EXCEPTION_CONTINUE_EXECUTION)
//
// Returns:
//  Exception expression so that this can be called specifically in the __except() paramter
//
// ******************************************************************************************
int WatsonReportFault(LPEXCEPTION_POINTERS pep, int iExpression);

// **********************************************************************
// Function name: SearchSortedArrayEx
//
// Purpose: a standard binary search.  Uses a
// caller-defined memcmp style comparison function, and allows the
// caller to pass in an LPARAM to provide the comparison with
// additional information.  Doesn't handle a block of storage
// containing differently size items.
//
// Returns: If Found, returns index to the location 
//          If not Found and fReturnInsertLocation is TRUE,
//              returns index to insert at
//          If not Found and fReturnInsertLocation is FALSE,
//              returns -1
//
// **********************************************************************
    
int SearchSortedArrayEx(void * const pvData,
                        const DWORD cData,
                        const DWORD cbDatum,
                        void * const pvToFind,
                        PFNSSACOMPARE pfnCompare,
                        const LPARAM lp,
                        BOOL fReturnInsertLocation,
                        BOOL *pfFound);

#ifdef __cplusplus
};
#endif // __cplusplus
