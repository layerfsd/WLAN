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

Copyright (c) 1997-98 Microsoft Corporation

Module Name:

    wpcpriv.h

Abstract:

    Include file for Windows CE WPC private functions.

Author:

    Michael G. Toepke (mtoepke) 12-Aug-1997
    Greg Keyser (gregk)         02-Mar-1999

--*/


#ifndef __WPCPRIV_H__
#define __WPCPRIV_H__

#include "ossvcs.h"

#include "aygshell.h"
#include "shbox.h"
#include <paygshel.h>
#include <bthutil.h>
#include "execdial.h"
#include <olectl.h>

/// <file_topic_scope tref="Shell"/>

// To enable autodoc tags
//@doc

// this needs to be done one by one
// because some components UI are always defining FAREAST.
#if defined(JAPAN) || defined(CHS) || defined(CHT) || defined(KOREA)
#define BANNER_FONT_HEIGHT 9
#define DATA_FONT_HEIGHT     9
#define LABEL_FONT_HEIGHT     9
#else // !JAPAN && !CHS && !CHT && !KOREA
#define BANNER_FONT_HEIGHT  10
#define DATA_FONT_HEIGHT     8
#define LABEL_FONT_HEIGHT     8
#endif // JAPAN || CHS || CHT || KOREA

#define MAX_STARTMENU_ITEMS 7

#define MAX_SOFTKEY_TEXT    50

#ifdef __cplusplus
extern "C" {
#endif

//
//@func BOOL    | SHSetSoftKey  | Set the properties for a given soft key
//
//@parm HWND    | hwndMB    | IN - Handle to the menu/soft key bar to be modified
//@parm int     | iBttnNum  | IN - index of soft key to be modified, numbered from 0
//                              to the total number of softkeys, going left to right
//@parm HWND    | hwndDest  | IN - Handle to the window that will receive WM_COMMAND
//                              messages when the soft key is pressed; if this value
//                              is NULL then the menubar's owner is assumed to be the
//                              destination window
//@parm int     | iBttnID   | IN - ID that will be sent as part of the WM_COMMAND message
//                              to hwndDest
//@parm LPTSTR  | szBttnTxt | IN - the soft key label
//@parm DWORD   | dwFlags   | IN - Special action to take.
//                              0:              - no special action
//                              1: SK_DISABLED  - disable the softkey
//                              2: SK_STARTMENU - open the startmenu
//                              3: SK_CTXMENU   - open the context menu
//                              4: SK_FOO       - press the done or foo button
//
//@rdesc Returns TRUE upon success, FALSE upon failure
//
//@comm Soft keys notify their associated window via a mechanism similar to that of menus.
//      When a key is pressed, a WM_COMMAND message (whose wParam is the iBttnID) is sent
//      to the window specified in hwndDest.  This function simply sets up this association.
//
BOOL SHSetSoftKey (HWND hwndMB, int iBttnNum, HWND hwndDest,int iBttnID, LPTSTR szBttnTxt, DWORD dwFlags);

//
//@func BOOL    | SHLoadSKFromReg   | Set the softkeys of the argument menubar to
//                                    the values specified in the registry
//
//@parm HWND    | hwndMB    | IN - Handle to the menu bar to be modified
//@parm TCHAR*  | szRegPath | IN - The path to the registry key describing the content
//                              of the softkeys. Path starts from in the adaptability key
//                              [HKLM\Adaptability]
//
//@rdesc Returns TRUE upon success, FALSE upon failure
//
//#comm The softkeys must be specified in the registry in a specific format
//      (# refers to the softkey button number, starting from 0)
//@flag SK_LABEL_#      | a string value describing the buttons label
//@flag SK_COMMAND_#    | a dword providing the command to be posted to the application's
//                          main window
//@flag SK_FLAGS_#      | a dword whose flags can be set as described in SHSetSoftKey
//
BOOL SHLoadSKFromReg (HWND hwndMB, TCHAR* szRegPath);

//
//@func BOOL    | SHShowSoftKeys    | Show the softkeys for the specified menubar
//
//@parm HWND    | hwndMB    | IN - Handle to the menu bar whose softkeys need to be shown
//
//@rdesc Returns TRUE upon success, FALSE upon failure
//
BOOL SHShowSoftKeys (HWND hwndMB);

//
//@func DWORD   | SHRegGetHLMDWValue    | read a dword value from the specified key path,
//                                        within the HKEY_LOCAL_MACHINE reg key
//
//@parm const TCHAR[]   | tszRegKey     | IN - Key path in which value resides
//@parm const TCHAR[]   | tszRegValue   | IN - Name of registry value to retrieve
//
//@rdesc Value retrieved from registry
//

DWORD SHRegGetHLMDWValue (const TCHAR tszRegKey[], const TCHAR tszRegValue[]);

//
//@func DWORD   | SHRegGetHKCUDWValue    | read a dword value from the specified key path,
//                                         within the HKEY_CURRENT_USER reg key
//
//@parm const TCHAR[]   | tszRegKey     | IN - Key path in which value resides
//@parm const TCHAR[]   | tszRegValue   | IN - Name of registry value to retrieve
//
//@rdesc Value retrieved from registry
//
DWORD SHRegGetHKCUDWValue (const TCHAR tszRegKey[], const TCHAR tszRegValue[]);

#ifdef __cplusplus
}
#endif

//
//Shell Notification Override Mode defines
//
// 0="Everything, as stated in notifications"
// 1="Vibrate only, but respect what's in notifications" (ie. it may NOT vibrate with this checked)
// 2="Silent Night", no sound or vibration when notifications fire
#define SNOM_EVERYTHINGON   0
#define SNOM_VIBRATEONLY    1
#define SNOM_SILENT         2
#define SNOM_LAST           SNOM_SILENT


// For SHNP_ICONIC priority notifications, this flag makes the notification a
// non-dismissable status notification.
#define SHNF_ICONIC_STATUS   0x00000800


// WM_COPYDATA shell enum
typedef enum
{
    E_BTH_PIN_REQUEST = 0,    // Pin request
    E_OBEX_RECV_NOTIFY,       // Obex receive notify
    E_MAKECONNECTION          // establish network connection
} eSHELL_COPY_DATA;


// Flags to SHToolkitQueryShell
#define SHTOOLKIT_PRIORITY_HIGH              1
#define SHTOOLKIT_PRIORITY_LOW               2


//$ BUGBUG Remove this for Peregrine/Stinger if and when we determine
//         we will not ship SH3 processor.

//This is defined in PWINBASE.H in \public\common\oak\inc but you can only
//see that file if you are WINCEOEM=1.  So, we define it here if it's not
//already defined.
#ifndef SHxCOMPILERPDATABug
#ifdef SHx
#define SHxCOMPILERPDATABug int SHxCompilerPDATABug = 0
#else
#define SHxCOMPILERPDATABug (0)
#endif
#endif

/* minimum and maximum number of characters entered in a combo box for which
   we suggest completions */
#define MIN_COMPLETE 3
#define MAX_COMPLETE 40

#ifdef __cplusplus
extern "C" {
#endif


//
//@func HRESULT    | SHScanBuffer  | Scan a byte buffer for viruses
//
//@parm LPBYTE  | pbScan    | Buffer to be scanned
//@parm DWORD   | cb        | Count of bytes in pbScan
//
//@rdesc    0 indicates success (no virus detected).
//          Anything else is an error. No specific errors are defined yet.
//
//@comm If any virus scanners are present on the system this routine will call
//      each virus scanner with the specified buffer to allow the scanner to
//      check the buffer. If an error is found the scan is terminated.
//
HRESULT WINAPI SHScanBuffer(LPBYTE pbScan, DWORD cb);

//
//@func HRESULT    | SHScanFile | Scan a file for viruses
//
//@parm LPCTSTR | pszFile   | Name of file to be scanned
//
//@rdesc    0 indicates success (no virus detected).
//          Anything else is an error. No specific errors are defined yet.
//
//@comm If any virus scanners are present on the system this routine will call
//      each virus scanner to check the specified file. If an error is found
//      the scan is terminated.
//
HRESULT WINAPI SHScanFile(LPCTSTR);

//
//@func BOOL    | SHFreeScanners    | Unload the virus scanner dlls.
//
//@rdesc Returns TRUE upon success, FALSE upon failure
//
//@comm Unloads the virus scanner dlls and frees associated memory.
//      Apps should call this when they are closing.
//
BOOL WINAPI SHFreeScanners(void);

HRESULT WINAPI SHSetSimToolkitMenu(LPCTSTR pszMenu);
UINT WINAPI SHGetSimToolkitMenu(LPTSTR pszMenu, UINT cchBuf);
HRESULT WINAPI SHToolkitQueryShell(DWORD dwPrioirty, LPCTSTR lpszText);
HRESULT WINAPI SHShowSimToolkitUI();


//
//@func HRESULT | SHSoundManGetDisplayNameList  | Get the ringtone display name list
//
//@parm TCHAR * | pszBuff   | IN - Display name list
//@parm int *   | pcchBuff  | IN/OUT - number of characters in display name list
//
//@rdesc S_OK if successful. Failure hresult otherwise.
//
//@comm pszBuff is a double-null-terminated list of null-terminated strings
//      caller should put size of pszBuff in pcchBuff. Function will put actual size
//      used in pszBuff.
//      If pszBuff is NULL, pcchBuff will still be filled in with the size needed
//      (including all NULL terminators)
//
HRESULT SHSoundManGetDisplayNameList(TCHAR* pszBuff, int* pcchBuff);
HRESULT SHSoundManGetFileName(TCHAR* pszDisplayName, TCHAR* pszFileName, int* pcchFileName);
HRESULT SHSoundManGetDisplayName(TCHAR *pszFileName, TCHAR *pszDisplayName, int* pcchDisplayName);


void SHMakeValidFilename(__in_ecount(cchFilename) __notnull LPTSTR pszFilename, DWORD cchFilename);

BOOL WINAPI
SHImListPopup( HWND hwnd, UINT uFlags, int nX, int nY, RECT *prc );

HANDLE WINAPI
Shell_HeapCreate( DWORD dwOptions, DWORD dwInitialSize, DWORD dwMaxSize );

LPVOID WINAPI
Shell_Alloc( HANDLE hHeap, DWORD dwFlags, DWORD dwBytes );

BOOL WINAPI
Shell_Free( HANDLE hHeap, LPVOID pMem );

// We expect that the length of the passed in strings will be bounded by MAX_PATH.
// If we find out they are not, we will return failure.
LPTSTR WINAPI
Shell_AllocString( HANDLE hHeap, __in LPCTSTR ptszStr );

LPTSTR WINAPI
Shell_CatStrAlloc( HANDLE hHeap, __in LPCTSTR ptsz1, __in LPCTSTR ptsz2, __in __opt LPCTSTR ptsz3 );

int WINAPI
Shell_LoadStringAlloc(
    HANDLE hHeap,
    HINSTANCE hInst,
    UINT uId,
    LPTSTR *pptszName );

LPTSTR WINAPI
Shell_RegAllocString(
    HANDLE hHeap,
    HKEY hkeyRoot,
    LPCTSTR ptszPath,
    LPCTSTR ptszValue );

//
// We are relying on the bonus behavior of RegQueryValueEx under WinCE: you
// can pass a relative path in the lpReserved parameter.  If this isn't
// available, we need to redefine calls to SHRegQueryValueEx to point to a
// function which does this for us, then calls the normal ReqQueryValueEx.
//
#define SHRegQueryValueEx   RegQueryValueEx

// the OS's RegSetValueEx doesn't support the same dword * trick, so
// we provide SHRegSetValueEx to do it instead.
// the 3rd param can be the subkey name.  this way you can set values
// without calling RegOpenKey/RegCloseKey
// internally however, it will do that, so if you're going to do
// a lot of value setting, it's better to open the key yourself
LONG SHRegSetValueEx(
    IN HKEY hKey,
    IN LPCWSTR lpValueName,
    IN LPCWSTR pszSubKeyName,
    IN DWORD dwType,
    IN CONST BYTE* lpData,
    IN DWORD cbData
    );


WINSHELLAPI void ComboEditAutoComplete(HWND hwnd, WPARAM wParam);
WINSHELLAPI void SHHandleActivate(HWND hwnd, WPARAM wParam, LPARAM lParam,
                 HWND* phwndLastFocus, BOOL fSipUp, DWORD dwFlags);

// The lparam to SHForceBaseState() tells us the reason that the app is closing/resetting...
#define BASESTATE_NORMAL            0
#define BASESTATE_CLOSE_ON_RELAUNCH 1   // Owned windows are being closed when a running app is re-launched.
#define BASESTATE_SHUTDOWN          2   // The system is about to shutdown.  Don't do anything expensive but make sure you persist everything important.
#define BASESTATE_NEWITEM           99

typedef struct _BASESTATEINFO {
    DWORD   cbSize;             // size of this structure
    HWND    hwndMain;           // Window handle of main window whose owned windows are to be closed.
    WPARAM  wParam;             // wParam of FCM_CLOSEWINDOW
    LPARAM  lParam;             // lParam of FCM_CLOSEWINDOW
    BOOL    fCloseMsgBoxes;     // Should MessageBoxes be closed?
} BASESTATEINFO, *LPBASESTATEINFO;


WINSHELLAPI BOOL SHForceBaseState(HWND hwndMain, LPARAM lParam);
WINSHELLAPI BOOL SHForceBaseStateEx(BASESTATEINFO* pbsi);
WINSHELLAPI HWND SHGetLastActiveWindow(HWND hwndOwner);
//WINSHELLAPI COLORREF SHGetSysColor(int nIndex);
WINSHELLAPI BOOL SHCreateMenuBarInternal(SHMENUBARINFO *pmbi);

#ifndef SZMENUWORKERCLASS
#define SZMENUWORKERCLASS TEXT("menu_worker")
#endif

void WINAPI SHSetBack(int eOp, HWND hwnd);
enum {
    BOP_EXCLUDE,        // always exclude from stack
    BOP_INCLUDE,        // undo an exclude
    BOP_REMOVE,         // remove current copy from stack (careful!)
    BOP_INSTANCE,       // allow multiple copies in stack
    BOP_RESET,          // reset entire stack (for bootup)
};

// for BOP_INSTANCE apps (e.g. pIE), we need to simulate a WM_ACTIVATE
// start at > 100 so that 'real' WA_*'s won't ever collide
// leading "_" because these are unofficial values
#define _WA_REACTIVATE  101     // PostMsg(hwnd, WM_ACTIVATE, _WA_REACT, 0)

// a way to notify apps when an event has happened
void NotifyAppsOnEvent(HKEY hkeyRoot, LPTSTR pszKey, LPCTSTR pszMessageName, BOOL fTryLaunch);


// Private extended window style to exclude top-level windows from the Running
// Programs list.  Note that this bit is used in Win32 for WS_EX_NOPARENTNOTIFY
// which is (1) not used in WinCE and (2) not applicable to top-level windows.

#define WS_EX_EXCLUDEFROMRUNNINGLIST 0x00000004L

// private flag for SHIdleTimerResetEx. Calling with these flags
// is equivalent to calling the old SHIdleTimerReset()
#define LEGACY_SESSION (LOCK_SESSION | HOME_SESSION | FLUSH_SESSION)

//
// Private flag telling us to use pixels, not dialog coordinates, when laying out dialogs
//

#define DS_USEPIXELS        0x8000L

//
// Private messages that the shell will send to our apps
//

// Bluetooth-related messages
#define WM_BTH_PINREQUEST         (WM_USER+401)
#define WM_BTH_PIN_REPORT         (WM_USER+402)
#define WM_BTH_REMOVE_BTDEVICE    (WM_USER+403)

/// <topic name="FCM_CLOSEWINDOW" displayname="FCM_CLOSEWINDOW">
///   FCM_CLOSEWINDOW is used to close child windows when an application wants
///   to bring itself back to a base state.  This is most commonly done when
///   an app is relaunched from the start menu or home screen and it wants a fresh
///   context.  Such apps call SHForceBaseState, which calls ForceWindowsClosed. <para />
///
///   Windows that do not correctly handle FCM_CLOSEWINDOW will in the best case
///   scenario be brought to the foreground on app relaunch and interupt the user's
///   task flow.  In the worst case they will not be brought up but will keep the
///   stack from unwinding and eventually cause a stack overflow. <para />
///
///   Here are some tips when implementing a handler for FCM_CLOSEWINDOW to
///   help you avoid MTTF problems: <para />
///   <list type="bullet">
///     <item><description>
///        Do not bring up new UI when processing this message.  Before implementing a handler
///        that saves state, consider what error conditions you can have, how frequent they may be,
///        and how critical they are.  If the FCM_CLOSEWINDOW handler causes any new UI to come up
///        (such as an error message), the stack will not unwind and the error message will come
///        up at a random time (when the user has launched an app that might not seem directly
///        related to child window).  A handler should either *cancel* the UI, or should save
///        opportunistically and allow the save to fail silently.
///     </description></item>
///     <item><description>
///        SendMessage vs PostMessage: There are two schools of thought on which is better in this scenario.
///        One school believes that we should always Post closure messages to ensure that the
///        window gets a chance to finish up anything that it was in the process of doing.
///        The other school believes that we want to close the window as quickly as possible
///        to avoid a perf impact on the app that is launching.  It's not clear that SendMessage
///        closes windows any faster in general, but it is clear that mixing Post and Send in
///        a chain of child windows can result in MTTF problems.  As an example, consider a main window
///        that opens modal child window A, which in turn opens a modal dialog box B.  The FCM_CLOSEWINDOW
///        for the dialog Posts a WM_COMMAND/IDOK, but the modal child window A *Sends* WM_CLOSE.  The result
///        is that window A processes WM_CLOSE by calling DestroyWindow, which destroys B as well.  B never gets
///        to process the WM_COMMAND that it posted, and never gets cleaned up appropriately.  It has a
///        message loop that checks an hwnd, and even though the window was destroyed, the hwnd variable was
///        not cleared and the message pump never exits.  Property sheets are a prime example of windows that work in this way.
///     </description></item>
///     <item><description>
///        Enabling or disabling a parent: Message boxes and modal dialogs by default
///        disable the parent window while they are running, and SHForceBaseState
///        will check the status of the parent window before trying to close anything.
///        A modal child window that is not a dialog must disable its parent on launch
///        and enable its parent on exit.
///     </description></item>
///     <item><description>
///        If a modal child window can transition to another modal child window, be
///        careful about the sequence of enable/disable calls.  We've seen problems
///        resulting from window A transitioning into window B, but window B disables the
///        parent *before* window A enables them, so the parent is left in an enabled
///        state while child window B is still up, and window B is never forced closed.
///     </description></item>
///   </list>
/// </topic>
#define FCM_CLOSEWINDOW       (WM_USER+301)

// wParam == hwnd of new app activated
// lParam == 0
#define WM_TASKBAR_NEWTITLE (WM_USER + 100)
#define WM_TASKBAR_HWBUTTON (WM_USER + 101)

#define WM_TASKBAR_STARTUI_PASSWD (WM_USER + 102)

#define WM_TASKBAR_NEWSKIN      (WM_USER + 103)

#define WM_TASKBAR_ACTIVATETODAY (WM_USER + 104)

#define WM_TASKBAR_POWEROFF (WM_USER + 110) // no params, just do what we should do when the user does the power off thing.
#define WM_TASKBAR_VOLUMECHANGE   (WM_USER + 111) // wParam == change in volume
#define WM_TASKBAR_SHELLVERB   (WM_USER + 112) // wParam == wParam of WM_HOTKEY. lParam == lParam of WM_HOTKEY.

#define WM_TASKBAR_GETREALDPI (WM_USER + 114) // returns the "true" DPI of the shell; HIWORD = x DPI, LOWORD = y DPI.

#define WM_TASKBAR_GETTITLEAREAWIDTH (WM_USER + 116)    // returns width of text area in the title bar. Smartphone Only.

#define WM_TASKBAR_GETTITLEFONT (WM_USER + 117)         // returns HFONT of text in title bar. Smartphone Only.

// App close options (WPARAM of WM_TASKBAR_CLOSE_APPS)
typedef enum  {
    ACO_INVALID = -1,                               // Invalid option.
    ACO_CLOSE_ALL_EXCEPT_MRU_AND_MRU2 = 0,          // Close all freeable apps, except MRU & MRU2
    ACO_CLOSE_ALL_EXCEPT_MRU,                       // Close all freeable apps, except MRU
    ACO_CLOSE_ALL,                                  // Close all freeable apps
    ACO_LAST = ACO_CLOSE_ALL
} APP_CLOSE_OPTIONS;


// Used in place of a WM_CREATE
// No longer used. Now used WM_CREATE
//#define WWM_WORKERCREATE     (WM_USER + 101)


// added by bwhull 7/23/01
  //#define WM_CTXMENU_CLOSE (WM_USER + 121)
  //#define WM_MENU_OPEN (WM_USER + 119)
  //#define MM_SETSELECTED (WM_USER + 120)
 // added for merlin softkeys
#define IDM_SK_STARTMENU (WM_USER + 122)
#define IDM_SK_FOO (WM_USER + 113)
#define IDM_SK_CTXMENU (WM_USER + 114)
  // for the today screen softkeys
#define TODAY_STARTCALENDAR (WM_USER + 115)
#define TODAY_STARTMAIL (WM_USER + 116)
#define TODAY_STARTTASKS (WM_USER + 117)
#define TODAY_STARTCONTACTS (WM_USER + 118)

// wParam is the tab
#define WM_TASKBAR_LAUNCHCONNSETTINGS (WM_USER + 122)

// a network connection has changed (used by NETUI)
#define WM_TASKBAR_NETCONNCHANGED (WM_USER + 123)

// a Wi-Fi adapter has been detected
#define WM_TASKBAR_WIFIADAPTERREADY (WM_USER + 124)
#define WM_TASKBAR_WIFICONNECTING   (WM_USER + 125)
#define WM_TASKBAR_ETHERNETCHANGED  (WM_USER + 128)

// a message sent to the shell so that it can react to a
// setting change before the rest of the system.
#define WM_TASKBAR_SETTINGCHANGE (WM_USER + 129)

// a message sent to the shell from clock.exe to get the
// shell to refresh the LIMITEDCLOCK variable from the Registry
#define WM_TASKBAR_LIMITEDCLOCK  (WM_USER + 130)

// a message sent to the shell to refresh the taskbar
#define WM_TASKBAR_REFRESH       (WM_USER + 131)

// a message sent to the shell when ActiveSync connects or disconnects
// so that the data icon can be refreshed.
#define WM_TASKBAR_CRADLED    (WM_USER + 133)

// a message sent to the shell by the notification subsystem to tell the shell
// to stop playing sound.
#define WM_TASKBAR_STOPEVENTSOUND   (WM_USER + 134)

// a message sent to the shell to determine if an app is freeable or not
#define WM_TASKBAR_IS_FREEABLE_PROCESS   (WM_USER + 135)

// NOTE: Some of the WM_USER offsets are taken by sh32cmn.h!
// For example, WM_USER + 126 is not available.

// returns TRUE if the welcome app is running
#define WM_TASKBAR_ISWELCOME    (WM_USER + 136)

#define WM_TASKBAR_BTHUTIL_MESSAGE (WM_USER + 137)

// Posted to taskbar to remove all windows from a certain process from its tasklist
// lParam = process id
#define WM_TASKBAR_REMOVE_PROCESS_WINDOWS (WM_USER + 138)

// sent to shell by WZC manager
#define WM_TASKBAR_LAUNCHWIFISETTINGS (WM_USER + 139)

// Dismiss notification icons
#define WM_TASKBAR_DISMISSNOTIFICATIONICON (WM_USER + 140)


// Retrieve the alpha value for taskbar & SoftKeyBar transparency (PPC Only)
#define TSKBM_GETALPHA (WM_USER + 150)
// wParam == 0 (This parameter is not used)
// lParam == 0 (This parameter is not used)
// Return value:The return value is the current alpha value, default is 255 (opaque).

// Set the alpha value for taskbar & menubar transparency (PPC Only)
#define TSKBM_SETALPHA (WM_USER + 151)
// wParam == alpha value (0 - 255) 0 == transparent, 255 == opaque
// lParam == 0 (This parameter is not used)
// Return value: The return value is undefined


// NOTE: Some of the WM_USER offsets are taken by ShellMessages.h!
// For example, WM_USER + 132 is not available.

//
// Private SHSipPreference flags.  These are continuations of the SIPSTATE enum.
// that's defined in the public AYGSHELL.H file.  Careful here.
//
#define  SIP_INPUTDIALOGINIT ((SIPSTATE)5)
#define  SIP_DOWN_NOVALIDATE ((SIPSTATE)6)

//
// Private flags in the fdwFlags field of SIPINFO.
// The public flags are defined in sipapi.h in the OS.
//   SIPF_GETCACHEDINFO - Get cached SIPINFO in shell to prevent calling SipGetInfo.
//
#define SIPF_GETCACHEDINFO    0x80000000

//
// Private SHRecognizeGesture flag.  Currently not used by anyone
//
#define  SHRG_MODELESS        0x00000004

//
// Related, but different, context menu notifications.  Sent to the app just
// before TrackPopupMenu() is called.  See SHTrackPopupMenu()
#define  CMN_INITMENUPOPUP 1100


// BUGBUG  This is no longer used, and should be removed.  Supporting
// code in SIPPREF.C should also be nuked.  Too close to RTM to touch
// SIPPREF.C.  This change just moves it here from AYGSHELL.H.
#define SIPM_SUBCLASSCHILD    (WM_USER + 1) // wParam == TRUE/FALSE to subclass/unsubclass the lParam lParam == hwnd to subclass/unsubclass


//
// Internal notification property sheet sends to subdialogs
//
#define PSN_INITCOMPLETE        (0U-250U)



typedef struct tagTOOLBARINFO {
    short iBitmap;
    unsigned short idCommand;
    short fsState;
    short fsStyle;
    unsigned short idString;
    unsigned short idToolTip;
    // use indices because it's faster
    unsigned short indexMenu;
} TOOLBARINFO;


typedef struct tagNMCONTEXTMENU {
    NMHDR hdr;
    POINT ptAction;
    HMENU hMenu;
    UINT  uID;
} NMCONTEXTMENU, *PNMCONTEXTMENU;

//For context menu's in listviews, return this flag when processing
//GN_CONTEXTMENU to have the listview deselect the item that was clicked
//on.
#define  LVRF_DESELECTITEM    100

typedef struct
{
    RECT rcBubble;
    POINT ptSource;
    DWORD dwAlign;
    BOOL fCritical:1;
    HWND hwnd;
} BUBBLEREGIONINFO;

typedef struct tagGRADIENTTITLEINFO
{
    HDC hdcMem;
    HWND hwnd;
    HFONT hFont;       //NULL to have calculated for you
    int iTitleHeight;  //0 to have calculated for you
    int iStemHeight;  // 0 if you don't know, then you'll get a "default"
    COLORREF clrLeft;  //0 to have calculated for you
    COLORREF clrRight; //0 to have calculated for you
    BOOL fCritical;
} GRADIENTTITLEINFO, *PGRADIENTTITLEINFO;

void SHSetBubbleRegion(BUBBLEREGIONINFO *pbri);
#define BUBBLEFRAME_OPAQUEFILL  0x0000
#define BUBBLEFRAME_MASK        0x0001
#define BUBBLEFRAME_WIREFRAME   0x0002
void SHPaintBubbleFrame(BUBBLEREGIONINFO *pbri, HDC hdc, DWORD dwFlags);

WINSHELLAPI HWND SHCreateWorkerWindow(TCHAR *pwszClass /* NULL accepted (preferred) */,
                                      WNDPROC pfnWndProc, HWND hwndParent,
                                      DWORD dwExStyle, DWORD dwFlags, TCHAR *pwszTitle,
                                      HMENU hmenu, void * p);
HWND SHCommandBar_GetCommandBarByID(HWND hwndCommandBands, int id);
void SHCommandBar_EnableCommand(HWND hwndCommandBands, int idCmd, BOOL fEnable);
void SHCommandBar_GetClientRect(HWND hwndMain, HWND hwndCommandBands, LPRECT prc);
int  SHMessageBox(HWND hwndParent, LPCTSTR lpszText, LPCTSTR lpszTitle,
                  UINT uFlags, int iDefault);
WINSHELLAPI BOOL SHHandleSipChange(HWND hwnd, WPARAM wParam, LPARAM lParam, BOOL* pBool);
/*PRIVATE*/BOOL SHCheckForContextMenu(HWND hwnd, LPARAM lParam, BOOL fComboboxEdit);
/*PRIVATE*/void DoEditContextMenu(HWND hwnd, BOOL fComboboxEdit);
/*PRIVATE*/LRESULT SendNotify(HWND hwndTo, HWND hwndFrom, int code, NMHDR* pnmhdr);

/* Provide access to the sub-class procs, so applications can benefit from
   this functionality without having to take the overhead of creating the
   sniffer window */

void SubClassThisWindow(HWND hwnd, BOOL f);

LRESULT CALLBACK EditSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CapeditSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ComboSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ComboBoxEditSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
/*
LRESULT CALLBACK ButtonSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TTButtonSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BoxSelectSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
*/
LRESULT CALLBACK DateTimeSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void CancelSIPUp(HWND hwndCombo);

/*PRIVATE*/WINSHELLAPI HWND SHCreateMainWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName,
                   DWORD dwStyle, HWND hWndParent, HMENU hMenu, HANDLE hInstance,
                   LPVOID lpParam);
