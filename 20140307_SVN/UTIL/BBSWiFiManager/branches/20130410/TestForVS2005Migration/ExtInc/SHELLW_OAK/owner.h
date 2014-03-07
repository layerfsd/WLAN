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
Copyright (c) 1995-1998  Microsoft Corporation

Module Name:

owner.h

Abstract:

Notes:


--*/

#ifndef OWNER_H
#define OWNER_H

// owner/notes/password include file
#define TYPE_OWNER          0
#define TYPE_NOTES          1
#define TYPE_PASSWORD       2

// Values to be used for PasswordOnPowerUp
// bit 1 is on/off
#define PASSWORD_NONE       0x00   // No password active
#define PASSWORD_ON         0x01

// bit 2 is strong vs week (when it's on)
#define PASSWORD_PIN        0x00   // Pin password (bit 2 is clear)
#define PASSWORD_STRONG     0x02   // Strong Password (bit 2 is set)

#define PASSWORD_LENGTH     40
#define MAX_NOTES_STRING    (6*32)
#define MAX_NAME_STRING     35
#define MAX_COMPANY_STRING  35
#define MAX_ADD_STRING      (4*32)
#define MAX_CC_STRING       4
#define MAX_AC_STRING       10
#define MAX_PHONE_STRING    24
#define MAX_EMAIL_STRING    35

typedef struct tagOWNER_PROFILE {
    // Owner ID
    TCHAR   szName[MAX_NAME_STRING+1];
    TCHAR   szCompany[MAX_COMPANY_STRING+1];
    TCHAR   szAdd[MAX_ADD_STRING+1];

    TCHAR   szWCC[MAX_CC_STRING+1];
    TCHAR   szWAC[MAX_AC_STRING+1];
    TCHAR   szWPhone[MAX_PHONE_STRING+1];

    TCHAR   szHCC[MAX_CC_STRING+1];
    TCHAR   szHAC[MAX_AC_STRING+1];
    TCHAR   szHPhone[MAX_PHONE_STRING+1];
    
    TCHAR   szEmail[MAX_EMAIL_STRING+1];
    
    BYTE    IDPowerUp;
    } OWNER_PROFILE;

typedef struct tagOWNER_NOTES {
    // Notes
    TCHAR   Notes[MAX_NOTES_STRING+1];
    BYTE    NotesPowerUp;
    } OWNER_NOTES ;

typedef struct tagOWNER_PASSWRD {
    // Password.
    BYTE    PasswordOnPowerUp;

#ifdef UNDER_NT         
    TCHAR   Password[PASSWORD_LENGTH+1];
#endif
    } OWNER_PASSWRD;

// Registry Key defines
// Owner
#define szOwner             TEXT("Owner")
// Notes
#define szNotes             TEXT("Owner Notes")
// Password
#define szPasswordPowerUp   TEXT("PowrPass")

#ifdef POST03
POST03
// OID
#define szBizCard           TEXT("BizCard")
#endif

#ifdef UNDER_NT
#define szRegistryKey       TEXT("WinceControlPanel\\Owner")
#else
#define szRegistryKey       TEXT("ControlPanel\\Owner")
#endif

#ifdef UNDER_NT
#define szPassword          TEXT("Pass")
#endif

#endif


