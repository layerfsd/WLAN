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
// XMLCmcfg.cpp: implementation of the CXMLCMConfig class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <wpcpriv.h>
#include <ehm.h>

#include "XMLCMCfg.h"
#include <ntddndis.h>

#define RELEASE_OBJ(obj)   { if (obj) { obj->Release(); obj=NULL; } }


/////////////////////////// Private Strings ///
const LPCTSTR g_szDestId = TEXT("DestId");


/////////////////////////// Exported //////

const LPCTSTR CXMLCMConfig::s_szXSLFindEntries = 
TEXT("/*/characteristic[@type=\"%s\"]");

// This finds all the information about all information stores.
const LPCTSTR CXMLCMConfig::s_szNetworkRequestBIG =
TEXT("<wap-provisioningdoc> \
                              <characteristic-query type=\"CM_Networks\"     recursive=\"true\" />  \
                              <characteristic-query type=\"CM_ProxyEntries\" recursive=\"true\" /> \
                              <characteristic-query type=\"CM_VPNEntries\"   recursive=\"false\" /> \
                              <characteristic-query type=\"CM_WiFiEntries\"   recursive=\"true\" />   \
                              <characteristic-query type=\"CM_NetEntries\"   recursive=\"true\" /> \
                              <characteristic-query type=\"CM_PPPEntries\"   recursive=\"false\" />   \
                              <characteristic-query type=\"CM_GPRSEntries\"   recursive=\"false\" />   \
                              </wap-provisioningdoc>");


const LPCTSTR CXMLCMConfig::s_szCM_Network =         TEXT("CM_Networks");
const LPCTSTR CXMLCMConfig::s_szCM_PPPEntries =      TEXT("CM_PPPEntries");
const LPCTSTR CXMLCMConfig::s_szCM_GPRSEntries =     TEXT("CM_GPRSEntries");
const LPCTSTR CXMLCMConfig::s_szCM_VPNEntries =      TEXT("CM_VPNEntries");
const LPCTSTR CXMLCMConfig::s_szCM_ProxyEntries =    TEXT("CM_ProxyEntries");
const LPCTSTR CXMLCMConfig::s_szCM_NetEntries =      TEXT("CM_NetEntries");
const LPCTSTR CXMLCMConfig::s_szCM_WiFiEntries =     TEXT("CM_WiFiEntries");

///////////////////////////
LPWSTR s_szDestId=L"DestId";
LPWSTR s_szSrcId=L"SrcId";
LPWSTR s_szProxy  = L"Proxy";


// Our special Meta networks
// We should be using the GUIDs defined in connmgr.h
const LPCTSTR CXMLCMConfig::s_szGUIDInternet = TEXT("{436EF144-B4FB-4863-A041-8F905A62C572}");
const LPCTSTR CXMLCMConfig::s_szGUIDCorp = TEXT("{A1182988-0D73-439E-87AD-2A5B369F808B}");
const LPCTSTR CXMLCMConfig::s_szGUIDWAP = TEXT("{7022E968-5A97-4051-BC1C-C578E2FBA5D9}");
const LPCTSTR CXMLCMConfig::s_szGUIDSecureWAP = TEXT("{F28D1F74-72BE-4394-A4A7-4E296219390C}");

//
const LPCWSTR CXMLCMConfig::s_szXSLFindNetworkNames =
L"/*/characteristic[@type=\"CM_Networks\"]/characteristic";  // leaves you pointing at the type="Microsoft"

// used to convert the parm to a parm-query
const LPCWSTR CXMLCMConfig::s_szXSLFindPPPEntries =
L"/*/characteristic[@type=\"CM_PPPEntries\" or @type=\"CM_GPRSEntries\"]/characteristic";
const LPCWSTR CXMLCMConfig::s_szXSLFindVPNEntries =
L"/*/characteristic[@type=\"CM_VPNEntries\"]/characteristic";