/*PRIVATE*/UINT SHShowContextMenu(HMENU hPopupMenu, UINT uID, HWND hwnd, HWND hwndCtl,
    UINT uFlags, LPPOINT lppt);
/*PRIVATE*/UINT SHCreateContextMenu(HINSTANCE hInst, UINT uID, HWND hwnd, HWND hwndCtl,
                   UINT uFlags, LPPOINT lppt);
/*PRIVATE*/BOOL SHLoadMenuExtensions(IUnknown *punkOwner, LPCTSTR pszContext, LPCTSTR pszClass,
                    HANDLE *phExtensions);
/*PRIVATE*/HRESULT SHQueryMenuExtensions(HANDLE hExtensions, HMENU hmenu, UINT indexMenu,
                    UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
/*PRIVATE*/void SHEnableEditMenu(HWND hwnd, HMENU hMenu, BOOL fValidateHwnd);
/*PRIVATE*/HWND SHFindForegroundMenuBar();
/*PRIVATE*/HWND SHFindMenuBarInternal(HWND hwnd, BOOL fAllowHidden, __inout_opt HWND * phwndPrevious);

// NOTE: Do not use SHRunCpl unless you know what you're doing.  If you want to launch
//       a control panel applet, use SHRunSafeApplet() defined below.
//
/*PRIVATE*/DWORD SHRunCpl(LPTSTR pszCmd);
//

/*PRIVATE*/void SHSetWindowBits(HWND hWnd, int iWhich, DWORD dwBits, DWORD dwValue);
/*PRIVATE*/BOOL IsFullScreenWindow(HWND hwnd, LPARAM lParam);
/*PRIVATE*/DWORD SHGetKOBits(int vKey, DWORD dwBits);
/*PRIVATE*/void SHSetKOBits(int vKey, DWORD dwNewBits, DWORD *pdwCurrentBits);
/*PRIVATE*/void SHTurnScreenOn(BOOL bTurnOn);


// SHEnableRadio
//
//  enables the radio for the caller.
//  when the radio is already enabled, this is a NOP.
//
//  if the radio is not ready to go, SHEnableRadio will fail
//  and it may prompt the user to....
//  1) to insert the radio and/or power it on via a bubble
//      parented to hwndParent
//  2) enter the SIM PIN via the SIMSecurity dialog parented
//      to hwndSIMCheckParent
//
//  after prompting the user to correct the situation, SHEnableRadio
//  will still fail and notify the user via a bubble parented to
//  hwndParent unless SHERF_NO_STATUS_UI is set.
//
//  return values:
//      TRUE:  radio is enabled and ready to go
//      FALSE: radio is not enabled.
//
#define SHERF_WAITFOR_READYSTATE_ALL        0x00000001
#define SHERF_WAITFOR_REGISTRATION          0x00000002
#define SHERF_NO_STATUS_UI                  0x00000004
#define SHERF_DISABLE_FLIGHTMODE_NOUI       0x00000008  // disables flight mode with no UI.  doesn't check SIM PIN or other states.
BOOL SHEnableRadio(HWND hwndSIMCheckParent, HWND hwndParent, DWORD dwSHERFlags, DWORD dwSimStyle);


// Loads a font from a resource file.  Assumes the font is of the format:
// [fontsize],[fontweight],[font family]
// For example: Bold 8-point Tahoma = "8,700,Tahoma"
//                     Normal 9-point system font = "9,400,"
//                     Bold font = ",700,"
//
// This function creates the font and returns it.
// The caller must call DeleteObject on this font.
HFONT SHLoadFontFromResource(HINSTANCE hinst, UINT idsFont);

//
// Draws some text on the screen over a previously drawn image.
// Specifics about the text to be dtawn are contained in three resources:
//      1) idsLocation - The (x,y) location in the DC of the text
//      2) idsText - The text itself
//      3) idsFont - The font to be used in drawing the text (format: font height,font weight,font name)
//
// nBitmapWidth and nBitmapHeight give information about the width and height of the image.
// that the text is being drawn over.  xPos and yPos tell the position of the bitmap
// in the DC.  crTextColor tells which color the text should be drawn.
//
void SHDrawTextOverImage(HINSTANCE hinst, HDC hdc, int xPos, int yPos, UINT nBitmapWidth, UINT nBitmapHeight, UINT idsLocation, UINT idsText, UINT idsFont, COLORREF crTextColor);


//
HWND SHGetLegacySupportWindow();

void SHReleaseLegacySupportWindow();

HMENU SHLoadMenuPopup(HINSTANCE hinst, UINT id);

/// <summary>
/// Append the processor-specific path, including the autorun executable
/// filename, to a storage card location string.
/// <seealso cref="SHGetAutoRunPath"/>
/// </summary>
/// <param name="pszBuffer">
/// A buffer that contains the storage card location string. Note that the
/// string should NOT include a terminating backslash. The subfolder path to
/// the autorun executable will be appended to this buffer.
/// </param>
/// <param name="cchBuffer">
/// The number of characters in <paramref name="pszBuffer"/>.
/// </param>
/// <returns>
/// If successful, returns the total number of characters in the string, not
/// including the terminating null. If the buffer is not large enough, the
/// return value is the number of characters needed in the buffer to contain
/// the entire string, including its terminating null. All other failures
/// return zero.
/// </returns>
DWORD SHAppendAutorunPath(LPTSTR pszBuffer, DWORD cchBuffer);

// protecting this in case someone is pulling in shlwapi w/ it's A/W versions in which case
// the StrStrI becomes a macro
#ifndef StrStrI
LPTSTR StrStrI(LPCTSTR lpFirst, LPCTSTR lpSrch);
#endif
void SHFillRectClr(HDC hdc, LPRECT prc, COLORREF clr);
BOOL SHColorDisplay();
UINT SHTrackPopupMenu(HMENU hmenu, UINT uFlags, int x, int y, RECT *rcEat, HWND hwnd, LPCRECT prc);
void SHDrawUnderline(HDC hdc, int x,int y, int iWidth);
void SHDrawUnderlineColor(HDC hdc, int x,int y, int iWidth, COLORREF clr);
BOOL IsSANMessage(UINT uMsg);
int LFHeightForPoint( HDC hdc, int iPoint);
BOOL SHIsPreRapierApp(HWND hwnd);
BOOL IsModulePreWinCE421(HMODULE hModule);

BOOL GetProtocol(LPCTSTR pszPath, LPTSTR pszProtocol, int cchProtocol);

#define SHGTF_AUTO 0 /* automatically choose either the time or the date */
#define SHGTF_TIME 1 /* Force the time to be returned, even if not today */
#define SHGTF_DATE 2 /* Force the date to be returned, even if today */
BOOL SHGetTimeFormat(LPTSTR lpText, const SYSTEMTIME *lpSt, int size, DWORD grfFlags);



// used to check to see if a bold font is allowed in the system
BOOL SHBoldFontAllowed();

// equivalent to the SetForegroundWindow((HWND)((DWORD)hwnd | 0x01)) hack
void SHSetForegroundLastActivePopup(HWND hwnd);

// this is like InvalidateRect except it invalidates the screen
// cross window boundaries.
void SHInvalidateScreen(LPRECT prc);


// Result values for SHGetScreenOrientation
typedef enum _SHSO {
    SHSO_PORTRAIT = 0,
    SHSO_LANDSCAPE,
    SHSO_SQUARE
} SHSO;

// Determine the mode in which the screen is currently running.
// [out] one of the SHSO_xxx flags declared above
UINT WINAPI SHGetScreenOrientation();

// Determine if the raised SIP might block screen elements in non-portrait orientation.
BOOL SHSipMightBlockUI();

// Determine if a dialog should skip setting its default focus.
BOOL SHSkipDialogInitialFocus();

//
// The following enumeration defines the possible options
// for SHSetDisplayRotation.
//
typedef enum _SHDR
{
    // The following constants are used to indicate specific
    // screen orientation angles.
    // These are the only values returned from
    // SHGetDisplayRotation.
    SHDR_PORTRAIT_NORMAL,   //   0 degrees
    SHDR_LANDSCAPE_RIGHT,   //  90 degrees
    SHDR_PORTRAIT_INVERTED, // 180 degrees
    SHDR_LANDSCAPE_LEFT,    // 270 degrees

    // The following three constants are used to set the rotation
    // according to the user's "preferred" orientation.
    // For landscape mode, a user can set right- or left-
    // handed. We remember this preference for SHDR_TOGGLE
    // and SHDR_LANDSCAPE.
    // (It is not possible to set SHDR_PORTRAIT_INVERTED as
    // a preference, so SHDR_PORTRAIT is always equivalent to
    // SHDR_PORTRAIT_NORMAL.)

    // SHDR_PORTRAIT sets the orientation to the preferred
    // portrait mode. (Currently normal (0 degrees) only.)
    SHDR_PORTRAIT,

    // SHDR_LANDSCAPE sets the orientation to the preferred
    // landscape mode. (Right (90) or Left (270))
    SHDR_LANDSCAPE,

    // SHDR_TOGGLE toggles between preferred portrait
    // and preferred landscape modes.
    SHDR_TOGGLE
} SHDR;

HRESULT SHSetDisplayRotation(DWORD dwRotation);
HRESULT SHGetDisplayRotation(DWORD *pdwRotation);

#define SHDRF_LANDSCAPE_FIXED       0x01
#define SHDRF_HIDE_ORIENTATION_UI   0x02

HRESULT SHGetLandscapeRotationSettings(DWORD * pdwRotation, DWORD * pdwFlags);


#define SHARF_IMAGEFILE         0x00001
#define SHARF_IMAGERES          0x00002
#define SHARF_CLIENTSTARTRECT   0x00004 // specifies that the rcStart is in in client coordinates
#define SHARF_CLIENTFINISHRECT  0x00008 // specifies that the rcFinish is in in client coordinates
#define SHARF_SYNCHRONOUS       0x00010 // specifies that the animation happens synchronously
#define SHARF_BITMAP            0x00020 // specifies that the hdc field contains a bitmap for the animation
#define SHARF_BMPGROW           0x00040 // Bitmap animation "grows"
#define SHARF_BMPUNCOVER        0x00080 // Bitmap animation "uncovers"
#define SHARF_INCLUDELAST       0x00100 // include the last frame in the animation
#define SHARF_ANIMATETIME       0x00200 // dwAnimateTime contains valid data
#define SHARF_USEPARENT         0x00400 // hwnd should be used as the parent window for the animation

typedef struct
{
    HWND hwnd;
    RECT rcStart;
    RECT rcFinish;
    DWORD dwFlags;
    DWORD dwAnimateTime; // duration of animation in MS

    LPCTSTR pszResID;
    HINSTANCE hinst;
    LPTSTR pszFile;
    HBITMAP hbmAnimate;
} SHANIMATEINFO;

HRESULT SHAnimateRects(SHANIMATEINFO *pshai);

// convenience api to animate from listview open.
// animates from iItem in hwndLV (if iItem == -1, uses focused item)
// and grows to hwndFinal
void SHAnimateListviewOpen(HWND hwndFinal, HWND hwndLV, int iItem);

//From winddi.h for now.  Uh, is this supported?
#ifndef CLEARTYPE_QUALITY
#define CLEARTYPE_QUALITY       5
#endif
#define SHCSF_BOLD          0x0001
#define SHCSF_CLEARTYPE     0x0002
#define SHCSF_HOTLINK       0x0004
HFONT SHCreateSystemFont(HWND hwnd, int iHeight, DWORD dwFlags);

#define SHGFH_BANNER        0x0001
#define SHGFH_DATA          0x0002
#define SHGFH_LABEL         0x0003
int SHGetFontHeight(UINT uType);

LCID SHGetSystemDefaultLCID(void);

#define IsJPN() (PRIMARYLANGID(LANGIDFROMLCID(SHGetSystemDefaultLCID()))==LANG_JAPANESE)
#define IsCHN() (PRIMARYLANGID(LANGIDFROMLCID(SHGetSystemDefaultLCID()))==LANG_CHINESE)
#define IsKOR() (PRIMARYLANGID(LANGIDFROMLCID(SHGetSystemDefaultLCID()))==LANG_KOREAN)
#define IsFE()  (IsJPN() || IsCHN() || IsKOR())

//***   LoadStringEtcOver -- LoadStringEtc, w/ OEM .reg override
//
// Note: This API is insecure. Strings loaded by this API that are used as sprintf format
//       strings can be exploited.
//
const TCHAR *LoadStringEtcOver(HINSTANCE hinst, UINT idsStr);

//***   LoadStringEtcOverNoCache -- LoadStringEtc, w/ OEM .reg override and no caching
//
// 1. Returned strings from LoadStringEtcOver may be NULL in out-of-memory conditions
// 2. Returned string must be deallocated by the caller using LocalFree.
//
// Note: This API is insecure. Strings loaded by this API that are used as sprintf format
//       strings can be exploited.
//
const TCHAR *LoadStringEtcOverNoCache(HINSTANCE hinst, UINT idsStr);

#define FILE_ATTRIBUTES_STORAGECARD  (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_TEMPORARY)

///////////////////////////////////////////////////////////////////////////////
// VOICEBAR and RECORDING
//
// Starting with Rapier, voice recording is now much more complex.  We can
// record to multiple destinations (file, memory), set upper limits on the
// size, and be triggered from other processes.  To handle this complexity,
// when initiating a recording through VWM_RECORD, we send the following
// parameters:
//
//      dataType    (EFILE, EMEMORY, etc)       type eFILE_DEST
//      fileDest    (EFD_LOCAL_MEM, EFD_CARD1)  type eFILE_DEST
//      maxSize     (0 bytes, MAXDWORD, 40000 bytes, etc)
//      this window
//
// This tells HotVoice (which is running in a separate task) everything it
// needs to know, including who to inform when recording is done and the data
// need to be moved to a final location.
//
// Some compromises need to be made when squeezing this data into two DWORD
// values -- Max size is shifted down 8 bits, which has the dual effects of
// rounding down and chunking the allowed sizes to multiples of 256 bytes.
//
// To assist this procedure, several macros are provided.  The first macro,
// MAKE_VBAR_LPARAM, squeezes the first 3 values into a single 32-bit (DWORD)
// value.  The other three macros extract (and cast) the original values from
// the composite LPARAM.

#define MAKE_VBAR_LPARAM(dataType, fileDest, maxSize) \
   ((LPARAM)                                          \
    ( ( ((DWORD) ((BYTE)(dataType)) ) << 28) |        \
      ( ((DWORD) ((BYTE)(fileDest)) ) << 24) |        \
      ( (        (DWORD)(maxSize)   ) >> 8)           \
    )                                                 \
   )

#define VBAR_LPARAM_DATATYPE(lParam)  ((eDATA_TYPE) ( ((DWORD)(lParam) >> 28) & 0xF ))
#define VBAR_LPARAM_FILEDEST(lParam)  ((eFILE_DEST) ( ((DWORD)(lParam) >> 24) & 0xF ))
#define VBAR_LPARAM_MAXSIZE(lParam)   (((DWORD)(lParam) & 0x00FFFFFF) << 8 )


// This defines the TYPE of data that we have collected, or are about to
// collect.  In some places, this value will be passed through COM, where
// it converted to a DWORD, so casting may be used.
// NOTE: some of these values may not currently be supported (in particular,
// IStreams are only a possible future addition).
// WARNING: This cannot execeed ONE BYTE (we're limited to values 0 - 15)
typedef enum
{
    ENONE,          // invalid value - variable not set
    EFILE,          // file    used for data I/O
    ESTREAM,        // IStream used for data I/O
    EMEMORY,        // a simple memory array (*may* be a memory-mapped file)
    EFILE_BUFFERED, // EFILE, but initially buffered to memory (see below)
    ELAST_ENTRY     // this must always be the last entry
} eDATA_TYPE;
// EFILE_BUFFERED is an internal-only flag (if users try to specify this value
// it will be treated as EFILE).  We *may* decide that this data needs to be
// buffered to memory before being sent to a file.  This will be transparent
// to callers, who will see behavior consistent with EFILE.


// When we record EFILE data, where exactly should it be saved?  This value
// gives more details on where that data will be initially stored.  For Rapier
// we only support one storage card.  Future additions include EFD_CARD 2, 3,
// etc.
// WARNING: This cannot execeed ONE BYTE (we're limited to values 0 - 15)
typedef enum
{
    EFD_LOCAL_MEM = 0,// Record to object store in main memory only
    EFD_CARD1,        // If card 1 is inserted record to it; otherwise main mem
    EFD_DEST_LAST,    // Last (sequential) entry in the list
    EFD_REGISTRY  =15 // Use the currently registered default choice
} eFILE_DEST;

typedef struct _tagDATALOC
{
    eDATA_TYPE  dataType;
    DWORD       size;
    TCHAR       szRecording[MAX_PATH];
} DATALOC, * PDATALOC;


//_____________________________________________________________________________
//
// Defined windows messages
//_____________________________________________________________________________

// szHotRecording
//
// This message is sent to the top-level window whenever the voice hotkey is
// pressed.  If the top-level window cannot handle voice recordings, it should
// return the normal default value of 0 (in which case, we'll record a file,
// and perhaps switch to the "Notes" application).  If the top-level app IS
// able to handle voice, then the non-zero return value will specify
// information such as where to place the data (in main object memory, on a
// storage card, etc.) and the maximum allowed size for the recording.
//
// Rich Ink controls (inkx.h) may be created with embedded voice capabilities.
// To exploit this and support automatic hot-key recording in a rich ink
// control, the top-level window must pass this message on to the appropriate
// control.  Here is code to accomplish that:
//
// First, this messages needs to be registered, as with:
//
//     UINT ulHotRecording = RegisterWindowMessage(szHotRecording);
//
// Then in the default section of WinProc, the registered message should be
// detected and passed through to the rich ink control:
//
//     if (message == ulHotRecording)
//     {
//         return_value = SendMessage (hWndRichInk, message, 0, 0);
//         if (return_value != 0)
//         {
//             optional code to make the voice bar visible in the ink control
//         }
//     }
//
// This message is redundantly defined in several include files.
#ifndef  szHotRecording
#define  szHotRecording  TEXT("WM_HOTRECORDING")
#endif //szHotRecording
// Additional notes: (not yet publicly supported)
// The value returned to szHotRecording should be MAKE_VBAR_LPARAM -- a
// non-zero value that specifies everything HotVoice needs to know when
// and if a recording is started.  That macro is defined elsewhere in this
// file.




// This is the leading portion of the filename for all file-based voice
// recordings.  These may be made to the on-device memory (typically in the
// form "\temp\~VRec_23.wav") or on the storage card (typically something
// like "\Storage Card 1\~VRec_23.wav").  If we are in the NOTES application
// it will take this file immediately after recording and move/rename it --
// moving it to an appropriate folder/directory, and renaming it to an
// appropriate localized name like "Recording17.wav" (this move should be
// very efficient, since it's only really a rename -- we aren't transporting
// across media).
//   BUT, if the recording was made without launching the NOTES app, then
// the file still lingers in its initial temporary location, with its oddball
// temporary name.  When NOTES first starts, it must search all possible
// temp locations to find these hidden files, perform the move/rename.
//   To speed the FindFile, we begin with an uncommon character.
#define szTempRecFile   TEXT("~VRec_")


typedef enum tagVoiceMessages {
    VWM_OPTIONS_CHANGED = WM_USER+1000, // user changed options
    VWM_RECORD,                         // Record.  Old (Gryphon/Wyvern) apps will call
                                        //   with wParam:0 lParam:0, new apps will call
                                        //   with wParam:hwnd lParam:MAKE_VBAR_LPARAM
                                        //   (see "VOICEBAR and RECORDING")
    VWM_STOP,                           // user wants to stop record
    VWM_STATUS,                         // OLD VOICECTL: Status Update
    VWM_STATUS_ONLY,                    // OLD VOICECTL: Status Update w/o bring to front
    VWM_ERROR,                          // Error Occured
    VWM_SET_RECORD_FILE ,               // Set record destination file
    VWM_POWER_OFF   = WM_USER+ 1010,    // Came back from Power OFF
    VWM_RECORDSTOP,                     // HotVoice received HotKey up, informing target window
    VWM_SET_DISABLE_APP_SWITCH,         // user wants to set the value for m_fDisableAppSwitch
    VWM_SETDATALOC,                     // user want to set the location & type of recording medium
    VWM_CLOSEALL,                       // user wants to close all files
    VWM_RECORDONPOWERUP,                // sent when HotKey is pressed on power up
    VWM_SETMAXRECSIZE,                  // wParam contains max bytes for embedded recordings
    VWM_GETMAXRECSIZE,                  // returns current setting for max recording size
    VWM_RECORDINGTIME,                  // returns 0 (if not recording) or
                                        //   GetTickCount from recording start
    VWM_NEWPARENT,                      // wParam: "parent" window -- attempts to connect, to
                                        //   receive notifications of recording & filename
} VOICE_MESSAGES;

//
// End Voice Recording
//
//------


//
// Start Font\Color Manager
//

typedef struct _tagCCtlTextDesc
{
    int     cCtlId;
    int     nPointSize;
    long    lWeight;
    ULONG   ulFgColorDesc;
} CCtlTextDesc;

enum CTLTEXT_RETURN
{
    CTLTEXT_CONTINUE,
    CTLTEXT_EXIT,
    CTLTEXT_UNDEFINED
};

// For System Colors
#define ACCENT_TEXT_COLOR           COLOR_ACCENTTEXT    // Default is blue
#define ALERT_TEXT_COLOR            COLOR_ALERT         // Default is red
#define STANDARD_TEXT_COLOR         COLOR_STATICTEXT    // Default is black


void* SHFontMgrCreate(const CCtlTextDesc rgCtlTextDesc[], int nSizeOfCtlTextDesc);

int SHFontMgrManageFonts(
        void*   pFontMgrObject,
        UINT    uiMessage,
        HWND    hDlg,
        UINT    wParam,
        LONG    lParam,
        ULONG*  pulRetVal
);

void SHFontMgrDestroy(void* pFontMgrObject);

// This function encompasses the other three functions above.
int SHRunFontManager(
    const   CCtlTextDesc rgCtlTextDesc[],
    int     nSizeOfCtlTextDesc,
    void**  ppvFontMgrObject,
    UINT    uiMessage,
    HWND    hDlg,
    UINT    wParam,
    LONG    lParam,
    ULONG*  pulRetVal
);


//
// End Font\Color Manager
//

//
// Code to draw header or group separator lines.
//

//
// SHHdrGrpSepLineDraw flags - mutually exclusive
//
#define SHD_HEADER              1
#define SHD_GROUPSEP            2
#define SHD_SHORT_GROUPSEP      3

// hwndText may be NULL if SHD_HEADER is passed for parameter 3, since the
// header is spaced from the top of the dialog.

void SHHdrGrpSepLineDraw(
    HWND hDlg,      // Handle to calling window or dialog.
    HWND hwndText,  // Handle to control under which to put the group sep.
    HDC  hdc,       // Handle to device context from BeginPaint.
    int  LineType   // SHD_HEADER, SHD_GROUPSEP or SHD_SHORT_GROUPSEP
);

//
// End SHHdrGrpSepLineDraw
//


//++++++
//
// Registered Application Information routines
//


//
// Flags for App Registration functions
//

#define RAI_CLASS               0x01
#define RAI_EXE                 0x02
#define RAI_PERF                0x04
#define RAI_FLAGS               0x08
#define RAI_SESSION             0x10
#define RAI_ALL                 0x1F


//
// Perf hint flags
//

#define RAI_PERF_BIGMEM         0x01
#define RAI_PERF_QUICKIE        0x02

//
// App Type Flags
//
// If this flag is set, don't TerminateProcess
// if the app fails to respond to WM_CLOSE
#define RAI_DONT_TERMINATE 0x80000000



//
// App Registration structure
//

typedef struct {
    DWORD  dwInfo;
    LPTSTR ptszWindowClassName;
    LPTSTR ptszExe;
    DWORD  dwPerfHint;
    DWORD  dwAppTypeFlags;
    BOOL   fNewSession;
} REGISTEREDAPPINFO;


//
// App Registration functions
//

BOOL GetRegisteredAppInfo( LPTSTR ptszApp, DWORD dwInfo, REGISTEREDAPPINFO *prai );
BOOL SetRegisteredAppInfo( LPTSTR ptszApp, DWORD dwInfo, REGISTEREDAPPINFO const *prai );
BOOL FreeRegisteredAppInfo( REGISTEREDAPPINFO *prai );


//
// End Registered Application Information routines
//
//------


#define REG_STR_NOTIFICATIONS   TEXT("ControlPanel\\Notifications")
#define REG_STR_DEFAULT         TEXT("Default")

// Flags for Notifications CPL. "Options" reg value can be a combination of
// these flags.
#define NOTIF_SOUND     0x00000001
#define NOTIF_VIBRATE   0x00000002
#define NOTIF_FLASH     0x00000004
#define NOTIF_MESSAGE   0x00000008
#define NOTIF_REPEAT    0x00000010
#define NOTIF_DISABLED  0x20000000

// Kinda hacky. Set the high bit to indicate this is a phone call.
// This lets us switch the controls to be the phone settings
// Same story for Reminder, which supports Repeat sounds
#define NOTIF_ISPHONE       0x80000000
#define NOTIF_ISREMINDER    0x40000000

//++++++
//
// Shell application notifications
//


//
// Shell app notifications codes.
//

#define SAN_NOTIFY_LAUNCH       1
#define SAN_NOTIFY_CLOSE        2
#define SAN_NOTIFY_QUERY_TAG    3
#define SAN_HARDWARE_COMMAND    4
// these are the commands sent via HW buttons
#define     SANCMD_ACTIVATE     0
#define     SANCMD_NEW          1
#define     SANCMD_NEXT         2
#define SAN_NOTIFY_POWERON      5

//
// App data associated with a SAN_NOTIFY_LAUNCH message.
//

typedef struct {
    BOOL fNewSession;
    LPTSTR ptszCmdLine;
} SAN_LAUNCH_STRUCT;


//
// Name of the app notification message (to use with RegisterWindowMessage).
//

#define SAN_WM_NAME             TEXT("SHWMAppNotify")

// RegisterWindowMessage string for Home/Today activation
#define MSG_ACTIVATE_HOME       TEXT("SHWMActivateHome")


//default talk button behavior
void OnDefTalkButton();

void PopulateAndLaunchDialer(HWND hwndOwner, const TCHAR *pszNum, const TCHAR *pszName);

//
// Some choice functions.
//

BOOL SHAppNotifyDone( HWND hwnd, LPARAM lparam, DWORD dwRes, void *pvRes );

//
// End shell application notifications
//
//------

//
// SHSavePWWarning - Warns the user of the dangers of saving
//                   his/her password.
//

void SHSavePWWarning(HWND hWnd);

//
// End SHSavePWWarning
//



//~~~~~~~~~~~~
//
//  Carrier Branding

//   =======CARRIER BRANDING GROUP========
//
// ..-------------------------------------
//   ! 2 ! 2 ! 2 ! 1 ! 1 ! 1 ! 0 ! 0 ! 0 !
// ..-------------------------------------
//
//  Bits 0-2 are for the dialer application group SHCBG_DIALER
#define SHCBG_DIALER            0
//  Bits 3-5 are for the call alert application group SHCBG_CALLALERT
#define SHCBG_CALLALERT         1
//  Bits 6-8 are for the sim security application group SHCBG_SIMSECURITY
#define SHCBG_SIMSECURITY       2
//  Bits 9-11 are for the signal strength application group SHCBG_SIGNALSTRENGTH
#define SHCBG_SIGNALSTRENGTH    3


//
// Carrier Branding Flags
//

//  ========CARRIER BRANDING FLAGS========
//
//    / GROUP 2   / GROUP 1   / GROUP 0 (See carrier branding group)
//   -------------------------------------
//   ! - ! - ! - ! - ! - ! - ! 2 ! 1 ! 0 !
//   -------------------------------------
//
// If all bits are zero eg SHCBF_SHOW_TEXTOVVERRIDE then return the BrandOverride string from the registry
#define SHCBF_SHOW_TEXTOVERRIDE     0

// Bit 0 selects whether to return the HOME network string or NETWORK string.
#define SHCBF_SHOW_HOME             0
#define SHCBF_SHOW_NETWORK          1
// Bit 1 if set will return the text string, even if bit 2 is set
#define SHCBF_SHOW_TEXT             2
// Bit 2 if set will return TRUE if bit 0 is not set (HOME), or if bit 0 is set (NETWORK) and not roaming
// It is the applications job to check that the bitmap exists and to choose the behavior when both the text is returned & the bitmap flag is TRUE
#define SHCBF_SHOW_BITMAP           4

DWORD SHGetCarrierBrandingFlag(DWORD dwBrandingGroup);
HRESULT SHGetCarrierBranding(DWORD dwShowBrandingFlag, BOOL *pbLoadBitmap, TCHAR *pszCarrierText, DWORD dwCarrierTextLen);

// Bitfield (iFormat): Align text right, by default (0) the helper function will align left
#define SHDCT_ALIGN_RIGHT 1

// Bitfield (iFormat): Margin left, if set will add a small amount of space on the left-side
#define SHDCT_MARGIN_LEFT 2
// Bitfield (iFormat): Margin right, if set will add a small amount of space on the right-side
#define SHDCT_MARGIN_RIGHT 4


RECT SHDrawBranding(HDC hDC, const RECT *prcDest, const TCHAR *pszBrandingText, HFONT hFont, HBITMAP hBrandBitmap, unsigned dxBrandWidth, int iFormat);

RECT SHDrawClippedText(HDC hDC, const RECT *prcDest, const TCHAR *pszText, HFONT hFont, int iFormat);

//
//  end Carrier Branding
//
//~~~~~~~~~~~~



//~~~~~~~~~~~~
//
//  SHGetDeviceFeatureLevel
//

#define SHDFLI_SOUND            0xc0020000
#define SHDFL_NOSOUND           0
#define SHDFL_BEEPONLY          1
#define SHDFL_FULLSOUND         2

#define SHDFLI_POCKETOFFICE     0xc0020001
#define SHDFL_NOPOCKETOFFICE    0x0
#define SHDFL_POCKETWORD        0x1
#define SHDFL_POCKETEXCEL       0x2
#define SHDFL_FULLPOCKETOFFICE  SHDFL_POCKETWORD | SHDFL_POCKETEXCEL

#define SHDFLI_INFRARED         0xc0020002
#define SHDFL_NOINFRARED        0
#define SHDFL_INFRARED          1

#define SHDFLI_VIBRATION        0xc0020003
#define SHDFL_NOVIBRATION       0
#define SHDFL_VIBRATION         1

#define SHDFLI_IMAP4            0xc0020004
#define SHDFL_NOIMAP4           0
#define SHDFL_IMAP4             1

#define SHDFLI_PHONE            0xc0020005


// Below defines should sync with the define PHGFL_* in public\cellcore\oak\inc\ccoreutl.h
#define SHDFL_NOPHONE                         0x00000000
#define SHDFL_PHONE                           0x00000001
#define SHDFL_PHONE_RADIOPRESENT              0x00000002
#define SHDFL_PHONE_RADIOON                   0x00000004
#define SHDFL_PHONE_LOCKFACILITY              0x00000008
#define SHDFL_PHONE_SIMSUPPORT                0x00000010
#define SHDFL_PHONE_VOICEDIALCONNECTS         0x00000020
#define SHDFL_PHONE_UNIQUECALLTRACKING        0x00000040
#define SHDFL_PHONE_AUTOTHREEWAYCALLING       0x00000080
#define SHDFL_PHONE_INTLPLUS                  0x00000100 // Convert "0" to "+"
#define SHDFL_PHONE_SMSUNICODE                0x00000200
#define SHDFL_PHONE_0340SMS                   0x00000400 // GSM SMS standard
#define SHDFL_PHONE_SUPPORTVOICEPRIVACY       0x00000800
#define SHDFL_PHONE_AKEY                      0x00001000
#define SHDFL_PHONE_IS637SMS                  0x00002000 // CDMA SMS standard
#define SHDFL_PHONE_SUPPORTGPRS               0x00004000
#define SHDFL_PHONE_SUPPORTMOSMS              0x00008000
#define SHDFL_PHONE_SUPPORTMULTILINES         0x00010000
#define SHDFL_PHONE_SUPPORTSPEAKERPHONE       0x00020000
#define SHDFL_PHONE_SUPPORTWCMP               0x00040000
#define SHDFL_PHONE_SUPPORTAUTOPREFIX         0x00080000
#define SHDFL_PHONE_SUPPORTINTERNATIONALCODE  0x00100000
#define SHDFL_PHONE_VTDIALCONNECTS            0x00200000 // Auto connects Video Telephony calls on dial success.

#define SHDFL_PHONE_LAST_CELLCOREFEATURE      SHDFL_PHONE_VTDIALCONNECTS

// After here we list the non-cellcore feature level bits that do not have a corresponding PHGFL_
#define SHDFL_PHONE_SUPPORTLOCATIONICONS      0x01000000
#define SHDFL_PHONE_SUPPORT0230DIALPARSING    0x02000000


// Aliased feature level bits.  For the time being we are using SHDFL_PHONE_SUPPORT0230DIALPARSING as
//  a 'GSM' flag, but we should be able to easily decoupled these later
#define SHDFL_PHONE_CALLFORWARDLISTENER       SHDFL_PHONE_SUPPORT0230DIALPARSING
#define SHDFL_PHONE_RESTRICTEDDIALING         SHDFL_PHONE_SUPPORT0230DIALPARSING
#define SHDFL_PHONE_NETWORKSELECTION          SHDFL_PHONE_SUPPORT0230DIALPARSING
#define SHDFL_PHONE_SUPPORTCALLBARRING        SHDFL_PHONE_SUPPORT0230DIALPARSING
#define SHDFL_PHONE_SUPPORTCALLERIDSUPSVC     SHDFL_PHONE_SUPPORT0230DIALPARSING
#define SHDFL_PHONE_SUPPORTCELLBROADCAST      SHDFL_PHONE_0340SMS // Should match PHGFL_CELLBROADCAST alias
#define SHDFL_PHONE_SUPPORTCALLFWDSUPSVC      SHDFL_PHONE_SUPPORT0230DIALPARSING
#define SHDFL_PHONE_SUPPORTSMSC               SHDFL_PHONE_SUPPORT0230DIALPARSING // Is there a better alias for this?


#define CONNECT_MODE_SHIFT        1

#define SHDFLI_BTH                0xc0020006
#define SHDFL_NOBTH               0x00
#define SHDFL_BTH                 0x01
#define SHDFL_BTH_CONNECTABLE     BTH_CONNECTABLE    << CONNECT_MODE_SHIFT
#define SHDFL_BTH_DISCOVERABLE    BTH_DISCOVERABLE   << CONNECT_MODE_SHIFT

#define SHDFLI_MMS                0xC002000A
#define SHDFL_NOMMS               0x0
#define SHDFL_MMS                 0x1

#define SHDFLI_MUI                0xc0020007
#define SHDFL_NOMUI               0
#define SHDFL_MUI                 1

#define SHDFLI_SPEECH             0xc0020008
#define SHDFL_NOSPEECH            0
#define SHDFL_SPEECH              1

#define SHDFLI_PERSISTENTSTORE    0xc0020009
#define SHDFL_NOPERSISTENTSTORAGE 0
#define SHDFL_PERSISTENTSTORAGE   1

#define SHDFLI_BTH_HANDSFREE      0xc002000B
#define SHDFL_NOHANDSFREE         0
#define SHDFL_HANDSFREE           1


#define SHDFLI_ML_EX              0xc002000C
#define SHDFL_NOML_EX             0x00000000
#define SHDFL_ML_OUTGOING         0x00000001
#define SHDFL_ML_VMAIL            0x00000002
#define SHDFL_ML_FORWARD          0x00000004
#define SHDFL_ML_ALL              SHDFL_ML_OUTGOING | SHDFL_ML_VMAIL | SHDFL_ML_FORWARD


#define SHDFLI_JAPAN              0xc0020010
#define SHDFL_NOJAPAN             0
#define SHDFL_JAPAN               1

#define SHDFLI_2TIERSUPPORT       0xc0020011
#define SHDFL_NO2TIERSUPPORT      0
#define SHDFL_2TIERSUPPORT        1


//to find out if a phone is there without special phone features.
#define SHDFLI_PHONEQUICK         0xc0020012
//#define SHDFL_NOPHONE                         0x00000000
//#define SHDFL_PHONE                           0x00000001

// wmp features query
#define SHDFLI_WMP_EQUALIZER      0xc0020013
#define SHDFLI_WMP_SEARCH         0xc0020014
#define SHDFLI_WMP_SAVE_PLAYLIST  0xc0020015
#define SHDFLI_WMP_FEATURE_SUPPORTED 1

#define SHDFLI_SOUND_RECORDING    0xc0020016
#define SHDFL_NOSOUND_RECORDING   0
#define SHDFL_SOUND_RECORDING     1

// CONTACT FIELDS SUPPORTED BY THE CURRENT SIM. ref SIMPHONEBOOKCAPS.
// Bits 0-7 are a value indicating the number of email addresses
// supported per contact.
// Bits 8-15 are a value indicating the number of additional phone
// numbers supported per contact.
// Bits 16-19 are a value indicating the number of USIM groups
// supported.
// Bits 20-27 are a value indicating the number of additional number
// tags supported.
// Bit 28 is unused.
// Bit 29 indicates if the hidden flag is supported.
// Bit 30 indicates that the additional name is supported.
// Bit 31, the most significant bit, indicates that the default name and
// number are supported. This will only be FALSE if there is no SIM
// support, or the device has never been booted with a SIM, or the SIM
// really has no phonebook support at all.
// If you boot with no SIM you'll get the capabilities from the
// previous boot that did have a SIM.
#define SHDFLI_SIMCTS_CONTACTSFIELDS     0xc0020017
#define SHDFL_SIMCTS_EMAILCOUNT          0x000000FF
#define SHDFL_SIMCTS_ADDNLNUMBERCOUNT    0x0000FF00
#define SHDFL_SIMCTS_GROUPCOUNT          0x000F0000
#define SHDFL_SIMCTS_PHONENUMBERTAGS     0x0FF00000
#define SHDFL_SIMCTS_HIDDENFLAG          0x20000000
#define SHDFL_SIMCTS_SECONDNAME          0x40000000
#define SHDFL_SIMCTS_BASE                0x80000000

#define SHDFL_SIMCTS_GET_SUPPORTED_EMAILCOUNT(_dwcaps) ((_dwcaps & SHDFL_SIMCTS_EMAILCOUNT))
#define SHDFL_SIMCTS_GET_SUPPORTED_ADDNLNUMBERCOUNT(_dwcaps) ((_dwcaps & SHDFL_SIMCTS_ADDNLNUMBERCOUNT) >> 8)
#define SHDFL_SIMCTS_GET_SUPPORTED_GROUPCOUNT(_dwcaps) ((_dwcaps & SHDFL_SIMCTS_GROUPCOUNT) >> 16)
#define SHDFL_SIMCTS_GET_SUPPORTED_TAGCOUNT(_dwcaps) ((_dwcaps & SHDFL_SIMCTS_PHONENUMBERTAGS) >> 20)


#define SHDFLI_VOIP               0xc0020018
#define SHDFL_NOVOIP              0
#define SHDFL_VOIP                1
#define SHDFL_VOIP_SETTINGSUI     2

// Fixed dialing capabilities of the current SIM.
// If the bit 31 (0 based, most significant bit) is set, the SIM supports
// Fixed Dialing Numbers
// If the bit 30 is set, the SIM supports an Alpha tag, which implies that the alpha tag
// identifier length of the current SIM's FDN records is greater than a minimum threshold
// stored in [HKLM\ControlPanel\Phone] - DWORD:MinFixedDialingAlphaTagLen
// If the bit 30 is set, the SIM's alpha identifier length can be obtained from the least
// significant two bytes. Else we consider the alpha tag length to be zero.
#define SHDFLI_SIMCTS_FIXEDDIALING                          (0xc0020019)
#define SHDFL_SIMCTS_FIXEDDIALING_ALPHA_TAG_LEN             (0x0000FFFF)
#define SHDFL_SIMCTS_FIXEDDIALING_BASE                      (0x80000000)
#define SHDFL_SIMCTS_FIXEDDIALING_ALPHA_TAG_SUPPORTED       (0x40000000)

#define SHDFL_SIMCTS_GET_FIXEDDIALING_ALPHA_TAG_LEN(_dwcaps)        ((_dwcaps & SHDFL_SIMCTS_FIXEDDIALING_ALPHA_TAG_LEN))
#define SHDFL_SIMCTS_IS_FIXEDDIALING_SUPPORTED(_dwcaps)             (BOOLIFY(((_dwcaps & SHDFL_SIMCTS_FIXEDDIALING_BASE))))
#define SHDFL_SIMCTS_IS_FIXEDDIALING_ALPHA_TAG_SUPPORTED(_dwcaps)   (BOOLIFY(((_dwcaps & SHDFL_SIMCTS_FIXEDDIALING_ALPHA_TAG_SUPPORTED))))

//
// NOTE: The range 0xc0030000 - 0xc0039999 will be reserved for
// exclusive features.
//

#define SHDFLI_BEGIN_EXCLUSIVE_RANGE    0xc0030000

//
// See FeatureExclusivity.h
//

#define SHDFLI_END_EXCLUSIVE_RANGE      0xc0039999

BOOL SHGetDeviceFeatureLevel(DWORD dwIndex, DWORD *pdwFeatureLevel);

// dwFeatureLevel is one or more SHDFL_PHONE_* flags.
BOOL CheckPhoneFeatureLevel(DWORD dwFeatureLevel);

// returns TRUE or FALSE depending on whether we should display SMS UI or not
BOOL CheckSmsUiEnabled(void);

// returns TRUE or FALSE depending on whether we should display SMS2 UI or not
BOOL CheckSms2UiEnabled(void);

// returns TRUE or FALSE depending on whether we should display Push-To-Talk UI or not
BOOL IsPushToTalkSupportEnabled(void);

//
//  end SHGetDeviceFeatureLevel
//
//~~~~~~~~~~~~

//
// CheckBeamingSupport
//
// Determines whether or not beaming is currently physically possible.
//

BOOL CheckBeamingSupported(void);

//~~~~~~~~~~~~
//
//  Document Management
//  (see docmgr.rtf and docmgr.c in wpcshell for more info)
//


// document disposition
typedef enum _SHDD
{
    SHDD_APPNOTLAUNCHED = 0xDD00,
    SHDD_NEW,
    SHDD_EXISTING,
    SHDD_ERROR
} SHDD;

// document identifier type
typedef enum _SHDIT
{
    SHDIT_PATH = 0,
    SHDIT_NUMERIC,
    SHDIT_NIL // sentinel
} SHDIT;

// document manager registration flags

#define SHDMRF_APPLAUNCH    0x00000000
#define SHDMRF_DOCWINDOW    0x00000001

#define SHDMRF_REGISTER     0x00000000
#define SHDMRF_UNREGISTER   0x00000002


BOOL SHDocManagerCreate(HWND hwndMain, SHDIT shdit, HANDLE *phDocManager);

BOOL SHDocManagerRegister(HANDLE hDocManager, DWORD grdmrfFlags, HWND hwnd,
        VOID *pvDocID);

SHDD SHDocManagerQuery(HANDLE hDocManager, VOID *pvDocID);

BOOL SHDocManagerDestroy(HANDLE hDocManager);


//
//  end Document Management
//
//~~~~~~~~~~~~


// ****************************  Start TPC only functions *********************

#define SHL_UNLOCK              0
#define SHL_DEVICE_LOCK         0x01
#define SHL_KEY_LOCK            0x02
#define SHL_SIM_LOCK            0x04
#define SHL_BOOT_LOCK           0x08
#define SHL_LOCK_IN_PROGRESS    0x80000000

// TPC lock/unlock functions
HRESULT SHLock(int nLockType);
HRESULT SHUnlock(UINT uVKey, LPCTSTR pszPassword);
HRESULT SHIsLocked(int *pnLock);
HRESULT SHWriteLockState(int nLock, int *pnLockCurrent);
HRESULT SHClearLockState(int nLockClear, int *pnLockNew);

// Tray message to reset device session
#define TM_RESET_SESSION             (WM_APP+12)
// Tray messages to handle special windows that Shell needs to know about
#define TM_SPECIAL_WINDOW_CREATED    (WM_APP+13)     // A window that Shell needs to know about was created.
#define TM_SPECIAL_WINDOW_DESTROYED  (WM_APP+14)     // A window that Shell needs to know was created.
#define TM_GET_SPECIAL_WINDOW        (WM_APP+15)     // Get specified special window. wParam is SWT_* constant
#define TM_SET_TRAY_ON_TOP           (WM_APP+16)     // Set/Remove tray's top style. wParam is TRUE to set. FALSE to remove.#define TM_SET_MENU_NUMBER          (WM_APP+17)     // wParam contains the current menu number.  -1 means to clear
#define TM_UNLOCK_SIM                (WM_APP+18)     // Prompt user to unlock SIM.
#define TM_SETVOLUMEKEYTRAP          (WM_APP+19)     // Register to trap volume keys
#define TM_SIM_LOCKCHANGED           (WM_APP+20)     // SIM lock has changed states
#define TM_HEADSET_CHANGE            (WM_APP+21)     // Headset state has changed (inserted or removed)
#define TM_CARKIT_CHANGE             (WM_APP+22)     // Carkit state has changed (inserted or removed)
#define TM_SPEAKER_CHANGE            (WM_APP+23)     // Speaker phone state changed
#define TM_HANDSFREE_CHANGE          (WM_APP+24)     // BT Handsfree change -> will change profile to Headset
#define TM_SET_STARTUP_WINDOW        (WM_APP+25)     // Set the specified special window to be a startup window. wParam is SWT_* constant. lParam is TRUE to set, FALSE to unset.
#define TM_SUPPRESS_SIMPINUI         (WM_APP+26)     // Suppress SIM PIN UI for emergency call

#define WM_CPROG_RADIO_REGISTER      (WM_APP+1)      // Register on the network
#define WM_CPROG_TOGGLE_SPEAKERPHONE (WM_APP+2)      // toggles the speakerphone state
#define WM_CPROG_SEND_VKEY_DTMF      (WM_APP+3)      // Sends the DTMF tone(s) through to the current call (converting from VKEY to DTMF chars)
#define WM_CPROG_CALL_ANSWER         (WM_APP+4)      // attempts to answer an offering call
#define WM_CPROG_CALL_REJECT         (WM_APP+5)      // attempts to reject an offering call
#define WM_CPROG_BT_HEADSET_CLOSE_AND_CONNECT (WM_APP+8) //attempts to close BT headset control channel, and optionally connects
#define WM_CPROG_BUSY_SUBSCRIBE      (WM_APP+9)      // this message is used in two cases: if this message is sended TO cprog then attempts to subscribe to cprog for "busy" call state notifications, if this message is sended BY cprog then is a busy notification.
#define WM_CPROG_DOVERB              (WM_APP + 10)   // attempts to execute a cprog-verb.
#define WM_CPROG_CLEARACCUM          (WM_APP + 11)   // (PPC only) clears the phone's accumulator

#define WM_CPROG_SHOWWINDOW          (WM_APP + 115)  // Tells cprog to show.  If WPARAM is TRUE, LPARAM indicates view below.
#define CPROG_VIEW_SMARTDIALER        8              // Index of PH_VIEW_SMARTDIAL.  Must match in phcanvas.h.

#define WM_CPROG_UPDATEVMAIL_NUMBER   (WM_APP+25)    // Have the phone update its voicemail numbers from the sim or registry

// Messages that will come from the home/today screen
#define WM_CPROG_KEYDOWN             (WM_APP + 0x600) // Analogous to WM_KEYDOWN
#define WM_CPROG_FIRSTKEYMSG         WM_CPROG_KEYDOWN
#define WM_CPROG_KEYUP               (WM_APP + 0x601) // Analogous to WM_KEYUP
#define WM_CPROG_CHAR                (WM_APP + 0x602) // Analogous to WM_CHAR
#define WM_CPROG_LASTKEYMSG          WM_CPROG_CHAR

// wParam of TM_SPECIAL_WINDOW_CREATED & TM_SPECIAL_WINDOW_DESTROYED is the window handle
// lParam of TM_SPECIAL_WINDOW_CREATED & TM_SPECIAL_WINDOW_DESTROYED is the type of the window
#define SWT_DESKTOP                 1     // Desktop window
#define SWT_CALLALERT               2     // Call Alert window
#define SWT_STARTUI                 3     // StartUI window
#define SWT_ENDKEYCLIENT            4     // Window that needs notification when END key is pressed.
#define SWT_CALLPROGRESS            5     // Call Progress Window

// StartUI private messages
#define SUIM_UNLOCK                     (WM_APP+65)     // Try to unlock device.
#define SUIM_EMERGENCY_CALL_STARTED     (WM_APP+66)     // Emergency call was started.

// Home private messages
#define HM_HIDE_OWNED_WINDOWS       (WM_APP+65) // Hide owned windows
#define HM_SHOW_OWNED_WINDOWS       (WM_APP+66) // Hide owned windows
#define HM_RELOAD_BUTTON            (WM_APP+67) // reload a button which can be customized
#define HM_REFRESH                  (WM_APP+68) // refresh the window
#define HM_EMPTYQUEUE               (WM_APP+69)
#define HM_RESET_SELECTION          (WM_APP+70) // Reset home screen selection

// Start menu private messages
#define SM_REFRESH_START_MENU        (WM_APP+67) // refresh the start menu

//start menu refresh all link files and icons
HRESULT SHRefreshStartMenu();

// Tray hide/show for full screen apps
void SHOnFullScreenAppActivate(HWND hwnd, WPARAM wParam, LPARAM lParam);

//default handler for TPC system hotkeys
BOOL SHHandleHotkey(HWND hwnd, UINT uMsg, WPARAM wP, LPARAM lP);

// Shell Verbs (used with SHProcessVerb)
// Values 3 and 4 used to be SV_KEYLOCK
// and SV_DEVICELOCK, respectively. These
// have been merged into a single lock verb.
// Existing applications will still work, but
// new code should always use SV_LOCK.
#define SV_NULL                 0       //  NOOP - do not use
#define SV_VOICERECOGNITION     1       //  Invokes voice tag recognition
#define SV_QUICKACCESSMENU      2       //  Launches Quick Access menu
#define SV_LOCK                 3       //  Locks the device (key lock
                                        //  or device lock)
/* Do not use                   4 */    //  Do not use. This value is
                                        //  permanently reserved for
                                        //  backwards compatibility.
#define SV_CAMERA               5       //  Triggers registered camera app


// Map a virtual key code to its associated verb
HRESULT WINAPI SHKeyToVerb(
    UINT uVKey,         // In: Key
    UINT *puVerb        // Out: Verb (SV_* constant)
);

// Process verb
HRESULT SHProcessVerb(
    UINT uVerb          // In: Verb (SV_* constant)
);

//////////////////////////////////////////////////////////////////////////////
//
// SH_VERB_CAMERA - Notifies a window to perform the "camera" verb
//
// Call RegisterWindowMessage on this string if you are interested in knowing
// when the camera hardware button has been pressed.
#define SH_VERB_CAMERA    TEXT("SH_VERB_CAMERA")

//
//////////////////////////////////////////////////////////////////////////////


HRESULT SHVoiceTagTrain(HWND hwndParent, DWORD dwCookie, IUnknown* pUnk);
HRESULT SHVoiceTagRecognize(HWND hwndParent, DWORD* pdwCookieOut, IUnknown* pUnk);
HRESULT SHVoiceTagPlayback(HWND hwndParent, DWORD dwCookie, IUnknown* pUnk);
HRESULT SHVoiceTagDelete(HWND hwndParent, DWORD dwCookie, IUnknown* pUnk);

//++++++
//
// SHEnumFiles/Folders : See function definition for documentation
//
// Definitions for enumeration helper functions.  Return FALSE to stop enumerating.
typedef BOOL (*SHFILEENUMPROC)(const TCHAR* pszPath, const TCHAR* pszFilename, LPARAM lParam);

#define CSIDL_EF_ROOT       (-1) // the root of the file system ( "\" )  This is useful if you want to enumerate a non-special folder.
#define CSIDL_EF_WINDOWS    (-2) // Enumerate in the windows folder
#define CSIDL_EF_IFFS       (-3) // Enumerate in the root of the IFFS volume (aka Persistent Storage volume)

HRESULT
SHEnumFiles(
    int nFolder,
    const TCHAR* pszSubDirSearch,
    const TCHAR** ppszExtensionList,
    DWORD dwFileAttributesMask,
    DWORD dwFileAttributes,
    SHFILEENUMPROC pfn,
    LPARAM lParam
    );



typedef BOOL (*SHFOLDERENUMPROC)(const TCHAR* pszPath, LPARAM lParam);

#define EFT_PERSONAL        1 // All the locations where a user may put their files.  This includes "My Documents", storage card roots, and the "My Documents" folders on storage cards if they exist.
#define EFT_STORAGECARDS    2 // The root folders of all the storage cards inserted into the device

HRESULT SHEnumFolders(int nFolderType, SHFOLDERENUMPROC pfn, LPARAM lParam);

// End SHEnumFiles/Folders


// Gets the string correspong to CE id.  The associations between CE ids and Strings are defined in MSDN
// szString must be a buffer with at least MAX_PATH characters
HRESULT SHGetCEString(ULONG idCEString, TCHAR szString[]);

// Unpacks CE strings in a path
// If the path contains %InstallDir% and the user passes NULL for pszInstallDir, we will return E_INVALIDARG
HRESULT SHUnpackDirID(
    LPTSTR  pszText,
    DWORD   cchMax,    // @comm    Max size of the text buffer
    LPTSTR  pszInstallDir);

// Canonicalizes a file system path.  Removes extraneous path separators. Set fFullPathProcessing
// to true to perform complate path processing - otherwise we simply merge adjacent separator chars
HRESULT SHCanonicalizePath(TCHAR * szPath, BOOL fFullPathProcessing);

//++++++
//
// SHGetSoundFileList : See function definition for documentation
//
typedef struct tagLabelDataAndSize
{
    TCHAR *pszLabel;
    TCHAR *pszData;
    short cchLabel; //not counting null term
    short cchData; // not counting null term
} LabelDataAndSize;
typedef struct tagFolderAndSubdir
{
    int     nFolder;
    const   TCHAR* pszSubdir;
} FolderAndSubdir;
HRESULT SHGetSoundFileList(const FolderAndSubdir *rgFolderAndSubdir, int cFolders, const TCHAR* pszFilterList, LabelDataAndSize** prgFiles, int* pcFiles, int* pcchLabels, int* pcchData, BOOL fWriteOnlyOnly);

// default user dir to search for sounds
//if this list is ever expanded, please grep the source
//base for refrences to SOUND_USERDIR and update accordingly
#define SOUND_USERDIR          CSIDL_APPDATA
#define SOUND_USERSUBDIR    TEXT("Sounds")
#define WINDOWS_RINGSUBDIR    TEXT("Rings")    // to be used as along with CSIDL_EF_WINDOWS - see pimgdlg2\wpc\addrdlg.cpp

// End SHGetSoundFileList

//
// Set statstore, update MRE when voice mail count changes
//
HRESULT SHOnVoiceMailCountChange(int nLineNumber, int cMails, LPWSTR pwszText);

#define UNKNOWN_VOICEMAIL_COUNT  (-1)
#define VOIP_VOICEMAIL_INDEX     3

// Set Statstore with proper voicemail count.  True means reset to 0, false means increment.
HRESULT SHOnMissedCallCountChange(int iLineNumber, BOOL fResetCount);


// ****************************  End TPC only functions *********************

//
//------
// Added by JWLM, 12/11/2001

HRESULT SHCHSetColorFromAttribute(LPVOID pCHThis,
                                  LPVOID pXDNnode,
                                  const TCHAR* pszAttributeName);

HRESULT SHCHSetColorFromString(LPVOID pCHThis,
                            const TCHAR* pszColorIn);

HRESULT SHCHGetCOLORREF(LPVOID pCHThis,
                        COLORREF* pcrReturn);

// dwFlags for DrawTextBaseLineOffset
#define DTBLO_LEFT_JUSTIFY_CLIPPED  0x00000001 // if the text is wider than the rect, left justify it.

BOOL DrawTextBaseLineOffset(HDC hdc, HFONT hfont, const TCHAR* pszText, const RECT* prc, int iBaseLineOffset, UINT uFormat, DWORD dwFlags);

BOOL SHIsRestrictedProcess(LPCTSTR pszApp);
BOOL SHIsFileOperationRestricted(LPCTSTR pszFrom, LPCTSTR pszTo);
BOOL SHIsFileOperationRestrictedEx(LPCTSTR pszFrom, LPCTSTR pszTo, BOOL fCheckMetabase);
BOOL PathIsRestricted(LPCTSTR pszFile);

// +++++++++++++++++++++++++Preset Messages APIs++++++++++++

#define PRESETMSGS_NATIVE_INBOX     1
#define PRESETMSGS_NATIVE_IM        2
//++++++
//
//  SHInitPresetMessages
//
//      Prepares for PresetMessages calls

WINSHELLAPI
HANDLE
SHInitPresetMessages(HKEY hKey, LPCTSTR szSubKey);

//
// End SHInitPresetMessages
//
//------

//++++++
//
//  SHReleasePresetMessages
//
//      Releases the memory allocated in SHInitPresetMessages

WINSHELLAPI
VOID
SHReleasePresetMessages(HANDLE handle);

//
// End SHReleasePresetMessages
//
//------

//++++++
//
//  SHRunPresetMessagesEdit
//
//      Displays the dialog for editing Preset Messages

WINSHELLAPI
HRESULT
SHRunPresetMessagesEdit(HANDLE handle, HWND hwnd);

//
// End SHRunPresetMessagesEdit
//
//------

//++++++
//
//  SHInsertPresetMessage
//
//      Pops up a listbox containing the list of preset messages.  The selected
//      message is added to the edit control hwndEditCtrl.

WINSHELLAPI
HRESULT
SHInsertPresetMessage(HANDLE handle, HWND hwndParent, HWND hwndEditCtrl);

//
// End SHInsertPresetMessage
//
//------

//++++++
//
//  SHPopulatePresetMessageMenu
//
//      Populates a menu with as many preset messages as will fit
//      within the min and max command ID specified, starting at
//      the specified index.

WINSHELLAPI
HRESULT
SHPopulatePresetMessageMenu(HANDLE handle, HMENU hMenu, DWORD dwMinID, DWORD dwMaxID, DWORD dwIndex);

//
// End SHPopulatePresetMessageMenu
//
//------

//++++++
//
//  SHGetPresetMessage
//
//      Retrieves the text to the specified preset message.

WINSHELLAPI
LPCTSTR
SHGetPresetMessage(HANDLE handle, DWORD dwIndex);

//
// End SHGetPresetMessage
//
//------

//++++++
//
//  SHSetPresetMessage
//
//      Sets the text for the specified index to the text provided.

WINSHELLAPI
HRESULT
SHSetPresetMessage(HANDLE handle, DWORD dwIndex, LPCTSTR pszMessage);

//
// End SHSetPresetMessage
//
//------

// -------------------------- End Preset Messages APIs-----------


// -------------------------- Misc SHELL32.EXE PSL APIs-----------

//++++++
//
//  SHNotifyAppsOnDock
//
//      The function is called when the user docks thier device.
//      Enumerates registry key and notifies all apps registered on that key.
//      Notification involves:
//        1) finding the window class specified by "class".
//           1a) if found, send it the message specified by "msg"
//        2) if not window class not found, launch command specified by "command"
//        3) if flag NotifyOnLaunch is set, we'll wait until we find "class" afterwhich we'll send "msg"
//           we'll wait a max of 10 seconds for launch
//
//      Added fDock parameter to SHNotifyAppsOnDock API. If fDock is TRUE,
//      the API is called when device is docked, and we send it the message specified by "DockMsg".
//      If fDock is FALSE, the API is called when device is undocked, and we send it the message
//      specified by "UnDockMsg".



void WINAPI
SHNotifyAppsOnDock(BOOL fDock);

//++++++
//
//  SHNotifyAppsOnHeadset
//
//      The function is called when the user plugs (or unplugs) a headset into the device.
//      Enumerates registry key and notifies all apps registered on that key.
//      Notification involves:
//        1) finding the window class specified by "class".
//           1a) if found, send it the message specified by "msg"
//        2) if window class not found, launch command specified by "command"
//        3) if flag NotifyOnLaunch is set, we'll wait until we find "class" afterwhich we'll send "msg"
//           we'll wait a max of 10 seconds for launch
//
//      If fHeadset is TRUE, the API is called when headset is plugged in, and we send it the
//          message specified by "HeadOnMsg".
//      If fHeadset is FALSE, the API is called when headset is unplugged, and we send it the
//          message specified by "HeadOffMsg".

void WINAPI
SHNotifyAppsOnHeadset(BOOL fHeadset);

//++++++
//
//  SHNotifyAppsOnCarkit
//
//      The function is called when the user plugs (or unplugs) a carkit into the device.
//      Enumerates registry key and notifies all apps registered on that key.
//      Notification involves:
//        1) finding the window class specified by "class".
//           1a) if found, send it the message specified by "msg"
//        2) if window class not found, launch command specified by "command"
//        3) if flag NotifyOnLaunch is set, we'll wait until we find "class" after which we'll send "msg"
//           we'll wait a max of 10 seconds for launch
//
//      If fCarkit is TRUE, the API is called when carkit is plugged in, and we send it the
//          message specified by "CarOnMsg".
//      If fCarkit is FALSE, the API is called when carkit is unplugged, and we send it the
//          message specified by "CarOffMsg".

void WINAPI
SHNotifyAppsOnCarkit(BOOL fCarkit);

//++++++
//
//  SHNotifyAppsOnSpeakerPhone
//
//      The function is called when the user puts the device into speaker phone mode.
//      Enumerates registry key and notifies all apps registered on that key.
//      Notification involves:
//        1) finding the window class specified by "class".
//           1a) if found, send it the message specified by "msg"
//        2) if window class not found, launch command specified by "command"
//        3) if flag NotifyOnLaunch is set, we'll wait until we find "class" after which we'll send "msg"
//           we'll wait a max of 10 seconds for launch
//
//      If fSpeaker is TRUE, the API is called when speaker phone mode is entered, and we send it the
//          message specified by "SpeakerOnMsg".
//      If fCarkit is FALSE, the API is called when speaker phone mode is ended, and we send it the
//          message specified by "SpeakerOffMsg".

void WINAPI
SHNotifyAppsOnSpeakerPhone(BOOL fSpeaker);

//++++++
//
//  SHNotifyAppsOnCallConnect
//
//      The function is called when a call is connected or disconnected.
//      Enumerates registry key and notifies all apps registered on that key.
//      Notification involves:
//        1) finding the window class specified by "class".
//           1a) if found, send it the message specified by "msg"
//
//      fConnect is TRUE when the API is called when a call is connected, and we send the
//          message specified by "ConnectMsg".
//      fConnect is FALSE when the API is called when the last call is disconnected, and we send the
//          message specified by "DisconnectMsg".

void WINAPI
SHNotifyAppsOnCallConnect(BOOL fConnect);

//++++++
//
//  SHNotifyAppsOnIncomingCall
//
//      The function is called when a call is incoming.
//      Enumerates registry key and notifies all apps registered on that key.
//      Notification involves:
//        1) finding the window class specified by "class".
//           1a) if found, send it the message specified by "msg"
//
//      fIncoming is TRUE when the API is called when a call is incoming, and we send the
//          message specified by "IncomingBegMsg".
//      fIncoming is FALSE when the API is called when the incoming call is accepted or rejected, and we send the
//          message specified by "IncomingEndMsg".
//      NOTE: currently IncomingEndMsg is not sent, but I wanted to include the possibility in the API for future use.

void WINAPI
SHNotifyAppsOnIncomingCall(BOOL fIncoming);

//++++++
//
//  SHSameWindowProcesses
//
//      Return TRUE if hwnd1 and hwnd2 are in the same process.

BOOL WINAPI
SHSameWindowProcesses(HWND hwnd1, HWND hwnd2);

//++++++
//
//  SHFindPreviousInstance
//
//      Looks for prev. instance of an app. If found, prev. instance is optionally activated.

HRESULT
SHFindPreviousInstance(const TCHAR *pszClass,
               const TCHAR *pszTitle,
               HANDLE *phMutex,
               HWND *phWnd,
               BOOL *pfFound);

//++++++
//
//  SHFindPreviousInstanceEx
//
//      Looks for prev. instance of an app. If found, you have an option of activating it.
HRESULT
SHFindPreviousInstanceEx(const TCHAR *pwszClass,
             const TCHAR *pwszTitle,
             HANDLE *phMutex,
             HWND *phWnd,
             BOOL *pfFound,
             BOOL fActivateWindow);


//++++++
//
//  SHGetPowerOnTime
//
//      Gets the time (in ms) since the last time the device was turned on.

WINSHELLAPI DWORD SHGetPowerOnTime(void);

//
// End SHGetPowerOnTime
//
//------

//++++++
//
//  SHGetNavBarItemRect
//
//      Gets the screen coordinates rect of the specified NavBar item.

#define SHNBI_RADIO               0x0001
#define SHNBI_VOLUME              0x0002
#define SHNBI_CLOCK               0x0003
#define SHNBI_NOTIFICATIONS       0x0004
#define SHNBI_DATACONNECTION      0x0005
#define SHNBI_CONNECTION          0x0006
#define SHNBI_LAST                SHNBI_CONNECTION

LRESULT SHGetNavBarItemRect(DWORD dwItem, LPRECT prc);

//++++++
//
//  SHSetSimToolkitMenu
//
//      Name of the link for toolkit app is passed from SIMPOLCY.DLL.
//
//      E_FAIL returned, since this will not be implemented on Merlin
//

HRESULT SHSetSimToolkitMenu(LPCTSTR pszMenu);
UINT WINAPI SHGetSimToolkitMenu(LPTSTR pszMenu, UINT cchBuf);

//++++++
//
//  SHToolkitQueryShell
//
//      Query to ask shell if it's OK to run the tookit app now.
//
//      E_FAIL returned, since this will not be implemented on Merlin
//

HRESULT SHToolkitQueryShell(DWORD dwPriority, LPCTSTR pwszText);

//++++++
//
//  SHShowSimToolkitUI
//
//      Show the SIM Toolkit UI (as hosted in the shell)
//
//      E_FAIL returned, since this will not be implemented on Merlin
//

HRESULT SHShowSimToolkitUI();

// -------------------------- End Misc SHELL32.EXE PSL APIs-----------


#define SHMBI_QUESTION      16102
HICON SHGetMessageBoxIcon(int i);

// which watermark to set (enumeration, not bit flags)
#define WATERMARK_TODAY     0x0001
#define WATERMARK_TODAYFULL 0x0002  // includes Start Menu

BOOL SHSetAsWatermark(HWND hwnd, LPCTSTR pszFile, int iWhich, DWORD* pdwAlpha);

/// <summary>
/// Delete wallpaper files from the Windows directory on Pocket PC.
/// </summary>
///
/// <param name="fStartMenu">
/// TRUE to delete ALL wallpaper files, including the Start Menu graphics,
/// FALSE to only delete the desktop wallpaper files and leave the Start Menu
/// files alone
/// </param>
HRESULT WINAPI SHDeleteTodayWallpaper(BOOL fStartMenu);

// cxDesired,cyDesired = size of resulting image (-1 to have the size be the input size)
// xPos, yPos == target location of the faded bitmap w/in the resulting bitmap (0x7FFFFFFF (SHFI_CENTERED) == centered)
// choose that instead of -1 because -1 might actually be valid
// clr == color of the rest of the image...
// uiFadeAmount = value from 0 to 10000 on how much to fade.
//      0 = none, 10000 = pure white
//   uTargetColor = number from 0 to 255  currently only allows grays
//      0 = black, 255 = white
#define SHFI_CENTERED (0x7FFFFFFF)
HBITMAP SHFadeImage(HBITMAP hbm, int cxDesired, int cyDesired,
                    int xPos, int yPos, COLORREF clr, UINT uiFadeAmount, UINT uTargetColor);

// Copies an HBITMAP.  The new HBITMAP will have bits that live in GWES
// and therefore the HBITMAP can be used across processes.
HRESULT SHCopyBitmap(HBITMAP hbm, HBITMAP *phbmDest);

// Draws a shape with particular color.
// prcBounds is the rc to draw on.
// xStart and yStart are the offest to the top left corner of prcBounds.
// hbmpMask is the bitmap mask, it must be a monochrome bitmap.
// crColor is the color to fill the bitmap mask.
HRESULT DrawShapeColor(HDC hdc, RECT *prcBounds, LONG xStart, LONG yStart, HBITMAP hbmpMask, COLORREF crColor );

// gives the shell a chance to do any font preprocessing...
// such as turning off cleartype if that's what the user
// asked us to do.
void SHPreProcessLogFont(LOGFONT* plf);


// style font
typedef enum _SHSF
{
    SHSF_LABEL = 0,
    SHSF_EDIT,
    SHSF_LISTBOX,
    SHSF_BUTTON,
    SHSF_CONTROL,
    SHSF_CHECKBOX,
    SHSF_COMBOBOX,
    SHSF_LISTVIEW,
    SHSF_RADIOBUTTON,
    SHSF_GROUPBOX,
    SHSF_PROGRESS,
    SHSF_SLIDER,
    SHSF_IMAGE,
    SHSF_TREEVIEW,
    SHSF_SPINNER,
    SHSF_PROPSHEETHEADING,
    SHSF_COPYRIGHTTEXT,
    SHSF_BIGHEADING,
    SHSF_BIGLABEL,
    SHSF_BIGTEXT,
    SHSF_BIGCOMBO,
    SHSF_DEFPUSHBUTTON,
    SHSF_HEADING,
    SHSF_FILELIST,
    SHSF_ALERTPROPSHEETHEADING,
    SHSF_FILTERHEADING,
    SHSF_PHONE_SMALL,
    SHSF_PHONE_SMALLWITHEMPHASIS,
    SHSF_PHONE_NORMAL,
    SHSF_PHONE_WITHEMPHASIS,
    SHSF_PHONE_LARGER,
    SHSF_PHONE_LARGERWITHEMPHASIS,
    SHSF_PHONE_ACCUMULATOR,
    SHSF_PHONE_ACCUMULATOR_MEDIUM,
    SHSF_PHONE_ACCUMULATOR_SMALL,
    SHSF_SMARTDIAL_NORMAL,
    SHSF_SPEEDDIAL_NORMAL,
    SHSF_SPEEDDIAL_WITHEMPHASIS,
    SHSF_SPEEDDIAL_LARGER,
    SHSF_CALLALERT_MAJOR,
    SHSF_ACTIVESYNC_DEVICENAME,
    SHSF_SIMSECURITY_DESCRIPTION,
    SHSF_SIMSECURITY_ACCUMULATOR,
    SHSF_UNREADEMAIL,
    SHSF_READEMAIL,
    SHSF_ATTACHMENTWELL,
    SHSF_TODAYLVITEMLINE1,
    SHSF_TODAYLVITEMLINE2,
    SHSF_PIMLISTVIEW,
    SHSF_PIMCALENDARLINE2,
    SHSF_ILISTLABEL,
    SHSF_CALENDARLISTVIEW,
    SHSF_CALENDAREVENT,
    SHSF_CALENDARMONTHHEADER,
    SHSF_CALENDARWEEKNUMS,
    SHSF_CALENDARWEEKVIEWBOLD,
    SHSF_CALENDARYEARDLG,
    SHSF_CALENDARLINE2,
    SHSF_CONTACTSLISTVIEW,
    SHSF_CONTACTSFINDBAR,
    SHSF_TASKSLISTVIEW,
    SHSF_TASKSQUICKADD,
    SHSF_CONTACTSLVGROUP,
    SHSF_CONTACTSLVNORMAL,
    SHSF_CONTACTSLVREASON,
    SHSF_CONTACTSLVBOLD,
    SHSF_SUMMCRDFIELDHEADING,
    SHSF_SUMMCRDFIELDVALUE,
    SHSF_SUMMCRDFIELDSMALLLABEL,
    SHSF_STARTMENULABEL,
    SHSF_FILEEXPLORERHEADER,
    SHSF_COUNT      // present the count of style font
} SHSTYLEFONT;

// Getting information from RCML global styles.
COLORREF SHGetStyleBkColor(SHSTYLEFONT eFontID, LPCTSTR pszModifier);
COLORREF SHGetStyleColor(SHSTYLEFONT eFontID, LPCTSTR pszModifier);
HFONT  SHGetStyleFont(SHSTYLEFONT eFontID, LPCTSTR pszModifier);

// Is the window owned by a legacy process?
BOOL WINAPI SHIsPreOzoneUpdate(HWND hwnd);

// Add a scrollbar to a dialog.
BOOL WINAPI SetDialogAutoScrollBar(HWND hDlg);

// Add a scrollbar to a dialog (either portrait or landscape) if any of its 
// controls are obscured. Also if the dialog has a scroll bar and its not 
// required the scroll bar is removed.
BOOL WINAPI SetDialogAutoScrollBarEx(HWND hDlg);

// Set a window size in response to an orientation change. This function is
// identical to SetWindowPos UNLESS the window is a scrollable dialog, so you
// only need to call this if that may be the case. fNoScroll should be true if
// the screen is now in portrait orientation, and false if it is landscape or
// square.
BOOL WINAPI SetWindowPosOnRotate(HWND hWnd, HWND hWndInsertAfter,
                                 int X, int Y, int cx, int cy,
                                 UINT uFlags, BOOL fNoScroll);

// Resizing dialogs
LRESULT CALLBACK SHResizeDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Applying Styles found in the resource file
BOOL WINAPI CALLBACK SHRCMLDialogProc( HINSTANCE hInst, LPTSTR pszDialogID, HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam );

//
//@func int     | SHEscapeAccelerators  | Translates accelerators (&) into escaped (&&) characters
//
//@parm LPCTSTR | pszIn     | IN - String to be translated (source)
//@parm LPTSTR  | pszOut    | OUT - String after translation (destination)
//@parm int     | cchOut    | IN - Number of characters in destination string passed.
//
//@rdesc Returns the number of characters copied into pszOut.
//
int SHEscapeAccelerators(LPCTSTR pszIn, LPTSTR pszOut, int cchOut);

//
//@func int     | SHUnEscapeAccelerators  | Translates escaped accelerators ('&&') into
//                                          unescaped accelerator characters ('&')
//
//@parm LPCTSTR | pszIn     | IN - String to be translated (source)
//@parm LPTSTR  | pszOut    | OUT - String after translation (destination)
//@parm int     | cchOut    | IN - Number of characters in destination string passed.
//
//@rdesc Returns the number of characters copied into pszOut.
//
int SHUnEscapeAccelerators(LPCTSTR pszIn, LPTSTR pszOut, int cchOut);

//
//@func int     | SHEscapeBubbleHtml    | Translates special characters into their
//                                        html equivalents, for use in bubbles
//
//@parm LPCTSTR | pszIn     | IN - String to be translated (source)
//@parm LPTSTR  | pszOut    | OUT - String after translation (destination)
//@parm int     | cchOut    | IN - Number of characters in destination string passed.
//
//@rdesc Returns the number of characters copied into pszOut.
//
//@comm  Translates '&lt;' into '&amp;lt', '&gt;' into '&amp;gt' and '&amp;' into '&amp;amp'.
//

// Comm line above if for autodoc purposes, so it looks weird here.
// Translation: Translates '<' into '&lt', '>' into '&gt' and '&' into '&amp'.
int SHEscapeBubbleHtml(LPCTSTR pszIn, LPTSTR pszOut, int cchOut);


// timezone functions
#define TIMEZONEIDRANGE 310
typedef struct
{
    LONG Bias;
    LONG StandardBias;
    LONG DaylightBias;
    SYSTEMTIME StandardDate;
    SYSTEMTIME DaylightDate;
} TZIDATA;

typedef struct
{
    TCHAR szDisplayName[80];
    TCHAR szStandard[80];
    TCHAR szDaylight[80];
    TZIDATA tzid;
    int iID;
} TIMEZONEDATA;

typedef struct
{
    HINSTANCE hinstLocName;
    HINSTANCE hinstTZIData;
    HKEY hkey;
    int idNameFirst;
    int idDataFirst;

    TIMEZONEDATA tzd;
} TIMEZONEENUMDATA;

// we enum the hinst for data and names of timezones.
// they start at idNameFirst and idDataFirst

// we check the registry first for any overrides.
// then we get the data from the hinstance.

// hinstLocName & hinstTZIData can be the same dll, but we allow
// them to be separate so that for MUI, you can have hte localized
// strings in separate language dlls and the data all shared
// in another.


// if you set the iID, you can fetch an item directly
BOOL TZGetData(TIMEZONEENUMDATA *ptzed);

// standard findfirst/findnext method with the exception
// that we do a FindOpen, not a findfirst to start.
// this allow the caller to open and jump straight to a get
// if they know the id
//
// the outdata is in ptzed->tzd.
// since this is a private API, i didn't hide all the internal data info
void TZFindClose(TIMEZONEENUMDATA* ptzed);
BOOL TZFindNext(TIMEZONEENUMDATA* ptzed);
TIMEZONEENUMDATA* TZFindOpen(HINSTANCE hinstLocName,
                             int idNameFirst,
                             HINSTANCE hinstTZIData,
                             int idDataFirst);


// for the button properties
// these define the shell actions that can be bound to buttons
// these look like they are bit fields, but they aren't...
#define APPBUTTONFLAG_STARTMENU     0x01
#define APPBUTTONFLAG_TOGGLESIP     0x02
#define APPBUTTONFLAG_DESKTOP       0x04
#define APPBUTTONFLAG_UP            0x05
#define APPBUTTONFLAG_DOWN          0x06
#define APPBUTTONFLAG_LEFT          0x07
#define APPBUTTONFLAG_RIGHT         0x08
#define APPBUTTONFLAG_NONE          0x09
#define APPBUTTONFLAG_DONE          0x0A
#define APPBUTTONFLAG_CTX           0x0B
#define APPBUTTONFLAG_ROTATESCREEN  0x0C
#define APPBUTTONFLAG_SK1           0x0D
#define APPBUTTONFLAG_SK2           0x0E

// this is the mask of all buttons actions
#define APPBUTTONFLAG_MASK          0x0f



#ifdef _OBJBASE_H_
// need basic com goo.

// create a generic IOleWindow object for this hwnd
IOleWindow *SHCreateCOleWindow(HWND hwnd);

#endif

// Helper function for File Explorer to load context menu extensions for file types
BOOL SHLoadFileContextMenuExtensions(IUnknown *punkOwner,
        LPCTSTR pszClass, HMENU hmenu, UINT idCmdFirst, UINT idCmdLast,
        HANDLE *phCMExtensions);

//++++++
//
//  SHMakeCall
//
//       Dials a number

// * NOTE: If you change anything about SHMakeCall, make sure
// * you keep PhoneMakeCall() in public\apps\sdk\inc\phone.h in sync.
// * PhoneMakeCall is the publicly exposed API which simply forwards
// * to SHMakeCall.
typedef struct tagSHMAKECALLINFO
{
    DWORD cbSize;
    DWORD dwFlags;

    //Params to tapiRequestMakeCall
    PCWSTR pszDestAddress;
    PCWSTR pszAppName;
    PCWSTR pszCalledParty;
    PCWSTR pszComment;

    // owner window for dialog box that appears when the PMCF_EDITBEFORECALLING flag is passed into PhoneMakeCall
    HWND hwndOwner;
} SHMAKECALLINFO, *PSHMAKECALLINFO;

// SHMakeCall flags
#define SHMCF_DEFAULT                0x00000001
#define SHMCF_PROMPTBEFORECALLING    0x00000002
#define SHMCF_CONFIRMDIALREQUEST     0x00000004
#define SHMCF_DIALONLINE1            0x00000008
#define SHMCF_DIALONLINE2            0x00000010
#define SHMCF_EDITBEFORECALLING      0x00000020
#define SHMCF_ALLOWSUPSVCS           0x00000040
#define SHMCF_NOPROGRESSINFOONCALL   0x00010000


LONG SHMakeCall(SHMAKECALLINFO *shmci);

//
// End SHMakeCall
//
//------

//++++++
//
//  SHGetUiInfo and SHSetUiInfo.
//
//   Get/set system specific UI params.
//

// SHGetUiInfo/SHSetUiInfo Flags

typedef enum tagSHUIINFOTYPE
{
    SHUI_INVALID = 0,           // illegal

// What kind of deivce are we, anyway?

    SHUI_PLATFORMTYPE,      // See enum below

// Font metric items for backwards compatibility

    SHUI_BANNERFONTHEIGHT,      // banner font size
    SHUI_DATAFONTHEIGHT,        // data font size,
    SHUI_LABELFONTHEIGHT,       // label font size

// UI Element items

    SHUI_MENUHEIGHT,            // Menu bar height
    SHUI_TASKBARHEIGHT,         // Taskbar height
    SHUI_CHECKBOXHEIGHT,        // Height of a check box
    SHUI_CHECKBOXWIDTH,         // Width of a check box
    SHUI_RADIOBUTTONHEIGHT,     // Height of a radio button
    SHUI_RADIOBUTTONWIDTH,      // Width of a radio button
    SHUI_APPBARHEIGHT,          // Application Bar height

// These should be contiguous, these are cached system metrics
    SHUI_SM_CACHE_FIRST,
    SHUI_SM_CXBORDER = SHUI_SM_CACHE_FIRST,
    SHUI_SM_CXICON,
    SHUI_SM_CYICON,
    SHUI_SM_CXSMICON,
    SHUI_SM_CYSMICON,
    SHUI_SM_CACHE_LAST = SHUI_SM_CYSMICON,

// Fonts available to the font manager -- External IDs

    SHUI_FM_LISTUITEXT,
    SHUI_FM_LISTUIBOLDTEXT,
    SHUI_FM_LISTUILABEL,
    SHUI_FM_LISTUILINKTEXT,
    SHUI_FM_APPLIST,
    SHUI_FM_APPNORMAL,
    SHUI_FM_APPREASON,
    SHUI_FM_DIALERDIRECT,
    SHUI_FM_DIALERDIRECT_LARGE,
    SHUI_FM_DIALERDIRECT_MEDIUM,
    SHUI_FM_DIALERDIRECT_SMALL,
    SHUI_FM_RINGERCALLER,
    SHUI_FM_RINGERINFO,
    SHUI_FM_CONTACTSMESSAGE,
    SHUI_FM_SOFTKEY,
    SHUI_FM_CPROG_ALERT_HEADER,
    SHUI_FM_CPROG_ALERT_NAME,
    SHUI_FM_CPROG_ALERT_NAME_SM,
    SHUI_FM_CPROG_ALERT_NUMBER,
    SHUI_FM_CPROG_ALERT_LINE_ID,
    SHUI_FM_SHBOX_HEADING,
    SHUI_FM_SHBOX_HEADING_SMALL,
    SHUI_FM_VOLDLG_LABEL,
    SHUI_FM_CPROG_METHOD,
    SHUI_FM_CPROG_CALL_STATUS,
    SHUI_FM_CPROG_ACCUMULATOR,
    SHUI_FM_CPROG_SINGLE_CALL_NAME,
    SHUI_FM_CPROG_SINGLE_CALL_NAME_SM,
    SHUI_FM_CPROG_SINGLE_CALL_DURATION,
    SHUI_FM_CPROG_MULTI_CALL_NAME,
    SHUI_FM_MAIL_BOLD,
    SHUI_FM_NOTES_BOLD,

// Fonts available to the font manager -- Internal IDs

    SHUI_IDFONTSYSTEM,
    SHUI_IDFONTDIALINGSMALL,
    SHUI_IDFONTDIALINGMEDIUM,
    SHUI_IDFONTDIALINGLARGE,
    SHUI_IDFONTSOFTKEY,
    SHUI_IDFONTTITLE1,
    SHUI_IDFONTTITLE2,
    SHUI_IDFONTTITLE3,
    SHUI_IDFONTTITLE4,
    SHUI_IDFONTTITLE5,
    SHUI_IDFONTTITLE6,
    SHUI_IDFONTLISTUITEXT,
    SHUI_IDFONTLISTUIBOLDTEXT,
    SHUI_IDFONTNORMALTEXT,
    SHUI_IDFONTMEDIUMTEXT,
    SHUI_IDFONTLARGETEXT,
    SHUI_IDFONTSMALLBOLDTEXT,
    SHUI_IDFONTNORMALBOLDTEXT,
    SHUI_IDFONTMEDIUMBOLDTEXT,
    SHUI_IDFONTLARGEBOLDTEXT,
    SHUI_IDFONTSMALLEXTRABOLDTEXT,
    SHUI_IDFONTNORMALEXTRABOLDTEXT,
    SHUI_IDFONTMEDIUMEXTRABOLDTEXT,
    SHUI_IDFONTLARGEEXTRABOLDTEXT,
    SHUI_IDFONTSMALLHEAVYTEXT,
    SHUI_IDFONTNORMALHEAVYTEXT,
    SHUI_IDFONTMEDIUMHEAVYTEXT,
    SHUI_IDFONTLARGEHEAVYTEXT,
    SHUI_IDFONTLISTUIUNDERLINETEXT,
    SHUI_IDFONTSMALLUNDERLINETEXT,
    SHUI_IDFONTNORMALUNDERLINETEXT,
    SHUI_IDFONTMEDIUMUNDERLINETEXT,
    SHUI_IDFONTLARGEUNDERLINETEXT,
    SHUI_IDFONTSMALLBOLDUNDERLINETEXT,
    SHUI_IDFONTNORMALBOLDUNDERLINETEXT,
    SHUI_IDFONTMEDIUMBOLDUNDERLINETEXT,
    SHUI_IDFONTLARGEBOLDUNDERLINETEXT,

    // Application font size
    SHUI_APPLICATIONFONTHEIGHT,
} SHUIINFOTYPE, *PSHUIINFOTYPE;

#define SHUI_FM_FIRST SHUI_FM_LISTUITEXT
#define SHUI_FM_LAST SHUI_FM_NOTES_BOLD
#define SHUI_IDFONT_FIRST SHUI_IDFONTSYSTEM
#define SHUI_IDFONT_LAST SHUI_IDFONTLARGEBOLDUNDERLINETEXT

// Platform types

typedef enum _tagPlatformType
{
    SmartPhonePlatform,     // SmartPhone
    PocketPCPlatform,       // Pocket PC
    HandheldPCPlatform,     // Handheld PC
    InvalidPlatform = -1L,  // Unsupported or uninitialized platform
} PLATFORMTYPE, *PPLATFORMTYPE;

BOOL SHGetUiInfo(SHUIINFOTYPE shuiFlags,
                 DWORD cbUiBufSize,
                 DWORD * pcbUiBufReqd,
                 PVOID pvUiBuf);

BOOL SHSetUiInfo(SHUIINFOTYPE shuiFlags,
                 DWORD cbUiBufSize,
                 PVOID pvUiBuf);

int SHGetMetric(SHUIINFOTYPE shuiFlags);

//
// End SHGetUiInfo/SHSetUiInfo
//
//------


//++++++
//
// Lucy test mode defines
//

// TODO: We should really have a separate .h file for all this stuff, I think
// We don't really want to be crowding in all our message and object IDs here, right?

//
// Getting state information:
//
#define WM_QUERYDATA (WM_APP + 3000)        // what is safe for this?
CASSERT(WM_QUERYDATA < 0xC000);
/*
   WM_QUERYDATA:  wParam = nIDObject - specificies what information we are requesing (application defined)
                  lParam = pBuffer   - the shared memory buffer in which to put data
   Returns the number of bytes of data it wrote into the buffer.  If lParam is NULL, returns an upper
    bound on the amount of data it potentially could write into a buffer.
*/

// CALLSTATE IDs
// The 'objects' we can serve to the client:
#define CALLSTATE_RESERVED        0x00      // don't use this!
#define CALLSTATE_BASIC           LUCY_MSGID_CPROG_BASICCALLSTATE      // basic cpstats

#define CALLSTATE_CID_HELD        0x10      // CID info of various kinds
#define CALLSTATE_CID_TALKING     0x20
#define CALLSTATE_CID_INCOMING    0x40
#define CALLSTATE_CID_OUTGOING    0x80
#define CALLSTATE_CID_CONNECTED   (CALLSTATE_CID_HELD | CALLSTATE_CID_TALKING)
#define CALLSTATE_CID_UNCONNECTED (CALLSTATE_CID_INCOMING | CALLSTATE_CID_OUTGOING)         // doesn't seem terribly useful, I'll admit
#define CALLSTATE_CID_ANY         (CALLSTATE_CID_CONNECTED | CALLSTATE_CID_UNCONNECTED)


#define CCH_STATEBUFFER                 512
// Designed for the usual case.  If somebody wants to use this as a plain data buffer
//  instead of a string buffer, put the burden on them.
typedef struct tagSTATEBUFFER {
    int cchSize;
    TCHAR *pszBuffer;
} STATEBUFFER;



//
// Interacting with the test log
//

#define WM_TESTMODECHANGE WM_SETTINGCHANGE
#define WM_ADDMSGID (WM_APP + 0)

// ID for applications
#define LUCY_APPID_UNKNOWN             0x0000
#define LUCY_APPID_CPROG               0x0001
#define LUCY_APPID_CPROGCTRL           0x0002

// ID for messages, each app could have upto 0xfffe messages here, as message id's start from index 1.
// This is to stop us having to bloat retail code with text messages
#define LUCY_MSGID_CPROGCTRL_ICONSUI_DRAWING       0x0001

// CProg messages
#define LUCY_MSGID_CPROG_BASICCALLSTATE            0x001
#define LUCY_MSGID_CPROG_INCOMING                  101L
#define LUCY_MSGID_CPROG_OUTGOING                  106L
#define LUCY_MSGID_CPROG_TALKING                   102L
#define LUCY_MSGID_CPROG_HELD                      103L
#define LUCY_MSGID_CPROG_CONNECTED                 104L
#define LUCY_MSGID_CPROG_ENDED                     105L
// Add Caller ID stuff here


// Try not to use this macro in retail code with inline messages as it will bloat the code
// If idmsg is 0 then this is a generic message, else the idmsg maps to a
#define LUCY_NOTIFY_MSGSTR(testmode,idapp,idmsg,str) if (0!=(testmode)) SHLucySendMsg(idapp, idmsg, str, 0);

// This version uses messages stored in lucy phone ext. dll, and reference by idapp, idmsg
#define LUCY_NOTIFY_MSGVAL(testmode,idapp,idmsg,value) if (0!=(testmode)) SHLucySendMsg(idapp, idmsg, NULL, value);

// This macro test if we have enabled test mode and returns 0, or the app test mask (for the moment 0xffffffff)
#define LUCY_TESTMODECHANGED(idapp,tstmask) SHLucyGetTestMode(idapp, &tstmask)

// Send test message to lucy worker window
// This function when called for the first time, will find & cache the window if it has been created.
// Sending in all params as 0, will force the function to re-find the window
// To send an msgid, make sure pszMessage is null.
// The idApp, idMsg pair must exist in the lucy extension dll table, nValue is also available to return useful data back
// Where possible the developer should restrain from including any verbose text messages that will
// be in shipping code, and instead use the idApp/idMsg mechanism instead.
LRESULT SHLucySendMsg(WORD idApp, WORD idMsg, TCHAR *pszMessage, WORD nValue);

// When the app receives the broacasted WM_TESTMODECHANGE message, this helper function should be called
// to determine if the app should run in test mode, and send test messages to the lucy extension
// If *grfTskMask returns zero then test mode is off, otherwise the mask will define zones for determining
// which test messages should be sent. For the moment this will always return 0xffffffff and idApp will be ignored
void SHLucyGetTestMode(DWORD idApp, DWORD *grfTstMask);

//
// End Lucy test mode defines
//
//------

//++++++
//
// Gradient Support
//

typedef struct tagSHGRADIENT
{
    HBITMAP     hbmPrevBG;
    HBITMAP     hbmBG;
    HDC         hdcBG;
} SHGRADIENT, *PSHGRADIENT;


//
// The functions
//

BOOL SHGradientInit(PSHGRADIENT pshg, int cx, LPCTSTR pszAppName);
BOOL SHGradientDeInit(PSHGRADIENT pshg);
BOOL SHGradientDraw(HDC hdc, PSHGRADIENT pshg, RECT *prc);

//////////////////////////////////////////////////////////////
//fancy gradients for softkeys and taskbar.
//
//not not pressed
#define GRADIENT_TYPE_SK_NORMAL         0x00000001
//pressed state of button
#define GRADIENT_TYPE_SK_HIGHLIGHT      0x00000002

///////////////////////////////////////////////////
//structure for drawing
//SK gradients consist of three parts
//start - SIP - end
//there is a fix point where start and SIP meets
//there is another fix point where SIP and end meets
//The color at a fixpoint does not change between
//the highlighted and the normal version
typedef struct _gradient_color_location
{
    ULONG       dwMode;             //GRADIENT_FILL_RECT_H or GRADIENT_FILL_RECT_V or GRADIENT_FILL_TRIANGLE
    int         xOffset;            //start point
    int         yOffset;            //start point
    //locations
    union
    {
        struct //horizontal and triangle
        {
            int         cyHeight;           //height
            int         cxFixptLeft;        //distance between start and SIP
            int         cxMiddle;           //distance between fix point and middle
            int         cxFixptRight;       //distance between middle and SIP end
            int         cxEnd;              //distance to right side
        };
        struct //vertical //give it names that make more sense
        {
            int         cxWidth;            //width
            int         cyFixptTop;         //distance between start and SIP
            int         cyMiddle;           //distance between fix point and middle
            int         cyFixptBottom;      //distance between middle and SIP end
            int         cyEnd;              //distance to the bottom
        };
    };
    //colors
    union
    {
        COLORREF    clrStart;           //color at left/top
        COLORREF    clrStartTop;        //color at left top - TRIANGLE
    };
    union
    {
        COLORREF    clrFixptLeft;       //color at fix point
        COLORREF    clrFixptTop;        //color at fix point
        COLORREF    clrFixptLeftTop;    //color at fix point top - TRIANGLE
    };
    union
    {
        COLORREF    clrMiddle;              //color at middle
        COLORREF    clrMiddleTop;           //color at middle top  - TRIANGLE
    };
    union
    {
        COLORREF    clrFixptRight;      //color of second fix point
        COLORREF    clrFixptBottom;     //color of second fix point
        COLORREF    clrFixptRightTop;   //color of second fix point top  - TRIANGLE
    };
    union
    {
        COLORREF    clrEnd;                 //color at right/bottom
        COLORREF    clrEndTop;              //color at right top  - TRIANGLE
    };

    //colors for TRIANGLE - bottom
    COLORREF    clrStartBottom;             //color at left bottom
    COLORREF    clrFixptLeftBottom;         //color at fix point bottom
    COLORREF    clrMiddleBottom;            //color at middle bottom
    COLORREF    clrFixptRightBottom;        //color of second fix point bottom
    COLORREF    clrEndBottom;               //color at right bottom
}GRADIENT_COLOR_LOCATION, *PGRADIENT_COLOR_LOCATION;

typedef struct _gradient_button_background_
{
    DWORD dwNormalGradientCount;
    DWORD dwHighlightGradientCount;
    PGRADIENT_COLOR_LOCATION pgclGradientInfoNormal;
    PGRADIENT_COLOR_LOCATION pgclGradientInfoHighlight;
}GRADIENT_BUTTON_BACKGROUND, *PGRADIENT_BUTTON_BACKGROUND;


COLORREF SHGetGradientColor(int nColor);
HRESULT SHDrawMultipleGradient(HDC hdc, HBITMAP hbmGradient, PGRADIENT_COLOR_LOCATION pgclGradientInfo);
HRESULT SHDrawMultipleGradientBackground(HDC hdc, HBITMAP hbmGradient,
                                         PGRADIENT_COLOR_LOCATION pgclGradientInfo,
                                         DWORD cGradients);
//platform specific functions
BOOL SHUpdateBaseHue(__out BYTE *pbHue, __out BOOL *pfGrey);

//color indices for the gradients
#define COLOR_GRADIENTSTART                         0
#define COLOR_GRADIENTEND                           1

//these 7 colors are renamed/extended below
#define COLOR_GRADIENTDARK_MIDDLE                   2 //color name is obsolete, value reused below
#define COLOR_GRADIENTDARK_FIXPT                    3 //color name is obsolete, value reused below
#define COLOR_GRADIENTBRIGHT_MIDDLE                 4 //color name is obsolete, value reused below
#define COLOR_GRADIENTBRIGHT_FIXPT                  5 //color name is obsolete, value reused below
#define COLOR_GRADIENTCENTER_MIDDLE                 6 //color name is obsolete, value reused below
#define COLOR_GRADIENTCENTER_FIXPT                  7 //color name is obsolete, value reused below
#define COLOR_GRADIENTCENTER_MIDDLE_HIGHLIGHT       8 //not used anywhere, as the title does not have a highlight in the middle

#define COLOR_GRADIENTSTART_HIGHLIGHT               9
#define COLOR_GRADIENTEND_HIGHLIGHT                 10
#define COLOR_GRADIENTSEPARATOR_MIDDLE              11
#define COLOR_GRADIENTSEPARATOR_START               12
#define COLOR_GRADIENTSEPARATOR_END                 13
#define COLOR_SHELL_MENUTEXT                        14
#define COLOR_SHELL_MENUTEXT_HIGHLIGHT              15
#define COLOR_GRADIENTSTART_DONE                    16
#define COLOR_GRADIENTEND_DONE                      17
#define COLOR_GRADIENTSTART_DONE_HIGHLIGHT          18
#define COLOR_GRADIENTEND_DONE_HIGHLIGHT            19
#define COLOR_SHELL_COMMANDBAR                      20
#define COLOR_SHELL_COMMANDBAR_TEXT                 21

//separate colors for SK because we allow OEM to give seperate hue
#define COLOR_SK_GRADIENTSTART                      22
#define COLOR_SK_GRADIENT_FIRST                     COLOR_SK_GRADIENTSTART
#define COLOR_SK_GRADIENTEND                        23

//these 7 colors are renamed/extended below
#define COLOR_SK_GRADIENTDARK_MIDDLE                24 //color name is obsolete, value reused below
#define COLOR_SK_GRADIENTDARK_FIXPT                 25 //color name is obsolete, value reused below
#define COLOR_SK_GRADIENTBRIGHT_MIDDLE              26 //color name is obsolete, value reused below
#define COLOR_SK_GRADIENTBRIGHT_FIXPT               27 //color name is obsolete, value reused below
#define COLOR_SK_GRADIENTCENTER_MIDDLE              28 //color name is obsolete, value reused below
#define COLOR_SK_GRADIENTCENTER_FIXPT               29 //color name is obsolete, value reused below
#define COLOR_SK_GRADIENTCENTER_MIDDLE_HIGHLIGHT    30 //color name is obsolete, value reused below

#define COLOR_SK_GRADIENTSTART_HIGHLIGHT            31
#define COLOR_SK_GRADIENTEND_HIGHLIGHT              32
#define COLOR_SK_GRADIENTSEPARATOR_MIDDLE           33
#define COLOR_SK_GRADIENTSEPARATOR_START            34
#define COLOR_SK_GRADIENTSEPARATOR_END              35
#define COLOR_SK_COMMANDBAR_SIP_HIGHLIGHT           36
#define COLOR_SK_GRADIENT_LAST                      COLOR_SK_COMMANDBAR_SIP_HIGHLIGHT

#define COLOR_SHELL_DONE_X                          37
#define COLOR_SHELL_DONE_X_HIGHLIGHT                38
#define COLOR_SHELL_NAVBARTEXT_HIGHLIGHT            39

#define COLOR_SHELL_CAPTION                         40
#define COLOR_SHELL_CAPTIONGRADIENT                 41

//extend the gradient title colors:
#define COLOR_GRADIENT2DTOP_START                   42
#define COLOR_GRADIENT2DTOP_MIDDLE                  COLOR_GRADIENTDARK_MIDDLE
#define COLOR_GRADIENT2DTOP_FIXPT                   COLOR_GRADIENTDARK_FIXPT
#define COLOR_GRADIENT2DTOP_END                     43
#define COLOR_GRADIENT2DBOTTOM_START                44
#define COLOR_GRADIENT2DBOTTOM_MIDDLE               COLOR_GRADIENTCENTER_MIDDLE
#define COLOR_GRADIENT2DBOTTOM_FIXPT                COLOR_GRADIENTCENTER_FIXPT
#define COLOR_GRADIENT2DBOTTOM_END                  45
#define COLOR_GRADIENTBOTTOM_START                  46
#define COLOR_GRADIENTBOTTOM_MIDDLE                 COLOR_GRADIENTBRIGHT_MIDDLE
#define COLOR_GRADIENTBOTTOM_FIXPT                  COLOR_GRADIENTBRIGHT_FIXPT
#define COLOR_GRADIENTBOTTOM_END                    47

//extend the gradient SK colors
#define COLOR_SK_GRADIENT2DTOP_START                48
#define COLOR_SK_GRADIENT2DTOP_MIDDLE               COLOR_SK_GRADIENTDARK_MIDDLE
#define COLOR_SK_GRADIENT2DTOP_FIXPT                COLOR_SK_GRADIENTDARK_FIXPT
#define COLOR_SK_GRADIENT2DTOP_END                  49
#define COLOR_SK_GRADIENT2DBOTTOM_START             50
#define COLOR_SK_GRADIENT2DBOTTOM_MIDDLE            COLOR_SK_GRADIENTCENTER_MIDDLE
#define COLOR_SK_GRADIENT2DBOTTOM_FIXPT             COLOR_SK_GRADIENTCENTER_FIXPT
#define COLOR_SK_GRADIENT2DBOTTOM_END               51
#define COLOR_SK_GRADIENTBOTTOM_START               52
#define COLOR_SK_GRADIENTBOTTOM_MIDDLE              COLOR_SK_GRADIENTBRIGHT_MIDDLE
#define COLOR_SK_GRADIENTBOTTOM_FIXPT               COLOR_SK_GRADIENTBRIGHT_FIXPT
#define COLOR_SK_GRADIENTBOTTOM_END                 53
//highlight for the SK (when SIP depressed)
#define COLOR_SK_GRADIENT2DBOTTOM_MIDDLE_HIGHLIGHT  COLOR_SK_GRADIENTCENTER_MIDDLE_HIGHLIGHT
#define COLOR_SK_GRADIENT2DTOP_MIDDLE_HIGHLIGHT     54
#define COLOR_SK_GRADIENTBOTTOM_MIDDLE_HIGHLIGHT    55

#define COLOR_SK_GRADIENTLINEDARK_START             56
#define COLOR_SK_GRADIENTLINEDARK_MIDDLE            57
#define COLOR_SK_GRADIENTLINEDARK_END               58
#define COLOR_SK_GRADIENTLINEBRIGHT_START           59
#define COLOR_SK_GRADIENTLINEBRIGHT_MIDDLE          60
#define COLOR_SK_GRADIENTLINEBRIGHT_END             61
#define COLOR_GRADIENTLINEDARK_START                62
#define COLOR_GRADIENTLINEDARK_MIDDLE               63
#define COLOR_GRADIENTLINEDARK_END                  64
#define COLOR_GRADIENTLINEBRIGHT_START              65
#define COLOR_GRADIENTLINEBRIGHT_MIDDLE             66
#define COLOR_GRADIENTLINEBRIGHT_END                67

//color of outer edge with frame
#define COLOR_GRADIENTSTART_OUTERDONE               68
#define COLOR_GRADIENTEND_OUTERDONE                 69
#define COLOR_GRADIENTSTART_OUTERDONE_HIGHLIGHT     70
#define COLOR_GRADIENTEND_OUTERDONE_HIGHLIGHT       71
#define COLOR_SHELL_DONE_BORDER                     72

//
// HSB color support
//
//////////////////////////////////////
//THE MOST IMPORTANT VALUE
//this is the default base hue makes a nice turquoise
#define SHUI_DEFAULT_PALETTE_BASE_HUE 185  //in photoshop range
/////////////////////////////////////

//range definition for HSB values
#define  PHOTOSHOP_RANGE_H   360            // range of values for HLS scrollbars
#define  PHOTOSHOP_RANGE_S   100            // range of values for HLS scrollbars
#define  PHOTOSHOP_RANGE_B   100            // range of values for HLS scrollbars
#define  WINDOWS_RANGE_H   240            // range of values for HLS scrollbars
#define  WINDOWS_RANGE_S   240            // range of values for HLS scrollbars
#define  WINDOWS_RANGE_B   240            // range of values for HLS scrollbars

//Macros to do the conversion from Photoshop to Windows ranges
#define HUE_PHOTOSHOP_TO_WINDOWS_RANGE(H) H * WINDOWS_RANGE_H / PHOTOSHOP_RANGE_H
#define SATURATION_PHOTOSHOP_TO_WINDOWS_RANGE(S) S * WINDOWS_RANGE_S / PHOTOSHOP_RANGE_S
#define BRIGHTNESS_PHOTOSHOP_TO_WINDOWS_RANGE(B) B * WINDOWS_RANGE_B / PHOTOSHOP_RANGE_B

//Macros to do the conversion from Windows to Photoshop ranges
#define HUE_WINDOWS_TO_PHOTOSHOP_RANGE(H) (H * PHOTOSHOP_RANGE_H / WINDOWS_RANGE_H)
#define SATURATION_WINDOWS_TO_PHOTOSHOP_RANGE(S) (S * PHOTOSHOP_RANGE_S / WINDOWS_RANGE_S)
#define BRIGHTNESS_WINDOWS_TO_PHOTOSHOP_RANGE(B) (B * PHOTOSHOP_RANGE_B / WINDOWS_RANGE_B)

//macro defining a color as HSB in Windows based on Photoshop values
#define COLOR_HSB_PHOTOSHOP_RANGE(H,S,B)    HSB( \
                                                HUE_PHOTOSHOP_TO_WINDOWS_RANGE(H), \
                                                SATURATION_PHOTOSHOP_TO_WINDOWS_RANGE(S), \
                                                BRIGHTNESS_PHOTOSHOP_TO_WINDOWS_RANGE(B) \
                                               )

