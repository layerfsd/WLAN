//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
// Copyright (c) Microsoft Corporation.  All rights reserved. 
//
// Use of this source code is subject to the terms of your Windows Mobile 
// Software Shared Source Premium Derivatives License Form or other 
// applicable Microsoft license agreement for the software. If you did not 
// accept the terms of such a license, you are not authorized to use this 
// source code.
//  *************************************************************************
//
//  miscsvcs_priv.h
//
//  Private Misc OS Services API declarations
//
//
//
//  History: 1 Nov 2002 Created
//
//  *************************************************************************

#pragma once

#include <windows.h>

#define LocalStrDup _tcsdup // free using LocalFree to avoid violating abstraction


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define  REG_szPSAtRootKey TEXT("System\\Platform")
#define  REG_szPSAtRoot    TEXT("PSAtRoot")

//****************************************************************************
// IsPSAtRoot
//
// Purpose: temporary function (while transitioning to Magneto where all devices have 'Persistent' storage)
//                                to check for ps at root
//
// Return Value:
//    TRUE if this is a device with 'persistent' storage mounted as root
//  FALSE if this device does not have PS at root 
BOOL IsPSAtRoot();

//  *************************************************************************
//  MountDBVol
//
//  Purpose:
//      Creates the required database volume.
//      The API is extensible for future with the enum flag
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
typedef enum
{ 
    eDBVolPIM,
    eDBVolPhone,
    eDBVolSystem,
    eDBVolSync    
} enumDBVol;
HRESULT MountDBVol(enumDBVol eDBVol, PCEGUID pceguid);

HRESULT CreatePimDatabases();

HRESULT LocalEnsureBufferSize(UINT cbCurrent, UINT cbNew, void** ppv);

void memswap(void* pvA, void* pvB, UINT cb);


//  *************************************************************************
//  VerifyString
//
//  Purpose:
//      Wrapper for GetStringTypeEx. Verifies that the specified string 
//      does not contain characters on the specified type.
//
//  Parameters:
//      psz      - the string
//      dwTypes  - combination of the CT_CTYPE3 flags(C3_KATAKANA, C3_HIRAGANA, C3_ALPHA, etc)
//      dwFlags  - combination of flags
//
//  Returns:
//      TRUE if the string does NOT contain characaters of the specified type(s)
//      FALSE if the string does contain characters of the specified type(s)
//
//  Side Effects:
//      none
//  *************************************************************************
BOOL VerifyString(LPTSTR psz, DWORD dwTypes, DWORD dwFlags);

// Flags for VerifyString:
#define VSF_HANGUEL 0x00000001   // Check for Hanguel characters

// Checks if the given string contains any Asian characters.
BOOL IsFEString(LPTSTR psz);

//
// Generic implementation of IStream::CopyTo()
// Given two streams, copies cb bytes of data from one to the other.
//
HRESULT StreamCopyTo
(
    IStream *pstmFrom, 
    IStream *pstmTo, 
    ULARGE_INTEGER cb, 
    ULARGE_INTEGER *pcbRead, 
    ULARGE_INTEGER *pcbWritten
);


//  *************************************************************************
//  InvStrCmp[N|I]
//
//  Purpose:
//      Wrapper to call CompareString in a locale-independent way.
//
//  Parameters:
//      pszString1 - the first string to compare
//      pszString2 - the second string to compare
//      cch        - the maximum number of characters to compare
//                   If either string is shorter than cch characters, and
//                   null-terminated, the comparison wil stop at the
//                   termination character. If either of the strings is NOT
//                   null-terminated, it is the caller's responsibility to
//                   ensure that cch is set properly.
//
//  Returns:
//      -1 if pszString1 is less than pszString2
//      0 if pszString1 and pszString2 are equal
//      1 if pszString1 is greater than pszString2
//
//  Side Effects:
//      none
//  *************************************************************************