const LPCWSTR CXMLCMConfig::s_szXSLFindVPNEntriesPPTP =
TEXT("/*/characteristic[@type=\"CM_VPNEntries\"]/characteristic/parm[@name=\"Type\" and @value=\"0\"]");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLCMConfig::CXMLCMConfig()
{
    SetNeedsRefresh(FALSE);
    m_szgInet[0]=0;
    m_szgCorp[0]=0;
}

CXMLCMConfig::~CXMLCMConfig()
{
}

void CXMLCMConfig::SetNeedsRefresh(BOOL b)
{
    if(b)
    {
        m_szgInet[0]=0;
        m_szgCorp[0]=0;
    }
    m_bNeedsRefresh=b; 
}

HRESULT CXMLCMConfig::NetworkQuery(IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid)
{
    UNREFERENCED_PARAMETER(pVoid);
    UNREFERENCED_PARAMETER(pCurrentNode);

    switch( cr )
    {
    case EStarted:
        break;
        
    case ENode:
        {
            IXMLDOMNode * pRandomNode=NULL;
            CXMLConfig::XMLHChangeTagName( pCurrentElem, L"characteristic-query", &pRandomNode );
            CXMLConfig::XMLSetAttribute( pRandomNode, L"recursive", L"false" );
            if( pRandomNode )
                pRandomNode->Release();
        }
        break;
        
    case EFinished:
        break;
    }
    return S_OK;
}

// when getting the RAS stuff - only get a few of the params.
HRESULT CXMLCMConfig::NetworkQueryRAS(IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid)
{
    HRESULT hr = S_OK;
    
    switch( cr )
    {
    case EStarted:
        break;
        
    case ENode:
        {
            EXPAND_NETWORKS * pEN=(EXPAND_NETWORKS*)pVoid;
            // find the existing nodes and rename them, or create a new one.
            HRESULT hr=S_OK;
            TCHAR szQuery[MAX_PATH];
            UINT i;
            for(i=0;i<pEN->uiCount;i++)
            {
                CHRA(StringCchPrintf(szQuery, ARRAYSIZE(szQuery), TEXT("./parm[@type='%s']"), pEN->szParms[i] ));
                IXMLDOMNode     * pResultNode   = NULL;
                if( SUCCEEDED( hr = pCurrentNode->selectSingleNode( szQuery, &pResultNode)) && pResultNode)
                {
                    IXMLDOMNode * pRandomNode=NULL;
                    IXMLDOMElement * pElement;
                    if( SUCCEEDED( pResultNode->QueryInterface( __uuidof( IXMLDOMElement ), (LPVOID*)&pElement )))
                    {
                        CXMLConfig::XMLHChangeTagName( pElement, L"parm-query", &pRandomNode );
                        CXMLConfig::XMLDump(pRandomNode);
                        if( pRandomNode )
                            pRandomNode->Release();
                        pResultNode->Release();
                        pElement->Release();
                    }
                }
                else
                {
                    // no node to rename - create one please.
                    IXMLDOMDocument * pDocument;    
                    hr=pCurrentElem->get_ownerDocument(&pDocument);
                    
                    if( pDocument )
                    {
                        IXMLDOMElement*     pChild      = NULL;
                        if( SUCCEEDED( hr=pDocument->createElement((BSTR)L"parm-query", &pChild)))
                        {
                            // set the type of the element up - make it unique.
                            VARIANT varType;
                            VariantInit(&varType);
                            varType.vt= VT_BSTR;
                            varType.bstrVal=::SysAllocString(pEN->szParms[i]);
                            
                            pChild->setAttribute( L"name", varType );  // parm[@name="pszName"]
                            hr=pCurrentNode->appendChild(pChild, NULL);
                            pChild->Release();
                            VariantClear(&varType);
                        }
                        pDocument->Release();
                    }
                }
            }
        }
        break;
        
    case EFinished:
        break;
    }

Error:
    return hr;
}

