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
 *  module: metabase.h
 *  author: scottsh
 *
 *  purpose: Definition of Metabase implementation classes:
 *               CConfigMetabaseImpl
 *               CConfigMetabaseEntryImpl
 *
 *  history: 7/23/2000      Scott Shell (ScottSh)       Created
 *
\*---------------------------------------------------------------------------*/

#pragma once

#include <msxml.h>
#include <cfgmgr.h>

// Metabase paths.
#define METABASE_PATH           TEXT("\\ConfigMetabase")
#define METABASE_LABEL_PATH     TEXT("\\MetabaseLabels")
#define METABASE_OPTIONS_PATH   TEXT("\\MetabaseOptions")

//
// Metabase DB Fields
//
#define METABASE_DB_DATATYPE            (0x00520000 | CEVT_UI2)
#define METABASE_DB_SEMANTICTYPE        (0x01520000 | CEVT_LPWSTR)
#define METABASE_DB_MAXVALUE            (0x02520000 | CEVT_I4)
#define METABASE_DB_MINVALUE            (0x03520000 | CEVT_I4)
#define METABASE_DB_MAXLENGTH           (0x04520000 | CEVT_UI4)
#define METABASE_DB_OPTIONS             (0x05520000 | CEVT_LPWSTR)
#define METABASE_DB_LABEL               (0x06520000 | CEVT_LPWSTR)
#define METABASE_DB_MULTIPLE            (0x07520000 | CEVT_BOOL)
#define METABASE_DB_RW_ACCESS           (0x08520000 | CEVT_UI4)
#define METABASE_DB_CHARACTERISTIC_PATH (0x09520000 | CEVT_LPWSTR)
#define METABASE_DB_CHANGE_SECURITY     (0x0B520000 | CEVT_UI4)
#define METABASE_DB_ENTRYID             (0x0F520000 | CEVT_UI4)
#define METABASE_DB_ACL                 (0x10520000 | CEVT_LPWSTR)

#define MAX_METABASE_PROPS              13

// Localized DB Fields
// (These values are used for both the Label DB and the Options DB)

#define LOCDB_ENTRYID                   (0x00000000 | CEVT_UI4)
#define MAKE_LANG_PROPID(LANG)          (DWORD)(((LANG) << 16) | CEVT_LPWSTR)
#define LANGID_FROM_PROPID(PROPID)      ((LANGID)(PROPID >> 16))


#define MAX_SUBPATHS        8

#define ENTRYID_PRESENT         0x0200

// Metabase entry path separator.
#define METABASE_PATH_SEPARATOR     TEXT('\\')

// Metabase entry wildcard.
#define METABASE_WILDCARD           TEXT('*')

