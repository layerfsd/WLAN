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
 *  WiFi MTE Unit test application
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
#include "system.h"
#include "sdapi.h"
#include "wifitest.h"
#include <sdcard.h>

#ifdef AR6K_MTE_FIRMWARE_BLOB
#include "athwlan_h.bin"
#include "datapatch_h.bin"
#endif

#define PRINT_CONSOLE(s) RETAILMSG(TRUE, s)

VOID PrintUsage() {
    PRINT_CONSOLE((TEXT("mte_test <SSID> <channel number> \r\n")));
}

/* output debug string */
VOID A_OutputDebugString(char *pDbgStr,...);

A_OUTPUT_DEBUG_FUNC g_pDebugOutputFunc = A_OutputDebugString;

HANDLE g_hDevice = INVALID_HANDLE_VALUE;
A_BOOL SDIO_ResetCard(void);

BOOL IssueBusCommand(AR6K_SDIO_CMD *pCmd, DWORD Length)
{
     if (!DeviceIoControl(g_hDevice,
                          IOCTL_CAR6K_BUS_PASSTHRU_CMD,
                          pCmd,
                          Length,
                          NULL,
                          0,
                          NULL,
                          NULL)) {
        PRINT_CONSOLE((TEXT("IOCTL_CAR6K_BUS_PASSTHRU_CMD failed. \r\n")));
        return FALSE;
    }
    return TRUE;
}

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
    return FALSE;
#endif
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
    UINT32                channel;
    CHAR                  charBuffer[34];
    MTE_WIFI_TEST_STATUS  testStatus;
    int                   i;

    memset(charBuffer,0,sizeof(charBuffer));

    do {

        if (argc != 3) {
            PrintUsage();
            break;
        }

        channel = _ttoi(argv[2]);

        if (!((channel > 0) && (channel <= MAX_WIFI_CHANNELS))) {
            PRINT_CONSOLE((TEXT("Invalid Channel number (should be 1-14) \r\n")));
            PrintUsage();
            break;
        }


        if (_tcslen(argv[1]) > 32) {
            PRINT_CONSOLE((TEXT("Invalid SSID (%s), no more than 32 characters allowed \r\n"),
                argv[1]));
            PrintUsage();
            break;
        }

            /* convert unicode to ASCII */
        for (i = 0; i < (int)_tcslen(argv[1]) ; i++) {
            charBuffer[i] = (CHAR)(argv[1])[i];
        }

        g_hDevice = CreateFile(TEXT("DRG1:"),
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               0);

        if (g_hDevice == INVALID_HANDLE_VALUE) {
            PRINT_CONSOLE((TEXT("Failed to open DRG1: \r\n")));
            break;
        }

            /* issue a a full reset to the target because the test handler will re-initialize the
             * SDIO card and re-download firmware */
        if (!SDIO_ResetCard()) {
            PRINT_CONSOLE((TEXT("Full reset failed. \r\n")));
            break;
        }

        if (!RunAR6kTest(channel,
                         charBuffer,
                         &testStatus)) {
            PRINT_CONSOLE((TEXT("RunAR6kTest failed \r\n")));
            break;
        }

        PRINT_CONSOLE((TEXT("RunAR6kTest Complete, MTE Status Code: %d , RSSI: %d \r\n"),
                  testStatus.StatusCode, testStatus.RSSI));

    } while (FALSE);

    if (g_hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(g_hDevice);
    }

    exit(0);
}

#define MAX_DEBUG_STRING 512

char  g_DebugBuffer[MAX_DEBUG_STRING];
WCHAR g_WideDebugBuffer[MAX_DEBUG_STRING];

void OutputToDebugger(CHAR *pDebugStr)
{
    PWCHAR pBuffer;

    pBuffer = g_WideDebugBuffer;

    while (*pDebugStr != (CHAR)0) {
        *pBuffer = (WCHAR)*pDebugStr;
        pDebugStr++;
        pBuffer++;
    }

    *pBuffer = (WCHAR)0;
    OutputDebugString(g_WideDebugBuffer);
}


    /* output debug string */
