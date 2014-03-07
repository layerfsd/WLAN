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

Copyright (c) Microsoft Corporation

Module Name:

    xipflash.h

Abstract:

    Flash interface that the OAL must expose
    for Microsoft Mobile Devices

--*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// **************************************************************************
//
// Function Name: FlashInit
// 
// Purpose: initializes or deinitializes resources for flash routines
//
// Arguments:
//    IN    bInit   TRUE- initialize, FALSE- deinitialize
//
// Return Values:
//
//      ERROR_SUCCESS
//      ERROR_OUTOFMEMORY   can't allocate memory for flash functions
//      
// Description:  
//    
//  Initialization required to write to flash.
//
//  For example, you may have routines that need to be relocated to RAM
//  in order to write the flash device.
//
//  init is platform dependent.
//

DWORD
WINAPI
FlashInit(
            BOOL    bInit
         );


// **************************************************************************
//
// Function Name: FlashWrite
// 
// Purpose: writes data to flash memory
//
// Arguments:
//    IN    pvDestAddr      dest address in flash
//    IN    dwLength        amount of data to write
//    IN    pvSrcAddr       src address of data
//
// Return Values:
//    
//    ERROR_SUCCESS
//    ERROR_NOT_READY   FlashInit(TRUE) wasn't called
//    ERROR_INVALID_PARAMETER   data not aligned or bad pointer
//    ERROR_INVALID_DATA   data verification check failed
//    non-zero error from flash device, indicates failure XX
//
//

DWORD
WINAPI
FlashWrite(
            LPVOID  pvDestAddr,
            LPVOID  pvSrcAddr,
            DWORD   dwLength
          );


// **************************************************************************
//
// Function Name: FlashRead
// 
// Purpose: reads data from flash memory
//
// Arguments:
//    IN    pvDestAddr      dest address of data
//    IN    dwLength        amount of data to write
//    IN    pvSrcAddr       src address in flash
//
// Return Values:
//    
//    ERROR_SUCCESS
//    ERROR_NOT_READY   FlashInit(TRUE) wasn't called
//    ERROR_INVALID_PARAMETER   bad pointer
//    ERROR_INVALID_DATA   data verification check failed
//    non-zero error from flash device, indicates failure XX
//
//

DWORD
WINAPI
FlashRead(
            LPVOID  pvDestAddr,
            LPVOID  pvSrcAddr,
            DWORD   dwLength
          );



// **************************************************************************
//
// Function Name: FlashErase
// 
// Purpose: erases blocks of flash memory
//
// Arguments:
//    IN    pvStartAddr     start address to erase
//    IN    dwLength        length of region to erase
//
// Return Values:
//    
//    ERROR_SUCCESS
//    ERROR_NOT_READY   FlashInit(TRUE) was not called
//
// Description:
//
//  Erases flash blocks containing the specified region
//
//

DWORD
WINAPI
FlashErase(
            LPVOID  pvStartAddr,
            DWORD   dwLength
          );



// **************************************************************************
//
// Function Name: FlashGetBlockInfo
// 
// Purpose: returns block address and length for a specified address
//
// Arguments:
//    IN    pvStartAddr     address to get block info for
//    OUT   dwBlockStart    start address of block the address is in
//    OUT   dwBlockLen      length of block the address is in
//
// Return Values:
//    
//    ERROR_SUCCESS
//    ERROR_NOT_READY   can't get flash params
//
//

DWORD
WINAPI
FlashGetBlockInfo(
                    LPVOID  pvStartAddr, 
                    DWORD * pdwBlockStart,
                    DWORD * pdwBlockLen
                 ) ;

#ifdef __cplusplus
}
#endif
