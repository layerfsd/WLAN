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

Module Name:   e_to_k.h

Abstract:  
    This contains declarations of the corresponding EDBG routines for 
    KITL in order to support building both EDGG and KITL.
Functions:


Notes: 

--*/
#ifndef _ETOK_H_
#define _ETOK_H_

#define KITLOutputDebugString    EdbgOutputDebugString
#define OEMKitlGetSecs           OEMEthGetSecs
#define KITLDebugZone            EdbgDebugZone
#define pKITLRegisterDfltClient  pEdbgRegisterDfltClient
#define pKITLSend                pEdbgSend
#define pKITLRecv                pEdbgRecv
#define pKITLInitializeInterrupt pEdbgInitializeInterrupt

#define KITLRegisterDfltClient   EdbgRegisterDfltClient
#define KITLRegisterClient       EdbgRegisterClient
#define KITLDeregisterClient     EdbgDeregisterClient
#define KITLSend                 EdbgSend
#define KITLRecv                 EdbgRecv
#define KITLSetDebug             EdbgSetDebug

#endif _ETOK_H_

