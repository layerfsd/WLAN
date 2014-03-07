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
Copyright (c) 1999-2000  Microsoft Corporation

-----------------------------------------------------------------------------

@doc EXTERNAL

@module simtkui.h - SimToolKit UI Layer |

The SIM Toolkit UI layer exposes an API that can be used to design the UI component of SimToolKit apps.


-------------------------------------------------------------------------------*/

#ifndef _SIMTKUI_H_
#define _SIMTKUI_H_

#include <windows.h>

// {BD224CD9-B318-4399-95DB-436BC1503A31}
DEFINE_GUID(IID_ISimToolkitUI, 0xbd224cd9, 0xb318, 0x4399, 0x95, 0xdb, 0x43, 0x6b, 0xc1, 0x50, 0x3a, 0x31);

// {3A02EAC7-5E82-4de4-A746-5D61088A366A}
DEFINE_GUID(IID_ISimToolkitUIv2, 0x3a02eac7, 0x5e82, 0x4de4, 0xa7, 0x46, 0x5d, 0x61, 0x8, 0x8a, 0x36, 0x6a);

// {9bdd7d11-de94-46ab-aa5d-27645ddcb9e7}
DEFINE_GUID(IID_ISimImage, 0x9bdd7d11, 0xde94, 0x46ab, 0xaa, 0x5d, 0x27, 0x64, 0x5d, 0xdc, 0xb9, 0xe7);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Return Results | Successful return results
//
// @comm Successful Result types
//
// ----------------------------------------------------------------------------- 
#define FACILITY_TOOLKITUI      (0x100)                                                     // @constdefine ToolkitUI Facility Code for HRESULT #define S_TOOLKIT_OK            (MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_TOOLKITUI, 0x01))  // @constdefine Command completed successfully
#define S_TOOLKIT_OK            (MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_TOOLKITUI, 0x01))  // @constdefine Command completed successfully
#define S_TOOLKIT_BACK          (MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_TOOLKITUI, 0x02))  // @constdefine User indicated a desire to move back in the toolkit session
#define S_TOOLKIT_HELP          (MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_TOOLKITUI, 0x03))  // @constdefine User asked for help
#define S_TOOLKIT_END           (MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_TOOLKITUI, 0x04))  // @constdefine User indicated an end to the toolkit session

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Toolkit Notify | Notifications (These Corresponds with the next Action Indicator)
//
// @comm Notification codes for toolkit functions -- these match 13.4 in GSM 11.14
//
// -----------------------------------------------------------------------------

#define TOOLKITUI_NEXTCMD_REFRESH       (0x01)       // @constdefine Refresh
#define TOOLKITUI_NEXTCMD_MORETIME      (0x02)       // @constdefine More Time
#define TOOLKITUI_NEXTCMD_POLLINTERVAL  (0x03)       // @constdefine Change poll interval
#define TOOLKITUI_NEXTCMD_POLLINGOFF    (0x04)       // @constdefine Turn polling off
#define TOOLKITUI_NEXTCMD_SETUPCALL     (0x10)       // @constdefine Setup Call
#define TOOLKITUI_NEXTCMD_SENDSS        (0x11)       // @constdefine Send USSD
#define TOOLKITUI_NEXTCMD_SENDSMS       (0x13)       // @constdefine Send SMS
#define TOOLKITUI_NEXTCMD_PLAYTONE      (0x20)       // @constdefine Play tone
#define TOOLKITUI_NEXTCMD_DISPLAYTEXT   (0x21)       // @constdefine Show text
#define TOOLKITUI_NEXTCMD_GETINKEY      (0x22)       // @constdefine Get key input
#define TOOLKITUI_NEXTCMD_GETINPUT      (0x23)       // @constdefine Get text input
#define TOOLKITUI_NEXTCMD_SELECTITEM    (0x24)       // @constdefine Select Item
#define TOOLKITUI_NEXTCMD_SETUPMENU     (0x25)       // @constdefine Setup Menu
#define TOOLKITUI_NEXTCMD_LOCALINFO     (0x26)       // @constdefine Local Info


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIM Toolkit flags | Various flags passed to UI commands
//
// @comm A subset of these flags are applicable to the individual call that uses them
//
// -----------------------------------------------------------------------------
#define TOOLKITUI_HIGHPRIORITY          (0x00000001)    // @constdefine High priority message
#define TOOLKITUI_DELAYCLEAR            (0x00000002)    // @constdefine The message should disappear on its own
#define TOOLKITUI_KEYPADDIGITS          (0x00000004)    // @constdefine User input should only consist of keypad digits
#define TOOLKITUI_NOECHO                (0x00000008)    // @constdefine The SIM UI should not echo entered characters
#define TOOLKITUI_HELPINFO              (0x00000010)    // @constdefine Context sensitive help is supported
#define TOOLKITUI_GSMDEFAULTALPHABET    (0x00000020)    // @constdefine User input should only consist of characters in the GSM default alphabet
#define TOOLKITUI_SETUPMENU             (0x00000040)    // @constdefine This is a SETUP MENU command
#define TOOLKITUI_YESNO                 (0x00000100)    // @constdefine Get Inkey yes/no response requested

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants TOOLKITTONE | Various tones the UI can be asked to play
//
// @comm None
//
// -----------------------------------------------------------------------------
#define TOOLKITTONE_DIAL                (0)             // @constdefine Play an OK tone
#define TOOLKITTONE_BUSY                (1)             // @constdefine Called party is busy
#define TOOLKITTONE_CONGESTION          (2)             // @constdefine Congestion
#define TOOLKITTONE_SERVICE             (3)             // @constdefine Radio service available
#define TOOLKITTONE_NOSERVICE           (4)             // @constdefine No radio service available
#define TOOLKITTONE_SPECIAL             (5)             // @constdefine Special information
#define TOOLKITTONE_CALLWAIT            (6)             // @constdefine Call waiting tone
#define TOOLKITTONE_RING                (7)             // @constdefine Ring tone
#define TOOLKITTONE_DEFAULT             (8)             // @constdefine Play the default tone
#define TOOLKITTONE_SUCCESS             (9)             // @constdefine Play an "ok" tone
#define TOOLKITTONE_ERROR               (10)            // @constdefine Play an "error" tone
#define TOOLKITTONE_DEFAULTDURATION     (0xffffffff)    // @constdefine Default duration

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Unavailable | Image encoding flags
// (refer to GSM11.11 v.8.3.0 1999, pages 111, 150-152 for details)
//
// -----------------------------------------------------------------------------
#define SIMIMAGE_INSTANCE_SCHEME_BASIC              (0x11)
#define SIMIMAGE_INSTANCE_SCHEME_COLOUR             (0x21)


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct TOOLKITMENUITEM | An individual menu item
//
// @comm None
//
// -----------------------------------------------------------------------------

