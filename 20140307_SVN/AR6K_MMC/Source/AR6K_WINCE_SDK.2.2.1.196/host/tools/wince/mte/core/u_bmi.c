/*++
Copyright (c) 2004-2008 Atheros Communications Inc.
All rights reserved.


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


 Simplified, single threaded (micro)implementation of BMI (for bootloaders)


--*/

#include "system.h"
#include "u_bmi.h"


#define COUNT_DEC_ADDRESS    0x00000440
#define BMI_COMMUNICATION_TIMEOUT       100000

/* ------ Global Variable Declarations ------- */
A_BOOL bmiDone = FALSE;

A_STATUS
bmiBufferSend(HIF_DEVICE *device,
              A_UCHAR *buffer,
              A_UINT32 length);

A_STATUS
bmiBufferReceive(HIF_DEVICE *device,
                 A_UCHAR *buffer,
                 A_UINT32 length);




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
        AR_DEBUG_PRINTF (ATH_DEBUG_BMI, ("BMIDone skipped\n"));
        return A_OK;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Done: Enter (device: 0x%p)\n", device));
    bmiDone = TRUE;
    cid = BMI_DONE;

    status = bmiBufferSend(device, (A_UCHAR *)&cid, sizeof(cid));
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
        return A_ERROR;
    }
    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Done: Exit\n"));

    return A_OK;
}

A_STATUS
BMIGetTargetInfo(HIF_DEVICE *device, struct bmi_target_info *targ_info)
{
    A_STATUS status;
    A_UINT32 cid;

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Get Target Info: Enter (device: 0x%p)\n", device));
    cid = BMI_GET_TARGET_INFO;

    status = bmiBufferSend(device, (A_UCHAR *)&cid, sizeof(cid));
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
        return A_ERROR;
    }

    status = bmiBufferReceive(device, (A_UCHAR *)&targ_info->target_ver,
                                                sizeof(targ_info->target_ver));
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to read Target Version from the device\n"));
        return A_ERROR;
    }

    if (targ_info->target_ver == TARGET_VERSION_SENTINAL) {
        /* Determine how many bytes are in the Target's targ_info */
        status = bmiBufferReceive(device, (A_UCHAR *)&targ_info->target_info_byte_count,
                                            sizeof(targ_info->target_info_byte_count));
        if (status != A_OK) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to read Target Info Byte Count from the device\n"));
            return A_ERROR;
        }

        /*
         * The Target's targ_info doesn't match the Host's targ_info.
         * We need to do some backwards compatibility work to make this OK.
         */
        A_ASSERT(targ_info->target_info_byte_count == sizeof(*targ_info));

        /* Read the remainder of the targ_info */
        status = bmiBufferReceive(device,
                        ((A_UCHAR *)targ_info)+sizeof(targ_info->target_info_byte_count),
                        sizeof(*targ_info)-sizeof(targ_info->target_info_byte_count));
        if (status != A_OK) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to read Target Info (%d bytes) from the device\n",
                                            targ_info->target_info_byte_count));
            return A_ERROR;
        }
    } else {
        /*
         * Target must be an AR6001 whose firmware does not
         * support BMI_GET_TARGET_INFO.  Construct the data
         * that it would have sent.
         */
        targ_info->target_info_byte_count=sizeof(*targ_info);
        targ_info->target_type=TARGET_TYPE_AR6001;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Get Target Info: Exit (ver: 0x%x type: 0x%x)\n",
                                    targ_info->target_ver, targ_info->target_type));

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
    static A_UCHAR data[BMI_DATASZ_MAX + sizeof(cid) + sizeof(address) + sizeof(length)];
    memset (&data, 0, BMI_DATASZ_MAX + sizeof(cid) + sizeof(address) + sizeof(length));

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
                ("BMI Read Memory: Enter (device: 0x%p, address: 0x%x, length: %d)\n",
                    device, address, length));

    cid = BMI_READ_MEMORY;

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
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
            return A_ERROR;
        }
        status = bmiBufferReceive(device, data, rxlen);
        if (status != A_OK) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to read from the device\n"));
            return A_ERROR;
        }
        A_MEMCPY(&buffer[length - remaining], data, rxlen);
        remaining -= rxlen; address += rxlen;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Read Memory: Exit\n"));
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
    const A_UINT32 header = sizeof(cid) + sizeof(address) + sizeof(length);
    static A_UCHAR data[BMI_DATASZ_MAX + sizeof(cid) + sizeof(address) + sizeof(length)];
    memset (&data, 0, BMI_DATASZ_MAX+header);

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
         ("BMI Write Memory: Enter (device: 0x%x, address: 0x%x, length: %d)\n",
         device, address, length));

    cid = BMI_WRITE_MEMORY;

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
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
            return A_ERROR;
        }
        remaining -= txlen; address += txlen;
    }
    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Write Memory: Exit\n"));

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
    static A_UCHAR data[sizeof(cid) + sizeof(address) + sizeof(*param)];
    memset (&data, 0, sizeof(cid) + sizeof(address) + sizeof(*param));

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
       ("BMI Execute: Enter (device: 0x%p, address: 0x%x, param: %d)\n",
        device, address, *param));

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
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
        return A_ERROR;
    }

    status = bmiBufferReceive(device, data, sizeof(*param));
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to read from the device\n"));
        return A_ERROR;
    }

    A_MEMCPY(param, data, sizeof(*param));

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Execute: Exit (param: %d)\n", *param));
    return A_OK;
}

