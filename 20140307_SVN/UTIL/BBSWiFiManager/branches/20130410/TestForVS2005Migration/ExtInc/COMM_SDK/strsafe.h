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
/******************************************************************
*                                                                 *
*  strsafe.h -- This module defines safer C library string        *
*               routine replacements. These are meant to make C   *
*               a bit more safe in reference to security and      *
*               robustness                                        *
*                                                                 *
*                                                                 *
******************************************************************/
#ifndef _STRSAFE_H_INCLUDED_
#define _STRSAFE_H_INCLUDED_
#pragma once

#include <stdio.h>      // for _vsnprintf, _vsnwprintf, getc, getwc
#include <string.h>     // for memset
#include <stdarg.h>     // for va_start, etc.

#include <specstrings.h>

#ifndef _SIZE_T_DEFINED
#ifdef  _WIN64
typedef unsigned __int64    size_t;
#else
typedef __w64 unsigned int  size_t;
#endif  // !_WIN64
#define _SIZE_T_DEFINED
#endif  // !_SIZE_T_DEFINED

#if !defined(_WCHAR_T_DEFINED) && !defined(_NATIVE_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

// define types for char and wchar string arrays
typedef __readableTo(sentinel(0)) wchar_t  *PWSTR;
typedef __readableTo(sentinel(0)) char  *PSTR;
typedef __readableTo(sentinel(0)) const wchar_t  *PCWSTR;
typedef __readableTo(sentinel(0)) const char  *PCSTR;


#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif // !_HRESULT_DEFINED

#ifndef SUCCEEDED
#define SUCCEEDED(hr)  ((HRESULT)(hr) >= 0)
#endif

#ifndef FAILED
#define FAILED(hr)  ((HRESULT)(hr) < 0)
#endif

#ifndef S_OK
#define S_OK  ((HRESULT)0x00000000L)
#endif

#ifdef __cplusplus
#define _STRSAFE_EXTERN_C    extern "C"
#else
#define _STRSAFE_EXTERN_C    extern
#endif

#ifdef UNDER_CE
#define _STRSAFE_CALL_CONVENTION
#else
#define _STRSAFE_CALL_CONVENTION    __stdcall
#endif

// If you do not want to use these functions inline (and instead want to link w/ strsafe.lib), then
// #define STRSAFE_LIB before including this header file.
#if defined(STRSAFE_LIB)
#define STRSAFEAPI  _STRSAFE_EXTERN_C HRESULT _STRSAFE_CALL_CONVENTION
#ifndef UNDER_CE
#pragma comment(lib, "strsafe.lib")
#endif
#elif defined(STRSAFE_LIB_IMPL)
#define STRSAFEAPI  _STRSAFE_EXTERN_C HRESULT _STRSAFE_CALL_CONVENTION
#else
#define STRSAFEAPI  __inline HRESULT _STRSAFE_CALL_CONVENTION
#define STRSAFE_INLINE
#endif

// Some functions always run inline because they use stdin and we want to avoid building multiple
// versions of strsafe lib depending on if you use msvcrt, libcmt, etc.
#define STRSAFE_INLINE_API  __inline HRESULT _STRSAFE_CALL_CONVENTION

// The user can request no "Cb" or no "Cch" functions, but not both!
#if defined(STRSAFE_NO_CB_FUNCTIONS) && defined(STRSAFE_NO_CCH_FUNCTIONS)
#error cannot specify both STRSAFE_NO_CB_FUNCTIONS and STRSAFE_NO_CCH_FUNCTIONS !!
#endif

// The user can request no "A" (ASCII) or no "W" (Wide) functions, but not both!
#if defined(STRSAFE_NO_A_FUNCTIONS) && defined(STRSAFE_NO_W_FUNCTIONS)
#error cannot specify both STRSAFE_NO_A_FUNCTIONS and STRSAFE_NO_W_FUNCTIONS !!
#endif

// This should only be defined when we are building strsafe.lib
#ifdef STRSAFE_LIB_IMPL
#define STRSAFE_INLINE
#endif


#define STRSAFE_MAX_CCH  2147483647 // max # of characters we support (same as INT_MAX)

// STRSAFE error return codes
//
#define STRSAFE_E_INSUFFICIENT_BUFFER       ((HRESULT)0x8007007AL)  // 0x7A = 122L = ERROR_INSUFFICIENT_BUFFER
#define STRSAFE_E_INVALID_PARAMETER         ((HRESULT)0x80070057L)  // 0x57 =  87L = ERROR_INVALID_PARAMETER
#define STRSAFE_E_END_OF_FILE               ((HRESULT)0x80070026L)  // 0x26 =  38L = ERROR_HANDLE_EOF

// Flags for controling the Ex functions
//
//      STRSAFE_FILL_BYTE(0xFF)     0x000000FF  // bottom byte specifies fill pattern
#define STRSAFE_IGNORE_NULLS        0x00000100  // treat null as TEXT("") -- don't fault on NULL buffers
#define STRSAFE_FILL_BEHIND_NULL    0x00000200  // fill in extra space behind the null terminator
#define STRSAFE_FILL_ON_FAILURE     0x00000400  // on failure, overwrite pszDest with fill pattern and null terminate it
#define STRSAFE_NULL_ON_FAILURE     0x00000800  // on failure, set *pszDest = TEXT('\0')
#define STRSAFE_NO_TRUNCATION       0x00001000  // instead of returning a truncated result, copy/append nothing to pszDest and null terminate it

#define STRSAFE_VALID_FLAGS         (0x000000FF | STRSAFE_IGNORE_NULLS | STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE | STRSAFE_NULL_ON_FAILURE | STRSAFE_NO_TRUNCATION)

// helper macro to set the fill character and specify buffer filling
#define STRSAFE_FILL_BYTE(x)        ((unsigned long)((x & 0x000000FF) | STRSAFE_FILL_BEHIND_NULL))
#define STRSAFE_FAILURE_BYTE(x)     ((unsigned long)((x & 0x000000FF) | STRSAFE_FILL_ON_FAILURE))

#define STRSAFE_GET_FILL_PATTERN(dwFlags)  ((int)(dwFlags & 0x000000FF))

// prototypes for the worker functions
#ifdef STRSAFE_INLINE
STRSAFEAPI StringCopyWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszSrc);
STRSAFEAPI StringCopyWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc);
STRSAFEAPI StringCopyExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PCSTR pszSrc, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCopyExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PCWSTR pszSrc, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCopyNWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchSrc);
STRSAFEAPI StringCopyNWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchSrc);
STRSAFEAPI StringCopyNExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PCSTR pszSrc, size_t cchSrc, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCopyNExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PCWSTR pszSrc, size_t cchSrc, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCatWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszSrc);
STRSAFEAPI StringCatWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc);
STRSAFEAPI StringCatExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PCSTR pszSrc, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCatExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PCWSTR pszSrc, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCatNWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchMaxAppend);
STRSAFEAPI StringCatNWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchMaxAppend);
STRSAFEAPI StringCatNExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PCSTR pszSrc, size_t cchMaxAppend, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCatNExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PCWSTR pszSrc, size_t cchMaxAppend, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringVPrintfWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszFormat, va_list argList);
STRSAFEAPI StringVPrintfWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszFormat, va_list argList);
STRSAFEAPI StringVPrintfExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags, PCSTR pszFormat, va_list argList);
STRSAFEAPI StringVPrintfExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags, PCWSTR pszFormat, va_list argList);
STRSAFEAPI StringLengthWorkerA(PCSTR psz, size_t cchMax, size_t* pcch);
STRSAFEAPI StringLengthWorkerW(PCWSTR psz, size_t cchMax, size_t* pcch);
#endif  // STRSAFE_INLINE

#ifdef STRSAFE_WITH_GETS
// these functions are always inline
STRSAFEAPI StringGetsExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringGetsExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags);
#endif

