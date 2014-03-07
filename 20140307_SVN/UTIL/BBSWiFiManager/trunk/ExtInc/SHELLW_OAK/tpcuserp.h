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
#ifndef _PTPCUSER_H_
#define _PTPCUSER_H_
// Copyright (c) 1999-2000, Microsoft Corp. All rights reserved.
//***   tpcuserp.h -- Private header for winuser.h TPC extensions

#ifdef __cplusplus
extern "C" {
#endif


// We don't really care about WS_CLIPCHILDREN, but the OS does 
// funky things if you pass in 0 for style.  So when we really DO want
// a 0 style, we have to pick something innocuous.
#define WS_NOSTYLE      WS_CLIPCHILDREN

// Shell notification extensions for TPC 
#define SHELLNOTIFY_SETINPUTMODEICON     0x000C
#define SHELLNOTIFY_SETCUSTOMINPUTICONS  0x000D
#define SHELLNOTIFY_ISCUSTOMINPUTICONS   0x000E

// For back-compat only
#define IMI_NONE                SHIME_MODE_NONE
#define IMI_SPELL               SHIME_MODE_SPELL
#define IMI_SPELL_CAPS          SHIME_MODE_SPELL_CAPS
#define IMI_SPELL_CAPS_LOCK     SHIME_MODE_SPELL_CAPS_LOCK
#define IMI_AMBIG               SHIME_MODE_AMBIGUOUS
#define IMI_AMBIG_CAPS          SHIME_MODE_AMBIGUOUS_CAPS
#define IMI_AMBIG_CAPS_LOCK     SHIME_MODE_AMBIGUOUS_CAPS_LOCK     
#define IMI_NUMBERS             SHIME_MODE_NUMBERS
#define IMI_CUSTOM              SHIME_MODE_CUSTOM


#if 0 // { This section is in winuserm.h. It is duplicated here to preserve some comments.

//***   VK_T* -- telephone keyboard
// DESCRIPTION
//  simulates a telephone keyboard
//  not sure what the VK_*'s will be, so we abstract them here.
//  apps should use the VK_T* names.
//
//  during the interim when we don't have phone hdwr, we fake things
//  w/ various menus/buttons/etc.  route them all thru here so that
//  the fake stuff is a) centralized and b) separated from the engine.
// NOTES
//  things marked "compat: autopc" must not be changed; they match the
// code from autopc.

#if 0
// the phone keypad maps to VK_* as shown below.
// some keys might not be present, e.g. VK_TFLIP.
    sofkey1     softkey2    VK_TSOFT1, VK_TSOFT2
            ^               VK_TUP
        <   +   >           VK_TLEFT, VK_TACTION, VK_TRIGHT
            v               VK_TDOWN
    home        back        VK_THOME, VK_TBACK
    talk        end         VK_TTALK, VK_TEND
    1       2       3       VK_T0..VK_T9
    4       5       6       ...
    7       8       9       ...
    *       0       #       VK_TSTAR, VK_TPOUND
// other buttons include
    VK_TRECORD
    VK_TPOWER, VK_TVOLUMEUP, VK_TVOLUMEDOWN
    VK_TFLIP
#endif

// semi-available: VK_F20, VK_F21 (plus the apc-reserved but unused-by-us ones)

// NOTE: The following mapping for TSOFT1 and TSOFT2 conflict with APC's VK_STAR
// and VK_POUND, but we need something that can be actuated on a CEPC, so we'll
// go with these for now... F17 and F18 used to be specified here.
#define VK_TSOFT1   VK_F1
#define VK_TSOFT2   VK_F2
#define VK_TTALK    VK_F3           // compat: autopc VK_SENDCALL
#define VK_TEND     VK_F4           // compat: autopc VK_ENDCALL
#define VK_THOME    VK_LWIN         // todo: temporary/change RHS?
#define VK_TBACK    VK_ESCAPE       // todo: temporary/change RHS?

// VK_F5..16 used by apc            // compat: autopc VK_*
// VK_F23..24 used by apc           // compat: autopc VK_*
#define VK_TACTION  VK_RETURN       
#define VK_TRECORD  VK_F10
#define VK_TFLIP    VK_F17
#define VK_TREVEALFULL          error:TBD
#define VK_TREVEALPARTIAL       error:TBD
#define VK_TPOWER   VK_F18
#define VK_TVOLUMEUP    VK_F6       // compat: autopc VK_VOLUMEUP
#define VK_TVOLUMEDOWN  VK_F7       // compat: autopc VK_VOLUMEDOWN

#define VK_TUP      VK_UP
#define VK_TDOWN    VK_DOWN
#define VK_TLEFT    VK_LEFT
#define VK_TRIGHT   VK_RIGHT

#define VK_ESROCKER VK_F20          // for GetKeyEventSource
#define VK_ESDPAD   VK_F21          // ...

#define VK_T0       L'0'
#define VK_T1       L'1'
#define VK_T2       L'2'
#define VK_T3       L'3'
#define VK_T4       L'4'
#define VK_T5       L'5'
#define VK_T6       L'6'
#define VK_T7       L'7'
#define VK_T8       L'8'
#define VK_T9       L'9'
#define VK_TSTAR    VK_F8           // !compat: autopc VK_STAR
#define VK_TPOUND   VK_F9           // !compat: autopc VK_POUND

// App keys
#define VK_APP_FIRST    0xC1
#define VK_APP1         0xC1 
#define VK_APP2         0xC2 
#define VK_APP3         0xC3
#define VK_APP4         0xC4
#define VK_APP5         0xC5
#define VK_APP6         0xC6
#define VK_APP_LAST     0xC6

#if 0 // { from autopc, 990514
// Telephony device key codes.
//
// Since these were originally based on key codes from earlier Telephony
// products at MS (Mark Moeller who is no longer at Microsoft dug these up
// I think from an earlier phone effort in MEG) we wanted to ensure that you
// guys were using the same key codes.  These shipped in Auto PC 1.0 (Apollo)
// in November 98.
#define VK_STAR         VK_F1
#define VK_POUND        VK_F2
#define VK_SENDCALL     VK_F3
#define VK_ENDCALL      VK_F4
#define VK_MUTE         VK_F5
#define VK_VOLUMEUP     VK_F6
#define VK_VOLUMEDOWN   VK_F7
#define VK_REDIAL       VK_F8
#define VK_STORE        VK_F9
#define VK_RECALL       VK_F10
#define VK_HOLD         VK_F11
#define VK_CONFERENCE   VK_F12
#define VK_TRANSFER     VK_F13
#define VK_HOOK         VK_F14
#define VK_CALLBACK     VK_F15
#define VK_SPEAKER      VK_F16
// (gap: VK_F17..VK_F22)
#define VK_SPEECH       VK_F24
#define VK_AUDIO        VK_F23
#endif // }

#endif // }

//***   messages
//
#define TAM_KEYHOLD         (WM_APP + 100)
#define TAM_HOLDDOWN        (TAM_KEYHOLD + WM_KEYDOWN - WM_KEYFIRST)
#define TAM_HOLDUP          (TAM_KEYHOLD + WM_KEYUP   - WM_KEYFIRST)
#define IDT_KEYHOLD         10              //just a unique timer ID
#define DLY_KEYHOLD         850             //KHDLY_KEYHOLD is the default of 500ms

#define LVN_HOLDDOWN        (LVN_LAST+1)    // todo: move to listview (?)

#ifdef __cplusplus
}
#endif

#endif // _PTPCUSER_H_
