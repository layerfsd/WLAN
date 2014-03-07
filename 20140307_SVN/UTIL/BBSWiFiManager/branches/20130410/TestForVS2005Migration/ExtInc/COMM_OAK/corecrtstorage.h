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
#ifndef __CORE_CRT_STORAGE_H_INCLUDED__
#define __CORE_CRT_STORAGE_H_INCLUDED__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CVTBUFSIZE (309+40) /* # of digits in max. dp value + extra */

#define CRTF_IMM_DISABLED   0x0001
#define CRTF_IMM_ENABLED    0x0002

typedef struct {
    PVOID   CurrentExcept;
    PVOID   ExceptContext;
    PVOID   TargetEntry;
    PVOID   CatchExceptRN;
    DWORD   ExceptState;
#if !defined(_M_IX86)
    PVOID   FrameInfoChain;
#endif
#if defined(_M_ARM)
    PVOID   CatchChain;
#endif
    PVOID   CurrExceptRethrow;
    int     _ProcessingThrow; /* for uncaught_exception */
} crtEHData;

typedef struct crtGlobTag_t {
    DWORD     dwFlags;
    char      *pchfpcvtbuf;
    long      rand;
    char      *nexttoken;
    DWORD     dwHimcDefault;
    DWORD     dwHwndDefaultIme;
    crtEHData EHData;
    wchar_t   *wnexttoken;
} crtGlob_t;

void InitCRTStorage(crtGlob_t* pcrtg);
void ClearCRTStorage(void);
crtGlob_t*  GetCRTStorage(void);
crtGlob_t*  GetCRTStorageEx(BOOL bCreateIfNecessary);

DWORD       GetCRTFlags(void);
void        SetCRTFlag(DWORD dwFlag);
void        ClearCRTFlag(DWORD dwFlag);

#ifdef __cplusplus
}
#endif

#endif

