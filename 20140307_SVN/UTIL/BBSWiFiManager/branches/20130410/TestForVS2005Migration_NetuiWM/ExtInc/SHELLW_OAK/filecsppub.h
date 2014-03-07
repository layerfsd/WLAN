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
/*---------------------------------------------------------------------------*\
 *
 * Public definitions for FileCSP
 *
 * (c) Copyright Microsoft Corp. 2002 All Rights Reserved
 *
 *  module: filecsppub.h
 *  author: rathraj
 *
 *  purpose: Public definitions
 *
 *  history: 6/16/2002      rathraj           Created
 *
\*---------------------------------------------------------------------------*/

#pragma once

#define FILECSP_FACILITY    0x45

#define FILECSP_E_COMSERVER_REGFAIL         MAKE_HRESULT(1, FILECSP_FACILITY, 1)
#define FILECSP_E_COMSERVER_UNREGFAIL       MAKE_HRESULT(1, FILECSP_FACILITY, 2)