#define HSBGetHValue(hsb)      ((BYTE)(hsb))
#define HSBGetSValue(hsb)      ((BYTE)(((WORD)(hsb)) >> 8))
#define HSBGetBValue(hsb)      ((BYTE)((hsb)>>16))
#define HSB(h,s,b)          ((COLORREF)(((BYTE)(h)|((WORD)((BYTE)(s))<<8))|(((DWORD)(BYTE)(b))<<16)))

BOOL SHUpdateSysColors();

//
// End Gradient Support
//
//------


// Shell Notifications API
//
// this is a private priority level

#define SHNP_PERSISTENT ((SHNP)((int)SHNP_INFORM - 1))

#ifndef SHIP_BUILD // {
//***   perf flags
//

extern DWORD g_dwPerf1;

#define PF_NICON    (g_dwPerf1 & 0x0001)    // suppress icons
#define PF_NTEXT    (g_dwPerf1 & 0x0002)    // suppress text
#define PF_NWATER   (g_dwPerf1 & 0x0004)    // suppress watermark

#ifdef UNICODE // (workaround mimeole NOT_UNICODE pblm)
__inline void PF_OnSettingChange()
{
        DWORD dwTmp = sizeof(g_dwPerf1);
        SHRegQueryValueEx(HKEY_CURRENT_USER,
            TEXT("Perf1"), (DWORD *)TEXT("ControlPanel\\Debug"),
            NULL, (BYTE *)&g_dwPerf1, &dwTmp);
        if (g_dwPerf1)
            RETAILMSG(1, (TEXT("Perf1: 0x%x\r\n"), g_dwPerf1));
        return;
}
#endif
#endif // }

