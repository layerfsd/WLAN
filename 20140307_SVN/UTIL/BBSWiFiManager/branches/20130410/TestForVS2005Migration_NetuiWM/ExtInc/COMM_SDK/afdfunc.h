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

afdfunc.h

Abstract:

Functions exposed from the AFD system process

Notes: 


--*/

// @CESYSGEN IF CE_MODULES_AFD

#ifndef _AFDFUNC_H_
#define _AFDFUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SOCKHAND_DEFINED
DECLARE_HANDLE(SOCKHAND);
typedef SOCKHAND *PSOCKHAND;
#define SOCKHAND_DEFINED
#endif

#include "ras.h"
#include "tapi.h"
			    
#ifdef __cplusplus
}
#endif

#ifdef WINCEOEM
#if defined(WINCEMACRO) || defined(WINCEAFDMACRO)
#include <mafdfunc.h>	// macros
#endif
#endif

#endif  // _AFDFUNC_H_

// @CESYSGEN ENDIF
