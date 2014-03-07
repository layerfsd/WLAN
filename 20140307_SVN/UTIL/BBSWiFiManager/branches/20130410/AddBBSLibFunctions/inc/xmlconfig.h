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
// XMLConfig.h: interface for the CXMLConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLCONFIG_H__D269A5B2_E69B_4AAB_B5BC_9C8954A068B3__INCLUDED_)
#define AFX_XMLCONFIG_H__D269A5B2_E69B_4AAB_B5BC_9C8954A068B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <msxml.h>

//
// These are the reasons why we call you back
//
typedef enum CALLBACK_REASON
{
    EStarting,  // the query has started, if we find something 
        EStarted,   // you will get a started notification
        EFinishing, // finishing if we sent you a started
        EFinished,  // and finished when we've done with the query
        ENode,
};

// create a table of mappings from struct offsets and types to names
typedef enum
{
    XVT_DWORD,      // 4 bytes
        XVT_GUID,       // a guid
        XVT_IPADDRESS,  // 4 strings
        XVT_BSTR,       // a POINTER to a string
        XVT_TCHAR,      // an array of chars
} XVARTYPE;

// Maps a struct to some XML entries.
typedef struct _XML_TO_STRUCT_MAPPER
{
    LPCTSTR pszName;    // name="%s"
    DWORD   dwOffset;   // offset into the struct where the data is.
    XVARTYPE type;
} XML_TO_STRUCT_MAPPER, *PXML_TO_STRUCT_MAPPER;

class CXMLConfig  
{
public:
    BOOL NoDocument() { return m_pDocument==NULL; }
    IXMLDOMDocument * GetDocument() { return m_pDocument; }
    
    
    // saving out the XML
    HRESULT Write(IStream * pStream);
    
    //
    // XSL processing
    //
    typedef HRESULT (* PFNNODECALLBACK)(IXMLDOMElement *, IXMLDOMNode *, CALLBACK_REASON cr, LPVOID pVoid);
    
    void ProcessXSL(LPCWSTR pszQuery, PFNNODECALLBACK pFN, LPVOID pVoid, IXMLDOMNode * pNode );
    HRESULT WriteConfig();
    HRESULT ProcessConfig( IXMLDOMNode * pNode=NULL, IXMLDOMNode ** ppResultingDocument=NULL );
    HRESULT ProcessConfig(LPCWSTR pszInitialRequest);
    HRESULT ProcessBSTR(LPCTSTR pszXML, IXMLDOMNode ** ppResultNode );
    
    CXMLConfig();
    virtual ~CXMLConfig();
    void Close();
    
    static HRESULT XMLHChangeTagName(IXMLDOMElement *pElement, LPCWSTR szNewName, IXMLDOMNode **ppOutNode);
    
    static BSTR GetCharacteristicValue( IXMLDOMNode * pNode, LPCWSTR pszName );
    static HRESULT SetCharacteristicValue( IXMLDOMNode *pNode,  LPCWSTR pszName, const VARIANT & var);
    static HRESULT SetCharacteristicValue( IXMLDOMNode * pNode, LPCWSTR pszName, LPCTSTR pszValue );
    
    static BSTR GetValue( IXMLDOMNode * pNode, LPCWSTR pszName );
    
    HRESULT FindNode(IXMLDOMNode *pNode, LPCTSTR pszQueryXSL, IXMLDOMElement **ppElement, IXMLDOMNode **ppNode);
    HRESULT FindNode(IXMLDOMNode *pNode, LPCTSTR pszFormat, LPCTSTR pszName, IXMLDOMElement **ppElement, IXMLDOMNode **ppNode);
    HRESULT CreateXMLNodeOfType( IXMLDOMNode * pNode, LPCWSTR szElementName, LPCTSTR szType, IXMLDOMNode ** ppNewElement);
    static HRESULT XMLSetAttribute( IXMLDOMNode * pNode, LPCWSTR pszElementName, LPCTSTR pszValue );
    static BSTR  XMLGetAttribute( IXMLDOMNode * pNode, LPCWSTR pszElementName);
    
    static void XMLDump( IXMLDOMNode * pNode );
    
    static const LPCTSTR s_szXSLFindNoChar;
    static const LPCTSTR s_szXSLFindNoCharError;
    
    // Win32 Struct helpers
    HRESULT Win32SetParam( IXMLDOMNode * pNode, PXML_TO_STRUCT_MAPPER pMapper, LPBYTE pBaseData );
    HRESULT Win32GetParam( IXMLDOMNode * pNode, PXML_TO_STRUCT_MAPPER pMapper, LPBYTE pBaseData );
    
    //
    HRESULT CreateParm( IXMLDOMNode * pNode, LPCTSTR pszName, DWORD dwValue );
    HRESULT CreateParm( IXMLDOMNode * pNode, LPCTSTR pszName, LPCTSTR pszValue );
    HRESULT CreateParm( IXMLDOMNode * pNode, LPCTSTR pszName, VARIANT & var);

    static HRESULT DeleteParm( IXMLDOMNode * pNode, LPCTSTR pszName );
    
    static void EscapeXML(LPCTSTR pszIn, LPTSTR *pszOut);
    
    void CleanupErrors();
    
protected:
    
    // This finds all the information about all information stores.
    IXMLDOMNode * m_pNode;
    IXMLDOMDocument * m_pDocument;
    void WriteXML( LPTSTR pszFile );
    static HRESULT DeleteNodeCallback(IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid);
    
};

#endif // !defined(AFX_XMLCONFIG_H__D269A5B2_E69B_4AAB_B5BC_9C8954A068B3__INCLUDED_)