A_STATUS
BMISetAppStart(HIF_DEVICE *device,
               A_UINT32 address)
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    static A_UCHAR data[sizeof(cid) + sizeof(address)];
    memset (&data, 0, sizeof(cid) + sizeof(address));

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
       ("BMI Set App Start: Enter (device: 0x%p, address: 0x%x)\n",
        device, address));

    cid = BMI_SET_APP_START;

    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &address, sizeof(address));
    offset += sizeof(address);
    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Set App Start: Exit\n"));
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
    static A_UCHAR data[sizeof(cid) + sizeof(address)];
    memset (&data, 0, sizeof(cid) + sizeof(address));

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
       ("BMI Read SOC Register: Enter (device: 0x%p, address: 0x%x)\n",
       device, address));

    cid = BMI_READ_SOC_REGISTER;

    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &address, sizeof(address));
    offset += sizeof(address);

    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
        return A_ERROR;
    }

    status = bmiBufferReceive(device, data, sizeof(*param));
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to read from the device\n"));
        return A_ERROR;
    }
    A_MEMCPY(param, data, sizeof(*param));

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Read SOC Register: Exit (value: %d)\n", *param));
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
    static A_UCHAR data[sizeof(cid) + sizeof(address) + sizeof(param)];

    memset (&data, 0, sizeof(cid) + sizeof(address) + sizeof(param));

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
     ("BMI Write SOC Register: Enter (device: 0x%p, address: 0x%x, param: %d)\n",
     device, address, param));

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
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Read SOC Register: Exit\n"));
    return A_OK;
}

A_STATUS
BMIrompatchInstall(HIF_DEVICE *device,
                   A_UINT32 ROM_addr,
                   A_UINT32 RAM_addr,
                   A_UINT32 nbytes,
                   A_UINT32 do_activate,
                   A_UINT32 *rompatch_id)
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    static A_UCHAR data[sizeof(cid) + sizeof(ROM_addr) + sizeof(RAM_addr) +
                                sizeof(nbytes) + sizeof(do_activate)];

    memset (&data, 0, sizeof(cid) + sizeof(ROM_addr) + sizeof(RAM_addr) +
                      sizeof(nbytes) + sizeof(do_activate));

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
         ("BMI rompatch Install: Enter (device: 0x%p, ROMaddr: 0x%x, RAMaddr: 0x%x length: %d activate: %d)\n",
         device, ROM_addr, RAM_addr, nbytes, do_activate));

    cid = BMI_ROMPATCH_INSTALL;

    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &ROM_addr, sizeof(ROM_addr));
    offset += sizeof(ROM_addr);
    A_MEMCPY(&data[offset], &RAM_addr, sizeof(RAM_addr));
    offset += sizeof(RAM_addr);
    A_MEMCPY(&data[offset], &nbytes, sizeof(nbytes));
    offset += sizeof(nbytes);
    A_MEMCPY(&data[offset], &do_activate, sizeof(do_activate));
    offset += sizeof(do_activate);
    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
        return A_ERROR;
    }

    status = bmiBufferReceive(device, (A_UCHAR *)rompatch_id, sizeof(*rompatch_id));
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to read from the device\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI rompatch Install: (rompatch_id=%d)\n", *rompatch_id));
    return A_OK;
}

A_STATUS
BMIrompatchUninstall(HIF_DEVICE *device,
                     A_UINT32 rompatch_id)
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    static A_UCHAR data[sizeof(cid) + sizeof(rompatch_id)];
    memset (&data, 0, sizeof(cid) + sizeof(rompatch_id));

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
         ("BMI rompatch Uninstall: Enter (device: 0x%p, rompatch_id: %d)\n",
                                         device, rompatch_id));

    cid = BMI_ROMPATCH_UNINSTALL;

    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &rompatch_id, sizeof(rompatch_id));
    offset += sizeof(rompatch_id);
    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI rompatch UNinstall: (rompatch_id=0x%x)\n", rompatch_id));
    return A_OK;
}

