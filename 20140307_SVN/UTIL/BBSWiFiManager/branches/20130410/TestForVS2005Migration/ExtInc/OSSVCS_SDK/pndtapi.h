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

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    pndtapi.h

    APIs to set up app/cmd routing for PendantBus devices

--*/

#pragma once

#include <windows.h>

//
// App codes defined by the PendantBus spec
//

#define PNDTBUS_APPID_FOREGROUND        0x00	// Send command to current foreground application.
#define PNDTBUS_APPID_SYSTEM            0x01	// System (for systemwide commands like power off)
#define PNDTBUS_APPID_HOME              0x02	// Today/Home Screen
#define PNDTBUS_APPID_START             0x03	// Start Menu
#define PNDTBUS_APPID_CALENDAR          0x04	// Calendar
#define PNDTBUS_APPID_CONTACTS          0x05	// Contacts
#define PNDTBUS_APPID_TASKS             0x06	// Tasks
#define PNDTBUS_APPID_EMAIL             0x07	// Email
#define PNDTBUS_APPID_BROWSER           0x08	// Web Browser
#define PNDTBUS_APPID_IM                0x09	// Instant Messenger
#define PNDTBUS_APPID_PHONE             0x0A	// Phone
#define PNDTBUS_APPID_DIALER            0x0B	// Dialer (if separate.  Will default to Phone if combined.)
#define PNDTBUS_APPID_CALLLOG           0x0C	// Call Log (if separate.  Will default to Phone if combined.)
#define PNDTBUS_APPID_WORDPROC          0x0D	// Word Processor
#define PNDTBUS_APPID_SPREADSHEET       0x0E	// Spreadsheet
#define PNDTBUS_APPID_NOTES             0x0F	// Note Taker
#define PNDTBUS_APPID_TSCLIENT          0x10	// Terminal Services Client
#define PNDTBUS_APPID_CALC              0x11	// Calculator
#define PNDTBUS_APPID_BEAM              0x12	// Beam Application
#define PNDTBUS_APPID_EBOOK             0x13	// Ebook Reader
#define PNDTBUS_APPID_MEDIA             0x14	// Media Player
#define PNDTBUS_APPID_RECORDER          0x15	// Voice Recorder (may be mapped to note taker).
#define PNDTBUS_APPID_SHORTCUTS         0x16	// Shortcuts
#define PNDTBUS_APPID_SYNC              0x17	// Syncronization Application
#define PNDTBUS_APPID_NEWSREADER        0x18	// Newsgroup Reader
#define PNDTBUS_APPID_MAP               0x19	// Map
#define PNDTBUS_APPID_FINANCE           0x20	// Home Finance (checkbook)
#define PNDTBUS_APPID_SLIDESHOW         0x21	// Slide Show
#define PNDTBUS_APPID_PROGRAMS          0x22	// Programs
#define PNDTBUS_APPID_SETTING           0x23	// Settings

// Reserved:
//                               0x24 - 0xEF	// Reserved

// Pendant-specific ISV applications (requires that the pendant has already sent a GUID).
#define PNDTBUS_APPID_OTHER1            0xF0
#define PNDTBUS_APPID_OTHER2            0xF1
#define PNDTBUS_APPID_OTHER3            0xF2
#define PNDTBUS_APPID_OTHER4            0xF3
#define PNDTBUS_APPID_OTHER5            0xF4
#define PNDTBUS_APPID_OTHER6            0xF5
#define PNDTBUS_APPID_OTHER7            0xF6
#define PNDTBUS_APPID_OTHER8            0xF7
#define PNDTBUS_APPID_OTHER9            0xF8
#define PNDTBUS_APPID_OTHER10           0xF9
#define PNDTBUS_APPID_OTHER11           0xFA
#define PNDTBUS_APPID_OTHER12           0xFB
#define PNDTBUS_APPID_OTHER13           0xFC
#define PNDTBUS_APPID_OTHER14           0xFD
#define PNDTBUS_APPID_OTHER15           0xFE
#define PNDTBUS_APPID_OTHER16           0xFF

