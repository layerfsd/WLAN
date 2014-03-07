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
// XMLCMCfg.h: interface for the CXMLCMCfg class.
//
// Specific configuration stuff for connection manager, mainly helpers
// and defined constants.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLCMCFG_H__D269A5B2_E69B_4AAB_B5BC_9C8954A068B3__INCLUDED_)
#define AFX_XMLCMCFG_H__D269A5B2_E69B_4AAB_B5BC_9C8954A068B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XMLConfig.h"

#define GUID_LEN 40

typedef enum NCITEM_TYPE
{
    E_UNKNOWN_TYPE=0,
        E_NETWORK,
        E_PPP,
        E_VPN,
        E_PROXY,
        E_NETCARD,
        E_WIFI,
        E_GPRS,
};

typedef struct _tagFINDMETANETWORK
{
    LPCTSTR pszMetaNetwork;         // what you're looking for - g_InternetGUID or g_CorpGUID
    LPCTSTR pszExcludeID;               // src guid to exclude when searching for dest 
    TCHAR   szEnabledBy[GUID_LEN];  // the network that gets you there.
    TCHAR   szEnabledType;          // the type of proxy, 0 or 1
} FINDMETANETWORK, * PFINDMETANETWORK;

class CXMLCMConfig  : public CXMLConfig
{
public:
    HRESULT GetSettings();
    HRESULT GetNetworks(LPCTSTR pszCSP);
    void    SetNeedsRefresh(BOOL b);

    CXMLCMConfig();
    virtual ~CXMLCMConfig();
    
    // NIC
    HRESULT SetNic( LPTSTR pszNetwork);
    HRESULT GetNic( LPTSTR *ppszNetwork);
    
    HRESULT GetEnablingNetwork( BOOL bInternet, __out_ecount(cchSelectedNetwork) LPTSTR pszSelectedNetwork, UINT cchSelectedNetwork);
    HRESULT GetMetaNetworkByEnablingNetwork(LPCTSTR pszEnablingNetwork, LPCTSTR* ppszMetaNetwork);

    static const LPCWSTR s_szNetworkRequestBIG;
    static const LPCTSTR s_szXSLFindNetworkNames;
    static const LPCTSTR s_szXSLFindEntries;

    static const LPCTSTR s_szCM_Network ;
    static const LPCTSTR s_szCM_PPPEntries ;
    static const LPCTSTR s_szCM_GPRSEntries ;
    static const LPCTSTR s_szCM_VPNEntries ;
    static const LPCTSTR s_szCM_ProxyEntries ;
    static const LPCTSTR s_szCM_NetEntries ;
    static const LPCWSTR s_szCM_WiFiEntries;

    static const LPCTSTR s_szGUIDInternet;
    static const LPCTSTR s_szGUIDCorp;
    static const LPCTSTR s_szGUIDWAP;
    static const LPCTSTR s_szGUIDSecureWAP;
    
    HRESULT FindNetEntry(LPCTSTR pszEntryName, LPCTSTR pszCSP,
        IXMLDOMNode **ppNewNode, BOOL fCreate = FALSE);

    DWORD DeleteEntryOfType(
        IXMLDOMNode * pDomNode,  // pointer to full path and file name of 
        //  phone-book file
        LPCTSTR lpszEntry,       // pointer to an entry name to delete
        NCITEM_TYPE eType
        );
    
    static HRESULT NetworkQuery(IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid);
    static HRESULT NetworkQueryRAS(IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid);
    static HRESULT VPNRemoveParmQueryRAS(IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid);

    static BOOL IsInternet( LPCTSTR pszGUID );
    static BOOL IsCorpnet( LPCTSTR pszGUID );
    static BOOL IsMetaNetwork( LPCTSTR pszGUID );

    static LPCTSTR GetElementType( NCITEM_TYPE eType );

    static HRESULT FindMetaNetworkCallback(IXMLDOMElement * pCurrentElem, IXMLDOMNode * pCurrentNode, CALLBACK_REASON cr, LPVOID pVoid);

protected:

    // From the first query, this replaces them all with re-queries.
    HRESULT ExpandNetworks();
    
    BOOL    m_bNeedsRefresh;
    typedef struct _tagEXPAND
    {
        UINT    uiCount;
        LPCTSTR * szParms;
    } EXPAND_NETWORKS;
    
    
    HRESULT FindNodeOfTypeAndName( IXMLDOMNode * pDomNode, LPCTSTR pszEntry, NCITEM_TYPE eType, IXMLDOMElement ** ppElement);
    static const LPCWSTR s_szXSLFindPPPEntries;
    static const LPCWSTR s_szXSLFindVPNEntries;
    static const LPCWSTR s_szXSLFindVPNEntriesPPTP;
    
    // cache the enabling networks
    TCHAR   m_szgInet[GUID_LEN+1];
    TCHAR   m_szgCorp[GUID_LEN+1];
};

extern const LPCTSTR g_szDestId ;


//
// Disconnect connection manager, and optionally refresh the XML
//
void DoDisconnect(BOOL bAndRefresh);


#endif // !defined(AFX_XMLCMCFG_H__D269A5B2_E69B_4AAB_B5BC_9C8954A068B3__INCLUDED_)
