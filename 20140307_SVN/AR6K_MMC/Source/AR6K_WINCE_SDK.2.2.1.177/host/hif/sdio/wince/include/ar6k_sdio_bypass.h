//------------------------------------------------------------------------------
// <copyright file="ar6k_sdio_bypass.h" company="Atheros">
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
// SDIO stack bypass definitions
//
// Author(s): ="Atheros"
//==============================================================================

/***
 *
 *  Atheros SDIO Stack bypass mode:
 *
 *  The sdio stack bypass mode requires cooperation with the underlying host controller driver (HCD).
 *  This mode provides direct access to the HCD from the client driver with minimal code impact to
 *  the HCD.
 *
 *  The architecture is as follows:
 *
 *       -----------------
 *       | Client Driver |<-----
 *       -----------------     |
 *               |             |
 *      -------------------    |
 *      | SDIO Bus Driver |    | <-- bypass
 *      -------------------    |
 *               |             |
 *       -----------------     |
 *       |      HCD      |<----|
 *       -----------------
 *
 *  The bypass mode is not activated until the client driver has been fully enumerated and initialized.
 *  The client driver switches to bypass mode by issuing a device IOCTL to the HCD's streams driver
 *  interface (all HCD's have a streams driver interface). The IOCTL indicates to the HCD driver
 *  which slot is operating in bypass mode.  The IOCTL also provides a set of callback pointers that
 *  are "replacement" functions for the standard SDIO bus indications (see AR6K_SDHC_API_INTERFACE_FUNCS
 *  below).  The host controller driver also returns the PSDCARD_HC_CONTEXT pointer for the slot
 *  that is operating in bypass mode (see AR6K_SDIO_BYPASS_INFO_OUTPUT) as well as the standard SDIO indication
 *  functions. The later is used to pass certain indications up to the bus driver on behalf of the HCD.
 *
 *  When operating in bypass mode, the host controller driver must use the replacement indications
 *  instead of calling the SDIO bus driver directly. This header file provides convenient macros
 *  for the HCD to manage the indications dynamically.
 *
 *  The client driver uses the host controller context pointer to issue bus requests and configuration
 *  requests directly to the HCD.  For all intents and purposes, the HCD can behave as if the bus
 *  and configuration requests came from the SDIO bus driver.  However these requests are being managed
 *  by the client driver itself (acting as a surrogate busdriver).
 *
 *  The bypass feature checks for the following registry keys:
 *
 *  [HKEY_LOCAL_MACHINE\Comm\AR6K_SD1\Parms\HIF\SDIO]
 *      "HCD_Bypass_Name"="<stream device name, e.g. SDH1:>"
 *      "HCD_Bypass_Slot"=dword:<slot number : e.g. 0>           
 * 
 *  If these keys are not present, the bypass mode is not enabled.
 * 
 *  Optionally, the HCD can provide private IOCTL codes to enable/disable bypass mode, the default
 *  codes can be overridden using the following keys
 *
 *   "HCD_Bypass_EnableIoctl"=dword:<hcd specific ioctl code>        
 *   "HCD_Bypass_DisableIoctl"=dword:<hcd specific ioctl code>
 *  
 *
 *  Additional Optimizations:
 *
 *  The bypass mode SD_BUS_REQUEST->Flags field may contain the private SD_REQUEST_FLAGS_SHORT_DATA_TRANSFER
 *  flag.  When set this provides a *hint* to the HCD that this data transfer is short and can be polled
 *  instead of taking an interrupt.  The client driver marks small requests (such as interrupt status
 *  reads) as "short" because the target *should* be awake and the transfer is very small (< 32 bytes).
 *  This can improve performance by allowing the HCD to poll for completion and not take context switch penalty
 *  of an interrupt.  When using this mode, the HCD is free to return the completion status directly
 *  from the bus request handler instead of returning SD_API_STATUS_PENDING.
 *
 *
*/

#ifndef AR6K_SDIO_BYPASS_H_
#define AR6K_SDIO_BYPASS_H_

#include <winioctl.h>


