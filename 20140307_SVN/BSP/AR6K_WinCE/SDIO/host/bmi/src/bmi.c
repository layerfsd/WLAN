//------------------------------------------------------------------------------
// <copyright file="bmi.c" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// This file contains the routines that implement the Boot loader messaging 
// interface
//
// Author(s): ="Atheros"
//==============================================================================

#include "hif.h"
#include "bmi.h"
#include "htc.h"
#include "bmi_internal.h"
#include "osapi.h"


/* Although we had envisioned BMI to run on top of HTC, this is not what the final implementation boiled down to on dragon. Its a part of BSP and does not use the HTC protocol either. On the host side, however, we were still living with the original idea. I think the time has come to accept the truth and separate it from HTC which has been carrying BMI's burden all this while. It shall make HTC state machine relatively simpler */

/* ------ Static Variables ------ */

/* ------ Global Variable Declarations ------- */
A_BOOL bmiDone;

/* APIs visible to the driver */
void
BMIInit(void) 
{
    bmiDone = FALSE;
}

A_STATUS 
BMIDone(HIF_DEVICE *device) 
{
    A_STATUS status;
    A_UINT32 cid;

    if (bmiDone) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Command disallowed\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Done: Enter (device: 0x%p)\n", device);
    bmiDone = TRUE;
    cid = BMI_DONE;

    status = bmiBufferSend(device, (A_UCHAR *)&cid, sizeof(cid));
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to write to the device\n");
        return A_ERROR;
    }
    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Done: Exit\n");

    return A_OK;
}

A_STATUS 
BMIGetTargetId(HIF_DEVICE *device, A_UINT32 *id) 
{
    A_STATUS status;
    A_UINT32 cid;

    if (bmiDone) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Command disallowed\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Get Target ID: Enter (device: 0x%p)\n", device);
    cid = BMI_GET_TARGET_ID;

    status = bmiBufferSend(device, (A_UCHAR *)&cid, sizeof(cid));
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to write to the device\n");
        return A_ERROR;
    }

    status = bmiBufferReceive(device, (A_UCHAR *)id, sizeof(*id));
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to read from the device\n");
        return A_ERROR;
    }
    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Get Target ID: Exit (ID: 0x%x)\n", *id);

    return A_OK;
}

A_STATUS 
BMIReadMemory(HIF_DEVICE *device, 
              A_UINT32 address, 
              A_UCHAR *buffer, 
              A_UINT32 length) 
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    A_UINT32 remaining, rxlen;
    A_UCHAR data[BMI_DATASZ_MAX + sizeof(cid) + sizeof(address) + sizeof(length)];

    if (bmiDone) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Command disallowed\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,
       "BMI Read Memory: Enter (device: 0x%p, address: 0x%x, length: %d)\n", 
        device, address, length);

    cid = BMI_READ_MEMORY;

#ifdef ONLY_16BIT
    length = length + (length % 2);
#endif
    remaining = length;
    while (remaining)
    {
        rxlen = (remaining < BMI_DATASZ_MAX) ? remaining : BMI_DATASZ_MAX;
        offset = 0;
        A_MEMCPY(&data[offset], &cid, sizeof(cid));
        offset += sizeof(cid);
        A_MEMCPY(&data[offset], &address, sizeof(address));
        offset += sizeof(address);
        A_MEMCPY(&data[offset], &rxlen, sizeof(rxlen));
        offset += sizeof(length);
        status = bmiBufferSend(device, data, offset);
        if (status != A_OK) {
            BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to write to the device\n");
            return A_ERROR;
        }
        status = bmiBufferReceive(device, data, rxlen);
        if (status != A_OK) {
            BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to read from the device\n");
            return A_ERROR;
        }
        A_MEMCPY(&buffer[length - remaining], data, rxlen);
        remaining -= rxlen; address += rxlen;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Read Memory: Exit\n");
    return A_OK;
}

A_STATUS 
BMIWriteMemory(HIF_DEVICE *device, 
               A_UINT32 address, 
               A_UCHAR *buffer, 
               A_UINT32 length) 
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    A_UINT32 remaining, txlen;
    A_UCHAR data[BMI_DATASZ_MAX + sizeof(cid) + sizeof(address) + sizeof(length)];
	A_UINT32 header=sizeof(cid) + sizeof(address) + sizeof(length);

    if (bmiDone) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Command disallowed\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,
         "BMI Write Memory: Enter (device: 0x%p, address: 0x%x, length: %d)\n", 
          device, address, length);

    cid = BMI_WRITE_MEMORY;

