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
#ifndef _CUNKNOWN_H_
#define _CUNKNOWN_H_
//***   cunknown.h -- IUnknown
// DESCRIPTION
//  default implementation of _cRef, ctor/dtor and (optional) AddRef/Release.
// w/o CUNK_NOMETHODS, AddRef/Release are defined (though client typically
// needs to do a fwder to resolve ambiguities); w/ CUNK_NOMETHODS, client
// must implement them (or call CUNK_ADDREF/RELEASE) itself.
//  there are two forms: CUnknownPrivate (no CoCreate), and CUnknown (CoCreate)
// NOTES
//  this is a virtual class

#define CUNK_ADDREF() \
    if (1) { \
        _cRef++; \
        return _cRef; \
    } \
    else \
        return -1

#define CUNK_RELEASE() \
    if (1) { \
        _cRef--; \
        if (_cRef > 0) \
            return _cRef; \
        delete this;        /* need this to call *virtual* dtor */ \
        return 0; \
    } \
    else \
        return -1

#ifndef DLL_ADDREF // just in case somebody needs to change the name
#define DLL_ADDREF()    DllAddRef()
#define DLL_RELEASE()   DllRelease()
#endif
STDAPI_(void) DLL_ADDREF();
STDAPI_(void) DLL_RELEASE();

//***   CUnknownPrivate, CUnknownPublic --
// DESCRIPTION
//  CUnknownPublic does DllAddRef/etc.,  CUnknownPrivate does not.
// _CUnknownBase is used so client can't cast btwn CUnknownPrivate/Public.
// doing so would confuse refcounts since _DllAddRef/etc. aren't virtual.
class _CUnknownBase :
    public IUnknown
{
public:
#ifndef CUNK_NOMETHODS // {
    // *** IUnknown methods ***
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID * ppvObj)
        { 
            UNREFERENCED_PARAMETER(riid);
            UNREFERENCED_PARAMETER(ppvObj);
            return E_NOTIMPL;
        };
    STDMETHODIMP_(ULONG) AddRef()
        {
            _cRef++;
            return _cRef;
        };
    STDMETHODIMP_(ULONG) Release()
        {
            _cRef--;
            if (_cRef > 0)
                return _cRef;
            delete this;        // n.b. need this to call *virtual* dtor
            // DLL_RELEASE will happen in derived class
            return 0;
        };
#endif // }

protected:
    virtual ~_CUnknownBase() { ; };     // n.b. must be virtual!
    _CUnknownBase()
        {
            _cRef = 1;
        };

    ULONG   _cRef;
};

class CUnknownPrivate :
    public _CUnknownBase
{
    /*NOTHING*/
};


class CUnknownPublic :
    public _CUnknownBase
{
public:

protected:
    CUnknownPublic()
        {
            ::DLL_ADDREF();     // must be *client* version (not ours)
        };
    /*virtual*/ ~CUnknownPublic()
        {
            ::DLL_RELEASE();    // must be *client* version (not ours)
        };

};

//***   CCF_CREATEOBJECT -- CFoo_Create implementation for class factory
// EXAMPLE
#if 0 // { sample client
HRESULT CFoo_Create(punkOuter, ppunk, poi)
{
    CCF_CREATEOBJECT(CFoo, IPreferredIface, punkOuter, ppunk, poi);
    /*NOTREACHED*/
}
#endif // }

#define CCF_CREATEOBJECT(clsName, ifaceName, punkOuter, ppunk, poi) \
    HRESULT hr = E_OUTOFMEMORY; \
    clsName *pObj; \
 \
    /* CCI will handle failure case */ \
    /* *ppunk = NULL; */ \
    pObj = new clsName; \
    if (pObj) { \
        ASSERT(pObj->_cRef == 1); \
        *ppunk = SAFECAST(pObj, ifaceName *); \
        hr = S_OK; \
    } \
    return hr;

#endif // _CUNKNOWN_H_
