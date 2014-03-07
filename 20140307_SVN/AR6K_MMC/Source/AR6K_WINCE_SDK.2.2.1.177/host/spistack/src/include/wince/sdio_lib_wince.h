//------------------------------------------------------------------------------
// <copyright file="sdio_lib_wince.h" company="Atheros">
//    Copyright (c) 2008 Atheros Corporation.  All rights reserved.
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

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 SDIO Library includes for Wince CE specific APIs

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef SDIO_LIB_WINCE_H_
#define SDIO_LIB_WINCE_H_

#include "sdlist.h"

    /* generic registry data fetch */
SDIO_STATUS SDLIB_GetRegistryKeyValue(HKEY hKey, 
                                      WCHAR *pKeyPath, 
                                      WCHAR *pValueName, 
                                      PUCHAR pValue, 
                                      ULONG BufferSize);
    /* get generic DWORD data from the registry */                                      
SDIO_STATUS SDLIB_GetRegistryKeyDWORD(HKEY   hKey, 
                                      WCHAR  *pKeyPath,
                                      WCHAR  *pValueName, 
                                      DWORD  *pValue);
                                  
#define SDGetDebugLevelFromPath(hkey,Path)              \
    SDLIB_GetRegistryKeyDWORD(hkey,                     \
                              (Path),                   \
                              TEXT("debuglevel"),       \
                              &DBG_GET_DEBUG_LEVEL())  
                                                                
typedef VOID (*PCT_WORKER_CALLBACK)(PVOID);

typedef struct _CT_WORKER_TASK {
    SDLIST              List;
    PCT_WORKER_CALLBACK pCallBack;
    PVOID               pContext;   
    BOOL                Queued;
}CT_WORKER_TASK, *PCT_WORKER_TASK;

#define SDLIB_InitializeWorkerTask(pT,pC,pCon)  { \
    memset((pT),0,sizeof(CT_WORKER_TASK));        \
    (pT)->pCallBack = (pC);                       \
    (pT)->pContext = (pCon);                      \
}

SDIO_STATUS SDLIB_QueueWorkTask(PVOID Worker, PCT_WORKER_TASK pTask);

VOID SDLIB_DestroyWorker(PVOID Worker);

PVOID SDLIB_CreateWorker(INT WorkerPriority);
                            
VOID SDLIB_FlushWorkTask(PVOID Worker, PCT_WORKER_TASK pTask);

BOOL SDIO_LibraryInit();

VOID SDIO_LibraryDeinit();

#endif /*SDIO_LIB_WINCE_H_*/
