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
/*++

Copyright (c) 1999 - 2000 Microsoft Corporation

Module Name:

    fshpddmdd.h

Abstract:

    Interface and shared definitions between XIP Flash MDD and PDD

Author:

    Randy Ramig (RandyRam) 12/20/00

Revision History:

--*/

#pragma once


// These are the commands that can be sent to the EraseFlash() and WriteFlash() routines.  The EthDown()
//  call back will invoke WriteFlash() for each DWORD that is written to the memory.
typedef enum {
	FLASH_START_ERASE,	// Begin the erase process.
	FLASH_CONT_ERASE	// Continue erasing flash pages from the current position.  If all flash pages have
						//  been erased, just return.
} FlashCommands;


UINT16 FlashWrite( DWORD dwPhysStart, DWORD dwPhysLen, DWORD dwFlashCache, char **ppszErrorMsg );
UINT16 IsFlash( DWORD dwPhysStart, DWORD dwPhysLen );
UINT16 FlashErase(FlashCommands FlashCommand, DWORD dwPhysStart, DWORD dwPhysLen, char **ppszErrorMsg );
BOOL  FlashInit();

DWORD FlashPDD_Write( DWORD dwPhysStart, DWORD dwPhysLen, DWORD dwSourceAddr );

//BUGBUG might have to do that with this whole file
extern "C" void INTERRUPTS_ON(void);
extern "C" void INTERRUPTS_OFF(void);
extern "C" void WriteString(char *);
