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
// MRU.H - Most Recently Used List
//
// this was broken out from pcommctr.h

#pragma once

#ifdef _WIN32

#ifdef __cplusplus
extern "C" {
#endif

//===================================================================
typedef int (CALLBACK *MRUCMPPROCA)(LPCSTR, LPCSTR);
typedef int (CALLBACK *MRUCMPPROCW)(LPCWSTR, LPCWSTR);

#ifdef UNICODE
#define MRUCMPPROC              MRUCMPPROCW
#else
#define MRUCMPPROC              MRUCMPPROCA
#endif

// NB This is cdecl - to be compatible with the crts.
typedef int (cdecl FAR *MRUCMPDATAPROC)(const void FAR *, const void FAR *,
                                        size_t);



typedef struct _MRUINFOA {
    DWORD cbSize;
    UINT uMax;
    UINT fFlags;
    HKEY hKey;
    LPCSTR lpszSubKey;
    MRUCMPPROCA lpfnCompare;
} MRUINFOA, FAR *LPMRUINFOA;

typedef struct _MRUINFOW {
    DWORD cbSize;
    UINT uMax;
    UINT fFlags;
    HKEY hKey;
    LPCWSTR lpszSubKey;
    MRUCMPPROCW lpfnCompare;
} MRUINFOW, FAR *LPMRUINFOW;

typedef struct _MRUDATAINFOA {
    DWORD cbSize;
    UINT uMax;
    UINT fFlags;
    HKEY hKey;
    LPCSTR lpszSubKey;
    MRUCMPDATAPROC lpfnCompare;
} MRUDATAINFOA, FAR *LPMRUDATAINFOA;

typedef struct _MRUDATAINFOW {
    DWORD cbSize;
    UINT uMax;
    UINT fFlags;
    HKEY hKey;
    LPCWSTR lpszSubKey;
    MRUCMPDATAPROC lpfnCompare;
} MRUDATAINFOW, FAR *LPMRUDATAINFOW;


#ifdef UNICODE
#define MRUINFO                 MRUINFOW
#define LPMRUINFO               LPMRUINFOW
#define MRUDATAINFO             MRUDATAINFOW
#define LPMRUDATAINFO           LPMRUDATAINFOW
#else
#define MRUINFO                 MRUINFOA
#define LPMRUINFO               LPMRUINFOA
#define MRUDATAINFO             MRUDATAINFOA
#define LPMRUDATAINFO           LPMRUDATAINFOA
#endif

#define MRU_BINARY              0x0001
#define MRU_CACHEWRITE          0x0002
#define MRU_ANSI                0x0004

//
// Define API decoration for direct importing of DLL references.
//
#ifndef MRUAPI
#if !defined(_MRU_)
#define MRUAPI  DECLSPEC_IMPORT
#else
#define MRUAPI
#endif
#endif // MRUAPI

HANDLE WINAPI CreateMRUListA(LPMRUINFOA lpmi);
MRUAPI HANDLE WINAPI CreateMRUListW(LPMRUINFOW lpmi);
MRUAPI void   WINAPI FreeMRUList(HANDLE hMRU);
MRUAPI int    WINAPI AddMRUStringA(HANDLE hMRU, LPCSTR szString);
MRUAPI int    WINAPI AddMRUStringW(HANDLE hMRU, LPCWSTR szString);
MRUAPI int    WINAPI DelMRUString(HANDLE hMRU, int nItem);
MRUAPI int    WINAPI FindMRUStringA(HANDLE hMRU, LPCSTR szString, LPINT lpiSlot);
MRUAPI int    WINAPI FindMRUStringW(HANDLE hMRU, LPCWSTR szString, LPINT lpiSlot);
MRUAPI int    WINAPI EnumMRUListA(HANDLE hMRU, int nItem, LPVOID lpData, UINT uLen);
MRUAPI int    WINAPI EnumMRUListW(HANDLE hMRU, int nItem, LPVOID lpData, UINT uLen);

MRUAPI int    WINAPI AddMRUData(HANDLE hMRU, const void FAR *lpData, UINT cbData);
MRUAPI int    WINAPI FindMRUData(HANDLE hMRU, const void FAR *lpData, UINT cbData,
                          LPINT lpiSlot);

#ifdef UNICODE
#define CreateMRUList           CreateMRUListW
#define AddMRUString            AddMRUStringW
#define FindMRUString           FindMRUStringW
#define EnumMRUList             EnumMRUListW
#else
#define CreateMRUList           CreateMRUListA
#define AddMRUString            AddMRUStringA
#define FindMRUString           FindMRUStringA
#define EnumMRUList             EnumMRUListA
#endif

MRUAPI UINT WINAPI PopulateComboWithMRU(HANDLE hMRU, HWND hwndCombo, UINT cItemsWant);

MRUAPI UINT WINAPI PopulateMenuWithMRU(HANDLE hMRU, HMENU hmenu, UINT cItemsWant);

#ifdef __cplusplus
}
#endif

#endif
