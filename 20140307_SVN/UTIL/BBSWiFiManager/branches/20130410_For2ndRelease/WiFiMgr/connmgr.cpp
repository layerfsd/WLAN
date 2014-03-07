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
#include <windev.h>
#include <macros.h>
#include <miscsvcs_priv.h>
#include <regext.h>
#include <ehm.h>

#include "cmnutils.h"
#include "wzcmgr.h"
#include "xmlcmcfg.h"
#include "connmgr_priv.h"
#include "cfgmgr.h"         // For ConfigManager interfaces
#include "wzcuip.h"

// strings
static const TCHAR gc_szBase[]                = TEXT("wap-provisioningdoc");
static const TCHAR gc_szCharacteristic[]      = TEXT("characteristic");
static const TCHAR gc_szCharacteristicQuery[] = TEXT("characteristic-query");
static const TCHAR gc_szNoCharacteristic[]    = TEXT("nocharacteristic");
static const TCHAR gc_szParm[]                = TEXT("parm");
static const TCHAR gc_szParmQuery[]           = TEXT("parm-query");
static const TCHAR gc_szType[]                = TEXT("type");
static const TCHAR gc_szName[]                = TEXT("name");
static const TCHAR gc_szValue[]               = TEXT("value");
static const TCHAR gc_szRecursive[]           = TEXT("recursive");
static const TCHAR gc_szDesc[]                = TEXT("Desc.Default");
static const TCHAR gc_szAdapter[]             = TEXT("Adapter");
static const TCHAR gc_szCurrentDTPTNetwork[]  = TEXT("CurrentDTPTNetwork");
static const TCHAR gc_szIpsecVPNAdapter[]     = TEXT("MSEC\\IPSECVPNVNIC1");
static const TCHAR gc_szDestId[]              = TEXT("DestId");

LPCTSTR            gc_szXSLDestId       = TEXT("//parm[@name=\"DestId\"]");
static const TCHAR gc_szXSLAdapter[]    = TEXT("//parm[@name=\"Adapter\"]");
static const TCHAR gc_szXSLNetEntries[] = TEXT("/*/characteristic[@type=\"CM_NetEntries\"]/characteristic");

static const TCHAR c_szRegPathAssocWired[] = WZC_REGKEY_BASE TEXT("\\Ethernet");
static const TCHAR c_szRegPathAssocWiFi[]  = TEXT("Comm\\ConnMgr\\Providers");
static const TCHAR c_szRegKeyAssocAP[]     = TEXT("{9eefcedf-a6bc-4457-a8ae-fb664a196450}");
static const TCHAR c_szRegKeyAssocAdhoc[]  = TEXT("{CB8D126F-71AC-49c5-959E-D796CE558A3B}");
static const TCHAR c_szFmtAssocWiFi[]      = TEXT("%s\\%s\\%s");


static HRESULT CreateChildNode(__notnull IXMLDOMNode *pNodeParent, LPCTSTR pszTag, IXMLDOMNode **ppNode)
{
    HRESULT         hr = S_OK;
    IXMLDOMDocument *pDoc = NULL;
    IXMLDOMElement  *pChild = NULL;

    ASSERT(NULL != pNodeParent);

    // Get the owner document
    if (FAILED(pNodeParent->QueryInterface(__uuidof(IXMLDOMDocument), (void**)&pDoc)))
    {
        CHRA(pNodeParent->get_ownerDocument(&pDoc));
    }

    // Create the child element
    CHR(pDoc->createElement((BSTR)pszTag, &pChild));
    CHRA(pNodeParent->appendChild(pChild, ppNode));

Error:
    RELEASE(pChild);
    RELEASE(pDoc);
    return hr;
}