struct ISimImage;

typedef struct toolkitmenu_item {
    DWORD dwIdentifier;                             //@field Identify the specific item in the menu
    DWORD dwNextAction;                             //@field What would happen if this was selected
    TCHAR *pwszText;                                //@field Text associated w/menu item
    ISimImage *pImage;                              //@field Image icon associated w/ item
    DWORD dwIconQualifier;                          //@field Qualifier designating how the icon should be used.
} TOOLKITMENUITEM, FAR *LPTOOLKITMENUITEM;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct TOOLKITMENU | A menu of items
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct toolkitmenu_tag {
    DWORD dwFlags;                                  //@field TOOLKITUI_* Constant
    DWORD dwDefaultItem;                            //@field Default Item in the menu
    DWORD dwMenuItemCount;                          //@field Number of TOOLKITMENU items
    TCHAR *pwszText;                                //@field Text associate w/menu
    TOOLKITMENUITEM *rgtmi;                         //@field Toolkitmenu items
} TOOLKITMENU, FAR *LPTOOLKITMENU;

struct ImageInstanceInfo_t 
{
    BOOL  IsColor;
    DWORD Width;
    DWORD Height;
};

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @enum SUIACTIVATION | Various types of UI activation
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef enum simuiactivation_tag {
    SUIA_USER = 0,
    SUIA_PROACTIVE_FOREGROUND,
    SUIA_PROACTIVE_BACKGROUND
} SUIACTIVATION, FAR *LPSUIACTIVATION;

class ISimToolkitPolicy
{
public:
// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called to tell the toolkit policy layer to end the session 
//       immediately. 
//
// @comm If you are in the middle of processing a function called from the policy layer,
//       you should instead return S_TOOLKIT_END from that function which has the same
//       effect as calling this API.
//
// -----------------------------------------------------------------------------
    virtual void EndSession() = 0;
};


/* #############################################################################
   ### ISimImage
   ###   SIM image object interface  
*/
DECLARE_INTERFACE_(ISimImage, IUnknown)
{
    //
    // Count of instances associated with the image
    //
    STDMETHOD_(DWORD, GetInstanceCount) (void) = 0;

    //
    // Information on a particular image instance
    //
    STDMETHOD (GetInstanceInfo) (DWORD dwInstance, ImageInstanceInfo_t* pInfo) = 0;

    //
    // Create a bitmap for a particular image instance
    //
    STDMETHOD (CreateInstanceBitmap) (DWORD dwInstance, HDC hdc, DWORD PixelsX, DWORD PixelsY, HBITMAP* pBitmap) = 0;
};


