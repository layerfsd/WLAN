//------------------------------------------------------------------------------
// <copyright file="athsrvc.cpp" company="Atheros">
//    Copyright (c) 2007-2008 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include <Ntddndis.h>
#include <Nuiouser.h>
#include <types.h>
#include <ceddk.h>
#include <stdio.h>
#include <service.h>
#include <svsutil.hxx>
#include <creg.hxx>
#include <a_drv.h>
// Use strsafe.h, rather that CRT string manipulation, since these
// functions are safer - especially when manipulating external strings
// like those provided in IOCTL calls or those read from the network
#include <strsafe.h>

#include <eventlogmsgs.h>

#define AR6001  1
#define AR6002  2
#define AR6K_DBG_BUFFER_SIZE    1500
#define AR6K_DBG_FILE_SIZE      (sizeof(dbg_rec) * 30) // 45K is a maximum size of DBGLOG
#define AR6K_DBG_MIN_FILE_SIZE  (sizeof(dbg_rec) * 15)
#define DBGLOG_OUTPUT_FILE      L"\\windows\\ar6ktgt.log"
#define DBGLOG_SDCARD_FILE      L"\\Storage Card\\ar6ktgt.log"

#define ATH_MONITOR_EVENT       0
#define ATH_DBGLOG_EVENT        1
#define ATH_SRVC_NUMEVENTS      2
HANDLE  hAthSrvcEvent[ATH_SRVC_NUMEVENTS];

TCHAR       dbglogRoot[256];
FILE       *g_outFile       = NULL;
HINSTANCE   g_hInst         = NULL;
HANDLE      g_hListenThread;
LONG        g_dwServiceState;     // Whether we're shutdown, starting up, running, shutting down, or unloading.
HANDLE      dbgLogEvent;          // Handle to dbgLog

// Global critical section
CRITICAL_SECTION g_cs;

// Handle to the worker thread
HANDLE  g_hWorkerThread;
HANDLE  wlanHandle          = NULL;
BOOL    gbIsExit            = FALSE;
BOOL    gbIsRaw             = FALSE;
DWORD   gbappId             = 1;
DWORD   g_dbgLogSize        = 0;
DWORD   dbglogFileSize      = 0;

struct {
    DWORD ts;
    DWORD length;
    UCHAR log[AR6K_DBG_BUFFER_SIZE];
} dbg_rec;

// Use CE's built-in debugging framework.
#ifdef DEBUG
DBGPARAM dpCurSettings = {
    TEXT("AthServer"), {
    TEXT("Error"),TEXT("Init"),TEXT("Host Driver"),TEXT("Target"),
    TEXT("API"),TEXT(""),TEXT(""),TEXT(""),
    TEXT(""),TEXT(""),TEXT(""),TEXT(""),
    TEXT(""),TEXT(""),TEXT(""),TEXT("") },
    0x0007  // Turn on Error, Init, and Client DEBUGZONE's by default
};

#define ZONE_ERROR  DEBUGZONE(0)
#define ZONE_INIT   DEBUGZONE(1)
#define ZONE_CLIENT DEBUGZONE(2)
#define ZONE_INTRF  DEBUGZONE(3)
#define ZONE_API    DEBUGZONE(4)
#endif

extern "C"
BOOL GetAR6KDbgLog (HANDLE hAdapter)
{
    BOOL  fRetVal           = FALSE;
    DWORD dwReturnedBytes   = 0;

    if (g_outFile == NULL)
    {
        return TRUE;
    }

    do
    {
        // Issue IOCTL to get the Debug Log info
        dwReturnedBytes = 0;

        fRetVal = DeviceIoControl(
                        hAdapter,
                        IOCTL_CAR6K_GET_DBGLOG,
                        NULL,
                        0,
                        dbg_rec.log,
                        AR6K_DBG_BUFFER_SIZE,
                        &dwReturnedBytes,
                        NULL);

        if(fRetVal != TRUE)
        {
            break;
        }

        if (dwReturnedBytes && dwReturnedBytes <= AR6K_DBG_BUFFER_SIZE)
        {
            if ( (g_dbgLogSize + sizeof(dbg_rec)) > dbglogFileSize)
            {
                g_dbgLogSize = 0;
                // rewind the file pointer to overwrite existing conetent
                if (fseek (g_outFile, 0L, SEEK_SET))
                {
                   fRetVal = FALSE;
                   break;
                }
            }

            dbg_rec.length = dwReturnedBytes;
            dbg_rec.ts = GetTickCount();
            fwrite (&dbg_rec, sizeof(dbg_rec), 1, g_outFile);
            fflush(g_outFile);
            g_dbgLogSize += sizeof(dbg_rec);
        }

    }while (FALSE);

    return fRetVal;
}

