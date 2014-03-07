//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#pragma once

#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

#include <phcanvas.h>
#include "rtcclient.h"

#include "CommonFunctions.hpp"
#include <string.hxx>

//constant to be prepended to all the messages
#define MODULE_NAME L"[VoIPPhoneCanvas]"

#ifdef DEBUG
    //the debug zones
    #define ZONE_VOIP_ERROR     DEBUGZONE(0) //an error has occurred
    #define ZONE_VOIP_CTOR      DEBUGZONE(1) //constructor/destructor/initialization calls (tracing)
    #define ZONE_VOIP_CALLERID  DEBUGZONE(2) //caller id functions + info (tracing)
    #define ZONE_VOIP_EVENT     DEBUGZONE(3) //rtc/canvas events
    #define ZONE_VOIP_FUNCTION  DEBUGZONE(4) //user functionality (e.g. hold press)
    #define ZONE_VOIP_CANVAS    DEBUGZONE(5) //interaction with the canvas apis
    #define ZONE_VOIP_VERB      DEBUGZONE(6) //verb availability
    #define ZONE_VOIP_PHSTATE   DEBUGZONE(7) //dump out the PHPHONESTATE structure
    #define ZONE_VOIP_PHCALL    DEBUGZONE(8) //dump out the PHCALLINFO structures
    #define ZONE_VOIP_REFCOUNT  DEBUGZONE(9) //track reference management
    #define ZONE_VOIP_DIALPLAN  DEBUGZONE(10)//verbosity for DialPlan parser
    #define ZONE_VOIP_RULES     DEBUGZONE(11)//dump out dial plan rules

    //internal debugger object
    class DebugTracer_t
    {
    public:
        DebugTracer_t(
            int zone, 
            __in const WCHAR *ZoneName, 
            __in const char *FnName
            )
        {
            strncpy(m_FnName, FnName, ARRAYSIZE(m_FnName));
            NULL_TERMINATE_ARRAY(m_FnName);

            wcsncpy(m_ZoneName, ZoneName, ARRAYSIZE(m_ZoneName));
            NULL_TERMINATE_ARRAY(m_ZoneName);
            
            m_Zone = zone;
            DEBUGMSG(m_Zone, (L"%s: %s tracing+++ %S Entering\r\n", MODULE_NAME, m_ZoneName, m_FnName));
        }

        ~DebugTracer_t()
        {
            DEBUGMSG(m_Zone, (L"%s: %s tracing--- %S Exiting\r\n", MODULE_NAME, m_ZoneName, m_FnName));
        }

    private:
        char  m_FnName[100];
        WCHAR m_ZoneName[50];
        int   m_Zone;
    };

    //overloaded debugmsg that spits out module name + debug zone
    static WCHAR *__s_dbg_wszZone   = NULL;
    static CHAR  *__s_dbg_szFn      = NULL;

    //tracing macro
    #define TRACE(zone) DebugTracer_t __tracer(zone, L#zone, __FUNCTION__)

    //overloaded debugmsg
    void __voip_debugset(__in const WCHAR*, __in const CHAR*);
    void __voip_debugmsg(__in const WCHAR *c_wszFmt, ...);
    #define VOIP_DEBUGMSG(zone, out) if (zone) { __voip_debugset(L#zone, __FUNCTION__); (__voip_debugmsg out); }
    #define VOIP_RETAILMSG(zone, out) VOIP_DEBUGMSG(zone, out)

#else //! defined DEBUG
    //do nothing in retail...
    #define ZONE_VOIP_ERROR     
    #define ZONE_VOIP_CTOR      
    #define ZONE_VOIP_CALLERID  
    #define ZONE_VOIP_EVENT     
    #define ZONE_VOIP_FUNCTION  
    #define ZONE_VOIP_CANVAS    
    #define ZONE_VOIP_VERB      
    #define ZONE_VOIP_PHSTATE   
    #define ZONE_VOIP_PHCALL    
    #define ZONE_VOIP_REFCOUNT  
    #define ZONE_VOIP_DIALPLAN
    #define ZONE_VOIP_RULES

    #define TRACE(zone)
    #define VOIP_DEBUGMSG(zone, out) 
    void __voip_retailmsg(__in const WCHAR *c_wszFmt, ...);
    #define VOIP_RETAILMSG(zone, out) (__voip_retailmsg out)

