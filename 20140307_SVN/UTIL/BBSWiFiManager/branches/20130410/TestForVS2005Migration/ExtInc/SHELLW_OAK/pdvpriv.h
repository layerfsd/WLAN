//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
// Copyright (c) Microsoft Corporation.  All rights reserved. 
//
// Use of this source code is subject to the terms of your Windows Mobile 
// Software Shared Source Premium Derivatives License Form or other 
// applicable Microsoft license agreement for the software. If you did not 
// accept the terms of such a license, you are not authorized to use this 
// source code.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(DEFINE_SHLGUID)
#include <shlguid.h>
#endif  // !DEFINE_SHLGUID

/////////////////////////////////////////////////////////////////////////////
// Browser Command Groups
/////////////////////////////////////////////////////////////////////////////
DEFINE_SHLGUID(CGID_ShellDocView, 0x000214D1L, 0, 0);

// please keep in order from weakest to strongest
enum SecureLockState 
{
    SECURELOCK_NOCHANGE                 = -1,
    SECURELOCK_SET_UNSECURE             = 0,
    SECURELOCK_SET_MIXED                = 1,
    SECURELOCK_SET_SECURE_UNKNOWNBIT    = 2,
    SECURELOCK_SET_SECURE_WEAK          = 3,
    SECURELOCK_SET_SECURE_MEDIUM        = 4,
    SECURELOCK_SET_FORTEZZA             = 5,
    SECURELOCK_SET_SECURE_STRONG        = 6
};

enum
{
    // 
    // OLECMDID: SHDVID_SETSECURELOCK
    // PARAMS
    //  IN: SecureLockState VT_I4
    //      one of the values in the SecureLockState enum
    //
    SHDVID_SETSECURELOCK,

    // 
    // OLECMDID: SHDVID_SETSECURITYZONE
    // PARAMS
    //  IN: URLZONE VT_IU4
    //      one of the values in the URLZONE enum (see urlmon.h)
    //
    SHDVID_SETSECURITYZONE,

    // 
    // OLECMDID: SHDVID_QUERYMIXEDMODE
    // PARAMS
    //  IN: NONE
    //  OUT: bContinue, VARIANT_BOOL
    //
    //  Asks hosts if it's okay to continue mixed mode
    //
    SHDVID_QUERYMIXEDMODE,

    // 
    // OLECMDID: SHDVID_FIREFILEDOWNLOAD
    // PARAMS
    //  IN: VT_BSTR
    //      URL whose content is to be moved/copied from cache
    //
    SHDVID_FIREFILEDOWNLOAD, 

    // 
    // OLECMDID: SHDVID_IMAGEONLYPAGE
    //      Sets/Clears Image Only Page state
    //  IN: VT_BOOL
    //      TRUE to set. FALSE to clear.
    //
    SHDVID_IMAGEONLYPAGE,

    // 
    // OLECMDID: SHDVID_FULLSCREENMODE
    //  OPTIONAL IN: VT_BOOL
    //      TRUE for full screen. FALSE for normal.
    //      Toggles full screen mode if parameter is not passed.
    //
    SHDVID_FULLSCREENMODE,

    // 
    // OLECMDID: SHDVID_TEMPCHROMEMODE
    //      Shows/Hides temporary chrome while in full screen mode
    //  IN: VT_BOOL
    //      TRUE for temporary chrome. Otherwise FALSE.
    //
    SHDVID_TEMPCHROMEMODE,

    // 
    // OLECMDID: SHDVID_OPENFAVORITES
    //      Opens Favorites dialog
    //  IN: NONE
    //      
    //
    SHDVID_OPENFAVORITES,

    // 
    // OLECMDID: SHDVID_OPENHISTORY
    //      Opens History dialog
    //  IN: NONE
    //      
    //
    SHDVID_OPENHISTORY,    

    // 
    // OLECMDID: SHDVID_BACK
    //      Go back to previous page
    //  IN: NONE
    //      
    //
    SHDVID_BACK,   

    // 
    // OLECMDID: SHDVID_HIBERNATE
    //      Hibernate by freeing any non-essential resources
    //  IN: NONE
    //      
    //
    SHDVID_HIBERNATE,

    // 
    // OLECMDID: SHDVID_ADDCONTACT
    //      Save a new contact to the address book.
    //  IN: VT_BOOL Show a prompt.
    //      VT_BSTR First name.
    //      VT_BSTR First name yomi.
    //      VT_BSTR Email1
    //      VT_BSTR Mobile Phone.
    //      
    //
    SHDVID_ADDCONTACT, 

    // 
    // OLECMDID: SHDVID_CLEARSAVEDVIEWERSTATES
    //      Clear the fastback viewer state so the next back navigation won't
    //      be from fastback cache.
    //  IN: NONE
    //      
    //
    SHDVID_CLEARSAVEDVIEWERSTATES,

    // 
    // OLECMDID: SHDVID_SEARCH
    //      Navigates to the search page to display results based
    //      on query.
    //  IN: VT_BSTR Search query.
    //
    SHDVID_SEARCH,

    // 
    // OLECMDID: SHDVID_GOTOADDRESSBAR
    //      Go to Address Bar
    //  IN: NONE
    SHDVID_GOTOADDRESSBAR,

    // 
    // OLECMDID: SHDVID_INPAGEOVERVIEW
    // PARAMS
    //  IN: bPageOverview VARIANT_BOOL
    //
    SHDVID_INPAGEOVERVIEW,
    
    // 
    // OLECMDID: SHDVID_COPY
    //      Copy any selected text to the clipboard
    //  IN: NONE
    SHDVID_COPY,

    // 
    // OLECMDID: SHDVID_DEACTIVATETEXTSELECTIONMODE
    //      Turn off text selection mode
    //  IN: NONE
    SHDVID_DEACTIVATETEXTSELECTIONMODE,
    
    //
    // OLECMDID: SHDVID_RESUME_IF_PENDING_URL
    //      Verifies if the URL provided is was left pending in the
    //      previous download, and if so, it triggers a resume.
    //      If the resume was triggered returns TRUE, else FALSE.
    //
    //  IN:  VT_BSTR       URL That we are trying to navigate.
    //  OUT: VARIANT_BOOL  Returns TRUE if IE is initiating a resume, FALSE otherwise.
    SHDVID_RESUME_IF_PENDING_URL
};

/////////////////////////////////////////////////////////////////////////////
// Viewer Command Groups (common for all viewers)
/////////////////////////////////////////////////////////////////////////////

// {E92527C5-F829-4db1-B096-6C4A884EA05C}
DEFINE_GUID(CGID_Viewer, 0xe92527c5, 0xf829, 0x4db1, 0xb0, 0x96, 0x6c, 0x4a, 0x88, 0x4e, 0xa0, 0x5c);

enum
{
    // 
    // OLECMDID: VIEWERCMDID_CLEARFOCUS
    //      Remove Focus from any element that may have it
    //  IN: NONE
    VIEWERCMDID_CLEARFOCUS,
};

#ifdef __cplusplus
}
#endif