VOID A_OutputDebugString(char *pDbgStr,...)
{
    va_list       argumentList;
    int           debugChars,ii;

    va_start(argumentList, pDbgStr);

    g_DebugBuffer[MAX_DEBUG_STRING - 1] = (CHAR)0;

    do {

        debugChars = _vsnprintf(g_DebugBuffer, (MAX_DEBUG_STRING - 1), pDbgStr, argumentList);

        if (debugChars < 0) {
            RETAILMSG(TRUE,
                   (TEXT(" Debug string TOO LONG!!! \r\n")));
            break;
        }

        if (debugChars > 2) {
            for (ii = (debugChars - 3); ii < debugChars; ii++) {
                    /* replace the last line feed with carriage return + line feed */
                if ('\n' == g_DebugBuffer[ii]) {
                    if (debugChars < (MAX_DEBUG_STRING - 3)) {
                        g_DebugBuffer[ii] = '\r';
                        g_DebugBuffer[ii+1] = '\n';
                        g_DebugBuffer[ii+2] = (CHAR)0;
                        break;
                    }
                }
            }
        }

        OutputToDebugger(g_DebugBuffer);

    } while (FALSE);

    va_end(argumentList);
}

void A_DelayMS(int Milliseconds)
{
    Sleep(Milliseconds);
}

A_BOOL SDIO_SetupBus(void)
{
    return TRUE;
}

    /* intermediate data buffer for all bus requests */
UCHAR           g_SDIOCmdBuffer[sizeof(AR6K_SDIO_CMD) + 2048];
AR6K_SDIO_CMD  *g_pSDIOCmdBuffer = (AR6K_SDIO_CMD *)g_SDIOCmdBuffer;


A_BOOL SDIO_ResetCard(void)
{
    memset(g_pSDIOCmdBuffer, 0, sizeof(*g_pSDIOCmdBuffer));
    g_pSDIOCmdBuffer->CommandType = AR6K_CMD_TYPE_FULL_RESET;

    if (!IssueBusCommand(g_pSDIOCmdBuffer,
                         sizeof(*g_pSDIOCmdBuffer))){
        DBG_LOG_PRINT(DBG_ZONE_ERR,("Reset Card FAILED \r\n"));
        return FALSE;
    }

    return TRUE;
}

A_BOOL SDIO_SetBusRate(A_UINT32 dwRate)
{
    memset(g_pSDIOCmdBuffer, 0, sizeof(*g_pSDIOCmdBuffer));
    g_pSDIOCmdBuffer->CommandType = AR6K_CMD_TYPE_CLK_CONFIG;
    g_pSDIOCmdBuffer->Request.AsClockRequest.ClockRate = dwRate;

    if (!IssueBusCommand(g_pSDIOCmdBuffer,
                         sizeof(*g_pSDIOCmdBuffer))){
        DBG_LOG_PRINT(DBG_ZONE_ERR,("SetRate2 FAILED \r\n"));
        return FALSE;
    }

    return TRUE;
}

void FillCommand(A_UCHAR              Command,
                 A_UINT32             Argument,
                 SD_RESPONSE_TYPE     ResponseType)
{
    DWORD  responseType = SDIO_CMD_RESPONSE_R1;

    memset(g_pSDIOCmdBuffer, 0, sizeof(*g_pSDIOCmdBuffer));
    g_pSDIOCmdBuffer->CommandType = AR6K_CMD_TYPE_BUS;
    g_pSDIOCmdBuffer->Request.AsBusRequest.Command = Command;
    g_pSDIOCmdBuffer->Request.AsBusRequest.Argument = Argument;

    switch (ResponseType) {
         case NoResponse:
            responseType = SDIO_CMD_RESPONSE_NONE;
            break;
         case ResponseR1:
            responseType = SDIO_CMD_RESPONSE_R1;
            break;
         case ResponseR1b:
            responseType = SDIO_CMD_RESPONSE_R1b;
            break;
        case ResponseR4:
            responseType = SDIO_CMD_RESPONSE_R4;
            break;
        case ResponseR5:
            responseType = SDIO_CMD_RESPONSE_R5;
            break;
        case ResponseR6:
            responseType = SDIO_CMD_RESPONSE_R6;
            break;
        default:
            break;
    }

    g_pSDIOCmdBuffer->Request.AsBusRequest.ResponseType = responseType;

}

