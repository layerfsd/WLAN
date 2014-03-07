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
#ifndef __SHBOX_H__
#define __SHBOX_H__

#include <shboxex.h>

#ifdef __cplusplus
extern "C" {
#endif

// Dialog with 1 buttons info. Must set unused fields to 0.
typedef struct _SHB_1BUTTON_INFO
{
    LPCTSTR pszButton;                   // Optional: Text of Button.  Default depends on type.
    unsigned short uIDButton;            // Optional: ID of Button. Default is IDOK.
} SHB_1BUTTON_INFO;

// Dialog with 2 buttons info. Must set unused fields to 0.
typedef struct _SHB_2BUTTON_INFO
{
    LPCTSTR pszButton1;                 // Optional: Text of Button1. Default depends on type.
    LPCTSTR pszButton2;                 // Optional: Text of Button2. Default depends on type.
    unsigned short uIDButton1;          // Optional: ID of Button1. Default is IDOK.
    unsigned short uIDButton2;          // Optional: ID of Button2. Default is IDCANCEL
    unsigned short uIDDefault;          // Optional: Button to be selected. Default is nIDButton2.
} SHB_2BUTTON_INFO;

// Text dialog with 1 buttons info. Must set unused fields to 0.
typedef struct _SHB_TEXTBOX_1BUTTON_INFO
{
    LPCTSTR pszText;                     // Text
    SHB_1BUTTON_INFO b1i;   
} SHB_TEXTBOX_1BUTTON_INFO;

// Text dialog with 2 buttons info. Must set unused fields to 0.
typedef struct _SHB_TEXTBOX_2BUTTON_INFO
{
    LPCTSTR pszText;                     // Text
    SHB_2BUTTON_INFO b2i;    
} SHB_TEXTBOX_2BUTTON_INFO;

typedef SHB_TEXTBOX_1BUTTON_INFO             SHB_INFORMATION_INFO;
typedef SHB_TEXTBOX_1BUTTON_INFO             SHB_EXCLAMATION_INFO;
typedef SHB_TEXTBOX_2BUTTON_INFO             SHB_CLASS0_SMS_MESSAGE_INFO;
typedef SHB_TEXTBOX_2BUTTON_INFO             SHB_ERROR_INFO;
typedef SHB_TEXTBOX_2BUTTON_INFO             SHB_QUESTION_INFO;
typedef SHB_TEXTBOX_2BUTTON_INFO             SHB_SMS_MESSAGE_INFO;

// Types
typedef enum _SHB_TYPE
{
    sbtFirst = 0,
    sbtInformation = sbtFirst,          // Information
    sbtExclamation,                     // Exclamation
    sbtError,                           // Error
    sbtQuestion,                        // Question
    sbtSMSMessage,                      // SMS Message
    sbtClass0SMSMessage,                // Class0 SMS Message
    sbtCallAlert,                       // Incoming call
    sbtPassword,                        // Password. Must provide template & hook with full implementation.
                                            // DWL_USER is used by the implementation. An application can use SH_BOX.pvUser instead.
    sbtSIMPassword,                     // SIM Password. Must provide template & hook with full implementation.
                                            // DWL_USER is used by the implementation. An application can use SH_BOX.pvUser instead.
    sbtReminder,                        // Reminder. Must provide template & hook with full implementation.
                                            // DWL_USER is used by the implementation. An application can use SH_BOX.pvUser instead.
    sbtLast = sbtReminder
} SHB_TYPE;

// SH_BOX Styles
#define SHBF_TIMED                      SHBEXF_TIMED          // Time out
#define SHBF_ABOVE_STARTUP              SHBEXF_ABOVE_STARTUP  // Above all app windows and the startup screen
#define SHBF_MODELESS                   SHBEXF_MODELESS       // Modeless. Must provide hook to process WM_COMMAND.
#define SHBF_CLOSE_ON_ACTIVATION_LOSS   SHBEXF_CLOSE_ON_ACTIVATION_LOSS // Close when box loses activation
#define SHBF_SETFOREGROUND              SHBEXF_SETFOREGROUND           // Set window to foreground
#define SHBF_TOPMOST                    SHBEXF_TOPMOST
                 
// Shell Box info. Must pass 0 in unused fields.
typedef struct _SH_BOX
{
    DWORD cbSize;                           // Struct size
    HWND hwndOwner;                         // Owner
    DWORD dwStyle;                          // SHBF_* styles
    LPCTSTR pszTitle;                       // Title.

    HINSTANCE hinst;                        // Optional HINSTANCE containing custom template. Reqd. for Password & Reminder.
    LPCTSTR pszTemplate;                    // Optional dialog template resource name. Reqd. for Password & Reminder.
    SHB_HOOK_PROC pfnHook;                  // Optional Optional Hookproc. Reqd. for Password & Reminder. Reqd. for modeless dialogs to handle WM_COMMAND.
    LPVOID pvUser;                          // User data to be passed back in hookproc. Also can be retieved using SHBM_GETUSERDATA

    int nIDSelected;                        // Out: ID of button selected by user. Returned only for modal boxes.
    HWND hwnd;                              // Out: HWND of box. Returned only for modeless boxes.

    SHB_TYPE sbtype;                        // Type
    union                                   // Info specific to each type
    {
        SHB_INFORMATION_INFO ii;            // Information info
        SHB_ERROR_INFO ei;                  // Error info
        SHB_QUESTION_INFO qi;               // Question info
        SHB_EXCLAMATION_INFO xi;            // Exclamation info
        SHB_SMS_MESSAGE_INFO si;            // SMS Message info
        SHB_CLASS0_SMS_MESSAGE_INFO csi;    // Class0 SMS Message info
    } info;

} SH_BOX, *PSH_BOX;

// Creates various types of dialog boxes. See SH_BOX for details.
// Returns S_OK on success
HRESULT SHBox(
    SH_BOX* psb       // In: Box info
);

// Create a modal text box with one or two buttons based on type. Returns -1 on failure.
// Returns IDOK (first button ID) or IDCANCEL (second button ID) on failure.
int SHTextBox(
    HWND hwndOwner,     // In: Owner window
    SHB_TYPE sbtype,    // In: Supported types are  sbtInformation, sbtExclamation, sbtError, sbtQuestion, sbtSMSMessage and sbtClass0SMSMessage
    LPCTSTR pszTitle,   // In: Title
    LPCTSTR pszText,    // In: Text
    DWORD dwStyle       // In: SHBF_* styles. SHBF_MODELESS is invalid.
);

// SHBox messages
#define SHBM_SHOW        (WM_APP + 65)      // Show/Hide dialog box. wParam == TRUE to Show. wParam == FALSE to Hide. lParam=TRUE to set foreground.
#define SHBM_GETUSERDATA (WM_APP + 66)      // Get pvUser passed in SH_BOX.
#define SHBM_SETUSERDATA (WM_APP + 67)      // Get pvUser passed in SH_BOX.
#define SHBM_CLOSE       (WM_APP + 68)      // Close dialog box.
#define SHBM_RESETTIMER  (WM_APP + 69)      // Reset timer, if this is a timed box.

// SHBox BubbleBox messages
#define SHBBM_UPDATEHTML    (WM_APP + 100) // Update span or div id. wParam = span/div id. lParam = new text
#define SHBBM_SCRIPTCOMMAND (WM_APP + 102) // lParam = BSTR bstrScript
#define SHBBM_NEWHTML       (WM_APP + 103) // lParam = pszNewHtml
#define SHBBM_SETIMAGE      (WM_APP + 104)
#define SHBM_UPDATESOFTKEYS (WM_APP + 105) // wParam is the index of the softkey to update, lParam is LPTSTR to the new value
                                           // For Softkey bars define with an HMENU wParam is 2, lParam is the HMENU to use
#define SHBM_SUBMIT_FORM    (WM_APP + 106) // wParam & lParam reserved for future use. Set to zero
#define SHBM_UPDATESKCMDS   (WM_APP + 107) // wParam is the index of the softkey to change, lParam is a pointer to the new
                                           // SOFTKEYCMD structure of that softkey. Not applicable to softkey bars
                                           // defined with an HMENU since their command IDs are specified in the HMENU
#define SHBM_OVERRIDEKEY    (WM_APP + 108) // Set the override key. wParam is virtual key of the override key.
                                           // lParam: TRUE - Override key; FALSE - Stop override key.

#ifdef __cplusplus
}
#endif

#endif //__SHBOX_H__