#endif //defined DEBUG

#define TRACE_FAILEDHR() VOIP_DEBUGMSG(FAILED(hr) && ZONE_VOIP_ERROR, (L"Routine failed with HRESULT = 0x%08x\r\n", hr))

//DebugMsg helper macro
#define TO_STRING(s) case(s): return L#s;
#define DEFAULT_TO_STRING() default: return L"<unknown>";

//DebugMsg helper fn
inline const WCHAR * VerbToString(PH_VERB verb)
{
#ifdef DEBUG    
    switch (verb)
    {
    TO_STRING(PH_VERB_TALK)
    TO_STRING(PH_VERB_END)
    TO_STRING(PH_VERB_CHLD1)
    TO_STRING(PH_VERB_HOLD)
    TO_STRING(PH_VERB_UNHOLD)
    TO_STRING(PH_VERB_SWAP)
    TO_STRING(PH_VERB_PRIVATE)
    TO_STRING(PH_VERB_CONFERENCE)
    TO_STRING(PH_VERB_MUTE)
    TO_STRING(PH_VERB_UNMUTE)
    TO_STRING(PH_VERB_FLASH)
    TO_STRING(PH_VERB_REJECT_INCOMING)
    TO_STRING(PH_VERB_ACCEPT_INCOMING)
    TO_STRING(PH_VERB_SPEAKERPHONEON)
    TO_STRING(PH_VERB_SPEAKERPHONEOFF)
    TO_STRING(PH_VERB_BTH_HF_ON)
    TO_STRING(PH_VERB_BTH_HF_OFF)
    TO_STRING(PH_VERB_NEWCONTACT)
    TO_STRING(PH_VERB_VIEWCONTACT)
    TO_STRING(PH_VERB_SIMPLE_TRANSFER)
    TO_STRING(PH_VERB_ATTENDED_TRANSFER)
    TO_STRING(PH_VERB_COMPLETE_TRANSFER)
    TO_STRING(PH_VERB_CANCEL_TRANSFER)
    TO_STRING(PH_VERB_LAST_DYNAMIC_VERB)
    TO_STRING(PH_VERB_NEWSMS) 
    TO_STRING(PH_VERB_NEWNOTE)
    TO_STRING(PH_VERB_LAUNCHOPTIONS)
    TO_STRING(PH_VERB_LAUNCHCONTACTS)
    TO_STRING(PH_VERB_LAUNCHCALENDAR)
    TO_STRING(PH_VERB_DIALVMAIL)
    TO_STRING(PH_VERB_DIALSPEEDDIAL)
    TO_STRING(PH_VERB_MINIMIZE)
    TO_STRING(PH_VERB_SWITCHVIEW)
    TO_STRING(PH_VERB_UPDATE_INACTIVITY_TIMER)
    TO_STRING(PH_VERB_ADD_ACCUMULATOR)
    TO_STRING(PH_VERB_REMOVE_ACCUMULATOR)
    TO_STRING(PH_VERB_LAST_STATIC_VERB)
    DEFAULT_TO_STRING();
    }
#else
    return NULL;
#endif
}

