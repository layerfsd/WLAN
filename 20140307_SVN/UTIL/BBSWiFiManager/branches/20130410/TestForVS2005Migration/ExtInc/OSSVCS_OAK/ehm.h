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
/***********************************************************
    ehm.h
    Error handling macros

Author(s): KKennedy (derived from many sources)

    {x,}{CVD}{H,P,B,W}R{A,}{Ex,}
    C=check, V=verify, D=dump
    H=hresult, P=ptr, B=bool, W=windows
    R=result
    A=assert
    Ex=extended version
    x=wart, suppresses assert due to practical pblm (obsolete!)

    C checks, and bails on failure after setting hr
        A adds an ASSERT before the bail
    V checks, and ASSERTs on failure
    D checks, and SPEWs on failure
    (yes, the 'A' is inconsistent)
    Ex adds overriding failure-case 'E_*' of user's choice
    x is obsolete, you shouldn't use it

    n.b. we only have some of the above implemented (the ones we've needed
    so far...)

    if you're an unlucky soul and run into a label collision, use the
    _ehmErrorLab mechanism (see below).

    _ehmOnAssertionFail 
        Allows custom processing of a failure in macros that assert.
        As defined here, this maps to OnAssertionFail().
        By defining this earlier, you could implement custom logging of the failure

    _ehmOnFail
        Allows custom processing of a failure in macros that don't assert. 
        As defined here, this maps to nothing.
        By defining this earlier, you could implement custom logging of the failure.


    UTCore.H - takes advantage of both _ehmOnFail and _ehmOnAssertionFail 
        extensions to provide slightly different behavior and enable logging 
        for unit tests.  

    see also the document: <todo: ptr to in kk's ehm .doc>

NOTES
    we use the "if (0,fResult), while (0,0)" style in order to suppress W4 warnings.  the PF_EXPR wraps that to keep prefix happy.



*/
#ifndef _EHM_H_
#define _EHM_H_

#ifdef FAILPOINTS_ENABLED
#include "fault\failpt.h"
#endif // FAILPOINTS_ENABLED


