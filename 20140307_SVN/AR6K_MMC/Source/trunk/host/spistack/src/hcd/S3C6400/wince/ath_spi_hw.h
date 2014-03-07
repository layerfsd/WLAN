//------------------------------------------------------------------------------
// <copyright file="ath_spi_hw.h" company="Atheros">
//    Copyright (c) 2008 Atheros Corporation.  All rights reserved.
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

#ifndef ATH_SPI_HW_H_
#define ATH_SPI_HW_H_


typedef struct _SDHCD_HW_DEVICE {
    UINT8           InitStateMask;
#define SDHC_REGISTERED            0x10
#define SDHC_COMMON_INIT           0x40
#define SDHC_HW_INIT               0x80
    PSDHCD_DEVICE   pDevice;               /* back pointer to the common layer */
    BOOL            IrqEnabled;
    SDHCD_DEVICE    SpiCommon;             /* storage for the SPI common layer */
    PVOID           pWorker;
    CT_WORKER_TASK  IOCompleteWorkTask;    /* work task for deferred I/O completion */   
    HANDLE          hIstEventSPIGpioIRQ;   /* interrupt service event */
    HANDLE          hIstSPIGpioIRQ;        /* interrupt service thread */ 
    BOOL            ShutDown;              /* shutdown IST */
    DWORD           SysIntrSPIGpioIRQ;     /* system interrupt for GPIO interrupt */ 
    BOOL            CommonBufferDMA;       /* common buffer is used flag */ 
    UINT8           *pDmaCommonBuffer;     /* if common buffer is used, this is the buffer */
	UINT8           *pDmaCommonPhysicalBuffer;     /* if common buffer is used, this is the buffer */
	PVOID           pSpiContext;
}SDHCD_HW_DEVICE, *PSDHCD_HW_DEVICE;

#define GET_HW_DEVICE(pDevice) ((PSDHCD_HW_DEVICE)((pDevice)->pHWDevice))

SDHCD_HW_DEVICE *InitializeSPIHW(PTSTR pRegPath);
void CleanupSPIHW(SDHCD_HW_DEVICE *pHWDevice);

#endif /*ATH_SPI_HW_H_*/
