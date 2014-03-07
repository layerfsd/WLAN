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
#ifndef _ALGUTIL_H_
#define _ALGUTIL_H_
//***   algutil.h -- algorithms/ data structures
//

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _DSA *HDSA;      // in case no commctrl.h 
int DSASearchKeyStr(HDSA hdsaNV, int iFirst, LPTSTR pszName);

typedef struct _SSA {
    void *  _aItem;     // memory for elts
    int     _cbItem;    // size of each item
} SSA;
void * SSASearchKeyPtr(const SSA *psa, int iFirst, void *pbKey, int cbKey);

int SearchMapInt(const int *src, const int *dst, int cnt, int val);

#define PERCENT(i, pct)     ((i) * (pct) / 100)

#ifdef __cplusplus
}
#endif

#endif // _ALGUTIL_H_