#ifdef __cplusplus
extern "C"
{
#endif


// 99% of the time ehm.h "just works".  however 1% of the time the "Error"
// label is already in use (e.g. for some other macro package).  if you hit
// that, do this:
//      #define _ehmErrorLab    EhmError
//      #include <ehm.h>
// ... and then use EhmError as your label.  it may seem a bit silly to add
// this extra indirection vs. just use EhmError always, but given that it's
// a 99%/1%, it seems worthwhile.
//
// our suggestion is that custom clients standardize on "EhmError", but if
// need be, they can choose whatever they want.
#ifndef _ehmErrorLab
#define _ehmErrorLab   Error
#endif

// If you define the value for _ehmOnAssertionFail before #including ehm.h you
// can change what _ehmOnAssertionFail does
// Remember this affects all xxxA macros
#ifndef _ehmOnAssertionFail
#define _ehmOnAssertionFail(eType, dwCode, pszFile, ulLine, pszMessage) \
         OnAssertionFail(eType, dwCode, pszFile, ulLine, pszMessage)
#endif


// If you define the value for _ehmOnFail before #including ehm.h you
// can change what _ehmOnFail does
// Remember this affects all non-A macros
#ifndef _ehmOnFail
#define _ehmOnFail(eType, dwCode, pszFile, ulLine, pszMessage)    { }
#endif


typedef enum
    {
    eHRESULT,
    eBOOL,
    ePOINTER,
    eWINDOWS
    } eCodeType;

BOOL ShouldBreakOnAssert();
BOOL OnAssertionFail(eCodeType eType, DWORD dwCode, const TCHAR* pszFile, unsigned long ulLine, const TCHAR* pszMessage);
BOOL SetBreakOnAssert(BOOL fBreakOnAssert);

// Check HRESULT
#define _CHREx0_DONTINJECTFAILURE(hResult) \
    do { \
        hr = (hResult); \
        if(FAILED(hr)) \
        { \
            _ehmOnFail(eHRESULT, 0, TEXT(__FILE__), __LINE__, TEXT("CHR(") TEXT( # hResult ) TEXT(")")); \
            goto _ehmErrorLab; \
        } \
    } while (0,0)

#define _CHR_DONTINJECTFAILURE(hResult, hrFail) \
    do { \
        hr = (hResult); \
        if(FAILED(hr)) \
        { \
            hr = (hrFail); \
            _ehmOnFail(eHRESULT, 0, TEXT(__FILE__), __LINE__, TEXT("CHR(") TEXT( # hResult ) TEXT(")")); \
            goto _ehmErrorLab; \
        } \
    } while (0,0)

#ifdef FAILPOINTS_ENABLED

#define _CHREx0(hResult) \
    do { \
        BOOL fFailPoint = SOS_StackHashingControl__GenericFailure(); \
        hr = (hResult); \
        hr = FAILED(hr) ? \
            hr : \
            fFailPoint ? \
                E_HRFAILPOINT: \
                hr; \
        if(FAILED(hr)) \
        { \
            _ehmOnFail(eHRESULT, 0, TEXT(__FILE__), __LINE__, TEXT("CHR(") TEXT( # hResult ) TEXT(")")); \
            goto _ehmErrorLab; \
        } \
    } while (0,0)

#define _CHR(hResult, hrFail) \
    do { \
        BOOL fFailPoint = SOS_StackHashingControl__GenericFailure(); \
        hr = (hResult); \
        hr = FAILED(hr) ? \
            hr : \
            fFailPoint ? \
                E_HRFAILPOINT: \
                hr; \
        if(FAILED(hr)) \
        { \
            hr = (hrFail); \
            _ehmOnFail(eHRESULT, 0, TEXT(__FILE__), __LINE__, TEXT("CHR(") TEXT( # hResult ) TEXT(")")); \
            goto _ehmErrorLab; \
        } \
    } while (0,0)

#else // #ifdef FAILPOINTS_ENABLED

#define _CHREx0(hResult) _CHREx0_DONTINJECTFAILURE(hResult)
#define _CHR(hResult, hrFail) _CHR_DONTINJECTFAILURE(hResult, hrFail)

#endif // #ifdef FAILPOINTS_ENABLED

// Check pointer result
#define _CPR_DONTINJECTFAILURE(p, hrFail) \
    do { \
        if (PF_EXPR(!(p))) \
        { \
            hr = (hrFail); \
            _ehmOnFail(ePOINTER, 0, TEXT(__FILE__), __LINE__, TEXT("CPR(") TEXT( # p ) TEXT(")")); \
            goto _ehmErrorLab; \
         } \
     } while (0,0)

// Check boolean result
#define _CBR_DONTINJECTFAILURE(fResult, hrFail) \
    do { \
        if (PF_EXPR(!(fResult))) \
        { \
            hr = (hrFail); \
            _ehmOnFail(eBOOL, 0, TEXT(__FILE__), __LINE__, TEXT("CBR(") TEXT( # fResult ) TEXT(")")); \
            goto _ehmErrorLab; \
        } \
    } while(0,0)

#ifdef FAILPOINTS_ENABLED

// Check pointer result
#define _CPR(p, hrFail) \
    do { \
        BOOL fFailPoint = SOS_StackHashingControl__GenericFailure(); \
        if (fFailPoint || PF_EXPR(!(p))) \
        { \
            hr = (hrFail); \
            _ehmOnFail(ePOINTER, 0, TEXT(__FILE__), __LINE__, TEXT("CPR(") TEXT( # p ) TEXT(")")); \
            goto _ehmErrorLab; \
        } \
    } while (0,0)

// Check boolean result
#define _CBR(fResult, hrFail) \
    do { \
        BOOL fFailPoint = SOS_StackHashingControl__GenericFailure(); \
        if (fFailPoint || PF_EXPR(!(fResult))) \
        { \
            hr = (hrFail); \
            _ehmOnFail(eBOOL, 0, TEXT(__FILE__), __LINE__, TEXT("CBR(") TEXT( # fResult ) TEXT(")")); \
            goto _ehmErrorLab; \
        } \
    } while(0,0)

#else // #ifdef FAILPOINTS_ENABLED

#define _CPR(hResult, hrFail) _CPR_DONTINJECTFAILURE(hResult, hrFail)
#define _CBR(hResult, hrFail) _CBR_DONTINJECTFAILURE(hResult, hrFail)

#endif // #ifdef FAILPOINTS_ENABLED


// Check windows result.  Exactly like CBR for the non-Asserting case - BUT we log differently
#define _CWR(fResult, hrFail) \
    do { \
        if (PF_EXPR(!(fResult))) \
        { \
            hr = (hrFail); \
            _ehmOnFail(eWINDOWS, 0, TEXT(__FILE__), __LINE__, TEXT("CWR(") TEXT( # fResult ) TEXT(")")); \
            goto _ehmErrorLab; \
        } \
    } while(0,0)


// The above macros with Asserts when the condition fails
#ifdef DEBUG

#ifdef FAILPOINTS_ENABLED


#define _CHRA_DONTINJECTFAILURE(hResult, hrFail) _CHR_DONTINJECTFAILURE(hResult, hrFail)
#define _CHRAEx0_DONTINJECTFAILURE(hResult, hrFail) _CHREx0_DONTINJECTFAILURE(hResult, hrFail)
#define _CPRA_DONTINJECTFAILURE(hResult, hrFail) _CPR_DONTINJECTFAILURE(hResult, hrFail)
#define _CBRA_DONTINJECTFAILURE(hResult, hrFail) _CBR_DONTINJECTFAILURE(hResult, hrFail)
#define _CWRA_DONTINJECTFAILURE(hResult, hrFail) _CWR_DONTINJECTFAILURE(hResult, hrFail)

#define _CHRAEx0(hResult)   _CHREx0(hResult)
#define _CHRA(hResult, hrFail)  _CHR(hResult, hrFail)
#define _CPRA(fResult, hrFail)  _CPR(fResult, hrFail)
#define _CBRA(fResult, hrFail)  _CBR(fResult, hrFail)
#define _CWRA(fResult, hrFail)  _CWR(fResult, hrFail)

#else // #ifdef FAILPOINTS_ENABLED

#define _CHRAEx0(hResult) \
    do { \
        hr = (hResult); \
        if(FAILED(hr)) \
            { \
            if(_ehmOnAssertionFail(eHRESULT, hr, TEXT(__FILE__), __LINE__, TEXT("CHRA(") TEXT( # hResult ) TEXT(")"))) \
                { \
                DebugBreak(); \
                } \
            goto _ehmErrorLab; \
            } \
        } \
    while (0,0)

#define _CHRA(hResult, hrFail) \
    do { \
        hr = (hResult); \
        if(FAILED(hr)) \
            { \
            hr = (hrFail); \
            if(_ehmOnAssertionFail(eHRESULT, hr, TEXT(__FILE__), __LINE__, TEXT("CHRA(") TEXT( # hResult ) TEXT(")"))) \
                { \
                DebugBreak(); \
                } \
            goto _ehmErrorLab; \
            } \
        } \
    while (0,0)

#define _CPRA(p, hrFail) \
    do  { \
        if (PF_EXPR(!(p))) \
            { \
            hr = (hrFail); \
            if(_ehmOnAssertionFail(ePOINTER, 0, TEXT(__FILE__), __LINE__, TEXT("CPRA(") TEXT( # p ) TEXT(")"))) \
                { \
                DebugBreak(); \
                } \
            goto _ehmErrorLab; \
            } \
        } \
    while (0,0)

#define _CBRA(fResult, hrFail) \
    do  { \
        if (PF_EXPR(!(fResult))) \
            { \
            hr = (hrFail); \
            if(_ehmOnAssertionFail(eBOOL, 0, TEXT(__FILE__), __LINE__, TEXT("CBRA(") TEXT( # fResult ) TEXT(")"))) \
                { \
                DebugBreak(); \
                } \
            goto _ehmErrorLab; \
            } \
        } \
    while (0,0)

#define _CWRA(fResult, hrFail) \
    do  { \
        if (PF_EXPR(!(fResult))) \
            { \
            hr = (hrFail); \
            if(_ehmOnAssertionFail(eWINDOWS, 0, TEXT(__FILE__), __LINE__, TEXT("CWRA(") TEXT( # fResult ) TEXT(")"))) \
                { \
                DebugBreak(); \
                } \
            goto _ehmErrorLab; \
            } \
        } \
    while (0,0)

#define _CHRA_DONTINJECTFAILURE(hResult, hrFail) _CHRA(Result, hrFail)
#define _CHRAEx0_DONTINJECTFAILURE(hResult, hrFail) _CHRAEx0(hResult, hrFail)
#define _CPRA_DONTINJECTFAILURE(hResult, hrFail) _CPRA(hResult, hrFail)
#define _CBRA_DONTINJECTFAILURE(hResult, hrFail) _CBRA(hResult, hrFail)
#define _CWRA_DONTINJECTFAILURE(hResult, hrFail) _CWRA(hResult, hrFail)

#endif // #ifdef FAILPOINTS_ENABLED


#define VBR(fResult) \
    do  { \
        if (PF_EXPR(!(fResult))) \
            { \
            if(_ehmOnAssertionFail(eBOOL, 0, TEXT(__FILE__), __LINE__, TEXT("VBR(") TEXT( # fResult ) TEXT(")"))) \
                { \
                DebugBreak(); \
                } \
            } \
        } \
    while (0,0)

#define VPR(fResult) \
    do  { \
        if (PF_EXPR(!(fResult))) \
            { \
            if(_ehmOnAssertionFail(ePOINTER, 0, TEXT(__FILE__), __LINE__, TEXT("VPR(") TEXT( # fResult ) TEXT(")"))) \
                { \
                DebugBreak(); \
                } \
            } \
        } \
    while (0,0)


// Verify Windows Result
#define VWR(fResult) \
    do  { \
        if (!(PF_EXPR(NULL != (fResult)))) \
            { \
            if(_ehmOnAssertionFail(eWINDOWS, 0, TEXT(__FILE__), __LINE__, TEXT("VWR(") TEXT( # fResult ) TEXT(")"))) \
                { \
                DebugBreak(); \
                } \
            } \
        } \
    while (0,0)


// Verify HRESULT (careful not to modify hr)
#define VHR(hResult) \
    do  { \
        HRESULT _EHM_hrTmp = (hResult); \
        if(FAILED(_EHM_hrTmp)) \
            { \
            if(_ehmOnAssertionFail(eHRESULT, _EHM_hrTmp, TEXT(__FILE__), __LINE__, TEXT("VHR(") TEXT( # hResult ) TEXT(")"))) \
                { \
                DebugBreak(); \
                } \
            } \
        } \
    while (0,0)

// NOTE: because the Dxx macros are intended for DEBUG spew - there is no logging extensibility
// make sure you keep the xTmp, can only eval arg 1x
// todo: dump GetLastError in DWR
#define DWR(fResult) \
    do { if (PF_EXPR(!(fResult))) {DEBUGMSG(1, (TEXT("DWR(") TEXT( # fResult ) TEXT(")\r\n") ));}} while (0,0)
#define DHR(hResult) \
    do { HRESULT hrTmp = hResult; if(FAILED(hrTmp)) {DEBUGMSG(1, (TEXT("DHR(") TEXT( # hResult ) TEXT(")=0x%x\r\n"), hrTmp));}} while (0,0)
#define DPR     DWR     // tmp
#define DBR     DWR     // tmp

#define CHRA(e) _CHRAEx0(e)
#define CPRA(e) _CPRA(e, E_OUTOFMEMORY)
#define CBRA(e) _CBRA(e, E_FAIL)
#define CWRA(e) _CWRA(e, E_FAIL)
#define CHRAEx(e, hrFail) _CHRA(e, hrFail)
#define CPRAEx(e, hrFail) _CPRA(e, hrFail)
#define CBRAEx(e, hrFail) _CBRA(e, hrFail)
#define CWRAEx(e, hrFail) _CWRA(e, hrFail)
#else
#define CHRA CHR
#define CPRA CPR
#define CBRA CBR
#define CWRA CWR
#define CHRAEx CHREx
#define CPRAEx CPREx
#define CBRAEx CBREx
#define CWRAEx CWREx
// Cast to void to eliminate prefast warnings.
#define VHR(x) ((void)(x))
#define VPR(x) ((void)(x))
#define VBR(x) ((void)(x))
#define VWR(x) ((void)(x))
#define DHR(x) (x)
#define DPR(x) (x)
#define DBR(x) (x)
#define DWR(x) (x)
#endif

#define CHR(e) _CHREx0(e)
#define CHR_DONTINJECTFAILURE(e)    _CHREx0_DONTINJECTFAILURE(e)
#define CHRA_DONTINJECTFAILURE(e)    _CHRAEx0_DONTINJECTFAILURE(e)
#define CPR(e) _CPR(e, E_OUTOFMEMORY)
#define CPR_DONTINJECTFAILURE(e)    _CPR_DONTINJECTFAILURE(e, E_OUTOFMEMORY)
#define CPRA_DONTINJECTFAILURE(e)    _CPRA_DONTINJECTFAILURE(e, E_OUTOFMEMORY)
#define CBR(e) _CBR(e, E_FAIL)
#define CBR_DONTINJECTFAILURE(e)    _CBR_DONTINJECTFAILURE(e, E_FAIL)
#define CBRA_DONTINJECTFAILURE(e)    _CBRA_DONTINJECTFAILURE(e, E_FAIL)
#define CWR(e) _CWR(e, E_FAIL)
#define CWR_DONTINJECTFAILURE(e)    _CWR_DONTINJECTFAILURE(e, E_FAIL)
#define CWRA_DONTINJECTFAILURE(e)    _CWRA_DONTINJECTFAILURE(e, E_FAIL)
#define CHREx(e, hrFail) _CHR(e, hrFail)
#define CPREx(e, hrFail) _CPR(e, hrFail)
#define CBREx(e, hrFail) _CBR(e, hrFail)
#define CWREx(e, hrFail) _CWR(e, hrFail)

#ifdef FAILPOINTS_ENABLED
#define ASSERT_DONTINJECTFAILURE(e) 
#else
#define ASSERT_DONTINJECTFAILURE(e) ASSERT(e)
#endif // FAILPOINTS_ENABLED

// obsolete (but still in use)
// - work around various pseudo-pblms:
//  partial images, CEPC no-radio, etc.
// - also things that we want to know about in dev, but not in QA/stress:
//  an e.g. is our DoVerb stuff, there are 'good' failures (END when no call,
// or TALK w/ 0 entries) and 'bad' failures (e.g. TAPI returns an error), we
// don't want to int3 during stress but we do want to on our dev machines
//
// eventually we'll make these do "if (g_Assert) int3;", then we
// can run w/ g_Assert on for dev and off for stress.
#define xCHRA   CHR     // should be CHRA but...
#define xCPRA   CPR     // should be CPRA but...
#define xCBRA   CBR     // should be CBRA but...
#define xCWRA   CWR     // should be CWRA but...
#define xVHR    DHR     // should be VHR  but...
#define xVPR    DPR     // should be VPR  but...
#define xVBR    DBR     // should be VBR  but...
#define xVWR    DWR     // should be VWR  but...

#ifdef __cplusplus
}
#endif

#endif // _EHM_H_
