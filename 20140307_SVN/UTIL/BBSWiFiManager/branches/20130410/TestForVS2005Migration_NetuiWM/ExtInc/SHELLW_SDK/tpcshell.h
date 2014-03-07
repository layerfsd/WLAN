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
#ifndef _TPCSHELL_H_
#define _TPCSHELL_H_
// Copyright (c) Microsoft Corporation. All rights reserved.	
//***   tpcshell.h -- TPC *shell.h-like things
// DESCRIPTION
//  'extensions' to *shell.h. e.g. TPC-specific APIs


#ifdef __cplusplus
extern "C" {
#endif

// IME Escape functions
#define IME_ESC_SET_MODE                   (IME_ESC_PRIVATE_FIRST)
#define IME_ESC_GET_MODE                   (IME_ESC_PRIVATE_FIRST + 1)
#define IME_ESC_SEND_BACK_TO_FOCUS_WINDOW  (IME_ESC_PRIVATE_FIRST + 2)
#define IME_ESC_SET_CUSTOM_SYMBOLS         (IME_ESC_PRIVATE_FIRST + 3)
#define IME_ESC_RETAIN_MODE_ICON           (IME_ESC_PRIVATE_FIRST + 4)
#define IME_ESC_SAVE_USER_WORDS            (IME_ESC_PRIVATE_FIRST + 5)
#define IME_ESC_CLEAR_ON_BACK_PRESS_HOLD   (IME_ESC_PRIVATE_FIRST + 6)
#define IME_ESC_SKIP_AMBIG_MODE            (IME_ESC_PRIVATE_FIRST + 7)

void SHSendBackToFocusWindow(UINT uMsg, WPARAM wp, LPARAM lp);

// WM_IME_NOTIFY/IMN_PRIVATE constants
#define IMNP_CLEAR_ALL                      (1)     // Clear window contents

#ifdef __cplusplus
}
#endif

#endif // _TPCSHELL_H_
