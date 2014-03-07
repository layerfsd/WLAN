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
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of your Windows Mobile
// Software Shared Source Premium Derivatives License Form or other
// applicable Microsoft license agreement for the software. If you did not
// accept the terms of such a license, you are not authorized to use this
// source code.
//==========================================================================*
//
// Filename:    stubs.cpp
//
// Purpose:     NetUI stub implementations
//
//==========================================================================*

#include <windows.h>
#include <wpcpriv.h>
#include <ehm.h>

extern "C"
DWORD LineTranslateDialog(HWND hParent, HWND *pDialogWnd)
{
    // This function previously created a "Dial Properties" dialog that hadn't
    // been updated in years and contained a number of potential security
    // violations. It was removed on 1 Sep 2004, but we're leaving this stub in
    // its place to maintain parity between the EDG and MDPG netui exports.
    UNREFERENCED_PARAMETER(hParent);
    UNREFERENCED_PARAMETER(pDialogWnd);

    // No one should ever call this!!
    ASSERT(FALSE);
    return (DWORD)-1;  // DialogBox failure code
}
