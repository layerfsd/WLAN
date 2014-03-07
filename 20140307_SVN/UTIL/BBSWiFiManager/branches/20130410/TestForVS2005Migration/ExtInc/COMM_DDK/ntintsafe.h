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
*  ntintsafe.h -- This module defines helper functions to prevent *
*                 integer overflow bugs for drivers. A similar    *
*                 file, intsafe.h, is available for applications. *
*                                                                 *
*                                                                 *
******************************************************************/
#ifndef _NTINTSAFE_H_INCLUDED_
#define _NTINTSAFE_H_INCLUDED_

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
#define RtlIntToChar(iOperand, pch)        RtlIntToUChar(iOperand, pch)
#else
__inline
NTSTATUS
RtlIntToChar(
    __in int iOperand,
    __out char* pch)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pch = '\0';

    if ((iOperand >= -128) && (iOperand <= 127))
    {
        *pch = (char)iOperand;
        status = STATUS_SUCCESS;
    }

    return status;
}
#endif // _CHAR_UNSIGNED

//
// INT -> UCHAR conversion
//
__inline
NTSTATUS
RtlIntToUChar(
    __in int iOperand,
    __out unsigned char* pch)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pch = '\0';

    if ((iOperand >= 0) && (iOperand <= 255))
    {
        *pch = (unsigned char)iOperand;
        status = STATUS_SUCCESS;
    }

    return status;
}

//
// INT -> BYTE conversion
//
#define RtlIntToByte(uOperand, pResult)    RtlIntToUChar(uOperand, pResult)

//
// UINT -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
#define RtlUIntToChar(uOperand, pch)       RtlUIntToUChar(uOperand, pch)
#else
__inline
NTSTATUS
RtlUIntToChar(
    __in unsigned int uOperand,
    __out char* pch)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pch = '\0';

    if (uOperand <= 127)
    {
        *pch = (char)uOperand;
        status = STATUS_SUCCESS;
    }

    return status;
}
#endif // _CHAR_UNSIGNED

//
// UINT -> UCHAR conversion
//
__inline
NTSTATUS
RtlUIntToUChar(
    __in unsigned int uOperand,
    __out unsigned char* pch)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pch = '\0';

    if (uOperand <= 255)
    {
        *pch = (unsigned char)uOperand;
        status = STATUS_SUCCESS;
    }

    return status;
}

//
// UINT -> BYTE conversion
//
#define RtlUIntToByte(uOperand, pResult)   RtlUIntToUChar(uOperand, pResult)

//
// INT -> UINT conversion
//
__inline
NTSTATUS
RtlIntToUInt(
    __in int iOperand,
    __out unsigned int* puResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *puResult = UINT_ERROR;

    if (iOperand >= 0)
    {
        *puResult = (unsigned int)iOperand;
        status = STATUS_SUCCESS;
    }

    return status;
}

//
// UINT -> INT conversion
//
__inline
NTSTATUS
RtlUIntToInt(
    __in unsigned int uOperand,
    __out int* piResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *piResult = INT_ERROR;

    if (uOperand <= INT_MAX)
    {
        *piResult = (int)uOperand;
        status = STATUS_SUCCESS;
    }

    return status;
}

//
// UINT -> USHORT conversion
//
__inline
NTSTATUS
RtlUIntToUShort(
    __in unsigned int uOperand,
    __out unsigned short* pusResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (HIWORD(uOperand) == 0)
    {
        *pusResult = LOWORD(uOperand);
        status = STATUS_SUCCESS;
    }

    return status;
}

//
// UINT -> WORD conversion
//
#define RtlUIntToWord(uOperand, pwResult)      RtlUIntToUShort(uOperand, pwResult)

