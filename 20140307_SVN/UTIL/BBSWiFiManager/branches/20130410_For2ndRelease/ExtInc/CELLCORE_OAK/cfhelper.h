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
/*************************************************************************
*
*  Copyright (C) Microsoft Corporation. All rights reserved.
*
*  MODULE : cfhelper.h
*
*  PURPOSE: Multiline Call Forwarding helper functions
*
*************************************************************************/

#ifndef __CFHELPER_H__
#define __CFHELPER_H__

#include <wtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CallForwarding_ChooseIconEnum
{
    ecfNoIcon,
    ecfIconLine1,
    ecfIconLine2,
    ecfIconLines12,
    ecfOldIcon,
    ecfLast,
} CallForwarding_ChooseIconEnum;

CallForwarding_ChooseIconEnum CallForwarding_ChooseIcon(BOOL bCFLine1, BOOL bCFLine2);

#ifdef __cplusplus
} // extern "C"
#endif


#endif //__CFHELPER_H__