HRESULT CreateProvisioningDoc(__notnull IXMLDOMDocument **ppDocument)
{
    HRESULT         hr = S_OK;
    IXMLDOMDocument *pDoc = NULL;

    // Create a document
    hr = CoCreateInstance(__uuidof(DOMDocument), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IXMLDOMDocument), (void**)&pDoc);
    CHR(hr);

    // Create the main node
    hr = CreateChildNode(pDoc, gc_szBase, NULL);
    CHR(hr);

    // Success
    *ppDocument = pDoc;
    pDoc = NULL;

Error:
    RELEASE(pDoc);
    return hr;
}

static HRESULT SetElementAttribute(__notnull IXMLDOMElement *pElem, LPCTSTR pszAttr, LPCTSTR pszText)
{
    HRESULT hr = S_OK;
    VARIANT var;

    VariantInit(&var);
    if (pszText)
    {
        // Allocate the string
        var.vt = VT_BSTR;
        var.bstrVal = SysAllocString(pszText);
        CPR(var.bstrVal);

        // Set the attribute
        hr = pElem->setAttribute((BSTR)pszAttr, var);
        CHR(hr);
    }

Error:
    VariantClear(&var);
    return hr;
}

static HRESULT CreateCharacteristic(__notnull IXMLDOMNode *pNodeParent, LPCTSTR pszType, IXMLDOMElement **ppElem)
{
    HRESULT        hr = S_OK;
    IXMLDOMNode    *pNode = NULL;
    IXMLDOMElement *pElem = NULL;

    // Create the element
    hr = CreateChildNode(pNodeParent, gc_szCharacteristic, &pNode);
    CHR(hr);

    // Assign the type
    CHRA(pNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
    hr = SetElementAttribute(pElem, gc_szType, pszType);
    CHR(hr);

    // Success
    if (ppElem)
    {
        *ppElem = pElem;
        pElem = NULL;
    }

Error:
    RELEASE(pElem);
    RELEASE(pNode);
    return hr;
}

HRESULT CreateCharacteristicQuery(IXMLDOMNode *pNodeParent, LPCTSTR pszType,
                                  BOOL fRecursive, IXMLDOMElement **ppElem)
{
    HRESULT        hr = S_OK;
    IXMLDOMNode    *pNode = NULL;
    IXMLDOMElement *pElem = NULL;

    // Create the element
    hr = CreateChildNode(pNodeParent, gc_szCharacteristicQuery, &pNode);
    CHR(hr);

    // Assign the type
    CHRA(pNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
    hr = SetElementAttribute(pElem, gc_szType, pszType);
    CHR(hr);

    if (fRecursive)
    {
        VARIANT var;
        VariantInit(&var);  // no need to clear

        var.vt = VT_BOOL;
        var.boolVal = VARIANT_TRUE;

        hr = pElem->setAttribute((BSTR)gc_szRecursive, var);
        CHR(hr);
    }

    // Success
    if (ppElem)
    {
        *ppElem = pElem;
        pElem = NULL;
    }

Error:
    RELEASE(pElem);
    RELEASE(pNode);
    return hr;
}

static HRESULT CreateNoCharacteristic(IXMLDOMNode *pNodeParent, LPCTSTR pszType,
                                      IXMLDOMElement **ppElem)
{
    HRESULT        hr = S_OK;
    IXMLDOMNode    *pNode = NULL;
    IXMLDOMElement *pElem = NULL;

    // Create the element
    hr = CreateChildNode(pNodeParent, gc_szNoCharacteristic, &pNode);
    CHR(hr);

    // Assign the type
    CHRA(pNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
    hr = SetElementAttribute(pElem, gc_szType, pszType);
    CHR(hr);

    // Success
    if (ppElem)
    {
        *ppElem = pElem;
        pElem = NULL;
    }

Error:
    RELEASE(pElem);
    RELEASE(pNode);
    return hr;
}

static HRESULT CreateParmEx(__inout IXMLDOMNode *pNodeParent, __in LPCTSTR pszName,
                            __in const VARIANT *pvarValue, __out __opt IXMLDOMElement **ppElem)
{
    HRESULT        hr = S_OK;
    IXMLDOMNode    *pNode = NULL;
    IXMLDOMElement *pElem = NULL;

    // Create the element
    hr = CreateChildNode(pNodeParent, gc_szParm, &pNode);
    CHR(hr);

    // Assign the attributes
    CHRA(pNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
    CHR(SetElementAttribute(pElem, gc_szName, pszName));
    hr = pElem->setAttribute((BSTR)gc_szValue, *const_cast<VARIANT*>(pvarValue));
    CHR(hr);

    // Success
    if (ppElem)
    {
        *ppElem = pElem;
        pElem = NULL;
    }

Error:
    RELEASE(pElem);
    RELEASE(pNode);
    return hr;
}

static HRESULT CreateParm(__inout IXMLDOMNode *pNodeParent, __in LPCTSTR pszName,
                          __in LPCTSTR pszValue, __out __opt IXMLDOMElement **ppElem)
{
    HRESULT hr = S_OK;
    VARIANT var;

    VariantInit(&var);

    // Allocate the string
    var.vt = VT_BSTR;
    var.bstrVal = SysAllocString(pszValue);
    CPR(var.bstrVal);

    // Create the parm node
    hr = CreateParmEx(pNodeParent, pszName, &var, ppElem);
    CHR(hr);

Error:
    VariantClear(&var);
    return hr;
}

HRESULT ProcessConfig(IXMLDOMDocument *pDoc)
{
    HRESULT        hr = S_OK;
    IConfigManager *pCfgMgr = NULL;

    // Process the XML
    hr = CoCreateInstance(__uuidof(CConfigManager), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IConfigManager), (void**)&pCfgMgr);
    CHR(hr);

    hr = pCfgMgr->ProcessDocument(pDoc, CFGFLAG_PROCESS | CFGFLAG_PROCESSINSECURE);
    CHR(hr);

Error:
    RELEASE(pCfgMgr);
    return hr;
}

static HRESULT CreateNetworkCharacteristic(__notnull IXMLDOMDocument *pDoc, __notnull LPCTSTR pszCSP,
                                           __notnull LPCTSTR pszNetID, IXMLDOMElement **ppElem)
{
    HRESULT         hr = S_OK;
    IXMLDOMNode     *pNode = NULL;
    IXMLDOMElement  *pElemCSP = NULL;
    IXMLDOMNode     *pNodeCSP = NULL;

    // Create the CSP characteristic
    CHRA(pDoc->get_firstChild(&pNode));
    CHR(CreateCharacteristic(pNode, pszCSP, &pElemCSP));
    CHRA(pElemCSP->QueryInterface(__uuidof(IXMLDOMNode), (void**)&pNodeCSP));

    // Create the network characteristic
    CHR(CreateCharacteristic(pNodeCSP, pszNetID, ppElem));

Error:
    RELEASE(pNodeCSP);
    RELEASE(pElemCSP);
    RELEASE(pNode);
    return hr;
}

HRESULT GetElementValue(IXMLDOMElement *pElem, LPTSTR pszValue, UINT cchValue)
{
    HRESULT hr = S_OK;
    VARIANT var;

    VariantInit(&var);

    CBRAEx(NULL != pszValue && 0 != cchValue, E_INVALIDARG);
    ASSERT(NULL != pElem);

    // Get the value
    pszValue[0] = TEXT('\0');
    hr = pElem->getAttribute((BSTR)gc_szValue, &var);
    if (S_OK != hr)
    {
        goto Exit;
    }

    // Change into a string if necessary
    hr = VariantChangeType(&var, &var, 0, VT_BSTR);
    CHR(hr);

    // Copy the string into the buffer
    hr = StringCchCopy(pszValue, cchValue, var.bstrVal);
    CHR(hr);

Error:
Exit:
    VariantClear(&var);
    return hr;
}

static HRESULT CreateDestGUIDRegistryPath(__out_ecount(cchBuffer) LPTSTR pszBuffer, UINT cchBuffer,
                                          __in __opt LPCTSTR pszName, BOOL fAdhoc)
{
    HRESULT hr = S_OK;

    // If pszName is NULL, it's a wired network card (no SSID)
    if (NULL == pszName)
    {
        hr = StringCchCopy(pszBuffer, cchBuffer, c_szRegPathAssocWired);
        CHR(hr);
    }
    else
    {
        // Create a path using the SSID and type of the network
        hr = StringCchPrintf(pszBuffer, cchBuffer, c_szFmtAssocWiFi,
            c_szRegPathAssocWiFi, fAdhoc ? c_szRegKeyAssocAdhoc : c_szRegKeyAssocAP, pszName);
        CHR(hr);
    }

Error:
    return hr;
}

extern "C"
HRESULT SetNetworkDestGUID(LPCTSTR pszName, BOOL fAdhoc, LPCTSTR pszMetaGUID)
{
    HRESULT hr = S_OK;
    TCHAR   szPath[MAX_PATH];
    HKEY    hkey = NULL;

    // Build the registry key path
    hr = CreateDestGUIDRegistryPath(szPath, ARRAYSIZE(szPath), pszName, fAdhoc);
    CHR(hr);

    // Create the registry key
    CBR(ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, szPath,
        0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hkey, NULL));

    // Write the value to the registry
    hr = RegistrySetString(hkey, NULL, gc_szDestId, pszMetaGUID);
    CHR(hr);

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

extern "C"
HRESULT GetNetworkDestGUID(LPCTSTR pszName, BOOL fAdhoc, LPTSTR pszGUID, UINT cchGUID)
{
    HRESULT hr = S_OK;
    TCHAR   szPath[MAX_PATH];
    HKEY    hkey = NULL;

    // Build the registry key path
    hr = CreateDestGUIDRegistryPath(szPath, ARRAYSIZE(szPath), pszName, fAdhoc);
    CHR(hr);

    // Open the registry key
    CBR(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, szPath,
        0, 0, &hkey));

    // Read the value from the registry
    hr = RegistryGetString(hkey, NULL, gc_szDestId, pszGUID, cchGUID);
    CHR(hr);

Error:
    if (NULL != hkey)
    {
        RegCloseKey(hkey);
    }

    return hr;
}

extern "C"
HRESULT ClearNetworkDestGUID(LPCTSTR pszName, BOOL fAdhoc)
{
    HRESULT hr = S_OK;
    TCHAR   szPath[MAX_PATH];

    // Build the registry key path
    hr = CreateDestGUIDRegistryPath(szPath, ARRAYSIZE(szPath), pszName, fAdhoc);
    CHR(hr);

    // Delete the registry key (all it contains is our destination GUID)
    CBREx(ERROR_SUCCESS == RegDeleteKey(HKEY_LOCAL_MACHINE, szPath), S_FALSE);

Error:
    return hr;
}

static HRESULT FreeLibraries()
{
    HRESULT hr = S_OK;

    // Free the COM DLLs that are loaded. Otherwise, they'll never get
    // unloaded, since we run in device.exe.
    CoFreeUnusedLibrariesEx(0, 0);

//Error:
    return hr;
}

static LPCTSTR GetSafeDeviceName(LPCTSTR pszDevice)
{
    // There's a problem in Configuration Manager that doesn't allow
    // backslashes in characteristic type strings. We've tried to work around
    // in many different ways, but the bottom line is that we can dramatically
    // improve reliability of configuration XML by removing backslashes
    // whenever possible. Since device names are just driver names, they don't
    // typically have slashes, but in some cases, they're preceded by a "path"
    // (i.e. "PCI\<driver-name>"). So in this case, we're just going to ditch
    // the text that precedes the backslash.
    LPCTSTR psz = _tcsrchr(pszDevice, TEXT('\\'));
    return ((psz && psz[1]) ? psz + 1 : pszDevice);
}

extern "C"
HRESULT CM_SetCurrentNetwork(LPCTSTR pszDevice, LPCTSTR pszDesc, UINT iMetaNetwork)
{
    HRESULT         hr = S_OK;
    IXMLDOMDocument *pDoc = NULL;
    IXMLDOMNode     *pNode = NULL;
    IXMLDOMElement  *pElem = NULL;
    TCHAR           szGUID[40];

    // Check inputs
    CBRAEx(iMetaNetwork < GetMetaNetworkCount(), E_INVALIDARG);

    // Create the document
    hr = CreateProvisioningDoc(&pDoc);
    CHR(hr);

    // Create the current network characteristic
    CHR(CreateNetworkCharacteristic(pDoc, CXMLCMConfig::s_szCM_NetEntries,
        GetSafeDeviceName(pszDevice), &pElem));
    CHRA(pElem->QueryInterface(__uuidof(IXMLDOMNode), (void**)&pNode));

    // Get the enabling network
    hr = CM_GetEnablingNetwork(iMetaNetwork, szGUID, ARRAYSIZE(szGUID));
    CHR(hr);

    // Create the DestId and Desc parms
    CHR(CreateParm(pNode, g_szDestId, szGUID, NULL));
    if (NULL != pszDesc)
    {
        CHR(CreateParm(pNode, gc_szDesc, pszDesc, NULL));
    }

    // Create the Adapter parm
    CHR(CreateParm(pNode, gc_szAdapter, pszDevice, NULL));

    // Process the document
    hr = ProcessConfig(pDoc);
    CHR(hr);

Error:
    RELEASE(pElem);
    RELEASE(pNode);
    RELEASE(pDoc);
    FreeLibraries();
    return hr;
}

extern "C"
HRESULT CM_ClearCurrentNetwork(LPCTSTR pszDevice)
{
    HRESULT         hr = S_OK;
    IXMLDOMDocument *pDoc = NULL;
    IXMLDOMElement  *pElem = NULL;
    IXMLDOMNode     *pNode = NULL;

    // Create a document
    hr = CreateProvisioningDoc(&pDoc);
    CHR(hr);

    // Create the CM_NetEntries characteristic
    CHRA(pDoc->get_firstChild(&pNode));
    hr = CreateCharacteristic(pNode, CXMLCMConfig::s_szCM_NetEntries, &pElem);
    CHR(hr);
    RELEASE(pNode);

    // Get the node interface
    CHRA(pElem->QueryInterface(__uuidof(IXMLDOMNode), (void**)&pNode));
    RELEASE(pElem);

    // Clear the device characteristic
    hr = CreateNoCharacteristic(pNode, GetSafeDeviceName(pszDevice), NULL);
    CHR(hr);

    // Process the XML
    hr = ProcessConfig(pDoc);
    CHR(hr);

Error:
    RELEASE(pNode);
    RELEASE(pElem);
    RELEASE(pDoc);
    FreeLibraries();
    return hr;
}

static HRESULT QueryNetEntries(__out IXMLDOMDocument **ppDoc)
{
    HRESULT         hr = S_OK;
    IXMLDOMDocument *pDoc = NULL;
    IXMLDOMNode     *pNode = NULL;
    IXMLDOMNodeList *pNodeList = NULL;
    IXMLDOMNode     *pNodeParent = NULL;
    IXMLDOMNode     *pNodeChild = NULL;
    IXMLDOMElement  *pElem = NULL;
    VARIANT         var;

    VariantInit(&var);

    // Create a document
    hr = CreateProvisioningDoc(&pDoc);
    CHR(hr);

    // Create the CM_NetEntries characteristic query
    CHRA(pDoc->get_firstChild(&pNode));
    hr = CreateCharacteristicQuery(pNode, CXMLCMConfig::s_szCM_NetEntries, TRUE, NULL);
    CHR(hr);
    RELEASE(pNode);

    // Process the XML
    hr = ProcessConfig(pDoc);
    CHR(hr);

    // CM_NetEntries doesn't handle recursive queries properly, so we have to
    // convert all characteristics into characteristic queries
    hr = pDoc->selectNodes((BSTR)gc_szXSLNetEntries, &pNodeList);
    CHR(hr);

    // Iterate through the networks
    while (S_OK == pNodeList->nextNode(&pNode))
    {
        // Get the parent node
        hr = pNode->get_parentNode(&pNodeParent);
        CHR(hr);

        // Get the characteristic type
        CHRA(pNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
        hr = pElem->getAttribute((BSTR)gc_szType, &var);
        CHR(hr);
        RELEASE(pElem);

        // Remove the node
        CHRA(pNodeParent->removeChild(pNode, &pNodeChild));
        RELEASE(pNode);
        RELEASE(pNodeChild);

        // Create the characteristic query
        CHRA(VariantChangeType(&var, &var, 0, VT_BSTR));
        hr = CreateCharacteristicQuery(pNodeParent, var.bstrVal, FALSE, NULL);
        CHR(hr);
        RELEASE(pNodeParent);
        VariantClear(&var);
    }

    // Re-process the configuration
    hr = ProcessConfig(pDoc);
    CHR(hr);

    // Success
    *ppDoc = pDoc;
    pDoc = NULL;

Error:
    VariantClear(&var);
    RELEASE(pElem);
    RELEASE(pNodeChild);
    RELEASE(pNodeParent);
    RELEASE(pNodeList);
    RELEASE(pNode);
    RELEASE(pDoc);
    return hr;
}

static HRESULT IsIpsecVPNAdapter(__in IXMLDOMNode *pNode)
{
    HRESULT        hr = S_OK;
    IXMLDOMElement *pElem = NULL;
    IXMLDOMNode    *pNodeChild = NULL;
    VARIANT        var;
    BSTR		   bStrTemp = NULL;	
    
    VariantInit(&var);
    bStrTemp = SysAllocString(gc_szXSLAdapter);
    CPR(bStrTemp);

    hr = pNode->selectSingleNode(bStrTemp, &pNodeChild);
    if (S_OK == hr)
    {
        // Get the adapter name
        CHRA(pNodeChild->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
        SysFreeString(bStrTemp);
        bStrTemp = SysAllocString(gc_szValue);
        CPR(bStrTemp);
        hr = pElem->getAttribute(bStrTemp, &var);
        CHR(hr);
        RELEASE(pElem);
        CHRA(VariantChangeType(&var, &var, 0, VT_BSTR));
        CBREx(InvStrCmp(var.bstrVal, gc_szIpsecVPNAdapter) == 0, S_FALSE);
        VariantClear(&var);
    }	
    
Error:
    SysFreeString(bStrTemp);
    VariantClear(&var);
    RELEASE(pNodeChild);
    return hr;
}

static HRESULT IsWiredNetwork(__in IXMLDOMNode *pNode)
{
    HRESULT        hr = S_OK;
    IXMLDOMElement *pElem = NULL;
    IXMLDOMNode    *pNodeChild = NULL;
    VARIANT        var;

    VariantInit(&var);

    // Get the characteristic node's type
    CHRA(pNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
    hr = pElem->getAttribute((BSTR)gc_szType, &var);
    CHR(hr);
    RELEASE(pElem);

    // We ignore all networks that have "CurrentDTPTNetwork" as their type.
    // This is really not a good method to reliably eliminate the ActiveSync
    // connection, but it's the best we've got right now.
    CHRA(VariantChangeType(&var, &var, 0, VT_BSTR));
    CBREx(InvStrCmp(var.bstrVal, gc_szCurrentDTPTNetwork), S_FALSE);
    VariantClear(&var);

    // NetEntries that don't specify the adapter name will not be treated as
    // wired network cards
    hr = pNode->selectSingleNode((BSTR)gc_szXSLAdapter, &pNodeChild);
    if (S_OK != hr)
    {
        // Just leave hr intact (could be S_FALSE)
        goto Error;
    }

    // Get the adapter name
    CHRA(pNodeChild->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
    hr = pElem->getAttribute((BSTR)gc_szValue, &var);
    CHR(hr);
    RELEASE(pElem);

    // Check if the adapter is wired
    CHRA(VariantChangeType(&var, &var, 0, VT_BSTR));
    CBREx(DEVICE_WIRED == GetDeviceInfo(var.bstrVal, NULL), S_FALSE);

Error:
    VariantClear(&var);
    RELEASE(pElem);
    RELEASE(pNodeChild);
    return hr;
}

extern "C"
HRESULT CM_SetWiredDestination(UINT iMeta)
{
    HRESULT         hr = S_OK;
    IXMLDOMDocument *pDoc = NULL;
    IXMLDOMNodeList *pNodeList = NULL;
    IXMLDOMNode     *pNode = NULL;
    IXMLDOMNode     *pNodeChild = NULL;
    IXMLDOMNode     *pNodeParent = NULL;
    IXMLDOMElement  *pElem = NULL;
    VARIANT         varGUID;
    TCHAR           szGUID[40];

    VariantInit(&varGUID);

    // First, save the setting
    hr = SetNetworkDestGUID(NULL, FALSE, GetMetaNetworkGUIDString(iMeta));
    CHR(hr);

    // Query the current NetEntries
    hr = QueryNetEntries(&pDoc);
    CHR(hr);

    // Select the NetEntries nodes
    hr = pDoc->selectNodes((BSTR)gc_szXSLNetEntries, &pNodeList);
    CHR(hr);

    // Iterate through the networks
    while (S_OK == pNodeList->nextNode(&pNode))
    {
        // Is this a wired network and is not the Yona VPN adapter?
        if (S_OK == IsWiredNetwork(pNode) &&
			S_OK != IsIpsecVPNAdapter(pNode) &&
            S_OK == pNode->selectSingleNode((BSTR)gc_szXSLDestId, &pNodeChild))
        {
            if (VT_EMPTY == varGUID.vt)
            {
                // Get the enabling network
                hr = CM_GetEnablingNetwork(iMeta, szGUID, ARRAYSIZE(szGUID));
                CHR(hr);

                // Create the variant
                varGUID.vt = VT_BSTR;
                varGUID.bstrVal = SysAllocString(szGUID);
                CPR(varGUID.bstrVal);
            }

            // Set the new destination network
            CHRA(pNodeChild->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pElem));
            hr = pElem->setAttribute((BSTR)gc_szValue, varGUID);
            CHR(hr);
            RELEASE(pElem);
        }
        else
        {
            // Just remove the network from our XML
            CHRA(pNode->get_parentNode(&pNodeParent));
            CHRA(pNodeParent->removeChild(pNode, &pNodeChild));
            RELEASE(pNodeParent);
        }

        // Clean up
        RELEASE(pNodeChild);
        RELEASE(pNode);
    }

    // If we actually updated any networks, we need to send the new configuration XML
    if (VT_EMPTY != varGUID.vt)
    {
        hr = ProcessConfig(pDoc);
        CHR(hr);
    }

Error:
    VariantClear(&varGUID);
    RELEASE(pElem);
    RELEASE(pNodeParent);
    RELEASE(pNodeChild);
    RELEASE(pNode);
    RELEASE(pNodeList);
    RELEASE(pDoc);
    FreeLibraries();
    return hr;
}

extern "C"
UINT CM_GetWiredDestination()
{
    HRESULT hr = S_OK;
    TCHAR   szGUID[40];
    UINT    iMeta = 1;  // default wired network is Work

    // Just check what the most recently saved selection was. Technically,
    // we're not actually checking the current wired connection (if any), but
    // the saved selection should always be correct.
    hr = GetNetworkDestGUID(NULL, FALSE, szGUID, ARRAYSIZE(szGUID));
    CHR(hr);

    GetMetaNetworkIndex(szGUID, &iMeta);

Error:
    return iMeta;
}
