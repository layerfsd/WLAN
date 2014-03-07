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

#ifndef SQMCLIENT_H__
#define SQMCLIENT_H__

#include <basetsd.h> // DWORD64

typedef DWORD HSQMSESSION;
typedef HSQMSESSION *LPHSQMSESSION;

// ----------------------------------------------------------------------------
// 
// Function:    SqmGetSession
// Description: This function creates a new session or retrieves the specified
//              named (shared) session.
//
// Parameters:
//
//     [IN] pszSessionIdentifier
//     Unique session identifier.  If this argument is NULL, then an unnamed
//     session is created.  The unnamed session can only be accessed by the
//     calling process.  To allow an external process to access a session, the
//     session must be created with an identifier.  At most one unnamed session
//     may be created by a process.  If more than one concurrent session is
//     required, then unique session identifiers must be used for all but one
//     session (unnamed).  Unless cross-process (shared) sessions are required,
//     it is recommended that unnamed sessions be used.  The first session
//     created by a process (named or unnamed) is considered the default
//     session of that process.  Passing NULL for the hSession argument of a
//     SQM function will target the default session of the calling process.  If
//     a default session does not exist, then calling a SQM function with a
//     NULL hSession argument fails silently.
//
//     [IN] cbMaxSessionSize
//     If a new session is being created, then this argument specifies the
//     maxmimum amount of memory that the session's data can consume.  Note
//     that the SQM engine is configured with a maximum session size which
//     places an upper-bound on this value.  If this argument is greater than
//     the SQM engine's configured maximum session size, then the session
//     inherits the SQM engine's maximum session size.  If a new session is not
//     being created, then this argument is ignored.
//
//     [IN] dwFlags
//     Session creation flags.  SQM_SESSION_CREATE_NEW specifies that if a
//     session is not presently associated with the specified session
//     identifier, then a new session is to be created.
//     SQM_SESSION_CREATE_NEW_PERSISTENT marks the session as persistent.  A
//     persistent session must be named.  A persistent session remains active
//     while it is not explicitly ended via SqmEndSession.  As suggested by its
//     name, a persistent session persists session data across power cycles.
//
// Return:
//
//     NULL             Failure.
//     (otherwise)      Success.  Handle (HSQMSESSION) to associated SQM
//                      session.
//
// ----------------------------------------------------------------------------
EXTERN_C
HSQMSESSION
SqmGetSession(
    LPWSTR pszSessionIdentifier,
    DWORD  cbMaxSessionSize,
    DWORD  dwFlags
    );

#define SQM_SESSION_CREATE_NEW            0x00000001
#define SQM_SESSION_CREATE_NEW_PERSISTENT 0x80000000

// ----------------------------------------------------------------------------
// 
// Function:    SqmGetManagedSession
// Description: This function retrieves a SQM handle to a managed session. A
//              session is a managed session if it is registered in the 
//              registry under the HKLM\Software\Microsoft\Sessions key and it
//              at-least has a MaxSize value.
//
// Parameters:
//
//     [IN] pszSessionIdentifier
//     Unique session identifier for the managed session. This cannot be NULL.
//
// Return:
//
//     NULL             Failure.
//     (otherwise)      Success.  Handle (HSQMSESSION) to associated SQM
//                      session.
//
// ----------------------------------------------------------------------------
EXTERN_C
HSQMSESSION
SqmGetManagedSession(
    LPWSTR pszSessionIdentifier
    );

#define SQM_REGKEY_SESSIONS L"System\\Sqm\\Sessions"
#define SQM_REGVALUE_SESSIONTIMEOUT L"Timeout"
#define SQM_REGVALUE_SESSIONCOUNT L"SessionCount"
#define SQM_REGVALUE_SESSIONPERSIST L"Persistent"
#define SQM_REGVALUE_SESSIONMAXSIZE L"MaxSize"

