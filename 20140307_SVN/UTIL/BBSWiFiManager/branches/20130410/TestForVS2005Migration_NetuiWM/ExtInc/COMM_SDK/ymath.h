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
/* ymath.h internal header */
#ifndef _YMATH
#define _YMATH
#include <math.h>
#ifndef _YVALS
#include <yvals.h>
#endif
		/* MACROS */
#define _FINITE		-1
#define _INFCODE	1
#define _NANCODE	2
		/* TYPE DEFINITIONS */
typedef union {
	unsigned short _W[5];
	float _F;
	double _D;
	long double _L;
	} _Dconst;
_C_LIB_DECL
		/* double DECLARATIONS */
_CRTIMP double __cdecl _Cosh(double, double);
_CRTIMP short __cdecl _Dtest(double *);
_CRTIMP short __cdecl _Exp(double *, double, short);
_CRTIMP double __cdecl _Sinh(double, double);
extern _CRTIMP const _Dconst _Denorm, _Hugeval, _Inf, _Nan, _Snan;
		/* float DECLARATIONS */
_CRTIMP float __cdecl _FCosh(float, float);
_CRTIMP short __cdecl _FDtest(float *);
_CRTIMP short __cdecl _FExp(float *, float, short);
_CRTIMP float __cdecl _FSinh(float, float);
extern _CRTIMP const _Dconst _FDenorm, _FInf, _FNan, _FSnan;
		/* long double DECLARATIONS */
_CRTIMP long double __cdecl _LCosh(long double, long double);
_CRTIMP short __cdecl _LDtest(long double *);
_CRTIMP short __cdecl _LExp(long double *, long double, short);
_CRTIMP long double __cdecl _LSinh(long double, long double);
extern _CRTIMP const _Dconst _LDenorm, _LInf, _LNan, _LSnan;
_END_C_LIB_DECL
#endif /* _YMATH */

/*
 * Copyright (c) 1995 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
