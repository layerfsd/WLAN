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
 * Configration Host
 *
 * (c) Copyright Microsoft Corp. 2000 All Rights Reserved
 *
 *  module: cstrings.h
 *  author: scottsh
 *
 *  purpose: Header file for Non-translated constant strings
 *
 *	history: 10/30/2000  	Scott Shell (ScottSh)		Created
 *
\*---------------------------------------------------------------------------*/
#pragma once

//
// App values
//

extern TCHAR const c_szAppId[];
extern TCHAR const c_szAppPath[];
extern TCHAR const c_szAppParams[];

extern TCHAR const c_szWAPXMLHeaderVal[];
extern TCHAR const c_szWAPWBXMLHeaderVal[];


//
// Header Names
//

extern TCHAR const c_szAppIdHeader[];               // Header for Application ID
extern TCHAR const c_szResponseHeader[];	        // Header for Response Type
extern TCHAR const c_szResponseAddrHeader[];	    // Header for Response Address
extern TCHAR const c_szInitiatorHeader[];	        // Header for push initiator
extern TCHAR const c_szDescriptionHeader[];	        // Header for push description
extern TCHAR const c_szContentTypeHeader[];	        // Header for Content Type
extern TCHAR const c_szContentURIHeader[];          // Header for Content URI 
extern TCHAR const c_szContentLocationHeader[];     // Header for Content Location