extern "C"
HANDLE GetAR6KHostDriverHandle ()
{
    HANDLE hAdapter = NULL;

    //
    // Get the Target Type from AR6K_SD to check whether the inserted targtet is
    // AR6001 (Dragon) or AR6002 (Mercury)
    //
    hAdapter = CreateFile(TEXT("DRG1:"),GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                INVALID_HANDLE_VALUE);

    return hAdapter;
}

/* get a string value key */
BOOL GetRegistryKeyStringValue(HKEY     hKey,
                               WCHAR   *pKeyPath,
                               WCHAR   *pValueName,
                               WCHAR   *pStringBuffer,
                               ULONG    MaxLength,
                               ULONG   *pLength)
{
    LONG  status;       /* reg api status */
    HKEY  hOpenKey;     /* opened key handle */
    DWORD type;
    ULONG bufferSize = MaxLength;
    BOOL  bRet = FALSE;

    status = RegOpenKeyEx(hKey,
                          pKeyPath,
                          0,
                          0,
                          &hOpenKey);

    if (status != ERROR_SUCCESS) {
        return FALSE;
    }

    status = RegQueryValueEx(hOpenKey,
                             pValueName,
                             NULL,
                             &type,
                             (PUCHAR)pStringBuffer,
                             &bufferSize);

    if (ERROR_SUCCESS == status)
    {
        if (REG_SZ == type)
        {
            *pLength = bufferSize;
            bRet =  TRUE;
        }
    }

    RegCloseKey (hOpenKey);

    return bRet;
}

/* get a dword value key */
BOOL GetRegistryKeyValue(HKEY    hKey,
                         WCHAR  *pKeyPath,
                         WCHAR  *pValueName,
                         DWORD  *pdwValue)
{
    LONG  status;       /* reg api status */
    HKEY  hOpenKey;     /* opened key handle */
    DWORD type;
    DWORD dwDataSize;
    DWORD dwRegVal;
    BOOL  bRet = FALSE;

    status = RegOpenKeyEx(hKey,
                          pKeyPath,
                          0,
                          0,
                          &hOpenKey);

    if (status != ERROR_SUCCESS) {
        return FALSE;
    }

    dwDataSize = sizeof(DWORD);

    status = RegQueryValueEx(hOpenKey,
                             pValueName,
                             NULL,
                             &type,
                             (LPBYTE)&dwRegVal,
                             &dwDataSize);


    if (ERROR_SUCCESS == status)
    {
        if (REG_DWORD == type)
        {
            *pdwValue = dwRegVal;
            bRet =  TRUE;
        }
    }

    RegCloseKey(hOpenKey);

    return bRet;
}