#ifdef ONLY_16BIT
    length = length + (length % 2);
#endif
    remaining = length;
    while (remaining)
    {
        txlen = (remaining < (BMI_DATASZ_MAX - header)) ? 
                                       remaining : (BMI_DATASZ_MAX - header);
        offset = 0;
        A_MEMCPY(&data[offset], &cid, sizeof(cid));
        offset += sizeof(cid);
        A_MEMCPY(&data[offset], &address, sizeof(address));
        offset += sizeof(address);
        A_MEMCPY(&data[offset], &txlen, sizeof(txlen));
        offset += sizeof(txlen);
        A_MEMCPY(&data[offset], &buffer[length - remaining], txlen);
        offset += txlen;
        status = bmiBufferSend(device, data, offset);
        if (status != A_OK) {
            BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to write to the device\n");
            return A_ERROR;
        }
        remaining -= txlen; address += txlen;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Write Memory: Exit\n");

    return A_OK;
}

A_STATUS 
BMIExecute(HIF_DEVICE *device, 
           A_UINT32 address, 
           A_UINT32 *param) 
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    A_UCHAR data[sizeof(cid) + sizeof(address) + sizeof(*param)];

    if (bmiDone) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Command disallowed\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,
       "BMI Execute: Enter (device: 0x%p, address: 0x%x, param: %d)\n", 
        device, address, *param);

    cid = BMI_EXECUTE;

    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &address, sizeof(address));
    offset += sizeof(address);
    A_MEMCPY(&data[offset], param, sizeof(*param));
    offset += sizeof(*param);
    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to write to the device\n");
        return A_ERROR;
    }

    status = bmiBufferReceive(device, data, sizeof(*param));
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to read from the device\n");
        return A_ERROR;
    }

    A_MEMCPY(param, data, sizeof(*param));

    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Execute: Exit (param: %d)\n", *param);
    return A_OK;
}

A_STATUS 
BMISetAppStart(HIF_DEVICE *device, 
               A_UINT32 address)
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    A_UCHAR data[sizeof(cid) + sizeof(address)];

    if (bmiDone) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Command disallowed\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,
       "BMI Set App Start: Enter (device: 0x%p, address: 0x%x)\n", 
        device, address);

    cid = BMI_SET_APP_START;

    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &address, sizeof(address));
    offset += sizeof(address);
    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to write to the device\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Set App Start: Exit\n");
    return A_OK;
}

A_STATUS 
BMIReadSOCRegister(HIF_DEVICE *device, 
                   A_UINT32 address, 
                   A_UINT32 *param) 
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    A_UCHAR data[sizeof(cid) + sizeof(address)];

    if (bmiDone) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Command disallowed\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,
       "BMI Read SOC Register: Enter (device: 0x%p, address: 0x%x)\n", 
        device, address);

    cid = BMI_READ_SOC_REGISTER;

    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &address, sizeof(address));
    offset += sizeof(address);
    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to write to the device\n");
        return A_ERROR;
    }

    status = bmiBufferReceive(device, data, sizeof(*param));
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to read from the device\n");
        return A_ERROR;
    }
    A_MEMCPY(param, data, sizeof(*param));

    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Read SOC Register: Exit (value: %d)\n", *param);
    return A_OK;
}

A_STATUS 
BMIWriteSOCRegister(HIF_DEVICE *device, 
                    A_UINT32 address, 
                    A_UINT32 param) 
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    A_UCHAR data[sizeof(cid) + sizeof(address) + sizeof(param)];

    if (bmiDone) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Command disallowed\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,
     "BMI Write SOC Register: Enter (device: 0x%p, address: 0x%x, param: %d)\n",
      device, address, param);

    cid = BMI_WRITE_SOC_REGISTER;

    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &address, sizeof(address));
    offset += sizeof(address);
    A_MEMCPY(&data[offset], &param, sizeof(param));
    offset += sizeof(param);
    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to write to the device\n");
        return A_ERROR;
    }

    BMI_DEBUG_PRINTF(ATH_LOG_INF,"BMI Read SOC Register: Exit\n");
    return A_OK;
}