//
// ULONGLONG -> ULONG conversion
//
__inline
NTSTATUS
RtlULongLongToULong(
    __in unsigned __int64 ullOperand,
    __out unsigned long* pulResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pulResult = ULONG_ERROR;
    
    if (HIDWORD(ullOperand) == 0)
    {
        *pulResult = LODWORD(ullOperand);
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// ULONGLONG -> UINT conversion
//
__inline
NTSTATUS
RtlULongLongToUInt(
    __in unsigned __int64 ullOperand,
    __out unsigned int* puResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *puResult = UINT_ERROR;
    
    if (HIDWORD(ullOperand) == 0)
    {
        *puResult = LODWORD(ullOperand);
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// USHORT addition
//
__inline
NTSTATUS
RtlUShortAdd(
    __in unsigned short usAugend,
    __in unsigned short usAddend,
    __out unsigned short* pusResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if ((unsigned short)(usAugend + usAddend) >= usAugend)
    {
        *pusResult = usAugend + usAddend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// WORD addtition
//
#define RtlWordAdd(wAugend, wAddend, pwResult)         RtlUShortAdd(wAugend, wAddend, pwResult)

//
// UINT addition
//
__inline
NTSTATUS
RtlUIntAdd(
    __in unsigned int uAugend,
    __in unsigned int uAddend,
    __out unsigned int* puResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *puResult = UINT_ERROR;

    if ((unsigned int)(uAugend + uAddend) >= uAugend)
    {
        *puResult = uAugend + uAddend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// UINT_PTR addition
//
#ifdef _WIN64
#define RtlUIntPtrAdd(uAugend, uAddend, puResult)      RtlULongLongAdd(uAugend, uAddend, puResult)
#else
#define RtlUIntPtrAdd(uAugend, uAddend, puResult)      RtlUIntAdd(uAugend, uAddend, puResult)
#endif // _WIN64

//
// ULONG addition
//
__inline
NTSTATUS
RtlULongAdd(
    __in unsigned long ulAugend,
    __in unsigned long ulAddend,
    __out unsigned long* pulResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pulResult = ULONG_ERROR;

    if ((unsigned long)(ulAugend + ulAddend) >= ulAugend)
    {
        *pulResult = ulAugend + ulAddend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// ULONG_PTR addition
//
#ifdef _WIN64
#define RtlULongPtrAdd(ulAugend, ulAddend, pulResult)  RtlULongLongAdd(ulAugend, ulAddend, pulResult)
#else
#define RtlULongPtrAdd(ulAugend, ulAddend, pulResult)  RtlULongAdd(ulAugend, ulAddend, pulResult)
#endif // _WIN64

//
// DWORD addition
//
#define RtlDWordAdd(dwAugend, dwAddend, pdwResult)     RtlULongAdd(dwAugend, dwAddend, pdwResult)

//
// DWORD_PTR addition
//
#ifdef _WIN64
#define RtlDWordPtrAdd(dwAugend, dwAddend, pdwResult)  RtlULongLongAdd(dwAugend, dwAddend, pdwResult)
#else
#define RtlDWordPtrAdd(dwAugend, dwAddend, pdwResult)  RtlULongAdd(dwAugend, dwAddend, pdwResult)
#endif // _WIN64

//
// size_t addition
//
__inline
NTSTATUS
RtlSizeTAdd(
    __in size_t Augend,
    __in size_t Addend,
    __out size_t* pResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pResult = SIZET_ERROR;

    if ((Augend + Addend) >= Augend)
    {
        *pResult = Augend + Addend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// SIZE_T addition
//
#ifdef _WIN64
#define RtlSIZETAdd(Augend, Addend, pResult)       RtlULongLongAdd(Augend, Addend, pResult)
#else
#define RtlSIZETAdd(Augend, Addend, pResult)       RtlULongAdd(Augend, Addend, pResult)
#endif // _WIN64

//
// ULONGLONG addition
//
__inline
NTSTATUS
RtlULongLongAdd(
    __in unsigned __int64 ullAugend,
    __in unsigned __int64 ullAddend,
    __out unsigned __int64* pullResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pullResult = ULONGLONG_ERROR;

    if ((unsigned __int64)(ullAugend + ullAddend) >= ullAugend)
    {
        *pullResult = ullAugend + ullAddend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// USHORT subtraction
//
__inline
NTSTATUS
RtlUShortSub(
    __in unsigned short usMinuend,
    __in unsigned short usSubtrahend,
    __out unsigned short* pusResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (usMinuend >= usSubtrahend)
    {
        *pusResult = usMinuend - usSubtrahend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// WORD subtraction
//
#define RtlWordSub(wMinuend, wSubtrahend, pwResult)    RtlUShortSub(wMinuend, wSubtrahend, pwResult)


//
// UINT subtraction
//
__inline
NTSTATUS
RtlUIntSub(
    __in unsigned int uMinuend,
    __in unsigned int uSubtrahend,
    __out unsigned int* puResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *puResult = UINT_ERROR;

    if (uMinuend >= uSubtrahend)
    {
        *puResult = uMinuend - uSubtrahend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// UINT_PTR subtraction
//
#ifdef _WIN64
#define RtlUIntPtrSub(uMinuend, uSubtrahend, puResult)     RtlULongLongSub(uMinuend, uSubtrahend, puResult)
#else
#define RtlUIntPtrSub(uMinuend, uSubtrahend, puResult)     RtlUIntSub(uMinuend, uSubtrahend, puResult)
#endif // _WIN64

//
// ULONG subtraction
//
__inline
NTSTATUS
RtlULongSub(
    __in unsigned long ulMinuend,
    __in unsigned long ulSubtrahend,
    __out unsigned long* pulResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pulResult = ULONG_ERROR;

    if (ulMinuend >= ulSubtrahend)
    {
        *pulResult = ulMinuend - ulSubtrahend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// ULONG_PTR subtraction
//
#ifdef _WIN64
#define RtlULongPtrSub(ulMinuend, ulSubtrahend, pulResult)     RtlULongLongSub(ulMinuend, ulSubtrahend, pulResult)
#else
#define RtlULongPtrSub(ulMinuend, ulSubtrahend, pulResult)     RtlULongSub(ulMinuend, ulSubtrahend, pulResult)
#endif // _WIN64


//
// DWORD subtraction
//
#define RtlDWordSub(dwMinuend, dwSubtrahend, pdwResult)        RtlULongSub(dwMinuend, dwSubtrahend, pdwResult)

//
// DWORD_PTR subtraction
//
#ifdef _WIN64
#define RtlDWordPtrSub(dwMinuend, dwSubtrahend, pdwResult)     RtlULongLongSub(dwMinuend, dwSubtrahend, pdwResult)
#else
#define RtlDWordPtrSub(dwMinuend, dwSubtrahend, pdwResult)     RtlULongSub(dwMinuend, dwSubtrahend, pdwResult)
#endif // _WIN64

//
// size_t subtraction
//
__inline
NTSTATUS
RtlSizeTSub(
    __in size_t Minuend,
    __in size_t Subtrahend,
    __out size_t* pResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pResult = SIZET_ERROR;

    if (Minuend >= Subtrahend)
    {
        *pResult = Minuend - Subtrahend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// SIZE_T subtraction
//
#ifdef _WIN64
#define RtlSIZETSub(Minuend, Subtrahend, pResult)       RtlULongLongSub(Minuend, Subtrahend, pResult)
#else
#define RtlSIZETSub(Minuend, Subtrahend, pResult)       RtlULongSub(Minuend, Subtrahend, pResult)
#endif // _WIN64

//
// ULONGLONG subtraction
//
__inline
NTSTATUS
RtlULongLongSub(
    __in unsigned __int64 ullMinuend,
    __in unsigned __int64 ullSubtrahend,
    __out unsigned __int64* pullResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
    *pullResult = ULONGLONG_ERROR;

    if (ullMinuend >= ullSubtrahend)
    {
        *pullResult = ullMinuend - ullSubtrahend;
        status = STATUS_SUCCESS;
    }
    
    return status;
}

//
// USHORT multiplication
//
__inline
NTSTATUS
RtlUShortMult(
    __in unsigned short usMultiplicand,
    __in unsigned short usMultiplier,
    __out unsigned short* pusResult)
{
    unsigned long ulResult = (unsigned long)usMultiplicand * usMultiplier;
    
    return RtlUIntToUShort(ulResult, pusResult);
}

//
// WORD multiplication
//
#define RtlWordMult(wMultiplicand, wMultiplier, pwResult)      RtlUShortMult(wMultiplicand, wMultiplier, pwResult)

//
// UINT multiplication
//
__inline
NTSTATUS
RtlUIntMult(
    __in unsigned int uMultiplicand,
    __in unsigned int uMultiplier,
    __out unsigned int* puResult)
{
    unsigned __int64 ull64Result = UInt32x32To64(uMultiplicand, uMultiplier);

    return RtlULongLongToUInt(ull64Result, puResult);
}

//
// UINT_PTR multiplication
//
#ifdef _WIN64
#define RtlUIntPtrMult(uMultiplicand, uMultiplier, puResult)       RtlULongLongMult(uMultiplicand, uMultiplier, puResult)
#else
#define RtlUIntPtrMult(uMultiplicand, uMultiplier, puResult)       RtlUIntMult(uMultiplicand, uMultiplier, puResult)
#endif // _WIN64

//
// ULONG multiplication
//
__inline
NTSTATUS
RtlULongMult(
    __in unsigned long ulMultiplicand,
    __in unsigned long ulMultiplier,
    __out unsigned long* pulResult)
{
    unsigned __int64 ull64Result = UInt32x32To64(ulMultiplicand, ulMultiplier);
    
    return RtlULongLongToULong(ull64Result, pulResult);
}

//
// ULONG_PTR multiplication
//
#ifdef _WIN64
#define RtlULongPtrMult(ulMultiplicand, ulMultiplier, pulResult)   RtlULongLongMult(ulMultiplicand, ulMultiplier, pulResult)
#else
#define RtlULongPtrMult(ulMultiplicand, ulMultiplier, pulResult)   RtlULongMult(ulMultiplicand, ulMultiplier, pulResult)
#endif // _WIN64


//
// DWORD multiplication
//
#define RtlDWordMult(dwMultiplicand, dwMultiplier, pdwResult)      RtlULongMult(dwMultiplicand, dwMultiplier, pdwResult)

//
// DWORD_PTR multiplication
//
#ifdef _WIN64
#define RtlDWordPtrMult(dwMultiplicand, dwMultiplier, pdwResult)   RtlULongLongMult(dwMultiplicand, dwMultiplier, pdwResult)
#else
#define RtlDWordPtrMult(dwMultiplicand, dwMultiplier, pdwResult)   RtlULongMult(dwMultiplicand, dwMultiplier, pdwResult)
#endif // _WIN64

//
// size_t multiplication
//

#ifdef _WIN64
#define RtlSizeTMult(Multiplicand, Multiplier, pResult)        RtlULongLongMult(Multiplicand, Multiplier, pResult)
#else
#define RtlSizeTMult(Multiplicand, Multiplier, pResult)        RtlUIntMult(Multiplicand, Multiplier, pResult)
#endif // _WIN64

//
// SIZE_T multiplication
//
#ifdef _WIN64
#define RtlSIZETMult(Multiplicand, Multiplier, pResult)        RtlULongLongMult(Multiplicand, Multiplier, pResult)
#else
#define RtlSIZETMult(Multiplicand, Multiplier, pResult)        RtlULongMult(Multiplicand, Multiplier, pResult)
#endif // _WIN64

//
// ULONGLONG multiplication
//
__inline
NTSTATUS
RtlULongLongMult(
    __in unsigned __int64 ullMultiplicand,
    __in unsigned __int64 ullMultiplier,
    __out unsigned __int64* pullResult)
{
    NTSTATUS status = STATUS_INTEGER_OVERFLOW;
#ifdef _AMD64_
    unsigned __int64 u64ResultHigh;
    unsigned __int64 u64ResultLow;
    
    *pullResult = ULONGLONG_ERROR;
    
    u64ResultLow = UnsignedMultiply128(ullMultiplicand, ullMultiplier, &u64ResultHigh);
    if (u64ResultHigh == 0)
    {
        *pullResult = u64ResultLow;
        status = STATUS_SUCCESS;
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
        status = STATUS_SUCCESS;
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
                    if (NT_SUCCESS(RtlULongLongAdd(bc << 32, ad << 32, &ullResult)))                        
#else
                    // << 32 not supported on 32-bit compilers
                    if (NT_SUCCESS(RtlULongLongAdd(bc * 4294967295, ad * 4294967295, &ullResult)))
#endif                                      
                    {
                        // b * d
                        bd = (unsigned __int64)dw_b * (unsigned __int64)dw_d;
                    
                        if (NT_SUCCESS(RtlULongLongAdd(ullResult, bd, &ullResult)))
                        {
                            *pullResult = ullResult;
                            status = STATUS_SUCCESS;
                        }
                    }
                }
            }
        }
    }
#endif // _AMD64_  
    
    return status;
}

#ifdef UNDER_CE
// Extra helper functions specific to CE

__inline
NTSTATUS
CeRtlULongAdd3(
	__in unsigned long arg1,
	__in unsigned long arg2,
	__in unsigned long arg3,
	__out unsigned long* pResult)
{
	if(NT_SUCCESS(RtlULongAdd(arg1,arg2,pResult)) &&
		NT_SUCCESS(RtlULongAdd(*pResult,arg3,pResult))){
		return STATUS_SUCCESS;
	}

	return STATUS_INTEGER_OVERFLOW;
}

__inline
NTSTATUS
CeRtlULongAdd4(
	__in unsigned long arg1,
	__in unsigned long arg2,
	__in unsigned long arg3,
	__in unsigned long arg4,
	__out unsigned long* pResult)
{
	if(NT_SUCCESS(RtlULongAdd(arg1,arg2,pResult)) &&
		NT_SUCCESS(RtlULongAdd(*pResult,arg3,pResult)) &&
		NT_SUCCESS(RtlULongAdd(*pResult,arg4,pResult))){
		return STATUS_SUCCESS;
	}

	return STATUS_INTEGER_OVERFLOW;
}

// (arg1*arg2) + arg3
__inline
NTSTATUS
CeRtlULongMult2Add(
	__in unsigned long arg1,
	__in unsigned long arg2,
	__in unsigned long arg3,
	__out unsigned long* pResult)
{
	if(NT_SUCCESS(RtlULongMult(arg1,arg2,pResult)) &&
		NT_SUCCESS(RtlULongAdd(*pResult,arg3,pResult))){
		return STATUS_SUCCESS;
	}

	return STATUS_INTEGER_OVERFLOW;
}

// (arg1+arg2) * arg3
__inline
NTSTATUS
CeRtlULongAdd2Mult(
	__in unsigned long arg1,
	__in unsigned long arg2,
	__in unsigned long arg3,
	__out unsigned long* pResult)
{
	if(NT_SUCCESS(RtlULongAdd(arg1,arg2,pResult)) &&
		NT_SUCCESS(RtlULongMult(*pResult,arg3,pResult))){
		return STATUS_SUCCESS;
	}

	return STATUS_INTEGER_OVERFLOW;
}

#define CeRtlDWordAdd3(arg1,arg2,arg3,pResult) CeRtlULongAdd3(arg1,arg2,arg3,pResult)

#define CeRtlDWordAdd4(arg1,arg2,arg3,arg4,pResult) CeRtlULongAdd4(arg1,arg2,arg3,arg4,pResult)

#define CeRtlDWordMult2Add(arg1,arg2,arg3,pResult) CeRtlULongMult2Add(arg1,arg2,arg3,pResult)

#define CeRtlDWordAdd2Mult(arg1,arg2,arg3,pResult) CeRtlULongAdd2Mult(arg1,arg2,arg3,pResult)

__inline
NTSTATUS
CeRtlUIntAdd3(
	__in unsigned int arg1,
	__in unsigned int arg2,
	__in unsigned int arg3,
	__out unsigned int* pResult)
{
	if(NT_SUCCESS(RtlUIntAdd(arg1,arg2,pResult)) &&
		NT_SUCCESS(RtlUIntAdd(*pResult,arg3,pResult))){
		return STATUS_SUCCESS;
	}

	return STATUS_INTEGER_OVERFLOW;
}

__inline
NTSTATUS
CeRtlUIntAdd4(
	__in unsigned int arg1,
	__in unsigned int arg2,
	__in unsigned int arg3,
	__in unsigned int arg4,
	__out unsigned int* pResult)
{
	if(NT_SUCCESS(RtlUIntAdd(arg1,arg2,pResult)) &&
		NT_SUCCESS(RtlUIntAdd(*pResult,arg3,pResult)) &&
		NT_SUCCESS(RtlUIntAdd(*pResult,arg4,pResult))){
		return STATUS_SUCCESS;
	}

	return STATUS_INTEGER_OVERFLOW;
}

#endif

#endif // _NTINTSAFE_H_INCLUDED_
