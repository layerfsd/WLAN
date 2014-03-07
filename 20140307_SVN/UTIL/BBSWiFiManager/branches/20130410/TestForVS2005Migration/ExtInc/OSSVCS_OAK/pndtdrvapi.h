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

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    pndtdrvapi.h

Abstract:

    This contains the API definitions that the PendantBus driver
    uses-- these are contained in a separate DLL and are a private
    interface between the driver and the API mgr


--*/

#pragma once

#define PNDTAPI_GETCMD_ORDINAL          1
#define PNDTAPI_GETAPPINFO_ORDINAL      2
#define PNDTAPI_DLL                     TEXT("pndtapi.dll")
#define PNDT_REFRESH_EVENT_NAME             TEXT("PndtRefreshEvent")

typedef HRESULT (WINAPI *PFNPNDTAPIGETCMD)(PndtAppID, PndtCmdID, PPNDTBUS_CMD_MAPPING);
typedef HRESULT (WINAPI *PFNPNDTAPIGETAPPINFO)(PndtAppID, LPTSTR szExe, LPTSTR szWin, LPTSTR szClass);


