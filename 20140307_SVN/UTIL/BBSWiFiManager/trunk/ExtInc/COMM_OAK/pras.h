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

ppras.h

Abstract:

Contains the private portions of ras.h

Notes: 


--*/
    
#ifndef _PRIV_RAS_H_
#define _PRIV_RAS_H_

#ifdef __cplusplus
extern "C" {
#endif



#ifdef UNDER_CE
#define RNA_GETINFO		3

// Our main dialog has DWL_USER set to the following value.
#define RNAAPP_MAGIC_NUM 0x006A6D6D

#endif // UNDER_CE



#ifdef __cplusplus
}
#endif


#endif // _PRIV_RAS_H_
