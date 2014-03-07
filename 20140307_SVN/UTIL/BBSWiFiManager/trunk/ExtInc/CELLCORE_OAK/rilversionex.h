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
// rilversionex.h
//

#pragma once
#ifndef _RILVERSION_H_
#define _RILVERSION_H_


#include <bldver.h>


#ifdef __cplusplus
extern "C" {
#endif


//  ::::::::::::::::::::::::::::::::  ++ VERSION NUMBERS  ::::::::::::::::::::::::::::::::

//
//  NOTE
//
//  -  Valid version numbers are in the range [1, ULONG_MAX] inclusively with ULONG_MAX meaning
//     that the corresponding RIL implementation element type has been deprecated.
//     Version number of zero must not be used.
//  -  In a version number range for an implementation element type, the low version number
//     must be smaller than or equal to the high version number.
//

//
//  RIL Versioning version history
//
//  ver 0x2: [Change description including BC info.]
//  ver 0x1: Initial RIL versioning check in.
//
#define RIL_VERSIONING_HIGH_VERSION             0x1
#define RIL_VERSIONING_LOW_VERSION              0x1
#define RIL_VERSIONING_HIGH_OPTIONAL            0x1  // The initial version is optional for proxy enforcement.

//
//  RIL API Routine version history
//  ver 0x4: Added RIL_ChangeCallBarringPasswordEx.
//  ver 0x3: PAP/CHAP changes for RILNDISGPRSCONTEXT.
//  ver 0x2: Added RIL_DataDormant.
//  ver 0x1: Initial RIL versioning check in.
//
#define RIL_API_HIGH_VERSION                    0x4
#define RIL_API_LOW_VERSION                     0x1


//
//  RIL Notification Class version history
//
//  ver 0x2: [Change description including BC info.]
//  ver 0x1: Initial RIL versioning check in.
//
#define RIL_NOTIF_CLASS_HIGH_VERSION            0x1
#define RIL_NOTIF_CLASS_LOW_VERSION             0x1


//
//  RIL Result Code version history
//
//  ver 0x2: [Change description including BC info.]
//  ver 0x1: Initial RIL versioning check in.
//
#define RIL_RESULT_CODE_HIGH_VERSION            0x1
#define RIL_RESULT_CODE_LOW_VERSION             0x1


//
//  RIL Notification Code version history
//
//  ver 0x2: Added RIL_NOTIFY_CALLMODIFICATIONINFO.
//  ver 0x1: Initial RIL versioning check in.
//
#define RIL_NOTIF_CODES_HIGH_VERSION            0x2
#define RIL_NOTIF_CODES_LOW_VERSION             0x1


//
//  RIL Error Class version history
//
//  ver 0x2: [Change description including BC info.]
//  ver 0x1: Initial RIL versioning check in.
//
#define RIL_ERROR_CLASS_HIGH_VERSION            0x1
#define RIL_ERROR_CLASS_LOW_VERSION             0x1


//
//  RIL Error Code version history
//
//  ver 0x3: Added RIL_E_PASSWORDMISMATCH.
//  ver 0x2: Added RIL_E_SIMCALLMODIFIED.
//  ver 0x1: Initial RIL versioning check in.
//
#define RIL_ERROR_CODE_HIGH_VERSION             0x3
#define RIL_ERROR_CODE_LOW_VERSION              0x1

//  ::::::::::::::::::::::::::::::::  -- VERSION NUMBERS  ::::::::::::::::::::::::::::::::


#define ARRAYECOUNT( a )                    (sizeof( a ) / sizeof(*( a )))
#define TSTRINGIZE( arg )                   _T( #arg )
#define TSTRINGIZE_MACROARG( macroarg )     TSTRINGIZE( macroarg )


typedef struct VERSIONEX
{
    TCHAR m_ElementTypeName[ MAX_PATH ];
    DWORD m_HighVersion;  //  ULONG_MAX == deprecated, 0 == reserved / not used
    DWORD m_LowVersion;   //  ULONG_MAX == deprecated, 0 == reserved / not used
} VERSIONEX, *LPVERSIONEX;
typedef const struct VERSIONEX *LPCVERSIONEX;

typedef enum RilImpleElementType_t
{
    RilVersioning = 0,
    Api,
    NotifClass,
    ResultCode,
    NotifCode,
    ErrorClass,
    ErrorCode,
    RilImpleElementTypeEcount
} RilImpleElementType_t;

typedef struct RILVERSIONEX
{
    size_t m_StructSize;
    TCHAR m_OsBuildName[ MAX_PATH ];
    struct VERSIONEX ElementType[ RilImpleElementTypeEcount ];
} RILVERSIONEX, *LPRILVERSIONEX;
typedef const struct RILVERSIONEX *LPCRILVERSIONEX;


#define DEFRILVERSIONEX( var )                                                                                  \
    struct RILVERSIONEX (var) =                                                                                 \
    {                                                                                                           \
        sizeof(struct RILVERSIONEX),                                                                            \
        _T("Microsoft Windows CE: Major version (") TSTRINGIZE_MACROARG(CE_MAJOR_VER)                           \
        _T(") Minor version (") TSTRINGIZE_MACROARG(CE_MINOR_VER)                                               \
        _T(") Build (") TSTRINGIZE_MACROARG(CE_BUILD_VER)                                                       \
        _T(") (") _T(__DATE__) _T(" ") _T(__TIME__) _T(")"),                                                    \
        {                                                                                                       \
            { _T("VersioningEx"),           RIL_VERSIONING_HIGH_VERSION,    RIL_VERSIONING_LOW_VERSION, },      \
            { _T("API Routines"),           RIL_API_HIGH_VERSION,           RIL_API_LOW_VERSION, },             \
            { _T("Notification Classes"),   RIL_NOTIF_CLASS_HIGH_VERSION,   RIL_NOTIF_CLASS_LOW_VERSION, },     \
            { _T("Result Codes"),           RIL_RESULT_CODE_HIGH_VERSION,   RIL_RESULT_CODE_LOW_VERSION, },     \
            { _T("Notification Codes"),     RIL_NOTIF_CODES_HIGH_VERSION,   RIL_NOTIF_CODES_LOW_VERSION, },     \
            { _T("Error Classes"),          RIL_ERROR_CLASS_HIGH_VERSION,   RIL_ERROR_CLASS_LOW_VERSION, },     \
            { _T("Error Codes"),            RIL_ERROR_CODE_HIGH_VERSION,    RIL_ERROR_CODE_LOW_VERSION, },      \
        },                                                                                                      \
    }


#ifdef __cplusplus
}  //  extern "C"
#endif


#endif  //  _RILVERSION_H_

