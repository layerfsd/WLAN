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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) 1995-1999  Microsoft Corporation

-----------------------------------------------------------------------------

@doc EXTERNAL

@module SUPSVCS.H - Supplementary Service handler |

Handles Supplementary Services for our friend the Dial Parser


-----------------------------------------------------------------------------

--*/


#ifndef _SUPSVCS_H_
#define _SUPSVCS_H_

#include <ril.h>  // MAXLENGTH_OPERATOR_NUMERIC
#include "dialact.h"


#ifdef __cplusplus
extern "C" {
#endif

// Errors
#define FACILITY_SUPSVCS                           0x100
#define SUPSVCS_E_PASSWORDERROR                    (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x01))
#define SUPSVCS_E_NETWORKERROR                     (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x02))
#define SUPSVCS_E_SIMERROR                         (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x03))
#define SUPSVCS_E_MISMATCHEDPARAMS                 (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x04))
#define SUPSVCS_E_BADCOMMAND                       (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x05))
#define SUPSVCS_E_INVALIDPARAM                     (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x06))
#define SUPSVCS_E_UNSUPPORTED                      (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x07))
#define SUPSVCS_E_FURTHERINFOREQUIRED              (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x08))
#define SUPSVCS_E_TERMINATED                       (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x09)) 
#define SUPSVCS_E_OTHERCLIENTRESPONDED             (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x0A)) 
#define SUPSVCS_E_CANCELLED                        (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x0B)) 
#define SUPSVCS_E_NODATA                           (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x0C))
#define SUPSVCS_E_INCORRECTPASSWORD                (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x0D))
#define SUPSVCS_E_PUKREQUIRED                      (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x0E))
#define SUPSVCS_E_PUK2REQUIRED                     (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x0F))

// This error is used to indicate that a USSD call has been modified by the SIM.
#define SUPSVCS_E_CALLMODIFIED                     (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SUPSVCS, 0x10))

typedef HANDLE  HSUPSVCS;

typedef struct tagSUPSVCSFWD
{
    DWORD dwEnabled;
    DWORD dwInfoClasses;
    DWORD dwDelayTime;
    TCHAR *pwszAddress;
} SUPSVCSFWD, *LPSUPSVCSFWD;

#define MAX_LEN_COUNTRY_CODE (4)

typedef struct tagSUPSVCOPERATOR
{
    DWORD dwIndex;
    TCHAR szNumName[MAXLENGTH_OPERATOR_NUMERIC];
    TCHAR *pwszName;    
    TCHAR wszCountryCode[MAX_LEN_COUNTRY_CODE];
    DWORD dwAccessType;
} SUPSVCOPERATOR, *LPSUPSVCOPERATOR;

// Access Type in string form will always be 2 characters long
#define CCH_ACCESS_TYPE                         (2)
#define FORMAT_ACCESS_TYPE                      (_T("%02u"))

#define SSMI_MAX_NUMBER_LENGTH                  (256)
#define SSMI_MAX_DESCRIPTION_LENGTH             (256)

// These values are combined in SUPSVCMODIFICATIONINFO::dwParams to indicate
// which fields of SUPSVCMODIFICATIONINFO contain valid information.
#define SSMI_PARAM_MODIFICATIONTYPE              (0x00000002)
#define SSMI_PARAM_OLDCALLTYPE                   (0x00000004)
#define SSMI_PARAM_NEWCALLTYPE                   (0x00000008)
#define SSMI_PARAM_ADDRESS                       (0x00000010)
#define SSMI_PARAM_ALPHAIDENTIFIER               (0x00000020)

// These values are used in SUPSVCMODIFICATIONINFO::dwAlphaIdentifierType to
// indicate the status of the SUPSVCMODIFICATIONINFO::aiIdentifier[].
#define SSMI_AI_UNKNOWN                          (0x00000000)
#define SSMI_AI_PRESENT                          (0x00000001)
#define SSMI_AI_NOTPRESENT                       (0x00000002)

// These values are used to indicate the old and new calltypes
// (SUPSVCMODIFICATIONINFO::dwOldCallType and SUPSVCMODIFICATIONINFO::dwNewCallType).
#define SSMI_CALLTYPE_UNKNOWN                    (0x00000000)
#define SSMI_CALLTYPE_VOICE                      (0x00000001)
#define SSMI_CALLTYPE_DATA                       (0x00000002)
#define SSMI_CALLTYPE_FAX                        (0x00000003)
#define SSMI_CALLTYPE_PTT                        (0x00000004)
#define SSMI_CALLTYPE_VT                         (0x00000005)
#define SSMI_CALLTYPE_USSD                       (0x00000006)

// These values are used in SUPSVCMODIFICATIONINFO::dwModificationType to
// indicate the type of call modification.
#define SSMI_CALLMODTYPE_UNKNOWN                 (0x00000000)
#define SSMI_CALLMODTYPE_BLOCKED                 (0x00000001)
#define SSMI_CALLMODTYPE_MODIFIED                (0x00000002)

// This type describes a USSD modification by the SIM.
typedef struct tagSUPSVCMODIFICATIONINFO
{
    DWORD dwParams;
    DWORD dwModificationType;
    DWORD dwOldCallType;
    DWORD dwNewCallType;
    
    TCHAR szNewAddres[SSMI_MAX_NUMBER_LENGTH];
 
    DWORD dwAlphaIdentifierType;
    TCHAR szAlphaIdentifierReason[SSMI_MAX_DESCRIPTION_LENGTH];

} SUPSVCMODIFICATIONINFO, *LPSUPSVCMODIFICATIONINFO;

typedef struct tagSUPSVCRESULT
{
    HRESULT hrResult;
    DWORD dwCode;
    DWORD dwEnabled;
    DWORD dwInfoClasses;
    DWORD dwInfoClassesPartial; // UNUSED -- Needed for XIP issues
    TCHAR *pwszText;
    TCHAR *pwszAddress;
    SUPSVCSFWD *rgfwd;
    DWORD dwFwdSize;
    SUPSVCOPERATOR *rgop;
    DWORD dwOperatorSize;

    // This flag is set to TRUE if and only if this result was generated
    // in response to a notification.
    BOOL  fNotification;

    // This is the SIM modification structure.  It is valid if and only if
    // hrResult == SUPSVCS_E_CALLMODIFIED, otherwise it is zeroed out.
    SUPSVCMODIFICATIONINFO ssmi;

} SUPSVCRESULT, *LPSUPSVCRESULT;

// Flags for Initialize
#define SUPSVCS_POSTMESSAGE                 0x01

// Values for Enabled
#define SUPSVCS_DISABLED                    1
#define SUPSVCS_ENABLED                     2
#define SUPSVCS_UNKNOWN                     3

// Values for InfoClasses (can be a combination of these)
#define SUPSVCS_INFOCLASS_VOICE             0x01
#define SUPSVCS_INFOCLASS_DATA              0x02
#define SUPSVCS_INFOCLASS_FAX               0x04
#define SUPSVCS_INFOCLASS_SMS               0x08      
#define SUPSVCS_INFOCLASS_DATACIRCUITSYNC   0x10      
#define SUPSVCS_INFOCLASS_DATACIRCUITASYNC  0x20     
#define SUPSVCS_INFOCLASS_PACKETACCESS      0x40     
#define SUPSVCS_INFOCLASS_PADACCESS         0x80      

HRESULT InitializeSupServices(HSUPSVCS *phSupService, HWND hWndCallBack, UINT uMsg, DWORD dwFlags);
HRESULT ExecuteSupService(HSUPSVCS hSupService, DIALACTION *pda, DWORD dwParam);
SUPSVCRESULT *ExecuteSupServiceSync(HSUPSVCS hSupService, DIALACTION *pda);
HRESULT CancelSupService(HSUPSVCS hSupService);
HRESULT DeinitializeSupServices(HSUPSVCS hSupService);
HRESULT GetSupServiceResult(HSUPSVCS hSupService, HRESULT hrCommandID, SUPSVCRESULT *pssr);
HRESULT FreeSupServiceResult(SUPSVCRESULT *pssr);
DWORD CodeToBearerService(DWORD dwCode);

#ifdef __cplusplus
}
#endif

#endif _SUPSVCS_H_