HRESULT CXMLCMConfig::VPNRemoveParmQueryRAS(IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid)
{
    UNREFERENCED_PARAMETER(pCurrentElem);

    switch( cr )
    {
    case EStarted:
        break;
        
    case ENode:
        {
            EXPAND_NETWORKS* pEN=(EXPAND_NETWORKS*)pVoid;
            IXMLDOMNode*     pNodeParent = NULL;
            UINT             i;
        
            if ( SUCCEEDED( pCurrentNode->get_parentNode(&pNodeParent) ) && pNodeParent )
            {
                for( i = 0; i < pEN->uiCount; i++)
                {
                    DeleteParm(pNodeParent, pEN->szParms[i]);
                }
            }
        }
        break;

    case EFinished:
        break;
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// changes the characteristic(s) to characteristic-query
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EN_PARM( query, array, CallBack )     { en.uiCount=ARRAYSIZE(array); en.szParms=array; ProcessXSL( query,    CallBack, (LPVOID)&en, NULL); }
HRESULT CXMLCMConfig::ExpandNetworks()
{
    HRESULT hr = S_OK;
    
    const LPCTSTR szNodeTypes[]={ 
        //        CXMLCMConfig::s_szCM_Network,
        //        s_szCM_VPNEntries,
        //        s_szCM_ProxyEntries,
        s_szCM_NetEntries,
            s_szCM_WiFiEntries};
        
        if( NoDocument() )
            ProcessConfig( s_szNetworkRequestBIG );
        
        // recursive="true" has been used to get this information.
        // however NetEntries doesn't seem to understand this.
        TCHAR szQuery[MAX_PATH];
        for(int i=0;i<ARRAYSIZE(szNodeTypes);i++)
        {
            CHRA(StringCchPrintf(szQuery, ARRAYSIZE(szQuery), TEXT( "/*/characteristic[@type=\"%s\"]/characteristic" ), szNodeTypes[i] ));
            ProcessXSL( szQuery ,  NetworkQuery, NULL, NULL);
        }
        
        static LPCTSTR s_RASParms[] = { TEXT("DestId"), TEXT("AreaCode"), TEXT("Phone"), TEXT("CountryCode"), TEXT("DeviceName"), TEXT("Enabled"), TEXT("UserName"), TEXT("Domain"), TEXT("Password") };
        static LPCTSTR s_VPNParms[] = { TEXT("SrcId"), TEXT("DestId"), TEXT("Phone"), TEXT("Enabled") , TEXT("UserName"), TEXT("Domain"), TEXT("Password"), TEXT("Type") };
        static LPCTSTR s_VPNParmsIPSec[] = { TEXT("IPSecAuth"), TEXT("PresharedKey") };
        
        // walk the network nodes, change them from char to char-query
        EXPAND_NETWORKS en;
        EN_PARM( s_szXSLFindPPPEntries, s_RASParms,      CXMLCMConfig::NetworkQueryRAS ); // does both GPRS and PPP
        EN_PARM( s_szXSLFindVPNEntries, s_VPNParms,      CXMLCMConfig::NetworkQueryRAS ); 
        EN_PARM( s_szXSLFindVPNEntries, s_VPNParmsIPSec, CXMLCMConfig::NetworkQueryRAS );
        
        hr = ProcessConfig();

        // There are nodes "IPSecAuth" and "PresharedKey" for both PPTP and IPSec/L2TP VPN type.
        // Need it for creating a rollback document. Strange! For detail info,
        // please reference the code in cspras_config.cpp at public\ossvcs\connmgr\cspras
        
        // We have to remove "IPSecAuth" and "PresharedKey" from PPTP VPN type
        EN_PARM( s_szXSLFindVPNEntriesPPTP, s_VPNParmsIPSec, CXMLCMConfig::VPNRemoveParmQueryRAS );

        CXMLConfig::XMLDump( GetDocument() );

Error:        
        return hr;
}

HRESULT CXMLCMConfig::GetNetworks(LPCTSTR pszCSP)
{
    HRESULT hr;
    TCHAR   szBuffer[MAX_PATH];

    ASSERT(NULL != pszCSP);

    // Create the initial request XML
    static const TCHAR s_szNetworkRequest[] = TEXT("<wap-provisioningdoc>")
                                                  TEXT("<characteristic-query type=\"%s\" recursive=\"true\"/>")
                                              TEXT("</wap-provisioningdoc>");

    hr = StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), s_szNetworkRequest, pszCSP);
    CHRA(hr);

    // Make the request
    hr = ProcessConfig(szBuffer);
    CHR(hr);

    if (s_szCM_WiFiEntries == pszCSP || s_szCM_NetEntries == pszCSP)
    {
        // The netcsp doesn't seem to understand recursive too well, so we force it
        static const TCHAR s_szCharacteristicXSL[] = TEXT("/*/characteristic[@type=\"%s\"]/characteristic");
        hr = StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), s_szCharacteristicXSL, pszCSP);
        CHRA(hr);

        ProcessXSL(szBuffer, NetworkQuery, NULL, NULL);
        hr = ProcessConfig();
        CHR(hr);
    }