// Internal implementation of the InvStrCmp functions. Note that the
// StringCchLength functions can and WILL fail when <cch> is less than the
// actual length of the string, which is a perfectly valid scenario. Under that
// circumstance, the string length will not be modified, and we'll only compare
// <cch> characters.
#define INVSTRCMP_INTERNALS(dwCmpFlags, pszString1, pszString2, cch) \
    size_t cchMax = ((size_t)(cch) < STRSAFE_MAX_CCH ? (size_t)(cch) : STRSAFE_MAX_CCH); \
    size_t cchString1 = (size_t)(cch), cchString2 = (size_t)(cch); \
    StringCchLength((pszString1), cchMax, &cchString1); \
    StringCchLength((pszString2), cchMax, &cchString2); \
    return (CompareString(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT), \
        (dwCmpFlags), (pszString1), (int)cchString1, (pszString2), (int)cchString2) - CSTR_EQUAL);

// Define INVSTRCMP_AS_INLINE to get the functions inlined into your code (and
// avoid linking to ossvcs.dll for these functions). Normally, however, you
// should use the normal, linked versions of the functions, which cost at least
// 28 bytes less ROM per call than the inline versions
#ifdef INVSTRCMP_AS_INLINE
__inline int InvStrCmpW(__in LPCWSTR pszString1, __in LPCWSTR pszString2) { INVSTRCMP_INTERNALS(0, pszString1, pszString2, -1); }
__inline int InvStrCmpIW(__in LPCWSTR pszString1, __in LPCWSTR pszString2) { INVSTRCMP_INTERNALS(NORM_IGNORECASE, pszString1, pszString2, -1); }
__inline int InvStrCmpNW(__in LPCWSTR pszString1, __in LPCWSTR pszString2, size_t cch) { INVSTRCMP_INTERNALS(0, pszString1, pszString2, cch); }
__inline int InvStrCmpNIW(__in LPCWSTR pszString1, __in LPCWSTR pszString2, size_t cch) { INVSTRCMP_INTERNALS(NORM_IGNORECASE, pszString1, pszString2, cch); }
#else
int InvStrCmpW(__in LPCWSTR pszString1, __in LPCWSTR pszString2);
int InvStrCmpIW(__in LPCWSTR pszString1, __in LPCWSTR pszString2);
int InvStrCmpNW(__in LPCWSTR pszString1, __in LPCWSTR pszString2, size_t cch);
int InvStrCmpNIW(__in LPCWSTR pszString1, __in LPCWSTR pszString2, size_t cch);
#endif

#ifdef UNICODE
#define InvStrCmp   InvStrCmpW
#define InvStrCmpI  InvStrCmpIW
#define InvStrCmpN  InvStrCmpNW
#define InvStrCmpNI InvStrCmpNIW
#endif

// **************************************************************************
// SafeWideCharToMultiByte
// 
// Purpose: Same as WideCharToMultiByte, except guarantees NULL termination.
//
// Parameters:
//  UINT    CodePage            [in]  code page
//  DWORD   dwFlags             [in]  performance and mapping flags
//  LPCWSTR lpWideCharStr       [in]  wide-character string
//  int     cchWideChar,        [in]  number of chars in string.
//  LPSTR   lpMultiByteStr,     [out] buffer for new string
//  int     cbMultiByte,        [in]  size of buffer
//  LPCSTR  lpDefaultChar,      [in]  default for unmappable chars
//  LPBOOL  lpUsedDefaultChar   [out] set when default char used
//
// Return Values:
//      0 on error, otherwise the number of bytes written to lpMultiByteStr,
//       including the NULL terminator
//
// **************************************************************************
int SafeWideCharToMultiByte(
                                    UINT    CodePage,
                                    DWORD   dwFlags,
    __in_ecount(cchWideChar)        LPCWSTR lpWideCharStr,
                                    int     cchWideChar,
    __out_ecount(cbMultiByte) __opt LPSTR   lpMultiByteStr,
                                    int     cbMultiByte,
    __in __opt                      LPCSTR  lpDefaultChar,
    __out __opt                     LPBOOL  lpUsedDefaultChar);


