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

#ifndef _NETWORK_OBJECT_H_
#define _NETWORK_OBJECT_H_

#include <windows.h>
#include <ehm.h>
#include <guts.h>


#ifdef __cplusplus

//
// CNetworkObject - Passes the network name for menu extensions
//
class CNetworkObject : public IDataObject
{
public:
    CNetworkObject();
    ~CNetworkObject();

    // IUnknown methods
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();
    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);

    // IDataObject methods
    STDMETHOD(GetData)(FORMATETC *pFormatetc, STGMEDIUM *pStgmedium);
    STDMETHOD(GetDataHere)(FORMATETC *petc, STGMEDIUM *pStgmedium);
    STDMETHOD(QueryGetData)(FORMATETC *petc);
    STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *pFormatetcIn, FORMATETC *pFormatetcOut);
    STDMETHOD(SetData)(FORMATETC *pFormatetc, STGMEDIUM *pStgmedium, BOOL fRelease);
    STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC **ppenumFormatetc);
    STDMETHOD(DAdvise)(FORMATETC * pFormatetc, DWORD advf, IAdviseSink * pAdvSink, DWORD * pdwConnection);
    STDMETHOD(DUnadvise)(DWORD dwConnection);
    STDMETHOD(EnumDAdvise)(IEnumSTATDATA ** ppenumAdvise);

    // Internal Method
    STDMETHOD(SetData)(HGLOBAL hglobal, CLIPFORMAT cf);

private:

    LONG        m_nRef;
    HGLOBAL     m_hglobal;
    CLIPFORMAT  m_cf;
};

#endif
#endif
