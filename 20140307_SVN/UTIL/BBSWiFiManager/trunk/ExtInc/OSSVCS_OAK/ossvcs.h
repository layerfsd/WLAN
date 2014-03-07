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
// **************************************************************************
// OSSVCS.H
// 
// This file is used to pull in all the header files from supporting services
// Think of it like WINDOWS.H, that just includes a bunch of other headers
//
// Copyright 2000 Microsoft Corporation, All Rights Reserved
//

#pragma once

/*
    HOW TO USE THESE FLAGS:
    
    in your code, you should wrap it with:
    
    #ifdef POST_STAB
    POST_STAB       // DO NOT FORGET THIS LINE
    
    #else // POST_STAB
    #endif // POST_STAB
    
    you MUST have the POST_STAB line right after the #ifdef
    and you must have the comments on the #else and #endif lines.
*/

// This will change to something to break the build once stability is over.
#define POST_STAB    


/*
    HOW TO USE THESE FLAGS:
    
    For 04 product development, in your code, you should wrap it with:
    
    #ifdef POST03
    POST03       // DO NOT FORGET THIS LINE
    
      // 04 specific code

    #else // POST03
    #endif // POST03
    
    you MUST have the POST03 line right after the #ifdef
    and you must have the comments on the #else and #endif lines.
*/

// To build for 04 product, uncomment the next line. Do NOT check in this change. Once
// 03 and 04 fork, the ifdef's will be removed and this will be defined to something that
// breaks the build to make sure all of these are gone.


// Once M3 is over, this will change to something to break the build
#define POSTM3


#include "guts.h"
#include "miscsvcs.h"
#include "DebugMemLib.hpp"
