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
#include <windows.h>
#include <wininet.h>
#include <wininetui.h>

// from ie\inet\wininet\inc\ierrui.hxx
#define DLG_FLAGS_CAN_HAVE_CERT_INFO            0x0001
#define DLG_FLAGS_HAS_CERT_INFO                 0x0002
#define DLG_FLAGS_HAS_TELL_ME_ABOUT             0x0004
#define DLG_FLAGS_HAS_DISABLED_SELECTION        0x0010
#define DLG_FLAGS_HAS_CERT_TEXT_IN_VOID         0x0020
#define DLG_FLAGS_BRING_TO_FOREGROUND           0x0040
#define DLG_FLAGS_IGNORE_CERT_CN_INVALID        INTERNET_FLAG_IGNORE_CERT_CN_INVALID
#define DLG_FLAGS_IGNORE_CERT_DATE_INVALID      INTERNET_FLAG_IGNORE_CERT_DATE_INVALID
#define DLG_FLAGS_IGNORE_REDIRECT_TO_HTTPS      INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS
#define DLG_FLAGS_IGNORE_REDIRECT_TO_HTTP       INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP
#define DLG_FLAGS_IGNORE_INVALID_CA             SECURITY_FLAG_IGNORE_UNKNOWN_CA
#define DLG_FLAGS_IGNORE_FAILED_REVOCATION      SECURITY_FLAG_IGNORE_REVOCATION
// These flags should not use the same bits as any other SECURITY_FLAGS. 
// These are for internal use only but maintained on the same DWORD 
// as the SECURITY_FLAG bits.
#define DLG_FLAGS_INVALID_CA                    0x01000000
#define DLG_FLAGS_SEC_CERT_CN_INVALID           0x02000000
#define DLG_FLAGS_SEC_CERT_DATE_INVALID         0x04000000
#define DLG_FLAGS_SEC_CERT_REV_FAILED           0x00800000
