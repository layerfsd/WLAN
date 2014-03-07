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
//
// ordreg.h
//
// Purpose: Common API for managing a list of registry keys that must have an order associated with them.
//
// Author: larencr
//

typedef USHORT REGLISTINDEX;

#define REGLISTINVALIDINDEX  ((REGLISTINDEX)-1)

typedef HANDLE HKEYORD;

#define ROLD_NAME    0x00000001
#define ROLD_HKEY     0x00000002
#define ROLD_INDEX   0x00000004
#define ROLD_ALL  (ROLD_NAME | ROLD_HKEY | ROLD_INDEX)
typedef struct tagRegOrderedListData
{
    DWORD dwFlags;
    REGLISTINDEX iItem;
    TCHAR *pszItemName;
    HKEY hkeyItem;
} RegOrderedListData;
#define FREE_REGORDEREDLISTDATA(prol) \
    if (ROLD_NAME & prol->dwFlags) \
    { \
        VWR(NULL == LocalFree(prol->pszItemName)); \
        prol->pszItemName = NULL; \
    } \
    if ((ROLD_HKEY & prol->dwFlags) && (NULL != prol->hkeyItem)) \
    { \
        RegCloseKey(prol->hkeyItem); \
        prol->hkeyItem = NULL; \
    } \


//
// Open the ordered list.
//

// access flags
#define ROLO_READ    0x00000001
#define ROLO_WRITE  0x00000002
HRESULT RegOrderedListOpen(HKEY hkeyRoot, const TCHAR *pszSubKeyName, DWORD dwFlags, HKEYORD *phkeyOrd);

//
// Close the ordered list.  Callers must match Opens with Closes!
//
HRESULT RegOrderedListClose(HKEYORD hkeyOrd);

//
// Retrieves the number of subkeys in the ordered list.
// Return of REGLISTINVALIDINDEX means an error occurred.
//
REGLISTINDEX RegOrderedListSize(HKEYORD hkeyOrd);

//
// Retrieves an individual item by index.
//
HRESULT RegOrderedListGetItem(HKEYORD hkeyOrd, REGLISTINDEX iItem, RegOrderedListData *prol);

//
// Creates a new item subkey in the list and inserts it after the iInsert index supplied.
// Optionally returns the new subkey handle.
// If the subkey handle is returned, the caller must close it.
//
HRESULT RegOrderedListInsert(HKEYORD hkeyOrd, REGLISTINDEX iInsert, const TCHAR *pszNewItemName, HKEY *phkeyItem);

//
// Deletes an item subkey by name.
//
HRESULT RegOrderedListDelete(HKEYORD hkeyOrd, const TCHAR *pszItemName);

//
// Moves the iCurrent item up or down by dMove slots in the list (other items are shifted accordingly).
//
HRESULT RegOrderedListMove(HKEYORD hkeyOrd, REGLISTINDEX iCurrent, int dMove);

//*******************************************************************************************************
// NOTE: the following functions are NYI!!!
//*******************************************************************************************************

//
// Retrieves the index to a named list item.
// Return of REGLISTINVALIDINDEX means an error occurred or the item was not found.
//
REGLISTINDEX RegOrderedListGetIndex(HKEYORD hkeyOrd, const TCHAR *pszItemName);

//
// Retrieves an ordered list of subkeys from the registry.  The callback function is called for each subkey in order (pvData is passed to it).
// Return TRUE from the callback to continue, FALSE to stop.
//
typedef BOOL (CALLBACK *REGORDEREDLISTGETLISTCALLBACK)(RegOrderedListData *prol, void *pvData);
HRESULT RegOrderedListGetList(HKEYORD hkeyOrd, REGORDEREDLISTGETLISTCALLBACK pfnCallback, void *pvData);

//
// Deletes the old list and then adds the new one defined by the array of item names.
// The callback function is called for each new subkey, pvData is passed to it.
// Return TRUE from the callback to continue, FALSE to stop.
//
typedef BOOL (CALLBACK *REGORDEREDLISTADDLISTCALLBACK)(RegOrderedListData *prol, void *pvData);
HRESULT RegOrderedListAddList(HKEYORD hkeyOrd, const TCHAR **rgszItemNames, DWORD cItemNames, REGORDEREDLISTADDLISTCALLBACK pfnCallback, void *pvData);

//
// Deletes a list.
//
HRESULT RegOrderedListDeleteList(HKEYORD hkeyOrd);
