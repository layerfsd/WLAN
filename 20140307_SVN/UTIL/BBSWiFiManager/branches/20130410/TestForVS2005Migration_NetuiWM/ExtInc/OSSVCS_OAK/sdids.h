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
/*****************************************************************************
    Copyright (c) Microsoft Corporation. All rights reserved.

    File: sdids.h

    Table of information about status data IDs

*/

// No "#pragma once" since this may get included multiple times to build different tables.

//
// If you add entries to the end of this table then you can just rebuild statstor (and ..\dll)
// and use them.  If you insert entries into the table then you need to do a full rebuild
// since you just shifted all the values following the new one you added.
//


// YOU MUST ADD ANY NEW ENTRIES TO THE END OF THIS LIST!!!
// IF YOU REMOVE AN ENTRY, YOU MUST PUT AN EMPTY ENTRY IN ITS PLACE TO MAINTAIN THE ORDER OF THIS TABLE!!!

// flags:
// if ((0x0f & flags) == 1) then the key should be under HKEY_CURRENT_USER
// if ((0x0f & flags) == 2) then the key should be under HKEY_LOCAL_MACHINE
// if the high bit of flags is 1 then this key should be volatile

// NOTE:
// 1. Do NOT define volatile and non-volatile keys under the same registry key.
// 2. Define non-volatile keys in public\ossvcs\oak\files\ossvcs.reg (without values) to make sure
// they are created (permanent)

// Due to current Registry API behavior, if a key marked as volatile gets created first then all
// the new (created) nodes from the registry path will be volatile as well.
// e.g. Suppose
//      HKEY_LOCAL_MACHINE\System\State\Foo is defined as non-volatile for SDID_X value
//      HKEY_LOCAL_MACHINE\System\State\Foo\SubKey is defined as volatile for SDID_Y value
//      and HKEY_LOCAL_MACHINE\System\State is already created and it is non-volatile.
// if HKEY_LOCAL_MACHINE\System\State\Foo does not exist, then its volatileness depends on which
// value is set first. If someone sets SDID_Y value first, Foo wil be volatile and consequently all
// subkeys - regardless of the non-volatile definition on the table -.

