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
//////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999-2000 Microsoft Corporation.  All rights reserved. //
//////////////////////////////////////////////////////////////////////////


#ifndef _SMSPRIV_H_
#define _SMSPRIV_H_


#include <windows.h>


#ifdef __cplusplus
extern "C" {
#endif


//
// APIs for SMS.dll
//
HRESULT SmsReceiveAllMessagesFromSIM (
);


#ifdef __cplusplus
}
#endif


#endif  // _SMSPRIV_H_