//
// UIH - UI Helpers
//

// helpers to limit the text for a set of edit controls
typedef struct _HELP_EM_LIMITTEXT
{
    UINT    uiID;
    WPARAM  wSize;
} HELP_EM_LIMITTEXT;

#define LIMITTEXT_CONTROLS(hDlg, controlArray) UIHLimitTextControls( hDlg, (HELP_EM_LIMITTEXT*)controlArray, ARRAYSIZE(controlArray) )

// Limits the text on an array of edit controls
void UIHLimitTextControls( HWND h, HELP_EM_LIMITTEXT * pIDs, UINT count);

#ifdef __cplusplus
// Enable / Disable an array of controls
#define ENABLE_CONTROLS(hDlg, controlArray, enabled) UIHEnableControls( hDlg, (UINT*)controlArray, ARRAYSIZE(controlArray), enabled )
inline void UIHEnableControls( HWND h, UINT * pIDs, UINT count, UINT enabled )
{
    while(count--)
        EnableWindow( GetDlgItem(h,*pIDs++), enabled );
}

#define SHOW_CONTROLS(hDlg, controlArray, visible) UIHShowControls( hDlg, (UINT*)controlArray, ARRAYSIZE(controlArray), visible)
inline void UIHShowControls( HWND h, UINT * pIDs, UINT count, UINT visible )
{
    while(count--)
        ShowWindow( GetDlgItem(h,*pIDs++), visible );
}
#endif

