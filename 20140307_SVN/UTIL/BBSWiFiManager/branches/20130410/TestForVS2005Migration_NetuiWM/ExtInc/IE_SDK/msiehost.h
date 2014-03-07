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
//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//
//  File:       msiehost.idl
//
//  Contents:   Definitions for Web Browser OC host (not automatable)
//
//----------------------------------------------------------------------------
//=--------------------------------------------------------------------------=
// msiehost.h
//=--------------------------------------------------------------------------=
// (C) Copyright 1999-2000 Microsoft Corporation.  All Rights Reserved.
//
// THIS CODE AND INFORMATION IS PROVIDED \"AS IS\" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//=--------------------------------------------------------------------------=

//---------------------------------------------------------------------------=
// Definitions for IOleCommandTarget IDs


#ifndef _MSIEHOST_H
#define _MSIEHOST_H

EXTERN_C const GUID CGID_InternetExplorer;

// CGID_InternetExplorer CMDID definitions
#define IECMDID_CLEAR_AUTOCOMPLETE_FOR_FORMS        0
#define IECMDID_SETID_AUTOCOMPLETE_FOR_FORMS        1

#define IECMDID_BEFORENAVIGATE_GETSHELLBROWSE     2
#define IECMDID_BEFORENAVIGATE_DOEXTERNALBROWSE   3
#define IECMDID_BEFORENAVIGATE_GETIDLIST          4

// Values for first parameter of IEID_CLEAR_AUTOCOMPLETE_FOR_FORMS
#define IECMDID_ARG_CLEAR_FORMS_ALL                 0
#define IECMDID_ARG_CLEAR_FORMS_ALL_BUT_PASSWORDS   1
#define IECMDID_ARG_CLEAR_FORMS_PASSWORDS_ONLY      2

#endif


