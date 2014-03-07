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
//////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999-2000 Microsoft Corporation.  All rights reserved. //
//////////////////////////////////////////////////////////////////////////


#pragma once


#ifndef _ATCI_FUNCTIONS_H_
#define _ATCI_FUNCTIONS_H_


bool InitializeCommandInterpreter(const TCHAR* const psImplementationDLLNames);
void DeinitializeCommandInterpreter();
TCHAR* InterpretString(const TCHAR* const psInterpretString);
char* InterpretStringA(const char* const psInterpretString);
WCHAR* InterpretStringW(const WCHAR* const psInterpretString);
void FreeInterpretStringResult(const TCHAR* const psInterpretStringResult);
void FreeInterpretStringResultA(const char* const psInterpretStringResult);
void FreeInterpretStringResultW(const WCHAR* const psInterpretStringResult);
bool StartHandlingInputOutput(const HANDLE hInputParam, const HANDLE hOutputParam, const HWND hwndNotifyDisconnect, const UINT uiMessage);
void StopHandlingInputOutput();


#endif // _ATCI_FUNCTIONS_H_
