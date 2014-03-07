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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

netui.h

Abstract:  

Functions:


Notes: 
    The netui library cannot be loaded before GWE has initialized (since it
    initializes common controls on DLL entry).  So, check that the WMGR API
    set has been registered before loading netui.dll.

--*/

#ifndef _CERTUI_H_
#define _CERTUI_H_
// @CESYSGEN IF CE_MODULES_NETUI

#include <windev.h>
#include <wincrypt.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _NETUI_PICKCERT
{
	int iSelected;	// [IN, OUT] the index of the selected cert
	int nCerts;		// [IN] number of certificates in ppCert array
	DWORD dwFlags;		// [IN] options
	PCCERT_CONTEXT *ppCerts	; // [IN] array of certs to display
} NETUI_PICKCERT, *PNETUI_PICKCERT;

BOOL
PickCertificate (HWND hParent, PNETUI_PICKCERT pPickCert);

BOOL
ImportCertificate(HWND hParent, HCERTSTORE hStore);

typedef
BOOL (WINAPI *PFNIMPORTCERTIFICATE)(
    IN HWND hParent,
    IN HCERTSTORE hStore
    );

BOOL
ShowCertificate(HWND hParent, PCCERT_CONTEXT pCert);

typedef
BOOL (WINAPI *PFNSHOWCERTIFICATE)(
    IN HWND hParent,
    IN PCCERT_CONTEXT pCert
    );
#ifdef __cplusplus
}
#endif
// @CESYSGEN ENDIF
#endif //_CERTUI_H_

