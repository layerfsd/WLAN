/*
 *
 * Copyright (c) 2008 Atheros Communications Inc.
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
 */
 
/*
 *
 *  Plumbing for running MTE in the bootloader
 * 
 * 
 * 
 */
 

#include <windows.h>
#include "system.h"
#include "wifitest.h"
#ifdef AR6K_MTE_FIRMWARE_BLOB 
#include "athwlan_h.bin"
#include "datapatch_h.bin"
#endif
#include "halether.h"

A_OUTPUT_DEBUG_FUNC g_pDebugOutputFunc = EdbgOutputDebugString;

A_BOOL GetFirmwareInfo(AR6K_FIRMWARE_INFO *pInfo)
{
#ifdef AR6K_MTE_FIRMWARE_BLOB    
    /* set firmware blob locations for testing */
    pInfo->pAthwlan = athwlan;
    pInfo->AthwlanLength = sizeof(athwlan);
    pInfo->pDataPatch = datapatch;
    pInfo->DataPatchLength = sizeof(datapatch);
    return TRUE;    
#else
    /* TODO, get firmware info from bootloader environment */
    return FALSE;
#endif
}



// TODO OEM specific code to execute MTE test.
//
//  OEM calls RunAR6kTest(); (see wifitest.h)
//

#if 0 // remove IFDEF and implement the following platform APIs:

void A_DelayMS(int Milliseconds) 
{
          
}

#endif


DWORD A_GetSecsTick()
{
    return OEMEthGetSecs();
}
