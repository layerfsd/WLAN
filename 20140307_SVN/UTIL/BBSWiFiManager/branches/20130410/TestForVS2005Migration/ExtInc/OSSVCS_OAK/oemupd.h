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
/*---------------------------------------------------------------------------*\
 *
 * Diff Update
 *
 * (c) Copyright Microsoft Corp. 2000 All Rights Reserved
 *
 *  module: oemflash.h
 *  author: scottsh
 *
 *  purpose: Interface to OEM flash functions
 *
 *	history: 7/12/2001  	Scott Shell (ScottSh)		Created
 *
\*---------------------------------------------------------------------------*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef DWORD (*OEMFlashReadFUNC)(LPVOID pvDestAddr, LPVOID pvSrcAddr, DWORD dwLength);
typedef DWORD (*OEMFlashWriteFUNC)(LPVOID pvDestAddr, LPVOID pvSrcAddr, DWORD dwLength);
typedef DWORD (*OEMFlashEraseFUNC)(LPVOID pvAddr, DWORD dwLength);
typedef void  (*OEMREBOOTFUNC)();
typedef BOOL  (*OEMDISPLAYBLTFUNC)(DWORD dwTop, DWORD dwLeft, DWORD dwBottom, DWORD dwRight, DWORD dwStrideWidth, LPBYTE pbData);

// These global function pointers must be set by the OEM prior to calling diff update.
extern OEMFlashReadFUNC  g_pfnFlashRead;
extern OEMFlashWriteFUNC g_pfnFlashWrite;
extern OEMFlashEraseFUNC g_pfnFlashErase;
extern OEMREBOOTFUNC     g_pfnReboot;
extern OEMDISPLAYBLTFUNC g_pfnDisplayBlt;

// To be filled in by OEM FIXUPVARs
extern DWORD       dwFlashBlkSize;
extern DWORD       dwFlashBlkBits;










#ifdef __cplusplus
};
#endif
