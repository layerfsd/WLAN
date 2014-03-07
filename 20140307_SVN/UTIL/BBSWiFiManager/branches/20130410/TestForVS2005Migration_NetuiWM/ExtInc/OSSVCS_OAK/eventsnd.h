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
/*---------------------------------------------------------------------------*\
 *
 * Stinger Event Sounds
 *
 * (c) Copyright Microsoft Corp. 2000 All Rights Reserved
 *
 *  file: eventsnd.h
 *  module: eventsnd.cpp
 *  author: lc
 *
 *  purpose: To enable applications to play sounds related to system and
 *           application events.
 *
 *  history: 10/03/2000     LC  Created
 *
\*---------------------------------------------------------------------------*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


//pre-defined event names that can be passed to PlayEventSound
//any string will work, really, as long as there's a registry entry for it
//NOTE: these strings should not be localized!
#define EVENTSOUND_ASTERISK             TEXT("SystemAsterisk")
#define EVENTSOUND_BUSYTONE             TEXT("busytone")
#define EVENTSOUND_INCALLBUZZ           TEXT("InCallBuzz")  // replacement sound for Vibrating notification while in call
#define EVENTSOUND_USSDBUZZ             TEXT("USSDBuzz")    // similar to InCallBuzz, but this one honors silent mode
#define EVENTSOUND_CALLWAITING          TEXT("callwaiting")
#define EVENTSOUND_CLOCK                TEXT("clock")
#define EVENTSOUND_CLOCK2               TEXT("clock2")
#define EVENTSOUND_CRITICALSTOP         TEXT("SystemHand")
#define EVENTSOUND_EMAIL                TEXT("e-mail")
#define EVENTSOUND_IM                   TEXT("IM")
#define EVENTSOUND_EXCLAMATION          TEXT("SystemExclamation")
#define EVENTSOUND_MENUPOPUP            TEXT("MenuPopup")
#define EVENTSOUND_MENUCOMMAND          TEXT("MenuCommand")
#define EVENTSOUND_PASSWORD             TEXT("password")
#define EVENTSOUND_QUESTION             TEXT("SystemQuestion")
#define EVENTSOUND_REMINDER             TEXT("reminder")
#define EVENTSOUND_DEFAULTRINGTONE      TEXT("ringtone0")
#define EVENTSOUND_RINGTONEBASE         TEXT("ringtone") //this must have a line id appended to it
#define EVENTSOUND_SMS                  TEXT("sms")
#define EVENTSOUND_CELLBR               TEXT("CellBroadcast")
#define EVENTSOUND_STARTUP              TEXT("SystemStart")
#define EVENTSOUND_DEFAULT              TEXT("SystemDefault")
#define EVENTSOUND_TOOLKIT              TEXT("toolkit")
#define EVENTSOUND_VOICEMAIL            TEXT("voicemail")
#define EVENTSOUND_WARNING              TEXT("warning")
#define EVENTSOUND_CONGESTION           TEXT("congestion") // fast busy sound heard when network busy
#define EVENTSOUND_CONNECT              TEXT("connect") // for activesync when device connects or enters service area
#define EVENTSOUND_DISCONNECT           TEXT("disconnect") // activesync- on disconnect
#define EVENTSOUND_INTERRUPT            TEXT("interrupt") //activesync interruption
#define EVENTSOUND_NETWORKBEGIN         TEXT("netbegin")
#define EVENTSOUND_NETWORKEND           TEXT("netend")
#define EVENTSOUND_NETWORKINTERRUPT     TEXT("netint")
#define EVENTSOUND_INFRAREDBEGIN        TEXT("infbeg")
#define EVENTSOUND_INFRAREDEND          TEXT("infend")
#define EVENTSOUND_INFRAINTERRUPT       TEXT("infint")
#define EVENTSOUND_VOICERECBEGIN        TEXT("vrecbeg")
#define EVENTSOUND_VOICERECEND          TEXT("vrecend")
#define EVENTSOUND_RINGOUT              TEXT("ringout")
#define EVENTSOUND_LOWBATTERY           TEXT("lowbattery") // could be your standard warning, but accessibility win to make it different.  plus, the sound guy designed a really cool sound for it.
#define EVENTSOUND_1KEY                 TEXT("dtmf1") // DTMF 1
#define EVENTSOUND_2KEY                 TEXT("dtmf2") // DTMF 2
#define EVENTSOUND_3KEY                 TEXT("dtmf3") // DTMF 3
#define EVENTSOUND_4KEY                 TEXT("dtmf4") // DTMF 4
#define EVENTSOUND_5KEY                 TEXT("dtmf5") // DTMF 5
#define EVENTSOUND_6KEY                 TEXT("dtmf6") // DTMF 6
#define EVENTSOUND_7KEY                 TEXT("dtmf7") // DTMF 7
#define EVENTSOUND_8KEY                 TEXT("dtmf8") // DTMF 8
#define EVENTSOUND_9KEY                 TEXT("dtmf9") // DTMF 9
#define EVENTSOUND_0KEY                 TEXT("dtmf0") // DTMF 0
#define EVENTSOUND_ASTERISKKEY          TEXT("dtmf*") // DTMF *
#define EVENTSOUND_POUNDKEY             TEXT("dtmf#") // DTMF #
#define EVENTSOUND_KEYPRESS             TEXT("keypress") // click made when keys are pressed
#define EVENTSOUND_DIALING              TEXT("dialing")  // sound made when dialing
#define EVENTSOUND_DIALING_VOIPCALL     TEXT("DialingVoIP") //sound made when dialing a new VoIP call
#define EVENTSOUND_RINGBACK_VOIP        TEXT("RingbackVoIP") //ringback sound after dialing a VoIP call
#define EVENTSOUND_TRANSFERCOMPLETE     TEXT("TransferComplete") //sound made when a call transfer complete
#define EVENTSOUND_SHUTDOWN             TEXT("shutdown") // sound made when phone is shutting off
#define EVENTSOUND_CALLDROPPED          TEXT("calldrop") // warning that call was dropped
#define EVENTSOUND_NETWORKCALLDROPPED   TEXT("NetworkDrop") // network dropped call.
#define EVENTSOUND_ENDING               TEXT("ending")   // sound made when ending a call
#define EVENTSOUND_VOICEPRIVACYDROPPED  TEXT("VoicePrivacyDrop") // sound made when Voice privacy drop
#define EVENTSOUND_RINGPREVIEW          TEXT("RingPreview") // settings for previewing ringtone sounds
#define EVENTSOUND_DOWNLOADCOMPLETE     TEXT("DownloadComplete") // sound for IE download complete
#define EVENTSOUND_LOUDESTINCALL      TEXT("LoudestInCallVolume") //for loudest feedback sound in call
#define EVENTSOUND_STANDARDINCALL      TEXT("StandardInCallVolume") //for regular feedback sound in call
#define EVENTSOUND_LOUDEST            TEXT("LoudestVolume") //for loudest feedback sound, when not in call 
#define EVENTSOUND_STANDARD            TEXT("StandardVolume") //for regular feedback sound, when not in call

#define REG_wszSoundsScript                                 L"Script"
#define REG_wszSoundsBeepMode                               L"BeepMode"
#define REG_wszSoundsBeepModeRefCount                       L"RefCount"
#define REG_wszSoundsBeepScript                             L"BeepScript"

enum REG_SoundsBeepMode
{
    REG_sbmClicksOnly,
    REG_sbmClicksAndAllowBeepMode,
    REG_sbmClicksAndBeeps,
    REG_sbmClicksAndContinousBeeps
};

//NOTE: do not localize these strings!
//special sound "file name" indicating that the sound to play really ought to be a vibrate
#define EVENTSOUND_VIBRATE      TEXT("*vibrate*")
//special sound "file name" indicating no sound should be played
#define EVENTSOUND_NONE         TEXT("*none*")

//standard file types used to find files for sounds
#define EVENTSOUND_FILETYPES    TEXT("*.wav\0*.mid\0*.rmi\0")

// Finds and cocreates and IEventSound object for extension matching pszExt
// pszExt must have format _T(".abc")
HRESULT FindEventSoundHandler(__in TCHAR* pszExt, __deref_out IUnknown** ppunk);

//  Creates a ; delimited string of supported sound types by searching the registry
HRESULT CreateRingtoneFilterString (__out TCHAR** ppszBuffer, __out DWORD* lpcchBufSize, TCHAR chDelimiter );

//  Creates a ; delimited string of supported sound types used only for ringtones by searching the registry
HRESULT CreateRestrictedRingtoneFilterString ( TCHAR** ppszBuffer, DWORD* lpcchBufSize, TCHAR chDelimiter );

// Get the default ring tone
HRESULT GetDefaultSound( __in TCHAR* pszEvent, __out TCHAR** ppszBuffer, __out DWORD* lpcchBufSize );
HRESULT GetDefaultRing ( __out TCHAR** ppszBuffer, __out DWORD* lpcchBufSize );

// plays sound file with script
HRESULT PlaySoundScript(__in const TCHAR *pszSoundFile, __in __opt const TCHAR *pszScript, __in __opt const TCHAR *pszEvent, DWORD dwAttenCategory, HWND hwndCallback);

// plays sound file with script and lets you play synchronously or asynchronously and define whether the sound should be played
// in preview mode.
HRESULT PlaySoundScriptEx(
                         __in const TCHAR *pszSoundFile, 
                         __in const TCHAR *pszScript, 
                         __in __opt const TCHAR *pszEvent, 
                         __in __opt DWORD dwAttenCategory, 
                         __in __opt HWND hwndCallback, 
                         __in __opt BOOL fForcePreview, 
                         __in __opt BOOL fAsync);

// plays sound file once
HRESULT PreviewEventSound(__in const TCHAR *pszSoundFile, __in __opt const TCHAR *pszEvent, DWORD dwAttenCategory);
HRESULT PreviewEventSoundNotify(__in const TCHAR *pszSoundFile, __in __opt const TCHAR *pszEvent, DWORD dwAttenCategory, HWND hwndCallback);
HRESULT PreviewEventSoundEx(__in const TCHAR *pszSoundFile, __in __opt const TCHAR *pszEvent, DWORD dwAttenCategory, HWND hwndParent);

// plays event sound using associated script.  pszContact can be NULL
HRESULT PlayEventSound(__in const TCHAR *pszEvent, __in __opt const TCHAR *pszContact);

//message sent to callback hwnd when script is done playing
#define WM_USER_EVENTSOUNDDONE WM_USER + 200
// plays event sound using passed in script, posts a message to the callback hwnd when done
HRESULT PlayEventSoundScript(__in const TCHAR *pszEvent, __in const TCHAR *pszScript, HWND hwndCallback);

// stops all events within this process.
HRESULT StopEventSounds();
// stops all events in all processes.
HRESULT StopAllEventSounds();

// apps set this to true to allow DTMF tones to play when PlayDTMFTone is called and to
// suppress keyclicks (probably when focus is gained), and set it to false to return to normal 
// click mode (probably when focus is lost)
// also, the device must be in "beep" mode for keypress sounds in order for this to have an effect
HRESULT SetWantDTMFTones(BOOL bDTMF);

// play a DTMF tone for a particular key.  Plays until StopDTMFTone is called.
// the device must be in "beep" mode for keypress sounds for this call to generate anything
// (but it is not an error to call it when not in beep mode)
// This function will play a click if the key passed does not have a DTMF tone associated with it.
HRESULT PlayDTMFTone(int vKey, LPARAM lKeyData, BOOL bLoop);
HRESULT StopDTMFTones();

// attenuation for sounds.  This sets a maximum level for the various categories.  Useful for limiting
// volume while in a call, for example.  Should be called again to restore the max volume when limit
// is no longer needed.
enum Sounds_AttenuationCategory
{
    // these values are used in the registry, so don't change them!
    AttenuationCategory_Default = 0,    // class 0: affected by system volume and not played during calls
    AttenuationCategory_Override = 1,   // class 1: affected by system volume that is played during calls as well
    AttenuationCategory_Ringer = 2,     // class 2: affected by ringer volume
    AttenuationCategory_Profile = 3,    // class 3: affected by profile volume only. Not played in calls
    //class 4:  it is currently used for VoIP calls made by RTC Client, affected by system volume, not affected by the attenuation
    //class 5:  it is a place holder cetegory for the volume that is not affected by system volume, nor by attenduation
};
HRESULT StoreAttenuationLevel(DWORD dwCategory, DWORD dwMaxVolume);
HRESULT SetAttenuationLevel(DWORD dwCategory, DWORD dwMaxVolume);
HRESULT UpdateAllAttenuationLevels();
HRESULT SetNotificationAttenuationLevels();
HRESULT ResetAllAttenuationLevels();

// Increments the ring volume up or down one tick (there are 6 ticks, currently: 0 - 5)
HRESULT IncrementRingVolume(LONG lChange, __out __opt LONG *plNewVolume);
// Increments the system volume up or down one tick (there are 6 ticks, currently: 0 - 5)
HRESULT IncrementSystemVolume(LONG lChange, __out __opt LONG *plNewVolume);
// Sets the system volume level (there are 6 levels, currently: 0 - 5)
HRESULT SetSystemVolumeLevel(DWORD dwSystemVolumeLevel);

// Suppress feedback (keyclick, menus) sounds
HRESULT SuppressFeedback(BOOL bSuppress);
// Suppress feedback (keyclick, menus) sounds for the current process
HRESULT SuppressFeedbackForCurrentProcess(BOOL bSuppress);

// For initializing and closing events used by this API (called in DLL_PROCESS_ATTACH/DETACH)
HRESULT InitEventSoundEvents();
HRESULT CloseEventSoundEvents();

// Returns the default directory containing available sounds for the event
// pszDirBuff must be large enough to contain the dir (MAX_PATH)
HRESULT GetSoundsDirectory(__in_opt const TCHAR *pszEvent, __out_ecount(MAX_PATH) __notnull TCHAR *pszDirBuff);

// Returns category info for the event
HRESULT GetCategoryInfo(__in const TCHAR *pszEvent, HKEY hkeyEvent, __out_ecount(MAX_PATH) TCHAR *pszCategory, __out HKEY *phkeyCategory);

// validates whether or not a sound is playable
HRESULT ValidateSound ( HWND hwndOwner, __in const TCHAR* pszSound, __in __opt const TCHAR* pszMessage, __in __opt const TCHAR* pszTitle );

// Sets reg key for default ring
HRESULT SetAsRingtone ( __in TCHAR* pszSound );

// copy sound to the rings directory
HRESULT CopyToRingsDirectory (__in const TCHAR* pszFilePath, __out TCHAR** ppszNewFilePath, __out DWORD* lpcchNewFile );

// Returns the fully-qualified file name for the sound
// pszSoundBuff must be large enough (MAX_PATH)
// return of S_OK means path was added, S_FALSE means no path was added.
HRESULT GetFullPathSound(__in_opt const TCHAR *pszEvent, __in const TCHAR *pszSoundIn, __out_ecount(MAX_PATH) __notnull TCHAR *pszSoundBuff);

// Returns the registry file name for the sound (only includes path if path is different than the default path
// for the event).
// ppszSoundOut is a pointer to somewhere in pszSoundIn and is NOT allocated (and so should not be freed).
HRESULT GetRegPathSound(__in const TCHAR *pszEvent, __in TCHAR *pszSoundIn, __out TCHAR **ppszSoundOut);

// TRUE if pszEvent is an event, FALSE if not
BOOL IsEventSoundEvent(__in const TCHAR *pszEvent);

HRESULT IsEventSoundOff(__in const TCHAR* pszEvent, __out BOOL* pfIsEventSoundOff);

HRESULT IsEventVibrateOn(__in const TCHAR *pszEvent, __out BOOL* pfIsEventVibrateOn);

BOOL IsRingEvent(__in const TCHAR* pszEvent);

//number of volumes for UI purposes
#define NUMUIVOLUMES 5


#ifdef __cplusplus
};
#endif // __cplusplus