static A_STATUS
_BMIrompatchChangeActivation(HIF_DEVICE *device,
                             A_UINT32 rompatch_count,
                             A_UINT32 *rompatch_list,
                             A_UINT32 do_activate)
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    static A_UCHAR data[BMI_DATASZ_MAX + sizeof(cid) + sizeof(rompatch_count)];
    A_UINT32 length;

    memset (&data, 0, BMI_DATASZ_MAX + sizeof(cid) + sizeof(rompatch_count));

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
         ("BMI Change rompatch Activation: Enter (device: 0x%p, count: %d)\n",
           device, rompatch_count));

    cid = do_activate ? BMI_ROMPATCH_ACTIVATE : BMI_ROMPATCH_DEACTIVATE;

    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &rompatch_count, sizeof(rompatch_count));
    offset += sizeof(rompatch_count);
    length = rompatch_count * sizeof(*rompatch_list);
    A_MEMCPY(&data[offset], rompatch_list, length);
    offset += length;
    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Change rompatch Activation: Exit\n"));

    return A_OK;
}

A_STATUS
BMIrompatchActivate(HIF_DEVICE *device,
                    A_UINT32 rompatch_count,
                    A_UINT32 *rompatch_list)
{
    return _BMIrompatchChangeActivation(device, rompatch_count, rompatch_list, 1);
}

A_STATUS
BMIrompatchDeactivate(HIF_DEVICE *device,
                      A_UINT32 rompatch_count,
                      A_UINT32 *rompatch_list)
{
    return _BMIrompatchChangeActivation(device, rompatch_count, rompatch_list, 0);
}

A_STATUS
BMILZData(HIF_DEVICE *device,
          A_UCHAR *buffer,
          A_UINT32 length)
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    A_UINT32 remaining, txlen;
    const A_UINT32 header = sizeof(cid) + sizeof(length);
    static A_UCHAR data[BMI_DATASZ_MAX + sizeof(cid) + sizeof(length)];

    memset (&data, 0, BMI_DATASZ_MAX+header);

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
         ("BMI Send LZ Data: Enter (device: 0x%p, length: %d)\n",
         device, length));

    cid = BMI_LZ_DATA;

    remaining = length;
    while (remaining)
    {
        txlen = (remaining < (BMI_DATASZ_MAX - header)) ?
                                       remaining : (BMI_DATASZ_MAX - header);
        offset = 0;
        A_MEMCPY(&data[offset], &cid, sizeof(cid));
        offset += sizeof(cid);
        A_MEMCPY(&data[offset], &txlen, sizeof(txlen));
        offset += sizeof(txlen);
        A_MEMCPY(&data[offset], &buffer[length - remaining], txlen);
        offset += txlen;
        status = bmiBufferSend(device, data, offset);
        if (status != A_OK) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to write to the device\n"));
            return A_ERROR;
        }
        remaining -= txlen;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI LZ Data: Exit\n"));

    return A_OK;
}