/* BMI Access routines */
A_STATUS 
bmiBufferSend(HIF_DEVICE *device, 
              A_UCHAR *buffer, 
              A_UINT32 length) 
{
    A_STATUS status;
    A_UINT32 timeout;
    A_UINT32 address;
#ifdef ONLY_16BIT
    A_UINT16 cmdCredits;
#else
    A_UCHAR cmdCredits;
#endif
    HIF_REQUEST request;

    /* Read the counter register to get the command credits */
    HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, HIF_SYNCHRONOUS, 
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
    address = COUNT_DEC_ADDRESS + (HTC_MAILBOX_NUM_MAX + ENDPOINT1) * 4;
    status = HIFReadWrite(device, address, (A_UCHAR *)&cmdCredits, 
                            sizeof(cmdCredits), &request, NULL);
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to decrement the command credit count register\n");
        return A_ERROR;
    }

    timeout = BMI_COMMUNICATION_TIMEOUT;
    while(timeout--) {
        if (cmdCredits == 1) {
            HIF_FRAME_REQUEST(&request, HIF_WRITE, HIF_EXTENDED_IO, 
                              HIF_SYNCHRONOUS, HIF_BYTE_BASIS, 
                              HIF_INCREMENTAL_ADDRESS);
            address = HIF_MBOX_START_ADDR(ENDPOINT1);
            status = HIFReadWrite(device, address, buffer, length, 
                                  &request, NULL);
            if (status != A_OK) {
                BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to send the BMI data to the device\n");
                return A_ERROR;
            }
            break;
        }

        HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, HIF_SYNCHRONOUS, 
                          HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
        address = COUNT_DEC_ADDRESS + (HTC_MAILBOX_NUM_MAX + ENDPOINT1) * 4;
        status = HIFReadWrite(device, address, (A_UCHAR *)&cmdCredits,
                sizeof(cmdCredits), &request, NULL);
        if (status != A_OK) {
            BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to decrement the command credit count register\n");
            return A_ERROR;
        }
        status = A_ERROR;
        A_MDELAY(1);
    }

    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"BMI Communication timeout\n");
    }

    return status;
}

A_STATUS 
bmiBufferReceive(HIF_DEVICE *device, 
                 A_UCHAR *buffer, 
                 A_UINT32 length) 
{
    A_STATUS status;
    A_UINT32 address;
    A_UINT32 timeout;
#ifdef ONLY_16BIT
    A_UINT16 cmdCredits;
#else
    A_UCHAR cmdCredits;
#endif
    HIF_REQUEST request;

    HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, HIF_SYNCHRONOUS, 
                      HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
    address = COUNT_ADDRESS + (HTC_MAILBOX_NUM_MAX + ENDPOINT1) * 1;
    status = HIFReadWrite(device, address, (A_UCHAR *)&cmdCredits, 
                    sizeof(cmdCredits), &request, NULL);
    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to decrement the command credit count register\n");
        return A_ERROR;
    }

    timeout = BMI_COMMUNICATION_TIMEOUT;
    while(timeout--) {
        if (cmdCredits == 1) {
            HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, 
                              HIF_SYNCHRONOUS, HIF_BYTE_BASIS, 
                              HIF_FIXED_ADDRESS);
            address = HIF_MBOX_END_ADDR(ENDPOINT1);
            status = HIFReadWrite(device, address, buffer, length, 
                                  &request, NULL);
            if (status != A_OK) {
                BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to read the BMI data from the device\n");
                return A_ERROR;
            }
            break;
        }

        HIF_FRAME_REQUEST(&request, HIF_READ, HIF_EXTENDED_IO, HIF_SYNCHRONOUS, 
                          HIF_BYTE_BASIS, HIF_FIXED_ADDRESS);
        address = COUNT_ADDRESS + (HTC_MAILBOX_NUM_MAX + ENDPOINT1) * 1;
        status = HIFReadWrite(device, address, (A_UCHAR *)&cmdCredits, 
                    sizeof(cmdCredits), &request, NULL);
        if (status != A_OK) {
            BMI_DEBUG_PRINTF(ATH_LOG_ERR,"Unable to decrement the command credit count register\n");
            return A_ERROR;
        }
        status = A_ERROR;
        A_MDELAY(1);
    }

    if (status != A_OK) {
        BMI_DEBUG_PRINTF(ATH_LOG_ERR,"BMI Communication timeout\n");
    }

    return status;
}