Error:
    return hr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Called back for every CM_ProxyEntries
// determins if it's enabled for the given network (FINDMETANETWORK)
// That is the DestId == *pFMN->pszMetaNetwork AND Enabled==1
//
// perhaps in the future, make this get both metanetworks at once?
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLCMConfig::FindMetaNetworkCallback( IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid )
{   
    UNREFERENCED_PARAMETER(pCurrentElem);

    PFINDMETANETWORK pFMN=(PFINDMETANETWORK)pVoid;
    switch( cr )
    {
        // pCurrentElem = NULL, pCurrentNode = NULL
    case EStarting: // we have started a new query - we don't know if there are any nodes yet.
        *pFMN->szEnabledBy=0;
        pFMN->szEnabledType=0;
        break;
        
    case ENode: // adding a node - makes sure we're adding something useful
        {
            if( pFMN->szEnabledType==TEXT('0') ) 
                return E_FAIL; // only find one.
            
            IXMLDOMNode * pNode;
            // we come in at the SrcId level, back up to get access to the other fun guys!
            if( SUCCEEDED( pCurrentNode->selectSingleNode( L"..", &pNode) ) && pNode )
            {
                XMLDump(pNode);
                BSTR bEnable = GetCharacteristicValue( pNode, L"Enable");
                if( bEnable && (lstrcmp(bEnable, TEXT("1"))==0) )
                {
                    // if it's null or HTTP that is enabled.
                    BSTR bType = GetCharacteristicValue( pNode, L"Type");
                    if( bType && ((lstrcmp( bType, TEXT("0") )== 0) || (lstrcmp(bType, TEXT("1"))==0)) )
                    {
                        // does this 'connection' get us over an enabled path to our destination??
                        BSTR bDestId = GetCharacteristicValue( pNode, s_szDestId );
                        if( lstrcmpi( bDestId , pFMN->pszMetaNetwork ) == 0 )
                        {
                            BSTR bSrcId = GetCharacteristicValue( pNode, s_szSrcId);
                            BOOL bCanUse=TRUE;
                            if( pFMN->szEnabledType==TEXT('1') ) // already found a type 1 proxy HTTP
                            {
                                if( pFMN->pszExcludeID &&(lstrcmpi( bSrcId,pFMN->pszExcludeID) == 0 )) // found another HTTP proxy 
                                    bCanUse=FALSE;                      // from Corp to Inet - don't use it.
                            }
                            
                            if( bCanUse )
                            {
                                lstrcpy( pFMN->szEnabledBy, bSrcId);
                                pFMN->szEnabledType=(TCHAR)*bType;  // remember which type it is.
                            }
                            SysFreeString(bSrcId);
                        }
                        SysFreeString(bDestId);
                    }
                    SysFreeString(bType);
                }
                SysFreeString(bEnable);
                pNode->Release();
            }
        }
        break;
    }
    return S_OK;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//
// Will fill pszSelectedNetwork with the network which is 
// enabling either the internet or the corpnet
// e.g. My Internet Settings, My Work Settings
//
HRESULT CXMLCMConfig::GetEnablingNetwork(BOOL bInternet, __out_ecount(cchSelectedNetwork) LPTSTR pszSelectedNetwork, UINT cchSelectedNetwork)
{
    HRESULT hr = S_OK;
    
    if( bInternet && m_szgInet[0] )
    {
        return StringCchCopy( pszSelectedNetwork, cchSelectedNetwork, m_szgInet );
    }
    
    if( !bInternet && m_szgCorp[0] )
    {
        return StringCchCopy( pszSelectedNetwork, cchSelectedNetwork, m_szgCorp );
    }
    
    FINDMETANETWORK fmn;
    //
    // Dest corporate GUID thing.
    // only show ISPs
    //
    static const LPCWSTR pszXSLFindEnabledMetaNetwork = 
        TEXT("/*/characteristic[@type=\"CM_ProxyEntries\"]/characteristic/parm[@name=\"DestId\" and @value=\"%s\"]");
    
    TCHAR szQuery[MAX_PATH];
    CHRA(StringCchPrintf(szQuery, ARRAYSIZE(szQuery), pszXSLFindEnabledMetaNetwork, bInternet ?
        s_szGUIDInternet : s_szGUIDCorp));
    
    fmn.pszMetaNetwork = bInternet ? s_szGUIDInternet : s_szGUIDCorp;
    fmn.pszExcludeID=bInternet?m_szgCorp:NULL;          // if ISP has HTTP proxy then use it in preference to corp HTTP proxy
    ProcessXSL( szQuery , FindMetaNetworkCallback, &fmn, NULL);
    if( *fmn.szEnabledBy )
    {
        CHRA(StringCchCopy( pszSelectedNetwork, cchSelectedNetwork, fmn.szEnabledBy));
        
        if (bInternet)
        {
            CHRA(StringCchCopy(m_szgInet, ARRAYSIZE(m_szgInet), pszSelectedNetwork));
        }
        else
        {
            CHRA(StringCchCopy(m_szgCorp, ARRAYSIZE(m_szgCorp), pszSelectedNetwork));
        }
    }
    else
    {
        hr = E_FAIL;
    }

Error:
    return hr;
}


HRESULT CXMLCMConfig::GetMetaNetworkByEnablingNetwork(LPCTSTR pszEnablingNetwork, LPCTSTR* ppszMetaNetwork)
{
    HRESULT              hr = E_FAIL;
    TCHAR   szGUID[GUID_LEN];
    
    CBREx(ppszMetaNetwork != NULL, E_INVALIDARG);
    
    if (   SUCCEEDED(GetEnablingNetwork(FALSE, szGUID, ARRAYSIZE(szGUID)))
        && !_tcsicmp(pszEnablingNetwork, szGUID))
        {
            *ppszMetaNetwork = s_szGUIDCorp;
            hr = S_OK;
        goto Error;
        }
    if (   SUCCEEDED(GetEnablingNetwork(TRUE, szGUID, ARRAYSIZE(szGUID)))
        && !_tcsicmp(pszEnablingNetwork, szGUID))
        {
            *ppszMetaNetwork = s_szGUIDInternet;
            hr = S_OK;
        goto Error;
    }

Error:
    return hr;
}

LPCTSTR CXMLCMConfig::GetElementType( NCITEM_TYPE eType )
{
    LPCTSTR pszTypeName=NULL;
    switch ( eType )
    {
    case E_NETWORK: 
        pszTypeName = s_szCM_Network;
        break;
        
    case E_PPP:
        pszTypeName = s_szCM_PPPEntries;
        break;
        
    case E_GPRS: 
        pszTypeName = s_szCM_GPRSEntries;
        break;
        
    case E_VPN:
        pszTypeName = s_szCM_VPNEntries;
        break;
        
    case E_PROXY: 
        pszTypeName = s_szCM_ProxyEntries;
        break;
        
    case E_NETCARD: 
        pszTypeName = s_szCM_NetEntries;
        break;
        
    case E_WIFI:
        pszTypeName = s_szCM_WiFiEntries;
        break;
        
    default:
        ASSERT(0);
        return NULL;
    }
    return pszTypeName;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This will re-use and exisgint PPPEntry or create a new one.
//
// NULL, CM_PPPEntries, "My ISP"
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLCMConfig::FindNetEntry(LPCTSTR pszEntryName, LPCTSTR pszCSP, IXMLDOMNode **ppNewNode, BOOL fCreate)
{
    HRESULT     hr = S_OK;
    const UINT  cchMaxSSID = ARRAYSIZE(((PNDIS_802_11_SSID)NULL)->Ssid);
    TCHAR       szBuffer[MAX_PATH+cchMaxSSID];
    IXMLDOMNode *pCSPNode = NULL;

    ASSERT(NULL != pszEntryName);
    ASSERT(NULL != pszCSP);
    ASSERT(NULL != ppNewNode);

    // Create the XSL query
    static const TCHAR s_szQueryXSL[] = TEXT("/*/characteristic[@type=\"%s\"]/characteristic[@type=\"%s\"]");
    hr = StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), s_szQueryXSL, pszCSP, pszEntryName);
    CHRA(hr);  // We explicitly added enough space for the SSID, so that must be one LONG CSP name...

    // Try to find the node
    if (SUCCEEDED(hr = FindNode(NULL, szBuffer, NULL, ppNewNode)) && *ppNewNode)
    {
        goto Exit;
    }

    // Create the node if necessary
    if (fCreate)
    {
        static const TCHAR s_szCharacteristic[] = TEXT("characteristic");
        IXMLDOMNode *pCSPNode;

        // Create the XSL query
        hr = StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), s_szXSLFindEntries, pszCSP);
        CHRA(hr);

        // Find or create the CSP characteristic node
        if (FAILED(FindNode(NULL, szBuffer, NULL, &pCSPNode)) || NULL == pCSPNode)
        {
            // Create a new one
            hr = CreateXMLNodeOfType(NULL, s_szCharacteristic, pszCSP, &pCSPNode);
            CHR(hr);
            ASSERT(NULL != pCSPNode);
        }

        // Create the network entry
        hr = CreateXMLNodeOfType(pCSPNode, s_szCharacteristic, pszEntryName, ppNewNode);
        CHR(hr);
    }

