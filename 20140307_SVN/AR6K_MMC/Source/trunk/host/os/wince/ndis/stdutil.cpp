//------------------------------------------------------------------------------
// <copyright file="stdutil.cpp" company="Atheros and Microsoft">
//    Copyright (c) 2004-2007 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
// Standard utility functions
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////////////
// Binary search support function

PVOID
bsearch(
   const void *key,
   const void *base,
   size_t      num,
   size_t      width,
   int ( __cdecl *compare ) ( const void *, const void *)
)
{
    PVOID pResult = NULL;
    DWORD dwLower, dwUpper, dwMiddle;
    int   compareResult;
    PBYTE pMiddleArrayElement;

    if (num == 0)
        return NULL;

    dwLower = 0;
    dwUpper = num - 1;

    while (dwLower <= dwUpper)
    {
        // Bisect [dwLower..dwUpper] with dwMiddle
        dwMiddle = dwLower + (dwUpper - dwLower) / 2;
        pMiddleArrayElement = (PBYTE)base + dwMiddle * width;

        compareResult = compare(key, pMiddleArrayElement);
        if (compareResult == 0)
        {
            // Match found, return it.
            pResult = pMiddleArrayElement;
            break;
        }
        else if (compareResult < 0)
        {
            // key is less than pMiddleArrayElement, so dwMiddle - 1
            // is our new upper bound
            if (dwMiddle == 0)
                break;
            dwUpper = dwMiddle - 1;
        }
        else
        {
            // key is greater than pMiddleArrayElement, so dwMiddle + 1
            // is our new lower bound
            dwLower = dwMiddle + 1;
        }
    }
    return pResult;
}
