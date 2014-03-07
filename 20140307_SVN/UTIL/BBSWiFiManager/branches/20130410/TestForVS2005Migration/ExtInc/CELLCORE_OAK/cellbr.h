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

    File: cellbr.h
    
    manages the cell broadcast backing store (registry based)

    History:
        Created - ers August 21 2001
*****************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "sms.h"

//the maximum index which can be broadcasted (65536 in GSM, but we like 999)
#define CELLBR_MAX_INDEX 999

//the minimum index which can be broadcasted
#define CELLBR_MIN_INDEX 0

//the channel over which the topic index comes in
#define CELLBR_CHANNEL_TOPIC_INDEX 0

//name max length
#define CELLBR_NAME_MAX 64

typedef struct _CELLBR_ITEM {
    BOOL    fEnabled;
    DWORD   dwIndex; // within the range CELLBR_MIN_INDEX - CELLBR_MAX_INDEX inclusive
    TCHAR   szName[CELLBR_NAME_MAX];
} CELLBR_ITEM;

HRESULT GetCellBroadcastItem(/*[in]*/DWORD dwIndex, /*[out]*/CELLBR_ITEM* pcbrItem);
HRESULT SetCellBroadcastItem(/*[in]*/const CELLBR_ITEM* pcbrItem);
HRESULT DeleteCellBroadcastItem(/*[in]*/DWORD dwIndex);

//NOTE: when successful heap memory is passed out in ppcbrItemList or ppsbrRangesList, allocated using LocalAlloc()
//      and it is the caller's responsibility to free
HRESULT GetCellBroadcastList(/*[out]*/DWORD* pdwItemCount, /*[out]*/CELLBR_ITEM** ppcbrItemList);
HRESULT SetCellBroadcastList(/*[in]*/DWORD dwItemCount, /*[in]*/const CELLBR_ITEM* pcbrItemList);
HRESULT GetRadioCSCBSettings(/*[out]*/ SMS_BROADCAST_RANGES**  ppsbrRangesList);
HRESULT SetRadioCSCBSettings(/*[in]*/DWORD dwItemCount, /*[in]*/const CELLBR_ITEM* pcbrItemList, /*[in]*/ SMS_BROADCAST_RANGES*  psbrOld);

#ifdef __cplusplus
}
#endif