//DebugMsg helper function
inline const WCHAR *StateToString(PH_GROUPSTATE gs)
{
#ifdef DEBUG
    
    switch (gs)
    {
    TO_STRING(PH_GS_IDLE)
    TO_STRING(PH_GS_CALLING)
    TO_STRING(PH_GS_RINGING)
    TO_STRING(PH_GS_BUSY)
    TO_STRING(PH_GS_TALKING)
    TO_STRING(PH_GS_HOLD)
    TO_STRING(PH_GS_ENDED)
    TO_STRING(PH_GS_CONGESTION)
    TO_STRING(PH_GS_DROPPEDCALL)
    TO_STRING(PH_GS_TRANSFERRING)
    TO_STRING(PH_GS_WAIT_FOR_SIMPLE_TRANSFER)
    TO_STRING(PH_GS_WAIT_FOR_ATTENDED_TRANSFER)
    DEFAULT_TO_STRING();
    }
#else
        return NULL;
#endif
}

inline const WCHAR *CallerIDStateToString(PH_CALLERIDSTATE state)
{
#ifdef DEBUG
    
    switch (state)
    {
    TO_STRING(PH_CID_UNAVAILABLE)
    TO_STRING(PH_CID_BLOCKED)
    TO_STRING(PH_CID_VALID)
    TO_STRING(PH_CID_AMBIGUOUS)
    DEFAULT_TO_STRING();
    }
#else
        return NULL;
#endif
}

inline const WCHAR *CallStateToString(PH_CALLSTATE cs)
{
#ifdef DEBUG
    
    switch (cs)
    {
    TO_STRING(PH_CS_UNKNOWN)
    TO_STRING(PH_CS_AVAILABLE)
    TO_STRING(PH_CS_CALLING)
    TO_STRING(PH_CS_TALKING)
    TO_STRING(PH_CS_HOLD)
    TO_STRING(PH_CS_IDLE)
    TO_STRING(PH_CS_IDLE_BUSY)
    TO_STRING(PH_CS_IDLE_NOANSWER)
    TO_STRING(PH_CS_IDLE_NODIALTONE)
    TO_STRING(PH_CS_IDLE_UNREACHABLE)
    TO_STRING(PH_CS_IDLE_BADADDRESS)
    TO_STRING(PH_CS_IDLE_CANCELLED)
    TO_STRING(PH_CS_IDLE_PHONECONNECTIONFAILURE)
    TO_STRING(PH_CS_IDLE_INVALIDSIMCARD)
    TO_STRING(PH_CS_IDLE_SIMCARDBUSY)
    TO_STRING(PH_CS_IDLE_NETWORKSERVICENOTAVAILABLE)
    TO_STRING(PH_CS_IDLE_EMERGENCYONLY)
    TO_STRING(PH_CS_IDLE_ERROR)
    TO_STRING(PH_CS_IN_CONFERENCE)
    TO_STRING(PH_CS_IDLE_CONGESTION)
    TO_STRING(PH_CS_IDLE_TEMPFAILURE)
    TO_STRING(PH_CS_TRANSFERRING)
    TO_STRING(PH_CS_IDLE_TRANSFERCOMPLETE)
    DEFAULT_TO_STRING();
    }
#else
        return NULL;
#endif
}