/* this function enable olca configuration setup using IOCTLS from the registry */
static BOOL ProcessOlcaConfiguration ()
{
    TCHAR keyPath[64];
    TCHAR valueKey[32];
    DWORD dwRegValue = 0;
    DWORD dwReturnedBytes = 0;

    _tcscpy(keyPath, _T("Services\\ATHSRVC"));

    do
    {
        _tcscpy(valueKey, _T("appId"));

        if (!GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                                 keyPath,
                                 valueKey,
                                 &dwRegValue))
        {
            gbappId = 1;
        }
        else
        {
            gbappId = dwRegValue;
        }

#ifdef HTC_EPPING_TEST
            // force HTC ping application 
        gbappId = 3;
#endif

        _tcscpy (valueKey, _T("dbglogFileSize"));

        if (!GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                                 keyPath,
                                 valueKey,
                                 &dwRegValue))
        {
            dbglogFileSize = AR6K_DBG_FILE_SIZE;
        }
        else
        {
            dwRegValue = (dwRegValue / sizeof(dbg_rec) + 1) * sizeof(dbg_rec);
            dwRegValue = (dwRegValue < AR6K_DBG_MIN_FILE_SIZE) ? AR6K_DBG_MIN_FILE_SIZE : dwRegValue;
            dbglogFileSize = dwRegValue;
        }

        _tcscpy (valueKey, _T("dbglogRoot"));

        if (!GetRegistryKeyStringValue(HKEY_LOCAL_MACHINE,
                                       keyPath,
                                       valueKey,
                                       dbglogRoot,
                                       sizeof(dbglogRoot)/sizeof(TCHAR),
                                       &dwRegValue))
        {
            _tcscpy (dbglogRoot, DBGLOG_OUTPUT_FILE);
        }
        else
        {
            _tcscat (dbglogRoot, TEXT("\\ar6ktgt.log"));
        }

    }while (FALSE);

    return TRUE;
}

extern "C"
DWORD WINAPI AthrWorkerThread(LPVOID lpv)
{
    DWORD   dwRet = 0;
    DWORD   dwCardtype = 0;
    DWORD   dwReturnedBytes = 0;
    HANDLE  hAdapter = NULL;
    TCHAR   cmdLine[256] = {0};

    do
    {
        dwRet = WaitForMultipleObjects (ATH_SRVC_NUMEVENTS, hAthSrvcEvent, FALSE, INFINITE);

        if (WAIT_OBJECT_0 == dwRet)
        {
            RETAILMSG (1, (L"Athsrvc::Thread Wakes up\n"));

            if (TRUE == gbIsExit)
            {
                RETAILMSG (1, (L"Athsrvc::Thread going to exit\n"));
                break;
            }

            do
            {
                hAdapter = GetAR6KHostDriverHandle();

                if (INVALID_HANDLE_VALUE == hAdapter) {
                    RETAILMSG (1, (L"Athsrvc::Getting AR6K Adapter is failed\n"));
                    break;
                }

                ProcessOlcaConfiguration ();

                switch (gbappId)
                {
                    case 0x0:
                        RETAILMSG (1, (L"Athsrvc::No application loaded \n"));
                        break;
                    case 0x1:
                        RETAILMSG (1, (L"Athsrvc::Load NDIS driver for WLAN/TCMD application \n"));
                        DeviceIoControl(
                                hAdapter,
                                IOCTL_CAR6K_NDIS_REGISTER,
                                NULL,
                                0,
                                NULL,
                                0,
                                &dwReturnedBytes,
                                NULL);

                        g_outFile = _wfopen (dbglogRoot, L"wbc");

                        if (NULL == g_outFile)
                        {
                            break;
                        }

                        break;

                    case 0x2:
#ifdef WINCE_ART
                        RETAILMSG (1, (L"Athsrvc::Load ART driver \n"));
                        DeviceIoControl(
                                hAdapter,
                                IOCTL_CAR6K_ART_REGISTER,
                                NULL,
                                0,
                                NULL,
                                0,
                                &dwReturnedBytes,
                                NULL);
#endif
                        break;
                    case 0x3:
#ifdef HTC_EPPING_TEST
                        RETAILMSG (1, (L"Athsrvc::Load EPPING test driver \n"));
                        DeviceIoControl(
                                hAdapter,
                                IOCTL_CAR6K_EPPING_REGISTER,
                                NULL,
                                0,
                                NULL,
                                0,
                                &dwReturnedBytes,
                                NULL);
#endif
                        break;
                    default:
                        RETAILMSG (1, (L"Athsrvc::Appid %d not supported\n",gbappId));
                        break;
                }


            } while (FALSE);

            if (hAdapter)
            {
                CloseHandle (hAdapter);
                hAdapter = NULL;
            }
        }

        if ((WAIT_OBJECT_0 + ATH_DBGLOG_EVENT) == dwRet)
        {
            hAdapter = (NULL == hAdapter)?GetAR6KHostDriverHandle():hAdapter;

            if (hAdapter != INVALID_HANDLE_VALUE)
            {
                //
                // DbgLog event is executed by the AR6K Driver
                //
                if (FALSE == GetAR6KDbgLog (hAdapter))
                {
                    RETAILMSG (1, (L"Unknown Error :: Dbglog file is closed\n"));
                    if (g_outFile)
                    {
                        fclose (g_outFile);
                        g_outFile = NULL;
                    }
                }
            }
        }


    }while (TRUE);

    CloseHandle(g_hWorkerThread);
    g_hWorkerThread = NULL;

    if (wlanHandle)
    {
        CloseHandle (wlanHandle);
        wlanHandle = NULL;
    }

    if (dbgLogEvent)
    {
        CloseHandle (dbgLogEvent);
        dbgLogEvent = NULL;
    }

    if (g_outFile)
    {
        fclose (g_outFile);
        g_outFile = NULL;
    }

    return 1;
}

