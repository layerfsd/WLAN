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


#ifndef _SMSSTORE_H_
#define _SMSSTORE_H_


#include <windows.h>
#include <ril.h>
#include <sms.h>
#include <smsstoreitem.h>


#ifdef __cplusplus
extern "C" {
#endif


//
// Errors and success codes
//
#define FACILITY_SMSSTORE               0x300

#define SMSSTORE_S_NEWMESSAGEREADY      MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_SMSSTORE, 0x01)
#define SMSSTORE_S_MOREMESSAGEPARTS     MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_SMSSTORE, 0x02)

#define SMSSTORE_E_NOMOREMESSAGES       MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_SMSSTORE, 0x01)
#define SMSSTORE_E_MESSAGENOTREADYET    MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_SMSSTORE, 0x02)
#define SMSSTORE_E_MESSAGEALREADYREAD   MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_SMSSTORE, 0x03)


//
//
//
HRESULT SMSStore_Open(
    LPCTSTR tszProtocolName,
    HANDLE* phStore
);

//
//
//
HRESULT SMSStore_Close(
    HANDLE hStore
);

//
//
//
HRESULT SMSStore_WriteItem(
    HANDLE hStore,
    const SMSSTOREITEMEX* pItem
);

//
//
//
HRESULT SMSStore_ReadItem(
    HANDLE hStore,
    SMSSTOREITEMEX* pItem
);

//
//
//
HRESULT SMSStore_DeleteItem(
    HANDLE hStore
);

//
//
//
HRESULT SMSStore_RestartItem(
    HANDLE hStore
);


#ifdef __cplusplus
}
#endif


#endif // _SMSSTORE_H_