#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchCopy(
    __out_ecount(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest,
    IN  LPCTSTR pszSrc
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strcpy'.
    The size of the destination buffer (in characters) is a parameter and
    this function will not write past the end of this buffer and it will
    ALWAYS null terminate the destination buffer (unless it is zero length).

    This routine is not a replacement for strncpy.  That function will pad the
    destination string with extra null termination characters if the count is
    greater than the length of the source string, and it will fail to null 
    terminate the destination string if the source string length is greater
    than or equal to the count. You can not blindly use this instead of strncpy:
    it is common for code to use it to "patch" strings and you would introduce
    errors if the code started null terminating in the middle of the string.

    This function returns a hresult, and not a pointer.  It returns S_OK
    if the string was copied without truncation and null terminated, otherwise
    it will return a failure code. In failure cases as much of pszSrc will be
    copied to pszDest as possible, and pszDest will be null terminated.

Arguments:

    pszDest     -   destination string

    cchDest     -   size of destination buffer in characters.
                    length must be = (_tcslen(src) + 1) to hold all of the
                    source including the null terminator

    pszSrc      -   source string which must be null terminated

Notes: 
    Behavior is undefined if source and destination strings overlap.
   
    pszDest and pszSrc should not be NULL. See StringCchCopyEx if you require
    the handling of NULL values.

Return Value:

    S_OK        -   if there was source data and it was all copied and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the copy operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

STRSAFEAPI StringCchCopyA(__out_ecount(cchDest) PSTR pszDest, size_t cchDest, PCSTR pszSrc);
STRSAFEAPI StringCchCopyW(__out_ecount(cchDest) PWSTR pszDest, size_t cchDest, PCWSTR pszSrc);
#ifdef UNICODE
#define StringCchCopy  StringCchCopyW
#else
#define StringCchCopy  StringCchCopyA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchCopyA(PSTR pszDest, size_t cchDest, PCSTR pszSrc)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyWorkerA(pszDest, cchDest, pszSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchCopyW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyWorkerW(pszDest, cchDest, pszSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbCopy(
    __out_bcount(cbDest) LPTSTR pszDest,
    __in  size_t cbDest,
    IN  LPCTSTR pszSrc
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strcpy'.
    The size of the destination buffer (in bytes) is a parameter and this
    function will not write past the end of this buffer and it will ALWAYS
    null terminate the destination buffer (unless it is zero length).

    This routine is not a replacement for strncpy.  That function will pad the
    destination string with extra null termination characters if the count is
    greater than the length of the source string, and it will fail to null 
    terminate the destination string if the source string length is greater
    than or equal to the count. You can not blindly use this instead of strncpy:
    it is common for code to use it to "patch" strings and you would introduce
    errors if the code started null terminating in the middle of the string.

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the string was copied without truncation and null terminated, otherwise
    it will return a failure code. In failure cases as much of pszSrc will be
    copied to pszDest as possible, and pszDest will be null terminated.

Arguments:

    pszDest     -   destination string

    cbDest      -   size of destination buffer in bytes.
                    length must be = ((_tcslen(src) + 1) * sizeof(TCHAR)) to
                    hold all of the source including the null terminator

    pszSrc      -   source string which must be null terminated

Notes: 
    Behavior is undefined if source and destination strings overlap.
   
    pszDest and pszSrc should not be NULL.  See StringCbCopyEx if you require 
    the handling of NULL values.

Return Value:

    S_OK        -   if there was source data and it was all copied and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the copy operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

STRSAFEAPI StringCbCopyA(__out_bcount(cbDest) PSTR pszDest, size_t cbDest, PCSTR pszSrc);
STRSAFEAPI StringCbCopyW(__out_bcount(cbDest) PWSTR pszDest, size_t cbDest, PCWSTR pszSrc);
#ifdef UNICODE
#define StringCbCopy  StringCbCopyW
#else
#define StringCbCopy  StringCbCopyA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbCopyA(PSTR pszDest, size_t cbDest, PCSTR pszSrc)
{
    HRESULT hr;
    size_t cchDest;
    
    // convert to count of characters
    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyWorkerA(pszDest, cchDest, pszSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbCopyW(PWSTR pszDest, size_t cbDest, PCWSTR pszSrc)
{
    HRESULT hr;
    size_t cchDest;
    
    // convert to count of characters
    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyWorkerW(pszDest, cchDest, pszSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchCopyEx(
    __out_ecount_opt(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest,
    IN  LPCTSTR pszSrc          OPTIONAL,
    __out_opt  LPTSTR* ppszDestEnd,
    __out_opt  size_t* pcchRemaining,
    IN  DWORD   dwFlags
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strcpy' with
    some additional parameters.  In addition to functionality provided by
    StringCchCopy, this routine also returns a pointer to the end of the
    destination string and the number of characters left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string

    cchDest         -   size of destination buffer in characters.
                        length must be = (_tcslen(pszSrc) + 1) to hold all of
                        the source including the null terminator

    pszSrc          -   source string which must be null terminated

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function copied any data, the result will point to the
                        null termination character

    pcchRemaining   -   if pcchRemaining is non-null, the function will return the
                        number of characters left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT("")).
                    this flag is useful for emulating functions like lstrcpy

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any truncated 
                    string returned when the failure is
                    STRSAFE_E_INSUFFICIENT_BUFFER

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any truncated string
                    returned when the failure is STRSAFE_E_INSUFFICIENT_BUFFER.

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL unless the STRSAFE_IGNORE_NULLS flag
    is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and pszSrc
    may be NULL.  An error may still be returned even though NULLS are ignored
    due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all copied and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the copy operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchCopyExA(__out_ecount_opt(cchDest) PSTR pszDest, size_t cchDest, PCSTR pszSrc, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCchCopyExW(__out_ecount_opt(cchDest) PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCchCopyEx  StringCchCopyExW
#else
#define StringCchCopyEx  StringCchCopyExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchCopyExA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(char) since cchDest < STRSAFE_MAX_CCH and sizeof(char) is 1
        cbDest = cchDest * sizeof(char);

        hr = StringCopyExWorkerA(pszDest, cchDest, cbDest, pszSrc, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchCopyExW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    
    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(wchar_t) since cchDest < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
        cbDest = cchDest * sizeof(wchar_t);

        hr = StringCopyExWorkerW(pszDest, cchDest, cbDest, pszSrc, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbCopyEx(
    __out_bcount_opt(cbDest) LPTSTR  pszDest,
    __in  size_t  cbDest,
    IN  LPCTSTR pszSrc          OPTIONAL,
    __out_opt LPTSTR* ppszDestEnd,
    __out_opt size_t* pcbRemaining,
    IN  DWORD   dwFlags
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strcpy' with
    some additional parameters.  In addition to functionality provided by
    StringCbCopy, this routine also returns a pointer to the end of the 
    destination string and the number of bytes left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string

    cbDest          -   size of destination buffer in bytes.
                        length must be ((_tcslen(pszSrc) + 1) * sizeof(TCHAR)) to 
                        hold all of the source including the null terminator

    pszSrc          -   source string which must be null terminated

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function copied any data, the result will point to the
                        null termination character

    pcbRemaining    -   pcbRemaining is non-null,the function will return the
                        number of bytes left in the destination string, 
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT("")).
                    this flag is useful for emulating functions like lstrcpy

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any truncated 
                    string returned when the failure is
                    STRSAFE_E_INSUFFICIENT_BUFFER

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any truncated string
                    returned when the failure is STRSAFE_E_INSUFFICIENT_BUFFER.

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL unless the STRSAFE_IGNORE_NULLS flag
    is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and pszSrc
    may be NULL.  An error may still be returned even though NULLS are ignored
    due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all copied and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the copy operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbCopyExA(__out_bcount_opt(cbDest) PSTR pszDest, size_t cbDest, PCSTR pszSrc, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pcbRemaining, unsigned long dwFlags);
STRSAFEAPI StringCbCopyExW(__out_bcount_opt(cbDest) PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcbRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCbCopyEx  StringCbCopyExW
#else
#define StringCbCopyEx  StringCbCopyExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbCopyExA(PSTR pszDest, size_t cbDest, PCSTR pszSrc, PSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyExWorkerA(pszDest, cchDest, cbDest, pszSrc, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(char) since cchRemaining < STRSAFE_MAX_CCH and sizeof(char) is 1
            *pcbRemaining = (cchRemaining * sizeof(char)) + (cbDest % sizeof(char));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbCopyExW(PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, PWSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyExWorkerW(pszDest, cchDest, cbDest, pszSrc, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(wchar_t) since cchRemaining < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
            *pcbRemaining = (cchRemaining * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchCopyN(
    __out_ecount(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest,
    __in_ecount(cchSrc)  LPCTSTR pszSrc,
    __in  size_t  cchSrc
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strncpy'.
    The size of the destination buffer (in characters) is a parameter and
    this function will not write past the end of this buffer and it will
    ALWAYS null terminate the destination buffer (unless it is zero length).

    This routine is meant as a replacement for strncpy, but it does behave
    differently. This function will not pad the destination buffer with extra
    null termination characters if cchSrc is greater than the length of pszSrc.

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the entire string or the first cchSrc characters were copied without
    truncation and the resultant destination string was null terminated, otherwise
    it will return a failure code. In failure cases as much of pszSrc will be
    copied to pszDest as possible, and pszDest will be null terminated.

Arguments:

    pszDest     -   destination string

    cchDest     -   size of destination buffer in characters.
                    length must be = (_tcslen(src) + 1) to hold all of the
                    source including the null terminator

    pszSrc      -   source string

    cchSrc      -   maximum number of characters to copy from source string

Notes: 
    Behavior is undefined if source and destination strings overlap.
   
    pszDest and pszSrc should not be NULL. See StringCchCopyNEx if you require
    the handling of NULL values.

Return Value:

    S_OK        -   if there was source data and it was all copied and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the copy operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

STRSAFEAPI StringCchCopyNA(__out_ecount(cchDest) PSTR pszDest, size_t cchDest, __in_ecount(cchSrc)  PCSTR pszSrc, size_t cchSrc);
STRSAFEAPI StringCchCopyNW(__out_ecount(cchDest) PWSTR pszDest, size_t cchDest, __in_ecount(cchSrc)  PCWSTR pszSrc, size_t cchSrc);
#ifdef UNICODE
#define StringCchCopyN  StringCchCopyNW
#else
#define StringCchCopyN  StringCchCopyNA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchCopyNA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchSrc)
{
    HRESULT hr;

    if ((cchDest > STRSAFE_MAX_CCH) ||
        (cchSrc > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyNWorkerA(pszDest, cchDest, pszSrc, cchSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchCopyNW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchSrc)
{
    HRESULT hr;

    if ((cchDest > STRSAFE_MAX_CCH) || 
        (cchSrc > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyNWorkerW(pszDest, cchDest, pszSrc, cchSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbCopyN(
    __out_bcount(cbDest) LPTSTR  pszDest,
    __in  size_t  cbDest,
    __in_bcount(cbSrc)  LPCTSTR pszSrc,
    __in  size_t  cbSrc
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strncpy'.
    The size of the destination buffer (in bytes) is a parameter and this
    function will not write past the end of this buffer and it will ALWAYS
    null terminate the destination buffer (unless it is zero length).

    This routine is meant as a replacement for strncpy, but it does behave
    differently. This function will not pad the destination buffer with extra
    null termination characters if cbSrc is greater than the size of pszSrc.

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the entire string or the first cbSrc characters were copied without
    truncation and the resultant destination string was null terminated, otherwise
    it will return a failure code. In failure cases as much of pszSrc will be
    copied to pszDest as possible, and pszDest will be null terminated.

Arguments:

    pszDest     -   destination string

    cbDest      -   size of destination buffer in bytes.
                    length must be = ((_tcslen(src) + 1) * sizeof(TCHAR)) to
                    hold all of the source including the null terminator

    pszSrc      -   source string

    cbSrc       -   maximum number of bytes to copy from source string

Notes: 
    Behavior is undefined if source and destination strings overlap.
   
    pszDest and pszSrc should not be NULL.  See StringCbCopyEx if you require 
    the handling of NULL values.

Return Value:

    S_OK        -   if there was source data and it was all copied and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the copy operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

STRSAFEAPI StringCbCopyNA(__out_bcount(cbDest) PSTR pszDest, size_t cbDest, __in_bcount(cbSrc)  PCSTR pszSrc, size_t cbSrc);
STRSAFEAPI StringCbCopyNW(__out_bcount(cbDest) PWSTR pszDest, size_t cbDest, __in_bcount(cbSrc)  PCWSTR pszSrc, size_t cbSrc);
#ifdef UNICODE
#define StringCbCopyN  StringCbCopyNW
#else
#define StringCbCopyN  StringCbCopyNA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbCopyNA(PSTR pszDest, size_t cbDest, PCSTR pszSrc, size_t cbSrc)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchSrc;

    // convert to count of characters
    cchDest = cbDest / sizeof(char);
    cchSrc = cbSrc / sizeof(char);

    if ((cchDest > STRSAFE_MAX_CCH) || 
        (cchSrc > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyNWorkerA(pszDest, cchDest, pszSrc, cchSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbCopyNW(PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, size_t cbSrc)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchSrc;
    
    // convert to count of characters
    cchDest = cbDest / sizeof(wchar_t);
    cchSrc = cbSrc / sizeof(wchar_t);

    if ((cchDest > STRSAFE_MAX_CCH) ||
        (cchSrc > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyNWorkerW(pszDest, cchDest, pszSrc, cchSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchCopyNEx(
    __out_ecount_opt(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest,
    __in_ecount_opt(cchSrc)  LPCTSTR pszSrc,
    __in  size_t  cchSrc,
    __out_opt LPTSTR* ppszDestEnd,
    __out_opt size_t* pcchRemaining,
    IN  DWORD   dwFlags
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strncpy' with
    some additional parameters.  In addition to functionality provided by
    StringCchCopyN, this routine also returns a pointer to the end of the
    destination string and the number of characters left in the destination
    string including the null terminator. The flags parameter allows
    additional controls.

    This routine is meant as a replacement for strncpy, but it does behave
    differently. This function will not pad the destination buffer with extra
    null termination characters if cchSrc is greater than the length of pszSrc.

Arguments:

    pszDest         -   destination string

    cchDest         -   size of destination buffer in characters.
                        length must be = (_tcslen(pszSrc) + 1) to hold all of
                        the source including the null terminator

    pszSrc          -   source string

    cchSrc          -   maximum number of characters to copy from the source
                        string

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function copied any data, the result will point to the
                        null termination character

    pcchRemaining   -   if pcchRemaining is non-null, the function will return the
                        number of characters left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT("")).
                    this flag is useful for emulating functions like lstrcpy

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any truncated 
                    string returned when the failure is
                    STRSAFE_E_INSUFFICIENT_BUFFER

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any truncated string
                    returned when the failure is STRSAFE_E_INSUFFICIENT_BUFFER.

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL unless the STRSAFE_IGNORE_NULLS flag
    is specified. If STRSAFE_IGNORE_NULLS is passed, both pszDest and pszSrc
    may be NULL. An error may still be returned even though NULLS are ignored
    due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all copied and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the copy operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchCopyNExA(__out_ecount_opt(cchDest) PSTR pszDest, size_t cchDest, __in_ecount_opt(cchSrc)  PCSTR pszSrc, size_t cchSrc, __out_opt PSTR* ppszDestEnd,
                             __out_opt size_t* pcchRemaining, unsigned long dwFlags);

STRSAFEAPI StringCchCopyNExW(__out_ecount_opt(cchDest) PWSTR pszDest, size_t cchDest, __in_ecount_opt(cchSrc)  PCWSTR pszSrc, size_t cchSrc, __out_opt PWSTR* ppszDestEnd,
                             __out_opt size_t* pcchRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCchCopyNEx  StringCchCopyNExW
#else
#define StringCchCopyNEx  StringCchCopyNExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchCopyNExA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchSrc, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;

    if ((cchDest > STRSAFE_MAX_CCH) ||
        (cchSrc > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(char) since cchDest < STRSAFE_MAX_CCH and sizeof(char) is 1
        cbDest = cchDest * sizeof(char);

        hr = StringCopyNExWorkerA(pszDest, cchDest, cbDest, pszSrc, cchSrc, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchCopyNExW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchSrc, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    
    if ((cchDest > STRSAFE_MAX_CCH) ||
        (cchSrc > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(wchar_t) since cchDest < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
        cbDest = cchDest * sizeof(wchar_t);

        hr = StringCopyNExWorkerW(pszDest, cchDest, cbDest, pszSrc, cchSrc, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbCopyNEx(
    __out_bcount_opt(cbDest) LPTSTR  pszDest,
    __in  size_t  cbDest,
    __in_bcount_opt(cbSrc) LPCTSTR pszSrc,
    __in  size_t  cbSrc,
    __out_opt LPTSTR* ppszDestEnd,
    __out_opt size_t* pcbRemaining,
    IN  DWORD   dwFlags
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strncpy' with
    some additional parameters.  In addition to functionality provided by
    StringCbCopyN, this routine also returns a pointer to the end of the 
    destination string and the number of bytes left in the destination string
    including the null terminator. The flags parameter allows additional controls.

    This routine is meant as a replacement for strncpy, but it does behave
    differently. This function will not pad the destination buffer with extra
    null termination characters if cbSrc is greater than the size of pszSrc.

Arguments:

    pszDest         -   destination string

    cbDest          -   size of destination buffer in bytes.
                        length must be ((_tcslen(pszSrc) + 1) * sizeof(TCHAR)) to 
                        hold all of the source including the null terminator

    pszSrc          -   source string

    cbSrc           -   maximum number of bytes to copy from source string

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function copied any data, the result will point to the
                        null termination character

    pcbRemaining    -   pcbRemaining is non-null,the function will return the
                        number of bytes left in the destination string, 
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT("")).
                    this flag is useful for emulating functions like lstrcpy

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any truncated 
                    string returned when the failure is
                    STRSAFE_E_INSUFFICIENT_BUFFER

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any truncated string
                    returned when the failure is STRSAFE_E_INSUFFICIENT_BUFFER.

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL unless the STRSAFE_IGNORE_NULLS flag
    is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and pszSrc
    may be NULL.  An error may still be returned even though NULLS are ignored
    due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all copied and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the copy operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbCopyNExA(__out_bcount_opt(cbDest) PSTR pszDest, size_t cbDest, __in_bcount_opt(cbSrc) PCSTR pszSrc, size_t cbSrc, __out_opt PSTR* ppszDestEnd,
                            __out_opt size_t* pcbRemaining, unsigned long dwFlags);
STRSAFEAPI StringCbCopyNExW(__out_bcount_opt(cbDest) PWSTR pszDest, size_t cbDest, __in_bcount_opt(cbSrc) PCWSTR pszSrc, size_t cbSrc, __out_opt PWSTR* ppszDestEnd,
                            __out_opt size_t* pcbRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCbCopyNEx  StringCbCopyNExW
#else
#define StringCbCopyNEx  StringCbCopyNExA
#endif // !UNICODE


#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbCopyNExA(PSTR pszDest, size_t cbDest, PCSTR pszSrc, size_t cbSrc, PSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchSrc;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(char);
    cchSrc = cbSrc / sizeof(char);

    if ((cchDest > STRSAFE_MAX_CCH) ||
        (cchSrc > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyNExWorkerA(pszDest, cchDest, cbDest, pszSrc, cchSrc, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(char) since cchRemaining < STRSAFE_MAX_CCH and sizeof(char) is 1
            *pcbRemaining = (cchRemaining * sizeof(char)) + (cbDest % sizeof(char));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbCopyNExW(PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, size_t cbSrc, PWSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchSrc;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(wchar_t);
    cchSrc = cbSrc / sizeof(wchar_t);

    if ((cchDest > STRSAFE_MAX_CCH) ||
        (cchSrc > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCopyNExWorkerW(pszDest, cchDest, cbDest, pszSrc, cchSrc, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(wchar_t) since cchRemaining < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
            *pcbRemaining = (cchRemaining * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchCat(
    __inout_ecount(cchDest) LPTSTR  pszDest,
    __in     size_t  cchDest,
    IN     LPCTSTR pszSrc
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strcat'.
    The size of the destination buffer (in characters) is a parameter and this
    function will not write past the end of this buffer and it will ALWAYS
    null terminate the destination buffer (unless it is zero length).

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the string was concatenated without truncation and null terminated, otherwise
    it will return a failure code. In failure cases as much of pszSrc will be
    appended to pszDest as possible, and pszDest will be null terminated.

Arguments:

    pszDest     -  destination string which must be null terminated

    cchDest     -  size of destination buffer in characters.
                   length must be = (_tcslen(pszDest) + _tcslen(pszSrc) + 1)
                   to hold all of the combine string plus the null 
                   terminator

    pszSrc      -  source string which must be null terminated

Notes: 
    Behavior is undefined if source and destination strings overlap.
   
    pszDest and pszSrc should not be NULL.  See StringCchCatEx if you require
    the handling of NULL values.

Return Value:

    S_OK        -   if there was source data and it was all concatenated and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchCatA(__inout_ecount(cchDest) PSTR pszDest, size_t cchDest, PCSTR pszSrc);
STRSAFEAPI StringCchCatW(__inout_ecount(cchDest) PWSTR pszDest, size_t cchDest, PCWSTR pszSrc);
#ifdef UNICODE
#define StringCchCat  StringCchCatW
#else
#define StringCchCat  StringCchCatA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchCatA(PSTR pszDest, size_t cchDest, PCSTR pszSrc)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCatWorkerA(pszDest, cchDest, pszSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchCatW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCatWorkerW(pszDest, cchDest, pszSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbCat(
    __inout_bcount(cbDest) LPTSTR  pszDest,
    __in     size_t  cbDest,
    IN     LPCTSTR pszSrc
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strcat'.
    The size of the destination buffer (in bytes) is a parameter and this
    function will not write past the end of this buffer and it will ALWAYS
    null terminate the destination buffer (unless it is zero length).

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the string was concatenated without truncation and null terminated, otherwise
    it will return a failure code. In failure cases as much of pszSrc will be
    appended to pszDest as possible, and pszDest will be null terminated.

Arguments:

    pszDest     -  destination string which must be null terminated

    cbDest      -  size of destination buffer in bytes.
                   length must be = ((_tcslen(pszDest) + _tcslen(pszSrc) + 1) * sizeof(TCHAR)
                   to hold all of the combine string plus the null 
                   terminator

    pszSrc      -  source string which must be null terminated

Notes: 
    Behavior is undefined if source and destination strings overlap.
   
    pszDest and pszSrc should not be NULL.  See StringCbCatEx if you require
    the handling of NULL values.

Return Value:

    S_OK        -   if there was source data and it was all concatenated and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbCatA(__inout_bcount(cbDest) PSTR pszDest, size_t cbDest, PCSTR pszSrc);
STRSAFEAPI StringCbCatW(__inout_bcount(cbDest) PWSTR pszDest, size_t cbDest, PCWSTR pszSrc);
#ifdef UNICODE
#define StringCbCat  StringCbCatW
#else
#define StringCbCat  StringCbCatA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbCatA(PSTR pszDest, size_t cbDest, PCSTR pszSrc)
{
    HRESULT hr;
    size_t cchDest;
    
    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCatWorkerA(pszDest, cchDest, pszSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbCatW(PWSTR pszDest, size_t cbDest, PCWSTR pszSrc)
{
    HRESULT hr;
    size_t cchDest;
    
    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCatWorkerW(pszDest, cchDest, pszSrc);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchCatEx(
    __inout_ecount_opt(cchDest) LPTSTR  pszDest,
    __in     size_t  cchDest,
    IN     LPCTSTR pszSrc          OPTIONAL,
    __out_opt    LPTSTR* ppszDestEnd,
    __out_opt    size_t* pcchRemaining,
    IN     DWORD   dwFlags
    );

Routine Description:
    
    This routine is a safer version of the C built-in function 'strcat' with
    some additional parameters.  In addition to functionality provided by
    StringCchCat, this routine also returns a pointer to the end of the 
    destination string and the number of characters left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string which must be null terminated

    cchDest         -   size of destination buffer in characters
                        length must be (_tcslen(pszDest) + _tcslen(pszSrc) + 1)
                        to hold all of the combine string plus the null
                        terminator.

    pszSrc          -   source string which must be null terminated

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function appended any data, the result will point to the
                        null termination character

    pcchRemaining   -   if pcchRemaining is non-null, the function will return the
                        number of characters left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT("")).
                    this flag is useful for emulating functions like lstrcat

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any pre-existing
                    or truncated string

        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any pre-existing or
                    truncated string

        STRSAFE_NO_TRUNCATION
                    if the function returns STRSAFE_E_INSUFFICIENT_BUFFER, pszDest
                    will not contain a truncated string, it will remain unchanged.

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL unless the STRSAFE_IGNORE_NULLS flag
    is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and pszSrc
    may be NULL.  An error may still be returned even though NULLS are ignored
    due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all concatenated and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchCatExA(__inout_ecount_opt(cchDest) PSTR pszDest, size_t cchDest, PCSTR pszSrc, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCchCatExW(__inout_ecount_opt(cchDest) PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCchCatEx  StringCchCatExW
#else
#define StringCchCatEx  StringCchCatExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchCatExA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(char) since cchDest < STRSAFE_MAX_CCH and sizeof(char) is 1
        cbDest = cchDest * sizeof(char);

        hr = StringCatExWorkerA(pszDest, cchDest, cbDest, pszSrc, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchCatExW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    
    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(wchar_t) since cchDest < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
        cbDest = cchDest * sizeof(wchar_t);

        hr = StringCatExWorkerW(pszDest, cchDest, cbDest, pszSrc, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbCatEx(
    __inout_bcount_opt(cbDest) LPTSTR  pszDest,
    __in     size_t  cbDest,
    IN     LPCTSTR pszSrc          OPTIONAL,
    __out_opt    LPTSTR* ppszDestEnd,
    __out_opt    size_t* pcbRemaining,
    IN     DWORD   dwFlags
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strcat' with
    some additional parameters.  In addition to functionality provided by
    StringCbCat, this routine also returns a pointer to the end of the 
    destination string and the number of bytes left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string which must be null terminated

    cbDest          -   size of destination buffer in bytes.
                        length must be ((_tcslen(pszDest) + _tcslen(pszSrc) + 1) * sizeof(TCHAR) 
                        to hold all of the combine string plus the null
                        terminator.

    pszSrc          -   source string which must be null terminated

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function appended any data, the result will point to the
                        null termination character

    pcbRemaining    -   if pcbRemaining is non-null, the function will return 
                        the number of bytes left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT("")).
                    this flag is useful for emulating functions like lstrcat

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any pre-existing
                    or truncated string

        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any pre-existing or
                    truncated string

        STRSAFE_NO_TRUNCATION
                    if the function returns STRSAFE_E_INSUFFICIENT_BUFFER, pszDest
                    will not contain a truncated string, it will remain unchanged.

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL unless the STRSAFE_IGNORE_NULLS flag
    is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and pszSrc
    may be NULL.  An error may still be returned even though NULLS are ignored
    due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all concatenated and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbCatExA(__inout_bcount_opt(cbDest) PSTR pszDest, size_t cbDest, PCSTR pszSrc, __out_opt  PSTR* ppszDestEnd, __out_opt  size_t* pcbRemaining, unsigned long dwFlags);
STRSAFEAPI StringCbCatExW(__inout_bcount_opt(cbDest) PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, __out_opt  PWSTR* ppszDestEnd, __out_opt  size_t* pcbRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCbCatEx  StringCbCatExW
#else
#define StringCbCatEx  StringCbCatExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbCatExA(PSTR pszDest, size_t cbDest, PCSTR pszSrc, PSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCatExWorkerA(pszDest, cchDest, cbDest, pszSrc, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(char) since cchRemaining < STRSAFE_MAX_CCH and sizeof(char) is 1
            *pcbRemaining = (cchRemaining * sizeof(char)) + (cbDest % sizeof(char));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbCatExW(PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, PWSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCatExWorkerW(pszDest, cchDest, cbDest, pszSrc, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(wchar_t) since cchRemaining < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
            *pcbRemaining = (cchRemaining * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchCatN(
    __inout_ecount(cchDest) LPTSTR  pszDest,
    __in     size_t  cchDest,
    IN     LPCTSTR pszSrc,
    IN     size_t  cchMaxAppend
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strncat'.
    The size of the destination buffer (in characters) is a parameter as well as
    the maximum number of characters to append, excluding the null terminator.
    This function will not write past the end of the destination buffer and it will
    ALWAYS null terminate pszDest (unless it is zero length).

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if all of pszSrc or the first cchMaxAppend characters were appended to the
    destination string and it was null terminated, otherwise it will return a
    failure code. In failure cases as much of pszSrc will be appended to pszDest
    as possible, and pszDest will be null terminated.

Arguments:

    pszDest         -   destination string which must be null terminated

    cchDest         -   size of destination buffer in characters.
                        length must be (_tcslen(pszDest) + min(cchMaxAppend, _tcslen(pszSrc)) + 1)
                        to hold all of the combine string plus the null
                        terminator.

    pszSrc          -   source string

    cchMaxAppend    -   maximum number of characters to append

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL. See StringCchCatNEx if you require
    the handling of NULL values.

Return Value:

    S_OK        -   if all of pszSrc or the first cchMaxAppend characters were 
                    concatenated to pszDest and the resultant dest string was
                    null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchCatNA(__inout_ecount(cchDest) PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchMaxAppend);
STRSAFEAPI StringCchCatNW(__inout_ecount(cchDest) PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchMaxAppend);
#ifdef UNICODE
#define StringCchCatN  StringCchCatNW
#else
#define StringCchCatN  StringCchCatNA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchCatNA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchMaxAppend)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCatNWorkerA(pszDest, cchDest, pszSrc, cchMaxAppend);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchCatNW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchMaxAppend)
{
    HRESULT hr;
    
    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringCatNWorkerW(pszDest, cchDest, pszSrc, cchMaxAppend);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbCatN(
    __inout_bcount(cbDest) LPTSTR  pszDest,
    __in     size_t  cbDest,
    IN     LPCTSTR pszSrc,
    IN     size_t  cbMaxAppend
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strncat'.
    The size of the destination buffer (in bytes) is a parameter as well as
    the maximum number of bytes to append, excluding the null terminator.
    This function will not write past the end of the destination buffer and it will
    ALWAYS null terminate pszDest (unless it is zero length).

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if all of pszSrc or the first cbMaxAppend bytes were appended to the
    destination string and it was null terminated, otherwise it will return a
    failure code. In failure cases as much of pszSrc will be appended to pszDest
    as possible, and pszDest will be null terminated.

Arguments:

    pszDest         -   destination string which must be null terminated

    cbDest          -   size of destination buffer in bytes.
                        length must be ((_tcslen(pszDest) + min(cbMaxAppend / sizeof(TCHAR), _tcslen(pszSrc)) + 1) * sizeof(TCHAR) 
                        to hold all of the combine string plus the null
                        terminator.

    pszSrc          -   source string

    cbMaxAppend     -   maximum number of bytes to append

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL. See StringCbCatNEx if you require
    the handling of NULL values.

Return Value:

    S_OK        -   if all of pszSrc or the first cbMaxAppend bytes were 
                    concatenated to pszDest and the resultant dest string was
                    null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbCatNA(__inout_bcount(cbDest) PSTR pszDest, size_t cbDest, PCSTR pszSrc, size_t cbMaxAppend);
STRSAFEAPI StringCbCatNW(__inout_bcount(cbDest) PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, size_t cbMaxAppend);
#ifdef UNICODE
#define StringCbCatN  StringCbCatNW
#else
#define StringCbCatN  StringCbCatNA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbCatNA(PSTR pszDest, size_t cbDest, PCSTR pszSrc, size_t cbMaxAppend)
{
    HRESULT hr;
    size_t cchDest;

    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cchMaxAppend;
        
        cchMaxAppend = cbMaxAppend / sizeof(char);

        hr = StringCatNWorkerA(pszDest, cchDest, pszSrc, cchMaxAppend);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbCatNW(PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, size_t cbMaxAppend)
{
    HRESULT hr;
    size_t cchDest;

    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cchMaxAppend;

        cchMaxAppend = cbMaxAppend / sizeof(wchar_t);

        hr = StringCatNWorkerW(pszDest, cchDest, pszSrc, cchMaxAppend);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchCatNEx(
    __inout_ecount_opt(cchDest) LPTSTR  pszDest,
    __in     size_t  cchDest,
    IN     LPCTSTR pszSrc          OPTIONAL,
    IN     size_t  cchMaxAppend,
    __out_opt   LPTSTR* ppszDestEnd,
    __out_opt   size_t* pcchRemaining,
    IN     DWORD   dwFlags
    );

Routine Description:
 
    This routine is a safer version of the C built-in function 'strncat', with 
    some additional parameters.  In addition to functionality provided by
    StringCchCatN, this routine also returns a pointer to the end of the 
    destination string and the number of characters left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string which must be null terminated

    cchDest         -   size of destination buffer in characters.
                        length must be (_tcslen(pszDest) + min(cchMaxAppend, _tcslen(pszSrc)) + 1)
                        to hold all of the combine string plus the null
                        terminator.

    pszSrc          -   source string

    cchMaxAppend    -   maximum number of characters to append

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function appended any data, the result will point to the
                        null termination character

    pcchRemaining   -   if pcchRemaining is non-null, the function will return the
                        number of characters left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT(""))

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any pre-existing
                    or truncated string

        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any pre-existing or
                    truncated string

        STRSAFE_NO_TRUNCATION
                    if the function returns STRSAFE_E_INSUFFICIENT_BUFFER, pszDest
                    will not contain a truncated string, it will remain unchanged.

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL unless the STRSAFE_IGNORE_NULLS flag
    is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and pszSrc
    may be NULL.  An error may still be returned even though NULLS are ignored
    due to insufficient space.

Return Value:

    S_OK        -   if all of pszSrc or the first cchMaxAppend characters were 
                    concatenated to pszDest and the resultant dest string was
                    null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchCatNExA(__inout_ecount_opt(cchDest) PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchMaxAppend, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags);
STRSAFEAPI StringCchCatNExW(__inout_ecount_opt(cchDest) PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchMaxAppend, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCchCatNEx  StringCchCatNExW
#else
#define StringCchCatNEx  StringCchCatNExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchCatNExA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchMaxAppend, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(char) since cchDest < STRSAFE_MAX_CCH and sizeof(char) is 1
        cbDest = cchDest * sizeof(char);

        hr = StringCatNExWorkerA(pszDest, cchDest, cbDest, pszSrc, cchMaxAppend, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchCatNExW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchMaxAppend, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    
    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(wchar_t) since cchDest < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
        cbDest = cchDest * sizeof(wchar_t);

        hr = StringCatNExWorkerW(pszDest, cchDest, cbDest, pszSrc, cchMaxAppend, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbCatNEx(
    __inout_bcount_opt(cbDest) LPTSTR  pszDest,
    __in     size_t  cbDest,
    IN     LPCTSTR pszSrc          OPTIONAL,
    IN     size_t  cbMaxAppend,
    __out_opt   LPTSTR* ppszDestEnd,
    __out_opt   size_t* pcchRemaining,
    IN     DWORD   dwFlags
    );

Routine Description:
    
    This routine is a safer version of the C built-in function 'strncat', with 
    some additional parameters.  In addition to functionality provided by
    StringCbCatN, this routine also returns a pointer to the end of the 
    destination string and the number of bytes left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string which must be null terminated

    cbDest          -   size of destination buffer in bytes.
                        length must be ((_tcslen(pszDest) + min(cbMaxAppend / sizeof(TCHAR), _tcslen(pszSrc)) + 1) * sizeof(TCHAR) 
                        to hold all of the combine string plus the null
                        terminator.

    pszSrc          -   source string

    cbMaxAppend     -   maximum number of bytes to append

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function appended any data, the result will point to the
                        null termination character

    pcbRemaining    -   if pcbRemaining is non-null, the function will return the
                        number of bytes left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT(""))

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any pre-existing
                    or truncated string

        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any pre-existing or
                    truncated string

        STRSAFE_NO_TRUNCATION
                    if the function returns STRSAFE_E_INSUFFICIENT_BUFFER, pszDest
                    will not contain a truncated string, it will remain unchanged.

Notes:
    Behavior is undefined if source and destination strings overlap.

    pszDest and pszSrc should not be NULL unless the STRSAFE_IGNORE_NULLS flag
    is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and pszSrc
    may be NULL.  An error may still be returned even though NULLS are ignored
    due to insufficient space.

Return Value:

    S_OK        -   if all of pszSrc or the first cbMaxAppend bytes were 
                    concatenated to pszDest and the resultant dest string was
                    null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbCatNExA(__inout_bcount_opt(cbDest) PSTR pszDest, size_t cbDest, PCSTR pszSrc, size_t cbMaxAppend, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pcbRemaining, unsigned long dwFlags);
STRSAFEAPI StringCbCatNExW(__inout_bcount_opt(cbDest) PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, size_t cbMaxAppend, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcbRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCbCatNEx  StringCbCatNExW
#else
#define StringCbCatNEx  StringCbCatNExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbCatNExA(PSTR pszDest, size_t cbDest, PCSTR pszSrc, size_t cbMaxAppend, PSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cchMaxAppend;
        
        cchMaxAppend = cbMaxAppend / sizeof(char);

        hr = StringCatNExWorkerA(pszDest, cchDest, cbDest, pszSrc, cchMaxAppend, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(char) since cchRemaining < STRSAFE_MAX_CCH and sizeof(char) is 1
            *pcbRemaining = (cchRemaining * sizeof(char)) + (cbDest % sizeof(char));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbCatNExW(PWSTR pszDest, size_t cbDest, PCWSTR pszSrc, size_t cbMaxAppend, PWSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cchMaxAppend;
        
        cchMaxAppend = cbMaxAppend / sizeof(wchar_t);

        hr = StringCatNExWorkerW(pszDest, cchDest, cbDest, pszSrc, cchMaxAppend, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(wchar_t) since cchRemaining < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
            *pcbRemaining = (cchRemaining * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchVPrintf(
    __out_ecount(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest,
    IN  LPCTSTR pszFormat,
    IN  va_list argList
    );

Routine Description:

    This routine is a safer version of the C built-in function 'vsprintf'.
    The size of the destination buffer (in characters) is a parameter and
    this function will not write past the end of this buffer and it will
    ALWAYS null terminate the destination buffer (unless it is zero length).

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the string was printed without truncation and null terminated, otherwise
    it will return a failure code. In failure cases it will return a truncated
    version of the ideal result.

Arguments:

    pszDest     -  destination string

    cchDest     -  size of destination buffer in characters
                   length must be sufficient to hold the resulting formatted
                   string, including the null terminator.

    pszFormat   -  format string which must be null terminated

    argList     -  va_list from the variable arguments according to the
                   stdarg.h convention

Notes: 
    Behavior is undefined if destination, format strings or any arguments
    strings overlap.
   
    pszDest and pszFormat should not be NULL.  See StringCchVPrintfEx if you
    require the handling of NULL values.

Return Value:

    S_OK        -   if there was sufficient space in the dest buffer for
                    the resultant string and it was null terminated.

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the print operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchVPrintfA(__out_ecount(cchDest) PSTR pszDest, size_t cchDest, PCSTR pszFormat, va_list argList);
STRSAFEAPI StringCchVPrintfW(__out_ecount(cchDest) PWSTR pszDest, size_t cchDest, PCWSTR pszFormat, va_list argList);
#ifdef UNICODE
#define StringCchVPrintf  StringCchVPrintfW
#else
#define StringCchVPrintf  StringCchVPrintfA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchVPrintfA(PSTR pszDest, size_t cchDest, PCSTR pszFormat, va_list argList)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringVPrintfWorkerA(pszDest, cchDest, pszFormat, argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchVPrintfW(PWSTR pszDest, size_t cchDest, PCWSTR pszFormat, va_list argList)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringVPrintfWorkerW(pszDest, cchDest, pszFormat, argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbVPrintf(
    __out_bcount(cbDest) LPTSTR  pszDest,
    __in  size_t  cbDest,
    IN  LPCTSTR pszFormat,
    IN  va_list argList
    );

Routine Description:

    This routine is a safer version of the C built-in function 'vsprintf'.
    The size of the destination buffer (in bytes) is a parameter and
    this function will not write past the end of this buffer and it will
    ALWAYS null terminate the destination buffer (unless it is zero length).

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the string was printed without truncation and null terminated, otherwise
    it will return a failure code. In failure cases it will return a truncated
    version of the ideal result.

Arguments:

    pszDest     -  destination string

    cbDest      -  size of destination buffer in bytes
                   length must be sufficient to hold the resulting formatted
                   string, including the null terminator.

    pszFormat   -  format string which must be null terminated

    argList     -  va_list from the variable arguments according to the
                   stdarg.h convention

Notes: 
    Behavior is undefined if destination, format strings or any arguments
    strings overlap.
   
    pszDest and pszFormat should not be NULL.  See StringCbVPrintfEx if you
    require the handling of NULL values.


Return Value:

    S_OK        -   if there was sufficient space in the dest buffer for
                    the resultant string and it was null terminated.

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the print operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbVPrintfA(__out_bcount(cbDest) PSTR pszDest, size_t cbDest, PCSTR pszFormat, va_list argList);
STRSAFEAPI StringCbVPrintfW(__out_bcount(cbDest) PWSTR pszDest, size_t cbDest, PCWSTR pszFormat, va_list argList);
#ifdef UNICODE
#define StringCbVPrintf  StringCbVPrintfW
#else
#define StringCbVPrintf  StringCbVPrintfA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbVPrintfA(PSTR pszDest, size_t cbDest, PCSTR pszFormat, va_list argList)
{
    HRESULT hr;
    size_t cchDest;

    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringVPrintfWorkerA(pszDest, cchDest, pszFormat, argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbVPrintfW(PWSTR pszDest, size_t cbDest, PCWSTR pszFormat, va_list argList)
{
    HRESULT hr;
    size_t cchDest;

    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringVPrintfWorkerW(pszDest, cchDest, pszFormat, argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchPrintf(
    __out_ecount(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest,
    IN  LPCTSTR pszFormat,
    ...
    );

Routine Description:

    This routine is a safer version of the C built-in function 'sprintf'.
    The size of the destination buffer (in characters) is a parameter and
    this function will not write past the end of this buffer and it will
    ALWAYS null terminate the destination buffer (unless it is zero length).

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the string was printed without truncation and null terminated, otherwise
    it will return a failure code. In failure cases it will return a truncated
    version of the ideal result.

Arguments:

    pszDest     -  destination string

    cchDest     -  size of destination buffer in characters
                   length must be sufficient to hold the resulting formatted
                   string, including the null terminator.

    pszFormat   -  format string which must be null terminated

    ...         -  additional parameters to be formatted according to
                   the format string

Notes: 
    Behavior is undefined if destination, format strings or any arguments
    strings overlap.
   
    pszDest and pszFormat should not be NULL.  See StringCchPrintfEx if you
    require the handling of NULL values.

Return Value:

    S_OK        -   if there was sufficient space in the dest buffer for
                    the resultant string and it was null terminated.

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the print operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchPrintfA(__out_ecount(cchDest) PSTR pszDest, size_t cchDest, PCSTR pszFormat, ...);
STRSAFEAPI StringCchPrintfW(__out_ecount(cchDest) PWSTR pszDest, size_t cchDest, PCWSTR pszFormat, ...);
#ifdef UNICODE
#define StringCchPrintf  StringCchPrintfW
#else
#define StringCchPrintf  StringCchPrintfA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchPrintfA(PSTR pszDest, size_t cchDest, PCSTR pszFormat, ...)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        va_list argList;

        va_start(argList, pszFormat);

        hr = StringVPrintfWorkerA(pszDest, cchDest, pszFormat, argList);
        
        va_end(argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchPrintfW(PWSTR pszDest, size_t cchDest, PCWSTR pszFormat, ...)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        va_list argList;

        va_start(argList, pszFormat);

        hr = StringVPrintfWorkerW(pszDest, cchDest, pszFormat, argList);
    
        va_end(argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbPrintf(
    __out_bcount(cbDest) LPTSTR  pszDest,
    __in  size_t  cbDest,
    IN  LPCTSTR pszFormat,
    ...
    );

Routine Description:

    This routine is a safer version of the C built-in function 'sprintf'.
    The size of the destination buffer (in bytes) is a parameter and
    this function will not write past the end of this buffer and it will
    ALWAYS null terminate the destination buffer (unless it is zero length).

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the string was printed without truncation and null terminated, otherwise
    it will return a failure code. In failure cases it will return a truncated
    version of the ideal result.

Arguments:

    pszDest     -  destination string

    cbDest      -  size of destination buffer in bytes
                   length must be sufficient to hold the resulting formatted
                   string, including the null terminator.

    pszFormat   -  format string which must be null terminated

    ...         -  additional parameters to be formatted according to
                   the format string

Notes: 
    Behavior is undefined if destination, format strings or any arguments
    strings overlap.
   
    pszDest and pszFormat should not be NULL.  See StringCbPrintfEx if you
    require the handling of NULL values.


Return Value:

    S_OK        -   if there was sufficient space in the dest buffer for
                    the resultant string and it was null terminated.

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the print operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbPrintfA(__out_bcount(cbDest) PSTR pszDest, size_t cbDest, PCSTR pszFormat, ...);
STRSAFEAPI StringCbPrintfW(__out_bcount(cbDest) PWSTR pszDest, size_t cbDest, PCWSTR pszFormat, ...);
#ifdef UNICODE
#define StringCbPrintf  StringCbPrintfW
#else
#define StringCbPrintf  StringCbPrintfA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbPrintfA(PSTR pszDest, size_t cbDest, PCSTR pszFormat, ...)
{
    HRESULT hr;
    size_t cchDest;
    
    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        va_list argList;

        va_start(argList, pszFormat);
   
        hr = StringVPrintfWorkerA(pszDest, cchDest, pszFormat, argList);

        va_end(argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbPrintfW(PWSTR pszDest, size_t cbDest, PCWSTR pszFormat, ...)
{
    HRESULT hr;
    size_t cchDest;
    
    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        va_list argList;

        va_start(argList, pszFormat);
    
        hr = StringVPrintfWorkerW(pszDest, cchDest, pszFormat, argList);

        va_end(argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchPrintfEx(
    __out_ecount_opt(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest,
    __out_opt LPTSTR* ppszDestEnd,
    __out_opt size_t* pcchRemaining,
    IN  DWORD   dwFlags,
    IN  LPCTSTR pszFormat       OPTIONAL,
    ...
    );

Routine Description:

    This routine is a safer version of the C built-in function 'sprintf' with
    some additional parameters.  In addition to functionality provided by
    StringCchPrintf, this routine also returns a pointer to the end of the 
    destination string and the number of characters left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string

    cchDest         -   size of destination buffer in characters.
                        length must be sufficient to contain the resulting
                        formatted string plus the null terminator.

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function printed any data, the result will point to the
                        null termination character

    pcchRemaining   -   if pcchRemaining is non-null, the function will return 
                        the number of characters left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT(""))

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any truncated 
                    string returned when the failure is
                    STRSAFE_E_INSUFFICIENT_BUFFER

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any truncated string
                    returned when the failure is STRSAFE_E_INSUFFICIENT_BUFFER.

    pszFormat       -   format string which must be null terminated

    ...             -   additional parameters to be formatted according to
                        the format string

Notes:
    Behavior is undefined if destination, format strings or any arguments
    strings overlap.

    pszDest and pszFormat should not be NULL unless the STRSAFE_IGNORE_NULLS
    flag is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and
    pszFormat may be NULL.  An error may still be returned even though NULLS
    are ignored due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all concatenated and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the print operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchPrintfExA(__out_ecount_opt(cchDest) PSTR pszDest, size_t cchDest, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags, PCSTR pszFormat, ...);
STRSAFEAPI StringCchPrintfExW(__out_ecount_opt(cchDest) PWSTR pszDest, size_t cchDest, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags, PCWSTR pszFormat, ...);
#ifdef UNICODE
#define StringCchPrintfEx  StringCchPrintfExW
#else
#define StringCchPrintfEx  StringCchPrintfExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchPrintfExA(PSTR pszDest, size_t cchDest, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags, PCSTR pszFormat, ...)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;
        va_list argList;

        // safe to multiply cchDest * sizeof(char) since cchDest < STRSAFE_MAX_CCH and sizeof(char) is 1
        cbDest = cchDest * sizeof(char);
        va_start(argList, pszFormat);

        hr = StringVPrintfExWorkerA(pszDest, cchDest, cbDest, ppszDestEnd, pcchRemaining, dwFlags, pszFormat, argList);
        
        va_end(argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchPrintfExW(PWSTR pszDest, size_t cchDest, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags, PCWSTR pszFormat, ...)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;
        va_list argList;

        // safe to multiply cchDest * sizeof(wchar_t) since cchDest < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
        cbDest = cchDest * sizeof(wchar_t);
        va_start(argList, pszFormat);

        hr = StringVPrintfExWorkerW(pszDest, cchDest, cbDest, ppszDestEnd, pcchRemaining, dwFlags, pszFormat, argList);
    
        va_end(argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbPrintfEx(
    __out_bcount_opt(cbDest) LPTSTR  pszDest,
    __in  size_t  cbDest,
    __out_opt LPTSTR* ppszDestEnd,
    __out_opt size_t* pcbRemaining,
    IN  DWORD   dwFlags,
    IN  LPCTSTR pszFormat       OPTIONAL,
    ...
    );

Routine Description:

    This routine is a safer version of the C built-in function 'sprintf' with
    some additional parameters.  In addition to functionality provided by
    StringCbPrintf, this routine also returns a pointer to the end of the 
    destination string and the number of bytes left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string

    cbDest          -   size of destination buffer in bytes.
                        length must be sufficient to contain the resulting
                        formatted string plus the null terminator.

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function printed any data, the result will point to the
                        null termination character

    pcbRemaining    -   if pcbRemaining is non-null, the function will return 
                        the number of bytes left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT(""))

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any truncated 
                    string returned when the failure is
                    STRSAFE_E_INSUFFICIENT_BUFFER

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any truncated string
                    returned when the failure is STRSAFE_E_INSUFFICIENT_BUFFER.

    pszFormat       -   format string which must be null terminated

    ...             -   additional parameters to be formatted according to
                        the format string

Notes:
    Behavior is undefined if destination, format strings or any arguments
    strings overlap.

    pszDest and pszFormat should not be NULL unless the STRSAFE_IGNORE_NULLS
    flag is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and
    pszFormat may be NULL.  An error may still be returned even though NULLS
    are ignored due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all concatenated and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the print operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbPrintfExA(__out_bcount_opt(cbDest) PSTR pszDest, size_t cbDest, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pcbRemaining, unsigned long dwFlags, PCSTR pszFormat, ...);
STRSAFEAPI StringCbPrintfExW(__out_bcount_opt(cbDest) PWSTR pszDest, size_t cbDest, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcbRemaining, unsigned long dwFlags, PCWSTR pszFormat, ...);
#ifdef UNICODE
#define StringCbPrintfEx  StringCbPrintfExW
#else
#define StringCbPrintfEx  StringCbPrintfExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbPrintfExA(PSTR pszDest, size_t cbDest, PSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags, PCSTR pszFormat, ...)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        va_list argList;

        va_start(argList, pszFormat);

        hr = StringVPrintfExWorkerA(pszDest, cchDest, cbDest, ppszDestEnd, &cchRemaining, dwFlags, pszFormat, argList);
    
        va_end(argList);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(char) since cchRemaining < STRSAFE_MAX_CCH and sizeof(char) is 1
            *pcbRemaining = (cchRemaining * sizeof(char)) + (cbDest % sizeof(char));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbPrintfExW(PWSTR pszDest, size_t cbDest, PWSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags, PCWSTR pszFormat, ...)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        va_list argList;

        va_start(argList, pszFormat);

        hr = StringVPrintfExWorkerW(pszDest, cchDest, cbDest, ppszDestEnd, &cchRemaining, dwFlags, pszFormat, argList);
    
        va_end(argList);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(wchar_t) since cchRemaining < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
            *pcbRemaining = (cchRemaining * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchVPrintfEx(
    __out_ecount_opt(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest,
    __out_opt LPTSTR* ppszDestEnd,
    __out_opt size_t* pcchRemaining,
    IN  DWORD   dwFlags,
    IN  LPCTSTR pszFormat       OPTIONAL,
    IN  va_list argList
    );

Routine Description:

    This routine is a safer version of the C built-in function 'vsprintf' with
    some additional parameters.  In addition to functionality provided by
    StringCchVPrintf, this routine also returns a pointer to the end of the 
    destination string and the number of characters left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string

    cchDest         -   size of destination buffer in characters.
                        length must be sufficient to contain the resulting
                        formatted string plus the null terminator.

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a 
                        pointer to the end of the destination string.  If the 
                        function printed any data, the result will point to the 
                        null termination character

    pcchRemaining   -   if pcchRemaining is non-null, the function will return
                        the number of characters left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT(""))

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any truncated 
                    string returned when the failure is
                    STRSAFE_E_INSUFFICIENT_BUFFER

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any truncated string
                    returned when the failure is STRSAFE_E_INSUFFICIENT_BUFFER.

    pszFormat       -   format string which must be null terminated

    argList         -   va_list from the variable arguments according to the
                        stdarg.h convention

Notes:
    Behavior is undefined if destination, format strings or any arguments
    strings overlap.

    pszDest and pszFormat should not be NULL unless the STRSAFE_IGNORE_NULLS
    flag is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and
    pszFormat may be NULL.  An error may still be returned even though NULLS
    are ignored due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all concatenated and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the print operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCchVPrintfExA(__out_ecount_opt(cchDest) PSTR pszDest, size_t cchDest, __out_opt PSTR* ppszDestEnd, __out_opt  size_t* pcchRemaining, unsigned long dwFlags, PCSTR pszFormat, va_list argList);
STRSAFEAPI StringCchVPrintfExW(__out_ecount_opt(cchDest) PWSTR pszDest, size_t cchDest, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags, PCWSTR pszFormat, va_list argList);
#ifdef UNICODE
#define StringCchVPrintfEx  StringCchVPrintfExW
#else
#define StringCchVPrintfEx  StringCchVPrintfExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchVPrintfExA(PSTR pszDest, size_t cchDest, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags, PCSTR pszFormat, va_list argList)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(char) since cchDest < STRSAFE_MAX_CCH and sizeof(char) is 1
        cbDest = cchDest * sizeof(char);
        
        hr = StringVPrintfExWorkerA(pszDest, cchDest, cbDest, ppszDestEnd, pcchRemaining, dwFlags, pszFormat, argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchVPrintfExW(PWSTR pszDest, size_t cchDest, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags, PCWSTR pszFormat, va_list argList)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;

        // safe to multiply cchDest * sizeof(wchar_t) since cchDest < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
        cbDest = cchDest * sizeof(wchar_t);

        hr = StringVPrintfExWorkerW(pszDest, cchDest, cbDest, ppszDestEnd, pcchRemaining, dwFlags, pszFormat, argList);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbVPrintfEx(
    __out_bcount_opt(cbDest) LPTSTR  pszDest,
    __in  size_t  cbDest,
    __out_opt LPTSTR* ppszDestEnd,
    __out_opt size_t* pcbRemaining,
    IN  DWORD   dwFlags,
    IN  LPCTSTR pszFormat       OPTIONAL,
    IN  va_list argList
    );

Routine Description:

    This routine is a safer version of the C built-in function 'vsprintf' with
    some additional parameters.  In addition to functionality provided by
    StringCbVPrintf, this routine also returns a pointer to the end of the 
    destination string and the number of characters left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string

    cbDest          -   size of destination buffer in bytes.
                        length must be sufficient to contain the resulting
                        formatted string plus the null terminator.

    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return 
                        a pointer to the end of the destination string.  If the
                        function printed any data, the result will point to the
                        null termination character

    pcbRemaining    -   if pcbRemaining is non-null, the function will return
                        the number of bytes left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT(""))

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated. This will overwrite any truncated 
                    string returned when the failure is
                    STRSAFE_E_INSUFFICIENT_BUFFER

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. This will overwrite any truncated string
                    returned when the failure is STRSAFE_E_INSUFFICIENT_BUFFER.

    pszFormat       -   format string which must be null terminated

    argList         -   va_list from the variable arguments according to the
                        stdarg.h convention

Notes:
    Behavior is undefined if destination, format strings or any arguments
    strings overlap.

    pszDest and pszFormat should not be NULL unless the STRSAFE_IGNORE_NULLS
    flag is specified.  If STRSAFE_IGNORE_NULLS is passed, both pszDest and
    pszFormat may be NULL.  An error may still be returned even though NULLS
    are ignored due to insufficient space.

Return Value:

    S_OK        -   if there was source data and it was all concatenated and the
                    resultant dest string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all falure cases

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that the print operation
                    failed due to insufficient space. When this error occurs,
                    the destination buffer is modified to contain a truncated
                    version of the ideal result and is null terminated. This
                    is useful for situations where truncation is ok.

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function

--*/

STRSAFEAPI StringCbVPrintfExA(__out_bcount_opt(cbDest) PSTR pszDest, size_t cbDest, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pcbRemaining, unsigned long dwFlags, PCSTR pszFormat, va_list argList);
STRSAFEAPI StringCbVPrintfExW(__out_bcount_opt(cbDest) PWSTR pszDest, size_t cbDest, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcbRemaining, unsigned long dwFlags, PCWSTR pszFormat, va_list argList);
#ifdef UNICODE
#define StringCbVPrintfEx  StringCbVPrintfExW
#else
#define StringCbVPrintfEx  StringCbVPrintfExA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbVPrintfExA(PSTR pszDest, size_t cbDest, PSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags, PCSTR pszFormat, va_list argList)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringVPrintfExWorkerA(pszDest, cchDest, cbDest, ppszDestEnd, &cchRemaining, dwFlags, pszFormat, argList);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(char) since cchRemaining < STRSAFE_MAX_CCH and sizeof(char) is 1
            *pcbRemaining = (cchRemaining * sizeof(char)) + (cbDest % sizeof(char));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbVPrintfExW(PWSTR pszDest, size_t cbDest, PWSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags, PCWSTR pszFormat, va_list argList)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;
    
    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringVPrintfExWorkerW(pszDest, cchDest, cbDest, ppszDestEnd, &cchRemaining, dwFlags, pszFormat, argList);
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(wchar_t) since cchRemaining < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
            *pcbRemaining = (cchRemaining * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchGets(
    __out_ecount(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest
    );

Routine Description:

    This routine is a safer version of the C built-in function 'gets'.
    The size of the destination buffer (in characters) is a parameter and
    this function will not write past the end of this buffer and it will
    ALWAYS null terminate the destination buffer (unless it is zero length).

    This routine is not a replacement for fgets.  That function does not replace
    newline characters with a null terminator.

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if any characters were read from stdin and copied to pszDest and pszDest was
    null terminated, otherwise it will return a failure code.

Arguments:

    pszDest     -   destination string

    cchDest     -   size of destination buffer in characters.

Notes: 
    pszDest should not be NULL. See StringCchGetsEx if you require the handling 
    of NULL values.

    cchDest must be > 1 for this function to succeed.

Return Value:

    S_OK        -   data was read from stdin and copied, and the resultant dest
                    string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    STRSAFE_E_END_OF_FILE
                -   this return value indicates an error or end-of-file condition,
                    use feof or ferror to determine which one has occured.

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that there was insufficient
                    space in the destination buffer to copy any data

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

#ifdef STRSAFE_WITH_GETS
STRSAFE_INLINE_API StringCchGetsA(__out_ecount(cchDest) PSTR pszDest, size_t cchDest);
STRSAFE_INLINE_API StringCchGetsW(__out_ecount(cchDest) PWSTR pszDest, size_t cchDest);
#ifdef UNICODE
#define StringCchGets  StringCchGetsW
#else
#define StringCchGets  StringCchGetsA
#endif // !UNICODE

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFE_INLINE_API StringCchGetsA(PSTR pszDest, size_t cchDest)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;
        
        // safe to multiply cchDest * sizeof(char) since cchDest < STRSAFE_MAX_CCH and sizeof(char) is 1
        cbDest = cchDest * sizeof(char);

        hr = StringGetsExWorkerA(pszDest, cchDest, cbDest, NULL, NULL, 0);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFE_INLINE_API StringCchGetsW(PWSTR pszDest, size_t cchDest)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;
        
        // safe to multiply cchDest * sizeof(wchar_t) since cchDest < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
        cbDest = cchDest * sizeof(wchar_t);

        hr = StringGetsExWorkerW(pszDest, cchDest, cbDest, NULL, NULL, 0);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // !STRSAFE_NO_CCH_FUNCTIONS
#endif  // STRSAFE_WITH_GETS

#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbGets(
    __out_bcount(cbDest) LPTSTR  pszDest,
    __in  size_t  cbDest
    );

Routine Description:

    This routine is a safer version of the C built-in function 'gets'.
    The size of the destination buffer (in bytes) is a parameter and
    this function will not write past the end of this buffer and it will
    ALWAYS null terminate the destination buffer (unless it is zero length).

    This routine is not a replacement for fgets.  That function does not replace
    newline characters with a null terminator.

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if any characters were read from stdin and copied to pszDest and pszDest was
    null terminated, otherwise it will return a failure code.

Arguments:

    pszDest     -   destination string

    cbDest      -   size of destination buffer in bytes.

Notes: 
    pszDest should not be NULL. See StringCbGetsEx if you require the handling 
    of NULL values.

    cbDest must be > sizeof(TCHAR) for this function to succeed.

Return Value:

    S_OK        -   data was read from stdin and copied, and the resultant dest
                    string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    STRSAFE_E_END_OF_FILE
                -   this return value indicates an error or end-of-file condition,
                    use feof or ferror to determine which one has occured.

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that there was insufficient
                    space in the destination buffer to copy any data

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

#ifdef STRSAFE_WITH_GETS
STRSAFE_INLINE_API StringCbGetsA(__out_bcount(cbDest) PSTR pszDest, size_t cbDest);
STRSAFE_INLINE_API StringCbGetsW(__out_bcount(cbDest) PWSTR pszDest, size_t cbDest);
#ifdef UNICODE
#define StringCbGets  StringCbGetsW
#else
#define StringCbGets  StringCbGetsA
#endif // !UNICODE

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFE_INLINE_API StringCbGetsA(PSTR pszDest, size_t cbDest)
{
    HRESULT hr;
    size_t cchDest;
    
    // convert to count of characters
    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringGetsExWorkerA(pszDest, cchDest, cbDest, NULL, NULL, 0);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFE_INLINE_API StringCbGetsW(PWSTR pszDest, size_t cbDest)
{
    HRESULT hr;
    size_t cchDest;
    
    // convert to count of characters
    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringGetsExWorkerW(pszDest, cchDest, cbDest, NULL, NULL, 0);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // !STRSAFE_NO_CB_FUNCTIONS
#endif  // STRSAFE_WITH_GETS

#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchGetsEx(
    __out_ecount_opt(cchDest) LPTSTR  pszDest,
    __in  size_t  cchDest,
    __out_opt LPTSTR* ppszDestEnd,
    __out_opt size_t* pcchRemaining,
    IN  DWORD   dwFlags
    );

Routine Description:

    This routine is a safer version of the C built-in function 'gets' with
    some additional parameters. In addition to functionality provided by
    StringCchGets, this routine also returns a pointer to the end of the
    destination string and the number of characters left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string

    cchDest         -   size of destination buffer in characters.
                     
    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function copied any data, the result will point to the
                        null termination character

    pcchRemaining   -   if pcchRemaining is non-null, the function will return the
                        number of characters left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT("")).

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated.

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. 
                    
Notes:
    pszDest should not be NULL unless the STRSAFE_IGNORE_NULLS flag is specified.
    If STRSAFE_IGNORE_NULLS is passed and pszDest is NULL, an error may still be 
    returned even though NULLS are ignored

    cchDest must be > 1 for this function to succeed.

Return Value:

    S_OK        -   data was read from stdin and copied, and the resultant dest
                    string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    STRSAFE_E_END_OF_FILE
                -   this return value indicates an error or end-of-file condition,
                    use feof or ferror to determine which one has occured.

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that there was insufficient
                    space in the destination buffer to copy any data

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

#ifdef STRSAFE_WITH_GETS
STRSAFE_INLINE_API StringCchGetsExA(__out_ecount_opt(cchDest) PSTR pszDest, size_t cchDest, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags);
STRSAFE_INLINE_API StringCchGetsExW(__out_ecount_opt(cchDest) PWSTR pszDest, size_t cchDest, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcchRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCchGetsEx  StringCchGetsExW
#else
#define StringCchGetsEx  StringCchGetsExA
#endif // !UNICODE

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFE_INLINE_API StringCchGetsExA(PSTR pszDest, size_t cchDest, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;
        
        // safe to multiply cchDest * sizeof(char) since cchDest < STRSAFE_MAX_CCH and sizeof(char) is 1
        cbDest = cchDest * sizeof(char);

        hr = StringGetsExWorkerA(pszDest, cchDest, cbDest, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFE_INLINE_API StringCchGetsExW(PWSTR pszDest, size_t cchDest, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cbDest;
        
        // safe to multiply cchDest * sizeof(wchar_t) since cchDest < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
        cbDest = cchDest * sizeof(wchar_t);

        hr = StringGetsExWorkerW(pszDest, cchDest, cbDest, ppszDestEnd, pcchRemaining, dwFlags);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // !STRSAFE_NO_CCH_FUNCTIONS
#endif  // STRSAFE_WITH_GETS

#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbGetsEx(
    __out_bcount_opt(cbDest) LPTSTR  pszDest,
    __in  size_t  cbDest,
    __out_opt LPTSTR* ppszDestEnd,
    __out_opt size_t* pcbRemaining,
    IN  DWORD   dwFlags
    );

Routine Description:

    This routine is a safer version of the C built-in function 'gets' with
    some additional parameters. In addition to functionality provided by
    StringCbGets, this routine also returns a pointer to the end of the
    destination string and the number of characters left in the destination string
    including the null terminator. The flags parameter allows additional controls.

Arguments:

    pszDest         -   destination string

    cbDest          -   size of destination buffer in bytes.
                     
    ppszDestEnd     -   if ppszDestEnd is non-null, the function will return a
                        pointer to the end of the destination string.  If the
                        function copied any data, the result will point to the
                        null termination character

    pcbRemaining    -   if pbRemaining is non-null, the function will return the
                        number of bytes left in the destination string,
                        including the null terminator

    dwFlags         -   controls some details of the string copy:

        STRSAFE_FILL_BEHIND_NULL
                    if the function succeeds, the low byte of dwFlags will be
                    used to fill the uninitialize part of destination buffer
                    behind the null terminator

        STRSAFE_IGNORE_NULLS
                    treat NULL string pointers like empty strings (TEXT("")).

        STRSAFE_FILL_ON_FAILURE
                    if the function fails, the low byte of dwFlags will be
                    used to fill all of the destination buffer, and it will
                    be null terminated.

        STRSAFE_NO_TRUNCATION /
        STRSAFE_NULL_ON_FAILURE
                    if the function fails, the destination buffer will be set
                    to the empty string. 
                    
Notes:
    pszDest should not be NULL unless the STRSAFE_IGNORE_NULLS flag is specified.
    If STRSAFE_IGNORE_NULLS is passed and pszDest is NULL, an error may still be 
    returned even though NULLS are ignored

    cbDest must be > sizeof(TCHAR) for this function to succeed

Return Value:

    S_OK        -   data was read from stdin and copied, and the resultant dest
                    string was null terminated

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    STRSAFE_E_END_OF_FILE
                -   this return value indicates an error or end-of-file condition,
                    use feof or ferror to determine which one has occured.

    STRSAFE_E_INSUFFICIENT_BUFFER / 
    HRESULT_CODE(hr) == ERROR_INSUFFICIENT_BUFFER
                -   this return value is an indication that there was insufficient
                    space in the destination buffer to copy any data

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

#ifdef STRSAFE_WITH_GETS
STRSAFE_INLINE_API StringCbGetsExA(__out_bcount_opt(cbDest) PSTR pszDest, size_t cbDest, __out_opt PSTR* ppszDestEnd, __out_opt size_t* pbRemaining, unsigned long dwFlags);
STRSAFE_INLINE_API StringCbGetsExW(__out_bcount_opt(cbDest) PWSTR pszDest, size_t cbDest, __out_opt PWSTR* ppszDestEnd, __out_opt size_t* pcbRemaining, unsigned long dwFlags);
#ifdef UNICODE
#define StringCbGetsEx  StringCbGetsExW
#else
#define StringCbGetsEx  StringCbGetsExA
#endif // !UNICODE

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFE_INLINE_API StringCbGetsExA(PSTR pszDest, size_t cbDest, PSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(char);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringGetsExWorkerA(pszDest, cchDest, cbDest, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr)                           ||
        (hr == STRSAFE_E_INSUFFICIENT_BUFFER)   ||
        (hr == STRSAFE_E_END_OF_FILE))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(char) since cchRemaining < STRSAFE_MAX_CCH and sizeof(char) is 1
            *pcbRemaining = (cchRemaining * sizeof(char)) + (cbDest % sizeof(char));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFE_INLINE_API StringCbGetsExW(PWSTR pszDest, size_t cbDest, PWSTR* ppszDestEnd, size_t* pcbRemaining, unsigned long dwFlags)
{
    HRESULT hr;
    size_t cchDest;
    size_t cchRemaining = 0;

    cchDest = cbDest / sizeof(wchar_t);

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringGetsExWorkerW(pszDest, cchDest, cbDest, ppszDestEnd, &cchRemaining, dwFlags);
    }

    if (SUCCEEDED(hr)                           ||
        (hr == STRSAFE_E_INSUFFICIENT_BUFFER)   ||
        (hr == STRSAFE_E_END_OF_FILE))
    {
        if (pcbRemaining)
        {
            // safe to multiply cchRemaining * sizeof(wchar_t) since cchRemaining < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
            *pcbRemaining = (cchRemaining * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t));
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // !STRSAFE_NO_CB_FUNCTIONS
#endif  // STRSAFE_WITH_GETS

#ifndef STRSAFE_NO_CCH_FUNCTIONS
/*++

STDAPI
StringCchLength(
    __in_ecount(cchMax)  LPCTSTR psz,
    __in  size_t  cchMax,
    __out_opt size_t* pcch
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strlen'.
    It is used to make sure a string is not larger than a given length, and
    it optionally returns the current length in characters not including
    the null terminator.

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the string is non-null and the length including the null terminator is
    less than or equal to cchMax characters.

Arguments:

    psz         -   string to check the length of

    cchMax      -   maximum number of characters including the null terminator
                    that psz is allowed to contain

    pcch        -   if the function succeeds and pcch is non-null, the current length
                    in characters of psz excluding the null terminator will be returned.
                    This out parameter is equivalent to the return value of strlen(psz)

Notes: 
    psz can be null but the function will fail

    cchMax should be greater than zero or the function will fail

Return Value:

    S_OK        -   psz is non-null and the length including the null terminator is
                    less than or equal to cchMax characters

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

STRSAFEAPI StringCchLengthA(__in_ecount_opt(cchMax) PCSTR psz, size_t cchMax, __out_opt size_t* pcch);
STRSAFEAPI StringCchLengthW(__in_ecount_opt(cchMax) PCWSTR psz, size_t cchMax, __out_opt size_t* pcch);
#ifdef UNICODE
#define StringCchLength  StringCchLengthW
#else
#define StringCchLength  StringCchLengthA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCchLengthA(PCSTR psz, size_t cchMax, size_t* pcch)
{
    HRESULT hr;
    
    if ((psz == NULL) || (cchMax > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringLengthWorkerA(psz, cchMax, pcch);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCchLengthW(PCWSTR psz, size_t cchMax, size_t* pcch)
{
    HRESULT hr;

    if ((psz == NULL) || (cchMax > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringLengthWorkerW(psz, cchMax, pcch);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CCH_FUNCTIONS


#ifndef STRSAFE_NO_CB_FUNCTIONS
/*++

STDAPI
StringCbLength(
    __in_bcount(cbMax)  LPCTSTR psz,
    __in  size_t  cbMax,
    __out_opt size_t* pcb
    );

Routine Description:

    This routine is a safer version of the C built-in function 'strlen'.
    It is used to make sure a string is not larger than a given length, and
    it optionally returns the current length in bytes not including 
    the null terminator.

    This function returns a hresult, and not a pointer.  It returns a S_OK
    if the string is non-null and the length including the null terminator is
    less than or equal to cbMax bytes.

Arguments:

    psz         -   string to check the length of

    cbMax       -   maximum number of bytes including the null terminator
                    that psz is allowed to contain

    pcb         -   if the function succeeds and pcb is non-null, the current length
                    in bytes of psz excluding the null terminator will be returned.
                    This out parameter is equivalent to the return value of strlen(psz) * sizeof(TCHAR)

Notes: 
    psz can be null but the function will fail

    cbMax should be greater than or equal to sizeof(TCHAR) or the function will fail

Return Value:

    S_OK        -   psz is non-null and the length including the null terminator is
                    less than or equal to cbMax bytes

    failure     -   you can use the macro HRESULT_CODE() to get a win32 error
                    code for all hresult falure cases

    It is strongly recommended to use the SUCCEEDED() / FAILED() macros to test the
    return value of this function.

--*/

STRSAFEAPI StringCbLengthA(__in_bcount(cbMax) PCSTR psz, size_t cbMax, __out_opt size_t* pcch);
STRSAFEAPI StringCbLengthW(__in_bcount(cbMax) PCWSTR psz, size_t cbMax, __out_opt size_t* pcch);
#ifdef UNICODE
#define StringCbLength  StringCbLengthW
#else
#define StringCbLength  StringCbLengthA
#endif // !UNICODE

#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCbLengthA(PCSTR psz, size_t cbMax, size_t* pcb)
{
    HRESULT hr;
    size_t cchMax;
    size_t cch = 0;

    cchMax = cbMax / sizeof(char);

    if ((psz == NULL) || (cchMax > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringLengthWorkerA(psz, cchMax, &cch);
    }

    if (SUCCEEDED(hr) && pcb)
    {
        // safe to multiply cch * sizeof(char) since cch < STRSAFE_MAX_CCH and sizeof(char) is 1
        *pcb = cch * sizeof(char);
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCbLengthW(PCWSTR psz, size_t cbMax, size_t* pcb)
{
    HRESULT hr;
    size_t cchMax;
    size_t cch = 0;

    cchMax = cbMax / sizeof(wchar_t);

    if ((psz == NULL) || (cchMax > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringLengthWorkerW(psz, cchMax, &cch);
    }

    if (SUCCEEDED(hr) && pcb)
    {
        // safe to multiply cch * sizeof(wchar_t) since cch < STRSAFE_MAX_CCH and sizeof(wchar_t) is 2
        *pcb = cch * sizeof(wchar_t);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE
#endif  // !STRSAFE_NO_CB_FUNCTIONS


// these are the worker functions that actually do the work
#ifdef STRSAFE_INLINE
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCopyWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszSrc)
{
    HRESULT hr = S_OK;

    if (cchDest == 0)
    {
        // can not null terminate a zero-byte dest buffer
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        while (cchDest && (*pszSrc != '\0'))
        {
            *pszDest++ = *pszSrc++;
            cchDest--;
        }

        if (cchDest == 0)
        {
            // we are going to truncate pszDest
            pszDest--;
            hr = STRSAFE_E_INSUFFICIENT_BUFFER;
        }

        *pszDest= '\0';
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCopyWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc)
{
    HRESULT hr = S_OK;

    if (cchDest == 0)
    {
        // can not null terminate a zero-byte dest buffer
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        while (cchDest && (*pszSrc != L'\0'))
        {
            *pszDest++ = *pszSrc++;
            cchDest--;
        }

        if (cchDest == 0)
        {
            // we are going to truncate pszDest
            pszDest--;
            hr = STRSAFE_E_INSUFFICIENT_BUFFER;
        }

        *pszDest= L'\0';
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCopyExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, const char * pszSrc, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    char *pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(char))    ||
    //        cbDest == (cchDest * sizeof(char)) + (cbDest % sizeof(char)));
 
    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest != 0) || (cbDest != 0))
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }

            if (pszSrc == NULL)
            {
                pszSrc = "";
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                pszDestEnd = pszDest;
                cchRemaining = 0;

                // only fail if there was actually src data to copy
                if (*pszSrc != '\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                pszDestEnd = pszDest;
                cchRemaining = cchDest;

                while (cchRemaining && (*pszSrc != '\0'))
                {
                    *pszDestEnd++= *pszSrc++;
                    cchRemaining--;
                }
    
                if (cchRemaining > 0)
                {
                    if (dwFlags & STRSAFE_FILL_BEHIND_NULL)
                    {
                        memset(pszDestEnd + 1, STRSAFE_GET_FILL_PATTERN(dwFlags), ((cchRemaining - 1) * sizeof(char)) + (cbDest % sizeof(char)));
                    }
                }
                else
                {
                    // we are going to truncate pszDest
                    pszDestEnd--;
                    cchRemaining++;

                    hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                }

                *pszDestEnd = '\0';
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);
            
                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = '\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE | STRSAFE_NO_TRUNCATION))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = '\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd) 
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCopyExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PCWSTR pszSrc, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    PWSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(wchar_t)) ||
    //        cbDest == (cchDest * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)));
 
    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest != 0) || (cbDest != 0))
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }

            if (pszSrc == NULL)
            {
                pszSrc = L"";
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                pszDestEnd = pszDest;
                cchRemaining = 0;

                // only fail if there was actually src data to copy
                if (*pszSrc != L'\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                pszDestEnd = pszDest;
                cchRemaining = cchDest;

                while (cchRemaining && (*pszSrc != L'\0'))
                {
                    *pszDestEnd++= *pszSrc++;
                    cchRemaining--;
                }
    
                if (cchRemaining > 0)
                {
                    if (dwFlags & STRSAFE_FILL_BEHIND_NULL)
                    {
                        memset(pszDestEnd + 1, STRSAFE_GET_FILL_PATTERN(dwFlags), ((cchRemaining - 1) * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)));
                    }
                }
                else
                {
                    // we are going to truncate pszDest
                    pszDestEnd--;
                    cchRemaining++;

                    hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                }

                *pszDestEnd = L'\0';
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);
                           
                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = L'\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE | STRSAFE_NO_TRUNCATION))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = L'\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd) 
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCopyNWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchSrc)
{
    HRESULT hr = S_OK;

    if (cchDest == 0)
    {
        // can not null terminate a zero-byte dest buffer
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        while (cchDest && cchSrc && (*pszSrc != '\0'))
        {
            *pszDest++= *pszSrc++;
            cchDest--;
            cchSrc--;
        }

        if (cchDest == 0)
        {
            // we are going to truncate pszDest
            pszDest--;
            hr = STRSAFE_E_INSUFFICIENT_BUFFER;
        }

        *pszDest= '\0';
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCopyNWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchSrc)
{
    HRESULT hr = S_OK;

    if (cchDest == 0)
    {
        // can not null terminate a zero-byte dest buffer
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        while (cchDest && cchSrc && (*pszSrc != L'\0'))
        {
            *pszDest++= *pszSrc++;
            cchDest--;
            cchSrc--;
        }

        if (cchDest == 0)
        {
            // we are going to truncate pszDest
            pszDest--;
            hr = STRSAFE_E_INSUFFICIENT_BUFFER;
        }

        *pszDest= L'\0';
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCopyNExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PCSTR pszSrc, size_t cchSrc, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    PSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(char))    ||
    //        cbDest == (cchDest * sizeof(char)) + (cbDest % sizeof(char)));
 
    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest != 0) || (cbDest != 0))
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }

            if (pszSrc == NULL)
            {
                pszSrc = "";
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                pszDestEnd = pszDest;
                cchRemaining = 0;

                // only fail if there was actually src data to copy
                if (*pszSrc != '\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                pszDestEnd = pszDest;
                cchRemaining = cchDest;

                while (cchRemaining && cchSrc && (*pszSrc != '\0'))
                {
                    *pszDestEnd++= *pszSrc++;
                    cchRemaining--;
                    cchSrc--;
                }
    
                if (cchRemaining > 0)
                {
                    if (dwFlags & STRSAFE_FILL_BEHIND_NULL)
                    {
                        memset(pszDestEnd + 1, STRSAFE_GET_FILL_PATTERN(dwFlags), ((cchRemaining - 1) * sizeof(char)) + (cbDest % sizeof(char)));
                    }
                }
                else
                {
                    // we are going to truncate pszDest
                    pszDestEnd--;
                    cchRemaining++;

                    hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                }

                *pszDestEnd = '\0';
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);
            
                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = '\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE | STRSAFE_NO_TRUNCATION))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = '\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd) 
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCopyNExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PCWSTR pszSrc, size_t cchSrc, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    PWSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(wchar_t)) ||
    //        cbDest == (cchDest * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)));
 
    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest != 0) || (cbDest != 0))
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }

            if (pszSrc == NULL)
            {
                pszSrc = L"";
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                pszDestEnd = pszDest;
                cchRemaining = 0;

                // only fail if there was actually src data to copy
                if (*pszSrc != L'\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                pszDestEnd = pszDest;
                cchRemaining = cchDest;

                while (cchRemaining && cchSrc && (*pszSrc != L'\0'))
                {
                    *pszDestEnd++= *pszSrc++;
                    cchRemaining--;
                    cchSrc--;
                }
    
                if (cchRemaining > 0)
                {
                    if (dwFlags & STRSAFE_FILL_BEHIND_NULL)
                    {
                        memset(pszDestEnd + 1, STRSAFE_GET_FILL_PATTERN(dwFlags), ((cchRemaining - 1) * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)));
                    }
                }
                else
                {
                    // we are going to truncate pszDest
                    pszDestEnd--;
                    cchRemaining++;

                    hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                }

                *pszDestEnd = L'\0';
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);
            
                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = L'\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE | STRSAFE_NO_TRUNCATION))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = L'\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd) 
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCatWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszSrc)
{
   HRESULT hr;
   size_t cchDestCurrent;

   hr = StringLengthWorkerA(pszDest, cchDest, &cchDestCurrent);

   if (SUCCEEDED(hr))
   {
       hr = StringCopyWorkerA(pszDest + cchDestCurrent,
                              cchDest - cchDestCurrent,
                              pszSrc);
   }

   return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCatWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc)
{
   HRESULT hr;
   size_t cchDestCurrent;

   hr = StringLengthWorkerW(pszDest, cchDest, &cchDestCurrent);

   if (SUCCEEDED(hr))
   {
       hr = StringCopyWorkerW(pszDest + cchDestCurrent,
                              cchDest - cchDestCurrent,
                              pszSrc);
   }

   return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCatExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PCSTR pszSrc, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    PSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(char))    ||
    //        cbDest == (cchDest * sizeof(char)) + (cbDest % sizeof(char)));

    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cchDestCurrent;

        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest == 0) && (cbDest == 0))
                {
                    cchDestCurrent = 0;
                }
                else
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }
            else
            {
                hr = StringLengthWorkerA(pszDest, cchDest, &cchDestCurrent);

                if (SUCCEEDED(hr))
                {
                    pszDestEnd = pszDest + cchDestCurrent;
                    cchRemaining = cchDest - cchDestCurrent;
                }
            }

            if (pszSrc == NULL)
            {
                pszSrc = "";
            }
        }
        else
        {
            hr = StringLengthWorkerA(pszDest, cchDest, &cchDestCurrent);

            if (SUCCEEDED(hr))
            {
                pszDestEnd = pszDest + cchDestCurrent;
                cchRemaining = cchDest - cchDestCurrent;
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                // only fail if there was actually src data to append
                if (*pszSrc != '\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                // we handle the STRSAFE_FILL_ON_FAILURE and STRSAFE_NULL_ON_FAILURE cases below, so do not pass
                // those flags through
                hr = StringCopyExWorkerA(pszDestEnd,
                                         cchRemaining,
                                         (cchRemaining * sizeof(char)) + (cbDest % sizeof(char)),
                                         pszSrc,
                                         &pszDestEnd,
                                         &cchRemaining,
                                         dwFlags & (~(STRSAFE_FILL_ON_FAILURE | STRSAFE_NULL_ON_FAILURE)));
            }
        }
    }
    
    if (FAILED(hr))
    {
        if (pszDest)
        {
            // STRSAFE_NO_TRUNCATION is taken care of by StringCopyExWorkerA()

            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);

                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = '\0';
                }
            }

            if (dwFlags & STRSAFE_NULL_ON_FAILURE)
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = '\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd) 
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCatExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PCWSTR pszSrc, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    PWSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(wchar_t)) ||
    //        cbDest == (cchDest * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)));

    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        size_t cchDestCurrent;

        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest == 0) && (cbDest == 0))
                {
                    cchDestCurrent = 0;
                }
                else
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }
            else
            {
                hr = StringLengthWorkerW(pszDest, cchDest, &cchDestCurrent);

                if (SUCCEEDED(hr))
                {
                    pszDestEnd = pszDest + cchDestCurrent;
                    cchRemaining = cchDest - cchDestCurrent;
                }
            }

            if (pszSrc == NULL)
            {
                pszSrc = L"";
            }
        }
        else
        {
            hr = StringLengthWorkerW(pszDest, cchDest, &cchDestCurrent);

            if (SUCCEEDED(hr))
            {
                pszDestEnd = pszDest + cchDestCurrent;
                cchRemaining = cchDest - cchDestCurrent;
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                // only fail if there was actually src data to append
                if (*pszSrc != L'\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                // we handle the STRSAFE_FILL_ON_FAILURE and STRSAFE_NULL_ON_FAILURE cases below, so do not pass
                // those flags through
                hr = StringCopyExWorkerW(pszDestEnd,
                                         cchRemaining,
                                         (cchRemaining * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)),
                                         pszSrc,
                                         &pszDestEnd,
                                         &cchRemaining,
                                         dwFlags & (~(STRSAFE_FILL_ON_FAILURE | STRSAFE_NULL_ON_FAILURE)));            
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            // STRSAFE_NO_TRUNCATION is taken care of by StringCopyExWorkerW()

            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);
            
                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = L'\0';
                }
            }

            if (dwFlags & STRSAFE_NULL_ON_FAILURE)
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = L'\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd) 
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCatNWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszSrc, size_t cchMaxAppend)
{
    HRESULT hr;
    size_t cchDestCurrent;

    hr = StringLengthWorkerA(pszDest, cchDest, &cchDestCurrent);

    if (SUCCEEDED(hr))
    {
        hr = StringCopyNWorkerA(pszDest + cchDestCurrent,
                                cchDest - cchDestCurrent,
                                pszSrc,
                                cchMaxAppend);
    }    

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCatNWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszSrc, size_t cchMaxAppend)
{
    HRESULT hr;
    size_t cchDestCurrent;

    hr = StringLengthWorkerW(pszDest, cchDest, &cchDestCurrent);

    if (SUCCEEDED(hr))
    {
        hr = StringCopyNWorkerW(pszDest + cchDestCurrent,
                                cchDest - cchDestCurrent,
                                pszSrc,
                                cchMaxAppend);
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringCatNExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PCSTR pszSrc, size_t cchMaxAppend, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    PSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;
    size_t cchDestCurrent = 0;

    // ASSERT(cbDest == (cchDest * sizeof(char))    ||
    //        cbDest == (cchDest * sizeof(char)) + (cbDest % sizeof(char)));

    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest == 0) && (cbDest == 0))
                {
                    cchDestCurrent = 0;
                }
                else
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }
            else
            {
                hr = StringLengthWorkerA(pszDest, cchDest, &cchDestCurrent);

                if (SUCCEEDED(hr))
                {
                    pszDestEnd = pszDest + cchDestCurrent;
                    cchRemaining = cchDest - cchDestCurrent;
                }
            }

            if (pszSrc == NULL)
            {
                pszSrc = "";
            }
        }
        else
        {
            hr = StringLengthWorkerA(pszDest, cchDest, &cchDestCurrent);

            if (SUCCEEDED(hr))
            {
                pszDestEnd = pszDest + cchDestCurrent;
                cchRemaining = cchDest - cchDestCurrent;
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                // only fail if there was actually src data to append
                if (*pszSrc != '\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                // we handle the STRSAFE_FILL_ON_FAILURE and STRSAFE_NULL_ON_FAILURE cases below, so do not pass
                // those flags through
                hr = StringCopyNExWorkerA(pszDestEnd,
                                          cchRemaining,
                                          (cchRemaining * sizeof(char)) + (cbDest % sizeof(char)),
                                          pszSrc,
                                          cchMaxAppend,
                                          &pszDestEnd,
                                          &cchRemaining,
                                          dwFlags & (~(STRSAFE_FILL_ON_FAILURE | STRSAFE_NULL_ON_FAILURE)));
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            // STRSAFE_NO_TRUNCATION is taken care of by StringCopyNExWorkerA()

            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);

                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = '\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = '\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd)
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringCatNExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PCWSTR pszSrc, size_t cchMaxAppend, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    PWSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;
    size_t cchDestCurrent = 0;


    // ASSERT(cbDest == (cchDest * sizeof(wchar_t)) ||
    //        cbDest == (cchDest * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)));

    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest == 0) && (cbDest == 0))
                {
                    cchDestCurrent = 0;
                }
                else
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }
            else
            {
                hr = StringLengthWorkerW(pszDest, cchDest, &cchDestCurrent);

                if (SUCCEEDED(hr))
                {
                    pszDestEnd = pszDest + cchDestCurrent;
                    cchRemaining = cchDest - cchDestCurrent;
                }
            }

            if (pszSrc == NULL)
            {
                pszSrc = L"";
            }
        }
        else
        {
            hr = StringLengthWorkerW(pszDest, cchDest, &cchDestCurrent);

            if (SUCCEEDED(hr))
            {
                pszDestEnd = pszDest + cchDestCurrent;
                cchRemaining = cchDest - cchDestCurrent;
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                // only fail if there was actually src data to append
                if (*pszSrc != L'\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                // we handle the STRSAFE_FILL_ON_FAILURE and STRSAFE_NULL_ON_FAILURE cases below, so do not pass
                // those flags through
                hr = StringCopyNExWorkerW(pszDestEnd,
                                          cchRemaining,
                                          (cchRemaining * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)),
                                          pszSrc,
                                          cchMaxAppend,
                                          &pszDestEnd,
                                          &cchRemaining,
                                          dwFlags & (~(STRSAFE_FILL_ON_FAILURE | STRSAFE_NULL_ON_FAILURE)));
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            // STRSAFE_NO_TRUNCATION is taken care of by StringCopyNExWorkerW()

            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);

                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = L'\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = L'\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd)
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringVPrintfWorkerA(PSTR pszDest, size_t cchDest, PCSTR pszFormat, va_list argList)
{
    HRESULT hr = S_OK;

    if (cchDest == 0)
    {
        // can not null terminate a zero-byte dest buffer
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        int iRet;
        size_t cchMax;

        // leave the last space for the null terminator
        cchMax = cchDest - 1;

        iRet = _vsnprintf(pszDest, cchMax, pszFormat, argList);
        // ASSERT((iRet < 0) || (((size_t)iRet) <= cchMax));

        if ((iRet < 0) || (((size_t)iRet) > cchMax))
        {
            // need to null terminate the string
            pszDest += cchMax;
            *pszDest = '\0';

            // we have truncated pszDest
            hr = STRSAFE_E_INSUFFICIENT_BUFFER;
        }
        else if (((size_t)iRet) == cchMax)
        {
            // need to null terminate the string
            pszDest += cchMax;
            *pszDest = '\0';
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringVPrintfWorkerW(PWSTR pszDest, size_t cchDest, PCWSTR pszFormat, va_list argList)
{
    HRESULT hr = S_OK;

    if (cchDest == 0)
    {
        // can not null terminate a zero-byte dest buffer
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else    
    {
        int iRet;
        size_t cchMax;

        // leave the last space for the null terminator
        cchMax = cchDest - 1;

        iRet = _vsnwprintf(pszDest, cchMax, pszFormat, argList);
        // ASSERT((iRet < 0) || (((size_t)iRet) <= cchMax));

        if ((iRet < 0) || (((size_t)iRet) > cchMax))
        {
            // need to null terminate the string
            pszDest += cchMax;
            *pszDest = L'\0';

            // we have truncated pszDest
            hr = STRSAFE_E_INSUFFICIENT_BUFFER;
        }
        else if (((size_t)iRet) == cchMax)
        {
            // need to null terminate the string
            pszDest += cchMax;
            *pszDest = L'\0';
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringVPrintfExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags, PCSTR pszFormat, va_list argList)
{
    HRESULT hr = S_OK;
    PSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(char))    ||
    //        cbDest == (cchDest * sizeof(char)) + (cbDest % sizeof(char)));

    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest != 0) || (cbDest != 0))
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }

            if (pszFormat == NULL)
            {
                pszFormat = "";
            }
        }
    
        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                pszDestEnd = pszDest;
                cchRemaining = 0;

                // only fail if there was actually a non-empty format string
                if (*pszFormat != '\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                int iRet;
                size_t cchMax;

                // leave the last space for the null terminator
                cchMax = cchDest - 1;

                iRet = _vsnprintf(pszDest, cchMax, pszFormat, argList);
                // ASSERT((iRet < 0) || (((size_t)iRet) <= cchMax));

                if ((iRet < 0) || (((size_t)iRet) > cchMax))
                {
                    // we have truncated pszDest
                    pszDestEnd = pszDest + cchMax;
                    cchRemaining = 1;

                    // need to null terminate the string
                    *pszDestEnd = '\0';

                    hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                }
                else if (((size_t)iRet) == cchMax)
                {
                    // string fit perfectly
                    pszDestEnd = pszDest + cchMax;
                    cchRemaining = 1;

                    // need to null terminate the string
                    *pszDestEnd = '\0';
                }
                else if (((size_t)iRet) < cchMax)
                {
                    // there is extra room
                    pszDestEnd = pszDest + iRet;
                    cchRemaining = cchDest - iRet;

                    if (dwFlags & STRSAFE_FILL_BEHIND_NULL)
                    {
                        memset(pszDestEnd + 1, STRSAFE_GET_FILL_PATTERN(dwFlags), ((cchRemaining - 1) * sizeof(char)) + (cbDest % sizeof(char)));
                    }
                }
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);

                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = '\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE | STRSAFE_NO_TRUNCATION))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = '\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd) 
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringVPrintfExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags, PCWSTR pszFormat, va_list argList)
{
    HRESULT hr = S_OK;
    PWSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(wchar_t)) ||
    //        cbDest == (cchDest * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)));

    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest != 0) || (cbDest != 0))
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }

            if (pszFormat == NULL)
            {
                pszFormat = L"";
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest == 0)
            {
                pszDestEnd = pszDest;
                cchRemaining = 0;

                // only fail if there was actually a non-empty format string
                if (*pszFormat != L'\0')
                {
                    if (pszDest == NULL)
                    {
                        hr = STRSAFE_E_INVALID_PARAMETER;
                    }
                    else
                    {
                        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                    }
                }
            }
            else
            {
                int iRet;
                size_t cchMax;

                // leave the last space for the null terminator
                cchMax = cchDest - 1;

                iRet = _vsnwprintf(pszDest, cchMax, pszFormat, argList);
                // ASSERT((iRet < 0) || (((size_t)iRet) <= cchMax));

                if ((iRet < 0) || (((size_t)iRet) > cchMax))
                {
                    // we have truncated pszDest
                    pszDestEnd = pszDest + cchMax;
                    cchRemaining = 1;

                    // need to null terminate the string
                    *pszDestEnd = L'\0';

                    hr = STRSAFE_E_INSUFFICIENT_BUFFER;
                }
                else if (((size_t)iRet) == cchMax)
                {
                    // string fit perfectly
                    pszDestEnd = pszDest + cchMax;
                    cchRemaining = 1;

                    // need to null terminate the string
                    *pszDestEnd = L'\0';
                }
                else if (((size_t)iRet) < cchMax)
                {
                    // there is extra room
                    pszDestEnd = pszDest + iRet;
                    cchRemaining = cchDest - iRet;

                    if (dwFlags & STRSAFE_FILL_BEHIND_NULL)
                    {
                        memset(pszDestEnd + 1, STRSAFE_GET_FILL_PATTERN(dwFlags), ((cchRemaining - 1) * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)));
                    }
                }
            }
        }
    }
    
    if (FAILED(hr))
    {
        if (pszDest)
        {
            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);

                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = L'\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE | STRSAFE_NO_TRUNCATION))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = L'\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) || (hr == STRSAFE_E_INSUFFICIENT_BUFFER))
    {
        if (ppszDestEnd) 
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS

#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFEAPI StringLengthWorkerA(PCSTR psz, size_t cchMax, size_t* pcch)
{
    HRESULT hr = S_OK;
    size_t cchMaxPrev = cchMax;

    while (cchMax && (*psz != '\0'))
    {
        psz++;
        cchMax--;
    }

    if (cchMax == 0)
    {
        // the string is longer than cchMax
        hr = STRSAFE_E_INVALID_PARAMETER;
    }

    if (SUCCEEDED(hr) && pcch)
    {
        *pcch = cchMaxPrev - cchMax;
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS
STRSAFEAPI StringLengthWorkerW(PCWSTR psz, size_t cchMax, size_t* pcch)
{
    HRESULT hr = S_OK;
    size_t cchMaxPrev = cchMax;

    while (cchMax && (*psz != L'\0'))
    {
        psz++;
        cchMax--;
    }

    if (cchMax == 0)
    {
        // the string is longer than cchMax
        hr = STRSAFE_E_INVALID_PARAMETER;
    }

    if (SUCCEEDED(hr) && pcch)
    {
        *pcch = cchMaxPrev - cchMax;
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_INLINE

#ifdef STRSAFE_WITH_GETS
#ifndef STRSAFE_NO_A_FUNCTIONS
STRSAFE_INLINE_API StringGetsExWorkerA(PSTR pszDest, size_t cchDest, size_t cbDest, PSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    PSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(char))    ||
    //        cbDest == (cchDest * sizeof(char)) + (cbDest % sizeof(char)));

    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest != 0) || (cbDest != 0))
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest <= 1)
            {
                pszDestEnd = pszDest;
                cchRemaining = cchDest;

                if (cchDest == 1)
                {
                    *pszDestEnd = '\0';
                }

                hr = STRSAFE_E_INSUFFICIENT_BUFFER;
            }
            else
            {
                pszDestEnd = pszDest;
                cchRemaining = cchDest;

                while (cchRemaining > 1)
                {
                    char ch;
                    int iRet = getc(stdin);
                    
                    if (iRet == EOF)
                    {
                        if (pszDestEnd == pszDest)
                        {
                            // we failed to read anything from stdin
                            hr = STRSAFE_E_END_OF_FILE;
                        }
                        break;
                    }
                    
                    ch = (char)iRet;

                    if (ch == '\n')
                    {
                        break;
                    }

                    *pszDestEnd = ch;

                    pszDestEnd++;
                    cchRemaining--;
                }

                if (cchRemaining > 0)
                {
                    // there is extra room
                    if (dwFlags & STRSAFE_FILL_BEHIND_NULL)
                    {
                        memset(pszDestEnd + 1, STRSAFE_GET_FILL_PATTERN(dwFlags), ((cchRemaining - 1) * sizeof(char)) + (cbDest % sizeof(char)));
                    }
                }

                *pszDestEnd = '\0';
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);

                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = '\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE | STRSAFE_NO_TRUNCATION))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = '\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) ||
        (hr == STRSAFE_E_INSUFFICIENT_BUFFER) ||
        (hr == STRSAFE_E_END_OF_FILE))
    {
        if (ppszDestEnd)
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_A_FUNCTIONS
#ifndef STRSAFE_NO_W_FUNCTIONS

STRSAFE_INLINE_API StringGetsExWorkerW(PWSTR pszDest, size_t cchDest, size_t cbDest, PWSTR* ppszDestEnd, size_t* pcchRemaining, unsigned long dwFlags)
{
    HRESULT hr = S_OK;
    PWSTR pszDestEnd = pszDest;
    size_t cchRemaining = 0;

    // ASSERT(cbDest == (cchDest * sizeof(char))    ||
    //        cbDest == (cchDest * sizeof(char)) + (cbDest % sizeof(char)));

    // only accept valid flags
    if (dwFlags & (~STRSAFE_VALID_FLAGS))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        if (dwFlags & STRSAFE_IGNORE_NULLS)
        {
            if (pszDest == NULL)
            {
                if ((cchDest != 0) || (cbDest != 0))
                {
                    // NULL pszDest and non-zero cchDest/cbDest is invalid
                    hr = STRSAFE_E_INVALID_PARAMETER;
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            if (cchDest <= 1)
            {
                pszDestEnd = pszDest;
                cchRemaining = cchDest;

                if (cchDest == 1)
                {
                    *pszDestEnd = L'\0';
                }

                hr = STRSAFE_E_INSUFFICIENT_BUFFER;
            }
            else
            {
                pszDestEnd = pszDest;
                cchRemaining = cchDest;

                while (cchRemaining > 1)
                {
                    wchar_t ch = (wchar_t)getwc(stdin);
                    // ASSERT(sizeof(wchar_t) == sizeof(wint_t));
                    
                    if (ch == WEOF)
                    {
                        if (pszDestEnd == pszDest)
                        {
                            // we failed to read anything from stdin
                            hr = STRSAFE_E_END_OF_FILE;
                        }
                        break;
                    }
                    
                    if (ch == L'\n')
                    {
                        break;
                    }

                    *pszDestEnd = ch;

                    pszDestEnd++;
                    cchRemaining--;
                }

                if (cchRemaining > 0)
                {
                    // there is extra room
                    if (dwFlags & STRSAFE_FILL_BEHIND_NULL)
                    {
                        memset(pszDestEnd + 1, STRSAFE_GET_FILL_PATTERN(dwFlags), ((cchRemaining - 1) * sizeof(wchar_t)) + (cbDest % sizeof(wchar_t)));
                    }
                }

                *pszDestEnd = L'\0';
            }
        }
    }

    if (FAILED(hr))
    {
        if (pszDest)
        {
            if (dwFlags & STRSAFE_FILL_ON_FAILURE)
            {
                memset(pszDest, STRSAFE_GET_FILL_PATTERN(dwFlags), cbDest);

                if (STRSAFE_GET_FILL_PATTERN(dwFlags) == 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;
                }
                else if (cchDest > 0)
                {
                    pszDestEnd = pszDest + cchDest - 1;
                    cchRemaining = 1;

                    // null terminate the end of the string
                    *pszDestEnd = L'\0';
                }
            }

            if (dwFlags & (STRSAFE_NULL_ON_FAILURE | STRSAFE_NO_TRUNCATION))
            {
                if (cchDest > 0)
                {
                    pszDestEnd = pszDest;
                    cchRemaining = cchDest;

                    // null terminate the beginning of the string
                    *pszDestEnd = L'\0';
                }
            }
        }
    }

    if (SUCCEEDED(hr) ||
        (hr == STRSAFE_E_INSUFFICIENT_BUFFER) ||
        (hr == STRSAFE_E_END_OF_FILE))
    {
        if (ppszDestEnd)
        {
            *ppszDestEnd = pszDestEnd;
        }

        if (pcchRemaining)
        {
            *pcchRemaining = cchRemaining;
        }
    }

    return hr;
}
#endif  // !STRSAFE_NO_W_FUNCTIONS
#endif  // STRSAFE_WITH_GETS

// Do not call these functions, they are worker functions for internal use within this file
#ifdef DEPRECATE_SUPPORTED
#pragma deprecated(StringCopyWorkerA)
#pragma deprecated(StringCopyWorkerW)
#pragma deprecated(StringCopyExWorkerA)
#pragma deprecated(StringCopyExWorkerW)
#pragma deprecated(StringCatWorkerA)
#pragma deprecated(StringCatWorkerW)
#pragma deprecated(StringCatExWorkerA)
#pragma deprecated(StringCatExWorkerW)
#pragma deprecated(StringCatNWorkerA)
#pragma deprecated(StringCatNWorkerW)
#pragma deprecated(StringCatNExWorkerA)
#pragma deprecated(StringCatNExWorkerW)
#pragma deprecated(StringVPrintfWorkerA)
#pragma deprecated(StringVPrintfWorkerW)
#pragma deprecated(StringVPrintfExWorkerA)
#pragma deprecated(StringVPrintfExWorkerW)
#pragma deprecated(StringLengthWorkerA)
#pragma deprecated(StringLengthWorkerW)
#else
#define StringCopyWorkerA        StringCopyWorkerA_instead_use_StringCchCopyA_or_StringCchCopyExA;
#define StringCopyWorkerW        StringCopyWorkerW_instead_use_StringCchCopyW_or_StringCchCopyExW;
#define StringCopyExWorkerA      StringCopyExWorkerA_instead_use_StringCchCopyA_or_StringCchCopyExA;
#define StringCopyExWorkerW      StringCopyExWorkerW_instead_use_StringCchCopyW_or_StringCchCopyExW;
#define StringCatWorkerA         StringCatWorkerA_instead_use_StringCchCatA_or_StringCchCatExA;
#define StringCatWorkerW         StringCatWorkerW_instead_use_StringCchCatW_or_StringCchCatExW;
#define StringCatExWorkerA       StringCatExWorkerA_instead_use_StringCchCatA_or_StringCchCatExA;
#define StringCatExWorkerW       StringCatExWorkerW_instead_use_StringCchCatW_or_StringCchCatExW;
#define StringCatNWorkerA        StringCatNWorkerA_instead_use_StringCchCatNA_or_StrincCbCatNA;
#define StringCatNWorkerW        StringCatNWorkerW_instead_use_StringCchCatNW_or_StringCbCatNW;
#define StringCatNExWorkerA      StringCatNExWorkerA_instead_use_StringCchCatNExA_or_StringCbCatNExA;
#define StringCatNExWorkerW      StringCatNExWorkerW_instead_use_StringCchCatNExW_or_StringCbCatNExW;
#define StringVPrintfWorkerA     StringVPrintfWorkerA_instead_use_StringCchVPrintfA_or_StringCchVPrintfExA;
#define StringVPrintfWorkerW     StringVPrintfWorkerW_instead_use_StringCchVPrintfW_or_StringCchVPrintfExW;
#define StringVPrintfExWorkerA   StringVPrintfExWorkerA_instead_use_StringCchVPrintfA_or_StringCchVPrintfExA;
#define StringVPrintfExWorkerW   StringVPrintfExWorkerW_instead_use_StringCchVPrintfW_or_StringCchVPrintfExW;
#define StringLengthWorkerA      StringLengthWorkerA_instead_use_StringCchLengthA_or_StringCbLengthA;
#define StringLengthWorkerW      StringLengthWorkerW_instead_use_StringCchLengthW_or_StringCbLengthW;
#endif // !DEPRECATE_SUPPORTED


#ifndef STRSAFE_NO_DEPRECATE
// Deprecate all of the unsafe functions to generate compiletime errors. If you do not want
// this then you can #define STRSAFE_NO_DEPRECATE before including this file.
#ifdef DEPRECATE_SUPPORTED
// First all the names that are a/w variants (or shouldn't be #defined by now anyway).
#pragma deprecated(lstrcpyA)
#pragma deprecated(lstrcpyW)
#pragma deprecated(lstrcatA)
#pragma deprecated(lstrcatW)
#pragma deprecated(wsprintfA)
#pragma deprecated(wsprintfW)

#pragma deprecated(StrCpyW)
#pragma deprecated(StrCatW)
#pragma deprecated(StrNCatA)
#pragma deprecated(StrNCatW)
#pragma deprecated(StrCatNA)
#pragma deprecated(StrCatNW)
#pragma deprecated(wvsprintfA)
#pragma deprecated(wvsprintfW)

#pragma deprecated(strcpy)
#pragma deprecated(wcscpy)
#pragma deprecated(strcat)
#pragma deprecated(wcscat)
#pragma deprecated(sprintf)
#pragma deprecated(swprintf)
#pragma deprecated(vsprintf)
#pragma deprecated(vswprintf)
#pragma deprecated(_snprintf)
#pragma deprecated(_snwprintf)
#pragma deprecated(_vsnprintf)
#pragma deprecated(_vsnwprintf)
#pragma deprecated(gets)
#pragma deprecated(_getws)

// Then all the windows.h names - we need to undef and redef based on UNICODE setting
#undef lstrcpy
#undef lstrcat
#undef wsprintf
#undef wvsprintf
#pragma deprecated(lstrcpy)
#pragma deprecated(lstrcat)
#pragma deprecated(wsprintf)
#pragma deprecated(wvsprintf)
#ifdef UNICODE
#define lstrcpy    lstrcpyW
#define lstrcat    lstrcatW
#define wsprintf   wsprintfW
#define wvsprintf  wvsprintfW
#else
#define lstrcpy    lstrcpyA
#define lstrcat    lstrcatA
#define wsprintf   wsprintfA
#define wvsprintf  wvsprintfA
#endif

// Then the shlwapi names - they key off UNICODE also.
#undef StrCpyA
#undef StrCpy
#undef StrCatA
#undef StrCat
#undef StrNCat
#undef StrCatN
#pragma deprecated(StrCpyA)
#pragma deprecated(StrCatA)
#pragma deprecated(StrCatN)
#pragma deprecated(StrCpy)
#pragma deprecated(StrCat)
#pragma deprecated(StrNCat)
#define StrCpyA lstrcpyA
#define StrCatA lstrcatA
#define StrCatN StrNCat
#ifdef UNICODE
#define StrCpy  StrCpyW
#define StrCat  StrCatW
#define StrNCat StrNCatW
#else
#define StrCpy  lstrcpyA
#define StrCat  lstrcatA
#define StrNCat StrNCatA
#endif

// Then all the CRT names - we need to undef/redef based on _UNICODE value.
#undef _tcscpy
#undef _ftcscpy
#undef _tcscat
#undef _ftcscat
#undef _stprintf
#undef _sntprintf
#undef _vstprintf
#undef _vsntprintf
#undef _getts
#pragma deprecated(_tcscpy)
#pragma deprecated(_ftcscpy)
#pragma deprecated(_tcscat)
#pragma deprecated(_ftcscat)
#pragma deprecated(_stprintf)
#pragma deprecated(_sntprintf)
#pragma deprecated(_vstprintf)
#pragma deprecated(_vsntprintf)
#pragma deprecated(_getts)
#ifdef _UNICODE
#define _tcscpy     wcscpy
#define _ftcscpy    wcscpy
#define _tcscat     wcscat
#define _ftcscat    wcscat
#define _stprintf   swprintf
#define _sntprintf  _snwprintf
#define _vstprintf  vswprintf
#define _vsntprintf _vsnwprintf
#define _getts      _getws
#else
#define _tcscpy     strcpy
#define _ftcscpy    strcpy
#define _tcscat     strcat
#define _ftcscat    strcat
#define _stprintf   sprintf
#define _sntprintf  _snprintf
#define _vstprintf  vsprintf
#define _vsntprintf _vsnprintf
#define _getts      gets
#endif
#else
#undef strcpy
#define strcpy      strcpy_instead_use_StringCbCopyA_or_StringCchCopyA;

#undef strcpyA
#define strcpyA     strcpyA_instead_use_StringCbCopyA_or_StringCchCopyA;

#undef strcpyW
#define strcpyW     strcpyW_instead_use_StringCbCopyW_or_StringCchCopyW;

#undef lstrcpy
#define lstrcpy     lstrcpy_instead_use_StringCbCopy_or_StringCchCopy;

#undef lstrcpyA
#define lstrcpyA    lstrcpyA_instead_use_StringCbCopyA_or_StringCchCopyA;

#undef lstrcpyW
#define lstrcpyW    lstrcpyW_instead_use_StringCbCopyW_or_StringCchCopyW;

#undef StrCpy
#define StrCpy      StrCpy_instead_use_StringCbCopy_or_StringCchCopy;

#undef StrCpyA
#define StrCpyA     StrCpyA_instead_use_StringCbCopyA_or_StringCchCopyA;

#undef StrCpyW
#define StrCpyW     StrCpyW_instead_use_StringCbCopyW_or_StringCchCopyW;

#undef _tcscpy
#define _tcscpy     _tcscpy_instead_use_StringCbCopy_or_StringCchCopy;

#undef _ftcscpy
#define _ftcscpy    _ftcscpy_instead_use_StringCbCopy_or_StringCchCopy;

#undef wcscpy
#define wcscpy      wcscpy_instead_use_StringCbCopyW_or_StringCchCopyW;

#undef strcat
#define strcat      strcat_instead_use_StringCbCatA_or_StringCchCatA;

#undef lstrcat
#define lstrcat     lstrcat_instead_use_StringCbCat_or_StringCchCat;

#undef lstrcatA
#define lstrcatA    lstrcatA_instead_use_StringCbCatA_or_StringCchCatA;

#undef lstrcatW
#define lstrcatW    lstrcatW_instead_use_StringCbCatW_or_StringCchCatW;

#undef StrCat
#define StrCat      StrCat_instead_use_StringCbCat_or_StringCchCat;

#undef StrCatA
#define StrCatA     StrCatA_instead_use_StringCbCatA_or_StringCchCatA;

#undef StrCatW
#define StrCatW     StrCatW_instead_use_StringCbCatW_or_StringCchCatW;

#undef StrNCat
#define StrNCat     StrNCat_instead_use_StringCbCatN_or_StringCchCatN;

#undef StrNCatA
#define StrNCatA    StrNCatA_instead_use_StringCbCatNA_or_StringCchCatNA;

#undef StrNCatW
#define StrNCatW    StrNCatW_instead_use_StringCbCatNW_or_StringCchCatNW;

#undef StrCatN
#define StrCatN     StrCatN_instead_use_StringCbCatN_or_StringCchCatN;

#undef StrCatNA
#define StrCatNA    StrCatNA_instead_use_StringCbCatNA_or_StringCchCatNA;

#undef StrCatNW
#define StrCatNW    StrCatNW_instead_use_StringCbCatNW_or_StringCchCatNW;

#undef wcscat
#define wcscat      wcscat_instead_use_StringCbCatW_or_StringCchCatW;

#undef _tcscat
#define _tcscat     _tcscat_instead_use_StringCbCat_or_StringCchCat;

#undef _ftcscat
#define _ftcscat    _ftcscat_instead_use_StringCbCat_or_StringCchCat;

#undef wsprintf     
#define wsprintf    wsprintf_instead_use_StringCbPrintf_or_StringCchPrintf;

#undef wsprintfA
#define wsprintfA   wsprintfA_instead_use_StringCbPrintfA_or_StringCchPrintfA;

#undef wsprintfW
#define wsprintfW   wsprintfW_instead_use_StringCbPrintfW_or_StringCchPrintfW;

#undef sprintf
#define sprintf     sprintf_instead_use_StringCbPrintfA_or_StringCchPrintfA;

#undef swprintf
#define swprintf    swprintf_instead_use_StringCbPrintfW_or_StringCchPrintfW;

#undef _stprintf
#define _stprintf   _stprintf_instead_use_StringCbPrintf_or_StringCchPrintf;

#undef wvsprintf
#define wvsprintf   wvsprintf_instead_use_StringCbVPrintf_or_StringCchVPrintf;

#undef wvsprintfA
#define wvsprintfA  wvsprintfA_instead_use_StringCbVPrintfA_or_StringCchVPrintfA;

#undef wvsprintfW
#define wvsprintfW  wvsprintfW_instead_use_StringCbVPrintfW_or_StringCchVPrintfW;

#undef vsprintf
#define vsprintf    vsprintf_instead_use_StringCbVPrintfA_or_StringCchVPrintfA;

#undef vswprintf
#define vswprintf   vswprintf_instead_use_StringCbVPrintfW_or_StringCchVPrintfW;

#undef _vstprintf
#define _vstprintf  _vstprintf_instead_use_StringCbVPrintf_or_StringCchVPrintf;

#undef _vsnprintf
#define _vsnprintf  _vsnprintf_instead_use_StringCbVPrintfA_or_StringCchVPrintfA;

#undef _vsnwprintf
#define _vsnwprintf _vsnwprintf_instead_use_StringCbVPrintfW_or_StringCchVPrintfW;

#undef _vsntprintf
#define _vsntprintf _vsntprintf_instead_use_StringCbVPrintf_or_StringCchVPrintf;

#undef _snprintf
#define _snprintf   _snprintf_instead_use_StringCbPrintfA_or_StringCchPrintfA;

#undef _snwprintf
#define _snwprintf  _snwprintf_instead_use_StringCbPrintfW_or_StringCchPrintfW;

#undef _sntprintf
#define _sntprintf  _sntprintf_instead_use_StringCbPrintf_or_StringCchPrintf;

#undef _getts
#define _getts      _getts_instead_use_StringCbGets_or_StringCchGets;

#undef gets
#define gets        _gets_instead_use_StringCbGetsA_or_StringCchGetsA;

#undef _getws
#define _getws      _getws_instead_use_StringCbGetsW_or_StringCchGetsW;
#endif  // !DEPRECATE_SUPPORTED
#endif  // !STRSAFE_NO_DEPRECATE

#endif  // _STRSAFE_H_INCLUDED_