extern "C" BOOL WINAPI DllEntry( HANDLE hInstDll, DWORD fdwReason, LPVOID lpvReserved)
{
    BOOL  fRet      = TRUE;
    DWORD dwRetCode = 0;

RETAILMSG (1, (L"builder:ar6k2:DllEntry() start\r\n"));

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DEBUGMSG(ZONE_INIT, (TEXT("ATHSRVC: DllMain attached\r\n")));
            DEBUGREGISTER((HINSTANCE)hInstDll);

            DisableThreadLibraryCalls((HMODULE)hInstDll);
            g_hInst = (HINSTANCE)hInstDll;

            InitializeCriticalSection(&g_cs);

            g_dwServiceState = SERVICE_STATE_UNINITIALIZED;

            wlanHandle = CreateEvent (NULL, FALSE, FALSE, L"ATHRWLAN6KEVENT");
            dwRetCode  = GetLastError ();

            if (NULL == wlanHandle)
            {
                RETAILMSG (1, (L"Event creation/open failed\n"));
                fRet = FALSE;
                break;
            }
            if (ERROR_ALREADY_EXISTS == dwRetCode)
            {
                RETAILMSG (1, (L"Event is already created %x\n", wlanHandle));
            }
            else
            {
                RETAILMSG (1, (L"Event is successfully created %x\n", wlanHandle));
            }

            dbgLogEvent = CreateEvent (NULL, FALSE, FALSE, TEXT("AR6KTGT_DBGLOG"));
            dwRetCode   = GetLastError ();

            if (NULL == dbgLogEvent)
            {
                RETAILMSG (1, (L"Create Dbglog event failed %u\n",GetLastError()));
                return FALSE;
            }

            hAthSrvcEvent[ATH_MONITOR_EVENT] = wlanHandle;
            hAthSrvcEvent[ATH_DBGLOG_EVENT]  = dbgLogEvent;
            break;

        case DLL_PROCESS_DETACH:

            DEBUGMSG(ZONE_INIT, (TEXT("ATHSRVC: DllMain detach\r\n")));

            if (g_hWorkerThread)
            {
                TerminateThread (g_hWorkerThread, 0);
            }

            DeleteCriticalSection(&g_cs);
            break;
    }
    return fRet;
}

extern "C" DWORD ATH_Init (DWORD dwData)
{
    DEBUGMSG(ZONE_INTRF,(L"ATHSRVC: ATH_Init(0x%08x)\r\n",dwData));

    EnterCriticalSection (&g_cs);

    if (g_dwServiceState != SERVICE_STATE_UNINITIALIZED) {
        // Someone is trying to load multiple times (for example,
        // trying to create "ATH1:").
        // Not supported, so fail to load service.
        DEBUGMSG(ZONE_ERROR,(L"ATHSRVC: ERROR: ATH service already "
                             L"initialized on ATH_Init() call\r\n"));
        LeaveCriticalSection (&g_cs);
        return 0;
    }

    g_hWorkerThread = CreateThread (NULL, 0, AthrWorkerThread, (LPVOID*) NULL, 0, NULL);

    if (NULL == g_hWorkerThread)
    {
        g_dwServiceState = SERVICE_STATE_OFF;
        LeaveCriticalSection (&g_cs);
        return 0;
    }
    else
    {
        CloseHandle (g_hWorkerThread);
    }

    g_dwServiceState = SERVICE_STATE_ON;

    LeaveCriticalSection (&g_cs);

    return 1;
}