// Maps ui controls to win32 structs
typedef struct _TEXT_TO_STRUCT_MAPPER
{
    UINT    uiControlID;  //
    UINT    cch;          // how many characters there are
    DWORD   dwOffset;     // offset into the struct where the data is.
} TEXT_TO_STRUCT_MAPPER, *PTEXT_TO_STRUCT_MAPPER;

#define TEXT_TO_UI( ctlID, structname, fieldname ) { ctlID, sizeof( ( (*(structname *)0). fieldname) ) / sizeof(TCHAR), offsetof( structname, fieldname ) },

#define SET_TEXT_CONTROLS(hDlg, pStruct, controlArray) UIHSetTextFromStruct( hDlg, (LPBYTE)pStruct, (PTEXT_TO_STRUCT_MAPPER)controlArray, ARRAYSIZE(controlArray) )
void UIHSetTextFromStruct( HWND hDlg, LPBYTE pStruct, PTEXT_TO_STRUCT_MAPPER pMap, UINT size );

#define GET_TEXT_CONTROLS(hDlg, pStruct, controlArray) UIHGetTextToStruct( hDlg, (LPBYTE)pStruct, (PTEXT_TO_STRUCT_MAPPER)controlArray, ARRAYSIZE(controlArray) )
UINT UIHGetTextToStruct( HWND hDlg, LPBYTE pStruct, PTEXT_TO_STRUCT_MAPPER pMap, UINT size );

