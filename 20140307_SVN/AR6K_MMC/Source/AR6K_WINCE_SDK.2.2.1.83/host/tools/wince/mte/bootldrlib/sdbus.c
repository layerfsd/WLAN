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
 *  Bootloader hooks for the SDBUS apis
 * 
 * 
 * 
 */

#include <windows.h>
#include "system.h"
#include "sdapi.h"


/* TODO OEM implementation of the following SD bus apis */

#if 0 // remove IFDEF and implement the following APIs:

A_BOOL SDIO_SetupBus(void)
{
    /* set up bus controller */    
    return TRUE;    
}

A_BOOL SDIO_SetBusRate(A_UINT32 Rate)
{
    /* TODO set bus rate for all future commands */
    return TRUE;   
}

A_BOOL SDIO_SetBusWidth(SD_BUS_WIDTH Width) 
{
    /* TODO set bus width for all future data commands */
    return TRUE;
}

A_BOOL SDIO_IssueCommand(A_UINT8              Cmd,
                         A_UINT32             Argument,
                         SD_RESPONSE_TYPE     ResponseType,
                         SD_COMMAND_RESPONSE  *pResponse)
{
    
    /* TODO issue simple SD command */
    return FALSE;
}

A_BOOL SDIO_IssueCMD53(A_BOOL   Write,
                       A_UINT32 Argument,
                       A_UINT32 BlockCount,
                       A_UINT32 BlockLength,
                       A_UINT8  *pBuffer)
{ 
    
    /* TODO issue CMD53 command */
    return FALSE;
}
#endif