//
// Command codes defined by the PendantBus Spec
//
#define PNDTBUS_CMDID_STARTAPP          0x001	// Start App	Start (bring to foreground) the app listed
#define PNDTBUS_CMDID_NEXT              0x002	// Next	Media next track, ebook next page, calendar next day, down arrow
#define PNDTBUS_CMDID_PREV              0x003	// Prev	Media prev track, ebook prev page, calendar prev day, up arrow
#define PNDTBUS_CMDID_FORWARD           0x004	// Forward	Browser forward, ebook next page, right arrow
#define PNDTBUS_CMDID_BACK              0x005	// Back	Browser back, ebook prev page, left arrow
#define PNDTBUS_CMDID_PAGEUP            0x006	// PgUp	Page Up most places, ebook prev page
#define PNDTBUS_CMDID_PAGEDOWN          0x007	// PgDown	Page Down most places, ebook next page
#define PNDTBUS_CMDID_PLAYPAUSE         0x008	// Play/Pause	Play or Pause (toggle) current media or audible ebook
#define PNDTBUS_CMDID_PLAY              0x009	// Play	Play current media (may be mapped to play/pause)
#define PNDTBUS_CMDID_PAUSE             0x00A	// Pause	Pause current media (may be mapped to play/pause)
#define PNDTBUS_CMDID_STOP              0x00B	// Stop	Stop current media or audible ebook, stop browser's rendering
#define PNDTBUS_CMDID_SHUFFLE           0x00C	// Shuffle	Set media player to play in random order
#define PNDTBUS_CMDID_REPEAT            0x00D	// Repeat	Repeat the current song or action
#define PNDTBUS_CMDID_VOLUP             0x00E	// Vol Up	Volume Up 
#define PNDTBUS_CMDID_VOLDOWN           0x00F	// Vol Down	Volume Down 
#define PNDTBUS_CMDID_MUTE              0x010	// Aud Mute	Audio Mute (turn off the speakers).  Different than Microphone Mute
#define PNDTBUS_CMDID_SCREENTOGGLE      0x011	// Screen Toggle	If screen is on, turn it off.  If screen is off, turn it on.
#define PNDTBUS_CMDID_SCREENON          0x012	// Screen On	Turn screen on.
#define PNDTBUS_CMDID_SCREENOFF         0x013	// Screen Off	Turn screen off.
#define PNDTBUS_CMDID_REFRESH           0x014	// Refresh	Browser Refresh
#define PNDTBUS_CMDID_HOME              0x015	// Home	Browser Home
#define PNDTBUS_CMDID_CUT               0x016	// Cut	Cut
#define PNDTBUS_CMDID_COPY              0x017	// Copy	Copy
#define PNDTBUS_CMDID_PASTE             0x018	// Paste	Paste
#define PNDTBUS_CMDID_UNDO              0x019	// Undo	Undo
#define PNDTBUS_CMDID_REDO              0x01A	// Redo	Redo
#define PNDTBUS_CMDID_FIND              0x01B	// Find	Start menu bring up Find Dialog, browser find
#define PNDTBUS_CMDID_SEND              0x01C	// Send	Phone send (talk), Email send message
#define PNDTBUS_CMDID_END               0x01D	// End	Phone hang up
#define PNDTBUS_CMDID_HOLD              0x01E	// Hold	Put caller on hold
#define PNDTBUS_CMDID_KEYPAD1           0x01F	// Keypad 1	Phone dial 1  
#define PNDTBUS_CMDID_KEYPAD2           0x020	// Keypad 2	Phone dial 2
#define PNDTBUS_CMDID_KEYPAD3           0x021	// Keypad 3	Phone dial 3
#define PNDTBUS_CMDID_KEYPAD4           0x022	// Keypad 4	Phone dial 4
#define PNDTBUS_CMDID_KEYPAD5           0x023	// Keypad 5	Phone dial 5
#define PNDTBUS_CMDID_KEYPAD6           0x024	// Keypad 6 	Phone dial 6
#define PNDTBUS_CMDID_KEYPAD7           0x025	// Keypad 7	Phone dial 7
#define PNDTBUS_CMDID_KEYPAD8           0x026	// Keypad 8	Phone dial 8
#define PNDTBUS_CMDID_KEYPAD9           0x027	// Keypad 9	Phone dial 9
#define PNDTBUS_CMDID_KEYPAD0           0x028	// Keypad 0	Phone dial 0
#define PNDTBUS_CMDID_KEYPADSTAR        0x029	// Keypad *	Phone dial *
#define PNDTBUS_CMDID_KEYPADHASH        0x02A	// Keypad #	Phone dial #
#define PNDTBUS_CMDID_NEW               0x02B	// New	New file
#define PNDTBUS_CMDID_ACTION            0x02C	// Action	Select list item
#define PNDTBUS_CMDID_DELETE            0x02D	// Delete	Delete item
#define PNDTBUS_CMDID_RENAME            0x02E	// Rename	Rename item
#define PNDTBUS_CMDID_FORWARDMAIL       0x02F	// Forward	Email forward message
#define PNDTBUS_CMDID_REPLY             0x030	// Reply	Email reply to message
#define PNDTBUS_CMDID_REPLYALL          0x031	// Reply All	Email reply to all 
#define PNDTBUS_CMDID_MARKREAD          0x032	// Mark Read	Mark message as read
#define PNDTBUS_CMDID_MARKUNREAD        0x033	// Mark Unread	Mark message as unread
#define PNDTBUS_CMDID_CONVREAD          0x034	// Conv Read	Mark Conversation as Read
#define PNDTBUS_CMDID_MARKALLREAD       0x035	// All Read	Mark All as Read
#define PNDTBUS_CMDID_SPELLCHECK        0x036	// Spell Check	Spell Check
#define PNDTBUS_CMDID_HELP              0x037	// Help	Help
#define PNDTBUS_CMDID_LOGIN             0x038	// Login	Log in to an instant messenger client.  Unlock the device.
#define PNDTBUS_CMDID_LOGOUT            0x039	// Logout	Log out on an instant messenger client.  Lock the device.
#define PNDTBUS_CMDID_POWEROFF          0x03A	// Power Off	Put the device to sleep.
#define PNDTBUS_CMDID_PRINT             0x03B	// Print	
#define PNDTBUS_CMDID_RECORD            0x03C	// Record	
#define PNDTBUS_CMDID_MICMUTE           0x03D	// Mic Mute	Microphone Mute (turn off the microphone).  Different than Audio Mute.