A_BOOL SDIO_IssueCMD53(A_BOOL   Write,
                       A_UINT32 Argument,
                       A_UINT32 BlockCount,
                       A_UINT32 BlockLength,
                       A_UINT8  *pBuffer)
{

    FillCommand(53,Argument,ResponseR5);
    g_pSDIOCmdBuffer->Request.AsBusRequest.DataTransfer = TRUE;
    g_pSDIOCmdBuffer->Request.AsBusRequest.DataWrite = (Write) ? TRUE : FALSE;
    g_pSDIOCmdBuffer->Request.AsBusRequest.DataBlocks = BlockCount;
    g_pSDIOCmdBuffer->Request.AsBusRequest.DataBlockLength = BlockLength;

    if (Write) {
        memcpy(&g_pSDIOCmdBuffer->Request.AsBusRequest.Buffer[0],
               pBuffer,
               BlockCount * BlockLength);
    }

    if (!IssueBusCommand(g_pSDIOCmdBuffer,
                         sizeof(*g_pSDIOCmdBuffer) +
                        (g_pSDIOCmdBuffer->Request.AsBusRequest.DataBlocks *
                         g_pSDIOCmdBuffer->Request.AsBusRequest.DataBlockLength))) {
        return FALSE;
    }

    if (!Write) {
            /* copy back on reads */
        memcpy(pBuffer,
               &g_pSDIOCmdBuffer->Request.AsBusRequest.Buffer[0],
               BlockCount * BlockLength);
    }

    return TRUE;
}

A_BOOL SDIO_IssueCommand(A_UINT8              Cmd,
                         A_UINT32             Argument,
                         SD_RESPONSE_TYPE     ResponseType,
                         SD_COMMAND_RESPONSE  *pResponse)
{
    FillCommand(Cmd,Argument,ResponseType);

    if (!IssueBusCommand(g_pSDIOCmdBuffer, sizeof(*g_pSDIOCmdBuffer))) {
        return FALSE;
    }

    if (pResponse != NULL) {
             /* copy response */
        memcpy(pResponse->ResponseBuffer,
               g_pSDIOCmdBuffer->Request.AsBusRequest.ResponseBuffer,
               SDCARD_RESPONSE_BUFFER_BYTES);
    }

    return TRUE;
}


A_BOOL  SDIO_SetBusWidth(SD_BUS_WIDTH Width)
{
    memset(g_pSDIOCmdBuffer, 0, sizeof(*g_pSDIOCmdBuffer));

    g_pSDIOCmdBuffer->CommandType = AR6K_CMD_TYPE_BUS_WIDTH_CONFIG;

    if (Width == SD_BUS_4BIT) {
        g_pSDIOCmdBuffer->Request.AsBusWidthRequest.BusWidth = 4;
    } else {
        g_pSDIOCmdBuffer->Request.AsBusWidthRequest.BusWidth = 1;
    }

    if (!IssueBusCommand(g_pSDIOCmdBuffer,
                         sizeof(*g_pSDIOCmdBuffer))){
        DBG_LOG_PRINT(DBG_ZONE_ERR,("SDSetBusWidth2 FAILED \r\n"));
        return FALSE;
    }

    return TRUE;
}

DWORD g_StartTick;
BOOL  g_StartTickCaptured = FALSE;

DWORD A_GetSecsTick()
{
    DWORD tick = GetTickCount();

    if (!g_StartTickCaptured) {
        g_StartTickCaptured = TRUE;
        g_StartTick = tick;
    }

    return (tick - g_StartTick) / 1000;
}