// ----------------------------------------------------------------------------
// 
// Function:    SqmEndSession
// Description: This function ends the target session and writes its associated
//              data to the specified file.  SQM employs a rolling session
//              model.  That is, this function does not invalidate the target
//              session (HSQMSESSION).  This function flushes the target
//              session's data and begins a new, empty session.  Moreover, all
//              subsequent SQM functions targeting the target session affect
//              the new session.  Note that a session's session flags and
//              machine and user IDs are copied to the new, empty session.
//
// Parameters:
//
//     [IN] hSession
//     The target session to end.  If this argument is NULL, then the default
//     session of the calling process is targeted.
//
//     [IN] pszPattern
//     Pattern which specifies the path of the file to which to write the
//     session's associated data.  If a fully-qualified path is specified, then
//     session's associated data is written to that file.  If a printf-style
//     format string (pattern) with a single %d specifier is specified, then
//     the session's associated data is written to a queue of files.  For
//     example, "foo%d.sqm" saves session data to foo1.sqm, foo2.sqm, foo3.sqm,
//     etc.  Files targeted by SqmEndSession must be named with a '.sqm'
//     (case-insenstive) extension.
//
//     [IN] dwMaxFilesToQueue
//     If a pattern with a %d specifier is supplied as the pszPattern argument,
//     then dwMaxFilesToQueue specifies the maximum number of files to queue
//     before overwriting existing files in the target directory that match the
//     supplied pattern.  Note that the SQM engine can be configured to enforce
//     an upper-bound on this argument.
//
//     [IN] dwFlags
//     File save flags.  SQM_OVERWRITE_OLDEST_FILE specifies that if the
//     maximum number of files have been queued, then the oldest file in the
//     target directory whose path matches the supplied pattern can be
//     overwritten.  SQM_OVERWRITE_ANY_FILE specifies that if the maximum
//     number of files have been queued, then the SQM engine can use its
//     discretion to select and overwrite any file in the target directory that
//     matches the supplied pattern.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmEndSession(
    HSQMSESSION hSession,
    LPCWSTR     pszPattern,
    DWORD       dwMaxFilesToQueue,
    DWORD       dwFlags
    );

#define SQM_OVERWRITE_ANY_FILE        0x00000001
#define SQM_OVERWRITE_OLDEST_FILE     0x00000002

// ----------------------------------------------------------------------------
// 
// Function:    SqmEndManagedSession
// Description: This function ends the target managed session, persisting it to
//              the store and uploading it. This is a special purpose function
//              that is typically used just by the SQMMGR directly, but you do
//              want a "normal" SQM session that is stored in the same place 
//              and following the same rules as other time-managed sessions,
//              then you can use SqmGetSession and SqmEndManagedSession.
//
// Parameters:
//
//     [IN] hSession
//     The target session to end.  This cannot be NULL.
//
//     [IN] dwFlags
//     0 or SQM_UPLOAD_LATER
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmEndManagedSession(
    HSQMSESSION hSession,
    DWORD       dwFlags
    );

#define SQM_REGVALUE_TRANSIENTDIR L"TransientDir"
#define SQM_REGVALUE_TRANSIENTSIGNATURE L"TransientSignature"
#define SQM_REGVALUE_MAXTRANSIENT L"MaxTransient"
#define SQM_REGVALUE_TRANSIENTFLAGS L"TransientFlags"
#define SQM_UPLOAD_LATER 0x00000001

// ----------------------------------------------------------------------------
// 
// Function:    SqmStartSession
// Description: This function sets the start time of the target session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmStartSession(
    HSQMSESSION hSession
    );

// ----------------------------------------------------------------------------
// 
// Function:    SqmGetSessionStartTime
// Description: This function returns the start time of the target session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
// Return:
//
//     0                Failure.
//     > 0              Success.  UTC time at which target session was started.
//
// ----------------------------------------------------------------------------
EXTERN_C
FILETIME
SqmGetSessionStartTime(
    HSQMSESSION hSession
    );

// ----------------------------------------------------------------------------
// 
// Function:    SqmGetEnabled
// Description: This function returns the logical AND of the target session's
//              enabled state and the SQM engine's enabled state.  If this
//              function returns FALSE, then the target session is considered
//              disabled.  As such, SQM functions targeting the target session
//              have no affect.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
// Return:
//
//     FALSE            The target session is disabled.
//     TRUE             The target session is enabled.
//
// ----------------------------------------------------------------------------
EXTERN_C
BOOL
SqmGetEnabled(
    HSQMSESSION hSession
    );

