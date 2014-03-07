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
//  *************************************************************************
//
//  ccoresvcs.h
//
//  Cellcore Services API declarations
//
//  Owner: romans@microsoft.com
//
//  Copyright 1999-2000 Microsoft Corporation, All Rights Reserved.
//
//  Project: Stinger
//
//  Platform: WinCE
//
//  History: 2000-06-15 [romans] Created
//
//  *************************************************************************

#pragma once

#include <windows.h>
#include "tapi.h"
#include "guts.h"
#include "msxml.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//
// We are relying on the bonus behavior of RegQueryValueEx under WinCE: you
// can pass a relative path in the lpReserved parameter.  If this isn't
// available, we need to redefine calls to SHRegQueryValueEx to point to a
// function which does this for us, then calls the normal ReqQueryValueEx.
//
#define SHRegQueryValueEx   RegQueryValueEx

// This event is set when the shell has initialized and shell APIs
// are ready to be called.
#define SHELL_API_READY_EVENT  TEXT("SYSTEM/ShellAPIReady")

// To enable autodoc tags
//@doc

#ifndef NO_VALIDATEAPIPERMISSION
//  *************************************************************************
//  ValidateAPIPermission
//
//  Purpose:
//      Validate that the caller has permission to call a named API
//
//  Details:
//      Uses the CeGetCallerTrust OS API and the settings in HKLM\Secure\APIs
//      to ensure that the caller has permission to call a particular API.
//      See the documentation of CeGetCallerTrust for more information.
//      
//  Parameters:
//      pszAPIModule  [in]  The name of the API's module
//      pszAPIName    [in]  The name of the API
//      vtOption      [in]  The trust level to check
//                          VAP_TRUST_CURRENT - Current process
//                          VAP_TRUST_CALLER - Caller process (for drivers)
//
//  Returns:
//      HRESULT
//
//  Side Effects:
//      None
//
//  Example:
//      If pszAPIModule is "Module" and pszAPIName is "APIName",
//      HKLM\Security\APIs\Module will be checked for a DWORD value named
//      "APIName" - if no such value is present, HKLM\Security\APIs\Module
//      will be checked for a default value - if no such value is present,
//      HKLM\Security\APIs will be checked for a default value - if no such
//      value is found, E_ACCESSDENIED will be returned.  If a value was
//      found at any of these steps, it will be treated as the minimum
//      acceptable trust level of a caller of the APIName API and checked
//      against the return value from CeGetCallerTrust to verify that the
//      caller meets the minimum trust level.  If the caller is acceptable,
//      S_OK will be returned - otherwise E_ACCESSDENIED will be returned.
//  *************************************************************************
typedef enum
{
    VAP_TRUST_CURRENT=1,
    VAP_TRUST_CALLER=2,
} VAP_TRUST;
HRESULT ValidateAPIPermission(LPCWSTR pszAPIModule, LPCWSTR pszAPIName, VAP_TRUST vtOption);
#else // NO_VALIDATEAPIPERMISSION
#define ValidateAPIPermission(m, n, o) (S_OK)
// Avoid warning C4127: conditional expression is constant
#pragma warning(disable : 4127)
#endif // NO_VALIDATEAPIPERMISSION

//  *************************************************************************
//  RestoreLastRadioTransmitReceiveState
//
//  Purpose:
//      Restores the last transmit/receive state of the device's radio module.
//
//  Details:
//      If the user turns the device off with the radio in full power mode,
//      he/she turns the device back on, the shell should call this API to
//      restore that state.  Note that before calling this API, the radio
//      will be powered off for safety.
//
//  Parameters:
//      rlrtrsoOption  [in]  One of:
//                           RLRTRS_DEFAULT
//                              Default behavior
//                           RLRTRS_FORCE_RADIO_POWER_OFF
//                              For reasons of safety, ensure that the
//                              radio power remains off (e.g., user powers
//                              device on in an airplane).
//
//  Returns:
//      HRESULT
//
//  Side Effects:
//      This API will also register the radio module on the network if it is
//      possible to do so.
//  *************************************************************************
typedef enum
{
    RLRTRS_DEFAULT = 0,
    RLRTRS_FORCE_RADIO_POWER_OFF,
    RLRTRS_FORCE_RADIO_POWER_ON,
    RLRTRS_RADIO_REGISTER,
    RLRTRS_DEVICE_POWER_OFF,
    RLRTRS_DEVICE_POWER_ON,
} RLRTRS_OPTION;
HRESULT RestoreLastRadioTransmitReceiveState(RLRTRS_OPTION rlrtrsoOption);
HRESULT RestoreLastRadioTransmitReceiveStateSync(RLRTRS_OPTION rlrtrsoOption);

HRESULT CfgUtilCreateCharacteristic(IXMLDOMNode *nodeXML, LPCTSTR szNodeType, IXMLDOMElement **pNewElement);
HRESULT CfgUtilCreateParm(IXMLDOMNode *nodeXML, LPCTSTR szNodeName, LPCTSTR szNodeValue, IXMLDOMElement **pNewElement);

// *************************************************************************
// Unsupported helper functions that you shouldn't use unless you know
// exactly what they do.
//
// *************************************************************************
int wcsncpyX(__out_ecount_opt(cchMax) TCHAR *pszDst, const TCHAR *pszSrc, int cchMax);

//*** LoadLibraryRes -- load a .res file (w/o calling DllMain, etc.)
//
#if 1 // FEAT_DATALESS
#define LoadLibraryRes(p) LoadLibraryEx((p), NULL, LOAD_LIBRARY_AS_DATAFILE)
#else
#define LoadLibraryRes(p) LoadLibrary((p))
#endif

#ifdef __cplusplus
};
#endif // __cplusplus

