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
// Filename:    connmgr_priv.h
//
// Purpose:     NetUI and Connection Manager interaction
//
//==========================================================================*
//==========================================================================*
#pragma once

#include <msxml2.h>


///<topic name="connections" displayname="Connections">
/// <topic_scope tref="Shell"/>
/// <summary>Device Connections</summary>
///</topic>

///<topic_scope tref="connections">

/// <summary>
/// Get the GUID string of the network that enables access to the specified
/// metanetwork.
/// </summary>
HRESULT CM_GetEnablingNetwork(UINT iMeta, __out_ecount(cchGUID) LPTSTR pszGUID, UINT cchGUID);

/// <summary>
/// Create the root document XML for provisioning.
/// </summary>
HRESULT CreateProvisioningDoc(__out IXMLDOMDocument **ppDocument);

/// <summary>
/// Create a characteristic query node.
/// </summary>
HRESULT CreateCharacteristicQuery(__in IXMLDOMNode *pNodeParent, __in LPCTSTR pszType,
                                  BOOL fRecursive, __out __opt IXMLDOMElement **ppElem);

/// <summary>
/// Process configuration XML through the Configuration Manager.
/// </summary>
HRESULT ProcessConfig(__in IXMLDOMDocument *pDoc);

/// <summary>
/// Get the value attribute of an element, in string form.
/// </summary>
HRESULT GetElementValue(__in IXMLDOMElement *pElem, __out_ecount(cchValue) LPTSTR pszValue, UINT cchValue);


#ifdef __cplusplus
extern "C" {
#endif


/// <summary>
/// Tell the Connection Manager which metanetwork the currently connected
/// network accesses.
/// </summary>
HRESULT CM_SetCurrentNetwork(__in LPCTSTR pszDevice, __in __opt LPCTSTR pszDesc, UINT iMeta);

/// <summary>
/// Remove the currently connected network from the Connection Manager network
/// entries list.
/// </summary>
HRESULT CM_ClearCurrentNetwork(__in LPCTSTR pszDevice);

/// <summary>
/// Set the destination network for wired network cards.
/// </summary>
HRESULT CM_SetWiredDestination(UINT iMeta);

/// <summary>
/// Get the destination network for wired network cards.
/// </summary>
UINT CM_GetWiredDestination();

HRESULT SetNetworkDestGUID(__in __opt LPCTSTR pszName, BOOL fAdhoc, __in LPCTSTR pszGUID);
HRESULT GetNetworkDestGUID(__in __opt LPCTSTR pszName, BOOL fAdhoc, __out_ecount(cchGUID) LPTSTR pszGUID, UINT cchGUID);
HRESULT ClearNetworkDestGUID(__in __opt LPCTSTR pszName, BOOL fAdhoc);

///</topic_scope> Connections

#ifdef __cplusplus
}
#endif
