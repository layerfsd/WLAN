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


#pragma once


#include <windows.h>

// the main reg location
const HKEY      XIP_ROOT_KEY                = HKEY_LOCAL_MACHINE;
const TCHAR     XIP_KEY[]                   = TEXT("Security\\UpdateXIP");
const TCHAR     XIP_UPDATE_BINARIES_LIST[]  = TEXT("Valid bin list");
const TCHAR     XIP_UPDATE_PENDING_LIST[]   = TEXT("Pending");
const TCHAR     XIP_UPDATE_REGION_LIST[]   = TEXT("Region");
const TCHAR     XIP_UPDATE_POSTPONED_LIST[]   = TEXT("Postponed");

const HKEY      COLDINIT_ROOT_KEY             = HKEY_LOCAL_MACHINE;
const TCHAR     COLDINIT_KEY[]                = TEXT("Security\\ColdInit");
const TCHAR     COLDINIT_CONFIG_FILE_XIPS[]   = TEXT("ConfigFileXIPList");
const TCHAR     COLDINIT_GUID_UI_EXE_TRIGGER[]= L"GUIDEXETRIGGER";

// to get romupdnote.exe to create a message box later on
const TCHAR     XIP_UPDATE_MSGBOX_TITLE[]   = TEXT("MsgBoxTitle");
const TCHAR     XIP_UPDATE_MSGBOX_TEXT[]    = TEXT("MsgBoxText");
const TCHAR     XIP_UPDATE_MSGBOX_TYPE[]    = TEXT("MsgBoxType");

// dir and logfiles
const TCHAR     XIP_LOG_FILE[]      = TEXT("\\Windows\\update\\updxiplog.txt");
const TCHAR     XIP_UPDATE_DIR[]    = TEXT("\\Windows\\update\\");

// extension, and max number of XIP files we'll do at once
const TCHAR     XIP_UPDATE_EXT[]    = TEXT(".xip");
const TCHAR     XDP_UPDATE_EXT[]    = TEXT(".xdp");
const TCHAR     XIP_MAX_FILES       = 7;

// File Extension Type Flags
#define UPDATE_FILETYPE_XIP     0x00000001
#define UPDATE_FILETYPE_XDP     0x00000002
#define UPDATE_FILETYPE_ANY     (UPDATE_FILETYPE_XIP | UPDATE_FILETYPE_XDP)

//
// useful macros
//

#define DIM(x)      (sizeof(x) / sizeof(x[0]))


// the CLSID for the notification interface

// 1a002df8-1efc-4fe1-a13b-1c6a6f665b47
DEFINE_GUID(CLSID_ROMUpdateNotification, 0x1a002df8, 0x1efc, 0x4fe1, 0xa1, 0x3b, 0x1c, 0x6a, 0x6f, 0x66, 0x5b, 0x47);

// other defs for notification stuff

const UINT ROMUPDATE_DISPLAY_TIME = 10;  // seconds