/* #############################################################################
   ### ISimToolKitUI
   ###   COM interface for the SimToolkit UI object. Called from SimPolcy.cpp to
   ###   provide a view component for the SIM application
*/

class ISimToolkitUI : public IUnknown
{
public:
// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called to tell the toolkit UI component to load its UI.
//        suia is SUIA_USER if the user has launched SIM Toolkit and
//        either SUIA_PROACTIVE_FOREGROUND or SUIA_PROACTIVE_BACKGROUND
//        if UI is being loaded because of a proactive toolkit command.
//        FOREGROUND/BACKGROUND specifies whether the command requires
//        that the UI be loaded as the foreground window.
//
// @comm You should return the HWND of whatever window you create, or NULL to 
//       indicate an error
//
// -----------------------------------------------------------------------------
    virtual HWND LoadUI(
        ISimToolkitPolicy *pSimToolkitPolicy,       //@parm Pointer to SimToolkitPolicy 
        SUIACTIVATION suia                          //@parm Indicates who activated Simtoolkit UI and how
        ) = 0;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called to tell the toolkit UI component to unload its UI
//
// @comm None
//
// -----------------------------------------------------------------------------
    virtual void UnloadUI() = 0;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func If the user takes too long to respond to a toolkit command, this function
//       will be called.
//
// @comm You should remove whatever UI you have displayed when this function is called
//
// -----------------------------------------------------------------------------
    virtual void CommandTimedOut() = 0;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called to let the UI know that it should remain up regardless of
//       whether it loses activation or not.
//
// @comm If this function is called with fKeepUI = TRUE, you are guaranteed to be called
//       again with fKeepUI = FALSE, and should not call EndSession between these points
//
// -----------------------------------------------------------------------------
    virtual void KeepUI(BOOL fKeepUI) = 0;
    
// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called if you have to display text to the screen
//
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code
//
// -----------------------------------------------------------------------------
    virtual HRESULT DisplayText(
        TCHAR *pwszText,                            //@parm Text to be Displayed
        DWORD dwFlags                               //@parm TOOLKITUI_* Constant
        ) = 0;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called to prompt the user to enter a single character
//
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code.
//       You should save the entered character into pchResult
//
// -----------------------------------------------------------------------------
    virtual HRESULT GetInkey(
        TCHAR *pwszText,                            //@parm Text associated w/input prompt
        TCHAR *pchResult,                           //@parm User entered text
        DWORD dwFlags                               //@parm TOOLKITUI_* Constant
        ) = 0;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called to prompt the user to enter a String
//
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code.
//       The entered string should be copied to pwszResult, which will be large enough
//       to accodomate a string up to the length specified by dwMaxResponse
//
// -----------------------------------------------------------------------------
    virtual HRESULT GetInput(
        TCHAR *pwszText,                            //@parm Text associate w/input prompt
        TCHAR *pwszDefaultText,                     //@parm Default text in user input field
        TCHAR *pwszResult,                          //@parm User Entered String
        DWORD dwMinResponse,                        //@parm Minimum pwszResult Length
        DWORD dwMaxResponse,                        //@parm Maximum pwszResult Length
        DWORD dwFlags                               //@parm TOOLKITUI_* Constant
        ) = 0;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called if you have to display UI before making a call
//
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code.
//       You should set pfOKToCall to TRUE if it is OK to place the call, and FALSE if not
//
// -----------------------------------------------------------------------------
    virtual HRESULT DisplayCallUI(
        TCHAR *pwszText,                            //@parm Text
        TCHAR *pwszAddress,                         //@parm Destination to Dial
        BOOL *pfOKToCall                            //@parm Boolean User Confirmation Message
        ) = 0;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called to display a menu
//
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code
//
// -----------------------------------------------------------------------------
    virtual HRESULT DisplayMenu(
        TOOLKITMENU *pMenu,                         //@parm Pointer to a Menu Object
        DWORD *pdwSelected                          //@parm Selected Item in the Menue
        ) = 0;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called if you have to play a tone
//
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code
//
// -----------------------------------------------------------------------------
    virtual HRESULT PlayTone(
        DWORD dwTone,                               //@parm value from TOOLKITTONE
        DWORD dwDuration,                           //@parm Duration of the tone in milliseconds
        TCHAR *pwszText                             //@parm Text associated with the tone
        ) = 0;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function is called if you have to display idle mode text
//
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code
//
// -----------------------------------------------------------------------------
    virtual HRESULT DisplayIdleModeText(
        TCHAR *pwszText                             //@parm Text
        ) = 0;
};


