#pragma once

#ifdef __cplusplus
extern "C" {
#endif // _cplusplus


#define SETUP_API __declspec(dllexport)

//
// Install_Init
//
// @comm    Called before any part of the application is installed
//
typedef enum
{
    codeINSTALL_INIT_CONTINUE  = 0,     // @comm Continue with the installation
    codeINSTALL_INIT_CANCEL             // @comm Immediately cancel the installation
}
codeINSTALL_INIT;

SETUP_API codeINSTALL_INIT Install_Init(
    HWND        hwndParent,
    BOOL        fFirstCall,     // is this the first time this function is being called?
    BOOL        fPreviouslyInstalled,
    LPCTSTR     pszInstallDir
);


//
// Install_Exit
//
// @comm    Called after the application is installed
//
typedef enum
{
    codeINSTALL_EXIT_DONE       = 0,    // @comm Exit the installation successfully
    codeINSTALL_EXIT_UNINSTALL          // @comm Uninstall the application before exiting the installation
}
codeINSTALL_EXIT;

SETUP_API codeINSTALL_EXIT Install_Exit(
    HWND    hwndParent,
    LPCTSTR pszInstallDir,      // final install directory
    WORD    cFailedDirs,
    WORD    cFailedFiles,
    WORD    cFailedRegKeys,
    WORD    cFailedRegVals,
    WORD    cFailedShortcuts
);


//
// Uninstall_Init
//
// @comm    Called before the application is uninstalled
//
typedef enum
{
    codeUNINSTALL_INIT_CONTINUE = 0,    // @comm Continue with the uninstallation
    codeUNINSTALL_INIT_CANCEL           // @comm Immediately cancel the uninstallation
}
codeUNINSTALL_INIT;

SETUP_API codeUNINSTALL_INIT Uninstall_Init(
    HWND        hwndParent,
    LPCTSTR     pszInstallDir
);


//
// Uninstall_Exit
//
// @comm    Called after the application is uninstalled
//
typedef enum
{
    codeUNINSTALL_EXIT_DONE     = 0     // @comm Exit the uninstallation successfully
}
codeUNINSTALL_EXIT;

SETUP_API codeUNINSTALL_EXIT Uninstall_Exit(
    HWND    hwndParent
);





#ifdef __cplusplus
};
#endif // _cplusplus