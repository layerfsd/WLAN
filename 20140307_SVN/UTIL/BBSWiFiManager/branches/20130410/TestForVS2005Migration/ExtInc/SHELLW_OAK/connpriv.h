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
/******************************************************************************
Copyright (c) 1999 Microsoft Corporation. All rights reserved.

Module:
    connpriv.h

Description
    Header file shared, cross directory, among the different connection 
    elements.        

Author:
	Joan Anastasio 

Revision History:
    5/26/99  a-joana   First generation.  	

******************************************************************************/

#ifndef _CONNPRIV_H_
#define _CONNPRIV_H_

#include "tapi.h"   // For VARSTRING def

// From unimodem\tspip.h in the OS.
#define TERMINAL_NONE       0x0000
#define TERMINAL_PRE        0x0001
#define TERMINAL_POST       0x0002
#define MANUAL_DIAL         0x0004

// This structure is used to obtain the DevCfg registry value.
typedef struct DevCfgRegVal_tag {
    VARSTRING    VarString;
    DEVMINICFG   DevMiniCfg;
} DEVCFGREGVAL, *LPDEVCFGREGVAL;

#endif // ndef _CONNPRIV_H_