Error:
Exit:
    if (NULL != pCSPNode)
    {
        pCSPNode->Release();
    }

    return hr;
}

BOOL CXMLCMConfig::IsInternet( LPCTSTR pszGUID )
{
    return lstrcmpi(pszGUID, s_szGUIDInternet) == 0;
}

BOOL CXMLCMConfig::IsCorpnet( LPCTSTR pszGUID )
{
    return lstrcmpi(pszGUID, s_szGUIDCorp) == 0;
}

BOOL CXMLCMConfig::IsMetaNetwork( LPCTSTR pszGUID )
{
    if( IsInternet(pszGUID) )
        return TRUE;
    
    if (lstrcmpi(pszGUID, s_szGUIDWAP) == 0)
        return TRUE;
    
    if (lstrcmpi(pszGUID, s_szGUIDSecureWAP) == 0)
        return TRUE;
    
    if( IsCorpnet(pszGUID) )
        return TRUE;
    
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Helper for the VPN and PPPEntries
//
///////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CXMLCMConfig::DeleteEntryOfType(
                                      IXMLDOMNode * pDomNode,  // pointer to full path and file name of 
                                      //  phone-book file
                                      LPCTSTR lpszEntry,       // pointer to an entry name to delete
                                      NCITEM_TYPE eType
                                      )
{
    UNREFERENCED_PARAMETER(pDomNode);

    IXMLDOMElement * pExisting;
    if( SUCCEEDED( FindNodeOfTypeAndName( NULL, lpszEntry, eType, &pExisting )) && pExisting )
    {
        // delete the existing node.
        IXMLDOMNode * pParentNode;
        if( SUCCEEDED( pExisting->get_parentNode( &pParentNode )))
        {
            // save the type
            IXMLDOMNode * pRandomNode;
            // XMLDump(pExisting);
            VARIANT varType;
            VariantInit(&varType);
            if( SUCCEEDED( pExisting->getAttribute(TEXT("type"), &varType)) )
            {
                // remove the node from our XML tree.
                pParentNode->removeChild( pExisting, &pRandomNode );
                if(pRandomNode)
                    pRandomNode->Release();
                
                IXMLDOMDocument * pDocument;
                HRESULT hr=pExisting->get_ownerDocument(&pDocument);
                
                if( pDocument  )
                {
                    IXMLDOMElement*     pChild      = NULL;
                    if( SUCCEEDED( hr=pDocument->createElement(TEXT("nocharacteristic"), &pChild)))
                    {
                        XMLSetAttribute( pChild, TEXT("type"), varType.bstrVal );
                        hr=pParentNode->appendChild(pChild, NULL);
                        
                        // no delete the entry.
                        // XMLDump(pParentNode);
                        ProcessConfig( pParentNode, &m_pNode );
                        
                        pChild->Release();
                    }
                    pDocument->Release();
                }
            }
            VariantClear(&varType);
            pParentNode->Release();
        }
        
        pExisting->Release();
    }
    return ERROR_INVALID_NAME;
}

