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
#ifndef _CCLSFACT_H_
#define _CCLSFACT_H_
//***   cclsfact.h -- IClassFactory default implementation/ helper utils
//

//***   IClassFactory stuff {

// seems like we ought to be able to use 'real' inheritance in the svr
// (which is desirable since then we can call AddRef ourselves).
//
// however on the client side (cfobj.cpp) we end up w/ a thunk in our
// vtbl, which causes us to recurse(?) infinitely when CCF_GCOSearch
// v-calls AddRef.
#define CCF_EXPT    0       // must be 0! 1 doesn't work yet...

//***   CIfaceCF
#if CCF_EXPT
#define STDMETHODX      STDMETHOD
#define STDMETHODX_     STDMETHOD_
#else
#define STDMETHODX(fn)      HRESULT STDMETHODCALLTYPE fn
#define STDMETHODX_(ret,fn) ret STDMETHODCALLTYPE fn
#endif

#if 0
class CIfaceCF /* : public 'fake' IClassFactory */
{
    // n.b. must have no explicit virtuals (o.w. struct layout bogus)!!!
#define virtual     *** error ***

    // *** IUnknown methods (n.b. 'fake' virtuals) ***
    STDMETHODX (QueryInterface)(REFIID, void **);
    STDMETHODX_(ULONG, AddRef)();
    STDMETHODX_(ULONG, Release)();

    // *** THISCLASS methods (n.b. 'fake' virtuals) ***
    STDMETHODX (CreateInstance)(IUnknown *, REFIID, void **);
    STDMETHODX (LockServer)(BOOL);

protected:
    IClassFactory * _vptr;      // vptr for our 'fake' virtuals

#undef  virtual
};
#endif

//***   CClassFactory, _ClassFactory --
// NOTES
//  POBJECTINFO is the class (the thing we usually party on)
//  OBJECTINFO is magic struct (explicitly statically init'ed, no ctor)
class CClassFactory;
typedef CClassFactory * POBJECTINFO;

typedef HRESULT (*PFNCREATEINSTANCE)(IUnknown *punkOuter, IUnknown **ppunk, POBJECTINFO poi);

extern "C" HRESULT CCF_GCOSearch(POBJECTINFO pcft, REFCLSID rclsid, REFIID riid, void **ppv);

class CClassFactory
#if CCF_EXPT
    : public IClassFactory  // nuke ICF?
#endif
{
public:
    // *** IUnknown methods (n.b. 'fake' virtuals) ***
    STDMETHODX (QueryInterface)(REFIID, void **);
    STDMETHODX_(ULONG, AddRef)();
    STDMETHODX_(ULONG, Release)();

    // *** THISCLASS methods (n.b. 'fake' virtuals) ***
    STDMETHODX (CreateInstance)(IUnknown *, REFIID, void **);
    STDMETHODX (LockServer)(BOOL);

    friend HRESULT CCF_GCOSearch(POBJECTINFO pcft, REFCLSID rclsid, REFIID riid, void **ppv);

public: // public so that Xxx_Create can see _pclsid if need be
#if !CCF_EXPT
    // *must* be 1st
    IClassFactory * _vptr;      // vptr for our 'fake' virtuals
#endif

    CLSID const *   _pclsid;
    PFNCREATEINSTANCE   _pfnCreateInstance;
};

typedef struct tagOBJECTINFO
{
    void *      _vptr;  // IClassFactory vptr

    // CClassFactory
    CLSID const *       _pclsid;
    PFNCREATEINSTANCE   _pfnCreateInstance;
} OBJECTINFO;

//***   typeof(ICF::vtbl)
struct IfaceCFVtbl
{
    // *** IUnknown methods ***
    HRESULT (STDMETHODCALLTYPE CClassFactory::* QueryInterface)(
        REFIID riid, LPVOID FAR* ppvObj);
    ULONG (STDMETHODCALLTYPE CClassFactory::* AddRef)();
    ULONG (STDMETHODCALLTYPE CClassFactory::* Release)();

    // *** THISCLASS methods ***
    HRESULT (STDMETHODCALLTYPE CClassFactory::* CreateInstance)(
        LPUNKNOWN pUnkOuter, REFIID riid, LPVOID FAR* ppvObject);
    HRESULT (STDMETHODCALLTYPE CClassFactory::* LockServer)(BOOL);
};

#define CCF_GetIClassFactory(that)  ((IClassFactory *)(that))

typedef HRESULT (STDMETHODCALLTYPE CClassFactory::* pfnQI)(
    REFIID riid, LPVOID FAR* ppvObj);
typedef ULONG (STDMETHODCALLTYPE CClassFactory::* pfnAR)();
typedef ULONG (STDMETHODCALLTYPE CClassFactory::* pfnR)();
typedef HRESULT (STDMETHODCALLTYPE CClassFactory::* pfnCI)(
        LPUNKNOWN pUnkOuter, REFIID riid, LPVOID FAR* ppvObject);
typedef HRESULT (STDMETHODCALLTYPE CClassFactory::* pfnLS)(BOOL);

#define CF_Vtbl(clsname) \
    const IfaceCFVtbl c_CFVtbl = { \
        (pfnQI)clsname::QueryInterface, \
        (pfnAR)clsname::AddRef, (pfnR)clsname::Release, \
        (pfnCI)clsname::CreateInstance, (pfnLS)clsname::LockServer, \
    }

#define CF_TABBEGIN         const OBJECTINFO g_cftThisDll[] = {
#define CF_TABENT(clsname)  { (void *)&c_CFVtbl, &CLSID_##clsname, C##clsname##_Create }
#define CF_TABEND           {0}, };

extern "C" STDMETHODIMP CCF_QueryInterface(CClassFactory *that, REFIID riid, void **ppvObj);
extern "C" STDMETHODIMP CCF_CreateInstance(CClassFactory *that, IUnknown *punkOuter, REFIID riid, void **ppv);

// }

#endif // _CCLSFACT_H_
