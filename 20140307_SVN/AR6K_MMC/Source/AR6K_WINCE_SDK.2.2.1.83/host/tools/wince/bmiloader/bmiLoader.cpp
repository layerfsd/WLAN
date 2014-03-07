//------------------------------------------------------------------------------
// <copyright file="bmiLoader.cpp" company="Atheros">
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
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
#include "stdafx.h"
#include <winioctl.h>
#include <Ntddndis.h>
#include <Nuiouser.h>


//#include <AR6000_bmi.h>
#include <bmi.h>
#include <a_drv.h>

#define PARAM_FLAG 0x1
#define FILE_FLAG 0x2

static HANDLE hAdapter;

#ifdef ATH_APPS_CONSOLE_OUTPUT
#define PRINT_CONSOLE(s) _tprintf s
#else
#define PRINT_CONSOLE(s) RETAILMSG(TRUE, s)
#endif

#define SLEEP_CMD               38
#define DN_BINARY_CMD           44

void
PrintUsage(void)
{
    PRINT_CONSOLE((_T("Usage: bmiLoader [-i device] commands\r\n")));
    PRINT_CONSOLE((_T("\r\n")));
    PRINT_CONSOLE((_T("Commands:\r\n")));
    PRINT_CONSOLE((_T("    -g -a <register address>\r\n")));
    PRINT_CONSOLE((_T("    -s -a <register address> -p <register value>\r\n")));
    PRINT_CONSOLE((_T("    -r -a <target address> -l <bytes> -f <filename>\r\n")));
    PRINT_CONSOLE((_T("    -w -a <target address> [-f <filename> | -p <value>]\r\n")));
    PRINT_CONSOLE((_T("    -e -a <function start address> -p <input-param>\r\n")));
    PRINT_CONSOLE((_T("    -b -a <function start address>\r\n")));
    PRINT_CONSOLE((_T("    -d \r\n")));
    PRINT_CONSOLE((_T("\r\n")));
}

