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
//
// profileshared.h
//
// Contains registry keys shared between the profile csp and the profiles app.
//

#define REG_wszProfilesRegKey                               L"ControlPanel\\Profiles"
#define REG_wszProfilesLocName                                 L"LocName"
#define REG_wszProfilesEditName                             L"EditName"
#define REG_wszProfilesActiveProfile                        L"ActiveProfile"
#define REG_wszProfilesDefaultActiveProfile                 L"DefaultActiveProfile"
#define REG_wszProfilesPreviousProfile                      L"PreviousProfile"
#define REG_wszProfilesXML                                  L"XML"
#define REG_wszProfilesMsgItems                             L"MsgItems"
#define REG_wszProfilesAutoOnly                             L"AutoOnly"

#define DIR_wszProfilesAppDataDir                           L"Profiles"
#define DIR_wszProfilesDefaultDir                           L"\\Windows\\"
#define FILE_wszProfilesExtension                           L".prof.xml"
#define FILE_wszProfilesDefaultExtension                    L".def" FILE_wszProfilesExtension

