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
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

ppwinbase.h

Abstract:

Private portion of winbase.h

Notes: 


--*/

#ifndef _PRIV_WINBASE_H__
#define _PRIV_WINBASE_H__

//
// Define API decoration for direct importing of DLL references.
//
    
#include <dbgapi.h>

#ifdef __cplusplus
extern "C" {
#endif
    
// exported from coredll
BOOL AttachDebugger(LPCWSTR dbgname);
BOOL AttachHdstub(LPCWSTR dbgname);
BOOL AttachOsAxsT0(LPCWSTR dbgname);
BOOL AttachOsAxsT1(LPCWSTR dbgname);
BOOL CaptureDumpFileOnDevice(DWORD dwProcessId, DWORD dwThreadId, LPCWSTR pwzExtraFilesPath);
// dwProcessIdOverride is passed from CeReportFault. In this case, dwProcessId and dwThreadId both are -1 to signal
// a ReportFault capture.
BOOL CaptureDumpFileOnDevice2(DWORD dwProcessId, DWORD dwThreadId, LPCWSTR pwzExtraFilesPath, DWORD dwProcessIdOverride);

// Customized version of ReportFault, dwProcessIdOverride is a process in which pep was created.
DWORD APIENTRY CeReportFault(LPEXCEPTION_POINTERS pep, DWORD dwMode, DWORD dwProcessIdOverride);

#define LocalDiscard( h )   LocalReAlloc( (h), 0, LMEM_MOVEABLE )
LPVOID WINAPI RemoteHeapAlloc(HANDLE hProc, HANDLE hHeap, DWORD dwFlags, DWORD dwBytes);
LPVOID WINAPI RemoteHeapReAlloc(HANDLE hProc, HANDLE hHeap, DWORD dwFlags, DWORD dwBytes, LPVOID lpMem);
BOOL WINAPI RemoteHeapFree(HANDLE hProc, HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
DWORD WINAPI RemoteHeapSize(HANDLE hProc, HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
HLOCAL WINAPI RemoteLocalAlloc( UINT uFlags, UINT uBytes );
HLOCAL WINAPI RemoteLocalReAlloc( HLOCAL hMem, UINT uBytes, UINT uFlags );
UINT WINAPI RemoteLocalSize( HLOCAL hMem ); 
HLOCAL WINAPI RemoteLocalFree( HLOCAL hMem );
UINT WINAPI LocalSizeInProcess( HLOCAL hMem, HANDLE    hProc ); 
HLOCAL WINAPI LocalAllocInProcess( UINT uFlags, UINT uBytes, HANDLE hProc ); 
HLOCAL WINAPI LocalFreeInProcess( HLOCAL hMem, HANDLE hProc ); 
VOID WINAPI SystemMemoryLow(void);
BOOL IsSystemFile(LPCWSTR lpszFileName);
BOOL MatchesWildcardMask(DWORD lenWildcardMask, PCWSTR pchWildcardMask, DWORD lenTarget, PCWSTR pchTarget);
BOOL SecureWipeAllVolumes();

// exported from the filesystem
void FileSystemPowerFunction (DWORD flags);
void CloseAllFileHandles ();
// @CESYSGEN IF FILESYS_FSPASS
BOOL SetPassword (LPWSTR lpszOldpassword, LPWSTR lspzNewPassword);
BOOL GetPasswordActive(void);
BOOL SetPasswordActive(BOOL bActive, LPWSTR lpszPassword);
BOOL SetPasswordStatus(DWORD dwStatus, LPWSTR lpszPassword);
DWORD GetPasswordStatus(void);
// Password status flags
#define PASSWORD_STATUS_ACTIVE              1
#define PASSWORD_STATUS_SCREENSAVERPROTECT  2
// @CESYSGEN ENDIF
BOOL ReadFileWithSeek(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped, DWORD dwLowOffset, DWORD dwHighOffset);
BOOL WriteFileWithSeek(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, PDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped, DWORD dwLowOffset, DWORD dwHighOffset);

// exported from device
HANDLE CreateDeviceHandle (LPCWSTR, DWORD, DWORD, HPROCESS);
BOOL GetDeviceByIndex(DWORD dwIndex, LPWIN32_FIND_DATA lpFindFileData);

#define FILE_SHARE_WRITE_OVERRIDE   0x40000000

#ifdef __cplusplus
}
#endif

#endif /* _PRIV_WINBASE_H__ */
