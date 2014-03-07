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
#ifndef _CDATAOBJ_H_
#define _CDATAOBJ_H_
//***   cdataobj.h -- default implementations for various COM objects
//

#ifndef DLLIMPORT // server (re)defines this, clients do not
#define DLLIMPORT   __declspec(dllimport)
#endif

class DLLIMPORT CDataObject :
    public IDataObject
{
public:
    // *** IUnknown ***
    //virtual STDMETHODIMP QueryInterface(REFIID riid, LPVOID * ppvObj);
    //virtual STDMETHODIMP_(ULONG) AddRef(void);
    //virtual STDMETHODIMP_(ULONG) Release(void);
    
    // *** IDataObject methods ***
    //virtual STDMETHODIMP GetData(FORMATETC *pfmtetcIn, STGMEDIUM *pstgmed);
    STDMETHODIMP GetDataHere(FORMATETC *pfmtetc, STGMEDIUM *pstgpmed);
    STDMETHODIMP QueryGetData(FORMATETC *pfmtetc);
    STDMETHODIMP GetCanonicalFormatEtc(FORMATETC *pfmtetcIn, FORMATETC *pfmtetcOut);
    STDMETHODIMP SetData(FORMATETC *pfmtetc, STGMEDIUM *pstgmed, BOOL bRelease);
    STDMETHODIMP EnumFormatEtc(DWORD dwDir, IEnumFORMATETC **ppiEfe);
    STDMETHODIMP DAdvise(FORMATETC *pfmtetc, DWORD dwAdviseFlags, IAdviseSink * piadvsink, PDWORD pdwConnection);
    STDMETHODIMP DUnadvise(DWORD dwConnection);
    STDMETHODIMP EnumDAdvise(IEnumSTATDATA **ppienumStatData);

    // *** THISCLASS methods ***
    STDMETHODIMP InitializeDO(const FORMATETC afmt[], UINT cfmt);
    STDMETHODIMP_(int) FindFE(FORMATETC *pfe);

    CDataObject();
    virtual ~CDataObject();

    // we might need these in client (DLLEXPORT would generate them)
    CDataObject(CDataObject const & that);
    CDataObject& operator=(CDataObject const & that);

protected:

private:
    // n.b. no _cRef, we're a virtual class
    int                 _cfmt;
    const FORMATETC *   _afmt;      // FORMATETC _afmt[]
};

STDAPI StgMedium_PutBytes(FORMATETC *pfe, STGMEDIUM *pmed, BYTE *pbIn, int cbIn);
STDAPI StgMedium_GetBytes(FORMATETC *pfe, STGMEDIUM *pmed, BYTE *pbData, int cbData);
STDAPI StgMedium_PutTSTR(FORMATETC *pfe, STGMEDIUM *pmed, TCHAR *pszIn);
STDAPI StgMedium_GetTSTR(FORMATETC *pfe, STGMEDIUM *pmed, TCHAR *psz, int cchMax);
STDAPI StgMedium_GetOID(FORMATETC *pfe, STGMEDIUM *pmed, CEOID *pOid);
STDAPI StgMedium_PutOID(FORMATETC *pfe, STGMEDIUM *pmed, CEOID oid);
STDAPI StgMedium_PutPROPID(FORMATETC *pfe, STGMEDIUM *pmed, CEPROPID propid);
#if 0
STDAPI StgMedium_PutDWORD(FORMATETC *pfe, STGMEDIUM *pmed, DWORD dw);
STDAPI StgMedium_GetDWORD(FORMATETC *pfe, STGMEDIUM *pmed, DWORD *pdw);
#endif

STDAPI CDataObject_GetDataTSTR(IDataObject *pdo, LPTSTR psz, int cchMax);
STDAPI CDataObject_GetDataOID(IDataObject *pdo, CEOID *poid);
STDAPI CDataObject_GetDataPROPID(IDataObject *pdo, CEPROPID *ppropid);
STDAPI CDataObject_GetDataPROPID(IDataObject *pdo, CEPROPID *ppropid);

#undef  DLLIMPORT       // make sure somebody else doesn't get by accident

#endif // _CDATAOBJ_H_
