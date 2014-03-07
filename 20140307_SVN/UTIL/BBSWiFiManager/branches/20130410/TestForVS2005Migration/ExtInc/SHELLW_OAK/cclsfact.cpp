// Copyright (c) 1994-1999, Microsoft Corp. All rights reserved.
//***   cfobj.cpp -- class factory for all objects in this DLL
// DESCRIPTION
//  this file has the shared-source 'master' implementation.  it is
// #included in each DLL that uses it (see 'USAGE', below).

// USAGE
#if 0 // {

// client.def
    DllGetClassObject   PRIVATE ; n.b. COM docs say *must* be private
    DllCanUnloadNow     PRIVATE ; n.b. COM docs say *must* be private

// client.cpp
#include "cclsfact.h"

#include "cfcli.cpp"

// your CCI'able classes go here
CF_TABBEGIN // {
    // your classes here...
    CF_TABENT(Foo),         // CLSID_Foo, CFoo_Create
    CF_TABENT(Bar),         // CLSID_Bar, CBar_Create
    ...
CF_TABEND // }

#endif // }

#include <objbase.h>
#include "cshutil.h"
#include "comutil2.h"   // CClassFactory

//***   client stuff {
//

LONG g_cRefThisDll /* = 0 */;

class CThisDllCF : public CClassFactory
{
public:
    // *** IUnknown ***
    STDMETHODIMP QueryInterface(REFIID riid, void ** ppvObj);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // *** THISCLASS ***
    STDMETHODIMP CreateInstance(IUnknown *, REFIID, void **);
    STDMETHODIMP LockServer(BOOL);
};

CF_Vtbl(CThisDllCF);    // c_CFVtbl, needed by CF_TABENT

//***   g_cftThisDll -- class factory table
// NOTES
//  perf: table should be ordered in most-to-least used order
// g_cftThisDll will be provided by client, using CF_TABENT etc.
extern const OBJECTINFO g_cftThisDll[];


//***   DllAddRef/Release, DllCanUnloadNow, DllGetClassObject {
//

STDAPI_(void) DllAddRef(void)
{
    InterlockedIncrement(&g_cRefThisDll);
    ASSERT(g_cRefThisDll < 1000);   // reasonable upper limit
}

STDAPI_(void) DllRelease(void)
{
    InterlockedDecrement(&g_cRefThisDll);

    ASSERT(g_cRefThisDll >= 0);      // don't underflow
}

HRESULT /*dllexport*/ DllCanUnloadNow(void)
{
    if (g_cRefThisDll)
        return S_FALSE;

    DEBUGMSG(ZONE_COM2, (TEXT("DllCanUnloadNow returning S_OK (bye, bye...)")));
    return S_OK;
}

HRESULT /*dllexport*/ DllGetClassObject(REFCLSID rclsid, REFIID riid, void ** ppvCF)
{
    HRESULT hr;

    hr = CCF_GCOSearch((POBJECTINFO)g_cftThisDll, rclsid, riid, ppvCF);
#if !CCF_EXPT
    if (SUCCEEDED(hr))
        DllAddRef();
#endif
    return hr;
}

// }

//***   CThisDllCF::IClassFactory::* {
//
// static class factory (no allocs!)
STDMETHODIMP_(ULONG) CThisDllCF::AddRef()
{
    DllAddRef();
    return 2;
}

STDMETHODIMP_(ULONG) CThisDllCF::Release()
{
    DllRelease();
    return 1;
}

HRESULT CThisDllCF::QueryInterface(REFIID riid, void ** ppvObj)
{
    HRESULT hr;

    ASSERT((void *)SAFECAST(this, CClassFactory *) == (void *)this);
    hr = CCF_QueryInterface(SAFECAST(this, CClassFactory *), riid, ppvObj);
#if !CCF_EXPT
    if (SUCCEEDED(hr))
        DllAddRef();
#endif
    return hr;
}

HRESULT CThisDllCF::CreateInstance(IUnknown * punkOuter, REFIID riid, void ** ppvObj)
{
    HRESULT hr;

    ASSERT((void *)SAFECAST(this, CClassFactory *) == (void *)this);
    hr = CCF_CreateInstance(SAFECAST(this, CClassFactory *), punkOuter, riid, ppvObj);
    return hr;
}

HRESULT CThisDllCF::LockServer(BOOL fLock)
{
    if (fLock)
        DllAddRef();
    else
        DllRelease();
    DEBUGMSG(ZONE_COM2, (TEXT("sccls: LockServer(%s) to %d"), fLock ? TEXT("LOCK") : TEXT("UNLOCK"), g_cRefThisDll));
    return S_OK;
}

// }

// }