// Maps ui controls HWNDs into win32 structs
// Maps a struct to some XML entries.
typedef struct _HWND_TO_STRUCT_MAPPER
{
    UINT    uiControlID;  //
    DWORD   dwOffset;     // offset into the struct where the data is.
} HWND_TO_STRUCT_MAPPER, *PHWND_TO_STRUCT_MAPPER;

#define HWND_TO_UI( ctlID, structname, fieldname ) { ctlID, offsetof( structname, fieldname ) },

#define SET_HWND_CONTROLS(hDlg, pStruct, controlArray) UIHSetHWNDToStruct( hDlg, (LPBYTE)pStruct, (PHWND_TO_STRUCT_MAPPER)controlArray, ARRAYSIZE(controlArray) )
void UIHSetHWNDToStruct( HWND hDlg, LPBYTE pStruct, PHWND_TO_STRUCT_MAPPER pMap, UINT size );

// Use SHRunSafeApplet() to launch control panel applets (typically in CPLMAIN.CPL) from
// your application.  This routine is asynchronous; a thread is spawned and the applet
// is run on that thread.
//

#define APPLET_CPL_MAIN         TEXT("cplmain.cpl")

// These indices need to keep in sync with appropriate CPL DLL 
#define APPLETID_CONTRAST_CPL       (0)
#define APPLETID_PASSWD_CPL         (1)
#define APPLETID_PROFILE_CPL        (2)
#define APPLETID_POWER_CPL          (3)
#define APPLETID_MEMORY_CPL         (4)
#define APPLETID_ABOUT_CPL          (5)
#define APPLETID_SCREEN_CPL         (6)
#define APPLETID_STYLUS_CPL         (7)
#define APPLETID_SIP_CPL            (8)
#define APPLETID_SOUNDS_CPL         (9)
#define APPLETID_REMOVE_CPL         (10)
#define APPLETID_MENUS_CPL          (11)
#define APPLETID_BUTTONS_CPL        (12)
#define APPLETID_TODAY_CPL          (13)
#define APPLETID_BEAM_CPL           (15)
#define APPLETID_CLOCK_CPL          (16)
#define APPLETID_NETWORK_CPL        (17)
#define APPLETID_REGIONAL_CPL       (18)
#define APPLETID_CONNECTIONS_CPL    (19)
#define APPLETID_PHONE_CPL          (20)
#define APPLETID_CERTS_CPL          (22)
#define APPLETID_BLUETOOTH_CPL      (23)
#define APPLETID_WATSON_CPL         (24)
#define APPLETID_GPS_CPL            (25)
#define APPLETID_WRLSMGR_CPL        (26)
#define APPLETID_ENCRYPTION_CPL     (27)
#define APPLETID_PCCONNECTION_CPL   (28)
#define APPLETID_CEIPUI_CPL         (29)


