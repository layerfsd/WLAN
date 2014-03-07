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
 * Push Router
 *
 * (c) Copyright Microsoft Corp. 2000 All Rights Reserved
 *
 *  module: prstrings.h
 *  author: scottsh
 *
 *  purpose: Header file for Non-translated constant strings
 *
 *	history: 10/16/2000  	Scott Shell (ScottSh)		Created
 *
\*---------------------------------------------------------------------------*/
#pragma once

//
// Registry Keys
//

// Root of the app registration registry
extern TCHAR const c_szAppRegistrationKey[];    
extern TCHAR const c_szPath[];          // Path to the registered executable
extern TCHAR const c_szParams[];        // Parameters to pass on the cmd line

//
// Header Names
//

extern TCHAR const c_szAppIdHeader[];               // Header for Application ID
extern TCHAR const c_szResponseAddrHeader[];	    // Header for Response Address
extern TCHAR const c_szResponseHeader[];	        // Header for Response Type
extern TCHAR const c_szInitiatorHeader[];	        // Header for push initiator
extern TCHAR const c_szDescriptionHeader[];	        // Header for push description
extern TCHAR const c_szContentURIHeader[];          // Header for Content URI 
extern TCHAR const c_szContentLocationHeader[];     // Header for Content Location
extern TCHAR const c_szContentTypeHeader[];         // Header for Content Type

// Header Values

extern TCHAR const c_szWAPXMLHeaderVal[];           // Header value for WAP-XML Content-Type
extern TCHAR const c_szWAPWBXMLHeaderVal[];         // Header value for WAP-WBXML Content-Type