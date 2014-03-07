//------------------------------------------------------------------------------
// <copyright file="stream_drv.c" company="Atheros">
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
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
// Driver entry points for Atheros SDIO based Wifi Card driver
//
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include <winioctl.h>
#include <types.h>
#include <ndis.h>

#include "hif.h"
#include "htc_api.h"
#include "bmi.h"
#include "athdrv_wince.h"
#include "host_version.h"
#include "htc_raw_drv.h"
#include "common_drv.h"
#include "wince_common_drv.h"
#include "stream_drv.h"

struct {
    HTC_HANDLE      htcTarget;
    HIF_DEVICE      *hifDevice;
    A_UINT32        targetType;
    A_UINT32        targetId;
} stream_drv_t;
static BOOL     bHTCRawOpened       = FALSE;
BOOL     bIsNdisRegistered   = FALSE;
#ifdef WINCE_ART
static BOOL     bIsARTStarted       = FALSE;
#endif

DWORD
NdisRegisterAdapterThread(PVOID Context)
{
    NDIS_STATUS   ndisStatus = NDIS_STATUS_SUCCESS;

    NdisRegisterAdapter(&ndisStatus, AR6K_ADAPTER_MINIPORT , AR6K_ADAPTER_INSTANCE);
    if (NDIS_STATUS_SUCCESS == ndisStatus)
    {
        bIsNdisRegistered = TRUE;
    }

    return ndisStatus;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Notes:  PLEASE DO NOT REMOVE THE FOLLOWING FUNCTIONS. THOUGH THEY WILL NEVER
//          BE USED, STREAMS BASED DRIVER MUST EXPORT THESE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
extern
BOOL WINAPI DRG_IOControl(
    DWORD   Handle,
    DWORD   IoctlCode,
    PBYTE   pInBuf,
    DWORD   InBufSize,
    PBYTE   pOutBuf,
    DWORD   OutBufSize,
    PDWORD  pBytesReturned
)
{
    A_STATUS status = A_OK;
    HIF_DEVICE *hifDevice;
    HTC_HANDLE htcTarget;

    hifDevice = stream_drv_t.hifDevice;
    htcTarget = stream_drv_t.htcTarget;

    switch (IoctlCode)
    {
        case IOCTL_CAR6K_GET_FIRMWARE_VERSION:
            {
                DWORD   dwFirmwareVersion = 0;
                dwFirmwareVersion = stream_drv_t.targetId;
                *(DWORD *)pOutBuf = dwFirmwareVersion;
                *pBytesReturned      = sizeof (DWORD);
                status = A_OK;
            }
            break;
            
        case IOCTL_CAR6K_GET_HOST_VERSION:
            {
                DWORD   dwHostVersion       = 0;
                dwHostVersion = AR6K_SW_VERSION;
                *(DWORD *)pOutBuf = dwHostVersion;
                *pBytesReturned   = sizeof (DWORD);
                status = A_OK;
            }
            break;


        case IOCTL_CAR6K_GET_TARGET_TYPE:
            {
                DWORD dwTargetType = 0;
                dwTargetType     = stream_drv_t.targetType;
                *(DWORD *)pOutBuf = dwTargetType;
                *pBytesReturned      = sizeof (DWORD);
                status = A_OK;
            }
            break;

        case IOCTL_CAR6K_NDIS_REGISTER:
            // Perform NDIS register adapter on a separate thread to avoid
            // blocking the SDIO bus driver
            {
                HANDLE  hThread = NULL;
                hThread = CreateThread (NULL, 0, NdisRegisterAdapterThread, NULL, 0, NULL);
                if (hThread)
                {
                    CeSetThreadPriority (hThread, 200);
                    CloseHandle (hThread);
                    status = A_OK;
                    bHTCRawOpened = FALSE;
                }
                else
                {
                    status = A_ERROR;
                }
            }
            break;
            
        case IOCTL_CAR6K_CONFIGURE_AR6000:
            {
                CONFIGURE_AR6000_PARAM *buffer;
                buffer = (CONFIGURE_AR6000_PARAM *)pInBuf;

                status = configure_ar6000(hifDevice, 
                               stream_drv_t.targetType,
                               stream_drv_t.targetId,
                               buffer->enableUART,
                               buffer->timerWAR,
                               buffer->clkFreq,
                               buffer->filePath,
                               buffer->fileRoot,
                               buffer->bCompressed,
                               buffer->isColdBoot,
                               buffer->eepromFile);
            }
            break;

        case IOCTL_CAR6K_BMI_DONE:
            status = BMIDone (hifDevice);
            break;

        case IOCTL_CAR6K_BMI_READ_MEMORY:
            {
                BMI_READ_MEMORY_PARAM *buffer;
                buffer = (BMI_READ_MEMORY_PARAM *)pInBuf;
                status = BMIReadMemory(hifDevice, buffer->address, buffer->databuffer, buffer->length);
            }
            break;

        case IOCTL_CAR6K_BMI_WRITE_MEMORY:
            {
                BMI_WRITE_MEMORY_PARAM *buffer;
                buffer = (BMI_WRITE_MEMORY_PARAM *)pInBuf;
                status = BMIWriteMemory (hifDevice, buffer->address, buffer->databuffer, buffer->length);
            }
            break;

        case IOCTL_CAR6K_BMI_EXECUTE:
            {
                BMI_EXECUTE_PARAM *buffer;
                buffer = (BMI_EXECUTE_PARAM *)pInBuf;
                status = BMIExecute(hifDevice, buffer->address, (A_UINT32 *)&buffer->param);
            }
            break;

        case IOCTL_CAR6K_BMI_SET_APP_START:
            {
                BMI_SET_APP_START_PARAM *buffer;
                buffer = (BMI_SET_APP_START_PARAM *)pInBuf;
                status = BMISetAppStart(hifDevice, buffer->address);
            }
            break;

        case IOCTL_CAR6K_BMI_READ_SOC_REGISTER:
            {
                BMI_READ_SOC_REG_PARAM *buffer;
                buffer = (BMI_READ_SOC_REG_PARAM *)pInBuf;
                status = BMIReadSOCRegister(hifDevice, buffer->address, (A_UINT32 *)&buffer->value);
            }
            break;

        case IOCTL_CAR6K_BMI_WRITE_SOC_REGISTER:
            {
                BMI_WRITE_SOC_REG_PARAM *buffer;
                buffer = (BMI_WRITE_SOC_REG_PARAM *)pInBuf;
                status = BMIWriteSOCRegister(hifDevice, buffer->address, buffer->param);
            }
            break;

        case IOCTL_CAR6K_BMI_ROMPATCH_INSTALL:
            {
                BMI_ROMPATCH_INSTALL_PARAM *buffer;
                buffer = (BMI_ROMPATCH_INSTALL_PARAM *)pInBuf;
                status = BMIrompatchInstall(hifDevice, buffer->romAddr, buffer->ramAddr, buffer->nBytes, buffer->doActivate, (A_UINT32*)&buffer->romPatchId );
            }
            break;

        case IOCTL_CAR6K_BMI_ROMPATCH_UNINSTALL:
            {
                BMI_ROMPATCH_UNINSTALL_PARAM *buffer;
                buffer = (BMI_ROMPATCH_UNINSTALL_PARAM *)pInBuf;
                status = BMIrompatchUninstall(hifDevice, buffer->romPatchId );
            }
            break;

        case IOCTL_CAR6K_BMI_ROMPATCH_ACTIVATE:
            {
                BMI_ROMPATCH_ACTIVATE_PARAM *buffer;
                buffer = (BMI_ROMPATCH_ACTIVATE_PARAM *)pInBuf;
                status = BMIrompatchActivate(hifDevice, buffer->romPatchCount, (A_UINT32*)&buffer->buffer);
            }
            break;

        case IOCTL_CAR6K_BMI_ROMPATCH_DEACTIVATE:
            {
                BMI_ROMPATCH_DEACTIVATE_PARAM *buffer;
                buffer = (BMI_ROMPATCH_DEACTIVATE_PARAM *)pInBuf;
                status = BMIrompatchDeactivate(hifDevice, buffer->romPatchCount, (A_UINT32 *)buffer->buffer );
            }
            break;

#ifdef HTC_RAW_INTERFACE
        case IOCTL_CAR6K_HTC_RAW_OPEN:
            if (!bHTCRawOpened) {

                // set htc block size
                status = ar6000_set_htc_params (hifDevice, stream_drv_t.targetType, 0, 0);
                if (status == A_OK )
                {
                    status = BMIDone (hifDevice);
                }
                if (status == A_OK )
                {
                   if (ar6000_htc_raw_open(htcTarget) == -1) {
                       status = A_ERROR;
                   }
                }
                if (status == A_OK)
                {
                    bHTCRawOpened = TRUE;
                }
            }
            break;

        case IOCTL_CAR6K_HTC_RAW_CLOSE:
            if (bHTCRawOpened) {
                if (ar6000_htc_raw_close(htcTarget) == -1) {
                    status = A_ERROR;
                } else {
                    bHTCRawOpened = FALSE;
                    status = A_OK;
                }
            }
            break;

        case IOCTL_CAR6K_HTC_RAW_READ:
            if (bHTCRawOpened) {
                A_INT32 bufLen=0;
                HTC_RAW_READ_PARAM *buffer;
                buffer = (HTC_RAW_READ_PARAM *)pInBuf;
                if ((bufLen = ar6000_htc_raw_read(htcTarget, (HTC_RAW_STREAM_ID)buffer->endPointId,
                        buffer->buffer, buffer->length)) != -1 ) {
                        //Copy the actual read no. of bytes @ the 4th byte pos in the buffer.
                        buffer->length = bufLen;
                    status = A_OK;
                } else {
                    status = A_ERROR;
                }
            }
            break;

        case IOCTL_CAR6K_HTC_RAW_WRITE:
            if (bHTCRawOpened)
            {
                HTC_RAW_WRITE_PARAM *buffer;
                buffer = (HTC_RAW_WRITE_PARAM *)pInBuf;
                if ( ar6000_htc_raw_write(htcTarget, (HTC_RAW_STREAM_ID)buffer->endPointId,
                     buffer->buffer, buffer->length) != -1) {
                    status = A_OK;
                } else {
                    status = A_ERROR;
                }
            }
            break;
#endif //HTC_RAW_INTERFACE

#ifdef HTC_EPPING_TEST
        case IOCTL_CAR6K_EP_PINGTEST:
            RunPingTest();
            break;
#endif
            
#ifdef HTC_EPPING_TEST
        case IOCTL_CAR6K_EPPING_REGISTER:
            SetupEpTest(htcTarget, stream_drv_t.targetType, stream_drv_t.targetId);
            break;
#endif

#ifdef WINCE_ART
        case IOCTL_CAR6K_ART_REGISTER:
            bHTCRawOpened = TRUE;
            bIsARTStarted = TRUE;
            ART_START();
            break;
#endif

        case IOCTL_CAR6K_GET_DBGLOG:
            status = drvDbgLogbuffer (OutBufSize, pOutBuf, pBytesReturned);
            break;
            
        case IOCTL_CAR6K_GRAB_DBGLOG:
            status = GetDbgLogs ();
            break;

        case IOCTL_CAR6K_MACADDR_UPDATE:
            eeprom_update_macaddr(hifDevice, stream_drv_t.targetType, (A_UCHAR *)pInBuf);
            status = A_OK;
            break;

        case IOCTL_CAR6K_RD_UPDATE:
            {
                A_UINT32 rd = 0;
                rd = *((A_UINT32 *)pInBuf);
                eeprom_update_rd(hifDevice, stream_drv_t.targetType, rd);
                status = A_OK;
            }
            break;

#ifdef AR6K_MTE_DRV_TEST
        case IOCTL_CAR6K_BUS_PASSTHRU_CMD:
        {
            extern A_BOOL ar6k_bus_cmd(void *pInput, int Length);
            
            if (!ar6k_bus_cmd(pInBuf,InBufSize)) {
                status = A_ERROR;    
            }
            
            break;    
        }
#endif

        case IOCTL_CAR6K_DUMP_CHIP_MEM:
        {
            A_UINT32 data = 0;
            A_UINT32 *address;
            address = (A_UINT32 *)pInBuf;
            ar6000_ReadRegDiag (hifDevice,address,&data);
            *(A_UINT32 *)pOutBuf = data;
            *pBytesReturned = sizeof (A_UINT32);
            status = A_OK;
        }
        break;

        default:
            status = A_ERROR;
            break;
    }

    return (status == A_OK) ? TRUE : FALSE;
}


extern
DWORD WINAPI DRG_Open(
    DWORD hDeviceContext,
    DWORD AccessCode,
    DWORD ShareMode)
{
    HTC_HANDLE      htcTarget;

    htcTarget = cmnGetHTCHandle();
    if (htcTarget == NULL) {
        return -1;
    }
    stream_drv_t.htcTarget = htcTarget;
    stream_drv_t.hifDevice = (HIF_DEVICE *)HTCGetHifDevice(htcTarget);
    cmnGetTargetInfo(&stream_drv_t.targetType,&stream_drv_t.targetId);

    return hDeviceContext;
}

extern
BOOL WINAPI DRG_Close(DWORD hOpenContext)
{
#ifdef WINCE_ART
    if (FALSE == bIsARTStarted)
    {
#endif
    stream_drv_t.htcTarget = NULL;
    stream_drv_t.hifDevice = NULL;
#ifdef WINCE_ART
    }
#endif
    return TRUE;
}

void DRG_Exit()
{
    NDIS_STATUS   ndisStatus;

    if (TRUE == bIsNdisRegistered)
    {
        NdisDeregisterAdapter (&ndisStatus, AR6K_ADAPTER_INSTANCE);
        bIsNdisRegistered = FALSE;
    }

#ifdef WINCE_ART
    if (TRUE == bIsARTStarted)
    {
        ART_STOP();
    }
#endif

    if (TRUE == bHTCRawOpened)
    {
        ar6000_htc_raw_close(stream_drv_t.htcTarget);
        bHTCRawOpened = FALSE;
    }

#ifdef HTC_EPPING_TEST
    CleanupEpTest();
#endif

    return;
}
