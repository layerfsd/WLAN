/*
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
 */
 
 
/*
 *
 *  Windows CE Endpoint Ping command line test
 * 
 * 
 * 
 */
 
#include <windows.h>
#include <windef.h>
#include <stdio.h>
#include <tchar.h>
#include <winioctl.h>
#include <a_drv.h>

#define PRINT_CONSOLE(s) RETAILMSG(TRUE, s) 
    
VOID PrintUsage() {
    PRINT_CONSOLE((TEXT("epping: \r\n")));
}

/**+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@function: _tmain() - test application main.

@input: argc - number of command line arguments (including application name)
@input: argv - command line argument strings

@return: application exit code

@notes:                         
                    
                    
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
VOID _tmain(int argc, TCHAR * argv[])
{
    HANDLE hDevice = INVALID_HANDLE_VALUE;
        
    do {
   
        hDevice = CreateFile(TEXT("DRG1:"),
                             GENERIC_READ | GENERIC_WRITE,
                             FILE_SHARE_READ | FILE_SHARE_WRITE,
                             NULL,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             0);
                             
        if (hDevice == INVALID_HANDLE_VALUE) {
            PRINT_CONSOLE((TEXT("Failed to open DRG1: \r\n")));
            break;    
        }
   
        if (!DeviceIoControl(hDevice,
                             IOCTL_CAR6K_EP_PINGTEST,
                             NULL,
                             0,
                             NULL,
                             0,
                             NULL,
                             NULL)) {
            PRINT_CONSOLE((TEXT("IOCTL_CAR6K_EP_PINGTEST failed. \r\n")));                    
        }
                        
            
    } while (FALSE);
       
    if (hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(hDevice);    
    } 
    
    exit(0);
}

