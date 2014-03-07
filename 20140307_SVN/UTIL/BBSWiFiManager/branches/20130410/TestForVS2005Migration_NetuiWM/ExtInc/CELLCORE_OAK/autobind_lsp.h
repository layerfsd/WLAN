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


Module Name:

	AutoBind_LSP.h

Abstract:

	AutoBind LSP API

Revision History:

Notes:

--*/


// Initialize the LSP for auto binding.  This is typically called by ConnMgr.
// Once this function has been called the LSP will turn on binding
// based on PID until LSPAB_Uninitialize is called,
// or the process that called LSPAB_Initialize terminates.
HRESULT LSPAB_Initialize();

// Discontinue binding based on PID for all processes.
HRESULT LSPAB_Uninitialize();

// Set default binding to use for processes that do not have an explicit
// binding specified via LSPAB_SetProcessBinding.
HRESULT LSPAB_SetDefaultBinding(
    IN BOOL fEnableBinding, 
    IN const struct sockaddr FAR *name,
    IN INT namelen);

// Get default binding used for processes that do not have an explicit
// binding specified via LSPAB_SetProcessBinding.
// Returns E_FAIL if name is of insufficient size in which case it sets
//     pnamelen to required size.
// Returns S_OK on success.
HRESULT LSPAB_GetDefaultBinding(
	OUT BOOL *pfInUse,
	OUT struct sockaddr FAR *name,
	IN OUT INT *pnamelen);

// Set binding to be used for all sockets opened by a given process.  The
// process can still bind explicitly, but if it doesn't, the implicit
// binding will always go to the interface specified here.
HRESULT LSPAB_SetProcessBinding(
    IN BOOL fEnableBinding, 
    IN const struct sockaddr FAR *name,
    IN INT namelen,
    IN DWORD pidProcessToBind);

// Get binding used for all sockets opened by a given process.
// Returns E_FAIL if binding for the given process could not be found, or
//     name is of insufficient size in which case it sets pnamelen to required
//     size.
// Returns S_OK on success.
HRESULT LSPAB_GetProcessBinding(
	IN DWORD pidProcessBound,
	OUT BOOL *pfInUse,
    OUT struct sockaddr FAR *name,
    IN OUT INT *pnamelen);