// **************************************************************************
// SafeMultiByteToWideChar
// 
// Purpose: Same as MultiByteToWideChar, except guarantees NULL termination.
//
// Parameters:
//  UINT    CodePage            [in]  code page
//  DWORD   dwFlags             [in]  performance and mapping flags
//  LPSTR   lpMultiByteStr,     [in]  string to map
//  int     cbMultiByte,        [in]  number of bytes in string
//  LPCWSTR lpWideCharStr       [out] wide-character buffer
//  int     cchWideChar,        [in]  size of buffer
//
// Return Values:
//      0 on error, otherwise the number of characters written to lpWideCharStr,
//      including the NULL terminator
//
// **************************************************************************
int SafeMultiByteToWideChar(
                                    UINT    CodePage,         
                                    DWORD   dwFlags,         
    __in_ecount(cbMultiByte)        LPCSTR  lpMultiByteStr, 
                                    int     cbMultiByte,       
    __out_ecount(cchWideChar) __opt LPWSTR  lpWideCharStr,  
                                    int     cchWideChar        
);


/// <summary>
///     Convert a string representing an unsigned DWORD
///     into an actual DWORD type.
/// </summary>
/// <param name="pszNumber">[in] String to parse</param>
/// <param name="nNumberBase">[in] Numeric base of the number in the string</param>
/// <param name="fIgnoreTrailingSpaces">[in] Flag indicating whether trailing spaces are permitted</param>
/// <param name="pdwNumber">[out] DWORD-type value</param>
/// <returns>
///     <para>Returns HRESULT type</para>
/// </returns>
/// <remarks>
///     <para>
///         The grammar that this function accepts is a more restrictive form 
///         for that of _tcstoul:  [+][0[x|X]][digits][whitespace]
///
///         The only bases allowed are 10 (decimal) and 16 (hexadecimal).
///     </para>
/// </remarks>
HRESULT SafeStringToDword(
    __in    LPCTSTR pszNumber, 
            int     nNumberBase,
            BOOL    fIgnoreTrailingSpaces,
    __out   DWORD*  pdwNumber);


// Used to map and copy embedded PSL string pointers
HRESULT MapAndCopyString(LPTSTR *ppszDest, LPCTSTR pszSrc);

// Used to copy data from an untrusted source
HRESULT SafeStrDup(LPCTSTR psz, size_t cchMax, __deref_out LPTSTR * ppszDup);

// Used to copy data from an untrusted source
HRESULT SafeBinDup(
    __in_ecount_opt(cbIn)           const LPBYTE    pbIn,
                                    DWORD           cbIn,
    __deref_out_ecount_opt(*pcbOut) LPBYTE*         ppbOut,
    __out                           DWORD*          pcbOut);

// SMul
// Multiply two signed integers.  Return TRUE if there is no overflow.
BOOL SMul(signed int factor1, signed int factor2, signed int *pProduct);

// SsMul
// Multiply two signed shorts.  Return TRUE if there is no overflow.
BOOL SsMul(signed short factor1, signed short factor2, signed short *pProduct);

// UMul
// Multiply two unsigned integers.  Return TRUE if there is no overflow.
BOOL UMul(unsigned int factor1, unsigned int factor2, unsigned int *pProduct);


// UsMul
// Multiply two unsigned shorts.  Return TRUE if there is no overflow.
BOOL UsMul(unsigned short factor1, unsigned short factor2, unsigned short *pProduct);

// SAdd
// Add two signed integers.  Return TRUE if there is no overflow.
BOOL SAdd(signed int addend1, signed int addend2, signed int *pSum);

// SsAdd
// Add two signed shorts.  Return TRUE if there is no overflow.
BOOL SsAdd(signed short addend1, signed short addend2, signed short *pSum);



// UAdd
// Add two unsigned integers.  Return TRUE if there is no overflow.
__inline BOOL UAdd(unsigned int addend1, unsigned int addend2, unsigned int *pSum)
{
    *pSum = addend1 + addend2;
    return *pSum >= addend1;
}