// ----------------------------------------------------------------------------
// 
// Function:    SqmSetEnabled
// Description: This function sets the target session's enabled state.  Setting
//              the target session's enabled state to TRUE does not necessarily
//              enabled the session.  See SqmGetEnabled.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN[ fEnabled
//     The value to assign to the target session's enabled state.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSetEnabled(
    HSQMSESSION hSession,
    BOOL        fEnabled
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmGetFlags
// Description: This function returns the target session's session flags.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [OUT] pdwFlags
//     The address to write the value of the target session's session flags
//     to.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmGetFlags(
    HSQMSESSION hSession,
    LPDWORD     pdwFlags
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSetFlags
// Description: This function sets the specified session flags of the target
//              session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwFlags
//     The flags to set as the target session's session flags.  SQM_FLAG_DEBUG
//     marks the target session as a debug session and specifies that its data
//     is to be discarded at the server.  SQM_FLAG_NEVER_THROTTLE specifies
//     that the target session is immune to sampling at the server
//     SQM_SECURE_UPLOAD specifies that the target session requires a secure
//     (HTTPS) transfer to the server.  SQM_DO_NOT_UPLOAD specifies that the
//     target session is to be deleted and not uploaded if targeted with
//     SqmStartUpload.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSetFlags(
    HSQMSESSION hSession,
    DWORD       dwFlags
    );

enum
{
    SQM_FLAG_DEBUG          = (1 << 0),
    SQM_FLAG_NEVER_THROTTLE = (1 << 1),
    SQM_SECURE_UPLOAD       = (1 << 2),
    SQM_DO_NOT_UPLOAD       = (1 << 3)
};

