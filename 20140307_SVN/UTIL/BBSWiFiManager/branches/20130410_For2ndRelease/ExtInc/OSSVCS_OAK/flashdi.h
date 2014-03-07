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

Copyright (c) 1993-2000 Microsoft Corporation, All rights reserved

Module Name:

	flashdi.hpp	

Abstract:

	Flash Driver Interface

Author:

	mikecal		5/28/97
    randyram    12/20/00

--*/


#pragma once

#define FILE_DEVICE_FLASH         FILE_DEVICE_UNKNOWN
#define FLASH_IOCTL_INTERNAL_INDEX       0x0000
#define FLASH_IOCTL_INDEX                0x00ff


/*++

	IOCTL_FLASH_WRITE

	Writes data to the flash.
	
	Input Buffer is a FLASH_WRITE_HEADER followed immediately by dwLength
    bytes of data.

	Output buffer is unused.

--*/

#define IOCTL_FLASH_WRITE_XIP \
			CTL_CODE(FILE_DEVICE_FLASH,  \
					FLASH_IOCTL_INDEX, \
					METHOD_BUFFERED,  \
					FILE_ANY_ACCESS)

typedef struct _FLASH_WRITE_XIP_HEADER
{
	DWORD   dwPhysAddr;
    DWORD	dwLength;
    DWORD   dwNumRecords;

} FLASH_WRITE_XIP_HEADER, *PFLASH_WRITE_XIP_HEADER;

typedef struct _FLASH_RECORD_INFO {

    LPVOID  pvPhysStart;    // physical starting address of record
    DWORD   dwPhysLen;      // physical len, in bytes of record
    DWORD   dwCheckSum;     // sum of all the bytes in the record data
    LPVOID  pvRAMStart;     // virtual address of record in RAM

}   FLASH_RECORD_INFO, *PFLASH_RECORD_INFO;



/*++

	IOCTL_FLASH_READ

	Reads data from the flash.
	
	Input Buffer is a FLASH_READ_HEADER followed immediately by dwLength
    bytes of data.

	Output buffer should be the same buffer as Input buffer.  Data read will
    be placed into the bytes after the header.

--*/

#define IOCTL_FLASH_READ \
			CTL_CODE(FILE_DEVICE_FLASH,  \
					FLASH_IOCTL_INDEX + 1, \
					METHOD_BUFFERED,  \
					FILE_ANY_ACCESS)

typedef struct _FLASH_READ_HEADER
{
	DWORD   dwPhysAddr;
    DWORD	dwLength;
} FLASH_READ_HEADER, *PFLASH_READ_HEADER;

/*++

	IOCTL_FLASH_ERASE

	Erases data from the flash.
	
	Input Buffer is a FLASH_ERASE_PARAMS 

	Output buffer is status code

--*/

#define IOCTL_FLASH_ERASE \
			CTL_CODE(FILE_DEVICE_FLASH,  \
					FLASH_IOCTL_INDEX + 2, \
					METHOD_BUFFERED,  \
					FILE_ANY_ACCESS)

typedef struct _FLASH_ERASE_PARAMS
{
	DWORD   dwPhysAddr;
    DWORD	dwLength;
} FLASH_ERASE_PARAMS, *PFLASH_ERASE_PARAMS;

/*++

	IOCTL_FLASH_WRITE

	Writes data to the flash.
	
	Input Buffer is a FLASH_WRITE_HEADER followed immediately by dwLength
    bytes of data.

	Output buffer is unused.

--*/

#define IOCTL_FLASH_WRITE \
			CTL_CODE(FILE_DEVICE_FLASH,  \
					FLASH_IOCTL_INDEX + 3, \
					METHOD_BUFFERED,  \
					FILE_ANY_ACCESS)

typedef struct _FLASH_WRITE_HEADER
{
	DWORD   dwPhysAddr;
    DWORD	dwLength;

} FLASH_WRITE_HEADER, *PFLASH_WRITE_HEADER;

