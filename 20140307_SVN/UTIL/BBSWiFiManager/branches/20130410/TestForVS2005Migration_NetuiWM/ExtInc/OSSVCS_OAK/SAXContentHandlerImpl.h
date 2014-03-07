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
// ************************************************************
// SAXContentHandlerImpl.h
// 
// Header for wrapper SAX content handler class
//
// Copyright 2003 Microsoft Corporation, All Rights Reserved
//
// ************************************************************

#pragma once

class CSAXContentHandlerImpl : public ISAXContentHandler  
{
public:
    // Constructor/Destructor
    CSAXContentHandlerImpl() {}
    virtual ~CSAXContentHandlerImpl() {}

    // IUnknown methods
    STDMETHOD (QueryInterface)(REFIID riid, void **ppv) PURE;
    STDMETHOD_(ULONG, AddRef)() PURE;
    STDMETHOD_(ULONG, Release)() PURE;

    // ISAXContentHandler methods
    HRESULT STDMETHODCALLTYPE putDocumentLocator( 
            /* [in] */ ISAXLocator* /* pLocator */)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE startDocument(void)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE endDocument(void)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE startPrefixMapping( 
            /* [in] */ const wchar_t* /* pwchPrefix */,
            /* [in] */ int /* cchPrefix */,
            /* [in] */ const wchar_t* /* pwchUri */,
            /* [in] */ int /* cchUri */)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE endPrefixMapping( 
            /* [in] */ const wchar_t* /* pwchPrefix */,
            /* [in] */ int /* cchPrefix */)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE startElement( 
            /* [in] */ const wchar_t* /* pwchNamespaceUri */,
            /* [in] */ int /* cchNamespaceUri */,
            /* [in] */ const wchar_t* /* pwchLocalName */,
            /* [in] */ int /* cchLocalName */,
            /* [in] */ const wchar_t* /* pwchQName */,
            /* [in] */ int /* cchQName */,
            /* [in] */ ISAXAttributes* /* pAttributes */)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE endElement( 
            /* [in] */ const wchar_t* /* pwchNamespaceUri */,
            /* [in] */ int /* cchNamespaceUri */,
            /* [in] */ const wchar_t* /* pwchLocalName */,
            /* [in] */ int /* cchLocalName */,
            /* [in] */ const wchar_t* /* pwchQName */,
            /* [in] */ int /* cchQName */)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE characters( 
            /* [in] */ const wchar_t* /* pwchChars */,
            /* [in] */ int /* cchChars */)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE ignorableWhitespace( 
            /* [in] */ const wchar_t* /* pwchChars */,
            /* [in] */ int /* cchChars */)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE processingInstruction( 
            /* [in] */ const wchar_t* /* pwchTarget */,
            /* [in] */ int /* cchTarget */,
            /* [in] */ const wchar_t* /* pwchData */,
            /* [in] */ int /* cchData */)
    {
        return S_OK;
    }
        
    HRESULT STDMETHODCALLTYPE skippedEntity( 
            /* [in] */ const wchar_t* /* pwchName */,
            /* [in] */ int /* cchName */)
    {
        return S_OK;
    }

private:
};