#define HIF_BYPASS_HCD_NAME_KEY_VAL             TEXT("HCD_Bypass_Name")
#define HIF_BYPASS_HCD_SLOT_KEY_VAL             TEXT("HCD_Bypass_Slot")
#define HIF_BYPASS_HCD_ENABLE_IOCTL_KEY_VAL     TEXT("HCD_Bypass_EnableIoctl")
#define HIF_BYPASS_HCD_DISABLE_IOCTL_KEY_VAL    TEXT("HCD_Bypass_DisableIoctl")

#define AR6K_IOCTL_CODE_DEVICE_TYPE 2555

/* We define some flags using unused bits in the SD_BUS_REQUEST.Flags field: */

#define SD_REQUEST_FLAGS_BYPASS                0x80000000   /* this request is a bypass request */
#define SD_REQUEST_FLAGS_SHORT_DATA_TRANSFER   0x40000000   /* the data transfer can be polled */

/* bypass interface pointers (emulates SDIO bus driver indication APIs) */
typedef struct _AR6K_SDHC_API_INTERFACE_FUNCS {
    VOID (*IndicateSlotStateChange)(PSDCARD_HC_CONTEXT pHcd,
                                    DWORD              SlotNumber,
                                    SD_SLOT_EVENT      SlotEvent);
    VOID (*IndicateBusRequestComplete)(PSDCARD_HC_CONTEXT pHcd,
                                       PSD_BUS_REQUEST    pRequest,
                                       SD_API_STATUS      Status);
    VOID (*UnlockRequest)(PSDCARD_HC_CONTEXT  pHcd,
                          PSD_BUS_REQUEST     pRequest);
    PSD_BUS_REQUEST (*GetAndLockCurrentRequest)(PSDCARD_HC_CONTEXT pHcd,
                                                DWORD              SlotIndex);
} AR6K_SDHC_API_INTERFACE_FUNCS;

/* information for the host controller to setup bypass mode */
typedef struct _AR6K_SDIO_BYPASS_INFO_INPUT {
    DWORD                        SlotNumber;             /* slot number to activate bypass */
    /* SDHC interface replacement callbacks
     * the host controller driver should use these functions to replace
     * the normal SDBUS calls */
   AR6K_SDHC_API_INTERFACE_FUNCS ByPassFuncs;
} AR6K_SDIO_BYPASS_INFO_INPUT;

/* information from the host controller to the AR6K driver to
 * perform bypass mode operation */
typedef struct _AR6K_SDIO_BYPASS_INFO_OUTPUT {
    PSDCARD_HC_CONTEXT  pHcd;         /* host controller context */
        /* host controller must pass back the original functions here */
    AR6K_SDHC_API_INTERFACE_FUNCS SDBusApis;
} AR6K_SDIO_BYPASS_INFO_OUTPUT;


/*  Enable SDIO bypass mode in host controller
 *  INPUT : AR6K_SDIO_BYPASS_INFO_INPUT  struct
 *  OUTPUT: AR6K_SDIO_BYPASS_INFO_OUTPUT struct
 */
#define IOCTL_AR6K_SDIO_BYPASS_ENABLE  CTL_CODE(AR6K_IOCTL_CODE_DEVICE_TYPE, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)

/*  Disable SDIO bypass mode in host controller
 *  INPUT : DWORD (slot number)
 *  OUTPUT: NONE
 */
#define IOCTL_AR6K_SDIO_BYPASS_DISABLE CTL_CODE(AR6K_IOCTL_CODE_DEVICE_TYPE, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)


#ifdef AR6K_BYPASS_MODE

/* useful macros for the host controller to manage bypass mode */

/* declare bypass mode state variables */
#define DECLARE_AR6K_BYPASS_MODE_INFO()             \
    PSDCARD_HC_CONTEXT          g_BypassHc = NULL;  \
    AR6K_SDIO_BYPASS_INFO_INPUT g_HcdBypassInfo


extern PSDCARD_HC_CONTEXT          g_BypassHc;
extern AR6K_SDIO_BYPASS_INFO_INPUT g_HcdBypassInfo;


#define AR6K_SDIO_GET_BYPASS_INPUT_INFO(pInput, dwLenIn, pHcdContext)            \
{                                                                                \
     memcpy(&g_HcdBypassInfo,(pBufIn),(dwLenIn));                                \
     g_BypassHc = pHcdContext;                                                   \
}

