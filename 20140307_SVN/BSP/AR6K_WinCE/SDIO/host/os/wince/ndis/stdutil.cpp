//------------------------------------------------------------------------------
// <copyright file="stdutil.cpp" company="Atheros and Microsoft">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
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
