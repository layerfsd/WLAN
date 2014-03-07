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
/*++

Copyright (c) Microsoft Corporation 2005.  All rights reserved.

Module Name: 
    phnsettutil.h

Abstract: 
    Helpers for phone settings

--*/

#pragma once
#include "ril.h"

BOOL
CanShowManualNetwork(
    );

BOOL
CanShowPreferredNetwork(
    BOOL bCheckManualNetworkSetting
    );


//////////////////////////////////////////////////
// Helpers to deal with Packing the AcT parameter in with the network ID


#define RIL_OPSELTYPE_ALL (RIL_PARAM_ON_GSM_ACT | RIL_PARAM_ON_GSMCOMPACT_ACT | RIL_PARAM_ON_UMTS_ACT )

#define FORMATSTRINGACT(x) ((x & RIL_OPSELTYPE_ALL) >> 4 )
#define ACTFORMATSTRING(x) ((x) << 4)

//Max network name plus AcT parameter
#define MAX_NETWORK_NAME (MAXLENGTH_OPERATOR_LONG + 32)

#define FORMATNETWORKNAMERIL( dwActUMTSStrID, dwActGSMStrID, pszBufOut, pszNetworkName, dwParam) \
                               {ASSERT(pszNetworkName);\
                               CASSERT( ARRAYSIZE(pszBufOut) == MAX_NETWORK_NAME);\
                               CHRA(StringCchCopy( pszBufOut, MAX_NETWORK_NAME, pszNetworkName));\
                               if(dwParam & RIL_OPSELTYPE_ALL) \
                               {\
                                void *vps[1] = {pszNetworkName};\
                                VWR(FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,\
                                    (TCHAR *) LoadStringEtcOver (HINST_RESDLL, (dwParam & RIL_PARAM_ON_UMTS_ACT) ? dwActUMTSStrID : dwActGSMStrID),\
                                    0, 0, (TCHAR*)(&pszBufOut), MAX_NETWORK_NAME, (va_list*)vps));\
                               }}

//
//////////////////////////////////////////////////


