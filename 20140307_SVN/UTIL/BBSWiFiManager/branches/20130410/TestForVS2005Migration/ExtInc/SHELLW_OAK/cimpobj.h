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
#ifndef _CIMPOBJ_H_
#define _CIMPOBJ_H_
//***   cimpobj.h -- default implementations for various COM objects
//

// w32, COM
#include "cunknown.h"
#include "cdataobj.h"
#include "cwndproc.h"

// {
//***   ccodly.h -- 'delayed' CoCreateInstance (helper class)
// DESCRIPTION
//  stub for a delayed instance.  used by (e.g.) CContextMenuLite.
// NOTES
//  for now all inline (no .cpp/.lib file).

class CDelayInstance
{
public:
    // n.b. non-virtual iface members!
    HRESULT Initialize(REFIID rclsid)
        {
            _clsidDelay = rclsid;
            return S_OK;
        };
    HRESULT CCIDelay(REFIID riid, void **ppvObj)
        {
            HRESULT hr;

            hr = CoCreateInstance(_clsidDelay, NULL, CLSCTX_ALL, riid, ppvObj);
            if (FAILED(hr)) {
                DEBUGMSG(ZONE_WARNING, (TEXT("cdi.CCID: CCI hr=%x\r\n"), hr));
            }
            return hr;
        };

protected:
    CDelayInstance() { };
    /*virtual*/ ~CDelayInstance() { };

private:
    CLSID       _clsidDelay;
};

// no code, all inline

// }

// {
//***   cmlite.h -- ContextMenu 'Lite'
// DESCRIPTION
//  stub for a delayed instance.
// NOTES
//  for now all inline (no .cpp/.lib file).

class CContextMenuLite :
    public IContextMenu
    ,public CDelayInstance
{
public:
    // *** IUnknown methods ***
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID * ppvObj)
        {
            static const QITAB qit[] = {
                // perf: last tuned NEVER
                QITABENT(CContextMenuLite, IContextMenu),   // IID_IContextMenu
                { 0 },
            };

            // n.b. we're a forwarder, not a tear-off.  if we ever do
            // tear-offs we need to make sure that IUnknown forces an
            // delayed instantiate and returns the 'real' object.
            return QISearch(this, qit,  riid, ppvObj);
        }
    // virtual AddRef();
    // virtual Release();

    // *** IContextMenu methods ***
    // virtual QueryContextMenu
    STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO lpici)
        {
            HRESULT hr;

            hr = _DelayLoad();
            if (SUCCEEDED(hr))
                hr = _pifDelay->InvokeCommand(lpici);
            return hr;
        }
    STDMETHODIMP GetCommandString(UINT idCmd, UINT uType,
        UINT * pwReserved, LPSTR pszName, UINT cchMax) 
            {
                UNREFERENCED_PARAMETER(idCmd);
                UNREFERENCED_PARAMETER(uType);
                UNREFERENCED_PARAMETER(pwReserved);
                UNREFERENCED_PARAMETER(pszName);
                UNREFERENCED_PARAMETER(cchMax);
                return E_NOTIMPL;
            };

    // *** THISCLASS methods ***
    HRESULT Initialize(REFIID rclsid) { return CDelayInstance::Initialize(rclsid); }

protected:
    CContextMenuLite() { _pifDelay = 0; };
    /*virtual*/ ~CContextMenuLite() { if (_pifDelay) _pifDelay->Release(); };
#if 0
    friend HRESULT CContextMenuLite_Create(IUnknown *punkOuter, IUnknown **ppunk, PCLASSFACTORY poi);
#endif

    HRESULT _DelayLoad()
        { return CDelayInstance::CCIDelay(IID_IContextMenu, (void **)&_pifDelay); };

    IContextMenu    *_pifDelay;

private:
};

// }

#endif // _CIMPOBJ_H_
