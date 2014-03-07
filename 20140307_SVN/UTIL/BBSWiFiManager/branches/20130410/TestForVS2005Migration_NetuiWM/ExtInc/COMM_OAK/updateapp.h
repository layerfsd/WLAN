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

#ifndef _UPDATEAPP_H_
#define _UPDATEAPP_H_

#include <windows.h>
#include <packageinfoapi.h>

// Registry paths and registry values
#define UA_ROOT_KEY                 HKEY_LOCAL_MACHINE
#define UA_REGKEY_PATH              TEXT("System\\ImageUpdate\\UpdateApp")
#define UA_REGVALUE_INPUTFILENAME   TEXT("InputFileName")
#define UA_REGVALUE_OUTPUTFILENAME  TEXT("OutputFileName")
#define UA_REGVALUE_PERSISTTEMPNAME TEXT("PersistentTempName")
#define UA_REGVALUE_RAMTEMPDIR      TEXT("RAMTempDir")
#define UA_REGVALUE_PERSISTENTROOT  TEXT("PersistentRoot")
#define UA_REGVALUE_RETRYLIMIT      TEXT("RetryLimit")
#define UA_REGVALUE_DISPLAYDLL      TEXT("DisplayDLL")
#define UA_REGVALUE_LOGFILEPATH     TEXT("ValidatorLogPath")
#define UA_REGVALUE_UALOGFILEPATH   TEXT("UpdateAppLogPath")
#define UA_REGVALUE_FSDMOUNTTIMEOUT TEXT("FSDMountTimeout")
#define UA_REGVALUE_INITSLEEP       TEXT("InitSleep")
#define UA_REGVALUE_EXTERNALDIR     TEXT("ExternalStoreDir")
#define UA_REGVALUE_SETTINGSDIR     TEXT("UpdateSettingsDir")

#define DEFAULT_RETRY_LIMIT         0xFFFFFFFF

// File system paths
#define NK_IMAGE_ROOT               TEXT("\\RAWFS")
#define NK_IMAGE_PATH               TEXT("\\RAWFS\\VOL:")
#define NK_BACKUP_FILE              TEXT("NKBackup")
#define RES_BACKUP_FILE             TEXT("ResBackup")
#define TMP_OUTPUT_FILE             TEXT("TmpOutputFile")
#define TMP2_OUTPUT_FILE            TEXT("Tmp2OutputFile")
#define ROM_INFO_PATH               TEXT("\\IMGFS\\.rom")
#define VM_INFO_PATH                TEXT("\\IMGFS\\.vm")
#define CIF_PATH                    TEXT("\\IMGFS\\packages.cif")
#define SOF_PATH                    TEXT("\\IMGFS\\packages.sof")  
#define IMGFS_ROOT                  TEXT("\\IMGFS")
#define VALIDATOR_TEMP_DIR          TEXT("UVTemp")

#define DSM_TYPEFLAG (TYPEFLAG_FILE | TYPEFLAG_S | TYPEFLAG_H)
#define DSM_ATTRIB   (FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN)

//Flags for InputFileHdr
#define UA_INPFILEHDR_SRCDWNLDAPP  0x001         //inputfile is created by uldr's download app


typedef struct _InputFileHdr
{
    DWORD cbSize;
    DWORD dwBitmapPathSize;
    DWORD dwPackageListSize;
    DWORD dwFlags;
} InputFileHdr, *PInputFileHdr;

typedef struct _OutputFileHdr
{
    DWORD cbSize;
    HRESULT hrOverallResult;
    DWORD dwResultTableSize;
} OutputFileHdr, *POutputFileHdr;

typedef struct _PkgResult
{
    GUID guid;
    VersionInfo version;
    HRESULT hr;
    
} PkgResult, *PPkgResult;

typedef struct _HistoryResult
{
    GUID guid;
    VersionInfo version;    
} HistoryResult, *PHistoryResult;

typedef struct _HistoryResultHeader
{
    WORD year;
    WORD month;
    WORD day;
    WORD hour;
    WORD minute;
    WORD numPackages;
} HistoryResultHeader, *PHistoryResultHeader;

#endif //#define _UPDATEAPP_H_

