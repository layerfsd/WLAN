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
/*****************************************************************************

    File: XMLHelp.h
    
    Helper functions for XML parsing

    History:
        Created - KK 3/18/00
        updated - ES 9/17/00
*/
#ifndef _XMLHELP_H_
#define _XMLHELP_H_

#include "msxml.h"

//just get the text of an attribute
HRESULT XMLHGetBstrVarFromAttr(IXMLDOMNode* pNode, const TCHAR* pszAttributeName, VARIANT* pvar);
HRESULT XMLHGetBstrVarFromAttr2(IXMLDOMNode* pNode, const TCHAR* pszAttributeName, VARIANT* pvar);
HRESULT XMLHGetStringAttributeVal(IXMLDOMNode* pnode, const TCHAR* pszAttributeName, const TCHAR* pszDefault, TCHAR* pszOut, UINT cchOut);

//obtain attribute values through the use of standard OLE coersion (VariantChangeType)
HRESULT XMLHGetIntAttributeVal(IXMLDOMNode* pnode, const TCHAR* pszAttributeName, int iDefault, int* piOut);
HRESULT XMLHGetBooleanAttributeVal(IXMLDOMNode* pnode, const TCHAR* pszAttributeName, BOOL fDefault, BOOL* pfOut);
HRESULT XMLHGetCLSIDAttributeVal(IXMLDOMNode* pNode, const TCHAR* pszAttributeName, CLSID* pClsidOut);
    
//these get values separated by comma or translation mapping
HRESULT XMLHGetTranslatedAttributeVal(IXMLDOMNode* pnode, const TCHAR* pszAttributeName, DWORD dwDefault, DWORD* pdwOut, ...);
HRESULT XMLHGetCommaSeperatedIntAttributeVal(IXMLDOMNode* pnode, const TCHAR* pszAttributeName, int iDefault, ...);
HRESULT XMLHGetRectAttributeVal(IXMLDOMNode* pnode, const TCHAR* pszAttributeName, const RECT* prcDefault, RECT* prcOut);
HRESULT XMLHGetCOLORREFAttributeVal(IXMLDOMNode* pnode, const TCHAR* pszAttributeName, COLORREF crDefault, COLORREF* pcrOut, BOOL fDefault, BOOL* pfTransparent);

// gets the text of an element
HRESULT XMLHGetElementText(IXMLDOMNode* pnode, const TCHAR* pszDefault, TCHAR* pszOut, UINT cchOut);
HRESULT XMLHGetSearchElementText(IXMLDOMNode* pnode, const TCHAR* pszTagName, const TCHAR* pszDefault, TCHAR* pszOut, UINT cchOut);

// creates an array of ids and a matching array of values for an id/value pair
// name strings are mapped to ids by using the rgMap param
struct NameIDMapElem
{
    const WCHAR* wszName;
    INT iID;
};
typedef HRESULT (*PFN_GETVALUE)(IXMLDOMNode *pNode, void *pValue);
HRESULT XMLHGetIDValueList(IXMLDOMNodeList *pNodeList, const NameIDMapElem* const rgMap, int cMapElems, PFN_GETVALUE pfnGetValue, INT **prgIDs, INT **prgVals, int *pcVals);

HRESULT XMLHChangeTagName(IXMLDOMElement *pElement, const TCHAR* szNewName, IXMLDOMNode **ppOutNode);
HRESULT XMLHChangeTagNameEx(IXMLDOMElement *pElement, LPCTSTR szNewName, IXMLDOMNode **ppOutNodeOld, IXMLDOMElement **ppOutElemNew);
HRESULT XMLHDeleteNodeListFromDoc(IXMLDOMNodeList *pNodeList);
HRESULT XMLHDeleteNodeFromDoc(IXMLDOMNode *pNode);
HRESULT XMLHDeleteAttributesWithExclusions(IXMLDOMNode *pNode, UINT cExclusions, ... );
HRESULT XMLHPrintNode(IXMLDOMNode *pNode, int iIndent);

// searches from the parent search node, for the first element of name search string, in the order of system languages
HRESULT XMLHGetStringWithDesiredLang(IXMLDOMNode* pnodeToBeSearched, const TCHAR* szSearchString, HINSTANCE hinstDefault, const TCHAR* pszDefault, TCHAR* pszOut, UINT cchOut);
HRESULT XMLHGetNodeWithDesiredLang(IXMLDOMNode *pNodeToBeSearched, const TCHAR* szSearchString, IXMLDOMNode **ppNodeOut);

HRESULT XMLHGetOwnerDoc(IXMLDOMNode *nodeXML, IXMLDOMDocument **ppDocument);
HRESULT XMLHCreateChildElement(IXMLDOMNode *nodeXML, const TCHAR* szNodeName, IXMLDOMDocument *pDocument, IXMLDOMElement **pNewElement);
HRESULT XMLHSetTextAttribute(IXMLDOMElement *pNode, const TCHAR* szAttrName, const TCHAR* szAttrValue);

HRESULT XMLHGetParentPosition(IXMLDOMNode* pChildNode, LONG* piPosition);
HRESULT XMLHInsertChildNodeByPosition(IXMLDOMNode* pNewParentNode, IXMLDOMNode* pChildInsert, LONG iPosition, IXMLDOMNode** ppNewChild);

HRESULT XMLHSkipXMLProlog(LPTSTR szXML, LPTSTR *pszXML);

// Get an attribute from this node or a sub node
HRESULT XMLHGetSubnodeAttr(IXMLDOMNode *piDOMNode, WCHAR const *szSubNodePath, WCHAR const *szAttr, WCHAR **pszRetString);

HRESULT XMLHEscapeString(LPCTSTR pszIn, LPTSTR* ppszOut, DWORD *pcchOut);


#endif // _XMLHELP_H_
