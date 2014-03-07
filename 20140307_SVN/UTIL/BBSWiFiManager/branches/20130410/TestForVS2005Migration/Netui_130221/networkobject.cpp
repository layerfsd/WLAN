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

#include <networkobject.h>

//
// CNetworkObject - Passes the network name for menu extensions
//
CNetworkObject::CNetworkObject() 
{
    m_nRef = 1;
    m_hglobal = NULL;
    m_cf = 0;
}

CNetworkObject::~CNetworkObject()
{
    LocalFree(m_hglobal);
    m_hglobal = NULL;
}

STDMETHODIMP_(ULONG) CNetworkObject::AddRef()
{
    ULONG ulRef = (ULONG)InterlockedIncrement(&m_nRef);
    return ulRef;
}

STDMETHODIMP_(ULONG) CNetworkObject::Release()
{
    int nLocal = InterlockedDecrement(&m_nRef);

    if (!nLocal)
    {
        delete this;
    }

    return nLocal;
}
    
STDMETHODIMP CNetworkObject::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
    if (ppv == NULL)
    {
        return E_INVALIDARG;
    }
    
    *ppv = NULL;
    
    if ((riid == __uuidof(IUnknown)) ||
        (riid == __uuidof(IDataObject)))
    {
        *ppv = this;
        AddRef();
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}

STDMETHODIMP CNetworkObject::GetData(FORMATETC *pFormatetc, STGMEDIUM *pStgmedium)
{
    HRESULT hr;

    CHR(QueryGetData(pFormatetc));

    pStgmedium->tymed = TYMED_HGLOBAL;

    pStgmedium->hGlobal = (HGLOBAL) LocalAlloc(LMEM_FIXED, LocalSize(m_hglobal));
    CPR(pStgmedium->hGlobal);

    memcpy(pStgmedium->hGlobal, m_hglobal, LocalSize(m_hglobal));

Error:
    return hr;
}

STDMETHODIMP CNetworkObject::GetDataHere(FORMATETC *petc, STGMEDIUM *pStgmedium)
{
    UNREFERENCED_PARAMETER(petc);
    UNREFERENCED_PARAMETER(pStgmedium);
    return E_NOTIMPL;
}

STDMETHODIMP CNetworkObject::QueryGetData(FORMATETC *petc)
{
    HRESULT hr = S_OK;
    if (petc == NULL)
    {
        CHR(E_INVALIDARG);
    }

    if(petc->cfFormat != m_cf ||
       petc->ptd != NULL) {
        CHR(DV_E_FORMATETC);
    }

    if(petc->dwAspect != DVASPECT_CONTENT)
    {
        CHR(DV_E_DVASPECT);
    }

    if(petc->lindex != -1)
    {
        CHR(DV_E_LINDEX);
    }

    if(petc->tymed != TYMED_HGLOBAL)
    {
        CHR(DV_E_TYMED);
    }

Error:
    return hr;
}

STDMETHODIMP CNetworkObject::GetCanonicalFormatEtc(FORMATETC *pFormatetcIn, FORMATETC *pFormatetcOut)
{
    UNREFERENCED_PARAMETER(pFormatetcIn);
    UNREFERENCED_PARAMETER(pFormatetcOut);
    return E_NOTIMPL;
}

STDMETHODIMP CNetworkObject::SetData(FORMATETC *pFormatetc, STGMEDIUM *pStgmedium, BOOL fRelease)
{
    UNREFERENCED_PARAMETER(pFormatetc);
    UNREFERENCED_PARAMETER(pStgmedium);
    UNREFERENCED_PARAMETER(fRelease);
    return E_NOTIMPL;
}

STDMETHODIMP CNetworkObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFormatetc)
{
    UNREFERENCED_PARAMETER(dwDirection);
    UNREFERENCED_PARAMETER(ppenumFormatetc);
    return E_NOTIMPL;
}

STDMETHODIMP CNetworkObject::DAdvise(FORMATETC * pFormatetc, DWORD advf, IAdviseSink * pAdvSink, DWORD * pdwConnection)
{
    UNREFERENCED_PARAMETER(pFormatetc);
    UNREFERENCED_PARAMETER(advf);
    UNREFERENCED_PARAMETER(pAdvSink);
    UNREFERENCED_PARAMETER(pdwConnection);
    return E_NOTIMPL;
}

STDMETHODIMP CNetworkObject::DUnadvise(DWORD dwConnection)
{
    UNREFERENCED_PARAMETER(dwConnection);
    return E_NOTIMPL;
}

STDMETHODIMP CNetworkObject::EnumDAdvise(IEnumSTATDATA ** ppenumAdvise)
{
    UNREFERENCED_PARAMETER(ppenumAdvise);
    return E_NOTIMPL;
}

STDMETHODIMP CNetworkObject::SetData(HGLOBAL hglobal, CLIPFORMAT cf)
{
    if (m_hglobal)
    {
        LocalFree(m_hglobal);
    }

    m_hglobal = hglobal;
    m_cf = cf;
    return S_OK;
}