A_STATUS
BMILZStreamStart(HIF_DEVICE *device,
                 A_UINT32 address)
{
    A_UINT32 cid;
    A_STATUS status;
    A_UINT32 offset;
    static A_UCHAR data[sizeof(cid) + sizeof(address)];

    if (bmiDone) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Command disallowed\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI,
         ("BMI LZ Stream Start: Enter (device: 0x%p, address: 0x%x)\n",
         device, address));

    cid = BMI_LZ_STREAM_START;
    offset = 0;
    A_MEMCPY(&data[offset], &cid, sizeof(cid));
    offset += sizeof(cid);
    A_MEMCPY(&data[offset], &address, sizeof(address));
    offset += sizeof(address);
    status = bmiBufferSend(device, data, offset);
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to Start LZ Stream to the device\n"));
        return A_ERROR;
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI LZ Stream Start: Exit\n"));

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
    static A_UINT32 cmdCredits;
    A_UINT32 mboxAddress[HTC_MAILBOX_NUM_MAX];

    HIFConfigureDevice(device, HIF_DEVICE_GET_MBOX_ADDR,
                       mboxAddress, sizeof(mboxAddress));

    cmdCredits = 0;
    timeout = BMI_COMMUNICATION_TIMEOUT;
    while(timeout-- && !cmdCredits) {
        /* Read the counter register to get the command credits */
        address = COUNT_DEC_ADDRESS + (HTC_MAILBOX_NUM_MAX + ENDPOINT1) * 4;
        /* hit the credit counter with a 4-byte access, the first byte read will hit the counter and cause
         * a decrement, while the remaining 3 bytes has no effect.  The rationale behind this is to
         * make all HIF accesses 4-byte aligned */
        status = HIFReadWrite(device, address, (A_UINT8 *)&cmdCredits, 4,
            HIF_RD_SYNC_BYTE_INC, NULL);
        if (status != A_OK) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to decrement the command credit count register\n"));
            return A_ERROR;
        }
        /* the counter is only 8=bits, ignore anything in the upper 3 bytes */
        cmdCredits &= 0xFF;
    }

    if (cmdCredits) {
        address = mboxAddress[ENDPOINT1];
        status = HIFReadWrite(device, address, buffer, length,
            HIF_WR_SYNC_BYTE_INC, NULL);
        if (status != A_OK) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to send the BMI data to the device\n"));
            return A_ERROR;
        }
    } else {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("BMI Communication timeout\n"));
        return A_ERROR;
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
    A_UINT32 mboxAddress[HTC_MAILBOX_NUM_MAX];

    HIFConfigureDevice(device, HIF_DEVICE_GET_MBOX_ADDR,
                       mboxAddress, sizeof(mboxAddress));

#define AVOID_BMI_READ_TIMEOUTS 0
#if AVOID_BMI_READ_TIMEOUTS
    /*
     * During normal bootups, small reads may be required.
     * Rather than issue an HIF Read and then wait as the Target adds
     * successive bytes to the FIFO, we wait here until the Target
     * has finished the entire response and has issued the next
     * BMI Command Credit.  At that point we read the full response
     * and continue.  This allows us to cleanly timeout on an unexpected
     * Target failure rather than risk problems at the HIF level.  In
     * particular, this avoids SDIO timeouts.  And on an interconnect
     * such as Compact Flash that does not provide any indication on
     * data timeout, it avoids a potential hang.
     *
     * This synchronization doesn't work for reads larger than the
     * size of the MBOX FIFO (128B), because the Target is unable
     * to push the 129th byte of data until AFTER an HIF Read is
     * posted. So for large reads we proceed to post the HIF Read
     * without this safety net.  Fortunately, large BMI reads do not
     * occur in practice -- they're supported for debug/development.
     *
     * This credit check is also useless when firmware chooses to
     * allow multiple outstanding BMI Command Credits.  To restrict
     * the Target to one BMI Command Credit, see HI_OPTION_BMI_CRED_LIMIT.
     *
     * For a more conservative Host implementation (which would be
     * safer for a Compact Flash interconnect):
     *   Set AVOID_BMI_READ_TIMEOUTS (above) to 1 and
     *   Set HI_OPTION_BMI_CRED_LIMIT and
     *   reduce BMI_DATASZ_MAX to 32 or 64
     */
    if (length < 128) { /* check against MBOX FIFO size */
        A_UINT32 timeout;
        static A_UINT32 cmdCredits;
        cmdCredits = 0;
        timeout = BMI_COMMUNICATION_TIMEOUT;
        while(timeout-- && !cmdCredits) {
            /* Read the counter register to get the command credits */
            address = COUNT_ADDRESS + (HTC_MAILBOX_NUM_MAX + ENDPOINT1) * 1;
            /* read the counter using a 4-byte read.  Since the counter is NOT auto-decrementing,
             * we can read this counter multiple times using a non-incrementing address mode.
             * The rationale here is to make all HIF accesses a multiple of 4 bytes */
            status = HIFReadWrite(device, address, (A_UINT8 *)&cmdCredits, sizeof(cmdCredits),
                HIF_RD_SYNC_BYTE_FIX, NULL);
            if (status != A_OK) {
                AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to read the command credit count register\n"));
                return A_ERROR;
            }
                /* we did a 4-byte read to the same count register so mask off upper bytes */
            cmdCredits &= 0xFF;
        }

        if (!cmdCredits) {
            AR_DEBUG_PRINTF(ATH_DEBUG_BMI, ("BMI Communication timeout\n"));
            return A_ERROR;
        }
    }
#endif

    address = mboxAddress[ENDPOINT1];
    status = HIFReadWrite(device, address, buffer, length, HIF_RD_SYNC_BYTE_INC, NULL);
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Unable to read the BMI data from the device\n"));
        return A_ERROR;
    }

    return A_OK;
}