//        Name of ID,                   type,           flags,       subkey name,                                             value name
SDID_INFO(SDID_TIME,                    CEVT_FILETIME,  0x80000002,  TEXT("System\\State\\DateTime"),                        TEXT("Time")) // Time that only changes each minute
SDID_INFO(SDID_DATE,                    CEVT_FILETIME,  0x80000002,  TEXT("System\\State\\DateTime"),                        TEXT("Date")) // For detecting date changes.
SDID_INFO(SDID_INPUT_STATE,             CEVT_UI4,       0x80000002,  TEXT("System\\State"),                                  TEXT("Input State")) // Input state flags. See ISF_* constants
SDID_INFO(SDID_TIMEZONE_BIAS,           CEVT_UI4,       0x80000002,  TEXT("System\\State\\DateTime"),                        TEXT("Timezone Bias")) //
SDID_INFO(SDID_BATTERY,                 CEVT_UI4,       0x80000002,  TEXT("System\\State\\Battery"),                         TEXT("Main")) // Battery flags & strength - Flags are in LOWORD. Strength is in HIWORD Range:0-100
SDID_INFO(SDID_BACKUP_BATTERY,          CEVT_UI4,       0x80000002,  TEXT("System\\State\\Battery"),                         TEXT("Backup")) // Backup Battery flags & strength - Flags are in LOWORD. Strength is in HIWORD Range:0-100
SDID_INFO(SDID_PHONE_STATUS_FLAGS,      CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Status")) //
SDID_INFO(SDID_RADIO_READY_STATE,       CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Radio Ready State")) // Ready state of the radio (e.g. SIM, SMS)
SDID_INFO(SDID_SIGNAL_STRENGTH,         CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Signal Strength")) //
SDID_INFO(SDID_CURRENT_OPERATOR_NAME,   CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Current Operator Name")) //
SDID_INFO(SDID_TALKING_CALLER_NAME,     CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Caller Name")) // not set when you are talking with a conference
SDID_INFO(SDID_TALKING_CALLID_STATUS,   CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Call ID Status")) // Values are same as TPCCallerIDState in tpcmgr.h
SDID_INFO(SDID_ACTIVE_CALL_COUNT,       CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Active Call Count")) //
SDID_INFO(SDID_CURRENT_PROFILE,         CEVT_LPWSTR,    0x00000001,  TEXT("System\\State"),                                  TEXT("Profile")) //
SDID_INFO(SDID_SIMTKIT_IDLE,            CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Simtkit Idle")) //
SDID_INFO(SDID_CELL_BROADCAST,          CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Cell Broadcast")) //
SDID_INFO(SDID_PREV_APPT_ENDTIME,       CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Appointments\\Previous"),          TEXT("End Time")) //
SDID_INFO(SDID_NEXT_APPT_SUBJECT,       CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\Next"),              TEXT("Subject")) //
SDID_INFO(SDID_NEXT_APPT_LOCATION,      CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\Next"),              TEXT("Location")) //
SDID_INFO(SDID_NEXT_APPT_STARTTIME,     CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Appointments\\Next"),              TEXT("Start Time")) //
SDID_INFO(SDID_NEXT_APPT_ENDTIME,       CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Appointments\\Next"),              TEXT("End Time")) //
SDID_INFO(SDID_NEXT_APPT_CONFLICT,      CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments\\Next"),              TEXT("Conflict")) // Values: 0 or 1
SDID_INFO(SDID_NEXT_EVENT_SUBJECT,      CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\Event"),             TEXT("Subject")) //
SDID_INFO(SDID_MSG_TOTAL_EMAIL_UNREAD,  CEVT_UI4,       0x00000001,  TEXT("System\\State\\Messages\\TotalEmail\\Unread"),    TEXT("Count")) // Count of unread mail across all accounts (except those that request to be excluded)
SDID_INFO(SDID_MSG_SMS_UNREAD,          CEVT_UI4,       0x00000001,  TEXT("System\\State\\Messages\\sms\\Unread"),           TEXT("Count")) // Count of unread mail in the SMS account
SDID_INFO(SDID_MSG_MMS_UNREAD,          CEVT_UI4,       0x00000001,  TEXT("System\\State\\Messages\\mms\\Unread"),           TEXT("Count")) // Count of unread mail in the MMS account
SDID_INFO(SDID_MSG_SYNC_UNREAD,         CEVT_UI4,       0x00000001,  TEXT("System\\State\\Messages\\sync\\Unread"),          TEXT("Count")) // Count of unread mail in the sync account
SDID_INFO(SDID_MSG_SMS_UNREAD_SZ,       CEVT_LPWSTR,    0x00000001,  TEXT("System\\State\\Messages\\sms\\Unread"),           TEXT("Text")) // SMS (##)
SDID_INFO(SDID_MSG_MMS_UNREAD_SZ,       CEVT_LPWSTR,    0x00000001,  TEXT("System\\State\\Messages\\mms\\Unread"),           TEXT("Text")) // MMS (##)
SDID_INFO(SDID_MSG_SYNC_UNREAD_SZ,      CEVT_LPWSTR,    0x00000001,  TEXT("System\\State\\Messages\\sync\\Unread"),          TEXT("Text")) // ActiveSync (##)
SDID_INFO(SDID_MSG_OTHER_EMAIL_UNREAD_SZ, CEVT_LPWSTR,  0x00000001,  TEXT("System\\State\\Messages\\OtherEmail\\Unread"),    TEXT("Text")) // Accounts (other than sync and sms) with unread mail (eg, Earthlink (5)\nAT&T (2)
SDID_INFO(SDID_MSG_LAST_RECEIVED_ACCT,  CEVT_LPWSTR,    0x00000001,  TEXT("System\\State\\Messages"),                        TEXT("Last Received Account")) // True name of the last e-mail account with activity
SDID_INFO(SDID_MSG_SMS_ACCT_SZ,         CEVT_LPWSTR,    0x00000001,  TEXT("System\\State\\Messages\\sms"),                   TEXT("Account Name")) // True name of the SMS account
SDID_INFO(SDID_MSG_MMS_ACCT_SZ,         CEVT_LPWSTR,    0x00000001,  TEXT("System\\State\\Messages\\mms"),                   TEXT("Account Name")) // True name of the MMS account
SDID_INFO(SDID_MSG_SYNC_ACCT_SZ,        CEVT_LPWSTR,    0x00000001,  TEXT("System\\State\\Messages\\sync"),                  TEXT("Account Name")) // True name of the ActiveSync account
SDID_INFO(SDID_MSG_OTHER_ACCT_SZ,       CEVT_LPWSTR,    0x00000001,  TEXT("System\\State\\Messages\\other"),                 TEXT("Account Name")) // True name of the single account in the 'other' plugin; blank if there are more than one
SDID_INFO(SDID_MSG_VMAIL_UNREAD,        CEVT_UI4,       0x00000001,  TEXT("System\\State\\Messages\\vmail\\Total\\Unread"),  TEXT("Count")) //
SDID_INFO(SDID_MSG_VMAIL_UNREAD_LINE1,  CEVT_UI4,       0x00000001,  TEXT("System\\State\\Messages\\vmail\\Line1\\Unread"),  TEXT("Count")) //
SDID_INFO(SDID_MSG_VMAIL_UNREAD_LINE2,  CEVT_UI4,       0x00000001,  TEXT("System\\State\\Messages\\vmail\\Line2\\Unread"),  TEXT("Count")) //
SDID_INFO(SDID_OWNER_NAME,              CEVT_LPWSTR,    0x00000001,  TEXT("ControlPanel\\Owner"),                            TEXT("Name")) // Need to leave in ControlPanel\Owner for legacy reasons
SDID_INFO(SDID_OWNER_PHONE_NUMBER,      CEVT_LPWSTR,    0x00000001,  TEXT("ControlPanel\\Owner"),                            TEXT("Telephone")) // Need to leave in ControlPanel\Owner for legacy reasons
SDID_INFO(SDID_OWNER_EMAIL,             CEVT_LPWSTR,    0x00000001,  TEXT("ControlPanel\\Owner"),                            TEXT("E-mail")) // Need to leave in ControlPanel\Owner for legacy reasons
SDID_INFO(SDID_OWNER_NOTES,             CEVT_LPWSTR,    0x00000001,  TEXT("ControlPanel\\Owner"),                            TEXT("Notes")) // Need to leave in ControlPanel\Owner for legacy reasons
SDID_INFO(SDID_SYNCHRONIZE,             CEVT_UI4,       0x80000002,  TEXT("System\\State\\ActiveSync"),                      TEXT("Synchronizing")) // Sync state. See SSF_* flags
SDID_INFO(SDID_LOCK,                    CEVT_UI4,       0x80000002,  TEXT("System\\State"),                                  TEXT("Lock")) // Lock state. See LSF_* flags
SDID_INFO(SDID_NOTIFY,                  CEVT_UI4,       0x00000002,  TEXT("System\\State\\Shell"),                           TEXT("Notify")) // Notify state. See NSF_* flags
SDID_INFO(SDID_MISSEDCALL_COUNT,        CEVT_UI4,       0x80000001,  TEXT("System\\State\\Phone"),                           TEXT("Missed Call Count")) //
SDID_INFO(SDID_HEADSET,                 CEVT_UI4,       0x80000002,  TEXT("System\\State\\Hardware"),                        TEXT("Headset")) //
SDID_INFO(SDID_CARKIT,                  CEVT_UI4,       0x80000002,  TEXT("System\\State\\Hardware"),                        TEXT("Car Kit")) //
SDID_INFO(SDID_SPEAKER,                 CEVT_UI4,       0x80000002,  TEXT("System\\State\\Hardware"),                        TEXT("Speaker")) //
SDID_INFO(SDID_START_MRU,               CEVT_BLOB,      0x80000001,  TEXT("System\\State\\Shell"),                           TEXT("Start MRU")) // Access with the interface defined in StartMRU.h
SDID_INFO(SDID_MRE,                     CEVT_BLOB,      0x80000001,  TEXT("System\\State\\Shell"),                           TEXT("Most Recent Event")) // Most Recent Event.  Use the MRE API to deal with this.
SDID_INFO(SDID_CATMAN,                  CEVT_BLOB,      0x80000002,  TEXT("System\\State"),                                  TEXT("Contact Category List")) // Contact category list.  Use the catman API for this.
SDID_INFO(SDID_DTMFMODE,                CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone\\Dtmf"),                     TEXT("DTMF Mode")) // Should DTMF apps play tones?
SDID_INFO(SDID_SIMSUPPORT_CAPS,         CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("SIM Support Capabilities")) // Sim Support caps
SDID_INFO(SDID_BLUETOOTH,               CEVT_UI4,       0x80000002,  TEXT("System\\State\\Hardware"),                        TEXT("Bluetooth")) // Bluetooth stack status
SDID_INFO(SDID_MSG_VMAIL_TEXT,          CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Messages\\vmail"),                 TEXT("Text")) //
SDID_INFO(SDID_MULTILINE_CAPS,          CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("MultiLine Capabilities")) // MultiLine Support
SDID_INFO(SDID_MSG_OTHER_EMAIL_UNREAD,  CEVT_UI4,       0x00000001,  TEXT("System\\State\\Messages\\OtherEmail\\Unread"),    TEXT("Count")) // Count of unread mail across all pop and imap accounts (except those that request to be excluded)
SDID_INFO(SDID_NEXT_APPT_OID,           CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments"),                    TEXT("Next")) //
SDID_INFO(SDID_NEXT_APPT_BUSYSTATUS,    CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments\\Next"),              TEXT("Busy Status")) //
SDID_INFO(SDID_NEXT_APPT_CATEGORIES,    CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\Next"),              TEXT("Categories")) //
SDID_INFO(SDID_CURRENT_APPT_OID,        CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments"),                    TEXT("Current")) //
SDID_INFO(SDID_CURRENT_APPT_SUBJECT,    CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\Current"),           TEXT("Subject")) //
SDID_INFO(SDID_CURRENT_APPT_LOCATION,   CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\Current"),           TEXT("Location")) //
SDID_INFO(SDID_CURRENT_APPT_STARTTIME,  CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Appointments\\Current"),           TEXT("Start Time")) //
SDID_INFO(SDID_CURRENT_APPT_ENDTIME,    CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Appointments\\Current"),           TEXT("End Time")) //
SDID_INFO(SDID_CURRENT_APPT_CONFLICT,   CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments\\Current"),           TEXT("Conflict")) // Values: 0 or 1
SDID_INFO(SDID_CURRENT_APPT_BUSYSTATUS, CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments\\Current"),           TEXT("Busy Status")) //
SDID_INFO(SDID_CURRENT_APPT_CATEGORIES, CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\Current"),           TEXT("Categories")) //
SDID_INFO(SDID_HOME_APPT_OID,           CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments"),                    TEXT("HomeScreen")) //
SDID_INFO(SDID_HOME_APPT_SUBJECT,       CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\HomeScreen"),        TEXT("Subject")) //
SDID_INFO(SDID_HOME_APPT_LOCATION,      CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\HomeScreen"),        TEXT("Location")) //
SDID_INFO(SDID_HOME_APPT_STARTTIME,     CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Appointments\\HomeScreen"),        TEXT("Start Time")) //
SDID_INFO(SDID_HOME_APPT_ENDTIME,       CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Appointments\\HomeScreen"),        TEXT("End Time")) //
SDID_INFO(SDID_HOME_APPT_CONFLICT,      CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments\\HomeScreen"),        TEXT("Conflict")) // Values: 0 or 1
SDID_INFO(SDID_HOME_APPT_BUSYSTATUS,    CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments\\HomeScreen"),        TEXT("Busy Status")) //
SDID_INFO(SDID_HOME_APPT_CATEGORIES,    CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\HomeScreen"),        TEXT("Categories")) //
SDID_INFO(SDID_TASKS_ACTIVE,            CEVT_UI4,       0x80000001,  TEXT("System\\State\\Tasks"),                           TEXT("Active")) //
SDID_INFO(SDID_TASKS_HIGH_PRIORITY,     CEVT_UI4,       0x80000001,  TEXT("System\\State\\Tasks"),                           TEXT("High Priority")) //
SDID_INFO(SDID_TASKS_DUE_TODAY,         CEVT_UI4,       0x80000001,  TEXT("System\\State\\Tasks"),                           TEXT("Due Today")) //
SDID_INFO(SDID_TASKS_OVERDUE,           CEVT_UI4,       0x80000001,  TEXT("System\\State\\Tasks"),                           TEXT("Overdue")) //
SDID_INFO(SDID_MSG_CURRENTLY_SYNCING_ACCT, CEVT_UI4,    0x00000001,  TEXT("System\\State\\Messages\\Syncing"),               TEXT("Syncing Account")) // Name of the currently syncing account-- none if Inbox is not syncing.
SDID_INFO(SDID_SHNOTIFICATION,          CEVT_UI4,       0x80000002,  TEXT("System\\State"),                                  TEXT("SHNotification")) // SHNotification changed
SDID_INFO(SDID_INCOMING_CALLER_NAME,    CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Incoming Caller Name"))
SDID_INFO(SDID_INCOMING_CALLID_STATUS,  CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Incoming Call ID Status"))
SDID_INFO(SDID_INCOMING_CALLER_NUMBER,  CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Incoming Caller Number"))
SDID_INFO(SDID_INCOMING_CALLER_METHOD,  CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Incoming Caller PropName"))
SDID_INFO(SDID_INCOMING_CALLER_OID,     CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Incoming Caller OID"))
SDID_INFO(SDID_INCOMING_CALLER_PROPID,  CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Incoming Caller PropID"))
SDID_INFO(SDID_TALKING_CALLER_NUMBER,   CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Caller Number"))
SDID_INFO(SDID_TALKING_CALLER_METHOD,   CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Caller PropName"))
SDID_INFO(SDID_TALKING_CALLER_OID,      CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Caller OID"))
SDID_INFO(SDID_TALKING_CALLER_PROPID,   CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Caller PropID"))
SDID_INFO(SDID_PHONE_CAPS_FLAGS,        CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("System Capabilities"))
SDID_INFO(SDID_NEXT_EVENT_OID,          CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments"),                    TEXT("Event")) //
SDID_INFO(SDID_NEXT_EVENT_LOCATION,     CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\Event"),             TEXT("Location")) //
SDID_INFO(SDID_NEXT_EVENT_STARTTIME,    CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Appointments\\Event"),             TEXT("Start Time")) //
SDID_INFO(SDID_NEXT_EVENT_ENDTIME,      CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Appointments\\Event"),             TEXT("End Time")) //
SDID_INFO(SDID_NEXT_EVENT_CONFLICT,     CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments\\Event"),             TEXT("Conflict")) // Values: 0 or 1
SDID_INFO(SDID_NEXT_EVENT_BUSYSTATUS,   CEVT_UI4,       0x80000001,  TEXT("System\\State\\Appointments\\Event"),             TEXT("Busy Status")) //
SDID_INFO(SDID_NEXT_EVENT_CATEGORIES,   CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\Appointments\\Event"),             TEXT("Categories")) //
SDID_INFO(SDID_MEDIAPLAYER_ARTIST,      CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("WM/OriginalArtist"))
SDID_INFO(SDID_MEDIAPLAYER_ALBUM,       CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("WM/AlbumTitle"))
SDID_INFO(SDID_MEDIAPLAYER_ARTISTALBUM, CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("WM/AlbumArtist"))
SDID_INFO(SDID_MEDIAPLAYER_TITLE,       CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("Title"))
SDID_INFO(SDID_MEDIAPLAYER_GENRE,       CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("WM/Genre"))
SDID_INFO(SDID_MEDIAPLAYER_RATING,      CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("UserRating"))
SDID_INFO(SDID_MEDIAPLAYER_PROTECTED,   CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("Protected"))
SDID_INFO(SDID_MEDIAPLAYER_DURATION,    CEVT_UI4,       0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("TotalDuration"))
SDID_INFO(SDID_MEDIAPLAYER_BITRATE,     CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("Bitrate"))
SDID_INFO(SDID_MEDIAPLAYER_TRACKNUM,    CEVT_LPWSTR,    0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("WM/TrackNumber"))
SDID_INFO(SDID_MEDIAPLAYER_ELAPSED,     CEVT_UI4,       0x80000001,  TEXT("System\\State\\MediaPlayer"),                     TEXT("Elapsed"))
SDID_INFO(SDID_APN_INFO,                CEVT_BLOB,      0x80000002,  TEXT("System\\State\\Connectivity"),                    TEXT("APN Info"))
SDID_INFO(SDID_RADIO_IMEI,              CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Radio IMEI"))
SDID_INFO(SDID_RADIO_MANUFACTURER,      CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Radio Manufacturer"))
SDID_INFO(SDID_RADIO_VERSION,           CEVT_LPWSTR,    0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Radio version"))
SDID_INFO(SDID_SIGNAL_STRENGTH_RAW,     CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Signal Strength Raw")) // Signal strength from 1-100, raw.
SDID_INFO(SDID_CELLULAR_SYSTEM_AVAILABLE,CEVT_UI4,      0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Cellular System Available")) // Flags indicating current cellular system coverage
SDID_INFO(SDID_HANDSFREE,               CEVT_UI4,       0x80000002,  TEXT("System\\State\\Hardware"),                        TEXT("Handsfree")) //
SDID_INFO(SDID_NEXT_ALARM,              CEVT_FILETIME,  0x80000001,  TEXT("System\\State\\Shell\\Alarms"),                   TEXT("Next")) // The next alarm set by the user
SDID_INFO(SDID_PHONE_STATUS_FLAGS_EX,   CEVT_UI4,       0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Extended Status")) // Extended phone status flags
SDID_INFO(SDID_MEMORY_RECOVERY_HINT,    CEVT_UI4,       0x80000002,  TEXT("System\\State\\Shell"),                           TEXT("MemoryRecoveryHint"))
SDID_INFO(SDID_WIFI,                    CEVT_UI4,       0x80000002,  TEXT("System\\State\\Hardware"),                        TEXT("WiFi")) // Wi-Fi state. See WSF_* flags
SDID_INFO(SDID_WIFI_SSIDS,              CEVT_UI4,       0x00000002,  TEXT("System\\State\\Connections\\Network"),            TEXT("Descriptions")) // Wi-Fi SSIDs in semicolon delimited list
SDID_INFO(SDID_CELLULAR_SYSTEM_CONNECTED,CEVT_UI4,      0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Cellular System Connected")) // Flags indicating current cellular system connected
SDID_INFO(SDID_VOIP_CURRENT_OPERATOR_NAME,CEVT_LPWSTR,  0x80000002,  TEXT("System\\State\\VoIP\\Current"),                   TEXT("Current Operator Name")) // VoIP Current Operator Name
SDID_INFO(SDID_VOIP_PHONE_NUMBER,       CEVT_LPWSTR,    0x00000002,  TEXT("System\\State\\VoIP"),                            TEXT("Phone Number")) // VoIP phone number or SIP URI
SDID_INFO(SDID_VOIP_VOICEMAIL_NUMBER,   CEVT_LPWSTR,    0x00000002,  TEXT("System\\State\\VoIP"),                            TEXT("Voicemail Number")) // VoIP voice mail number
SDID_INFO(SDID_PHONE_NETWORK_PREFERENCES,CEVT_UI4,      0x00000002,  TEXT("System\\State\\VoIP\\Phone"),                     TEXT("Network Preferences")) // Settings used to determine use of cellular or voip
SDID_INFO(SDID_PHONE_NIGHT_STARTTIME,   CEVT_FILETIME,  0x00000002,  TEXT("System\\State\\VoIP\\Phone\\Night"),              TEXT("Start Time"))
SDID_INFO(SDID_PHONE_NIGHT_ENDTIME,     CEVT_FILETIME,  0x00000002,  TEXT("System\\State\\VoIP\\Phone\\Night"),              TEXT("End Time"))
SDID_INFO(SDID_MSG_VMAIL_UNREAD_VOIP,   CEVT_UI4,       0x00000001,  TEXT("System\\State\\Messages\\vmail\\VoIP\\Unread"),   TEXT("Count")) // Number of unread voice mail for VoIP
SDID_INFO(SDID_VOIP_UPDATED_SETTINGS,   CEVT_UI4,       0x00000002,  TEXT("System\\State\\VoIP"),                            TEXT("UpdatedSettings")) // Flag used to determine what VoIP settings have been updated
SDID_INFO(SDID_VOIP_STATUS_FLAGS,       CEVT_UI4,       0x80000002,  TEXT("System\\State\\VoIP\\Current"),                   TEXT("Status")) // VoIP status flags
SDID_INFO(SDID_LKGSETTING_CALLIDBLOCKING, CEVT_UI4,     0x00000002,  TEXT("System\\State\\LKGSetting"),                      TEXT("LKGCallID")) //Last Known Good "Block Call ID" settings from network (0: show call id to everyone, non-0: show to no-one unless the override is set)
SDID_INFO(SDID_LKGSETTING_CALLWAITING,  CEVT_UI4,       0x00000002,  TEXT("System\\State\\LKGSetting"),                      TEXT("LKGCallWaiting")) //Last Known Good "Enable Call Waiting" settings from network
SDID_INFO(SDID_LKGSETTING_CALLBARRING,  CEVT_UI4,       0x00000002,  TEXT("System\\State\\LKGSetting"),                      TEXT("LKGCallBarring")) //Last Known Good "Enable Call Barring for incoming calls" settings from network
SDID_INFO(SDID_LKGSETTING_VOICEFORWARDING, CEVT_BLOB,   0x00000002,  TEXT("System\\State\\LKGSetting"),                      TEXT("LKGVoiceForwarding")) //Last Known Good "voice forwarding" data from network - value is a LKG_VOICEFORWARDING_SETTINGS struct
SDID_INFO(SDID_TALKING_CALL_STARTTIME,  CEVT_FILETIME,  0x80000002,  TEXT("System\\State\\Phone"),                           TEXT("Call Start Time"))
SDID_INFO(SDID_INTERNETSHARING,         CEVT_UI4,       0x80000002,  TEXT("System\\State\\Connectivity"),                    TEXT("InternetSharing"))  // Flags indicating IS status.
SDID_INFO(SDID_CELLULAR_NETWORK_CONTEXTS,CEVT_UI4,      0x80000002,  TEXT("Comm\\Cellular\\Ril"),                            TEXT("NetworkContexts")) // Number of context supported by the network
SDID_INFO(SDID_VOIP_SERVICE_BLOCKED,    CEVT_UI4,       0x80000002,  TEXT("Security\\Phone\\VoIP"),                          TEXT("BlockService"))    // OEM/Operator can toggle this bit to block VoIP calling service
SDID_INFO(SDID_NETWORK_TZ,              CEVT_BLOB,      0x80000002,  TEXT("System\\State\\DateTime"),                        TEXT("NetworkTime Notification"))    //NetworkTime Notification
SDID_INFO(SDID_LOCALDATE,               CEVT_FILETIME,  0x80000002,  TEXT("System\\State\\DateTime"),                        TEXT("LocalDate")) // For detecting date changes in local time
SDID_INFO(SDID_MOBILEVPN_STATUS,        CEVT_UI4,       0x80000002,  TEXT("System\\State\\Connectivity"),                    TEXT("Mobile VPN Status"))  //Mobile VPN Connection Status
#undef SDID_INFO

