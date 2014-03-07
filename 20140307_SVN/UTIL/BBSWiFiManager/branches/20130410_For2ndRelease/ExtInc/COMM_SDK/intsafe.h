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
*  intsafe.h -- This module defines helper functions to prevent   *
*               integer overflow bugs.                            *
*                                                                 *
*                                                                 *
******************************************************************/
#ifndef _INTSAFE_H_INCLUDED_
#define _INTSAFE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include <specstrings.h>    // for __in, etc.


#if defined(_AMD64_) && !defined(UnsignedMultiply128)
#ifdef __cplusplus
extern "C" {
#endif
#define UnsignedMultiply128 _umul128
ULONG64
UnsignedMultiply128 (
    IN  ULONG64  Multiplier,
    IN  ULONG64  Multiplicand,
    OUT ULONG64 *HighProduct
    );
#pragma intrinsic(_umul128)
#ifdef __cplusplus
}
#endif
#endif // _AMD64_ && !UnsignedMultiply128

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


#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif // !_HRESULT_DEFINED

#ifndef S_OK
#define S_OK  ((HRESULT)0x00000000L)
#endif

#define INTSAFE_E_ARITHMETIC_OVERFLOW       ((HRESULT)0x80070216L)  // 0x216 = 534 = ERROR_ARITHMETIC_OVERFLOW

#ifndef LOWORD
#define LOWORD(_l)      ((unsigned short)((unsigned long)(_l) & 0xffff))
#endif // !LOWORD

#ifndef HIWORD
#define HIWORD(_l)      ((unsigned short)((unsigned long)(_l) >> 16))
#endif // !HIWORD

#ifndef HIDWORD
#define HIDWORD(_qw)    (unsigned long)((_qw)>>32)
#endif // !HIDWORD

#ifndef LODWORD
#define LODWORD(_qw)    (unsigned long)(_qw)
#endif // !LODWORD

#ifndef UInt32x32To64
#define UInt32x32To64(a, b) ((unsigned __int64)((unsigned long)(a)) * (unsigned __int64)((unsigned long)(b)))
#endif // !UInt32x32To64

#ifndef INT_MAX
#define INT_MAX         2147483647
#endif // !INT_MAX

//
// It is common for -1 to be used as an error value for various types
//
#define USHORT_ERROR    (0xffff)
#define INT_ERROR       (2147483647)
#define UINT_ERROR      (0xffffffff)
#define ULONG_ERROR     (0xffffffff)
#define SIZET_ERROR     ((size_t)-1)
#define ULONGLONG_ERROR (0xffffffffffffffff)

//
// We make some assumptions about the sizes of various types. Let's be
// explicit about those assumptions and check them.
//
C_ASSERT(sizeof(unsigned short) == 2);
C_ASSERT(sizeof(unsigned int) == 4);
C_ASSERT(sizeof(unsigned long) == 4);


//
// INT -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
#define IntToChar(iOperand, pch)        IntToUChar(iOperand, pch)
#else
__inline
HRESULT
IntToChar(
    __in int iOperand,
    __out char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = '\0';

    if ((iOperand >= -128) && (iOperand <= 127))
    {
        *pch = (char)iOperand;
        hr = S_OK;
    }

    return hr;
}
#endif // _CHAR_UNSIGNED