#define AR6K_SDIO_GET_BYPASS_SLOT_NUMBER() g_HcdBypassInfo.SlotNumber


#define AR6K_SDIO_SET_BYPASS_OUTPUT_INFO(pOutput, pHcdContext)                   \
{                                                                                \
    (pOutput)->pHcd = (pHcdContext);                                             \
    (pOutput)->SDBusApis.IndicateSlotStateChange = SDHCDIndicateSlotStateChange; \
}

#define AR6K_SDIO_DISABLE_BYPASS() \
    { g_BypassHc = NULL; g_HcdBypassInfo.SlotNumber = -1; }

#define REDIRECT_SLOT_STATE_CHANGE(pHc,slot,state)          \
{                                                           \
    if (g_HcdBypassInfo.SlotNumber == (slot)) {                              \
        g_HcdBypassInfo.ByPassFuncs.IndicateSlotStateChange((pHc),(slot),(state)); \
    } else {                                                \
        SDHCDIndicateSlotStateChange((pHc),(slot),(state)); \
    }                                                       \
}

#define REDIRECT_BUS_COMPLETE(pHc,slot,pReq,status)              \
{                                                                \
    if (g_HcdBypassInfo.SlotNumber == (slot)) {                              \
        g_HcdBypassInfo.ByPassFuncs.IndicateBusRequestComplete((pHc),(pReq),(status)); \
    } else {                                                \
        SDHCDIndicateBusRequestComplete((pHc),(pReq),(status)); \
    }                                                       \
}

#define REDIRECT_UNLOCK_REQUEST(pHc,slot,pReq)                    \
{                                                            \
    if (g_HcdBypassInfo.SlotNumber == (slot)) {                               \
        g_HcdBypassInfo.ByPassFuncs.UnlockRequest((pHc),(pReq)); \
    } else {                                                 \
        SDHCDUnlockRequest((pHc),(pReq));                    \
    }                                                        \
}

#define REDIRECT_GET_AND_LOCK_CURRENT_REQ(pReq,pHc,slot)     \
{                                                            \
    if (g_HcdBypassInfo.SlotNumber == (slot)) {                               \
        (pReq) = g_HcdBypassInfo.ByPassFuncs.GetAndLockCurrentRequest((pHc),(slot)); \
    } else {                                                 \
        (pReq) = SDHCDGetAndLockCurrentRequest((pHc),(slot)); \
    }                                                        \
}

#define IS_BYPASS_REQUEST(pReq) ((pReq)->Flags & SD_REQUEST_FLAGS_BYPASS)

#define CHECK_BYPASS_MODE(slot)  (g_HcdBypassInfo.SlotNumber == (slot))

#define IS_SHORT_DATA_REQUEST(pReq) ((pReq)->Flags & SD_REQUEST_FLAGS_SHORT_DATA_TRANSFER)

#else

#define DECLARE_AR6K_BYPASS_MODE_INFO()

#define REDIRECT_SLOT_STATE_CHANGE(pHc,slot,state) SDHCDIndicateSlotStateChange((pHc),(slot),(state))

#define REDIRECT_BUS_COMPLETE(pHc,pReq,status)  SDHCDIndicateBusRequestComplete((pHc),(pReq),(status))

#define REDIRECT_UNLOCK_REQUEST(pHc,pReq) SDHCDUnlockRequest((pHc),(pReq))

#define REDIRECT_GET_AND_LOCK_CURRENT_REQ(pReq,pHc,slot) (pReq) = SDHCDGetAndLockCurrentRequest((pHc),(slot))

#define IS_BYPASS_REQUEST(pReq)     FALSE

#define CHECK_BYPASS_MODE(slot)      FALSE

#define IS_SHORT_DATA_REQUEST(pReq) FALSE

#define AR6K_SDIO_GET_BYPASS_INPUT_INFO(pInput, dwLenIn, pHcdContext)

#define AR6K_SDIO_SET_BYPASS_OUTPUT_INFO(pOutput, pHcdContext)

#define AR6K_SDIO_GET_BYPASS_SLOT_NUMBER()  ((DWORD)-1)

#define AR6K_SDIO_DISABLE_BYPASS()

#endif

#endif /*AR6K_SDIO_BYPASS_H_*/
