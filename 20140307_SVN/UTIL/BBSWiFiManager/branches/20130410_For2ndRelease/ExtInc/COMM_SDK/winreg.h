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
/*++ BUILD Version: 0001    // Increment this if a change has global effects


Module Name:

    Winreg.h

Abstract:

    This module contains the function prototypes and constant, type and
    structure definitions for the Windows 32-Bit Registry API.

--*/

#ifndef _WINREG_
#define _WINREG_


#ifdef _MAC
#include <macwin32.h>
#endif

// @CESYSGEN IF FILESYS_FSREG || FILESYS_FSREGHIVE || FILESYS_FSRGLITE

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINVER
#define WINVER 0x0500   // version 5.0
#endif /* !WINVER */

//
// Requested Key access mask type.
//

typedef ACCESS_MASK REGSAM;

//
// Reserved Key Handles.
//

#define HKEY_CLASSES_ROOT           (( HKEY ) (ULONG_PTR)0x80000000 )
#define HKEY_CURRENT_USER           (( HKEY ) (ULONG_PTR)0x80000001 )
#define HKEY_LOCAL_MACHINE          (( HKEY ) (ULONG_PTR)0x80000002 )
#define HKEY_USERS                  (( HKEY ) (ULONG_PTR)0x80000003 )

//
// Default values for parameters that do not exist in the Win 3.1
// compatible APIs.
//

#define WIN31_CLASS                 NULL

//
// API Prototypes.
//


WINADVAPI
LONG
APIENTRY
RegCloseKey (
    __in_opt HKEY hKey
    );

WINADVAPI
LONG
APIENTRY
RegCreateKeyExA (
    __in_opt HKEY hKey,
    __in LPCSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPSTR lpClass,
      DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition
    );
WINADVAPI
LONG
APIENTRY
RegCreateKeyExW (
    __in_opt HKEY hKey,
    __in LPCWSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPWSTR lpClass,
      DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition
    );
#ifdef UNICODE
#define RegCreateKeyEx  RegCreateKeyExW
#else
#define RegCreateKeyEx  RegCreateKeyExA
#endif // !UNICODE

WINADVAPI
LONG
APIENTRY
RegDeleteKeyA (
    __in_opt HKEY hKey,
    __in LPCSTR lpSubKey
    );
WINADVAPI
LONG
APIENTRY
RegDeleteKeyW (
    __in_opt HKEY hKey,
    __in LPCWSTR lpSubKey
    );
#ifdef UNICODE
#define RegDeleteKey  RegDeleteKeyW
#else
#define RegDeleteKey  RegDeleteKeyA
#endif // !UNICODE

WINADVAPI
LONG
APIENTRY
RegDeleteValueA (
    __in_opt HKEY hKey,
    __in_opt LPCSTR lpValueName
    );
WINADVAPI
LONG
APIENTRY
RegDeleteValueW (
    __in_opt HKEY hKey,
    __in_opt LPCWSTR lpValueName
    );
#ifdef UNICODE
#define RegDeleteValue  RegDeleteValueW
#else
#define RegDeleteValue  RegDeleteValueA
#endif // !UNICODE

WINADVAPI
LONG
APIENTRY
RegEnumKeyExA (
    __in_opt HKEY hKey,
      DWORD dwIndex,
    __out_ecount(*lpcchName) __opt LPSTR lpName,
    __inout LPDWORD lpcchName,
    __reserved LPDWORD lpReserved,
    __inout_ecount(*lpcchClass) __opt LPSTR lpClass,
    __inout __opt LPDWORD lpcchClass,
    __reserved PFILETIME lpftLastWriteTime
    );
WINADVAPI
LONG
APIENTRY
RegEnumKeyExW (
    __in_opt HKEY hKey,
      DWORD dwIndex,
    __out_ecount(*lpcchName) __opt LPWSTR lpName,
    __inout LPDWORD lpcchName,
    __reserved LPDWORD lpReserved,
    __inout_ecount(*lpcchClass) __opt LPWSTR lpClass,
    __inout __opt LPDWORD lpcchClass,
    __reserved PFILETIME lpftLastWriteTime
    );
#ifdef UNICODE
#define RegEnumKeyEx  RegEnumKeyExW
#else
#define RegEnumKeyEx  RegEnumKeyExA
#endif // !UNICODE

WINADVAPI
LONG
APIENTRY
RegEnumValueA (
    __in_opt HKEY hKey,
      DWORD dwIndex,
    __out_ecount(*lpcchValueName) __opt LPSTR lpValueName,
    __inout LPDWORD lpcchValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount(*lpcbData) __opt LPBYTE lpData,
     __opt LPDWORD lpcbData
    );
WINADVAPI
LONG
APIENTRY
RegEnumValueW (
    __in_opt HKEY hKey,
      DWORD dwIndex,
    __out_ecount(*lpcchValueName) __opt LPWSTR lpValueName,
    __inout LPDWORD lpcchValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount(*lpcbData) __opt LPBYTE lpData,
    __opt LPDWORD lpcbData
    );
#ifdef UNICODE
#define RegEnumValue  RegEnumValueW
#else
#define RegEnumValue  RegEnumValueA
#endif // !UNICODE

WINADVAPI
LONG
APIENTRY
RegFlushKey (
    __in HKEY hKey
    );

WINADVAPI
LONG
APIENTRY
RegOpenKeyExA (
    __in_opt HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    );
WINADVAPI
LONG
APIENTRY
RegOpenKeyExW (
    __in_opt HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    );
