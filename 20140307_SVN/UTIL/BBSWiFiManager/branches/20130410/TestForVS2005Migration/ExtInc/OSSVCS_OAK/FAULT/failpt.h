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
// ***************************************************************************
//  failpt.h
//
//  Owner: rajeevby@microsoft.com
//
//  Copyright 2004 Microsoft Corporation, All Rights Reserved
//
//  exposes the SOS_StackHashingControl functions. 
//  note that we cannot use classes as this code may be used by C code - hence,
//  the use of SOS_StackHashingControl__ prefix on the functions
//
// ***************************************************************************

#pragma once

// define an error code we can use to return if a failure was introduced
// and no hr was specified by the caller - for ex. CHR vs CHREx 
//
#define E_HRFAILPOINT MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, 0xBAAD)


#ifdef FAILPOINTS_ENABLED

#define FAILPOINTS_INIT()  {\
    SOS_StackHashingControl__NonCRTInit();\
    SOS_StackHashingControl__TurnOn(0,  NULL, NULL);\
    }
    
// To introduce failure point into code wrap 
// the function call which could fail into appropriate define
//
// Examples:
//
//      HANDLE_HASHFAILURE      ( CreateThread (xxxx))
//
//      NULLPTR_HASHFAILURE     ( CreateFiber (xxxx)
//
//      BOOL_HASHFAILURE        ( TerminateProcess (xxxx))
//


//  You could turn on stack hashing testing by calling
//      SOS_StackHashingControl__TurnOn (startingPoint, hashFile)
//  and off by calling
//      SOS_StackHashingControl__TurnOff ()
//



// if STACK_HASH_REALLY_ANNOYING is defined there are two more failures introduced:
//      ACCESSVIOLATION_HASHFAILURE which could be inserted into every try block and
//      TIMEOUT_HASHFAILURE which will force stack hashing to exercise timeout logic for 
//                                           wait operations
//
// Examples:
//      EX_TRYHANDLER (EX_ANY, EX_ANY, EX_ANY, ReportAndBackoutHandler)
//      {
//          ....some logic...
//          ACCESSVIOLATION_HASHFAILURE;
//      }
//      EX_CATCH
//      {
//          // Worker AVed or Asserted: kill it
//          //
//          SOS_OS::GetLogUnlocalizedRoutine () 
//                  (L"OS Error :Terminating worker due to exception in ProcessTasks\n");
//      }
//      EX_CATCH_END;
//  and 
//      result = TIMEOUT_HASHFAILURE(timeOut,
//                  WaitForSingleObjectEx (GetEvent (), timeOut, alertableWait));
//
//


//----------------------------------------------------------------------------
// Function: NULLPTR_HASHFAILURE
//
// Description:
//      add SOS stack hashing failure point for the function call which can return null pointer
//
// Return values:
//      NULL first time for the given stack, actual value otherwise
//
// Notes:
//
#define NULLPTR_HASHFAILURE(pointerExpression) \
            (SOS_StackHashingControl__GenericFailure () ? NULL : (pointerExpression))


//----------------------------------------------------------------------------
// Function: HANDLE_HASHFAILURE
//
// Description:
//      add SOS stack hashing failure point for the function call which can return NULL handle
//
// Return values:
//      NULL first time for the given stack, actual value otherwise
//
// Notes:
//      The logic is exactly same as in NULLPTR_HASHFAILURE, this define is added just
//      for naming convinience
//
#define HANDLE_HASHFAILURE(handleExpression)    \
            (SOS_StackHashingControl__GenericFailure () ? NULL: (handleExpression))



//----------------------------------------------------------------------------
// Function: BOOL_HASHFAILURE
//
// Description:
//      add SOS stack hashing failure point for the function call which can return boolean FALSE
//
// Return values:
//      
//
// Notes:
//
#define BOOL_HASHFAILURE(boolExpression)    \
            (SOS_StackHashingControl__GenericFailure () ? FALSE : (boolExpression))

// This define will add two more failure points which can generate a lot of noise and
// extra debugging efforts so they are off by default
//
#ifdef STACK_HASH_REALLY_ANNOYING

//----------------------------------------------------------------------------
// Function: ACCESSVIOLATION_HASHFAILURE
//
// Description:
//      add AV stack hashing failure point into try block
//
// Return values:
//      None. Just generates AV.
//
// Notes:
//
#define ACCESSVIOLATION_HASHFAILURE SOS_StackHashingControl__AccessViolationFailure ()

//----------------------------------------------------------------------------
// Function: TIMEOUT_HASHFAILURE
//
// Description:
//      adds stack hashing logic to simulate WAIT_TIMEOUT condition for the given stack
//
// Return values:
//      first time for the given stack - WAIT_TIMEOUT
//
// Notes:
//
#define TIMEOUT_HASHFAILURE(timeout, waitStatement) \
    ((timeout != INFINITE) && (timeout != 0) && \
    SOS_StackHashingControl__GenericFailure () ? WAIT_TIMEOUT : (waitStatement))

#else
#define ACCESSVIOLATION_HASHFAILURE ;
#define TIMEOUT_HASHFAILURE(timeout, waitStatement) waitStatement
#endif

#else // FAILPOINTS_ENABLED

#define FAILPOINTS_INIT()

// No stack hashing overhead in retail
//
//
#define NULLPTR_HASHFAILURE(pointerExpression)          pointerExpression
#define HANDLE_HASHFAILURE(handleExpression)            handleExpression
#define BOOL_HASHFAILURE(boolExpression)                boolExpression
#define ACCESSVIOLATION_HASHFAILURE ;
#define TIMEOUT_HASHFAILURE(timeout, waitStatement) waitStatement

#endif // FAILPOINTS_ENABLED


// SOS_StackHashingControl prefix is a placeholder for all stack hashing functions 
// and definitions.
// Since we are using function RtlCaptureStackBackTrace to generate stack we can't ship this
// in retail code. Please modify code to use StackWalk64 if retail version is required
//
//

typedef struct _FAILPOINTS_COUNTERS
{
    DWORD   dwNumUniqueStacks;
} FAILPOINTS_COUNTERS;


// ONLY non-CRT apps should call the NonCRTInit function
// for CRT apps, this has already been called
//
EXTERN_C BOOL     SOS_StackHashingControl__NonCRTInit ();

// Enable stack hashing testing from given starting point (# of calls to failure points since stack 
// hashing is enabled)
// If hashFile is specified we will load stack hash table from file and start appending to it
// all consequtive calls to TurnOnSOSStackHashing will not force hashFile reload
//
EXTERN_C BOOL    SOS_StackHashingControl__TurnOn (
                        IN LONG           startingPoint,
                        IN const WCHAR*   hashFile,
                        IN ULONG_PTR      taskPtr);

// Disables stack hashing 
//
EXTERN_C void    SOS_StackHashingControl__TurnOff ();

// returns the FAILPOINT counter values
//
EXTERN_C HRESULT SOS_StackHashingControl__GetCounters (
                        IN OUT FAILPOINTS_COUNTERS *pCounters);

// Return TRUE if the current stack was never hit
//      
EXTERN_C BOOL    SOS_StackHashingControl__GenericFailure ();

// Generates AV if the current stack was never hit
//
EXTERN_C void    SOS_StackHashingControl__AccessViolationFailure ();

// returns E_HRFAILPOINT if we should fail
//
EXTERN_C HRESULT  SOS_StackHashingControl__HResultFailure(
                        IN  HRESULT hrCaller
                        );