extern "C" DWORD ATH_Deinit (DWORD dwData)
{
    EnterCriticalSection (&g_cs);

    g_dwServiceState = SERVICE_STATE_UNLOADING;

    // If worker threads are active, you MUST block until they
    // have shutdown.  Real services may take extra action to force
    // the threads to end (to speed up the shutdown process.

    // This function is also the proper place to do
    // uninitialization of other components that are not safe
    // to be called from DllMain.

    g_dwServiceState = SERVICE_STATE_SHUTTING_DOWN;

    if (g_hWorkerThread)
    {
        DEBUGMSG(ZONE_INIT,(L"ATHSRVC: Waiting for worker thread to "
                            L"complete before service shutdown\r\n"));

        RETAILMSG (1, (L"ATHSRVC: Wait for worker thread to kill\n"));

        //
        // Wakeup thread to kill
        //
        gbIsExit = TRUE;

        if (wlanHandle )
        {
            SetEvent (wlanHandle);
            Sleep (100);
        }
        HANDLE hWorker = g_hWorkerThread;
        LeaveCriticalSection (&g_cs);

        // Block until the worker is through running.
        WaitForSingleObject (hWorker, INFINITE);
        RETAILMSG (1, (L"ATHSRVC: Thread has been killed safely\n"));
    }
    else
    {
        LeaveCriticalSection (&g_cs);
    }

    g_dwServiceState = SERVICE_STATE_OFF;



    // Service is unloaded no matter what is returned.
    DEBUGMSG(ZONE_INIT,(L"ATHSRVC: Completed shutdown.  Returning to "
                        L"services.exe for unload\r\n"));
    return 1;

}

extern "C" DWORD ATH_Open (DWORD dwData, DWORD dwAccess,
                           DWORD dwShareMode)
{
    //
    // Handles calls application made to CreateFile.
    // Since no client state is maintained, no more work is required.
    //
    DEBUGMSG(ZONE_INTRF,   (L"ATHSRVC:ATH_Open(0x%08x,0x%08x,0x%08x)\r\n",
                            dwData,dwAccess,dwShareMode));

    return 1;
}

extern "C" BOOL ATH_Close (DWORD dwData)
{
    // Handles calls application made to CloseHandle.
    // dwData is the value returned during the call to ATH_Open
    // This is where resources allocated in the dwData handle
    // (had there been any) would be freed.
    DEBUGMSG(ZONE_INTRF,(L"ATHSRVC: ATH_Close(0x%08x)\r\n",dwData));

    return 1;

}

