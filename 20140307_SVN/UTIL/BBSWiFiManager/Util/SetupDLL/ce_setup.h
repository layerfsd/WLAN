//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
// ***************************************************************
//
// CE_SETUP.H
//
// This public header file specifies function prototypes and supported return values.
//
// ***************************************************************
#ifdef __cplusplus
extern "C" {
#endif

    // ***************************************************************
    //
    // Install_Init
    //
    // Called before any part of the application is installed
    //
    // ***************************************************************
    typedef enum
    {
        codeINSTALL_INIT_CONTINUE  = 0,     // Continue with the installation
        codeINSTALL_INIT_CANCEL             // Immediately cancel the installation
    }
    codeINSTALL_INIT;

    codeINSTALL_INIT
        Install_Init(
        HWND        hwndParent,
        BOOL        fFirstCall,     // is this the first time this function is being called?
        BOOL        fPreviouslyInstalled,
        LPCTSTR     pszInstallDir
        );

    typedef codeINSTALL_INIT (*pfnINSTALL_INIT)( HWND, BOOL, BOOL, LPCTSTR );
    const TCHAR szINSTALL_INIT[]    = TEXT("Install_Init");

    // ***************************************************************
    //
    // Install_Exit
    //
    // Called after the application is installed
    //
    // ***************************************************************
    typedef enum
    {
        codeINSTALL_EXIT_DONE       = 0,    // Exit the installation successfully
        codeINSTALL_EXIT_UNINSTALL          // Uninstall the application before exiting the installation
    }
    codeINSTALL_EXIT;

    codeINSTALL_EXIT
        Install_Exit(
        HWND    hwndParent,
        LPCTSTR pszInstallDir,      // final install directory
        WORD    cFailedDirs,
        WORD    cFailedFiles,
        WORD    cFailedRegKeys,
        WORD    cFailedRegVals,
        WORD    cFailedShortcuts
        );

    typedef codeINSTALL_EXIT (*pfnINSTALL_EXIT)( HWND, LPCTSTR, WORD, WORD, WORD, WORD, WORD );
    const TCHAR szINSTALL_EXIT[]    = TEXT("Install_Exit");

    // ***************************************************************
    //
    // Uninstall_Init
    //
    // Called before the application is uninstalled
    //
    // ***************************************************************
    typedef enum
    {
        codeUNINSTALL_INIT_CONTINUE = 0,    // Continue with the uninstallation
        codeUNINSTALL_INIT_CANCEL           // Immediately cancel the uninstallation
    }
    codeUNINSTALL_INIT;

    codeUNINSTALL_INIT
        Uninstall_Init(
        HWND        hwndParent,
        LPCTSTR     pszInstallDir
        );

    typedef codeUNINSTALL_INIT (*pfnUNINSTALL_INIT)( HWND, LPCTSTR );
    const TCHAR szUNINSTALL_INIT[]  = TEXT("Uninstall_Init");

    // ***************************************************************
    //
    // Uninstall_Exit
    //
    // Called after the application is uninstalled
    //
    // ***************************************************************
    typedef enum
    {
        codeUNINSTALL_EXIT_DONE     = 0     // Exit the uninstallation successfully
    }
    codeUNINSTALL_EXIT;

    codeUNINSTALL_EXIT
        Uninstall_Exit(
        HWND    hwndParent
        );

    typedef codeUNINSTALL_EXIT (*pfnUNINSTALL_EXIT)( HWND );
    const TCHAR szUNINSTALL_EXIT[]  = TEXT("Uninstall_Exit");

#ifdef __cplusplus
}       // extern "C"
#endif