HRESULT CXMLCMConfig::SetNic(LPTSTR pszNetwork)
{
    IXMLDOMNode * pWirelessNode;
    if( SUCCEEDED( FindNetEntry(TEXT("CurrentWirelessNetwork"), s_szCM_NetEntries, &pWirelessNode, TRUE)) && pWirelessNode)
    {
        SetCharacteristicValue( pWirelessNode, L"DestId", pszNetwork );
        pWirelessNode->Release();
    }
    return S_OK;
}

HRESULT CXMLCMConfig::GetNic(LPTSTR *ppszBuf)
{
    HRESULT hr = E_OUTOFMEMORY;
    IXMLDOMNode *pNode;
    
    if (NULL == ppszBuf)
        return E_POINTER;
    
    if ( SUCCEEDED( FindNetEntry(TEXT("CurrentWirelessNetwork"), s_szCM_NetEntries, &pNode)) && pNode)
    {
        IXMLDOMNode *pNodeValue = NULL;
        
        if ( SUCCEEDED( pNode->selectSingleNode(L"./parm[@name=\"DestId\"]",
            &pNodeValue) ) )
        {
            BSTR bstrValue = GetValue( pNodeValue, L"value" );
            
            if (SysStringLen(bstrValue) != 0)
            {
                *ppszBuf = bstrValue;
                hr = S_OK;
                SysFreeString(bstrValue);
            }
            
            pNodeValue->Release();
        }
        
        pNode->Release();
    }
    
    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Given a root node, witll find an entry by name, based on a type.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLCMConfig::FindNodeOfTypeAndName( IXMLDOMNode * pDomNode, LPCTSTR pszEntry, NCITEM_TYPE eType, IXMLDOMElement ** ppElement)
{
    HRESULT hr= S_OK;

    if( ppElement == NULL )
        return E_INVALIDARG;
    
    LPCTSTR pszTypeName=GetElementType( eType );
    
    if( pszTypeName==NULL)
        return E_INVALIDARG;
    
    // Look for node in GPRS as well, when etype is E_PPP
    if (E_PPP == eType)
    {
        // Use const instead of hardcoded below ?
        // This string is not going to be used in other places
        // to use code further below.
        pszTypeName = TEXT("CM_PPPEntries\" or @type=\"CM_GPRSEntries");
    }

    TCHAR szFormat[MAX_PATH];
    CHRA(StringCchPrintf(szFormat, ARRAYSIZE(szFormat), TEXT("/*/characteristic[@type=\"%s\"]/characteristic[@type=\"%%s\"]"), pszTypeName));
    
    CHR(FindNode( pDomNode, szFormat, pszEntry, ppElement, NULL ) );

Error:
    return hr;
}

HRESULT CXMLCMConfig::GetSettings()
{
    HRESULT hr;
    if( SUCCEEDED( hr=ProcessConfig(s_szNetworkRequestBIG) ) )
        ExpandNetworks();

    return hr;
}
