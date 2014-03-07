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
//***************************************************************************
// CertHashlist.h
//
// Functions for maintaining and searching lists of SHA-1/MD5 hashes.
//
// Copyright 2002 Microsoft Corporation, All Rights Reserved.
//

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// Forward declaration: see HLPriv.h for definition.
struct tagCertHashList;
typedef struct tagCertHashList* HCERTHASHLIST;

//
// Function prototypes for functions exported from ossvcs.dll
//

// Creates an empty hash list. 
HRESULT LSM_CreateHashList(HCERTHASHLIST* phHL);

// Closes a hash list that was created with LoadHashList or CreateHashList
HRESULT LSM_CloseHashList(HCERTHASHLIST hHL);

// Loads a hash list out from persistant storage, either a file or the registry.
HRESULT LSM_LoadHashList(LPCTSTR pszPath, HCERTHASHLIST* phHL);

// Saves a hash list to persistent storage, either a file or the registry.
HRESULT LSM_SaveHashList(LPCTSTR pszPath, HCERTHASHLIST hHL);

// Adds a new hash to the list (maintaining the list in sorted order).
HRESULT LSM_AddItemToHashList(HCERTHASHLIST hHL, const BYTE* pbyItem, ULONG cbyItem, BOOL* pbListModified);

// Deletes the specified item from the hash list.
HRESULT LSM_DeleteItemFromHashList(HCERTHASHLIST hHL, const BYTE* pbyItem, ULONG cbyItem);

// Searches for an item in the hash list and returns a pointer to it if found.
HRESULT LSM_FindItemInHashList(HCERTHASHLIST hHL, const BYTE* pbyItem, ULONG cbyItem, const BYTE** ppbyHashEntry);

// Returns the number of entries in the hash list
HRESULT LSM_CountHashListEntries(HCERTHASHLIST hHL, DWORD* pdwNumEntries);

// Returns a pointer to the entry specified by the 0-based index. 
HRESULT LSM_GetEntryFromHashList(HCERTHASHLIST hHL, DWORD dwIndex, const BYTE** ppbyHashEntry);

// Returns a pointer to the beginning of the hash list data and
// the number of bytes in the list. Used for transfering data to the kernel.
HRESULT LSM_GetHashListData(HCERTHASHLIST hHL, const BYTE** ppbyHLData, LPDWORD pcbyHLData);

#if defined(__cplusplus)
};
#endif // _cplusplus

/* EOF */