//
// INT -> UCHAR conversion
//
__inline
HRESULT
IntToUChar(
    __in int iOperand,
    __out unsigned char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = '\0';

    if ((iOperand >= 0) && (iOperand <= 255))
    {
        *pch = (unsigned char)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> BYTE conversion
//
#define IntToByte(uOperand, pResult)    IntToUChar(uOperand, pResult)

//
// UINT -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
#define UIntToChar(uOperand, pch)       UIntToUChar(uOperand, pch)
#else
__inline
HRESULT
UIntToChar(
    __in unsigned int uOperand,
    __out char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = '\0';

    if (uOperand <= 127)
    {
        *pch = (char)uOperand;
        hr = S_OK;
    }

    return hr;
}
#endif // _CHAR_UNSIGNED

//
// UINT -> UCHAR conversion
//
__inline
HRESULT
UIntToUChar(
    __in unsigned int uOperand,
    __out unsigned char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = '\0';

    if (uOperand <= 255)
    {
        *pch = (unsigned char)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// UINT -> BYTE conversion
//
#define UIntToByte(uOperand, pResult)   UIntToUChar(uOperand, pResult)

//
// INT -> UINT conversion
//
__inline
HRESULT
IntToUInt(
    __in int iOperand,
    __out unsigned int* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

    if (iOperand >= 0)
    {
        *puResult = (unsigned int)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// UINT -> INT conversion
//
__inline
HRESULT
UIntToInt(
    __in unsigned int uOperand,
    __out int* piResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *piResult = INT_ERROR;

    if (uOperand <= INT_MAX)
    {
        *piResult = (int)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// UINT -> USHORT conversion
//
__inline
HRESULT
UIntToUShort(
    __in unsigned int uOperand,
    __out unsigned short* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (HIWORD(uOperand) == 0)
    {
        *pusResult = LOWORD(uOperand);
        hr = S_OK;
    }

    return hr;
}

//
// UINT -> WORD conversion
//
#define UIntToWord(uOperand, pwResult)      UIntToUShort(uOperand, pwResult)

//
// ULONGLONG -> ULONG conversion
//
__inline
HRESULT
ULongLongToULong(
    __in unsigned __int64 ullOperand,
    __out unsigned long* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;
    
    if (HIDWORD(ullOperand) == 0)
    {
        *pulResult = LODWORD(ullOperand);
        hr = S_OK;
    }
    
    return hr;
}

//
// ULONGLONG -> UINT conversion
//
__inline
HRESULT
ULongLongToUInt(
    __in unsigned __int64 ullOperand,
    __out unsigned int* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;
    
    if (HIDWORD(ullOperand) == 0)
    {
        *puResult = LODWORD(ullOperand);
        hr = S_OK;
    }
    
    return hr;
}

//
// USHORT addition
//
__inline
HRESULT
UShortAdd(
    __in unsigned short usAugend,
    __in unsigned short usAddend,
    __out unsigned short* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if ((unsigned short)(usAugend + usAddend) >= usAugend)
    {
        *pusResult = usAugend + usAddend;
        hr = S_OK;
    }
    
    return hr;
}

//
// WORD addtition
//
#define WordAdd(wAugend, wAddend, pwResult)         UShortAdd(wAugend, wAddend, pwResult)

//
// UINT addition
//
__inline
HRESULT
UIntAdd(
    __in unsigned int uAugend,
    __in unsigned int uAddend,
    __out unsigned int* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

    if ((unsigned int)(uAugend + uAddend) >= uAugend)
    {
        *puResult = uAugend + uAddend;
        hr = S_OK;
    }
    
    return hr;
}

//
// UINT_PTR addition
//
#ifdef _WIN64
#define UIntPtrAdd(uAugend, uAddend, puResult)      ULongLongAdd(uAugend, uAddend, puResult)
#else
#define UIntPtrAdd(uAugend, uAddend, puResult)      UIntAdd(uAugend, uAddend, puResult)
#endif // _WIN64

//
// ULONG addition
//
__inline
HRESULT
ULongAdd(
    __in unsigned long ulAugend,
    __in unsigned long ulAddend,
    __out unsigned long* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;

    if ((unsigned long)(ulAugend + ulAddend) >= ulAugend)
    {
        *pulResult = ulAugend + ulAddend;
        hr = S_OK;
    }
    
    return hr;
}

//
// ULONG_PTR addition
//
#ifdef _WIN64
#define ULongPtrAdd(ulAugend, ulAddend, pulResult)  ULongLongAdd(ulAugend, ulAddend, pulResult)
#else
#define ULongPtrAdd(ulAugend, ulAddend, pulResult)  ULongAdd(ulAugend, ulAddend, pulResult)
#endif // _WIN64

//
// DWORD addition
//
#define DWordAdd(dwAugend, dwAddend, pdwResult)     ULongAdd(dwAugend, dwAddend, pdwResult)

//
// DWORD_PTR addition
//
#ifdef _WIN64
#define DWordPtrAdd(dwAugend, dwAddend, pdwResult)  ULongLongAdd(dwAugend, dwAddend, pdwResult)
#else
#define DWordPtrAdd(dwAugend, dwAddend, pdwResult)  ULongAdd(dwAugend, dwAddend, pdwResult)
#endif // _WIN64

//
// size_t addition
//
__inline
HRESULT
SizeTAdd(
    __in size_t Augend,
    __in size_t Addend,
    __out size_t* pResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pResult = SIZET_ERROR;

    if ((Augend + Addend) >= Augend)
    {
        *pResult = Augend + Addend;
        hr = S_OK;
    }
    
    return hr;
}

//
// SIZE_T addition
//
#ifdef _WIN64
#define SIZETAdd(Augend, Addend, pResult)       ULongLongAdd(Augend, Addend, pResult)
#else
#define SIZETAdd(Augend, Addend, pResult)       ULongAdd(Augend, Addend, pResult)
#endif // _WIN64

//
// ULONGLONG addition
//
__inline
HRESULT
ULongLongAdd(
    __in unsigned __int64 ullAugend,
    __in unsigned __int64 ullAddend,
    __out unsigned __int64* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = ULONGLONG_ERROR;

    if ((unsigned __int64)(ullAugend + ullAddend) >= ullAugend)
    {
        *pullResult = ullAugend + ullAddend;
        hr = S_OK;
    }
    
    return hr;
}

//
// USHORT subtraction
//
__inline
HRESULT
UShortSub(
    __in unsigned short usMinuend,
    __in unsigned short usSubtrahend,
    __out unsigned short* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (usMinuend >= usSubtrahend)
    {
        *pusResult = usMinuend - usSubtrahend;
        hr = S_OK;
    }
    
    return hr;
}

//
// WORD subtraction
//
#define WordSub(wMinuend, wSubtrahend, pwResult)    UShortSub(wMinuend, wSubtrahend, pwResult)


//
// UINT subtraction
//
__inline
HRESULT
UIntSub(
    __in unsigned int uMinuend,
    __in unsigned int uSubtrahend,
    __out unsigned int* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

    if (uMinuend >= uSubtrahend)
    {
        *puResult = uMinuend - uSubtrahend;
        hr = S_OK;
    }
    
    return hr;
}

//
// UINT_PTR subtraction
//
#ifdef _WIN64
#define UIntPtrSub(uMinuend, uSubtrahend, puResult)     ULongLongSub(uMinuend, uSubtrahend, puResult)
#else
#define UIntPtrSub(uMinuend, uSubtrahend, puResult)     UIntSub(uMinuend, uSubtrahend, puResult)
#endif // _WIN64

//
// ULONG subtraction
//
__inline
HRESULT
ULongSub(
    __in unsigned long ulMinuend,
    __in unsigned long ulSubtrahend,
    __out unsigned long* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;

    if (ulMinuend >= ulSubtrahend)
    {
        *pulResult = ulMinuend - ulSubtrahend;
        hr = S_OK;
    }
    
    return hr;
}

//
// ULONG_PTR subtraction
//
#ifdef _WIN64
#define ULongPtrSub(ulMinuend, ulSubtrahend, pulResult)     ULongLongSub(ulMinuend, ulSubtrahend, pulResult)
#else
#define ULongPtrSub(ulMinuend, ulSubtrahend, pulResult)     ULongSub(ulMinuend, ulSubtrahend, pulResult)
#endif // _WIN64


//
// DWORD subtraction
//
#define DWordSub(dwMinuend, dwSubtrahend, pdwResult)        ULongSub(dwMinuend, dwSubtrahend, pdwResult)

//
// DWORD_PTR subtraction
//
#ifdef _WIN64
#define DWordPtrSub(dwMinuend, dwSubtrahend, pdwResult)     ULongLongSub(dwMinuend, dwSubtrahend, pdwResult)
#else
#define DWordPtrSub(dwMinuend, dwSubtrahend, pdwResult)     ULongSub(dwMinuend, dwSubtrahend, pdwResult)
#endif // _WIN64

//
// size_t subtraction
//
__inline
HRESULT
SizeTSub(
    __in size_t Minuend,
    __in size_t Subtrahend,
    __out size_t* pResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pResult = SIZET_ERROR;

    if (Minuend >= Subtrahend)
    {
        *pResult = Minuend - Subtrahend;
        hr = S_OK;
    }
    
    return hr;
}

//
// SIZE_T subtraction
//
#ifdef _WIN64
#define SIZETSub(Minuend, Subtrahend, pResult)       ULongLongSub(Minuend, Subtrahend, pResult)
#else
#define SIZETSub(Minuend, Subtrahend, pResult)       ULongSub(Minuend, Subtrahend, pResult)
#endif // _WIN64

//
// ULONGLONG subtraction
//
__inline
HRESULT
ULongLongSub(
    __in unsigned __int64 ullMinuend,
    __in unsigned __int64 ullSubtrahend,
    __out unsigned __int64* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = ULONGLONG_ERROR;

    if (ullMinuend >= ullSubtrahend)
    {
        *pullResult = ullMinuend - ullSubtrahend;
        hr = S_OK;
    }
    
    return hr;
}

//
// USHORT multiplication
//
__inline
HRESULT
UShortMult(
    __in unsigned short usMultiplicand,
    __in unsigned short usMultiplier,
    __out unsigned short* pusResult)
{
    unsigned long ulResult = (unsigned long)usMultiplicand * usMultiplier;
    
    return UIntToUShort(ulResult, pusResult);
}

//
// WORD multiplication
//
#define WordMult(wMultiplicand, wMultiplier, pwResult)      UShortMult(wMultiplicand, wMultiplier, pwResult)

//
// UINT multiplication
//
__inline
HRESULT
UIntMult(
    __in unsigned int uMultiplicand,
    __in unsigned int uMultiplier,
    __out unsigned int* puResult)
{
    unsigned __int64 ull64Result = UInt32x32To64(uMultiplicand, uMultiplier);

    return ULongLongToUInt(ull64Result, puResult);
}

//
// UINT_PTR multiplication
//
#ifdef _WIN64
#define UIntPtrMult(uMultiplicand, uMultiplier, puResult)       ULongLongMult(uMultiplicand, uMultiplier, puResult)
#else
#define UIntPtrMult(uMultiplicand, uMultiplier, puResult)       UIntMult(uMultiplicand, uMultiplier, puResult)
#endif // _WIN64

//
// ULONG multiplication
//
__inline
HRESULT
ULongMult(
    __in unsigned long ulMultiplicand,
    __in unsigned long ulMultiplier,
    __out unsigned long* pulResult)
{
    unsigned __int64 ull64Result = UInt32x32To64(ulMultiplicand, ulMultiplier);
    
    return ULongLongToULong(ull64Result, pulResult);
}

//
// ULONG_PTR multiplication
//
#ifdef _WIN64
#define ULongPtrMult(ulMultiplicand, ulMultiplier, pulResult)   ULongLongMult(ulMultiplicand, ulMultiplier, pulResult)
#else
#define ULongPtrMult(ulMultiplicand, ulMultiplier, pulResult)   ULongMult(ulMultiplicand, ulMultiplier, pulResult)
#endif // _WIN64


//
// DWORD multiplication
//
#define DWordMult(dwMultiplicand, dwMultiplier, pdwResult)      ULongMult(dwMultiplicand, dwMultiplier, pdwResult)

//
// DWORD_PTR multiplication
//
#ifdef _WIN64
#define DWordPtrMult(dwMultiplicand, dwMultiplier, pdwResult)   ULongLongMult(dwMultiplicand, dwMultiplier, pdwResult)
#else
#define DWordPtrMult(dwMultiplicand, dwMultiplier, pdwResult)   ULongMult(dwMultiplicand, dwMultiplier, pdwResult)
#endif // _WIN64

//
// size_t multiplication
//

#ifdef _WIN64
#define SizeTMult(Multiplicand, Multiplier, pResult)        ULongLongMult(Multiplicand, Multiplier, pResult)
#else
#define SizeTMult(Multiplicand, Multiplier, pResult)        UIntMult(Multiplicand, Multiplier, pResult)
#endif // _WIN64

//
// SIZE_T multiplication
//
#ifdef _WIN64
#define SIZETMult(Multiplicand, Multiplier, pResult)        ULongLongMult(Multiplicand, Multiplier, pResult)
#else
#define SIZETMult(Multiplicand, Multiplier, pResult)        ULongMult(Multiplicand, Multiplier, pResult)
#endif // _WIN64

//
// ULONGLONG multiplication
//
__inline
HRESULT
ULongLongMult(
    __in unsigned __int64 ullMultiplicand,
    __in unsigned __int64 ullMultiplier,
    __out unsigned __int64* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
#ifdef _AMD64_
    unsigned __int64 u64ResultHigh;
    unsigned __int64 u64ResultLow;
    
    *pullResult = ULONGLONG_ERROR;
    
    u64ResultLow = UnsignedMultiply128(ullMultiplicand, ullMultiplier, &u64ResultHigh);
    if (u64ResultHigh == 0)
    {
        *pullResult = u64ResultLow;
        hr = S_OK;
    }
#else
    // 64x64 into 128 is like 32.32 x 32.32.
    //
    // a.b * c.d = a*(c.d) + .b*(c.d) = a*c + a*.d + .b*c + .b*.d
    // back in non-decimal notation where A=a*2^32 and C=c*2^32:  
    // A*C + A*d + b*C + b*d
    // So there are four components to add together.
    //   result = (a*c*2^64) + (a*d*2^32) + (b*c*2^32) + (b*d)
    //
    // a * c must be 0 or there would be bits in the high 64-bits
    // a * d must be less than 2^32 or there would be bits in the high 64-bits
    // b * c must be less than 2^32 or there would be bits in the high 64-bits
    // then there must be no overflow of the resulting values summed up.
    
    unsigned long dw_a;
    unsigned long dw_b;
    unsigned long dw_c;
    unsigned long dw_d;
    unsigned __int64 ac = 0;
    unsigned __int64 ad = 0;
    unsigned __int64 bc = 0;
    unsigned __int64 bd = 0;
    unsigned __int64 ullResult = 0;
    
    *pullResult = ULONGLONG_ERROR;

    dw_a = HIDWORD(ullMultiplicand);
    dw_c = HIDWORD(ullMultiplier);

    // common case -- if high dwords are both zero, no chance for overflow
    if ((dw_a == 0) && (dw_c == 0))
    {
        dw_b = LODWORD(ullMultiplicand);
        dw_d = LODWORD(ullMultiplier);

        *pullResult = (unsigned __int64)dw_b * (unsigned __int64)dw_d;
        hr = S_OK;
    }
    else
    {
        // a * c must be 0 or there would be bits set in the high 64-bits
        if ((dw_a == 0) ||
            (dw_c == 0) ||
            (((ac = (unsigned __int64)dw_a * (unsigned __int64)dw_c)) == 0))
        {
            dw_d = LODWORD(ullMultiplier);

            // a * d must be less than 2^32 or there would be bits set in the high 64-bits
            ad = (unsigned __int64)dw_a * (unsigned __int64)dw_d;
            if (ad <= 4294967295)
            {
                dw_b = LODWORD(ullMultiplicand);

                // b * c must be less than 2^32 or there would be bits set in the high 64-bits
                bc = (unsigned __int64)dw_b * (unsigned __int64)dw_c;
                if (bc <= 4294967295)
                {
                    // now sum them all up checking for overflow.
                    // shifting is safe because we already checked for overflow above
#ifdef _WIN64 
                    if (SUCCEEDED(ULongLongAdd(bc << 32, ad << 32, &ullResult)))                        
#else
                    // << 32 not supported on 32-bit compilers
                    if (SUCCEEDED(ULongLongAdd(bc * 4294967295, ad * 4294967295, &ullResult)))
#endif                                      
                    {
                        // b * d
                        bd = (unsigned __int64)dw_b * (unsigned __int64)dw_d;
                    
                        if (SUCCEEDED(ULongLongAdd(ullResult, bd, &ullResult)))
                        {
                            *pullResult = ullResult;
                            hr = S_OK;
                        }
                    }
                }
            }
        }
    }
#endif // _AMD64_  
    
    return hr;
}

#ifdef UNDER_CE
// Extra helper functions specific to CE

__inline
HRESULT
CeULongAdd3(
	__in unsigned long arg1,
	__in unsigned long arg2,
	__in unsigned long arg3,
	__out unsigned long* pResult)
{
	if(SUCCEEDED(ULongAdd(arg1,arg2,pResult)) &&
		SUCCEEDED(ULongAdd(*pResult,arg3,pResult))){
		return S_OK;
	}

	return INTSAFE_E_ARITHMETIC_OVERFLOW;
}

__inline
HRESULT
CeULongAdd4(
	__in unsigned long arg1,
	__in unsigned long arg2,
	__in unsigned long arg3,
	__in unsigned long arg4,
	__out unsigned long* pResult)
{
	if(SUCCEEDED(ULongAdd(arg1,arg2,pResult)) &&
		SUCCEEDED(ULongAdd(*pResult,arg3,pResult)) &&
		SUCCEEDED(ULongAdd(*pResult,arg4,pResult))){
		return S_OK;
	}

	return INTSAFE_E_ARITHMETIC_OVERFLOW;
}

__inline
HRESULT
CeULongAdd10(
	__in unsigned long arg1,
	__in unsigned long arg2,
	__in unsigned long arg3,
	__in unsigned long arg4,
	__in unsigned long arg5,
	__in unsigned long arg6,
	__in unsigned long arg7,
	__in unsigned long arg8,
	__in unsigned long arg9,
	__in unsigned long arg10,
    __out unsigned long* pResult)
{
	if(SUCCEEDED(CeULongAdd4(arg1,arg2,arg3,arg4,pResult)) &&
		SUCCEEDED(CeULongAdd4(*pResult,arg5,arg6,arg7,pResult)) &&
		SUCCEEDED(CeULongAdd4(*pResult,arg8,arg9,arg10,pResult))){
		return S_OK;
	}

	return INTSAFE_E_ARITHMETIC_OVERFLOW;
}


// (arg1*arg2) + arg3
__inline
HRESULT
CeULongMult2Add(
	__in unsigned long arg1,
	__in unsigned long arg2,
	__in unsigned long arg3,
	__out unsigned long* pResult)
{
	if(SUCCEEDED(ULongMult(arg1,arg2,pResult)) &&
		SUCCEEDED(ULongAdd(*pResult,arg3,pResult))){
		return S_OK;
	}

	return INTSAFE_E_ARITHMETIC_OVERFLOW;
}

// (arg1+arg2) * arg3
__inline
HRESULT
CeULongAdd2Mult(
	__in unsigned long arg1,
	__in unsigned long arg2,
	__in unsigned long arg3,
	__out unsigned long* pResult)
{
	if(SUCCEEDED(ULongAdd(arg1,arg2,pResult)) &&
		SUCCEEDED(ULongMult(*pResult,arg3,pResult))){
		return S_OK;
	}

	return INTSAFE_E_ARITHMETIC_OVERFLOW;
}

#define CeDWordAdd3(arg1,arg2,arg3,pResult) CeULongAdd3(arg1,arg2,arg3,pResult)

#define CeDWordAdd4(arg1,arg2,arg3,arg4,pResult) CeULongAdd4(arg1,arg2,arg3,arg4,pResult)

#define CeDWordMult2Add(arg1,arg2,arg3,pResult) CeULongMult2Add(arg1,arg2,arg3,pResult)

#define CeDWordAdd2Mult(arg1,arg2,arg3,pResult) CeULongAdd2Mult(arg1,arg2,arg3,pResult)

__inline
HRESULT
CeUIntAdd3(
	__in unsigned int arg1,
	__in unsigned int arg2,
	__in unsigned int arg3,
	__out unsigned int* pResult)
{
	if(SUCCEEDED(UIntAdd(arg1,arg2,pResult)) &&
		SUCCEEDED(UIntAdd(*pResult,arg3,pResult))){
		return S_OK;
	}

	return INTSAFE_E_ARITHMETIC_OVERFLOW;
}

__inline
HRESULT
CeUIntAdd4(
	__in unsigned int arg1,
	__in unsigned int arg2,
	__in unsigned int arg3,
	__in unsigned int arg4,
	__out unsigned int* pResult)
{
	if(SUCCEEDED(UIntAdd(arg1,arg2,pResult)) &&
		SUCCEEDED(UIntAdd(*pResult,arg3,pResult)) &&
		SUCCEEDED(UIntAdd(*pResult,arg4,pResult))){
		return S_OK;
	}

	return INTSAFE_E_ARITHMETIC_OVERFLOW;
}

#endif



#endif // _INTSAFE_H_INCLUDED_
