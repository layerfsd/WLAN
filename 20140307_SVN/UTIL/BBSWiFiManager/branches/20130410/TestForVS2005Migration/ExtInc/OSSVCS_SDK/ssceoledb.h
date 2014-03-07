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
//=======================================================================
// Microsoft SQL Server for Windows CE 1.0 OLE DB Provider
//
// Copyright Microsoft (c) 1999-1999 Microsoft Corporation.
//
// File: SSCEOLEDB.H
//
// File Comments: This file documents the information necessary to use the
//				  OLE DB provider for SQL Server for Windows CE.
//
//=======================================================================

#ifndef SSCEOLEDB_H
#define SSCEOLEDB_H


// ****************************************************************************
//							  OLE DB Provider CLSID                           
// ****************************************************************************

// Microsoft SQL Server for Windows CE 1.0 Provider (Microsoft.SQLSERVER.OLEDB.CE.1.0)
//
//const GUID CLSID_SQLSERVERCE_1_0 = {0x7D0703CB,0x4C23,0x11d2,{0x88,0x82,0x00,0xC0,0x4F,0xD9,0x37,0xF0}};

// Microsoft SQL Server for Windows CE 2.0 Provider (Microsoft.SQLSERVER.OLEDB.CE.2.0)
//
//const GUID CLSID_SQLSERVERCE_2_0 = {0x76A85B2E,0x9DE0,0x4ded,{0x8E,0x69,0x4D,0xEF,0xDB,0x9C,0x09,0x17}};

// Microsoft SQL Server Lite for Windows 2.0 Provider (Microsoft.SQLSERVER.LITE.OLEDB.2.0)
//
const GUID CLSID_SQLLITE_2_0 = {0x587ccdd3,0xf251,0x48fe,{0xb7,0x62,0x4f,0xad,0x6c,0xcf,0x6b,0x77}};

// Provider-Specific DBInit Property Set
// {2B9AB5BA-4F6C-4ddd-BF18-24DD4BD41848}
//
static const GUID DBPROPSET_SSCE_DBINIT = { 0x2b9ab5ba, 0x4f6c, 0x4ddd, { 0xbf, 0x18, 0x24, 0xdd, 0x4b, 0xd4, 0x18, 0x48 } };

// Provider-Specific Table Property Set
// {FA66DA6D-9173-11d3-B27B-00C04F68DBFF}
//
static const GUID DBPROPSET_SSCE_TABLE = { 0xfa66da6d, 0x9173, 0x11d3, { 0xb2, 0x7b, 0x0, 0xc0, 0x4f, 0x68, 0xdb, 0xff } };

// Provider-Specific Column Property Set
// {352CC8D5-9181-11d3-B27B-00C04F68DBFF}
//
static const GUID DBPROPSET_SSCE_COLUMN = { 0x352cc8d5, 0x9181, 0x11d3, { 0xb2, 0x7b, 0x0, 0xc0, 0x4f, 0x68, 0xdb, 0xff } };

// Provider-Specific Rowset Property Set
// {5C17C602-A107-11d3-B27B-00C04F68DBFF}
//
static const GUID DBPROPSET_SSCE_ROWSET = { 0x5c17c602, 0xa107, 0x11d3, { 0xb2, 0x7b, 0x0, 0xc0, 0x4f, 0x68, 0xdb, 0xff } };

// Provider-Specific Session Property Set
// {22FE7D33-5E5C-4a45-B723-8BED2374A06B}
//
static const GUID DBPROPSET_SSCE_SESSION = { 0x22fe7d33, 0x5e5c, 0x4a45, { 0xb7, 0x23, 0x8b, 0xed, 0x23, 0x74, 0xa0, 0x6b } };

// Provider-Specific INDEX Property Set
// {3BAA93D1-550B-4645-8B98-6E09C7203A4F}
//
static const GUID DBPROPSET_SSCE_INDEX = { 0x3baa93d1, 0x550b, 0x4645, { 0x8b, 0x98, 0x6e, 0x9, 0xc7, 0x20, 0x3a, 0x4f } };


// UNDONE(conorc) 11/2/1999 - determine how much to document in the header file
//
#define DBPROP_SSCE_TBL_DISTINCT					0x64
#define DBPROP_SSCE_COL_TTKEYASCENDING				0x65
#define DBPROP_SSCE_COL_TTKEYORDINAL				0x66
#define DBPROP_SSCE_TBL_TTTYPE						0x67
#define DBPROP_SSCE_PARTIALSETDATA					0x68
#define DBPROP_SSCE_COL_ROWGUID						0x69
#define DBPROP_SSCE_MAXBUFFERSIZE					0x70
#define DBPROP_SSCE_DBPASSWORD						0x71
#define DBPROP_SSCE_ENCRYPTDATABASE					0x72
#define DBPROP_SSCE_TEMPFILE_DIRECTORY				0x73
#define DBPROP_SSCE_LOGFILE_DIRECTORY				0x74
#define DBPROP_SSCE_TRANSACTION_COMMIT_MODE			0x75
#define DBPROP_SSCE_LOCK_TIMEOUT          			0x76
#define DBPROP_SSCE_TRACK_INDEX_CARDINALITY			0x77
#define DBPROP_SSCE_AUTO_COMPACT          			0x78

const GUID IID_ISSCECompact		= {0x35437031,0x85a0,0x11d3,{0x88,0xc4,0x00,0xc0,0x4f,0xd9,0x37,0xf0}};
class ISSCECompact : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Compact( 
        /* [in] */ ULONG cPropertySets,
        /* [size_is][out][in] */ DBPROPSET __RPC_FAR rgPropertySets[  ]) = 0;
};

// {C19F4B47-AB5E-49a9-9F2E-AB7AB5C55914}
static const GUID IID_IRowsetPosition = 
{ 0xc19f4b47, 0xab5e, 0x49a9, { 0x9f, 0x2e, 0xab, 0x7a, 0xb5, 0xc5, 0x59, 0x14 } };

class IRowsetPosition : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetRecordCount( 
        /* [in] */ HCHAPTER hChapter,
        /* [out] */ ULONG * pcRows) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetCurrentPosition( 
        /* [in] */ HCHAPTER hChapter,
        /* [out] */ ULONG * pulPosition) = 0;
};


#endif //SSCEOLEDB_H