// UsAdd
// Add two unsigned shorts.  Return TRUE if there is no overflow.
__inline BOOL UsAdd(unsigned short addend1, unsigned short addend2, unsigned short *pSum)
{
    *pSum = addend1 + addend2;
    return *pSum >= addend1;
}


// DwAdd
// Add two DWORDs.  Return TRUE if there is no overflow.
__inline BOOL DwAdd(DWORD addend1, DWORD addend2, DWORD *pSum)
{
    *pSum = addend1 + addend2;
    return *pSum >= addend1;
}

// DwMul
// Multiply two DWORDs.  Return TRUE if there is no overflow.
__inline BOOL DwMul(DWORD factor1, DWORD factor2, DWORD *pProduct)
{
    return UMul((unsigned int)factor1, (unsigned int)factor2, (unsigned int *)pProduct);
}




// CharsToBytes()
//
// Convert a character count to a byte count.   
// Returns TRUE if there is no overflow.
// Useful for preventing integer overflow attacks.
//
// Note: Even though cch is declared as unsigned, you can pass in
// a signed value.  However, pcb should always be unsigned.
__inline BOOL CharsToBytes (UINT cch, UINT *pcb)
{
    ASSERT(sizeof(TCHAR) == 2);
    *pcb = cch * 2;
    // If cch is unsigned, the high bit being set is bad because it will 
    // cause overflow.
    // If cch is signed, the high bit being set is bad because it means 
    // cch is negative, which we'll assume for the purposes of this function
    // is not a valid "count of characters".
    return 0 == (cch & (1 << ((sizeof(int) * 8) - 1)));
}    



//  *************************************************************************
//  IsFullBoot
//
//  Purpose: returns TRUE if we are fully booted, or FALSE if not
//           similar to WaitForFullBoot (implemented in the same place - but 
//                                          defined in public\oak\miscsvcs.h)
//
//  *************************************************************************
BOOL IsFullBoot();


//  *************************************************************************
//  WaitForShellReady
//  
//  Purpose: Wait for a shell being ready.  This call blocks until the past
//           paint message sent to Today/Home screen at boot time.
//  
// **************************************************************************
void WaitForShellReady();


//  *************************************************************************
//  IsShellReady
//  
//  Purpose: Wait for a SingleObject - an Shell_Ready Event, using 0 for the
//           time. SingleObject will return WAIT_OBJECT_0 if Shell_Ready Event
//           is signaled which will cause IsShellReady to return TRUE, else
//           return FALSE
//  
//  *************************************************************************
BOOL IsShellReady();


/// <summary>
///     RecursiveCreateDirectory
///     Recusively creates directores specified in pszPath, if they do not exist
/// </summary>
/// <param name="pszDir">
///     [IN] LPTSTR
///     Directory path to create
/// </param>
/// <returns>
///     HRESULT, error code
/// </returns>
/// <remarks>
/// </remarks>
HRESULT RecursiveCreateDirectory(__in LPTSTR pszDir);


// **************************************************************************
// RecursiveRemoveDirectory
// 
// Purpose: This function will delete all the contents of the directory and 
//          the directory itself. If the directory contains sub-directories, 
//          this function will be called recursively on each of those 
//          sub-directories
//
// Arguments:
//    IN LPCTSTR pszDir - Directory to delete
//
// Return Values:
//    HRESULT
//    returns an error code indicating success or failure
//
// Comments: The same functionality is provided by the shell API 
//           SHFileOperation(). Whenever possible, it is recommended
//           to use that function because it checks the security against
//           metabase to see if the caller has access to the directory
//           and files they are trying to remove. 
//           RecursiveRemoveDirectory should be used only in cases
//           where you know you want to delete some system files or
//           otherwise restricted (in the metabase) files.
//
// **************************************************************************
HRESULT RecursiveRemoveDirectory(LPCTSTR pszDir);

INT GetProcHeapUsage(DWORD dwProcID);

#ifdef __cplusplus
};
#endif // __cplusplus

