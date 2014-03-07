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
//


#include <windows.h>
#include <wpcpriv.h>
#include <macros.h>
#include <ehm.h>

#include "connmgr_priv.h"
#include "xmlcmcfg.h"
#include "wzcuip.h"


// strings
static const TCHAR gc_szXSLDestIdLocal[] = TEXT("parm[@name=\"DestId\"]");
extern LPCTSTR gc_szXSLDestId;


HRESULT CM_GetEnablingNetwork(UINT iMeta, LPTSTR pszGUID, UINT cchGUID)
{
    HRESULT         hr = S_OK;
    IXMLDOMDocument *pDoc = NULL;
    IXMLDOMNode     *pNode = NULL;
    IXMLDOMNodeList *pNodeList = NULL;
    IXMLDOMElement  *pElem = NULL;
    FINDMETANETWORK fmn = {0};

    fmn.pszMetaNetwork = GetMetaNetworkGUIDString(iMeta);

    if (lstrcmp(fmn.pszMetaNetwork, CXMLCMConfig::s_szGUIDInternet) != 0)
    {
        // Create a document
        hr = CreateProvisioningDoc(&pDoc);
        CHR(hr);

        // Create a Proxy characteristic query
        CHRA(pDoc->get_firstChild(&pNode));
        CHR(CreateCharacteristicQuery(pNode, CXMLCMConfig::s_szCM_ProxyEntries, TRUE, NULL));
        RELEASE(pNode);

        // Process the XML
        hr = ProcessConfig(pDoc);
        CHR(hr);

        // Find any proxies which get to our destination network
        hr = pDoc->selectNodes((BSTR)gc_szXSLDestId, &pNodeList);
        CHR(hr);

        while (S_OK == pNodeList->nextNode(&pNode))
        {
            // Callback
            CHRA(pNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
            CXMLCMConfig::FindMetaNetworkCallback(pElem, pNode, ENode, &fmn);  // don't check HRESULT, since it returns random junk

            RELEASE(pElem);
            RELEASE(pNode);
        }
    }

    // Copy the proxy we found
    hr = StringCchCopy(pszGUID, cchGUID,
        *fmn.szEnabledBy ? fmn.szEnabledBy : fmn.pszMetaNetwork);
    CHR(hr);

Error:
    RELEASE(pElem);
    RELEASE(pNode);
    RELEASE(pNodeList);
    RELEASE(pDoc);
    return hr;
}