// 0x03E - 0xEFF	Reserved	Reserved for future expansion
// 0xF00 - 0xFFF	Pendant Specific	Pendant Specific ISV commands (requires that the pendant has already sent a GUID).

//
// valid actions to perform on a command mapping
//

#define PNDTBUS_CMDACTION_WINMSG       1    // PostMessage to window
#define PNDTBUS_CMDACTION_CALLDLL      2    // call DLL entry point, passing appid/cmdid
#define PNDTBUS_CMDACTION_KEYBDEVT     3    // call keybd_event

#define PNDTBUS_MAX_NAMELEN            48
#define PNDTBUS_ANY_SIZE               1

typedef DWORD   PndtCmdAction;
typedef BYTE    PndtAppID;
typedef DWORD   PndtCmdID;
typedef DWORD   (WINAPI *PFNPNDTAPPCMDHANDLER)(PndtAppID, PndtCmdID);


typedef struct tagCmdMapping
{
    PndtCmdID       ID;        // the cmd this mapping is for
    PndtCmdAction   Action;    // PNDTBUS_CMDACTION
    
    union
    {
        struct {

            UINT    uMsg;
            WPARAM  wParam;
            LPARAM  lParam;

        }   WM;             // when Action == CMDACTION_WINMSG

        struct {                                                

            TCHAR               szDllName[MAX_PATH];     
            TCHAR               szProcName[PNDTBUS_MAX_NAMELEN];    // PFNPNDTAPPCMDHANDLER

        }   DLL;            // when Action == CMDACTION_CALLDLL

        KEYBDINPUT 
            KBDEVT;         // when Action == CMDACTION_KEYBDEVT
    };

}   PNDTBUS_CMD_MAPPING, *PPNDTBUS_CMD_MAPPING;

typedef struct tagPndtCmdMapTable
{
    DWORD   dwSize;         // set to sizeof(PNDTBUS_CMD_MAPTABLE)

    // strings needed for PNDTBUS_CMDACTION_WINMSG
    TCHAR   szClassName[PNDTBUS_MAX_NAMELEN];
    TCHAR   szWindowName[PNDTBUS_MAX_NAMELEN];
    TCHAR   szExeName[MAX_PATH];        // name of app to launch to get classname/windowname created

    DWORD   dwNumEntries;   // number of entries in rgTable

    PNDTBUS_CMD_MAPPING rgTable[PNDTBUS_ANY_SIZE];

} PNDTBUS_CMD_MAPTABLE, *PPNDTBUS_CMD_MAPTABLE;

typedef struct tagPndtBinding
{
    TCHAR       szCmdMapName[PNDTBUS_MAX_NAMELEN];
    PndtAppID   AppID;

} PNDTBUS_BINDING, *PPNDTBUS_BINDING;

