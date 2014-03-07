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

    File: StatStor.h
    
    Private interface to the status store.  Mainly the old version of the API.

    History:
        Created - KK 5-3-1

*/
#pragma once

#include "ststore.h"

HRESULT StatStoreSetInfo(UINT cids, const StatusDataID* rgids, const CEPROPVAL* pVals);
HRESULT StatStoreSetUINT(StatusDataID id, BOOL fDelete, UINT uVal);
HRESULT StatStoreSetString(StatusDataID id, BOOL fDelete, const TCHAR* pszVal);
HRESULT StatStoreGetInfo(UINT cids, const StatusDataID* rgids, CEPROPVAL** ppVals);
HRESULT StatStoreUpdateUINT(StatusDataID id, BOOL fDelete, UINT uVal, UINT uMask);
HRESULT StatStoreSetCallback(HWND hwndParent, PFN_ONSTATUSDATACHANGE pfn, LPARAM lParam);
HRESULT StatStoreRemoveCallback(HWND hwndParent);
HRESULT StatStoreCleanup();