int processCmd(LPTSTR    lpCmdLine)
{
    DWORD dwReturnedBytes;
    PUCHAR buffer = NULL;
    LPWSTR temp=NULL;
    wchar_t devName[128]=TEXT("AR6K1");
    unsigned int argcount=0;
    unsigned int cmd =0,flag=0;
    unsigned int address = 0, length = 0, param = 0;
    unsigned int Andmask = 0xdeadbeef, Ormask = 0xdeadbeef;
    wchar_t fileName[128];
    wchar_t filePath[128];
    unsigned int fileSize=0;

    memset(filePath,0,sizeof(filePath));

    FILE *fd = NULL;
    BMI_READ_SOC_REG_PARAM readSoc = {0};
    BMI_READ_SOC_REG_PARAM *preadSoc = &readSoc;

    BMI_WRITE_SOC_REG_PARAM writeSoc = {0};
    BMI_WRITE_SOC_REG_PARAM *pwriteSoc = &writeSoc;

    BMI_EXECUTE_PARAM bmiExecParam = {0};
    BMI_EXECUTE_PARAM *pbmiExecParam = &bmiExecParam;

    BMI_SET_APP_START_PARAM appSetParam     = {0};
    BMI_SET_APP_START_PARAM *pappSetParam   = &appSetParam;

    temp = wcstok(lpCmdLine, TEXT(" \r\n"));
    while (temp) {
        argcount++;
        if ( !wcscmp(temp, TEXT("-i")) ) {

            temp = wcstok(NULL, TEXT(" \r\n"));
            if ( !temp ) {

                PrintUsage();
                return 0;
            }

            wcscpy(devName, temp);

        } else if ( !wcscmp(temp, TEXT("-g")) ) {

            cmd = BMI_READ_SOC_REGISTER;

            temp = wcstok(NULL, TEXT(" \r\n"));
            if ( !temp ) {
                PrintUsage();
                return 0;
            }

            if ( !wcscmp(temp, TEXT("-a")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if ( !temp ) {
                    PrintUsage();
                    return 0;
                }
                address = wcstoul(temp, NULL, 0);
            } else {
                PrintUsage();
                return 0;
            }
        } else if ( !wcscmp(temp, TEXT("-s")) ) {

            cmd = BMI_WRITE_SOC_REGISTER;

            temp = wcstok(NULL, TEXT(" \r\n"));

            if (!temp) {
                PrintUsage();
                return 0;
            }

            if ( !wcscmp(temp, TEXT("-a")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                address = wcstoul(temp, NULL, 0);
            } else {
                PrintUsage();
                return 0;
            }

            temp = wcstok(NULL, TEXT(" \r\n"));

            if ( !wcscmp(temp, TEXT("-p")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                param = wcstoul(temp, NULL, 0);
            } else if ( !wcscmp(temp, TEXT("-A")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                Andmask = wcstoul(temp, NULL, 0);
            } else if ( !wcscmp(temp, TEXT("-O")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                Ormask = wcstoul(temp, NULL, 0);
            } else {
                PrintUsage();
                return 0;
            }
        } else if ( !wcscmp(temp, TEXT("-r")) ) {
            cmd = BMI_READ_MEMORY;

            temp = wcstok(NULL, TEXT(" \r\n"));

            if (!temp) {
                PrintUsage();
                return 0;
            }

            if ( !wcscmp(temp, TEXT("-a")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                address = wcstoul(temp, NULL, 0);
            } else {
                PrintUsage();
                return 0;
            }

            temp = wcstok(NULL, TEXT(" \r\n"));

            if ( !wcscmp(temp, TEXT("-l")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                length = wcstoul(temp, NULL, 0);
            } else {
                PrintUsage();
                return 0;
            }
            temp = wcstok(NULL, TEXT(" \r\n"));

            if ( !wcscmp(temp, TEXT("-f")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                wcscpy(fileName, temp);
            } else {
                PrintUsage();
                return 0;
            }
        } else if ( !wcscmp(temp, TEXT("-w")) ) {

            cmd = BMI_WRITE_MEMORY;

            temp = wcstok(NULL, TEXT(" \r\n"));

            if (!temp) {
                PrintUsage();
                return 0;
            }

            if ( !wcscmp(temp, TEXT("-a")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                address = wcstoul(temp, NULL, 0);
            } else {
                PrintUsage();
                return 0;
            }
            temp = wcstok(NULL, TEXT(" \r\n"));

            if ( !wcscmp(temp, TEXT("-f")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                wcscpy(fileName, temp);
                flag = FILE_FLAG;
            } else if (!wcscmp(temp, TEXT("-p")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                param = wcstoul(temp, NULL, 0);
                flag = PARAM_FLAG;
            } else {
                PrintUsage();
                return 0;
            }

        } else if ( !wcscmp(temp, TEXT("-e")) ) {

            cmd = BMI_EXECUTE;

            temp = wcstok(NULL, TEXT(" \r\n"));

            if (!temp) {
                PrintUsage();
                return 0;
            }

            if ( !wcscmp(temp, TEXT("-a")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                address = wcstoul(temp, NULL, 0);
            } else {
                PrintUsage();
                return 0;
            }

            temp = wcstok(NULL, TEXT(" \r\n"));

            if ( !wcscmp(temp, TEXT("-p")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if (!temp) {
                    PrintUsage();
                    return 0;
                }
                param = wcstoul(temp, NULL, 0);
            } else {
                PrintUsage();
                return 0;
            }

        } else if ( !wcscmp(temp, TEXT("-b")) ) {

            cmd = BMI_SET_APP_START;

            temp = wcstok(NULL, TEXT(" \r\n"));
            if ( !temp ) {
                PrintUsage();
                return 0;
            }

            if ( !wcscmp(temp, TEXT("-a")) ) {
                temp = wcstok(NULL, TEXT(" \r\n"));

                if ( !temp ) {
                    PrintUsage();
                    return 0;
                }
                address = wcstoul(temp, NULL, 0);
            } else {
                PrintUsage();
                return 0;
            }

        } else if ( !wcscmp(temp, TEXT("-d")) ) {

            cmd = BMI_DONE;
        } else {
            PrintUsage();
            return 0;
        }

    temp = wcstok(NULL, TEXT(" \r\n"));

    }

    if (!argcount) {
        PrintUsage();
        return 0;
    }

    // Check DRG is available, If so, go for it to access BMI stuff, since it may be a OLCA type
    // of target
    //
    hAdapter = CreateFile(TEXT("DRG1:"),GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                INVALID_HANDLE_VALUE);

    if (NULL == hAdapter)
    {
        PRINT_CONSOLE((_T ("No adapter is available\n")));
        exit (-1);
    }

    PRINT_CONSOLE((_T("DRG Handle: %x\n"),hAdapter));

    switch(cmd) {

    case BMI_DONE:

        //
        // Send this IOCTL to DRG Handle
        //
        if ( !DeviceIoControl(
                hAdapter,
                IOCTL_CAR6K_BMI_DONE,
                NULL,
                0,
                NULL,
                0,
                &dwReturnedBytes,
                NULL) )
        {
            PRINT_CONSOLE((_T("BMI_DONE failed, errorcode: %d\n"),GetLastError()));
            exit(-1);
        }
        break;

    case BMI_READ_MEMORY:


        //PRINT_CONSOLE((_T("DRG::BMI Read Memory\n")));
        BMI_READ_MEMORY_PARAM *pbmiReadMemory;

        //
        // Send this IOCTL to DRG Handle
        //
        dwReturnedBytes = 0;
        buffer = (PUCHAR) malloc(sizeof(BMI_READ_MEMORY_PARAM) + length);

        if (buffer == NULL) {
            exit(-1);
        }

        memset(buffer, 0, sizeof(BMI_READ_MEMORY_PARAM));

        pbmiReadMemory = (BMI_READ_MEMORY_PARAM *)buffer;
        pbmiReadMemory->address = address;
        pbmiReadMemory->length  = length;

        if ( !DeviceIoControl(
                hAdapter,
                IOCTL_CAR6K_BMI_READ_MEMORY,
                pbmiReadMemory,
                length + sizeof (BMI_READ_MEMORY_PARAM) - 1,
                pbmiReadMemory,
                length + sizeof (BMI_READ_MEMORY_PARAM) - 1,
                &dwReturnedBytes,
                NULL) )
        {
            PRINT_CONSOLE((_T("BMI_READ_MEMORY failed, errorcode: %d\n"),GetLastError()));
            exit(-1);
        }

        wcscat(filePath, fileName);

        PRINT_CONSOLE((_T("%s\n"), filePath));
        // Open the file & write the buffer to it.
        if ( !(fd = _wfopen(filePath,TEXT("wb"))) ) {
            PRINT_CONSOLE((_T("Could not open file %s\n"),fileName));
        } else {
            fwrite(pbmiReadMemory->databuffer, 1, length, fd);
            fclose(fd);
            PRINT_CONSOLE((_T("Successfully ended\n")));
        }

        break;

    case BMI_WRITE_MEMORY:

        if (flag == FILE_FLAG) {

            PRINT_CONSOLE((_T("BMI Write Memory (address: 0x%x, filename: %s)\n"), \
                    address, fileName));

        } else if (flag == PARAM_FLAG) {

            PRINT_CONSOLE((_T("BMI Write Memory (address: 0x%x, value: 0x%x)\n"), \
                    address, param));
            dwReturnedBytes = 0;

            BMI_WRITE_MEMORY_PARAM *pbmiWriteMemCommand;

            buffer = (PUCHAR)malloc(8 + sizeof(BMI_WRITE_MEMORY_PARAM));

            if (buffer == NULL) {
                exit (-1);
            }

            pbmiWriteMemCommand = (BMI_WRITE_MEMORY_PARAM *)buffer;
            pbmiWriteMemCommand->address = address;
            pbmiWriteMemCommand->length  = sizeof(param);
            memcpy(&pbmiWriteMemCommand->databuffer[0], &param, sizeof(param));

            if ( !DeviceIoControl(
                hAdapter,
                IOCTL_CAR6K_BMI_WRITE_MEMORY,
                pbmiWriteMemCommand,
                sizeof (BMI_WRITE_MEMORY_PARAM),
                pbmiWriteMemCommand,
                sizeof (BMI_WRITE_MEMORY_PARAM),
                &dwReturnedBytes,
                NULL) )
            {
                PRINT_CONSOLE((_T("BMI_WRITE_MEMORY failed, errorcode: %d\n"),GetLastError()));
                free(buffer);
                exit(-1);
            }
            free(buffer);
            buffer = NULL;
            goto end;
        }

        wcscat(filePath, fileName);

        PRINT_CONSOLE((_T("FilePath: %s\n"),filePath));

        //Determine the length of the file
        if ( !(fd = _wfopen(filePath,TEXT("rb"))) ) {
            PRINT_CONSOLE((_T("Could not open file %s\n"),filePath));
            break;
        }

        if (fseek(fd,0,SEEK_END)) {
            PRINT_CONSOLE((_T("Could not seek to end of file %s\n"),filePath));
            break;
        }

        if ((fileSize = ftell(fd)) < 0) {
            PRINT_CONSOLE((_T("Could not get curr pos of file ptr %s\n"),filePath));
            break;
        }

        fclose(fd);

        PRINT_CONSOLE((_T("FileSize: %d\n"),fileSize));

        //
        // Send this IOCTL to DRG Handle
        //

        dwReturnedBytes = 0;

        BMI_WRITE_MEMORY_PARAM *pbmiWriteMemCommand;

        buffer = (PUCHAR)malloc(MAX_BUF + sizeof(BMI_WRITE_MEMORY_PARAM));

        if (buffer == NULL) {
            exit (-1);
        }

        pbmiWriteMemCommand = (BMI_WRITE_MEMORY_PARAM *)buffer;
        pbmiWriteMemCommand->address = address;
        pbmiWriteMemCommand->length  = fileSize;
        // Open the file & initialize the buffer with the contents of the file
        if ( !(fd = _wfopen(filePath,TEXT("rb"))) ) {
            PRINT_CONSOLE((_T("Could not open file %s\n"),filePath));
            break;
        } else {
            DWORD file_left = fileSize;
            while(file_left)
            {
                size_t nSize = fread(pbmiWriteMemCommand->databuffer, 1, (file_left<(MAX_BUF))?file_left:MAX_BUF, fd);
                pbmiWriteMemCommand->length  = (file_left<(MAX_BUF))?file_left:MAX_BUF;
                pbmiWriteMemCommand->address = address + fileSize - file_left;
        if ( !DeviceIoControl(
                hAdapter,
                IOCTL_CAR6K_BMI_WRITE_MEMORY,
                pbmiWriteMemCommand,
                sizeof (BMI_WRITE_MEMORY_PARAM),
                pbmiWriteMemCommand,
                sizeof (BMI_WRITE_MEMORY_PARAM),
                &dwReturnedBytes,
                NULL) )
        {
            PRINT_CONSOLE((_T("BMI_WRITE_MEMORY failed, errorcode: %d\n"),GetLastError()));
            free(buffer);
            exit(-1);
        }
                if(file_left >= MAX_BUF)
                    file_left = file_left - MAX_BUF;
                else
                    file_left=0;
            }
            fclose(fd);
            free(buffer);
            buffer = NULL;
        }
end:
        PRINT_CONSOLE((_T("Completed Write Mem\n")));
        break;

    case BMI_READ_SOC_REGISTER:

        //
        // Send this IOCTL to DRG Handle
        //
        dwReturnedBytes = 0;
        buffer = NULL;
        preadSoc->address = address;

        if ( !DeviceIoControl(
                hAdapter,
                IOCTL_CAR6K_BMI_READ_SOC_REGISTER,
                preadSoc,
                sizeof (BMI_READ_SOC_REG_PARAM),
                preadSoc,
                sizeof (BMI_READ_SOC_REG_PARAM),
                &dwReturnedBytes,
                NULL) )
        {
            PRINT_CONSOLE((_T("BMI_READ_MEMORY failed, errorcode: %d\n"),GetLastError()));
            exit(-1);
        }

        PRINT_CONSOLE((_T("Return Value from target: 0x%x\n"), preadSoc->value));

        break;

    case BMI_WRITE_SOC_REGISTER:

        PRINT_CONSOLE((_T("BMI Write SOC Register (address: 0x%x, value:0x%x)\n"), \
            address, param));

        if (Andmask != 0xdeadbeef)
        {
            //
            // Send this IOCTL to DRG Handle
            //
            dwReturnedBytes = 0;
            buffer = NULL;
            preadSoc->address = address;

            if ( !DeviceIoControl(
                    hAdapter,
                    IOCTL_CAR6K_BMI_READ_SOC_REGISTER,
                    preadSoc,
                    sizeof (BMI_READ_SOC_REG_PARAM),
                    preadSoc,
                    sizeof (BMI_READ_SOC_REG_PARAM),
                    &dwReturnedBytes,
                    NULL) )
            {
                PRINT_CONSOLE((_T("BMI_READ_MEMORY failed, errorcode: %d\n"),GetLastError()));
                exit(-1);
            }

            //PRINT_CONSOLE((_T("Return Value from target: 0x%x\n"), preadSoc->value));

            param = preadSoc->value & Andmask;

            //PRINT_CONSOLE((_T("Andmask %x\n"),Andmask));
            //PRINT_CONSOLE((_T("New value %x\n"),param));
        }
        else if (Ormask != 0xdeadbeef)
        {
            //
            // Send this IOCTL to DRG Handle
            //
            dwReturnedBytes = 0;
            buffer = NULL;
            preadSoc->address = address;

            if ( !DeviceIoControl(
                    hAdapter,
                    IOCTL_CAR6K_BMI_READ_SOC_REGISTER,
                    preadSoc,
                    sizeof (BMI_READ_SOC_REG_PARAM),
                    preadSoc,
                    sizeof (BMI_READ_SOC_REG_PARAM),
                    &dwReturnedBytes,
                    NULL) )
            {
                PRINT_CONSOLE((_T("BMI_READ_MEMORY failed, errorcode: %d\n"),GetLastError()));
                exit(-1);
            }

            //PRINT_CONSOLE((_T("Return Value from target: 0x%x\n"), preadSoc->value));

            param = preadSoc->value | Ormask;

            //PRINT_CONSOLE((_T("Ormask %x\n"),Ormask));
            //PRINT_CONSOLE((_T("New value %x\n"),param));
        }

        //
        // Send this IOCTL to DRG Handle
        //
        dwReturnedBytes = 0;
        buffer = NULL;

        pwriteSoc->address = address;
        pwriteSoc->param = param;

        if ( !DeviceIoControl(
                hAdapter,
                IOCTL_CAR6K_BMI_WRITE_SOC_REGISTER,
                pwriteSoc,
                sizeof (BMI_WRITE_SOC_REG_PARAM),
                pwriteSoc,
                sizeof (BMI_WRITE_SOC_REG_PARAM),
                &dwReturnedBytes,
                NULL) )
        {
            PRINT_CONSOLE((_T("BMI_READ_MEMORY failed, errorcode: %d\n"),GetLastError()));
            exit(-1);
        }
        break;

    case BMI_EXECUTE:

        //
        // Send this IOCTL to DRG Handle
        //
        dwReturnedBytes = 0;
        buffer = NULL;

        if (buffer)
        {
            //PRINT_CONSOLE((_T ("DRG::buffer successfully allocated\n")));
            pbmiExecParam->address    = address;
            pbmiExecParam->param      = param;
        }
        else
        {
            PRINT_CONSOLE((_T ("DRG::buffer couldn't be allocated\n")));
            exit (-1);
        }

        if ( !DeviceIoControl(
                hAdapter,
                IOCTL_CAR6K_BMI_EXECUTE,
                pbmiExecParam,
                sizeof (BMI_EXECUTE_PARAM),
                pbmiExecParam,
                sizeof (BMI_EXECUTE_PARAM),
                &dwReturnedBytes,
                NULL) )
        {
            PRINT_CONSOLE((_T("BMI_READ_MEMORY failed, errorcode: %d\n"),GetLastError()));
            exit(-1);
        }

        break;

    case BMI_SET_APP_START:

        PRINT_CONSOLE((_T("BMI Set App Start (address: 0x%x)\n"), address));

        //
        // Send this IOCTL to DRG Handle
        //
        dwReturnedBytes = 0;
        buffer = NULL;

        pappSetParam->address  = address;

        if ( !DeviceIoControl(
                hAdapter,
                IOCTL_CAR6K_BMI_SET_APP_START,
                pappSetParam,
                sizeof (BMI_SET_APP_START_PARAM),
                pappSetParam,
                sizeof (BMI_SET_APP_START_PARAM),
                &dwReturnedBytes,
                NULL) )
        {
            PRINT_CONSOLE((_T("BMI_READ_MEMORY failed, errorcode: %d\n"),GetLastError()));
            exit(-1);
        }


        break;

    default:
        PrintUsage();
        exit(-1);
    } //switch

    if (buffer)
        free(buffer);

    if (hAdapter)
    {
        CloseHandle (hAdapter);
        hAdapter = NULL;
    }

    return 0;
}

int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPTSTR    lpCmdLine,
                    int       nCmdShow)
{
    BOOL bIstestcmd = FALSE;

    if (wcsstr(lpCmdLine, TEXT("-tcmd")))
    {
        bIstestcmd = TRUE;
    }

    processCmd (lpCmdLine);

    return 0;
}
