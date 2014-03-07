/// <company>Microsoft Corporation</company>
/// <copyright>Copyright (c) Microsoft Corporation.  All rights reserved.</copyright>
/// <summary>
///     omadm.h
///         Provisioning APIs, constants, and utilities related to OMA-DM
/// </summary>
/// <remarks>
///     <para>
///         Use of this source code is subject to the terms of the Microsoft
///         premium shared source license agreement under which you licensed
///         this source code. If you did not accept the terms of the license
///         agreement, you are not authorized to use this source code.
///         For the terms of the license, please see the license agreement
///         signed by you and Microsoft.
///         THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
///     </para>
/// </remarks>
/// <history>
///     <record date="10/28/2005">Created</record>
/// </history>

#pragma once

#include <cfgdata.h>


// *******************************************************************************************
//  Constants
// *******************************************************************************************

// Enumeration specifying options for UI during an OMA-DM session
typedef enum OMADM_UIMODE
{ 
    OMADM_UIMODE_NONE                           = 0,
    OMADM_UIMODE_DEFAULT                        = 1,
    OMADM_UIMODE_BACKGROUND                     = 2,
    OMADM_UIMODE_INFORMATIVE                    = 3,
    OMADM_UIMODE_USER_INTERACTIVE               = 4,
} OMADM_UIMODE;

// Enumeration specifying various OMA-defined client event types
// Each enumeration has a value matching its definition in OMA.
typedef enum OMADM_EVENT_TYPE 
{
    OMADM_EVENT_TYPE_UNKNOWN                    = 0,
    OMADM_EVENT_TYPE_DEVICE_EVENT               = 1224,
    OMADM_EVENT_TYPE_GENERIC                    = 1226
} OMADM_EVENT_TYPE;

// Enumeration specifying the state of the current OMA-DM session
typedef enum OMADM_SESSION_STATE 
{
    OMADM_SESSION_STATE_UNKNOWN                 = 0,
    OMADM_SESSION_STATE_READY                   = 1,
    OMADM_SESSION_STATE_CONNECTING              = 2,
    OMADM_SESSION_STATE_COMMUNICATING           = 3,
    OMADM_SESSION_STATE_ALERT_STATUS_AVAILABLE  = 4,
    OMADM_SESSION_STATE_COMPLETED               = 5
} OMADM_SESSION_STATE;

// Enumeration specifying the Mark to be sent to the server
typedef enum OMADM_MARK 
{
    OMADM_MARK_UNKNOWN                          = 0,
    OMADM_MARK_FATAL                            = 1,
    OMADM_MARK_CRITICAL                         = 2,
    OMADM_MARK_MINOR                            = 3,
    OMADM_MARK_WARNING                          = 4,
    OMADM_MARK_INFORMATIONAL                    = 5,
    OMADM_MARK_HARMLESS                         = 6,
    OMADM_MARK_INDETERMINATE                    = 7,
} OMADM_MARK;

// Structure specifying various parameters for an OMA-DM Alert sent
// from the client to the server in a client-initiated session
// cbSize must be set to sizeof(OMADMALERTINFO)
typedef struct OMADMALERTINFO
{
    DWORD               cbSize;
    OMADM_EVENT_TYPE    eventtype;
    LPCTSTR             pszAlertType;
    LPCTSTR             pszAlertSource;
    LPCTSTR             pszAlertData;
    CFG_DATATYPE        dataformat;
    OMADM_MARK          mark;
    LPCTSTR             pszCorrelator;
    DWORD               dwStatusCode;
    LPCTSTR             pszAlertTarget;
} OMADMALERTINFO, *POMADMALERTINFO;

// cbSize must be set to sizeof(OMADMINITIATIONINFO)
typedef struct OMADMINITIATIONINFO
{
    DWORD               cbSize;
    LPCTSTR             pszInitiatorID;
    OMADM_UIMODE        uimode;
    OMADM_SESSION_STATE sessionstate;
    OMADMALERTINFO*     rgaiAlerts;
    DWORD               caiAlerts;
} OMADMINITIATIONINFO, *POMADMINITIATIONINFO;


// *******************************************************************************************
//  APIs
// *******************************************************************************************

/// <summary>
///     Free members of the OMADMINITIATIONINFO structure.
/// </summary>
/// <param name="piiInitInfo">[in] Initiation structure</param>
/// <returns>
///     <para>Returns void type</para>
/// </returns>
void OmaDmFreeInitiationInfo(__in POMADMINITIATIONINFO piiInitInfo);


/// <summary>
///     Get information for an OMA-DM session initiation.
/// </summary>
/// <param name="pszInitiationID">[in] ID of initiated session (for session notification listeners)</param>
/// <param name="piiInitInfo">[out] Initiation information</param>
/// <returns>
///     <para>Returns HRESULT type</para>
/// </returns>
/// <remarks>
///     <para>
///         The returned OMADMINITATIONINFO parameter must be freed via OmaDmFreeInitiationInfo.
///     </para>
/// </remarks>
HRESULT OmaDmGetInitiationInfo(
    __in    LPCTSTR                 pszInitiationID,
    __out   POMADMINITIATIONINFO    piiInitInfo);


/// <summary>
///     Enumerate all the initiation ID's under a server ID.
/// </summary>
/// <param name="pszServerID">[in] Server ID to look under</param>
/// <param name="prgpszInitiationIDs">[out] Pointer to an array of initiation IDs</param>
/// <param name="pcInitiationIDs">[out] Number of initiation IDs enumerated</param>
/// <returns>
///     <para>Returns HRESULT type</para>
/// </returns>
/// <remarks>
///     <para>
///         Caller needs call LocalFree for each of the pointers in the returned
///         array and for the array itself.
///     </para>
/// </remarks>
HRESULT OmaDmEnumerateInitiationInfo(
    __in                            LPCTSTR     pszServerID,
    __out_ecount(*pcInitiationIDs)  LPTSTR**    prgpszInitiationIDs, 
    __out                           DWORD*      pcInitiationIDs);

    
/// <summary>
///     Initiate an OMA-DM session by sending a trigger to the OMA-DM client.
/// </summary>
/// <param name="pszServerID">[in] Server ID of DM account whose credentials are to be used</param>
/// <param name="eUIMode">[in] User interaction mode</param>
/// <param name="rgaiAlerts">[in] Alerts to send to the server</param>
/// <param name="caiAlerts">[in] Number of elements in the rgaiAlerts array</param>
/// <param name="ppszInitiationID">[out] ID of initiated session (for session notification listeners)</param>
/// <returns>
///     <para>Returns HRESULT type</para>
/// </returns>
/// <remarks>
///     <para>
///         Callers must call LocalFree on the returned value of ppszInitiationID.
///     </para>
/// </remarks>
HRESULT OmaDmInitiateSession(
    __in                        LPCTSTR                 pszServerID, 
                                const OMADM_UIMODE      uimode, 
    __in_ecount_opt(caiAlerts)  const OMADMALERTINFO*   rgaiAlerts,
                                const DWORD             caiAlerts,
    __deref_out_opt             LPTSTR*                 ppszInitiationID);


/// <summary>
///     Remove saved state for a particular client-initiated session.
/// </summary>
/// <param name="pszInitiationID">[in] ID of initiated session (for session notification listeners)</param>
/// <returns>
///     <para>Returns HRESULT type</para>
/// </returns>
HRESULT OmaDmCloseSession(__in_opt LPCTSTR pszInitiationID);