#ifdef UNICODE
#define RegOpenKeyEx  RegOpenKeyExW
#else
#define RegOpenKeyEx  RegOpenKeyExA
#endif // !UNICODE

WINADVAPI
LONG
APIENTRY
RegQueryInfoKeyA (
    __in_opt HKEY hKey,
    __out_ecount(*lpcchClass) __opt LPSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpcSubKeys,
    __out_opt LPDWORD lpcbMaxSubKeyLen,
    __out_opt LPDWORD lpcbMaxClassLen,
    __out_opt LPDWORD lpcValues,
    __out_opt LPDWORD lpcbMaxValueNameLen,
    __out_opt LPDWORD lpcbMaxValueLen,
    __reserved LPDWORD lpcbSecurityDescriptor,
    __reserved PFILETIME lpftLastWriteTime
    );
WINADVAPI
LONG
APIENTRY
RegQueryInfoKeyW (
    __in_opt HKEY hKey,
    __out_ecount(*lpcchClass) __opt LPWSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpcSubKeys,
    __out_opt LPDWORD lpcbMaxSubKeyLen,
    __out_opt LPDWORD lpcbMaxClassLen,
    __out_opt LPDWORD lpcValues,
    __out_opt LPDWORD lpcbMaxValueNameLen,
    __out_opt LPDWORD lpcbMaxValueLen,
    __reserved LPDWORD lpcbSecurityDescriptor,
    __reserved PFILETIME lpftLastWriteTime
    );
#ifdef UNICODE
#define RegQueryInfoKey  RegQueryInfoKeyW
#else
#define RegQueryInfoKey  RegQueryInfoKeyA
#endif // !UNICODE

WINADVAPI
LONG
APIENTRY
RegQueryValueExA (
    __in_opt HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount(*lpcbData) __opt LPBYTE lpData,
     __opt LPDWORD lpcbData
    );
WINADVAPI
LONG
APIENTRY
RegQueryValueExW (
    __in_opt HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount(*lpcbData) __opt LPBYTE lpData,
    __opt LPDWORD lpcbData
    );
#ifdef UNICODE
#define RegQueryValueEx  RegQueryValueExW
#else
#define RegQueryValueEx  RegQueryValueExA
#endif // !UNICODE

WINADVAPI
LONG
APIENTRY
RegSetValueExA (
    __in_opt HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved DWORD Reserved,
     DWORD dwType,
    __in_bcount(cbData) __opt CONST BYTE* lpData,
     DWORD cbData
    );

WINADVAPI
LONG
APIENTRY
RegSetValueExW (
    __in_opt HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved DWORD Reserved,
     DWORD dwType,
    __in_bcount(cbData) __opt CONST BYTE* lpData,
     DWORD cbData
    );
#ifdef UNICODE
#define RegSetValueEx  RegSetValueExW
#else
#define RegSetValueEx  RegSetValueExA
#endif // !UNICODE

WINADVAPI
HANDLE  
APIENTRY
CeFindFirstRegChange(
    __in HKEY hKey, 
    __in BOOL bWatchSubTree, 
    __in_opt DWORD dwNotifyFilter
    );

WINADVAPI
BOOL    
APIENTRY
CeFindNextRegChange(
    __in HANDLE hNotify
    );

WINADVAPI
BOOL    
APIENTRY
CeFindCloseRegChange(
    __in HANDLE hNotify
    );


#define CeRegTestSetValue CeRegTestSetValueW    

#define REG_FLAGS_TESTSET_NEW 0x1
#define REG_FLAGS_TESTSET_NOMATCH 0x2

WINADVAPI
LONG
APIENTRY
CeRegTestSetValueW(
    __in HKEY hKey, 
    __in LPCWSTR lpValueName, 
     DWORD dwType, 
    __in_bcount(cbOldData) __opt CONST BYTE* lpOldData, 
     DWORD cbOldData,
    __in_bcount(cbNewData) __opt CONST BYTE* lpNewData, 
     DWORD cbNewData,
     DWORD dwFlags
    );                      


#define CE_REG_INFO_FLAG_TRUST_PROTECTED        0x1

typedef struct _CE_REGISTRY_INFO {
    DWORD cbSize; // set to sizeof(FS_REGISTRY_INFO)
    HKEY hRootKey; // Root key of the registry HKLM, HKCU
    DWORD dwFlags;
    TCHAR *pszFullKeyName;
    LPDWORD pdwKeyNameLen;
} CE_REGISTRY_INFO, *PCE_REGISTRY_INFO;

WINADVAPI
LONG
APIENTRY
CeRegGetInfo(
    __in HKEY hKey, 
    __inout PCE_REGISTRY_INFO pInfo 
    );                      


typedef struct _REG_NOTIFY_INFORMATION {
    DWORD NextEntryOffset;
    DWORD Action;
    DWORD RegNameLength;
    WCHAR RegName[1];
} REG_NOTIFY_INFORMATION, *PREG_NOTIFY_INFORMATION;

WINADVAPI
LONG
APIENTRY
CeRegGetNotificationInfo(
    __in HANDLE hChangeHandle,
    __in DWORD dwFlags, 
    __inout_bcount(nBufferLength) __opt LPVOID lpBuffer, 
    __in DWORD nBufferLength, 
    __out LPDWORD lpBytesReturned, 
    __out LPDWORD lpBytesAvailable
    );


#ifdef __cplusplus
}
#endif

#ifdef WINCEOEM
#include <pwinreg.h>
#ifdef WINCEMACRO
#include <mwinreg.h>    // internal defines 
#endif
#endif

// @CESYSGEN ENDIF

#endif // _WINREG_