// ----------------------------------------------------------------------------
//
// Function:    SqmClearFlags
// Description: This function clears the specified session flags of the target
//              session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwFlags
//     The flags to remove from the target session's session flags.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmClearFlags(
    HSQMSESSION hSession,
    DWORD       dwFlags
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmGetMachineId
// Description: This function returns the machine ID associated with the target
//              session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [OUT] pguid
//     The address to write the machine ID associated with the target session
//     to.  0 indicates that a machine ID is not associated with the
//     target session.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmGetMachineId(
    HSQMSESSION hSession,
    LPGUID      pguid
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmGetUserId
// Description: This function returns the user ID associated with the target
//              session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [OUT] pguid
//     The address to write the user ID associated with the target session to.
//     0 indicates that a user ID is not associated with the target session.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmGetUserId(
    HSQMSESSION hSession,
    LPGUID      pguid
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSetMachineId
// Description: This function associates a machine ID with the target session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] pguid
//     The address containing the machine ID to associate with the target
//     session.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSetMachineId(
    HSQMSESSION hSession,
    LPGUID      pguid
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSetUserId
// Description: This function associates a user ID with the target session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] pguid
//     The address containing the user ID to associate with the target session.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSetUserId(
    HSQMSESSION hSession,
    LPGUID      pguid
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmCreateNewId
// Description: This function generates an ID that can be used as a session's
//              machine or user ID.
//
// Parameters:
//
//     [OUT] pguid
//     The address to write the generated ID to.
//
// Return:
//
//     FALSE            Failure.
//     TRUE             Success.
//
// ----------------------------------------------------------------------------
EXTERN_C
BOOL
SqmCreateNewId(
    LPGUID pguid
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmReadSharedMachineId
// Description: This function returns the SQM engine's shared machine ID.  The
//              SQM engine's shared machine ID is stored in the registry at
//              HKLM\System\SQM\Engine\MachineId.  Any application which uses
//              SQM may use this ID as the machine ID for its sessions.  If
//              this value does not exist in the registry, then any application
//              may create it using SqmWriteSharedMachineId.
//
// Parameters:
//
//     [OUT] pguid
//     The address to write the SQM engine's shared machine ID to.
//
// Return:
//
//     FALSE            Failure.  HKLM\System\SQM\Engine\MachineId does not
//                      exist.
//     TRUE             Success.
//
// ----------------------------------------------------------------------------
EXTERN_C
BOOL
SqmReadSharedMachineId(
    LPGUID pguid
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmReadSharedUserId
// Description: This function returns the SQM engine's shared user ID.  The
//              SQM engine's shared user ID is stored in the registry at
//              HKLM\System\SQM\Engine\UserId.  Any application which uses
//              SQM may use this ID as the user ID for its sessions.  If
//              this value does not exist in the registry, then any application
//              may create it using SqmWriteSharedUserId.
//
// Parameters:
//
//     [OUT] pguid
//     The address to write the SQM engine's shared user ID to.
//
// Return:
//
//     FALSE            Failure.  HKLM\System\SQM\Engine\UserId does not exist.
//     TRUE             Success.
//
// ----------------------------------------------------------------------------
EXTERN_C
BOOL
SqmReadSharedUserId(
    LPGUID pguid
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmWriteSharedMachineId
// Description: This function sets the SQM engine's shared machine ID.  The
//              SQM engine's shared machine ID is stored in the registry at
//              HKLM\System\SQM\Engine\MachineId.
//
// Parameters:
//
//     [IN] pguid
//     The address containing the machine ID to set as the SQM engine's shared
//     machine ID.
//
// Return:
//
//     FALSE            Failure.
//     TRUE             Success.
//
// ----------------------------------------------------------------------------
EXTERN_C
BOOL
SqmWriteSharedMachineId(
    LPGUID pguid
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmWriteSharedUserId
// Description: This function sets the SQM engine's shared user ID.  The SQM
//              engine's shared user ID is stored in the registry at
//              HKLM\System\SQM\Engine\UserId.
//
// Parameters:
//
//     [OUT] pguid
//     The address containing the user ID to set as the SQM engine's shared
//     user ID.
//
// Return:
//
//     FALSE            Failure.
//     TRUE             Success.
//
// ----------------------------------------------------------------------------
EXTERN_C
BOOL
SqmWriteSharedUserId(
    LPGUID pguid
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSet
// Description: This function sets the value of the target datapoint to the
//              specified value.  If the target datapoint is not in the target
//              session, then it is added to the target session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] dwVal
//     The value to set the value of the target datapoint to.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSet(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD       dwVal
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmIncrement
// Description: This function increments the value of the target datapoint by
//              the specified value.  If the target datapoint is not in the
//              target session, then it is added to the target session with an
//              initial value equal to that of the specified value.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] dwInc
//     The value by which to increment the value of the target datapoint.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmIncrement(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD       dwInc
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSetBits
// Description: This function sets the value of the target datapoint to the
//              logical OR of the specified value and the value of the target
//              datapoint.  If the target datapoint is not in the target
//              session, then it is added to the target session with an initial
//              value equal to that of the specified value.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] dwOrBits
//     The value to logical OR with the value of the target datapoint.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSetBits(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD       dwOrBits
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSetIfMax
// Description: This function sets the value of the target datapoint to the
//              specified value if the specified value is greater than the
//              value of the target datapoint.  If the target datapoint is not
//              in the target session, then it is added to the target session
//              with an initial value equal to that of the specified value.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] dwVal
//     The value to set the value of the target datapoint to if it is greater
//     than the value of the target datapoint.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSetIfMax(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD       dwVal
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSetIfMin
// Description: This function sets the value of the target datapoint to the
//              specified value if the specified value is less than the value
//              of the target datapoint.  If the target datapoint is not in the
//              target session, then it is added to the target session with an
//              initial value equal to that of the specified value.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] dwVal
//     The value to set the value of the target datapoint to if it is less
//     than the value of the target datapoint.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSetIfMin(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD       dwVal
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmAddToAverage
// Description: This function adds the specified value to the target datapoint
//              and increments the sample size of the target datapoint.
//              Moreover, this function marks the target datapoint as a
//              running-average datapoint.  If a target datapoint is marked as
//              a running-average datapoint, no other SQM functions should
//              target it.  Modifying the value of a datapoint marked as a
//              running-average datapoint with other SQM functions marks the
//              datapoint as a standard datapoint and resets its sample size.
//              If the target datapoint is not in the target session, then it
//              is added to the target session with an initial value equal to
//              that of the specified value.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] dwVal
//     The value to add to the value of the datapoint.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmAddToAverage(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD       dwVal
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmAddToStream
// Description: This function adds an entry to the target stream datapoint.  A
//              stream datapoint is an ordered list of DWORD n-tuples.  A
//              stream datapoint has a fixed width.  For example, a stream
//              datapoint of 3-tuples has a width of 3.  In general, a stream
//              datapoint of n-tuples has a width of n.  If the target stream
//              datapoint is not in the target session, then it is added to the
//              target session and its width is set to the value of the nArgs
//              argument and its first entry is populated with the supplied
//              arguments.  This function adds one entry to the target stream
//              datapoint.  This function fails silently if the specified
//              width does not match that of the target stream datapoint.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] nArgs
//     The width of the entry (i.e., the width of the stream datapoint).
//
//     [IN] ...
//
//     Arguments/entry.  nArgs DWORDs.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmAddToStream(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD       nArgs,
    ...
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmGet
// Description: This function returns the value of the target datapoint.  If
//              the target datapoint is not in the target session, then it is
//              added to the target session with an initial value of 0.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [OUT] pdwVal
//     The address to write the value of the target datapoint to.
//
// Return:
//
//     FALSE            Failure.  The target datapoint is not in the target
//                      session.  The target datapoint was added to the target
//                      session with an initial value of 0.
//     TRUE             Success.
//
// ----------------------------------------------------------------------------
EXTERN_C
BOOL
SqmGet(
    HSQMSESSION hSession,
    DWORD       dwId,
    LPDWORD     pdwVal
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSetAppVersion
// Description: This function sets the application version of the target
//              session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwVersionHigh
//     High 32 bits of 64-bit application version number.
//
//     [IN] dwVersionLow
//     Low 32 bits of 64-bit application version number.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSetAppVersion(
    HSQMSESSION hSession,
    DWORD       dwVersionHigh,
    DWORD       dwVersionLow
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmTimerStart
// Description: This function starts the timer associated with the target
//              datapoint.  Each datapoint in a session is associated with a
//              timer.  SqmTimerRecord, SqmTimerAccumulate, and
//              SqmTimerAddToAverage are used to stop the timer and set its
//              duration as the value of the target datapoint.  Calling this
//              function again before calling SqmTimerRecord,
//              SqmTimerAccumulate or SqmTimerAddToAverage resets the timer.
//              If the target datapoint is not in the target session, then this
//              function fails silently.  The timer measures milliseconds.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmTimerStart(
    HSQMSESSION hSession,
    DWORD       dwId
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmTimerRecord
// Description: This function ends the timer associated with the target
//              datapoint and sets its duration as the value of the target
//              datapoint.  If the target datapoint is not in the target
//              session, then this function fails silently.  The timer
//              measures milliseconds.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmTimerRecord(
    HSQMSESSION hSession,
    DWORD       dwId
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmTimerAccumulate
// Description: This function ends the timer associated with the target
//              datapoint and adds its duration to the value of the target
//              datapoint.  If the target datapoint is not in the target
//              session, then this function fails silently.  The timer
//              measures milliseconds.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmTimerAccumulate(
    HSQMSESSION hSession,
    DWORD       dwId
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmTimerAddToAverage
// Description: This function ends the timer associated with the target
//              datapoint and adds its value to the value of the target
//              datapoint and increments the sample size of the target
//              datapoint.  Moreover, this function marks the target datapoint
//              as a running-average datapoint.  If the target datapoint is not
//              in the target session, then this function fails silently.  The
//              timer measures milliseconds.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmTimerAddToAverage(
    HSQMSESSION hSession,
    DWORD       dwId
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSet64
// Description: This function sets the value of the target datapoint to the
//              specified value.  If the target datapoint is not in the target
//              session, then it is added to the target session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] qwVal
//     The value to set the value of the target datapoint to.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSet64(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD64     qwVal
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSetBool
// Description: This function sets the value of the target datapoint to the
//              specified value.  If the target datapoint is not in the target
//              session, then it is added to the target session.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] dwVal
//     The value to set the value of the target datapoint to.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------

EXTERN_C
VOID
SqmSetBool(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD       dwVal
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmAddToStreamV
// Description: This function adds an entry to the target stream datapoint.  A
//              stream datapoint is an ordered list of DWORD n-tuples.  A
//              stream datapoint has a fixed width.  For example, a stream
//              datapoint of 3-tuples has a width of 3.  In general, a stream
//              datapoint of n-tuples has a width of n.  If the target stream
//              datapoint is not in the target session, then it is added to the
//              target session and its width is set to the value of the nArgs
//              argument and its first entry is populated with the supplied
//              arguments.  This function adds one entry to the target stream
//              datapoint.  This function fails silently if the specified
//              width does not match that of the target stream datapoint.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [IN] nArgs
//     The width of the entry (i.e., the width of the stream datapoint).
//
//     [IN] argList
//
//     Arguments/entry.  nArgs DWORDs.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmAddToStreamV(
    HSQMSESSION hSession,
    DWORD       dwId,
    DWORD       nArgs,
    va_list     argList
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmGet64
// Description: This function returns the value of the target datapoint.  If
//              the target datapoint is not in the target session, then it is
//              added to the target session with an initial value of 0.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.
//
//     [IN] dwId
//     The target datapoint.
//
//     [OUT] pqwVal
//     The address to write the value of the target datapoint to.
//
// Return:
//
//     FALSE            Failure.  The target datapoint is not in the target
//                      session.  The target datapoint was added to the target
//                      session with an initial value of 0.
//     TRUE             Success.
//
// ----------------------------------------------------------------------------
EXTERN_C
BOOL
SqmGet64(
    HSQMSESSION hSession,
    DWORD       dwId,
    PDWORD64    pqwVal
    );

// Not supported.
typedef
BOOL
(WINAPI * SQMUPLOADCALLBACK)(
    HRESULT hr,
    LPCWSTR szFilePath,
    DWORD   dwHttpResponse
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmStartUpload
// Description: This function initiates the uploading of the specified file
//              or set of files.
//
// Parameters:
//
//     [IN] pszPattern
//     The file path or set of files to upload.  If this value specifies a
//     fully-qualified path, then this function initiates the uploading of that
//     particular file.  If this value specifies a set of files, then it is
//     required to conform to the pattern specification defined by
//     SqmEndSession.
//
//     [IN] szUrl
//     The target (unsecure) URL.
//
//     [IN] szSecureUrl
//     The target secure URL.
//
//     [IN] dwFlags
//     Upload flags.  SQM_UPLOAD_SINGLE_FILE specifies that only the first file
//     that matches the supplied pattern is to be uploaded to the server.
//     SQM_UPLOAD_ALL_FILES specifies that all files that match the supplied
//     pattern are to be uploaded to the server.  SQM_UPLOAD_WHEN_CONVENIENT
//     specifies that the that the files that match the supplied pattern are
//     not required to be uploaded immediately.
//
//     [IN] pfnCallback
//     Not supported.  Ignore.
//
// Return:
//
//     The number of files queued for upload.
//
// ----------------------------------------------------------------------------
EXTERN_C
DWORD
SqmStartUpload(
    LPCWSTR           szPattern,
    LPCWSTR           szUrl,
    LPCWSTR           szSecureUrl,
    DWORD             dwFlags,
    SQMUPLOADCALLBACK pfnCallback
    );

#define SQM_UPLOAD_SINGLE_FILE     0x00000001
#define SQM_UPLOAD_ALL_FILES       0x00000002
#define SQM_UPLOAD_WHEN_CONVENIENT 0x80000000

// Not supported.
EXTERN_C
VOID
SqmAbortUpload(
    DWORD dwTimeoutMilliseconds
    );

// ----------------------------------------------------------------------------
//
// Function:    SqmSaveSession
// Description: This function saves the session data of the target persisent
//              session to non-volatile storage.
//
// Parameters:
//
//     [IN] hSession
//     The target session.  If this argument is NULL, then the default session
//     of the calling process is targeted.  If the target session or the
//     default session is not a persistent session, then this function has no
//     effect.
//
// Return:
//
//     None.
//
// ----------------------------------------------------------------------------
EXTERN_C
VOID
SqmSaveSession(
    HSQMSESSION hSession
    );

#endif // SQMCLIENT_H__
