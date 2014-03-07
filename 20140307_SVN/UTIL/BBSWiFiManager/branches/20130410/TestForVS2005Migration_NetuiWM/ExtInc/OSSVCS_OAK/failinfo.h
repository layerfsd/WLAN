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
//
// Security Loader Failure Info Declarations 
//
//    Interface header file
//

#pragma once


// Name of global LSM mutex that protects the kernel failure info stack
#define LSM_FAILURE_INFO_MUTEX_NAME     TEXT("SYSTEM/__LSM_Failure_Info__")

// Number of Failure Info blobs to be allocated at a time in the security loader.
#define KERNEL_FAILURE_INFO_CHUNK       4

// Max number of bytes a hash will take (MD5 needs 16, SHA-1 needs 20).
#define MAX_HASH_BYTES_LEN              20

// Error code returned from CreateProcess when the user clicks No on a
// prompt.
#define LSM_ERROR_USER_ACCESSDENIED     (30123UL)

// Window message to prompt UI
#define WM_PROMPT_LSM_UI                (WM_USER + 0x0100)

enum ModuleType { mt_dll, mt_exe, mt_cab };
enum FindType { ownerthreadid, ownerid };

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

//
// Class declarations
//

typedef struct tagFailureInfo 
{
    ModuleType  mt;
    DWORD   dwOwnerThreadId;
    DWORD   dwOwnerID;
    BYTE    pbHash[MAX_HASH_BYTES_LEN];
    DWORD   cbHash;
    BOOL    fShouldPrompt;
    BOOL    fIsAllocated;
    TCHAR   szModuleName[MAX_PATH];
    TCHAR   szPublisherName[MAX_PATH];
    BOOL    fUserAccepted;    
} FAILURE_INFO, *PFAILURE_INFO;


class CFailureInfo : public tagFailureInfo
{
public:
    void    SetHash(LPBYTE pbHashToSet, DWORD cbHashToSet) 
    { 
        memset(pbHash, 0, MAX_HASH_BYTES_LEN); 
        ASSERT(cbHashToSet <= MAX_HASH_BYTES_LEN);
        cbHash = min(cbHashToSet, MAX_HASH_BYTES_LEN);
        memcpy(pbHash, pbHashToSet, cbHash);
    }
    
    void    SetPromptFlag(BOOL fPrompt) { fShouldPrompt = fPrompt; }
    void    SetUserAcceptedFlag(BOOL fAccepted) { fUserAccepted = fAccepted; }    
    void    SetThreadId(DWORD dwThreadId) { dwOwnerThreadId = dwThreadId; }
    void    SetOwnerID(DWORD dwID) { dwOwnerID = dwID; }
    LPTSTR  GetModuleName() { return szModuleName; }
    void    SetModuleType(ModuleType mtype) { mt = mtype; }
    void    SetModuleName(LPCTSTR pszModName, DWORD cch) 
    { 
        ASSERT(pszModName);

        if (cch < ARRAYSIZE(szModuleName))
        {
            memcpy(szModuleName, pszModName, (cch + 1) * sizeof(TCHAR)); 
        }
    }

    void    SetPublisherName(LPCTSTR pszPublisherName, DWORD cch) 
    { 
        ASSERT(pszPublisherName);

        if (cch < ARRAYSIZE(szPublisherName))
        {
            memcpy(szPublisherName, pszPublisherName, (cch + 1) * sizeof(TCHAR)); 
        }
    }

};

typedef struct tagPromptUIParams
{
    ModuleType  mt;
    LPTSTR      szModuleName;
    LPCTSTR     szUIText;
    BOOL        fTimeout;
} PROMPTUIPARAMS, *PPROMPTUIPARAMS;


//
// Function prototypes for PSL functions in shell
//

// Thread func to handle all LSM UI.
DWORD WINAPI 
LSMThreadProc(LPVOID lpParameter);


// Shell PSLs

// Prompt users to allow or disallow running an unsigned module. 
// This version of the function is called by the loader module
HRESULT 
LSM_LoaderPromptOnUnsignedCode(HANDLE hFailureInfo);


// Prompt users to allow or disallow running an unsigned module. 
// This version of the function is called by users directly
HRESULT 
LSM_PromptOnUnsignedCode(LPCTSTR pszModuleName,
                         LPCTSTR pszPublisherName,
                         LPBYTE pbHash,
                         DWORD cbHash,
                         BOOL fShouldPrompt,
                         BOOL fUserAccepted,
                         BOOL fTimeOutPrompt);

//
// Function prototypes for functions exported from ossvcs.dll (which are also PSL's from the shell)
//

// Allocates a failure info blob at top of stack in security loader
HRESULT
LSM_AllocFailureInfo(DWORD dwOwnerID, DWORD dwOwnerThreadId, HANDLE *phFailureInfo, ModuleType mtype);

// Frees failure info blob
HRESULT
LSM_FreeFailureInfo(HANDLE hFailureInfo);

// Clears failure info blob 
HRESULT
LSM_ClearFailureInfo(HANDLE hFailureInfo, BOOL fClearOwnerThreadId);

HRESULT 
LSM_SetFailureInfoThreadId(DWORD dwOwnerID, DWORD dwOwnerThreadId);

HRESULT 
LSM_SetFailureInfoModuleName(DWORD dwOwnerThreadId, LPTSTR szModuleName);

HRESULT 
LSM_SetFailureInfoPublisherName(DWORD dwOwnerThreadId, LPTSTR szPublisherName);

HRESULT 
LSM_SetFailureInfoHash(DWORD dwOwnerThreadId, LPBYTE pbHash, DWORD cbHash);

HRESULT 
LSM_SetFailureInfoPromptFlag(DWORD dwOwnerThreadId, BOOL fShouldPrompt);

// This function is not a PSL. It is to be used only within shell's process space
// (Currently used only by LSM_LoaderPromptOnUnsignedCode)
CFailureInfo *
LSM_GetFailureInfo(HANDLE hFailureInfo);

//
// Friendly names for LSM_* thunks
//
#define xxx_LSM_AllocFailureInfo                    PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 72, (DWORD, DWORD, HANDLE*, ModuleType))

#define xxx_LSM_FreeFailureInfo                     PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 73, (HANDLE))

#define xxx_LSM_ClearFailureInfo                    PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 74, (HANDLE, BOOL))

#define xxx_LSM_PromptOnUnsignedCode                PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 75, (LPCTSTR, LPCTSTR, LPBYTE, DWORD, BOOL, BOOL, BOOL))

#define xxx_LSM_LoaderPromptOnUnsignedCode          PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 76, (HANDLE))

#define xxx_LSM_SetFailureInfoThreadId              PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 82, (DWORD, DWORD))

#define xxx_LSM_SetFailureInfoModuleName            PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 83, (DWORD, LPTSTR))

#define xxx_LSM_SetFailureInfoHash                  PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 84, (DWORD, LPBYTE, DWORD))

#define xxx_LSM_SetFailureInfoPromptFlag            PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 85, (DWORD, BOOL))

#define xxx_LSM_SetFailureInfoPublisherName         PRIV_IMPLICIT_DECL(HRESULT, SH_SHELL, 86, (DWORD, LPTSTR))

//
// Forward declarations for variables in the kernel
//
extern CFailureInfo  *g_aFailureInfo;
extern LPBYTE g_pbPEL;
extern DWORD g_cbPEL;


