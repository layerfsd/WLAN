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
 * 
 * Portions Copyright Microsoft Corporation 
 * 
 * SD API definitions that are implemented by the platform (OEM).
 * 
 */

#ifndef SDAPI_H_
#define SDAPI_H_


/* SD host APIs */
typedef enum  _SD_RESPONSE_TYPE {
    NoResponse  = 0,
    ResponseR1  = 1,
    ResponseR1b = 2,
    ResponseR2  = 3,
    ResponseR3  = 4, 
    ResponseR4  = 5,
    ResponseR5  = 6,
    ResponseR6  = 7,
    ResponseR7  = 8
} SD_RESPONSE_TYPE, *PSD_RESPONSE_TYPE;


#define SDCARD_RESPONSE_BUFFER_BYTES 17

typedef struct _SD_COMMAND_RESPONSE {
    SD_RESPONSE_TYPE ResponseType;
    UCHAR            ResponseBuffer[SDCARD_RESPONSE_BUFFER_BYTES];  
}SD_COMMAND_RESPONSE, *PSD_COMMAND_RESPONSE;

typedef enum { 
    SD_READ = 0,         // block read
    SD_WRITE = 1,        // block write
    SD_COMMAND = 2,      // command only
} SD_TRANSFER_CLASS;

typedef enum _SD_BUS_WIDTH {
   SD_BUS_1BIT = 0,
   SD_BUS_4BIT = 1 
} SD_BUS_WIDTH;

A_BOOL SDIO_IssueCMD53(A_BOOL   Write,
                       A_UINT32 Argument,
                       A_UINT32 BlockCount,
                       A_UINT32 BlockLength,
                       A_UINT8  *pBuffer);
                  
A_BOOL SDIO_IssueCommand(A_UINT8              Cmd,
                         A_UINT32             Argument,
                         SD_RESPONSE_TYPE     ResponseType,
                         SD_COMMAND_RESPONSE  *pResponse);

A_BOOL SDIO_SetupBus(void);

A_BOOL SDIO_SetBusRate(A_UINT32 ClockRate);
   
A_BOOL SDIO_SetBusWidth(SD_BUS_WIDTH Width); 
                                  
#endif /*SDAPI_H_*/


