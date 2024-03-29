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

/*
Module Name:
	wininetui.h

Abstract:
	Contains manifests, macros, types and prototypes for Microsoft Windows Replacable UI for wininet
*/

#if !defined(_WININETUI_)
#define _WININETUI_

#if defined(__cplusplus)
extern "C" {
#endif

#define IDD_GOONLINE                    110
#define IDD_GOOFFLINE                   4014

#define IDS_SECURITY_CAPTION            139
#define IDS_CERT_REVOKED                140


// Replaceable UI
typedef enum tagAuthType
{
	REALM_AUTH   = 0,
	NTLM_AUTH    = 1,
	PASSPORT_AUTH   = 2
} AuthType;


typedef struct tagInvalidPassType
{

	LPSTR             lpszRealm;
	LPSTR             lpszUsername;
	LPSTR             lpszPassword;
	LPSTR             lpszHost;
	ULONG             ulMaxField;
	BOOL              fIsProxy;
	AuthType          eAuthType;
} InvalidPassType;


typedef struct
{
	DWORD       dwDlgFlags;
	DWORD       dwDlgId;
	HINTERNET   hInternetMapped;        
	LPVOID      lpVoid;
} ERRORINFODLGTYPE, *PERRORINFODLGTYPE;

DWORD IsDialogBoxHandled(	IN HWND     hWnd,
							IN DWORD    dwError,
							IN LPVOID   lpInfo);

DWORD IsMessageBoxHandled(	IN HWND     hWnd,
							IN UINT     uText, 
							IN UINT     uCaption, 
							IN UINT     uType);

#if defined(__cplusplus)
}
#endif


#endif // !defined(_WININETUI_)

