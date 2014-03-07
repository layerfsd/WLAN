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
// Copyright (c) 2001, Microsoft Corp. All rights reserved.
#ifndef _CALLTIMERS_H_
#define _CALLTIMERS_H_

#include <calltimershook.h>

//parameter convention:
// pftCallStart && pftCallEnd must always be non NULL
// pftCallConnect == NULL IFF the call didn't connect
STDAPI CumulativeCallTimers_CalculateRecordedDuration(DWORD dwCallFlags, 
                                        const FILETIME* pftCallStart, 
                                        const FILETIME* pftCallConnect,
                                        const FILETIME* pftCallEnd,
                                        FILETIME* pftDuration);

STDAPI CumulativeCallTimers_GetLineCount(__out UINT* piLineCount);

EXTERN_C HRESULT CumulativeCallTimers_GetLastCallDuration(UINT iLine, FILETIME* pftTimerValue);

EXTERN_C HRESULT CumulativeCallTimers_ResetCurrentPeriod(UINT iLine);

//passed out as UTC
EXTERN_C HRESULT CumulativeCallTimers_GetLastResetTime(UINT iLine, FILETIME* pst); 

typedef enum _TimerPeriod {
    PhoneLifeTime,
    Current,
    Last,

    //always last
    TimerPeriodLast
} TimerPeriod;
 
typedef enum _TimerType {
 
    All,
    IncomingVoice,
    OutgoingVoice,
    IncomingData,
    OutgoingData,
    Roaming,
  
    //always last
    TimerTypeLast
} TimerType;
 
EXTERN_C HRESULT CumulativeCallTimers_GetNumCalls(TimerType eTT, TimerPeriod eTP, UINT iLine, UINT *piNumCalls);
EXTERN_C HRESULT CumulativeCallTimers_GetAccumulatedTime(TimerType eTT, TimerPeriod eTP, UINT iLine, FILETIME* pftTimerValue);


// VoIP related call timer functions
EXTERN_C HRESULT CumulativeCallTimers_GetAccumulatedVoIPTime(TimerType eTT, TimerPeriod eTP, FILETIME * pftTimerValue); 
EXTERN_C HRESULT CumulativeCallTimers_GetLastVoIPCallDuration(FILETIME * pftTimerValue); 
EXTERN_C HRESULT CumulativeCallTimers_GetVoIPNumCalls(TimerType eTT, TimerPeriod eTP, UINT *piNumCalls);
EXTERN_C HRESULT CumulativeCallTimers_ResetVoIPCurrentPeriod(); 
EXTERN_C HRESULT CumulativeCallTimers_VoIPCallNotify(const ULONGLONG ullCallStart, const ULONGLONG ullCallEnd, BOOL fOutgoing); 
EXTERN_C HRESULT CumulativeCallTimers_GetVoIPLastResetTime(FILETIME* pst); 

#endif // _CALLTIMERS_H_
