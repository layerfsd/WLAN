/*
 * Bluetooth Filter loader
 * 
 * This filter library loads the actual filter DLL.  This allows the filter to be demand-based loaded by
 * the a Transport driver when it enumerates hot-pluggable HCI instances.
 * 
 * This loader can also be compiled in a statically linked manner.
 * 
 * Copyright (c) 2007 Atheros Communications Inc.
 * All rights reserved.
 *
 * 
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
 * 
 */

#include "a_types.h"
#include "a_osapi.h"
#include "dl_list.h"
#include "athbtfilter.h"
#include <pkfuncs.h>

#ifdef ATH_USE_DLL
    /* filter DLL function entry points */
typedef int  (*ATHBT_FILTER_DLL_CREATE_ENTRY)(ATH_BT_FILTER_INSTANCE *);
typedef void (*ATHBT_FILTER_DLL_DESTROY_ENTRY)(ATH_BT_FILTER_INSTANCE *);

    
static LONG                             g_FilterRefCount = 0;   /* filter DLL reference count */
static HINSTANCE                        g_hDLL = NULL;          /* handle to DLL */
static ATHBT_FILTER_DLL_CREATE_ENTRY    g_pCreate = NULL;
static ATHBT_FILTER_DLL_DESTROY_ENTRY   g_pDestroy = NULL;
#else

    /* Static linkage */
extern void AthBtFilter_Destroy(ATH_BT_FILTER_INSTANCE *pInstance);
extern int AthBtFilter_Create(ATH_BT_FILTER_INSTANCE *pInstance);

#endif

void AthBtFilter_Detach(ATH_BT_FILTER_INSTANCE *pInstance)
{
#ifdef ATH_USE_DLL
    LONG    newVal;
    
    if (pInstance->Attached) {
        pInstance->Attached = FALSE;
        pInstance->FilterEnabled = 0;           
        if (pInstance->pContext != NULL) {
                /* let filter DLL cleanup */
            g_pDestroy(pInstance);
            pInstance->pContext = NULL;        
        }
   
        newVal = InterlockedDecrement(&g_FilterRefCount);    
        if (0 == newVal) {
            g_pCreate = NULL;
            g_pDestroy = NULL;
                /* ref count went to zero, unload filter DLL */    
            if (g_hDLL != NULL) {
                FreeLibrary(g_hDLL);
                g_hDLL = NULL;
            }
        }
    }
#else
    AthBtFilter_Destroy(pInstance);
#endif
}

/* Attach API implementations */
int AthBtFilter_Attach(ATH_BT_FILTER_INSTANCE *pInstance)
{
#ifdef ATH_USE_DLL
    int     retVal = 0;
    LONG    newVal;
    
    pInstance->Attached = TRUE;
    
    newVal = InterlockedIncrement(&g_FilterRefCount);
    
    do {
                
        if (newVal != 1) {
                /* already loaded, nothing else to do */
            break;   
        }  
            /* set failure status */
        retVal = -1;
        
            /* try to load the filter DLL */
        g_hDLL = LoadDriver(TEXT("athbtfilter.dll"));
        
        if (NULL == g_hDLL) {
            RETAILMSG(1, (TEXT("Failed to load athbt filter (winerror:%d) \r\n"), GetLastError()));
            break;    
        }        
            /* get the API entry points */
        g_pCreate = (ATHBT_FILTER_DLL_CREATE_ENTRY) GetProcAddress(g_hDLL, TEXT("AthBtFilter_Create"));
        if (NULL == g_pCreate) {
            RETAILMSG(1, (TEXT("Failed to get Create API from athbt filter (winerror:%d) \r\n"), GetLastError()));   
            break; 
        }        
        g_pDestroy = (ATHBT_FILTER_DLL_DESTROY_ENTRY) GetProcAddress(g_hDLL, TEXT("AthBtFilter_Destroy"));
        if (NULL == g_pDestroy) {
            RETAILMSG(1, (TEXT("Failed to get Create API from athbt filter (winerror:%d) \r\n"), GetLastError()));  
            break;    
        }
       
            /* if we reach here, everything is OK */
        retVal = 0;
        
    } while (FALSE); 
     
    if (0 == retVal) {
            /* call filter library creation API */
        retVal = g_pCreate(pInstance);     
    }
    
    if (retVal < 0) {
        AthBtFilter_Detach(pInstance);    
    }
    
    return retVal;
#else
    return AthBtFilter_Create(pInstance);
#endif
}



