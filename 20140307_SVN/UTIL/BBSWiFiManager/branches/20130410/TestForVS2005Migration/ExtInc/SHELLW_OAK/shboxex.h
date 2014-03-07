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
#ifndef __SHBOXEX_H__
#define __SHBOXEX_H__

#ifdef __cplusplus
extern "C" {
#endif

// Type of box
typedef enum _SHBX_TYPE
{
    sbextTextBox = 0,                   // Dialog with text and one or two button
    sbextClassicMessageBox,             // Classic Message Box.
    sbextCallAlertBox,
    sbextCustomBox,                      // Custom box. Caller must provide template and hook with full implementation.
                                         // DWL_USER is used by the implementation. An application can use SH_BOX.pvUser instead.
    sbextBubbleBox,
    sbextToast,                          // Toast pops up from the bottom of the work area
} SHBEX_TYPE;

// Type of Images that can be displayed in a box
// The order of these are related to image resource IDs and MB_ICON* flags.
typedef enum _SHBEX_IMAGE
{
    sbexiFirst = 0,

    sbexiNone = sbexiFirst,             // No image
    sbexiStop,                          // Stop image
    sbexiQuestion,                      // Question image
    sbexiExclamation,                   // Exclamation image
    sbexiInformation,                   // Information image

    sbexiLast = sbexiInformation
    
} SHBEX_IMAGE;


typedef struct _SHBEX_BUTTON_INFO
{
    short cButtons;                     // Number of buttons. Must be 1 or 2 if buttons are specified, 0 for HMENU 
    union
    {
        struct 
        {
            LPCTSTR pszButton1;                 // Optional: Text of Button1. Default is "OK"
            LPCTSTR pszButton2;                 // Optional: Text of Button2. Default is "Cancel"
            unsigned short uIDButton1;          // Optional: ID of Button1. Default is IDOK.
            unsigned short uIDButton2;          // Optional: ID of Button2. Default is IDCANCEL
            unsigned short uIDDefault;          // Optional: Default button for autoclose. Default is nIDButton2.
        };
        HMENU   hMenu;                      // Toasts can optionally have an HMENU instead of using buttons.
    };
} SHBEX_BUTTON_INFO;

// Textbox info. Must set unused fields to 0.
typedef struct _SHBEX_TEXTBOX_INFO
{
    LPCTSTR pszHeadingText;             // Heading Text
    LPCTSTR pszText;                    // Text
    SHBEX_IMAGE sbeximage;              // Optional: Image
    SHBEX_BUTTON_INFO bi;               // Button info.
} SHBEX_TEXTBOX_INFO;

// Classic Message Box info. Must set unused fields to 0.
typedef struct _SHBEX_CLASSIC_MSGBOX_INFO
{
    LPCTSTR pszText;                    // Text
    UINT uType;                         // MB_* type
    unsigned short uIDDefault;          // Optional: Default button for autoclose. Required with SHBEXF_AUTOCLOSE.
} SHBEX_CLASSIC_MSGBOX_INFO;

// Classic Message Box info. Must set unused fields to 0.
typedef struct _SHBEX_CALL_ALERT_INFO
{
    SHBEX_BUTTON_INFO bi;               // Button info.
} SHBEX_CALL_ALERT_INFO;

// Classic Message Box info. Must set unused fields to 0.
typedef struct _SHBEX_BUBBLE_INFO
{
    SHBEX_BUTTON_INFO bi;               // Button info.
    int iXPos;                          // position of the cartoon bubble
    int iYPos;
    int cxPos;                          // -1 for auto-calc
    int cyPos;                          // -1 for auto-calc
    POINT pt;                           // the place the bubble should stem from
    DWORD dwOriginatingIcon;            // Icon the bubble stems from.  If specified, this info is used
                                        // instead of pt.
    DWORD dwAlign;                      // one of SHBOXALIGN specifying which side the bubble triange sticks out of
    DWORD dwFlags;                      // SHBBIF_*
    LPCTSTR pszContent;
    HWND hwndContainer;                 // out: the container hwnd
    COLORREF clrLeft;                   // For gradient fill when you have a title
    COLORREF clrRight;                  // For gradient fill when you have a title
} SHBEX_BUBBLE_INFO;
#define SHBBIF_URL          0x000001
#define SHBBIF_HTML         0x000002
#define SHBBIF_CRITICAL     0x000004
#define SHBBIF_NOSCRIPT     0x000008
#define SHBBIF_DIALOG       0x000010
#define SHBBIF_TITLETIME    0x000020
#define SHBBIF_SPINNERS     0x000040
#define SHBBIF_WANTVKTTALK  0x000080

#define SHBB_DEFAULT_BUBBLEWIDTH (-2)
#define SHBB_DEFAULT_BUBBLEPOS   (-1)  // pass in to either iYPos and/or iXPos to auto calc the position of the bubble.

typedef enum SHBOX_ALIGN
{
    SHBOX_LEFTALIGN,
    SHBOX_TOPALIGN,
    SHBOX_RIGHTALIGN,
    SHBOX_BOTTOMALIGN,
} SHBOXALIGN;

// SH_BOXEX Styles
#define SHBEXF_TIMED                    0x00000001          // Time out
#define SHBEXF_TOPMOST                  0x00000002          // Above all app windows
#define SHBEXF_ABOVE_STARTUP            0x00000004          // Above all app windows and the startup screen
#define SHBEXF_SETFOREGROUND            0x00000008          // Call SetForegroundWindow on the box
#define SHBEXF_NOAUTOCLOSE_ON_RELAUNCH  0x00000010          // Don't autoclose on relaunch
#define SHBEXF_POPUP                    0x00000020          // Popup box. Default is full screen.
#define SHBEXF_MODELESS                 0x00000100          // Modeless. Not supported for sbextClassicMessageBox. Must provide hook to process WM_COMMAND.
#define SHBEXF_CLOSE_ON_ACTIVATION_LOSS 0x00000200          // Close when activation is lost
#define SHBEXF_CLOSE_LIKE_MENUS         0x00000400          // Close when clicked outside of bubble window.  Only valid for bubble types.
#define SHBEXF_SUPRESS_DEFAULT_SOUND    0x00000800          // supresses any default sounds
#define SHBEXF_WRAP_HEADER_TEXT         0x00001000          // Allow DT_WORDBREAK for the header text
#define SHBEXF_SMALL_HEADER_FONT        0x00002000          // Uses a smaller font for the header text

// Hook Proc type.
// Return TRUE to stop further processing. Return FALSE otherwise.
typedef BOOL (CALLBACK* SHB_HOOK_PROC)(
    HWND,       // In: Dialog window handle
    UINT,       // In: Message ID
    WPARAM,     // In: Message wParam
    LPARAM,     // In: Message lParam
    LPVOID      // In: User data (dwUser)
);

// Shell BoxEx info. Must set unused fields to 0.
typedef struct _SH_BOXEX
{
    DWORD cbSize;                           // Struct size
    HWND hwndOwner;                         // Owner
    DWORD dwStyle;                          // SHBEXF_* styles
    UINT nTimeOut;                          // Timeout in ms if style includes SHBEXF_TIMED. Set to 0 for default timeout.
    LPCTSTR pszTitle;                       // Title.
    int nZOrderPri;                         // Z Order Priority

    HINSTANCE hinst;                        // Optional HINSTANCE containing custom template
    LPCTSTR pszTemplate;                    // Optional dialog template resource name. hinst is also required to use this
    LPCTSTR pszBannerKey;                   // Optional banner bitmap loaded from Banner key of "BannerImage" (TPC Textbox Only)

    int idrMenu;                            // Optional menu bar resource id override. Must be of type RCDATA. hinst is also required to use this
                                            // even with this the caller is still responsible for setting default button info
                                            
    SHB_HOOK_PROC pfnHook;                  // Optional Hookproc. Reqd. for modeless dialogs to handle WM_COMMAND.
    LPVOID pvUser;                          // User data to be passed back in hookproc
    LPARAM lReserved;                       // Reserved. Must be 0.
    
    int nIDSelected;                        // Out: ID of button selected by user. Returned only for modal boxes.
    HWND hwnd;                              // Out: HWND of box. Returned only for modeless boxes.

    SHBEX_TYPE sbextype;                    // Type
    union                                   // Info specific to each type
    {
        SHBEX_TEXTBOX_INFO ti;              // Text box with one or two buttons
        SHBEX_CLASSIC_MSGBOX_INFO cmi;      // Classic Message Box info
        SHBEX_CALL_ALERT_INFO cai;          // Call Alert info
        SHBEX_BUBBLE_INFO bi;
    } info;

} SH_BOXEX, *PSH_BOXEX;

// Returns S_OK on success.
HRESULT SHBoxEx(
    SH_BOXEX* psbex       // In: Box info
);

#ifdef __cplusplus
}
#endif

#endif //__SHBOXEX_H__
