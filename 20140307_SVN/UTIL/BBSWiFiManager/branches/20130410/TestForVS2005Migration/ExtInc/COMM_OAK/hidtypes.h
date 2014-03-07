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
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:
    hidtypes.h

Abstract:
    HID types used by HID class drivers and clients.

Functions:

Notes:

--*/

#ifndef _HIDTYPES_H_
#define _HIDTYPES_H_


typedef enum _HID_STRING_TYPE {
    HID_STRING_INDEXED = 0,
    HID_STRING_ID_IMANUFACTURER,
    HID_STRING_ID_IPRODUCT,
    HID_STRING_ID_ISERIALNUMBER,
    HID_STRING_MAX // Placeholder
} HID_STRING_TYPE;


#endif // _HIDTYPES_H_