typedef struct tagPndtBindingTable
{
    DWORD   dwSize;         // set to sizeof(PNDTBUS_BINDING_TABLE)

    DWORD   dwNumEntries;   // number of entries in rgTable

    PNDTBUS_BINDING rgTable[PNDTBUS_ANY_SIZE];

} PNDTBUS_BINDING_TABLE, *PPNDTBUS_BINDING_TABLE;

// ***************************************************************************
//
//  Function Name: PndtBus_SetCmdMap
// 
//  Purpose: Enters a command mapping table into the PendantBus database
//           This will overwrite an existing mapping by the same name
//
//  Arguments:
//      IN        szCmdMapName        name of the command mapping
//      IN        pCmdMapTable        the command mapping table
//
//  Return Values:
//
//      E_INVALIDARG                  invalid parameter
//      E_FAIL                        error writing to database, check GetLastError
//      S_OK                          Success
//                                    GetLastError will equal ERROR_ALREADY_EXISTS
//                                    if a mapping was overwritten
//
//

HRESULT
WINAPI
PndtBus_SetCmdMap(
                    LPCTSTR                 szCmdMapName,
                    PPNDTBUS_CMD_MAPTABLE   pCmdMapTable
                 );

// ***************************************************************************
//
//  Function Name: PndtBus_GetCmdMap
// 
//  Purpose: Gets the command mapping table from the PendantBus database
//
//  Arguments:
//      IN        szCmdMapName        name of the command mapping to get
//      OUT       pCmdMapTable        the command mapping table
//      IN/OUT    puSize              IN: size of pCmdMapTable buffer
//                                    OUT: size used for table, or size needed if
//                                    pCmdMapTable is too small (ERROR_BUFFER_OVERFLOW)
//
//  Return Values:
//
//      E_INVALIDARG                  invalid parameter
//      E_FAIL                        error accessing database, check GetLastError
//      HRESULT_FROM_WIN32(ERROR_NOT_FOUND) the mapping was not found              
//      HRESULT_FROM_WIN32(ERROR_BUFFER_OVERFLOW) puSize is set to the required amount of space
//      S_OK                          Success
//
//

HRESULT
WINAPI
PndtBus_GetCmdMap(
                    LPCTSTR                 szCmdMapName,
                    PPNDTBUS_CMD_MAPTABLE   pCmdMapTable,
                    UINT *                  puSize
                 );

// ***************************************************************************
//
//  Function Name: PndtBus_DeleteCmdMap
// 
//  Purpose: Deletes the command mapping table from the PendantBus database
//           If the mapping was bound to an AppID, it is unbound.
//
//  Arguments:
//      IN        szCmdMapName        name of the command mapping to delete
//
//  Return Values:
//
//      E_INVALIDARG                  invalid parameter
//      E_FAIL                        error accessing database, check GetLastError
//      S_OK                          Success
//
//

HRESULT
WINAPI
PndtBus_DeleteCmdMap(
                        LPCTSTR szCmdMapName
                    );

// ***************************************************************************
//
//  Function Name: PndtBus_BindCmdMapToAppID
// 
//  Purpose: Activates the named CmdMap for the Application ID
//
//  Arguments:
//      IN        szCmdMapName        name of the command mapping
//      IN        BindToAppID         the App ID to bind to
//
//  Return Values:
//
//      E_INVALIDARG                  invalid parameter
//      E_FAIL                        error accessing database, check GetLastError
//      HRESULT_FROM_WIN32(ERROR_NOT_FOUND) the mapping was not found              
//      S_OK                          Success
//
//

HRESULT
WINAPI
PndtBus_BindCmdMapToAppID(
                            LPCTSTR     szCmdMapName,
                            PndtAppID   BindToAppID
                         );

// ***************************************************************************
//
//  Function Name: PndtBus_GetCurrentBindings
// 
//  Purpose: Gets the current table of AppID<>CmdMap bindings
//
//  Arguments:
//      OUT       pBindingTable       pointer to binding table         
//      IN/OUT    puSize              IN: size of pBindingTable buffer
//                                    OUT: size used for table, or size needed if
//                                    pBindingTable is too small (ERROR_BUFFER_OVERFLOW)
//
//  Return Values:
//
//      E_INVALIDARG                  invalid parameter
//      E_FAIL                        error accessing database, check GetLastError
//      HRESULT_FROM_WIN32(ERROR_BUFFER_OVERFLOW) puSize is set to size needed
//      S_OK                          Success
//
//
HRESULT
WINAPI
PndtBus_GetCurrentBindings(
                            PPNDTBUS_BINDING_TABLE   pBindingTable,
                            UINT *                   puSize
                          );

