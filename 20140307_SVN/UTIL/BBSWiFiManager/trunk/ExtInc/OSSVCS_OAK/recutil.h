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
// Copyright (c) 1999-2000, Microsoft Corp. All rights reserved.
#ifndef _RECUTIL_H_
#define _RECUTIL_H_

const DWORD CPV_IGNORECASE = 0x00000001; // ignore case on prop val comparisons

HRESULT FindProp(const CEPROPVAL* rgv, UINT cv, CEPROPID id, const CEPROPVAL** ppv);
HRESULT PropValToString(TCHAR* pszBuff, int cchBuff, const CEPROPVAL* ppv);
BOOL StringToPropVal(CEPROPVAL *pVal, const TCHAR *pszNum);
UINT GetPropMemSize(const CEPROPVAL* pVal);
CEPROPVAL *SafeLocalReAlloc(CEPROPVAL *pVals, UINT cVals, UINT uBytes, UINT uFlag);
HRESULT ModifyPropValString(CEPROPVAL** ppValues, UINT* pcValues, CEPROPID idProp, const TCHAR* pszNewVal);
HRESULT ModifyPropValPrimitiveType(CEPROPVAL** ppValues, UINT* pcValues, CEPROPID idProp, const CEVALUNION* pvalu);
HRESULT ModifyPropVal(CEPROPVAL** ppValues, UINT* pcValues, const CEPROPVAL* pNewVal);
ULONG SafeULONG(const CEPROPVAL* pVal);
const TCHAR* SafeLPWSTR(const CEPROPVAL* pVal);
int ComparePropValUnions(CEPROPID id, DWORD dwFlags, const CEVALUNION* pValUnionA, const CEVALUNION* pValUnionB);
int ComparePropVals(DWORD dwFlags, const CEPROPVAL* pValA, const CEPROPVAL* pValB);
HRESULT CopyPropValue(const CEPROPVAL* pValSource, CEPROPVAL* pValDest, CEPROPID idPropDest, void** ppvNextBlock);
HRESULT DuplicateCEPROPVALArray(__in_ecount(cValues) const CEPROPVAL* pValuesSource, UINT cValues, UINT cAppendEmpty, __deref_out CEPROPVAL** ppValsOut);
HRESULT DuplicateCEPROPVALArrayNoAlloc(__in_ecount(cValues) const CEPROPVAL* pValuesSource, UINT cValues, UINT cAppendEmpty, __out_bcount(cbValuesDest) CEPROPVAL* pValuesDest, UINT cbValuesDest);
HRESULT SparsePvalsToRealPvals(UINT cSparseProps, const CEPROPVAL *pSparseVals, UINT *pcProps, CEPROPVAL **ppRealPvals);

const DWORD tpafPointToOldValues = 0x00000001;
const DWORD tpafAllocNewValues = 0x00000002;
HRESULT TranslatePropValArray(const CEPROPVAL* pValsSource, UINT cVals, const CEPROPID* rgids, UINT cids, CEPROPVAL** ppValsOut, DWORD dwFlags);

HRESULT CreateEmptyRecord(const CEPROPID* rgids, UINT cids, UINT cbExtra, CEPROPVAL **ppVals, BYTE** ppbExtra);

BOOL DoPropsContainData(const CEPROPVAL* pValsSource, UINT cVals);

// Is FileTime zero?
#define IS_FILETIME_ZERO(ft) (!((ft).dwLowDateTime) && !((ft).dwHighDateTime))

__inline BOOL IsLPWSTREmpty(const CEPROPVAL *pVal)
{
    ASSERT (TypeFromPropID(pVal->propid) == CEVT_LPWSTR);
    // NULL strings are possible for calculated properties
    return ((pVal->wFlags & CEDB_PROPNOTFOUND) || 
            (pVal->val.lpwstr == NULL) || 
            (pVal->val.lpwstr[0] == '\0'));
}

#ifdef DEBUG
void Dbg_VerifyCEPROPVALArray(const CEPROPVAL* pVals, UINT cVals);
#else
#define Dbg_VerifyCEPROPVALArray(pVals, cVals)  /*NOTHING*/
#endif

#endif // _RECUTIL_H_