extern "C" BOOL ATH_IOControl(DWORD dwData, DWORD dwCode, PBYTE pBufIn,
                              DWORD dwLenIn, PBYTE pBufOut, DWORD dwLenOut,
                              PDWORD pdwActualOut)
{
    DWORD dwError = ERROR_SUCCESS;

    DEBUGMSG(ZONE_INTRF,(L"ATHSRVC: ATH_IOControl(0x%08x,0x%08x,"
                         L"0x%08x,0x%08x,0x%08x,0x%08x,0x%08x)\r\n",
                         dwData, dwCode, pBufIn, dwLenIn,
                         pBufOut, dwLenOut, pdwActualOut));

    EnterCriticalSection (&g_cs);

    switch (dwCode)
    {
        // Control code sent to start a service.  (services start ATH0:)
        case IOCTL_SERVICE_START:
            // In real services, you need to be very careful about
            // state changes and timing issues, and you'll almost
            // certainly do more work than just changing a var's value.
            // ATHR server will accept connections.
            if (g_dwServiceState != SERVICE_STATE_OFF) {
                DEBUGMSG(ZONE_ERROR,(L"ATHSRVC: ERROR: IOCTL_SERVICE_START"
                                     L" fails because service is not off. "
                                     L"State=<%d>\r\n",g_dwServiceState));
                dwError = ERROR_SERVICE_ALREADY_RUNNING;
            }
            else
            {
                DEBUGMSG(ZONE_INIT,(L"ATHSRVC: State changed to ON\r\n"));
                g_dwServiceState = SERVICE_STATE_ON;
                dwError = ERROR_SUCCESS;
            }
            break;

        // Control code sent to refresh a service. (services refresh ATH0:)
        case IOCTL_SERVICE_REFRESH:
            // In real services, you need to be very careful about
            // state changes and timing issues, and you'll almost
            // certainly do more work than just changing a var's value.

            if (g_dwServiceState != SERVICE_STATE_ON)
            {
                DEBUGMSG(ZONE_ERROR,(L"ATHSRVC:ERROR:IOCTL_SERVICE_REFRESH"
                                     L" fails because service is not on. "
                                     L"State=<%d>\r\n",g_dwServiceState));
                dwError = ERROR_SERVICE_NOT_ACTIVE;
            }
            else
            {
                DEBUGMSG(ZONE_INIT,(L"ATHSRVC: Stop on a refresh\r\n"));
                g_dwServiceState = SERVICE_STATE_SHUTTING_DOWN;
                // Shut the service down, re-read configuration
                // (if we have any) and then restart.
                DEBUGMSG(ZONE_INIT,(L"ATHSRVC: Restarting on refresh\r\n"));
                g_dwServiceState = SERVICE_STATE_ON;
                dwError = ERROR_SUCCESS;
            }
            break;

        // Control code sent to stop a service.  (services stop ATH0:)
        case IOCTL_SERVICE_STOP:

            // In real services, you need to be very careful about
            // state changes and timing issues, and you'll almost
            // certainly do more work than just changing a var's value.
            // ATHR Server will stop accepting new connections.
            if (g_dwServiceState != SERVICE_STATE_ON)
            {
                DEBUGMSG(ZONE_ERROR,(L"ATHSRVC:ERROR:IOCTL_SERVICE_STOP "
                                     L"fails.  Service state is not on. "
                                     L"Current State=<%d>\r\n",
                                     g_dwServiceState));

                dwError = ERROR_SERVICE_NOT_ACTIVE;
            }
            else
            {
                DEBUGMSG(ZONE_INIT,(L"ATHSRVC: Service stopping\r\n"));
                g_dwServiceState = SERVICE_STATE_OFF;
                dwError = ERROR_SUCCESS;
            }
            break;

        // An application (possibly services.exe itself) is
        // querying for the service's running state.
        case IOCTL_SERVICE_STATUS:

            // No need for critical section since this is an atomic read.
            __try
            {
                if (pBufOut && dwLenOut == sizeof(DWORD))
                {
                    *(DWORD *)pBufOut = g_dwServiceState;

                    if (pdwActualOut)
                        *pdwActualOut = sizeof(DWORD);

                    dwError = ERROR_SUCCESS;
                }
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                DEBUGMSG(ZONE_ERROR,(L"ATHSRVC: ERROR Invalid pointer "
                                     L"on IOCTL_SERVICE_STATUS\r\n"));
                dwError = ERROR_INVALID_PARAMETER;
            }
            break;


        case IOCTL_SERVICE_QUERY_CAN_DEINIT:
            if (g_dwServiceState == SERVICE_STATE_ON)
            {
                *pBufOut = 0;
            }
            else
            {
                *pBufOut = 1;
            }

            break;


    }

    if (dwError != ERROR_SUCCESS)
    {
        SetLastError (dwError);
    }

    LeaveCriticalSection(&g_cs);

    return (dwError == ERROR_SUCCESS);
}


extern "C" DWORD ATH_Seek (DWORD dwData, long pos, DWORD type)
{
    return 0;
}



