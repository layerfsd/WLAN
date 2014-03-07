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

#ifndef __CESDDL_H__
#define __CESDDL_H__


#ifdef __cplusplus
extern "C" {
#endif

//
// SDDL Version information
//
#define SDDL_REVISION_1     1
#define SDDL_REVISION       SDDL_REVISION_1

#define CE_SD_REVISION      SDDL_REVISION_1

//
// SDDL Component tags
//
#define SDDL_OWNER                          TEXT("O")       // Owner tag
#define SDDL_GROUP                          TEXT("G")       // Group tag
#define SDDL_DACL                           TEXT("D")       // DACL tag

#define SDDL_OWNERC                         TEXT('O')       // Owner tag char
#define SDDL_GROUPC                         TEXT('G')       // Group tag char
#define SDDL_DACLC                          TEXT('D')       // DACL tag char


//
// SDDL Ace types
//
#define SDDL_ACCESS_ALLOWED                 TEXT("A")   // Access allowed
#define SDDL_ACCESS_DENIED                  TEXT("D")   // Access denied

#define SDDL_ACCESS_ALLOWEDC                TEXT('A')   // Access allowed char
#define SDDL_ACCESS_DENIEDC                 TEXT('D')   // Access denied char


//
// SDDL Rights
//
#define SDDL_KEY_ALL                        TEXT("KA")
#define SDDL_KEY_READ                       TEXT("KR")
#define SDDL_KEY_WRITE                      TEXT("KW")
#define SDDL_KEY_EXECUTE                    TEXT("KX")

//
// CE extention
//
#define SDDL_SYNC_ALL                       TEXT("SA")


//
// SDDL Seperators - character version
//
#define SDDL_SEPERATORC                     TEXT(';')
#define SDDL_DELIMINATORC                   TEXT(':')
#define SDDL_ACE_BEGINC                     TEXT('(')
#define SDDL_ACE_ENDC                       TEXT(')')

//
// SDDL Seperators - string version
//
#define SDDL_SEPERATOR                     TEXT(";")
#define SDDL_DELIMINATOR                   TEXT(":")
#define SDDL_ACE_BEGIN                     TEXT("(")
#define SDDL_ACE_END                       TEXT(")")

//
// CE extention
//
#define SDDL_BEGINNAME                     TEXT("<")
#define SDDL_ENDNAME                       TEXT(">")

#define SDDL_BEGINNAMEC                    TEXT('<')
#define SDDL_ENDNAMEC                      TEXT('>')

BOOL
CeConvertStrToSD (
    IN  LPCWSTR StringSecurityDescriptor,
    IN  DWORD StringSDRevision,
    OUT PSECURITY_DESCRIPTOR  *SecurityDescriptor,
    OUT PULONG  SecurityDescriptorSize OPTIONAL
    );

BOOL
CeConvertSDToStr(
    IN  PSECURITY_DESCRIPTOR  SecurityDescriptor,
    IN  DWORD RequestedStringSDRevision,
    IN  SECURITY_INFORMATION SecurityInformation,
    OUT LPWSTR  *StringSecurityDescriptor OPTIONAL,
    OUT PULONG StringSecurityDescriptorLen OPTIONAL
    );


#ifdef __cplusplus
}
#endif

#endif  // endif __CESDDL_H__