void SHRunSafeApplet(LPCTSTR pszApplet, DWORD dwIndex, DWORD nTabIndex);

// SHGetLocaleInfo -- Get locale info as given by the current UI
// locale. Most locale info is UI locale independent, but localized
// strings (such as the names of languages and countries, regions, or
// other special forms) are not.

int SHGetLocaleInfo(
    LCID Locale,                /* Locale for which to return info */
    LCTYPE LCType,              /* Type of info to retrieve */
    LPTSTR lpLCData,            /* Pointer to a buffer to return data in */
    int cchData                 /* Number of characters in the buffer */
    );

// Temporarily implemented here until os implements CopyIcon
HICON SHCopyIcon(HICON hicon);

// Get original class name of an ATL-wrapped control class.
// If passed in class name indicates that corresponding control is NOT ATL-wrapped,
// then returns parameter unmodified.
// If passed in class name indicates that corresponding control IS ATL-wrapped,
// then returns class name of equivalent non-ATL-wrapped control.
LPTSTR SHOriginalClassNameFromATL(LPTSTR pszAtlClassName);

typedef struct
{
    LPVOID *ppDest;     // Will contain result of the mapping
    LPVOID  pSrc;       // Pointer to be mapped to caller and validated
    DWORD   dwLen;      // Length, in bytes, of the region of memory being validated
} MAPCPDATA;

// Safely map the source pointers to the caller process, checking for errors in the mapping
// or in the size of the validated data
HRESULT SHMapCallerPtrArray(__inout_ecount(cMap) MAPCPDATA *rgmcpdMap, UINT cMap);

/////////////////////////////////////////////////////////////////////////////////////////////////////

//
// DPI awareness
//

// can't include macros.h as w32em.h complains, and using just MulDiv upsets utils.cpp in shell
// The two macros HIDPISIGN and HIDPIABS are there to ensure correct rounding
// for negative numbers passed into HIDPIMulDiv as a (we want -1.5 to round to -1, 2.5 to round to 2, etc).
// So we use the absolute value of a, and then multiply the result by the sign of a.
// b and c should never be negative, as b is the dpi of the device (presumably 192 or 96), and
// c is always 96, as that is our original dpi we developed on.
#ifndef HIDPIMulDiv
#define HIDPISIGN(a) (((a)<0)?-1:1)
#define HIDPIABS(a) (((a)<0)?-(a):(a))

#define HIDPIMulDiv(a,b,c) ((((HIDPIABS(a)*(b))+((c)>>1))/(c))*HIDPISIGN(a))
#define HIDPIMulDivFloor(a,b,c) (((HIDPIABS(a))*((b)/(c)))*HIDPISIGN(a))

#endif

//$REVIEW: HIDPIMulDiv is a macro, and not an inline function, because C does not seem
// to obey the __inline directive. This means that macros that use HIDPIMulDiv
// (SCALE*, UNSCALE*) should only be called on simple arguments such as literals, constants
// and variables. These macros should NEVER be called directly on a function or complex
// expression, because the arguments to HIDPIMulDiv get used many times within the macro.
// For the same reason, one of these macros should NEVER, EVER be embedded within another.
// E.g.: The construct SCALEX(UNSCALEX(SHGetMetric(nMetric))) generates code that calls
//       SHGetMetric(nMetric) 16 times.
// An alternative to having to follow this rule is to figure out why __inline functions are
// not expanded out in C files, solving this problem (if there's a way) and making this
// and inline function.

// how much to scale a design that assumes 96-DPI pixels
EXTERN_C int g_HIDPI_LogPixelsX;
EXTERN_C int g_HIDPI_LogPixelsY;

// You need to define these somewhere in your module only if you make use of the scaling macros.
#define HIDPI_ENABLE \
    int g_HIDPI_LogPixelsX; \
    int g_HIDPI_LogPixelsY;

// Scale a screen pixel constant specified in 96 DPI into the screen DPI,
// using cached DPI values in module.

#define SCALEX(argX) (HIDPIMulDiv(argX,g_HIDPI_LogPixelsX,96))
#define SCALEY(argY) (HIDPIMulDiv(argY,g_HIDPI_LogPixelsY,96))

// This will scale X & Y without trying to round it
#define SCALEXFLOOR(argX) (HIDPIMulDivFloor(argX,g_HIDPI_LogPixelsX,96))
#define SCALEYFLOOR(argY) (HIDPIMulDivFloor(argY,g_HIDPI_LogPixelsY,96))

// Scale a screen pixel constant specified in 96 DPI into the screen DPI,
// recomputing screen DPI every time. This is necessary because some threads
// are owned by a pixel-doubled process, and the DPI perceived by the owner
// process may be different from the DPI perceived by the current process.
// These macros always use the DPI perceived by the owner process.
// Useful in common APIs that compare a scaled constant against some system
// metric obtained through a pixel-doubled function (which would perceive
// DPI like the owner process).
#define SCALEX_NOCACHE(argX) (HIDPIMulDiv(argX,GetDeviceCaps(NULL, LOGPIXELSX),96))
#define SCALEY_NOCACHE(argY) (HIDPIMulDiv(argY,GetDeviceCaps(NULL, LOGPIXELSY),96))


// Convert a screen pixel quantity specified in screen DPI into 96 DPI, for
// comparison purposes
#define UNSCALEX(argX) (HIDPIMulDiv(argX,96,g_HIDPI_LogPixelsX))
#define UNSCALEY(argY) (HIDPIMulDiv(argY,96,g_HIDPI_LogPixelsY))
#define UNSCALERECT(rc) {HIDPI_Dbg_ChkCtor(); rc.left = UNSCALEX(rc.left); rc.right = UNSCALEX(rc.right); rc.top = UNSCALEY(rc.top); rc.bottom = UNSCALEY(rc.bottom);}

#define SCALERECT(rc) {HIDPI_Dbg_ChkCtor(); rc.left = SCALEX(rc.left); rc.right = SCALEX(rc.right); rc.top = SCALEY(rc.top); rc.bottom = SCALEY(rc.bottom);}
#define SCALERECT_NOCACHE(rc) {HIDPI_Dbg_ChkCtor(); rc.left = SCALEX_NOCACHE(rc.left); rc.right = SCALEX_NOCACHE(rc.right); rc.top = SCALEY_NOCACHE(rc.top); rc.bottom = SCALEY_NOCACHE(rc.bottom);}
#define SCALEPT(pt) {HIDPI_Dbg_ChkCtor(); pt.x = SCALEX(pt.x); pt.y = SCALEY(pt.y);}


//
//@func void    | HIDPI_InitScaling  | Initialize scaling in a module
//
//@comm Should be called once at module startup (WinMain or DLL_PROCESS_ATTACH) for every
//      module that uses SCALEX and SCALEY
__inline void HIDPI_InitScaling()
{
    // After Gdi::Initialize() is called GWES will call GetDeviceCaps and
    //   store the pixels per inch for the display in the registry
    DWORD cbSize;

    cbSize = sizeof(g_HIDPI_LogPixelsX);
    if (ERROR_SUCCESS != RegQueryValueEx(HKEY_LOCAL_MACHINE, TEXT("LogicalPixelsX"),
                                         (DWORD*)TEXT("SYSTEM\\GWE\\Display"),
                                         NULL, (BYTE*)&g_HIDPI_LogPixelsX, &cbSize))
    {
        g_HIDPI_LogPixelsX = 96;
    }
    ASSERT(g_HIDPI_LogPixelsX > 0);

    cbSize = sizeof(g_HIDPI_LogPixelsY);
    if (ERROR_SUCCESS != RegQueryValueEx(HKEY_LOCAL_MACHINE, TEXT("LogicalPixelsY"),
                                         (DWORD*)TEXT("SYSTEM\\GWE\\Display"),
                                         NULL, (BYTE*)&g_HIDPI_LogPixelsY, &cbSize))
    {
        g_HIDPI_LogPixelsY = 96;
    }
    ASSERT(g_HIDPI_LogPixelsY > 0);
}

#define HIDPI_Dbg_ChkCtor() ASSERT(g_HIDPI_LogPixelsX != 0)

//
//@func BOOL    | SHStretchBitmap  | Stretch images in a bitmap, in place
//
//@parm HBITMAP*    | phbm      | [in/out] Bitmap to be stretched
//@parm int         | cxDstImg  | Desired width of an image in the bitmap
//@parm int         | cyDstImg  | Desired height of an image in the bitmap
//@parm int         | cImagesX  | Number of images in the bitmap, horizontally
//@parm int         | cImagesY  | Number of images in the bitmap, vertically
//
//@rdesc    TRUE indicates that bitmap was stretched.
//
//@comm
// SHStretchBitmap can be used to stretch an entire bitmap or to stretch
// an image array bitmap image per image.
//
// SHStretchBitmap(phbm, cxDstImg, cyDstImg, 1, 1) means stretch entire bitmap.
//
// SHStretchBitmap(phbm, cxDstImg, cyDstImg, x, y) means stretch the images in a bitmap
//   which has y rows and x images in each row.
//
// So cxDstImg * cImagesX is the target bitmap width and cyDstImg * cImagesY is the height.
//
BOOL SHStretchBitmap(
    HBITMAP* phbm,
    int cxDstImg,
    int cyDstImg,
    int cImagesX,
    int cImagesY
    );
#define HIDPI_StretchBitmap         SHStretchBitmap


//
//@func BOOL    | SHStretchIcon  | Stretch an icon, in place
//
//@parm HICON*  | phbm      | [in/out] Icon to be stretched
//@parm int     | cxIcon    | Desired width of icon
//@parm int     | cyIcon    | Desired height of icon
//
//@rdesc    TRUE indicates that icon was stretched.
//
BOOL SHStretchIcon(
    HICON* phic,
    int cxIcon,
    int cyIcon
    );

//
//@func BOOL    | SHGetBitmapLogPixels  | Get the logical DPI of a bitmap
//
//@parm HINSTANCE   | hinst         | Instance of the module where bitmap is located
//@parm LPCTSTR     | lpbmp         | Name of bitmap resource in instance
//                                    (same semantics as for LoadBitmap)
//@parm int*        | pnLogPixelsX  | [out] Horizontal logical DPI of bitmap (defaults to 96)
//@parm int*        | pnLogPixelsY  | [out] Vertical logical DPI of bitmap (defaults to 96)
//
//@rdesc    TRUE indicates that DPI was retrieved successfully.
//
//@comm
// We return any DPI less than 96 DPI as 96 DPI (72 used for fonts)
//
BOOL SHGetBitmapLogPixels(
    HINSTANCE hinst,
    LPCTSTR lpbmp,
    int* pnLogPixelsX,
    int* pnLogPixelsY
    );
#define HIDPI_GetBitmapLogPixels    SHGetBitmapLogPixels

#ifndef WPC_NEW
#ifdef _INC_COMMCTRL
#ifndef NOIMAGEAPIS

#ifndef ILC_COLORMASK
#define ILC_COLORMASK 0x00FE          // Seems to be defined only in commctrl.h, not pcommctr.h??
#endif

//@func HIMAGELIST  | HIDPI_ImageList_LoadImage | Wrapper around ImageList_LoadImage
//                                                to handle stretching on load
//@comm
// Same semantics as ImageList_LoadImage.
//
HIMAGELIST HIDPI_ImageList_LoadImage(
    HINSTANCE hinst,
    LPCTSTR lpbmp,
    int cx,
    int cGrow,
    COLORREF crMask,
    UINT uType,
    UINT uFlags
    );

//@func int | HIDPI_ImageList_ReplaceIcon   | Wrapper around ImageList_ReplaceIcon to
//                                            handle stretching when icons are changed
//@comm
// Same semantics as ImageList_ReplaceIcon.
//
int HIDPI_ImageList_ReplaceIcon(
    HIMAGELIST himl,
    int i,
    HICON hicon
    );

//@func int | HIDPI_ImageList_AddIcon   | Wrapper around ImageList_AddIcon to
//                                        handle stretching when icons are added
//@comm
// Same semantics as ImageList_AddIcon.
//
#define HIDPI_ImageList_AddIcon(himl, hicon) HIDPI_ImageList_ReplaceIcon(himl, -1, hicon)

#endif  // using imagelist
#endif  // commctrl.h
#endif  // WPC_NEW - common controls themselves bring in wpcpriv with this define

//
//@func HRESULT | SHStretchBltBitmap  | Draws an entire bitmap into the desired HDC,
//                                      stretching if necessary to meet desired dimensions.
//
//@parm HDC     | hdcDest   | Destination HDC (bitmap will be stretched here)
//@parm int     | xDest     | x-coord of the upper-left corner of destination rect
//@parm int     | yDest     | y-coord of the upper-left corner of destination rect
//@parm int     | cxDest    | width of destination rect
//@parm int     | cyDest    | height of destination rect
//@parm DWORD   | dwRop     | raster operation to be performed (directly passed to StretchBlt)
//@parm HBITMAP | hbm       | handle to bitmap to be stretched
//
//@rdesc    E_FAIL on failure, S_OK on successful stretch.
//
HRESULT SHStretchBltBitmap(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest, DWORD dwRop, HBITMAP hbm);
#define HIDPI_StretchBltBitmap      SHStretchBltBitmap

//
//@func HRESULT | SHStretchBltBitmapEx  | Draws an entire bitmap into the desired HDC, via
//                                        the specified source HDC, stretching if necessary
//                                        to meet desired dimensions.
//
//@parm HDC     | hdcDest   | Destination HDC (bitmap will be stretched here)
//@parm int     | xDest     | x-coord of the upper-left corner of destination rect
//@parm int     | yDest     | y-coord of the upper-left corner of destination rect
//@parm int     | cxDest    | width of destination rect
//@parm int     | cyDest    | height of destination rect
//@parm HDC     | hdcSrc    | Source DC (bitmap will be selected here)
//@parm DWORD   | dwRop     | raster operation to be performed (directly passed to StretchBlt)
//@parm HBITMAP | hbm       | handle to bitmap to be stretched
//
//@rdesc    E_FAIL on failure, S_OK on successful stretch.
//
//@comm
// Same as SHStretchBltBitmap, but allows caller to specify a source HDC
// (in case one exists that can be reused).
//
HRESULT SHStretchBltBitmapEx(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest,
                                      HDC hdcSrc, DWORD dwRop, HBITMAP hbm);
#define HIDPI_StretchBltBitmapEx    SHStretchBltBitmapEx

//
//@func BOOL   | SHBorderRectangle    | Thunk into GDI's Rectangle, with added bonus of making pens of
//                                      SM_CXBORDER width draw as if they had PS_INSIDEFRAME style.
//
//@parm HDC | hdc       | IN - Device Context
//@parm int | nLeft     | IN - Left bound of rectangle (included in drawing)
//@parm int | nTop      | IN - Top bound of rectangle (included in drawing)
//@parm int | nRight    | IN - Right bound of rectangle (NOT included in drawing)
//@parm int | nBottom   | IN - Bottom bound of rectangle (NOT included in drawing)
//
//@rdesc Nonzero indicates success. Zero indicates failure.
//
//@comm This function selects the border pen into the hdc for you.
//
BOOL SHBorderRectangle(HDC hdc, int nLeft, int nTop, int nRight, int nBottom);

//
//@func BOOL   | SHRectangle    | Thunk into GDI's Rectangle, with added bonus of making pens of
//                                any width draw as if they had PS_INSIDEFRAME style.
//                                Gets currently selected pen thickness from hdc.
//
//@parm HDC | hdc       | IN - Device Context
//@parm int | nLeft     | IN - Left bound of rectangle (included in drawing)
//@parm int | nTop      | IN - Top bound of rectangle (included in drawing)
//@parm int | nRight    | IN - Right bound of rectangle (NOT included in drawing)
//@parm int | nBottom   | IN - Bottom bound of rectangle (NOT included in drawing)
//
//@rdesc Nonzero indicates success. Zero indicates failure.
//
//@comm This function assumes that the selected pen is or draws like PS_SOLID.
//      NOTE: This function is slower than SHBorderRectangle. Use only when lines in question
//            would really be thicker or thinner than SM_CXBORDER.
//
BOOL SHRectangle(HDC hdc, int nLeft, int nTop, int nRight, int nBottom);


#define PS_RIGHTBIAS    0x10
#define PS_LEFTBIAS     0x20
#define PS_DOWNBIAS     0x40
#define PS_UPBIAS       0x80
#define PS_DOWNRIGHT    (PS_DOWNBIAS | PS_RIGHTBIAS)
#define PS_UPLEFT       (PS_UPBIAS | PS_LEFTBIAS)

#define PS_BIAS_MASK    (PS_RIGHTBIAS | PS_LEFTBIAS | PS_DOWNBIAS | PS_UPBIAS)

