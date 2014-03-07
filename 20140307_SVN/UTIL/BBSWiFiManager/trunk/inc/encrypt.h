//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// Get/set passwords
/////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
HRESULT SetCachedCredentials(LPCTSTR pszRealm, LPCTSTR pszUsername, LPCTSTR pszPassword);
HRESULT GetCachedCredentials(LPTSTR pszRealm, LPTSTR pszUsername, DWORD cchUsername, LPTSTR pszPassword, DWORD cchPassword);
HRESULT ClearCachedCredential(LPCTSTR  pszRealm);
#ifdef __cplusplus
}
#endif

#ifdef FULL_DEBUG
HRESULT TestCredentials();
#endif	// FULL_DEBUG
