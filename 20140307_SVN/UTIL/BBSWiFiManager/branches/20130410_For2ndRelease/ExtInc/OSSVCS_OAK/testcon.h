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
/***********************************************************
    tetcon.h

    Helper for controlling in-line tests.
    

*/
#pragma once

class ITestCon :
    public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetTestStatus(const TCHAR* pszTestName, BOOL* pfRan) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnTestComplete(const TCHAR* pszTestName, HRESULT hrTest) = 0;
    virtual HRESULT STDMETHODCALLTYPE DumpTestResults() = 0;
};

HRESULT GetTestCon(ITestCon** pptc);