#define TO_STRING_APPEND(prop) \
    case (prop): OutString.append(L#prop); OutString.append(L" "); break;
    
inline void CallInfoFlagsToString(DWORD Flags, ce::wstring& OutString)
{
#ifdef DEBUG
    
    switch (Flags & PH_CIF_FINDSTATE_MASK)
    {
        TO_STRING_APPEND(PH_CIF_FINDNOTSTARTED)
        TO_STRING_APPEND(PH_CIF_FINDSTARTED)
        TO_STRING_APPEND(PH_CIF_FINDSUCCEEDED)
        TO_STRING_APPEND(PH_CIF_FINDFAILED)
        ;
    }

    switch (Flags & PH_CIF_DIRECTION_MASK)
    {
        TO_STRING_APPEND(PH_CIF_INCOMING)
        TO_STRING_APPEND(PH_CIF_OUTGOING)
        ;
    }

    switch (Flags & PH_CIF_CLOG_WRITESTATE_MASK)
    {
        TO_STRING_APPEND(PH_CIF_CLOG_WRITENOTSTARTED)
        TO_STRING_APPEND(PH_CIF_CLOG_WRITESTARTED)
        TO_STRING_APPEND(PH_CIF_CLOG_WRITEPENDING)
        TO_STRING_APPEND(PH_CIF_CLOG_WRITESUCCEEDED)
        TO_STRING_APPEND(PH_CIF_CLOG_WRITEFAILED)
        ;
    }

    switch (Flags & PH_CIF_CALLNETWORK_MASK)
    {
        TO_STRING_APPEND(PH_CIF_CALLNETWORK_VOIP)
        TO_STRING_APPEND(PH_CIF_CALLNETWORK_CELLULAR)
        ; 
    }
#endif
}

inline void CallBehaviorFlagsToString(DWORD Flags, ce::wstring& OutString)
{
#ifdef DEBUG
    
    switch (Flags & PH_CBF_LOGENTRYMASK)
    {
        TO_STRING_APPEND(PH_CBF_LOGENTRY)
        TO_STRING_APPEND(PH_CBF_NOLOGENTRY)
    }

    switch (Flags & PH_CBF_CALLERIDMASK)
    {
        TO_STRING_APPEND(PH_CBF_CALLERIDLOOKUP)
        TO_STRING_APPEND(PH_CBF_NOCALLERIDLOOKUP)
    }

    switch (Flags & PH_CBF_PROGRESSONCALLMASK)
    {
        TO_STRING_APPEND(PH_CBF_CALLPROGRESSONCALL)
        TO_STRING_APPEND(PH_CBF_HIDEPROGRESSONCALL)
        ;  
    }

    switch (Flags & PH_CBF_SHOWDIALERMASK)
    {
        TO_STRING_APPEND(PH_CBF_SHOWDIALERONCALL)
        ;
    }
#endif
}

inline void CallLogFlagsToString(DWORD Flags, ce::wstring& OutString)
{
#ifdef DEBUG
    
    switch (Flags & PH_CLF_DDIRECTIONMASK)
    {
        TO_STRING_APPEND(PH_CLF_DINCOMING)
        TO_STRING_APPEND(PH_CLF_DOUTGOING)
        ;
    }

    switch (Flags & PH_CLF_MMISSEDMASK)
    {
        TO_STRING_APPEND(PH_CLF_MMISSED)
        TO_STRING_APPEND(PH_CLF_MNOTMISSED)
        ;
    }
    
    switch (Flags & PH_CLF_EENDEDMASK)
    {
        TO_STRING_APPEND(PH_CLF_ENOEND)
        TO_STRING_APPEND(PH_CLF_EENDED)
        ;
    }
    
    switch (Flags & PH_CLF_VVOICEMASK)
    {
        TO_STRING_APPEND(PH_CLF_VVOICE)
        TO_STRING_APPEND(PH_CLF_VDATA)
        ;
    }

    switch (Flags & PH_CLF_RROAMMASK)
    {
        TO_STRING_APPEND(PH_CLF_RLOCAL)
        TO_STRING_APPEND(PH_CLF_RROAM)
        ;
    }

    switch (Flags & PH_CLF_LLINEMASK)
    {
        TO_STRING_APPEND(PH_CLF_LLINE0)
        TO_STRING_APPEND(PH_CLF_LLINE1)
        TO_STRING_APPEND(PH_CLF_LLINE2)
        TO_STRING_APPEND(PH_CLF_LLINEOTHER)
        ;
    }

    switch (Flags & PH_CLF_CICALLERIDMASK)
    {
        TO_STRING_APPEND(PH_CLF_CIUNAVAILABLE)
        TO_STRING_APPEND(PH_CLF_CIBLOCKED)
        TO_STRING_APPEND(PH_CLF_CIAVAILABLE)
        ;
    }

    switch (Flags & PH_CLF_UENDEDTIMEMASK)
    {
        TO_STRING_APPEND(PH_CLF_UHAVEENDEDTIME)
        TO_STRING_APPEND(PH_CLF_UNOENDEDTIEM)
        ;
    }

    switch (Flags & PH_CLF_ICUSTOMICONMASK)
    {
        TO_STRING_APPEND(PH_CLF_INOCUSTOMICON)
        TO_STRING_APPEND(PH_CLF_ICUSTOMICON)
        ;
    }
#endif
}

inline const WCHAR *RTCSessionStateToString(RTC_SESSION_STATE rtcss)
{
#ifdef DEBUG
    
    switch (rtcss)
    {
    TO_STRING(RTCSS_IDLE)
    TO_STRING(RTCSS_INCOMING)
    TO_STRING(RTCSS_ANSWERING)
    TO_STRING(RTCSS_INPROGRESS)
    TO_STRING(RTCSS_CONNECTED)
    TO_STRING(RTCSS_DISCONNECTED)
    TO_STRING(RTCSS_HOLD)
    TO_STRING(RTCSS_REFER)
    DEFAULT_TO_STRING();
    }
#else
        return NULL;
#endif
}

inline const WCHAR* RTCRegistrationStateToString(RTC_REGISTRATION_STATE rtcrs)
{
#ifdef DEBUG
    
    switch (rtcrs)
    {
    TO_STRING(RTCRS_NOT_REGISTERED)
    TO_STRING(RTCRS_REGISTERING)
    TO_STRING(RTCRS_REGISTERED)
    TO_STRING(RTCRS_REJECTED)
    TO_STRING(RTCRS_UNREGISTERING)
    TO_STRING(RTCRS_ERROR)
    TO_STRING(RTCRS_LOGGED_OFF)
    TO_STRING(RTCRS_LOCAL_PA_LOGGED_OFF)
    TO_STRING(RTCRS_REMOTE_PA_LOGGED_OFF)
    DEFAULT_TO_STRING();
    }
#else
        return NULL;
#endif
}

inline const WCHAR* GroupIndexToString(PH_GROUPINDEX index)
{
#ifdef DEBUG
    
    switch (index)
    {
    TO_STRING(PH_GI_INVALID)
    TO_STRING(PH_GI_DEFAULT)
    TO_STRING(PH_GI_EXTPHONESTATE)
    TO_STRING(PH_GI_MSFTPHONESTATE)
    DEFAULT_TO_STRING();
    }
#else
        return NULL;
#endif
}

//DebugMsg helper function
inline VOID DebugDumpPhoneState(
    __in const PHPHONESTATE *pphState
    )
{
#ifdef DEBUG
    
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"%s: PHPHONESTATE:\r\n", MODULE_NAME));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tcGroups:   %d\r\n", pphState->cGroups));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tcTalking:  %d\r\n", pphState->cTalking));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tcBusy:     %d\r\n", pphState->cBusy));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tcConnected:%d\r\n", pphState->cConnected));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tcHeld:     %d\r\n", pphState->cHeld));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tcOutgoing: %d\r\n", pphState->cOutgoing));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tcIncoming: %d\r\n", pphState->cIncoming));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tcEnded:    %d\r\n", pphState->cEnded));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tiGroup1:   %d\r\n", pphState->iGroup1));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tiGroup2:   %d\r\n", pphState->iGroup2));
#endif
}

