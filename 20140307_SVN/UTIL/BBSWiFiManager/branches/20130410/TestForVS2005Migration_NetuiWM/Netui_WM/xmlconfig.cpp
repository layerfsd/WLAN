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
// XMLConfig.cpp: implementation of the CXMLConfig class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <ehm.h>

#include "XMLConfig.h"
#include "cfgmgr.h"         // For ConfigManager interfaces
#include "connmgr.h"
#include "conndll.h"
#include "wpcpriv.h"
#include "xmlhelp.h"

//
//
//
const LPCTSTR CXMLConfig::s_szXSLFindNoChar = 
TEXT("//nocharacteristic");

const LPCTSTR CXMLConfig::s_szXSLFindNoCharError = 
TEXT("//nocharacteristic-error");

////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
void CXMLConfig::WriteXML( LPTSTR pszFile )
{
    UNREFERENCED_PARAMETER(pszFile);

#if 0
    if( g_CmdLine && FALSE )
    {
        FileStream fs;
        fs.open(pszFile, FALSE );
        Write(&fs);
    }
#endif
}

///////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLConfig::CXMLConfig()
{
    m_pDocument=NULL;
    m_pNode=NULL;
}

CXMLConfig::~CXMLConfig()
{
    Close();
}


void CXMLConfig::Close()
{
    if (m_pNode)
    {
        m_pNode->Release();
        m_pNode = NULL;
    }
    
    if (m_pDocument)
    {
        m_pDocument->Release();
        m_pDocument = NULL;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XML goo
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Helper classes
//
void CXMLConfig::XMLDump( IXMLDOMNode * pNode )
{
    TCHAR szBuf[MAX_PATH];
    BSTR b=NULL;

    if( pNode == NULL)
        return ;

    int cIndent=0;

    BOOL bStartTag=FALSE, bEndTag=FALSE;

    if( SUCCEEDED( pNode->get_xml(&b) ))
    {
        // we know how big the un-formatted doc is
        // we know that with adding tabs per element and a CRLF per element, we're not going to be more than
        // double in size (each element MUST have a '<' a '>' and  at least ONE char for an element name.
        //         
        LPTSTR pszStart=b;
        LPTSTR pszEnd=pszStart;
        while( *pszEnd )
        {
            pszEnd++;
            if( *pszEnd == TEXT('<') )
            {
                bStartTag=TRUE;
            }
            else
                if( *pszEnd==TEXT('>') )
                {
                    szBuf[0] = TEXT('\0');

                    for (int i = 0; i < cIndent; i++)
                    {
                        StringCchCat(szBuf, ARRAYSIZE(szBuf), TEXT("    "));
                    }

                    *pszEnd=0;
                    if( lstrlen(pszStart) < 180 )
                    {
                        StringCchCat(szBuf, ARRAYSIZE(szBuf), pszStart);
                    }
                    else
                    {
                        StringCchCat(szBuf, ARRAYSIZE(szBuf),
                            TEXT("<... Stuff ..."));
                    }

                    *pszEnd=TEXT('>');
                    pszStart=pszEnd+1;

                    StringCchCat(szBuf, ARRAYSIZE(szBuf), TEXT(">\r\n"));

                    DEBUGMSG(1, (szBuf));

                    bStartTag=FALSE;

                    if (bEndTag || (TEXT('/') == *(pszEnd-1)))
                    {
                        cIndent--;
                        bEndTag = FALSE;
                    }
                }
                else
                    if( *pszEnd == TEXT('/') )
                    {
                        if (bStartTag)
                        {
                            bEndTag = TRUE;
                            cIndent--;
                        }
                    }
                    else
                    {
                        if (bStartTag)
                            cIndent++;
                        bStartTag=FALSE;
                    }
        }
        SysFreeString(b);
    }
}



//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::WriteConfig()
{
    return ProcessConfig(m_pNode, &m_pNode );       // processes the doc, and gets us a node.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This gets all the connection manager settings
// returns the ROOT results of network.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::ProcessConfig(LPCWSTR pszInitialRequest)
{
    HRESULT      hr = S_OK;
    BSTR         bstrXML = NULL;
    VARIANT_BOOL isSuccessful;

    Close();

    // Allocate a BSTR
    bstrXML = ::SysAllocString( pszInitialRequest );
    CPR(bstrXML);

    // Create the document object
    hr = CoCreateInstance(__uuidof(DOMDocument), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IXMLDOMDocument), (void**)&m_pDocument);
    CHR(hr);

    // Load the XML string into the document
    hr = m_pDocument->loadXML(bstrXML, &isSuccessful);
    CHR(hr);

    // Process the document and get the node
    hr = ProcessConfig(m_pDocument, &m_pNode);
    CHR(hr);

Error:
    if (bstrXML)
    {
        SysFreeString(bstrXML);
    }

    if (FAILED(hr))
    {
        Close();
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This gets all the connection manager settings
// Persist the NEW information
// Does a little trick if passed in the DOCUMENT
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::ProcessConfig(IXMLDOMNode * pNode, IXMLDOMNode ** ppResultingDocument )
{
    HRESULT hr=NOERROR;
    BOOL bDocument=FALSE;
    IXMLDOMDocument * pDocument;
    
    if( NULL==pNode)
        pNode=m_pNode;
    
    if (NULL == pNode)
        return E_POINTER;
    
    if( SUCCEEDED( pNode->QueryInterface( __uuidof( IXMLDOMDocument ), (LPVOID*)&pDocument )))
    {
        bDocument=TRUE;
        if( ppResultingDocument == NULL )
            ppResultingDocument = &m_pNode; // this is OK if we are passing in a document, but I think it's wrong otherwise.
    }

    HCURSOR hc = SetCursor(LoadCursor(NULL, IDC_WAIT));
    
    if( bDocument || SUCCEEDED( pNode->get_ownerDocument(&pDocument)) && pDocument )
    {
        IConfigManager * pConfigManager;
        if( SUCCEEDED( hr=CoCreateInstance(__uuidof(CConfigManager), NULL,
            CLSCTX_INPROC_SERVER, __uuidof(IConfigManager),
            (LPVOID*)&pConfigManager)) )
        {
            if( SUCCEEDED( hr=pConfigManager->ProcessDocument( pDocument, CFGFLAG_PROCESS | CFGFLAG_PROCESSINSECURE ) ) )
            {
                // Did they pass in a document, or a node - documents are special
                if( bDocument && ppResultingDocument )
                {
                    // Get the topmost node of the document
                    // do we need to do this every time?
                    IXMLDOMNode * pNewXMLFile;
                    if( SUCCEEDED( pDocument->QueryInterface(__uuidof(IXMLDOMNode), (void**)&pNewXMLFile)) )
                    {
                        // worked! We have the root of the tree.
                        if( *ppResultingDocument )
                            (*ppResultingDocument)->Release();
                        *ppResultingDocument = pNewXMLFile;
                        CleanupErrors();
                    }
                }
            }
            else
            {
                XMLDump(pDocument); // our document
            }            
            pConfigManager->Release();
        }
        pDocument->Release();
    }
    
    SetCursor(hc);
    
    return hr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This will walk the results of an XSL query, calling you back for each element, and node found
// if the callback returns E_FAIL, this enumeration stops.
// if you want to keep onto something returned, use AddRef (and remember to release)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CXMLConfig::ProcessXSL(LPCWSTR pszQuery, PFNNODECALLBACK pFN, LPVOID pVoid, IXMLDOMNode *pNode)
{
    HRESULT hr=E_FAIL;
    
    if (NULL == m_pNode)
        return;
    
    if( pNode == NULL )
        m_pNode->selectSingleNode( L"wap-provisioningdoc", &pNode);
    else
        pNode->AddRef();
    
    IXMLDOMNodeList *       pNodeList       = NULL;
    hr = pFN(NULL, NULL, EStarting, pVoid );    // the query is starting.
    if( SUCCEEDED( hr = pNode->selectNodes( (LPWSTR)pszQuery, &pNodeList)) )
    {
        // Now we have to walk through this list and configure them all...
        // should only be one
        IXMLDOMNode *   pCurrentNode=NULL;
        BOOL bStarted=FALSE;
        while (SUCCEEDED(hr) && (SUCCEEDED(hr = pNodeList->nextNode(&pCurrentNode)) && pCurrentNode))
        {
            if( !bStarted )
            {
                hr = pFN(NULL, pCurrentNode, EStarted, pVoid );
                bStarted=TRUE;
                if( FAILED(hr) )
                {
                    pCurrentNode->Release();
                    break;
                }
            }
            
            // Get the element from this NODE.
            IXMLDOMElement *   pCurrentElem    = NULL;
            if( SUCCEEDED( pCurrentNode->QueryInterface(__uuidof(IXMLDOMElement), (LPVOID*)&pCurrentElem)) )
            {
                // Tell client we got somethings
                hr = pFN(pCurrentElem, pCurrentNode, ENode, pVoid );
                pCurrentElem->Release();
            }
            pCurrentNode->Release();
        }
        
        // Tell client we're done.
        if(bStarted)
            pFN(NULL,NULL, EFinishing, pVoid);
        pNodeList->Release();
    }
    pFN(NULL,NULL, EFinished, pVoid);
    pNode->Release();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Escape special characters to protect XML processing
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CXMLConfig::EscapeXML(LPCTSTR pszIn, LPTSTR *ppszOut)
{
    DWORD cchOut = MAX_PATH;
    *ppszOut = new TCHAR[MAX_PATH];
    
    //XMLHEscapeString(pszIn, ppszOut, &cchOut);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Have the config manager go process something - the XML tree here is local, and doesn't update our document itself
//
// May require us to completely re-load our tree.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::ProcessBSTR(LPCTSTR pszXML, IXMLDOMNode ** ppResultNode )
{
    UNREFERENCED_PARAMETER(ppResultNode);

    IXMLDOMDocument * pDocument;
    HRESULT hr;
    BSTR bstrXML=SysAllocString( pszXML );
    if( SUCCEEDED( hr=CoCreateInstance(__uuidof(DOMDocument), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IXMLDOMDocument), (void**)&pDocument)))
    {
        VARIANT_BOOL isSuccessful;
        if( SUCCEEDED( hr=pDocument->loadXML(bstrXML, &isSuccessful)) )
        {
            IXMLDOMNode * pHiddenNode=NULL;
            hr = ProcessConfig(pDocument, &pHiddenNode);
            if(pHiddenNode)
                pHiddenNode->Release(); // we don't hand out this document, it's local to us.
        }
        pDocument->Release();
    }
    SysFreeString(bstrXML);
    return hr;
}

#define RELEASE(p) { if(p) p->Release(); p=NULL; }

HRESULT CXMLConfig::XMLHChangeTagName(IXMLDOMElement *pElement, LPCWSTR szNewName, IXMLDOMNode **ppOutNode)
{
    HRESULT                 hr           = NOERROR;
    IXMLDOMDocument *       pDOMDoc      = NULL;
    IXMLDOMElement *        pNewElem     = NULL;
    IXMLDOMNamedNodeMap *   pOldMap      = NULL;
    IXMLDOMNode *           pNode        = NULL;
    IXMLDOMNode *           pOutNode     = NULL;
    IXMLDOMAttribute *      pAttr        = NULL;
    IXMLDOMAttribute *      pOutAttr     = NULL;
    IXMLDOMNode *           pParent      = NULL;
    BSTR                    bstrNewName  = NULL;

    bstrNewName = ::SysAllocString(szNewName);
    CPR(bstrNewName);
    
    // Get the Document
    CHR(pElement->get_ownerDocument(&pDOMDoc));
    
    // Create a new Element with the correct name
    CHR(pDOMDoc->createElement(bstrNewName, &pNewElem));
    
    // Get Attribute Maps for both elements
    CHR(pElement->get_attributes(&pOldMap));
    
    CHR(pOldMap->nextNode(&pNode));
    
    while(pNode) {
        CHR(pNode->QueryInterface(__uuidof(IXMLDOMAttribute), (LPVOID*)&pAttr));
        
        // Remove the Attribute from the Old element
        CHR(pElement->removeAttributeNode(pAttr, &pOutAttr));
        
        // Insert the Attribute into the new element
        CHR(pNewElem->setAttributeNode(pOutAttr, NULL));
        
        RELEASE(pOutAttr);
        RELEASE(pAttr);
        RELEASE(pNode);
        
        // Reset the iterator (because we removed the current node)
        CHR(pOldMap->reset());
        CHR(pOldMap->nextNode(&pNode));
    }
    
    // Okay, all attributes moved over.  We're done with the Attribute maps
    RELEASE(pOldMap);
    
    // Now move over child elements
    CHR(pElement->get_firstChild(&pNode));
    
    while(pNode) {
        // Adding the child to a new parent automaticall removes it from
        // the old parent
        CHR(pNewElem->appendChild(pNode, NULL));
        
        RELEASE(pNode);
        
        // Since we've removed the old node, the next child will be the 
        // first child node.
        CHR(pElement->get_firstChild(&pNode));
    }
    
    // Now we have to replace the parentage of the old node.
    CHR(pElement->get_parentNode(&pParent));
    
    CHR(pParent->replaceChild(pNewElem, pElement, ppOutNode));
    
Error:
    RELEASE(pDOMDoc);
    RELEASE(pNewElem);
    RELEASE(pOldMap);
    RELEASE(pNode);
    RELEASE(pOutNode);
    RELEASE(pAttr);
    RELEASE(pOutAttr);
    RELEASE(pParent);
    SysFreeString(bstrNewName);
    
    return hr;
    
}

HRESULT CXMLConfig::Write(IStream *pStream)
{
    IPersistStream *pFile;
    
    if(SUCCEEDED( m_pDocument->QueryInterface( __uuidof( IPersistStream ), (LPVOID*)&pFile ) ))
    {
        pFile->Save(pStream, TRUE);
        pFile->Release();
    }
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XSL from here down to find a parm/@name="pszParm"
// and get the "value" from it.
// YOU free the BSTR (SysFreeString)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BSTR CXMLConfig::GetValue( IXMLDOMNode * pNode, LPCWSTR pszParm  )
{
    if( pNode == NULL )
        return NULL;
    if( pszParm == NULL )
        return NULL;
    
    BSTR bstr=NULL;
    
    // Get the element from this NODE.
    IXMLDOMElement *   pCurrentElem    = NULL;
    if( SUCCEEDED( pNode->QueryInterface(__uuidof(IXMLDOMElement), (LPVOID*)&pCurrentElem)) )
    {
        VARIANT varType;
        VariantInit(&varType);
        if( SUCCEEDED( pCurrentElem->getAttribute((LPWSTR)pszParm, &varType)) )
        {
            // verify that we got a BSTR...
            if (varType.vt == VT_BSTR)
                bstr = ::SysAllocString(varType.bstrVal);
        }
        VariantClear(&varType);              
        pCurrentElem->Release();
    }
    return bstr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XSL from here down to find a parm/@name="pszParm"
// and get the "value" from it.
// YOU free the BSTR (SysFreeString)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BSTR CXMLConfig::GetCharacteristicValue(IXMLDOMNode *pNode, LPCWSTR pszParm)
{
    if( pNode == NULL )
        return NULL;
    if( pszParm == NULL )
        return NULL;
    
    HRESULT hr=E_FAIL;
    
    // Form the query
    TCHAR szQuery[MAX_PATH];
    VHR(StringCchPrintfEx(szQuery, ARRAYSIZE(szQuery), NULL, NULL, STRSAFE_NULL_ON_FAILURE, TEXT("./parm[@name=\"%s\"]"), pszParm ));
    
    BSTR bstr=NULL;
    IXMLDOMNode * pResultNode=NULL;
    
    // Perform the query
    if( SUCCEEDED( hr = pNode->selectSingleNode( szQuery, &pResultNode)) && pResultNode)
    {
        bstr = GetValue( pResultNode, L"value" );
        pResultNode->Release();
    }
    
    return bstr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Creates a <parm name="%pszName%" value="%var%"> as a child of pNode
// will re-use and existing one if there.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::SetCharacteristicValue(IXMLDOMNode *pNode, LPCWSTR pszName, const VARIANT & var)
{
    HRESULT hr=NOERROR;
    VARIANT varType;
    
    IXMLDOMDocument * pDocument;    
    hr=pNode->get_ownerDocument(&pDocument);

    VariantInit(&varType);
    
    if( pDocument )
    {
        varType.vt= VT_BSTR;
        varType.bstrVal=::SysAllocString(pszName);
        
        // names are wide.
        WCHAR szQuery[MAX_PATH];
        CHR(StringCchPrintfW(szQuery, ARRAYSIZE(szQuery), L"parm[@name=\"%s\"]", pszName));
        
        IXMLDOMNode * pExistingNode=NULL;
        if( SUCCEEDED( hr=pNode->selectSingleNode( szQuery, &pExistingNode)) && pExistingNode )
        {
            IXMLDOMElement * pElement;
            if( SUCCEEDED( hr=pExistingNode->QueryInterface( __uuidof( IXMLDOMElement ), (LPVOID*)&pElement)))
            {
                pElement->setAttribute( L"value", var ); // update the value=
                pElement->Release();
            }
            pExistingNode->Release();
        }
        else
        {
            // create the 'holding' characteristic.
            IXMLDOMElement*     pChild      = NULL;
            if( SUCCEEDED( hr=pDocument->createElement((BSTR)L"parm", &pChild)))
            {
                // set the type of the element up - make it unique.
                pChild->setAttribute( L"name", varType );  // parm[@name="pszName"]
                pChild->setAttribute( L"value", var ); // parm[@value="var"]
                hr=pNode->appendChild(pChild, NULL);
                pChild->Release();
            }
        }
    }
Error:
    RELEASE(pDocument);
    VariantClear(&varType);
    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Helper to CreateParm - just takes a string
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::SetCharacteristicValue( IXMLDOMNode * pNode, LPCWSTR pszName, LPCTSTR pszValue )
{
    VARIANT varType;
    VariantInit(&varType);
    varType.vt=VT_BSTR;
    varType.bstrVal=SysAllocString(pszValue);
    HRESULT hr = SetCharacteristicValue(pNode, pszName, varType );
    VariantClear( &varType );
    return hr;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finds an entry, failing that creates it for you.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::CreateXMLNodeOfType( IXMLDOMNode * pNode, LPCWSTR szElementName, LPCTSTR szType, IXMLDOMNode ** ppNewElement)
{
    //
    // Failed to find the entry, create one.
    //
    IXMLDOMDocument*    pDocument   = NULL;
    HRESULT             hr          = NOERROR;
    
    if (pNode == NULL )
        m_pNode->selectSingleNode( L"wap-provisioningdoc", &pNode);
    else
        pNode->AddRef();
    
    hr=pNode->get_ownerDocument(&pDocument);
    
    if( pDocument  )
    {
        IXMLDOMElement*     pChild      = NULL;
        // create the 'holding' characteristic.
        if( SUCCEEDED( hr=pDocument->createElement((LPWSTR)szElementName, &pChild)))
        {
            XMLSetAttribute( pChild, L"type", szType );    // <characteristic type="CM_PPPEntries" >
            
            // root the new element
            if( SUCCEEDED( hr=pNode->appendChild(pChild, NULL)))
            {
                // hand it back
                if (ppNewElement)
                {
                    *ppNewElement = pChild; // does this QI properly down to an IXMLDOMNode from IXMLDOMElement??
                    pChild->AddRef();
                }
            }
            pChild->Release();
        }
        pDocument->Release();
    }
    
    pNode->Release();
    return hr;
}

HRESULT CXMLConfig::XMLSetAttribute( IXMLDOMNode * pNode, LPCWSTR pszElementName, LPCTSTR pszValue )
{
    HRESULT hr=NOERROR;
    
    IXMLDOMElement * pElement;
    if( SUCCEEDED( hr=pNode->QueryInterface( __uuidof( IXMLDOMElement ), (LPVOID*)&pElement)))
    {
        VARIANT varType;
        VariantInit(&varType);
        varType.vt= VT_BSTR;
        
        varType.bstrVal=SysAllocString(pszValue);
        hr= pElement->setAttribute( (LPWSTR)pszElementName, varType );
        
        VariantClear(&varType);
        pElement->Release();
    }
    return hr;
}

BSTR CXMLConfig::XMLGetAttribute( IXMLDOMNode * pNode, LPCWSTR pszElementName )
{
    HRESULT hr=NOERROR;
    BSTR    bstrAttribute = NULL;
    
    IXMLDOMElement * pElement;
    if( SUCCEEDED( hr=pNode->QueryInterface( __uuidof( IXMLDOMElement ), (LPVOID*)&pElement)))
    {
        VARIANT varType;
        
        VariantInit(&varType);
        
        hr= pElement->getAttribute( (LPWSTR)pszElementName, &varType );
        if (SUCCEEDED(hr))
        {
            if (varType.vt == VT_BSTR)
            {
                bstrAttribute = SysAllocString(V_BSTR(&varType));
            }
        }
        
        VariantClear(&varType);
        pElement->Release();
    }
    
    return bstrAttribute;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Given a NODE, will perform an XSL query on it provided, return the Element and OR the node found
//
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::FindNode(IXMLDOMNode *pNode, LPCTSTR pszQueryXSL,
                             IXMLDOMElement **ppElement, IXMLDOMNode **ppNode)
{
    if( ppElement==NULL && ppNode==NULL )
        return E_INVALIDARG;
    
    if( ppElement )
        *ppElement = NULL;
    
    if( ppNode ) 
        *ppNode = NULL;
    
    if (pNode == NULL )
        m_pNode->selectSingleNode( L"wap-provisioningdoc", &pNode);
    else
        pNode->AddRef();
    
    HRESULT hr=E_FAIL;
    IXMLDOMNode     * pResultNode   = NULL;
    if( SUCCEEDED( hr = pNode->selectSingleNode( (LPTSTR)pszQueryXSL, &pResultNode)) && pResultNode)
    {
        // Get them the element from this NODE - kinda common?
        if( ppElement )
            hr= pResultNode->QueryInterface(__uuidof(IXMLDOMElement), (LPVOID*)ppElement);
        
        // Return the NODE if they wanted it.
        if( ppNode )
            *ppNode = pResultNode;
        else
            pResultNode->Release();
    }
    pNode->Release();
    return hr;
}

HRESULT CXMLConfig::FindNode(IXMLDOMNode *pNode, LPCTSTR pszFormat, LPCTSTR pszName,
                             IXMLDOMElement **ppElement, IXMLDOMNode **ppNode)
{
    HRESULT hr = S_OK;
    UINT    cchQuery;
    LPTSTR  pszQuery = NULL;

    ASSERT(NULL != pszFormat);
    ASSERT(NULL != pszName);

    cchQuery = lstrlen(pszFormat) + lstrlen(pszName) + 1;
    pszQuery = new TCHAR[cchQuery];
    CPR(pszQuery);

    hr = StringCchPrintf(pszQuery, cchQuery, pszFormat, pszName);
    CHR(hr);

    hr = FindNode(pNode, pszQuery, ppElement, ppNode);
    CHR(hr);

Error:
    if (pszQuery)
    {
        delete[] pszQuery;
    }

    return hr;
}

HRESULT CXMLConfig::DeleteNodeCallback(IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid)
{
    UNREFERENCED_PARAMETER(pVoid);
    UNREFERENCED_PARAMETER(pCurrentElem);

    switch( cr )
    {
    case EStarted:
        // XMLDump((IXMLDOMNode*)pVoid);
        break;
        
    case ENode:
        {
            IXMLDOMNode * pParentNode;
            XMLDump(pCurrentNode);
            if( SUCCEEDED( pCurrentNode->get_parentNode( &pParentNode )))
            {
                IXMLDOMNode * pRandomNode=NULL;
                pParentNode->removeChild( pCurrentNode, &pRandomNode );
                if(pRandomNode)
                    pRandomNode->Release();
                pParentNode->Release();
            }
        }
        break;
        
    case EFinished:
        // XMLDump((IXMLDOMNode*)pVoid);
        break;
    }
    return S_OK;
}



//
// Clean up config manager errors
//
// We now have to remove stuff we don't like the look of
// nocharacteristic
//
//
void CXMLConfig::CleanupErrors()
{
    if( m_pNode )
    {
        ProcessXSL( s_szXSLFindNoChar,      DeleteNodeCallback, m_pNode, m_pNode );
        ProcessXSL( s_szXSLFindNoCharError, DeleteNodeCallback, m_pNode, m_pNode );
        // ProcessXSL( s_szXSLFindDecSpecific, DeleteNodeCallback, m_pNode, m_pNode );
        
        // remove  UPD info from PPP and GPRS
        TCHAR szQuery[MAX_PATH];
        static LPCTSTR s_rgUPDparms[] = { TEXT("UserName"), TEXT("Password"), TEXT("Domain") };
        for(int i=0;i<ARRAYSIZE(s_rgUPDparms);i++)
        {
            VHR(StringCchPrintfEx(szQuery, ARRAYSIZE(szQuery), NULL, NULL, STRSAFE_NULL_ON_FAILURE, 
                TEXT("/*//characteristic[@type=\"CM_PPPEntries\" or @type=\"CM_GPRSEntries\"]//parm[@name=\"%s\"]"),s_rgUPDparms[i]));
            ProcessXSL( szQuery,   DeleteNodeCallback, m_pNode, m_pNode );
        }
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Given a XML_TO_STRUCT mapping entry, will set the information
// in the pNode. Data starts at pBaseData
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::Win32SetParam( IXMLDOMNode * pNode, PXML_TO_STRUCT_MAPPER pMapper, LPBYTE pBaseData )
{
    VARIANT varType;
    VariantInit(&varType);
    HRESULT hr= NOERROR;
    
    LPBYTE pData = pBaseData+pMapper->dwOffset;
    
    switch(pMapper->type)
    {
    case XVT_DWORD:
        varType.vt= VT_I4;
        varType.lVal = *((DWORD*)pData);
        break;
    case XVT_GUID:
        ASSERT(0);
        break;
    case XVT_IPADDRESS:
        {
            RASIPADDR * pAddr = (RASIPADDR*)(pData);
            TCHAR szIPAddress[ 3*4+3 ];
            CHRA(StringCchPrintf(szIPAddress, ARRAYSIZE(szIPAddress), TEXT("%d.%d.%d.%d"), // pAddr->a, pAddr->b, pAddr->c, pAddr->d );
                pAddr->d, pAddr->c, pAddr->b, pAddr->a ));
            varType.vt= VT_BSTR;
            varType.bstrVal = SysAllocString( szIPAddress );
        }
        break;
    case XVT_BSTR:
        ASSERT(0);
        varType.vt= VT_BSTR;
        varType.bstrVal = SysAllocString( *(LPCTSTR*)pData);
        break;
    case XVT_TCHAR:
        varType.vt= VT_BSTR;
        varType.bstrVal = SysAllocString( (LPCTSTR)pData);
        break;
    default:
        ASSERT(0);
    }
    hr= CreateParm( pNode, pMapper->pszName, varType );
    VariantClear(&varType);

Error:
    return hr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Given a XML_TO_STRUCT mapping entry, will set the information
// in the pNode. Data starts at pBaseData
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::Win32GetParam( IXMLDOMNode * pNode, PXML_TO_STRUCT_MAPPER pMapper, LPBYTE pBaseData )
{
    ASSERT(0);
    BSTR bCurrentValue = GetCharacteristicValue(pNode, pMapper->pszName );
    if( SysStringLen(bCurrentValue) == 0 )
        return E_FAIL;
    
    LPBYTE pData = pBaseData+pMapper->dwOffset;
    
    switch(pMapper->type)
    {
    case XVT_DWORD:
        *((DWORD*)pData) = wcstol(bCurrentValue , NULL,10);
        break;
    case XVT_GUID:
    case XVT_IPADDRESS:
    case XVT_BSTR:
        ASSERT(0);
        break;
    case XVT_TCHAR:
        lstrcpy( (LPTSTR)pData, bCurrentValue);
        break;
    default:
        ASSERT(0);
    }
    SysFreeString(bCurrentValue);
    return S_OK;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Helper to CreateParm - just takes a string
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::CreateParm( IXMLDOMNode * pNode, LPCTSTR pszName, LPCTSTR pszValue )
{
    ASSERT(pszName);
    ASSERT(pszValue);
    VARIANT varType;
    VariantInit(&varType);
    varType.vt=VT_BSTR;
    varType.bstrVal=SysAllocString(pszValue);
    HRESULT hr = CreateParm(pNode, pszName, varType );
    VariantClear( &varType );
    return hr;
}

HRESULT CXMLConfig::CreateParm( IXMLDOMNode * pNode, LPCTSTR pszName, DWORD dwValue )
{
    ASSERT(pszName);
    VARIANT varType;
    VariantInit(&varType);
    varType.vt= VT_I4;
    varType.lVal = dwValue;
    HRESULT hr = CreateParm(pNode, pszName, varType );
    VariantClear( &varType );
    return hr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Creates a <parm name="%pszName%" value="%var%"> as a child of pNode
// will re-use and existing one if there.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CXMLConfig::CreateParm( IXMLDOMNode * pNode, LPCTSTR pszName, VARIANT & var)
{
    HRESULT hr=NOERROR;
    // XMLDump(pNode);
    
    IXMLDOMDocument * pDocument;    
    hr=pNode->get_ownerDocument(&pDocument);
    
    if( pDocument )
    {
        VARIANT varType;
        VariantInit(&varType);
        varType.vt= VT_BSTR;
        varType.bstrVal=SysAllocString(pszName);
        
        TCHAR szQuery[MAX_PATH];
        CHRA(StringCchPrintf(szQuery, ARRAYSIZE(szQuery), TEXT("parm[@name=\"%s\"]"), pszName ));
        IXMLDOMNode * pExistingNode=NULL;
        if( SUCCEEDED( hr=pNode->selectSingleNode( szQuery, &pExistingNode)) && pExistingNode )
        {
            IXMLDOMElement * pElement;
            if( SUCCEEDED( hr=pExistingNode->QueryInterface( __uuidof( IXMLDOMElement ), (LPVOID*)&pElement)))
            {
                pElement->setAttribute( TEXT("value"), var ); // update the value=
                pElement->Release();
            }
            pExistingNode->Release();
        }
        else
        {
            // create the 'holding' characteristic.
            IXMLDOMElement*     pChild      = NULL;
            if( SUCCEEDED( hr=pDocument->createElement((BSTR)TEXT("parm"), &pChild)))
            {
                // set the type of the element up - make it unique.
                pChild->setAttribute( TEXT("name"), varType );  // parm[@name="pszName"]
                
                pChild->setAttribute( TEXT("value"), var ); // parm[@value="var"]
                
                // XMLDump(pChild);
                // root the new element
                hr=pNode->appendChild(pChild, NULL);
                pChild->Release();
            }
        }
        
        VariantClear(&varType);
        pDocument->Release();
    }
    
Error:
    return hr;
}

HRESULT CXMLConfig::DeleteParm( IXMLDOMNode * pNode, LPCTSTR pszName )
{
    HRESULT      hr = NOERROR;
    TCHAR        szQuery[MAX_PATH];
    IXMLDOMNode* pExistingNode=NULL;

    CHR(StringCchPrintf(szQuery, ARRAYSIZE(szQuery), TEXT("parm[@name=\"%s\"]"), pszName));

    if (SUCCEEDED(hr=pNode->selectSingleNode( szQuery, &pExistingNode)) && pExistingNode)
    {
        hr = XMLHDeleteNodeFromDoc(pExistingNode);

        RELEASE(pExistingNode);
    }

Error:
    return hr;
}