/* #############################################################################
   ### ISimToolKitUIV2
   ###   COM interface with the calls that implement the
   ###   new GSM99 SIM standard and icon support
   ###   Called from SimPolcy.cpp to provide a view component for the SIM app
*/

class ISimToolkitUIv2 : public IUnknown {

public:
    
// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called if you have to display UI
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code
// -----------------------------------------------------------------------------
    virtual HRESULT DisplayText(
        TCHAR *pwszText,                            //@parm Text to be Displayed
        DWORD dwFlags,                              //@parm TOOLKITUI_* Constant
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not
        ) = 0;

// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called to prompt the user to enter a single character
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code.
//       You should save the entered character into pchResult
// -----------------------------------------------------------------------------
    virtual HRESULT GetInkey(
        TCHAR *pwszText,                            //@parm Text associated w/input prompt
        TCHAR *pchResult,                           //@parm User entered text
        DWORD dwFlags,                              //@parm TOOLKITUI_* Constant
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not
        ) = 0;

// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called to prompt the user to enter a String
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code.
//       The entered string should be copied to pwszResult, which will be large enough
//       to accomodate a string up to the length specified by dwMaxResponse
// -----------------------------------------------------------------------------
    virtual HRESULT GetInput(
        TCHAR *pwszText,                            //@parm Text associate w/input prompt
        TCHAR *pwszDefaultText,                     //@parm Default text in user input field
        TCHAR *pwszResult,                          //@parm User Entered String
        DWORD dwMinResponse,                        //@parm Minimum pwszResult Length
        DWORD dwMaxResponse,                        //@parm Maximum pwszResult Length
        DWORD dwFlags,                              //@parm TOOLKITUI_* Constant
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not
        ) = 0;

// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called if you have to display UI before making a call
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code.
//       You should set pfOKToCall to TRUE if it is OK to place the call, and FALSE if not
// -----------------------------------------------------------------------------
    virtual HRESULT DisplayCallUI(
        TCHAR *pwszText,                            //@parm Text
        TCHAR *pwszAddress,                         //@parm Destination to Dial
        BOOL *pfOKToCall,                           //@parm Boolean User Confirmation Message
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not        
        ) = 0;

// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called to display a menu
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code
// -----------------------------------------------------------------------------
    virtual HRESULT DisplayMenu(
        TOOLKITMENU *pMenu,                         //@parm Pointer to a Menu Object
        DWORD *pdwSelected,                         //@parm Selected Item in the Menu
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not        
        ) = 0;

// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called if you have to play a tone
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code
// -----------------------------------------------------------------------------
    virtual HRESULT PlayTone(
        DWORD dwTone,                               //@parm value from TOOLKITTONE
        DWORD dwDuration,                           //@parm Duration of the tone in milliseconds
        TCHAR *pwszText,                            //@parm Text associated with the tone
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not        
        ) = 0;

// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called if you have to display idle mode text
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code
// -----------------------------------------------------------------------------
    virtual HRESULT DisplayIdleModeText(
        TCHAR *pwszText,                            //@parm Text
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not        
        ) = 0;

// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called when app request launching a browser
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code.
//       You should set pfOKToLaunch to TRUE if it is OK to launch browser, and FALSE if not
// -----------------------------------------------------------------------------
    virtual HRESULT LaunchBrowser(
        TCHAR *pwszText,                            //@parm Text
        TCHAR *pwszAddress,                         //@parm Destination to Dial
        BOOL *pfOKToLAunch,                         //@parm Boolean User Confirmation Message
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not        
        ) = 0;
// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called when app request sending dial tones
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code.
//       You should set pfOKToSend to TRUE if it is OK to send tones, and FALSE if not
// -----------------------------------------------------------------------------
    virtual HRESULT SendDTMF(
        TCHAR *pwszText,                            //@parm Text to show
        TCHAR *pwszDTMF,                            //@parm (unused) tone info
        BOOL *pfOKToSend,                           //@parm Boolean that receives user confirm
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not        
        ) = 0;

// -----------------------------------------------------------------------------
// @doc EXTERNAL
// @func This function is called when app request open data channel
// @comm The return value should be one of the S_TOOLKITUI_ constants, or an error code.
//       You should set pfOKToOpen to TRUE if it is OK to open channel, and FALSE if not
// -----------------------------------------------------------------------------
    virtual HRESULT OpenChannel(
        TCHAR *pwszText,                            //@parm Text to show
        BOOL *pfOKToOpen,                           //@parm Boolean that receives user confirm
        ISimImage* pImage,                          //@parm Image to be displayed or NULL if not applicable
        DWORD dwIconQualifier                       //@parm As specified in GSM standard, selects whether text should accompany the icon or not        
        ) = 0;

};


#endif // _SIMTKUI_H_