inline VOID DebugDumpGroupInfo(
    __in const PHGROUPINFO *phGroupInfo, 
    UINT         idxGroup
    )
{
#ifdef DEBUG
    
    if (! phGroupInfo)
    {
        return;
    }
    
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"%s: PHGROUPINFO for group %d\r\n", MODULE_NAME, idxGroup));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tGroupState:%s\r\n", StateToString(phGroupInfo->groupstate)));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tcCalls    :%d\r\n", phGroupInfo->cCalls));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tfMuted:    %d\r\n", phGroupInfo->fMuted));
    DEBUGMSG(ZONE_VOIP_PHSTATE, (L"\t\tiCallFirst:%d\r\n", phGroupInfo->iCallFirst));
#endif
}

//Structure debugging macros
inline VOID DebugDumpCallInfo(
    __in const PHCALLINFO* pphCallInfo, 
    UINT                   idxCall
    )
{
#ifdef DEBUG

    if (! pphCallInfo)
    {
        return;
    }
    
    DEBUGMSG(ZONE_VOIP_PHCALL, (L"%s: PHCALLINFO for call %d\r\n", MODULE_NAME, idxCall));
    DEBUGMSG(ZONE_VOIP_PHCALL, (L"\t\tiCallNextInGroup: %d\r\n", pphCallInfo->iCallNextInGroup));
    DEBUGMSG(ZONE_VOIP_PHCALL, (L"\t\tszDisplayName:    %s\r\n", pphCallInfo->szDisplayName));
    DEBUGMSG(ZONE_VOIP_PHCALL, (L"\t\tszDisplayNumber:  %s\r\n", pphCallInfo->szDisplayNumber));
    DEBUGMSG(ZONE_VOIP_PHCALL, (L"\t\tcidstate:         %s\r\n", CallerIDStateToString(pphCallInfo->cidstate)));
    DEBUGMSG(ZONE_VOIP_PHCALL, (L"\t\tcallstate:        %s\r\n", CallStateToString(pphCallInfo->callstate)));

    ce::wstring FlagsAsStr;
    CallInfoFlagsToString(pphCallInfo->callinfoflags, FlagsAsStr);
    DEBUGMSG(ZONE_VOIP_PHCALL, (L"\t\tcallinfoflags:    %s\r\n", (const WCHAR*)FlagsAsStr));

    FlagsAsStr = L"";
    CallBehaviorFlagsToString(pphCallInfo->callbehaviorflags, FlagsAsStr);
    DEBUGMSG(ZONE_VOIP_PHCALL, (L"\t\tcallbehaviorflags:%s\r\n", (const WCHAR*)FlagsAsStr));

    FlagsAsStr = L"";
    CallLogFlagsToString(pphCallInfo->calllogflags, FlagsAsStr);
    DEBUGMSG(ZONE_VOIP_PHCALL, (L"\t\tcalllogflags:%s\r\n", (const WCHAR*)FlagsAsStr));

    DEBUGMSG(ZONE_VOIP_PHCALL, (L"\t\thcall: 0x%x\r\n", pphCallInfo->hcall)); 
#endif
}

