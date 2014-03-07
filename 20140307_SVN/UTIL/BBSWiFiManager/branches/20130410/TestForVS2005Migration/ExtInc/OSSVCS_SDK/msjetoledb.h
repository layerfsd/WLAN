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
// Microsoft JET OLEDB Provider
//
// Microsoft Confidential.  Copyright (c) 1995-7 Microsoft Corporation.
//
// Component: Microsoft JET Database Engine OLEDB Layer 
//
// File: MSJETOLEDB.H
//
// File Comments: This file contains the GUIDS necessary to load the 
//				  Microsoft JET OLEDB Layer for the JET 3.51 database
//				  Engine.
//
//=======================================================================

#ifndef MSJETOLEDB_H
#define MSJETOLEDB_H

// OLE DB Provider
const GUID CLSID_JETOLEDB_3_51			= {0xdee35060,0x506b,0x11cf,{0xb1,0xaa,0x00,0xaa,0x00,0xb8,0xde,0x95}};
const GUID CLSID_JETOLEDB_4_00			= {0xdee35070,0x506b,0x11cf,{0xb1,0xaa,0x00,0xaa,0x00,0xb8,0xde,0x95}};

const GUID DBPROPSET_JETOLEDB_ROWSET	= {0xa69de420,0x0025,0x11d0,{0xbc,0x9c,0x00,0xc0,0x4f,0xd7,0x05,0xc2}};
const GUID DBPROPSET_JETOLEDB_SESSION   = {0xb20f6c12,0x9b2a,0x11d0,{0x9e,0xbd,0x00,0xc0,0x4f,0xc2,0xc2,0xe0}};
const GUID DBPROPSET_JETOLEDB_DBINIT	= {0x82cf8156,0x9b40,0x11d0,{0x9e,0xbd,0x00,0xc0,0x4f,0xc2,0xc2,0xe0}};
const GUID DBPROPSET_JETOLEDB_TABLE		= {0xe64cc5fc,0x9ff2,0x11d0,{0x9e,0xbd,0x00,0xc0,0x4f,0xc2,0xc2,0xe0}};

// PROPIDs for DBPROPSET_JETOLEDB_ROWSET:
#define DBPROP_JETOLEDB_ODBCPASSTHROUGH					0xFD
#define DBPROP_JETOLEDB_ODBCPASSTHROUGHCONNECTSTRING	0xF2
#define DBPROP_JETOLEDB_BULKPARTIAL						0xEF
#define DBPROP_JETOLEDB_3_5_ENABLEIRowsetIndex			0x101	// 3.5 ONLY - enable IRowsetIndex interface for Seek

// PROPIDs for DBPROPSET_JETOLEDB_SESSION:
#define DBPROP_JETOLEDB_RECYCLELONGVALUEPAGES			0xF9

// PROPIDs for DBPROPSET_JETOLEDB_DBINIT:
#define DBPROP_JETOLEDB_REGPATH							0xFB
#define DBPROP_JETOLEDB_SYSDBPATH						0xFA	// Full Path to System Database
#define DBPROP_JETOLEDB_DATABASEPASSWORD				0x100	// Password for Database 
#define DBPROP_JETOLEDB_GLOBALBULKPARTIAL				0x109	// Database Default Partial/No Partial Behavior

// PROPIDs for DBPROPSET_JETOLEDB_TABLE:
#define DBPROP_JETOLEDB_LINK							0xF7
#define DBPROP_JETOLEDB_LINKEXCLUSIVE					0xF6
#define DBPROP_JETOLEDB_LINKDATASOURCE					0xF5
#define DBPROP_JETOLEDB_LINKPROVIDERSTRING				0xF4
#define DBPROP_JETOLEDB_LINKREMOTETABLE					0xF3
#define DBPROP_JETOLEDB_LINKCACHE_AUTHINFO				0xF0

// DBPROP_JETOLEDB_GLOBALBULKPARTIAL/DBPROP_JETOLEDB_BULKPARTIAL Enumeration Values
#define DBPROPVAL_BP_DEFAULT							0x00	// Default (only valid for DBPROP_JETOLEDB_BULKPARTIAL)
#define DBPROPVAL_BP_PARTIAL							0x01	// Use partial updates (like Access)
#define DBPROPVAL_BP_NOPARTIAL							0x02	// Use No Partial Behavior (all or nothing)

#endif //MSJETOLEDB_H