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
//==========================================================================*
//
// Filename:    cmnutils.cpp
//
// Purpose:     NetUI utility code
//
//==========================================================================*
//==========================================================================*

#include <windows.h>
#include <wpcpriv.h>
#include <ehm.h>

#include <connmgr.h>

#include "netuip.h"
#include "wzcuip.h"
#include "xmlcmcfg.h"


struct NAMECBINFO
{
    PFNENUMNAMECB pfnCallback;
    LPVOID        pvParam;
};

static HRESULT GetNetworkNamesCallback(IXMLDOMElement *pElem, IXMLDOMNode *pNode,
                                       CALLBACK_REASON cr, LPVOID pvParam)
{
    HRESULT    hr = S_OK;
    BSTR       bstr = NULL;
    UINT       iMetaNetwork;
    NAMECBINFO *pncbi;

    if (ENode == cr)
    {
        bstr = CXMLCMConfig::GetCharacteristicValue(pNode, g_szDestId);
        if (NULL != bstr &&
            SUCCEEDED(GetMetaNetworkIndex(bstr, &iMetaNetwork)))
        {
            SysFreeString(bstr);
            bstr = CXMLCMConfig::GetValue(pNode, TEXT("type"));
            CBR(0 != SysStringLen(bstr));

            pncbi = static_cast<NAMECBINFO*>(pvParam);
            hr = pncbi->pfnCallback(iMetaNetwork, bstr, pncbi->pvParam);
            CHR(hr);
        }
    }

Error:
    SysFreeString(bstr);
    return hr;
}

extern "C"
HRESULT EnumMetaNetworkNames(PFNENUMNAMECB pfnCallback, LPVOID pvParam)
{
    HRESULT      hr = S_OK;
    CXMLCMConfig xcfg;
    NAMECBINFO   ncbi;

    // Load up the current entries
    hr = xcfg.GetNetworks(CXMLCMConfig::s_szCM_Network);
    CHR(hr);

    // Create the callback structure
    ncbi.pfnCallback = pfnCallback;
    ncbi.pvParam = pvParam;

    // Get the network names
    xcfg.ProcessXSL(CXMLCMConfig::s_szXSLFindNetworkNames,
        GetNetworkNamesCallback, &ncbi, NULL);

Error:
    return hr;
}

extern "C"
void PostShellStatusMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND s_hwndTaskBar = NULL;

    if (NULL == s_hwndTaskBar || !IsWindow(s_hwndTaskBar))
    {
        s_hwndTaskBar = FindWindow(TEXT("HHTaskBar"), NULL);
    }

    if (NULL != s_hwndTaskBar)
    {
        PostMessage(s_hwndTaskBar, uMsg, wParam, lParam);
    }
}
