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
#ifndef _COMUTIL_H_
#define _COMUTIL_H_
// Copyright (c) 1994-1999, Microsoft Corp. All rights reserved.
//***   comutil.h -- COM helper utils
//

#ifdef __cplusplus
extern "C" {
#endif

#include "qistub.h"

// dup'ed from cclsfact.h.  most people (Xxx_Create implementors) only
// need this bare minimum.  only the class factory clients (1 per DLL)
// need the full cclsfact.h
#ifdef __cplusplus
class CClassFactory;
typedef CClassFactory * POBJECTINFO;
#endif

#define SHStringFromGUIDW   StringFromGUID2
STDAPI_(BOOL) SHGUIDFromStringW(LPCWSTR psz, GUID *pguid);

#ifdef UNICODE
#define SHStringFromGUID    SHStringFromGUIDW
#define SHGUIDFromString    SHGUIDFromStringW
#else
#define SHStringFromGUID    *** error ***       /* NYI */
#define SHGUIDFromString    *** error ***       /* NYI */
#endif

STDAPI_(void) IUnknown_Set(IUnknown **ppunkDst, IUnknown *punkSrc);
STDAPI IUnknown_SetOwner(IUnknown *punk, IUnknown *punkOwner);
STDAPI IUnknown_GetObjectOfPDP(IUnknown *punk, CEOID oid, CEPROPID hhpr, REFIID riid, void **ppv);
STDAPI IUnknown_GetDIO(IUnknown *punk, CEOID oid, void *pv1, void *pv2, BYTE **ppBuf, DWORD *pcbBuf);

// Copied from OLE source code
// format for string form of GUID is:
// ????{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}
#define GUIDSTR_MAX (1+ 8 + 1 + 4 + 1 + 4 + 1 + 4 + 1 + 12 + 1 + 1)

#ifdef DEBUG
#define DBG_MENU_CITEMMAX   100     // (approx) max # of items in a single menu

void Dbg_DumpGUID(LPTSTR pszFmt, REFGUID rguid);
#endif

#ifdef __cplusplus
}
#endif

#endif // _COMUTIL_H_
