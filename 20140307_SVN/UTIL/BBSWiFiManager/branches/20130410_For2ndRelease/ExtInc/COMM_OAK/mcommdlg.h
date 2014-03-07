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

mcommdlg.h

Abstract:  

Notes: 


--*/
#ifndef _INC_MCOMMDLG
#define _INC_MCOMMDLG

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINCEMACRO
#error WINCEMACRO not defined __FILE__
#endif

// exported for commdlg (prototyped in commdlg.h)
#define GetOpenFileNameW IMPLICIT_DECL(BOOL, SH_SHELL, 3, \
		(LPOPENFILENAMEW))
#define GetSaveFileNameW IMPLICIT_DECL(BOOL, SH_SHELL, 4, \
		(LPOPENFILENAMEW))

#ifdef __cplusplus
}
#endif

#endif  /* _INC_MCOMMDLG */
