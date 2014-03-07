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

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:

autodial.h

Abstract:

Autodial function declarations.

Notes: 

--*/

#ifndef _AUTODIAL_H_
#define _AUTODIAL_H_
#include <windows.h>
#include <ras.h>

DWORD AutoDialInitializeModule(void);
DWORD AutoDialCleanupModule(void);
DWORD AutoDialStartConnection(TCHAR *szUseDialEntry);
DWORD AutoDialEndConnection(void);
DWORD AutoDialGetConnectionStatus(RASCONNSTATUS *lpRasConn, TCHAR *szRasName);
#endif   /* _AUTODIAL_H_ */
