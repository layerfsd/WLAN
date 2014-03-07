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
/////////////////////////////////////////////////
// File: slimg.h
// Secure Loader (signed image) Framework
// Copyright 2005 Microsoft Corporation
//

#ifndef __SLIMG_H_
#define __SLIMG_H_

#ifdef __cplusplus
extern "C" {
#endif

//Image headers
const char * BINHEADER =        "B000FF\x0A";
const char * SIGNEDBINHEADER =  "S000FF\x0A";
const char * SIGNEDRAWHEADER =  "R000FF\x0A";

//Flags
#define SLIMG_FLAG_NORMAL         0x1
#define SLIMG_FLAG_END_FILE       0x2
#define SLIMG_FLAG_CHUNK_FIRST    0x4
#define SLIMG_FLAG_CHUNK          0x8

#ifdef __cplusplus
}
#endif

#endif
