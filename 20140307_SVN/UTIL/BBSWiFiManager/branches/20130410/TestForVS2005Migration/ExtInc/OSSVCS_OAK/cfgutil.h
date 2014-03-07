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
/*---------------------------------------------------------------------------*\
 *
 * Configuration Manager
 *
 * (c) Copyright Microsoft Corp. 2000 All Rights Reserved
 *
 *  module: cfgmgr.idl
 *  author: scottsh
 *
 *  purpose: Configuration Manager utility functions for writing CSPs
 *
 *  history: 7/24/2000      Scott Shell (ScottSh)       Created
 *           8/02/2000      evans                       added a bunch more fns
\*---------------------------------------------------------------------------*/

#pragma once

#include "cfgmgr.h"

#define MAKE_CFGPARSEINFO( dw, sz ) { (dw), (sz) }
#define ENUM_MAKE_CFGPARSEINFO( en, sz ) MAKE_CFGPARSEINFO( (DWORD)(en), (sz) )
#define FROM_TAG_MAKE_CFGPARSEINFO( dw ) MAKE_CFGPARSEINFO( (dw), TEXT(#dw) )
#define FROM_ENUM_TAG_MAKE_CFGPARSEINFO( en ) FROM_TAG_MAKE_CFGPARSEINFO( (DWORD)(en) )

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    DWORD dwId;
    LPCTSTR szName;
} CFGPARSEINFO;

enum enumMetabaseDB
{
    eMetabaseDBPath,
    eMetabaseDBLabels,
    eMetabaseDBOptions
};

HRESULT CfgUtilOpenMetabaseDatabase(PCEGUID pguidMetabaseVol, PCEGUID pguidLangVol,
    enumMetabaseDB eMetabaseDB, CEPROPID propidSort, DWORD dwFlags, HANDLE* phDB);
HRESULT CfgUtilMountMetabaseVolumes(PCEGUID pguidMetabaseVol, PCEGUID pguidLangVol);

HRESULT CfgUtilCheckCharacteristicNode(IXMLDOMNode *pNode);

DWORD CfgUtilParseName(CFGPARSEINFO const pi[], UINT iSize, LPCTSTR szName);
DWORD CfgUtilParseNameByLen(CFGPARSEINFO const pi[], UINT iSize, LPCTSTR szName, DWORD cchName);
LPCTSTR CfgUtilGetName(CFGPARSEINFO const pi[], UINT iSize, DWORD dwId);

ConfigNodeType CfgUtilParseElementName(LPCTSTR szElementName);
LPCTSTR CfgUtilGetElementName(ConfigNodeType cnt);
HRESULT CfgUtilGetNodeType(IXMLDOMElement *pElement, ConfigNodeType *pNodeType);

ConfigParmAttributeType CfgUtilParseParmAttributeName(LPCTSTR szAttributeName);
LPCTSTR CfgUtilGetParmAttributeName(ConfigParmAttributeType cat);

ConfigDataType CfgUtilParseConfigDataTypeName(LPCTSTR szDTName);
LPCTSTR CfgUtilGetConfigDataTypeName(ConfigDataType cdt);

ConfigSemanticType CfgUtilParseConfigSemanticTypeName(LPCTSTR szSTName);
LPCTSTR CfgUtilGetConfigSemanticTypeName(ConfigSemanticType cst);

ConfigTranslationType CfgUtilParseConfigTranslationTypeName(LPCTSTR szTTName);
LPCTSTR CfgUtilGetConfigTranslationTypeName(ConfigTranslationType ctt);
HRESULT CfgUtilGetElementTranslationType(IXMLDOMElement *pCurrentElem, ConfigTranslationType *peTransType);

LPCTSTR CfgUtilGetErrorDescription(HRESULT hr);
HRESULT CfgUtilGetXpathFromRootNode(IXMLDOMElement *pElement, BSTR *pbstrPathFromRoot, ConfigNodeType eNodeType);

BSTR CfgUtilChopPathAfterSegment(BSTR bstrPath, DWORD dwSegment);
BSTR CfgUtilRemovePathSegment(BSTR bstrPath, DWORD dwSegment);

HRESULT CfgUtilCreateCharacteristic(IXMLDOMNode *nodeXML, LPCTSTR szNodeType, IXMLDOMElement **pNewElement);
HRESULT CfgUtilCreateParm(IXMLDOMNode *nodeXML, LPCTSTR szNodeName, LPCTSTR szNodeValue, IXMLDOMElement **pNewElement);

HRESULT CfgUtilRemoveChildNode(IXMLDOMElement *pChild, IXMLDOMElement *pParent);

/// <summary>
///     Return the path to the recovery file, with the filename that is passed in
///     or a default filename (if the input filename is not specified).
/// </summary>
/// <param name="pszRecoveryFile">[in] Recovery file name to use</param>
/// <param name="pszBuffer">[out] Path to recovery file</param>
/// <param name="cchBuffer">[in] Size (in characters) of the output buffer</param>
/// <returns>
///     <para>Returns HRESULT type</para>
/// </returns>
HRESULT CfgUtilGetCfgMgrRecoveryFile(
    __in_opt                LPCTSTR pszRecoveryFile,
    __out_ecount(cchBuffer) LPTSTR  pszBuffer, 
                            DWORD   cchBuffer);

/// <summary>
///     Return the path to the directory where the recovery data is held.
/// </summary>
/// <param name="pszBuffer">[out] Buffer to hold the path</param>
/// <param name="cchBuffer">[in] Buffer size, in characters</param>
/// <returns>
///     <para>Returns HRESULT type</para>
/// </returns>
HRESULT CfgUtilGetCfgMgrRecoveryDirectory(__out_ecount(cchBuffer) LPTSTR pszBuffer, DWORD cchBuffer);

HRESULT CfgUtilReplaceString(LPCTSTR szText, LPCTSTR szName, LPCTSTR szValue, LPTSTR szOutBuff, DWORD cchOutBuff);

HRESULT CfgUtilLoadFile(LPCTSTR szPath, LPSTR *pszOutput, DWORD *pdwLen);
HRESULT CfgUtilLoadTextFile(LPCTSTR szPath, BSTR *pbstrOutputData);
HRESULT CfgUtilRecoveryRollback(LPCTSTR szRecoveryFileName);

HRESULT CfgUtilFindNextEntryId(DWORD *pdwEntryId);

HRESULT CfgUtilSemanticTypeToMimeType(LPCTSTR pszSemanticType, LPCTSTR *ppszMimeType);
HRESULT CfgUtilMimeTypeToSemanticType(LPCTSTR pszMimeType, LPCTSTR *ppszSemanticType);

/// <summary>
///     Create the recovery directory.
/// </summary>
/// <returns>
///     <para>Returns HRESULT type</para>
/// </returns>
HRESULT CfgUtilCreateCfgMgrRecoveryDir();

#ifdef __cplusplus
};
#endif
