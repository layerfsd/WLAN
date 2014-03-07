//------------------------------------------------------------------------------
// <copyright file="dll_main.c" company="Atheros">
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
//    The software is licensed “as-is.” 
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// CF Dll main source
//
// Author(s): ="Atheros"
//==============================================================================


#include <windows.h>

BOOL WINAPI DllEntry(HINSTANCE hInstance, ULONG Reason, LPVOID pReserved)
{
    BOOL fRet;
    
    if ( Reason == DLL_PROCESS_ATTACH ) {
		DEBUGREGISTER(hInstance);
		fRet = TRUE;
    } else if ( Reason == DLL_PROCESS_DETACH ) {
		fRet = TRUE;
    }

    return fRet;
}
