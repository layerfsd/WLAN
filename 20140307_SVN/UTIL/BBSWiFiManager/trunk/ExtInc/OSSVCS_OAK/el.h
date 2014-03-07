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
// Copyright (c) 2000 Microsoft Corporation.  All rights reserved.      //
//////////////////////////////////////////////////////////////////////////

#pragma once

/**
 * el.h
 *
 * Event Logging
 *
 * Use the event log to record key events that occur during runtime that
 * could be used to help track down customer support or software issues.
 *
 * SECURITY NOTE:
 * Do not put any sensitive information in the event log, for example,
 * passwords.  The event log is visible to any code running on the device.
 *
 * Event Logging Concepts
 *
 * - Event ID: Unique identifier for an event.
 *
 * - Severity: Used to manage the type of events that are logged. Adjusting
 *    the severity threshold down causes more, potentially less important
 *    events to be logged, while adjusting the threshold up restricts the 
 *    events that are logged to more severe or critical events. The severity
 *    level of an event is embedded in the high 3 bits of the event ID.
 *
 * - Module ID: A module registers with the event log using a GUID. The GUID
 *    helps scope events at a module level, and also allows module settings
 *    to be adjusted at a global level by specifying the module's GUID.
 *
 * - Counters: When a module registers with the event log, it can indicate
 *    a number of counters that the module maintains. Methods exist to
 *    increment and reset these counters. Counters are identified using a
 *    zero-based index.
 *
 * - Zones: A module can define a set of up to 32 zones as a way of logically
 *    segregating events within the module. Each zone is defined by a bit, and
 *    a mask indicates in which zones events will be logged.
 *
 * - Event parameters: There are currently 5 defined types of parameters that
 *    can be recorded as part of an event. Parameters are passed to the event
 *    log by indicating the types of arguments that are present in the
 *    variable arguments list and passing pointers to each value as the actual
 *    arguments for the call. Each parameter type is specified by a hex value
 *    0x0 thru 0xF. Concatenate up to 8 hex numbers to indicate the type and
 *    order of the parameters being passed. Parameters types are specified in
 *    reverse order with respect to the order in which they are passed in the
 *    variable arguments list.
 *
 *    Example:
 *
 *     EL_LogEvent(hEventLog, dwZone, dwEventId,
 *                 0x351, &dw32bit, pbBlob, cbBlob, szAnsiString);
 *
 * - Parameter types:
 *
 *   0x1    32-bit value    : Pass address of 32-bit value
 *   0x2    64-bit value    : Pass address of 64-bit value
 *   0x3    CHAR string     : Pass address of null-terminated Ansi string.
 *   0x4    WCHAR string    : Pass address of null-terminated Unicode string.
 *   0x5    Blob            : Pass address of memory AND
 *                                 number of bytes in blob (not a pointer).
 *
 * - Registration: A module obtains a handle to the event log by first
 *    registering with it. Pass in a GUID and the number of counters that the
 *    module plans to keep track of. Unregister to close the event log handle.
 *
 * - Flush: The event log uses shared memory to reduce the cost of logging an
 *    event. If there is a need to reboot the system, then the flush API should
 *    be called to cleanly flush any pending events in shared memory.
 *
 * - Copy: An API exists to take a snapshot of the current log. This flushes
 *    the log and stores a copy to a separate file.
 *
 * - Upload: When a desktop computer is connected to the device, a snapshot of
 *    the log can be copied to a file on the desktop computer.
 *
 * - Global settings adjustment: A handle is not required to adjust the
 *    settings for a particular module. A zone mask and severity threshold can
 *    be set or retrieved for a module if the module's GUID is known.
 *
 */

//
// Parameter types
//
#define EL_PARAM_NONE                   0x0
#define EL_PARAM_32BIT                  0x1
#define EL_PARAM_64BIT                  0x2
#define EL_PARAM_CHAR                   0x3
#define EL_PARAM_WCHAR                  0x4
#define EL_PARAM_BLOB                   0x5 // two args: data followed by length

#define EL_PARAM_MASK                   0xF

//
// Event severity levels
//
#define EL_SEVERITY_TRACE               0x0
#define EL_SEVERITY_DEBUG               0x1
#define EL_SEVERITY_INFORMATIONAL       0x2
#define EL_SEVERITY_WARNING             0x3
#define EL_SEVERITY_ERROR               0x4
#define EL_SEVERITY_CRITICAL            0x5
#define EL_SEVERITY_UNUSED1             0x6
#define EL_SEVERITY_UNUSED2             0x7

#define EL_SEVERITY_MASK                0x7
#define EL_EVENTID_MASK                 0x1FFF

#define EL_MAX_COUNTERS                 64

//
// Macro for constructing event IDs
//
#define EL_MAKE_EVENTID(id, sev)        ((((WORD)(sev)) << 13) | ((WORD)(id) & EL_EVENTID_MASK))

//
// Macro for determining the severity of an event
//
#define EL_SEVERITY_FROM_EVENTID(id)    ((((WORD)(id)) >> 13) & EL_SEVERITY_MASK)

//
// Macro for concatenating parameters
// (Although it is less code to write explicit hex values.)
//
#define EL_P(arg1, arg2)                ((arg2 << 4) | arg1)


//
// Event log client-specific functions (requires HANDLE)
//

HRESULT EL_Register(const GUID * pModuleGUID, DWORD dwNumCounters, HANDLE * phEventLog);

HRESULT EL_Unregister(HANDLE hEventLog);

HRESULT EL_IncrementCounter(HANDLE hEventLog, DWORD dwCounterIndex);

HRESULT EL_ResetCounter(HANDLE hEventLog, DWORD dwCounterIndex);

HRESULT EL_LogEvent(HANDLE hEventLog, DWORD dwZones, 
                    DWORD dwEventID, DWORD dwParams, ...);

HRESULT EL_SetZones(HANDLE hEventLog, DWORD dwZoneMask);

HRESULT EL_GetZones(HANDLE hEventLog, DWORD * pdwZoneMask);

HRESULT EL_SetSeverity(HANDLE hEventLog, DWORD dwSeverity);

HRESULT EL_GetSeverity(HANDLE hEventLog, DWORD * pdwSeverity);


//
// Global event log control functions
//

HRESULT EL_SetZonesForModule(GUID * pModuleGUID, DWORD dwZoneMask);

HRESULT EL_GetZonesForModule(GUID * pModuleGUID, DWORD * pdwZoneMask);

HRESULT EL_SetSeverityForModule(GUID * pModuleGUID, DWORD dwSeverity);

HRESULT EL_GetSeverityForModule(GUID * pModuleGUID, DWORD * pdwSeverity);

HRESULT EL_SetProcessMask(DWORD dwProcMask);

HRESULT EL_GetProcessMask(DWORD * pdwProcMask);


//
// Utility functions
//

HRESULT EL_FlushEventLogBuffer();

HRESULT EL_UploadLogSnapshot(TCHAR * szTargetFilename);

HRESULT EL_CopyLogSnapshot(TCHAR * szTargetFilename);