inline void TraceDialPlanElement(
    __in const WCHAR *pFormatString,
    __in_ecount(NameLength) const WCHAR *pName,
    int NameLength
    )
{
#ifdef DEBUG
    WCHAR Name[MAX_PATH];

    StringCchCopyNEx(
                    Name,
                    ARRAYSIZE(Name),
                    pName,
                    NameLength,
                    NULL,
                    NULL,
                    STRSAFE_IGNORE_NULLS
                    );
    VOIP_DEBUGMSG(ZONE_VOIP_DIALPLAN, (pFormatString, Name));
#endif
}

inline void TraceDialingRuleAttribute(
    __in const WCHAR *pFormatString,
    
    __in_ecount(NameLength)  const WCHAR *pName,
    int NameLength,

    __in_ecount(ValueLength) const WCHAR *pValue,
    int ValueLength
    )
{
#ifdef DEBUG
    WCHAR Name[MAX_PATH];
    WCHAR Value[MAX_PATH];

    StringCchCopyNEx(
                    Name,
                    ARRAYSIZE(Name),
                    pName,
                    NameLength,
                    NULL,
                    NULL,
                    STRSAFE_IGNORE_NULLS
                    );
    StringCchCopyNEx(
                    Value,
                    ARRAYSIZE(Value),
                    pValue,
                    ValueLength,
                    NULL,
                    NULL,
                    STRSAFE_IGNORE_NULLS
                    );
    VOIP_DEBUGMSG(ZONE_VOIP_DIALPLAN, (pFormatString, Name, Value));
#endif
}

#endif /* __DEBUG_HPP__ */