//
//@func BOOL   | SHBorderPolyline    | Thunk into GDI's Polyline, with added bonus of controlling how lines
//                                     thicker than one pixel are drawn. Assumes pen selected has SM_CXBORDER width.
//
//@parm HDC             | hdc       | IN - Device Context
//@parm const POINT *   | lppt      | IN - Array of POINTS that specify line to draw
//@parm int             | cPoints   | IN - Number of points in array
//@parm int             | nStyle    | IN - Style in which we want to draw line (PS_DOWNRIGHT, PS_UPLEFT
//                                         or an existing pen style, like PS_SOLID).
//
//@rdesc Nonzero indicates success. Zero indicates failure.
//
//@comm The points specified in lppt will always be included in the polyline drawn and are the
//      "reference line".
//      - PS_RIGHTBIAS, PS_LEFTBIAS, PS_DOWNBIAS, PS_UPBIAS, PS_DOWNRIGHT and PS_UPLEFT allow
//        you to specify the direction towards which the thickness will be drawn, with respect
//        to the reference line. E.g.: PS_DOWNRIGHT specifies that the thickness will be drawn
//        downwards and to the right of the reference line.
//      - If other pen styles are specified, such as PS_SOLID, callers get the default Polyline
//        behavior, which is to center the pen drawing around the reference line.
//      NOTE: This function selects the border pen into the hdc for you.
//
BOOL SHBorderPolyline(HDC hdc, const POINT *lppt, int cPoints, int nStyle);

//
//@func BOOL   | SHPolyline    | Thunk into GDI's Polyline, with added bonus of controlling how lines
//                               thicker than one pixel are drawn. Gets currently selected pen thickness from hdc.
//
//@parm HDC             | hdc       | IN - Device Context
//@parm const POINT *   | lppt      | IN - Array of POINTS that specify line to draw
//@parm int             | cPoints   | IN - Number of points in array
//@parm int             | nStyle    | IN - Style in which we want to draw line (PS_DOWNRIGHT, PS_UPLEFT
//                                         or an existing pen style, like PS_SOLID).
//
//@rdesc Nonzero indicates success. Zero indicates failure.
//
//@comm The points specified in lppt will always be included in the polyline drawn and are the
//      "reference line".
//      - PS_RIGHTBIAS, PS_LEFTBIAS, PS_DOWNBIAS, PS_UPBIAS, PS_DOWNRIGHT and PS_UPLEFT allow
//        you to specify the direction towards which the thickness will be drawn, with respect
//        to the reference line. E.g.: PS_DOWNRIGHT specifies that the thickness will be drawn
//        downwards and to the right of the reference line.
//      - If other pen styles are specified, such as PS_SOLID, callers get the default Polyline
//        behavior, which is to center the pen drawing around the reference line.
//      NOTE: This function assumes that the selected pen is or draws like PS_SOLID.
//            This function is slower than SHBorderPolyline. Use only when lines in question
//            would really be thicker or thinner than SM_CXBORDER.
//
BOOL SHPolyline(HDC hdc, const POINT *lppt, int cPoints, int nStyle);


int SHSetStretchMode(HDC hdc);


#ifdef __cplusplus


class HIDPI
{
public:
    static void InitScaling()
    {
        HIDPI_InitScaling();
    }

    static inline void Dbg_ChkCtor()
    {
        // if this fires, you need to call HIDPI::InitScaling in your modules ‘main’ init routine
        HIDPI_Dbg_ChkCtor();
    }

#ifndef WPC_NEW
#ifdef _INC_COMMCTRL
#ifndef NOIMAGEAPIS

    static  HIMAGELIST ImageList_LoadImage(
        HINSTANCE hi,
        LPCTSTR lpbmp,
        int cx,
        int cGrow,
        COLORREF crMask,
        UINT uType,
        UINT uFlags
        )
    {
        return HIDPI_ImageList_LoadImage(hi, lpbmp, cx, cGrow, crMask, uType, uFlags);
    }

    static int ImageList_ReplaceIcon(
        HIMAGELIST himl,
        int i,
        HICON hicon
        )
    {
        return HIDPI_ImageList_ReplaceIcon(himl, i, hicon);
    }

#endif  // using imagelist
#endif  // commctrl.h
#endif  // WPC_NEW - common controls themselves bring in wpcpriv with this define

    static HRESULT StretchBltBitmap(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest, DWORD dwRop, HBITMAP hbm)
    {
        return SHStretchBltBitmap(hdcDest, xDest, yDest, cxDest, cyDest, dwRop, hbm);
    }

    static HRESULT StretchBltBitmapEx(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest,
                                      HDC hdcSrc, DWORD dwRop, HBITMAP hbm)
    {
        return SHStretchBltBitmapEx( hdcDest, xDest, yDest, cxDest, cyDest, hdcSrc, dwRop, hbm);
    }

    static  BOOL StretchBitmap(
        HBITMAP* phbm,
        int cxDest,
        int cyDest
        )
    {
        return SHStretchBitmap(phbm, cxDest, cyDest, 1, 1);
    }

    static  BOOL StretchImages(
        HBITMAP* phbm,
        int cxDest,
        int cyDest,
        int cImagesX,
        int cImagesY
        )
    {
        return SHStretchBitmap(phbm, cxDest, cyDest, cImagesX, cImagesY);
    }

private:
    HIDPI() {};
    ~HIDPI() {};

};

#else
// "HIDPI class only available for C++ files"
#endif // #ifdef __cplusplus

/*
SHDrawIconEx

This function draws an icon in the specified device context, performing the raster operations as specified.
The behavior is the same as DrawIconEx. Additionally, if the icon contains an alpha channel the icon will be
alpha blended.

Parameters
hdc
    [in] Handle to the device context for a window.
xLeft
    [in] Specifies the logical x-coordinate of the upper-left corner of the icon.
yTop
    [in] Specifies the logical y-coordinate of the upper-left corner of the icon.
hIcon
    [in] Handle to the icon to be drawn. The icon resource must have been previously loaded by using the LoadImage function.
cxWidth
    [in] Specifies the logical width of the icon. This parameter must be zero or the native dimensions of the icon.
cyWidth
    [in] Specifies the logical height of the icon. This parameter must be zero or the native dimensions of the icon.
istepIfAniCur
    [in] Animated icons are not supported; set to zero.
hbrFlickerFreeDraw
    [in] Specifies that the system draws the icon directly into the device context; set to NULL.
diFlags
    [in] Specifies the following drawing flag. Value Description
    DI_NORMAL Draws the icon using the image and the mask.
    DI_IMAGE Draws the icon or cursor using the image.
    DI_MASK Draws the icon or cursor using the mask.

*/
BOOL SHDrawIconEx(
  HDC hdc,
  int xLeft,
  int yTop,
  HICON hIcon,
  int cxWidth,
  int cyWidth,
  UINT istepIfAniCur,
  HBRUSH hbrFlickerFreeDraw,
  UINT diFlags
);

/*SHDrawIcon
This function draws an icon in the specified device context.
The behavior is the same as DrawIconEx. Additionally, if the icon contains an alpha channel the icon will be
alpha blended.

hDC
[in] Handle to the device context for a window.
X
[in] Specifies the logical x-coordinate of the upper-left corner of the icon.
Y
[in] Specifies the logical y-coordinate of the upper-left corner of the icon.
hIcon
Handle to the icon to be drawn. The icon resource must have been previously loaded by using the LoadIcon or LoadImage functions.
*/

BOOL SHDrawIcon(
  HDC hdc,
  int xLeft,
  int yTop,
  HICON hIcon
);

BOOL SHDrawIconColor(
  HDC hdc,
  int xLeft,
  int yTop,
  HICON hIcon,
  int cxWidth,
  int cyWidth,
  UINT istepIfAniCur,
  HBRUSH hbrFlickerFreeDraw,
  UINT diFlags,
  COLORREF clrForeground
);

//
//@func HINSTANCE   | DPI_LoadLibraryRes    | Loads the DPI satellite of the
//                                            specified resource library.
//
//@parm const TCHAR*    | szLibFileName | Name of library to load
//
//@rdesc    Same as LoadLibraryEx
//
//@comm
//  If caller specifies "shellresdll.dll", the function will load "shellresdll.192.dll"
//  in a 192 DPI system and "shellresdll.96.dll" in a 96 DPI system.
//  Library will be loaded as datafile (LOAD_LIBRARY_AS_DATAFILE), so it's OK to call
//  this function from a DLL_PROCESS_ATTACH, unlike with other LoadLibrary calls.
//
HINSTANCE DPI_LoadLibraryRes(const TCHAR* szLibFileName);

//
//@func HBITMAP | DPI_LoadImageFile | Loads the DPI satellite image of the
//                                    specified image file.
//
//@parm LPCTSTR | szFileName | Name of image to load
//
//@rdesc    Same as LoadImageFile
//
//@comm
//  If caller specifies "image.gif", the function will load "image.192.gif"
//  in a 192 DPI system and "image.96.gif" in a 96 DPI system.
//
HBITMAP DPI_LoadImageFile(LPCTSTR szFileName);

//
//@func HICON   | DPI_ExtractIconEx | Loads the specified icon from the DPI
//                                    satellite of the specified module.
//
//@parm LPCTSTR     | lpszFile      | module name
//@parm int         | nIconIndex    | index of first icon to extract
//@parm HICON FAR*  | phIconLarge   | [out] Large icons extracted
//@parm HICON FAR*  | phIconSmall   | [out] Small icons extracted
//@parm UINT        | nIcons        | number of icons to extract
//
//@rdesc    Same as ExtractIconEx
//
//@comm
//  If caller specifies "officeres.dll" as the module, the function will try to
//  load the icon first from "officeres.192.dll" in a 192 DPI system
//  ("officeres.96.dll" in a 96 DPI system) and then from "officeres.dll" if the
//  first attempt to load failed.
//
HICON DPI_ExtractIconEx(LPCTSTR lpszFile, int nIconIndex, HICON FAR *phIconLarge,
                        HICON FAR *phIconSmall, UINT nIcons);

__inline HBITMAP _dpi_LoadBitmap(HINSTANCE hInst, LPCTSTR szBitmapName)
{
    HBITMAP hbm = LoadBitmap(hInst, szBitmapName);
    // If you hit this ASSERT, that means the bitmap is not in satellite DLL.
    ASSERT(hbm);
    return hbm;
}

__inline HICON _dpi_LoadIcon(HINSTANCE hInst, LPCTSTR szIconName)
{
    HICON hicon = LoadIcon(hInst, szIconName);
    // If you hit this ASSERT, that means the icon is not in satellite DLL.
    ASSERT(hicon);
    return hicon;
}

__inline HANDLE _dpi_LoadImage(HINSTANCE hInst, LPCTSTR szName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
{
    HANDLE handle = LoadImage(hInst, szName, uType, cxDesired, cyDesired, fuLoad);
    // If you hit this ASSERT, that means the image is not in satellite DLL.
    ASSERT(handle);
    return handle;
}

__inline HRSRC _dpi_FindResource(HMODULE hModule, LPCTSTR szName, LPCTSTR szType)
{
    HRSRC hrSrc = FindResource(hModule, szName, szType);
    // If you hit this ASSERT, that means the resource is not in satellite DLL.
    ASSERT(hrSrc);
    return hrSrc;
}

__inline HGLOBAL _dpi_LoadResource(HMODULE hModule, HRSRC hResInfo)
{
    HGLOBAL hGlobal = LoadResource(hModule, hResInfo);
    // If you hit this ASSERT, that means the resource is not in satellite DLL.
    ASSERT(hGlobal);
    return hGlobal;
}

//@topic DPI-satellite Instance-loader helpers |
//          These are macros that cause loading to happen from a DPI satellite rather
//          than a base resource module. They assume that if you specified a hInst
//          variable name, such as g_hInstRes, there will be an existing variable name
//          that represents the DPI satellite for that module such as g_hInstResDPI,
//          i.e., with "DPI" appended to the variable name. The DPI satellite instance
//          will have been initialized through DPI_LoadLibraryRes.
//
//@flag DPI_LoadBitmap          | Thunk to LoadBitmap
//@flag DPI_LoadIcon            | Thunk to LoadIcon
//@flag DPI_LoadImage           | Thunk to LoadImage
//@flag DPI_ImageList_LoadImage | Thunk to HIDPI_ImageList_LoadImage
//@flag DPI_FindResource        | Thunk to FindResource
//@flag DPI_LoadResource        | Thunk to LoadResource
//
#define DPI_LoadBitmap(hInst, BitmapName) _dpi_LoadBitmap(hInst ## DPI, BitmapName)
#define DPI_LoadIcon(hInst, IconName) _dpi_LoadIcon(hInst ## DPI, IconName)
#define DPI_LoadImage(hInst, szName, uType, cxDesired, cyDesired, fuLoad) _dpi_LoadImage(hInst ## DPI, szName, uType, cxDesired, cyDesired, fuLoad)
#define DPI_ImageList_LoadImage(hInst, pbmp, cx, cGrow, crMask, uType, Flags) HIDPI_ImageList_LoadImage(hInst ## DPI, pbmp, cx, cGrow, crMask, uType, Flags)
#define DPI_FindResource(hModule, pName, pType) _dpi_FindResource(hModule ## DPI, pName, pType)
#define DPI_LoadResource(hModule, hResInfo) _dpi_LoadResource(hModule ## DPI, hResInfo)


// End of DPI Awareness APIs
/////////////////////////////////////////////////////////////////////////////////////////////////////

// This hasn't moved into the OS headers yet.
#ifndef DT_END_ELLIPSIS
#define DT_END_ELLIPSIS     0x00008000
#endif // ! DT_END_ELLIPSIS

//////////////////////////////////////////////////////////////////////////////
//
// Append DPI-specific subkey to specified reg key
//
//  Method appends the "\\%dDPI" to the end of a reg key.
//  For example adds "\\192DPI" if it's 192DPI.
//

HRESULT SHGetDPISpecificRegKey(LPTSTR pszKey, LPTSTR pszBaseKey, DWORD cchLen);

//
//////////////////////////////////////////////////////////////////////////////

HRESULT SHLoadHelperObject(HWND hwndDlg, LPCTSTR pszBHORegPath, __out IObjectWithSite **ppBHO);

/***************************************************************************\
* ProcessAlphaBitmap
*
* Examines the source bitmap to see if it supports and uses an alpha
* channel. If it does, a new DIB section is created that contains a
* premultiplied copy of the data from the source bitmap.
*
* If the source bitmap is not capable of supporting, or simply doesn't use,
* an alpha channel, the return value is NULL.
*
* If an error occurs, the return value is NULL.
*
* 
\***************************************************************************/
HBITMAP ProcessAlphaBitmap(HBITMAP hbmSource);

//////////////////////////////////////////////////////////////////////////////
//
// Animation Sequence API (internal)
// Used for loading startup and shutdown animation engines
// {

typedef struct
{
    TCHAR szName[MAX_PATH]; // Path to splash screen image
    DWORD dwDuration;       // Duration of animation, in milliseconds
    HBITMAP hbmImage;       // Loaded splash image
} SHANIMSEQELEMSPLASH;

typedef struct
{
    BOOL fAnimation; // Are we doing an animation or showing a splash screen?
    union
    {
        SHANIMSEQELEMSPLASH asplInfo;   // Information for displaying splash screen
        TCHAR szDll[MAX_PATH];          // Name of animation engine dll
    };
} SHANIMSEQELEMINFO;

typedef struct
{
    UINT cAnimations;               // Number of animation engines specified
    SHANIMSEQELEMINFO* paiAnimations;  // Properties of specified animations
    WCHAR wszAnimFunc[MAX_PATH];    // Name of animation function implemented by engines
    HANDLE hEventReqStop;           // Handle to event used to request animation engine to stop
    HANDLE hEventDone;              // Handle to event used by engine to notify that it is done
    HANDLE hThreadAnim;             // Handle to current animation thread
    HANDLE hThreadWait;             // Handle to current wait thread
    UINT iNextAnimation;            // Index of animation engine that will be used next
    UINT uIDTimer;                  // Timer ID used
    HWND hwndOwner;                 // Window that owns the animation sequence
} SHANIMSEQ;

typedef HRESULT (*PFN_SHANIMSEQDOANIMATION)(HWND, HANDLE, HANDLE);

HRESULT SHAnimationSequenceInit(LPCTSTR pszRegKey, LPCWSTR pwszAnimFunc, LPCTSTR pszEventReqStop, LPCTSTR pszEventDone,
                  SHANIMSEQ **ppasAnimations);
HRESULT SHAnimationSequenceFree(SHANIMSEQ *pasAnimations);
HRESULT SHAnimationSequenceLaunch(SHANIMSEQ *pasAnimations, UINT uIDTimer, HWND hwndOwner);
HRESULT SHAnimationSequenceHandleTimer(SHANIMSEQ *pasAnimations);
HRESULT SHAnimationSequenceHalt(SHANIMSEQ *pasAnimations);

BOOL SHAnimationSequenceIsAnimating(SHANIMSEQ *pasAnimations);

// }
//
// End of Animation Sequence API
//
//////////////////////////////////////////////////////////////////////////////



// **************************************************************************
// Function Name: SHLoadFormattedImageFile
//
// Purpose: Used to create an HBITMAP based on a file.
//
// Arguments:
//    IN LPCTSTR    pszFileName  - The name of the file.
//    IN BITMAPINFO* pDesiredBmi - If pDesiredBmi is not NULL, then:
//
//   - The compression type will be pDesiredBmi->bmiHeader.biCompression.
//   - The bit count per pixel will be pDesiredBmi->bmiHeader.biBitCount.
//
//   IN COLORREF crTransparentOverride - the transparent color to use.
//
//   If crTransparentOverride == CLR_INVALID, then White will be used as the
//   background color.
//
// Return Values:
//    HBITMAP
//    Handle to a DIB Section.
HBITMAP SHLoadFormattedImageFile(LPCTSTR pszFileName, BITMAPINFO* pDesiredBmi, COLORREF crTransparentOverride);


// **************************************************************************
// Private message for Menu Bar.
//
#define SHCMBM_ISKEYOVERRIDDEN  (WM_USER + 407) // wParam == VirtKey; return TRUE/FALSE.

// **************************************************************************
// Function Name: SHCheckWirelessManagerEnabled
//
// Purpose: Check if the Wireless Manager feature is enabled.
//
// Return Values:
//    BOOL, true if the feature is enabled
BOOL SHCheckWirelessManagerEnabled();

// **************************************************************************
// Function Name: SHLaunchWirelessManager
//
// Purpose: Launch the Wireless Manager application.
//
// Return Values:
//    HRESULT
HRESULT SHLaunchWirelessManager();

// **************************************************************************
// Function Name: SHDismissNotificationIcon
// 
// Purpose: Removes the missed call notification icon, new SMS notification flag,
//          or new E-mail notification flag
//
// Return Values:
//    HRESULT
typedef enum _SDNI
{
  SDNI_MISSEDCALL,
  SDNI_NEWSMS,
  SDNI_NEWEMAIL,
} SDNI;

HRESULT SHDismissNotificationIcon(SDNI sdni);

// **************************************************************************
// Function Name: PHGetCurrentCarrierData
//
// Purpose: Writes current network operator name or string representing
//          current network registration status to pszOut.
//
// Return Values:
//    HRESULT
HRESULT PHGetCurrentCarrierData(DWORD dwFlags, TCHAR* pszOut, UINT cchOut);


// **************************************************************************
// Function Name: PHGetCurrentVoIPServiceData
//
// Purpose: Writes current voip operator name or string representing
//          current sip server registration status to pszOut.
//
// Return Values:
//    HRESULT
HRESULT PHGetCurrentVoIPServiceData(DWORD dwFlags, TCHAR* pszOut, UINT cchOut);

#define RETRYECS_DEFAULT_RETRIES 1

/// <summary>
///     Try to enter critical section, yielding instead of entering wait queue for the
///     specified number of times.
/// </summary>
/// <param name="lpCriticalSection">
///     Address of critical section to enter
/// </param>
/// <param name="nRetries">
///     Number of retries
/// </param>
/// <remarks>
///     When this function returns, caller will be the owner of the critical section.
///     In fact, when <paramref name="nRetries"/> is zero behavior will be equivalent to
///     calling EnterCriticalSection directly.
///
///     Yielding control a few times when failing to acquire critical section is desirable
///     in order to avoid lock convoys, where two or more threads each only get one entry
///     into critical section before having to block waiting for one of the other threads
///     that newly acquired said critical section when the first thread left it. Yielding
///     in this way ensures that each thread will be able to get at least a full scheduler
///     quantum entering, leaving and re-entering the same critical section several times
///     before another thread takes it over, therefore improving performance.
/// </remarks>
__inline void RetryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection, UINT nRetries)
{
    while(!TryEnterCriticalSection(lpCriticalSection))
    {
        if( nRetries-- > 0)
        {
            Sleep(0);
            continue;
        }

        EnterCriticalSection(lpCriticalSection);
        break;
    }
}

#define DefaultRetryEnterCriticalSection(pcs) RetryEnterCriticalSection(pcs, RETRYECS_DEFAULT_RETRIES)

/// <summary>
///   This function is called by the keyboard driver to set the
///   input mode icon it desired with the given input mode.
/// </summary>
/// <param name="nMode">
///   <para>
///     Current keyboard driver mode. It can be one of the following.
///   </para>
///   <para>
///     KBDI_SHIME_MODE_NONE                 Input mode is None.
///     KBDI_SHIME_MODE_SPELL                Input mode is Spell.
///     KBDI_SHIME_MODE_SPELL_CAPS           Input mode is Spell Caps.
///     KBDI_SHIME_MODE_SPELL_CAPS_LOCK      Input mode is Spell Caps Locks.
///     KBDI_SHIME_MODE_AMBIGUOUS            Input mode is Ambiguous.
///     KBDI_SHIME_MODE_AMBIGUOUS_CAPS       Input mode is Ambiguous Caps.
///     KBDI_SHIME_MODE_AMBIGUOUS_CAPS_LOCK  Input mode is Ambiguous Caps Locks.
///     KBDI_SHIME_MODE_NUMBERS              Input mode is numbers.
///     KBDI_SHIME_MODE_CUSTOM               Input mode is custom.
///   </para>
/// </param>
/// <returns>
///     S_OK means succeeded, otherwise means failure.
/// </returns>
/// <remarks>
///   <para>
///        This API is for OEM only
///   </para>
///   <para>
///     The Keyboard driver must call this API every time the user manually changes
///     the keyboard driver into a new mode in order to set the input mode icon.
///     This API will detect if IME is active or not. If IME is not active for the
///     window with input focus this API will set the input mode icon match the current
///     keyboard mode
///   </para>
/// </remarks>
HRESULT SHSetInputModeIconFromKeyboard(SHIME_MODE nMode);

BOOL SHCheckTaskManagerPresent();

HRESULT SHLaunchTaskManager();

#ifdef __cplusplus
}
#endif



#endif // __WPCPRIV_H__

